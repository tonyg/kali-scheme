/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <string.h>
#include "scheme48/libscheme48/scheme48.h"
#include "scheme48/libscheme48/scheme48vm.h"

/* Specific exceptions */

void
s48_new_raise_type_error (s48_arena_t arena,
                          s48_word_t * value)
{
  s48_new_raise_scheme_exception (arena,
                                  S48_EXCEPTION_WRONG_TYPE_ARGUMENT,
                                  1, value);
}

void
s48_new_raise_range_error (s48_arena_t arena,
                           s48_word_t * value,
                           s48_word_t * min,
                           s48_word_t * max)
{
  s48_new_raise_scheme_exception (arena,
                                  S48_EXCEPTION_INDEX_OUT_OF_RANGE,
                                  3, value, min, max);
}

void
s48_new_raise_fixnum_range_error (s48_arena_t arena,
                                  s48_fixnum_t value,
                                  s48_fixnum_t min,
                                  s48_fixnum_t max)
{
  struct s48_new_raise_fixnum_range_error_locals
    {
      S48_FRAME;
      s48_word_t value;
      s48_word_t min;
      s48_word_t max;
    } l;

  S48_PROTECT_FRAME (l);

  s48_fixnum_to_number (&l.value, arena, value);
  s48_fixnum_to_number (&l.min, arena, min);
  s48_fixnum_to_number (&l.max, arena, max);

  s48_new_raise_range_error (arena, &l.value, &l.min, &l.max);

  S48_UNPROTECT_FRAME (l);
}

void
s48_new_raise_closed_channel_error (s48_arena_t arena)
{
  s48_new_raise_scheme_exception (arena,
                                  S48_EXCEPTION_CLOSED_CHANNEL,
                                  0);
}

void
s48_new_raise_os_error (s48_arena_t arena,
                        int errn)
{
  struct s48_new_raise_os_error_frame
    {
      S48_FRAME;
      s48_word_t errn;
      s48_word_t string;
    } l;

  S48_PROTECT_FRAME (l);

  s48_fixnum_to_number (&l.errn, arena, errn);
  s48_new_make_string (&l.string, arena, strerror (errn));

  s48_new_raise_scheme_exception (arena,
                                  S48_EXCEPTION_OS_ERROR,
                                  2, &l.errn, &l.string);

  S48_UNPROTECT_FRAME (l);
}

void
s48_new_raise_string_os_error (s48_arena_t arena,
                               char * reason)
{
  struct s48_new_raise_string_os_error_frame
    {
      S48_FRAME;
      s48_word_t string;
    } l;

  S48_PROTECT_FRAME (l);

  s48_new_make_string (&l.string, arena, reason);

  s48_new_raise_scheme_exception (arena,
                                  S48_EXCEPTION_OS_ERROR,
                                  1, &l.string);

  S48_UNPROTECT_FRAME (l);
}

void
s48_new_raise_out_of_memory_error (s48_arena_t arena)
{
  s48_new_raise_scheme_exception (arena,
                                  S48_EXCEPTION_OUT_OF_MEMORY,
                                  0);
}



/* Specific exceptions */

void
s48_raise_argument_type_error (s48_value value)
{
  s48_raise_scheme_exception(S48_EXCEPTION_WRONG_TYPE_ARGUMENT, 1, value);
}

/* Superceded name for preceding function, retained for compatibility. */

void
s48_raise_argtype_error (s48_value value)
{
  s48_raise_scheme_exception(S48_EXCEPTION_WRONG_TYPE_ARGUMENT, 1, value);
}

void
s48_raise_argument_number_error (s48_value value, s48_value min, s48_value max)
{
  s48_raise_scheme_exception(S48_EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS,
			     3, value, min, max);
}

/* Superceded name for preceding function, retained for compatibility. */

void
s48_raise_argnumber_error (s48_value value, s48_value min, s48_value max)
{
  s48_raise_scheme_exception(S48_EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS,
			     3, value, min, max);
}

void
s48_raise_range_error (s48_value value, s48_value min, s48_value max)
{
  s48_raise_scheme_exception(S48_EXCEPTION_INDEX_OUT_OF_RANGE,
			     3, value, min, max);
}

void
s48_raise_closed_channel_error ()
{
  s48_raise_scheme_exception(S48_EXCEPTION_CLOSED_CHANNEL, 0);
}

void
s48_raise_os_error (int the_errno)
{
  s48_raise_scheme_exception(S48_EXCEPTION_OS_ERROR, 2,
			     s48_enter_fixnum(the_errno),
			     s48_enter_string(strerror(the_errno)));
}

void
s48_raise_string_os_error (char *reason)
{
  s48_raise_scheme_exception(S48_EXCEPTION_OS_ERROR, 1,
			     s48_enter_string(reason));
}

void
s48_raise_out_of_memory_error ()
{
  s48_raise_scheme_exception(S48_EXCEPTION_OUT_OF_MEMORY, 0);
}
