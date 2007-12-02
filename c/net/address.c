/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Socket addresses
 */

/* Main source: RFC 3493 - Basic Socket Interface Extensions for IPv6 */

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#define HAVE_THREADS
#define DECLARE_THREAD_PROC(name, param_name) \
       DWORD WINAPI name(LPVOID param_name)
#define EXIT_THREAD_PROC() do { ExitThread(0); return 0; } while (0)
typedef HANDLE thread_t;
#define START_THREAD(desc, name, arg) \
  ((desc = CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE) name, (LPVOID) arg, 0, NULL)) == NULL)
#define DETACH_THREAD(desc) ;
#else
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/un.h>

#include "sysdep.h"
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#define HAVE_THREADS
#define DECLARE_THREAD_PROC(name, param_name) \
       void* name(void* param_name)
#define EXIT_THREAD_PROC() do { return NULL; } while (0)
typedef pthread_t thread_t;
#define START_THREAD(desc, name, arg) \
  pthread_create(&desc, NULL, name, (void*) arg)
#define DETACH_THREAD(desc) pthread_detach(desc)
#endif
#endif

#include <errno.h>

#include <scheme48.h>

#include "address.h"

static int
maybe_extract_latin1_string(s48_value sch_str, char* buffer, size_t size)
{
  if (s48_string_length(sch_str) > (size - 1))
    return 0;
  s48_copy_string_to_latin_1(sch_str, buffer);
  buffer[s48_string_length(sch_str)] = '\0';
  return 1;
}

static char*
maybe_extract_fresh_latin1_string(s48_value sch_str, size_t size)
{
  char *result;
  if (s48_string_length(sch_str) > (size - 1))
    return NULL;
  
  result = malloc(size);
  if (result == NULL)
    s48_out_of_memory_error();

  s48_copy_string_to_latin_1(sch_str, result);
  result[s48_string_length(sch_str)] = '\0';
  return result;
}
 
/* The C code knows about these constants. */

sa_family_t
s48_extract_af(s48_value sch_af_val)
{
  long af_val = s48_extract_fixnum(sch_af_val);
  if (af_val > 100) /* unknown address family */
    return af_val - 100;
  else
    switch (af_val)
    {
    case 0:
      return AF_INET;
    case 1:
      return AF_INET6;
    case 2:
      return AF_UNIX;
    case 3 :
      return AF_UNSPEC;
    }
}

s48_value
s48_enter_af(sa_family_t af)
{
  switch(af)
    {
    case AF_INET:
      return s48_enter_fixnum(0);
    case AF_INET6:
      return s48_enter_fixnum(1);
    case AF_UNIX:
      return s48_enter_fixnum(2);
    case AF_UNSPEC:
      return s48_enter_fixnum(3);
    default:
      return s48_enter_fixnum((int) af + 100);
    }
}

/* IPv4 addresses */

/* may GC */
static s48_value
enter_in_addr(const struct in_addr* addr)
{
  return s48_enter_unsigned_integer(ntohl(addr->s_addr));
}

static void
extract_in_addr(s48_value sch_addr, struct in_addr* addr)
{
  addr->s_addr = htonl(s48_extract_unsigned_integer(sch_addr));
}

/* may GC */
static s48_value
s48_enter_sockaddr_in_raw(const struct sockaddr_in *saddr)
{
  s48_value sch_native = S48_MAKE_VALUE(struct sockaddr_in);
  memcpy(S48_EXTRACT_VALUE_POINTER(sch_native, void), saddr,
	 sizeof(struct sockaddr_in));
  return sch_native;
}

/* may GC */
static s48_value
s48_enter_sockaddr_in(const struct sockaddr_in *saddr)
{
  S48_DECLARE_GC_PROTECT(1);
  s48_value sch_saddr = s48_make_vector(4, S48_UNSPECIFIC);
  S48_GC_PROTECT_1(sch_saddr);

  S48_VECTOR_SET(sch_saddr, 0, s48_enter_sockaddr_in_raw(saddr));
  S48_VECTOR_SET(sch_saddr, 1, s48_enter_af(AF_INET));
  S48_VECTOR_SET(sch_saddr, 2, s48_enter_fixnum(ntohs(saddr->sin_port)));
  S48_VECTOR_SET(sch_saddr, 3, enter_in_addr(&(saddr->sin_addr)));

  S48_GC_UNPROTECT();
  return sch_saddr;
}

