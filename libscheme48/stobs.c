/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include "scheme48/libscheme48/scheme48.h"
#include "scheme48/libscheme48/scheme48vm.h"

/* Wrapper to provide new style API for allocating stobs until it's
 * merged into the VM code.
 */

s48_error_t
s48_new_allocate_stob (s48_word_t * answer,
                       s48_arena_t arena,
                       long type,
                       long size)
{
  *answer = s48_allocate_stob (type, size);
  return 0;
}

/*
 * Type-safe procedures for checking types and dereferencing and setting slots.
 */

int
s48_new_stob_has_type (s48_arena_t arena,
                       s48_word_t * thing,
                       int type)
{
  return S48_STOB_P (*thing) && (S48_STOB_TYPE (*thing) == type);
}

void
s48_new_stob_check_type (s48_arena_t arena,
                         s48_word_t * thing,
                         int type)
{
  if (!s48_new_stob_has_type (arena, thing, type))
    s48_new_raise_type_error (arena, thing);
}

void
s48_new_stob_check_mutable (s48_arena_t arena,
                            s48_word_t * thing)
{
  if (S48_STOB_IMMUTABLEP (*thing))
    s48_new_raise_type_error (arena, thing);
}

int
s48_stob_has_type(s48_value thing, int type)
{
  return S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type);
}


long
s48_new_stob_length (s48_arena_t arena,
                     s48_word_t * thing,
                     int type)
{
  s48_new_stob_check_type (arena, thing, type);

  return S48_STOB_DESCRIPTOR_LENGTH (*thing);
}

long
s48_stob_length(s48_value thing, int type)
{
  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_raise_argument_type_error(thing);
  
  return S48_STOB_DESCRIPTOR_LENGTH(thing);
}


long
s48_new_stob_byte_length (s48_arena_t arena,
                          s48_word_t * thing,
                          int type)
{
  long length;

  s48_new_stob_check_type (arena, thing, type);

  length = S48_STOB_BYTE_LENGTH (*thing);
  if (type == S48_STOBTYPE_STRING)
    --length;

  return length;
}

long
s48_stob_byte_length(s48_value thing, int type)
{
  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_raise_argument_type_error(thing);

  if (type == S48_STOBTYPE_STRING)
    return S48_STOB_BYTE_LENGTH(thing) - 1;
  else
    return S48_STOB_BYTE_LENGTH(thing);
}

s48_error_t
s48_new_stob_ref (s48_word_t * result,
                  s48_arena_t arena,
                  s48_word_t * thing,
                  int type,
                  long offset)
{
  long length;

  length = s48_new_stob_length (arena, thing, type);

  if ((offset < 0) || (offset >= length))
    s48_new_raise_fixnum_range_error (arena, offset, 0, length - 1);

  *result = S48_STOB_REF (*thing, offset);
  return 0;
}

s48_value
s48_stob_ref(s48_value thing, int type, long offset)
{
  long length;

  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_raise_argument_type_error(thing);

  length = S48_STOB_DESCRIPTOR_LENGTH(thing);

  if (offset < 0 || length <= offset)
    s48_raise_range_error(s48_enter_integer(offset),
			  S48_UNSAFE_ENTER_FIXNUM(0),
		          S48_UNSAFE_ENTER_FIXNUM(length - 1));
			  
  return S48_STOB_REF(thing, offset);
}

s48_error_t
s48_new_stob_set (s48_arena_t arena,
                  s48_word_t * thing,
                  int type,
                  long offset,
                  s48_word_t * value)
{
  long length;

  length = s48_new_stob_length (arena, thing, type);

  if ((offset < 0) || (offset >= length))
    s48_new_raise_fixnum_range_error (arena, offset, 0, length - 1);

  s48_new_stob_check_mutable (arena, thing);

  S48_STOB_SET (*thing, offset, *value);
  return 0;
}

void
s48_stob_set(s48_value thing, int type, long offset, s48_value value)
{
  long length;
  
  if (!(S48_STOB_P(thing) &&
	(S48_STOB_TYPE(thing) == type) &&
	!S48_STOB_IMMUTABLEP(thing)))
    s48_raise_argument_type_error(thing);
  
  length = S48_STOB_DESCRIPTOR_LENGTH(thing);

  if (offset < 0 || length <= offset)
    s48_raise_range_error(s48_enter_integer(offset),
			  S48_UNSAFE_ENTER_FIXNUM(0),
			  S48_UNSAFE_ENTER_FIXNUM(length - 1));
			  
  S48_STOB_SET(thing, offset, value);
}


char
s48_new_stob_byte_ref (s48_arena_t arena,
                       s48_word_t * thing,
                       int type,
                       long offset)
{
  long length;

  length = s48_new_stob_byte_length (arena, thing, type);

  if ((offset < 0) || (offset >= length))
    s48_new_raise_fixnum_range_error (arena, offset, 0, length - 1);

  return S48_STOB_BYTE_REF (*thing, offset);
}

char
s48_stob_byte_ref(s48_value thing, int type, long offset)
{
  long length;

  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_raise_argument_type_error(thing);
  
  length = (type == S48_STOBTYPE_STRING) ?
           S48_STOB_BYTE_LENGTH(thing) - 1 :
           S48_STOB_BYTE_LENGTH(thing);
  
  if (offset < 0 || length <= offset)
    s48_raise_range_error(s48_enter_integer(offset),
			  S48_UNSAFE_ENTER_FIXNUM(0),
			  S48_UNSAFE_ENTER_FIXNUM(length - 1));
			  
  return S48_STOB_BYTE_REF(thing, offset);
}


s48_error_t
s48_new_stob_byte_set (s48_arena_t arena,
                       s48_word_t * thing,
                       int type,
                       long offset,
                       char value)
{
  long length;

  length = s48_new_stob_byte_length (arena, thing, type);

  if ((offset < 0) || (offset >= length))
    s48_new_raise_fixnum_range_error (arena, offset, 0, length - 1);

  s48_new_stob_check_mutable (arena, thing);

  S48_STOB_BYTE_SET (*thing, offset, value);
  return 0;
}


void
s48_stob_byte_set(s48_value thing, int type, long offset, char value)
{
  long length;

  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_raise_argument_type_error(thing);
  
  length = (type == S48_STOBTYPE_STRING) ?
           S48_STOB_BYTE_LENGTH(thing) - 1 :
           S48_STOB_BYTE_LENGTH(thing);
  
  if (offset < 0 || length <= offset)
    s48_raise_range_error(s48_enter_integer(offset),
			  S48_UNSAFE_ENTER_FIXNUM(0),
			  S48_UNSAFE_ENTER_FIXNUM(length - 1));
			  
  S48_STOB_BYTE_SET(thing, offset, value);
}
