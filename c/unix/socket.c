/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * An interface to Unix sockets.
 */

#define _XOPEN_SOURCE_EXTENDED 1		/* AIX wants this to be 1 */
#include "sysdep.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "c-mods.h"
#include "scheme48.h"
#include "unix.h"
#include "fd-io.h"		/* ps_close_fd() */
#include "event.h"		/* add_pending_fd() */

/* Henry Cejtin says that 5 is the largest safe number for this. */
#define	LISTEN_QUEUE_SIZE	5

extern void		s48_init_socket(void);
static s48_value	s48_socket(s48_value udp_p, s48_value input_p),
			s48_bind(s48_value socket_channel, s48_value number),
			s48_socket_number(s48_value socket_channel),
			s48_listen(s48_value socket_channel),
			s48_accept(s48_value socket_channel, s48_value retry_p),
			s48_connect(s48_value socket_channel,
				    s48_value address,
				    s48_value port,
				    s48_value retry_p),
                        s48_dup_socket_channel(s48_value socket_fd),
			s48_close_socket_half(s48_value socket_channel,
					      s48_value input_p),
                        s48_get_host_by_name(s48_value machine),
                        s48_get_host_by_name_result(void),
                        s48_get_host_by_address(s48_value address),
                        s48_get_host_by_address_result(void),
			s48_get_host_name(void),
			s48_udp_send(s48_value channel,
				     s48_value udp_address,
				     s48_value buffer,
				     s48_value length),
			s48_udp_receive(s48_value channel, s48_value message),
			s48_lookup_udp_address(s48_value address, s48_value port);

/* Forward declaration. */
static s48_value dup_socket_channel(int socket_fd);

/*
 * Record type imported from Scheme.
 */

static s48_value	s48_udp_address_type_binding = S48_FALSE;

/*
 * An array of udp_addresess (and S48_FALSE's) acting as a hash table.
 * The initial size is small so that rehashing will be tested often.
 */

#define INITIAL_CONNECTIONS_SIZE	16

static s48_value	connections = S48_FALSE;
static long		connections_size = INITIAL_CONNECTIONS_SIZE; 
static long		connections_index_mask = INITIAL_CONNECTIONS_SIZE - 1;
static long		connection_count = 0;

/*
 * For asynch gethostbyname.
 */

long gethostby_event_uid;


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
  S48_EXPORT_FUNCTION(s48_dup_socket_channel);
  S48_EXPORT_FUNCTION(s48_close_socket_half);
  S48_EXPORT_FUNCTION(s48_get_host_by_name);
  S48_EXPORT_FUNCTION(s48_get_host_by_name_result);
  S48_EXPORT_FUNCTION(s48_get_host_by_address);
  S48_EXPORT_FUNCTION(s48_get_host_by_address_result);
  S48_EXPORT_FUNCTION(s48_get_host_name);
  
  S48_EXPORT_FUNCTION(s48_udp_send);
  S48_EXPORT_FUNCTION(s48_udp_receive);
  S48_EXPORT_FUNCTION(s48_lookup_udp_address);

  /* we need this even if we don't do asynch lookups */
  gethostby_event_uid = s48_external_event_uid("s48_event_gethostby");

  S48_GC_PROTECT_GLOBAL(s48_udp_address_type_binding);
  s48_udp_address_type_binding = s48_get_imported_binding("s48-udp-address-type");

  S48_GC_PROTECT_GLOBAL(connections);
  connections = s48_make_vector(INITIAL_CONNECTIONS_SIZE, S48_FALSE);
}

/*
 * Create an internet-domain stream (reliable, sequenced) socket.
 * We return an input channel on success and raise an exception on failure.
 * The socket has been made non-blocking.
 */