static s48_value
s48_make_sockaddr_in_raw(s48_value sch_in_addr, s48_value sch_port)
{
  struct sockaddr_in saddr;
#ifdef SIN6_LEN
  /* sockaddr addresses either all have the length, or none has */
  saddr.sin_len = sizeof(struct sockaddr_in);
#endif
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(s48_extract_fixnum(sch_port));
  extract_in_addr(sch_in_addr, &(saddr.sin_addr));
  return s48_enter_sockaddr_in_raw(&saddr);
}

static s48_value
s48_get_inaddr_any(void)
{
  struct in_addr addr;
  addr.s_addr = htonl(INADDR_ANY);
  return enter_in_addr(&addr);
}

static s48_value
s48_get_inaddr_broadcast(void)
{
  struct in_addr addr;
  addr.s_addr = htonl(INADDR_BROADCAST);
  return enter_in_addr(&addr);
}

/* IPv6 addresses */

/* may GC */
s48_value
s48_enter_in6_addr(const struct in6_addr* addr)
{
  s48_value sch_addr = s48_make_byte_vector(16);
  char* bytes = s48_extract_byte_vector(sch_addr);
  int i = 0;
  while (i < 16)
    {
      bytes[i] = (char) addr->s6_addr[i];
      ++i;
    }
  return sch_addr;
}

void
s48_extract_in6_addr(s48_value sch_addr, struct in6_addr* addr)
{
  char* bytes = s48_extract_byte_vector(sch_addr);
  int i = 0;
  while (i < 16)
    {
      addr->s6_addr[i] = (uint8_t) bytes[i];
      ++i;
    }
}

/* may GC */
static s48_value
s48_enter_sockaddr_in6_raw(const struct sockaddr_in6 *saddr)
{
  s48_value sch_native = S48_MAKE_VALUE(struct sockaddr_in6);
  memcpy(S48_EXTRACT_VALUE_POINTER(sch_native, void), saddr,
	 sizeof(struct sockaddr_in6));
  return sch_native;
}

/* may GC */
static s48_value
s48_enter_sockaddr_in6(const struct sockaddr_in6 *saddr)
{
  S48_DECLARE_GC_PROTECT(1);
  s48_value sch_saddr = s48_make_vector(5, S48_UNSPECIFIC);
  S48_GC_PROTECT_1(sch_saddr);

  S48_VECTOR_SET(sch_saddr, 0, s48_enter_sockaddr_in6_raw(saddr));
  S48_VECTOR_SET(sch_saddr, 1, s48_enter_af(AF_INET6));
  S48_VECTOR_SET(sch_saddr, 2, s48_enter_fixnum(ntohs(saddr->sin6_port)));
  /* flowinfo is insignificant */
  S48_VECTOR_SET(sch_saddr, 3, s48_enter_in6_addr(&(saddr->sin6_addr)));
  S48_VECTOR_SET(sch_saddr, 4, s48_enter_unsigned_integer(saddr->sin6_scope_id));

  S48_GC_UNPROTECT();
  return sch_saddr;
}

static s48_value
s48_make_sockaddr_in6_raw(s48_value sch_addr, s48_value sch_port,
			  s48_value sch_scope_id)
{
  struct sockaddr_in6 saddr;
#ifdef SIN6_LEN
  saddr.sin6_len = sizeof(struct sockaddr_in6);
#endif
  saddr.sin6_family = AF_INET6;
  saddr.sin6_port = htons(s48_extract_fixnum(sch_port));
  saddr.sin6_flowinfo = 0;
  s48_extract_in6_addr(sch_addr, &(saddr.sin6_addr));
  saddr.sin6_scope_id = s48_extract_unsigned_integer(sch_scope_id);
  return s48_enter_sockaddr_in6_raw(&saddr);
}

static s48_value
s48_get_in6addr_any(void)
{
  return s48_enter_in6_addr(&in6addr_any);
}

static s48_value
s48_get_in6addr_loopback(void)
{
  return s48_enter_in6_addr(&in6addr_loopback);
}

