
#include "c/scheme48.h"

long
frog(long arg_count, long *args)
{
  long i, res;

  for (i = 0, res = s48_enter_integer(-100); i < arg_count; res += args[i], i++);

  return res;
}
