#include <stdio.h>
#include "prescheme.h"

long fact(unsigned char);
void all(void);
long SoneS;

long fact(unsigned char n_0X)
{
  long arg0K1;
  long arg0K0;
  long i_1X;
  long r_2X;
 {arg0K0 = (((unsigned long) n_0X));
  arg0K1 = (SoneS);
  goto L37;}
 L37: {
  i_1X = arg0K0;
  r_2X = arg0K1;
  if ((i_1X < (SoneS))) {
    return r_2X;}
  else {
    arg0K0 = (i_1X - (SoneS));
    arg0K1 = (i_1X * r_2X);
    goto L37;}}}

void all(void)
{
  unsigned char arg1K0;
  long x_3X;
  unsigned char v_4X;
  long v_5X;
 {x_3X = fact(10);
  if ((100 < x_3X)) {
    arg1K0 = 10;
    goto L81;}
  else {
    arg1K0 = 20;
    goto L81;}}
 L81: {
  v_4X = arg1K0;
  v_5X = fact(v_4X);
  SoneS = v_5X;
  return;}}
void
main(void)
{
SoneS = 1;
}