#ifdef SUN_LEN
/* BSD 4.4 */
#define SOCKADDR_UN_LEN(su) SUN_LEN(&su)
#else
#define SOCKADDR_UN_LEN(su) (sizeof(su) - sizeof((su).sun_path) + strlen((su).sun_path))
#endif

#ifndef _WIN32

/* Unix domain addresses */

/* may GC */
static s48_value
s48_enter_sockaddr_un_raw(const struct sockaddr_un *saddr)
{
  size_t len = SOCKADDR_UN_LEN(*saddr);
  s48_value sch_native = S48_MAKE_SIZED_VALUE(len);
  memcpy(S48_EXTRACT_VALUE_POINTER(sch_native, void), saddr, len+1);
  return sch_native;
}

static s48_value
s48_make_sockaddr_un_raw(s48_value sch_path)
{
  struct sockaddr_un saddr;
  size_t max_path_len = sizeof(saddr) + ((char*)&(saddr.sun_path) - (char*)&saddr);
  s48_value sch_native;

  if (S48_BYTE_VECTOR_LENGTH(sch_path) > max_path_len)
    s48_assertion_violation("s48_make_sockaddr_un_raw", "path too long", 1,
			    sch_path);
  
  saddr.sun_family = AF_UNIX;
  strcpy(saddr.sun_path, s48_extract_byte_vector(sch_path));

#ifdef SIN6_LEN
  saddr.sun_len = SUN_LEN(&saddr);
#endif

  return s48_enter_sockaddr_un_raw(&saddr);
}


/* may GC */
static s48_value
s48_enter_sockaddr_un(const struct sockaddr_un *saddr)
{
  S48_DECLARE_GC_PROTECT(1);
  s48_value sch_saddr = s48_make_vector(3, S48_UNSPECIFIC);
  S48_GC_PROTECT_1(sch_saddr);

  S48_VECTOR_SET(sch_saddr, 0, s48_enter_sockaddr_un_raw(saddr));
  S48_VECTOR_SET(sch_saddr, 1, s48_enter_af(AF_UNIX));
  S48_VECTOR_SET(sch_saddr, 2, s48_enter_byte_string((char*)saddr->sun_path));

  S48_GC_UNPROTECT();
  return sch_saddr;
		
}

#endif /* !_WIN32 */

/* Generic addresses */

/* may GC */
static s48_value
s48_enter_sockaddr_unknown_raw(const struct sockaddr *saddr, socklen_t addrlen)
{
  s48_value sch_native = S48_MAKE_SIZED_VALUE(addrlen);
  memcpy(S48_EXTRACT_VALUE_POINTER(sch_native, void), saddr,
	 addrlen);
  return sch_native;
}


s48_value
s48_enter_sockaddr(const struct sockaddr* saddr, socklen_t addrlen)
{
  switch (saddr->sa_family)
    {
    case AF_INET:
      return s48_enter_sockaddr_in((const struct sockaddr_in*) saddr);
    case AF_INET6:
      return s48_enter_sockaddr_in6((const struct sockaddr_in6*) saddr);
#ifndef _WIN32
    case AF_UNIX:
      return s48_enter_sockaddr_un((const struct sockaddr_un*) saddr);
#endif
    default:
      {
	S48_DECLARE_GC_PROTECT(1);
	s48_value sch_saddr = s48_make_vector(2, S48_UNSPECIFIC);
	S48_GC_PROTECT_1(sch_saddr);
	
	S48_VECTOR_SET(sch_saddr, 0, s48_enter_sockaddr_unknown_raw(saddr, addrlen));
	S48_VECTOR_SET(sch_saddr, 1, s48_enter_af(saddr->sa_family));
	
	S48_GC_UNPROTECT();
	return sch_saddr;
      }
    }
}

/* Interfaces */

#ifndef _WIN32 /* supposedly available on Vista, so there's hope */

/* note an error is indicated by 0 */
static s48_value
s48_if_nametoindex(s48_value sch_ifname)
{
  char name[IF_NAMESIZE];
  if (!maybe_extract_latin1_string(sch_ifname, name, sizeof(name)))
    return s48_enter_fixnum(0);
  return s48_enter_unsigned_integer(if_nametoindex(name));
}

