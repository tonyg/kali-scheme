/* Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * An interface to Unix sockets.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include "c-mods.h"
#include "scheme48.h"
#include "fd-io.h"		/* ps_close_fd() */
#include "event.h"		/* add_pending_fd() */

/* Henry Cejtin says that 5 is the largest safe number for this. */
#define	LISTEN_QUEUE_SIZE	5

extern void		s48_init_socket(void);
static s48_value	s48_socket(s48_value server_p),
			s48_bind(s48_value socket_channel, s48_value number),
			s48_socket_number(s48_value socket_channel),
			s48_listen(s48_value socket_channel),
			s48_accept(s48_value socket_channel),
			s48_connect(s48_value socket_channel,
				    s48_value machine,
				    s48_value port),
			s48_close_socket_half(s48_value socket_channel,
					      s48_value input_p),
			s48_get_host_name(void);

/*
 * Install all exported functions in Scheme48.
 */
void
s48_init_socket(void)
{
  S48_EXPORT_FUNCTION(s48_socket);
  S48_EXPORT_FUNCTION(s48_bind);
  S48_EXPORT_FUNCTION(s48_socket_number);
  S48_EXPORT_FUNCTION(s48_listen);
  S48_EXPORT_FUNCTION(s48_accept);
  S48_EXPORT_FUNCTION(s48_connect);
  S48_EXPORT_FUNCTION(s48_close_socket_half);
  S48_EXPORT_FUNCTION(s48_get_host_name);
}

/*
 * Create an internet-domain stream (reliable, sequenced) socket.
 * We return an input channel on success and raise an exception on failure.
 * The socket has been made non-blocking.
 */

static s48_value
s48_socket(s48_value server_p)
{
  int		fd,
    		mode;
  s48_value	channel;

  fd = socket(AF_INET, SOCK_STREAM, 0);

  if (fd < 0)
    s48_raise_os_error(errno);

  if (-1 == fcntl(fd, F_SETFL, O_NONBLOCK))
    s48_raise_os_error(errno);

  mode = (server_p == S48_FALSE) ?
    S48_CHANNEL_STATUS_SPECIAL_OUTPUT :
    S48_CHANNEL_STATUS_SPECIAL_INPUT;

  channel = s48_add_channel(mode, s48_enter_string("socket"), fd);

  if (!S48_CHANNEL_P(channel)) {
    ps_close_fd(fd);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(channel), 0); };

  return channel;
}

/*
 * Given an internet-domain stream socket and a port number, bind
 * the socket to the port and prepare to receive connections.
 * If the port number is #f, then we bind the socket to any available
 * port.
 * 
 * Nothing useful is returned.
 */

static s48_value
s48_bind(s48_value channel, s48_value port_number)
{
  int			socket_fd,
			port;
  struct sockaddr_in	address;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  if (port_number == S48_FALSE)
    port = 0;
  else
    port = s48_extract_fixnum(port_number);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    s48_raise_os_error(errno);

  return S48_UNSPECIFIC;
}

/*
 * Return the port number associated with an internet stream socket.
 */

static s48_value
s48_socket_number(s48_value channel)
{
  int			socket_fd,
			len;
  struct sockaddr_in	address;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  address.sin_addr.s_addr = htonl(INADDR_ANY);

  len = sizeof(address);

  if ((getsockname(socket_fd, (struct sockaddr *)&address, &len) < 0)
	|| (address.sin_family != AF_INET))
    s48_raise_os_error(errno);

  return s48_enter_fixnum(htons(address.sin_port));
}

