/*
 * Procedures exported from socket.c.
 * Note, sysdep.h must be included before this file.
 */
#if defined(HAVE_SOCKET)

extern int	internet_stream_socket(void),
		socket_bind(int sock, int port),
		socket_accept(int sock),
		socket_connect(int sock, char *mach, int port);

#endif
