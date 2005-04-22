/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_GC_DATA_H
#define __S48_GC_DATA_H

#include "scheme48.h"

#define S48_UNSIGNED_HIGH_BITS(x, offset, len) ((((unsigned long)(x)) >> offset)\
  & ((1L << len) - 1))

/* selecting/mutating fields of a header directly */

#define S48_HEADER_LENGTH_IN_BYTES(h) ((unsigned long)(h) >> 8)
#define S48_HEADER_TYPE(h) S48_UNSIGNED_HIGH_BITS(h, 2, 5)
#define S48_HEADER_LENGTH_IN_CELLS(h) \
  S48_BYTES_TO_CELLS(S48_HEADER_LENGTH_IN_BYTES(h))
#define S48_HEADER_LENGTH_IN_A_UNITS(h) \
  (S48_BYTES_TO_A_UNITS(S48_HEADER_LENGTH_IN_BYTES(h)))
#define S48_HEADER_IMMUTABLE_P(h) S48_UNSIGNED_HIGH_BITS(h, 7, 1)

#define S48_HEADER_MAKE_IMMUTABLE(h) ((h) |= (1L << 7))
#define S48_HEADER_MAKE_MUTABLE(x) ((h) &= ~(1L << 7))

#define S48_MAKE_HEADER(stobtype, size) ( (size << 8) | (stobtype << 2) | S48_HEADER_TAG )

/* some type predicates */

#define S48_LEAST_B_VECTOR_TYPE S48_STOBTYPE_STRING
#define S48_B_VECTOR_HEADER_P(h) \
  (S48_HEADER_TYPE(h) >= S48_LEAST_B_VECTOR_TYPE)
#define S48_D_VECTOR_HEADER_P(h) \
  (S48_HEADER_TYPE(h) < S48_LEAST_B_VECTOR_TYPE)
#define S48_CONTINUATION_HEADER_P(h) \
  (S48_HEADER_TYPE(h) == S48_STOBTYPE_CONTINUATION)

/* some other stob thing */

#define S48_ADDRESS_TO_STOB_DESCRIPTOR(a) \
  ((s48_value)(((unsigned long)a) | S48_STOB_TAG))

#define S48_STOB_OVERHEAD_IN_CELLS 1
#define S48_STOB_OVERHEAD_IN_BYTES \
  S48_CELLS_TO_BYTES(S48_STOB_OVERHEAD_IN_CELLS)
#define S48_STOB_OVERHEAD_IN_A_UNITS \
  S48_BYTES_TO_A_UNITS(S48_STOB_OVERHEAD_IN_BYTES)
#define S48_ADDRESS_AT_HEADER(stob) \
  ((s48_address)(((unsigned long)S48_ADDRESS_AFTER_HEADER(stob, s48_address))\
                   - S48_STOB_OVERHEAD_IN_A_UNITS))

#endif
