/*
 * Socket support for Scheme 48.  Note, the division of labor is rather
 * silly.  Given that socket_connect() creates it's own socket, there is
 * no reason for socket_bind() to not do the same (and to eliminate
 * internet_stream_socket()).  The interface to extension() is a little
 * easier this way, because it never has to return two values.
 * This code was written by a loyal member of the state.  As such, I of
 * course ask nothing in return.  From each according to their ability,
 * to each according to their need.  I have what I need.
 * I don't need a copyright.
 */
#include "sysdep.h"

#if defined(HAVE_SOCKET)

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "socket.h"
#include "scheme48vm.h"


#define	LISTENQ		5


/*
 * Create an internet-domain stream (reliable, sequenced) socket.
 * We return the socket (a small non-negative integer) on success
 * or a negative integer on failure.
 * Note, the socket has been made non-blocking.
 */
int
internet_stream_socket(void)
{
	int	res;

	res = socket(AF_INET, SOCK_STREAM, 0);
	if (res < 0)
		return (res);
	fcntl(res, F_SETFL, O_NONBLOCK);
	return (res);
}


/*
 * Given an internet-domain stream socket and a port number, bind
 * the socket to the port and prepare to receive connections.  If
 * the port number is 0 then we bind the socket to any available
 * port.  If all goes well, we return the port number we are bound
 * to.  It not, we return -1.
 */
int
socket_bind(int sock, int port)
{
	struct sockaddr_in	addr;
	int			len;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	if ((bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	||  (listen(sock, LISTENQ) < 0))
		return (-1);
	if (port == 0) {
		len = sizeof(addr);
		if ((getsockname(sock, (struct sockaddr *)&addr, &len) < 0)
		||  (addr.sin_family != AF_INET))
			return (-1);
		port = htons(addr.sin_port);
	}
	return (port);
}


/*
 * Given an internet-domain stream socket which has been bound (via
 * socket_bind), accept a connection and return the resulting socket
 * (a small, non-negative integer).
 * If this fails because the client hasn't connected yet, then we add
 * the socket to the pending queue and return -2.  If it fails for some
 * other reason, then we return -1.
 * Note, if all goes well, the socket returned has been made non-blocking.
 */
int
socket_accept(int sock)
{
	int			len,
				res;
	struct sockaddr_in	addr;

	len = sizeof(addr);
	res = accept(sock, (struct sockaddr *)&addr, &len);
	if (res >= 0) {
		fcntl(res, F_SETFL, O_NONBLOCK);
		return (res);
	}
	if ((errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN)
	&&  (add_pending_fd(sock, TRUE)))
		return (-2);
	return (-1);
}


/*
 * Given an internet-domain stream socket, a machine name and a port number,
 * connect the socket to that machine/port.  If the machine name is the empty
 * string, use the local machine.
 * If this succeeds return a small non-negative integer.
 * If it fails because the connect would block, add the socket to the pending
 * queue (for output) and return -2.  If it fails for some other reason,
 * return -1.
 */
int
socket_connect(int sock, char *mach, int port)
{
	char			mbuff[MAXHOSTNAMELEN];
	struct hostent		*hp;
	struct sockaddr_in	addr;

	if (*mach == '\0') {
		if (gethostname(mbuff, sizeof(mbuff)) < 0)
			return (-1);
		mach = mbuff;
	}
	hp = gethostbyname(mach);
	if (hp == NULL)
		return (-1);
	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = hp->h_addrtype;
	if (hp->h_length > sizeof(addr.sin_addr))
		return (-1);
	memcpy((void *)&addr.sin_addr, (void *)hp->h_addr, hp->h_length);
	addr.sin_port = htons(port);
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) >= 0)
		return (sock);
	if (errno == EISCONN)	/* somewhat questionable */
		return (sock);
	if ((errno == EWOULDBLOCK
		|| errno == EINTR
		|| errno == EINPROGRESS
		|| errno == EALREADY
		|| errno == EAGAIN)
	&&  (add_pending_fd(sock, FALSE)))
		return (-2);
	return (-1);
}

#endif	/* HAVE_SOCKET */
