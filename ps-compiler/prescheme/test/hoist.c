#include <stdio.h>
#include "prescheme.h"


long foo(long x_0X, long y_1X)
{
  extern long foo(long, long);
  unsigned char arg0K0;
  unsigned char y_2X;
  long v_3X;
  long v_4X;

 {if ((0 == x_0X)) {
    arg0K0 = 10;
    goto L140;}
  else {
    arg0K0 = 4;
    goto L140;}}
 L140: {
  y_2X = arg0K0;
  v_3X = foo((-1 + ((long)y_2X)), 5);
  v_4X = foo((-2 + ((long)y_2X)), 4);
  return((v_4X + v_3X));}}

long test()
{
  extern long foo(long, long);
  long v_5X;

 {v_5X = foo(30, 40);
  return(v_5X);}}

