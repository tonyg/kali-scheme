/*
 * This is a fake version of the dynamic loading library for HP-UX.
 * We implement libdl using their shl_... routines.
 */
#include "sysdep.h"

#include <dl.h>
#include <errno.h>

static char	*lasterror;

char	*
dlerror(void)
{
	char	*res;

	res = lasterror;
	lasterror = NULL;
	return (res);
}


void	*
dlopen(char *name, int flags)
{
	return (shl_load(name, BIND_IMMEDIATE, 0L));
}

int
dlclose(void *lib)
{
	return (shl_unload(lib));
}


void	*
dlsym(void *lib, char *name)
{
	long	res;
	
	if (shl_findsym((shl_t *)lib, name, TYPE_UNDEFINED, &res) == 0)
	  return ((void *) res);
	else {
	  lasterror = "Symbol not found";
	  return (NULL); }
}