static s48_value
s48_socket(s48_value udp_p, s48_value input_p)
{
  int		fd,
    		mode,
    		status;
  s48_value	channel;
  int on = 1;

  RETRY_OR_RAISE_NEG(fd, socket(PF_INET,
				(udp_p == S48_FALSE) ?
				  SOCK_STREAM :
				  SOCK_DGRAM,
				0));
  RETRY_OR_RAISE_NEG(status, fcntl(fd, F_SETFL, O_NONBLOCK));

  /*
   * If we don't do this, we may get "Address already in use" if we
   * try to do anything on the same port too soon.
   * Alan Bawden says this is OK:
   * http://news.gmane.org/gmane.lisp.scheme.scheme48/cutoff=1672
   */
  RETRY_OR_RAISE_NEG(status,
		     setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
				&on, sizeof(on)));

  mode = (input_p == S48_FALSE) ?
    S48_CHANNEL_STATUS_SPECIAL_OUTPUT :
    S48_CHANNEL_STATUS_SPECIAL_INPUT;

  channel = s48_add_channel(mode, s48_enter_string_latin_1("socket"), fd);

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
			port,
    			status;
  struct sockaddr_in	address;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  if (port_number == S48_FALSE)
    port = 0;
  else
    port = s48_extract_fixnum(port_number);

  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(port);

  RETRY_OR_RAISE_NEG(status,
		     bind(socket_fd,
			  (struct sockaddr *)&address,
			  sizeof(address)));

  return S48_UNSPECIFIC;
}

/*
 * Return the port number associated with an internet stream socket.
 */

static s48_value
s48_socket_number(s48_value channel)
{
  int			socket_fd,
			status;
  socklen_t		len;
  struct sockaddr_in	address;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  address.sin_addr.s_addr = htonl(INADDR_ANY);

  len = sizeof(address);

  RETRY_NEG(status, getsockname(socket_fd, (struct sockaddr *)&address, &len));

  if ((status < 0) || (address.sin_family != AF_INET))
    s48_raise_os_error(errno);

  return s48_enter_fixnum(ntohs(address.sin_port));
}

static s48_value
s48_listen(s48_value channel)
{
  int			socket_fd;
  int			status;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  RETRY_OR_RAISE_NEG(status, listen(socket_fd, LISTEN_QUEUE_SIZE));

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
s48_accept(s48_value channel, s48_value retry_p)
{
  int			socket_fd,
   			connect_fd,
			status;
  socklen_t		len;
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
    
    RETRY_OR_RAISE_NEG(status, fcntl(connect_fd, F_SETFL, O_NONBLOCK));

    input_channel = s48_add_channel(S48_CHANNEL_STATUS_INPUT,
				    s48_enter_string_latin_1("socket connection"),
				    connect_fd);

    if (!S48_CHANNEL_P(input_channel)) {
      ps_close_fd(connect_fd);		/* retries if interrupted */
      s48_raise_scheme_exception(s48_extract_fixnum(input_channel), 0); };
    
    return input_channel;
  }

  /*
   * Check for errors.  If we need to retry we mark the socket as pending
   * and return #F to tell the Scheme procedure to wait.
   */

  if ((errno != EWOULDBLOCK) && (errno != EINTR) && (errno != EAGAIN))
    s48_raise_os_error(errno);

  if (! s48_add_pending_fd(socket_fd, PSTRUE))
    s48_raise_out_of_memory_error();

  return S48_FALSE;
}

static s48_value
call_gethostbyaddr(s48_value sch_addr)
{
  char *addr = s48_extract_byte_vector(sch_addr);
  char		*hostname;
  struct hostent *hostdata;
  
  hostdata = gethostbyaddr(addr,
			   sizeof(struct in_addr),
			   AF_INET);
  if (hostdata == NULL)
    hostname = inet_ntoa(*((struct in_addr *) addr));
  else
    hostname = hostdata->h_name;
  
  return s48_enter_string_latin_1(hostname);
}

#ifdef HAVE_PTHREAD_H
#define S48_MAX_HOST_NAME_LENGTH 128
static char host_name[S48_MAX_HOST_NAME_LENGTH + 1]; /* is enough */
static char host_address[128]; /* should be enough */
int host_address_size;
int host_address_errno;

/*
 * Only one call can be active at any given time:
 * We expect mutual exclusion at the Scheme end
 */

static void
gethostbyname_thread(void *ignore)
{
  static struct hostent *host; /* will be thread-local */

  RETRY_NULL(host, gethostbyname(host_name));
  if (host == NULL)
      host_address_errno = h_errno;
  else
    {
      host_address_errno = 0;
      host_address_size = host->h_length;
      memcpy(host_address, host->h_addr, host_address_size);
    }
  s48_note_external_event(gethostby_event_uid);
}

