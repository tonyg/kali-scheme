#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

long copy_weak_pointer(long, long, long*);
long real_copy_object(long, long, long*);
void clean_weak_pointers(void);
void write_barrier(long, long);
void end_collection(void);
long gc_count(void);
unsigned char image_writing_okayP(void);
void register_static_areas(unsigned char, long*, long*, unsigned char, long*, long*);
long preallocate_space(long);
long heap_size(void);
void begin_collection(void);
void initialize_heap(long, long);
unsigned char availableP(long);
long allocate_space(long, long, long);
long available(void);
long check_image_header(unsigned char*);
long find_all(unsigned char);
unsigned char trace_locationsB(long, long);
long read_image(long);
long find_all_records(long);
long trace_value(long);
unsigned char trace_stob_contentsB(long);
long write_image(long, FILE *, void(*)(void));
long do_gc(long, long*);
long Sweak_pointer_limitS;
long Sweak_pointer_hpS;
long Ssaved_limitS;
long Ssaved_hpS;
long Sgc_countS;
long Sstartup_procS;
long Sold_hpS;
long Sold_beginS;
FILE * Simage_portS;
unsigned char SeofPS;
long SstatusS;
unsigned char Sfinding_typeS;
unsigned char Simpure_area_countS;
unsigned char Spure_area_countS;
long *Simpure_sizesS;
long *Spure_sizesS;
long *Simpure_areasS;
long *Spure_areasS;
long Soldspace_endS;
long Soldspace_beginS;
long Snewspace_endS;
long Snewspace_beginS;
long SlimitS;
long ShpS;
long Hthe_record_type;

long copy_weak_pointer(long weak_0X, long frontier_1X, long *TT0)
{
  long arg0K0;
  unsigned char temp_2X;
  long old_3X;
  long new_frontier_4X;
  long frontier_5X;
  long new_6X;
 {temp_2X = -1 == (Sweak_pointer_hpS);
  if (temp_2X) {
    goto L2022;}
  else {
    if (((Sweak_pointer_hpS) < (Sweak_pointer_limitS))) {
      arg0K0 = frontier_1X;
      goto L2027;}
    else {
      goto L2022;}}}
 L2022: {
  old_3X = Sweak_pointer_hpS;
  new_frontier_4X = 1024 + frontier_1X;
  Sweak_pointer_hpS = frontier_1X;
  Sweak_pointer_limitS = new_frontier_4X;
  *((long *) (Sweak_pointer_hpS)) = 261190;
  *((long *) (8 + (Sweak_pointer_hpS))) = old_3X;
  arg0K0 = new_frontier_4X;
  goto L2027;}
 L2027: {
  frontier_5X = arg0K0;
  new_6X = 7 + (Sweak_pointer_hpS);
  *((long *) (4 + (Sweak_pointer_hpS))) = (*((long *) (-3 + weak_0X)));
  Sweak_pointer_hpS = (8 + (Sweak_pointer_hpS));
  *((long *) (-7 + weak_0X)) = new_6X;
  *TT0 = frontier_5X;
  return new_6X;}}

long real_copy_object(long thing_7X, long frontier_8X, long *TT0)
{
  long a_9X;
  long h_10X;
  long descriptor_11X;
  long data_addr_12X;
  long new_13X;
 {h_10X = *((long *) (-7 + thing_7X));
  if ((3 == (3 & h_10X))) {
    *TT0 = frontier_8X;
    return h_10X;}
  else {
    if ((1074 == h_10X)) {
      descriptor_11X = *((long *) (-3 + thing_7X));
      if ((3 == (3 & descriptor_11X))) {
        a_9X = -3 + descriptor_11X;
        if ((a_9X < (Soldspace_beginS))) {
          goto L3660;}
        else {
          if ((a_9X < (Soldspace_endS))) {
            return copy_weak_pointer(thing_7X, frontier_8X, TT0);}
          else {
            goto L3660;}}}
      else {
        goto L3660;}}
    else {
      goto L3660;}}}
 L3660: {
  *((long *) frontier_8X) = h_10X;
  data_addr_12X = 4 + frontier_8X;
  new_13X = 3 + data_addr_12X;
  *((long *) (-7 + thing_7X)) = new_13X;
  memcpy((void *)data_addr_12X, (void *)(-3 + thing_7X),((long)(((unsigned long)h_10X)>>8)));
  *TT0 = (data_addr_12X + (-4 & (3 + ((long)(((unsigned long)h_10X)>>8)))));
  return new_13X;}}

void clean_weak_pointers(void)
{
  long arg0K1;
  long arg0K0;
  long end_14X;
  long x_15X;
  long x2_16X;
  long scan_17X;
  long value_18X;
  long a_19X;
  long h_20X;
  long start_21X;
  long end_22X;
  long next_23X;
 {if ((-1 == (Sweak_pointer_hpS))) {
    return;}
  else {
    x_15X = Sweak_pointer_limitS;
    end_14X = Sweak_pointer_hpS;
    arg0K0 = (-1024 + x_15X);
    arg0K1 = end_14X;
    goto L4609;}}
 L4609: {
  start_21X = arg0K0;
  end_22X = arg0K1;
  next_23X = *((long *) (8 + start_21X));
  arg0K0 = start_21X;
  goto L2825;}
 L2825: {
  scan_17X = arg0K0;
  if ((scan_17X < end_22X)) {
    *((long *) scan_17X) = 1074;
    value_18X = *((long *) (4 + scan_17X));
    if ((3 == (3 & value_18X))) {
      a_19X = -3 + value_18X;
      if ((a_19X < (Soldspace_beginS))) {
        goto L2871;}
      else {
        if ((a_19X < (Soldspace_endS))) {
          if ((3 == (3 & value_18X))) {
            h_20X = *((long *) (-7 + value_18X));
            if ((3 == (3 & h_20X))) {
              arg0K0 = h_20X;
              goto L2866;}
            else {
              arg0K0 = 1;
              goto L2866;}}
          else {
            goto L2871;}}
        else {
          goto L2871;}}}
    else {
      goto L2871;}}
  else {
    if ((-1 == next_23X)) {
      if ((end_14X < (Sweak_pointer_limitS))) {
        *((long *) end_14X) = (70 + ((((-4 & (-4 + ((Sweak_pointer_limitS) - end_14X))))<<8)));
        return;}
      else {
        return;}}
    else {
      arg0K0 = (-1024 + next_23X);
      arg0K1 = next_23X;
      goto L4609;}}}
 L2871: {
  arg0K0 = (8 + scan_17X);
  goto L2825;}
 L2866: {
  x2_16X = arg0K0;
  *((long *) (4 + scan_17X)) = x2_16X;
  goto L2871;}}

