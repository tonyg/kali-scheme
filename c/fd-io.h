
#define STDIN_FD() 0
#define STDOUT_FD() 1
#define STDERR_FD() 2

extern int ps_open_fd(char *in_filename, psbool is_input, long *status);

extern int ps_close_fd(long fd_as_long);

extern psbool ps_check_fd(long fd_as_long, psbool is_read, long *status);

extern long ps_read_fd(long fd_as_long, char *buf_as_long, long max, psbool waitp,
		       psbool *eofp, psbool *pending, long *status);

extern long ps_write_fd(long fd_as_long, char *buf_as_long, long max,
			psbool *pending, long *status);

extern long ps_abort_fd_op(long fd_as_long);

extern long ps_io_buffer_size(void);

extern char *ps_console_encoding(long fd_as_long);

