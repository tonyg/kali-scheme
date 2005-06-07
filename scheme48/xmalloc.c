#include "xalloc.h"

#include <stdio.h>
#include <stdlib.h>

void *
xmalloc (size_t n)
{
  void *mem = malloc(n);

  if (!mem)
    {
      fprintf(stderr, "out of memory\n");
      abort();
    }
}

/* arch-tag: 30a60002-7682-4930-9275-0197186991ea
   (do not change this comment) */
