/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * More or less platform-unspecific socket stuff.
 */

#include <stdlib.h>

#ifdef _WIN32
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <ws2tcpip.h>

#define RETRY_NEG(STATUS, CALL) STATUS = (CALL)
#define RETRY_OR_RAISE_NEG(STATUS, CALL)			\
do {								\
    STATUS = (CALL);						\
    if (STATUS == SOCKET_ERROR)					\
      s48_os_error(NULL, WSAGetLastError(), 0);			\
 } while (0)

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>

#include "unix.h"
#endif

#include <scheme48.h>

#include "address.h"
#include "socket.h"

/* The C code knows about these constants. */

static int
extract_how(s48_value sch_how)
{
  long how_val = s48_extract_fixnum(sch_how);
  switch (how_val)
    {
#ifdef _WIN32
    case 0:
      return SD_RECEIVE;
    case 1:
      return SD_SEND;
    case 2:
      return SD_BOTH;
#else
    case 0:
      return SHUT_RD;
    case 1:
      return SHUT_WR;
    case 2:
      return SHUT_RDWR;
#endif
    }
}

int
s48_extract_msg_flags(s48_value sch_flags)
{
  long flags = s48_extract_fixnum(sch_flags);
  return (((flags & 0x01) ? MSG_OOB : 0)
	  | ((flags & 0x02) ? MSG_PEEK : 0)
	  | ((flags & 0x04) ? MSG_DONTROUTE : 0));
} 

/*
 * Close half of a socket; if `input_p' is true we close the input half,
 * otherwise the output half.  This horribleness is forced upon us by
 * Unix's use of bidirectional file descriptors.
 */

static s48_value
s48_shutdown(s48_value sch_channel, s48_value sch_how)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int how = extract_how(sch_how);
  int status;

  /*
   * We ignore `endpoint is not connected' errors, as we just want to get
   * the file descriptor closed.
   */
  RETRY_NEG(status, shutdown(socket_fd, how));
#ifdef _WIN32
  if ((status == SOCKET_ERROR) && (WSAGetLastError() != WSAENOTCONN))
#else
  if ((0 > status) && (errno != ENOTCONN))
#endif
    s48_os_error("s48_close_socket_half", errno, 1, sch_channel);
  
  return S48_UNSPECIFIC;
}

static s48_value
s48_bind(s48_value sch_channel, s48_value sch_saddr)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int status;
  const struct sockaddr *sa
    = S48_EXTRACT_VALUE_POINTER(sch_saddr, const struct sockaddr);
  socklen_t salen = S48_VALUE_SIZE(sch_saddr);

  RETRY_OR_RAISE_NEG(status, bind(socket_fd, sa, salen));

  return S48_UNSPECIFIC;
}

static s48_value
s48_listen(s48_value sch_channel, s48_value sch_queue_size)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int queue_size = s48_extract_integer(sch_queue_size);
  int status;

  RETRY_OR_RAISE_NEG(status, listen(socket_fd, queue_size));

  return S48_UNSPECIFIC;
}

static s48_value
s48_max_connection_count(void)
{
  /* not a fixnum on Windows! */
  return s48_enter_integer(SOMAXCONN);
}

static s48_value
s48_getsockname(s48_value sch_channel)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int status;
  struct sockaddr_storage sa;
  socklen_t salen = sizeof(struct sockaddr_storage);
  
  RETRY_OR_RAISE_NEG(status,
		     getsockname(socket_fd, (struct sockaddr*)&sa, &salen));
  return s48_enter_sockaddr((struct sockaddr *)&sa, salen);
}

static s48_value
s48_getpeername(s48_value sch_channel)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int status;
  struct sockaddr_storage sa;
  socklen_t salen = sizeof(struct sockaddr_storage);
  
  RETRY_OR_RAISE_NEG(status,
		     getpeername(socket_fd, (struct sockaddr*)&sa, &salen));
  return s48_enter_sockaddr((struct sockaddr *)&sa, salen);
}

static s48_value
setsockopt_boolean(int level, int option,
		   s48_value sch_channel, s48_value sch_val)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int status, on;

  on = (S48_EQ_P(sch_val, S48_FALSE) ? 0 : 1);
  RETRY_OR_RAISE_NEG(status, \
		     setsockopt(socket_fd, level, option,
				(void*)&on, sizeof(on)));
  return S48_UNSPECIFIC;
}

