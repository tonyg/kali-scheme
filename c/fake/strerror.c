/*
 * If the system doesn't have a strerror procedure, we provide our own.
 * Note, this depends on sys_nerr and sys_errlist being provided.
 * If your system doesn't provide that either, you can replace this
 * procedure with one that always returns "Unknown error".
 */
#include "sysdep.h"


extern int	sys_nerr;
extern char	*sys_errlist[];


char	*
strerror(int errnum)
{
	if ((0 <= errnum)
	&&  (errnum < sys_nerr))
		return (sys_errlist[errnum]);
	else
		return ("Unknown error");
}
