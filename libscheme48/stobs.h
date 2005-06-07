/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */
#ifndef INCLUDE__FFI__STOBS_H
#define INCLUDE__FFI__STOBS_H

/* automatically generated __STDC__ prototypes */
extern s48_error_t s48_new_allocate_stob (s48_word_t * answer,
                                          s48_arena_t arena,
                                          long type,
                                          long size);
extern int s48_new_stob_has_type (s48_arena_t arena,
                                  s48_word_t * thing,
                                  int type);
extern void s48_new_stob_check_type (s48_arena_t arena,
                                     s48_word_t * thing,
                                     int type);
extern void s48_new_stob_check_mutable (s48_arena_t arena,
                                        s48_word_t * thing);
extern int s48_stob_has_type(s48_value thing, int type);
extern long s48_new_stob_length (s48_arena_t arena,
                                 s48_word_t * thing,
                                 int type);
extern long s48_stob_length(s48_value thing, int type);
extern long s48_new_stob_byte_length (s48_arena_t arena,
                                      s48_word_t * thing,
                                      int type);
extern long s48_stob_byte_length(s48_value thing, int type);
extern s48_error_t s48_new_stob_ref (s48_word_t * result,
                                     s48_arena_t arena,
                                     s48_word_t * thing,
                                     int type,
                                     long offset);
extern s48_value s48_stob_ref(s48_value thing, int type, long offset);
extern s48_error_t s48_new_stob_set (s48_arena_t arena,
                                     s48_word_t * thing,
                                     int type,
                                     long offset,
                                     s48_word_t * value);
extern void s48_stob_set(s48_value thing, int type, long offset, s48_value value);
extern char s48_new_stob_byte_ref (s48_arena_t arena,
                                   s48_word_t * thing,
                                   int type,
                                   long offset);
extern char s48_stob_byte_ref(s48_value thing, int type, long offset);
extern s48_error_t s48_new_stob_byte_set (s48_arena_t arena,
                                          s48_word_t * thing,
                                          int type,
                                          long offset,
                                          char value);
extern void s48_stob_byte_set(s48_value thing, int type, long offset, char value);
#endif  /* INCLUDE__FFI__STOBS_H */
