/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <string.h>
#include <limits.h>

#include "config-options.h"

#include "scheme48/libscheme48/scheme48.h"
#include "scheme48/libscheme48/scheme48vm.h"
#include "scheme48/libscheme48/bignum.h"

/********************************/
/* Equality operators
 */

int
s48_values_eq (s48_arena_t arena,
               s48_word_t * a,
               s48_word_t * b)
{
  return S48_EQ_P (*a, *b);
}

/********************************/
/* Built-in types and objects.
 */

s48_error_t
s48_make_nil (s48_word_t * result,
              s48_arena_t arena)
{
  *result = S48_NULL;
  return 0;
}

s48_error_t
s48_is_nil (s48_arena_t arena,
            s48_word_t * value)
{
  return *value == S48_NULL;
}

s48_error_t
s48_make_false (s48_word_t * result,
                s48_arena_t arena)
{
  *result = S48_FALSE;
  return 0;
}

int
s48_is_false (s48_arena_t arena,
              s48_word_t * value)
{
  return *value == S48_FALSE;
}

s48_error_t
s48_make_true (s48_word_t * result,
               s48_arena_t arena)
{
  *result = S48_TRUE;
  return 0;
}

int
s48_is_true (s48_arena_t arena,
             s48_word_t * value)
{
  return *value == S48_TRUE;
}

s48_error_t
s48_make_unspecific (s48_word_t * result,
                     s48_arena_t arena)
{
  *result = S48_UNSPECIFIC;
  return 0;
}

int
s48_to_cbool (s48_arena_t arena,
              s48_word_t * value)
{
  return !s48_is_false (arena, value);
}

s48_error_t
s48_from_cbool (s48_word_t * result,
                s48_arena_t arena,
                int cbool)
{
  if (cbool)
    return s48_make_true (result, arena);
  else
    return s48_make_false (result, arena);
}

/********************************/
/* Support routines for external code */

void *
s48_pointer_value (s48_arena_t arena,
                   s48_word_t * value)
{
  return s48_value_pointer (*value);
}

void *
s48_value_pointer(s48_value value)
{
  S48_CHECK_VALUE(value);

  return S48_ADDRESS_AFTER_HEADER(value, void *);
}

/********************************/
/* Numbers, characters, and pointers. */

/*
 * These two functions have the same range as the unsafe macros, but they signal
 * an error if things go wrong, instead of silently producing garbage.  Unlike
 * the integer versions they cannot cause a GC.
 */

s48_error_t
s48_make_fixnum (s48_word_t * result,
                 s48_arena_t arena,
                 long value)
{
  *result = s48_enter_fixnum (value);
  return 0;
}

s48_value
s48_enter_fixnum(long value)
{
  if (value < S48_MIN_FIXNUM_VALUE || S48_MAX_FIXNUM_VALUE < value)
    s48_raise_argument_type_error(s48_enter_integer(value));

  return S48_UNSAFE_ENTER_FIXNUM(value);
}

s48_fixnum_t
s48_fixnum_value (s48_arena_t arena,
                  s48_word_t * value)
{
  return s48_extract_fixnum (*value);
}

long
s48_extract_fixnum(s48_value value)
{
  if (! S48_FIXNUM_P(value))
    s48_raise_argument_type_error(value);
  
  return S48_UNSAFE_EXTRACT_FIXNUM(value);
}

/* If we have a fixnum we just extract it. For bignums call the
 * functions in bignum.c.
 */

s48_error_t
s48_fixnum_to_number (s48_word_t * result,
                      s48_arena_t arena,
                      s48_fixnum_t value)
{
  *result = s48_enter_integer (value);
  return 0;
}

s48_error_t
s48_number_to_fixnum (s48_fixnum_t * result,
                      s48_arena_t arena,
                      s48_word_t * value)
{
  *result = s48_extract_integer (*value);
  return 0;
}

long
s48_new_extract_integer (s48_arena_t arena,
                         s48_word_t * value)
{
  return s48_extract_integer (*value);
}

long
s48_extract_integer(s48_value value)
{
  if (S48_FIXNUM_P(value))
    return S48_UNSAFE_EXTRACT_FIXNUM(value);

  if (S48_BIGNUM_P(value)){
    bignum_type bignum = S48_ADDRESS_AFTER_HEADER(value, long);
    
    if (! s48_bignum_fits_in_word_p(bignum, 32, 1))
      s48_raise_argument_type_error (value);
    else return s48_bignum_to_long(bignum);
  }
  else s48_raise_argument_type_error(value);
}

