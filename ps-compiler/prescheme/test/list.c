#include <stdio.h>
#include "prescheme.h"
unsigned char Speeked_charS;
unsigned char Speeked_charPS;
FILE * Sinput_portS;
long ShpS;
long SmemoryS;
FILE * Scurrent_output_portS;
FILE * Scurrent_input_portS;

unsigned char read_r_paren()
{
  extern unsigned char read_r_paren();
  extern FILE * Sinput_portS;
  extern unsigned char Speeked_charPS;
  extern unsigned char Speeked_charS;
  unsigned char arg0K0;
  unsigned char Kchar_0X;
  unsigned char xWW598_1X;
  unsigned char v_2X;

 {if ((Speeked_charPS)) {
    arg0K0 = (Speeked_charS);
    goto L766;}
  else {
    { int TTchar;
      PS_GETC((Sinput_portS), TTchar);
      if (EOF == TTchar) {
        arg0K0 = 0;
        goto L766;}
      else {
        unsigned char Kchar_0X = TTchar;
        Speeked_charPS = 1;
        Speeked_charS = Kchar_0X;
        arg0K0 = Kchar_0X;
        goto L766;}}}}
 L766: {
  xWW598_1X = arg0K0;
  if ((32 == ((long)xWW598_1X))) {
    goto L776;}
  else {
    if ((10 == ((long)xWW598_1X))) {
      goto L776;}
    else {
      if ((41 == ((long)xWW598_1X))) {
        return(1);}
      else {
        return(0);}}}}
 L776: {
  if ((Speeked_charPS)) {
    Speeked_charPS = 0;
    goto L778;}
  else {
    { int TTchar;
      PS_GETC((Sinput_portS), TTchar);
      if (EOF == TTchar) {
        goto L778;}
      else {
        unsigned char Kchar_3X = TTchar;
        goto L778;}}}}
 L778: {
  v_2X = read_r_paren();
  return(v_2X);}}

long read_list()
{
  extern long read_list();
  extern long make_pair(long, long);
  extern long read_s_exp();
  extern unsigned char read_r_paren();
  extern FILE * Sinput_portS;
  extern unsigned char Speeked_charPS;
  extern unsigned char Speeked_charS;
  unsigned char arg0K0;
  unsigned char Kchar_4X;
  unsigned char xWW596_5X;
  long head_6X;
  long v_7X;
  long v_8X;
  unsigned char v_9X;
  long res_10X;
  long v_11X;

 {if ((Speeked_charPS)) {
    arg0K0 = (Speeked_charS);
    goto L840;}
  else {
    { int TTchar;
      PS_GETC((Sinput_portS), TTchar);
      if (EOF == TTchar) {
        arg0K0 = 0;
        goto L840;}
      else {
        unsigned char Kchar_4X = TTchar;
        Speeked_charPS = 1;
        Speeked_charS = Kchar_4X;
        arg0K0 = Kchar_4X;
        goto L840;}}}}
 L840: {
  xWW596_5X = arg0K0;
  if ((32 == ((long)xWW596_5X))) {
    goto L850;}
  else {
    if ((10 == ((long)xWW596_5X))) {
      goto L850;}
    else {
      if ((41 == ((long)xWW596_5X))) {
        return(1);}
      else {
        if ((46 == ((long)xWW596_5X))) {
          if ((Speeked_charPS)) {
            Speeked_charPS = 0;
            goto L864;}
          else {
            { int TTchar;
              PS_GETC((Sinput_portS), TTchar);
              if (EOF == TTchar) {
                goto L864;}
              else {
                unsigned char Kchar_12X = TTchar;
                goto L864;}}}}
        else {
          head_6X = read_s_exp();
          v_7X = read_list();
          v_8X = make_pair(head_6X, v_7X);
          return(v_8X);}}}}}
 L850: {
  if ((Speeked_charPS)) {
    Speeked_charPS = 0;
    goto L852;}
  else {
    { int TTchar;
      PS_GETC((Sinput_portS), TTchar);
      if (EOF == TTchar) {
        goto L852;}
      else {
        unsigned char Kchar_13X = TTchar;
        goto L852;}}}}
 L864: {
  res_10X = read_s_exp();
  v_9X = read_r_paren();
  if (v_9X) {
    return(res_10X);}
  else {
    return(-1);}}
 L852: {
  v_11X = read_list();
  return(v_11X);}}