static s48_value
s48_get_host_by_name(s48_value machine)
{
  pthread_t t;

  int machine_length = S48_BYTE_VECTOR_LENGTH(machine);
  if (machine_length > S48_MAX_HOST_NAME_LENGTH)
    s48_raise_argument_type_error(machine);

  memcpy(host_name, s48_extract_byte_vector(machine), machine_length);
  host_name[machine_length] = '\0';

  if (pthread_create(&t, NULL, gethostbyname_thread, NULL))
    {
      static struct hostent *host;
      RETRY_NULL(host, gethostbyname(host_name));
      if (host == NULL)
	s48_raise_os_error(h_errno);
      return s48_enter_byte_vector(host->h_addr, host->h_length);
    }
  else
    {
      pthread_detach(t);
      return S48_FALSE;
    }
}

static s48_value
s48_get_host_by_name_result(void)
{
  if (host_address_errno)
    s48_raise_os_error(host_address_errno);
  return s48_enter_byte_vector(host_address, host_address_size);
}

/*
 * Get the name of the `sch_addr''s host.  If we can't get the real
 * host name we use the numbers-and-dots representation of the
 * address.
 */


static void
gethostbyaddr_thread(void *ignore)
{
  struct in_addr addr;
  struct hostent *hostdata;
  
  hostdata = gethostbyaddr(host_address,
			   sizeof(struct in_addr),
			   AF_INET);
  if (hostdata == NULL)
    strcpy(host_name, inet_ntoa(*((struct in_addr *) host_address)));
  else
    strcpy(host_name, hostdata->h_name);

  s48_note_external_event(gethostby_event_uid);
}
 
static s48_value
s48_get_host_by_address(s48_value sch_addr)
{
  pthread_t t;

  host_address_size = sizeof(struct in_addr);
  memcpy(host_address,
	 (char *) s48_extract_byte_vector(sch_addr), host_address_size);

  if (pthread_create(&t, NULL, gethostbyaddr_thread, NULL))
      return call_gethostbyaddr(sch_addr);
  else
    {
      pthread_detach(t);
      return S48_FALSE;
    }
}
	 
static s48_value 
s48_get_host_by_address_result(void)
{
  return s48_enter_string_latin_1(host_name);
}
#else
static s48_value
s48_get_host_by_name(s48_value machine)
{
  struct hostent *host;
  char* machine_name = s48_extract_byte_vector(machine);
  RETRY_NULL(host, gethostbyname(machine_name));
  if (host == NULL)
    s48_raise_os_error(h_errno);
  return s48_enter_byte_vector(host->h_addr, host->h_length);
}

static s48_value
s48_get_host_by_name_result(void)
{
  /* won't be used here */
}

/*
 * Get the name of the `sch_addr''s host.  If we can't get the real
 * host name we use the numbers-and-dots representation of the
 * address.
 */
 
static s48_value
s48_get_host_by_address(s48_value sch_addr)
{
  return call_gethostbyaddr(sch_addr);
}

static s48_value
s48_get_host_by_address_result(void)
{
  /* won't be used here */
}
#endif


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
s48_connect(s48_value channel,
	    s48_value sch_address,
	    s48_value port,
	    s48_value retry_p)
{
  int			socket_fd,
    			port_number;
  struct sockaddr_in	address;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  S48_CHECK_FIXNUM(port);
  port_number = S48_UNSAFE_EXTRACT_FIXNUM(port);
  
  /*
   * Initialize `address'.
   */

  memset((void *)&address, 0, sizeof(address));
  address.sin_family = AF_INET;

  if (S48_BYTE_VECTOR_LENGTH(sch_address) > sizeof(address.sin_addr))
    s48_raise_range_error(s48_enter_fixnum(S48_BYTE_VECTOR_LENGTH(sch_address)),
			  S48_UNSAFE_ENTER_FIXNUM(0),
			  s48_enter_fixnum(sizeof(address.sin_addr)));
  memcpy((void *)&address.sin_addr,
	 (void *)s48_extract_byte_vector(sch_address),
	 S48_BYTE_VECTOR_LENGTH(sch_address));
  address.sin_port = htons(port_number);

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

  if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) >= 0
      || ((errno == EISCONN) && (retry_p == S48_TRUE))) {
    S48_STOB_SET(channel, S48_CHANNEL_STATUS_OFFSET, S48_CHANNEL_STATUS_INPUT);
    
    return dup_socket_channel(socket_fd); }
    
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

  if (! (s48_add_pending_fd(socket_fd, PSFALSE)))
    s48_raise_out_of_memory_error();

  return S48_FALSE;
}

/*
 * dup() `socket_fd' and return an output channel holding the result.
 *
 * We have to versions, one for calling from C and one for calling from Scheme.
 */