s48_value
s48_enter_ulong(unsigned long value)
{
  if (value <= LONG_MAX)
    return s48_enter_integer((long) value);
  else
    return s48_enter_bignum (s48_ulong_to_bignum (value));
}

unsigned long
s48_extract_ulong(s48_value value)
{
  if (S48_FIXNUM_P(value))
    return S48_UNSAFE_EXTRACT_FIXNUM(value);

  if (S48_BIGNUM_P(value)){
    bignum_type bignum = s48_extract_bignum(value);
    
    if (! s48_bignum_fits_in_word_p(bignum, 32, 0))
      s48_raise_argument_type_error (value);
    else return s48_bignum_to_ulong(bignum);
  }
  else s48_raise_argument_type_error(value);
}

void *
s48_extract_bignum(s48_value value)
{
  return S48_ADDRESS_AFTER_HEADER(value, long);
}

/*
 * Doubles and characters are straightforward.
 */

s48_error_t
s48_make_double (s48_word_t * result,
                 s48_arena_t arena,
                 double value)
{
  s48_new_allocate_stob (result, arena, S48_STOBTYPE_DOUBLE, sizeof (double));
  S48_UNSAFE_EXTRACT_DOUBLE (*result) = value;

  return 0;
}

s48_value
s48_enter_double(double value)
{
  s48_value obj;

  obj = s48_allocate_stob(S48_STOBTYPE_DOUBLE, sizeof(double));
  S48_UNSAFE_EXTRACT_DOUBLE(obj) = value;

  return obj;
}

double
s48_double_value (s48_arena_t arena,
                  s48_word_t * value)
{
  return s48_extract_double (*value);
}

double
s48_extract_double(s48_value s48_double)
{
  if (! S48_DOUBLE_P(s48_double))
    s48_raise_argument_type_error(s48_double);
  
  return S48_UNSAFE_EXTRACT_DOUBLE(s48_double);
}

s48_error_t
s48_make_character (s48_word_t * result,
                    s48_arena_t arena,
                    unsigned int codepoint)
{
  if (codepoint > 255)
    s48_new_raise_fixnum_range_error (arena, codepoint, 0, 255);

  *result = S48_UNSAFE_ENTER_CHAR (codepoint);
  return 0;
}

s48_value
s48_enter_char(unsigned char a_char)
{
  if (a_char > 255)
    s48_raise_range_error(s48_enter_fixnum(a_char),
			  s48_enter_fixnum(0),
			  s48_enter_fixnum(255));

  return S48_UNSAFE_ENTER_CHAR(a_char);
}

unsigned char
s48_character_codepoint (s48_arena_t arena,
                         s48_word_t * value)
{
  return s48_extract_char (*value);
}

unsigned char
s48_extract_char(s48_value a_char)
{
  if (! S48_CHAR_P(a_char))
    s48_raise_argument_type_error(a_char);
  
  return S48_UNSAFE_EXTRACT_CHAR(a_char);
}

/********************************/
/* Allocation */

s48_error_t
s48_make_pointer (s48_word_t * result,
                  s48_arena_t arena,
                  void * ptr)
{
  s48_new_allocate_stob (result, arena, S48_STOBTYPE_BYTE_VECTOR, sizeof(void *));
  *(S48_ADDRESS_AFTER_HEADER (*result, void *)) = ptr;

  return 0;
}

s48_value
s48_enter_pointer(void *pointer)
{
  s48_value obj;

  obj = s48_allocate_stob(S48_STOBTYPE_BYTE_VECTOR, sizeof(void *));
  *(S48_ADDRESS_AFTER_HEADER(obj, void *)) = pointer;

  return obj;
}

s48_error_t
s48_new_cons (s48_word_t * result,
              s48_arena_t arena,
              s48_word_t * car,
              s48_word_t * cdr)
{
  struct s48_new_cons_locals
    {
      S48_FRAME;
      s48_word_t tmp;
    } l;

  S48_PROTECT_FRAME (l);

  s48_new_allocate_stob (&l.tmp, arena, S48_STOBTYPE_PAIR, 2);
  S48_UNSAFE_SET_CAR (l.tmp, *car);
  S48_UNSAFE_SET_CDR (l.tmp, *cdr);

  S48_LSET (result, &l.tmp);

  S48_UNPROTECT_FRAME (l);
  return 0;
}