long read_s_exp()
{
  extern long read_s_exp();
  extern long read_list();
  extern FILE * Sinput_portS;
  extern unsigned char Speeked_charPS;
  extern unsigned char Speeked_charS;
  long arg1K0;
  unsigned char arg0K0;
  unsigned char merged_arg0K0;

  int digitP_return_tag;
  unsigned char digitP_return_value;
  int really_read_number_return_tag;
  long really_read_number_return_value;
  int read_number_return_tag;
  long read_number_return_value;
  unsigned char Kchar_14X;
  unsigned char xWW593_15X;
  unsigned char Kchar_16X;
  long x_17X;
  unsigned char v_18X;
  unsigned char v_19X;
  long x_20X;
  long v_21X;
  long v_22X;
  unsigned char Kchar_23X;
  long v_24X;
  long v_25X;
  unsigned char xWW592_26X;
  long v_27X;
  long r_28X;
  unsigned char Kchar_29X;
  unsigned char ch_30X;
  unsigned char v_31X;
  unsigned char ch_32X;
  unsigned char c_33X;
  unsigned char ch_34X;
  long c_35X;
  long c_36X;
 {if ((Speeked_charPS)) {
    arg0K0 = (Speeked_charS);
    goto L955;}
  else {
    { int TTchar;
      PS_GETC((Sinput_portS), TTchar);
      if (EOF == TTchar) {
        arg0K0 = 0;
        goto L955;}
      else {
        unsigned char Kchar_14X = TTchar;
        Speeked_charPS = 1;
        Speeked_charS = Kchar_14X;
        arg0K0 = Kchar_14X;
        goto L955;}}}}
 L955: {
  xWW593_15X = arg0K0;
  if ((32 == ((long)xWW593_15X))) {
    goto L965;}
  else {
    if ((10 == ((long)xWW593_15X))) {
      goto L965;}
    else {
      if ((45 == ((long)xWW593_15X))) {
        goto L980;}
      else {
        if ((43 == ((long)xWW593_15X))) {
          goto L980;}
        else {
          if ((40 == ((long)xWW593_15X))) {
            if ((Speeked_charPS)) {
              Speeked_charPS = 0;
              goto L990;}
            else {
              { int TTchar;
                PS_GETC((Sinput_portS), TTchar);
                if (EOF == TTchar) {
                  goto L990;}
                else {
                  unsigned char Kchar_37X = TTchar;
                  goto L990;}}}}
          else {
            if ((Speeked_charPS)) {
              arg0K0 = (Speeked_charS);
              goto L995;}
            else {
              { int TTchar;
                PS_GETC((Sinput_portS), TTchar);
                if (EOF == TTchar) {
                  arg0K0 = 0;
                  goto L995;}
                else {
                  unsigned char Kchar_16X = TTchar;
                  Speeked_charPS = 1;
                  Speeked_charS = Kchar_16X;
                  arg0K0 = Kchar_16X;
                  goto L995;}}}}}}}}}
 L965: {
  if ((Speeked_charPS)) {
    Speeked_charPS = 0;
    goto L967;}
  else {
    { int TTchar;
      PS_GETC((Sinput_portS), TTchar);
      if (EOF == TTchar) {
        goto L967;}
      else {
        unsigned char Kchar_38X = TTchar;
        goto L967;}}}}
 L980: {
  read_number_return_tag = 0;
  goto read_number;
 read_number_return_0:
  x_17X = read_number_return_value;
  return((((x_17X)<<1)));}
 L995: {
  v_18X = arg0K0;
  merged_arg0K0 = v_18X;
  digitP_return_tag = 0;
  goto digitP;
 digitP_return_0:
  v_19X = digitP_return_value;
  if (v_19X) {
    read_number_return_tag = 1;
    goto read_number;
   read_number_return_1:
    x_20X = read_number_return_value;
    return((((x_20X)<<1)));}
  else {
    return(-1);}}
 L990: {
  v_21X = read_list();
  return(v_21X);}
 L967: {
  v_22X = read_s_exp();
  return(v_22X);}
 read_number: {
{ if ((Speeked_charPS)) {
    arg0K0 = (Speeked_charS);
    goto L689;}
  else {
    { int TTchar;
      PS_GETC((Sinput_portS), TTchar);
      if (EOF == TTchar) {
        arg0K0 = 0;
        goto L689;}
      else {
        unsigned char Kchar_23X = TTchar;
        Speeked_charPS = 1;
        Speeked_charS = Kchar_23X;
        arg0K0 = Kchar_23X;
        goto L689;}}}}
 L689: {
  xWW592_26X = arg0K0;
  if ((45 == ((long)xWW592_26X))) {
    if ((Speeked_charPS)) {
      Speeked_charPS = 0;
      goto L694;}
    else {
      { int TTchar;
        PS_GETC((Sinput_portS), TTchar);
        if (EOF == TTchar) {
          goto L694;}
        else {
          unsigned char Kchar_39X = TTchar;
          goto L694;}}}}
  else {
    if ((43 == ((long)xWW592_26X))) {
      if ((Speeked_charPS)) {
        Speeked_charPS = 0;
        goto L702;}
      else {
        { int TTchar;
          PS_GETC((Sinput_portS), TTchar);
          if (EOF == TTchar) {
            goto L702;}
          else {
            unsigned char Kchar_40X = TTchar;
            goto L702;}}}}
    else {
      really_read_number_return_tag = 0;
      goto really_read_number;
     really_read_number_return_0:
      v_27X = really_read_number_return_value;
      read_number_return_value = v_27X;
      goto read_number_return;}}}
 L702: {
  really_read_number_return_tag = 1;
  goto really_read_number;
 really_read_number_return_1:
  v_25X = really_read_number_return_value;
  read_number_return_value = v_25X;
  goto read_number_return;}
 L694: {
  really_read_number_return_tag = 2;
  goto really_read_number;
 really_read_number_return_2:
  v_24X = really_read_number_return_value;
  read_number_return_value = (0 - v_24X);
  goto read_number_return;}
 read_number_return:
  switch (read_number_return_tag) {
  case 0: goto read_number_return_0;
  case 1: goto read_number_return_1;
  }}

 really_read_number: {
{ arg1K0 = 0;
  goto L630;}
 L633: {
  ch_30X = arg0K0;
  merged_arg0K0 = ch_30X;
  digitP_return_tag = 1;
  goto digitP;
 digitP_return_1:
  v_31X = digitP_return_value;
  if (v_31X) {
    if ((Speeked_charPS)) {
      Speeked_charPS = 0;
      goto L640;}
    else {
      { int TTchar;
        PS_GETC((Sinput_portS), TTchar);
        if (EOF == TTchar) {
          goto L640;}
        else {
          unsigned char Kchar_41X = TTchar;
          goto L640;}}}}
  else {
    really_read_number_return_value = r_28X;
    goto really_read_number_return;}}
 L640: {
  arg1K0 = ((-48 + ((long)(ch_30X))) + (10 * r_28X));
  goto L630;}
 L630: {
  r_28X = arg1K0;
  if ((Speeked_charPS)) {
    arg0K0 = (Speeked_charS);
    goto L633;}
  else {
    { int TTchar;
      PS_GETC((Sinput_portS), TTchar);
      if (EOF == TTchar) {
        arg0K0 = 0;
        goto L633;}
      else {
        unsigned char Kchar_29X = TTchar;
        Speeked_charPS = 1;
        Speeked_charS = Kchar_29X;
        arg0K0 = Kchar_29X;
        goto L633;}}}}
 really_read_number_return:
  switch (really_read_number_return_tag) {
  case 0: goto really_read_number_return_0;
  case 1: goto really_read_number_return_1;
  case 2: goto really_read_number_return_2;
  }}

 digitP: {
  ch_34X = merged_arg0K0;{
  ch_32X = ch_34X;
  if ((((long)ch_32X) < 48)) {
    digitP_return_value = 0;
    goto digitP_return;}
  else {
    if ((57 < ((long)ch_32X))) {
      digitP_return_value = 0;
      goto digitP_return;}
    else {
      digitP_return_value = 1;
      goto digitP_return;}}}
 digitP_return:
  switch (digitP_return_tag) {
  case 0: goto digitP_return_0;
  case 1: goto digitP_return_1;
  }}
}

