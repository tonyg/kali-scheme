/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/

/* Changes for mobot system:
     Short multiply instruction
     Poll for alarm interrupts
     Smaller stack size
     static struct {...} -> const static struct {...}
     char Qps_error_0(char *) -> void Qps_error_0(const char *)
     void Qps_error_1(char*, long) -> void Qps_error_1(const char*, long)
     long Qps_error_2(char*, long, long) ->
         void Qps_error_2(const char*, long, long)
     char* -> const char* in declarations of write_string
     fix uses of (void) return value of Qps_error_0
     added const to types of merged_arg4K0 and string_850X
   -JAR
 */

#ifdef ROBOT
#include <vubugg.h>		/* define _jtime ... */
extern unsigned long alarm_time;
#define ALARM_CHECK \
    if (_jtime >= alarm_time) { \
        Spending_interruptsS |= 1; \
	alarm_time = 0xffffffff; \
      }
#define ps_run_time() _jtime
#define ps_real_time() _jtime
#define char_ready_p listen
#else
#define ALARM_CHECK
#endif

#ifdef ROBOT
#define small_multiply(x,y) \
    ({ long temp_ = (y); \
       asm volatile ("mulu %2,%0" : "=d" (temp_) : \
		     "0" (temp_), "g" ((short)(x))); \
       temp_; })
#else
#define small_multiply(x,y) ((x) * (y))
#endif



#include <stdio.h>
#include "prescheme.h"
char goto_arg1K0;
long goto_arg0K4;
long goto_arg0K3;
long goto_arg0K2;
long goto_arg0K1;
long goto_arg0K0;
FILE * Scurrent_input_portS;
FILE * Scurrent_output_portS;
unsigned char *SmemoryS;
long Smemory_beginS;
long Smemory_endS;
long ShpS;
long SlimitS;
FILE * *Sopen_portsS;
long *Sopen_vm_portsS;
long Snewspace_beginS;
long Snewspace_endS;
long Soldspace_beginS;
long Soldspace_endS;
long Sstack_beginS;
long Sstack_endS;
long Sstack_sizeS;
long Sinitial_heap_sizeS;
long Sgc_countS;
long Ssaved_hpS;
long Ssaved_limitS;
long Sweak_pointer_hpS;
long Sweak_pointer_limitS;
long SstackS;
long Sstack_limitS;
long Sbottom_of_stackS;
unsigned char Sexception_space_usedPS;
long StemplateS;
long SenvS;
long ScontS;
long Scode_pointerS;
long SnargsS;
long SvalS;
long Senabled_interruptsS;
long Sexception_handlerS;
long Sinterrupt_handlersS;
long Sdynamic_stateS;
long Spending_interruptsS;
long Sinterrupt_templateS;
long Sminimum_recovered_spaceS;

void write_string(const char *str_0X, FILE * port_1X)
{
  long arg0K0;

 {arg0K0 = 0;
  goto L5207;
 L5207: {
  long i_2X = arg0K0;
  char ch_3X;
  ch_3X = *((str_0X + i_2X));
  if ((0 == ((long)ch_3X))) {
    return;}
  else {
    putc(ch_3X, port_1X);
    arg0K0 = (1 + i_2X);
    goto L5207;}}}}

long copy_stob(long stob_4X, char key_5X)
{
  extern long ShpS;
  long arg0K1;
  long arg0K0;

 {long len_6X;
  long h_7X;
  long new_8X;
  len_6X = (*((long*)((unsigned char*)(-7 + stob_4X)))) >> 8;
  h_7X = *((long*)((unsigned char*)(-7 + stob_4X)));
  *((long*)((unsigned char*)ShpS)) = (2 + (((len_6X << 6) + (31 & (h_7X >> 2))) << 2));
  ShpS = (4 + ShpS);
  new_8X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_6X)));
  if ((0 < len_6X)) {
    *((long*)((unsigned char*)(-4 + ShpS))) = 0;
    arg0K0 = new_8X;
    goto L11724;}
  else {
    arg0K0 = new_8X;
    goto L11724;}
 L11724: {
  long new_9X = arg0K0;
  long bytes_10X;
  long from_11X;
  bytes_10X = (*((long*)((unsigned char*)(-7 + stob_4X)))) >> 8;
  from_11X = -3 + stob_4X;
  arg0K0 = from_11X;
  arg0K1 = (-3 + new_9X);
  goto L11739;
 L11739: {
  long from_12X = arg0K0;
  long to_13X = arg0K1;
  if ((from_12X < (from_11X + (-4 & (3 + bytes_10X))))) {
    *((long*)((unsigned char*)to_13X)) = (*((long*)((unsigned char*)from_12X)));
    arg0K0 = (4 + from_12X);
    arg0K1 = (4 + to_13X);
    goto L11739;}
  else {
    return(new_9X);}}}}}

void close_port(long vm_port_14X)
{
  extern FILE * *Sopen_portsS;
  extern long *Sopen_vm_portsS;
  extern void Qps_error_0(const char*);
  static const struct{long length; char contents[42];} XX15
     = {41, "this shouldn't happen when closing a port"};

 {if (((*((long*)((unsigned char*)(1 + vm_port_14X)))) < 0)) {
    return;}
  else {
    long index_16X;
    index_16X = (*((long*)((unsigned char*)(1 + vm_port_14X)))) >> 2;
    if ((0 == index_16X)) {
      return;}
    else {
      if ((1 == index_16X)) {
        return;}
      else {
        long mode_17X;
        FILE * port_18X;
        mode_17X = (*((long*)((unsigned char*)(-3 + vm_port_14X)))) >> 2;
        port_18X = *((Sopen_portsS + ((*((long*)((unsigned char*)(1 + vm_port_14X)))) >> 2)));
        if ((1 == mode_17X)) {
          fclose(port_18X);
          goto L12977;}
        else {
          if ((2 == mode_17X)) {
            fclose(port_18X);
            goto L12977;}
          else {
            Qps_error_0((XX15.contents));
            goto L12977;}}
       L12977: {
        *((long*)((unsigned char*)(-3 + vm_port_14X))) = 0;
        *((long*)((unsigned char*)(1 + vm_port_14X))) = -4;
        *((Sopen_vm_portsS + index_16X)) = 1;
        return;}}}}}}

void close_port_noisily(long port_19X)
{
  extern void close_port(long);
  extern void write_string(const char*, FILE *);
  extern FILE * Scurrent_output_portS;
  extern void Qps_error_0(const char*);
  static const struct{long length; char contents[14];} XX20
     = {13, "Port closed: "};
  static const struct{long length; char contents[38];} XX21
     = {37, "block write did not send enough bytes"};

 {long string_22X;
  FILE * port_23X;
  long count_24X;
  long sent_25X;
  close_port(port_19X);
  write_string((XX20.contents), Scurrent_output_portS);
  string_22X = *((long*)((unsigned char*)(9 + port_19X)));
  port_23X = Scurrent_output_portS;
  count_24X = (*((long*)((unsigned char*)(-7 + string_22X)))) >> 8;
  sent_25X = fwrite((void *)((unsigned char*)(-3 + string_22X)), sizeof(char), count_24X, port_23X);
  if ((sent_25X == count_24X)) {
    goto L14239;}
  else {
    Qps_error_0((XX21.contents));
    goto L14239;}
 L14239: {
  FILE * port_26X;
  port_26X = Scurrent_output_portS;
  putc(10, port_26X);
  if ((port_26X == Scurrent_output_portS)) {
    fflush(port_26X);
    return;}
  else {
    return;}}}}

void scan(long start_27X, long end_28X)
{
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern long ShpS;
  extern long copy_weak_pointer(long);
  long arg0K0;
  long RShpS = ShpS;

 {arg0K0 = start_27X;
  goto L15120;
 L15120: {
  long addr_29X = arg0K0;
  if ((addr_29X < end_28X)) {
    long next_30X;
    long thing_31X;
    next_30X = 4 + addr_29X;
    thing_31X = *((long*)((unsigned char*)addr_29X));
    if ((2 == (3 & thing_31X))) {
      if (((31 & (thing_31X >> 2)) < 15)) {
        goto L34627;}
      else {
        arg0K0 = (next_30X + (-4 & (3 + (thing_31X >> 8))));
        goto L15120;}}
    else {
      goto L34627;}
   L34627: {
    if ((3 == (3 & thing_31X))) {
      long a_32X;
      a_32X = -3 + thing_31X;
      if ((a_32X < Soldspace_beginS)) {
        arg0K0 = next_30X;
        goto L15120;}
      else {
        unsigned char v_33X;
        v_33X = a_32X < Soldspace_endS;
        if (v_33X) {
          long h_34X;
          h_34X = *((long*)((unsigned char*)(-7 + thing_31X)));
          if ((3 == (3 & h_34X))) {
            arg0K0 = h_34X;
            goto L34663;}
          else {
            if ((1070 == h_34X)) {
              long v_35X;
              ShpS = RShpS;
              v_35X = copy_weak_pointer(thing_31X);
              RShpS = ShpS;
              arg0K0 = v_35X;
              goto L34663;}
            else {
              long new_36X;
              long x_37X;
              *((long*)((unsigned char*)RShpS)) = h_34X;
              RShpS = (4 + RShpS);
              new_36X = 3 + RShpS;
              *((long*)((unsigned char*)(-7 + thing_31X))) = new_36X;
              x_37X = RShpS;
              arg0K0 = (-3 + thing_31X);
              goto L34657;
             L34657: {
              long o_38X = arg0K0;
              if ((RShpS < (x_37X + (-4 & (3 + (h_34X >> 8)))))) {
                *((long*)((unsigned char*)RShpS)) = (*((long*)((unsigned char*)o_38X)));
                RShpS = (4 + RShpS);
                arg0K0 = (4 + o_38X);
                goto L34657;}
              else {
                arg0K0 = new_36X;
                goto L34663;}}}}
         L34663: {
          long value_39X = arg0K0;
          *((long*)((unsigned char*)addr_29X)) = value_39X;
          arg0K0 = next_30X;
          goto L15120;}}
        else {
          arg0K0 = next_30X;
          goto L15120;}}}
    else {
      arg0K0 = next_30X;
      goto L15120;}}}
  else {
    ShpS = RShpS;
    return;}}}}

long copy_weak_pointer(long weak_40X)
{
  extern long ShpS;
  extern long Sweak_pointer_limitS;
  extern long Sweak_pointer_hpS;

 {long y_41X;
  y_41X = Sweak_pointer_hpS;
  if ((-1 == y_41X)) {
    goto L11222;}
  else {
    if ((Sweak_pointer_hpS < Sweak_pointer_limitS)) {
      goto L11227;}
    else {
      goto L11222;}}
 L11222: {
  long old_42X;
  old_42X = Sweak_pointer_hpS;
  Sweak_pointer_hpS = ShpS;
  ShpS = (1024 + ShpS);
  Sweak_pointer_limitS = ShpS;
  *((long*)((unsigned char*)Sweak_pointer_hpS)) = 261186 /* was 262210 */;
  *((long*)((unsigned char*)(8 + Sweak_pointer_hpS))) = old_42X;
  goto L11227;}
 L11227: {
  long new_43X;
  new_43X = 7 + Sweak_pointer_hpS;
  *((long*)((unsigned char*)(4 + Sweak_pointer_hpS))) = (*((long*)((unsigned char*)(-3 + weak_40X))));
  Sweak_pointer_hpS = (8 + Sweak_pointer_hpS);
  *((long*)((unsigned char*)(-7 + weak_40X))) = new_43X;
  return(new_43X);}}}

void clean_weak_pointers()
{
  extern long Sweak_pointer_hpS;
  extern long Sweak_pointer_limitS;
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  long arg0K1;
  long arg0K0;

 {if ((-1 == Sweak_pointer_hpS)) {
    return;}
  else {
    long x_44X;
    long end_45X;
    x_44X = Sweak_pointer_limitS;
    end_45X = Sweak_pointer_hpS;
    arg0K0 = (-1024 + x_44X);
    arg0K1 = end_45X;
    goto L12751;
   L12751: {
    long start_46X = arg0K0;
    long end_47X = arg0K1;
    long next_48X;
    next_48X = *((long*)((unsigned char*)(8 + start_46X)));
    arg0K0 = start_46X;
    goto L9945;
   L9945: {
    long scan_49X = arg0K0;
    if ((scan_49X < end_47X)) {
      long value_50X;
      *((long*)((unsigned char*)scan_49X)) = 1070;
      value_50X = *((long*)((unsigned char*)(4 + scan_49X)));
      if ((3 == (3 & value_50X))) {
        long a_51X;
        a_51X = -3 + value_50X;
        if ((a_51X < Soldspace_beginS)) {
          goto L9990;}
        else {
          unsigned char v_52X;
          v_52X = a_51X < Soldspace_endS;
          if (v_52X) {
            if ((3 == (3 & value_50X))) {
              long h_53X;
              h_53X = *((long*)((unsigned char*)(-7 + value_50X)));
              if ((3 == (3 & h_53X))) {
                arg0K0 = h_53X;
                goto L9985;}
              else {
                arg0K0 = 1;
                goto L9985;}
             L9985: {
              long value_54X = arg0K0;
              *((long*)((unsigned char*)(4 + scan_49X))) = value_54X;
              goto L9990;}}
            else {
              goto L9990;}}
          else {
            goto L9990;}}}
      else {
        goto L9990;}
     L9990: {
      arg0K0 = (8 + scan_49X);
      goto L9945;}}
    else {
      if ((-1 == next_48X)) {
        if ((end_45X < Sweak_pointer_limitS)) {
          *((long*)((unsigned char*)end_45X)) = (66 + ((-4 & (-4 + (Sweak_pointer_limitS - end_45X))) << 8));
          return;}
        else {
          return;}}
      else {
        arg0K0 = (-1024 + next_48X);
        arg0K1 = next_48X;
        goto L12751;}}}}}}}

long save_env_in_heap(long env_55X, long cont_56X, char key_57X, char reason_58X)
{
  extern long Sstack_beginS;
  extern long Sstack_endS;
  extern long copy_stob(long, char);
  long arg0K0;

 {long new_59X;
  new_59X = copy_stob(env_55X, key_57X);
  *((long*)((unsigned char*)(-7 + env_55X))) = new_59X;
  arg0K0 = new_59X;
  goto L14533;
 L14533: {
  long env_60X = arg0K0;
  long p_61X;
  p_61X = *((long*)((unsigned char*)(-3 + env_60X)));
  if ((p_61X < Sstack_beginS)) {
    goto L14557;}
  else {
    if ((Sstack_endS < p_61X)) {
      goto L14557;}
    else {
      long env_62X;
      long new_63X;
      env_62X = *((long*)((unsigned char*)(-3 + env_60X)));
      new_63X = copy_stob(env_62X, key_57X);
      *((long*)((unsigned char*)(-7 + env_62X))) = new_63X;
      *((long*)((unsigned char*)(-3 + env_60X))) = new_63X;
      arg0K0 = new_63X;
      goto L14533;}}
 L14557: {
  arg0K0 = cont_56X;
  goto L14562;
 L14562: {
  long cont_64X = arg0K0;
  long env_65X;
  env_65X = *((long*)((unsigned char*)(9 + cont_64X)));
  if ((3 == (3 & env_65X))) {
    if ((3 == (3 & (*((long*)((unsigned char*)(-7 + env_65X))))))) {
      *((long*)((unsigned char*)(9 + cont_64X))) = (*((long*)((unsigned char*)(-7 + env_65X))));
      arg0K0 = (*((long*)((unsigned char*)(-3 + cont_64X))));
      goto L14562;}
    else {
      goto L14596;}}
  else {
    goto L14596;}
 L14596: {
  arg0K0 = env_55X;
  goto L14601;
 L14601: {
  long env_66X = arg0K0;
  long h_67X;
  h_67X = *((long*)((unsigned char*)(-7 + env_66X)));
  if ((3 == (3 & h_67X))) {
    *((long*)((unsigned char*)(-7 + env_66X))) = (*((long*)((unsigned char*)(-7 + h_67X))));
    if ((3 == (3 & (*((long*)((unsigned char*)(-3 + h_67X))))))) {
      arg0K0 = (*((long*)((unsigned char*)(-3 + h_67X))));
      goto L14601;}
    else {
      return(new_59X);}}
  else {
    return(new_59X);}}}}}}}}

void trace_env(long env_68X)
{
  extern long Sstack_beginS;
  extern long Sstack_endS;
  extern void trace_env(long);
  extern void scan(long, long);
  extern long SlimitS;
  extern void Qps_error_0(const char*);
  extern long ShpS;
  static const struct{long length; char contents[22];} XX69
     = {21, "out of memory (sorry)"};
  long arg0K0;

 {if ((env_68X < Sstack_beginS)) {
    return;}
  else {
    if ((Sstack_endS < env_68X)) {
      return;}
    else {
      long byte_count_70X;
      long starting_hp_71X;
      byte_count_70X = (*((long*)((unsigned char*)(-7 + env_68X)))) >> 8;
      starting_hp_71X = ShpS;
      scan((-3 + env_68X), ((-3 + env_68X) + (-4 & (3 + byte_count_70X))));
      arg0K0 = starting_hp_71X;
      goto L33905;
     L33905: {
      long start_72X = arg0K0;
      long end_73X;
      end_73X = ShpS;
      scan(start_72X, end_73X);
      if ((ShpS < SlimitS)) {
        if ((end_73X < ShpS)) {
          arg0K0 = end_73X;
          goto L33905;}
        else {
          goto L33882;}}
      else {
        Qps_error_0((XX69.contents));
        goto L33882;}}
     L33882: {

      trace_env((*((long*)((unsigned char*)(-3 + env_68X)))));
      return;}}}}}

void push_continuation(long code_pointer_74X, long size_75X, char key_76X)
{
  extern long SstackS;
  extern long StemplateS;
  extern long SenvS;
  extern long ScontS;

 {long cont_77X;
  long val_78X;
  SstackS = (-20 + SstackS);
  *((long*)((unsigned char*)(4 + SstackS))) = (4130 + (size_75X << 10));
  cont_77X = 11 + SstackS;
  val_78X = 12 + ((code_pointer_74X - (*((long*)((unsigned char*)(-3 + StemplateS))))) << 2);
  *((long*)((unsigned char*)(1 + cont_77X))) = val_78X;
  *((long*)((unsigned char*)(5 + cont_77X))) = StemplateS;
  *((long*)((unsigned char*)(9 + cont_77X))) = SenvS;
  *((long*)((unsigned char*)(-3 + cont_77X))) = ScontS;
  ScontS = cont_77X;
  return;}}

void start_exception(char args_79X)
{
  extern void push_continuation(long, long, char);
  extern long Scode_pointerS;
  extern long Sstack_beginS;
  extern unsigned char Sexception_space_usedPS;
  extern long SstackS;
  extern long Sstack_limitS;
  extern void Qps_error_0(const char*);
  static const struct{long length; char contents[48];} XX80
     = {47, "insufficient space on stack for exception frame"};
  long arg0K1;
  long arg0K0;
  char arg1K0;

 {unsigned char x_81X;
  x_81X = Sstack_limitS < (-296 + SstackS);
  if (x_81X) {
    arg1K0 = 0;
    goto L16166;}
  else {
    unsigned char x_82X;
    Sstack_limitS = Sstack_beginS;
    Sexception_space_usedPS = 1;
    x_82X = Sstack_limitS < (-296 + SstackS);
    if (x_82X) {
      arg1K0 = 0;
      goto L16166;}
    else {
      Qps_error_0((XX80.contents));
      arg1K0 = 0;
      goto L16166;}}
 L16166: {
  char key_83X = arg1K0;
  arg0K0 = (4 + SstackS);
  arg0K1 = 0;
  goto L16193;
 L16193: {
  long p_84X = arg0K0;
  long i_85X = arg0K1;
  if ((2 == (3 & (*((long*)((unsigned char*)p_84X)))))) {
    push_continuation(Scode_pointerS, i_85X, key_83X);
    *((long*)((unsigned char*)SstackS)) = (((long)(*((unsigned char*)((unsigned char*)(-1 + (Scode_pointerS - ((long)args_79X))))))) << 2);
    SstackS = (-4 + SstackS);
    return;}
  else {
    arg0K0 = (4 + p_84X);
    arg0K1 = (1 + i_85X);
    goto L16193;}}}}}

void begin_collection()
{
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern long Ssaved_limitS;
  extern long Ssaved_hpS;
  extern long Snewspace_endS;
  extern long SlimitS;
  extern long Snewspace_beginS;
  extern long ShpS;
  extern long Sweak_pointer_hpS;

 {long b_86X;
  long e_87X;
  b_86X = Snewspace_beginS;
  Snewspace_beginS = Soldspace_beginS;
  Soldspace_beginS = b_86X;
  e_87X = Snewspace_endS;
  Snewspace_endS = Soldspace_endS;
  Soldspace_endS = e_87X;
  Ssaved_limitS = SlimitS;
  Ssaved_hpS = ShpS;
  SlimitS = Snewspace_endS;
  ShpS = Snewspace_beginS;
  Sweak_pointer_hpS = -1;
  return;}}

long trace_value(long value_88X)
{
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern void scan(long, long);
  extern long SlimitS;
  extern void Qps_error_0(const char*);
  extern long ShpS;
  extern long copy_weak_pointer(long);
  static const struct{long length; char contents[22];} XX69
     = {21, "out of memory (sorry)"};
  long arg0K0;

 {if ((3 == (3 & value_88X))) {
    long a_89X;
    a_89X = -3 + value_88X;
    if ((a_89X < Soldspace_beginS)) {
      return(value_88X);}
    else {
      unsigned char v_90X;
      v_90X = a_89X < Soldspace_endS;
      if (v_90X) {
        long start_91X;
        long h_92X;
        start_91X = ShpS;
        h_92X = *((long*)((unsigned char*)(-7 + value_88X)));
        if ((3 == (3 & h_92X))) {
          arg0K0 = h_92X;
          goto L16407;}
        else {
          if ((1070 == h_92X)) {
            long v_93X;
            v_93X = copy_weak_pointer(value_88X);
            arg0K0 = v_93X;
            goto L16407;}
          else {
            long new_94X;
            long x_95X;
            *((long*)((unsigned char*)ShpS)) = h_92X;
            ShpS = (4 + ShpS);
            new_94X = 3 + ShpS;
            *((long*)((unsigned char*)(-7 + value_88X))) = new_94X;
            x_95X = ShpS;
            arg0K0 = (-3 + value_88X);
            goto L12823;
           L12823: {
            long o_96X = arg0K0;
            if ((ShpS < (x_95X + (-4 & (3 + (h_92X >> 8)))))) {
              *((long*)((unsigned char*)ShpS)) = (*((long*)((unsigned char*)o_96X)));
              ShpS = (4 + ShpS);
              arg0K0 = (4 + o_96X);
              goto L12823;}
            else {
              arg0K0 = new_94X;
              goto L16407;}}}}
       L16407: {
        long new_97X = arg0K0;
        arg0K0 = start_91X;
        goto L16420;
       L16420: {
        long start_98X = arg0K0;
        long end_99X;
        end_99X = ShpS;
        scan(start_98X, end_99X);
        if ((ShpS < SlimitS)) {
          if ((end_99X < ShpS)) {
            arg0K0 = end_99X;
            goto L16420;}
          else {
            return(new_97X);}}
        else {
          Qps_error_0((XX69.contents));
          return(new_97X);}}}}
      else {
        return(value_88X);}}}
  else {
    return(value_88X);}}}

void end_collection()
{
  extern void clean_weak_pointers();
  extern long *Sopen_vm_portsS;
  extern void close_port_noisily(long);
  extern long Sgc_countS;
  long arg0K0;

 {clean_weak_pointers();
  arg0K0 = 2;
  goto L15234;
 L15234: {
  long i_100X = arg0K0;
  if ((100 == i_100X)) {
    Sgc_countS = (1 + Sgc_countS);
    return;}
  else {
    long port_101X;
    port_101X = *((Sopen_vm_portsS + i_100X));
    if ((1 == (255 & port_101X))) {
      arg0K0 = (1 + i_100X);
      goto L15234;}
    else {
      if ((3 == (3 & (*((long*)((unsigned char*)(-7 + port_101X))))))) {
        arg0K0 = (*((long*)((unsigned char*)(-7 + port_101X))));
        goto L15243;}
      else {
        long id_102X;
        long descriptor_103X;
        id_102X = *((long*)((unsigned char*)(9 + port_101X)));
        descriptor_103X = *((long*)((unsigned char*)(-7 + id_102X)));
        if ((3 == (3 & descriptor_103X))) {
          *((long*)((unsigned char*)(9 + port_101X))) = (*((long*)((unsigned char*)(-7 + id_102X))));
          goto L15182;}
        else {
          goto L15182;}
       L15182: {
        close_port_noisily(port_101X);
        arg0K0 = 1;
        goto L15243;}}
     L15243: {
      long value_104X = arg0K0;
      *((Sopen_vm_portsS + i_100X)) = value_104X;
      arg0K0 = (1 + i_100X);
      goto L15234;}}}}}}

long preserve_continuation(long cont_105X, char key_106X, char reason_107X)
{
  extern long copy_stob(long, char);
  extern long Sbottom_of_stackS;
  extern long Sstack_beginS;
  extern long Sstack_endS;
  extern long save_env_in_heap(long, long, char, char);
  extern long SenvS;
  long arg0K1;
  long arg0K0;

 {long p_108X;
  p_108X = SenvS;
  if ((p_108X < Sstack_beginS)) {
    goto L15855;}
  else {
    if ((Sstack_endS < p_108X)) {
      goto L15855;}
    else {
      long v_109X;
      v_109X = save_env_in_heap(SenvS, cont_105X, key_106X, reason_107X);
      SenvS = v_109X;
      goto L15855;}}
 L15855: {
  long end_110X;
  end_110X = *((long*)((unsigned char*)(-3 + Sbottom_of_stackS)));
  arg0K0 = cont_105X;
  arg0K1 = Sbottom_of_stackS;
  goto L15863;
 L15863: {
  long cont_111X = arg0K0;
  long previous_112X = arg0K1;
  if ((cont_111X == Sbottom_of_stackS)) {
    *((long*)((unsigned char*)(-3 + previous_112X))) = end_110X;
    return((*((long*)((unsigned char*)(-3 + Sbottom_of_stackS)))));}
  else {
    long p_113X;
    p_113X = *((long*)((unsigned char*)(9 + cont_111X)));
    if ((p_113X < Sstack_beginS)) {
      goto L15885;}
    else {
      if ((Sstack_endS < p_113X)) {
        goto L15885;}
      else {
        save_env_in_heap((*((long*)((unsigned char*)(9 + cont_111X)))), cont_111X, key_106X, reason_107X);
        goto L15885;}}
   L15885: {
    long new_114X;
    new_114X = copy_stob(cont_111X, key_106X);
    *((long*)((unsigned char*)(-3 + previous_112X))) = new_114X;
    arg0K0 = (*((long*)((unsigned char*)(-3 + new_114X))));
    arg0K1 = new_114X;
    goto L15863;}}}}}}