void write_barrier(long address_24X, long value_25X)
{

 {return;}}

void end_collection(void)
{

 {Sgc_countS = (1 + (Sgc_countS));
  return;}}

long gc_count(void)
{

 {return (Sgc_countS);}}

unsigned char image_writing_okayP(void)
{

 {if ((0 == (((unsigned long) (Spure_area_countS))))) {
    return (0 == (((unsigned long) (Simpure_area_countS))));}
  else {
    return 0;}}}

void register_static_areas(unsigned char pure_count_26X, long *pure_areas_27X, long *pure_sizes_28X, unsigned char impure_count_29X, long *impure_areas_30X, long *impure_sizes_31X)
{

 {Spure_area_countS = pure_count_26X;
  Spure_areasS = pure_areas_27X;
  Spure_sizesS = pure_sizes_28X;
  Simpure_area_countS = impure_count_29X;
  Simpure_areasS = impure_areas_30X;
  Simpure_sizesS = impure_sizes_31X;
  return;}}

long preallocate_space(long cells_32X)
{

 {return 0;}}

long heap_size(void)
{

 {return ((Snewspace_endS) - (Snewspace_beginS));}}

void begin_collection(void)
{
  long b_33X;
  long e_34X;
 {b_33X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_33X;
  e_34X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_34X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = -1;
  return;}}

void initialize_heap(long start_35X, long size_36X)
{
  long cells_37X;
  long semisize_38X;
 {cells_37X = size_36X / 2;
  semisize_38X = ((cells_37X)<<2);
  Snewspace_beginS = start_35X;
  Snewspace_endS = ((Snewspace_beginS) + semisize_38X);
  Soldspace_beginS = (Snewspace_endS);
  Soldspace_endS = ((Soldspace_beginS) + semisize_38X);
  ShpS = (Snewspace_beginS);
  SlimitS = (Snewspace_endS);
  return;}}

unsigned char availableP(long cells_39X)
{

 {return (((ShpS) + (((cells_39X)<<2))) < (SlimitS));}}

long allocate_space(long type_40X, long len_41X, long key_42X)
{
  long new_43X;
 {new_43X = ShpS;
  ShpS = ((ShpS) + (-4 & (3 + len_41X)));
  return new_43X;}}

long available(void)
{

 {return (((((SlimitS) - (ShpS)))>>2));}}