void print_s_exp(long x_42X, FILE * out_43X)
{
  extern void print_s_exp(long, FILE *);
  long arg1K1;
  long arg1K0;
  long x_44X;
  long x_45X;
  long mask_46X;
  long v_47X;
  long x_48X;
  long mask_49X;
  long digit_50X;
  long v_51X;
  long v_52X;
  long x_53X;
  long x_54X;

 {if ((0 == (1 & x_42X))) {
    x_44X = ((x_42X)>>1);
    if ((x_44X < 0)) {
      putc(45, out_43X);
      arg1K0 = (0 - x_44X);
      goto L501;}
    else {
      arg1K0 = x_44X;
      goto L501;}}
  else {
    if ((1 == x_42X)) {
      putc(40, out_43X);
      putc(41, out_43X);
      return;}
    else {
      if ((1 == (1 & x_42X))) {
        putc(40, out_43X);
        print_s_exp((*((long *) (-1 + x_42X))), out_43X);
        arg1K0 = (*((long *) (3 + x_42X)));
        goto L1141;}
      else {
        return;}}}}
 L538: {
  x_45X = arg1K0;
  mask_46X = arg1K1;
  if ((x_45X < 10)) {
    arg1K0 = x_53X;
    arg1K1 = mask_46X;
    goto L503;}
  else {
    v_47X = x_45X / 10;
    arg1K0 = v_47X;
    arg1K1 = (10 * mask_46X);
    goto L538;}}
 L503: {
  x_48X = arg1K0;
  mask_49X = arg1K1;
  digit_50X = x_48X / mask_49X;
  putc(((48 + digit_50X)), out_43X);
  if ((1 < mask_49X)) {
    v_51X = x_48X % mask_49X;
    v_52X = mask_49X / 10;
    arg1K0 = v_51X;
    arg1K1 = v_52X;
    goto L503;}
  else {
    return;}}
 L501: {
  x_53X = arg1K0;
  arg1K0 = x_53X;
  arg1K1 = 1;
  goto L538;}
 L1141: {
  x_54X = arg1K0;
  if ((1 == x_54X)) {
    putc(41, out_43X);
    return;}
  else {
    if ((1 == (1 & x_54X))) {
      putc(32, out_43X);
      print_s_exp((*((long *) (-1 + x_54X))), out_43X);
      arg1K0 = (*((long *) (3 + x_54X)));
      goto L1141;}
    else {
      putc(32, out_43X);
      putc(46, out_43X);
      putc(32, out_43X);
      print_s_exp(x_54X, out_43X);
      putc(41, out_43X);
      return;}}}}

