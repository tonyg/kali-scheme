/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/


/* Implementation of the vm-extension opcode.  This is completetly
   optional; nothing in the standard system uses these features.
   If you have ANSI C but not POSIX support, try compiling with -DPOSIX=0.

   If you want to add features, please send mail to
   scheme-48-request@martigny.ai.mit.edu to obtain a range of unique
   keys for your own use.  Specify how many cases you will need.  This
   will ensure that these numbers won't be allocated for some
   different purpose in a future release of Scheme 48.

   fdopen: POSIX.1
   getenv: POSIX.1, ANSI C
   setuid, setgid: POSIX.1
   popen: POSIX.2
   floating point: POSIX.1, ANSI C (should we be linking with -lM or -lm?)
   sprintf: POSIX.1, ANSI C
   atof: POSIX.1, ANSI C
   chroot: not standard

 */

#ifndef POSIX
#  define POSIX 2
#endif

#include <stdio.h>
#include "sysdep.h"
#include "scheme48.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>		/* setuid & setgid */

/* #include <fcntl.h>		/* for O_RDWR */

#define GREATEST_FIXNUM_VALUE ((1 << 29) - 1)
#define LEAST_FIXNUM_VALUE (-1 << 29)
#define PORT_INDEX(x) EXTRACT_FIXNUM(STOB_REF(x, 1))
#define FOR_INPUT 1
#define FOR_OUTPUT 2

FILE **port_to_stream(scheme_value);

typedef struct {
  char b[sizeof(double)];
} unaligned_double;

typedef union {
  double f;
  unaligned_double b;
} float_or_bytes;


/******************************************/

