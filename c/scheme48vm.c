#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm-prelude.h"

static char add_more_channels(long);
static void enqueue_channelB(long, long);
static long close_channelB(long);
static void push_continuationB(char *, long);
static long enter_string(char*, long);
static char HcleanerB2160(long);
static void push_exception_continuationB(long, long);
static long make_registered_channel(long, long, long, long, long*);
static long channel_close_error(long, long, long);
static void HtopD8861(void);
static long Hlookup2102(long, long, long);
static long Hlookup2083(long, long, long);
static void HtopD8854(void);
static long collect_saving_temps(long, long, long*);
void s48_set_extension_valueB(long);
void s48_note_event(void);
void s48_set_os_signal(long, long);
long s48_exported_bindings(void);
char * s48_set_gc_roots_baseB(void);
char s48_release_gc_roots_baseB(char *);
void s48_disable_interruptsB(void);
void s48_push_gc_rootsB(char *, long);
void s48_register_gc_rootB(char *);
void s48_stack_setB(long, long);
long s48_stack_ref(long);
void s48_push(long);
long s48_resetup_external_exception(long, long);
void s48_enable_interruptsB(void);
char s48_pop_gc_rootsB(void);
void s48_mark_traced_channels_closedB(void);
long s48_set_channel_os_index(long, long);
long s48_cleaned_imported_bindings(void);
long s48_copy_symbol_table(void);
void s48_setup_external_exception(long, long);
void s48_close_channel(long);
char s48_warn_about_undefined_imported_bindings(void);
void s48_define_exported_binding(char*, long);
long s48_add_channel(long, long, long);
long s48_get_imported_binding(char*);
long s48_allocate_stob(long, long);
void s48_initialize_vm(char *, long);
long s48_restart(long, long);
long s48_call_startup_procedure(char**, long);
static long Snumber_of_channelsS;
static long *Svm_channelsS;
static long Spending_channels_headS;
static long Spending_channels_tailS;
static char * Sstack_beginS;
static char * Sstack_endS;
static char * SstackS;
static char * Sstack_limitS;
static long ScontS;
static long Sbottom_of_stackS;
static char Sstack_warningPS;
static long SenvS;
static long StemplateS;
static char * Scode_pointerS;
static long SvalS;
static long Sexception_handlersS;
static long Sinterrupt_handlersS;
static long Scurrent_threadS;
static long Ssession_dataS;
static long Sfinalizer_alistS;
static long Sfinalize_theseS;
static long Senabled_interruptsS;
static long Spending_interruptsS;
static long Sinterrupted_templateS;
static long Sinterrupt_templateS;
static long Sexception_templateS;
static long Ssaved_pcS;
static long Slosing_opcodeS;
static long Sos_signal_typeS;
static long Sos_signal_argumentS;
static char Sexternal_exceptionPS;
static long Sexternal_exception_nargsS;
static long Simported_bindingsS;
static long Sexported_bindingsS;
static long Sthe_symbol_tableS;
static char * Sexternal_root_stackS;
static char * Sexternal_root_stack_baseS;
static char * Spermanent_external_rootsS;
static void (*Sgc_root_procS)(void);
static void (*Spost_gc_cleanupS)(void);
char s48_Spending_interruptPS;
long s48_Sextension_valueS;
long s48_Scallback_return_stack_blockS;
char s48_Spending_eventsPS;

