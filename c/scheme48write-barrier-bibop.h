/* Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifdef __COMPILING_SCHEME48_ITSELF__

/* The VM has only a few occurrences, which we want to inline. */

#include "area_roots.h"

#define S48_WRITE_BARRIER(stob, address, value) \
  s48_write_barrier_inline((stob), (address), (value))

#else

/*
 * For external code, the inlined version may be too hefty.  Use the
 * separate version here.
 */

extern void s48_write_barrier(long stob, char* address, long value);

#define S48_WRITE_BARRIER(stob, address, value) \
  s48_write_barrier((stob), (address), (value))

#endif



