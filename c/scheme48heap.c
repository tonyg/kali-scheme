#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

static long copy_weak_pointer(long, char *, char **);
static void clean_weak_pointers(void);
static char gc_recordP(long);
static void note_undumpableB(long);
void s48_write_barrier(long, char *, long);
long s48_heap_size(void);
long s48_preallocate_space(long);
void s48_register_static_areas(long, char **, long*, long, char **, long*);
long s48_gc_count(void);
void s48_end_collection(void);
long s48_undumpable_records(long*);
long s48_startup_procedure(void);
long s48_initial_symbols(void);
long s48_initial_imported_bindings(void);
long s48_initial_exported_bindings(void);
long s48_resumer_records(void);
void s48_set_image_valuesB(long, long, long, long, long);
void s48_initialization_completeB(void);
void s48_initialize_heap(char *, long);
char s48_availableP(long);
char s48_image_writing_okayP(void);
char * s48_allocate_space(long, long, long);
long s48_available(void);
char s48_extantP(long);
long s48_check_image_header(char*);
long s48_find_all(long);
long s48_read_image(void);
long s48_find_all_records(long);
long s48_trace_locationsB(char *, char *);
void s48_do_gc(void);
long s48_trace_stob_contentsB(long);
void s48_begin_collection(void);
long s48_trace_value(long);
long s48_write_image(long, FILE *);
void s48_initializing_gc_root(void);
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
static long Sgc_countS;
static char Swriting_imagePS;
static long Sundumpable_recordsS;
static long Sundumpable_countS;
static char * Sfrom_beginS;
static char * Sfrom_endS;
static char * Sweak_pointer_hpS;
static char * Sweak_pointer_limitS;
static long SstatusS;
static char SeofPS;
static FILE * Simage_portS;
static char * Sold_beginS;
static char * Sold_hpS;
static long Sstartup_procedureS;
static long SsymbolsS;
static long Simported_bindingsS;
static long Sexported_bindingsS;
static long Sresumer_recordsS;
static char SinitializingPS;
static long Hthe_record_type9260;
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
    goto L2423;}
  else {
    if (((Sweak_pointer_hpS) < (Sweak_pointer_limitS))) {
      arg0K0 = frontier_2X;
      goto L2428;}
    else {
      goto L2423;}}}
 L2423: {
  old_4X = Sweak_pointer_hpS;
  new_frontier_5X = frontier_2X + 1024;
  Sweak_pointer_hpS = frontier_2X;
  Sweak_pointer_limitS = new_frontier_5X;
  *((long *) (Sweak_pointer_hpS)) = 261190;
  *((long *) ((Sweak_pointer_hpS) + 8)) = (((long) old_4X));
  arg0K0 = new_frontier_5X;
  goto L2428;}
 L2428: {
  frontier_6X = arg0K0;
  new_7X = 3 + (((long) ((Sweak_pointer_hpS) + 4)));
  *((long *) ((Sweak_pointer_hpS) + 4)) = (*((long *) (((char *) (-3 + weak_1X)))));
  Sweak_pointer_hpS = ((Sweak_pointer_hpS) + 8);
  *((long *) ((((char *) (-3 + weak_1X))) + -4)) = new_7X;
  *TT0 = frontier_6X;
  return new_7X;}
}
static void clean_weak_pointers(void)
{
  char * arg0K1;
  char * arg0K0;
  long arg1K0;
  long x2_17X;
  long h_16X;
  char * a_15X;
  long value_14X;
  char * scan_13X;
  char * next_12X;
  char * end_11X;
  char * start_10X;
  char * end_9X;
  char * x_8X;
 {  if (((Sweak_pointer_hpS) == NULL)) {
    return;}
  else {
    x_8X = Sweak_pointer_limitS;
    end_9X = Sweak_pointer_hpS;
    arg0K0 = (x_8X + -1024);
    arg0K1 = end_9X;
    goto L3994;}}
 L3994: {
  start_10X = arg0K0;
  end_11X = arg0K1;
  next_12X = ((char *) (*((long *) (start_10X + 8))));
  arg0K0 = start_10X;
  goto L2635;}
 L2635: {
  scan_13X = arg0K0;
  if ((scan_13X < end_11X)) {
    *((long *) scan_13X) = 1074;
    value_14X = *((long *) (scan_13X + 4));
    if ((3 == (3 & value_14X))) {
      a_15X = ((char *) (-3 + value_14X));
      if ((a_15X < (Sfrom_beginS))) {
        goto L2681;}
      else {
        if ((a_15X < (Sfrom_endS))) {
          if ((3 == (3 & value_14X))) {
            h_16X = *((long *) ((((char *) (-3 + value_14X))) + -4));
            if ((3 == (3 & h_16X))) {
              arg1K0 = h_16X;
              goto L2676;}
            else {
              arg1K0 = 1;
              goto L2676;}}
          else {
            goto L2681;}}
        else {
          goto L2681;}}}
    else {
      goto L2681;}}
  else {
    if ((next_12X == NULL)) {
      if ((end_9X < (Sweak_pointer_limitS))) {
        *((long *) end_9X) = (70 + ((((-4 & ((Sweak_pointer_limitS) - (end_9X + 4))))<<8)));
        return;}
      else {
        return;}}
    else {
      arg0K0 = (next_12X + -1024);
      arg0K1 = next_12X;
      goto L3994;}}}
 L2681: {
  arg0K0 = (scan_13X + 8);
  goto L2635;}
 L2676: {
  x2_17X = arg1K0;
  *((long *) (scan_13X + 4)) = x2_17X;
  goto L2681;}
}
static char gc_recordP(long x_18X)
{
  long header_19X;
 {  if ((3 == (3 & x_18X))) {
    header_19X = *((long *) ((((char *) (-3 + x_18X))) + -4));
    if ((3 == (3 & header_19X))) {
      if ((3 == (3 & header_19X))) {
        return (8 == (31 & ((((*((long *) ((((char *) (-3 + header_19X))) + -4))))>>2))));}
      else {
        return 0;}}
    else {
      if ((3 == (3 & x_18X))) {
        return (8 == (31 & ((((*((long *) ((((char *) (-3 + x_18X))) + -4))))>>2))));}
      else {
        return 0;}}}
  else {
    return 0;}}
}
static void note_undumpableB(long thing_20X)
{
  long arg1K0;
  char * temp_34X;
  char * temp_33X;
  char * temp_32X;
  char * temp_31X;
  long x_30X;
  char * new_29X;
  long b_28X;
  char v_27X;
  char * temp_26X;
  char * temp_25X;
  char * temp_24X;
  char * temp_23X;
  long list_22X;
  long list_21X;
 {  if ((1000 < (Sundumpable_countS))) {
    return;}
  else {
    list_21X = Sundumpable_recordsS;
    arg1K0 = list_21X;
    goto L6367;}}
 L6367: {
  list_22X = arg1K0;
  if ((25 == list_22X)) {
    Sundumpable_countS = (1 + (Sundumpable_countS));
    temp_23X = s48_SlimitS;
    s48_SlimitS = (Soldspace_limitS);
    Soldspace_limitS = temp_23X;
    temp_24X = s48_ShpS;
    s48_ShpS = (Soldspace_hpS);
    Soldspace_hpS = temp_24X;
    temp_25X = Snewspace_beginS;
    Snewspace_beginS = (Soldspace_beginS);
    Soldspace_beginS = temp_25X;
    temp_26X = Snewspace_endS;
    Snewspace_endS = (Soldspace_endS);
    Soldspace_endS = temp_26X;
    v_27X = ((s48_ShpS) + 12) < (s48_SlimitS);
    if (v_27X) {
      b_28X = Sundumpable_recordsS;
      new_29X = s48_ShpS;
      s48_ShpS = ((s48_ShpS) + 12);
      *((long *) new_29X) = 2050;
      x_30X = 3 + (((long) (new_29X + 4)));
      *((long *) (((char *) (-3 + x_30X)))) = thing_20X;
      *((long *) ((((char *) (-3 + x_30X))) + 4)) = b_28X;
      Sundumpable_recordsS = x_30X;
      goto L6289;}
    else {
      goto L6289;}}
  else {
    if (((*((long *) (((char *) (-3 + list_22X))))) == thing_20X)) {
      return;}
    else {
      arg1K0 = (*((long *) ((((char *) (-3 + list_22X))) + 4)));
      goto L6367;}}}
 L6289: {
  temp_31X = s48_SlimitS;
  s48_SlimitS = (Soldspace_limitS);
  Soldspace_limitS = temp_31X;
  temp_32X = s48_ShpS;
  s48_ShpS = (Soldspace_hpS);
  Soldspace_hpS = temp_32X;
  temp_33X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = temp_33X;
  temp_34X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = temp_34X;
  return;}
}
void s48_write_barrier(long stob_35X, char * address_36X, long value_37X)
{

 {  return;}
}
long s48_heap_size(void)
{

 {  return ((Snewspace_endS) - (Snewspace_beginS));}
}
long s48_preallocate_space(long cells_38X)
{

 {  return 0;}
}
void s48_register_static_areas(long pure_count_39X, char * *pure_areas_40X, long *pure_sizes_41X, long impure_count_42X, char * *impure_areas_43X, long *impure_sizes_44X)
{

 {  Spure_area_countS = pure_count_39X;
  Spure_areasS = pure_areas_40X;
  Spure_sizesS = pure_sizes_41X;
  Simpure_area_countS = impure_count_42X;
  Simpure_areasS = impure_areas_43X;
  Simpure_sizesS = impure_sizes_44X;
  return;}
}
long s48_gc_count(void)
{

 {  return (Sgc_countS);}
}
void s48_end_collection(void)
{

 {  Sgc_countS = (1 + (Sgc_countS));
  return;}
}
long s48_undumpable_records(long *TT0)
{

 {  *TT0 = (Sundumpable_countS);
  return (Sundumpable_recordsS);}
}
long s48_startup_procedure(void)
{

 {  return (Sstartup_procedureS);}
}
long s48_initial_symbols(void)
{

 {  return (SsymbolsS);}
}
long s48_initial_imported_bindings(void)
{

 {  return (Simported_bindingsS);}
}
long s48_initial_exported_bindings(void)
{

 {  return (Sexported_bindingsS);}
}
long s48_resumer_records(void)
{

 {  return (Sresumer_recordsS);}
}
void s48_set_image_valuesB(long startup_proc_45X, long symbols_46X, long imports_47X, long exports_48X, long records_49X)
{

 {  Sstartup_procedureS = startup_proc_45X;
  SsymbolsS = symbols_46X;
  Simported_bindingsS = imports_47X;
  Sexported_bindingsS = exports_48X;
  Sresumer_recordsS = records_49X;
  return;}
}
void s48_initialization_completeB(void)
{

 {  SinitializingPS = 0;
  return;}
}
void s48_initialize_heap(char * start_50X, long size_51X)
{
  long semisize_53X;
  long cells_52X;
 {  cells_52X = size_51X / 2;
  semisize_53X = ((cells_52X)<<2);
  Snewspace_beginS = start_50X;
  Snewspace_endS = ((Snewspace_beginS) + semisize_53X);
  s48_ShpS = (Snewspace_beginS);
  s48_SlimitS = (Snewspace_endS);
  Soldspace_beginS = (Snewspace_endS);
  Soldspace_endS = ((Soldspace_beginS) + semisize_53X);
  Soldspace_hpS = (Soldspace_beginS);
  Soldspace_limitS = (Soldspace_endS);
  return;}
}
char s48_availableP(long cells_54X)
{

 {  return (((s48_ShpS) + (((cells_54X)<<2))) < (s48_SlimitS));}
}
char s48_image_writing_okayP(void)
{

 {  if ((0 < (Simpure_area_countS))) {
    return 0;}
  else {
    if ((0 < (Spure_area_countS))) {
      return 0;}
    else {
      return 1;}}}
}
char * s48_allocate_space(long type_55X, long len_56X, long key_57X)
{
  char * new_58X;
 {  new_58X = s48_ShpS;
  s48_ShpS = ((s48_ShpS) + (-4 & (3 + len_56X)));
  return new_58X;}
}
long s48_available(void)
{

 {  return (((((s48_SlimitS) - (s48_ShpS)))>>2));}
}
char s48_extantP(long thing_59X)
{
  char * a_60X;
 {  if ((3 == (3 & thing_59X))) {
    if ((3 == (3 & thing_59X))) {
      a_60X = ((char *) (-3 + thing_59X));
      if ((a_60X < (Sfrom_beginS))) {
        return 1;}
      else {
        if ((a_60X < (Sfrom_endS))) {
          return (3 == (3 & (*((long *) ((((char *) (-3 + thing_59X))) + -4)))));}
        else {
          return 1;}}}
    else {
      return 1;}}
  else {
    return 1;}}
}
long s48_check_image_header(char *filename_61X)
{
  long arg1K0;
  long arg1K2;
  long arg1K1;
  char arg2K1;
  char arg2K0;
  long status_122X;
  char eofP_121X;
  long status_120X;
  long status_119X;
  long status_118X;
  long status_117X;
  long status_116X;
  char eofP_115X;
  char ch_114X;
  long v_113X;
  long status_112X;
  char eofP_111X;
  long thing_110X;
  long v_109X;
  long status_108X;
  char eofP_107X;
  long thing_106X;
  long v_105X;
  long status_104X;
  char eofP_103X;
  long thing_102X;
  long v_101X;
  long status_100X;
  char eofP_99X;
  long thing_98X;
  long v_97X;
  long status_96X;
  char eofP_95X;
  long thing_94X;
  long cells_93X;
  long status_92X;
  char eofP_91X;
  long thing_90X;
  long cells_89X;
  long status_88X;
  char eofP_87X;
  char thing_86X;
  long status_85X;
  char eofP_84X;
  long thing_83X;
  long old_bytes_per_cell_82X;
  long status_81X;
  char eofP_80X;
  long status_79X;
  char eofP_78X;
  char ch_77X;
  long i_76X;
  long status_75X;
  char eofP_74X;
  long thing_73X;
  char same_versionP_72X;
  long status_71X;
  char eofP_70X;
  long status_69X;
  char eofP_68X;
  char ch_67X;
  long status_66X;
  char eofP_65X;
  char ch_64X;
  long status_63X;
  FILE * port_62X;
 {  port_62X = ps_open_input_file(filename_61X, &status_63X);
  if ((status_63X == NO_ERRORS)) {
    SstatusS = NO_ERRORS;
    SeofPS = 0;
    if ((SeofPS)) {
      goto L2893;}
    else {
      goto L3626;}}
  else {
    ps_write_string("Can't open heap image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    ps_write_string((ps_error_string(status_63X)), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2893: {
  if ((SeofPS)) {
    goto L2926;}
  else {
    goto L3590;}}
 L3626: {
  PS_READ_CHAR(port_62X, ch_64X, eofP_65X, status_66X)
  if (eofP_65X) {
    arg2K0 = eofP_65X;
    arg1K1 = status_66X;
    goto L2871;}
  else {
    if ((status_66X == NO_ERRORS)) {
      if ((12 == ch_64X)) {
        arg2K0 = 0;
        arg1K1 = status_66X;
        goto L2871;}
      else {
        goto L3626;}}
    else {
      arg2K0 = eofP_65X;
      arg1K1 = status_66X;
      goto L2871;}}}
 L2926: {
  if ((SeofPS)) {
    arg2K0 = 0;
    goto L2959;}
  else {
    arg1K0 = 0;
    goto L3566;}}
 L3590: {
  PS_READ_CHAR(port_62X, ch_67X, eofP_68X, status_69X)
  if (eofP_68X) {
    arg2K0 = eofP_68X;
    arg1K1 = status_69X;
    goto L2904;}
  else {
    if ((status_69X == NO_ERRORS)) {
      if ((10 == ch_67X)) {
        arg2K0 = 0;
        arg1K1 = status_69X;
        goto L2904;}
      else {
        goto L3590;}}
    else {
      arg2K0 = eofP_68X;
      arg1K1 = status_69X;
      goto L2904;}}}
 L2871: {
  eofP_70X = arg2K0;
  status_71X = arg1K1;
  if (eofP_70X) {
    SeofPS = 1;
    goto L2893;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2893;}
    else {
      SeofPS = 1;
      SstatusS = status_71X;
      goto L2893;}}}
 L2959: {
  same_versionP_72X = arg2K0;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2994;}
  else {
    PS_READ_INTEGER(port_62X, thing_73X, eofP_74X, status_75X)
    if (eofP_74X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2994;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_73X;
        goto L2994;}
      else {
        SeofPS = 1;
        SstatusS = status_75X;
        arg1K0 = -1;
        goto L2994;}}}}
 L3566: {
  i_76X = arg1K0;
  PS_READ_CHAR(port_62X, ch_77X, eofP_78X, status_79X)
  if (eofP_78X) {
    arg2K0 = 0;
    arg2K1 = eofP_78X;
    arg1K2 = status_79X;
    goto L2937;}
  else {
    if ((status_79X == NO_ERRORS)) {
      if ((i_76X == (strlen((char *) "Vanilla 20")))) {
        arg2K0 = (10 == ch_77X);
        arg2K1 = 0;
        arg1K2 = status_79X;
        goto L2937;}
      else {
        if ((ch_77X == (*("Vanilla 20" + i_76X)))) {
          arg1K0 = (1 + i_76X);
          goto L3566;}
        else {
          arg2K0 = 0;
          arg2K1 = 0;
          arg1K2 = status_79X;
          goto L2937;}}}
    else {
      arg2K0 = 0;
      arg2K1 = eofP_78X;
      arg1K2 = status_79X;
      goto L2937;}}}
 L2904: {
  eofP_80X = arg2K0;
  status_81X = arg1K1;
  if (eofP_80X) {
    SeofPS = 1;
    goto L2926;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2926;}
    else {
      SeofPS = 1;
      SstatusS = status_81X;
      goto L2926;}}}
 L2994: {
  old_bytes_per_cell_82X = arg1K0;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L3029;}
  else {
    PS_READ_INTEGER(port_62X, thing_83X, eofP_84X, status_85X)
    if (eofP_84X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L3029;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_83X;
        goto L3029;}
      else {
        SeofPS = 1;
        SstatusS = status_85X;
        arg1K0 = -1;
        goto L3029;}}}}
 L2937: {
  thing_86X = arg2K0;
  eofP_87X = arg2K1;
  status_88X = arg1K2;
  if (eofP_87X) {
    SeofPS = 1;
    arg2K0 = 0;
    goto L2959;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      arg2K0 = thing_86X;
      goto L2959;}
    else {
      SeofPS = 1;
      SstatusS = status_88X;
      arg2K0 = 0;
      goto L2959;}}}
 L3029: {
  cells_89X = arg1K0;
  Sold_beginS = (((char *) (((cells_89X)<<2))));
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L3067;}
  else {
    PS_READ_INTEGER(port_62X, thing_90X, eofP_91X, status_92X)
    if (eofP_91X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L3067;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_90X;
        goto L3067;}
      else {
        SeofPS = 1;
        SstatusS = status_92X;
        arg1K0 = -1;
        goto L3067;}}}}
 L3067: {
  cells_93X = arg1K0;
  Sold_hpS = (((char *) (((cells_93X)<<2))));
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L3105;}
  else {
    PS_READ_INTEGER(port_62X, thing_94X, eofP_95X, status_96X)
    if (eofP_95X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L3105;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_94X;
        goto L3105;}
      else {
        SeofPS = 1;
        SstatusS = status_96X;
        arg1K0 = -1;
        goto L3105;}}}}
 L3105: {
  v_97X = arg1K0;
  SsymbolsS = v_97X;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L3141;}
  else {
    PS_READ_INTEGER(port_62X, thing_98X, eofP_99X, status_100X)
    if (eofP_99X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L3141;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_98X;
        goto L3141;}
      else {
        SeofPS = 1;
        SstatusS = status_100X;
        arg1K0 = -1;
        goto L3141;}}}}
 L3141: {
  v_101X = arg1K0;
  Simported_bindingsS = v_101X;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L3177;}
  else {
    PS_READ_INTEGER(port_62X, thing_102X, eofP_103X, status_104X)
    if (eofP_103X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L3177;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_102X;
        goto L3177;}
      else {
        SeofPS = 1;
        SstatusS = status_104X;
        arg1K0 = -1;
        goto L3177;}}}}
 L3177: {
  v_105X = arg1K0;
  Sexported_bindingsS = v_105X;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L3213;}
  else {
    PS_READ_INTEGER(port_62X, thing_106X, eofP_107X, status_108X)
    if (eofP_107X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L3213;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_106X;
        goto L3213;}
      else {
        SeofPS = 1;
        SstatusS = status_108X;
        arg1K0 = -1;
        goto L3213;}}}}
 L3213: {
  v_109X = arg1K0;
  Sresumer_recordsS = v_109X;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L3249;}
  else {
    PS_READ_INTEGER(port_62X, thing_110X, eofP_111X, status_112X)
    if (eofP_111X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L3249;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_110X;
        goto L3249;}
      else {
        SeofPS = 1;
        SstatusS = status_112X;
        arg1K0 = -1;
        goto L3249;}}}}
 L3249: {
  v_113X = arg1K0;
  Sstartup_procedureS = v_113X;
  Simage_portS = port_62X;
  if ((SeofPS)) {
    goto L3288;}
  else {
    goto L3497;}}
 L3288: {
  if (((SstatusS) == NO_ERRORS)) {
    if ((SeofPS)) {
      SstatusS = EDOM;
      ps_write_string("Premature EOF when reading image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L3369;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L3369;}}
    else {
      if (same_versionP_72X) {
        if ((4 == old_bytes_per_cell_82X)) {
          return ((Sold_hpS) - (Sold_beginS));}
        else {
          ps_write_string("Incompatible bytes-per-cell in image", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L3429;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L3429;}}}
      else {
        ps_write_string("Format of image is incompatible with this version of system", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L3399;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L3399;}}}}
  else {
    ps_write_string("Error reading from image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L3344;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L3344;}}}
 L3497: {
  PS_READ_CHAR(port_62X, ch_114X, eofP_115X, status_116X)
  if (eofP_115X) {
    arg2K0 = eofP_115X;
    arg1K1 = status_116X;
    goto L3266;}
  else {
    if ((status_116X == NO_ERRORS)) {
      if ((12 == ch_114X)) {
        arg2K0 = 0;
        arg1K1 = status_116X;
        goto L3266;}
      else {
        goto L3497;}}
    else {
      arg2K0 = eofP_115X;
      arg1K1 = status_116X;
      goto L3266;}}}
 L3369: {
  status_117X = ps_close(port_62X);
  if ((status_117X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3429: {
  status_118X = ps_close(port_62X);
  if ((status_118X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3399: {
  status_119X = ps_close(port_62X);
  if ((status_119X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3344: {
  status_120X = ps_close(port_62X);
  if ((status_120X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3266: {
  eofP_121X = arg2K0;
  status_122X = arg1K1;
  if (eofP_121X) {
    SeofPS = 1;
    goto L3288;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L3288;}
    else {
      SeofPS = 1;
      SstatusS = status_122X;
      goto L3288;}}}
}
long s48_find_all(long type_123X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc923124_return_tag;
  char Hproc9231240_return_value;
  char * start_125X;
  char * end_126X;
  char * next_142X;
  long d_141X;
  char * addr_140X;
  long type_139X;
  char v_138X;
  long i_137X;
  long count_136X;
  long *sizes_135X;
  char * *areas_134X;
  char v_133X;
  long i_132X;
  long count_131X;
  long *sizes_130X;
  char * *areas_129X;
  char v_128X;
  char * start_hp_127X;
 {  Sfinding_typeS = type_123X;
  start_hp_127X = s48_ShpS;
  *((long *) (s48_ShpS)) = 0;
  s48_ShpS = ((s48_ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_127X;
  Hproc923124_return_tag = 0;
  goto Hproc923124;
 Hproc923124_return_0:
  v_128X = Hproc9231240_return_value;
  if (v_128X) {
    if ((0 < (Simpure_area_countS))) {
      areas_129X = Simpure_areasS;
      sizes_130X = Simpure_sizesS;
      count_131X = Simpure_area_countS;
      arg1K0 = 0;
      goto L5577;}
    else {
      goto L5517;}}
  else {
    goto L5535;}}
 L5577: {
  i_132X = arg1K0;
  if ((i_132X < count_131X)) {
    merged_arg0K0 = (*(areas_129X + i_132X));
    merged_arg0K1 = ((*(areas_129X + i_132X)) + (*(sizes_130X + i_132X)));
    Hproc923124_return_tag = 1;
    goto Hproc923124;
   Hproc923124_return_1:
    v_133X = Hproc9231240_return_value;
    if (v_133X) {
      arg1K0 = (1 + i_132X);
      goto L5577;}
    else {
      goto L5535;}}
  else {
    goto L5517;}}
 L5517: {
  if ((0 < (Spure_area_countS))) {
    areas_134X = Spure_areasS;
    sizes_135X = Spure_sizesS;
    count_136X = Spure_area_countS;
    arg1K0 = 0;
    goto L5598;}
  else {
    goto L5524;}}
 L5535: {
  s48_ShpS = start_hp_127X;
  return 1;}
 L5598: {
  i_137X = arg1K0;
  if ((i_137X < count_136X)) {
    merged_arg0K0 = (*(areas_134X + i_137X));
    merged_arg0K1 = ((*(areas_134X + i_137X)) + (*(sizes_135X + i_137X)));
    Hproc923124_return_tag = 2;
    goto Hproc923124;
   Hproc923124_return_2:
    v_138X = Hproc9231240_return_value;
    if (v_138X) {
      arg1K0 = (1 + i_137X);
      goto L5598;}
    else {
      goto L5535;}}
  else {
    goto L5524;}}
 L5524: {
  *((long *) start_hp_127X) = (10 + (((((s48_ShpS) - (start_hp_127X + 4)))<<8)));
  return (3 + (((long) (start_hp_127X + 4))));}
 Hproc923124: {
  start_125X = merged_arg0K0;
  end_126X = merged_arg0K1;{
  type_139X = Sfinding_typeS;
  arg0K0 = start_125X;
  goto L4503;}
 L4503: {
  addr_140X = arg0K0;
  if ((addr_140X < end_126X)) {
    d_141X = *((long *) addr_140X);
    next_142X = addr_140X + (4 + (-4 & (3 + ((long)(((unsigned long)d_141X)>>8)))));
    if ((2 == (3 & d_141X))) {
      if ((type_139X == (31 & (((d_141X)>>2))))) {
        if ((((s48_ShpS) + 16) < (s48_SlimitS))) {
          *((long *) (s48_ShpS)) = (3 + (((long) (addr_140X + 4))));
          s48_ShpS = ((s48_ShpS) + 4);
          arg0K0 = next_142X;
          goto L4503;}
        else {
          Hproc9231240_return_value = 0;
          goto Hproc923124_return;}}
      else {
        arg0K0 = next_142X;
        goto L4503;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc9231240_return_value = 0;
      goto Hproc923124_return;}}
  else {
    Hproc9231240_return_value = 1;
    goto Hproc923124_return;}}
 Hproc923124_return:
  switch (Hproc923124_return_tag) {
  case 0: goto Hproc923124_return_0;
  case 1: goto Hproc923124_return_1;
  default: goto Hproc923124_return_2;
  }}

}
long s48_read_image(void)
{
  FILE * arg4K0;
  char *arg3K1;
  char arg2K1;
  char arg2K0;
  char * arg0K0;
  long arg1K0;
  long v_192X;
  long descriptor_191X;
  long v_190X;
  long descriptor_189X;
  long v_188X;
  long descriptor_187X;
  long v_186X;
  long d_185X;
  long descriptor_184X;
  long v_183X;
  long descriptor_182X;
  char * ptr_181X;
  long descriptor_180X;
  long status_179X;
  long status_178X;
  long status_177X;
  long status_176X;
  char * next_175X;
  long value_174X;
  long x_173X;
  long x_172X;
  char * ptr_171X;
  long status_170X;
  long status_169X;
  long status_168X;
  char eofP_167X;
  char v_166X;
  char *string_165X;
  char okayP_164X;
  long status_163X;
  long status_162X;
  long status_161X;
  char eofP_160X;
  long got_159X;
  long need_158X;
  char * start_157X;
  char * new_limit_156X;
  char * new_hp_155X;
  long delta_154X;
  char reverseP_153X;
  FILE * port_152X;
  long x_151X;
  long x_150X;
  char * addr_149X;
  char *string_148X;
  char okayP_147X;
  long status_146X;
  char eofP_145X;
  long got_144X;
  FILE * port_143X;
 {  port_143X = Simage_portS;
  got_144X = ps_read_block(port_143X, ((char *) (s48_ShpS)), 4, &eofP_145X, &status_146X);
  if ((status_146X == NO_ERRORS)) {
    if (eofP_145X) {
      arg2K0 = 0;
      arg3K1 = "Premature EOF when reading image file";
      goto L6079;}
    else {
      if ((got_144X < 4)) {
        arg2K0 = 0;
        arg3K1 = "Read returned too few bytes";
        goto L6079;}
      else {
        arg2K0 = 1;
        arg3K1 = "";
        goto L6079;}}}
  else {
    SstatusS = status_146X;
    arg2K0 = 0;
    arg3K1 = "Error reading from image file";
    goto L6079;}}
 L6079: {
  okayP_147X = arg2K0;
  string_148X = arg3K1;
  if (okayP_147X) {
    if ((1 == (*((long *) (s48_ShpS))))) {
      arg4K0 = port_143X;
      arg2K1 = 0;
      goto L5092;}
    else {
      addr_149X = s48_ShpS;
      x_150X = *((unsigned char *) addr_149X);
      *((unsigned char *) addr_149X) = (*((unsigned char *) (addr_149X + 3)));
      *((unsigned char *) (addr_149X + 3)) = x_150X;
      x_151X = *((unsigned char *) (addr_149X + 1));
      *((unsigned char *) (addr_149X + 1)) = (*((unsigned char *) (addr_149X + 2)));
      *((unsigned char *) (addr_149X + 2)) = x_151X;
      if ((1 == (*((long *) (s48_ShpS))))) {
        arg4K0 = port_143X;
        arg2K1 = 1;
        goto L5092;}
      else {
        ps_write_string("Unable to correct byte order", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L6187;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L6187;}}}}
  else {
    ps_write_string(string_148X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L6135;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L6135;}}}
 L5092: {
  port_152X = arg4K0;
  reverseP_153X = arg2K1;
  delta_154X = (s48_ShpS) - (Sold_beginS);
  new_hp_155X = (Sold_hpS) + delta_154X;
  new_limit_156X = Snewspace_endS;
  start_157X = s48_ShpS;
  if ((new_hp_155X < new_limit_156X)) {
    need_158X = (Sold_hpS) - (Sold_beginS);
    got_159X = ps_read_block(port_152X, ((char *) (s48_ShpS)), need_158X, &eofP_160X, &status_161X);
    if ((status_161X == NO_ERRORS)) {
      if (eofP_160X) {
        arg2K0 = 0;
        arg3K1 = "Premature EOF when reading image file";
        goto L5114;}
      else {
        if ((got_159X < need_158X)) {
          arg2K0 = 0;
          arg3K1 = "Read returned too few bytes";
          goto L5114;}
        else {
          arg2K0 = 1;
          arg3K1 = "";
          goto L5114;}}}
    else {
      SstatusS = status_161X;
      arg2K0 = 0;
      arg3K1 = "Error reading from image file";
      goto L5114;}}
  else {
    ps_write_string("Heap not big enough to restore this image", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L5210;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L5210;}}}
 L6187: {
  status_162X = ps_close(port_143X);
  if ((status_162X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L6135: {
  status_163X = ps_close(port_143X);
  if ((status_163X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L5114: {
  okayP_164X = arg2K0;
  string_165X = arg3K1;
  PS_READ_CHAR(port_152X, v_166X, eofP_167X, status_168X)
  if (okayP_164X) {
    if ((status_168X == NO_ERRORS)) {
      if (eofP_167X) {
        status_169X = ps_close(port_152X);
        if ((status_169X == NO_ERRORS)) {
          if (reverseP_153X) {
            ps_write_string("Correcting byte order of resumed image.", (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = start_157X;
            goto L4309;}
          else {
            goto L5152;}}
        else {
          ps_write_string("Error closing image file", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L5345;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L5345;}}}
      else {
        ps_write_string("Image file has extraneous data after image", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L5315;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L5315;}}}
    else {
      ps_write_string("Error reading from image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L5285;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L5285;}}}
  else {
    ps_write_string(string_165X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L5255;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L5255;}}}
 L5210: {
  status_170X = ps_close(port_152X);
  if ((status_170X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4309: {
  ptr_171X = arg0K0;
  if ((ptr_171X < new_hp_155X)) {
    x_172X = *((unsigned char *) ptr_171X);
    *((unsigned char *) ptr_171X) = (*((unsigned char *) (ptr_171X + 3)));
    *((unsigned char *) (ptr_171X + 3)) = x_172X;
    x_173X = *((unsigned char *) (ptr_171X + 1));
    *((unsigned char *) (ptr_171X + 1)) = (*((unsigned char *) (ptr_171X + 2)));
    *((unsigned char *) (ptr_171X + 2)) = x_173X;
    value_174X = *((long *) ptr_171X);
    next_175X = ptr_171X + 4;
    if ((2 == (3 & value_174X))) {
      if (((31 & (((value_174X)>>2))) < 16)) {
        arg0K0 = next_175X;
        goto L4309;}
      else {
        arg0K0 = (next_175X + (-4 & (3 + ((long)(((unsigned long)value_174X)>>8)))));
        goto L4309;}}
    else {
      arg0K0 = next_175X;
      goto L4309;}}
  else {
    goto L5152;}}
 L5152: {
  if ((0 == delta_154X)) {
    goto L5162;}
  else {
    arg0K0 = start_157X;
    goto L4408;}}
 L5345: {
  status_176X = ps_close(port_152X);
  if ((status_176X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L5315: {
  status_177X = ps_close(port_152X);
  if ((status_177X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L5285: {
  status_178X = ps_close(port_152X);
  if ((status_178X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L5255: {
  status_179X = ps_close(port_152X);
  if ((status_179X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L5162: {
  s48_ShpS = new_hp_155X;
  descriptor_180X = Sstartup_procedureS;
  if ((3 == (3 & descriptor_180X))) {
    arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_180X))) + delta_154X))));
    goto L5166;}
  else {
    arg1K0 = descriptor_180X;
    goto L5166;}}
 L4408: {
  ptr_181X = arg0K0;
  if ((ptr_181X < new_hp_155X)) {
    descriptor_182X = *((long *) ptr_181X);
    if ((3 == (3 & descriptor_182X))) {
      arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_182X))) + delta_154X))));
      goto L4413;}
    else {
      arg1K0 = descriptor_182X;
      goto L4413;}}
  else {
    goto L5162;}}
 L5166: {
  v_183X = arg1K0;
  Sstartup_procedureS = v_183X;
  descriptor_184X = SsymbolsS;
  if ((3 == (3 & descriptor_184X))) {
    arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_184X))) + delta_154X))));
    goto L5171;}
  else {
    arg1K0 = descriptor_184X;
    goto L5171;}}
 L4413: {
  d_185X = arg1K0;
  *((long *) ptr_181X) = d_185X;
  if ((2 == (3 & d_185X))) {
    if (((31 & (((d_185X)>>2))) < 16)) {
      goto L4427;}
    else {
      arg0K0 = (ptr_181X + (4 + (-4 & (3 + ((long)(((unsigned long)d_185X)>>8))))));
      goto L4408;}}
  else {
    goto L4427;}}
 L5171: {
  v_186X = arg1K0;
  SsymbolsS = v_186X;
  descriptor_187X = Simported_bindingsS;
  if ((3 == (3 & descriptor_187X))) {
    arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_187X))) + delta_154X))));
    goto L5176;}
  else {
    arg1K0 = descriptor_187X;
    goto L5176;}}
 L4427: {
  arg0K0 = (ptr_181X + 4);
  goto L4408;}
 L5176: {
  v_188X = arg1K0;
  Simported_bindingsS = v_188X;
  descriptor_189X = Sexported_bindingsS;
  if ((3 == (3 & descriptor_189X))) {
    arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_189X))) + delta_154X))));
    goto L5181;}
  else {
    arg1K0 = descriptor_189X;
    goto L5181;}}
 L5181: {
  v_190X = arg1K0;
  Sexported_bindingsS = v_190X;
  descriptor_191X = Sresumer_recordsS;
  if ((3 == (3 & descriptor_191X))) {
    arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_191X))) + delta_154X))));
    goto L5186;}
  else {
    arg1K0 = descriptor_191X;
    goto L5186;}}
 L5186: {
  v_192X = arg1K0;
  Sresumer_recordsS = v_192X;
  return 0;}
}
long s48_find_all_records(long record_type_193X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc969194_return_tag;
  char Hproc9691940_return_value;
  char * start_195X;
  char * end_196X;
  long obj_213X;
  char * next_212X;
  long d_211X;
  char * addr_210X;
  long type_209X;
  char v_208X;
  long i_207X;
  long count_206X;
  long *sizes_205X;
  char * *areas_204X;
  char v_203X;
  long i_202X;
  long count_201X;
  long *sizes_200X;
  char * *areas_199X;
  char v_198X;
  char * start_hp_197X;
 {  Hthe_record_type9260 = record_type_193X;
  Sfinding_typeS = 8;
  start_hp_197X = s48_ShpS;
  *((long *) (s48_ShpS)) = 0;
  s48_ShpS = ((s48_ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_197X;
  Hproc969194_return_tag = 0;
  goto Hproc969194;
 Hproc969194_return_0:
  v_198X = Hproc9691940_return_value;
  if (v_198X) {
    if ((0 < (Simpure_area_countS))) {
      areas_199X = Simpure_areasS;
      sizes_200X = Simpure_sizesS;
      count_201X = Simpure_area_countS;
      arg1K0 = 0;
      goto L5742;}
    else {
      goto L5682;}}
  else {
    goto L5700;}}
 L5742: {
  i_202X = arg1K0;
  if ((i_202X < count_201X)) {
    merged_arg0K0 = (*(areas_199X + i_202X));
    merged_arg0K1 = ((*(areas_199X + i_202X)) + (*(sizes_200X + i_202X)));
    Hproc969194_return_tag = 1;
    goto Hproc969194;
   Hproc969194_return_1:
    v_203X = Hproc9691940_return_value;
    if (v_203X) {
      arg1K0 = (1 + i_202X);
      goto L5742;}
    else {
      goto L5700;}}
  else {
    goto L5682;}}
 L5682: {
  if ((0 < (Spure_area_countS))) {
    areas_204X = Spure_areasS;
    sizes_205X = Spure_sizesS;
    count_206X = Spure_area_countS;
    arg1K0 = 0;
    goto L5763;}
  else {
    goto L5689;}}
 L5700: {
  s48_ShpS = start_hp_197X;
  return 1;}
 L5763: {
  i_207X = arg1K0;
  if ((i_207X < count_206X)) {
    merged_arg0K0 = (*(areas_204X + i_207X));
    merged_arg0K1 = ((*(areas_204X + i_207X)) + (*(sizes_205X + i_207X)));
    Hproc969194_return_tag = 2;
    goto Hproc969194;
   Hproc969194_return_2:
    v_208X = Hproc9691940_return_value;
    if (v_208X) {
      arg1K0 = (1 + i_207X);
      goto L5763;}
    else {
      goto L5700;}}
  else {
    goto L5689;}}
 L5689: {
  *((long *) start_hp_197X) = (10 + (((((s48_ShpS) - (start_hp_197X + 4)))<<8)));
  return (3 + (((long) (start_hp_197X + 4))));}
 Hproc969194: {
  start_195X = merged_arg0K0;
  end_196X = merged_arg0K1;{
  type_209X = Sfinding_typeS;
  arg0K0 = start_195X;
  goto L4670;}
 L4670: {
  addr_210X = arg0K0;
  if ((addr_210X < end_196X)) {
    d_211X = *((long *) addr_210X);
    next_212X = addr_210X + (4 + (-4 & (3 + ((long)(((unsigned long)d_211X)>>8)))));
    if ((2 == (3 & d_211X))) {
      if ((type_209X == (31 & (((d_211X)>>2))))) {
        obj_213X = 3 + (((long) (addr_210X + 4)));
        if (((*((long *) (((char *) (-3 + obj_213X))))) == (Hthe_record_type9260))) {
          if ((((s48_ShpS) + 16) < (s48_SlimitS))) {
            *((long *) (s48_ShpS)) = obj_213X;
            s48_ShpS = ((s48_ShpS) + 4);
            arg0K0 = next_212X;
            goto L4670;}
          else {
            Hproc9691940_return_value = 0;
            goto Hproc969194_return;}}
        else {
          arg0K0 = next_212X;
          goto L4670;}}
      else {
        arg0K0 = next_212X;
        goto L4670;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc9691940_return_value = 0;
      goto Hproc969194_return;}}
  else {
    Hproc9691940_return_value = 1;
    goto Hproc969194_return;}}
 Hproc969194_return:
  switch (Hproc969194_return_tag) {
  case 0: goto Hproc969194_return_0;
  case 1: goto Hproc969194_return_1;
  default: goto Hproc969194_return_2;
  }}

}
long s48_trace_locationsB(char * start_214X, char * end_215X)
{
  char * arg0K0;
  char * arg0K1;
  long arg1K0;
  long new_232X;
  char * data_addr_231X;
  char * frontier_230X;
  long new_thing_229X;
  char * frontier_228X;
  long new_thing_227X;
  char * a_226X;
  long descriptor_225X;
  long h_224X;
  char * a_223X;
  char v_222X;
  long type_221X;
  char v_220X;
  char * next_219X;
  long thing_218X;
  char * frontier_217X;
  char * addr_216X;
 {  arg0K0 = start_214X;
  arg0K1 = (s48_ShpS);
  goto L6441;}
 L6441: {
  addr_216X = arg0K0;
  frontier_217X = arg0K1;
  if ((addr_216X < end_215X)) {
    thing_218X = *((long *) addr_216X);
    next_219X = addr_216X + 4;
    if ((2 == (3 & thing_218X))) {
      if ((2 == (3 & thing_218X))) {
        if (((31 & (((thing_218X)>>2))) < 16)) {
          arg0K0 = next_219X;
          arg0K1 = frontier_217X;
          goto L6441;}
        else {
          arg0K0 = (next_219X + (-4 & (3 + ((long)(((unsigned long)thing_218X)>>8)))));
          arg0K1 = frontier_217X;
          goto L6441;}}
      else {
        arg0K0 = next_219X;
        arg0K1 = frontier_217X;
        goto L6441;}}
    else {
      if ((Swriting_imagePS)) {
        v_220X = gc_recordP(thing_218X);
        if (v_220X) {
          type_221X = *((long *) (((char *) (-3 + thing_218X))));
          v_222X = gc_recordP(type_221X);
          if (v_222X) {
            if ((1 == (*((long *) ((((char *) (-3 + type_221X))) + 4))))) {
              note_undumpableB(thing_218X);
              *((long *) addr_216X) = (*((long *) ((((char *) (-3 + thing_218X))) + 4)));
              arg0K0 = addr_216X;
              arg0K1 = frontier_217X;
              goto L6441;}
            else {
              goto L6475;}}
          else {
            goto L6475;}}
        else {
          goto L6475;}}
      else {
        goto L6475;}}}
  else {
    s48_ShpS = frontier_217X;
    return 0;}}
 L6475: {
  if ((3 == (3 & thing_218X))) {
    a_223X = ((char *) (-3 + thing_218X));
    if ((a_223X < (Sfrom_beginS))) {
      arg0K0 = next_219X;
      arg0K1 = frontier_217X;
      goto L6441;}
    else {
      if ((a_223X < (Sfrom_endS))) {
        h_224X = *((long *) ((((char *) (-3 + thing_218X))) + -4));
        if ((3 == (3 & h_224X))) {
          arg1K0 = h_224X;
          arg0K1 = frontier_217X;
          goto L6482;}
        else {
          if ((1074 == h_224X)) {
            descriptor_225X = *((long *) (((char *) (-3 + thing_218X))));
            if ((3 == (3 & descriptor_225X))) {
              a_226X = ((char *) (-3 + descriptor_225X));
              if ((a_226X < (Sfrom_beginS))) {
                goto L7062;}
              else {
                if ((a_226X < (Sfrom_endS))) {
                  new_thing_227X = copy_weak_pointer(thing_218X, frontier_217X, &frontier_228X);
                  arg1K0 = new_thing_227X;
                  arg0K1 = frontier_228X;
                  goto L6482;}
                else {
                  goto L7062;}}}
            else {
              goto L7062;}}
          else {
            goto L7062;}}}
      else {
        arg0K0 = next_219X;
        arg0K1 = frontier_217X;
        goto L6441;}}}
  else {
    arg0K0 = next_219X;
    arg0K1 = frontier_217X;
    goto L6441;}}
 L6482: {
  new_thing_229X = arg1K0;
  frontier_230X = arg0K1;
  *((long *) addr_216X) = new_thing_229X;
  arg0K0 = next_219X;
  arg0K1 = frontier_230X;
  goto L6441;}
 L7062: {
  *((long *) frontier_217X) = h_224X;
  data_addr_231X = frontier_217X + 4;
  new_232X = 3 + (((long) data_addr_231X));
  *((long *) ((((char *) (-3 + thing_218X))) + -4)) = new_232X;
  memcpy((void *)data_addr_231X, (void *)(((char *) (-3 + thing_218X))),((long)(((unsigned long)h_224X)>>8)));
  arg1K0 = new_232X;
  arg0K1 = (data_addr_231X + (-4 & (3 + ((long)(((unsigned long)h_224X)>>8)))));
  goto L6482;}
}
void s48_do_gc(void)
{
  char * arg0K0;
  char * end_234X;
  char * start_233X;
 {  arg0K0 = (Snewspace_beginS);
  goto L6697;}
 L6697: {
  start_233X = arg0K0;
  end_234X = s48_ShpS;
  s48_trace_locationsB(start_233X, end_234X);
  if (((((((s48_SlimitS) - (s48_ShpS)))>>2)) < 0)) {
    ps_error("GC error: ran out of space in new heap", 0);
    return clean_weak_pointers();}
  else {
    if ((end_234X < (s48_ShpS))) {
      arg0K0 = end_234X;
      goto L6697;}
    else {
      return clean_weak_pointers();}}}
}
long s48_trace_stob_contentsB(long stob_235X)
{
  char * start_237X;
  long h_236X;
 {  h_236X = *((long *) ((((char *) (-3 + stob_235X))) + -4));
  start_237X = ((char *) (-3 + stob_235X));
  return s48_trace_locationsB(start_237X, (start_237X + (-4 & (3 + ((long)(((unsigned long)h_236X)>>8))))));}
}
void s48_begin_collection(void)
{
  long arg1K0;
  long i_245X;
  long count_244X;
  long *sizes_243X;
  char * *areas_242X;
  char * temp_241X;
  char * temp_240X;
  char * temp_239X;
  char * temp_238X;
 {  Sfrom_beginS = (Snewspace_beginS);
  Sfrom_endS = (Snewspace_endS);
  temp_238X = s48_SlimitS;
  s48_SlimitS = (Soldspace_limitS);
  Soldspace_limitS = temp_238X;
  temp_239X = s48_ShpS;
  s48_ShpS = (Soldspace_hpS);
  Soldspace_hpS = temp_239X;
  temp_240X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = temp_240X;
  temp_241X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = temp_241X;
  s48_ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = NULL;
  Swriting_imagePS = 0;
  if ((0 < (Simpure_area_countS))) {
    areas_242X = Simpure_areasS;
    sizes_243X = Simpure_sizesS;
    count_244X = Simpure_area_countS;
    arg1K0 = 0;
    goto L6812;}
  else {
    return;}}
 L6812: {
  i_245X = arg1K0;
  if ((i_245X < count_244X)) {
    s48_trace_locationsB((*(areas_242X + i_245X)), ((*(areas_242X + i_245X)) + (*(sizes_243X + i_245X))));
    arg1K0 = (1 + i_245X);
    goto L6812;}
  else {
    return;}}
}
long s48_trace_value(long stob_246X)
{
  char * arg0K1;
  long arg1K0;
  long new_261X;
  char * data_addr_260X;
  char * new_hp_259X;
  long new_thing_258X;
  char * new_hp_257X;
  long new_thing_256X;
  char * a_255X;
  long descriptor_254X;
  long h_253X;
  char * frontier_252X;
  char * a_251X;
  char v_250X;
  long type_249X;
  char v_248X;
  long stob_247X;
 {  arg1K0 = stob_247X;
  goto L7253;}
 L7253: {
  stob_247X = arg1K0;
  if ((Swriting_imagePS)) {
    v_248X = gc_recordP(stob_246X);
    if (v_248X) {
      type_249X = *((long *) (((char *) (-3 + stob_246X))));
      v_250X = gc_recordP(type_249X);
      if (v_250X) {
        if ((1 == (*((long *) ((((char *) (-3 + type_249X))) + 4))))) {
          note_undumpableB(stob_246X);
          arg1K0 = (*((long *) ((((char *) (-3 + stob_246X))) + 4)));
          goto L7253;}
        else {
          goto L6840;}}
      else {
        goto L6840;}}
    else {
      goto L6840;}}
  else {
    goto L6840;}}
 L6840: {
  if ((3 == (3 & stob_246X))) {
    a_251X = ((char *) (-3 + stob_246X));
    if ((a_251X < (Sfrom_beginS))) {
      return stob_246X;}
    else {
      if ((a_251X < (Sfrom_endS))) {
        frontier_252X = s48_ShpS;
        h_253X = *((long *) ((((char *) (-3 + stob_246X))) + -4));
        if ((3 == (3 & h_253X))) {
          arg1K0 = h_253X;
          arg0K1 = frontier_252X;
          goto L6862;}
        else {
          if ((1074 == h_253X)) {
            descriptor_254X = *((long *) (((char *) (-3 + stob_246X))));
            if ((3 == (3 & descriptor_254X))) {
              a_255X = ((char *) (-3 + descriptor_254X));
              if ((a_255X < (Sfrom_beginS))) {
                goto L4193;}
              else {
                if ((a_255X < (Sfrom_endS))) {
                  new_thing_256X = copy_weak_pointer(stob_246X, frontier_252X, &new_hp_257X);
                  arg1K0 = new_thing_256X;
                  arg0K1 = new_hp_257X;
                  goto L6862;}
                else {
                  goto L4193;}}}
            else {
              goto L4193;}}
          else {
            goto L4193;}}}
      else {
        return stob_246X;}}}
  else {
    return stob_246X;}}
 L6862: {
  new_thing_258X = arg1K0;
  new_hp_259X = arg0K1;
  s48_ShpS = new_hp_259X;
  return new_thing_258X;}
 L4193: {
  *((long *) frontier_252X) = h_253X;
  data_addr_260X = frontier_252X + 4;
  new_261X = 3 + (((long) data_addr_260X));
  *((long *) ((((char *) (-3 + stob_246X))) + -4)) = new_261X;
  memcpy((void *)data_addr_260X, (void *)(((char *) (-3 + stob_246X))),((long)(((unsigned long)h_253X)>>8)));
  arg1K0 = new_261X;
  arg0K1 = (data_addr_260X + (-4 & (3 + ((long)(((unsigned long)h_253X)>>8)))));
  goto L6862;}
}
long s48_write_image(long resume_proc_262X, FILE * port_263X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int abort_collection_return_tag;
  int Hproc967264_return_tag;
  char Hproc9672640_return_value;
  char * start_265X;
  char * end_266X;
  long h_320X;
  long h_319X;
  long d_318X;
  char * addr_317X;
  char * temp_316X;
  char * temp_315X;
  char * temp_314X;
  char * temp_313X;
  long type_312X;
  long obj_311X;
  char * next_310X;
  long d_309X;
  char * addr_308X;
  long type_307X;
  long status_306X;
  long v_305X;
  long v_304X;
  long v_303X;
  long v_302X;
  long v_301X;
  long v_300X;
  long v_299X;
  char * v_298X;
  long v_297X;
  char * v_296X;
  long v_295X;
  long v_294X;
  long v_293X;
  long v_292X;
  long v_291X;
  long imported_bindings_290X;
  long resumer_records_289X;
  char v_288X;
  long i_287X;
  long count_286X;
  long *sizes_285X;
  char * *areas_284X;
  char v_283X;
  long i_282X;
  long count_281X;
  long *sizes_280X;
  char * *areas_279X;
  char v_278X;
  char * start_hp_277X;
  long symbols_276X;
  char * end_275X;
  char * start_274X;
  long resume_proc_273X;
  long exported_bindings_272X;
  long v_271X;
  char * temp_270X;
  char * temp_269X;
  char * temp_268X;
  char * temp_267X;
 {  Sfrom_beginS = (Snewspace_beginS);
  Sfrom_endS = (Snewspace_endS);
  temp_267X = s48_SlimitS;
  s48_SlimitS = (Soldspace_limitS);
  Soldspace_limitS = temp_267X;
  temp_268X = s48_ShpS;
  s48_ShpS = (Soldspace_hpS);
  Soldspace_hpS = temp_268X;
  temp_269X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = temp_269X;
  temp_270X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = temp_270X;
  s48_ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = NULL;
  Swriting_imagePS = 1;
  Sundumpable_recordsS = 25;
  Sundumpable_countS = 0;
  v_271X = s48_exported_bindings();
  exported_bindings_272X = s48_trace_value(v_271X);
  resume_proc_273X = s48_trace_value(resume_proc_262X);
  arg0K0 = (Snewspace_beginS);
  goto L6983;}
 L6983: {
  start_274X = arg0K0;
  end_275X = s48_ShpS;
  s48_trace_locationsB(start_274X, end_275X);
  if (((((((s48_SlimitS) - (s48_ShpS)))>>2)) < 0)) {
    ps_error("GC error: ran out of space in new heap", 0);
    clean_weak_pointers();
    goto L6952;}
  else {
    if ((end_275X < (s48_ShpS))) {
      arg0K0 = end_275X;
      goto L6983;}
    else {
      clean_weak_pointers();
      goto L6952;}}}
 L6952: {
  s48_mark_traced_channels_closedB();
  symbols_276X = s48_copy_symbol_table();
  Sfinding_typeS = 8;
  start_hp_277X = s48_ShpS;
  *((long *) (s48_ShpS)) = 0;
  s48_ShpS = ((s48_ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_277X;
  Hproc967264_return_tag = 0;
  goto Hproc967264;
 Hproc967264_return_0:
  v_278X = Hproc9672640_return_value;
  if (v_278X) {
    if ((0 < (Simpure_area_countS))) {
      areas_279X = Simpure_areasS;
      sizes_280X = Simpure_sizesS;
      count_281X = Simpure_area_countS;
      arg1K0 = 0;
      goto L5907;}
    else {
      goto L5847;}}
  else {
    goto L5865;}}
 L5907: {
  i_282X = arg1K0;
  if ((i_282X < count_281X)) {
    merged_arg0K0 = (*(areas_279X + i_282X));
    merged_arg0K1 = ((*(areas_279X + i_282X)) + (*(sizes_280X + i_282X)));
    Hproc967264_return_tag = 1;
    goto Hproc967264;
   Hproc967264_return_1:
    v_283X = Hproc9672640_return_value;
    if (v_283X) {
      arg1K0 = (1 + i_282X);
      goto L5907;}
    else {
      goto L5865;}}
  else {
    goto L5847;}}
 L5847: {
  if ((0 < (Spure_area_countS))) {
    areas_284X = Spure_areasS;
    sizes_285X = Spure_sizesS;
    count_286X = Spure_area_countS;
    arg1K0 = 0;
    goto L5928;}
  else {
    goto L5854;}}
 L5865: {
  s48_ShpS = start_hp_277X;
  arg1K0 = 1;
  goto L7002;}
 L5928: {
  i_287X = arg1K0;
  if ((i_287X < count_286X)) {
    merged_arg0K0 = (*(areas_284X + i_287X));
    merged_arg0K1 = ((*(areas_284X + i_287X)) + (*(sizes_285X + i_287X)));
    Hproc967264_return_tag = 2;
    goto Hproc967264;
   Hproc967264_return_2:
    v_288X = Hproc9672640_return_value;
    if (v_288X) {
      arg1K0 = (1 + i_287X);
      goto L5928;}
    else {
      goto L5865;}}
  else {
    goto L5854;}}
 L5854: {
  *((long *) start_hp_277X) = (10 + (((((s48_ShpS) - (start_hp_277X + 4)))<<8)));
  arg1K0 = (3 + (((long) (start_hp_277X + 4))));
  goto L7002;}
 L7002: {
  resumer_records_289X = arg1K0;
  imported_bindings_290X = s48_cleaned_imported_bindings();
  if ((1 == resumer_records_289X)) {
    abort_collection_return_tag = 0;
    goto abort_collection;
   abort_collection_return_0:
    return ENOMEM;}
  else {
    SstatusS = NO_ERRORS;
    if (((SstatusS) == NO_ERRORS)) {
      PS_WRITE_CHAR(10, port_263X, v_291X)
      SstatusS = v_291X;
      goto L2019;}
    else {
      goto L2019;}}}
 L2019: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_263X, v_292X)
    SstatusS = v_292X;
    goto L2028;}
  else {
    goto L2028;}}
 L2028: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_293X)
    SstatusS = v_293X;
    goto L2037;}
  else {
    goto L2037;}}
 L2037: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string("Vanilla 20", port_263X));
    goto L2044;}
  else {
    goto L2044;}}
 L2044: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_294X)
    SstatusS = v_294X;
    goto L2053;}
  else {
    goto L2053;}}
 L2053: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, port_263X));
    goto L2123;}
  else {
    goto L2123;}}
 L2123: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_295X)
    SstatusS = v_295X;
    goto L2055;}
  else {
    goto L2055;}}
 L2055: {
  v_296X = Snewspace_beginS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(((((((long) v_296X)))>>2)), port_263X));
    goto L2143;}
  else {
    goto L2143;}}
 L2143: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_297X)
    SstatusS = v_297X;
    goto L2061;}
  else {
    goto L2061;}}
 L2061: {
  v_298X = s48_ShpS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(((((((long) v_298X)))>>2)), port_263X));
    goto L2163;}
  else {
    goto L2163;}}
 L2163: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_299X)
    SstatusS = v_299X;
    goto L2067;}
  else {
    goto L2067;}}
 L2067: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(symbols_276X, port_263X));
    goto L2178;}
  else {
    goto L2178;}}
 L2178: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_300X)
    SstatusS = v_300X;
    goto L2069;}
  else {
    goto L2069;}}
 L2069: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(imported_bindings_290X, port_263X));
    goto L2193;}
  else {
    goto L2193;}}
 L2193: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_301X)
    SstatusS = v_301X;
    goto L2071;}
  else {
    goto L2071;}}
 L2071: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(exported_bindings_272X, port_263X));
    goto L2208;}
  else {
    goto L2208;}}
 L2208: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_302X)
    SstatusS = v_302X;
    goto L2073;}
  else {
    goto L2073;}}
 L2073: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resumer_records_289X, port_263X));
    goto L2223;}
  else {
    goto L2223;}}
 L2223: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_303X)
    SstatusS = v_303X;
    goto L2075;}
  else {
    goto L2075;}}
 L2075: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_273X, port_263X));
    goto L2238;}
  else {
    goto L2238;}}
 L2238: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_263X, v_304X)
    SstatusS = v_304X;
    goto L2077;}
  else {
    goto L2077;}}
 L2077: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_263X, v_305X)
    SstatusS = v_305X;
    goto L2086;}
  else {
    goto L2086;}}
 L2086: {
  *((long *) (s48_ShpS)) = 1;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_263X, ((char *) (s48_ShpS)), (((s48_ShpS) + 4) - (s48_ShpS))));
    goto L2105;}
  else {
    goto L2105;}}
 L2105: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_263X, ((char *) (Snewspace_beginS)), ((s48_ShpS) - (Snewspace_beginS))));
    goto L2118;}
  else {
    goto L2118;}}
 L2118: {
  status_306X = SstatusS;
  abort_collection_return_tag = 1;
  goto abort_collection;
 abort_collection_return_1:
  return status_306X;}
 Hproc967264: {
  start_265X = merged_arg0K0;
  end_266X = merged_arg0K1;{
  type_307X = Sfinding_typeS;
  arg0K0 = start_265X;
  goto L4852;}
 L4852: {
  addr_308X = arg0K0;
  if ((addr_308X < end_266X)) {
    d_309X = *((long *) addr_308X);
    next_310X = addr_308X + (4 + (-4 & (3 + ((long)(((unsigned long)d_309X)>>8)))));
    if ((2 == (3 & d_309X))) {
      if ((type_307X == (31 & (((d_309X)>>2))))) {
        obj_311X = 3 + (((long) (addr_308X + 4)));
        type_312X = *((long *) (((char *) (-3 + obj_311X))));
        if ((3 == (3 & type_312X))) {
          if ((8 == (31 & ((((*((long *) ((((char *) (-3 + type_312X))) + -4))))>>2))))) {
            if ((3 == (3 & (*((long *) ((((char *) (-3 + type_312X))) + 4)))))) {
              if ((((s48_ShpS) + 16) < (s48_SlimitS))) {
                *((long *) (s48_ShpS)) = obj_311X;
                s48_ShpS = ((s48_ShpS) + 4);
                arg0K0 = next_310X;
                goto L4852;}
              else {
                Hproc9672640_return_value = 0;
                goto Hproc967264_return;}}
            else {
              arg0K0 = next_310X;
              goto L4852;}}
          else {
            arg0K0 = next_310X;
            goto L4852;}}
        else {
          arg0K0 = next_310X;
          goto L4852;}}
      else {
        arg0K0 = next_310X;
        goto L4852;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc9672640_return_value = 0;
      goto Hproc967264_return;}}
  else {
    Hproc9672640_return_value = 1;
    goto Hproc967264_return;}}
 Hproc967264_return:
  switch (Hproc967264_return_tag) {
  case 0: goto Hproc967264_return_0;
  case 1: goto Hproc967264_return_1;
  default: goto Hproc967264_return_2;
  }}

 abort_collection: {
{ temp_313X = s48_SlimitS;
  s48_SlimitS = (Soldspace_limitS);
  Soldspace_limitS = temp_313X;
  temp_314X = s48_ShpS;
  s48_ShpS = (Soldspace_hpS);
  Soldspace_hpS = temp_314X;
  temp_315X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = temp_315X;
  temp_316X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = temp_316X;
  arg0K0 = (Snewspace_beginS);
  goto L4085;}
 L4085: {
  addr_317X = arg0K0;
  if ((addr_317X < (s48_ShpS))) {
    d_318X = *((long *) addr_317X);
    if ((2 == (3 & d_318X))) {
      arg1K0 = d_318X;
      goto L4106;}
    else {
      h_319X = *((long *) ((((char *) (-3 + d_318X))) + -4));
      *((long *) addr_317X) = h_319X;
      arg1K0 = h_319X;
      goto L4106;}}
  else {
    goto abort_collection_return;}}
 L4106: {
  h_320X = arg1K0;
  arg0K0 = (addr_317X + (4 + (-4 & (3 + ((long)(((unsigned long)h_320X)>>8))))));
  goto L4085;}
 abort_collection_return:
  switch (abort_collection_return_tag) {
  case 0: goto abort_collection_return_0;
  default: goto abort_collection_return_1;
  }}

}
void s48_initializing_gc_root(void)
{
  long v_325X;
  long v_324X;
  long v_323X;
  long v_322X;
  long v_321X;
 {  if ((SinitializingPS)) {
    v_321X = s48_trace_value((Sstartup_procedureS));
    Sstartup_procedureS = v_321X;
    v_322X = s48_trace_value((SsymbolsS));
    SsymbolsS = v_322X;
    v_323X = s48_trace_value((Simported_bindingsS));
    Simported_bindingsS = v_323X;
    v_324X = s48_trace_value((Sexported_bindingsS));
    Sexported_bindingsS = v_324X;
    v_325X = s48_trace_value((Sresumer_recordsS));
    Sresumer_recordsS = v_325X;
    return;}
  else {
    return;}}
}void
s48_heap_init(void)
{
Spure_area_countS = 0;
Simpure_area_countS = 0;
Sfinding_typeS = 1;
Sgc_countS = 0;
SstatusS = NO_ERRORS;
SeofPS = 0;
SinitializingPS = 1;
Hthe_record_type9260 = 1;
}
