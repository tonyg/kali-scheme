/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */


#include <stdarg.h>
#include "scheme48/libscheme48/scheme48.h"
#include "scheme48/libscheme48/records.h"



int
s48_is_record (s48_arena_t arena,
               s48_word_t * value)
{
  return S48_RECORD_P (*value);
}

s48_error_t
s48_new_shared_binding_ref (s48_word_t * result,
                            s48_arena_t arena,
                            s48_word_t * type_shared_binding)
{
  s48_new_stob_ref (result, arena, type_shared_binding, S48_STOBTYPE_SHARED_BINDING, 2);
  return 0;
}

s48_error_t
s48_new_record_type_check (s48_arena_t arena,
                           s48_word_t * record,
                           s48_word_t * type_binding)
{
  struct s48_new_record_type_check_frame
    {
      S48_FRAME;
      s48_word_t tmp;
    } l;

  S48_PROTECT_FRAME (l);

  if (!S48_RECORD_P (S48_SHARED_BINDING_REF (*type_binding)))
    {
      l.tmp = S48_SHARED_BINDING_NAME (*type_binding);
      s48_new_raise_scheme_exception (arena,
                                      S48_EXCEPTION_UNBOUND_EXTERNAL_NAME,
                                      1, &l.tmp);
    }

  if (!s48_is_record (arena, record))
    s48_new_raise_type_error (arena, record);

  if (   S48_UNSAFE_SHARED_BINDING_REF (*type_binding)
      != S48_UNSAFE_RECORD_REF (*record, -1))
    s48_new_raise_type_error (arena, record);

  S48_UNPROTECT_FRAME (l);
  return 0;
}

int
s48_record_type_number_of_fields (s48_arena_t arena,
                                  s48_word_t * record_type)
{
  struct s48_record_type_number_of_fields_frame
    {
      S48_FRAME;
      s48_word_t n_fields;
    } l;

  int answer;

  S48_PROTECT_FRAME (l);

  s48_record_ref (&l.n_fields, arena, record_type, 4);
  answer = s48_new_extract_integer (arena, &l.n_fields);

  S48_UNPROTECT_FRAME (l);
}

s48_error_t
s48_record_set_type (s48_arena_t arena,
                     s48_word_t * record,
                     s48_word_t * type_binding)
{
  s48_new_stob_set (arena, record, S48_STOBTYPE_RECORD, 0, type_binding);
  return 0;
}

s48_error_t
s48_record_ref_type (s48_word_t * result,
                     s48_arena_t arena,
                     s48_word_t * record)
{
  s48_new_stob_ref (result, arena, record, S48_STOBTYPE_RECORD, 0);
  return 0;
}

s48_error_t
s48_record_set (s48_arena_t arena,
                s48_word_t * record,
                unsigned int i,
                s48_word_t * value)
{
  s48_new_stob_set (arena, record, S48_STOBTYPE_RECORD, i + 1, value);
  return 0;
}

s48_error_t
s48_record_ref (s48_word_t * result,
                s48_arena_t arena,
                s48_word_t * record,
                unsigned int i)
{
  s48_new_stob_ref (result, arena, record, S48_STOBTYPE_RECORD, i + 1);
  return 0;
}

s48_error_t
s48_shared_binding_check (s48_arena_t arena,
                          s48_word_t * value)
{
  S48_SHARED_BINDING_CHECK (*value);
  return 0;
}

s48_error_t
s48_new_make_record (s48_word_t * result,
                     s48_arena_t arena,
                     s48_word_t * type_shared_binding,
                     ...)
{
  struct s48_new_make_record_frame
    {
      S48_FRAME;
      s48_word_t record;
      s48_word_t record_type;
      s48_word_t unspecific;
    } l;

  int i, n_fields;
  va_list ap;

  s48_shared_binding_check (arena, type_shared_binding);
  s48_shared_binding_check (arena, &the_record_type_binding);

  S48_PROTECT_FRAME (l);

  s48_new_shared_binding_ref (&l.record_type, arena, type_shared_binding);
  s48_new_record_type_check (arena, &l.record_type, &the_record_type_binding);

  n_fields = s48_record_type_number_of_fields (arena, &l.record_type);

  s48_new_allocate_stob (&l.record, arena, S48_STOBTYPE_RECORD, n_fields + 1);

  s48_record_set_type (arena, &l.record, &l.record_type);

  va_start (ap, type_shared_binding);
  for (i = 0; i < n_fields; ++i)
    {
      s48_word_t * next;

      next = va_arg (ap, s48_word_t *);
      if (next == s48_va_end)
        break;

      s48_record_set (arena, &l.record, i, next);
    }
  va_end (ap);

  s48_make_unspecific (&l.unspecific, arena);
  for ( ; i < n_fields; ++i)
    s48_record_set (arena, &l.record, i, &l.unspecific);

  S48_LSET (result, &l.record);

  S48_UNPROTECT_FRAME (l);
  return 0;
}

s48_value
s48_make_record(s48_value type_shared_binding)
{
    int i, number_of_fields;
    s48_value record = S48_FALSE;
    s48_value record_type = S48_FALSE;
    S48_DECLARE_GC_PROTECT(1);

    S48_GC_PROTECT_1(record_type);

    S48_SHARED_BINDING_CHECK(type_shared_binding);
    S48_SHARED_BINDING_CHECK(the_record_type_binding);

    record_type = S48_SHARED_BINDING_REF(type_shared_binding);

    s48_check_record_type(record_type, the_record_type_binding);

    number_of_fields =
      S48_UNSAFE_EXTRACT_FIXNUM(S48_RECORD_TYPE_NUMBER_OF_FIELDS(record_type));

    record = s48_allocate_stob(S48_STOBTYPE_RECORD, number_of_fields + 1);

    S48_UNSAFE_RECORD_SET(record, -1, record_type);
    for (i = 0; i < number_of_fields; ++i)
      S48_UNSAFE_RECORD_SET(record, i, S48_UNSPECIFIC);

    S48_GC_UNPROTECT();

    return record;
}



/* arch-tag: Matthew Dempsky Wed Apr 20 23:00:10 2005 (records.c)
 */
