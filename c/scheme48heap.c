#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

long copy_weak_pointer(long, long, long*);
long real_copy_object(long, long, long*);
void clean_weak_pointers(void);
void write_barrier(long, long);
void end_collection(void);
long gc_count(void);
char image_writing_okayP(void);
void register_static_areas(unsigned char, long*, long*, unsigned char, long*, long*);
long preallocate_space(long);
long heap_size(void);
void begin_collection(void);
void initialize_heap(long, long);
char availableP(long);
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
char SeofPS;
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
  char temp_2X;
  long old_3X;
  long new_frontier_4X;
  long frontier_5X;
  long new_6X;
 {temp_2X = -1 == (Sweak_pointer_hpS);
  if (temp_2X) {
    goto L2017;}
  else {
    if (((Sweak_pointer_hpS) < (Sweak_pointer_limitS))) {
      arg0K0 = frontier_1X;
      goto L2022;}
    else {
      goto L2017;}}}
 L2017: {
  old_3X = Sweak_pointer_hpS;
  new_frontier_4X = 1024 + frontier_1X;
  Sweak_pointer_hpS = frontier_1X;
  Sweak_pointer_limitS = new_frontier_4X;
  *((long *) (Sweak_pointer_hpS)) = 261190;
  *((long *) (8 + (Sweak_pointer_hpS))) = old_3X;
  arg0K0 = new_frontier_4X;
  goto L2022;}
 L2022: {
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
          goto L3652;}
        else {
          if ((a_9X < (Soldspace_endS))) {
            return copy_weak_pointer(thing_7X, frontier_8X, TT0);}
          else {
            goto L3652;}}}
      else {
        goto L3652;}}
    else {
      goto L3652;}}}
 L3652: {
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
    goto L4713;}}
 L4713: {
  start_21X = arg0K0;
  end_22X = arg0K1;
  next_23X = *((long *) (8 + start_21X));
  arg0K0 = start_21X;
  goto L2820;}
 L2820: {
  scan_17X = arg0K0;
  if ((scan_17X < end_22X)) {
    *((long *) scan_17X) = 1074;
    value_18X = *((long *) (4 + scan_17X));
    if ((3 == (3 & value_18X))) {
      a_19X = -3 + value_18X;
      if ((a_19X < (Soldspace_beginS))) {
        goto L2866;}
      else {
        if ((a_19X < (Soldspace_endS))) {
          if ((3 == (3 & value_18X))) {
            h_20X = *((long *) (-7 + value_18X));
            if ((3 == (3 & h_20X))) {
              arg0K0 = h_20X;
              goto L2861;}
            else {
              arg0K0 = 1;
              goto L2861;}}
          else {
            goto L2866;}}
        else {
          goto L2866;}}}
    else {
      goto L2866;}}
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
      goto L4713;}}}
 L2866: {
  arg0K0 = (8 + scan_17X);
  goto L2820;}
 L2861: {
  x2_16X = arg0K0;
  *((long *) (4 + scan_17X)) = x2_16X;
  goto L2866;}}

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

char image_writing_okayP(void)
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

