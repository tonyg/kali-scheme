/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/


/* Implementation of the vm-extension opcode */

#include <stdio.h>
#include <fcntl.h>		/* for O_RDWR */
#include <string.h>
#include "scheme48.h"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>		/* setuid & setgid */

#if defined(SOCKET_SUPPORT)
/* Things for socket library... */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#endif

FILE **port_to_stream(scheme_value port)
{
  int index;
  extern FILE **Sopen_portsS;

  if (!portp(port))
    return NULL;		/* not a port */

  index = port_index(port);
  if (index < 0)
    return NULL;		/* port not open */

  return &Sopen_portsS[index];
}

/* 0 = failure, 1 = success */
int null_terminate(scheme_value string, char *buffer, long buffer_size)
{
  if (!stringp(string)) return 0;
  {
    size_t len = string_length(string);
    if (len >= buffer_size) return 0;
    strncpy(buffer, &string_ref(string, 0), len);
    buffer[len] = '\0';
    return 1;
  }
}

/******************************************/

scheme_value
extended_vm (long key, scheme_value value)
{
  switch (key) {
  
  case 0:			/* read jumpers on 68000 board */
    return 0;

#if defined(SOCKET_SUPPORT)
  case 20:
    { int s = internet_stream_socket();
      return (s < 0) ? UNDEFINED : ENTER_FIXNUM(s);
    }

  case 21:
    { int port = socket_bind(EXTRACT_FIXNUM(value), 0);
      return (port < 0) ? UNDEFINED : ENTER_FIXNUM(port);
    }

  case 22:
    { int fd = socket_accept(EXTRACT_FIXNUM(value));
      return (fd < 0) ? UNDEFINED : ENTER_FIXNUM(fd);
    }

  case 23:
    if (!pairp(value)) return UNDEFINED;
    { char *hostname = &string_ref(car(value), 0);
      long hostname_length = string_length(car(value));
      long port = EXTRACT_FIXNUM(cdr(value));
      int fd;
      if (hostname[hostname_length] != '\0')
	return UNDEFINED;
      fd = socket_connect(hostname, port);
      return (fd < 0) ? UNDEFINED : ENTER_FIXNUM(fd);
    }
#endif

  /* fdopen() support */
  case 24:
  case 25:
    if (!pairp(value)) return UNDEFINED;
    { scheme_value port = car(value);
      long fd = EXTRACT_FIXNUM(cdr(value));
      FILE **pstream = port_to_stream(port);
      FILE *new_stream;

      if (pstream == NULL) return UNDEFINED;
      new_stream = fdopen(fd, key == 24 ? "r" : "w");
      if (new_stream == NULL) return UNDEFINED;
      fclose(*pstream);
      *pstream = new_stream;
      return SCHTRUE;
    }

  /* getenv() */
  case 26: {
#   define GETENV_BUFFER_SIZE 200
    scheme_value env_var, result_buffer;
    char buffer[GETENV_BUFFER_SIZE];
    char *result;
    size_t result_len;
    
    if (!pairp(value)) return UNDEFINED;
    env_var = car(value);
    result_buffer = cdr(value);
    if (!stringp(env_var) || !stringp(result_buffer)) return UNDEFINED;
    if (!null_terminate(env_var, buffer, GETENV_BUFFER_SIZE))
      return UNDEFINED;
    result = getenv(buffer);
    if (result == NULL)
      return SCHFALSE;
    result_len = strlen(result);
    if (result_len > string_length(result_buffer))
      return UNDEFINED;
    strncpy(&string_ref(result_buffer, 0), result, result_len);
    return ENTER_FIXNUM(result_len);
  }

  case 27: {
    /* This is intended for use by HTTP scripts... */
    if (!pairp(value) || !FIXNUMP(car(value)) || !FIXNUMP(cdr(value)))
      return UNDEFINED;
    if (setgid(EXTRACT_FIXNUM(cdr(value))) != 0 ||
	setuid(EXTRACT_FIXNUM(car(value))) != 0)
      return SCHFALSE;
    else
      return SCHTRUE;
  }

  /* popen() support */
  case 97:
  case 98: {
#   define POPEN_BUFFER_SIZE 200
    if (!pairp(value)) return UNDEFINED;
    { scheme_value port = car(value);
      FILE **pstream = port_to_stream(port);
      scheme_value command = cdr(value);
      char buffer[POPEN_BUFFER_SIZE];
      FILE *new_stream;
      /* extern FILE *popen(const char *, const char *);  --POSIX stdio.h */

      if (pstream == NULL) return UNDEFINED;
      if (!null_terminate(command, buffer, POPEN_BUFFER_SIZE))
	return UNDEFINED;

      new_stream = popen(buffer, key == 97 ? "r" : "w");
      if (new_stream == NULL) return UNDEFINED;
      fclose(*pstream);
      *pstream = new_stream;
      return SCHTRUE;
    }
  }

  /* Floating point */
  case 99: {
    extern scheme_value vm_float_op(long, scheme_value);
    if (!vectorp(value)) return UNDEFINED;
    return vm_float_op(EXTRACT_FIXNUM(vector_ref(value, 0)), value);
  }

  default:
    return UNDEFINED;
  }
}

/* stdlib.h declares atof */