static char add_more_channels(long index_5X)
{
  long arg0K0;
  long i_11X;
  long i_10X;
  long *new_vm_channels_9X;
  long new_count_8X;
  long y_7X;
  long x_6X;
 {  x_6X = 1 + index_5X;
  y_7X = 8 + (Snumber_of_channelsS);
  if ((x_6X < y_7X)) {
    arg0K0 = y_7X;
    goto L2366;}
  else {
    arg0K0 = x_6X;
    goto L2366;}}
 L2366: {
  new_count_8X = arg0K0;
  new_vm_channels_9X = (long*)malloc(sizeof(long) * new_count_8X);
  if ((NULL == new_vm_channels_9X)) {
    return 0;}
  else {
    arg0K0 = 0;
    goto L2378;}}
 L2378: {
  i_10X = arg0K0;
  if ((i_10X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L2393;}
  else {
    *(new_vm_channels_9X + i_10X) = (*((Svm_channelsS) + i_10X));
    arg0K0 = (1 + i_10X);
    goto L2378;}}
 L2393: {
  i_11X = arg0K0;
  if ((i_11X == new_count_8X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_9X;
    Snumber_of_channelsS = new_count_8X;
    return 1;}
  else {
    *(new_vm_channels_9X + i_11X) = 1;
    arg0K0 = (1 + i_11X);
    goto L2393;}}
}
static void enqueue_channelB(long index_12X, long status_13X)
{
  char * addr_18X;
  long x_17X;
  char * addr_16X;
  long val_15X;
  long channel_14X;
 {  channel_14X = *((Svm_channelsS) + index_12X);
  val_15X = ((status_13X)<<2);
  addr_16X = (((char *) (-3 + channel_14X))) + 16;
  S48_WRITE_BARRIER(channel_14X, addr_16X, val_15X);
  *((long *) addr_16X) = val_15X;
  if ((1 == (*((long *) ((((char *) (-3 + channel_14X))) + 12))))) {
    if ((1 == (Spending_channels_headS))) {
      Spending_channels_headS = channel_14X;
      Spending_channels_tailS = channel_14X;
      return;}
    else {
      x_17X = Spending_channels_tailS;
      addr_18X = (((char *) (-3 + x_17X))) + 12;
      S48_WRITE_BARRIER(x_17X, addr_18X, channel_14X);
      *((long *) addr_18X) = channel_14X;
      Spending_channels_tailS = channel_14X;
      return;}}
  else {
    return;}}
}
static long close_channelB(long channel_19X)
{
  long arg0K0;
  char * addr_27X;
  long status_26X;
  long v_25X;
  long v_24X;
  long v_23X;
  long v_22X;
  long x_21X;
  long os_index_20X;
 {  os_index_20X = (((*((long *) ((((char *) (-3 + channel_19X))) + 8))))>>2);
  x_21X = *((long *) ((((char *) (-3 + channel_19X))) + 16));
  if ((5 == x_21X)) {
    v_22X = ps_abort_fd_op(os_index_20X);
    enqueue_channelB(os_index_20X, v_22X);
    goto L7996;}
  else {
    goto L7996;}}
 L7996: {
  v_23X = *((long *) (((char *) (-3 + channel_19X))));
  if ((4 == v_23X)) {
    goto L8011;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_19X))))))) {
      goto L8011;}
    else {
      v_24X = ps_close_fd(os_index_20X);
      arg0K0 = v_24X;
      goto L8018;}}}
 L8011: {
  v_25X = ps_close_fd(os_index_20X);
  arg0K0 = v_25X;
  goto L8018;}
 L8018: {
  status_26X = arg0K0;
  *((Svm_channelsS) + os_index_20X) = 1;
  addr_27X = ((char *) (-3 + channel_19X));
  S48_WRITE_BARRIER(channel_19X, addr_27X, 0);
  *((long *) addr_27X) = 0;
  return status_26X;}
}
static void push_continuationB(char * code_pointer_28X, long size_29X)
{
  long cont_32X;
  long template_31X;
  long pc_30X;
 {  pc_30X = (((code_pointer_28X - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2);
  template_31X = StemplateS;
  SstackS = ((SstackS) + -20);
  *((long *) ((SstackS) + 4)) = (128 | (4134 + (((size_29X)<<10))));
  cont_32X = 3 + (((long) ((SstackS) + 8)));
  *((long *) ((((char *) (-3 + cont_32X))) + 4)) = pc_30X;
  *((long *) ((((char *) (-3 + cont_32X))) + 8)) = template_31X;
  *((long *) ((((char *) (-3 + cont_32X))) + 12)) = (SenvS);
  *((long *) (((char *) (-3 + cont_32X)))) = (ScontS);
  ScontS = cont_32X;
  return;}
}
static long enter_string(char *string_33X, long key_34X)
{
  long arg0K0;
  long i_39X;
  long string_38X;
  char * addr_37X;
  long len_36X;
  long z_35X;
 {  z_35X = strlen((char *) string_33X);
  len_36X = 1 + z_35X;
  addr_37X = ALLOCATE_SPACE(16, (4 + len_36X));
  *((long *) addr_37X) = (66 + (((len_36X)<<8)));
  string_38X = 3 + (((long) (addr_37X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_38X))) + z_35X)) = 0;
  arg0K0 = 0;
  goto L9526;}
 L9526: {
  i_39X = arg0K0;
  if ((i_39X < z_35X)) {
    *((unsigned char *) ((((char *) (-3 + string_38X))) + i_39X)) = ((*(string_33X + i_39X)));
    arg0K0 = (1 + i_39X);
    goto L9526;}
  else {
    return string_38X;}}
}
static char HcleanerB2160(long table_40X)
{
  long arg0K1;
  long arg0K0;
  long v_51X;
  char * addr_50X;
  long new_foo_49X;
  char v_48X;
  char * addr_47X;
  long okay_46X;
  long foo_45X;
  long foo_44X;
  char temp_43X;
  long i_42X;
  long table_41X;
 {  table_41X = s48_trace_value(table_40X);
  arg0K0 = 0;
  goto L9716;}
 L9716: {
  i_42X = arg0K0;
  temp_43X = 1024 == i_42X;
  if (temp_43X) {
    return temp_43X;}
  else {
    foo_44X = *((long *) ((((char *) (-3 + table_41X))) + (((i_42X)<<2))));
    arg0K0 = foo_44X;
    arg0K1 = 1;
    goto L9736;}}
 L9736: {
  foo_45X = arg0K0;
  okay_46X = arg0K1;
  if ((1 == foo_45X)) {
    addr_47X = (((char *) (-3 + table_41X))) + (((i_42X)<<2));
    S48_WRITE_BARRIER(table_41X, addr_47X, okay_46X);
    *((long *) addr_47X) = okay_46X;
    arg0K0 = (1 + i_42X);
    goto L9716;}
  else {
    v_48X = s48_extantP(foo_45X);
    if (v_48X) {
      new_foo_49X = s48_trace_value(foo_45X);
      addr_50X = (((char *) (-3 + new_foo_49X))) + 4;
      S48_WRITE_BARRIER(new_foo_49X, addr_50X, okay_46X);
      *((long *) addr_50X) = okay_46X;
      arg0K0 = new_foo_49X;
      goto L9741;}
    else {
      arg0K0 = okay_46X;
      goto L9741;}}}
 L9741: {
  v_51X = arg0K0;
  arg0K0 = (*((long *) ((((char *) (-3 + foo_45X))) + 4)));
  arg0K1 = v_51X;
  goto L9736;}
}
static void push_exception_continuationB(long exception_52X, long instruction_size_53X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long v_58X;
  long i_57X;
  char * p_56X;
  long tem_55X;
  long opcode_54X;
 {  opcode_54X = *((unsigned char *) (Scode_pointerS));
  *((long *) (SstackS)) = (((instruction_size_53X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_52X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (StemplateS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  SstackS = ((SstackS) + -4);
  tem_55X = Sexception_templateS;
  StemplateS = tem_55X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_55X))))))));
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L9949;}
 L9949: {
  p_56X = arg1K0;
  i_57X = arg0K1;
  if ((2 == (3 & (*((long *) p_56X))))) {
    if ((22 == (*((long *) p_56X)))) {
      arg0K0 = (-1 + i_57X);
      goto L9890;}
    else {
      arg0K0 = i_57X;
      goto L9890;}}
  else {
    arg1K0 = (p_56X + 4);
    arg0K1 = (1 + i_57X);
    goto L9949;}}
 L9890: {
  v_58X = arg0K0;
  push_continuationB((Scode_pointerS), v_58X);
  *((long *) (SstackS)) = (((opcode_54X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_52X)<<2));
  SstackS = ((SstackS) + -4);
  return;}
}
static long make_registered_channel(long mode_59X, long id_60X, long os_index_61X, long key_62X, long *TT0)
{
  long x_66X;
  char * addr_65X;
  char x_64X;
  char temp_63X;
 {  temp_63X = os_index_61X < (Snumber_of_channelsS);
  if (temp_63X) {
    goto L10816;}
  else {
    x_64X = add_more_channels(os_index_61X);
    if (x_64X) {
      goto L10816;}
    else {
      *TT0 = 9;
      return 1;}}}
 L10816: {
  if ((1 == (*((Svm_channelsS) + os_index_61X)))) {
    addr_65X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_65X) = 5142;
    x_66X = 3 + (((long) (addr_65X + 4)));
    *((long *) (((char *) (-3 + x_66X)))) = (((mode_59X)<<2));
    *((long *) ((((char *) (-3 + x_66X))) + 4)) = id_60X;
    *((long *) ((((char *) (-3 + x_66X))) + 8)) = (((os_index_61X)<<2));
    *((long *) ((((char *) (-3 + x_66X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_66X))) + 16)) = 1;
    *((Svm_channelsS) + os_index_61X) = x_66X;
    *TT0 = 9;
    return x_66X;}
  else {
    *TT0 = 11;
    return 1;}}
}
static long channel_close_error(long status_67X, long index_68X, long id_69X)
{
  long v_70X;
 {  ps_write_string("Error: ", (stderr));
  ps_write_string((ps_error_string(status_67X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(" while closing port ", (stderr));
  if ((3 == (3 & id_69X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + id_69X))) + -4))))>>2))))) {
      ps_write_string((((char *)(((char *) (-3 + id_69X))))), (stderr));
      goto L11462;}
    else {
      goto L11456;}}
  else {
    goto L11456;}}
 L11462: {
  PS_WRITE_CHAR(10, (stderr), v_70X)
  return v_70X;}
 L11456: {
  ps_write_integer((((index_68X)>>2)), (stderr));
  goto L11462;}
}
static void HtopD8861(void)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long env_113X;
  long env_112X;
  long last_env_111X;
  long cont_110X;
  long env_109X;
  long v_108X;
  long env_107X;
  long p_106X;
  long arg_count_105X;
  long i_104X;
  char * p_103X;
  char * addr_102X;
  long val_101X;
  long unused_100X;
  char * a_99X;
  char x_98X;
  long pair_97X;
  long v_96X;
  long v_95X;
  long env_94X;
  long alist_93X;
  long x2_92X;
  char * cell_91X;
  long i_90X;
  long x2_89X;
  char * cell_88X;
  long v_87X;
  long v_86X;
  long v_85X;
  long v_84X;
  long v_83X;
  long v_82X;
  long v_81X;
  long v_80X;
  long v_79X;
  long v_78X;
  long v_77X;
  long v_76X;
  long v_75X;
  long v_74X;
  char * frame_73X;
  long length_72X;
  char * frame_71X;
 {  arg1K0 = (Sexternal_root_stackS);
  goto L6255;}
 L6255: {
  frame_71X = arg1K0;
  if ((frame_71X == NULL)) {
    arg1K0 = (Spermanent_external_rootsS);
    goto L6281;}
  else {
    length_72X = *((long *) frame_71X);
    arg0K0 = 0;
    goto L6263;}}
 L6281: {
  frame_73X = arg1K0;
  if ((frame_73X == NULL)) {
    s48_initializing_gc_root();
    v_74X = s48_trace_value((Simported_bindingsS));
    Simported_bindingsS = v_74X;
    v_75X = s48_trace_value((Sexported_bindingsS));
    Sexported_bindingsS = v_75X;
    Ssaved_pcS = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
    v_76X = s48_trace_value((StemplateS));
    StemplateS = v_76X;
    v_77X = s48_trace_value((SvalS));
    SvalS = v_77X;
    v_78X = s48_trace_value((Scurrent_threadS));
    Scurrent_threadS = v_78X;
    v_79X = s48_trace_value((Ssession_dataS));
    Ssession_dataS = v_79X;
    v_80X = s48_trace_value((Sexception_handlersS));
    Sexception_handlersS = v_80X;
    v_81X = s48_trace_value((Sexception_templateS));
    Sexception_templateS = v_81X;
    v_82X = s48_trace_value((Sinterrupt_handlersS));
    Sinterrupt_handlersS = v_82X;
    v_83X = s48_trace_value((Sinterrupt_templateS));
    Sinterrupt_templateS = v_83X;
    v_84X = s48_trace_value((Sinterrupted_templateS));
    Sinterrupted_templateS = v_84X;
    v_85X = s48_trace_value((Sfinalize_theseS));
    Sfinalize_theseS = v_85X;
    v_86X = s48_trace_value((Sos_signal_typeS));
    Sos_signal_typeS = v_86X;
    v_87X = s48_trace_value((Sos_signal_argumentS));
    Sos_signal_argumentS = v_87X;
    arg0K0 = (Sfinalizer_alistS);
    goto L5289;}
  else {
    cell_88X = ((char *) (*((long *) (frame_73X + 4))));
    x2_89X = s48_trace_value((*((long *) cell_88X)));
    *((long *) cell_88X) = x2_89X;
    arg1K0 = (((char *) (*((long *) frame_73X))));
    goto L6281;}}
 L6263: {
  i_90X = arg0K0;
  if ((i_90X == length_72X)) {
    arg1K0 = (((char *) (*((long *) (frame_71X + 4)))));
    goto L6255;}
  else {
    cell_91X = ((char *) (*((long *) (frame_71X + (8 + (((i_90X)<<2)))))));
    x2_92X = s48_trace_value((*((long *) cell_91X)));
    *((long *) cell_91X) = x2_92X;
    arg0K0 = (1 + i_90X);
    goto L6263;}}
 L5289: {
  alist_93X = arg0K0;
  if ((25 == alist_93X)) {
    env_94X = s48_trace_value((SenvS));
    SenvS = env_94X;
    v_95X = s48_trace_value((Spending_channels_headS));
    Spending_channels_headS = v_95X;
    v_96X = s48_trace_value((Spending_channels_tailS));
    Spending_channels_tailS = v_96X;
    if ((Sstack_warningPS)) {
      arg1K0 = (Sstack_beginS);
      goto L5883;}
    else {
      goto L5916;}}
  else {
    pair_97X = *((long *) (((char *) (-3 + alist_93X))));
    x_98X = s48_extantP((*((long *) (((char *) (-3 + pair_97X))))));
    if (x_98X) {
      goto L5314;}
    else {
      s48_trace_stob_contentsB((*((long *) (((char *) (-3 + pair_97X))))));
      goto L5314;}}}
 L5883: {
  a_99X = arg1K0;
  if ((252645135 == (*((long *) a_99X)))) {
    arg1K0 = (a_99X + 4);
    goto L5883;}
  else {
    unused_100X = (((a_99X - (Sstack_beginS)))>>2);
    if ((unused_100X < 30)) {
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      ps_write_string("[Alert: stack overconsumption (", (stderr));
      ps_write_integer(unused_100X, (stderr));
      ps_write_string("); please inform the Scheme 48 implementors]", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sstack_warningPS = 0;
      goto L5916;}
    else {
      goto L5916;}}}
 L5916: {
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L5981;}
 L5314: {
  val_101X = s48_trace_value((*((long *) ((((char *) (-3 + pair_97X))) + 4))));
  addr_102X = (((char *) (-3 + pair_97X))) + 4;
  S48_WRITE_BARRIER(pair_97X, addr_102X, val_101X);
  *((long *) addr_102X) = val_101X;
  arg0K0 = (*((long *) ((((char *) (-3 + alist_93X))) + 4)));
  goto L5289;}
 L5981: {
  p_103X = arg1K0;
  i_104X = arg0K1;
  if ((2 == (3 & (*((long *) p_103X))))) {
    if ((22 == (*((long *) p_103X)))) {
      arg0K0 = (-1 + i_104X);
      goto L5918;}
    else {
      arg0K0 = i_104X;
      goto L5918;}}
  else {
    arg1K0 = (p_103X + 4);
    arg0K1 = (1 + i_104X);
    goto L5981;}}
 L5918: {
  arg_count_105X = arg0K0;
  s48_trace_locationsB(((SstackS) + 4), ((SstackS) + (4 + (((arg_count_105X)<<2)))));
  p_106X = SenvS;
  if ((3 == (3 & p_106X))) {
    if ((p_106X < (((long) (Sstack_beginS))))) {
      goto L5934;}
    else {
      if (((((long) (Sstack_endS))) < p_106X)) {
        goto L5934;}
      else {
        env_107X = SenvS;
        arg0K0 = env_107X;
        goto L6094;}}}
  else {
    goto L5934;}}
 L5934: {
  v_108X = s48_trace_value((SenvS));
  SenvS = v_108X;
  goto L5939;}
 L6094: {
  env_109X = arg0K0;
  if ((3 == (3 & env_109X))) {
    if ((env_109X < (((long) (Sstack_beginS))))) {
      goto L5939;}
    else {
      if (((((long) (Sstack_endS))) < env_109X)) {
        goto L5939;}
      else {
        s48_trace_stob_contentsB(env_109X);
        arg0K0 = (*((long *) (((char *) (-3 + env_109X)))));
        goto L6094;}}}
  else {
    goto L5939;}}
 L5939: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L5944;}
 L5944: {
  cont_110X = arg0K0;
  last_env_111X = arg0K1;
  env_112X = *((long *) ((((char *) (-3 + cont_110X))) + 12));
  s48_trace_stob_contentsB(cont_110X);
  if ((env_112X == last_env_111X)) {
    goto L5962;}
  else {
    arg0K0 = env_112X;
    goto L6049;}}
 L5962: {
  if ((cont_110X == (Sbottom_of_stackS))) {
    return;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + cont_110X)))));
    arg0K1 = env_112X;
    goto L5944;}}
 L6049: {
  env_113X = arg0K0;
  if ((3 == (3 & env_113X))) {
    if ((env_113X < (((long) (Sstack_beginS))))) {
      goto L5962;}
    else {
      if (((((long) (Sstack_endS))) < env_113X)) {
        goto L5962;}
      else {
        s48_trace_stob_contentsB(env_113X);
        arg0K0 = (*((long *) (((char *) (-3 + env_113X)))));
        goto L6049;}}}
  else {
    goto L5962;}}
}
static long Hlookup2102(long table_114X, long string_115X, long key_116X)
{
  long arg0K1;
  long arg0K0;
  long len_127X;
  long s2_126X;
  char * addr_125X;
  long x_124X;
  char * addr_123X;
  long foo_122X;
  long bucket_121X;
  long index_120X;
  long h_119X;
  long i_118X;
  long n_117X;
 {  n_117X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_115X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L13083;}
 L13083: {
  i_118X = arg0K0;
  h_119X = arg0K1;
  if ((i_118X < n_117X)) {
    arg0K0 = (1 + i_118X);
    arg0K1 = (h_119X + (((*((unsigned char *) ((((char *) (-3 + string_115X))) + i_118X))))));
    goto L13083;}
  else {
    index_120X = 1023 & h_119X;
    bucket_121X = *((long *) ((((char *) (-3 + table_114X))) + (((index_120X)<<2))));
    arg0K0 = bucket_121X;
    goto L13051;}}
 L13051: {
  foo_122X = arg0K0;
  if ((1 == foo_122X)) {
    addr_123X = ALLOCATE_SPACE(13, 20);
    *((long *) addr_123X) = 4150;
    x_124X = 3 + (((long) (addr_123X + 4)));
    *((long *) (((char *) (-3 + x_124X)))) = string_115X;
    *((long *) ((((char *) (-3 + x_124X))) + 4)) = 1;
    *((long *) ((((char *) (-3 + x_124X))) + 8)) = 13;
    *((long *) ((((char *) (-3 + x_124X))) + 12)) = bucket_121X;
    addr_125X = (((char *) (-3 + table_114X))) + (((index_120X)<<2));
    S48_WRITE_BARRIER(table_114X, addr_125X, x_124X);
    *((long *) addr_125X) = x_124X;
    return x_124X;}
  else {
    s2_126X = *((long *) (((char *) (-3 + foo_122X))));
    len_127X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_115X))) + -4))))>>8);
    if ((len_127X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_126X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_126X))), (void *)(((char *) (-3 + string_115X))),len_127X)))) {
        return foo_122X;}
      else {
        goto L13067;}}
    else {
      goto L13067;}}}
 L13067: {
  arg0K0 = (*((long *) ((((char *) (-3 + foo_122X))) + 12)));
  goto L13051;}
}
static long Hlookup2083(long table_128X, long string_129X, long key_130X)
{
  long arg0K1;
  long arg0K0;
  long len_141X;
  long s2_140X;
  char * addr_139X;
  long x_138X;
  char * addr_137X;
  long foo_136X;
  long bucket_135X;
  long index_134X;
  long h_133X;
  long i_132X;
  long n_131X;
 {  n_131X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_129X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L13208;}
 L13208: {
  i_132X = arg0K0;
  h_133X = arg0K1;
  if ((i_132X < n_131X)) {
    arg0K0 = (1 + i_132X);
    arg0K1 = (h_133X + (((*((unsigned char *) ((((char *) (-3 + string_129X))) + i_132X))))));
    goto L13208;}
  else {
    index_134X = 1023 & h_133X;
    bucket_135X = *((long *) ((((char *) (-3 + table_128X))) + (((index_134X)<<2))));
    arg0K0 = bucket_135X;
    goto L13176;}}
 L13176: {
  foo_136X = arg0K0;
  if ((1 == foo_136X)) {
    addr_137X = ALLOCATE_SPACE(13, 20);
    *((long *) addr_137X) = 4150;
    x_138X = 3 + (((long) (addr_137X + 4)));
    *((long *) (((char *) (-3 + x_138X)))) = string_129X;
    *((long *) ((((char *) (-3 + x_138X))) + 4)) = 5;
    *((long *) ((((char *) (-3 + x_138X))) + 8)) = 13;
    *((long *) ((((char *) (-3 + x_138X))) + 12)) = bucket_135X;
    addr_139X = (((char *) (-3 + table_128X))) + (((index_134X)<<2));
    S48_WRITE_BARRIER(table_128X, addr_139X, x_138X);
    *((long *) addr_139X) = x_138X;
    return x_138X;}
  else {
    s2_140X = *((long *) (((char *) (-3 + foo_136X))));
    len_141X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_129X))) + -4))))>>8);
    if ((len_141X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_140X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_140X))), (void *)(((char *) (-3 + string_129X))),len_141X)))) {
        return foo_136X;}
      else {
        goto L13192;}}
    else {
      goto L13192;}}}
 L13192: {
  arg0K0 = (*((long *) ((((char *) (-3 + foo_136X))) + 12)));
  goto L13176;}
}
static void HtopD8854(void)
{
  long arg0K2;
  long arg0K1;
  long arg0K0;
  long status_168X;
  long id_167X;
  long new_166X;
  long id_165X;
  long header_164X;
  long channel_163X;
  long i_162X;
  char * addr_161X;
  long l_160X;
  long v_159X;
  char * addr_158X;
  char * addr_157X;
  char * addr_156X;
  char * addr_155X;
  long val_154X;
  char tracedP_153X;
  long next_152X;
  long thing_151X;
  long pair_150X;
  long alist_149X;
  long l2_148X;
  long goners_147X;
  long okay_146X;
  long alist_145X;
  long pc_144X;
  long tem_143X;
  long new_142X;
 {  new_142X = s48_trace_value((Sthe_symbol_tableS));
  HcleanerB2160(new_142X);
  Sthe_symbol_tableS = new_142X;
  tem_143X = StemplateS;
  pc_144X = Ssaved_pcS;
  StemplateS = tem_143X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_143X)))))))) + (((pc_144X)>>2)));
  arg0K0 = (Sfinalizer_alistS);
  arg0K1 = 25;
  arg0K2 = 25;
  goto L7722;}
 L7722: {
  alist_145X = arg0K0;
  okay_146X = arg0K1;
  goners_147X = arg0K2;
  if ((25 == alist_145X)) {
    Sfinalizer_alistS = okay_146X;
    l2_148X = Sfinalize_theseS;
    if ((25 == goners_147X)) {
      arg0K0 = l2_148X;
      goto L7730;}
    else {
      arg0K0 = goners_147X;
      goto L7783;}}
  else {
    alist_149X = s48_trace_value(alist_145X);
    pair_150X = s48_trace_value((*((long *) (((char *) (-3 + alist_149X))))));
    thing_151X = *((long *) (((char *) (-3 + pair_150X))));
    next_152X = *((long *) ((((char *) (-3 + alist_149X))) + 4));
    tracedP_153X = s48_extantP(thing_151X);
    val_154X = s48_trace_value(thing_151X);
    addr_155X = ((char *) (-3 + pair_150X));
    S48_WRITE_BARRIER(pair_150X, addr_155X, val_154X);
    *((long *) addr_155X) = val_154X;
    addr_156X = ((char *) (-3 + alist_149X));
    S48_WRITE_BARRIER(alist_149X, addr_156X, pair_150X);
    *((long *) addr_156X) = pair_150X;
    if (tracedP_153X) {
      addr_157X = (((char *) (-3 + alist_149X))) + 4;
      S48_WRITE_BARRIER(alist_149X, addr_157X, okay_146X);
      *((long *) addr_157X) = okay_146X;
      arg0K0 = next_152X;
      arg0K1 = alist_149X;
      arg0K2 = goners_147X;
      goto L7722;}
    else {
      addr_158X = (((char *) (-3 + alist_149X))) + 4;
      S48_WRITE_BARRIER(alist_149X, addr_158X, goners_147X);
      *((long *) addr_158X) = goners_147X;
      arg0K0 = next_152X;
      arg0K1 = okay_146X;
      arg0K2 = alist_149X;
      goto L7722;}}}
 L7730: {
  v_159X = arg0K0;
  Sfinalize_theseS = v_159X;
  arg0K0 = 0;
  goto L12515;}
 L7783: {
  l_160X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_160X))) + 4))))) {
    addr_161X = (((char *) (-3 + l_160X))) + 4;
    S48_WRITE_BARRIER(l_160X, addr_161X, l2_148X);
    *((long *) addr_161X) = l2_148X;
    arg0K0 = goners_147X;
    goto L7730;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_160X))) + 4)));
    goto L7783;}}
 L12515: {
  i_162X = arg0K0;
  if ((i_162X == (Snumber_of_channelsS))) {
    Spending_interruptsS = (4 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        return;}
      else {
        return;}}
    else {
      s48_Spending_interruptPS = 1;
      return;}}
  else {
    channel_163X = *((Svm_channelsS) + i_162X);
    if ((1 == channel_163X)) {
      goto L12551;}
    else {
      header_164X = *((long *) ((((char *) (-3 + channel_163X))) + -4));
      if ((3 == (3 & header_164X))) {
        arg0K0 = header_164X;
        goto L12544;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_163X))))))) {
          arg0K0 = 1;
          goto L12544;}
        else {
          id_165X = *((long *) ((((char *) (-3 + channel_163X))) + 4));
          if ((0 == (3 & id_165X))) {
            arg0K0 = id_165X;
            goto L12227;}
          else {
            if ((3 == (3 & (*((long *) ((((char *) (-3 + id_165X))) + -4)))))) {
              arg0K0 = (*((long *) ((((char *) (-3 + id_165X))) + -4)));
              goto L12227;}
            else {
              arg0K0 = id_165X;
              goto L12227;}}}}}}}
 L12551: {
  arg0K0 = (1 + i_162X);
  goto L12515;}
 L12544: {
  new_166X = arg0K0;
  *((Svm_channelsS) + i_162X) = new_166X;
  goto L12551;}
 L12227: {
  id_167X = arg0K0;
  status_168X = close_channelB(channel_163X);
  if ((status_168X == NO_ERRORS)) {
    goto L12242;}
  else {
    channel_close_error(status_168X, (*((long *) ((((char *) (-3 + channel_163X))) + 8))), id_167X);
    goto L12242;}}
 L12242: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_167X))) {
    ps_write_integer((((id_167X)>>2)), (stderr));
    goto L12260;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_167X))))), (stderr));
    goto L12260;}}
 L12260: {
  ps_write_string(" ", (stderr));
  ps_write_integer(((((*((long *) ((((char *) (-3 + channel_163X))) + 8))))>>2)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L12544;}
}
static long collect_saving_temps(long value0_169X, long value1_170X, long *TT0)
{
  long value0_172X;
  long value1_171X;
 {  s48_begin_collection();
  (*(Sgc_root_procS))();
  value1_171X = s48_trace_value(value1_170X);
  value0_172X = s48_trace_value(value0_169X);
  s48_do_gc();
  (*(Spost_gc_cleanupS))();
  s48_end_collection();
  *TT0 = value1_171X;
  return value0_172X;}
}
void s48_set_extension_valueB(long value_173X)
{

 {  s48_Sextension_valueS = value_173X;
  return;}
}
void s48_note_event(void)
{

 {  s48_Spending_eventsPS = 1;
  s48_Spending_interruptPS = 1;
  return;}
}
void s48_set_os_signal(long type_174X, long argument_175X)
{

 {  Sos_signal_typeS = type_174X;
  Sos_signal_argumentS = argument_175X;
  return;}
}
long s48_exported_bindings(void)
{

 {  return (Sexported_bindingsS);}
}
char * s48_set_gc_roots_baseB(void)
{
  char * old_base_176X;
 {  old_base_176X = Sexternal_root_stack_baseS;
  Sexternal_root_stack_baseS = (Sexternal_root_stackS);
  return old_base_176X;}
}
char s48_release_gc_roots_baseB(char * old_base_177X)
{
  char okayP_178X;
 {  okayP_178X = (Sexternal_root_stackS) == (Sexternal_root_stack_baseS);
  Sexternal_root_stack_baseS = old_base_177X;
  return okayP_178X;}
}
void s48_disable_interruptsB(void)
{

 {  s48_Spending_interruptPS = 0;
  Senabled_interruptsS = 0;
  return;}
}
void s48_push_gc_rootsB(char * frame_179X, long n_180X)
{

 {  *((long *) frame_179X) = n_180X;
  *((long *) (frame_179X + 4)) = (((long) (Sexternal_root_stackS)));
  Sexternal_root_stackS = frame_179X;
  return;}
}
void s48_register_gc_rootB(char * loc_addr_181X)
{
  char * frame_182X;
 {  frame_182X = (char *)malloc(8);
  if ((frame_182X == NULL)) {
    ps_error("out of memory registering a global root", 0);
    goto L3175;}
  else {
    goto L3175;}}
 L3175: {
  *((long *) frame_182X) = (((long) (Spermanent_external_rootsS)));
  *((long *) (frame_182X + 4)) = (((long) loc_addr_181X));
  Spermanent_external_rootsS = frame_182X;
  return;}
}
void s48_stack_setB(long x_183X, long value_184X)
{

 {  *((long *) ((SstackS) + (4 + (((x_183X)<<2))))) = value_184X;
  return;}
}
long s48_stack_ref(long i_185X)
{

 {  return (*((long *) ((SstackS) + (4 + (((i_185X)<<2))))));}
}
void s48_push(long x_186X)
{

 {  *((long *) (SstackS)) = x_186X;
  SstackS = ((SstackS) + -4);
  return;}
}
long s48_resetup_external_exception(long new_why_187X, long additional_nargs_188X)
{
  long old_why_190X;
  long old_nargs_189X;
 {  old_nargs_189X = Sexternal_exception_nargsS;
  old_why_190X = *((long *) ((SstackS) + (4 + (((old_nargs_189X)<<2)))));
  *((long *) ((SstackS) + (4 + (((old_nargs_189X)<<2))))) = (((new_why_187X)<<2));
  Sexternal_exception_nargsS = (old_nargs_189X + additional_nargs_188X);
  return old_why_190X;}
}
void s48_enable_interruptsB(void)
{

 {  Senabled_interruptsS = -1;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Spending_interruptPS = 0;
    if ((s48_Spending_eventsPS)) {
      s48_Spending_interruptPS = 1;
      return;}
    else {
      return;}}
  else {
    s48_Spending_interruptPS = 1;
    return;}}
}
char s48_pop_gc_rootsB(void)
{

 {  if (((Sexternal_root_stackS) == (Sexternal_root_stack_baseS))) {
    return 0;}
  else {
    Sexternal_root_stackS = (((char *) (*((long *) ((Sexternal_root_stackS) + 4)))));
    return 1;}}
}
void s48_mark_traced_channels_closedB(void)
{
  long arg0K0;
  char * addr_196X;
  char * addr_195X;
  long descriptor_194X;
  long header_193X;
  long channel_192X;
  long i_191X;
 {  arg0K0 = 0;
  goto L6332;}
 L6332: {
  i_191X = arg0K0;
  if ((i_191X == (Snumber_of_channelsS))) {
    return;}
  else {
    channel_192X = *((Svm_channelsS) + i_191X);
    if ((1 == channel_192X)) {
      goto L6392;}
    else {
      header_193X = *((long *) ((((char *) (-3 + channel_192X))) + -4));
      if ((3 == (3 & header_193X))) {
        ps_write_string("Channel closed in dumped image: ", (stderr));
        descriptor_194X = *((long *) ((((char *) (-3 + channel_192X))) + 4));
        if ((0 == (3 & descriptor_194X))) {
          ps_write_integer(((((*((long *) ((((char *) (-3 + channel_192X))) + 4))))>>2)), (stderr));
          goto L6378;}
        else {
          ps_write_string((((char *)(((char *) (-3 + (*((long *) ((((char *) (-3 + channel_192X))) + 4)))))))), (stderr));
          goto L6378;}}
      else {
        goto L6392;}}}}
 L6392: {
  arg0K0 = (1 + i_191X);
  goto L6332;}
 L6378: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  addr_195X = ((char *) (-3 + header_193X));
  S48_WRITE_BARRIER(header_193X, addr_195X, 0);
  *((long *) addr_195X) = 0;
  addr_196X = (((char *) (-3 + header_193X))) + 8;
  S48_WRITE_BARRIER(header_193X, addr_196X, -4);
  *((long *) addr_196X) = -4;
  goto L6392;}
}
long s48_set_channel_os_index(long channel_197X, long os_index_198X)
{
  char * addr_205X;
  long val_204X;
  long v_203X;
  long x_202X;
  long old_index_201X;
  char x_200X;
  char temp_199X;
 {  temp_199X = os_index_198X < (Snumber_of_channelsS);
  if (temp_199X) {
    goto L7901;}
  else {
    x_200X = add_more_channels(os_index_198X);
    if (x_200X) {
      goto L7901;}
    else {
      return 36;}}}
 L7901: {
  if ((1 == (*((Svm_channelsS) + os_index_198X)))) {
    old_index_201X = (((*((long *) ((((char *) (-3 + channel_197X))) + 8))))>>2);
    x_202X = *((long *) ((((char *) (-3 + channel_197X))) + 16));
    if ((5 == x_202X)) {
      v_203X = ps_abort_fd_op(old_index_201X);
      enqueue_channelB(old_index_201X, v_203X);
      goto L7887;}
    else {
      goto L7887;}}
  else {
    return 44;}}
 L7887: {
  *((Svm_channelsS) + old_index_201X) = 1;
  *((Svm_channelsS) + os_index_198X) = channel_197X;
  val_204X = ((os_index_198X)<<2);
  addr_205X = (((char *) (-3 + channel_197X))) + 8;
  S48_WRITE_BARRIER(channel_197X, addr_205X, val_204X);
  *((long *) addr_205X) = val_204X;
  return 5;}
}
long s48_cleaned_imported_bindings(void)
{
  long arg0K1;
  long arg0K0;
  long v_219X;
  char * addr_218X;
  long entry_217X;
  char * addr_216X;
  long new_foo_215X;
  char v_214X;
  char * addr_213X;
  long okay_212X;
  long foo_211X;
  long i_210X;
  long foo_209X;
  long i_208X;
  long table_207X;
  long table_206X;
 {  table_206X = s48_trace_value((Simported_bindingsS));
  table_207X = s48_trace_value(table_206X);
  arg0K0 = 0;
  goto L8578;}
 L8578: {
  i_208X = arg0K0;
  if ((1024 == i_208X)) {
    arg0K0 = 0;
    goto L9667;}
  else {
    foo_209X = *((long *) ((((char *) (-3 + table_207X))) + (((i_208X)<<2))));
    arg0K0 = foo_209X;
    arg0K1 = 1;
    goto L8598;}}
 L9667: {
  i_210X = arg0K0;
  if ((1024 == i_210X)) {
    return table_206X;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + table_206X))) + (((i_210X)<<2)))));
    goto L9675;}}
 L8598: {
  foo_211X = arg0K0;
  okay_212X = arg0K1;
  if ((1 == foo_211X)) {
    addr_213X = (((char *) (-3 + table_207X))) + (((i_208X)<<2));
    S48_WRITE_BARRIER(table_207X, addr_213X, okay_212X);
    *((long *) addr_213X) = okay_212X;
    arg0K0 = (1 + i_208X);
    goto L8578;}
  else {
    v_214X = s48_extantP(foo_211X);
    if (v_214X) {
      new_foo_215X = s48_trace_value(foo_211X);
      addr_216X = (((char *) (-3 + new_foo_215X))) + 12;
      S48_WRITE_BARRIER(new_foo_215X, addr_216X, okay_212X);
      *((long *) addr_216X) = okay_212X;
      arg0K0 = new_foo_215X;
      goto L8603;}
    else {
      arg0K0 = okay_212X;
      goto L8603;}}}
 L9675: {
  entry_217X = arg0K0;
  if ((1 == entry_217X)) {
    arg0K0 = (1 + i_210X);
    goto L9667;}
  else {
    addr_218X = (((char *) (-3 + entry_217X))) + 8;
    S48_WRITE_BARRIER(entry_217X, addr_218X, 529);
    *((long *) addr_218X) = 529;
    arg0K0 = (*((long *) ((((char *) (-3 + entry_217X))) + 12)));
    goto L9675;}}
 L8603: {
  v_219X = arg0K0;
  arg0K0 = (*((long *) ((((char *) (-3 + foo_211X))) + 12)));
  arg0K1 = v_219X;
  goto L8598;}
}
long s48_copy_symbol_table(void)
{
  long new_220X;
 {  new_220X = s48_trace_value((Sthe_symbol_tableS));
  HcleanerB2160(new_220X);
  return new_220X;}
}
void s48_setup_external_exception(long why_221X, long nargs_222X)
{

 {  push_exception_continuationB(why_221X, 1);
  if ((10 < nargs_222X)) {
    ps_error("too many arguments from external exception", 0);
    goto L11018;}
  else {
    goto L11018;}}
 L11018: {
  Sexternal_exception_nargsS = nargs_222X;
  Sexternal_exceptionPS = 1;
  return;}
}
void s48_close_channel(long os_index_223X)
{
  long obj_224X;
 {  if ((os_index_223X < 0)) {
    return;}
  else {
    if ((os_index_223X < (Snumber_of_channelsS))) {
      obj_224X = *((Svm_channelsS) + os_index_223X);
      if ((3 == (3 & obj_224X))) {
        if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_224X))) + -4))))>>2))))) {
          close_channelB((*((Svm_channelsS) + os_index_223X)));
          return;}
        else {
          return;}}
      else {
        return;}}
    else {
      return;}}}
}
char s48_warn_about_undefined_imported_bindings(void)
{
  long arg0K0;
  long name_230X;
  long entry_229X;
  char temp_228X;
  long i_227X;
  long table_226X;
  FILE * out_225X;
 {  out_225X = stderr;
  table_226X = Simported_bindingsS;
  arg0K0 = 0;
  goto L11558;}
 L11558: {
  i_227X = arg0K0;
  temp_228X = 1024 == i_227X;
  if (temp_228X) {
    return temp_228X;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + table_226X))) + (((i_227X)<<2)))));
    goto L11566;}}
 L11566: {
  entry_229X = arg0K0;
  if ((1 == entry_229X)) {
    arg0K0 = (1 + i_227X);
    goto L11558;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + entry_229X))) + 8)))))) {
      name_230X = *((long *) (((char *) (-3 + entry_229X))));
      ps_write_string("undefined imported name ", out_225X);
      if ((3 == (3 & name_230X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + name_230X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + name_230X))))), out_225X);
          goto L11546;}
        else {
          goto L11544;}}
      else {
        goto L11544;}}
    else {
      goto L11569;}}}
 L11546: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_225X, ignoreXX) }
  goto L11569;}
 L11544: {
  ps_write_string("<invalid name>", out_225X);
  goto L11546;}
 L11569: {
  arg0K0 = (*((long *) ((((char *) (-3 + entry_229X))) + 12)));
  goto L11566;}
}
void s48_define_exported_binding(char *name_231X, long value_232X)
{
  char arg2K0;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char * addr_245X;
  long x_244X;
  long name_243X;
  long value_242X;
  long key_241X;
  long temp_240X;
  long key_239X;
  char okayP_238X;
  char v_237X;
  long temp1_236X;
  long temp0_235X;
  char v_234X;
  long space_233X;
 {  space_233X = 6 + ((((4 + (strlen((char *) name_231X))))>>2));
  v_234X = AVAILABLEp(space_233X);
  if (v_234X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = value_232X;
    goto L14338;}
  else {
    temp0_235X = collect_saving_temps(value_232X, 1, &temp1_236X);
    v_237X = AVAILABLEp(space_233X);
    if (v_237X) {
      arg2K0 = 1;
      arg0K1 = 0;
      arg0K2 = temp0_235X;
      goto L14338;}
    else {
      arg2K0 = 0;
      arg0K1 = 0;
      arg0K2 = temp0_235X;
      goto L14338;}}}
 L14338: {
  okayP_238X = arg2K0;
  key_239X = arg0K1;
  temp_240X = arg0K2;
  if (okayP_238X) {
    arg0K0 = key_239X;
    arg0K1 = temp_240X;
    goto L14319;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_239X;
    arg0K1 = temp_240X;
    goto L14319;}}
 L14319: {
  key_241X = arg0K0;
  value_242X = arg0K1;
  name_243X = enter_string(name_231X, key_241X);
  x_244X = Hlookup2083((Simported_bindingsS), name_243X, key_241X);
  addr_245X = (((char *) (-3 + x_244X))) + 8;
  S48_WRITE_BARRIER(x_244X, addr_245X, value_242X);
  *((long *) addr_245X) = value_242X;
  return;}
}
long s48_add_channel(long mode_246X, long id_247X, long os_index_248X)
{
  char arg2K0;
  long arg0K1;
  long arg0K0;
  long status_256X;
  long channel_255X;
  long v_254X;
  long key_253X;
  char okayP_252X;
  char v_251X;
  long temp1_250X;
  char v_249X;
 {  v_249X = AVAILABLEp(6);
  if (v_249X) {
    arg2K0 = 1;
    arg0K1 = 0;
    goto L14727;}
  else {
    collect_saving_temps(1, 1, &temp1_250X);
    v_251X = AVAILABLEp(6);
    if (v_251X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L14727;}
    else {
      arg2K0 = 0;
      arg0K1 = 0;
      goto L14727;}}}
 L14727: {
  okayP_252X = arg2K0;
  key_253X = arg0K1;
  if (okayP_252X) {
    arg0K0 = key_253X;
    goto L14710;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_253X;
    goto L14710;}}
 L14710: {
  v_254X = arg0K0;
  channel_255X = make_registered_channel((((mode_246X)>>2)), id_247X, os_index_248X, v_254X, &status_256X);
  if ((3 == (3 & channel_255X))) {
    if ((5 == (31 & ((((*((long *) ((((char *) (-3 + channel_255X))) + -4))))>>2))))) {
      return channel_255X;}
    else {
      goto L14721;}}
  else {
    goto L14721;}}
 L14721: {
  return (((status_256X)<<2));}
}
long s48_get_imported_binding(char *name_257X)
{
  char arg2K0;
  long arg0K1;
  long arg0K0;
  long name_265X;
  long key_264X;
  long key_263X;
  char okayP_262X;
  char v_261X;
  long temp1_260X;
  char v_259X;
  long space_258X;
 {  space_258X = 6 + ((((4 + (strlen((char *) name_257X))))>>2));
  v_259X = AVAILABLEp(space_258X);
  if (v_259X) {
    arg2K0 = 1;
    arg0K1 = 0;
    goto L15416;}
  else {
    collect_saving_temps(1, 1, &temp1_260X);
    v_261X = AVAILABLEp(space_258X);
    if (v_261X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L15416;}
    else {
      arg2K0 = 0;
      arg0K1 = 0;
      goto L15416;}}}
 L15416: {
  okayP_262X = arg2K0;
  key_263X = arg0K1;
  if (okayP_262X) {
    arg0K0 = key_263X;
    goto L15403;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_263X;
    goto L15403;}}
 L15403: {
  key_264X = arg0K0;
  name_265X = enter_string(name_257X, key_264X);
  return Hlookup2102((Sexported_bindingsS), name_265X, key_264X);}
}
long s48_allocate_stob(long type_266X, long size_267X)
{
  char * addr_271X;
  char v_270X;
  long temp1_269X;
  char v_268X;
 {  v_268X = AVAILABLEp(size_267X);
  if (v_268X) {
    goto L15711;}
  else {
    collect_saving_temps(1, 1, &temp1_269X);
    v_270X = AVAILABLEp(size_267X);
    if (v_270X) {
      goto L15711;}
    else {
      ps_error("Scheme48 heap overflow", 0);
      goto L15711;}}}
 L15711: {
  addr_271X = ALLOCATE_SPACE(type_266X, (4 + size_267X));
  *((long *) addr_271X) = (2 + (((((((size_267X)<<6)) + type_266X))<<2)));
  return (3 + (((long) (addr_271X + 4))));}
}
void s48_initialize_vm(char * stack_begin_272X, long stack_size_273X)
{
  char arg2K0;
  char * arg1K0;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char * addr_344X;
  long code_343X;
  long temp_342X;
  char * addr_341X;
  char * addr_340X;
  char * addr_339X;
  long code_338X;
  long temp_337X;
  char * addr_336X;
  char * addr_335X;
  char v_334X;
  long temp1_333X;
  char v_332X;
  long cont_331X;
  char * addr_330X;
  long code_329X;
  long temp_328X;
  char * addr_327X;
  char * addr_326X;
  char v_325X;
  long temp1_324X;
  char v_323X;
  char * a_322X;
  long size_321X;
  char * start_320X;
  char * stack_319X;
  long v_318X;
  char * addr_317X;
  long i_316X;
  char * addr_315X;
  char * addr_314X;
  long val_313X;
  long index_312X;
  long h_311X;
  long i_310X;
  long table_309X;
  char * addr_308X;
  long v_307X;
  char * addr_306X;
  long i_305X;
  long n_304X;
  long string_303X;
  long foo_302X;
  long table_301X;
  long i_300X;
  long table_299X;
  char * addr_298X;
  long exported_bindings_297X;
  long imported_bindings_296X;
  long n_295X;
  long symbols_294X;
  long temp1_293X;
  long temp0_292X;
  char okayP_291X;
  char * addr_290X;
  long maybe_289X;
  long v_288X;
  long maybe_287X;
  long i_286X;
  char v_285X;
  long temp1_284X;
  long temp0_283X;
  char v_282X;
  long imported_bindings_281X;
  long exported_bindings_280X;
  long table_279X;
  char * addr_278X;
  char v_277X;
  long temp1_276X;
  char v_275X;
  long symbol_table_274X;
 {  symbol_table_274X = s48_initial_symbols();
  if ((symbol_table_274X == 1)) {
    v_275X = AVAILABLEp(1025);
    if (v_275X) {
      goto L15442;}
    else {
      collect_saving_temps(1, 1, &temp1_276X);
      v_277X = AVAILABLEp(1025);
      if (v_277X) {
        goto L15442;}
      else {
        ps_error("Scheme48 heap overflow", 0);
        goto L15442;}}}
  else {
    Sthe_symbol_tableS = symbol_table_274X;
    goto L15834;}}
 L15442: {
  addr_278X = ALLOCATE_SPACE(2, 4100);
  *((long *) addr_278X) = 1048586;
  table_279X = 3 + (((long) (addr_278X + 4)));
  arg0K0 = 0;
  goto L15517;}
 L15834: {
  exported_bindings_280X = s48_initial_exported_bindings();
  imported_bindings_281X = s48_initial_imported_bindings();
  v_282X = AVAILABLEp(2050);
  if (v_282X) {
    arg2K0 = 1;
    arg0K1 = imported_bindings_281X;
    arg0K2 = exported_bindings_280X;
    goto L14420;}
  else {
    temp0_283X = collect_saving_temps(imported_bindings_281X, exported_bindings_280X, &temp1_284X);
    v_285X = AVAILABLEp(2050);
    if (v_285X) {
      arg2K0 = 1;
      arg0K1 = temp0_283X;
      arg0K2 = temp1_284X;
      goto L14420;}
    else {
      arg2K0 = 0;
      arg0K1 = temp0_283X;
      arg0K2 = temp1_284X;
      goto L14420;}}}
 L15517: {
  i_286X = arg0K0;
  if ((1024 == i_286X)) {
    Sthe_symbol_tableS = table_279X;
    maybe_287X = s48_find_all(1);
    if ((maybe_287X == 1)) {
      collect_saving_temps(0, 0, &v_288X);
      maybe_289X = s48_find_all(1);
      if ((maybe_289X == 1)) {
        ps_error("insufficient heap space to build symbol table", 0);
        arg0K0 = maybe_289X;
        goto L15471;}
      else {
        arg0K0 = maybe_289X;
        goto L15471;}}
    else {
      arg0K0 = maybe_287X;
      goto L15471;}}
  else {
    addr_290X = (((char *) (-3 + table_279X))) + (((i_286X)<<2));
    S48_WRITE_BARRIER(table_279X, addr_290X, 1);
    *((long *) addr_290X) = 1;
    arg0K0 = (1 + i_286X);
    goto L15517;}}
 L14420: {
  okayP_291X = arg2K0;
  temp0_292X = arg0K1;
  temp1_293X = arg0K2;
  if (okayP_291X) {
    arg0K0 = temp0_292X;
    arg0K1 = temp1_293X;
    goto L14381;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp0_292X;
    arg0K1 = temp1_293X;
    goto L14381;}}
 L15471: {
  symbols_294X = arg0K0;
  n_295X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_294X))) + -4))))>>8))))>>2);
  arg0K0 = 0;
  goto L15538;}
 L14381: {
  imported_bindings_296X = arg0K0;
  exported_bindings_297X = arg0K1;
  if ((1 == imported_bindings_296X)) {
    addr_298X = ALLOCATE_SPACE(2, 4100);
    *((long *) addr_298X) = 1048586;
    table_299X = 3 + (((long) (addr_298X + 4)));
    arg0K0 = 0;
    goto L14482;}
  else {
    arg0K0 = imported_bindings_296X;
    goto L14396;}}
 L15538: {
  i_300X = arg0K0;
  if ((i_300X == n_295X)) {
    goto L15834;}
  else {
    table_301X = Sthe_symbol_tableS;
    foo_302X = *((long *) ((((char *) (-3 + symbols_294X))) + (((i_300X)<<2))));
    string_303X = *((long *) (((char *) (-3 + foo_302X))));
    n_304X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_303X))) + -4))))>>8));
    arg0K0 = 0;
    arg0K1 = 0;
    goto L13005;}}
 L14482: {
  i_305X = arg0K0;
  if ((1024 == i_305X)) {
    arg0K0 = table_299X;
    goto L14396;}
  else {
    addr_306X = (((char *) (-3 + table_299X))) + (((i_305X)<<2));
    S48_WRITE_BARRIER(table_299X, addr_306X, 1);
    *((long *) addr_306X) = 1;
    arg0K0 = (1 + i_305X);
    goto L14482;}}
 L14396: {
  v_307X = arg0K0;
  Simported_bindingsS = v_307X;
  if ((1 == exported_bindings_297X)) {
    addr_308X = ALLOCATE_SPACE(2, 4100);
    *((long *) addr_308X) = 1048586;
    table_309X = 3 + (((long) (addr_308X + 4)));
    arg0K0 = 0;
    goto L14459;}
  else {
    arg0K0 = exported_bindings_297X;
    goto L14409;}}
 L13005: {
  i_310X = arg0K0;
  h_311X = arg0K1;
  if ((i_310X < n_304X)) {
    arg0K0 = (1 + i_310X);
    arg0K1 = (h_311X + (((*((unsigned char *) ((((char *) (-3 + string_303X))) + i_310X))))));
    goto L13005;}
  else {
    index_312X = 1023 & h_311X;
    val_313X = *((long *) ((((char *) (-3 + table_301X))) + (((index_312X)<<2))));
    addr_314X = (((char *) (-3 + foo_302X))) + 4;
    S48_WRITE_BARRIER(foo_302X, addr_314X, val_313X);
    *((long *) addr_314X) = val_313X;
    addr_315X = (((char *) (-3 + table_301X))) + (((index_312X)<<2));
    S48_WRITE_BARRIER(table_301X, addr_315X, foo_302X);
    *((long *) addr_315X) = foo_302X;
    arg0K0 = (1 + i_300X);
    goto L15538;}}
 L14459: {
  i_316X = arg0K0;
  if ((1024 == i_316X)) {
    arg0K0 = table_309X;
    goto L14409;}
  else {
    addr_317X = (((char *) (-3 + table_309X))) + (((i_316X)<<2));
    S48_WRITE_BARRIER(table_309X, addr_317X, 1);
    *((long *) addr_317X) = 1;
    arg0K0 = (1 + i_316X);
    goto L14459;}}
 L14409: {
  v_318X = arg0K0;
  Sexported_bindingsS = v_318X;
  if ((stack_size_273X < 8131)) {
    stack_319X = (char *)malloc(32524);
    if ((stack_319X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_319X;
      arg0K1 = 8131;
      goto L14928;}
    else {
      arg1K0 = stack_319X;
      arg0K1 = 8131;
      goto L14928;}}
  else {
    arg1K0 = stack_begin_272X;
    arg0K1 = stack_size_273X;
    goto L14928;}}
 L14928: {
  start_320X = arg1K0;
  size_321X = arg0K1;
  Sstack_beginS = start_320X;
  Sstack_endS = (start_320X + (((size_321X)<<2)));
  Sstack_limitS = ((Sstack_beginS) + 524);
  SstackS = ((Sstack_endS) + -4);
  ScontS = 1;
  SenvS = 17;
  arg1K0 = start_320X;
  goto L14960;}
 L14960: {
  a_322X = arg1K0;
  if ((a_322X == (Sstack_endS))) {
    v_323X = AVAILABLEp(5);
    if (v_323X) {
      goto L14977;}
    else {
      collect_saving_temps(1, 1, &temp1_324X);
      v_325X = AVAILABLEp(5);
      if (v_325X) {
        goto L14977;}
      else {
        ps_error("Scheme48 heap overflow", 0);
        goto L14977;}}}
  else {
    *((long *) a_322X) = 252645135;
    arg1K0 = (a_322X + 4);
    goto L14960;}}
 L14977: {
  addr_326X = ALLOCATE_SPACE(17, 6);
  *((long *) addr_326X) = 582;
  addr_327X = ALLOCATE_SPACE(11, 12);
  *((long *) addr_327X) = 2094;
  temp_328X = 3 + (((long) (addr_327X + 4)));
  code_329X = 3 + (((long) (addr_326X + 4)));
  addr_330X = ((char *) (-3 + temp_328X));
  S48_WRITE_BARRIER(temp_328X, addr_330X, code_329X);
  *((long *) addr_330X) = code_329X;
  *((unsigned char *) (((char *) (-3 + code_329X)))) = 23;
  *((unsigned char *) ((((char *) (-3 + code_329X))) + 1)) = 31;
  SstackS = ((SstackS) + -20);
  *((long *) ((SstackS) + 4)) = 4262;
  cont_331X = 3 + (((long) ((SstackS) + 8)));
  *((long *) ((((char *) (-3 + cont_331X))) + 4)) = 0;
  *((long *) ((((char *) (-3 + cont_331X))) + 8)) = temp_328X;
  *((long *) ((((char *) (-3 + cont_331X))) + 12)) = (SenvS);
  *((long *) (((char *) (-3 + cont_331X)))) = (ScontS);
  ScontS = cont_331X;
  Sbottom_of_stackS = (ScontS);
  v_332X = AVAILABLEp(5);
  if (v_332X) {
    goto L15156;}
  else {
    collect_saving_temps(1, 1, &temp1_333X);
    v_334X = AVAILABLEp(5);
    if (v_334X) {
      goto L15156;}
    else {
      ps_error("Scheme48 heap overflow", 0);
      goto L15156;}}}
 L15156: {
  addr_335X = ALLOCATE_SPACE(17, 6);
  *((long *) addr_335X) = 582;
  addr_336X = ALLOCATE_SPACE(11, 12);
  *((long *) addr_336X) = 2094;
  temp_337X = 3 + (((long) (addr_336X + 4)));
  code_338X = 3 + (((long) (addr_335X + 4)));
  addr_339X = ((char *) (-3 + temp_337X));
  S48_WRITE_BARRIER(temp_337X, addr_339X, code_338X);
  *((long *) addr_339X) = code_338X;
  *((unsigned char *) (((char *) (-3 + code_338X)))) = 34;
  *((unsigned char *) ((((char *) (-3 + code_338X))) + 1)) = 140;
  Sinterrupt_templateS = temp_337X;
  addr_340X = ALLOCATE_SPACE(17, 6);
  *((long *) addr_340X) = 582;
  addr_341X = ALLOCATE_SPACE(11, 12);
  *((long *) addr_341X) = 2094;
  temp_342X = 3 + (((long) (addr_341X + 4)));
  code_343X = 3 + (((long) (addr_340X + 4)));
  addr_344X = ((char *) (-3 + temp_342X));
  S48_WRITE_BARRIER(temp_342X, addr_344X, code_343X);
  *((long *) addr_344X) = code_343X;
  *((unsigned char *) (((char *) (-3 + code_343X)))) = 137;
  *((unsigned char *) ((((char *) (-3 + code_343X))) + 1)) = 123;
  Sexception_templateS = temp_342X;
  return;}
}
long s48_restart(long proc_345X, long nargs_346X)
{
  char * arg1K1;
  char * arg1K0;
  char *arg3K0;
  char arg2K3;
  char arg2K1;
  char arg2K2;
  char arg2K0;
  long arg0K3;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char *merged_arg3K0;
  long merged_arg0K3;
  long merged_arg0K2;
  long merged_arg0K1;
  long merged_arg0K0;

  int pop_continuationB_return_tag;
  int get_error_string_return_tag;
  long get_error_string0_return_value;
  int okay_argument_list_return_tag;
  char okay_argument_list0_return_value;
  long okay_argument_list1_return_value;
  int get_current_port_return_tag;
  long get_current_port0_return_value;
  int copy_continuation_from_heapB_return_tag;
  long copy_continuation_from_heapB0_return_value;
  int copy_listS_return_tag;
  long copy_listS0_return_value;
  int pop_args_GlistS_return_tag;
  long pop_args_GlistS0_return_value;
  int save_env_in_heap_return_tag;
  long save_env_in_heap0_return_value;
  int copy_env_return_tag;
  long copy_env0_return_value;
  int really_preserve_continuation_return_tag;
  long really_preserve_continuation0_return_value;
  int copy_stack_into_heap_return_tag;
  int push_list_return_tag;
  long push_list0_return_value;
  int rest_list_setup_return_tag;
  int check_events_return_tag;
  char check_events0_return_value;
  int loseD0_return_tag;
  long status_347X;
  long list_348X;
  long marker_349X;
  long cont_350X;
  long list_351X;
  long length_352X;
  long start_353X;
  long count_354X;
  long env_355X;
  long cont_356X;
  long key_357X;
  long reason_358X;
  long env_359X;
  long key_360X;
  long reason_361X;
  long key_362X;
  long list_363X;
  long count_364X;
  long wants_stack_args_365X;
  long stack_arg_count_366X;
  long list_args_367X;
  long list_arg_count_368X;
  char *message_369X;
  long pc_1284X;
  long tem_1283X;
  long cont_1282X;
  long i_1281X;
  long string_1280X;
  char * addr_1279X;
  long len_1278X;
  long len_1277X;
  long x_1276X;
  char *string_1275X;
  char move_slowP_1274X;
  long slow_1273X;
  long len_1272X;
  long fast_1271X;
  long v_1270X;
  char *v_1269X;
  long v_1268X;
  long v_1267X;
  long obj_1266X;
  long env_1265X;
  long thread_1264X;
  long v_1263X;
  long new_cont_1262X;
  char * top_1261X;
  char * addr_1260X;
  long x_1259X;
  char * addr_1258X;
  long a_1257X;
  long last_1256X;
  long l_1255X;
  long x_1254X;
  char * addr_1253X;
  long a_1252X;
  long list_1251X;
  long temp_1250X;
  char okayP_1249X;
  char v_1248X;
  long temp1_1247X;
  long temp0_1246X;
  char v_1245X;
  long space_1244X;
  long x_1243X;
  char * addr_1242X;
  long a_1241X;
  long count_1240X;
  long args_1239X;
  long start_1238X;
  long temp_1237X;
  char okayP_1236X;
  char v_1235X;
  long temp1_1234X;
  long temp0_1233X;
  char v_1232X;
  long space_1231X;
  long env_1230X;
  long cont_1229X;
  char * addr_1228X;
  long new_1227X;
  long p_1226X;
  long env_1225X;
  long top_1224X;
  char * addr_1223X;
  long new_1222X;
  char * data_addr_1221X;
  char * addr_1220X;
  long header_1219X;
  long new_1218X;
  char * data_addr_1217X;
  char * addr_1216X;
  long header_1215X;
  long p_1214X;
  long previous_1213X;
  long cont_1212X;
  long end_1211X;
  long v_1210X;
  long p_1209X;
  char * arg_1208X;
  char * loc_1207X;
  long stob_1206X;
  char * top_1205X;
  long arg_count_1204X;
  long i_1203X;
  char * p_1202X;
  long v_1201X;
  long l_1200X;
  long i_1199X;
  long key_1198X;
  char okayP_1197X;
  long list_1196X;
  char v_1195X;
  long temp1_1194X;
  char v_1193X;
  long space_1192X;
  long x_1191X;
  long v_1190X;
  long x_1189X;
  long v_1188X;
  long count_1187X;
  long x_1186X;
  long v_1185X;
  long status_1184X;
  long channel_1183X;
  long type_1182X;
  char v_1181X;
  long template_1180X;
  char not_firstP_1179X;
  long cont_1178X;
  char not_firstP_1177X;
  FILE * out_1176X;
  long current_template_1175X;
  long why_1174X;
  long x_1173X;
  long x_1172X;
  long x_1171X;
  char * addr_1170X;
  long status_1169X;
  char pendingP_1168X;
  char eofP_1167X;
  long got_1166X;
  long length_1165X;
  long v_1164X;
  long obj_1163X;
  long reason_1162X;
  long status_1161X;
  long v_1160X;
  long v_1159X;
  long key_1158X;
  char okayP_1157X;
  char v_1156X;
  char * addr_1155X;
  long value_1154X;
  long offset_1153X;
  long i_1152X;
  long count_1151X;
  long y_1150X;
  char * addr_1149X;
  long val_1148X;
  char * addr_1147X;
  long value_1146X;
  long len_1145X;
  long s2_1144X;
  long foo_1143X;
  long previous_foo_1142X;
  long val_1141X;
  long x_1140X;
  char * addr_1139X;
  long status_1138X;
  char pendingP_1137X;
  long got_1136X;
  long length_1135X;
  long count_1134X;
  long start_1133X;
  char waitP_1132X;
  long x_1131X;
  long status_1130X;
  long channel_1129X;
  long v_1128X;
  long v_1127X;
  long v_1126X;
  long reason_1125X;
  long channel_1124X;
  long index_1123X;
  long val_1122X;
  long final_stack_arg_count_1121X;
  char v_1120X;
  long temp1_1119X;
  char v_1118X;
  long space_1117X;
  long stack_arg_count_1116X;
  long stack_slots_1115X;
  char x_1114X;
  long channel_1113X;
  long obj_1112X;
  long arg_count_1111X;
  long count_1110X;
  long i_1109X;
  long env_1108X;
  long value_1107X;
  char *v_1106X;
  long y_1105X;
  long i_1104X;
  long l_1103X;
  long bucket_1102X;
  long index_1101X;
  long h_1100X;
  long i_1099X;
  long v_1098X;
  long v_1097X;
  long x_1096X;
  long status_1095X;
  long reason_1094X;
  long v_1093X;
  long reason_1092X;
  long x_1091X;
  long obj_1090X;
  long v_1089X;
  long v_1088X;
  long len_1087X;
  long s2_1086X;
  char * addr_1085X;
  long x_1084X;
  char * addr_1083X;
  long foo_1082X;
  long rest_list_1081X;
  long i_1080X;
  long n_1079X;
  long args_1078X;
  long skip_1077X;
  long template_1076X;
  long stack_arg_count_1075X;
  long skip_1074X;
  long skip_1073X;
  long template_1072X;
  long stack_arg_count_1071X;
  long skip_1070X;
  char * addr_1069X;
  long next_1068X;
  long channel_1067X;
  long m_1066X;
  long i_1065X;
  long value_1064X;
  long back_1063X;
  long env_1062X;
  long offset_1061X;
  long i_1060X;
  long x_1059X;
  char * addr_1058X;
  long a_1057X;
  long env_1056X;
  long v_1055X;
  long p_1054X;
  long key_1053X;
  long string_1052X;
  char * addr_1051X;
  long len_1050X;
  long n_1049X;
  long table_1048X;
  long x_1047X;
  long arg2_1046X;
  long key_1045X;
  char temp_1044X;
  char minutesP_1043X;
  long status_1042X;
  long status_1041X;
  long undumpable_count_1040X;
  long undumpables_1039X;
  long status_1038X;
  long status_1037X;
  long status_1036X;
  long status_1035X;
  FILE * port_1034X;
  char x_1033X;
  long comment_string_1032X;
  long obj_1031X;
  long arg3_1030X;
  long arg2_1029X;
  long key_1028X;
  long x_1027X;
  char * addr_1026X;
  long channel_1025X;
  long res_1024X;
  long i_1023X;
  char * addr_1022X;
  char * addr_1021X;
  long val_1020X;
  char v_1019X;
  long channel_1018X;
  long count_1017X;
  long start_1016X;
  long obj_1015X;
  long arg4_1014X;
  long arg3_1013X;
  long arg2_1012X;
  long key_1011X;
  long arg5_1010X;
  long arg4_1009X;
  long arg3_1008X;
  long arg2_1007X;
  long key_1006X;
  long x_1005X;
  long status_1004X;
  long channel_1003X;
  long obj_1002X;
  long key_1001X;
  long mode_1000X;
  long arg2_999X;
  long key_998X;
  long bucket_997X;
  long index_996X;
  long h_995X;
  long i_994X;
  long i_993X;
  long i_992X;
  char * addr_991X;
  long i_990X;
  long i_989X;
  long i_988X;
  long c_987X;
  long b_986X;
  long c_985X;
  long b_984X;
  char x_983X;
  long c_982X;
  long b_981X;
  long c_980X;
  long mid_c_979X;
  long v_978X;
  long v_977X;
  long lo_c_976X;
  long hi_b_975X;
  long hi_a_974X;
  long lo_b_973X;
  long lo_a_972X;
  long b_971X;
  long v_970X;
  long v_969X;
  long args_968X;
  long next_967X;
  long next_op_966X;
  long cont_965X;
  long index_964X;
  long length_963X;
  long wants_stack_args_962X;
  long skip_961X;
  long skip_960X;
  long stack_space_959X;
  long protocol_958X;
  long index_957X;
  long length_956X;
  long wants_stack_args_955X;
  long skip_954X;
  long skip_953X;
  long stack_space_952X;
  long protocol_951X;
  long n_950X;
  long tem_949X;
  long stack_arg_count_948X;
  long v_947X;
  long v_946X;
  long key_945X;
  long start_i_944X;
  long temp_943X;
  char okayP_942X;
  long key_941X;
  char okayP_940X;
  long obj_939X;
  long obj_938X;
  long type_937X;
  long thing_936X;
  long stuff_935X;
  char * addr_934X;
  long val_933X;
  long x_932X;
  long i_931X;
  long b_930X;
  long p_929X;
  long port_928X;
  long Kchar_927X;
  long x_926X;
  long i_925X;
  long b_924X;
  long p_923X;
  long p_922X;
  long port_921X;
  char * addr_920X;
  long val_919X;
  long x_918X;
  long i_917X;
  long b_916X;
  long p_915X;
  long p_914X;
  long port_913X;
  long list_912X;
  long head_911X;
  char move_slowP_910X;
  long slow_909X;
  long list_908X;
  long n_907X;
  long arg2_906X;
  long val_905X;
  long mseconds_904X;
  long seconds_903X;
  long option_902X;
  long key_901X;
  char okayP_900X;
  long result_899X;
  char * args_898X;
  long name_897X;
  long proc_896X;
  long rest_list_895X;
  long x_894X;
  long tem_893X;
  long pc_892X;
  long v_891X;
  long value_890X;
  long vector_889X;
  long type_888X;
  char firstP_887X;
  long v_886X;
  long vector_885X;
  char firstP_884X;
  long x_883X;
  char * addr_882X;
  long b_881X;
  long x_880X;
  char * addr_879X;
  long proc_878X;
  long stob_877X;
  long h_876X;
  long i_875X;
  long key_874X;
  char okayP_873X;
  long y_872X;
  long n_871X;
  char * addr_870X;
  long prev_869X;
  long ch_868X;
  long val_867X;
  long key_866X;
  char okayP_865X;
  long key_864X;
  char okayP_863X;
  long key_862X;
  char okayP_861X;
  long key_860X;
  char okayP_859X;
  long val_858X;
  char * addr_857X;
  char * addr_856X;
  long val_855X;
  long n_854X;
  long string_853X;
  long table_852X;
  long obj_851X;
  long string_850X;
  char * addr_849X;
  long len_848X;
  long vector_847X;
  char * addr_846X;
  long value_845X;
  char * addr_844X;
  long len_843X;
  long init_842X;
  char okayP_841X;
  long rest_list_840X;
  long stack_nargs_839X;
  long new_838X;
  char * addr_837X;
  long len_836X;
  long type_835X;
  long new_834X;
  char * addr_833X;
  long len_832X;
  long type_831X;
  long value_830X;
  long val_829X;
  long val_828X;
  long val_827X;
  long val_826X;
  long r_825X;
  long n_824X;
  long a_823X;
  long a_822X;
  long val_821X;
  long val_820X;
  long val_819X;
  long val_818X;
  long val_817X;
  long a_816X;
  long a_815X;
  long val_814X;
  long val_813X;
  long delta_812X;
  long delta_811X;
  long offset_810X;
  long key_809X;
  char okayP_808X;
  char v_807X;
  long tem_806X;
  long key_805X;
  char okayP_804X;
  long list_arg_count_803X;
  long list_args_802X;
  long stack_nargs_801X;
  long obj_800X;
  char * addr_799X;
  long list_args_798X;
  long follower_797X;
  long list_796X;
  long args_795X;
  long list_arg_count_794X;
  char okayP_793X;
  long stack_nargs_792X;
  long list_args_791X;
  long total_arg_count_790X;
  long code_789X;
  long obj_788X;
  long obj_787X;
  long list_arg_count_786X;
  long list_args_785X;
  long stack_arg_count_784X;
  long obj_783X;
  char * arg_782X;
  char * loc_781X;
  long args_780X;
  long v_779X;
  long v_778X;
  long v_777X;
  long list_arg_count_776X;
  long list_args_775X;
  long stack_arg_count_774X;
  long exception_773X;
  long code_772X;
  long stack_arg_count_771X;
  char v_770X;
  long key_769X;
  char okayP_768X;
  long key_767X;
  char okayP_766X;
  char * addr_765X;
  long value_764X;
  long v_763X;
  long new_env_762X;
  char * addr_761X;
  long len_760X;
  char v_759X;
  long temp1_758X;
  long temp0_757X;
  char v_756X;
  long env_755X;
  char v_754X;
  long temp1_753X;
  char v_752X;
  long space_751X;
  char * addr_750X;
  long value_749X;
  long index_748X;
  long i_747X;
  long env_746X;
  long i_745X;
  long env_744X;
  long i_743X;
  long env_742X;
  long obj_741X;
  long opcode_740X;
  long nargs_739X;
  FILE * out_738X;
  long x_737X;
  long v_736X;
  long v_735X;
  long v_734X;
  long v_733X;
  long v_732X;
  long v_731X;
  long v_730X;
  long count_729X;
  long to_index_728X;
  long from_index_727X;
  long arg5_726X;
  long arg4_725X;
  long arg3_724X;
  long arg2_723X;
  char * addr_722X;
  long len_721X;
  long value_720X;
  long index_719X;
  long arg4_718X;
  long arg3_717X;
  long arg2_716X;
  long len_715X;
  long index_714X;
  long arg3_713X;
  long arg2_712X;
  long list_711X;
  long thing_710X;
  char v_709X;
  long temp1_708X;
  char v_707X;
  long space_706X;
  long len_705X;
  long x_704X;
  long obj_703X;
  long arg2_702X;
  long x_701X;
  long arg2_700X;
  long status_699X;
  long value_698X;
  long key_697X;
  long arg2_696X;
  long mseconds_695X;
  long seconds_694X;
  long mseconds_693X;
  long seconds_692X;
  long x_691X;
  long other_690X;
  long option_689X;
  long arg2_688X;
  long x_687X;
  long arg2_686X;
  char v_685X;
  long temp1_684X;
  char v_683X;
  long rest_list_682X;
  long p_681X;
  long nargs_680X;
  long x_679X;
  long arg2_678X;
  long p_677X;
  long p_676X;
  long p_675X;
  long old_674X;
  long temp_673X;
  long obj_672X;
  long p_671X;
  long template_670X;
  long p_669X;
  long temp_668X;
  long obj_667X;
  long x_666X;
  long type_665X;
  long x_664X;
  long bytes_663X;
  long x_662X;
  long other_661X;
  long key_660X;
  long arg2_659X;
  char v_658X;
  long temp1_657X;
  char v_656X;
  long n_655X;
  long x_654X;
  long obj_653X;
  long v_652X;
  char v_651X;
  long temp1_650X;
  char v_649X;
  long x_648X;
  char v_647X;
  long temp1_646X;
  char v_645X;
  long space_644X;
  char * addr_643X;
  long next_642X;
  long channel_641X;
  long n_640X;
  char * addr_639X;
  long head_638X;
  long channel_637X;
  long obj_636X;
  char v_635X;
  long temp1_634X;
  char v_633X;
  char v_632X;
  long temp1_631X;
  char v_630X;
  char v_629X;
  long temp1_628X;
  char v_627X;
  char v_626X;
  long temp1_625X;
  char v_624X;
  long x_623X;
  long x_622X;
  long x_621X;
  long arg2_620X;
  long descriptor_619X;
  long x_618X;
  long obj_617X;
  char v_616X;
  long temp1_615X;
  char v_614X;
  long len_613X;
  char Kchar_612X;
  long index_611X;
  long arg3_610X;
  long arg2_609X;
  long len_608X;
  long index_607X;
  long arg2_606X;
  long obj_605X;
  char v_604X;
  long temp1_603X;
  char v_602X;
  long size_601X;
  char init_600X;
  long len_599X;
  long arg2_598X;
  long len_597X;
  long Kchar_596X;
  long index_595X;
  long arg3_594X;
  long arg2_593X;
  long len_592X;
  long index_591X;
  long arg2_590X;
  long obj_589X;
  char v_588X;
  long temp1_587X;
  char v_586X;
  long size_585X;
  long init_584X;
  long len_583X;
  long arg2_582X;
  char * addr_581X;
  long index_580X;
  long len_579X;
  long type_578X;
  long value_577X;
  long stob_576X;
  long arg2_575X;
  long index_574X;
  long len_573X;
  long type_572X;
  long index_571X;
  long stob_570X;
  char v_569X;
  long temp1_568X;
  long temp0_567X;
  char v_566X;
  long size_565X;
  long len_564X;
  long type_563X;
  long init_562X;
  long len_561X;
  char * addr_560X;
  long type_559X;
  long offset_558X;
  long value_557X;
  long stob_556X;
  long type_555X;
  long offset_554X;
  long stob_553X;
  char v_552X;
  long temp1_551X;
  char v_550X;
  long space_549X;
  long len_548X;
  char v_547X;
  long temp1_546X;
  char v_545X;
  long space_544X;
  long len_543X;
  long type_542X;
  long stob_541X;
  long type_540X;
  long x_539X;
  long x_538X;
  long x_537X;
  long x_536X;
  long arg2_535X;
  long x_534X;
  long arg2_533X;
  long x_532X;
  long x_531X;
  long result_530X;
  long x_529X;
  long x_528X;
  long count_527X;
  long value_526X;
  long y_525X;
  long x_524X;
  long x_523X;
  long arg2_522X;
  long x_521X;
  long arg2_520X;
  long x_519X;
  long arg2_518X;
  long x_517X;
  long x_516X;
  long arg2_515X;
  long x_514X;
  long arg2_513X;
  long x_512X;
  long arg2_511X;
  long n_510X;
  long n_509X;
  long x_508X;
  long x_507X;
  long x_506X;
  long x_505X;
  long x_504X;
  long x_503X;
  long x_502X;
  long x_501X;
  long x_500X;
  long x_499X;
  long x_498X;
  long x_497X;
  long x_496X;
  long x_495X;
  long a_494X;
  long y_493X;
  long x_492X;
  long b_491X;
  long a_490X;
  long x_489X;
  long arg2_488X;
  long x_487X;
  long arg2_486X;
  long x_485X;
  long arg2_484X;
  long x_483X;
  long arg2_482X;
  long x_481X;
  long arg2_480X;
  long x_479X;
  long arg2_478X;
  long b_477X;
  long a_476X;
  long x_475X;
  long arg2_474X;
  long z_473X;
  long y_472X;
  long x_471X;
  long b_470X;
  long a_469X;
  long x_468X;
  long arg2_467X;
  long z_466X;
  long y_465X;
  long x_464X;
  long x_463X;
  long x_462X;
  long x_461X;
  long n_460X;
  long n_459X;
  long n_458X;
  long n_457X;
  long x_456X;
  long x_455X;
  long arg2_454X;
  long rest_list_453X;
  long x_452X;
  long rest_list_451X;
  long stack_nargs_450X;
  long arg0_449X;
  long arg1_448X;
  long rest_list_447X;
  long arg0_446X;
  long stack_nargs_445X;
  long index_444X;
  long val_443X;
  long max_442X;
  long p_441X;
  char v_440X;
  long temp1_439X;
  char v_438X;
  long space_437X;
  long space_436X;
  long index_435X;
  long v_434X;
  char v_433X;
  long temp1_432X;
  char v_431X;
  long space_430X;
  long code_429X;
  long tem_428X;
  long stack_nargs_427X;
  long p_426X;
  long obj_425X;
  long consumer_424X;
  long cont_423X;
  long rest_list_422X;
  long proc_421X;
  long stack_nargs_420X;
  long args_419X;
  long length_418X;
  char okayP_417X;
  long list_args_416X;
  long stob_415X;
  char * top_of_args_414X;
  long nargs_413X;
  long obj_412X;
  long stack_arg_count_411X;
  char v_410X;
  long temp1_409X;
  char v_408X;
  long space_407X;
  long code_406X;
  long template_405X;
  long obj_404X;
  long stack_arg_count_403X;
  long cont_402X;
  char v_401X;
  long temp1_400X;
  char v_399X;
  long space_398X;
  char v_397X;
  long temp1_396X;
  char v_395X;
  long space_394X;
  long total_count_393X;
  long p_392X;
  long v_391X;
  char * addr_390X;
  long val_389X;
  long location_388X;
  long location_387X;
  long env_386X;
  long back_385X;
  long env_384X;
  long back_383X;
  long env_382X;
  long back_381X;
  long count_380X;
  char * code_pointer_379X;
  long code_378X;
  char * addr_377X;
  long proc_376X;
  long temp_375X;
  char okayP_374X;
  char v_373X;
  long temp1_372X;
  long temp0_371X;
  char v_370X;
 {  v_370X = AVAILABLEp(2);
  if (v_370X) {
    arg2K0 = 1;
    arg0K1 = proc_345X;
    goto L19375;}
  else {
    temp0_371X = collect_saving_temps(proc_345X, 1, &temp1_372X);
    v_373X = AVAILABLEp(2);
    if (v_373X) {
      arg2K0 = 1;
      arg0K1 = temp0_371X;
      goto L19375;}
    else {
      arg2K0 = 0;
      arg0K1 = temp0_371X;
      goto L19375;}}}
 L19375: {
  okayP_374X = arg2K0;
  temp_375X = arg0K1;
  if (okayP_374X) {
    arg0K0 = temp_375X;
    goto L19346;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_375X;
    goto L19346;}}
 L19346: {
  proc_376X = arg0K0;
  addr_377X = ALLOCATE_SPACE(17, 6);
  *((long *) addr_377X) = 582;
  code_378X = 3 + (((long) (addr_377X + 4)));
  *((unsigned char *) (((char *) (-3 + code_378X)))) = 24;
  *((unsigned char *) ((((char *) (-3 + code_378X))) + 1)) = nargs_346X;
  Scode_pointerS = (((char *) (-3 + code_378X)));
  SvalS = proc_376X;
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L18438: {
  code_pointer_379X = arg1K0;
  switch ((*((unsigned char *) code_pointer_379X))) {
    case 0 : 
    case 145 : {
      push_exception_continuationB(15, 1);
      arg0K0 = 0;
      goto L16665;}
      break;
    case 1 : {
      count_380X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      *((long *) (SstackS)) = (SenvS);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (1034 + (((count_380X)<<10)));
      SstackS = ((SstackS) + -4);
      SenvS = (3 + (((long) ((SstackS) + 8))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 2 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 3 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 4 : {
      back_381X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_382X = SenvS;
      arg0K0 = env_382X;
      arg0K1 = back_381X;
      goto L23824;}
      break;
    case 5 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L16665;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
      break;
    case 6 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L16665;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
      break;
    case 7 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS)))))))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L16665;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
      break;
    case 8 : {
      back_383X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_384X = SenvS;
      arg0K0 = env_384X;
      arg0K1 = back_383X;
      goto L20247;}
      break;
    case 9 : {
      back_385X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_386X = SenvS;
      arg0K0 = env_386X;
      arg0K1 = back_385X;
      goto L20185;}
      break;
    case 10 : {
      location_387X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_387X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        push_exception_continuationB(1, 3);
        *((long *) (SstackS)) = location_387X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
      break;
    case 11 : {
      location_388X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      if ((273 == (*((long *) ((((char *) (-3 + location_388X))) + 4))))) {
        push_exception_continuationB(1, 3);
        *((long *) (SstackS)) = location_388X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}
      else {
        val_389X = SvalS;
        addr_390X = (((char *) (-3 + location_388X))) + 4;
        S48_WRITE_BARRIER(location_388X, addr_390X, val_389X);
        *((long *) addr_390X) = val_389X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
      break;
    case 12 : {
      v_391X = *((unsigned char *) ((Scode_pointerS) + 3));
      if ((0 == v_391X)) {
        p_392X = SenvS;
        if ((3 == (3 & p_392X))) {
          if ((p_392X < (((long) (Sstack_beginS))))) {
            arg0K0 = 0;
            goto L20964;}
          else {
            if (((((long) (Sstack_endS))) < p_392X)) {
              arg0K0 = 0;
              goto L20964;}
            else {
              arg0K0 = ((Sstack_endS) - (Sstack_beginS));
              goto L20964;}}}
        else {
          arg0K0 = 0;
          goto L20964;}}
      else {
        arg0K0 = (SvalS);
        goto L20971;}}
      break;
    case 13 : {
      total_count_393X = *((unsigned char *) ((Scode_pointerS) + 2));
      space_394X = 1 + total_count_393X;
      v_395X = AVAILABLEp(space_394X);
      if (v_395X) {
        goto L20766;}
      else {
        collect_saving_temps(1, 1, &temp1_396X);
        v_397X = AVAILABLEp(space_394X);
        if (v_397X) {
          goto L20766;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L20766;}}}
      break;
    case 14 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 15 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L16665;}
      else {
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
      break;
    case 16 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L16665;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
      break;
    case 17 : {
      SstackS = ((SstackS) + 4);
      SvalS = (*((long *) (SstackS)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 18 : {
      SvalS = (*((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 19 : {
      *((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 20 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (*((unsigned char *) ((Scode_pointerS) + 3))));
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 21 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))));
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 22 : {
      space_398X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
      v_399X = AVAILABLEp(space_398X);
      if (v_399X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L20692;}
      else {
        collect_saving_temps(1, 1, &temp1_400X);
        v_401X = AVAILABLEp(space_398X);
        if (v_401X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L20692;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L20692;}}}
      break;
    case 23 : {
      cont_402X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
      if ((3 == (3 & cont_402X))) {
        if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_402X))) + -4))))>>2))))) {
          if ((3 == (3 & cont_402X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_402X))) + -4))))>>2))))) {
              merged_arg0K0 = cont_402X;
              copy_continuation_from_heapB_return_tag = 0;
              goto copy_continuation_from_heapB;
             copy_continuation_from_heapB_return_0:
              goto L19847;}
            else {
              goto L19882;}}
          else {
            goto L19882;}}
        else {
          goto L19848;}}
      else {
        goto L19848;}}
      break;
    case 24 : {
      stack_arg_count_403X = *((unsigned char *) ((Scode_pointerS) + 1));
      obj_404X = SvalS;
      if ((3 == (3 & obj_404X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_404X))) + -4))))>>2))))) {
          template_405X = *((long *) (((char *) (-3 + (SvalS)))));
          code_406X = *((long *) (((char *) (-3 + template_405X))));
          if ((stack_arg_count_403X == (*((unsigned char *) ((((char *) (-3 + code_406X))) + 1))))) {
            StemplateS = template_405X;
            SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
            Scode_pointerS = ((((char *) (-3 + code_406X))) + 2);
            if (((SstackS) < (Sstack_limitS))) {
              space_407X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
              v_408X = AVAILABLEp(space_407X);
              if (v_408X) {
                arg2K0 = 1;
                arg0K1 = 0;
                goto L19208;}
              else {
                collect_saving_temps(1, 1, &temp1_409X);
                v_410X = AVAILABLEp(space_407X);
                if (v_410X) {
                  arg2K0 = 1;
                  arg0K1 = 0;
                  goto L19208;}
                else {
                  arg2K0 = 0;
                  arg0K1 = 0;
                  goto L19208;}}}
            else {
              goto L19120;}}
          else {
            arg0K0 = stack_arg_count_403X;
            goto L16066;}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_403X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L17161;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_403X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17161;}}
      break;
    case 25 : {
      stack_arg_count_411X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      obj_412X = SvalS;
      if ((3 == (3 & obj_412X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_412X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_411X;
          goto L16066;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_411X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L17161;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_411X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17161;}}
      break;
    case 26 : {
      nargs_413X = *((unsigned char *) ((Scode_pointerS) + 1));
      top_of_args_414X = SstackS;
      stob_415X = ScontS;
      arg1K0 = ((((char *) (-3 + stob_415X))) + -8);
      arg1K1 = (top_of_args_414X + (((nargs_413X)<<2)));
      goto L17628;}
      break;
    case 27 : {
      SstackS = ((SstackS) + 4);
      list_args_416X = *((long *) (SstackS));
      merged_arg0K0 = list_args_416X;
      okay_argument_list_return_tag = 0;
      goto okay_argument_list;
     okay_argument_list_return_0:
      okayP_417X = okay_argument_list0_return_value;
      length_418X = okay_argument_list1_return_value;
      if (okayP_417X) {
        arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
        arg0K1 = list_args_416X;
        arg0K2 = length_418X;
        goto L17666;}
      else {
        *((long *) (SstackS)) = list_args_416X;
        SstackS = ((SstackS) + -4);
        merged_arg0K0 = 25;
        merged_arg0K1 = (1 + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        pop_args_GlistS_return_tag = 0;
        goto pop_args_GlistS;
       pop_args_GlistS_return_0:
        args_419X = pop_args_GlistS0_return_value;
        push_exception_continuationB(5, 0);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = args_419X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 28 : {
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      stack_nargs_420X = (((*((long *) (SstackS))))>>2);
      proc_421X = *((long *) ((SstackS) + (4 + (((stack_nargs_420X)<<2)))));
      *((long *) ((SstackS) + (4 + (((stack_nargs_420X)<<2))))) = 1;
      SstackS = ((SstackS) + 4);
      rest_list_422X = *((long *) (SstackS));
      if ((25 == rest_list_422X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = (-2 + stack_nargs_420X);
        goto L11273;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_422X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_422X)))));
          arg0K1 = (-1 + stack_nargs_420X);
          goto L11273;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_422X))) + 4)))))) + 4)));
          arg0K1 = rest_list_422X;
          goto L11314;}}}
      break;
    case 29 : {
      SstackS = ((SstackS) + 4);
      cont_423X = *((long *) (SstackS));
      if ((3 == (3 & cont_423X))) {
        if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_423X))) + -4))))>>2))))) {
          merged_arg0K0 = cont_423X;
          copy_continuation_from_heapB_return_tag = 1;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_1:
          goto L17571;}
        else {
          goto L17578;}}
      else {
        goto L17578;}}
      break;
    case 30 : {
      SstackS = ((SstackS) + 4);
      consumer_424X = *((long *) (SstackS));
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = consumer_424X;
      obj_425X = SvalS;
      if ((3 == (3 & obj_425X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_425X))) + -4))))>>2))))) {
          arg0K0 = 1;
          goto L16066;}
        else {
          arg0K0 = 3;
          arg0K1 = 1;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L17161;}}
      else {
        arg0K0 = 3;
        arg0K1 = 1;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17161;}}
      break;
    case 31 : {
      pop_continuationB_return_tag = 0;
      goto pop_continuationB;
     pop_continuationB_return_0:
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 32 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L20427;}
      break;
    case 33 : {
      SstackS = ((SstackS) + 4);
      p_426X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_427X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      arg0K0 = stack_nargs_427X;
      arg0K1 = (*((long *) (SstackS)));
      arg0K2 = ((((p_426X)>>2)) - stack_nargs_427X);
      goto L20427;}
      break;
    case 34 : {
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 35 : {
      tem_428X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      StemplateS = tem_428X;
      Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_428X))))))));
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 36 : {
      code_429X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))))))));
      if ((0 == (*((unsigned char *) ((((char *) (-3 + code_429X))) + 1))))) {
        if (((SstackS) < (Sstack_limitS))) {
          space_430X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
          v_431X = AVAILABLEp(space_430X);
          if (v_431X) {
            arg2K0 = 1;
            arg0K1 = 0;
            goto L18944;}
          else {
            collect_saving_temps(1, 1, &temp1_432X);
            v_433X = AVAILABLEp(space_430X);
            if (v_433X) {
              arg2K0 = 1;
              arg0K1 = 0;
              goto L18944;}
            else {
              arg2K0 = 0;
              arg0K1 = 0;
              goto L18944;}}}
        else {
          goto L18817;}}
      else {
        v_434X = *((unsigned char *) ((((char *) (-3 + code_429X))) + 1));
        if ((66 == v_434X)) {
          if ((0 == (*((unsigned char *) ((((char *) (-3 + code_429X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_429X))) + -4))))>>8)))))))) {
            index_435X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_429X))) + -4))))>>8));
            space_436X = ((((*((unsigned char *) ((((char *) (-3 + code_429X))) + index_435X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_429X))) + (1 + index_435X))));
            if ((space_436X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
              goto L18817;}
            else {
              space_437X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
              v_438X = AVAILABLEp(space_437X);
              if (v_438X) {
                arg2K0 = 1;
                arg0K1 = 0;
                goto L19000;}
              else {
                collect_saving_temps(1, 1, &temp1_439X);
                v_440X = AVAILABLEp(space_437X);
                if (v_440X) {
                  arg2K0 = 1;
                  arg0K1 = 0;
                  goto L19000;}
                else {
                  arg2K0 = 0;
                  arg0K1 = 0;
                  goto L19000;}}}}
          else {
            goto L18862;}}
        else {
          goto L18862;}}}
      break;
    case 37 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
      break;
    case 38 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 39 : {
      if ((0 == (3 & (SvalS)))) {
        p_441X = SvalS;
        max_442X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_443X = ((p_441X)>>2);
        if ((val_443X < 0)) {
          goto L18718;}
        else {
          if ((val_443X < max_442X)) {
            index_444X = 1 + (((val_443X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_444X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_444X)))));
            goto L18720;}
          else {
            goto L18718;}}}
      else {
        push_exception_continuationB(5, 0);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 40 : {
      stack_nargs_445X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((0 == stack_nargs_445X)) {
        arg0_446X = *((long *) ((SstackS) + 12));
        rest_list_447X = *((long *) ((SstackS) + 8));
        *((long *) (SstackS)) = arg0_446X;
        SstackS = ((SstackS) + -4);
        SvalS = (*((long *) (((char *) (-3 + rest_list_447X)))));
        goto L19750;}
      else {
        arg1_448X = *((long *) ((SstackS) + (4 + (((stack_nargs_445X)<<2)))));
        arg0_449X = *((long *) ((SstackS) + (8 + (((stack_nargs_445X)<<2)))));
        *((long *) ((SstackS) + (8 + (((stack_nargs_445X)<<2))))) = 1;
        *((long *) ((SstackS) + 4)) = (-4 + (((stack_nargs_445X)<<2)));
        *((long *) (SstackS)) = arg0_449X;
        SstackS = ((SstackS) + -4);
        SvalS = arg1_448X;
        goto L19750;}}
      break;
    case 41 : {
      stack_nargs_450X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((stack_nargs_450X == 0)) {
        rest_list_451X = *((long *) ((SstackS) + 8));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_451X))) + 4))))) {
          arg0K0 = 1;
          goto L18616;}
        else {
          *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_451X))) + 4)));
          *((long *) ((SstackS) + 12)) = (SvalS);
          arg0K0 = -2;
          goto L18616;}}
      else {
        if ((stack_nargs_450X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 8))))) {
            arg0K0 = 1;
            goto L18616;}
          else {
            *((long *) ((SstackS) + 4)) = 0;
            *((long *) ((SstackS) + 12)) = (SvalS);
            arg0K0 = -2;
            goto L18616;}}
        else {
          *((long *) ((SstackS) + (8 + (((stack_nargs_450X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L18616;}}}
      break;
    case 42 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        x_452X = (((*((long *) ((SstackS) + 4))))>>2);
        if ((x_452X == 0)) {
          rest_list_453X = *((long *) ((SstackS) + 8));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_453X))) + 4))))) {
            arg0K0 = 1;
            goto L19641;}
          else {
            *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_453X))) + 4)));
            *((long *) ((SstackS) + 12)) = (*((long *) (((char *) (-3 + rest_list_453X)))));
            arg0K0 = -2;
            goto L19641;}}
        else {
          if ((x_452X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 8))))) {
              arg0K0 = 1;
              goto L19641;}
            else {
              *((long *) ((SstackS) + 4)) = 0;
              arg0K0 = -2;
              goto L19641;}}
          else {
            arg0K0 = -2;
            goto L19641;}}}}
      break;
    case 43 : {
      SstackS = ((SstackS) + 4);
      arg2_454X = *((long *) (SstackS));
      x_455X = SvalS;
      if ((arg2_454X == x_455X)) {
        arg0K0 = 5;
        goto L38026;}
      else {
        arg0K0 = 1;
        goto L38026;}}
      break;
    case 44 : {
      x_456X = SvalS;
      if ((0 == (3 & x_456X))) {
        arg0K0 = 5;
        goto L38038;}
      else {
        if ((3 == (3 & x_456X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_456X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L38038;}
          else {
            goto L10622;}}
        else {
          goto L10622;}}}
      break;
    case 45 : {
      n_457X = SvalS;
      if ((0 == (3 & n_457X))) {
        goto L28391;}
      else {
        if ((3 == (3 & n_457X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_457X))) + -4))))>>2))))) {
            goto L28391;}
          else {
            goto L28392;}}
        else {
          goto L28392;}}}
      break;
    case 46 : {
      n_458X = SvalS;
      if ((0 == (3 & n_458X))) {
        goto L28548;}
      else {
        if ((3 == (3 & n_458X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_458X))) + -4))))>>2))))) {
            goto L28548;}
          else {
            goto L28523;}}
        else {
          goto L28523;}}}
      break;
    case 47 : {
      n_459X = SvalS;
      if ((0 == (3 & n_459X))) {
        goto L28747;}
      else {
        if ((3 == (3 & n_459X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_459X))) + -4))))>>2))))) {
            goto L28747;}
          else {
            goto L28722;}}
        else {
          goto L28722;}}}
      break;
    case 48 : {
      n_460X = SvalS;
      if ((0 == (3 & n_460X))) {
        goto L28946;}
      else {
        if ((3 == (3 & n_460X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_460X))) + -4))))>>2))))) {
            goto L28946;}
          else {
            goto L28921;}}
        else {
          goto L28921;}}}
      break;
    case 49 : {
      x_461X = SvalS;
      if ((0 == (3 & x_461X))) {
        SvalS = 5;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_461X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 50 : {
      x_462X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_462X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 51 : {
      x_463X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_463X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 52 : {
      SstackS = ((SstackS) + 4);
      x_464X = *((long *) (SstackS));
      y_465X = SvalS;
      if ((0 == (3 & (x_464X | y_465X)))) {
        z_466X = (((x_464X)>>2)) + (((y_465X)>>2));
        if ((536870911 < z_466X)) {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = x_464X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = y_465X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L16665;}
        else {
          if ((z_466X < -536870912)) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = x_464X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_465X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L16665;}
          else {
            SvalS = (((z_466X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L18438;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_464X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_465X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 53 : {
      SstackS = ((SstackS) + 4);
      arg2_467X = *((long *) (SstackS));
      x_468X = SvalS;
      if ((0 == (3 & (arg2_467X | x_468X)))) {
        a_469X = ((arg2_467X)>>2);
        b_470X = ((x_468X)>>2);
        if ((a_469X < 0)) {
          arg0K0 = (0 - a_469X);
          goto L6775;}
        else {
          arg0K0 = a_469X;
          goto L6775;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_467X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_468X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 54 : {
      SstackS = ((SstackS) + 4);
      x_471X = *((long *) (SstackS));
      y_472X = SvalS;
      if ((0 == (3 & (x_471X | y_472X)))) {
        z_473X = (((x_471X)>>2)) - (((y_472X)>>2));
        if ((536870911 < z_473X)) {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = x_471X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = y_472X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L16665;}
        else {
          if ((z_473X < -536870912)) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = x_471X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_472X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L16665;}
          else {
            SvalS = (((z_473X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L18438;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_471X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_472X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 55 : {
      SstackS = ((SstackS) + 4);
      arg2_474X = *((long *) (SstackS));
      x_475X = SvalS;
      if ((0 == (3 & (arg2_474X | x_475X)))) {
        if ((0 == x_475X)) {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = arg2_474X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_475X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L16665;}
        else {
          a_476X = ((arg2_474X)>>2);
          b_477X = ((x_475X)>>2);
          if ((a_476X < 0)) {
            arg0K0 = (0 - a_476X);
            goto L7056;}
          else {
            arg0K0 = a_476X;
            goto L7056;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_474X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_475X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 56 : {
      SstackS = ((SstackS) + 4);
      arg2_478X = *((long *) (SstackS));
      x_479X = SvalS;
      if ((0 == (3 & (arg2_478X | x_479X)))) {
        if ((arg2_478X == x_479X)) {
          arg0K0 = 5;
          goto L31744;}
        else {
          arg0K0 = 1;
          goto L31744;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_478X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_479X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 57 : {
      SstackS = ((SstackS) + 4);
      arg2_480X = *((long *) (SstackS));
      x_481X = SvalS;
      if ((0 == (3 & (arg2_480X | x_481X)))) {
        if ((arg2_480X < x_481X)) {
          arg0K0 = 5;
          goto L31822;}
        else {
          arg0K0 = 1;
          goto L31822;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_480X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_481X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 58 : {
      SstackS = ((SstackS) + 4);
      arg2_482X = *((long *) (SstackS));
      x_483X = SvalS;
      if ((0 == (3 & (arg2_482X | x_483X)))) {
        if ((x_483X < arg2_482X)) {
          arg0K0 = 5;
          goto L31900;}
        else {
          arg0K0 = 1;
          goto L31900;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_482X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_483X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 59 : {
      SstackS = ((SstackS) + 4);
      arg2_484X = *((long *) (SstackS));
      x_485X = SvalS;
      if ((0 == (3 & (arg2_484X | x_485X)))) {
        if ((x_485X < arg2_484X)) {
          arg0K0 = 1;
          goto L31978;}
        else {
          arg0K0 = 5;
          goto L31978;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_484X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_485X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 60 : {
      SstackS = ((SstackS) + 4);
      arg2_486X = *((long *) (SstackS));
      x_487X = SvalS;
      if ((0 == (3 & (arg2_486X | x_487X)))) {
        if ((arg2_486X < x_487X)) {
          arg0K0 = 1;
          goto L32056;}
        else {
          arg0K0 = 5;
          goto L32056;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_486X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_487X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 61 : {
      SstackS = ((SstackS) + 4);
      arg2_488X = *((long *) (SstackS));
      x_489X = SvalS;
      if ((0 == (3 & (arg2_488X | x_489X)))) {
        if ((0 == x_489X)) {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = arg2_488X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_489X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L16665;}
        else {
          a_490X = ((arg2_488X)>>2);
          b_491X = ((x_489X)>>2);
          if ((a_490X < 0)) {
            arg0K0 = (0 - a_490X);
            goto L7233;}
          else {
            arg0K0 = a_490X;
            goto L7233;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_488X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_489X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 62 : {
      SstackS = ((SstackS) + 4);
      x_492X = *((long *) (SstackS));
      y_493X = SvalS;
      if ((0 == (3 & (x_492X | y_493X)))) {
        if ((0 == y_493X)) {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = x_492X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = y_493X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L16665;}
        else {
          a_494X = ((x_492X)>>2);
          if ((a_494X < 0)) {
            arg0K0 = (0 - a_494X);
            goto L25338;}
          else {
            arg0K0 = a_494X;
            goto L25338;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_492X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_493X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 63 : {
      x_495X = SvalS;
      if ((0 == (3 & x_495X))) {
        SvalS = x_495X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_495X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 64 : {
      x_496X = SvalS;
      if ((0 == (3 & x_496X))) {
        SvalS = x_496X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_496X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 65 : {
      x_497X = SvalS;
      if ((0 == (3 & x_497X))) {
        SvalS = 4;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_497X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 66 : {
      x_498X = SvalS;
      if ((0 == (3 & x_498X))) {
        SvalS = x_498X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_498X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 67 : {
      x_499X = SvalS;
      if ((0 == (3 & x_499X))) {
        SvalS = 0;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_499X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 68 : {
      x_500X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_500X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 69 : {
      x_501X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_501X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 70 : {
      x_502X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_502X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 71 : {
      x_503X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_503X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 72 : {
      x_504X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_504X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 73 : {
      x_505X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_505X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 74 : {
      x_506X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_506X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 75 : {
      x_507X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_507X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 76 : {
      x_508X = SvalS;
      if ((0 == (3 & x_508X))) {
        if ((x_508X < 0)) {
          goto L38295;}
        else {
          SvalS = 0;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}}
      else {
        goto L38295;}}
      break;
    case 77 : {
      n_509X = SvalS;
      if ((0 == (3 & n_509X))) {
        n_510X = ((n_509X)>>2);
        if ((n_510X < 0)) {
          arg0K0 = (0 - n_510X);
          goto L25624;}
        else {
          arg0K0 = n_510X;
          goto L25624;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = n_509X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 78 : {
      SstackS = ((SstackS) + 4);
      arg2_511X = *((long *) (SstackS));
      x_512X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_511X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_512X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}
      break;
    case 79 : {
      SstackS = ((SstackS) + 4);
      arg2_513X = *((long *) (SstackS));
      x_514X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_513X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_514X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}
      break;
    case 80 : {
      SstackS = ((SstackS) + 4);
      arg2_515X = *((long *) (SstackS));
      x_516X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_515X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_516X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}
      break;
    case 81 : {
      x_517X = SvalS;
      if ((0 == (3 & x_517X))) {
        SvalS = (~ (3 | x_517X));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_517X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 82 : {
      SstackS = ((SstackS) + 4);
      arg2_518X = *((long *) (SstackS));
      x_519X = SvalS;
      if ((0 == (3 & (arg2_518X | x_519X)))) {
        SvalS = (arg2_518X & x_519X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_518X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_519X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 83 : {
      SstackS = ((SstackS) + 4);
      arg2_520X = *((long *) (SstackS));
      x_521X = SvalS;
      if ((0 == (3 & (arg2_520X | x_521X)))) {
        SvalS = (arg2_520X | x_521X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_520X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_521X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 84 : {
      SstackS = ((SstackS) + 4);
      arg2_522X = *((long *) (SstackS));
      x_523X = SvalS;
      if ((0 == (3 & (arg2_522X | x_523X)))) {
        SvalS = (arg2_522X ^ x_523X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_522X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_523X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 85 : {
      SstackS = ((SstackS) + 4);
      x_524X = *((long *) (SstackS));
      y_525X = SvalS;
      if ((0 == (3 & (x_524X | y_525X)))) {
        value_526X = ((x_524X)>>2);
        count_527X = ((y_525X)>>2);
        if ((count_527X < 0)) {
          PS_SHIFT_RIGHT(value_526X, (0 - count_527X), x_528X)
          SvalS = (((x_528X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          PS_SHIFT_LEFT(value_526X, count_527X, x_529X)
          result_530X = (((((x_529X)<<2)))>>2);
          PS_SHIFT_RIGHT(result_530X, count_527X, x_531X)
          if ((value_526X == x_531X)) {
            if ((value_526X < 0)) {
              if ((result_530X < 0)) {
                goto L25903;}
              else {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = x_524X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = y_525X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L16665;}}
            else {
              if ((result_530X < 0)) {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = x_524X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = y_525X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L16665;}
              else {
                goto L25903;}}}
          else {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = x_524X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_525X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L16665;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_524X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_525X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 86 : {
      x_532X = SvalS;
      if ((9 == (255 & x_532X))) {
        arg0K0 = 5;
        goto L38383;}
      else {
        arg0K0 = 1;
        goto L38383;}}
      break;
    case 87 : {
      SstackS = ((SstackS) + 4);
      arg2_533X = *((long *) (SstackS));
      if ((9 == (255 & arg2_533X))) {
        if ((9 == (255 & (SvalS)))) {
          x_534X = SvalS;
          if ((arg2_533X == x_534X)) {
            arg0K0 = 5;
            goto L28166;}
          else {
            arg0K0 = 1;
            goto L28166;}}
        else {
          goto L28139;}}
      else {
        goto L28139;}}
      break;
    case 88 : {
      SstackS = ((SstackS) + 4);
      arg2_535X = *((long *) (SstackS));
      if ((9 == (255 & arg2_535X))) {
        if ((9 == (255 & (SvalS)))) {
          x_536X = SvalS;
          if ((arg2_535X < x_536X)) {
            arg0K0 = 5;
            goto L28076;}
          else {
            arg0K0 = 1;
            goto L28076;}}
        else {
          goto L28049;}}
      else {
        goto L28049;}}
      break;
    case 89 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 90 : {
      if ((0 == (3 & (SvalS)))) {
        x_537X = (((SvalS))>>2);
        if ((255 < x_537X)) {
          goto L32748;}
        else {
          if ((x_537X < 0)) {
            goto L32748;}
          else {
            SvalS = (9 + (((((x_537X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L18438;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 91 : {
      x_538X = SvalS;
      if ((21 == x_538X)) {
        arg0K0 = 5;
        goto L38412;}
      else {
        arg0K0 = 1;
        goto L38412;}}
      break;
    case 92 : {
      x_539X = SvalS;
      type_540X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_539X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_539X))) + -4))))>>2))) == type_540X)) {
          arg0K0 = 5;
          goto L38426;}
        else {
          arg0K0 = 1;
          goto L38426;}}
      else {
        arg0K0 = 1;
        goto L38426;}}
      break;
    case 93 : {
      stob_541X = SvalS;
      type_542X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_541X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_541X))) + -4))))>>2))) == type_542X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_541X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          goto L21229;}}
      else {
        goto L21229;}}
      break;
    case 94 : {
      len_543X = *((unsigned char *) ((Scode_pointerS) + 1));
      space_544X = 4 + (((len_543X)<<2));
      v_545X = AVAILABLEp(space_544X);
      if (v_545X) {
        goto L21301;}
      else {
        collect_saving_temps(1, 1, &temp1_546X);
        v_547X = AVAILABLEp(space_544X);
        if (v_547X) {
          goto L21301;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L21301;}}}
      break;
    case 95 : {
      SstackS = ((SstackS) + 4);
      len_548X = (((*((long *) (SstackS))))>>2);
      space_549X = 4 + (((len_548X)<<2));
      v_550X = AVAILABLEp(space_549X);
      if (v_550X) {
        goto L21483;}
      else {
        collect_saving_temps(1, 1, &temp1_551X);
        v_552X = AVAILABLEp(space_549X);
        if (v_552X) {
          goto L21483;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L21483;}}}
      break;
    case 96 : {
      stob_553X = SvalS;
      offset_554X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_555X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_553X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_553X))) + -4))))>>2))) == type_555X)) {
          SvalS = (*((long *) ((((char *) (-3 + stob_553X))) + (((offset_554X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          goto L21693;}}
      else {
        goto L21693;}}
      break;
    case 97 : {
      SstackS = ((SstackS) + 4);
      stob_556X = *((long *) (SstackS));
      value_557X = SvalS;
      offset_558X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_559X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_556X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_556X))) + -4))))>>2))) == type_559X)) {
          if ((3 == (3 & stob_556X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_556X))) + -4)))))) {
              addr_560X = (((char *) (-3 + stob_556X))) + (((offset_558X)<<2));
              S48_WRITE_BARRIER(stob_556X, addr_560X, value_557X);
              *((long *) addr_560X) = value_557X;
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 3);
              arg1K0 = (Scode_pointerS);
              goto L18438;}
            else {
              goto L21796;}}
          else {
            goto L21796;}}
        else {
          goto L21796;}}
      else {
        goto L21796;}}
      break;
    case 98 : {
      SstackS = ((SstackS) + 4);
      len_561X = *((long *) (SstackS));
      init_562X = SvalS;
      type_563X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & len_561X))) {
        len_564X = ((len_561X)>>2);
        size_565X = 1 + len_564X;
        if ((len_564X < 0)) {
          goto L21981;}
        else {
          if ((4194305 < size_565X)) {
            goto L21981;}
          else {
            v_566X = AVAILABLEp(size_565X);
            if (v_566X) {
              arg2K0 = 1;
              arg0K1 = init_562X;
              goto L21999;}
            else {
              temp0_567X = collect_saving_temps(init_562X, 1, &temp1_568X);
              v_569X = AVAILABLEp(size_565X);
              if (v_569X) {
                arg2K0 = 1;
                arg0K1 = temp0_567X;
                goto L21999;}
              else {
                arg2K0 = 0;
                arg0K1 = temp0_567X;
                goto L21999;}}}}}
      else {
        push_exception_continuationB(5, 2);
        *((long *) (SstackS)) = (((type_563X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = len_561X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = init_562X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L16665;}}
      break;
    case 99 : {
      SstackS = ((SstackS) + 4);
      stob_570X = *((long *) (SstackS));
      index_571X = SvalS;
      type_572X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & index_571X))) {
        if ((3 == (3 & stob_570X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + stob_570X))) + -4))))>>2))) == type_572X)) {
            len_573X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_570X))) + -4))))>>8))))>>2);
            index_574X = ((index_571X)>>2);
            if ((index_574X < 0)) {
              goto L22277;}
            else {
              if ((index_574X < len_573X)) {
                SvalS = (*((long *) ((((char *) (-3 + stob_570X))) + (-4 & index_571X))));
                Scode_pointerS = ((Scode_pointerS) + 2);
                arg1K0 = (Scode_pointerS);
                goto L18438;}
              else {
                goto L22277;}}}
          else {
            goto L22254;}}
        else {
          goto L22254;}}
      else {
        goto L22254;}}
      break;
    case 100 : {
      SstackS = ((SstackS) + 4);
      arg2_575X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stob_576X = *((long *) (SstackS));
      value_577X = SvalS;
      type_578X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_575X))) {
        if ((3 == (3 & stob_576X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + stob_576X))) + -4))))>>2))) == type_578X)) {
            if ((3 == (3 & stob_576X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_576X))) + -4)))))) {
                len_579X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_576X))) + -4))))>>8))))>>2);
                index_580X = ((arg2_575X)>>2);
                if ((index_580X < 0)) {
                  goto L22517;}
                else {
                  if ((index_580X < len_579X)) {
                    addr_581X = (((char *) (-3 + stob_576X))) + (-4 & arg2_575X);
                    S48_WRITE_BARRIER(stob_576X, addr_581X, value_577X);
                    *((long *) addr_581X) = value_577X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 2);
                    arg1K0 = (Scode_pointerS);
                    goto L18438;}
                  else {
                    goto L22517;}}}
              else {
                goto L22492;}}
            else {
              goto L22492;}}
          else {
            goto L22492;}}
        else {
          goto L22492;}}
      else {
        goto L22492;}}
      break;
    case 101 : {
      SstackS = ((SstackS) + 4);
      arg2_582X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_582X | (SvalS))))) {
        len_583X = ((arg2_582X)>>2);
        init_584X = (((SvalS))>>2);
        size_585X = 1 + ((((3 + len_583X))>>2));
        if ((len_583X < 0)) {
          goto L26074;}
        else {
          if ((4194305 < size_585X)) {
            goto L26074;}
          else {
            v_586X = AVAILABLEp(size_585X);
            if (v_586X) {
              goto L26166;}
            else {
              collect_saving_temps(1, 1, &temp1_587X);
              v_588X = AVAILABLEp(size_585X);
              if (v_588X) {
                goto L26166;}
              else {
                push_exception_continuationB(8, 1);
                *((long *) (SstackS)) = (((len_583X)<<2));
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = (((init_584X)<<2));
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L16665;}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_582X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 102 : {
      obj_589X = SvalS;
      if ((3 == (3 & obj_589X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_589X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          goto L36239;}}
      else {
        goto L36239;}}
      break;
    case 103 : {
      SstackS = ((SstackS) + 4);
      arg2_590X = *((long *) (SstackS));
      if ((3 == (3 & arg2_590X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_590X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_591X = (((SvalS))>>2);
            len_592X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_590X))) + -4))))>>8);
            if ((index_591X < 0)) {
              goto L32854;}
            else {
              if ((index_591X < len_592X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_590X))) + index_591X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L18438;}
              else {
                goto L32854;}}}
          else {
            goto L35818;}}
        else {
          goto L35818;}}
      else {
        goto L35818;}}
      break;
    case 104 : {
      SstackS = ((SstackS) + 4);
      arg2_593X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_594X = *((long *) (SstackS));
      if ((3 == (3 & arg3_594X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_594X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_593X | (SvalS))))) {
            index_595X = ((arg2_593X)>>2);
            Kchar_596X = (((SvalS))>>2);
            if ((3 == (3 & arg3_594X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_594X))) + -4)))))) {
                len_597X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_594X))) + -4))))>>8);
                if ((index_595X < 0)) {
                  goto L29802;}
                else {
                  if ((index_595X < len_597X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_594X))) + index_595X)) = Kchar_596X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L18438;}
                  else {
                    goto L29802;}}}
              else {
                goto L29781;}}
            else {
              goto L29781;}}
          else {
            goto L34230;}}
        else {
          goto L34230;}}
      else {
        goto L34230;}}
      break;
    case 105 : {
      SstackS = ((SstackS) + 4);
      arg2_598X = *((long *) (SstackS));
      if ((0 == (3 & arg2_598X))) {
        if ((9 == (255 & (SvalS)))) {
          len_599X = ((arg2_598X)>>2);
          init_600X = ((((SvalS))>>8));
          size_601X = 1 + ((((4 + len_599X))>>2));
          if ((len_599X < 0)) {
            goto L26308;}
          else {
            if ((4194305 < size_601X)) {
              goto L26308;}
            else {
              v_602X = AVAILABLEp(size_601X);
              if (v_602X) {
                goto L26400;}
              else {
                collect_saving_temps(1, 1, &temp1_603X);
                v_604X = AVAILABLEp(size_601X);
                if (v_604X) {
                  goto L26400;}
                else {
                  push_exception_continuationB(8, 1);
                  *((long *) (SstackS)) = (((len_599X)<<2));
                  SstackS = ((SstackS) + -4);
                  *((long *) (SstackS)) = (9 + ((((init_600X))<<8)));
                  SstackS = ((SstackS) + -4);
                  arg0K0 = 2;
                  goto L16665;}}}}}
        else {
          goto L32935;}}
      else {
        goto L32935;}}
      break;
    case 106 : {
      obj_605X = SvalS;
      if ((3 == (3 & obj_605X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_605X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          goto L36289;}}
      else {
        goto L36289;}}
      break;
    case 107 : {
      SstackS = ((SstackS) + 4);
      arg2_606X = *((long *) (SstackS));
      if ((3 == (3 & arg2_606X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_606X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_607X = (((SvalS))>>2);
            len_608X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_606X))) + -4))))>>8));
            if ((index_607X < 0)) {
              goto L32998;}
            else {
              if ((index_607X < len_608X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_606X))) + index_607X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L18438;}
              else {
                goto L32998;}}}
          else {
            goto L35914;}}
        else {
          goto L35914;}}
      else {
        goto L35914;}}
      break;
    case 108 : {
      SstackS = ((SstackS) + 4);
      arg2_609X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_610X = *((long *) (SstackS));
      if ((3 == (3 & arg3_610X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg3_610X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_609X))) {
            if ((9 == (255 & (SvalS)))) {
              index_611X = ((arg2_609X)>>2);
              Kchar_612X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_610X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_610X))) + -4)))))) {
                  len_613X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_610X))) + -4))))>>8));
                  if ((index_611X < 0)) {
                    goto L29957;}
                  else {
                    if ((index_611X < len_613X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_610X))) + index_611X)) = (Kchar_612X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L18438;}
                    else {
                      goto L29957;}}}
                else {
                  goto L29936;}}
              else {
                goto L29936;}}
            else {
              goto L34356;}}
          else {
            goto L34356;}}
        else {
          goto L34356;}}
      else {
        goto L34356;}}
      break;
    case 109 : {
      v_614X = AVAILABLEp(3);
      if (v_614X) {
        goto L24478;}
      else {
        collect_saving_temps(1, 1, &temp1_615X);
        v_616X = AVAILABLEp(3);
        if (v_616X) {
          goto L24478;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L24478;}}}
      break;
    case 110 : {
      obj_617X = SvalS;
      if ((3 == (3 & obj_617X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_617X))) + -4))))>>2))))) {
          x_618X = SvalS;
          descriptor_619X = *((long *) ((((char *) (-3 + x_618X))) + 4));
          if ((17 == (255 & descriptor_619X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_618X))) + 4))))) {
              arg0K0 = 5;
              goto L33976;}
            else {
              arg0K0 = 1;
              goto L33976;}}
          else {
            arg0K0 = 5;
            goto L33976;}}
        else {
          goto L33957;}}
      else {
        goto L33957;}}
      break;
    case 111 : {
      SstackS = ((SstackS) + 4);
      arg2_620X = *((long *) (SstackS));
      if ((3 == (3 & arg2_620X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_620X))) + -4))))>>2))))) {
          x_621X = SvalS;
          if ((1 == x_621X)) {
            goto L29544;}
          else {
            if ((5 == x_621X)) {
              goto L29544;}
            else {
              goto L29551;}}}
        else {
          goto L29551;}}
      else {
        goto L29551;}}
      break;
    case 112 : {
      x_622X = SvalS;
      if ((3 == (3 & x_622X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_622X))) + -4)))))) {
          arg0K0 = 1;
          goto L38498;}
        else {
          arg0K0 = 5;
          goto L38498;}}
      else {
        arg0K0 = 5;
        goto L38498;}}
      break;
    case 113 : {
      x_623X = SvalS;
      if ((3 == (3 & x_623X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_623X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_623X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_623X))) + -4))));
          goto L33080;}
        else {
          goto L33080;}}
      else {
        goto L33080;}}
      break;
    case 114 : {
      v_624X = AVAILABLEp(72);
      if (v_624X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L33131;}
      else {
        collect_saving_temps(1, 1, &temp1_625X);
        v_626X = AVAILABLEp(72);
        if (v_626X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L33131;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L33131;}}}
      break;
    case 115 : {
      v_627X = AVAILABLEp(66);
      if (v_627X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L34474;}
      else {
        collect_saving_temps(1, 1, &temp1_628X);
        v_629X = AVAILABLEp(66);
        if (v_629X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L34474;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L34474;}}}
      break;
    case 116 : {
      v_630X = AVAILABLEp(66);
      if (v_630X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L37343;}
      else {
        collect_saving_temps(1, 1, &temp1_631X);
        v_632X = AVAILABLEp(66);
        if (v_632X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L37343;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L37343;}}}
      break;
    case 117 : {
      v_633X = AVAILABLEp(66);
      if (v_633X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L37603;}
      else {
        collect_saving_temps(1, 1, &temp1_634X);
        v_635X = AVAILABLEp(66);
        if (v_635X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L37603;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L37603;}}}
      break;
    case 118 : {
      obj_636X = SvalS;
      if ((3 == (3 & obj_636X))) {
        if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_636X))) + -4))))>>2))))) {
          channel_637X = SvalS;
          head_638X = Spending_channels_headS;
          if ((1 == head_638X)) {
            addr_639X = (((char *) (-3 + channel_637X))) + 16;
            S48_WRITE_BARRIER(channel_637X, addr_639X, 1);
            *((long *) addr_639X) = 1;
            n_640X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_637X))) + 8))))>>2)));
            arg0K0 = (((n_640X)<<2));
            goto L33202;}
          else {
            if ((channel_637X == head_638X)) {
              channel_641X = Spending_channels_headS;
              next_642X = *((long *) ((((char *) (-3 + channel_641X))) + 12));
              Spending_channels_headS = next_642X;
              addr_643X = (((char *) (-3 + channel_641X))) + 12;
              S48_WRITE_BARRIER(channel_641X, addr_643X, 1);
              *((long *) addr_643X) = 1;
              if ((1 == next_642X)) {
                Spending_channels_tailS = 1;
                goto L8173;}
              else {
                goto L8173;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_638X))) + 12)));
              arg0K1 = head_638X;
              goto L8179;}}}
        else {
          goto L33185;}}
      else {
        goto L33185;}}
      break;
    case 119 : {
      space_644X = 3 * (Snumber_of_channelsS);
      v_645X = AVAILABLEp(space_644X);
      if (v_645X) {
        goto L14777;}
      else {
        collect_saving_temps(1, 1, &temp1_646X);
        v_647X = AVAILABLEp(space_644X);
        if (v_647X) {
          goto L14777;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L14777;}}}
      break;
    case 120 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 121 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 122 : {
      x_648X = SvalS;
      push_exception_continuationB(16, 1);
      *((long *) (SstackS)) = x_648X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
      break;
    case 123 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 124 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 125 : {
      v_649X = AVAILABLEp(66);
      if (v_649X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L34575;}
      else {
        collect_saving_temps(1, 1, &temp1_650X);
        v_651X = AVAILABLEp(66);
        if (v_651X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L34575;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L34575;}}}
      break;
    case 126 : {
      SvalS = 13;
      collect_saving_temps(0, 0, &v_652X);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 127 : {
      obj_653X = SvalS;
      if ((3 == (3 & obj_653X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_653X))) + -4))))>>2))))) {
          x_654X = SvalS;
          n_655X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_654X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L35407;}
        else {
          goto L35384;}}
      else {
        goto L35384;}}
      break;
    case 128 : {
      v_656X = AVAILABLEp(6);
      if (v_656X) {
        goto L38564;}
      else {
        collect_saving_temps(1, 1, &temp1_657X);
        v_658X = AVAILABLEp(6);
        if (v_658X) {
          goto L38564;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L38564;}}}
      break;
    case 129 : {
      SstackS = ((SstackS) + 4);
      arg2_659X = *((long *) (SstackS));
      if ((0 == (3 & arg2_659X))) {
        key_660X = ((arg2_659X)>>2);
        other_661X = SvalS;
        if ((5 == key_660X)) {
          SvalS = (-4 & other_661X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          if ((0 == key_660X)) {
            x_662X = s48_available();
            SvalS = (((x_662X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L18438;}
          else {
            if ((1 == key_660X)) {
              bytes_663X = s48_heap_size();
              SvalS = (-4 & (3 + bytes_663X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L18438;}
            else {
              if ((2 == key_660X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L18438;}
              else {
                if ((3 == key_660X)) {
                  x_664X = s48_gc_count();
                  SvalS = (((x_664X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L18438;}
                else {
                  if ((4 == key_660X)) {
                    push_exception_continuationB(15, 1);
                    *((long *) (SstackS)) = (((key_660X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_661X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L16665;}
                  else {
                    push_exception_continuationB(18, 1);
                    *((long *) (SstackS)) = (((key_660X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_661X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L16665;}}}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_659X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 130 : {
      if ((0 == (3 & (SvalS)))) {
        type_665X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L33293;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 131 : {
      x_666X = SvalS;
      arg2K0 = 1;
      arg0K1 = x_666X;
      goto L38614;}
      break;
    case 132 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 133 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 134 : {
      SvalS = (Ssession_dataS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 135 : {
      Ssession_dataS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 136 : {
      obj_667X = SvalS;
      if ((3 == (3 & obj_667X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_667X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 160)) {
            goto L20570;}
          else {
            temp_668X = Sexception_handlersS;
            Sexception_handlersS = (SvalS);
            SvalS = temp_668X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L18438;}}
        else {
          goto L20570;}}
      else {
        goto L20570;}}
      break;
    case 137 : {
      SstackS = ((SstackS) + 4);
      p_669X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      template_670X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      p_671X = *((long *) (SstackS));
      StemplateS = template_670X;
      Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_670X)))))))) + ((((((((((p_669X)>>2)) + (((p_671X)>>2))))<<2)))>>2)));
      arg1K0 = (Scode_pointerS);
      goto L18438;}
      break;
    case 138 : {
      obj_672X = SvalS;
      if ((3 == (3 & obj_672X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_672X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 5)) {
            goto L19494;}
          else {
            temp_673X = Sinterrupt_handlersS;
            Sinterrupt_handlersS = (SvalS);
            SvalS = temp_673X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L18438;}}
        else {
          goto L19494;}}
      else {
        goto L19494;}}
      break;
    case 139 : {
      old_674X = Senabled_interruptsS;
      p_675X = SvalS;
      Senabled_interruptsS = (((p_675X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L38655;}
        else {
          goto L38655;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L38655;}}
      break;
    case 140 : {
      SstackS = ((SstackS) + 4);
      p_676X = *((long *) (SstackS));
      Senabled_interruptsS = (((p_676X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L18452;}
        else {
          goto L18452;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L18452;}}
      break;
    case 141 : {
      if ((0 == (3 & (SvalS)))) {
        p_677X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          s48_Spending_interruptPS = 0;
          if ((s48_Spending_eventsPS)) {
            s48_Spending_interruptPS = 1;
            goto L37163;}
          else {
            goto L37163;}}
        else {
          s48_Spending_interruptPS = 1;
          goto L37163;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}}
      break;
    case 142 : {
      SstackS = ((SstackS) + 4);
      arg2_678X = *((long *) (SstackS));
      if ((0 == (3 & arg2_678X))) {
        x_679X = SvalS;
        if ((1 == x_679X)) {
          goto L34103;}
        else {
          if ((5 == x_679X)) {
            goto L34103;}
          else {
            goto L34108;}}}
      else {
        goto L34108;}}
      break;
    case 143 : {
      SstackS = ((SstackS) + 4);
      nargs_680X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      p_681X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      rest_list_682X = *((long *) (SstackS));
      if ((12 < nargs_680X)) {
        push_exception_continuationB(20, (1 + (*((long *) ((SstackS) + (-4 & p_681X))))));
        *((long *) (SstackS)) = nargs_680X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}
      else {
        arg0K0 = rest_list_682X;
        goto L22787;}}
      break;
    case 144 : {
      v_683X = AVAILABLEp(5);
      if (v_683X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L33402;}
      else {
        collect_saving_temps(1, 1, &temp1_684X);
        v_685X = AVAILABLEp(5);
        if (v_685X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L33402;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L33402;}}}
      break;
    case 146 : {
      SstackS = ((SstackS) + 4);
      arg2_686X = *((long *) (SstackS));
      if ((3 == (3 & arg2_686X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_686X))) + -4))))>>2))))) {
          x_687X = SvalS;
          if ((1 == x_687X)) {
            goto L23905;}
          else {
            if ((5 == x_687X)) {
              goto L23905;}
            else {
              goto L23910;}}}
        else {
          goto L23910;}}
      else {
        goto L23910;}}
      break;
    case 147 : {
      SstackS = ((SstackS) + 4);
      arg2_688X = *((long *) (SstackS));
      if ((0 == (3 & arg2_688X))) {
        option_689X = ((arg2_688X)>>2);
        other_690X = SvalS;
        if ((2 == option_689X)) {
          x_691X = CHEAP_TIME();
          SvalS = (((x_691X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          if ((0 == option_689X)) {
            seconds_692X = s48_run_time(&mseconds_693X);
            arg0K0 = option_689X;
            arg0K1 = seconds_692X;
            arg0K2 = mseconds_693X;
            goto L35318;}
          else {
            if ((1 == option_689X)) {
              seconds_694X = s48_real_time(&mseconds_695X);
              arg0K0 = option_689X;
              arg0K1 = seconds_694X;
              arg0K2 = mseconds_695X;
              goto L35318;}
            else {
              push_exception_continuationB(18, 1);
              *((long *) (SstackS)) = (((option_689X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = other_690X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L16665;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_688X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 148 : {
      SstackS = ((SstackS) + 4);
      arg2_696X = *((long *) (SstackS));
      if ((0 == (3 & arg2_696X))) {
        key_697X = ((arg2_696X)>>2);
        value_698X = SvalS;
        status_699X = s48_extended_vm(key_697X, value_698X);
        if ((0 == status_699X)) {
          SvalS = (s48_Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          if ((1 == status_699X)) {
            push_exception_continuationB(23, 1);
            *((long *) (SstackS)) = (((key_697X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_698X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L16665;}
          else {
            push_exception_continuationB(24, 1);
            *((long *) (SstackS)) = (((key_697X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_698X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L16665;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_696X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
      break;
    case 149 : {
      SstackS = ((SstackS) + 4);
      arg2_700X = *((long *) (SstackS));
      x_701X = SvalS;
      Senabled_interruptsS = -1;
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L38715;}
        else {
          goto L38715;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L38715;}}
      break;
    case 150 : {
      SstackS = ((SstackS) + 4);
      arg2_702X = *((long *) (SstackS));
      if ((3 == (3 & arg2_702X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_702X))) + -4))))>>2))))) {
          obj_703X = SvalS;
          if ((3 == (3 & obj_703X))) {
            if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_703X))) + -4))))>>2))))) {
              x_704X = SvalS;
              len_705X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_702X))) + -4))))>>8);
              if ((len_705X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_704X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_704X))), (void *)(((char *) (-3 + arg2_702X))),len_705X)))) {
                  arg0K0 = 5;
                  goto L27842;}
                else {
                  arg0K0 = 1;
                  goto L27842;}}
              else {
                arg0K0 = 1;
                goto L27842;}}
            else {
              goto L27809;}}
          else {
            goto L27809;}}
        else {
          goto L27809;}}
      else {
        goto L27809;}}
      break;
    case 151 : {
      space_706X = 1 + ((((4 + ((((SvalS))>>2))))>>2));
      v_707X = AVAILABLEp(space_706X);
      if (v_707X) {
        goto L33611;}
      else {
        collect_saving_temps(1, 1, &temp1_708X);
        v_709X = AVAILABLEp(space_706X);
        if (v_709X) {
          goto L33611;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L33611;}}}
      break;
    case 152 : {
      SstackS = ((SstackS) + 4);
      thing_710X = *((long *) (SstackS));
      list_711X = SvalS;
      arg0K0 = list_711X;
      arg0K1 = list_711X;
      arg2K2 = 1;
      goto L29199;}
      break;
    case 153 : {
      SstackS = ((SstackS) + 4);
      arg2_712X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_713X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        index_714X = (((SvalS))>>2);
        if ((3 == (3 & arg3_713X))) {
          if ((8 == (31 & ((((*((long *) ((((char *) (-3 + arg3_713X))) + -4))))>>2))))) {
            if ((arg2_712X == (*((long *) (((char *) (-3 + arg3_713X))))))) {
              len_715X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_713X))) + -4))))>>8))))>>2);
              if ((index_714X < 0)) {
                goto L27515;}
              else {
                if ((index_714X < len_715X)) {
                  SvalS = (*((long *) ((((char *) (-3 + arg3_713X))) + (((index_714X)<<2)))));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L18438;}
                else {
                  goto L27515;}}}
            else {
              goto L27535;}}
          else {
            goto L27535;}}
        else {
          goto L27535;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg3_713X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_712X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L16665;}}
      break;
    case 154 : {
      SstackS = ((SstackS) + 4);
      arg2_716X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_717X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_718X = *((long *) (SstackS));
      if ((0 == (3 & arg2_716X))) {
        index_719X = ((arg2_716X)>>2);
        value_720X = SvalS;
        if ((3 == (3 & arg4_718X))) {
          if ((8 == (31 & ((((*((long *) ((((char *) (-3 + arg4_718X))) + -4))))>>2))))) {
            if ((arg3_717X == (*((long *) (((char *) (-3 + arg4_718X))))))) {
              if ((3 == (3 & arg4_718X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_718X))) + -4)))))) {
                  len_721X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_718X))) + -4))))>>8))))>>2);
                  if ((index_719X < 0)) {
                    goto L30704;}
                  else {
                    if ((index_719X < len_721X)) {
                      addr_722X = (((char *) (-3 + arg4_718X))) + (((index_719X)<<2));
                      S48_WRITE_BARRIER(arg4_718X, addr_722X, value_720X);
                      *((long *) addr_722X) = value_720X;
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L18438;}
                    else {
                      goto L30704;}}}
                else {
                  goto L30726;}}
              else {
                goto L30726;}}
            else {
              goto L30726;}}
          else {
            goto L30726;}}
        else {
          goto L30726;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg4_718X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_717X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_716X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 4;
        goto L16665;}}
      break;
    case 155 : {
      SstackS = ((SstackS) + 4);
      arg2_723X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_724X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_725X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_726X = *((long *) (SstackS));
      if ((0 == (3 & ((arg4_725X | arg2_723X) | (SvalS))))) {
        from_index_727X = ((arg4_725X)>>2);
        to_index_728X = ((arg2_723X)>>2);
        count_729X = (((SvalS))>>2);
        if ((3 == (3 & arg5_726X))) {
          if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_726X))) + -4))))>>2))))) {
            goto L30955;}
          else {
            goto L30950;}}
        else {
          goto L30950;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg5_726X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_725X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_724X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_723X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 5;
        goto L16665;}}
      break;
    case 156 : {
      v_730X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_730X)) {
        arg0K0 = (SvalS);
        goto L23037;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_731X = get_current_port0_return_value;
        arg0K0 = v_731X;
        goto L23037;}}
      break;
    case 157 : {
      v_732X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_732X)) {
        arg0K0 = (SvalS);
        goto L23257;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_733X = get_current_port0_return_value;
        arg0K0 = v_733X;
        goto L23257;}}
      break;
    case 158 : {
      v_734X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_734X)) {
        v_735X = SvalS;
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = v_735X;
        goto L23470;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_736X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_736X;
        goto L23470;}}
      break;
    case 159 : {
      x_737X = SvalS;
      out_738X = stderr;
      arg0K0 = x_737X;
      goto L29452;}
      break;
  }}
 L16665: {
  nargs_739X = arg0K0;
  opcode_740X = (((*((long *) ((SstackS) + (8 + (((nargs_739X)<<2)))))))>>2);
  obj_741X = Sexception_handlersS;
  if ((3 == (3 & obj_741X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_741X))) + -4))))>>2))))) {
      goto L16724;}
    else {
      goto L16797;}}
  else {
    goto L16797;}}
 L23824: {
  env_742X = arg0K0;
  i_743X = arg0K1;
  if ((0 == i_743X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_742X))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 3);
      arg0K0 = 0;
      goto L16665;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L18438;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_742X)))));
    arg0K1 = (-1 + i_743X);
    goto L23824;}}
 L20247: {
  env_744X = arg0K0;
  i_745X = arg0K1;
  if ((0 == i_745X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_744X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 5);
      arg0K0 = 0;
      goto L16665;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L18438;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_744X)))));
    arg0K1 = (-1 + i_745X);
    goto L20247;}}
 L20185: {
  env_746X = arg0K0;
  i_747X = arg0K1;
  if ((0 == i_747X)) {
    index_748X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
    value_749X = SvalS;
    addr_750X = (((char *) (-3 + env_746X))) + (((index_748X)<<2));
    S48_WRITE_BARRIER(env_746X, addr_750X, value_749X);
    *((long *) addr_750X) = value_749X;
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 5);
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_746X)))));
    arg0K1 = (-1 + i_747X);
    goto L20185;}}
 L20964: {
  space_751X = arg0K0;
  v_752X = AVAILABLEp(space_751X);
  if (v_752X) {
    arg2K0 = 1;
    arg0K1 = 0;
    goto L21038;}
  else {
    collect_saving_temps(1, 1, &temp1_753X);
    v_754X = AVAILABLEp(space_751X);
    if (v_754X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L21038;}
    else {
      arg2K0 = 0;
      arg0K1 = 0;
      goto L21038;}}}
 L20971: {
  env_755X = arg0K0;
  v_756X = AVAILABLEp(3);
  if (v_756X) {
    arg2K0 = 1;
    arg0K1 = env_755X;
    goto L20996;}
  else {
    temp0_757X = collect_saving_temps(env_755X, 1, &temp1_758X);
    v_759X = AVAILABLEp(3);
    if (v_759X) {
      arg2K0 = 1;
      arg0K1 = temp0_757X;
      goto L20996;}
    else {
      arg2K0 = 0;
      arg0K1 = temp0_757X;
      goto L20996;}}}
 L20766: {
  len_760X = ((total_count_393X)<<2);
  addr_761X = ALLOCATE_SPACE(2, (4 + len_760X));
  *((long *) addr_761X) = (10 + (((len_760X)<<8)));
  new_env_762X = 3 + (((long) (addr_761X + 4)));
  v_763X = *((unsigned char *) ((Scode_pointerS) + 1));
  if ((0 == v_763X)) {
    arg0K0 = 0;
    goto L20780;}
  else {
    value_764X = SvalS;
    addr_765X = ((char *) (-3 + new_env_762X));
    S48_WRITE_BARRIER(new_env_762X, addr_765X, value_764X);
    *((long *) addr_765X) = value_764X;
    arg0K0 = 1;
    goto L20780;}}
 L20692: {
  okayP_766X = arg2K0;
  key_767X = arg0K1;
  if (okayP_766X) {
    arg0K0 = key_767X;
    goto L20680;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_767X;
    goto L20680;}}
 L19847: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L19882: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_402X;
  ScontS = (Sbottom_of_stackS);
  goto L19847;}
 L19848: {
  if ((1 == cont_402X)) {
    if ((0 == (3 & (SvalS)))) {
      s48_Scallback_return_stack_blockS = 1;
      return ((((SvalS))>>2));}
    else {
      goto L19863;}}
  else {
    goto L19863;}}
 L19208: {
  okayP_768X = arg2K0;
  key_769X = arg0K1;
  if (okayP_768X) {
    arg0K0 = key_769X;
    goto L19163;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_769X;
    goto L19163;}}
 L19120: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 0;
      goto check_events;
     check_events_return_0:
      v_770X = check_events0_return_value;
      if (v_770X) {
        arg0K0 = stack_arg_count_403X;
        goto L16894;}
      else {
        goto L19124;}}
    else {
      arg0K0 = stack_arg_count_403X;
      goto L16894;}}
  else {
    goto L19124;}}
 L16066: {
  stack_arg_count_771X = arg0K0;
  code_772X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_772X))) + 1)));
  arg0K1 = 64;
  goto L16080;}
 L17161: {
  exception_773X = arg0K0;
  stack_arg_count_774X = arg0K1;
  list_args_775X = arg0K2;
  list_arg_count_776X = arg0K3;
  if (((StemplateS) == (SvalS))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      return v_777X;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      return v_778X;}}
  else {
    merged_arg0K0 = list_args_775X;
    merged_arg0K1 = list_arg_count_776X;
    copy_listS_return_tag = 0;
    goto copy_listS;
   copy_listS_return_0:
    v_779X = copy_listS0_return_value;
    merged_arg0K0 = v_779X;
    merged_arg0K1 = stack_arg_count_774X;
    pop_args_GlistS_return_tag = 1;
    goto pop_args_GlistS;
   pop_args_GlistS_return_1:
    args_780X = pop_args_GlistS0_return_value;
    push_exception_continuationB(exception_773X, 0);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_780X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}}
 L17628: {
  loc_781X = arg1K0;
  arg_782X = arg1K1;
  if ((top_of_args_414X < arg_782X)) {
    *((long *) loc_781X) = (*((long *) arg_782X));
    arg1K0 = (loc_781X + -4);
    arg1K1 = (arg_782X + -4);
    goto L17628;}
  else {
    SstackS = loc_781X;
    obj_783X = SvalS;
    if ((3 == (3 & obj_783X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_783X))) + -4))))>>2))))) {
        arg0K0 = nargs_413X;
        goto L16066;}
      else {
        arg0K0 = 3;
        arg0K1 = nargs_413X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17161;}}
    else {
      arg0K0 = 3;
      arg0K1 = nargs_413X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L17161;}}}
 L17666: {
  stack_arg_count_784X = arg0K0;
  list_args_785X = arg0K1;
  list_arg_count_786X = arg0K2;
  if ((0 == list_arg_count_786X)) {
    obj_787X = SvalS;
    if ((3 == (3 & obj_787X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_787X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_784X;
        goto L16066;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_784X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17161;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_784X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L17161;}}
  else {
    obj_788X = SvalS;
    if ((3 == (3 & obj_788X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_788X))) + -4))))>>2))))) {
        code_789X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
        total_arg_count_790X = stack_arg_count_784X + list_arg_count_786X;
        arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_789X))) + 1)));
        arg0K1 = 64;
        goto L16357;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_784X;
        arg0K2 = list_args_785X;
        arg0K3 = list_arg_count_786X;
        goto L17161;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_784X;
      arg0K2 = list_args_785X;
      arg0K3 = list_arg_count_786X;
      goto L17161;}}}
 L11273: {
  list_args_791X = arg0K0;
  stack_nargs_792X = arg0K1;
  merged_arg0K0 = list_args_791X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_793X = okay_argument_list0_return_value;
  list_arg_count_794X = okay_argument_list1_return_value;
  if (okayP_793X) {
    SvalS = proc_421X;
    arg0K0 = stack_nargs_792X;
    arg0K1 = list_args_791X;
    arg0K2 = list_arg_count_794X;
    goto L17666;}
  else {
    *((long *) (SstackS)) = list_args_791X;
    SstackS = ((SstackS) + -4);
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_792X);
    pop_args_GlistS_return_tag = 2;
    goto pop_args_GlistS;
   pop_args_GlistS_return_2:
    args_795X = pop_args_GlistS0_return_value;
    SstackS = ((SstackS) + 4);
    push_exception_continuationB(5, 0);
    *((long *) (SstackS)) = proc_421X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_795X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}}
 L11314: {
  list_796X = arg0K0;
  follower_797X = arg0K1;
  if ((25 == list_796X)) {
    list_args_798X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_797X))) + 4)))))));
    addr_799X = (((char *) (-3 + follower_797X))) + 4;
    S48_WRITE_BARRIER(follower_797X, addr_799X, list_args_798X);
    *((long *) addr_799X) = list_args_798X;
    arg0K0 = rest_list_422X;
    arg0K1 = (-1 + stack_nargs_420X);
    goto L11273;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_796X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_797X))) + 4)));
    goto L11314;}}
 L17571: {
  obj_800X = SvalS;
  if ((3 == (3 & obj_800X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_800X))) + -4))))>>2))))) {
      arg0K0 = 0;
      goto L16066;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L17161;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L17161;}}
 L17578: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_423X;
  ScontS = (Sbottom_of_stackS);
  goto L17571;}
 L20427: {
  stack_nargs_801X = arg0K0;
  list_args_802X = arg0K1;
  list_arg_count_803X = arg0K2;
  if ((1 == stack_nargs_801X)) {
    SstackS = ((SstackS) + 4);
    SvalS = (*((long *) (SstackS)));
    pop_continuationB_return_tag = 1;
    goto pop_continuationB;
   pop_continuationB_return_1:
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    if (((ScontS) == (Sbottom_of_stackS))) {
      arg0K0 = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
      goto L20438;}
    else {
      arg0K0 = (ScontS);
      goto L20438;}}}
 L18944: {
  okayP_804X = arg2K0;
  key_805X = arg0K1;
  if (okayP_804X) {
    arg0K0 = key_805X;
    goto L18932;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_805X;
    goto L18932;}}
 L18817: {
  tem_806X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  StemplateS = tem_806X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_806X)))))))) + 2);
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 1;
      goto check_events;
     check_events_return_1:
      v_807X = check_events0_return_value;
      if (v_807X) {
        goto L18826;}
      else {
        goto L18829;}}
    else {
      goto L18826;}}
  else {
    goto L18829;}}
 L19000: {
  okayP_808X = arg2K0;
  key_809X = arg0K1;
  if (okayP_808X) {
    arg0K0 = key_809X;
    goto L18987;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_809X;
    goto L18987;}}
 L18862: {
  push_exception_continuationB(5, 4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L18718: {
  arg0K0 = (2 + (((max_442X)<<1)));
  goto L18720;}
 L18720: {
  offset_810X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_810X);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L19750: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L18616: {
  delta_811X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_811X);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L19641: {
  delta_812X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_812X);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L38026: {
  val_813X = arg0K0;
  SvalS = val_813X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L38038: {
  val_814X = arg0K0;
  SvalS = val_814X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L10622: {
  if ((3 == (3 & x_456X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_456X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L38038;}
    else {
      goto L10628;}}
  else {
    goto L10628;}}
 L28391: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28392: {
  if ((3 == (3 & n_457X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + n_457X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_457X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
    else {
      goto L28396;}}
  else {
    goto L28396;}}
 L28548: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28523: {
  if ((3 == (3 & n_458X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + n_458X))) + -4))))>>2))))) {
      goto L28548;}
    else {
      goto L28537;}}
  else {
    goto L28537;}}
 L28747: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28722: {
  if ((3 == (3 & n_459X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + n_459X))) + -4))))>>2))))) {
      goto L28747;}
    else {
      goto L28736;}}
  else {
    goto L28736;}}
 L28946: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28921: {
  if ((3 == (3 & n_460X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + n_460X))) + -4))))>>2))))) {
      goto L28946;}
    else {
      goto L28935;}}
  else {
    goto L28935;}}
 L6775: {
  a_815X = arg0K0;
  if ((b_470X < 0)) {
    arg0K0 = (0 - b_470X);
    goto L6779;}
  else {
    arg0K0 = b_470X;
    goto L6779;}}
 L7056: {
  a_816X = arg0K0;
  if ((b_477X < 0)) {
    arg0K0 = (0 - b_477X);
    goto L7060;}
  else {
    arg0K0 = b_477X;
    goto L7060;}}
 L31744: {
  val_817X = arg0K0;
  SvalS = val_817X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L31822: {
  val_818X = arg0K0;
  SvalS = val_818X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L31900: {
  val_819X = arg0K0;
  SvalS = val_819X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L31978: {
  val_820X = arg0K0;
  SvalS = val_820X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L32056: {
  val_821X = arg0K0;
  SvalS = val_821X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L7233: {
  a_822X = arg0K0;
  if ((b_491X < 0)) {
    arg0K0 = (0 - b_491X);
    goto L7237;}
  else {
    arg0K0 = b_491X;
    goto L7237;}}
 L25338: {
  a_823X = arg0K0;
  n_824X = ((y_493X)>>2);
  if ((n_824X < 0)) {
    arg0K0 = (0 - n_824X);
    goto L25340;}
  else {
    arg0K0 = n_824X;
    goto L25340;}}
 L38295: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_508X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L25624: {
  r_825X = arg0K0;
  if ((536870911 < r_825X)) {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = n_509X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 1;
    goto L16665;}
  else {
    SvalS = (((r_825X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L25903: {
  SvalS = (((result_530X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L38383: {
  val_826X = arg0K0;
  SvalS = val_826X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28166: {
  val_827X = arg0K0;
  SvalS = val_827X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28139: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_533X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L28076: {
  val_828X = arg0K0;
  SvalS = val_828X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28049: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_535X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L32748: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((x_537X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L38412: {
  val_829X = arg0K0;
  SvalS = val_829X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L38426: {
  value_830X = arg0K0;
  SvalS = value_830X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L21229: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = stob_541X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_542X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L21301: {
  type_831X = *((unsigned char *) ((Scode_pointerS) + 2));
  len_832X = ((len_543X)<<2);
  addr_833X = ALLOCATE_SPACE(type_831X, (4 + len_832X));
  *((long *) addr_833X) = (2 + (((((((len_832X)<<6)) + type_831X))<<2)));
  new_834X = 3 + (((long) (addr_833X + 4)));
  if ((len_543X < 1)) {
    goto L21336;}
  else {
    *((long *) ((((char *) (-3 + new_834X))) + (-4 + (((len_543X)<<2))))) = (SvalS);
    arg0K0 = (-2 + len_543X);
    goto L21320;}}
 L21483: {
  type_835X = *((unsigned char *) ((Scode_pointerS) + 1));
  len_836X = ((len_548X)<<2);
  addr_837X = ALLOCATE_SPACE(type_835X, (4 + len_836X));
  *((long *) addr_837X) = (2 + (((((((len_836X)<<6)) + type_835X))<<2)));
  new_838X = 3 + (((long) (addr_837X + 4)));
  SstackS = ((SstackS) + 4);
  stack_nargs_839X = (((*((long *) (SstackS))))>>2);
  SstackS = ((SstackS) + 4);
  rest_list_840X = *((long *) (SstackS));
  arg0K0 = (-1 + stack_nargs_839X);
  goto L21505;}
 L21693: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_553X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_555X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_554X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L21796: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_556X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_559X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_558X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_557X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L16665;}
 L21981: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = (((type_563X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_564X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = init_562X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L21999: {
  okayP_841X = arg2K0;
  init_842X = arg0K1;
  if (okayP_841X) {
    len_843X = ((len_564X)<<2);
    addr_844X = ALLOCATE_SPACE(type_563X, (4 + len_843X));
    *((long *) addr_844X) = (2 + (((((((len_843X)<<6)) + type_563X))<<2)));
    value_845X = 3 + (((long) (addr_844X + 4)));
    arg0K0 = (-1 + len_564X);
    goto L22028;}
  else {
    push_exception_continuationB(8, 2);
    *((long *) (SstackS)) = (((type_563X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_564X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = init_842X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L16665;}}
 L22277: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = stob_570X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_572X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_571X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L22254: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = stob_570X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_572X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_571X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L22517: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = stob_576X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_578X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_575X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_577X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L16665;}
 L22492: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = stob_576X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_578X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_575X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_577X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L16665;}
 L26074: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_583X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_584X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L26166: {
  addr_846X = ALLOCATE_SPACE(17, (4 + len_583X));
  *((long *) addr_846X) = (70 + (((len_583X)<<8)));
  vector_847X = 3 + (((long) (addr_846X + 4)));
  arg0K0 = (-1 + len_583X);
  goto L26115;}
 L36239: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L32854: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_590X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_591X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L35818: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_590X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L29802: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_594X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_595X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_596X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L29781: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_594X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_595X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_596X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L34230: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_594X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_593X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L26308: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_599X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_600X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L26400: {
  len_848X = 1 + len_599X;
  addr_849X = ALLOCATE_SPACE(16, (4 + len_848X));
  *((long *) addr_849X) = (66 + (((len_848X)<<8)));
  string_850X = 3 + (((long) (addr_849X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_850X))) + len_599X)) = 0;
  arg0K0 = (-1 + len_599X);
  goto L26349;}
 L32935: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_598X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L36289: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L32998: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_606X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_607X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L35914: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_606X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L29957: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_610X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_611X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_612X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L29936: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_610X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_611X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_612X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L34356: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_610X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_609X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L24478: {
  obj_851X = SvalS;
  if ((3 == (3 & obj_851X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_851X))) + -4))))>>2))))) {
      table_852X = Sthe_symbol_tableS;
      string_853X = SvalS;
      n_854X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_853X))) + -4))))>>8));
      arg0K0 = 0;
      arg0K1 = 0;
      goto L13333;}
    else {
      goto L24488;}}
  else {
    goto L24488;}}
 L33976: {
  val_855X = arg0K0;
  SvalS = val_855X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L33957: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L29544: {
  if ((1 == (SvalS))) {
    addr_856X = (((char *) (-3 + arg2_620X))) + 4;
    S48_WRITE_BARRIER(arg2_620X, addr_856X, 273);
    *((long *) addr_856X) = 273;
    goto L29550;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_620X))) + 4)))))) {
      addr_857X = (((char *) (-3 + arg2_620X))) + 4;
      S48_WRITE_BARRIER(arg2_620X, addr_857X, 529);
      *((long *) addr_857X) = 529;
      goto L29550;}
    else {
      goto L29550;}}}
 L29551: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_620X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L38498: {
  val_858X = arg0K0;
  SvalS = val_858X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L33080: {
  SvalS = x_623X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L33131: {
  okayP_859X = arg2K0;
  key_860X = arg0K1;
  if (okayP_859X) {
    arg0K0 = key_860X;
    goto L33097;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_860X;
    goto L33097;}}
 L34474: {
  okayP_861X = arg2K0;
  key_862X = arg0K1;
  if (okayP_861X) {
    arg0K0 = key_862X;
    goto L34456;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_862X;
    goto L34456;}}
 L37343: {
  okayP_863X = arg2K0;
  key_864X = arg0K1;
  if (okayP_863X) {
    arg0K0 = key_864X;
    goto L37273;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_864X;
    goto L37273;}}
 L37603: {
  okayP_865X = arg2K0;
  key_866X = arg0K1;
  if (okayP_865X) {
    arg0K0 = key_866X;
    goto L37545;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_866X;
    goto L37545;}}
 L33202: {
  val_867X = arg0K0;
  SvalS = val_867X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L8173: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_637X))) + 16)));
  goto L33202;}
 L8179: {
  ch_868X = arg0K0;
  prev_869X = arg0K1;
  if ((1 == ch_868X)) {
    addr_870X = (((char *) (-3 + channel_637X))) + 16;
    S48_WRITE_BARRIER(channel_637X, addr_870X, 1);
    *((long *) addr_870X) = 1;
    n_871X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_637X))) + 8))))>>2)));
    arg0K0 = (((n_871X)<<2));
    goto L33202;}
  else {
    if ((ch_868X == channel_637X)) {
      y_872X = Spending_channels_tailS;
      if ((ch_868X == y_872X)) {
        Spending_channels_tailS = prev_869X;
        goto L8203;}
      else {
        goto L8203;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_868X))) + 12)));
      arg0K1 = ch_868X;
      goto L8179;}}}
 L33185: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L14777: {
  arg0K0 = (-1 + (Snumber_of_channelsS));
  arg0K1 = 25;
  goto L14784;}
 L34575: {
  okayP_873X = arg2K0;
  key_874X = arg0K1;
  if (okayP_873X) {
    arg0K0 = key_874X;
    goto L34529;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_874X;
    goto L34529;}}
 L35407: {
  i_875X = arg0K0;
  h_876X = arg0K1;
  if ((i_875X < n_655X)) {
    arg0K0 = (1 + i_875X);
    arg0K1 = (h_876X + (((*((unsigned char *) ((((char *) (-3 + x_654X))) + i_875X))))));
    goto L35407;}
  else {
    SvalS = (((h_876X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L35384: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L38564: {
  SstackS = ((SstackS) + 4);
  stob_877X = *((long *) (SstackS));
  proc_878X = SvalS;
  if ((3 == (3 & stob_877X))) {
    if ((3 == (3 & proc_878X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_878X))) + -4))))>>2))))) {
        addr_879X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_879X) = 2050;
        x_880X = 3 + (((long) (addr_879X + 4)));
        *((long *) (((char *) (-3 + x_880X)))) = stob_877X;
        *((long *) ((((char *) (-3 + x_880X))) + 4)) = proc_878X;
        b_881X = Sfinalizer_alistS;
        addr_882X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_882X) = 2050;
        x_883X = 3 + (((long) (addr_882X + 4)));
        *((long *) (((char *) (-3 + x_883X)))) = x_880X;
        *((long *) ((((char *) (-3 + x_883X))) + 4)) = b_881X;
        Sfinalizer_alistS = x_883X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        goto L30563;}}
    else {
      goto L30563;}}
  else {
    goto L30563;}}
 L33293: {
  firstP_884X = arg2K0;
  vector_885X = s48_find_all(type_665X);
  if ((1 == vector_885X)) {
    if (firstP_884X) {
      collect_saving_temps(0, 0, &v_886X);
      arg2K0 = 0;
      goto L33293;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = (((type_665X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}}
  else {
    SvalS = vector_885X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L38614: {
  firstP_887X = arg2K0;
  type_888X = arg0K1;
  vector_889X = s48_find_all_records(type_888X);
  if ((1 == vector_889X)) {
    if (firstP_887X) {
      value_890X = collect_saving_temps(type_888X, 0, &v_891X);
      arg2K0 = 0;
      arg0K1 = value_890X;
      goto L38614;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = type_888X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}}
  else {
    SvalS = vector_889X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L20570: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L19494: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L38655: {
  SvalS = (((old_674X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L18452: {
  SstackS = ((SstackS) + 4);
  pc_892X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tem_893X = *((long *) (SstackS));
  StemplateS = tem_893X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_893X)))))))) + (((pc_892X)>>2)));
  SstackS = ((SstackS) + 4);
  SvalS = (*((long *) (SstackS)));
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L37163: {
  x_894X = s48_schedule_alarm_interrupt((((p_677X)>>2)));
  SvalS = (((x_894X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L34103: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L34107;}
  else {
    arg2K0 = 1;
    goto L34107;}}
 L34108: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_678X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L22787: {
  rest_list_895X = arg0K0;
  if ((25 == rest_list_895X)) {
    proc_896X = *((long *) ((SstackS) + (((nargs_680X)<<2))));
    name_897X = *((long *) ((SstackS) + (-4 + (((nargs_680X)<<2)))));
    args_898X = (SstackS) + 4;
    *((long *) (SstackS)) = (10 + (((nargs_680X)<<10)));
    SstackS = ((SstackS) + -4);
    if ((3 == (3 & name_897X))) {
      if ((16 == (31 & ((((*((long *) ((((char *) (-3 + name_897X))) + -4))))>>2))))) {
        if ((3 == (3 & proc_896X))) {
          if ((17 == (31 & ((((*((long *) ((((char *) (-3 + proc_896X))) + -4))))>>2))))) {
            if ((4 == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proc_896X))) + -4))))>>8)))) {
              result_899X = s48_external_call(proc_896X, name_897X, (-2 + nargs_680X), args_898X);
              if ((Sexternal_exceptionPS)) {
                Sexternal_exceptionPS = 0;
                arg0K0 = (Sexternal_exception_nargsS);
                goto L16665;}
              else {
                SvalS = result_899X;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L18438;}}
            else {
              goto L22847;}}
          else {
            goto L22847;}}
        else {
          goto L22847;}}
      else {
        goto L22847;}}
    else {
      goto L22847;}}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + rest_list_895X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_895X))) + 4)));
    goto L22787;}}
 L33402: {
  okayP_900X = arg2K0;
  key_901X = arg0K1;
  if (okayP_900X) {
    arg0K0 = key_901X;
    goto L33368;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_901X;
    goto L33368;}}
 L23905: {
  if ((1 == (SvalS))) {
    arg0K0 = (Sexported_bindingsS);
    goto L23938;}
  else {
    arg0K0 = (Simported_bindingsS);
    goto L23938;}}
 L23910: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_686X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L35318: {
  option_902X = arg0K0;
  seconds_903X = arg0K1;
  mseconds_904X = arg0K2;
  if ((536869 < seconds_903X)) {
    push_exception_continuationB(6, 1);
    *((long *) (SstackS)) = (((option_902X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_903X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_904X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L16665;}
  else {
    SvalS = (((((1000 * seconds_903X) + mseconds_904X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L38715: {
  s48_Scallback_return_stack_blockS = arg2_700X;
  return x_701X;}
 L27842: {
  val_905X = arg0K0;
  SvalS = val_905X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L27809: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_702X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L33611: {
  SstackS = ((SstackS) + 4);
  arg2_906X = *((long *) (SstackS));
  if ((0 == (3 & (SvalS)))) {
    n_907X = (((SvalS))>>2);
    if ((3 == (3 & arg2_906X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_906X))) + -4))))>>2))))) {
        goto L27369;}
      else {
        goto L27321;}}
    else {
      goto L27321;}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_906X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}}
 L29199: {
  list_908X = arg0K0;
  slow_909X = arg0K1;
  move_slowP_910X = arg2K2;
  if ((25 == list_908X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    if ((3 == (3 & list_908X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_908X))) + -4))))>>2))))) {
        head_911X = *((long *) (((char *) (-3 + list_908X))));
        if ((3 == (3 & head_911X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_911X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_911X))))) == thing_710X)) {
              SvalS = head_911X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L18438;}
            else {
              list_912X = *((long *) ((((char *) (-3 + list_908X))) + 4));
              if ((list_912X == slow_909X)) {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = thing_710X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = list_711X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L16665;}
              else {
                if (move_slowP_910X) {
                  arg0K0 = list_912X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_909X))) + 4)));
                  arg2K2 = 0;
                  goto L29199;}
                else {
                  arg0K0 = list_912X;
                  arg0K1 = slow_909X;
                  arg2K2 = 1;
                  goto L29199;}}}}
          else {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = thing_710X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = list_711X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L16665;}}
        else {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = thing_710X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = list_711X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L16665;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = thing_710X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_711X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = thing_710X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = list_711X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}}}
 L27515: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_713X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_712X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_714X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L27535: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_713X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_712X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_714X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L30704: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg4_718X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_717X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_719X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_720X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L16665;}
 L30726: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_718X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_717X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_719X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_720X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L16665;}
 L30955: {
  if ((3 == (3 & arg5_726X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_726X))) + -4))))>>2))))) {
      goto L30968;}
    else {
      goto L30963;}}
  else {
    goto L30963;}}
 L30950: {
  if ((3 == (3 & arg5_726X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg5_726X))) + -4))))>>2))))) {
      goto L30955;}
    else {
      goto L31022;}}
  else {
    goto L31022;}}
 L23037: {
  port_913X = arg0K0;
  if ((3 == (3 & port_913X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_913X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_913X))) + 4))))>>2))))) {
        goto L23097;}
      else {
        p_914X = *((long *) ((((char *) (-3 + port_913X))) + 24));
        p_915X = *((long *) ((((char *) (-3 + port_913X))) + 28));
        b_916X = *((long *) ((((char *) (-3 + port_913X))) + 20));
        i_917X = ((p_914X)>>2);
        x_918X = *((long *) ((((char *) (-3 + port_913X))) + 12));
        if ((5 == x_918X)) {
          goto L23077;}
        else {
          if ((i_917X == (((p_915X)>>2)))) {
            goto L23077;}
          else {
            val_919X = 4 + (((i_917X)<<2));
            addr_920X = (((char *) (-3 + port_913X))) + 24;
            S48_WRITE_BARRIER(port_913X, addr_920X, val_919X);
            *((long *) addr_920X) = val_919X;
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_916X))) + i_917X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L18438;}}}}
    else {
      goto L23097;}}
  else {
    goto L23097;}}
 L23257: {
  port_921X = arg0K0;
  if ((3 == (3 & port_921X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_921X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_921X))) + 4))))>>2))))) {
        goto L23317;}
      else {
        p_922X = *((long *) ((((char *) (-3 + port_921X))) + 24));
        p_923X = *((long *) ((((char *) (-3 + port_921X))) + 28));
        b_924X = *((long *) ((((char *) (-3 + port_921X))) + 20));
        i_925X = ((p_922X)>>2);
        x_926X = *((long *) ((((char *) (-3 + port_921X))) + 12));
        if ((5 == x_926X)) {
          goto L23297;}
        else {
          if ((i_925X == (((p_923X)>>2)))) {
            goto L23297;}
          else {
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_924X))) + i_925X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L18438;}}}}
    else {
      goto L23317;}}
  else {
    goto L23317;}}
 L23470: {
  Kchar_927X = arg0K0;
  port_928X = arg0K1;
  if ((9 == (255 & Kchar_927X))) {
    if ((3 == (3 & port_928X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_928X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_928X))) + 4))))>>2))))) {
          goto L23531;}
        else {
          p_929X = *((long *) ((((char *) (-3 + port_928X))) + 24));
          b_930X = *((long *) ((((char *) (-3 + port_928X))) + 20));
          i_931X = ((p_929X)>>2);
          x_932X = *((long *) ((((char *) (-3 + port_928X))) + 12));
          if ((5 == x_932X)) {
            goto L23513;}
          else {
            if ((i_931X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_930X))) + -4))))>>8)))) {
              goto L23513;}
            else {
              val_933X = 4 + (((i_931X)<<2));
              addr_934X = (((char *) (-3 + port_928X))) + 24;
              S48_WRITE_BARRIER(port_928X, addr_934X, val_933X);
              *((long *) addr_934X) = val_933X;
              *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_928X))) + 20)))))) + i_931X)) = (((((Kchar_927X)>>8))));
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L18438;}}}}
      else {
        goto L23531;}}
    else {
      goto L23531;}}
  else {
    goto L23531;}}
 L29452: {
  stuff_935X = arg0K0;
  if ((3 == (3 & stuff_935X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_935X))) + -4))))>>2))))) {
      thing_936X = *((long *) (((char *) (-3 + stuff_935X))));
      if ((0 == (3 & thing_936X))) {
        ps_write_integer((((thing_936X)>>2)), out_738X);
        goto L29458;}
      else {
        if ((9 == (255 & thing_936X))) {
          ps_write_string("#\\", out_738X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_936X)>>8))), out_738X, ignoreXX) }
          goto L29458;}
        else {
          if ((3 == (3 & thing_936X))) {
            if ((8 == (31 & ((((*((long *) ((((char *) (-3 + thing_936X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_936X))) + -4))))>>8))))>>2)))) {
                type_937X = *((long *) (((char *) (-3 + thing_936X))));
                if ((3 == (3 & type_937X))) {
                  if ((8 == (31 & ((((*((long *) ((((char *) (-3 + type_937X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_937X))) + -4))))>>8))))>>2)))) {
                      obj_938X = *((long *) ((((char *) (-3 + type_937X))) + 8));
                      if ((3 == (3 & obj_938X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_938X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_738X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_936X)))))))) + 8))))))))))))), out_738X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_738X, ignoreXX) }
                          goto L29458;}
                        else {
                          goto L11650;}}
                      else {
                        goto L11650;}}
                    else {
                      goto L11650;}}
                  else {
                    goto L11650;}}
                else {
                  goto L11650;}}
              else {
                goto L11650;}}
            else {
              goto L11650;}}
          else {
            goto L11650;}}}}
    else {
      goto L29443;}}
  else {
    goto L29443;}}
 L16724: {
  SvalS = (*((long *) ((((char *) (-3 + (Sexception_handlersS)))) + (((opcode_740X)<<2)))));
  obj_939X = SvalS;
  if ((3 == (3 & obj_939X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_939X))) + -4))))>>2))))) {
      goto L16741;}
    else {
      goto L16811;}}
  else {
    goto L16811;}}
 L16797: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L16724;}
 L21038: {
  okayP_940X = arg2K0;
  key_941X = arg0K1;
  if (okayP_940X) {
    arg0K0 = key_941X;
    goto L20966;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_941X;
    goto L20966;}}
 L20996: {
  okayP_942X = arg2K0;
  temp_943X = arg0K1;
  if (okayP_942X) {
    arg0K0 = temp_943X;
    goto L20977;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_943X;
    goto L20977;}}
 L20780: {
  start_i_944X = arg0K0;
  arg0K0 = start_i_944X;
  arg0K1 = 2;
  arg0K2 = (SenvS);
  goto L20788;}
 L20680: {
  key_945X = arg0K0;
  if ((1 == (ScontS))) {
    arg0K0 = 1;
    goto L20684;}
  else {
    merged_arg0K0 = key_945X;
    merged_arg0K1 = 2;
    really_preserve_continuation_return_tag = 0;
    goto really_preserve_continuation;
   really_preserve_continuation_return_0:
    v_946X = really_preserve_continuation0_return_value;
    arg0K0 = v_946X;
    goto L20684;}}
 L19863: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_402X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L19163: {
  v_947X = arg0K0;
  merged_arg0K0 = v_947X;
  copy_stack_into_heap_return_tag = 0;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_0:
  if (((SstackS) < (Sstack_limitS))) {
    ps_error("Couldn't get default procedure space (how can this happen?)", 0);
    goto L19120;}
  else {
    goto L19120;}}
 L16894: {
  stack_arg_count_948X = arg0K0;
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (StemplateS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  tem_949X = Sinterrupt_templateS;
  StemplateS = tem_949X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_949X))))))));
  push_continuationB((Scode_pointerS), (4 + stack_arg_count_948X));
  n_950X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L16999;}
 L19124: {
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L16080: {
  protocol_951X = arg0K0;
  stack_space_952X = arg0K1;
  if ((68 == protocol_951X)) {
    if ((stack_arg_count_771X < 3)) {
      skip_953X = *((unsigned char *) ((((char *) (-3 + code_772X))) + (2 + stack_arg_count_771X)));
      if ((0 == skip_953X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_771X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17161;}
      else {
        arg0K0 = skip_953X;
        arg0K1 = stack_arg_count_771X;
        goto L16084;}}
    else {
      skip_954X = *((unsigned char *) ((((char *) (-3 + code_772X))) + 5));
      if ((0 == skip_954X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_771X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17161;}
      else {
        arg0K0 = skip_954X;
        goto L16107;}}}
  else {
    if ((67 == protocol_951X)) {
      if ((stack_arg_count_771X < (*((unsigned char *) ((((char *) (-3 + code_772X))) + 2))))) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_771X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17161;}
      else {
        arg0K0 = 3;
        goto L16107;}}
    else {
      if ((65 == protocol_951X)) {
        wants_stack_args_955X = ((((*((unsigned char *) ((((char *) (-3 + code_772X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_772X))) + 3)));
        if ((stack_arg_count_771X < wants_stack_args_955X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_771X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L17161;}
        else {
          merged_arg0K0 = wants_stack_args_955X;
          merged_arg0K1 = stack_arg_count_771X;
          merged_arg0K2 = 25;
          merged_arg0K3 = 0;
          rest_list_setup_return_tag = 0;
          goto rest_list_setup;
         rest_list_setup_return_0:
          arg0K0 = 4;
          arg0K1 = (1 + wants_stack_args_955X);
          goto L16084;}}
      else {
        if ((63 < protocol_951X)) {
          if ((64 == protocol_951X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_772X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_772X))) + 3)))) == stack_arg_count_771X)) {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_771X;
              goto L16084;}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_771X;
              arg0K2 = 25;
              arg0K3 = 0;
              goto L17161;}}
          else {
            if ((66 == protocol_951X)) {
              length_956X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_772X))) + -4))))>>8);
              index_957X = -2 + length_956X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_772X))) + (-3 + length_956X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_772X))) + index_957X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_772X))) + (1 + index_957X)))));
              goto L16080;}
            else {
              ps_error("unknown protocol", 1, protocol_951X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_771X;
              arg0K2 = 25;
              arg0K3 = 0;
              goto L17161;}}}
        else {
          if ((protocol_951X == stack_arg_count_771X)) {
            arg0K0 = 2;
            arg0K1 = stack_arg_count_771X;
            goto L16084;}
          else {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_771X;
            arg0K2 = 25;
            arg0K3 = 0;
            goto L17161;}}}}}}
 L16357: {
  protocol_958X = arg0K0;
  stack_space_959X = arg0K1;
  if ((68 == protocol_958X)) {
    if ((total_arg_count_790X < 3)) {
      skip_960X = *((unsigned char *) ((((char *) (-3 + code_789X))) + (2 + total_arg_count_790X)));
      if ((0 == skip_960X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_784X;
        arg0K2 = list_args_785X;
        arg0K3 = list_arg_count_786X;
        goto L17161;}
      else {
        merged_arg0K0 = list_args_785X;
        merged_arg0K1 = list_arg_count_786X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = skip_960X;
        arg0K1 = total_arg_count_790X;
        goto L16361;}}
    else {
      skip_961X = *((unsigned char *) ((((char *) (-3 + code_789X))) + 5));
      if ((0 == skip_961X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_784X;
        arg0K2 = list_args_785X;
        arg0K3 = list_arg_count_786X;
        goto L17161;}
      else {
        arg0K0 = skip_961X;
        goto L16386;}}}
  else {
    if ((67 == protocol_958X)) {
      if ((total_arg_count_790X < (*((unsigned char *) ((((char *) (-3 + code_789X))) + 2))))) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_784X;
        arg0K2 = list_args_785X;
        arg0K3 = list_arg_count_786X;
        goto L17161;}
      else {
        arg0K0 = 3;
        goto L16386;}}
    else {
      if ((63 < protocol_958X)) {
        if ((65 == protocol_958X)) {
          wants_stack_args_962X = ((((*((unsigned char *) ((((char *) (-3 + code_789X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_789X))) + 3)));
          if ((total_arg_count_790X < wants_stack_args_962X)) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_784X;
            arg0K2 = list_args_785X;
            arg0K3 = list_arg_count_786X;
            goto L17161;}
          else {
            merged_arg0K0 = wants_stack_args_962X;
            merged_arg0K1 = stack_arg_count_784X;
            merged_arg0K2 = list_args_785X;
            merged_arg0K3 = list_arg_count_786X;
            rest_list_setup_return_tag = 1;
            goto rest_list_setup;
           rest_list_setup_return_1:
            arg0K0 = 4;
            arg0K1 = (1 + wants_stack_args_962X);
            goto L16361;}}
        else {
          if ((64 == protocol_958X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_789X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_789X))) + 3)))) == total_arg_count_790X)) {
              merged_arg0K0 = list_args_785X;
              merged_arg0K1 = list_arg_count_786X;
              push_list_return_tag = 1;
              goto push_list;
             push_list_return_1:
              arg0K0 = 4;
              arg0K1 = total_arg_count_790X;
              goto L16361;}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_784X;
              arg0K2 = list_args_785X;
              arg0K3 = list_arg_count_786X;
              goto L17161;}}
          else {
            if ((66 == protocol_958X)) {
              length_963X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_789X))) + -4))))>>8);
              index_964X = -2 + length_963X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_789X))) + (-3 + length_963X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_789X))) + index_964X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_789X))) + (1 + index_964X)))));
              goto L16357;}
            else {
              ps_error("unknown protocol", 1, protocol_958X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_784X;
              arg0K2 = list_args_785X;
              arg0K3 = list_arg_count_786X;
              goto L17161;}}}}
      else {
        if ((protocol_958X == total_arg_count_790X)) {
          merged_arg0K0 = list_args_785X;
          merged_arg0K1 = list_arg_count_786X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          arg0K0 = 2;
          arg0K1 = total_arg_count_790X;
          goto L16361;}
        else {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_784X;
          arg0K2 = list_args_785X;
          arg0K3 = list_arg_count_786X;
          goto L17161;}}}}}
 L20438: {
  cont_965X = arg0K0;
  if ((3 == (3 & cont_965X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_965X))) + -4))))>>2))))) {
      next_op_966X = *((unsigned char *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_965X))) + 8))))))))))) + ((((*((long *) ((((char *) (-3 + cont_965X))) + 4))))>>2))));
      if ((34 == next_op_966X)) {
        pop_continuationB_return_tag = 2;
        goto pop_continuationB;
       pop_continuationB_return_2:
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        if ((30 == next_op_966X)) {
          next_967X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_967X)))));
            goto L19282;}
          else {
            ScontS = next_967X;
            goto L19282;}}
        else {
          merged_arg0K0 = list_args_802X;
          merged_arg0K1 = stack_nargs_801X;
          pop_args_GlistS_return_tag = 3;
          goto pop_args_GlistS;
         pop_args_GlistS_return_3:
          args_968X = pop_args_GlistS0_return_value;
          push_exception_continuationB(4, 0);
          *((long *) (SstackS)) = 1;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = args_968X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L16665;}}}
    else {
      goto L20444;}}
  else {
    goto L20444;}}
 L18932: {
  v_969X = arg0K0;
  merged_arg0K0 = v_969X;
  copy_stack_into_heap_return_tag = 1;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_1:
  if (((SstackS) < (Sstack_limitS))) {
    ps_error("Couldn't get default procedure space (how can this happen?)", 0);
    goto L18817;}
  else {
    goto L18817;}}
 L18826: {
  arg0K0 = (*((unsigned char *) ((Scode_pointerS) + 3)));
  goto L16894;}
 L18829: {
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L18987: {
  v_970X = arg0K0;
  merged_arg0K0 = v_970X;
  copy_stack_into_heap_return_tag = 2;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_2:
  if ((space_436X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L18817;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L18817;}}
 L10628: {
  if ((3 == (3 & x_456X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + x_456X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L38038;}
    else {
      goto L10634;}}
  else {
    goto L10634;}}
 L28396: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28537: {
  if ((3 == (3 & n_458X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + n_458X))) + -4))))>>2))))) {
      goto L28548;}
    else {
      goto L28549;}}
  else {
    goto L28549;}}
 L28736: {
  if ((3 == (3 & n_459X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + n_459X))) + -4))))>>2))))) {
      goto L28747;}
    else {
      goto L28748;}}
  else {
    goto L28748;}}
 L28935: {
  if ((3 == (3 & n_460X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + n_460X))) + -4))))>>2))))) {
      goto L28946;}
    else {
      goto L28947;}}
  else {
    goto L28947;}}
 L6779: {
  b_971X = arg0K0;
  lo_a_972X = 65535 & a_815X;
  lo_b_973X = 65535 & b_971X;
  hi_a_974X = 65535 & (((a_815X)>>16));
  hi_b_975X = 65535 & (((b_971X)>>16));
  lo_c_976X = SMALL_MULTIPLY(lo_a_972X, lo_b_973X);
  v_977X = SMALL_MULTIPLY(lo_b_973X, hi_a_974X);
  v_978X = SMALL_MULTIPLY(lo_a_972X, hi_b_975X);
  mid_c_979X = v_978X + v_977X;
  c_980X = lo_c_976X + (((mid_c_979X)<<16));
  if ((0 < hi_a_974X)) {
    if ((0 < hi_b_975X)) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_467X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_468X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}
    else {
      goto L6821;}}
  else {
    goto L6821;}}
 L7060: {
  b_981X = arg0K0;
  c_982X = a_816X / b_981X;
  x_983X = 0 == (a_816X % b_981X);
  if (x_983X) {
    if ((a_476X < 0)) {
      if ((b_477X < 0)) {
        goto L7115;}
      else {
        goto L7114;}}
    else {
      if ((b_477X < 0)) {
        goto L7114;}
      else {
        goto L7115;}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_474X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_475X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}}
 L7237: {
  b_984X = arg0K0;
  c_985X = a_822X / b_984X;
  if ((a_490X < 0)) {
    if ((b_491X < 0)) {
      goto L7283;}
    else {
      goto L7282;}}
  else {
    if ((b_491X < 0)) {
      goto L7282;}
    else {
      goto L7283;}}}
 L25340: {
  b_986X = arg0K0;
  c_987X = a_823X % b_986X;
  if ((a_494X < 0)) {
    arg0K0 = (0 - c_987X);
    goto L25344;}
  else {
    arg0K0 = c_987X;
    goto L25344;}}
 L21336: {
  SvalS = new_834X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L21320: {
  i_988X = arg0K0;
  if ((i_988X < 0)) {
    goto L21336;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_834X))) + (((i_988X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_988X);
    goto L21320;}}
 L21505: {
  i_989X = arg0K0;
  if ((i_989X < 0)) {
    arg0K0 = stack_nargs_839X;
    arg0K1 = rest_list_840X;
    goto L21523;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_838X))) + (((i_989X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_989X);
    goto L21505;}}
 L22028: {
  i_990X = arg0K0;
  if ((i_990X < 0)) {
    SvalS = value_845X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    addr_991X = (((char *) (-3 + value_845X))) + (((i_990X)<<2));
    S48_WRITE_BARRIER(value_845X, addr_991X, init_842X);
    *((long *) addr_991X) = init_842X;
    arg0K0 = (-1 + i_990X);
    goto L22028;}}
 L26115: {
  i_992X = arg0K0;
  if ((i_992X < 0)) {
    SvalS = vector_847X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_847X))) + i_992X)) = init_584X;
    arg0K0 = (-1 + i_992X);
    goto L26115;}}
 L26349: {
  i_993X = arg0K0;
  if ((i_993X < 0)) {
    SvalS = string_850X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_850X))) + i_993X)) = (init_600X);
    arg0K0 = (-1 + i_993X);
    goto L26349;}}
 L13333: {
  i_994X = arg0K0;
  h_995X = arg0K1;
  if ((i_994X < n_854X)) {
    arg0K0 = (1 + i_994X);
    arg0K1 = (h_995X + (((*((unsigned char *) ((((char *) (-3 + string_853X))) + i_994X))))));
    goto L13333;}
  else {
    index_996X = 1023 & h_995X;
    bucket_997X = *((long *) ((((char *) (-3 + table_852X))) + (((index_996X)<<2))));
    arg0K0 = bucket_997X;
    goto L13301;}}
 L24488: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L29550: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L33097: {
  key_998X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_999X = *((long *) (SstackS));
  if ((0 == (3 & (SvalS)))) {
    mode_1000X = (((SvalS))>>2);
    if ((1 == mode_1000X)) {
      goto L26773;}
    else {
      if ((2 == mode_1000X)) {
        goto L26773;}
      else {
        if ((3 == mode_1000X)) {
          goto L26773;}
        else {
          if ((4 == mode_1000X)) {
            goto L26773;}
          else {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = arg2_999X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((mode_1000X)<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L16665;}}}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_999X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}}
 L34456: {
  key_1001X = arg0K0;
  obj_1002X = SvalS;
  if ((3 == (3 & obj_1002X))) {
    if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_1002X))) + -4))))>>2))))) {
      channel_1003X = SvalS;
      if ((0 == (*((long *) (((char *) (-3 + channel_1003X))))))) {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = channel_1003X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L16665;}
      else {
        status_1004X = close_channelB(channel_1003X);
        if ((status_1004X == NO_ERRORS)) {
          SvalS = 13;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}
        else {
          push_exception_continuationB(25, 1);
          *((long *) (SstackS)) = channel_1003X;
          SstackS = ((SstackS) + -4);
          merged_arg0K0 = status_1004X;
          merged_arg0K1 = key_1001X;
          get_error_string_return_tag = 0;
          goto get_error_string;
         get_error_string_return_0:
          x_1005X = get_error_string0_return_value;
          *((long *) (SstackS)) = x_1005X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L16665;}}}
    else {
      goto L34464;}}
  else {
    goto L34464;}}
 L37273: {
  key_1006X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_1007X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_1008X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg4_1009X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg5_1010X = *((long *) (SstackS));
  if ((0 == (3 & (arg4_1009X | arg3_1008X)))) {
    if ((1 == arg2_1007X)) {
      goto L37305;}
    else {
      if ((5 == arg2_1007X)) {
        goto L37305;}
      else {
        goto L37325;}}}
  else {
    goto L37325;}}
 L37545: {
  key_1011X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_1012X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_1013X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg4_1014X = *((long *) (SstackS));
  if ((0 == (3 & (arg3_1013X | arg2_1012X)))) {
    obj_1015X = SvalS;
    if ((3 == (3 & obj_1015X))) {
      if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_1015X))) + -4))))>>2))))) {
        start_1016X = ((arg3_1013X)>>2);
        count_1017X = ((arg2_1012X)>>2);
        channel_1018X = SvalS;
        v_1019X = 8 == (*((long *) (((char *) (-3 + channel_1018X)))));
        if (v_1019X) {
          if ((3 == (3 & arg4_1014X))) {
            if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg4_1014X))) + -4))))>>2))))) {
              goto L36856;}
            else {
              goto L36848;}}
          else {
            goto L36848;}}
        else {
          arg0K0 = 5;
          goto L36739;}}
      else {
        goto L37587;}}
    else {
      goto L37587;}}
  else {
    goto L37587;}}
 L8203: {
  val_1020X = *((long *) ((((char *) (-3 + ch_868X))) + 12));
  addr_1021X = (((char *) (-3 + prev_869X))) + 12;
  S48_WRITE_BARRIER(prev_869X, addr_1021X, val_1020X);
  *((long *) addr_1021X) = val_1020X;
  addr_1022X = (((char *) (-3 + ch_868X))) + 12;
  S48_WRITE_BARRIER(ch_868X, addr_1022X, 1);
  *((long *) addr_1022X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_868X))) + 16)));
  goto L33202;}
 L14784: {
  i_1023X = arg0K0;
  res_1024X = arg0K1;
  if ((-1 == i_1023X)) {
    SvalS = res_1024X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    channel_1025X = *((Svm_channelsS) + i_1023X);
    if ((3 == (3 & channel_1025X))) {
      if ((5 == (31 & ((((*((long *) ((((char *) (-3 + channel_1025X))) + -4))))>>2))))) {
        addr_1026X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_1026X) = 2050;
        x_1027X = 3 + (((long) (addr_1026X + 4)));
        *((long *) (((char *) (-3 + x_1027X)))) = channel_1025X;
        *((long *) ((((char *) (-3 + x_1027X))) + 4)) = res_1024X;
        arg0K0 = x_1027X;
        goto L14798;}
      else {
        arg0K0 = res_1024X;
        goto L14798;}}
    else {
      arg0K0 = res_1024X;
      goto L14798;}}}
 L34529: {
  key_1028X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_1029X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_1030X = *((long *) (SstackS));
  if ((3 == (3 & arg3_1030X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg3_1030X))) + -4))))>>2))))) {
      obj_1031X = SvalS;
      if ((3 == (3 & obj_1031X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_1031X))) + -4))))>>2))))) {
          comment_string_1032X = SvalS;
          x_1033X = s48_image_writing_okayP();
          if (x_1033X) {
            port_1034X = ps_open_output_file((((char *)(((char *) (-3 + arg3_1030X))))), &status_1035X);
            if ((status_1035X == NO_ERRORS)) {
              status_1036X = ps_write_string((((char *)(((char *) (-3 + comment_string_1032X))))), port_1034X);
              if ((status_1036X == NO_ERRORS)) {
                status_1037X = s48_write_image(arg2_1029X, port_1034X);
                if ((status_1037X == NO_ERRORS)) {
                  status_1038X = ps_close(port_1034X);
                  if ((status_1038X == NO_ERRORS)) {
                    undumpables_1039X = s48_undumpable_records(&undumpable_count_1040X);
                    if ((0 == undumpable_count_1040X)) {
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L18438;}
                    else {
                      push_exception_continuationB(26, 1);
                      *((long *) (SstackS)) = arg3_1030X;
                      SstackS = ((SstackS) + -4);
                      *((long *) (SstackS)) = arg2_1029X;
                      SstackS = ((SstackS) + -4);
                      *((long *) (SstackS)) = comment_string_1032X;
                      SstackS = ((SstackS) + -4);
                      *((long *) (SstackS)) = undumpables_1039X;
                      SstackS = ((SstackS) + -4);
                      *((long *) (SstackS)) = (((undumpable_count_1040X)<<2));
                      SstackS = ((SstackS) + -4);
                      arg0K0 = 5;
                      goto L16665;}}
                  else {
                    arg0K0 = 25;
                    arg0K1 = status_1038X;
                    goto L30184;}}
                else {
                  status_1041X = ps_close(port_1034X);
                  if ((status_1041X == NO_ERRORS)) {
                    arg0K0 = 25;
                    arg0K1 = status_1037X;
                    goto L30184;}
                  else {
                    ps_write_string("Unable to close image file", (stderr));
                    { long ignoreXX;
                    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                    arg0K0 = 25;
                    arg0K1 = status_1037X;
                    goto L30184;}}}
              else {
                status_1042X = ps_close(port_1034X);
                if ((status_1042X == NO_ERRORS)) {
                  arg0K0 = 25;
                  arg0K1 = status_1036X;
                  goto L30184;}
                else {
                  ps_write_string("Unable to close image file", (stderr));
                  { long ignoreXX;
                  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                  arg0K0 = 25;
                  arg0K1 = status_1036X;
                  goto L30184;}}}
            else {
              arg0K0 = 10;
              arg0K1 = status_1035X;
              goto L30184;}}
          else {
            push_exception_continuationB(15, 1);
            *((long *) (SstackS)) = arg3_1030X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = arg2_1029X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = comment_string_1032X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 3;
            goto L16665;}}
        else {
          goto L34561;}}
      else {
        goto L34561;}}
    else {
      goto L34561;}}
  else {
    goto L34561;}}
 L30563: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = stob_877X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_878X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L34107: {
  minutesP_1043X = arg2K0;
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_1044X = check_events0_return_value;
      if (temp_1044X) {
        goto L34133;}
      else {
        goto L34138;}}
    else {
      goto L34133;}}
  else {
    goto L34138;}}
 L22847: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proc_896X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = name_897X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L33368: {
  key_1045X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_1046X = *((long *) (SstackS));
  if ((3 == (3 & arg2_1046X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_1046X))) + -4))))>>2))))) {
      x_1047X = SvalS;
      if ((1 == x_1047X)) {
        goto L33385;}
      else {
        if ((5 == x_1047X)) {
          goto L33385;}
        else {
          goto L33390;}}}
    else {
      goto L33390;}}
  else {
    goto L33390;}}
 L23938: {
  table_1048X = arg0K0;
  n_1049X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_686X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L12880;}
 L27369: {
  len_1050X = 1 + n_907X;
  addr_1051X = ALLOCATE_SPACE(16, (4 + len_1050X));
  *((long *) addr_1051X) = (66 + (((len_1050X)<<8)));
  string_1052X = 3 + (((long) (addr_1051X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1052X))) + n_907X)) = 0;
  arg0K0 = arg2_906X;
  arg0K1 = (-1 + n_907X);
  goto L27346;}
 L27321: {
  if ((25 == arg2_906X)) {
    goto L27369;}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_906X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_907X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}}
 L30968: {
  if ((from_index_727X < 0)) {
    goto L31022;}
  else {
    if ((to_index_728X < 0)) {
      goto L31022;}
    else {
      if ((count_729X < 0)) {
        goto L31022;}
      else {
        if ((3 == (3 & arg5_726X))) {
          if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_726X))) + -4))))>>2))))) {
            arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_726X))) + -4))))>>8)));
            goto L30989;}
          else {
            goto L30985;}}
        else {
          goto L30985;}}}}}
 L30963: {
  if ((3 == (3 & arg5_726X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg5_726X))) + -4))))>>2))))) {
      goto L30968;}
    else {
      goto L31022;}}
  else {
    goto L31022;}}
 L31022: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_726X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_727X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_724X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_728X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_729X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L16665;}
 L23097: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_913X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L23077: {
  push_exception_continuationB(14, 2);
  *((long *) (SstackS)) = port_913X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L23317: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_921X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L23297: {
  push_exception_continuationB(14, 2);
  *((long *) (SstackS)) = port_921X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L23531: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = Kchar_927X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_928X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L23513: {
  push_exception_continuationB(14, 2);
  *((long *) (SstackS)) = Kchar_927X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_928X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L29458: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_935X))) + 4)));
  goto L29452;}
 L11650: {
  if ((3 == (3 & thing_936X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + thing_936X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_936X)))));
      goto L11692;}
    else {
      goto L11658;}}
  else {
    goto L11658;}}
 L29443: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_738X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L16741: {
  StemplateS = (SvalS);
  Slosing_opcodeS = opcode_740X;
  arg0K0 = (2 + nargs_739X);
  goto L16066;}
 L16811: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L16741;}
 L20966: {
  key_1053X = arg0K0;
  p_1054X = SenvS;
  if ((3 == (3 & p_1054X))) {
    if ((p_1054X < (((long) (Sstack_beginS))))) {
      goto L21061;}
    else {
      if (((((long) (Sstack_endS))) < p_1054X)) {
        goto L21061;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_1053X;
        merged_arg0K3 = 0;
        save_env_in_heap_return_tag = 0;
        goto save_env_in_heap;
       save_env_in_heap_return_0:
        v_1055X = save_env_in_heap0_return_value;
        SenvS = v_1055X;
        goto L21061;}}}
  else {
    goto L21061;}}
 L20977: {
  env_1056X = arg0K0;
  a_1057X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  addr_1058X = ALLOCATE_SPACE(3, 12);
  *((long *) addr_1058X) = 2062;
  x_1059X = 3 + (((long) (addr_1058X + 4)));
  *((long *) (((char *) (-3 + x_1059X)))) = a_1057X;
  *((long *) ((((char *) (-3 + x_1059X))) + 4)) = env_1056X;
  if ((3 == (3 & x_1059X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_1059X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_1059X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_1059X))) + -4))));
      arg0K0 = x_1059X;
      goto L20985;}
    else {
      arg0K0 = x_1059X;
      goto L20985;}}
  else {
    arg0K0 = x_1059X;
    goto L20985;}}
 L20788: {
  i_1060X = arg0K0;
  offset_1061X = arg0K1;
  env_1062X = arg0K2;
  if ((i_1060X == total_count_393X)) {
    SvalS = new_env_762X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1061X));
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    back_1063X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1061X)));
    arg0K0 = env_1062X;
    arg0K1 = back_1063X;
    goto L20884;}}
 L20684: {
  value_1064X = arg0K0;
  SvalS = value_1064X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L16999: {
  i_1065X = arg0K0;
  m_1066X = arg0K1;
  if ((0 == (n_950X & m_1066X))) {
    arg0K0 = (1 + i_1065X);
    arg0K1 = (((m_1066X)<<1));
    goto L16999;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_1066X));
    if ((i_1065X == 0)) {
      *((long *) (SstackS)) = (Sinterrupted_templateS);
      SstackS = ((SstackS) + -4);
      Sinterrupted_templateS = 1;
      *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16918;}
    else {
      if ((i_1065X == 2)) {
        *((long *) (SstackS)) = (Sfinalize_theseS);
        SstackS = ((SstackS) + -4);
        Sfinalize_theseS = 25;
        *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16918;}
      else {
        if ((i_1065X == 3)) {
          channel_1067X = Spending_channels_headS;
          next_1068X = *((long *) ((((char *) (-3 + channel_1067X))) + 12));
          Spending_channels_headS = next_1068X;
          addr_1069X = (((char *) (-3 + channel_1067X))) + 12;
          S48_WRITE_BARRIER(channel_1067X, addr_1069X, 1);
          *((long *) addr_1069X) = 1;
          if ((1 == next_1068X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_1067X;
            goto L8364;}
          else {
            arg0K0 = channel_1067X;
            goto L8364;}}
        else {
          if ((i_1065X == 4)) {
            *((long *) (SstackS)) = (Sos_signal_typeS);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (Sos_signal_argumentS);
            SstackS = ((SstackS) + -4);
            Sos_signal_typeS = 1;
            Sos_signal_argumentS = 1;
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 3;
            goto L16918;}
          else {
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L16918;}}}}}}
 L16084: {
  skip_1070X = arg0K0;
  stack_arg_count_1071X = arg0K1;
  template_1072X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_1072X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_1072X)))))))) + skip_1070X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  arg0K0 = stack_space_952X;
  arg0K1 = stack_arg_count_1071X;
  goto L15974;}
 L16107: {
  skip_1073X = arg0K0;
  *((long *) (SstackS)) = 25;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((stack_arg_count_771X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((stack_arg_count_771X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_1073X;
  arg0K1 = (3 + stack_arg_count_771X);
  goto L16084;}
 L16361: {
  skip_1074X = arg0K0;
  stack_arg_count_1075X = arg0K1;
  template_1076X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_1076X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_1076X)))))))) + skip_1074X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  arg0K0 = stack_space_959X;
  arg0K1 = stack_arg_count_1075X;
  goto L15974;}
 L16386: {
  skip_1077X = arg0K0;
  if ((total_arg_count_790X < 3)) {
    arg0K0 = total_arg_count_790X;
    goto L16394;}
  else {
    if ((2 < stack_arg_count_784X)) {
      arg0K0 = stack_arg_count_784X;
      goto L16394;}
    else {
      arg0K0 = 2;
      goto L16394;}}}
 L19282: {
  SvalS = (*((long *) ((((char *) (-3 + cont_965X))) + 16)));
  arg0K0 = stack_nargs_801X;
  arg0K1 = list_args_802X;
  arg0K2 = list_arg_count_803X;
  goto L17666;}
 L20444: {
  merged_arg0K0 = list_args_802X;
  merged_arg0K1 = stack_nargs_801X;
  pop_args_GlistS_return_tag = 4;
  goto pop_args_GlistS;
 pop_args_GlistS_return_4:
  args_1078X = pop_args_GlistS0_return_value;
  push_exception_continuationB(4, 0);
  *((long *) (SstackS)) = 1;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = args_1078X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L10634: {
  if ((3 == (3 & x_456X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_456X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L38038;}
    else {
      arg0K0 = 1;
      goto L38038;}}
  else {
    arg0K0 = 1;
    goto L38038;}}
 L28549: {
  if ((3 == (3 & n_458X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + n_458X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_458X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
    else {
      goto L28553;}}
  else {
    goto L28553;}}
 L28748: {
  if ((3 == (3 & n_459X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + n_459X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_459X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
    else {
      goto L28752;}}
  else {
    goto L28752;}}
 L28947: {
  if ((3 == (3 & n_460X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + n_460X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_460X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L16665;}
    else {
      goto L28951;}}
  else {
    goto L28951;}}
 L6821: {
  if ((536870911 < lo_c_976X)) {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_467X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_468X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}
  else {
    if ((lo_c_976X < 0)) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_467X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_468X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}
    else {
      if ((8192 < mid_c_979X)) {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_467X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_468X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}
      else {
        if ((a_469X < 0)) {
          if ((b_470X < 0)) {
            goto L6848;}
          else {
            goto L6855;}}
        else {
          if ((b_470X < 0)) {
            goto L6855;}
          else {
            goto L6848;}}}}}}
 L7115: {
  if ((536870911 < c_982X)) {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_474X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_475X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}
  else {
    SvalS = (((c_982X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L7114: {
  SvalS = ((((0 - c_982X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L7283: {
  if ((536870911 < c_985X)) {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_488X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_489X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}
  else {
    SvalS = (((c_985X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L7282: {
  SvalS = ((((0 - c_985X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L25344: {
  n_1079X = arg0K0;
  SvalS = (((n_1079X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L21523: {
  i_1080X = arg0K0;
  rest_list_1081X = arg0K1;
  if ((25 == rest_list_1081X)) {
    SvalS = new_838X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    *((long *) ((((char *) (-3 + new_838X))) + (((i_1080X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_1081X)))));
    arg0K0 = (1 + i_1080X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_1081X))) + 4)));
    goto L21523;}}
 L13301: {
  foo_1082X = arg0K0;
  if ((1 == foo_1082X)) {
    addr_1083X = ALLOCATE_SPACE(1, 12);
    *((long *) addr_1083X) = 2054;
    x_1084X = 3 + (((long) (addr_1083X + 4)));
    *((long *) (((char *) (-3 + x_1084X)))) = string_853X;
    *((long *) ((((char *) (-3 + x_1084X))) + 4)) = bucket_997X;
    addr_1085X = (((char *) (-3 + table_852X))) + (((index_996X)<<2));
    S48_WRITE_BARRIER(table_852X, addr_1085X, x_1084X);
    *((long *) addr_1085X) = x_1084X;
    arg0K0 = x_1084X;
    goto L24529;}
  else {
    s2_1086X = *((long *) (((char *) (-3 + foo_1082X))));
    len_1087X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_853X))) + -4))))>>8);
    if ((len_1087X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1086X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1086X))), (void *)(((char *) (-3 + string_853X))),len_1087X)))) {
        arg0K0 = foo_1082X;
        goto L24529;}
      else {
        goto L13317;}}
    else {
      goto L13317;}}}
 L26773: {
  if ((0 == (3 & arg2_999X))) {
    if (((((arg2_999X)>>2)) < 0)) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_999X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_1000X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}
    else {
      arg0K0 = (((arg2_999X)>>2));
      goto L26593;}}
  else {
    if ((3 == (3 & arg2_999X))) {
      if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_999X))) + -4))))>>2))))) {
        if ((1 == mode_1000X)) {
          goto L26651;}
        else {
          if ((3 == mode_1000X)) {
            goto L26651;}
          else {
            v_1088X = ps_open_fd((((char *)(((char *) (-3 + arg2_999X))))), 0, &v_1089X);
            arg0K0 = v_1088X;
            arg0K1 = v_1089X;
            goto L26664;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_999X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_1000X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_999X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_1000X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}}}
 L34464: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L16665;}
 L37305: {
  obj_1090X = SvalS;
  if ((3 == (3 & obj_1090X))) {
    if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_1090X))) + -4))))>>2))))) {
      x_1091X = SvalS;
      if ((1 == arg2_1007X)) {
        arg2K0 = 0;
        goto L37322;}
      else {
        arg2K0 = 1;
        goto L37322;}}
    else {
      goto L37325;}}
  else {
    goto L37325;}}
 L37325: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_1010X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_1009X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_1008X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_1007X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L16665;}
 L36856: {
  if ((3 == (3 & arg4_1014X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg4_1014X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_1014X))) + -4))))>>8)));
      goto L36868;}
    else {
      goto L36864;}}
  else {
    goto L36864;}}
 L36848: {
  if ((3 == (3 & arg4_1014X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_1014X))) + -4))))>>2))))) {
      goto L36856;}
    else {
      arg0K0 = 5;
      goto L36739;}}
  else {
    arg0K0 = 5;
    goto L36739;}}
 L36739: {
  reason_1092X = arg0K0;
  push_exception_continuationB(reason_1092X, 1);
  *((long *) (SstackS)) = arg4_1014X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1016X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1017X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = channel_1018X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L16665;}
 L37587: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_1014X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_1013X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_1012X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L16665;}
 L14798: {
  v_1093X = arg0K0;
  arg0K0 = (-1 + i_1023X);
  arg0K1 = v_1093X;
  goto L14784;}
 L30184: {
  reason_1094X = arg0K0;
  status_1095X = arg0K1;
  push_exception_continuationB(reason_1094X, 1);
  *((long *) (SstackS)) = arg3_1030X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_1029X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = comment_string_1032X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_1095X;
  merged_arg0K1 = key_1028X;
  get_error_string_return_tag = 1;
  goto get_error_string;
 get_error_string_return_1:
  x_1096X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_1096X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L16665;}
 L34561: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_1030X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_1029X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16665;}
 L34133: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L34138: {
  if ((0 == (Spending_interruptsS))) {
    s48_wait_for_event((((arg2_678X)>>2)), minutesP_1043X);
    goto L34133;}
  else {
    goto L34133;}}
 L33385: {
  if ((1 == (SvalS))) {
    v_1097X = Hlookup2102((Sexported_bindingsS), arg2_1046X, key_1045X);
    arg0K0 = v_1097X;
    goto L33442;}
  else {
    v_1098X = Hlookup2083((Simported_bindingsS), arg2_1046X, key_1045X);
    arg0K0 = v_1098X;
    goto L33442;}}
 L33390: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_1046X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L16665;}
 L12880: {
  i_1099X = arg0K0;
  h_1100X = arg0K1;
  if ((i_1099X < n_1049X)) {
    arg0K0 = (1 + i_1099X);
    arg0K1 = (h_1100X + (((*((unsigned char *) ((((char *) (-3 + arg2_686X))) + i_1099X))))));
    goto L12880;}
  else {
    index_1101X = 1023 & h_1100X;
    bucket_1102X = *((long *) ((((char *) (-3 + table_1048X))) + (((index_1101X)<<2))));
    arg0K0 = 1;
    arg0K1 = bucket_1102X;
    goto L12844;}}
 L27346: {
  l_1103X = arg0K0;
  i_1104X = arg0K1;
  if ((i_1104X < 0)) {
    SvalS = string_1052X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1052X))) + i_1104X)) = ((((((*((long *) (((char *) (-3 + l_1103X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_1103X))) + 4)));
    arg0K1 = (-1 + i_1104X);
    goto L27346;}}
 L30989: {
  y_1105X = arg0K0;
  if ((y_1105X < (from_index_727X + count_729X))) {
    goto L31022;}
  else {
    if ((3 == (3 & arg3_724X))) {
      if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg3_724X))) + -4))))>>2))))) {
        arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_724X))) + -4))))>>8)));
        goto L31004;}
      else {
        goto L31000;}}
    else {
      goto L31000;}}}
 L30985: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_726X))) + -4))))>>8));
  goto L30989;}
 L11692: {
  v_1106X = arg3K0;
  ps_write_string(v_1106X, out_738X);
  goto L29458;}
 L11658: {
  if ((3 == (3 & thing_936X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_936X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_936X))))))))));
      goto L11692;}
    else {
      goto L11666;}}
  else {
    goto L11666;}}
 L21061: {
  arg0K0 = (SenvS);
  goto L20971;}
 L20985: {
  value_1107X = arg0K0;
  SvalS = value_1107X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L20884: {
  env_1108X = arg0K0;
  i_1109X = arg0K1;
  if ((0 == i_1109X)) {
    count_1110X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_1061X)));
    arg0K0 = count_1110X;
    arg0K1 = i_1060X;
    arg0K2 = (2 + offset_1061X);
    goto L20805;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1108X)))));
    arg0K1 = (-1 + i_1109X);
    goto L20884;}}
 L16918: {
  arg_count_1111X = arg0K0;
  obj_1112X = Sinterrupt_handlersS;
  if ((3 == (3 & obj_1112X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_1112X))) + -4))))>>2))))) {
      goto L16932;}
    else {
      goto L17012;}}
  else {
    goto L17012;}}
 L8364: {
  channel_1113X = arg0K0;
  x_1114X = 1 == (Spending_channels_headS);
  if (x_1114X) {
    goto L8378;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L8378;}
      else {
        goto L8378;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L8378;}}}
 L15974: {
  stack_slots_1115X = arg0K0;
  stack_arg_count_1116X = arg0K1;
  if ((stack_slots_1115X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L15976;}
  else {
    space_1117X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
    v_1118X = AVAILABLEp(space_1117X);
    if (v_1118X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L16033;}
    else {
      collect_saving_temps(1, 1, &temp1_1119X);
      v_1120X = AVAILABLEp(space_1117X);
      if (v_1120X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L16033;}
      else {
        arg2K0 = 0;
        arg0K1 = 0;
        goto L16033;}}}}
 L16394: {
  final_stack_arg_count_1121X = arg0K0;
  if ((stack_arg_count_784X < final_stack_arg_count_1121X)) {
    arg0K0 = final_stack_arg_count_1121X;
    goto L16398;}
  else {
    arg0K0 = stack_arg_count_784X;
    goto L16398;}}
 L28553: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28752: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L28951: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L6848: {
  if ((536870911 < c_980X)) {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_467X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_468X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}
  else {
    SvalS = (((c_980X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L6855: {
  if ((536870912 < c_980X)) {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_467X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_468X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}
  else {
    SvalS = ((((0 - c_980X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L24529: {
  val_1122X = arg0K0;
  SvalS = val_1122X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L13317: {
  arg0K0 = (*((long *) ((((char *) (-3 + foo_1082X))) + 4)));
  goto L13301;}
 L26593: {
  index_1123X = arg0K0;
  channel_1124X = make_registered_channel(mode_1000X, arg2_999X, index_1123X, key_998X, &reason_1125X);
  if ((1 == channel_1124X)) {
    if ((3 == (3 & arg2_999X))) {
      if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_999X))) + -4))))>>2))))) {
        if ((1 == mode_1000X)) {
          goto L26708;}
        else {
          if ((3 == mode_1000X)) {
            goto L26708;}
          else {
            v_1126X = ps_close_fd(index_1123X);
            arg0K0 = v_1126X;
            goto L26703;}}}
      else {
        push_exception_continuationB(reason_1125X, 1);
        *((long *) (SstackS)) = arg2_999X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_1000X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L16665;}}
    else {
      push_exception_continuationB(reason_1125X, 1);
      *((long *) (SstackS)) = arg2_999X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_1000X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}}
  else {
    SvalS = channel_1124X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L26651: {
  v_1127X = ps_open_fd((((char *)(((char *) (-3 + arg2_999X))))), 1, &v_1128X);
  arg0K0 = v_1127X;
  arg0K1 = v_1128X;
  goto L26664;}
 L26664: {
  channel_1129X = arg0K0;
  status_1130X = arg0K1;
  if ((status_1130X == NO_ERRORS)) {
    arg0K0 = channel_1129X;
    goto L26593;}
  else {
    if ((status_1130X == ENOENT)) {
      push_exception_continuationB(10, 1);
      *((long *) (SstackS)) = arg2_999X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_1000X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L16665;}
    else {
      push_exception_continuationB(25, 1);
      *((long *) (SstackS)) = arg2_999X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_1000X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1130X;
      merged_arg0K1 = key_998X;
      get_error_string_return_tag = 2;
      goto get_error_string;
     get_error_string_return_2:
      x_1131X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1131X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 3;
      goto L16665;}}}
 L37322: {
  waitP_1132X = arg2K0;
  start_1133X = ((arg4_1009X)>>2);
  count_1134X = ((arg3_1008X)>>2);
  if ((4 == (*((long *) (((char *) (-3 + x_1091X))))))) {
    if ((3 == (3 & arg5_1010X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + arg5_1010X))) + -4)))))) {
        if ((3 == (3 & arg5_1010X))) {
          if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_1010X))) + -4))))>>2))))) {
            goto L36458;}
          else {
            goto L36450;}}
        else {
          goto L36450;}}
      else {
        arg0K0 = 5;
        goto L36341;}}
    else {
      arg0K0 = 5;
      goto L36341;}}
  else {
    arg0K0 = 5;
    goto L36341;}}
 L36868: {
  length_1135X = arg0K0;
  if ((length_1135X < (start_1016X + count_1017X))) {
    arg0K0 = 7;
    goto L36739;}
  else {
    got_1136X = ps_write_fd(((((*((long *) ((((char *) (-3 + channel_1018X))) + 8))))>>2)), ((((char *) (-3 + arg4_1014X))) + start_1016X), count_1017X, &pendingP_1137X, &status_1138X);
    if ((status_1138X == NO_ERRORS)) {
      if (pendingP_1137X) {
        addr_1139X = (((char *) (-3 + channel_1018X))) + 16;
        S48_WRITE_BARRIER(channel_1018X, addr_1139X, 5);
        *((long *) addr_1139X) = 5;
        s48_Spending_interruptPS = 0;
        Senabled_interruptsS = 0;
        arg0K0 = 13;
        goto L36739;}
      else {
        SvalS = (((got_1136X)<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}}
    else {
      push_exception_continuationB(25, 1);
      *((long *) (SstackS)) = arg4_1014X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1016X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1017X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = channel_1018X;
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1138X;
      merged_arg0K1 = key_1011X;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_1140X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1140X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 5;
      goto L16665;}}}
 L36864: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_1014X))) + -4))))>>8));
  goto L36868;}
 L33442: {
  val_1141X = arg0K0;
  SvalS = val_1141X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L12844: {
  previous_foo_1142X = arg0K0;
  foo_1143X = arg0K1;
  if ((1 == foo_1143X)) {
    goto L23940;}
  else {
    s2_1144X = *((long *) (((char *) (-3 + foo_1143X))));
    len_1145X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_686X))) + -4))))>>8);
    if ((len_1145X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1144X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1144X))), (void *)(((char *) (-3 + arg2_686X))),len_1145X)))) {
        if ((1 == previous_foo_1142X)) {
          value_1146X = *((long *) ((((char *) (-3 + foo_1143X))) + 12));
          addr_1147X = (((char *) (-3 + table_1048X))) + (((index_1101X)<<2));
          S48_WRITE_BARRIER(table_1048X, addr_1147X, value_1146X);
          *((long *) addr_1147X) = value_1146X;
          goto L23940;}
        else {
          val_1148X = *((long *) ((((char *) (-3 + foo_1143X))) + 12));
          addr_1149X = (((char *) (-3 + previous_foo_1142X))) + 12;
          S48_WRITE_BARRIER(previous_foo_1142X, addr_1149X, val_1148X);
          *((long *) addr_1149X) = val_1148X;
          goto L23940;}}
      else {
        goto L12906;}}
    else {
      goto L12906;}}}
 L31004: {
  y_1150X = arg0K0;
  if ((y_1150X < (to_index_728X + count_729X))) {
    goto L31022;}
  else {
    memcpy((void *)((((char *) (-3 + arg3_724X))) + to_index_728X), (void *)((((char *) (-3 + arg5_726X))) + from_index_727X),count_729X);
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L18438;}}
 L31000: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_724X))) + -4))))>>8));
  goto L31004;}
 L11666: {
  if ((1 == thing_936X)) {
    goto L11669;}
  else {
    if ((5 == thing_936X)) {
      goto L11669;}
    else {
      if ((25 == thing_936X)) {
        arg3K0 = "()";
        goto L11692;}
      else {
        if ((3 == (3 & thing_936X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_936X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L11692;}
          else {
            goto L11682;}}
        else {
          goto L11682;}}}}}
 L20805: {
  count_1151X = arg0K0;
  i_1152X = arg0K1;
  offset_1153X = arg0K2;
  if ((0 == count_1151X)) {
    arg0K0 = i_1152X;
    arg0K1 = offset_1153X;
    arg0K2 = env_1108X;
    goto L20788;}
  else {
    value_1154X = *((long *) ((((char *) (-3 + env_1108X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1153X)))))<<2))));
    addr_1155X = (((char *) (-3 + new_env_762X))) + (((i_1152X)<<2));
    S48_WRITE_BARRIER(new_env_762X, addr_1155X, value_1154X);
    *((long *) addr_1155X) = value_1154X;
    arg0K0 = (-1 + count_1151X);
    arg0K1 = (1 + i_1152X);
    arg0K2 = (1 + offset_1153X);
    goto L20805;}}
 L16932: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Spending_interruptPS = 0;
    if ((s48_Spending_eventsPS)) {
      s48_Spending_interruptPS = 1;
      goto L16934;}
    else {
      goto L16934;}}
  else {
    s48_Spending_interruptPS = 1;
    goto L16934;}}
 L17012: {
  ps_error("interrupt handler is not a vector", 0);
  goto L16932;}
 L8378: {
  *((long *) (SstackS)) = channel_1113X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + channel_1113X))) + 16)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L16918;}
 L15976: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 3;
      goto check_events;
     check_events_return_3:
      v_1156X = check_events0_return_value;
      if (v_1156X) {
        arg0K0 = stack_arg_count_1116X;
        goto L16894;}
      else {
        goto L15980;}}
    else {
      arg0K0 = stack_arg_count_1116X;
      goto L16894;}}
  else {
    goto L15980;}}
 L16033: {
  okayP_1157X = arg2K0;
  key_1158X = arg0K1;
  if (okayP_1157X) {
    arg0K0 = key_1158X;
    goto L15987;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_1158X;
    goto L15987;}}
 L16398: {
  v_1159X = arg0K0;
  merged_arg0K0 = v_1159X;
  merged_arg0K1 = stack_arg_count_784X;
  merged_arg0K2 = list_args_785X;
  merged_arg0K3 = list_arg_count_786X;
  rest_list_setup_return_tag = 2;
  goto rest_list_setup;
 rest_list_setup_return_2:
  *((long *) (SstackS)) = (((final_stack_arg_count_1121X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_790X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_1077X;
  arg0K1 = (3 + final_stack_arg_count_1121X);
  goto L16361;}
 L26708: {
  v_1160X = ps_close_fd(index_1123X);
  arg0K0 = v_1160X;
  goto L26703;}
 L26703: {
  status_1161X = arg0K0;
  if ((status_1161X == NO_ERRORS)) {
    push_exception_continuationB(reason_1125X, 1);
    *((long *) (SstackS)) = arg2_999X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_1000X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}
  else {
    channel_close_error(status_1161X, index_1123X, arg2_999X);
    push_exception_continuationB(reason_1125X, 1);
    *((long *) (SstackS)) = arg2_999X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_1000X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L16665;}}
 L36458: {
  if ((3 == (3 & arg5_1010X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_1010X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_1010X))) + -4))))>>8)));
      goto L36470;}
    else {
      goto L36466;}}
  else {
    goto L36466;}}
 L36450: {
  if ((3 == (3 & arg5_1010X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg5_1010X))) + -4))))>>2))))) {
      goto L36458;}
    else {
      arg0K0 = 5;
      goto L36341;}}
  else {
    arg0K0 = 5;
    goto L36341;}}
 L36341: {
  reason_1162X = arg0K0;
  push_exception_continuationB(reason_1162X, 1);
  *((long *) (SstackS)) = arg5_1010X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1133X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1134X)<<2));
  SstackS = ((SstackS) + -4);
  if (waitP_1132X) {
    arg0K0 = 5;
    goto L36356;}
  else {
    arg0K0 = 1;
    goto L36356;}}
 L23940: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L12906: {
  arg0K0 = foo_1143X;
  arg0K1 = (*((long *) ((((char *) (-3 + foo_1143X))) + 12)));
  goto L12844;}
 L11669: {
  if ((1 == thing_936X)) {
    arg3K0 = "#f";
    goto L11692;}
  else {
    arg3K0 = "#t";
    goto L11692;}}
 L11682: {
  if ((3 == (3 & thing_936X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_936X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L11692;}
    else {
      goto L11686;}}
  else {
    goto L11686;}}
 L16934: {
  SvalS = (*((long *) ((((char *) (-3 + (Sinterrupt_handlersS)))) + (((i_1065X)<<2)))));
  obj_1163X = SvalS;
  if ((3 == (3 & obj_1163X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1163X))) + -4))))>>2))))) {
      goto L16951;}
    else {
      goto L17037;}}
  else {
    goto L17037;}}
 L15980: {
  arg1K0 = (Scode_pointerS);
  goto L18438;}
 L15987: {
  v_1164X = arg0K0;
  merged_arg0K0 = v_1164X;
  copy_stack_into_heap_return_tag = 3;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_3:
  if ((stack_slots_1115X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L15976;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L15976;}}
 L36470: {
  length_1165X = arg0K0;
  if ((length_1165X < (start_1133X + count_1134X))) {
    arg0K0 = 7;
    goto L36341;}
  else {
    got_1166X = ps_read_fd(((((*((long *) ((((char *) (-3 + x_1091X))) + 8))))>>2)), ((((char *) (-3 + arg5_1010X))) + start_1133X), count_1134X, waitP_1132X, &eofP_1167X, &pendingP_1168X, &status_1169X);
    if ((status_1169X == NO_ERRORS)) {
      if (eofP_1167X) {
        SvalS = 21;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L18438;}
      else {
        if (pendingP_1168X) {
          addr_1170X = (((char *) (-3 + x_1091X))) + 16;
          S48_WRITE_BARRIER(x_1091X, addr_1170X, 5);
          *((long *) addr_1170X) = 5;
          s48_Spending_interruptPS = 0;
          Senabled_interruptsS = 0;
          arg0K0 = 13;
          goto L36341;}
        else {
          SvalS = (((got_1166X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L18438;}}}
    else {
      push_exception_continuationB(25, 1);
      *((long *) (SstackS)) = arg5_1010X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1133X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1134X)<<2));
      SstackS = ((SstackS) + -4);
      if (waitP_1132X) {
        arg0K0 = 5;
        goto L36393;}
      else {
        arg0K0 = 1;
        goto L36393;}}}}
 L36466: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_1010X))) + -4))))>>8));
  goto L36470;}
 L36356: {
  x_1171X = arg0K0;
  *((long *) (SstackS)) = x_1171X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1091X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L16665;}
 L11686: {
  if ((3 == (3 & thing_936X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_936X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L11692;}
    else {
      arg3K0 = "???";
      goto L11692;}}
  else {
    arg3K0 = "???";
    goto L11692;}}
 L16951: {
  StemplateS = (SvalS);
  Slosing_opcodeS = (0 - i_1065X);
  arg0K0 = arg_count_1111X;
  goto L16066;}
 L17037: {
  ps_error("interrupt handler is not a closure", 1, i_1065X);
  goto L16951;}
 L36393: {
  x_1172X = arg0K0;
  *((long *) (SstackS)) = x_1172X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1091X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_1169X;
  merged_arg0K1 = key_1006X;
  get_error_string_return_tag = 4;
  goto get_error_string;
 get_error_string_return_4:
  x_1173X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_1173X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 6;
  goto L16665;}
 loseD0: {
  message_369X = merged_arg3K0;{
  why_1174X = (((*((long *) ((SstackS) + (4 + (((nargs_739X)<<2)))))))>>2);
  ps_write_string("Template UIDs: ", (stderr));
  current_template_1175X = StemplateS;
  out_1176X = stderr;
  if ((0 == (3 & (*((long *) ((((char *) (-3 + current_template_1175X))) + 4)))))) {
    if ((current_template_1175X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
      arg2K0 = 0;
      goto L10518;}
    else {
      ps_write_integer(((((*((long *) ((((char *) (-3 + current_template_1175X))) + 4))))>>2)), out_1176X);
      arg2K0 = 1;
      goto L10518;}}
  else {
    arg2K0 = 0;
    goto L10518;}}
 L10518: {
  not_firstP_1177X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_1177X;
  goto L10525;}
 L10525: {
  cont_1178X = arg0K0;
  not_firstP_1179X = arg2K1;
  if ((3 == (3 & cont_1178X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_1178X))) + -4))))>>2))))) {
      template_1180X = *((long *) ((((char *) (-3 + cont_1178X))) + 8));
      if ((0 == (3 & (*((long *) ((((char *) (-3 + template_1180X))) + 4)))))) {
        if ((template_1180X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
          arg2K0 = not_firstP_1179X;
          goto L10534;}
        else {
          if (not_firstP_1179X) {
            ps_write_string(" <- ", out_1176X);
            goto L10573;}
          else {
            goto L10573;}}}
      else {
        arg2K0 = not_firstP_1179X;
        goto L10534;}}
    else {
      goto L16684;}}
  else {
    goto L16684;}}
 L10534: {
  v_1181X = arg2K0;
  arg0K0 = (*((long *) (((char *) (-3 + cont_1178X)))));
  arg2K1 = v_1181X;
  goto L10525;}
 L10573: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_1180X))) + 4))))>>2)), out_1176X);
  arg2K0 = 1;
  goto L10534;}
 L16684: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_1174X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_739X)<<2))))))))))))) {
      ps_error(message_369X, 3, opcode_740X, why_1174X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_739X)<<2)))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L16708;}}
  else {
    goto L16708;}}
 L16708: {
  ps_error(message_369X, 2, opcode_740X, why_1174X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L39788;}
 L39788: {
  type_1182X = s48_get_next_event(&channel_1183X, &status_1184X);
  if ((type_1182X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L15869;}
    else {
      arg0K0 = 1;
      goto L15869;}}
  else {
    if ((type_1182X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L15869;}
    else {
      if ((type_1182X == IO_COMPLETION_EVENT)) {
        enqueue_channelB(channel_1183X, status_1184X);
        arg0K0 = 8;
        goto L15869;}
      else {
        if ((type_1182X == OS_SIGNAL_EVENT)) {
          arg0K0 = 16;
          goto L15869;}
        else {
          if ((type_1182X == NO_EVENT)) {
            arg0K0 = 0;
            goto L15869;}
          else {
            if ((type_1182X == ERROR_EVENT)) {
              ps_write_string("OS error while getting event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              ps_write_string((ps_error_string(status_1184X)), (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L15869;}
            else {
              ps_write_string("unknown type of event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L15869;}}}}}}}
 L15869: {
  v_1185X = arg0K0;
  Spending_interruptsS = (v_1185X | (Spending_interruptsS));
  if ((type_1182X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L15896;}
      else {
        goto L15896;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L15896;}}
  else {
    goto L39788;}}
 L15896: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      goto L39788;}
    else {
      check_events0_return_value = 1;
      goto check_events_return;}}
  else {
    check_events0_return_value = 0;
    goto check_events_return;}}
 check_events_return:
  switch (check_events_return_tag) {
  case 0: goto check_events_return_0;
  case 1: goto check_events_return_1;
  case 2: goto check_events_return_2;
  default: goto check_events_return_3;
  }}

 rest_list_setup: {
  wants_stack_args_365X = merged_arg0K0;
  stack_arg_count_366X = merged_arg0K1;
  list_args_367X = merged_arg0K2;
  list_arg_count_368X = merged_arg0K3;{
  if ((stack_arg_count_366X == wants_stack_args_365X)) {
    merged_arg0K0 = list_args_367X;
    merged_arg0K1 = list_arg_count_368X;
    copy_listS_return_tag = 1;
    goto copy_listS;
   copy_listS_return_1:
    x_1186X = copy_listS0_return_value;
    *((long *) (SstackS)) = x_1186X;
    SstackS = ((SstackS) + -4);
    goto rest_list_setup_return;}
  else {
    if ((stack_arg_count_366X < wants_stack_args_365X)) {
      count_1187X = wants_stack_args_365X - stack_arg_count_366X;
      merged_arg0K0 = list_args_367X;
      merged_arg0K1 = count_1187X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      v_1188X = push_list0_return_value;
      merged_arg0K0 = v_1188X;
      merged_arg0K1 = (list_arg_count_368X - count_1187X);
      copy_listS_return_tag = 2;
      goto copy_listS;
     copy_listS_return_2:
      x_1189X = copy_listS0_return_value;
      *((long *) (SstackS)) = x_1189X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setup_return;}
    else {
      merged_arg0K0 = list_args_367X;
      merged_arg0K1 = list_arg_count_368X;
      copy_listS_return_tag = 3;
      goto copy_listS;
     copy_listS_return_3:
      v_1190X = copy_listS0_return_value;
      merged_arg0K0 = v_1190X;
      merged_arg0K1 = (stack_arg_count_366X - wants_stack_args_365X);
      pop_args_GlistS_return_tag = 5;
      goto pop_args_GlistS;
     pop_args_GlistS_return_5:
      x_1191X = pop_args_GlistS0_return_value;
      *((long *) (SstackS)) = x_1191X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setup_return;}}}
 rest_list_setup_return:
  switch (rest_list_setup_return_tag) {
  case 0: goto rest_list_setup_return_0;
  case 1: goto rest_list_setup_return_1;
  default: goto rest_list_setup_return_2;
  }}

 push_list: {
  list_363X = merged_arg0K0;
  count_364X = merged_arg0K1;{
  *((long *) (SstackS)) = list_363X;
  SstackS = ((SstackS) + -4);
  if ((count_364X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L15285;}
  else {
    space_1192X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
    v_1193X = AVAILABLEp(space_1192X);
    if (v_1193X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L15369;}
    else {
      collect_saving_temps(1, 1, &temp1_1194X);
      v_1195X = AVAILABLEp(space_1192X);
      if (v_1195X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L15369;}
      else {
        arg2K0 = 0;
        arg0K1 = 0;
        goto L15369;}}}}
 L15285: {
  SstackS = ((SstackS) + 4);
  list_1196X = *((long *) (SstackS));
  arg0K0 = count_364X;
  arg0K1 = list_1196X;
  goto L15294;}
 L15369: {
  okayP_1197X = arg2K0;
  key_1198X = arg0K1;
  if (okayP_1197X) {
    arg0K0 = key_1198X;
    goto L15320;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_1198X;
    goto L15320;}}
 L15294: {
  i_1199X = arg0K0;
  l_1200X = arg0K1;
  if ((0 < i_1199X)) {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1200X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (-1 + i_1199X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1200X))) + 4)));
    goto L15294;}
  else {
    push_list0_return_value = l_1200X;
    goto push_list_return;}}
 L15320: {
  v_1201X = arg0K0;
  merged_arg0K0 = v_1201X;
  copy_stack_into_heap_return_tag = 4;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_4:
  if ((count_364X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L15285;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L15285;}}
 push_list_return:
  switch (push_list_return_tag) {
  case 0: goto push_list_return_0;
  case 1: goto push_list_return_1;
  case 2: goto push_list_return_2;
  default: goto push_list_return_3;
  }}

 copy_stack_into_heap: {
  key_362X = merged_arg0K0;{
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L13514;}
 L13514: {
  p_1202X = arg1K0;
  i_1203X = arg0K1;
  if ((2 == (3 & (*((long *) p_1202X))))) {
    if ((22 == (*((long *) p_1202X)))) {
      arg0K0 = (-1 + i_1203X);
      goto L13501;}
    else {
      arg0K0 = i_1203X;
      goto L13501;}}
  else {
    arg1K0 = (p_1202X + 4);
    arg0K1 = (1 + i_1203X);
    goto L13514;}}
 L13501: {
  arg_count_1204X = arg0K0;
  top_1205X = SstackS;
  if ((1 == (ScontS))) {
    goto L13506;}
  else {
    merged_arg0K0 = key_362X;
    merged_arg0K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L13506;}}
 L13506: {
  stob_1206X = ScontS;
  arg1K0 = ((((char *) (-3 + stob_1206X))) + -8);
  arg1K1 = (top_1205X + (((arg_count_1204X)<<2)));
  goto L13539;}
 L13539: {
  loc_1207X = arg1K0;
  arg_1208X = arg1K1;
  if ((top_1205X < arg_1208X)) {
    *((long *) loc_1207X) = (*((long *) arg_1208X));
    arg1K0 = (loc_1207X + -4);
    arg1K1 = (arg_1208X + -4);
    goto L13539;}
  else {
    SstackS = loc_1207X;
    goto copy_stack_into_heap_return;}}
 copy_stack_into_heap_return:
  switch (copy_stack_into_heap_return_tag) {
  case 0: goto copy_stack_into_heap_return_0;
  case 1: goto copy_stack_into_heap_return_1;
  case 2: goto copy_stack_into_heap_return_2;
  case 3: goto copy_stack_into_heap_return_3;
  default: goto copy_stack_into_heap_return_4;
  }}

 really_preserve_continuation: {
  key_360X = merged_arg0K0;
  reason_361X = merged_arg0K1;{
  p_1209X = SenvS;
  if ((3 == (3 & p_1209X))) {
    if ((p_1209X < (((long) (Sstack_beginS))))) {
      goto L12690;}
    else {
      if (((((long) (Sstack_endS))) < p_1209X)) {
        goto L12690;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_360X;
        merged_arg0K3 = reason_361X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_1210X = save_env_in_heap0_return_value;
        SenvS = v_1210X;
        goto L12690;}}}
  else {
    goto L12690;}}
 L12690: {
  end_1211X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L12643;}
 L12643: {
  cont_1212X = arg0K0;
  previous_1213X = arg0K1;
  if ((cont_1212X == (Sbottom_of_stackS))) {
    *((long *) (((char *) (-3 + previous_1213X)))) = end_1211X;
    ScontS = (Sbottom_of_stackS);
    really_preserve_continuation0_return_value = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
    goto really_preserve_continuation_return;}
  else {
    p_1214X = *((long *) ((((char *) (-3 + cont_1212X))) + 12));
    if ((3 == (3 & p_1214X))) {
      if ((p_1214X < (((long) (Sstack_beginS))))) {
        goto L12661;}
      else {
        if (((((long) (Sstack_endS))) < p_1214X)) {
          goto L12661;}
        else {
          merged_arg0K0 = (*((long *) ((((char *) (-3 + cont_1212X))) + 12)));
          merged_arg0K1 = cont_1212X;
          merged_arg0K2 = key_360X;
          merged_arg0K3 = reason_361X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L12661;}}}
    else {
      goto L12661;}}}
 L12661: {
  header_1215X = *((long *) ((((char *) (-3 + cont_1212X))) + -4));
  addr_1216X = ALLOCATE_SPACE((31 & (((header_1215X)>>2))), (4 + ((long)(((unsigned long)header_1215X)>>8))));
  data_addr_1217X = addr_1216X + 4;
  *((long *) addr_1216X) = header_1215X;
  memcpy((void *)data_addr_1217X, (void *)(((char *) (-3 + cont_1212X))),((long)(((unsigned long)header_1215X)>>8)));
  new_1218X = 3 + (((long) data_addr_1217X));
  *((long *) (((char *) (-3 + previous_1213X)))) = new_1218X;
  arg0K0 = (*((long *) (((char *) (-3 + new_1218X)))));
  arg0K1 = new_1218X;
  goto L12643;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 copy_env: {
  env_359X = merged_arg0K0;{
  header_1219X = *((long *) ((((char *) (-3 + env_359X))) + -4));
  addr_1220X = ALLOCATE_SPACE((31 & (((header_1219X)>>2))), (4 + ((long)(((unsigned long)header_1219X)>>8))));
  data_addr_1221X = addr_1220X + 4;
  *((long *) addr_1220X) = header_1219X;
  memcpy((void *)data_addr_1221X, (void *)(((char *) (-3 + env_359X))),((long)(((unsigned long)header_1219X)>>8)));
  new_1222X = 3 + (((long) data_addr_1221X));
  addr_1223X = ((char *) (-3 + env_359X));
  S48_WRITE_BARRIER(env_359X, addr_1223X, 22);
  *((long *) addr_1223X) = 22;
  *((long *) ((((char *) (-3 + env_359X))) + -4)) = new_1222X;
  copy_env0_return_value = new_1222X;
  goto copy_env_return;}
 copy_env_return:
  switch (copy_env_return_tag) {
  case 0: goto copy_env_return_0;
  default: goto copy_env_return_1;
  }}

 save_env_in_heap: {
  env_355X = merged_arg0K0;
  cont_356X = merged_arg0K1;
  key_357X = merged_arg0K2;
  reason_358X = merged_arg0K3;{
  merged_arg0K0 = env_355X;
  merged_arg0K1 = key_357X;
  merged_arg0K2 = reason_358X;
  copy_env_return_tag = 0;
  goto copy_env;
 copy_env_return_0:
  top_1224X = copy_env0_return_value;
  arg0K0 = top_1224X;
  goto L11922;}
 L11922: {
  env_1225X = arg0K0;
  p_1226X = *((long *) (((char *) (-3 + env_1225X))));
  if ((3 == (3 & p_1226X))) {
    if ((p_1226X < (((long) (Sstack_beginS))))) {
      goto L11941;}
    else {
      if (((((long) (Sstack_endS))) < p_1226X)) {
        goto L11941;}
      else {
        merged_arg0K0 = (*((long *) (((char *) (-3 + env_1225X)))));
        merged_arg0K1 = key_357X;
        merged_arg0K2 = reason_358X;
        copy_env_return_tag = 1;
        goto copy_env;
       copy_env_return_1:
        new_1227X = copy_env0_return_value;
        addr_1228X = ((char *) (-3 + env_1225X));
        S48_WRITE_BARRIER(env_1225X, addr_1228X, new_1227X);
        *((long *) addr_1228X) = new_1227X;
        arg0K0 = new_1227X;
        goto L11922;}}}
  else {
    goto L11941;}}
 L11941: {
  arg0K0 = cont_356X;
  goto L11945;}
 L11945: {
  cont_1229X = arg0K0;
  env_1230X = *((long *) ((((char *) (-3 + cont_1229X))) + 12));
  if ((3 == (3 & env_1230X))) {
    if ((3 == (3 & (*((long *) ((((char *) (-3 + env_1230X))) + -4)))))) {
      *((long *) ((((char *) (-3 + cont_1229X))) + 12)) = (*((long *) ((((char *) (-3 + env_1230X))) + -4)));
      arg0K0 = (*((long *) (((char *) (-3 + cont_1229X)))));
      goto L11945;}
    else {
      save_env_in_heap0_return_value = top_1224X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = top_1224X;
    goto save_env_in_heap_return;}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  default: goto save_env_in_heap_return_2;
  }}

 pop_args_GlistS: {
  start_353X = merged_arg0K0;
  count_354X = merged_arg0K1;{
  space_1231X = 3 * count_354X;
  v_1232X = AVAILABLEp(space_1231X);
  if (v_1232X) {
    arg2K0 = 1;
    arg0K1 = start_353X;
    goto L14260;}
  else {
    temp0_1233X = collect_saving_temps(start_353X, 1, &temp1_1234X);
    v_1235X = AVAILABLEp(space_1231X);
    if (v_1235X) {
      arg2K0 = 1;
      arg0K1 = temp0_1233X;
      goto L14260;}
    else {
      arg2K0 = 0;
      arg0K1 = temp0_1233X;
      goto L14260;}}}
 L14260: {
  okayP_1236X = arg2K0;
  temp_1237X = arg0K1;
  if (okayP_1236X) {
    arg0K0 = temp_1237X;
    goto L14234;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_1237X;
    goto L14234;}}
 L14234: {
  start_1238X = arg0K0;
  arg0K0 = start_1238X;
  arg0K1 = count_354X;
  goto L14243;}
 L14243: {
  args_1239X = arg0K0;
  count_1240X = arg0K1;
  if ((0 == count_1240X)) {
    pop_args_GlistS0_return_value = args_1239X;
    goto pop_args_GlistS_return;}
  else {
    SstackS = ((SstackS) + 4);
    a_1241X = *((long *) (SstackS));
    addr_1242X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_1242X) = 2050;
    x_1243X = 3 + (((long) (addr_1242X + 4)));
    *((long *) (((char *) (-3 + x_1243X)))) = a_1241X;
    *((long *) ((((char *) (-3 + x_1243X))) + 4)) = args_1239X;
    arg0K0 = x_1243X;
    arg0K1 = (-1 + count_1240X);
    goto L14243;}}
 pop_args_GlistS_return:
  switch (pop_args_GlistS_return_tag) {
  case 0: goto pop_args_GlistS_return_0;
  case 1: goto pop_args_GlistS_return_1;
  case 2: goto pop_args_GlistS_return_2;
  case 3: goto pop_args_GlistS_return_3;
  case 4: goto pop_args_GlistS_return_4;
  default: goto pop_args_GlistS_return_5;
  }}

 copy_listS: {
  list_351X = merged_arg0K0;
  length_352X = merged_arg0K1;{
  if ((0 == length_352X)) {
    copy_listS0_return_value = 25;
    goto copy_listS_return;}
  else {
    space_1244X = 3 * length_352X;
    v_1245X = AVAILABLEp(space_1244X);
    if (v_1245X) {
      arg2K0 = 1;
      arg0K1 = list_351X;
      goto L14139;}
    else {
      temp0_1246X = collect_saving_temps(list_351X, 1, &temp1_1247X);
      v_1248X = AVAILABLEp(space_1244X);
      if (v_1248X) {
        arg2K0 = 1;
        arg0K1 = temp0_1246X;
        goto L14139;}
      else {
        arg2K0 = 0;
        arg0K1 = temp0_1246X;
        goto L14139;}}}}
 L14139: {
  okayP_1249X = arg2K0;
  temp_1250X = arg0K1;
  if (okayP_1249X) {
    arg0K0 = temp_1250X;
    goto L14097;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_1250X;
    goto L14097;}}
 L14097: {
  list_1251X = arg0K0;
  a_1252X = *((long *) (((char *) (-3 + list_1251X))));
  addr_1253X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_1253X) = 2050;
  x_1254X = 3 + (((long) (addr_1253X + 4)));
  *((long *) (((char *) (-3 + x_1254X)))) = a_1252X;
  *((long *) ((((char *) (-3 + x_1254X))) + 4)) = 25;
  arg0K0 = (*((long *) ((((char *) (-3 + list_1251X))) + 4)));
  arg0K1 = x_1254X;
  goto L14112;}
 L14112: {
  l_1255X = arg0K0;
  last_1256X = arg0K1;
  if ((25 == l_1255X)) {
    copy_listS0_return_value = x_1254X;
    goto copy_listS_return;}
  else {
    a_1257X = *((long *) (((char *) (-3 + l_1255X))));
    addr_1258X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_1258X) = 2050;
    x_1259X = 3 + (((long) (addr_1258X + 4)));
    *((long *) (((char *) (-3 + x_1259X)))) = a_1257X;
    *((long *) ((((char *) (-3 + x_1259X))) + 4)) = 25;
    addr_1260X = (((char *) (-3 + last_1256X))) + 4;
    S48_WRITE_BARRIER(last_1256X, addr_1260X, x_1259X);
    *((long *) addr_1260X) = x_1259X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_1255X))) + 4)));
    arg0K1 = x_1259X;
    goto L14112;}}
 copy_listS_return:
  switch (copy_listS_return_tag) {
  case 0: goto copy_listS_return_0;
  case 1: goto copy_listS_return_1;
  case 2: goto copy_listS_return_2;
  default: goto copy_listS_return_3;
  }}

 copy_continuation_from_heapB: {
  cont_350X = merged_arg0K0;{
  top_1261X = (((char *) (-3 + (Sbottom_of_stackS)))) + (-8 - (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_350X))) + -4))))>>8)))));
  new_cont_1262X = 3 + (((long) (top_1261X + 4)));
  SstackS = (top_1261X + -4);
  ScontS = new_cont_1262X;
  v_1263X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_350X))) + -4))))>>8))))>>2);
  memcpy((void *)top_1261X, (void *)((((char *) (-3 + cont_350X))) + -4),(4 + (((v_1263X)<<2))));
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = (*((long *) (((char *) (-3 + new_cont_1262X)))));
  *((long *) (((char *) (-3 + new_cont_1262X)))) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_1262X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  default: goto copy_continuation_from_heapB_return_1;
  }}

 get_current_port: {
  marker_349X = merged_arg0K0;{
  thread_1264X = Scurrent_threadS;
  if ((3 == (3 & thread_1264X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + thread_1264X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_1264X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_1264X))) + 4)));
        goto L10214;}
      else {
        goto L10264;}}
    else {
      goto L10264;}}
  else {
    goto L10264;}}
 L10214: {
  env_1265X = arg0K0;
  if ((3 == (3 & env_1265X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_1265X))) + -4))))>>2))))) {
      obj_1266X = *((long *) (((char *) (-3 + env_1265X))));
      if ((3 == (3 & obj_1266X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_1266X))) + -4))))>>2))))) {
          if ((marker_349X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_1265X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_1265X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_1265X))) + 4)));
            goto L10214;}}
        else {
          goto L10286;}}
      else {
        goto L10286;}}
    else {
      goto L10286;}}
  else {
    goto L10286;}}
 L10264: {
  ps_error("current thread is not a record", 0);
  get_current_port0_return_value = v_1267X;
  goto get_current_port_return;}
 L10286: {
  if ((25 == env_1265X)) {
    if (((((marker_349X)>>2)) == 1)) {
      arg3K0 = "dynamic environment doesn't have current-output-port";
      goto L10240;}
    else {
      arg3K0 = "dynamic environment doesn't have current-input-port";
      goto L10240;}}
  else {
    ps_error("dynamic environment is not a proper list", 0);
    get_current_port0_return_value = v_1268X;
    goto get_current_port_return;}}
 L10240: {
  v_1269X = arg3K0;
  ps_error(v_1269X, 0);
  get_current_port0_return_value = v_1270X;
  goto get_current_port_return;}
 get_current_port_return:
  switch (get_current_port_return_tag) {
  case 0: goto get_current_port_return_0;
  case 1: goto get_current_port_return_1;
  default: goto get_current_port_return_2;
  }}

 okay_argument_list: {
  list_348X = merged_arg0K0;{
  arg0K0 = list_348X;
  arg0K1 = 0;
  arg0K2 = list_348X;
  arg2K3 = 0;
  goto L10081;}
 L10081: {
  fast_1271X = arg0K0;
  len_1272X = arg0K1;
  slow_1273X = arg0K2;
  move_slowP_1274X = arg2K3;
  if ((25 == fast_1271X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_1272X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_1271X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_1271X))) + -4))))>>2))))) {
        if (move_slowP_1274X) {
          if ((fast_1271X == slow_1273X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_1271X))) + 4)));
            arg0K1 = (1 + len_1272X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_1273X))) + 4)));
            arg2K3 = 0;
            goto L10081;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_1271X))) + 4)));
          arg0K1 = (1 + len_1272X);
          arg0K2 = slow_1273X;
          arg2K3 = 1;
          goto L10081;}}
      else {
        okay_argument_list0_return_value = 0;
        okay_argument_list1_return_value = 0;
        goto okay_argument_list_return;}}
    else {
      okay_argument_list0_return_value = 0;
      okay_argument_list1_return_value = 0;
      goto okay_argument_list_return;}}}
 okay_argument_list_return:
  switch (okay_argument_list_return_tag) {
  case 0: goto okay_argument_list_return_0;
  default: goto okay_argument_list_return_1;
  }}

 get_error_string: {
  status_347X = merged_arg0K0;{
  string_1275X = ps_error_string(status_347X);
  x_1276X = strlen((char *) string_1275X);
  if ((x_1276X < 256)) {
    arg0K0 = x_1276X;
    goto L9589;}
  else {
    arg0K0 = 256;
    goto L9589;}}
 L9589: {
  len_1277X = arg0K0;
  len_1278X = 1 + len_1277X;
  addr_1279X = ALLOCATE_SPACE(16, (4 + len_1278X));
  *((long *) addr_1279X) = (66 + (((len_1278X)<<8)));
  string_1280X = 3 + (((long) (addr_1279X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1280X))) + len_1277X)) = 0;
  arg0K0 = 0;
  goto L9599;}
 L9599: {
  i_1281X = arg0K0;
  if ((i_1281X == len_1277X)) {
    get_error_string0_return_value = string_1280X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1280X))) + i_1281X)) = ((*(string_1275X + i_1281X)));
    arg0K0 = (1 + i_1281X);
    goto L9599;}}
 get_error_string_return:
  switch (get_error_string_return_tag) {
  case 0: goto get_error_string_return_0;
  case 1: goto get_error_string_return_1;
  case 2: goto get_error_string_return_2;
  case 3: goto get_error_string_return_3;
  default: goto get_error_string_return_4;
  }}

 pop_continuationB: {
{ cont_1282X = ScontS;
  tem_1283X = *((long *) ((((char *) (-3 + cont_1282X))) + 8));
  pc_1284X = *((long *) ((((char *) (-3 + cont_1282X))) + 4));
  StemplateS = tem_1283X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1283X)))))))) + (((pc_1284X)>>2)));
  SenvS = (*((long *) ((((char *) (-3 + cont_1282X))) + 12)));
  ScontS = (*((long *) (((char *) (-3 + cont_1282X)))));
  SstackS = ((((char *) (-3 + cont_1282X))) + 12);
  goto pop_continuationB_return;}
 pop_continuationB_return:
  switch (pop_continuationB_return_tag) {
  case 0: goto pop_continuationB_return_0;
  case 1: goto pop_continuationB_return_1;
  default: goto pop_continuationB_return_2;
  }}

}
long s48_call_startup_procedure(char **startup_vector_1285X, long startup_vector_length_1286X)
{
  char arg2K0;
  long arg0K1;
  long arg0K0;
  long v_1335X;
  long x_1334X;
  long x_1333X;
  char * addr_1332X;
  long b_1331X;
  long channel_1330X;
  long x_1329X;
  char * addr_1328X;
  long b_1327X;
  long channel_1326X;
  long x_1325X;
  char * addr_1324X;
  long b_1323X;
  long channel_1322X;
  long key_1321X;
  long key_1320X;
  char okayP_1319X;
  char v_1318X;
  long temp1_1317X;
  char v_1316X;
  long space_1315X;
  long i_1314X;
  long length_1313X;
  long *v_1312X;
  long v_1311X;
  long v_1310X;
  long y_1309X;
  long x_1308X;
  long v_1307X;
  long x_1306X;
  long y_1305X;
  char * addr_1304X;
  long value_1303X;
  long x_1302X;
  long y_1301X;
  long i_1300X;
  long vector_1299X;
  char * addr_1298X;
  long len_1297X;
  long key_1296X;
  long key_1295X;
  char okayP_1294X;
  char v_1293X;
  long temp1_1292X;
  char v_1291X;
  long space_1290X;
  long size_1289X;
  long i_1288X;
  long tem_1287X;
 {  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  SenvS = 13;
  tem_1287X = Sinterrupt_templateS;
  StemplateS = tem_1287X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1287X))))))));
  SvalS = 13;
  Scurrent_threadS = 25;
  Ssession_dataS = 25;
  Sexception_handlersS = 25;
  Sinterrupt_handlersS = 25;
  Senabled_interruptsS = 0;
  Sfinalizer_alistS = 25;
  Sfinalize_theseS = 25;
  Spending_interruptsS = 0;
  s48_Spending_interruptPS = 0;
  Sinterrupted_templateS = 1;
  arg0K0 = 0;
  arg0K1 = 0;
  goto L15603;}
 L15603: {
  i_1288X = arg0K0;
  size_1289X = arg0K1;
  if ((i_1288X == startup_vector_length_1286X)) {
    space_1290X = 1 + (size_1289X + startup_vector_length_1286X);
    v_1291X = AVAILABLEp(space_1290X);
    if (v_1291X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L15640;}
    else {
      collect_saving_temps(1, 1, &temp1_1292X);
      v_1293X = AVAILABLEp(space_1290X);
      if (v_1293X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L15640;}
      else {
        arg2K0 = 0;
        arg0K1 = 0;
        goto L15640;}}}
  else {
    arg0K0 = (1 + i_1288X);
    arg0K1 = (1 + (size_1289X + ((((4 + (strlen((char *) (*(startup_vector_1285X + i_1288X))))))>>2))));
    goto L15603;}}
 L15640: {
  okayP_1294X = arg2K0;
  key_1295X = arg0K1;
  if (okayP_1294X) {
    arg0K0 = key_1295X;
    goto L15618;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_1295X;
    goto L15618;}}
 L15618: {
  key_1296X = arg0K0;
  len_1297X = ((startup_vector_length_1286X)<<2);
  addr_1298X = ALLOCATE_SPACE(2, (4 + len_1297X));
  *((long *) addr_1298X) = (10 + (((len_1297X)<<8)));
  vector_1299X = 3 + (((long) (addr_1298X + 4)));
  arg0K0 = 0;
  goto L15676;}
 L15676: {
  i_1300X = arg0K0;
  if ((i_1300X == startup_vector_length_1286X)) {
    *((long *) (SstackS)) = vector_1299X;
    SstackS = ((SstackS) + -4);
    y_1301X = fileno((stderr));
    x_1302X = fileno((stdout));
    if ((x_1302X < y_1301X)) {
      arg0K0 = y_1301X;
      goto L14512;}
    else {
      arg0K0 = x_1302X;
      goto L14512;}}
  else {
    value_1303X = enter_string((*(startup_vector_1285X + i_1300X)), key_1296X);
    addr_1304X = (((char *) (-3 + vector_1299X))) + (((i_1300X)<<2));
    S48_WRITE_BARRIER(vector_1299X, addr_1304X, value_1303X);
    *((long *) addr_1304X) = value_1303X;
    arg0K0 = (1 + i_1300X);
    goto L15676;}}
 L14512: {
  y_1305X = arg0K0;
  x_1306X = fileno((stdin));
  if ((x_1306X < y_1305X)) {
    arg0K0 = y_1305X;
    goto L14514;}
  else {
    arg0K0 = x_1306X;
    goto L14514;}}
 L14514: {
  v_1307X = arg0K0;
  x_1308X = Snumber_of_channelsS;
  y_1309X = 1 + v_1307X;
  if ((x_1308X < y_1309X)) {
    arg0K0 = y_1309X;
    goto L14516;}
  else {
    arg0K0 = x_1308X;
    goto L14516;}}
 L14516: {
  v_1310X = arg0K0;
  Snumber_of_channelsS = v_1310X;
  v_1311X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L14538;}
  else {
    goto L14538;}}
 L14538: {
  v_1312X = Svm_channelsS;
  length_1313X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L14586;}
 L14586: {
  i_1314X = arg0K0;
  if ((i_1314X < length_1313X)) {
    *(v_1312X + i_1314X) = 1;
    arg0K0 = (1 + i_1314X);
    goto L14586;}
  else {
    space_1315X = 3 * (7 + ((((4 + (strlen((char *) "standard output"))))>>2)));
    v_1316X = AVAILABLEp(space_1315X);
    if (v_1316X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L14600;}
    else {
      collect_saving_temps(1, 1, &temp1_1317X);
      v_1318X = AVAILABLEp(space_1315X);
      if (v_1318X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L14600;}
      else {
        arg2K0 = 0;
        arg0K1 = 0;
        goto L14600;}}}}
 L14600: {
  okayP_1319X = arg2K0;
  key_1320X = arg0K1;
  if (okayP_1319X) {
    arg0K0 = key_1320X;
    goto L14544;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_1320X;
    goto L14544;}}
 L14544: {
  key_1321X = arg0K0;
  channel_1322X = fileno((stdin));
  b_1323X = enter_string("standard input", key_1321X);
  addr_1324X = ALLOCATE_SPACE(5, 24);
  *((long *) addr_1324X) = 5142;
  x_1325X = 3 + (((long) (addr_1324X + 4)));
  *((long *) (((char *) (-3 + x_1325X)))) = 4;
  *((long *) ((((char *) (-3 + x_1325X))) + 4)) = b_1323X;
  *((long *) ((((char *) (-3 + x_1325X))) + 8)) = (((channel_1322X)<<2));
  *((long *) ((((char *) (-3 + x_1325X))) + 12)) = 1;
  *((long *) ((((char *) (-3 + x_1325X))) + 16)) = 1;
  *((Svm_channelsS) + channel_1322X) = x_1325X;
  channel_1326X = fileno((stderr));
  b_1327X = enter_string("standard error", key_1321X);
  addr_1328X = ALLOCATE_SPACE(5, 24);
  *((long *) addr_1328X) = 5142;
  x_1329X = 3 + (((long) (addr_1328X + 4)));
  *((long *) (((char *) (-3 + x_1329X)))) = 8;
  *((long *) ((((char *) (-3 + x_1329X))) + 4)) = b_1327X;
  *((long *) ((((char *) (-3 + x_1329X))) + 8)) = (((channel_1326X)<<2));
  *((long *) ((((char *) (-3 + x_1329X))) + 12)) = 1;
  *((long *) ((((char *) (-3 + x_1329X))) + 16)) = 1;
  *((Svm_channelsS) + channel_1326X) = x_1329X;
  channel_1330X = fileno((stdout));
  b_1331X = enter_string("standard output", key_1321X);
  addr_1332X = ALLOCATE_SPACE(5, 24);
  *((long *) addr_1332X) = 5142;
  x_1333X = 3 + (((long) (addr_1332X + 4)));
  *((long *) (((char *) (-3 + x_1333X)))) = 8;
  *((long *) ((((char *) (-3 + x_1333X))) + 4)) = b_1331X;
  *((long *) ((((char *) (-3 + x_1333X))) + 8)) = (((channel_1330X)<<2));
  *((long *) ((((char *) (-3 + x_1333X))) + 12)) = 1;
  *((long *) ((((char *) (-3 + x_1333X))) + 16)) = 1;
  *((Svm_channelsS) + channel_1330X) = x_1333X;
  *((long *) (SstackS)) = x_1325X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1333X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1329X;
  SstackS = ((SstackS) + -4);
  x_1334X = s48_resumer_records();
  *((long *) (SstackS)) = x_1334X;
  SstackS = ((SstackS) + -4);
  s48_initialization_completeB();
  v_1335X = s48_startup_procedure();
  return s48_restart(v_1335X, 5);}
}void
s48_init(void)
{
Snumber_of_channelsS = 100;
Spending_channels_headS = 1;
Spending_channels_tailS = 1;
Sstack_warningPS = 1;
Slosing_opcodeS = 0;
Sos_signal_typeS = 1;
Sos_signal_argumentS = 1;
Sexternal_exceptionPS = 0;
Sexternal_root_stackS = NULL;
Sexternal_root_stack_baseS = NULL;
Spermanent_external_rootsS = NULL;
Sgc_root_procS = HtopD8861;
Spost_gc_cleanupS = HtopD8854;
s48_Scallback_return_stack_blockS = 1;
s48_Spending_eventsPS = 0;
}