static s48_value
s48_if_indextoname(s48_value sch_ifindex)
{
  char ifname[IF_NAMESIZE];
  if (if_indextoname(s48_extract_unsigned_integer(sch_ifindex), ifname) != NULL)
    return s48_enter_string_latin_1(ifname);
  else
    s48_os_error("s48_if_indextoname", errno, 1, sch_ifindex);
}

/* Return a vector with alternating names and indices. */
static s48_value
s48_if_nameindex(void)
{
  s48_value sch_table = S48_UNSPECIFIC;
  struct if_nameindex *index = if_nameindex();
  int index_size;
  S48_DECLARE_GC_PROTECT(1);

  S48_GC_PROTECT_1(sch_table);
  if (index == NULL)
    s48_os_error("s48_if_nameindex", errno, 0);

  index_size = 0;
  {
    struct if_nameindex *p = index;
    while (p->if_index != 0)
      {
	++p;
	++index_size;
      }
  }

  sch_table = s48_make_vector(2 * index_size, S48_UNSPECIFIC);
  
  {
    int i = 0;
    while (i < index_size)
      {
	S48_VECTOR_SET(sch_table, i * 2,
		       s48_enter_unsigned_integer(index[i].if_index));
	S48_VECTOR_SET(sch_table, (i * 2) + 1,
		       s48_enter_string_latin_1(index[i].if_name));
	++i;
      }
  }
  if_freenameindex(index);
  S48_GC_UNPROTECT();
  return sch_table;
}

#endif

/* Nodename translation */

int
s48_extract_socket_type(s48_value sch_socktype)
{
  long socktype_val = s48_extract_fixnum(sch_socktype);
  if (socktype_val > 100)
    return socktype_val - 100;
  else
    switch (socktype_val)
      {
      case 0:
	return SOCK_STREAM;
      case 1:
	return SOCK_DGRAM;
      }
}

s48_value
s48_enter_socket_type(int socktype)
{
  switch (socktype)
    {
    case SOCK_STREAM:
      return s48_enter_fixnum(0);
    case SOCK_DGRAM:
      return s48_enter_fixnum(1);
    default: 
      return s48_enter_fixnum((int) socktype + 100); 
    }
}

static int
extract_ai_flags(s48_value sch_flags)
{
  long flags = s48_extract_fixnum(sch_flags);
  return (((flags & 0x01) ? AI_PASSIVE : 0)
	  | ((flags & 0x02) ? AI_CANONNAME : 0)
	  | ((flags & 0x04) ? AI_NUMERICHOST : 0));
}

static s48_value
enter_ai_flags(int flags)
{
  return
    s48_enter_fixnum(((flags & AI_PASSIVE) ? 0x01 : 0)
		     | ((flags & AI_CANONNAME) ? 0x02 : 0)
		     | ((flags & AI_NUMERICHOST) ? 0x04 : 0));
}

static int
extract_ip_protocol(s48_value sch_protocol)
{
  long ip = s48_extract_fixnum(sch_protocol);
  if (ip > 100)
    return ip - 100;
  else
    switch (ip)
      {
      case 0:
	return IPPROTO_IP;
      case 1:
	return IPPROTO_IPV6;
      case 2:
	return IPPROTO_ICMP;
      case 3:
	return IPPROTO_RAW;
      case 4:
	return IPPROTO_TCP;
      case 5:
	return IPPROTO_UDP;
    }
}

static s48_value
enter_ip_protocol(int protocol)
{
  switch (protocol)
    {
    case IPPROTO_IP:
      return s48_enter_fixnum(0);
    case IPPROTO_IPV6:
      return s48_enter_fixnum(1);
    case IPPROTO_ICMP:
      return s48_enter_fixnum(2);
    case IPPROTO_RAW:
      return s48_enter_fixnum(3);
    case IPPROTO_TCP:
      return s48_enter_fixnum(4);
    case IPPROTO_UDP:
      return s48_enter_fixnum(5);
    default:
      return s48_enter_fixnum(protocol + 100);
    }
}

struct getaddrinfo_handshake
{
  long event_uid;
  char* nodename;
  char* servname;
  struct addrinfo hints;
  int status;
  struct addrinfo* result;
};

