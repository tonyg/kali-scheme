#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"
#include "scheme48heap.h"

static void relocate_image(long, char *, char *);
static char relocate_symbol_tableB(long, long);
static char relocate_binding_tableB(long, long);
long s48_startup_procedure(void);
long s48_initial_symbols(void);
long s48_initial_imported_bindings(void);
long s48_initial_exported_bindings(void);
long s48_resumer_records(void);
void s48_set_image_valuesB(long, long, long, long, long);
void s48_initializing_gc_root(void);
void s48_initialization_completeB(void);
void s48_reverse_byte_orderB(char *, char *);
void s48_relocate_all(long, char *, char *, long, long, long);
long s48_read_image(char*, long);
static long SstatusS;
static char SeofPS;
static long Sstartup_procedureS;
static long SsymbolsS;
static long Simported_bindingsS;
static long Sexported_bindingsS;
static long Sresumer_recordsS;
static char SinitializingPS;

static void relocate_image(long delta_0X, char * start_1X, char * end_2X)
{
  char * arg1K0;
  long arg0K0;
  long d_5X;
  long descriptor_4X;
  char * ptr_3X;
 {  arg1K0 = start_1X;
  goto L1670;}
 L1670: {
  ptr_3X = arg1K0;
  if ((ptr_3X < end_2X)) {
    descriptor_4X = *((long *) ptr_3X);
    if ((3 == (3 & descriptor_4X))) {
      arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_4X))) + delta_0X))));
      goto L1675;}
    else {
      arg0K0 = descriptor_4X;
      goto L1675;}}
  else {
    return;}}
 L1675: {
  d_5X = arg0K0;
  *((long *) ptr_3X) = d_5X;
  if ((2 == (3 & d_5X))) {
    if (((31 & (((d_5X)>>2))) < 17)) {
      goto L1689;}
    else {
      arg1K0 = (ptr_3X + (4 + (-4 & (3 + ((long)(((unsigned long)d_5X)>>8))))));
      goto L1670;}}
  else {
    goto L1689;}}
 L1689: {
  arg1K0 = (ptr_3X + 4);
  goto L1670;}
}
static char relocate_symbol_tableB(long table_6X, long delta_7X)
{
  long arg0K0;
  char * addr_20X;
  long val_19X;
  long next_18X;
  long next_17X;
  long link_16X;
  long entry_15X;
  char * addr_14X;
  long value_13X;
  long bucket_12X;
  long bucket_11X;
  long link_10X;
  char temp_9X;
  long i_8X;
 {  if ((3 == (3 & table_6X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + table_6X))) + -4))))>>2))))) {
      arg0K0 = 0;
      goto L1847;}
    else {
      return;}}
  else {
    return;}}
 L1847: {
  i_8X = arg0K0;
  temp_9X = 1024 == i_8X;
  if (temp_9X) {
    return temp_9X;}
  else {
    link_10X = *((long *) ((((char *) (-3 + table_6X))) + (((i_8X)<<2))));
    if ((0 == (3 & link_10X))) {
      arg0K0 = (3 + (-4 & link_10X));
      goto L1793;}
    else {
      arg0K0 = link_10X;
      goto L1793;}}}
 L1793: {
  bucket_11X = arg0K0;
  if ((1 == bucket_11X)) {
    goto L1852;}
  else {
    bucket_12X = bucket_11X + delta_7X;
    if ((3 == (3 & bucket_12X))) {
      arg0K0 = (-4 & bucket_12X);
      goto L1804;}
    else {
      arg0K0 = bucket_12X;
      goto L1804;}}}
 L1852: {
  arg0K0 = (1 + i_8X);
  goto L1847;}
 L1804: {
  value_13X = arg0K0;
  addr_14X = (((char *) (-3 + table_6X))) + (((i_8X)<<2));S48_WRITE_BARRIER(table_6X, addr_14X, value_13X);
  *((long *) addr_14X) = value_13X;
  arg0K0 = bucket_12X;
  goto L1810;}
 L1810: {
  entry_15X = arg0K0;
  link_16X = *((long *) ((((char *) (-3 + entry_15X))) + 4));
  if ((0 == (3 & link_16X))) {
    arg0K0 = (3 + (-4 & link_16X));
    goto L1814;}
  else {
    arg0K0 = link_16X;
    goto L1814;}}
 L1814: {
  next_17X = arg0K0;
  if ((1 == next_17X)) {
    goto L1852;}
  else {
    next_18X = next_17X + delta_7X;
    if ((3 == (3 & next_18X))) {
      arg0K0 = (-4 & next_18X);
      goto L1825;}
    else {
      arg0K0 = next_18X;
      goto L1825;}}}
 L1825: {
  val_19X = arg0K0;
  addr_20X = (((char *) (-3 + entry_15X))) + 4;S48_WRITE_BARRIER(entry_15X, addr_20X, val_19X);
  *((long *) addr_20X) = val_19X;
  arg0K0 = next_18X;
  goto L1810;}
}
static char relocate_binding_tableB(long table_21X, long delta_22X)
{
  long arg0K0;
  char * addr_35X;
  long val_34X;
  long next_33X;
  long next_32X;
  long link_31X;
  long entry_30X;
  char * addr_29X;
  long value_28X;
  long bucket_27X;
  long bucket_26X;
  long link_25X;
  char temp_24X;
  long i_23X;
 {  if ((3 == (3 & table_21X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + table_21X))) + -4))))>>2))))) {
      arg0K0 = 0;
      goto L2047;}
    else {
      return;}}
  else {
    return;}}
 L2047: {
  i_23X = arg0K0;
  temp_24X = 1024 == i_23X;
  if (temp_24X) {
    return temp_24X;}
  else {
    link_25X = *((long *) ((((char *) (-3 + table_21X))) + (((i_23X)<<2))));
    if ((0 == (3 & link_25X))) {
      arg0K0 = (3 + (-4 & link_25X));
      goto L1993;}
    else {
      arg0K0 = link_25X;
      goto L1993;}}}
 L1993: {
  bucket_26X = arg0K0;
  if ((1 == bucket_26X)) {
    goto L2052;}
  else {
    bucket_27X = bucket_26X + delta_22X;
    if ((3 == (3 & bucket_27X))) {
      arg0K0 = (-4 & bucket_27X);
      goto L2004;}
    else {
      arg0K0 = bucket_27X;
      goto L2004;}}}
 L2052: {
  arg0K0 = (1 + i_23X);
  goto L2047;}
 L2004: {
  value_28X = arg0K0;
  addr_29X = (((char *) (-3 + table_21X))) + (((i_23X)<<2));S48_WRITE_BARRIER(table_21X, addr_29X, value_28X);
  *((long *) addr_29X) = value_28X;
  arg0K0 = bucket_27X;
  goto L2010;}
 L2010: {
  entry_30X = arg0K0;
  link_31X = *((long *) ((((char *) (-3 + entry_30X))) + 12));
  if ((0 == (3 & link_31X))) {
    arg0K0 = (3 + (-4 & link_31X));
    goto L2014;}
  else {
    arg0K0 = link_31X;
    goto L2014;}}
 L2014: {
  next_32X = arg0K0;
  if ((1 == next_32X)) {
    goto L2052;}
  else {
    next_33X = next_32X + delta_22X;
    if ((3 == (3 & next_33X))) {
      arg0K0 = (-4 & next_33X);
      goto L2025;}
    else {
      arg0K0 = next_33X;
      goto L2025;}}}
 L2025: {
  val_34X = arg0K0;
  addr_35X = (((char *) (-3 + entry_30X))) + 12;S48_WRITE_BARRIER(entry_30X, addr_35X, val_34X);
  *((long *) addr_35X) = val_34X;
  arg0K0 = next_33X;
  goto L2010;}
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
void s48_set_image_valuesB(long startup_proc_36X, long symbols_37X, long imports_38X, long exports_39X, long records_40X)
{

 {  Sstartup_procedureS = startup_proc_36X;
  SsymbolsS = symbols_37X;
  Simported_bindingsS = imports_38X;
  Sexported_bindingsS = exports_39X;
  Sresumer_recordsS = records_40X;
  return;}
}
void s48_initializing_gc_root(void)
{
  long v_45X;
  long v_44X;
  long v_43X;
  long v_42X;
  long v_41X;
 {  if ((SinitializingPS)) {
    v_41X = s48_trace_value((Sstartup_procedureS));
    Sstartup_procedureS = v_41X;
    v_42X = s48_trace_value((SsymbolsS));
    SsymbolsS = v_42X;
    v_43X = s48_trace_value((Simported_bindingsS));
    Simported_bindingsS = v_43X;
    v_44X = s48_trace_value((Sexported_bindingsS));
    Sexported_bindingsS = v_44X;
    v_45X = s48_trace_value((Sresumer_recordsS));
    Sresumer_recordsS = v_45X;
    return;}
  else {
    return;}}
}
void s48_initialization_completeB(void)
{

 {  SinitializingPS = 0;
  return;}
}
void s48_reverse_byte_orderB(char * start_46X, char * end_47X)
{
  char * arg1K0;
  char * next_52X;
  long value_51X;
  long x_50X;
  long x_49X;
  char * ptr_48X;
 {  ps_write_string("Correcting byte order of resumed image.", (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg1K0 = start_46X;
  goto L1571;}
 L1571: {
  ptr_48X = arg1K0;
  if ((ptr_48X < end_47X)) {
    x_49X = *((unsigned char *) ptr_48X);
    *((unsigned char *) ptr_48X) = (*((unsigned char *) (ptr_48X + 3)));
    *((unsigned char *) (ptr_48X + 3)) = x_49X;
    x_50X = *((unsigned char *) (ptr_48X + 1));
    *((unsigned char *) (ptr_48X + 1)) = (*((unsigned char *) (ptr_48X + 2)));
    *((unsigned char *) (ptr_48X + 2)) = x_50X;
    value_51X = *((long *) ptr_48X);
    next_52X = ptr_48X + 4;
    if ((2 == (3 & value_51X))) {
      if (((31 & (((value_51X)>>2))) < 17)) {
        arg1K0 = next_52X;
        goto L1571;}
      else {
        arg1K0 = (next_52X + (-4 & (3 + ((long)(((unsigned long)value_51X)>>8)))));
        goto L1571;}}
    else {
      arg1K0 = next_52X;
      goto L1571;}}
  else {
    return;}}
}
void s48_relocate_all(long delta_53X, char * new_begin_54X, char * new_hp_55X, long symbols_56X, long imported_57X, long exported_58X)
{

 {relocate_symbol_tableB(symbols_56X, delta_53X);relocate_binding_tableB(imported_57X, delta_53X);relocate_binding_tableB(exported_58X, delta_53X);
  relocate_image(delta_53X, new_begin_54X, new_hp_55X);
  return;}
}
long s48_read_image(char *filename_59X, long heap_size_60X)
{
  char * arg1K3;
  char * arg1K2;
  char * arg1K1;
  FILE * arg4K0;
  char *arg3K1;
  char arg2K1;
  char arg2K4;
  char arg2K0;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  long v_164X;
  long descriptor_163X;
  long v_162X;
  long descriptor_161X;
  long v_160X;
  long descriptor_159X;
  long v_158X;
  long status_157X;
  long status_156X;
  long status_155X;
  long status_154X;
  long descriptor_153X;
  long v_152X;
  long descriptor_151X;
  long status_150X;
  long status_149X;
  char eofP_148X;
  char v_147X;
  char *string_146X;
  char okayP_145X;
  long status_144X;
  long status_143X;
  long status_142X;
  char eofP_141X;
  long got_140X;
  long need_139X;
  char * new_hp_138X;
  long delta_137X;
  char reverseP_136X;
  char * old_hp_135X;
  char * old_begin_134X;
  char * new_begin_133X;
  FILE * port_132X;
  long status_131X;
  char eofP_130X;
  long status_129X;
  long status_128X;
  long status_127X;
  long x_126X;
  long x_125X;
  char *string_124X;
  char okayP_123X;
  long status_122X;
  long status_121X;
  char eofP_120X;
  char ch_119X;
  long status_118X;
  char eofP_117X;
  long got_116X;
  char * new_begin_115X;
  long v_114X;
  long status_113X;
  char eofP_112X;
  long thing_111X;
  long v_110X;
  long status_109X;
  char eofP_108X;
  long thing_107X;
  long v_106X;
  long status_105X;
  char eofP_104X;
  long thing_103X;
  long v_102X;
  long status_101X;
  char eofP_100X;
  long thing_99X;
  long v_98X;
  long status_97X;
  char eofP_96X;
  long thing_95X;
  char * old_hp_94X;
  long cells_93X;
  long status_92X;
  char eofP_91X;
  long thing_90X;
  char * old_begin_89X;
  long cells_88X;
  long status_87X;
  char eofP_86X;
  char thing_85X;
  long status_84X;
  char eofP_83X;
  long thing_82X;
  long old_bytes_per_cell_81X;
  long status_80X;
  char eofP_79X;
  long status_78X;
  char eofP_77X;
  char ch_76X;
  long i_75X;
  long status_74X;
  char eofP_73X;
  long thing_72X;
  char same_versionP_71X;
  long status_70X;
  char eofP_69X;
  long status_68X;
  char eofP_67X;
  char ch_66X;
  long status_65X;
  char eofP_64X;
  char ch_63X;
  long status_62X;
  FILE * port_61X;
 {  port_61X = ps_open_input_file(filename_59X, &status_62X);
  if ((status_62X == NO_ERRORS)) {
    SstatusS = NO_ERRORS;
    SeofPS = 0;
    if ((SeofPS)) {
      goto L2746;}
    else {
      goto L3485;}}
  else {
    ps_write_string("Can't open heap image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    ps_write_string((ps_error_string(status_62X)), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2746: {
  if ((SeofPS)) {
    goto L2779;}
  else {
    goto L3449;}}
 L3485: {
  PS_READ_CHAR(port_61X, ch_63X, eofP_64X, status_65X)
  if (eofP_64X) {
    arg2K0 = eofP_64X;
    arg0K1 = status_65X;
    goto L2724;}
  else {
    if ((status_65X == NO_ERRORS)) {
      if ((12 == ch_63X)) {
        arg2K0 = 0;
        arg0K1 = status_65X;
        goto L2724;}
      else {
        goto L3485;}}
    else {
      arg2K0 = eofP_64X;
      arg0K1 = status_65X;
      goto L2724;}}}
 L2779: {
  if ((SeofPS)) {
    arg2K0 = 0;
    goto L2812;}
  else {
    arg0K0 = 0;
    goto L3425;}}
 L3449: {
  PS_READ_CHAR(port_61X, ch_66X, eofP_67X, status_68X)
  if (eofP_67X) {
    arg2K0 = eofP_67X;
    arg0K1 = status_68X;
    goto L2757;}
  else {
    if ((status_68X == NO_ERRORS)) {
      if ((10 == ch_66X)) {
        arg2K0 = 0;
        arg0K1 = status_68X;
        goto L2757;}
      else {
        goto L3449;}}
    else {
      arg2K0 = eofP_67X;
      arg0K1 = status_68X;
      goto L2757;}}}
 L2724: {
  eofP_69X = arg2K0;
  status_70X = arg0K1;
  if (eofP_69X) {
    SeofPS = 1;
    goto L2746;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2746;}
    else {
      SeofPS = 1;
      SstatusS = status_70X;
      goto L2746;}}}
 L2812: {
  same_versionP_71X = arg2K0;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2847;}
  else {
    PS_READ_INTEGER(port_61X, thing_72X, eofP_73X, status_74X)
    if (eofP_73X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2847;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_72X;
        goto L2847;}
      else {
        SeofPS = 1;
        SstatusS = status_74X;
        arg0K0 = -1;
        goto L2847;}}}}
 L3425: {
  i_75X = arg0K0;
  PS_READ_CHAR(port_61X, ch_76X, eofP_77X, status_78X)
  if (eofP_77X) {
    arg2K0 = 0;
    arg2K1 = eofP_77X;
    arg0K2 = status_78X;
    goto L2790;}
  else {
    if ((status_78X == NO_ERRORS)) {
      if ((i_75X == (strlen((char *) "Vanilla 24")))) {
        arg2K0 = (10 == ch_76X);
        arg2K1 = 0;
        arg0K2 = status_78X;
        goto L2790;}
      else {
        if ((ch_76X == (*("Vanilla 24" + i_75X)))) {
          arg0K0 = (1 + i_75X);
          goto L3425;}
        else {
          arg2K0 = 0;
          arg2K1 = 0;
          arg0K2 = status_78X;
          goto L2790;}}}
    else {
      arg2K0 = 0;
      arg2K1 = eofP_77X;
      arg0K2 = status_78X;
      goto L2790;}}}
 L2757: {
  eofP_79X = arg2K0;
  status_80X = arg0K1;
  if (eofP_79X) {
    SeofPS = 1;
    goto L2779;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2779;}
    else {
      SeofPS = 1;
      SstatusS = status_80X;
      goto L2779;}}}
 L2847: {
  old_bytes_per_cell_81X = arg0K0;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2882;}
  else {
    PS_READ_INTEGER(port_61X, thing_82X, eofP_83X, status_84X)
    if (eofP_83X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2882;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_82X;
        goto L2882;}
      else {
        SeofPS = 1;
        SstatusS = status_84X;
        arg0K0 = -1;
        goto L2882;}}}}
 L2790: {
  thing_85X = arg2K0;
  eofP_86X = arg2K1;
  status_87X = arg0K2;
  if (eofP_86X) {
    SeofPS = 1;
    arg2K0 = 0;
    goto L2812;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      arg2K0 = thing_85X;
      goto L2812;}
    else {
      SeofPS = 1;
      SstatusS = status_87X;
      arg2K0 = 0;
      goto L2812;}}}
 L2882: {
  cells_88X = arg0K0;
  old_begin_89X = ((char *) (((cells_88X)<<2)));
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2919;}
  else {
    PS_READ_INTEGER(port_61X, thing_90X, eofP_91X, status_92X)
    if (eofP_91X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2919;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_90X;
        goto L2919;}
      else {
        SeofPS = 1;
        SstatusS = status_92X;
        arg0K0 = -1;
        goto L2919;}}}}
 L2919: {
  cells_93X = arg0K0;
  old_hp_94X = ((char *) (((cells_93X)<<2)));
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2956;}
  else {
    PS_READ_INTEGER(port_61X, thing_95X, eofP_96X, status_97X)
    if (eofP_96X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2956;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_95X;
        goto L2956;}
      else {
        SeofPS = 1;
        SstatusS = status_97X;
        arg0K0 = -1;
        goto L2956;}}}}
 L2956: {
  v_98X = arg0K0;
  SsymbolsS = v_98X;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2992;}
  else {
    PS_READ_INTEGER(port_61X, thing_99X, eofP_100X, status_101X)
    if (eofP_100X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2992;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_99X;
        goto L2992;}
      else {
        SeofPS = 1;
        SstatusS = status_101X;
        arg0K0 = -1;
        goto L2992;}}}}
 L2992: {
  v_102X = arg0K0;
  Simported_bindingsS = v_102X;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L3028;}
  else {
    PS_READ_INTEGER(port_61X, thing_103X, eofP_104X, status_105X)
    if (eofP_104X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L3028;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_103X;
        goto L3028;}
      else {
        SeofPS = 1;
        SstatusS = status_105X;
        arg0K0 = -1;
        goto L3028;}}}}
 L3028: {
  v_106X = arg0K0;
  Sexported_bindingsS = v_106X;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L3064;}
  else {
    PS_READ_INTEGER(port_61X, thing_107X, eofP_108X, status_109X)
    if (eofP_108X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L3064;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_107X;
        goto L3064;}
      else {
        SeofPS = 1;
        SstatusS = status_109X;
        arg0K0 = -1;
        goto L3064;}}}}
 L3064: {
  v_110X = arg0K0;
  Sresumer_recordsS = v_110X;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L3100;}
  else {
    PS_READ_INTEGER(port_61X, thing_111X, eofP_112X, status_113X)
    if (eofP_112X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L3100;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_111X;
        goto L3100;}
      else {
        SeofPS = 1;
        SstatusS = status_113X;
        arg0K0 = -1;
        goto L3100;}}}}
 L3100: {
  v_114X = arg0K0;
  Sstartup_procedureS = v_114X;
  if ((SeofPS)) {
    goto L3136;}
  else {
    goto L3356;}}
 L3136: {
  if (((SstatusS) == NO_ERRORS)) {
    if ((SeofPS)) {
      SstatusS = EDOM;
      ps_write_string("Premature EOF when reading image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L3225;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L3225;}}
    else {
      if (same_versionP_71X) {
        if ((4 == old_bytes_per_cell_81X)) {
          new_begin_115X = s48_initialize_heap(heap_size_60X, ((((3 + (old_hp_94X - old_begin_89X)))>>2)), old_begin_89X);
          got_116X = ps_read_block(port_61X, ((char *) new_begin_115X), 4, &eofP_117X, &status_118X);
          if ((status_118X == NO_ERRORS)) {
            if (eofP_117X) {
              arg2K0 = 0;
              arg3K1 = "Premature EOF when reading image file";
              goto L2558;}
            else {
              if ((got_116X < 4)) {
                arg2K0 = 0;
                arg3K1 = "Read returned too few bytes";
                goto L2558;}
              else {
                arg2K0 = 1;
                arg3K1 = "";
                goto L2558;}}}
          else {
            SstatusS = status_118X;
            arg2K0 = 0;
            arg3K1 = "Error reading from image file";
            goto L2558;}}
        else {
          ps_write_string("Incompatible bytes-per-cell in image", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L3285;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L3285;}}}
      else {
        ps_write_string("Format of image is incompatible with this version of system", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L3255;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L3255;}}}}
  else {
    ps_write_string("Error reading from image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L3200;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L3200;}}}
 L3356: {
  PS_READ_CHAR(port_61X, ch_119X, eofP_120X, status_121X)
  if (eofP_120X) {
    arg2K0 = eofP_120X;
    arg0K1 = status_121X;
    goto L3114;}
  else {
    if ((status_121X == NO_ERRORS)) {
      if ((12 == ch_119X)) {
        arg2K0 = 0;
        arg0K1 = status_121X;
        goto L3114;}
      else {
        goto L3356;}}
    else {
      arg2K0 = eofP_120X;
      arg0K1 = status_121X;
      goto L3114;}}}
 L3225: {
  status_122X = ps_close(port_61X);
  if ((status_122X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2558: {
  okayP_123X = arg2K0;
  string_124X = arg3K1;
  if (okayP_123X) {
    if ((1 == (*((long *) new_begin_115X)))) {
      arg4K0 = port_61X;
      arg1K1 = new_begin_115X;
      arg1K2 = old_begin_89X;
      arg1K3 = old_hp_94X;
      arg2K4 = 0;
      goto L2188;}
    else {
      x_125X = *((unsigned char *) new_begin_115X);
      *((unsigned char *) new_begin_115X) = (*((unsigned char *) (new_begin_115X + 3)));
      *((unsigned char *) (new_begin_115X + 3)) = x_125X;
      x_126X = *((unsigned char *) (new_begin_115X + 1));
      *((unsigned char *) (new_begin_115X + 1)) = (*((unsigned char *) (new_begin_115X + 2)));
      *((unsigned char *) (new_begin_115X + 2)) = x_126X;
      if ((1 == (*((long *) new_begin_115X)))) {
        arg4K0 = port_61X;
        arg1K1 = new_begin_115X;
        arg1K2 = old_begin_89X;
        arg1K3 = old_hp_94X;
        arg2K4 = 1;
        goto L2188;}
      else {
        ps_write_string("Unable to correct byte order", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2655;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2655;}}}}
  else {
    ps_write_string(string_124X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2609;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2609;}}}
 L3285: {
  status_127X = ps_close(port_61X);
  if ((status_127X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3255: {
  status_128X = ps_close(port_61X);
  if ((status_128X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3200: {
  status_129X = ps_close(port_61X);
  if ((status_129X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3114: {
  eofP_130X = arg2K0;
  status_131X = arg0K1;
  if (eofP_130X) {
    SeofPS = 1;
    goto L3136;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L3136;}
    else {
      SeofPS = 1;
      SstatusS = status_131X;
      goto L3136;}}}
 L2188: {
  port_132X = arg4K0;
  new_begin_133X = arg1K1;
  old_begin_134X = arg1K2;
  old_hp_135X = arg1K3;
  reverseP_136X = arg2K4;
  delta_137X = new_begin_133X - old_begin_134X;
  new_hp_138X = old_hp_135X + delta_137X;
  need_139X = old_hp_135X - old_begin_134X;
  got_140X = ps_read_block(port_132X, ((char *) new_begin_133X), need_139X, &eofP_141X, &status_142X);
  if ((status_142X == NO_ERRORS)) {
    if (eofP_141X) {
      arg2K0 = 0;
      arg3K1 = "Premature EOF when reading image file";
      goto L2196;}
    else {
      if ((got_140X < need_139X)) {
        arg2K0 = 0;
        arg3K1 = "Read returned too few bytes";
        goto L2196;}
      else {
        arg2K0 = 1;
        arg3K1 = "";
        goto L2196;}}}
  else {
    SstatusS = status_142X;
    arg2K0 = 0;
    arg3K1 = "Error reading from image file";
    goto L2196;}}
 L2655: {
  status_143X = ps_close(port_61X);
  if ((status_143X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2609: {
  status_144X = ps_close(port_61X);
  if ((status_144X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2196: {
  okayP_145X = arg2K0;
  string_146X = arg3K1;
  PS_READ_CHAR(port_132X, v_147X, eofP_148X, status_149X)
  if (okayP_145X) {
    if ((status_149X == NO_ERRORS)) {
      if (eofP_148X) {
        status_150X = ps_close(port_132X);
        if ((status_150X == NO_ERRORS)) {
          descriptor_151X = Sstartup_procedureS;
          if ((3 == (3 & descriptor_151X))) {
            arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_151X))) + delta_137X))));
            goto L2228;}
          else {
            arg0K0 = descriptor_151X;
            goto L2228;}}
        else {
          ps_write_string("Error closing image file", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L2395;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L2395;}}}
      else {
        ps_write_string("Image file has extraneous data after image", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2365;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2365;}}}
    else {
      ps_write_string("Error reading from image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L2335;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L2335;}}}
  else {
    ps_write_string(string_146X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2305;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2305;}}}
 L2228: {
  v_152X = arg0K0;
  Sstartup_procedureS = v_152X;
  descriptor_153X = SsymbolsS;
  if ((3 == (3 & descriptor_153X))) {
    arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_153X))) + delta_137X))));
    goto L2233;}
  else {
    arg0K0 = descriptor_153X;
    goto L2233;}}
 L2395: {
  status_154X = ps_close(port_132X);
  if ((status_154X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2365: {
  status_155X = ps_close(port_132X);
  if ((status_155X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2335: {
  status_156X = ps_close(port_132X);
  if ((status_156X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2305: {
  status_157X = ps_close(port_132X);
  if ((status_157X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2233: {
  v_158X = arg0K0;
  SsymbolsS = v_158X;
  descriptor_159X = Simported_bindingsS;
  if ((3 == (3 & descriptor_159X))) {
    arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_159X))) + delta_137X))));
    goto L2238;}
  else {
    arg0K0 = descriptor_159X;
    goto L2238;}}
 L2238: {
  v_160X = arg0K0;
  Simported_bindingsS = v_160X;
  descriptor_161X = Sexported_bindingsS;
  if ((3 == (3 & descriptor_161X))) {
    arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_161X))) + delta_137X))));
    goto L2243;}
  else {
    arg0K0 = descriptor_161X;
    goto L2243;}}
 L2243: {
  v_162X = arg0K0;
  Sexported_bindingsS = v_162X;
  descriptor_163X = Sresumer_recordsS;
  if ((3 == (3 & descriptor_163X))) {
    arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_163X))) + delta_137X))));
    goto L2248;}
  else {
    arg0K0 = descriptor_163X;
    goto L2248;}}
 L2248: {
  v_164X = arg0K0;
  Sresumer_recordsS = v_164X;
  if (reverseP_136X) {s48_reverse_byte_orderB(new_begin_133X, new_hp_138X);
    goto L2259;}
  else {
    goto L2259;}}
 L2259: {
  if ((0 == delta_137X)) {
    return 0;}
  else {relocate_symbol_tableB((SsymbolsS), delta_137X);relocate_binding_tableB((Simported_bindingsS), delta_137X);relocate_binding_tableB((Sexported_bindingsS), delta_137X);relocate_image(delta_137X, new_begin_133X, new_hp_138X);
    return 0;}}
}void
s48_read_image_init(void)
{
SstatusS = NO_ERRORS;
SeofPS = 0;
SinitializingPS = 1;
}