char availableP(long cells_39X)
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
  char arg1K1;
  char arg1K0;
  long status_45X;
  FILE * port_46X;
  long v_47X;
  long v_48X;
  char eofP_49X;
  long status_50X;
  unsigned char ch_51X;
  char eofP_52X;
  long status_53X;
  char eofP_54X;
  long status_55X;
  unsigned char ch_56X;
  char eofP_57X;
  long status_58X;
  char thing_59X;
  char eofP_60X;
  long status_61X;
  long i_62X;
  long status_63X;
  char eofP_64X;
  unsigned char ch_65X;
  char eofP_66X;
  long status_67X;
  unsigned char ch_68X;
  char eofP_69X;
  long status_70X;
  signed char v_71X;
  long v_72X;
  long status_73X;
  signed char v_74X;
  long v_75X;
  long status_76X;
  signed char v_77X;
  long v_78X;
  long status_79X;
  signed char v_80X;
  long v_81X;
  long status_82X;
  long v_83X;
  long v_84X;
  long v_85X;
  long v_86X;
  long v_87X;
  long v_88X;
  long v_89X;
  long v_90X;
  long v_91X;
  long cells_92X;
  long thing_93X;
  char eofP_94X;
  long status_95X;
  long cells_96X;
  long thing_97X;
  char eofP_98X;
  long status_99X;
  long old_bytes_per_cell_100X;
  long thing_101X;
  char eofP_102X;
  long status_103X;
  char same_versionP_104X;
  long thing_105X;
  char eofP_106X;
  long status_107X;
 {port_46X = ps_open_input_file(filename_44X, &status_45X);
  if ((status_45X == NO_ERRORS)) {
    SstatusS = NO_ERRORS;
    SeofPS = 0;
    if ((SeofPS)) {
      goto L2186;}
    else {
      goto L2767;}}
  else {
    ps_write_string(((unsigned char *) "Can't open heap image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    ps_write_string((ps_error_string(status_45X)), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2186: {
  if ((SeofPS)) {
    goto L2219;}
  else {
    goto L2731;}}
 L2767: {
  PS_READ_CHAR(port_46X, ch_51X, eofP_52X, status_53X)
  if (eofP_52X) {
    arg1K0 = eofP_52X;
    arg0K1 = status_53X;
    goto L2164;}
  else {
    if ((status_53X == NO_ERRORS)) {
      if ((12 == ch_51X)) {
        arg1K0 = 0;
        arg0K1 = status_53X;
        goto L2164;}
      else {
        goto L2767;}}
    else {
      arg1K0 = eofP_52X;
      arg0K1 = status_53X;
      goto L2164;}}}
 L2219: {
  if ((SeofPS)) {
    arg1K0 = 0;
    goto L2252;}
  else {
    arg0K0 = 0;
    goto L2707;}}
 L2731: {
  PS_READ_CHAR(port_46X, ch_56X, eofP_57X, status_58X)
  if (eofP_57X) {
    arg1K0 = eofP_57X;
    arg0K1 = status_58X;
    goto L2197;}
  else {
    if ((status_58X == NO_ERRORS)) {
      if ((10 == ch_56X)) {
        arg1K0 = 0;
        arg0K1 = status_58X;
        goto L2197;}
      else {
        goto L2731;}}
    else {
      arg1K0 = eofP_57X;
      arg0K1 = status_58X;
      goto L2197;}}}
 L2164: {
  eofP_49X = arg1K0;
  status_50X = arg0K1;
  if (eofP_49X) {
    SeofPS = 1;
    goto L2186;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2186;}
    else {
      SeofPS = 1;
      SstatusS = status_50X;
      goto L2186;}}}
 L2252: {
  same_versionP_104X = arg1K0;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2287;}
  else {
    PS_READ_INTEGER(port_46X, thing_105X, eofP_106X, status_107X)
    if (eofP_106X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2287;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_105X;
        goto L2287;}
      else {
        SeofPS = 1;
        SstatusS = status_107X;
        arg0K0 = -1;
        goto L2287;}}}}
 L2707: {
  i_62X = arg0K0;
  PS_READ_CHAR(port_46X, ch_65X, eofP_64X, status_63X)
  if (eofP_64X) {
    arg1K0 = 0;
    arg1K1 = eofP_64X;
    arg0K2 = status_63X;
    goto L2230;}
  else {
    if ((status_63X == NO_ERRORS)) {
      if ((i_62X == (strlen((char *) ((unsigned char *) "Vanilla 18"))))) {
        arg1K0 = (10 == ch_65X);
        arg1K1 = 0;
        arg0K2 = status_63X;
        goto L2230;}
      else {
        if ((ch_65X == (*(((unsigned char *) "Vanilla 18") + i_62X)))) {
          arg0K0 = (1 + i_62X);
          goto L2707;}
        else {
          arg1K0 = 0;
          arg1K1 = 0;
          arg0K2 = status_63X;
          goto L2230;}}}
    else {
      arg1K0 = 0;
      arg1K1 = eofP_64X;
      arg0K2 = status_63X;
      goto L2230;}}}
 L2197: {
  eofP_54X = arg1K0;
  status_55X = arg0K1;
  if (eofP_54X) {
    SeofPS = 1;
    goto L2219;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2219;}
    else {
      SeofPS = 1;
      SstatusS = status_55X;
      goto L2219;}}}
 L2287: {
  old_bytes_per_cell_100X = arg0K0;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2322;}
  else {
    PS_READ_INTEGER(port_46X, thing_101X, eofP_102X, status_103X)
    if (eofP_102X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2322;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_101X;
        goto L2322;}
      else {
        SeofPS = 1;
        SstatusS = status_103X;
        arg0K0 = -1;
        goto L2322;}}}}
 L2230: {
  thing_59X = arg1K0;
  eofP_60X = arg1K1;
  status_61X = arg0K2;
  if (eofP_60X) {
    SeofPS = 1;
    arg1K0 = 0;
    goto L2252;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      arg1K0 = thing_59X;
      goto L2252;}
    else {
      SeofPS = 1;
      SstatusS = status_61X;
      arg1K0 = 0;
      goto L2252;}}}
 L2322: {
  cells_96X = arg0K0;
  Sold_beginS = (((cells_96X)<<2));
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2360;}
  else {
    PS_READ_INTEGER(port_46X, thing_97X, eofP_98X, status_99X)
    if (eofP_98X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2360;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_97X;
        goto L2360;}
      else {
        SeofPS = 1;
        SstatusS = status_99X;
        arg0K0 = -1;
        goto L2360;}}}}
 L2360: {
  cells_92X = arg0K0;
  Sold_hpS = (((cells_92X)<<2));
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2398;}
  else {
    PS_READ_INTEGER(port_46X, thing_93X, eofP_94X, status_95X)
    if (eofP_94X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2398;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_93X;
        goto L2398;}
      else {
        SeofPS = 1;
        SstatusS = status_95X;
        arg0K0 = -1;
        goto L2398;}}}}
 L2398: {
  v_91X = arg0K0;
  Sstartup_procS = v_91X;
  Simage_portS = port_46X;
  if ((SeofPS)) {
    goto L2437;}
  else {
    goto L2654;}}
 L2437: {
  if (((SstatusS) == NO_ERRORS)) {
    if ((SeofPS)) {
      SstatusS = EDOM;
      ps_write_string(((unsigned char *) "Premature EOF when reading image file"), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L2526;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L2526;}}
    else {
      if (same_versionP_104X) {
        if ((4 == old_bytes_per_cell_100X)) {
          return ((Sold_hpS) - (Sold_beginS));}
        else {
          ps_write_string(((unsigned char *) "Incompatible bytes-per-cell in image"), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L2586;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L2586;}}}
      else {
        ps_write_string(((unsigned char *) "Format of image is incompatible with this version of system"), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2556;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2556;}}}}
  else {
    ps_write_string(((unsigned char *) "Error reading from image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2501;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2501;}}}
 L2654: {
  PS_READ_CHAR(port_46X, ch_68X, eofP_69X, status_70X)
  if (eofP_69X) {
    arg1K0 = eofP_69X;
    arg0K1 = status_70X;
    goto L2415;}
  else {
    if ((status_70X == NO_ERRORS)) {
      if ((12 == ch_68X)) {
        arg1K0 = 0;
        arg0K1 = status_70X;
        goto L2415;}
      else {
        goto L2654;}}
    else {
      arg1K0 = eofP_69X;
      arg0K1 = status_70X;
      goto L2415;}}}
 L2526: {
  status_79X = ps_close(port_46X);
  if ((status_79X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L2449;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L2449;}}
 L2586: {
  status_73X = ps_close(port_46X);
  if ((status_73X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L2461;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L2461;}}
 L2556: {
  status_76X = ps_close(port_46X);
  if ((status_76X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L2455;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L2455;}}
 L2501: {
  status_82X = ps_close(port_46X);
  if ((status_82X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L2442;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L2442;}}
 L2415: {
  eofP_66X = arg1K0;
  status_67X = arg0K1;
  if (eofP_66X) {
    SeofPS = 1;
    goto L2437;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2437;}
    else {
      SeofPS = 1;
      SstatusS = status_67X;
      goto L2437;}}}
 L2449: {
  v_77X = arg2K0;
  return (((long) v_77X));}
 L2461: {
  v_71X = arg2K0;
  return (((long) v_71X));}
 L2455: {
  v_74X = arg2K0;
  return (((long) v_74X));}
 L2442: {
  v_80X = arg2K0;
  return (((long) v_80X));}}

long find_all(unsigned char type_108X)
{
  long arg0K0;
  long merged_arg0K1;
  long merged_arg0K0;

  int Hproc109_return_tag;
  char Hproc1090_return_value;
  char v_110X;
  long start_hp_111X;
  long *areas_112X;
  long *sizes_113X;
  unsigned char count_114X;
  long i_115X;
  char v_116X;
  long i_117X;
  char v_118X;
  long *areas_119X;
  long *sizes_120X;
  unsigned char count_121X;
  unsigned char type_122X;
  long addr_123X;
  long d_124X;
  long start_125X;
  long end_126X;
 {Sfinding_typeS = type_108X;
  start_hp_111X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = (4 + (ShpS));
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_111X;
  Hproc109_return_tag = 0;
  goto Hproc109;
 Hproc109_return_0:
  v_110X = Hproc1090_return_value;
  if (v_110X) {
    areas_112X = Simpure_areasS;
    sizes_113X = Simpure_sizesS;
    count_114X = Simpure_area_countS;
    arg0K0 = 0;
    goto L4275;}
  else {
    goto L4231;}}
 L4275: {
  i_117X = arg0K0;
  if ((i_117X < (((unsigned long) count_114X)))) {
    merged_arg0K0 = (*(areas_112X + i_117X));
    merged_arg0K1 = ((*(areas_112X + i_117X)) + (*(sizes_113X + i_117X)));
    Hproc109_return_tag = 1;
    goto Hproc109;
   Hproc109_return_1:
    v_118X = Hproc1090_return_value;
    if (v_118X) {
      arg0K0 = (1 + i_117X);
      goto L4275;}
    else {
      goto L4231;}}
  else {
    areas_119X = Spure_areasS;
    sizes_120X = Spure_sizesS;
    count_121X = Spure_area_countS;
    arg0K0 = 0;
    goto L4294;}}
 L4231: {
  ShpS = start_hp_111X;
  return 1;}
 L4294: {
  i_115X = arg0K0;
  if ((i_115X < (((unsigned long) count_121X)))) {
    merged_arg0K0 = (*(areas_119X + i_115X));
    merged_arg0K1 = ((*(areas_119X + i_115X)) + (*(sizes_120X + i_115X)));
    Hproc109_return_tag = 2;
    goto Hproc109;
   Hproc109_return_2:
    v_116X = Hproc1090_return_value;
    if (v_116X) {
      arg0K0 = (1 + i_115X);
      goto L4294;}
    else {
      goto L4231;}}
  else {
    *((long *) start_hp_111X) = (-1014 + (((((ShpS) - start_hp_111X))<<8)));
    return (7 + start_hp_111X);}}
 Hproc109: {
  start_125X = merged_arg0K0;
  end_126X = merged_arg0K1;{
  type_122X = Sfinding_typeS;
  arg0K0 = start_125X;
  goto L3306;}
 L3306: {
  addr_123X = arg0K0;
  if ((addr_123X < end_126X)) {
    d_124X = *((long *) addr_123X);
    if ((2 == (3 & d_124X))) {
      if (((((unsigned long) type_122X)) == (31 & (((d_124X)>>2))))) {
        if (((16 + (ShpS)) < (SlimitS))) {
          *((long *) (ShpS)) = (7 + addr_123X);
          ShpS = (4 + (ShpS));
          goto L3340;}
        else {
          Hproc1090_return_value = 0;
          goto Hproc109_return;}}
      else {
        goto L3340;}}
    else {
      ps_write_string(((unsigned char *) "heap is in an inconsistent state."), (stderr));
      Hproc1090_return_value = 0;
      goto Hproc109_return;}}
  else {
    Hproc1090_return_value = 1;
    goto Hproc109_return;}}
 L3340: {
  arg0K0 = (4 + (addr_123X + (-4 & (3 + ((long)(((unsigned long)d_124X)>>8))))));
  goto L3306;}
 Hproc109_return:
  switch (Hproc109_return_tag) {
  case 0: goto Hproc109_return_0;
  case 1: goto Hproc109_return_1;
  default: goto Hproc109_return_2;
  }}
}

unsigned char trace_locationsB(long start_127X, long end_128X)
{
  long arg0K1;
  long arg0K0;
  long new_thing_129X;
  long frontier_130X;
  long data_addr_131X;
  long new_132X;
  long new_thing_133X;
  long frontier_134X;
  long descriptor_135X;
  long a_136X;
  long h_137X;
  long a_138X;
  long addr_139X;
  long frontier_140X;
  long thing_141X;
  long next_142X;
 {arg0K0 = start_127X;
  arg0K1 = (ShpS);
  goto L4523;}
 L4523: {
  addr_139X = arg0K0;
  frontier_140X = arg0K1;
  if ((addr_139X < end_128X)) {
    thing_141X = *((long *) addr_139X);
    next_142X = 4 + addr_139X;
    if ((2 == (3 & thing_141X))) {
      if (((31 & (((thing_141X)>>2))) < 16)) {
        goto L4541;}
      else {
        arg0K0 = (next_142X + (-4 & (3 + ((long)(((unsigned long)thing_141X)>>8)))));
        arg0K1 = frontier_140X;
        goto L4523;}}
    else {
      goto L4541;}}
  else {
    ShpS = frontier_140X;
    return 0;}}
 L4541: {
  if ((3 == (3 & thing_141X))) {
    a_136X = -3 + thing_141X;
    if ((a_136X < (Soldspace_beginS))) {
      arg0K0 = next_142X;
      arg0K1 = frontier_140X;
      goto L4523;}
    else {
      if ((a_136X < (Soldspace_endS))) {
        h_137X = *((long *) (-7 + thing_141X));
        if ((3 == (3 & h_137X))) {
          arg0K0 = h_137X;
          arg0K1 = frontier_140X;
          goto L4548;}
        else {
          if ((1074 == h_137X)) {
            descriptor_135X = *((long *) (-3 + thing_141X));
            if ((3 == (3 & descriptor_135X))) {
              a_138X = -3 + descriptor_135X;
              if ((a_138X < (Soldspace_beginS))) {
                goto L5569;}
              else {
                if ((a_138X < (Soldspace_endS))) {
                  new_thing_133X = copy_weak_pointer(thing_141X, frontier_140X, &frontier_134X);
                  arg0K0 = new_thing_133X;
                  arg0K1 = frontier_134X;
                  goto L4548;}
                else {
                  goto L5569;}}}
            else {
              goto L5569;}}
          else {
            goto L5569;}}}
      else {
        arg0K0 = next_142X;
        arg0K1 = frontier_140X;
        goto L4523;}}}
  else {
    arg0K0 = next_142X;
    arg0K1 = frontier_140X;
    goto L4523;}}
 L4548: {
  new_thing_129X = arg0K0;
  frontier_130X = arg0K1;
  *((long *) addr_139X) = new_thing_129X;
  arg0K0 = next_142X;
  arg0K1 = frontier_130X;
  goto L4523;}
 L5569: {
  *((long *) frontier_140X) = h_137X;
  data_addr_131X = 4 + frontier_140X;
  new_132X = 3 + data_addr_131X;
  *((long *) (-7 + thing_141X)) = new_132X;
  memcpy((void *)data_addr_131X, (void *)(-3 + thing_141X),((long)(((unsigned long)h_137X)>>8)));
  arg0K0 = new_132X;
  arg0K1 = (data_addr_131X + (-4 & (3 + ((long)(((unsigned long)h_137X)>>8)))));
  goto L4548;}}

long read_image(long startup_space_143X)
{
  FILE * arg4K0;
  unsigned char *arg3K1;
  char arg1K1;
  char arg1K0;
  signed char arg2K0;
  long arg0K2;
  long arg0K0;
  long got_144X;
  char eofP_145X;
  long status_146X;
  FILE * port_147X;
  long ptr_148X;
  unsigned char x_149X;
  unsigned char x_150X;
  long value_151X;
  long next_152X;
  long d_153X;
  long ptr_154X;
  long descriptor_155X;
  signed char v_156X;
  long v_157X;
  long status_158X;
  signed char v_159X;
  long v_160X;
  long status_161X;
  signed char v_162X;
  long v_163X;
  long status_164X;
  signed char v_165X;
  long v_166X;
  long status_167X;
  char okayP_168X;
  unsigned char *string_169X;
  long v_170X;
  long v_171X;
  long status_172X;
  long v_173X;
  long v_174X;
  long v_175X;
  long v_176X;
  long v_177X;
  long v_178X;
  unsigned char v_179X;
  char eofP_180X;
  long status_181X;
  long v_182X;
  signed char v_183X;
  long v_184X;
  long status_185X;
  long descriptor_186X;
  FILE * port_187X;
  char reverseP_188X;
  long startup_space_189X;
  long got_190X;
  char eofP_191X;
  long status_192X;
  long need_193X;
  long v_194X;
  long v_195X;
  long start_196X;
  long new_limit_197X;
  long new_hp_198X;
  long delta_199X;
  signed char v_200X;
  long v_201X;
  long status_202X;
  signed char v_203X;
  long v_204X;
  long status_205X;
  char okayP_206X;
  unsigned char *string_207X;
  long v_208X;
  long v_209X;
  unsigned char x_210X;
  unsigned char x_211X;
  long addr_212X;
  long v_213X;
  long v_214X;
 {port_147X = Simage_portS;
  got_144X = ps_read_block(port_147X, ((char *) (ShpS)), 4, &eofP_145X, &status_146X);
  if ((status_146X == NO_ERRORS)) {
    if (eofP_145X) {
      arg1K0 = 0;
      arg3K1 = ((unsigned char *) "Premature EOF when reading image file");
      goto L4811;}
    else {
      if ((got_144X < 4)) {
        arg1K0 = 0;
        arg3K1 = ((unsigned char *) "Read returned too few bytes");
        goto L4811;}
      else {
        arg1K0 = 1;
        arg3K1 = ((unsigned char *) "");
        goto L4811;}}}
  else {
    SstatusS = status_146X;
    arg1K0 = 0;
    arg3K1 = ((unsigned char *) "Error reading from image file");
    goto L4811;}}
 L4811: {
  okayP_206X = arg1K0;
  string_207X = arg3K1;
  if (okayP_206X) {
    if ((1 == (*((long *) (ShpS))))) {
      arg4K0 = port_147X;
      arg1K1 = 0;
      arg0K2 = startup_space_143X;
      goto L3840;}
    else {
      addr_212X = ShpS;
      x_211X = *((unsigned char *) addr_212X);
      *((unsigned char *) addr_212X) = (*((unsigned char *) (3 + addr_212X)));
      *((unsigned char *) (3 + addr_212X)) = x_211X;
      x_210X = *((unsigned char *) (1 + addr_212X));
      *((unsigned char *) (1 + addr_212X)) = (*((unsigned char *) (2 + addr_212X)));
      *((unsigned char *) (2 + addr_212X)) = x_210X;
      if ((1 == (*((long *) (ShpS))))) {
        arg4K0 = port_147X;
        arg1K1 = 1;
        arg0K2 = startup_space_143X;
        goto L3840;}
      else {
        ps_write_string(((unsigned char *) "Unable to correct byte order"), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L4911;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L4911;}}}}
  else {
    ps_write_string(string_207X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L4865;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L4865;}}}
 L3840: {
  port_187X = arg4K0;
  reverseP_188X = arg1K1;
  startup_space_189X = arg0K2;
  delta_199X = (ShpS) - (Sold_beginS);
  new_hp_198X = (Sold_hpS) + delta_199X;
  new_limit_197X = Snewspace_endS;
  start_196X = ShpS;
  if (((startup_space_189X + new_hp_198X) < new_limit_197X)) {
    need_193X = (Sold_hpS) - (Sold_beginS);
    got_190X = ps_read_block(port_187X, ((char *) (ShpS)), need_193X, &eofP_191X, &status_192X);
    if ((status_192X == NO_ERRORS)) {
      if (eofP_191X) {
        arg1K0 = 0;
        arg3K1 = ((unsigned char *) "Premature EOF when reading image file");
        goto L3858;}
      else {
        if ((got_190X < need_193X)) {
          arg1K0 = 0;
          arg3K1 = ((unsigned char *) "Read returned too few bytes");
          goto L3858;}
        else {
          arg1K0 = 1;
          arg3K1 = ((unsigned char *) "");
          goto L3858;}}}
    else {
      SstatusS = status_192X;
      arg1K0 = 0;
      arg3K1 = ((unsigned char *) "Error reading from image file");
      goto L3858;}}
  else {
    ps_write_string(((unsigned char *) "Heap not big enough to restore this image"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L3932;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L3932;}}}
 L4911: {
  status_202X = ps_close(port_147X);
  if ((status_202X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L4833;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L4833;}}
 L4865: {
  status_205X = ps_close(port_147X);
  if ((status_205X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L4820;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L4820;}}
 L3858: {
  okayP_168X = arg1K0;
  string_169X = arg3K1;
  PS_READ_CHAR(port_187X, v_179X, eofP_180X, status_181X)
  if (okayP_168X) {
    if ((status_181X == NO_ERRORS)) {
      if (eofP_180X) {
        status_172X = ps_close(port_187X);
        if ((status_172X == NO_ERRORS)) {
          if (reverseP_188X) {
            ps_write_string(((unsigned char *) "Correcting byte order of resumed image."), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = start_196X;
            goto L3194;}
          else {
            goto L3904;}}
        else {
          ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L4067;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L4067;}}}
      else {
        ps_write_string(((unsigned char *) "Image file has extraneous data after image"), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L4037;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L4037;}}}
    else {
      ps_write_string(((unsigned char *) "Error reading from image file"), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L4007;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L4007;}}}
  else {
    ps_write_string(string_169X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L3977;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L3977;}}}
 L3932: {
  status_185X = ps_close(port_187X);
  if ((status_185X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L3853;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L3853;}}
 L4833: {
  v_200X = arg2K0;
  return (((long) v_200X));}
 L4820: {
  v_203X = arg2K0;
  return (((long) v_203X));}
 L3194: {
  ptr_148X = arg0K0;
  if ((ptr_148X < new_hp_198X)) {
    x_149X = *((unsigned char *) ptr_148X);
    *((unsigned char *) ptr_148X) = (*((unsigned char *) (3 + ptr_148X)));
    *((unsigned char *) (3 + ptr_148X)) = x_149X;
    x_150X = *((unsigned char *) (1 + ptr_148X));
    *((unsigned char *) (1 + ptr_148X)) = (*((unsigned char *) (2 + ptr_148X)));
    *((unsigned char *) (2 + ptr_148X)) = x_150X;
    value_151X = *((long *) ptr_148X);
    next_152X = 4 + ptr_148X;
    if ((2 == (3 & value_151X))) {
      if (((31 & (((value_151X)>>2))) < 16)) {
        arg0K0 = next_152X;
        goto L3194;}
      else {
        arg0K0 = (next_152X + (-4 & (3 + ((long)(((unsigned long)value_151X)>>8)))));
        goto L3194;}}
    else {
      arg0K0 = next_152X;
      goto L3194;}}
  else {
    goto L3904;}}
 L3904: {
  if ((0 == delta_199X)) {
    goto L3914;}
  else {
    arg0K0 = start_196X;
    goto L3087;}}
 L4067: {
  status_158X = ps_close(port_187X);
  if ((status_158X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L3895;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L3895;}}
 L4037: {
  status_161X = ps_close(port_187X);
  if ((status_161X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L3889;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L3889;}}
 L4007: {
  status_164X = ps_close(port_187X);
  if ((status_164X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L3883;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L3883;}}
 L3977: {
  status_167X = ps_close(port_187X);
  if ((status_167X == NO_ERRORS)) {
    arg2K0 = -1;
    goto L3877;}
  else {
    ps_write_string(((unsigned char *) "Error closing image file"), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    arg2K0 = -1;
    goto L3877;}}
 L3853: {
  v_183X = arg2K0;
  return (((long) v_183X));}
 L3914: {
  ShpS = new_hp_198X;
  descriptor_186X = Sstartup_procS;
  if ((3 == (3 & descriptor_186X))) {
    return (3 + ((-3 + descriptor_186X) + delta_199X));}
  else {
    return descriptor_186X;}}
 L3087: {
  ptr_154X = arg0K0;
  if ((ptr_154X < new_hp_198X)) {
    descriptor_155X = *((long *) ptr_154X);
    if ((3 == (3 & descriptor_155X))) {
      arg0K0 = (3 + ((-3 + descriptor_155X) + delta_199X));
      goto L3094;}
    else {
      arg0K0 = descriptor_155X;
      goto L3094;}}
  else {
    goto L3914;}}
 L3895: {
  v_156X = arg2K0;
  return (((long) v_156X));}
 L3889: {
  v_159X = arg2K0;
  return (((long) v_159X));}
 L3883: {
  v_162X = arg2K0;
  return (((long) v_162X));}
 L3877: {
  v_165X = arg2K0;
  return (((long) v_165X));}
 L3094: {
  d_153X = arg0K0;
  *((long *) ptr_154X) = d_153X;
  if ((2 == (3 & d_153X))) {
    if (((31 & (((d_153X)>>2))) < 16)) {
      goto L3110;}
    else {
      arg0K0 = ((4 + ptr_154X) + (-4 & (3 + ((long)(((unsigned long)d_153X)>>8)))));
      goto L3087;}}
  else {
    goto L3110;}}
 L3110: {
  arg0K0 = (4 + ptr_154X);
  goto L3087;}}

long find_all_records(long record_type_215X)
{
  long arg0K0;
  long merged_arg0K1;
  long merged_arg0K0;

  int Hproc216_return_tag;
  char Hproc2160_return_value;
  char v_217X;
  long start_hp_218X;
  long *areas_219X;
  long *sizes_220X;
  unsigned char count_221X;
  long i_222X;
  char v_223X;
  long i_224X;
  char v_225X;
  long *areas_226X;
  long *sizes_227X;
  unsigned char count_228X;
  unsigned char type_229X;
  long addr_230X;
  long d_231X;
  long record_232X;
  long start_233X;
  long end_234X;
 {Hthe_record_type = record_type_215X;
  Sfinding_typeS = 8;
  start_hp_218X = ShpS;
  *((long *) (ShpS)) = 0;
  ShpS = (4 + (ShpS));
  merged_arg0K0 = (Snewspace_beginS);
  merged_arg0K1 = start_hp_218X;
  Hproc216_return_tag = 0;
  goto Hproc216;
 Hproc216_return_0:
  v_217X = Hproc2160_return_value;
  if (v_217X) {
    areas_219X = Simpure_areasS;
    sizes_220X = Simpure_sizesS;
    count_221X = Simpure_area_countS;
    arg0K0 = 0;
    goto L4428;}
  else {
    goto L4384;}}
 L4428: {
  i_224X = arg0K0;
  if ((i_224X < (((unsigned long) count_221X)))) {
    merged_arg0K0 = (*(areas_219X + i_224X));
    merged_arg0K1 = ((*(areas_219X + i_224X)) + (*(sizes_220X + i_224X)));
    Hproc216_return_tag = 1;
    goto Hproc216;
   Hproc216_return_1:
    v_225X = Hproc2160_return_value;
    if (v_225X) {
      arg0K0 = (1 + i_224X);
      goto L4428;}
    else {
      goto L4384;}}
  else {
    areas_226X = Spure_areasS;
    sizes_227X = Spure_sizesS;
    count_228X = Spure_area_countS;
    arg0K0 = 0;
    goto L4447;}}
 L4384: {
  ShpS = start_hp_218X;
  return 1;}
 L4447: {
  i_222X = arg0K0;
  if ((i_222X < (((unsigned long) count_228X)))) {
    merged_arg0K0 = (*(areas_226X + i_222X));
    merged_arg0K1 = ((*(areas_226X + i_222X)) + (*(sizes_227X + i_222X)));
    Hproc216_return_tag = 2;
    goto Hproc216;
   Hproc216_return_2:
    v_223X = Hproc2160_return_value;
    if (v_223X) {
      arg0K0 = (1 + i_222X);
      goto L4447;}
    else {
      goto L4384;}}
  else {
    *((long *) start_hp_218X) = (-1014 + (((((ShpS) - start_hp_218X))<<8)));
    return (7 + start_hp_218X);}}
 Hproc216: {
  start_233X = merged_arg0K0;
  end_234X = merged_arg0K1;{
  type_229X = Sfinding_typeS;
  arg0K0 = start_233X;
  goto L3465;}
 L3465: {
  addr_230X = arg0K0;
  if ((addr_230X < end_234X)) {
    d_231X = *((long *) addr_230X);
    if ((2 == (3 & d_231X))) {
      if (((((unsigned long) type_229X)) == (31 & (((d_231X)>>2))))) {
        record_232X = 7 + addr_230X;
        if (((Hthe_record_type) == (*((long *) (-3 + record_232X))))) {
          if (((16 + (ShpS)) < (SlimitS))) {
            *((long *) (ShpS)) = record_232X;
            ShpS = (4 + (ShpS));
            goto L3499;}
          else {
            Hproc2160_return_value = 0;
            goto Hproc216_return;}}
        else {
          goto L3499;}}
      else {
        goto L3499;}}
    else {
      ps_write_string(((unsigned char *) "heap is in an inconsistent state."), (stderr));
      Hproc2160_return_value = 0;
      goto Hproc216_return;}}
  else {
    Hproc2160_return_value = 1;
    goto Hproc216_return;}}
 L3499: {
  arg0K0 = (4 + (addr_230X + (-4 & (3 + ((long)(((unsigned long)d_231X)>>8))))));
  goto L3465;}
 Hproc216_return:
  switch (Hproc216_return_tag) {
  case 0: goto Hproc216_return_0;
  case 1: goto Hproc216_return_1;
  default: goto Hproc216_return_2;
  }}
}

long trace_value(long stob_235X)
{
  long new_thing_236X;
  long new_hp_237X;
  long a_238X;
 {if ((3 == (3 & stob_235X))) {
    a_238X = -3 + stob_235X;
    if ((a_238X < (Soldspace_beginS))) {
      return stob_235X;}
    else {
      if ((a_238X < (Soldspace_endS))) {
        new_thing_236X = real_copy_object(stob_235X, (ShpS), &new_hp_237X);
        ShpS = new_hp_237X;
        return new_thing_236X;}
      else {
        return stob_235X;}}}
  else {
    return stob_235X;}}}

unsigned char trace_stob_contentsB(long stob_239X)
{
  long h_240X;
  long start_241X;
 {h_240X = *((long *) (-7 + stob_239X));
  start_241X = -3 + stob_239X;
  return trace_locationsB(start_241X, (start_241X + (-4 & (3 + ((long)(((unsigned long)h_240X)>>8))))));}}

long write_image(long resume_proc_242X, FILE * port_243X, void (*mark_traced_channels_closedB_244X)(void))
{
  long arg0K0;
  long new_thing_245X;
  long new_hp_246X;
  long e_247X;
  long b_248X;
  long a_249X;
  long h_250X;
  long addr_251X;
  long h_252X;
  long d_253X;
  long v_254X;
  long v_255X;
  long v_256X;
  long cells_257X;
  long v_258X;
  long cells_259X;
  long v_260X;
  long status_261X;
  long b_262X;
  long e_263X;
  long v_264X;
  long v_265X;
  long v_266X;
  long v_267X;
  long start_268X;
  long end_269X;
  long resume_proc_270X;
  long start_271X;
 {b_248X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_248X;
  e_247X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_247X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = -1;
  if ((3 == (3 & resume_proc_242X))) {
    a_249X = -3 + resume_proc_242X;
    if ((a_249X < (Soldspace_beginS))) {
      arg0K0 = resume_proc_242X;
      goto L5416;}
    else {
      if ((a_249X < (Soldspace_endS))) {
        new_thing_245X = real_copy_object(resume_proc_242X, (ShpS), &new_hp_246X);
        ShpS = new_hp_246X;
        arg0K0 = new_thing_245X;
        goto L5416;}
      else {
        arg0K0 = resume_proc_242X;
        goto L5416;}}}
  else {
    arg0K0 = resume_proc_242X;
    goto L5416;}}
 L5416: {
  resume_proc_270X = arg0K0;
  start_271X = Snewspace_beginS;
  arg0K0 = start_271X;
  goto L5468;}
 L5468: {
  start_268X = arg0K0;
  end_269X = ShpS;
  trace_locationsB(start_268X, end_269X);
  if (((ShpS) < (SlimitS))) {
    if ((end_269X < (ShpS))) {
      arg0K0 = end_269X;
      goto L5468;}
    else {
      goto L5420;}}
  else {
    ps_error(((unsigned char *) "GC error: ran out of space in new heap"), 0);
    goto L5420;}}
 L5420: {
  clean_weak_pointers();
  mark_traced_channels_closedB_244X();
  SstatusS = NO_ERRORS;
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_243X, v_267X)
    SstatusS = v_267X;
    goto L1488;}
  else {
    goto L1488;}}
 L1488: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_243X, v_266X)
    SstatusS = v_266X;
    goto L1497;}
  else {
    goto L1497;}}
 L1497: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_243X, v_265X)
    SstatusS = v_265X;
    goto L1506;}
  else {
    goto L1506;}}
 L1506: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string(((unsigned char *) "Vanilla 18"), port_243X));
    goto L1513;}
  else {
    goto L1513;}}
 L1513: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_243X, v_264X)
    SstatusS = v_264X;
    goto L1522;}
  else {
    goto L1522;}}
 L1522: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, port_243X));
    goto L1566;}
  else {
    goto L1566;}}
 L1566: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_243X, v_260X)
    SstatusS = v_260X;
    goto L1524;}
  else {
    goto L1524;}}
 L1524: {
  cells_259X = Snewspace_beginS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_259X)>>2)), port_243X));
    goto L1584;}
  else {
    goto L1584;}}
 L1584: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_243X, v_258X)
    SstatusS = v_258X;
    goto L1528;}
  else {
    goto L1528;}}
 L1528: {
  cells_257X = ShpS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((cells_257X)>>2)), port_243X));
    goto L1602;}
  else {
    goto L1602;}}
 L1602: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_243X, v_256X)
    SstatusS = v_256X;
    goto L1532;}
  else {
    goto L1532;}}
 L1532: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_270X, port_243X));
    goto L1617;}
  else {
    goto L1617;}}
 L1617: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_243X, v_255X)
    SstatusS = v_255X;
    goto L1534;}
  else {
    goto L1534;}}
 L1534: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_243X, v_254X)
    SstatusS = v_254X;
    goto L1543;}
  else {
    goto L1543;}}
 L1543: {
  *((long *) (ShpS)) = 1;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_243X, ((char *) (ShpS)), ((4 + (ShpS)) - (ShpS))));
    goto L1554;}
  else {
    goto L1554;}}
 L1554: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_243X, ((char *) (Snewspace_beginS)), ((ShpS) - (Snewspace_beginS))));
    goto L1561;}
  else {
    goto L1561;}}
 L1561: {
  status_261X = SstatusS;
  b_262X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_262X;
  e_263X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_263X;
  SlimitS = (Ssaved_limitS);
  ShpS = (Ssaved_hpS);
  arg0K0 = (Snewspace_beginS);
  goto L3002;}
 L3002: {
  addr_251X = arg0K0;
  if ((addr_251X < (ShpS))) {
    d_253X = *((long *) addr_251X);
    if ((2 == (3 & d_253X))) {
      arg0K0 = d_253X;
      goto L3023;}
    else {
      h_252X = *((long *) (-7 + d_253X));
      *((long *) addr_251X) = h_252X;
      arg0K0 = h_252X;
      goto L3023;}}
  else {
    return status_261X;}}
 L3023: {
  h_250X = arg0K0;
  arg0K0 = (4 + (addr_251X + (-4 & (3 + ((long)(((unsigned long)h_250X)>>8))))));
  goto L3002;}}

