/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>              /* for errno, (POSIX?/ANSI) */
#include <string.h>		/* FD_ZERO sometimes needs this */
#include <locale.h>		/* ISO C99 */
#include <langinfo.h>		/* SUSv2 */
#include "sysdep.h"
#include "c-mods.h"
#include "scheme48vm.h"
#include "event.h"
#include "fd-io.h"
#include "unix.h"

/* Non-blocking I/O on file descriptors.

 There appear to be two ways to get non-blocking input and output.  One
 is to open files with the O_NONBLOCK flag (and to use fcntl() to do the
 same to stdin and stdout), the other is to call select() on each file
 descriptor before doing the I/O operation.  O_NONBLOCK has the problem
 of being a property of the file descriptor, and its use with stdin and
 stdout can lead to horrible results.

 We use a mixture of both.  For input files we call select() before doing
 a read(), because read() will return immediately if there are any bytes
 available at all, and using O_NONBLOCK on stdin is a very bad idea.
 Output files are opened using O_NONBLOCK and stdout is left alone.

*/

int
ps_open_fd(char *filename, psbool is_input, long *status)
{
#define FILE_NAME_SIZE 1024
#define PERMISSION 0666   /* read and write for everyone */

  char filename_temp[FILE_NAME_SIZE];
  char *expanded;
  extern char *s48_expand_file_name(char *, char *, int);

  int flags;
  mode_t mode;

  expanded = s48_expand_file_name(filename, filename_temp, FILE_NAME_SIZE);
  if (expanded == NULL)
    return -1;

  if (is_input) {
    flags = O_RDONLY;
    mode = 0; }
  else {
    flags = O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK;
    mode = PERMISSION; }

  /* keep trying if interrupted */
  while(1) {
    int fd = open(expanded, flags, mode);
    if (fd != -1) {
      *status = NO_ERRORS;
      return fd; }
    else if (errno != EINTR) {
      *status = errno;
      return -1; }
  }
}

int
ps_close_fd(long fd_as_long)
{
  int fd = (int)fd_as_long;

  /* keep retrying if interrupted */
  while(1) {
    int status = close(fd);
    if (status != -1) {
      s48_remove_fd(fd);
      return NO_ERRORS; }
    else if (errno != EINTR)
      return errno;
  }
}

psbool ps_check_fd(long fd_as_long, psbool is_read, long *status)
{
  int fd = (int)fd_as_long;
  int ready;

  struct timeval timeout;
  fd_set fds;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  timerclear(&timeout);

  *status = NO_ERRORS;

  while(1) {
    ready = select(fd + 1,
		   is_read ? &fds : NULL,
		   is_read ? NULL : &fds,
		   &fds,
		   &timeout);
    if (ready == 0)
	return PSFALSE;
    else if (ready == 1)
	return PSTRUE;
    else if (errno != EINTR) {
	*status = errno;
	return PSFALSE; } } 
}

long
ps_read_fd(long fd_as_long, char *buffer, long max, psbool waitp,
	   psbool *eofp, psbool *pending, long *status)
{
  int got, ready;
  void *buf = (void *)buffer;
  int fd = (int)fd_as_long;

  struct timeval timeout;
  fd_set readfds;

  FD_ZERO(&readfds);
  FD_SET(fd, &readfds);
  timerclear(&timeout);

  /* for the normal return */
  *eofp = PSFALSE;
  *pending = PSFALSE;
  *status = NO_ERRORS;

  while(1) {
    ready = select(fd + 1, &readfds, NULL, &readfds, &timeout);
    if (ready == 0) {
      if (!waitp)
	return 0;
      else if (s48_add_pending_fd(fd, PSTRUE)) {
	*pending = PSTRUE;
	return 0; }
      else {
	*status = ENOMEM;    /* as close as POSIX gets */
	return 0; }}
    else if (ready == -1) {
      if (errno != EINTR) {
	*status = errno;
	return 0; } }
    else {          /* characters waiting */

      got = read(fd, buf, max);

      if (got > 0) {       /* all is well */
	return got; }
      else if (got == 0) { /* end of file */
	*eofp = PSTRUE;
	return 0; }
      else if (errno == EINTR) {			/* HCC */
	return 0; }
      else if (errno == EAGAIN) {			/* HCC */
	if (!waitp)
	  return 0;
	else if (s48_add_pending_fd(fd, PSTRUE)) {
	  *pending = PSTRUE;
	  return 0; }
	else {
	  *status = ENOMEM;    /* as close as POSIX gets */
	  return 0; } }
      else {
	*status = errno;
	return 0; } } }
}

long
ps_write_fd(long fd_as_long, char *buffer, long max, psbool *pending, long *status)
{
  int sent;
  int fd = (int)fd_as_long;
  void *buf = (void *)buffer;

  *pending = PSFALSE;
  *status = NO_ERRORS;

  sent = write(fd, buf, max);
  if (sent > 0)
    {}
  else if (errno == EINTR || errno == EAGAIN) {		/* HCC */
    if (s48_add_pending_fd(fd, PSFALSE))
      *pending = PSTRUE;
    else
      *status = ENOMEM;    /* as close as POSIX gets */
    sent = 0; }
  else {
    *status = errno;
    sent = 0; }

  return sent;
}

long
ps_io_buffer_size(void)
{
  return 4096;
}

char *
ps_console_encoding(long fd_as_long)
{
  static char *encoding_STDIN = NULL;
  static char *encoding_STDOUT = NULL;
  static char *encoding_STDERR = NULL;
  static char setlocale_called = PSFALSE;
  char *codeset;

  char** encoding_p;

  if (fd_as_long == STDIN_FD())
    encoding_p = &encoding_STDIN;
  else if (fd_as_long == STDOUT_FD())
    encoding_p = &encoding_STDOUT;
  else if (fd_as_long == STDERR_FD())
    encoding_p = &encoding_STDERR;
    
  /* Mike has no clue what the rationale for needing this is. */
  if (!setlocale_called)
    {
      setlocale(LC_CTYPE, "");
      setlocale_called = PSTRUE;
    }

  if (*encoding_p == NULL)
    {
      codeset = nl_langinfo(CODESET); /* this ain't reentrant */
      *encoding_p = malloc(strlen(codeset) + 1);
      if (*encoding_p == NULL)
	return NULL;
      strcpy(*encoding_p, codeset);
    }

  return *encoding_p;
}

long
ps_abort_fd_op(long fd_as_long)
{
  int fd = (int)fd_as_long;

  if (!s48_remove_fd(fd))
    fprintf(stderr, "Error: ps_abort_fd_op, no pending operation on fd %d\n",
	            fd);
  return 0;      /* because we do not actually do any I/O in parallel the
		    status is always zero: no characters transfered. */
}

/*
 * This checks that fd's destined for output channels are marked
 * as nonblocking.  Stdout and stderr are a special case because
 * we do not want to screw up the shell, if we were invoked from
 * one.
 */

s48_value
s48_add_channel(s48_value mode, s48_value id, long fd)
{
  if (mode == S48_CHANNEL_STATUS_OUTPUT
      && fd != 1
      && fd != 2) {
    int flags;
    RETRY_OR_RAISE_NEG(flags, fcntl(fd, F_GETFL));
    if ((flags & O_NONBLOCK) == 0)
      fprintf(stderr,
        "Warning: output channel file descriptor %d is not non-blocking\n",
	      fd); }
  return s48_really_add_channel(mode, id, fd);
}
