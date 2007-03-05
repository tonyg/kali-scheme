/*
 * If the system doesn't have an nl_langinfo procedure, we provide our
 * own dummy version.
 */

#include "sysdep.h"

#if	defined(HAVE_LANGINFO_H)

#include <stdio.h>
#include <stdlib.h>

char *
nl_langinfo(nl_item item)
{
  if (item == CODESET)
    return "ASCII";
  else
    {
      fprintf(stderr, "unknown nl_item argument to nl_langinfo: %d", item);
      exit(-1);
    }
}

#endif