static s48_value
s48_dup_socket_channel(s48_value channel)
{
  int			socket_fd;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  
  return dup_socket_channel(socket_fd);
}

static s48_value
dup_socket_channel(int socket_fd)
{
  int 			output_fd;
  s48_value		output_channel;

  RETRY_OR_RAISE_NEG(output_fd, dup(socket_fd));

  output_channel = s48_add_channel(S48_CHANNEL_STATUS_OUTPUT,
				   s48_enter_string_latin_1("socket connection"),
				   output_fd);
  
  if (!S48_CHANNEL_P(output_channel)) {
    ps_close_fd(output_fd);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(output_channel), 0); };
  
  return output_channel;
}

/*
 * Close half of a socket; if `input_p' is true we close the input half,
 * otherwise the output half.  This horribleness is forced upon us by
 * Unix's use of bidirectional file descriptors.
 */

static s48_value
s48_close_socket_half(s48_value channel, s48_value input_p)
{
  int	socket_fd,
    	status;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  /* We ignore `endpoint is not connected' errors, as we just want to get
     the file descriptor closed. */
  RETRY_NEG(status, shutdown(socket_fd, S48_EXTRACT_BOOLEAN(input_p) ? 0 : 1));
  if ((0 > status) && (errno != ENOTCONN))
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
  int	status;

  RETRY_OR_RAISE_NEG(status, gethostname(mbuff, sizeof(mbuff)));

  return s48_enter_string_latin_1(mbuff);
}

/*
 * UDP sockets.
 */

/*
 * A udp_address is a Scheme record with the following fields:
 *   address		; an in_addr inside a Scheme byte vector
 *   port		; the port as a fixnum
 *   name		; printable version of the name as a Scheme string
 *   + possibly others that we don't care about
 */

#define UDP_ADDRESS_PTR(udp)						\
(S48_UNSAFE_EXTRACT_VALUE_POINTER(S48_UNSAFE_RECORD_REF((udp), 0),	\
			          struct in_addr))

#define UDP_PORT(udp)							\
(S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_RECORD_REF((udp), 1)))

/*
 * Forward declaration of internal procedures.
 */

static void		connection_init();
static s48_value	address_connection(struct sockaddr_in *addr);
static s48_value	lookup_connection(struct in_addr address,
					  unsigned long port);
static s48_value	address_hash(struct in_addr address,
				     unsigned long port);
static s48_value	add_new_connection(int index,
					   struct in_addr address,
					   unsigned long port);
static void		expand_and_rehash();
static s48_value	get_hostname(struct in_addr addr);

/*
 * Receive a message.  Returns pair (<byte-count> . <sender>).
 */

static s48_value
s48_udp_receive(s48_value channel, s48_value buffer)
{
  int			socket_fd;
  struct sockaddr_in	from;
  socklen_t		from_len = sizeof(struct sockaddr_in);
  int			count;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  S48_CHECK_VALUE(buffer);
  
  count = recvfrom(socket_fd,
		   S48_UNSAFE_EXTRACT_VALUE_POINTER(buffer, void *),
		   S48_UNSAFE_BYTE_VECTOR_LENGTH(buffer),
		   0,
		   (struct sockaddr*)&from,
		   &from_len);
  
  if (0 <= count)
    return s48_cons(S48_UNSAFE_ENTER_FIXNUM(count),
		    address_connection(&from));
		    
  /*
   * Check for errors.  If we need to retry we mark the socket as pending
   * and return #F to tell the Scheme procedure to wait.
   */
  
  if (errno != EWOULDBLOCK && errno != EINTR && errno != EALREADY
      && errno != EINPROGRESS && errno != EAGAIN)
    s48_raise_os_error(errno);

  if (! (s48_add_pending_fd(socket_fd, PSTRUE)))
    s48_raise_out_of_memory_error();

  return S48_FALSE;
}

