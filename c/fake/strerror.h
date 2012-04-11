/*
 * If we don't have strerror(), we fake it using sys_nerr and sys_errlist.
 */
#if	! defined(HAVE_STRERROR)

extern char	*strerror(int errnum);

#endif
