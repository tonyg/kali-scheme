/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/

#include <stdio.h>

void TTerror(message, count, data1, data2, data3)
  const char *message;
  int count;
  long data1, data2, data3;
{
  switch (count) {
  case 0:
    fprintf(stderr, "\n%s\n", message);
    break;
  case 1:
    fprintf(stderr, "\n%s\n%d\n", message, data1);
    break;
  case 2:
    fprintf(stderr, "\n%s\n%d\n%d\n", message, data1, data2);
    break;
  case 3:
    fprintf(stderr, "\n%s\n%d\n%d\n%d\n", message, data1, data2, data3);
    break;
  }
  exit(-1);
}
