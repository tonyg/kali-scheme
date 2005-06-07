/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef INCLUDE__LIBSCHEME48__RECORDS_H
#define INCLUDE__LIBSCHEME48__RECORDS_H


#include "scheme48/libscheme48/scheme48.h"


/* automatically generated __STDC__ prototypes */
extern int s48_is_record (s48_arena_t arena,
                          s48_word_t * value);
extern s48_error_t s48_new_shared_binding_ref (s48_word_t * result,
                                               s48_arena_t arena,
                                               s48_word_t * type_shared_binding);
extern s48_error_t s48_new_record_type_check (s48_arena_t arena,
                                              s48_word_t * record,
                                              s48_word_t * type_binding);
extern int s48_record_type_number_of_fields (s48_arena_t arena,
                                             s48_word_t * record_type);
extern s48_error_t s48_record_set_type (s48_arena_t arena,
                                        s48_word_t * record,
                                        s48_word_t * type_binding);
extern s48_error_t s48_record_ref_type (s48_word_t * result,
                                        s48_arena_t arena,
                                        s48_word_t * record);
extern s48_error_t s48_record_set (s48_arena_t arena,
                                   s48_word_t * record,
                                   unsigned int i,
                                   s48_word_t * value);
extern s48_error_t s48_record_ref (s48_word_t * result,
                                   s48_arena_t arena,
                                   s48_word_t * record,
                                   unsigned int i);
extern s48_error_t s48_shared_binding_check (s48_arena_t arena,
                                             s48_word_t * value);
extern s48_error_t s48_new_make_record (s48_word_t * result,
                                        s48_arena_t arena,
                                        s48_word_t * type_shared_binding,
                                        ...);
extern s48_value s48_make_record(s48_value type_shared_binding);
#endif  /* INCLUDE__LIBSCHEME48__RECORDS_H */


/* arch-tag: Matthew Dempsky Wed Apr 20 23:01:29 2005 (records.h)
 */