long make_pair(long x_55X, long y_56X)
{
  extern long ShpS;
  long p_57X;
  long p_58X;

 {p_57X = ShpS;
  ShpS = (16 + (ShpS));
  p_58X = 1 + p_57X;
  *((long *) (-1 + p_58X)) = x_55X;
  *((long *) (3 + p_58X)) = y_56X;
  return(p_58X);}}

void test(long size_59X)
{
  extern long SmemoryS;
  extern long ShpS;
  extern long make_pair(long, long);
  extern FILE * Scurrent_output_portS;
  extern FILE * Scurrent_input_portS;
  extern FILE * Sinput_portS;
  extern long read_s_exp();
  extern void print_s_exp(long, FILE *);
  long v_60X;
  long v_61X;
  long v_62X;
  long s_exp_63X;
  FILE * out_64X;
  long v_65X;

 {SmemoryS = ((long)malloc(size_59X));
  ShpS = (SmemoryS);
  v_60X = make_pair(6, 8);
  v_61X = make_pair(v_60X, 1);
  v_62X = make_pair(4, v_61X);
  s_exp_63X = make_pair(2, v_62X);
  out_64X = Scurrent_output_portS;
  print_s_exp(s_exp_63X, out_64X);
  putc(10, out_64X);
  Sinput_portS = (Scurrent_input_portS);
  v_65X = read_s_exp();
  print_s_exp(v_65X, out_64X);
  putc(10, out_64X);
  return;}}
main()
{
Speeked_charPS = 0;
Scurrent_output_portS = stdout;
Scurrent_input_portS = stdin;
test(10000);
}