long check_image_header(unsigned char *filename_44X)
{
  signed char arg2K0;
  long arg0K0;
  long arg0K2;
  long arg0K1;
  unsigned char arg1K1;
  unsigned char arg1K0;
  long status_45X;
  FILE * port_46X;
  unsigned char eofP_47X;
  long status_48X;
  unsigned char ch_49X;
  unsigned char eofP_50X;
  long status_51X;
  unsigned char eofP_52X;
  long status_53X;
  unsigned char ch_54X;
  unsigned char eofP_55X;
  long status_56X;
  unsigned char thing_57X;
  unsigned char eofP_58X;
  long status_59X;
  long i_60X;
  long status_61X;
  unsigned char eofP_62X;
  unsigned char ch_63X;
  unsigned char eofP_64X;
  long status_65X;
  unsigned char ch_66X;
  unsigned char eofP_67X;
  long status_68X;
  signed char v_69X;
  long status_70X;
  signed char v_71X;
  long status_72X;
  signed char v_73X;
  long status_74X;
  signed char v_75X;
  long status_76X;
  long v_77X;
  long cells_78X;
  long thing_79X;
  unsigned char eofP_80X;
  long status_81X;
  long cells_82X;
  long thing_83X;
  unsigned char eofP_84X;
  long status_85X;
  long old_bytes_per_cell_86X;
  long thing_87X;
  unsigned char eofP_88X;
  long status_89X;
  unsigned char same_versionP_90X;
  long thing_91X;
  unsigned char eofP_92X;
  long status_93X;
 {port_46X = ps_open_input_file(filename_44X, &status_45X);
  if ((status_45X == NO_ERRORS)) {
    SstatusS = NO_ERRORS;
    SeofPS = 0;
    if ((SeofPS)) {
      goto L2191;}
    else {
      goto L2772;}}
  else {
    ps_write_string(((unsigned char *) "Can't open heap image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    ps_write_string((ps_error_string(status_45X)), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2191: {
  if ((SeofPS)) {
    goto L2224;}
  else {
    goto L2736;}}
 L2772: {
  PS_READ_CHAR(port_46X, ch_49X, eofP_50X, status_51X)
  if (eofP_50X) {
    arg1K0 = eofP_50X;
    arg0K1 = status_51X;
    goto L2169;}
  else {
    if ((status_51X == NO_ERRORS)) {
      if ((12 == ch_49X)) {
        arg1K0 = 0;
        arg0K1 = status_51X;
        goto L2169;}
      else {
        goto L2772;}}
    else {
      arg1K0 = eofP_50X;
      arg0K1 = status_51X;
      goto L2169;}}}
 L2224: {
  if ((SeofPS)) {
    arg1K0 = 0;
    goto L2257;}
  else {
    arg0K0 = 0;
    goto L2712;}}
 L2736: {
  PS_READ_CHAR(port_46X, ch_54X, eofP_55X, status_56X)
  if (eofP_55X) {
    arg1K0 = eofP_55X;
    arg0K1 = status_56X;
    goto L2202;}
  else {
    if ((status_56X == NO_ERRORS)) {
      if ((10 == ch_54X)) {
        arg1K0 = 0;
        arg0K1 = status_56X;
        goto L2202;}
      else {
        goto L2736;}}
    else {
      arg1K0 = eofP_55X;
      arg0K1 = status_56X;
      goto L2202;}}}
 L2169: {
  eofP_47X = arg1K0;
  status_48X = arg0K1;
  if (eofP_47X) {
    SeofPS = 1;
    goto L2191;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2191;}
    else {
      SeofPS = 1;
      SstatusS = status_48X;
      goto L2191;}}}
 L2257: {
  same_versionP_90X = arg1K0;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2292;}
  else {
    PS_READ_INTEGER(port_46X, thing_91X, eofP_92X, status_93X)
    if (eofP_92X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2292;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_91X;
        goto L2292;}
      else {
        SeofPS = 1;
        SstatusS = status_93X;
        arg0K0 = -1;
        goto L2292;}}}}
 L2712: {
  i_60X = arg0K0;
  PS_READ_CHAR(port_46X, ch_63X, eofP_62X, status_61X)
  if (eofP_62X) {
    arg1K0 = 0;
    arg1K1 = eofP_62X;
    arg0K2 = status_61X;
    goto L2235;}
  else {
    if ((status_61X == NO_ERRORS)) {
      if ((i_60X == (strlen(((unsigned char *) "Vanilla 18"))))) {
        arg1K0 = (10 == ch_63X);
        arg1K1 = 0;
        arg0K2 = status_61X;
        goto L2235;}
      else {
        if ((ch_63X == (*(((unsigned char *) "Vanilla 18") + i_60X)))) {
          arg0K0 = (1 + i_60X);
          goto L2712;}
        else {
          arg1K0 = 0;
          arg1K1 = 0;
          arg0K2 = status_61X;
          goto L2235;}}}
    else {
      arg1K0 = 0;
      arg1K1 = eofP_62X;
      arg0K2 = status_61X;
      goto L2235;}}}
 L2202: {
  eofP_52X = arg1K0;
  status_53X = arg0K1;
  if (eofP_52X) {
    SeofPS = 1;
    goto L2224;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2224;}
    else {
      SeofPS = 1;
      SstatusS = status_53X;
      goto L2224;}}}
 L2292: {
  old_bytes_per_cell_86X = arg0K0;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2327;}
  else {
    PS_READ_INTEGER(port_46X, thing_87X, eofP_88X, status_89X)
    if (eofP_88X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2327;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_87X;
        goto L2327;}
      else {
        SeofPS = 1;
        SstatusS = status_89X;
        arg0K0 = -1;
        goto L2327;}}}}
 L2235: {
  thing_57X = arg1K0;
  eofP_58X = arg1K1;
  status_59X = arg0K2;
  if (eofP_58X) {
    SeofPS = 1;
    arg1K0 = 0;
    goto L2257;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      arg1K0 = thing_57X;
      goto L2257;}
    else {
      SeofPS = 1;
      SstatusS = status_59X;
      arg1K0 = 0;
      goto L2257;}}}
 L2327: {
  cells_82X = arg0K0;
  Sold_beginS = (((cells_82X)<<2));
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2365;}
  else {
    PS_READ_INTEGER(port_46X, thing_83X, eofP_84X, status_85X)
    if (eofP_84X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2365;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_83X;
        goto L2365;}
      else {
        SeofPS = 1;
        SstatusS = status_85X;
        arg0K0 = -1;
        goto L2365;}}}}
 L2365: {
  cells_78X = arg0K0;
  Sold_hpS = (((cells_78X)<<2));
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2403;}
  else {
    PS_READ_INTEGER(port_46X, thing_79X, eofP_80X, status_81X)
    if (eofP_80X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2403;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_79X;
        goto L2403;}
      else {
        SeofPS = 1;
        SstatusS = status_81X;
        arg0K0 = -1;
        goto L2403;}}}}
 L2403: {
  v_77X = arg0K0;
  Sstartup_procS = v_77X;
  Simage_portS = port_46X;
  if ((SeofPS)) {
    goto L2442;}
  else {
    goto L2659;}}
 L2442: {
  if (((SstatusS) == NO_ERRORS)) {
    if ((SeofPS)) {
      SstatusS = EDOM;
      ps_write_string(((unsigned char *) "Premature EOF when reading image file"), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L2531;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L2531;}}
    else {
      if (same_versionP_90X) {
        if ((4 == old_bytes_per_cell_86X)) {
          return ((Sold_hpS) - (Sold_beginS));}
        else {
          ps_write_string(((unsigned char *) "Incompatible bytes-per-cell in image"), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L2591;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L2591;}}}
      else {
        ps_write_string(((unsigned char *) "Format of image is incompatible with this version of system"), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2561;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2561;}}}}
  else {
    ps_write_string(((unsigned char *) "Error reading from image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2506;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2506;}}}
 L2659: {
  PS_READ_CHAR(port_46X, ch_66X, eofP_67X, status_68X)
  if (eofP_67X) {
    arg1K0 = eofP_67X;
    arg0K1 = status_68X;
    goto L2420;}
  else {
    if ((status_68X == NO_ERRORS)) {
      if ((12 == ch_66X)) {
        arg1K0 = 0;
        arg0K1 = status_68X;
        goto L2420;}
      else {
        goto L2659;}}
    else {
      arg1K0 = eofP_67X;
      arg0K1 = status_68X;
      goto L2420;}}}
 L2531: {
  status_74X = ps_close(port_46X);
  if ((status_74X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L2454;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L2454;}}
 L2591: {
  status_70X = ps_close(port_46X);
  if ((status_70X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L2466;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L2466;}}
 L2561: {
  status_72X = ps_close(port_46X);
  if ((status_72X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L2460;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L2460;}}
 L2506: {
  status_76X = ps_close(port_46X);
  if ((status_76X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L2447;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L2447;}}
 L2420: {
  eofP_64X = arg1K0;
  status_65X = arg0K1;
  if (eofP_64X) {
    SeofPS = 1;
    goto L2442;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2442;}
    else {
      SeofPS = 1;
      SstatusS = status_65X;
      goto L2442;}}}
 L2454: {
  v_73X = arg2K0;
  return (((long) v_73X));}
 L2466: {
  v_69X = arg2K0;
  return (((long) v_69X));}
 L2460: {
  v_71X = arg2K0;
  return (((long) v_71X));}
 L2447: {
  v_75X = arg2K0;
  return (((long) v_75X));}}

long find_all(unsigned char type_94X)
{
  long arg0K0;
  long merged_arg0K1;
  long merged_arg0K0;

  int Hproc95_return_tag;
  unsigned char Hproc950_return_value;
  unsigned char v_96X;
  long start_hp_97X;
  long *areas_98X;
  long *sizes_99X;
  unsigned char count_100X;
  long i_101X;
  unsigned char v_102X;
  long i_103X;
  unsigned char v_104X;
  long *areas_105X;
  long *sizes_106X;
  unsigned char count_107X;
  unsigned char type_108X;
  long addr_109X;
  long d_110X;
  long start_111X;
  long end_112X;
 {Sfinding_typeS = type_94X;
  start_hp_97X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = (4 + (ShpS));
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_97X;
  Hproc95_return_tag = 0;
  goto Hproc95;
 Hproc95_return_0:
  v_96X = Hproc950_return_value;
  if (v_96X) {
    areas_98X = Simpure_areasS;
    sizes_99X = Simpure_sizesS;
    count_100X = Simpure_area_countS;
    arg0K0 = 0;
    goto L4171;}
  else {
    goto L4127;}}
 L4171: {
  i_103X = arg0K0;
  if ((i_103X < (((unsigned long) count_100X)))) {
    merged_arg0K0 = (*(areas_98X + i_103X));
    merged_arg0K1 = ((*(areas_98X + i_103X)) + (*(sizes_99X + i_103X)));
    Hproc95_return_tag = 1;
    goto Hproc95;
   Hproc95_return_1:
    v_104X = Hproc950_return_value;
    if (v_104X) {
      arg0K0 = (1 + i_103X);
      goto L4171;}
    else {
      goto L4127;}}
  else {
    areas_105X = Spure_areasS;
    sizes_106X = Spure_sizesS;
    count_107X = Spure_area_countS;
    arg0K0 = 0;
    goto L4190;}}
 L4127: {
  ShpS = start_hp_97X;
  return 1;}
 L4190: {
  i_101X = arg0K0;
  if ((i_101X < (((unsigned long) count_107X)))) {
    merged_arg0K0 = (*(areas_105X + i_101X));
    merged_arg0K1 = ((*(areas_105X + i_101X)) + (*(sizes_106X + i_101X)));
    Hproc95_return_tag = 2;
    goto Hproc95;
   Hproc95_return_2:
    v_102X = Hproc950_return_value;
    if (v_102X) {
      arg0K0 = (1 + i_101X);
      goto L4190;}
    else {
      goto L4127;}}
  else {
    *((long *) start_hp_97X) = (-1014 + (((((ShpS) - start_hp_97X))<<8)));
    return (7 + start_hp_97X);}}
 Hproc95: {
  start_111X = merged_arg0K0;
  end_112X = merged_arg0K1;{
  type_108X = Sfinding_typeS;
  arg0K0 = start_111X;
  goto L3314;}
 L3314: {
  addr_109X = arg0K0;
  if ((addr_109X < end_112X)) {
    d_110X = *((long *) addr_109X);
    if ((2 == (3 & d_110X))) {
      if (((((unsigned long) type_108X)) == (31 & (((d_110X)>>2))))) {
        if (((16 + (ShpS)) < (SlimitS))) {
          *((long *) (ShpS)) = (7 + addr_109X);
          ShpS = (4 + (ShpS));
          goto L3348;}
        else {
          Hproc950_return_value = 0;
          goto Hproc95_return;}}
      else {
        goto L3348;}}
    else {
      ps_write_string(((unsigned char *) "heap is in an inconsistent state."), (stderr));
      Hproc950_return_value = 0;
      goto Hproc95_return;}}
  else {
    Hproc950_return_value = 1;
    goto Hproc95_return;}}
 L3348: {
  arg0K0 = (4 + (addr_109X + (-4 & (3 + ((long)(((unsigned long)d_110X)>>8))))));
  goto L3314;}
 Hproc95_return:
  switch (Hproc95_return_tag) {
  case 0: goto Hproc95_return_0;
  case 1: goto Hproc95_return_1;
  default: goto Hproc95_return_2;
  }}
}

unsigned char trace_locationsB(long start_113X, long end_114X)
{
  long arg0K1;
  long arg0K0;
  long new_thing_115X;
  long frontier_116X;
  long data_addr_117X;
  long new_118X;
  long new_thing_119X;
  long frontier_120X;
  long descriptor_121X;
  long a_122X;
  long h_123X;
  long a_124X;
  long addr_125X;
  long frontier_126X;
  long thing_127X;
  long next_128X;
 {arg0K0 = start_113X;
  arg0K1 = (ShpS);
  goto L4419;}
 L4419: {
  addr_125X = arg0K0;
  frontier_126X = arg0K1;
  if ((addr_125X < end_114X)) {
    thing_127X = *((long *) addr_125X);
    next_128X = 4 + addr_125X;
    if ((2 == (3 & thing_127X))) {
      if (((31 & (((thing_127X)>>2))) < 16)) {
        goto L4437;}
      else {
        arg0K0 = (next_128X + (-4 & (3 + ((long)(((unsigned long)thing_127X)>>8)))));
        arg0K1 = frontier_126X;
        goto L4419;}}
    else {
      goto L4437;}}
  else {
    ShpS = frontier_126X;
    return 0;}}
 L4437: {
  if ((3 == (3 & thing_127X))) {
    a_122X = -3 + thing_127X;
    if ((a_122X < (Soldspace_beginS))) {
      arg0K0 = next_128X;
      arg0K1 = frontier_126X;
      goto L4419;}
    else {
      if ((a_122X < (Soldspace_endS))) {
        h_123X = *((long *) (-7 + thing_127X));
        if ((3 == (3 & h_123X))) {
          arg0K0 = h_123X;
          arg0K1 = frontier_126X;
          goto L4444;}
        else {
          if ((1074 == h_123X)) {
            descriptor_121X = *((long *) (-3 + thing_127X));
            if ((3 == (3 & descriptor_121X))) {
              a_124X = -3 + descriptor_121X;
              if ((a_124X < (Soldspace_beginS))) {
                goto L5465;}
              else {
                if ((a_124X < (Soldspace_endS))) {
                  new_thing_119X = copy_weak_pointer(thing_127X, frontier_126X, &frontier_120X);
                  arg0K0 = new_thing_119X;
                  arg0K1 = frontier_120X;
                  goto L4444;}
                else {
                  goto L5465;}}}
            else {
              goto L5465;}}
          else {
            goto L5465;}}}
      else {
        arg0K0 = next_128X;
        arg0K1 = frontier_126X;
        goto L4419;}}}
  else {
    arg0K0 = next_128X;
    arg0K1 = frontier_126X;
    goto L4419;}}
 L4444: {
  new_thing_115X = arg0K0;
  frontier_116X = arg0K1;
  *((long *) addr_125X) = new_thing_115X;
  arg0K0 = next_128X;
  arg0K1 = frontier_116X;
  goto L4419;}
 L5465: {
  *((long *) frontier_126X) = h_123X;
  data_addr_117X = 4 + frontier_126X;
  new_118X = 3 + data_addr_117X;
  *((long *) (-7 + thing_127X)) = new_118X;
  memcpy((void *)data_addr_117X, (void *)(-3 + thing_127X),((long)(((unsigned long)h_123X)>>8)));
  arg0K0 = new_118X;
  arg0K1 = (data_addr_117X + (-4 & (3 + ((long)(((unsigned long)h_123X)>>8)))));
  goto L4444;}}

long read_image(long startup_space_129X)
{
  FILE * arg4K0;
  unsigned char *arg3K1;
  unsigned char arg1K1;
  unsigned char arg1K0;
  signed char arg2K0;
  long arg0K2;
  long arg0K0;
  long got_130X;
  unsigned char eofP_131X;
  long status_132X;
  FILE * port_133X;
  long ptr_134X;
  long value_135X;
  unsigned char x_136X;
  unsigned char x_137X;
  long d_138X;
  long ptr_139X;
  long descriptor_140X;
  signed char v_141X;
  long status_142X;
  signed char v_143X;
  long status_144X;
  unsigned char okayP_145X;
  unsigned char *string_146X;
  long status_147X;
  signed char v_148X;
  long status_149X;
  long descriptor_150X;
  FILE * port_151X;
  unsigned char reverseP_152X;
  long startup_space_153X;
  long got_154X;
  unsigned char eofP_155X;
  long status_156X;
  long start_157X;
  long new_limit_158X;
  long new_hp_159X;
  long delta_160X;
  signed char v_161X;
  long status_162X;
  signed char v_163X;
  long status_164X;
  unsigned char okayP_165X;
  unsigned char *string_166X;
  unsigned char x_167X;
  unsigned char x_168X;
  long addr_169X;
 {port_133X = Simage_portS;
  got_130X = ps_read_block(port_133X, ((char *) (ShpS)), 4, &eofP_131X, &status_132X);
  if ((status_132X == NO_ERRORS)) {
    if (eofP_131X) {
      arg1K0 = 0;
      arg3K1 = ((unsigned char *) "Premature EOF when reading image file");
      goto L4707;}
    else {
      if ((got_130X < 4)) {
        arg1K0 = 0;
        arg3K1 = ((unsigned char *) "Read returned too few bytes");
        goto L4707;}
      else {
        arg1K0 = 1;
        arg3K1 = ((unsigned char *) "");
        goto L4707;}}}
  else {
    SstatusS = status_132X;
    arg1K0 = 0;
    arg3K1 = ((unsigned char *) "Error reading from image file");
    goto L4707;}}
 L4707: {
  okayP_165X = arg1K0;
  string_166X = arg3K1;
  if (okayP_165X) {
    if ((1 == (*((long *) (ShpS))))) {
      arg4K0 = port_133X;
      arg1K1 = 0;
      arg0K2 = startup_space_129X;
      goto L3848;}
    else {
      addr_169X = ShpS;
      x_168X = *((unsigned char *) addr_169X);
      *((unsigned char *) addr_169X) = (*((unsigned char *) (3 + addr_169X)));
      *((unsigned char *) (3 + addr_169X)) = x_168X;
      x_167X = *((unsigned char *) (1 + addr_169X));
      *((unsigned char *) (1 + addr_169X)) = (*((unsigned char *) (2 + addr_169X)));
      *((unsigned char *) (2 + addr_169X)) = x_167X;
      if ((1 == (*((long *) (ShpS))))) {
        arg4K0 = port_133X;
        arg1K1 = 1;
        arg0K2 = startup_space_129X;
        goto L3848;}
      else {
        ps_write_string(((unsigned char *) "Unable to correct byte order"), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L4807;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L4807;}}}}
  else {
    ps_write_string(string_166X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4761;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4761;}}}
 L3848: {
  port_151X = arg4K0;
  reverseP_152X = arg1K1;
  startup_space_153X = arg0K2;
  delta_160X = (ShpS) - (Sold_beginS);
  new_hp_159X = (Sold_hpS) + delta_160X;
  new_limit_158X = Snewspace_endS;
  start_157X = ShpS;
  if (((startup_space_153X + new_hp_159X) < new_limit_158X)) {
    got_154X = ps_read_block(port_151X, ((char *) (ShpS)), ((Sold_hpS) - (Sold_beginS)), &eofP_155X, &status_156X);
    if ((status_156X == NO_ERRORS)) {
      if (eofP_155X) {
        arg1K0 = 0;
        arg3K1 = ((unsigned char *) "Premature EOF when reading image file");
        goto L3866;}
      else {
        if ((got_154X < 4)) {
          arg1K0 = 0;
          arg3K1 = ((unsigned char *) "Read returned too few bytes");
          goto L3866;}
        else {
          arg1K0 = 1;
          arg3K1 = ((unsigned char *) "");
          goto L3866;}}}
    else {
      SstatusS = status_156X;
      arg1K0 = 0;
      arg3K1 = ((unsigned char *) "Error reading from image file");
      goto L3866;}}
  else {
    ps_write_string(((unsigned char *) "Heap not big enough to restore this image"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L3918;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L3918;}}}
 L4807: {
  status_162X = ps_close(port_133X);
  if ((status_162X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L4729;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L4729;}}
 L4761: {
  status_164X = ps_close(port_133X);
  if ((status_164X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L4716;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L4716;}}
 L3866: {
  okayP_145X = arg1K0;
  string_146X = arg3K1;
  if (okayP_145X) {
    status_147X = ps_close(port_151X);
    if ((status_147X == NO_ERRORS)) {
      if (reverseP_152X) {
        ps_write_string(((unsigned char *) "Correcting byte order of resumed image."), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        arg0K0 = start_157X;
        goto L3199;}
      else {
        goto L3890;}}
    else {
      ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L3993;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L3993;}}}
  else {
    ps_write_string(string_146X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L3963;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L3963;}}}
 L3918: {
  status_149X = ps_close(port_151X);
  if ((status_149X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L3861;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L3861;}}
 L4729: {
  v_161X = arg2K0;
  return (((long) v_161X));}
 L4716: {
  v_163X = arg2K0;
  return (((long) v_163X));}
 L3199: {
  ptr_134X = arg0K0;
  x_137X = *((unsigned char *) ptr_134X);
  *((unsigned char *) ptr_134X) = (*((unsigned char *) (3 + ptr_134X)));
  *((unsigned char *) (3 + ptr_134X)) = x_137X;
  x_136X = *((unsigned char *) (1 + ptr_134X));
  *((unsigned char *) (1 + ptr_134X)) = (*((unsigned char *) (2 + ptr_134X)));
  *((unsigned char *) (2 + ptr_134X)) = x_136X;
  value_135X = *((long *) ptr_134X);
  if ((ptr_134X < new_hp_159X)) {
    if ((2 == (3 & value_135X))) {
      if (((31 & (((value_135X)>>2))) < 16)) {
        goto L3220;}
      else {
        arg0K0 = ((4 + ptr_134X) + (-4 & (3 + ((long)(((unsigned long)value_135X)>>8)))));
        goto L3199;}}
    else {
      goto L3220;}}
  else {
    goto L3890;}}
 L3890: {
  if ((0 == delta_160X)) {
    goto L3900;}
  else {
    arg0K0 = start_157X;
    goto L3092;}}
 L3993: {
  status_142X = ps_close(port_151X);
  if ((status_142X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L3881;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L3881;}}
 L3963: {
  status_144X = ps_close(port_151X);
  if ((status_144X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L3875;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L3875;}}
 L3861: {
  v_148X = arg2K0;
  return (((long) v_148X));}
 L3220: {
  arg0K0 = (4 + ptr_134X);
  goto L3199;}
 L3900: {
  ShpS = new_hp_159X;
  descriptor_150X = Sstartup_procS;
  if ((3 == (3 & descriptor_150X))) {
    return (3 + ((-3 + descriptor_150X) + delta_160X));}
  else {
    return descriptor_150X;}}
 L3092: {
  ptr_139X = arg0K0;
  if ((ptr_139X < new_hp_159X)) {
    descriptor_140X = *((long *) ptr_139X);
    if ((3 == (3 & descriptor_140X))) {
      arg0K0 = (3 + ((-3 + descriptor_140X) + delta_160X));
      goto L3099;}
    else {
      arg0K0 = descriptor_140X;
      goto L3099;}}
  else {
    goto L3900;}}
 L3881: {
  v_141X = arg2K0;
  return (((long) v_141X));}
 L3875: {
  v_143X = arg2K0;
  return (((long) v_143X));}
 L3099: {
  d_138X = arg0K0;
  *((long *) ptr_139X) = d_138X;
  if ((2 == (3 & d_138X))) {
    if (((31 & (((d_138X)>>2))) < 16)) {
      goto L3115;}
    else {
      arg0K0 = ((4 + ptr_139X) + (-4 & (3 + ((long)(((unsigned long)d_138X)>>8)))));
      goto L3092;}}
  else {
    goto L3115;}}
 L3115: {
  arg0K0 = (4 + ptr_139X);
  goto L3092;}}

long find_all_records(long record_type_170X)
{
  long arg0K0;
  long merged_arg0K1;
  long merged_arg0K0;

  int Hproc171_return_tag;
  unsigned char Hproc1710_return_value;
  unsigned char v_172X;
  long start_hp_173X;
  long *areas_174X;
  long *sizes_175X;
  unsigned char count_176X;
  long i_177X;
  unsigned char v_178X;
  long i_179X;
  unsigned char v_180X;
  long *areas_181X;
  long *sizes_182X;
  unsigned char count_183X;
  unsigned char type_184X;
  long addr_185X;
  long d_186X;
  long record_187X;
  long start_188X;
  long end_189X;
 {Hthe_record_type = record_type_170X;
  Sfinding_typeS = 8;
  start_hp_173X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = (4 + (ShpS));
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_173X;
  Hproc171_return_tag = 0;
  goto Hproc171;
 Hproc171_return_0:
  v_172X = Hproc1710_return_value;
  if (v_172X) {
    areas_174X = Simpure_areasS;
    sizes_175X = Simpure_sizesS;
    count_176X = Simpure_area_countS;
    arg0K0 = 0;
    goto L4324;}
  else {
    goto L4280;}}
 L4324: {
  i_179X = arg0K0;
  if ((i_179X < (((unsigned long) count_176X)))) {
    merged_arg0K0 = (*(areas_174X + i_179X));
    merged_arg0K1 = ((*(areas_174X + i_179X)) + (*(sizes_175X + i_179X)));
    Hproc171_return_tag = 1;
    goto Hproc171;
   Hproc171_return_1:
    v_180X = Hproc1710_return_value;
    if (v_180X) {
      arg0K0 = (1 + i_179X);
      goto L4324;}
    else {
      goto L4280;}}
  else {
    areas_181X = Spure_areasS;
    sizes_182X = Spure_sizesS;
    count_183X = Spure_area_countS;
    arg0K0 = 0;
    goto L4343;}}
 L4280: {
  ShpS = start_hp_173X;
  return 1;}
 L4343: {
  i_177X = arg0K0;
  if ((i_177X < (((unsigned long) count_183X)))) {
    merged_arg0K0 = (*(areas_181X + i_177X));
    merged_arg0K1 = ((*(areas_181X + i_177X)) + (*(sizes_182X + i_177X)));
    Hproc171_return_tag = 2;
    goto Hproc171;
   Hproc171_return_2:
    v_178X = Hproc1710_return_value;
    if (v_178X) {
      arg0K0 = (1 + i_177X);
      goto L4343;}
    else {
      goto L4280;}}
  else {
    *((long *) start_hp_173X) = (-1014 + (((((ShpS) - start_hp_173X))<<8)));
    return (7 + start_hp_173X);}}
 Hproc171: {
  start_188X = merged_arg0K0;
  end_189X = merged_arg0K1;{
  type_184X = Sfinding_typeS;
  arg0K0 = start_188X;
  goto L3473;}
 L3473: {
  addr_185X = arg0K0;
  if ((addr_185X < end_189X)) {
    d_186X = *((long *) addr_185X);
    if ((2 == (3 & d_186X))) {
      if (((((unsigned long) type_184X)) == (31 & (((d_186X)>>2))))) {
        record_187X = 7 + addr_185X;
        if (((Hthe_record_type) == (*((long *) (-3 + record_187X))))) {
          if (((16 + (ShpS)) < (SlimitS))) {
            *((long *) (ShpS)) = record_187X;
            ShpS = (4 + (ShpS));
            goto L3507;}
          else {
            Hproc1710_return_value = 0;
            goto Hproc171_return;}}
        else {
          goto L3507;}}
      else {
        goto L3507;}}
    else {
      ps_write_string(((unsigned char *) "heap is in an inconsistent state."), (stderr));
      Hproc1710_return_value = 0;
      goto Hproc171_return;}}
  else {
    Hproc1710_return_value = 1;
    goto Hproc171_return;}}
 L3507: {
  arg0K0 = (4 + (addr_185X + (-4 & (3 + ((long)(((unsigned long)d_186X)>>8))))));
  goto L3473;}
 Hproc171_return:
  switch (Hproc171_return_tag) {
  case 0: goto Hproc171_return_0;
  case 1: goto Hproc171_return_1;
  default: goto Hproc171_return_2;
  }}
}

long trace_value(long stob_190X)
{
  long new_thing_191X;
  long new_hp_192X;
  long a_193X;
 {if ((3 == (3 & stob_190X))) {
    a_193X = -3 + stob_190X;
    if ((a_193X < (Soldspace_beginS))) {
      return stob_190X;}
    else {
      if ((a_193X < (Soldspace_endS))) {
        new_thing_191X = real_copy_object(stob_190X, (ShpS), &new_hp_192X);
        ShpS = new_hp_192X;
        return new_thing_191X;}
      else {
        return stob_190X;}}}
  else {
    return stob_190X;}}}

unsigned char trace_stob_contentsB(long stob_194X)
{
  long h_195X;
  long start_196X;
 {h_195X = *((long *) (-7 + stob_194X));
  start_196X = -3 + stob_194X;
  return trace_locationsB(start_196X, (start_196X + (-4 & (3 + ((long)(((unsigned long)h_195X)>>8))))));}}

long write_image(long resume_proc_197X, FILE * port_198X, void (*mark_traced_channels_closedB_199X)(void))
{
  long arg0K0;
  long new_thing_200X;
  long new_hp_201X;
  long e_202X;
  long b_203X;
  long a_204X;
  long h_205X;
  long addr_206X;
  long h_207X;
  long d_208X;
  long v_209X;
  long v_210X;
  long v_211X;
  long cells_212X;
  long v_213X;
  long cells_214X;
  long v_215X;
  long status_216X;
  long b_217X;
  long e_218X;
  long v_219X;
  long v_220X;
  long v_221X;
  long v_222X;
  long start_223X;
  long end_224X;
  long resume_proc_225X;
  long start_226X;
 {b_203X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_203X;
  e_202X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_202X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = -1;
  if ((3 == (3 & resume_proc_197X))) {
    a_204X = -3 + resume_proc_197X;
    if ((a_204X < (Soldspace_beginS))) {
      arg0K0 = resume_proc_197X;
      goto L5312;}
    else {
      if ((a_204X < (Soldspace_endS))) {
        new_thing_200X = real_copy_object(resume_proc_197X, (ShpS), &new_hp_201X);
        ShpS = new_hp_201X;
        arg0K0 = new_thing_200X;
        goto L5312;}
      else {
        arg0K0 = resume_proc_197X;
        goto L5312;}}}
  else {
    arg0K0 = resume_proc_197X;
    goto L5312;}}
 L5312: {
  resume_proc_225X = arg0K0;
  start_226X = Snewspace_beginS;
  arg0K0 = start_226X;
  goto L5364;}
 L5364: {
  start_223X = arg0K0;
  end_224X = ShpS;
  trace_locationsB(start_223X, end_224X);
  if (((ShpS) < (SlimitS))) {
    if ((end_224X < (ShpS))) {
      arg0K0 = end_224X;
      goto L5364;}
    else {
      goto L5316;}}
  else {
    ps_error(((unsigned char *) "GC error: ran out of space in new heap"), 0);
    goto L5316;}}
 L5316: {
  clean_weak_pointers();
  mark_traced_channels_closedB_199X();
  SstatusS = NO_ERRORS;
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_222X)
    SstatusS = v_222X;
    goto L1493;}
  else {
    goto L1493;}}
 L1493: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_198X, v_221X)
    SstatusS = v_221X;
    goto L1502;}
  else {
    goto L1502;}}
 L1502: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_220X)
    SstatusS = v_220X;
    goto L1511;}
  else {
    goto L1511;}}
 L1511: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string(((unsigned char *) "Vanilla 18"), port_198X));
    goto L1518;}
  else {
    goto L1518;}}
 L1518: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_219X)
    SstatusS = v_219X;
    goto L1527;}
  else {
    goto L1527;}}
 L1527: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, port_198X));
    goto L1571;}
  else {
    goto L1571;}}
 L1571: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_215X)
    SstatusS = v_215X;
    goto L1529;}
  else {
    goto L1529;}}
 L1529: {
  cells_214X = Snewspace_beginS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_214X)>>2)), port_198X));
    goto L1589;}
  else {
    goto L1589;}}
 L1589: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_213X)
    SstatusS = v_213X;
    goto L1533;}
  else {
    goto L1533;}}
 L1533: {
  cells_212X = ShpS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_212X)>>2)), port_198X));
    goto L1607;}
  else {
    goto L1607;}}
 L1607: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_211X)
    SstatusS = v_211X;
    goto L1537;}
  else {
    goto L1537;}}
 L1537: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_225X, port_198X));
    goto L1622;}
  else {
    goto L1622;}}
 L1622: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_198X, v_210X)
    SstatusS = v_210X;
    goto L1539;}
  else {
    goto L1539;}}
 L1539: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_198X, v_209X)
    SstatusS = v_209X;
    goto L1548;}
  else {
    goto L1548;}}
 L1548: {
  *((long *) (ShpS)) = 1;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_198X, ((char *) (ShpS)), ((4 + (ShpS)) - (ShpS))));
    goto L1559;}
  else {
    goto L1559;}}
 L1559: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_198X, ((char *) (Snewspace_beginS)), ((ShpS) - (Snewspace_beginS))));
    goto L1566;}
  else {
    goto L1566;}}
 L1566: {
  status_216X = SstatusS;
  b_217X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_217X;
  e_218X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_218X;
  SlimitS = (Ssaved_limitS);
  ShpS = (Ssaved_hpS);
  arg0K0 = (Snewspace_beginS);
  goto L3007;}
 L3007: {
  addr_206X = arg0K0;
  if ((addr_206X < (ShpS))) {
    d_208X = *((long *) addr_206X);
    if ((2 == (3 & d_208X))) {
      arg0K0 = d_208X;
      goto L3028;}
    else {
      h_207X = *((long *) (-7 + d_208X));
      *((long *) addr_206X) = h_207X;
      arg0K0 = h_207X;
      goto L3028;}}
  else {
    return status_216X;}}
 L3028: {
  h_205X = arg0K0;
  arg0K0 = (4 + (addr_206X + (-4 & (3 + ((long)(((unsigned long)h_205X)>>8))))));
  goto L3007;}}

