
#define STDIN_FD() 0
#define STDOUT_FD() 1
#define STDERR_FD() 2

extern int ps_open_fd(unsigned char *in_filename, bool is_input, long *status);

extern int ps_close_fd(long fd_as_long);

/* `buf' in following two declarations is really a pointer to characters,
    but it's easier to put the cast in the C code than mess with the
    Pre-Scheme compiler */

extern long ps_read_fd(long fd_as_long, long buf_as_long, long max, bool waitp,
		       bool *eofp, bool *pending, long *status);

extern long ps_write_fd(long fd_as_long, long buf_as_long, long max,
			bool *pending, long *status);

extern long ps_abort_fd_op(long fd_as_long);
