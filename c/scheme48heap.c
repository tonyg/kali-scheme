#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

static long copy_weak_pointer(long, char *, char **);
static long real_copy_object(long, char *, char **);
static void clean_weak_pointers(void);
void write_barrier(char *, long);
char * heap_pointer(void);
void set_heap_pointerB(char *);
long heap_size(void);
char * heap_limits(char **, char **, char **, char **);
long preallocate_space(long);
void register_static_areas(long, char **, long*, long, char **, long*);
char image_writing_okayP(void);
long gc_count(void);
void end_collection(void);
void initialize_heap(char *, long);
char availableP(long);
void begin_collection(void);
char * allocate_space(long, long, long);
long available(void);
long check_image_header(char*);
long trace_locationsB(char *, char *);
long find_all(long);
long trace_stob_contentsB(long);
long trace_value(long);
long find_all_records(long);
long read_image(long);
long write_image(long, FILE *, void(*)(void));
long do_gc(long, long*);
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
static long SstatusS;
static char SeofPS;
static FILE * Simage_portS;
static char * Sold_beginS;
static char * Sold_hpS;
static long Sstartup_procS;
static long Sgc_countS;
static char * Ssaved_hpS;
static char * Ssaved_limitS;
static char * Sweak_pointer_hpS;
static char * Sweak_pointer_limitS;
static long Hthe_record_type8040;
char * ShpS;
char * SlimitS;

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
    goto L1918;}
  else {
    if (((Sweak_pointer_hpS) < (Sweak_pointer_limitS))) {
      arg0K0 = frontier_2X;
      goto L1923;}
    else {
      goto L1918;}}}
 L1918: {
  old_4X = Sweak_pointer_hpS;
  new_frontier_5X = frontier_2X + 1024;
  Sweak_pointer_hpS = frontier_2X;
  Sweak_pointer_limitS = new_frontier_5X;
  *((long *) (Sweak_pointer_hpS)) = 261202;
  *((long *) ((Sweak_pointer_hpS) + 8)) = (((long) old_4X));
  arg0K0 = new_frontier_5X;
  goto L1923;}
 L1923: {
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
    if ((1074 == h_10X)) {
      descriptor_11X = *((long *) (((char *) (-3 + thing_8X))));
      if ((3 == (3 & descriptor_11X))) {
        a_12X = ((char *) (-3 + descriptor_11X));
        if ((a_12X < (Soldspace_beginS))) {
          goto L3073;}
        else {
          if ((a_12X < (Soldspace_endS))) {
            return copy_weak_pointer(thing_8X, frontier_9X, TT0);}
          else {
            goto L3073;}}}
      else {
        goto L3073;}}
    else {
      goto L3073;}}}
 L3073: {
  *((long *) frontier_9X) = h_10X;
  data_addr_13X = frontier_9X + 4;
  new_14X = 3 + (((long) data_addr_13X));
  *((long *) ((((char *) (-3 + thing_8X))) + -4)) = new_14X;
  memcpy((void *)data_addr_13X, (void *)(((char *) (-3 + thing_8X))),((long)(((unsigned long)h_10X)>>8)));
  *TT0 = (data_addr_13X + (-4 & (3 + ((long)(((unsigned long)h_10X)>>8)))));
  return new_14X;}
}
static void clean_weak_pointers(void)
{
  char * arg0K1;
  char * arg0K0;
  long arg1K0;
  long x2_24X;
  long h_23X;
  char * a_22X;
  long value_21X;
  char * scan_20X;
  char * next_19X;
  char * end_18X;
  char * start_17X;
  char * end_16X;
  char * x_15X;
 {  if (((Sweak_pointer_hpS) == NULL)) {
    return;}
  else {
    x_15X = Sweak_pointer_limitS;
    end_16X = Sweak_pointer_hpS;
    arg0K0 = (x_15X + -1024);
    arg0K1 = end_16X;
    goto L3730;}}
 L3730: {
  start_17X = arg0K0;
  end_18X = arg0K1;
  next_19X = ((char *) (*((long *) (start_17X + 8))));
  arg0K0 = start_17X;
  goto L2029;}
 L2029: {
  scan_20X = arg0K0;
  if ((scan_20X < end_18X)) {
    *((long *) scan_20X) = 1074;
    value_21X = *((long *) (scan_20X + 4));
    if ((3 == (3 & value_21X))) {
      a_22X = ((char *) (-3 + value_21X));
      if ((a_22X < (Soldspace_beginS))) {
        goto L2075;}
      else {
        if ((a_22X < (Soldspace_endS))) {
          if ((3 == (3 & value_21X))) {
            h_23X = *((long *) ((((char *) (-3 + value_21X))) + -4));
            if ((3 == (3 & h_23X))) {
              arg1K0 = h_23X;
              goto L2070;}
            else {
              arg1K0 = 1;
              goto L2070;}}
          else {
            goto L2075;}}
        else {
          goto L2075;}}}
    else {
      goto L2075;}}
  else {
    if ((next_19X == NULL)) {
      if ((end_16X < (Sweak_pointer_limitS))) {
        *((long *) end_16X) = (82 + ((((-4 & ((Sweak_pointer_limitS) - (end_16X + 4))))<<8)));
        return;}
      else {
        return;}}
    else {
      arg0K0 = (next_19X + -1024);
      arg0K1 = next_19X;
      goto L3730;}}}
 L2075: {
  arg0K0 = (scan_20X + 8);
  goto L2029;}
 L2070: {
  x2_24X = arg1K0;
  *((long *) (scan_20X + 4)) = x2_24X;
  goto L2075;}
}
void write_barrier(char * address_25X, long value_26X)
{

 {  return;}
}
char * heap_pointer(void)
{

 {  return (ShpS);}
}
void set_heap_pointerB(char * value_27X)
{

 {  ShpS = value_27X;
  return;}
}
long heap_size(void)
{

 {  return ((Snewspace_endS) - (Snewspace_beginS));}
}
char * heap_limits(char * *TT0, char * *TT1, char * *TT2, char * *TT3)
{

 {  *TT0 = (Snewspace_beginS);
  *TT1 = (Snewspace_endS);
  *TT2 = (Soldspace_beginS);
  *TT3 = (Soldspace_endS);
  return (ShpS);}
}
long preallocate_space(long cells_28X)
{

 {  return 0;}
}
void register_static_areas(long pure_count_29X, char * *pure_areas_30X, long *pure_sizes_31X, long impure_count_32X, char * *impure_areas_33X, long *impure_sizes_34X)
{

 {  Spure_area_countS = pure_count_29X;
  Spure_areasS = pure_areas_30X;
  Spure_sizesS = pure_sizes_31X;
  Simpure_area_countS = impure_count_32X;
  Simpure_areasS = impure_areas_33X;
  Simpure_sizesS = impure_sizes_34X;
  return;}
}
char image_writing_okayP(void)
{

 {  if ((0 == (Spure_area_countS))) {
    return (0 == (Simpure_area_countS));}
  else {
    return 0;}}
}
long gc_count(void)
{

 {  return (Sgc_countS);}
}
void end_collection(void)
{

 {  Sgc_countS = (1 + (Sgc_countS));
  return;}
}
void initialize_heap(char * start_35X, long size_36X)
{
  long semisize_38X;
  long cells_37X;
 {  cells_37X = size_36X / 2;
  semisize_38X = ((cells_37X)<<2);
  Snewspace_beginS = start_35X;
  Snewspace_endS = ((Snewspace_beginS) + semisize_38X);
  Soldspace_beginS = (Snewspace_endS);
  Soldspace_endS = ((Soldspace_beginS) + semisize_38X);
  ShpS = (Snewspace_beginS);
  SlimitS = (Snewspace_endS);
  return;}
}
char availableP(long cells_39X)
{

 {  return (((ShpS) + (((cells_39X)<<2))) < (SlimitS));}
}
void begin_collection(void)
{
  char * e_41X;
  char * b_40X;
 {  b_40X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_40X;
  e_41X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_41X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = NULL;
  return;}
}
char * allocate_space(long type_42X, long len_43X, long key_44X)
{
  char * new_45X;
 {  new_45X = ShpS;
  ShpS = ((ShpS) + (-4 & (3 + len_43X)));
  return new_45X;}
}
long available(void)
{

 {  return (((((SlimitS) - (ShpS)))>>2));}
}
long check_image_header(char *filename_46X)
{
  long arg1K0;
  long arg1K2;
  long arg1K1;
  char arg2K1;
  char arg2K0;
  long status_91X;
  char eofP_90X;
  long status_89X;
  long status_88X;
  long status_87X;
  long status_86X;
  long status_85X;
  char eofP_84X;
  char ch_83X;
  long v_82X;
  long status_81X;
  char eofP_80X;
  long thing_79X;
  long cells_78X;
  long status_77X;
  char eofP_76X;
  long thing_75X;
  long cells_74X;
  long status_73X;
  char eofP_72X;
  char thing_71X;
  long status_70X;
  char eofP_69X;
  long thing_68X;
  long old_bytes_per_cell_67X;
  long status_66X;
  char eofP_65X;
  long status_64X;
  char eofP_63X;
  char ch_62X;
  long i_61X;
  long status_60X;
  char eofP_59X;
  long thing_58X;
  char same_versionP_57X;
  long status_56X;
  char eofP_55X;
  long status_54X;
  char eofP_53X;
  char ch_52X;
  long status_51X;
  char eofP_50X;
  char ch_49X;
  long status_48X;
  FILE * port_47X;
 {  port_47X = ps_open_input_file(filename_46X, &status_48X);
  if ((status_48X == NO_ERRORS)) {
    SstatusS = NO_ERRORS;
    SeofPS = 0;
    if ((SeofPS)) {
      goto L2250;}
    else {
      goto L2823;}}
  else {
    ps_write_string("Can't open heap image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    ps_write_string((ps_error_string(status_48X)), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2250: {
  if ((SeofPS)) {
    goto L2283;}
  else {
    goto L2787;}}
 L2823: {
  PS_READ_CHAR(port_47X, ch_49X, eofP_50X, status_51X)
  if (eofP_50X) {
    arg2K0 = eofP_50X;
    arg1K1 = status_51X;
    goto L2228;}
  else {
    if ((status_51X == NO_ERRORS)) {
      if ((12 == ch_49X)) {
        arg2K0 = 0;
        arg1K1 = status_51X;
        goto L2228;}
      else {
        goto L2823;}}
    else {
      arg2K0 = eofP_50X;
      arg1K1 = status_51X;
      goto L2228;}}}
 L2283: {
  if ((SeofPS)) {
    arg2K0 = 0;
    goto L2316;}
  else {
    arg1K0 = 0;
    goto L2763;}}
 L2787: {
  PS_READ_CHAR(port_47X, ch_52X, eofP_53X, status_54X)
  if (eofP_53X) {
    arg2K0 = eofP_53X;
    arg1K1 = status_54X;
    goto L2261;}
  else {
    if ((status_54X == NO_ERRORS)) {
      if ((10 == ch_52X)) {
        arg2K0 = 0;
        arg1K1 = status_54X;
        goto L2261;}
      else {
        goto L2787;}}
    else {
      arg2K0 = eofP_53X;
      arg1K1 = status_54X;
      goto L2261;}}}
 L2228: {
  eofP_55X = arg2K0;
  status_56X = arg1K1;
  if (eofP_55X) {
    SeofPS = 1;
    goto L2250;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2250;}
    else {
      SeofPS = 1;
      SstatusS = status_56X;
      goto L2250;}}}
 L2316: {
  same_versionP_57X = arg2K0;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2351;}
  else {
    PS_READ_INTEGER(port_47X, thing_58X, eofP_59X, status_60X)
    if (eofP_59X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2351;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_58X;
        goto L2351;}
      else {
        SeofPS = 1;
        SstatusS = status_60X;
        arg1K0 = -1;
        goto L2351;}}}}
 L2763: {
  i_61X = arg1K0;
  PS_READ_CHAR(port_47X, ch_62X, eofP_63X, status_64X)
  if (eofP_63X) {
    arg2K0 = 0;
    arg2K1 = eofP_63X;
    arg1K2 = status_64X;
    goto L2294;}
  else {
    if ((status_64X == NO_ERRORS)) {
      if ((i_61X == (strlen((char *) "Kali 19")))) {
        arg2K0 = (10 == ch_62X);
        arg2K1 = 0;
        arg1K2 = status_64X;
        goto L2294;}
      else {
        if ((ch_62X == (*("Kali 19" + i_61X)))) {
          arg1K0 = (1 + i_61X);
          goto L2763;}
        else {
          arg2K0 = 0;
          arg2K1 = 0;
          arg1K2 = status_64X;
          goto L2294;}}}
    else {
      arg2K0 = 0;
      arg2K1 = eofP_63X;
      arg1K2 = status_64X;
      goto L2294;}}}
 L2261: {
  eofP_65X = arg2K0;
  status_66X = arg1K1;
  if (eofP_65X) {
    SeofPS = 1;
    goto L2283;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2283;}
    else {
      SeofPS = 1;
      SstatusS = status_66X;
      goto L2283;}}}
 L2351: {
  old_bytes_per_cell_67X = arg1K0;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2386;}
  else {
    PS_READ_INTEGER(port_47X, thing_68X, eofP_69X, status_70X)
    if (eofP_69X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2386;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_68X;
        goto L2386;}
      else {
        SeofPS = 1;
        SstatusS = status_70X;
        arg1K0 = -1;
        goto L2386;}}}}
 L2294: {
  thing_71X = arg2K0;
  eofP_72X = arg2K1;
  status_73X = arg1K2;
  if (eofP_72X) {
    SeofPS = 1;
    arg2K0 = 0;
    goto L2316;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      arg2K0 = thing_71X;
      goto L2316;}
    else {
      SeofPS = 1;
      SstatusS = status_73X;
      arg2K0 = 0;
      goto L2316;}}}
 L2386: {
  cells_74X = arg1K0;
  Sold_beginS = (((char *) (((cells_74X)<<2))));
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2424;}
  else {
    PS_READ_INTEGER(port_47X, thing_75X, eofP_76X, status_77X)
    if (eofP_76X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2424;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_75X;
        goto L2424;}
      else {
        SeofPS = 1;
        SstatusS = status_77X;
        arg1K0 = -1;
        goto L2424;}}}}
 L2424: {
  cells_78X = arg1K0;
  Sold_hpS = (((char *) (((cells_78X)<<2))));
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2462;}
  else {
    PS_READ_INTEGER(port_47X, thing_79X, eofP_80X, status_81X)
    if (eofP_80X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2462;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_79X;
        goto L2462;}
      else {
        SeofPS = 1;
        SstatusS = status_81X;
        arg1K0 = -1;
        goto L2462;}}}}
 L2462: {
  v_82X = arg1K0;
  Sstartup_procS = v_82X;
  Simage_portS = port_47X;
  if ((SeofPS)) {
    goto L2501;}
  else {
    goto L2710;}}
 L2501: {
  if (((SstatusS) == NO_ERRORS)) {
    if ((SeofPS)) {
      SstatusS = EDOM;
      ps_write_string("Premature EOF when reading image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L2582;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L2582;}}
    else {
      if (same_versionP_57X) {
        if ((4 == old_bytes_per_cell_67X)) {
          return ((Sold_hpS) - (Sold_beginS));}
        else {
          ps_write_string("Incompatible bytes-per-cell in image", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L2642;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L2642;}}}
      else {
        ps_write_string("Format of image is incompatible with this version of system", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2612;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2612;}}}}
  else {
    ps_write_string("Error reading from image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2557;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2557;}}}
 L2710: {
  PS_READ_CHAR(port_47X, ch_83X, eofP_84X, status_85X)
  if (eofP_84X) {
    arg2K0 = eofP_84X;
    arg1K1 = status_85X;
    goto L2479;}
  else {
    if ((status_85X == NO_ERRORS)) {
      if ((12 == ch_83X)) {
        arg2K0 = 0;
        arg1K1 = status_85X;
        goto L2479;}
      else {
        goto L2710;}}
    else {
      arg2K0 = eofP_84X;
      arg1K1 = status_85X;
      goto L2479;}}}
 L2582: {
  status_86X = ps_close(port_47X);
  if ((status_86X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2642: {
  status_87X = ps_close(port_47X);
  if ((status_87X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2612: {
  status_88X = ps_close(port_47X);
  if ((status_88X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2557: {
  status_89X = ps_close(port_47X);
  if ((status_89X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2479: {
  eofP_90X = arg2K0;
  status_91X = arg1K1;
  if (eofP_90X) {
    SeofPS = 1;
    goto L2501;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2501;}
    else {
      SeofPS = 1;
      SstatusS = status_91X;
      goto L2501;}}}
}
long trace_locationsB(char * start_92X, char * end_93X)
{
  char * arg0K0;
  char * arg0K1;
  long arg1K0;
  long new_107X;
  char * data_addr_106X;
  char * frontier_105X;
  long new_thing_104X;
  char * frontier_103X;
  long new_thing_102X;
  char * a_101X;
  long descriptor_100X;
  long h_99X;
  char * a_98X;
  char * next_97X;
  long thing_96X;
  char * frontier_95X;
  char * addr_94X;
 {  arg0K0 = start_92X;
  arg0K1 = (ShpS);
  goto L3822;}
 L3822: {
  addr_94X = arg0K0;
  frontier_95X = arg0K1;
  if ((addr_94X < end_93X)) {
    thing_96X = *((long *) addr_94X);
    next_97X = addr_94X + 4;
    if ((2 == (3 & thing_96X))) {
      if (((31 & (((thing_96X)>>2))) < 19)) {
        goto L3836;}
      else {
        arg0K0 = (next_97X + (-4 & (3 + ((long)(((unsigned long)thing_96X)>>8)))));
        arg0K1 = frontier_95X;
        goto L3822;}}
    else {
      goto L3836;}}
  else {
    ShpS = frontier_95X;
    return 0;}}
 L3836: {
  if ((3 == (3 & thing_96X))) {
    a_98X = ((char *) (-3 + thing_96X));
    if ((a_98X < (Soldspace_beginS))) {
      arg0K0 = next_97X;
      arg0K1 = frontier_95X;
      goto L3822;}
    else {
      if ((a_98X < (Soldspace_endS))) {
        h_99X = *((long *) ((((char *) (-3 + thing_96X))) + -4));
        if ((3 == (3 & h_99X))) {
          arg1K0 = h_99X;
          arg0K1 = frontier_95X;
          goto L3843;}
        else {
          if ((1074 == h_99X)) {
            descriptor_100X = *((long *) (((char *) (-3 + thing_96X))));
            if ((3 == (3 & descriptor_100X))) {
              a_101X = ((char *) (-3 + descriptor_100X));
              if ((a_101X < (Soldspace_beginS))) {
                goto L5360;}
              else {
                if ((a_101X < (Soldspace_endS))) {
                  new_thing_102X = copy_weak_pointer(thing_96X, frontier_95X, &frontier_103X);
                  arg1K0 = new_thing_102X;
                  arg0K1 = frontier_103X;
                  goto L3843;}
                else {
                  goto L5360;}}}
            else {
              goto L5360;}}
          else {
            goto L5360;}}}
      else {
        arg0K0 = next_97X;
        arg0K1 = frontier_95X;
        goto L3822;}}}
  else {
    arg0K0 = next_97X;
    arg0K1 = frontier_95X;
    goto L3822;}}
 L3843: {
  new_thing_104X = arg1K0;
  frontier_105X = arg0K1;
  *((long *) addr_94X) = new_thing_104X;
  arg0K0 = next_97X;
  arg0K1 = frontier_105X;
  goto L3822;}
 L5360: {
  *((long *) frontier_95X) = h_99X;
  data_addr_106X = frontier_95X + 4;
  new_107X = 3 + (((long) data_addr_106X));
  *((long *) ((((char *) (-3 + thing_96X))) + -4)) = new_107X;
  memcpy((void *)data_addr_106X, (void *)(((char *) (-3 + thing_96X))),((long)(((unsigned long)h_99X)>>8)));
  arg1K0 = new_107X;
  arg0K1 = (data_addr_106X + (-4 & (3 + ((long)(((unsigned long)h_99X)>>8)))));
  goto L3843;}
}
long find_all(long type_108X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc801109_return_tag;
  char Hproc8011090_return_value;
  char * start_110X;
  char * end_111X;
  long d_126X;
  char * addr_125X;
  long type_124X;
  char v_123X;
  long i_122X;
  long count_121X;
  long *sizes_120X;
  char * *areas_119X;
  char v_118X;
  long i_117X;
  long count_116X;
  long *sizes_115X;
  char * *areas_114X;
  char v_113X;
  char * start_hp_112X;
 {  Sfinding_typeS = type_108X;
  start_hp_112X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = ((ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_112X;
  Hproc801109_return_tag = 0;
  goto Hproc801109;
 Hproc801109_return_0:
  v_113X = Hproc8011090_return_value;
  if (v_113X) {
    areas_114X = Simpure_areasS;
    sizes_115X = Simpure_sizesS;
    count_116X = Simpure_area_countS;
    arg1K0 = 0;
    goto L4041;}
  else {
    goto L4001;}}
 L4041: {
  i_117X = arg1K0;
  if ((i_117X < count_116X)) {
    merged_arg0K0 = (*(areas_114X + i_117X));
    merged_arg0K1 = ((*(areas_114X + i_117X)) + (*(sizes_115X + i_117X)));
    Hproc801109_return_tag = 1;
    goto Hproc801109;
   Hproc801109_return_1:
    v_118X = Hproc8011090_return_value;
    if (v_118X) {
      arg1K0 = (1 + i_117X);
      goto L4041;}
    else {
      goto L4001;}}
  else {
    areas_119X = Spure_areasS;
    sizes_120X = Spure_sizesS;
    count_121X = Spure_area_countS;
    arg1K0 = 0;
    goto L4060;}}
 L4001: {
  ShpS = start_hp_112X;
  return 1;}
 L4060: {
  i_122X = arg1K0;
  if ((i_122X < count_121X)) {
    merged_arg0K0 = (*(areas_119X + i_122X));
    merged_arg0K1 = ((*(areas_119X + i_122X)) + (*(sizes_120X + i_122X)));
    Hproc801109_return_tag = 2;
    goto Hproc801109;
   Hproc801109_return_2:
    v_123X = Hproc8011090_return_value;
    if (v_123X) {
      arg1K0 = (1 + i_122X);
      goto L4060;}
    else {
      goto L4001;}}
  else {
    *((long *) start_hp_112X) = (10 + (((((ShpS) - (start_hp_112X + 4)))<<8)));
    return (3 + (((long) (start_hp_112X + 4))));}}
 Hproc801109: {
  start_110X = merged_arg0K0;
  end_111X = merged_arg0K1;{
  type_124X = Sfinding_typeS;
  arg0K0 = start_110X;
  goto L3189;}
 L3189: {
  addr_125X = arg0K0;
  if ((addr_125X < end_111X)) {
    d_126X = *((long *) addr_125X);
    if ((2 == (3 & d_126X))) {
      if ((type_124X == (31 & (((d_126X)>>2))))) {
        if ((((ShpS) + 16) < (SlimitS))) {
          *((long *) (ShpS)) = (3 + (((long) (addr_125X + 4))));
          ShpS = ((ShpS) + 4);
          goto L3221;}
        else {
          Hproc8011090_return_value = 0;
          goto Hproc801109_return;}}
      else {
        goto L3221;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc8011090_return_value = 0;
      goto Hproc801109_return;}}
  else {
    Hproc8011090_return_value = 1;
    goto Hproc801109_return;}}
 L3221: {
  arg0K0 = (addr_125X + (4 + (-4 & (3 + ((long)(((unsigned long)d_126X)>>8))))));
  goto L3189;}
 Hproc801109_return:
  switch (Hproc801109_return_tag) {
  case 0: goto Hproc801109_return_0;
  case 1: goto Hproc801109_return_1;
  default: goto Hproc801109_return_2;
  }}

}
long trace_stob_contentsB(long stob_127X)
{
  char * start_129X;
  long h_128X;
 {  h_128X = *((long *) ((((char *) (-3 + stob_127X))) + -4));
  start_129X = ((char *) (-3 + stob_127X));
  return trace_locationsB(start_129X, (start_129X + (-4 & (3 + ((long)(((unsigned long)h_128X)>>8))))));}
}
long trace_value(long stob_130X)
{
  char * new_hp_133X;
  long new_thing_132X;
  char * a_131X;
 {  if ((3 == (3 & stob_130X))) {
    a_131X = ((char *) (-3 + stob_130X));
    if ((a_131X < (Soldspace_beginS))) {
      return stob_130X;}
    else {
      if ((a_131X < (Soldspace_endS))) {
        new_thing_132X = real_copy_object(stob_130X, (ShpS), &new_hp_133X);
        ShpS = new_hp_133X;
        return new_thing_132X;}
      else {
        return stob_130X;}}}
  else {
    return stob_130X;}}
}
long find_all_records(long record_type_134X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc807135_return_tag;
  char Hproc8071350_return_value;
  char * start_136X;
  char * end_137X;
  long record_153X;
  long d_152X;
  char * addr_151X;
  long type_150X;
  char v_149X;
  long i_148X;
  long count_147X;
  long *sizes_146X;
  char * *areas_145X;
  char v_144X;
  long i_143X;
  long count_142X;
  long *sizes_141X;
  char * *areas_140X;
  char v_139X;
  char * start_hp_138X;
 {  Hthe_record_type8040 = record_type_134X;
  Sfinding_typeS = 8;
  start_hp_138X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = ((ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_138X;
  Hproc807135_return_tag = 0;
  goto Hproc807135;
 Hproc807135_return_0:
  v_139X = Hproc8071350_return_value;
  if (v_139X) {
    areas_140X = Simpure_areasS;
    sizes_141X = Simpure_sizesS;
    count_142X = Simpure_area_countS;
    arg1K0 = 0;
    goto L4188;}
  else {
    goto L4148;}}
 L4188: {
  i_143X = arg1K0;
  if ((i_143X < count_142X)) {
    merged_arg0K0 = (*(areas_140X + i_143X));
    merged_arg0K1 = ((*(areas_140X + i_143X)) + (*(sizes_141X + i_143X)));
    Hproc807135_return_tag = 1;
    goto Hproc807135;
   Hproc807135_return_1:
    v_144X = Hproc8071350_return_value;
    if (v_144X) {
      arg1K0 = (1 + i_143X);
      goto L4188;}
    else {
      goto L4148;}}
  else {
    areas_145X = Spure_areasS;
    sizes_146X = Spure_sizesS;
    count_147X = Spure_area_countS;
    arg1K0 = 0;
    goto L4207;}}
 L4148: {
  ShpS = start_hp_138X;
  return 1;}
 L4207: {
  i_148X = arg1K0;
  if ((i_148X < count_147X)) {
    merged_arg0K0 = (*(areas_145X + i_148X));
    merged_arg0K1 = ((*(areas_145X + i_148X)) + (*(sizes_146X + i_148X)));
    Hproc807135_return_tag = 2;
    goto Hproc807135;
   Hproc807135_return_2:
    v_149X = Hproc8071350_return_value;
    if (v_149X) {
      arg1K0 = (1 + i_148X);
      goto L4207;}
    else {
      goto L4148;}}
  else {
    *((long *) start_hp_138X) = (10 + (((((ShpS) - (start_hp_138X + 4)))<<8)));
    return (3 + (((long) (start_hp_138X + 4))));}}
 Hproc807135: {
  start_136X = merged_arg0K0;
  end_137X = merged_arg0K1;{
  type_150X = Sfinding_typeS;
  arg0K0 = start_136X;
  goto L3336;}
 L3336: {
  addr_151X = arg0K0;
  if ((addr_151X < end_137X)) {
    d_152X = *((long *) addr_151X);
    if ((2 == (3 & d_152X))) {
      if ((type_150X == (31 & (((d_152X)>>2))))) {
        record_153X = 3 + (((long) (addr_151X + 4)));
        if (((Hthe_record_type8040) == (*((long *) (((char *) (-3 + record_153X))))))) {
          if ((((ShpS) + 16) < (SlimitS))) {
            *((long *) (ShpS)) = record_153X;
            ShpS = ((ShpS) + 4);
            goto L3368;}
          else {
            Hproc8071350_return_value = 0;
            goto Hproc807135_return;}}
        else {
          goto L3368;}}
      else {
        goto L3368;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc8071350_return_value = 0;
      goto Hproc807135_return;}}
  else {
    Hproc8071350_return_value = 1;
    goto Hproc807135_return;}}
 L3368: {
  arg0K0 = (addr_151X + (4 + (-4 & (3 + ((long)(((unsigned long)d_152X)>>8))))));
  goto L3336;}
 Hproc807135_return:
  switch (Hproc807135_return_tag) {
  case 0: goto Hproc807135_return_0;
  case 1: goto Hproc807135_return_1;
  default: goto Hproc807135_return_2;
  }}

}
long read_image(long startup_space_154X)
{
  FILE * arg4K0;
  char *arg3K1;
  char arg2K1;
  char arg2K0;
  char * arg0K0;
  long arg1K2;
  long arg1K0;
  long d_196X;
  long descriptor_195X;
  char * ptr_194X;
  long descriptor_193X;
  long status_192X;
  long status_191X;
  long status_190X;
  long status_189X;
  char * next_188X;
  long value_187X;
  long x_186X;
  long x_185X;
  char * ptr_184X;
  long status_183X;
  long status_182X;
  long status_181X;
  char eofP_180X;
  char v_179X;
  char *string_178X;
  char okayP_177X;
  long status_176X;
  long status_175X;
  long status_174X;
  char eofP_173X;
  long got_172X;
  long need_171X;
  char * start_170X;
  char * new_limit_169X;
  char * new_hp_168X;
  long delta_167X;
  long startup_space_166X;
  char reverseP_165X;
  FILE * port_164X;
  long x_163X;
  long x_162X;
  char * addr_161X;
  char *string_160X;
  char okayP_159X;
  long status_158X;
  char eofP_157X;
  long got_156X;
  FILE * port_155X;
 {  port_155X = Simage_portS;
  got_156X = ps_read_block(port_155X, ((char *) (ShpS)), 4, &eofP_157X, &status_158X);
  if ((status_158X == NO_ERRORS)) {
    if (eofP_157X) {
      arg2K0 = 0;
      arg3K1 = "Premature EOF when reading image file";
      goto L4943;}
    else {
      if ((got_156X < 4)) {
        arg2K0 = 0;
        arg3K1 = "Read returned too few bytes";
        goto L4943;}
      else {
        arg2K0 = 1;
        arg3K1 = "";
        goto L4943;}}}
  else {
    SstatusS = status_158X;
    arg2K0 = 0;
    arg3K1 = "Error reading from image file";
    goto L4943;}}
 L4943: {
  okayP_159X = arg2K0;
  string_160X = arg3K1;
  if (okayP_159X) {
    if ((1 == (*((long *) (ShpS))))) {
      arg4K0 = port_155X;
      arg2K1 = 0;
      arg1K2 = startup_space_154X;
      goto L4264;}
    else {
      addr_161X = ShpS;
      x_162X = *((unsigned char *) addr_161X);
      *((unsigned char *) addr_161X) = (*((unsigned char *) (addr_161X + 3)));
      *((unsigned char *) (addr_161X + 3)) = x_162X;
      x_163X = *((unsigned char *) (addr_161X + 1));
      *((unsigned char *) (addr_161X + 1)) = (*((unsigned char *) (addr_161X + 2)));
      *((unsigned char *) (addr_161X + 2)) = x_163X;
      if ((1 == (*((long *) (ShpS))))) {
        arg4K0 = port_155X;
        arg2K1 = 1;
        arg1K2 = startup_space_154X;
        goto L4264;}
      else {
        ps_write_string("Unable to correct byte order", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L5039;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L5039;}}}}
  else {
    ps_write_string(string_160X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4993;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4993;}}}
 L4264: {
  port_164X = arg4K0;
  reverseP_165X = arg2K1;
  startup_space_166X = arg1K2;
  delta_167X = (ShpS) - (Sold_beginS);
  new_hp_168X = (Sold_hpS) + delta_167X;
  new_limit_169X = Snewspace_endS;
  start_170X = ShpS;
  if (((new_hp_168X + startup_space_166X) < new_limit_169X)) {
    need_171X = (Sold_hpS) - (Sold_beginS);
    got_172X = ps_read_block(port_164X, ((char *) (ShpS)), need_171X, &eofP_173X, &status_174X);
    if ((status_174X == NO_ERRORS)) {
      if (eofP_173X) {
        arg2K0 = 0;
        arg3K1 = "Premature EOF when reading image file";
        goto L4280;}
      else {
        if ((got_172X < need_171X)) {
          arg2K0 = 0;
          arg3K1 = "Read returned too few bytes";
          goto L4280;}
        else {
          arg2K0 = 1;
          arg3K1 = "";
          goto L4280;}}}
    else {
      SstatusS = status_174X;
      arg2K0 = 0;
      arg3K1 = "Error reading from image file";
      goto L4280;}}
  else {
    ps_write_string("Heap not big enough to restore this image", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4346;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4346;}}}
 L5039: {
  status_175X = ps_close(port_155X);
  if ((status_175X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4993: {
  status_176X = ps_close(port_155X);
  if ((status_176X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4280: {
  okayP_177X = arg2K0;
  string_178X = arg3K1;
  PS_READ_CHAR(port_164X, v_179X, eofP_180X, status_181X)
  if (okayP_177X) {
    if ((status_181X == NO_ERRORS)) {
      if (eofP_180X) {
        status_182X = ps_close(port_164X);
        if ((status_182X == NO_ERRORS)) {
          if (reverseP_165X) {
            ps_write_string("Correcting byte order of resumed image.", (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = start_170X;
            goto L3493;}
          else {
            goto L4318;}}
        else {
          ps_write_string("Error closing image file", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L4481;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L4481;}}}
      else {
        ps_write_string("Image file has extraneous data after image", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L4451;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L4451;}}}
    else {
      ps_write_string("Error reading from image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L4421;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L4421;}}}
  else {
    ps_write_string(string_178X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4391;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4391;}}}
 L4346: {
  status_183X = ps_close(port_164X);
  if ((status_183X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3493: {
  ptr_184X = arg0K0;
  if ((ptr_184X < new_hp_168X)) {
    x_185X = *((unsigned char *) ptr_184X);
    *((unsigned char *) ptr_184X) = (*((unsigned char *) (ptr_184X + 3)));
    *((unsigned char *) (ptr_184X + 3)) = x_185X;
    x_186X = *((unsigned char *) (ptr_184X + 1));
    *((unsigned char *) (ptr_184X + 1)) = (*((unsigned char *) (ptr_184X + 2)));
    *((unsigned char *) (ptr_184X + 2)) = x_186X;
    value_187X = *((long *) ptr_184X);
    next_188X = ptr_184X + 4;
    if ((2 == (3 & value_187X))) {
      if (((31 & (((value_187X)>>2))) < 19)) {
        arg0K0 = next_188X;
        goto L3493;}
      else {
        arg0K0 = (next_188X + (-4 & (3 + ((long)(((unsigned long)value_187X)>>8)))));
        goto L3493;}}
    else {
      arg0K0 = next_188X;
      goto L3493;}}
  else {
    goto L4318;}}
 L4318: {
  if ((0 == delta_167X)) {
    goto L4328;}
  else {
    arg0K0 = start_170X;
    goto L3592;}}
 L4481: {
  status_189X = ps_close(port_164X);
  if ((status_189X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4451: {
  status_190X = ps_close(port_164X);
  if ((status_190X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4421: {
  status_191X = ps_close(port_164X);
  if ((status_191X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4391: {
  status_192X = ps_close(port_164X);
  if ((status_192X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4328: {
  ShpS = new_hp_168X;
  descriptor_193X = Sstartup_procS;
  if ((3 == (3 & descriptor_193X))) {
    return (3 + (((long) ((((char *) (-3 + descriptor_193X))) + delta_167X))));}
  else {
    return descriptor_193X;}}
 L3592: {
  ptr_194X = arg0K0;
  if ((ptr_194X < new_hp_168X)) {
    descriptor_195X = *((long *) ptr_194X);
    if ((3 == (3 & descriptor_195X))) {
      arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_195X))) + delta_167X))));
      goto L3597;}
    else {
      arg1K0 = descriptor_195X;
      goto L3597;}}
  else {
    goto L4328;}}
 L3597: {
  d_196X = arg1K0;
  *((long *) ptr_194X) = d_196X;
  if ((2 == (3 & d_196X))) {
    if (((31 & (((d_196X)>>2))) < 19)) {
      goto L3611;}
    else {
      arg0K0 = (ptr_194X + (4 + (-4 & (3 + ((long)(((unsigned long)d_196X)>>8))))));
      goto L3592;}}
  else {
    goto L3611;}}
 L3611: {
  arg0K0 = (ptr_194X + 4);
  goto L3592;}
}
long write_image(long resume_proc_197X, FILE * port_198X, void (*mark_traced_channels_closedB_199X)(void))
{
  char * arg0K0;
  long arg1K0;
  long h_226X;
  long h_225X;
  long d_224X;
  char * addr_223X;
  char * e_222X;
  char * b_221X;
  long status_220X;
  long v_219X;
  long v_218X;
  long v_217X;
  long cells_216X;
  long v_215X;
  long cells_214X;
  long v_213X;
  long v_212X;
  long v_211X;
  long v_210X;
  long v_209X;
  char * end_208X;
  char * start_207X;
  char * start_206X;
  long resume_proc_205X;
  char * new_hp_204X;
  long new_thing_203X;
  char * a_202X;
  char * e_201X;
  char * b_200X;
 {  b_200X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_200X;
  e_201X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_201X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = NULL;
  if ((3 == (3 & resume_proc_197X))) {
    a_202X = ((char *) (-3 + resume_proc_197X));
    if ((a_202X < (Soldspace_beginS))) {
      arg1K0 = resume_proc_197X;
      goto L5080;}
    else {
      if ((a_202X < (Soldspace_endS))) {
        new_thing_203X = real_copy_object(resume_proc_197X, (ShpS), &new_hp_204X);
        ShpS = new_hp_204X;
        arg1K0 = new_thing_203X;
        goto L5080;}
      else {
        arg1K0 = resume_proc_197X;
        goto L5080;}}}
  else {
    arg1K0 = resume_proc_197X;
    goto L5080;}}
 L5080: {
  resume_proc_205X = arg1K0;
  start_206X = Snewspace_beginS;
  arg0K0 = start_206X;
  goto L5132;}
 L5132: {
  start_207X = arg0K0;
  end_208X = ShpS;
  trace_locationsB(start_207X, end_208X);
  if (((ShpS) < (SlimitS))) {
    if ((end_208X < (ShpS))) {
      arg0K0 = end_208X;
      goto L5132;}
    else {
      goto L5084;}}
  else {
    ps_error("GC error: ran out of space in new heap", 0);
    goto L5084;}}
 L5084: {
  clean_weak_pointers();
  (*mark_traced_channels_closedB_199X)();
  SstatusS = NO_ERRORS;
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_209X)
    SstatusS = v_209X;
    goto L1679;}
  else {
    goto L1679;}}
 L1679: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_198X, v_210X)
    SstatusS = v_210X;
    goto L1688;}
  else {
    goto L1688;}}
 L1688: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_211X)
    SstatusS = v_211X;
    goto L1697;}
  else {
    goto L1697;}}
 L1697: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string("Kali 19", port_198X));
    goto L1704;}
  else {
    goto L1704;}}
 L1704: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_212X)
    SstatusS = v_212X;
    goto L1713;}
  else {
    goto L1713;}}
 L1713: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, port_198X));
    goto L1757;}
  else {
    goto L1757;}}
 L1757: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_213X)
    SstatusS = v_213X;
    goto L1715;}
  else {
    goto L1715;}}
 L1715: {
  cells_214X = ((long) (Snewspace_beginS));
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_214X)>>2)), port_198X));
    goto L1775;}
  else {
    goto L1775;}}
 L1775: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_215X)
    SstatusS = v_215X;
    goto L1719;}
  else {
    goto L1719;}}
 L1719: {
  cells_216X = ((long) (ShpS));
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_216X)>>2)), port_198X));
    goto L1793;}
  else {
    goto L1793;}}
 L1793: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_217X)
    SstatusS = v_217X;
    goto L1723;}
  else {
    goto L1723;}}
 L1723: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_205X, port_198X));
    goto L1808;}
  else {
    goto L1808;}}
 L1808: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_218X)
    SstatusS = v_218X;
    goto L1725;}
  else {
    goto L1725;}}
 L1725: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_198X, v_219X)
    SstatusS = v_219X;
    goto L1734;}
  else {
    goto L1734;}}
 L1734: {
  *((long *) (ShpS)) = 1;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_198X, ((char *) (ShpS)), (((ShpS) + 4) - (ShpS))));
    goto L1745;}
  else {
    goto L1745;}}
 L1745: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_198X, ((char *) (Snewspace_beginS)), ((ShpS) - (Snewspace_beginS))));
    goto L1752;}
  else {
    goto L1752;}}
 L1752: {
  status_220X = SstatusS;
  b_221X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_221X;
  e_222X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_222X;
  SlimitS = (Ssaved_limitS);
  ShpS = (Ssaved_hpS);
  arg0K0 = (Snewspace_beginS);
  goto L2981;}
 L2981: {
  addr_223X = arg0K0;
  if ((addr_223X < (ShpS))) {
    d_224X = *((long *) addr_223X);
    if ((2 == (3 & d_224X))) {
      arg1K0 = d_224X;
      goto L3000;}
    else {
      h_225X = *((long *) ((((char *) (-3 + d_224X))) + -4));
      *((long *) addr_223X) = h_225X;
      arg1K0 = h_225X;
      goto L3000;}}
  else {
    return status_220X;}}
 L3000: {
  h_226X = arg1K0;
  arg0K0 = (addr_223X + (4 + (-4 & (3 + ((long)(((unsigned long)h_226X)>>8))))));
  goto L2981;}
}
long do_gc(long finalizer_alist_227X, long *TT0)
{
  char * arg0K0;
  char arg2K0;
  long arg1K2;
  long arg1K1;
  long arg1K0;
  long thing_259X;
  char tracedP_258X;
  char * new_hp_257X;
  long new_thing_256X;
  char * a_255X;
  long next_254X;
  long thing_253X;
  char * new_hp_252X;
  long new_thing_251X;
  char * new_hp_250X;
  long new_thing_249X;
  long goners_248X;
  long okay_247X;
  long alist_246X;
  long value_245X;
  char * new_hp_244X;
  long new_thing_243X;
  char * a_242X;
  long stob_241X;
  char * end_240X;
  char * start_239X;
  char * start_238X;
  long h_237X;
  long stob_236X;
  long descriptor_235X;
  long pair_234X;
  char * start_233X;
  long alist_232X;
  long i_231X;
  long count_230X;
  long *sizes_229X;
  char * *areas_228X;
 {  areas_228X = Simpure_areasS;
  sizes_229X = Simpure_sizesS;
  count_230X = Simpure_area_countS;
  arg1K0 = 0;
  goto L5304;}
 L5304: {
  i_231X = arg1K0;
  if ((i_231X < count_230X)) {
    trace_locationsB((*(areas_228X + i_231X)), ((*(areas_228X + i_231X)) + (*(sizes_229X + i_231X))));
    arg1K0 = (1 + i_231X);
    goto L5304;}
  else {
    arg1K0 = finalizer_alist_227X;
    goto L5155;}}
 L5155: {
  alist_232X = arg1K0;
  if ((25 == alist_232X)) {
    start_233X = Snewspace_beginS;
    arg0K0 = start_233X;
    goto L5319;}
  else {
    pair_234X = *((long *) (((char *) (-3 + alist_232X))));
    descriptor_235X = *((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + pair_234X)))))))) + -4));
    if ((2 == (3 & descriptor_235X))) {
      stob_236X = *((long *) (((char *) (-3 + pair_234X))));
      h_237X = *((long *) ((((char *) (-3 + stob_236X))) + -4));
      start_238X = ((char *) (-3 + stob_236X));
      trace_locationsB(start_238X, (start_238X + (-4 & (3 + ((long)(((unsigned long)h_237X)>>8))))));
      goto L5180;}
    else {
      goto L5180;}}}
 L5319: {
  start_239X = arg0K0;
  end_240X = ShpS;
  trace_locationsB(start_239X, end_240X);
  if (((ShpS) < (SlimitS))) {
    if ((end_240X < (ShpS))) {
      arg0K0 = end_240X;
      goto L5319;}
    else {
      goto L5290;}}
  else {
    ps_error("GC error: ran out of space in new heap", 0);
    goto L5290;}}
 L5180: {
  stob_241X = *((long *) ((((char *) (-3 + pair_234X))) + 4));
  if ((3 == (3 & stob_241X))) {
    a_242X = ((char *) (-3 + stob_241X));
    if ((a_242X < (Soldspace_beginS))) {
      arg1K0 = stob_241X;
      goto L5184;}
    else {
      if ((a_242X < (Soldspace_endS))) {
        new_thing_243X = real_copy_object(stob_241X, (ShpS), &new_hp_244X);
        ShpS = new_hp_244X;
        arg1K0 = new_thing_243X;
        goto L5184;}
      else {
        arg1K0 = stob_241X;
        goto L5184;}}}
  else {
    arg1K0 = stob_241X;
    goto L5184;}}
 L5290: {
  clean_weak_pointers();
  arg1K0 = finalizer_alist_227X;
  arg1K1 = 25;
  arg1K2 = 25;
  goto L4739;}
 L5184: {
  value_245X = arg1K0;
  *((long *) ((((char *) (-3 + pair_234X))) + 4)) = value_245X;
  arg1K0 = (*((long *) ((((char *) (-3 + alist_232X))) + 4)));
  goto L5155;}
 L4739: {
  alist_246X = arg1K0;
  okay_247X = arg1K1;
  goners_248X = arg1K2;
  if ((25 == alist_246X)) {
    *TT0 = goners_248X;
    return okay_247X;}
  else {
    new_thing_249X = real_copy_object(alist_246X, (ShpS), &new_hp_250X);
    ShpS = new_hp_250X;
    new_thing_251X = real_copy_object((*((long *) (((char *) (-3 + new_thing_249X))))), (ShpS), &new_hp_252X);
    ShpS = new_hp_252X;
    thing_253X = *((long *) (((char *) (-3 + new_thing_251X))));
    next_254X = *((long *) ((((char *) (-3 + new_thing_249X))) + 4));
    if ((3 == (3 & thing_253X))) {
      a_255X = ((char *) (-3 + thing_253X));
      if ((a_255X < (Soldspace_beginS))) {
        arg2K0 = 1;
        arg1K1 = thing_253X;
        goto L4783;}
      else {
        if ((a_255X < (Soldspace_endS))) {
          if ((3 == (3 & (*((long *) ((((char *) (-3 + thing_253X))) + -4)))))) {
            arg2K0 = 1;
            arg1K1 = (*((long *) ((((char *) (-3 + thing_253X))) + -4)));
            goto L4783;}
          else {
            new_thing_256X = real_copy_object(thing_253X, (ShpS), &new_hp_257X);
            ShpS = new_hp_257X;
            arg2K0 = 0;
            arg1K1 = new_thing_256X;
            goto L4783;}}
        else {
          arg2K0 = 1;
          arg1K1 = thing_253X;
          goto L4783;}}}
    else {
      arg2K0 = 1;
      arg1K1 = thing_253X;
      goto L4783;}}}
 L4783: {
  tracedP_258X = arg2K0;
  thing_259X = arg1K1;
  *((long *) (((char *) (-3 + new_thing_251X)))) = thing_259X;
  *((long *) (((char *) (-3 + new_thing_249X)))) = new_thing_251X;
  if (tracedP_258X) {
    *((long *) ((((char *) (-3 + new_thing_249X))) + 4)) = okay_247X;
    arg1K0 = next_254X;
    arg1K1 = new_thing_249X;
    arg1K2 = goners_248X;
    goto L4739;}
  else {
    *((long *) ((((char *) (-3 + new_thing_249X))) + 4)) = goners_248X;
    arg1K0 = next_254X;
    arg1K1 = okay_247X;
    arg1K2 = new_thing_249X;
    goto L4739;}}
}void
s48_heap_init(void)
{
Spure_area_countS = 0;
Simpure_area_countS = 0;
Sfinding_typeS = 1;
SstatusS = NO_ERRORS;
SeofPS = 0;
Sgc_countS = 0;
Hthe_record_type8040 = 536870911;
}