void trace_stack(long cont_115X)
{
  extern long SstackS;
  extern long Sbottom_of_stackS;
  extern void trace_env(long);
  extern void scan(long, long);
  extern long SlimitS;
  extern void Qps_error_0(const char*);
  extern long ShpS;
  static const struct{long length; char contents[22];} XX69
     = {21, "out of memory (sorry)"};
  long arg0K1;
  long arg0K0;

 {long x_116X;
  long starting_hp_117X;
  x_116X = SstackS;
  starting_hp_117X = ShpS;
  scan((4 + x_116X), (-7 + cont_115X));
  arg0K0 = starting_hp_117X;
  goto L34105;
 L34105: {
  long start_118X = arg0K0;
  long end_119X;
  end_119X = ShpS;
  scan(start_118X, end_119X);
  if ((ShpS < SlimitS)) {
    if ((end_119X < ShpS)) {
      arg0K0 = end_119X;
      goto L34105;}
    else {
      goto L33961;}}
  else {
    Qps_error_0((XX69.contents));
    goto L33961;}}
 L33961: {
  arg0K0 = cont_115X;
  arg0K1 = 0;
  goto L33966;
 L33966: {
  long cont_120X = arg0K0;
  long last_env_121X = arg0K1;
  long env_122X;
  long byte_count_123X;
  long starting_hp_124X;
  env_122X = *((long*)((unsigned char*)(9 + cont_120X)));
  byte_count_123X = (*((long*)((unsigned char*)(-7 + cont_120X)))) >> 8;
  starting_hp_124X = ShpS;
  scan((-3 + cont_120X), ((-3 + cont_120X) + (-4 & (3 + byte_count_123X))));
  arg0K0 = starting_hp_124X;
  goto L34067;
 L34067: {
  long start_125X = arg0K0;
  long end_126X;
  end_126X = ShpS;
  scan(start_125X, end_126X);
  if ((ShpS < SlimitS)) {
    if ((end_126X < ShpS)) {
      arg0K0 = end_126X;
      goto L34067;}
    else {
      goto L33975;}}
  else {
    Qps_error_0((XX69.contents));
    goto L33975;}}
 L33975: {
  if ((env_122X == last_env_121X)) {
    goto L33987;}
  else {
    trace_env(env_122X);
    goto L33987;}
 L33987: {
  if ((cont_120X == Sbottom_of_stackS)) {
    return;}
  else {
    arg0K0 = (*((long*)((unsigned char*)(-3 + cont_120X))));
    arg0K1 = env_122X;
    goto L33966;}}}}}}}

