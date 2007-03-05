/*
 * If we have a sys/langinfo.h, then include it.
 */
#if	defined(HAVE_LANGINFO_H)

#include <langinfo.h>

#else

typedef int nl_item;
#define CODESET 0
extern char *nl_langinfo(nl_item item);

#endif