long do_gc(long finalizer_alist_272X, long *TT0)
{
  char arg1K0;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  long *areas_273X;
  long *sizes_274X;
  unsigned char count_275X;
  long value_276X;
  long new_thing_277X;
  long new_hp_278X;
  long stob_279X;
  long a_280X;
  long alist_281X;
  long start_282X;
  long h_283X;
  long stob_284X;
  long pair_285X;
  long descriptor_286X;
  long start_287X;
  char tracedP_288X;
  long thing_289X;
  long alist_290X;
  long okay_291X;
  long goners_292X;
  long new_thing_293X;
  long new_hp_294X;
  long next_295X;
  long thing_296X;
  long new_thing_297X;
  long new_hp_298X;
  long new_thing_299X;
  long new_hp_300X;
  long a_301X;
  long start_302X;
  long end_303X;
  long i_304X;
 {areas_273X = Simpure_areasS;
  sizes_274X = Simpure_sizesS;
  count_275X = Simpure_area_countS;
  arg0K0 = 0;
  goto L5513;}
 L5513: {
  i_304X = arg0K0;
  if ((i_304X < (((unsigned long) count_275X)))) {
    trace_locationsB((*(areas_273X + i_304X)), ((*(areas_273X + i_304X)) + (*(sizes_274X + i_304X))));
    arg0K0 = (1 + i_304X);
    goto L5513;}
  else {
    arg0K0 = finalizer_alist_272X;
    goto L5287;}}
 L5287: {
  alist_281X = arg0K0;
  if ((25 == alist_281X)) {
    start_287X = Snewspace_beginS;
    arg0K0 = start_287X;
    goto L5528;}
  else {
    pair_285X = *((long *) (-3 + alist_281X));
    descriptor_286X = *((long *) (-7 + (*((long *) (-3 + pair_285X)))));
    if ((2 == (3 & descriptor_286X))) {
      stob_284X = *((long *) (-3 + pair_285X));
      h_283X = *((long *) (-7 + stob_284X));
      start_282X = -3 + stob_284X;
      trace_locationsB(start_282X, (start_282X + (-4 & (3 + ((long)(((unsigned long)h_283X)>>8))))));
      goto L5312;}
    else {
      goto L5312;}}}
 L5528: {
  start_302X = arg0K0;
  end_303X = ShpS;
  trace_locationsB(start_302X, end_303X);
  if (((ShpS) < (SlimitS))) {
    if ((end_303X < (ShpS))) {
      arg0K0 = end_303X;
      goto L5528;}
    else {
      goto L5499;}}
  else {
    ps_error(((unsigned char *) "GC error: ran out of space in new heap"), 0);
    goto L5499;}}
 L5312: {
  stob_279X = *((long *) (1 + pair_285X));
  if ((3 == (3 & stob_279X))) {
    a_280X = -3 + stob_279X;
    if ((a_280X < (Soldspace_beginS))) {
      arg0K0 = stob_279X;
      goto L5316;}
    else {
      if ((a_280X < (Soldspace_endS))) {
        new_thing_277X = real_copy_object(stob_279X, (ShpS), &new_hp_278X);
        ShpS = new_hp_278X;
        arg0K0 = new_thing_277X;
        goto L5316;}
      else {
        arg0K0 = stob_279X;
        goto L5316;}}}
  else {
    arg0K0 = stob_279X;
    goto L5316;}}
 L5499: {
  clean_weak_pointers();
  arg0K0 = finalizer_alist_272X;
  arg0K1 = 25;
  arg0K2 = 25;
  goto L4959;}
 L5316: {
  value_276X = arg0K0;
  *((long *) (1 + pair_285X)) = value_276X;
  arg0K0 = (*((long *) (1 + alist_281X)));
  goto L5287;}
 L4959: {
  alist_290X = arg0K0;
  okay_291X = arg0K1;
  goners_292X = arg0K2;
  if ((25 == alist_290X)) {
    *TT0 = goners_292X;
    return okay_291X;}
  else {
    new_thing_299X = real_copy_object(alist_290X, (ShpS), &new_hp_300X);
    ShpS = new_hp_300X;
    new_thing_297X = real_copy_object((*((long *) (-3 + new_thing_299X))), (ShpS), &new_hp_298X);
    ShpS = new_hp_298X;
    thing_296X = *((long *) (-3 + new_thing_297X));
    next_295X = *((long *) (1 + new_thing_299X));
    if ((3 == (3 & thing_296X))) {
      a_301X = -3 + thing_296X;
      if ((a_301X < (Soldspace_beginS))) {
        arg1K0 = 1;
        arg0K1 = thing_296X;
        goto L5003;}
      else {
        if ((a_301X < (Soldspace_endS))) {
          if ((3 == (3 & (*((long *) (-7 + thing_296X)))))) {
            arg1K0 = 1;
            arg0K1 = (*((long *) (-7 + thing_296X)));
            goto L5003;}
          else {
            new_thing_293X = real_copy_object(thing_296X, (ShpS), &new_hp_294X);
            ShpS = new_hp_294X;
            arg1K0 = 0;
            arg0K1 = new_thing_293X;
            goto L5003;}}
        else {
          arg1K0 = 1;
          arg0K1 = thing_296X;
          goto L5003;}}}
    else {
      arg1K0 = 1;
      arg0K1 = thing_296X;
      goto L5003;}}}
 L5003: {
  tracedP_288X = arg1K0;
  thing_289X = arg0K1;
  *((long *) (-3 + new_thing_297X)) = thing_289X;
  *((long *) (-3 + new_thing_299X)) = new_thing_297X;
  if (tracedP_288X) {
    *((long *) (1 + new_thing_299X)) = okay_291X;
    arg0K0 = next_295X;
    arg0K1 = new_thing_299X;
    arg0K2 = goners_292X;
    goto L4959;}
  else {
    *((long *) (1 + new_thing_299X)) = goners_292X;
    arg0K0 = next_295X;
    arg0K1 = okay_291X;
    arg0K2 = new_thing_299X;
    goto L4959;}}}
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