static s48_value
enter_addrinfo(const struct addrinfo *ai)
{
  S48_DECLARE_GC_PROTECT(1);
  s48_value sch_ai = s48_make_vector(5, S48_UNSPECIFIC);
  S48_GC_PROTECT_1(sch_ai);
	
  S48_VECTOR_SET(sch_ai, 0, s48_enter_af(ai->ai_family));
  S48_VECTOR_SET(sch_ai, 1, s48_enter_socket_type(ai->ai_socktype));
  S48_VECTOR_SET(sch_ai, 2, enter_ip_protocol(ai->ai_protocol));
  S48_VECTOR_SET(sch_ai, 3,
		 (ai->ai_canonname != NULL)
		 ? s48_enter_string_latin_1(ai->ai_canonname) : S48_FALSE);
  S48_VECTOR_SET(sch_ai, 4, s48_enter_sockaddr(ai->ai_addr, ai->ai_addrlen));
  
  S48_GC_UNPROTECT();
  return sch_ai;
}

static void
free_getaddrinfo_handshake(struct getaddrinfo_handshake *handshake)
{
  free(handshake->nodename);
  free(handshake->servname);
  free(handshake);
}

static s48_value
get_addrinfo_result(struct getaddrinfo_handshake *handshake)
{
  struct addrinfo *p;
  int i, addrinfo_count;

  s48_unregister_external_event_uid(handshake->event_uid);

  if (handshake->status != 0)
    {
      int status = handshake->status;
      free_getaddrinfo_handshake(handshake);
      if (status == EAI_NONAME)
	return S48_FALSE;
      else
	s48_error("s48_getaddrinfo_result", gai_strerror(status), 0);
    }

  addrinfo_count = 0;
  p = handshake->result;
  while (p != NULL)
    {
      ++addrinfo_count;
      p = p->ai_next;
    }

  {
    S48_DECLARE_GC_PROTECT(1);
    s48_value sch_result = s48_make_vector(addrinfo_count, S48_UNSPECIFIC);

    S48_GC_PROTECT_1(sch_result);
    
    i = 0;
    p = handshake->result;
    while (i < addrinfo_count)
      {
	S48_VECTOR_SET(sch_result, i, enter_addrinfo(p));
	p = p->ai_next;
	++i;
      }
    
    freeaddrinfo(handshake->result);
    free_getaddrinfo_handshake(handshake);
    S48_GC_UNPROTECT();
    return sch_result;
  }
}

static s48_value
s48_getaddrinfo_result(s48_value sch_handshake)
{
  return get_addrinfo_result(s48_extract_pointer(sch_handshake));
}

#ifdef HAVE_THREADS
static
DECLARE_THREAD_PROC(getaddrinfo_thread, void_handshake)
{
  struct getaddrinfo_handshake *handshake
    = (struct getaddrinfo_handshake *) void_handshake;
  
  handshake->status
    = getaddrinfo(handshake->nodename, handshake->servname,
		  &(handshake->hints), &(handshake->result));
  s48_note_external_event(handshake->event_uid);
  EXIT_THREAD_PROC();
}
#endif

