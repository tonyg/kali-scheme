#include <stdio.h>
#include "prescheme.h"
int Scurrent_output_portS;
int Scurrent_input_portS;

long test()
{
  extern int Scurrent_input_portS;
  extern int Scurrent_output_portS;
  long arg0K0;
  unsigned char okayP_0X;
  unsigned char eofP_1X;
  long count_2X;
  long b_3X;
  long count_4X;
  unsigned char okayP_5X;
  long res_6X;

 {b_3X = (long)malloc(16);
  count_2X = ps_read((Scurrent_input_portS), (void *)b_3X, 16, &okayP_0X, &eofP_1X);
  if (okayP_0X) {
    if (eofP_1X) {
      arg0K0 = -1;
      goto L207;}
    else {
      count_4X = ps_write((Scurrent_output_portS), (void *)b_3X, count_2X, &okayP_5X);
      if (okayP_5X) {
        arg0K0 = count_4X;
        goto L207;}
      else {
        arg0K0 = -1;
        goto L207;}}}
  else {
    arg0K0 = -1;
    goto L207;}}
 L207: {
  res_6X = arg0K0;
  free(b_3X);
  return(res_6X);}}
main()
{
Scurrent_output_portS = 1;
Scurrent_input_portS = 0;
test();
}