scheme_value
extended_vm (long key, scheme_value value)
{
  double x, y;

  switch (key) {
  
    /* Cases 0 through 19 are reserved for the mobot system. */

  case 0:			/* read jumpers on 68000 board */
    return ENTER_FIXNUM(0);

#if defined(SOCKET_SUPPORT)
  case 20:
    { extern int internet_stream_socket();
      int s = internet_stream_socket();
      return (s < 0) ? UNDEFINED : ENTER_FIXNUM(s);
    }

  case 21:
    { extern int socket_bind(int, int);
      int port = socket_bind(EXTRACT_FIXNUM(value), 0);
      return (port < 0) ? UNDEFINED : ENTER_FIXNUM(port);
    }

  case 22:
    { extern int socket_accept(int);
      int fd = socket_accept(EXTRACT_FIXNUM(value));
      return (fd < 0) ? UNDEFINED : ENTER_FIXNUM(fd);
    }

  case 23:
    if (!PAIRP(value)) return UNDEFINED;
    { extern int socket_connect(char *, int);
      char *hostname = &STRING_REF(CAR(value), 0);
      long hostname_length = STRING_LENGTH(CAR(value));
      long port = EXTRACT_FIXNUM(CDR(value));
      int fd;
      if (hostname[hostname_length] != '\0')
	return UNDEFINED;
      fd = socket_connect(hostname, port);
      return (fd < 0) ? UNDEFINED : ENTER_FIXNUM(fd);
    }
#endif

#if POSIX
  /* fdopen() support */
  case 24:
  case 25:
    if (!PAIRP(value)) return UNDEFINED;
    { scheme_value port = CAR(value);
      long fd = EXTRACT_FIXNUM(CDR(value));
      FILE **pstream = port_to_stream(port);
      FILE *new_stream;

      if (pstream == NULL) return UNDEFINED;
      new_stream = fdopen(fd, key == 24 ? "r" : "w");
      if (new_stream == NULL) return UNDEFINED;
      fclose(*pstream);
      *pstream = new_stream;
      return UNSPECIFIC;
    }
#endif /* POSIX */

  /* getenv() */
  case 26: {
    scheme_value env_var, result_buffer;
    char *result;
    size_t result_len;
    
    if (!PAIRP(value)) return UNDEFINED;
    env_var = CAR(value);
    result_buffer = CDR(value);
    if (!STRINGP(env_var) || !STRINGP(result_buffer)) return UNDEFINED;
    result = getenv(&STRING_REF(env_var, 0));
    if (result == NULL)
      return SCHFALSE;
    result_len = strlen(result);
    if (result_len > STRING_LENGTH(result_buffer))
      return UNDEFINED;
    strncpy(&STRING_REF(result_buffer, 0), result, result_len);
    return ENTER_FIXNUM(result_len);
  }

#if POSIX
  case 27: {
    /* This is intended for use by HTTP scripts... */
    if (!PAIRP(value) || !FIXNUMP(CAR(value)) || !FIXNUMP(CDR(value)))
      return UNDEFINED;
    if (setgid(EXTRACT_FIXNUM(CDR(value))) != 0) {
      perror("setgid");
      return SCHFALSE; }
    if (setuid(EXTRACT_FIXNUM(CAR(value))) != 0) {
      perror("setuid");
      return SCHFALSE; }
    else
      return SCHTRUE;
  }
#endif

#if defined(HAVE_CHROOT)
  case 28: {
    if (!STRINGP(value))
      return UNDEFINED;
    else if (chroot(&STRING_REF(value, 0)) != 0) {
      perror("chroot");
      return SCHFALSE; }
    else
      return SCHTRUE;
  }
#endif

#if POSIX >= 2
  case 96: {
    int status;
    if (!STRINGP(value))
      return UNDEFINED;
    status = system(&STRING_REF(value, 0));
    if (status == -1) {
      perror("chroot");
      return UNDEFINED; }
    else
      return ENTER_FIXNUM(status); /* cf. waitpid() */
  }

  /* popen() support.  Rather kludgey; there's no pclose(), so
     zombies will pile up. */
  case 97:
  case 98: {
    if (!PAIRP(value)) return UNDEFINED;
    { scheme_value port = CAR(value);
      FILE **pstream = port_to_stream(port);
      scheme_value command = CDR(value);
      FILE *new_stream;
      struct sigaction action;

      if (pstream == NULL) return UNDEFINED;

      action.sa_handler = SIG_IGN;
      action.sa_flags = 0;
      sigemptyset(&action.sa_mask);
      sigaction(SIGPIPE, &action, NULL);

      new_stream = popen(&STRING_REF(command, 0), key == 97 ? "r" : "w");
      if (new_stream == NULL) return UNDEFINED;
      fclose(*pstream);
      *pstream = new_stream;
      return SCHTRUE;
    }
  }
#endif /* POSIX.2 */


  /* Floating point */

#define FLOP 100
#define FLOP2(i) case FLOP+(i): \
		   if (!STOBP(value) || STOB_LLENGTH(value) != 2) \
		     return UNDEFINED;
#define FLOP3(i) case FLOP+(i): \
		   if (!STOBP(value) || STOB_LLENGTH(value) != 3) \
		     return UNDEFINED;

#define get_arg(args,i) STOB_REF(args,(i))
#define get_string_arg(args,i) (&STRING_REF(get_arg(args,i), 0))

#define get_float_arg(args, i, var) EXTRACT_FLOAT(get_arg(args, i), var)
#define set_float_arg(args, i, val) SET_FLOAT(get_arg(args, i), val)

#define EXTRACT_FLOAT(stob, var) \
  { scheme_value temp_ = (stob); \
    float_or_bytes loser_; \
    if (!STOBP(temp_)) return UNDEFINED; \
    loser_.b = *(unaligned_double*)(&STOB_REF(temp_, 0)); \
    (var) = loser_.f; }

#define SET_FLOAT(stob, val) \
  { scheme_value temp_ = (stob); \
    float_or_bytes loser_; \
    if (!STOBP(temp_)) return UNDEFINED; \
    loser_.f = (double)(val); \
    *(unaligned_double*)(&STOB_REF(temp_, 0)) = loser_.b; }

  FLOP3(0) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x + y);
    return UNSPECIFIC;}
  FLOP3(1) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x - y);
    return UNSPECIFIC;}
  FLOP3(2) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x * y);
    return UNSPECIFIC;}
  FLOP3(3) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (y == 0.0) return UNDEFINED;
    set_float_arg(value, 2, x / y);
    return UNSPECIFIC;}
  FLOP2(4) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    return ENTER_BOOLEAN(x == y);}
  FLOP2(5) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    return ENTER_BOOLEAN(x < y);}
  FLOP2(6) {			/* fixnum->float */
    scheme_value arg = get_arg(value, 0);
    if (!FIXNUMP(arg)) return SCHFALSE;
    set_float_arg(value, 1, EXTRACT_FIXNUM(arg));
    return SCHTRUE;}
  FLOP2(7) {			/* string->float */
    char *str = get_string_arg(value, 0);
    set_float_arg(value, 1, atof(str));
    return UNSPECIFIC;}
  FLOP2(8) {			/* float->string */
    size_t len;
    char *str = get_string_arg(value,1);
    get_float_arg(value, 0, x);
    sprintf(str, "%g", x);
    len = strlen(str);
    if (len > STRING_LENGTH(get_arg(value,1)))
      /* unlikely but catastrophic */
      fprintf(stderr, "printing float: output too long: %s\n",
	      str);
    return ENTER_FIXNUM(len);}

    /* exp log sin cos tan asin acos atan sqrt */

  FLOP2(9) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, exp(x));
    return UNSPECIFIC;}
  FLOP2(10) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, log(x));
    return UNSPECIFIC;}
  FLOP2(11) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, sin(x));
    return UNSPECIFIC;}
  FLOP2(12) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, cos(x));
    return UNSPECIFIC;}
  FLOP2(13) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, tan(x));
    return UNSPECIFIC;}
  FLOP2(14) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, asin(x));
    return UNSPECIFIC;}
  FLOP2(15) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, acos(x));
    return UNSPECIFIC;}
  FLOP3(16) {			/* atan */
    get_float_arg(value, 0, y);
    get_float_arg(value, 1, x);
    set_float_arg(value, 2, atan2(y, x));
    return UNSPECIFIC;}
  FLOP2(17) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, sqrt(x));
    return UNSPECIFIC;}

  FLOP2(18) {			/* floor */
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, floor(x));
    return UNSPECIFIC;}
  case FLOP+19: {		/* integer? */
    EXTRACT_FLOAT(value, x);
    return ENTER_BOOLEAN(fmod(x, 1.0) == 0.0); }
  case FLOP+20: {		/* float->fixnum */
    EXTRACT_FLOAT(value, x);
    if (x <= (double)GREATEST_FIXNUM_VALUE
	&& x >= (double)LEAST_FIXNUM_VALUE)
      return ENTER_FIXNUM((long)x);
    else
      return SCHFALSE;}
  FLOP3(21) {			/* quotient */
    double z;
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (fmod(x, 1.0) != 0.0 || fmod(y, 1.0) != 0.0) return UNDEFINED;
    if (y == 0.0) return UNDEFINED;
    z = x / y;
    set_float_arg(value, 2, z < 0.0 ? ceil(z) : floor(z));
    return UNSPECIFIC;}
  FLOP3(22) {			/* remainder */
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (fmod(x, 1.0) != 0.0 || fmod(y, 1.0) != 0.0) return UNDEFINED;
    if (y == 0.0) return UNDEFINED;

    /* "fmod(double x, double y) returns the floating-point remainder
       (f) of the division of x by y, where f has the same sign as x,
       such that x=iy+f for some integer i, and |f| < |y|." */

    set_float_arg(value, 2, fmod(x, y));
    return UNSPECIFIC;}

  default:
    return UNDEFINED;
  }
}


FILE **
port_to_stream(scheme_value port)
{
  int index;
  extern FILE **Sopen_portsS;

  if (!PORTP(port))
    return NULL;		/* not a port */

  index = PORT_INDEX(port);
  if (index < 0)
    return NULL;		/* port not open */

  return &Sopen_portsS[index];
}