s48_value
s48_cons(s48_value v1, s48_value v2)
{
  s48_value obj;
  S48_DECLARE_GC_PROTECT(2);

  S48_GC_PROTECT_2(v1, v2);

  obj = s48_allocate_stob(S48_STOBTYPE_PAIR, 2);
  S48_UNSAFE_SET_CAR(obj, v1);
  S48_UNSAFE_SET_CDR(obj, v2);

  S48_GC_UNPROTECT();
  return obj;
}

s48_error_t
s48_car (s48_word_t * result,
         s48_arena_t arena,
         s48_word_t * value)
{
  *result = S48_CAR (*value);
  return 0;
}

s48_error_t
s48_cdr (s48_word_t * result,
         s48_arena_t arena,
         s48_word_t * value)
{
  *result = S48_CDR (*value);
  return 0;
}

s48_error_t
s48_new_make_weak_pointer (s48_word_t * result,
                           s48_arena_t arena,
                           s48_word_t * value)
{
  struct s48_new_make_weak_pointer_locals
    {
      S48_FRAME;
      s48_word_t tmp;
    } l;

  S48_PROTECT_FRAME (l);

  s48_new_allocate_stob (&l.tmp, arena, S48_STOBTYPE_WEAK_POINTER, 1);
  S48_STOB_SET (l.tmp, 0, *value);

  S48_LSET (result, &l.tmp);

  S48_UNPROTECT_FRAME (l);
  return 0;
}

s48_value
s48_make_weak_pointer(s48_value value)
{
  s48_value obj;
  S48_DECLARE_GC_PROTECT(1);

  S48_GC_PROTECT_1(value);

  obj = s48_allocate_stob(S48_STOBTYPE_WEAK_POINTER, 1);
  S48_STOB_SET(obj, 0, value);

  S48_GC_UNPROTECT();
  return obj;
}

/*
 * Entering and extracting strings.
 */

s48_error_t
s48_make_string_n (s48_word_t * result,
                   s48_arena_t arena,
                   char * str,
                   long length)
{
  s48_new_allocate_stob (result, arena, S48_STOBTYPE_STRING, length + 1);

  memcpy (S48_UNSAFE_EXTRACT_STRING (*result), str, length);
  *(S48_UNSAFE_EXTRACT_STRING (*result) + length) = '\0';

  return 0;
}

s48_value
s48_enter_substring(char *str, long length)
{
  s48_value obj = s48_allocate_stob(S48_STOBTYPE_STRING, length + 1);
  memcpy(S48_UNSAFE_EXTRACT_STRING(obj), str, length);
  *(S48_UNSAFE_EXTRACT_STRING(obj) + length) = '\0';
  return obj;
}

s48_error_t
s48_new_make_string (s48_word_t * result,
                     s48_arena_t arena,
                     char * str)
{
  return s48_make_string_n (result, arena, str, strlen (str));
}

s48_value
s48_enter_string(char *str)
{
  return s48_enter_substring(str, strlen(str));
}

s48_error_t
s48_string_lock_data (char ** result,
                      s48_arena_t arena,
                      s48_word_t * str)
{
  *result = s48_extract_string (*str);
  return 0;
}

s48_error_t
s48_string_unlock_data (s48_arena_t arena,
                        char * data)
{
  return 0;
}

char *
s48_extract_string(s48_value string)
{
  S48_CHECK_STRING(string);

  return S48_UNSAFE_EXTRACT_STRING(string);
}

/*
 * Entering and extracting byte vectors.
 */

s48_value
s48_enter_byte_vector(char *bytes, long length)
{
  s48_value obj = s48_allocate_stob(S48_STOBTYPE_BYTE_VECTOR, length);
  memcpy(S48_UNSAFE_EXTRACT_STRING(obj), bytes, length);
  return obj;
}

char *
s48_extract_byte_vector(s48_value byte_vector)
{
  S48_CHECK_VALUE(byte_vector);

  return S48_UNSAFE_EXTRACT_STRING(byte_vector);
}

