#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

static long copy_weak_pointer(long, char *, char **);
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
long s48_trace_locationsB(char *, char *);
long s48_find_all(long);
long s48_trace_stob_contentsB(long);
long s48_trace_value(long);
long s48_find_all_records(long);
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
static long Hthe_record_type10620;
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
    goto L1255;}
  else {
    if (((Sweak_pointer_hpS) < (Sweak_pointer_limitS))) {
      arg0K0 = frontier_2X;
      goto L1260;}
    else {
      goto L1255;}}}
 L1255: {
  old_4X = Sweak_pointer_hpS;
  new_frontier_5X = frontier_2X + 1024;
  Sweak_pointer_hpS = frontier_2X;
  Sweak_pointer_limitS = new_frontier_5X;
  *((long *) (Sweak_pointer_hpS)) = 261194;
  *((long *) ((Sweak_pointer_hpS) + 8)) = (((long) old_4X));
  arg0K0 = new_frontier_5X;
  goto L1260;}
 L1260: {
  frontier_6X = arg0K0;
  new_7X = 3 + (((long) ((Sweak_pointer_hpS) + 4)));
  *((long *) ((Sweak_pointer_hpS) + 4)) = (*((long *) (((char *) (-3 + weak_1X)))));
  Sweak_pointer_hpS = ((Sweak_pointer_hpS) + 8);
  *((long *) ((((char *) (-3 + weak_1X))) + -4)) = new_7X;
  *TT0 = frontier_6X;
  return new_7X;}
}
void s48_write_barrier(long stob_8X, char * address_9X, long value_10X)
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
void s48_register_static_areas(long pure_count_11X, char * *pure_areas_12X, long *pure_sizes_13X, long impure_count_14X, char * *impure_areas_15X, long *impure_sizes_16X)
{

 {  Spure_area_countS = pure_count_11X;
  Spure_areasS = pure_areas_12X;
  Spure_sizesS = pure_sizes_13X;
  Simpure_area_countS = impure_count_14X;
  Simpure_areasS = impure_areas_15X;
  Simpure_sizesS = impure_sizes_16X;
  return;}
}
long s48_gc_count(void)
{

 {  return (Sgc_countS);}
}
char * s48_initialize_heap(long heap_size_17X, long image_size_18X, char * image_start_19X)
{
  long arg1K0;
  char * temp_29X;
  char * temp_28X;
  char * temp_27X;
  char * temp_26X;
  long semisize_25X;
  long cells_24X;
  char * v_23X;
  char * heap_22X;
  long heap_size_21X;
  long minimum_size_20X;
 {  minimum_size_20X = ((image_size_18X)<<2);
  if ((heap_size_17X < minimum_size_20X)) {
    ps_write_string("heap size ", (stderr));
    ps_write_integer(heap_size_17X, (stderr));
    ps_write_string(" is too small, using ", (stderr));
    ps_write_integer(minimum_size_20X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg1K0 = minimum_size_20X;
    goto L710;}
  else {
    arg1K0 = heap_size_17X;
    goto L710;}}
 L710: {
  heap_size_21X = arg1K0;
  heap_22X = (char *)malloc((((heap_size_21X)<<2)));
  if ((heap_22X == NULL)) {
    ps_error("unable to allocate heap space", 0);
    return v_23X;}
  else {
    cells_24X = heap_size_21X / 2;
    semisize_25X = ((cells_24X)<<2);
    Snewspace_beginS = heap_22X;
    Snewspace_endS = ((Snewspace_beginS) + semisize_25X);
    Soldspace_beginS = (Snewspace_endS);
    Soldspace_endS = ((Soldspace_beginS) + semisize_25X);
    if (((Soldspace_beginS) == image_start_19X)) {
      temp_26X = s48_SlimitS;
      s48_SlimitS = (Soldspace_limitS);
      Soldspace_limitS = temp_26X;
      temp_27X = s48_ShpS;
      s48_ShpS = (Soldspace_hpS);
      Soldspace_hpS = temp_27X;
      temp_28X = Snewspace_beginS;
      Snewspace_beginS = (Soldspace_beginS);
      Soldspace_beginS = temp_28X;
      temp_29X = Snewspace_endS;
      Snewspace_endS = (Soldspace_endS);
      Soldspace_endS = temp_29X;
      goto L746;}
    else {
      goto L746;}}}
 L746: {
  Soldspace_hpS = (Soldspace_beginS);
  Soldspace_limitS = (Soldspace_endS);
  s48_ShpS = ((Snewspace_beginS) + (((image_size_18X)<<2)));
  s48_SlimitS = (Snewspace_endS);
  return (Snewspace_beginS);}
}
char * s48_allocate_small(long len_30X)
{
  char * new_31X;
 {  new_31X = s48_ShpS;
  s48_ShpS = ((s48_ShpS) + (-4 & (3 + len_30X)));
  return new_31X;}
}
long s48_available(void)
{

 {  return (((((s48_SlimitS) - (s48_ShpS)))>>2));}
}
char s48_extantP(long thing_32X)
{
  char * a_33X;
 {  if ((3 == (3 & thing_32X))) {
    if ((3 == (3 & thing_32X))) {
      a_33X = ((char *) (-3 + thing_32X));
      if ((a_33X < (Sfrom_beginS))) {
        return 1;}
      else {
        if ((a_33X < (Sfrom_endS))) {
          return (3 == (3 & (*((long *) ((((char *) (-3 + thing_32X))) + -4)))));}
        else {
          return 1;}}}
    else {
      return 1;}}
  else {
    return 1;}}
}
char s48_check_heap(long error_count_34X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int check_area_return_tag;
  char check_area0_return_value;
  char * start_35X;
  char * end_36X;
  char temp_54X;
  char * addr_53X;
  long x_52X;
  char * addr_51X;
  char * next_50X;
  long d_49X;
  char * addr_48X;
  char v_47X;
  long i_46X;
  long count_45X;
  long *sizes_44X;
  char * *areas_43X;
  char v_42X;
  long i_41X;
  long count_40X;
  long *sizes_39X;
  char * *areas_38X;
  char v_37X;
 {  Sheap_errors_leftS = error_count_34X;
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = (s48_ShpS);
  check_area_return_tag = 0;
  goto check_area;
 check_area_return_0:
  v_37X = check_area0_return_value;
  if (v_37X) {
    if ((0 < (Simpure_area_countS))) {
      areas_38X = Simpure_areasS;
      sizes_39X = Simpure_sizesS;
      count_40X = Simpure_area_countS;
      arg1K0 = 0;
      goto L2648;}
    else {
      goto L2632;}}
  else {
    return 0;}}
 L2648: {
  i_41X = arg1K0;
  if ((i_41X < count_40X)) {
    merged_arg0K0 = (*(areas_38X + i_41X));
    merged_arg0K1 = ((*(areas_38X + i_41X)) + (*(sizes_39X + i_41X)));
    check_area_return_tag = 1;
    goto check_area;
   check_area_return_1:
    v_42X = check_area0_return_value;
    if (v_42X) {
      arg1K0 = (1 + i_41X);
      goto L2648;}
    else {
      return 0;}}
  else {
    goto L2632;}}
 L2632: {
  if ((0 < (Spure_area_countS))) {
    areas_43X = Spure_areasS;
    sizes_44X = Spure_sizesS;
    count_45X = Spure_area_countS;
    arg1K0 = 0;
    goto L2669;}
  else {
    return 1;}}
 L2669: {
  i_46X = arg1K0;
  if ((i_46X < count_45X)) {
    merged_arg0K0 = (*(areas_43X + i_46X));
    merged_arg0K1 = ((*(areas_43X + i_46X)) + (*(sizes_44X + i_46X)));
    check_area_return_tag = 2;
    goto check_area;
   check_area_return_2:
    v_47X = check_area0_return_value;
    if (v_47X) {
      arg1K0 = (1 + i_46X);
      goto L2669;}
    else {
      return 0;}}
  else {
    return 1;}}
 check_area: {
  start_35X = merged_arg0K0;
  end_36X = merged_arg0K1;{
  arg0K0 = start_35X;
  goto L1998;}
 L1998: {
  addr_48X = arg0K0;
  if ((addr_48X < end_36X)) {
    d_49X = *((long *) addr_48X);
    next_50X = addr_48X + (4 + (-4 & (3 + ((long)(((unsigned long)d_49X)>>8)))));
    if ((2 == (3 & d_49X))) {
      if ((end_36X < next_50X)) {
        ps_write_string("Heap-check: header too large.", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
        check_area0_return_value = ((Sheap_errors_leftS) < 1);
        goto check_area_return;}
      else {
        if ((2 == (3 & d_49X))) {
          if (((31 & (((d_49X)>>2))) < 17)) {
            goto L2024;}
          else {
            arg0K0 = next_50X;
            goto L1998;}}
        else {
          goto L2024;}}}
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
 L2024: {
  arg0K0 = (addr_48X + 4);
  goto L1366;}
 L1366: {
  addr_51X = arg0K0;
  if ((addr_51X == next_50X)) {
    arg0K0 = next_50X;
    goto L1998;}
  else {
    x_52X = *((long *) addr_51X);
    if ((2 == (3 & x_52X))) {
      ps_write_string("Heap-check: unexpected header.", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
      if (((Sheap_errors_leftS) < 1)) {
        arg0K0 = next_50X;
        goto L1998;}
      else {
        check_area0_return_value = 0;
        goto check_area_return;}}
    else {
      if ((3 == (3 & x_52X))) {
        addr_53X = (((char *) (-3 + x_52X))) + -4;
        temp_54X = addr_53X < (Snewspace_beginS);
        if (temp_54X) {
          goto L1020;}
        else {
          if ((addr_53X < (s48_ShpS))) {
            if ((2 == (3 & (*((long *) addr_53X))))) {
              goto L1391;}
            else {
              ps_write_string("Heap-check: stob has no header.", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
              if (((Sheap_errors_leftS) < 1)) {
                goto L1391;}
              else {
                check_area0_return_value = 0;
                goto check_area_return;}}}
          else {
            goto L1020;}}}
      else {
        goto L1391;}}}}
 L1020: {
  ps_write_string("Heap-check: address out of bounds.", (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  Sheap_errors_leftS = (-1 + (Sheap_errors_leftS));
  if (((Sheap_errors_leftS) < 1)) {
    goto L1391;}
  else {
    check_area0_return_value = 0;
    goto check_area_return;}}
 L1391: {
  arg0K0 = (addr_51X + 4);
  goto L1366;}
 check_area_return:
  switch (check_area_return_tag) {
  case 0: goto check_area_return_0;
  case 1: goto check_area_return_1;
  default: goto check_area_return_2;
  }}

}
long s48_trace_locationsB(char * start_55X, char * end_56X)
{
  char * arg0K0;
  char * arg0K1;
  long arg1K0;
  long new_70X;
  char * data_addr_69X;
  char * frontier_68X;
  long new_thing_67X;
  char * frontier_66X;
  long new_thing_65X;
  char * a_64X;
  long descriptor_63X;
  long h_62X;
  char * a_61X;
  char * next_60X;
  long thing_59X;
  char * frontier_58X;
  char * addr_57X;
 {  arg0K0 = start_55X;
  arg0K1 = (s48_ShpS);
  goto L2707;}
 L2707: {
  addr_57X = arg0K0;
  frontier_58X = arg0K1;
  if ((addr_57X < end_56X)) {
    thing_59X = *((long *) addr_57X);
    next_60X = addr_57X + 4;
    if ((2 == (3 & thing_59X))) {
      if ((2 == (3 & thing_59X))) {
        if (((31 & (((thing_59X)>>2))) < 17)) {
          arg0K0 = next_60X;
          arg0K1 = frontier_58X;
          goto L2707;}
        else {
          arg0K0 = (next_60X + (-4 & (3 + ((long)(((unsigned long)thing_59X)>>8)))));
          arg0K1 = frontier_58X;
          goto L2707;}}
      else {
        arg0K0 = next_60X;
        arg0K1 = frontier_58X;
        goto L2707;}}
    else {
      if ((3 == (3 & thing_59X))) {
        a_61X = ((char *) (-3 + thing_59X));
        if ((a_61X < (Sfrom_beginS))) {
          arg0K0 = next_60X;
          arg0K1 = frontier_58X;
          goto L2707;}
        else {
          if ((a_61X < (Sfrom_endS))) {
            h_62X = *((long *) ((((char *) (-3 + thing_59X))) + -4));
            if ((3 == (3 & h_62X))) {
              arg1K0 = h_62X;
              arg0K1 = frontier_58X;
              goto L2732;}
            else {
              if ((1078 == h_62X)) {
                descriptor_63X = *((long *) (((char *) (-3 + thing_59X))));
                if ((3 == (3 & descriptor_63X))) {
                  a_64X = ((char *) (-3 + descriptor_63X));
                  if ((a_64X < (Sfrom_beginS))) {
                    goto L3769;}
                  else {
                    if ((a_64X < (Sfrom_endS))) {
                      new_thing_65X = copy_weak_pointer(thing_59X, frontier_58X, &frontier_66X);
                      arg1K0 = new_thing_65X;
                      arg0K1 = frontier_66X;
                      goto L2732;}
                    else {
                      goto L3769;}}}
                else {
                  goto L3769;}}
              else {
                goto L3769;}}}
          else {
            arg0K0 = next_60X;
            arg0K1 = frontier_58X;
            goto L2707;}}}
      else {
        arg0K0 = next_60X;
        arg0K1 = frontier_58X;
        goto L2707;}}}
  else {
    s48_ShpS = frontier_58X;
    return 0;}}
 L2732: {
  new_thing_67X = arg1K0;
  frontier_68X = arg0K1;
  *((long *) addr_57X) = new_thing_67X;
  arg0K0 = next_60X;
  arg0K1 = frontier_68X;
  goto L2707;}
 L3769: {
  *((long *) frontier_58X) = h_62X;
  data_addr_69X = frontier_58X + 4;
  new_70X = 3 + (((long) data_addr_69X));
  *((long *) ((((char *) (-3 + thing_59X))) + -4)) = new_70X;
  memcpy((void *)data_addr_69X, (void *)(((char *) (-3 + thing_59X))),((long)(((unsigned long)h_62X)>>8)));
  arg1K0 = new_70X;
  arg0K1 = (data_addr_69X + (-4 & (3 + ((long)(((unsigned long)h_62X)>>8)))));
  goto L2732;}
}
long s48_find_all(long type_71X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc105972_return_tag;
  char Hproc1059720_return_value;
  char * start_73X;
  char * end_74X;
  char * next_90X;
  long d_89X;
  char * addr_88X;
  long type_87X;
  char v_86X;
  long i_85X;
  long count_84X;
  long *sizes_83X;
  char * *areas_82X;
  char v_81X;
  long i_80X;
  long count_79X;
  long *sizes_78X;
  char * *areas_77X;
  char v_76X;
  char * start_hp_75X;
 {  Sfinding_typeS = type_71X;
  start_hp_75X = s48_ShpS;
  *((long *) (s48_ShpS)) = 0;
  s48_ShpS = ((s48_ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_75X;
  Hproc105972_return_tag = 0;
  goto Hproc105972;
 Hproc105972_return_0:
  v_76X = Hproc1059720_return_value;
  if (v_76X) {
    if ((0 < (Simpure_area_countS))) {
      areas_77X = Simpure_areasS;
      sizes_78X = Simpure_sizesS;
      count_79X = Simpure_area_countS;
      arg1K0 = 0;
      goto L2956;}
    else {
      goto L2896;}}
  else {
    goto L2914;}}
 L2956: {
  i_80X = arg1K0;
  if ((i_80X < count_79X)) {
    merged_arg0K0 = (*(areas_77X + i_80X));
    merged_arg0K1 = ((*(areas_77X + i_80X)) + (*(sizes_78X + i_80X)));
    Hproc105972_return_tag = 1;
    goto Hproc105972;
   Hproc105972_return_1:
    v_81X = Hproc1059720_return_value;
    if (v_81X) {
      arg1K0 = (1 + i_80X);
      goto L2956;}
    else {
      goto L2914;}}
  else {
    goto L2896;}}
 L2896: {
  if ((0 < (Spure_area_countS))) {
    areas_82X = Spure_areasS;
    sizes_83X = Spure_sizesS;
    count_84X = Spure_area_countS;
    arg1K0 = 0;
    goto L2977;}
  else {
    goto L2903;}}
 L2914: {
  s48_ShpS = start_hp_75X;
  return 1;}
 L2977: {
  i_85X = arg1K0;
  if ((i_85X < count_84X)) {
    merged_arg0K0 = (*(areas_82X + i_85X));
    merged_arg0K1 = ((*(areas_82X + i_85X)) + (*(sizes_83X + i_85X)));
    Hproc105972_return_tag = 2;
    goto Hproc105972;
   Hproc105972_return_2:
    v_86X = Hproc1059720_return_value;
    if (v_86X) {
      arg1K0 = (1 + i_85X);
      goto L2977;}
    else {
      goto L2914;}}
  else {
    goto L2903;}}
 L2903: {
  *((long *) start_hp_75X) = (10 + (((((s48_ShpS) - (start_hp_75X + 4)))<<8)));
  return (3 + (((long) (start_hp_75X + 4))));}
 Hproc105972: {
  start_73X = merged_arg0K0;
  end_74X = merged_arg0K1;{
  type_87X = Sfinding_typeS;
  arg0K0 = start_73X;
  goto L2281;}
 L2281: {
  addr_88X = arg0K0;
  if ((addr_88X < end_74X)) {
    d_89X = *((long *) addr_88X);
    next_90X = addr_88X + (4 + (-4 & (3 + ((long)(((unsigned long)d_89X)>>8)))));
    if ((2 == (3 & d_89X))) {
      if ((type_87X == (31 & (((d_89X)>>2))))) {
        if ((((s48_ShpS) + 16) < (s48_SlimitS))) {
          *((long *) (s48_ShpS)) = (3 + (((long) (addr_88X + 4))));
          s48_ShpS = ((s48_ShpS) + 4);
          arg0K0 = next_90X;
          goto L2281;}
        else {
          Hproc1059720_return_value = 0;
          goto Hproc105972_return;}}
      else {
        arg0K0 = next_90X;
        goto L2281;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc1059720_return_value = 0;
      goto Hproc105972_return;}}
  else {
    Hproc1059720_return_value = 1;
    goto Hproc105972_return;}}
 Hproc105972_return:
  switch (Hproc105972_return_tag) {
  case 0: goto Hproc105972_return_0;
  case 1: goto Hproc105972_return_1;
  default: goto Hproc105972_return_2;
  }}

}
long s48_trace_stob_contentsB(long stob_91X)
{
  char * start_93X;
  long h_92X;
 {  h_92X = *((long *) ((((char *) (-3 + stob_91X))) + -4));
  start_93X = ((char *) (-3 + stob_91X));
  return s48_trace_locationsB(start_93X, (start_93X + (-4 & (3 + ((long)(((unsigned long)h_92X)>>8))))));}
}
long s48_trace_value(long stob_94X)
{
  char * arg0K1;
  long arg1K0;
  long new_105X;
  char * data_addr_104X;
  char * new_hp_103X;
  long new_thing_102X;
  char * new_hp_101X;
  long new_thing_100X;
  char * a_99X;
  long descriptor_98X;
  long h_97X;
  char * frontier_96X;
  char * a_95X;
 {  if ((3 == (3 & stob_94X))) {
    a_95X = ((char *) (-3 + stob_94X));
    if ((a_95X < (Sfrom_beginS))) {
      return stob_94X;}
    else {
      if ((a_95X < (Sfrom_endS))) {
        frontier_96X = s48_ShpS;
        h_97X = *((long *) ((((char *) (-3 + stob_94X))) + -4));
        if ((3 == (3 & h_97X))) {
          arg1K0 = h_97X;
          arg0K1 = frontier_96X;
          goto L3326;}
        else {
          if ((1078 == h_97X)) {
            descriptor_98X = *((long *) (((char *) (-3 + stob_94X))));
            if ((3 == (3 & descriptor_98X))) {
              a_99X = ((char *) (-3 + descriptor_98X));
              if ((a_99X < (Sfrom_beginS))) {
                goto L2165;}
              else {
                if ((a_99X < (Sfrom_endS))) {
                  new_thing_100X = copy_weak_pointer(stob_94X, frontier_96X, &new_hp_101X);
                  arg1K0 = new_thing_100X;
                  arg0K1 = new_hp_101X;
                  goto L3326;}
                else {
                  goto L2165;}}}
            else {
              goto L2165;}}
          else {
            goto L2165;}}}
      else {
        return stob_94X;}}}
  else {
    return stob_94X;}}
 L3326: {
  new_thing_102X = arg1K0;
  new_hp_103X = arg0K1;
  s48_ShpS = new_hp_103X;
  return new_thing_102X;}
 L2165: {
  *((long *) frontier_96X) = h_97X;
  data_addr_104X = frontier_96X + 4;
  new_105X = 3 + (((long) data_addr_104X));
  *((long *) ((((char *) (-3 + stob_94X))) + -4)) = new_105X;
  memcpy((void *)data_addr_104X, (void *)(((char *) (-3 + stob_94X))),((long)(((unsigned long)h_97X)>>8)));
  arg1K0 = new_105X;
  arg0K1 = (data_addr_104X + (-4 & (3 + ((long)(((unsigned long)h_97X)>>8)))));
  goto L3326;}
}
long s48_find_all_records(long record_type_106X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc1064107_return_tag;
  char Hproc10641070_return_value;
  char * start_108X;
  char * end_109X;
  long obj_126X;
  char * next_125X;
  long d_124X;
  char * addr_123X;
  long type_122X;
  char v_121X;
  long i_120X;
  long count_119X;
  long *sizes_118X;
  char * *areas_117X;
  char v_116X;
  long i_115X;
  long count_114X;
  long *sizes_113X;
  char * *areas_112X;
  char v_111X;
  char * start_hp_110X;
 {  Hthe_record_type10620 = record_type_106X;
  Sfinding_typeS = 9;
  start_hp_110X = s48_ShpS;
  *((long *) (s48_ShpS)) = 0;
  s48_ShpS = ((s48_ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_110X;
  Hproc1064107_return_tag = 0;
  goto Hproc1064107;
 Hproc1064107_return_0:
  v_111X = Hproc10641070_return_value;
  if (v_111X) {
    if ((0 < (Simpure_area_countS))) {
      areas_112X = Simpure_areasS;
      sizes_113X = Simpure_sizesS;
      count_114X = Simpure_area_countS;
      arg1K0 = 0;
      goto L3121;}
    else {
      goto L3061;}}
  else {
    goto L3079;}}
 L3121: {
  i_115X = arg1K0;
  if ((i_115X < count_114X)) {
    merged_arg0K0 = (*(areas_112X + i_115X));
    merged_arg0K1 = ((*(areas_112X + i_115X)) + (*(sizes_113X + i_115X)));
    Hproc1064107_return_tag = 1;
    goto Hproc1064107;
   Hproc1064107_return_1:
    v_116X = Hproc10641070_return_value;
    if (v_116X) {
      arg1K0 = (1 + i_115X);
      goto L3121;}
    else {
      goto L3079;}}
  else {
    goto L3061;}}
 L3061: {
  if ((0 < (Spure_area_countS))) {
    areas_117X = Spure_areasS;
    sizes_118X = Spure_sizesS;
    count_119X = Spure_area_countS;
    arg1K0 = 0;
    goto L3142;}
  else {
    goto L3068;}}
 L3079: {
  s48_ShpS = start_hp_110X;
  return 1;}
 L3142: {
  i_120X = arg1K0;
  if ((i_120X < count_119X)) {
    merged_arg0K0 = (*(areas_117X + i_120X));
    merged_arg0K1 = ((*(areas_117X + i_120X)) + (*(sizes_118X + i_120X)));
    Hproc1064107_return_tag = 2;
    goto Hproc1064107;
   Hproc1064107_return_2:
    v_121X = Hproc10641070_return_value;
    if (v_121X) {
      arg1K0 = (1 + i_120X);
      goto L3142;}
    else {
      goto L3079;}}
  else {
    goto L3068;}}
 L3068: {
  *((long *) start_hp_110X) = (10 + (((((s48_ShpS) - (start_hp_110X + 4)))<<8)));
  return (3 + (((long) (start_hp_110X + 4))));}
 Hproc1064107: {
  start_108X = merged_arg0K0;
  end_109X = merged_arg0K1;{
  type_122X = Sfinding_typeS;
  arg0K0 = start_108X;
  goto L2448;}
 L2448: {
  addr_123X = arg0K0;
  if ((addr_123X < end_109X)) {
    d_124X = *((long *) addr_123X);
    next_125X = addr_123X + (4 + (-4 & (3 + ((long)(((unsigned long)d_124X)>>8)))));
    if ((2 == (3 & d_124X))) {
      if ((type_122X == (31 & (((d_124X)>>2))))) {
        obj_126X = 3 + (((long) (addr_123X + 4)));
        if (((*((long *) (((char *) (-3 + obj_126X))))) == (Hthe_record_type10620))) {
          if ((((s48_ShpS) + 16) < (s48_SlimitS))) {
            *((long *) (s48_ShpS)) = obj_126X;
            s48_ShpS = ((s48_ShpS) + 4);
            arg0K0 = next_125X;
            goto L2448;}
          else {
            Hproc10641070_return_value = 0;
            goto Hproc1064107_return;}}
        else {
          arg0K0 = next_125X;
          goto L2448;}}
      else {
        arg0K0 = next_125X;
        goto L2448;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc10641070_return_value = 0;
      goto Hproc1064107_return;}}
  else {
    Hproc10641070_return_value = 1;
    goto Hproc1064107_return;}}
 Hproc1064107_return:
  switch (Hproc1064107_return_tag) {
  case 0: goto Hproc1064107_return_0;
  case 1: goto Hproc1064107_return_1;
  default: goto Hproc1064107_return_2;
  }}

}
void s48_collect(void)
{
  char * arg0K1;
  char * arg0K0;
  long arg1K0;
  long x2_146X;
  long h_145X;
  char * a_144X;
  long value_143X;
  char * scan_142X;
  char * next_141X;
  char * end_140X;
  char * start_139X;
  char * end_138X;
  char * x_137X;
  char * end_136X;
  char * start_135X;
  long i_134X;
  long count_133X;
  long *sizes_132X;
  char * *areas_131X;
  char * temp_130X;
  char * temp_129X;
  char * temp_128X;
  char * temp_127X;
 {  Sfrom_beginS = (Snewspace_beginS);
  Sfrom_endS = (Snewspace_endS);
  temp_127X = s48_SlimitS;
  s48_SlimitS = (Soldspace_limitS);
  Soldspace_limitS = temp_127X;
  temp_128X = s48_ShpS;
  s48_ShpS = (Soldspace_hpS);
  Soldspace_hpS = temp_128X;
  temp_129X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = temp_129X;
  temp_130X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = temp_130X;
  s48_ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = NULL;
  if ((0 < (Simpure_area_countS))) {
    areas_131X = Simpure_areasS;
    sizes_132X = Simpure_sizesS;
    count_133X = Simpure_area_countS;
    arg1K0 = 0;
    goto L3433;}
  else {
    goto L3383;}}
 L3433: {
  i_134X = arg1K0;
  if ((i_134X < count_133X)) {
    s48_trace_locationsB((*(areas_131X + i_134X)), ((*(areas_131X + i_134X)) + (*(sizes_132X + i_134X))));
    arg1K0 = (1 + i_134X);
    goto L3433;}
  else {
    goto L3383;}}
 L3383: {
  s48_gc_root();
  arg0K0 = (Snewspace_beginS);
  goto L3445;}
 L3445: {
  start_135X = arg0K0;
  end_136X = s48_ShpS;
  s48_trace_locationsB(start_135X, end_136X);
  if (((((((s48_SlimitS) - (s48_ShpS)))>>2)) < 0)) {
    ps_error("GC error: ran out of space in new heap", 0);
    goto L3387;}
  else {
    if ((end_136X < (s48_ShpS))) {
      arg0K0 = end_136X;
      goto L3445;}
    else {
      goto L3387;}}}
 L3387: {
  if (((Sweak_pointer_hpS) == NULL)) {
    goto L3389;}
  else {
    x_137X = Sweak_pointer_limitS;
    end_138X = Sweak_pointer_hpS;
    arg0K0 = (x_137X + -1024);
    arg0K1 = end_138X;
    goto L1907;}}
 L3389: {
  s48_post_gc_cleanup();
  Sgc_countS = (1 + (Sgc_countS));
  return;}
 L1907: {
  start_139X = arg0K0;
  end_140X = arg0K1;
  next_141X = ((char *) (*((long *) (start_139X + 8))));
  arg0K0 = start_139X;
  goto L1567;}
 L1567: {
  scan_142X = arg0K0;
  if ((scan_142X < end_140X)) {
    *((long *) scan_142X) = 1078;
    value_143X = *((long *) (scan_142X + 4));
    if ((3 == (3 & value_143X))) {
      a_144X = ((char *) (-3 + value_143X));
      if ((a_144X < (Sfrom_beginS))) {
        goto L1613;}
      else {
        if ((a_144X < (Sfrom_endS))) {
          if ((3 == (3 & value_143X))) {
            h_145X = *((long *) ((((char *) (-3 + value_143X))) + -4));
            if ((3 == (3 & h_145X))) {
              arg1K0 = h_145X;
              goto L1608;}
            else {
              arg1K0 = 1;
              goto L1608;}}
          else {
            goto L1613;}}
        else {
          goto L1613;}}}
    else {
      goto L1613;}}
  else {
    if ((next_141X == NULL)) {
      if ((end_138X < (Sweak_pointer_limitS))) {
        *((long *) end_138X) = (74 + ((((-4 & ((Sweak_pointer_limitS) - (end_138X + 4))))<<8)));
        goto L3389;}
      else {
        goto L3389;}}
    else {
      arg0K0 = (next_141X + -1024);
      arg0K1 = next_141X;
      goto L1907;}}}
 L1613: {
  arg0K0 = (scan_142X + 8);
  goto L1567;}
 L1608: {
  x2_146X = arg1K0;
  *((long *) (scan_142X + 4)) = x2_146X;
  goto L1613;}
}
void s48_make_availableAgc(long len_147X)
{
  char x_148X;
 {  x_148X = ((s48_ShpS) + (-4 & (3 + len_147X))) < (s48_SlimitS);
  if (x_148X) {
    goto L3480;}
  else {
    s48_collect();
    goto L3480;}}
 L3480: {
  if ((((s48_ShpS) + (-4 & (3 + len_147X))) < (s48_SlimitS))) {
    return;}
  else {
    ps_error("Scheme 48 heap overflow", 0);
    return;}}
}
char * s48_allocate_tracedAgc(long len_149X)
{
  char * new_151X;
  char x_150X;
 {  x_150X = ((s48_ShpS) + (-4 & (3 + len_149X))) < (s48_SlimitS);
  if (x_150X) {
    goto L3554;}
  else {
    s48_collect();
    goto L3554;}}
 L3554: {
  if ((((s48_ShpS) + (-4 & (3 + len_149X))) < (s48_SlimitS))) {
    new_151X = s48_ShpS;
    s48_ShpS = ((s48_ShpS) + (-4 & (3 + len_149X)));
    return new_151X;}
  else {
    return NULL;}}
}
char * s48_allocate_untracedAgc(long len_152X)
{
  char * new_154X;
  char x_153X;
 {  x_153X = ((s48_ShpS) + (-4 & (3 + len_152X))) < (s48_SlimitS);
  if (x_153X) {
    goto L3626;}
  else {
    s48_collect();
    goto L3626;}}
 L3626: {
  if ((((s48_ShpS) + (-4 & (3 + len_152X))) < (s48_SlimitS))) {
    new_154X = s48_ShpS;
    s48_ShpS = ((s48_ShpS) + (-4 & (3 + len_152X)));
    return new_154X;}
  else {
    return NULL;}}
}
long s48_allocate_stob(long type_155X, long size_156X)
{
  long arg1K0;
  char * arg0K0;
  long v_165X;
  char * thing_164X;
  char * new_163X;
  char * new_162X;
  char x_161X;
  char x_160X;
  long needed_159X;
  long length_in_bytes_158X;
  char tracedP_157X;
 {  tracedP_157X = type_155X < 17;
  if (tracedP_157X) {
    arg1K0 = (((size_156X)<<2));
    goto L3652;}
  else {
    arg1K0 = size_156X;
    goto L3652;}}
 L3652: {
  length_in_bytes_158X = arg1K0;
  needed_159X = 4 + length_in_bytes_158X;
  if (tracedP_157X) {
    x_160X = ((s48_ShpS) + (-4 & (3 + needed_159X))) < (s48_SlimitS);
    if (x_160X) {
      goto L3710;}
    else {
      s48_collect();
      goto L3710;}}
  else {
    x_161X = ((s48_ShpS) + (-4 & (3 + needed_159X))) < (s48_SlimitS);
    if (x_161X) {
      goto L3729;}
    else {
      s48_collect();
      goto L3729;}}}
 L3710: {
  if ((((s48_ShpS) + (-4 & (3 + needed_159X))) < (s48_SlimitS))) {
    new_162X = s48_ShpS;
    s48_ShpS = ((s48_ShpS) + (-4 & (3 + needed_159X)));
    arg0K0 = new_162X;
    goto L3668;}
  else {
    arg0K0 = NULL;
    goto L3668;}}
 L3729: {
  if ((((s48_ShpS) + (-4 & (3 + needed_159X))) < (s48_SlimitS))) {
    new_163X = s48_ShpS;
    s48_ShpS = ((s48_ShpS) + (-4 & (3 + needed_159X)));
    arg0K0 = new_163X;
    goto L3668;}
  else {
    arg0K0 = NULL;
    goto L3668;}}
 L3668: {
  thing_164X = arg0K0;
  if ((thing_164X == NULL)) {
    ps_error("insufficient heap space for external allocation", 0);
    return v_165X;}
  else {
    *((long *) thing_164X) = (2 + (((((((length_in_bytes_158X)<<6)) + type_155X))<<2)));
    return (3 + (((long) (thing_164X + 4))));}}
}void
s48_heap_init(void)
{
Spure_area_countS = 0;
Simpure_area_countS = 0;
Sfinding_typeS = 1;
Sheap_errors_leftS = 0;
Sgc_countS = 0;
Hthe_record_type10620 = 1;
}