static s48_value
s48_getaddrinfo(s48_value sch_nodename, s48_value sch_servname,
		s48_value sch_hint_flags, s48_value sch_hint_family,
		s48_value sch_hint_socktype, s48_value sch_hint_protocol)
{
  struct getaddrinfo_handshake *handshake;
#ifdef HAVE_THREADS
  thread_t t;
#endif

  handshake = malloc(sizeof(struct getaddrinfo_handshake));
  if (handshake == NULL)
    s48_out_of_memory_error();

  if (S48_EQ_P(sch_nodename, S48_FALSE))
      handshake->nodename = NULL;
  else
    {
      handshake->nodename
	= maybe_extract_fresh_latin1_string(sch_nodename, NI_MAXHOST);
      if (handshake->nodename == NULL)
	return S48_FALSE; /* debatable */
    }

  if (S48_EQ_P(sch_servname, S48_FALSE))
    handshake->servname = NULL;
  else
    {
      handshake->servname
	= maybe_extract_fresh_latin1_string(sch_servname, NI_MAXSERV);
      if (handshake->servname == NULL)
	return S48_FALSE; /* debatable */
    }
  
  handshake->hints.ai_flags = extract_ai_flags(sch_hint_flags);
  handshake->hints.ai_family = s48_extract_af(sch_hint_family);
  handshake->hints.ai_socktype
    = (S48_EQ_P(sch_hint_socktype, S48_FALSE)
       ? 0
       : s48_extract_socket_type(sch_hint_socktype));
  handshake->hints.ai_protocol
    = (S48_EQ_P(sch_hint_protocol, S48_FALSE)
       ? 0
       : extract_ip_protocol(sch_hint_protocol));
  handshake->hints.ai_addrlen = 0;
  handshake->hints.ai_canonname = NULL;
  handshake->hints.ai_addr = NULL;
  handshake->hints.ai_next = NULL;

  handshake->event_uid = s48_external_event_uid();

#ifdef HAVE_THREADS
  if (START_THREAD(t, getaddrinfo_thread, handshake))
#endif
    {
      handshake->status
	= getaddrinfo(handshake->nodename, handshake->servname,
		      &(handshake->hints), &(handshake->result));
      return get_addrinfo_result(handshake);
    }
#ifdef HAVE_THREADS
  else
    {
      s48_value sch_event_uid = S48_UNSPECIFIC;
      s48_value sch_handshake = S48_UNSPECIFIC;
      s48_value sch_result;
      S48_DECLARE_GC_PROTECT(2);

      DETACH_THREAD(t);

      S48_GC_PROTECT_2(sch_event_uid, sch_handshake);
      sch_event_uid = s48_enter_integer(handshake->event_uid);
      sch_handshake = s48_enter_pointer(handshake);

      sch_result = s48_cons(sch_event_uid, sch_handshake);
      S48_GC_UNPROTECT();
      return sch_result;
    }
#endif
}

static int
extract_ni_flags(s48_value sch_flags)
{
  long flags = s48_extract_fixnum(sch_flags);
  return (((flags & 0x01) ? NI_NOFQDN : 0)
	  | ((flags & 0x02) ? NI_NUMERICHOST : 0)
	  | ((flags & 0x04) ? NI_NAMEREQD : 0)
	  | ((flags & 0x08) ? NI_NUMERICSERV : 0)
	  | ((flags & 0x10) ? NI_DGRAM: 0));
}

static s48_value
enter_ni_flags(int flags)
{
  return
    s48_enter_fixnum(((flags & NI_NOFQDN) ? 0x01 : 0)
		     | ((flags & NI_NUMERICHOST) ? 0x02 : 0)
		     | ((flags & NI_NAMEREQD) ? 0x04 : 0)
		     | ((flags & NI_NUMERICSERV) ? 0x08 : 0)
		     | ((flags & NI_DGRAM) ? 0x10 : 0));
}

struct getnameinfo_handshake
{
  long event_uid;
  struct sockaddr_storage addr;
  socklen_t len;
  int flags;
  int status;
  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];
};

static s48_value
getnameinfo_result(struct getnameinfo_handshake* handshake)
{
  s48_value sch_host = S48_UNSPECIFIC, sch_serv = S48_UNSPECIFIC;
  s48_value sch_result = S48_UNSPECIFIC;
  S48_DECLARE_GC_PROTECT(3);

  s48_unregister_external_event_uid(handshake->event_uid);

  if (handshake->status != 0)
    {
      int status = handshake->status;
      free(handshake);
      s48_error("s48_getnameinfo_result", gai_strerror(status), 0);
    }

  S48_GC_PROTECT_3(sch_host, sch_serv, sch_result);
  /* we use a vector to be able to distinguish from a pair */
  sch_result = s48_make_vector(2, S48_UNSPECIFIC);
  S48_VECTOR_SET(sch_result, 0, s48_enter_string_latin_1(handshake->host));
  S48_VECTOR_SET(sch_result, 1, s48_enter_string_latin_1(handshake->serv));
  S48_GC_UNPROTECT();

  free(handshake);

  return sch_result;
}

static s48_value
s48_getnameinfo_result(s48_value sch_handshake)
{
  return getnameinfo_result(s48_extract_pointer(sch_handshake));
}