static s48_value
getsockopt_boolean(int level, int option,
		   s48_value sch_channel)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int status, on;
  socklen_t onlen = sizeof(on);

  RETRY_OR_RAISE_NEG(status,
		     getsockopt(socket_fd, level, option,
				(void*)&on, &onlen));
  return (on ? S48_TRUE : S48_FALSE);
}

static s48_value
setsockopt_int(int level, int option,
	       s48_value sch_channel, s48_value sch_val)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int status, val;

  val = s48_extract_integer(sch_val);
  RETRY_OR_RAISE_NEG(status,
		     setsockopt(socket_fd, level, option,
				(void*)&val, sizeof(val)));
  return S48_UNSPECIFIC;
}

static s48_value
getsockopt_int(int level, int option,
	       s48_value sch_channel)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int status, val;
  socklen_t vallen = sizeof(val);

  RETRY_OR_RAISE_NEG(status,
		     getsockopt(socket_fd, level, option,
				(void*)&val, &vallen));
  return s48_enter_integer(val);
}

#define DEFINE_SOCKET_OPTION_SETTER(name, level, option, type) \
static s48_value \
name(s48_value sch_channel, s48_value sch_val) \
{ \
  return setsockopt_##type(level, option, \
			   sch_channel, sch_val); \
}

#define DEFINE_SOCKET_OPTION_GETTER(name, level, option, type) \
static s48_value \
name(s48_value sch_channel) \
{ \
  return getsockopt_##type(level, option, \
			   sch_channel); \
}

/* POSIX */


DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_DEBUG, SOL_SOCKET, SO_DEBUG, boolean)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_DEBUG, SOL_SOCKET, SO_DEBUG, boolean)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_ACCEPTCONN, SOL_SOCKET, SO_ACCEPTCONN, boolean)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_ACCEPTCONN, SOL_SOCKET, SO_ACCEPTCONN, boolean)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_BROADCAST, SOL_SOCKET, SO_BROADCAST, boolean)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_BROADCAST, SOL_SOCKET, SO_BROADCAST, boolean)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_REUSEADDR, SOL_SOCKET, SO_REUSEADDR, boolean)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_REUSEADDR, SOL_SOCKET, SO_REUSEADDR, boolean)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_KEEPALIVE, SOL_SOCKET, SO_KEEPALIVE, boolean)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_KEEPALIVE, SOL_SOCKET, SO_KEEPALIVE, boolean)
/* LINGER leads to close() blocking */
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_OOBINLINE, SOL_SOCKET, SO_OOBINLINE, boolean)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_OOBINLINE, SOL_SOCKET, SO_OOBINLINE, boolean)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_SNDBUF, SOL_SOCKET, SO_SNDBUF, int)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_SNDBUF, SOL_SOCKET, SO_SNDBUF, int)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_RCVBUF, SOL_SOCKET, SO_RCVBUF, int)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_RCVBUF, SOL_SOCKET, SO_RCVBUF, int)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_ERROR, SOL_SOCKET, SO_ERROR, int)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_DONTROUTE, SOL_SOCKET, SO_DONTROUTE, boolean)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_DONTROUTE, SOL_SOCKET, SO_DONTROUTE, boolean)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_RCVLOWAT, SOL_SOCKET, SO_RCVLOWAT, int)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_RCVLOWAT, SOL_SOCKET, SO_RCVLOWAT, int)
/* SO_RCVTIMEO makes input functions block */
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_SO_SNDLOWAT, SOL_SOCKET, SO_SNDLOWAT, int)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_SO_SNDLOWAT, SOL_SOCKET, SO_SNDLOWAT, int)
/* SO_SNDTIMEO makes output functions block */

/* RFC 3493 */

DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_IPV6_UNICAST_HOPS, SOL_SOCKET, IPV6_UNICAST_HOPS, int)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_IPV6_UNICAST_HOPS, SOL_SOCKET, IPV6_UNICAST_HOPS, int)
/* IPV6_MULTICAST_IF is really unsigned */
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_IPV6_MULTICAST_IF, SOL_SOCKET, IPV6_MULTICAST_IF, int)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_IPV6_MULTICAST_IF, SOL_SOCKET, IPV6_MULTICAST_IF, int)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_IPV6_MULTICAST_HOPS, SOL_SOCKET, IPV6_MULTICAST_HOPS, int)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_IPV6_MULTICAST_HOPS, SOL_SOCKET, IPV6_MULTICAST_HOPS, int)
DEFINE_SOCKET_OPTION_SETTER(s48_setsockopt_IPV6_MULTICAST_LOOP, SOL_SOCKET, IPV6_MULTICAST_LOOP, boolean)
DEFINE_SOCKET_OPTION_GETTER(s48_getsockopt_IPV6_MULTICAST_LOOP, SOL_SOCKET, IPV6_MULTICAST_LOOP, boolean)

static s48_value
ipv6_socket_group_op(int group_op,
		     s48_value sch_channel,
		     s48_value sch_address,
		     s48_value sch_if)
{
  socket_t socket_fd = s48_extract_socket_fd(sch_channel);
  int status;
  struct ipv6_mreq mreq;
  s48_extract_in6_addr(sch_address, &(mreq.ipv6mr_multiaddr));
  mreq.ipv6mr_interface = s48_extract_unsigned_integer(sch_if);

  RETRY_OR_RAISE_NEG(status,
		     setsockopt(socket_fd, IPPROTO_IPV6, group_op,
				(void*)&mreq, sizeof(struct ipv6_mreq)));
  return S48_UNSPECIFIC;
}

static s48_value
s48_ipv6_socket_join_group(s48_value sch_channel,
			   s48_value sch_address,
			   s48_value sch_if)
{
  return ipv6_socket_group_op(IPV6_JOIN_GROUP,
			      sch_channel, sch_address, sch_if);
}

static s48_value
s48_ipv6_socket_leave_group(s48_value sch_channel,
			    s48_value sch_address,
			    s48_value sch_if)
{
  return ipv6_socket_group_op(IPV6_LEAVE_GROUP,
			      sch_channel, sch_address, sch_if);
}

void
s48_init_net_sockets(void)
{
  S48_EXPORT_FUNCTION(s48_bind);
  S48_EXPORT_FUNCTION(s48_listen);
  S48_EXPORT_FUNCTION(s48_max_connection_count);
  S48_EXPORT_FUNCTION(s48_shutdown);
  S48_EXPORT_FUNCTION(s48_getsockname);
  S48_EXPORT_FUNCTION(s48_getpeername);

  S48_EXPORT_FUNCTION(s48_setsockopt_SO_DEBUG);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_DEBUG);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_ACCEPTCONN);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_BROADCAST);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_BROADCAST);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_REUSEADDR);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_REUSEADDR);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_KEEPALIVE);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_KEEPALIVE);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_OOBINLINE);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_OOBINLINE);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_SNDBUF);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_SNDBUF);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_RCVBUF);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_RCVBUF);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_ERROR);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_DONTROUTE);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_DONTROUTE);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_RCVLOWAT);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_RCVLOWAT);
  S48_EXPORT_FUNCTION(s48_setsockopt_SO_SNDLOWAT);
  S48_EXPORT_FUNCTION(s48_getsockopt_SO_SNDLOWAT);

  S48_EXPORT_FUNCTION(s48_setsockopt_IPV6_UNICAST_HOPS);
  S48_EXPORT_FUNCTION(s48_getsockopt_IPV6_UNICAST_HOPS);
  S48_EXPORT_FUNCTION(s48_setsockopt_IPV6_MULTICAST_IF);
  S48_EXPORT_FUNCTION(s48_getsockopt_IPV6_MULTICAST_IF);
  S48_EXPORT_FUNCTION(s48_setsockopt_IPV6_MULTICAST_HOPS);
  S48_EXPORT_FUNCTION(s48_getsockopt_IPV6_MULTICAST_HOPS);
  S48_EXPORT_FUNCTION(s48_setsockopt_IPV6_MULTICAST_LOOP);
  S48_EXPORT_FUNCTION(s48_getsockopt_IPV6_MULTICAST_LOOP);

  S48_EXPORT_FUNCTION(s48_ipv6_socket_join_group);
  S48_EXPORT_FUNCTION(s48_ipv6_socket_leave_group);
}
