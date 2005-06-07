/* globals.c:
 *
 ****************************************************************
 * Copyright (C) 2005 Matthew Dempsky
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include <stdlib.h>
#include <stdio.h>
#include "scheme48/libscheme48/globals.h"



static unsigned int num_globals = 0;
static s48_word_t ** globals = 0;



s48_global_t
s48_allocate_global ()
{
  s48_global_t answer;
  s48_word_t * new_global;

  new_global = (s48_word_t *)malloc (sizeof (s48_word_t));
  S48_GC_PROTECT_GLOBAL (*new_global);
  *new_global = S48_UNSPECIFIC;

  answer.v = num_globals++;
  globals = realloc (globals, num_globals * sizeof (*globals));
  globals[answer.v] = new_global;

  return answer;
}



void
s48_global_ref (s48_word_t * result,
                s48_arena_t arena,
                s48_global_t global)
{
  if (global.v >= num_globals)
    {
#undef FIXME
      /* I'm reluctant to make this into a genuine Scheme exception
         because at run-time there's nothing you can do to fix
         it. -mrd */
      fprintf (stderr, "Bad global value in s48_global_ref().\n");
      exit (1);
    }

  *result = *globals[global.v];
}

void
s48_global_set (s48_arena_t arena,
                s48_global_t global,
                s48_word_t * value)
{
  if (global.v >= num_globals)
    {
#undef FIXME
      /* I'm reluctant to make this into a genuine Scheme exception
         because at run-time there's nothing you can do to fix
         it. -mrd */
      fprintf (stderr, "Bad global value in s48_global_set().\n");
      exit (1);
    }

  *globals[global.v] = *value;
}



void
s48_register_global_init (void (*init) (s48_arena_t))
{
  (*init) (0);
}



/* arch-tag: Matthew Dempsky Sat Apr 23 17:01:55 2005 (globals.c)
 */