#ifdef HAVE_THREADS
static
DECLARE_THREAD_PROC(getnameinfo_thread, void_handshake)
{
  struct getnameinfo_handshake *handshake = void_handshake;
  
  handshake->status
    = getnameinfo((struct sockaddr*)&(handshake->addr), handshake->len,
		  handshake->host, NI_MAXHOST,
		  handshake->serv, NI_MAXSERV,
		  handshake->flags);
  s48_note_external_event(handshake->event_uid);
  EXIT_THREAD_PROC();
}
#endif


static s48_value
s48_getnameinfo(s48_value sch_saddr, s48_value sch_flags)
{
  const struct sockaddr *sa
    = S48_EXTRACT_VALUE_POINTER(sch_saddr, const struct sockaddr);
  socklen_t salen = S48_VALUE_SIZE(sch_saddr);
#ifdef HAVE_THREADS
  thread_t t;
#endif

  struct getnameinfo_handshake *handshake
    = malloc(sizeof(struct getnameinfo_handshake));

  if (handshake == NULL)
    s48_out_of_memory_error();

  memcpy(&(handshake->addr), sa, salen);
  handshake->len = salen;

  handshake->flags = extract_ni_flags(sch_flags);

  handshake->event_uid = s48_external_event_uid();

#ifdef HAVE_THREADS
  if (START_THREAD(t, getnameinfo_thread, handshake))
#endif
    {
      handshake->status
	= getnameinfo((struct sockaddr*) &(handshake->addr), handshake->len,
		      handshake->host, NI_MAXHOST,
		      handshake->serv, NI_MAXSERV,
		      handshake->flags);
      return getnameinfo_result(handshake);
    }
#ifdef HAVE_THREADS
  else
    {
      s48_value sch_event_uid = S48_UNSPECIFIC;
      s48_value sch_handshake = S48_UNSPECIFIC;
      s48_value sch_result;
      S48_DECLARE_GC_PROTECT(2);

      DETACH_THREAD(t);

      S48_GC_PROTECT_2(sch_event_uid, sch_handshake);
      sch_event_uid = s48_enter_integer(handshake->event_uid);
      sch_handshake = s48_enter_pointer(handshake);

      sch_result = s48_cons(sch_event_uid, sch_handshake);
      S48_GC_UNPROTECT();
      return sch_result;
    }
#endif
}

/* Adress conversion */

s48_value
s48_inet_pton(s48_value sch_af, s48_value sch_src)
{
  sa_family_t af = s48_extract_af(sch_af);
  int status;

  switch (af)
    {
    case AF_INET:
      {
	char src[INET_ADDRSTRLEN+1]; /* be safe */
	struct in_addr addr;
	s48_copy_string_to_latin_1(sch_src, src);
	src[s48_string_length(sch_src)] = '\0';
#ifdef _WIN32
	{
	  INT size = sizeof(struct in_addr);
	  if (WSAStringToAddress(src, AF_INET, 0, 
				 (LPSOCKADDR)&addr, &size)
	      == 0)
	  return enter_in_addr(&addr);
	}
#else
	status = inet_pton(AF_INET, src, &addr);
	if (status == 1)
	  return enter_in_addr(&addr);
#endif
	break;
      }
    case AF_INET6:
      {
	char src[INET6_ADDRSTRLEN+1]; /* be safe */
	struct in6_addr addr;
	s48_copy_string_to_latin_1(sch_src, src);
	src[s48_string_length(sch_src)] = '\0';
#ifdef _WIN32
	{
	  INT size = sizeof(struct in6_addr);
	  if (WSAStringToAddress(src, AF_INET6, 0,
				 (LPSOCKADDR)&addr, &size)
	      == 0)
	  return s48_enter_in6_addr(&addr);
	}
#else
	status = inet_pton(AF_INET6, src, &addr);
	if (status == 1)
	  return s48_enter_in6_addr(&addr);
#endif
      }
    default:
      s48_assertion_violation("s48_inet_pton", "invalid adddress family", 1, sch_af);
    }

  return S48_FALSE;
}

