
#define STDIN_FD() 0
#define STDOUT_FD() 1
#define STDERR_FD() 2

extern int ps_open_fd(char *in_filename, bool is_input, long *status);

extern int ps_close_fd(long fd_as_long);

extern bool ps_check_fd(long fd_as_long, bool is_read, long *status);

extern long ps_read_fd(long fd_as_long, char *buf_as_long, long max, bool waitp,
		       bool *eofp, bool *pending, long *status);

extern long ps_write_fd(long fd_as_long, char *buf_as_long, long max,
			bool *pending, long *status);

extern long ps_abort_fd_op(long fd_as_long);
