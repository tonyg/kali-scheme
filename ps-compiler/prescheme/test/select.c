#include <stdio.h>
#include "prescheme.h"
FILE * Scurrent_output_portS;
FILE * Scurrent_input_portS;

void test()
{
  extern FILE * Scurrent_output_portS;
  extern FILE * Scurrent_input_portS;
  long arg0K1;
  long arg0K0;
  long merged_arg0K0;
  long merged_arg0K2;
  FILE * merged_arg1K1;
  FILE * merged_arg1K0;

  int got_char_return_tag;
  int write_number_no_newline_return_tag;
  FILE * out_0X;
  FILE * in_1X;
  fd_set *s2_2X;
  fd_set *s1_3X;
  long i_4X;
  unsigned char v_5X;
  long xWW607_6X;
  long x_7X;
  long x_8X;
  long mask_9X;
  long digit_10X;
  long v_11X;
  long v_12X;
  long x_13X;
  long mask_14X;
  long v_15X;
  unsigned char Kchar_16X;

  FILE * in_17X;
  FILE * out_18X;
  long i_19X;
  long x_20X;
  FILE * port_21X;
 {out_0X = Scurrent_output_portS;
  in_1X = Scurrent_input_portS;
  s2_2X = (fd_set*)malloc(sizeof(fd_set));
  s1_3X = (fd_set*)malloc(sizeof(fd_set));
  arg0K0 = 0;
  goto L310;}
 L310: {
  i_4X = arg0K0;
  v_5X = char_ready_p(in_1X);
  if (v_5X) {
    merged_arg1K0 = in_1X;
    merged_arg1K1 = out_0X;
    merged_arg0K2 = i_4X;
    got_char_return_tag = 0;
    goto got_char;
   got_char_return_0:
    arg0K0 = 0;
    goto L310;}
  else {
    FD_ZERO(s1_3X);
    FD_ZERO(s2_2X);
    FD_SET(fileno(in_1X), s1_3X);
    xWW607_6X = ps_select(s1_3X, s2_2X, 0);
    if ((0 == xWW607_6X)) {
      arg0K0 = (1 + i_4X);
      goto L310;}
    else {
      if ((1 == xWW607_6X)) {
        if ((FD_ISSET(fileno(in_1X), s1_3X))) {
          merged_arg1K0 = in_1X;
          merged_arg1K1 = out_0X;
          merged_arg0K2 = i_4X;
          got_char_return_tag = 1;
          goto got_char;
         got_char_return_1:
          arg0K0 = 0;
          goto L310;}
        else {
          fputs("not in port set", out_0X);
          putc(10, out_0X);
          return;}}
      else {
        fputs("funny port count ", out_0X);
        return;}}}}
 write_number_no_newline: {
  x_20X = merged_arg0K0;
  port_21X = merged_arg1K1;{
  if ((x_20X < 0)) {
    putc(45, port_21X);
    arg0K0 = (0 - x_20X);
    goto L200;}
  else {
    arg0K0 = x_20X;
    goto L200;}}
 L237: {
  x_13X = arg0K0;
  mask_14X = arg0K1;
  if ((x_13X < 10)) {
    arg0K0 = x_7X;
    arg0K1 = mask_14X;
    goto L202;}
  else {
    v_15X = x_13X / 10;
    arg0K0 = v_15X;
    arg0K1 = (10 * mask_14X);
    goto L237;}}
 L202: {
  x_8X = arg0K0;
  mask_9X = arg0K1;
  digit_10X = x_8X / mask_9X;
  putc(((48 + digit_10X)), port_21X);
  if ((1 < mask_9X)) {
    v_11X = x_8X % mask_9X;
    v_12X = mask_9X / 10;
    arg0K0 = v_11X;
    arg0K1 = v_12X;
    goto L202;}
  else {
    goto write_number_no_newline_return;}}
 L200: {
  x_7X = arg0K0;
  arg0K0 = x_7X;
  arg0K1 = 1;
  goto L237;}
 write_number_no_newline_return:
  switch (write_number_no_newline_return_tag) {
  case 0: goto write_number_no_newline_return_0;
  case 1: goto write_number_no_newline_return_1;
  }}

 got_char: {
  in_17X = merged_arg1K0;
  out_18X = merged_arg1K1;
  i_19X = merged_arg0K2;{
  fputs("Got ", out_18X);
  { int TTchar;
    PS_GETC(in_17X, TTchar);
    if (EOF == TTchar) {
      fputs("EOF!", out_18X);
      goto L280;}
    else {
      unsigned char Kchar_16X = TTchar;
      merged_arg0K0 = ((long)(((long)Kchar_16X)));
      merged_arg1K1 = out_18X;
      write_number_no_newline_return_tag = 0;
      goto write_number_no_newline;
     write_number_no_newline_return_0:
      goto L280;}}}
 L280: {
  fputs(" after ", out_18X);
  merged_arg0K0 = i_19X;
  merged_arg1K1 = out_18X;
  write_number_no_newline_return_tag = 1;
  goto write_number_no_newline;
 write_number_no_newline_return_1:
  putc(10, out_18X);
  goto got_char_return;}
 got_char_return:
  switch (got_char_return_tag) {
  case 0: goto got_char_return_0;
  case 1: goto got_char_return_1;
  }}
}
main()
{
Scurrent_output_portS = stdout;
Scurrent_input_portS = stdin;
}
