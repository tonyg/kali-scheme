/*Copyright 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.*/


/* Implementation of the vm-extension opcode.  This is completely
   optional; nothing in the standard system uses these features.
   If you have ANSI C but not POSIX support, try compiling with -DPOSIX=0.

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
#include "socket.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>		/* setuid & setgid */
#include <errno.h>
#include <netdb.h>		/* gethostbyname */  /* Kali code */

#include <sys/types.h>
#include <sys/wait.h>


#define GREATEST_FIXNUM_VALUE ((1 << 29) - 1)
#define LEAST_FIXNUM_VALUE (-1 << 29)
#define CHANNEL_INDEX(x) EXTRACT_FIXNUM(STOB_REF(x, 1))
#define FOR_INPUT 1
#define FOR_OUTPUT 2

typedef struct {
  char b[sizeof(double)];
} unaligned_double;

typedef union {
  double f;
  unaligned_double b;
} float_or_bytes;

extern long Sextension_valueS;  /* how values are returned */

/* return status values */
#define EXT_ST_OKAY 0
#define EXT_ST_EXCEPTION 1

#define EXT_RETURN(value) {Sextension_valueS = (value); return EXT_ST_OKAY; }
#define EXT_EXCEPTION     return EXT_ST_EXCEPTION

/******************************************/

