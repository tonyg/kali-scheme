/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdio.h>
#include <stdlib.h>
#include "scheme48/libscheme48/scheme48.h"
#include "scheme48/libscheme48/scheme48vm.h"

void
s48_protect_n (s48_arena_t arena,
               void * frame,
               int n_vars)
{
  int i;
  long * l_frame;
  long * v;

  if (n_vars > 10)
    {
      fprintf (stderr, "Too many variables to protect in s48_protect_n\n");
      exit (1);
    }

  l_frame = (long *)frame;
  v = l_frame + 12;
  
  for (i = 0; i < n_vars; ++i)
    {
      s48_make_unspecific ((s48_word_t *)&v[i], arena);
      l_frame[i + 2] = (long)&v[i];
    }

  s48_push_gc_rootsB ((char *)frame, n_vars);
}