char Tinterpret()
{
  extern long extended_vm(long, long);
#ifndef ROBOT  /* JAR HACK */
  extern long ps_real_time();
  extern long ps_run_time();
#endif
  extern long ps_ticks_per_second();
  extern unsigned char lookup_external_name(long, long);
  extern long ps_schedule_interrupt(long);
  extern long Sinterrupt_handlersS;
  extern long Sexception_handlerS;
  extern long Sdynamic_stateS;
  extern long Sgc_countS;
  extern long Sminimum_recovered_spaceS;
  extern long Sstack_sizeS;
  extern long Sweak_pointer_hpS;
  extern long trace_value(long);
  extern void clean_weak_pointers();
  extern long Soldspace_beginS;
  extern long Snewspace_endS;
  extern long Soldspace_endS;
  extern long Ssaved_limitS;
  extern long Ssaved_hpS;
  extern void close_port_noisily(long);
  extern void write_string(const char*, FILE *);
  extern long Snewspace_beginS;
  extern FILE * Scurrent_output_portS;
  extern void close_port(long);
  extern FILE * *Sopen_portsS;
  extern void Qps_error_0(const char*);
  extern long *Sopen_vm_portsS;
  extern long SlimitS;
  extern long Senabled_interruptsS;
  extern long Spending_interruptsS;
  extern long preserve_continuation(long, char, char);
  extern long Sbottom_of_stackS;
  extern void push_continuation(long, long, char);
  extern long ScontS;
  extern long save_env_in_heap(long, long, char, char);
  extern long Sstack_beginS;
  extern long Sstack_endS;
  extern long StemplateS;
  extern long ShpS;
  extern long SenvS;
  extern long Sstack_limitS;
  extern long Scode_pointerS;
  extern long SvalS;
  extern long SnargsS;
  extern long SstackS;
  extern char Tinterpret();
  static const struct{long length; char contents[34];} XX127
     = {33, "Heap is in an inconsistent state."};
  static const struct{long length; char contents[37];} XX128
     = {36, "This is a Scheme 48 heap image file."};
  static const struct{long length; char contents[38];} XX21
     = {37, "block write did not send enough bytes"};
  static const struct{long length; char contents[17];} XX129
     = {16, "ran out of ports"};
  FILE * arg3K0;
  unsigned char arg2K3;
  char arg1K0;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char merged_arg1K0;
  FILE * merged_arg3K1;
  long merged_arg0K4;
  long merged_arg0K3;
  long merged_arg0K2;
  long merged_arg0K1;
  long merged_arg0K0;
  long RScode_pointerS = Scode_pointerS;
  long RSvalS = SvalS;
  long RSstackS = SstackS;

  int write_number_no_newline_return_tag;
  int stob_equalP_return_tag;
  unsigned char stob_equalP_return_value;
  int extract_string_return_tag;
  char *extract_string_return_value;
  int copy_continuation_from_heap_return_tag;
  long copy_continuation_from_heap_return_value;
  int collect_saving_temp_return_tag;
  long collect_saving_temp_return_value;
  int interpreter_collect_return_tag;
  long interpreter_collect_return_value;
  int get_stack_space_return_tag;
  int ensure_space_return_tag;
  char ensure_space_return_value;
  int pop_args_list_return_tag;
  long pop_args_list_return_value;
 START: {
  unsigned char b_130X;
  b_130X = *((unsigned char*)((unsigned char*)RScode_pointerS));
  RScode_pointerS = (1 + RScode_pointerS);
  switch (b_130X) {
    case 0 : {
      unsigned char b_131X;
      b_131X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((SnargsS == ((long)b_131X))) {
        goto START;}
      else {
        long x_132X;
        x_132X = SnargsS;
        if ((64 == x_132X)) {
          long v_133X;
          RSstackS = (4 + RSstackS);
          merged_arg0K0 = (*((long*)((unsigned char*)RSstackS)));
          merged_arg0K1 = 63;
          pop_args_list_return_tag = 0;
          goto pop_args_list;
         pop_args_list_return_0:
          v_133X = pop_args_list_return_value;
          arg0K0 = v_133X;
          goto L33490;}
        else {
          long v_134X;
          merged_arg0K0 = 25;
          merged_arg0K1 = SnargsS;
          pop_args_list_return_tag = 1;
          goto pop_args_list;
         pop_args_list_return_1:
          v_134X = pop_args_list_return_value;
          arg0K0 = v_134X;
          goto L33490;}
       L33490: {
        long args_135X = arg0K0;
        merged_arg1K0 = 1;
        merged_arg0K1 = RSvalS;
        merged_arg0K2 = args_135X;
        goto raise_exception_no_cont2;}}}
      break;
    case 1 : {
      unsigned char b_136X;
      b_136X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((SnargsS < ((long)b_136X))) {
        long x_137X;
        x_137X = SnargsS;
        if ((64 == x_137X)) {
          long v_138X;
          RSstackS = (4 + RSstackS);
          merged_arg0K0 = (*((long*)((unsigned char*)RSstackS)));
          merged_arg0K1 = 63;
          pop_args_list_return_tag = 2;
          goto pop_args_list;
         pop_args_list_return_2:
          v_138X = pop_args_list_return_value;
          arg0K0 = v_138X;
          goto L33429;}
        else {
          long v_139X;
          merged_arg0K0 = 25;
          merged_arg0K1 = SnargsS;
          pop_args_list_return_tag = 3;
          goto pop_args_list;
         pop_args_list_return_3:
          v_139X = pop_args_list_return_value;
          arg0K0 = v_139X;
          goto L33429;}
       L33429: {
        long args_140X = arg0K0;
        merged_arg1K0 = 1;
        merged_arg0K1 = RSvalS;
        merged_arg0K2 = args_140X;
        goto raise_exception_no_cont2;}}
      else {
        goto START;}}
      break;
    case 2 : {
      RSvalS = (SnargsS << 2);
      goto START;}
      break;
    case 3 : {
      unsigned char x_141X;
      x_141X = Sstack_limitS < (-8 + RSstackS);
      if (x_141X) {
        goto L28441;}
      else {
        merged_arg1K0 = 2;
        get_stack_space_return_tag = 0;
        goto get_stack_space;
       get_stack_space_return_0:
        goto L28441;}
     L28441: {
      unsigned char count_142X;
      long env_143X;
      count_142X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      *((long*)((unsigned char*)RSstackS)) = 0;
      RSstackS = (-4 + RSstackS);
      *((long*)((unsigned char*)RSstackS)) = (1034 + (((long)count_142X) << 10));
      RSstackS = (-4 + RSstackS);
      env_143X = 11 + RSstackS;
      RScode_pointerS = (1 + RScode_pointerS);
      *((long*)((unsigned char*)(-3 + env_143X))) = SenvS;
      SenvS = env_143X;
      goto START;}}
      break;
    case 4 : {
      unsigned char count_144X;
      long len_145X;
      long new_146X;
      merged_arg0K0 = (2 + SnargsS);
      ensure_space_return_tag = 0;
      goto ensure_space;
     ensure_space_return_0:
      count_144X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      len_145X = 4 + (((long)count_144X) << 2);
      *((long*)((unsigned char*)ShpS)) = (10 + (len_145X << 8));
      ShpS = (4 + ShpS);
      new_146X = 3 + ShpS;
      ShpS = (ShpS + (-4 & (3 + len_145X)));
      if ((0 < len_145X)) {
        *((long*)((unsigned char*)(-4 + ShpS))) = 0;
        arg0K0 = new_146X;
        goto L9388;}
      else {
        arg0K0 = new_146X;
        goto L9388;}
     L9388: {
      long stob_147X = arg0K0;
      long from_148X;
      from_148X = 4 + RSstackS;
      arg0K0 = from_148X;
      arg0K1 = (1 + stob_147X);
      goto L9420;
     L9420: {
      long from_149X = arg0K0;
      long to_150X = arg0K1;
      if ((from_149X < (from_148X + (((long)count_144X) << 2)))) {
        *((long*)((unsigned char*)to_150X)) = (*((long*)((unsigned char*)from_149X)));
        arg0K0 = (4 + from_149X);
        arg0K1 = (4 + to_150X);
        goto L9420;}
      else {
        RSstackS = (RSstackS - ((0 - ((long)count_144X)) << 2));
        RScode_pointerS = (1 + RScode_pointerS);
        *((long*)((unsigned char*)(-3 + stob_147X))) = SenvS;
        SenvS = stob_147X;
        goto START;}}}}
      break;
    case 5 : {
      SenvS = (*((long*)((unsigned char*)(-3 + SenvS))));
      goto START;}
      break;
    case 6 : {
      unsigned char b_151X;
      long x_152X;
      b_151X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      x_152X = SnargsS;
      if ((64 == x_152X)) {
        long x_153X;
        long v_154X;
        x_153X = SnargsS;
        RSstackS = (4 + RSstackS);
        merged_arg0K0 = (*((long*)((unsigned char*)RSstackS)));
        merged_arg0K1 = ((-1 + x_153X) - ((long)b_151X));
        pop_args_list_return_tag = 4;
        goto pop_args_list;
       pop_args_list_return_4:
        v_154X = pop_args_list_return_value;
        arg0K0 = v_154X;
        goto L27625;}
      else {
        long v_155X;
        merged_arg0K0 = 25;
        merged_arg0K1 = (SnargsS - ((long)b_151X));
        pop_args_list_return_tag = 5;
        goto pop_args_list;
       pop_args_list_return_5:
        v_155X = pop_args_list_return_value;
        arg0K0 = v_155X;
        goto L27625;}
     L27625: {
      long args_156X = arg0K0;
      RSvalS = args_156X;
      SnargsS = (1 + ((long)b_151X));
      goto START;}}
      break;
    case 7 : {
      unsigned char b_157X;
      b_157X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((unsigned char*)((-3 + StemplateS) + (((long)b_157X) << 2)))));
      goto START;}
      break;
    case 8 : {
      unsigned char b_158X;
      long env_159X;
      b_158X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      env_159X = SenvS;
      arg0K0 = env_159X;
      arg0K1 = ((long)b_158X);
      goto L27980;
     L27980: {
      long env_160X = arg0K0;
      long i_161X = arg0K1;
      if ((0 == i_161X)) {
        unsigned char b_162X;
        b_162X = *((unsigned char*)((unsigned char*)RScode_pointerS));
        RScode_pointerS = (1 + RScode_pointerS);
        RSvalS = (*((long*)((unsigned char*)((-3 + env_160X) + (((long)b_162X) << 2)))));
        if ((529 == RSvalS)) {
          merged_arg1K0 = 2;
          goto raise_exception;}
        else {
          goto START;}}
      else {
        arg0K0 = (*((long*)((unsigned char*)(-3 + env_160X))));
        arg0K1 = (-1 + i_161X);
        goto L27980;}}}
      break;
    case 9 : {
      long env_163X;
      unsigned char b_164X;
      env_163X = SenvS;
      b_164X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((unsigned char*)((-3 + env_163X) + (((long)b_164X) << 2)))));
      if ((529 == RSvalS)) {
        merged_arg1K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 10 : {
      long env_165X;
      unsigned char b_166X;
      env_165X = *((long*)((unsigned char*)(-3 + SenvS)));
      b_166X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((unsigned char*)((-3 + env_165X) + (((long)b_166X) << 2)))));
      if ((529 == RSvalS)) {
        merged_arg1K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 11 : {
      long env_167X;
      unsigned char b_168X;
      env_167X = *((long*)((unsigned char*)(-3 + (*((long*)((unsigned char*)(-3 + SenvS)))))));
      b_168X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((unsigned char*)((-3 + env_167X) + (((long)b_168X) << 2)))));
      if ((529 == RSvalS)) {
        merged_arg1K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 12 : {
      unsigned char b_169X;
      long env_170X;
      b_169X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      env_170X = SenvS;
      arg0K0 = env_170X;
      arg0K1 = ((long)b_169X);
      goto L27577;
     L27577: {
      long env_171X = arg0K0;
      long i_172X = arg0K1;
      if ((0 == i_172X)) {
        unsigned char b_173X;
        b_173X = *((unsigned char*)((unsigned char*)RScode_pointerS));
        RScode_pointerS = (1 + RScode_pointerS);
        *((long*)((unsigned char*)((-3 + env_171X) + (((long)b_173X) << 2)))) = RSvalS;
        RSvalS = 13;
        goto START;}
      else {
        arg0K0 = (*((long*)((unsigned char*)(-3 + env_171X))));
        arg0K1 = (-1 + i_172X);
        goto L27577;}}}
      break;
    case 13 : {
      unsigned char b_174X;
      long location_175X;
      b_174X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      location_175X = *((long*)((unsigned char*)((-3 + StemplateS) + (((long)b_174X) << 2))));
      RSvalS = (*((long*)((unsigned char*)(-3 + location_175X))));
      if ((17 == (255 & RSvalS))) {
        merged_arg1K0 = 1;
        merged_arg0K1 = location_175X;
        goto raise_exception1;}
      else {
        goto START;}}
      break;
    case 14 : {
      unsigned char b_176X;
      long location_177X;
      b_176X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      location_177X = *((long*)((unsigned char*)((-3 + StemplateS) + (((long)b_176X) << 2))));
      if ((273 == (*((long*)((unsigned char*)(-3 + location_177X)))))) {
        merged_arg1K0 = 1;
        merged_arg0K1 = location_177X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}
      else {
        *((long*)((unsigned char*)(-3 + location_177X))) = RSvalS;
        RSvalS = 13;
        goto START;}}
      break;
    case 15 : {
      long p_178X;
      p_178X = SenvS;
      if ((p_178X < Sstack_beginS)) {
        goto L27438;}
      else {
        if ((Sstack_endS < p_178X)) {
          goto L27438;}
        else {
          long env_179X;
          env_179X = SenvS;
          arg0K0 = env_179X;
          arg0K1 = 0;
          goto L21643;
         L21643: {
          long env_180X = arg0K0;
          long size_181X = arg0K1;
          long len_182X;
          long size_183X;
          long env_184X;
          len_182X = (3 + ((*((long*)((unsigned char*)(-7 + env_180X)))) >> 8)) >> 2;
          size_183X = size_181X + (1 + len_182X);
          env_184X = *((long*)((unsigned char*)(-3 + env_180X)));
          if ((env_184X < Sstack_beginS)) {
            arg0K0 = size_183X;
            goto L21627;}
          else {
            if ((Sstack_endS < env_184X)) {
              arg0K0 = size_183X;
              goto L21627;}
            else {
              arg0K0 = env_184X;
              arg0K1 = size_183X;
              goto L21643;}}}
         L21627: {
          long v_185X = arg0K0;
          char key_186X;
          long v_187X;
          merged_arg0K0 = v_185X;
          ensure_space_return_tag = 1;
          goto ensure_space;
         ensure_space_return_1:
          key_186X = ensure_space_return_value;
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          v_187X = save_env_in_heap(SenvS, ScontS, key_186X, 0);
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          SenvS = v_187X;
          goto L27438;}}}
     L27438: {
      unsigned char b_188X;
      long a_189X;
      long b_190X;
      long new_191X;
      merged_arg0K0 = 3;
      ensure_space_return_tag = 2;
      goto ensure_space;
     ensure_space_return_2:
      b_188X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      a_189X = *((long*)((unsigned char*)((-3 + StemplateS) + (((long)b_188X) << 2))));
      b_190X = SenvS;
      *((long*)((unsigned char*)ShpS)) = 2062;
      ShpS = (4 + ShpS);
      new_191X = 3 + ShpS;
      ShpS = (8 + ShpS);
      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
      *((long*)((unsigned char*)(-3 + new_191X))) = a_189X;
      *((long*)((unsigned char*)(1 + new_191X))) = b_190X;
      RSvalS = new_191X;
      goto START;}}
      break;
    case 16 : {
      *((long*)((unsigned char*)RSstackS)) = RSvalS;
      RSstackS = (-4 + RSstackS);
      goto START;}
      break;
    case 17 : {
      RSstackS = (4 + RSstackS);
      RSvalS = (*((long*)((unsigned char*)RSstackS)));
      goto START;}
      break;
    case 18 : {
      unsigned char b_192X;
      b_192X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((unsigned char*)(RSstackS + (4 + (((long)b_192X) << 2))))));
      goto START;}
      break;
    case 19 : {
      unsigned char b_193X;
      b_193X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      *((long*)((unsigned char*)(RSstackS + (4 + (((long)b_193X) << 2))))) = RSvalS;
      goto START;}
      break;
    case 20 : {
      unsigned char x_194X;
      x_194X = Sstack_limitS < (-276 + RSstackS);
      if (x_194X) {
        arg1K0 = 0;
        goto L28345;}
      else {
        merged_arg1K0 = 69;
        get_stack_space_return_tag = 1;
        goto get_stack_space;
       get_stack_space_return_1:
        arg1K0 = 0;
        goto L28345;}
     L28345: {
      char key_195X = arg1K0;
      unsigned char b_196X;
      unsigned char b_197X;
      unsigned char b_198X;
      b_196X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      b_197X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      b_198X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      SvalS = RSvalS;
      SstackS = RSstackS;
      Scode_pointerS = RScode_pointerS;
      push_continuation((RScode_pointerS + ((((long)b_196X) << 8) + ((long)b_197X))), ((long)b_198X), key_195X);
      RSvalS = SvalS;
      RSstackS = SstackS;
      RScode_pointerS = Scode_pointerS;
      goto START;}}
      break;
    case 21 : {
      long v_199X;
      char key_200X;
      long cont_201X;
      v_199X = 1 + ((Sstack_endS - RSstackS) >> 2);
      merged_arg0K0 = v_199X;
      ensure_space_return_tag = 3;
      goto ensure_space;
     ensure_space_return_3:
      key_200X = ensure_space_return_value;
      SvalS = RSvalS;
      SstackS = RSstackS;
      Scode_pointerS = RScode_pointerS;
      cont_201X = preserve_continuation(ScontS, key_200X, 2);
      RSvalS = SvalS;
      RSstackS = SstackS;
      RScode_pointerS = Scode_pointerS;
      if ((1 == (255 & cont_201X))) {
        RSstackS = (-11 + Sbottom_of_stackS);
        Sstack_limitS = (296 + Sstack_beginS);
        *((long*)((unsigned char*)(-3 + Sbottom_of_stackS))) = 1;
        arg0K0 = Sbottom_of_stackS;
        goto L27357;}
      else {
        RSstackS = (-11 + Sbottom_of_stackS);
          *((long*)((unsigned char*)(-3 + Sbottom_of_stackS))) = cont_201X;
        arg0K0 = Sbottom_of_stackS;
        goto L27357;}
/*
        long v_202X;
        merged_arg0K0 = cont_201X;
        copy_continuation_from_heap_return_tag = 0;
        goto copy_continuation_from_heap;
       copy_continuation_from_heap_return_0:
        v_202X = copy_continuation_from_heap_return_value;
        arg0K0 = v_202X;
        goto L27357;}
*/
     L27357: {
      long v_203X = arg0K0;
      ScontS = v_203X;
      RSvalS = cont_201X;
      goto START;}}
      break;
    case 22 : {
      long cont_204X;
      cont_204X = *((long*)((unsigned char*)(-3 + Sbottom_of_stackS)));
      if ((1 == (255 & cont_204X))) {
        arg0K0 = cont_204X;
        goto L27273;}
      else {
        long v_205X;
        merged_arg0K0 = cont_204X;
        copy_continuation_from_heap_return_tag = 1;
        goto copy_continuation_from_heap;
       copy_continuation_from_heap_return_1:
        v_205X = copy_continuation_from_heap_return_value;
        arg0K0 = v_205X;
        goto L27273;}
     L27273: {
      long cont_206X = arg0K0;
      if ((1 == (255 & cont_206X))) {
        if ((0 == (3 & RSvalS))) {
          RSvalS = (RSvalS >> 2);
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          TTreturn_value = 0;
          return(0L);}
        else {
          RSstackS = (-11 + Sbottom_of_stackS);
          Sstack_limitS = (296 + Sstack_beginS);
          *((long*)((unsigned char*)(-3 + Sbottom_of_stackS))) = 1;
          merged_arg1K0 = 0;
          merged_arg0K1 = RSvalS;
          goto raise_exception1;}}
      else {
        ScontS = cont_206X;
        goto START;}}}
      break;
    case 23 : {
      SnargsS = ((long)(*((unsigned char*)((unsigned char*)RScode_pointerS))));
      merged_arg1K0 = 0;
      goto perform_application;}
      break;
    case 24 : {
      long cont_207X;
      long start_arg_208X;
      long start_loc_209X;
      SnargsS = ((long)(*((unsigned char*)((unsigned char*)RScode_pointerS))));
      cont_207X = ScontS;
      start_arg_208X = RSstackS + (SnargsS << 2);
      start_loc_209X = -11 + cont_207X;
      if ((start_arg_208X < start_loc_209X)) {
        arg0K0 = start_loc_209X;
        arg0K1 = start_arg_208X;
        goto L32851;
       L32851: {
        long loc_210X = arg0K0;
        long arg_211X = arg0K1;
        if ((RSstackS < arg_211X)) {
          *((long*)((unsigned char*)loc_210X)) = (*((long*)((unsigned char*)arg_211X)));
          arg0K0 = (-4 + loc_210X);
          arg0K1 = (-4 + arg_211X);
          goto L32851;}
        else {
          RSstackS = loc_210X;
          goto L32834;}}}
      else {
        goto L32834;}
     L32834: {
      merged_arg1K0 = 0;
      goto perform_application;}}
      break;
    case 25 : {
      long proc_212X;
      long list_213X;
      RSstackS = (4 + RSstackS);
      proc_212X = *((long*)((unsigned char*)RSstackS));
      list_213X = RSvalS;
      arg0K0 = list_213X;
      arg0K1 = 0;
      arg0K2 = list_213X;
      arg2K3 = 0;
      goto L32791;
     L32791: {
      long fast_214X = arg0K0;
      long len_215X = arg0K1;
      long slow_216X = arg0K2;
      unsigned char move_slowP_217X = arg2K3;
      if ((25 == fast_214X)) {
        long args_218X;
        long nargs_219X;
        args_218X = RSvalS;
        nargs_219X = len_215X + ((long)(*((unsigned char*)((unsigned char*)RScode_pointerS))));
        if ((63 < nargs_219X)) {
          arg0K0 = (-63 + nargs_219X);
          goto L21895;}
        else {
          arg0K0 = 0;
          goto L21895;}
       L21895: {
        long overflow_220X = arg0K0;
        arg0K0 = (len_215X - overflow_220X);
        arg0K1 = args_218X;
        goto L21935;
       L21935: {
        long i_221X = arg0K0;
        long l_222X = arg0K1;
        if ((0 < i_221X)) {
          *((long*)((unsigned char*)RSstackS)) = (*((long*)((unsigned char*)(-3 + l_222X))));
          RSstackS = (-4 + RSstackS);
          arg0K0 = (-1 + i_221X);
          arg0K1 = (*((long*)((unsigned char*)(1 + l_222X))));
          goto L21935;}
        else {
          if ((0 == overflow_220X)) {
            SnargsS = nargs_219X;
            goto L32761;}
          else {
            long list_223X;
            long a_224X;
            long new_225X;
            *((long*)((unsigned char*)RSstackS)) = l_222X;
            RSstackS = (-4 + RSstackS);
            merged_arg0K0 = (3 * overflow_220X);
            ensure_space_return_tag = 4;
            goto ensure_space;
           ensure_space_return_4:
            RSstackS = (4 + RSstackS);
            list_223X = *((long*)((unsigned char*)RSstackS));
            a_224X = *((long*)((unsigned char*)(-3 + list_223X)));
            *((long*)((unsigned char*)ShpS)) = 2050;
            ShpS = (4 + ShpS);
            new_225X = 3 + ShpS;
            ShpS = (8 + ShpS);
            *((long*)((unsigned char*)(-4 + ShpS))) = 0;
            *((long*)((unsigned char*)(-3 + new_225X))) = a_224X;
            *((long*)((unsigned char*)(1 + new_225X))) = 25;
            arg0K0 = (*((long*)((unsigned char*)(1 + list_223X))));
            arg0K1 = new_225X;
            goto L21786;
           L21786: {
            long l_226X = arg0K0;
            long last_227X = arg0K1;
            if ((25 == l_226X)) {
              *((long*)((unsigned char*)RSstackS)) = new_225X;
              RSstackS = (-4 + RSstackS);
              SnargsS = 64;
              goto L32761;}
            else {
              long a_228X;
              long new_229X;
              a_228X = *((long*)((unsigned char*)(-3 + l_226X)));
              *((long*)((unsigned char*)ShpS)) = 2050;
              ShpS = (4 + ShpS);
              new_229X = 3 + ShpS;
              ShpS = (8 + ShpS);
              *((long*)((unsigned char*)(-4 + ShpS))) = 0;
              *((long*)((unsigned char*)(-3 + new_229X))) = a_228X;
              *((long*)((unsigned char*)(1 + new_229X))) = 25;
              *((long*)((unsigned char*)(1 + last_227X))) = new_229X;
              arg0K0 = (*((long*)((unsigned char*)(1 + l_226X))));
              arg0K1 = new_229X;
              goto L21786;}}}}}}
       L32761: {
        RSvalS = proc_212X;
        merged_arg1K0 = 0;
        goto perform_application;}}
      else {
        if ((3 == (3 & fast_214X))) {
          long x_230X;
          x_230X = 31 & ((*((long*)((unsigned char*)(-7 + fast_214X)))) >> 2);
          if ((0 == x_230X)) {
            if (move_slowP_217X) {
              if ((fast_214X == slow_216X)) {
                goto L32766;}
              else {
                arg0K0 = (*((long*)((unsigned char*)(1 + fast_214X))));
                arg0K1 = (1 + len_215X);
                arg0K2 = (*((long*)((unsigned char*)(1 + slow_216X))));
                arg2K3 = 0;
                goto L32791;}}
            else {
              arg0K0 = (*((long*)((unsigned char*)(1 + fast_214X))));
              arg0K1 = (1 + len_215X);
              arg0K2 = slow_216X;
              arg2K3 = 1;
              goto L32791;}}
          else {
            goto L32766;}}
        else {
          goto L32766;}}}
     L32766: {
      long args_231X;
      merged_arg0K0 = 25;
      merged_arg0K1 = ((long)(*((unsigned char*)((unsigned char*)RScode_pointerS))));
      pop_args_list_return_tag = 6;
      goto pop_args_list;
     pop_args_list_return_6:
      args_231X = pop_args_list_return_value;
      merged_arg1K0 = 0;
      merged_arg0K1 = proc_212X;
      merged_arg0K2 = args_231X;
      merged_arg0K3 = RSvalS;
      goto raise_exception3;}}
      break;
    case 26 : {
      long cont_232X;
      RSstackS = (4 + RSstackS);
      cont_232X = *((long*)((unsigned char*)RSstackS));
      if ((1 == (255 & cont_232X))) {
        RSstackS = (-11 + Sbottom_of_stackS);
        Sstack_limitS = (296 + Sstack_beginS);
        *((long*)((unsigned char*)(-3 + Sbottom_of_stackS))) = 1;
        arg0K0 = Sbottom_of_stackS;
        goto L32732;}
      else {
        long v_233X;
        merged_arg0K0 = cont_232X;
        copy_continuation_from_heap_return_tag = 2;
        goto copy_continuation_from_heap;
       copy_continuation_from_heap_return_2:
        v_233X = copy_continuation_from_heap_return_value;
        arg0K0 = v_233X;
        goto L32732;}
     L32732: {
      long v_234X = arg0K0;
      ScontS = v_234X;
      SnargsS = 0;
      merged_arg1K0 = 0;
      goto perform_application;}}
      break;
    case 27 : {
      long consumer_235X;
      RSstackS = (4 + RSstackS);
      consumer_235X = *((long*)((unsigned char*)RSstackS));
      *((long*)((unsigned char*)RSstackS)) = RSvalS;
      RSstackS = (-4 + RSstackS);
      RSvalS = consumer_235X;
      SnargsS = 1;
      merged_arg1K0 = 0;
      goto perform_application;}
      break;
    case 28 : {
      long cont_236X;
      long pc_237X;
      long tem_238X;
      cont_236X = ScontS;
      pc_237X = *((long*)((unsigned char*)(1 + cont_236X)));
      tem_238X = *((long*)((unsigned char*)(5 + cont_236X)));
      StemplateS = tem_238X;
      RScode_pointerS = ((-3 + (*((long*)((unsigned char*)(-3 + tem_238X))))) + (pc_237X >> 2));
      SenvS = (*((long*)((unsigned char*)(9 + cont_236X))));
      ScontS = (*((long*)((unsigned char*)(-3 + cont_236X))));
      RSstackS = (9 + cont_236X);
      goto START;}
      break;
    case 29 : {
      merged_arg0K0 = SnargsS;
      goto return_values;}
      break;
    case 30 : {
      merged_arg0K0 = ((long)(*((unsigned char*)((unsigned char*)RScode_pointerS))));
      goto return_values;}
      break;
    case 31 : {
      goto START;}
      break;
    case 32 : {
      unsigned char b_239X;
      long tem_240X;
      b_239X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      tem_240X = *((long*)((unsigned char*)((-3 + StemplateS) + (((long)b_239X) << 2))));
      StemplateS = tem_240X;
      RScode_pointerS = (-3 + (*((long*)((unsigned char*)(-3 + tem_240X)))));
      goto START;}
      break;
    case 33 : {
      unsigned char b_241X;
      unsigned char b_242X;
      long tem_243X;
      b_241X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      SnargsS = ((long)b_241X);
      b_242X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      tem_243X = *((long*)((unsigned char*)((-3 + StemplateS) + (((long)b_242X) << 2))));
      StemplateS = tem_243X;
      RScode_pointerS = (-3 + (*((long*)((unsigned char*)(-3 + tem_243X)))));
      if ((0 == (Spending_interruptsS & Senabled_interruptsS))) {
        goto START;}
      else {
        goto handle_interrupt;}}
      break;
    case 34 : {
      unsigned char b_244X;
      unsigned char b_245X;
      b_244X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      b_245X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((1 == (255 & RSvalS))) {
        RScode_pointerS = (RScode_pointerS + ((((long)b_244X) << 8) + ((long)b_245X)));
        goto START;}
      else {
        goto START;}}
      break;
    case 35 : {
      unsigned char b_246X;
      unsigned char b_247X;
      b_246X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      b_247X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      RScode_pointerS = (RScode_pointerS + ((((long)b_246X) << 8) + ((long)b_247X)));
      goto START;}
      break;
    case 36 : {
      if ((0 == (3 & RSvalS))) {
        long val_248X;
        unsigned char b_249X;
        val_248X = RSvalS >> 2;
        b_249X = *((unsigned char*)((unsigned char*)RScode_pointerS));
        RScode_pointerS = (1 + RScode_pointerS);
        if ((val_248X < 0)) {
          arg0K0 = (((long)b_249X) << 1);
          goto L27071;}
        else {
          if ((val_248X < ((long)b_249X))) {
            unsigned char b_250X;
            unsigned char b_251X;
            RScode_pointerS = (RScode_pointerS + (val_248X << 1));
            b_250X = *((unsigned char*)((unsigned char*)RScode_pointerS));
            RScode_pointerS = (1 + RScode_pointerS);
            b_251X = *((unsigned char*)((unsigned char*)RScode_pointerS));
            RScode_pointerS = (1 + RScode_pointerS);
            arg0K0 = ((((long)b_250X) << 8) + ((long)b_251X));
            goto L27071;}
          else {
            arg0K0 = (((long)b_249X) << 1);
            goto L27071;}}
       L27071: {
        long offset_252X = arg0K0;
        RScode_pointerS = (RScode_pointerS + offset_252X);
        goto START;}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 37 : {
      long arg2_253X;
      long x_254X;
      RSstackS = (4 + RSstackS);
      arg2_253X = *((long*)((unsigned char*)RSstackS));
      x_254X = RSvalS;
      if ((arg2_253X == x_254X)) {
        arg1K0 = 5;
        goto L19449;}
      else {
        arg1K0 = 1;
        goto L19449;}
     L19449: {
      char val_255X = arg1K0;
      RSvalS = ((long)val_255X);
      goto START;}}
      break;
    case 38 : {
      long x_256X;
      x_256X = RSvalS;
      if ((0 == (3 & x_256X))) {
        arg1K0 = 5;
        goto L26275;}
      else {
        if ((3 == (3 & x_256X))) {
          long x_257X;
          x_257X = 31 & ((*((long*)((unsigned char*)(-7 + x_256X)))) >> 2);
          if ((9 == x_257X)) {
            arg1K0 = 5;
            goto L26275;}
          else {
            arg1K0 = 1;
            goto L26275;}}
        else {
          arg1K0 = 1;
          goto L26275;}}
     L26275: {
      char val_258X = arg1K0;
      RSvalS = ((long)val_258X);
      goto START;}}
      break;
    case 39 : {
      long x_259X;
      x_259X = RSvalS;
      if ((0 == (3 & x_259X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_259X))) {
          long x_260X;
          x_260X = 31 & ((*((long*)((unsigned char*)(-7 + x_259X)))) >> 2);
          if ((9 == x_260X)) {
            merged_arg1K0 = 0;
            merged_arg0K1 = x_259X;
            goto raise_exception1;}
          else {
            goto L26150;}}
        else {
          goto L26150;}
       L26150: {
        RSvalS = 1;
        goto START;}}}
      break;
    case 40 : {
      long x_261X;
      x_261X = RSvalS;
      if ((0 == (3 & x_261X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_261X))) {
          long x_262X;
          x_262X = 31 & ((*((long*)((unsigned char*)(-7 + x_261X)))) >> 2);
          if ((9 == x_262X)) {
            merged_arg1K0 = 0;
            merged_arg0K1 = x_261X;
            goto raise_exception1;}
          else {
            goto L26020;}}
        else {
          goto L26020;}
       L26020: {
        RSvalS = 1;
        goto START;}}}
      break;
    case 41 : {
      long x_263X;
      x_263X = RSvalS;
      if ((0 == (3 & x_263X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_263X))) {
          long x_264X;
          x_264X = 31 & ((*((long*)((unsigned char*)(-7 + x_263X)))) >> 2);
          if ((9 == x_264X)) {
            merged_arg1K0 = 0;
            merged_arg0K1 = x_263X;
            goto raise_exception1;}
          else {
            goto L25890;}}
        else {
          goto L25890;}
       L25890: {
        RSvalS = 1;
        goto START;}}}
      break;
    case 42 : {
      long x_265X;
      x_265X = RSvalS;
      if ((0 == (3 & x_265X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_265X))) {
          long x_266X;
          x_266X = 31 & ((*((long*)((unsigned char*)(-7 + x_265X)))) >> 2);
          if ((9 == x_266X)) {
            merged_arg1K0 = 0;
            merged_arg0K1 = x_265X;
            goto raise_exception1;}
          else {
            goto L25760;}}
        else {
          goto L25760;}
       L25760: {
        RSvalS = 1;
        goto START;}}}
      break;
    case 43 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = 5;
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 44 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 45 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 46 : {
      long arg2_267X;
      RSstackS = (4 + RSstackS);
      arg2_267X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_267X | RSvalS)))) {
        long x_268X;
        long z_269X;
        x_268X = RSvalS;
        z_269X = (arg2_267X >> 2) + (x_268X >> 2);
        if ((536870911 < z_269X)) {
          goto L20950;}
        else {
          if ((z_269X < -536870912)) {
            goto L20950;}
          else {
            RSvalS = (z_269X << 2);
            goto START;}}
       L20950: {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_267X;
        merged_arg0K2 = x_268X;
        goto raise_exception2;}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_267X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 47 : {
      long arg2_270X;
      RSstackS = (4 + RSstackS);
      arg2_270X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_270X | RSvalS)))) {
        long x_271X;
        long a_272X;
        long b_273X;
        x_271X = RSvalS;
        a_272X = arg2_270X >> 2;
        b_273X = x_271X >> 2;
        if ((a_272X < 0)) {
          arg0K0 = (0 - a_272X);
          goto L20686;}
        else {
          arg0K0 = a_272X;
          goto L20686;}
       L20686: {
        long a_274X = arg0K0;
        if ((b_273X < 0)) {
          arg0K0 = (0 - b_273X);
          goto L20688;}
        else {
          arg0K0 = b_273X;
          goto L20688;}
       L20688: {
        long b_275X = arg0K0;
        long lo_a_276X;
        long lo_b_277X;
        long hi_a_278X;
        long hi_b_279X;
        long lo_c_280X;
        long mid_c_281X;
        long c_282X;
        lo_a_276X = 32767 & a_274X;
        lo_b_277X = 32767 & b_275X;
        hi_a_278X = 32767 & (a_274X >> 15);
        hi_b_279X = 32767 & (b_275X >> 15);
	/* Mobot patch  -JAR */
        lo_c_280X = small_multiply(lo_a_276X, lo_b_277X);
        mid_c_281X = small_multiply(lo_a_276X, hi_b_279X)
	  + small_multiply(lo_b_277X, hi_a_278X);
        c_282X = lo_c_280X + (mid_c_281X << 15);
        if ((0 < hi_a_278X)) {
          if ((0 < hi_b_279X)) {
            goto L20703;}
          else {
            goto L20708;}}
        else {
          goto L20708;}
       L20708: {
        if ((536870911 < lo_c_280X)) {
          goto L20703;}
        else {
          if ((65535 < mid_c_281X)) {
            goto L20703;}
          else {
            if ((a_272X < 0)) {
              if ((b_273X < 0)) {
                goto L20718;}
              else {
                goto L20711;}}
            else {
              if ((b_273X < 0)) {
                goto L20711;}
              else {
                goto L20718;}}
           L20711: {
            if ((536870912 < c_282X)) {
              merged_arg1K0 = 0;
              merged_arg0K1 = arg2_270X;
              merged_arg0K2 = x_271X;
              goto raise_exception2;}
            else {
              RSvalS = ((0 - c_282X) << 2);
              goto START;}}
           L20718: {
            if ((536870911 < c_282X)) {
              merged_arg1K0 = 0;
              merged_arg0K1 = arg2_270X;
              merged_arg0K2 = x_271X;
              goto raise_exception2;}
            else {
              RSvalS = (c_282X << 2);
              goto START;}}}}}
       L20703: {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_270X;
        merged_arg0K2 = x_271X;
        goto raise_exception2;}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_270X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 48 : {
      long arg2_283X;
      RSstackS = (4 + RSstackS);
      arg2_283X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_283X | RSvalS)))) {
        long x_284X;
        long z_285X;
        x_284X = RSvalS;
        z_285X = (arg2_283X >> 2) - (x_284X >> 2);
        if ((536870911 < z_285X)) {
          goto L20508;}
        else {
          if ((z_285X < -536870912)) {
            goto L20508;}
          else {
            RSvalS = (z_285X << 2);
            goto START;}}
       L20508: {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_283X;
        merged_arg0K2 = x_284X;
        goto raise_exception2;}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_283X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 49 : {
      long arg2_286X;
      RSstackS = (4 + RSstackS);
      arg2_286X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_286X | RSvalS)))) {
        long x_287X;
        x_287X = RSvalS;
        if ((0 == x_287X)) {
          merged_arg1K0 = 0;
          merged_arg0K1 = arg2_286X;
          merged_arg0K2 = x_287X;
          goto raise_exception2;}
        else {
          long a_288X;
          long b_289X;
          a_288X = arg2_286X >> 2;
          b_289X = x_287X >> 2;
          if ((a_288X < 0)) {
            arg0K0 = (0 - a_288X);
            goto L20279;}
          else {
            arg0K0 = a_288X;
            goto L20279;}
         L20279: {
          long a_290X = arg0K0;
          if ((b_289X < 0)) {
            arg0K0 = (0 - b_289X);
            goto L20281;}
          else {
            arg0K0 = b_289X;
            goto L20281;}
         L20281: {
          long b_291X = arg0K0;
          long c_292X;
          c_292X = a_290X / b_291X;
          if ((a_288X < 0)) {
            if ((b_289X < 0)) {
              goto L20284;}
            else {
              arg0K0 = ((0 - c_292X) << 2);
              goto L20236;}}
          else {
            if ((b_289X < 0)) {
              arg0K0 = ((0 - c_292X) << 2);
              goto L20236;}
            else {
              goto L20284;}}
         L20284: {
          if ((536870911 < c_292X)) {
            merged_arg1K0 = 0;
            merged_arg0K1 = arg2_286X;
            merged_arg0K2 = x_287X;
            goto raise_exception2;}
          else {
            arg0K0 = (c_292X << 2);
            goto L20236;}}}}}
       L20236: {
        long q_293X = arg0K0;
        if ((0 == x_287X)) {
          merged_arg1K0 = 0;
          merged_arg0K1 = arg2_286X;
          merged_arg0K2 = x_287X;
          goto raise_exception2;}
        else {
          long a_294X;
          long b_295X;
          a_294X = arg2_286X >> 2;
          b_295X = x_287X >> 2;
          if ((a_294X < 0)) {
            arg0K0 = (0 - a_294X);
            goto L20243;}
          else {
            arg0K0 = a_294X;
            goto L20243;}
         L20243: {
          long a_296X = arg0K0;
          if ((b_295X < 0)) {
            arg0K0 = (0 - b_295X);
            goto L20245;}
          else {
            arg0K0 = b_295X;
            goto L20245;}
         L20245: {
          long b_297X = arg0K0;
          long c_298X;
          c_298X = a_296X % b_297X;
          if ((a_294X < 0)) {
            arg0K0 = (0 - c_298X);
            goto L20249;}
          else {
            arg0K0 = c_298X;
            goto L20249;}
         L20249: {
          long n_299X = arg0K0;
          if ((0 == (n_299X << 2))) {
            RSvalS = q_293X;
            goto START;}
          else {
            merged_arg1K0 = 0;
            merged_arg0K1 = arg2_286X;
            merged_arg0K2 = x_287X;
            goto raise_exception2;}}}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_286X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 50 : {
      long arg2_300X;
      RSstackS = (4 + RSstackS);
      arg2_300X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_300X | RSvalS)))) {
        long x_301X;
        x_301X = RSvalS;
        if ((arg2_300X == x_301X)) {
          arg1K0 = 5;
          goto L19304;}
        else {
          arg1K0 = 1;
          goto L19304;}
       L19304: {
        char val_302X = arg1K0;
        RSvalS = ((long)val_302X);
        goto START;}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_300X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 51 : {
      long arg2_303X;
      RSstackS = (4 + RSstackS);
      arg2_303X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_303X | RSvalS)))) {
        long x_304X;
        x_304X = RSvalS;
        if ((arg2_303X < x_304X)) {
          arg1K0 = 5;
          goto L19159;}
        else {
          arg1K0 = 1;
          goto L19159;}
       L19159: {
        char val_305X = arg1K0;
        RSvalS = ((long)val_305X);
        goto START;}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_303X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 52 : {
      long arg2_306X;
      RSstackS = (4 + RSstackS);
      arg2_306X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_306X | RSvalS)))) {
        long x_307X;
        x_307X = RSvalS;
        if ((0 == x_307X)) {
          merged_arg1K0 = 0;
          merged_arg0K1 = arg2_306X;
          merged_arg0K2 = x_307X;
          goto raise_exception2;}
        else {
          long a_308X;
          long b_309X;
          a_308X = arg2_306X >> 2;
          b_309X = x_307X >> 2;
          if ((a_308X < 0)) {
            arg0K0 = (0 - a_308X);
            goto L20014;}
          else {
            arg0K0 = a_308X;
            goto L20014;}
         L20014: {
          long a_310X = arg0K0;
          if ((b_309X < 0)) {
            arg0K0 = (0 - b_309X);
            goto L20016;}
          else {
            arg0K0 = b_309X;
            goto L20016;}
         L20016: {
          long b_311X = arg0K0;
          long c_312X;
          c_312X = a_310X / b_311X;
          if ((a_308X < 0)) {
            if ((b_309X < 0)) {
              goto L20022;}
            else {
              goto L20019;}}
          else {
            if ((b_309X < 0)) {
              goto L20019;}
            else {
              goto L20022;}}
         L20019: {
          RSvalS = ((0 - c_312X) << 2);
          goto START;}
         L20022: {
          if ((536870911 < c_312X)) {
            merged_arg1K0 = 0;
            merged_arg0K1 = arg2_306X;
            merged_arg0K2 = x_307X;
            goto raise_exception2;}
          else {
            RSvalS = (c_312X << 2);
            goto START;}}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_306X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 53 : {
      long arg2_313X;
      RSstackS = (4 + RSstackS);
      arg2_313X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_313X | RSvalS)))) {
        long x_314X;
        x_314X = RSvalS;
        if ((0 == x_314X)) {
          merged_arg1K0 = 0;
          merged_arg0K1 = arg2_313X;
          merged_arg0K2 = x_314X;
          goto raise_exception2;}
        else {
          long a_315X;
          long b_316X;
          a_315X = arg2_313X >> 2;
          b_316X = x_314X >> 2;
          if ((a_315X < 0)) {
            arg0K0 = (0 - a_315X);
            goto L19822;}
          else {
            arg0K0 = a_315X;
            goto L19822;}
         L19822: {
          long a_317X = arg0K0;
          if ((b_316X < 0)) {
            arg0K0 = (0 - b_316X);
            goto L19824;}
          else {
            arg0K0 = b_316X;
            goto L19824;}
         L19824: {
          long b_318X = arg0K0;
          long c_319X;
          c_319X = a_317X % b_318X;
          if ((a_315X < 0)) {
            arg0K0 = (0 - c_319X);
            goto L19828;}
          else {
            arg0K0 = c_319X;
            goto L19828;}
         L19828: {
          long n_320X = arg0K0;
          RSvalS = (n_320X << 2);
          goto START;}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_313X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 54 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = RSvalS;
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 55 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = RSvalS;
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 56 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = 4;
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 57 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = RSvalS;
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 58 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = 0;
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 59 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 60 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 61 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 62 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 63 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 64 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 65 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 66 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 67 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 68 : {
      if ((0 == (3 & RSvalS))) {
        long x_321X;
        x_321X = RSvalS;
        if ((x_321X < 0)) {
          merged_arg1K0 = 0;
          merged_arg0K1 = x_321X;
          goto raise_exception1;}
        else {
          RSvalS = 0;
          goto START;}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 69 : {
      if ((0 == (3 & RSvalS))) {
        long x_322X;
        x_322X = RSvalS;
        if ((x_322X < 0)) {
          arg0K0 = (0 - x_322X);
          goto L24500;}
        else {
          arg0K0 = x_322X;
          goto L24500;}
       L24500: {
        long r_323X = arg0K0;
        if ((536870911 < r_323X)) {
          merged_arg1K0 = 0;
          merged_arg0K1 = x_322X;
          goto raise_exception1;}
        else {
          RSvalS = (r_323X << 2);
          goto START;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 70 : {
      long arg2_324X;
      RSstackS = (4 + RSstackS);
      arg2_324X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_324X | RSvalS)))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_324X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_324X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 71 : {
      long arg2_325X;
      RSstackS = (4 + RSstackS);
      arg2_325X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_325X | RSvalS)))) {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_325X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_325X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 72 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = ((~ (RSvalS >> 2)) << 2);
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 73 : {
      long arg2_326X;
      RSstackS = (4 + RSstackS);
      arg2_326X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_326X | RSvalS)))) {
        RSvalS = (((arg2_326X >> 2) & (RSvalS >> 2)) << 2);
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_326X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 74 : {
      long arg2_327X;
      RSstackS = (4 + RSstackS);
      arg2_327X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_327X | RSvalS)))) {
        RSvalS = (((arg2_327X >> 2) | (RSvalS >> 2)) << 2);
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_327X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 75 : {
      long arg2_328X;
      RSstackS = (4 + RSstackS);
      arg2_328X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_328X | RSvalS)))) {
        RSvalS = (((arg2_328X >> 2) ^ (RSvalS >> 2)) << 2);
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_328X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 76 : {
      long arg2_329X;
      RSstackS = (4 + RSstackS);
      arg2_329X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & (arg2_329X | RSvalS)))) {
        long x_330X;
        long count_331X;
        long value_332X;
        x_330X = RSvalS;
        count_331X = x_330X >> 2;
        value_332X = arg2_329X >> 2;
        if ((count_331X < 0)) {
          long x_333X;
          PS_SHIFT_RIGHT(value_332X, (0 - count_331X), x_333X)
          RSvalS = (x_333X << 2);
          goto START;}
        else {
          long x_334X;
          long result_335X;
          long x_336X;
          PS_SHIFT_LEFT(value_332X, count_331X, x_334X)
          result_335X = (x_334X << 2) >> 2;
          PS_SHIFT_RIGHT(result_335X, count_331X, x_336X)
          if ((value_332X == x_336X)) {
            if ((value_332X < 0)) {
              if ((result_335X < 0)) {
                goto L19625;}
              else {
                goto L19622;}}
            else {
              if ((result_335X < 0)) {
                goto L19622;}
              else {
                goto L19625;}}}
          else {
            goto L19622;}
         L19622: {
          merged_arg1K0 = 0;
          merged_arg0K1 = arg2_329X;
          merged_arg0K2 = x_330X;
          goto raise_exception2;}
         L19625: {
          RSvalS = (result_335X << 2);
          goto START;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_329X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 77 : {
      long x_337X;
      x_337X = RSvalS;
      if ((9 == (255 & x_337X))) {
        arg1K0 = 5;
        goto L24366;}
      else {
        arg1K0 = 1;
        goto L24366;}
     L24366: {
      char val_338X = arg1K0;
      RSvalS = ((long)val_338X);
      goto START;}}
      break;
    case 78 : {
      long arg2_339X;
      RSstackS = (4 + RSstackS);
      arg2_339X = *((long*)((unsigned char*)RSstackS));
      if ((9 == (255 & arg2_339X))) {
        if ((9 == (255 & RSvalS))) {
          long d_340X;
          d_340X = RSvalS;
          if (((((char)(arg2_339X >> 8))) == (((char)(d_340X >> 8))))) {
            arg1K0 = 5;
            goto L18341;}
          else {
            arg1K0 = 1;
            goto L18341;}
         L18341: {
          char val_341X = arg1K0;
          RSvalS = ((long)val_341X);
          goto START;}}
        else {
          goto L18332;}}
      else {
        goto L18332;}
     L18332: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg2_339X;
      merged_arg0K2 = RSvalS;
      goto raise_exception2;}}
      break;
    case 79 : {
      long arg2_342X;
      RSstackS = (4 + RSstackS);
      arg2_342X = *((long*)((unsigned char*)RSstackS));
      if ((9 == (255 & arg2_342X))) {
        if ((9 == (255 & RSvalS))) {
          long d_343X;
          d_343X = RSvalS;
          if (((((char)(arg2_342X >> 8))) < (((char)(d_343X >> 8))))) {
            arg1K0 = 5;
            goto L18196;}
          else {
            arg1K0 = 1;
            goto L18196;}
         L18196: {
          char val_344X = arg1K0;
          RSvalS = ((long)val_344X);
          goto START;}}
        else {
          goto L18187;}}
      else {
        goto L18187;}
     L18187: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg2_342X;
      merged_arg0K2 = RSvalS;
      goto raise_exception2;}}
      break;
    case 80 : {
      if ((9 == (255 & RSvalS))) {
	/* JAR PATCH: char --> unsigned char */
        RSvalS = (((long)((((unsigned char)(RSvalS >> 8))))) << 2);
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 81 : {
      if ((0 == (3 & RSvalS))) {
        long x_345X;
        x_345X = RSvalS >> 2;
        if ((255 < x_345X)) {
          goto L24176;}
        else {
          if ((x_345X < 0)) {
            goto L24176;}
          else {
            RSvalS = (9 + (((long)((((char)x_345X)))) << 8));
            goto START;}}
       L24176: {
        merged_arg1K0 = 0;
        merged_arg0K1 = (x_345X << 2);
        goto raise_exception1;}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 82 : {
      long x_346X;
      x_346X = RSvalS;
      if ((21 == x_346X)) {
        arg1K0 = 5;
        goto L24099;}
      else {
        arg1K0 = 1;
        goto L24099;}
     L24099: {
      char val_347X = arg1K0;
      RSvalS = ((long)val_347X);
      goto START;}}
      break;
    case 83 : {
      unsigned char b_348X;
      unsigned char b_349X;
      long len_350X;
      long new_351X;
      merged_arg0K0 = (((long)(*((unsigned char*)((unsigned char*)RScode_pointerS)))) << 2);
      ensure_space_return_tag = 5;
      goto ensure_space;
     ensure_space_return_5:
      b_348X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      b_349X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      len_350X = ((long)b_348X) << 2;
      *((long*)((unsigned char*)ShpS)) = (2 + (((len_350X << 6) + ((long)b_349X)) << 2));
      ShpS = (4 + ShpS);
      new_351X = 3 + ShpS;
      ShpS = (ShpS + (-4 & (3 + len_350X)));
      if ((0 < len_350X)) {
        *((long*)((unsigned char*)(-4 + ShpS))) = 0;
        arg0K0 = new_351X;
        goto L26625;}
      else {
        arg0K0 = new_351X;
        goto L26625;}
     L26625: {
      long new_352X = arg0K0;
      if ((((long)b_348X) < 1)) {
        goto L26665;}
      else {
        *((long*)((unsigned char*)(-7 + (new_352X + (((long)b_348X) << 2))))) = RSvalS;
        arg0K0 = (-2 + ((long)b_348X));
        goto L26640;
       L26640: {
        long i_353X = arg0K0;
        if ((i_353X < 0)) {
          goto L26665;}
        else {
          RSstackS = (4 + RSstackS);
          *((long*)((unsigned char*)((-3 + new_352X) + (i_353X << 2)))) = (*((long*)((unsigned char*)RSstackS)));
          arg0K0 = (-1 + i_353X);
          goto L26640;}}}
     L26665: {
      RSvalS = new_352X;
      goto START;}}}
      break;
    case 84 : {
      long arg2_354X;
      long init_355X;
      unsigned char b_356X;
      RSstackS = (4 + RSstackS);
      arg2_354X = *((long*)((unsigned char*)RSstackS));
      init_355X = RSvalS;
      b_356X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & arg2_354X))) {
        long len_357X;
        len_357X = arg2_354X >> 2;
        if ((len_357X < 0)) {
          goto L30976;}
        else {
          unsigned char v_358X;
          v_358X = (ShpS + (len_357X << 4)) < SlimitS;
          if (v_358X) {
            arg0K0 = len_357X;
            arg0K1 = init_355X;
            goto L30955;}
          else {
            long init_359X;
            unsigned char v_360X;
            merged_arg0K0 = init_355X;
            collect_saving_temp_return_tag = 0;
            goto collect_saving_temp;
           collect_saving_temp_return_0:
            init_359X = collect_saving_temp_return_value;
            v_360X = (ShpS + (len_357X << 4)) < SlimitS;
            if (v_360X) {
              arg0K0 = len_357X;
              arg0K1 = init_359X;
              goto L30955;}
            else {
              goto L30976;}}}}
      else {
        goto L30976;}
     L30976: {
      merged_arg1K0 = 1;
      merged_arg0K1 = (((long)b_356X) << 2);
      merged_arg0K2 = arg2_354X;
      merged_arg0K3 = init_355X;
      goto raise_exception3;}
     L30955: {
      long len_361X = arg0K0;
      long init_362X = arg0K1;
      long len_363X;
      long new_364X;
      len_363X = len_361X << 2;
      *((long*)((unsigned char*)ShpS)) = (2 + (((len_363X << 6) + ((long)b_356X)) << 2));
      ShpS = (4 + ShpS);
      new_364X = 3 + ShpS;
      ShpS = (ShpS + (-4 & (3 + len_363X)));
      if ((0 < len_363X)) {
        *((long*)((unsigned char*)(-4 + ShpS))) = 0;
        arg0K0 = new_364X;
        goto L30961;}
      else {
        arg0K0 = new_364X;
        goto L30961;}
     L30961: {
      long val_365X = arg0K0;
      arg0K0 = (-1 + len_361X);
      goto L30965;
     L30965: {
      long i_366X = arg0K0;
      if ((i_366X < 0)) {
        RSvalS = val_365X;
        goto START;}
      else {
        *((long*)((unsigned char*)((-3 + val_365X) + (i_366X << 2)))) = init_362X;
        arg0K0 = (-1 + i_366X);
        goto L30965;}}}}}
      break;
    case 85 : {
      long arg2_367X;
      long init_368X;
      unsigned char b_369X;
      RSstackS = (4 + RSstackS);
      arg2_367X = *((long*)((unsigned char*)RSstackS));
      init_368X = RSvalS;
      b_369X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & (arg2_367X | init_368X)))) {
        long len_370X;
        len_370X = arg2_367X >> 2;
        if ((len_370X < 0)) {
          goto L30727;}
        else {
          unsigned char v_371X;
          v_371X = (ShpS + (len_370X << 2)) < SlimitS;
          if (v_371X) {
            arg0K0 = len_370X;
            arg0K1 = init_368X;
            goto L30706;}
          else {
            long init_372X;
            unsigned char v_373X;
            merged_arg0K0 = init_368X;
            collect_saving_temp_return_tag = 1;
            goto collect_saving_temp;
           collect_saving_temp_return_1:
            init_372X = collect_saving_temp_return_value;
            v_373X = (ShpS + (len_370X << 2)) < SlimitS;
            if (v_373X) {
              arg0K0 = len_370X;
              arg0K1 = init_372X;
              goto L30706;}
            else {
              goto L30727;}}}}
      else {
        goto L30727;}
     L30727: {
      merged_arg1K0 = 1;
      merged_arg0K1 = (((long)b_369X) << 2);
      merged_arg0K2 = arg2_367X;
      merged_arg0K3 = init_368X;
      goto raise_exception3;}
     L30706: {
      long len_374X = arg0K0;
      long init_375X = arg0K1;
      long new_376X;
      *((long*)((unsigned char*)ShpS)) = (2 + (((len_374X << 6) + ((long)b_369X)) << 2));
      ShpS = (4 + ShpS);
      new_376X = 3 + ShpS;
      ShpS = (ShpS + (-4 & (3 + len_374X)));
      if ((0 < len_374X)) {
        *((long*)((unsigned char*)(-4 + ShpS))) = 0;
        arg0K0 = new_376X;
        goto L30712;}
      else {
        arg0K0 = new_376X;
        goto L30712;}
     L30712: {
      long val_377X = arg0K0;
      arg0K0 = (-1 + len_374X);
      goto L30716;
     L30716: {
      long i_378X = arg0K0;
      if ((i_378X < 0)) {
        RSvalS = val_377X;
        goto START;}
      else {
        *((unsigned char*)((unsigned char*)((-3 + val_377X) + i_378X))) = (init_375X >> 2);
        arg0K0 = (-1 + i_378X);
        goto L30716;}}}}}
      break;
    case 86 : {
      long arg2_379X;
      long init_380X;
      unsigned char b_381X;
      RSstackS = (4 + RSstackS);
      arg2_379X = *((long*)((unsigned char*)RSstackS));
      init_380X = RSvalS;
      b_381X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & arg2_379X))) {
        if ((9 == (255 & init_380X))) {
          long len_382X;
          len_382X = arg2_379X >> 2;
          if ((len_382X < 0)) {
            goto L30476;}
          else {
            unsigned char v_383X;
            v_383X = (ShpS + (len_382X << 2)) < SlimitS;
            if (v_383X) {
              arg0K0 = len_382X;
              arg0K1 = init_380X;
              goto L30455;}
            else {
              long init_384X;
              unsigned char v_385X;
              merged_arg0K0 = init_380X;
              collect_saving_temp_return_tag = 2;
              goto collect_saving_temp;
             collect_saving_temp_return_2:
              init_384X = collect_saving_temp_return_value;
              v_385X = (ShpS + (len_382X << 2)) < SlimitS;
              if (v_385X) {
                arg0K0 = len_382X;
                arg0K1 = init_384X;
                goto L30455;}
              else {
                goto L30476;}}}}
        else {
          goto L30476;}}
      else {
        goto L30476;}
     L30476: {
      merged_arg1K0 = 1;
      merged_arg0K1 = (((long)b_381X) << 2);
      merged_arg0K2 = arg2_379X;
      merged_arg0K3 = init_380X;
      goto raise_exception3;}
     L30455: {
      long len_386X = arg0K0;
      long init_387X = arg0K1;
      long new_388X;
      *((long*)((unsigned char*)ShpS)) = (2 + (((len_386X << 6) + ((long)b_381X)) << 2));
      ShpS = (4 + ShpS);
      new_388X = 3 + ShpS;
      ShpS = (ShpS + (-4 & (3 + len_386X)));
      if ((0 < len_386X)) {
        *((long*)((unsigned char*)(-4 + ShpS))) = 0;
        arg0K0 = new_388X;
        goto L30461;}
      else {
        arg0K0 = new_388X;
        goto L30461;}
     L30461: {
      long val_389X = arg0K0;
      arg0K0 = (-1 + len_386X);
      goto L30465;
     L30465: {
      long i_390X = arg0K0;
      if ((i_390X < 0)) {
        RSvalS = val_389X;
        goto START;}
      else {
        *((unsigned char*)((unsigned char*)((-3 + val_389X) + i_390X))) = ((((char)(init_387X >> 8))));
        arg0K0 = (-1 + i_390X);
        goto L30465;}}}}}
      break;
    case 87 : {
      long x_391X;
      unsigned char b_392X;
      x_391X = RSvalS;
      b_392X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_391X))) {
        long x_393X;
        x_393X = 31 & ((*((long*)((unsigned char*)(-7 + x_391X)))) >> 2);
        if ((x_393X == ((long)b_392X))) {
          arg1K0 = 5;
          goto L24000;}
        else {
          arg1K0 = 1;
          goto L24000;}}
      else {
        arg1K0 = 1;
        goto L24000;}
     L24000: {
      char val_394X = arg1K0;
      RSvalS = ((long)val_394X);
      goto START;}}
      break;
    case 88 : {
      long x_395X;
      unsigned char b_396X;
      x_395X = RSvalS;
      b_396X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_395X))) {
        long x_397X;
        x_397X = 31 & ((*((long*)((unsigned char*)(-7 + x_395X)))) >> 2);
        if ((x_397X == ((long)b_396X))) {
          long x_398X;
          x_398X = (3 + ((*((long*)((unsigned char*)(-7 + x_395X)))) >> 8)) >> 2;
          RSvalS = (x_398X << 2);
          goto START;}
        else {
          goto L23881;}}
      else {
        goto L23881;}
     L23881: {
      merged_arg1K0 = 1;
      merged_arg0K1 = x_395X;
      merged_arg0K2 = (((long)b_396X) << 2);
      goto raise_exception2;}}
      break;
    case 89 : {
      long x_399X;
      unsigned char b_400X;
      x_399X = RSvalS;
      b_400X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_399X))) {
        long x_401X;
        x_401X = 31 & ((*((long*)((unsigned char*)(-7 + x_399X)))) >> 2);
        if ((x_401X == ((long)b_400X))) {
          long x_402X;
          x_402X = (*((long*)((unsigned char*)(-7 + x_399X)))) >> 8;
          RSvalS = (x_402X << 2);
          goto START;}
        else {
          goto L23765;}}
      else {
        goto L23765;}
     L23765: {
      merged_arg1K0 = 1;
      merged_arg0K1 = x_399X;
      merged_arg0K2 = (((long)b_400X) << 2);
      goto raise_exception2;}}
      break;
    case 90 : {
      long x_403X;
      unsigned char b_404X;
      unsigned char b_405X;
      x_403X = RSvalS;
      b_404X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      b_405X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_403X))) {
        long x_406X;
        x_406X = 31 & ((*((long*)((unsigned char*)(-7 + x_403X)))) >> 2);
        if ((x_406X == ((long)b_404X))) {
          RSvalS = (*((long*)((unsigned char*)((-3 + x_403X) + (((long)b_405X) << 2)))));
          goto START;}
        else {
          goto L28842;}}
      else {
        goto L28842;}
     L28842: {
      merged_arg1K0 = 2;
      merged_arg0K1 = x_403X;
      merged_arg0K2 = (((long)b_404X) << 2);
      merged_arg0K3 = (((long)b_405X) << 2);
      goto raise_exception3;}}
      break;
    case 91 : {
      long arg2_407X;
      long x_408X;
      unsigned char b_409X;
      unsigned char b_410X;
      RSstackS = (4 + RSstackS);
      arg2_407X = *((long*)((unsigned char*)RSstackS));
      x_408X = RSvalS;
      b_409X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      b_410X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & arg2_407X))) {
        long x_411X;
        x_411X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_407X)))) >> 2);
        if ((x_411X == ((long)b_409X))) {
          if ((3 == (3 & arg2_407X))) {
            if ((0 == (128 & (*((long*)((unsigned char*)(-7 + arg2_407X))))))) {
              *((long*)((unsigned char*)((-3 + arg2_407X) + (((long)b_410X) << 2)))) = x_408X;
              RSvalS = 13;
              goto START;}
            else {
              goto L32223;}}
          else {
            goto L32223;}}
        else {
          goto L32223;}}
      else {
        goto L32223;}
     L32223: {
      merged_arg1K0 = 2;
      merged_arg0K1 = arg2_407X;
      merged_arg0K2 = (((long)b_409X) << 2);
      merged_arg0K3 = (((long)b_410X) << 2);
      merged_arg0K4 = x_408X;
      goto raise_exception4;}}
      break;
    case 92 : {
      long arg2_412X;
      long x_413X;
      unsigned char b_414X;
      RSstackS = (4 + RSstackS);
      arg2_412X = *((long*)((unsigned char*)RSstackS));
      x_413X = RSvalS;
      b_414X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & x_413X))) {
        long index_415X;
        index_415X = x_413X >> 2;
        if ((3 == (3 & arg2_412X))) {
          long x_416X;
          x_416X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_412X)))) >> 2);
          if ((x_416X == ((long)b_414X))) {
            long len_417X;
            len_417X = (3 + ((*((long*)((unsigned char*)(-7 + arg2_412X)))) >> 8)) >> 2;
            if ((index_415X < 0)) {
              goto L29490;}
            else {
              if ((index_415X < len_417X)) {
                RSvalS = (*((long*)((unsigned char*)((-3 + arg2_412X) + (index_415X << 2)))));
                goto START;}
              else {
                goto L29490;}}}
          else {
            goto L29490;}}
        else {
          goto L29490;}}
      else {
        goto L29490;}
     L29490: {
      merged_arg1K0 = 1;
      merged_arg0K1 = arg2_412X;
      merged_arg0K2 = (((long)b_414X) << 2);
      merged_arg0K3 = x_413X;
      goto raise_exception3;}}
      break;
    case 93 : {
      long arg2_418X;
      long x_419X;
      unsigned char b_420X;
      RSstackS = (4 + RSstackS);
      arg2_418X = *((long*)((unsigned char*)RSstackS));
      x_419X = RSvalS;
      b_420X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & x_419X))) {
        long index_421X;
        index_421X = x_419X >> 2;
        if ((3 == (3 & arg2_418X))) {
          long x_422X;
          x_422X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_418X)))) >> 2);
          if ((x_422X == ((long)b_420X))) {
            long len_423X;
            len_423X = (*((long*)((unsigned char*)(-7 + arg2_418X)))) >> 8;
            if ((index_421X < 0)) {
              goto L29303;}
            else {
              if ((index_421X < len_423X)) {
                RSvalS = (((long)(*((unsigned char*)((unsigned char*)((-3 + arg2_418X) + index_421X))))) << 2);
                goto START;}
              else {
                goto L29303;}}}
          else {
            goto L29303;}}
        else {
          goto L29303;}}
      else {
        goto L29303;}
     L29303: {
      merged_arg1K0 = 1;
      merged_arg0K1 = arg2_418X;
      merged_arg0K2 = (((long)b_420X) << 2);
      merged_arg0K3 = x_419X;
      goto raise_exception3;}}
      break;
    case 94 : {
      long arg2_424X;
      long x_425X;
      unsigned char b_426X;
      RSstackS = (4 + RSstackS);
      arg2_424X = *((long*)((unsigned char*)RSstackS));
      x_425X = RSvalS;
      b_426X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & x_425X))) {
        long index_427X;
        index_427X = x_425X >> 2;
        if ((3 == (3 & arg2_424X))) {
          long x_428X;
          x_428X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_424X)))) >> 2);
          if ((x_428X == ((long)b_426X))) {
            long len_429X;
            len_429X = (*((long*)((unsigned char*)(-7 + arg2_424X)))) >> 8;
            if ((index_427X < 0)) {
              goto L29116;}
            else {
              if ((index_427X < len_429X)) {
                char x_430X;
                x_430X = (*((unsigned char*)((unsigned char*)((-3 + arg2_424X) + index_427X))));
                RSvalS = (9 + (((long)(x_430X)) << 8));
                goto START;}
              else {
                goto L29116;}}}
          else {
            goto L29116;}}
        else {
          goto L29116;}}
      else {
        goto L29116;}
     L29116: {
      merged_arg1K0 = 1;
      merged_arg0K1 = arg2_424X;
      merged_arg0K2 = (((long)b_426X) << 2);
      merged_arg0K3 = x_425X;
      goto raise_exception3;}}
      break;
    case 95 : {
      long arg2_431X;
      long arg3_432X;
      long value_433X;
      unsigned char b_434X;
      RSstackS = (4 + RSstackS);
      arg2_431X = *((long*)((unsigned char*)RSstackS));
      RSstackS = (4 + RSstackS);
      arg3_432X = *((long*)((unsigned char*)RSstackS));
      value_433X = RSvalS;
      b_434X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & arg3_432X))) {
        long x_435X;
        x_435X = 31 & ((*((long*)((unsigned char*)(-7 + arg3_432X)))) >> 2);
        if ((x_435X == ((long)b_434X))) {
          if ((0 == (3 & arg2_431X))) {
            long len_436X;
            long index_437X;
            len_436X = (3 + ((*((long*)((unsigned char*)(-7 + arg3_432X)))) >> 8)) >> 2;
            index_437X = arg2_431X >> 2;
            if ((index_437X < 0)) {
              goto L31992;}
            else {
              if ((index_437X < len_436X)) {
                if ((3 == (3 & arg3_432X))) {
                  if ((0 == (128 & (*((long*)((unsigned char*)(-7 + arg3_432X))))))) {
                    *((long*)((unsigned char*)((-3 + arg3_432X) + (-4 & arg2_431X)))) = value_433X;
                    RSvalS = 13;
                    goto START;}
                  else {
                    goto L31992;}}
                else {
                  goto L31992;}}
              else {
                goto L31992;}}}
          else {
            goto L31992;}}
        else {
          goto L31992;}}
      else {
        goto L31992;}
     L31992: {
      merged_arg1K0 = 1;
      merged_arg0K1 = arg3_432X;
      merged_arg0K2 = (((long)b_434X) << 2);
      merged_arg0K3 = arg2_431X;
      merged_arg0K4 = value_433X;
      goto raise_exception4;}}
      break;
    case 96 : {
      long arg2_438X;
      long arg3_439X;
      long value_440X;
      unsigned char b_441X;
      RSstackS = (4 + RSstackS);
      arg2_438X = *((long*)((unsigned char*)RSstackS));
      RSstackS = (4 + RSstackS);
      arg3_439X = *((long*)((unsigned char*)RSstackS));
      value_440X = RSvalS;
      b_441X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & arg3_439X))) {
        long x_442X;
        x_442X = 31 & ((*((long*)((unsigned char*)(-7 + arg3_439X)))) >> 2);
        if ((x_442X == ((long)b_441X))) {
          if ((0 == (3 & arg2_438X))) {
            long len_443X;
            long index_444X;
            len_443X = (*((long*)((unsigned char*)(-7 + arg3_439X)))) >> 8;
            index_444X = arg2_438X >> 2;
            if ((index_444X < 0)) {
              goto L31723;}
            else {
              if ((index_444X < len_443X)) {
                if ((0 == (3 & value_440X))) {
                  if ((3 == (3 & arg3_439X))) {
                    if ((0 == (128 & (*((long*)((unsigned char*)(-7 + arg3_439X))))))) {
                      *((unsigned char*)((unsigned char*)((-3 + arg3_439X) + (arg2_438X >> 2)))) = (value_440X >> 2);
                      RSvalS = 13;
                      goto START;}
                    else {
                      goto L31723;}}
                  else {
                    goto L31723;}}
                else {
                  goto L31723;}}
              else {
                goto L31723;}}}
          else {
            goto L31723;}}
        else {
          goto L31723;}}
      else {
        goto L31723;}
     L31723: {
      merged_arg1K0 = 1;
      merged_arg0K1 = arg3_439X;
      merged_arg0K2 = (((long)b_441X) << 2);
      merged_arg0K3 = arg2_438X;
      merged_arg0K4 = value_440X;
      goto raise_exception4;}}
      break;
    case 97 : {
      long arg2_445X;
      long arg3_446X;
      long value_447X;
      unsigned char b_448X;
      RSstackS = (4 + RSstackS);
      arg2_445X = *((long*)((unsigned char*)RSstackS));
      RSstackS = (4 + RSstackS);
      arg3_446X = *((long*)((unsigned char*)RSstackS));
      value_447X = RSvalS;
      b_448X = *((unsigned char*)((unsigned char*)RScode_pointerS));
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & arg3_446X))) {
        long x_449X;
        x_449X = 31 & ((*((long*)((unsigned char*)(-7 + arg3_446X)))) >> 2);
        if ((x_449X == ((long)b_448X))) {
          if ((0 == (3 & arg2_445X))) {
            long len_450X;
            long index_451X;
            len_450X = (*((long*)((unsigned char*)(-7 + arg3_446X)))) >> 8;
            index_451X = arg2_445X >> 2;
            if ((index_451X < 0)) {
              goto L31454;}
            else {
              if ((index_451X < len_450X)) {
                if ((9 == (255 & value_447X))) {
                  if ((3 == (3 & arg3_446X))) {
                    if ((0 == (128 & (*((long*)((unsigned char*)(-7 + arg3_446X))))))) {
                      *((unsigned char*)((unsigned char*)((-3 + arg3_446X) + (arg2_445X >> 2)))) = ((((char)(value_447X >> 8))));
                      RSvalS = 13;
                      goto START;}
                    else {
                      goto L31454;}}
                  else {
                    goto L31454;}}
                else {
                  goto L31454;}}
              else {
                goto L31454;}}}
          else {
            goto L31454;}}
        else {
          goto L31454;}}
      else {
        goto L31454;}
     L31454: {
      merged_arg1K0 = 1;
      merged_arg0K1 = arg3_446X;
      merged_arg0K2 = (((long)b_448X) << 2);
      merged_arg0K3 = arg2_445X;
      merged_arg0K4 = value_447X;
      goto raise_exception4;}}
      break;
    case 98 : {
      long obj_452X;
      obj_452X = RSvalS;
      if ((3 == (3 & obj_452X))) {
        long x_453X;
        x_453X = 31 & ((*((long*)((unsigned char*)(-7 + obj_452X)))) >> 2);
        if ((4 == x_453X)) {
          long x_454X;
          long descriptor_455X;
          x_454X = RSvalS;
          descriptor_455X = *((long*)((unsigned char*)(-3 + x_454X)));
          if ((17 == (255 & descriptor_455X))) {
            if ((529 == (*((long*)((unsigned char*)(-3 + x_454X)))))) {
              arg1K0 = 5;
              goto L23629;}
            else {
              arg1K0 = 1;
              goto L23629;}}
          else {
            arg1K0 = 5;
            goto L23629;}
         L23629: {
          char val_456X = arg1K0;
          RSvalS = ((long)val_456X);
          goto START;}}
        else {
          goto L23620;}}
      else {
        goto L23620;}
     L23620: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 99 : {
      long arg2_457X;
      RSstackS = (4 + RSstackS);
      arg2_457X = *((long*)((unsigned char*)RSstackS));
      if ((3 == (3 & arg2_457X))) {
        long x_458X;
        x_458X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_457X)))) >> 2);
        if ((4 == x_458X)) {
          long x_459X;
          x_459X = RSvalS;
          if ((1 == x_459X)) {
            goto L17938;}
          else {
            if ((5 == x_459X)) {
              goto L17938;}
            else {
              goto L17970;}}}
        else {
          goto L17970;}}
      else {
        goto L17970;}
     L17970: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg2_457X;
      merged_arg0K2 = RSvalS;
      goto raise_exception2;}
     L17938: {
      if ((1 == RSvalS)) {
        *((long*)((unsigned char*)(-3 + arg2_457X))) = 273;
        goto L17967;}
      else {
        if ((17 == (255 & (*((long*)((unsigned char*)(-3 + arg2_457X))))))) {
          *((long*)((unsigned char*)(-3 + arg2_457X))) = 529;
          goto L17967;}
        else {
          goto L17967;}}
     L17967: {
      RSvalS = 13;
      goto START;}}}
      break;
    case 100 : {
      long x_460X;
      x_460X = RSvalS;
      if ((3 == (3 & x_460X))) {
        if ((0 == (128 & (*((long*)((unsigned char*)(-7 + x_460X))))))) {
          arg1K0 = 1;
          goto L23518;}
        else {
          arg1K0 = 5;
          goto L23518;}}
      else {
        arg1K0 = 5;
        goto L23518;}
     L23518: {
      char val_461X = arg1K0;
      RSvalS = ((long)val_461X);
      goto START;}}
      break;
    case 101 : {
      long x_462X;
      x_462X = RSvalS;
      if ((3 == (3 & x_462X))) {
        if ((0 == (128 & (*((long*)((unsigned char*)(-7 + x_462X))))))) {
          *((long*)((unsigned char*)(-7 + x_462X))) = (128 | (*((long*)((unsigned char*)(-7 + x_462X)))));
          goto L23426;}
        else {
          goto L23426;}}
      else {
        goto L23426;}
     L23426: {
      RSvalS = 13;
      goto START;}}
      break;
    case 102 : {
      long arg2_463X;
      merged_arg0K0 = 5;
      ensure_space_return_tag = 6;
      goto ensure_space;
     ensure_space_return_6:
      RSstackS = (4 + RSstackS);
      arg2_463X = *((long*)((unsigned char*)RSstackS));
      if ((3 == (3 & arg2_463X))) {
        long x_464X;
        x_464X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_463X)))) >> 2);
        if ((15 == x_464X)) {
          if ((0 == (3 & RSvalS))) {
            long arg2_465X;
            arg2_465X = RSvalS >> 2;
            arg0K0 = 0;
            goto L29811;
           L29811: {
            long i_466X = arg0K0;
            if ((i_466X < 100)) {
              if ((1 == (255 & (*((Sopen_vm_portsS + i_466X)))))) {
                arg0K0 = i_466X;
                arg0K1 = arg2_463X;
                goto L29730;}
              else {
                arg0K0 = (1 + i_466X);
                goto L29811;}}
            else {
              arg0K0 = -1;
              arg0K1 = arg2_463X;
              goto L29730;}}
           L29730: {
            long index_467X = arg0K0;
            long filename_468X = arg0K1;
            if ((index_467X < 0)) {
              long filename_469X;
              merged_arg0K0 = filename_468X;
              collect_saving_temp_return_tag = 3;
              goto collect_saving_temp;
             collect_saving_temp_return_3:
              filename_469X = collect_saving_temp_return_value;
              arg0K0 = 0;
              goto L29914;
             L29914: {
              long i_470X = arg0K0;
              if ((i_470X < 100)) {
                if ((1 == (255 & (*((Sopen_vm_portsS + i_470X)))))) {
                  arg0K0 = i_470X;
                  goto L29781;}
                else {
                  arg0K0 = (1 + i_470X);
                  goto L29914;}}
              else {
                arg0K0 = -1;
                goto L29781;}}
             L29781: {
              long index_471X = arg0K0;
              if ((index_471X < 0)) {
                char v_472X;
                SvalS = RSvalS;
                SstackS = RSstackS;
                Scode_pointerS = RScode_pointerS;
                v_472X = 0; /* JAR HACK */ Qps_error_0((XX129.contents));
                RSvalS = SvalS;
                RSstackS = SstackS;
                RScode_pointerS = Scode_pointerS;
                SvalS = RSvalS;
                SstackS = RSstackS;
                Scode_pointerS = RScode_pointerS;
                TTreturn_value = v_472X;
                return(0L);}
              else {
                arg0K0 = index_471X;
                arg0K1 = filename_469X;
                goto L29730;}}}
            else {
              if ((2 == arg2_465X)) {
                char *spec_473X;
                merged_arg0K0 = filename_468X;
                extract_string_return_tag = 0;
                goto extract_string;
               extract_string_return_0:
                spec_473X = extract_string_return_value;
                arg3K0 = (ps_open(spec_473X, "w"));
                goto L29752;}
              else {
                char *spec_474X;
                merged_arg0K0 = filename_468X;
                extract_string_return_tag = 1;
                goto extract_string;
               extract_string_return_1:
                spec_474X = extract_string_return_value;
                arg3K0 = (ps_open(spec_474X, "r"));
                goto L29752;}
             L29752: {
              FILE * port_475X = arg3K0;
              if ((NULL == port_475X)) {
                RSvalS = 1;
                goto START;}
              else {
                long new_476X;
                *((long*)((unsigned char*)ShpS)) = 4118;
                ShpS = (4 + ShpS);
                new_476X = 3 + ShpS;
                ShpS = (16 + ShpS);
                *((long*)((unsigned char*)(-4 + ShpS))) = 0;
                *((long*)((unsigned char*)(-3 + new_476X))) = (arg2_465X << 2);
                *((long*)((unsigned char*)(1 + new_476X))) = (index_467X << 2);
                *((long*)((unsigned char*)(5 + new_476X))) = 1;
                *((long*)((unsigned char*)(9 + new_476X))) = filename_468X;
                *((Sopen_portsS + index_467X)) = port_475X;
                *((Sopen_vm_portsS + index_467X)) = new_476X;
                RSvalS = new_476X;
                goto START;}}}}}
          else {
            goto L29801;}}
        else {
          goto L29801;}}
      else {
        goto L29801;}
     L29801: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg2_463X;
      merged_arg0K2 = RSvalS;
      goto raise_exception2;}}
      break;
    case 103 : {
      long obj_477X;
      obj_477X = RSvalS;
      if ((3 == (3 & obj_477X))) {
        long x_478X;
        x_478X = 31 & ((*((long*)((unsigned char*)(-7 + obj_477X)))) >> 2);
        if ((5 == x_478X)) {
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          close_port(RSvalS);
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          RSvalS = 13;
          goto START;}
        else {
          goto L23344;}}
      else {
        goto L23344;}
     L23344: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 104 : {
      long x_479X;
      x_479X = RSvalS;
      if ((3 == (3 & x_479X))) {
        long x_480X;
        x_480X = 31 & ((*((long*)((unsigned char*)(-7 + x_479X)))) >> 2);
        if ((5 == x_480X)) {
          unsigned char v_481X;
          v_481X = 4 == (*((long*)((unsigned char*)(-3 + x_479X))));
          if (v_481X) {
            arg1K0 = 5;
            goto L23253;}
          else {
            arg1K0 = 1;
            goto L23253;}}
        else {
          arg1K0 = 1;
          goto L23253;}}
      else {
        arg1K0 = 1;
        goto L23253;}
     L23253: {
      char val_482X = arg1K0;
      RSvalS = ((long)val_482X);
      goto START;}}
      break;
    case 105 : {
      long x_483X;
      x_483X = RSvalS;
      if ((3 == (3 & x_483X))) {
        long x_484X;
        x_484X = 31 & ((*((long*)((unsigned char*)(-7 + x_483X)))) >> 2);
        if ((5 == x_484X)) {
          unsigned char v_485X;
          v_485X = 8 == (*((long*)((unsigned char*)(-3 + x_483X))));
          if (v_485X) {
            arg1K0 = 5;
            goto L23153;}
          else {
            arg1K0 = 1;
            goto L23153;}}
        else {
          arg1K0 = 1;
          goto L23153;}}
      else {
        arg1K0 = 1;
        goto L23153;}
     L23153: {
      char val_486X = arg1K0;
      RSvalS = ((long)val_486X);
      goto START;}}
      break;
    case 106 : {
      long p_487X;
      p_487X = RSvalS;
      if ((3 == (3 & p_487X))) {
        long x_488X;
        x_488X = 31 & ((*((long*)((unsigned char*)(-7 + p_487X)))) >> 2);
        if ((5 == x_488X)) {
          unsigned char v_489X;
          v_489X = 4 == (*((long*)((unsigned char*)(-3 + p_487X))));
          if (v_489X) {
            if (((*((long*)((unsigned char*)(1 + p_487X)))) < 0)) {
              goto L23016;}
            else {
              long x_490X;
              long c_491X;
              x_490X = RSvalS;
              c_491X = *((long*)((unsigned char*)(5 + x_490X)));
              if ((1 == (255 & c_491X))) {
                int x_492X;
                int c_493X;
                PS_GETC((*((Sopen_portsS + ((*((long*)((unsigned char*)(1 + x_490X)))) >> 2)))), x_492X)
                c_493X = x_492X;
                if (((EOF == c_493X))) {
                  arg0K0 = 21;
                  goto L23013;}
                else {
                  arg0K0 = (9 + (((long)(((char)c_493X))) << 8));
                  goto L23013;}}
              else {
                *((long*)((unsigned char*)(5 + x_490X))) = 1;
                arg0K0 = c_491X;
                goto L23013;}
             L23013: {
              long val_494X = arg0K0;
              RSvalS = val_494X;
              goto START;}}}
          else {
            goto L23016;}}
        else {
          goto L23016;}}
      else {
        goto L23016;}
     L23016: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 107 : {		/* peek-char */
      long p_495X;
      p_495X = RSvalS;
      if ((3 == (3 & p_495X))) { /* stob? */
        long x_496X;
        x_496X = 31 & ((*((long*)((unsigned char*)(-7 + p_495X)))) >> 2);
        if ((5 == x_496X)) {	/* port? */
          unsigned char v_497X;
          v_497X = 4 == (*((long*)((unsigned char*)(-3 + p_495X))));
          if (v_497X) {		/* input-port? */
            if (((*((long*)((unsigned char*)(1 + p_495X)))) < 0)) {
              goto L22863;}
            else {
              long x_498X;
              long c_499X;
              x_498X = RSvalS;
              c_499X = *((long*)((unsigned char*)(5 + x_498X)));
              if ((1 == (255 & c_499X))) { /* false? */
                int x_500X;
                int c_501X;
                PS_GETC((*((Sopen_portsS + ((*((long*)((unsigned char*)(1 + x_498X)))) >> 2)))), x_500X)
                c_501X = x_500X;
                if (((EOF == c_501X))) {
                  arg0K0 = 21;
                  goto L22854;}
                else {
                  arg0K0 = (9 + (((long)(((char)c_501X))) << 8));
                  goto L22854;}
               L22854: {
                long c_502X = arg0K0;
                *((long*)((unsigned char*)(5 + x_498X))) = c_502X;
                arg0K0 = c_502X;
                goto L22860;}}
              else {
                arg0K0 = c_499X;
                goto L22860;}
             L22860: {
              long val_503X = arg0K0;
              RSvalS = val_503X;
              goto START;}}}
          else {
            goto L22863;}}
        else {
          goto L22863;}}
      else {
        goto L22863;}
     L22863: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 108 : {		/* char-ready? */
      long p_504X;
      p_504X = RSvalS;
      if ((3 == (3 & p_504X))) { /* stob? */
        long x_505X;
        x_505X = 31 & ((*((long*)((unsigned char*)(-7 + p_504X)))) >> 2);
        if ((5 == x_505X)) {	/* port? */
          unsigned char v_506X;
          v_506X = 4 == (*((long*)((unsigned char*)(-3 + p_504X))));
          if (v_506X) {		/* input-port? */
            if (((*((long*)((unsigned char*)(1 + p_504X)))) < 0)) {
              goto L22718;}
	    /* JAR PATCH */
	    else if (1 != *((long*)((unsigned char*)(5 + p_504X)))) {
	      arg1K0 = 5;
	      goto L22715;}
            else {
              unsigned char b_507X;
              b_507X = char_ready_p((*((Sopen_portsS + ((*((long*)((unsigned char*)(1 + RSvalS)))) >> 2)))));
              if (b_507X) {
                arg1K0 = 5;
                goto L22715;}
              else {
                arg1K0 = 1;
                goto L22715;}
             L22715: {
              char val_508X = arg1K0;
              RSvalS = ((long)val_508X);
              goto START;}}}
          else {
            goto L22718;}}
        else {
          goto L22718;}}
      else {
        goto L22718;}
     L22718: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 109 : {
      long arg2_509X;
      RSstackS = (4 + RSstackS);
      arg2_509X = *((long*)((unsigned char*)RSstackS));
      if ((9 == (255 & arg2_509X))) {
        long p_510X;
        p_510X = RSvalS;
        if ((3 == (3 & p_510X))) {
          long x_511X;
          x_511X = 31 & ((*((long*)((unsigned char*)(-7 + p_510X)))) >> 2);
          if ((5 == x_511X)) {
            unsigned char v_512X;
            v_512X = 8 == (*((long*)((unsigned char*)(-3 + p_510X))));
            if (v_512X) {
              if (((*((long*)((unsigned char*)(1 + p_510X)))) < 0)) {
                goto L17776;}
              else {
                putc((((char)(arg2_509X >> 8))), (*((Sopen_portsS + ((*((long*)((unsigned char*)(1 + RSvalS)))) >> 2)))));
                RSvalS = 13;
                goto START;}}
            else {
              goto L17776;}}
          else {
            goto L17776;}}
        else {
          goto L17776;}}
      else {
        goto L17776;}
     L17776: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg2_509X;
      merged_arg0K2 = RSvalS;
      goto raise_exception2;}}
      break;
    case 110 : {
      long arg2_513X;
      RSstackS = (4 + RSstackS);
      arg2_513X = *((long*)((unsigned char*)RSstackS));
      if ((3 == (3 & arg2_513X))) {
        long x_514X;
        x_514X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_513X)))) >> 2);
        if ((15 == x_514X)) {
          long p_515X;
          p_515X = RSvalS;
          if ((3 == (3 & p_515X))) {
            long x_516X;
            x_516X = 31 & ((*((long*)((unsigned char*)(-7 + p_515X)))) >> 2);
            if ((5 == x_516X)) {
              unsigned char v_517X;
              v_517X = 8 == (*((long*)((unsigned char*)(-3 + p_515X))));
              if (v_517X) {
                if (((*((long*)((unsigned char*)(1 + p_515X)))) < 0)) {
                  goto L17573;}
                else {
                  FILE * port_518X;
                  long count_519X;
                  long sent_520X;
                  port_518X = *((Sopen_portsS + ((*((long*)((unsigned char*)(1 + RSvalS)))) >> 2)));
                  count_519X = (*((long*)((unsigned char*)(-7 + arg2_513X)))) >> 8;
                  sent_520X = fwrite((void *)((unsigned char*)(-3 + arg2_513X)), sizeof(char), count_519X, port_518X);
                  if ((sent_520X == count_519X)) {
                    goto L17570;}
                  else {
                    SvalS = RSvalS;
                    SstackS = RSstackS;
                    Scode_pointerS = RScode_pointerS;
                    Qps_error_0((XX21.contents));
                    RSvalS = SvalS;
                    RSstackS = SstackS;
                    RScode_pointerS = Scode_pointerS;
                    goto L17570;}
                 L17570: {
                  RSvalS = 13;
                  goto START;}}}
              else {
                goto L17573;}}
            else {
              goto L17573;}}
          else {
            goto L17573;}}
        else {
          goto L17573;}}
      else {
        goto L17573;}
     L17573: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg2_513X;
      merged_arg0K2 = RSvalS;
      goto raise_exception2;}}
      break;
    case 111 : {
      long p_521X;
      p_521X = RSvalS;
      if ((3 == (3 & p_521X))) {
        long x_522X;
        x_522X = 31 & ((*((long*)((unsigned char*)(-7 + p_521X)))) >> 2);
        if ((5 == x_522X)) {
          unsigned char v_523X;
          v_523X = 8 == (*((long*)((unsigned char*)(-3 + p_521X))));
          if (v_523X) {
            if (((*((long*)((unsigned char*)(1 + p_521X)))) < 0)) {
              goto L22605;}
            else {
              fflush((*((Sopen_portsS + ((*((long*)((unsigned char*)(1 + RSvalS)))) >> 2)))));
              RSvalS = 13;
              goto START;}}
          else {
            goto L22605;}}
        else {
          goto L22605;}}
      else {
        goto L22605;}
     L22605: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 112 : {
      RSvalS = 529;
      goto START;}
      break;
    case 113 : {
      RSvalS = 13;
      goto START;}
      break;
    case 114 : {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}
      break;
    case 115 : {
      RSvalS = 1;
      goto START;}
      break;
    case 116 : {
      long arg2_524X;
      long arg3_525X;
      RSstackS = (4 + RSstackS);
      arg2_524X = *((long*)((unsigned char*)RSstackS));
      RSstackS = (4 + RSstackS);
      arg3_525X = *((long*)((unsigned char*)RSstackS));
      if ((3 == (3 & arg3_525X))) {
        long x_526X;
        x_526X = 31 & ((*((long*)((unsigned char*)(-7 + arg3_525X)))) >> 2);
        if ((15 == x_526X)) {
          long obj_527X;
          obj_527X = RSvalS;
          if ((3 == (3 & obj_527X))) {
            long x_528X;
            x_528X = 31 & ((*((long*)((unsigned char*)(-7 + obj_527X)))) >> 2);
            if ((15 == x_528X)) {
              long x_529X;
              char *spec_530X;
              FILE * port_531X;
              x_529X = RSvalS;
              merged_arg0K0 = arg3_525X;
              extract_string_return_tag = 2;
              goto extract_string;
             extract_string_return_2:
              spec_530X = extract_string_return_value;
              port_531X = ps_open(spec_530X, "w");
              if ((NULL == port_531X)) {
                merged_arg1K0 = 0;
                merged_arg0K1 = arg3_525X;
                merged_arg0K2 = arg2_524X;
                goto raise_exception2;}
              else {
                long count_532X;
                long sent_533X;
                count_532X = (*((long*)((unsigned char*)(-7 + x_529X)))) >> 8;
                sent_533X = fwrite((void *)((unsigned char*)(-3 + x_529X)), sizeof(char), count_532X, port_531X);
                if ((sent_533X == count_532X)) {
                  goto L28581;}
                else {
                  SvalS = RSvalS;
                  SstackS = RSstackS;
                  Scode_pointerS = RScode_pointerS;
                  Qps_error_0((XX21.contents));
                  RSvalS = SvalS;
                  RSstackS = SstackS;
                  RScode_pointerS = Scode_pointerS;
                  goto L28581;}
               L28581: {
                long b_534X;
                long e_535X;
                long resume_proc_536X;
                b_534X = Snewspace_beginS;
                Snewspace_beginS = Soldspace_beginS;
                Soldspace_beginS = b_534X;
                e_535X = Snewspace_endS;
                Snewspace_endS = Soldspace_endS;
                Soldspace_endS = e_535X;
                Ssaved_limitS = SlimitS;
                Ssaved_hpS = ShpS;
                SlimitS = Snewspace_endS;
                ShpS = Snewspace_beginS;
                Sweak_pointer_hpS = -1;
                SvalS = RSvalS;
                SstackS = RSstackS;
                Scode_pointerS = RScode_pointerS;
                resume_proc_536X = trace_value(arg2_524X);
                RSvalS = SvalS;
                RSstackS = SstackS;
                RScode_pointerS = Scode_pointerS;
                SvalS = RSvalS;
                SstackS = RSstackS;
                Scode_pointerS = RScode_pointerS;
                clean_weak_pointers();
                RSvalS = SvalS;
                RSstackS = SstackS;
                RScode_pointerS = Scode_pointerS;
                arg0K0 = 2;
                goto L15455;
               L15455: {
                long i_537X = arg0K0;
                if ((100 == i_537X)) {
                  SvalS = RSvalS;
                  SstackS = RSstackS;
                  Scode_pointerS = RScode_pointerS;
                  write_string((XX128.contents), port_531X);
                  RSvalS = SvalS;
                  RSstackS = SstackS;
                  RScode_pointerS = Scode_pointerS;
                  putc(10, port_531X);
                  if ((port_531X == Scurrent_output_portS)) {
                    fflush(port_531X);
                    goto L15273;}
                  else {
                    goto L15273;}
                 L15273: {
                  putc(12, port_531X);
                  putc(10, port_531X);
                  if ((port_531X == Scurrent_output_portS)) {
                    fflush(port_531X);
                    goto L15277;}
                  else {
                    goto L15277;}
                 L15277: {
                  long count_538X;
                  long sent_539X;
                  merged_arg0K0 = 14;
                  merged_arg3K1 = port_531X;
                  write_number_no_newline_return_tag = 0;
                  goto write_number_no_newline;
                 write_number_no_newline_return_0:
                  putc(10, port_531X);
                  merged_arg0K0 = 4;
                  merged_arg3K1 = port_531X;
                  write_number_no_newline_return_tag = 1;
                  goto write_number_no_newline;
                 write_number_no_newline_return_1:
                  putc(10, port_531X);
                  merged_arg0K0 = (Snewspace_beginS >> 2);
                  merged_arg3K1 = port_531X;
                  write_number_no_newline_return_tag = 2;
                  goto write_number_no_newline;
                 write_number_no_newline_return_2:
                  putc(10, port_531X);
                  merged_arg0K0 = (ShpS >> 2);
                  merged_arg3K1 = port_531X;
                  write_number_no_newline_return_tag = 3;
                  goto write_number_no_newline;
                 write_number_no_newline_return_3:
                  putc(10, port_531X);
                  merged_arg0K0 = resume_proc_536X;
                  merged_arg3K1 = port_531X;
                  write_number_no_newline_return_tag = 4;
                  goto write_number_no_newline;
                 write_number_no_newline_return_4:
                  putc(10, port_531X);
                  putc(12, port_531X);
                  *((long*)((unsigned char*)ShpS)) = 1;
                  count_538X = (4 + ShpS) - ShpS;
                  sent_539X = fwrite((void *)((unsigned char*)ShpS), sizeof(char), count_538X, port_531X);
                  if ((sent_539X == count_538X)) {
                    goto L15301;}
                  else {
                    SvalS = RSvalS;
                    SstackS = RSstackS;
                    Scode_pointerS = RScode_pointerS;
                    Qps_error_0((XX21.contents));
                    RSvalS = SvalS;
                    RSstackS = SstackS;
                    RScode_pointerS = Scode_pointerS;
                    goto L15301;}
                 L15301: {
                  long count_540X;
                  long sent_541X;
                  count_540X = ShpS - Snewspace_beginS;
                  sent_541X = fwrite((void *)((unsigned char*)Snewspace_beginS), sizeof(char), count_540X, port_531X);
                  if ((sent_541X == count_540X)) {
                    arg0K0 = (ShpS - Snewspace_beginS);
                    goto L28591;}
                  else {
                    SvalS = RSvalS;
                    SstackS = RSstackS;
                    Scode_pointerS = RScode_pointerS;
                    Qps_error_0((XX21.contents));
                    RSvalS = SvalS;
                    RSstackS = SstackS;
                    RScode_pointerS = Scode_pointerS;
                    arg0K0 = (ShpS - Snewspace_beginS);
                    goto L28591;}}}}}
                else {
                  long port_542X;
                  port_542X = *((Sopen_vm_portsS + i_537X));
                  if ((1 == (255 & port_542X))) {
                    arg0K0 = (1 + i_537X);
                    goto L15455;}
                  else {
                    if ((3 == (3 & (*((long*)((unsigned char*)(-7 + port_542X))))))) {
                      SvalS = RSvalS;
                      SstackS = RSstackS;
                      Scode_pointerS = RScode_pointerS;
                      close_port_noisily((*((long*)((unsigned char*)(-7 + port_542X)))));
                      RSvalS = SvalS;
                      RSstackS = SstackS;
                      RScode_pointerS = Scode_pointerS;
                      arg0K0 = 1;
                      goto L15464;}
                    else {
                      arg0K0 = port_542X;
                      goto L15464;}
                   L15464: {
                    long value_543X = arg0K0;
                    *((Sopen_vm_portsS + i_537X)) = value_543X;
                    arg0K0 = (1 + i_537X);
                    goto L15455;}}}}
               L28591: {
                long size_544X = arg0K0;
                long b_545X;
                long e_546X;
                fclose(port_531X);
                b_545X = Snewspace_beginS;
                Snewspace_beginS = Soldspace_beginS;
                Soldspace_beginS = b_545X;
                e_546X = Snewspace_endS;
                Snewspace_endS = Soldspace_endS;
                Soldspace_endS = e_546X;
                SlimitS = Ssaved_limitS;
                ShpS = Ssaved_hpS;
                arg0K0 = Snewspace_beginS;
                goto L9853;
               L9853: {
                long addr_547X = arg0K0;
                if ((addr_547X < ShpS)) {
                  long d_548X;
                  d_548X = *((long*)((unsigned char*)addr_547X));
                  if ((2 == (3 & d_548X))) {
                    arg0K0 = d_548X;
                    goto L9875;}
                  else {
                    long h_549X;
                    h_549X = *((long*)((unsigned char*)(-7 + d_548X)));
                    *((long*)((unsigned char*)addr_547X)) = h_549X;
                    arg0K0 = h_549X;
                    goto L9875;}
                 L9875: {
                  long h_550X = arg0K0;
                  arg0K0 = (4 + (addr_547X + (-4 & (3 + (h_550X >> 8)))));
                  goto L9853;}}
                else {
                  RSvalS = (size_544X << 2);
                  goto START;}}}}}}
            else {
              goto L28600;}}
          else {
            goto L28600;}}
        else {
          goto L28600;}}
      else {
        goto L28600;}
     L28600: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg3_525X;
      merged_arg0K2 = arg2_524X;
      merged_arg0K3 = RSvalS;
      goto raise_exception3;}}
      break;
    case 117 : {
      RSvalS = 13;
      interpreter_collect_return_tag = 0;
      goto interpreter_collect;
     interpreter_collect_return_0:
      RSvalS = 13;
      goto START;}
      break;
    case 118 : {
      long arg2_551X;
      RSstackS = (4 + RSstackS);
      arg2_551X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & arg2_551X))) {
        long x_552X;
        long key_553X;
        x_552X = RSvalS;
        key_553X = arg2_551X >> 2;
        if ((0 == key_553X)) {
          long n_554X;
          n_554X = (SlimitS - ShpS) >> 2;
          RSvalS = (n_554X << 2);
          goto START;}
        else {
          if ((1 == key_553X)) {
            RSvalS = (-4 & (3 + (Snewspace_endS - Snewspace_beginS)));
            goto START;}
          else {
            if ((2 == key_553X)) {
              RSvalS = (Sstack_sizeS << 2);
              goto START;}
            else {
              if ((3 == key_553X)) {
                if ((0 == (3 & x_552X))) {
                  long old_555X;
                  old_555X = Sminimum_recovered_spaceS;
                  Sminimum_recovered_spaceS = (x_552X >> 2);
                  RSvalS = (old_555X << 2);
                  goto START;}
                else {
                  merged_arg1K0 = 0;
                  merged_arg0K1 = (key_553X << 2);
                  merged_arg0K2 = x_552X;
                  goto raise_exception2;}}
              else {
                if ((4 == key_553X)) {
                  RSvalS = (Sgc_countS << 2);
                  goto START;}
                else {
                  merged_arg1K0 = 0;
                  merged_arg0K1 = (key_553X << 2);
                  merged_arg0K2 = x_552X;
                  goto raise_exception2;}}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_551X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 119 : {
      long obj_556X;
      obj_556X = RSvalS;
      if ((3 == (3 & obj_556X))) {
        long x_557X;
        x_557X = 31 & ((*((long*)((unsigned char*)(-7 + obj_556X)))) >> 2);
        if ((2 == x_557X)) {
          long table_558X;
          long start_hp_559X;
          table_558X = RSvalS;
          start_hp_559X = ShpS;
          arg0K0 = Snewspace_beginS;
          goto L16049;
         L16049: {
          long addr_560X = arg0K0;
          if ((addr_560X < start_hp_559X)) {
            long d_561X;
            d_561X = *((long*)((unsigned char*)addr_560X));
            if ((2 == (3 & d_561X))) {
              if ((1 == (31 & (d_561X >> 2)))) {
                unsigned char v_562X;
                v_562X = (12 + ShpS) < SlimitS;
                if (v_562X) {
                  long obj_563X;
                  long x_564X;
                  long s_565X;
                  long n_566X;
                  obj_563X = 7 + addr_560X;
                  x_564X = (3 + ((*((long*)((unsigned char*)(-7 + table_558X)))) >> 8)) >> 2;
                  s_565X = *((long*)((unsigned char*)(-3 + obj_563X)));
                  n_566X = (*((long*)((unsigned char*)(-7 + s_565X)))) >> 8;
                  arg0K0 = 0;
                  arg0K1 = 0;
                  goto L15093;
                 L15093: {
                  long i_567X = arg0K0;
                  long h_568X = arg0K1;
                  if ((i_567X < n_566X)) {
                    char x_569X;
                    x_569X = (*((unsigned char*)((unsigned char*)((-3 + s_565X) + i_567X))));
                    arg0K0 = (1 + i_567X);
                    arg0K1 = (h_568X + ((long)(x_569X)));
                    goto L15093;}
                  else {
                    long index_570X;
                    long bucket_571X;
                    index_570X = h_568X & (-1 + x_564X);
                    bucket_571X = *((long*)((unsigned char*)((-3 + table_558X) + (index_570X << 2))));
                    arg0K0 = bucket_571X;
                    goto L14995;
                   L14995: {
                    long b_572X = arg0K0;
                    if ((25 == b_572X)) {
                      long new_573X;
                      *((long*)((unsigned char*)ShpS)) = 2050;
                      ShpS = (4 + ShpS);
                      new_573X = 3 + ShpS;
                      ShpS = (8 + ShpS);
                      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
                      *((long*)((unsigned char*)(-3 + new_573X))) = obj_563X;
                      *((long*)((unsigned char*)(1 + new_573X))) = bucket_571X;
                      *((long*)((unsigned char*)((-3 + table_558X) + (index_570X << 2)))) = new_573X;
                      arg0K0 = (4 + (addr_560X + (-4 & (3 + (d_561X >> 8)))));
                      goto L16049;}
                    else {
                      if ((obj_563X == (*((long*)((unsigned char*)(-3 + b_572X)))))) {
                        arg0K0 = (4 + (addr_560X + (-4 & (3 + (d_561X >> 8)))));
                        goto L16049;}
                      else {
                        arg0K0 = (*((long*)((unsigned char*)(1 + b_572X))));
                        goto L14995;}}}}}}
                else {
                  goto L16027;}}
              else {
                arg0K0 = (4 + (addr_560X + (-4 & (3 + (d_561X >> 8)))));
                goto L16049;}}
            else {
              SvalS = RSvalS;
              SstackS = RSstackS;
              Scode_pointerS = RScode_pointerS;
              write_string((XX127.contents), Scurrent_output_portS);
              RSvalS = SvalS;
              RSstackS = SstackS;
              RScode_pointerS = Scode_pointerS;
              goto L16027;}}
          else {
            RSvalS = 13;
            goto START;}
         L16027: {
          ShpS = start_hp_559X;
          merged_arg1K0 = 0;
          goto raise_exception;}}}
        else {
          goto L26514;}}
      else {
        goto L26514;}
     L26514: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 120 : {
      if ((0 == (3 & RSvalS))) {
        long type_574X;
        long start_hp_575X;
        type_574X = RSvalS >> 2;
        start_hp_575X = ShpS;
        *((long*)((unsigned char*)ShpS)) = 0;
        ShpS = (4 + ShpS);
        arg0K0 = Snewspace_beginS;
        goto L10397;
       L10397: {
        long addr_576X = arg0K0;
        if ((addr_576X < start_hp_575X)) {
          long d_577X;
          d_577X = *((long*)((unsigned char*)addr_576X));
          if ((2 == (3 & d_577X))) {
            if ((type_574X == (31 & (d_577X >> 2)))) {
              unsigned char v_578X;
              v_578X = (16 + ShpS) < SlimitS;
              if (v_578X) {
                *((long*)((unsigned char*)ShpS)) = (7 + addr_576X);
                ShpS = (4 + ShpS);
                arg0K0 = (4 + (addr_576X + (-4 & (3 + (d_577X >> 8)))));
                goto L10397;}
              else {
                goto L10348;}}
            else {
              arg0K0 = (4 + (addr_576X + (-4 & (3 + (d_577X >> 8)))));
              goto L10397;}}
          else {
            SvalS = RSvalS;
            SstackS = RSstackS;
            Scode_pointerS = RScode_pointerS;
            write_string((XX127.contents), Scurrent_output_portS);
            RSvalS = SvalS;
            RSstackS = SstackS;
            RScode_pointerS = Scode_pointerS;
            goto L10348;}}
        else {
          *((long*)((unsigned char*)start_hp_575X)) = (-1014 + ((ShpS - start_hp_575X) << 8));
          arg0K0 = (7 + start_hp_575X);
          goto L22438;}
       L10348: {
        ShpS = start_hp_575X;
        arg0K0 = 1;
        goto L22438;}}
       L22438: {
        long vector_579X = arg0K0;
        if ((1 == (255 & vector_579X))) {
          merged_arg1K0 = 0;
          merged_arg0K1 = (type_574X << 2);
          goto raise_exception1;}
        else {
          RSvalS = vector_579X;
          goto START;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 121 : {
      RSvalS = Sdynamic_stateS;
      goto START;}
      break;
    case 122 : {
      Sdynamic_stateS = RSvalS;
      RSvalS = 13;
      goto START;}
      break;
    case 123 : {
      long obj_580X;
      obj_580X = RSvalS;
      if ((3 == (3 & obj_580X))) {
        long x_581X;
        x_581X = 31 & ((*((long*)((unsigned char*)(-7 + obj_580X)))) >> 2);
        if ((3 == x_581X)) {
          Sexception_handlerS = RSvalS;
          goto START;}
        else {
          goto L26984;}}
      else {
        goto L26984;}
     L26984: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 124 : {
      long obj_582X;
      obj_582X = RSvalS;
      if ((3 == (3 & obj_582X))) {
        long x_583X;
        x_583X = 31 & ((*((long*)((unsigned char*)(-7 + obj_582X)))) >> 2);
        if ((2 == x_583X)) {
          long x_584X;
          x_584X = (3 + ((*((long*)((unsigned char*)(-7 + RSvalS)))) >> 8)) >> 2;
          if ((x_584X < 3)) {
            goto L26890;}
          else {
            Sinterrupt_handlersS = RSvalS;
            goto START;}}
        else {
          goto L26890;}}
      else {
        goto L26890;}
     L26890: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 125 : {
      long temp_585X;
      temp_585X = Senabled_interruptsS;
      Senabled_interruptsS = (RSvalS >> 2);
      RSvalS = (temp_585X << 2);
      goto START;}
      break;
    case 126 : {
      long tem_586X;
      RSstackS = (4 + RSstackS);
      Senabled_interruptsS = ((*((long*)((unsigned char*)RSstackS))) >> 2);
      RSstackS = (4 + RSstackS);
      SnargsS = ((*((long*)((unsigned char*)RSstackS))) >> 2);
      RSstackS = (4 + RSstackS);
      SenvS = (*((long*)((unsigned char*)RSstackS)));
      RSstackS = (4 + RSstackS);
      tem_586X = *((long*)((unsigned char*)RSstackS));
      StemplateS = tem_586X;
      RScode_pointerS = (-3 + (*((long*)((unsigned char*)(-3 + tem_586X)))));
      goto START;}
      break;
    case 127 : {
      if ((0 == (3 & RSvalS))) {
        long p_587X;
        long n_588X;
        p_587X = RSvalS;
        Spending_interruptsS = (-2 & Spending_interruptsS);
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        n_588X = ps_schedule_interrupt((p_587X >> 2));
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSvalS = (n_588X << 2);
        goto START;}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 128 : {
      long obj_589X;
      obj_589X = RSvalS;
      if ((3 == (3 & obj_589X))) {
        long x_590X;
        x_590X = 31 & ((*((long*)((unsigned char*)(-7 + obj_589X)))) >> 2);
        if ((12 == x_590X)) {
          long x_591X;
          long value_592X;
          long name_593X;
          x_591X = RSvalS;
          value_592X = *((long*)((unsigned char*)(1 + x_591X)));
          name_593X = *((long*)((unsigned char*)(-3 + x_591X)));
          if ((3 == (3 & name_593X))) {
            long x_594X;
            x_594X = 31 & ((*((long*)((unsigned char*)(-7 + name_593X)))) >> 2);
            if ((15 == x_594X)) {
              if ((3 == (3 & value_592X))) {
                long x_595X;
                x_595X = 31 & ((*((long*)((unsigned char*)(-7 + value_592X)))) >> 2);
                if ((16 == x_595X)) {
                  unsigned char v_596X;
                  SvalS = RSvalS;
                  SstackS = RSstackS;
                  Scode_pointerS = RScode_pointerS;
                  v_596X = lookup_external_name((-3 + name_593X), (-3 + value_592X));
                  RSvalS = SvalS;
                  RSstackS = SstackS;
                  RScode_pointerS = Scode_pointerS;
                  if (v_596X) {
                    RSvalS = 13;
                    goto START;}
                  else {
                    goto L22130;}}
                else {
                  goto L22130;}}
              else {
                goto L22130;}}
            else {
              goto L22130;}}
          else {
            goto L22130;}
         L22130: {
          merged_arg1K0 = 0;
          merged_arg0K1 = x_591X;
          goto raise_exception1;}}
        else {
          goto L22133;}}
      else {
        goto L22133;}
     L22133: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 129 : {
      long proc_597X;
      proc_597X = *((long*)((unsigned char*)(RSstackS + (SnargsS << 2))));
      if ((3 == (3 & proc_597X))) {
        long x_598X;
        x_598X = 31 & ((*((long*)((unsigned char*)(-7 + proc_597X)))) >> 2);
        if ((12 == x_598X)) {
          SnargsS = (-1 + SnargsS);
          RSvalS = proc_597X;
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          TTreturn_value = 1;
          return(0L);}
        else {
          goto L26421;}}
      else {
        goto L26421;}
     L26421: {
      merged_arg1K0 = 0;
      merged_arg0K1 = proc_597X;
      goto raise_exception1;}}
      break;
    case 130 : {
      long arg2_599X;
      RSstackS = (4 + RSstackS);
      arg2_599X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & arg2_599X))) {
        long x_600X;
        long key_601X;
        x_600X = RSvalS;
        key_601X = arg2_599X >> 2;
        if ((0 == key_601X)) {
          long n_602X;
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          n_602X = ps_ticks_per_second();
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          RSvalS = (n_602X << 2);
          goto START;}
        else {
          if ((1 == key_601X)) {
            long n_603X;
            SvalS = RSvalS;
            SstackS = RSstackS;
            Scode_pointerS = RScode_pointerS;
            n_603X = ps_run_time();
            RSvalS = SvalS;
            RSstackS = SstackS;
            RScode_pointerS = Scode_pointerS;
            RSvalS = (n_603X << 2);
            goto START;}
          else {
            if ((2 == key_601X)) {
              long n_604X;
              SvalS = RSvalS;
              SstackS = RSstackS;
              Scode_pointerS = RScode_pointerS;
              n_604X = ps_real_time();
              RSvalS = SvalS;
              RSstackS = SstackS;
              RScode_pointerS = Scode_pointerS;
              RSvalS = (n_604X << 2);
              goto START;}
            else {
              merged_arg1K0 = 0;
              merged_arg0K1 = (key_601X << 2);
              merged_arg0K2 = x_600X;
              goto raise_exception2;}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_599X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 131 : {
      long arg2_605X;
      RSstackS = (4 + RSstackS);
      arg2_605X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & arg2_605X))) {
        long x_606X;
        long key_607X;
        long return_value_608X;
        x_606X = RSvalS;
        key_607X = arg2_605X >> 2;
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        return_value_608X = extended_vm(key_607X, x_606X);
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        if ((17 == (255 & return_value_608X))) {
          merged_arg1K0 = 0;
          merged_arg0K1 = (key_607X << 2);
          merged_arg0K2 = x_606X;
          goto raise_exception2;}
        else {
          RSvalS = return_value_608X;
          goto START;}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_605X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 132 : {
      long arg2_609X;
      RSstackS = (4 + RSstackS);
      arg2_609X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & arg2_609X))) {
        RSvalS = RSvalS;
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        TTreturn_value = 0;
        return(0L);}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_609X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 133 : {
      long arg2_610X;
      RSstackS = (4 + RSstackS);
      arg2_610X = *((long*)((unsigned char*)RSstackS));
      if ((3 == (3 & arg2_610X))) {
        long x_611X;
        x_611X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_610X)))) >> 2);
        if ((15 == x_611X)) {
          long obj_612X;
          obj_612X = RSvalS;
          if ((3 == (3 & obj_612X))) {
            long x_613X;
            x_613X = 31 & ((*((long*)((unsigned char*)(-7 + obj_612X)))) >> 2);
            if ((15 == x_613X)) {
              unsigned char x_614X;
              merged_arg0K0 = arg2_610X;
              merged_arg0K1 = RSvalS;
              stob_equalP_return_tag = 0;
              goto stob_equalP;
             stob_equalP_return_0:
              x_614X = stob_equalP_return_value;
              if (x_614X) {
                arg1K0 = 5;
                goto L16627;}
              else {
                arg1K0 = 1;
                goto L16627;}
             L16627: {
              char val_615X = arg1K0;
              RSvalS = ((long)val_615X);
              goto START;}}
            else {
              goto L16618;}}
          else {
            goto L16618;}}
        else {
          goto L16618;}}
      else {
        goto L16618;}
     L16618: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg2_610X;
      merged_arg0K2 = RSvalS;
      goto raise_exception2;}}
      break;
    case 134 : {
      long obj_616X;
      obj_616X = RSvalS;
      if ((3 == (3 & obj_616X))) {
        long x_617X;
        x_617X = 31 & ((*((long*)((unsigned char*)(-7 + obj_616X)))) >> 2);
        if ((15 == x_617X)) {
          long x_618X;
          long n_619X;
          x_618X = RSvalS;
          n_619X = (*((long*)((unsigned char*)(-7 + x_618X)))) >> 8;
          arg0K0 = 0;
          arg0K1 = 0;
          goto L22023;
         L22023: {
          long i_620X = arg0K0;
          long h_621X = arg0K1;
          if ((i_620X < n_619X)) {
            char x_622X;
            x_622X = (*((unsigned char*)((unsigned char*)((-3 + x_618X) + i_620X))));
            arg0K0 = (1 + i_620X);
            arg0K1 = (h_621X + ((long)(x_622X)));
            goto L22023;}
          else {
            RSvalS = (h_621X << 2);
            goto START;}}}
        else {
          goto L22003;}}
      else {
        goto L22003;}
     L22003: {
      merged_arg1K0 = 0;
      merged_arg0K1 = RSvalS;
      goto raise_exception1;}}
      break;
    case 135 : {
      long arg2_623X;
      merged_arg0K0 = (1 + ((3 + RSvalS) >> 2));
      ensure_space_return_tag = 7;
      goto ensure_space;
     ensure_space_return_7:
      RSstackS = (4 + RSstackS);
      arg2_623X = *((long*)((unsigned char*)RSstackS));
      if ((0 == (3 & RSvalS))) {
        long arg2_624X;
        arg2_624X = RSvalS >> 2;
        if ((3 == (3 & arg2_623X))) {
          long x_625X;
          x_625X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_623X)))) >> 2);
          if ((0 == x_625X)) {
            goto L21453;}
          else {
            goto L21344;}}
        else {
          goto L21344;}
       L21344: {
        if ((25 == arg2_623X)) {
          goto L21453;}
        else {
          merged_arg1K0 = 0;
          merged_arg0K1 = arg2_623X;
          merged_arg0K2 = (arg2_624X << 2);
          goto raise_exception2;}}
       L21453: {
        long new_626X;
        *((long*)((unsigned char*)ShpS)) = (62 + (arg2_624X << 8));
        ShpS = (4 + ShpS);
        new_626X = 3 + ShpS;
        ShpS = (ShpS + (-4 & (3 + arg2_624X)));
        if ((0 < arg2_624X)) {
          *((long*)((unsigned char*)(-4 + ShpS))) = 0;
          arg0K0 = new_626X;
          goto L21361;}
        else {
          arg0K0 = new_626X;
          goto L21361;}
       L21361: {
        long obj_627X = arg0K0;
        arg0K0 = arg2_623X;
        arg0K1 = (-1 + arg2_624X);
        goto L21366;
       L21366: {
        long l_628X = arg0K0;
        long i_629X = arg0K1;
        if ((i_629X < 0)) {
          RSvalS = obj_627X;
          goto START;}
        else {
          *((unsigned char*)((unsigned char*)((-3 + obj_627X) + i_629X))) = ((((char)((*((long*)((unsigned char*)(-3 + l_628X)))) >> 8))));
          arg0K0 = (*((long*)((unsigned char*)(1 + l_628X))));
          arg0K1 = (-1 + i_629X);
          goto L21366;}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg0K1 = arg2_623X;
        merged_arg0K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 136 : {
      long arg2_630X;
      merged_arg0K0 = 5;
      ensure_space_return_tag = 8;
      goto ensure_space;
     ensure_space_return_8:
      RSstackS = (4 + RSstackS);
      arg2_630X = *((long*)((unsigned char*)RSstackS));
      if ((3 == (3 & arg2_630X))) {
        long x_631X;
        x_631X = 31 & ((*((long*)((unsigned char*)(-7 + arg2_630X)))) >> 2);
        if ((15 == x_631X)) {
          long obj_632X;
          obj_632X = RSvalS;
          if ((3 == (3 & obj_632X))) {
            long x_633X;
            x_633X = 31 & ((*((long*)((unsigned char*)(-7 + obj_632X)))) >> 2);
            if ((2 == x_633X)) {
              long table_634X;
              long x_635X;
              long n_636X;
              table_634X = RSvalS;
              x_635X = (3 + ((*((long*)((unsigned char*)(-7 + table_634X)))) >> 8)) >> 2;
              n_636X = (*((long*)((unsigned char*)(-7 + arg2_630X)))) >> 8;
              arg0K0 = 0;
              arg0K1 = 0;
              goto L14097;
             L14097: {
              long i_637X = arg0K0;
              long h_638X = arg0K1;
              if ((i_637X < n_636X)) {
                char x_639X;
                x_639X = (*((unsigned char*)((unsigned char*)((-3 + arg2_630X) + i_637X))));
                arg0K0 = (1 + i_637X);
                arg0K1 = (h_638X + ((long)(x_639X)));
                goto L14097;}
              else {
                long index_640X;
                long bucket_641X;
                index_640X = h_638X & (-1 + x_635X);
                bucket_641X = *((long*)((unsigned char*)((-3 + table_634X) + (index_640X << 2))));
                arg0K0 = bucket_641X;
                goto L13992;
               L13992: {
                long b_642X = arg0K0;
                if ((25 == b_642X)) {
                  long new_643X;
                  long new_644X;
                  *((long*)((unsigned char*)ShpS)) = 1030;
                  ShpS = (4 + ShpS);
                  new_643X = 3 + ShpS;
                  ShpS = (4 + ShpS);
                  *((long*)((unsigned char*)(-4 + ShpS))) = 0;
                  *((long*)((unsigned char*)(-3 + new_643X))) = arg2_630X;
                  *((long*)((unsigned char*)ShpS)) = 2050;
                  ShpS = (4 + ShpS);
                  new_644X = 3 + ShpS;
                  ShpS = (8 + ShpS);
                  *((long*)((unsigned char*)(-4 + ShpS))) = 0;
                  *((long*)((unsigned char*)(-3 + new_644X))) = new_643X;
                  *((long*)((unsigned char*)(1 + new_644X))) = bucket_641X;
                  *((long*)((unsigned char*)((-3 + table_634X) + (index_640X << 2)))) = new_644X;
                  arg0K0 = new_643X;
                  goto L21165;}
                else {
                  unsigned char v_645X;
                  merged_arg0K0 = arg2_630X;
                  merged_arg0K1 = (*((long*)((unsigned char*)(-3 + (*((long*)((unsigned char*)(-3 + b_642X))))))));
                  stob_equalP_return_tag = 1;
                  goto stob_equalP;
                 stob_equalP_return_1:
                  v_645X = stob_equalP_return_value;
                  if (v_645X) {
                    arg0K0 = (*((long*)((unsigned char*)(-3 + b_642X))));
                    goto L21165;}
                  else {
                    arg0K0 = (*((long*)((unsigned char*)(1 + b_642X))));
                    goto L13992;}}}}}
             L21165: {
              long val_646X = arg0K0;
              RSvalS = val_646X;
              goto START;}}
            else {
              goto L21168;}}
          else {
            goto L21168;}}
        else {
          goto L21168;}}
      else {
        goto L21168;}
     L21168: {
      merged_arg1K0 = 0;
      merged_arg0K1 = arg2_630X;
      merged_arg0K2 = RSvalS;
      goto raise_exception2;}}
      break;
  }}
 return_values: {
  long nargs_647X = merged_arg0K0;
  extern long Sbottom_of_stackS;
  extern long SvalS;
  extern long StemplateS;
  extern long Scode_pointerS;
  extern long SenvS;
  extern long ScontS;
  extern long SstackS;
  extern char Tinterpret();
  long arg0K0;
  if ((1 == nargs_647X)) {
    long cont_648X;
    long pc_649X;
    long tem_650X;
    RSstackS = (4 + RSstackS);
    RSvalS = (*((long*)((unsigned char*)RSstackS)));
    cont_648X = ScontS;
    pc_649X = *((long*)((unsigned char*)(1 + cont_648X)));
    tem_650X = *((long*)((unsigned char*)(5 + cont_648X)));
    StemplateS = tem_650X;
    RScode_pointerS = ((-3 + (*((long*)((unsigned char*)(-3 + tem_650X))))) + (pc_649X >> 2));
    SenvS = (*((long*)((unsigned char*)(9 + cont_648X))));
    ScontS = (*((long*)((unsigned char*)(-3 + cont_648X))));
    RSstackS = (9 + cont_648X);
    goto START;}
  else {
    if ((ScontS == Sbottom_of_stackS)) {
      long obj_651X;
      obj_651X = *((long*)((unsigned char*)(-3 + ScontS)));
      if ((3 == (3 & obj_651X))) {
        long x_652X;
        x_652X = 31 & ((*((long*)((unsigned char*)(-7 + obj_651X)))) >> 2);
        if ((8 == x_652X)) {
          merged_arg0K0 = (*((long*)((unsigned char*)(-3 + ScontS))));
          merged_arg0K1 = nargs_647X;
          goto really_return_values;}
        else {
          goto L32525;}}
      else {
        goto L32525;}
     L32525: {
      if ((64 == nargs_647X)) {
        long v_653X;
        RSstackS = (4 + RSstackS);
        merged_arg0K0 = (*((long*)((unsigned char*)RSstackS)));
        merged_arg0K1 = 63;
        pop_args_list_return_tag = 7;
        goto pop_args_list;
       pop_args_list_return_7:
        v_653X = pop_args_list_return_value;
        arg0K0 = v_653X;
        goto L32553;}
      else {
        long v_654X;
        merged_arg0K0 = 25;
        merged_arg0K1 = nargs_647X;
        pop_args_list_return_tag = 8;
        goto pop_args_list;
       pop_args_list_return_8:
        v_654X = pop_args_list_return_value;
        arg0K0 = v_654X;
        goto L32553;}
     L32553: {
      long args_655X = arg0K0;
      merged_arg1K0 = 0;
      merged_arg0K1 = args_655X;
      goto raise_exception1;}}}
    else {
      merged_arg0K0 = ScontS;
      merged_arg0K1 = nargs_647X;
      goto really_return_values;}}}
 really_return_values: {
  long cont_656X = merged_arg0K0;
  long nargs_657X = merged_arg0K1;
  extern long Sbottom_of_stackS;
  extern long SnargsS;
  extern long SvalS;
  extern long StemplateS;
  extern long Scode_pointerS;
  extern long SenvS;
  extern long ScontS;
  extern long SstackS;
  extern char Tinterpret();
  long arg0K0;
  unsigned char op_658X;
  op_658X = *((unsigned char*)((unsigned char*)((-3 + (*((long*)((unsigned char*)(-3 + (*((long*)((unsigned char*)(5 + cont_656X))))))))) + ((*((long*)((unsigned char*)(1 + cont_656X)))) >> 2))));
  if ((31 == ((long)op_658X))) {
    long cont_659X;
    long pc_660X;
    long tem_661X;
    cont_659X = ScontS;
    pc_660X = *((long*)((unsigned char*)(1 + cont_659X)));
    tem_661X = *((long*)((unsigned char*)(5 + cont_659X)));
    StemplateS = tem_661X;
    RScode_pointerS = ((-3 + (*((long*)((unsigned char*)(-3 + tem_661X))))) + (pc_660X >> 2));
    SenvS = (*((long*)((unsigned char*)(9 + cont_659X))));
    ScontS = (*((long*)((unsigned char*)(-3 + cont_659X))));
    RSstackS = (9 + cont_659X);
    goto START;}
  else {
    if ((27 == ((long)op_658X))) {
      long x_662X;
      long y_663X;
      x_662X = ScontS;
      y_663X = Sbottom_of_stackS;
      if ((x_662X == y_663X)) {
        long cont_664X;
        cont_664X = *((long*)((unsigned char*)(-3 + ScontS)));
        *((long*)((unsigned char*)(-3 + ScontS))) = (*((long*)((unsigned char*)(-3 + cont_664X))));
        arg0K0 = cont_664X;
        goto L32895;}
      else {
        long cont_665X;
        cont_665X = ScontS;
        ScontS = (*((long*)((unsigned char*)(-3 + cont_665X))));
        arg0K0 = cont_665X;
        goto L32895;}
     L32895: {
      long cont_666X = arg0K0;
      SnargsS = nargs_657X;
      RSvalS = (*((long*)((unsigned char*)(13 + cont_666X))));
      merged_arg1K0 = 0;
      goto perform_application;}}
    else {
      if ((64 == nargs_657X)) {
        long v_667X;
        RSstackS = (4 + RSstackS);
        merged_arg0K0 = (*((long*)((unsigned char*)RSstackS)));
        merged_arg0K1 = 63;
        pop_args_list_return_tag = 9;
        goto pop_args_list;
       pop_args_list_return_9:
        v_667X = pop_args_list_return_value;
        arg0K0 = v_667X;
        goto L33303;}
      else {
        long v_668X;
        merged_arg0K0 = 25;
        merged_arg0K1 = nargs_657X;
        pop_args_list_return_tag = 10;
        goto pop_args_list;
       pop_args_list_return_10:
        v_668X = pop_args_list_return_value;
        arg0K0 = v_668X;
        goto L33303;}
     L33303: {
      long args_669X = arg0K0;
      merged_arg1K0 = 0;
      merged_arg0K1 = args_669X;
      goto raise_exception1;}}}}
 raise_exception2: {
  char args_670X = merged_arg1K0;
  long a1_671X = merged_arg0K1;
  long a2_672X = merged_arg0K2;
  extern void start_exception(char);
  extern long SstackS;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  start_exception(args_670X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  *((long*)((unsigned char*)RSstackS)) = a1_671X;
  RSstackS = (-4 + RSstackS);
  *((long*)((unsigned char*)RSstackS)) = a2_672X;
  RSstackS = (-4 + RSstackS);
  merged_arg1K0 = 2;
  goto raise;}
 raise_exception1: {
  char args_673X = merged_arg1K0;
  long a1_674X = merged_arg0K1;
  extern void start_exception(char);
  extern long SstackS;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  start_exception(args_673X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  *((long*)((unsigned char*)RSstackS)) = a1_674X;
  RSstackS = (-4 + RSstackS);
  merged_arg1K0 = 1;
  goto raise;}
 raise_exception: {
  char args_675X = merged_arg1K0;
  extern void start_exception(char);
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  start_exception(args_675X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  merged_arg1K0 = 0;
  goto raise;}
 raise_exception3: {
  char args_676X = merged_arg1K0;
  long a1_677X = merged_arg0K1;
  long a2_678X = merged_arg0K2;
  long a3_679X = merged_arg0K3;
  extern void start_exception(char);
  extern long SstackS;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  start_exception(args_676X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  *((long*)((unsigned char*)RSstackS)) = a1_677X;
  RSstackS = (-4 + RSstackS);
  *((long*)((unsigned char*)RSstackS)) = a2_678X;
  RSstackS = (-4 + RSstackS);
  *((long*)((unsigned char*)RSstackS)) = a3_679X;
  RSstackS = (-4 + RSstackS);
  merged_arg1K0 = 3;
  goto raise;}
 raise_exception4: {
  char args_680X = merged_arg1K0;
  long a1_681X = merged_arg0K1;
  long a2_682X = merged_arg0K2;
  long a3_683X = merged_arg0K3;
  long a4_684X = merged_arg0K4;
  extern void start_exception(char);
  extern long SstackS;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  start_exception(args_680X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  *((long*)((unsigned char*)RSstackS)) = a1_681X;
  RSstackS = (-4 + RSstackS);
  *((long*)((unsigned char*)RSstackS)) = a2_682X;
  RSstackS = (-4 + RSstackS);
  *((long*)((unsigned char*)RSstackS)) = a3_683X;
  RSstackS = (-4 + RSstackS);
  *((long*)((unsigned char*)RSstackS)) = a4_684X;
  RSstackS = (-4 + RSstackS);
  merged_arg1K0 = 4;
  goto raise;}
 handle_interrupt: {
  extern long Spending_interruptsS;
  extern long SenvS;
  extern long Sinterrupt_templateS;
  extern long StemplateS;
  extern long Scode_pointerS;
  extern void push_continuation(long, long, char);
  extern long SnargsS;
  extern long Sinterrupt_handlersS;
  extern long Senabled_interruptsS;
  extern long SvalS;
  extern void Qps_error_1(const char*, long);
  extern void Qps_error_0(const char*);
  extern long SstackS;
  extern long Sstack_limitS;
  static const struct{long length; char contents[35];} XX685
     = {34, "interrupt handler is not a closure"};
  static const struct{long length; char contents[34];} XX686
     = {33, "interrupt handler is not a vector"};
  char arg1K0;
  long arg0K1;
  long arg0K0;
  unsigned char x_687X;
  x_687X = Sstack_limitS < (-276 + RSstackS);
  if (x_687X) {
    arg1K0 = 0;
    goto L33027;}
  else {
    merged_arg1K0 = 69;
    get_stack_space_return_tag = 2;
    goto get_stack_space;
   get_stack_space_return_2:
    arg1K0 = 0;
    goto L33027;}
 L33027: {
  char key_688X = arg1K0;
  long n_689X;
  n_689X = Spending_interruptsS & Senabled_interruptsS;
  arg0K0 = 0;
  arg0K1 = 1;
  goto L33228;
 L33228: {
  long i_690X = arg0K0;
  long m_691X = arg0K1;
  if ((0 == (n_689X & m_691X))) {
    arg0K0 = (1 + i_690X);
    arg0K1 = (m_691X << 1);
    goto L33228;}
  else {
    long tem_692X;
    long obj_693X;
    Spending_interruptsS = (n_689X & (~ m_691X));
    *((long*)((unsigned char*)RSstackS)) = StemplateS;
    RSstackS = (-4 + RSstackS);
    *((long*)((unsigned char*)RSstackS)) = SenvS;
    RSstackS = (-4 + RSstackS);
    *((long*)((unsigned char*)RSstackS)) = (SnargsS << 2);
    RSstackS = (-4 + RSstackS);
    *((long*)((unsigned char*)RSstackS)) = (Senabled_interruptsS << 2);
    RSstackS = (-4 + RSstackS);
    tem_692X = Sinterrupt_templateS;
    StemplateS = tem_692X;
    RScode_pointerS = (-3 + (*((long*)((unsigned char*)(-3 + tem_692X)))));
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    push_continuation(RScode_pointerS, (4 + SnargsS), key_688X);
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    *((long*)((unsigned char*)RSstackS)) = (Senabled_interruptsS << 2);
    RSstackS = (-4 + RSstackS);
    SnargsS = 1;
    obj_693X = Sinterrupt_handlersS;
    if ((3 == (3 & obj_693X))) {
      long x_694X;
      x_694X = 31 & ((*((long*)((unsigned char*)(-7 + obj_693X)))) >> 2);
      if ((2 == x_694X)) {
        goto L33066;}
      else {
        goto L33138;}}
    else {
      goto L33138;}
   L33138: {
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    Qps_error_0((XX686.contents));
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    goto L33066;}
   L33066: {
    long obj_695X;
    RSvalS = (*((long*)((unsigned char*)((-3 + Sinterrupt_handlersS) + (i_690X << 2)))));
    Senabled_interruptsS = 0;
    obj_695X = RSvalS;
    if ((3 == (3 & obj_695X))) {
      long x_696X;
      x_696X = 31 & ((*((long*)((unsigned char*)(-7 + obj_695X)))) >> 2);
      if ((3 == x_696X)) {
        goto L33082;}
      else {
        goto L33092;}}
    else {
      goto L33092;}
   L33092: {
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    Qps_error_1((XX685.contents), i_690X);
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    goto L33082;}
   L33082: {
    merged_arg1K0 = 0;
    goto perform_application;}}}}}}
 raise_exception_no_cont2: {
  char args_697X = merged_arg1K0;
  long a1_698X = merged_arg0K1;
  long a2_699X = merged_arg0K2;
  extern long Scode_pointerS;
  extern long SstackS;
  *((long*)((unsigned char*)RSstackS)) = (((long)(*((unsigned char*)((unsigned char*)(-1 + (RScode_pointerS - ((long)args_697X))))))) << 2);
  RSstackS = (-4 + RSstackS);
  *((long*)((unsigned char*)RSstackS)) = a1_698X;
  RSstackS = (-4 + RSstackS);
  *((long*)((unsigned char*)RSstackS)) = a2_699X;
  RSstackS = (-4 + RSstackS);
  merged_arg1K0 = 2;
  goto raise;}
 raise: {
  char nargs_700X = merged_arg1K0;
  extern long SnargsS;
  extern long Sexception_handlerS;
  extern long SvalS;
  extern long Sstack_beginS;
  extern unsigned char Sexception_space_usedPS;
  extern long SstackS;
  extern long Sstack_limitS;
  extern void Qps_error_0(const char*);
  static const struct{long length; char contents[35];} XX701
     = {34, "exception handler is not a closure"};
  long obj_702X;
  SnargsS = (1 + ((long)nargs_700X));
  RSvalS = Sexception_handlerS;
  obj_702X = RSvalS;
  if ((3 == (3 & obj_702X))) {
    long x_703X;
    x_703X = 31 & ((*((long*)((unsigned char*)(-7 + obj_702X)))) >> 2);
    if ((3 == x_703X)) {
      goto L32950;}
    else {
      goto L32981;}}
  else {
    goto L32981;}
 L32981: {
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  Qps_error_0((XX701.contents));
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  goto L32950;}
 L32950: {
  if (Sexception_space_usedPS) {
    unsigned char x_704X;
    x_704X = Sstack_limitS < (-296 + RSstackS);
    if (x_704X) {
      goto L32959;}
    else {
      merged_arg1K0 = 74;
      get_stack_space_return_tag = 3;
      goto get_stack_space;
     get_stack_space_return_3:
      goto L32959;}
   L32959: {
    Sstack_limitS = (296 + Sstack_beginS);
    Sexception_space_usedPS = 0;
    goto L32952;}}
  else {
    goto L32952;}
 L32952: {
  merged_arg1K0 = 0;
  goto perform_application;}}}
 perform_application: {
  char bytes_consumed_705X = merged_arg1K0;
  extern long SnargsS;
  extern long SstackS;
  extern long SenvS;
  extern long SvalS;
  extern long StemplateS;
  extern long Scode_pointerS;
  extern long Senabled_interruptsS;
  extern long Spending_interruptsS;
  extern char Tinterpret();
  long arg0K0;
  long obj_706X;
  ALARM_CHECK			/* Mobot patch  -JAR */
  obj_706X = RSvalS;
  if ((3 == (3 & obj_706X))) {
    long x_707X;
    x_707X = 31 & ((*((long*)((unsigned char*)(-7 + obj_706X)))) >> 2);
    if ((3 == x_707X)) {
      long tem_708X;
      SenvS = (*((long*)((unsigned char*)(1 + RSvalS))));
      tem_708X = *((long*)((unsigned char*)(-3 + RSvalS)));
      StemplateS = tem_708X;
      RScode_pointerS = (-3 + (*((long*)((unsigned char*)(-3 + tem_708X)))));
      if ((0 == (Spending_interruptsS & Senabled_interruptsS))) {
        goto START;}
      else {
        goto handle_interrupt;}}
    else {
      goto L32653;}}
  else {
    goto L32653;}
 L32653: {
  long x_709X;
  x_709X = SnargsS;
  if ((64 == x_709X)) {
    long v_710X;
    RSstackS = (4 + RSstackS);
    merged_arg0K0 = (*((long*)((unsigned char*)RSstackS)));
    merged_arg0K1 = 63;
    pop_args_list_return_tag = 11;
    goto pop_args_list;
   pop_args_list_return_11:
    v_710X = pop_args_list_return_value;
    arg0K0 = v_710X;
    goto L33396;}
  else {
    long v_711X;
    merged_arg0K0 = 25;
    merged_arg0K1 = SnargsS;
    pop_args_list_return_tag = 12;
    goto pop_args_list;
   pop_args_list_return_12:
    v_711X = pop_args_list_return_value;
    arg0K0 = v_711X;
    goto L33396;}
 L33396: {
  long args_712X = arg0K0;
  merged_arg1K0 = bytes_consumed_705X;
  merged_arg0K1 = RSvalS;
  merged_arg0K2 = args_712X;
  goto raise_exception_no_cont2;}}}
 pop_args_list: {
  long start_713X = merged_arg0K0;
  long count_714X = merged_arg0K1;
  extern long SnargsS;
  extern long SstackS;
  extern long ShpS;
  long arg0K1;
  long arg0K0;
  merged_arg0K0 = (3 * SnargsS);
  ensure_space_return_tag = 9;
  goto ensure_space;
 ensure_space_return_9:
  arg0K0 = start_713X;
  arg0K1 = count_714X;
  goto L21691;
 L21691: {
  long args_715X = arg0K0;
  long count_716X = arg0K1;
  if ((0 < count_716X)) {
    long a_717X;
    long new_718X;
    RSstackS = (4 + RSstackS);
    a_717X = *((long*)((unsigned char*)RSstackS));
    *((long*)((unsigned char*)ShpS)) = 2050;
    ShpS = (4 + ShpS);
    new_718X = 3 + ShpS;
    ShpS = (8 + ShpS);
    *((long*)((unsigned char*)(-4 + ShpS))) = 0;
    *((long*)((unsigned char*)(-3 + new_718X))) = a_717X;
    *((long*)((unsigned char*)(1 + new_718X))) = args_715X;
    arg0K0 = new_718X;
    arg0K1 = (-1 + count_716X);
    goto L21691;}
  else {
    pop_args_list_return_value = args_715X;
    goto pop_args_list_return;}}}
 pop_args_list_return:
  switch (pop_args_list_return_tag) {
  case 0: goto pop_args_list_return_0;
  case 1: goto pop_args_list_return_1;
  case 2: goto pop_args_list_return_2;
  case 3: goto pop_args_list_return_3;
  case 4: goto pop_args_list_return_4;
  case 5: goto pop_args_list_return_5;
  case 6: goto pop_args_list_return_6;
  case 7: goto pop_args_list_return_7;
  case 8: goto pop_args_list_return_8;
  case 9: goto pop_args_list_return_9;
  case 10: goto pop_args_list_return_10;
  case 11: goto pop_args_list_return_11;
  case 12: goto pop_args_list_return_12;
  }
 ensure_space: {
  long space_719X = merged_arg0K0;
  extern long SlimitS;
  extern long ShpS;
  extern void Qps_error_0(const char*);
  static const struct{long length; char contents[23];} XX720
     = {22, "Scheme48 heap overflow"};
  unsigned char x_721X;
  x_721X = (ShpS + (space_719X << 2)) < SlimitS;
  if (x_721X) {
    goto L21081;}
  else {
    interpreter_collect_return_tag = 1;
    goto interpreter_collect;
   interpreter_collect_return_1:
    goto L21081;}
 L21081: {
  unsigned char v_722X;
  v_722X = (ShpS + (space_719X << 2)) < SlimitS;
  if (v_722X) {
    ensure_space_return_value = 0;
    goto ensure_space_return;}
  else {
    char v_723X;
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    v_723X = 0; /* JAR HACK */ Qps_error_0((XX720.contents));
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    ensure_space_return_value = v_723X;
    goto ensure_space_return;}}}
 ensure_space_return:
  switch (ensure_space_return_tag) {
  case 0: goto ensure_space_return_0;
  case 1: goto ensure_space_return_1;
  case 2: goto ensure_space_return_2;
  case 3: goto ensure_space_return_3;
  case 4: goto ensure_space_return_4;
  case 5: goto ensure_space_return_5;
  case 6: goto ensure_space_return_6;
  case 7: goto ensure_space_return_7;
  case 8: goto ensure_space_return_8;
  case 9: goto ensure_space_return_9;
  }
 get_stack_space: {
  char cells_724X = merged_arg1K0;
  extern long preserve_continuation(long, char, char);
  extern long ScontS;
  extern long Sstack_beginS;
  extern long Sstack_limitS;
  extern long Sbottom_of_stackS;
  extern long SlimitS;
  extern long ShpS;
  extern long SstackS;
  extern long Sstack_endS;
  extern void Qps_error_1(const char*, long);
  static const struct{long length; char contents[15];} XX725
     = {14, "stack overflow"};
  static const struct{long length; char contents[34];} XX726
     = {33, "not enough room in heap for stack"};
  long arg0K1;
  long arg0K0;
  long cells_727X;
  unsigned char x_728X;
  cells_727X = 1 + ((Sstack_endS - RSstackS) >> 2);
  x_728X = (ShpS + (cells_727X << 2)) < SlimitS;
  if (x_728X) {
    goto L28177;}
  else {
    long cells_729X;
    unsigned char x_730X;
    interpreter_collect_return_tag = 2;
    goto interpreter_collect;
   interpreter_collect_return_2:
    cells_729X = 1 + ((Sstack_endS - RSstackS) >> 2);
    x_730X = (ShpS + (cells_729X << 2)) < SlimitS;
    if (x_730X) {
      goto L28177;}
    else {
      long v_731X;
      v_731X = 1 + ((Sstack_endS - RSstackS) >> 2);
      SvalS = RSvalS;
      SstackS = RSstackS;
      Scode_pointerS = RScode_pointerS;
      Qps_error_1((XX726.contents), v_731X);
      RSvalS = SvalS;
      RSstackS = SstackS;
      RScode_pointerS = Scode_pointerS;
      goto L28177;}}
 L28177: {
  arg0K0 = (4 + RSstackS);
  arg0K1 = 0;
  goto L16382;
 L16382: {
  long p_732X = arg0K0;
  long i_733X = arg0K1;
  if ((2 == (3 & (*((long*)((unsigned char*)p_732X)))))) {
    long len_734X;
    long new_735X;
    len_734X = i_733X << 2;
    *((long*)((unsigned char*)ShpS)) = (10 + (len_734X << 8));
    ShpS = (4 + ShpS);
    new_735X = 3 + ShpS;
    ShpS = (ShpS + (-4 & (3 + len_734X)));
    if ((0 < len_734X)) {
      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
      arg0K0 = new_735X;
      goto L16221;}
    else {
      arg0K0 = new_735X;
      goto L16221;}
   L16221: {
    long vec_736X = arg0K0;
    arg0K0 = (-1 + i_733X);
    goto L16227;
   L16227: {
    long i_737X = arg0K0;
    if ((-1 < i_737X)) {
      RSstackS = (4 + RSstackS);
      *((long*)((unsigned char*)((-3 + vec_736X) + (i_737X << 2)))) = (*((long*)((unsigned char*)RSstackS)));
      arg0K0 = (-1 + i_737X);
      goto L16227;}
    else {
      long cont_738X;
      SvalS = RSvalS;
      SstackS = RSstackS;
      Scode_pointerS = RScode_pointerS;
      cont_738X = preserve_continuation(ScontS, 0, 1);
      RSvalS = SvalS;
      RSstackS = SstackS;
      RScode_pointerS = Scode_pointerS;
      if ((1 == (255 & cont_738X))) {
        RSstackS = (-11 + Sbottom_of_stackS);
        Sstack_limitS = (296 + Sstack_beginS);
        *((long*)((unsigned char*)(-3 + Sbottom_of_stackS))) = 1;
        arg0K0 = Sbottom_of_stackS;
        goto L16255;}
      else {
        long v_739X;
        merged_arg0K0 = cont_738X;
        copy_continuation_from_heap_return_tag = 3;
        goto copy_continuation_from_heap;
       copy_continuation_from_heap_return_3:
        v_739X = copy_continuation_from_heap_return_value;
        arg0K0 = v_739X;
        goto L16255;}
     L16255: {
      long v_740X = arg0K0;
      ScontS = v_740X;
      arg0K0 = 0;
      goto L16261;
     L16261: {
      long i_741X = arg0K0;
      if ((i_741X < i_733X)) {
        *((long*)((unsigned char*)RSstackS)) = (*((long*)((unsigned char*)((-3 + vec_736X) + (i_741X << 2)))));
        RSstackS = (-4 + RSstackS);
        arg0K0 = (1 + i_741X);
        goto L16261;}
      else {
        unsigned char x_742X;
        x_742X = Sstack_limitS < (RSstackS - (((long)cells_724X) << 2));
        if (x_742X) {
          goto get_stack_space_return;}
        else {
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          Qps_error_1((XX725.contents), ((long)cells_724X));
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          goto get_stack_space_return;}}}}}}}}
  else {
    arg0K0 = (4 + p_732X);
    arg0K1 = (1 + i_733X);
    goto L16382;}}}}
 get_stack_space_return:
  switch (get_stack_space_return_tag) {
  case 0: goto get_stack_space_return_0;
  case 1: goto get_stack_space_return_1;
  case 2: goto get_stack_space_return_2;
  case 3: goto get_stack_space_return_3;
  }
 interpreter_collect: {
  long v_743X;
  merged_arg0K0 = 0;
  collect_saving_temp_return_tag = 4;
  goto collect_saving_temp;
 collect_saving_temp_return_4:
  v_743X = collect_saving_temp_return_value;
  interpreter_collect_return_value = v_743X;
  goto interpreter_collect_return;}
 interpreter_collect_return:
  switch (interpreter_collect_return_tag) {
  case 0: goto interpreter_collect_return_0;
  case 1: goto interpreter_collect_return_1;
  case 2: goto interpreter_collect_return_2;
  }
 collect_saving_temp: {
  long value_744X = merged_arg0K0;
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern long Ssaved_limitS;
  extern long Ssaved_hpS;
  extern long Snewspace_endS;
  extern long Snewspace_beginS;
  extern long Sweak_pointer_hpS;
  extern long StemplateS;
  extern long Scode_pointerS;
  extern long SenvS;
  extern long SvalS;
  extern long Sdynamic_stateS;
  extern long Sexception_handlerS;
  extern long Sinterrupt_handlersS;
  extern long Sinterrupt_templateS;
  extern long ScontS;
  extern void trace_stack(long);
  extern long trace_value(long);
  extern void end_collection();
  extern long SlimitS;
  extern long Sminimum_recovered_spaceS;
  extern long ShpS;
  extern long Spending_interruptsS;
  long b_745X;
  long e_746X;
  long pc_747X;
  long tem_748X;
  long v_749X;
  long v_750X;
  long v_751X;
  long v_752X;
  long v_753X;
  long v_754X;
  long value_755X;
  unsigned char x_756X;
  b_745X = Snewspace_beginS;
  Snewspace_beginS = Soldspace_beginS;
  Soldspace_beginS = b_745X;
  e_746X = Snewspace_endS;
  Snewspace_endS = Soldspace_endS;
  Soldspace_endS = e_746X;
  Ssaved_limitS = SlimitS;
  Ssaved_hpS = ShpS;
  SlimitS = Snewspace_endS;
  ShpS = Snewspace_beginS;
  Sweak_pointer_hpS = -1;
  pc_747X = 12 + ((RScode_pointerS - (*((long*)((unsigned char*)(-3 + StemplateS))))) << 2);
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  tem_748X = trace_value(StemplateS);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  StemplateS = tem_748X;
  RScode_pointerS = ((-3 + (*((long*)((unsigned char*)(-3 + tem_748X))))) + (pc_747X >> 2));
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  v_749X = trace_value(SenvS);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  SenvS = v_749X;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  v_750X = trace_value(RSvalS);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  RSvalS = v_750X;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  v_751X = trace_value(Sdynamic_stateS);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  Sdynamic_stateS = v_751X;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  v_752X = trace_value(Sexception_handlerS);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  Sexception_handlerS = v_752X;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  v_753X = trace_value(Sinterrupt_handlersS);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  Sinterrupt_handlersS = v_753X;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  v_754X = trace_value(Sinterrupt_templateS);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  Sinterrupt_templateS = v_754X;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  trace_stack(ScontS);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  value_755X = trace_value(value_744X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  end_collection();
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  x_756X = (ShpS + (Sminimum_recovered_spaceS << 2)) < SlimitS;
  if (x_756X) {
    collect_saving_temp_return_value = value_755X;
    goto collect_saving_temp_return;}
  else {
    Spending_interruptsS = (4 | Spending_interruptsS);
    collect_saving_temp_return_value = value_755X;
    goto collect_saving_temp_return;}}
 collect_saving_temp_return:
  switch (collect_saving_temp_return_tag) {
  case 0: goto collect_saving_temp_return_0;
  case 1: goto collect_saving_temp_return_1;
  case 2: goto collect_saving_temp_return_2;
  case 3: goto collect_saving_temp_return_3;
  case 4: goto collect_saving_temp_return_4;
  }
 copy_continuation_from_heap: {
  long cont_757X = merged_arg0K0;
  extern long SstackS;
  extern long Sbottom_of_stackS;
  long arg0K1;
  long arg0K0;
  long cells_758X;
  long top_759X;
  long new_cont_760X;
  long y_761X;
  long from_762X;
  cells_758X = (3 + ((*((long*)((unsigned char*)(-7 + cont_757X)))) >> 8)) >> 2;
  top_759X = (-11 + Sbottom_of_stackS) - (cells_758X << 2);
  new_cont_760X = 7 + top_759X;
  RSstackS = (-4 + top_759X);
  y_761X = (3 + ((*((long*)((unsigned char*)(-7 + cont_757X)))) >> 8)) >> 2;
  from_762X = -7 + cont_757X;
  arg0K0 = from_762X;
  arg0K1 = top_759X;
  goto L13879;
 L13879: {
  long from_763X = arg0K0;
  long to_764X = arg0K1;
  if ((from_763X < (from_762X + (4 + (y_761X << 2))))) {
    *((long*)((unsigned char*)to_764X)) = (*((long*)((unsigned char*)from_763X)));
    arg0K0 = (4 + from_763X);
    arg0K1 = (4 + to_764X);
    goto L13879;}
  else {
    *((long*)((unsigned char*)(-3 + Sbottom_of_stackS))) = (*((long*)((unsigned char*)(-3 + new_cont_760X))));
    *((long*)((unsigned char*)(-3 + new_cont_760X))) = Sbottom_of_stackS;
    copy_continuation_from_heap_return_value = new_cont_760X;
    goto copy_continuation_from_heap_return;}}}
 copy_continuation_from_heap_return:
  switch (copy_continuation_from_heap_return_tag) {
/*
  case 0: goto copy_continuation_from_heap_return_0;
*/
  case 1: goto copy_continuation_from_heap_return_1;
  case 2: goto copy_continuation_from_heap_return_2;
  case 3: goto copy_continuation_from_heap_return_3;
  }
 extract_string: {
  long string_765X = merged_arg0K0;
  long arg0K0;
  long z_766X;
  char *string_767X;
  char *vec_768X;
  z_766X = (*((long*)((unsigned char*)(-7 + string_765X)))) >> 8;
  string_767X = (char*)malloc(sizeof(char*) * (5 + z_766X));
  *((long*)((unsigned char*)string_767X)) = z_766X;
  *((string_767X + (4 + z_766X))) = 0;
  vec_768X = string_767X + 4;
  arg0K0 = 0;
  goto L12474;
 L12474: {
  long i_769X = arg0K0;
  if ((i_769X < z_766X)) {
    char value_770X;
    value_770X = (*((unsigned char*)((unsigned char*)((-3 + string_765X) + i_769X))));
    *((vec_768X + i_769X)) = value_770X;
    arg0K0 = (1 + i_769X);
    goto L12474;}
  else {
    extract_string_return_value = vec_768X;
    goto extract_string_return;}}}
 extract_string_return:
  switch (extract_string_return_tag) {
  case 0: goto extract_string_return_0;
  case 1: goto extract_string_return_1;
  case 2: goto extract_string_return_2;
  }
 stob_equalP: {
  long stob1_771X = merged_arg0K0;
  long stob2_772X = merged_arg0K1;
  long arg0K0;
  long z2_773X;
  long z1_774X;
  z2_773X = *((long*)((unsigned char*)(-7 + stob2_772X)));
  z1_774X = *((long*)((unsigned char*)(-7 + stob1_771X)));
  if (((128 | z1_774X) == (128 | z2_773X))) {
    arg0K0 = 0;
    goto L9648;
   L9648: {
    long i_775X = arg0K0;
    if ((i_775X < ((3 + (z1_774X >> 8)) >> 2))) {
      if (((*((long*)((unsigned char*)((-3 + stob1_771X) + (i_775X << 2))))) == (*((long*)((unsigned char*)((-3 + stob2_772X) + (i_775X << 2))))))) {
        arg0K0 = (1 + i_775X);
        goto L9648;}
      else {
        stob_equalP_return_value = 0;
        goto stob_equalP_return;}}
    else {
      stob_equalP_return_value = 1;
      goto stob_equalP_return;}}}
  else {
    stob_equalP_return_value = 0;
    goto stob_equalP_return;}}
 stob_equalP_return:
  switch (stob_equalP_return_tag) {
  case 0: goto stob_equalP_return_0;
  case 1: goto stob_equalP_return_1;
  }
 write_number_no_newline: {
  long x_776X = merged_arg0K0;
  FILE * port_777X = merged_arg3K1;
  long arg0K1;
  long arg0K0;
  if ((x_776X < 0)) {
    putc(45, port_777X);
    arg0K0 = (0 - x_776X);
    goto L4985;}
  else {
    arg0K0 = x_776X;
    goto L4985;}
 L4985: {
  long x_778X = arg0K0;
  arg0K0 = x_778X;
  arg0K1 = 1;
  goto L5030;
 L5030: {
  long x_779X = arg0K0;
  long mask_780X = arg0K1;
  if ((x_779X < 10)) {
    arg0K0 = x_778X;
    arg0K1 = mask_780X;
    goto L4990;}
  else {
    long v_781X;
    v_781X = x_779X / 10;
    arg0K0 = v_781X;
    arg0K1 = (10 * mask_780X);
    goto L5030;}}
 L4990: {
  long x_782X = arg0K0;
  long mask_783X = arg0K1;
  long digit_784X;
  digit_784X = x_782X / mask_783X;
  putc((((char)(48 + digit_784X))), port_777X);
  if ((1 < mask_783X)) {
    long v_785X;
    long v_786X;
    v_785X = x_782X % mask_783X;
    v_786X = mask_783X / 10;
    arg0K0 = v_785X;
    arg0K1 = v_786X;
    goto L4990;}
  else {
    goto write_number_no_newline_return;}}}}
 write_number_no_newline_return:
  switch (write_number_no_newline_return_tag) {
  case 0: goto write_number_no_newline_return_0;
  case 1: goto write_number_no_newline_return_1;
  case 2: goto write_number_no_newline_return_2;
  case 3: goto write_number_no_newline_return_3;
  case 4: goto write_number_no_newline_return_4;
  }}

long resume(char *filename_787X, char *startup_string_788X)
{
  extern long StemplateS;
  extern long SenvS;
  extern long ScontS;
  extern long SnargsS;
  extern long SvalS;
  extern long Sdynamic_stateS;
  extern long Sexception_handlerS;
  extern long Sinterrupt_handlersS;
  extern long Senabled_interruptsS;
  extern long Spending_interruptsS;
  extern long Scode_pointerS;
  extern long restart(long);
  extern long Sinitial_heap_sizeS;
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern long Sstack_sizeS;
  extern long SlimitS;
  extern long Snewspace_beginS;
  extern long Snewspace_endS;
  extern FILE * Scurrent_input_portS;
  extern FILE * *Sopen_portsS;
  extern long *Sopen_vm_portsS;
  extern long Sinterrupt_templateS;
  extern long Sstack_endS;
  extern long Sstack_beginS;
  extern long Sstack_limitS;
  extern long SstackS;
  extern long Sbottom_of_stackS;
  extern void write_string(const char*, FILE *);
  extern FILE * Scurrent_output_portS;
  extern void Qps_error_1(const char*, long);
  extern long ShpS;
  extern long Smemory_beginS;
  extern unsigned char *SmemoryS;
  extern long Smemory_endS;
  extern void Qps_error_2(const char*, long, long);
  extern void Qps_error_0(const char*);
  static const struct{long length; char contents[42];} XX789
     = {41, "heap not big enough to restore this image"};
  static const struct{long length; char contents[3];} XX790
     = {2, "so"};
  static const struct{long length; char contents[3];} XX791
     = {2, "si"};
  static const struct{long length; char contents[40];} XX792
     = {39, "Correcting byte order of resumed image."};
  static const struct{long length; char contents[36];} XX793
     = {35, "block read did not get enough bytes"};
  static const struct{long length; char contents[29];} XX794
     = {28, "unable to correct byte order"};
  static const struct{long length; char contents[28];} XX795
     = {27, "incompatible bytes-per-cell"};
  static const struct{long length; char contents[60];} XX796
     = {59, "format of image is incompatible with this version of system"};
  static const struct{long length; char contents[41];} XX797
     = {40, "end of file while looking for page break"};
  static const struct{long length; char contents[27];} XX798
     = {26, "Can't open heap image file"};
  unsigned char arg2K0;
  long arg0K0;
  const char *merged_arg4K0;  /* JAR HACK - added const */
  FILE * merged_arg3K0;

  int read_number_return_tag;
  long read_number_return_value;
  int enter_string_return_tag;
  long enter_string_return_value;
 {long command_line_space_799X;
  FILE * port_800X;
  command_line_space_799X = 3 + ((3 + (*((long*)(((unsigned char*)startup_string_788X) + -4)))) >> 2);
  port_800X = ps_open(filename_787X, "r");
  if ((NULL == port_800X)) {
    Qps_error_0((XX798.contents));
    goto L15524;}
  else {
    goto L15524;}
 L15524: {
  goto L15801;
 L15801: {
  int x_801X;
  int ch_802X;
  PS_GETC(port_800X, x_801X)
  ch_802X = x_801X;
  if (((EOF == ch_802X))) {
    Qps_error_0((XX797.contents));
    goto L15526;}
  else {
    if ((12 == ((char)ch_802X))) {
      goto L15526;}
    else {
      goto L15801;}}}
 L15526: {
  long old_level_803X;
  long old_bytes_per_cell_804X;
  long cells_805X;
  long old_begin_806X;
  long cells_807X;
  long old_hp_808X;
  long startup_proc_809X;
  merged_arg3K0 = port_800X;
  read_number_return_tag = 0;
  goto read_number;
 read_number_return_0:
  old_level_803X = read_number_return_value;
  merged_arg3K0 = port_800X;
  read_number_return_tag = 1;
  goto read_number;
 read_number_return_1:
  old_bytes_per_cell_804X = read_number_return_value;
  merged_arg3K0 = port_800X;
  read_number_return_tag = 2;
  goto read_number;
 read_number_return_2:
  cells_805X = read_number_return_value;
  old_begin_806X = cells_805X << 2;
  merged_arg3K0 = port_800X;
  read_number_return_tag = 3;
  goto read_number;
 read_number_return_3:
  cells_807X = read_number_return_value;
  old_hp_808X = cells_807X << 2;
  merged_arg3K0 = port_800X;
  read_number_return_tag = 4;
  goto read_number;
 read_number_return_4:
  startup_proc_809X = read_number_return_value;
  goto L15776;
 L15776: {
  int x_810X;
  int ch_811X;
  PS_GETC(port_800X, x_810X)
  ch_811X = x_810X;
  if (((EOF == ch_811X))) {
    Qps_error_0((XX797.contents));
    goto L15548;}
  else {
    if ((12 == ((char)ch_811X))) {
      goto L15548;}
    else {
      goto L15776;}}}
 L15548: {
  if ((14 == old_level_803X)) {
    goto L15560;}
  else {
    Qps_error_2((XX796.contents), old_level_803X, 14);
    goto L15560;}
 L15560: {
  if ((4 == old_bytes_per_cell_804X)) {
    goto L15572;}
  else {
    Qps_error_2((XX795.contents), old_bytes_per_cell_804X, 4);
    goto L15572;}
 L15572: {
  long size_812X;
  long size_813X;
  size_812X = Sinitial_heap_sizeS;
  size_813X = size_812X << 2;
  if ((size_813X == (Smemory_endS - Smemory_beginS))) {
    goto L7124;}
  else {
    SmemoryS = ((unsigned char*)malloc(size_813X));
    Smemory_beginS = ((long)SmemoryS);
    Smemory_endS = (((long)SmemoryS) + size_813X);
    goto L7124;}
 L7124: {
  long semisize_814X;
  long delta_815X;
  long new_hp_816X;
  long new_limit_817X;
  semisize_814X = (-2 & (((Smemory_endS - Smemory_beginS) >> 2) - Sstack_sizeS)) << 1;
  Snewspace_beginS = Smemory_beginS;
  Snewspace_endS = (Snewspace_beginS + semisize_814X);
  Soldspace_beginS = Snewspace_endS;
  Soldspace_endS = (Soldspace_beginS + semisize_814X);
  Sstack_beginS = Soldspace_endS;
  Sstack_endS = (Sstack_beginS + (Sstack_sizeS << 2));
  ShpS = Snewspace_beginS;
  SlimitS = Snewspace_endS;
  delta_815X = Snewspace_beginS - old_begin_806X;
  new_hp_816X = old_hp_808X + delta_815X;
  new_limit_817X = Snewspace_endS;
  if ((((20 + command_line_space_799X) + new_hp_816X) < new_limit_817X)) {
    long got_818X;
    got_818X = fread((void *)((unsigned char*)ShpS), sizeof(char), 4, port_800X);
    if ((4 == got_818X)) {
      goto L8617;}
    else {
      Qps_error_0((XX793.contents));
      goto L8617;}
   L8617: {
    if ((1 == (*((long*)((unsigned char*)ShpS))))) {
      arg2K0 = 0;
      goto L15600;}
    else {
      long addr_819X;
      unsigned char x_820X;
      unsigned char x_821X;
      addr_819X = ShpS;
      x_820X = *((unsigned char*)((unsigned char*)addr_819X));
      *((unsigned char*)((unsigned char*)addr_819X)) = (*((unsigned char*)((unsigned char*)(3 + addr_819X))));
      *((unsigned char*)((unsigned char*)(3 + addr_819X))) = x_820X;
      x_821X = *((unsigned char*)((unsigned char*)(1 + addr_819X)));
      *((unsigned char*)((unsigned char*)(1 + addr_819X))) = (*((unsigned char*)((unsigned char*)(2 + addr_819X))));
      *((unsigned char*)((unsigned char*)(2 + addr_819X))) = x_821X;
      if ((1 == (*((long*)((unsigned char*)ShpS))))) {
        arg2K0 = 1;
        goto L15600;}
      else {
        Qps_error_1((XX794.contents), (*((long*)((unsigned char*)ShpS))));
        arg2K0 = 0;
        goto L15600;}}}
   L15600: {
    unsigned char reverseP_822X = arg2K0;
    long count_823X;
    long got_824X;
    count_823X = old_hp_808X - old_begin_806X;
    got_824X = fread((void *)((unsigned char*)ShpS), sizeof(char), count_823X, port_800X);
    if ((got_824X == count_823X)) {
      goto L15604;}
    else {
      Qps_error_0((XX793.contents));
      goto L15604;}
   L15604: {
    if (reverseP_822X) {
      FILE * port_825X;
      write_string((XX792.contents), Scurrent_output_portS);
      port_825X = Scurrent_output_portS;
      putc(10, port_825X);
      if ((port_825X == Scurrent_output_portS)) {
        fflush(port_825X);
        goto L8869;}
      else {
        goto L8869;}
     L8869: {
      arg0K0 = ShpS;
      goto L8874;
     L8874: {
      long ptr_826X = arg0K0;
      unsigned char x_827X;
      unsigned char x_828X;
      long value_829X;
      x_827X = *((unsigned char*)((unsigned char*)ptr_826X));
      *((unsigned char*)((unsigned char*)ptr_826X)) = (*((unsigned char*)((unsigned char*)(3 + ptr_826X))));
      *((unsigned char*)((unsigned char*)(3 + ptr_826X))) = x_827X;
      x_828X = *((unsigned char*)((unsigned char*)(1 + ptr_826X)));
      *((unsigned char*)((unsigned char*)(1 + ptr_826X))) = (*((unsigned char*)((unsigned char*)(2 + ptr_826X))));
      *((unsigned char*)((unsigned char*)(2 + ptr_826X))) = x_828X;
      value_829X = *((long*)((unsigned char*)ptr_826X));
      if ((ptr_826X < new_hp_816X)) {
        if ((2 == (3 & value_829X))) {
          if (((31 & (value_829X >> 2)) < 15)) {
            arg0K0 = (4 + ptr_826X);
            goto L8900;}
          else {
            arg0K0 = ((4 + ptr_826X) + (-4 & (3 + (value_829X >> 8))));
            goto L8900;}}
        else {
          arg0K0 = (4 + ptr_826X);
          goto L8900;}
       L8900: {
        long v_830X = arg0K0;
        arg0K0 = v_830X;
        goto L8874;}}
      else {
        goto L15612;}}}}
    else {
      goto L15612;}
   L15612: {
    if ((0 == delta_815X)) {
      ShpS = new_hp_816X;
      goto L15623;}
    else {
      goto L8755;
     L8755: {
      if ((ShpS < new_hp_816X)) {
        long descriptor_831X;
        descriptor_831X = *((long*)((unsigned char*)ShpS));
        if ((3 == (3 & descriptor_831X))) {
          arg0K0 = (3 + ((-3 + descriptor_831X) + delta_815X));
          goto L8765;}
        else {
          arg0K0 = descriptor_831X;
          goto L8765;}
       L8765: {
        long d_832X = arg0K0;
        *((long*)((unsigned char*)ShpS)) = d_832X;
        ShpS = (4 + ShpS);
        if ((2 == (3 & d_832X))) {
          if (((31 & (d_832X >> 2)) < 15)) {
            goto L8784;}
          else {
            ShpS = (ShpS + (-4 & (3 + (d_832X >> 8))));
            goto L8784;}}
        else {
          goto L8784;}
       L8784: {
        goto L8755;}}}
      else {
        goto L15623;}}}
   L15623: {
    long *vec_833X;
    Sopen_portsS = ((FILE **)malloc(sizeof(FILE **) * 100));
    Sopen_vm_portsS = ((long*)malloc(sizeof(long*) * 100));
    vec_833X = Sopen_vm_portsS;
    arg0K0 = 0;
    goto L13438;
   L13438: {
    long i_834X = arg0K0;
    if ((i_834X < 100)) {
      *((vec_833X + i_834X)) = 1;
      arg0K0 = (1 + i_834X);
      goto L13438;}
    else {
      long d_835X;
      long new_836X;
      long d_837X;
      long new_838X;
      long new_839X;
      long new_840X;
      long bottom_841X;
      long new_842X;
      long new_843X;
      *(Sopen_portsS) = Scurrent_input_portS;
      *((Sopen_portsS + 1)) = Scurrent_output_portS;
      merged_arg4K0 = (XX790.contents);
      enter_string_return_tag = 0;
      goto enter_string;
     enter_string_return_0:
      d_835X = enter_string_return_value;
      *((long*)((unsigned char*)ShpS)) = 4118;
      ShpS = (4 + ShpS);
      new_836X = 3 + ShpS;
      ShpS = (16 + ShpS);
      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
      *((long*)((unsigned char*)(-3 + new_836X))) = 8;
      *((long*)((unsigned char*)(1 + new_836X))) = 4;
      *((long*)((unsigned char*)(5 + new_836X))) = 1;
      *((long*)((unsigned char*)(9 + new_836X))) = d_835X;
      merged_arg4K0 = (XX791.contents);
      enter_string_return_tag = 1;
      goto enter_string;
     enter_string_return_1:
      d_837X = enter_string_return_value;
      *((long*)((unsigned char*)ShpS)) = 4118;
      ShpS = (4 + ShpS);
      new_838X = 3 + ShpS;
      ShpS = (16 + ShpS);
      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
      *((long*)((unsigned char*)(-3 + new_838X))) = 4;
      *((long*)((unsigned char*)(1 + new_838X))) = 0;
      *((long*)((unsigned char*)(5 + new_838X))) = 1;
      *((long*)((unsigned char*)(9 + new_838X))) = d_837X;
      *(Sopen_vm_portsS) = new_838X;
      *((Sopen_vm_portsS + 1)) = new_836X;
      *((long*)((unsigned char*)ShpS)) = 834;
      ShpS = (4 + ShpS);
      new_839X = 3 + ShpS;
      ShpS = (4 + ShpS);
      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
      *((long*)((unsigned char*)ShpS)) = 2090;
      ShpS = (4 + ShpS);
      new_840X = 3 + ShpS;
      ShpS = (8 + ShpS);
      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
      *((long*)((unsigned char*)(-3 + new_840X))) = new_839X;
      *((unsigned char*)((unsigned char*)(-3 + new_839X))) = 31;
      *((unsigned char*)((unsigned char*)(-3 + new_839X))) = 126;
      *((unsigned char*)((unsigned char*)(-2 + new_839X))) = 28;
      Sinterrupt_templateS = new_840X;
      SstackS = (-4 + Sstack_endS);
      Sstack_limitS = (296 + Sstack_beginS);
      SstackS = (-20 + SstackS);
      *((long*)((unsigned char*)(4 + SstackS))) = 4130;
      bottom_841X = 11 + SstackS;
      *((long*)((unsigned char*)(1 + bottom_841X))) = 0;
      *((long*)((unsigned char*)(-3 + bottom_841X))) = 1;
      *((long*)((unsigned char*)ShpS)) = 578;
      ShpS = (4 + ShpS);
      new_842X = 3 + ShpS;
      ShpS = (4 + ShpS);
      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
      *((long*)((unsigned char*)ShpS)) = 2090;
      ShpS = (4 + ShpS);
      new_843X = 3 + ShpS;
      ShpS = (8 + ShpS);
      *((long*)((unsigned char*)(-4 + ShpS))) = 0;
      *((long*)((unsigned char*)(-3 + new_843X))) = new_842X;
      *((unsigned char*)((unsigned char*)(-3 + new_842X))) = 22;
      *((unsigned char*)((unsigned char*)(-2 + new_842X))) = 28;
      *((long*)((unsigned char*)(5 + bottom_841X))) = new_843X;
      *((long*)((unsigned char*)(9 + bottom_841X))) = 17;
      Sbottom_of_stackS = bottom_841X;
      if ((3 == (3 & startup_proc_809X))) {
        arg0K0 = (3 + ((-3 + startup_proc_809X) + delta_815X));
        goto L15829;}
      else {
        arg0K0 = startup_proc_809X;
        goto L15829;}}}}}}}}
  else {
    long v_844X;
    v_844X = 0; /* JAR HACK */ Qps_error_2((XX789.contents), new_hp_816X, new_limit_817X);
    arg0K0 = v_844X;
    goto L15829;}}}}}}}
 L15829: {
  long val_845X = arg0K0;
  long tem_846X;
  long x_847X;
  long new_848X;
  long v_849X;
  fclose(port_800X);
  tem_846X = Sinterrupt_templateS;
  StemplateS = tem_846X;
  Scode_pointerS = (-3 + (*((long*)((unsigned char*)(-3 + tem_846X)))));
  SenvS = 13;
  SstackS = (-11 + Sbottom_of_stackS);
  Sstack_limitS = (296 + Sstack_beginS);
  *((long*)((unsigned char*)(-3 + Sbottom_of_stackS))) = 1;
  ScontS = Sbottom_of_stackS;
  SnargsS = 13;
  SvalS = 13;
  Sdynamic_stateS = 25;
  Sexception_handlerS = 25;
  Sinterrupt_handlersS = 25;
  Senabled_interruptsS = 0;
  Spending_interruptsS = 0;
  merged_arg4K0 = startup_string_788X;
  enter_string_return_tag = 2;
  goto enter_string;
 enter_string_return_2:
  x_847X = enter_string_return_value;
  *((long*)((unsigned char*)SstackS)) = x_847X;
  SstackS = (-4 + SstackS);
  *((long*)((unsigned char*)SstackS)) = (*(Sopen_vm_portsS));
  SstackS = (-4 + SstackS);
  *((long*)((unsigned char*)SstackS)) = (*((Sopen_vm_portsS + 1)));
  SstackS = (-4 + SstackS);
  *((long*)((unsigned char*)ShpS)) = 578;
  ShpS = (4 + ShpS);
  new_848X = 3 + ShpS;
  ShpS = (4 + ShpS);
  *((long*)((unsigned char*)(-4 + ShpS))) = 0;
  *((unsigned char*)((unsigned char*)(-3 + new_848X))) = 23;
  *((unsigned char*)((unsigned char*)(-2 + new_848X))) = 3;
  Scode_pointerS = (-3 + new_848X);
  v_849X = restart(val_845X);
  return(v_849X);}}
 enter_string: {
  const char *string_850X = merged_arg4K0;  /* JAR HACK - added const */
  extern long ShpS;
  long arg0K0;
  long z_851X;
  long new_852X;
  /* was: z_851X = *((long*)(((unsigned char*)string_850X) + -4)); */
  z_851X = *((const long*)(string_850X + -4));
  *((long*)((unsigned char*)ShpS)) = (62 + (z_851X << 8));
  ShpS = (4 + ShpS);
  new_852X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + z_851X)));
  if ((0 < z_851X)) {
    *((long*)((unsigned char*)(-4 + ShpS))) = 0;
    arg0K0 = new_852X;
    goto L11629;}
  else {
    arg0K0 = new_852X;
    goto L11629;}
 L11629: {
  long s_853X = arg0K0;
  arg0K0 = 0;
  goto L11634;
 L11634: {
  long i_854X = arg0K0;
  if ((i_854X < z_851X)) {
    *((unsigned char*)((unsigned char*)((-3 + s_853X) + i_854X))) = ((*((string_850X + i_854X))));
    arg0K0 = (1 + i_854X);
    goto L11634;}
  else {
    enter_string_return_value = s_853X;
    goto enter_string_return;}}}}
 enter_string_return:
  switch (enter_string_return_tag) {
  case 0: goto enter_string_return_0;
  case 1: goto enter_string_return_1;
  case 2: goto enter_string_return_2;
  }
 read_number: {
  FILE * port_855X = merged_arg3K0;
  int arg5K1;
  long arg0K0;
  goto L5342;
 L5342: {
  int x_856X;
  int ch_857X;
  PS_GETC(port_855X, x_856X)
  ch_857X = x_856X;
  if (((EOF == ch_857X))) {
    read_number_return_value = 0;
    goto read_number_return;}
  else {
    char cha_858X;
    cha_858X = ((char)ch_857X);
    if ((45 == ((long)cha_858X))) {
      int x_859X;
      int ch_860X;
      PS_GETC(port_855X, x_859X)
      ch_860X = x_859X;
      arg0K0 = 0;
      arg5K1 = ch_860X;
      goto L5422;
     L5422: {
      long r_861X = arg0K0;
      int ch_862X = arg5K1;
      if (((EOF == ch_862X))) {
        read_number_return_value = (0 - r_861X);
        goto read_number_return;}
      else {
        long ch_863X;
        ch_863X = -48 + ((long)(((char)ch_862X)));
        if ((ch_863X < 0)) {
          read_number_return_value = (0 - r_861X);
          goto read_number_return;}
        else {
          if ((9 < ch_863X)) {
            read_number_return_value = (0 - r_861X);
            goto read_number_return;}
          else {
            int x_864X;
            int v_865X;
            PS_GETC(port_855X, x_864X)
            v_865X = x_864X;
            arg0K0 = (ch_863X + (10 * r_861X));
            arg5K1 = v_865X;
            goto L5422;}}}}}
    else {
      if ((43 == ((long)cha_858X))) {
        int x_866X;
        int ch_867X;
        PS_GETC(port_855X, x_866X)
        ch_867X = x_866X;
        arg0K0 = 0;
        arg5K1 = ch_867X;
        goto L5451;
       L5451: {
        long r_868X = arg0K0;
        int ch_869X = arg5K1;
        if (((EOF == ch_869X))) {
          read_number_return_value = r_868X;
          goto read_number_return;}
        else {
          long ch_870X;
          ch_870X = -48 + ((long)(((char)ch_869X)));
          if ((ch_870X < 0)) {
            read_number_return_value = r_868X;
            goto read_number_return;}
          else {
            if ((9 < ch_870X)) {
              read_number_return_value = r_868X;
              goto read_number_return;}
            else {
              int x_871X;
              int v_872X;
              PS_GETC(port_855X, x_871X)
              v_872X = x_871X;
              arg0K0 = (ch_870X + (10 * r_868X));
              arg5K1 = v_872X;
              goto L5451;}}}}}
      else {
        if ((((long)cha_858X) < 48)) {
          goto L5342;}
        else {
          if ((57 < ((long)cha_858X))) {
            goto L5342;}
          else {
            arg0K0 = 0;
            arg5K1 = ch_857X;
            goto L5480;
           L5480: {
            long r_873X = arg0K0;
            int ch_874X = arg5K1;
            if (((EOF == ch_874X))) {
              read_number_return_value = r_873X;
              goto read_number_return;}
            else {
              long ch_875X;
              ch_875X = -48 + ((long)(((char)ch_874X)));
              if ((ch_875X < 0)) {
                read_number_return_value = r_873X;
                goto read_number_return;}
              else {
                if ((9 < ch_875X)) {
                  read_number_return_value = r_873X;
                  goto read_number_return;}
                else {
                  int x_876X;
                  int v_877X;
                  PS_GETC(port_855X, x_876X)
                  v_877X = x_876X;
                  arg0K0 = (ch_875X + (10 * r_873X));
                  arg5K1 = v_877X;
                  goto L5480;}}}}}}}}}}}
 read_number_return:
  switch (read_number_return_tag) {
  case 0: goto read_number_return_0;
  case 1: goto read_number_return_1;
  case 2: goto read_number_return_2;
  case 3: goto read_number_return_3;
  case 4: goto read_number_return_4;
  }}

long restart(long value_878X)
{
  extern char Tinterpret();
  extern void Qps_error_1(const char*, long);
  extern long call_external_value(long, long, long);
  extern long SnargsS;
  extern long SstackS;
  extern long SvalS;
  static const struct{long length; char contents[24];} XX879
     = {23, "unkown VM return option"};

 {SvalS = value_878X;
  goto L27783;
 L27783: {
  char option_880X;
  option_880X = TTrun_machine((long)Tinterpret);
  if ((0 == ((long)option_880X))) {
    return(SvalS);}
  else {
    if ((1 == ((long)option_880X))) {
      long v_881X;
      long v_882X;
      long v_883X;
      v_881X = *((long*)((unsigned char*)(-3 + (*((long*)((unsigned char*)(1 + SvalS)))))));
      v_882X = 4 + SstackS;
      v_883X = call_external_value(v_881X, SnargsS, v_882X);
      SvalS = v_883X;
      SstackS = (SstackS - ((-1 - SnargsS) << 2));
      goto L27783;}
    else {
      Qps_error_1((XX879.contents), ((long)option_880X));
      return(-1);}}}}}
scheme48_init()
{
Scurrent_input_portS = stdin;
Scurrent_output_portS = stdout;
Smemory_beginS = 0;
Smemory_endS = 0;
ShpS = 0;
SlimitS = 0;
#ifdef ROBOT
Sstack_sizeS = 2000;
#else
Sstack_sizeS = 10000;
#endif
Sinitial_heap_sizeS = 100000;
Sgc_countS = 0;
Ssaved_hpS = 0;
Ssaved_limitS = 0;
Sweak_pointer_hpS = -1;
Sweak_pointer_limitS = 0;
Sexception_space_usedPS = 0;
Sminimum_recovered_spaceS = 0;
}