static s48_value
s48_listen(s48_value channel)
{
  int			socket_fd;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  if (listen(socket_fd, LISTEN_QUEUE_SIZE) < 0)
    s48_raise_os_error(errno);

  return S48_UNSPECIFIC;
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
s48_accept(s48_value channel)
{
  int			socket_fd,
   			connect_fd,
			output_fd,
			len;
  struct sockaddr_in	address;
  s48_value		input_channel,
			output_channel;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  len = sizeof(address);
  connect_fd = accept(socket_fd, (struct sockaddr *)&address, &len);
  
  /*
   * Check for a connection.  If we have one we create two channels, one
   * input and one, with a dup()'ed fd, output.  Lots of error checking
   * makes this messy.
   */

  if (connect_fd >= 0) {
    
    S48_DECLARE_GC_PROTECT(1);
  
    if (-1 == fcntl(connect_fd, F_SETFL, O_NONBLOCK))
      s48_raise_os_error(errno);

    input_channel = s48_add_channel(S48_CHANNEL_STATUS_INPUT,
				    s48_enter_string("socket connection"),
				    connect_fd);

    if (!S48_CHANNEL_P(input_channel)) {
      ps_close_fd(connect_fd);		/* retries if interrupted */
      s48_raise_scheme_exception(s48_extract_fixnum(input_channel), 0); };
    
    output_fd = dup(connect_fd);

    if (output_fd == -1)
      s48_raise_os_error(errno);

    S48_GC_PROTECT_1(input_channel);
  
    output_channel = s48_add_channel(S48_CHANNEL_STATUS_OUTPUT,
				     s48_enter_string("socket connection"),
				     output_fd);
    
    if (!S48_CHANNEL_P(output_channel)) {
      /* input_channel will eventually be closed by the GC */
      ps_close_fd(output_fd);		/* retries if interrupted */
      s48_raise_scheme_exception(s48_extract_fixnum(output_channel), 0); };
    
    S48_GC_UNPROTECT();

    return s48_cons(input_channel, output_channel);
  }

  /*
   * Check for errors.  If we need to retry we mark the socket as pending
   * and return #F to tell the Scheme procedure to wait.
   */

  if ((errno != EWOULDBLOCK) && (errno != EINTR) && (errno == EAGAIN))
    s48_raise_os_error(errno);

  if (! s48_add_pending_fd(socket_fd, TRUE))
    s48_raise_out_of_memory_error();

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
s48_connect(s48_value channel, s48_value machine, s48_value port)
{
  int			socket_fd,
			output_fd,
    			port_number;
  char			*machine_name;
  struct hostent	*host;
  struct sockaddr_in	address;
  s48_value		output_channel;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  S48_CHECK_STRING(machine);
  machine_name = S48_UNSAFE_EXTRACT_STRING(machine);
  
  S48_CHECK_FIXNUM(port);
  port_number = S48_UNSAFE_EXTRACT_FIXNUM(port);
  
  /*
   * Get the host and initialize `address'.
   */

  host = gethostbyname(machine_name);
  if (host == NULL)
    s48_raise_os_error(errno);
  
  memset((void *)&address, 0, sizeof(address));
  address.sin_family = host->h_addrtype;

  if (host->h_length > sizeof(address.sin_addr))
    s48_raise_range_error(s48_enter_fixnum(host->h_length),
			  S48_UNSAFE_ENTER_FIXNUM(0),
			  s48_enter_fixnum(sizeof(address.sin_addr)));
  memcpy((void *)&address.sin_addr, (void *)host->h_addr, host->h_length);
  address.sin_port = htons(port_number);

  /*
   * Try the connection.  If it works we make an output channel and return it.
   * The original socket channel will be used as the input channel.
   */

  if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) >= 0) {
    S48_STOB_SET(channel, S48_CHANNEL_STATUS_OFFSET, S48_CHANNEL_STATUS_INPUT);
    
    output_fd = dup(socket_fd);
    
    if (output_fd == -1)
      s48_raise_os_error(errno);
    
    output_channel = s48_add_channel(S48_CHANNEL_STATUS_OUTPUT,
				     s48_enter_string("socket connection"),
				     output_fd);
    
    if (!S48_CHANNEL_P(output_channel)) {
      ps_close_fd(output_fd);		/* retries if interrupted */
      s48_raise_scheme_exception(s48_extract_fixnum(output_channel), 0); };
    
    return output_channel;
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
    s48_raise_os_error(errno);

  if (! (s48_add_pending_fd(socket_fd, FALSE)))
    s48_raise_out_of_memory_error();

  return S48_FALSE;
}

/*
 * Close half of a socket; if `input_p' is true we close the input half,
 * otherwise the output half.  This horribleness is forced upon us by
 * Unix's use of bidirectional file descriptors.
 */

static s48_value
s48_close_socket_half(s48_value channel, s48_value input_p)
{
  int	socket_fd;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  /* We ignore `endpoint is not connected' errors, as we just want to get
     the file descriptor closed. */
  if ((0 > shutdown(socket_fd, S48_EXTRACT_BOOLEAN(input_p) ? 0 : 1))
      && (errno != ENOTCONN))
    s48_raise_os_error(errno);
  
  return S48_TRUE;
}  


/*
 * Get the name of the local machine.
 */

static s48_value
s48_get_host_name(void)
{
  char	mbuff[MAXHOSTNAMELEN];

  if (gethostname(mbuff, sizeof(mbuff)) < 0)
    s48_raise_os_error(errno);
  
  return s48_enter_string(mbuff);
}

