/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Unix-specific sockets stuff.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>


#include <scheme48.h>

#include "c-mods.h"
#include "unix.h"
#include "fd-io.h"		/* ps_close_fd() */
#include "event.h"		/* add_pending_fd() */

#include "sysdep.h"

#include "socket.h"
#include "address.h"

static s48_value
s48_socket(s48_value sch_af, s48_value sch_type, s48_value sch_protocol)
{
  socket_t fd;
  int mode, status;
  s48_value sch_channel;
  int af = s48_extract_af(sch_af);
  int socktype = s48_extract_socket_type(sch_type);
  int protocol = s48_extract_fixnum(sch_protocol);

  RETRY_OR_RAISE_NEG(fd, socket(af, socktype, protocol));
  RETRY_OR_RAISE_NEG(status, fcntl(fd, F_SETFL, O_NONBLOCK));

  sch_channel = s48_add_channel(S48_CHANNEL_STATUS_SPECIAL_INPUT,
				s48_enter_string_latin_1("socket"), fd);

  if (!S48_CHANNEL_P(sch_channel))
    {
      ps_close_fd(fd);		/* retries if interrupted */
      s48_raise_scheme_exception(s48_extract_fixnum(sch_channel), 0);
    }
  
  return sch_channel;
}

static s48_value
s48_socketpair(s48_value sch_af, s48_value sch_type, s48_value sch_protocol)
{
  int status;
  s48_value sch_channel0 = S48_UNSPECIFIC, sch_channel1 = S48_UNSPECIFIC;
  s48_value sch_result;
  int af = s48_extract_af(sch_af);
  int socktype = s48_extract_socket_type(sch_type);
  int protocol = s48_extract_fixnum(sch_protocol);
  socket_t fds[2];
  S48_DECLARE_GC_PROTECT(2);

  RETRY_OR_RAISE_NEG(status, socketpair(af, socktype, protocol, fds));

  RETRY_OR_RAISE_NEG(status, fcntl(fds[0], F_SETFL, O_NONBLOCK));
  RETRY_OR_RAISE_NEG(status, fcntl(fds[1], F_SETFL, O_NONBLOCK));

  S48_GC_PROTECT_2(sch_channel0, sch_channel1);
  sch_channel0 = s48_add_channel(S48_CHANNEL_STATUS_INPUT,
				 s48_enter_string_latin_1("socket"), fds[0]);
  sch_channel1 = s48_add_channel(S48_CHANNEL_STATUS_INPUT,
				 s48_enter_string_latin_1("socket"), fds[1]);

  
  sch_result = s48_cons(sch_channel0, sch_channel1);
  S48_GC_UNPROTECT();
  return sch_result;
}

/*
 * dup() `socket_fd' and return an output channel holding the result.
 *
 * We have to versions, one for calling from C and one for calling from Scheme.
 */

static s48_value
dup_socket_channel(socket_t socket_fd)
{
  socket_t output_fd;
  s48_value output_channel;

  RETRY_OR_RAISE_NEG(output_fd, dup(socket_fd));

  output_channel = s48_add_channel(S48_CHANNEL_STATUS_OUTPUT,
				   s48_enter_string_latin_1("socket connection"),
				   output_fd);
  
  if (!S48_CHANNEL_P(output_channel))
    {
      ps_close_fd(output_fd);		/* retries if interrupted */
      s48_raise_scheme_exception(s48_extract_fixnum(output_channel), 0);
    };
  
  return output_channel;
}

socket_t
s48_extract_socket_fd(s48_value sch_channel)
{
  S48_CHECK_CHANNEL(sch_channel);
  return S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(sch_channel));
}

static s48_value
s48_dup_socket_channel(s48_value sch_channel)
{
  return dup_socket_channel(s48_extract_socket_fd(sch_channel));
}

/*
 * Given an internet-domain stream socket which has been bound
 * accept a connection and return the resulting socket as a pair of channels
 * (after marking it non-blocking).
 *
 * If the accept fails because the client hasn't connected yet, then we
 * return #f.
 *
 * If it fails for any other reason, then an exception is raised.
 */

