/* Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Scheme 48/POSIX I/O interface
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "scheme48.h"
#include "scheme48vm.h"		/* ps_close_fd() */
#include "posix.h"
#include "c-mods.h"
#include "unix.h"
#include "fd-io.h"

extern void		s48_init_posix_io(void);
static s48_value	posix_dup(s48_value channel, s48_value mode_change_p),
			posix_dup2(s48_value channel, s48_value new_fd),
			posix_pipe(void),
			posix_close_on_exec_p(s48_value channel),
			posix_set_close_on_exec(s48_value channel,
						s48_value close_p),
     			posix_io_flags(s48_value channel, s48_value options);

static s48_value	s48_enter_file_options(int options);
  
/*
 * Record types imported from Scheme.
 */
static s48_value	posix_file_options_type_binding = S48_FALSE;

/*
 * Install all exported functions in Scheme48.
 */
void
s48_init_posix_io(void)
{
  S48_EXPORT_FUNCTION(posix_dup);
  S48_EXPORT_FUNCTION(posix_dup2);
  S48_EXPORT_FUNCTION(posix_pipe);
  S48_EXPORT_FUNCTION(posix_close_on_exec_p);
  S48_EXPORT_FUNCTION(posix_set_close_on_exec);
  S48_EXPORT_FUNCTION(posix_io_flags);

  S48_GC_PROTECT_GLOBAL(posix_file_options_type_binding);
  posix_file_options_type_binding =
    s48_get_imported_binding("posix-file-options-type");
}

/*
 * Moves `channel' to a new file descriptor and returns a new channel that uses
 * `channel''s old file descriptor.
 *
 * Without all the error checking, this is:
 *   old_fd = channel_os_index(channel);
 *   new_fd = dup(old_fd);
 *   s48_set_channel_os_index(channel, new_fd);
 *   return s48_add_channel(old_fd);
 *
 */

static s48_value
posix_dup(s48_value channel, s48_value new_mode)
{
  int		new_fd, old_fd;
  long 		status;
  s48_value 	new_channel;

  if (!S48_CHANNEL_P(channel) ||
      S48_CHANNEL_STATUS(channel) == S48_CHANNEL_STATUS_CLOSED)
    s48_raise_argtype_error(channel);
  
  old_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  RETRY_OR_RAISE_NEG(new_fd, dup(old_fd));
  
  status = s48_set_channel_os_index(channel, new_fd);

  if (status != S48_TRUE) {
    ps_close_fd(new_fd);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(status), 1, channel); }

  new_channel = s48_add_channel((new_mode == S48_FALSE) ?
				  S48_UNSAFE_CHANNEL_STATUS(channel) :
				  new_mode,
				S48_UNSAFE_CHANNEL_ID(channel),
				old_fd);

  if (!S48_CHANNEL_P(new_channel)) {
    ps_close_fd(old_fd);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(new_channel), 1, channel); }

  return new_channel;
}

/*
 * Same again, except that we get told what the new file descriptor is to be.
 * We close the channel currently using that descriptor, if there be one.
 *
 * Without all the error checking, this is:
 *   old_fd = channel_os_index(channel);
 *   dup2(old_fd, new_fd);
 *   s48_set_channel_os_index(channel, new_fd);
 *   return s48_add_channel(old_fd);
 */

static s48_value
posix_dup2(s48_value channel, s48_value new_fd)
{
  s48_value 	new_channel;
  long 		s48_status;
  int 		status;
  int 		new_c_fd, old_c_fd;

  if (!S48_CHANNEL_P(channel) ||
      S48_CHANNEL_STATUS(channel) == S48_CHANNEL_STATUS_CLOSED)
    s48_raise_argtype_error(channel);

  if (!S48_FIXNUM_P(new_fd) || new_fd < 0)
    s48_raise_argtype_error(new_fd);

  old_c_fd = s48_extract_fixnum(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  new_c_fd = s48_extract_fixnum(new_fd);

  s48_close_channel(new_c_fd);

  RETRY_OR_RAISE_NEG(status, dup2(old_c_fd, new_c_fd));

  s48_status = s48_set_channel_os_index(channel, new_c_fd);

  if (s48_status != S48_TRUE) {
    ps_close_fd(new_c_fd);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(s48_status), 1, channel); }

  new_channel = s48_add_channel(S48_UNSAFE_CHANNEL_STATUS(channel),
				S48_UNSAFE_CHANNEL_ID(channel),
				old_c_fd);

  if (!S48_CHANNEL_P(new_channel)) {
    ps_close_fd(old_c_fd);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(new_channel), 1, channel); }

  return new_channel;
}

/*
 * Opens a pipe and returns a pair (<input-channel> . <output-channel>).
 *
 * Synopsis:
 *    int fds[2];
 *    pipe(fds);
 *    return s48_cons(s48_add_channel(fds[1]), s48_add_channel(fds[2]));
 */

