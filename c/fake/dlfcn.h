/*
 * This include file is for systems which do not have dynamic loading.
 */
#if ! defined(HAVE_DLOPEN)

extern void	*dlopen(char *filename, int flags);
extern char	*dlerror(void);
extern void	*dlsym(void *lib, char *name);
extern int	dlclose(void *lib);

#endif
