/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/


/* Implementation of the vm-extension opcode */

#include <stdio.h>
#include <fcntl.h>		/* for O_RDWR */
#include <string.h>
#include "scheme48.h"

/* Things for socket library... */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>

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


/* Sockets */

/*
 *  This code based upon:
 *            The simple socket library
 *            Examples from "UNIX Programmer' Supplementary Documents Vol. 1
 *            Examples from "BSD Sockets: A Quick and Dirty Primer" by
 *                          Jim Frost.
 *            Geoffrey Brown's experimental xscheme functions 
 *
 *   The following copyright covers code derived from the simple socket library
 */

/* Simple Socket Libary
   Written by Mat Watson and Hubert Bartels.  12/01/90
 */
/*
Copyright (c) 1990 Mat Watson and Hubert Bartels. All rights reserved. 

Permission to use, copy, and/or distribute for any purpose and 
without fee is hereby granted, provided that both the above copyright 
notice and this permission notice appear in all copies and derived works. 
Fees for distribution of this software or for derived sources may only 
be charged with the express written permission of the copyright holders. 
This software is provided ``as is'' without express or implied warranty.
*/


/* hack by Basile STARYNKEVITCH */
#if defined(svr4) || defined(__svr4__) || defined(SYSV)
#define NO_BCOPY
#endif

#ifdef NO_BCOPY
#define bcopy(From,To,Len) memcpy(To,From,Len)
#define bzero(To,Len) memset(To, 0, Len)
#endif /*NO_BCOPY*/

int internet_stream_socket()
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    perror("opening stream socket");
  return sock;
}

int socket_bind(int sock, int portno) /* returns assigned port number */
{
  int length;
  struct sockaddr_in name;

  /* Create name with wildcards */
  
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = INADDR_ANY;

  /* ..., htons, ...  - convert values between host and network byte order
     u_short htons(u_short hostshort); */
  name.sin_port = htons(portno);

  /* bind() assigns a name to an unnamed socket.
     int bind(int s, struct sockaddr *name, int namelen); */
  if (bind(sock, &name, sizeof(name)) < 0) {
    perror("binding datagram socket");
    return -1; }
 
  /* Find out the assigned port number */
  length = sizeof(name);
  if (getsockname(sock, &name, &length)) {
    perror("getting socket name");
    return -1; }

  /* Max # of queued connects */
  listen(sock, 5);

  return ntohs(name.sin_port);
} /* SockServer */


/*
 *  Accept a connection from a server port.
 *  Returns a file descriptor.
 */

int socket_accept(int s)
{
  int fd;
  do
    fd = accept(s, NULL, NULL);
  while (fd < 0 && errno == EINTR);
  if (fd < 0)
    perror("accept");
  return fd;
}


/*
 * Connect to a server port.
 * Returns a file descriptor.
 *  
 */

int socket_connect(char *hostname, int portnum)
{
  int sock;
  struct sockaddr_in name;
  struct hostent *hp;
  
  /* Construct name, with no wildcards, of the socket to send to.
   * Gethostbyname() returns a structure including the network address
   * of the specified host.
   */

  hp = gethostbyname(hostname);
  if (hp == 0) {
    fprintf(stderr, "unknown host: %s\n", hostname);
    return -1; }
 
  bzero(&name, sizeof(name));
  bcopy(hp->h_addr_list[0], (char *) &name.sin_addr, (int) hp->h_length);
  name.sin_family = AF_INET;
  name.sin_port = htons(portnum);

  /* create socket */

  sock = internet_stream_socket();
  if (sock < 0) {
    perror("opening stream socket");
    return -1; }
    
  if (connect(sock, &name, sizeof(name)) < 0) {
    perror(hostname);
    close(sock); }
  return sock; 
}


/******************************************/

scheme_value
extended_vm (long key, scheme_value value)
{
  switch (key) {
  
  case 0:			/* read jumpers on 68000 board */
    return 0;

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

  case 97:
  case 98: {
#   define POPEN_BUFFER_SIZE 200
    if (!pairp(value)) return UNDEFINED;
    { scheme_value port = car(value);
      FILE **pstream = port_to_stream(port);
      scheme_value command = cdr(value);
      char buffer[POPEN_BUFFER_SIZE];
      long len;
      FILE *new_stream;
      /* extern FILE *popen(const char *, const char *);  --POSIX stdio.h */

      if (pstream == NULL) return UNDEFINED;
      if (!stringp(command)) return UNDEFINED;

      len = string_length(command);
      if (len >= POPEN_BUFFER_SIZE) return UNDEFINED;
      strncpy(buffer, &string_ref(command, 0), len);
      buffer[len] = '\0';

      new_stream = popen(buffer, key == 97 ? "r" : "w");
      if (new_stream == NULL) return UNDEFINED;
      fclose(*pstream);
      *pstream = new_stream;
      return SCHTRUE;
    }
  }

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

#include <stdlib.h>
#include <math.h>

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
    int len;
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