#define get_arg(args,i) vector_ref(args,(i)+1)
#define get_int_arg(args,i) EXTRACT_FIXNUM(get_arg(args,i))
#define get_string_arg(args,i) (&string_ref(get_arg(args,i), 0))

typedef struct {
  char b[sizeof(double)];
} unaligned_double;

typedef union {
  double f;
  unaligned_double b;
} float_or_bytes;

#define get_float_arg(args, i, var) \
  { scheme_value temp_ = get_arg(args,i); \
    float_or_bytes loser_; \
    if (!byte_vectorp(temp_)) return UNDEFINED; \
    loser_.b = *(unaligned_double*)(&byte_vector_ref(temp_, 0)); \
    (var) = loser_.f; }

#define set_float_arg(args, i, val) \
  { scheme_value temp_ = get_arg(args,i); \
    float_or_bytes loser_; \
    if (!byte_vectorp(temp_)) return UNDEFINED; \
    loser_.f = (double)(val); \
    *(unaligned_double*)(&byte_vector_ref(temp_, 0)) = loser_.b; }
   
scheme_value
vm_float_op( long op, scheme_value args )
{
  double x, y;

  switch (op) {
  case 0: {
    get_float_arg(args, 0, x);
    get_float_arg(args, 1, y);
    set_float_arg(args, 2, x + y);
    return 0;}
  case 1: {
    get_float_arg(args, 0, x);
    get_float_arg(args, 1, y);
    set_float_arg(args, 2, x - y);
    return 0;}
  case 2: {
    get_float_arg(args, 0, x);
    get_float_arg(args, 1, y);
    set_float_arg(args, 2, x * y);
    return 0;}
  case 3: {
    get_float_arg(args, 0, x);
    get_float_arg(args, 1, y);
    if (y == 0) return UNDEFINED;
    set_float_arg(args, 2, x / y);
    return 0;}
  case 4: {
    get_float_arg(args, 0, x);
    get_float_arg(args, 1, y);
    return (x == y) ? 0 : 4;}
  case 5: {
    get_float_arg(args, 0, x);
    get_float_arg(args, 1, y);
    return (x < y) ? 0 : 4;}
  case 6: {          /* integer -> float */
    scheme_value arg = get_arg(args, 0);
    if (!FIXNUMP(arg)) return UNDEFINED;
    set_float_arg(args, 1, EXTRACT_FIXNUM(arg));
    return 0;}
  case 7: {          /* string -> float */
    static char buffer[80];
    long len =	  get_int_arg(args, 1);
    strncpy(buffer, get_string_arg(args, 0), len);
    buffer[len] = '\0';
    set_float_arg(args, 2, atof(buffer));
    return 0;}
  case 8: {
    size_t len;
    char *str =   get_string_arg(args,1);
    get_float_arg(args, 0, x);
    sprintf(str, "%g", x);
    len = strlen(str);
    if (len > string_length(get_arg(args,1))) /* unlikely */
      fprintf(stderr, "printing float: output too long: %s\n",
	      str);
    get_arg(args, 2) = ENTER_FIXNUM(len);
    return 0;}

    /* exp log sin cos tan asin acos atan sqrt */

  case 9: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, exp(x));
    return 0;}
  case 10: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, log(x));
    return 0;}
  case 11: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, sin(x));
    return 0;}
  case 12: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, cos(x));
    return 0;}
  case 13: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, tan(x));
    return 0;}
  case 14: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, asin(x));
    return 0;}
  case 15: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, acos(x));
    return 0;}
  case 16: {
    get_float_arg(args, 0, y);
    get_float_arg(args, 1, x);
    set_float_arg(args, 2, atan2(y, x));
    return 0;}
  case 17: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, sqrt(x));
    return 0;}
  case 18: {
    get_float_arg(args, 0, x);
    set_float_arg(args, 1, floor(x));
    return 0;}
  case 19: {			/* integer? */
    get_float_arg(args, 0, x);
    return (fmod(x, 1.0) == 0.0) ? SCHTRUE : SCHFALSE; }
  case 20: {			/* inexact->exact */
    get_float_arg(args, 0, x);
    if (x <= (double)GREATEST_FIXNUM_VALUE
	&& x >= (double)LEAST_FIXNUM_VALUE)
      return ENTER_FIXNUM((long)x);
    else
      return UNDEFINED;
  }
  case 21: {			/* quotient */
    double z;
    get_float_arg(args, 0, x);
    get_float_arg(args, 1, y);
    if (fmod(x, 1.0) != 0 || fmod(y, 1.0)) return UNDEFINED;
    if (y == 0) return UNDEFINED;
    z = x / y;
    set_float_arg(args, 2, z < 0 ? ceil(z) : floor(z));
    return 0;}
  case 22: {			/* remainder */
    get_float_arg(args, 0, x);
    get_float_arg(args, 1, y);
    if (fmod(x, 1.0) != 0 || fmod(y, 1.0)) return UNDEFINED;
    if (y == 0) return UNDEFINED;

    /* "fmod(double x, double y) returns the floating-point remainder
       (f) of the division of x by y, where f has the same sign as x,
       such that x=iy+f for some integer i, and |f| < |y|." */

    set_float_arg(args, 2, fmod(x, y));
    return 0;}
  default:
    return UNDEFINED;
  }
}