static s48_value
s48_inet_ntop(s48_value sch_af, s48_value sch_src)
{
  sa_family_t af = s48_extract_af(sch_af);

  switch (af)
    {
    case AF_INET:
      {
	char dest[INET_ADDRSTRLEN+1]; /* be safe */
	struct in_addr addr;
	extract_in_addr(sch_src, &addr);
#ifdef _WIN32
	{
	  DWORD destlen;
	  if (WSAAddressToString((struct sockaddr *)&addr, sizeof(struct in_addr),
				 NULL, dest, &destlen) != 0)
	    s48_os_error("s48_inet_ntop", WSAGetLastError(), 2, sch_af, sch_src);
	}
	    
#else
	if (inet_ntop(AF_INET, &addr, dest, sizeof(dest)) == NULL)
	  s48_os_error("s48_inet_ntop", errno, 2, sch_af, sch_src);
#endif
	return s48_enter_string_latin_1(dest);
      }
    case AF_INET6:
      {
	char dest[INET6_ADDRSTRLEN+1]; /* be safe */
	struct in6_addr addr;
	s48_extract_in6_addr(sch_src, &addr);
#ifdef _WIN32
	{
	  DWORD destlen;
	  if (WSAAddressToString((struct sockaddr *)&addr, sizeof(struct in6_addr),
				 NULL, dest, &destlen) != 0)
	    s48_os_error("s48_inet_ntop", WSAGetLastError(), 2, sch_af, sch_src);
	}
#else
	if (inet_ntop(AF_INET6, &addr, dest, sizeof(dest)) == NULL)
	  s48_os_error("s48_inet_ntop", errno, 2, sch_af, sch_src);
#endif
	return s48_enter_string_latin_1(dest);
      }
    default:
      s48_assertion_violation("s48_inet_ntop", "invalid adddress family", 1, sch_af);
    }
}

/* Address testing */

#define DEFINE_ADDRESS_TESTER(name) \
static s48_value \
s48_##name(s48_value sch_addr) \
{ \
  struct in6_addr addr; \
  s48_extract_in6_addr(sch_addr, &addr); \
  return S48_ENTER_BOOLEAN(name(&addr)); \
}

DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_UNSPECIFIED)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_LOOPBACK)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_MULTICAST)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_LINKLOCAL)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_SITELOCAL)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_V4MAPPED)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_V4COMPAT)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_MC_NODELOCAL)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_MC_LINKLOCAL)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_MC_SITELOCAL)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_MC_ORGLOCAL)
DEFINE_ADDRESS_TESTER(IN6_IS_ADDR_MC_GLOBAL)

void
s48_init_net_addresses(void)
{
  S48_EXPORT_FUNCTION(s48_make_sockaddr_in_raw);
  S48_EXPORT_FUNCTION(s48_get_inaddr_any);
  S48_EXPORT_FUNCTION(s48_get_inaddr_broadcast);

  S48_EXPORT_FUNCTION(s48_make_sockaddr_in6_raw);
  S48_EXPORT_FUNCTION(s48_get_in6addr_any);
  S48_EXPORT_FUNCTION(s48_get_in6addr_loopback);

#ifndef _WIN32
  S48_EXPORT_FUNCTION(s48_make_sockaddr_un_raw);

  S48_EXPORT_FUNCTION(s48_if_nametoindex);
  S48_EXPORT_FUNCTION(s48_if_indextoname);
  S48_EXPORT_FUNCTION(s48_if_nameindex);
#endif

  S48_EXPORT_FUNCTION(s48_getaddrinfo);
  S48_EXPORT_FUNCTION(s48_getaddrinfo_result);
  S48_EXPORT_FUNCTION(s48_getnameinfo);
  S48_EXPORT_FUNCTION(s48_getnameinfo_result);
  
  S48_EXPORT_FUNCTION(s48_inet_pton);
  S48_EXPORT_FUNCTION(s48_inet_ntop);

  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_UNSPECIFIED);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_LOOPBACK);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_MULTICAST);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_LINKLOCAL);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_SITELOCAL);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_V4MAPPED);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_V4COMPAT);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_MC_NODELOCAL);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_MC_LINKLOCAL);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_MC_SITELOCAL);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_MC_ORGLOCAL);
  S48_EXPORT_FUNCTION(s48_IN6_IS_ADDR_MC_GLOBAL);
}