long do_gc(long finalizer_alist_227X, long *TT0)
{
  unsigned char arg1K0;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  long *areas_228X;
  long *sizes_229X;
  unsigned char count_230X;
  long value_231X;
  long new_thing_232X;
  long new_hp_233X;
  long stob_234X;
  long a_235X;
  long alist_236X;
  long start_237X;
  long h_238X;
  long stob_239X;
  long pair_240X;
  long descriptor_241X;
  long start_242X;
  unsigned char tracedP_243X;
  long thing_244X;
  long alist_245X;
  long okay_246X;
  long goners_247X;
  long new_thing_248X;
  long new_hp_249X;
  long next_250X;
  long thing_251X;
  long new_thing_252X;
  long new_hp_253X;
  long new_thing_254X;
  long new_hp_255X;
  long a_256X;
  long start_257X;
  long end_258X;
  long i_259X;
 {areas_228X = Simpure_areasS;
  sizes_229X = Simpure_sizesS;
  count_230X = Simpure_area_countS;
  arg0K0 = 0;
  goto L5409;}
 L5409: {
  i_259X = arg0K0;
  if ((i_259X < (((unsigned long) count_230X)))) {
    trace_locationsB((*(areas_228X + i_259X)), ((*(areas_228X + i_259X)) + (*(sizes_229X + i_259X))));
    arg0K0 = (1 + i_259X);
    goto L5409;}
  else {
    arg0K0 = finalizer_alist_227X;
    goto L5183;}}
 L5183: {
  alist_236X = arg0K0;
  if ((25 == alist_236X)) {
    start_242X = Snewspace_beginS;
    arg0K0 = start_242X;
    goto L5424;}
  else {
    pair_240X = *((long *) (-3 + alist_236X));
    descriptor_241X = *((long *) (-7 + (*((long *) (-3 + pair_240X)))));
    if ((2 == (3 & descriptor_241X))) {
      stob_239X = *((long *) (-3 + pair_240X));
      h_238X = *((long *) (-7 + stob_239X));
      start_237X = -3 + stob_239X;
      trace_locationsB(start_237X, (start_237X + (-4 & (3 + ((long)(((unsigned long)h_238X)>>8))))));
      goto L5208;}
    else {
      goto L5208;}}}
 L5424: {
  start_257X = arg0K0;
  end_258X = ShpS;
  trace_locationsB(start_257X, end_258X);
  if (((ShpS) < (SlimitS))) {
    if ((end_258X < (ShpS))) {
      arg0K0 = end_258X;
      goto L5424;}
    else {
      goto L5395;}}
  else {
    ps_error(((unsigned char *) "GC error: ran out of space in new heap"), 0);
    goto L5395;}}
 L5208: {
  stob_234X = *((long *) (1 + pair_240X));
  if ((3 == (3 & stob_234X))) {
    a_235X = -3 + stob_234X;
    if ((a_235X < (Soldspace_beginS))) {
      arg0K0 = stob_234X;
      goto L5212;}
    else {
      if ((a_235X < (Soldspace_endS))) {
        new_thing_232X = real_copy_object(stob_234X, (ShpS), &new_hp_233X);
        ShpS = new_hp_233X;
        arg0K0 = new_thing_232X;
        goto L5212;}
      else {
        arg0K0 = stob_234X;
        goto L5212;}}}
  else {
    arg0K0 = stob_234X;
    goto L5212;}}
 L5395: {
  clean_weak_pointers();
  arg0K0 = finalizer_alist_227X;
  arg0K1 = 25;
  arg0K2 = 25;
  goto L4855;}
 L5212: {
  value_231X = arg0K0;
  *((long *) (1 + pair_240X)) = value_231X;
  arg0K0 = (*((long *) (1 + alist_236X)));
  goto L5183;}
 L4855: {
  alist_245X = arg0K0;
  okay_246X = arg0K1;
  goners_247X = arg0K2;
  if ((25 == alist_245X)) {
    *TT0 = goners_247X;
    return okay_246X;}
  else {
    new_thing_254X = real_copy_object(alist_245X, (ShpS), &new_hp_255X);
    ShpS = new_hp_255X;
    new_thing_252X = real_copy_object((*((long *) (-3 + new_thing_254X))), (ShpS), &new_hp_253X);
    ShpS = new_hp_253X;
    thing_251X = *((long *) (-3 + new_thing_252X));
    next_250X = *((long *) (1 + new_thing_254X));
    if ((3 == (3 & thing_251X))) {
      a_256X = -3 + thing_251X;
      if ((a_256X < (Soldspace_beginS))) {
        arg1K0 = 1;
        arg0K1 = thing_251X;
        goto L4899;}
      else {
        if ((a_256X < (Soldspace_endS))) {
          if ((3 == (3 & (*((long *) (-7 + thing_251X)))))) {
            arg1K0 = 1;
            arg0K1 = (*((long *) (-7 + thing_251X)));
            goto L4899;}
          else {
            new_thing_248X = real_copy_object(thing_251X, (ShpS), &new_hp_249X);
            ShpS = new_hp_249X;
            arg1K0 = 0;
            arg0K1 = new_thing_248X;
            goto L4899;}}
        else {
          arg1K0 = 1;
          arg0K1 = thing_251X;
          goto L4899;}}}
    else {
      arg1K0 = 1;
      arg0K1 = thing_251X;
      goto L4899;}}}
 L4899: {
  tracedP_243X = arg1K0;
  thing_244X = arg0K1;
  *((long *) (-3 + new_thing_252X)) = thing_244X;
  *((long *) (-3 + new_thing_254X)) = new_thing_252X;
  if (tracedP_243X) {
    *((long *) (1 + new_thing_254X)) = okay_246X;
    arg0K0 = next_250X;
    arg0K1 = new_thing_254X;
    arg0K2 = goners_247X;
    goto L4855;}
  else {
    *((long *) (1 + new_thing_254X)) = goners_247X;
    arg0K0 = next_250X;
    arg0K1 = okay_246X;
    arg0K2 = new_thing_254X;
    goto L4855;}}}
void
s48_heap_init(void)
{
Sweak_pointer_limitS = 0;
Sweak_pointer_hpS = -1;
Ssaved_limitS = 0;
Ssaved_hpS = 0;
Sgc_countS = 0;
SeofPS = 0;
SstatusS = NO_ERRORS;
Sfinding_typeS = 1;
Simpure_area_countS = 0;
Spure_area_countS = 0;
SlimitS = 0;
ShpS = 0;
Hthe_record_type = 536870911;
}
