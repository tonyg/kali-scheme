#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

static long copy_weak_pointer(long, char *, char **);
static long real_copy_object(long, char *, char **);
static void clean_weak_pointers(void);
void write_barrier(char *, long);
void end_collection(void);
long gc_count(void);
char image_writing_okayP(void);
void register_static_areas(long, char **, long*, long, char **, long*);
long preallocate_space(long);
long heap_size(void);
void begin_collection(void);
char availableP(long);
void initialize_heap(char *, long);
char * allocate_space(long, long, long);
long available(void);
long check_image_header(char*);
long trace_locationsB(char *, char *);
long find_all(long);
long trace_value(long);
long trace_stob_contentsB(long);
long read_image(long);
long find_all_records(long);
long write_image(long, FILE *, void(*)(void));
long do_gc(long, long*);
static char * Sweak_pointer_limitS;
static char * Sweak_pointer_hpS;
static char * Ssaved_limitS;
static char * Ssaved_hpS;
static long Sgc_countS;
static long Sstartup_procS;
static char * Sold_hpS;
static char * Sold_beginS;
static FILE * Simage_portS;
static char SeofPS;
static long SstatusS;
static long Sfinding_typeS;
static long Simpure_area_countS;
static long Spure_area_countS;
static long *Simpure_sizesS;
static long *Spure_sizesS;
static char * *Simpure_areasS;
static char * *Spure_areasS;
static char * Soldspace_endS;
static char * Soldspace_beginS;
static char * Snewspace_endS;
static char * Snewspace_beginS;
static long Hthe_record_type7460;
char * SlimitS;
char * ShpS;

