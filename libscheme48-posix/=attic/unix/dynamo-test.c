/* No Copyright. */

/*
 * A simple test file for dynamic loading, dynamic name lookup, and
 * old-style external calls.
 */

#include <stdio.h>
#include "scheme48.h"

/*
 * These should only be called on characters or other immediates.
 */

s48_value
s48_dynamo_test(s48_value arg0, s48_value arg1, s48_value arg2)
{
  int i;

  s48_value vector = s48_make_vector(3, S48_FALSE);

  S48_VECTOR_SET(vector, 0, arg0);
  S48_VECTOR_SET(vector, 1, arg1);
  S48_VECTOR_SET(vector, 2, arg2);

  return vector;
}

s48_value
s48_old_dynamo_test(long nargs, s48_value args[])
{
  int i;

  s48_value vector = s48_make_vector(nargs, S48_FALSE);

  for (i = 0; i < nargs; i++)
    S48_VECTOR_SET(vector, i, args[i]);

  return vector;
}

