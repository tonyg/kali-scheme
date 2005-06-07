/*
 * This is a fake version of the dynamic loading library for machines
 * which don't have it, and don't even have an nlist.
 * We fake it so that everything fails.
 */
#include "sysdep.h"


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
	lasterror = "Dynamic loading not supported on this machine";
	return (NULL);
}


int
dlclose(void *lib)
{
	return (0);
}


void	*
dlsym(void *lib, char *name)
{
	lasterror = "Dynamic loading not supported on this machine";
	return (NULL);
}