static long copy_weak_pointer(long weak_1X, char * frontier_2X, char * *TT0)
{
  char * arg0K0;
  long new_7X;
  char * frontier_6X;
  char * new_frontier_5X;
  char * old_4X;
  char * x_3X;
 {  x_3X = Sweak_pointer_hpS;
  if ((x_3X == ((char *) -1))) {
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
          goto L2974;}
        else {
          if ((a_12X < (Soldspace_endS))) {
            return copy_weak_pointer(thing_8X, frontier_9X, TT0);}
          else {
            goto L2974;}}}
      else {
        goto L2974;}}
    else {
      goto L2974;}}}
 L2974: {
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
 {  if (((Sweak_pointer_hpS) == ((char *) -1))) {
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
    if ((next_19X == ((char *) -1))) {
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
void end_collection(void)
{

 {  Sgc_countS = (1 + (Sgc_countS));
  return;}
}
long gc_count(void)
{

 {  return (Sgc_countS);}
}
char image_writing_okayP(void)
{

 {  if ((0 == (Spure_area_countS))) {
    return (0 == (Simpure_area_countS));}
  else {
    return 0;}}
}
void register_static_areas(long pure_count_27X, char * *pure_areas_28X, long *pure_sizes_29X, long impure_count_30X, char * *impure_areas_31X, long *impure_sizes_32X)
{

 {  Spure_area_countS = pure_count_27X;
  Spure_areasS = pure_areas_28X;
  Spure_sizesS = pure_sizes_29X;
  Simpure_area_countS = impure_count_30X;
  Simpure_areasS = impure_areas_31X;
  Simpure_sizesS = impure_sizes_32X;
  return;}
}
long preallocate_space(long cells_33X)
{

 {  return 0;}
}
long heap_size(void)
{

 {  return ((Snewspace_endS) - (Snewspace_beginS));}
}
void begin_collection(void)
{
  char * e_35X;
  char * b_34X;
 {  b_34X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_34X;
  e_35X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_35X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = ((char *) -1);
  return;}
}
char availableP(long cells_36X)
{

 {  return (((ShpS) + (((cells_36X)<<2))) < (SlimitS));}
}
void initialize_heap(char * start_37X, long size_38X)
{
  long semisize_40X;
  long cells_39X;
 {  cells_39X = size_38X / 2;
  semisize_40X = ((cells_39X)<<2);
  Snewspace_beginS = start_37X;
  Snewspace_endS = ((Snewspace_beginS) + semisize_40X);
  Soldspace_beginS = (Snewspace_endS);
  Soldspace_endS = ((Soldspace_beginS) + semisize_40X);
  ShpS = (Snewspace_beginS);
  SlimitS = (Snewspace_endS);
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
      goto L2263;}
    else {
      goto L2836;}}
  else {
    ps_write_string("Can't open heap image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    ps_write_string((ps_error_string(status_47X)), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2263: {
  if ((SeofPS)) {
    goto L2296;}
  else {
    goto L2800;}}
 L2836: {
  PS_READ_CHAR(port_46X, ch_48X, eofP_49X, status_50X)
  if (eofP_49X) {
    arg2K0 = eofP_49X;
    arg1K1 = status_50X;
    goto L2241;}
  else {
    if ((status_50X == NO_ERRORS)) {
      if ((12 == ch_48X)) {
        arg2K0 = 0;
        arg1K1 = status_50X;
        goto L2241;}
      else {
        goto L2836;}}
    else {
      arg2K0 = eofP_49X;
      arg1K1 = status_50X;
      goto L2241;}}}
 L2296: {
  if ((SeofPS)) {
    arg2K0 = 0;
    goto L2329;}
  else {
    arg1K0 = 0;
    goto L2776;}}
 L2800: {
  PS_READ_CHAR(port_46X, ch_51X, eofP_52X, status_53X)
  if (eofP_52X) {
    arg2K0 = eofP_52X;
    arg1K1 = status_53X;
    goto L2274;}
  else {
    if ((status_53X == NO_ERRORS)) {
      if ((10 == ch_51X)) {
        arg2K0 = 0;
        arg1K1 = status_53X;
        goto L2274;}
      else {
        goto L2800;}}
    else {
      arg2K0 = eofP_52X;
      arg1K1 = status_53X;
      goto L2274;}}}
 L2241: {
  eofP_54X = arg2K0;
  status_55X = arg1K1;
  if (eofP_54X) {
    SeofPS = 1;
    goto L2263;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2263;}
    else {
      SeofPS = 1;
      SstatusS = status_55X;
      goto L2263;}}}
 L2329: {
  same_versionP_56X = arg2K0;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2364;}
  else {
    PS_READ_INTEGER(port_46X, thing_57X, eofP_58X, status_59X)
    if (eofP_58X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2364;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_57X;
        goto L2364;}
      else {
        SeofPS = 1;
        SstatusS = status_59X;
        arg1K0 = -1;
        goto L2364;}}}}
 L2776: {
  i_60X = arg1K0;
  PS_READ_CHAR(port_46X, ch_61X, eofP_62X, status_63X)
  if (eofP_62X) {
    arg2K0 = 0;
    arg2K1 = eofP_62X;
    arg1K2 = status_63X;
    goto L2307;}
  else {
    if ((status_63X == NO_ERRORS)) {
      if ((i_60X == (strlen((char *) "Vanilla 19")))) {
        arg2K0 = (10 == ch_61X);
        arg2K1 = 0;
        arg1K2 = status_63X;
        goto L2307;}
      else {
        if ((ch_61X == (*("Vanilla 19" + i_60X)))) {
          arg1K0 = (1 + i_60X);
          goto L2776;}
        else {
          arg2K0 = 0;
          arg2K1 = 0;
          arg1K2 = status_63X;
          goto L2307;}}}
    else {
      arg2K0 = 0;
      arg2K1 = eofP_62X;
      arg1K2 = status_63X;
      goto L2307;}}}
 L2274: {
  eofP_64X = arg2K0;
  status_65X = arg1K1;
  if (eofP_64X) {
    SeofPS = 1;
    goto L2296;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2296;}
    else {
      SeofPS = 1;
      SstatusS = status_65X;
      goto L2296;}}}
 L2364: {
  old_bytes_per_cell_66X = arg1K0;
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2399;}
  else {
    PS_READ_INTEGER(port_46X, thing_67X, eofP_68X, status_69X)
    if (eofP_68X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2399;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_67X;
        goto L2399;}
      else {
        SeofPS = 1;
        SstatusS = status_69X;
        arg1K0 = -1;
        goto L2399;}}}}
 L2307: {
  thing_70X = arg2K0;
  eofP_71X = arg2K1;
  status_72X = arg1K2;
  if (eofP_71X) {
    SeofPS = 1;
    arg2K0 = 0;
    goto L2329;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      arg2K0 = thing_70X;
      goto L2329;}
    else {
      SeofPS = 1;
      SstatusS = status_72X;
      arg2K0 = 0;
      goto L2329;}}}
 L2399: {
  cells_73X = arg1K0;
  Sold_beginS = (((char *) (((cells_73X)<<2))));
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2437;}
  else {
    PS_READ_INTEGER(port_46X, thing_74X, eofP_75X, status_76X)
    if (eofP_75X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2437;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_74X;
        goto L2437;}
      else {
        SeofPS = 1;
        SstatusS = status_76X;
        arg1K0 = -1;
        goto L2437;}}}}
 L2437: {
  cells_77X = arg1K0;
  Sold_hpS = (((char *) (((cells_77X)<<2))));
  if ((SeofPS)) {
    arg1K0 = -1;
    goto L2475;}
  else {
    PS_READ_INTEGER(port_46X, thing_78X, eofP_79X, status_80X)
    if (eofP_79X) {
      SeofPS = 1;
      arg1K0 = -1;
      goto L2475;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg1K0 = thing_78X;
        goto L2475;}
      else {
        SeofPS = 1;
        SstatusS = status_80X;
        arg1K0 = -1;
        goto L2475;}}}}
 L2475: {
  v_81X = arg1K0;
  Sstartup_procS = v_81X;
  Simage_portS = port_46X;
  if ((SeofPS)) {
    goto L2514;}
  else {
    goto L2723;}}
 L2514: {
  if (((SstatusS) == NO_ERRORS)) {
    if ((SeofPS)) {
      SstatusS = EDOM;
      ps_write_string("Premature EOF when reading image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L2595;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L2595;}}
    else {
      if (same_versionP_56X) {
        if ((4 == old_bytes_per_cell_66X)) {
          return ((Sold_hpS) - (Sold_beginS));}
        else {
          ps_write_string("Incompatible bytes-per-cell in image", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L2655;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L2655;}}}
      else {
        ps_write_string("Format of image is incompatible with this version of system", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2625;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2625;}}}}
  else {
    ps_write_string("Error reading from image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2570;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2570;}}}
 L2723: {
  PS_READ_CHAR(port_46X, ch_82X, eofP_83X, status_84X)
  if (eofP_83X) {
    arg2K0 = eofP_83X;
    arg1K1 = status_84X;
    goto L2492;}
  else {
    if ((status_84X == NO_ERRORS)) {
      if ((12 == ch_82X)) {
        arg2K0 = 0;
        arg1K1 = status_84X;
        goto L2492;}
      else {
        goto L2723;}}
    else {
      arg2K0 = eofP_83X;
      arg1K1 = status_84X;
      goto L2492;}}}
 L2595: {
  status_85X = ps_close(port_46X);
  if ((status_85X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2655: {
  status_86X = ps_close(port_46X);
  if ((status_86X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2625: {
  status_87X = ps_close(port_46X);
  if ((status_87X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2570: {
  status_88X = ps_close(port_46X);
  if ((status_88X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2492: {
  eofP_89X = arg2K0;
  status_90X = arg1K1;
  if (eofP_89X) {
    SeofPS = 1;
    goto L2514;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2514;}
    else {
      SeofPS = 1;
      SstatusS = status_90X;
      goto L2514;}}}
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
  goto L3816;}
 L3816: {
  addr_93X = arg0K0;
  frontier_94X = arg0K1;
  if ((addr_93X < end_92X)) {
    thing_95X = *((long *) addr_93X);
    next_96X = addr_93X + 4;
    if ((2 == (3 & thing_95X))) {
      if (((31 & (((thing_95X)>>2))) < 16)) {
        goto L3830;}
      else {
        arg0K0 = (next_96X + (-4 & (3 + ((long)(((unsigned long)thing_95X)>>8)))));
        arg0K1 = frontier_94X;
        goto L3816;}}
    else {
      goto L3830;}}
  else {
    ShpS = frontier_94X;
    return 0;}}
 L3830: {
  if ((3 == (3 & thing_95X))) {
    a_97X = ((char *) (-3 + thing_95X));
    if ((a_97X < (Soldspace_beginS))) {
      arg0K0 = next_96X;
      arg0K1 = frontier_94X;
      goto L3816;}
    else {
      if ((a_97X < (Soldspace_endS))) {
        h_98X = *((long *) ((((char *) (-3 + thing_95X))) + -4));
        if ((3 == (3 & h_98X))) {
          arg1K0 = h_98X;
          arg0K1 = frontier_94X;
          goto L3837;}
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
                  goto L3837;}
                else {
                  goto L5339;}}}
            else {
              goto L5339;}}
          else {
            goto L5339;}}}
      else {
        arg0K0 = next_96X;
        arg0K1 = frontier_94X;
        goto L3816;}}}
  else {
    arg0K0 = next_96X;
    arg0K1 = frontier_94X;
    goto L3816;}}
 L3837: {
  new_thing_103X = arg1K0;
  frontier_104X = arg0K1;
  *((long *) addr_93X) = new_thing_103X;
  arg0K0 = next_96X;
  arg0K1 = frontier_104X;
  goto L3816;}
 L5339: {
  *((long *) frontier_94X) = h_98X;
  data_addr_105X = frontier_94X + 4;
  new_106X = 3 + (((long) data_addr_105X));
  *((long *) ((((char *) (-3 + thing_95X))) + -4)) = new_106X;
  memcpy((void *)data_addr_105X, (void *)(((char *) (-3 + thing_95X))),((long)(((unsigned long)h_98X)>>8)));
  arg1K0 = new_106X;
  arg0K1 = (data_addr_105X + (-4 & (3 + ((long)(((unsigned long)h_98X)>>8)))));
  goto L3837;}
}
long find_all(long type_107X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc747108_return_tag;
  char Hproc7471080_return_value;
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
  Hproc747108_return_tag = 0;
  goto Hproc747108;
 Hproc747108_return_0:
  v_112X = Hproc7471080_return_value;
  if (v_112X) {
    areas_113X = Simpure_areasS;
    sizes_114X = Simpure_sizesS;
    count_115X = Simpure_area_countS;
    arg1K0 = 0;
    goto L4368;}
  else {
    goto L4328;}}
 L4368: {
  i_116X = arg1K0;
  if ((i_116X < count_115X)) {
    merged_arg0K0 = (*(areas_113X + i_116X));
    merged_arg0K1 = ((*(areas_113X + i_116X)) + (*(sizes_114X + i_116X)));
    Hproc747108_return_tag = 1;
    goto Hproc747108;
   Hproc747108_return_1:
    v_117X = Hproc7471080_return_value;
    if (v_117X) {
      arg1K0 = (1 + i_116X);
      goto L4368;}
    else {
      goto L4328;}}
  else {
    areas_118X = Spure_areasS;
    sizes_119X = Spure_sizesS;
    count_120X = Spure_area_countS;
    arg1K0 = 0;
    goto L4387;}}
 L4328: {
  ShpS = start_hp_111X;
  return 1;}
 L4387: {
  i_121X = arg1K0;
  if ((i_121X < count_120X)) {
    merged_arg0K0 = (*(areas_118X + i_121X));
    merged_arg0K1 = ((*(areas_118X + i_121X)) + (*(sizes_119X + i_121X)));
    Hproc747108_return_tag = 2;
    goto Hproc747108;
   Hproc747108_return_2:
    v_122X = Hproc7471080_return_value;
    if (v_122X) {
      arg1K0 = (1 + i_121X);
      goto L4387;}
    else {
      goto L4328;}}
  else {
    *((long *) start_hp_111X) = (10 + (((((ShpS) - (start_hp_111X + 4)))<<8)));
    return (3 + (((long) (start_hp_111X + 4))));}}
 Hproc747108: {
  start_109X = merged_arg0K0;
  end_110X = merged_arg0K1;{
  type_123X = Sfinding_typeS;
  arg0K0 = start_109X;
  goto L3362;}
 L3362: {
  addr_124X = arg0K0;
  if ((addr_124X < end_110X)) {
    d_125X = *((long *) addr_124X);
    if ((2 == (3 & d_125X))) {
      if ((type_123X == (31 & (((d_125X)>>2))))) {
        if ((((ShpS) + 16) < (SlimitS))) {
          *((long *) (ShpS)) = (3 + (((long) (addr_124X + 4))));
          ShpS = ((ShpS) + 4);
          goto L3394;}
        else {
          Hproc7471080_return_value = 0;
          goto Hproc747108_return;}}
      else {
        goto L3394;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc7471080_return_value = 0;
      goto Hproc747108_return;}}
  else {
    Hproc7471080_return_value = 1;
    goto Hproc747108_return;}}
 L3394: {
  arg0K0 = (addr_124X + (4 + (-4 & (3 + ((long)(((unsigned long)d_125X)>>8))))));
  goto L3362;}
 Hproc747108_return:
  switch (Hproc747108_return_tag) {
  case 0: goto Hproc747108_return_0;
  case 1: goto Hproc747108_return_1;
  default: goto Hproc747108_return_2;
  }}

}
long trace_value(long stob_126X)
{
  char * new_hp_129X;
  long new_thing_128X;
  char * a_127X;
 {  if ((3 == (3 & stob_126X))) {
    a_127X = ((char *) (-3 + stob_126X));
    if ((a_127X < (Soldspace_beginS))) {
      return stob_126X;}
    else {
      if ((a_127X < (Soldspace_endS))) {
        new_thing_128X = real_copy_object(stob_126X, (ShpS), &new_hp_129X);
        ShpS = new_hp_129X;
        return new_thing_128X;}
      else {
        return stob_126X;}}}
  else {
    return stob_126X;}}
}
long trace_stob_contentsB(long stob_130X)
{
  char * start_132X;
  long h_131X;
 {  h_131X = *((long *) ((((char *) (-3 + stob_130X))) + -4));
  start_132X = ((char *) (-3 + stob_130X));
  return trace_locationsB(start_132X, (start_132X + (-4 & (3 + ((long)(((unsigned long)h_131X)>>8))))));}
}
long read_image(long startup_space_133X)
{
  FILE * arg4K0;
  char *arg3K1;
  char arg2K1;
  char arg2K0;
  long arg1K2;
  long arg1K0;
  char * arg0K0;
  long d_175X;
  long descriptor_174X;
  char * ptr_173X;
  long descriptor_172X;
  long status_171X;
  long status_170X;
  long status_169X;
  long status_168X;
  char * next_167X;
  long value_166X;
  long x_165X;
  long x_164X;
  char * ptr_163X;
  long status_162X;
  long status_161X;
  long status_160X;
  char eofP_159X;
  char v_158X;
  char *string_157X;
  char okayP_156X;
  long status_155X;
  long status_154X;
  long status_153X;
  char eofP_152X;
  long got_151X;
  long need_150X;
  char * start_149X;
  char * new_limit_148X;
  char * new_hp_147X;
  long delta_146X;
  long startup_space_145X;
  char reverseP_144X;
  FILE * port_143X;
  long x_142X;
  long x_141X;
  char * addr_140X;
  char *string_139X;
  char okayP_138X;
  long status_137X;
  char eofP_136X;
  long got_135X;
  FILE * port_134X;
 {  port_134X = Simage_portS;
  got_135X = ps_read_block(port_134X, ((char *) (ShpS)), 4, &eofP_136X, &status_137X);
  if ((status_137X == NO_ERRORS)) {
    if (eofP_136X) {
      arg2K0 = 0;
      arg3K1 = "Premature EOF when reading image file";
      goto L4915;}
    else {
      if ((got_135X < 4)) {
        arg2K0 = 0;
        arg3K1 = "Read returned too few bytes";
        goto L4915;}
      else {
        arg2K0 = 1;
        arg3K1 = "";
        goto L4915;}}}
  else {
    SstatusS = status_137X;
    arg2K0 = 0;
    arg3K1 = "Error reading from image file";
    goto L4915;}}
 L4915: {
  okayP_138X = arg2K0;
  string_139X = arg3K1;
  if (okayP_138X) {
    if ((1 == (*((long *) (ShpS))))) {
      arg4K0 = port_134X;
      arg2K1 = 0;
      arg1K2 = startup_space_133X;
      goto L3949;}
    else {
      addr_140X = ShpS;
      x_141X = *((unsigned char *) addr_140X);
      *((unsigned char *) addr_140X) = (*((unsigned char *) (addr_140X + 3)));
      *((unsigned char *) (addr_140X + 3)) = x_141X;
      x_142X = *((unsigned char *) (addr_140X + 1));
      *((unsigned char *) (addr_140X + 1)) = (*((unsigned char *) (addr_140X + 2)));
      *((unsigned char *) (addr_140X + 2)) = x_142X;
      if ((1 == (*((long *) (ShpS))))) {
        arg4K0 = port_134X;
        arg2K1 = 1;
        arg1K2 = startup_space_133X;
        goto L3949;}
      else {
        ps_write_string("Unable to correct byte order", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L5011;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L5011;}}}}
  else {
    ps_write_string(string_139X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4965;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4965;}}}
 L3949: {
  port_143X = arg4K0;
  reverseP_144X = arg2K1;
  startup_space_145X = arg1K2;
  delta_146X = (ShpS) - (Sold_beginS);
  new_hp_147X = (Sold_hpS) + delta_146X;
  new_limit_148X = Snewspace_endS;
  start_149X = ShpS;
  if (((new_hp_147X + startup_space_145X) < new_limit_148X)) {
    need_150X = (Sold_hpS) - (Sold_beginS);
    got_151X = ps_read_block(port_143X, ((char *) (ShpS)), need_150X, &eofP_152X, &status_153X);
    if ((status_153X == NO_ERRORS)) {
      if (eofP_152X) {
        arg2K0 = 0;
        arg3K1 = "Premature EOF when reading image file";
        goto L3965;}
      else {
        if ((got_151X < need_150X)) {
          arg2K0 = 0;
          arg3K1 = "Read returned too few bytes";
          goto L3965;}
        else {
          arg2K0 = 1;
          arg3K1 = "";
          goto L3965;}}}
    else {
      SstatusS = status_153X;
      arg2K0 = 0;
      arg3K1 = "Error reading from image file";
      goto L3965;}}
  else {
    ps_write_string("Heap not big enough to restore this image", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4031;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4031;}}}
 L5011: {
  status_154X = ps_close(port_134X);
  if ((status_154X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4965: {
  status_155X = ps_close(port_134X);
  if ((status_155X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3965: {
  okayP_156X = arg2K0;
  string_157X = arg3K1;
  PS_READ_CHAR(port_143X, v_158X, eofP_159X, status_160X)
  if (okayP_156X) {
    if ((status_160X == NO_ERRORS)) {
      if (eofP_159X) {
        status_161X = ps_close(port_143X);
        if ((status_161X == NO_ERRORS)) {
          if (reverseP_144X) {
            ps_write_string("Correcting byte order of resumed image.", (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = start_149X;
            goto L3262;}
          else {
            goto L4003;}}
        else {
          ps_write_string("Error closing image file", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L4166;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L4166;}}}
      else {
        ps_write_string("Image file has extraneous data after image", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L4136;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L4136;}}}
    else {
      ps_write_string("Error reading from image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L4106;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L4106;}}}
  else {
    ps_write_string(string_157X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4076;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4076;}}}
 L4031: {
  status_162X = ps_close(port_143X);
  if ((status_162X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3262: {
  ptr_163X = arg0K0;
  if ((ptr_163X < new_hp_147X)) {
    x_164X = *((unsigned char *) ptr_163X);
    *((unsigned char *) ptr_163X) = (*((unsigned char *) (ptr_163X + 3)));
    *((unsigned char *) (ptr_163X + 3)) = x_164X;
    x_165X = *((unsigned char *) (ptr_163X + 1));
    *((unsigned char *) (ptr_163X + 1)) = (*((unsigned char *) (ptr_163X + 2)));
    *((unsigned char *) (ptr_163X + 2)) = x_165X;
    value_166X = *((long *) ptr_163X);
    next_167X = ptr_163X + 4;
    if ((2 == (3 & value_166X))) {
      if (((31 & (((value_166X)>>2))) < 16)) {
        arg0K0 = next_167X;
        goto L3262;}
      else {
        arg0K0 = (next_167X + (-4 & (3 + ((long)(((unsigned long)value_166X)>>8)))));
        goto L3262;}}
    else {
      arg0K0 = next_167X;
      goto L3262;}}
  else {
    goto L4003;}}
 L4003: {
  if ((0 == delta_146X)) {
    goto L4013;}
  else {
    arg0K0 = start_149X;
    goto L3167;}}
 L4166: {
  status_168X = ps_close(port_143X);
  if ((status_168X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4136: {
  status_169X = ps_close(port_143X);
  if ((status_169X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4106: {
  status_170X = ps_close(port_143X);
  if ((status_170X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4076: {
  status_171X = ps_close(port_143X);
  if ((status_171X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L4013: {
  ShpS = new_hp_147X;
  descriptor_172X = Sstartup_procS;
  if ((3 == (3 & descriptor_172X))) {
    return (3 + (((long) ((((char *) (-3 + descriptor_172X))) + delta_146X))));}
  else {
    return descriptor_172X;}}
 L3167: {
  ptr_173X = arg0K0;
  if ((ptr_173X < new_hp_147X)) {
    descriptor_174X = *((long *) ptr_173X);
    if ((3 == (3 & descriptor_174X))) {
      arg1K0 = (3 + (((long) ((((char *) (-3 + descriptor_174X))) + delta_146X))));
      goto L3172;}
    else {
      arg1K0 = descriptor_174X;
      goto L3172;}}
  else {
    goto L4013;}}
 L3172: {
  d_175X = arg1K0;
  *((long *) ptr_173X) = d_175X;
  if ((2 == (3 & d_175X))) {
    if (((31 & (((d_175X)>>2))) < 16)) {
      goto L3186;}
    else {
      arg0K0 = (ptr_173X + (4 + (-4 & (3 + ((long)(((unsigned long)d_175X)>>8))))));
      goto L3167;}}
  else {
    goto L3186;}}
 L3186: {
  arg0K0 = (ptr_173X + 4);
  goto L3167;}
}
long find_all_records(long record_type_176X)
{
  char * arg0K0;
  long arg1K0;
  char * merged_arg0K1;
  char * merged_arg0K0;

  int Hproc765177_return_tag;
  char Hproc7651770_return_value;
  char * start_178X;
  char * end_179X;
  long record_195X;
  long d_194X;
  char * addr_193X;
  long type_192X;
  char v_191X;
  long i_190X;
  long count_189X;
  long *sizes_188X;
  char * *areas_187X;
  char v_186X;
  long i_185X;
  long count_184X;
  long *sizes_183X;
  char * *areas_182X;
  char v_181X;
  char * start_hp_180X;
 {  Hthe_record_type7460 = record_type_176X;
  Sfinding_typeS = 8;
  start_hp_180X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = ((ShpS) + 4);
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_180X;
  Hproc765177_return_tag = 0;
  goto Hproc765177;
 Hproc765177_return_0:
  v_181X = Hproc7651770_return_value;
  if (v_181X) {
    areas_182X = Simpure_areasS;
    sizes_183X = Simpure_sizesS;
    count_184X = Simpure_area_countS;
    arg1K0 = 0;
    goto L4515;}
  else {
    goto L4475;}}
 L4515: {
  i_185X = arg1K0;
  if ((i_185X < count_184X)) {
    merged_arg0K0 = (*(areas_182X + i_185X));
    merged_arg0K1 = ((*(areas_182X + i_185X)) + (*(sizes_183X + i_185X)));
    Hproc765177_return_tag = 1;
    goto Hproc765177;
   Hproc765177_return_1:
    v_186X = Hproc7651770_return_value;
    if (v_186X) {
      arg1K0 = (1 + i_185X);
      goto L4515;}
    else {
      goto L4475;}}
  else {
    areas_187X = Spure_areasS;
    sizes_188X = Spure_sizesS;
    count_189X = Spure_area_countS;
    arg1K0 = 0;
    goto L4534;}}
 L4475: {
  ShpS = start_hp_180X;
  return 1;}
 L4534: {
  i_190X = arg1K0;
  if ((i_190X < count_189X)) {
    merged_arg0K0 = (*(areas_187X + i_190X));
    merged_arg0K1 = ((*(areas_187X + i_190X)) + (*(sizes_188X + i_190X)));
    Hproc765177_return_tag = 2;
    goto Hproc765177;
   Hproc765177_return_2:
    v_191X = Hproc7651770_return_value;
    if (v_191X) {
      arg1K0 = (1 + i_190X);
      goto L4534;}
    else {
      goto L4475;}}
  else {
    *((long *) start_hp_180X) = (10 + (((((ShpS) - (start_hp_180X + 4)))<<8)));
    return (3 + (((long) (start_hp_180X + 4))));}}
 Hproc765177: {
  start_178X = merged_arg0K0;
  end_179X = merged_arg0K1;{
  type_192X = Sfinding_typeS;
  arg0K0 = start_178X;
  goto L3509;}
 L3509: {
  addr_193X = arg0K0;
  if ((addr_193X < end_179X)) {
    d_194X = *((long *) addr_193X);
    if ((2 == (3 & d_194X))) {
      if ((type_192X == (31 & (((d_194X)>>2))))) {
        record_195X = 3 + (((long) (addr_193X + 4)));
        if (((Hthe_record_type7460) == (*((long *) (((char *) (-3 + record_195X))))))) {
          if ((((ShpS) + 16) < (SlimitS))) {
            *((long *) (ShpS)) = record_195X;
            ShpS = ((ShpS) + 4);
            goto L3541;}
          else {
            Hproc7651770_return_value = 0;
            goto Hproc765177_return;}}
        else {
          goto L3541;}}
      else {
        goto L3541;}}
    else {
      ps_write_string("heap is in an inconsistent state.", (stderr));
      Hproc7651770_return_value = 0;
      goto Hproc765177_return;}}
  else {
    Hproc7651770_return_value = 1;
    goto Hproc765177_return;}}
 L3541: {
  arg0K0 = (addr_193X + (4 + (-4 & (3 + ((long)(((unsigned long)d_194X)>>8))))));
  goto L3509;}
 Hproc765177_return:
  switch (Hproc765177_return_tag) {
  case 0: goto Hproc765177_return_0;
  case 1: goto Hproc765177_return_1;
  default: goto Hproc765177_return_2;
  }}

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
  Sweak_pointer_hpS = ((char *) -1);
  if ((3 == (3 & resume_proc_196X))) {
    a_201X = ((char *) (-3 + resume_proc_196X));
    if ((a_201X < (Soldspace_beginS))) {
      arg1K0 = resume_proc_196X;
      goto L5186;}
    else {
      if ((a_201X < (Soldspace_endS))) {
        new_thing_202X = real_copy_object(resume_proc_196X, (ShpS), &new_hp_203X);
        ShpS = new_hp_203X;
        arg1K0 = new_thing_202X;
        goto L5186;}
      else {
        arg1K0 = resume_proc_196X;
        goto L5186;}}}
  else {
    arg1K0 = resume_proc_196X;
    goto L5186;}}
 L5186: {
  resume_proc_204X = arg1K0;
  start_205X = Snewspace_beginS;
  arg0K0 = start_205X;
  goto L5238;}
 L5238: {
  start_206X = arg0K0;
  end_207X = ShpS;
  trace_locationsB(start_206X, end_207X);
  if (((ShpS) < (SlimitS))) {
    if ((end_207X < (ShpS))) {
      arg0K0 = end_207X;
      goto L5238;}
    else {
      goto L5190;}}
  else {
    ps_error("GC error: ran out of space in new heap", 0);
    goto L5190;}}
 L5190: {
  clean_weak_pointers();
  (*mark_traced_channels_closedB_198X)();
  SstatusS = NO_ERRORS;
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_208X)
    SstatusS = v_208X;
    goto L1474;}
  else {
    goto L1474;}}
 L1474: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_197X, v_209X)
    SstatusS = v_209X;
    goto L1483;}
  else {
    goto L1483;}}
 L1483: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_210X)
    SstatusS = v_210X;
    goto L1492;}
  else {
    goto L1492;}}
 L1492: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string("Vanilla 19", port_197X));
    goto L1499;}
  else {
    goto L1499;}}
 L1499: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_211X)
    SstatusS = v_211X;
    goto L1508;}
  else {
    goto L1508;}}
 L1508: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, port_197X));
    goto L1552;}
  else {
    goto L1552;}}
 L1552: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_212X)
    SstatusS = v_212X;
    goto L1510;}
  else {
    goto L1510;}}
 L1510: {
  cells_213X = ((long) (Snewspace_beginS));
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_213X)>>2)), port_197X));
    goto L1570;}
  else {
    goto L1570;}}
 L1570: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_214X)
    SstatusS = v_214X;
    goto L1514;}
  else {
    goto L1514;}}
 L1514: {
  cells_215X = ((long) (ShpS));
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_215X)>>2)), port_197X));
    goto L1588;}
  else {
    goto L1588;}}
 L1588: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_216X)
    SstatusS = v_216X;
    goto L1518;}
  else {
    goto L1518;}}
 L1518: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_204X, port_197X));
    goto L1603;}
  else {
    goto L1603;}}
 L1603: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_197X, v_217X)
    SstatusS = v_217X;
    goto L1520;}
  else {
    goto L1520;}}
 L1520: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_197X, v_218X)
    SstatusS = v_218X;
    goto L1529;}
  else {
    goto L1529;}}
 L1529: {
  *((long *) (ShpS)) = 1;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_197X, ((char *) (ShpS)), (((ShpS) + 4) - (ShpS))));
    goto L1540;}
  else {
    goto L1540;}}
 L1540: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_197X, ((char *) (Snewspace_beginS)), ((ShpS) - (Snewspace_beginS))));
    goto L1547;}
  else {
    goto L1547;}}
 L1547: {
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
  goto L3094;}
 L3094: {
  addr_222X = arg0K0;
  if ((addr_222X < (ShpS))) {
    d_223X = *((long *) addr_222X);
    if ((2 == (3 & d_223X))) {
      arg1K0 = d_223X;
      goto L3113;}
    else {
      h_224X = *((long *) ((((char *) (-3 + d_223X))) + -4));
      *((long *) addr_222X) = h_224X;
      arg1K0 = h_224X;
      goto L3113;}}
  else {
    return status_219X;}}
 L3113: {
  h_225X = arg1K0;
  arg0K0 = (addr_222X + (4 + (-4 & (3 + ((long)(((unsigned long)h_225X)>>8))))));
  goto L3094;}
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
    goto L5057;}}
 L5057: {
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
      goto L5082;}
    else {
      goto L5082;}}}
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
 L5082: {
  stob_240X = *((long *) ((((char *) (-3 + pair_233X))) + 4));
  if ((3 == (3 & stob_240X))) {
    a_241X = ((char *) (-3 + stob_240X));
    if ((a_241X < (Soldspace_beginS))) {
      arg1K0 = stob_240X;
      goto L5086;}
    else {
      if ((a_241X < (Soldspace_endS))) {
        new_thing_242X = real_copy_object(stob_240X, (ShpS), &new_hp_243X);
        ShpS = new_hp_243X;
        arg1K0 = new_thing_242X;
        goto L5086;}
      else {
        arg1K0 = stob_240X;
        goto L5086;}}}
  else {
    arg1K0 = stob_240X;
    goto L5086;}}
 L5269: {
  clean_weak_pointers();
  arg1K0 = finalizer_alist_226X;
  arg1K1 = 25;
  arg1K2 = 25;
  goto L4595;}
 L5086: {
  value_244X = arg1K0;
  *((long *) ((((char *) (-3 + pair_233X))) + 4)) = value_244X;
  arg1K0 = (*((long *) ((((char *) (-3 + alist_231X))) + 4)));
  goto L5057;}
 L4595: {
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
        goto L4639;}
      else {
        if ((a_254X < (Soldspace_endS))) {
          if ((3 == (3 & (*((long *) ((((char *) (-3 + thing_252X))) + -4)))))) {
            arg2K0 = 1;
            arg1K1 = (*((long *) ((((char *) (-3 + thing_252X))) + -4)));
            goto L4639;}
          else {
            new_thing_255X = real_copy_object(thing_252X, (ShpS), &new_hp_256X);
            ShpS = new_hp_256X;
            arg2K0 = 0;
            arg1K1 = new_thing_255X;
            goto L4639;}}
        else {
          arg2K0 = 1;
          arg1K1 = thing_252X;
          goto L4639;}}}
    else {
      arg2K0 = 1;
      arg1K1 = thing_252X;
      goto L4639;}}}
 L4639: {
  tracedP_257X = arg2K0;
  thing_258X = arg1K1;
  *((long *) (((char *) (-3 + new_thing_250X)))) = thing_258X;
  *((long *) (((char *) (-3 + new_thing_248X)))) = new_thing_250X;
  if (tracedP_257X) {
    *((long *) ((((char *) (-3 + new_thing_248X))) + 4)) = okay_246X;
    arg1K0 = next_253X;
    arg1K1 = new_thing_248X;
    arg1K2 = goners_247X;
    goto L4595;}
  else {
    *((long *) ((((char *) (-3 + new_thing_248X))) + 4)) = goners_247X;
    arg1K0 = next_253X;
    arg1K1 = okay_246X;
    arg1K2 = new_thing_248X;
    goto L4595;}}
}void
s48_heap_init(void)
{
Sgc_countS = 0;
SeofPS = 0;
SstatusS = NO_ERRORS;
Sfinding_typeS = 1;
Simpure_area_countS = 0;
Spure_area_countS = 0;
Hthe_record_type7460 = 536870911;
}
