/*
 * Lookup external names in the running scheme virtual machine and, on
 * machines which support it, do dynamic loading.
 */
#include <stdlib.h>
#include <unistd.h>
#include "sysdep.h"
#include "scheme48.h"


#if	defined(HAVE_DLOPEN)

#include <dlfcn.h>

#if	defined(HAVE_LIBGEN_H) && defined(HAVE_LIBGEN)
#include <libgen.h>
#endif

#if	defined(RTLD_NOW)
#define	DLOPEN_MODE	RTLD_NOW
#elif	defined(RTLD_LAZY)
#define	DLOPEN_MODE	(RTLD_LAZY)
#else
#define	DLOPEN_MODE	(1)
#endif

#endif	/* defined(HAVE_DLOPEN) */


#define	bool		char		/* boolean type */
#define	TRUE		(0 == 0)
#define	FALSE		(! TRUE)


#if	defined(HAVE_DLOPEN)

static bool	dynamic_load(char *name);
static char	*resolve(char *name);


/*
 * Linked list of dynamically loaded libraries.
 */
static struct	dlob {
	struct dlob	*next;
	char		*name;
	void		*handle;
}	*dlobs;


/*
 * Lookup an external name (either in a dynamically loaded library, or
 * in the running executable).
 * On success we return TRUE, having set *(long *)svlocp to the location.
 * On failure, we return FALSE.
 */
long
lookup_external_name(long svname, long svlocp)
{
	struct dlob	*dp;
	void		*res;
	static void	*self;

	for (dp = dlobs; dp != NULL; dp = dp->next) {
		res = dlsym(dp->handle, (char *)svname);
		if (dlerror() == NULL) {
			*(long *)svlocp = (long)res;
			return (TRUE);
		}
	}
	if (self == NULL) {
		self = dlopen((char *)NULL, DLOPEN_MODE);
		if (dlerror() != NULL)
			return (FALSE);
	}
	res = dlsym(self, (char *)svname);
	if (dlerror() == NULL) {
		*(long *)svlocp = (long)res;
		return (TRUE);
	}
	return (FALSE);
}


/*
 * External to load a library.
 * On success we return #T, on failure #F.
 * Note, if you load the same file a second time, afterwards you must
 * evaluate (lookup-all-externals) in package externals to update any
 * externals the pointed to the old version of the library.
 */
long
s48_dynamic_load(long argc, long argv[])
{
	long	arg;

	if (argc != 1)
		return (SCHFALSE);
	arg = argv[0];
	if (! STRINGP(arg))
		return (SCHFALSE);
	return (dynamic_load(&STRING_REF(arg, 0)) ? SCHTRUE : SCHFALSE);
}


static bool
dynamic_load(char *name)
{
	struct dlob	**dpp,
			*dp;
	void		*handle;

	name = resolve(name);
	for (dpp = &dlobs;; dpp = &dp->next) {
		dp = *dpp;
		if (dp == NULL) {
			handle = dlopen(name, DLOPEN_MODE);
			if (dlerror() != NULL)
				return (FALSE);
			dp = (struct dlob *)malloc(sizeof(*dp) + strlen(name) + 1);
			if (dp == NULL) {
				dlclose(handle);
				return (FALSE);
			}
			dp->next = dlobs;
			dlobs = dp;
			dp->name = (char *)(dp + 1);
			strcpy(dp->name, name);
			dp->handle = handle;
			return (TRUE);
		} else if (strcmp(name, dp->name) == 0) {
			dlclose(dp->handle);
			dp->handle = dlopen(name, DLOPEN_MODE);
			if (dlerror() != NULL) {
				*dpp = dp->next;
				free((void *)dp);
				return (FALSE);
			}
			return (TRUE);
		}
	}
}


static char	*
resolve(char *name)
{
#if	defined(HAVE_LIBGEN_H) && defined(HAVE_LIBGEN)
	char	*res,
		*path;

	path = getenv("S48_EXTERN_PATH");
	if (path != NULL) {
		res = pathfind(path, name, "r");
		if (res != NULL)
			return (res);
	}
	path = getenv("LD_LIBRARY_PATH");
	if (path != NULL) {
		res = pathfind(path, name, "r");
		if (res != NULL)
			return (res);
	}
#endif
	return (name);
}


#elif	defined(HAVE_NLIST)

#include <nlist.h>

#ifdef	USCORE
#include <string.h>
#endif

#if	! defined(NLIST_HAS_N_NAME)
#define	n_name	n_un.n_name
#endif

long
lookup_external_name(long svname, long svlocation)
{
	struct nlist	names[2];
	int		status;
	char		*name;
	long		*location;
	extern char	*object_file;
#ifdef	USCORE
	int		len;
	char		*tmp,
			buff[40];
#endif

	name = (char *)svname;
	location = (long *)svlocation;
	if (object_file == NULL)
		return (FALSE);
#ifdef	USCORE
	len = 1 + strlen(name) + 1;
	if (len <= sizeof(buff))
		tmp = buff;
	else {
		tmp = (char *)malloc(len);
		if (tmp == NULL)
			return (FALSE);
	}
	tmp[0] = '_';
	strcpy(tmp+1, name);
	name = tmp;
#endif
	names[0].n_name = name;
	names[0].n_value = 0;			/* for Linux */
	names[0].n_type = 0;			/* for Linux */
	names[1].n_name = NULL;
	status = nlist(object_file, names);
#ifdef	USCORE
	if (tmp != buff)
		free(tmp);
#endif
	if ((status != 0)
	||  (names[0].n_value == 0 && names[0].n_type == 0))
		return (FALSE);
	*location = names[0].n_value;
	return (TRUE);
}

#else	/* neither HAVE_DLOPEN nor HAVE_NLIST defined */

long
lookup_external_name(long svname, long svlocation)
{
	return (FALSE);
}

#endif
