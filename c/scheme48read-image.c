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
  goto L1676;}
 L1676: {
  ptr_3X = arg1K0;
  if ((ptr_3X < end_2X)) {
    descriptor_4X = *((long *) ptr_3X);
    if ((3 == (3 & descriptor_4X))) {
      arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_4X))) + delta_0X))));
      goto L1681;}
    else {
      arg0K0 = descriptor_4X;
      goto L1681;}}
  else {
    return;}}
 L1681: {
  d_5X = arg0K0;
  *((long *) ptr_3X) = d_5X;
  if ((2 == (3 & d_5X))) {
    if (((31 & (((d_5X)>>2))) < 17)) {
      goto L1695;}
    else {
      arg1K0 = (ptr_3X + (4 + (-4 & (3 + ((long)(((unsigned long)d_5X)>>8))))));
      goto L1676;}}
  else {
    goto L1695;}}
 L1695: {
  arg1K0 = (ptr_3X + 4);
  goto L1676;}
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
      goto L1853;}
    else {
      return;}}
  else {
    return;}}
 L1853: {
  i_8X = arg0K0;
  temp_9X = 1024 == i_8X;
  if (temp_9X) {
    return temp_9X;}
  else {
    link_10X = *((long *) ((((char *) (-3 + table_6X))) + (((i_8X)<<2))));
    if ((0 == (3 & link_10X))) {
      arg0K0 = (3 + (-4 & link_10X));
      goto L1799;}
    else {
      arg0K0 = link_10X;
      goto L1799;}}}
 L1799: {
  bucket_11X = arg0K0;
  if ((1 == bucket_11X)) {
    goto L1858;}
  else {
    bucket_12X = bucket_11X + delta_7X;
    if ((3 == (3 & bucket_12X))) {
      arg0K0 = (-4 & bucket_12X);
      goto L1810;}
    else {
      arg0K0 = bucket_12X;
      goto L1810;}}}
 L1858: {
  arg0K0 = (1 + i_8X);
  goto L1853;}
 L1810: {
  value_13X = arg0K0;
  addr_14X = (((char *) (-3 + table_6X))) + (((i_8X)<<2));
  S48_WRITE_BARRIER(table_6X, addr_14X, value_13X);
  *((long *) addr_14X) = value_13X;
  arg0K0 = bucket_12X;
  goto L1816;}
 L1816: {
  entry_15X = arg0K0;
  link_16X = *((long *) ((((char *) (-3 + entry_15X))) + 4));
  if ((0 == (3 & link_16X))) {
    arg0K0 = (3 + (-4 & link_16X));
    goto L1820;}
  else {
    arg0K0 = link_16X;
    goto L1820;}}
 L1820: {
  next_17X = arg0K0;
  if ((1 == next_17X)) {
    goto L1858;}
  else {
    next_18X = next_17X + delta_7X;
    if ((3 == (3 & next_18X))) {
      arg0K0 = (-4 & next_18X);
      goto L1831;}
    else {
      arg0K0 = next_18X;
      goto L1831;}}}
 L1831: {
  val_19X = arg0K0;
  addr_20X = (((char *) (-3 + entry_15X))) + 4;
  S48_WRITE_BARRIER(entry_15X, addr_20X, val_19X);
  *((long *) addr_20X) = val_19X;
  arg0K0 = next_18X;
  goto L1816;}
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
      goto L2053;}
    else {
      return;}}
  else {
    return;}}
 L2053: {
  i_23X = arg0K0;
  temp_24X = 1024 == i_23X;
  if (temp_24X) {
    return temp_24X;}
  else {
    link_25X = *((long *) ((((char *) (-3 + table_21X))) + (((i_23X)<<2))));
    if ((0 == (3 & link_25X))) {
      arg0K0 = (3 + (-4 & link_25X));
      goto L1999;}
    else {
      arg0K0 = link_25X;
      goto L1999;}}}
 L1999: {
  bucket_26X = arg0K0;
  if ((1 == bucket_26X)) {
    goto L2058;}
  else {
    bucket_27X = bucket_26X + delta_22X;
    if ((3 == (3 & bucket_27X))) {
      arg0K0 = (-4 & bucket_27X);
      goto L2010;}
    else {
      arg0K0 = bucket_27X;
      goto L2010;}}}
 L2058: {
  arg0K0 = (1 + i_23X);
  goto L2053;}
 L2010: {
  value_28X = arg0K0;
  addr_29X = (((char *) (-3 + table_21X))) + (((i_23X)<<2));
  S48_WRITE_BARRIER(table_21X, addr_29X, value_28X);
  *((long *) addr_29X) = value_28X;
  arg0K0 = bucket_27X;
  goto L2016;}
 L2016: {
  entry_30X = arg0K0;
  link_31X = *((long *) ((((char *) (-3 + entry_30X))) + 12));
  if ((0 == (3 & link_31X))) {
    arg0K0 = (3 + (-4 & link_31X));
    goto L2020;}
  else {
    arg0K0 = link_31X;
    goto L2020;}}
 L2020: {
  next_32X = arg0K0;
  if ((1 == next_32X)) {
    goto L2058;}
  else {
    next_33X = next_32X + delta_22X;
    if ((3 == (3 & next_33X))) {
      arg0K0 = (-4 & next_33X);
      goto L2031;}
    else {
      arg0K0 = next_33X;
      goto L2031;}}}
 L2031: {
  val_34X = arg0K0;
  addr_35X = (((char *) (-3 + entry_30X))) + 12;
  S48_WRITE_BARRIER(entry_30X, addr_35X, val_34X);
  *((long *) addr_35X) = val_34X;
  arg0K0 = next_33X;
  goto L2016;}
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
  goto L1577;}
 L1577: {
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
        goto L1577;}
      else {
        arg1K0 = (next_52X + (-4 & (3 + ((long)(((unsigned long)value_51X)>>8)))));
        goto L1577;}}
    else {
      arg1K0 = next_52X;
      goto L1577;}}
  else {
    return;}}
}
void s48_relocate_all(long delta_53X, char * new_begin_54X, char * new_hp_55X, long symbols_56X, long imported_57X, long exported_58X)
{

 {  relocate_symbol_tableB(symbols_56X, delta_53X);
  relocate_binding_tableB(imported_57X, delta_53X);
  relocate_binding_tableB(exported_58X, delta_53X);
  return relocate_image(delta_53X, new_begin_54X, new_hp_55X);}
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
      goto L2752;}
    else {
      goto L3491;}}
  else {
    ps_write_string("Can't open heap image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    ps_write_string((ps_error_string(status_62X)), (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2752: {
  if ((SeofPS)) {
    goto L2785;}
  else {
    goto L3455;}}
 L3491: {
  PS_READ_CHAR(port_61X, ch_63X, eofP_64X, status_65X)
  if (eofP_64X) {
    arg2K0 = eofP_64X;
    arg0K1 = status_65X;
    goto L2730;}
  else {
    if ((status_65X == NO_ERRORS)) {
      if ((12 == ch_63X)) {
        arg2K0 = 0;
        arg0K1 = status_65X;
        goto L2730;}
      else {
        goto L3491;}}
    else {
      arg2K0 = eofP_64X;
      arg0K1 = status_65X;
      goto L2730;}}}
 L2785: {
  if ((SeofPS)) {
    arg2K0 = 0;
    goto L2818;}
  else {
    arg0K0 = 0;
    goto L3431;}}
 L3455: {
  PS_READ_CHAR(port_61X, ch_66X, eofP_67X, status_68X)
  if (eofP_67X) {
    arg2K0 = eofP_67X;
    arg0K1 = status_68X;
    goto L2763;}
  else {
    if ((status_68X == NO_ERRORS)) {
      if ((10 == ch_66X)) {
        arg2K0 = 0;
        arg0K1 = status_68X;
        goto L2763;}
      else {
        goto L3455;}}
    else {
      arg2K0 = eofP_67X;
      arg0K1 = status_68X;
      goto L2763;}}}
 L2730: {
  eofP_69X = arg2K0;
  status_70X = arg0K1;
  if (eofP_69X) {
    SeofPS = 1;
    goto L2752;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2752;}
    else {
      SeofPS = 1;
      SstatusS = status_70X;
      goto L2752;}}}
 L2818: {
  same_versionP_71X = arg2K0;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2853;}
  else {
    PS_READ_INTEGER(port_61X, thing_72X, eofP_73X, status_74X)
    if (eofP_73X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2853;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_72X;
        goto L2853;}
      else {
        SeofPS = 1;
        SstatusS = status_74X;
        arg0K0 = -1;
        goto L2853;}}}}
 L3431: {
  i_75X = arg0K0;
  PS_READ_CHAR(port_61X, ch_76X, eofP_77X, status_78X)
  if (eofP_77X) {
    arg2K0 = 0;
    arg2K1 = eofP_77X;
    arg0K2 = status_78X;
    goto L2796;}
  else {
    if ((status_78X == NO_ERRORS)) {
      if ((i_75X == (strlen((char *) "Vanilla 23")))) {
        arg2K0 = (10 == ch_76X);
        arg2K1 = 0;
        arg0K2 = status_78X;
        goto L2796;}
      else {
        if ((ch_76X == (*("Vanilla 23" + i_75X)))) {
          arg0K0 = (1 + i_75X);
          goto L3431;}
        else {
          arg2K0 = 0;
          arg2K1 = 0;
          arg0K2 = status_78X;
          goto L2796;}}}
    else {
      arg2K0 = 0;
      arg2K1 = eofP_77X;
      arg0K2 = status_78X;
      goto L2796;}}}
 L2763: {
  eofP_79X = arg2K0;
  status_80X = arg0K1;
  if (eofP_79X) {
    SeofPS = 1;
    goto L2785;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L2785;}
    else {
      SeofPS = 1;
      SstatusS = status_80X;
      goto L2785;}}}
 L2853: {
  old_bytes_per_cell_81X = arg0K0;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2888;}
  else {
    PS_READ_INTEGER(port_61X, thing_82X, eofP_83X, status_84X)
    if (eofP_83X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2888;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_82X;
        goto L2888;}
      else {
        SeofPS = 1;
        SstatusS = status_84X;
        arg0K0 = -1;
        goto L2888;}}}}
 L2796: {
  thing_85X = arg2K0;
  eofP_86X = arg2K1;
  status_87X = arg0K2;
  if (eofP_86X) {
    SeofPS = 1;
    arg2K0 = 0;
    goto L2818;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      arg2K0 = thing_85X;
      goto L2818;}
    else {
      SeofPS = 1;
      SstatusS = status_87X;
      arg2K0 = 0;
      goto L2818;}}}
 L2888: {
  cells_88X = arg0K0;
  old_begin_89X = ((char *) (((cells_88X)<<2)));
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2925;}
  else {
    PS_READ_INTEGER(port_61X, thing_90X, eofP_91X, status_92X)
    if (eofP_91X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2925;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_90X;
        goto L2925;}
      else {
        SeofPS = 1;
        SstatusS = status_92X;
        arg0K0 = -1;
        goto L2925;}}}}
 L2925: {
  cells_93X = arg0K0;
  old_hp_94X = ((char *) (((cells_93X)<<2)));
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2962;}
  else {
    PS_READ_INTEGER(port_61X, thing_95X, eofP_96X, status_97X)
    if (eofP_96X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2962;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_95X;
        goto L2962;}
      else {
        SeofPS = 1;
        SstatusS = status_97X;
        arg0K0 = -1;
        goto L2962;}}}}
 L2962: {
  v_98X = arg0K0;
  SsymbolsS = v_98X;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L2998;}
  else {
    PS_READ_INTEGER(port_61X, thing_99X, eofP_100X, status_101X)
    if (eofP_100X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L2998;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_99X;
        goto L2998;}
      else {
        SeofPS = 1;
        SstatusS = status_101X;
        arg0K0 = -1;
        goto L2998;}}}}
 L2998: {
  v_102X = arg0K0;
  Simported_bindingsS = v_102X;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L3034;}
  else {
    PS_READ_INTEGER(port_61X, thing_103X, eofP_104X, status_105X)
    if (eofP_104X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L3034;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_103X;
        goto L3034;}
      else {
        SeofPS = 1;
        SstatusS = status_105X;
        arg0K0 = -1;
        goto L3034;}}}}
 L3034: {
  v_106X = arg0K0;
  Sexported_bindingsS = v_106X;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L3070;}
  else {
    PS_READ_INTEGER(port_61X, thing_107X, eofP_108X, status_109X)
    if (eofP_108X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L3070;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_107X;
        goto L3070;}
      else {
        SeofPS = 1;
        SstatusS = status_109X;
        arg0K0 = -1;
        goto L3070;}}}}
 L3070: {
  v_110X = arg0K0;
  Sresumer_recordsS = v_110X;
  if ((SeofPS)) {
    arg0K0 = -1;
    goto L3106;}
  else {
    PS_READ_INTEGER(port_61X, thing_111X, eofP_112X, status_113X)
    if (eofP_112X) {
      SeofPS = 1;
      arg0K0 = -1;
      goto L3106;}
    else {
      if (((SstatusS) == NO_ERRORS)) {
        arg0K0 = thing_111X;
        goto L3106;}
      else {
        SeofPS = 1;
        SstatusS = status_113X;
        arg0K0 = -1;
        goto L3106;}}}}
 L3106: {
  v_114X = arg0K0;
  Sstartup_procedureS = v_114X;
  if ((SeofPS)) {
    goto L3142;}
  else {
    goto L3362;}}
 L3142: {
  if (((SstatusS) == NO_ERRORS)) {
    if ((SeofPS)) {
      SstatusS = EDOM;
      ps_write_string("Premature EOF when reading image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L3231;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L3231;}}
    else {
      if (same_versionP_71X) {
        if ((4 == old_bytes_per_cell_81X)) {
          new_begin_115X = s48_initialize_heap(heap_size_60X, ((((3 + (old_hp_94X - old_begin_89X)))>>2)), old_begin_89X);
          got_116X = ps_read_block(port_61X, ((char *) new_begin_115X), 4, &eofP_117X, &status_118X);
          if ((status_118X == NO_ERRORS)) {
            if (eofP_117X) {
              arg2K0 = 0;
              arg3K1 = "Premature EOF when reading image file";
              goto L2564;}
            else {
              if ((got_116X < 4)) {
                arg2K0 = 0;
                arg3K1 = "Read returned too few bytes";
                goto L2564;}
              else {
                arg2K0 = 1;
                arg3K1 = "";
                goto L2564;}}}
          else {
            SstatusS = status_118X;
            arg2K0 = 0;
            arg3K1 = "Error reading from image file";
            goto L2564;}}
        else {
          ps_write_string("Incompatible bytes-per-cell in image", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L3291;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L3291;}}}
      else {
        ps_write_string("Format of image is incompatible with this version of system", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L3261;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L3261;}}}}
  else {
    ps_write_string("Error reading from image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L3206;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L3206;}}}
 L3362: {
  PS_READ_CHAR(port_61X, ch_119X, eofP_120X, status_121X)
  if (eofP_120X) {
    arg2K0 = eofP_120X;
    arg0K1 = status_121X;
    goto L3120;}
  else {
    if ((status_121X == NO_ERRORS)) {
      if ((12 == ch_119X)) {
        arg2K0 = 0;
        arg0K1 = status_121X;
        goto L3120;}
      else {
        goto L3362;}}
    else {
      arg2K0 = eofP_120X;
      arg0K1 = status_121X;
      goto L3120;}}}
 L3231: {
  status_122X = ps_close(port_61X);
  if ((status_122X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2564: {
  okayP_123X = arg2K0;
  string_124X = arg3K1;
  if (okayP_123X) {
    if ((1 == (*((long *) new_begin_115X)))) {
      arg4K0 = port_61X;
      arg1K1 = new_begin_115X;
      arg1K2 = old_begin_89X;
      arg1K3 = old_hp_94X;
      arg2K4 = 0;
      goto L2194;}
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
        goto L2194;}
      else {
        ps_write_string("Unable to correct byte order", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2661;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2661;}}}}
  else {
    ps_write_string(string_124X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2615;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2615;}}}
 L3291: {
  status_127X = ps_close(port_61X);
  if ((status_127X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3261: {
  status_128X = ps_close(port_61X);
  if ((status_128X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3206: {
  status_129X = ps_close(port_61X);
  if ((status_129X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L3120: {
  eofP_130X = arg2K0;
  status_131X = arg0K1;
  if (eofP_130X) {
    SeofPS = 1;
    goto L3142;}
  else {
    if (((SstatusS) == NO_ERRORS)) {
      goto L3142;}
    else {
      SeofPS = 1;
      SstatusS = status_131X;
      goto L3142;}}}
 L2194: {
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
      goto L2202;}
    else {
      if ((got_140X < need_139X)) {
        arg2K0 = 0;
        arg3K1 = "Read returned too few bytes";
        goto L2202;}
      else {
        arg2K0 = 1;
        arg3K1 = "";
        goto L2202;}}}
  else {
    SstatusS = status_142X;
    arg2K0 = 0;
    arg3K1 = "Error reading from image file";
    goto L2202;}}
 L2661: {
  status_143X = ps_close(port_61X);
  if ((status_143X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2615: {
  status_144X = ps_close(port_61X);
  if ((status_144X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2202: {
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
            goto L2234;}
          else {
            arg0K0 = descriptor_151X;
            goto L2234;}}
        else {
          ps_write_string("Error closing image file", (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          if (((SstatusS) == NO_ERRORS)) {
            goto L2401;}
          else {
            ps_write_string((ps_error_string((SstatusS))), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            goto L2401;}}}
      else {
        ps_write_string("Image file has extraneous data after image", (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        if (((SstatusS) == NO_ERRORS)) {
          goto L2371;}
        else {
          ps_write_string((ps_error_string((SstatusS))), (stderr));
          { long ignoreXX;
          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
          goto L2371;}}}
    else {
      ps_write_string("Error reading from image file", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      if (((SstatusS) == NO_ERRORS)) {
        goto L2341;}
      else {
        ps_write_string((ps_error_string((SstatusS))), (stderr));
        { long ignoreXX;
        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
        goto L2341;}}}
  else {
    ps_write_string(string_146X, (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    if (((SstatusS) == NO_ERRORS)) {
      goto L2311;}
    else {
      ps_write_string((ps_error_string((SstatusS))), (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      goto L2311;}}}
 L2234: {
  v_152X = arg0K0;
  Sstartup_procedureS = v_152X;
  descriptor_153X = SsymbolsS;
  if ((3 == (3 & descriptor_153X))) {
    arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_153X))) + delta_137X))));
    goto L2239;}
  else {
    arg0K0 = descriptor_153X;
    goto L2239;}}
 L2401: {
  status_154X = ps_close(port_132X);
  if ((status_154X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2371: {
  status_155X = ps_close(port_132X);
  if ((status_155X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2341: {
  status_156X = ps_close(port_132X);
  if ((status_156X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2311: {
  status_157X = ps_close(port_132X);
  if ((status_157X == NO_ERRORS)) {
    return -1;}
  else {
    ps_write_string("Error closing image file", (stderr));
    { long ignoreXX;
    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
    return -1;}}
 L2239: {
  v_158X = arg0K0;
  SsymbolsS = v_158X;
  descriptor_159X = Simported_bindingsS;
  if ((3 == (3 & descriptor_159X))) {
    arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_159X))) + delta_137X))));
    goto L2244;}
  else {
    arg0K0 = descriptor_159X;
    goto L2244;}}
 L2244: {
  v_160X = arg0K0;
  Simported_bindingsS = v_160X;
  descriptor_161X = Sexported_bindingsS;
  if ((3 == (3 & descriptor_161X))) {
    arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_161X))) + delta_137X))));
    goto L2249;}
  else {
    arg0K0 = descriptor_161X;
    goto L2249;}}
 L2249: {
  v_162X = arg0K0;
  Sexported_bindingsS = v_162X;
  descriptor_163X = Sresumer_recordsS;
  if ((3 == (3 & descriptor_163X))) {
    arg0K0 = (3 + (((long) ((((char *) (-3 + descriptor_163X))) + delta_137X))));
    goto L2254;}
  else {
    arg0K0 = descriptor_163X;
    goto L2254;}}
 L2254: {
  v_164X = arg0K0;
  Sresumer_recordsS = v_164X;
  if (reverseP_136X) {
    s48_reverse_byte_orderB(new_begin_133X, new_hp_138X);
    goto L2265;}
  else {
    goto L2265;}}
 L2265: {
  if ((0 == delta_137X)) {
    return 0;}
  else {
    relocate_symbol_tableB((SsymbolsS), delta_137X);
    relocate_binding_tableB((Simported_bindingsS), delta_137X);
    relocate_binding_tableB((Sexported_bindingsS), delta_137X);
    relocate_image(delta_137X, new_begin_133X, new_hp_138X);
    return 0;}}
}void
s48_read_image_init(void)
{
SstatusS = NO_ERRORS;
SeofPS = 0;
SinitializingPS = 1;
}