scheme_value
extended_vm (long key, scheme_value value)
{
  double x, y;

  switch (key) {

    /* Cases 0 through 19 are reserved for the mobot system. */

  case 0:			/* read jumpers on 68000 board */
    EXT_RETURN(ENTER_FIXNUM(0));

#if defined(HAVE_SOCKET)
  case 20:
    { int s = internet_stream_socket();
      if (s < 0)
	EXT_EXCEPTION;
      else
	EXT_RETURN(ENTER_FIXNUM(s));
    }

  case 21:
    { int sock, port;

      if (!PAIRP(value) || !FIXNUMP(CAR(value)) || !FIXNUMP(CDR(value)))
	EXT_EXCEPTION;
      sock = EXTRACT_FIXNUM(CAR(value));
      port = EXTRACT_FIXNUM(CDR(value));
      port = socket_bind(sock, port);
      if (port < 0)
	EXT_EXCEPTION;
      else
	EXT_RETURN(ENTER_FIXNUM(port));
    }

  case 22:
    { int sock, fd;

      if (!FIXNUMP(value))
	EXT_EXCEPTION;

      sock = EXTRACT_FIXNUM(value);

      if (sock < 0)
	EXT_EXCEPTION;
      fd = socket_accept(sock);
      if (fd >= 0)
	EXT_RETURN(ENTER_FIXNUM(fd))
      else if (fd == -2)
	EXT_RETURN(ENTER_FIXNUM(-1))
      else
	EXT_EXCEPTION;
    }

  case 23: {
	int	sock;
	char	*mach;
	int	port,
		res;

	if (! (PAIRP(value) && FIXNUMP(CAR(value))
		&& PAIRP(CDR(value)) && STRINGP(CAR(CDR(value)))
		&& FIXNUMP(CDR(CDR(value)))))
	    EXT_EXCEPTION;
	sock = EXTRACT_FIXNUM(CAR(value));
	mach = &STRING_REF(CAR(CDR(value)), 0);
	port = EXTRACT_FIXNUM(CDR(CDR(value)));
	res = socket_connect(sock, mach, port);
	if (res >= 0)
		EXT_RETURN(ENTER_FIXNUM(res))
	else if (res == -2)
		EXT_RETURN(ENTER_FIXNUM(-1))
	else
		EXT_EXCEPTION;
    }

#endif

  /* getenv() */
  case 26: {
    scheme_value env_var, result_buffer;
    char *result;
    size_t result_len;

    if (!PAIRP(value)) EXT_EXCEPTION;
    env_var = CAR(value);
    result_buffer = CDR(value);
    if (!STRINGP(env_var) || !STRINGP(result_buffer)) EXT_EXCEPTION;
    result = getenv(&STRING_REF(env_var, 0));
    if (result == NULL)
      EXT_RETURN(SCHFALSE);
    result_len = strlen(result);
    if (result_len > STRING_LENGTH(result_buffer))
      EXT_EXCEPTION;
    strncpy(&STRING_REF(result_buffer, 0), result, result_len);
    EXT_RETURN(ENTER_FIXNUM(result_len));
  }

#if POSIX
  case 27: {
    /* This is intended for use by HTTP scripts... */
    if (!PAIRP(value) || !FIXNUMP(CAR(value)) || !FIXNUMP(CDR(value)))
      EXT_EXCEPTION;
    if (setgid(EXTRACT_FIXNUM(CDR(value))) != 0) {
      perror("setgid");
      EXT_RETURN(SCHFALSE); }
    if (setuid(EXTRACT_FIXNUM(CAR(value))) != 0) {
      perror("setuid");
      EXT_RETURN(SCHFALSE); }
    else
      EXT_RETURN(SCHTRUE);
  }
#endif

#if defined(HAVE_CHROOT)
  case 28: {
    if (!STRINGP(value))
      EXT_EXCEPTION;
    else if (chroot(&STRING_REF(value, 0)) != 0) {
      perror("chroot");
      EXT_RETURN(SCHFALSE); }
    else
      EXT_RETURN(SCHTRUE);
  }
#endif

  /* dup() support */
  case 29:
    { long fd = dup(EXTRACT_FIXNUM(value));
      if (fd < 0)
	EXT_EXCEPTION;
      else
	EXT_RETURN(ENTER_FIXNUM(fd));
    }

  /* extract file descriptor from channel */
  case 30:
    { int fd;

      if (!FIXNUMP(value))
	EXT_EXCEPTION;

      fd = EXTRACT_FIXNUM(value);

      if (fd < 0)
	EXT_EXCEPTION;
      else
	EXT_RETURN(ENTER_FIXNUM(fd));
    }

    /* system() is supposedly POSIX.2 and ANSI C */

#if POSIX >= 2
  case 96: {
    int status;
    if (!STRINGP(value))
      EXT_EXCEPTION;
    status = system(&STRING_REF(value, 0));
    if (status == -1) {
      perror("chroot");
      EXT_EXCEPTION; }
    else
      EXT_RETURN(ENTER_FIXNUM(status)); /* cf. waitpid() */
  }
#endif /* POSIX.2 */


  /* Floating point */

#define FLOP 100
#define FLOP2(i) case FLOP+(i): \
		   if (!STOBP(value) || STOB_LLENGTH(value) != 2) \
		     EXT_EXCEPTION;
#define FLOP3(i) case FLOP+(i): \
		   if (!STOBP(value) || STOB_LLENGTH(value) != 3) \
		     EXT_EXCEPTION;

#define get_arg(args,i) STOB_REF(args,(i))
#define get_string_arg(args,i) (&STRING_REF(get_arg(args,i), 0))

#define get_float_arg(args, i, var) EXTRACT_FLOAT(get_arg(args, i), var)
#define set_float_arg(args, i, val) SET_FLOAT(get_arg(args, i), val)

#define EXTRACT_FLOAT(stob, var) \
  { scheme_value temp_ = (stob); \
    float_or_bytes loser_; \
    if (!STOBP(temp_)) EXT_EXCEPTION; \
    loser_.b = *(unaligned_double*)(&STOB_REF(temp_, 0)); \
    (var) = loser_.f; }

#define SET_FLOAT(stob, val) \
  { scheme_value temp_ = (stob); \
    float_or_bytes loser_; \
    if (!STOBP(temp_)) EXT_EXCEPTION; \
    loser_.f = (double)(val); \
    *(unaligned_double*)(&STOB_REF(temp_, 0)) = loser_.b; }

  FLOP3(0) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x + y);
    EXT_RETURN(UNSPECIFIC);}
  FLOP3(1) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x - y);
    EXT_RETURN(UNSPECIFIC);}
  FLOP3(2) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x * y);
    EXT_RETURN(UNSPECIFIC);}
  FLOP3(3) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (y == 0.0) EXT_EXCEPTION;
    set_float_arg(value, 2, x / y);
    EXT_RETURN(UNSPECIFIC);}
  FLOP2(4) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    EXT_RETURN(ENTER_BOOLEAN(x == y));}
  FLOP2(5) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    EXT_RETURN(ENTER_BOOLEAN(x < y));}
  FLOP2(6) {			/* fixnum->float */
    scheme_value arg = get_arg(value, 0);
    if (!FIXNUMP(arg)) EXT_RETURN(SCHFALSE);
    set_float_arg(value, 1, EXTRACT_FIXNUM(arg));
    EXT_RETURN(SCHTRUE);}
  FLOP2(7) {			/* string->float */
    char *str = get_string_arg(value, 0);
    set_float_arg(value, 1, atof(str));
    EXT_RETURN(UNSPECIFIC);}
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
    EXT_RETURN(ENTER_FIXNUM(len));}

    /* exp log sin cos tan asin acos atan sqrt */

  FLOP2(9) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, exp(x));
    EXT_RETURN(UNSPECIFIC);}
  FLOP2(10) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, log(x));
    EXT_RETURN(UNSPECIFIC);}
  FLOP2(11) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, sin(x));
    EXT_RETURN(UNSPECIFIC);}
  FLOP2(12) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, cos(x));
    EXT_RETURN(UNSPECIFIC);}
  FLOP2(13) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, tan(x));
    EXT_RETURN(UNSPECIFIC);}
  FLOP2(14) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, asin(x));
    EXT_RETURN(UNSPECIFIC);}
  FLOP2(15) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, acos(x));
    EXT_RETURN(UNSPECIFIC);}
  FLOP3(16) {			/* atan */
    get_float_arg(value, 0, y);
    get_float_arg(value, 1, x);
    set_float_arg(value, 2, atan2(y, x));
    EXT_RETURN(UNSPECIFIC);}
  FLOP2(17) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, sqrt(x));
    EXT_RETURN(UNSPECIFIC);}

  FLOP2(18) {			/* floor */
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, floor(x));
    EXT_RETURN(UNSPECIFIC);}
  case FLOP+19: {		/* integer? */
    EXTRACT_FLOAT(value, x);
    EXT_RETURN(ENTER_BOOLEAN(fmod(x, 1.0) == 0.0)); }
  case FLOP+20: {		/* float->fixnum */
    EXTRACT_FLOAT(value, x);
    if (x <= (double)GREATEST_FIXNUM_VALUE
	&& x >= (double)LEAST_FIXNUM_VALUE)
      {
      EXT_RETURN(ENTER_FIXNUM((long)x)); }
    else
      EXT_RETURN(SCHFALSE);}
  FLOP3(21) {			/* quotient */
    double z;
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (fmod(x, 1.0) != 0.0 || fmod(y, 1.0) != 0.0) EXT_EXCEPTION;
    if (y == 0.0) EXT_EXCEPTION;
    z = x / y;
    set_float_arg(value, 2, z < 0.0 ? ceil(z) : floor(z));
    EXT_RETURN(UNSPECIFIC);}
  FLOP3(22) {			/* remainder */
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (fmod(x, 1.0) != 0.0 || fmod(y, 1.0) != 0.0) EXT_EXCEPTION;
    if (y == 0.0) EXT_EXCEPTION;

    /* "fmod(double x, double y) returns the floating-point remainder
       (f) of the division of x by y, where f has the same sign as x,
       such that x=iy+f for some integer i, and |f| < |y|." */

    set_float_arg(value, 2, fmod(x, y));
    EXT_RETURN(UNSPECIFIC);}

  default:
    EXT_EXCEPTION;
  }
}


