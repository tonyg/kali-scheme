/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>

#include <scheme48.h>

/* OS-unspecific */

int s48_extract_msg_flags(s48_value sch_flags);

/* OS-specific */

#ifdef WIN32
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif

socket_t s48_extract_socket_fd(s48_value sch_channel);

#endif
