#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm-prelude.h"

static char add_more_channels(long);
static void enqueue_channelB(long, long);
static long close_channelB(long);
static void push_continuationB(char *, long);
static void push_exception_continuationB(long, long);
static long add_log_entryAgc(long, long, long, long, long, char);
static long enter_bignum(char *);
static long enter_stringAgc(char*);
static long channel_close_error(long, long, long);
static long make_registered_channel(long, long, long, long, long*);
static long Hlookup2670(long, long, long);
static long Hlookup2651(long, long, long);
static void HtopD10192(void);
static void HtopD10203(void);
void s48_set_extension_valueB(long);
void s48_note_event(void);
long s48_channel_count(void);
long *s48_channels(void);
void s48_set_os_signal(long, long);
void s48_reset_interruptsB(void);
long s48_imported_bindings(void);
long s48_exported_bindings(void);
long s48_symbol_table(void);
char * s48_set_gc_roots_baseB(void);
char s48_release_gc_roots_baseB(char *);
void s48_reset_external_rootsB(void);
void s48_disable_interruptsB(void);
void s48_push_gc_rootsB(char *, long);
void s48_register_gc_rootB(char *);
void s48_stack_setB(long, long);
long s48_stack_ref(long);
void s48_push(long);
long s48_resetup_external_exception(long, long);
void s48_enable_interruptsB(void);
char s48_pop_gc_rootsB(void);
char * s48_shorten_bignum(char *, long);
long s48_allocate_bignum(long);
long s48_set_channel_os_index(long, long);
void s48_setup_external_exception(long, long);
void s48_close_channel(long);
char s48_warn_about_undefined_imported_bindings(void);
long s48_really_add_channel(long, long, long);
long s48_enter_integer(long);
long s48_get_imported_binding(char*);
void s48_define_exported_binding(char*, long);
void s48_initialize_vm(char *, long);
void s48_post_gc_cleanup(void);
void s48_gc_root(void);
long s48_restart(long, long);
long s48_call_startup_procedure(char**, long);
static long Snumber_of_channelsS;
static long *Svm_channelsS;
static long Spending_channels_headS;
static long Spending_channels_tailS;
static long Stemp0S;
static long Stemp1S;
static char * Sstack_beginS;
static char * Sstack_endS;
static long Sbottom_of_stackS;
static char Sstack_warningPS;
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
static long Sempty_logS;
static void (*Spost_gc_cleanupS)(void);
static void (*Sgc_root_procS)(void);
char * SstackS;
char * Sstack_limitS;
long ScontS;
long SenvS;
long StemplateS;
char * Scode_pointerS;
long SvalS;
char s48_Spending_interruptPS;
long s48_Sextension_valueS;
long s48_Scallback_return_stack_blockS;
char s48_Spending_eventsPS;

static char add_more_channels(long index_4X)
{
  long arg0K0;
  long i_10X;
  long i_9X;
  long *new_vm_channels_8X;
  long new_count_7X;
  long y_6X;
  long x_5X;
 {  x_5X = 1 + index_4X;
  y_6X = 8 + (Snumber_of_channelsS);
  if ((x_5X < y_6X)) {
    arg0K0 = y_6X;
    goto L2916;}
  else {
    arg0K0 = x_5X;
    goto L2916;}}
 L2916: {
  new_count_7X = arg0K0;
  new_vm_channels_8X = (long*)malloc(sizeof(long) * new_count_7X);
  if ((NULL == new_vm_channels_8X)) {
    return 0;}
  else {
    arg0K0 = 0;
    goto L2928;}}
 L2928: {
  i_9X = arg0K0;
  if ((i_9X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L3001;}
  else {
    *(new_vm_channels_8X + i_9X) = (*((Svm_channelsS) + i_9X));
    arg0K0 = (1 + i_9X);
    goto L2928;}}
 L3001: {
  i_10X = arg0K0;
  if ((i_10X == new_count_7X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_8X;
    Snumber_of_channelsS = new_count_7X;
    return 1;}
  else {
    *(new_vm_channels_8X + i_10X) = 1;
    arg0K0 = (1 + i_10X);
    goto L3001;}}
}
static void enqueue_channelB(long index_11X, long status_12X)
{
  char * addr_17X;
  long x_16X;
  char * addr_15X;
  long val_14X;
  long channel_13X;
 {  channel_13X = *((Svm_channelsS) + index_11X);
  val_14X = ((status_12X)<<2);
  addr_15X = (((char *) (-3 + channel_13X))) + 16;
  S48_WRITE_BARRIER(channel_13X, addr_15X, val_14X);
  *((long *) addr_15X) = val_14X;
  if ((1 == (*((long *) ((((char *) (-3 + channel_13X))) + 12))))) {
    if ((1 == (Spending_channels_headS))) {
      Spending_channels_headS = channel_13X;
      Spending_channels_tailS = channel_13X;
      return;}
    else {
      x_16X = Spending_channels_tailS;
      addr_17X = (((char *) (-3 + x_16X))) + 12;
      S48_WRITE_BARRIER(x_16X, addr_17X, channel_13X);
      *((long *) addr_17X) = channel_13X;
      Spending_channels_tailS = channel_13X;
      return;}}
  else {
    return;}}
}
static long close_channelB(long channel_18X)
{
  long arg0K0;
  char * addr_26X;
  long status_25X;
  long v_24X;
  long v_23X;
  long v_22X;
  long v_21X;
  long x_20X;
  long os_index_19X;
 {  os_index_19X = (((*((long *) ((((char *) (-3 + channel_18X))) + 8))))>>2);
  x_20X = *((long *) ((((char *) (-3 + channel_18X))) + 16));
  if ((5 == x_20X)) {
    v_21X = ps_abort_fd_op(os_index_19X);
    enqueue_channelB(os_index_19X, v_21X);
    goto L10660;}
  else {
    goto L10660;}}
 L10660: {
  v_22X = *((long *) (((char *) (-3 + channel_18X))));
  if ((4 == v_22X)) {
    goto L10675;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_18X))))))) {
      goto L10675;}
    else {
      v_23X = ps_close_fd(os_index_19X);
      arg0K0 = v_23X;
      goto L10682;}}}
 L10675: {
  v_24X = ps_close_fd(os_index_19X);
  arg0K0 = v_24X;
  goto L10682;}
 L10682: {
  status_25X = arg0K0;
  *((Svm_channelsS) + os_index_19X) = 1;
  addr_26X = ((char *) (-3 + channel_18X));
  S48_WRITE_BARRIER(channel_18X, addr_26X, 0);
  *((long *) addr_26X) = 0;
  return status_25X;}
}
static void push_continuationB(char * code_pointer_27X, long size_28X)
{
  long cont_31X;
  long template_30X;
  long pc_29X;
 {  pc_29X = (((code_pointer_27X - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2);
  template_30X = StemplateS;
  SstackS = ((SstackS) + -20);
  *((long *) ((SstackS) + 4)) = (128 | (4138 + (((size_28X)<<10))));
  cont_31X = 3 + (((long) ((SstackS) + 8)));
  *((long *) ((((char *) (-3 + cont_31X))) + 4)) = pc_29X;
  *((long *) ((((char *) (-3 + cont_31X))) + 8)) = template_30X;
  *((long *) ((((char *) (-3 + cont_31X))) + 12)) = (SenvS);
  *((long *) (((char *) (-3 + cont_31X)))) = (ScontS);
  ScontS = cont_31X;
  return;}
}
static void push_exception_continuationB(long exception_32X, long instruction_size_33X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long v_38X;
  long i_37X;
  char * p_36X;
  long tem_35X;
  long opcode_34X;
 {  opcode_34X = *((unsigned char *) (Scode_pointerS));
  *((long *) (SstackS)) = (((instruction_size_33X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_32X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (StemplateS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  SstackS = ((SstackS) + -4);
  tem_35X = Sexception_templateS;
  StemplateS = tem_35X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_35X))))))));
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L12425;}
 L12425: {
  p_36X = arg1K0;
  i_37X = arg0K1;
  if ((2 == (3 & (*((long *) p_36X))))) {
    if ((26 == (*((long *) p_36X)))) {
      arg0K0 = (-1 + i_37X);
      goto L12366;}
    else {
      arg0K0 = i_37X;
      goto L12366;}}
  else {
    arg1K0 = (p_36X + 4);
    arg0K1 = (1 + i_37X);
    goto L12425;}}
 L12366: {
  v_38X = arg0K0;
  push_continuationB((Scode_pointerS), v_38X);
  *((long *) (SstackS)) = (((opcode_34X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_32X)<<2));
  SstackS = ((SstackS) + -4);
  return;}
}
static long add_log_entryAgc(long proposal_index_39X, long i_40X, long stob_41X, long index_42X, long value_43X, char verifyP_44X)
{
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char * addr_70X;
  long value_69X;
  long value_68X;
  long v_67X;
  char * addr_66X;
  long i_65X;
  long stob_64X;
  long proposal_63X;
  long new_62X;
  char * addr_61X;
  char * addr_60X;
  long value_59X;
  long v_58X;
  long vector_57X;
  char * addr_56X;
  char * addr_55X;
  char * addr_54X;
  long log_53X;
  long value_52X;
  long stob_51X;
  long proposal_50X;
  char * addr_49X;
  long len_48X;
  long new_size_47X;
  long log_size_46X;
  long proposal_45X;
 {  proposal_45X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  log_size_46X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + proposal_45X))) + (((proposal_index_39X)<<2)))))))) + -4))))>>8))))>>2);
  if ((i_40X == (-1 + log_size_46X))) {
    Stemp0S = stob_41X;
    Stemp1S = value_43X;
    if ((1 == log_size_46X)) {
      arg0K0 = 17;
      goto L9885;}
    else {
      arg0K0 = (-1 + (((log_size_46X)<<1)));
      goto L9885;}}
  else {
    arg0K0 = proposal_45X;
    arg0K1 = stob_41X;
    arg0K2 = value_43X;
    goto L12535;}}
 L9885: {
  new_size_47X = arg0K0;
  len_48X = ((new_size_47X)<<2);
  addr_49X = s48_allocate_tracedAgc((4 + len_48X));
  if ((addr_49X == NULL)) {
    arg0K0 = 1;
    goto L9931;}
  else {
    *((long *) addr_49X) = (10 + (((len_48X)<<8)));
    arg0K0 = (3 + (((long) (addr_49X + 4))));
    goto L9931;}}
 L12535: {
  proposal_50X = arg0K0;
  stob_51X = arg0K1;
  value_52X = arg0K2;
  log_53X = *((long *) ((((char *) (-3 + proposal_50X))) + (((proposal_index_39X)<<2))));
  addr_54X = (((char *) (-3 + log_53X))) + (((i_40X)<<2));
  S48_WRITE_BARRIER(log_53X, addr_54X, stob_51X);
  *((long *) addr_54X) = stob_51X;
  addr_55X = (((char *) (-3 + log_53X))) + (4 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_55X, index_42X);
  *((long *) addr_55X) = index_42X;
  addr_56X = (((char *) (-3 + log_53X))) + (8 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_56X, value_52X);
  *((long *) addr_56X) = value_52X;
  if (verifyP_44X) {
    arg0K0 = value_52X;
    goto L12556;}
  else {
    arg0K0 = 29;
    goto L12556;}}
 L9931: {
  vector_57X = arg0K0;
  if ((1 == vector_57X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_58X;
    goto L9889;}
  else {
    arg0K0 = vector_57X;
    goto L9889;}}
 L12556: {
  value_59X = arg0K0;
  addr_60X = (((char *) (-3 + log_53X))) + (12 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_60X, value_59X);
  *((long *) addr_60X) = value_59X;
  addr_61X = (((char *) (-3 + log_53X))) + (16 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_61X, 1);
  *((long *) addr_61X) = 1;
  return value_52X;}
 L9889: {
  new_62X = arg0K0;
  proposal_63X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 < log_size_46X)) {
    stob_64X = *((long *) ((((char *) (-3 + proposal_63X))) + 4));
    memcpy((void *)(((char *) (-3 + new_62X))), (void *)(((char *) (-3 + stob_64X))),(-4 + (((log_size_46X)<<2))));
    goto L9909;}
  else {
    goto L9909;}}
 L9909: {
  arg0K0 = (4 + log_size_46X);
  goto L9913;}
 L9913: {
  i_65X = arg0K0;
  if ((i_65X == new_size_47X)) {
    addr_66X = (((char *) (-3 + proposal_63X))) + (((proposal_index_39X)<<2));
    S48_WRITE_BARRIER(proposal_63X, addr_66X, new_62X);
    *((long *) addr_66X) = new_62X;
    v_67X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
    value_68X = Stemp1S;
    Stemp1S = 1;
    value_69X = Stemp0S;
    Stemp0S = 1;
    arg0K0 = v_67X;
    arg0K1 = value_69X;
    arg0K2 = value_68X;
    goto L12535;}
  else {
    addr_70X = (((char *) (-3 + new_62X))) + (((i_65X)<<2));
    S48_WRITE_BARRIER(new_62X, addr_70X, 0);
    *((long *) addr_70X) = 0;
    arg0K0 = (1 + i_65X);
    goto L9913;}}
}
static long enter_bignum(char * external_bignum_71X)
{
  long desc_72X;
 {  desc_72X = 3 + (((long) external_bignum_71X));
  if ((3 == (3 & desc_72X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + desc_72X))) + -4))))>>2))))) {
      goto L13169;}
    else {
      goto L13183;}}
  else {
    goto L13183;}}
 L13169: {
  if ((3 == (3 & desc_72X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + desc_72X))) + -4)))))) {
      *((long *) ((((char *) (-3 + desc_72X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + desc_72X))) + -4))));
      return desc_72X;}
    else {
      return desc_72X;}}
  else {
    return desc_72X;}}
 L13183: {
  ps_error("not a bignum", 1, desc_72X);
  goto L13169;}
}
static long enter_stringAgc(char *string_73X)
{
  long arg0K0;
  long i_78X;
  long string_77X;
  char * addr_76X;
  long len_75X;
  long z_74X;
 {  z_74X = strlen((char *) string_73X);
  s48_make_availableAgc((4 + (-4 & (4 + z_74X))));
  len_75X = 1 + z_74X;
  addr_76X = s48_allocate_small((4 + len_75X));
  *((long *) addr_76X) = (70 + (((len_75X)<<8)));
  string_77X = 3 + (((long) (addr_76X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_77X))) + z_74X)) = 0;
  arg0K0 = 0;
  goto L14105;}
 L14105: {
  i_78X = arg0K0;
  if ((i_78X == z_74X)) {
    return string_77X;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_77X))) + i_78X)) = ((*(string_73X + i_78X)));
    arg0K0 = (1 + i_78X);
    goto L14105;}}
}
static long channel_close_error(long status_79X, long index_80X, long id_81X)
{
  long v_82X;
 {  ps_write_string("Error: ", (stderr));
  ps_write_string((ps_error_string(status_79X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(" while closing port ", (stderr));
  if ((3 == (3 & id_81X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + id_81X))) + -4))))>>2))))) {
      ps_write_string((((char *)(((char *) (-3 + id_81X))))), (stderr));
      goto L15080;}
    else {
      goto L15074;}}
  else {
    goto L15074;}}
 L15080: {
  PS_WRITE_CHAR(10, (stderr), v_82X)
  return v_82X;}
 L15074: {
  ps_write_integer((((index_80X)>>2)), (stderr));
  goto L15080;}
}
static long make_registered_channel(long mode_83X, long id_84X, long os_index_85X, long key_86X, long *TT0)
{
  long x_90X;
  char * addr_89X;
  char x_88X;
  char temp_87X;
 {  temp_87X = os_index_85X < (Snumber_of_channelsS);
  if (temp_87X) {
    goto L16132;}
  else {
    x_88X = add_more_channels(os_index_85X);
    if (x_88X) {
      goto L16132;}
    else {
      *TT0 = 9;
      return 1;}}}
 L16132: {
  if ((1 == (*((Svm_channelsS) + os_index_85X)))) {
    addr_89X = s48_allocate_small(24);
    *((long *) addr_89X) = 5146;
    x_90X = 3 + (((long) (addr_89X + 4)));
    *((long *) (((char *) (-3 + x_90X)))) = (((mode_83X)<<2));
    *((long *) ((((char *) (-3 + x_90X))) + 4)) = id_84X;
    *((long *) ((((char *) (-3 + x_90X))) + 8)) = (((os_index_85X)<<2));
    *((long *) ((((char *) (-3 + x_90X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_90X))) + 16)) = 1;
    *((Svm_channelsS) + os_index_85X) = x_90X;
    *TT0 = 9;
    return x_90X;}
  else {
    *TT0 = 11;
    return 1;}}
}
static long Hlookup2670(long table_91X, long string_92X, long key_93X)
{
  long arg0K1;
  long arg0K0;
  char * addr_108X;
  long value_107X;
  long link_106X;
  long x_105X;
  char * addr_104X;
  long next_103X;
  long len_102X;
  long s2_101X;
  long foo_100X;
  long bucket_99X;
  long link_98X;
  long index_97X;
  long h_96X;
  long i_95X;
  long n_94X;
 {  n_94X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_92X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L19999;}
 L19999: {
  i_95X = arg0K0;
  h_96X = arg0K1;
  if ((i_95X < n_94X)) {
    arg0K0 = (1 + i_95X);
    arg0K1 = (h_96X + (((*((unsigned char *) ((((char *) (-3 + string_92X))) + i_95X))))));
    goto L19999;}
  else {
    index_97X = 1023 & h_96X;
    link_98X = *((long *) ((((char *) (-3 + table_91X))) + (((index_97X)<<2))));
    if ((0 == (3 & link_98X))) {
      arg0K0 = (3 + (-4 & link_98X));
      goto L19955;}
    else {
      arg0K0 = link_98X;
      goto L19955;}}}
 L19955: {
  bucket_99X = arg0K0;
  arg0K0 = bucket_99X;
  goto L19961;}
 L19961: {
  foo_100X = arg0K0;
  if ((1 == foo_100X)) {
    if ((3 == (3 & bucket_99X))) {
      arg0K0 = (-4 & bucket_99X);
      goto L19966;}
    else {
      arg0K0 = bucket_99X;
      goto L19966;}}
  else {
    s2_101X = *((long *) (((char *) (-3 + foo_100X))));
    len_102X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_92X))) + -4))))>>8);
    if ((len_102X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_101X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_101X))), (void *)(((char *) (-3 + string_92X))),len_102X)))) {
        return foo_100X;}
      else {
        goto L19981;}}
    else {
      goto L19981;}}}
 L19966: {
  next_103X = arg0K0;
  addr_104X = s48_allocate_small(20);
  *((long *) addr_104X) = 4154;
  x_105X = 3 + (((long) (addr_104X + 4)));
  *((long *) (((char *) (-3 + x_105X)))) = string_92X;
  *((long *) ((((char *) (-3 + x_105X))) + 4)) = 1;
  *((long *) ((((char *) (-3 + x_105X))) + 8)) = 13;
  *((long *) ((((char *) (-3 + x_105X))) + 12)) = next_103X;
  if ((3 == (3 & x_105X))) {
    arg0K0 = (-4 & x_105X);
    goto L19972;}
  else {
    arg0K0 = x_105X;
    goto L19972;}}
 L19981: {
  link_106X = *((long *) ((((char *) (-3 + foo_100X))) + 12));
  if ((0 == (3 & link_106X))) {
    arg0K0 = (3 + (-4 & link_106X));
    goto L19961;}
  else {
    arg0K0 = link_106X;
    goto L19961;}}
 L19972: {
  value_107X = arg0K0;
  addr_108X = (((char *) (-3 + table_91X))) + (((index_97X)<<2));
  S48_WRITE_BARRIER(table_91X, addr_108X, value_107X);
  *((long *) addr_108X) = value_107X;
  return x_105X;}
}
static long Hlookup2651(long table_109X, long string_110X, long key_111X)
{
  long arg0K1;
  long arg0K0;
  char * addr_126X;
  long value_125X;
  long link_124X;
  long x_123X;
  char * addr_122X;
  long next_121X;
  long len_120X;
  long s2_119X;
  long foo_118X;
  long bucket_117X;
  long link_116X;
  long index_115X;
  long h_114X;
  long i_113X;
  long n_112X;
 {  n_112X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_110X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L20151;}
 L20151: {
  i_113X = arg0K0;
  h_114X = arg0K1;
  if ((i_113X < n_112X)) {
    arg0K0 = (1 + i_113X);
    arg0K1 = (h_114X + (((*((unsigned char *) ((((char *) (-3 + string_110X))) + i_113X))))));
    goto L20151;}
  else {
    index_115X = 1023 & h_114X;
    link_116X = *((long *) ((((char *) (-3 + table_109X))) + (((index_115X)<<2))));
    if ((0 == (3 & link_116X))) {
      arg0K0 = (3 + (-4 & link_116X));
      goto L20107;}
    else {
      arg0K0 = link_116X;
      goto L20107;}}}
 L20107: {
  bucket_117X = arg0K0;
  arg0K0 = bucket_117X;
  goto L20113;}
 L20113: {
  foo_118X = arg0K0;
  if ((1 == foo_118X)) {
    if ((3 == (3 & bucket_117X))) {
      arg0K0 = (-4 & bucket_117X);
      goto L20118;}
    else {
      arg0K0 = bucket_117X;
      goto L20118;}}
  else {
    s2_119X = *((long *) (((char *) (-3 + foo_118X))));
    len_120X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_110X))) + -4))))>>8);
    if ((len_120X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_119X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_119X))), (void *)(((char *) (-3 + string_110X))),len_120X)))) {
        return foo_118X;}
      else {
        goto L20133;}}
    else {
      goto L20133;}}}
 L20118: {
  next_121X = arg0K0;
  addr_122X = s48_allocate_small(20);
  *((long *) addr_122X) = 4154;
  x_123X = 3 + (((long) (addr_122X + 4)));
  *((long *) (((char *) (-3 + x_123X)))) = string_110X;
  *((long *) ((((char *) (-3 + x_123X))) + 4)) = 5;
  *((long *) ((((char *) (-3 + x_123X))) + 8)) = 13;
  *((long *) ((((char *) (-3 + x_123X))) + 12)) = next_121X;
  if ((3 == (3 & x_123X))) {
    arg0K0 = (-4 & x_123X);
    goto L20124;}
  else {
    arg0K0 = x_123X;
    goto L20124;}}
 L20133: {
  link_124X = *((long *) ((((char *) (-3 + foo_118X))) + 12));
  if ((0 == (3 & link_124X))) {
    arg0K0 = (3 + (-4 & link_124X));
    goto L20113;}
  else {
    arg0K0 = link_124X;
    goto L20113;}}
 L20124: {
  value_125X = arg0K0;
  addr_126X = (((char *) (-3 + table_109X))) + (((index_115X)<<2));
  S48_WRITE_BARRIER(table_109X, addr_126X, value_125X);
  *((long *) addr_126X) = value_125X;
  return x_123X;}
}
static void HtopD10192(void)
{
  long arg0K2;
  long arg0K1;
  long arg0K0;
  long new_164X;
  long status_163X;
  long id_162X;
  long v_161X;
  char v_160X;
  long channel_159X;
  long i_158X;
  char * addr_157X;
  long next_link_156X;
  long new_foo_155X;
  char v_154X;
  char * addr_153X;
  long foo_152X;
  char * addr_151X;
  long l_150X;
  long v_149X;
  long okay_link_148X;
  long foo_link_147X;
  char * addr_146X;
  char * addr_145X;
  char * addr_144X;
  char * addr_143X;
  long val_142X;
  char tracedP_141X;
  long next_140X;
  long thing_139X;
  long pair_138X;
  long alist_137X;
  long l2_136X;
  long goners_135X;
  long okay_134X;
  long alist_133X;
  long foo_link_132X;
  long v_131X;
  long pc_130X;
  long tem_129X;
  long i_128X;
  long table_127X;
 {  table_127X = s48_trace_value((Sthe_symbol_tableS));
  arg0K0 = 0;
  goto L20784;}
 L20784: {
  i_128X = arg0K0;
  if ((1024 == i_128X)) {
    Sthe_symbol_tableS = table_127X;
    tem_129X = StemplateS;
    pc_130X = Ssaved_pcS;
    StemplateS = tem_129X;
    Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_129X)))))))) + (((pc_130X)>>2)));
    v_131X = SHARED_REF((Sfinalizer_alistS));
    arg0K0 = v_131X;
    arg0K1 = 25;
    arg0K2 = 25;
    goto L10274;}
  else {
    foo_link_132X = *((long *) ((((char *) (-3 + table_127X))) + (((i_128X)<<2))));
    arg0K0 = foo_link_132X;
    arg0K1 = 1;
    goto L13627;}}
 L10274: {
  alist_133X = arg0K0;
  okay_134X = arg0K1;
  goners_135X = arg0K2;
  if ((25 == alist_133X)) {
    SHARED_SETB((Sfinalizer_alistS), okay_134X);
    l2_136X = Sfinalize_theseS;
    if ((25 == goners_135X)) {
      arg0K0 = l2_136X;
      goto L10281;}
    else {
      arg0K0 = goners_135X;
      goto L10336;}}
  else {
    alist_137X = s48_trace_value(alist_133X);
    pair_138X = s48_trace_value((*((long *) (((char *) (-3 + alist_137X))))));
    thing_139X = *((long *) (((char *) (-3 + pair_138X))));
    next_140X = *((long *) ((((char *) (-3 + alist_137X))) + 4));
    tracedP_141X = s48_extantP(thing_139X);
    val_142X = s48_trace_value(thing_139X);
    addr_143X = ((char *) (-3 + pair_138X));
    S48_WRITE_BARRIER(pair_138X, addr_143X, val_142X);
    *((long *) addr_143X) = val_142X;
    addr_144X = ((char *) (-3 + alist_137X));
    S48_WRITE_BARRIER(alist_137X, addr_144X, pair_138X);
    *((long *) addr_144X) = pair_138X;
    if (tracedP_141X) {
      addr_145X = (((char *) (-3 + alist_137X))) + 4;
      S48_WRITE_BARRIER(alist_137X, addr_145X, okay_134X);
      *((long *) addr_145X) = okay_134X;
      arg0K0 = next_140X;
      arg0K1 = alist_137X;
      arg0K2 = goners_135X;
      goto L10274;}
    else {
      addr_146X = (((char *) (-3 + alist_137X))) + 4;
      S48_WRITE_BARRIER(alist_137X, addr_146X, goners_135X);
      *((long *) addr_146X) = goners_135X;
      arg0K0 = next_140X;
      arg0K1 = okay_134X;
      arg0K2 = alist_137X;
      goto L10274;}}}
 L13627: {
  foo_link_147X = arg0K0;
  okay_link_148X = arg0K1;
  if ((0 == (3 & foo_link_147X))) {
    arg0K0 = (3 + (-4 & foo_link_147X));
    goto L13629;}
  else {
    arg0K0 = foo_link_147X;
    goto L13629;}}
 L10281: {
  v_149X = arg0K0;
  Sfinalize_theseS = v_149X;
  arg0K0 = 0;
  goto L19374;}
 L10336: {
  l_150X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_150X))) + 4))))) {
    addr_151X = (((char *) (-3 + l_150X))) + 4;
    S48_WRITE_BARRIER(l_150X, addr_151X, l2_136X);
    *((long *) addr_151X) = l2_136X;
    arg0K0 = goners_135X;
    goto L10281;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_150X))) + 4)));
    goto L10336;}}
 L13629: {
  foo_152X = arg0K0;
  if ((1 == foo_152X)) {
    addr_153X = (((char *) (-3 + table_127X))) + (((i_128X)<<2));
    S48_WRITE_BARRIER(table_127X, addr_153X, okay_link_148X);
    *((long *) addr_153X) = okay_link_148X;
    arg0K0 = (1 + i_128X);
    goto L20784;}
  else {
    v_154X = s48_extantP(foo_152X);
    if (v_154X) {
      new_foo_155X = s48_trace_value(foo_152X);
      next_link_156X = *((long *) ((((char *) (-3 + new_foo_155X))) + 4));
      addr_157X = (((char *) (-3 + new_foo_155X))) + 4;
      S48_WRITE_BARRIER(new_foo_155X, addr_157X, okay_link_148X);
      *((long *) addr_157X) = okay_link_148X;
      if ((3 == (3 & new_foo_155X))) {
        arg0K0 = next_link_156X;
        arg0K1 = (-4 & new_foo_155X);
        goto L13627;}
      else {
        arg0K0 = next_link_156X;
        arg0K1 = new_foo_155X;
        goto L13627;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + foo_152X))) + 4)));
      arg0K1 = okay_link_148X;
      goto L13627;}}}
 L19374: {
  i_158X = arg0K0;
  if ((i_158X == (Snumber_of_channelsS))) {
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
    channel_159X = *((Svm_channelsS) + i_158X);
    if ((1 == channel_159X)) {
      goto L19408;}
    else {
      v_160X = s48_extantP(channel_159X);
      if (v_160X) {
        v_161X = s48_trace_value(channel_159X);
        arg0K0 = v_161X;
        goto L19401;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_159X))))))) {
          arg0K0 = 1;
          goto L19401;}
        else {
          id_162X = *((long *) ((((char *) (-3 + channel_159X))) + 4));
          status_163X = close_channelB(channel_159X);
          if ((status_163X == NO_ERRORS)) {
            goto L17400;}
          else {
            channel_close_error(status_163X, (*((long *) ((((char *) (-3 + channel_159X))) + 8))), id_162X);
            goto L17400;}}}}}}
 L19408: {
  arg0K0 = (1 + i_158X);
  goto L19374;}
 L19401: {
  new_164X = arg0K0;
  *((Svm_channelsS) + i_158X) = new_164X;
  goto L19408;}
 L17400: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_162X))) {
    ps_write_integer((((id_162X)>>2)), (stderr));
    goto L17418;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_162X))))), (stderr));
    goto L17418;}}
 L17418: {
  ps_write_string(" ", (stderr));
  ps_write_integer(((((*((long *) ((((char *) (-3 + channel_159X))) + 8))))>>2)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L19401;}
}
static void HtopD10203(void)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long merged_arg0K0;

  int Hentry_tracer850165_return_tag;
  long Hentry_tracer8501650_return_value;
  long foo_link_166X;
  char * addr_233X;
  long next_link_232X;
  long new_foo_231X;
  long foo_230X;
  long done_link_229X;
  long foo_link_228X;
  long env_227X;
  long v_226X;
  long v_225X;
  long env_224X;
  long last_env_223X;
  long cont_222X;
  long env_221X;
  long v_220X;
  long env_219X;
  long p_218X;
  long arg_count_217X;
  long i_216X;
  char * p_215X;
  long unused_214X;
  char * a_213X;
  char * addr_212X;
  long val_211X;
  char * addr_210X;
  long val_209X;
  long channel_208X;
  long i_207X;
  char x_206X;
  long pair_205X;
  long v_204X;
  long v_203X;
  long env_202X;
  long alist_201X;
  char * addr_200X;
  long value_199X;
  long v_198X;
  long v_197X;
  long v_196X;
  long v_195X;
  long v_194X;
  long v_193X;
  long v_192X;
  long v_191X;
  long v_190X;
  long v_189X;
  long v_188X;
  long v_187X;
  long v_186X;
  long v_185X;
  long v_184X;
  long v_183X;
  long v_182X;
  long i_181X;
  char * addr_180X;
  long value_179X;
  long table_178X;
  long i_177X;
  long x2_176X;
  char * cell_175X;
  long i_174X;
  long x2_173X;
  char * cell_172X;
  long table_171X;
  char * frame_170X;
  long length_169X;
  char * frame_168X;
  long v_167X;
 {  v_167X = s48_trace_value((Sempty_logS));
  Sempty_logS = v_167X;
  arg1K0 = (Sexternal_root_stackS);
  goto L6928;}
 L6928: {
  frame_168X = arg1K0;
  if ((frame_168X == NULL)) {
    arg1K0 = (Spermanent_external_rootsS);
    goto L6954;}
  else {
    length_169X = *((long *) frame_168X);
    arg0K0 = 0;
    goto L6936;}}
 L6954: {
  frame_170X = arg1K0;
  if ((frame_170X == NULL)) {
    s48_initializing_gc_root();
    table_171X = s48_trace_value((Simported_bindingsS));
    arg0K0 = 0;
    goto L17683;}
  else {
    cell_172X = ((char *) (*((long *) (frame_170X + 4))));
    x2_173X = s48_trace_value((*((long *) cell_172X)));
    *((long *) cell_172X) = x2_173X;
    arg1K0 = (((char *) (*((long *) frame_170X))));
    goto L6954;}}
 L6936: {
  i_174X = arg0K0;
  if ((i_174X == length_169X)) {
    arg1K0 = (((char *) (*((long *) (frame_168X + 4)))));
    goto L6928;}
  else {
    cell_175X = ((char *) (*((long *) (frame_168X + (8 + (((i_174X)<<2)))))));
    x2_176X = s48_trace_value((*((long *) cell_175X)));
    *((long *) cell_175X) = x2_176X;
    arg0K0 = (1 + i_174X);
    goto L6936;}}
 L17683: {
  i_177X = arg0K0;
  if ((1024 == i_177X)) {
    Simported_bindingsS = table_171X;
    table_178X = s48_trace_value((Sexported_bindingsS));
    arg0K0 = 0;
    goto L17704;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_171X))) + (((i_177X)<<2)))));
    Hentry_tracer850165_return_tag = 0;
    goto Hentry_tracer850165;
   Hentry_tracer850165_return_0:
    value_179X = Hentry_tracer8501650_return_value;
    addr_180X = (((char *) (-3 + table_171X))) + (((i_177X)<<2));
    S48_WRITE_BARRIER(table_171X, addr_180X, value_179X);
    *((long *) addr_180X) = value_179X;
    arg0K0 = (1 + i_177X);
    goto L17683;}}
 L17704: {
  i_181X = arg0K0;
  if ((1024 == i_181X)) {
    Sexported_bindingsS = table_178X;
    Ssaved_pcS = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
    v_182X = s48_trace_value((StemplateS));
    StemplateS = v_182X;
    v_183X = s48_trace_value((SvalS));
    SvalS = v_183X;
    v_184X = s48_trace_value((Scurrent_threadS));
    Scurrent_threadS = v_184X;
    v_185X = s48_trace_value((Sinterrupt_templateS));
    Sinterrupt_templateS = v_185X;
    v_186X = s48_trace_value((Sexception_templateS));
    Sexception_templateS = v_186X;
    v_187X = s48_trace_value((Sinterrupted_templateS));
    Sinterrupted_templateS = v_187X;
    v_188X = s48_trace_value((Sos_signal_typeS));
    Sos_signal_typeS = v_188X;
    v_189X = s48_trace_value((Sos_signal_argumentS));
    Sos_signal_argumentS = v_189X;
    v_190X = SHARED_REF((Ssession_dataS));
    v_191X = s48_trace_value(v_190X);
    SHARED_SETB((Ssession_dataS), v_191X);
    v_192X = SHARED_REF((Sexception_handlersS));
    v_193X = s48_trace_value(v_192X);
    SHARED_SETB((Sexception_handlersS), v_193X);
    v_194X = SHARED_REF((Sinterrupt_handlersS));
    v_195X = s48_trace_value(v_194X);
    SHARED_SETB((Sinterrupt_handlersS), v_195X);
    v_196X = SHARED_REF((Sfinalize_theseS));
    v_197X = s48_trace_value(v_196X);
    SHARED_SETB((Sfinalize_theseS), v_197X);
    v_198X = SHARED_REF((Sfinalizer_alistS));
    arg0K0 = v_198X;
    goto L6992;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_178X))) + (((i_181X)<<2)))));
    Hentry_tracer850165_return_tag = 1;
    goto Hentry_tracer850165;
   Hentry_tracer850165_return_1:
    value_199X = Hentry_tracer8501650_return_value;
    addr_200X = (((char *) (-3 + table_178X))) + (((i_181X)<<2));
    S48_WRITE_BARRIER(table_178X, addr_200X, value_199X);
    *((long *) addr_200X) = value_199X;
    arg0K0 = (1 + i_181X);
    goto L17704;}}
 L6992: {
  alist_201X = arg0K0;
  if ((25 == alist_201X)) {
    env_202X = s48_trace_value((SenvS));
    SenvS = env_202X;
    v_203X = s48_trace_value((Spending_channels_headS));
    Spending_channels_headS = v_203X;
    v_204X = s48_trace_value((Spending_channels_tailS));
    Spending_channels_tailS = v_204X;
    arg0K0 = 0;
    goto L10406;}
  else {
    pair_205X = *((long *) (((char *) (-3 + alist_201X))));
    x_206X = s48_extantP((*((long *) (((char *) (-3 + pair_205X))))));
    if (x_206X) {
      goto L7017;}
    else {
      s48_trace_stob_contentsB((*((long *) (((char *) (-3 + pair_205X))))));
      goto L7017;}}}
 L10406: {
  i_207X = arg0K0;
  if ((i_207X == (Snumber_of_channelsS))) {
    if ((Sstack_warningPS)) {
      arg1K0 = (Sstack_beginS);
      goto L7540;}
    else {
      goto L7573;}}
  else {
    channel_208X = *((Svm_channelsS) + i_207X);
    if ((1 == channel_208X)) {
      goto L10434;}
    else {
      if ((0 == (*((long *) (((char *) (-3 + channel_208X))))))) {
        goto L10434;}
      else {
        val_209X = s48_trace_value((*((long *) ((((char *) (-3 + channel_208X))) + 4))));
        addr_210X = (((char *) (-3 + channel_208X))) + 4;
        S48_WRITE_BARRIER(channel_208X, addr_210X, val_209X);
        *((long *) addr_210X) = val_209X;
        goto L10434;}}}}
 L7017: {
  val_211X = s48_trace_value((*((long *) ((((char *) (-3 + pair_205X))) + 4))));
  addr_212X = (((char *) (-3 + pair_205X))) + 4;
  S48_WRITE_BARRIER(pair_205X, addr_212X, val_211X);
  *((long *) addr_212X) = val_211X;
  arg0K0 = (*((long *) ((((char *) (-3 + alist_201X))) + 4)));
  goto L6992;}
 L7540: {
  a_213X = arg1K0;
  if ((252645135 == (*((long *) a_213X)))) {
    arg1K0 = (a_213X + 4);
    goto L7540;}
  else {
    unused_214X = (((a_213X - (Sstack_beginS)))>>2);
    if ((unused_214X < 30)) {
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      ps_write_string("[Alert: stack overconsumption (", (stderr));
      ps_write_integer(unused_214X, (stderr));
      ps_write_string("); please inform the Scheme 48 implementors]", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sstack_warningPS = 0;
      goto L7573;}
    else {
      goto L7573;}}}
 L7573: {
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L7638;}
 L10434: {
  arg0K0 = (1 + i_207X);
  goto L10406;}
 L7638: {
  p_215X = arg1K0;
  i_216X = arg0K1;
  if ((2 == (3 & (*((long *) p_215X))))) {
    if ((26 == (*((long *) p_215X)))) {
      arg0K0 = (-1 + i_216X);
      goto L7575;}
    else {
      arg0K0 = i_216X;
      goto L7575;}}
  else {
    arg1K0 = (p_215X + 4);
    arg0K1 = (1 + i_216X);
    goto L7638;}}
 L7575: {
  arg_count_217X = arg0K0;
  s48_trace_locationsB(((SstackS) + 4), ((SstackS) + (4 + (((arg_count_217X)<<2)))));
  p_218X = SenvS;
  if ((3 == (3 & p_218X))) {
    if ((p_218X < (((long) (Sstack_beginS))))) {
      goto L7591;}
    else {
      if (((((long) (Sstack_endS))) < p_218X)) {
        goto L7591;}
      else {
        env_219X = SenvS;
        arg0K0 = env_219X;
        goto L7751;}}}
  else {
    goto L7591;}}
 L7591: {
  v_220X = s48_trace_value((SenvS));
  SenvS = v_220X;
  goto L7596;}
 L7751: {
  env_221X = arg0K0;
  if ((3 == (3 & env_221X))) {
    if ((env_221X < (((long) (Sstack_beginS))))) {
      goto L7596;}
    else {
      if (((((long) (Sstack_endS))) < env_221X)) {
        goto L7596;}
      else {
        s48_trace_stob_contentsB(env_221X);
        arg0K0 = (*((long *) (((char *) (-3 + env_221X)))));
        goto L7751;}}}
  else {
    goto L7596;}}
 L7596: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L7601;}
 L7601: {
  cont_222X = arg0K0;
  last_env_223X = arg0K1;
  env_224X = *((long *) ((((char *) (-3 + cont_222X))) + 12));
  s48_trace_stob_contentsB(cont_222X);
  if ((env_224X == last_env_223X)) {
    goto L7619;}
  else {
    arg0K0 = env_224X;
    goto L7706;}}
 L7619: {
  if ((cont_222X == (Sbottom_of_stackS))) {
    v_225X = s48_trace_value((Stemp0S));
    Stemp0S = v_225X;
    v_226X = s48_trace_value((Stemp1S));
    Stemp1S = v_226X;
    return;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + cont_222X)))));
    arg0K1 = env_224X;
    goto L7601;}}
 L7706: {
  env_227X = arg0K0;
  if ((3 == (3 & env_227X))) {
    if ((env_227X < (((long) (Sstack_beginS))))) {
      goto L7619;}
    else {
      if (((((long) (Sstack_endS))) < env_227X)) {
        goto L7619;}
      else {
        s48_trace_stob_contentsB(env_227X);
        arg0K0 = (*((long *) (((char *) (-3 + env_227X)))));
        goto L7706;}}}
  else {
    goto L7619;}}
 Hentry_tracer850165: {
  foo_link_166X = merged_arg0K0;{
  arg0K0 = foo_link_166X;
  arg0K1 = 1;
  goto L13699;}
 L13699: {
  foo_link_228X = arg0K0;
  done_link_229X = arg0K1;
  if ((0 == (3 & foo_link_228X))) {
    arg0K0 = (3 + (-4 & foo_link_228X));
    goto L13701;}
  else {
    arg0K0 = foo_link_228X;
    goto L13701;}}
 L13701: {
  foo_230X = arg0K0;
  if ((1 == foo_230X)) {
    Hentry_tracer8501650_return_value = done_link_229X;
    goto Hentry_tracer850165_return;}
  else {
    new_foo_231X = s48_trace_value(foo_230X);
    next_link_232X = *((long *) ((((char *) (-3 + new_foo_231X))) + 12));
    addr_233X = (((char *) (-3 + new_foo_231X))) + 12;
    S48_WRITE_BARRIER(new_foo_231X, addr_233X, done_link_229X);
    *((long *) addr_233X) = done_link_229X;
    if ((3 == (3 & new_foo_231X))) {
      arg0K0 = next_link_232X;
      arg0K1 = (-4 & new_foo_231X);
      goto L13699;}
    else {
      arg0K0 = next_link_232X;
      arg0K1 = new_foo_231X;
      goto L13699;}}}
 Hentry_tracer850165_return:
  switch (Hentry_tracer850165_return_tag) {
  case 0: goto Hentry_tracer850165_return_0;
  default: goto Hentry_tracer850165_return_1;
  }}

}
void s48_set_extension_valueB(long value_234X)
{

 {  s48_Sextension_valueS = value_234X;
  return;}
}
void s48_note_event(void)
{

 {  s48_Spending_eventsPS = 1;
  s48_Spending_interruptPS = 1;
  return;}
}
long s48_channel_count(void)
{

 {  return (Snumber_of_channelsS);}
}
long *s48_channels(void)
{

 {  return (Svm_channelsS);}
}
void s48_set_os_signal(long type_235X, long argument_236X)
{

 {  Sos_signal_typeS = type_235X;
  Sos_signal_argumentS = argument_236X;
  return;}
}
void s48_reset_interruptsB(void)
{

 {  Sos_signal_typeS = 1;
  Sos_signal_argumentS = 1;
  Senabled_interruptsS = 0;
  Spending_interruptsS = 0;
  s48_Spending_interruptPS = 0;
  return;}
}
long s48_imported_bindings(void)
{

 {  return (Simported_bindingsS);}
}
long s48_exported_bindings(void)
{

 {  return (Sexported_bindingsS);}
}
long s48_symbol_table(void)
{

 {  return (Sthe_symbol_tableS);}
}
char * s48_set_gc_roots_baseB(void)
{
  char * old_base_237X;
 {  old_base_237X = Sexternal_root_stack_baseS;
  Sexternal_root_stack_baseS = (Sexternal_root_stackS);
  return old_base_237X;}
}
char s48_release_gc_roots_baseB(char * old_base_238X)
{
  char okayP_239X;
 {  okayP_239X = (Sexternal_root_stackS) == (Sexternal_root_stack_baseS);
  Sexternal_root_stackS = old_base_238X;
  Sexternal_root_stack_baseS = old_base_238X;
  return okayP_239X;}
}
void s48_reset_external_rootsB(void)
{

 {  Sexternal_root_stackS = NULL;
  Sexternal_root_stack_baseS = NULL;
  Spermanent_external_rootsS = NULL;
  return;}
}
void s48_disable_interruptsB(void)
{

 {  s48_Spending_interruptPS = 0;
  Senabled_interruptsS = 0;
  return;}
}
void s48_push_gc_rootsB(char * frame_240X, long n_241X)
{

 {  *((long *) frame_240X) = n_241X;
  *((long *) (frame_240X + 4)) = (((long) (Sexternal_root_stackS)));
  Sexternal_root_stackS = frame_240X;
  return;}
}
void s48_register_gc_rootB(char * loc_addr_242X)
{
  char * frame_243X;
 {  frame_243X = (char *)malloc(8);
  if ((frame_243X == NULL)) {
    ps_error("out of memory registering a global root", 0);
    goto L3857;}
  else {
    goto L3857;}}
 L3857: {
  *((long *) frame_243X) = (((long) (Spermanent_external_rootsS)));
  *((long *) (frame_243X + 4)) = (((long) loc_addr_242X));
  Spermanent_external_rootsS = frame_243X;
  return;}
}
void s48_stack_setB(long x_244X, long value_245X)
{

 {  *((long *) ((SstackS) + (4 + (((x_244X)<<2))))) = value_245X;
  return;}
}
long s48_stack_ref(long i_246X)
{

 {  return (*((long *) ((SstackS) + (4 + (((i_246X)<<2))))));}
}
void s48_push(long x_247X)
{

 {  *((long *) (SstackS)) = x_247X;
  SstackS = ((SstackS) + -4);
  return;}
}
long s48_resetup_external_exception(long new_why_248X, long additional_nargs_249X)
{
  long old_why_251X;
  long old_nargs_250X;
 {  old_nargs_250X = Sexternal_exception_nargsS;
  old_why_251X = *((long *) ((SstackS) + (4 + (((old_nargs_250X)<<2)))));
  *((long *) ((SstackS) + (4 + (((old_nargs_250X)<<2))))) = (((new_why_248X)<<2));
  Sexternal_exception_nargsS = (old_nargs_250X + additional_nargs_249X);
  return old_why_251X;}
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
char * s48_shorten_bignum(char * external_bignum_252X, long number_of_digits_253X)
{
  long waste_size_258X;
  long old_data_size_257X;
  long new_data_size_256X;
  long new_size_255X;
  long bignum_254X;
 {  bignum_254X = 3 + (((long) external_bignum_252X));
  new_size_255X = 8 + (((number_of_digits_253X)<<2));
  new_data_size_256X = -4 + new_size_255X;
  old_data_size_257X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + bignum_254X))) + -4))))>>8);
  waste_size_258X = old_data_size_257X - new_data_size_256X;
  if ((waste_size_258X < 0)) {
    ps_error("shorten bignum", 2, new_data_size_256X, old_data_size_257X);
    goto L6505;}
  else {
    goto L6505;}}
 L6505: {
  if ((waste_size_258X < 4)) {
    return external_bignum_252X;}
  else {
    *((long *) ((((char *) (-3 + bignum_254X))) + -4)) = (82 + (((new_data_size_256X)<<8)));
    *((long *) ((((char *) (((long) ((((char *) (-3 + bignum_254X))) + (-4 & (3 + new_size_255X))))))) + -4)) = (-942 + (((waste_size_258X)<<8)));
    return external_bignum_252X;}}
}
long s48_allocate_bignum(long size_259X)
{
  char * addr_260X;
 {  addr_260X = s48_allocate_small((4 + size_259X));
  *((long *) addr_260X) = (82 + (((size_259X)<<8)));
  return (3 + (((long) (addr_260X + 4))));}
}
long s48_set_channel_os_index(long channel_261X, long os_index_262X)
{
  char * addr_269X;
  long val_268X;
  long v_267X;
  long x_266X;
  long old_index_265X;
  char x_264X;
  char temp_263X;
 {  temp_263X = os_index_262X < (Snumber_of_channelsS);
  if (temp_263X) {
    goto L10565;}
  else {
    x_264X = add_more_channels(os_index_262X);
    if (x_264X) {
      goto L10565;}
    else {
      return 36;}}}
 L10565: {
  if ((1 == (*((Svm_channelsS) + os_index_262X)))) {
    old_index_265X = (((*((long *) ((((char *) (-3 + channel_261X))) + 8))))>>2);
    x_266X = *((long *) ((((char *) (-3 + channel_261X))) + 16));
    if ((5 == x_266X)) {
      v_267X = ps_abort_fd_op(old_index_265X);
      enqueue_channelB(old_index_265X, v_267X);
      goto L10551;}
    else {
      goto L10551;}}
  else {
    return 44;}}
 L10551: {
  *((Svm_channelsS) + old_index_265X) = 1;
  *((Svm_channelsS) + os_index_262X) = channel_261X;
  val_268X = ((os_index_262X)<<2);
  addr_269X = (((char *) (-3 + channel_261X))) + 8;
  S48_WRITE_BARRIER(channel_261X, addr_269X, val_268X);
  *((long *) addr_269X) = val_268X;
  return 5;}
}
void s48_setup_external_exception(long why_270X, long nargs_271X)
{

 {  push_exception_continuationB(why_270X, 1);
  if ((10 < nargs_271X)) {
    ps_error("too many arguments from external exception", 0);
    goto L14185;}
  else {
    goto L14185;}}
 L14185: {
  Sexternal_exception_nargsS = nargs_271X;
  Sexternal_exceptionPS = 1;
  return;}
}
void s48_close_channel(long os_index_272X)
{
  long obj_273X;
 {  if ((os_index_272X < 0)) {
    return;}
  else {
    if ((os_index_272X < (Snumber_of_channelsS))) {
      obj_273X = *((Svm_channelsS) + os_index_272X);
      if ((3 == (3 & obj_273X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_273X))) + -4))))>>2))))) {
          close_channelB((*((Svm_channelsS) + os_index_272X)));
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
  long link_281X;
  long name_280X;
  long entry_279X;
  long link_278X;
  char temp_277X;
  long i_276X;
  long table_275X;
  FILE * out_274X;
 {  out_274X = stderr;
  table_275X = Simported_bindingsS;
  arg0K0 = 0;
  goto L15613;}
 L15613: {
  i_276X = arg0K0;
  temp_277X = 1024 == i_276X;
  if (temp_277X) {
    return temp_277X;}
  else {
    link_278X = *((long *) ((((char *) (-3 + table_275X))) + (((i_276X)<<2))));
    if ((0 == (3 & link_278X))) {
      arg0K0 = (3 + (-4 & link_278X));
      goto L15587;}
    else {
      arg0K0 = link_278X;
      goto L15587;}}}
 L15587: {
  entry_279X = arg0K0;
  if ((1 == entry_279X)) {
    arg0K0 = (1 + i_276X);
    goto L15613;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + entry_279X))) + 8)))))) {
      name_280X = *((long *) (((char *) (-3 + entry_279X))));
      ps_write_string("undefined imported name ", out_274X);
      if ((3 == (3 & name_280X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_280X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + name_280X))))), out_274X);
          goto L17594;}
        else {
          goto L17592;}}
      else {
        goto L17592;}}
    else {
      goto L15594;}}}
 L17594: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_274X, ignoreXX) }
  goto L15594;}
 L17592: {
  ps_write_string("<invalid name>", out_274X);
  goto L17594;}
 L15594: {
  link_281X = *((long *) ((((char *) (-3 + entry_279X))) + 12));
  if ((0 == (3 & link_281X))) {
    arg0K0 = (3 + (-4 & link_281X));
    goto L15587;}
  else {
    arg0K0 = link_281X;
    goto L15587;}}
}
long s48_really_add_channel(long mode_282X, long id_283X, long os_index_284X)
{
  long status_286X;
  long channel_285X;
 {  s48_make_availableAgc(24);
  channel_285X = make_registered_channel((((mode_282X)>>2)), id_283X, os_index_284X, 0, &status_286X);
  if ((3 == (3 & channel_285X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_285X))) + -4))))>>2))))) {
      return channel_285X;}
    else {
      goto L17737;}}
  else {
    goto L17737;}}
 L17737: {
  return (((status_286X)<<2));}
}
long s48_enter_integer(long x_287X)
{
  char * v_288X;
 {  s48_make_availableAgc(16);
  if ((536870911 < x_287X)) {
    goto L19357;}
  else {
    if ((x_287X < -536870912)) {
      goto L19357;}
    else {
      return (((x_287X)<<2));}}}
 L19357: {
  v_288X = (char *) s48_long_to_bignum(x_287X);
  return enter_bignum(v_288X);}
}
long s48_get_imported_binding(char *name_289X)
{
  long value_291X;
  long value_290X;
 {  value_290X = enter_stringAgc(name_289X);
  Stemp0S = value_290X;
  s48_make_availableAgc(20);
  value_291X = Stemp0S;
  Stemp0S = 1;
  return Hlookup2670((Sexported_bindingsS), value_291X, 0);}
}
void s48_define_exported_binding(char *name_292X, long value_293X)
{
  char * addr_298X;
  long x_297X;
  long value_296X;
  long value_295X;
  long name_294X;
 {  Stemp0S = value_293X;
  name_294X = enter_stringAgc(name_292X);
  Stemp1S = name_294X;
  s48_make_availableAgc(20);
  value_295X = Stemp0S;
  Stemp0S = 1;
  value_296X = Stemp1S;
  Stemp1S = 1;
  x_297X = Hlookup2651((Simported_bindingsS), value_296X, 0);
  addr_298X = (((char *) (-3 + x_297X))) + 8;
  S48_WRITE_BARRIER(x_297X, addr_298X, value_295X);
  *((long *) addr_298X) = value_295X;
  return;}
}
void s48_initialize_vm(char * stack_begin_299X, long stack_size_300X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;

  int make_hash_tableAgc_return_tag;
  long make_hash_tableAgc0_return_value;
  char * addr_354X;
  long i_353X;
  long table_352X;
  long v_351X;
  long vector_350X;
  char * addr_349X;
  char * addr_348X;
  long x_347X;
  long v_346X;
  long v_345X;
  long vector_344X;
  char * addr_343X;
  char * addr_342X;
  long code_341X;
  long temp_340X;
  char * addr_339X;
  char * addr_338X;
  char * addr_337X;
  long code_336X;
  long temp_335X;
  char * addr_334X;
  char * addr_333X;
  long cont_332X;
  char * addr_331X;
  long code_330X;
  long temp_329X;
  char * addr_328X;
  char * addr_327X;
  char * a_326X;
  char * addr_325X;
  long value_324X;
  long size_323X;
  char * start_322X;
  char * addr_321X;
  long val_320X;
  long index_319X;
  long h_318X;
  long i_317X;
  char * stack_316X;
  long n_315X;
  long string_314X;
  long foo_313X;
  long table_312X;
  long i_311X;
  long v_310X;
  long v_309X;
  long imported_bindings_308X;
  long exported_bindings_307X;
  long n_306X;
  long symbols_305X;
  long maybe_304X;
  long maybe_303X;
  long v_302X;
  long symbol_table_301X;
 {  symbol_table_301X = s48_initial_symbols();
  if ((symbol_table_301X == 1)) {
    make_hash_tableAgc_return_tag = 0;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_0:
    v_302X = make_hash_tableAgc0_return_value;
    Sthe_symbol_tableS = v_302X;
    maybe_303X = s48_find_all(1);
    if ((maybe_303X == 1)) {
      s48_collect();
      maybe_304X = s48_find_all(1);
      if ((maybe_304X == 1)) {
        ps_error("insufficient heap space to build symbol table", 0);
        arg0K0 = maybe_304X;
        goto L20482;}
      else {
        arg0K0 = maybe_304X;
        goto L20482;}}
    else {
      arg0K0 = maybe_303X;
      goto L20482;}}
  else {
    Sthe_symbol_tableS = symbol_table_301X;
    goto L20953;}}
 L20482: {
  symbols_305X = arg0K0;
  n_306X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_305X))) + -4))))>>8))))>>2);
  arg0K0 = 0;
  goto L20503;}
 L20953: {
  exported_bindings_307X = s48_initial_exported_bindings();
  imported_bindings_308X = s48_initial_imported_bindings();
  if ((1 == imported_bindings_308X)) {
    make_hash_tableAgc_return_tag = 1;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_1:
    v_309X = make_hash_tableAgc0_return_value;
    Simported_bindingsS = v_309X;
    make_hash_tableAgc_return_tag = 2;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_2:
    v_310X = make_hash_tableAgc0_return_value;
    Sexported_bindingsS = v_310X;
    goto L20959;}
  else {
    Simported_bindingsS = imported_bindings_308X;
    Sexported_bindingsS = exported_bindings_307X;
    goto L20959;}}
 L20503: {
  i_311X = arg0K0;
  if ((i_311X == n_306X)) {
    goto L20953;}
  else {
    table_312X = Sthe_symbol_tableS;
    foo_313X = *((long *) ((((char *) (-3 + symbols_305X))) + (((i_311X)<<2))));
    string_314X = *((long *) (((char *) (-3 + foo_313X))));
    n_315X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_314X))) + -4))))>>8));
    arg0K0 = 0;
    arg0K1 = 0;
    goto L19910;}}
 L20959: {
  if ((stack_size_300X < 8134)) {
    stack_316X = (char *)malloc(32536);
    if ((stack_316X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_316X;
      arg0K1 = 8134;
      goto L17801;}
    else {
      arg1K0 = stack_316X;
      arg0K1 = 8134;
      goto L17801;}}
  else {
    arg1K0 = stack_begin_299X;
    arg0K1 = stack_size_300X;
    goto L17801;}}
 L19910: {
  i_317X = arg0K0;
  h_318X = arg0K1;
  if ((i_317X < n_315X)) {
    arg0K0 = (1 + i_317X);
    arg0K1 = (h_318X + (((*((unsigned char *) ((((char *) (-3 + string_314X))) + i_317X))))));
    goto L19910;}
  else {
    index_319X = 1023 & h_318X;
    val_320X = *((long *) ((((char *) (-3 + table_312X))) + (((index_319X)<<2))));
    addr_321X = (((char *) (-3 + foo_313X))) + 4;
    S48_WRITE_BARRIER(foo_313X, addr_321X, val_320X);
    *((long *) addr_321X) = val_320X;
    if ((3 == (3 & foo_313X))) {
      arg0K0 = (-4 & foo_313X);
      goto L19896;}
    else {
      arg0K0 = foo_313X;
      goto L19896;}}}
 L17801: {
  start_322X = arg1K0;
  size_323X = arg0K1;
  Sstack_beginS = start_322X;
  Sstack_endS = (start_322X + (((size_323X)<<2)));
  Sstack_limitS = ((Sstack_beginS) + 536);
  SstackS = ((Sstack_endS) + -4);
  ScontS = 1;
  SenvS = 17;
  arg1K0 = start_322X;
  goto L17833;}
 L19896: {
  value_324X = arg0K0;
  addr_325X = (((char *) (-3 + table_312X))) + (((index_319X)<<2));
  S48_WRITE_BARRIER(table_312X, addr_325X, value_324X);
  *((long *) addr_325X) = value_324X;
  arg0K0 = (1 + i_311X);
  goto L20503;}
 L17833: {
  a_326X = arg1K0;
  if ((a_326X == (Sstack_endS))) {
    s48_make_availableAgc(20);
    addr_327X = s48_allocate_small(6);
    *((long *) addr_327X) = 586;
    addr_328X = s48_allocate_small(12);
    *((long *) addr_328X) = 2098;
    temp_329X = 3 + (((long) (addr_328X + 4)));
    code_330X = 3 + (((long) (addr_327X + 4)));
    addr_331X = ((char *) (-3 + temp_329X));
    S48_WRITE_BARRIER(temp_329X, addr_331X, code_330X);
    *((long *) addr_331X) = code_330X;
    *((unsigned char *) (((char *) (-3 + code_330X)))) = 0;
    *((unsigned char *) ((((char *) (-3 + code_330X))) + 1)) = 71;
    SstackS = ((SstackS) + -20);
    *((long *) ((SstackS) + 4)) = 4266;
    cont_332X = 3 + (((long) ((SstackS) + 8)));
    *((long *) ((((char *) (-3 + cont_332X))) + 4)) = 0;
    *((long *) ((((char *) (-3 + cont_332X))) + 8)) = temp_329X;
    *((long *) ((((char *) (-3 + cont_332X))) + 12)) = (SenvS);
    *((long *) (((char *) (-3 + cont_332X)))) = (ScontS);
    ScontS = cont_332X;
    Sbottom_of_stackS = (ScontS);
    s48_make_availableAgc(40);
    addr_333X = s48_allocate_small(7);
    *((long *) addr_333X) = 842;
    addr_334X = s48_allocate_small(12);
    *((long *) addr_334X) = 2098;
    temp_335X = 3 + (((long) (addr_334X + 4)));
    code_336X = 3 + (((long) (addr_333X + 4)));
    addr_337X = ((char *) (-3 + temp_335X));
    S48_WRITE_BARRIER(temp_335X, addr_337X, code_336X);
    *((long *) addr_337X) = code_336X;
    *((unsigned char *) (((char *) (-3 + code_336X)))) = 0;
    *((unsigned char *) ((((char *) (-3 + code_336X))) + 1)) = 66;
    *((unsigned char *) ((((char *) (-3 + code_336X))) + 2)) = 148;
    Sinterrupt_templateS = temp_335X;
    addr_338X = s48_allocate_small(7);
    *((long *) addr_338X) = 842;
    addr_339X = s48_allocate_small(12);
    *((long *) addr_339X) = 2098;
    temp_340X = 3 + (((long) (addr_339X + 4)));
    code_341X = 3 + (((long) (addr_338X + 4)));
    addr_342X = ((char *) (-3 + temp_340X));
    S48_WRITE_BARRIER(temp_340X, addr_342X, code_341X);
    *((long *) addr_342X) = code_341X;
    *((unsigned char *) (((char *) (-3 + code_341X)))) = 0;
    *((unsigned char *) ((((char *) (-3 + code_341X))) + 1)) = 1;
    *((unsigned char *) ((((char *) (-3 + code_341X))) + 2)) = 145;
    Sexception_templateS = temp_340X;
    s48_make_availableAgc(32);
    s48_bignum_make_cached_constants();
    addr_343X = s48_allocate_tracedAgc(8);
    if ((addr_343X == NULL)) {
      arg0K0 = 1;
      goto L21010;}
    else {
      *((long *) addr_343X) = 1034;
      arg0K0 = (3 + (((long) (addr_343X + 4))));
      goto L21010;}}
  else {
    *((long *) a_326X) = 252645135;
    arg1K0 = (a_326X + 4);
    goto L17833;}}
 L21010: {
  vector_344X = arg0K0;
  if ((1 == vector_344X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_345X;
    goto L20997;}
  else {
    arg0K0 = vector_344X;
    goto L20997;}}
 L20997: {
  v_346X = arg0K0;
  Sempty_logS = v_346X;
  x_347X = Sempty_logS;
  addr_348X = ((char *) (-3 + x_347X));
  S48_WRITE_BARRIER(x_347X, addr_348X, 1);
  *((long *) addr_348X) = 1;
  return;}
 make_hash_tableAgc: {
{ addr_349X = s48_allocate_tracedAgc(4100);
  if ((addr_349X == NULL)) {
    arg0K0 = 1;
    goto L9795;}
  else {
    *((long *) addr_349X) = 1048586;
    arg0K0 = (3 + (((long) (addr_349X + 4))));
    goto L9795;}}
 L9795: {
  vector_350X = arg0K0;
  if ((1 == vector_350X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_351X;
    goto L9783;}
  else {
    arg0K0 = vector_350X;
    goto L9783;}}
 L9783: {
  table_352X = arg0K0;
  arg0K0 = 0;
  goto L9818;}
 L9818: {
  i_353X = arg0K0;
  if ((1024 == i_353X)) {
    make_hash_tableAgc0_return_value = table_352X;
    goto make_hash_tableAgc_return;}
  else {
    addr_354X = (((char *) (-3 + table_352X))) + (((i_353X)<<2));
    S48_WRITE_BARRIER(table_352X, addr_354X, 1);
    *((long *) addr_354X) = 1;
    arg0K0 = (1 + i_353X);
    goto L9818;}}
 make_hash_tableAgc_return:
  switch (make_hash_tableAgc_return_tag) {
  case 0: goto make_hash_tableAgc_return_0;
  case 1: goto make_hash_tableAgc_return_1;
  default: goto make_hash_tableAgc_return_2;
  }}

}
void s48_post_gc_cleanup(void)
{

 {  return (Spost_gc_cleanupS)();}
}
void s48_gc_root(void)
{

 {  return (Sgc_root_procS)();}
}
long s48_restart(long proc_355X, long nargs_356X)
{
  char *arg3K0;
  char arg2K3;
  char arg2K1;
  char arg2K2;
  char arg2K0;
  char * arg1K1;
  char * arg1K0;
  long arg0K4;
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
  int make_closure_return_tag;
  long make_closure0_return_value;
  int proposal_d_read_return_tag;
  long proposal_d_read0_return_value;
  int proposal_d_write_return_tag;
  int copy_continuation_from_heapB_return_tag;
  long copy_continuation_from_heapB0_return_value;
  int okay_copy_argsP_return_tag;
  char okay_copy_argsP0_return_value;
  int shift_space_return_tag;
  long shift_space0_return_value;
  int copy_listSAgc_return_tag;
  long copy_listSAgc0_return_value;
  int pop_args_GlistSAgc_return_tag;
  long pop_args_GlistSAgc0_return_value;
  int save_env_in_heap_return_tag;
  long save_env_in_heap0_return_value;
  int really_preserve_continuation_return_tag;
  long really_preserve_continuation0_return_value;
  int copy_stack_into_heap_return_tag;
  int push_list_return_tag;
  long push_list0_return_value;
  int rest_list_setupAgc_return_tag;
  int check_events_return_tag;
  char check_events0_return_value;
  int loseD0_return_tag;
  long status_357X;
  long list_358X;
  long marker_359X;
  long a_360X;
  long b_361X;
  long stob_362X;
  long index_363X;
  long stob_364X;
  long index_365X;
  long value_366X;
  long cont_367X;
  long stack_arg_count_368X;
  long thing_369X;
  long index_370X;
  long count_371X;
  long x_372X;
  long n_373X;
  long list_374X;
  long length_375X;
  long start_376X;
  long count_377X;
  long env_378X;
  long cont_379X;
  long key_380X;
  long reason_381X;
  long key_382X;
  long list_383X;
  long count_384X;
  long wants_stack_args_385X;
  long stack_arg_count_386X;
  long list_args_387X;
  long list_arg_count_388X;
  char *message_389X;
  long pc_1641X;
  long tem_1640X;
  long cont_1639X;
  long i_1638X;
  long string_1637X;
  char * addr_1636X;
  long len_1635X;
  long len_1634X;
  long x_1633X;
  char *string_1632X;
  char move_slowP_1631X;
  long slow_1630X;
  long len_1629X;
  long fast_1628X;
  long v_1627X;
  char *v_1626X;
  long v_1625X;
  long v_1624X;
  long obj_1623X;
  long env_1622X;
  long thread_1621X;
  long x_1620X;
  char * addr_1619X;
  long v_1618X;
  long next_stob_1617X;
  long i_1616X;
  long log_1615X;
  char * addr_1614X;
  long next_stob_1613X;
  long i_1612X;
  long log_1611X;
  long v_1610X;
  char * x_1609X;
  char * new_stack_1608X;
  long new_cont_1607X;
  char * top_1606X;
  long v_1605X;
  long v_1604X;
  long n_1603X;
  long extra_1602X;
  long x_size_1601X;
  char * addr_1600X;
  long x_1599X;
  char * addr_1598X;
  long a_1597X;
  long last_1596X;
  long l_1595X;
  long x_1594X;
  char * addr_1593X;
  long a_1592X;
  long value_1591X;
  long x_1590X;
  char * addr_1589X;
  long a_1588X;
  long count_1587X;
  long args_1586X;
  long value_1585X;
  long env_1584X;
  long cont_1583X;
  char * addr_1582X;
  char * addr_1581X;
  long new_1580X;
  char * data_addr_1579X;
  char * addr_1578X;
  long header_1577X;
  long env_1576X;
  long p_1575X;
  long env_1574X;
  char * addr_1573X;
  long new_1572X;
  char * data_addr_1571X;
  char * addr_1570X;
  long header_1569X;
  long new_1568X;
  char * data_addr_1567X;
  char * addr_1566X;
  long header_1565X;
  long p_1564X;
  long previous_1563X;
  long cont_1562X;
  long end_1561X;
  long v_1560X;
  long p_1559X;
  char * arg_1558X;
  char * loc_1557X;
  char * top_1556X;
  long arg_count_1555X;
  long i_1554X;
  char * p_1553X;
  long l_1552X;
  long i_1551X;
  long list_1550X;
  long x_1549X;
  long v_1548X;
  long x_1547X;
  long v_1546X;
  long count_1545X;
  long x_1544X;
  long v_1543X;
  long status_1542X;
  long channel_1541X;
  long type_1540X;
  char v_1539X;
  long template_1538X;
  char not_firstP_1537X;
  long cont_1536X;
  char not_firstP_1535X;
  FILE * out_1534X;
  long current_template_1533X;
  long why_1532X;
  long val_1531X;
  long val_1530X;
  long val_1529X;
  long val_1528X;
  long val_1527X;
  long val_1526X;
  long n_1525X;
  char v_1524X;
  char * external_bignum_1523X;
  char * x_1522X;
  long val_1521X;
  long n_1520X;
  char v_1519X;
  char * external_bignum_1518X;
  char * x_1517X;
  long val_1516X;
  long n_1515X;
  char v_1514X;
  char * external_bignum_1513X;
  char * x_1512X;
  long v_1511X;
  long n_1510X;
  char v_1509X;
  long v_1508X;
  long rem_1507X;
  long quot_1506X;
  char div_by_zeroP_1505X;
  long val_1504X;
  long n_1503X;
  char v_1502X;
  char * external_bignum_1501X;
  char * x_1500X;
  long val_1499X;
  long n_1498X;
  char v_1497X;
  char * external_bignum_1496X;
  char * x_1495X;
  long val_1494X;
  long n_1493X;
  char v_1492X;
  char * external_bignum_1491X;
  char * x_1490X;
  long link_1489X;
  long v_1488X;
  long v_1487X;
  long stob_1486X;
  long v_1485X;
  long stob_1484X;
  char * addr_1483X;
  long x_1482X;
  char * addr_1481X;
  char * addr_1480X;
  long value_1479X;
  char * addr_1478X;
  long value_1477X;
  long copies_1476X;
  char * v_1475X;
  long value_1474X;
  char * y_1473X;
  char * v_1472X;
  long value_1471X;
  char * y_1470X;
  char * v_1469X;
  long value_1468X;
  char * y_1467X;
  long val_1466X;
  long v_1465X;
  long n_1464X;
  char v_1463X;
  char * rem_1462X;
  char * quot_1461X;
  char div_by_zeroP_1460X;
  char * x_1459X;
  char * v_1458X;
  long value_1457X;
  char * y_1456X;
  char * v_1455X;
  long value_1454X;
  char * y_1453X;
  char * v_1452X;
  long value_1451X;
  char * y_1450X;
  long obj_1449X;
  char * addr_1448X;
  long val_1447X;
  char * addr_1446X;
  long value_1445X;
  long len_1444X;
  long s2_1443X;
  long foo_1442X;
  long previous_foo_1441X;
  long value_1440X;
  long verify_1439X;
  long copies_1438X;
  long stob_1437X;
  long i_1436X;
  long x_1435X;
  long x_1434X;
  long val_1433X;
  char * addr_1432X;
  long value_1431X;
  char * v_1430X;
  long value_1429X;
  long v_1428X;
  char * v_1427X;
  long value_1426X;
  long v_1425X;
  char * v_1424X;
  long value_1423X;
  long v_1422X;
  long val_1421X;
  long val_1420X;
  long n_1419X;
  char v_1418X;
  char * external_bignum_1417X;
  char * x_1416X;
  long val_1415X;
  long n_1414X;
  char v_1413X;
  char * external_bignum_1412X;
  char * x_1411X;
  char * v_1410X;
  long value_1409X;
  char * y_1408X;
  char * v_1407X;
  long value_1406X;
  long v_1405X;
  long val_1404X;
  long v_1403X;
  char * v_1402X;
  char * v_1401X;
  long value_1400X;
  long extra1_1399X;
  long length1_1398X;
  char * v_1397X;
  long value_1396X;
  long v_1395X;
  long v_1394X;
  long bucket_1393X;
  char * addr_1392X;
  long value_1391X;
  long verify_1390X;
  long log_1389X;
  long stob_1388X;
  long i_1387X;
  long x_1386X;
  long status_1385X;
  char pendingP_1384X;
  char eofP_1383X;
  long got_1382X;
  long y_1381X;
  long link_1380X;
  long val_1379X;
  long x_1378X;
  char * addr_1377X;
  long b_1376X;
  long val_1375X;
  long extra1_1374X;
  long length1_1373X;
  long extra1_1372X;
  long length1_1371X;
  long extra1_1370X;
  long length1_1369X;
  long n_1368X;
  char * v_1367X;
  long val_1366X;
  long n_1365X;
  char v_1364X;
  char * external_bignum_1363X;
  char * v_1362X;
  long n_1361X;
  char v_1360X;
  char * external_bignum_1359X;
  char * v_1358X;
  long value_1357X;
  char * y_1356X;
  char * v_1355X;
  long value_1354X;
  char * y_1353X;
  char * v_1352X;
  long value_1351X;
  long extra1_1350X;
  long val_1349X;
  long v_1348X;
  char * v_1347X;
  long extra1_1346X;
  long length1_1345X;
  long val_1344X;
  long v_1343X;
  char * v_1342X;
  long x_1341X;
  long val_1340X;
  long v_1339X;
  char * v_1338X;
  long extra0_1337X;
  long length0_1336X;
  long extra1_1335X;
  long length1_1334X;
  long final_stack_arg_count_1333X;
  char v_1332X;
  char x_1331X;
  long channel_1330X;
  long handlers_1329X;
  long arg_count_1328X;
  char * addr_1327X;
  long value_1326X;
  long offset_1325X;
  long i_1324X;
  long count_1323X;
  char * addr_1322X;
  long value_1321X;
  long offset_1320X;
  long i_1319X;
  long count_1318X;
  char *v_1317X;
  long link_1316X;
  long index_1315X;
  long h_1314X;
  long i_1313X;
  char * addr_1312X;
  char * addr_1311X;
  long value_1310X;
  char * addr_1309X;
  char * addr_1308X;
  char * addr_1307X;
  long value_1306X;
  char * addr_1305X;
  char * addr_1304X;
  long value_1303X;
  long proposal_1302X;
  long entry_1301X;
  long thing_1300X;
  long log_1299X;
  long copies_1298X;
  char * addr_1297X;
  long x_1296X;
  long val_1295X;
  long status_1294X;
  long v_1293X;
  long len_1292X;
  long s2_1291X;
  long foo_1290X;
  long val_1289X;
  long v_1288X;
  long n_1287X;
  char v_1286X;
  char * external_bignum_1285X;
  char * x_1284X;
  long extra0_1283X;
  long length0_1282X;
  long extra0_1281X;
  long length0_1280X;
  long extra0_1279X;
  long length0_1278X;
  char * v_1277X;
  long value_1276X;
  long v_1275X;
  char * v_1274X;
  long value_1273X;
  long v_1272X;
  char * v_1271X;
  long v_1270X;
  char * x_1269X;
  char * v_1268X;
  long n_1267X;
  char * v_1266X;
  long value_1265X;
  long extra1_1264X;
  char * v_1263X;
  long value_1262X;
  long extra1_1261X;
  long val_1260X;
  long val_1259X;
  long val_1258X;
  long val_1257X;
  long val_1256X;
  long extra0_1255X;
  long length0_1254X;
  long val_1253X;
  long v_1252X;
  char * v_1251X;
  long x_1250X;
  long val_1249X;
  long v_1248X;
  char * v_1247X;
  long extra0_1246X;
  long length0_1245X;
  long y_1244X;
  long x_1243X;
  long extra0_1242X;
  long length0_1241X;
  char * arg_1240X;
  char * loc_1239X;
  long l_1238X;
  long stack_nargs_1237X;
  long x_1236X;
  long v_1235X;
  long skip_1234X;
  long template_1233X;
  long stack_arg_count_1232X;
  long skip_1231X;
  char * addr_1230X;
  long next_1229X;
  long channel_1228X;
  long m_1227X;
  long i_1226X;
  long count_1225X;
  long index_1224X;
  long i_1223X;
  long env_1222X;
  long count_1221X;
  long i_1220X;
  long env_1219X;
  long i_1218X;
  long l_1217X;
  long n_1216X;
  long table_1215X;
  long val_1214X;
  char temp_1213X;
  char minutesP_1212X;
  long v_1211X;
  long vector_1210X;
  char * addr_1209X;
  long x_1208X;
  long value_1207X;
  long verify_1206X;
  long copies_1205X;
  long stob_1204X;
  long i_1203X;
  char * addr_1202X;
  long v_1201X;
  char * addr_1200X;
  char * addr_1199X;
  long val_1198X;
  long x_1197X;
  long status_1196X;
  char pendingP_1195X;
  long got_1194X;
  long y_1193X;
  char v_1192X;
  long count_1191X;
  long start_1190X;
  char waitP_1189X;
  long x_1188X;
  long status_1187X;
  long channel_1186X;
  long v_1185X;
  long v_1184X;
  long v_1183X;
  long reason_1182X;
  long channel_1181X;
  long index_1180X;
  long bucket_1179X;
  long i_1178X;
  long i_1177X;
  char * addr_1176X;
  long i_1175X;
  long rest_list_1174X;
  long i_1173X;
  long v_1172X;
  long n_1171X;
  char v_1170X;
  char * external_bignum_1169X;
  char * x_1168X;
  long val_1167X;
  long v_1166X;
  char * v_1165X;
  char * v_1164X;
  long value_1163X;
  long needed_1162X;
  long y_1161X;
  long y_1160X;
  long x_1159X;
  long val_1158X;
  long v_1157X;
  char * v_1156X;
  long val_1155X;
  long v_1154X;
  char * v_1153X;
  long extra_1152X;
  long length_1151X;
  long count_1150X;
  long x_1149X;
  long extra_1148X;
  long length_1147X;
  char * v_1146X;
  long value_1145X;
  long extra_1144X;
  long length_1143X;
  long x_1142X;
  long c_1141X;
  long b_1140X;
  long extra0_1139X;
  long length0_1138X;
  long c_1137X;
  long b_1136X;
  long extra0_1135X;
  long length0_1134X;
  long val_1133X;
  long v_1132X;
  long v_1131X;
  long v_1130X;
  long val_1129X;
  long v_1128X;
  long v_1127X;
  long v_1126X;
  long val_1125X;
  long v_1124X;
  long v_1123X;
  long v_1122X;
  long val_1121X;
  long v_1120X;
  long v_1119X;
  long v_1118X;
  long val_1117X;
  char b_1116X;
  long result_1115X;
  char x_1114X;
  long c_1113X;
  long b_1112X;
  long c_1111X;
  long mid_c_1110X;
  long v_1109X;
  long v_1108X;
  long lo_c_1107X;
  long hi_b_1106X;
  long hi_a_1105X;
  long lo_b_1104X;
  long lo_a_1103X;
  long b_1102X;
  long args_1101X;
  char * arg_top_1100X;
  long list_arg_count_1099X;
  long list_args_1098X;
  long stack_nargs_1097X;
  long bytes_used_1096X;
  long count_1095X;
  long v_1094X;
  char * arg_1093X;
  char * loc_1092X;
  long args_1091X;
  long v_1090X;
  long cont_1089X;
  long index_1088X;
  long length_1087X;
  long wants_stack_args_1086X;
  long skip_1085X;
  long skip_1084X;
  long stack_space_1083X;
  long protocol_1082X;
  long n_1081X;
  long tem_1080X;
  char * addr_1079X;
  long x_1078X;
  long stack_arg_count_1077X;
  long back_1076X;
  long env_1075X;
  long offset_1074X;
  long i_1073X;
  long back_1072X;
  long env_1071X;
  long offset_1070X;
  long i_1069X;
  long obj_1068X;
  long obj_1067X;
  long type_1066X;
  long thing_1065X;
  long stuff_1064X;
  char * addr_1063X;
  long val_1062X;
  long x_1061X;
  long i_1060X;
  long b_1059X;
  long p_1058X;
  long port_1057X;
  long Kchar_1056X;
  long x_1055X;
  long i_1054X;
  long b_1053X;
  long p_1052X;
  long p_1051X;
  long port_1050X;
  char * addr_1049X;
  long val_1048X;
  long x_1047X;
  long i_1046X;
  long b_1045X;
  long p_1044X;
  long p_1043X;
  long port_1042X;
  char * addr_1041X;
  long value_1040X;
  long list_1039X;
  long head_1038X;
  char move_slowP_1037X;
  long slow_1036X;
  long list_1035X;
  long string_1034X;
  char * addr_1033X;
  long len_1032X;
  long val_1031X;
  long mseconds_1030X;
  long seconds_1029X;
  long option_1028X;
  long v_1027X;
  long v_1026X;
  long result_1025X;
  char * args_1024X;
  long name_1023X;
  long proc_1022X;
  long rest_list_1021X;
  long x_1020X;
  long tem_1019X;
  long pc_1018X;
  char * addr_1017X;
  long x_1016X;
  long proposal_1015X;
  long value_1014X;
  long vector_1013X;
  long type_1012X;
  char firstP_1011X;
  long vector_1010X;
  char firstP_1009X;
  long h_1008X;
  long i_1007X;
  long x_1006X;
  long status_1005X;
  long reason_1004X;
  char * addr_1003X;
  long next_stob_1002X;
  long i_1001X;
  long v_1000X;
  long next_stob_999X;
  long i_998X;
  long value_997X;
  char * addr_996X;
  long count_995X;
  long to_index_994X;
  long from_index_993X;
  long copies_992X;
  long left_991X;
  long value_990X;
  long value_989X;
  long verify_988X;
  long log_987X;
  long stob_986X;
  long i_985X;
  char * addr_984X;
  long old_983X;
  long x_982X;
  char * addr_981X;
  long channel_980X;
  long res_979X;
  long i_978X;
  long y_977X;
  long n_976X;
  char * addr_975X;
  long prev_974X;
  long ch_973X;
  long val_972X;
  long val_971X;
  long v_970X;
  long v_969X;
  long val_968X;
  char * addr_967X;
  char * addr_966X;
  long val_965X;
  long link_964X;
  long index_963X;
  long h_962X;
  long i_961X;
  long vector_960X;
  long vector_959X;
  char * addr_958X;
  long value_957X;
  long value_956X;
  long x_955X;
  char * addr_954X;
  long i_953X;
  long i_952X;
  long value_951X;
  long val_950X;
  long val_949X;
  long val_948X;
  long val_947X;
  char * v_946X;
  long value_945X;
  long needed_944X;
  long y_943X;
  long x_942X;
  long result_941X;
  long x_940X;
  long x_939X;
  long x_938X;
  long count_937X;
  long value_936X;
  long val_935X;
  long val_934X;
  long x_933X;
  long x_932X;
  long n_931X;
  long a_930X;
  long y_929X;
  long x_928X;
  long a_927X;
  long y_926X;
  long x_925X;
  long val_924X;
  long val_923X;
  char b_922X;
  long val_921X;
  char b_920X;
  long val_919X;
  char b_918X;
  long val_917X;
  long Kdouble_916X;
  char * addr_915X;
  double value_914X;
  long value_913X;
  double y_912X;
  long value_911X;
  long a_910X;
  long Kdouble_909X;
  char * addr_908X;
  double x_907X;
  long value_906X;
  double y_905X;
  long value_904X;
  long val_903X;
  long v_902X;
  char * v_901X;
  long Kdouble_900X;
  char * addr_899X;
  double x_898X;
  long value_897X;
  double y_896X;
  long value_895X;
  long a_894X;
  long Kdouble_893X;
  char * addr_892X;
  double x_891X;
  long value_890X;
  double y_889X;
  long value_888X;
  long val_887X;
  long v_886X;
  char * v_885X;
  long val_884X;
  long val_883X;
  long delta_882X;
  long delta_881X;
  long offset_880X;
  char v_879X;
  long tem_878X;
  long args_877X;
  long index_876X;
  char * arg_top_875X;
  long args_874X;
  long count_873X;
  long index_872X;
  long next_871X;
  long cont_870X;
  long args_869X;
  long protocol_868X;
  long pc_867X;
  long code_866X;
  long cont_865X;
  long list_arg_count_864X;
  long list_args_863X;
  long stack_nargs_862X;
  long args_861X;
  long args_860X;
  long x_859X;
  char * addr_858X;
  long a_857X;
  long wants_stack_args_856X;
  long index_855X;
  long next_854X;
  long cont_853X;
  long protocol_852X;
  long pc_851X;
  long code_850X;
  long cont_849X;
  long obj_848X;
  char * addr_847X;
  long list_args_846X;
  long follower_845X;
  long list_844X;
  long proc_843X;
  long args_842X;
  long list_arg_count_841X;
  char okayP_840X;
  long stack_nargs_839X;
  long list_args_838X;
  long obj_837X;
  long obj_836X;
  long list_arg_count_835X;
  long list_args_834X;
  long stack_arg_count_833X;
  long args_832X;
  long v_831X;
  long v_830X;
  long v_829X;
  long list_arg_count_828X;
  long list_args_827X;
  long stack_arg_count_826X;
  long exception_825X;
  long total_arg_count_824X;
  long code_823X;
  long list_arg_count_822X;
  long list_args_821X;
  long stack_arg_count_820X;
  char * arg_819X;
  char * loc_818X;
  char v_817X;
  long value_816X;
  long start_i_815X;
  long start_i_814X;
  char * addr_813X;
  long closure_812X;
  long index_811X;
  char * addr_810X;
  long x_809X;
  char * addr_808X;
  long value_807X;
  long i_806X;
  long value_805X;
  long key_804X;
  long env_803X;
  long v_802X;
  long p_801X;
  long v_800X;
  char * addr_799X;
  long value_798X;
  long index_797X;
  long i_796X;
  long env_795X;
  long i_794X;
  long env_793X;
  long i_792X;
  long env_791X;
  long handlers_790X;
  long opcode_789X;
  long nargs_788X;
  FILE * out_787X;
  long x_786X;
  long v_785X;
  long v_784X;
  long v_783X;
  long v_782X;
  long v_781X;
  long v_780X;
  long v_779X;
  long v_778X;
  long len_777X;
  long value_776X;
  long index_775X;
  long arg4_774X;
  long arg3_773X;
  long arg2_772X;
  long v_771X;
  long v_770X;
  long len_769X;
  long index_768X;
  long arg3_767X;
  long arg2_766X;
  long list_765X;
  long thing_764X;
  long n_763X;
  long arg2_762X;
  long len_761X;
  long x_760X;
  long obj_759X;
  long arg2_758X;
  long x_757X;
  long arg2_756X;
  long status_755X;
  long value_754X;
  long key_753X;
  long arg2_752X;
  long mseconds_751X;
  long seconds_750X;
  long mseconds_749X;
  long seconds_748X;
  long x_747X;
  long other_746X;
  long option_745X;
  long arg2_744X;
  long x_743X;
  long arg2_742X;
  long x_741X;
  long arg2_740X;
  long rest_list_739X;
  long p_738X;
  long nargs_737X;
  long x_736X;
  long arg2_735X;
  long p_734X;
  long p_733X;
  long p_732X;
  long old_731X;
  long temp_730X;
  long obj_729X;
  long p_728X;
  long template_727X;
  long p_726X;
  long temp_725X;
  long obj_724X;
  long val_723X;
  long x_722X;
  long type_721X;
  long x_720X;
  long bytes_719X;
  long x_718X;
  long other_717X;
  long key_716X;
  long arg2_715X;
  long x_714X;
  char * addr_713X;
  long b_712X;
  long x_711X;
  char * addr_710X;
  long proc_709X;
  long stob_708X;
  long n_707X;
  long x_706X;
  long obj_705X;
  long status_704X;
  long status_703X;
  long status_702X;
  long status_701X;
  long status_700X;
  long status_699X;
  FILE * port_698X;
  long undumpables_697X;
  long obj_696X;
  long arg4_695X;
  long arg3_694X;
  long arg2_693X;
  long x_692X;
  long log_691X;
  long index_690X;
  long x_689X;
  long len_688X;
  long byte_687X;
  long index_686X;
  long arg3_685X;
  long arg2_684X;
  long log_683X;
  long index_682X;
  long x_681X;
  long len_680X;
  long index_679X;
  long arg2_678X;
  char v_677X;
  char v_676X;
  long v_675X;
  long count_674X;
  long to_index_673X;
  long from_index_672X;
  long arg5_671X;
  long arg4_670X;
  long arg3_669X;
  long arg2_668X;
  long v_667X;
  long x_666X;
  long type_665X;
  long offset_664X;
  long stob_663X;
  long log_662X;
  long proposal_661X;
  long proposal_660X;
  char * addr_659X;
  long next_658X;
  long channel_657X;
  long n_656X;
  char * addr_655X;
  long head_654X;
  long channel_653X;
  long obj_652X;
  long x_651X;
  long status_650X;
  char readyP_649X;
  long channel_648X;
  long obj_647X;
  char v_646X;
  long count_645X;
  long start_644X;
  long arg4_643X;
  long arg3_642X;
  long arg2_641X;
  long x_640X;
  long arg5_639X;
  long arg4_638X;
  long arg3_637X;
  long arg2_636X;
  long x_635X;
  long status_634X;
  long channel_633X;
  long obj_632X;
  long mode_631X;
  long arg2_630X;
  long x_629X;
  long x_628X;
  long x_627X;
  long arg2_626X;
  long descriptor_625X;
  long x_624X;
  long obj_623X;
  long n_622X;
  long string_621X;
  long table_620X;
  long obj_619X;
  long len_618X;
  char Kchar_617X;
  long index_616X;
  long arg3_615X;
  long arg2_614X;
  long len_613X;
  long index_612X;
  long arg2_611X;
  long obj_610X;
  char * addr_609X;
  long len_608X;
  char init_607X;
  long len_606X;
  long arg2_605X;
  long len_604X;
  long Kchar_603X;
  long index_602X;
  long arg3_601X;
  long arg2_600X;
  long len_599X;
  long index_598X;
  long arg2_597X;
  long obj_596X;
  char * addr_595X;
  long init_594X;
  long len_593X;
  long arg2_592X;
  long v_591X;
  long index_590X;
  long len_589X;
  long type_588X;
  long value_587X;
  long stob_586X;
  long arg2_585X;
  long v_584X;
  long v_583X;
  long index_582X;
  long len_581X;
  long type_580X;
  long index_579X;
  long stob_578X;
  char * addr_577X;
  long len_576X;
  long len_575X;
  long type_574X;
  long init_573X;
  long len_572X;
  long v_571X;
  long type_570X;
  long offset_569X;
  long value_568X;
  long stob_567X;
  long type_566X;
  long offset_565X;
  long stob_564X;
  long rest_list_563X;
  long stack_nargs_562X;
  long new_561X;
  char * addr_560X;
  long len_559X;
  long type_558X;
  long len_557X;
  long new_556X;
  char * addr_555X;
  long len_554X;
  long type_553X;
  long len_552X;
  long type_551X;
  long stob_550X;
  long type_549X;
  long x_548X;
  long x_547X;
  long x_546X;
  long x_545X;
  long arg2_544X;
  long x_543X;
  long arg2_542X;
  long x_541X;
  long v_540X;
  long v_539X;
  long y_538X;
  long x_537X;
  long y_536X;
  long x_535X;
  long y_534X;
  long x_533X;
  long y_532X;
  long x_531X;
  long x_530X;
  long x_529X;
  long x_528X;
  long x_527X;
  long arg2_526X;
  long x_525X;
  long arg2_524X;
  long x_523X;
  long arg2_522X;
  long x_521X;
  long x_520X;
  long v_519X;
  long x_518X;
  long x_517X;
  long x_516X;
  long x_515X;
  long x_514X;
  long x_513X;
  long x_512X;
  long x_511X;
  long x_510X;
  long n_509X;
  long n_508X;
  long n_507X;
  long n_506X;
  long n_505X;
  long a_504X;
  long y_503X;
  long x_502X;
  long b_501X;
  long a_500X;
  long y_499X;
  long x_498X;
  long y_497X;
  long x_496X;
  long y_495X;
  long x_494X;
  long y_493X;
  long x_492X;
  long y_491X;
  long x_490X;
  long y_489X;
  long x_488X;
  long b_487X;
  long a_486X;
  long y_485X;
  long x_484X;
  long x_483X;
  long y_482X;
  long x_481X;
  long b_480X;
  long a_479X;
  long y_478X;
  long x_477X;
  long x_476X;
  long y_475X;
  long x_474X;
  long x_473X;
  long x_472X;
  long n_471X;
  long n_470X;
  long n_469X;
  long n_468X;
  long n_467X;
  long x_466X;
  long x_465X;
  long arg2_464X;
  long rest_list_463X;
  long x_462X;
  long rest_list_461X;
  long stack_nargs_460X;
  long arg0_459X;
  long arg1_458X;
  long rest_list_457X;
  long arg0_456X;
  long stack_nargs_455X;
  long index_454X;
  long val_453X;
  long max_452X;
  long p_451X;
  long space_450X;
  long index_449X;
  long v_448X;
  long code_447X;
  long tem_446X;
  long stack_nargs_445X;
  long p_444X;
  long cont_443X;
  long rest_list_442X;
  long proc_441X;
  long stack_nargs_440X;
  long args_439X;
  long length_438X;
  char okayP_437X;
  long list_args_436X;
  long obj_435X;
  long stack_arg_count_434X;
  char * top_of_cont_433X;
  char * top_of_args_432X;
  long proc_431X;
  long protocol_430X;
  long code_429X;
  long template_428X;
  long obj_427X;
  long stack_arg_count_426X;
  long v_425X;
  char * addr_424X;
  long value_423X;
  long v_422X;
  long new_env_421X;
  char * addr_420X;
  long len_419X;
  long total_count_418X;
  char * addr_417X;
  long value_416X;
  long v_415X;
  long new_env_414X;
  char * addr_413X;
  long len_412X;
  long total_count_411X;
  long vector_410X;
  char * addr_409X;
  long len_408X;
  long count_407X;
  long p_406X;
  long v_405X;
  char * addr_404X;
  long val_403X;
  long location_402X;
  long location_401X;
  long env_400X;
  long back_399X;
  long env_398X;
  long back_397X;
  long env_396X;
  long back_395X;
  long count_394X;
  char * code_pointer_393X;
  long code_392X;
  long value_391X;
  char * addr_390X;
 {  Stemp0S = proc_355X;
  s48_make_availableAgc(8);
  addr_390X = s48_allocate_small(6);
  *((long *) addr_390X) = 586;
  value_391X = Stemp0S;
  Stemp0S = 1;
  code_392X = 3 + (((long) (addr_390X + 4)));
  *((unsigned char *) (((char *) (-3 + code_392X)))) = 25;
  *((unsigned char *) ((((char *) (-3 + code_392X))) + 1)) = nargs_356X;
  Scode_pointerS = (((char *) (-3 + code_392X)));
  SvalS = value_391X;
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L23693: {
  code_pointer_393X = arg1K0;
  switch ((*((unsigned char *) code_pointer_393X))) {
    case 0 : {
      push_exception_continuationB(14, 1);
      arg0K0 = 0;
      goto L21748;}
      break;
    case 1 : {
      count_394X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      *((long *) (SstackS)) = (SenvS);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (1034 + (((count_394X)<<10)));
      SstackS = ((SstackS) + -4);
      SenvS = (3 + (((long) ((SstackS) + 8))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 2 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 3 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 4 : {
      back_395X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_396X = SenvS;
      arg0K0 = env_396X;
      arg0K1 = back_395X;
      goto L30965;}
      break;
    case 5 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21748;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23693;}}
      break;
    case 6 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21748;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23693;}}
      break;
    case 7 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS)))))))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21748;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23693;}}
      break;
    case 8 : {
      back_397X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_398X = SenvS;
      arg0K0 = env_398X;
      arg0K1 = back_397X;
      goto L25660;}
      break;
    case 9 : {
      back_399X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_400X = SenvS;
      arg0K0 = env_400X;
      arg0K1 = back_399X;
      goto L25598;}
      break;
    case 10 : {
      location_401X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_401X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        push_exception_continuationB(1, 3);
        *((long *) (SstackS)) = location_401X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21748;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23693;}}
      break;
    case 11 : {
      location_402X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      if ((273 == (*((long *) ((((char *) (-3 + location_402X))) + 4))))) {
        push_exception_continuationB(1, 3);
        *((long *) (SstackS)) = location_402X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}
      else {
        val_403X = SvalS;
        addr_404X = (((char *) (-3 + location_402X))) + 4;
        S48_WRITE_BARRIER(location_402X, addr_404X, val_403X);
        *((long *) addr_404X) = val_403X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23693;}}
      break;
    case 12 : {
      v_405X = *((unsigned char *) ((Scode_pointerS) + 3));
      if ((0 == v_405X)) {
        p_406X = SenvS;
        if ((3 == (3 & p_406X))) {
          if ((p_406X < (((long) (Sstack_beginS))))) {
            arg0K0 = 0;
            goto L26335;}
          else {
            if (((((long) (Sstack_endS))) < p_406X)) {
              arg0K0 = 0;
              goto L26335;}
            else {
              arg0K0 = ((Sstack_endS) - (Sstack_beginS));
              goto L26335;}}}
        else {
          arg0K0 = 0;
          goto L26335;}}
      else {
        s48_make_availableAgc(12);
        arg0K0 = (SvalS);
        arg0K1 = 0;
        goto L26348;}}
      break;
    case 13 : {
      count_407X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      s48_make_availableAgc((((((2 + count_407X) + (3 * count_407X)))<<2)));
      len_408X = 4 + (((count_407X)<<2));
      addr_409X = s48_allocate_small((4 + len_408X));
      *((long *) addr_409X) = (10 + (((len_408X)<<8)));
      vector_410X = 3 + (((long) (addr_409X + 4)));
      arg0K0 = 1;
      goto L25385;}
      break;
    case 14 : {
      total_count_411X = *((unsigned char *) ((Scode_pointerS) + 2));
      s48_make_availableAgc((4 + (((total_count_411X)<<2))));
      len_412X = ((total_count_411X)<<2);
      addr_413X = s48_allocate_small((4 + len_412X));
      *((long *) addr_413X) = (10 + (((len_412X)<<8)));
      new_env_414X = 3 + (((long) (addr_413X + 4)));
      v_415X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_415X)) {
        arg0K0 = 0;
        goto L26672;}
      else {
        value_416X = SvalS;
        addr_417X = ((char *) (-3 + new_env_414X));
        S48_WRITE_BARRIER(new_env_414X, addr_417X, value_416X);
        *((long *) addr_417X) = value_416X;
        arg0K0 = 1;
        goto L26672;}}
      break;
    case 15 : {
      total_count_418X = ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 3)));
      s48_make_availableAgc((4 + (((total_count_418X)<<2))));
      len_419X = ((total_count_418X)<<2);
      addr_420X = s48_allocate_small((4 + len_419X));
      *((long *) addr_420X) = (10 + (((len_419X)<<8)));
      new_env_421X = 3 + (((long) (addr_420X + 4)));
      v_422X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_422X)) {
        arg0K0 = 0;
        goto L26523;}
      else {
        value_423X = SvalS;
        addr_424X = ((char *) (-3 + new_env_421X));
        S48_WRITE_BARRIER(new_env_421X, addr_424X, value_423X);
        *((long *) addr_424X) = value_423X;
        arg0K0 = 1;
        goto L26523;}}
      break;
    case 16 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 17 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L21748;}
      else {
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23693;}}
      break;
    case 18 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L21748;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23693;}}
      break;
    case 19 : {
      SstackS = ((SstackS) + 4);
      SvalS = (*((long *) (SstackS)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 20 : {
      SvalS = (*((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 21 : {
      *((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 22 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (*((unsigned char *) ((Scode_pointerS) + 3))));
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 23 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))));
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 24 : {
      s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
      if ((1 == (ScontS))) {
        arg0K0 = 1;
        goto L47651;}
      else {
        merged_arg0K0 = 0;
        merged_arg0K1 = 2;
        really_preserve_continuation_return_tag = 0;
        goto really_preserve_continuation;
       really_preserve_continuation_return_0:
        v_425X = really_preserve_continuation0_return_value;
        arg0K0 = v_425X;
        goto L47651;}}
      break;
    case 25 : {
      stack_arg_count_426X = *((unsigned char *) ((Scode_pointerS) + 1));
      obj_427X = SvalS;
      if ((3 == (3 & obj_427X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_427X))) + -4))))>>2))))) {
          template_428X = *((long *) (((char *) (-3 + (SvalS)))));
          code_429X = *((long *) (((char *) (-3 + template_428X))));
          protocol_430X = *((unsigned char *) ((((char *) (-3 + code_429X))) + 1));
          if ((protocol_430X == stack_arg_count_426X)) {
            StemplateS = template_428X;
            SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
            Scode_pointerS = ((((char *) (-3 + code_429X))) + 2);
            if (((SstackS) < (Sstack_limitS))) {
              s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
              merged_arg0K0 = 0;
              copy_stack_into_heap_return_tag = 0;
              goto copy_stack_into_heap;
             copy_stack_into_heap_return_0:
              if (((SstackS) < (Sstack_limitS))) {
                ps_error("Couldn't get default procedure space (how can this happen?)", 0);
                goto L24350;}
              else {
                goto L24350;}}
            else {
              goto L24350;}}
          else {
            if (((127 & protocol_430X) == stack_arg_count_426X)) {
              proc_431X = SvalS;
              StemplateS = (*((long *) (((char *) (-3 + proc_431X)))));
              Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS))))))))) + 2);
              top_of_args_432X = SstackS;
              top_of_cont_433X = (((char *) (-3 + (ScontS)))) + -8;
              if ((top_of_cont_433X == (top_of_args_432X + (((stack_arg_count_426X)<<2))))) {
                s48_call_native_code(proc_431X);
                goto L24360;}
              else {
                SstackS = top_of_cont_433X;
                arg1K0 = (SstackS);
                arg1K1 = (top_of_args_432X + (((stack_arg_count_426X)<<2)));
                goto L7909;}}
            else {
              arg0K0 = stack_arg_count_426X;
              arg0K1 = 25;
              arg0K2 = 0;
              goto L21378;}}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_426X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22250;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_426X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22250;}}
      break;
    case 26 : {
      stack_arg_count_434X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      obj_435X = SvalS;
      if ((3 == (3 & obj_435X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_435X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_434X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L21378;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_434X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22250;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_434X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22250;}}
      break;
    case 27 : {
      SstackS = ((SstackS) + 4);
      list_args_436X = *((long *) (SstackS));
      merged_arg0K0 = list_args_436X;
      okay_argument_list_return_tag = 0;
      goto okay_argument_list;
     okay_argument_list_return_0:
      okayP_437X = okay_argument_list0_return_value;
      length_438X = okay_argument_list1_return_value;
      if (okayP_437X) {
        arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
        arg0K1 = list_args_436X;
        arg0K2 = length_438X;
        goto L22879;}
      else {
        *((long *) (SstackS)) = list_args_436X;
        SstackS = ((SstackS) + -4);
        merged_arg0K0 = 25;
        merged_arg0K1 = (1 + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        pop_args_GlistSAgc_return_tag = 0;
        goto pop_args_GlistSAgc;
       pop_args_GlistSAgc_return_0:
        args_439X = pop_args_GlistSAgc0_return_value;
        push_exception_continuationB(5, 0);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = args_439X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
      break;
    case 28 : {
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      stack_nargs_440X = (((*((long *) (SstackS))))>>2);
      proc_441X = *((long *) ((SstackS) + (4 + (((stack_nargs_440X)<<2)))));
      *((long *) ((SstackS) + (4 + (((stack_nargs_440X)<<2))))) = 1;
      SstackS = ((SstackS) + 4);
      rest_list_442X = *((long *) (SstackS));
      if ((25 == rest_list_442X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = (-2 + stack_nargs_440X);
        goto L14861;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_442X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_442X)))));
          arg0K1 = (-1 + stack_nargs_440X);
          goto L14861;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_442X))) + 4)))))) + 4)));
          arg0K1 = rest_list_442X;
          goto L14902;}}}
      break;
    case 29 : {
      SstackS = ((SstackS) + 4);
      cont_443X = *((long *) (SstackS));
      if ((3 == (3 & cont_443X))) {
        if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_443X))) + -4))))>>2))))) {
          merged_arg0K0 = cont_443X;
          merged_arg0K1 = 0;
          copy_continuation_from_heapB_return_tag = 0;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_0:
          goto L22824;}
        else {
          goto L22831;}}
      else {
        goto L22831;}}
      break;
    case 30 : {
      goto L24960;}
      break;
    case 31 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L47114;}
      break;
    case 32 : {
      SstackS = ((SstackS) + 4);
      p_444X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_445X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      arg0K0 = stack_nargs_445X;
      arg0K1 = (*((long *) (SstackS)));
      arg0K2 = ((((p_444X)>>2)) - stack_nargs_445X);
      goto L47114;}
      break;
    case 33 : {
      tem_446X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      StemplateS = tem_446X;
      Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_446X))))))));
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 34 : {
      code_447X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))))))));
      if ((0 == (*((unsigned char *) ((((char *) (-3 + code_447X))) + 1))))) {
        if (((SstackS) < (Sstack_limitS))) {
          s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
          merged_arg0K0 = 0;
          copy_stack_into_heap_return_tag = 1;
          goto copy_stack_into_heap;
         copy_stack_into_heap_return_1:
          if (((SstackS) < (Sstack_limitS))) {
            ps_error("Couldn't get default procedure space (how can this happen?)", 0);
            goto L24090;}
          else {
            goto L24090;}}
        else {
          goto L24090;}}
      else {
        v_448X = *((unsigned char *) ((((char *) (-3 + code_447X))) + 1));
        if ((67 == v_448X)) {
          if ((0 == (*((unsigned char *) ((((char *) (-3 + code_447X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_447X))) + -4))))>>8)))))))) {
            index_449X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_447X))) + -4))))>>8));
            space_450X = ((((*((unsigned char *) ((((char *) (-3 + code_447X))) + index_449X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_447X))) + (1 + index_449X))));
            if ((space_450X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
              goto L24090;}
            else {
              s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
              merged_arg0K0 = 0;
              copy_stack_into_heap_return_tag = 2;
              goto copy_stack_into_heap;
             copy_stack_into_heap_return_2:
              if ((space_450X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
                goto L24090;}
              else {
                ps_error("VM's stack is too small (how can this happen?)", 0);
                goto L24090;}}}
          else {
            goto L24133;}}
        else {
          goto L24133;}}}
      break;
    case 35 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L23693;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23693;}}
      break;
    case 36 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 37 : {
      if ((0 == (3 & (SvalS)))) {
        p_451X = SvalS;
        max_452X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_453X = ((p_451X)>>2);
        if ((val_453X < 0)) {
          goto L23991;}
        else {
          if ((val_453X < max_452X)) {
            index_454X = 1 + (((val_453X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_454X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_454X)))));
            goto L23993;}
          else {
            goto L23991;}}}
      else {
        push_exception_continuationB(5, 0);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21748;}}
      break;
    case 38 : {
      stack_nargs_455X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((0 == stack_nargs_455X)) {
        arg0_456X = *((long *) ((SstackS) + 12));
        rest_list_457X = *((long *) ((SstackS) + 8));
        *((long *) (SstackS)) = arg0_456X;
        SstackS = ((SstackS) + -4);
        SvalS = (*((long *) (((char *) (-3 + rest_list_457X)))));
        goto L24876;}
      else {
        arg1_458X = *((long *) ((SstackS) + (4 + (((stack_nargs_455X)<<2)))));
        arg0_459X = *((long *) ((SstackS) + (8 + (((stack_nargs_455X)<<2)))));
        *((long *) ((SstackS) + (8 + (((stack_nargs_455X)<<2))))) = 1;
        *((long *) ((SstackS) + 4)) = (-4 + (((stack_nargs_455X)<<2)));
        *((long *) (SstackS)) = arg0_459X;
        SstackS = ((SstackS) + -4);
        SvalS = arg1_458X;
        goto L24876;}}
      break;
    case 39 : {
      stack_nargs_460X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((stack_nargs_460X == 0)) {
        rest_list_461X = *((long *) ((SstackS) + 8));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_461X))) + 4))))) {
          arg0K0 = 1;
          goto L23889;}
        else {
          *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_461X))) + 4)));
          *((long *) ((SstackS) + 12)) = (SvalS);
          arg0K0 = -2;
          goto L23889;}}
      else {
        if ((stack_nargs_460X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 8))))) {
            arg0K0 = 1;
            goto L23889;}
          else {
            *((long *) ((SstackS) + 4)) = 0;
            *((long *) ((SstackS) + 12)) = (SvalS);
            arg0K0 = -2;
            goto L23889;}}
        else {
          *((long *) ((SstackS) + (8 + (((stack_nargs_460X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L23889;}}}
      break;
    case 40 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23693;}
      else {
        x_462X = (((*((long *) ((SstackS) + 4))))>>2);
        if ((x_462X == 0)) {
          rest_list_463X = *((long *) ((SstackS) + 8));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_463X))) + 4))))) {
            arg0K0 = 1;
            goto L24767;}
          else {
            *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_463X))) + 4)));
            *((long *) ((SstackS) + 12)) = (*((long *) (((char *) (-3 + rest_list_463X)))));
            arg0K0 = -2;
            goto L24767;}}
        else {
          if ((x_462X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 8))))) {
              arg0K0 = 1;
              goto L24767;}
            else {
              *((long *) ((SstackS) + 4)) = 0;
              arg0K0 = -2;
              goto L24767;}}
          else {
            arg0K0 = -2;
            goto L24767;}}}}
      break;
    case 41 : {
      SstackS = ((SstackS) + 4);
      arg2_464X = *((long *) (SstackS));
      x_465X = SvalS;
      if ((arg2_464X == x_465X)) {
        arg0K0 = 5;
        goto L47704;}
      else {
        arg0K0 = 1;
        goto L47704;}}
      break;
    case 42 : {
      x_466X = SvalS;
      if ((0 == (3 & x_466X))) {
        arg0K0 = 5;
        goto L47716;}
      else {
        if ((3 == (3 & x_466X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_466X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L47716;}
          else {
            goto L13499;}}
        else {
          goto L13499;}}}
      break;
    case 43 : {
      n_467X = SvalS;
      if ((0 == (3 & n_467X))) {
        goto L39205;}
      else {
        if ((3 == (3 & n_467X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
            goto L39205;}
          else {
            goto L39206;}}
        else {
          goto L39206;}}}
      break;
    case 44 : {
      n_468X = SvalS;
      if ((0 == (3 & n_468X))) {
        goto L39362;}
      else {
        if ((3 == (3 & n_468X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
            goto L39362;}
          else {
            goto L39337;}}
        else {
          goto L39337;}}}
      break;
    case 45 : {
      n_469X = SvalS;
      if ((0 == (3 & n_469X))) {
        goto L39561;}
      else {
        if ((3 == (3 & n_469X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_469X))) + -4))))>>2))))) {
            goto L39561;}
          else {
            goto L39536;}}
        else {
          goto L39536;}}}
      break;
    case 46 : {
      n_470X = SvalS;
      if ((0 == (3 & n_470X))) {
        goto L39760;}
      else {
        if ((3 == (3 & n_470X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_470X))) + -4))))>>2))))) {
            goto L39760;}
          else {
            goto L39735;}}
        else {
          goto L39735;}}}
      break;
    case 47 : {
      n_471X = SvalS;
      if ((0 == (3 & n_471X))) {
        goto L32461;}
      else {
        if ((3 == (3 & n_471X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_471X))) + -4))))>>2))))) {
            goto L32461;}
          else {
            goto L32462;}}
        else {
          goto L32462;}}}
      break;
    case 48 : {
      x_472X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_472X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 49 : {
      x_473X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_473X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 50 : {
      SstackS = ((SstackS) + 4);
      x_474X = *((long *) (SstackS));
      y_475X = SvalS;
      if ((0 == (3 & (x_474X | y_475X)))) {
        s48_make_availableAgc(16);
        x_476X = (((x_474X)>>2)) + (((y_475X)>>2));
        if ((536870911 < x_476X)) {
          goto L32587;}
        else {
          if ((x_476X < -536870912)) {
            goto L32587;}
          else {
            arg0K0 = (((x_476X)<<2));
            goto L32533;}}}
      else {
        if ((0 == (3 & x_474X))) {
          goto L32539;}
        else {
          if ((3 == (3 & x_474X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_474X))) + -4))))>>2))))) {
              goto L32539;}
            else {
              goto L32548;}}
          else {
            goto L32548;}}}}
      break;
    case 51 : {
      SstackS = ((SstackS) + 4);
      x_477X = *((long *) (SstackS));
      y_478X = SvalS;
      if ((0 == (3 & (x_477X | y_478X)))) {
        a_479X = ((x_477X)>>2);
        b_480X = ((y_478X)>>2);
        if ((a_479X < 0)) {
          arg0K0 = (0 - a_479X);
          goto L8056;}
        else {
          arg0K0 = a_479X;
          goto L8056;}}
      else {
        if ((0 == (3 & x_477X))) {
          goto L41507;}
        else {
          if ((3 == (3 & x_477X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_477X))) + -4))))>>2))))) {
              goto L41507;}
            else {
              goto L41516;}}
          else {
            goto L41516;}}}}
      break;
    case 52 : {
      SstackS = ((SstackS) + 4);
      x_481X = *((long *) (SstackS));
      y_482X = SvalS;
      if ((0 == (3 & (x_481X | y_482X)))) {
        s48_make_availableAgc(16);
        x_483X = (((x_481X)>>2)) - (((y_482X)>>2));
        if ((536870911 < x_483X)) {
          goto L32876;}
        else {
          if ((x_483X < -536870912)) {
            goto L32876;}
          else {
            arg0K0 = (((x_483X)<<2));
            goto L32822;}}}
      else {
        if ((0 == (3 & x_481X))) {
          goto L32828;}
        else {
          if ((3 == (3 & x_481X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_481X))) + -4))))>>2))))) {
              goto L32828;}
            else {
              goto L32837;}}
          else {
            goto L32837;}}}}
      break;
    case 53 : {
      SstackS = ((SstackS) + 4);
      x_484X = *((long *) (SstackS));
      y_485X = SvalS;
      if ((0 == y_485X)) {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_484X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_485X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}
      else {
        if ((0 == (3 & (x_484X | y_485X)))) {
          if ((0 == y_485X)) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = x_484X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_485X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21748;}
          else {
            a_486X = ((x_484X)>>2);
            b_487X = ((y_485X)>>2);
            if ((a_486X < 0)) {
              arg0K0 = (0 - a_486X);
              goto L8314;}
            else {
              arg0K0 = a_486X;
              goto L8314;}}}
        else {
          if ((0 == (3 & x_484X))) {
            goto L41760;}
          else {
            if ((3 == (3 & x_484X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_484X))) + -4))))>>2))))) {
                goto L41760;}
              else {
                goto L41793;}}
            else {
              goto L41793;}}}}}
      break;
    case 54 : {
      SstackS = ((SstackS) + 4);
      x_488X = *((long *) (SstackS));
      y_489X = SvalS;
      if ((0 == (3 & (x_488X | y_489X)))) {
        if ((x_488X == y_489X)) {
          arg0K0 = 5;
          goto L33107;}
        else {
          arg0K0 = 1;
          goto L33107;}}
      else {
        if ((0 == (3 & x_488X))) {
          goto L33113;}
        else {
          if ((3 == (3 & x_488X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_488X))) + -4))))>>2))))) {
              goto L33113;}
            else {
              goto L33124;}}
          else {
            goto L33124;}}}}
      break;
    case 55 : {
      SstackS = ((SstackS) + 4);
      x_490X = *((long *) (SstackS));
      y_491X = SvalS;
      if ((0 == (3 & (x_490X | y_491X)))) {
        if ((x_490X < y_491X)) {
          arg0K0 = 5;
          goto L33367;}
        else {
          arg0K0 = 1;
          goto L33367;}}
      else {
        if ((0 == (3 & x_490X))) {
          goto L33373;}
        else {
          if ((3 == (3 & x_490X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_490X))) + -4))))>>2))))) {
              goto L33373;}
            else {
              goto L33384;}}
          else {
            goto L33384;}}}}
      break;
    case 56 : {
      SstackS = ((SstackS) + 4);
      x_492X = *((long *) (SstackS));
      y_493X = SvalS;
      if ((0 == (3 & (x_492X | y_493X)))) {
        if ((y_493X < x_492X)) {
          arg0K0 = 5;
          goto L33690;}
        else {
          arg0K0 = 1;
          goto L33690;}}
      else {
        if ((0 == (3 & x_492X))) {
          goto L33696;}
        else {
          if ((3 == (3 & x_492X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_492X))) + -4))))>>2))))) {
              goto L33696;}
            else {
              goto L33707;}}
          else {
            goto L33707;}}}}
      break;
    case 57 : {
      SstackS = ((SstackS) + 4);
      x_494X = *((long *) (SstackS));
      y_495X = SvalS;
      if ((0 == (3 & (x_494X | y_495X)))) {
        if ((y_495X < x_494X)) {
          arg0K0 = 1;
          goto L34013;}
        else {
          arg0K0 = 5;
          goto L34013;}}
      else {
        if ((0 == (3 & x_494X))) {
          goto L34019;}
        else {
          if ((3 == (3 & x_494X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_494X))) + -4))))>>2))))) {
              goto L34019;}
            else {
              goto L34030;}}
          else {
            goto L34030;}}}}
      break;
    case 58 : {
      SstackS = ((SstackS) + 4);
      x_496X = *((long *) (SstackS));
      y_497X = SvalS;
      if ((0 == (3 & (x_496X | y_497X)))) {
        if ((x_496X < y_497X)) {
          arg0K0 = 1;
          goto L34307;}
        else {
          arg0K0 = 5;
          goto L34307;}}
      else {
        if ((0 == (3 & x_496X))) {
          goto L34313;}
        else {
          if ((3 == (3 & x_496X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_496X))) + -4))))>>2))))) {
              goto L34313;}
            else {
              goto L34324;}}
          else {
            goto L34324;}}}}
      break;
    case 59 : {
      SstackS = ((SstackS) + 4);
      x_498X = *((long *) (SstackS));
      y_499X = SvalS;
      if ((0 == y_499X)) {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_498X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_499X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}
      else {
        if ((0 == (3 & (x_498X | y_499X)))) {
          if ((0 == y_499X)) {
            arg0K0 = x_498X;
            arg0K1 = y_499X;
            goto L19013;}
          else {
            a_500X = ((x_498X)>>2);
            b_501X = ((y_499X)>>2);
            if ((a_500X < 0)) {
              arg0K0 = (0 - a_500X);
              goto L8959;}
            else {
              arg0K0 = a_500X;
              goto L8959;}}}
        else {
          if ((0 == (3 & x_498X))) {
            goto L34613;}
          else {
            if ((3 == (3 & x_498X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_498X))) + -4))))>>2))))) {
                goto L34613;}
              else {
                goto L34622;}}
            else {
              goto L34622;}}}}}
      break;
    case 60 : {
      SstackS = ((SstackS) + 4);
      x_502X = *((long *) (SstackS));
      y_503X = SvalS;
      if ((0 == y_503X)) {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_502X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_503X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}
      else {
        if ((0 == (3 & (x_502X | y_503X)))) {
          if ((0 == y_503X)) {
            arg0K0 = x_502X;
            arg0K1 = y_503X;
            goto L18922;}
          else {
            a_504X = ((x_502X)>>2);
            if ((a_504X < 0)) {
              arg0K0 = (0 - a_504X);
              goto L34827;}
            else {
              arg0K0 = a_504X;
              goto L34827;}}}
        else {
          if ((0 == (3 & x_502X))) {
            goto L34789;}
          else {
            if ((3 == (3 & x_502X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_502X))) + -4))))>>2))))) {
                goto L34789;}
              else {
                goto L34798;}}
            else {
              goto L34798;}}}}}
      break;
    case 61 : {
      n_505X = SvalS;
      if ((0 == (3 & n_505X))) {
        goto L34976;}
      else {
        if ((3 == (3 & n_505X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_505X))) + -4))))>>2))))) {
            goto L34976;}
          else {
            goto L34977;}}
        else {
          goto L34977;}}}
      break;
    case 62 : {
      n_506X = SvalS;
      if ((0 == (3 & n_506X))) {
        goto L35031;}
      else {
        if ((3 == (3 & n_506X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_506X))) + -4))))>>2))))) {
            goto L35031;}
          else {
            goto L35032;}}
        else {
          goto L35032;}}}
      break;
    case 63 : {
      n_507X = SvalS;
      if ((0 == (3 & n_507X))) {
        goto L35086;}
      else {
        if ((3 == (3 & n_507X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_507X))) + -4))))>>2))))) {
            goto L35086;}
          else {
            goto L35089;}}
        else {
          goto L35089;}}}
      break;
    case 64 : {
      n_508X = SvalS;
      if ((0 == (3 & n_508X))) {
        goto L35146;}
      else {
        if ((3 == (3 & n_508X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_508X))) + -4))))>>2))))) {
            goto L35146;}
          else {
            goto L35147;}}
        else {
          goto L35147;}}}
      break;
    case 65 : {
      n_509X = SvalS;
      if ((0 == (3 & n_509X))) {
        goto L35201;}
      else {
        if ((3 == (3 & n_509X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_509X))) + -4))))>>2))))) {
            goto L35201;}
          else {
            goto L35204;}}
        else {
          goto L35204;}}}
      break;
    case 66 : {
      x_510X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_510X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 67 : {
      x_511X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_511X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 68 : {
      x_512X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_512X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 69 : {
      x_513X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_513X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 70 : {
      x_514X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_514X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 71 : {
      x_515X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_515X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 72 : {
      x_516X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_516X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 73 : {
      x_517X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_517X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 74 : {
      x_518X = SvalS;
      if ((0 == (3 & x_518X))) {
        if ((x_518X < 0)) {
          goto L47870;}
        else {
          goto L47876;}}
      else {
        v_519X = s48_bignum_test((((char *) (-3 + x_518X))));
        if ((-1 == v_519X)) {
          goto L47870;}
        else {
          goto L47876;}}}
      break;
    case 75 : {
      x_520X = SvalS;
      if ((0 == (3 & x_520X))) {
        goto L22499;}
      else {
        if ((3 == (3 & x_520X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_520X))) + -4))))>>2))))) {
            goto L22499;}
          else {
            goto L22502;}}
        else {
          goto L22502;}}}
      break;
    case 76 : {
      x_521X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_521X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 77 : {
      SstackS = ((SstackS) + 4);
      arg2_522X = *((long *) (SstackS));
      x_523X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_522X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_523X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}
      break;
    case 78 : {
      SstackS = ((SstackS) + 4);
      arg2_524X = *((long *) (SstackS));
      x_525X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_524X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_525X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}
      break;
    case 79 : {
      SstackS = ((SstackS) + 4);
      arg2_526X = *((long *) (SstackS));
      x_527X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_526X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_527X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}
      break;
    case 80 : {
      x_528X = SvalS;
      if ((0 == (3 & x_528X))) {
        SvalS = (~ (3 | x_528X));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23693;}
      else {
        if ((0 == (3 & x_528X))) {
          goto L35355;}
        else {
          if ((3 == (3 & x_528X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_528X))) + -4))))>>2))))) {
              goto L35355;}
            else {
              goto L35358;}}
          else {
            goto L35358;}}}}
      break;
    case 81 : {
      x_529X = SvalS;
      if ((0 == (3 & x_529X))) {
        x_530X = ((x_529X)>>2);
        if ((x_530X < 0)) {
          arg0K0 = (~ x_530X);
          goto L35446;}
        else {
          arg0K0 = x_530X;
          goto L35446;}}
      else {
        if ((0 == (3 & x_529X))) {
          goto L35429;}
        else {
          if ((3 == (3 & x_529X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_529X))) + -4))))>>2))))) {
              goto L35429;}
            else {
              goto L35432;}}
          else {
            goto L35432;}}}}
      break;
    case 82 : {
      SstackS = ((SstackS) + 4);
      x_531X = *((long *) (SstackS));
      y_532X = SvalS;
      if ((0 == (3 & (x_531X | y_532X)))) {
        SvalS = (x_531X & y_532X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23693;}
      else {
        if ((0 == (3 & x_531X))) {
          goto L35531;}
        else {
          if ((3 == (3 & x_531X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_531X))) + -4))))>>2))))) {
              goto L35531;}
            else {
              goto L35540;}}
          else {
            goto L35540;}}}}
      break;
    case 83 : {
      SstackS = ((SstackS) + 4);
      x_533X = *((long *) (SstackS));
      y_534X = SvalS;
      if ((0 == (3 & (x_533X | y_534X)))) {
        SvalS = (x_533X | y_534X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23693;}
      else {
        if ((0 == (3 & x_533X))) {
          goto L35694;}
        else {
          if ((3 == (3 & x_533X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_533X))) + -4))))>>2))))) {
              goto L35694;}
            else {
              goto L35703;}}
          else {
            goto L35703;}}}}
      break;
    case 84 : {
      SstackS = ((SstackS) + 4);
      x_535X = *((long *) (SstackS));
      y_536X = SvalS;
      if ((0 == (3 & (x_535X | y_536X)))) {
        SvalS = (x_535X ^ y_536X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23693;}
      else {
        if ((0 == (3 & x_535X))) {
          goto L35857;}
        else {
          if ((3 == (3 & x_535X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_535X))) + -4))))>>2))))) {
              goto L35857;}
            else {
              goto L35866;}}
          else {
            goto L35866;}}}}
      break;
    case 85 : {
      SstackS = ((SstackS) + 4);
      x_537X = *((long *) (SstackS));
      y_538X = SvalS;
      if ((3 == (3 & y_538X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_538X))) + -4))))>>2))))) {
          v_539X = s48_bignum_test((((char *) (-3 + y_538X))));
          if ((1 == v_539X)) {
            push_exception_continuationB(6, 1);
            *((long *) (SstackS)) = x_537X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_538X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21748;}
          else {
            if ((0 == (3 & x_537X))) {
              if ((x_537X < 0)) {
                arg0K0 = -4;
                goto L32241;}
              else {
                arg0K0 = 0;
                goto L32241;}}
            else {
              if ((3 == (3 & x_537X))) {
                if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_537X))) + -4))))>>2))))) {
                  v_540X = s48_bignum_test((((char *) (-3 + x_537X))));
                  if ((1 == v_540X)) {
                    arg0K0 = 0;
                    goto L32257;}
                  else {
                    arg0K0 = -4;
                    goto L32257;}}
                else {
                  goto L32258;}}
              else {
                goto L32258;}}}}
        else {
          goto L42086;}}
      else {
        goto L42086;}}
      break;
    case 86 : {
      x_541X = SvalS;
      if ((9 == (255 & x_541X))) {
        arg0K0 = 5;
        goto L47967;}
      else {
        arg0K0 = 1;
        goto L47967;}}
      break;
    case 87 : {
      SstackS = ((SstackS) + 4);
      arg2_542X = *((long *) (SstackS));
      if ((9 == (255 & arg2_542X))) {
        if ((9 == (255 & (SvalS)))) {
          x_543X = SvalS;
          if ((arg2_542X == x_543X)) {
            arg0K0 = 5;
            goto L38980;}
          else {
            arg0K0 = 1;
            goto L38980;}}
        else {
          goto L38953;}}
      else {
        goto L38953;}}
      break;
    case 88 : {
      SstackS = ((SstackS) + 4);
      arg2_544X = *((long *) (SstackS));
      if ((9 == (255 & arg2_544X))) {
        if ((9 == (255 & (SvalS)))) {
          x_545X = SvalS;
          if ((arg2_544X < x_545X)) {
            arg0K0 = 5;
            goto L38890;}
          else {
            arg0K0 = 1;
            goto L38890;}}
        else {
          goto L38863;}}
      else {
        goto L38863;}}
      break;
    case 89 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23693;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21748;}}
      break;
    case 90 : {
      if ((0 == (3 & (SvalS)))) {
        x_546X = (((SvalS))>>2);
        if ((255 < x_546X)) {
          goto L43276;}
        else {
          if ((x_546X < 0)) {
            goto L43276;}
          else {
            SvalS = (9 + (((((x_546X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23693;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21748;}}
      break;
    case 91 : {
      x_547X = SvalS;
      if ((21 == x_547X)) {
        arg0K0 = 5;
        goto L47996;}
      else {
        arg0K0 = 1;
        goto L47996;}}
      break;
    case 92 : {
      x_548X = SvalS;
      type_549X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_548X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_548X))) + -4))))>>2))) == type_549X)) {
          arg0K0 = 5;
          goto L48010;}
        else {
          arg0K0 = 1;
          goto L48010;}}
      else {
        arg0K0 = 1;
        goto L48010;}}
      break;
    case 93 : {
      stob_550X = SvalS;
      type_551X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_550X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_550X))) + -4))))>>2))) == type_551X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_550X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          goto L26860;}}
      else {
        goto L26860;}}
      break;
    case 94 : {
      len_552X = *((unsigned char *) ((Scode_pointerS) + 1));
      s48_make_availableAgc((4 + (((len_552X)<<2))));
      type_553X = *((unsigned char *) ((Scode_pointerS) + 2));
      len_554X = ((len_552X)<<2);
      addr_555X = s48_allocate_small((4 + len_554X));
      *((long *) addr_555X) = (2 + (((((((len_554X)<<6)) + type_553X))<<2)));
      new_556X = 3 + (((long) (addr_555X + 4)));
      if ((len_552X < 1)) {
        goto L26965;}
      else {
        *((long *) ((((char *) (-3 + new_556X))) + (-4 + (((len_552X)<<2))))) = (SvalS);
        arg0K0 = (-2 + len_552X);
        goto L26949;}}
      break;
    case 95 : {
      SstackS = ((SstackS) + 4);
      len_557X = (((*((long *) (SstackS))))>>2);
      s48_make_availableAgc((4 + (((len_557X)<<2))));
      type_558X = *((unsigned char *) ((Scode_pointerS) + 1));
      len_559X = ((len_557X)<<2);
      addr_560X = s48_allocate_small((4 + len_559X));
      *((long *) addr_560X) = (2 + (((((((len_559X)<<6)) + type_558X))<<2)));
      new_561X = 3 + (((long) (addr_560X + 4)));
      SstackS = ((SstackS) + 4);
      stack_nargs_562X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      rest_list_563X = *((long *) (SstackS));
      arg0K0 = (-1 + stack_nargs_562X);
      goto L27081;}
      break;
    case 96 : {
      stob_564X = SvalS;
      offset_565X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_566X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_564X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_564X))) + -4))))>>2))) == type_566X)) {
          SvalS = (*((long *) ((((char *) (-3 + stob_564X))) + (((offset_565X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          goto L27241;}}
      else {
        goto L27241;}}
      break;
    case 97 : {
      SstackS = ((SstackS) + 4);
      stob_567X = *((long *) (SstackS));
      value_568X = SvalS;
      offset_569X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_570X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_567X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_567X))) + -4))))>>2))) == type_570X)) {
          if ((3 == (3 & stob_567X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_567X))) + -4)))))) {
              v_571X = *((unsigned char *) ((Scode_pointerS) + 3));
              if ((0 == v_571X)) {
                goto L27358;}
              else {
                if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                  goto L27358;}
                else {
                  merged_arg0K0 = stob_567X;
                  merged_arg0K1 = (((offset_569X)<<2));
                  merged_arg0K2 = value_568X;
                  proposal_d_write_return_tag = 0;
                  goto proposal_d_write;
                 proposal_d_write_return_0:
                  goto L27367;}}}
            else {
              goto L27368;}}
          else {
            goto L27368;}}
        else {
          goto L27368;}}
      else {
        goto L27368;}}
      break;
    case 98 : {
      SstackS = ((SstackS) + 4);
      len_572X = *((long *) (SstackS));
      init_573X = SvalS;
      type_574X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & len_572X))) {
        len_575X = ((len_572X)>>2);
        if ((len_575X < 0)) {
          goto L27585;}
        else {
          if ((4194304 < len_575X)) {
            goto L27585;}
          else {
            Stemp0S = init_573X;
            len_576X = ((len_575X)<<2);
            addr_577X = s48_allocate_tracedAgc((4 + len_576X));
            if ((addr_577X == NULL)) {
              arg0K0 = 1;
              goto L27602;}
            else {
              *((long *) addr_577X) = (2 + (((((((len_576X)<<6)) + type_574X))<<2)));
              arg0K0 = (3 + (((long) (addr_577X + 4))));
              goto L27602;}}}}
      else {
        push_exception_continuationB(5, 2);
        *((long *) (SstackS)) = (((type_574X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = len_572X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = init_573X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L21748;}}
      break;
    case 99 : {
      SstackS = ((SstackS) + 4);
      stob_578X = *((long *) (SstackS));
      index_579X = SvalS;
      type_580X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & index_579X))) {
        if ((3 == (3 & stob_578X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + stob_578X))) + -4))))>>2))) == type_580X)) {
            len_581X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_578X))) + -4))))>>8))))>>2);
            index_582X = ((index_579X)>>2);
            if ((index_582X < 0)) {
              goto L27875;}
            else {
              if ((index_582X < len_581X)) {
                v_583X = *((unsigned char *) ((Scode_pointerS) + 2));
                if ((0 == v_583X)) {
                  goto L27865;}
                else {
                  if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                    goto L27865;}
                  else {
                    merged_arg0K0 = stob_578X;
                    merged_arg0K1 = index_579X;
                    proposal_d_read_return_tag = 0;
                    goto proposal_d_read;
                   proposal_d_read_return_0:
                    v_584X = proposal_d_read0_return_value;
                    arg0K0 = v_584X;
                    goto L27874;}}}
              else {
                goto L27875;}}}
          else {
            goto L27830;}}
        else {
          goto L27830;}}
      else {
        goto L27830;}}
      break;
    case 100 : {
      SstackS = ((SstackS) + 4);
      arg2_585X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stob_586X = *((long *) (SstackS));
      value_587X = SvalS;
      type_588X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_585X))) {
        if ((3 == (3 & stob_586X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + stob_586X))) + -4))))>>2))) == type_588X)) {
            if ((3 == (3 & stob_586X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_586X))) + -4)))))) {
                len_589X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_586X))) + -4))))>>8))))>>2);
                index_590X = ((arg2_585X)>>2);
                if ((index_590X < 0)) {
                  goto L28162;}
                else {
                  if ((index_590X < len_589X)) {
                    v_591X = *((unsigned char *) ((Scode_pointerS) + 2));
                    if ((0 == v_591X)) {
                      goto L28152;}
                    else {
                      if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                        goto L28152;}
                      else {
                        merged_arg0K0 = stob_586X;
                        merged_arg0K1 = arg2_585X;
                        merged_arg0K2 = value_587X;
                        proposal_d_write_return_tag = 1;
                        goto proposal_d_write;
                       proposal_d_write_return_1:
                        goto L28161;}}}
                  else {
                    goto L28162;}}}
              else {
                goto L28115;}}
            else {
              goto L28115;}}
          else {
            goto L28115;}}
        else {
          goto L28115;}}
      else {
        goto L28115;}}
      break;
    case 101 : {
      SstackS = ((SstackS) + 4);
      arg2_592X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_592X | (SvalS))))) {
        len_593X = ((arg2_592X)>>2);
        init_594X = (((SvalS))>>2);
        if ((len_593X < 0)) {
          goto L36078;}
        else {
          if ((4194304 < ((((3 + len_593X))>>2)))) {
            goto L36078;}
          else {
            addr_595X = s48_allocate_untracedAgc((4 + len_593X));
            if ((addr_595X == NULL)) {
              arg0K0 = 1;
              goto L36091;}
            else {
              *((long *) addr_595X) = (74 + (((len_593X)<<8)));
              arg0K0 = (3 + (((long) (addr_595X + 4))));
              goto L36091;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_592X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
      break;
    case 102 : {
      obj_596X = SvalS;
      if ((3 == (3 & obj_596X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + obj_596X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          goto L46781;}}
      else {
        goto L46781;}}
      break;
    case 103 : {
      SstackS = ((SstackS) + 4);
      arg2_597X = *((long *) (SstackS));
      if ((3 == (3 & arg2_597X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_597X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_598X = (((SvalS))>>2);
            len_599X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_597X))) + -4))))>>8);
            if ((index_598X < 0)) {
              goto L43382;}
            else {
              if ((index_598X < len_599X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_597X))) + index_598X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23693;}
              else {
                goto L43382;}}}
          else {
            goto L46297;}}
        else {
          goto L46297;}}
      else {
        goto L46297;}}
      break;
    case 104 : {
      SstackS = ((SstackS) + 4);
      arg2_600X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_601X = *((long *) (SstackS));
      if ((3 == (3 & arg3_601X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_601X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_600X | (SvalS))))) {
            index_602X = ((arg2_600X)>>2);
            Kchar_603X = (((SvalS))>>2);
            if ((3 == (3 & arg3_601X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_601X))) + -4)))))) {
                len_604X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_601X))) + -4))))>>8);
                if ((index_602X < 0)) {
                  goto L40729;}
                else {
                  if ((index_602X < len_604X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_601X))) + index_602X)) = Kchar_603X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L23693;}
                  else {
                    goto L40729;}}}
              else {
                goto L40708;}}
            else {
              goto L40708;}}
          else {
            goto L45077;}}
        else {
          goto L45077;}}
      else {
        goto L45077;}}
      break;
    case 105 : {
      SstackS = ((SstackS) + 4);
      arg2_605X = *((long *) (SstackS));
      if ((0 == (3 & arg2_605X))) {
        if ((9 == (255 & (SvalS)))) {
          len_606X = ((arg2_605X)>>2);
          init_607X = ((((SvalS))>>8));
          if ((len_606X < 0)) {
            goto L36258;}
          else {
            if ((4194304 < ((((4 + len_606X))>>2)))) {
              goto L36258;}
            else {
              len_608X = 1 + len_606X;
              addr_609X = s48_allocate_untracedAgc((4 + len_608X));
              if ((addr_609X == NULL)) {
                arg0K0 = 1;
                goto L36271;}
              else {
                *((long *) addr_609X) = (70 + (((len_608X)<<8)));
                arg0K0 = (3 + (((long) (addr_609X + 4))));
                goto L36271;}}}}
        else {
          goto L43463;}}
      else {
        goto L43463;}}
      break;
    case 106 : {
      obj_610X = SvalS;
      if ((3 == (3 & obj_610X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_610X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          goto L46831;}}
      else {
        goto L46831;}}
      break;
    case 107 : {
      SstackS = ((SstackS) + 4);
      arg2_611X = *((long *) (SstackS));
      if ((3 == (3 & arg2_611X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_611X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_612X = (((SvalS))>>2);
            len_613X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_611X))) + -4))))>>8));
            if ((index_612X < 0)) {
              goto L43526;}
            else {
              if ((index_612X < len_613X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_611X))) + index_612X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23693;}
              else {
                goto L43526;}}}
          else {
            goto L46393;}}
        else {
          goto L46393;}}
      else {
        goto L46393;}}
      break;
    case 108 : {
      SstackS = ((SstackS) + 4);
      arg2_614X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_615X = *((long *) (SstackS));
      if ((3 == (3 & arg3_615X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_615X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_614X))) {
            if ((9 == (255 & (SvalS)))) {
              index_616X = ((arg2_614X)>>2);
              Kchar_617X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_615X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_615X))) + -4)))))) {
                  len_618X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_615X))) + -4))))>>8));
                  if ((index_616X < 0)) {
                    goto L40884;}
                  else {
                    if ((index_616X < len_618X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_615X))) + index_616X)) = (Kchar_617X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L23693;}
                    else {
                      goto L40884;}}}
                else {
                  goto L40863;}}
              else {
                goto L40863;}}
            else {
              goto L45203;}}
          else {
            goto L45203;}}
        else {
          goto L45203;}}
      else {
        goto L45203;}}
      break;
    case 109 : {
      s48_make_availableAgc(12);
      obj_619X = SvalS;
      if ((3 == (3 & obj_619X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_619X))) + -4))))>>2))))) {
          table_620X = Sthe_symbol_tableS;
          string_621X = SvalS;
          n_622X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_621X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L20303;}
        else {
          goto L32403;}}
      else {
        goto L32403;}}
      break;
    case 110 : {
      obj_623X = SvalS;
      if ((3 == (3 & obj_623X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_623X))) + -4))))>>2))))) {
          x_624X = SvalS;
          descriptor_625X = *((long *) ((((char *) (-3 + x_624X))) + 4));
          if ((17 == (255 & descriptor_625X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_624X))) + 4))))) {
              arg0K0 = 5;
              goto L44767;}
            else {
              arg0K0 = 1;
              goto L44767;}}
          else {
            arg0K0 = 5;
            goto L44767;}}
        else {
          goto L44748;}}
      else {
        goto L44748;}}
      break;
    case 111 : {
      SstackS = ((SstackS) + 4);
      arg2_626X = *((long *) (SstackS));
      if ((3 == (3 & arg2_626X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_626X))) + -4))))>>2))))) {
          x_627X = SvalS;
          if ((1 == x_627X)) {
            goto L40471;}
          else {
            if ((5 == x_627X)) {
              goto L40471;}
            else {
              goto L40478;}}}
        else {
          goto L40478;}}
      else {
        goto L40478;}}
      break;
    case 112 : {
      x_628X = SvalS;
      if ((3 == (3 & x_628X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_628X))) + -4)))))) {
          arg0K0 = 1;
          goto L48082;}
        else {
          arg0K0 = 5;
          goto L48082;}}
      else {
        arg0K0 = 5;
        goto L48082;}}
      break;
    case 113 : {
      x_629X = SvalS;
      if ((3 == (3 & x_629X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_629X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_629X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_629X))) + -4))));
          goto L43608;}
        else {
          goto L43608;}}
      else {
        goto L43608;}}
      break;
    case 114 : {
      s48_make_availableAgc(288);
      SstackS = ((SstackS) + 4);
      arg2_630X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        mode_631X = (((SvalS))>>2);
        if ((1 == mode_631X)) {
          goto L36665;}
        else {
          if ((2 == mode_631X)) {
            goto L36665;}
          else {
            if ((3 == mode_631X)) {
              goto L36665;}
            else {
              if ((4 == mode_631X)) {
                goto L36665;}
              else {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = arg2_630X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = (((mode_631X)<<2));
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21748;}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_630X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
      break;
    case 115 : {
      s48_make_availableAgc(264);
      obj_632X = SvalS;
      if ((3 == (3 & obj_632X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_632X))) + -4))))>>2))))) {
          channel_633X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_633X))))))) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = channel_633X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21748;}
          else {
            status_634X = close_channelB(channel_633X);
            if ((status_634X == NO_ERRORS)) {
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23693;}
            else {
              push_exception_continuationB(24, 1);
              *((long *) (SstackS)) = channel_633X;
              SstackS = ((SstackS) + -4);
              merged_arg0K0 = status_634X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 0;
              goto get_error_string;
             get_error_string_return_0:
              x_635X = get_error_string0_return_value;
              *((long *) (SstackS)) = x_635X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21748;}}}
        else {
          goto L48116;}}
      else {
        goto L48116;}}
      break;
    case 116 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_636X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_637X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_638X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_639X = *((long *) (SstackS));
      if ((3 == (3 & arg5_639X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg5_639X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg3_637X | arg2_636X)))) {
            x_640X = SvalS;
            if ((1 == x_640X)) {
              goto L43733;}
            else {
              if ((5 == x_640X)) {
                goto L43733;}
              else {
                goto L43744;}}}
          else {
            goto L43744;}}
        else {
          goto L43744;}}
      else {
        goto L43744;}}
      break;
    case 117 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_641X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_642X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_643X = *((long *) (SstackS));
      if ((3 == (3 & arg4_643X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg4_643X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_641X | (SvalS))))) {
            start_644X = ((arg2_641X)>>2);
            count_645X = (((SvalS))>>2);
            v_646X = 8 == (*((long *) (((char *) (-3 + arg4_643X)))));
            if (v_646X) {
              if ((3 == (3 & arg3_642X))) {
                if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_642X))) + -4))))>>2))))) {
                  goto L37304;}
                else {
                  goto L37479;}}
              else {
                goto L37479;}}
            else {
              goto L37358;}}
          else {
            goto L43939;}}
        else {
          goto L43939;}}
      else {
        goto L43939;}}
      break;
    case 118 : {
      s48_make_availableAgc(264);
      obj_647X = SvalS;
      if ((3 == (3 & obj_647X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_647X))) + -4))))>>2))))) {
          channel_648X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_648X))))))) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = channel_648X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21748;}
          else {
            readyP_649X = ps_check_fd(((((*((long *) ((((char *) (-3 + channel_648X))) + 8))))>>2)), (4 == (*((long *) (((char *) (-3 + channel_648X)))))), &status_650X);
            if ((status_650X == NO_ERRORS)) {
              if (readyP_649X) {
                arg0K0 = 5;
                goto L40062;}
              else {
                arg0K0 = 1;
                goto L40062;}}
            else {
              push_exception_continuationB(24, 1);
              *((long *) (SstackS)) = channel_648X;
              SstackS = ((SstackS) + -4);
              merged_arg0K0 = status_650X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 1;
              goto get_error_string;
             get_error_string_return_1:
              x_651X = get_error_string0_return_value;
              *((long *) (SstackS)) = x_651X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21748;}}}
        else {
          goto L48138;}}
      else {
        goto L48138;}}
      break;
    case 119 : {
      obj_652X = SvalS;
      if ((3 == (3 & obj_652X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_652X))) + -4))))>>2))))) {
          channel_653X = SvalS;
          head_654X = Spending_channels_headS;
          if ((1 == head_654X)) {
            addr_655X = (((char *) (-3 + channel_653X))) + 16;
            S48_WRITE_BARRIER(channel_653X, addr_655X, 1);
            *((long *) addr_655X) = 1;
            n_656X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_653X))) + 8))))>>2)));
            arg0K0 = (((n_656X)<<2));
            goto L44081;}
          else {
            if ((channel_653X == head_654X)) {
              channel_657X = Spending_channels_headS;
              next_658X = *((long *) ((((char *) (-3 + channel_657X))) + 12));
              Spending_channels_headS = next_658X;
              addr_659X = (((char *) (-3 + channel_657X))) + 12;
              S48_WRITE_BARRIER(channel_657X, addr_659X, 1);
              *((long *) addr_659X) = 1;
              if ((1 == next_658X)) {
                Spending_channels_tailS = 1;
                goto L10837;}
              else {
                goto L10837;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_654X))) + 12)));
              arg0K1 = head_654X;
              goto L10843;}}}
        else {
          goto L44064;}}
      else {
        goto L44064;}}
      break;
    case 120 : {
      s48_make_availableAgc(((((3 * (Snumber_of_channelsS)))<<2)));
      arg0K0 = (-1 + (Snumber_of_channelsS));
      arg0K1 = 25;
      goto L15791;}
      break;
    case 121 : {
      SvalS = (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 122 : {
      proposal_660X = SvalS;
      if ((1 == proposal_660X)) {
        goto L37612;}
      else {
        if ((3 == (3 & proposal_660X))) {
          if ((2 == (31 & ((((*((long *) ((((char *) (-3 + proposal_660X))) + -4))))>>2))))) {
            if ((4 == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proposal_660X))) + -4))))>>8))))>>2)))) {
              if ((1 == (*((long *) (((char *) (-3 + proposal_660X))))))) {
                goto L37612;}
              else {
                goto L37643;}}
            else {
              goto L37643;}}
          else {
            goto L37643;}}
        else {
          goto L37643;}}}
      break;
    case 123 : {
      proposal_661X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
      if ((1 == proposal_661X)) {
        push_exception_continuationB(26, 1);
        arg0K0 = 0;
        goto L21748;}
      else {
        GET_PROPOSAL_LOCK();
        log_662X = *((long *) ((((char *) (-3 + proposal_661X))) + 4));
        arg0K0 = 0;
        goto L9246;}}
      break;
    case 124 : {
      stob_663X = SvalS;
      offset_664X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_665X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_663X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_663X))) + -4))))>>2))) == type_665X)) {
          x_666X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
          if ((1 == x_666X)) {
            arg0K0 = (*((long *) ((((char *) (-3 + stob_663X))) + (((offset_664X)<<2)))));
            goto L28460;}
          else {
            merged_arg0K0 = stob_663X;
            merged_arg0K1 = (((offset_664X)<<2));
            proposal_d_read_return_tag = 1;
            goto proposal_d_read;
           proposal_d_read_return_1:
            v_667X = proposal_d_read0_return_value;
            arg0K0 = v_667X;
            goto L28460;}}
        else {
          goto L28461;}}
      else {
        goto L28461;}}
      break;
    case 125 : {
      SstackS = ((SstackS) + 4);
      arg2_668X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_669X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_670X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_671X = *((long *) (SstackS));
      if ((0 == (3 & ((arg4_670X | arg2_668X) | (SvalS))))) {
        from_index_672X = ((arg4_670X)>>2);
        to_index_673X = ((arg2_668X)>>2);
        count_674X = (((SvalS))>>2);
        v_675X = *((unsigned char *) ((Scode_pointerS) + 1));
        merged_arg0K0 = arg5_671X;
        merged_arg0K1 = from_index_672X;
        merged_arg0K2 = count_674X;
        okay_copy_argsP_return_tag = 0;
        goto okay_copy_argsP;
       okay_copy_argsP_return_0:
        v_676X = okay_copy_argsP0_return_value;
        if (v_676X) {
          merged_arg0K0 = arg3_669X;
          merged_arg0K1 = to_index_673X;
          merged_arg0K2 = count_674X;
          okay_copy_argsP_return_tag = 1;
          goto okay_copy_argsP;
         okay_copy_argsP_return_1:
          v_677X = okay_copy_argsP0_return_value;
          if (v_677X) {
            if ((3 == (3 & arg3_669X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_669X))) + -4)))))) {
                if ((count_674X < 0)) {
                  goto L28554;}
                else {
                  if ((0 == v_675X)) {
                    goto L28614;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L28614;}
                    else {
                      arg0K0 = 4096;
                      arg0K1 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 12)));
                      goto L28702;}}}}
              else {
                goto L28554;}}
            else {
              goto L28554;}}
          else {
            goto L28554;}}
        else {
          goto L28554;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg5_671X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_670X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_669X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_668X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 5;
        goto L21748;}}
      break;
    case 126 : {
      SstackS = ((SstackS) + 4);
      arg2_678X = *((long *) (SstackS));
      if ((3 == (3 & arg2_678X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_678X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_679X = (((SvalS))>>2);
            len_680X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_678X))) + -4))))>>8);
            if ((index_679X < 0)) {
              goto L28911;}
            else {
              if ((index_679X < len_680X)) {
                x_681X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                if ((1 == x_681X)) {
                  arg0K0 = ((((*((unsigned char *) ((((char *) (-3 + arg2_678X))) + index_679X))))<<2));
                  goto L28910;}
                else {
                  index_682X = ((index_679X)<<2);
                  log_683X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                  arg0K0 = 0;
                  goto L14356;}}
              else {
                goto L28911;}}}
          else {
            goto L31709;}}
        else {
          goto L31709;}}
      else {
        goto L31709;}}
      break;
    case 127 : {
      SstackS = ((SstackS) + 4);
      arg2_684X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_685X = *((long *) (SstackS));
      if ((3 == (3 & arg3_685X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_685X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_684X))) {
            index_686X = ((arg2_684X)>>2);
            byte_687X = SvalS;
            if ((3 == (3 & arg3_685X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_685X))) + -4)))))) {
                if ((0 == (3 & byte_687X))) {
                  len_688X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_685X))) + -4))))>>8);
                  if ((index_686X < 0)) {
                    goto L29037;}
                  else {
                    if ((index_686X < len_688X)) {
                      x_689X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                      if ((1 == x_689X)) {
                        *((unsigned char *) ((((char *) (-3 + arg3_685X))) + index_686X)) = (((byte_687X)>>2));
                        goto L29036;}
                      else {
                        index_690X = ((index_686X)<<2);
                        log_691X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                        arg0K0 = 0;
                        goto L14532;}}
                    else {
                      goto L29037;}}}
                else {
                  goto L29002;}}
              else {
                goto L29002;}}
            else {
              goto L29002;}}
          else {
            goto L31812;}}
        else {
          goto L31812;}}
      else {
        goto L31812;}}
      break;
    case 128 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 129 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 130 : {
      x_692X = SvalS;
      push_exception_continuationB(15, 1);
      *((long *) (SstackS)) = x_692X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
      break;
    case 131 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 132 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 133 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_693X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_694X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_695X = *((long *) (SstackS));
      if ((3 == (3 & arg4_695X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_695X))) + -4))))>>2))))) {
          if ((3 == (3 & arg2_693X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_693X))) + -4))))>>2))))) {
              obj_696X = SvalS;
              if ((3 == (3 & obj_696X))) {
                if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_696X))) + -4))))>>2))))) {
                  undumpables_697X = SvalS;
                  port_698X = ps_open_output_file((((char *)(((char *) (-3 + arg4_695X))))), &status_699X);
                  if ((status_699X == NO_ERRORS)) {
                    status_700X = ps_write_string((((char *)(((char *) (-3 + arg2_693X))))), port_698X);
                    if ((status_700X == NO_ERRORS)) {
                      status_701X = s48_write_image(arg3_694X, undumpables_697X, port_698X);
                      if ((status_701X == NO_ERRORS)) {
                        status_702X = ps_close(port_698X);
                        if ((status_702X == NO_ERRORS)) {
                          SvalS = 13;
                          Scode_pointerS = ((Scode_pointerS) + 1);
                          arg1K0 = (Scode_pointerS);
                          goto L23693;}
                        else {
                          arg0K0 = 24;
                          arg0K1 = status_702X;
                          goto L41112;}}
                      else {
                        status_703X = ps_close(port_698X);
                        if ((status_703X == NO_ERRORS)) {
                          arg0K0 = 24;
                          arg0K1 = status_701X;
                          goto L41112;}
                        else {
                          ps_write_string("Unable to close image file", (stderr));
                          { long ignoreXX;
                          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                          arg0K0 = 24;
                          arg0K1 = status_701X;
                          goto L41112;}}}
                    else {
                      status_704X = ps_close(port_698X);
                      if ((status_704X == NO_ERRORS)) {
                        arg0K0 = 24;
                        arg0K1 = status_700X;
                        goto L41112;}
                      else {
                        ps_write_string("Unable to close image file", (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg0K0 = 24;
                        arg0K1 = status_700X;
                        goto L41112;}}}
                  else {
                    arg0K0 = 10;
                    arg0K1 = status_699X;
                    goto L41112;}}
                else {
                  goto L45400;}}
              else {
                goto L45400;}}
            else {
              goto L45400;}}
          else {
            goto L45400;}}
        else {
          goto L45400;}}
      else {
        goto L45400;}}
      break;
    case 134 : {
      SvalS = 13;
      s48_collect();
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 135 : {
      obj_705X = SvalS;
      if ((3 == (3 & obj_705X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_705X))) + -4))))>>2))))) {
          x_706X = SvalS;
          n_707X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_706X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L46041;}
        else {
          goto L46018;}}
      else {
        goto L46018;}}
      break;
    case 136 : {
      s48_make_availableAgc(24);
      SstackS = ((SstackS) + 4);
      stob_708X = *((long *) (SstackS));
      proc_709X = SvalS;
      if ((3 == (3 & stob_708X))) {
        if ((3 == (3 & proc_709X))) {
          if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_709X))) + -4))))>>2))))) {
            GET_PROPOSAL_LOCK();
            addr_710X = s48_allocate_small(12);
            *((long *) addr_710X) = 2050;
            x_711X = 3 + (((long) (addr_710X + 4)));
            *((long *) (((char *) (-3 + x_711X)))) = stob_708X;
            *((long *) ((((char *) (-3 + x_711X))) + 4)) = proc_709X;
            b_712X = SHARED_REF((Sfinalizer_alistS));
            addr_713X = s48_allocate_small(12);
            *((long *) addr_713X) = 2050;
            x_714X = 3 + (((long) (addr_713X + 4)));
            *((long *) (((char *) (-3 + x_714X)))) = x_711X;
            *((long *) ((((char *) (-3 + x_714X))) + 4)) = b_712X;
            SHARED_SETB((Sfinalizer_alistS), x_714X);
            RELEASE_PROPOSAL_LOCK();
            SvalS = 13;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23693;}
          else {
            goto L41387;}}
        else {
          goto L41387;}}
      else {
        goto L41387;}}
      break;
    case 137 : {
      SstackS = ((SstackS) + 4);
      arg2_715X = *((long *) (SstackS));
      if ((0 == (3 & arg2_715X))) {
        key_716X = ((arg2_715X)>>2);
        other_717X = SvalS;
        if ((5 == key_716X)) {
          SvalS = (-4 & other_717X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          if ((0 == key_716X)) {
            x_718X = s48_available();
            SvalS = (((x_718X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23693;}
          else {
            if ((1 == key_716X)) {
              bytes_719X = s48_heap_size();
              SvalS = (-4 & (3 + bytes_719X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23693;}
            else {
              if ((2 == key_716X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23693;}
              else {
                if ((3 == key_716X)) {
                  x_720X = s48_gc_count();
                  SvalS = (((x_720X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L23693;}
                else {
                  if ((4 == key_716X)) {
                    push_exception_continuationB(14, 1);
                    *((long *) (SstackS)) = (((key_716X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_717X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L21748;}
                  else {
                    push_exception_continuationB(17, 1);
                    *((long *) (SstackS)) = (((key_716X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_717X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L21748;}}}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_715X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
      break;
    case 138 : {
      if ((0 == (3 & (SvalS)))) {
        type_721X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L44200;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21748;}}
      break;
    case 139 : {
      x_722X = SvalS;
      arg2K0 = 1;
      arg0K1 = x_722X;
      goto L48240;}
      break;
    case 140 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 141 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 142 : {
      val_723X = SHARED_REF((Ssession_dataS));
      SvalS = val_723X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 143 : {
      SHARED_SETB((Ssession_dataS), (SvalS));
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 144 : {
      obj_724X = SvalS;
      if ((3 == (3 & obj_724X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_724X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 166)) {
            goto L26155;}
          else {
            temp_725X = SHARED_REF((Sexception_handlersS));
            SHARED_SETB((Sexception_handlersS), (SvalS));
            SvalS = temp_725X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23693;}}
        else {
          goto L26155;}}
      else {
        goto L26155;}}
      break;
    case 145 : {
      SstackS = ((SstackS) + 4);
      p_726X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      template_727X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      p_728X = *((long *) (SstackS));
      StemplateS = template_727X;
      Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_727X)))))))) + ((((((((((p_726X)>>2)) + (((p_728X)>>2))))<<2)))>>2)));
      arg1K0 = (Scode_pointerS);
      goto L23693;}
      break;
    case 146 : {
      obj_729X = SvalS;
      if ((3 == (3 & obj_729X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_729X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 5)) {
            goto L24619;}
          else {
            temp_730X = SHARED_REF((Sinterrupt_handlersS));
            SHARED_SETB((Sinterrupt_handlersS), (SvalS));
            SvalS = temp_730X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23693;}}
        else {
          goto L24619;}}
      else {
        goto L24619;}}
      break;
    case 147 : {
      old_731X = Senabled_interruptsS;
      p_732X = SvalS;
      Senabled_interruptsS = (((p_732X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L48288;}
        else {
          goto L48288;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L48288;}}
      break;
    case 148 : {
      SstackS = ((SstackS) + 4);
      p_733X = *((long *) (SstackS));
      Senabled_interruptsS = (((p_733X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L23707;}
        else {
          goto L23707;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L23707;}}
      break;
    case 149 : {
      if ((0 == (3 & (SvalS)))) {
        p_734X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          s48_Spending_interruptPS = 0;
          if ((s48_Spending_eventsPS)) {
            s48_Spending_interruptPS = 1;
            goto L46966;}
          else {
            goto L46966;}}
        else {
          s48_Spending_interruptPS = 1;
          goto L46966;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21748;}}
      break;
    case 150 : {
      SstackS = ((SstackS) + 4);
      arg2_735X = *((long *) (SstackS));
      if ((0 == (3 & arg2_735X))) {
        x_736X = SvalS;
        if ((1 == x_736X)) {
          goto L44950;}
        else {
          if ((5 == x_736X)) {
            goto L44950;}
          else {
            goto L44955;}}}
      else {
        goto L44955;}}
      break;
    case 151 : {
      SstackS = ((SstackS) + 4);
      nargs_737X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      p_738X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      rest_list_739X = *((long *) (SstackS));
      if ((14 < nargs_737X)) {
        push_exception_continuationB(19, 1);
        *((long *) (SstackS)) = (*((long *) ((SstackS) + (-4 & p_738X))));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = nargs_737X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}
      else {
        arg0K0 = rest_list_739X;
        goto L29236;}}
      break;
    case 152 : {
      s48_make_availableAgc(20);
      SstackS = ((SstackS) + 4);
      arg2_740X = *((long *) (SstackS));
      if ((3 == (3 & arg2_740X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_740X))) + -4))))>>2))))) {
          x_741X = SvalS;
          if ((1 == x_741X)) {
            goto L44295;}
          else {
            if ((5 == x_741X)) {
              goto L44295;}
            else {
              goto L44300;}}}
        else {
          goto L44300;}}
      else {
        goto L44300;}}
      break;
    case 153 : {
      SstackS = ((SstackS) + 4);
      arg2_742X = *((long *) (SstackS));
      if ((3 == (3 & arg2_742X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_742X))) + -4))))>>2))))) {
          x_743X = SvalS;
          if ((1 == x_743X)) {
            goto L31047;}
          else {
            if ((5 == x_743X)) {
              goto L31047;}
            else {
              goto L31052;}}}
        else {
          goto L31052;}}
      else {
        goto L31052;}}
      break;
    case 154 : {
      SstackS = ((SstackS) + 4);
      arg2_744X = *((long *) (SstackS));
      if ((0 == (3 & arg2_744X))) {
        option_745X = ((arg2_744X)>>2);
        other_746X = SvalS;
        if ((2 == option_745X)) {
          x_747X = CHEAP_TIME();
          SvalS = (((x_747X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          if ((0 == option_745X)) {
            seconds_748X = s48_run_time(&mseconds_749X);
            arg0K0 = option_745X;
            arg0K1 = seconds_748X;
            arg0K2 = mseconds_749X;
            goto L45952;}
          else {
            if ((1 == option_745X)) {
              seconds_750X = s48_real_time(&mseconds_751X);
              arg0K0 = option_745X;
              arg0K1 = seconds_750X;
              arg0K2 = mseconds_751X;
              goto L45952;}
            else {
              push_exception_continuationB(17, 1);
              *((long *) (SstackS)) = (((option_745X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = other_746X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21748;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_744X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
      break;
    case 155 : {
      SstackS = ((SstackS) + 4);
      arg2_752X = *((long *) (SstackS));
      if ((0 == (3 & arg2_752X))) {
        key_753X = ((arg2_752X)>>2);
        value_754X = SvalS;
        status_755X = s48_extended_vm(key_753X, value_754X);
        if ((0 == status_755X)) {
          SvalS = (s48_Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          if ((1 == status_755X)) {
            push_exception_continuationB(22, 1);
            *((long *) (SstackS)) = (((key_753X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_754X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21748;}
          else {
            push_exception_continuationB(23, 1);
            *((long *) (SstackS)) = (((key_753X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_754X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21748;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_752X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
      break;
    case 156 : {
      SstackS = ((SstackS) + 4);
      arg2_756X = *((long *) (SstackS));
      x_757X = SvalS;
      Senabled_interruptsS = -1;
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L48348;}
        else {
          goto L48348;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L48348;}}
      break;
    case 157 : {
      SstackS = ((SstackS) + 4);
      arg2_758X = *((long *) (SstackS));
      if ((3 == (3 & arg2_758X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_758X))) + -4))))>>2))))) {
          obj_759X = SvalS;
          if ((3 == (3 & obj_759X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_759X))) + -4))))>>2))))) {
              x_760X = SvalS;
              len_761X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_758X))) + -4))))>>8);
              if ((len_761X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_760X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_760X))), (void *)(((char *) (-3 + arg2_758X))),len_761X)))) {
                  arg0K0 = 5;
                  goto L38656;}
                else {
                  arg0K0 = 1;
                  goto L38656;}}
              else {
                arg0K0 = 1;
                goto L38656;}}
            else {
              goto L38623;}}
          else {
            goto L38623;}}
        else {
          goto L38623;}}
      else {
        goto L38623;}}
      break;
    case 158 : {
      s48_make_availableAgc((4 + (-4 & (4 + ((((SvalS))>>2))))));
      SstackS = ((SstackS) + 4);
      arg2_762X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        n_763X = (((SvalS))>>2);
        if ((3 == (3 & arg2_762X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_762X))) + -4))))>>2))))) {
            goto L38358;}
          else {
            goto L38310;}}
        else {
          goto L38310;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_762X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
      break;
    case 159 : {
      SstackS = ((SstackS) + 4);
      thing_764X = *((long *) (SstackS));
      list_765X = SvalS;
      arg0K0 = list_765X;
      arg0K1 = list_765X;
      arg2K2 = 1;
      goto L40125;}
      break;
    case 160 : {
      SstackS = ((SstackS) + 4);
      arg2_766X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_767X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        index_768X = (((SvalS))>>2);
        if ((3 == (3 & arg3_767X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg3_767X))) + -4))))>>2))))) {
            if ((arg2_766X == (*((long *) (((char *) (-3 + arg3_767X))))))) {
              len_769X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_767X))) + -4))))>>8))))>>2);
              if ((index_768X < 0)) {
                goto L29578;}
              else {
                if ((index_768X < len_769X)) {
                  v_770X = *((unsigned char *) ((Scode_pointerS) + 1));
                  if ((0 == v_770X)) {
                    goto L29568;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L29568;}
                    else {
                      merged_arg0K0 = arg3_767X;
                      merged_arg0K1 = (((index_768X)<<2));
                      proposal_d_read_return_tag = 2;
                      goto proposal_d_read;
                     proposal_d_read_return_2:
                      v_771X = proposal_d_read0_return_value;
                      arg0K0 = v_771X;
                      goto L29577;}}}
                else {
                  goto L29578;}}}
            else {
              goto L29598;}}
          else {
            goto L29598;}}
        else {
          goto L29598;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg3_767X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_766X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L21748;}}
      break;
    case 161 : {
      SstackS = ((SstackS) + 4);
      arg2_772X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_773X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_774X = *((long *) (SstackS));
      if ((0 == (3 & arg2_772X))) {
        index_775X = ((arg2_772X)>>2);
        value_776X = SvalS;
        if ((3 == (3 & arg4_774X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg4_774X))) + -4))))>>2))))) {
            if ((arg3_773X == (*((long *) (((char *) (-3 + arg4_774X))))))) {
              if ((3 == (3 & arg4_774X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_774X))) + -4)))))) {
                  len_777X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_774X))) + -4))))>>8))))>>2);
                  if ((index_775X < 0)) {
                    goto L29836;}
                  else {
                    if ((index_775X < len_777X)) {
                      v_778X = *((unsigned char *) ((Scode_pointerS) + 1));
                      if ((0 == v_778X)) {
                        goto L29826;}
                      else {
                        if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                          goto L29826;}
                        else {
                          merged_arg0K0 = arg4_774X;
                          merged_arg0K1 = (((index_775X)<<2));
                          merged_arg0K2 = value_776X;
                          proposal_d_write_return_tag = 2;
                          goto proposal_d_write;
                         proposal_d_write_return_2:
                          goto L29835;}}}
                    else {
                      goto L29836;}}}
                else {
                  goto L29858;}}
              else {
                goto L29858;}}
            else {
              goto L29858;}}
          else {
            goto L29858;}}
        else {
          goto L29858;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg4_774X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_773X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_772X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 4;
        goto L21748;}}
      break;
    case 162 : {
      v_779X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_779X)) {
        arg0K0 = (SvalS);
        goto L30115;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_780X = get_current_port0_return_value;
        arg0K0 = v_780X;
        goto L30115;}}
      break;
    case 163 : {
      v_781X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_781X)) {
        arg0K0 = (SvalS);
        goto L30356;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_782X = get_current_port0_return_value;
        arg0K0 = v_782X;
        goto L30356;}}
      break;
    case 164 : {
      v_783X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_783X)) {
        v_784X = SvalS;
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = v_784X;
        goto L30590;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_785X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_785X;
        goto L30590;}}
      break;
    case 165 : {
      x_786X = SvalS;
      out_787X = stderr;
      arg0K0 = x_786X;
      goto L40378;}
      break;
  }}
 L21748: {
  nargs_788X = arg0K0;
  opcode_789X = (((*((long *) ((SstackS) + (8 + (((nargs_788X)<<2)))))))>>2);
  handlers_790X = SHARED_REF((Sexception_handlersS));
  if ((3 == (3 & handlers_790X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_790X))) + -4))))>>2))))) {
      goto L21811;}
    else {
      goto L21884;}}
  else {
    goto L21884;}}
 L30965: {
  env_791X = arg0K0;
  i_792X = arg0K1;
  if ((0 == i_792X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_791X))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 3);
      arg0K0 = 0;
      goto L21748;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23693;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_791X)))));
    arg0K1 = (-1 + i_792X);
    goto L30965;}}
 L25660: {
  env_793X = arg0K0;
  i_794X = arg0K1;
  if ((0 == i_794X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_793X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 5);
      arg0K0 = 0;
      goto L21748;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23693;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_793X)))));
    arg0K1 = (-1 + i_794X);
    goto L25660;}}
 L25598: {
  env_795X = arg0K0;
  i_796X = arg0K1;
  if ((0 == i_796X)) {
    index_797X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
    value_798X = SvalS;
    addr_799X = (((char *) (-3 + env_795X))) + (((index_797X)<<2));
    S48_WRITE_BARRIER(env_795X, addr_799X, value_798X);
    *((long *) addr_799X) = value_798X;
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 5);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_795X)))));
    arg0K1 = (-1 + i_796X);
    goto L25598;}}
 L26335: {
  v_800X = arg0K0;
  s48_make_availableAgc((12 + (((v_800X)<<2))));
  p_801X = SenvS;
  if ((3 == (3 & p_801X))) {
    if ((p_801X < (((long) (Sstack_beginS))))) {
      goto L26389;}
    else {
      if (((((long) (Sstack_endS))) < p_801X)) {
        goto L26389;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = 0;
        merged_arg0K3 = 0;
        save_env_in_heap_return_tag = 0;
        goto save_env_in_heap;
       save_env_in_heap_return_0:
        v_802X = save_env_in_heap0_return_value;
        SenvS = v_802X;
        goto L26389;}}}
  else {
    goto L26389;}}
 L26348: {
  env_803X = arg0K0;
  key_804X = arg0K1;
  merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  merged_arg0K1 = env_803X;
  merged_arg0K2 = key_804X;
  make_closure_return_tag = 0;
  goto make_closure;
 make_closure_return_0:
  value_805X = make_closure0_return_value;
  SvalS = value_805X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L25385: {
  i_806X = arg0K0;
  if ((count_407X < i_806X)) {
    value_807X = *((long *) (((char *) (-3 + (SvalS)))));
    addr_808X = ((char *) (-3 + vector_410X));
    S48_WRITE_BARRIER(vector_410X, addr_808X, value_807X);
    *((long *) addr_808X) = value_807X;
    x_809X = SvalS;
    addr_810X = ((char *) (-3 + x_809X));
    S48_WRITE_BARRIER(x_809X, addr_810X, vector_410X);
    *((long *) addr_810X) = vector_410X;
    *((long *) (SstackS)) = (SenvS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (1034 + (((count_407X)<<10)));
    SstackS = ((SstackS) + -4);
    SenvS = (3 + (((long) ((SstackS) + 8))));
    Scode_pointerS = ((Scode_pointerS) + (3 + (((count_407X)<<1))));
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    index_811X = ((i_806X)<<1);
    merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + index_811X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_811X))))))<<2)))));
    merged_arg0K1 = (SvalS);
    merged_arg0K2 = 0;
    make_closure_return_tag = 1;
    goto make_closure;
   make_closure_return_1:
    closure_812X = make_closure0_return_value;
    addr_813X = (((char *) (-3 + vector_410X))) + (((i_806X)<<2));
    S48_WRITE_BARRIER(vector_410X, addr_813X, closure_812X);
    *((long *) addr_813X) = closure_812X;
    *((long *) (SstackS)) = closure_812X;
    SstackS = ((SstackS) + -4);
    arg0K0 = (1 + i_806X);
    goto L25385;}}
 L26672: {
  start_i_814X = arg0K0;
  arg0K0 = start_i_814X;
  arg0K1 = 2;
  arg0K2 = (SenvS);
  goto L26680;}
 L26523: {
  start_i_815X = arg0K0;
  arg0K0 = start_i_815X;
  arg0K1 = 3;
  arg0K2 = (SenvS);
  goto L26531;}
 L47651: {
  value_816X = arg0K0;
  SvalS = value_816X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L24350: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 0;
      goto check_events;
     check_events_return_0:
      v_817X = check_events0_return_value;
      if (v_817X) {
        arg0K0 = stack_arg_count_426X;
        goto L21981;}
      else {
        goto L24354;}}
    else {
      arg0K0 = stack_arg_count_426X;
      goto L21981;}}
  else {
    goto L24354;}}
 L24360: {
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L7909: {
  loc_818X = arg1K0;
  arg_819X = arg1K1;
  if ((top_of_args_432X < arg_819X)) {
    *((long *) loc_818X) = (*((long *) arg_819X));
    arg1K0 = (loc_818X + -4);
    arg1K1 = (arg_819X + -4);
    goto L7909;}
  else {
    SstackS = loc_818X;
    s48_call_native_code(proc_431X);
    goto L24360;}}
 L21378: {
  stack_arg_count_820X = arg0K0;
  list_args_821X = arg0K1;
  list_arg_count_822X = arg0K2;
  code_823X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  total_arg_count_824X = stack_arg_count_820X + list_arg_count_822X;
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_823X))) + 1)));
  arg0K1 = 64;
  goto L21393;}
 L22250: {
  exception_825X = arg0K0;
  stack_arg_count_826X = arg0K1;
  list_args_827X = arg0K2;
  list_arg_count_828X = arg0K3;
  if (((StemplateS) == (SvalS))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      return v_829X;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      return v_830X;}}
  else {
    merged_arg0K0 = list_args_827X;
    merged_arg0K1 = list_arg_count_828X;
    copy_listSAgc_return_tag = 0;
    goto copy_listSAgc;
   copy_listSAgc_return_0:
    v_831X = copy_listSAgc0_return_value;
    merged_arg0K0 = v_831X;
    merged_arg0K1 = stack_arg_count_826X;
    pop_args_GlistSAgc_return_tag = 1;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_1:
    args_832X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(exception_825X, 0);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_832X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}}
 L22879: {
  stack_arg_count_833X = arg0K0;
  list_args_834X = arg0K1;
  list_arg_count_835X = arg0K2;
  if ((0 == list_arg_count_835X)) {
    obj_836X = SvalS;
    if ((3 == (3 & obj_836X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_836X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_833X;
        arg0K1 = 25;
        arg0K2 = 0;
        goto L21378;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_833X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22250;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_833X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L22250;}}
  else {
    obj_837X = SvalS;
    if ((3 == (3 & obj_837X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_837X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_833X;
        arg0K1 = list_args_834X;
        arg0K2 = list_arg_count_835X;
        goto L21378;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_833X;
        arg0K2 = list_args_834X;
        arg0K3 = list_arg_count_835X;
        goto L22250;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_833X;
      arg0K2 = list_args_834X;
      arg0K3 = list_arg_count_835X;
      goto L22250;}}}
 L14861: {
  list_args_838X = arg0K0;
  stack_nargs_839X = arg0K1;
  merged_arg0K0 = list_args_838X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_840X = okay_argument_list0_return_value;
  list_arg_count_841X = okay_argument_list1_return_value;
  if (okayP_840X) {
    SvalS = proc_441X;
    arg0K0 = stack_nargs_839X;
    arg0K1 = list_args_838X;
    arg0K2 = list_arg_count_841X;
    goto L22879;}
  else {
    *((long *) (SstackS)) = list_args_838X;
    SstackS = ((SstackS) + -4);
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_839X);
    pop_args_GlistSAgc_return_tag = 2;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_2:
    args_842X = pop_args_GlistSAgc0_return_value;
    SstackS = ((SstackS) + 4);
    proc_843X = *((long *) (SstackS));
    push_exception_continuationB(5, 0);
    *((long *) (SstackS)) = proc_843X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_842X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}}
 L14902: {
  list_844X = arg0K0;
  follower_845X = arg0K1;
  if ((25 == list_844X)) {
    list_args_846X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_845X))) + 4)))))));
    addr_847X = (((char *) (-3 + follower_845X))) + 4;
    S48_WRITE_BARRIER(follower_845X, addr_847X, list_args_846X);
    *((long *) addr_847X) = list_args_846X;
    arg0K0 = rest_list_442X;
    arg0K1 = (-1 + stack_nargs_440X);
    goto L14861;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_844X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_845X))) + 4)));
    goto L14902;}}
 L22824: {
  obj_848X = SvalS;
  if ((3 == (3 & obj_848X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_848X))) + -4))))>>2))))) {
      arg0K0 = 0;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L21378;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L22250;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L22250;}}
 L22831: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_443X;
  ScontS = (Sbottom_of_stackS);
  goto L22824;}
 L24960: {
  cont_849X = ScontS;
  code_850X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_849X))) + 8)))))));
  pc_851X = (((*((long *) ((((char *) (-3 + cont_849X))) + 4))))>>2);
  protocol_852X = *((unsigned char *) ((((char *) (-3 + code_850X))) + (1 + pc_851X)));
  if ((1 == protocol_852X)) {
    goto L24989;}
  else {
    if ((66 == protocol_852X)) {
      goto L24989;}
    else {
      if ((71 == protocol_852X)) {
        cont_853X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
        if ((3 == (3 & cont_853X))) {
          if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_853X))) + -4))))>>2))))) {
            merged_arg0K0 = cont_853X;
            merged_arg0K1 = 0;
            copy_continuation_from_heapB_return_tag = 1;
            goto copy_continuation_from_heapB;
           copy_continuation_from_heapB_return_1:
            goto L24960;}
          else {
            arg0K0 = cont_853X;
            goto L22314;}}
        else {
          arg0K0 = cont_853X;
          goto L22314;}}
      else {
        if ((70 == protocol_852X)) {
          next_854X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_854X)))));
            goto L25007;}
          else {
            ScontS = next_854X;
            goto L25007;}}
        else {
          if ((65 == protocol_852X)) {
            index_855X = 2 + pc_851X;
            wants_stack_args_856X = ((((*((unsigned char *) ((((char *) (-3 + code_850X))) + index_855X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_850X))) + (1 + index_855X))));
            if ((0 == wants_stack_args_856X)) {
              pop_continuationB_return_tag = 0;
              goto pop_continuationB;
             pop_continuationB_return_0:
              s48_make_availableAgc(12);
              a_857X = SvalS;
              addr_858X = s48_allocate_small(12);
              *((long *) addr_858X) = 2050;
              x_859X = 3 + (((long) (addr_858X + 4)));
              *((long *) (((char *) (-3 + x_859X)))) = a_857X;
              *((long *) ((((char *) (-3 + x_859X))) + 4)) = 25;
              *((long *) (SstackS)) = x_859X;
              SstackS = ((SstackS) + -4);
              Scode_pointerS = ((Scode_pointerS) + 4);
              arg1K0 = (Scode_pointerS);
              goto L23693;}
            else {
              if ((1 == wants_stack_args_856X)) {
                pop_continuationB_return_tag = 1;
                goto pop_continuationB;
               pop_continuationB_return_1:
                *((long *) (SstackS)) = (SvalS);
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = 25;
                SstackS = ((SstackS) + -4);
                Scode_pointerS = ((Scode_pointerS) + 4);
                arg1K0 = (Scode_pointerS);
                goto L23693;}
              else {
                *((long *) (SstackS)) = (SvalS);
                SstackS = ((SstackS) + -4);
                merged_arg0K0 = 25;
                merged_arg0K1 = 1;
                pop_args_GlistSAgc_return_tag = 3;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_3:
                args_860X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_860X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21748;}}}
          else {
            *((long *) (SstackS)) = (SvalS);
            SstackS = ((SstackS) + -4);
            merged_arg0K0 = 25;
            merged_arg0K1 = 1;
            pop_args_GlistSAgc_return_tag = 4;
            goto pop_args_GlistSAgc;
           pop_args_GlistSAgc_return_4:
            args_861X = pop_args_GlistSAgc0_return_value;
            push_exception_continuationB(4, 0);
            *((long *) (SstackS)) = 1;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = args_861X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21748;}}}}}}
 L47114: {
  stack_nargs_862X = arg0K0;
  list_args_863X = arg0K1;
  list_arg_count_864X = arg0K2;
  cont_865X = ScontS;
  code_866X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_865X))) + 8)))))));
  pc_867X = (((*((long *) ((((char *) (-3 + cont_865X))) + 4))))>>2);
  protocol_868X = *((unsigned char *) ((((char *) (-3 + code_866X))) + (1 + pc_867X)));
  if ((1 == protocol_868X)) {
    if ((1 == (stack_nargs_862X + list_arg_count_864X))) {
      if ((1 == stack_nargs_862X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        goto L47209;}
      else {
        arg0K0 = (*((long *) (((char *) (-3 + list_args_863X)))));
        goto L47209;}}
    else {
      merged_arg0K0 = list_args_863X;
      merged_arg0K1 = stack_nargs_862X;
      pop_args_GlistSAgc_return_tag = 5;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_5:
      args_869X = pop_args_GlistSAgc0_return_value;
      push_exception_continuationB(4, 0);
      *((long *) (SstackS)) = 1;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = args_869X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}}
  else {
    if ((66 == protocol_868X)) {
      pop_continuationB_return_tag = 2;
      goto pop_continuationB;
     pop_continuationB_return_2:
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23693;}
    else {
      if ((71 == protocol_868X)) {
        cont_870X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
        if ((3 == (3 & cont_870X))) {
          if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_870X))) + -4))))>>2))))) {
            merged_arg0K0 = cont_870X;
            merged_arg0K1 = stack_nargs_862X;
            copy_continuation_from_heapB_return_tag = 2;
            goto copy_continuation_from_heapB;
           copy_continuation_from_heapB_return_2:
            arg0K0 = stack_nargs_862X;
            arg0K1 = list_args_863X;
            arg0K2 = list_arg_count_864X;
            goto L47114;}
          else {
            goto L47157;}}
        else {
          goto L47157;}}
      else {
        if ((70 == protocol_868X)) {
          next_871X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_871X)))));
            goto L47165;}
          else {
            ScontS = next_871X;
            goto L47165;}}
        else {
          if ((63 < protocol_868X)) {
            if ((65 == protocol_868X)) {
              index_872X = 2 + pc_867X;
              count_873X = ((((*((unsigned char *) ((((char *) (-3 + code_866X))) + index_872X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_866X))) + (1 + index_872X))));
              if (((stack_nargs_862X + list_arg_count_864X) < count_873X)) {
                merged_arg0K0 = list_args_863X;
                merged_arg0K1 = stack_nargs_862X;
                pop_args_GlistSAgc_return_tag = 6;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_6:
                args_874X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_874X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21748;}
              else {
                arg_top_875X = SstackS;
                pop_continuationB_return_tag = 3;
                goto pop_continuationB;
               pop_continuationB_return_3:
                arg1K0 = (SstackS);
                arg1K1 = (arg_top_875X + (((stack_nargs_862X)<<2)));
                goto L25945;}}
            else {
              if ((64 == protocol_868X)) {
                index_876X = 2 + pc_867X;
                arg0K0 = (((((*((unsigned char *) ((((char *) (-3 + code_866X))) + index_876X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_866X))) + (1 + index_876X)))));
                arg0K1 = 3;
                arg0K2 = stack_nargs_862X;
                arg0K3 = list_args_863X;
                arg0K4 = list_arg_count_864X;
                goto L25797;}
              else {
                ps_error("unknown protocol", 1, protocol_868X);
                merged_arg0K0 = list_args_863X;
                merged_arg0K1 = stack_nargs_862X;
                pop_args_GlistSAgc_return_tag = 7;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_7:
                args_877X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_877X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21748;}}}
          else {
            arg0K0 = protocol_868X;
            arg0K1 = 1;
            arg0K2 = stack_nargs_862X;
            arg0K3 = list_args_863X;
            arg0K4 = list_arg_count_864X;
            goto L25797;}}}}}}
 L24090: {
  tem_878X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  StemplateS = tem_878X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_878X)))))))) + 2);
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 1;
      goto check_events;
     check_events_return_1:
      v_879X = check_events0_return_value;
      if (v_879X) {
        arg0K0 = 0;
        goto L21981;}
      else {
        goto L24100;}}
    else {
      arg0K0 = 0;
      goto L21981;}}
  else {
    goto L24100;}}
 L24133: {
  push_exception_continuationB(5, 4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L23991: {
  arg0K0 = (2 + (((max_452X)<<1)));
  goto L23993;}
 L23993: {
  offset_880X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_880X);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L24876: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L23889: {
  delta_881X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_881X);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L24767: {
  delta_882X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_882X);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L47704: {
  val_883X = arg0K0;
  SvalS = val_883X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L47716: {
  val_884X = arg0K0;
  SvalS = val_884X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L13499: {
  if ((3 == (3 & x_466X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + x_466X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L47716;}
    else {
      goto L13505;}}
  else {
    goto L13505;}}
 L39205: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L39206: {
  if ((3 == (3 & n_467X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_467X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
    else {
      goto L39210;}}
  else {
    goto L39210;}}
 L39362: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L39337: {
  if ((3 == (3 & n_468X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      goto L39362;}
    else {
      goto L39351;}}
  else {
    goto L39351;}}
 L39561: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L39536: {
  if ((3 == (3 & n_469X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_469X))) + -4))))>>2))))) {
      goto L39561;}
    else {
      goto L39550;}}
  else {
    goto L39550;}}
 L39760: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L39735: {
  if ((3 == (3 & n_470X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_470X))) + -4))))>>2))))) {
      goto L39760;}
    else {
      goto L39749;}}
  else {
    goto L39749;}}
 L32461: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L32462: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_471X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L32587: {
  v_885X = (char *) s48_long_to_bignum(x_476X);
  v_886X = enter_bignum(v_885X);
  arg0K0 = v_886X;
  goto L32533;}
 L32533: {
  val_887X = arg0K0;
  SvalS = val_887X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L32539: {
  if ((0 == (3 & y_475X))) {
    goto L32545;}
  else {
    if ((3 == (3 & y_475X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_475X))) + -4))))>>2))))) {
        goto L32545;}
      else {
        goto L32548;}}
    else {
      goto L32548;}}}
 L32548: {
  if ((3 == (3 & x_474X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_474X))) + -4))))>>2))))) {
      if ((3 == (3 & y_475X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_475X))) + -4))))>>2))))) {
          Stemp0S = x_474X;
          Stemp1S = y_475X;
          s48_make_availableAgc(12);
          value_888X = Stemp1S;
          Stemp1S = 1;
          y_889X = *((double *) (((char *) (-3 + value_888X))));
          value_890X = Stemp0S;
          Stemp0S = 1;
          x_891X = *((double *) (((char *) (-3 + value_890X))));
          addr_892X = s48_allocate_small(12);
          *((long *) addr_892X) = 2126;
          Kdouble_893X = 3 + (((long) (addr_892X + 4)));
          *((double *) (((char *) (-3 + Kdouble_893X)))) = (x_891X + y_889X);
          SvalS = Kdouble_893X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          goto L32562;}}
      else {
        goto L32562;}}
    else {
      goto L32562;}}
  else {
    goto L32562;}}
 L8056: {
  a_894X = arg0K0;
  if ((b_480X < 0)) {
    arg0K0 = (0 - b_480X);
    goto L8060;}
  else {
    arg0K0 = b_480X;
    goto L8060;}}
 L41507: {
  if ((0 == (3 & y_478X))) {
    goto L41513;}
  else {
    if ((3 == (3 & y_478X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_478X))) + -4))))>>2))))) {
        goto L41513;}
      else {
        goto L41516;}}
    else {
      goto L41516;}}}
 L41516: {
  if ((3 == (3 & x_477X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_477X))) + -4))))>>2))))) {
      if ((3 == (3 & y_478X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_478X))) + -4))))>>2))))) {
          Stemp0S = x_477X;
          Stemp1S = y_478X;
          s48_make_availableAgc(12);
          value_895X = Stemp1S;
          Stemp1S = 1;
          y_896X = *((double *) (((char *) (-3 + value_895X))));
          value_897X = Stemp0S;
          Stemp0S = 1;
          x_898X = *((double *) (((char *) (-3 + value_897X))));
          addr_899X = s48_allocate_small(12);
          *((long *) addr_899X) = 2126;
          Kdouble_900X = 3 + (((long) (addr_899X + 4)));
          *((double *) (((char *) (-3 + Kdouble_900X)))) = (x_898X * y_896X);
          SvalS = Kdouble_900X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          goto L41530;}}
      else {
        goto L41530;}}
    else {
      goto L41530;}}
  else {
    goto L41530;}}
 L32876: {
  v_901X = (char *) s48_long_to_bignum(x_483X);
  v_902X = enter_bignum(v_901X);
  arg0K0 = v_902X;
  goto L32822;}
 L32822: {
  val_903X = arg0K0;
  SvalS = val_903X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L32828: {
  if ((0 == (3 & y_482X))) {
    goto L32834;}
  else {
    if ((3 == (3 & y_482X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_482X))) + -4))))>>2))))) {
        goto L32834;}
      else {
        goto L32837;}}
    else {
      goto L32837;}}}
 L32837: {
  if ((3 == (3 & x_481X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_481X))) + -4))))>>2))))) {
      if ((3 == (3 & y_482X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_482X))) + -4))))>>2))))) {
          Stemp0S = x_481X;
          Stemp1S = y_482X;
          s48_make_availableAgc(12);
          value_904X = Stemp1S;
          Stemp1S = 1;
          y_905X = *((double *) (((char *) (-3 + value_904X))));
          value_906X = Stemp0S;
          Stemp0S = 1;
          x_907X = *((double *) (((char *) (-3 + value_906X))));
          addr_908X = s48_allocate_small(12);
          *((long *) addr_908X) = 2126;
          Kdouble_909X = 3 + (((long) (addr_908X + 4)));
          *((double *) (((char *) (-3 + Kdouble_909X)))) = (x_907X - y_905X);
          SvalS = Kdouble_909X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23693;}
        else {
          goto L32851;}}
      else {
        goto L32851;}}
    else {
      goto L32851;}}
  else {
    goto L32851;}}
 L8314: {
  a_910X = arg0K0;
  if ((b_487X < 0)) {
    arg0K0 = (0 - b_487X);
    goto L8318;}
  else {
    arg0K0 = b_487X;
    goto L8318;}}
 L41760: {
  if ((0 == (3 & y_485X))) {
    goto L41766;}
  else {
    if ((3 == (3 & y_485X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_485X))) + -4))))>>2))))) {
        goto L41766;}
      else {
        goto L41793;}}
    else {
      goto L41793;}}}
 L41793: {
  if ((3 == (3 & x_484X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_484X))) + -4))))>>2))))) {
      if ((3 == (3 & y_485X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_485X))) + -4))))>>2))))) {
          if ((0. == (*((double *) (((char *) (-3 + y_485X))))))) {
            arg0K0 = 1;
            goto L41806;}
          else {
            Stemp0S = x_484X;
            Stemp1S = y_485X;
            s48_make_availableAgc(12);
            value_911X = Stemp1S;
            Stemp1S = 1;
            y_912X = *((double *) (((char *) (-3 + value_911X))));
            value_913X = Stemp0S;
            Stemp0S = 1;
            value_914X = (*((double *) (((char *) (-3 + value_913X))))) / y_912X;
            addr_915X = s48_allocate_small(12);
            *((long *) addr_915X) = 2126;
            Kdouble_916X = 3 + (((long) (addr_915X + 4)));
            *((double *) (((char *) (-3 + Kdouble_916X)))) = value_914X;
            arg0K0 = Kdouble_916X;
            goto L41806;}}
        else {
          goto L41813;}}
      else {
        goto L41813;}}
    else {
      goto L41813;}}
  else {
    goto L41813;}}
 L33107: {
  val_917X = arg0K0;
  SvalS = val_917X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L33113: {
  if ((0 == (3 & y_489X))) {
    goto L33119;}
  else {
    if ((3 == (3 & y_489X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_489X))) + -4))))>>2))))) {
        goto L33119;}
      else {
        goto L33124;}}
    else {
      goto L33124;}}}
 L33124: {
  if ((3 == (3 & x_488X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_488X))) + -4))))>>2))))) {
      if ((3 == (3 & y_489X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_489X))) + -4))))>>2))))) {
          b_918X = (*((double *) (((char *) (-3 + x_488X))))) == (*((double *) (((char *) (-3 + y_489X)))));
          if (b_918X) {
            arg0K0 = 5;
            goto L33139;}
          else {
            arg0K0 = 1;
            goto L33139;}}
        else {
          goto L33140;}}
      else {
        goto L33140;}}
    else {
      goto L33140;}}
  else {
    goto L33140;}}
 L33367: {
  val_919X = arg0K0;
  SvalS = val_919X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L33373: {
  if ((0 == (3 & y_491X))) {
    goto L33379;}
  else {
    if ((3 == (3 & y_491X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_491X))) + -4))))>>2))))) {
        goto L33379;}
      else {
        goto L33384;}}
    else {
      goto L33384;}}}
 L33384: {
  if ((3 == (3 & x_490X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_490X))) + -4))))>>2))))) {
      if ((3 == (3 & y_491X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_491X))) + -4))))>>2))))) {
          b_920X = (*((double *) (((char *) (-3 + x_490X))))) < (*((double *) (((char *) (-3 + y_491X)))));
          if (b_920X) {
            arg0K0 = 5;
            goto L33399;}
          else {
            arg0K0 = 1;
            goto L33399;}}
        else {
          goto L33400;}}
      else {
        goto L33400;}}
    else {
      goto L33400;}}
  else {
    goto L33400;}}
 L33690: {
  val_921X = arg0K0;
  SvalS = val_921X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L33696: {
  if ((0 == (3 & y_493X))) {
    goto L33702;}
  else {
    if ((3 == (3 & y_493X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_493X))) + -4))))>>2))))) {
        goto L33702;}
      else {
        goto L33707;}}
    else {
      goto L33707;}}}
 L33707: {
  if ((3 == (3 & x_492X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_492X))) + -4))))>>2))))) {
      if ((3 == (3 & y_493X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_493X))) + -4))))>>2))))) {
          b_922X = (*((double *) (((char *) (-3 + y_493X))))) < (*((double *) (((char *) (-3 + x_492X)))));
          if (b_922X) {
            arg0K0 = 5;
            goto L33722;}
          else {
            arg0K0 = 1;
            goto L33722;}}
        else {
          goto L33723;}}
      else {
        goto L33723;}}
    else {
      goto L33723;}}
  else {
    goto L33723;}}
 L34013: {
  val_923X = arg0K0;
  SvalS = val_923X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L34019: {
  if ((0 == (3 & y_495X))) {
    goto L34025;}
  else {
    if ((3 == (3 & y_495X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_495X))) + -4))))>>2))))) {
        goto L34025;}
      else {
        goto L34030;}}
    else {
      goto L34030;}}}
 L34030: {
  if ((3 == (3 & x_494X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_494X))) + -4))))>>2))))) {
      if ((3 == (3 & y_495X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_495X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + y_495X))))) < (*((double *) (((char *) (-3 + x_494X))))))) {
            arg0K0 = 1;
            goto L34045;}
          else {
            arg0K0 = 5;
            goto L34045;}}
        else {
          goto L34046;}}
      else {
        goto L34046;}}
    else {
      goto L34046;}}
  else {
    goto L34046;}}
 L34307: {
  val_924X = arg0K0;
  SvalS = val_924X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L34313: {
  if ((0 == (3 & y_497X))) {
    goto L34319;}
  else {
    if ((3 == (3 & y_497X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_497X))) + -4))))>>2))))) {
        goto L34319;}
      else {
        goto L34324;}}
    else {
      goto L34324;}}}
 L34324: {
  if ((3 == (3 & x_496X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_496X))) + -4))))>>2))))) {
      if ((3 == (3 & y_497X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_497X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + x_496X))))) < (*((double *) (((char *) (-3 + y_497X))))))) {
            arg0K0 = 1;
            goto L34339;}
          else {
            arg0K0 = 5;
            goto L34339;}}
        else {
          goto L34340;}}
      else {
        goto L34340;}}
    else {
      goto L34340;}}
  else {
    goto L34340;}}
 L19013: {
  x_925X = arg0K0;
  y_926X = arg0K1;
  if ((0 == (3 & x_925X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19043;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_925X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19043;}}
 L8959: {
  a_927X = arg0K0;
  if ((b_501X < 0)) {
    arg0K0 = (0 - b_501X);
    goto L8963;}
  else {
    arg0K0 = b_501X;
    goto L8963;}}
 L34613: {
  if ((0 == (3 & y_499X))) {
    goto L34619;}
  else {
    if ((3 == (3 & y_499X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_499X))) + -4))))>>2))))) {
        goto L34619;}
      else {
        goto L34622;}}
    else {
      goto L34622;}}}
 L34622: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_498X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_499X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L18922: {
  x_928X = arg0K0;
  y_929X = arg0K1;
  if ((0 == (3 & x_928X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18952;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_928X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18952;}}
 L34827: {
  a_930X = arg0K0;
  n_931X = ((y_503X)>>2);
  if ((n_931X < 0)) {
    arg0K0 = (0 - n_931X);
    goto L34829;}
  else {
    arg0K0 = n_931X;
    goto L34829;}}
 L34789: {
  if ((0 == (3 & y_503X))) {
    goto L34795;}
  else {
    if ((3 == (3 & y_503X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_503X))) + -4))))>>2))))) {
        goto L34795;}
      else {
        goto L34798;}}
    else {
      goto L34798;}}}
 L34798: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_502X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_503X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L34976: {
  SvalS = n_505X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L34977: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_505X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L35031: {
  SvalS = n_506X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L35032: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_506X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L35086: {
  SvalS = 4;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L35089: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_507X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L35146: {
  SvalS = n_508X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L35147: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_508X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L35201: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L35204: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_509X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L47870: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_518X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L47876: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L22499: {
  x_932X = SvalS;
  if ((0 == (3 & x_932X))) {
    s48_make_availableAgc(16);
    if ((x_932X < 0)) {
      arg0K0 = (0 - x_932X);
      goto L22527;}
    else {
      arg0K0 = x_932X;
      goto L22527;}}
  else {
    if ((0 == (3 & x_932X))) {
      arg0K0 = 1;
      arg0K1 = 3;
      goto L18530;}
    else {
      arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_932X))) + -4))))>>8))))>>2)));
      arg0K1 = 0;
      goto L18530;}}}
 L22502: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L35355: {
  if ((0 == (3 & x_528X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18877;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_528X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18877;}}
 L35358: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_528X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L35446: {
  x_933X = arg0K0;
  arg0K0 = x_933X;
  arg0K1 = 0;
  goto L35452;}
 L35429: {
  if ((0 == (3 & x_529X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18819;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_529X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18819;}}
 L35432: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_529X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L35531: {
  if ((0 == (3 & y_532X))) {
    goto L35537;}
  else {
    if ((3 == (3 & y_532X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_532X))) + -4))))>>2))))) {
        goto L35537;}
      else {
        goto L35540;}}
    else {
      goto L35540;}}}
 L35540: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_531X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_532X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L35694: {
  if ((0 == (3 & y_534X))) {
    goto L35700;}
  else {
    if ((3 == (3 & y_534X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_534X))) + -4))))>>2))))) {
        goto L35700;}
      else {
        goto L35703;}}
    else {
      goto L35703;}}}
 L35703: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_533X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_534X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L35857: {
  if ((0 == (3 & y_536X))) {
    goto L35863;}
  else {
    if ((3 == (3 & y_536X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_536X))) + -4))))>>2))))) {
        goto L35863;}
      else {
        goto L35866;}}
    else {
      goto L35866;}}}
 L35866: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_535X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_536X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L32241: {
  val_934X = arg0K0;
  SvalS = val_934X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L32257: {
  val_935X = arg0K0;
  SvalS = val_935X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L32258: {
  push_exception_continuationB(6, 1);
  *((long *) (SstackS)) = x_537X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_538X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L42086: {
  if ((0 == (3 & y_538X))) {
    if ((0 == (3 & x_537X))) {
      value_936X = ((x_537X)>>2);
      count_937X = ((y_538X)>>2);
      if ((count_937X < 0)) {
        PS_SHIFT_RIGHT(value_936X, (0 - count_937X), x_938X)
        x_939X = x_938X;
        s48_make_availableAgc(16);
        if ((536870911 < x_939X)) {
          goto L42206;}
        else {
          if ((x_939X < -536870912)) {
            goto L42206;}
          else {
            arg0K0 = (((x_939X)<<2));
            goto L42201;}}}
      else {
        PS_SHIFT_LEFT(value_936X, count_937X, x_940X)
        result_941X = x_940X;
        PS_SHIFT_RIGHT(result_941X, count_937X, x_942X)
        if ((value_936X == x_942X)) {
          if ((value_936X < 0)) {
            if ((result_941X < 0)) {
              s48_make_availableAgc(16);
              if ((536870911 < result_941X)) {
                goto L42228;}
              else {
                if ((result_941X < -536870912)) {
                  goto L42228;}
                else {
                  arg0K0 = (((result_941X)<<2));
                  goto L42223;}}}
            else {
              arg0K0 = x_537X;
              arg0K1 = y_538X;
              goto L42099;}}
          else {
            if ((result_941X < 0)) {
              arg0K0 = x_537X;
              arg0K1 = y_538X;
              goto L42099;}
            else {
              s48_make_availableAgc(16);
              if ((536870911 < result_941X)) {
                goto L42250;}
              else {
                if ((result_941X < -536870912)) {
                  goto L42250;}
                else {
                  arg0K0 = (((result_941X)<<2));
                  goto L42245;}}}}}
        else {
          arg0K0 = x_537X;
          arg0K1 = y_538X;
          goto L42099;}}}
    else {
      if ((3 == (3 & x_537X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_537X))) + -4))))>>2))))) {
          y_943X = ((y_538X)>>2);
          merged_arg0K0 = x_537X;
          merged_arg0K1 = y_943X;
          shift_space_return_tag = 0;
          goto shift_space;
         shift_space_return_0:
          needed_944X = shift_space0_return_value;
          Stemp0S = x_537X;
          s48_make_availableAgc((((needed_944X)<<2)));
          value_945X = Stemp0S;
          Stemp0S = 1;
          if ((0 == (3 & value_945X))) {
            v_946X = (char *) s48_long_to_bignum((((value_945X)>>2)));
            arg1K0 = v_946X;
            goto L42281;}
          else {
            arg1K0 = (((char *) (-3 + value_945X)));
            goto L42281;}}
        else {
          goto L42108;}}
      else {
        goto L42108;}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_537X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_538X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}}
 L47967: {
  val_947X = arg0K0;
  SvalS = val_947X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L38980: {
  val_948X = arg0K0;
  SvalS = val_948X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L38953: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_542X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L38890: {
  val_949X = arg0K0;
  SvalS = val_949X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L38863: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_544X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L43276: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((x_546X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L47996: {
  val_950X = arg0K0;
  SvalS = val_950X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L48010: {
  value_951X = arg0K0;
  SvalS = value_951X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L26860: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = stob_550X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_551X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L26965: {
  SvalS = new_556X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L26949: {
  i_952X = arg0K0;
  if ((i_952X < 0)) {
    goto L26965;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_556X))) + (((i_952X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_952X);
    goto L26949;}}
 L27081: {
  i_953X = arg0K0;
  if ((i_953X < 0)) {
    arg0K0 = stack_nargs_562X;
    arg0K1 = rest_list_563X;
    goto L27215;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_561X))) + (((i_953X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_953X);
    goto L27081;}}
 L27241: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_564X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_566X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_565X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L27358: {
  addr_954X = (((char *) (-3 + stob_567X))) + (((offset_569X)<<2));
  S48_WRITE_BARRIER(stob_567X, addr_954X, value_568X);
  *((long *) addr_954X) = value_568X;
  goto L27367;}
 L27367: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L27368: {
  push_exception_continuationB(5, 4);
  *((long *) (SstackS)) = stob_567X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_570X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_569X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_568X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L27585: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = (((type_574X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_575X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = init_573X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L27602: {
  x_955X = arg0K0;
  value_956X = Stemp0S;
  Stemp0S = 1;
  if ((1 == x_955X)) {
    push_exception_continuationB(8, 2);
    *((long *) (SstackS)) = (((type_574X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_575X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = value_956X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L21748;}
  else {
    arg0K0 = (-1 + len_575X);
    goto L27628;}}
 L27875: {
  push_exception_continuationB(7, 3);
  *((long *) (SstackS)) = stob_578X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_580X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_579X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L27865: {
  arg0K0 = (*((long *) ((((char *) (-3 + stob_578X))) + (-4 & index_579X))));
  goto L27874;}
 L27874: {
  value_957X = arg0K0;
  SvalS = value_957X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L27830: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_578X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_580X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_579X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L28162: {
  push_exception_continuationB(7, 3);
  *((long *) (SstackS)) = stob_586X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_588X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_585X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_587X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L28152: {
  addr_958X = (((char *) (-3 + stob_586X))) + (-4 & arg2_585X);
  S48_WRITE_BARRIER(stob_586X, addr_958X, value_587X);
  *((long *) addr_958X) = value_587X;
  goto L28161;}
 L28161: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L28115: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_586X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_588X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_585X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_587X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L36078: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_593X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_594X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L36091: {
  vector_959X = arg0K0;
  if ((1 == vector_959X)) {
    push_exception_continuationB(8, 1);
    *((long *) (SstackS)) = (((len_593X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((init_594X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}
  else {
    arg0K0 = (-1 + len_593X);
    goto L36113;}}
 L46781: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L43382: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_597X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_598X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L46297: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_597X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L40729: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_601X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_602X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_603X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L40708: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_601X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_602X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_603X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L45077: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_601X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_600X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L36258: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_606X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_607X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L36271: {
  vector_960X = arg0K0;
  if ((1 == vector_960X)) {
    push_exception_continuationB(8, 1);
    *((long *) (SstackS)) = (((len_606X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (9 + ((((init_607X))<<8)));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_960X))) + len_606X)) = 0;
    arg0K0 = (-1 + len_606X);
    goto L36293;}}
 L43463: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_605X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L46831: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L43526: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_611X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_612X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L46393: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_611X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L40884: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_615X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_616X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_617X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L40863: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_615X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_616X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_617X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L45203: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_615X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_614X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L20303: {
  i_961X = arg0K0;
  h_962X = arg0K1;
  if ((i_961X < n_622X)) {
    arg0K0 = (1 + i_961X);
    arg0K1 = (h_962X + (((*((unsigned char *) ((((char *) (-3 + string_621X))) + i_961X))))));
    goto L20303;}
  else {
    index_963X = 1023 & h_962X;
    link_964X = *((long *) ((((char *) (-3 + table_620X))) + (((index_963X)<<2))));
    if ((0 == (3 & link_964X))) {
      arg0K0 = (3 + (-4 & link_964X));
      goto L20259;}
    else {
      arg0K0 = link_964X;
      goto L20259;}}}
 L32403: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L44767: {
  val_965X = arg0K0;
  SvalS = val_965X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L44748: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L40471: {
  if ((1 == (SvalS))) {
    addr_966X = (((char *) (-3 + arg2_626X))) + 4;
    S48_WRITE_BARRIER(arg2_626X, addr_966X, 273);
    *((long *) addr_966X) = 273;
    goto L40477;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_626X))) + 4)))))) {
      addr_967X = (((char *) (-3 + arg2_626X))) + 4;
      S48_WRITE_BARRIER(arg2_626X, addr_967X, 529);
      *((long *) addr_967X) = 529;
      goto L40477;}
    else {
      goto L40477;}}}
 L40478: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_626X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L48082: {
  val_968X = arg0K0;
  SvalS = val_968X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L43608: {
  SvalS = x_629X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L36665: {
  if ((0 == (3 & arg2_630X))) {
    if (((((arg2_630X)>>2)) < 0)) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_630X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_631X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}
    else {
      arg0K0 = (((arg2_630X)>>2));
      goto L36485;}}
  else {
    if ((3 == (3 & arg2_630X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_630X))) + -4))))>>2))))) {
        if ((1 == mode_631X)) {
          goto L36543;}
        else {
          if ((3 == mode_631X)) {
            goto L36543;}
          else {
            v_969X = ps_open_fd((((char *)(((char *) (-3 + arg2_630X))))), 0, &v_970X);
            arg0K0 = v_969X;
            arg0K1 = v_970X;
            goto L36556;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_630X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_631X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_630X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_631X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}}}
 L48116: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L43733: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L43743;}
  else {
    arg2K0 = 1;
    goto L43743;}}
 L43744: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_639X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_638X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_637X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_636X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21748;}
 L37304: {
  if ((3 == (3 & arg3_642X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_642X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_642X))) + -4))))>>8)));
      goto L37306;}
    else {
      goto L37493;}}
  else {
    goto L37493;}}
 L37479: {
  if ((3 == (3 & arg3_642X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_642X))) + -4))))>>2))))) {
      goto L37304;}
    else {
      goto L37358;}}
  else {
    goto L37358;}}
 L37358: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_643X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_642X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_644X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_645X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L43939: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_643X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_642X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_641X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L40062: {
  val_971X = arg0K0;
  SvalS = val_971X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L48138: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L44081: {
  val_972X = arg0K0;
  SvalS = val_972X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L10837: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_653X))) + 16)));
  goto L44081;}
 L10843: {
  ch_973X = arg0K0;
  prev_974X = arg0K1;
  if ((1 == ch_973X)) {
    addr_975X = (((char *) (-3 + channel_653X))) + 16;
    S48_WRITE_BARRIER(channel_653X, addr_975X, 1);
    *((long *) addr_975X) = 1;
    n_976X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_653X))) + 8))))>>2)));
    arg0K0 = (((n_976X)<<2));
    goto L44081;}
  else {
    if ((ch_973X == channel_653X)) {
      y_977X = Spending_channels_tailS;
      if ((ch_973X == y_977X)) {
        Spending_channels_tailS = prev_974X;
        goto L10867;}
      else {
        goto L10867;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_973X))) + 12)));
      arg0K1 = ch_973X;
      goto L10843;}}}
 L44064: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L15791: {
  i_978X = arg0K0;
  res_979X = arg0K1;
  if ((-1 == i_978X)) {
    SvalS = res_979X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    channel_980X = *((Svm_channelsS) + i_978X);
    if ((3 == (3 & channel_980X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_980X))) + -4))))>>2))))) {
        addr_981X = s48_allocate_small(12);
        *((long *) addr_981X) = 2050;
        x_982X = 3 + (((long) (addr_981X + 4)));
        *((long *) (((char *) (-3 + x_982X)))) = channel_980X;
        *((long *) ((((char *) (-3 + x_982X))) + 4)) = res_979X;
        arg0K0 = x_982X;
        goto L15805;}
      else {
        arg0K0 = res_979X;
        goto L15805;}}
    else {
      arg0K0 = res_979X;
      goto L15805;}}}
 L37612: {
  old_983X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 == old_983X)) {
    goto L37628;}
  else {
    addr_984X = ((char *) (-3 + old_983X));
    S48_WRITE_BARRIER(old_983X, addr_984X, 1);
    *((long *) addr_984X) = 1;
    goto L37628;}}
 L37643: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proposal_660X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L9246: {
  i_985X = arg0K0;
  stob_986X = *((long *) ((((char *) (-3 + log_662X))) + (((i_985X)<<2))));
  if ((1 == stob_986X)) {
    log_987X = *((long *) ((((char *) (-3 + proposal_661X))) + 8));
    arg0K0 = 0;
    goto L9484;}
  else {
    verify_988X = *((long *) ((((char *) (-3 + log_662X))) + (12 + (((i_985X)<<2)))));
    value_989X = *((long *) ((((char *) (-3 + log_662X))) + (8 + (((i_985X)<<2)))));
    if ((29 == verify_988X)) {
      if ((3 == (3 & stob_986X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_986X))) + -4)))))) {
          goto L9298;}
        else {
          goto L37874;}}
      else {
        goto L37874;}}
    else {
      if ((verify_988X == (*((long *) ((((char *) (-3 + stob_986X))) + (-4 & (*((long *) ((((char *) (-3 + log_662X))) + (4 + (((i_985X)<<2)))))))))))) {
        if ((verify_988X == value_989X)) {
          goto L9298;}
        else {
          if ((3 == (3 & stob_986X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_986X))) + -4)))))) {
              goto L9298;}
            else {
              goto L37874;}}
          else {
            goto L37874;}}}
      else {
        goto L37874;}}}}
 L28460: {
  value_990X = arg0K0;
  SvalS = value_990X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L28461: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_663X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_665X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_664X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L28554: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg5_671X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_672X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_669X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_673X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_674X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21748;}
 L28614: {
  memcpy((void *)((((char *) (-3 + arg3_669X))) + to_index_673X), (void *)((((char *) (-3 + arg5_671X))) + from_index_672X),count_674X);
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L28702: {
  left_991X = arg0K0;
  copies_992X = arg0K1;
  if ((1 == copies_992X)) {
    if ((left_991X < count_674X)) {
      goto L28554;}
    else {
      from_index_993X = ((from_index_672X)<<2);
      to_index_994X = ((to_index_673X)<<2);
      count_995X = ((count_674X)<<2);
      Stemp0S = arg5_671X;
      Stemp1S = arg3_669X;
      addr_996X = s48_allocate_tracedAgc(28);
      if ((addr_996X == NULL)) {
        arg0K0 = 1;
        goto L10054;}
      else {
        *((long *) addr_996X) = 6154;
        arg0K0 = (3 + (((long) (addr_996X + 4))));
        goto L10054;}}}
  else {
    arg0K0 = (left_991X - ((((*((long *) ((((char *) (-3 + copies_992X))) + 16))))>>2)));
    arg0K1 = (*((long *) ((((char *) (-3 + copies_992X))) + 20)));
    goto L28702;}}
 L28911: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_678X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_679X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L28910: {
  value_997X = arg0K0;
  SvalS = value_997X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L14356: {
  i_998X = arg0K0;
  next_stob_999X = *((long *) ((((char *) (-3 + log_683X))) + (((i_998X)<<2))));
  if ((1 == next_stob_999X)) {
    v_1000X = add_log_entryAgc(2, i_998X, arg2_678X, index_682X, ((((*((unsigned char *) ((((char *) (-3 + arg2_678X))) + (((index_682X)>>2))))))<<2)), 1);
    arg0K0 = v_1000X;
    goto L28910;}
  else {
    if ((arg2_678X == next_stob_999X)) {
      if ((index_682X == (*((long *) ((((char *) (-3 + log_683X))) + (4 + (((i_998X)<<2)))))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + log_683X))) + (8 + (((i_998X)<<2))))));
        goto L28910;}
      else {
        goto L14378;}}
    else {
      goto L14378;}}}
 L31709: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_678X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L29037: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_685X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_686X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_687X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L29036: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L14532: {
  i_1001X = arg0K0;
  next_stob_1002X = *((long *) ((((char *) (-3 + log_691X))) + (((i_1001X)<<2))));
  if ((1 == next_stob_1002X)) {
    add_log_entryAgc(2, i_1001X, arg3_685X, index_690X, byte_687X, 0);
    goto L29036;}
  else {
    if ((arg3_685X == next_stob_1002X)) {
      if ((index_690X == (*((long *) ((((char *) (-3 + log_691X))) + (4 + (((i_1001X)<<2)))))))) {
        addr_1003X = (((char *) (-3 + log_691X))) + (8 + (((i_1001X)<<2)));
        S48_WRITE_BARRIER(log_691X, addr_1003X, byte_687X);
        *((long *) addr_1003X) = byte_687X;
        goto L29036;}
      else {
        goto L14552;}}
    else {
      goto L14552;}}}
 L29002: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_685X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_686X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_687X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L31812: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_685X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_684X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L41112: {
  reason_1004X = arg0K0;
  status_1005X = arg0K1;
  push_exception_continuationB(reason_1004X, 1);
  *((long *) (SstackS)) = arg4_695X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_694X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_693X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_1005X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 2;
  goto get_error_string;
 get_error_string_return_2:
  x_1006X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_1006X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L45400: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_695X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_694X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_693X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L46041: {
  i_1007X = arg0K0;
  h_1008X = arg0K1;
  if ((i_1007X < n_707X)) {
    arg0K0 = (1 + i_1007X);
    arg0K1 = (h_1008X + (((*((unsigned char *) ((((char *) (-3 + x_706X))) + i_1007X))))));
    goto L46041;}
  else {
    SvalS = (((h_1008X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}}
 L46018: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L41387: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = stob_708X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_709X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L44200: {
  firstP_1009X = arg2K0;
  vector_1010X = s48_find_all(type_721X);
  if ((1 == vector_1010X)) {
    if (firstP_1009X) {
      s48_collect();
      arg2K0 = 0;
      goto L44200;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = (((type_721X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}}
  else {
    SvalS = vector_1010X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}}
 L48240: {
  firstP_1011X = arg2K0;
  type_1012X = arg0K1;
  vector_1013X = s48_find_all_records(type_1012X);
  if ((1 == vector_1013X)) {
    if (firstP_1011X) {
      Stemp0S = type_1012X;
      s48_collect();
      value_1014X = Stemp0S;
      Stemp0S = 1;
      arg2K0 = 0;
      arg0K1 = value_1014X;
      goto L48240;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = type_1012X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}}
  else {
    SvalS = vector_1013X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}}
 L26155: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L24619: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L48288: {
  SvalS = (((old_731X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L23707: {
  SstackS = ((SstackS) + 4);
  proposal_1015X = *((long *) (SstackS));
  x_1016X = Scurrent_threadS;
  addr_1017X = (((char *) (-3 + x_1016X))) + 12;
  S48_WRITE_BARRIER(x_1016X, addr_1017X, proposal_1015X);
  *((long *) addr_1017X) = proposal_1015X;
  SstackS = ((SstackS) + 4);
  pc_1018X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tem_1019X = *((long *) (SstackS));
  StemplateS = tem_1019X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1019X)))))))) + (((pc_1018X)>>2)));
  SstackS = ((SstackS) + 4);
  SvalS = (*((long *) (SstackS)));
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L46966: {
  x_1020X = s48_schedule_alarm_interrupt((((p_734X)>>2)));
  SvalS = (((x_1020X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L44950: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L44954;}
  else {
    arg2K0 = 1;
    goto L44954;}}
 L44955: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_735X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L29236: {
  rest_list_1021X = arg0K0;
  if ((25 == rest_list_1021X)) {
    proc_1022X = *((long *) ((SstackS) + (((nargs_737X)<<2))));
    name_1023X = *((long *) ((SstackS) + (-4 + (((nargs_737X)<<2)))));
    args_1024X = (SstackS) + 4;
    *((long *) (SstackS)) = (10 + (((nargs_737X)<<10)));
    SstackS = ((SstackS) + -4);
    if ((3 == (3 & name_1023X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_1023X))) + -4))))>>2))))) {
        if ((3 == (3 & proc_1022X))) {
          if ((18 == (31 & ((((*((long *) ((((char *) (-3 + proc_1022X))) + -4))))>>2))))) {
            if ((4 == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proc_1022X))) + -4))))>>8)))) {
              result_1025X = s48_external_call(proc_1022X, name_1023X, (-2 + nargs_737X), args_1024X);
              if ((Sexternal_exceptionPS)) {
                Sexternal_exceptionPS = 0;
                arg0K0 = (Sexternal_exception_nargsS);
                goto L21748;}
              else {
                SvalS = result_1025X;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23693;}}
            else {
              goto L29502;}}
          else {
            goto L29502;}}
        else {
          goto L29502;}}
      else {
        goto L29502;}}
    else {
      goto L29502;}}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + rest_list_1021X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_1021X))) + 4)));
    goto L29236;}}
 L44295: {
  if ((1 == (SvalS))) {
    v_1026X = Hlookup2670((Sexported_bindingsS), arg2_740X, 0);
    arg0K0 = v_1026X;
    goto L44334;}
  else {
    v_1027X = Hlookup2651((Simported_bindingsS), arg2_740X, 0);
    arg0K0 = v_1027X;
    goto L44334;}}
 L44300: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_740X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L31047: {
  if ((1 == (SvalS))) {
    arg0K0 = (Sexported_bindingsS);
    goto L31080;}
  else {
    arg0K0 = (Simported_bindingsS);
    goto L31080;}}
 L31052: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_742X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L45952: {
  option_1028X = arg0K0;
  seconds_1029X = arg0K1;
  mseconds_1030X = arg0K2;
  if ((536869 < seconds_1029X)) {
    push_exception_continuationB(6, 1);
    *((long *) (SstackS)) = (((option_1028X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_1029X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_1030X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L21748;}
  else {
    SvalS = (((((1000 * seconds_1029X) + mseconds_1030X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}}
 L48348: {
  s48_Scallback_return_stack_blockS = arg2_756X;
  return x_757X;}
 L38656: {
  val_1031X = arg0K0;
  SvalS = val_1031X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L38623: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_758X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L38358: {
  len_1032X = 1 + n_763X;
  addr_1033X = s48_allocate_small((4 + len_1032X));
  *((long *) addr_1033X) = (70 + (((len_1032X)<<8)));
  string_1034X = 3 + (((long) (addr_1033X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1034X))) + n_763X)) = 0;
  arg0K0 = arg2_762X;
  arg0K1 = (-1 + n_763X);
  goto L38335;}
 L38310: {
  if ((25 == arg2_762X)) {
    goto L38358;}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_762X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_763X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}}
 L40125: {
  list_1035X = arg0K0;
  slow_1036X = arg0K1;
  move_slowP_1037X = arg2K2;
  if ((25 == list_1035X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    if ((3 == (3 & list_1035X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_1035X))) + -4))))>>2))))) {
        head_1038X = *((long *) (((char *) (-3 + list_1035X))));
        if ((3 == (3 & head_1038X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_1038X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_1038X))))) == thing_764X)) {
              SvalS = head_1038X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23693;}
            else {
              list_1039X = *((long *) ((((char *) (-3 + list_1035X))) + 4));
              if ((list_1039X == slow_1036X)) {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = thing_764X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = list_765X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21748;}
              else {
                if (move_slowP_1037X) {
                  arg0K0 = list_1039X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_1036X))) + 4)));
                  arg2K2 = 0;
                  goto L40125;}
                else {
                  arg0K0 = list_1039X;
                  arg0K1 = slow_1036X;
                  arg2K2 = 1;
                  goto L40125;}}}}
          else {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = thing_764X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = list_765X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21748;}}
        else {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = thing_764X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = list_765X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L21748;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = thing_764X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_765X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = thing_764X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = list_765X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}}}
 L29578: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = arg3_767X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_766X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_768X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L29568: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg3_767X))) + (((index_768X)<<2)))));
  goto L29577;}
 L29577: {
  value_1040X = arg0K0;
  SvalS = value_1040X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L29598: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg3_767X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_766X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_768X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21748;}
 L29836: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = arg4_774X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_773X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_775X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_776X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L29826: {
  addr_1041X = (((char *) (-3 + arg4_774X))) + (((index_775X)<<2));
  S48_WRITE_BARRIER(arg4_774X, addr_1041X, value_776X);
  *((long *) addr_1041X) = value_776X;
  goto L29835;}
 L29835: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L29858: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg4_774X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_773X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_775X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_776X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21748;}
 L30115: {
  port_1042X = arg0K0;
  if ((3 == (3 & port_1042X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1042X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1042X))) + 4))))>>2))))) {
        goto L30185;}
      else {
        p_1043X = *((long *) ((((char *) (-3 + port_1042X))) + 24));
        p_1044X = *((long *) ((((char *) (-3 + port_1042X))) + 28));
        b_1045X = *((long *) ((((char *) (-3 + port_1042X))) + 20));
        i_1046X = ((p_1043X)>>2);
        x_1047X = *((long *) ((((char *) (-3 + port_1042X))) + 12));
        if ((5 == x_1047X)) {
          goto L30165;}
        else {
          if ((1 == b_1045X)) {
            goto L30165;}
          else {
            if ((i_1046X == (((p_1044X)>>2)))) {
              goto L30165;}
            else {
              val_1048X = 4 + (((i_1046X)<<2));
              addr_1049X = (((char *) (-3 + port_1042X))) + 24;
              S48_WRITE_BARRIER(port_1042X, addr_1049X, val_1048X);
              *((long *) addr_1049X) = val_1048X;
              SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1045X))) + i_1046X))))))<<8)));
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23693;}}}}}
    else {
      goto L30185;}}
  else {
    goto L30185;}}
 L30356: {
  port_1050X = arg0K0;
  if ((3 == (3 & port_1050X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1050X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1050X))) + 4))))>>2))))) {
        goto L30426;}
      else {
        p_1051X = *((long *) ((((char *) (-3 + port_1050X))) + 24));
        p_1052X = *((long *) ((((char *) (-3 + port_1050X))) + 28));
        b_1053X = *((long *) ((((char *) (-3 + port_1050X))) + 20));
        i_1054X = ((p_1051X)>>2);
        x_1055X = *((long *) ((((char *) (-3 + port_1050X))) + 12));
        if ((5 == x_1055X)) {
          goto L30406;}
        else {
          if ((1 == b_1053X)) {
            goto L30406;}
          else {
            if ((i_1054X == (((p_1052X)>>2)))) {
              goto L30406;}
            else {
              SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1053X))) + i_1054X))))))<<8)));
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23693;}}}}}
    else {
      goto L30426;}}
  else {
    goto L30426;}}
 L30590: {
  Kchar_1056X = arg0K0;
  port_1057X = arg0K1;
  if ((9 == (255 & Kchar_1056X))) {
    if ((3 == (3 & port_1057X))) {
      if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1057X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_1057X))) + 4))))>>2))))) {
          goto L30661;}
        else {
          p_1058X = *((long *) ((((char *) (-3 + port_1057X))) + 24));
          b_1059X = *((long *) ((((char *) (-3 + port_1057X))) + 20));
          i_1060X = ((p_1058X)>>2);
          x_1061X = *((long *) ((((char *) (-3 + port_1057X))) + 12));
          if ((5 == x_1061X)) {
            goto L30643;}
          else {
            if ((1 == b_1059X)) {
              goto L30643;}
            else {
              if ((i_1060X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_1059X))) + -4))))>>8)))) {
                goto L30643;}
              else {
                val_1062X = 4 + (((i_1060X)<<2));
                addr_1063X = (((char *) (-3 + port_1057X))) + 24;
                S48_WRITE_BARRIER(port_1057X, addr_1063X, val_1062X);
                *((long *) addr_1063X) = val_1062X;
                *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_1057X))) + 20)))))) + i_1060X)) = (((((Kchar_1056X)>>8))));
                SvalS = 13;
                Scode_pointerS = ((Scode_pointerS) + 2);
                arg1K0 = (Scode_pointerS);
                goto L23693;}}}}}
      else {
        goto L30661;}}
    else {
      goto L30661;}}
  else {
    goto L30661;}}
 L40378: {
  stuff_1064X = arg0K0;
  if ((3 == (3 & stuff_1064X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_1064X))) + -4))))>>2))))) {
      thing_1065X = *((long *) (((char *) (-3 + stuff_1064X))));
      if ((0 == (3 & thing_1065X))) {
        ps_write_integer((((thing_1065X)>>2)), out_787X);
        goto L40384;}
      else {
        if ((9 == (255 & thing_1065X))) {
          ps_write_string("#\\", out_787X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_1065X)>>8))), out_787X, ignoreXX) }
          goto L40384;}
        else {
          if ((3 == (3 & thing_1065X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thing_1065X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_1065X))) + -4))))>>8))))>>2)))) {
                type_1066X = *((long *) (((char *) (-3 + thing_1065X))));
                if ((3 == (3 & type_1066X))) {
                  if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_1066X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_1066X))) + -4))))>>8))))>>2)))) {
                      obj_1067X = *((long *) ((((char *) (-3 + type_1066X))) + 8));
                      if ((3 == (3 & obj_1067X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_1067X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_787X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_1065X)))))))) + 8))))))))))))), out_787X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_787X, ignoreXX) }
                          goto L40384;}
                        else {
                          goto L15241;}}
                      else {
                        goto L15241;}}
                    else {
                      goto L15241;}}
                  else {
                    goto L15241;}}
                else {
                  goto L15241;}}
              else {
                goto L15241;}}
            else {
              goto L15241;}}
          else {
            goto L15241;}}}}
    else {
      goto L40369;}}
  else {
    goto L40369;}}
 L21811: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_790X))) + (((opcode_789X)<<2)))));
  obj_1068X = SvalS;
  if ((3 == (3 & obj_1068X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1068X))) + -4))))>>2))))) {
      goto L21828;}
    else {
      goto L21898;}}
  else {
    goto L21898;}}
 L21884: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L21811;}
 L26389: {
  arg0K0 = (SenvS);
  arg0K1 = 0;
  goto L26348;}
 L26680: {
  i_1069X = arg0K0;
  offset_1070X = arg0K1;
  env_1071X = arg0K2;
  if ((i_1069X == total_count_411X)) {
    SvalS = new_env_414X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1070X));
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    back_1072X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1070X)));
    arg0K0 = env_1071X;
    arg0K1 = back_1072X;
    goto L26758;}}
 L26531: {
  i_1073X = arg0K0;
  offset_1074X = arg0K1;
  env_1075X = arg0K2;
  if ((i_1073X == total_count_418X)) {
    SvalS = new_env_421X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1074X));
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    back_1076X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1074X)));
    arg0K0 = env_1075X;
    arg0K1 = back_1076X;
    goto L26609;}}
 L21981: {
  stack_arg_count_1077X = arg0K0;
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (StemplateS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  x_1078X = Scurrent_threadS;
  addr_1079X = (((char *) (-3 + x_1078X))) + 12;
  S48_WRITE_BARRIER(x_1078X, addr_1079X, 1);
  *((long *) addr_1079X) = 1;
  tem_1080X = Sinterrupt_templateS;
  StemplateS = tem_1080X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1080X))))))));
  push_continuationB((Scode_pointerS), (5 + stack_arg_count_1077X));
  n_1081X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L22115;}
 L24354: {
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L21393: {
  protocol_1082X = arg0K0;
  stack_space_1083X = arg0K1;
  if ((69 == protocol_1082X)) {
    if ((total_arg_count_824X < 3)) {
      skip_1084X = *((unsigned char *) ((((char *) (-3 + code_823X))) + (2 + total_arg_count_824X)));
      if ((0 == skip_1084X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_820X;
        arg0K2 = list_args_821X;
        arg0K3 = list_arg_count_822X;
        goto L22250;}
      else {
        merged_arg0K0 = list_args_821X;
        merged_arg0K1 = list_arg_count_822X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = skip_1084X;
        arg0K1 = total_arg_count_824X;
        goto L21397;}}
    else {
      skip_1085X = *((unsigned char *) ((((char *) (-3 + code_823X))) + 5));
      if ((0 == skip_1085X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_820X;
        arg0K2 = list_args_821X;
        arg0K3 = list_arg_count_822X;
        goto L22250;}
      else {
        arg0K0 = skip_1085X;
        goto L21430;}}}
  else {
    if ((63 < protocol_1082X)) {
      if ((65 == protocol_1082X)) {
        wants_stack_args_1086X = ((((*((unsigned char *) ((((char *) (-3 + code_823X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_823X))) + 3)));
        if ((total_arg_count_824X < wants_stack_args_1086X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_820X;
          arg0K2 = list_args_821X;
          arg0K3 = list_arg_count_822X;
          goto L22250;}
        else {
          merged_arg0K0 = wants_stack_args_1086X;
          merged_arg0K1 = stack_arg_count_820X;
          merged_arg0K2 = list_args_821X;
          merged_arg0K3 = list_arg_count_822X;
          rest_list_setupAgc_return_tag = 0;
          goto rest_list_setupAgc;
         rest_list_setupAgc_return_0:
          arg0K0 = 4;
          arg0K1 = (1 + wants_stack_args_1086X);
          goto L21397;}}
      else {
        if ((68 == protocol_1082X)) {
          if ((total_arg_count_824X < (*((unsigned char *) ((((char *) (-3 + code_823X))) + 2))))) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_820X;
            arg0K2 = list_args_821X;
            arg0K3 = list_arg_count_822X;
            goto L22250;}
          else {
            arg0K0 = 3;
            goto L21430;}}
        else {
          if ((64 == protocol_1082X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_823X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_823X))) + 3)))) == total_arg_count_824X)) {
              if ((0 == list_arg_count_822X)) {
                arg0K0 = 4;
                arg0K1 = total_arg_count_824X;
                goto L21397;}
              else {
                merged_arg0K0 = list_args_821X;
                merged_arg0K1 = list_arg_count_822X;
                push_list_return_tag = 1;
                goto push_list;
               push_list_return_1:
                arg0K0 = 4;
                arg0K1 = total_arg_count_824X;
                goto L21397;}}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_820X;
              arg0K2 = list_args_821X;
              arg0K3 = list_arg_count_822X;
              goto L22250;}}
          else {
            if ((67 == protocol_1082X)) {
              length_1087X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_823X))) + -4))))>>8);
              index_1088X = -2 + length_1087X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_823X))) + (-3 + length_1087X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_823X))) + index_1088X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_823X))) + (1 + index_1088X)))));
              goto L21393;}
            else {
              ps_error("unknown protocol", 1, protocol_1082X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_820X;
              arg0K2 = list_args_821X;
              arg0K3 = list_arg_count_822X;
              goto L22250;}}}}}
    else {
      if ((protocol_1082X == total_arg_count_824X)) {
        if ((0 == list_arg_count_822X)) {
          arg0K0 = 2;
          arg0K1 = total_arg_count_824X;
          goto L21397;}
        else {
          merged_arg0K0 = list_args_821X;
          merged_arg0K1 = list_arg_count_822X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          arg0K0 = 2;
          arg0K1 = total_arg_count_824X;
          goto L21397;}}
      else {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_820X;
        arg0K2 = list_args_821X;
        arg0K3 = list_arg_count_822X;
        goto L22250;}}}}
 L24989: {
  pop_continuationB_return_tag = 4;
  goto pop_continuationB;
 pop_continuationB_return_4:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L22314: {
  cont_1089X = arg0K0;
  if ((1 == cont_1089X)) {
    if ((0 == (3 & (SvalS)))) {
      s48_Scallback_return_stack_blockS = 1;
      return ((((SvalS))>>2));}
    else {
      goto L22329;}}
  else {
    goto L22329;}}
 L25007: {
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  SvalS = (*((long *) ((((char *) (-3 + cont_849X))) + 16)));
  arg0K0 = 1;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L22879;}
 L47209: {
  v_1090X = arg0K0;
  SvalS = v_1090X;
  pop_continuationB_return_tag = 5;
  goto pop_continuationB;
 pop_continuationB_return_5:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L47157: {
  if ((1 == (stack_nargs_862X + list_arg_count_864X))) {
    if ((1 == stack_nargs_862X)) {
      SstackS = ((SstackS) + 4);
      arg0K0 = (*((long *) (SstackS)));
      goto L47250;}
    else {
      arg0K0 = (*((long *) (((char *) (-3 + list_args_863X)))));
      goto L47250;}}
  else {
    merged_arg0K0 = list_args_863X;
    merged_arg0K1 = stack_nargs_862X;
    pop_args_GlistSAgc_return_tag = 8;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_8:
    args_1091X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1091X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}}
 L47165: {
  SvalS = (*((long *) ((((char *) (-3 + cont_865X))) + 16)));
  arg0K0 = stack_nargs_862X;
  arg0K1 = list_args_863X;
  arg0K2 = list_arg_count_864X;
  goto L22879;}
 L25945: {
  loc_1092X = arg1K0;
  arg_1093X = arg1K1;
  if ((arg_top_875X < arg_1093X)) {
    *((long *) loc_1092X) = (*((long *) arg_1093X));
    arg1K0 = (loc_1092X + -4);
    arg1K1 = (arg_1093X + -4);
    goto L25945;}
  else {
    SstackS = loc_1092X;
    if ((count_873X < stack_nargs_862X)) {
      merged_arg0K0 = list_args_863X;
      merged_arg0K1 = (stack_nargs_862X - count_873X);
      pop_args_GlistSAgc_return_tag = 9;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_9:
      v_1094X = pop_args_GlistSAgc0_return_value;
      arg0K0 = v_1094X;
      goto L25924;}
    else {
      arg0K0 = stack_nargs_862X;
      arg0K1 = list_args_863X;
      goto L25906;}}}
 L25797: {
  count_1095X = arg0K0;
  bytes_used_1096X = arg0K1;
  stack_nargs_1097X = arg0K2;
  list_args_1098X = arg0K3;
  list_arg_count_1099X = arg0K4;
  if ((count_1095X == (stack_nargs_1097X + list_arg_count_1099X))) {
    arg_top_1100X = SstackS;
    pop_continuationB_return_tag = 6;
    goto pop_continuationB;
   pop_continuationB_return_6:
    arg1K0 = (SstackS);
    arg1K1 = (arg_top_1100X + (((stack_nargs_1097X)<<2)));
    goto L25829;}
  else {
    merged_arg0K0 = list_args_1098X;
    merged_arg0K1 = stack_nargs_1097X;
    pop_args_GlistSAgc_return_tag = 10;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_10:
    args_1101X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1101X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}}
 L24100: {
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L13505: {
  if ((3 == (3 & x_466X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_466X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L47716;}
    else {
      goto L13511;}}
  else {
    goto L13511;}}
 L39210: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L39351: {
  if ((3 == (3 & n_468X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      goto L39362;}
    else {
      goto L39363;}}
  else {
    goto L39363;}}
 L39550: {
  if ((3 == (3 & n_469X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_469X))) + -4))))>>2))))) {
      goto L39561;}
    else {
      goto L39562;}}
  else {
    goto L39562;}}
 L39749: {
  if ((3 == (3 & n_470X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_470X))) + -4))))>>2))))) {
      goto L39760;}
    else {
      goto L39761;}}
  else {
    goto L39761;}}
 L32545: {
  if ((0 == (3 & x_474X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17186;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_474X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17186;}}
 L32562: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_474X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_475X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L8060: {
  b_1102X = arg0K0;
  lo_a_1103X = 65535 & a_894X;
  lo_b_1104X = 65535 & b_1102X;
  hi_a_1105X = 65535 & (((a_894X)>>16));
  hi_b_1106X = 65535 & (((b_1102X)>>16));
  lo_c_1107X = SMALL_MULTIPLY(lo_a_1103X, lo_b_1104X);
  v_1108X = SMALL_MULTIPLY(lo_b_1104X, hi_a_1105X);
  v_1109X = SMALL_MULTIPLY(lo_a_1103X, hi_b_1106X);
  mid_c_1110X = v_1109X + v_1108X;
  c_1111X = lo_c_1107X + (((mid_c_1110X)<<16));
  if ((0 < hi_a_1105X)) {
    if ((0 < hi_b_1106X)) {
      arg0K0 = x_477X;
      arg0K1 = y_478X;
      goto L19104;}
    else {
      goto L8102;}}
  else {
    goto L8102;}}
 L41513: {
  arg0K0 = x_477X;
  arg0K1 = y_478X;
  goto L19104;}
 L41530: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_477X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_478X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L32834: {
  if ((0 == (3 & x_481X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17143;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_481X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17143;}}
 L32851: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_481X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_482X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L8318: {
  b_1112X = arg0K0;
  c_1113X = a_910X / b_1112X;
  x_1114X = 0 == (a_910X % b_1112X);
  if (x_1114X) {
    if ((a_486X < 0)) {
      if ((b_487X < 0)) {
        s48_make_availableAgc(16);
        if ((536870911 < c_1113X)) {
          goto L49111;}
        else {
          if ((c_1113X < -536870912)) {
            goto L49111;}
          else {
            arg0K0 = (((c_1113X)<<2));
            goto L49106;}}}
      else {
        goto L8364;}}
    else {
      if ((b_487X < 0)) {
        goto L8364;}
      else {
        s48_make_availableAgc(16);
        if ((536870911 < c_1113X)) {
          goto L49133;}
        else {
          if ((c_1113X < -536870912)) {
            goto L49133;}
          else {
            arg0K0 = (((c_1113X)<<2));
            goto L49128;}}}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_484X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_485X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}}
 L41766: {
  if ((0 == (3 & x_484X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16711;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_484X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16711;}}
 L41806: {
  result_1115X = arg0K0;
  if ((1 == result_1115X)) {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_484X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_485X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}
  else {
    SvalS = result_1115X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}}
 L41813: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_484X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_485X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L33119: {
  if ((0 == (3 & (x_488X | y_489X)))) {
    if ((x_488X == y_489X)) {
      arg0K0 = 5;
      goto L33123;}
    else {
      arg0K0 = 1;
      goto L33123;}}
  else {
    if ((3 == (3 & x_488X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_488X))) + -4))))>>2))))) {
        if ((3 == (3 & y_489X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_489X))) + -4))))>>2))))) {
            b_1116X = s48_bignum_equal_p((((char *) (-3 + x_488X))), (((char *) (-3 + y_489X))));
            if (b_1116X) {
              arg0K0 = 5;
              goto L33123;}
            else {
              arg0K0 = 1;
              goto L33123;}}
          else {
            arg0K0 = 1;
            goto L33123;}}
        else {
          arg0K0 = 1;
          goto L33123;}}
      else {
        arg0K0 = 1;
        goto L33123;}}
    else {
      arg0K0 = 1;
      goto L33123;}}}
 L33139: {
  val_1117X = arg0K0;
  SvalS = val_1117X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L33140: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_488X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_489X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L33379: {
  if ((0 == (3 & x_490X))) {
    if ((0 == (3 & y_491X))) {
      if ((x_490X < y_491X)) {
        arg0K0 = 5;
        goto L33383;}
      else {
        arg0K0 = 1;
        goto L33383;}}
    else {
      v_1118X = s48_bignum_test((((char *) (-3 + y_491X))));
      if ((1 == v_1118X)) {
        arg0K0 = 5;
        goto L33383;}
      else {
        arg0K0 = 1;
        goto L33383;}}}
  else {
    if ((0 == (3 & y_491X))) {
      v_1119X = s48_bignum_test((((char *) (-3 + x_490X))));
      if ((1 == v_1119X)) {
        arg0K0 = 1;
        goto L33383;}
      else {
        arg0K0 = 5;
        goto L33383;}}
    else {
      v_1120X = s48_bignum_compare((((char *) (-3 + x_490X))), (((char *) (-3 + y_491X))));
      if ((-1 == v_1120X)) {
        arg0K0 = 5;
        goto L33383;}
      else {
        arg0K0 = 1;
        goto L33383;}}}}
 L33399: {
  val_1121X = arg0K0;
  SvalS = val_1121X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L33400: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_490X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_491X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L33702: {
  if ((0 == (3 & y_493X))) {
    if ((0 == (3 & x_492X))) {
      if ((y_493X < x_492X)) {
        arg0K0 = 5;
        goto L33706;}
      else {
        arg0K0 = 1;
        goto L33706;}}
    else {
      v_1122X = s48_bignum_test((((char *) (-3 + x_492X))));
      if ((1 == v_1122X)) {
        arg0K0 = 5;
        goto L33706;}
      else {
        arg0K0 = 1;
        goto L33706;}}}
  else {
    if ((0 == (3 & x_492X))) {
      v_1123X = s48_bignum_test((((char *) (-3 + y_493X))));
      if ((1 == v_1123X)) {
        arg0K0 = 1;
        goto L33706;}
      else {
        arg0K0 = 5;
        goto L33706;}}
    else {
      v_1124X = s48_bignum_compare((((char *) (-3 + y_493X))), (((char *) (-3 + x_492X))));
      if ((-1 == v_1124X)) {
        arg0K0 = 5;
        goto L33706;}
      else {
        arg0K0 = 1;
        goto L33706;}}}}
 L33722: {
  val_1125X = arg0K0;
  SvalS = val_1125X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L33723: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_492X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_493X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L34025: {
  if ((0 == (3 & y_495X))) {
    if ((0 == (3 & x_494X))) {
      if ((y_495X < x_494X)) {
        arg0K0 = 1;
        goto L34029;}
      else {
        arg0K0 = 5;
        goto L34029;}}
    else {
      v_1126X = s48_bignum_test((((char *) (-3 + x_494X))));
      if ((1 == v_1126X)) {
        arg0K0 = 1;
        goto L34029;}
      else {
        arg0K0 = 5;
        goto L34029;}}}
  else {
    if ((0 == (3 & x_494X))) {
      v_1127X = s48_bignum_test((((char *) (-3 + y_495X))));
      if ((1 == v_1127X)) {
        arg0K0 = 5;
        goto L34029;}
      else {
        arg0K0 = 1;
        goto L34029;}}
    else {
      v_1128X = s48_bignum_compare((((char *) (-3 + y_495X))), (((char *) (-3 + x_494X))));
      if ((-1 == v_1128X)) {
        arg0K0 = 1;
        goto L34029;}
      else {
        arg0K0 = 5;
        goto L34029;}}}}
 L34045: {
  val_1129X = arg0K0;
  SvalS = val_1129X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L34046: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_494X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_495X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L34319: {
  if ((0 == (3 & x_496X))) {
    if ((0 == (3 & y_497X))) {
      if ((x_496X < y_497X)) {
        arg0K0 = 1;
        goto L34323;}
      else {
        arg0K0 = 5;
        goto L34323;}}
    else {
      v_1130X = s48_bignum_test((((char *) (-3 + y_497X))));
      if ((1 == v_1130X)) {
        arg0K0 = 1;
        goto L34323;}
      else {
        arg0K0 = 5;
        goto L34323;}}}
  else {
    if ((0 == (3 & y_497X))) {
      v_1131X = s48_bignum_test((((char *) (-3 + x_496X))));
      if ((1 == v_1131X)) {
        arg0K0 = 5;
        goto L34323;}
      else {
        arg0K0 = 1;
        goto L34323;}}
    else {
      v_1132X = s48_bignum_compare((((char *) (-3 + x_496X))), (((char *) (-3 + y_497X))));
      if ((-1 == v_1132X)) {
        arg0K0 = 1;
        goto L34323;}
      else {
        arg0K0 = 5;
        goto L34323;}}}}
 L34339: {
  val_1133X = arg0K0;
  SvalS = val_1133X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L34340: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_496X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_497X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L19043: {
  length0_1134X = arg0K0;
  extra0_1135X = arg0K1;
  if ((0 == (3 & y_926X))) {
    arg0K0 = 3;
    goto L19045;}
  else {
    arg0K0 = 0;
    goto L19045;}}
 L8963: {
  b_1136X = arg0K0;
  c_1137X = a_927X / b_1136X;
  if ((a_500X < 0)) {
    if ((b_501X < 0)) {
      goto L9009;}
    else {
      goto L9008;}}
  else {
    if ((b_501X < 0)) {
      goto L9008;}
    else {
      goto L9009;}}}
 L34619: {
  arg0K0 = x_498X;
  arg0K1 = y_499X;
  goto L19013;}
 L18952: {
  length0_1138X = arg0K0;
  extra0_1139X = arg0K1;
  if ((0 == (3 & y_929X))) {
    arg0K0 = 3;
    goto L18954;}
  else {
    arg0K0 = 0;
    goto L18954;}}
 L34829: {
  b_1140X = arg0K0;
  c_1141X = a_930X % b_1140X;
  if ((a_504X < 0)) {
    arg0K0 = (0 - c_1141X);
    goto L34833;}
  else {
    arg0K0 = c_1141X;
    goto L34833;}}
 L34795: {
  arg0K0 = x_502X;
  arg0K1 = y_503X;
  goto L18922;}
 L22527: {
  x_1142X = arg0K0;
  if ((536870911 < x_1142X)) {
    goto L22528;}
  else {
    if ((x_1142X < -536870912)) {
      goto L22528;}
    else {
      return (((x_1142X)<<2));}}}
 L18530: {
  length_1143X = arg0K0;
  extra_1144X = arg0K1;
  Stemp0S = x_932X;
  s48_make_availableAgc(((((length_1143X + extra_1144X))<<2)));
  value_1145X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1145X))) {
    v_1146X = (char *) s48_long_to_bignum((((value_1145X)>>2)));
    arg1K0 = v_1146X;
    goto L18522;}
  else {
    arg1K0 = (((char *) (-3 + value_1145X)));
    goto L18522;}}
 L18877: {
  length_1147X = arg0K0;
  extra_1148X = arg0K1;
  if ((length_1147X < 1)) {
    arg0K0 = 1;
    goto L18879;}
  else {
    arg0K0 = length_1147X;
    goto L18879;}}
 L35452: {
  x_1149X = arg0K0;
  count_1150X = arg0K1;
  if ((0 == x_1149X)) {
    SvalS = (((count_1150X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    arg0K0 = (((x_1149X)>>1));
    arg0K1 = (count_1150X + (1 & x_1149X));
    goto L35452;}}
 L18819: {
  length_1151X = arg0K0;
  extra_1152X = arg0K1;
  if ((length_1151X < 1)) {
    arg0K0 = 1;
    goto L18821;}
  else {
    arg0K0 = length_1151X;
    goto L18821;}}
 L35537: {
  if ((0 == (3 & x_531X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16925;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_531X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16925;}}
 L35700: {
  if ((0 == (3 & x_533X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16882;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_533X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16882;}}
 L35863: {
  if ((0 == (3 & x_535X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16839;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_535X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16839;}}
 L42206: {
  v_1153X = (char *) s48_long_to_bignum(x_939X);
  v_1154X = enter_bignum(v_1153X);
  arg0K0 = v_1154X;
  goto L42201;}
 L42201: {
  val_1155X = arg0K0;
  SvalS = val_1155X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L42228: {
  v_1156X = (char *) s48_long_to_bignum(result_941X);
  v_1157X = enter_bignum(v_1156X);
  arg0K0 = v_1157X;
  goto L42223;}
 L42223: {
  val_1158X = arg0K0;
  SvalS = val_1158X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L42099: {
  x_1159X = arg0K0;
  y_1160X = arg0K1;
  y_1161X = ((y_1160X)>>2);
  merged_arg0K0 = x_1159X;
  merged_arg0K1 = y_1161X;
  shift_space_return_tag = 1;
  goto shift_space;
 shift_space_return_1:
  needed_1162X = shift_space0_return_value;
  Stemp0S = x_1159X;
  s48_make_availableAgc((((needed_1162X)<<2)));
  value_1163X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1163X))) {
    v_1164X = (char *) s48_long_to_bignum((((value_1163X)>>2)));
    arg1K0 = v_1164X;
    goto L42173;}
  else {
    arg1K0 = (((char *) (-3 + value_1163X)));
    goto L42173;}}
 L42250: {
  v_1165X = (char *) s48_long_to_bignum(result_941X);
  v_1166X = enter_bignum(v_1165X);
  arg0K0 = v_1166X;
  goto L42245;}
 L42245: {
  val_1167X = arg0K0;
  SvalS = val_1167X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L42281: {
  x_1168X = arg1K0;
  external_bignum_1169X = (char *) s48_bignum_arithmetic_shift(x_1168X, y_943X);
  v_1170X = s48_bignum_fits_in_word_p(external_bignum_1169X, 30, 1);
  if (v_1170X) {
    n_1171X = s48_bignum_to_long(external_bignum_1169X);
    arg0K0 = (((n_1171X)<<2));
    goto L42107;}
  else {
    v_1172X = enter_bignum(external_bignum_1169X);
    arg0K0 = v_1172X;
    goto L42107;}}
 L42108: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_537X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_538X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L27215: {
  i_1173X = arg0K0;
  rest_list_1174X = arg0K1;
  if ((25 == rest_list_1174X)) {
    SvalS = new_561X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    *((long *) ((((char *) (-3 + new_561X))) + (((i_1173X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_1174X)))));
    arg0K0 = (1 + i_1173X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_1174X))) + 4)));
    goto L27215;}}
 L27628: {
  i_1175X = arg0K0;
  if ((i_1175X < 0)) {
    SvalS = x_955X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    addr_1176X = (((char *) (-3 + x_955X))) + (((i_1175X)<<2));
    S48_WRITE_BARRIER(x_955X, addr_1176X, value_956X);
    *((long *) addr_1176X) = value_956X;
    arg0K0 = (-1 + i_1175X);
    goto L27628;}}
 L36113: {
  i_1177X = arg0K0;
  if ((i_1177X < 0)) {
    SvalS = vector_959X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_959X))) + i_1177X)) = init_594X;
    arg0K0 = (-1 + i_1177X);
    goto L36113;}}
 L36293: {
  i_1178X = arg0K0;
  if ((i_1178X < 0)) {
    SvalS = vector_960X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_960X))) + i_1178X)) = (init_607X);
    arg0K0 = (-1 + i_1178X);
    goto L36293;}}
 L20259: {
  bucket_1179X = arg0K0;
  arg0K0 = bucket_1179X;
  goto L20265;}
 L40477: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L36485: {
  index_1180X = arg0K0;
  channel_1181X = make_registered_channel(mode_631X, arg2_630X, index_1180X, 0, &reason_1182X);
  if ((1 == channel_1181X)) {
    if ((3 == (3 & arg2_630X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_630X))) + -4))))>>2))))) {
        if ((1 == mode_631X)) {
          goto L36600;}
        else {
          if ((3 == mode_631X)) {
            goto L36600;}
          else {
            v_1183X = ps_close_fd(index_1180X);
            arg0K0 = v_1183X;
            goto L36595;}}}
      else {
        push_exception_continuationB(reason_1182X, 1);
        *((long *) (SstackS)) = arg2_630X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_631X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21748;}}
    else {
      push_exception_continuationB(reason_1182X, 1);
      *((long *) (SstackS)) = arg2_630X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_631X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}}
  else {
    SvalS = channel_1181X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}}
 L36543: {
  v_1184X = ps_open_fd((((char *)(((char *) (-3 + arg2_630X))))), 1, &v_1185X);
  arg0K0 = v_1184X;
  arg0K1 = v_1185X;
  goto L36556;}
 L36556: {
  channel_1186X = arg0K0;
  status_1187X = arg0K1;
  if ((status_1187X == NO_ERRORS)) {
    arg0K0 = channel_1186X;
    goto L36485;}
  else {
    if ((status_1187X == ENOENT)) {
      push_exception_continuationB(10, 1);
      *((long *) (SstackS)) = arg2_630X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_631X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21748;}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg2_630X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_631X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1187X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_1188X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1188X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 3;
      goto L21748;}}}
 L43743: {
  waitP_1189X = arg2K0;
  start_1190X = ((arg3_637X)>>2);
  count_1191X = ((arg2_636X)>>2);
  v_1192X = 4 == (*((long *) (((char *) (-3 + arg5_639X)))));
  if (v_1192X) {
    if ((3 == (3 & arg4_638X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_638X))) + -4))))>>2))))) {
        goto L36927;}
      else {
        goto L37142;}}
    else {
      goto L37142;}}
  else {
    goto L36995;}}
 L37306: {
  y_1193X = arg0K0;
  if ((y_1193X < (start_644X + count_645X))) {
    goto L37358;}
  else {
    got_1194X = ps_write_fd(((((*((long *) ((((char *) (-3 + arg4_643X))) + 8))))>>2)), ((((char *) (-3 + arg3_642X))) + start_644X), count_645X, &pendingP_1195X, &status_1196X);
    if ((status_1196X == NO_ERRORS)) {
      if (pendingP_1195X) {
        arg0K0 = 1;
        goto L37357;}
      else {
        arg0K0 = (((got_1194X)<<2));
        goto L37357;}}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg4_643X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg3_642X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_644X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_645X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1196X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 4;
      goto get_error_string;
     get_error_string_return_4:
      x_1197X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1197X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 5;
      goto L21748;}}}
 L37493: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_642X))) + -4))))>>8));
  goto L37306;}
 L10867: {
  val_1198X = *((long *) ((((char *) (-3 + ch_973X))) + 12));
  addr_1199X = (((char *) (-3 + prev_974X))) + 12;
  S48_WRITE_BARRIER(prev_974X, addr_1199X, val_1198X);
  *((long *) addr_1199X) = val_1198X;
  addr_1200X = (((char *) (-3 + ch_973X))) + 12;
  S48_WRITE_BARRIER(ch_973X, addr_1200X, 1);
  *((long *) addr_1200X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_973X))) + 16)));
  goto L44081;}
 L15805: {
  v_1201X = arg0K0;
  arg0K0 = (-1 + i_978X);
  arg0K1 = v_1201X;
  goto L15791;}
 L37628: {
  if ((1 == proposal_660X)) {
    goto L37640;}
  else {
    addr_1202X = ((char *) (-3 + proposal_660X));
    S48_WRITE_BARRIER(proposal_660X, addr_1202X, 5);
    *((long *) addr_1202X) = 5;
    goto L37640;}}
 L9484: {
  i_1203X = arg0K0;
  stob_1204X = *((long *) ((((char *) (-3 + log_987X))) + (((i_1203X)<<2))));
  if ((1 == stob_1204X)) {
    copies_1205X = *((long *) ((((char *) (-3 + proposal_661X))) + 12));
    arg0K0 = copies_1205X;
    goto L37974;}
  else {
    verify_1206X = *((long *) ((((char *) (-3 + log_987X))) + (12 + (((i_1203X)<<2)))));
    value_1207X = *((long *) ((((char *) (-3 + log_987X))) + (8 + (((i_1203X)<<2)))));
    if ((29 == verify_1206X)) {
      if ((3 == (3 & stob_1204X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1204X))) + -4)))))) {
          goto L9536;}
        else {
          goto L37874;}}
      else {
        goto L37874;}}
    else {
      if ((verify_1206X == ((((*((unsigned char *) ((((char *) (-3 + stob_1204X))) + ((((*((long *) ((((char *) (-3 + log_987X))) + (4 + (((i_1203X)<<2)))))))>>2))))))<<2)))) {
        if ((verify_1206X == value_1207X)) {
          goto L9536;}
        else {
          if ((3 == (3 & stob_1204X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1204X))) + -4)))))) {
              goto L9536;}
            else {
              goto L37874;}}
          else {
            goto L37874;}}}
      else {
        goto L37874;}}}}
 L9298: {
  arg0K0 = (4 + i_985X);
  goto L9246;}
 L37874: {
  RELEASE_PROPOSAL_LOCK();
  x_1208X = Scurrent_threadS;
  addr_1209X = (((char *) (-3 + x_1208X))) + 12;
  S48_WRITE_BARRIER(x_1208X, addr_1209X, 1);
  *((long *) addr_1209X) = 1;
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L10054: {
  vector_1210X = arg0K0;
  if ((1 == vector_1210X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1211X;
    goto L10017;}
  else {
    arg0K0 = vector_1210X;
    goto L10017;}}
 L14378: {
  arg0K0 = (4 + i_998X);
  goto L14356;}
 L14552: {
  arg0K0 = (4 + i_1001X);
  goto L14532;}
 L44954: {
  minutesP_1212X = arg2K0;
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_1213X = check_events0_return_value;
      if (temp_1213X) {
        goto L44980;}
      else {
        goto L44985;}}
    else {
      goto L44980;}}
  else {
    goto L44985;}}
 L29502: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proc_1022X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = name_1023X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L44334: {
  val_1214X = arg0K0;
  SvalS = val_1214X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L31080: {
  table_1215X = arg0K0;
  n_1216X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_742X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L19772;}
 L38335: {
  l_1217X = arg0K0;
  i_1218X = arg0K1;
  if ((i_1218X < 0)) {
    SvalS = string_1034X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1034X))) + i_1218X)) = ((((((*((long *) (((char *) (-3 + l_1217X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_1217X))) + 4)));
    arg0K1 = (-1 + i_1218X);
    goto L38335;}}
 L30185: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_1042X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L30165: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = port_1042X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L30426: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_1050X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L30406: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = port_1050X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21748;}
 L30661: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = Kchar_1056X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1057X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L30643: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = Kchar_1056X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1057X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L40384: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_1064X))) + 4)));
  goto L40378;}
 L15241: {
  if ((3 == (3 & thing_1065X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_1065X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_1065X)))));
      goto L15283;}
    else {
      goto L15249;}}
  else {
    goto L15249;}}
 L40369: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_787X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L21828: {
  StemplateS = (SvalS);
  Slosing_opcodeS = opcode_789X;
  arg0K0 = (2 + nargs_788X);
  arg0K1 = 25;
  arg0K2 = 0;
  goto L21378;}
 L21898: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L21828;}
 L26758: {
  env_1219X = arg0K0;
  i_1220X = arg0K1;
  if ((0 == i_1220X)) {
    count_1221X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_1070X)));
    arg0K0 = count_1221X;
    arg0K1 = i_1069X;
    arg0K2 = (2 + offset_1070X);
    goto L26697;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1219X)))));
    arg0K1 = (-1 + i_1220X);
    goto L26758;}}
 L26609: {
  env_1222X = arg0K0;
  i_1223X = arg0K1;
  if ((0 == i_1223X)) {
    index_1224X = 1 + offset_1074X;
    count_1225X = ((((*((unsigned char *) ((Scode_pointerS) + (1 + index_1224X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_1224X))));
    arg0K0 = count_1225X;
    arg0K1 = i_1073X;
    arg0K2 = (3 + offset_1074X);
    goto L26548;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1222X)))));
    arg0K1 = (-1 + i_1223X);
    goto L26609;}}
 L22115: {
  i_1226X = arg0K0;
  m_1227X = arg0K1;
  if ((0 == (n_1081X & m_1227X))) {
    arg0K0 = (1 + i_1226X);
    arg0K1 = (((m_1227X)<<1));
    goto L22115;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_1227X));
    if ((i_1226X == 0)) {
      *((long *) (SstackS)) = (Sinterrupted_templateS);
      SstackS = ((SstackS) + -4);
      Sinterrupted_templateS = 1;
      *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L22011;}
    else {
      if ((i_1226X == 2)) {
        *((long *) (SstackS)) = (Sfinalize_theseS);
        SstackS = ((SstackS) + -4);
        Sfinalize_theseS = 25;
        *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L22011;}
      else {
        if ((i_1226X == 3)) {
          channel_1228X = Spending_channels_headS;
          next_1229X = *((long *) ((((char *) (-3 + channel_1228X))) + 12));
          Spending_channels_headS = next_1229X;
          addr_1230X = (((char *) (-3 + channel_1228X))) + 12;
          S48_WRITE_BARRIER(channel_1228X, addr_1230X, 1);
          *((long *) addr_1230X) = 1;
          if ((1 == next_1229X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_1228X;
            goto L11028;}
          else {
            arg0K0 = channel_1228X;
            goto L11028;}}
        else {
          if ((i_1226X == 4)) {
            *((long *) (SstackS)) = (Sos_signal_typeS);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (Sos_signal_argumentS);
            SstackS = ((SstackS) + -4);
            Sos_signal_typeS = 1;
            Sos_signal_argumentS = 1;
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 3;
            goto L22011;}
          else {
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L22011;}}}}}}
 L21397: {
  skip_1231X = arg0K0;
  stack_arg_count_1232X = arg0K1;
  template_1233X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_1233X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_1233X)))))))) + skip_1231X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  if ((stack_space_1083X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L21304;}
  else {
    s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
    merged_arg0K0 = 0;
    copy_stack_into_heap_return_tag = 3;
    goto copy_stack_into_heap;
   copy_stack_into_heap_return_3:
    if ((stack_space_1083X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L21304;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L21304;}}}
 L21430: {
  skip_1234X = arg0K0;
  if ((total_arg_count_824X < 3)) {
    arg0K0 = total_arg_count_824X;
    goto L21438;}
  else {
    if ((2 < stack_arg_count_820X)) {
      arg0K0 = stack_arg_count_820X;
      goto L21438;}
    else {
      arg0K0 = 2;
      goto L21438;}}}
 L22329: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  push_exception_continuationB(5, 0);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_1089X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L47250: {
  v_1235X = arg0K0;
  SvalS = v_1235X;
  arg0K0 = cont_870X;
  goto L22314;}
 L25924: {
  x_1236X = arg0K0;
  *((long *) (SstackS)) = x_1236X;
  SstackS = ((SstackS) + -4);
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L25906: {
  stack_nargs_1237X = arg0K0;
  l_1238X = arg0K1;
  if ((count_873X == stack_nargs_1237X)) {
    arg0K0 = l_1238X;
    goto L25924;}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1238X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (1 + stack_nargs_1237X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1238X))) + 4)));
    goto L25906;}}
 L25829: {
  loc_1239X = arg1K0;
  arg_1240X = arg1K1;
  if ((arg_top_1100X < arg_1240X)) {
    *((long *) loc_1239X) = (*((long *) arg_1240X));
    arg1K0 = (loc_1239X + -4);
    arg1K1 = (arg_1240X + -4);
    goto L25829;}
  else {
    SstackS = loc_1239X;
    if ((0 == list_arg_count_1099X)) {
      goto L25816;}
    else {
      merged_arg0K0 = list_args_1098X;
      merged_arg0K1 = list_arg_count_1099X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      goto L25816;}}}
 L13511: {
  if ((3 == (3 & x_466X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + x_466X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L47716;}
    else {
      arg0K0 = 1;
      goto L47716;}}
  else {
    arg0K0 = 1;
    goto L47716;}}
 L39363: {
  if ((3 == (3 & n_468X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_468X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
    else {
      goto L39367;}}
  else {
    goto L39367;}}
 L39562: {
  if ((3 == (3 & n_469X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_469X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_469X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
    else {
      goto L39566;}}
  else {
    goto L39566;}}
 L39761: {
  if ((3 == (3 & n_470X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_470X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_470X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21748;}
    else {
      goto L39765;}}
  else {
    goto L39765;}}
 L17186: {
  length0_1241X = arg0K0;
  extra0_1242X = arg0K1;
  if ((0 == (3 & y_475X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17194;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_475X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17194;}}
 L19104: {
  x_1243X = arg0K0;
  y_1244X = arg0K1;
  if ((0 == (3 & x_1243X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17106;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1243X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17106;}}
 L8102: {
  if ((536870911 < lo_c_1107X)) {
    arg0K0 = x_477X;
    arg0K1 = y_478X;
    goto L19104;}
  else {
    if ((lo_c_1107X < 0)) {
      arg0K0 = x_477X;
      arg0K1 = y_478X;
      goto L19104;}
    else {
      if ((8192 < mid_c_1110X)) {
        arg0K0 = x_477X;
        arg0K1 = y_478X;
        goto L19104;}
      else {
        if ((a_479X < 0)) {
          if ((b_480X < 0)) {
            s48_make_availableAgc(16);
            if ((536870911 < c_1111X)) {
              goto L49008;}
            else {
              if ((c_1111X < -536870912)) {
                goto L49008;}
              else {
                arg0K0 = (((c_1111X)<<2));
                goto L49003;}}}
          else {
            goto L8130;}}
        else {
          if ((b_480X < 0)) {
            goto L8130;}
          else {
            s48_make_availableAgc(16);
            if ((536870911 < c_1111X)) {
              goto L49030;}
            else {
              if ((c_1111X < -536870912)) {
                goto L49030;}
              else {
                arg0K0 = (((c_1111X)<<2));
                goto L49025;}}}}}}}}
 L17143: {
  length0_1245X = arg0K0;
  extra0_1246X = arg0K1;
  if ((0 == (3 & y_482X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17151;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_482X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17151;}}
 L49111: {
  v_1247X = (char *) s48_long_to_bignum(c_1113X);
  v_1248X = enter_bignum(v_1247X);
  arg0K0 = v_1248X;
  goto L49106;}
 L49106: {
  val_1249X = arg0K0;
  SvalS = val_1249X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L8364: {
  x_1250X = 0 - c_1113X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1250X)) {
    goto L49089;}
  else {
    if ((x_1250X < -536870912)) {
      goto L49089;}
    else {
      arg0K0 = (((x_1250X)<<2));
      goto L49084;}}}
 L49133: {
  v_1251X = (char *) s48_long_to_bignum(c_1113X);
  v_1252X = enter_bignum(v_1251X);
  arg0K0 = v_1252X;
  goto L49128;}
 L49128: {
  val_1253X = arg0K0;
  SvalS = val_1253X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L16711: {
  length0_1254X = arg0K0;
  extra0_1255X = arg0K1;
  if ((0 == (3 & y_485X))) {
    arg0K0 = 3;
    goto L16719;}
  else {
    arg0K0 = 0;
    goto L16719;}}
 L33123: {
  val_1256X = arg0K0;
  SvalS = val_1256X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L33383: {
  val_1257X = arg0K0;
  SvalS = val_1257X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L33706: {
  val_1258X = arg0K0;
  SvalS = val_1258X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L34029: {
  val_1259X = arg0K0;
  SvalS = val_1259X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L34323: {
  val_1260X = arg0K0;
  SvalS = val_1260X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L19045: {
  extra1_1261X = arg0K0;
  Stemp0S = x_925X;
  Stemp1S = y_926X;
  s48_make_availableAgc(((((((8 + (((length0_1134X)<<1))) + extra0_1135X) + extra1_1261X))<<2)));
  value_1262X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1262X))) {
    v_1263X = (char *) s48_long_to_bignum((((value_1262X)>>2)));
    arg1K0 = v_1263X;
    goto L19031;}
  else {
    arg1K0 = (((char *) (-3 + value_1262X)));
    goto L19031;}}
 L9009: {
  if ((536870911 < c_1137X)) {
    arg0K0 = x_498X;
    arg0K1 = y_499X;
    goto L19013;}
  else {
    SvalS = (((c_1137X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}}
 L9008: {
  SvalS = ((((0 - c_1137X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L18954: {
  extra1_1264X = arg0K0;
  Stemp0S = x_928X;
  Stemp1S = y_929X;
  s48_make_availableAgc(((((((8 + (((length0_1138X)<<1))) + extra0_1139X) + extra1_1264X))<<2)));
  value_1265X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1265X))) {
    v_1266X = (char *) s48_long_to_bignum((((value_1265X)>>2)));
    arg1K0 = v_1266X;
    goto L18940;}
  else {
    arg1K0 = (((char *) (-3 + value_1265X)));
    goto L18940;}}
 L34833: {
  n_1267X = arg0K0;
  SvalS = (((n_1267X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L22528: {
  v_1268X = (char *) s48_long_to_bignum(x_1142X);
  return enter_bignum(v_1268X);}
 L18522: {
  x_1269X = arg1K0;
  v_1270X = s48_bignum_test(x_1269X);
  if ((-1 == v_1270X)) {
    v_1271X = (char *) s48_bignum_negate(x_1269X);
    arg1K0 = v_1271X;
    goto L18524;}
  else {
    arg1K0 = x_1269X;
    goto L18524;}}
 L18879: {
  v_1272X = arg0K0;
  Stemp0S = x_528X;
  s48_make_availableAgc((((((3 + v_1272X) + extra_1148X))<<2)));
  value_1273X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1273X))) {
    v_1274X = (char *) s48_long_to_bignum((((value_1273X)>>2)));
    arg1K0 = v_1274X;
    goto L18869;}
  else {
    arg1K0 = (((char *) (-3 + value_1273X)));
    goto L18869;}}
 L18821: {
  v_1275X = arg0K0;
  Stemp0S = x_529X;
  s48_make_availableAgc((((((3 + v_1275X) + extra_1152X))<<2)));
  value_1276X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1276X))) {
    v_1277X = (char *) s48_long_to_bignum((((value_1276X)>>2)));
    arg1K0 = v_1277X;
    goto L18811;}
  else {
    arg1K0 = (((char *) (-3 + value_1276X)));
    goto L18811;}}
 L16925: {
  length0_1278X = arg0K0;
  extra0_1279X = arg0K1;
  if ((0 == (3 & y_532X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16933;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_532X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16933;}}
 L16882: {
  length0_1280X = arg0K0;
  extra0_1281X = arg0K1;
  if ((0 == (3 & y_534X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16890;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_534X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16890;}}
 L16839: {
  length0_1282X = arg0K0;
  extra0_1283X = arg0K1;
  if ((0 == (3 & y_536X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16847;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_536X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16847;}}
 L42173: {
  x_1284X = arg1K0;
  external_bignum_1285X = (char *) s48_bignum_arithmetic_shift(x_1284X, y_1161X);
  v_1286X = s48_bignum_fits_in_word_p(external_bignum_1285X, 30, 1);
  if (v_1286X) {
    n_1287X = s48_bignum_to_long(external_bignum_1285X);
    arg0K0 = (((n_1287X)<<2));
    goto L42101;}
  else {
    v_1288X = enter_bignum(external_bignum_1285X);
    arg0K0 = v_1288X;
    goto L42101;}}
 L42107: {
  val_1289X = arg0K0;
  SvalS = val_1289X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L20265: {
  foo_1290X = arg0K0;
  if ((1 == foo_1290X)) {
    if ((3 == (3 & bucket_1179X))) {
      arg0K0 = (-4 & bucket_1179X);
      goto L20270;}
    else {
      arg0K0 = bucket_1179X;
      goto L20270;}}
  else {
    s2_1291X = *((long *) (((char *) (-3 + foo_1290X))));
    len_1292X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_621X))) + -4))))>>8);
    if ((len_1292X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1291X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1291X))), (void *)(((char *) (-3 + string_621X))),len_1292X)))) {
        arg0K0 = foo_1290X;
        goto L32426;}
      else {
        goto L20285;}}
    else {
      goto L20285;}}}
 L36600: {
  v_1293X = ps_close_fd(index_1180X);
  arg0K0 = v_1293X;
  goto L36595;}
 L36595: {
  status_1294X = arg0K0;
  if ((status_1294X == NO_ERRORS)) {
    push_exception_continuationB(reason_1182X, 1);
    *((long *) (SstackS)) = arg2_630X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_631X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}
  else {
    channel_close_error(status_1294X, index_1180X, arg2_630X);
    push_exception_continuationB(reason_1182X, 1);
    *((long *) (SstackS)) = arg2_630X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_631X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21748;}}
 L36927: {
  if ((3 == (3 & arg4_638X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_638X))) + -4)))))) {
      if ((3 == (3 & arg4_638X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_638X))) + -4))))>>2))))) {
          arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_638X))) + -4))))>>8)));
          goto L36934;}
        else {
          goto L37168;}}
      else {
        goto L37168;}}
    else {
      goto L36995;}}
  else {
    goto L36995;}}
 L37142: {
  if ((3 == (3 & arg4_638X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg4_638X))) + -4))))>>2))))) {
      goto L36927;}
    else {
      goto L36995;}}
  else {
    goto L36995;}}
 L36995: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_639X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_638X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1190X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1191X)<<2));
  SstackS = ((SstackS) + -4);
  if (waitP_1189X) {
    arg0K0 = 5;
    goto L37011;}
  else {
    arg0K0 = 1;
    goto L37011;}}
 L37357: {
  val_1295X = arg0K0;
  SvalS = val_1295X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L37640: {
  x_1296X = Scurrent_threadS;
  addr_1297X = (((char *) (-3 + x_1296X))) + 12;
  S48_WRITE_BARRIER(x_1296X, addr_1297X, proposal_660X);
  *((long *) addr_1297X) = proposal_660X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L37974: {
  copies_1298X = arg0K0;
  if ((1 == copies_1298X)) {
    log_1299X = *((long *) ((((char *) (-3 + proposal_661X))) + 4));
    arg0K0 = 0;
    goto L8788;}
  else {
    thing_1300X = *((long *) ((((char *) (-3 + copies_1298X))) + 8));
    if ((3 == (3 & thing_1300X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + thing_1300X))) + -4)))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + copies_1298X))) + 20)));
        goto L37974;}
      else {
        goto L37874;}}
    else {
      goto L37874;}}}
 L9536: {
  arg0K0 = (4 + i_1203X);
  goto L9484;}
 L10017: {
  entry_1301X = arg0K0;
  proposal_1302X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  value_1303X = Stemp0S;
  Stemp0S = 1;
  addr_1304X = ((char *) (-3 + entry_1301X));
  S48_WRITE_BARRIER(entry_1301X, addr_1304X, value_1303X);
  *((long *) addr_1304X) = value_1303X;
  addr_1305X = (((char *) (-3 + entry_1301X))) + 4;
  S48_WRITE_BARRIER(entry_1301X, addr_1305X, from_index_993X);
  *((long *) addr_1305X) = from_index_993X;
  value_1306X = Stemp1S;
  Stemp1S = 1;
  addr_1307X = (((char *) (-3 + entry_1301X))) + 8;
  S48_WRITE_BARRIER(entry_1301X, addr_1307X, value_1306X);
  *((long *) addr_1307X) = value_1306X;
  addr_1308X = (((char *) (-3 + entry_1301X))) + 12;
  S48_WRITE_BARRIER(entry_1301X, addr_1308X, to_index_994X);
  *((long *) addr_1308X) = to_index_994X;
  addr_1309X = (((char *) (-3 + entry_1301X))) + 16;
  S48_WRITE_BARRIER(entry_1301X, addr_1309X, count_995X);
  *((long *) addr_1309X) = count_995X;
  value_1310X = *((long *) ((((char *) (-3 + proposal_1302X))) + 12));
  addr_1311X = (((char *) (-3 + entry_1301X))) + 20;
  S48_WRITE_BARRIER(entry_1301X, addr_1311X, value_1310X);
  *((long *) addr_1311X) = value_1310X;
  addr_1312X = (((char *) (-3 + proposal_1302X))) + 12;
  S48_WRITE_BARRIER(proposal_1302X, addr_1312X, entry_1301X);
  *((long *) addr_1312X) = entry_1301X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L44980: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L44985: {
  if ((0 == (Spending_interruptsS))) {
    s48_wait_for_event((((arg2_735X)>>2)), minutesP_1212X);
    goto L44980;}
  else {
    goto L44980;}}
 L19772: {
  i_1313X = arg0K0;
  h_1314X = arg0K1;
  if ((i_1313X < n_1216X)) {
    arg0K0 = (1 + i_1313X);
    arg0K1 = (h_1314X + (((*((unsigned char *) ((((char *) (-3 + arg2_742X))) + i_1313X))))));
    goto L19772;}
  else {
    index_1315X = 1023 & h_1314X;
    link_1316X = *((long *) ((((char *) (-3 + table_1215X))) + (((index_1315X)<<2))));
    if ((0 == (3 & link_1316X))) {
      arg0K0 = (3 + (-4 & link_1316X));
      goto L19727;}
    else {
      arg0K0 = link_1316X;
      goto L19727;}}}
 L15283: {
  v_1317X = arg3K0;
  ps_write_string(v_1317X, out_787X);
  goto L40384;}
 L15249: {
  if ((3 == (3 & thing_1065X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_1065X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_1065X))))))))));
      goto L15283;}
    else {
      goto L15257;}}
  else {
    goto L15257;}}
 L26697: {
  count_1318X = arg0K0;
  i_1319X = arg0K1;
  offset_1320X = arg0K2;
  if ((0 == count_1318X)) {
    arg0K0 = i_1319X;
    arg0K1 = offset_1320X;
    arg0K2 = env_1219X;
    goto L26680;}
  else {
    value_1321X = *((long *) ((((char *) (-3 + env_1219X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1320X)))))<<2))));
    addr_1322X = (((char *) (-3 + new_env_414X))) + (((i_1319X)<<2));
    S48_WRITE_BARRIER(new_env_414X, addr_1322X, value_1321X);
    *((long *) addr_1322X) = value_1321X;
    arg0K0 = (-1 + count_1318X);
    arg0K1 = (1 + i_1319X);
    arg0K2 = (1 + offset_1320X);
    goto L26697;}}
 L26548: {
  count_1323X = arg0K0;
  i_1324X = arg0K1;
  offset_1325X = arg0K2;
  if ((0 == count_1323X)) {
    arg0K0 = i_1324X;
    arg0K1 = offset_1325X;
    arg0K2 = env_1222X;
    goto L26531;}
  else {
    value_1326X = *((long *) ((((char *) (-3 + env_1222X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1325X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1325X))))))<<2))));
    addr_1327X = (((char *) (-3 + new_env_421X))) + (((i_1324X)<<2));
    S48_WRITE_BARRIER(new_env_421X, addr_1327X, value_1326X);
    *((long *) addr_1327X) = value_1326X;
    arg0K0 = (-1 + count_1323X);
    arg0K1 = (1 + i_1324X);
    arg0K2 = (2 + offset_1325X);
    goto L26548;}}
 L22011: {
  arg_count_1328X = arg0K0;
  handlers_1329X = SHARED_REF((Sinterrupt_handlersS));
  if ((3 == (3 & handlers_1329X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_1329X))) + -4))))>>2))))) {
      goto L22029;}
    else {
      goto L22128;}}
  else {
    goto L22128;}}
 L11028: {
  channel_1330X = arg0K0;
  x_1331X = 1 == (Spending_channels_headS);
  if (x_1331X) {
    goto L11042;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L11042;}
      else {
        goto L11042;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L11042;}}}
 L21304: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 3;
      goto check_events;
     check_events_return_3:
      v_1332X = check_events0_return_value;
      if (v_1332X) {
        arg0K0 = stack_arg_count_1232X;
        goto L21981;}
      else {
        goto L21308;}}
    else {
      arg0K0 = stack_arg_count_1232X;
      goto L21981;}}
  else {
    goto L21308;}}
 L21438: {
  final_stack_arg_count_1333X = arg0K0;
  if ((stack_arg_count_820X < final_stack_arg_count_1333X)) {
    arg0K0 = final_stack_arg_count_1333X;
    goto L21442;}
  else {
    arg0K0 = stack_arg_count_820X;
    goto L21442;}}
 L25816: {
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1096X));
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L39367: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L39566: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L39765: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L17194: {
  length1_1334X = arg0K0;
  extra1_1335X = arg0K1;
  if ((length0_1241X < length1_1334X)) {
    arg0K0 = length1_1334X;
    goto L17216;}
  else {
    arg0K0 = length0_1241X;
    goto L17216;}}
 L17106: {
  length0_1336X = arg0K0;
  extra0_1337X = arg0K1;
  if ((0 == (3 & y_1244X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17114;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_1244X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17114;}}
 L49008: {
  v_1338X = (char *) s48_long_to_bignum(c_1111X);
  v_1339X = enter_bignum(v_1338X);
  arg0K0 = v_1339X;
  goto L49003;}
 L49003: {
  val_1340X = arg0K0;
  SvalS = val_1340X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L8130: {
  x_1341X = 0 - c_1111X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1341X)) {
    goto L48986;}
  else {
    if ((x_1341X < -536870912)) {
      goto L48986;}
    else {
      arg0K0 = (((x_1341X)<<2));
      goto L48981;}}}
 L49030: {
  v_1342X = (char *) s48_long_to_bignum(c_1111X);
  v_1343X = enter_bignum(v_1342X);
  arg0K0 = v_1343X;
  goto L49025;}
 L49025: {
  val_1344X = arg0K0;
  SvalS = val_1344X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L17151: {
  length1_1345X = arg0K0;
  extra1_1346X = arg0K1;
  if ((length0_1245X < length1_1345X)) {
    arg0K0 = length1_1345X;
    goto L17173;}
  else {
    arg0K0 = length0_1245X;
    goto L17173;}}
 L49089: {
  v_1347X = (char *) s48_long_to_bignum(x_1250X);
  v_1348X = enter_bignum(v_1347X);
  arg0K0 = v_1348X;
  goto L49084;}
 L49084: {
  val_1349X = arg0K0;
  SvalS = val_1349X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L16719: {
  extra1_1350X = arg0K0;
  Stemp0S = x_484X;
  Stemp1S = y_485X;
  s48_make_availableAgc(((((((8 + (((length0_1254X)<<1))) + extra0_1255X) + extra1_1350X))<<2)));
  value_1351X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1351X))) {
    v_1352X = (char *) s48_long_to_bignum((((value_1351X)>>2)));
    arg1K0 = v_1352X;
    goto L18358;}
  else {
    arg1K0 = (((char *) (-3 + value_1351X)));
    goto L18358;}}
 L19031: {
  y_1353X = arg1K0;
  value_1354X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1354X))) {
    v_1355X = (char *) s48_long_to_bignum((((value_1354X)>>2)));
    arg1K0 = v_1355X;
    goto L19027;}
  else {
    arg1K0 = (((char *) (-3 + value_1354X)));
    goto L19027;}}
 L18940: {
  y_1356X = arg1K0;
  value_1357X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1357X))) {
    v_1358X = (char *) s48_long_to_bignum((((value_1357X)>>2)));
    arg1K0 = v_1358X;
    goto L18936;}
  else {
    arg1K0 = (((char *) (-3 + value_1357X)));
    goto L18936;}}
 L18524: {
  external_bignum_1359X = arg1K0;
  v_1360X = s48_bignum_fits_in_word_p(external_bignum_1359X, 30, 1);
  if (v_1360X) {
    n_1361X = s48_bignum_to_long(external_bignum_1359X);
    return (((n_1361X)<<2));}
  else {
    return enter_bignum(external_bignum_1359X);}}
 L18869: {
  v_1362X = arg1K0;
  external_bignum_1363X = (char *) s48_bignum_bitwise_not(v_1362X);
  v_1364X = s48_bignum_fits_in_word_p(external_bignum_1363X, 30, 1);
  if (v_1364X) {
    n_1365X = s48_bignum_to_long(external_bignum_1363X);
    arg0K0 = (((n_1365X)<<2));
    goto L35357;}
  else {
    val_1366X = enter_bignum(external_bignum_1363X);
    arg0K0 = val_1366X;
    goto L35357;}}
 L18811: {
  v_1367X = arg1K0;
  n_1368X = s48_bignum_bit_count(v_1367X);
  SvalS = (((n_1368X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L16933: {
  length1_1369X = arg0K0;
  extra1_1370X = arg0K1;
  if ((length0_1278X < length1_1369X)) {
    arg0K0 = length1_1369X;
    goto L16955;}
  else {
    arg0K0 = length0_1278X;
    goto L16955;}}
 L16890: {
  length1_1371X = arg0K0;
  extra1_1372X = arg0K1;
  if ((length0_1280X < length1_1371X)) {
    arg0K0 = length1_1371X;
    goto L16912;}
  else {
    arg0K0 = length0_1280X;
    goto L16912;}}
 L16847: {
  length1_1373X = arg0K0;
  extra1_1374X = arg0K1;
  if ((length0_1282X < length1_1373X)) {
    arg0K0 = length1_1373X;
    goto L16869;}
  else {
    arg0K0 = length0_1282X;
    goto L16869;}}
 L42101: {
  val_1375X = arg0K0;
  SvalS = val_1375X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L20270: {
  b_1376X = arg0K0;
  addr_1377X = s48_allocate_small(12);
  *((long *) addr_1377X) = 2054;
  x_1378X = 3 + (((long) (addr_1377X + 4)));
  *((long *) (((char *) (-3 + x_1378X)))) = string_621X;
  *((long *) ((((char *) (-3 + x_1378X))) + 4)) = b_1376X;
  if ((3 == (3 & x_1378X))) {
    arg0K0 = (-4 & x_1378X);
    goto L20276;}
  else {
    arg0K0 = x_1378X;
    goto L20276;}}
 L32426: {
  val_1379X = arg0K0;
  SvalS = val_1379X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L20285: {
  link_1380X = *((long *) ((((char *) (-3 + foo_1290X))) + 4));
  if ((0 == (3 & link_1380X))) {
    arg0K0 = (3 + (-4 & link_1380X));
    goto L20265;}
  else {
    arg0K0 = link_1380X;
    goto L20265;}}
 L36934: {
  y_1381X = arg0K0;
  if ((y_1381X < (start_1190X + count_1191X))) {
    goto L36995;}
  else {
    got_1382X = ps_read_fd(((((*((long *) ((((char *) (-3 + arg5_639X))) + 8))))>>2)), ((((char *) (-3 + arg4_638X))) + start_1190X), count_1191X, waitP_1189X, &eofP_1383X, &pendingP_1384X, &status_1385X);
    if ((status_1385X == NO_ERRORS)) {
      if (eofP_1383X) {
        arg0K0 = 21;
        goto L36994;}
      else {
        if (pendingP_1384X) {
          arg0K0 = 1;
          goto L36994;}
        else {
          arg0K0 = (((got_1382X)<<2));
          goto L36994;}}}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg5_639X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_638X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1190X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1191X)<<2));
      SstackS = ((SstackS) + -4);
      if (waitP_1189X) {
        arg0K0 = 5;
        goto L36977;}
      else {
        arg0K0 = 1;
        goto L36977;}}}}
 L37168: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_638X))) + -4))))>>8));
  goto L36934;}
 L37011: {
  x_1386X = arg0K0;
  *((long *) (SstackS)) = x_1386X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21748;}
 L8788: {
  i_1387X = arg0K0;
  stob_1388X = *((long *) ((((char *) (-3 + log_1299X))) + (((i_1387X)<<2))));
  if ((1 == stob_1388X)) {
    log_1389X = *((long *) ((((char *) (-3 + proposal_661X))) + 8));
    arg0K0 = 0;
    goto L11695;}
  else {
    verify_1390X = *((long *) ((((char *) (-3 + log_1299X))) + (12 + (((i_1387X)<<2)))));
    value_1391X = *((long *) ((((char *) (-3 + log_1299X))) + (8 + (((i_1387X)<<2)))));
    if ((verify_1390X == value_1391X)) {
      goto L8820;}
    else {
      addr_1392X = (((char *) (-3 + stob_1388X))) + (-4 & (*((long *) ((((char *) (-3 + log_1299X))) + (4 + (((i_1387X)<<2)))))));
      S48_WRITE_BARRIER(stob_1388X, addr_1392X, value_1391X);
      *((long *) addr_1392X) = value_1391X;
      goto L8820;}}}
 L19727: {
  bucket_1393X = arg0K0;
  arg0K0 = 1;
  arg0K1 = bucket_1393X;
  goto L19734;}
 L15257: {
  if ((1 == thing_1065X)) {
    goto L15260;}
  else {
    if ((5 == thing_1065X)) {
      goto L15260;}
    else {
      if ((25 == thing_1065X)) {
        arg3K0 = "()";
        goto L15283;}
      else {
        if ((3 == (3 & thing_1065X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_1065X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L15283;}
          else {
            goto L15273;}}
        else {
          goto L15273;}}}}}
 L22029: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Spending_interruptPS = 0;
    if ((s48_Spending_eventsPS)) {
      s48_Spending_interruptPS = 1;
      goto L22031;}
    else {
      goto L22031;}}
  else {
    s48_Spending_interruptPS = 1;
    goto L22031;}}
 L22128: {
  ps_error("interrupt handler is not a vector", 0);
  goto L22029;}
 L11042: {
  *((long *) (SstackS)) = channel_1330X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + channel_1330X))) + 16)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L22011;}
 L21308: {
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L21442: {
  v_1394X = arg0K0;
  merged_arg0K0 = v_1394X;
  merged_arg0K1 = stack_arg_count_820X;
  merged_arg0K2 = list_args_821X;
  merged_arg0K3 = list_arg_count_822X;
  rest_list_setupAgc_return_tag = 1;
  goto rest_list_setupAgc;
 rest_list_setupAgc_return_1:
  *((long *) (SstackS)) = (((final_stack_arg_count_1333X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_824X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_1234X;
  arg0K1 = (3 + final_stack_arg_count_1333X);
  goto L21397;}
 L17216: {
  v_1395X = arg0K0;
  Stemp0S = x_474X;
  Stemp1S = y_475X;
  s48_make_availableAgc(((((((3 + v_1395X) + extra0_1242X) + extra1_1335X))<<2)));
  value_1396X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1396X))) {
    v_1397X = (char *) s48_long_to_bignum((((value_1396X)>>2)));
    arg1K0 = v_1397X;
    goto L19270;}
  else {
    arg1K0 = (((char *) (-3 + value_1396X)));
    goto L19270;}}
 L17114: {
  length1_1398X = arg0K0;
  extra1_1399X = arg0K1;
  Stemp0S = x_1243X;
  Stemp1S = y_1244X;
  s48_make_availableAgc(((((((2 + (length0_1336X + length1_1398X)) + extra0_1337X) + extra1_1399X))<<2)));
  value_1400X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1400X))) {
    v_1401X = (char *) s48_long_to_bignum((((value_1400X)>>2)));
    arg1K0 = v_1401X;
    goto L19122;}
  else {
    arg1K0 = (((char *) (-3 + value_1400X)));
    goto L19122;}}
 L48986: {
  v_1402X = (char *) s48_long_to_bignum(x_1341X);
  v_1403X = enter_bignum(v_1402X);
  arg0K0 = v_1403X;
  goto L48981;}
 L48981: {
  val_1404X = arg0K0;
  SvalS = val_1404X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L17173: {
  v_1405X = arg0K0;
  Stemp0S = x_481X;
  Stemp1S = y_482X;
  s48_make_availableAgc(((((((3 + v_1405X) + extra0_1246X) + extra1_1346X))<<2)));
  value_1406X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1406X))) {
    v_1407X = (char *) s48_long_to_bignum((((value_1406X)>>2)));
    arg1K0 = v_1407X;
    goto L19196;}
  else {
    arg1K0 = (((char *) (-3 + value_1406X)));
    goto L19196;}}
 L18358: {
  y_1408X = arg1K0;
  value_1409X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1409X))) {
    v_1410X = (char *) s48_long_to_bignum((((value_1409X)>>2)));
    arg1K0 = v_1410X;
    goto L18354;}
  else {
    arg1K0 = (((char *) (-3 + value_1409X)));
    goto L18354;}}
 L19027: {
  x_1411X = arg1K0;
  external_bignum_1412X = (char *)s48_bignum_quotient(x_1411X, y_1353X);
  v_1413X = s48_bignum_fits_in_word_p(external_bignum_1412X, 30, 1);
  if (v_1413X) {
    n_1414X = s48_bignum_to_long(external_bignum_1412X);
    arg0K0 = (((n_1414X)<<2));
    goto L49791;}
  else {
    val_1415X = enter_bignum(external_bignum_1412X);
    arg0K0 = val_1415X;
    goto L49791;}}
 L18936: {
  x_1416X = arg1K0;
  external_bignum_1417X = (char *)s48_bignum_remainder(x_1416X, y_1356X);
  v_1418X = s48_bignum_fits_in_word_p(external_bignum_1417X, 30, 1);
  if (v_1418X) {
    n_1419X = s48_bignum_to_long(external_bignum_1417X);
    arg0K0 = (((n_1419X)<<2));
    goto L49791;}
  else {
    val_1420X = enter_bignum(external_bignum_1417X);
    arg0K0 = val_1420X;
    goto L49791;}}
 L35357: {
  val_1421X = arg0K0;
  SvalS = val_1421X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L16955: {
  v_1422X = arg0K0;
  Stemp0S = x_531X;
  Stemp1S = y_532X;
  s48_make_availableAgc(((((((3 + v_1422X) + extra0_1279X) + extra1_1370X))<<2)));
  value_1423X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1423X))) {
    v_1424X = (char *) s48_long_to_bignum((((value_1423X)>>2)));
    arg1K0 = v_1424X;
    goto L18744;}
  else {
    arg1K0 = (((char *) (-3 + value_1423X)));
    goto L18744;}}
 L16912: {
  v_1425X = arg0K0;
  Stemp0S = x_533X;
  Stemp1S = y_534X;
  s48_make_availableAgc(((((((3 + v_1425X) + extra0_1281X) + extra1_1372X))<<2)));
  value_1426X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1426X))) {
    v_1427X = (char *) s48_long_to_bignum((((value_1426X)>>2)));
    arg1K0 = v_1427X;
    goto L18670;}
  else {
    arg1K0 = (((char *) (-3 + value_1426X)));
    goto L18670;}}
 L16869: {
  v_1428X = arg0K0;
  Stemp0S = x_535X;
  Stemp1S = y_536X;
  s48_make_availableAgc(((((((3 + v_1428X) + extra0_1283X) + extra1_1374X))<<2)));
  value_1429X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1429X))) {
    v_1430X = (char *) s48_long_to_bignum((((value_1429X)>>2)));
    arg1K0 = v_1430X;
    goto L18596;}
  else {
    arg1K0 = (((char *) (-3 + value_1429X)));
    goto L18596;}}
 L20276: {
  value_1431X = arg0K0;
  addr_1432X = (((char *) (-3 + table_620X))) + (((index_963X)<<2));
  S48_WRITE_BARRIER(table_620X, addr_1432X, value_1431X);
  *((long *) addr_1432X) = value_1431X;
  arg0K0 = x_1378X;
  goto L32426;}
 L36994: {
  val_1433X = arg0K0;
  SvalS = val_1433X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L36977: {
  x_1434X = arg0K0;
  *((long *) (SstackS)) = x_1434X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_1385X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 5;
  goto get_error_string;
 get_error_string_return_5:
  x_1435X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_1435X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 6;
  goto L21748;}
 L11695: {
  i_1436X = arg0K0;
  stob_1437X = *((long *) ((((char *) (-3 + log_1389X))) + (((i_1436X)<<2))));
  if ((1 == stob_1437X)) {
    copies_1438X = *((long *) ((((char *) (-3 + proposal_661X))) + 12));
    arg0K0 = copies_1438X;
    goto L11610;}
  else {
    verify_1439X = *((long *) ((((char *) (-3 + log_1389X))) + (12 + (((i_1436X)<<2)))));
    value_1440X = *((long *) ((((char *) (-3 + log_1389X))) + (8 + (((i_1436X)<<2)))));
    if ((verify_1439X == value_1440X)) {
      goto L11727;}
    else {
      *((unsigned char *) ((((char *) (-3 + stob_1437X))) + ((((*((long *) ((((char *) (-3 + log_1389X))) + (4 + (((i_1436X)<<2)))))))>>2)))) = (((value_1440X)>>2));
      goto L11727;}}}
 L8820: {
  arg0K0 = (4 + i_1387X);
  goto L8788;}
 L19734: {
  previous_foo_1441X = arg0K0;
  foo_1442X = arg0K1;
  if ((1 == foo_1442X)) {
    goto L31082;}
  else {
    s2_1443X = *((long *) (((char *) (-3 + foo_1442X))));
    len_1444X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_742X))) + -4))))>>8);
    if ((len_1444X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1443X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1443X))), (void *)(((char *) (-3 + arg2_742X))),len_1444X)))) {
        if ((1 == previous_foo_1441X)) {
          value_1445X = *((long *) ((((char *) (-3 + foo_1442X))) + 12));
          addr_1446X = (((char *) (-3 + table_1215X))) + (((index_1315X)<<2));
          S48_WRITE_BARRIER(table_1215X, addr_1446X, value_1445X);
          *((long *) addr_1446X) = value_1445X;
          goto L31082;}
        else {
          val_1447X = *((long *) ((((char *) (-3 + foo_1442X))) + 12));
          addr_1448X = (((char *) (-3 + previous_foo_1441X))) + 12;
          S48_WRITE_BARRIER(previous_foo_1441X, addr_1448X, val_1447X);
          *((long *) addr_1448X) = val_1447X;
          goto L31082;}}
      else {
        goto L19802;}}
    else {
      goto L19802;}}}
 L15260: {
  if ((1 == thing_1065X)) {
    arg3K0 = "#f";
    goto L15283;}
  else {
    arg3K0 = "#t";
    goto L15283;}}
 L15273: {
  if ((3 == (3 & thing_1065X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_1065X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L15283;}
    else {
      goto L15277;}}
  else {
    goto L15277;}}
 L22031: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_1329X))) + (((i_1226X)<<2)))));
  obj_1449X = SvalS;
  if ((3 == (3 & obj_1449X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1449X))) + -4))))>>2))))) {
      goto L22048;}
    else {
      goto L22153;}}
  else {
    goto L22153;}}
 L19270: {
  y_1450X = arg1K0;
  value_1451X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1451X))) {
    v_1452X = (char *) s48_long_to_bignum((((value_1451X)>>2)));
    arg1K0 = v_1452X;
    goto L19266;}
  else {
    arg1K0 = (((char *) (-3 + value_1451X)));
    goto L19266;}}
 L19122: {
  y_1453X = arg1K0;
  value_1454X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1454X))) {
    v_1455X = (char *) s48_long_to_bignum((((value_1454X)>>2)));
    arg1K0 = v_1455X;
    goto L19118;}
  else {
    arg1K0 = (((char *) (-3 + value_1454X)));
    goto L19118;}}
 L19196: {
  y_1456X = arg1K0;
  value_1457X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1457X))) {
    v_1458X = (char *) s48_long_to_bignum((((value_1457X)>>2)));
    arg1K0 = v_1458X;
    goto L19192;}
  else {
    arg1K0 = (((char *) (-3 + value_1457X)));
    goto L19192;}}
 L18354: {
  x_1459X = arg1K0;
  div_by_zeroP_1460X = s48_bignum_divide(x_1459X, y_1408X, &quot_1461X, &rem_1462X);
  if (div_by_zeroP_1460X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = 0;
    goto L41771;}
  else {
    v_1463X = s48_bignum_fits_in_word_p(rem_1462X, 30, 1);
    if (v_1463X) {
      n_1464X = s48_bignum_to_long(rem_1462X);
      arg0K0 = (((n_1464X)<<2));
      goto L18381;}
    else {
      v_1465X = enter_bignum(rem_1462X);
      arg0K0 = v_1465X;
      goto L18381;}}}
 L49791: {
  val_1466X = arg0K0;
  SvalS = val_1466X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L18744: {
  y_1467X = arg1K0;
  value_1468X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1468X))) {
    v_1469X = (char *) s48_long_to_bignum((((value_1468X)>>2)));
    arg1K0 = v_1469X;
    goto L18740;}
  else {
    arg1K0 = (((char *) (-3 + value_1468X)));
    goto L18740;}}
 L18670: {
  y_1470X = arg1K0;
  value_1471X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1471X))) {
    v_1472X = (char *) s48_long_to_bignum((((value_1471X)>>2)));
    arg1K0 = v_1472X;
    goto L18666;}
  else {
    arg1K0 = (((char *) (-3 + value_1471X)));
    goto L18666;}}
 L18596: {
  y_1473X = arg1K0;
  value_1474X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1474X))) {
    v_1475X = (char *) s48_long_to_bignum((((value_1474X)>>2)));
    arg1K0 = v_1475X;
    goto L18592;}
  else {
    arg1K0 = (((char *) (-3 + value_1474X)));
    goto L18592;}}
 L11610: {
  copies_1476X = arg0K0;
  if ((1 == copies_1476X)) {
    value_1477X = Sempty_logS;
    addr_1478X = (((char *) (-3 + proposal_661X))) + 4;
    S48_WRITE_BARRIER(proposal_661X, addr_1478X, value_1477X);
    *((long *) addr_1478X) = value_1477X;
    value_1479X = Sempty_logS;
    addr_1480X = (((char *) (-3 + proposal_661X))) + 8;
    S48_WRITE_BARRIER(proposal_661X, addr_1480X, value_1479X);
    *((long *) addr_1480X) = value_1479X;
    addr_1481X = (((char *) (-3 + proposal_661X))) + 12;
    S48_WRITE_BARRIER(proposal_661X, addr_1481X, 1);
    *((long *) addr_1481X) = 1;
    RELEASE_PROPOSAL_LOCK();
    x_1482X = Scurrent_threadS;
    addr_1483X = (((char *) (-3 + x_1482X))) + 12;
    S48_WRITE_BARRIER(x_1482X, addr_1483X, 1);
    *((long *) addr_1483X) = 1;
    SvalS = 5;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23693;}
  else {
    stob_1484X = *((long *) (((char *) (-3 + copies_1476X))));
    v_1485X = (((*((long *) ((((char *) (-3 + copies_1476X))) + 4))))>>2);
    stob_1486X = *((long *) ((((char *) (-3 + copies_1476X))) + 8));
    v_1487X = (((*((long *) ((((char *) (-3 + copies_1476X))) + 12))))>>2);
    v_1488X = (((*((long *) ((((char *) (-3 + copies_1476X))) + 16))))>>2);
    memcpy((void *)((((char *) (-3 + stob_1486X))) + v_1487X), (void *)((((char *) (-3 + stob_1484X))) + v_1485X),v_1488X);
    arg0K0 = (*((long *) ((((char *) (-3 + copies_1476X))) + 20)));
    goto L11610;}}
 L11727: {
  arg0K0 = (4 + i_1436X);
  goto L11695;}
 L31082: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L19802: {
  link_1489X = *((long *) ((((char *) (-3 + foo_1442X))) + 12));
  if ((0 == (3 & link_1489X))) {
    arg0K0 = foo_1442X;
    arg0K1 = (3 + (-4 & link_1489X));
    goto L19734;}
  else {
    arg0K0 = foo_1442X;
    arg0K1 = link_1489X;
    goto L19734;}}
 L15277: {
  if ((3 == (3 & thing_1065X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_1065X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L15283;}
    else {
      arg3K0 = "???";
      goto L15283;}}
  else {
    arg3K0 = "???";
    goto L15283;}}
 L22048: {
  StemplateS = (SvalS);
  Slosing_opcodeS = (0 - i_1226X);
  arg0K0 = arg_count_1328X;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L21378;}
 L22153: {
  ps_error("interrupt handler is not a closure", 1, i_1226X);
  goto L22048;}
 L19266: {
  x_1490X = arg1K0;
  external_bignum_1491X = (char *)s48_bignum_add(x_1490X, y_1450X);
  v_1492X = s48_bignum_fits_in_word_p(external_bignum_1491X, 30, 1);
  if (v_1492X) {
    n_1493X = s48_bignum_to_long(external_bignum_1491X);
    arg0K0 = (((n_1493X)<<2));
    goto L32547;}
  else {
    val_1494X = enter_bignum(external_bignum_1491X);
    arg0K0 = val_1494X;
    goto L32547;}}
 L19118: {
  x_1495X = arg1K0;
  external_bignum_1496X = (char *)s48_bignum_multiply(x_1495X, y_1453X);
  v_1497X = s48_bignum_fits_in_word_p(external_bignum_1496X, 30, 1);
  if (v_1497X) {
    n_1498X = s48_bignum_to_long(external_bignum_1496X);
    arg0K0 = (((n_1498X)<<2));
    goto L49791;}
  else {
    val_1499X = enter_bignum(external_bignum_1496X);
    arg0K0 = val_1499X;
    goto L49791;}}
 L19192: {
  x_1500X = arg1K0;
  external_bignum_1501X = (char *)s48_bignum_subtract(x_1500X, y_1456X);
  v_1502X = s48_bignum_fits_in_word_p(external_bignum_1501X, 30, 1);
  if (v_1502X) {
    n_1503X = s48_bignum_to_long(external_bignum_1501X);
    arg0K0 = (((n_1503X)<<2));
    goto L32836;}
  else {
    val_1504X = enter_bignum(external_bignum_1501X);
    arg0K0 = val_1504X;
    goto L32836;}}
 L41771: {
  div_by_zeroP_1505X = arg2K0;
  quot_1506X = arg0K1;
  rem_1507X = arg0K2;
  if (div_by_zeroP_1505X) {
    goto L41792;}
  else {
    if ((0 == (3 & rem_1507X))) {
      if ((0 == rem_1507X)) {
        SvalS = quot_1506X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23693;}
      else {
        goto L41792;}}
    else {
      goto L41792;}}}
 L18381: {
  v_1508X = arg0K0;
  v_1509X = s48_bignum_fits_in_word_p(quot_1461X, 30, 1);
  if (v_1509X) {
    n_1510X = s48_bignum_to_long(quot_1461X);
    arg2K0 = 0;
    arg0K1 = (((n_1510X)<<2));
    arg0K2 = v_1508X;
    goto L41771;}
  else {
    v_1511X = enter_bignum(quot_1461X);
    arg2K0 = 0;
    arg0K1 = v_1511X;
    arg0K2 = v_1508X;
    goto L41771;}}
 L18740: {
  x_1512X = arg1K0;
  external_bignum_1513X = (char *) s48_bignum_bitwise_and(x_1512X, y_1467X);
  v_1514X = s48_bignum_fits_in_word_p(external_bignum_1513X, 30, 1);
  if (v_1514X) {
    n_1515X = s48_bignum_to_long(external_bignum_1513X);
    arg0K0 = (((n_1515X)<<2));
    goto L35539;}
  else {
    val_1516X = enter_bignum(external_bignum_1513X);
    arg0K0 = val_1516X;
    goto L35539;}}
 L18666: {
  x_1517X = arg1K0;
  external_bignum_1518X = (char *) s48_bignum_bitwise_ior(x_1517X, y_1470X);
  v_1519X = s48_bignum_fits_in_word_p(external_bignum_1518X, 30, 1);
  if (v_1519X) {
    n_1520X = s48_bignum_to_long(external_bignum_1518X);
    arg0K0 = (((n_1520X)<<2));
    goto L35702;}
  else {
    val_1521X = enter_bignum(external_bignum_1518X);
    arg0K0 = val_1521X;
    goto L35702;}}
 L18592: {
  x_1522X = arg1K0;
  external_bignum_1523X = (char *) s48_bignum_bitwise_xor(x_1522X, y_1473X);
  v_1524X = s48_bignum_fits_in_word_p(external_bignum_1523X, 30, 1);
  if (v_1524X) {
    n_1525X = s48_bignum_to_long(external_bignum_1523X);
    arg0K0 = (((n_1525X)<<2));
    goto L35865;}
  else {
    val_1526X = enter_bignum(external_bignum_1523X);
    arg0K0 = val_1526X;
    goto L35865;}}
 L32547: {
  val_1527X = arg0K0;
  SvalS = val_1527X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L32836: {
  val_1528X = arg0K0;
  SvalS = val_1528X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L41792: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_484X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_485X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21748;}
 L35539: {
  val_1529X = arg0K0;
  SvalS = val_1529X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L35702: {
  val_1530X = arg0K0;
  SvalS = val_1530X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 L35865: {
  val_1531X = arg0K0;
  SvalS = val_1531X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23693;}
 loseD0: {
  message_389X = merged_arg3K0;{
  why_1532X = (((*((long *) ((SstackS) + (4 + (((nargs_788X)<<2)))))))>>2);
  ps_write_string("Template UIDs: ", (stderr));
  current_template_1533X = StemplateS;
  out_1534X = stderr;
  if ((0 == (3 & (*((long *) ((((char *) (-3 + current_template_1533X))) + 4)))))) {
    if ((current_template_1533X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
      arg2K0 = 0;
      goto L13395;}
    else {
      ps_write_integer(((((*((long *) ((((char *) (-3 + current_template_1533X))) + 4))))>>2)), out_1534X);
      arg2K0 = 1;
      goto L13395;}}
  else {
    arg2K0 = 0;
    goto L13395;}}
 L13395: {
  not_firstP_1535X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_1535X;
  goto L13402;}
 L13402: {
  cont_1536X = arg0K0;
  not_firstP_1537X = arg2K1;
  if ((3 == (3 & cont_1536X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_1536X))) + -4))))>>2))))) {
      template_1538X = *((long *) ((((char *) (-3 + cont_1536X))) + 8));
      if ((0 == (3 & (*((long *) ((((char *) (-3 + template_1538X))) + 4)))))) {
        if ((template_1538X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
          arg2K0 = not_firstP_1537X;
          goto L13411;}
        else {
          if (not_firstP_1537X) {
            ps_write_string(" <- ", out_1534X);
            goto L13450;}
          else {
            goto L13450;}}}
      else {
        arg2K0 = not_firstP_1537X;
        goto L13411;}}
    else {
      goto L21767;}}
  else {
    goto L21767;}}
 L13411: {
  v_1539X = arg2K0;
  arg0K0 = (*((long *) (((char *) (-3 + cont_1536X)))));
  arg2K1 = v_1539X;
  goto L13402;}
 L13450: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_1538X))) + 4))))>>2)), out_1534X);
  arg2K0 = 1;
  goto L13411;}
 L21767: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_1532X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_788X)<<2))))))))))))) {
      ps_error(message_389X, 3, opcode_789X, why_1532X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_788X)<<2)))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L21791;}}
  else {
    goto L21791;}}
 L21791: {
  ps_error(message_389X, 2, opcode_789X, why_1532X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L49800;}
 L49800: {
  type_1540X = s48_get_next_event(&channel_1541X, &status_1542X);
  if ((type_1540X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L21197;}
    else {
      arg0K0 = 1;
      goto L21197;}}
  else {
    if ((type_1540X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L21197;}
    else {
      if ((type_1540X == IO_COMPLETION_EVENT)) {
        enqueue_channelB(channel_1541X, status_1542X);
        arg0K0 = 8;
        goto L21197;}
      else {
        if ((type_1540X == OS_SIGNAL_EVENT)) {
          arg0K0 = 16;
          goto L21197;}
        else {
          if ((type_1540X == NO_EVENT)) {
            arg0K0 = 0;
            goto L21197;}
          else {
            if ((type_1540X == ERROR_EVENT)) {
              ps_write_string("OS error while getting event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              ps_write_string((ps_error_string(status_1542X)), (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L21197;}
            else {
              ps_write_string("unknown type of event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L21197;}}}}}}}
 L21197: {
  v_1543X = arg0K0;
  Spending_interruptsS = (v_1543X | (Spending_interruptsS));
  if ((type_1540X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L21224;}
      else {
        goto L21224;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L21224;}}
  else {
    goto L49800;}}
 L21224: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      goto L49800;}
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

 rest_list_setupAgc: {
  wants_stack_args_385X = merged_arg0K0;
  stack_arg_count_386X = merged_arg0K1;
  list_args_387X = merged_arg0K2;
  list_arg_count_388X = merged_arg0K3;{
  if ((stack_arg_count_386X == wants_stack_args_385X)) {
    merged_arg0K0 = list_args_387X;
    merged_arg0K1 = list_arg_count_388X;
    copy_listSAgc_return_tag = 1;
    goto copy_listSAgc;
   copy_listSAgc_return_1:
    x_1544X = copy_listSAgc0_return_value;
    *((long *) (SstackS)) = x_1544X;
    SstackS = ((SstackS) + -4);
    goto rest_list_setupAgc_return;}
  else {
    if ((stack_arg_count_386X < wants_stack_args_385X)) {
      count_1545X = wants_stack_args_385X - stack_arg_count_386X;
      merged_arg0K0 = list_args_387X;
      merged_arg0K1 = count_1545X;
      push_list_return_tag = 4;
      goto push_list;
     push_list_return_4:
      v_1546X = push_list0_return_value;
      merged_arg0K0 = v_1546X;
      merged_arg0K1 = (list_arg_count_388X - count_1545X);
      copy_listSAgc_return_tag = 2;
      goto copy_listSAgc;
     copy_listSAgc_return_2:
      x_1547X = copy_listSAgc0_return_value;
      *((long *) (SstackS)) = x_1547X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setupAgc_return;}
    else {
      merged_arg0K0 = list_args_387X;
      merged_arg0K1 = list_arg_count_388X;
      copy_listSAgc_return_tag = 3;
      goto copy_listSAgc;
     copy_listSAgc_return_3:
      v_1548X = copy_listSAgc0_return_value;
      merged_arg0K0 = v_1548X;
      merged_arg0K1 = (stack_arg_count_386X - wants_stack_args_385X);
      pop_args_GlistSAgc_return_tag = 11;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_11:
      x_1549X = pop_args_GlistSAgc0_return_value;
      *((long *) (SstackS)) = x_1549X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setupAgc_return;}}}
 rest_list_setupAgc_return:
  switch (rest_list_setupAgc_return_tag) {
  case 0: goto rest_list_setupAgc_return_0;
  default: goto rest_list_setupAgc_return_1;
  }}

 push_list: {
  list_383X = merged_arg0K0;
  count_384X = merged_arg0K1;{
  *((long *) (SstackS)) = list_383X;
  SstackS = ((SstackS) + -4);
  if ((count_384X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L21041;}
  else {
    s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
    merged_arg0K0 = 0;
    copy_stack_into_heap_return_tag = 4;
    goto copy_stack_into_heap;
   copy_stack_into_heap_return_4:
    if ((count_384X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L21041;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L21041;}}}
 L21041: {
  SstackS = ((SstackS) + 4);
  list_1550X = *((long *) (SstackS));
  arg0K0 = count_384X;
  arg0K1 = list_1550X;
  goto L21050;}
 L21050: {
  i_1551X = arg0K0;
  l_1552X = arg0K1;
  if ((0 < i_1551X)) {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1552X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (-1 + i_1551X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1552X))) + 4)));
    goto L21050;}
  else {
    push_list0_return_value = l_1552X;
    goto push_list_return;}}
 push_list_return:
  switch (push_list_return_tag) {
  case 0: goto push_list_return_0;
  case 1: goto push_list_return_1;
  case 2: goto push_list_return_2;
  case 3: goto push_list_return_3;
  default: goto push_list_return_4;
  }}

 copy_stack_into_heap: {
  key_382X = merged_arg0K0;{
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L20615;}
 L20615: {
  p_1553X = arg1K0;
  i_1554X = arg0K1;
  if ((2 == (3 & (*((long *) p_1553X))))) {
    if ((26 == (*((long *) p_1553X)))) {
      arg0K0 = (-1 + i_1554X);
      goto L20595;}
    else {
      arg0K0 = i_1554X;
      goto L20595;}}
  else {
    arg1K0 = (p_1553X + 4);
    arg0K1 = (1 + i_1554X);
    goto L20615;}}
 L20595: {
  arg_count_1555X = arg0K0;
  top_1556X = SstackS;
  if ((1 == (ScontS))) {
    goto L20600;}
  else {
    merged_arg0K0 = key_382X;
    merged_arg0K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L20600;}}
 L20600: {
  SstackS = ((((char *) (-3 + (ScontS)))) + -8);
  arg1K0 = (SstackS);
  arg1K1 = (top_1556X + (((arg_count_1555X)<<2)));
  goto L20644;}
 L20644: {
  loc_1557X = arg1K0;
  arg_1558X = arg1K1;
  if ((top_1556X < arg_1558X)) {
    *((long *) loc_1557X) = (*((long *) arg_1558X));
    arg1K0 = (loc_1557X + -4);
    arg1K1 = (arg_1558X + -4);
    goto L20644;}
  else {
    SstackS = loc_1557X;
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
  key_380X = merged_arg0K0;
  reason_381X = merged_arg0K1;{
  p_1559X = SenvS;
  if ((3 == (3 & p_1559X))) {
    if ((p_1559X < (((long) (Sstack_beginS))))) {
      goto L19539;}
    else {
      if (((((long) (Sstack_endS))) < p_1559X)) {
        goto L19539;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_380X;
        merged_arg0K3 = reason_381X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_1560X = save_env_in_heap0_return_value;
        SenvS = v_1560X;
        goto L19539;}}}
  else {
    goto L19539;}}
 L19539: {
  end_1561X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L19492;}
 L19492: {
  cont_1562X = arg0K0;
  previous_1563X = arg0K1;
  if ((cont_1562X == (Sbottom_of_stackS))) {
    *((long *) (((char *) (-3 + previous_1563X)))) = end_1561X;
    ScontS = (Sbottom_of_stackS);
    really_preserve_continuation0_return_value = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
    goto really_preserve_continuation_return;}
  else {
    p_1564X = *((long *) ((((char *) (-3 + cont_1562X))) + 12));
    if ((3 == (3 & p_1564X))) {
      if ((p_1564X < (((long) (Sstack_beginS))))) {
        goto L19510;}
      else {
        if (((((long) (Sstack_endS))) < p_1564X)) {
          goto L19510;}
        else {
          merged_arg0K0 = (*((long *) ((((char *) (-3 + cont_1562X))) + 12)));
          merged_arg0K1 = cont_1562X;
          merged_arg0K2 = key_380X;
          merged_arg0K3 = reason_381X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L19510;}}}
    else {
      goto L19510;}}}
 L19510: {
  header_1565X = *((long *) ((((char *) (-3 + cont_1562X))) + -4));
  addr_1566X = s48_allocate_small((4 + ((long)(((unsigned long)header_1565X)>>8))));
  data_addr_1567X = addr_1566X + 4;
  *((long *) addr_1566X) = header_1565X;
  memcpy((void *)data_addr_1567X, (void *)(((char *) (-3 + cont_1562X))),((long)(((unsigned long)header_1565X)>>8)));
  new_1568X = 3 + (((long) data_addr_1567X));
  *((long *) (((char *) (-3 + previous_1563X)))) = new_1568X;
  arg0K0 = (*((long *) (((char *) (-3 + new_1568X)))));
  arg0K1 = new_1568X;
  goto L19492;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 save_env_in_heap: {
  env_378X = merged_arg0K0;
  cont_379X = merged_arg0K1;{
  header_1569X = *((long *) ((((char *) (-3 + env_378X))) + -4));
  addr_1570X = s48_allocate_small((4 + ((long)(((unsigned long)header_1569X)>>8))));
  data_addr_1571X = addr_1570X + 4;
  *((long *) addr_1570X) = header_1569X;
  memcpy((void *)data_addr_1571X, (void *)(((char *) (-3 + env_378X))),((long)(((unsigned long)header_1569X)>>8)));
  new_1572X = 3 + (((long) data_addr_1571X));
  addr_1573X = ((char *) (-3 + env_378X));
  S48_WRITE_BARRIER(env_378X, addr_1573X, 26);
  *((long *) addr_1573X) = 26;
  *((long *) ((((char *) (-3 + env_378X))) + -4)) = new_1572X;
  arg0K0 = new_1572X;
  goto L16452;}
 L16452: {
  env_1574X = arg0K0;
  p_1575X = *((long *) (((char *) (-3 + env_1574X))));
  if ((3 == (3 & p_1575X))) {
    if ((p_1575X < (((long) (Sstack_beginS))))) {
      goto L16663;}
    else {
      if (((((long) (Sstack_endS))) < p_1575X)) {
        goto L16663;}
      else {
        env_1576X = *((long *) (((char *) (-3 + env_1574X))));
        header_1577X = *((long *) ((((char *) (-3 + env_1576X))) + -4));
        addr_1578X = s48_allocate_small((4 + ((long)(((unsigned long)header_1577X)>>8))));
        data_addr_1579X = addr_1578X + 4;
        *((long *) addr_1578X) = header_1577X;
        memcpy((void *)data_addr_1579X, (void *)(((char *) (-3 + env_1576X))),((long)(((unsigned long)header_1577X)>>8)));
        new_1580X = 3 + (((long) data_addr_1579X));
        addr_1581X = ((char *) (-3 + env_1576X));
        S48_WRITE_BARRIER(env_1576X, addr_1581X, 26);
        *((long *) addr_1581X) = 26;
        *((long *) ((((char *) (-3 + env_1576X))) + -4)) = new_1580X;
        addr_1582X = ((char *) (-3 + env_1574X));
        S48_WRITE_BARRIER(env_1574X, addr_1582X, new_1580X);
        *((long *) addr_1582X) = new_1580X;
        arg0K0 = new_1580X;
        goto L16452;}}}
  else {
    goto L16663;}}
 L16663: {
  arg0K0 = cont_379X;
  goto L16675;}
 L16675: {
  cont_1583X = arg0K0;
  env_1584X = *((long *) ((((char *) (-3 + cont_1583X))) + 12));
  if ((3 == (3 & env_1584X))) {
    if ((3 == (3 & (*((long *) ((((char *) (-3 + env_1584X))) + -4)))))) {
      *((long *) ((((char *) (-3 + cont_1583X))) + 12)) = (*((long *) ((((char *) (-3 + env_1584X))) + -4)));
      arg0K0 = (*((long *) (((char *) (-3 + cont_1583X)))));
      goto L16675;}
    else {
      save_env_in_heap0_return_value = new_1572X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = new_1572X;
    goto save_env_in_heap_return;}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  default: goto save_env_in_heap_return_2;
  }}

 pop_args_GlistSAgc: {
  start_376X = merged_arg0K0;
  count_377X = merged_arg0K1;{
  Stemp0S = start_376X;
  s48_make_availableAgc(((((3 * count_377X))<<2)));
  value_1585X = Stemp0S;
  Stemp0S = 1;
  arg0K0 = value_1585X;
  arg0K1 = count_377X;
  goto L15994;}
 L15994: {
  args_1586X = arg0K0;
  count_1587X = arg0K1;
  if ((0 == count_1587X)) {
    pop_args_GlistSAgc0_return_value = args_1586X;
    goto pop_args_GlistSAgc_return;}
  else {
    SstackS = ((SstackS) + 4);
    a_1588X = *((long *) (SstackS));
    addr_1589X = s48_allocate_small(12);
    *((long *) addr_1589X) = 2050;
    x_1590X = 3 + (((long) (addr_1589X + 4)));
    *((long *) (((char *) (-3 + x_1590X)))) = a_1588X;
    *((long *) ((((char *) (-3 + x_1590X))) + 4)) = args_1586X;
    arg0K0 = x_1590X;
    arg0K1 = (-1 + count_1587X);
    goto L15994;}}
 pop_args_GlistSAgc_return:
  switch (pop_args_GlistSAgc_return_tag) {
  case 0: goto pop_args_GlistSAgc_return_0;
  case 1: goto pop_args_GlistSAgc_return_1;
  case 2: goto pop_args_GlistSAgc_return_2;
  case 3: goto pop_args_GlistSAgc_return_3;
  case 4: goto pop_args_GlistSAgc_return_4;
  case 5: goto pop_args_GlistSAgc_return_5;
  case 6: goto pop_args_GlistSAgc_return_6;
  case 7: goto pop_args_GlistSAgc_return_7;
  case 8: goto pop_args_GlistSAgc_return_8;
  case 9: goto pop_args_GlistSAgc_return_9;
  case 10: goto pop_args_GlistSAgc_return_10;
  default: goto pop_args_GlistSAgc_return_11;
  }}

 copy_listSAgc: {
  list_374X = merged_arg0K0;
  length_375X = merged_arg0K1;{
  if ((0 == length_375X)) {
    copy_listSAgc0_return_value = 25;
    goto copy_listSAgc_return;}
  else {
    Stemp0S = list_374X;
    s48_make_availableAgc(((((3 * length_375X))<<2)));
    value_1591X = Stemp0S;
    Stemp0S = 1;
    a_1592X = *((long *) (((char *) (-3 + value_1591X))));
    addr_1593X = s48_allocate_small(12);
    *((long *) addr_1593X) = 2050;
    x_1594X = 3 + (((long) (addr_1593X + 4)));
    *((long *) (((char *) (-3 + x_1594X)))) = a_1592X;
    *((long *) ((((char *) (-3 + x_1594X))) + 4)) = 25;
    arg0K0 = (*((long *) ((((char *) (-3 + value_1591X))) + 4)));
    arg0K1 = x_1594X;
    goto L15882;}}
 L15882: {
  l_1595X = arg0K0;
  last_1596X = arg0K1;
  if ((25 == l_1595X)) {
    copy_listSAgc0_return_value = x_1594X;
    goto copy_listSAgc_return;}
  else {
    a_1597X = *((long *) (((char *) (-3 + l_1595X))));
    addr_1598X = s48_allocate_small(12);
    *((long *) addr_1598X) = 2050;
    x_1599X = 3 + (((long) (addr_1598X + 4)));
    *((long *) (((char *) (-3 + x_1599X)))) = a_1597X;
    *((long *) ((((char *) (-3 + x_1599X))) + 4)) = 25;
    addr_1600X = (((char *) (-3 + last_1596X))) + 4;
    S48_WRITE_BARRIER(last_1596X, addr_1600X, x_1599X);
    *((long *) addr_1600X) = x_1599X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_1595X))) + 4)));
    arg0K1 = x_1599X;
    goto L15882;}}
 copy_listSAgc_return:
  switch (copy_listSAgc_return_tag) {
  case 0: goto copy_listSAgc_return_0;
  case 1: goto copy_listSAgc_return_1;
  case 2: goto copy_listSAgc_return_2;
  default: goto copy_listSAgc_return_3;
  }}

 shift_space: {
  x_372X = merged_arg0K0;
  n_373X = merged_arg0K1;{
  if ((0 == (3 & x_372X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16744;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_372X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16744;}}
 L16744: {
  x_size_1601X = arg0K0;
  extra_1602X = arg0K1;
  if ((n_373X < 0)) {
    if ((x_size_1601X < 1)) {
      arg0K0 = 1;
      goto L16789;}
    else {
      arg0K0 = x_size_1601X;
      goto L16789;}}
  else {
    n_1603X = n_373X / 30;
    arg0K0 = (5 + (x_size_1601X + n_1603X));
    goto L16764;}}
 L16789: {
  v_1604X = arg0K0;
  arg0K0 = (9 + ((((v_1604X)<<1)) + x_size_1601X));
  goto L16764;}
 L16764: {
  v_1605X = arg0K0;
  shift_space0_return_value = (extra_1602X + v_1605X);
  goto shift_space_return;}
 shift_space_return:
  switch (shift_space_return_tag) {
  case 0: goto shift_space_return_0;
  default: goto shift_space_return_1;
  }}

 okay_copy_argsP: {
  thing_369X = merged_arg0K0;
  index_370X = merged_arg0K1;
  count_371X = merged_arg0K2;{
  if ((index_370X < 0)) {
    okay_copy_argsP0_return_value = 0;
    goto okay_copy_argsP_return;}
  else {
    if ((3 == (3 & thing_369X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_369X))) + -4))))>>2))))) {
        if (((-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_369X))) + -4))))>>8))) < (index_370X + count_371X))) {
          okay_copy_argsP0_return_value = 0;
          goto okay_copy_argsP_return;}
        else {
          okay_copy_argsP0_return_value = 1;
          goto okay_copy_argsP_return;}}
      else {
        goto L15484;}}
    else {
      goto L15484;}}}
 L15484: {
  if ((3 == (3 & thing_369X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + thing_369X))) + -4))))>>2))))) {
      if ((((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_369X))) + -4))))>>8)) < (index_370X + count_371X))) {
        okay_copy_argsP0_return_value = 0;
        goto okay_copy_argsP_return;}
      else {
        okay_copy_argsP0_return_value = 1;
        goto okay_copy_argsP_return;}}
    else {
      okay_copy_argsP0_return_value = 0;
      goto okay_copy_argsP_return;}}
  else {
    okay_copy_argsP0_return_value = 0;
    goto okay_copy_argsP_return;}}
 okay_copy_argsP_return:
  switch (okay_copy_argsP_return_tag) {
  case 0: goto okay_copy_argsP_return_0;
  default: goto okay_copy_argsP_return_1;
  }}

 copy_continuation_from_heapB: {
  cont_367X = merged_arg0K0;
  stack_arg_count_368X = merged_arg0K1;{
  top_1606X = (((char *) (-3 + (Sbottom_of_stackS)))) + (-8 - (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_367X))) + -4))))>>8)))));
  new_cont_1607X = 3 + (((long) (top_1606X + 4)));
  if ((0 == stack_arg_count_368X)) {
    SstackS = (top_1606X + -4);
    goto L14674;}
  else {
    new_stack_1608X = top_1606X + (-4 - (((stack_arg_count_368X)<<2)));
    if ((new_stack_1608X < (SstackS))) {
      x_1609X = SstackS;
      memcpy((void *)(new_stack_1608X + 4), (void *)(x_1609X + 4),(((stack_arg_count_368X)<<2)));
      SstackS = new_stack_1608X;
      goto L14674;}
    else {
      goto L14674;}}}
 L14674: {
  ScontS = new_cont_1607X;
  v_1610X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_367X))) + -4))))>>8))))>>2);
  memcpy((void *)top_1606X, (void *)((((char *) (-3 + cont_367X))) + -4),(4 + (((v_1610X)<<2))));
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = (*((long *) (((char *) (-3 + new_cont_1607X)))));
  *((long *) (((char *) (-3 + new_cont_1607X)))) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_1607X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  case 1: goto copy_continuation_from_heapB_return_1;
  default: goto copy_continuation_from_heapB_return_2;
  }}

 proposal_d_write: {
  stob_364X = merged_arg0K0;
  index_365X = merged_arg0K1;
  value_366X = merged_arg0K2;{
  log_1611X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L14445;}
 L14445: {
  i_1612X = arg0K0;
  next_stob_1613X = *((long *) ((((char *) (-3 + log_1611X))) + (((i_1612X)<<2))));
  if ((1 == next_stob_1613X)) {
    add_log_entryAgc(1, i_1612X, stob_364X, index_365X, value_366X, 0);
    goto proposal_d_write_return;}
  else {
    if ((stob_364X == next_stob_1613X)) {
      if ((index_365X == (*((long *) ((((char *) (-3 + log_1611X))) + (4 + (((i_1612X)<<2)))))))) {
        addr_1614X = (((char *) (-3 + log_1611X))) + (8 + (((i_1612X)<<2)));
        S48_WRITE_BARRIER(log_1611X, addr_1614X, value_366X);
        *((long *) addr_1614X) = value_366X;
        goto proposal_d_write_return;}
      else {
        goto L14465;}}
    else {
      goto L14465;}}}
 L14465: {
  arg0K0 = (4 + i_1612X);
  goto L14445;}
 proposal_d_write_return:
  switch (proposal_d_write_return_tag) {
  case 0: goto proposal_d_write_return_0;
  case 1: goto proposal_d_write_return_1;
  default: goto proposal_d_write_return_2;
  }}

 proposal_d_read: {
  stob_362X = merged_arg0K0;
  index_363X = merged_arg0K1;{
  log_1615X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L14268;}
 L14268: {
  i_1616X = arg0K0;
  next_stob_1617X = *((long *) ((((char *) (-3 + log_1615X))) + (((i_1616X)<<2))));
  if ((1 == next_stob_1617X)) {
    v_1618X = add_log_entryAgc(1, i_1616X, stob_362X, index_363X, (*((long *) ((((char *) (-3 + stob_362X))) + (-4 & index_363X)))), 1);
    proposal_d_read0_return_value = v_1618X;
    goto proposal_d_read_return;}
  else {
    if ((stob_362X == next_stob_1617X)) {
      if ((index_363X == (*((long *) ((((char *) (-3 + log_1615X))) + (4 + (((i_1616X)<<2)))))))) {
        proposal_d_read0_return_value = (*((long *) ((((char *) (-3 + log_1615X))) + (8 + (((i_1616X)<<2))))));
        goto proposal_d_read_return;}
      else {
        goto L14290;}}
    else {
      goto L14290;}}}
 L14290: {
  arg0K0 = (4 + i_1616X);
  goto L14268;}
 proposal_d_read_return:
  switch (proposal_d_read_return_tag) {
  case 0: goto proposal_d_read_return_0;
  case 1: goto proposal_d_read_return_1;
  default: goto proposal_d_read_return_2;
  }}

 make_closure: {
  a_360X = merged_arg0K0;
  b_361X = merged_arg0K1;{
  addr_1619X = s48_allocate_small(12);
  *((long *) addr_1619X) = 2062;
  x_1620X = 3 + (((long) (addr_1619X + 4)));
  *((long *) (((char *) (-3 + x_1620X)))) = a_360X;
  *((long *) ((((char *) (-3 + x_1620X))) + 4)) = b_361X;
  if ((3 == (3 & x_1620X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_1620X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_1620X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_1620X))) + -4))));
      make_closure0_return_value = x_1620X;
      goto make_closure_return;}
    else {
      make_closure0_return_value = x_1620X;
      goto make_closure_return;}}
  else {
    make_closure0_return_value = x_1620X;
    goto make_closure_return;}}
 make_closure_return:
  switch (make_closure_return_tag) {
  case 0: goto make_closure_return_0;
  default: goto make_closure_return_1;
  }}

 get_current_port: {
  marker_359X = merged_arg0K0;{
  thread_1621X = Scurrent_threadS;
  if ((3 == (3 & thread_1621X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thread_1621X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_1621X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_1621X))) + 4)));
        goto L12852;}
      else {
        goto L12902;}}
    else {
      goto L12902;}}
  else {
    goto L12902;}}
 L12852: {
  env_1622X = arg0K0;
  if ((3 == (3 & env_1622X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_1622X))) + -4))))>>2))))) {
      obj_1623X = *((long *) (((char *) (-3 + env_1622X))));
      if ((3 == (3 & obj_1623X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_1623X))) + -4))))>>2))))) {
          if ((marker_359X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_1622X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_1622X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_1622X))) + 4)));
            goto L12852;}}
        else {
          goto L12924;}}
      else {
        goto L12924;}}
    else {
      goto L12924;}}
  else {
    goto L12924;}}
 L12902: {
  ps_error("current thread is not a record", 0);
  get_current_port0_return_value = v_1624X;
  goto get_current_port_return;}
 L12924: {
  if ((25 == env_1622X)) {
    if (((((marker_359X)>>2)) == 1)) {
      arg3K0 = "dynamic environment doesn't have current-output-port";
      goto L12878;}
    else {
      arg3K0 = "dynamic environment doesn't have current-input-port";
      goto L12878;}}
  else {
    ps_error("dynamic environment is not a proper list", 0);
    get_current_port0_return_value = v_1625X;
    goto get_current_port_return;}}
 L12878: {
  v_1626X = arg3K0;
  ps_error(v_1626X, 0);
  get_current_port0_return_value = v_1627X;
  goto get_current_port_return;}
 get_current_port_return:
  switch (get_current_port_return_tag) {
  case 0: goto get_current_port_return_0;
  case 1: goto get_current_port_return_1;
  default: goto get_current_port_return_2;
  }}

 okay_argument_list: {
  list_358X = merged_arg0K0;{
  arg0K0 = list_358X;
  arg0K1 = 0;
  arg0K2 = list_358X;
  arg2K3 = 0;
  goto L12719;}
 L12719: {
  fast_1628X = arg0K0;
  len_1629X = arg0K1;
  slow_1630X = arg0K2;
  move_slowP_1631X = arg2K3;
  if ((25 == fast_1628X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_1629X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_1628X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_1628X))) + -4))))>>2))))) {
        if (move_slowP_1631X) {
          if ((fast_1628X == slow_1630X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_1628X))) + 4)));
            arg0K1 = (1 + len_1629X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_1630X))) + 4)));
            arg2K3 = 0;
            goto L12719;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_1628X))) + 4)));
          arg0K1 = (1 + len_1629X);
          arg0K2 = slow_1630X;
          arg2K3 = 1;
          goto L12719;}}
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
  status_357X = merged_arg0K0;{
  string_1632X = ps_error_string(status_357X);
  x_1633X = strlen((char *) string_1632X);
  if ((x_1633X < 256)) {
    arg0K0 = x_1633X;
    goto L11908;}
  else {
    arg0K0 = 256;
    goto L11908;}}
 L11908: {
  len_1634X = arg0K0;
  len_1635X = 1 + len_1634X;
  addr_1636X = s48_allocate_small((4 + len_1635X));
  *((long *) addr_1636X) = (70 + (((len_1635X)<<8)));
  string_1637X = 3 + (((long) (addr_1636X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1637X))) + len_1634X)) = 0;
  arg0K0 = 0;
  goto L11918;}
 L11918: {
  i_1638X = arg0K0;
  if ((i_1638X == len_1634X)) {
    get_error_string0_return_value = string_1637X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1637X))) + i_1638X)) = ((*(string_1632X + i_1638X)));
    arg0K0 = (1 + i_1638X);
    goto L11918;}}
 get_error_string_return:
  switch (get_error_string_return_tag) {
  case 0: goto get_error_string_return_0;
  case 1: goto get_error_string_return_1;
  case 2: goto get_error_string_return_2;
  case 3: goto get_error_string_return_3;
  case 4: goto get_error_string_return_4;
  default: goto get_error_string_return_5;
  }}

 pop_continuationB: {
{ cont_1639X = ScontS;
  tem_1640X = *((long *) ((((char *) (-3 + cont_1639X))) + 8));
  pc_1641X = *((long *) ((((char *) (-3 + cont_1639X))) + 4));
  StemplateS = tem_1640X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1640X)))))))) + (((pc_1641X)>>2)));
  SenvS = (*((long *) ((((char *) (-3 + cont_1639X))) + 12)));
  ScontS = (*((long *) (((char *) (-3 + cont_1639X)))));
  SstackS = ((((char *) (-3 + cont_1639X))) + 12);
  goto pop_continuationB_return;}
 pop_continuationB_return:
  switch (pop_continuationB_return_tag) {
  case 0: goto pop_continuationB_return_0;
  case 1: goto pop_continuationB_return_1;
  case 2: goto pop_continuationB_return_2;
  case 3: goto pop_continuationB_return_3;
  case 4: goto pop_continuationB_return_4;
  case 5: goto pop_continuationB_return_5;
  default: goto pop_continuationB_return_6;
  }}

}
long s48_call_startup_procedure(char **startup_vector_1642X, long startup_vector_length_1643X)
{
  long arg0K0;
  long merged_arg0K1;
  char *merged_arg3K0;

  int enter_string_return_tag;
  long enter_string0_return_value;
  char *string_1644X;
  long i_1684X;
  long string_1683X;
  char * addr_1682X;
  long len_1681X;
  long len_1680X;
  long v_1679X;
  long x_1678X;
  long x_1677X;
  char * addr_1676X;
  long b_1675X;
  long channel_1674X;
  long x_1673X;
  char * addr_1672X;
  long b_1671X;
  long channel_1670X;
  long x_1669X;
  char * addr_1668X;
  long b_1667X;
  long channel_1666X;
  long i_1665X;
  long length_1664X;
  long *v_1663X;
  long v_1662X;
  long v_1661X;
  long y_1660X;
  long x_1659X;
  long v_1658X;
  long x_1657X;
  long y_1656X;
  char * addr_1655X;
  long value_1654X;
  long x_1653X;
  long y_1652X;
  long i_1651X;
  long vector_1650X;
  long v_1649X;
  long vector_1648X;
  char * addr_1647X;
  long len_1646X;
  long tem_1645X;
 {  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  SenvS = 13;
  tem_1645X = Sinterrupt_templateS;
  StemplateS = tem_1645X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1645X))))))));
  SvalS = 13;
  Scurrent_threadS = 25;
  SHARED_SETB((Ssession_dataS), 25);
  SHARED_SETB((Sexception_handlersS), 25);
  SHARED_SETB((Sinterrupt_handlersS), 25);
  Senabled_interruptsS = 0;
  SHARED_SETB((Sfinalizer_alistS), 25);
  Sfinalize_theseS = 25;
  Spending_interruptsS = 0;
  s48_Spending_interruptPS = 0;
  Sinterrupted_templateS = 1;
  len_1646X = ((startup_vector_length_1643X)<<2);
  addr_1647X = s48_allocate_tracedAgc((4 + len_1646X));
  if ((addr_1647X == NULL)) {
    arg0K0 = 1;
    goto L16396;}
  else {
    *((long *) addr_1647X) = (10 + (((len_1646X)<<8)));
    arg0K0 = (3 + (((long) (addr_1647X + 4))));
    goto L16396;}}
 L16396: {
  vector_1648X = arg0K0;
  if ((1 == vector_1648X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1649X;
    goto L16382;}
  else {
    arg0K0 = vector_1648X;
    goto L16382;}}
 L16382: {
  vector_1650X = arg0K0;
  arg0K0 = 0;
  goto L16419;}
 L16419: {
  i_1651X = arg0K0;
  if ((i_1651X == startup_vector_length_1643X)) {
    *((long *) (SstackS)) = vector_1650X;
    SstackS = ((SstackS) + -4);
    y_1652X = fileno((stderr));
    x_1653X = fileno((stdout));
    if ((x_1653X < y_1652X)) {
      arg0K0 = y_1652X;
      goto L18106;}
    else {
      arg0K0 = x_1653X;
      goto L18106;}}
  else {
    value_1654X = enter_stringAgc((*(startup_vector_1642X + i_1651X)));
    addr_1655X = (((char *) (-3 + vector_1650X))) + (((i_1651X)<<2));
    S48_WRITE_BARRIER(vector_1650X, addr_1655X, value_1654X);
    *((long *) addr_1655X) = value_1654X;
    arg0K0 = (1 + i_1651X);
    goto L16419;}}
 L18106: {
  y_1656X = arg0K0;
  x_1657X = fileno((stdin));
  if ((x_1657X < y_1656X)) {
    arg0K0 = y_1656X;
    goto L18108;}
  else {
    arg0K0 = x_1657X;
    goto L18108;}}
 L18108: {
  v_1658X = arg0K0;
  x_1659X = Snumber_of_channelsS;
  y_1660X = 1 + v_1658X;
  if ((x_1659X < y_1660X)) {
    arg0K0 = y_1660X;
    goto L18110;}
  else {
    arg0K0 = x_1659X;
    goto L18110;}}
 L18110: {
  v_1661X = arg0K0;
  Snumber_of_channelsS = v_1661X;
  v_1662X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L18132;}
  else {
    goto L18132;}}
 L18132: {
  v_1663X = Svm_channelsS;
  length_1664X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L18180;}
 L18180: {
  i_1665X = arg0K0;
  if ((i_1665X < length_1664X)) {
    *(v_1663X + i_1665X) = 1;
    arg0K0 = (1 + i_1665X);
    goto L18180;}
  else {
    s48_make_availableAgc(((((3 * (7 + ((((4 + (strlen((char *) "standard output"))))>>2)))))<<2)));
    channel_1666X = fileno((stdin));
    merged_arg3K0 = "standard input";
    merged_arg0K1 = 0;
    enter_string_return_tag = 0;
    goto enter_string;
   enter_string_return_0:
    b_1667X = enter_string0_return_value;
    addr_1668X = s48_allocate_small(24);
    *((long *) addr_1668X) = 5146;
    x_1669X = 3 + (((long) (addr_1668X + 4)));
    *((long *) (((char *) (-3 + x_1669X)))) = 4;
    *((long *) ((((char *) (-3 + x_1669X))) + 4)) = b_1667X;
    *((long *) ((((char *) (-3 + x_1669X))) + 8)) = (((channel_1666X)<<2));
    *((long *) ((((char *) (-3 + x_1669X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1669X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1666X) = x_1669X;
    channel_1670X = fileno((stderr));
    merged_arg3K0 = "standard error";
    merged_arg0K1 = 0;
    enter_string_return_tag = 1;
    goto enter_string;
   enter_string_return_1:
    b_1671X = enter_string0_return_value;
    addr_1672X = s48_allocate_small(24);
    *((long *) addr_1672X) = 5146;
    x_1673X = 3 + (((long) (addr_1672X + 4)));
    *((long *) (((char *) (-3 + x_1673X)))) = 8;
    *((long *) ((((char *) (-3 + x_1673X))) + 4)) = b_1671X;
    *((long *) ((((char *) (-3 + x_1673X))) + 8)) = (((channel_1670X)<<2));
    *((long *) ((((char *) (-3 + x_1673X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1673X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1670X) = x_1673X;
    channel_1674X = fileno((stdout));
    merged_arg3K0 = "standard output";
    merged_arg0K1 = 0;
    enter_string_return_tag = 2;
    goto enter_string;
   enter_string_return_2:
    b_1675X = enter_string0_return_value;
    addr_1676X = s48_allocate_small(24);
    *((long *) addr_1676X) = 5146;
    x_1677X = 3 + (((long) (addr_1676X + 4)));
    *((long *) (((char *) (-3 + x_1677X)))) = 8;
    *((long *) ((((char *) (-3 + x_1677X))) + 4)) = b_1675X;
    *((long *) ((((char *) (-3 + x_1677X))) + 8)) = (((channel_1674X)<<2));
    *((long *) ((((char *) (-3 + x_1677X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1677X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1674X) = x_1677X;
    *((long *) (SstackS)) = x_1669X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1677X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1673X;
    SstackS = ((SstackS) + -4);
    x_1678X = s48_resumer_records();
    *((long *) (SstackS)) = x_1678X;
    SstackS = ((SstackS) + -4);
    s48_initialization_completeB();
    v_1679X = s48_startup_procedure();
    return s48_restart(v_1679X, 5);}}
 enter_string: {
  string_1644X = merged_arg3K0;{
  len_1680X = strlen((char *) string_1644X);
  len_1681X = 1 + len_1680X;
  addr_1682X = s48_allocate_small((4 + len_1681X));
  *((long *) addr_1682X) = (70 + (((len_1681X)<<8)));
  string_1683X = 3 + (((long) (addr_1682X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1683X))) + len_1680X)) = 0;
  arg0K0 = 0;
  goto L14060;}
 L14060: {
  i_1684X = arg0K0;
  if ((i_1684X == len_1680X)) {
    enter_string0_return_value = string_1683X;
    goto enter_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1683X))) + i_1684X)) = ((*(string_1644X + i_1684X)));
    arg0K0 = (1 + i_1684X);
    goto L14060;}}
 enter_string_return:
  switch (enter_string_return_tag) {
  case 0: goto enter_string_return_0;
  case 1: goto enter_string_return_1;
  default: goto enter_string_return_2;
  }}

}void
s48_init(void)
{
Snumber_of_channelsS = 100;
Spending_channels_headS = 1;
Spending_channels_tailS = 1;
Stemp0S = 1;
Stemp1S = 1;
Sstack_warningPS = 1;
Slosing_opcodeS = 0;
Sos_signal_typeS = 1;
Sos_signal_argumentS = 1;
Sexternal_exceptionPS = 0;
Simported_bindingsS = 1;
Sexported_bindingsS = 1;
Sexternal_root_stackS = NULL;
Sexternal_root_stack_baseS = NULL;
Spermanent_external_rootsS = NULL;
Spost_gc_cleanupS = HtopD10192;
Sgc_root_procS = HtopD10203;
s48_Scallback_return_stack_blockS = 1;
s48_Spending_eventsPS = 0;
}
