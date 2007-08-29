/* -*-C-*-

$Id: bignum.h,v 9.28 1992/09/18 16:52:34 jinx Exp $

Copyright 1986,1987,1988,1989,1992,2004 Massachusetts Institute of Technology

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

   3. The name of the author may not be used to endorse or promote
      products derived from this software without specific prior
      written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

/* License changed to modified BSD by cph on 2004-11-08.  */

/* External Interface to Bignum Code */

/* The `unsigned long' type is used for the conversion procedures
   `bignum_to_long' and `long_to_bignum'.  Older implementations of C
   don't support this type; if you have such an implementation you can
   disable these procedures using the following flag (alternatively
   you could write alternate versions that don't require this type). */
/* #define BIGNUM_NO_ULONG */

#include "scheme48.h"

typedef void * bignum_type;
#define BIGNUM_OUT_OF_BAND ((bignum_type) 0)

enum bignum_comparison
{
  bignum_comparison_equal = 0,
  bignum_comparison_less = -1,
  bignum_comparison_greater = 1
};

typedef void * bignum_procedure_context;
extern void s48_bignum_make_cached_constants();
extern int s48_bignum_equal_p(bignum_type, bignum_type);
extern enum bignum_comparison s48_bignum_test(bignum_type);
extern enum bignum_comparison s48_bignum_compare(bignum_type, bignum_type);
extern bignum_type s48_bignum_add(bignum_type, bignum_type);
extern bignum_type s48_bignum_subtract(bignum_type, bignum_type);
extern bignum_type s48_bignum_negate(bignum_type);
extern bignum_type s48_bignum_multiply(bignum_type, bignum_type);
extern int s48_bignum_divide(bignum_type numerator, bignum_type denominator,
			     void * quotient, void * remainder);
extern bignum_type s48_bignum_quotient(bignum_type, bignum_type);
extern bignum_type s48_bignum_remainder(bignum_type, bignum_type);
extern bignum_type s48_long_to_bignum(long);
extern bignum_type s48_ulong_to_bignum(unsigned long);
extern long s48_bignum_to_long(bignum_type);
extern unsigned long s48_bignum_to_ulong(bignum_type);
extern bignum_type s48_double_to_bignum(double);
extern double s48_bignum_to_double(bignum_type);
extern int s48_bignum_fits_in_word_p(bignum_type, long word_length,
				     int twos_complement_p);
extern bignum_type s48_bignum_length_in_bits(bignum_type);
extern bignum_type s48_bignum_length_upper_limit(void);
extern bignum_type s48_digit_stream_to_bignum
       (unsigned int n_digits,
	unsigned int (*producer)(bignum_procedure_context),
	bignum_procedure_context context,
	unsigned int radix,
	int negative_p);
extern void s48_bignum_to_digit_stream
	    (bignum_type,
	     unsigned int radix,
	     void (*consumer)(bignum_procedure_context, long),
	     bignum_procedure_context context);
extern long s48_bignum_max_digit_stream_radix(void);

/* Added bitwise operators. */

extern bignum_type s48_bignum_bitwise_not(bignum_type),
                   s48_bignum_arithmetic_shift(bignum_type, long),
                   s48_bignum_bitwise_and(bignum_type, bignum_type),
                   s48_bignum_bitwise_ior(bignum_type, bignum_type),
                   s48_bignum_bitwise_xor(bignum_type, bignum_type);

extern int s48_bignum_oddp(bignum_type);
extern long s48_bignum_bit_count(bignum_type);
