#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

static long copy_weak_pointer(long, char *, char **);
static long real_copy_object(long, char *, char **);
static void clean_weak_pointers(void);
void write_barrier(char *, long);
long heap_size(void);
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
static long Hthe_record_type7610;
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
    goto L1897;}
  else {
    if (((Sweak_pointer_hpS) < (Sweak_pointer_limitS))) {
      arg0K0 = frontier_2X;
      goto L1902;}
    else {
      goto L1897;}}}
 L1897: {
  old_4X = Sweak_pointer_hpS;
  new_frontier_5X = frontier_2X + 1024;
  Sweak_pointer_hpS = frontier_2X;
  Sweak_pointer_limitS = new_frontier_5X;
  *((long *) (Sweak_pointer_hpS)) = 261190;
  *((long *) ((Sweak_pointer_hpS) + 8)) = (((long) old_4X));
  arg0K0 = new_frontier_5X;
  goto L1902;}
 L1902: {
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
          goto L3052;}
        else {
          if ((a_12X < (Soldspace_endS))) {
            return copy_weak_pointer(thing_8X, frontier_9X, TT0);}
          else {
            goto L3052;}}}
      else {
        goto L3052;}}
    else {
      goto L3052;}}}
 L3052: {
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
    goto L3709;}}
 L3709: {
  start_17X = arg0K0;
  end_18X = arg0K1;
  next_19X = ((char *) (*((long *) (start_17X + 8))));
  arg0K0 = start_17X;
  goto L2008;}
 L2008: {
  scan_20X = arg0K0;
  if ((scan_20X < end_18X)) {
    *((long *) scan_20X) = 1074;
    value_21X = *((long *) (scan_20X + 4));
    if ((3 == (3 & value_21X))) {
      a_22X = ((char *) (-3 + value_21X));
      if ((a_22X < (Soldspace_beginS))) {
        goto L2054;}
      else {
        if ((a_22X < (Soldspace_endS))) {
          if ((3 == (3 & value_21X))) {
            h_23X = *((long *) ((((char *) (-3 + value_21X))) + -4));
            if ((3 == (3 & h_23X))) {
              arg1K0 = h_23X;
              goto L2049;}
            else {
              arg1K0 = 1;
              goto L2049;}}
          else {
            goto L2054;}}
        else {
          goto L2054;}}}
    else {
      goto L2054;}}
  else {
    if ((next_19X == NULL)) {
      if ((end_16X < (Sweak_pointer_limitS))) {
        *((long *) end_16X) = (70 + ((((-4 & ((Sweak_pointer_limitS) - (end_16X + 4))))<<8)));
        return;}
      else {
        return;}}
    else {
      arg0K0 = (next_19X + -1024);
      arg0K1 = next_19X;
      goto L3709;}}}
 L2054: {
  arg0K0 = (scan_20X + 8);
  goto L2008;}
 L2049: {
  x2_24X = arg1K0;
  *((long *) (scan_20X + 4)) = x2_24X;
  goto L2054;}
}
void write_barrier(char * address_25X, long value_26X)
{

 {  return;}
}
long heap_size(void)
{

 {  return ((Snewspace_endS) - (Snewspace_beginS));}
}
long preallocate_space(long cells_27X)
{

 {  return 0;}
}
void register_static_areas(long pure_count_28X, char * *pure_areas_29X, long *pure_sizes_30X, long impure_count_31X, char * *impure_areas_32X, long *impure_sizes_33X)
{

 {  Spure_area_countS = pure_count_28X;
  Spure_areasS = pure_areas_29X;
  Spure_sizesS = pure_sizes_30X;
  Simpure_area_countS = impure_count_31X;
  Simpure_areasS = impure_areas_32X;
  Simpure_sizesS = impure_sizes_33X;
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
void initialize_heap(char * start_34X, long size_35X)
{
  long semisize_37X;
  long cells_36X;
 {  cells_36X = size_35X / 2;
  semisize_37X = ((cells_36X)<<2);
  Snewspace_beginS = start_34X;
  Snewspace_endS = ((Snewspace_beginS) + semisize_37X);
  Soldspace_beginS = (Snewspace_endS);
  Soldspace_endS = ((Soldspace_beginS) + semisize_37X);
  ShpS = (Snewspace_beginS);
  SlimitS = (Snewspace_endS);
  return;}
}
char availableP(long cells_38X)
{

 {  return (((ShpS) + (((cells_38X)<<2))) < (SlimitS));}
}
void begin_collection(void)
{
  char * e_40X;
  char * b_39X;
 {  b_39X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_39X;
  e_40X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_40X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = NULL;
  return;}
}
char * allocate_space(long type_41X, long len_42X, long key_43X)
{
  char * new_44X;
 {  new_44X = ShpS;
  ShpS = ((ShpS) + (-4 & (3 + len_42X)));
  return new_44X;}
}
long available(void)
{

 {  return (((((SlimitS) - (ShpS)))>>2));}
}
long check_image_header(char *filename_45X)
{
  long arg1K0;
  long arg1K2;
  long arg1K1;
  char arg2K1;
  char arg2K0;
  long status_90X;
  char eofP_89X;
  long status_88X;
  long status_87X;
  long status_86X;
  long status_85X;
  long status_84X;
  char eofP_83X;
  char ch_82X;
  long v_81X;
  long status_80X;
  char eofP_79X;
  long thing_78X;
  long cells_77X;
  long status_76X;
  char eofP_75X;
  long thing_74X;
  long cells_73X;
  long status_72X;
  char eofP_71X;
  char thing_70X;
  long status_69X;
  char eofP_68X;
  long thing_67X;
  long old_bytes_per_cell_66X;
  long status_65X;
  char eofP_64X;
  long status_63X;
  char eofP_62X;
  char ch_61X;
  long i_60X;
  long status_59X;
  char eofP_58X;
  long thing_57X;
  char same_versionP_56X;
  long status_55X;
  char eofP_54X;
  long status_53X;
  char eofP_52X;
  char ch_51X;
  long status_50X;
  char eofP_49X;
  char ch_48X;
  long status_47X;
  FILE * port_46X;
 {  port_46X = ps_open_input_file(filename_45X, &status_47X);
  if ((status_47X == NO_ERRORS)) {
    SstatusS = NO_ERRORS;
    SeofPS = 0;
    if ((SeofPS)) {
      goto L2229;}
    else {
      goto L2802;}}
  else {
    ps_write_string("Can't open heap image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    ps_write_string((ps_error_string(status_47X)), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2229: {
  if ((SeofPS)) {
    goto L2262;}
  else {
    goto L2766;}}
 L2802: {
  PS_READ_CHAR(port_46X, ch_48X, eofP_49X, status_50X)
  if (eofP_49X) {
    arg2K0 = eofP_49X;
    arg1K1 = status_50X;
    goto L2207;}
  else {
    if ((status_50X == NO_ERRORS)) {
      if ((12 == ch_48X)) {
        arg2K0 = 0;
        arg1K1 = status_50X;
        goto L2207;}
      else {
        goto L2802;}}
    else {
      arg2K0 = eofP_49X;
      arg1K1 = status_50X;
      goto L2207;}}}
 L2262: {
  if ((SeofPS)) {
    arg2K0 = 0;
    goto L2295;}
  else {
    arg1K0 = 0;
    goto L2742;}}
 L2766: {
  PS_READ_CHAR(port_46X, ch_51X, eofP_52X, status_53X)
  if (eofP_52X) {
    arg2K0 = eofP_52X;
    arg1K1 = status_53X;
    goto L2240;}
  else {
    if ((status_53X == NO_ERRORS)) {
      if ((10 == ch_51X)) {
        arg2K0 = 0;
        arg1K1 = status_53X;
        goto L2240;}
      else {
        goto L2766;}}
    else {
      arg2K0 = eofP_52X;
      arg1K1 = status_53X;
      goto L2240;}}}
 L2207: {
  eofP_54X = arg2K0;
  status_55X = arg1K1;
  if (eofP_54X) {
    SeofPS = 1;
    goto L2229;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2229;}
    else {
      SeofPS = 1;
      SstatusS = status_55X;
      goto L2229;}}}
 L2295: {
  same_versionP_56X = arg2K0;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2330;}
  else {
    PS_READ_INTEGER(port_46X, thing_57X, eofP_58X, status_59X)
    if (eofP_58X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2330;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_57X;
        goto L2330;}
      else {
        SeofPS = 1;
        SstatusS = status_59X;
        arg1K0 = -1;
        goto L2330;}}}}
 L2742: {
  i_60X = arg1K0;
  PS_READ_CHAR(port_46X, ch_61X, eofP_62X, status_63X)
  if (eofP_62X) {
    arg2K0 = 0;
    arg2K1 = eofP_62X;
    arg1K2 = status_63X;
    goto L2273;}
  else {
    if ((status_63X == NO_ERRORS)) {
      if ((i_60X == (strlen((char *) "Vanilla 19")))) {
        arg2K0 = (10 == ch_61X);
        arg2K1 = 0;
        arg1K2 = status_63X;
        goto L2273;}
      else {
        if ((ch_61X == (*("Vanilla 19" + i_60X)))) {
          arg1K0 = (1 + i_60X);
          goto L2742;}
        else {
          arg2K0 = 0;
          arg2K1 = 0;
          arg1K2 = status_63X;
          goto L2273;}}}
    else {
      arg2K0 = 0;
      arg2K1 = eofP_62X;
      arg1K2 = status_63X;
      goto L2273;}}}
 L2240: {
  eofP_64X = arg2K0;
  status_65X = arg1K1;
  if (eofP_64X) {
    SeofPS = 1;
    goto L2262;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2262;}
    else {
      SeofPS = 1;
      SstatusS = status_65X;
      goto L2262;}}}
 L2330: {
  old_bytes_per_cell_66X = arg1K0;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2365;}
  else {
    PS_READ_INTEGER(port_46X, thing_67X, eofP_68X, status_69X)
    if (eofP_68X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2365;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_67X;
        goto L2365;}
      else {
        SeofPS = 1;
        SstatusS = status_69X;
        arg1K0 = -1;
        goto L2365;}}}}
 L2273: {
  thing_70X = arg2K0;
  eofP_71X = arg2K1;
  status_72X = arg1K2;
  if (eofP_71X) {
    SeofPS = 1;
    arg2K0 = 0;
    goto L2295;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      arg2K0 = thing_70X;
      goto L2295;}
    else {
      SeofPS = 1;
      SstatusS = status_72X;
      arg2K0 = 0;
      goto L2295;}}}
 L2365: {
  cells_73X = arg1K0;
  Sold_beginS = (((char *) (((cells_73X)<<2))));
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2403;}
  else {
    PS_READ_INTEGER(port_46X, thing_74X, eofP_75X, status_76X)
    if (eofP_75X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2403;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_74X;
        goto L2403;}
      else {
        SeofPS = 1;
        SstatusS = status_76X;
        arg1K0 = -1;
        goto L2403;}}}}
 L2403: {
  cells_77X = arg1K0;
  Sold_hpS = (((char *) (((cells_77X)<<2))));
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2441;}
  else {
    PS_READ_INTEGER(port_46X, thing_78X, eofP_79X, status_80X)
    if (eofP_79X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2441;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_78X;
        goto L2441;}
      else {
        SeofPS = 1;
        SstatusS = status_80X;
        arg1K0 = -1;
        goto L2441;}}}}
 L2441: {
  v_81X = arg1K0;
  Sstartup_procS = v_81X;
  Simage_portS = port_46X;
  if ((SeofPS)) {
    goto L2480;}
  else {
    goto L2689;}}
 L2480: {
  if (((SstatusS) == NO_ERRORS)) {
    if ((SeofPS)) {
      SstatusS = EDOM;
      ps_write_string("Premature EOF when reading image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L2561;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L2561;}}
    else {
      if (same_versionP_56X) {
        if ((4 == old_bytes_per_cell_66X)) {
          return ((Sold_hpS) - (Sold_beginS));}
        else {
          ps_write_string("Incompatible bytes-per-cell in image", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L2621;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L2621;}}}
      else {
        ps_write_string("Format of image is incompatible with this version of system", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2591;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2591;}}}}
  else {
    ps_write_string("Error reading from image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2536;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2536;}}}
 L2689: {
  PS_READ_CHAR(port_46X, ch_82X, eofP_83X, status_84X)
  if (eofP_83X) {
    arg2K0 = eofP_83X;
    arg1K1 = status_84X;
    goto L2458;}
  else {
    if ((status_84X == NO_ERRORS)) {
      if ((12 == ch_82X)) {
        arg2K0 = 0;
        arg1K1 = status_84X;
        goto L2458;}
      else {
        goto L2689;}}
    else {
      arg2K0 = eofP_83X;
      arg1K1 = status_84X;
      goto L2458;}}}
 L2561: {
  status_85X = ps_close(port_46X);
  if ((status_85X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2621: {
  status_86X = ps_close(port_46X);
  if ((status_86X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2591: {
  status_87X = ps_close(port_46X);
  if ((status_87X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2536: {
  status_88X = ps_close(port_46X);
  if ((status_88X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2458: {
  eofP_89X = arg2K0;
  status_90X = arg1K1;
  if (eofP_89X) {
    SeofPS = 1;
    goto L2480;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2480;}
    else {
      SeofPS = 1;
      SstatusS = status_90X;
      goto L2480;}}}
}
long trace_locationsB(char * start_91X, char * end_92X)
{
  char * arg0K0;
  char * arg0K1;
  long arg1K0;
  long new_106X;
  char * data_addr_105X;
  char * frontier_104X;
  long new_thing_103X;
  char * frontier_102X;
  long new_thing_101X;
  char * a_100X;
  long descriptor_99X;
  long h_98X;
  char * a_97X;
  char * next_96X;
  long thing_95X;
  char * frontier_94X;
  char * addr_93X;
 {  arg0K0 = start_91X;
  arg0K1 = (ShpS);
  goto L3801;}
 L3801: {
  addr_93X = arg0K0;
  frontier_94X = arg0K1;
  if ((addr_93X < end_92X)) {
    thing_95X = *((long *) addr_93X);
    next_96X = addr_93X + 4;
    if ((2 == (3 & thing_95X))) {
      if (((31 & (((thing_95X)>>2))) < 16)) {
        goto L3815;}
      else {
        arg0K0 = (next_96X + (-4 & (3 + ((long)(((unsigned long)thing_95X)>>8)))));
        arg0K1 = frontier_94X;
        goto L3801;}}
    else {
      goto L3815;}}
  else {
    ShpS = frontier_94X;
    return 0;}}
 L3815: {
  if ((3 == (3 & thing_95X))) {
    a_97X = ((char *) (-3 + thing_95X));
    if ((a_97X < (Soldspace_beginS))) {
      arg0K0 = next_96X;
      arg0K1 = frontier_94X;
      goto L3801;}
    else {
      if ((a_97X < (Soldspace_endS))) {
        h_98X = *((long *) ((((char *) (-3 + thing_95X))) + -4));
        if ((3 == (3 & h_98X))) {
          arg1K0 = h_98X;
          arg0K1 = frontier_94X;
          goto L3822;}
        else {
          if ((1074 == h_98X)) {
            descriptor_99X = *((long *) (((char *) (-3 + thing_95X))));
            if ((3 == (3 & descriptor_99X))) {
              a_100X = ((char *) (-3 + descriptor_99X));
              if ((a_100X < (Soldspace_beginS))) {
                goto L5339;}
              else {
                if ((a_100X < (Soldspace_endS))) {
                  new_thing_101X = copy_weak_pointer(thing_95X, frontier_94X, &frontier_102X);
                  arg1K0 = new_thing_101X;
                  arg0K1 = frontier_102X;
                  goto L3822;}
                else {
                  goto L5339;}}}
            else {
              goto L5339;}}
          else {
            goto L5339;}}}
      else {
        arg0K0 = next_96X;
        arg0K1 = frontier_94X;
        goto L3801;}}}
  else {
    arg0K0 = next_96X;
    arg0K1 = frontier_94X;
    goto L3801;}}
 L3822: {
  new_thing_103X = arg1K0;
  frontier_104X = arg0K1;
  *((long *) addr_93X) = new_thing_103X;
  arg0K0 = next_96X;
  arg0K1 = frontier_104X;
  goto L3801;}
 L5339: {
  *((long *) frontier_94X) = h_98X;
  data_addr_105X = frontier_94X + 4;
  new_106X = 3 + (((long) data_addr_105X));
  *((long *) ((((char *) (-3 + thing_95X))) + -4)) = new_106X;
  memcpy((void *)data_addr_105X, (void *)(((char *) (-3 + thing_95X))),((long)(((unsigned long)h_98X)>>8)));
  arg1K0 = new_106X;
  arg0K1 = (data_addr_105X + (-4 & (3 + ((long)(((unsigned long)h_98X)>>8)))));
  goto L3822;}
}
long find_all(long type_107X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc758108_return_tag;
  char Hproc7581080_return_value;
  char * start_109X;
  char * end_110X;
  long d_125X;
  char * addr_124X;
  long type_123X;
  char v_122X;
  long i_121X;
  long count_120X;
  long *sizes_119X;
  char * *areas_118X;
  char v_117X;
  long i_116X;
  long count_115X;
  long *sizes_114X;
  char * *areas_113X;
  char v_112X;
  char * start_hp_111X;
 {  Sfinding_typeS = type_107X;
  start_hp_111X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = ((ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_111X;
  Hproc758108_return_tag = 0;
  goto Hproc758108;
 Hproc758108_return_0:
  v_112X = Hproc7581080_return_value;
  if (v_112X) {
    areas_113X = Simpure_areasS;
    sizes_114X = Simpure_sizesS;
    count_115X = Simpure_area_countS;
    arg1K0 = 0;
    goto L4020;}
  else {
    goto L3980;}}
 L4020: {
  i_116X = arg1K0;
  if ((i_116X < count_115X)) {
    merged_arg0K0 = (*(areas_113X + i_116X));
    merged_arg0K1 = ((*(areas_113X + i_116X)) + (*(sizes_114X + i_116X)));
    Hproc758108_return_tag = 1;
    goto Hproc758108;
   Hproc758108_return_1:
    v_117X = Hproc7581080_return_value;
    if (v_117X) {
      arg1K0 = (1 + i_116X);
      goto L4020;}
    else {
      goto L3980;}}
  else {
    areas_118X = Spure_areasS;
    sizes_119X = Spure_sizesS;
    count_120X = Spure_area_countS;
    arg1K0 = 0;
    goto L4039;}}
 L3980: {
  ShpS = start_hp_111X;
  return 1;}
 L4039: {
  i_121X = arg1K0;
  if ((i_121X < count_120X)) {
    merged_arg0K0 = (*(areas_118X + i_121X));
    merged_arg0K1 = ((*(areas_118X + i_121X)) + (*(sizes_119X + i_121X)));
    Hproc758108_return_tag = 2;
    goto Hproc758108;
   Hproc758108_return_2:
    v_122X = Hproc7581080_return_value;
    if (v_122X) {
      arg1K0 = (1 + i_121X);
      goto L4039;}
    else {
      goto L3980;}}
  else {
    *((long *) start_hp_111X) = (10 + (((((ShpS) - (start_hp_111X + 4)))<<8)));
    return (3 + (((long) (start_hp_111X + 4))));}}
 Hproc758108: {
  start_109X = merged_arg0K0;
  end_110X = merged_arg0K1;{
  type_123X = Sfinding_typeS;
  arg0K0 = start_109X;
  goto L3168;}
 L3168: {
  addr_124X = arg0K0;
  if ((addr_124X < end_110X)) {
    d_125X = *((long *) addr_124X);
    if ((2 == (3 & d_125X))) {
      if ((type_123X == (31 & (((d_125X)>>2))))) {
        if ((((ShpS) + 16) < (SlimitS))) {
          *((long *) (ShpS)) = (3 + (((long) (addr_124X + 4))));
          ShpS = ((ShpS) + 4);
          goto L3200;}
        else {
          Hproc7581080_return_value = 0;
          goto Hproc758108_return;}}
      else {
        goto L3200;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc7581080_return_value = 0;
      goto Hproc758108_return;}}
  else {
    Hproc7581080_return_value = 1;
    goto Hproc758108_return;}}
 L3200: {
  arg0K0 = (addr_124X + (4 + (-4 & (3 + ((long)(((unsigned long)d_125X)>>8))))));
  goto L3168;}
 Hproc758108_return:
  switch (Hproc758108_return_tag) {
  case 0: goto Hproc758108_return_0;
  case 1: goto Hproc758108_return_1;
  default: goto Hproc758108_return_2;
  }}

}
long trace_stob_contentsB(long stob_126X)
{
  char * start_128X;
  long h_127X;
 {  h_127X = *((long *) ((((char *) (-3 + stob_126X))) + -4));
  start_128X = ((char *) (-3 + stob_126X));
  return trace_locationsB(start_128X, (start_128X + (-4 & (3 + ((long)(((unsigned long)h_127X)>>8))))));}
}
long trace_value(long stob_129X)
{
  char * new_hp_132X;
  long new_thing_131X;
  char * a_130X;
 {  if ((3 == (3 & stob_129X))) {
    a_130X = ((char *) (-3 + stob_129X));
    if ((a_130X < (Soldspace_beginS))) {
      return stob_129X;}
    else {
      if ((a_130X < (Soldspace_endS))) {
        new_thing_131X = real_copy_object(stob_129X, (ShpS), &new_hp_132X);
        ShpS = new_hp_132X;
        return new_thing_131X;}
      else {
        return stob_129X;}}}
  else {
    return stob_129X;}}
}
long find_all_records(long record_type_133X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc764134_return_tag;
  char Hproc7641340_return_value;
  char * start_135X;
  char * end_136X;
  long record_152X;
  long d_151X;
  char * addr_150X;
  long type_149X;
  char v_148X;
  long i_147X;
  long count_146X;
  long *sizes_145X;
  char * *areas_144X;
  char v_143X;
  long i_142X;
  long count_141X;
  long *sizes_140X;
  char * *areas_139X;
  char v_138X;
  char * start_hp_137X;
 {  Hthe_record_type7610 = record_type_133X;
  Sfinding_typeS = 8;
  start_hp_137X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = ((ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_137X;
  Hproc764134_return_tag = 0;
  goto Hproc764134;
 Hproc764134_return_0:
  v_138X = Hproc7641340_return_value;
  if (v_138X) {
    areas_139X = Simpure_areasS;
    sizes_140X = Simpure_sizesS;
    count_141X = Simpure_area_countS;
    arg1K0 = 0;
    goto L4167;}
  else {
    goto L4127;}}
 L4167: {
  i_142X = arg1K0;
  if ((i_142X < count_141X)) {
    merged_arg0K0 = (*(areas_139X + i_142X));
    merged_arg0K1 = ((*(areas_139X + i_142X)) + (*(sizes_140X + i_142X)));
    Hproc764134_return_tag = 1;
    goto Hproc764134;
   Hproc764134_return_1:
    v_143X = Hproc7641340_return_value;
    if (v_143X) {
      arg1K0 = (1 + i_142X);
      goto L4167;}
    else {
      goto L4127;}}
  else {
    areas_144X = Spure_areasS;
    sizes_145X = Spure_sizesS;
    count_146X = Spure_area_countS;
    arg1K0 = 0;
    goto L4186;}}
 L4127: {
  ShpS = start_hp_137X;
  return 1;}
 L4186: {
  i_147X = arg1K0;
  if ((i_147X < count_146X)) {
    merged_arg0K0 = (*(areas_144X + i_147X));
    merged_arg0K1 = ((*(areas_144X + i_147X)) + (*(sizes_145X + i_147X)));
    Hproc764134_return_tag = 2;
    goto Hproc764134;
   Hproc764134_return_2:
    v_148X = Hproc7641340_return_value;
    if (v_148X) {
      arg1K0 = (1 + i_147X);
      goto L4186;}
    else {
      goto L4127;}}
  else {
    *((long *) start_hp_137X) = (10 + (((((ShpS) - (start_hp_137X + 4)))<<8)));
    return (3 + (((long) (start_hp_137X + 4))));}}
 Hproc764134: {
  start_135X = merged_arg0K0;
  end_136X = merged_arg0K1;{
  type_149X = Sfinding_typeS;
  arg0K0 = start_135X;
  goto L3315;}
 L3315: {
  addr_150X = arg0K0;
  if ((addr_150X < end_136X)) {
    d_151X = *((long *) addr_150X);
    if ((2 == (3 & d_151X))) {
      if ((type_149X == (31 & (((d_151X)>>2))))) {
        record_152X = 3 + (((long) (addr_150X + 4)));
        if (((Hthe_record_type7610) == (*((long *) (((char *) (-3 + record_152X))))))) {
          if ((((ShpS) + 16) < (SlimitS))) {
            *((long *) (ShpS)) = record_152X;
            ShpS = ((ShpS) + 4);
            goto L3347;}
          else {
            Hproc7641340_return_value = 0;
            goto Hproc764134_return;}}
        else {
          goto L3347;}}
      else {
        goto L3347;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc7641340_return_value = 0;
      goto Hproc764134_return;}}
  else {
    Hproc7641340_return_value = 1;
    goto Hproc764134_return;}}
 L3347: {
  arg0K0 = (addr_150X + (4 + (-4 & (3 + ((long)(((unsigned long)d_151X)>>8))))));
  goto L3315;}
 Hproc764134_return:
  switch (Hproc764134_return_tag) {
  case 0: goto Hproc764134_return_0;
  case 1: goto Hproc764134_return_1;
  default: goto Hproc764134_return_2;
  }}

}
long read_image(long startup_space_153X)
{
  FILE * arg4K0;
  char *arg3K1;
  char arg2K1;
  char arg2K0;
  char * arg0K0;
  long arg1K2;
  long arg1K0;
  long d_195X;
  long descriptor_194X;
  char * ptr_193X;
  long descriptor_192X;
  long status_191X;
  long status_190X;
  long status_189X;
  long status_188X;
  char * next_187X;
  long value_186X;
  long x_185X;
  long x_184X;
  char * ptr_183X;
  long status_182X;
  long status_181X;
  long status_180X;
  char eofP_179X;
  char v_178X;
  char *string_177X;
  char okayP_176X;
  long status_175X;
  long status_174X;
  long status_173X;
  char eofP_172X;
  long got_171X;
  long need_170X;
  char * start_169X;
  char * new_limit_168X;
  char * new_hp_167X;
  long delta_166X;
  long startup_space_165X;
  char reverseP_164X;
  FILE * port_163X;
  long x_162X;
  long x_161X;
  char * addr_160X;
  char *string_159X;
  char okayP_158X;
  long status_157X;
  char eofP_156X;
  long got_155X;
  FILE * port_154X;
 {  port_154X = Simage_portS;
  got_155X = ps_read_block(port_154X, ((char *) (ShpS)), 4, &eofP_156X, &status_157X);
  if ((status_157X == NO_ERRORS)) {
    if (eofP_156X) {
      arg2K0 = 0;
      arg3K1 = "Premature EOF when reading image file";
      goto L4922;}
    else {
      if ((got_155X < 4)) {
        arg2K0 = 0;
        arg3K1 = "Read returned too few bytes";
        goto L4922;}
      else {
        arg2K0 = 1;
        arg3K1 = "";
        goto L4922;}}}
  else {
    SstatusS = status_157X;
    arg2K0 = 0;
    arg3K1 = "Error reading from image file";
    goto L4922;}}
 L4922: {
  okayP_158X = arg2K0;
  string_159X = arg3K1;
  if (okayP_158X) {
    if ((1 == (*((long *) (ShpS))))) {
      arg4K0 = port_154X;
      arg2K1 = 0;
      arg1K2 = startup_space_153X;
      goto L4243;}
    else {
      addr_160X = ShpS;
      x_161X = *((unsigned char *) addr_160X);
      *((unsigned char *) addr_160X) = (*((unsigned char *) (addr_160X + 3)));
      *((unsigned char *) (addr_160X + 3)) = x_161X;
      x_162X = *((unsigned char *) (addr_160X + 1));
      *((unsigned char *) (addr_160X + 1)) = (*((unsigned char *) (addr_160X + 2)));
      *((unsigned char *) (addr_160X + 2)) = x_162X;
      if ((1 == (*((long *) (ShpS))))) {
        arg4K0 = port_154X;
        arg2K1 = 1;
        arg1K2 = startup_space_153X;
        goto L4243;}
      else {
        ps_write_string("Unable to correct byte order", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L5018;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L5018;}}}}
  else {
    ps_write_string(string_159X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4972;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4972;}}}
 L4243: {
  port_163X = arg4K0;
  reverseP_164X = arg2K1;
  startup_space_165X = arg1K2;
  delta_166X = (ShpS) - (Sold_beginS);
  new_hp_167X = (Sold_hpS) + delta_166X;
  new_limit_168X = Snewspace_endS;
  start_169X = ShpS;
  if (((new_hp_167X + startup_space_165X) < new_limit_168X)) {
    need_170X = (Sold_hpS) - (Sold_beginS);
    got_171X = ps_read_block(port_163X, ((char *) (ShpS)), need_170X, &eofP_172X, &status_173X);
    if ((status_173X == NO_ERRORS)) {
      if (eofP_172X) {
        arg2K0 = 0;
        arg3K1 = "Premature EOF when reading image file";
        goto L4259;}
      else {
        if ((got_171X < need_170X)) {
          arg2K0 = 0;
          arg3K1 = "Read returned too few bytes";
          goto L4259;}
        else {
          arg2K0 = 1;
          arg3K1 = "";
          goto L4259;}}}
    else {
      SstatusS = status_173X;
      arg2K0 = 0;
      arg3K1 = "Error reading from image file";
      goto L4259;}}
  else {
    ps_write_string("Heap not big enough to restore this image", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4325;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4325;}}}
 L5018: {
  status_174X = ps_close(port_154X);
  if ((status_174X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4972: {
  status_175X = ps_close(port_154X);
  if ((status_175X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4259: {
  okayP_176X = arg2K0;
  string_177X = arg3K1;
  PS_READ_CHAR(port_163X, v_178X, eofP_179X, status_180X)
  if (okayP_176X) {
    if ((status_180X == NO_ERRORS)) {
      if (eofP_179X) {
        status_181X = ps_close(port_163X);
        if ((status_181X == NO_ERRORS)) {
          if (reverseP_164X) {
            ps_write_string("Correcting byte order of resumed image.", (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = start_169X;
            goto L3472;}
          else {
            goto L4297;}}
        else {
          ps_write_string("Error closing image file", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L4460;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L4460;}}}
      else {
        ps_write_string("Image file has extraneous data after image", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L4430;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L4430;}}}
    else {
      ps_write_string("Error reading from image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L4400;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L4400;}}}
  else {
    ps_write_string(string_177X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4370;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4370;}}}
 L4325: {
  status_182X = ps_close(port_163X);
  if ((status_182X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3472: {
  ptr_183X = arg0K0;
  if ((ptr_183X < new_hp_167X)) {
    x_184X = *((unsigned char *) ptr_183X);
    *((unsigned char *) ptr_183X) = (*((unsigned char *) (ptr_183X + 3)));
    *((unsigned char *) (ptr_183X + 3)) = x_184X;
    x_185X = *((unsigned char *) (ptr_183X + 1));
    *((unsigned char *) (ptr_183X + 1)) = (*((unsigned char *) (ptr_183X + 2)));
    *((unsigned char *) (ptr_183X + 2)) = x_185X;
    value_186X = *((long *) ptr_183X);
    next_187X = ptr_183X + 4;
    if ((2 == (3 & value_186X))) {
      if (((31 & (((value_186X)>>2))) < 16)) {
        arg0K0 = next_187X;
        goto L3472;}
      else {
        arg0K0 = (next_187X + (-4 & (3 + ((long)(((unsigned long)value_186X)>>8)))));
        goto L3472;}}
    else {
      arg0K0 = next_187X;
      goto L3472;}}
  else {
    goto L4297;}}
 L4297: {
  if ((0 == delta_166X)) {
    goto L4307;}
  else {
    arg0K0 = start_169X;
    goto L3571;}}
 L4460: {
  status_188X = ps_close(port_163X);
  if ((status_188X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4430: {
  status_189X = ps_close(port_163X);
  if ((status_189X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4400: {
  status_190X = ps_close(port_163X);
  if ((status_190X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4370: {
  status_191X = ps_close(port_163X);
  if ((status_191X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4307: {
  ShpS = new_hp_167X;
  descriptor_192X = Sstartup_procS;
  if ((3 == (3 & descriptor_192X))) {
    return (3 + (((long) ((((char *) (-3 + descriptor_192X))) + delta_166X))));}
  else {
    return descriptor_192X;}}
 L3571: {
  ptr_193X = arg0K0;
  if ((ptr_193X < new_hp_167X)) {
    descriptor_194X = *((long *) ptr_193X);
    if ((3 == (3 & descriptor_194X))) {
      arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_194X))) + delta_166X))));
      goto L3576;}
    else {
      arg1K0 = descriptor_194X;
      goto L3576;}}
  else {
    goto L4307;}}
 L3576: {
  d_195X = arg1K0;
  *((long *) ptr_193X) = d_195X;
  if ((2 == (3 & d_195X))) {
    if (((31 & (((d_195X)>>2))) < 16)) {
      goto L3590;}
    else {
      arg0K0 = (ptr_193X + (4 + (-4 & (3 + ((long)(((unsigned long)d_195X)>>8))))));
      goto L3571;}}
  else {
    goto L3590;}}
 L3590: {
  arg0K0 = (ptr_193X + 4);
  goto L3571;}
}
long write_image(long resume_proc_196X, FILE * port_197X, void (*mark_traced_channels_closedB_198X)(void))
{
  char * arg0K0;
  long arg1K0;
  long h_225X;
  long h_224X;
  long d_223X;
  char * addr_222X;
  char * e_221X;
  char * b_220X;
  long status_219X;
  long v_218X;
  long v_217X;
  long v_216X;
  long cells_215X;
  long v_214X;
  long cells_213X;
  long v_212X;
  long v_211X;
  long v_210X;
  long v_209X;
  long v_208X;
  char * end_207X;
  char * start_206X;
  char * start_205X;
  long resume_proc_204X;
  char * new_hp_203X;
  long new_thing_202X;
  char * a_201X;
  char * e_200X;
  char * b_199X;
 {  b_199X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_199X;
  e_200X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_200X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = NULL;
  if ((3 == (3 & resume_proc_196X))) {
    a_201X = ((char *) (-3 + resume_proc_196X));
    if ((a_201X < (Soldspace_beginS))) {
      arg1K0 = resume_proc_196X;
      goto L5059;}
    else {
      if ((a_201X < (Soldspace_endS))) {
        new_thing_202X = real_copy_object(resume_proc_196X, (ShpS), &new_hp_203X);
        ShpS = new_hp_203X;
        arg1K0 = new_thing_202X;
        goto L5059;}
      else {
        arg1K0 = resume_proc_196X;
        goto L5059;}}}
  else {
    arg1K0 = resume_proc_196X;
    goto L5059;}}
 L5059: {
  resume_proc_204X = arg1K0;
  start_205X = Snewspace_beginS;
  arg0K0 = start_205X;
  goto L5111;}
 L5111: {
  start_206X = arg0K0;
  end_207X = ShpS;
  trace_locationsB(start_206X, end_207X);
  if (((ShpS) < (SlimitS))) {
    if ((end_207X < (ShpS))) {
      arg0K0 = end_207X;
      goto L5111;}
    else {
      goto L5063;}}
  else {
    ps_error("GC error: ran out of space in new heap", 0);
    goto L5063;}}
 L5063: {
  clean_weak_pointers();
  (*mark_traced_channels_closedB_198X)();
  SstatusS = NO_ERRORS;
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_208X)
    SstatusS = v_208X;
    goto L1658;}
  else {
    goto L1658;}}
 L1658: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_197X, v_209X)
    SstatusS = v_209X;
    goto L1667;}
  else {
    goto L1667;}}
 L1667: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_210X)
    SstatusS = v_210X;
    goto L1676;}
  else {
    goto L1676;}}
 L1676: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string("Vanilla 19", port_197X));
    goto L1683;}
  else {
    goto L1683;}}
 L1683: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_211X)
    SstatusS = v_211X;
    goto L1692;}
  else {
    goto L1692;}}
 L1692: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, port_197X));
    goto L1736;}
  else {
    goto L1736;}}
 L1736: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_212X)
    SstatusS = v_212X;
    goto L1694;}
  else {
    goto L1694;}}
 L1694: {
  cells_213X = ((long) (Snewspace_beginS));
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_213X)>>2)), port_197X));
    goto L1754;}
  else {
    goto L1754;}}
 L1754: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_214X)
    SstatusS = v_214X;
    goto L1698;}
  else {
    goto L1698;}}
 L1698: {
  cells_215X = ((long) (ShpS));
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_215X)>>2)), port_197X));
    goto L1772;}
  else {
    goto L1772;}}
 L1772: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_216X)
    SstatusS = v_216X;
    goto L1702;}
  else {
    goto L1702;}}
 L1702: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_204X, port_197X));
    goto L1787;}
  else {
    goto L1787;}}
 L1787: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_217X)
    SstatusS = v_217X;
    goto L1704;}
  else {
    goto L1704;}}
 L1704: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_197X, v_218X)
    SstatusS = v_218X;
    goto L1713;}
  else {
    goto L1713;}}
 L1713: {
  *((long *) (ShpS)) = 1;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_197X, ((char *) (ShpS)), (((ShpS) + 4) - (ShpS))));
    goto L1724;}
  else {
    goto L1724;}}
 L1724: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_197X, ((char *) (Snewspace_beginS)), ((ShpS) - (Snewspace_beginS))));
    goto L1731;}
  else {
    goto L1731;}}
 L1731: {
  status_219X = SstatusS;
  b_220X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_220X;
  e_221X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_221X;
  SlimitS = (Ssaved_limitS);
  ShpS = (Ssaved_hpS);
  arg0K0 = (Snewspace_beginS);
  goto L2960;}
 L2960: {
  addr_222X = arg0K0;
  if ((addr_222X < (ShpS))) {
    d_223X = *((long *) addr_222X);
    if ((2 == (3 & d_223X))) {
      arg1K0 = d_223X;
      goto L2979;}
    else {
      h_224X = *((long *) ((((char *) (-3 + d_223X))) + -4));
      *((long *) addr_222X) = h_224X;
      arg1K0 = h_224X;
      goto L2979;}}
  else {
    return status_219X;}}
 L2979: {
  h_225X = arg1K0;
  arg0K0 = (addr_222X + (4 + (-4 & (3 + ((long)(((unsigned long)h_225X)>>8))))));
  goto L2960;}
}
long do_gc(long finalizer_alist_226X, long *TT0)
{
  char * arg0K0;
  char arg2K0;
  long arg1K2;
  long arg1K1;
  long arg1K0;
  long thing_258X;
  char tracedP_257X;
  char * new_hp_256X;
  long new_thing_255X;
  char * a_254X;
  long next_253X;
  long thing_252X;
  char * new_hp_251X;
  long new_thing_250X;
  char * new_hp_249X;
  long new_thing_248X;
  long goners_247X;
  long okay_246X;
  long alist_245X;
  long value_244X;
  char * new_hp_243X;
  long new_thing_242X;
  char * a_241X;
  long stob_240X;
  char * end_239X;
  char * start_238X;
  char * start_237X;
  long h_236X;
  long stob_235X;
  long descriptor_234X;
  long pair_233X;
  char * start_232X;
  long alist_231X;
  long i_230X;
  long count_229X;
  long *sizes_228X;
  char * *areas_227X;
 {  areas_227X = Simpure_areasS;
  sizes_228X = Simpure_sizesS;
  count_229X = Simpure_area_countS;
  arg1K0 = 0;
  goto L5283;}
 L5283: {
  i_230X = arg1K0;
  if ((i_230X < count_229X)) {
    trace_locationsB((*(areas_227X + i_230X)), ((*(areas_227X + i_230X)) + (*(sizes_228X + i_230X))));
    arg1K0 = (1 + i_230X);
    goto L5283;}
  else {
    arg1K0 = finalizer_alist_226X;
    goto L5134;}}
 L5134: {
  alist_231X = arg1K0;
  if ((25 == alist_231X)) {
    start_232X = Snewspace_beginS;
    arg0K0 = start_232X;
    goto L5298;}
  else {
    pair_233X = *((long *) (((char *) (-3 + alist_231X))));
    descriptor_234X = *((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + pair_233X)))))))) + -4));
    if ((2 == (3 & descriptor_234X))) {
      stob_235X = *((long *) (((char *) (-3 + pair_233X))));
      h_236X = *((long *) ((((char *) (-3 + stob_235X))) + -4));
      start_237X = ((char *) (-3 + stob_235X));
      trace_locationsB(start_237X, (start_237X + (-4 & (3 + ((long)(((unsigned long)h_236X)>>8))))));
      goto L5159;}
    else {
      goto L5159;}}}
 L5298: {
  start_238X = arg0K0;
  end_239X = ShpS;
  trace_locationsB(start_238X, end_239X);
  if (((ShpS) < (SlimitS))) {
    if ((end_239X < (ShpS))) {
      arg0K0 = end_239X;
      goto L5298;}
    else {
      goto L5269;}}
  else {
    ps_error("GC error: ran out of space in new heap", 0);
    goto L5269;}}
 L5159: {
  stob_240X = *((long *) ((((char *) (-3 + pair_233X))) + 4));
  if ((3 == (3 & stob_240X))) {
    a_241X = ((char *) (-3 + stob_240X));
    if ((a_241X < (Soldspace_beginS))) {
      arg1K0 = stob_240X;
      goto L5163;}
    else {
      if ((a_241X < (Soldspace_endS))) {
        new_thing_242X = real_copy_object(stob_240X, (ShpS), &new_hp_243X);
        ShpS = new_hp_243X;
        arg1K0 = new_thing_242X;
        goto L5163;}
      else {
        arg1K0 = stob_240X;
        goto L5163;}}}
  else {
    arg1K0 = stob_240X;
    goto L5163;}}
 L5269: {
  clean_weak_pointers();
  arg1K0 = finalizer_alist_226X;
  arg1K1 = 25;
  arg1K2 = 25;
  goto L4718;}
 L5163: {
  value_244X = arg1K0;
  *((long *) ((((char *) (-3 + pair_233X))) + 4)) = value_244X;
  arg1K0 = (*((long *) ((((char *) (-3 + alist_231X))) + 4)));
  goto L5134;}
 L4718: {
  alist_245X = arg1K0;
  okay_246X = arg1K1;
  goners_247X = arg1K2;
  if ((25 == alist_245X)) {
    *TT0 = goners_247X;
    return okay_246X;}
  else {
    new_thing_248X = real_copy_object(alist_245X, (ShpS), &new_hp_249X);
    ShpS = new_hp_249X;
    new_thing_250X = real_copy_object((*((long *) (((char *) (-3 + new_thing_248X))))), (ShpS), &new_hp_251X);
    ShpS = new_hp_251X;
    thing_252X = *((long *) (((char *) (-3 + new_thing_250X))));
    next_253X = *((long *) ((((char *) (-3 + new_thing_248X))) + 4));
    if ((3 == (3 & thing_252X))) {
      a_254X = ((char *) (-3 + thing_252X));
      if ((a_254X < (Soldspace_beginS))) {
        arg2K0 = 1;
        arg1K1 = thing_252X;
        goto L4762;}
      else {
        if ((a_254X < (Soldspace_endS))) {
          if ((3 == (3 & (*((long *) ((((char *) (-3 + thing_252X))) + -4)))))) {
            arg2K0 = 1;
            arg1K1 = (*((long *) ((((char *) (-3 + thing_252X))) + -4)));
            goto L4762;}
          else {
            new_thing_255X = real_copy_object(thing_252X, (ShpS), &new_hp_256X);
            ShpS = new_hp_256X;
            arg2K0 = 0;
            arg1K1 = new_thing_255X;
            goto L4762;}}
        else {
          arg2K0 = 1;
          arg1K1 = thing_252X;
          goto L4762;}}}
    else {
      arg2K0 = 1;
      arg1K1 = thing_252X;
      goto L4762;}}}
 L4762: {
  tracedP_257X = arg2K0;
  thing_258X = arg1K1;
  *((long *) (((char *) (-3 + new_thing_250X)))) = thing_258X;
  *((long *) (((char *) (-3 + new_thing_248X)))) = new_thing_250X;
  if (tracedP_257X) {
    *((long *) ((((char *) (-3 + new_thing_248X))) + 4)) = okay_246X;
    arg1K0 = next_253X;
    arg1K1 = new_thing_248X;
    arg1K2 = goners_247X;
    goto L4718;}
  else {
    *((long *) ((((char *) (-3 + new_thing_248X))) + 4)) = goners_247X;
    arg1K0 = next_253X;
    arg1K1 = okay_246X;
    arg1K2 = new_thing_248X;
    goto L4718;}}
}void
s48_heap_init(void)
{
Spure_area_countS = 0;
Simpure_area_countS = 0;
Sfinding_typeS = 1;
SstatusS = NO_ERRORS;
SeofPS = 0;
Sgc_countS = 0;
Hthe_record_type7610 = 536870911;
}
