/* utils.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Matthew Dempsky
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBSCHEME48__UTILS_H
#define INCLUDE__LIBSCHEME48__UTILS_H


#include <stdarg.h>


/* automatically generated __STDC__ prototypes */
extern s48_error_t s48_list (s48_word_t * result,
                             s48_arena_t arena,
                             ...);
extern s48_error_t s48_vlist (s48_word_t * result,
                              s48_arena_t arena,
                              va_list ap);
extern long s48_length(s48_value list);
extern long s48_new_length (s48_arena_t arena,
                            s48_word_t * lst);
#endif  /* INCLUDE__LIBSCHEME48__UTILS_H */


/* arch-tag: Matthew Dempsky Tue Apr 19 23:42:24 2005 (utils.h)
 */
