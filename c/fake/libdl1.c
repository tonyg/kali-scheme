/*
 * This is a fake version of the dynamic loading library for machines
 * which don't have it, but which have nlist.  We fake the stuff so that
 * looking up in a NULL open library returns symbols in the current executable
 * (whose name is pointed to by object_file).
 */
#include "sysdep.h"
#include <stdlib.h>
#include <nlist.h>

#ifdef	USCORE
#include <string.h>
#endif

#if defined(HAVE_DLOPEN)
#include <dlfcn.h>
#else
#include "../fake/dlfcn.h"
#endif

#if	! defined(NLIST_HAS_N_NAME)
#define	n_name	n_un.n_name
#endif

extern char* s48_object_file;

static char	self[] = "Scheme 48 executable",
		*lasterror;

extern char*    s48_object_file;

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
	if (name == NULL)
		return ((void *)self);
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
	struct nlist	names[2];
	int		status;
#ifdef	USCORE
	int		len;
	char		*tmp,
			buff[40];
#endif

	if (lib != self) {
		lasterror = "Bad library pointer passed to dlsym()";
		return (NULL);
	}
	if (s48_object_file == NULL) {
		lasterror = "I don't know the name of my executable";
		return (NULL);
	}
#ifdef	USCORE
	len = 1 + strlen(name) + 1;
	if (len <= sizeof(buff))
		tmp = buff;
	else {
		tmp = (char *)malloc(len);
		if (tmp == NULL) {
			lasterror = "Out of space";
			return (NULL);
		}
	}
	tmp[0] = '_';
	strcpy(tmp + 1, name);
	name = tmp;
#endif
	names[0].n_name = name;
	names[0].n_value = 0;		/* for Linux */
	names[0].n_type = 0;		/* for Linux */
	names[1].n_name = NULL;
	status = nlist(s48_object_file, names);
#ifdef	USCORE
	if (tmp != buff)
		free((void *)tmp);
#endif
	if ((status != 0)
	||  (names[0].n_value == 0 && names[0].n_type == 0)) {
		lasterror = "Symbol not found";
		return (NULL);
	}
	return (void *)(names[0].n_value);
}
