/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/

#include <stdio.h>

void Qps_error_0(message)
  const char *message;
{
  fprintf(stderr, "\n%s\n", message);
  exit(-1);
}

void Qps_error_1(message, data1)
  const char *message;
  long data1;
{
  fprintf(stderr, "\n%s\n%d\n", message, data1);
  exit(-1);
}

void Qps_error_2(message, data1, data2)
  const char *message;
  long data1, data2;
{
  fprintf(stderr, "\n%s\n%d\n%d\n", message, data1, data2);
  exit(-1);
}