static s48_value
s48_accept(s48_value sch_channel, s48_value sch_retry_p)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  socket_t connect_fd;
  int status;
  struct sockaddr_storage address;
  socklen_t len;
  s48_value input_channel, output_channel;

  len = sizeof(address);
  
  connect_fd = accept(socket_fd, (struct sockaddr *)&address, &len);
  
  /*
   * Check for a connection.  If we have one we create two channels, one
   * input and one, with a dup()'ed fd, output.  Lots of error checking
   * makes this messy.
   */

  if (connect_fd >= 0) {
    
    RETRY_OR_RAISE_NEG(status, fcntl(connect_fd, F_SETFL, O_NONBLOCK));

    input_channel = s48_add_channel(S48_CHANNEL_STATUS_INPUT,
				    s48_enter_string_latin_1("socket connection"),
				    connect_fd);

    if (!S48_CHANNEL_P(input_channel))
      {
	ps_close_fd(connect_fd);		/* retries if interrupted */
	s48_raise_scheme_exception(s48_extract_fixnum(input_channel), 0);
      };
    
    return input_channel;
  }

  /*
   * Check for errors.  If we need to retry we mark the socket as pending
   * and return #F to tell the Scheme procedure to wait.
   */

  if ((errno != EWOULDBLOCK) && (errno != EINTR) && (errno != EAGAIN))
    s48_os_error("s48_accept", errno, 2, sch_channel, sch_retry_p);

  if (! s48_add_pending_fd(socket_fd, PSTRUE))
    s48_out_of_memory_error();

  return S48_FALSE;
}

/*
 * Given an internet-domain stream socket, a machine name and a port number,
 * connect the socket to that machine/port.
 *
 * If this succeeds, it returns an output channel for the connection.
 * If it fails because the connect would block, add the socket to the
 * pending queue (for output) and return #f.
 * If it fails for any other reason, raise an exception.
 */

static s48_value
s48_connect(s48_value sch_channel, s48_value sch_address,
	    s48_value sch_retry_p)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);

  /*
   * Try the connection.  If it works we make an output channel and return it.
   * The original socket channel will be used as the input channel.
   *
   * FreeBSD's connect() behaves oddly.  If you get told to wait, wait for
   * select() to signal the all-clear, and then try to connect again, you
   * get an `already connected' (EISCONN) error.  To handle this we pass in
   * a retry_p flag.  If retry_p is  true the `already connected' error is
   * ignored.
   */

  if (connect(socket_fd,
	      S48_EXTRACT_VALUE_POINTER(sch_address, struct sockaddr),
	      S48_VALUE_SIZE(sch_address)) >= 0
      || ((errno == EISCONN) && (sch_retry_p == S48_TRUE)))
    {
      S48_STOB_SET(sch_channel,
		   S48_CHANNEL_STATUS_OFFSET, S48_CHANNEL_STATUS_INPUT);
      return dup_socket_channel(socket_fd);
    }
    
  /*
   * Check for errors.  If we need to retry we mark the socket as pending
   * and return #F to tell the Scheme procedure to wait.
   */

  /* already connected, will raise an error from Scheme */
  if (errno == EISCONN)
    return S48_TRUE;

  if (errno != EWOULDBLOCK && errno != EINTR && errno != EALREADY
      && errno != EINPROGRESS && errno != EAGAIN)
    s48_os_error("s48_connect", errno, 3, sch_channel, sch_address, sch_retry_p);

  if (! (s48_add_pending_fd(socket_fd, PSFALSE)))
    s48_out_of_memory_error();

  return S48_FALSE;
}

/*
 * Receive a message.  Returns pair (<byte-count> . <sender>) or just
 * <byte-count> if want_sender_p is false.
 */

