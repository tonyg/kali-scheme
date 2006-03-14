/* Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_GC_CHECK_HEAP_H
#define __S48_GC_CHECK_HEAP_H

#include "scheme48.h"

extern char s48_check_heap(long error_count);
extern char s48_stob_in_heapP(s48_value stob);

#endif