/* --------------------------------------------------
   Entry points intended for use with (external-call ...).
   The actual arguments are argv[n-1], argv[n-2], ..., argv[0].
   For Unix system calls, a return value of #f means success, #t means
   invalid argument, and integer means Unixoid error.

   Missing, among others:
    chdir chmod mkdir rmdir stat fstat link unlink seek tell getpid getppid  
    environ
 */

/* (let ((results (cons 0 0)))
     (let ((errno (external-call s48-pipe results)))
       (if errno
           (error ... errno)
	   (values (car results) (cdr results))))) */

scheme_value s48_pipe(long argc, scheme_value *argv)
{
  int fd[2];
  scheme_value result;

  if (argc != 1) return SCHTRUE;
  result = argv[0];
  if (!PAIRP(result)) return SCHTRUE;

  if (pipe(&fd[0]) == -1) return ENTER_FIXNUM(errno);
  
  CAR(result) = ENTER_FIXNUM(fd[0]);
  CDR(result) = ENTER_FIXNUM(fd[1]);
  return SCHFALSE;
}

/* (let* ((results (cons 0 0))
	  (errno (external-call s48-waitpid results pid options)))
     (if errno
	 (interpret-syscall-error errno)
	 (values (car results) (cdr results)))) */

scheme_value s48_waitpid(long argc, scheme_value *argv)
{
  int pid, status;
  scheme_value result, pidf, optf;

  if (argc != 3) return SCHTRUE;
  result = argv[2];
  pidf = argv[1];
  optf = argv[0];
  if (!PAIRP(result) || !FIXNUMP(pidf) || !FIXNUMP(optf))
    return SCHTRUE;

  if ((pid = waitpid(EXTRACT_FIXNUM(pidf),
		     &status,
		     EXTRACT_FIXNUM(optf)))
      == -1)
    return ENTER_FIXNUM(errno);

  CAR(result) = ENTER_FIXNUM(pid);
  CDR(result) = ENTER_FIXNUM(status);
  return SCHFALSE;
}

