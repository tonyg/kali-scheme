#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

static long copy_weak_pointer(long, char *, char **);
static long real_copy_object(long, char *, char **);
void s48_write_barrier(long, char *, long);
char * s48_heap_pointer(void);
char * s48_heap_begin(void);
long s48_heap_size(void);
char * s48_oldspace_begin(void);
char * s48_oldspace_end(void);
void s48_register_static_areas(long, char **, long*, long, char **, long*);
long s48_gc_count(void);
char * s48_initialize_heap(long, long, char *);
char * s48_allocate_small(long);
long s48_available(void);
char s48_extantP(long);
char s48_check_heap(long);
long s48_find_all(long);
long s48_trace_value(long);
long s48_find_all_records(long);
void s48_trace_continuation_contentsB(char *, char *, long);
void s48_trace_locationsB(char *, char *);
void s48_trace_stob_contentsB(long);
void s48_collect(void);
void s48_make_availableAgc(long);
char * s48_allocate_tracedAgc(long);
char * s48_allocate_untracedAgc(long);
long s48_allocate_stob(long, long);
static char * Soldspace_hpS;
static char * Soldspace_limitS;
static char * Snewspace_beginS;
static char * Snewspace_endS;
static char * Soldspace_beginS;
static char * Soldspace_endS;
static char * *Spure_areasS;
static char * *Simpure_areasS;
static long *Spure_sizesS;
static long *Simpure_sizesS;
static long Spure_area_countS;
static long Simpure_area_countS;
static long Sfinding_typeS;
static long Sheap_errors_leftS;
static long Sgc_countS;
static char * Sfrom_beginS;
static char * Sfrom_endS;
static char * Sweak_pointer_hpS;
static char * Sweak_pointer_limitS;
static long Hthe_record_type10740;
char * s48_ShpS;
char * s48_SlimitS;