static s48_value
s48_recvfrom(s48_value sch_channel,
	     s48_value sch_buffer, s48_value sch_start, s48_value sch_count,
	     s48_value sch_flags,
	     s48_value sch_want_sender_p)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int want_sender_p = !(S48_EQ_P(sch_want_sender_p, S48_FALSE));
  struct sockaddr_storage from;
  socklen_t from_len = (want_sender_p ? sizeof(struct sockaddr_storage) : 0);
  int flags = s48_extract_msg_flags(sch_flags);
  size_t buffer_size = S48_BYTE_VECTOR_LENGTH(sch_buffer);
  size_t start = s48_extract_unsigned_integer(sch_start);
  size_t count = s48_extract_unsigned_integer(sch_count);
  ssize_t status;

  if ((start + count) > buffer_size)
    s48_assertion_violation("s48_sendto", "buffer start or count is wrong", 3,
			    sch_buffer, sch_start, sch_count);

  status = recvfrom(socket_fd,
		    s48_extract_byte_vector(sch_buffer) + start,
		    count,
		    flags,
		    want_sender_p ? (struct sockaddr*)&from : NULL,
		    &from_len);
  
  if (0 <= status)
    if (want_sender_p)
      {
	s48_value sch_count = S48_UNSPECIFIC, sch_saddr = S48_UNSPECIFIC;
	s48_value sch_result;
	S48_DECLARE_GC_PROTECT(2);
	S48_GC_PROTECT_2(sch_count, sch_saddr);
	sch_count = s48_enter_unsigned_integer(status);
	sch_saddr = s48_enter_sockaddr((struct sockaddr *)&from, from_len);
	sch_result = s48_cons(sch_count, sch_saddr);
	S48_GC_UNPROTECT();
	return sch_result;
      }
    else
      return s48_enter_unsigned_integer(count);
  
  /*
   * Check for errors.  If we need to retry we mark the socket as pending
   * and return #F to tell the Scheme procedure to wait.
   */
  
  if (errno != EWOULDBLOCK && errno != EINTR && errno != EALREADY
      && errno != EINPROGRESS && errno != EAGAIN)
    s48_os_error("s48_recv", errno, 6,
		 sch_channel, sch_buffer, sch_start, sch_count,
		 sch_flags, sch_want_sender_p);

  if (! (s48_add_pending_fd(socket_fd, PSTRUE)))
    s48_out_of_memory_error();

  return S48_FALSE;
}

static s48_value
s48_sendto(s48_value sch_channel,
	   s48_value sch_buffer, s48_value sch_start, s48_value sch_count,
	   s48_value sch_flags,
	   s48_value sch_saddr)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  ssize_t sent;
  const struct sockaddr *sa
    = S48_EXTRACT_VALUE_POINTER(sch_saddr, const struct sockaddr);
  socklen_t salen = S48_VALUE_SIZE(sch_saddr);
  int flags = s48_extract_msg_flags(sch_flags);
  size_t buffer_size = S48_BYTE_VECTOR_LENGTH(sch_buffer);
  size_t start = s48_extract_unsigned_integer(sch_start);
  size_t count = s48_extract_unsigned_integer(sch_count);

  if ((start + count) > buffer_size)
    s48_assertion_violation("s48_sendto", "buffer start or count is wrong", 3,
			    sch_buffer, sch_start, sch_count);
  
  sent = sendto(socket_fd,
		s48_extract_byte_vector(sch_buffer) + start,
		count,
		flags,
		(struct sockaddr *) sa, salen);
  
  if (0 <= sent)
    return s48_enter_unsigned_integer(sent);

  /*
   * Check for errors.  If we need to retry we mark the socket as pending
   * and return #F to tell the Scheme procedure to wait.
   */
  
  if (errno != EWOULDBLOCK && errno != EINTR && errno != EALREADY
      && errno != EINPROGRESS && errno != EAGAIN)
    s48_os_error("s48_sendto", errno, 6,
		 sch_channel, sch_saddr, sch_flags, sch_buffer, sch_start, sch_count);

  if (! (s48_add_pending_fd(socket_fd, PSFALSE)))
    s48_out_of_memory_error();

  return S48_FALSE;
}
void
s48_init_os_sockets(void)
{
  S48_EXPORT_FUNCTION(s48_socket);
  S48_EXPORT_FUNCTION(s48_socketpair);
  S48_EXPORT_FUNCTION(s48_dup_socket_channel);
  S48_EXPORT_FUNCTION(s48_accept);
  S48_EXPORT_FUNCTION(s48_connect);
  S48_EXPORT_FUNCTION(s48_recvfrom);
  S48_EXPORT_FUNCTION(s48_sendto);
}
