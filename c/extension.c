/* Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/* Implementation of the vm-extension opcode.  This is completely
   optional; nothing in the standard system uses these features.

   The vm-extension opcode is being phased out.  New code should use the
   external-call opcode to call C procedures.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <errno.h>

#include "scheme48.h"

#define GREATEST_FIXNUM_VALUE ((1 << 29) - 1)
#define LEAST_FIXNUM_VALUE (-1 << 29)
#define CHANNEL_INDEX(x) EXTRACT_FIXNUM(STOB_REF(x, 1))
#define FOR_INPUT 1
#define FOR_OUTPUT 2

typedef struct {
  char b[sizeof(double)];
} unaligned_double;

typedef union {
  double f;
  unaligned_double b;
} float_or_bytes;

extern long s48_Sextension_valueS;  /* how values are returned */

/* return status values */
#define EXT_ST_OKAY 0
#define EXT_ST_EXCEPTION 1

#define EXT_RETURN(value) {s48_Sextension_valueS = (value); return EXT_ST_OKAY; }
#define EXT_EXCEPTION     return EXT_ST_EXCEPTION

/******************************************/

s48_value
s48_extended_vm (long key, s48_value value)
{
  double x, y;

  switch (key) {

    /* Cases 0 through 19 are reserved for the mobot system. */

  case 0:			/* read jumpers on 68000 board */
    EXT_RETURN(S48_UNSAFE_ENTER_FIXNUM(0));

  /* Floating point */

#define FLOP 100
#define FLOP2(i) case FLOP+(i): \
		   if (!S48_STOB_P(value) || S48_STOB_DESCRIPTOR_LENGTH(value) != 2) \
		     EXT_EXCEPTION;
#define FLOP3(i) case FLOP+(i): \
		   if (!S48_STOB_P(value) || S48_STOB_DESCRIPTOR_LENGTH(value) != 3) \
		     EXT_EXCEPTION;

#define get_arg(args,i) S48_STOB_REF(args,(i))
#define get_string_arg(args,i) (S48_UNSAFE_EXTRACT_STRING(get_arg(args,i)))

#define get_float_arg(args, i, var) EXTRACT_FLOAT(get_arg(args, i), var)
#define set_float_arg(args, i, val) SET_FLOAT(get_arg(args, i), val)

#define EXTRACT_FLOAT(stob, var) \
  { s48_value temp_ = (stob); \
    float_or_bytes loser_; \
    if (!S48_STOB_P(temp_)) EXT_EXCEPTION; \
    loser_.b = *(unaligned_double*)(&S48_STOB_REF(temp_, 0)); \
    (var) = loser_.f; }

#define SET_FLOAT(stob, val) \
  { s48_value temp_ = (stob); \
    float_or_bytes loser_; \
    if (!S48_STOB_P(temp_)) EXT_EXCEPTION; \
    loser_.f = (double)(val); \
    *(unaligned_double*)(&S48_STOB_REF(temp_, 0)) = loser_.b; }

  FLOP3(0) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x + y);
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP3(1) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x - y);
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP3(2) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    set_float_arg(value, 2, x * y);
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP3(3) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (y == 0.0) EXT_EXCEPTION;
    set_float_arg(value, 2, x / y);
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(4) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    EXT_RETURN(S48_ENTER_BOOLEAN(x == y));}
  FLOP2(5) {
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    EXT_RETURN(S48_ENTER_BOOLEAN(x < y));}
  FLOP2(6) {			/* fixnum->float */
    s48_value arg = get_arg(value, 0);
    if (!S48_FIXNUM_P(arg)) EXT_RETURN(S48_FALSE);
    set_float_arg(value, 1, S48_UNSAFE_EXTRACT_FIXNUM(arg));
    EXT_RETURN(S48_TRUE);}
  FLOP2(7) {			/* string->float */
    char *str = get_string_arg(value, 0);
    set_float_arg(value, 1, atof(str));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(8) {			/* float->string */
    size_t len;
    char *str = get_string_arg(value,1);
    get_float_arg(value, 0, x);
    sprintf(str, "%g", x);
    len = strlen(str);
    if (len > S48_UNSAFE_STRING_LENGTH(get_arg(value,1)))
      /* unlikely but catastrophic */
      fprintf(stderr, "printing float: output too long: %s\n",
	      str);
    EXT_RETURN(S48_UNSAFE_ENTER_FIXNUM(len));}

    /* exp log sin cos tan asin acos atan1 atan2 sqrt */

  FLOP2(9) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, exp(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(10) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, log(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(11) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, sin(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(12) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, cos(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(13) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, tan(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(14) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, asin(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(15) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, acos(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(16) {			/* atan 1 */
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, atan(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP3(17) {			/* atan 2 */
    get_float_arg(value, 0, y);
    get_float_arg(value, 1, x);
    set_float_arg(value, 2, atan2(y, x));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP2(18) {
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, sqrt(x));
    EXT_RETURN(S48_UNSPECIFIC);}

  FLOP2(19) {			/* floor */
    get_float_arg(value, 0, x);
    set_float_arg(value, 1, floor(x));
    EXT_RETURN(S48_UNSPECIFIC);}
  case FLOP+20: {		/* integer? */
    EXTRACT_FLOAT(value, x);
    EXT_RETURN(S48_ENTER_BOOLEAN(fmod(x, 1.0) == 0.0)); }
  case FLOP+21: {		/* float->fixnum */
    EXTRACT_FLOAT(value, x);
    if (x <= (double)GREATEST_FIXNUM_VALUE
	&& x >= (double)LEAST_FIXNUM_VALUE)
      {
      EXT_RETURN(S48_UNSAFE_ENTER_FIXNUM((long)x)); }
    else
      EXT_RETURN(S48_FALSE);}
  FLOP3(22) {			/* quotient */
    double z;
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (fmod(x, 1.0) != 0.0 || fmod(y, 1.0) != 0.0) EXT_EXCEPTION;
    if (y == 0.0) EXT_EXCEPTION;
    z = x / y;
    set_float_arg(value, 2, z < 0.0 ? ceil(z) : floor(z));
    EXT_RETURN(S48_UNSPECIFIC);}
  FLOP3(23) {			/* remainder */
    get_float_arg(value, 0, x);
    get_float_arg(value, 1, y);
    if (fmod(x, 1.0) != 0.0 || fmod(y, 1.0) != 0.0) EXT_EXCEPTION;
    if (y == 0.0) EXT_EXCEPTION;

    /* "fmod(double x, double y) returns the floating-point remainder
       (f) of the division of x by y, where f has the same sign as x,
       such that x=iy+f for some integer i, and |f| < |y|." */

    set_float_arg(value, 2, fmod(x, y));
    EXT_RETURN(S48_UNSPECIFIC);}

  default:
    EXT_EXCEPTION;
  }
}