static s48_value
s48_udp_send(s48_value channel,
		s48_value address,
		s48_value buffer,
		s48_value count)
{
  int socket_fd;
  int sent;
  struct sockaddr_in	to;
  
  S48_CHECK_CHANNEL(channel);
  s48_check_record_type(address, s48_udp_address_type_binding);
  S48_CHECK_VALUE(buffer);
  S48_CHECK_FIXNUM(count);
  
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));

  memset(&to, 0, sizeof(to));
  to.sin_family = AF_INET;
  to.sin_addr = *(UDP_ADDRESS_PTR(address));
  to.sin_port = htons(UDP_PORT(address));

  sent = sendto(socket_fd,
		S48_UNSAFE_EXTRACT_VALUE_POINTER(buffer, void *),
		S48_UNSAFE_EXTRACT_FIXNUM(count),
		0,
		(struct sockaddr *)&to,
		sizeof(struct sockaddr_in));
  
  if (0 <= sent)
    return S48_UNSAFE_ENTER_FIXNUM(sent);

  /*
   * Check for errors.  If we need to retry we mark the socket as pending
   * and return #F to tell the Scheme procedure to wait.
   */
  
  if (errno != EWOULDBLOCK && errno != EINTR && errno != EALREADY
      && errno != EINPROGRESS && errno != EAGAIN)
    s48_raise_os_error(errno);

  if (! (s48_add_pending_fd(socket_fd, PSFALSE)))
    s48_raise_out_of_memory_error();

  return S48_FALSE;
}

/*
 * We keep a hash table of the sockets we have corresponded with.
 *
 * Get the connection struct for `addr', creating a new one if necessary.
 * We hash the address and port to get an initial index into the `connections'
 * array and then search linearly from there.
 */

static s48_value
address_connection(struct sockaddr_in *addr)
{
  return lookup_connection(addr->sin_addr, ntohs(addr->sin_port));
}

static s48_value
s48_lookup_udp_address(s48_value address, s48_value port)
{
  return lookup_connection(*((struct in_addr *) s48_extract_byte_vector(address)),
			   s48_extract_fixnum(port));
}

/*
 * `port' should be in host byte order.
 */

static s48_value
lookup_connection(struct in_addr address, unsigned long port)
{
  unsigned long hash = address_hash(address, port);
  int		i = hash & connections_index_mask;

  while (1) {
    s48_value	search = S48_UNSAFE_VECTOR_REF(connections, i);
    if (search == S48_FALSE)
      return add_new_connection(i, address, port);
    else if ((address.s_addr == UDP_ADDRESS_PTR(search)->s_addr)
	      && port == UDP_PORT(search))
      return search;
    else
      i = (i + 1) & connections_index_mask; }
}

static s48_value
address_hash(struct in_addr address, unsigned long port)
{
  unsigned long hash = port ^ address.s_addr;

  return (hash >> 16) * (hash & 0xFFFF);
}

/*
 * Add a new connection record for `addr', putting it at `index' in the
 * connections' hash table. 
 */

static s48_value
add_new_connection(int index, struct in_addr address, unsigned long port)
{
  s48_value	udp_address;
  s48_value	sch_address;
  s48_value	name;

  S48_DECLARE_GC_PROTECT(1);

  udp_address = s48_make_record(s48_udp_address_type_binding);

  S48_GC_PROTECT_1(udp_address);

  sch_address
    = s48_enter_byte_vector((char*) (&address), sizeof(struct in_addr)); /* may GC */
  
  S48_UNSAFE_RECORD_SET(udp_address, 0, sch_address);
  S48_UNSAFE_RECORD_SET(udp_address, 1, S48_UNSAFE_ENTER_FIXNUM(port));
  
  S48_UNSAFE_RECORD_SET(udp_address, 2, S48_FALSE);

  S48_UNSAFE_VECTOR_SET(connections, index, udp_address);
  connection_count += 1;
  if (connection_count * 3 > connections_size)
    expand_and_rehash();				/* may GC */

  S48_GC_UNPROTECT();
  
  return udp_address;
}

/*
 * Double the size of the hash table and rehash all connections into it.
 */
    
static void
expand_and_rehash()
{
  s48_value	new_connections;
  long		new_size = connections_size * 2;
  long		new_index_mask = new_size - 1;
  long		i;

  new_connections = s48_make_vector(new_size, S48_FALSE);

  for(i = 0; i < connections_size; i++) {
    s48_value next = S48_UNSAFE_VECTOR_REF(connections, i);
    if (next != S48_FALSE) {
      i = address_hash(*UDP_ADDRESS_PTR(next), UDP_PORT(next))
	  & new_index_mask;
      for (; S48_UNSAFE_VECTOR_REF(new_connections, i) != S48_FALSE;
	   i = (i + 1) & new_index_mask);
      S48_UNSAFE_VECTOR_SET(new_connections, i, next); }}

  connections = new_connections;
  connections_size = new_size;
  connections_index_mask = new_index_mask;
}
