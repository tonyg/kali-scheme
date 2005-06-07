/* utils.c:
 *
 ****************************************************************
 * Copyright (C) 2005 Matthew Dempsky
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include <stdarg.h>
#include "scheme48/libscheme48/scheme48.h"
#include "scheme48/libscheme48/utils.h"



/* __STDC__ prototypes for static functions */
static s48_error_t list_helper (s48_word_t * result,
                                s48_arena_t arena,
                                va_list ap);



s48_error_t
s48_list (s48_word_t * result,
          s48_arena_t arena,
          ...)
{
  va_list ap;
  s48_error_t err;

  va_start (ap, arena);
  err = s48_vlist (result, arena, ap);
  va_end (ap);

  return err;
}

s48_error_t
s48_vlist (s48_word_t * result,
           s48_arena_t arena,
           va_list ap)
{
  struct s48_vlist_locals
    {
      S48_FRAME;
      s48_word_t tmp;
    } l;

  s48_error_t err;

  S48_PROTECT_FRAME (l);

  err = list_helper (&l.tmp, arena, ap);
  if (!err)
    S48_LSET (result, &l.tmp);

  S48_UNPROTECT_FRAME (l);
  return err;
}

/* This function isn't exported directly because it doesn't behave
 * correctly if `result' aliases with any of the pointers in `ap'.
 * Wrapping it with s48_vlist (see above) ensures this function is
 * safe nonetheless.
 */
static s48_error_t
list_helper (s48_word_t * result,
             s48_arena_t arena,
             va_list ap)
{
  s48_word_t * next;

  next = va_arg (ap, s48_word_t *);
  if (next == s48_va_end)
    {
      return s48_make_nil (result, arena);
    }
  else
    {
      s48_error_t err;

      err = list_helper (result, arena, ap);
      if (err)
        return err;

      return s48_new_cons (result, arena, next, result);
    }
}



long
s48_length(s48_value list)
{
  long i = 0;

  while (!(S48_EQ(list, S48_NULL)))
    {
      list = S48_CDR(list);
      ++i;
    }
  return S48_UNSAFE_ENTER_FIXNUM(i);
}

long
s48_new_length (s48_arena_t arena,
                s48_word_t * lst)
{
  struct s48_new_length_locals
    {
      S48_FRAME;
      s48_word_t fast;
      s48_word_t slow;
    } l;

  long len = 0;

  S48_PROTECT_FRAME (l);

  S48_LSET (&l.fast, lst);
  S48_LSET (&l.slow, lst);

  while (1)
    {
      if (s48_is_nil (arena, &l.fast))
        break;

      ++len;
      s48_cdr (&l.fast, arena, &l.fast);

      if (s48_is_nil (arena, &l.fast))
        break;

      ++len;
      s48_cdr (&l.fast, arena, &l.fast);
      s48_cdr (&l.slow, arena, &l.slow);

      if (s48_values_eq (arena, &l.fast, &l.slow))
        {
          len = -1;
          break;
        }
    }

  S48_UNPROTECT_FRAME (l);
  return len;
}



/* arch-tag: Matthew Dempsky Tue Apr 19 23:09:24 2005 (utils.c)
 */
