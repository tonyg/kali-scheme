#include <stdio.h>
#include "prescheme.h"
FILE * Scurrent_input_portS;
long SoneS;

void all()
{
  extern long fact(unsigned char);
  extern long SoneS;
  extern FILE * Scurrent_input_portS;
  unsigned char arg0K0;
  long x_0X;
  unsigned char v_1X;
  unsigned char v_2X;
  long v_3X;

 {x_0X = fact(10);
  if ((100 < x_0X)) {
    arg0K0 = 10;
    goto L213;}
  else {
    v_1X = char_ready_p((Scurrent_input_portS));
    if (v_1X) {
      arg0K0 = 10;
      goto L213;}
    else {
      arg0K0 = 20;
      goto L213;}}}
 L213: {
  v_2X = arg0K0;
  v_3X = fact(v_2X);
  SoneS = v_3X;
  return;}}

long fact(unsigned char n_4X)
{
  extern long SoneS;
  long arg1K1;
  long arg1K0;
  long i_5X;
  long r_6X;

 {arg1K0 = ((long)n_4X);
  arg1K1 = (SoneS);
  goto L153;}
 L153: {
  i_5X = arg1K0;
  r_6X = arg1K1;
  if ((i_5X < (SoneS))) {
    return(r_6X);}
  else {
    arg1K0 = (i_5X - (SoneS));
    arg1K1 = (i_5X * r_6X);
    goto L153;}}}
main()
{
Scurrent_input_portS = stdin;
SoneS = 1;
}