static s48_value
posix_pipe()
{
  int 		fildes[2],
    		status;
  s48_value	in_channel = S48_FALSE,
            	out_channel = S48_FALSE;
  s48_value 	id = s48_enter_string("pipe");

  S48_DECLARE_GC_PROTECT(3);
  
  S48_GC_PROTECT_3(in_channel, out_channel, id);

  RETRY_OR_RAISE_NEG(status, pipe(fildes));

  in_channel = s48_add_channel(S48_CHANNEL_STATUS_INPUT, id, fildes[0]);

  if (!S48_CHANNEL_P(in_channel)) {
    ps_close_fd(fildes[0]);		/* retries if interrupted */
    ps_close_fd(fildes[1]);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(in_channel), 0); }

  out_channel = s48_add_channel(S48_CHANNEL_STATUS_OUTPUT, id, fildes[1]);

  if (!S48_CHANNEL_P(out_channel)) {
    s48_close_channel(fildes[0]);
    ps_close_fd(fildes[1]);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(in_channel), 0); }

  S48_GC_UNPROTECT();

  return s48_cons(in_channel, out_channel);
}

static s48_value
posix_close_on_exec_p(s48_value channel)
{
  int	c_fd,
	status;

  if (!S48_CHANNEL_P(channel) ||
      S48_CHANNEL_STATUS(channel) == S48_CHANNEL_STATUS_CLOSED)
    s48_raise_argtype_error(channel);
  
  c_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  RETRY_OR_RAISE_NEG(status, fcntl(c_fd, F_GETFD));

  return S48_ENTER_BOOLEAN(status & FD_CLOEXEC);
}

static s48_value
posix_set_close_on_exec(s48_value channel, s48_value value)
{
  int	status, new_status;
  int	c_fd;

  if (!S48_CHANNEL_P(channel) ||
      S48_CHANNEL_STATUS(channel) == S48_CHANNEL_STATUS_CLOSED)
    s48_raise_argtype_error(channel);

  c_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  
  RETRY_OR_RAISE_NEG(status, fcntl(c_fd, F_GETFD));

  if (S48_EXTRACT_BOOLEAN(value))
    new_status = status | FD_CLOEXEC;
  else
    new_status = status & ! FD_CLOEXEC;
  
  if (new_status != status)
    RETRY_OR_RAISE_NEG(status, fcntl(c_fd, F_SETFD, new_status));
  
  return S48_UNSPECIFIC;
}

static s48_value
posix_io_flags(s48_value channel, s48_value options)
{
  int	status;
  int	c_fd;

  if (!S48_CHANNEL_P(channel) ||
      S48_CHANNEL_STATUS(channel) == S48_CHANNEL_STATUS_CLOSED)
    s48_raise_argtype_error(channel);

  c_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  if (options == S48_FALSE) {
  
    RETRY_OR_RAISE_NEG(status, fcntl(c_fd, F_GETFL));

    return s48_enter_file_options(status);
  }
  else {
    int c_options = s48_extract_file_options(options);

    RETRY_OR_RAISE_NEG(status, fcntl(c_fd, F_SETFL, c_options));
    
    return S48_UNSPECIFIC;
  }
}

/* ************************************************************ */
/* File options.
 *
 * We translate the local bits into our own bits and vice versa.
 */

s48_value
s48_enter_file_options(int file_options)
{
  s48_value	sch_file_options;
  int		my_file_options;

  my_file_options =
    (O_CREAT    & file_options ? 00001 : 0) |
    (O_EXCL     & file_options ? 00002 : 0) |
    (O_NOCTTY   & file_options ? 00004 : 0) |
    (O_TRUNC    & file_options ? 00010 : 0) |
    (O_APPEND   & file_options ? 00020 : 0) |
    /* POSIX 2nd ed., not in Linux
    (O_DSYNC    & file_options ? 00040 : 0) |
    */
    (O_NONBLOCK & file_options ? 00100 : 0) |
    /* POSIX 2nd ed., not in Linux
    (O_RSYNC    & file_options ? 00200 : 0) |
    */
    /* Not in FreeBSD
    (O_SYNC     & file_options ? 00400 : 0) |
    */
    (O_RDONLY   & file_options ? 01000 : 0) |
    (O_RDWR     & file_options ? 02000 : 0) |
    (O_WRONLY   & file_options ? 04000 : 0);

  sch_file_options = s48_make_record(posix_file_options_type_binding);
  S48_UNSAFE_RECORD_SET(sch_file_options, 0, s48_enter_fixnum(my_file_options));

  return sch_file_options;
}

int
s48_extract_file_options(s48_value sch_file_options)
{
  int	c_file_options;
  long	file_options;

  s48_check_record_type(sch_file_options, posix_file_options_type_binding);

  file_options = s48_extract_fixnum(S48_UNSAFE_RECORD_REF(sch_file_options, 0));

  c_file_options =
    (00001 & file_options ? O_CREAT    : 0) |
    (00002 & file_options ? O_EXCL     : 0) |
    (00004 & file_options ? O_NOCTTY   : 0) |
    (00010 & file_options ? O_TRUNC    : 0) |
    (00020 & file_options ? O_APPEND   : 0) |
    /* POSIX 2nd ed., not in Linux
    (00040 & file_options ? O_DSYNC    : 0) |
    */
    (00100 & file_options ? O_NONBLOCK : 0) |
    /* POSIX 2nd ed., not in Linux
    (00200 & file_options ? O_RSYNC    : 0) |
    */
    /* Not in FreeBSD
    (00400 & file_options ? O_SYNC     : 0) |
    */
    (01000 & file_options ? O_RDONLY   : 0) |
    (02000 & file_options ? O_RDWR     : 0) |
    (04000 & file_options ? O_WRONLY   : 0);

  return c_file_options;
}

