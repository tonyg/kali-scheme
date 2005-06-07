/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */
#ifndef INCLUDE__C__EXCEPTIONS_H
#define INCLUDE__C__EXCEPTIONS_H

/* automatically generated __STDC__ prototypes */
extern void s48_new_raise_type_error (s48_arena_t arena,
                                      s48_word_t * value);
extern void s48_new_raise_range_error (s48_arena_t arena,
                                       s48_word_t * value,
                                       s48_word_t * min,
                                       s48_word_t * max);
extern void s48_new_raise_fixnum_range_error (s48_arena_t arena,
                                              s48_fixnum_t value,
                                              s48_fixnum_t min,
                                              s48_fixnum_t max);
extern void s48_new_raise_closed_channel_error (s48_arena_t arena);
extern void s48_new_raise_os_error (s48_arena_t arena,
                                    int errn);
extern void s48_new_raise_string_os_error (s48_arena_t arena,
                                           char * reason);
extern void s48_new_raise_out_of_memory_error (s48_arena_t arena);
extern void s48_raise_argument_type_error (s48_value value);
extern void s48_raise_argtype_error (s48_value value);
extern void s48_raise_argument_number_error (s48_value value, s48_value min, s48_value max);
extern void s48_raise_argnumber_error (s48_value value, s48_value min, s48_value max);
extern void s48_raise_range_error (s48_value value, s48_value min, s48_value max);
extern void s48_raise_closed_channel_error ();
extern void s48_raise_os_error (int the_errno);
extern void s48_raise_string_os_error (char *reason);
extern void s48_raise_out_of_memory_error ();
#endif  /* INCLUDE__C__EXCEPTIONS_H */