static long copy_weak_pointer(long weak_1X, char * frontier_2X, char * *TT0)
{
  char * arg0K0;
  long new_7X;
  char * frontier_6X;
  char * new_frontier_5X;
  char * old_4X;
  char * x_3X;
 {  x_3X = Sweak_pointer_hpS;
  if ((x_3X == NULL)) {
    goto L1619;}
  else {
    if (((Sweak_pointer_hpS) < (Sweak_pointer_limitS))) {
      arg0K0 = frontier_2X;
      goto L1624;}
    else {
      goto L1619;}}}
 L1619: {
  old_4X = Sweak_pointer_hpS;
  new_frontier_5X = frontier_2X + 1024;
  Sweak_pointer_hpS = frontier_2X;
  Sweak_pointer_limitS = new_frontier_5X;
  *((long *) (Sweak_pointer_hpS)) = 261194;
  *((long *) ((Sweak_pointer_hpS) + 8)) = (((long) old_4X));
  arg0K0 = new_frontier_5X;
  goto L1624;}
 L1624: {
  frontier_6X = arg0K0;
  new_7X = 3 + (((long) ((Sweak_pointer_hpS) + 4)));
  *((long *) ((Sweak_pointer_hpS) + 4)) = (*((long *) (((char *) (-3 + weak_1X)))));
  Sweak_pointer_hpS = ((Sweak_pointer_hpS) + 8);
  *((long *) ((((char *) (-3 + weak_1X))) + -4)) = new_7X;
  *TT0 = frontier_6X;
  return new_7X;}
}
static long real_copy_object(long thing_8X, char * frontier_9X, char * *TT0)
{
  long new_14X;
  char * data_addr_13X;
  char * a_12X;
  long descriptor_11X;
  long h_10X;
 {  h_10X = *((long *) ((((char *) (-3 + thing_8X))) + -4));
  if ((3 == (3 & h_10X))) {
    *TT0 = frontier_9X;
    return h_10X;}
  else {
    if ((1078 == h_10X)) {
      descriptor_11X = *((long *) (((char *) (-3 + thing_8X))));
      if ((3 == (3 & descriptor_11X))) {
        a_12X = ((char *) (-3 + descriptor_11X));
        if ((a_12X < (Sfrom_beginS))) {
          goto L2266;}
        else {
          if ((a_12X < (Sfrom_endS))) {
            return copy_weak_pointer(thing_8X, frontier_9X, TT0);}
          else {
            goto L2266;}}}
      else {
        goto L2266;}}
    else {
      goto L2266;}}}
 L2266: {
  *((long *) frontier_9X) = h_10X;
  data_addr_13X = frontier_9X + 4;
  new_14X = 3 + (((long) data_addr_13X));
  *((long *) ((((char *) (-3 + thing_8X))) + -4)) = new_14X;
  memcpy((void *)data_addr_13X, (void *)(((char *) (-3 + thing_8X))),((long)(((unsigned long)h_10X)>>8)));
  *TT0 = (data_addr_13X + (-4 & (3 + ((long)(((unsigned long)h_10X)>>8)))));
  return new_14X;}
}
void s48_write_barrier(long stob_15X, char * address_16X, long value_17X)
{

 {  return;}
}
char * s48_heap_pointer(void)
{

 {  return (s48_ShpS);}
}
char * s48_heap_begin(void)
{

 {  return (Snewspace_beginS);}
}
long s48_heap_size(void)
{

 {  return ((Snewspace_endS) - (Snewspace_beginS));}
}
char * s48_oldspace_begin(void)
{

 {  return (Soldspace_beginS);}
}
char * s48_oldspace_end(void)
{

 {  return (Soldspace_endS);}
}
void s48_register_static_areas(long pure_count_18X, char * *pure_areas_19X, long *pure_sizes_20X, long impure_count_21X, char * *impure_areas_22X, long *impure_sizes_23X)
{

 {  Spure_area_countS = pure_count_18X;
  Spure_areasS = pure_areas_19X;
  Spure_sizesS = pure_sizes_20X;
  Simpure_area_countS = impure_count_21X;
  Simpure_areasS = impure_areas_22X;
  Simpure_sizesS = impure_sizes_23X;
  return;}
}
long s48_gc_count(void)
{

 {  return (Sgc_countS);}
}
char * s48_initialize_heap(long heap_size_24X, long image_size_25X, char * image_start_26X)
{
  long arg1K0;
  char * temp_36X;
  char * temp_35X;
  char * temp_34X;
  char * temp_33X;
  long semisize_32X;
  long cells_31X;
  char * v_30X;
  char * heap_29X;
  long heap_size_28X;
  long minimum_size_27X;
 {  minimum_size_27X = ((image_size_25X)<<2);
  if ((heap_size_24X < minimum_size_27X)) {
    ps_write_string("heap size ", (stderr));
    ps_write_integer(heap_size_24X, (stderr));
    ps_write_string(" is too small, using ", (stderr));
    ps_write_integer(minimum_size_27X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg1K0 = minimum_size_27X;
    goto L771;}
  else {
    arg1K0 = heap_size_24X;
    goto L771;}}
 L771: {
  heap_size_28X = arg1K0;
  heap_29X = (char *)malloc((((heap_size_28X)<<2)));
  if ((heap_29X == NULL)) {
    ps_error("unable to allocate heap space", 0);
    return v_30X;}
  else {
    cells_31X = heap_size_28X / 2;
    semisize_32X = ((cells_31X)<<2);
    Snewspace_beginS = heap_29X;
    Snewspace_endS = ((Snewspace_beginS) + semisize_32X);
    Soldspace_beginS = (Snewspace_endS);
    Soldspace_endS = ((Soldspace_beginS) + semisize_32X);
    if (((Soldspace_beginS) == image_start_26X)) {
      temp_33X = s48_SlimitS;
      s48_SlimitS = (Soldspace_limitS);
      Soldspace_limitS = temp_33X;
      temp_34X = s48_ShpS;
      s48_ShpS = (Soldspace_hpS);
      Soldspace_hpS = temp_34X;
      temp_35X = Snewspace_beginS;
      Snewspace_beginS = (Soldspace_beginS);
      Soldspace_beginS = temp_35X;
      temp_36X = Snewspace_endS;
      Snewspace_endS = (Soldspace_endS);
      Soldspace_endS = temp_36X;
      goto L807;}
    else {
      goto L807;}}}
 L807: {
  Soldspace_hpS = (Soldspace_beginS);
  Soldspace_limitS = (Soldspace_endS);
  s48_ShpS = ((Snewspace_beginS) + (((image_size_25X)<<2)));
  s48_SlimitS = (Snewspace_endS);
  return (Snewspace_beginS);}
}
char * s48_allocate_small(long len_37X)
{
  char * new_38X;
 {  new_38X = s48_ShpS;
  s48_ShpS = ((s48_ShpS) + (-4 & (3 + len_37X)));
  return new_38X;}
}
long s48_available(void)
{

 {  return (((((s48_SlimitS) - (s48_ShpS)))>>2));}
}
char s48_extantP(long thing_39X)
{
  char * a_40X;
 {  if ((3 == (3 & thing_39X))) {
    if ((3 == (3 & thing_39X))) {
      a_40X = ((char *) (-3 + thing_39X));
      if ((a_40X < (Sfrom_beginS))) {
        return 1;}
      else {
        if ((a_40X < (Sfrom_endS))) {
          return (3 == (3 & (*((long *) ((((char *) (-3 + thing_39X))) + -4)))));}
        else {
          return 1;}}}
    else {
      return 1;}}
  else {
    return 1;}}
}
char s48_check_heap(long error_count_41X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int check_area_return_tag;
  char check_area0_return_value;
  char * start_42X;
  char * end_43X;
  char temp_61X;
  char * addr_60X;
  long x_59X;
  char * addr_58X;
  char * next_57X;
  long d_56X;
  char * addr_55X;
  char v_54X;
  long i_53X;
  long count_52X;
  long *sizes_51X;
  char * *areas_50X;
  char v_49X;
  long i_48X;
  long count_47X;
  long *sizes_46X;
  char * *areas_45X;
  char v_44X;
 {  Sheap_errors_leftS = error_count_41X;
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = (s48_ShpS);
  check_area_return_tag = 0;
  goto check_area;
 check_area_return_0:
  v_44X = check_area0_return_value;
  if (v_44X) {
    if ((0 < (Simpure_area_countS))) {
      areas_45X = Simpure_areasS;
      sizes_46X = Simpure_sizesS;
      count_47X = Simpure_area_countS;
      arg1K0 = 0;
      goto L2749;}
    else {
      goto L2733;}}
  else {
    return 0;}}
 L2749: {
  i_48X = arg1K0;
  if ((i_48X < count_47X)) {
    merged_arg0K0 = (*(areas_45X + i_48X));
    merged_arg0K1 = ((*(areas_45X + i_48X)) + (*(sizes_46X + i_48X)));
    check_area_return_tag = 1;
    goto check_area;
   check_area_return_1:
    v_49X = check_area0_return_value;
    if (v_49X) {
      arg1K0 = (1 + i_48X);
      goto L2749;}
    else {
      return 0;}}
  else {
    goto L2733;}}
 L2733: {
  if ((0 < (Spure_area_countS))) {
    areas_50X = Spure_areasS;
    sizes_51X = Spure_sizesS;
    count_52X = Spure_area_countS;
    arg1K0 = 0;
    goto L2770;}
  else {
    return 1;}}
 L2770: {
  i_53X = arg1K0;
  if ((i_53X < count_52X)) {
    merged_arg0K0 = (*(areas_50X + i_53X));
    merged_arg0K1 = ((*(areas_50X + i_53X)) + (*(sizes_51X + i_53X)));
    check_area_return_tag = 2;
    goto check_area;
   check_area_return_2:
    v_54X = check_area0_return_value;
    if (v_54X) {
      arg1K0 = (1 + i_53X);
      goto L2770;}
    else {
      return 0;}}
  else {
    return 1;}}
 check_area: {
  start_42X = merged_arg0K0;
  end_43X = merged_arg0K1;{
  arg0K0 = start_42X;
  goto L2099;}
 L2099: {
  addr_55X = arg0K0;
  if ((addr_55X < end_43X)) {
    d_56X = *((long *) addr_55X);
    next_57X = addr_55X + (4 + (-4 & (3 + ((long)(((unsigned long)d_56X)>>8)))));
    if ((2 == (3 & d_56X))) {
      if ((end_43X < next_57X)) {
        ps_write_string("Heap-check: header too large.", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
        check_area0_return_value = ((Sheap_errors_leftS) < 1);
        goto check_area_return;}
      else {
        if ((2 == (3 & d_56X))) {
          if (((31 & (((d_56X)>>2))) < 17)) {
            goto L2125;}
          else {
            arg0K0 = next_57X;
            goto L2099;}}
        else {
          goto L2125;}}}
    else {
      ps_write_string("Heap-check: unexpected non-header.", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
      check_area0_return_value = ((Sheap_errors_leftS) < 1);
      goto check_area_return;}}
  else {
    check_area0_return_value = 1;
    goto check_area_return;}}
 L2125: {
  arg0K0 = (addr_55X + 4);
  goto L1730;}
 L1730: {
  addr_58X = arg0K0;
  if ((addr_58X == next_57X)) {
    arg0K0 = next_57X;
    goto L2099;}
  else {
    x_59X = *((long *) addr_58X);
    if ((2 == (3 & x_59X))) {
      ps_write_string("Heap-check: unexpected header.", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
      if (((Sheap_errors_leftS) < 1)) {
        arg0K0 = next_57X;
        goto L2099;}
      else {
        check_area0_return_value = 0;
        goto check_area_return;}}
    else {
      if ((3 == (3 & x_59X))) {
        addr_60X = (((char *) (-3 + x_59X))) + -4;
        temp_61X = addr_60X < (Snewspace_beginS);
        if (temp_61X) {
          goto L1148;}
        else {
          if ((addr_60X < (s48_ShpS))) {
            if ((2 == (3 & (*((long *) addr_60X))))) {
              goto L1755;}
            else {
              ps_write_string("Heap-check: stob has no header.", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
              if (((Sheap_errors_leftS) < 1)) {
                goto L1755;}
              else {
                check_area0_return_value = 0;
                goto check_area_return;}}}
          else {
            goto L1148;}}}
      else {
        goto L1755;}}}}
 L1148: {
  ps_write_string("Heap-check: address out of bounds.", (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
  if (((Sheap_errors_leftS) < 1)) {
    goto L1755;}
  else {
    check_area0_return_value = 0;
    goto check_area_return;}}
 L1755: {
  arg0K0 = (addr_58X + 4);
  goto L1730;}
 check_area_return:
  switch (check_area_return_tag) {
  case 0: goto check_area_return_0;
  case 1: goto check_area_return_1;
  default: goto check_area_return_2;
  }}

}
long s48_find_all(long type_62X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc107163_return_tag;
  char Hproc1071630_return_value;
  char * start_64X;
  char * end_65X;
  char * next_81X;
  long d_80X;
  char * addr_79X;
  long type_78X;
  char v_77X;
  long i_76X;
  long count_75X;
  long *sizes_74X;
  char * *areas_73X;
  char v_72X;
  long i_71X;
  long count_70X;
  long *sizes_69X;
  char * *areas_68X;
  char v_67X;
  char * start_hp_66X;
 {  Sfinding_typeS = type_62X;
  start_hp_66X = s48_ShpS;
  *((long *) (s48_ShpS)) = 0;
  s48_ShpS = ((s48_ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_66X;
  Hproc107163_return_tag = 0;
  goto Hproc107163;
 Hproc107163_return_0:
  v_67X = Hproc1071630_return_value;
  if (v_67X) {
    if ((0 < (Simpure_area_countS))) {
      areas_68X = Simpure_areasS;
      sizes_69X = Simpure_sizesS;
      count_70X = Simpure_area_countS;
      arg1K0 = 0;
      goto L2899;}
    else {
      goto L2839;}}
  else {
    goto L2857;}}
 L2899: {
  i_71X = arg1K0;
  if ((i_71X < count_70X)) {
    merged_arg0K0 = (*(areas_68X + i_71X));
    merged_arg0K1 = ((*(areas_68X + i_71X)) + (*(sizes_69X + i_71X)));
    Hproc107163_return_tag = 1;
    goto Hproc107163;
   Hproc107163_return_1:
    v_72X = Hproc1071630_return_value;
    if (v_72X) {
      arg1K0 = (1 + i_71X);
      goto L2899;}
    else {
      goto L2857;}}
  else {
    goto L2839;}}
 L2839: {
  if ((0 < (Spure_area_countS))) {
    areas_73X = Spure_areasS;
    sizes_74X = Spure_sizesS;
    count_75X = Spure_area_countS;
    arg1K0 = 0;
    goto L2920;}
  else {
    goto L2846;}}
 L2857: {
  s48_ShpS = start_hp_66X;
  return 1;}
 L2920: {
  i_76X = arg1K0;
  if ((i_76X < count_75X)) {
    merged_arg0K0 = (*(areas_73X + i_76X));
    merged_arg0K1 = ((*(areas_73X + i_76X)) + (*(sizes_74X + i_76X)));
    Hproc107163_return_tag = 2;
    goto Hproc107163;
   Hproc107163_return_2:
    v_77X = Hproc1071630_return_value;
    if (v_77X) {
      arg1K0 = (1 + i_76X);
      goto L2920;}
    else {
      goto L2857;}}
  else {
    goto L2846;}}
 L2846: {
  *((long *) start_hp_66X) = (10 + (((((s48_ShpS) - (start_hp_66X + 4)))<<8)));
  return (3 + (((long) (start_hp_66X + 4))));}
 Hproc107163: {
  start_64X = merged_arg0K0;
  end_65X = merged_arg0K1;{
  type_78X = Sfinding_typeS;
  arg0K0 = start_64X;
  goto L2382;}
 L2382: {
  addr_79X = arg0K0;
  if ((addr_79X < end_65X)) {
    d_80X = *((long *) addr_79X);
    next_81X = addr_79X + (4 + (-4 & (3 + ((long)(((unsigned long)d_80X)>>8)))));
    if ((2 == (3 & d_80X))) {
      if ((type_78X == (31 & (((d_80X)>>2))))) {
        if ((((s48_ShpS) + 16) < (s48_SlimitS))) {
          *((long *) (s48_ShpS)) = (3 + (((long) (addr_79X + 4))));
          s48_ShpS = ((s48_ShpS) + 4);
          arg0K0 = next_81X;
          goto L2382;}
        else {
          Hproc1071630_return_value = 0;
          goto Hproc107163_return;}}
      else {
        arg0K0 = next_81X;
        goto L2382;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc1071630_return_value = 0;
      goto Hproc107163_return;}}
  else {
    Hproc1071630_return_value = 1;
    goto Hproc107163_return;}}
 Hproc107163_return:
  switch (Hproc107163_return_tag) {
  case 0: goto Hproc107163_return_0;
  case 1: goto Hproc107163_return_1;
  default: goto Hproc107163_return_2;
  }}

}
long s48_trace_value(long stob_82X)
{
  char * new_hp_85X;
  long new_thing_84X;
  char * a_83X;
 {  if ((3 == (3 & stob_82X))) {
    a_83X = ((char *) (-3 + stob_82X));
    if ((a_83X < (Sfrom_beginS))) {
      return stob_82X;}
    else {
      if ((a_83X < (Sfrom_endS))) {
        new_thing_84X = real_copy_object(stob_82X, (s48_ShpS), &new_hp_85X);
        s48_ShpS = new_hp_85X;
        return new_thing_84X;}
      else {
        return stob_82X;}}}
  else {
    return stob_82X;}}
}
long s48_find_all_records(long record_type_86X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc107987_return_tag;
  char Hproc1079870_return_value;
  char * start_88X;
  char * end_89X;
  long obj_106X;
  char * next_105X;
  long d_104X;
  char * addr_103X;
  long type_102X;
  char v_101X;
  long i_100X;
  long count_99X;
  long *sizes_98X;
  char * *areas_97X;
  char v_96X;
  long i_95X;
  long count_94X;
  long *sizes_93X;
  char * *areas_92X;
  char v_91X;
  char * start_hp_90X;
 {  Hthe_record_type10740 = record_type_86X;
  Sfinding_typeS = 9;
  start_hp_90X = s48_ShpS;
  *((long *) (s48_ShpS)) = 0;
  s48_ShpS = ((s48_ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_90X;
  Hproc107987_return_tag = 0;
  goto Hproc107987;
 Hproc107987_return_0:
  v_91X = Hproc1079870_return_value;
  if (v_91X) {
    if ((0 < (Simpure_area_countS))) {
      areas_92X = Simpure_areasS;
      sizes_93X = Simpure_sizesS;
      count_94X = Simpure_area_countS;
      arg1K0 = 0;
      goto L3064;}
    else {
      goto L3004;}}
  else {
    goto L3022;}}
 L3064: {
  i_95X = arg1K0;
  if ((i_95X < count_94X)) {
    merged_arg0K0 = (*(areas_92X + i_95X));
    merged_arg0K1 = ((*(areas_92X + i_95X)) + (*(sizes_93X + i_95X)));
    Hproc107987_return_tag = 1;
    goto Hproc107987;
   Hproc107987_return_1:
    v_96X = Hproc1079870_return_value;
    if (v_96X) {
      arg1K0 = (1 + i_95X);
      goto L3064;}
    else {
      goto L3022;}}
  else {
    goto L3004;}}
 L3004: {
  if ((0 < (Spure_area_countS))) {
    areas_97X = Spure_areasS;
    sizes_98X = Spure_sizesS;
    count_99X = Spure_area_countS;
    arg1K0 = 0;
    goto L3085;}
  else {
    goto L3011;}}
 L3022: {
  s48_ShpS = start_hp_90X;
  return 1;}
 L3085: {
  i_100X = arg1K0;
  if ((i_100X < count_99X)) {
    merged_arg0K0 = (*(areas_97X + i_100X));
    merged_arg0K1 = ((*(areas_97X + i_100X)) + (*(sizes_98X + i_100X)));
    Hproc107987_return_tag = 2;
    goto Hproc107987;
   Hproc107987_return_2:
    v_101X = Hproc1079870_return_value;
    if (v_101X) {
      arg1K0 = (1 + i_100X);
      goto L3085;}
    else {
      goto L3022;}}
  else {
    goto L3011;}}
 L3011: {
  *((long *) start_hp_90X) = (10 + (((((s48_ShpS) - (start_hp_90X + 4)))<<8)));
  return (3 + (((long) (start_hp_90X + 4))));}
 Hproc107987: {
  start_88X = merged_arg0K0;
  end_89X = merged_arg0K1;{
  type_102X = Sfinding_typeS;
  arg0K0 = start_88X;
  goto L2549;}
 L2549: {
  addr_103X = arg0K0;
  if ((addr_103X < end_89X)) {
    d_104X = *((long *) addr_103X);
    next_105X = addr_103X + (4 + (-4 & (3 + ((long)(((unsigned long)d_104X)>>8)))));
    if ((2 == (3 & d_104X))) {
      if ((type_102X == (31 & (((d_104X)>>2))))) {
        obj_106X = 3 + (((long) (addr_103X + 4)));
        if (((*((long *) (((char *) (-3 + obj_106X))))) == (Hthe_record_type10740))) {
          if ((((s48_ShpS) + 16) < (s48_SlimitS))) {
            *((long *) (s48_ShpS)) = obj_106X;
            s48_ShpS = ((s48_ShpS) + 4);
            arg0K0 = next_105X;
            goto L2549;}
          else {
            Hproc1079870_return_value = 0;
            goto Hproc107987_return;}}
        else {
          arg0K0 = next_105X;
          goto L2549;}}
      else {
        arg0K0 = next_105X;
        goto L2549;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc1079870_return_value = 0;
      goto Hproc107987_return;}}
  else {
    Hproc1079870_return_value = 1;
    goto Hproc107987_return;}}
 Hproc107987_return:
  switch (Hproc107987_return_tag) {
  case 0: goto Hproc107987_return_0;
  case 1: goto Hproc107987_return_1;
  default: goto Hproc107987_return_2;
  }}

}
void s48_trace_continuation_contentsB(char * contents_pointer_107X, char * code_pointer_108X, long mask_size_109X)
{
  char * arg0K0;
  char * arg0K1;
  long arg1K0;
  long x2_119X;
  char * new_hp_118X;
  long new_thing_117X;
  char * a_116X;
  long stob_115X;
  char * ptr_114X;
  long mask_113X;
  char * trace_ptr_112X;
  char * mask_ptr_111X;
  char * mask_pointer_110X;
 {  mask_pointer_110X = code_pointer_108X + -7;
  arg0K0 = (mask_pointer_110X + (0 - mask_size_109X));
  arg0K1 = contents_pointer_107X;
  goto L3219;}
 L3219: {
  mask_ptr_111X = arg0K0;
  trace_ptr_112X = arg0K1;
  if ((mask_ptr_111X == mask_pointer_110X)) {
    return;}
  else {
    arg1K0 = (*((unsigned char *) mask_ptr_111X));
    arg0K1 = trace_ptr_112X;
    goto L3227;}}
 L3227: {
  mask_113X = arg1K0;
  ptr_114X = arg0K1;
  if ((0 == mask_113X)) {
    arg0K0 = (mask_ptr_111X + 1);
    arg0K1 = (trace_ptr_112X + 32);
    goto L3219;}
  else {
    if ((1 == (1 & mask_113X))) {
      stob_115X = *((long *) ptr_114X);
      if ((3 == (3 & stob_115X))) {
        a_116X = ((char *) (-3 + stob_115X));
        if ((a_116X < (Sfrom_beginS))) {
          arg1K0 = stob_115X;
          goto L3240;}
        else {
          if ((a_116X < (Sfrom_endS))) {
            new_thing_117X = real_copy_object(stob_115X, (s48_ShpS), &new_hp_118X);
            s48_ShpS = new_hp_118X;
            arg1K0 = new_thing_117X;
            goto L3240;}
          else {
            arg1K0 = stob_115X;
            goto L3240;}}}
      else {
        arg1K0 = stob_115X;
        goto L3240;}}
    else {
      goto L3245;}}}
 L3240: {
  x2_119X = arg1K0;
  *((long *) ptr_114X) = x2_119X;
  goto L3245;}
 L3245: {
  arg1K0 = (((mask_113X)>>1));
  arg0K1 = (ptr_114X + 4);
  goto L3227;}
}
void s48_trace_locationsB(char * start_120X, char * end_121X)
{
  char * arg0K0;
  char * arg0K1;
  long arg1K0;
  long new_139X;
  char * data_addr_138X;
  char * frontier_137X;
  long new_thing_136X;
  char * data_pointer_135X;
  long mask_size_134X;
  char * code_pointer_133X;
  long size_132X;
  char * frontier_131X;
  long new_thing_130X;
  char * a_129X;
  long descriptor_128X;
  long h_127X;
  char * a_126X;
  char * next_125X;
  long thing_124X;
  char * frontier_123X;
  char * addr_122X;
 {  arg0K0 = start_120X;
  arg0K1 = (s48_ShpS);
  goto L3400;}
 L3400: {
  addr_122X = arg0K0;
  frontier_123X = arg0K1;
  if ((addr_122X < end_121X)) {
    thing_124X = *((long *) addr_122X);
    next_125X = addr_122X + 4;
    if ((2 == (3 & thing_124X))) {
      if ((2 == (3 & thing_124X))) {
        if (((31 & (((thing_124X)>>2))) < 17)) {
          goto L3417;}
        else {
          arg0K0 = (next_125X + (-4 & (3 + ((long)(((unsigned long)thing_124X)>>8)))));
          arg0K1 = frontier_123X;
          goto L3400;}}
      else {
        goto L3417;}}
    else {
      if ((3 == (3 & thing_124X))) {
        a_126X = ((char *) (-3 + thing_124X));
        if ((a_126X < (Sfrom_beginS))) {
          arg0K0 = next_125X;
          arg0K1 = frontier_123X;
          goto L3400;}
        else {
          if ((a_126X < (Sfrom_endS))) {
            h_127X = *((long *) ((((char *) (-3 + thing_124X))) + -4));
            if ((3 == (3 & h_127X))) {
              arg1K0 = h_127X;
              arg0K1 = frontier_123X;
              goto L3435;}
            else {
              if ((1078 == h_127X)) {
                descriptor_128X = *((long *) (((char *) (-3 + thing_124X))));
                if ((3 == (3 & descriptor_128X))) {
                  a_129X = ((char *) (-3 + descriptor_128X));
                  if ((a_129X < (Sfrom_beginS))) {
                    goto L4018;}
                  else {
                    if ((a_129X < (Sfrom_endS))) {
                      new_thing_130X = copy_weak_pointer(thing_124X, frontier_123X, &frontier_131X);
                      arg1K0 = new_thing_130X;
                      arg0K1 = frontier_131X;
                      goto L3435;}
                    else {
                      goto L4018;}}}
                else {
                  goto L4018;}}
              else {
                goto L4018;}}}
          else {
            arg0K0 = next_125X;
            arg0K1 = frontier_123X;
            goto L3400;}}}
      else {
        arg0K0 = next_125X;
        arg0K1 = frontier_123X;
        goto L3400;}}}
  else {
    s48_ShpS = frontier_123X;
    return;}}
 L3417: {
  if ((10 == (31 & (((thing_124X)>>2))))) {
    size_132X = -4 & (3 + ((long)(((unsigned long)thing_124X)>>8)));
    s48_ShpS = frontier_123X;
    code_pointer_133X = (((char *) (-3 + (*((long *) (next_125X + 4)))))) + ((((*((long *) next_125X)))>>2));
    mask_size_134X = *((unsigned char *) (code_pointer_133X + -3));
    if ((0 == mask_size_134X)) {s48_trace_locationsB(next_125X, (next_125X + size_132X));
      goto L3486;}
    else {
      data_pointer_135X = next_125X + 12;s48_trace_locationsB(next_125X, data_pointer_135X);s48_trace_continuation_contentsB(data_pointer_135X, code_pointer_133X, mask_size_134X);
      goto L3486;}}
  else {
    arg0K0 = next_125X;
    arg0K1 = frontier_123X;
    goto L3400;}}
 L3435: {
  new_thing_136X = arg1K0;
  frontier_137X = arg0K1;
  *((long *) addr_122X) = new_thing_136X;
  arg0K0 = next_125X;
  arg0K1 = frontier_137X;
  goto L3400;}
 L4018: {
  *((long *) frontier_123X) = h_127X;
  data_addr_138X = frontier_123X + 4;
  new_139X = 3 + (((long) data_addr_138X));
  *((long *) ((((char *) (-3 + thing_124X))) + -4)) = new_139X;
  memcpy((void *)data_addr_138X, (void *)(((char *) (-3 + thing_124X))),((long)(((unsigned long)h_127X)>>8)));
  arg1K0 = new_139X;
  arg0K1 = (data_addr_138X + (-4 & (3 + ((long)(((unsigned long)h_127X)>>8)))));
  goto L3435;}
 L3486: {
  arg0K0 = (next_125X + size_132X);
  arg0K1 = (s48_ShpS);
  goto L3400;}
}
void s48_trace_stob_contentsB(long stob_140X)
{
  char * start_142X;
  long h_141X;
 {  h_141X = *((long *) ((((char *) (-3 + stob_140X))) + -4));
  start_142X = ((char *) (-3 + stob_140X));
  s48_trace_locationsB(start_142X, (start_142X + (-4 & (3 + ((long)(((unsigned long)h_141X)>>8))))));
  return;}
}
void s48_collect(void)
{
  char * arg0K1;
  char * arg0K0;
  long arg1K0;
  long x2_158X;
  long h_157X;
  char * a_156X;
  long value_155X;
  char * scan_154X;
  char * next_153X;
  char * end_152X;
  char * start_151X;
  char * end_150X;
  char * x_149X;
  char * end_148X;
  char * start_147X;
  char * temp_146X;
  char * temp_145X;
  char * temp_144X;
  char * temp_143X;
 {  Sfrom_beginS = (Snewspace_beginS);
  Sfrom_endS = (Snewspace_endS);
  temp_143X = s48_SlimitS;
  s48_SlimitS = (Soldspace_limitS);
  Soldspace_limitS = temp_143X;
  temp_144X = s48_ShpS;
  s48_ShpS = (Soldspace_hpS);
  Soldspace_hpS = temp_144X;
  temp_145X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = temp_145X;
  temp_146X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = temp_146X;
  s48_ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = NULL;s48_gc_root();
  arg0K0 = (Snewspace_beginS);
  goto L3698;}
 L3698: {
  start_147X = arg0K0;
  end_148X = s48_ShpS;s48_trace_locationsB(start_147X, end_148X);
  if (((((((s48_SlimitS) - (s48_ShpS)))>>2)) < 0)) {
    ps_error("GC error: ran out of space in new heap", 0);
    goto L3658;}
  else {
    if ((end_148X < (s48_ShpS))) {
      arg0K0 = end_148X;
      goto L3698;}
    else {
      goto L3658;}}}
 L3658: {
  if (((Sweak_pointer_hpS) == NULL)) {
    goto L3660;}
  else {
    x_149X = Sweak_pointer_limitS;
    end_150X = Sweak_pointer_hpS;
    arg0K0 = (x_149X + -1024);
    arg0K1 = end_150X;
    goto L2008;}}
 L3660: {
s48_post_gc_cleanup();
  Sgc_countS = (1 + (Sgc_countS));
  return;}
 L2008: {
  start_151X = arg0K0;
  end_152X = arg0K1;
  next_153X = ((char *) (*((long *) (start_151X + 8))));
  arg0K0 = start_151X;
  goto L1437;}
 L1437: {
  scan_154X = arg0K0;
  if ((scan_154X < end_152X)) {
    *((long *) scan_154X) = 1078;
    value_155X = *((long *) (scan_154X + 4));
    if ((3 == (3 & value_155X))) {
      a_156X = ((char *) (-3 + value_155X));
      if ((a_156X < (Sfrom_beginS))) {
        goto L1483;}
      else {
        if ((a_156X < (Sfrom_endS))) {
          if ((3 == (3 & value_155X))) {
            h_157X = *((long *) ((((char *) (-3 + value_155X))) + -4));
            if ((3 == (3 & h_157X))) {
              arg1K0 = h_157X;
              goto L1478;}
            else {
              arg1K0 = 1;
              goto L1478;}}
          else {
            goto L1483;}}
        else {
          goto L1483;}}}
    else {
      goto L1483;}}
  else {
    if ((next_153X == NULL)) {
      if ((end_150X < (Sweak_pointer_limitS))) {
        *((long *) end_150X) = (74 + ((((-4 & ((Sweak_pointer_limitS) - (end_150X + 4))))<<8)));
        goto L3660;}
      else {
        goto L3660;}}
    else {
      arg0K0 = (next_153X + -1024);
      arg0K1 = next_153X;
      goto L2008;}}}
 L1483: {
  arg0K0 = (scan_154X + 8);
  goto L1437;}
 L1478: {
  x2_158X = arg1K0;
  *((long *) (scan_154X + 4)) = x2_158X;
  goto L1483;}
}
void s48_make_availableAgc(long len_159X)
{
  char x_160X;
 {  x_160X = ((s48_ShpS) + (-4 & (3 + len_159X))) < (s48_SlimitS);
  if (x_160X) {
    goto L3729;}
  else {s48_collect();
    goto L3729;}}
 L3729: {
  if ((((s48_ShpS) + (-4 & (3 + len_159X))) < (s48_SlimitS))) {
    return;}
  else {
    ps_error("Scheme 48 heap overflow", 0);
    return;}}
}
char * s48_allocate_tracedAgc(long len_161X)
{
  char * new_163X;
  char x_162X;
 {  x_162X = ((s48_ShpS) + (-4 & (3 + len_161X))) < (s48_SlimitS);
  if (x_162X) {
    goto L3803;}
  else {s48_collect();
    goto L3803;}}
 L3803: {
  if ((((s48_ShpS) + (-4 & (3 + len_161X))) < (s48_SlimitS))) {
    new_163X = s48_ShpS;
    s48_ShpS = ((s48_ShpS) + (-4 & (3 + len_161X)));
    return new_163X;}
  else {
    return NULL;}}
}
char * s48_allocate_untracedAgc(long len_164X)
{
  char * new_166X;
  char x_165X;
 {  x_165X = ((s48_ShpS) + (-4 & (3 + len_164X))) < (s48_SlimitS);
  if (x_165X) {
    goto L3875;}
  else {s48_collect();
    goto L3875;}}
 L3875: {
  if ((((s48_ShpS) + (-4 & (3 + len_164X))) < (s48_SlimitS))) {
    new_166X = s48_ShpS;
    s48_ShpS = ((s48_ShpS) + (-4 & (3 + len_164X)));
    return new_166X;}
  else {
    return NULL;}}
}
long s48_allocate_stob(long type_167X, long size_168X)
{
  long arg1K0;
  char * arg0K0;
  long v_177X;
  char * thing_176X;
  char * new_175X;
  char * new_174X;
  char x_173X;
  char x_172X;
  long needed_171X;
  long length_in_bytes_170X;
  char tracedP_169X;
 {  tracedP_169X = type_167X < 17;
  if (tracedP_169X) {
    arg1K0 = (((size_168X)<<2));
    goto L3901;}
  else {
    arg1K0 = size_168X;
    goto L3901;}}
 L3901: {
  length_in_bytes_170X = arg1K0;
  needed_171X = 4 + length_in_bytes_170X;
  if (tracedP_169X) {
    x_172X = ((s48_ShpS) + (-4 & (3 + needed_171X))) < (s48_SlimitS);
    if (x_172X) {
      goto L3959;}
    else {s48_collect();
      goto L3959;}}
  else {
    x_173X = ((s48_ShpS) + (-4 & (3 + needed_171X))) < (s48_SlimitS);
    if (x_173X) {
      goto L3978;}
    else {s48_collect();
      goto L3978;}}}
 L3959: {
  if ((((s48_ShpS) + (-4 & (3 + needed_171X))) < (s48_SlimitS))) {
    new_174X = s48_ShpS;
    s48_ShpS = ((s48_ShpS) + (-4 & (3 + needed_171X)));
    arg0K0 = new_174X;
    goto L3917;}
  else {
    arg0K0 = NULL;
    goto L3917;}}
 L3978: {
  if ((((s48_ShpS) + (-4 & (3 + needed_171X))) < (s48_SlimitS))) {
    new_175X = s48_ShpS;
    s48_ShpS = ((s48_ShpS) + (-4 & (3 + needed_171X)));
    arg0K0 = new_175X;
    goto L3917;}
  else {
    arg0K0 = NULL;
    goto L3917;}}
 L3917: {
  thing_176X = arg0K0;
  if ((thing_176X == NULL)) {
    ps_error("insufficient heap space for external allocation", 0);
    return v_177X;}
  else {
    *((long *) thing_176X) = (2 + (((((((length_in_bytes_170X)<<6)) + type_167X))<<2)));
    return (3 + (((long) (thing_176X + 4))));}}
}void
s48_heap_init(void)
{
Spure_area_countS = 0;
Simpure_area_countS = 0;
Sfinding_typeS = 1;
Sheap_errors_leftS = 0;
Sgc_countS = 0;
Hthe_record_type10740 = 1;
}