/*
 * Making various kinds of stored objects.
 */

s48_value
s48_make_string(int length, char init)
{
  s48_value obj = s48_allocate_stob(S48_STOBTYPE_STRING, length+1);
  memset(S48_UNSAFE_EXTRACT_STRING(obj), init, length);
  S48_UNSAFE_EXTRACT_STRING(obj)[length] = '\0';
  return obj;
}

s48_error_t
s48_new_make_vector (s48_word_t * result,
                     s48_arena_t arena,
                     int n_elts,
                     s48_word_t * init)
{
  struct s48_new_make_vector_locals
    {
      S48_FRAME;
      s48_word_t tmp;
    } l;
  int i;

  S48_PROTECT_FRAME (l);

  s48_new_allocate_stob (&l.tmp, arena, S48_STOBTYPE_VECTOR, n_elts);
  for (i = 0; i < n_elts; ++i)
    S48_UNSAFE_VECTOR_SET (l.tmp, i, *init);

  S48_LSET (result, &l.tmp);

  S48_UNPROTECT_FRAME (l);
  return 0;
}

void
s48_new_vector_ref (s48_word_t * result,
                    s48_arena_t arena,
                    s48_word_t * vector,
                    int n)
{
  s48_new_stob_ref (result, arena, vector, S48_STOBTYPE_VECTOR, n);
}

void
s48_new_vector_set (s48_arena_t arena,
                    s48_word_t * vector,
                    int n,
                    s48_word_t * value)
{
  s48_new_stob_set (arena, value, S48_STOBTYPE_VECTOR, n, value);
}


s48_value
s48_make_vector(int length, s48_value init)
{
  int i;
  s48_value obj;
  S48_DECLARE_GC_PROTECT(1);

  S48_GC_PROTECT_1(init);

  obj = s48_allocate_stob(S48_STOBTYPE_VECTOR, length);
  for (i = 0; i < length; ++i)
    S48_UNSAFE_VECTOR_SET(obj, i, init);

  S48_GC_UNPROTECT();

  return obj;
}

s48_error_t
s48_new_make_byte_vector (s48_word_t * result,
                          s48_arena_t arena,
                          int length)
{
  return s48_new_allocate_stob (result, arena, S48_STOBTYPE_BYTE_VECTOR, length);
}

void *
s48_new_value_pointer (s48_arena_t arena,
                       s48_word_t * value)
{
  return s48_value_pointer (*value);
}

void
s48_new_make_value (s48_word_t * result,
                    s48_arena_t arena,
                    void * value,
                    size_t size)
{
  struct s48_new_make_value_frame
    {
      S48_FRAME;
      s48_word_t tmp;
    } l;

  S48_PROTECT_FRAME (l);

  s48_new_make_byte_vector (&l.tmp, arena, size);
  memcpy (s48_new_value_pointer (arena, &l.tmp), value, size);

  S48_LSET (result, &l.tmp);

  S48_UNPROTECT_FRAME (l);
}

void
s48_value_extract (s48_arena_t arena,
                   s48_word_t * value,
                   void * dest,
                   size_t size)
{
  memcpy (dest, s48_new_value_pointer (arena, value), size);
}

void
s48_value_set (s48_arena_t arena,
               s48_word_t * value,
               void * src,
               size_t size)
{
  memcpy (s48_new_value_pointer (arena, value), src, size);
}

s48_value
s48_make_byte_vector(int length)
{
    return s48_allocate_stob(S48_STOBTYPE_BYTE_VECTOR, length);
}

/*
 * Raise an exception if `record' is not a record whose type is the one
 * found in `type_binding'.
 */
void
s48_check_record_type(s48_value record, s48_value type_binding)
{
  if (! S48_RECORD_P(S48_SHARED_BINDING_REF(type_binding)))
    s48_raise_scheme_exception(S48_EXCEPTION_UNBOUND_EXTERNAL_NAME, 1,
			       S48_SHARED_BINDING_NAME(type_binding));

  if ((! S48_RECORD_P(record)) ||
      (S48_UNSAFE_SHARED_BINDING_REF(type_binding) !=
       S48_UNSAFE_RECORD_REF(record, -1)))
    s48_raise_argument_type_error(record);
}    
