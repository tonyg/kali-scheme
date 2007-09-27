/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include "scheme48.h"
#include "sysdep.h"

void
s48_init_sysexits(void)
{
  /* For SRFI 22 */
  s48_define_exported_binding("EX_SOFTWARE", s48_enter_integer(EX_SOFTWARE));
}
