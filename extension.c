/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/


/* Implementation of the vm-extension opcode */

#include <stdio.h>
#include <fcntl.h>		/* for O_RDWR */
#include "scheme48.h"

FILE **port_to_stream(long port)
{
  int index;
  extern FILE **Sopen_portsS;

  if ((port & 3L) != 3L ||
      (((*((long *) ((port - 3L) + -4L))) >> 2L) & 31L) != 5L)
    return NULL;		/* not a port */

  index = (*((long *) ((port - 3L) + 4L))) >> 2L;
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


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>

/* hack by Basile STARYNKEVITCH */
#if defined(svr4) || defined(__svr4__) || defined(SYSV)
#include <string.h>
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

long
extended_vm (long key, long value)
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
    if (!pairp(value)) return UNDEFINED;
    { long port = car(value);
      long fd = EXTRACT_FIXNUM(cdr(value));
      FILE **pstream = port_to_stream(port);
      FILE *new_stream;

      if (pstream == NULL) return UNDEFINED;
      new_stream = fdopen(fd, "r");
      if (new_stream == NULL) return UNDEFINED;
      fclose(*pstream);
      *pstream = new_stream;
      return SCHTRUE;
    }

  case 25:			/* Same as above except "w" for "r" */
    if (!pairp(value)) return UNDEFINED;
    { long port = car(value);
      long fd = EXTRACT_FIXNUM(cdr(value));
      FILE **pstream = port_to_stream(port);
      FILE *new_stream;

      if (pstream == NULL) return UNDEFINED;
      new_stream = fdopen(fd, "w");
      if (new_stream == NULL) return UNDEFINED;
      fclose(*pstream);
      *pstream = new_stream;
      return SCHTRUE;
    }

  case 99: {
    long *values = (long *)(value - 3);
    extern long vm_float_op();
    return vm_float_op((*values)>>2, values + 1);
  }

  default:
    return UNDEFINED;
  }
}

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define get_ptr_arg(args,i) (((long) *((args) + (i))) - 3)
#define get_int_arg(args,i) ((*((args) + (i)))>>2)

long
vm_float_op( long op, long *args )
{
  switch (op) {
  case 0: {
    double *x = (double*)get_ptr_arg(args,0);
    double *y = (double*)get_ptr_arg(args,1);
    double *r = (double*)get_ptr_arg(args,2);
    *r = *x + *y;
    return 0;}
  case 1: {
    double *x = (double*)get_ptr_arg(args,0);
    double *y = (double*)get_ptr_arg(args,1);
    double *r = (double*)get_ptr_arg(args,2);
    *r = *x - *y;
    return 0;}
  case 2: {
    double *x = (double*)get_ptr_arg(args,0);
    double *y = (double*)get_ptr_arg(args,1);
    double *r = (double*)get_ptr_arg(args,2);
    *r = *x * *y;
    return 0;}
  case 3: {
    double *x = (double*)get_ptr_arg(args,0);
    double *y = (double*)get_ptr_arg(args,1);
    double *r = (double*)get_ptr_arg(args,2);
    *r = *x / *y;
    return 0;}
  case 4: {
    double *x = (double*)get_ptr_arg(args,0);
    double *y = (double*)get_ptr_arg(args,1);
    return (*x == *y) ? 0 : 4;}
  case 5: {
    double *x = (double*)get_ptr_arg(args,0);
    double *y = (double*)get_ptr_arg(args,1);
    return (*x < *y) ? 0 : 4;}
  case 6: {          /* integer -> float */
    long x =             get_int_arg(args,0);
    double *r = (double*)get_ptr_arg(args,1);
    *r = x;
    return 0;}
  case 7: {          /* string -> float */
    static char buffer[80];
    char *str =     (char*)get_ptr_arg(args,0);
    long len =             get_int_arg(args,1);
    double *res = (double*)get_ptr_arg(args,2);
    strncpy(buffer, str, len);
    buffer[len] = '\0';
    *res = atof(buffer);
    return 0;}
  case 8: {          /* float -> string */
    double *num = (double*)get_ptr_arg(args,0);
    char *str =     (char*)get_ptr_arg(args,1);
    sprintf(str, "%g", *num);
    *(args + 2) = strlen(str) << 2;
    return 0;}
  }
}
