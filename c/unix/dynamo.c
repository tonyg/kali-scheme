/*
 * Lookup external names in the running scheme virtual machine and, on
 * machines which support it, do dynamic loading.
 */

#include <stdlib.h>
#include <unistd.h>
#include "sysdep.h"
#include "scheme48.h"
#include <dlfcn.h>


#if	defined(RTLD_NOW)
#define	DLOPEN_MODE	RTLD_NOW
#elif	defined(RTLD_LAZY)
#define	DLOPEN_MODE	(RTLD_LAZY)
#else
#define	DLOPEN_MODE	(1)
#endif

#define	bool		char		/* boolean type */
#define	TRUE		(0 == 0)
#define	FALSE		(! TRUE)


/*
 * Linked list of dynamically loaded libraries.
 */
static struct	dlob {
	struct dlob	*next;
	char		*name;
	void		*handle;
}	*dlobs;


static s48_value	s48_external_lookup(s48_value svname, s48_value svlocp),
			s48_old_external_call(s48_value svproc, s48_value svargv),
			s48_dynamic_load(s48_value filename);
static long		lookup_external_name(char *name, long *locp);
static bool		dynamic_load(char *name);


/*
 * Install all exported functions in Scheme48.
 */
void
s48_init_external_lookup(void)
{
	S48_EXPORT_FUNCTION(s48_external_lookup);
	S48_EXPORT_FUNCTION(s48_old_external_call);
	S48_EXPORT_FUNCTION(s48_dynamic_load);
}


/*
 * Glue between Scheme48 types and C types for external name lookup.
 * Look up svname (either in a dynamically loaded library, or in the
 * running executable).
 * On success we return TRUE, having set *(long *)svlocp to the location.
 * On failure, we return FALSE.
 */
static s48_value
s48_external_lookup(s48_value svname, s48_value svlocp)
{
	char	*name;
	long	*locp,
		res;

	name = s48_extract_string(svname);
	locp = S48_EXTRACT_VALUE_POINTER(svlocp, long);
	res = lookup_external_name(name, locp);
	return (S48_ENTER_BOOLEAN(res));
}


/*
 * Glue between Scheme48 types and C types for external call.
 * svproc is a byte vector containing the procedure and svargs is a
 * vector of arguments.
 */
static s48_value
s48_old_external_call(s48_value svproc, s48_value svargv)
{
	s48_value	(*func)();
	long		*argv,
			argc;

	func = (s48_value (*)())*S48_EXTRACT_VALUE_POINTER(svproc, long);
	argc = S48_VECTOR_LENGTH(svargv);
	argv = S48_ADDRESS_AFTER_HEADER(svargv, long);
	return (func(argc, argv));
}


/*
 * Lookup an external name (either in a dynamically loaded library, or
 * in the running executable).
 * On success we return TRUE, having set *(long *)locp to the location.
 * On failure, we return FALSE.
 */
static long
lookup_external_name(char *name, long *locp)
{
	struct dlob	*dp;
	void		*res;
	static void	*self;

	for (dp = dlobs; dp != NULL; dp = dp->next) {
		res = dlsym(dp->handle, name);
		if (dlerror() == NULL) {
			*locp = (long)res;
			return (TRUE);
		}
	}
	if (self == NULL) {
		self = dlopen((char *)NULL, DLOPEN_MODE);
		if (dlerror() != NULL)
			return (FALSE);
	}
	res = dlsym(self, name);
	if (dlerror() == NULL) {
		*locp = (long)res;
		return (TRUE);
	}
	return (FALSE);
}


/*
 * External to load a library.
 * Raises an exception if the file cannot be loaded, or loaded properly.
 * Note, if you load the same file a second time, afterwards you must
 * evaluate (lookup-all-externals) in package externals to update any
 * externals the pointed to the old version of the library.
 */

s48_value
s48_dynamic_load(s48_value filename)
{
	S48_CHECK_STRING(filename);

	if (! dynamic_load(S48_UNSAFE_EXTRACT_STRING(filename)))
	  /* the cast below is to remove the const part of the type */
	  s48_raise_string_os_error((char *)dlerror());

	return S48_UNSPECIFIC;
}


static bool
dynamic_load(char *name)
{
	struct dlob	**dpp,
			*dp;
	void		*handle;

	for (dpp = &dlobs;; dpp = &dp->next) {
		dp = *dpp;
		if (dp == NULL) {
			handle = dlopen(name, DLOPEN_MODE);
			if (handle == NULL)
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
			if (dp->handle == NULL) {
				*dpp = dp->next;
				free((void *)dp);
				return (FALSE);
			}
			return (TRUE);
		}
	}
}