/* (let ((results (cons 0 0)))
     (let ((errno (external-call s48-fork results)))
       (if errno
	   (decode-syscall-error errno)
	   (car results))))    ;child pid or zero   */

scheme_value s48_fork(long argc, scheme_value *argv)
{
  int pid;
  scheme_value result;

  if (argc != 1) return SCHTRUE;
  result = argv[0];
  if (!PAIRP(result)) return SCHTRUE;

  if ((pid = fork()) == -1)
    return ENTER_FIXNUM(errno);
  
  CAR(result) = ENTER_FIXNUM(pid);
  return SCHFALSE;
}

/* exit(status) */
scheme_value s48_exit(long argc, scheme_value *argv)
{
  exit(EXTRACT_FIXNUM(argv[0]));
}

/* close(fd) */
scheme_value s48_close(long argc, scheme_value *argv)
{
  scheme_value fd;

  if (argc != 1) return SCHTRUE;
  fd = argv[0];
  if (!FIXNUMP(fd)) return SCHTRUE;

  if (close(EXTRACT_FIXNUM(fd)) == -1)
    return ENTER_FIXNUM(errno);
  
  return SCHFALSE;
}

/* dup(fd) -> result */
scheme_value s48_dup(long argc, scheme_value *argv)
{
  scheme_value result, fd;
  int new;

  if (argc != 2) return SCHTRUE;
  result = argv[1];
  fd = argv[0];
  if (!FIXNUMP(fd) || !PAIRP(result)) return SCHTRUE;

  if ((new = dup(EXTRACT_FIXNUM(fd))) == -1)
    return ENTER_FIXNUM(errno);
  
  CAR(result) = ENTER_FIXNUM(new);
  return SCHFALSE;
}

/* kill(pid, sig) */
scheme_value s48_kill(long argc, scheme_value *argv)
{
  scheme_value pid, sig;

  if (argc != 2) return SCHTRUE;
  pid = argv[1];
  sig = argv[0];
  if (!FIXNUMP(pid) || !FIXNUMP(sig)) return SCHTRUE;

  if (kill(EXTRACT_FIXNUM(pid), EXTRACT_FIXNUM(sig)) == -1)
    return ENTER_FIXNUM(errno);
  
  return SCHFALSE;
}


/* dup(fd) */

/* execv(path, argv) */
scheme_value s48_execv(long argc, scheme_value *argv)
{
  scheme_value prog, arg_vec, result;
  int unix_argc;
  char **unix_argv;

  if (argc != 2) return SCHTRUE;
  prog = argv[1];
  arg_vec = argv[0];
  if (!STRINGP(prog) || !VECTORP(arg_vec)) return SCHTRUE;
  unix_argc = VECTOR_LENGTH(arg_vec);

  unix_argv = malloc((argc+1) * sizeof(char *));

  { int i;
    for(i = 0; i < unix_argc; i++) {
      scheme_value arg = VECTOR_REF(arg_vec, i);
      if (!STRINGP(arg)) return SCHFALSE;
      unix_argv[i] = &STRING_REF(arg, 0);
    }
    unix_argv[i] = (char *)0;
  }

  if (execv(&STRING_REF(prog, 0), unix_argv) == -1)
    result = ENTER_FIXNUM(errno);
  result = SCHFALSE;
  free(unix_argv);
  return result;
}



/* (let ((s (make-string 100)))
     (substring s 0 (or (external-call s48_strerror s errno)
			(error "?"))))
 */

scheme_value s48_strerror(long argc, scheme_value *argv)
{
  scheme_value result, err;
  int result_len;
  char *err_string;

  if (argc != 2) return SCHFALSE;
  result = argv[1];
  err = argv[0];
  if (!STRINGP(result) || !FIXNUMP(err)) return SCHFALSE;

  err_string = strerror(EXTRACT_FIXNUM(err));
  result_len = strlen(err_string);
  if (result_len > STRING_LENGTH(result))
    return SCHFALSE;
  strncpy(&STRING_REF(result, 0), err_string, result_len);

  return ENTER_FIXNUM(result_len);
}
