/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */
#ifndef INCLUDE__C__BOXING_H
#define INCLUDE__C__BOXING_H

#include <unistd.h>

/* automatically generated __STDC__ prototypes */
extern int s48_values_eq (s48_arena_t arena,
                          s48_word_t * a,
                          s48_word_t * b);
extern s48_error_t s48_make_nil (s48_word_t * result,
                                 s48_arena_t arena);
extern s48_error_t s48_is_nil (s48_arena_t arena,
                               s48_word_t * value);
extern s48_error_t s48_make_false (s48_word_t * result,
                                   s48_arena_t arena);
extern int s48_is_false (s48_arena_t arena,
                         s48_word_t * value);
extern s48_error_t s48_make_true (s48_word_t * result,
                                  s48_arena_t arena);
extern int s48_is_true (s48_arena_t arena,
                        s48_word_t * value);
extern s48_error_t s48_make_unspecific (s48_word_t * result,
                                        s48_arena_t arena);
extern int s48_to_cbool (s48_arena_t arena,
                         s48_word_t * value);
extern s48_error_t s48_from_cbool (s48_word_t * result,
                                   s48_arena_t arena,
                                   int cbool);
extern void * s48_pointer_value (s48_arena_t arena,
                                 s48_word_t * value);
extern void * s48_value_pointer(s48_value value);
extern s48_error_t s48_make_fixnum (s48_word_t * result,
                                    s48_arena_t arena,
                                    long value);
extern s48_value s48_enter_fixnum(long value);
extern s48_fixnum_t s48_fixnum_value (s48_arena_t arena,
                                      s48_word_t * value);
extern long s48_extract_fixnum(s48_value value);
extern s48_error_t s48_fixnum_to_number (s48_word_t * result,
                                         s48_arena_t arena,
                                         s48_fixnum_t value);
extern s48_error_t s48_number_to_fixnum (s48_fixnum_t * result,
                                         s48_arena_t arena,
                                         s48_word_t * value);
extern long s48_new_extract_integer (s48_arena_t arena,
                                     s48_word_t * value);
extern long s48_extract_integer(s48_value value);
extern s48_value s48_enter_ulong(unsigned long value);
extern unsigned long s48_extract_ulong(s48_value value);
extern void * s48_extract_bignum(s48_value value);
extern s48_error_t s48_make_double (s48_word_t * result,
                                    s48_arena_t arena,
                                    double value);
extern s48_value s48_enter_double(double value);
extern double s48_double_value (s48_arena_t arena,
                                s48_word_t * value);
extern double s48_extract_double(s48_value s48_double);
extern s48_error_t s48_make_character (s48_word_t * result,
                                       s48_arena_t arena,
                                       unsigned int codepoint);
extern s48_value s48_enter_char(unsigned char a_char);
extern unsigned char s48_character_codepoint (s48_arena_t arena,
                                              s48_word_t * value);
extern unsigned char s48_extract_char(s48_value a_char);
extern s48_error_t s48_make_pointer (s48_word_t * result,
                                     s48_arena_t arena,
                                     void * ptr);
extern s48_value s48_enter_pointer(void *pointer);
extern s48_error_t s48_new_cons (s48_word_t * result,
                                 s48_arena_t arena,
                                 s48_word_t * car,
                                 s48_word_t * cdr);
extern s48_value s48_cons(s48_value v1, s48_value v2);
extern s48_error_t s48_car (s48_word_t * result,
                            s48_arena_t arena,
                            s48_word_t * value);
extern s48_error_t s48_cdr (s48_word_t * result,
                            s48_arena_t arena,
                            s48_word_t * value);
extern s48_error_t s48_new_make_weak_pointer (s48_word_t * result,
                                              s48_arena_t arena,
                                              s48_word_t * value);
extern s48_value s48_make_weak_pointer(s48_value value);
extern s48_error_t s48_make_string_n (s48_word_t * result,
                                      s48_arena_t arena,
                                      char * str,
                                      long length);
extern s48_value s48_enter_substring(char *str, long length);
extern s48_error_t s48_new_make_string (s48_word_t * result,
                                        s48_arena_t arena,
                                        char * str);
extern s48_value s48_enter_string(char *str);
extern s48_error_t s48_string_lock_data (char ** result,
                                         s48_arena_t arena,
                                         s48_word_t * str);
extern s48_error_t s48_string_unlock_data (s48_arena_t arena,
                                           char * data);
extern char * s48_extract_string(s48_value string);
extern s48_value s48_enter_byte_vector(char *bytes, long length);
extern char * s48_extract_byte_vector(s48_value byte_vector);
extern s48_value s48_make_string(int length, char init);
extern s48_error_t s48_new_make_vector (s48_word_t * result,
                                        s48_arena_t arena,
                                        int n_elts,
                                        s48_word_t * init);
extern void s48_new_vector_ref (s48_word_t * result,
                                s48_arena_t arena,
                                s48_word_t * vector,
                                int n);
extern void s48_new_vector_set (s48_arena_t arena,
                                s48_word_t * vector,
                                int n,
                                s48_word_t * value);
extern s48_value s48_make_vector(int length, s48_value init);
extern s48_error_t s48_new_make_byte_vector (s48_word_t * result,
                                             s48_arena_t arena,
                                             int length);
extern void * s48_new_value_pointer (s48_arena_t arena,
                                     s48_word_t * value);
extern void s48_new_make_value (s48_word_t * result,
                                s48_arena_t arena,
                                void * value,
                                size_t size);
extern void s48_value_extract (s48_arena_t arena,
                               s48_word_t * value,
                               void * dest,
                               size_t size);
extern void s48_value_set (s48_arena_t arena,
                           s48_word_t * value,
                           void * src,
                           size_t size);
extern s48_value s48_make_byte_vector(int length);
extern void s48_check_record_type(s48_value record, s48_value type_binding);
#endif  /* INCLUDE__C__BOXING_H */
