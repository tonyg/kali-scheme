/* globals.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Matthew Dempsky
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBSCHEME48__GLOBALS_H
#define INCLUDE__LIBSCHEME48__GLOBALS_H


#include "scheme48/libscheme48/scheme48.h"



typedef struct
{
  unsigned int v;
} s48_global_t;



#define S48_UNINIT_GLOBAL ((s48_global_t){ -1 })

#ifndef S48_MAGIC_SNARFER
#define S48_DECL_GLOBAL(g) s48_global_t g = S48_UNINIT_GLOBAL
#else
#define S48_DECL_GLOBAL(g) %%% g = s48_allocate_global () @@@
#endif

#ifndef S48_MAGIC_SNARFER2
#define S48_DECL_GLOBAL_INIT(f)
#else
#define S48_DECL_GLOBAL_INIT(f) %%% s48_register_global_init (&(f)) @@@
#endif


/* automatically generated __STDC__ prototypes */
extern s48_global_t s48_allocate_global ();
extern void s48_global_ref (s48_word_t * result,
                            s48_arena_t arena,
                            s48_global_t global);
extern void s48_global_set (s48_arena_t arena,
                            s48_global_t global,
                            s48_word_t * value);
extern void s48_register_global_init (void (*init) (s48_arena_t));
#endif  /* INCLUDE__LIBSCHEME48__GLOBALS_H */


/* arch-tag: Matthew Dempsky Sat Apr 23 17:03:15 2005 (globals.h)
 */
