#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm-prelude.h"

static char add_more_channels(long);
static void push_exception_continuationB(char *, long, long, long, long);
static void enqueue_channelB(long, long);
static long close_channelB(long);
static long add_log_entryAgc(long, long, long, long, long, char);
static long enter_bignum(char *);
static long enter_stringAgc(char*);
static long channel_close_error(long, long, long);
static long make_registered_channel(long, long, long, long, long*);
static long really_preserve_continuation(long);
static long Hlookup2690(long, long, long);
static long Hlookup2671(long, long, long);
static void HtopD10322(void);
static long current_code_vector(void);
static void HtopD10333(void);
void s48_set_native_protocolB(long);
void s48_set_extension_valueB(long);
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
void s48_note_event(void);
void s48_disable_interruptsB(void);
void s48_push_gc_rootsB(char *, long);
void s48_register_gc_rootB(char *);
void s48_stack_setB(long, long);
long s48_stack_ref(long);
void s48_push(long);
long s48_resetup_external_exception(long, long);
char s48_pop_gc_rootsB(void);
char * s48_shorten_bignum(char *, long);
long s48_allocate_bignum(long);
void s48_enable_interruptsB(void);
long s48_set_channel_os_index(long, long);
void s48_close_channel(long);
char s48_warn_about_undefined_imported_bindings(void);
void check_stack(void);
long s48_really_add_channel(long, long, long);
long s48_enter_integer(long);
void s48_copy_stack_into_heap(void);
long s48_get_imported_binding(char*);
void s48_define_exported_binding(char*, long);
void s48_initialize_vm(char *, long);
void s48_post_gc_cleanup(void);
void s48_setup_external_exception(long, long);
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
static char * Sreal_stack_limitS;
static char * Sbottom_of_stackS;
static long Sheap_continuationS;
static char Sstack_warningPS;
static long Soldspace_beginS;
static long Soldspace_endS;
static long Sexception_handlersS;
static long Sinterrupt_handlersS;
static long Slast_code_calledS;
static char * Slast_code_pointer_resumedS;
static long Scurrent_threadS;
static long Ssession_dataS;
static long Sfinalizer_alistS;
static long Sfinalize_theseS;
static long Senabled_interruptsS;
static long Spending_interruptsS;
static long Sinterrupted_templateS;
static long Sinterrupt_return_codeS;
static long Sexception_return_codeS;
static long Scall_with_values_return_codeS;
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
char * s48_Sstack_limitS;
char * ScontS;
char * Scode_pointerS;
long SvalS;
char s48_Spending_interruptPS;
long s48_Snative_protocolS;
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
    goto L2964;}
  else {
    arg0K0 = x_5X;
    goto L2964;}}
 L2964: {
  new_count_7X = arg0K0;
  new_vm_channels_8X = (long*)malloc(sizeof(long) * new_count_7X);
  if ((NULL == new_vm_channels_8X)) {
    return 0;}
  else {
    arg0K0 = 0;
    goto L2976;}}
 L2976: {
  i_9X = arg0K0;
  if ((i_9X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L3049;}
  else {
    *(new_vm_channels_8X + i_9X) = (*((Svm_channelsS) + i_9X));
    arg0K0 = (1 + i_9X);
    goto L2976;}}
 L3049: {
  i_10X = arg0K0;
  if ((i_10X == new_count_7X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_8X;
    Snumber_of_channelsS = new_count_7X;
    return 1;}
  else {
    *(new_vm_channels_8X + i_10X) = 1;
    arg0K0 = (1 + i_10X);
    goto L3049;}}
}
static void push_exception_continuationB(char * code_pointer_11X, long pc_12X, long code_13X, long exception_14X, long inst_size_15X)
{
  long data_17X;
  long n_16X;
 {  SstackS = ((SstackS) + -20);
  n_16X = ((((ScontS) - (SstackS)))>>2);
  data_17X = 3 + (((long) (SstackS)));
  *((long *) (((char *) (-3 + data_17X)))) = (((n_16X)<<2));
  *((long *) ((((char *) (-3 + data_17X))) + 4)) = pc_12X;
  *((long *) ((((char *) (-3 + data_17X))) + 8)) = code_13X;
  *((long *) ((((char *) (-3 + data_17X))) + 12)) = exception_14X;
  *((long *) ((((char *) (-3 + data_17X))) + 16)) = inst_size_15X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((long) code_pointer_11X));
  ScontS = (SstackS);
  return;}
}
static void enqueue_channelB(long index_18X, long status_19X)
{
  char * addr_24X;
  long x_23X;
  char * addr_22X;
  long val_21X;
  long channel_20X;
 {  channel_20X = *((Svm_channelsS) + index_18X);
  val_21X = ((status_19X)<<2);
  addr_22X = (((char *) (-3 + channel_20X))) + 16;S48_WRITE_BARRIER(channel_20X, addr_22X, val_21X);
  *((long *) addr_22X) = val_21X;
  if ((1 == (*((long *) ((((char *) (-3 + channel_20X))) + 12))))) {
    if ((1 == (Spending_channels_headS))) {
      Spending_channels_headS = channel_20X;
      Spending_channels_tailS = channel_20X;
      return;}
    else {
      x_23X = Spending_channels_tailS;
      addr_24X = (((char *) (-3 + x_23X))) + 12;S48_WRITE_BARRIER(x_23X, addr_24X, channel_20X);
      *((long *) addr_24X) = channel_20X;
      Spending_channels_tailS = channel_20X;
      return;}}
  else {
    return;}}
}
static long close_channelB(long channel_25X)
{
  long arg0K0;
  char * addr_33X;
  long status_32X;
  long v_31X;
  long v_30X;
  long v_29X;
  long v_28X;
  long x_27X;
  long os_index_26X;
 {  os_index_26X = (((*((long *) ((((char *) (-3 + channel_25X))) + 8))))>>2);
  x_27X = *((long *) ((((char *) (-3 + channel_25X))) + 16));
  if ((5 == x_27X)) {
    v_28X = ps_abort_fd_op(os_index_26X);enqueue_channelB(os_index_26X, v_28X);
    goto L11195;}
  else {
    goto L11195;}}
 L11195: {
  v_29X = *((long *) (((char *) (-3 + channel_25X))));
  if ((4 == v_29X)) {
    goto L11210;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_25X))))))) {
      goto L11210;}
    else {
      v_30X = ps_close_fd(os_index_26X);
      arg0K0 = v_30X;
      goto L11217;}}}
 L11210: {
  v_31X = ps_close_fd(os_index_26X);
  arg0K0 = v_31X;
  goto L11217;}
 L11217: {
  status_32X = arg0K0;
  *((Svm_channelsS) + os_index_26X) = 1;
  addr_33X = ((char *) (-3 + channel_25X));S48_WRITE_BARRIER(channel_25X, addr_33X, 0);
  *((long *) addr_33X) = 0;
  return status_32X;}
}
static long add_log_entryAgc(long proposal_index_34X, long i_35X, long stob_36X, long index_37X, long value_38X, char verifyP_39X)
{
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char * addr_65X;
  long value_64X;
  long value_63X;
  long v_62X;
  char * addr_61X;
  long i_60X;
  long stob_59X;
  long proposal_58X;
  long new_57X;
  char * addr_56X;
  char * addr_55X;
  long value_54X;
  long v_53X;
  long vector_52X;
  char * addr_51X;
  char * addr_50X;
  char * addr_49X;
  long log_48X;
  long value_47X;
  long stob_46X;
  long proposal_45X;
  char * addr_44X;
  long len_43X;
  long new_size_42X;
  long log_size_41X;
  long proposal_40X;
 {  proposal_40X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  log_size_41X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + proposal_40X))) + (((proposal_index_34X)<<2)))))))) + -4))))>>8))))>>2);
  if ((i_35X == (-1 + log_size_41X))) {
    Stemp0S = stob_36X;
    Stemp1S = value_38X;
    if ((1 == log_size_41X)) {
      arg0K0 = 17;
      goto L9530;}
    else {
      arg0K0 = (-1 + (((log_size_41X)<<1)));
      goto L9530;}}
  else {
    arg0K0 = proposal_40X;
    arg0K1 = stob_36X;
    arg0K2 = value_38X;
    goto L13088;}}
 L9530: {
  new_size_42X = arg0K0;
  len_43X = ((new_size_42X)<<2);
  addr_44X = s48_allocate_tracedAgc((4 + len_43X));
  if ((addr_44X == NULL)) {
    arg0K0 = 1;
    goto L9576;}
  else {
    *((long *) addr_44X) = (10 + (((len_43X)<<8)));
    arg0K0 = (3 + (((long) (addr_44X + 4))));
    goto L9576;}}
 L13088: {
  proposal_45X = arg0K0;
  stob_46X = arg0K1;
  value_47X = arg0K2;
  log_48X = *((long *) ((((char *) (-3 + proposal_45X))) + (((proposal_index_34X)<<2))));
  addr_49X = (((char *) (-3 + log_48X))) + (((i_35X)<<2));S48_WRITE_BARRIER(log_48X, addr_49X, stob_46X);
  *((long *) addr_49X) = stob_46X;
  addr_50X = (((char *) (-3 + log_48X))) + (4 + (((i_35X)<<2)));S48_WRITE_BARRIER(log_48X, addr_50X, index_37X);
  *((long *) addr_50X) = index_37X;
  addr_51X = (((char *) (-3 + log_48X))) + (8 + (((i_35X)<<2)));S48_WRITE_BARRIER(log_48X, addr_51X, value_47X);
  *((long *) addr_51X) = value_47X;
  if (verifyP_39X) {
    arg0K0 = value_47X;
    goto L13109;}
  else {
    arg0K0 = 29;
    goto L13109;}}
 L9576: {
  vector_52X = arg0K0;
  if ((1 == vector_52X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_53X;
    goto L9534;}
  else {
    arg0K0 = vector_52X;
    goto L9534;}}
 L13109: {
  value_54X = arg0K0;
  addr_55X = (((char *) (-3 + log_48X))) + (12 + (((i_35X)<<2)));S48_WRITE_BARRIER(log_48X, addr_55X, value_54X);
  *((long *) addr_55X) = value_54X;
  addr_56X = (((char *) (-3 + log_48X))) + (16 + (((i_35X)<<2)));S48_WRITE_BARRIER(log_48X, addr_56X, 1);
  *((long *) addr_56X) = 1;
  return value_47X;}
 L9534: {
  new_57X = arg0K0;
  proposal_58X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 < log_size_41X)) {
    stob_59X = *((long *) ((((char *) (-3 + proposal_58X))) + 4));
    memcpy((void *)(((char *) (-3 + new_57X))), (void *)(((char *) (-3 + stob_59X))),(-4 + (((log_size_41X)<<2))));
    goto L9554;}
  else {
    goto L9554;}}
 L9554: {
  arg0K0 = (4 + log_size_41X);
  goto L9558;}
 L9558: {
  i_60X = arg0K0;
  if ((i_60X == new_size_42X)) {
    addr_61X = (((char *) (-3 + proposal_58X))) + (((proposal_index_34X)<<2));S48_WRITE_BARRIER(proposal_58X, addr_61X, new_57X);
    *((long *) addr_61X) = new_57X;
    v_62X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
    value_63X = Stemp1S;
    Stemp1S = 1;
    value_64X = Stemp0S;
    Stemp0S = 1;
    arg0K0 = v_62X;
    arg0K1 = value_64X;
    arg0K2 = value_63X;
    goto L13088;}
  else {
    addr_65X = (((char *) (-3 + new_57X))) + (((i_60X)<<2));S48_WRITE_BARRIER(new_57X, addr_65X, 0);
    *((long *) addr_65X) = 0;
    arg0K0 = (1 + i_60X);
    goto L9558;}}
}
static long enter_bignum(char * external_bignum_66X)
{
  long desc_67X;
 {  desc_67X = 3 + (((long) external_bignum_66X));
  if ((3 == (3 & desc_67X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + desc_67X))) + -4))))>>2))))) {
      goto L13782;}
    else {
      goto L13796;}}
  else {
    goto L13796;}}
 L13782: {
  if ((3 == (3 & desc_67X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + desc_67X))) + -4)))))) {
      *((long *) ((((char *) (-3 + desc_67X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + desc_67X))) + -4))));
      return desc_67X;}
    else {
      return desc_67X;}}
  else {
    return desc_67X;}}
 L13796: {
  ps_error("not a bignum", 1, desc_67X);
  goto L13782;}
}
static long enter_stringAgc(char *string_68X)
{
  long arg0K0;
  long i_73X;
  long string_72X;
  char * addr_71X;
  long len_70X;
  long z_69X;
 {  z_69X = strlen((char *) string_68X);s48_make_availableAgc((4 + (-4 & (4 + z_69X))));
  len_70X = 1 + z_69X;
  addr_71X = s48_allocate_small((4 + len_70X));
  *((long *) addr_71X) = (70 + (((len_70X)<<8)));
  string_72X = 3 + (((long) (addr_71X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_72X))) + z_69X)) = 0;
  arg0K0 = 0;
  goto L14938;}
 L14938: {
  i_73X = arg0K0;
  if ((i_73X == z_69X)) {
    return string_72X;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_72X))) + i_73X)) = ((*(string_68X + i_73X)));
    arg0K0 = (1 + i_73X);
    goto L14938;}}
}
static long channel_close_error(long status_74X, long index_75X, long id_76X)
{
  long v_77X;
 {  ps_write_string("Error: ", (stderr));
  ps_write_string((ps_error_string(status_74X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(" while closing port ", (stderr));
  if ((3 == (3 & id_76X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + id_76X))) + -4))))>>2))))) {
      ps_write_string((((char *)(((char *) (-3 + id_76X))))), (stderr));
      goto L15802;}
    else {
      goto L15796;}}
  else {
    goto L15796;}}
 L15802: {
  PS_WRITE_CHAR(10, (stderr), v_77X)
  return v_77X;}
 L15796: {
  ps_write_integer((((index_75X)>>2)), (stderr));
  goto L15802;}
}
static long make_registered_channel(long mode_78X, long id_79X, long os_index_80X, long key_81X, long *TT0)
{
  long x_85X;
  char * addr_84X;
  char x_83X;
  char temp_82X;
 {  temp_82X = os_index_80X < (Snumber_of_channelsS);
  if (temp_82X) {
    goto L17409;}
  else {
    x_83X = add_more_channels(os_index_80X);
    if (x_83X) {
      goto L17409;}
    else {
      *TT0 = 9;
      return 1;}}}
 L17409: {
  if ((1 == (*((Svm_channelsS) + os_index_80X)))) {
    addr_84X = s48_allocate_small(24);
    *((long *) addr_84X) = 5146;
    x_85X = 3 + (((long) (addr_84X + 4)));
    *((long *) (((char *) (-3 + x_85X)))) = (((mode_78X)<<2));
    *((long *) ((((char *) (-3 + x_85X))) + 4)) = id_79X;
    *((long *) ((((char *) (-3 + x_85X))) + 8)) = (((os_index_80X)<<2));
    *((long *) ((((char *) (-3 + x_85X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_85X))) + 16)) = 1;
    *((Svm_channelsS) + os_index_80X) = x_85X;
    *TT0 = 9;
    return x_85X;}
  else {
    *TT0 = 11;
    return 1;}}
}
static long really_preserve_continuation(long key_86X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  char * next_99X;
  long pc_98X;
  char * pointer_97X;
  char * pointer_96X;
  long new_95X;
  char * addr_94X;
  long len_93X;
  long size_92X;
  long size_91X;
  char * pointer_90X;
  long previous_89X;
  char * cont_88X;
  long temp_87X;
 {  if (((ScontS) == (Sbottom_of_stackS))) {
    goto L18883;}
  else {
    temp_87X = Sheap_continuationS;
    arg1K0 = (ScontS);
    arg0K1 = 1;
    goto L18853;}}
 L18883: {
  return (Sheap_continuationS);}
 L18853: {
  cont_88X = arg1K0;
  previous_89X = arg0K1;
  if ((cont_88X == (Sbottom_of_stackS))) {
    *((long *) ((((char *) (-3 + previous_89X))) + 8)) = temp_87X;
    ScontS = (Sbottom_of_stackS);
    goto L18883;}
  else {
    pointer_90X = (((char *) (*((long *) cont_88X)))) + -2;
    size_91X = ((((*((unsigned char *) pointer_90X)))<<8)) + (*((unsigned char *) (pointer_90X + 1)));
    if ((65535 == size_91X)) {
      arg0K0 = ((((*((long *) (cont_88X + 4))))>>2));
      goto L17481;}
    else {
      arg0K0 = size_91X;
      goto L17481;}}}
 L17481: {
  size_92X = arg0K0;
  len_93X = 12 + (((size_92X)<<2));
  addr_94X = s48_allocate_small((4 + len_93X));
  *((long *) addr_94X) = (42 + (((len_93X)<<8)));
  new_95X = 3 + (((long) (addr_94X + 4)));
  pointer_96X = ((char *) (*((long *) cont_88X)));
  pointer_97X = pointer_96X + -5;
  pc_98X = ((((*((unsigned char *) pointer_97X)))<<8)) + (*((unsigned char *) (pointer_97X + 1)));
  memcpy((void *)((((char *) (-3 + new_95X))) + 12), (void *)(cont_88X + 4),(((size_92X)<<2)));
  *((long *) (((char *) (-3 + new_95X)))) = (((pc_98X)<<2));
  *((long *) ((((char *) (-3 + new_95X))) + 4)) = (3 + (((long) (pointer_96X + (0 - pc_98X)))));
  next_99X = cont_88X + (4 + (((size_92X)<<2)));
  if ((3 == (3 & previous_89X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + previous_89X))) + -4))))>>2))))) {
      *((long *) ((((char *) (-3 + previous_89X))) + 8)) = new_95X;
      arg1K0 = next_99X;
      arg0K1 = new_95X;
      goto L18853;}
    else {
      goto L18871;}}
  else {
    goto L18871;}}
 L18871: {
  Sheap_continuationS = new_95X;
  arg1K0 = next_99X;
  arg0K1 = new_95X;
  goto L18853;}
}
static long Hlookup2690(long table_100X, long string_101X, long key_102X)
{
  long arg0K1;
  long arg0K0;
  char * addr_117X;
  long value_116X;
  long link_115X;
  long x_114X;
  char * addr_113X;
  long next_112X;
  long len_111X;
  long s2_110X;
  long foo_109X;
  long bucket_108X;
  long link_107X;
  long index_106X;
  long h_105X;
  long i_104X;
  long n_103X;
 {  n_103X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_101X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L20645;}
 L20645: {
  i_104X = arg0K0;
  h_105X = arg0K1;
  if ((i_104X < n_103X)) {
    arg0K0 = (1 + i_104X);
    arg0K1 = (h_105X + (((*((unsigned char *) ((((char *) (-3 + string_101X))) + i_104X))))));
    goto L20645;}
  else {
    index_106X = 1023 & h_105X;
    link_107X = *((long *) ((((char *) (-3 + table_100X))) + (((index_106X)<<2))));
    if ((0 == (3 & link_107X))) {
      arg0K0 = (3 + (-4 & link_107X));
      goto L20601;}
    else {
      arg0K0 = link_107X;
      goto L20601;}}}
 L20601: {
  bucket_108X = arg0K0;
  arg0K0 = bucket_108X;
  goto L20607;}
 L20607: {
  foo_109X = arg0K0;
  if ((1 == foo_109X)) {
    if ((3 == (3 & bucket_108X))) {
      arg0K0 = (-4 & bucket_108X);
      goto L20612;}
    else {
      arg0K0 = bucket_108X;
      goto L20612;}}
  else {
    s2_110X = *((long *) (((char *) (-3 + foo_109X))));
    len_111X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_101X))) + -4))))>>8);
    if ((len_111X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_110X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_110X))), (void *)(((char *) (-3 + string_101X))),len_111X)))) {
        return foo_109X;}
      else {
        goto L20627;}}
    else {
      goto L20627;}}}
 L20612: {
  next_112X = arg0K0;
  addr_113X = s48_allocate_small(20);
  *((long *) addr_113X) = 4154;
  x_114X = 3 + (((long) (addr_113X + 4)));
  *((long *) (((char *) (-3 + x_114X)))) = string_101X;
  *((long *) ((((char *) (-3 + x_114X))) + 4)) = 1;
  *((long *) ((((char *) (-3 + x_114X))) + 8)) = 13;
  *((long *) ((((char *) (-3 + x_114X))) + 12)) = next_112X;
  if ((3 == (3 & x_114X))) {
    arg0K0 = (-4 & x_114X);
    goto L20618;}
  else {
    arg0K0 = x_114X;
    goto L20618;}}
 L20627: {
  link_115X = *((long *) ((((char *) (-3 + foo_109X))) + 12));
  if ((0 == (3 & link_115X))) {
    arg0K0 = (3 + (-4 & link_115X));
    goto L20607;}
  else {
    arg0K0 = link_115X;
    goto L20607;}}
 L20618: {
  value_116X = arg0K0;
  addr_117X = (((char *) (-3 + table_100X))) + (((index_106X)<<2));S48_WRITE_BARRIER(table_100X, addr_117X, value_116X);
  *((long *) addr_117X) = value_116X;
  return x_114X;}
}
static long Hlookup2671(long table_118X, long string_119X, long key_120X)
{
  long arg0K1;
  long arg0K0;
  char * addr_135X;
  long value_134X;
  long link_133X;
  long x_132X;
  char * addr_131X;
  long next_130X;
  long len_129X;
  long s2_128X;
  long foo_127X;
  long bucket_126X;
  long link_125X;
  long index_124X;
  long h_123X;
  long i_122X;
  long n_121X;
 {  n_121X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_119X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L20797;}
 L20797: {
  i_122X = arg0K0;
  h_123X = arg0K1;
  if ((i_122X < n_121X)) {
    arg0K0 = (1 + i_122X);
    arg0K1 = (h_123X + (((*((unsigned char *) ((((char *) (-3 + string_119X))) + i_122X))))));
    goto L20797;}
  else {
    index_124X = 1023 & h_123X;
    link_125X = *((long *) ((((char *) (-3 + table_118X))) + (((index_124X)<<2))));
    if ((0 == (3 & link_125X))) {
      arg0K0 = (3 + (-4 & link_125X));
      goto L20753;}
    else {
      arg0K0 = link_125X;
      goto L20753;}}}
 L20753: {
  bucket_126X = arg0K0;
  arg0K0 = bucket_126X;
  goto L20759;}
 L20759: {
  foo_127X = arg0K0;
  if ((1 == foo_127X)) {
    if ((3 == (3 & bucket_126X))) {
      arg0K0 = (-4 & bucket_126X);
      goto L20764;}
    else {
      arg0K0 = bucket_126X;
      goto L20764;}}
  else {
    s2_128X = *((long *) (((char *) (-3 + foo_127X))));
    len_129X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_119X))) + -4))))>>8);
    if ((len_129X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_128X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_128X))), (void *)(((char *) (-3 + string_119X))),len_129X)))) {
        return foo_127X;}
      else {
        goto L20779;}}
    else {
      goto L20779;}}}
 L20764: {
  next_130X = arg0K0;
  addr_131X = s48_allocate_small(20);
  *((long *) addr_131X) = 4154;
  x_132X = 3 + (((long) (addr_131X + 4)));
  *((long *) (((char *) (-3 + x_132X)))) = string_119X;
  *((long *) ((((char *) (-3 + x_132X))) + 4)) = 5;
  *((long *) ((((char *) (-3 + x_132X))) + 8)) = 13;
  *((long *) ((((char *) (-3 + x_132X))) + 12)) = next_130X;
  if ((3 == (3 & x_132X))) {
    arg0K0 = (-4 & x_132X);
    goto L20770;}
  else {
    arg0K0 = x_132X;
    goto L20770;}}
 L20779: {
  link_133X = *((long *) ((((char *) (-3 + foo_127X))) + 12));
  if ((0 == (3 & link_133X))) {
    arg0K0 = (3 + (-4 & link_133X));
    goto L20759;}
  else {
    arg0K0 = link_133X;
    goto L20759;}}
 L20770: {
  value_134X = arg0K0;
  addr_135X = (((char *) (-3 + table_118X))) + (((index_124X)<<2));S48_WRITE_BARRIER(table_118X, addr_135X, value_134X);
  *((long *) addr_135X) = value_134X;
  return x_132X;}
}
static void HtopD10322(void)
{
  long arg0K2;
  long arg0K1;
  long arg0K0;
  long new_173X;
  long status_172X;
  long id_171X;
  long v_170X;
  char v_169X;
  long channel_168X;
  long i_167X;
  char * addr_166X;
  long next_link_165X;
  long new_foo_164X;
  char v_163X;
  char * addr_162X;
  long foo_161X;
  char * addr_160X;
  long l_159X;
  long v_158X;
  long okay_link_157X;
  long foo_link_156X;
  char * addr_155X;
  char * addr_154X;
  char * addr_153X;
  char * addr_152X;
  long val_151X;
  char tracedP_150X;
  long next_149X;
  long thing_148X;
  long pair_147X;
  long alist_146X;
  long l2_145X;
  long goners_144X;
  long okay_143X;
  long alist_142X;
  long foo_link_141X;
  long v_140X;
  long pc_139X;
  long code_138X;
  long i_137X;
  long table_136X;
 {  table_136X = s48_trace_value((Sthe_symbol_tableS));
  arg0K0 = 0;
  goto L21612;}
 L21612: {
  i_137X = arg0K0;
  if ((1024 == i_137X)) {
    Sthe_symbol_tableS = table_136X;
    code_138X = Slast_code_calledS;
    pc_139X = Ssaved_pcS;
    Slast_code_calledS = code_138X;
    Scode_pointerS = ((((char *) (-3 + code_138X))) + pc_139X);
    Slast_code_pointer_resumedS = (Scode_pointerS);
    v_140X = SHARED_REF((Sfinalizer_alistS));
    arg0K0 = v_140X;
    arg0K1 = 25;
    arg0K2 = 25;
    goto L10809;}
  else {
    foo_link_141X = *((long *) ((((char *) (-3 + table_136X))) + (((i_137X)<<2))));
    arg0K0 = foo_link_141X;
    arg0K1 = 1;
    goto L14237;}}
 L10809: {
  alist_142X = arg0K0;
  okay_143X = arg0K1;
  goners_144X = arg0K2;
  if ((25 == alist_142X)) {SHARED_SETB((Sfinalizer_alistS), okay_143X);
    l2_145X = Sfinalize_theseS;
    if ((25 == goners_144X)) {
      arg0K0 = l2_145X;
      goto L10816;}
    else {
      arg0K0 = goners_144X;
      goto L10871;}}
  else {
    alist_146X = s48_trace_value(alist_142X);
    pair_147X = s48_trace_value((*((long *) (((char *) (-3 + alist_146X))))));
    thing_148X = *((long *) (((char *) (-3 + pair_147X))));
    next_149X = *((long *) ((((char *) (-3 + alist_146X))) + 4));
    tracedP_150X = s48_extantP(thing_148X);
    val_151X = s48_trace_value(thing_148X);
    addr_152X = ((char *) (-3 + pair_147X));S48_WRITE_BARRIER(pair_147X, addr_152X, val_151X);
    *((long *) addr_152X) = val_151X;
    addr_153X = ((char *) (-3 + alist_146X));S48_WRITE_BARRIER(alist_146X, addr_153X, pair_147X);
    *((long *) addr_153X) = pair_147X;
    if (tracedP_150X) {
      addr_154X = (((char *) (-3 + alist_146X))) + 4;S48_WRITE_BARRIER(alist_146X, addr_154X, okay_143X);
      *((long *) addr_154X) = okay_143X;
      arg0K0 = next_149X;
      arg0K1 = alist_146X;
      arg0K2 = goners_144X;
      goto L10809;}
    else {
      addr_155X = (((char *) (-3 + alist_146X))) + 4;S48_WRITE_BARRIER(alist_146X, addr_155X, goners_144X);
      *((long *) addr_155X) = goners_144X;
      arg0K0 = next_149X;
      arg0K1 = okay_143X;
      arg0K2 = alist_146X;
      goto L10809;}}}
 L14237: {
  foo_link_156X = arg0K0;
  okay_link_157X = arg0K1;
  if ((0 == (3 & foo_link_156X))) {
    arg0K0 = (3 + (-4 & foo_link_156X));
    goto L14239;}
  else {
    arg0K0 = foo_link_156X;
    goto L14239;}}
 L10816: {
  v_158X = arg0K0;
  Sfinalize_theseS = v_158X;
  arg0K0 = 0;
  goto L20179;}
 L10871: {
  l_159X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_159X))) + 4))))) {
    addr_160X = (((char *) (-3 + l_159X))) + 4;S48_WRITE_BARRIER(l_159X, addr_160X, l2_145X);
    *((long *) addr_160X) = l2_145X;
    arg0K0 = goners_144X;
    goto L10816;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_159X))) + 4)));
    goto L10871;}}
 L14239: {
  foo_161X = arg0K0;
  if ((1 == foo_161X)) {
    addr_162X = (((char *) (-3 + table_136X))) + (((i_137X)<<2));S48_WRITE_BARRIER(table_136X, addr_162X, okay_link_157X);
    *((long *) addr_162X) = okay_link_157X;
    arg0K0 = (1 + i_137X);
    goto L21612;}
  else {
    v_163X = s48_extantP(foo_161X);
    if (v_163X) {
      new_foo_164X = s48_trace_value(foo_161X);
      next_link_165X = *((long *) ((((char *) (-3 + new_foo_164X))) + 4));
      addr_166X = (((char *) (-3 + new_foo_164X))) + 4;S48_WRITE_BARRIER(new_foo_164X, addr_166X, okay_link_157X);
      *((long *) addr_166X) = okay_link_157X;
      if ((3 == (3 & new_foo_164X))) {
        arg0K0 = next_link_165X;
        arg0K1 = (-4 & new_foo_164X);
        goto L14237;}
      else {
        arg0K0 = next_link_165X;
        arg0K1 = new_foo_164X;
        goto L14237;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + foo_161X))) + 4)));
      arg0K1 = okay_link_157X;
      goto L14237;}}}
 L20179: {
  i_167X = arg0K0;
  if ((i_167X == (Snumber_of_channelsS))) {
    Spending_interruptsS = (4 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Sstack_limitS = (Sreal_stack_limitS);
      if ((s48_Spending_eventsPS)) {
        s48_Sstack_limitS = (((char *) -1));
        return;}
      else {
        return;}}
    else {
      s48_Sstack_limitS = (((char *) -1));
      return;}}
  else {
    channel_168X = *((Svm_channelsS) + i_167X);
    if ((1 == channel_168X)) {
      goto L20213;}
    else {
      v_169X = s48_extantP(channel_168X);
      if (v_169X) {
        v_170X = s48_trace_value(channel_168X);
        arg0K0 = v_170X;
        goto L20206;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_168X))))))) {
          arg0K0 = 1;
          goto L20206;}
        else {
          id_171X = *((long *) ((((char *) (-3 + channel_168X))) + 4));
          status_172X = close_channelB(channel_168X);
          if ((status_172X == NO_ERRORS)) {
            goto L18343;}
          else {channel_close_error(status_172X, (*((long *) ((((char *) (-3 + channel_168X))) + 8))), id_171X);
            goto L18343;}}}}}}
 L20213: {
  arg0K0 = (1 + i_167X);
  goto L20179;}
 L20206: {
  new_173X = arg0K0;
  *((Svm_channelsS) + i_167X) = new_173X;
  goto L20213;}
 L18343: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_171X))) {
    ps_write_integer((((id_171X)>>2)), (stderr));
    goto L18361;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_171X))))), (stderr));
    goto L18361;}}
 L18361: {
  ps_write_string(" ", (stderr));
  ps_write_integer(((((*((long *) ((((char *) (-3 + channel_168X))) + 8))))>>2)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L20206;}
}
static long current_code_vector(void)
{
  long arg0K0;
  long merged_arg0K0;

  int safe_byte_vector_length_return_tag;
  long safe_byte_vector_length0_return_value;
  long code_174X;
  long header_187X;
  long code_186X;
  long v_185X;
  long v_184X;
  char * start_183X;
  char * code_pointer_182X;
  long code_181X;
  char * pointer_180X;
  char * code_pointer_179X;
  long v_178X;
  char * start_177X;
  long code_176X;
  char * code_pointer_175X;
 {  code_pointer_175X = Scode_pointerS;
  code_176X = Slast_code_calledS;
  start_177X = ((char *) (-3 + code_176X));
  if ((code_pointer_175X < start_177X)) {
    goto L21938;}
  else {
    merged_arg0K0 = code_176X;
    safe_byte_vector_length_return_tag = 0;
    goto safe_byte_vector_length;
   safe_byte_vector_length_return_0:
    v_178X = safe_byte_vector_length0_return_value;
    if ((code_pointer_175X < (start_177X + v_178X))) {
      return (Slast_code_calledS);}
    else {
      goto L21938;}}}
 L21938: {
  code_pointer_179X = Slast_code_pointer_resumedS;
  pointer_180X = code_pointer_179X + -5;
  code_181X = 3 + (((long) (code_pointer_179X + (0 - (((((*((unsigned char *) pointer_180X)))<<8)) + (*((unsigned char *) (pointer_180X + 1))))))));
  code_pointer_182X = Scode_pointerS;
  start_183X = ((char *) (-3 + code_181X));
  if ((code_pointer_182X < start_183X)) {
    goto L21946;}
  else {
    merged_arg0K0 = code_181X;
    safe_byte_vector_length_return_tag = 1;
    goto safe_byte_vector_length;
   safe_byte_vector_length_return_1:
    v_184X = safe_byte_vector_length0_return_value;
    if ((code_pointer_182X < (start_183X + v_184X))) {
      return code_181X;}
    else {
      goto L21946;}}}
 L21946: {
  ps_error("VM error: unable to locate current code vector", 0);
  return v_185X;}
 safe_byte_vector_length: {
  code_174X = merged_arg0K0;{
  arg0K0 = code_186X;
  goto L52555;}
 L52555: {
  code_186X = arg0K0;
  header_187X = *((long *) ((((char *) (-3 + code_174X))) + -4));
  if ((3 == (3 & header_187X))) {
    arg0K0 = header_187X;
    goto L52555;}
  else {
    safe_byte_vector_length0_return_value = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_174X))) + -4))))>>8));
    goto safe_byte_vector_length_return;}}
 safe_byte_vector_length_return:
  switch (safe_byte_vector_length_return_tag) {
  case 0: goto safe_byte_vector_length_return_0;
  default: goto safe_byte_vector_length_return_1;
  }}

}
static void HtopD10333(void)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long merged_arg0K0;

  int Hentry_tracer861188_return_tag;
  long Hentry_tracer8611880_return_value;
  long foo_link_189X;
  char * addr_261X;
  long next_link_260X;
  long new_foo_259X;
  long foo_258X;
  long done_link_257X;
  long foo_link_256X;
  long v_255X;
  long size_254X;
  char * pointer_253X;
  long v_252X;
  long v_251X;
  long v_250X;
  long cells_249X;
  long size_248X;
  char * pointer_247X;
  char * contents_pointer_246X;
  long new_code_245X;
  long mask_size_244X;
  char * code_pointer_243X;
  long pc_242X;
  char * pointer_241X;
  char * pointer_240X;
  char * cont_239X;
  long unused_238X;
  char * a_237X;
  char * addr_236X;
  long val_235X;
  char * addr_234X;
  long val_233X;
  long channel_232X;
  long i_231X;
  char x_230X;
  long pair_229X;
  long v_228X;
  long v_227X;
  long alist_226X;
  char * addr_225X;
  long value_224X;
  long v_223X;
  long v_222X;
  long v_221X;
  long v_220X;
  long v_219X;
  long v_218X;
  long v_217X;
  long v_216X;
  long v_215X;
  long v_214X;
  long v_213X;
  long v_212X;
  long v_211X;
  long v_210X;
  long v_209X;
  long v_208X;
  long v_207X;
  long v_206X;
  long code_205X;
  long i_204X;
  char * addr_203X;
  long value_202X;
  long table_201X;
  long i_200X;
  long x2_199X;
  char * cell_198X;
  long i_197X;
  long x2_196X;
  char * cell_195X;
  long table_194X;
  char * frame_193X;
  long length_192X;
  char * frame_191X;
  long v_190X;
 {  v_190X = s48_trace_value((Sempty_logS));
  Sempty_logS = v_190X;
  arg1K0 = (Sexternal_root_stackS);
  goto L7607;}
 L7607: {
  frame_191X = arg1K0;
  if ((frame_191X == NULL)) {
    arg1K0 = (Spermanent_external_rootsS);
    goto L7633;}
  else {
    length_192X = *((long *) frame_191X);
    arg0K0 = 0;
    goto L7615;}}
 L7633: {
  frame_193X = arg1K0;
  if ((frame_193X == NULL)) {s48_initializing_gc_root();
    table_194X = s48_trace_value((Simported_bindingsS));
    arg0K0 = 0;
    goto L18684;}
  else {
    cell_195X = ((char *) (*((long *) (frame_193X + 4))));
    x2_196X = s48_trace_value((*((long *) cell_195X)));
    *((long *) cell_195X) = x2_196X;
    arg1K0 = (((char *) (*((long *) frame_193X))));
    goto L7633;}}
 L7615: {
  i_197X = arg0K0;
  if ((i_197X == length_192X)) {
    arg1K0 = (((char *) (*((long *) (frame_191X + 4)))));
    goto L7607;}
  else {
    cell_198X = ((char *) (*((long *) (frame_191X + (8 + (((i_197X)<<2)))))));
    x2_199X = s48_trace_value((*((long *) cell_198X)));
    *((long *) cell_198X) = x2_199X;
    arg0K0 = (1 + i_197X);
    goto L7615;}}
 L18684: {
  i_200X = arg0K0;
  if ((1024 == i_200X)) {
    Simported_bindingsS = table_194X;
    table_201X = s48_trace_value((Sexported_bindingsS));
    arg0K0 = 0;
    goto L18705;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_194X))) + (((i_200X)<<2)))));
    Hentry_tracer861188_return_tag = 0;
    goto Hentry_tracer861188;
   Hentry_tracer861188_return_0:
    value_202X = Hentry_tracer8611880_return_value;
    addr_203X = (((char *) (-3 + table_194X))) + (((i_200X)<<2));S48_WRITE_BARRIER(table_194X, addr_203X, value_202X);
    *((long *) addr_203X) = value_202X;
    arg0K0 = (1 + i_200X);
    goto L18684;}}
 L18705: {
  i_204X = arg0K0;
  if ((1024 == i_204X)) {
    Sexported_bindingsS = table_201X;
    code_205X = current_code_vector();
    Ssaved_pcS = ((Scode_pointerS) - (((char *) (-3 + code_205X))));
    v_206X = s48_trace_value(code_205X);
    Slast_code_calledS = v_206X;
    v_207X = s48_trace_value((SvalS));
    SvalS = v_207X;
    v_208X = s48_trace_value((Scurrent_threadS));
    Scurrent_threadS = v_208X;
    v_209X = s48_trace_value((Sinterrupt_return_codeS));
    Sinterrupt_return_codeS = v_209X;
    v_210X = s48_trace_value((Sexception_return_codeS));
    Sexception_return_codeS = v_210X;
    v_211X = s48_trace_value((Scall_with_values_return_codeS));
    Scall_with_values_return_codeS = v_211X;
    v_212X = s48_trace_value((Sinterrupted_templateS));
    Sinterrupted_templateS = v_212X;
    v_213X = s48_trace_value((Sos_signal_typeS));
    Sos_signal_typeS = v_213X;
    v_214X = s48_trace_value((Sos_signal_argumentS));
    Sos_signal_argumentS = v_214X;
    v_215X = SHARED_REF((Ssession_dataS));
    v_216X = s48_trace_value(v_215X);SHARED_SETB((Ssession_dataS), v_216X);
    v_217X = SHARED_REF((Sexception_handlersS));
    v_218X = s48_trace_value(v_217X);SHARED_SETB((Sexception_handlersS), v_218X);
    v_219X = SHARED_REF((Sinterrupt_handlersS));
    v_220X = s48_trace_value(v_219X);SHARED_SETB((Sinterrupt_handlersS), v_220X);
    v_221X = SHARED_REF((Sfinalize_theseS));
    v_222X = s48_trace_value(v_221X);SHARED_SETB((Sfinalize_theseS), v_222X);
    v_223X = SHARED_REF((Sfinalizer_alistS));
    arg0K0 = v_223X;
    goto L7673;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_201X))) + (((i_204X)<<2)))));
    Hentry_tracer861188_return_tag = 1;
    goto Hentry_tracer861188;
   Hentry_tracer861188_return_1:
    value_224X = Hentry_tracer8611880_return_value;
    addr_225X = (((char *) (-3 + table_201X))) + (((i_204X)<<2));S48_WRITE_BARRIER(table_201X, addr_225X, value_224X);
    *((long *) addr_225X) = value_224X;
    arg0K0 = (1 + i_204X);
    goto L18705;}}
 L7673: {
  alist_226X = arg0K0;
  if ((25 == alist_226X)) {
    v_227X = s48_trace_value((Spending_channels_headS));
    Spending_channels_headS = v_227X;
    v_228X = s48_trace_value((Spending_channels_tailS));
    Spending_channels_tailS = v_228X;
    arg0K0 = 0;
    goto L10941;}
  else {
    pair_229X = *((long *) (((char *) (-3 + alist_226X))));
    x_230X = s48_extantP((*((long *) (((char *) (-3 + pair_229X))))));
    if (x_230X) {
      goto L7698;}
    else {s48_trace_stob_contentsB((*((long *) (((char *) (-3 + pair_229X))))));
      goto L7698;}}}
 L10941: {
  i_231X = arg0K0;
  if ((i_231X == (Snumber_of_channelsS))) {
    if ((Sstack_warningPS)) {
      arg1K0 = (Sstack_beginS);
      goto L5244;}
    else {
      goto L14489;}}
  else {
    channel_232X = *((Svm_channelsS) + i_231X);
    if ((1 == channel_232X)) {
      goto L10969;}
    else {
      if ((0 == (*((long *) (((char *) (-3 + channel_232X))))))) {
        goto L10969;}
      else {
        val_233X = s48_trace_value((*((long *) ((((char *) (-3 + channel_232X))) + 4))));
        addr_234X = (((char *) (-3 + channel_232X))) + 4;S48_WRITE_BARRIER(channel_232X, addr_234X, val_233X);
        *((long *) addr_234X) = val_233X;
        goto L10969;}}}}
 L7698: {
  val_235X = s48_trace_value((*((long *) ((((char *) (-3 + pair_229X))) + 4))));
  addr_236X = (((char *) (-3 + pair_229X))) + 4;S48_WRITE_BARRIER(pair_229X, addr_236X, val_235X);
  *((long *) addr_236X) = val_235X;
  arg0K0 = (*((long *) ((((char *) (-3 + alist_226X))) + 4)));
  goto L7673;}
 L5244: {
  a_237X = arg1K0;
  if ((252645135 == (*((long *) a_237X)))) {
    arg1K0 = (a_237X + 4);
    goto L5244;}
  else {
    unused_238X = (((a_237X - (Sstack_beginS)))>>2);
    if ((unused_238X < 30)) {
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      ps_write_string("[Alert: stack overconsumption (", (stderr));
      ps_write_integer(unused_238X, (stderr));
      ps_write_string("); please inform the Scheme 48 implementors]", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sstack_warningPS = 0;
      goto L14489;}
    else {
      goto L14489;}}}
 L14489: {
s48_trace_locationsB((SstackS), ((SstackS) + (-4 & ((ScontS) - (SstackS)))));
  arg1K0 = (ScontS);
  goto L14499;}
 L10969: {
  arg0K0 = (1 + i_231X);
  goto L10941;}
 L14499: {
  cont_239X = arg1K0;
  pointer_240X = ((char *) (*((long *) cont_239X)));
  pointer_241X = pointer_240X + -5;
  pc_242X = ((((*((unsigned char *) pointer_241X)))<<8)) + (*((unsigned char *) (pointer_241X + 1)));
  code_pointer_243X = ((char *) (*((long *) cont_239X)));
  mask_size_244X = *((unsigned char *) (code_pointer_243X + -3));
  new_code_245X = s48_trace_value((3 + (((long) (pointer_240X + (0 - pc_242X))))));
  contents_pointer_246X = cont_239X + 4;
  *((long *) cont_239X) = (((long) ((((char *) (-3 + new_code_245X))) + pc_242X)));
  if ((0 == mask_size_244X)) {
    pointer_247X = (((char *) (*((long *) cont_239X)))) + -2;
    size_248X = ((((*((unsigned char *) pointer_247X)))<<8)) + (*((unsigned char *) (pointer_247X + 1)));
    if ((65535 == size_248X)) {
      arg0K0 = ((((*((long *) (cont_239X + 4))))>>2));
      goto L12069;}
    else {
      arg0K0 = size_248X;
      goto L12069;}}
  else {s48_trace_continuation_contentsB(contents_pointer_246X, code_pointer_243X, mask_size_244X);
    goto L14509;}}
 L12069: {
  cells_249X = arg0K0;s48_trace_locationsB(contents_pointer_246X, (contents_pointer_246X + (((cells_249X)<<2))));
  goto L14509;}
 L14509: {
  if ((cont_239X == (Sbottom_of_stackS))) {
    v_250X = s48_trace_value((Sheap_continuationS));
    Sheap_continuationS = v_250X;
    v_251X = s48_trace_value((Stemp0S));
    Stemp0S = v_251X;
    v_252X = s48_trace_value((Stemp1S));
    Stemp1S = v_252X;
    return;}
  else {
    pointer_253X = (((char *) (*((long *) cont_239X)))) + -2;
    size_254X = ((((*((unsigned char *) pointer_253X)))<<8)) + (*((unsigned char *) (pointer_253X + 1)));
    if ((65535 == size_254X)) {
      arg0K0 = ((((*((long *) (cont_239X + 4))))>>2));
      goto L14547;}
    else {
      arg0K0 = size_254X;
      goto L14547;}}}
 L14547: {
  v_255X = arg0K0;
  arg1K0 = (cont_239X + (4 + (((v_255X)<<2))));
  goto L14499;}
 Hentry_tracer861188: {
  foo_link_189X = merged_arg0K0;{
  arg0K0 = foo_link_189X;
  arg0K1 = 1;
  goto L14309;}
 L14309: {
  foo_link_256X = arg0K0;
  done_link_257X = arg0K1;
  if ((0 == (3 & foo_link_256X))) {
    arg0K0 = (3 + (-4 & foo_link_256X));
    goto L14311;}
  else {
    arg0K0 = foo_link_256X;
    goto L14311;}}
 L14311: {
  foo_258X = arg0K0;
  if ((1 == foo_258X)) {
    Hentry_tracer8611880_return_value = done_link_257X;
    goto Hentry_tracer861188_return;}
  else {
    new_foo_259X = s48_trace_value(foo_258X);
    next_link_260X = *((long *) ((((char *) (-3 + new_foo_259X))) + 12));
    addr_261X = (((char *) (-3 + new_foo_259X))) + 12;S48_WRITE_BARRIER(new_foo_259X, addr_261X, done_link_257X);
    *((long *) addr_261X) = done_link_257X;
    if ((3 == (3 & new_foo_259X))) {
      arg0K0 = next_link_260X;
      arg0K1 = (-4 & new_foo_259X);
      goto L14309;}
    else {
      arg0K0 = next_link_260X;
      arg0K1 = new_foo_259X;
      goto L14309;}}}
 Hentry_tracer861188_return:
  switch (Hentry_tracer861188_return_tag) {
  case 0: goto Hentry_tracer861188_return_0;
  default: goto Hentry_tracer861188_return_1;
  }}

}
void s48_set_native_protocolB(long protocol_262X)
{

 {  s48_Snative_protocolS = protocol_262X;
  return;}
}
void s48_set_extension_valueB(long value_263X)
{

 {  s48_Sextension_valueS = value_263X;
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
void s48_set_os_signal(long type_264X, long argument_265X)
{

 {  Sos_signal_typeS = type_264X;
  Sos_signal_argumentS = argument_265X;
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
  char * old_base_266X;
 {  old_base_266X = Sexternal_root_stack_baseS;
  Sexternal_root_stack_baseS = (Sexternal_root_stackS);
  return old_base_266X;}
}
char s48_release_gc_roots_baseB(char * old_base_267X)
{
  char okayP_268X;
 {  okayP_268X = (Sexternal_root_stackS) == (Sexternal_root_stack_baseS);
  Sexternal_root_stackS = old_base_267X;
  Sexternal_root_stack_baseS = old_base_267X;
  return okayP_268X;}
}
void s48_reset_external_rootsB(void)
{

 {  Sexternal_root_stackS = NULL;
  Sexternal_root_stack_baseS = NULL;
  Spermanent_external_rootsS = NULL;
  return;}
}
void s48_note_event(void)
{

 {  s48_Spending_eventsPS = 1;
  s48_Sstack_limitS = (((char *) -1));
  return;}
}
void s48_disable_interruptsB(void)
{

 {  s48_Spending_interruptPS = 0;
  Senabled_interruptsS = 0;
  return;}
}
void s48_push_gc_rootsB(char * frame_269X, long n_270X)
{

 {  *((long *) frame_269X) = n_270X;
  *((long *) (frame_269X + 4)) = (((long) (Sexternal_root_stackS)));
  Sexternal_root_stackS = frame_269X;
  return;}
}
void s48_register_gc_rootB(char * loc_addr_271X)
{
  char * frame_272X;
 {  frame_272X = (char *)malloc(8);
  if ((frame_272X == NULL)) {
    ps_error("out of memory registering a global root", 0);
    goto L3886;}
  else {
    goto L3886;}}
 L3886: {
  *((long *) frame_272X) = (((long) (Spermanent_external_rootsS)));
  *((long *) (frame_272X + 4)) = (((long) loc_addr_271X));
  Spermanent_external_rootsS = frame_272X;
  return;}
}
void s48_stack_setB(long x_273X, long value_274X)
{

 {  *((long *) ((SstackS) + (((x_273X)<<2)))) = value_274X;
  return;}
}
long s48_stack_ref(long i_275X)
{

 {  return (*((long *) ((SstackS) + (((i_275X)<<2)))));}
}
void s48_push(long x_276X)
{

 {  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_276X;
  return;}
}
long s48_resetup_external_exception(long new_why_277X, long additional_nargs_278X)
{
  long old_why_280X;
  long old_nargs_279X;
 {  old_nargs_279X = Sexternal_exception_nargsS;
  old_why_280X = *((long *) ((SstackS) + (((old_nargs_279X)<<2))));
  *((long *) ((SstackS) + (((old_nargs_279X)<<2)))) = (((new_why_277X)<<2));
  Sexternal_exception_nargsS = (old_nargs_279X + additional_nargs_278X);
  return old_why_280X;}
}
char s48_pop_gc_rootsB(void)
{

 {  if (((Sexternal_root_stackS) == (Sexternal_root_stack_baseS))) {
    return 0;}
  else {
    Sexternal_root_stackS = (((char *) (*((long *) ((Sexternal_root_stackS) + 4)))));
    return 1;}}
}
char * s48_shorten_bignum(char * external_bignum_281X, long number_of_digits_282X)
{
  long waste_size_287X;
  long old_data_size_286X;
  long new_data_size_285X;
  long new_size_284X;
  long bignum_283X;
 {  bignum_283X = 3 + (((long) external_bignum_281X));
  new_size_284X = 8 + (((number_of_digits_282X)<<2));
  new_data_size_285X = -4 + new_size_284X;
  old_data_size_286X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + bignum_283X))) + -4))))>>8);
  waste_size_287X = old_data_size_286X - new_data_size_285X;
  if ((waste_size_287X < 0)) {
    ps_error("shorten bignum", 2, new_data_size_285X, old_data_size_286X);
    goto L6984;}
  else {
    goto L6984;}}
 L6984: {
  if ((waste_size_287X < 4)) {
    return external_bignum_281X;}
  else {
    *((long *) ((((char *) (-3 + bignum_283X))) + -4)) = (82 + (((new_data_size_285X)<<8)));
    *((long *) ((((char *) (((long) ((((char *) (-3 + bignum_283X))) + (-4 & (3 + new_size_284X))))))) + -4)) = (-942 + (((waste_size_287X)<<8)));
    return external_bignum_281X;}}
}
long s48_allocate_bignum(long size_288X)
{
  char * addr_289X;
 {  addr_289X = s48_allocate_small((4 + size_288X));
  *((long *) addr_289X) = (82 + (((size_288X)<<8)));
  return (3 + (((long) (addr_289X + 4))));}
}
void s48_enable_interruptsB(void)
{

 {  Senabled_interruptsS = -1;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Sstack_limitS = (Sreal_stack_limitS);
    if ((s48_Spending_eventsPS)) {
      s48_Sstack_limitS = (((char *) -1));
      return;}
    else {
      return;}}
  else {
    s48_Sstack_limitS = (((char *) -1));
    return;}}
}
long s48_set_channel_os_index(long channel_290X, long os_index_291X)
{
  char * addr_298X;
  long val_297X;
  long v_296X;
  long x_295X;
  long old_index_294X;
  char x_293X;
  char temp_292X;
 {  temp_292X = os_index_291X < (Snumber_of_channelsS);
  if (temp_292X) {
    goto L11100;}
  else {
    x_293X = add_more_channels(os_index_291X);
    if (x_293X) {
      goto L11100;}
    else {
      return 36;}}}
 L11100: {
  if ((1 == (*((Svm_channelsS) + os_index_291X)))) {
    old_index_294X = (((*((long *) ((((char *) (-3 + channel_290X))) + 8))))>>2);
    x_295X = *((long *) ((((char *) (-3 + channel_290X))) + 16));
    if ((5 == x_295X)) {
      v_296X = ps_abort_fd_op(old_index_294X);enqueue_channelB(old_index_294X, v_296X);
      goto L11086;}
    else {
      goto L11086;}}
  else {
    return 44;}}
 L11086: {
  *((Svm_channelsS) + old_index_294X) = 1;
  *((Svm_channelsS) + os_index_291X) = channel_290X;
  val_297X = ((os_index_291X)<<2);
  addr_298X = (((char *) (-3 + channel_290X))) + 8;S48_WRITE_BARRIER(channel_290X, addr_298X, val_297X);
  *((long *) addr_298X) = val_297X;
  return 5;}
}
void s48_close_channel(long os_index_299X)
{
  long obj_300X;
 {  if ((os_index_299X < 0)) {
    return;}
  else {
    if ((os_index_299X < (Snumber_of_channelsS))) {
      obj_300X = *((Svm_channelsS) + os_index_299X);
      if ((3 == (3 & obj_300X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_300X))) + -4))))>>2))))) {close_channelB((*((Svm_channelsS) + os_index_299X)));
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
  long link_308X;
  long name_307X;
  long entry_306X;
  long link_305X;
  char temp_304X;
  long i_303X;
  long table_302X;
  FILE * out_301X;
 {  out_301X = stderr;
  table_302X = Simported_bindingsS;
  arg0K0 = 0;
  goto L16697;}
 L16697: {
  i_303X = arg0K0;
  temp_304X = 1024 == i_303X;
  if (temp_304X) {
    return temp_304X;}
  else {
    link_305X = *((long *) ((((char *) (-3 + table_302X))) + (((i_303X)<<2))));
    if ((0 == (3 & link_305X))) {
      arg0K0 = (3 + (-4 & link_305X));
      goto L16671;}
    else {
      arg0K0 = link_305X;
      goto L16671;}}}
 L16671: {
  entry_306X = arg0K0;
  if ((1 == entry_306X)) {
    arg0K0 = (1 + i_303X);
    goto L16697;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + entry_306X))) + 8)))))) {
      name_307X = *((long *) (((char *) (-3 + entry_306X))));
      ps_write_string("undefined imported name ", out_301X);
      if ((3 == (3 & name_307X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_307X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + name_307X))))), out_301X);
          goto L18595;}
        else {
          goto L18593;}}
      else {
        goto L18593;}}
    else {
      goto L16678;}}}
 L18595: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_301X, ignoreXX) }
  goto L16678;}
 L18593: {
  ps_write_string("<invalid name>", out_301X);
  goto L18595;}
 L16678: {
  link_308X = *((long *) ((((char *) (-3 + entry_306X))) + 12));
  if ((0 == (3 & link_308X))) {
    arg0K0 = (3 + (-4 & link_308X));
    goto L16671;}
  else {
    arg0K0 = link_308X;
    goto L16671;}}
}
void check_stack(void)
{
  char * arg1K0;
  char * arg1K1;
  long arg0K0;
  long v_330X;
  long address_329X;
  long x_328X;
  char * ptr_327X;
  long mask_326X;
  long size_325X;
  char * pointer_324X;
  long address_323X;
  long x_322X;
  char * addr_321X;
  char * trace_ptr_320X;
  char * mask_ptr_319X;
  long v_318X;
  char * mask_pointer_317X;
  long size_316X;
  char * pointer_315X;
  char * contents_pointer_314X;
  long mask_size_313X;
  char * code_pointer_312X;
  char * cont_311X;
  char * v_310X;
  char * v_309X;
 {  v_309X = s48_oldspace_begin();
  Soldspace_beginS = (((long) v_309X));
  v_310X = s48_oldspace_end();
  Soldspace_endS = (((long) v_310X));
  arg1K0 = (ScontS);
  goto L18733;}
 L18733: {
  cont_311X = arg1K0;
  if ((cont_311X == (Sbottom_of_stackS))) {
    return;}
  else {
    code_pointer_312X = ((char *) (*((long *) cont_311X)));
    mask_size_313X = *((unsigned char *) (code_pointer_312X + -3));
    contents_pointer_314X = cont_311X + 4;
    if ((0 == mask_size_313X)) {
      pointer_315X = (((char *) (*((long *) cont_311X)))) + -2;
      size_316X = ((((*((unsigned char *) pointer_315X)))<<8)) + (*((unsigned char *) (pointer_315X + 1)));
      if ((65535 == size_316X)) {
        arg0K0 = ((((*((long *) (cont_311X + 4))))>>2));
        goto L14465;}
      else {
        arg0K0 = size_316X;
        goto L14465;}}
    else {
      mask_pointer_317X = code_pointer_312X + -7;
      arg1K0 = (mask_pointer_317X + (0 - mask_size_313X));
      arg1K1 = contents_pointer_314X;
      goto L5566;}}}
 L14465: {
  v_318X = arg0K0;
  arg1K0 = contents_pointer_314X;
  goto L5499;}
 L5566: {
  mask_ptr_319X = arg1K0;
  trace_ptr_320X = arg1K1;
  if ((mask_ptr_319X == mask_pointer_317X)) {
    goto L16916;}
  else {
    arg0K0 = (*((unsigned char *) mask_ptr_319X));
    arg1K1 = trace_ptr_320X;
    goto L5574;}}
 L5499: {
  addr_321X = arg1K0;
  if ((addr_321X < (cont_311X + (4 + (((v_318X)<<2)))))) {
    x_322X = *((long *) addr_321X);
    if ((2 == (3 & x_322X))) {
      goto L5519;}
    else {
      if ((3 == (3 & x_322X))) {
        address_323X = -3 + x_322X;
        if ((address_323X < (Soldspace_beginS))) {
          goto L5504;}
        else {
          if (((Soldspace_endS) < address_323X)) {
            goto L5504;}
          else {
            goto L5519;}}}
      else {
        goto L5504;}}}
  else {
    goto L16916;}}
 L16916: {
  pointer_324X = (((char *) (*((long *) cont_311X)))) + -2;
  size_325X = ((((*((unsigned char *) pointer_324X)))<<8)) + (*((unsigned char *) (pointer_324X + 1)));
  if ((65535 == size_325X)) {
    arg0K0 = ((((*((long *) (cont_311X + 4))))>>2));
    goto L16921;}
  else {
    arg0K0 = size_325X;
    goto L16921;}}
 L5574: {
  mask_326X = arg0K0;
  ptr_327X = arg1K1;
  if ((0 == mask_326X)) {
    arg1K0 = (mask_ptr_319X + 1);
    arg1K1 = (trace_ptr_320X + 32);
    goto L5566;}
  else {
    if ((1 == (1 & mask_326X))) {
      x_328X = *((long *) ptr_327X);
      if ((2 == (3 & x_328X))) {
        goto L5631;}
      else {
        if ((3 == (3 & x_328X))) {
          address_329X = -3 + x_328X;
          if ((address_329X < (Soldspace_beginS))) {
            goto L5590;}
          else {
            if (((Soldspace_endS) < address_329X)) {
              goto L5590;}
            else {
              goto L5631;}}}
        else {
          goto L5590;}}}
    else {
      goto L5590;}}}
 L5519: {
  ps_error("bad descriptor in stack", 1, x_322X);
  goto L5504;}
 L5504: {
  arg1K0 = (addr_321X + 4);
  goto L5499;}
 L16921: {
  v_330X = arg0K0;
  arg1K0 = (cont_311X + (4 + (((v_330X)<<2))));
  goto L18733;}
 L5631: {
  ps_error("bad descriptor in stack", 1, x_328X);
  goto L5590;}
 L5590: {
  arg0K0 = (((mask_326X)>>1));
  arg1K1 = (ptr_327X + 4);
  goto L5574;}
}
long s48_really_add_channel(long mode_331X, long id_332X, long os_index_333X)
{
  long status_335X;
  long channel_334X;
 {s48_make_availableAgc(24);
  channel_334X = make_registered_channel((((mode_331X)>>2)), id_332X, os_index_333X, 0, &status_335X);
  if ((3 == (3 & channel_334X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_334X))) + -4))))>>2))))) {
      return channel_334X;}
    else {
      goto L18804;}}
  else {
    goto L18804;}}
 L18804: {
  return (((status_335X)<<2));}
}
long s48_enter_integer(long x_336X)
{
  char * v_337X;
 {s48_make_availableAgc(16);
  if ((536870911 < x_336X)) {
    goto L20162;}
  else {
    if ((x_336X < -536870912)) {
      goto L20162;}
    else {
      return (((x_336X)<<2));}}}
 L20162: {
  v_337X = (char *) s48_long_to_bignum(x_336X);
  return enter_bignum(v_337X);}
}
void s48_copy_stack_into_heap(void)
{
  char * arg1K1;
  char * arg1K0;
  char * arg_341X;
  char * loc_340X;
  char * top_339X;
  long arg_count_338X;
 {s48_make_availableAgc(((((-4 & ((Sstack_endS) - (SstackS))))<<2)));
  arg_count_338X = ((((ScontS) - (SstackS)))>>2);
  top_339X = SstackS;
  if ((1 == (((long) (ScontS))))) {
    goto L21102;}
  else {really_preserve_continuation(0);
    goto L21102;}}
 L21102: {
  SstackS = (ScontS);
  arg1K0 = ((SstackS) + -4);
  arg1K1 = (top_339X + (-4 + (((arg_count_338X)<<2))));
  goto L21131;}
 L21131: {
  loc_340X = arg1K0;
  arg_341X = arg1K1;
  if ((arg_341X < top_339X)) {
    SstackS = ((SstackS) + (0 - (((arg_count_338X)<<2))));
    return;}
  else {
    *((long *) loc_340X) = (*((long *) arg_341X));
    arg1K0 = (loc_340X + -4);
    arg1K1 = (arg_341X + -4);
    goto L21131;}}
}
long s48_get_imported_binding(char *name_342X)
{
  long value_344X;
  long value_343X;
 {  value_343X = enter_stringAgc(name_342X);
  Stemp0S = value_343X;s48_make_availableAgc(20);
  value_344X = Stemp0S;
  Stemp0S = 1;
  return Hlookup2690((Sexported_bindingsS), value_344X, 0);}
}
void s48_define_exported_binding(char *name_345X, long value_346X)
{
  char * addr_351X;
  long x_350X;
  long value_349X;
  long value_348X;
  long name_347X;
 {  Stemp0S = value_346X;
  name_347X = enter_stringAgc(name_345X);
  Stemp1S = name_347X;s48_make_availableAgc(20);
  value_348X = Stemp0S;
  Stemp0S = 1;
  value_349X = Stemp1S;
  Stemp1S = 1;
  x_350X = Hlookup2671((Simported_bindingsS), value_349X, 0);
  addr_351X = (((char *) (-3 + x_350X))) + 8;S48_WRITE_BARRIER(x_350X, addr_351X, value_348X);
  *((long *) addr_351X) = value_348X;
  return;}
}
void s48_initialize_vm(char * stack_begin_352X, long stack_size_353X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long merged_arg0K3;
  long merged_arg0K2;
  long merged_arg0K1;
  long merged_arg0K0;

  int make_hash_tableAgc_return_tag;
  long make_hash_tableAgc0_return_value;
  int make_return_code_return_tag;
  long make_return_code0_return_value;
  long protocol_354X;
  long opcode_355X;
  long size_356X;
  char * addr_400X;
  long i_399X;
  long table_398X;
  long v_397X;
  long vector_396X;
  char * addr_395X;
  long code_394X;
  char * addr_393X;
  char * addr_392X;
  long x_391X;
  long v_390X;
  long v_389X;
  long vector_388X;
  char * addr_387X;
  long v_386X;
  long v_385X;
  long v_384X;
  long code_383X;
  char * a_382X;
  char * addr_381X;
  long value_380X;
  long size_379X;
  char * start_378X;
  char * addr_377X;
  long val_376X;
  long index_375X;
  long h_374X;
  long i_373X;
  char * stack_372X;
  long n_371X;
  long string_370X;
  long foo_369X;
  long table_368X;
  long i_367X;
  long v_366X;
  long v_365X;
  long imported_bindings_364X;
  long exported_bindings_363X;
  long n_362X;
  long symbols_361X;
  long maybe_360X;
  long maybe_359X;
  long v_358X;
  long symbol_table_357X;
 {  symbol_table_357X = s48_initial_symbols();
  if ((symbol_table_357X == 1)) {
    make_hash_tableAgc_return_tag = 0;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_0:
    v_358X = make_hash_tableAgc0_return_value;
    Sthe_symbol_tableS = v_358X;
    maybe_359X = s48_find_all(1);
    if ((maybe_359X == 1)) {s48_collect();
      maybe_360X = s48_find_all(1);
      if ((maybe_360X == 1)) {
        ps_error("insufficient heap space to build symbol table", 0);
        arg0K0 = maybe_360X;
        goto L21182;}
      else {
        arg0K0 = maybe_360X;
        goto L21182;}}
    else {
      arg0K0 = maybe_359X;
      goto L21182;}}
  else {
    Sthe_symbol_tableS = symbol_table_357X;
    goto L21734;}}
 L21182: {
  symbols_361X = arg0K0;
  n_362X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_361X))) + -4))))>>8))))>>2);
  arg0K0 = 0;
  goto L21203;}
 L21734: {
  exported_bindings_363X = s48_initial_exported_bindings();
  imported_bindings_364X = s48_initial_imported_bindings();
  if ((1 == imported_bindings_364X)) {
    make_hash_tableAgc_return_tag = 1;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_1:
    v_365X = make_hash_tableAgc0_return_value;
    Simported_bindingsS = v_365X;
    make_hash_tableAgc_return_tag = 2;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_2:
    v_366X = make_hash_tableAgc0_return_value;
    Sexported_bindingsS = v_366X;
    goto L21740;}
  else {
    Simported_bindingsS = imported_bindings_364X;
    Sexported_bindingsS = exported_bindings_363X;
    goto L21740;}}
 L21203: {
  i_367X = arg0K0;
  if ((i_367X == n_362X)) {
    goto L21734;}
  else {
    table_368X = Sthe_symbol_tableS;
    foo_369X = *((long *) ((((char *) (-3 + symbols_361X))) + (((i_367X)<<2))));
    string_370X = *((long *) (((char *) (-3 + foo_369X))));
    n_371X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_370X))) + -4))))>>8));
    arg0K0 = 0;
    arg0K1 = 0;
    goto L20556;}}
 L21740: {
  if ((stack_size_353X < 8128)) {
    stack_372X = (char *)malloc(32512);
    if ((stack_372X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_372X;
      arg0K1 = 8128;
      goto L12822;}
    else {
      arg1K0 = stack_372X;
      arg0K1 = 8128;
      goto L12822;}}
  else {
    arg1K0 = stack_begin_352X;
    arg0K1 = stack_size_353X;
    goto L12822;}}
 L20556: {
  i_373X = arg0K0;
  h_374X = arg0K1;
  if ((i_373X < n_371X)) {
    arg0K0 = (1 + i_373X);
    arg0K1 = (h_374X + (((*((unsigned char *) ((((char *) (-3 + string_370X))) + i_373X))))));
    goto L20556;}
  else {
    index_375X = 1023 & h_374X;
    val_376X = *((long *) ((((char *) (-3 + table_368X))) + (((index_375X)<<2))));
    addr_377X = (((char *) (-3 + foo_369X))) + 4;S48_WRITE_BARRIER(foo_369X, addr_377X, val_376X);
    *((long *) addr_377X) = val_376X;
    if ((3 == (3 & foo_369X))) {
      arg0K0 = (-4 & foo_369X);
      goto L20542;}
    else {
      arg0K0 = foo_369X;
      goto L20542;}}}
 L12822: {
  start_378X = arg1K0;
  size_379X = arg0K1;
  Sstack_beginS = start_378X;
  Sstack_endS = (start_378X + (((size_379X)<<2)));
  Sreal_stack_limitS = ((Sstack_beginS) + 512);
  s48_Sstack_limitS = (Sreal_stack_limitS);
  SstackS = (Sstack_endS);
  ScontS = (((char *) 1));
  arg1K0 = start_378X;
  goto L12852;}
 L20542: {
  value_380X = arg0K0;
  addr_381X = (((char *) (-3 + table_368X))) + (((index_375X)<<2));S48_WRITE_BARRIER(table_368X, addr_381X, value_380X);
  *((long *) addr_381X) = value_380X;
  arg0K0 = (1 + i_367X);
  goto L21203;}
 L12852: {
  a_382X = arg1K0;
  if ((a_382X == (Sstack_endS))) {s48_make_availableAgc(20);
    merged_arg0K0 = 71;
    merged_arg0K1 = 0;
    merged_arg0K2 = 0;
    merged_arg0K3 = 0;
    make_return_code_return_tag = 0;
    goto make_return_code;
   make_return_code_return_0:
    code_383X = make_return_code0_return_value;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 1;
    ScontS = (SstackS);
    *((long *) (ScontS)) = (((long) ((((char *) (-3 + code_383X))) + 11)));
    Sbottom_of_stackS = (ScontS);
    Sheap_continuationS = 1;s48_make_availableAgc(60);
    merged_arg0K0 = 66;
    merged_arg0K1 = 150;
    merged_arg0K2 = 65535;
    merged_arg0K3 = 0;
    make_return_code_return_tag = 1;
    goto make_return_code;
   make_return_code_return_1:
    v_384X = make_return_code0_return_value;
    Sinterrupt_return_codeS = v_384X;
    merged_arg0K0 = 1;
    merged_arg0K1 = 147;
    merged_arg0K2 = 65535;
    merged_arg0K3 = 0;
    make_return_code_return_tag = 2;
    goto make_return_code;
   make_return_code_return_2:
    v_385X = make_return_code0_return_value;
    Sexception_return_codeS = v_385X;
    merged_arg0K0 = 70;
    merged_arg0K1 = 0;
    merged_arg0K2 = 1;
    merged_arg0K3 = 0;
    make_return_code_return_tag = 3;
    goto make_return_code;
   make_return_code_return_3:
    v_386X = make_return_code0_return_value;
    Scall_with_values_return_codeS = v_386X;s48_make_availableAgc(32);s48_bignum_make_cached_constants();
    addr_387X = s48_allocate_tracedAgc(8);
    if ((addr_387X == NULL)) {
      arg0K0 = 1;
      goto L21804;}
    else {
      *((long *) addr_387X) = 1034;
      arg0K0 = (3 + (((long) (addr_387X + 4))));
      goto L21804;}}
  else {
    *((long *) a_382X) = 252645135;
    arg1K0 = (a_382X + 4);
    goto L12852;}}
 L21804: {
  vector_388X = arg0K0;
  if ((1 == vector_388X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_389X;
    goto L21791;}
  else {
    arg0K0 = vector_388X;
    goto L21791;}}
 L21791: {
  v_390X = arg0K0;
  Sempty_logS = v_390X;
  x_391X = Sempty_logS;
  addr_392X = ((char *) (-3 + x_391X));S48_WRITE_BARRIER(x_391X, addr_392X, 1);
  *((long *) addr_392X) = 1;
  return;}
 make_return_code: {
  protocol_354X = merged_arg0K0;
  opcode_355X = merged_arg0K1;
  size_356X = merged_arg0K2;{
  addr_393X = s48_allocate_small(18);
  *((long *) addr_393X) = 3658;
  code_394X = 3 + (((long) (addr_393X + 4)));
  *((unsigned char *) (((char *) (-3 + code_394X)))) = 0;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 1)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 2)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 3)) = 23;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 4)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 5)) = 8;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 6)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 7)) = 11;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 8)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 9)) = (255 & (((size_356X)>>8)));
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 10)) = (255 & size_356X);
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 11)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 12)) = protocol_354X;
  *((unsigned char *) ((((char *) (-3 + code_394X))) + 13)) = opcode_355X;
  make_return_code0_return_value = code_394X;
  goto make_return_code_return;}
 make_return_code_return:
  switch (make_return_code_return_tag) {
  case 0: goto make_return_code_return_0;
  case 1: goto make_return_code_return_1;
  case 2: goto make_return_code_return_2;
  default: goto make_return_code_return_3;
  }}

 make_hash_tableAgc: {
{ addr_395X = s48_allocate_tracedAgc(4100);
  if ((addr_395X == NULL)) {
    arg0K0 = 1;
    goto L9440;}
  else {
    *((long *) addr_395X) = 1048586;
    arg0K0 = (3 + (((long) (addr_395X + 4))));
    goto L9440;}}
 L9440: {
  vector_396X = arg0K0;
  if ((1 == vector_396X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_397X;
    goto L9428;}
  else {
    arg0K0 = vector_396X;
    goto L9428;}}
 L9428: {
  table_398X = arg0K0;
  arg0K0 = 0;
  goto L9463;}
 L9463: {
  i_399X = arg0K0;
  if ((1024 == i_399X)) {
    make_hash_tableAgc0_return_value = table_398X;
    goto make_hash_tableAgc_return;}
  else {
    addr_400X = (((char *) (-3 + table_398X))) + (((i_399X)<<2));S48_WRITE_BARRIER(table_398X, addr_400X, 1);
    *((long *) addr_400X) = 1;
    arg0K0 = (1 + i_399X);
    goto L9463;}}
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
void s48_setup_external_exception(long why_401X, long nargs_402X)
{
  long n_404X;
  long code_403X;
 {  code_403X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_403X)))))<<2)), code_403X, (((why_401X)<<2)), 4);
  n_404X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_404X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((why_401X)<<2));
  if ((10 < nargs_402X)) {
    ps_error("too many arguments from external exception", 0);
    goto L22205;}
  else {
    goto L22205;}}
 L22205: {
  Sexternal_exception_nargsS = nargs_402X;
  Sexternal_exceptionPS = 1;
  return;}
}
void s48_gc_root(void)
{

 {  return (Sgc_root_procS)();}
}
long s48_restart(long proc_405X, long nargs_406X)
{
  char *arg3K0;
  char arg2K3;
  char arg2K1;
  char arg2K0;
  char arg2K2;
  char * arg1K1;
  char * arg1K0;
  long arg0K4;
  long arg0K3;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char *merged_arg3K0;
  FILE * merged_arg4K1;
  FILE * merged_arg4K2;
  char merged_arg2K0;
  char merged_arg2K1;
  char * merged_arg1K0;
  long merged_arg0K3;
  long merged_arg0K2;
  long merged_arg0K1;
  long merged_arg0K0;

  int move_args_above_contB_return_tag;
  int get_error_string_return_tag;
  long get_error_string0_return_value;
  int copy_continuation_from_heapB_return_tag;
  char * copy_continuation_from_heapB0_return_value;
  int okay_argument_list_return_tag;
  char okay_argument_list0_return_value;
  long okay_argument_list1_return_value;
  int get_current_port_return_tag;
  long get_current_port0_return_value;
  int make_closure_return_tag;
  long make_closure0_return_value;
  int pending_interruptP_return_tag;
  char pending_interruptP0_return_value;
  int proposal_d_read_return_tag;
  long proposal_d_read0_return_value;
  int proposal_d_write_return_tag;
  int okay_copy_argsP_return_tag;
  char okay_copy_argsP0_return_value;
  int pop_continuationB_return_tag;
  int shift_space_return_tag;
  long shift_space0_return_value;
  int copy_listSAgc_return_tag;
  long copy_listSAgc0_return_value;
  int pop_args_GlistSAgc_return_tag;
  long pop_args_GlistSAgc0_return_value;
  int ensure_stack_spaceB_return_tag;
  char ensure_stack_spaceB0_return_value;
  int push_list_return_tag;
  long push_list0_return_value;
  int rest_list_setupAgc_return_tag;
  int find_template_return_tag;
  long find_template0_return_value;
  int maybe_write_template_return_tag;
  char maybe_write_template0_return_value;
  int really_show_stack_continuation_uids_return_tag;
  long really_show_stack_continuation_uids0_return_value;
  int loseD0_return_tag;
  long nargs_407X;
  long status_408X;
  long cont_409X;
  long stack_arg_count_410X;
  long list_411X;
  long marker_412X;
  long a_413X;
  long b_414X;
  long stob_415X;
  long index_416X;
  long stob_417X;
  long index_418X;
  long value_419X;
  long thing_420X;
  long index_421X;
  long count_422X;
  long x_423X;
  long n_424X;
  long list_425X;
  long length_426X;
  long start_427X;
  long count_428X;
  long need_429X;
  long list_430X;
  long count_431X;
  long wants_stack_args_432X;
  long stack_arg_count_433X;
  long list_args_434X;
  long list_arg_count_435X;
  char * start_436X;
  long count_437X;
  long code_vector_438X;
  long template_439X;
  char not_firstP_440X;
  FILE * out_441X;
  char not_firstP_442X;
  FILE * out_443X;
  char *message_444X;
  char * arg_2093X;
  char * loc_2092X;
  char * top_of_args_2091X;
  long i_2090X;
  long string_2089X;
  char * addr_2088X;
  long len_2087X;
  long len_2086X;
  long x_2085X;
  char *string_2084X;
  char * new_stack_2083X;
  char * new_cont_2082X;
  long stack_size_2081X;
  char move_slowP_2080X;
  long slow_2079X;
  long len_2078X;
  long fast_2077X;
  long v_2076X;
  char *v_2075X;
  long v_2074X;
  long v_2073X;
  long obj_2072X;
  long env_2071X;
  long thread_2070X;
  long x_2069X;
  char * addr_2068X;
  long v_2067X;
  long status_2066X;
  long channel_2065X;
  long type_2064X;
  long v_2063X;
  long next_stob_2062X;
  long i_2061X;
  long log_2060X;
  char * addr_2059X;
  long next_stob_2058X;
  long i_2057X;
  long log_2056X;
  long v_2055X;
  long v_2054X;
  long size_2053X;
  char * pointer_2052X;
  char * cont_2051X;
  long v_2050X;
  long v_2049X;
  long n_2048X;
  long extra_2047X;
  long x_size_2046X;
  char * addr_2045X;
  long x_2044X;
  char * addr_2043X;
  long a_2042X;
  long last_2041X;
  long l_2040X;
  long x_2039X;
  char * addr_2038X;
  long a_2037X;
  long value_2036X;
  long x_2035X;
  char * addr_2034X;
  long a_2033X;
  long count_2032X;
  long args_2031X;
  long value_2030X;
  char interruptP_2029X;
  long x_2028X;
  long l_2027X;
  long i_2026X;
  long list_2025X;
  long x_2024X;
  long v_2023X;
  long x_2022X;
  long v_2021X;
  long count_2020X;
  long x_2019X;
  long next_2018X;
  long i_2017X;
  long obj_2016X;
  long obj_2015X;
  long name_2014X;
  long v_2013X;
  long size_2012X;
  char * pointer_2011X;
  char v_2010X;
  long v_2009X;
  long v_2008X;
  char v_2007X;
  long v_2006X;
  char not_firstP_2005X;
  long cont_2004X;
  long size_2003X;
  char * pointer_2002X;
  long v_2001X;
  char * pointer_2000X;
  char * code_pointer_1999X;
  long cont_1998X;
  char not_firstP_1997X;
  char * cont_1996X;
  long why_1995X;
  char not_firstP_1994X;
  long template_1993X;
  FILE * out_1992X;
  long current_code_1991X;
  long val_1990X;
  long val_1989X;
  long val_1988X;
  long val_1987X;
  long val_1986X;
  long val_1985X;
  long n_1984X;
  char v_1983X;
  char * external_bignum_1982X;
  char * x_1981X;
  long val_1980X;
  long n_1979X;
  char v_1978X;
  char * external_bignum_1977X;
  char * x_1976X;
  long val_1975X;
  long n_1974X;
  char v_1973X;
  char * external_bignum_1972X;
  char * x_1971X;
  long v_1970X;
  long n_1969X;
  char v_1968X;
  long v_1967X;
  long rem_1966X;
  long quot_1965X;
  char div_by_zeroP_1964X;
  long val_1963X;
  long n_1962X;
  char v_1961X;
  char * external_bignum_1960X;
  char * x_1959X;
  long val_1958X;
  long n_1957X;
  char v_1956X;
  char * external_bignum_1955X;
  char * x_1954X;
  long val_1953X;
  long n_1952X;
  char v_1951X;
  char * external_bignum_1950X;
  char * x_1949X;
  long link_1948X;
  long v_1947X;
  long v_1946X;
  long stob_1945X;
  long v_1944X;
  long stob_1943X;
  char * addr_1942X;
  long x_1941X;
  char * addr_1940X;
  char * addr_1939X;
  long value_1938X;
  char * addr_1937X;
  long value_1936X;
  long copies_1935X;
  char * v_1934X;
  long value_1933X;
  char * y_1932X;
  char * v_1931X;
  long value_1930X;
  char * y_1929X;
  char * v_1928X;
  long value_1927X;
  char * y_1926X;
  long val_1925X;
  long v_1924X;
  long n_1923X;
  char v_1922X;
  char * rem_1921X;
  char * quot_1920X;
  char div_by_zeroP_1919X;
  char * x_1918X;
  char * v_1917X;
  long value_1916X;
  char * y_1915X;
  char * v_1914X;
  long value_1913X;
  char * y_1912X;
  char * v_1911X;
  long value_1910X;
  char * y_1909X;
  char * addr_1908X;
  long val_1907X;
  char * addr_1906X;
  long value_1905X;
  long len_1904X;
  long s2_1903X;
  long foo_1902X;
  long previous_foo_1901X;
  long value_1900X;
  long verify_1899X;
  long copies_1898X;
  long stob_1897X;
  long i_1896X;
  long x_1895X;
  long x_1894X;
  long val_1893X;
  char * addr_1892X;
  long value_1891X;
  char * v_1890X;
  long value_1889X;
  long v_1888X;
  char * v_1887X;
  long value_1886X;
  long v_1885X;
  char * v_1884X;
  long value_1883X;
  long v_1882X;
  long val_1881X;
  long val_1880X;
  long n_1879X;
  char v_1878X;
  char * external_bignum_1877X;
  char * x_1876X;
  long val_1875X;
  long n_1874X;
  char v_1873X;
  char * external_bignum_1872X;
  char * x_1871X;
  char * v_1870X;
  long value_1869X;
  char * y_1868X;
  char * v_1867X;
  long value_1866X;
  long v_1865X;
  long val_1864X;
  long v_1863X;
  char * v_1862X;
  char * v_1861X;
  long value_1860X;
  long extra1_1859X;
  long length1_1858X;
  char * v_1857X;
  long value_1856X;
  long v_1855X;
  long bucket_1854X;
  char * addr_1853X;
  long value_1852X;
  long verify_1851X;
  long log_1850X;
  long stob_1849X;
  long i_1848X;
  long x_1847X;
  long n_1846X;
  long code_1845X;
  long status_1844X;
  char pendingP_1843X;
  char eofP_1842X;
  long got_1841X;
  long y_1840X;
  long link_1839X;
  long val_1838X;
  long x_1837X;
  char * addr_1836X;
  long b_1835X;
  long val_1834X;
  long extra1_1833X;
  long length1_1832X;
  long extra1_1831X;
  long length1_1830X;
  long extra1_1829X;
  long length1_1828X;
  long n_1827X;
  char * v_1826X;
  long val_1825X;
  long n_1824X;
  char v_1823X;
  char * external_bignum_1822X;
  char * v_1821X;
  long n_1820X;
  char v_1819X;
  char * external_bignum_1818X;
  char * v_1817X;
  long value_1816X;
  char * y_1815X;
  char * v_1814X;
  long value_1813X;
  char * y_1812X;
  char * v_1811X;
  long value_1810X;
  long extra1_1809X;
  long val_1808X;
  long v_1807X;
  char * v_1806X;
  long extra1_1805X;
  long length1_1804X;
  long val_1803X;
  long v_1802X;
  char * v_1801X;
  long x_1800X;
  long val_1799X;
  long v_1798X;
  char * v_1797X;
  long extra0_1796X;
  long length0_1795X;
  long extra1_1794X;
  long length1_1793X;
  char * addr_1792X;
  long value_1791X;
  long offset_1790X;
  long i_1789X;
  long count_1788X;
  char * addr_1787X;
  long value_1786X;
  long offset_1785X;
  long i_1784X;
  long count_1783X;
  char *v_1782X;
  long link_1781X;
  long index_1780X;
  long h_1779X;
  long i_1778X;
  char * addr_1777X;
  char * addr_1776X;
  long value_1775X;
  char * addr_1774X;
  char * addr_1773X;
  char * addr_1772X;
  long value_1771X;
  char * addr_1770X;
  char * addr_1769X;
  long value_1768X;
  long proposal_1767X;
  long entry_1766X;
  long thing_1765X;
  long log_1764X;
  long copies_1763X;
  char * addr_1762X;
  long x_1761X;
  long val_1760X;
  long n_1759X;
  long code_1758X;
  long status_1757X;
  long v_1756X;
  long len_1755X;
  long s2_1754X;
  long foo_1753X;
  long val_1752X;
  long v_1751X;
  long n_1750X;
  char v_1749X;
  char * external_bignum_1748X;
  char * x_1747X;
  long extra0_1746X;
  long length0_1745X;
  long extra0_1744X;
  long length0_1743X;
  long extra0_1742X;
  long length0_1741X;
  char * v_1740X;
  long value_1739X;
  long v_1738X;
  char * v_1737X;
  long value_1736X;
  long v_1735X;
  char * v_1734X;
  long v_1733X;
  char * x_1732X;
  char * v_1731X;
  long n_1730X;
  char * v_1729X;
  long value_1728X;
  long extra1_1727X;
  char * v_1726X;
  long value_1725X;
  long extra1_1724X;
  long val_1723X;
  long val_1722X;
  long val_1721X;
  long val_1720X;
  long val_1719X;
  long extra0_1718X;
  long length0_1717X;
  long val_1716X;
  long v_1715X;
  char * v_1714X;
  long x_1713X;
  long val_1712X;
  long v_1711X;
  char * v_1710X;
  long extra0_1709X;
  long length0_1708X;
  long y_1707X;
  long x_1706X;
  long extra0_1705X;
  long length0_1704X;
  long x_1703X;
  long n_1702X;
  long code_1701X;
  long bytes_used_1700X;
  long count_1699X;
  long index_1698X;
  long env_1697X;
  long offset_1696X;
  long i_1695X;
  long bytes_used_1694X;
  long count_1693X;
  long env_1692X;
  long offset_1691X;
  long i_1690X;
  long n_1689X;
  long x_1688X;
  long obj_1687X;
  long n_1686X;
  long code_1685X;
  long n_1684X;
  long code_1683X;
  long n_1682X;
  long code_1681X;
  long n_1680X;
  long code_1679X;
  long i_1678X;
  long l_1677X;
  long n_1676X;
  long table_1675X;
  long val_1674X;
  long n_1673X;
  long code_1672X;
  char x_1671X;
  char minutesP_1670X;
  long v_1669X;
  long vector_1668X;
  char * addr_1667X;
  long x_1666X;
  long value_1665X;
  long verify_1664X;
  long copies_1663X;
  long stob_1662X;
  long i_1661X;
  char * addr_1660X;
  long v_1659X;
  char * addr_1658X;
  char * addr_1657X;
  long val_1656X;
  long x_1655X;
  long n_1654X;
  long code_1653X;
  long status_1652X;
  char pendingP_1651X;
  long got_1650X;
  long y_1649X;
  char v_1648X;
  long count_1647X;
  long start_1646X;
  char waitP_1645X;
  long x_1644X;
  long n_1643X;
  long code_1642X;
  long status_1641X;
  long channel_1640X;
  long v_1639X;
  long v_1638X;
  long v_1637X;
  long reason_1636X;
  long channel_1635X;
  long index_1634X;
  long bucket_1633X;
  long i_1632X;
  long i_1631X;
  char * addr_1630X;
  long i_1629X;
  long rest_list_1628X;
  long i_1627X;
  long v_1626X;
  long n_1625X;
  char v_1624X;
  char * external_bignum_1623X;
  char * x_1622X;
  long val_1621X;
  long v_1620X;
  char * v_1619X;
  char * v_1618X;
  long value_1617X;
  long needed_1616X;
  long y_1615X;
  long y_1614X;
  long x_1613X;
  long val_1612X;
  long v_1611X;
  char * v_1610X;
  long val_1609X;
  long v_1608X;
  char * v_1607X;
  long extra_1606X;
  long length_1605X;
  long count_1604X;
  long x_1603X;
  long extra_1602X;
  long length_1601X;
  char * v_1600X;
  long value_1599X;
  long extra_1598X;
  long length_1597X;
  long x_1596X;
  long c_1595X;
  long b_1594X;
  long extra0_1593X;
  long length0_1592X;
  long c_1591X;
  long b_1590X;
  long extra0_1589X;
  long length0_1588X;
  long n_1587X;
  long code_1586X;
  long val_1585X;
  long v_1584X;
  long v_1583X;
  long v_1582X;
  long n_1581X;
  long code_1580X;
  long val_1579X;
  long v_1578X;
  long v_1577X;
  long v_1576X;
  long n_1575X;
  long code_1574X;
  long val_1573X;
  long v_1572X;
  long v_1571X;
  long v_1570X;
  long n_1569X;
  long code_1568X;
  long val_1567X;
  long v_1566X;
  long v_1565X;
  long v_1564X;
  long n_1563X;
  long code_1562X;
  long val_1561X;
  char b_1560X;
  long result_1559X;
  char x_1558X;
  long c_1557X;
  long b_1556X;
  long n_1555X;
  long code_1554X;
  long c_1553X;
  long mid_c_1552X;
  long v_1551X;
  long v_1550X;
  long lo_c_1549X;
  long hi_b_1548X;
  long hi_a_1547X;
  long lo_b_1546X;
  long lo_a_1545X;
  long b_1544X;
  long n_1543X;
  long code_1542X;
  long n_1541X;
  long code_1540X;
  long x_1539X;
  long v_1538X;
  long cont_1537X;
  char * addr_1536X;
  long value_1535X;
  long offset_1534X;
  long i_1533X;
  long count_1532X;
  char * addr_1531X;
  long value_1530X;
  long offset_1529X;
  long i_1528X;
  long count_1527X;
  char x_1526X;
  long channel_1525X;
  long handlers_1524X;
  long arg_count_1523X;
  char * arg_1522X;
  char * loc_1521X;
  long x_1520X;
  long l_1519X;
  long stack_nargs_1518X;
  long x_1517X;
  long v_1516X;
  long v_1515X;
  long v_1514X;
  long v_1513X;
  long obj_1512X;
  long type_1511X;
  long thing_1510X;
  long stuff_1509X;
  char * addr_1508X;
  long val_1507X;
  long n_1506X;
  long code_1505X;
  long i_1504X;
  long n_1503X;
  long code_1502X;
  long b_1501X;
  long port_1500X;
  long Kchar_1499X;
  long n_1498X;
  long code_1497X;
  long i_1496X;
  long p_1495X;
  long n_1494X;
  long code_1493X;
  long b_1492X;
  long port_1491X;
  char * addr_1490X;
  long val_1489X;
  long n_1488X;
  long code_1487X;
  long i_1486X;
  long p_1485X;
  long n_1484X;
  long code_1483X;
  long b_1482X;
  long port_1481X;
  long n_1480X;
  long code_1479X;
  char * addr_1478X;
  long n_1477X;
  long code_1476X;
  long n_1475X;
  long code_1474X;
  long value_1473X;
  long n_1472X;
  long code_1471X;
  long list_1470X;
  long head_1469X;
  char move_slowP_1468X;
  long slow_1467X;
  long list_1466X;
  long n_1465X;
  long code_1464X;
  long string_1463X;
  char * addr_1462X;
  long len_1461X;
  long x_1460X;
  long n_1459X;
  long code_1458X;
  long val_1457X;
  long n_1456X;
  long code_1455X;
  long mseconds_1454X;
  long seconds_1453X;
  long option_1452X;
  long x_1451X;
  long n_1450X;
  long code_1449X;
  long x_1448X;
  long n_1447X;
  long code_1446X;
  long v_1445X;
  long v_1444X;
  long x_1443X;
  long result_1442X;
  char * args_1441X;
  long name_1440X;
  long proc_1439X;
  long rest_list_1438X;
  long x_1437X;
  long n_1436X;
  long code_1435X;
  long x_1434X;
  long v_1433X;
  long code_1432X;
  long pc_1431X;
  char * addr_1430X;
  long x_1429X;
  long proposal_1428X;
  long x_1427X;
  long n_1426X;
  long code_1425X;
  long x_1424X;
  long n_1423X;
  long code_1422X;
  long n_1421X;
  long code_1420X;
  long value_1419X;
  long vector_1418X;
  long type_1417X;
  char firstP_1416X;
  long n_1415X;
  long code_1414X;
  long vector_1413X;
  char firstP_1412X;
  long n_1411X;
  long code_1410X;
  long x_1409X;
  long n_1408X;
  long code_1407X;
  long h_1406X;
  long i_1405X;
  long x_1404X;
  long n_1403X;
  long code_1402X;
  long x_1401X;
  long n_1400X;
  long code_1399X;
  long status_1398X;
  long reason_1397X;
  long x_1396X;
  long n_1395X;
  long code_1394X;
  long n_1393X;
  long code_1392X;
  char * addr_1391X;
  long next_stob_1390X;
  long i_1389X;
  long n_1388X;
  long code_1387X;
  long x_1386X;
  long n_1385X;
  long code_1384X;
  long v_1383X;
  long next_stob_1382X;
  long i_1381X;
  long value_1380X;
  long n_1379X;
  long code_1378X;
  char * addr_1377X;
  long count_1376X;
  long to_index_1375X;
  long from_index_1374X;
  long copies_1373X;
  long left_1372X;
  long n_1371X;
  long code_1370X;
  long n_1369X;
  long code_1368X;
  long value_1367X;
  long value_1366X;
  long verify_1365X;
  long log_1364X;
  long stob_1363X;
  long i_1362X;
  long n_1361X;
  long code_1360X;
  char * addr_1359X;
  long old_1358X;
  long x_1357X;
  char * addr_1356X;
  long channel_1355X;
  long res_1354X;
  long i_1353X;
  long x_1352X;
  long n_1351X;
  long code_1350X;
  long y_1349X;
  long n_1348X;
  char * addr_1347X;
  long prev_1346X;
  long ch_1345X;
  long val_1344X;
  long x_1343X;
  long n_1342X;
  long code_1341X;
  long val_1340X;
  long x_1339X;
  long n_1338X;
  long code_1337X;
  long n_1336X;
  long code_1335X;
  long x_1334X;
  long n_1333X;
  long code_1332X;
  long x_1331X;
  long n_1330X;
  long code_1329X;
  long n_1328X;
  long code_1327X;
  long reason_1326X;
  long v_1325X;
  long v_1324X;
  long val_1323X;
  long x_1322X;
  long n_1321X;
  long code_1320X;
  char * addr_1319X;
  char * addr_1318X;
  long x_1317X;
  long n_1316X;
  long code_1315X;
  long val_1314X;
  long x_1313X;
  long n_1312X;
  long code_1311X;
  long link_1310X;
  long index_1309X;
  long h_1308X;
  long i_1307X;
  long x_1306X;
  long n_1305X;
  long code_1304X;
  long n_1303X;
  long code_1302X;
  long n_1301X;
  long code_1300X;
  long x_1299X;
  long n_1298X;
  long code_1297X;
  long n_1296X;
  long code_1295X;
  long x_1294X;
  long n_1293X;
  long code_1292X;
  long x_1291X;
  long n_1290X;
  long code_1289X;
  long n_1288X;
  long code_1287X;
  long vector_1286X;
  long n_1285X;
  long code_1284X;
  long x_1283X;
  long n_1282X;
  long code_1281X;
  long n_1280X;
  long code_1279X;
  long n_1278X;
  long code_1277X;
  long x_1276X;
  long n_1275X;
  long code_1274X;
  long n_1273X;
  long code_1272X;
  long x_1271X;
  long n_1270X;
  long code_1269X;
  long n_1268X;
  long code_1267X;
  long vector_1266X;
  long n_1265X;
  long code_1264X;
  long n_1263X;
  long code_1262X;
  char * addr_1261X;
  long n_1260X;
  long code_1259X;
  long n_1258X;
  long code_1257X;
  long value_1256X;
  long n_1255X;
  long code_1254X;
  long n_1253X;
  long code_1252X;
  long value_1251X;
  long x_1250X;
  long n_1249X;
  long code_1248X;
  long n_1247X;
  long code_1246X;
  char * addr_1245X;
  long n_1244X;
  long code_1243X;
  long value_1242X;
  long i_1241X;
  long value_1240X;
  long i_1239X;
  long n_1238X;
  long code_1237X;
  long value_1236X;
  long val_1235X;
  long n_1234X;
  long code_1233X;
  long x_1232X;
  long n_1231X;
  long code_1230X;
  long val_1229X;
  long x_1228X;
  long n_1227X;
  long code_1226X;
  long val_1225X;
  long val_1224X;
  char * v_1223X;
  long value_1222X;
  long needed_1221X;
  long y_1220X;
  long x_1219X;
  long result_1218X;
  long x_1217X;
  long x_1216X;
  long x_1215X;
  long count_1214X;
  long value_1213X;
  long n_1212X;
  long code_1211X;
  long val_1210X;
  long val_1209X;
  long n_1208X;
  long code_1207X;
  long n_1206X;
  long code_1205X;
  long n_1204X;
  long code_1203X;
  long n_1202X;
  long code_1201X;
  long x_1200X;
  long n_1199X;
  long code_1198X;
  long x_1197X;
  long n_1196X;
  long code_1195X;
  long x_1194X;
  long n_1193X;
  long code_1192X;
  long n_1191X;
  long code_1190X;
  long n_1189X;
  long code_1188X;
  long n_1187X;
  long code_1186X;
  long n_1185X;
  long code_1184X;
  long n_1183X;
  long a_1182X;
  long y_1181X;
  long x_1180X;
  long a_1179X;
  long y_1178X;
  long x_1177X;
  long val_1176X;
  long val_1175X;
  char b_1174X;
  long val_1173X;
  char b_1172X;
  long val_1171X;
  char b_1170X;
  long val_1169X;
  long Kdouble_1168X;
  char * addr_1167X;
  double value_1166X;
  long value_1165X;
  double y_1164X;
  long value_1163X;
  long a_1162X;
  long n_1161X;
  long code_1160X;
  long y_1159X;
  long x_1158X;
  long Kdouble_1157X;
  char * addr_1156X;
  double x_1155X;
  long value_1154X;
  double y_1153X;
  long value_1152X;
  long val_1151X;
  long v_1150X;
  char * v_1149X;
  long Kdouble_1148X;
  char * addr_1147X;
  double x_1146X;
  long value_1145X;
  double y_1144X;
  long value_1143X;
  long a_1142X;
  long Kdouble_1141X;
  char * addr_1140X;
  double x_1139X;
  long value_1138X;
  double y_1137X;
  long value_1136X;
  long val_1135X;
  long v_1134X;
  char * v_1133X;
  long n_1132X;
  long code_1131X;
  long x_1130X;
  long n_1129X;
  long code_1128X;
  long val_1127X;
  long val_1126X;
  long delta_1125X;
  long delta_1124X;
  long offset_1123X;
  long index_1122X;
  long v_1121X;
  long x_1120X;
  long x_1119X;
  long x_1118X;
  long x_1117X;
  char * addr_1116X;
  long a_1115X;
  long wants_stack_args_1114X;
  long size_1113X;
  char * pointer_1112X;
  char * cont_1111X;
  long proc_1110X;
  long offset_1109X;
  long cont_1108X;
  long protocol_1107X;
  char * code_pointer_1106X;
  long obj_1105X;
  char * addr_1104X;
  long list_args_1103X;
  long follower_1102X;
  long list_1101X;
  long n_1100X;
  long code_1099X;
  long proc_1098X;
  long args_1097X;
  long list_arg_count_1096X;
  char okayP_1095X;
  long stack_nargs_1094X;
  long list_args_1093X;
  long obj_1092X;
  long v_1091X;
  long proc_1090X;
  long protocol_1089X;
  long code_1088X;
  long template_1087X;
  long obj_1086X;
  long stack_arg_count_1085X;
  long value_1084X;
  char * addr_1083X;
  long value_1082X;
  long offset_1081X;
  long i_1080X;
  long count_1079X;
  long total_count_1078X;
  long offset_1077X;
  long i_1076X;
  long new_env_1075X;
  char * addr_1074X;
  long value_1073X;
  long offset_1072X;
  long i_1071X;
  long count_1070X;
  long total_count_1069X;
  long offset_1068X;
  long i_1067X;
  long new_env_1066X;
  long n_1065X;
  long n_1064X;
  long x_1063X;
  long x_1062X;
  char * addr_1061X;
  long next_1060X;
  long channel_1059X;
  long n_1058X;
  long x_1057X;
  long n_1056X;
  long x_1055X;
  long m_1054X;
  long i_1053X;
  char * arg_top_1052X;
  long list_arg_count_1051X;
  long list_args_1050X;
  long stack_nargs_1049X;
  long bytes_used_1048X;
  long count_1047X;
  long v_1046X;
  char * arg_1045X;
  char * loc_1044X;
  long obj_1043X;
  long obj_1042X;
  long list_arg_count_1041X;
  long list_args_1040X;
  long stack_arg_count_1039X;
  long v_1038X;
  long v_1037X;
  long size_1036X;
  char * pointer_1035X;
  char * cont_1034X;
  long protocol_skip_1033X;
  long v_1032X;
  long bytes_used_1031X;
  long n_1030X;
  long code_1029X;
  long args_1028X;
  char not_firstP_1027X;
  long template_1026X;
  FILE * out_1025X;
  long current_code_1024X;
  long list_args_1023X;
  long stack_nargs_1022X;
  long v_1021X;
  FILE * out_1020X;
  long x_1019X;
  long v_1018X;
  long v_1017X;
  long v_1016X;
  long v_1015X;
  long v_1014X;
  long v_1013X;
  long v_1012X;
  long v_1011X;
  long x_1010X;
  long n_1009X;
  long code_1008X;
  long v_1007X;
  long len_1006X;
  long value_1005X;
  long index_1004X;
  long arg4_1003X;
  long arg3_1002X;
  long arg2_1001X;
  long x_1000X;
  long n_999X;
  long code_998X;
  long v_997X;
  long v_996X;
  long len_995X;
  long index_994X;
  long arg3_993X;
  long arg2_992X;
  long n_991X;
  long code_990X;
  long list_989X;
  long arg2_988X;
  long x_987X;
  long n_986X;
  long code_985X;
  long n_984X;
  long arg2_983X;
  long len_982X;
  long x_981X;
  long obj_980X;
  long arg2_979X;
  long x_978X;
  long arg2_977X;
  long x_976X;
  long n_975X;
  long code_974X;
  long n_973X;
  long code_972X;
  long n_971X;
  long code_970X;
  long status_969X;
  long value_968X;
  long key_967X;
  long arg2_966X;
  long x_965X;
  long n_964X;
  long code_963X;
  long n_962X;
  long code_961X;
  long mseconds_960X;
  long seconds_959X;
  long mseconds_958X;
  long seconds_957X;
  long x_956X;
  long other_955X;
  long option_954X;
  long arg2_953X;
  long x_952X;
  long arg2_951X;
  long x_950X;
  long arg2_949X;
  long x_948X;
  long n_947X;
  long code_946X;
  long rest_list_945X;
  long p_944X;
  long nargs_943X;
  long p_942X;
  long x_941X;
  long arg2_940X;
  long x_939X;
  long n_938X;
  long code_937X;
  long p_936X;
  long p_935X;
  long x_934X;
  long p_933X;
  long old_932X;
  long temp_931X;
  long obj_930X;
  long size_929X;
  long code_928X;
  long pc_927X;
  long data_926X;
  long temp_925X;
  long obj_924X;
  long val_923X;
  long type_922X;
  long x_921X;
  long n_920X;
  long code_919X;
  long type_918X;
  long x_917X;
  long n_916X;
  long code_915X;
  long n_914X;
  long code_913X;
  long n_912X;
  long code_911X;
  long x_910X;
  long bytes_909X;
  long x_908X;
  long other_907X;
  long key_906X;
  long arg2_905X;
  long x_904X;
  char * addr_903X;
  long b_902X;
  long x_901X;
  char * addr_900X;
  long proc_899X;
  long arg2_898X;
  long n_897X;
  long x_896X;
  long obj_895X;
  long status_894X;
  long status_893X;
  long status_892X;
  long status_891X;
  long status_890X;
  long status_889X;
  FILE * port_888X;
  long undumpables_887X;
  long obj_886X;
  long arg4_885X;
  long arg3_884X;
  long arg2_883X;
  long n_882X;
  long code_881X;
  long arg_880X;
  long log_879X;
  long index_878X;
  long x_877X;
  long len_876X;
  long byte_875X;
  long index_874X;
  long arg3_873X;
  long arg2_872X;
  long log_871X;
  long index_870X;
  long x_869X;
  long len_868X;
  long index_867X;
  long arg2_866X;
  long x_865X;
  long n_864X;
  long code_863X;
  char v_862X;
  char v_861X;
  long v_860X;
  long count_859X;
  long to_index_858X;
  long from_index_857X;
  long arg5_856X;
  long arg4_855X;
  long arg3_854X;
  long arg2_853X;
  long v_852X;
  long x_851X;
  long type_850X;
  long offset_849X;
  long stob_848X;
  long log_847X;
  long n_846X;
  long code_845X;
  long proposal_844X;
  long proposal_843X;
  char * addr_842X;
  long next_841X;
  long channel_840X;
  long n_839X;
  char * addr_838X;
  long head_837X;
  long channel_836X;
  long obj_835X;
  long x_834X;
  long n_833X;
  long code_832X;
  long status_831X;
  char readyP_830X;
  long n_829X;
  long code_828X;
  long channel_827X;
  long obj_826X;
  char v_825X;
  long count_824X;
  long start_823X;
  long arg4_822X;
  long arg3_821X;
  long arg2_820X;
  long x_819X;
  long arg5_818X;
  long arg4_817X;
  long arg3_816X;
  long arg2_815X;
  long x_814X;
  long n_813X;
  long code_812X;
  long status_811X;
  long n_810X;
  long code_809X;
  long channel_808X;
  long obj_807X;
  long x_806X;
  long n_805X;
  long code_804X;
  long mode_803X;
  long arg2_802X;
  long x_801X;
  long x_800X;
  long x_799X;
  long arg2_798X;
  long descriptor_797X;
  long x_796X;
  long obj_795X;
  long n_794X;
  long string_793X;
  long table_792X;
  long obj_791X;
  long len_790X;
  char Kchar_789X;
  long index_788X;
  long arg3_787X;
  long arg2_786X;
  long len_785X;
  long index_784X;
  long arg2_783X;
  long obj_782X;
  char * addr_781X;
  long len_780X;
  char init_779X;
  long len_778X;
  long arg2_777X;
  long len_776X;
  long Kchar_775X;
  long index_774X;
  long arg3_773X;
  long arg2_772X;
  long len_771X;
  long index_770X;
  long arg2_769X;
  long obj_768X;
  long x_767X;
  long n_766X;
  long code_765X;
  char * addr_764X;
  long init_763X;
  long len_762X;
  long arg2_761X;
  long v_760X;
  long index_759X;
  long len_758X;
  long type_757X;
  long value_756X;
  long arg3_755X;
  long arg2_754X;
  long v_753X;
  long v_752X;
  long index_751X;
  long len_750X;
  long type_749X;
  long index_748X;
  long arg2_747X;
  long n_746X;
  long code_745X;
  char * addr_744X;
  long len_743X;
  long len_742X;
  long type_741X;
  long init_740X;
  long arg2_739X;
  long v_738X;
  long type_737X;
  long offset_736X;
  long value_735X;
  long arg2_734X;
  long type_733X;
  long offset_732X;
  long stob_731X;
  long rest_list_730X;
  long stack_nargs_729X;
  long p_728X;
  long new_727X;
  char * addr_726X;
  long len_725X;
  long type_724X;
  long len_723X;
  long p_722X;
  long new_721X;
  char * addr_720X;
  long len_719X;
  long type_718X;
  long len_717X;
  long type_716X;
  long stob_715X;
  long type_714X;
  long x_713X;
  long x_712X;
  long x_711X;
  long n_710X;
  long code_709X;
  long x_708X;
  long x_707X;
  long n_706X;
  long code_705X;
  long x_704X;
  long arg2_703X;
  long x_702X;
  long arg2_701X;
  long x_700X;
  long v_699X;
  long n_698X;
  long code_697X;
  long v_696X;
  long y_695X;
  long arg2_694X;
  long y_693X;
  long arg2_692X;
  long y_691X;
  long arg2_690X;
  long y_689X;
  long arg2_688X;
  long x_687X;
  long x_686X;
  long x_685X;
  long arg2_684X;
  long arg2_683X;
  long arg2_682X;
  long x_681X;
  long v_680X;
  long x_679X;
  long n_678X;
  long n_677X;
  long n_676X;
  long n_675X;
  long n_674X;
  long a_673X;
  long y_672X;
  long arg2_671X;
  long b_670X;
  long a_669X;
  long y_668X;
  long arg2_667X;
  long y_666X;
  long arg2_665X;
  long y_664X;
  long arg2_663X;
  long y_662X;
  long arg2_661X;
  long y_660X;
  long arg2_659X;
  long y_658X;
  long arg2_657X;
  long b_656X;
  long a_655X;
  long y_654X;
  long arg2_653X;
  long x_652X;
  long y_651X;
  long arg2_650X;
  long b_649X;
  long a_648X;
  long y_647X;
  long arg2_646X;
  long x_645X;
  long y_644X;
  long arg2_643X;
  long n_642X;
  long n_641X;
  long n_640X;
  long n_639X;
  long n_638X;
  long x_637X;
  long x_636X;
  long arg2_635X;
  long rest_list_634X;
  long x_633X;
  long rest_list_632X;
  long stack_nargs_631X;
  long arg0_630X;
  long arg1_629X;
  long rest_list_628X;
  long arg0_627X;
  long stack_nargs_626X;
  long x_625X;
  long n_624X;
  long code_623X;
  long index_622X;
  long val_621X;
  long max_620X;
  long p_619X;
  char * code_pointer_618X;
  long return_pointer_offset_617X;
  long nargs_616X;
  long code_615X;
  long template_614X;
  long rest_list_613X;
  long stack_nargs_612X;
  long p_611X;
  long p_610X;
  long cont_609X;
  long v_608X;
  long rest_list_607X;
  long proc_606X;
  long stack_nargs_605X;
  long p_604X;
  long p_603X;
  long x_602X;
  long n_601X;
  long code_600X;
  long args_599X;
  char * code_pointer_598X;
  long return_pointer_offset_597X;
  long stack_nargs_596X;
  long length_595X;
  char okayP_594X;
  long list_args_593X;
  char * code_pointer_592X;
  long return_pointer_offset_591X;
  long stack_arg_count_590X;
  char * code_pointer_589X;
  long stack_arg_count_588X;
  long v_587X;
  long x_586X;
  long x_585X;
  long x_584X;
  long x_583X;
  long value_582X;
  long x_581X;
  long template_580X;
  long new_env_579X;
  char * addr_578X;
  long len_577X;
  long closures_576X;
  long total_count_575X;
  long template_574X;
  long new_env_573X;
  char * addr_572X;
  long len_571X;
  long closures_570X;
  long total_count_569X;
  char * addr_568X;
  long val_567X;
  long x_566X;
  long n_565X;
  long code_564X;
  long location_563X;
  long index_562X;
  long template_561X;
  long n_560X;
  long code_559X;
  long location_558X;
  long index_557X;
  long template_556X;
  long x_555X;
  long x_554X;
  long n_553X;
  long n_552X;
  long code_551X;
  char * code_pointer_550X;
  long v_549X;
  long code_548X;
  long n_547X;
  char * addr_546X;
  long x_545X;
  long x_544X;
  long pc_543X;
  long code_542X;
  long x_541X;
  long v_540X;
  long v_539X;
  long v_538X;
  long n_537X;
  char * arg_top_536X;
  long count_535X;
  long size_534X;
  char * pointer_533X;
  char * cont_532X;
  long offset_531X;
  long cont_530X;
  long v_529X;
  long v_528X;
  long protocol_527X;
  char * code_pointer_526X;
  long list_arg_count_525X;
  long list_args_524X;
  long stack_nargs_523X;
  long v_522X;
  long proc_521X;
  long v_520X;
  char v_519X;
  char v_518X;
  char v_517X;
  long v_516X;
  long n_515X;
  long code_514X;
  long size_513X;
  char * pointer_512X;
  char * cont_511X;
  long code_510X;
  long n_509X;
  char * addr_508X;
  long x_507X;
  long x_506X;
  char v_505X;
  long obj_504X;
  long stack_arg_count_503X;
  long tag_502X;
  long final_stack_arg_count_501X;
  char interruptP_500X;
  long obj_499X;
  long x_498X;
  long x_497X;
  long envUtemplate_496X;
  long needed_stack_space_495X;
  long template_494X;
  long used_493X;
  long code_492X;
  long tag_491X;
  long skip_490X;
  long x_489X;
  long x_488X;
  long envUtemplate_487X;
  long template_486X;
  long used_485X;
  long envUtemp_offset_484X;
  long code_483X;
  long handlers_482X;
  long opcode_481X;
  long nargs_480X;
  long v_479X;
  long proc_478X;
  long v_477X;
  long proc_476X;
  long index_475X;
  long length_474X;
  long v_473X;
  long proc_472X;
  long v_471X;
  long proc_470X;
  long v_469X;
  long proc_468X;
  long wants_stack_args_467X;
  long skip_466X;
  char nativeP_465X;
  long stack_space_464X;
  long protocol_463X;
  long v_462X;
  long x_461X;
  long n_460X;
  long code_459X;
  long args_458X;
  long v_457X;
  long v_456X;
  long v_455X;
  long list_arg_count_454X;
  long list_args_453X;
  long stack_arg_count_452X;
  long exception_451X;
  long total_arg_count_450X;
  long code_449X;
  long list_arg_count_448X;
  long list_args_447X;
  long stack_arg_count_446X;
  long obj_445X;
 {  if ((3 == (3 & proc_405X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_405X))) + -4))))>>2))))) {
      SvalS = proc_405X;
      obj_445X = SvalS;
      if ((3 == (3 & obj_445X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_445X))) + -4))))>>2))))) {
          arg0K0 = nargs_406X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L26593;}
        else {
          arg0K0 = 3;
          arg0K1 = nargs_406X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L23245;}}
      else {
        arg0K0 = 3;
        arg0K1 = nargs_406X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L23245;}}
    else {
      goto L24209;}}
  else {
    goto L24209;}}
 L26593: {
  stack_arg_count_446X = arg0K0;
  list_args_447X = arg0K1;
  list_arg_count_448X = arg0K2;
  code_449X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  total_arg_count_450X = stack_arg_count_446X + list_arg_count_448X;
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_449X))) + 1)));
  arg0K1 = 64;
  arg2K2 = 0;
  goto L26609;}
 L23245: {
  exception_451X = arg0K0;
  stack_arg_count_452X = arg0K1;
  list_args_453X = arg0K2;
  list_arg_count_454X = arg0K3;
  if (((Scode_pointerS) == (((char *) 0)))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      return v_455X;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      return v_456X;}}
  else {
    merged_arg0K0 = list_args_453X;
    merged_arg0K1 = list_arg_count_454X;
    copy_listSAgc_return_tag = 0;
    goto copy_listSAgc;
   copy_listSAgc_return_0:
    v_457X = copy_listSAgc0_return_value;
    merged_arg0K0 = v_457X;
    merged_arg0K1 = stack_arg_count_452X;
    pop_args_GlistSAgc_return_tag = 0;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_0:
    args_458X = pop_args_GlistSAgc0_return_value;
    code_459X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_459X)))))<<2)), code_459X, (((exception_451X)<<2)), 0);
    n_460X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_460X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((exception_451X)<<2));
    x_461X = SvalS;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_461X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_458X;
    arg0K0 = 2;
    goto L22779;}}
 L24209: {
  ps_error("s48-restart called with non-procedure", 1, proc_405X);
  return v_462X;}
 L26609: {
  protocol_463X = arg0K0;
  stack_space_464X = arg0K1;
  nativeP_465X = arg2K2;
  if ((69 == protocol_463X)) {
    if ((total_arg_count_450X < 3)) {
      skip_466X = *((unsigned char *) ((((char *) (-3 + code_449X))) + (3 + total_arg_count_450X)));
      if ((0 == skip_466X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_446X;
        arg0K2 = list_args_447X;
        arg0K3 = list_arg_count_448X;
        goto L23245;}
      else {
        merged_arg0K0 = list_args_447X;
        merged_arg0K1 = list_arg_count_448X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = code_449X;
        arg0K1 = 6;
        arg0K2 = skip_466X;
        arg0K3 = (*((long *) (((char *) (-3 + (SvalS))))));
        goto L22340;}}
    else {
      if ((0 == (*((unsigned char *) ((((char *) (-3 + code_449X))) + 2))))) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_446X;
        arg0K2 = list_args_447X;
        arg0K3 = list_arg_count_448X;
        goto L23245;}
      else {
        arg0K0 = 6;
        goto L26648;}}}
  else {
    if ((63 < protocol_463X)) {
      if ((65 == protocol_463X)) {
        wants_stack_args_467X = ((((*((unsigned char *) ((((char *) (-3 + code_449X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_449X))) + 3)));
        if ((total_arg_count_450X < wants_stack_args_467X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_446X;
          arg0K2 = list_args_447X;
          arg0K3 = list_arg_count_448X;
          goto L23245;}
        else {
          merged_arg0K0 = wants_stack_args_467X;
          merged_arg0K1 = stack_arg_count_446X;
          merged_arg0K2 = list_args_447X;
          merged_arg0K3 = list_arg_count_448X;
          rest_list_setupAgc_return_tag = 0;
          goto rest_list_setupAgc;
         rest_list_setupAgc_return_0:
          if (nativeP_465X) {
            proc_468X = SvalS;
            merged_arg0K0 = (1 + wants_stack_args_467X);
            move_args_above_contB_return_tag = 0;
            goto move_args_above_contB;
           move_args_above_contB_return_0:
            v_469X = s48_call_native_procedure(proc_468X, 4);
            arg0K0 = v_469X;
            goto L24120;}
          else {
            arg0K0 = code_449X;
            arg0K1 = 4;
            arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
            arg0K3 = stack_space_464X;
            goto L25400;}}}
      else {
        if ((68 == protocol_463X)) {
          if ((total_arg_count_450X < (*((unsigned char *) ((((char *) (-3 + code_449X))) + 2))))) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_446X;
            arg0K2 = list_args_447X;
            arg0K3 = list_arg_count_448X;
            goto L23245;}
          else {
            arg0K0 = 3;
            goto L26648;}}
        else {
          if ((127 < protocol_463X)) {
            arg0K0 = (127 & protocol_463X);
            arg0K1 = stack_space_464X;
            arg2K2 = 1;
            goto L26609;}
          else {
            if ((64 == protocol_463X)) {
              if (((((((*((unsigned char *) ((((char *) (-3 + code_449X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_449X))) + 3)))) == total_arg_count_450X)) {
                if ((0 == list_arg_count_448X)) {
                  if (nativeP_465X) {
                    proc_470X = SvalS;
                    merged_arg0K0 = total_arg_count_450X;
                    move_args_above_contB_return_tag = 1;
                    goto move_args_above_contB;
                   move_args_above_contB_return_1:
                    v_471X = s48_call_native_procedure(proc_470X, 4);
                    arg0K0 = v_471X;
                    goto L24120;}
                  else {
                    arg0K0 = code_449X;
                    arg0K1 = 4;
                    arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
                    arg0K3 = stack_space_464X;
                    goto L25400;}}
                else {
                  merged_arg0K0 = list_args_447X;
                  merged_arg0K1 = list_arg_count_448X;
                  push_list_return_tag = 1;
                  goto push_list;
                 push_list_return_1:
                  if (nativeP_465X) {
                    proc_472X = SvalS;
                    merged_arg0K0 = total_arg_count_450X;
                    move_args_above_contB_return_tag = 2;
                    goto move_args_above_contB;
                   move_args_above_contB_return_2:
                    v_473X = s48_call_native_procedure(proc_472X, 4);
                    arg0K0 = v_473X;
                    goto L24120;}
                  else {
                    arg0K0 = code_449X;
                    arg0K1 = 4;
                    arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
                    arg0K3 = stack_space_464X;
                    goto L25400;}}}
              else {
                arg0K0 = 4;
                arg0K1 = stack_arg_count_446X;
                arg0K2 = list_args_447X;
                arg0K3 = list_arg_count_448X;
                goto L23245;}}
            else {
              if ((67 == protocol_463X)) {
                length_474X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_449X))) + -4))))>>8);
                index_475X = -2 + length_474X;
                arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_449X))) + (-3 + length_474X))));
                arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_449X))) + index_475X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_449X))) + (1 + index_475X)))));
                arg2K2 = nativeP_465X;
                goto L26609;}
              else {
                ps_error("unknown protocol", 1, protocol_463X);
                arg0K0 = 4;
                arg0K1 = stack_arg_count_446X;
                arg0K2 = list_args_447X;
                arg0K3 = list_arg_count_448X;
                goto L23245;}}}}}}
    else {
      if ((protocol_463X == total_arg_count_450X)) {
        if ((0 == list_arg_count_448X)) {
          if (nativeP_465X) {
            proc_476X = SvalS;
            merged_arg0K0 = total_arg_count_450X;
            move_args_above_contB_return_tag = 3;
            goto move_args_above_contB;
           move_args_above_contB_return_3:
            v_477X = s48_call_native_procedure(proc_476X, 2);
            arg0K0 = v_477X;
            goto L24120;}
          else {
            arg0K0 = code_449X;
            arg0K1 = 2;
            arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
            arg0K3 = stack_space_464X;
            goto L25400;}}
        else {
          merged_arg0K0 = list_args_447X;
          merged_arg0K1 = list_arg_count_448X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          if (nativeP_465X) {
            proc_478X = SvalS;
            merged_arg0K0 = total_arg_count_450X;
            move_args_above_contB_return_tag = 4;
            goto move_args_above_contB;
           move_args_above_contB_return_4:
            v_479X = s48_call_native_procedure(proc_478X, 2);
            arg0K0 = v_479X;
            goto L24120;}
          else {
            arg0K0 = code_449X;
            arg0K1 = 2;
            arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
            arg0K3 = stack_space_464X;
            goto L25400;}}}
      else {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_446X;
        arg0K2 = list_args_447X;
        arg0K3 = list_arg_count_448X;
        goto L23245;}}}}
 L22779: {
  nargs_480X = arg0K0;
  opcode_481X = (((*((long *) ((SstackS) + (4 + (((nargs_480X)<<2)))))))>>2);
  handlers_482X = SHARED_REF((Sexception_handlersS));
  if ((3 == (3 & handlers_482X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_482X))) + -4))))>>2))))) {
      goto L22844;}
    else {
      goto L22927;}}
  else {
    goto L22927;}}
 L22340: {
  code_483X = arg0K0;
  envUtemp_offset_484X = arg0K1;
  used_485X = arg0K2;
  template_486X = arg0K3;
  envUtemplate_487X = *((unsigned char *) ((((char *) (-3 + code_483X))) + envUtemp_offset_484X));
  if ((3 == envUtemplate_487X)) {
    x_488X = *((long *) ((((char *) (-3 + (SvalS)))) + 4));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_488X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = template_486X;
    goto L22344;}
  else {
    if ((1 == envUtemplate_487X)) {
      x_489X = *((long *) ((((char *) (-3 + (SvalS)))) + 4));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_489X;
      goto L22344;}
    else {
      if ((2 == envUtemplate_487X)) {
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = template_486X;
        goto L22344;}
      else {
        goto L22344;}}}}
 L26648: {
  skip_490X = arg0K0;
  if ((total_arg_count_450X < 3)) {
    arg0K0 = total_arg_count_450X;
    goto L26656;}
  else {
    if ((2 < stack_arg_count_446X)) {
      arg0K0 = stack_arg_count_446X;
      goto L26656;}
    else {
      arg0K0 = 2;
      goto L26656;}}}
 L24120: {
  tag_491X = arg0K0;
  arg0K0 = tag_491X;
  goto L24124;}
 L25400: {
  code_492X = arg0K0;
  used_493X = arg0K1;
  template_494X = arg0K2;
  needed_stack_space_495X = arg0K3;
  envUtemplate_496X = *((unsigned char *) ((((char *) (-3 + code_492X))) + used_493X));
  if ((3 == envUtemplate_496X)) {
    x_497X = *((long *) ((((char *) (-3 + (SvalS)))) + 4));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_497X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = template_494X;
    goto L25404;}
  else {
    if ((1 == envUtemplate_496X)) {
      x_498X = *((long *) ((((char *) (-3 + (SvalS)))) + 4));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_498X;
      goto L25404;}
    else {
      if ((2 == envUtemplate_496X)) {
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = template_494X;
        goto L25404;}
      else {
        goto L25404;}}}}
 L22844: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_482X))) + (((opcode_481X)<<2)))));
  obj_499X = SvalS;
  if ((3 == (3 & obj_499X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_499X))) + -4))))>>2))))) {
      goto L22861;}
    else {
      goto L22941;}}
  else {
    goto L22941;}}
 L22927: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L22844;}
 L22344: {
  Slast_code_calledS = code_483X;
  Scode_pointerS = ((((char *) (-3 + code_483X))) + used_485X);
  if (((SstackS) < (s48_Sstack_limitS))) {
    interruptP_500X = (s48_Sstack_limitS) == (((char *) -1));
    s48_Sstack_limitS = (Sreal_stack_limitS);
    if (((SstackS) < (Sreal_stack_limitS))) {s48_copy_stack_into_heap();
      if (((SstackS) < (Sreal_stack_limitS))) {
        ps_error("VM's stack is too small (how can this happen?)", 0);
        if (interruptP_500X) {
          goto L22351;}
        else {
          goto L22358;}}
      else {
        if (interruptP_500X) {
          goto L22351;}
        else {
          goto L22358;}}}
    else {
      if (interruptP_500X) {
        goto L22351;}
      else {
        goto L22358;}}}
  else {
    goto L22358;}}
 L26656: {
  final_stack_arg_count_501X = arg0K0;
  if ((stack_arg_count_446X < final_stack_arg_count_501X)) {
    arg0K0 = final_stack_arg_count_501X;
    goto L26660;}
  else {
    arg0K0 = stack_arg_count_446X;
    goto L26660;}}
 L24124: {
  tag_502X = arg0K0;
  if ((tag_502X == 0)) {
    arg0K0 = (s48_Snative_protocolS);
    arg0K1 = 25;
    arg0K2 = 0;
    goto L22492;}
  else {
    if ((tag_502X == 1)) {
      stack_arg_count_503X = s48_Snative_protocolS;
      obj_504X = SvalS;
      if ((3 == (3 & obj_504X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_504X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_503X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L26593;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_503X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L23245;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_503X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L23245;}}
    else {
      if ((tag_502X == 2)) {
        pending_interruptP_return_tag = 0;
        goto pending_interruptP;
       pending_interruptP_return_0:
        v_505X = pending_interruptP0_return_value;
        if (v_505X) {
          x_506X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_506X;
          x_507X = Scurrent_threadS;
          addr_508X = (((char *) (-3 + x_507X))) + 12;S48_WRITE_BARRIER(x_507X, addr_508X, 1);
          *((long *) addr_508X) = 1;
          n_509X = Senabled_interruptsS;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_509X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 12;
          code_510X = Sinterrupt_return_codeS;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((long) ((((char *) (-3 + code_510X))) + 11)));
          ScontS = (SstackS);
          goto L23023;}
        else {
          SstackS = (ScontS);
          cont_511X = ScontS;
          pointer_512X = (((char *) (*((long *) cont_511X)))) + -2;
          size_513X = ((((*((unsigned char *) pointer_512X)))<<8)) + (*((unsigned char *) (pointer_512X + 1)));
          if ((65535 == size_513X)) {
            arg0K0 = ((((*((long *) (cont_511X + 4))))>>2));
            goto L24167;}
          else {
            arg0K0 = size_513X;
            goto L24167;}}}
      else {
        if ((tag_502X == 3)) {
          code_514X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_514X)))))<<2)), code_514X, 108, 0);
          n_515X = *((unsigned char *) (Scode_pointerS));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_515X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 108;
          arg0K0 = 0;
          goto L22779;}
        else {
          ps_error("unexpected native return value", 1, tag_502X);
          return v_516X;}}}}}
 L25404: {
  Slast_code_calledS = code_492X;
  Scode_pointerS = ((((char *) (-3 + code_492X))) + (1 + used_493X));
  merged_arg0K0 = needed_stack_space_495X;
  ensure_stack_spaceB_return_tag = 0;
  goto ensure_stack_spaceB;
 ensure_stack_spaceB_return_0:
  v_517X = ensure_stack_spaceB0_return_value;
  if (v_517X) {
    pending_interruptP_return_tag = 1;
    goto pending_interruptP;
   pending_interruptP_return_1:
    v_518X = pending_interruptP0_return_value;
    if (v_518X) {
      goto L23759;}
    else {
      goto L25418;}}
  else {
    goto L25418;}}
 L22861: {
  Scode_pointerS = (((char *) 0));
  Slosing_opcodeS = opcode_481X;
  arg0K0 = (2 + nargs_480X);
  arg0K1 = 25;
  arg0K2 = 0;
  goto L26593;}
 L22941: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L22861;}
 L22351: {
  pending_interruptP_return_tag = 2;
  goto pending_interruptP;
 pending_interruptP_return_2:
  v_519X = pending_interruptP0_return_value;
  if (v_519X) {
    goto L23759;}
  else {
    goto L22358;}}
 L22358: {
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L26660: {
  v_520X = arg0K0;
  merged_arg0K0 = v_520X;
  merged_arg0K1 = stack_arg_count_446X;
  merged_arg0K2 = list_args_447X;
  merged_arg0K3 = list_arg_count_448X;
  rest_list_setupAgc_return_tag = 1;
  goto rest_list_setupAgc;
 rest_list_setupAgc_return_1:
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((final_stack_arg_count_501X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_450X)<<2));
  if (nativeP_465X) {
    proc_521X = SvalS;
    merged_arg0K0 = (3 + final_stack_arg_count_501X);
    move_args_above_contB_return_tag = 5;
    goto move_args_above_contB;
   move_args_above_contB_return_5:
    v_522X = s48_call_native_procedure(proc_521X, skip_490X);
    arg0K0 = v_522X;
    goto L24120;}
  else {
    arg0K0 = code_449X;
    arg0K1 = skip_490X;
    arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
    arg0K3 = stack_space_464X;
    goto L25400;}}
 L22492: {
  stack_nargs_523X = arg0K0;
  list_args_524X = arg0K1;
  list_arg_count_525X = arg0K2;
  code_pointer_526X = ((char *) (*((long *) (ScontS))));
  protocol_527X = *((unsigned char *) (code_pointer_526X + 1));
  if ((1 == protocol_527X)) {
    if ((1 == (stack_nargs_523X + list_arg_count_525X))) {
      if ((1 == stack_nargs_523X)) {
        v_528X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_528X;
        goto L22581;}
      else {
        arg0K0 = (*((long *) (((char *) (-3 + list_args_524X)))));
        goto L22581;}}
    else {
      arg0K0 = stack_nargs_523X;
      arg0K1 = list_args_524X;
      goto L23418;}}
  else {
    if ((66 == protocol_527X)) {
      pop_continuationB_return_tag = 0;
      goto pop_continuationB;
     pop_continuationB_return_0:
      arg0K0 = 1;
      goto L25390;}
    else {
      if ((127 < protocol_527X)) {
        if ((129 == protocol_527X)) {
          if ((1 == (stack_nargs_523X + list_arg_count_525X))) {
            if ((1 == stack_nargs_523X)) {
              v_529X = *((long *) (SstackS));
              SstackS = ((SstackS) + 4);
              arg0K0 = v_529X;
              goto L24914;}
            else {
              arg0K0 = (*((long *) (((char *) (-3 + list_args_524X)))));
              goto L24914;}}
          else {
            arg0K0 = stack_nargs_523X;
            arg0K1 = list_args_524X;
            goto L23418;}}
        else {
          if ((194 == protocol_527X)) {
            arg0K0 = 2;
            goto L24626;}
          else {
            ps_error("unknown native return protocol", 1, protocol_527X);
            arg0K0 = stack_nargs_523X;
            arg0K1 = list_args_524X;
            goto L23418;}}}
      else {
        if ((71 == protocol_527X)) {
          cont_530X = Sheap_continuationS;
          if ((3 == (3 & cont_530X))) {
            if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_530X))) + -4))))>>2))))) {
              merged_arg0K0 = cont_530X;
              merged_arg0K1 = stack_nargs_523X;
              copy_continuation_from_heapB_return_tag = 0;
              goto copy_continuation_from_heapB;
             copy_continuation_from_heapB_return_0:
              arg0K0 = stack_nargs_523X;
              arg0K1 = list_args_524X;
              arg0K2 = list_arg_count_525X;
              goto L22492;}
            else {
              goto L22527;}}
          else {
            goto L22527;}}
        else {
          if ((70 == protocol_527X)) {
            SvalS = (*((long *) ((ScontS) + 4)));
            offset_531X = ((((*((unsigned char *) (code_pointer_526X + 2))))<<8)) + (*((unsigned char *) (code_pointer_526X + 3)));
            if ((0 == offset_531X)) {
              cont_532X = ScontS;
              pointer_533X = (((char *) (*((long *) cont_532X)))) + -2;
              size_534X = ((((*((unsigned char *) pointer_533X)))<<8)) + (*((unsigned char *) (pointer_533X + 1)));
              if ((65535 == size_534X)) {
                arg0K0 = ((((*((long *) (cont_532X + 4))))>>2));
                goto L22644;}
              else {
                arg0K0 = size_534X;
                goto L22644;}}
            else {
              ScontS = ((ScontS) + 4);
              *((long *) (ScontS)) = (((long) (code_pointer_526X + offset_531X)));
              merged_arg0K0 = stack_nargs_523X;
              move_args_above_contB_return_tag = 6;
              goto move_args_above_contB;
             move_args_above_contB_return_6:
              arg0K0 = stack_nargs_523X;
              arg0K1 = list_args_524X;
              arg0K2 = list_arg_count_525X;
              goto L32292;}}
          else {
            if ((63 < protocol_527X)) {
              if ((65 == protocol_527X)) {
                count_535X = ((((*((unsigned char *) (code_pointer_526X + 2))))<<8)) + (*((unsigned char *) (code_pointer_526X + 3)));
                if (((stack_nargs_523X + list_arg_count_525X) < count_535X)) {
                  arg0K0 = stack_nargs_523X;
                  arg0K1 = list_args_524X;
                  goto L23418;}
                else {
                  arg_top_536X = SstackS;
                  pop_continuationB_return_tag = 1;
                  goto pop_continuationB;
                 pop_continuationB_return_1:
                  arg1K0 = ((SstackS) + -4);
                  arg1K1 = (arg_top_536X + (-4 + (((stack_nargs_523X)<<2))));
                  goto L26341;}}
              else {
                if ((64 == protocol_527X)) {
                  arg0K0 = (((((*((unsigned char *) (code_pointer_526X + 2))))<<8)) + (*((unsigned char *) (code_pointer_526X + 3))));
                  arg0K1 = 3;
                  arg0K2 = stack_nargs_523X;
                  arg0K3 = list_args_524X;
                  arg0K4 = list_arg_count_525X;
                  goto L26207;}
                else {
                  ps_error("unknown protocol", 1, protocol_527X);
                  arg0K0 = stack_nargs_523X;
                  arg0K1 = list_args_524X;
                  goto L23418;}}}
            else {
              arg0K0 = protocol_527X;
              arg0K1 = 1;
              arg0K2 = stack_nargs_523X;
              arg0K3 = list_args_524X;
              arg0K4 = list_arg_count_525X;
              goto L26207;}}}}}}}
 L23023: {
  n_537X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L23076;}
 L24167: {
  v_538X = arg0K0;
  ScontS = (cont_511X + (4 + (((v_538X)<<2))));
  v_539X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  v_540X = s48_invoke_native_continuation((2 + (((long) (((char *) v_539X))))));
  arg0K0 = v_540X;
  goto L24124;}
 L23759: {
  x_541X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_541X;
  code_542X = current_code_vector();
  pc_543X = (Scode_pointerS) - (((char *) (-3 + code_542X)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = code_542X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((pc_543X)<<2));
  x_544X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_544X;
  x_545X = Scurrent_threadS;
  addr_546X = (((char *) (-3 + x_545X))) + 12;S48_WRITE_BARRIER(x_545X, addr_546X, 1);
  *((long *) addr_546X) = 1;
  n_547X = Senabled_interruptsS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_547X)<<2));
  code_548X = Sinterrupt_return_codeS;
  v_549X = ((((ScontS) - (SstackS)))>>2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (4 + (((v_549X)<<2)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((long) ((((char *) (-3 + code_548X))) + 11)));
  ScontS = (SstackS);
  goto L23023;}
 L25418: {
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L24972: {
  code_pointer_550X = arg1K0;
  switch ((*((unsigned char *) code_pointer_550X))) {
    case 0 : 
    case 23 : 
    case 27 : 
    case 34 : {
      code_551X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_551X)))))<<2)), code_551X, 56, 4);
      n_552X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_552X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 56;
      n_553X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_553X)<<2));
      arg0K0 = 1;
      goto L22779;}
      break;
    case 1 : {
      SvalS = (-512 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 2 : {
      x_554X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_554X;
      SvalS = (-512 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 3 : {
      x_555X = -512 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_555X;
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 4 : {
      template_556X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      index_557X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
      location_558X = *((long *) ((((char *) (-3 + template_556X))) + (((index_557X)<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_558X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        code_559X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_559X)))))<<2)), code_559X, 4, 20);
        n_560X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_560X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 4;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = location_558X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = template_556X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((index_557X)<<2));
        arg0K0 = 3;
        goto L22779;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 5);
        arg1K0 = (Scode_pointerS);
        goto L24972;}}
      break;
    case 5 : {
      template_561X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      index_562X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
      location_563X = *((long *) ((((char *) (-3 + template_561X))) + (((index_562X)<<2))));
      if ((273 == (*((long *) ((((char *) (-3 + location_563X))) + 4))))) {
        code_564X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_564X)))))<<2)), code_564X, 4, 20);
        n_565X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_565X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 4;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = location_563X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = template_561X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((index_562X)<<2));
        x_566X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_566X;
        arg0K0 = 4;
        goto L22779;}
      else {
        val_567X = SvalS;
        addr_568X = (((char *) (-3 + location_563X))) + 4;S48_WRITE_BARRIER(location_563X, addr_568X, val_567X);
        *((long *) addr_568X) = val_567X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 5);
        arg1K0 = (Scode_pointerS);
        goto L24972;}}
      break;
    case 6 : {
      total_count_569X = *((unsigned char *) ((Scode_pointerS) + 1));
      closures_570X = *((unsigned char *) ((Scode_pointerS) + 2));s48_make_availableAgc((((((1 + total_count_569X) + (3 * closures_570X)))<<2)));
      len_571X = ((total_count_569X)<<2);
      addr_572X = s48_allocate_small((4 + len_571X));
      *((long *) addr_572X) = (10 + (((len_571X)<<8)));
      new_env_573X = 3 + (((long) (addr_572X + 4)));
      if ((0 == closures_570X)) {
        arg0K0 = new_env_573X;
        arg0K1 = 0;
        arg0K2 = 2;
        arg0K3 = total_count_569X;
        goto L8182;}
      else {
        template_574X = *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<2))));
        arg0K0 = closures_570X;
        arg0K1 = 0;
        arg0K2 = 3;
        goto L17214;}}
      break;
    case 7 : {
      total_count_575X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      closures_576X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));s48_make_availableAgc((((((1 + total_count_575X) + (3 * closures_576X)))<<2)));
      len_577X = ((total_count_575X)<<2);
      addr_578X = s48_allocate_small((4 + len_577X));
      *((long *) addr_578X) = (10 + (((len_577X)<<8)));
      new_env_579X = 3 + (((long) (addr_578X + 4)));
      if ((0 == closures_576X)) {
        arg0K0 = new_env_579X;
        arg0K1 = 0;
        arg0K2 = 4;
        arg0K3 = total_count_575X;
        goto L10756;}
      else {
        template_580X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 5))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 6)))))<<2))));
        arg0K0 = closures_576X;
        arg0K1 = 0;
        arg0K2 = 6;
        goto L17275;}}
      break;
    case 8 : {
      x_581X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_581X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 9 : {
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 10 : {
      value_582X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SvalS = value_582X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 11 : {
      SstackS = ((SstackS) + (0 - ((((0 - (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))))<<2))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 12 : {
      SvalS = (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 13 : {
      x_583X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_583X;
      SvalS = (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 14 : {
      x_584X = *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_584X;
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 15 : {
      SvalS = (*((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 16 : {
      *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 17 : {
      *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 18 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 19 : {
      x_585X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_585X;
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 20 : {
      x_586X = *((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2))));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_586X;
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 21 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))))))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 22 : {s48_make_availableAgc(((((-4 & ((Sstack_endS) - (SstackS))))<<2)));
      if ((1 == (((long) (ScontS))))) {
        arg0K0 = 1;
        goto L50500;}
      else {
        v_587X = really_preserve_continuation(0);
        arg0K0 = v_587X;
        goto L50500;}}
      break;
    case 24 : {
      stack_arg_count_588X = *((unsigned char *) ((Scode_pointerS) + 3));
      code_pointer_589X = (Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      ScontS = ((SstackS) + (((stack_arg_count_588X)<<2)));
      *((long *) (ScontS)) = (((long) code_pointer_589X));
      arg0K0 = stack_arg_count_588X;
      goto L24809;}
      break;
    case 25 : {
      merged_arg0K0 = (*((unsigned char *) ((Scode_pointerS) + 1)));
      move_args_above_contB_return_tag = 7;
      goto move_args_above_contB;
     move_args_above_contB_return_7:
      arg0K0 = (*((unsigned char *) ((Scode_pointerS) + 1)));
      goto L24809;}
      break;
    case 26 : {
      stack_arg_count_590X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
      return_pointer_offset_591X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      if ((0 == return_pointer_offset_591X)) {
        merged_arg0K0 = stack_arg_count_590X;
        move_args_above_contB_return_tag = 8;
        goto move_args_above_contB;
       move_args_above_contB_return_8:
        goto L24086;}
      else {
        code_pointer_592X = (Scode_pointerS) + return_pointer_offset_591X;
        ScontS = ((SstackS) + (((stack_arg_count_590X)<<2)));
        *((long *) (ScontS)) = (((long) code_pointer_592X));
        goto L24086;}}
      break;
    case 28 : {
      list_args_593X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      merged_arg0K0 = list_args_593X;
      okay_argument_list_return_tag = 0;
      goto okay_argument_list;
     okay_argument_list_return_0:
      okayP_594X = okay_argument_list0_return_value;
      length_595X = okay_argument_list1_return_value;
      if (okayP_594X) {
        stack_nargs_596X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
        return_pointer_offset_597X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
        if ((0 == return_pointer_offset_597X)) {
          merged_arg0K0 = stack_nargs_596X;
          move_args_above_contB_return_tag = 9;
          goto move_args_above_contB;
         move_args_above_contB_return_9:
          arg0K0 = stack_nargs_596X;
          arg0K1 = list_args_593X;
          arg0K2 = length_595X;
          goto L32292;}
        else {
          code_pointer_598X = (Scode_pointerS) + return_pointer_offset_597X;
          ScontS = ((SstackS) + (((stack_nargs_596X)<<2)));
          *((long *) (ScontS)) = (((long) code_pointer_598X));
          arg0K0 = stack_nargs_596X;
          arg0K1 = list_args_593X;
          arg0K2 = length_595X;
          goto L32292;}}
      else {
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_args_593X;
        merged_arg0K0 = 25;
        merged_arg0K1 = (1 + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        pop_args_GlistSAgc_return_tag = 1;
        goto pop_args_GlistSAgc;
       pop_args_GlistSAgc_return_1:
        args_599X = pop_args_GlistSAgc0_return_value;
        code_600X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_600X)))))<<2)), code_600X, 20, 0);
        n_601X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_601X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_602X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_602X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = args_599X;
        arg0K0 = 2;
        goto L22779;}}
      break;
    case 29 : {
      p_603X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      p_604X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_605X = ((p_604X)>>2);
      proc_606X = *((long *) ((SstackS) + (((stack_nargs_605X)<<2))));
      merged_arg0K0 = (((p_603X)>>2));
      move_args_above_contB_return_tag = 10;
      goto move_args_above_contB;
     move_args_above_contB_return_10:
      rest_list_607X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((25 == rest_list_607X)) {
        v_608X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_608X;
        arg0K1 = (-2 + stack_nargs_605X);
        goto L15528;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_607X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_607X)))));
          arg0K1 = (-1 + stack_nargs_605X);
          goto L15528;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_607X))) + 4)))))) + 4)));
          arg0K1 = rest_list_607X;
          goto L15573;}}}
      break;
    case 30 : {
      cont_609X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & cont_609X))) {
        if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_609X))) + -4))))>>2))))) {
          merged_arg0K0 = cont_609X;
          merged_arg0K1 = 0;
          copy_continuation_from_heapB_return_tag = 1;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_1:
          goto L24044;}
        else {
          goto L24053;}}
      else {
        goto L24053;}}
      break;
    case 31 : {
      goto L40323;}
      break;
    case 32 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L22492;}
      break;
    case 33 : {
      p_610X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      p_611X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_612X = ((p_611X)>>2);
      rest_list_613X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = stack_nargs_612X;
      arg0K1 = rest_list_613X;
      arg0K2 = ((((p_610X)>>2)) - stack_nargs_612X);
      goto L22492;}
      break;
    case 35 : {
      template_614X = *((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))))))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 5))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 6)))))<<2))));
      code_615X = *((long *) (((char *) (-3 + template_614X))));
      nargs_616X = *((unsigned char *) ((Scode_pointerS) + 7));
      return_pointer_offset_617X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      if ((0 == return_pointer_offset_617X)) {
        merged_arg0K0 = nargs_616X;
        move_args_above_contB_return_tag = 11;
        goto move_args_above_contB;
       move_args_above_contB_return_11:
        goto L26471;}
      else {
        code_pointer_618X = (Scode_pointerS) + return_pointer_offset_617X;
        ScontS = ((SstackS) + (((nargs_616X)<<2)));
        *((long *) (ScontS)) = (((long) code_pointer_618X));
        goto L26471;}}
      break;
    case 36 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L24972;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L24972;}}
      break;
    case 37 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 38 : {
      Scode_pointerS = ((Scode_pointerS) + (0 - (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))));
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 39 : {
      if ((0 == (3 & (SvalS)))) {
        p_619X = SvalS;
        max_620X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_621X = ((p_619X)>>2);
        if ((val_621X < 0)) {
          goto L25259;}
        else {
          if ((val_621X < max_620X)) {
            index_622X = 1 + (((val_621X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_622X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_622X)))));
            goto L25261;}
          else {
            goto L25259;}}}
      else {
        code_623X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_623X)))))<<2)), code_623X, 20, 0);
        n_624X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_624X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_625X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_625X;
        arg0K0 = 1;
        goto L22779;}}
      break;
    case 40 : {
      stack_nargs_626X = (((*((long *) (SstackS))))>>2);
      if ((0 == stack_nargs_626X)) {
        arg0_627X = *((long *) ((SstackS) + 8));
        rest_list_628X = *((long *) ((SstackS) + 4));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg0_627X;
        SvalS = (*((long *) (((char *) (-3 + rest_list_628X)))));
        goto L25844;}
      else {
        arg1_629X = *((long *) ((SstackS) + (((stack_nargs_626X)<<2))));
        arg0_630X = *((long *) ((SstackS) + (4 + (((stack_nargs_626X)<<2)))));
        *((long *) ((SstackS) + (4 + (((stack_nargs_626X)<<2))))) = 1;
        *((long *) (SstackS)) = (-4 + (((stack_nargs_626X)<<2)));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg0_630X;
        SvalS = arg1_629X;
        goto L25844;}}
      break;
    case 41 : {
      stack_nargs_631X = (((*((long *) (SstackS))))>>2);
      if ((stack_nargs_631X == 0)) {
        rest_list_632X = *((long *) ((SstackS) + 4));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_632X))) + 4))))) {
          arg0K0 = 1;
          goto L25157;}
        else {
          *((long *) ((SstackS) + 4)) = (*((long *) ((((char *) (-3 + rest_list_632X))) + 4)));
          *((long *) ((SstackS) + 8)) = (SvalS);
          arg0K0 = -2;
          goto L25157;}}
      else {
        if ((stack_nargs_631X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 4))))) {
            arg0K0 = 1;
            goto L25157;}
          else {
            *((long *) (SstackS)) = 0;
            *((long *) ((SstackS) + 8)) = (SvalS);
            arg0K0 = -2;
            goto L25157;}}
        else {
          *((long *) ((SstackS) + (4 + (((stack_nargs_631X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L25157;}}}
      break;
    case 42 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L24972;}
      else {
        x_633X = (((*((long *) (SstackS))))>>2);
        if ((x_633X == 0)) {
          rest_list_634X = *((long *) ((SstackS) + 4));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_634X))) + 4))))) {
            arg0K0 = 1;
            goto L25735;}
          else {
            *((long *) ((SstackS) + 4)) = (*((long *) ((((char *) (-3 + rest_list_634X))) + 4)));
            *((long *) ((SstackS) + 8)) = (*((long *) (((char *) (-3 + rest_list_634X)))));
            arg0K0 = -2;
            goto L25735;}}
        else {
          if ((x_633X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 4))))) {
              arg0K0 = 1;
              goto L25735;}
            else {
              *((long *) (SstackS)) = 0;
              arg0K0 = -2;
              goto L25735;}}
          else {
            arg0K0 = -2;
            goto L25735;}}}}
      break;
    case 43 : {
      arg2_635X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_636X = SvalS;
      if ((arg2_635X == x_636X)) {
        arg0K0 = 5;
        goto L50557;}
      else {
        arg0K0 = 1;
        goto L50557;}}
      break;
    case 44 : {
      x_637X = SvalS;
      if ((0 == (3 & x_637X))) {
        arg0K0 = 5;
        goto L50569;}
      else {
        if ((3 == (3 & x_637X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_637X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L50569;}
          else {
            goto L14108;}}
        else {
          goto L14108;}}}
      break;
    case 45 : {
      n_638X = SvalS;
      if ((0 == (3 & n_638X))) {
        goto L41618;}
      else {
        if ((3 == (3 & n_638X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_638X))) + -4))))>>2))))) {
            goto L41618;}
          else {
            goto L41619;}}
        else {
          goto L41619;}}}
      break;
    case 46 : {
      n_639X = SvalS;
      if ((0 == (3 & n_639X))) {
        goto L41767;}
      else {
        if ((3 == (3 & n_639X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_639X))) + -4))))>>2))))) {
            goto L41767;}
          else {
            goto L41742;}}
        else {
          goto L41742;}}}
      break;
    case 47 : {
      n_640X = SvalS;
      if ((0 == (3 & n_640X))) {
        goto L41958;}
      else {
        if ((3 == (3 & n_640X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_640X))) + -4))))>>2))))) {
            goto L41958;}
          else {
            goto L41933;}}
        else {
          goto L41933;}}}
      break;
    case 48 : {
      n_641X = SvalS;
      if ((0 == (3 & n_641X))) {
        goto L42149;}
      else {
        if ((3 == (3 & n_641X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_641X))) + -4))))>>2))))) {
            goto L42149;}
          else {
            goto L42124;}}
        else {
          goto L42124;}}}
      break;
    case 49 : {
      n_642X = SvalS;
      if ((0 == (3 & n_642X))) {
        goto L33849;}
      else {
        if ((3 == (3 & n_642X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_642X))) + -4))))>>2))))) {
            goto L33849;}
          else {
            goto L33850;}}
        else {
          goto L33850;}}}
      break;
    case 50 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 51 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 52 : {
      arg2_643X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_644X = SvalS;
      if ((0 == (3 & (arg2_643X | y_644X)))) {s48_make_availableAgc(16);
        x_645X = (((arg2_643X)>>2)) + (((y_644X)>>2));
        if ((536870911 < x_645X)) {
          goto L33991;}
        else {
          if ((x_645X < -536870912)) {
            goto L33991;}
          else {
            arg0K0 = (((x_645X)<<2));
            goto L33937;}}}
      else {
        if ((0 == (3 & arg2_643X))) {
          goto L33943;}
        else {
          if ((3 == (3 & arg2_643X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_643X))) + -4))))>>2))))) {
              goto L33943;}
            else {
              goto L33952;}}
          else {
            goto L33952;}}}}
      break;
    case 53 : {
      arg2_646X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_647X = SvalS;
      if ((0 == (3 & (arg2_646X | y_647X)))) {
        a_648X = ((arg2_646X)>>2);
        b_649X = ((y_647X)>>2);
        if ((a_648X < 0)) {
          arg0K0 = (0 - a_648X);
          goto L8347;}
        else {
          arg0K0 = a_648X;
          goto L8347;}}
      else {
        if ((0 == (3 & arg2_646X))) {
          goto L44022;}
        else {
          if ((3 == (3 & arg2_646X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_646X))) + -4))))>>2))))) {
              goto L44022;}
            else {
              goto L44031;}}
          else {
            goto L44031;}}}}
      break;
    case 54 : {
      arg2_650X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_651X = SvalS;
      if ((0 == (3 & (arg2_650X | y_651X)))) {s48_make_availableAgc(16);
        x_652X = (((arg2_650X)>>2)) - (((y_651X)>>2));
        if ((536870911 < x_652X)) {
          goto L34296;}
        else {
          if ((x_652X < -536870912)) {
            goto L34296;}
          else {
            arg0K0 = (((x_652X)<<2));
            goto L34242;}}}
      else {
        if ((0 == (3 & arg2_650X))) {
          goto L34248;}
        else {
          if ((3 == (3 & arg2_650X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_650X))) + -4))))>>2))))) {
              goto L34248;}
            else {
              goto L34257;}}
          else {
            goto L34257;}}}}
      break;
    case 55 : {
      arg2_653X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_654X = SvalS;
      if ((0 == y_654X)) {
        arg0K0 = arg2_653X;
        arg0K1 = y_654X;
        goto L23692;}
      else {
        if ((0 == (3 & (arg2_653X | y_654X)))) {
          if ((0 == y_654X)) {
            arg0K0 = arg2_653X;
            arg0K1 = y_654X;
            goto L23692;}
          else {
            a_655X = ((arg2_653X)>>2);
            b_656X = ((y_654X)>>2);
            if ((a_655X < 0)) {
              arg0K0 = (0 - a_655X);
              goto L8605;}
            else {
              arg0K0 = a_655X;
              goto L8605;}}}
        else {
          if ((0 == (3 & arg2_653X))) {
            goto L44265;}
          else {
            if ((3 == (3 & arg2_653X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_653X))) + -4))))>>2))))) {
                goto L44265;}
              else {
                goto L44298;}}
            else {
              goto L44298;}}}}}
      break;
    case 56 : {
      arg2_657X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_658X = SvalS;
      if ((0 == (3 & (arg2_657X | y_658X)))) {
        if ((arg2_657X == y_658X)) {
          arg0K0 = 5;
          goto L34543;}
        else {
          arg0K0 = 1;
          goto L34543;}}
      else {
        if ((0 == (3 & arg2_657X))) {
          goto L34549;}
        else {
          if ((3 == (3 & arg2_657X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_657X))) + -4))))>>2))))) {
              goto L34549;}
            else {
              goto L34560;}}
          else {
            goto L34560;}}}}
      break;
    case 57 : {
      arg2_659X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_660X = SvalS;
      if ((0 == (3 & (arg2_659X | y_660X)))) {
        if ((arg2_659X < y_660X)) {
          arg0K0 = 5;
          goto L34819;}
        else {
          arg0K0 = 1;
          goto L34819;}}
      else {
        if ((0 == (3 & arg2_659X))) {
          goto L34825;}
        else {
          if ((3 == (3 & arg2_659X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_659X))) + -4))))>>2))))) {
              goto L34825;}
            else {
              goto L34836;}}
          else {
            goto L34836;}}}}
      break;
    case 58 : {
      arg2_661X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_662X = SvalS;
      if ((0 == (3 & (arg2_661X | y_662X)))) {
        if ((y_662X < arg2_661X)) {
          arg0K0 = 5;
          goto L35158;}
        else {
          arg0K0 = 1;
          goto L35158;}}
      else {
        if ((0 == (3 & arg2_661X))) {
          goto L35164;}
        else {
          if ((3 == (3 & arg2_661X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_661X))) + -4))))>>2))))) {
              goto L35164;}
            else {
              goto L35175;}}
          else {
            goto L35175;}}}}
      break;
    case 59 : {
      arg2_663X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_664X = SvalS;
      if ((0 == (3 & (arg2_663X | y_664X)))) {
        if ((y_664X < arg2_663X)) {
          arg0K0 = 1;
          goto L35497;}
        else {
          arg0K0 = 5;
          goto L35497;}}
      else {
        if ((0 == (3 & arg2_663X))) {
          goto L35503;}
        else {
          if ((3 == (3 & arg2_663X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_663X))) + -4))))>>2))))) {
              goto L35503;}
            else {
              goto L35514;}}
          else {
            goto L35514;}}}}
      break;
    case 60 : {
      arg2_665X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_666X = SvalS;
      if ((0 == (3 & (arg2_665X | y_666X)))) {
        if ((arg2_665X < y_666X)) {
          arg0K0 = 1;
          goto L35807;}
        else {
          arg0K0 = 5;
          goto L35807;}}
      else {
        if ((0 == (3 & arg2_665X))) {
          goto L35813;}
        else {
          if ((3 == (3 & arg2_665X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_665X))) + -4))))>>2))))) {
              goto L35813;}
            else {
              goto L35824;}}
          else {
            goto L35824;}}}}
      break;
    case 61 : {
      arg2_667X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_668X = SvalS;
      if ((0 == y_668X)) {
        arg0K0 = arg2_667X;
        arg0K1 = y_668X;
        goto L23692;}
      else {
        if ((0 == (3 & (arg2_667X | y_668X)))) {
          if ((0 == y_668X)) {
            arg0K0 = arg2_667X;
            arg0K1 = y_668X;
            goto L19818;}
          else {
            a_669X = ((arg2_667X)>>2);
            b_670X = ((y_668X)>>2);
            if ((a_669X < 0)) {
              arg0K0 = (0 - a_669X);
              goto L9219;}
            else {
              arg0K0 = a_669X;
              goto L9219;}}}
        else {
          if ((0 == (3 & arg2_667X))) {
            goto L36129;}
          else {
            if ((3 == (3 & arg2_667X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_667X))) + -4))))>>2))))) {
                goto L36129;}
              else {
                arg0K0 = arg2_667X;
                arg0K1 = y_668X;
                goto L23692;}}
            else {
              arg0K0 = arg2_667X;
              arg0K1 = y_668X;
              goto L23692;}}}}}
      break;
    case 62 : {
      arg2_671X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_672X = SvalS;
      if ((0 == y_672X)) {
        arg0K0 = arg2_671X;
        arg0K1 = y_672X;
        goto L23692;}
      else {
        if ((0 == (3 & (arg2_671X | y_672X)))) {
          if ((0 == y_672X)) {
            arg0K0 = arg2_671X;
            arg0K1 = y_672X;
            goto L19727;}
          else {
            a_673X = ((arg2_671X)>>2);
            if ((a_673X < 0)) {
              arg0K0 = (0 - a_673X);
              goto L36309;}
            else {
              arg0K0 = a_673X;
              goto L36309;}}}
        else {
          if ((0 == (3 & arg2_671X))) {
            goto L36283;}
          else {
            if ((3 == (3 & arg2_671X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_671X))) + -4))))>>2))))) {
                goto L36283;}
              else {
                arg0K0 = arg2_671X;
                arg0K1 = y_672X;
                goto L23692;}}
            else {
              arg0K0 = arg2_671X;
              arg0K1 = y_672X;
              goto L23692;}}}}}
      break;
    case 63 : {
      n_674X = SvalS;
      if ((0 == (3 & n_674X))) {
        goto L36448;}
      else {
        if ((3 == (3 & n_674X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_674X))) + -4))))>>2))))) {
            goto L36448;}
          else {
            goto L36449;}}
        else {
          goto L36449;}}}
      break;
    case 64 : {
      n_675X = SvalS;
      if ((0 == (3 & n_675X))) {
        goto L36519;}
      else {
        if ((3 == (3 & n_675X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_675X))) + -4))))>>2))))) {
            goto L36519;}
          else {
            goto L36520;}}
        else {
          goto L36520;}}}
      break;
    case 65 : {
      n_676X = SvalS;
      if ((0 == (3 & n_676X))) {
        goto L36590;}
      else {
        if ((3 == (3 & n_676X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_676X))) + -4))))>>2))))) {
            goto L36590;}
          else {
            goto L36593;}}
        else {
          goto L36593;}}}
      break;
    case 66 : {
      n_677X = SvalS;
      if ((0 == (3 & n_677X))) {
        goto L36666;}
      else {
        if ((3 == (3 & n_677X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_677X))) + -4))))>>2))))) {
            goto L36666;}
          else {
            goto L36667;}}
        else {
          goto L36667;}}}
      break;
    case 67 : {
      n_678X = SvalS;
      if ((0 == (3 & n_678X))) {
        goto L36737;}
      else {
        if ((3 == (3 & n_678X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_678X))) + -4))))>>2))))) {
            goto L36737;}
          else {
            goto L36740;}}
        else {
          goto L36740;}}}
      break;
    case 68 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 69 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 70 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 71 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 72 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 73 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 74 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 75 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 76 : {
      x_679X = SvalS;
      if ((0 == (3 & x_679X))) {
        if ((x_679X < 0)) {
          arg0K0 = x_679X;
          goto L23661;}
        else {
          goto L50675;}}
      else {
        v_680X = s48_bignum_test((((char *) (-3 + x_679X))));
        if ((-1 == v_680X)) {
          arg0K0 = x_679X;
          goto L23661;}
        else {
          goto L50675;}}}
      break;
    case 77 : {
      x_681X = SvalS;
      if ((0 == (3 & x_681X))) {
        goto L23571;}
      else {
        if ((3 == (3 & x_681X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_681X))) + -4))))>>2))))) {
            goto L23571;}
          else {
            goto L23574;}}
        else {
          goto L23574;}}}
      break;
    case 78 : {
      arg0K0 = (SvalS);
      goto L23661;}
      break;
    case 79 : {
      arg2_682X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = arg2_682X;
      arg0K1 = (SvalS);
      goto L23692;}
      break;
    case 80 : {
      arg2_683X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = arg2_683X;
      arg0K1 = (SvalS);
      goto L23692;}
      break;
    case 81 : {
      arg2_684X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = arg2_684X;
      arg0K1 = (SvalS);
      goto L23692;}
      break;
    case 82 : {
      x_685X = SvalS;
      if ((0 == (3 & x_685X))) {
        SvalS = (~ (3 | x_685X));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L24972;}
      else {
        if ((0 == (3 & x_685X))) {
          goto L36900;}
        else {
          if ((3 == (3 & x_685X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_685X))) + -4))))>>2))))) {
              goto L36900;}
            else {
              goto L36903;}}
          else {
            goto L36903;}}}}
      break;
    case 83 : {
      x_686X = SvalS;
      if ((0 == (3 & x_686X))) {
        x_687X = ((x_686X)>>2);
        if ((x_687X < 0)) {
          arg0K0 = (~ x_687X);
          goto L37007;}
        else {
          arg0K0 = x_687X;
          goto L37007;}}
      else {
        if ((0 == (3 & x_686X))) {
          goto L36990;}
        else {
          if ((3 == (3 & x_686X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_686X))) + -4))))>>2))))) {
              goto L36990;}
            else {
              goto L36993;}}
          else {
            goto L36993;}}}}
      break;
    case 84 : {
      arg2_688X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_689X = SvalS;
      if ((0 == (3 & (arg2_688X | y_689X)))) {
        SvalS = (arg2_688X & y_689X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L24972;}
      else {
        if ((0 == (3 & arg2_688X))) {
          goto L37108;}
        else {
          if ((3 == (3 & arg2_688X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_688X))) + -4))))>>2))))) {
              goto L37108;}
            else {
              goto L37117;}}
          else {
            goto L37117;}}}}
      break;
    case 85 : {
      arg2_690X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_691X = SvalS;
      if ((0 == (3 & (arg2_690X | y_691X)))) {
        SvalS = (arg2_690X | y_691X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L24972;}
      else {
        if ((0 == (3 & arg2_690X))) {
          goto L37287;}
        else {
          if ((3 == (3 & arg2_690X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_690X))) + -4))))>>2))))) {
              goto L37287;}
            else {
              goto L37296;}}
          else {
            goto L37296;}}}}
      break;
    case 86 : {
      arg2_692X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_693X = SvalS;
      if ((0 == (3 & (arg2_692X | y_693X)))) {
        SvalS = (arg2_692X ^ y_693X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L24972;}
      else {
        if ((0 == (3 & arg2_692X))) {
          goto L37466;}
        else {
          if ((3 == (3 & arg2_692X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_692X))) + -4))))>>2))))) {
              goto L37466;}
            else {
              goto L37475;}}
          else {
            goto L37475;}}}}
      break;
    case 87 : {
      arg2_694X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_695X = SvalS;
      if ((3 == (3 & y_695X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_695X))) + -4))))>>2))))) {
          v_696X = s48_bignum_test((((char *) (-3 + y_695X))));
          if ((1 == v_696X)) {
            code_697X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_697X)))))<<2)), code_697X, 24, 4);
            n_698X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_698X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 24;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = arg2_694X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_695X;
            arg0K0 = 2;
            goto L22779;}
          else {
            if ((0 == (3 & arg2_694X))) {
              if ((arg2_694X < 0)) {
                arg0K0 = -4;
                goto L33581;}
              else {
                arg0K0 = 0;
                goto L33581;}}
            else {
              if ((3 == (3 & arg2_694X))) {
                if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_694X))) + -4))))>>2))))) {
                  v_699X = s48_bignum_test((((char *) (-3 + arg2_694X))));
                  if ((1 == v_699X)) {
                    arg0K0 = 0;
                    goto L33597;}
                  else {
                    arg0K0 = -4;
                    goto L33597;}}
                else {
                  goto L33598;}}
              else {
                goto L33598;}}}}
        else {
          goto L44547;}}
      else {
        goto L44547;}}
      break;
    case 88 : {
      x_700X = SvalS;
      if ((9 == (255 & x_700X))) {
        arg0K0 = 5;
        goto L50736;}
      else {
        arg0K0 = 1;
        goto L50736;}}
      break;
    case 89 : {
      arg2_701X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((9 == (255 & arg2_701X))) {
        if ((9 == (255 & (SvalS)))) {
          x_702X = SvalS;
          if ((arg2_701X == x_702X)) {
            arg0K0 = 5;
            goto L41359;}
          else {
            arg0K0 = 1;
            goto L41359;}}
        else {
          goto L41330;}}
      else {
        goto L41330;}}
      break;
    case 90 : {
      arg2_703X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((9 == (255 & arg2_703X))) {
        if ((9 == (255 & (SvalS)))) {
          x_704X = SvalS;
          if ((arg2_703X < x_704X)) {
            arg0K0 = 5;
            goto L41251;}
          else {
            arg0K0 = 1;
            goto L41251;}}
        else {
          goto L41222;}}
      else {
        goto L41222;}}
      break;
    case 91 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L24972;}
      else {
        code_705X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_705X)))))<<2)), code_705X, 20, 4);
        n_706X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_706X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_707X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_707X;
        arg0K0 = 1;
        goto L22779;}}
      break;
    case 92 : {
      if ((0 == (3 & (SvalS)))) {
        x_708X = (((SvalS))>>2);
        if ((255 < x_708X)) {
          goto L37636;}
        else {
          if ((x_708X < 0)) {
            goto L37636;}
          else {
            SvalS = (9 + (((((x_708X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L24972;}}}
      else {
        code_709X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_709X)))))<<2)), code_709X, 20, 4);
        n_710X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_710X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_711X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_711X;
        arg0K0 = 1;
        goto L22779;}}
      break;
    case 93 : {
      x_712X = SvalS;
      if ((21 == x_712X)) {
        arg0K0 = 5;
        goto L50756;}
      else {
        arg0K0 = 1;
        goto L50756;}}
      break;
    case 94 : {
      x_713X = SvalS;
      type_714X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_713X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_713X))) + -4))))>>2))) == type_714X)) {
          arg0K0 = 5;
          goto L50770;}
        else {
          arg0K0 = 1;
          goto L50770;}}
      else {
        arg0K0 = 1;
        goto L50770;}}
      break;
    case 95 : {
      stob_715X = SvalS;
      type_716X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_715X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_715X))) + -4))))>>2))) == type_716X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_715X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          goto L27693;}}
      else {
        goto L27693;}}
      break;
    case 96 : {
      len_717X = *((unsigned char *) ((Scode_pointerS) + 1));s48_make_availableAgc((4 + (((len_717X)<<2))));
      type_718X = *((unsigned char *) ((Scode_pointerS) + 2));
      len_719X = ((len_717X)<<2);
      addr_720X = s48_allocate_small((4 + len_719X));
      *((long *) addr_720X) = (2 + (((((((len_719X)<<6)) + type_718X))<<2)));
      new_721X = 3 + (((long) (addr_720X + 4)));
      if ((len_717X < 1)) {
        goto L27814;}
      else {
        *((long *) ((((char *) (-3 + new_721X))) + (-4 + (((len_717X)<<2))))) = (SvalS);
        arg0K0 = (-2 + len_717X);
        goto L27798;}}
      break;
    case 97 : {
      p_722X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      len_723X = ((p_722X)>>2);s48_make_availableAgc((4 + (((len_723X)<<2))));
      type_724X = *((unsigned char *) ((Scode_pointerS) + 1));
      len_725X = ((len_723X)<<2);
      addr_726X = s48_allocate_small((4 + len_725X));
      *((long *) addr_726X) = (2 + (((((((len_725X)<<6)) + type_724X))<<2)));
      new_727X = 3 + (((long) (addr_726X + 4)));
      p_728X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_729X = ((p_728X)>>2);
      rest_list_730X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = (-1 + stack_nargs_729X);
      goto L27932;}
      break;
    case 98 : {
      stob_731X = SvalS;
      offset_732X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_733X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_731X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_731X))) + -4))))>>2))) == type_733X)) {
          SvalS = (*((long *) ((((char *) (-3 + stob_731X))) + (((offset_732X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          goto L28100;}}
      else {
        goto L28100;}}
      break;
    case 99 : {
      arg2_734X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      value_735X = SvalS;
      offset_736X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_737X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & arg2_734X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + arg2_734X))) + -4))))>>2))) == type_737X)) {
          if ((3 == (3 & arg2_734X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + arg2_734X))) + -4)))))) {
              v_738X = *((unsigned char *) ((Scode_pointerS) + 3));
              if ((0 == v_738X)) {
                goto L28233;}
              else {
                if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                  goto L28233;}
                else {
                  merged_arg0K0 = arg2_734X;
                  merged_arg0K1 = (((offset_736X)<<2));
                  merged_arg0K2 = value_735X;
                  proposal_d_write_return_tag = 0;
                  goto proposal_d_write;
                 proposal_d_write_return_0:
                  goto L28242;}}}
            else {
              goto L28243;}}
          else {
            goto L28243;}}
        else {
          goto L28243;}}
      else {
        goto L28243;}}
      break;
    case 100 : {
      arg2_739X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      init_740X = SvalS;
      type_741X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_739X))) {
        len_742X = ((arg2_739X)>>2);
        if ((len_742X < 0)) {
          goto L28476;}
        else {
          if ((4194304 < len_742X)) {
            goto L28476;}
          else {
            Stemp0S = init_740X;
            len_743X = ((len_742X)<<2);
            addr_744X = s48_allocate_tracedAgc((4 + len_743X));
            if ((addr_744X == NULL)) {
              arg0K0 = 1;
              goto L28493;}
            else {
              *((long *) addr_744X) = (2 + (((((((len_743X)<<6)) + type_741X))<<2)));
              arg0K0 = (3 + (((long) (addr_744X + 4))));
              goto L28493;}}}}
      else {
        code_745X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_745X)))))<<2)), code_745X, 20, 8);
        n_746X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_746X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((type_741X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_739X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = init_740X;
        arg0K0 = 3;
        goto L22779;}}
      break;
    case 101 : {
      arg2_747X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      index_748X = SvalS;
      type_749X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & index_748X))) {
        if ((3 == (3 & arg2_747X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg2_747X))) + -4))))>>2))) == type_749X)) {
            len_750X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_747X))) + -4))))>>8))))>>2);
            index_751X = ((index_748X)>>2);
            if ((index_751X < 0)) {
              goto L28814;}
            else {
              if ((index_751X < len_750X)) {
                v_752X = *((unsigned char *) ((Scode_pointerS) + 2));
                if ((0 == v_752X)) {
                  goto L28804;}
                else {
                  if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                    goto L28804;}
                  else {
                    merged_arg0K0 = arg2_747X;
                    merged_arg0K1 = index_748X;
                    proposal_d_read_return_tag = 0;
                    goto proposal_d_read;
                   proposal_d_read_return_0:
                    v_753X = proposal_d_read0_return_value;
                    arg0K0 = v_753X;
                    goto L28813;}}}
              else {
                goto L28814;}}}
          else {
            goto L28769;}}
        else {
          goto L28769;}}
      else {
        goto L28769;}}
      break;
    case 102 : {
      arg2_754X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_755X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      value_756X = SvalS;
      type_757X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_754X))) {
        if ((3 == (3 & arg3_755X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg3_755X))) + -4))))>>2))) == type_757X)) {
            if ((3 == (3 & arg3_755X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_755X))) + -4)))))) {
                len_758X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_755X))) + -4))))>>8))))>>2);
                index_759X = ((arg2_754X)>>2);
                if ((index_759X < 0)) {
                  goto L29133;}
                else {
                  if ((index_759X < len_758X)) {
                    v_760X = *((unsigned char *) ((Scode_pointerS) + 2));
                    if ((0 == v_760X)) {
                      goto L29123;}
                    else {
                      if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                        goto L29123;}
                      else {
                        merged_arg0K0 = arg3_755X;
                        merged_arg0K1 = arg2_754X;
                        merged_arg0K2 = value_756X;
                        proposal_d_write_return_tag = 1;
                        goto proposal_d_write;
                       proposal_d_write_return_1:
                        goto L29132;}}}
                  else {
                    goto L29133;}}}
              else {
                goto L29086;}}
            else {
              goto L29086;}}
          else {
            goto L29086;}}
        else {
          goto L29086;}}
      else {
        goto L29086;}}
      break;
    case 103 : {
      arg2_761X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (arg2_761X | (SvalS))))) {
        len_762X = ((arg2_761X)>>2);
        init_763X = (((SvalS))>>2);
        if ((len_762X < 0)) {
          goto L37719;}
        else {
          if ((4194304 < ((((3 + len_762X))>>2)))) {
            goto L37719;}
          else {
            addr_764X = s48_allocate_untracedAgc((4 + len_762X));
            if ((addr_764X == NULL)) {
              arg0K0 = 1;
              goto L37732;}
            else {
              *((long *) addr_764X) = (74 + (((len_762X)<<8)));
              arg0K0 = (3 + (((long) (addr_764X + 4))));
              goto L37732;}}}}
      else {
        code_765X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_765X)))))<<2)), code_765X, 20, 4);
        n_766X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_766X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_761X;
        x_767X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_767X;
        arg0K0 = 2;
        goto L22779;}}
      break;
    case 104 : {
      obj_768X = SvalS;
      if ((3 == (3 & obj_768X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + obj_768X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          goto L49839;}}
      else {
        goto L49839;}}
      break;
    case 105 : {
      arg2_769X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_769X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_769X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_770X = (((SvalS))>>2);
            len_771X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_769X))) + -4))))>>8);
            if ((index_770X < 0)) {
              goto L46046;}
            else {
              if ((index_770X < len_771X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_769X))) + index_770X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L24972;}
              else {
                goto L46046;}}}
          else {
            goto L49400;}}
        else {
          goto L49400;}}
      else {
        goto L49400;}}
      break;
    case 106 : {
      arg2_772X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_773X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_773X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_773X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_772X | (SvalS))))) {
            index_774X = ((arg2_772X)>>2);
            Kchar_775X = (((SvalS))>>2);
            if ((3 == (3 & arg3_773X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_773X))) + -4)))))) {
                len_776X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_773X))) + -4))))>>8);
                if ((index_774X < 0)) {
                  goto L43116;}
                else {
                  if ((index_774X < len_776X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_773X))) + index_774X)) = Kchar_775X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L24972;}
                  else {
                    goto L43116;}}}
              else {
                goto L43095;}}
            else {
              goto L43095;}}
          else {
            goto L47948;}}
        else {
          goto L47948;}}
      else {
        goto L47948;}}
      break;
    case 107 : {
      arg2_777X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_777X))) {
        if ((9 == (255 & (SvalS)))) {
          len_778X = ((arg2_777X)>>2);
          init_779X = ((((SvalS))>>8));
          if ((len_778X < 0)) {
            goto L37931;}
          else {
            if ((4194304 < ((((4 + len_778X))>>2)))) {
              goto L37931;}
            else {
              len_780X = 1 + len_778X;
              addr_781X = s48_allocate_untracedAgc((4 + len_780X));
              if ((addr_781X == NULL)) {
                arg0K0 = 1;
                goto L37944;}
              else {
                *((long *) addr_781X) = (70 + (((len_780X)<<8)));
                arg0K0 = (3 + (((long) (addr_781X + 4))));
                goto L37944;}}}}
        else {
          goto L46143;}}
      else {
        goto L46143;}}
      break;
    case 108 : {
      obj_782X = SvalS;
      if ((3 == (3 & obj_782X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_782X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          goto L49905;}}
      else {
        goto L49905;}}
      break;
    case 109 : {
      arg2_783X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_783X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_783X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_784X = (((SvalS))>>2);
            len_785X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_783X))) + -4))))>>8));
            if ((index_784X < 0)) {
              goto L46224;}
            else {
              if ((index_784X < len_785X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_783X))) + index_784X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L24972;}
              else {
                goto L46224;}}}
          else {
            goto L49514;}}
        else {
          goto L49514;}}
      else {
        goto L49514;}}
      break;
    case 110 : {
      arg2_786X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_787X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_787X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_787X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_786X))) {
            if ((9 == (255 & (SvalS)))) {
              index_788X = ((arg2_786X)>>2);
              Kchar_789X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_787X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_787X))) + -4)))))) {
                  len_790X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_787X))) + -4))))>>8));
                  if ((index_788X < 0)) {
                    goto L43303;}
                  else {
                    if ((index_788X < len_790X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_787X))) + index_788X)) = (Kchar_789X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L24972;}
                    else {
                      goto L43303;}}}
                else {
                  goto L43282;}}
              else {
                goto L43282;}}
            else {
              goto L48094;}}
          else {
            goto L48094;}}
        else {
          goto L48094;}}
      else {
        goto L48094;}}
      break;
    case 111 : {s48_make_availableAgc(12);
      obj_791X = SvalS;
      if ((3 == (3 & obj_791X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_791X))) + -4))))>>2))))) {
          table_792X = Sthe_symbol_tableS;
          string_793X = SvalS;
          n_794X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_793X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L20949;}
        else {
          goto L33775;}}
      else {
        goto L33775;}}
      break;
    case 112 : {
      obj_795X = SvalS;
      if ((3 == (3 & obj_795X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_795X))) + -4))))>>2))))) {
          x_796X = SvalS;
          descriptor_797X = *((long *) ((((char *) (-3 + x_796X))) + 4));
          if ((17 == (255 & descriptor_797X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_796X))) + 4))))) {
              arg0K0 = 5;
              goto L47578;}
            else {
              arg0K0 = 1;
              goto L47578;}}
          else {
            arg0K0 = 5;
            goto L47578;}}
        else {
          goto L47559;}}
      else {
        goto L47559;}}
      break;
    case 113 : {
      arg2_798X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_798X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_798X))) + -4))))>>2))))) {
          x_799X = SvalS;
          if ((1 == x_799X)) {
            goto L42861;}
          else {
            if ((5 == x_799X)) {
              goto L42861;}
            else {
              goto L42868;}}}
        else {
          goto L42868;}}
      else {
        goto L42868;}}
      break;
    case 114 : {
      x_800X = SvalS;
      if ((3 == (3 & x_800X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_800X))) + -4)))))) {
          arg0K0 = 1;
          goto L50837;}
        else {
          arg0K0 = 5;
          goto L50837;}}
      else {
        arg0K0 = 5;
        goto L50837;}}
      break;
    case 115 : {
      x_801X = SvalS;
      if ((3 == (3 & x_801X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_801X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_801X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_801X))) + -4))));
          goto L46322;}
        else {
          goto L46322;}}
      else {
        goto L46322;}}
      break;
    case 116 : {s48_make_availableAgc(288);
      arg2_802X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        mode_803X = (((SvalS))>>2);
        if ((1 == mode_803X)) {
          goto L38386;}
        else {
          if ((2 == mode_803X)) {
            goto L38386;}
          else {
            if ((3 == mode_803X)) {
              goto L38386;}
            else {
              if ((4 == mode_803X)) {
                goto L38386;}
              else {
                arg0K0 = 5;
                goto L38160;}}}}}
      else {
        code_804X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_804X)))))<<2)), code_804X, 20, 4);
        n_805X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_805X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_802X;
        x_806X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_806X;
        arg0K0 = 2;
        goto L22779;}}
      break;
    case 117 : {s48_make_availableAgc(264);
      obj_807X = SvalS;
      if ((3 == (3 & obj_807X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_807X))) + -4))))>>2))))) {
          channel_808X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_808X))))))) {
            code_809X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_809X)))))<<2)), code_809X, 20, 4);
            n_810X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_810X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 20;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = channel_808X;
            arg0K0 = 1;
            goto L22779;}
          else {
            status_811X = close_channelB(channel_808X);
            if ((status_811X == NO_ERRORS)) {
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L24972;}
            else {
              code_812X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_812X)))))<<2)), code_812X, 96, 4);
              n_813X = *((unsigned char *) (Scode_pointerS));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((n_813X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = 96;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = channel_808X;
              merged_arg0K0 = status_811X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 0;
              goto get_error_string;
             get_error_string_return_0:
              x_814X = get_error_string0_return_value;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = x_814X;
              arg0K0 = 2;
              goto L22779;}}}
        else {
          goto L48222;}}
      else {
        goto L48222;}}
      break;
    case 118 : {s48_make_availableAgc(264);
      arg2_815X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_816X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_817X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_818X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg5_818X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg5_818X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg3_816X | arg2_815X)))) {
            x_819X = SvalS;
            if ((1 == x_819X)) {
              goto L46465;}
            else {
              if ((5 == x_819X)) {
                goto L46465;}
              else {
                goto L46476;}}}
          else {
            goto L46476;}}
        else {
          goto L46476;}}
      else {
        goto L46476;}}
      break;
    case 119 : {s48_make_availableAgc(264);
      arg2_820X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_821X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_822X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg4_822X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg4_822X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_820X | (SvalS))))) {
            start_823X = ((arg2_820X)>>2);
            count_824X = (((SvalS))>>2);
            v_825X = 8 == (*((long *) (((char *) (-3 + arg4_822X)))));
            if (v_825X) {
              if ((3 == (3 & arg3_821X))) {
                if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_821X))) + -4))))>>2))))) {
                  goto L38985;}
                else {
                  goto L39192;}}
              else {
                goto L39192;}}
            else {
              goto L39039;}}
          else {
            goto L46695;}}
        else {
          goto L46695;}}
      else {
        goto L46695;}}
      break;
    case 120 : {s48_make_availableAgc(264);
      obj_826X = SvalS;
      if ((3 == (3 & obj_826X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_826X))) + -4))))>>2))))) {
          channel_827X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_827X))))))) {
            code_828X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_828X)))))<<2)), code_828X, 20, 4);
            n_829X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_829X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 20;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = channel_827X;
            arg0K0 = 1;
            goto L22779;}
          else {
            readyP_830X = ps_check_fd(((((*((long *) ((((char *) (-3 + channel_827X))) + 8))))>>2)), (4 == (*((long *) (((char *) (-3 + channel_827X)))))), &status_831X);
            if ((status_831X == NO_ERRORS)) {
              if (readyP_830X) {
                arg0K0 = 5;
                goto L42459;}
              else {
                arg0K0 = 1;
                goto L42459;}}
            else {
              code_832X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_832X)))))<<2)), code_832X, 96, 4);
              n_833X = *((unsigned char *) (Scode_pointerS));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((n_833X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = 96;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = channel_827X;
              merged_arg0K0 = status_831X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 1;
              goto get_error_string;
             get_error_string_return_1:
              x_834X = get_error_string0_return_value;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = x_834X;
              arg0K0 = 2;
              goto L22779;}}}
        else {
          goto L47640;}}
      else {
        goto L47640;}}
      break;
    case 121 : {
      obj_835X = SvalS;
      if ((3 == (3 & obj_835X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_835X))) + -4))))>>2))))) {
          channel_836X = SvalS;
          head_837X = Spending_channels_headS;
          if ((1 == head_837X)) {
            addr_838X = (((char *) (-3 + channel_836X))) + 16;S48_WRITE_BARRIER(channel_836X, addr_838X, 1);
            *((long *) addr_838X) = 1;
            n_839X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_836X))) + 8))))>>2)));
            arg0K0 = (((n_839X)<<2));
            goto L46859;}
          else {
            if ((channel_836X == head_837X)) {
              channel_840X = Spending_channels_headS;
              next_841X = *((long *) ((((char *) (-3 + channel_840X))) + 12));
              Spending_channels_headS = next_841X;
              addr_842X = (((char *) (-3 + channel_840X))) + 12;S48_WRITE_BARRIER(channel_840X, addr_842X, 1);
              *((long *) addr_842X) = 1;
              if ((1 == next_841X)) {
                Spending_channels_tailS = 1;
                goto L11355;}
              else {
                goto L11355;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_837X))) + 12)));
              arg0K1 = head_837X;
              goto L11361;}}}
        else {
          goto L46842;}}
      else {
        goto L46842;}}
      break;
    case 122 : {s48_make_availableAgc(((((3 * (Snumber_of_channelsS)))<<2)));
      arg0K0 = (-1 + (Snumber_of_channelsS));
      arg0K1 = 25;
      goto L16944;}
      break;
    case 123 : {
      SvalS = (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 124 : {
      proposal_843X = SvalS;
      if ((1 == proposal_843X)) {
        goto L39325;}
      else {
        if ((3 == (3 & proposal_843X))) {
          if ((2 == (31 & ((((*((long *) ((((char *) (-3 + proposal_843X))) + -4))))>>2))))) {
            if ((4 == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proposal_843X))) + -4))))>>8))))>>2)))) {
              if ((1 == (*((long *) (((char *) (-3 + proposal_843X))))))) {
                goto L39325;}
              else {
                goto L39356;}}
            else {
              goto L39356;}}
          else {
            goto L39356;}}
        else {
          goto L39356;}}}
      break;
    case 125 : {
      proposal_844X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
      if ((1 == proposal_844X)) {
        code_845X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_845X)))))<<2)), code_845X, 104, 4);
        n_846X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_846X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 104;
        arg0K0 = 0;
        goto L22779;}
      else {GET_PROPOSAL_LOCK();
        log_847X = *((long *) ((((char *) (-3 + proposal_844X))) + 4));
        arg0K0 = 0;
        goto L10133;}}
      break;
    case 126 : {
      stob_848X = SvalS;
      offset_849X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_850X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_848X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_848X))) + -4))))>>2))) == type_850X)) {
          x_851X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
          if ((1 == x_851X)) {
            arg0K0 = (*((long *) ((((char *) (-3 + stob_848X))) + (((offset_849X)<<2)))));
            goto L29463;}
          else {
            merged_arg0K0 = stob_848X;
            merged_arg0K1 = (((offset_849X)<<2));
            proposal_d_read_return_tag = 1;
            goto proposal_d_read;
           proposal_d_read_return_1:
            v_852X = proposal_d_read0_return_value;
            arg0K0 = v_852X;
            goto L29463;}}
        else {
          goto L29464;}}
      else {
        goto L29464;}}
      break;
    case 127 : {
      arg2_853X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_854X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_855X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_856X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & ((arg4_855X | arg2_853X) | (SvalS))))) {
        from_index_857X = ((arg4_855X)>>2);
        to_index_858X = ((arg2_853X)>>2);
        count_859X = (((SvalS))>>2);
        v_860X = *((unsigned char *) ((Scode_pointerS) + 1));
        merged_arg0K0 = arg5_856X;
        merged_arg0K1 = from_index_857X;
        merged_arg0K2 = count_859X;
        okay_copy_argsP_return_tag = 0;
        goto okay_copy_argsP;
       okay_copy_argsP_return_0:
        v_861X = okay_copy_argsP0_return_value;
        if (v_861X) {
          merged_arg0K0 = arg3_854X;
          merged_arg0K1 = to_index_858X;
          merged_arg0K2 = count_859X;
          okay_copy_argsP_return_tag = 1;
          goto okay_copy_argsP;
         okay_copy_argsP_return_1:
          v_862X = okay_copy_argsP0_return_value;
          if (v_862X) {
            if ((3 == (3 & arg3_854X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_854X))) + -4)))))) {
                if ((count_859X < 0)) {
                  goto L29573;}
                else {
                  if ((0 == v_860X)) {
                    goto L29633;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L29633;}
                    else {
                      arg0K0 = 4096;
                      arg0K1 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 12)));
                      goto L29737;}}}}
              else {
                goto L29573;}}
            else {
              goto L29573;}}
          else {
            goto L29573;}}
        else {
          goto L29573;}}
      else {
        code_863X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_863X)))))<<2)), code_863X, 20, 4);
        n_864X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_864X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg5_856X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_855X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_854X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_853X;
        x_865X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_865X;
        arg0K0 = 5;
        goto L22779;}}
      break;
    case 128 : {
      arg2_866X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_866X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_866X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_867X = (((SvalS))>>2);
            len_868X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_866X))) + -4))))>>8);
            if ((index_867X < 0)) {
              goto L29946;}
            else {
              if ((index_867X < len_868X)) {
                x_869X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                if ((1 == x_869X)) {
                  arg0K0 = ((((*((unsigned char *) ((((char *) (-3 + arg2_866X))) + index_867X))))<<2));
                  goto L29945;}
                else {
                  index_870X = ((index_867X)<<2);
                  log_871X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                  arg0K0 = 0;
                  goto L15135;}}
              else {
                goto L29946;}}}
          else {
            goto L32969;}}
        else {
          goto L32969;}}
      else {
        goto L32969;}}
      break;
    case 129 : {
      arg2_872X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_873X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_873X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_873X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_872X))) {
            index_874X = ((arg2_872X)>>2);
            byte_875X = SvalS;
            if ((3 == (3 & arg3_873X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_873X))) + -4)))))) {
                if ((0 == (3 & byte_875X))) {
                  len_876X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_873X))) + -4))))>>8);
                  if ((index_874X < 0)) {
                    goto L30088;}
                  else {
                    if ((index_874X < len_876X)) {
                      x_877X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                      if ((1 == x_877X)) {
                        *((unsigned char *) ((((char *) (-3 + arg3_873X))) + index_874X)) = (((byte_875X)>>2));
                        goto L30087;}
                      else {
                        index_878X = ((index_874X)<<2);
                        log_879X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                        arg0K0 = 0;
                        goto L15311;}}
                    else {
                      goto L30088;}}}
                else {
                  goto L30053;}}
              else {
                goto L30053;}}
            else {
              goto L30053;}}
          else {
            goto L33090;}}
        else {
          goto L33090;}}
      else {
        goto L33090;}}
      break;
    case 130 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 131 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 132 : {
      arg_880X = SvalS;
      code_881X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_881X)))))<<2)), code_881X, 60, 4);
      n_882X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_882X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 60;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg_880X;
      arg0K0 = 1;
      goto L22779;}
      break;
    case 133 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 134 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 135 : {s48_make_availableAgc(264);
      arg2_883X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_884X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_885X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg4_885X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_885X))) + -4))))>>2))))) {
          if ((3 == (3 & arg2_883X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_883X))) + -4))))>>2))))) {
              obj_886X = SvalS;
              if ((3 == (3 & obj_886X))) {
                if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_886X))) + -4))))>>2))))) {
                  undumpables_887X = SvalS;
                  port_888X = ps_open_output_file((((char *)(((char *) (-3 + arg4_885X))))), &status_889X);
                  if ((status_889X == NO_ERRORS)) {
                    status_890X = ps_write_string((((char *)(((char *) (-3 + arg2_883X))))), port_888X);
                    if ((status_890X == NO_ERRORS)) {
                      status_891X = s48_write_image(arg3_884X, undumpables_887X, port_888X);
                      if ((status_891X == NO_ERRORS)) {
                        status_892X = ps_close(port_888X);
                        if ((status_892X == NO_ERRORS)) {
                          SvalS = 13;
                          Scode_pointerS = ((Scode_pointerS) + 1);
                          arg1K0 = (Scode_pointerS);
                          goto L24972;}
                        else {
                          arg0K0 = 24;
                          arg0K1 = status_892X;
                          goto L43595;}}
                      else {
                        status_893X = ps_close(port_888X);
                        if ((status_893X == NO_ERRORS)) {
                          arg0K0 = 24;
                          arg0K1 = status_891X;
                          goto L43595;}
                        else {
                          ps_write_string("Unable to close image file", (stderr));
                          { long ignoreXX;
                          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                          arg0K0 = 24;
                          arg0K1 = status_891X;
                          goto L43595;}}}
                    else {
                      status_894X = ps_close(port_888X);
                      if ((status_894X == NO_ERRORS)) {
                        arg0K0 = 24;
                        arg0K1 = status_890X;
                        goto L43595;}
                      else {
                        ps_write_string("Unable to close image file", (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg0K0 = 24;
                        arg0K1 = status_890X;
                        goto L43595;}}}
                  else {
                    arg0K0 = 10;
                    arg0K1 = status_889X;
                    goto L43595;}}
                else {
                  goto L48327;}}
              else {
                goto L48327;}}
            else {
              goto L48327;}}
          else {
            goto L48327;}}
        else {
          goto L48327;}}
      else {
        goto L48327;}}
      break;
    case 136 : {
      SvalS = 13;s48_collect();
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 137 : {
      obj_895X = SvalS;
      if ((3 == (3 & obj_895X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_895X))) + -4))))>>2))))) {
          x_896X = SvalS;
          n_897X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_896X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L49080;}
        else {
          goto L49057;}}
      else {
        goto L49057;}}
      break;
    case 138 : {s48_make_availableAgc(24);
      arg2_898X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      proc_899X = SvalS;
      if ((3 == (3 & arg2_898X))) {
        if ((3 == (3 & proc_899X))) {
          if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_899X))) + -4))))>>2))))) {GET_PROPOSAL_LOCK();
            addr_900X = s48_allocate_small(12);
            *((long *) addr_900X) = 2050;
            x_901X = 3 + (((long) (addr_900X + 4)));
            *((long *) (((char *) (-3 + x_901X)))) = arg2_898X;
            *((long *) ((((char *) (-3 + x_901X))) + 4)) = proc_899X;
            b_902X = SHARED_REF((Sfinalizer_alistS));
            addr_903X = s48_allocate_small(12);
            *((long *) addr_903X) = 2050;
            x_904X = 3 + (((long) (addr_903X + 4)));
            *((long *) (((char *) (-3 + x_904X)))) = x_901X;
            *((long *) ((((char *) (-3 + x_904X))) + 4)) = b_902X;SHARED_SETB((Sfinalizer_alistS), x_904X);RELEASE_PROPOSAL_LOCK();
            SvalS = 13;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L24972;}
          else {
            goto L43886;}}
        else {
          goto L43886;}}
      else {
        goto L43886;}}
      break;
    case 139 : {
      arg2_905X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_905X))) {
        key_906X = ((arg2_905X)>>2);
        other_907X = SvalS;
        if ((5 == key_906X)) {
          SvalS = (-4 & other_907X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          if ((0 == key_906X)) {
            x_908X = s48_available();
            SvalS = (((x_908X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L24972;}
          else {
            if ((1 == key_906X)) {
              bytes_909X = s48_heap_size();
              SvalS = (-4 & (3 + bytes_909X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L24972;}
            else {
              if ((2 == key_906X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L24972;}
              else {
                if ((3 == key_906X)) {
                  x_910X = s48_gc_count();
                  SvalS = (((x_910X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L24972;}
                else {
                  if ((4 == key_906X)) {
                    code_911X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_911X)))))<<2)), code_911X, 56, 4);
                    n_912X = *((unsigned char *) (Scode_pointerS));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((n_912X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = 56;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((key_906X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_907X;
                    arg0K0 = 2;
                    goto L22779;}
                  else {
                    code_913X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_913X)))))<<2)), code_913X, 68, 4);
                    n_914X = *((unsigned char *) (Scode_pointerS));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((n_914X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = 68;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((key_906X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_907X;
                    arg0K0 = 2;
                    goto L22779;}}}}}}}
      else {
        code_915X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_915X)))))<<2)), code_915X, 20, 4);
        n_916X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_916X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_905X;
        x_917X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_917X;
        arg0K0 = 2;
        goto L22779;}}
      break;
    case 140 : {
      if ((0 == (3 & (SvalS)))) {
        type_918X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L39789;}
      else {
        code_919X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_919X)))))<<2)), code_919X, 20, 4);
        n_920X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_920X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_921X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_921X;
        arg0K0 = 1;
        goto L22779;}}
      break;
    case 141 : {
      type_922X = SvalS;
      arg2K0 = 1;
      arg0K1 = type_922X;
      goto L39877;}
      break;
    case 142 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 143 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 144 : {
      val_923X = SHARED_REF((Ssession_dataS));
      SvalS = val_923X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 145 : {SHARED_SETB((Ssession_dataS), (SvalS));
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 146 : {
      obj_924X = SvalS;
      if ((3 == (3 & obj_924X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_924X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 169)) {
            goto L27025;}
          else {
            temp_925X = SHARED_REF((Sexception_handlersS));SHARED_SETB((Sexception_handlersS), (SvalS));
            SvalS = temp_925X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L24972;}}
        else {
          goto L27025;}}
      else {
        goto L27025;}}
      break;
    case 147 : {
      data_926X = 3 + (((long) (SstackS)));
      SstackS = ((SstackS) + 20);
      pc_927X = *((long *) ((((char *) (-3 + data_926X))) + 4));
      code_928X = *((long *) ((((char *) (-3 + data_926X))) + 8));
      size_929X = *((long *) ((((char *) (-3 + data_926X))) + 16));
      Slast_code_calledS = code_928X;
      Scode_pointerS = ((((char *) (-3 + code_928X))) + ((((pc_927X)>>2)) + (((size_929X)>>2))));
      arg1K0 = (Scode_pointerS);
      goto L24972;}
      break;
    case 148 : {
      obj_930X = SvalS;
      if ((3 == (3 & obj_930X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_930X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 5)) {
            goto L25571;}
          else {
            temp_931X = SHARED_REF((Sinterrupt_handlersS));SHARED_SETB((Sinterrupt_handlersS), (SvalS));
            SvalS = temp_931X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L24972;}}
        else {
          goto L25571;}}
      else {
        goto L25571;}}
      break;
    case 149 : {
      old_932X = Senabled_interruptsS;
      p_933X = SvalS;
      Senabled_interruptsS = (((p_933X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Sstack_limitS = (Sreal_stack_limitS);
        if ((s48_Spending_eventsPS)) {
          s48_Sstack_limitS = (((char *) -1));
          goto L50961;}
        else {
          goto L50961;}}
      else {
        s48_Sstack_limitS = (((char *) -1));
        goto L50961;}}
      break;
    case 150 : {
      x_934X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      p_935X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      Senabled_interruptsS = (((p_935X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Sstack_limitS = (Sreal_stack_limitS);
        if ((s48_Spending_eventsPS)) {
          s48_Sstack_limitS = (((char *) -1));
          goto L8904;}
        else {
          goto L8904;}}
      else {
        s48_Sstack_limitS = (((char *) -1));
        goto L8904;}}
      break;
    case 151 : {
      if ((0 == (3 & (SvalS)))) {
        p_936X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          s48_Sstack_limitS = (Sreal_stack_limitS);
          if ((s48_Spending_eventsPS)) {
            s48_Sstack_limitS = (((char *) -1));
            goto L50074;}
          else {
            goto L50074;}}
        else {
          s48_Sstack_limitS = (((char *) -1));
          goto L50074;}}
      else {
        code_937X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_937X)))))<<2)), code_937X, 20, 4);
        n_938X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_938X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_939X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_939X;
        arg0K0 = 1;
        goto L22779;}}
      break;
    case 152 : {
      arg2_940X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_940X))) {
        x_941X = SvalS;
        if ((1 == x_941X)) {
          goto L47811;}
        else {
          if ((5 == x_941X)) {
            goto L47811;}
          else {
            goto L47816;}}}
      else {
        goto L47816;}}
      break;
    case 153 : {
      p_942X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      nargs_943X = ((p_942X)>>2);
      p_944X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      rest_list_945X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((14 < nargs_943X)) {
        code_946X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_946X)))))<<2)), code_946X, 76, 4);
        n_947X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_947X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 76;
        x_948X = *((long *) ((SstackS) + (-4 + (-4 & p_944X))));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_948X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = nargs_943X;
        arg0K0 = 2;
        goto L22779;}
      else {
        arg0K0 = rest_list_945X;
        goto L30319;}}
      break;
    case 154 : {s48_make_availableAgc(20);
      arg2_949X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_949X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_949X))) + -4))))>>2))))) {
          x_950X = SvalS;
          if ((1 == x_950X)) {
            goto L47052;}
          else {
            if ((5 == x_950X)) {
              goto L47052;}
            else {
              goto L47057;}}}
        else {
          goto L47057;}}
      else {
        goto L47057;}}
      break;
    case 155 : {
      arg2_951X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_951X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_951X))) + -4))))>>2))))) {
          x_952X = SvalS;
          if ((1 == x_952X)) {
            goto L32151;}
          else {
            if ((5 == x_952X)) {
              goto L32151;}
            else {
              goto L32156;}}}
        else {
          goto L32156;}}
      else {
        goto L32156;}}
      break;
    case 156 : {
      arg2_953X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_953X))) {
        option_954X = ((arg2_953X)>>2);
        other_955X = SvalS;
        if ((2 == option_954X)) {
          x_956X = CHEAP_TIME();
          SvalS = (((x_956X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          if ((0 == option_954X)) {
            seconds_957X = s48_run_time(&mseconds_958X);
            arg0K0 = option_954X;
            arg0K1 = seconds_957X;
            arg0K2 = mseconds_958X;
            goto L48975;}
          else {
            if ((1 == option_954X)) {
              seconds_959X = s48_real_time(&mseconds_960X);
              arg0K0 = option_954X;
              arg0K1 = seconds_959X;
              arg0K2 = mseconds_960X;
              goto L48975;}
            else {
              code_961X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_961X)))))<<2)), code_961X, 68, 4);
              n_962X = *((unsigned char *) (Scode_pointerS));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((n_962X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = 68;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((option_954X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = other_955X;
              arg0K0 = 2;
              goto L22779;}}}}
      else {
        code_963X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_963X)))))<<2)), code_963X, 20, 4);
        n_964X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_964X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_953X;
        x_965X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_965X;
        arg0K0 = 2;
        goto L22779;}}
      break;
    case 157 : {
      arg2_966X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_966X))) {
        key_967X = ((arg2_966X)>>2);
        value_968X = SvalS;
        status_969X = s48_extended_vm(key_967X, value_968X);
        if ((0 == status_969X)) {
          SvalS = (s48_Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          if ((1 == status_969X)) {
            code_970X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_970X)))))<<2)), code_970X, 88, 4);
            n_971X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_971X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 88;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((key_967X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_968X;
            arg0K0 = 2;
            goto L22779;}
          else {
            code_972X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_972X)))))<<2)), code_972X, 92, 4);
            n_973X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_973X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 92;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((key_967X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_968X;
            arg0K0 = 2;
            goto L22779;}}}
      else {
        code_974X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_974X)))))<<2)), code_974X, 20, 4);
        n_975X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_975X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_966X;
        x_976X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_976X;
        arg0K0 = 2;
        goto L22779;}}
      break;
    case 158 : {
      arg2_977X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_978X = SvalS;
      Senabled_interruptsS = -1;
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Sstack_limitS = (Sreal_stack_limitS);
        if ((s48_Spending_eventsPS)) {
          s48_Sstack_limitS = (((char *) -1));
          goto L50995;}
        else {
          goto L50995;}}
      else {
        s48_Sstack_limitS = (((char *) -1));
        goto L50995;}}
      break;
    case 159 : {
      arg2_979X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_979X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_979X))) + -4))))>>2))))) {
          obj_980X = SvalS;
          if ((3 == (3 & obj_980X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_980X))) + -4))))>>2))))) {
              x_981X = SvalS;
              len_982X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_979X))) + -4))))>>8);
              if ((len_982X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_981X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_981X))), (void *)(((char *) (-3 + arg2_979X))),len_982X)))) {
                  arg0K0 = 5;
                  goto L40999;}
                else {
                  arg0K0 = 1;
                  goto L40999;}}
              else {
                arg0K0 = 1;
                goto L40999;}}
            else {
              goto L40964;}}
          else {
            goto L40964;}}
        else {
          goto L40964;}}
      else {
        goto L40964;}}
      break;
    case 160 : {s48_make_availableAgc((4 + (-4 & (4 + ((((SvalS))>>2))))));
      arg2_983X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        n_984X = (((SvalS))>>2);
        if ((3 == (3 & arg2_983X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_983X))) + -4))))>>2))))) {
            goto L40167;}
          else {
            goto L40119;}}
        else {
          goto L40119;}}
      else {
        code_985X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_985X)))))<<2)), code_985X, 20, 4);
        n_986X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_986X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_983X;
        x_987X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_987X;
        arg0K0 = 2;
        goto L22779;}}
      break;
    case 161 : {
      arg2_988X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      list_989X = SvalS;
      arg0K0 = list_989X;
      arg0K1 = list_989X;
      arg2K2 = 1;
      goto L42538;}
      break;
    case 162 : {
      if ((529 == (SvalS))) {
        code_990X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_990X)))))<<2)), code_990X, 0, 4);
        n_991X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_991X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 0;
        arg0K0 = 0;
        goto L22779;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L24972;}}
      break;
    case 163 : {
      arg2_992X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_993X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        index_994X = (((SvalS))>>2);
        if ((3 == (3 & arg3_993X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg3_993X))) + -4))))>>2))))) {
            if ((arg2_992X == (*((long *) (((char *) (-3 + arg3_993X))))))) {
              len_995X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_993X))) + -4))))>>8))))>>2);
              if ((index_994X < 0)) {
                goto L30709;}
              else {
                if ((index_994X < len_995X)) {
                  v_996X = *((unsigned char *) ((Scode_pointerS) + 1));
                  if ((0 == v_996X)) {
                    goto L30699;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L30699;}
                    else {
                      merged_arg0K0 = arg3_993X;
                      merged_arg0K1 = (((index_994X)<<2));
                      proposal_d_read_return_tag = 2;
                      goto proposal_d_read;
                     proposal_d_read_return_2:
                      v_997X = proposal_d_read0_return_value;
                      arg0K0 = v_997X;
                      goto L30708;}}}
                else {
                  goto L30709;}}}
            else {
              goto L30729;}}
          else {
            goto L30729;}}
        else {
          goto L30729;}}
      else {
        code_998X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_998X)))))<<2)), code_998X, 20, 4);
        n_999X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_999X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_993X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_992X;
        x_1000X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_1000X;
        arg0K0 = 3;
        goto L22779;}}
      break;
    case 164 : {
      arg2_1001X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_1002X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_1003X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_1001X))) {
        index_1004X = ((arg2_1001X)>>2);
        value_1005X = SvalS;
        if ((3 == (3 & arg4_1003X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg4_1003X))) + -4))))>>2))))) {
            if ((arg3_1002X == (*((long *) (((char *) (-3 + arg4_1003X))))))) {
              if ((3 == (3 & arg4_1003X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_1003X))) + -4)))))) {
                  len_1006X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_1003X))) + -4))))>>8))))>>2);
                  if ((index_1004X < 0)) {
                    goto L30999;}
                  else {
                    if ((index_1004X < len_1006X)) {
                      v_1007X = *((unsigned char *) ((Scode_pointerS) + 1));
                      if ((0 == v_1007X)) {
                        goto L30989;}
                      else {
                        if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                          goto L30989;}
                        else {
                          merged_arg0K0 = arg4_1003X;
                          merged_arg0K1 = (((index_1004X)<<2));
                          merged_arg0K2 = value_1005X;
                          proposal_d_write_return_tag = 2;
                          goto proposal_d_write;
                         proposal_d_write_return_2:
                          goto L30998;}}}
                    else {
                      goto L30999;}}}
                else {
                  goto L31021;}}
              else {
                goto L31021;}}
            else {
              goto L31021;}}
          else {
            goto L31021;}}
        else {
          goto L31021;}}
      else {
        code_1008X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1008X)))))<<2)), code_1008X, 20, 4);
        n_1009X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_1009X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_1003X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_1002X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_1001X;
        x_1010X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_1010X;
        arg0K0 = 4;
        goto L22779;}}
      break;
    case 165 : {
      v_1011X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_1011X)) {
        arg0K0 = (SvalS);
        goto L31310;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_1012X = get_current_port0_return_value;
        arg0K0 = v_1012X;
        goto L31310;}}
      break;
    case 166 : {
      v_1013X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_1013X)) {
        arg0K0 = (SvalS);
        goto L31560;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_1014X = get_current_port0_return_value;
        arg0K0 = v_1014X;
        goto L31560;}}
      break;
    case 167 : {
      v_1015X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_1015X)) {
        v_1016X = SvalS;
        v_1017X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_1017X;
        arg0K1 = v_1016X;
        goto L31803;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_1018X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_1018X;
        goto L31803;}}
      break;
    case 168 : {
      x_1019X = SvalS;
      out_1020X = stderr;
      arg0K0 = x_1019X;
      goto L42768;}
      break;
  }}
 L22581: {
  v_1021X = arg0K0;
  SvalS = v_1021X;
  pop_continuationB_return_tag = 2;
  goto pop_continuationB;
 pop_continuationB_return_2:
  arg0K0 = 1;
  goto L25390;}
 L23418: {
  stack_nargs_1022X = arg0K0;
  list_args_1023X = arg0K1;
  ps_write_string("RETURN-EXCEPTION: ", (stderr));
  current_code_1024X = current_code_vector();
  out_1025X = stderr;
  merged_arg1K0 = (SstackS);
  merged_arg0K1 = (((((ScontS) - (SstackS)))>>2));
  merged_arg0K2 = current_code_1024X;
  find_template_return_tag = 0;
  goto find_template;
 find_template_return_0:
  template_1026X = find_template0_return_value;
  merged_arg0K0 = template_1026X;
  merged_arg2K1 = 0;
  merged_arg4K2 = out_1025X;
  maybe_write_template_return_tag = 0;
  goto maybe_write_template;
 maybe_write_template_return_0:
  not_firstP_1027X = maybe_write_template0_return_value;
  merged_arg2K0 = not_firstP_1027X;
  merged_arg4K1 = out_1025X;
  really_show_stack_continuation_uids_return_tag = 0;
  goto really_show_stack_continuation_uids;
 really_show_stack_continuation_uids_return_0:
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  merged_arg0K0 = list_args_1023X;
  merged_arg0K1 = stack_nargs_1022X;
  pop_args_GlistSAgc_return_tag = 2;
  goto pop_args_GlistSAgc;
 pop_args_GlistSAgc_return_2:
  args_1028X = pop_args_GlistSAgc0_return_value;
  code_1029X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1029X)))))<<2)), code_1029X, 16, 0);
  n_1030X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1030X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 16;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 1;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = args_1028X;
  arg0K0 = 2;
  goto L22779;}
 L25390: {
  bytes_used_1031X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1031X));
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L24914: {
  v_1032X = arg0K0;
  SvalS = v_1032X;
  arg0K0 = 2;
  goto L24626;}
 L24626: {
  protocol_skip_1033X = arg0K0;
  SstackS = (ScontS);
  cont_1034X = ScontS;
  pointer_1035X = (((char *) (*((long *) cont_1034X)))) + -2;
  size_1036X = ((((*((unsigned char *) pointer_1035X)))<<8)) + (*((unsigned char *) (pointer_1035X + 1)));
  if ((65535 == size_1036X)) {
    arg0K0 = ((((*((long *) (cont_1034X + 4))))>>2));
    goto L24637;}
  else {
    arg0K0 = size_1036X;
    goto L24637;}}
 L22527: {
  if ((1 == (stack_nargs_523X + list_arg_count_525X))) {
    if ((1 == stack_nargs_523X)) {
      v_1037X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = v_1037X;
      goto L22605;}
    else {
      arg0K0 = (*((long *) (((char *) (-3 + list_args_524X)))));
      goto L22605;}}
  else {
    arg0K0 = stack_nargs_523X;
    arg0K1 = list_args_524X;
    goto L23418;}}
 L22644: {
  v_1038X = arg0K0;
  ScontS = (cont_532X + (4 + (((v_1038X)<<2))));
  merged_arg0K0 = stack_nargs_523X;
  move_args_above_contB_return_tag = 12;
  goto move_args_above_contB;
 move_args_above_contB_return_12:
  arg0K0 = stack_nargs_523X;
  arg0K1 = list_args_524X;
  arg0K2 = list_arg_count_525X;
  goto L32292;}
 L32292: {
  stack_arg_count_1039X = arg0K0;
  list_args_1040X = arg0K1;
  list_arg_count_1041X = arg0K2;
  if ((0 == list_arg_count_1041X)) {
    obj_1042X = SvalS;
    if ((3 == (3 & obj_1042X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1042X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_1039X;
        arg0K1 = 25;
        arg0K2 = 0;
        goto L26593;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_1039X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L23245;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_1039X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L23245;}}
  else {
    obj_1043X = SvalS;
    if ((3 == (3 & obj_1043X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1043X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_1039X;
        arg0K1 = list_args_1040X;
        arg0K2 = list_arg_count_1041X;
        goto L26593;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_1039X;
        arg0K2 = list_args_1040X;
        arg0K3 = list_arg_count_1041X;
        goto L23245;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_1039X;
      arg0K2 = list_args_1040X;
      arg0K3 = list_arg_count_1041X;
      goto L23245;}}}
 L26341: {
  loc_1044X = arg1K0;
  arg_1045X = arg1K1;
  if ((arg_1045X < arg_top_536X)) {
    SstackS = ((SstackS) + (0 - (((stack_nargs_523X)<<2))));
    if ((count_535X < stack_nargs_523X)) {
      merged_arg0K0 = list_args_524X;
      merged_arg0K1 = (stack_nargs_523X - count_535X);
      pop_args_GlistSAgc_return_tag = 3;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_3:
      v_1046X = pop_args_GlistSAgc0_return_value;
      arg0K0 = v_1046X;
      goto L26320;}
    else {
      arg0K0 = stack_nargs_523X;
      arg0K1 = list_args_524X;
      goto L26302;}}
  else {
    *((long *) loc_1044X) = (*((long *) arg_1045X));
    arg1K0 = (loc_1044X + -4);
    arg1K1 = (arg_1045X + -4);
    goto L26341;}}
 L26207: {
  count_1047X = arg0K0;
  bytes_used_1048X = arg0K1;
  stack_nargs_1049X = arg0K2;
  list_args_1050X = arg0K3;
  list_arg_count_1051X = arg0K4;
  if ((count_1047X == (stack_nargs_1049X + list_arg_count_1051X))) {
    arg_top_1052X = SstackS;
    pop_continuationB_return_tag = 3;
    goto pop_continuationB;
   pop_continuationB_return_3:
    arg1K0 = ((SstackS) + -4);
    arg1K1 = (arg_top_1052X + (-4 + (((stack_nargs_1049X)<<2))));
    goto L26239;}
  else {
    arg0K0 = stack_nargs_1049X;
    arg0K1 = list_args_1050X;
    goto L23418;}}
 L23076: {
  i_1053X = arg0K0;
  m_1054X = arg0K1;
  if ((0 == (n_537X & m_1054X))) {
    arg0K0 = (1 + i_1053X);
    arg0K1 = (((m_1054X)<<1));
    goto L23076;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_1054X));
    if ((i_1053X == 0)) {
      x_1055X = Sinterrupted_templateS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1055X;
      Sinterrupted_templateS = 1;
      n_1056X = Senabled_interruptsS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1056X)<<2));
      arg0K0 = 2;
      goto L23029;}
    else {
      if ((i_1053X == 2)) {
        x_1057X = Sfinalize_theseS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_1057X;
        Sfinalize_theseS = 25;
        n_1058X = Senabled_interruptsS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_1058X)<<2));
        arg0K0 = 2;
        goto L23029;}
      else {
        if ((i_1053X == 3)) {
          channel_1059X = Spending_channels_headS;
          next_1060X = *((long *) ((((char *) (-3 + channel_1059X))) + 12));
          Spending_channels_headS = next_1060X;
          addr_1061X = (((char *) (-3 + channel_1059X))) + 12;S48_WRITE_BARRIER(channel_1059X, addr_1061X, 1);
          *((long *) addr_1061X) = 1;
          if ((1 == next_1060X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_1059X;
            goto L11546;}
          else {
            arg0K0 = channel_1059X;
            goto L11546;}}
        else {
          if ((i_1053X == 4)) {
            x_1062X = Sos_signal_typeS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_1062X;
            x_1063X = Sos_signal_argumentS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_1063X;
            Sos_signal_typeS = 1;
            Sos_signal_argumentS = 1;
            n_1064X = Senabled_interruptsS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1064X)<<2));
            arg0K0 = 3;
            goto L23029;}
          else {
            n_1065X = Senabled_interruptsS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1065X)<<2));
            arg0K0 = 1;
            goto L23029;}}}}}}
 L8182: {
  new_env_1066X = arg0K0;
  i_1067X = arg0K1;
  offset_1068X = arg0K2;
  total_count_1069X = arg0K3;
  arg0K0 = (*((unsigned char *) ((Scode_pointerS) + (1 + offset_1068X))));
  arg0K1 = i_1067X;
  arg0K2 = (1 + offset_1068X);
  goto L8188;}
 L17214: {
  count_1070X = arg0K0;
  i_1071X = arg0K1;
  offset_1072X = arg0K2;
  if ((0 == count_1070X)) {
    arg0K0 = new_env_573X;
    arg0K1 = i_1071X;
    arg0K2 = offset_1072X;
    arg0K3 = total_count_569X;
    goto L8182;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + template_574X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1072X)))))<<2)))));
    merged_arg0K1 = new_env_573X;
    merged_arg0K2 = 0;
    make_closure_return_tag = 0;
    goto make_closure;
   make_closure_return_0:
    value_1073X = make_closure0_return_value;
    addr_1074X = (((char *) (-3 + new_env_573X))) + (((i_1071X)<<2));S48_WRITE_BARRIER(new_env_573X, addr_1074X, value_1073X);
    *((long *) addr_1074X) = value_1073X;
    arg0K0 = (-1 + count_1070X);
    arg0K1 = (1 + i_1071X);
    arg0K2 = (1 + offset_1072X);
    goto L17214;}}
 L10756: {
  new_env_1075X = arg0K0;
  i_1076X = arg0K1;
  offset_1077X = arg0K2;
  total_count_1078X = arg0K3;
  arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1077X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1077X)))));
  arg0K1 = i_1076X;
  arg0K2 = (2 + offset_1077X);
  goto L10762;}
 L17275: {
  count_1079X = arg0K0;
  i_1080X = arg0K1;
  offset_1081X = arg0K2;
  if ((0 == count_1079X)) {
    arg0K0 = new_env_579X;
    arg0K1 = i_1080X;
    arg0K2 = offset_1081X;
    arg0K3 = total_count_575X;
    goto L10756;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + template_580X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1081X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1081X))))))<<2)))));
    merged_arg0K1 = new_env_579X;
    merged_arg0K2 = 0;
    make_closure_return_tag = 1;
    goto make_closure;
   make_closure_return_1:
    value_1082X = make_closure0_return_value;
    addr_1083X = (((char *) (-3 + new_env_579X))) + (((i_1080X)<<2));S48_WRITE_BARRIER(new_env_579X, addr_1083X, value_1082X);
    *((long *) addr_1083X) = value_1082X;
    arg0K0 = (-1 + count_1079X);
    arg0K1 = (1 + i_1080X);
    arg0K2 = (2 + offset_1081X);
    goto L17275;}}
 L50500: {
  value_1084X = arg0K0;
  SvalS = value_1084X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L24809: {
  stack_arg_count_1085X = arg0K0;
  obj_1086X = SvalS;
  if ((3 == (3 & obj_1086X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1086X))) + -4))))>>2))))) {
      template_1087X = *((long *) (((char *) (-3 + (SvalS)))));
      code_1088X = *((long *) (((char *) (-3 + template_1087X))));
      protocol_1089X = *((unsigned char *) ((((char *) (-3 + code_1088X))) + 1));
      if ((protocol_1089X == stack_arg_count_1085X)) {
        arg0K0 = code_1088X;
        arg0K1 = 2;
        arg0K2 = 3;
        arg0K3 = template_1087X;
        goto L22340;}
      else {
        if (((127 & protocol_1089X) == stack_arg_count_1085X)) {
          proc_1090X = SvalS;
          merged_arg0K0 = stack_arg_count_1085X;
          move_args_above_contB_return_tag = 13;
          goto move_args_above_contB;
         move_args_above_contB_return_13:
          v_1091X = s48_call_native_procedure(proc_1090X, 2);
          arg0K0 = v_1091X;
          goto L24120;}
        else {
          arg0K0 = stack_arg_count_1085X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L26593;}}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_1085X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L23245;}}
  else {
    arg0K0 = 3;
    arg0K1 = stack_arg_count_1085X;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L23245;}}
 L24086: {
  obj_1092X = SvalS;
  if ((3 == (3 & obj_1092X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1092X))) + -4))))>>2))))) {
      arg0K0 = stack_arg_count_590X;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L26593;}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_590X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L23245;}}
  else {
    arg0K0 = 3;
    arg0K1 = stack_arg_count_590X;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L23245;}}
 L15528: {
  list_args_1093X = arg0K0;
  stack_nargs_1094X = arg0K1;
  merged_arg0K0 = list_args_1093X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_1095X = okay_argument_list0_return_value;
  list_arg_count_1096X = okay_argument_list1_return_value;
  if (okayP_1095X) {
    SvalS = proc_606X;
    arg0K0 = stack_nargs_1094X;
    arg0K1 = list_args_1093X;
    arg0K2 = list_arg_count_1096X;
    goto L32292;}
  else {
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = list_args_1093X;
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_1094X);
    pop_args_GlistSAgc_return_tag = 4;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_4:
    args_1097X = pop_args_GlistSAgc0_return_value;
    proc_1098X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    code_1099X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1099X)))))<<2)), code_1099X, 20, 0);
    n_1100X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1100X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 20;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = proc_1098X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1097X;
    arg0K0 = 2;
    goto L22779;}}
 L15573: {
  list_1101X = arg0K0;
  follower_1102X = arg0K1;
  if ((25 == list_1101X)) {
    list_args_1103X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_1102X))) + 4)))))));
    addr_1104X = (((char *) (-3 + follower_1102X))) + 4;S48_WRITE_BARRIER(follower_1102X, addr_1104X, list_args_1103X);
    *((long *) addr_1104X) = list_args_1103X;
    arg0K0 = rest_list_607X;
    arg0K1 = (-1 + stack_nargs_605X);
    goto L15528;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_1101X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_1102X))) + 4)));
    goto L15573;}}
 L24044: {
  obj_1105X = SvalS;
  if ((3 == (3 & obj_1105X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1105X))) + -4))))>>2))))) {
      arg0K0 = 0;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L26593;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L23245;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L23245;}}
 L24053: {
  SstackS = (Sbottom_of_stackS);
  Sheap_continuationS = cont_609X;
  ScontS = (Sbottom_of_stackS);
  goto L24044;}
 L40323: {
  code_pointer_1106X = ((char *) (*((long *) (ScontS))));
  protocol_1107X = *((unsigned char *) (code_pointer_1106X + 1));
  if ((1 == protocol_1107X)) {
    goto L40340;}
  else {
    if ((66 == protocol_1107X)) {
      goto L40340;}
    else {
      if ((129 == protocol_1107X)) {
        arg0K0 = 2;
        goto L24626;}
      else {
        if ((194 == protocol_1107X)) {
          arg0K0 = 2;
          goto L24626;}
        else {
          if ((71 == protocol_1107X)) {
            cont_1108X = Sheap_continuationS;
            if ((3 == (3 & cont_1108X))) {
              if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_1108X))) + -4))))>>2))))) {
                merged_arg0K0 = cont_1108X;
                merged_arg0K1 = 0;
                copy_continuation_from_heapB_return_tag = 2;
                goto copy_continuation_from_heapB;
               copy_continuation_from_heapB_return_2:
                goto L40323;}
              else {
                arg0K0 = cont_1108X;
                goto L23319;}}
            else {
              arg0K0 = cont_1108X;
              goto L23319;}}
          else {
            if ((70 == protocol_1107X)) {
              offset_1109X = ((((*((unsigned char *) (code_pointer_1106X + 2))))<<8)) + (*((unsigned char *) (code_pointer_1106X + 3)));
              proc_1110X = *((long *) ((ScontS) + 4));
              if ((0 == offset_1109X)) {
                cont_1111X = ScontS;
                pointer_1112X = (((char *) (*((long *) cont_1111X)))) + -2;
                size_1113X = ((((*((unsigned char *) pointer_1112X)))<<8)) + (*((unsigned char *) (pointer_1112X + 1)));
                if ((65535 == size_1113X)) {
                  arg0K0 = ((((*((long *) (cont_1111X + 4))))>>2));
                  goto L40482;}
                else {
                  arg0K0 = size_1113X;
                  goto L40482;}}
              else {
                ScontS = ((ScontS) + 4);
                *((long *) (ScontS)) = (((long) (code_pointer_1106X + offset_1109X)));
                goto L40387;}}
            else {
              if ((65 == protocol_1107X)) {
                wants_stack_args_1114X = ((((*((unsigned char *) (code_pointer_1106X + 2))))<<8)) + (*((unsigned char *) (code_pointer_1106X + 3)));
                if ((0 == wants_stack_args_1114X)) {
                  pop_continuationB_return_tag = 4;
                  goto pop_continuationB;
                 pop_continuationB_return_4:s48_make_availableAgc(12);
                  a_1115X = SvalS;
                  addr_1116X = s48_allocate_small(12);
                  *((long *) addr_1116X) = 2050;
                  x_1117X = 3 + (((long) (addr_1116X + 4)));
                  *((long *) (((char *) (-3 + x_1117X)))) = a_1115X;
                  *((long *) ((((char *) (-3 + x_1117X))) + 4)) = 25;
                  SstackS = ((SstackS) + -4);
                  *((long *) (SstackS)) = x_1117X;
                  Scode_pointerS = ((Scode_pointerS) + 4);
                  arg1K0 = (Scode_pointerS);
                  goto L24972;}
                else {
                  if ((1 == wants_stack_args_1114X)) {
                    pop_continuationB_return_tag = 5;
                    goto pop_continuationB;
                   pop_continuationB_return_5:
                    x_1118X = SvalS;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_1118X;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = 25;
                    Scode_pointerS = ((Scode_pointerS) + 4);
                    arg1K0 = (Scode_pointerS);
                    goto L24972;}
                  else {
                    x_1119X = SvalS;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_1119X;
                    arg0K0 = 1;
                    arg0K1 = 25;
                    goto L23418;}}}
              else {
                x_1120X = SvalS;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_1120X;
                arg0K0 = 1;
                arg0K1 = 25;
                goto L23418;}}}}}}}}
 L26471: {
  if ((nargs_616X == (*((unsigned char *) ((((char *) (-3 + code_615X))) + 1))))) {
    arg0K0 = code_615X;
    arg0K1 = 2;
    arg0K2 = 3;
    arg0K3 = template_614X;
    goto L22340;}
  else {
    v_1121X = *((unsigned char *) ((((char *) (-3 + code_615X))) + 1));
    if ((67 == v_1121X)) {
      if ((nargs_616X == (*((unsigned char *) ((((char *) (-3 + code_615X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_615X))) + -4))))>>8)))))))) {
        index_1122X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_615X))) + -4))))>>8));
        arg0K0 = code_615X;
        arg0K1 = 6;
        arg0K2 = template_614X;
        arg0K3 = (((((*((unsigned char *) ((((char *) (-3 + code_615X))) + index_1122X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_615X))) + (1 + index_1122X)))));
        goto L25400;}
      else {
        goto L26493;}}
    else {
      goto L26493;}}}
 L25259: {
  arg0K0 = (2 + (((max_620X)<<1)));
  goto L25261;}
 L25261: {
  offset_1123X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_1123X);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L25844: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L25157: {
  delta_1124X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_1124X);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L25735: {
  delta_1125X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_1125X);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L50557: {
  val_1126X = arg0K0;
  SvalS = val_1126X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L50569: {
  val_1127X = arg0K0;
  SvalS = val_1127X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L14108: {
  if ((3 == (3 & x_637X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + x_637X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L50569;}
    else {
      goto L14114;}}
  else {
    goto L14114;}}
 L41618: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L41619: {
  if ((3 == (3 & n_638X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_638X))) + -4))))>>2))))) {
      arg0K0 = n_638X;
      goto L23661;}
    else {
      goto L41623;}}
  else {
    goto L41623;}}
 L41767: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L41742: {
  if ((3 == (3 & n_639X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_639X))) + -4))))>>2))))) {
      goto L41767;}
    else {
      goto L41756;}}
  else {
    goto L41756;}}
 L41958: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L41933: {
  if ((3 == (3 & n_640X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_640X))) + -4))))>>2))))) {
      goto L41958;}
    else {
      goto L41947;}}
  else {
    goto L41947;}}
 L42149: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L42124: {
  if ((3 == (3 & n_641X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_641X))) + -4))))>>2))))) {
      goto L42149;}
    else {
      goto L42138;}}
  else {
    goto L42138;}}
 L33849: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L33850: {
  code_1128X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1128X)))))<<2)), code_1128X, 20, 4);
  n_1129X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1129X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_642X;
  arg0K0 = 1;
  goto L22779;}
 L23661: {
  x_1130X = arg0K0;
  code_1131X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1131X)))))<<2)), code_1131X, 20, 4);
  n_1132X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1132X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1130X;
  arg0K0 = 1;
  goto L22779;}
 L33991: {
  v_1133X = (char *) s48_long_to_bignum(x_645X);
  v_1134X = enter_bignum(v_1133X);
  arg0K0 = v_1134X;
  goto L33937;}
 L33937: {
  val_1135X = arg0K0;
  SvalS = val_1135X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L33943: {
  if ((0 == (3 & y_644X))) {
    goto L33949;}
  else {
    if ((3 == (3 & y_644X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_644X))) + -4))))>>2))))) {
        goto L33949;}
      else {
        goto L33952;}}
    else {
      goto L33952;}}}
 L33952: {
  if ((3 == (3 & arg2_643X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_643X))) + -4))))>>2))))) {
      if ((3 == (3 & y_644X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_644X))) + -4))))>>2))))) {
          Stemp0S = arg2_643X;
          Stemp1S = y_644X;s48_make_availableAgc(12);
          value_1136X = Stemp1S;
          Stemp1S = 1;
          y_1137X = *((double *) (((char *) (-3 + value_1136X))));
          value_1138X = Stemp0S;
          Stemp0S = 1;
          x_1139X = *((double *) (((char *) (-3 + value_1138X))));
          addr_1140X = s48_allocate_small(12);
          *((long *) addr_1140X) = 2126;
          Kdouble_1141X = 3 + (((long) (addr_1140X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1141X)))) = (x_1139X + y_1137X);
          SvalS = Kdouble_1141X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          goto L33966;}}
      else {
        goto L33966;}}
    else {
      goto L33966;}}
  else {
    goto L33966;}}
 L8347: {
  a_1142X = arg0K0;
  if ((b_649X < 0)) {
    arg0K0 = (0 - b_649X);
    goto L8351;}
  else {
    arg0K0 = b_649X;
    goto L8351;}}
 L44022: {
  if ((0 == (3 & y_647X))) {
    goto L44028;}
  else {
    if ((3 == (3 & y_647X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_647X))) + -4))))>>2))))) {
        goto L44028;}
      else {
        goto L44031;}}
    else {
      goto L44031;}}}
 L44031: {
  if ((3 == (3 & arg2_646X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_646X))) + -4))))>>2))))) {
      if ((3 == (3 & y_647X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_647X))) + -4))))>>2))))) {
          Stemp0S = arg2_646X;
          Stemp1S = y_647X;s48_make_availableAgc(12);
          value_1143X = Stemp1S;
          Stemp1S = 1;
          y_1144X = *((double *) (((char *) (-3 + value_1143X))));
          value_1145X = Stemp0S;
          Stemp0S = 1;
          x_1146X = *((double *) (((char *) (-3 + value_1145X))));
          addr_1147X = s48_allocate_small(12);
          *((long *) addr_1147X) = 2126;
          Kdouble_1148X = 3 + (((long) (addr_1147X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1148X)))) = (x_1146X * y_1144X);
          SvalS = Kdouble_1148X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          arg0K0 = arg2_646X;
          arg0K1 = y_647X;
          goto L23692;}}
      else {
        arg0K0 = arg2_646X;
        arg0K1 = y_647X;
        goto L23692;}}
    else {
      arg0K0 = arg2_646X;
      arg0K1 = y_647X;
      goto L23692;}}
  else {
    arg0K0 = arg2_646X;
    arg0K1 = y_647X;
    goto L23692;}}
 L34296: {
  v_1149X = (char *) s48_long_to_bignum(x_652X);
  v_1150X = enter_bignum(v_1149X);
  arg0K0 = v_1150X;
  goto L34242;}
 L34242: {
  val_1151X = arg0K0;
  SvalS = val_1151X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L34248: {
  if ((0 == (3 & y_651X))) {
    goto L34254;}
  else {
    if ((3 == (3 & y_651X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_651X))) + -4))))>>2))))) {
        goto L34254;}
      else {
        goto L34257;}}
    else {
      goto L34257;}}}
 L34257: {
  if ((3 == (3 & arg2_650X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_650X))) + -4))))>>2))))) {
      if ((3 == (3 & y_651X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_651X))) + -4))))>>2))))) {
          Stemp0S = arg2_650X;
          Stemp1S = y_651X;s48_make_availableAgc(12);
          value_1152X = Stemp1S;
          Stemp1S = 1;
          y_1153X = *((double *) (((char *) (-3 + value_1152X))));
          value_1154X = Stemp0S;
          Stemp0S = 1;
          x_1155X = *((double *) (((char *) (-3 + value_1154X))));
          addr_1156X = s48_allocate_small(12);
          *((long *) addr_1156X) = 2126;
          Kdouble_1157X = 3 + (((long) (addr_1156X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1157X)))) = (x_1155X - y_1153X);
          SvalS = Kdouble_1157X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L24972;}
        else {
          goto L34271;}}
      else {
        goto L34271;}}
    else {
      goto L34271;}}
  else {
    goto L34271;}}
 L23692: {
  x_1158X = arg0K0;
  y_1159X = arg0K1;
  code_1160X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1160X)))))<<2)), code_1160X, 20, 4);
  n_1161X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1161X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1158X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_1159X;
  arg0K0 = 2;
  goto L22779;}
 L8605: {
  a_1162X = arg0K0;
  if ((b_656X < 0)) {
    arg0K0 = (0 - b_656X);
    goto L8609;}
  else {
    arg0K0 = b_656X;
    goto L8609;}}
 L44265: {
  if ((0 == (3 & y_654X))) {
    goto L44271;}
  else {
    if ((3 == (3 & y_654X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_654X))) + -4))))>>2))))) {
        goto L44271;}
      else {
        goto L44298;}}
    else {
      goto L44298;}}}
 L44298: {
  if ((3 == (3 & arg2_653X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_653X))) + -4))))>>2))))) {
      if ((3 == (3 & y_654X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_654X))) + -4))))>>2))))) {
          if ((0. == (*((double *) (((char *) (-3 + y_654X))))))) {
            arg0K0 = 1;
            goto L44311;}
          else {
            Stemp0S = arg2_653X;
            Stemp1S = y_654X;s48_make_availableAgc(12);
            value_1163X = Stemp1S;
            Stemp1S = 1;
            y_1164X = *((double *) (((char *) (-3 + value_1163X))));
            value_1165X = Stemp0S;
            Stemp0S = 1;
            value_1166X = (*((double *) (((char *) (-3 + value_1165X))))) / y_1164X;
            addr_1167X = s48_allocate_small(12);
            *((long *) addr_1167X) = 2126;
            Kdouble_1168X = 3 + (((long) (addr_1167X + 4)));
            *((double *) (((char *) (-3 + Kdouble_1168X)))) = value_1166X;
            arg0K0 = Kdouble_1168X;
            goto L44311;}}
        else {
          arg0K0 = arg2_653X;
          arg0K1 = y_654X;
          goto L23692;}}
      else {
        arg0K0 = arg2_653X;
        arg0K1 = y_654X;
        goto L23692;}}
    else {
      arg0K0 = arg2_653X;
      arg0K1 = y_654X;
      goto L23692;}}
  else {
    arg0K0 = arg2_653X;
    arg0K1 = y_654X;
    goto L23692;}}
 L34543: {
  val_1169X = arg0K0;
  SvalS = val_1169X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L34549: {
  if ((0 == (3 & y_658X))) {
    goto L34555;}
  else {
    if ((3 == (3 & y_658X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_658X))) + -4))))>>2))))) {
        goto L34555;}
      else {
        goto L34560;}}
    else {
      goto L34560;}}}
 L34560: {
  if ((3 == (3 & arg2_657X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_657X))) + -4))))>>2))))) {
      if ((3 == (3 & y_658X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_658X))) + -4))))>>2))))) {
          b_1170X = (*((double *) (((char *) (-3 + arg2_657X))))) == (*((double *) (((char *) (-3 + y_658X)))));
          if (b_1170X) {
            arg0K0 = 5;
            goto L34575;}
          else {
            arg0K0 = 1;
            goto L34575;}}
        else {
          goto L34576;}}
      else {
        goto L34576;}}
    else {
      goto L34576;}}
  else {
    goto L34576;}}
 L34819: {
  val_1171X = arg0K0;
  SvalS = val_1171X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L34825: {
  if ((0 == (3 & y_660X))) {
    goto L34831;}
  else {
    if ((3 == (3 & y_660X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_660X))) + -4))))>>2))))) {
        goto L34831;}
      else {
        goto L34836;}}
    else {
      goto L34836;}}}
 L34836: {
  if ((3 == (3 & arg2_659X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_659X))) + -4))))>>2))))) {
      if ((3 == (3 & y_660X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_660X))) + -4))))>>2))))) {
          b_1172X = (*((double *) (((char *) (-3 + arg2_659X))))) < (*((double *) (((char *) (-3 + y_660X)))));
          if (b_1172X) {
            arg0K0 = 5;
            goto L34851;}
          else {
            arg0K0 = 1;
            goto L34851;}}
        else {
          goto L34852;}}
      else {
        goto L34852;}}
    else {
      goto L34852;}}
  else {
    goto L34852;}}
 L35158: {
  val_1173X = arg0K0;
  SvalS = val_1173X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35164: {
  if ((0 == (3 & y_662X))) {
    goto L35170;}
  else {
    if ((3 == (3 & y_662X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_662X))) + -4))))>>2))))) {
        goto L35170;}
      else {
        goto L35175;}}
    else {
      goto L35175;}}}
 L35175: {
  if ((3 == (3 & arg2_661X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_661X))) + -4))))>>2))))) {
      if ((3 == (3 & y_662X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_662X))) + -4))))>>2))))) {
          b_1174X = (*((double *) (((char *) (-3 + y_662X))))) < (*((double *) (((char *) (-3 + arg2_661X)))));
          if (b_1174X) {
            arg0K0 = 5;
            goto L35190;}
          else {
            arg0K0 = 1;
            goto L35190;}}
        else {
          goto L35191;}}
      else {
        goto L35191;}}
    else {
      goto L35191;}}
  else {
    goto L35191;}}
 L35497: {
  val_1175X = arg0K0;
  SvalS = val_1175X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35503: {
  if ((0 == (3 & y_664X))) {
    goto L35509;}
  else {
    if ((3 == (3 & y_664X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_664X))) + -4))))>>2))))) {
        goto L35509;}
      else {
        goto L35514;}}
    else {
      goto L35514;}}}
 L35514: {
  if ((3 == (3 & arg2_663X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_663X))) + -4))))>>2))))) {
      if ((3 == (3 & y_664X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_664X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + y_664X))))) < (*((double *) (((char *) (-3 + arg2_663X))))))) {
            arg0K0 = 1;
            goto L35529;}
          else {
            arg0K0 = 5;
            goto L35529;}}
        else {
          goto L35530;}}
      else {
        goto L35530;}}
    else {
      goto L35530;}}
  else {
    goto L35530;}}
 L35807: {
  val_1176X = arg0K0;
  SvalS = val_1176X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35813: {
  if ((0 == (3 & y_666X))) {
    goto L35819;}
  else {
    if ((3 == (3 & y_666X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_666X))) + -4))))>>2))))) {
        goto L35819;}
      else {
        goto L35824;}}
    else {
      goto L35824;}}}
 L35824: {
  if ((3 == (3 & arg2_665X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_665X))) + -4))))>>2))))) {
      if ((3 == (3 & y_666X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_666X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + arg2_665X))))) < (*((double *) (((char *) (-3 + y_666X))))))) {
            arg0K0 = 1;
            goto L35839;}
          else {
            arg0K0 = 5;
            goto L35839;}}
        else {
          goto L35840;}}
      else {
        goto L35840;}}
    else {
      goto L35840;}}
  else {
    goto L35840;}}
 L19818: {
  x_1177X = arg0K0;
  y_1178X = arg0K1;
  if ((0 == (3 & x_1177X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19848;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1177X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19848;}}
 L9219: {
  a_1179X = arg0K0;
  if ((b_670X < 0)) {
    arg0K0 = (0 - b_670X);
    goto L9223;}
  else {
    arg0K0 = b_670X;
    goto L9223;}}
 L36129: {
  if ((0 == (3 & y_668X))) {
    goto L36135;}
  else {
    if ((3 == (3 & y_668X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_668X))) + -4))))>>2))))) {
        goto L36135;}
      else {
        arg0K0 = arg2_667X;
        arg0K1 = y_668X;
        goto L23692;}}
    else {
      arg0K0 = arg2_667X;
      arg0K1 = y_668X;
      goto L23692;}}}
 L19727: {
  x_1180X = arg0K0;
  y_1181X = arg0K1;
  if ((0 == (3 & x_1180X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19757;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1180X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19757;}}
 L36309: {
  a_1182X = arg0K0;
  n_1183X = ((y_672X)>>2);
  if ((n_1183X < 0)) {
    arg0K0 = (0 - n_1183X);
    goto L36311;}
  else {
    arg0K0 = n_1183X;
    goto L36311;}}
 L36283: {
  if ((0 == (3 & y_672X))) {
    goto L36289;}
  else {
    if ((3 == (3 & y_672X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_672X))) + -4))))>>2))))) {
        goto L36289;}
      else {
        arg0K0 = arg2_671X;
        arg0K1 = y_672X;
        goto L23692;}}
    else {
      arg0K0 = arg2_671X;
      arg0K1 = y_672X;
      goto L23692;}}}
 L36448: {
  SvalS = n_674X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L36449: {
  code_1184X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1184X)))))<<2)), code_1184X, 20, 4);
  n_1185X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1185X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_674X;
  arg0K0 = 1;
  goto L22779;}
 L36519: {
  SvalS = n_675X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L36520: {
  code_1186X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1186X)))))<<2)), code_1186X, 20, 4);
  n_1187X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1187X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_675X;
  arg0K0 = 1;
  goto L22779;}
 L36590: {
  SvalS = 4;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L36593: {
  code_1188X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1188X)))))<<2)), code_1188X, 20, 4);
  n_1189X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1189X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_676X;
  arg0K0 = 1;
  goto L22779;}
 L36666: {
  SvalS = n_677X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L36667: {
  code_1190X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1190X)))))<<2)), code_1190X, 20, 4);
  n_1191X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1191X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_677X;
  arg0K0 = 1;
  goto L22779;}
 L36737: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L36740: {
  code_1192X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1192X)))))<<2)), code_1192X, 20, 4);
  n_1193X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1193X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_678X;
  arg0K0 = 1;
  goto L22779;}
 L50675: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L23571: {
  x_1194X = SvalS;
  if ((0 == (3 & x_1194X))) {s48_make_availableAgc(16);
    if ((x_1194X < 0)) {
      arg0K0 = (0 - x_1194X);
      goto L23599;}
    else {
      arg0K0 = x_1194X;
      goto L23599;}}
  else {
    if ((0 == (3 & x_1194X))) {
      arg0K0 = 1;
      arg0K1 = 3;
      goto L19335;}
    else {
      arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1194X))) + -4))))>>8))))>>2)));
      arg0K1 = 0;
      goto L19335;}}}
 L23574: {
  code_1195X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1195X)))))<<2)), code_1195X, 20, 4);
  n_1196X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1196X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1197X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1197X;
  arg0K0 = 1;
  goto L22779;}
 L36900: {
  if ((0 == (3 & x_685X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19682;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_685X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19682;}}
 L36903: {
  code_1198X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1198X)))))<<2)), code_1198X, 20, 4);
  n_1199X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1199X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_685X;
  arg0K0 = 1;
  goto L22779;}
 L37007: {
  x_1200X = arg0K0;
  arg0K0 = x_1200X;
  arg0K1 = 0;
  goto L37013;}
 L36990: {
  if ((0 == (3 & x_686X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19624;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_686X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19624;}}
 L36993: {
  code_1201X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1201X)))))<<2)), code_1201X, 20, 4);
  n_1202X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1202X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_686X;
  arg0K0 = 1;
  goto L22779;}
 L37108: {
  if ((0 == (3 & y_689X))) {
    goto L37114;}
  else {
    if ((3 == (3 & y_689X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_689X))) + -4))))>>2))))) {
        goto L37114;}
      else {
        goto L37117;}}
    else {
      goto L37117;}}}
 L37117: {
  code_1203X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1203X)))))<<2)), code_1203X, 20, 4);
  n_1204X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1204X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_688X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_689X;
  arg0K0 = 2;
  goto L22779;}
 L37287: {
  if ((0 == (3 & y_691X))) {
    goto L37293;}
  else {
    if ((3 == (3 & y_691X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_691X))) + -4))))>>2))))) {
        goto L37293;}
      else {
        goto L37296;}}
    else {
      goto L37296;}}}
 L37296: {
  code_1205X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1205X)))))<<2)), code_1205X, 20, 4);
  n_1206X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1206X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_690X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_691X;
  arg0K0 = 2;
  goto L22779;}
 L37466: {
  if ((0 == (3 & y_693X))) {
    goto L37472;}
  else {
    if ((3 == (3 & y_693X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_693X))) + -4))))>>2))))) {
        goto L37472;}
      else {
        goto L37475;}}
    else {
      goto L37475;}}}
 L37475: {
  code_1207X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1207X)))))<<2)), code_1207X, 20, 4);
  n_1208X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1208X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_692X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_693X;
  arg0K0 = 2;
  goto L22779;}
 L33581: {
  val_1209X = arg0K0;
  SvalS = val_1209X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L33597: {
  val_1210X = arg0K0;
  SvalS = val_1210X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L33598: {
  code_1211X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1211X)))))<<2)), code_1211X, 24, 4);
  n_1212X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1212X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 24;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_694X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_695X;
  arg0K0 = 2;
  goto L22779;}
 L44547: {
  if ((0 == (3 & y_695X))) {
    if ((0 == (3 & arg2_694X))) {
      value_1213X = ((arg2_694X)>>2);
      count_1214X = ((y_695X)>>2);
      if ((count_1214X < 0)) {
        PS_SHIFT_RIGHT(value_1213X, (0 - count_1214X), x_1215X)
        x_1216X = x_1215X;s48_make_availableAgc(16);
        if ((536870911 < x_1216X)) {
          goto L44655;}
        else {
          if ((x_1216X < -536870912)) {
            goto L44655;}
          else {
            arg0K0 = (((x_1216X)<<2));
            goto L44650;}}}
      else {
        PS_SHIFT_LEFT(value_1213X, count_1214X, x_1217X)
        result_1218X = x_1217X;
        PS_SHIFT_RIGHT(result_1218X, count_1214X, x_1219X)
        if ((value_1213X == x_1219X)) {
          if ((value_1213X < 0)) {
            if ((result_1218X < 0)) {s48_make_availableAgc(16);
              if ((536870911 < result_1218X)) {
                goto L44677;}
              else {
                if ((result_1218X < -536870912)) {
                  goto L44677;}
                else {
                  arg0K0 = (((result_1218X)<<2));
                  goto L44672;}}}
            else {
              arg0K0 = arg2_694X;
              arg0K1 = y_695X;
              goto L44560;}}
          else {
            if ((result_1218X < 0)) {
              arg0K0 = arg2_694X;
              arg0K1 = y_695X;
              goto L44560;}
            else {s48_make_availableAgc(16);
              if ((536870911 < result_1218X)) {
                goto L44699;}
              else {
                if ((result_1218X < -536870912)) {
                  goto L44699;}
                else {
                  arg0K0 = (((result_1218X)<<2));
                  goto L44694;}}}}}
        else {
          arg0K0 = arg2_694X;
          arg0K1 = y_695X;
          goto L44560;}}}
    else {
      if ((3 == (3 & arg2_694X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_694X))) + -4))))>>2))))) {
          y_1220X = ((y_695X)>>2);
          merged_arg0K0 = arg2_694X;
          merged_arg0K1 = y_1220X;
          shift_space_return_tag = 0;
          goto shift_space;
         shift_space_return_0:
          needed_1221X = shift_space0_return_value;
          Stemp0S = arg2_694X;s48_make_availableAgc((((needed_1221X)<<2)));
          value_1222X = Stemp0S;
          Stemp0S = 1;
          if ((0 == (3 & value_1222X))) {
            v_1223X = (char *) s48_long_to_bignum((((value_1222X)>>2)));
            arg1K0 = v_1223X;
            goto L44730;}
          else {
            arg1K0 = (((char *) (-3 + value_1222X)));
            goto L44730;}}
        else {
          arg0K0 = arg2_694X;
          arg0K1 = y_695X;
          goto L23692;}}
      else {
        arg0K0 = arg2_694X;
        arg0K1 = y_695X;
        goto L23692;}}}
  else {
    arg0K0 = arg2_694X;
    arg0K1 = y_695X;
    goto L23692;}}
 L50736: {
  val_1224X = arg0K0;
  SvalS = val_1224X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L41359: {
  val_1225X = arg0K0;
  SvalS = val_1225X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L41330: {
  code_1226X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1226X)))))<<2)), code_1226X, 20, 4);
  n_1227X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1227X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_701X;
  x_1228X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1228X;
  arg0K0 = 2;
  goto L22779;}
 L41251: {
  val_1229X = arg0K0;
  SvalS = val_1229X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L41222: {
  code_1230X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1230X)))))<<2)), code_1230X, 20, 4);
  n_1231X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1231X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_703X;
  x_1232X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1232X;
  arg0K0 = 2;
  goto L22779;}
 L37636: {
  code_1233X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1233X)))))<<2)), code_1233X, 20, 4);
  n_1234X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1234X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((x_708X)<<2));
  arg0K0 = 1;
  goto L22779;}
 L50756: {
  val_1235X = arg0K0;
  SvalS = val_1235X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L50770: {
  value_1236X = arg0K0;
  SvalS = value_1236X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L27693: {
  code_1237X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1237X)))))<<2)), code_1237X, 20, 8);
  n_1238X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1238X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_715X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_716X)<<2));
  arg0K0 = 2;
  goto L22779;}
 L27814: {
  SvalS = new_721X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L27798: {
  i_1239X = arg0K0;
  if ((i_1239X < 0)) {
    goto L27814;}
  else {
    value_1240X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_721X))) + (((i_1239X)<<2)))) = value_1240X;
    arg0K0 = (-1 + i_1239X);
    goto L27798;}}
 L27932: {
  i_1241X = arg0K0;
  if ((i_1241X < 0)) {
    arg0K0 = stack_nargs_729X;
    arg0K1 = rest_list_730X;
    goto L28074;}
  else {
    value_1242X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_727X))) + (((i_1241X)<<2)))) = value_1242X;
    arg0K0 = (-1 + i_1241X);
    goto L27932;}}
 L28100: {
  code_1243X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1243X)))))<<2)), code_1243X, 20, 12);
  n_1244X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1244X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_731X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_733X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_732X)<<2));
  arg0K0 = 3;
  goto L22779;}
 L28233: {
  addr_1245X = (((char *) (-3 + arg2_734X))) + (((offset_736X)<<2));S48_WRITE_BARRIER(arg2_734X, addr_1245X, value_735X);
  *((long *) addr_1245X) = value_735X;
  goto L28242;}
 L28242: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L28243: {
  code_1246X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1246X)))))<<2)), code_1246X, 20, 16);
  n_1247X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1247X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_734X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_737X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_736X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_735X;
  arg0K0 = 4;
  goto L22779;}
 L28476: {
  code_1248X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1248X)))))<<2)), code_1248X, 20, 8);
  n_1249X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1249X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_741X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_742X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = init_740X;
  arg0K0 = 3;
  goto L22779;}
 L28493: {
  x_1250X = arg0K0;
  value_1251X = Stemp0S;
  Stemp0S = 1;
  if ((1 == x_1250X)) {
    code_1252X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1252X)))))<<2)), code_1252X, 32, 8);
    n_1253X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1253X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 32;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((type_741X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_742X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = value_1251X;
    arg0K0 = 3;
    goto L22779;}
  else {
    arg0K0 = (-1 + len_742X);
    goto L28519;}}
 L28814: {
  code_1254X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1254X)))))<<2)), code_1254X, 28, 12);
  n_1255X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1255X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_747X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_749X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_748X;
  arg0K0 = 3;
  goto L22779;}
 L28804: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg2_747X))) + (-4 & index_748X))));
  goto L28813;}
 L28813: {
  value_1256X = arg0K0;
  SvalS = value_1256X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L28769: {
  code_1257X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1257X)))))<<2)), code_1257X, 20, 12);
  n_1258X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1258X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_747X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_749X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_748X;
  arg0K0 = 3;
  goto L22779;}
 L29133: {
  code_1259X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1259X)))))<<2)), code_1259X, 28, 12);
  n_1260X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1260X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_755X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_757X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_754X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_756X;
  arg0K0 = 4;
  goto L22779;}
 L29123: {
  addr_1261X = (((char *) (-3 + arg3_755X))) + (-4 & arg2_754X);S48_WRITE_BARRIER(arg3_755X, addr_1261X, value_756X);
  *((long *) addr_1261X) = value_756X;
  goto L29132;}
 L29132: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L29086: {
  code_1262X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1262X)))))<<2)), code_1262X, 20, 12);
  n_1263X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1263X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_755X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_757X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_754X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_756X;
  arg0K0 = 4;
  goto L22779;}
 L37719: {
  code_1264X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1264X)))))<<2)), code_1264X, 20, 4);
  n_1265X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1265X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_762X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_763X)<<2));
  arg0K0 = 2;
  goto L22779;}
 L37732: {
  vector_1266X = arg0K0;
  if ((1 == vector_1266X)) {
    code_1267X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1267X)))))<<2)), code_1267X, 32, 4);
    n_1268X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1268X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 32;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_762X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((init_763X)<<2));
    arg0K0 = 2;
    goto L22779;}
  else {
    arg0K0 = (-1 + len_762X);
    goto L37754;}}
 L49839: {
  code_1269X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1269X)))))<<2)), code_1269X, 20, 4);
  n_1270X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1270X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1271X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1271X;
  arg0K0 = 1;
  goto L22779;}
 L46046: {
  code_1272X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1272X)))))<<2)), code_1272X, 28, 4);
  n_1273X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1273X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_769X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_770X)<<2));
  arg0K0 = 2;
  goto L22779;}
 L49400: {
  code_1274X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1274X)))))<<2)), code_1274X, 20, 4);
  n_1275X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1275X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_769X;
  x_1276X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1276X;
  arg0K0 = 2;
  goto L22779;}
 L43116: {
  code_1277X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1277X)))))<<2)), code_1277X, 28, 4);
  n_1278X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1278X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_773X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_774X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_775X)<<2));
  arg0K0 = 3;
  goto L22779;}
 L43095: {
  code_1279X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1279X)))))<<2)), code_1279X, 20, 4);
  n_1280X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1280X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_773X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_774X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_775X)<<2));
  arg0K0 = 3;
  goto L22779;}
 L47948: {
  code_1281X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1281X)))))<<2)), code_1281X, 20, 4);
  n_1282X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1282X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_773X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_772X;
  x_1283X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1283X;
  arg0K0 = 3;
  goto L22779;}
 L37931: {
  code_1284X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1284X)))))<<2)), code_1284X, 20, 4);
  n_1285X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1285X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_778X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_779X))<<8)));
  arg0K0 = 2;
  goto L22779;}
 L37944: {
  vector_1286X = arg0K0;
  if ((1 == vector_1286X)) {
    code_1287X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1287X)))))<<2)), code_1287X, 32, 4);
    n_1288X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1288X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 32;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_778X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (9 + ((((init_779X))<<8)));
    arg0K0 = 2;
    goto L22779;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1286X))) + len_778X)) = 0;
    arg0K0 = (-1 + len_778X);
    goto L37966;}}
 L46143: {
  code_1289X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1289X)))))<<2)), code_1289X, 20, 4);
  n_1290X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1290X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_777X;
  x_1291X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1291X;
  arg0K0 = 2;
  goto L22779;}
 L49905: {
  code_1292X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1292X)))))<<2)), code_1292X, 20, 4);
  n_1293X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1293X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1294X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1294X;
  arg0K0 = 1;
  goto L22779;}
 L46224: {
  code_1295X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1295X)))))<<2)), code_1295X, 28, 4);
  n_1296X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1296X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_783X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_784X)<<2));
  arg0K0 = 2;
  goto L22779;}
 L49514: {
  code_1297X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1297X)))))<<2)), code_1297X, 20, 4);
  n_1298X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1298X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_783X;
  x_1299X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1299X;
  arg0K0 = 2;
  goto L22779;}
 L43303: {
  code_1300X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1300X)))))<<2)), code_1300X, 28, 4);
  n_1301X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1301X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_787X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_788X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_789X))<<8)));
  arg0K0 = 3;
  goto L22779;}
 L43282: {
  code_1302X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1302X)))))<<2)), code_1302X, 20, 4);
  n_1303X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1303X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_787X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_788X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_789X))<<8)));
  arg0K0 = 3;
  goto L22779;}
 L48094: {
  code_1304X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1304X)))))<<2)), code_1304X, 20, 4);
  n_1305X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1305X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_787X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_786X;
  x_1306X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1306X;
  arg0K0 = 3;
  goto L22779;}
 L20949: {
  i_1307X = arg0K0;
  h_1308X = arg0K1;
  if ((i_1307X < n_794X)) {
    arg0K0 = (1 + i_1307X);
    arg0K1 = (h_1308X + (((*((unsigned char *) ((((char *) (-3 + string_793X))) + i_1307X))))));
    goto L20949;}
  else {
    index_1309X = 1023 & h_1308X;
    link_1310X = *((long *) ((((char *) (-3 + table_792X))) + (((index_1309X)<<2))));
    if ((0 == (3 & link_1310X))) {
      arg0K0 = (3 + (-4 & link_1310X));
      goto L20905;}
    else {
      arg0K0 = link_1310X;
      goto L20905;}}}
 L33775: {
  code_1311X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1311X)))))<<2)), code_1311X, 20, 4);
  n_1312X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1312X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1313X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1313X;
  arg0K0 = 1;
  goto L22779;}
 L47578: {
  val_1314X = arg0K0;
  SvalS = val_1314X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L47559: {
  code_1315X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1315X)))))<<2)), code_1315X, 20, 4);
  n_1316X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1316X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1317X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1317X;
  arg0K0 = 1;
  goto L22779;}
 L42861: {
  if ((1 == (SvalS))) {
    addr_1318X = (((char *) (-3 + arg2_798X))) + 4;S48_WRITE_BARRIER(arg2_798X, addr_1318X, 273);
    *((long *) addr_1318X) = 273;
    goto L42867;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_798X))) + 4)))))) {
      addr_1319X = (((char *) (-3 + arg2_798X))) + 4;S48_WRITE_BARRIER(arg2_798X, addr_1319X, 529);
      *((long *) addr_1319X) = 529;
      goto L42867;}
    else {
      goto L42867;}}}
 L42868: {
  code_1320X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1320X)))))<<2)), code_1320X, 20, 4);
  n_1321X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1321X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_798X;
  x_1322X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1322X;
  arg0K0 = 2;
  goto L22779;}
 L50837: {
  val_1323X = arg0K0;
  SvalS = val_1323X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L46322: {
  SvalS = x_801X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L38386: {
  if ((0 == (3 & arg2_802X))) {
    if (((((arg2_802X)>>2)) < 0)) {
      arg0K0 = 5;
      goto L38160;}
    else {
      arg0K0 = (((arg2_802X)>>2));
      goto L38190;}}
  else {
    if ((3 == (3 & arg2_802X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_802X))) + -4))))>>2))))) {
        if ((1 == mode_803X)) {
          goto L38248;}
        else {
          if ((3 == mode_803X)) {
            goto L38248;}
          else {
            v_1324X = ps_open_fd((((char *)(((char *) (-3 + arg2_802X))))), 0, &v_1325X);
            arg0K0 = v_1324X;
            arg0K1 = v_1325X;
            goto L38261;}}}
      else {
        arg0K0 = 5;
        goto L38160;}}
    else {
      arg0K0 = 5;
      goto L38160;}}}
 L38160: {
  reason_1326X = arg0K0;
  code_1327X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1327X)))))<<2)), code_1327X, (((reason_1326X)<<2)), 4);
  n_1328X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1328X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((reason_1326X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_802X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((mode_803X)<<2));
  arg0K0 = 2;
  goto L22779;}
 L48222: {
  code_1329X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1329X)))))<<2)), code_1329X, 20, 4);
  n_1330X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1330X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1331X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1331X;
  arg0K0 = 1;
  goto L22779;}
 L46465: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L46475;}
  else {
    arg2K0 = 1;
    goto L46475;}}
 L46476: {
  code_1332X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1332X)))))<<2)), code_1332X, 20, 4);
  n_1333X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1333X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_818X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_817X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_816X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_815X;
  x_1334X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1334X;
  arg0K0 = 5;
  goto L22779;}
 L38985: {
  if ((3 == (3 & arg3_821X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_821X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_821X))) + -4))))>>8)));
      goto L38987;}
    else {
      goto L39206;}}
  else {
    goto L39206;}}
 L39192: {
  if ((3 == (3 & arg3_821X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_821X))) + -4))))>>2))))) {
      goto L38985;}
    else {
      goto L39039;}}
  else {
    goto L39039;}}
 L39039: {
  code_1335X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1335X)))))<<2)), code_1335X, 20, 4);
  n_1336X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1336X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_822X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_821X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_823X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_824X)<<2));
  arg0K0 = 4;
  goto L22779;}
 L46695: {
  code_1337X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1337X)))))<<2)), code_1337X, 20, 4);
  n_1338X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1338X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_822X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_821X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_820X;
  x_1339X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1339X;
  arg0K0 = 4;
  goto L22779;}
 L42459: {
  val_1340X = arg0K0;
  SvalS = val_1340X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L47640: {
  code_1341X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1341X)))))<<2)), code_1341X, 20, 4);
  n_1342X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1342X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1343X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1343X;
  arg0K0 = 1;
  goto L22779;}
 L46859: {
  val_1344X = arg0K0;
  SvalS = val_1344X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L11355: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_836X))) + 16)));
  goto L46859;}
 L11361: {
  ch_1345X = arg0K0;
  prev_1346X = arg0K1;
  if ((1 == ch_1345X)) {
    addr_1347X = (((char *) (-3 + channel_836X))) + 16;S48_WRITE_BARRIER(channel_836X, addr_1347X, 1);
    *((long *) addr_1347X) = 1;
    n_1348X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_836X))) + 8))))>>2)));
    arg0K0 = (((n_1348X)<<2));
    goto L46859;}
  else {
    if ((ch_1345X == channel_836X)) {
      y_1349X = Spending_channels_tailS;
      if ((ch_1345X == y_1349X)) {
        Spending_channels_tailS = prev_1346X;
        goto L11385;}
      else {
        goto L11385;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_1345X))) + 12)));
      arg0K1 = ch_1345X;
      goto L11361;}}}
 L46842: {
  code_1350X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1350X)))))<<2)), code_1350X, 20, 4);
  n_1351X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1351X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1352X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1352X;
  arg0K0 = 1;
  goto L22779;}
 L16944: {
  i_1353X = arg0K0;
  res_1354X = arg0K1;
  if ((-1 == i_1353X)) {
    SvalS = res_1354X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    channel_1355X = *((Svm_channelsS) + i_1353X);
    if ((3 == (3 & channel_1355X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_1355X))) + -4))))>>2))))) {
        addr_1356X = s48_allocate_small(12);
        *((long *) addr_1356X) = 2050;
        x_1357X = 3 + (((long) (addr_1356X + 4)));
        *((long *) (((char *) (-3 + x_1357X)))) = channel_1355X;
        *((long *) ((((char *) (-3 + x_1357X))) + 4)) = res_1354X;
        arg0K0 = x_1357X;
        goto L16958;}
      else {
        arg0K0 = res_1354X;
        goto L16958;}}
    else {
      arg0K0 = res_1354X;
      goto L16958;}}}
 L39325: {
  old_1358X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 == old_1358X)) {
    goto L39341;}
  else {
    addr_1359X = ((char *) (-3 + old_1358X));S48_WRITE_BARRIER(old_1358X, addr_1359X, 1);
    *((long *) addr_1359X) = 1;
    goto L39341;}}
 L39356: {
  code_1360X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1360X)))))<<2)), code_1360X, 20, 4);
  n_1361X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1361X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proposal_843X;
  arg0K0 = 1;
  goto L22779;}
 L10133: {
  i_1362X = arg0K0;
  stob_1363X = *((long *) ((((char *) (-3 + log_847X))) + (((i_1362X)<<2))));
  if ((1 == stob_1363X)) {
    log_1364X = *((long *) ((((char *) (-3 + proposal_844X))) + 8));
    arg0K0 = 0;
    goto L10371;}
  else {
    verify_1365X = *((long *) ((((char *) (-3 + log_847X))) + (12 + (((i_1362X)<<2)))));
    value_1366X = *((long *) ((((char *) (-3 + log_847X))) + (8 + (((i_1362X)<<2)))));
    if ((29 == verify_1365X)) {
      if ((3 == (3 & stob_1363X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1363X))) + -4)))))) {
          goto L10185;}
        else {
          goto L39603;}}
      else {
        goto L39603;}}
    else {
      if ((verify_1365X == (*((long *) ((((char *) (-3 + stob_1363X))) + (-4 & (*((long *) ((((char *) (-3 + log_847X))) + (4 + (((i_1362X)<<2)))))))))))) {
        if ((verify_1365X == value_1366X)) {
          goto L10185;}
        else {
          if ((3 == (3 & stob_1363X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1363X))) + -4)))))) {
              goto L10185;}
            else {
              goto L39603;}}
          else {
            goto L39603;}}}
      else {
        goto L39603;}}}}
 L29463: {
  value_1367X = arg0K0;
  SvalS = value_1367X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L29464: {
  code_1368X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1368X)))))<<2)), code_1368X, 20, 12);
  n_1369X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1369X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_848X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_850X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_849X)<<2));
  arg0K0 = 3;
  goto L22779;}
 L29573: {
  code_1370X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1370X)))))<<2)), code_1370X, 20, 8);
  n_1371X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1371X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_856X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_857X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_854X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_858X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_859X)<<2));
  arg0K0 = 5;
  goto L22779;}
 L29633: {
  memcpy((void *)((((char *) (-3 + arg3_854X))) + to_index_858X), (void *)((((char *) (-3 + arg5_856X))) + from_index_857X),count_859X);
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L29737: {
  left_1372X = arg0K0;
  copies_1373X = arg0K1;
  if ((1 == copies_1373X)) {
    if ((left_1372X < count_859X)) {
      goto L29573;}
    else {
      from_index_1374X = ((from_index_857X)<<2);
      to_index_1375X = ((to_index_858X)<<2);
      count_1376X = ((count_859X)<<2);
      Stemp0S = arg5_856X;
      Stemp1S = arg3_854X;
      addr_1377X = s48_allocate_tracedAgc(28);
      if ((addr_1377X == NULL)) {
        arg0K0 = 1;
        goto L9699;}
      else {
        *((long *) addr_1377X) = 6154;
        arg0K0 = (3 + (((long) (addr_1377X + 4))));
        goto L9699;}}}
  else {
    arg0K0 = (left_1372X - ((((*((long *) ((((char *) (-3 + copies_1373X))) + 16))))>>2)));
    arg0K1 = (*((long *) ((((char *) (-3 + copies_1373X))) + 20)));
    goto L29737;}}
 L29946: {
  code_1378X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1378X)))))<<2)), code_1378X, 28, 4);
  n_1379X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1379X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_866X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_867X)<<2));
  arg0K0 = 2;
  goto L22779;}
 L29945: {
  value_1380X = arg0K0;
  SvalS = value_1380X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L15135: {
  i_1381X = arg0K0;
  next_stob_1382X = *((long *) ((((char *) (-3 + log_871X))) + (((i_1381X)<<2))));
  if ((1 == next_stob_1382X)) {
    v_1383X = add_log_entryAgc(2, i_1381X, arg2_866X, index_870X, ((((*((unsigned char *) ((((char *) (-3 + arg2_866X))) + (((index_870X)>>2))))))<<2)), 1);
    arg0K0 = v_1383X;
    goto L29945;}
  else {
    if ((arg2_866X == next_stob_1382X)) {
      if ((index_870X == (*((long *) ((((char *) (-3 + log_871X))) + (4 + (((i_1381X)<<2)))))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + log_871X))) + (8 + (((i_1381X)<<2))))));
        goto L29945;}
      else {
        goto L15157;}}
    else {
      goto L15157;}}}
 L32969: {
  code_1384X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1384X)))))<<2)), code_1384X, 20, 4);
  n_1385X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1385X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_866X;
  x_1386X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1386X;
  arg0K0 = 2;
  goto L22779;}
 L30088: {
  code_1387X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1387X)))))<<2)), code_1387X, 28, 4);
  n_1388X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1388X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_873X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_874X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_875X;
  arg0K0 = 3;
  goto L22779;}
 L30087: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L15311: {
  i_1389X = arg0K0;
  next_stob_1390X = *((long *) ((((char *) (-3 + log_879X))) + (((i_1389X)<<2))));
  if ((1 == next_stob_1390X)) {add_log_entryAgc(2, i_1389X, arg3_873X, index_878X, byte_875X, 0);
    goto L30087;}
  else {
    if ((arg3_873X == next_stob_1390X)) {
      if ((index_878X == (*((long *) ((((char *) (-3 + log_879X))) + (4 + (((i_1389X)<<2)))))))) {
        addr_1391X = (((char *) (-3 + log_879X))) + (8 + (((i_1389X)<<2)));S48_WRITE_BARRIER(log_879X, addr_1391X, byte_875X);
        *((long *) addr_1391X) = byte_875X;
        goto L30087;}
      else {
        goto L15331;}}
    else {
      goto L15331;}}}
 L30053: {
  code_1392X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1392X)))))<<2)), code_1392X, 20, 4);
  n_1393X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1393X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_873X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_874X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_875X;
  arg0K0 = 3;
  goto L22779;}
 L33090: {
  code_1394X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1394X)))))<<2)), code_1394X, 20, 4);
  n_1395X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1395X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_873X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_872X;
  x_1396X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1396X;
  arg0K0 = 3;
  goto L22779;}
 L43595: {
  reason_1397X = arg0K0;
  status_1398X = arg0K1;
  code_1399X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1399X)))))<<2)), code_1399X, (((reason_1397X)<<2)), 4);
  n_1400X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1400X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((reason_1397X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_885X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_884X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_883X;
  merged_arg0K0 = status_1398X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 2;
  goto get_error_string;
 get_error_string_return_2:
  x_1401X = get_error_string0_return_value;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1401X;
  arg0K0 = 4;
  goto L22779;}
 L48327: {
  code_1402X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1402X)))))<<2)), code_1402X, 20, 4);
  n_1403X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1403X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_885X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_884X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_883X;
  x_1404X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1404X;
  arg0K0 = 4;
  goto L22779;}
 L49080: {
  i_1405X = arg0K0;
  h_1406X = arg0K1;
  if ((i_1405X < n_897X)) {
    arg0K0 = (1 + i_1405X);
    arg0K1 = (h_1406X + (((*((unsigned char *) ((((char *) (-3 + x_896X))) + i_1405X))))));
    goto L49080;}
  else {
    SvalS = (((h_1406X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}}
 L49057: {
  code_1407X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1407X)))))<<2)), code_1407X, 20, 4);
  n_1408X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1408X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1409X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1409X;
  arg0K0 = 1;
  goto L22779;}
 L43886: {
  code_1410X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1410X)))))<<2)), code_1410X, 20, 4);
  n_1411X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1411X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_898X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_899X;
  arg0K0 = 2;
  goto L22779;}
 L39789: {
  firstP_1412X = arg2K0;
  vector_1413X = s48_find_all(type_918X);
  if ((1 == vector_1413X)) {
    if (firstP_1412X) {s48_collect();
      arg2K0 = 0;
      goto L39789;}
    else {
      code_1414X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1414X)))))<<2)), code_1414X, 32, 4);
      n_1415X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1415X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 32;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((type_918X)<<2));
      arg0K0 = 1;
      goto L22779;}}
  else {
    SvalS = vector_1413X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}}
 L39877: {
  firstP_1416X = arg2K0;
  type_1417X = arg0K1;
  vector_1418X = s48_find_all_records(type_1417X);
  if ((1 == vector_1418X)) {
    if (firstP_1416X) {
      Stemp0S = type_1417X;s48_collect();
      value_1419X = Stemp0S;
      Stemp0S = 1;
      arg2K0 = 0;
      arg0K1 = value_1419X;
      goto L39877;}
    else {
      code_1420X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1420X)))))<<2)), code_1420X, 32, 4);
      n_1421X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1421X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 32;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = type_1417X;
      arg0K0 = 1;
      goto L22779;}}
  else {
    SvalS = vector_1418X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}}
 L27025: {
  code_1422X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1422X)))))<<2)), code_1422X, 20, 4);
  n_1423X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1423X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1424X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1424X;
  arg0K0 = 1;
  goto L22779;}
 L25571: {
  code_1425X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1425X)))))<<2)), code_1425X, 20, 4);
  n_1426X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1426X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1427X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1427X;
  arg0K0 = 1;
  goto L22779;}
 L50961: {
  SvalS = (((old_932X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L8904: {
  proposal_1428X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  x_1429X = Scurrent_threadS;
  addr_1430X = (((char *) (-3 + x_1429X))) + 12;S48_WRITE_BARRIER(x_1429X, addr_1430X, proposal_1428X);
  *((long *) addr_1430X) = proposal_1428X;
  if ((12 == x_934X)) {
    arg0K0 = 0;
    arg0K1 = 25;
    arg0K2 = 0;
    goto L22492;}
  else {
    pc_1431X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    code_1432X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    Slast_code_calledS = code_1432X;
    Scode_pointerS = ((((char *) (-3 + code_1432X))) + (((pc_1431X)>>2)));
    v_1433X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    SvalS = v_1433X;
    arg1K0 = (Scode_pointerS);
    goto L24972;}}
 L50074: {
  x_1434X = s48_schedule_alarm_interrupt((((p_936X)>>2)));
  SvalS = (((x_1434X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L47811: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L47815;}
  else {
    arg2K0 = 1;
    goto L47815;}}
 L47816: {
  code_1435X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1435X)))))<<2)), code_1435X, 20, 4);
  n_1436X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1436X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_940X;
  x_1437X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1437X;
  arg0K0 = 2;
  goto L22779;}
 L30319: {
  rest_list_1438X = arg0K0;
  if ((25 == rest_list_1438X)) {
    proc_1439X = *((long *) ((SstackS) + (-4 + (((nargs_943X)<<2)))));
    name_1440X = *((long *) ((SstackS) + (-8 + (((nargs_943X)<<2)))));
    args_1441X = SstackS;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (10 + (((nargs_943X)<<10)));
    if ((3 == (3 & name_1440X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_1440X))) + -4))))>>2))))) {
        if ((3 == (3 & proc_1439X))) {
          if ((18 == (31 & ((((*((long *) ((((char *) (-3 + proc_1439X))) + -4))))>>2))))) {
            if ((4 == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proc_1439X))) + -4))))>>8)))) {
              result_1442X = s48_external_call(proc_1439X, name_1440X, (-2 + nargs_943X), args_1441X);
              if ((Sexternal_exceptionPS)) {
                Sexternal_exceptionPS = 0;
                arg0K0 = (Sexternal_exception_nargsS);
                goto L22779;}
              else {
                SvalS = result_1442X;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L24972;}}
            else {
              goto L30623;}}
          else {
            goto L30623;}}
        else {
          goto L30623;}}
      else {
        goto L30623;}}
    else {
      goto L30623;}}
  else {
    x_1443X = *((long *) (((char *) (-3 + rest_list_1438X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1443X;
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_1438X))) + 4)));
    goto L30319;}}
 L47052: {
  if ((1 == (SvalS))) {
    v_1444X = Hlookup2690((Sexported_bindingsS), arg2_949X, 0);
    arg0K0 = v_1444X;
    goto L47093;}
  else {
    v_1445X = Hlookup2671((Simported_bindingsS), arg2_949X, 0);
    arg0K0 = v_1445X;
    goto L47093;}}
 L47057: {
  code_1446X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1446X)))))<<2)), code_1446X, 20, 4);
  n_1447X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1447X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_949X;
  x_1448X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1448X;
  arg0K0 = 2;
  goto L22779;}
 L32151: {
  if ((1 == (SvalS))) {
    arg0K0 = (Sexported_bindingsS);
    goto L32186;}
  else {
    arg0K0 = (Simported_bindingsS);
    goto L32186;}}
 L32156: {
  code_1449X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1449X)))))<<2)), code_1449X, 20, 4);
  n_1450X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1450X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_951X;
  x_1451X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1451X;
  arg0K0 = 2;
  goto L22779;}
 L48975: {
  option_1452X = arg0K0;
  seconds_1453X = arg0K1;
  mseconds_1454X = arg0K2;
  if ((536869 < seconds_1453X)) {
    code_1455X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1455X)))))<<2)), code_1455X, 24, 4);
    n_1456X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1456X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 24;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((option_1452X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_1453X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_1454X)<<2));
    arg0K0 = 3;
    goto L22779;}
  else {
    SvalS = (((((1000 * seconds_1453X) + mseconds_1454X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}}
 L50995: {
  s48_Scallback_return_stack_blockS = arg2_977X;
  return x_978X;}
 L40999: {
  val_1457X = arg0K0;
  SvalS = val_1457X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L40964: {
  code_1458X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1458X)))))<<2)), code_1458X, 20, 4);
  n_1459X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1459X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_979X;
  x_1460X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1460X;
  arg0K0 = 2;
  goto L22779;}
 L40167: {
  len_1461X = 1 + n_984X;
  addr_1462X = s48_allocate_small((4 + len_1461X));
  *((long *) addr_1462X) = (70 + (((len_1461X)<<8)));
  string_1463X = 3 + (((long) (addr_1462X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1463X))) + n_984X)) = 0;
  arg0K0 = arg2_983X;
  arg0K1 = (-1 + n_984X);
  goto L40144;}
 L40119: {
  if ((25 == arg2_983X)) {
    goto L40167;}
  else {
    code_1464X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1464X)))))<<2)), code_1464X, 20, 4);
    n_1465X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1465X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 20;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = arg2_983X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_984X)<<2));
    arg0K0 = 2;
    goto L22779;}}
 L42538: {
  list_1466X = arg0K0;
  slow_1467X = arg0K1;
  move_slowP_1468X = arg2K2;
  if ((25 == list_1466X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    if ((3 == (3 & list_1466X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_1466X))) + -4))))>>2))))) {
        head_1469X = *((long *) (((char *) (-3 + list_1466X))));
        if ((3 == (3 & head_1469X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_1469X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_1469X))))) == arg2_988X)) {
              SvalS = head_1469X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L24972;}
            else {
              list_1470X = *((long *) ((((char *) (-3 + list_1466X))) + 4));
              if ((list_1470X == slow_1467X)) {
                goto L42524;}
              else {
                if (move_slowP_1468X) {
                  arg0K0 = list_1470X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_1467X))) + 4)));
                  arg2K2 = 0;
                  goto L42538;}
                else {
                  arg0K0 = list_1470X;
                  arg0K1 = slow_1467X;
                  arg2K2 = 1;
                  goto L42538;}}}}
          else {
            goto L42524;}}
        else {
          goto L42524;}}
      else {
        goto L42524;}}
    else {
      goto L42524;}}}
 L30709: {
  code_1471X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1471X)))))<<2)), code_1471X, 28, 8);
  n_1472X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1472X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_993X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_992X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_994X)<<2));
  arg0K0 = 3;
  goto L22779;}
 L30699: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg3_993X))) + (((index_994X)<<2)))));
  goto L30708;}
 L30708: {
  value_1473X = arg0K0;
  SvalS = value_1473X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L30729: {
  code_1474X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1474X)))))<<2)), code_1474X, 20, 8);
  n_1475X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1475X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_993X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_992X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_994X)<<2));
  arg0K0 = 3;
  goto L22779;}
 L30999: {
  code_1476X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1476X)))))<<2)), code_1476X, 28, 8);
  n_1477X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1477X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_1003X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_1002X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_1004X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_1005X;
  arg0K0 = 4;
  goto L22779;}
 L30989: {
  addr_1478X = (((char *) (-3 + arg4_1003X))) + (((index_1004X)<<2));S48_WRITE_BARRIER(arg4_1003X, addr_1478X, value_1005X);
  *((long *) addr_1478X) = value_1005X;
  goto L30998;}
 L30998: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L31021: {
  code_1479X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1479X)))))<<2)), code_1479X, 20, 8);
  n_1480X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1480X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_1003X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_1002X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_1004X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_1005X;
  arg0K0 = 4;
  goto L22779;}
 L31310: {
  port_1481X = arg0K0;
  if ((3 == (3 & port_1481X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1481X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1481X))) + 4))))>>2))))) {
        goto L31367;}
      else {
        b_1482X = *((long *) ((((char *) (-3 + port_1481X))) + 16));
        if ((1 == b_1482X)) {
          code_1483X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1483X)))))<<2)), code_1483X, 52, 8);
          n_1484X = *((unsigned char *) (Scode_pointerS));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_1484X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 52;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = port_1481X;
          arg0K0 = 1;
          goto L22779;}
        else {
          p_1485X = *((long *) ((((char *) (-3 + port_1481X))) + 24));
          i_1486X = (((*((long *) ((((char *) (-3 + port_1481X))) + 20))))>>2);
          if ((i_1486X == (((p_1485X)>>2)))) {
            code_1487X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1487X)))))<<2)), code_1487X, 52, 8);
            n_1488X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1488X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 52;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1481X;
            arg0K0 = 1;
            goto L22779;}
          else {
            val_1489X = 4 + (((i_1486X)<<2));
            addr_1490X = (((char *) (-3 + port_1481X))) + 20;S48_WRITE_BARRIER(port_1481X, addr_1490X, val_1489X);
            *((long *) addr_1490X) = val_1489X;
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1482X))) + i_1486X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L24972;}}}}
    else {
      goto L31367;}}
  else {
    goto L31367;}}
 L31560: {
  port_1491X = arg0K0;
  if ((3 == (3 & port_1491X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1491X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1491X))) + 4))))>>2))))) {
        goto L31617;}
      else {
        b_1492X = *((long *) ((((char *) (-3 + port_1491X))) + 16));
        if ((1 == b_1492X)) {
          code_1493X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1493X)))))<<2)), code_1493X, 52, 8);
          n_1494X = *((unsigned char *) (Scode_pointerS));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_1494X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 52;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = port_1491X;
          arg0K0 = 1;
          goto L22779;}
        else {
          p_1495X = *((long *) ((((char *) (-3 + port_1491X))) + 24));
          i_1496X = (((*((long *) ((((char *) (-3 + port_1491X))) + 20))))>>2);
          if ((i_1496X == (((p_1495X)>>2)))) {
            code_1497X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1497X)))))<<2)), code_1497X, 52, 8);
            n_1498X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1498X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 52;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1491X;
            arg0K0 = 1;
            goto L22779;}
          else {
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1492X))) + i_1496X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L24972;}}}}
    else {
      goto L31617;}}
  else {
    goto L31617;}}
 L31803: {
  Kchar_1499X = arg0K0;
  port_1500X = arg0K1;
  if ((9 == (255 & Kchar_1499X))) {
    if ((3 == (3 & port_1500X))) {
      if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1500X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_1500X))) + 4))))>>2))))) {
          goto L31863;}
        else {
          b_1501X = *((long *) ((((char *) (-3 + port_1500X))) + 16));
          if ((1 == b_1501X)) {
            code_1502X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1502X)))))<<2)), code_1502X, 52, 8);
            n_1503X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1503X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 52;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = Kchar_1499X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1500X;
            arg0K0 = 2;
            goto L22779;}
          else {
            i_1504X = (((*((long *) ((((char *) (-3 + port_1500X))) + 20))))>>2);
            if ((i_1504X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_1501X))) + -4))))>>8)))) {
              code_1505X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1505X)))))<<2)), code_1505X, 52, 8);
              n_1506X = *((unsigned char *) (Scode_pointerS));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((n_1506X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = 52;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = Kchar_1499X;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = port_1500X;
              arg0K0 = 2;
              goto L22779;}
            else {
              val_1507X = 4 + (((i_1504X)<<2));
              addr_1508X = (((char *) (-3 + port_1500X))) + 20;S48_WRITE_BARRIER(port_1500X, addr_1508X, val_1507X);
              *((long *) addr_1508X) = val_1507X;
              *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_1500X))) + 16)))))) + i_1504X)) = (((((Kchar_1499X)>>8))));
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L24972;}}}}
      else {
        goto L31863;}}
    else {
      goto L31863;}}
  else {
    goto L31863;}}
 L42768: {
  stuff_1509X = arg0K0;
  if ((3 == (3 & stuff_1509X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_1509X))) + -4))))>>2))))) {
      thing_1510X = *((long *) (((char *) (-3 + stuff_1509X))));
      if ((0 == (3 & thing_1510X))) {
        ps_write_integer((((thing_1510X)>>2)), out_1020X);
        goto L42774;}
      else {
        if ((9 == (255 & thing_1510X))) {
          ps_write_string("#\\", out_1020X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_1510X)>>8))), out_1020X, ignoreXX) }
          goto L42774;}
        else {
          if ((3 == (3 & thing_1510X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>8))))>>2)))) {
                type_1511X = *((long *) (((char *) (-3 + thing_1510X))));
                if ((3 == (3 & type_1511X))) {
                  if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_1511X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_1511X))) + -4))))>>8))))>>2)))) {
                      obj_1512X = *((long *) ((((char *) (-3 + type_1511X))) + 8));
                      if ((3 == (3 & obj_1512X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_1512X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_1020X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_1510X)))))))) + 8))))))))))))), out_1020X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_1020X, ignoreXX) }
                          goto L42774;}
                        else {
                          goto L16221;}}
                      else {
                        goto L16221;}}
                    else {
                      goto L16221;}}
                  else {
                    goto L16221;}}
                else {
                  goto L16221;}}
              else {
                goto L16221;}}
            else {
              goto L16221;}}
          else {
            goto L16221;}}}}
    else {
      goto L42759;}}
  else {
    goto L42759;}}
 L24637: {
  v_1513X = arg0K0;
  ScontS = (cont_1034X + (4 + (((v_1513X)<<2))));
  v_1514X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  v_1515X = s48_invoke_native_continuation(((((long) (((char *) v_1514X)))) + protocol_skip_1033X));
  arg0K0 = v_1515X;
  goto L24120;}
 L22605: {
  v_1516X = arg0K0;
  SvalS = v_1516X;
  arg0K0 = cont_530X;
  goto L23319;}
 L26320: {
  x_1517X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1517X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L26302: {
  stack_nargs_1518X = arg0K0;
  l_1519X = arg0K1;
  if ((count_535X == stack_nargs_1518X)) {
    arg0K0 = l_1519X;
    goto L26320;}
  else {
    x_1520X = *((long *) (((char *) (-3 + l_1519X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1520X;
    arg0K0 = (1 + stack_nargs_1518X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1519X))) + 4)));
    goto L26302;}}
 L26239: {
  loc_1521X = arg1K0;
  arg_1522X = arg1K1;
  if ((arg_1522X < arg_top_1052X)) {
    SstackS = ((SstackS) + (0 - (((stack_nargs_1049X)<<2))));
    if ((0 == list_arg_count_1051X)) {
      goto L26226;}
    else {
      merged_arg0K0 = list_args_1050X;
      merged_arg0K1 = list_arg_count_1051X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      goto L26226;}}
  else {
    *((long *) loc_1521X) = (*((long *) arg_1522X));
    arg1K0 = (loc_1521X + -4);
    arg1K1 = (arg_1522X + -4);
    goto L26239;}}
 L23029: {
  arg_count_1523X = arg0K0;
  handlers_1524X = SHARED_REF((Sinterrupt_handlersS));
  if ((3 == (3 & handlers_1524X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_1524X))) + -4))))>>2))))) {
      goto L23047;}
    else {
      goto L23089;}}
  else {
    goto L23089;}}
 L11546: {
  channel_1525X = arg0K0;
  x_1526X = 1 == (Spending_channels_headS);
  if (x_1526X) {
    goto L11560;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Sstack_limitS = (Sreal_stack_limitS);
      if ((s48_Spending_eventsPS)) {
        s48_Sstack_limitS = (((char *) -1));
        goto L11560;}
      else {
        goto L11560;}}
    else {
      s48_Sstack_limitS = (((char *) -1));
      goto L11560;}}}
 L8188: {
  count_1527X = arg0K0;
  i_1528X = arg0K1;
  offset_1529X = arg0K2;
  if ((0 == count_1527X)) {
    if ((i_1528X < total_count_1069X)) {
      arg0K0 = i_1528X;
      arg0K1 = offset_1529X;
      goto L6820;}
    else {
      arg0K0 = offset_1529X;
      goto L52546;}}
  else {
    value_1530X = *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1529X)))))<<2))));
    addr_1531X = (((char *) (-3 + new_env_1066X))) + (((i_1528X)<<2));S48_WRITE_BARRIER(new_env_1066X, addr_1531X, value_1530X);
    *((long *) addr_1531X) = value_1530X;
    arg0K0 = (-1 + count_1527X);
    arg0K1 = (1 + i_1528X);
    arg0K2 = (1 + offset_1529X);
    goto L8188;}}
 L10762: {
  count_1532X = arg0K0;
  i_1533X = arg0K1;
  offset_1534X = arg0K2;
  if ((0 == count_1532X)) {
    if ((i_1533X < total_count_1078X)) {
      arg0K0 = i_1533X;
      arg0K1 = offset_1534X;
      goto L7533;}
    else {
      arg0K0 = offset_1534X;
      goto L52536;}}
  else {
    value_1535X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1534X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1534X))))))<<2))));
    addr_1536X = (((char *) (-3 + new_env_1075X))) + (((i_1533X)<<2));S48_WRITE_BARRIER(new_env_1075X, addr_1536X, value_1535X);
    *((long *) addr_1536X) = value_1535X;
    arg0K0 = (-1 + count_1532X);
    arg0K1 = (1 + i_1533X);
    arg0K2 = (2 + offset_1534X);
    goto L10762;}}
 L40340: {
  pop_continuationB_return_tag = 6;
  goto pop_continuationB;
 pop_continuationB_return_6:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L23319: {
  cont_1537X = arg0K0;
  if ((1 == cont_1537X)) {
    if ((0 == (3 & (SvalS)))) {
      s48_Scallback_return_stack_blockS = 1;
      return ((((SvalS))>>2));}
    else {
      goto L23334;}}
  else {
    goto L23334;}}
 L40482: {
  v_1538X = arg0K0;
  ScontS = (cont_1111X + (4 + (((v_1538X)<<2))));
  merged_arg0K0 = 0;
  move_args_above_contB_return_tag = 14;
  goto move_args_above_contB;
 move_args_above_contB_return_14:
  goto L40387;}
 L40387: {
  x_1539X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1539X;
  SvalS = proc_1110X;
  arg0K0 = 1;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L32292;}
 L26493: {
  code_1540X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1540X)))))<<2)), code_1540X, 20, 32);
  n_1541X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1541X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = template_614X;
  arg0K0 = 1;
  goto L22779;}
 L14114: {
  if ((3 == (3 & x_637X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_637X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L50569;}
    else {
      goto L14120;}}
  else {
    goto L14120;}}
 L41623: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L41756: {
  if ((3 == (3 & n_639X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_639X))) + -4))))>>2))))) {
      goto L41767;}
    else {
      goto L41768;}}
  else {
    goto L41768;}}
 L41947: {
  if ((3 == (3 & n_640X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_640X))) + -4))))>>2))))) {
      goto L41958;}
    else {
      goto L41959;}}
  else {
    goto L41959;}}
 L42138: {
  if ((3 == (3 & n_641X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_641X))) + -4))))>>2))))) {
      goto L42149;}
    else {
      goto L42150;}}
  else {
    goto L42150;}}
 L33949: {
  if ((0 == (3 & arg2_643X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18166;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_643X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18166;}}
 L33966: {
  code_1542X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1542X)))))<<2)), code_1542X, 20, 4);
  n_1543X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1543X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_643X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_644X;
  arg0K0 = 2;
  goto L22779;}
 L8351: {
  b_1544X = arg0K0;
  lo_a_1545X = 65535 & a_1142X;
  lo_b_1546X = 65535 & b_1544X;
  hi_a_1547X = 65535 & (((a_1142X)>>16));
  hi_b_1548X = 65535 & (((b_1544X)>>16));
  lo_c_1549X = SMALL_MULTIPLY(lo_a_1545X, lo_b_1546X);
  v_1550X = SMALL_MULTIPLY(lo_b_1546X, hi_a_1547X);
  v_1551X = SMALL_MULTIPLY(lo_a_1545X, hi_b_1548X);
  mid_c_1552X = v_1551X + v_1550X;
  c_1553X = lo_c_1549X + (((mid_c_1552X)<<16));
  if ((0 < hi_a_1547X)) {
    if ((0 < hi_b_1548X)) {
      arg0K0 = arg2_646X;
      arg0K1 = y_647X;
      goto L19909;}
    else {
      goto L8393;}}
  else {
    goto L8393;}}
 L44028: {
  arg0K0 = arg2_646X;
  arg0K1 = y_647X;
  goto L19909;}
 L34254: {
  if ((0 == (3 & arg2_650X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18123;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_650X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18123;}}
 L34271: {
  code_1554X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1554X)))))<<2)), code_1554X, 20, 4);
  n_1555X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1555X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_650X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_651X;
  arg0K0 = 2;
  goto L22779;}
 L8609: {
  b_1556X = arg0K0;
  c_1557X = a_1162X / b_1556X;
  x_1558X = 0 == (a_1162X % b_1556X);
  if (x_1558X) {
    if ((a_655X < 0)) {
      if ((b_656X < 0)) {s48_make_availableAgc(16);
        if ((536870911 < c_1557X)) {
          goto L51797;}
        else {
          if ((c_1557X < -536870912)) {
            goto L51797;}
          else {
            arg0K0 = (((c_1557X)<<2));
            goto L51792;}}}
      else {
        goto L8655;}}
    else {
      if ((b_656X < 0)) {
        goto L8655;}
      else {s48_make_availableAgc(16);
        if ((536870911 < c_1557X)) {
          goto L51819;}
        else {
          if ((c_1557X < -536870912)) {
            goto L51819;}
          else {
            arg0K0 = (((c_1557X)<<2));
            goto L51814;}}}}}
  else {
    arg0K0 = arg2_653X;
    arg0K1 = y_654X;
    goto L23692;}}
 L44271: {
  if ((0 == (3 & arg2_653X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17691;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_653X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17691;}}
 L44311: {
  result_1559X = arg0K0;
  if ((1 == result_1559X)) {
    arg0K0 = arg2_653X;
    arg0K1 = y_654X;
    goto L23692;}
  else {
    SvalS = result_1559X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}}
 L34555: {
  if ((0 == (3 & (arg2_657X | y_658X)))) {
    if ((arg2_657X == y_658X)) {
      arg0K0 = 5;
      goto L34559;}
    else {
      arg0K0 = 1;
      goto L34559;}}
  else {
    if ((3 == (3 & arg2_657X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_657X))) + -4))))>>2))))) {
        if ((3 == (3 & y_658X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_658X))) + -4))))>>2))))) {
            b_1560X = s48_bignum_equal_p((((char *) (-3 + arg2_657X))), (((char *) (-3 + y_658X))));
            if (b_1560X) {
              arg0K0 = 5;
              goto L34559;}
            else {
              arg0K0 = 1;
              goto L34559;}}
          else {
            arg0K0 = 1;
            goto L34559;}}
        else {
          arg0K0 = 1;
          goto L34559;}}
      else {
        arg0K0 = 1;
        goto L34559;}}
    else {
      arg0K0 = 1;
      goto L34559;}}}
 L34575: {
  val_1561X = arg0K0;
  SvalS = val_1561X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L34576: {
  code_1562X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1562X)))))<<2)), code_1562X, 20, 4);
  n_1563X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1563X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_657X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_658X;
  arg0K0 = 2;
  goto L22779;}
 L34831: {
  if ((0 == (3 & arg2_659X))) {
    if ((0 == (3 & y_660X))) {
      if ((arg2_659X < y_660X)) {
        arg0K0 = 5;
        goto L34835;}
      else {
        arg0K0 = 1;
        goto L34835;}}
    else {
      v_1564X = s48_bignum_test((((char *) (-3 + y_660X))));
      if ((1 == v_1564X)) {
        arg0K0 = 5;
        goto L34835;}
      else {
        arg0K0 = 1;
        goto L34835;}}}
  else {
    if ((0 == (3 & y_660X))) {
      v_1565X = s48_bignum_test((((char *) (-3 + arg2_659X))));
      if ((1 == v_1565X)) {
        arg0K0 = 1;
        goto L34835;}
      else {
        arg0K0 = 5;
        goto L34835;}}
    else {
      v_1566X = s48_bignum_compare((((char *) (-3 + arg2_659X))), (((char *) (-3 + y_660X))));
      if ((-1 == v_1566X)) {
        arg0K0 = 5;
        goto L34835;}
      else {
        arg0K0 = 1;
        goto L34835;}}}}
 L34851: {
  val_1567X = arg0K0;
  SvalS = val_1567X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L34852: {
  code_1568X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1568X)))))<<2)), code_1568X, 20, 4);
  n_1569X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1569X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_659X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_660X;
  arg0K0 = 2;
  goto L22779;}
 L35170: {
  if ((0 == (3 & y_662X))) {
    if ((0 == (3 & arg2_661X))) {
      if ((y_662X < arg2_661X)) {
        arg0K0 = 5;
        goto L35174;}
      else {
        arg0K0 = 1;
        goto L35174;}}
    else {
      v_1570X = s48_bignum_test((((char *) (-3 + arg2_661X))));
      if ((1 == v_1570X)) {
        arg0K0 = 5;
        goto L35174;}
      else {
        arg0K0 = 1;
        goto L35174;}}}
  else {
    if ((0 == (3 & arg2_661X))) {
      v_1571X = s48_bignum_test((((char *) (-3 + y_662X))));
      if ((1 == v_1571X)) {
        arg0K0 = 1;
        goto L35174;}
      else {
        arg0K0 = 5;
        goto L35174;}}
    else {
      v_1572X = s48_bignum_compare((((char *) (-3 + y_662X))), (((char *) (-3 + arg2_661X))));
      if ((-1 == v_1572X)) {
        arg0K0 = 5;
        goto L35174;}
      else {
        arg0K0 = 1;
        goto L35174;}}}}
 L35190: {
  val_1573X = arg0K0;
  SvalS = val_1573X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35191: {
  code_1574X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1574X)))))<<2)), code_1574X, 20, 4);
  n_1575X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1575X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_661X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_662X;
  arg0K0 = 2;
  goto L22779;}
 L35509: {
  if ((0 == (3 & y_664X))) {
    if ((0 == (3 & arg2_663X))) {
      if ((y_664X < arg2_663X)) {
        arg0K0 = 1;
        goto L35513;}
      else {
        arg0K0 = 5;
        goto L35513;}}
    else {
      v_1576X = s48_bignum_test((((char *) (-3 + arg2_663X))));
      if ((1 == v_1576X)) {
        arg0K0 = 1;
        goto L35513;}
      else {
        arg0K0 = 5;
        goto L35513;}}}
  else {
    if ((0 == (3 & arg2_663X))) {
      v_1577X = s48_bignum_test((((char *) (-3 + y_664X))));
      if ((1 == v_1577X)) {
        arg0K0 = 5;
        goto L35513;}
      else {
        arg0K0 = 1;
        goto L35513;}}
    else {
      v_1578X = s48_bignum_compare((((char *) (-3 + y_664X))), (((char *) (-3 + arg2_663X))));
      if ((-1 == v_1578X)) {
        arg0K0 = 1;
        goto L35513;}
      else {
        arg0K0 = 5;
        goto L35513;}}}}
 L35529: {
  val_1579X = arg0K0;
  SvalS = val_1579X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35530: {
  code_1580X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1580X)))))<<2)), code_1580X, 20, 4);
  n_1581X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1581X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_663X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_664X;
  arg0K0 = 2;
  goto L22779;}
 L35819: {
  if ((0 == (3 & arg2_665X))) {
    if ((0 == (3 & y_666X))) {
      if ((arg2_665X < y_666X)) {
        arg0K0 = 1;
        goto L35823;}
      else {
        arg0K0 = 5;
        goto L35823;}}
    else {
      v_1582X = s48_bignum_test((((char *) (-3 + y_666X))));
      if ((1 == v_1582X)) {
        arg0K0 = 1;
        goto L35823;}
      else {
        arg0K0 = 5;
        goto L35823;}}}
  else {
    if ((0 == (3 & y_666X))) {
      v_1583X = s48_bignum_test((((char *) (-3 + arg2_665X))));
      if ((1 == v_1583X)) {
        arg0K0 = 5;
        goto L35823;}
      else {
        arg0K0 = 1;
        goto L35823;}}
    else {
      v_1584X = s48_bignum_compare((((char *) (-3 + arg2_665X))), (((char *) (-3 + y_666X))));
      if ((-1 == v_1584X)) {
        arg0K0 = 1;
        goto L35823;}
      else {
        arg0K0 = 5;
        goto L35823;}}}}
 L35839: {
  val_1585X = arg0K0;
  SvalS = val_1585X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35840: {
  code_1586X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1586X)))))<<2)), code_1586X, 20, 4);
  n_1587X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1587X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_665X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_666X;
  arg0K0 = 2;
  goto L22779;}
 L19848: {
  length0_1588X = arg0K0;
  extra0_1589X = arg0K1;
  if ((0 == (3 & y_1178X))) {
    arg0K0 = 3;
    goto L19850;}
  else {
    arg0K0 = 0;
    goto L19850;}}
 L9223: {
  b_1590X = arg0K0;
  c_1591X = a_1179X / b_1590X;
  if ((a_669X < 0)) {
    if ((b_670X < 0)) {
      goto L9269;}
    else {
      goto L9268;}}
  else {
    if ((b_670X < 0)) {
      goto L9268;}
    else {
      goto L9269;}}}
 L36135: {
  arg0K0 = arg2_667X;
  arg0K1 = y_668X;
  goto L19818;}
 L19757: {
  length0_1592X = arg0K0;
  extra0_1593X = arg0K1;
  if ((0 == (3 & y_1181X))) {
    arg0K0 = 3;
    goto L19759;}
  else {
    arg0K0 = 0;
    goto L19759;}}
 L36311: {
  b_1594X = arg0K0;
  c_1595X = a_1182X % b_1594X;
  if ((a_673X < 0)) {
    arg0K0 = (0 - c_1595X);
    goto L36315;}
  else {
    arg0K0 = c_1595X;
    goto L36315;}}
 L36289: {
  arg0K0 = arg2_671X;
  arg0K1 = y_672X;
  goto L19727;}
 L23599: {
  x_1596X = arg0K0;
  if ((536870911 < x_1596X)) {
    goto L23600;}
  else {
    if ((x_1596X < -536870912)) {
      goto L23600;}
    else {
      return (((x_1596X)<<2));}}}
 L19335: {
  length_1597X = arg0K0;
  extra_1598X = arg0K1;
  Stemp0S = x_1194X;s48_make_availableAgc(((((length_1597X + extra_1598X))<<2)));
  value_1599X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1599X))) {
    v_1600X = (char *) s48_long_to_bignum((((value_1599X)>>2)));
    arg1K0 = v_1600X;
    goto L19327;}
  else {
    arg1K0 = (((char *) (-3 + value_1599X)));
    goto L19327;}}
 L19682: {
  length_1601X = arg0K0;
  extra_1602X = arg0K1;
  if ((length_1601X < 1)) {
    arg0K0 = 1;
    goto L19684;}
  else {
    arg0K0 = length_1601X;
    goto L19684;}}
 L37013: {
  x_1603X = arg0K0;
  count_1604X = arg0K1;
  if ((0 == x_1603X)) {
    SvalS = (((count_1604X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    arg0K0 = (((x_1603X)>>1));
    arg0K1 = (count_1604X + (1 & x_1603X));
    goto L37013;}}
 L19624: {
  length_1605X = arg0K0;
  extra_1606X = arg0K1;
  if ((length_1605X < 1)) {
    arg0K0 = 1;
    goto L19626;}
  else {
    arg0K0 = length_1605X;
    goto L19626;}}
 L37114: {
  if ((0 == (3 & arg2_688X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17905;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_688X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17905;}}
 L37293: {
  if ((0 == (3 & arg2_690X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17862;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_690X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17862;}}
 L37472: {
  if ((0 == (3 & arg2_692X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17819;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_692X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17819;}}
 L44655: {
  v_1607X = (char *) s48_long_to_bignum(x_1216X);
  v_1608X = enter_bignum(v_1607X);
  arg0K0 = v_1608X;
  goto L44650;}
 L44650: {
  val_1609X = arg0K0;
  SvalS = val_1609X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L44677: {
  v_1610X = (char *) s48_long_to_bignum(result_1218X);
  v_1611X = enter_bignum(v_1610X);
  arg0K0 = v_1611X;
  goto L44672;}
 L44672: {
  val_1612X = arg0K0;
  SvalS = val_1612X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L44560: {
  x_1613X = arg0K0;
  y_1614X = arg0K1;
  y_1615X = ((y_1614X)>>2);
  merged_arg0K0 = x_1613X;
  merged_arg0K1 = y_1615X;
  shift_space_return_tag = 1;
  goto shift_space;
 shift_space_return_1:
  needed_1616X = shift_space0_return_value;
  Stemp0S = x_1613X;s48_make_availableAgc((((needed_1616X)<<2)));
  value_1617X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1617X))) {
    v_1618X = (char *) s48_long_to_bignum((((value_1617X)>>2)));
    arg1K0 = v_1618X;
    goto L44622;}
  else {
    arg1K0 = (((char *) (-3 + value_1617X)));
    goto L44622;}}
 L44699: {
  v_1619X = (char *) s48_long_to_bignum(result_1218X);
  v_1620X = enter_bignum(v_1619X);
  arg0K0 = v_1620X;
  goto L44694;}
 L44694: {
  val_1621X = arg0K0;
  SvalS = val_1621X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L44730: {
  x_1622X = arg1K0;
  external_bignum_1623X = (char *) s48_bignum_arithmetic_shift(x_1622X, y_1220X);
  v_1624X = s48_bignum_fits_in_word_p(external_bignum_1623X, 30, 1);
  if (v_1624X) {
    n_1625X = s48_bignum_to_long(external_bignum_1623X);
    arg0K0 = (((n_1625X)<<2));
    goto L44568;}
  else {
    v_1626X = enter_bignum(external_bignum_1623X);
    arg0K0 = v_1626X;
    goto L44568;}}
 L28074: {
  i_1627X = arg0K0;
  rest_list_1628X = arg0K1;
  if ((25 == rest_list_1628X)) {
    SvalS = new_727X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    *((long *) ((((char *) (-3 + new_727X))) + (((i_1627X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_1628X)))));
    arg0K0 = (1 + i_1627X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_1628X))) + 4)));
    goto L28074;}}
 L28519: {
  i_1629X = arg0K0;
  if ((i_1629X < 0)) {
    SvalS = x_1250X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    addr_1630X = (((char *) (-3 + x_1250X))) + (((i_1629X)<<2));S48_WRITE_BARRIER(x_1250X, addr_1630X, value_1251X);
    *((long *) addr_1630X) = value_1251X;
    arg0K0 = (-1 + i_1629X);
    goto L28519;}}
 L37754: {
  i_1631X = arg0K0;
  if ((i_1631X < 0)) {
    SvalS = vector_1266X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1266X))) + i_1631X)) = init_763X;
    arg0K0 = (-1 + i_1631X);
    goto L37754;}}
 L37966: {
  i_1632X = arg0K0;
  if ((i_1632X < 0)) {
    SvalS = vector_1286X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1286X))) + i_1632X)) = (init_779X);
    arg0K0 = (-1 + i_1632X);
    goto L37966;}}
 L20905: {
  bucket_1633X = arg0K0;
  arg0K0 = bucket_1633X;
  goto L20911;}
 L42867: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L38190: {
  index_1634X = arg0K0;
  channel_1635X = make_registered_channel(mode_803X, arg2_802X, index_1634X, 0, &reason_1636X);
  if ((1 == channel_1635X)) {
    if ((3 == (3 & arg2_802X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_802X))) + -4))))>>2))))) {
        if ((1 == mode_803X)) {
          goto L38321;}
        else {
          if ((3 == mode_803X)) {
            goto L38321;}
          else {
            v_1637X = ps_close_fd(index_1634X);
            arg0K0 = v_1637X;
            goto L38316;}}}
      else {
        arg0K0 = reason_1636X;
        goto L38160;}}
    else {
      arg0K0 = reason_1636X;
      goto L38160;}}
  else {
    SvalS = channel_1635X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}}
 L38248: {
  v_1638X = ps_open_fd((((char *)(((char *) (-3 + arg2_802X))))), 1, &v_1639X);
  arg0K0 = v_1638X;
  arg0K1 = v_1639X;
  goto L38261;}
 L38261: {
  channel_1640X = arg0K0;
  status_1641X = arg0K1;
  if ((status_1641X == NO_ERRORS)) {
    arg0K0 = channel_1640X;
    goto L38190;}
  else {
    if ((status_1641X == ENOENT)) {
      arg0K0 = 10;
      goto L38160;}
    else {
      code_1642X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1642X)))))<<2)), code_1642X, 96, 4);
      n_1643X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1643X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 96;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_802X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_803X)<<2));
      merged_arg0K0 = status_1641X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_1644X = get_error_string0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1644X;
      arg0K0 = 3;
      goto L22779;}}}
 L46475: {
  waitP_1645X = arg2K0;
  start_1646X = ((arg3_816X)>>2);
  count_1647X = ((arg2_815X)>>2);
  v_1648X = 4 == (*((long *) (((char *) (-3 + arg5_818X)))));
  if (v_1648X) {
    if ((3 == (3 & arg4_817X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_817X))) + -4))))>>2))))) {
        goto L38576;}
      else {
        goto L38823;}}
    else {
      goto L38823;}}
  else {
    goto L38644;}}
 L38987: {
  y_1649X = arg0K0;
  if ((y_1649X < (start_823X + count_824X))) {
    goto L39039;}
  else {
    got_1650X = ps_write_fd(((((*((long *) ((((char *) (-3 + arg4_822X))) + 8))))>>2)), ((((char *) (-3 + arg3_821X))) + start_823X), count_824X, &pendingP_1651X, &status_1652X);
    if ((status_1652X == NO_ERRORS)) {
      if (pendingP_1651X) {
        arg0K0 = 1;
        goto L39038;}
      else {
        arg0K0 = (((got_1650X)<<2));
        goto L39038;}}
    else {
      code_1653X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1653X)))))<<2)), code_1653X, 96, 4);
      n_1654X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1654X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 96;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_822X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg3_821X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_823X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_824X)<<2));
      merged_arg0K0 = status_1652X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 4;
      goto get_error_string;
     get_error_string_return_4:
      x_1655X = get_error_string0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1655X;
      arg0K0 = 5;
      goto L22779;}}}
 L39206: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_821X))) + -4))))>>8));
  goto L38987;}
 L11385: {
  val_1656X = *((long *) ((((char *) (-3 + ch_1345X))) + 12));
  addr_1657X = (((char *) (-3 + prev_1346X))) + 12;S48_WRITE_BARRIER(prev_1346X, addr_1657X, val_1656X);
  *((long *) addr_1657X) = val_1656X;
  addr_1658X = (((char *) (-3 + ch_1345X))) + 12;S48_WRITE_BARRIER(ch_1345X, addr_1658X, 1);
  *((long *) addr_1658X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_1345X))) + 16)));
  goto L46859;}
 L16958: {
  v_1659X = arg0K0;
  arg0K0 = (-1 + i_1353X);
  arg0K1 = v_1659X;
  goto L16944;}
 L39341: {
  if ((1 == proposal_843X)) {
    goto L39353;}
  else {
    addr_1660X = ((char *) (-3 + proposal_843X));S48_WRITE_BARRIER(proposal_843X, addr_1660X, 5);
    *((long *) addr_1660X) = 5;
    goto L39353;}}
 L10371: {
  i_1661X = arg0K0;
  stob_1662X = *((long *) ((((char *) (-3 + log_1364X))) + (((i_1661X)<<2))));
  if ((1 == stob_1662X)) {
    copies_1663X = *((long *) ((((char *) (-3 + proposal_844X))) + 12));
    arg0K0 = copies_1663X;
    goto L39719;}
  else {
    verify_1664X = *((long *) ((((char *) (-3 + log_1364X))) + (12 + (((i_1661X)<<2)))));
    value_1665X = *((long *) ((((char *) (-3 + log_1364X))) + (8 + (((i_1661X)<<2)))));
    if ((29 == verify_1664X)) {
      if ((3 == (3 & stob_1662X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1662X))) + -4)))))) {
          goto L10423;}
        else {
          goto L39603;}}
      else {
        goto L39603;}}
    else {
      if ((verify_1664X == ((((*((unsigned char *) ((((char *) (-3 + stob_1662X))) + ((((*((long *) ((((char *) (-3 + log_1364X))) + (4 + (((i_1661X)<<2)))))))>>2))))))<<2)))) {
        if ((verify_1664X == value_1665X)) {
          goto L10423;}
        else {
          if ((3 == (3 & stob_1662X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1662X))) + -4)))))) {
              goto L10423;}
            else {
              goto L39603;}}
          else {
            goto L39603;}}}
      else {
        goto L39603;}}}}
 L10185: {
  arg0K0 = (4 + i_1362X);
  goto L10133;}
 L39603: {
RELEASE_PROPOSAL_LOCK();
  x_1666X = Scurrent_threadS;
  addr_1667X = (((char *) (-3 + x_1666X))) + 12;S48_WRITE_BARRIER(x_1666X, addr_1667X, 1);
  *((long *) addr_1667X) = 1;
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L9699: {
  vector_1668X = arg0K0;
  if ((1 == vector_1668X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1669X;
    goto L9662;}
  else {
    arg0K0 = vector_1668X;
    goto L9662;}}
 L15157: {
  arg0K0 = (4 + i_1381X);
  goto L15135;}
 L15331: {
  arg0K0 = (4 + i_1389X);
  goto L15311;}
 L47815: {
  minutesP_1670X = arg2K0;
  pending_interruptP_return_tag = 3;
  goto pending_interruptP;
 pending_interruptP_return_3:
  x_1671X = pending_interruptP0_return_value;
  if (x_1671X) {
    goto L47845;}
  else {
    if ((0 == (Spending_interruptsS))) {s48_wait_for_event((((arg2_940X)>>2)), minutesP_1670X);
      goto L47845;}
    else {
      goto L47845;}}}
 L30623: {
  code_1672X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1672X)))))<<2)), code_1672X, 20, 4);
  n_1673X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1673X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_1439X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = name_1440X;
  arg0K0 = 2;
  goto L22779;}
 L47093: {
  val_1674X = arg0K0;
  SvalS = val_1674X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L32186: {
  table_1675X = arg0K0;
  n_1676X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_951X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L20418;}
 L40144: {
  l_1677X = arg0K0;
  i_1678X = arg0K1;
  if ((i_1678X < 0)) {
    SvalS = string_1463X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1463X))) + i_1678X)) = ((((((*((long *) (((char *) (-3 + l_1677X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_1677X))) + 4)));
    arg0K1 = (-1 + i_1678X);
    goto L40144;}}
 L42524: {
  code_1679X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1679X)))))<<2)), code_1679X, 20, 4);
  n_1680X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1680X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_988X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = list_989X;
  arg0K0 = 2;
  goto L22779;}
 L31367: {
  code_1681X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1681X)))))<<2)), code_1681X, 20, 8);
  n_1682X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1682X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1481X;
  arg0K0 = 1;
  goto L22779;}
 L31617: {
  code_1683X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1683X)))))<<2)), code_1683X, 20, 8);
  n_1684X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1684X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1491X;
  arg0K0 = 1;
  goto L22779;}
 L31863: {
  code_1685X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1685X)))))<<2)), code_1685X, 20, 8);
  n_1686X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1686X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = Kchar_1499X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1500X;
  arg0K0 = 2;
  goto L22779;}
 L42774: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_1509X))) + 4)));
  goto L42768;}
 L16221: {
  if ((3 == (3 & thing_1510X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_1510X)))));
      goto L16279;}
    else {
      goto L16229;}}
  else {
    goto L16229;}}
 L42759: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_1020X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L26226: {
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1048X));
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L23047: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_1524X))) + (((i_1053X)<<2)))));
  obj_1687X = SvalS;
  if ((3 == (3 & obj_1687X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1687X))) + -4))))>>2))))) {
      goto L23064;}
    else {
      goto L23103;}}
  else {
    goto L23103;}}
 L23089: {
  ps_error("interrupt handler is not a vector", 0);
  goto L23047;}
 L11560: {
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = channel_1525X;
  x_1688X = *((long *) ((((char *) (-3 + channel_1525X))) + 16));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1688X;
  n_1689X = Senabled_interruptsS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1689X)<<2));
  arg0K0 = 3;
  goto L23029;}
 L6820: {
  i_1690X = arg0K0;
  offset_1691X = arg0K1;
  if ((i_1690X == total_count_1069X)) {
    arg0K0 = offset_1691X;
    goto L52546;}
  else {
    env_1692X = *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1691X)))))<<2))));
    count_1693X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_1691X)));
    arg0K0 = count_1693X;
    arg0K1 = i_1690X;
    arg0K2 = (2 + offset_1691X);
    goto L6837;}}
 L52546: {
  bytes_used_1694X = arg0K0;
  SvalS = new_env_573X;
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1694X));
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L7533: {
  i_1695X = arg0K0;
  offset_1696X = arg0K1;
  if ((i_1695X == total_count_1078X)) {
    arg0K0 = offset_1696X;
    goto L52536;}
  else {
    env_1697X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1696X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1696X))))))<<2))));
    index_1698X = 2 + offset_1696X;
    count_1699X = ((((*((unsigned char *) ((Scode_pointerS) + (1 + index_1698X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_1698X))));
    arg0K0 = count_1699X;
    arg0K1 = i_1695X;
    arg0K2 = (4 + offset_1696X);
    goto L7550;}}
 L52536: {
  bytes_used_1700X = arg0K0;
  SvalS = new_env_579X;
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1700X));
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L23334: {
  SstackS = (Sbottom_of_stackS);
  Sheap_continuationS = 1;
  ScontS = (Sbottom_of_stackS);
  code_1701X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1701X)))))<<2)), code_1701X, 20, 0);
  n_1702X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1702X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1703X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1703X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_1537X;
  arg0K0 = 2;
  goto L22779;}
 L14120: {
  if ((3 == (3 & x_637X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + x_637X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L50569;}
    else {
      arg0K0 = 1;
      goto L50569;}}
  else {
    arg0K0 = 1;
    goto L50569;}}
 L41768: {
  if ((3 == (3 & n_639X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_639X))) + -4))))>>2))))) {
      arg0K0 = n_639X;
      goto L23661;}
    else {
      goto L41772;}}
  else {
    goto L41772;}}
 L41959: {
  if ((3 == (3 & n_640X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_640X))) + -4))))>>2))))) {
      arg0K0 = n_640X;
      goto L23661;}
    else {
      goto L41963;}}
  else {
    goto L41963;}}
 L42150: {
  if ((3 == (3 & n_641X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_641X))) + -4))))>>2))))) {
      arg0K0 = n_641X;
      goto L23661;}
    else {
      goto L42154;}}
  else {
    goto L42154;}}
 L18166: {
  length0_1704X = arg0K0;
  extra0_1705X = arg0K1;
  if ((0 == (3 & y_644X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18174;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_644X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18174;}}
 L19909: {
  x_1706X = arg0K0;
  y_1707X = arg0K1;
  if ((0 == (3 & x_1706X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18086;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1706X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18086;}}
 L8393: {
  if ((536870911 < lo_c_1549X)) {
    arg0K0 = arg2_646X;
    arg0K1 = y_647X;
    goto L19909;}
  else {
    if ((lo_c_1549X < 0)) {
      arg0K0 = arg2_646X;
      arg0K1 = y_647X;
      goto L19909;}
    else {
      if ((8192 < mid_c_1552X)) {
        arg0K0 = arg2_646X;
        arg0K1 = y_647X;
        goto L19909;}
      else {
        if ((a_648X < 0)) {
          if ((b_649X < 0)) {s48_make_availableAgc(16);
            if ((536870911 < c_1553X)) {
              goto L51706;}
            else {
              if ((c_1553X < -536870912)) {
                goto L51706;}
              else {
                arg0K0 = (((c_1553X)<<2));
                goto L51701;}}}
          else {
            goto L8421;}}
        else {
          if ((b_649X < 0)) {
            goto L8421;}
          else {s48_make_availableAgc(16);
            if ((536870911 < c_1553X)) {
              goto L51728;}
            else {
              if ((c_1553X < -536870912)) {
                goto L51728;}
              else {
                arg0K0 = (((c_1553X)<<2));
                goto L51723;}}}}}}}}
 L18123: {
  length0_1708X = arg0K0;
  extra0_1709X = arg0K1;
  if ((0 == (3 & y_651X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18131;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_651X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18131;}}
 L51797: {
  v_1710X = (char *) s48_long_to_bignum(c_1557X);
  v_1711X = enter_bignum(v_1710X);
  arg0K0 = v_1711X;
  goto L51792;}
 L51792: {
  val_1712X = arg0K0;
  SvalS = val_1712X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L8655: {
  x_1713X = 0 - c_1557X;s48_make_availableAgc(16);
  if ((536870911 < x_1713X)) {
    goto L51775;}
  else {
    if ((x_1713X < -536870912)) {
      goto L51775;}
    else {
      arg0K0 = (((x_1713X)<<2));
      goto L51770;}}}
 L51819: {
  v_1714X = (char *) s48_long_to_bignum(c_1557X);
  v_1715X = enter_bignum(v_1714X);
  arg0K0 = v_1715X;
  goto L51814;}
 L51814: {
  val_1716X = arg0K0;
  SvalS = val_1716X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L17691: {
  length0_1717X = arg0K0;
  extra0_1718X = arg0K1;
  if ((0 == (3 & y_654X))) {
    arg0K0 = 3;
    goto L17699;}
  else {
    arg0K0 = 0;
    goto L17699;}}
 L34559: {
  val_1719X = arg0K0;
  SvalS = val_1719X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L34835: {
  val_1720X = arg0K0;
  SvalS = val_1720X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35174: {
  val_1721X = arg0K0;
  SvalS = val_1721X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35513: {
  val_1722X = arg0K0;
  SvalS = val_1722X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L35823: {
  val_1723X = arg0K0;
  SvalS = val_1723X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L19850: {
  extra1_1724X = arg0K0;
  Stemp0S = x_1177X;
  Stemp1S = y_1178X;s48_make_availableAgc(((((((8 + (((length0_1588X)<<1))) + extra0_1589X) + extra1_1724X))<<2)));
  value_1725X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1725X))) {
    v_1726X = (char *) s48_long_to_bignum((((value_1725X)>>2)));
    arg1K0 = v_1726X;
    goto L19836;}
  else {
    arg1K0 = (((char *) (-3 + value_1725X)));
    goto L19836;}}
 L9269: {
  if ((536870911 < c_1591X)) {
    arg0K0 = arg2_667X;
    arg0K1 = y_668X;
    goto L19818;}
  else {
    SvalS = (((c_1591X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}}
 L9268: {
  SvalS = ((((0 - c_1591X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L19759: {
  extra1_1727X = arg0K0;
  Stemp0S = x_1180X;
  Stemp1S = y_1181X;s48_make_availableAgc(((((((8 + (((length0_1592X)<<1))) + extra0_1593X) + extra1_1727X))<<2)));
  value_1728X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1728X))) {
    v_1729X = (char *) s48_long_to_bignum((((value_1728X)>>2)));
    arg1K0 = v_1729X;
    goto L19745;}
  else {
    arg1K0 = (((char *) (-3 + value_1728X)));
    goto L19745;}}
 L36315: {
  n_1730X = arg0K0;
  SvalS = (((n_1730X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L23600: {
  v_1731X = (char *) s48_long_to_bignum(x_1596X);
  return enter_bignum(v_1731X);}
 L19327: {
  x_1732X = arg1K0;
  v_1733X = s48_bignum_test(x_1732X);
  if ((-1 == v_1733X)) {
    v_1734X = (char *) s48_bignum_negate(x_1732X);
    arg1K0 = v_1734X;
    goto L19329;}
  else {
    arg1K0 = x_1732X;
    goto L19329;}}
 L19684: {
  v_1735X = arg0K0;
  Stemp0S = x_685X;s48_make_availableAgc((((((3 + v_1735X) + extra_1602X))<<2)));
  value_1736X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1736X))) {
    v_1737X = (char *) s48_long_to_bignum((((value_1736X)>>2)));
    arg1K0 = v_1737X;
    goto L19674;}
  else {
    arg1K0 = (((char *) (-3 + value_1736X)));
    goto L19674;}}
 L19626: {
  v_1738X = arg0K0;
  Stemp0S = x_686X;s48_make_availableAgc((((((3 + v_1738X) + extra_1606X))<<2)));
  value_1739X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1739X))) {
    v_1740X = (char *) s48_long_to_bignum((((value_1739X)>>2)));
    arg1K0 = v_1740X;
    goto L19616;}
  else {
    arg1K0 = (((char *) (-3 + value_1739X)));
    goto L19616;}}
 L17905: {
  length0_1741X = arg0K0;
  extra0_1742X = arg0K1;
  if ((0 == (3 & y_689X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17913;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_689X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17913;}}
 L17862: {
  length0_1743X = arg0K0;
  extra0_1744X = arg0K1;
  if ((0 == (3 & y_691X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17870;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_691X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17870;}}
 L17819: {
  length0_1745X = arg0K0;
  extra0_1746X = arg0K1;
  if ((0 == (3 & y_693X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17827;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_693X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17827;}}
 L44622: {
  x_1747X = arg1K0;
  external_bignum_1748X = (char *) s48_bignum_arithmetic_shift(x_1747X, y_1615X);
  v_1749X = s48_bignum_fits_in_word_p(external_bignum_1748X, 30, 1);
  if (v_1749X) {
    n_1750X = s48_bignum_to_long(external_bignum_1748X);
    arg0K0 = (((n_1750X)<<2));
    goto L44562;}
  else {
    v_1751X = enter_bignum(external_bignum_1748X);
    arg0K0 = v_1751X;
    goto L44562;}}
 L44568: {
  val_1752X = arg0K0;
  SvalS = val_1752X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L20911: {
  foo_1753X = arg0K0;
  if ((1 == foo_1753X)) {
    if ((3 == (3 & bucket_1633X))) {
      arg0K0 = (-4 & bucket_1633X);
      goto L20916;}
    else {
      arg0K0 = bucket_1633X;
      goto L20916;}}
  else {
    s2_1754X = *((long *) (((char *) (-3 + foo_1753X))));
    len_1755X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_793X))) + -4))))>>8);
    if ((len_1755X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1754X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1754X))), (void *)(((char *) (-3 + string_793X))),len_1755X)))) {
        arg0K0 = foo_1753X;
        goto L33798;}
      else {
        goto L20931;}}
    else {
      goto L20931;}}}
 L38321: {
  v_1756X = ps_close_fd(index_1634X);
  arg0K0 = v_1756X;
  goto L38316;}
 L38316: {
  status_1757X = arg0K0;
  if ((status_1757X == NO_ERRORS)) {
    arg0K0 = reason_1636X;
    goto L38160;}
  else {channel_close_error(status_1757X, index_1634X, arg2_802X);
    arg0K0 = reason_1636X;
    goto L38160;}}
 L38576: {
  if ((3 == (3 & arg4_817X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_817X))) + -4)))))) {
      if ((3 == (3 & arg4_817X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_817X))) + -4))))>>2))))) {
          arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_817X))) + -4))))>>8)));
          goto L38583;}
        else {
          goto L38849;}}
      else {
        goto L38849;}}
    else {
      goto L38644;}}
  else {
    goto L38644;}}
 L38823: {
  if ((3 == (3 & arg4_817X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg4_817X))) + -4))))>>2))))) {
      goto L38576;}
    else {
      goto L38644;}}
  else {
    goto L38644;}}
 L38644: {
  code_1758X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1758X)))))<<2)), code_1758X, 20, 4);
  n_1759X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1759X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_818X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_817X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1646X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1647X)<<2));
  if (waitP_1645X) {
    arg0K0 = 5;
    goto L38660;}
  else {
    arg0K0 = 1;
    goto L38660;}}
 L39038: {
  val_1760X = arg0K0;
  SvalS = val_1760X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L39353: {
  x_1761X = Scurrent_threadS;
  addr_1762X = (((char *) (-3 + x_1761X))) + 12;S48_WRITE_BARRIER(x_1761X, addr_1762X, proposal_843X);
  *((long *) addr_1762X) = proposal_843X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L39719: {
  copies_1763X = arg0K0;
  if ((1 == copies_1763X)) {
    log_1764X = *((long *) ((((char *) (-3 + proposal_844X))) + 4));
    arg0K0 = 0;
    goto L9048;}
  else {
    thing_1765X = *((long *) ((((char *) (-3 + copies_1763X))) + 8));
    if ((3 == (3 & thing_1765X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + thing_1765X))) + -4)))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + copies_1763X))) + 20)));
        goto L39719;}
      else {
        goto L39603;}}
    else {
      goto L39603;}}}
 L10423: {
  arg0K0 = (4 + i_1661X);
  goto L10371;}
 L9662: {
  entry_1766X = arg0K0;
  proposal_1767X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  value_1768X = Stemp0S;
  Stemp0S = 1;
  addr_1769X = ((char *) (-3 + entry_1766X));S48_WRITE_BARRIER(entry_1766X, addr_1769X, value_1768X);
  *((long *) addr_1769X) = value_1768X;
  addr_1770X = (((char *) (-3 + entry_1766X))) + 4;S48_WRITE_BARRIER(entry_1766X, addr_1770X, from_index_1374X);
  *((long *) addr_1770X) = from_index_1374X;
  value_1771X = Stemp1S;
  Stemp1S = 1;
  addr_1772X = (((char *) (-3 + entry_1766X))) + 8;S48_WRITE_BARRIER(entry_1766X, addr_1772X, value_1771X);
  *((long *) addr_1772X) = value_1771X;
  addr_1773X = (((char *) (-3 + entry_1766X))) + 12;S48_WRITE_BARRIER(entry_1766X, addr_1773X, to_index_1375X);
  *((long *) addr_1773X) = to_index_1375X;
  addr_1774X = (((char *) (-3 + entry_1766X))) + 16;S48_WRITE_BARRIER(entry_1766X, addr_1774X, count_1376X);
  *((long *) addr_1774X) = count_1376X;
  value_1775X = *((long *) ((((char *) (-3 + proposal_1767X))) + 12));
  addr_1776X = (((char *) (-3 + entry_1766X))) + 20;S48_WRITE_BARRIER(entry_1766X, addr_1776X, value_1775X);
  *((long *) addr_1776X) = value_1775X;
  addr_1777X = (((char *) (-3 + proposal_1767X))) + 12;S48_WRITE_BARRIER(proposal_1767X, addr_1777X, entry_1766X);
  *((long *) addr_1777X) = entry_1766X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L47845: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L20418: {
  i_1778X = arg0K0;
  h_1779X = arg0K1;
  if ((i_1778X < n_1676X)) {
    arg0K0 = (1 + i_1778X);
    arg0K1 = (h_1779X + (((*((unsigned char *) ((((char *) (-3 + arg2_951X))) + i_1778X))))));
    goto L20418;}
  else {
    index_1780X = 1023 & h_1779X;
    link_1781X = *((long *) ((((char *) (-3 + table_1675X))) + (((index_1780X)<<2))));
    if ((0 == (3 & link_1781X))) {
      arg0K0 = (3 + (-4 & link_1781X));
      goto L20373;}
    else {
      arg0K0 = link_1781X;
      goto L20373;}}}
 L16279: {
  v_1782X = arg3K0;
  ps_write_string(v_1782X, out_1020X);
  goto L42774;}
 L16229: {
  if ((3 == (3 & thing_1510X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_1510X))))))))));
      goto L16279;}
    else {
      goto L16237;}}
  else {
    goto L16237;}}
 L23064: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Sstack_limitS = (Sreal_stack_limitS);
    if ((s48_Spending_eventsPS)) {
      s48_Sstack_limitS = (((char *) -1));
      goto L23066;}
    else {
      goto L23066;}}
  else {
    s48_Sstack_limitS = (((char *) -1));
    goto L23066;}}
 L23103: {
  ps_error("interrupt handler is not a closure", 1, i_1053X);
  goto L23064;}
 L6837: {
  count_1783X = arg0K0;
  i_1784X = arg0K1;
  offset_1785X = arg0K2;
  if ((0 == count_1783X)) {
    arg0K0 = i_1784X;
    arg0K1 = offset_1785X;
    goto L6820;}
  else {
    value_1786X = *((long *) ((((char *) (-3 + env_1692X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1785X)))))<<2))));
    addr_1787X = (((char *) (-3 + new_env_1066X))) + (((i_1784X)<<2));S48_WRITE_BARRIER(new_env_1066X, addr_1787X, value_1786X);
    *((long *) addr_1787X) = value_1786X;
    arg0K0 = (-1 + count_1783X);
    arg0K1 = (1 + i_1784X);
    arg0K2 = (1 + offset_1785X);
    goto L6837;}}
 L7550: {
  count_1788X = arg0K0;
  i_1789X = arg0K1;
  offset_1790X = arg0K2;
  if ((0 == count_1788X)) {
    arg0K0 = i_1789X;
    arg0K1 = offset_1790X;
    goto L7533;}
  else {
    value_1791X = *((long *) ((((char *) (-3 + env_1697X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1790X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1790X))))))<<2))));
    addr_1792X = (((char *) (-3 + new_env_1075X))) + (((i_1789X)<<2));S48_WRITE_BARRIER(new_env_1075X, addr_1792X, value_1791X);
    *((long *) addr_1792X) = value_1791X;
    arg0K0 = (-1 + count_1788X);
    arg0K1 = (1 + i_1789X);
    arg0K2 = (2 + offset_1790X);
    goto L7550;}}
 L41772: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L41963: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L42154: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L18174: {
  length1_1793X = arg0K0;
  extra1_1794X = arg0K1;
  if ((length0_1704X < length1_1793X)) {
    arg0K0 = length1_1793X;
    goto L18196;}
  else {
    arg0K0 = length0_1704X;
    goto L18196;}}
 L18086: {
  length0_1795X = arg0K0;
  extra0_1796X = arg0K1;
  if ((0 == (3 & y_1707X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18094;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_1707X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18094;}}
 L51706: {
  v_1797X = (char *) s48_long_to_bignum(c_1553X);
  v_1798X = enter_bignum(v_1797X);
  arg0K0 = v_1798X;
  goto L51701;}
 L51701: {
  val_1799X = arg0K0;
  SvalS = val_1799X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L8421: {
  x_1800X = 0 - c_1553X;s48_make_availableAgc(16);
  if ((536870911 < x_1800X)) {
    goto L51684;}
  else {
    if ((x_1800X < -536870912)) {
      goto L51684;}
    else {
      arg0K0 = (((x_1800X)<<2));
      goto L51679;}}}
 L51728: {
  v_1801X = (char *) s48_long_to_bignum(c_1553X);
  v_1802X = enter_bignum(v_1801X);
  arg0K0 = v_1802X;
  goto L51723;}
 L51723: {
  val_1803X = arg0K0;
  SvalS = val_1803X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L18131: {
  length1_1804X = arg0K0;
  extra1_1805X = arg0K1;
  if ((length0_1708X < length1_1804X)) {
    arg0K0 = length1_1804X;
    goto L18153;}
  else {
    arg0K0 = length0_1708X;
    goto L18153;}}
 L51775: {
  v_1806X = (char *) s48_long_to_bignum(x_1713X);
  v_1807X = enter_bignum(v_1806X);
  arg0K0 = v_1807X;
  goto L51770;}
 L51770: {
  val_1808X = arg0K0;
  SvalS = val_1808X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L17699: {
  extra1_1809X = arg0K0;
  Stemp0S = arg2_653X;
  Stemp1S = y_654X;s48_make_availableAgc(((((((8 + (((length0_1717X)<<1))) + extra0_1718X) + extra1_1809X))<<2)));
  value_1810X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1810X))) {
    v_1811X = (char *) s48_long_to_bignum((((value_1810X)>>2)));
    arg1K0 = v_1811X;
    goto L19163;}
  else {
    arg1K0 = (((char *) (-3 + value_1810X)));
    goto L19163;}}
 L19836: {
  y_1812X = arg1K0;
  value_1813X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1813X))) {
    v_1814X = (char *) s48_long_to_bignum((((value_1813X)>>2)));
    arg1K0 = v_1814X;
    goto L19832;}
  else {
    arg1K0 = (((char *) (-3 + value_1813X)));
    goto L19832;}}
 L19745: {
  y_1815X = arg1K0;
  value_1816X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1816X))) {
    v_1817X = (char *) s48_long_to_bignum((((value_1816X)>>2)));
    arg1K0 = v_1817X;
    goto L19741;}
  else {
    arg1K0 = (((char *) (-3 + value_1816X)));
    goto L19741;}}
 L19329: {
  external_bignum_1818X = arg1K0;
  v_1819X = s48_bignum_fits_in_word_p(external_bignum_1818X, 30, 1);
  if (v_1819X) {
    n_1820X = s48_bignum_to_long(external_bignum_1818X);
    return (((n_1820X)<<2));}
  else {
    return enter_bignum(external_bignum_1818X);}}
 L19674: {
  v_1821X = arg1K0;
  external_bignum_1822X = (char *) s48_bignum_bitwise_not(v_1821X);
  v_1823X = s48_bignum_fits_in_word_p(external_bignum_1822X, 30, 1);
  if (v_1823X) {
    n_1824X = s48_bignum_to_long(external_bignum_1822X);
    arg0K0 = (((n_1824X)<<2));
    goto L36902;}
  else {
    val_1825X = enter_bignum(external_bignum_1822X);
    arg0K0 = val_1825X;
    goto L36902;}}
 L19616: {
  v_1826X = arg1K0;
  n_1827X = s48_bignum_bit_count(v_1826X);
  SvalS = (((n_1827X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L17913: {
  length1_1828X = arg0K0;
  extra1_1829X = arg0K1;
  if ((length0_1741X < length1_1828X)) {
    arg0K0 = length1_1828X;
    goto L17935;}
  else {
    arg0K0 = length0_1741X;
    goto L17935;}}
 L17870: {
  length1_1830X = arg0K0;
  extra1_1831X = arg0K1;
  if ((length0_1743X < length1_1830X)) {
    arg0K0 = length1_1830X;
    goto L17892;}
  else {
    arg0K0 = length0_1743X;
    goto L17892;}}
 L17827: {
  length1_1832X = arg0K0;
  extra1_1833X = arg0K1;
  if ((length0_1745X < length1_1832X)) {
    arg0K0 = length1_1832X;
    goto L17849;}
  else {
    arg0K0 = length0_1745X;
    goto L17849;}}
 L44562: {
  val_1834X = arg0K0;
  SvalS = val_1834X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L20916: {
  b_1835X = arg0K0;
  addr_1836X = s48_allocate_small(12);
  *((long *) addr_1836X) = 2054;
  x_1837X = 3 + (((long) (addr_1836X + 4)));
  *((long *) (((char *) (-3 + x_1837X)))) = string_793X;
  *((long *) ((((char *) (-3 + x_1837X))) + 4)) = b_1835X;
  if ((3 == (3 & x_1837X))) {
    arg0K0 = (-4 & x_1837X);
    goto L20922;}
  else {
    arg0K0 = x_1837X;
    goto L20922;}}
 L33798: {
  val_1838X = arg0K0;
  SvalS = val_1838X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L20931: {
  link_1839X = *((long *) ((((char *) (-3 + foo_1753X))) + 4));
  if ((0 == (3 & link_1839X))) {
    arg0K0 = (3 + (-4 & link_1839X));
    goto L20911;}
  else {
    arg0K0 = link_1839X;
    goto L20911;}}
 L38583: {
  y_1840X = arg0K0;
  if ((y_1840X < (start_1646X + count_1647X))) {
    goto L38644;}
  else {
    got_1841X = ps_read_fd(((((*((long *) ((((char *) (-3 + arg5_818X))) + 8))))>>2)), ((((char *) (-3 + arg4_817X))) + start_1646X), count_1647X, waitP_1645X, &eofP_1842X, &pendingP_1843X, &status_1844X);
    if ((status_1844X == NO_ERRORS)) {
      if (eofP_1842X) {
        arg0K0 = 21;
        goto L38643;}
      else {
        if (pendingP_1843X) {
          arg0K0 = 1;
          goto L38643;}
        else {
          arg0K0 = (((got_1841X)<<2));
          goto L38643;}}}
    else {
      code_1845X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1845X)))))<<2)), code_1845X, 96, 4);
      n_1846X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1846X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 96;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg5_818X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_817X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1646X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1647X)<<2));
      if (waitP_1645X) {
        arg0K0 = 5;
        goto L38626;}
      else {
        arg0K0 = 1;
        goto L38626;}}}}
 L38849: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_817X))) + -4))))>>8));
  goto L38583;}
 L38660: {
  x_1847X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1847X;
  arg0K0 = 5;
  goto L22779;}
 L9048: {
  i_1848X = arg0K0;
  stob_1849X = *((long *) ((((char *) (-3 + log_1764X))) + (((i_1848X)<<2))));
  if ((1 == stob_1849X)) {
    log_1850X = *((long *) ((((char *) (-3 + proposal_844X))) + 8));
    arg0K0 = 0;
    goto L12250;}
  else {
    verify_1851X = *((long *) ((((char *) (-3 + log_1764X))) + (12 + (((i_1848X)<<2)))));
    value_1852X = *((long *) ((((char *) (-3 + log_1764X))) + (8 + (((i_1848X)<<2)))));
    if ((verify_1851X == value_1852X)) {
      goto L9080;}
    else {
      addr_1853X = (((char *) (-3 + stob_1849X))) + (-4 & (*((long *) ((((char *) (-3 + log_1764X))) + (4 + (((i_1848X)<<2)))))));S48_WRITE_BARRIER(stob_1849X, addr_1853X, value_1852X);
      *((long *) addr_1853X) = value_1852X;
      goto L9080;}}}
 L20373: {
  bucket_1854X = arg0K0;
  arg0K0 = 1;
  arg0K1 = bucket_1854X;
  goto L20380;}
 L16237: {
  if ((1 == thing_1510X)) {
    goto L16240;}
  else {
    if ((5 == thing_1510X)) {
      goto L16240;}
    else {
      if ((25 == thing_1510X)) {
        arg3K0 = "()";
        goto L16279;}
      else {
        if ((3 == (3 & thing_1510X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L16279;}
          else {
            goto L16253;}}
        else {
          goto L16253;}}}}}
 L23066: {
  Scode_pointerS = (((char *) 0));
  Slosing_opcodeS = (0 - i_1053X);
  arg0K0 = arg_count_1523X;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L26593;}
 L18196: {
  v_1855X = arg0K0;
  Stemp0S = arg2_643X;
  Stemp1S = y_644X;s48_make_availableAgc(((((((3 + v_1855X) + extra0_1705X) + extra1_1794X))<<2)));
  value_1856X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1856X))) {
    v_1857X = (char *) s48_long_to_bignum((((value_1856X)>>2)));
    arg1K0 = v_1857X;
    goto L20075;}
  else {
    arg1K0 = (((char *) (-3 + value_1856X)));
    goto L20075;}}
 L18094: {
  length1_1858X = arg0K0;
  extra1_1859X = arg0K1;
  Stemp0S = x_1706X;
  Stemp1S = y_1707X;s48_make_availableAgc(((((((2 + (length0_1795X + length1_1858X)) + extra0_1796X) + extra1_1859X))<<2)));
  value_1860X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1860X))) {
    v_1861X = (char *) s48_long_to_bignum((((value_1860X)>>2)));
    arg1K0 = v_1861X;
    goto L19927;}
  else {
    arg1K0 = (((char *) (-3 + value_1860X)));
    goto L19927;}}
 L51684: {
  v_1862X = (char *) s48_long_to_bignum(x_1800X);
  v_1863X = enter_bignum(v_1862X);
  arg0K0 = v_1863X;
  goto L51679;}
 L51679: {
  val_1864X = arg0K0;
  SvalS = val_1864X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L18153: {
  v_1865X = arg0K0;
  Stemp0S = arg2_650X;
  Stemp1S = y_651X;s48_make_availableAgc(((((((3 + v_1865X) + extra0_1709X) + extra1_1805X))<<2)));
  value_1866X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1866X))) {
    v_1867X = (char *) s48_long_to_bignum((((value_1866X)>>2)));
    arg1K0 = v_1867X;
    goto L20001;}
  else {
    arg1K0 = (((char *) (-3 + value_1866X)));
    goto L20001;}}
 L19163: {
  y_1868X = arg1K0;
  value_1869X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1869X))) {
    v_1870X = (char *) s48_long_to_bignum((((value_1869X)>>2)));
    arg1K0 = v_1870X;
    goto L19159;}
  else {
    arg1K0 = (((char *) (-3 + value_1869X)));
    goto L19159;}}
 L19832: {
  x_1871X = arg1K0;
  external_bignum_1872X = (char *)s48_bignum_quotient(x_1871X, y_1812X);
  v_1873X = s48_bignum_fits_in_word_p(external_bignum_1872X, 30, 1);
  if (v_1873X) {
    n_1874X = s48_bignum_to_long(external_bignum_1872X);
    arg0K0 = (((n_1874X)<<2));
    goto L52526;}
  else {
    val_1875X = enter_bignum(external_bignum_1872X);
    arg0K0 = val_1875X;
    goto L52526;}}
 L19741: {
  x_1876X = arg1K0;
  external_bignum_1877X = (char *)s48_bignum_remainder(x_1876X, y_1815X);
  v_1878X = s48_bignum_fits_in_word_p(external_bignum_1877X, 30, 1);
  if (v_1878X) {
    n_1879X = s48_bignum_to_long(external_bignum_1877X);
    arg0K0 = (((n_1879X)<<2));
    goto L52526;}
  else {
    val_1880X = enter_bignum(external_bignum_1877X);
    arg0K0 = val_1880X;
    goto L52526;}}
 L36902: {
  val_1881X = arg0K0;
  SvalS = val_1881X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L17935: {
  v_1882X = arg0K0;
  Stemp0S = arg2_688X;
  Stemp1S = y_689X;s48_make_availableAgc(((((((3 + v_1882X) + extra0_1742X) + extra1_1829X))<<2)));
  value_1883X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1883X))) {
    v_1884X = (char *) s48_long_to_bignum((((value_1883X)>>2)));
    arg1K0 = v_1884X;
    goto L19549;}
  else {
    arg1K0 = (((char *) (-3 + value_1883X)));
    goto L19549;}}
 L17892: {
  v_1885X = arg0K0;
  Stemp0S = arg2_690X;
  Stemp1S = y_691X;s48_make_availableAgc(((((((3 + v_1885X) + extra0_1744X) + extra1_1831X))<<2)));
  value_1886X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1886X))) {
    v_1887X = (char *) s48_long_to_bignum((((value_1886X)>>2)));
    arg1K0 = v_1887X;
    goto L19475;}
  else {
    arg1K0 = (((char *) (-3 + value_1886X)));
    goto L19475;}}
 L17849: {
  v_1888X = arg0K0;
  Stemp0S = arg2_692X;
  Stemp1S = y_693X;s48_make_availableAgc(((((((3 + v_1888X) + extra0_1746X) + extra1_1833X))<<2)));
  value_1889X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1889X))) {
    v_1890X = (char *) s48_long_to_bignum((((value_1889X)>>2)));
    arg1K0 = v_1890X;
    goto L19401;}
  else {
    arg1K0 = (((char *) (-3 + value_1889X)));
    goto L19401;}}
 L20922: {
  value_1891X = arg0K0;
  addr_1892X = (((char *) (-3 + table_792X))) + (((index_1309X)<<2));S48_WRITE_BARRIER(table_792X, addr_1892X, value_1891X);
  *((long *) addr_1892X) = value_1891X;
  arg0K0 = x_1837X;
  goto L33798;}
 L38643: {
  val_1893X = arg0K0;
  SvalS = val_1893X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L38626: {
  x_1894X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1894X;
  merged_arg0K0 = status_1844X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 5;
  goto get_error_string;
 get_error_string_return_5:
  x_1895X = get_error_string0_return_value;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1895X;
  arg0K0 = 6;
  goto L22779;}
 L12250: {
  i_1896X = arg0K0;
  stob_1897X = *((long *) ((((char *) (-3 + log_1850X))) + (((i_1896X)<<2))));
  if ((1 == stob_1897X)) {
    copies_1898X = *((long *) ((((char *) (-3 + proposal_844X))) + 12));
    arg0K0 = copies_1898X;
    goto L12165;}
  else {
    verify_1899X = *((long *) ((((char *) (-3 + log_1850X))) + (12 + (((i_1896X)<<2)))));
    value_1900X = *((long *) ((((char *) (-3 + log_1850X))) + (8 + (((i_1896X)<<2)))));
    if ((verify_1899X == value_1900X)) {
      goto L12282;}
    else {
      *((unsigned char *) ((((char *) (-3 + stob_1897X))) + ((((*((long *) ((((char *) (-3 + log_1850X))) + (4 + (((i_1896X)<<2)))))))>>2)))) = (((value_1900X)>>2));
      goto L12282;}}}
 L9080: {
  arg0K0 = (4 + i_1848X);
  goto L9048;}
 L20380: {
  previous_foo_1901X = arg0K0;
  foo_1902X = arg0K1;
  if ((1 == foo_1902X)) {
    goto L32188;}
  else {
    s2_1903X = *((long *) (((char *) (-3 + foo_1902X))));
    len_1904X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_951X))) + -4))))>>8);
    if ((len_1904X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1903X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1903X))), (void *)(((char *) (-3 + arg2_951X))),len_1904X)))) {
        if ((1 == previous_foo_1901X)) {
          value_1905X = *((long *) ((((char *) (-3 + foo_1902X))) + 12));
          addr_1906X = (((char *) (-3 + table_1675X))) + (((index_1780X)<<2));S48_WRITE_BARRIER(table_1675X, addr_1906X, value_1905X);
          *((long *) addr_1906X) = value_1905X;
          goto L32188;}
        else {
          val_1907X = *((long *) ((((char *) (-3 + foo_1902X))) + 12));
          addr_1908X = (((char *) (-3 + previous_foo_1901X))) + 12;S48_WRITE_BARRIER(previous_foo_1901X, addr_1908X, val_1907X);
          *((long *) addr_1908X) = val_1907X;
          goto L32188;}}
      else {
        goto L20448;}}
    else {
      goto L20448;}}}
 L16240: {
  if ((1 == thing_1510X)) {
    arg3K0 = "#f";
    goto L16279;}
  else {
    arg3K0 = "#t";
    goto L16279;}}
 L16253: {
  if ((3 == (3 & thing_1510X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L16279;}
    else {
      goto L16257;}}
  else {
    goto L16257;}}
 L20075: {
  y_1909X = arg1K0;
  value_1910X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1910X))) {
    v_1911X = (char *) s48_long_to_bignum((((value_1910X)>>2)));
    arg1K0 = v_1911X;
    goto L20071;}
  else {
    arg1K0 = (((char *) (-3 + value_1910X)));
    goto L20071;}}
 L19927: {
  y_1912X = arg1K0;
  value_1913X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1913X))) {
    v_1914X = (char *) s48_long_to_bignum((((value_1913X)>>2)));
    arg1K0 = v_1914X;
    goto L19923;}
  else {
    arg1K0 = (((char *) (-3 + value_1913X)));
    goto L19923;}}
 L20001: {
  y_1915X = arg1K0;
  value_1916X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1916X))) {
    v_1917X = (char *) s48_long_to_bignum((((value_1916X)>>2)));
    arg1K0 = v_1917X;
    goto L19997;}
  else {
    arg1K0 = (((char *) (-3 + value_1916X)));
    goto L19997;}}
 L19159: {
  x_1918X = arg1K0;
  div_by_zeroP_1919X = s48_bignum_divide(x_1918X, y_1868X, &quot_1920X, &rem_1921X);
  if (div_by_zeroP_1919X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = 0;
    goto L44276;}
  else {
    v_1922X = s48_bignum_fits_in_word_p(rem_1921X, 30, 1);
    if (v_1922X) {
      n_1923X = s48_bignum_to_long(rem_1921X);
      arg0K0 = (((n_1923X)<<2));
      goto L19186;}
    else {
      v_1924X = enter_bignum(rem_1921X);
      arg0K0 = v_1924X;
      goto L19186;}}}
 L52526: {
  val_1925X = arg0K0;
  SvalS = val_1925X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L19549: {
  y_1926X = arg1K0;
  value_1927X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1927X))) {
    v_1928X = (char *) s48_long_to_bignum((((value_1927X)>>2)));
    arg1K0 = v_1928X;
    goto L19545;}
  else {
    arg1K0 = (((char *) (-3 + value_1927X)));
    goto L19545;}}
 L19475: {
  y_1929X = arg1K0;
  value_1930X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1930X))) {
    v_1931X = (char *) s48_long_to_bignum((((value_1930X)>>2)));
    arg1K0 = v_1931X;
    goto L19471;}
  else {
    arg1K0 = (((char *) (-3 + value_1930X)));
    goto L19471;}}
 L19401: {
  y_1932X = arg1K0;
  value_1933X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1933X))) {
    v_1934X = (char *) s48_long_to_bignum((((value_1933X)>>2)));
    arg1K0 = v_1934X;
    goto L19397;}
  else {
    arg1K0 = (((char *) (-3 + value_1933X)));
    goto L19397;}}
 L12165: {
  copies_1935X = arg0K0;
  if ((1 == copies_1935X)) {
    value_1936X = Sempty_logS;
    addr_1937X = (((char *) (-3 + proposal_844X))) + 4;S48_WRITE_BARRIER(proposal_844X, addr_1937X, value_1936X);
    *((long *) addr_1937X) = value_1936X;
    value_1938X = Sempty_logS;
    addr_1939X = (((char *) (-3 + proposal_844X))) + 8;S48_WRITE_BARRIER(proposal_844X, addr_1939X, value_1938X);
    *((long *) addr_1939X) = value_1938X;
    addr_1940X = (((char *) (-3 + proposal_844X))) + 12;S48_WRITE_BARRIER(proposal_844X, addr_1940X, 1);
    *((long *) addr_1940X) = 1;RELEASE_PROPOSAL_LOCK();
    x_1941X = Scurrent_threadS;
    addr_1942X = (((char *) (-3 + x_1941X))) + 12;S48_WRITE_BARRIER(x_1941X, addr_1942X, 1);
    *((long *) addr_1942X) = 1;
    SvalS = 5;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L24972;}
  else {
    stob_1943X = *((long *) (((char *) (-3 + copies_1935X))));
    v_1944X = (((*((long *) ((((char *) (-3 + copies_1935X))) + 4))))>>2);
    stob_1945X = *((long *) ((((char *) (-3 + copies_1935X))) + 8));
    v_1946X = (((*((long *) ((((char *) (-3 + copies_1935X))) + 12))))>>2);
    v_1947X = (((*((long *) ((((char *) (-3 + copies_1935X))) + 16))))>>2);
    memcpy((void *)((((char *) (-3 + stob_1945X))) + v_1946X), (void *)((((char *) (-3 + stob_1943X))) + v_1944X),v_1947X);
    arg0K0 = (*((long *) ((((char *) (-3 + copies_1935X))) + 20)));
    goto L12165;}}
 L12282: {
  arg0K0 = (4 + i_1896X);
  goto L12250;}
 L32188: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L20448: {
  link_1948X = *((long *) ((((char *) (-3 + foo_1902X))) + 12));
  if ((0 == (3 & link_1948X))) {
    arg0K0 = foo_1902X;
    arg0K1 = (3 + (-4 & link_1948X));
    goto L20380;}
  else {
    arg0K0 = foo_1902X;
    arg0K1 = link_1948X;
    goto L20380;}}
 L16257: {
  if ((3 == (3 & thing_1510X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L16279;}
    else {
      goto L16261;}}
  else {
    goto L16261;}}
 L20071: {
  x_1949X = arg1K0;
  external_bignum_1950X = (char *)s48_bignum_add(x_1949X, y_1909X);
  v_1951X = s48_bignum_fits_in_word_p(external_bignum_1950X, 30, 1);
  if (v_1951X) {
    n_1952X = s48_bignum_to_long(external_bignum_1950X);
    arg0K0 = (((n_1952X)<<2));
    goto L33951;}
  else {
    val_1953X = enter_bignum(external_bignum_1950X);
    arg0K0 = val_1953X;
    goto L33951;}}
 L19923: {
  x_1954X = arg1K0;
  external_bignum_1955X = (char *)s48_bignum_multiply(x_1954X, y_1912X);
  v_1956X = s48_bignum_fits_in_word_p(external_bignum_1955X, 30, 1);
  if (v_1956X) {
    n_1957X = s48_bignum_to_long(external_bignum_1955X);
    arg0K0 = (((n_1957X)<<2));
    goto L52526;}
  else {
    val_1958X = enter_bignum(external_bignum_1955X);
    arg0K0 = val_1958X;
    goto L52526;}}
 L19997: {
  x_1959X = arg1K0;
  external_bignum_1960X = (char *)s48_bignum_subtract(x_1959X, y_1915X);
  v_1961X = s48_bignum_fits_in_word_p(external_bignum_1960X, 30, 1);
  if (v_1961X) {
    n_1962X = s48_bignum_to_long(external_bignum_1960X);
    arg0K0 = (((n_1962X)<<2));
    goto L34256;}
  else {
    val_1963X = enter_bignum(external_bignum_1960X);
    arg0K0 = val_1963X;
    goto L34256;}}
 L44276: {
  div_by_zeroP_1964X = arg2K0;
  quot_1965X = arg0K1;
  rem_1966X = arg0K2;
  if (div_by_zeroP_1964X) {
    arg0K0 = arg2_653X;
    arg0K1 = y_654X;
    goto L23692;}
  else {
    if ((0 == (3 & rem_1966X))) {
      if ((0 == rem_1966X)) {
        SvalS = quot_1965X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L24972;}
      else {
        arg0K0 = arg2_653X;
        arg0K1 = y_654X;
        goto L23692;}}
    else {
      arg0K0 = arg2_653X;
      arg0K1 = y_654X;
      goto L23692;}}}
 L19186: {
  v_1967X = arg0K0;
  v_1968X = s48_bignum_fits_in_word_p(quot_1920X, 30, 1);
  if (v_1968X) {
    n_1969X = s48_bignum_to_long(quot_1920X);
    arg2K0 = 0;
    arg0K1 = (((n_1969X)<<2));
    arg0K2 = v_1967X;
    goto L44276;}
  else {
    v_1970X = enter_bignum(quot_1920X);
    arg2K0 = 0;
    arg0K1 = v_1970X;
    arg0K2 = v_1967X;
    goto L44276;}}
 L19545: {
  x_1971X = arg1K0;
  external_bignum_1972X = (char *) s48_bignum_bitwise_and(x_1971X, y_1926X);
  v_1973X = s48_bignum_fits_in_word_p(external_bignum_1972X, 30, 1);
  if (v_1973X) {
    n_1974X = s48_bignum_to_long(external_bignum_1972X);
    arg0K0 = (((n_1974X)<<2));
    goto L37116;}
  else {
    val_1975X = enter_bignum(external_bignum_1972X);
    arg0K0 = val_1975X;
    goto L37116;}}
 L19471: {
  x_1976X = arg1K0;
  external_bignum_1977X = (char *) s48_bignum_bitwise_ior(x_1976X, y_1929X);
  v_1978X = s48_bignum_fits_in_word_p(external_bignum_1977X, 30, 1);
  if (v_1978X) {
    n_1979X = s48_bignum_to_long(external_bignum_1977X);
    arg0K0 = (((n_1979X)<<2));
    goto L37295;}
  else {
    val_1980X = enter_bignum(external_bignum_1977X);
    arg0K0 = val_1980X;
    goto L37295;}}
 L19397: {
  x_1981X = arg1K0;
  external_bignum_1982X = (char *) s48_bignum_bitwise_xor(x_1981X, y_1932X);
  v_1983X = s48_bignum_fits_in_word_p(external_bignum_1982X, 30, 1);
  if (v_1983X) {
    n_1984X = s48_bignum_to_long(external_bignum_1982X);
    arg0K0 = (((n_1984X)<<2));
    goto L37474;}
  else {
    val_1985X = enter_bignum(external_bignum_1982X);
    arg0K0 = val_1985X;
    goto L37474;}}
 L16261: {
  if ((3 == (3 & thing_1510X))) {
    if ((12 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
      arg3K0 = "#{template}";
      goto L16279;}
    else {
      goto L16265;}}
  else {
    goto L16265;}}
 L33951: {
  val_1986X = arg0K0;
  SvalS = val_1986X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L34256: {
  val_1987X = arg0K0;
  SvalS = val_1987X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L37116: {
  val_1988X = arg0K0;
  SvalS = val_1988X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L37295: {
  val_1989X = arg0K0;
  SvalS = val_1989X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L37474: {
  val_1990X = arg0K0;
  SvalS = val_1990X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L24972;}
 L16265: {
  if ((3 == (3 & thing_1510X))) {
    if ((4 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
      arg3K0 = "#{location}";
      goto L16279;}
    else {
      goto L16269;}}
  else {
    goto L16269;}}
 L16269: {
  if ((3 == (3 & thing_1510X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
      arg3K0 = "#{code-vector}";
      goto L16279;}
    else {
      goto L16273;}}
  else {
    goto L16273;}}
 L16273: {
  if ((3 == (3 & thing_1510X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + thing_1510X))) + -4))))>>2))))) {
      arg3K0 = "#{continuation}";
      goto L16279;}
    else {
      arg3K0 = "???";
      goto L16279;}}
  else {
    arg3K0 = "???";
    goto L16279;}}
 loseD0: {
  message_444X = merged_arg3K0;{
  ps_write_string("Template UIDs: ", (stderr));
  current_code_1991X = current_code_vector();
  out_1992X = stderr;
  merged_arg1K0 = (SstackS);
  merged_arg0K1 = (((((ScontS) - (SstackS)))>>2));
  merged_arg0K2 = current_code_1991X;
  find_template_return_tag = 1;
  goto find_template;
 find_template_return_1:
  template_1993X = find_template0_return_value;
  merged_arg0K0 = template_1993X;
  merged_arg2K1 = 0;
  merged_arg4K2 = out_1992X;
  maybe_write_template_return_tag = 1;
  goto maybe_write_template;
 maybe_write_template_return_1:
  not_firstP_1994X = maybe_write_template0_return_value;
  merged_arg2K0 = not_firstP_1994X;
  merged_arg4K1 = out_1992X;
  really_show_stack_continuation_uids_return_tag = 1;
  goto really_show_stack_continuation_uids;
 really_show_stack_continuation_uids_return_1:
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  why_1995X = (((*((long *) ((SstackS) + (((nargs_480X)<<2))))))>>2);
  if ((why_1995X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (-4 + (((nargs_480X)<<2)))))))))))))) {
      ps_error(message_444X, 3, opcode_481X, why_1995X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (-4 + (((nargs_480X)<<2))))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L22824;}}
  else {
    goto L22824;}}
 L22824: {
  ps_error(message_444X, 2, opcode_481X, why_1995X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 really_show_stack_continuation_uids: {
  not_firstP_442X = merged_arg2K0;
  out_443X = merged_arg4K1;{
  arg1K0 = (ScontS);
  arg2K1 = not_firstP_442X;
  goto L20276;}
 L20276: {
  cont_1996X = arg1K0;
  not_firstP_1997X = arg2K1;
  if ((cont_1996X == (Sbottom_of_stackS))) {
    cont_1998X = Sheap_continuationS;
    arg0K0 = cont_1998X;
    arg2K1 = not_firstP_1997X;
    goto L18448;}
  else {
    code_pointer_1999X = ((char *) (*((long *) cont_1996X)));
    pointer_2000X = code_pointer_1999X + -5;
    v_2001X = 3 + (((long) (code_pointer_1999X + (0 - (((((*((unsigned char *) pointer_2000X)))<<8)) + (*((unsigned char *) (pointer_2000X + 1))))))));
    pointer_2002X = (((char *) (*((long *) cont_1996X)))) + -2;
    size_2003X = ((((*((unsigned char *) pointer_2002X)))<<8)) + (*((unsigned char *) (pointer_2002X + 1)));
    if ((65535 == size_2003X)) {
      arg0K0 = ((((*((long *) (cont_1996X + 4))))>>2));
      goto L15630;}
    else {
      arg0K0 = size_2003X;
      goto L15630;}}}
 L18448: {
  cont_2004X = arg0K0;
  not_firstP_2005X = arg2K1;
  if ((3 == (3 & cont_2004X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_2004X))) + -4))))>>2))))) {
      merged_arg1K0 = (((char *) (-3 + cont_2004X)));
      merged_arg0K1 = ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_2004X))) + -4))))>>8))))>>2));
      merged_arg0K2 = (*((long *) ((((char *) (-3 + cont_2004X))) + 4)));
      find_template_return_tag = 2;
      goto find_template;
     find_template_return_2:
      v_2006X = find_template0_return_value;
      merged_arg0K0 = v_2006X;
      merged_arg2K1 = not_firstP_2005X;
      merged_arg4K2 = out_443X;
      maybe_write_template_return_tag = 2;
      goto maybe_write_template;
     maybe_write_template_return_2:
      v_2007X = maybe_write_template0_return_value;
      arg0K0 = (*((long *) ((((char *) (-3 + cont_2004X))) + 8)));
      arg2K1 = v_2007X;
      goto L18448;}
    else {
      really_show_stack_continuation_uids0_return_value = 0;
      goto really_show_stack_continuation_uids_return;}}
  else {
    really_show_stack_continuation_uids0_return_value = 0;
    goto really_show_stack_continuation_uids_return;}}
 L15630: {
  v_2008X = arg0K0;
  merged_arg1K0 = (cont_1996X + 4);
  merged_arg0K1 = v_2008X;
  merged_arg0K2 = v_2001X;
  find_template_return_tag = 3;
  goto find_template;
 find_template_return_3:
  v_2009X = find_template0_return_value;
  merged_arg0K0 = v_2009X;
  merged_arg2K1 = not_firstP_1997X;
  merged_arg4K2 = out_443X;
  maybe_write_template_return_tag = 3;
  goto maybe_write_template;
 maybe_write_template_return_3:
  v_2010X = maybe_write_template0_return_value;
  pointer_2011X = (((char *) (*((long *) cont_1996X)))) + -2;
  size_2012X = ((((*((unsigned char *) pointer_2011X)))<<8)) + (*((unsigned char *) (pointer_2011X + 1)));
  if ((65535 == size_2012X)) {
    arg0K0 = ((((*((long *) (cont_1996X + 4))))>>2));
    goto L20294;}
  else {
    arg0K0 = size_2012X;
    goto L20294;}}
 L20294: {
  v_2013X = arg0K0;
  arg1K0 = (cont_1996X + (4 + (((v_2013X)<<2))));
  arg2K1 = v_2010X;
  goto L20276;}
 really_show_stack_continuation_uids_return:
  switch (really_show_stack_continuation_uids_return_tag) {
  case 0: goto really_show_stack_continuation_uids_return_0;
  default: goto really_show_stack_continuation_uids_return_1;
  }}

 maybe_write_template: {
  template_439X = merged_arg0K0;
  not_firstP_440X = merged_arg2K1;
  out_441X = merged_arg4K2;{
  if (not_firstP_440X) {
    ps_write_string(" <- ", out_441X);
    goto L15869;}
  else {
    goto L15869;}}
 L15869: {
  if ((3 == (3 & template_439X))) {
    if ((12 == (31 & ((((*((long *) ((((char *) (-3 + template_439X))) + -4))))>>2))))) {
      name_2014X = *((long *) ((((char *) (-3 + template_439X))) + 4));
      if ((0 == (3 & name_2014X))) {
        ps_write_integer((((name_2014X)>>2)), out_441X);
        maybe_write_template0_return_value = 1;
        goto maybe_write_template_return;}
      else {
        if ((3 == (3 & name_2014X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + name_2014X))) + -4))))>>2))))) {
            obj_2015X = *((long *) ((((char *) (-3 + name_2014X))) + 8));
            if ((3 == (3 & obj_2015X))) {
              if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_2015X))) + -4))))>>2))))) {
                ps_write_string((((char *)(((char *) (-3 + (*((long *) ((((char *) (-3 + name_2014X))) + 8)))))))), out_441X);
                maybe_write_template0_return_value = 1;
                goto maybe_write_template_return;}
              else {
                goto L15904;}}
            else {
              goto L15904;}}
          else {
            goto L15904;}}
        else {
          goto L15904;}}}
    else {
      goto L15927;}}
  else {
    goto L15927;}}
 L15904: {
  if ((3 == (3 & name_2014X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + name_2014X))) + -4))))>>2))))) {
      obj_2016X = *((long *) ((((char *) (-3 + name_2014X))) + 8));
      if ((3 == (3 & obj_2016X))) {
        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_2016X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + name_2014X))) + 8))))))))))))), out_441X);
          maybe_write_template0_return_value = 1;
          goto maybe_write_template_return;}
        else {
          goto L15924;}}
      else {
        goto L15924;}}
    else {
      goto L15924;}}
  else {
    goto L15924;}}
 L15927: {
  ps_write_string(" ?? ", out_441X);
  maybe_write_template0_return_value = 1;
  goto maybe_write_template_return;}
 L15924: {
  ps_write_string("?", out_441X);
  maybe_write_template0_return_value = 1;
  goto maybe_write_template_return;}
 maybe_write_template_return:
  switch (maybe_write_template_return_tag) {
  case 0: goto maybe_write_template_return_0;
  case 1: goto maybe_write_template_return_1;
  case 2: goto maybe_write_template_return_2;
  default: goto maybe_write_template_return_3;
  }}

 find_template: {
  start_436X = merged_arg1K0;
  count_437X = merged_arg0K1;
  code_vector_438X = merged_arg0K2;{
  arg0K0 = 0;
  goto L14011;}
 L14011: {
  i_2017X = arg0K0;
  if ((i_2017X == count_437X)) {
    find_template0_return_value = 1;
    goto find_template_return;}
  else {
    next_2018X = *((long *) (start_436X + (((i_2017X)<<2))));
    if ((3 == (3 & next_2018X))) {
      if ((12 == (31 & ((((*((long *) ((((char *) (-3 + next_2018X))) + -4))))>>2))))) {
        if (((*((long *) (((char *) (-3 + next_2018X))))) == code_vector_438X)) {
          find_template0_return_value = next_2018X;
          goto find_template_return;}
        else {
          goto L14033;}}
      else {
        goto L14033;}}
    else {
      goto L14033;}}}
 L14033: {
  arg0K0 = (1 + i_2017X);
  goto L14011;}
 find_template_return:
  switch (find_template_return_tag) {
  case 0: goto find_template_return_0;
  case 1: goto find_template_return_1;
  case 2: goto find_template_return_2;
  default: goto find_template_return_3;
  }}

 rest_list_setupAgc: {
  wants_stack_args_432X = merged_arg0K0;
  stack_arg_count_433X = merged_arg0K1;
  list_args_434X = merged_arg0K2;
  list_arg_count_435X = merged_arg0K3;{
  if ((stack_arg_count_433X == wants_stack_args_432X)) {
    merged_arg0K0 = list_args_434X;
    merged_arg0K1 = list_arg_count_435X;
    copy_listSAgc_return_tag = 1;
    goto copy_listSAgc;
   copy_listSAgc_return_1:
    x_2019X = copy_listSAgc0_return_value;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2019X;
    goto rest_list_setupAgc_return;}
  else {
    if ((stack_arg_count_433X < wants_stack_args_432X)) {
      count_2020X = wants_stack_args_432X - stack_arg_count_433X;
      merged_arg0K0 = list_args_434X;
      merged_arg0K1 = count_2020X;
      push_list_return_tag = 4;
      goto push_list;
     push_list_return_4:
      v_2021X = push_list0_return_value;
      merged_arg0K0 = v_2021X;
      merged_arg0K1 = (list_arg_count_435X - count_2020X);
      copy_listSAgc_return_tag = 2;
      goto copy_listSAgc;
     copy_listSAgc_return_2:
      x_2022X = copy_listSAgc0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_2022X;
      goto rest_list_setupAgc_return;}
    else {
      merged_arg0K0 = list_args_434X;
      merged_arg0K1 = list_arg_count_435X;
      copy_listSAgc_return_tag = 3;
      goto copy_listSAgc;
     copy_listSAgc_return_3:
      v_2023X = copy_listSAgc0_return_value;
      merged_arg0K0 = v_2023X;
      merged_arg0K1 = (stack_arg_count_433X - wants_stack_args_432X);
      pop_args_GlistSAgc_return_tag = 5;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_5:
      x_2024X = pop_args_GlistSAgc0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_2024X;
      goto rest_list_setupAgc_return;}}}
 rest_list_setupAgc_return:
  switch (rest_list_setupAgc_return_tag) {
  case 0: goto rest_list_setupAgc_return_0;
  default: goto rest_list_setupAgc_return_1;
  }}

 push_list: {
  list_430X = merged_arg0K0;
  count_431X = merged_arg0K1;{
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = list_430X;
  merged_arg0K0 = count_431X;
  ensure_stack_spaceB_return_tag = 1;
  goto ensure_stack_spaceB;
 ensure_stack_spaceB_return_1:
  list_2025X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg0K0 = count_431X;
  arg0K1 = list_2025X;
  goto L21646;}
 L21646: {
  i_2026X = arg0K0;
  l_2027X = arg0K1;
  if ((0 < i_2026X)) {
    x_2028X = *((long *) (((char *) (-3 + l_2027X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2028X;
    arg0K0 = (-1 + i_2026X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_2027X))) + 4)));
    goto L21646;}
  else {
    push_list0_return_value = l_2027X;
    goto push_list_return;}}
 push_list_return:
  switch (push_list_return_tag) {
  case 0: goto push_list_return_0;
  case 1: goto push_list_return_1;
  case 2: goto push_list_return_2;
  case 3: goto push_list_return_3;
  default: goto push_list_return_4;
  }}

 ensure_stack_spaceB: {
  need_429X = merged_arg0K0;{
  if ((need_429X < (64 + (((((SstackS) - (s48_Sstack_limitS)))>>2))))) {
    ensure_stack_spaceB0_return_value = 0;
    goto ensure_stack_spaceB_return;}
  else {
    interruptP_2029X = (s48_Sstack_limitS) == (((char *) -1));
    s48_Sstack_limitS = (Sreal_stack_limitS);
    if ((need_429X < (64 + (((((SstackS) - (Sreal_stack_limitS)))>>2))))) {
      ensure_stack_spaceB0_return_value = interruptP_2029X;
      goto ensure_stack_spaceB_return;}
    else {s48_copy_stack_into_heap();
      if ((need_429X < (64 + (((((SstackS) - (Sreal_stack_limitS)))>>2))))) {
        ensure_stack_spaceB0_return_value = interruptP_2029X;
        goto ensure_stack_spaceB_return;}
      else {
        ps_error("VM's stack is too small (how can this happen?)", 0);
        ensure_stack_spaceB0_return_value = interruptP_2029X;
        goto ensure_stack_spaceB_return;}}}}
 ensure_stack_spaceB_return:
  switch (ensure_stack_spaceB_return_tag) {
  case 0: goto ensure_stack_spaceB_return_0;
  default: goto ensure_stack_spaceB_return_1;
  }}

 pop_args_GlistSAgc: {
  start_427X = merged_arg0K0;
  count_428X = merged_arg0K1;{
  Stemp0S = start_427X;s48_make_availableAgc(((((3 * count_428X))<<2)));
  value_2030X = Stemp0S;
  Stemp0S = 1;
  arg0K0 = value_2030X;
  arg0K1 = count_428X;
  goto L17147;}
 L17147: {
  args_2031X = arg0K0;
  count_2032X = arg0K1;
  if ((0 == count_2032X)) {
    pop_args_GlistSAgc0_return_value = args_2031X;
    goto pop_args_GlistSAgc_return;}
  else {
    a_2033X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    addr_2034X = s48_allocate_small(12);
    *((long *) addr_2034X) = 2050;
    x_2035X = 3 + (((long) (addr_2034X + 4)));
    *((long *) (((char *) (-3 + x_2035X)))) = a_2033X;
    *((long *) ((((char *) (-3 + x_2035X))) + 4)) = args_2031X;
    arg0K0 = x_2035X;
    arg0K1 = (-1 + count_2032X);
    goto L17147;}}
 pop_args_GlistSAgc_return:
  switch (pop_args_GlistSAgc_return_tag) {
  case 0: goto pop_args_GlistSAgc_return_0;
  case 1: goto pop_args_GlistSAgc_return_1;
  case 2: goto pop_args_GlistSAgc_return_2;
  case 3: goto pop_args_GlistSAgc_return_3;
  case 4: goto pop_args_GlistSAgc_return_4;
  default: goto pop_args_GlistSAgc_return_5;
  }}

 copy_listSAgc: {
  list_425X = merged_arg0K0;
  length_426X = merged_arg0K1;{
  if ((0 == length_426X)) {
    copy_listSAgc0_return_value = 25;
    goto copy_listSAgc_return;}
  else {
    Stemp0S = list_425X;s48_make_availableAgc(((((3 * length_426X))<<2)));
    value_2036X = Stemp0S;
    Stemp0S = 1;
    a_2037X = *((long *) (((char *) (-3 + value_2036X))));
    addr_2038X = s48_allocate_small(12);
    *((long *) addr_2038X) = 2050;
    x_2039X = 3 + (((long) (addr_2038X + 4)));
    *((long *) (((char *) (-3 + x_2039X)))) = a_2037X;
    *((long *) ((((char *) (-3 + x_2039X))) + 4)) = 25;
    arg0K0 = (*((long *) ((((char *) (-3 + value_2036X))) + 4)));
    arg0K1 = x_2039X;
    goto L17035;}}
 L17035: {
  l_2040X = arg0K0;
  last_2041X = arg0K1;
  if ((25 == l_2040X)) {
    copy_listSAgc0_return_value = x_2039X;
    goto copy_listSAgc_return;}
  else {
    a_2042X = *((long *) (((char *) (-3 + l_2040X))));
    addr_2043X = s48_allocate_small(12);
    *((long *) addr_2043X) = 2050;
    x_2044X = 3 + (((long) (addr_2043X + 4)));
    *((long *) (((char *) (-3 + x_2044X)))) = a_2042X;
    *((long *) ((((char *) (-3 + x_2044X))) + 4)) = 25;
    addr_2045X = (((char *) (-3 + last_2041X))) + 4;S48_WRITE_BARRIER(last_2041X, addr_2045X, x_2044X);
    *((long *) addr_2045X) = x_2044X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_2040X))) + 4)));
    arg0K1 = x_2044X;
    goto L17035;}}
 copy_listSAgc_return:
  switch (copy_listSAgc_return_tag) {
  case 0: goto copy_listSAgc_return_0;
  case 1: goto copy_listSAgc_return_1;
  case 2: goto copy_listSAgc_return_2;
  default: goto copy_listSAgc_return_3;
  }}

 shift_space: {
  x_423X = merged_arg0K0;
  n_424X = merged_arg0K1;{
  if ((0 == (3 & x_423X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17724;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_423X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17724;}}
 L17724: {
  x_size_2046X = arg0K0;
  extra_2047X = arg0K1;
  if ((n_424X < 0)) {
    if ((x_size_2046X < 1)) {
      arg0K0 = 1;
      goto L17769;}
    else {
      arg0K0 = x_size_2046X;
      goto L17769;}}
  else {
    n_2048X = n_424X / 30;
    arg0K0 = (5 + (x_size_2046X + n_2048X));
    goto L17744;}}
 L17769: {
  v_2049X = arg0K0;
  arg0K0 = (9 + ((((v_2049X)<<1)) + x_size_2046X));
  goto L17744;}
 L17744: {
  v_2050X = arg0K0;
  shift_space0_return_value = (extra_2047X + v_2050X);
  goto shift_space_return;}
 shift_space_return:
  switch (shift_space_return_tag) {
  case 0: goto shift_space_return_0;
  default: goto shift_space_return_1;
  }}

 pop_continuationB: {
{ SstackS = (ScontS);
  cont_2051X = ScontS;
  pointer_2052X = (((char *) (*((long *) cont_2051X)))) + -2;
  size_2053X = ((((*((unsigned char *) pointer_2052X)))<<8)) + (*((unsigned char *) (pointer_2052X + 1)));
  if ((65535 == size_2053X)) {
    arg0K0 = ((((*((long *) (cont_2051X + 4))))>>2));
    goto L16879;}
  else {
    arg0K0 = size_2053X;
    goto L16879;}}
 L16879: {
  v_2054X = arg0K0;
  ScontS = (cont_2051X + (4 + (((v_2054X)<<2))));
  v_2055X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  Scode_pointerS = (((char *) v_2055X));
  Slast_code_pointer_resumedS = (Scode_pointerS);
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

 okay_copy_argsP: {
  thing_420X = merged_arg0K0;
  index_421X = merged_arg0K1;
  count_422X = merged_arg0K2;{
  if ((index_421X < 0)) {
    okay_copy_argsP0_return_value = 0;
    goto okay_copy_argsP_return;}
  else {
    if ((3 == (3 & thing_420X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_420X))) + -4))))>>2))))) {
        if (((-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_420X))) + -4))))>>8))) < (index_421X + count_422X))) {
          okay_copy_argsP0_return_value = 0;
          goto okay_copy_argsP_return;}
        else {
          okay_copy_argsP0_return_value = 1;
          goto okay_copy_argsP_return;}}
      else {
        goto L16568;}}
    else {
      goto L16568;}}}
 L16568: {
  if ((3 == (3 & thing_420X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + thing_420X))) + -4))))>>2))))) {
      if ((((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_420X))) + -4))))>>8)) < (index_421X + count_422X))) {
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

 proposal_d_write: {
  stob_417X = merged_arg0K0;
  index_418X = merged_arg0K1;
  value_419X = merged_arg0K2;{
  log_2056X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L15224;}
 L15224: {
  i_2057X = arg0K0;
  next_stob_2058X = *((long *) ((((char *) (-3 + log_2056X))) + (((i_2057X)<<2))));
  if ((1 == next_stob_2058X)) {add_log_entryAgc(1, i_2057X, stob_417X, index_418X, value_419X, 0);
    goto proposal_d_write_return;}
  else {
    if ((stob_417X == next_stob_2058X)) {
      if ((index_418X == (*((long *) ((((char *) (-3 + log_2056X))) + (4 + (((i_2057X)<<2)))))))) {
        addr_2059X = (((char *) (-3 + log_2056X))) + (8 + (((i_2057X)<<2)));S48_WRITE_BARRIER(log_2056X, addr_2059X, value_419X);
        *((long *) addr_2059X) = value_419X;
        goto proposal_d_write_return;}
      else {
        goto L15244;}}
    else {
      goto L15244;}}}
 L15244: {
  arg0K0 = (4 + i_2057X);
  goto L15224;}
 proposal_d_write_return:
  switch (proposal_d_write_return_tag) {
  case 0: goto proposal_d_write_return_0;
  case 1: goto proposal_d_write_return_1;
  default: goto proposal_d_write_return_2;
  }}

 proposal_d_read: {
  stob_415X = merged_arg0K0;
  index_416X = merged_arg0K1;{
  log_2060X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L15047;}
 L15047: {
  i_2061X = arg0K0;
  next_stob_2062X = *((long *) ((((char *) (-3 + log_2060X))) + (((i_2061X)<<2))));
  if ((1 == next_stob_2062X)) {
    v_2063X = add_log_entryAgc(1, i_2061X, stob_415X, index_416X, (*((long *) ((((char *) (-3 + stob_415X))) + (-4 & index_416X)))), 1);
    proposal_d_read0_return_value = v_2063X;
    goto proposal_d_read_return;}
  else {
    if ((stob_415X == next_stob_2062X)) {
      if ((index_416X == (*((long *) ((((char *) (-3 + log_2060X))) + (4 + (((i_2061X)<<2)))))))) {
        proposal_d_read0_return_value = (*((long *) ((((char *) (-3 + log_2060X))) + (8 + (((i_2061X)<<2))))));
        goto proposal_d_read_return;}
      else {
        goto L15069;}}
    else {
      goto L15069;}}}
 L15069: {
  arg0K0 = (4 + i_2061X);
  goto L15047;}
 proposal_d_read_return:
  switch (proposal_d_read_return_tag) {
  case 0: goto proposal_d_read_return_0;
  case 1: goto proposal_d_read_return_1;
  default: goto proposal_d_read_return_2;
  }}

 pending_interruptP: {
{ if ((s48_Spending_eventsPS)) {
    s48_Spending_eventsPS = 0;
    goto L15018;}
  else {
    goto L15004;}}
 L15018: {
  type_2064X = s48_get_next_event(&channel_2065X, &status_2066X);
  if ((type_2064X == ALARM_EVENT)) {
    arg0K0 = 1;
    goto L15024;}
  else {
    if ((type_2064X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L15024;}
    else {
      if ((type_2064X == IO_COMPLETION_EVENT)) {enqueue_channelB(channel_2065X, status_2066X);
        arg0K0 = 8;
        goto L15024;}
      else {
        if ((type_2064X == OS_SIGNAL_EVENT)) {
          arg0K0 = 16;
          goto L15024;}
        else {
          if ((type_2064X == NO_EVENT)) {
            arg0K0 = 0;
            goto L15024;}
          else {
            if ((type_2064X == ERROR_EVENT)) {
              ps_write_string("OS error while getting event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              ps_write_string((ps_error_string(status_2066X)), (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L15024;}
            else {
              ps_write_string("unknown type of event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L15024;}}}}}}}
 L15004: {
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Sstack_limitS = (Sreal_stack_limitS);
    if ((s48_Spending_eventsPS)) {
      s48_Sstack_limitS = (((char *) -1));
      pending_interruptP0_return_value = 0;
      goto pending_interruptP_return;}
    else {
      pending_interruptP0_return_value = 0;
      goto pending_interruptP_return;}}
  else {
    pending_interruptP0_return_value = 1;
    goto pending_interruptP_return;}}
 L15024: {
  v_2067X = arg0K0;
  Spending_interruptsS = (v_2067X | (Spending_interruptsS));
  if ((type_2064X == NO_EVENT)) {
    goto L15004;}
  else {
    goto L15018;}}
 pending_interruptP_return:
  switch (pending_interruptP_return_tag) {
  case 0: goto pending_interruptP_return_0;
  case 1: goto pending_interruptP_return_1;
  case 2: goto pending_interruptP_return_2;
  default: goto pending_interruptP_return_3;
  }}

 make_closure: {
  a_413X = merged_arg0K0;
  b_414X = merged_arg0K1;{
  addr_2068X = s48_allocate_small(12);
  *((long *) addr_2068X) = 2062;
  x_2069X = 3 + (((long) (addr_2068X + 4)));
  *((long *) (((char *) (-3 + x_2069X)))) = a_413X;
  *((long *) ((((char *) (-3 + x_2069X))) + 4)) = b_414X;
  if ((3 == (3 & x_2069X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_2069X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_2069X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_2069X))) + -4))));
      make_closure0_return_value = x_2069X;
      goto make_closure_return;}
    else {
      make_closure0_return_value = x_2069X;
      goto make_closure_return;}}
  else {
    make_closure0_return_value = x_2069X;
    goto make_closure_return;}}
 make_closure_return:
  switch (make_closure_return_tag) {
  case 0: goto make_closure_return_0;
  default: goto make_closure_return_1;
  }}

 get_current_port: {
  marker_412X = merged_arg0K0;{
  thread_2070X = Scurrent_threadS;
  if ((3 == (3 & thread_2070X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thread_2070X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_2070X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_2070X))) + 4)));
        goto L13465;}
      else {
        goto L13515;}}
    else {
      goto L13515;}}
  else {
    goto L13515;}}
 L13465: {
  env_2071X = arg0K0;
  if ((3 == (3 & env_2071X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_2071X))) + -4))))>>2))))) {
      obj_2072X = *((long *) (((char *) (-3 + env_2071X))));
      if ((3 == (3 & obj_2072X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_2072X))) + -4))))>>2))))) {
          if ((marker_412X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_2071X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_2071X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_2071X))) + 4)));
            goto L13465;}}
        else {
          goto L13537;}}
      else {
        goto L13537;}}
    else {
      goto L13537;}}
  else {
    goto L13537;}}
 L13515: {
  ps_error("current thread is not a record", 0);
  get_current_port0_return_value = v_2073X;
  goto get_current_port_return;}
 L13537: {
  if ((25 == env_2071X)) {
    if (((((marker_412X)>>2)) == 1)) {
      arg3K0 = "dynamic environment doesn't have current-output-port";
      goto L13491;}
    else {
      arg3K0 = "dynamic environment doesn't have current-input-port";
      goto L13491;}}
  else {
    ps_error("dynamic environment is not a proper list", 0);
    get_current_port0_return_value = v_2074X;
    goto get_current_port_return;}}
 L13491: {
  v_2075X = arg3K0;
  ps_error(v_2075X, 0);
  get_current_port0_return_value = v_2076X;
  goto get_current_port_return;}
 get_current_port_return:
  switch (get_current_port_return_tag) {
  case 0: goto get_current_port_return_0;
  case 1: goto get_current_port_return_1;
  default: goto get_current_port_return_2;
  }}

 okay_argument_list: {
  list_411X = merged_arg0K0;{
  arg0K0 = list_411X;
  arg0K1 = 0;
  arg0K2 = list_411X;
  arg2K3 = 0;
  goto L13332;}
 L13332: {
  fast_2077X = arg0K0;
  len_2078X = arg0K1;
  slow_2079X = arg0K2;
  move_slowP_2080X = arg2K3;
  if ((25 == fast_2077X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_2078X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_2077X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_2077X))) + -4))))>>2))))) {
        if (move_slowP_2080X) {
          if ((fast_2077X == slow_2079X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_2077X))) + 4)));
            arg0K1 = (1 + len_2078X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_2079X))) + 4)));
            arg2K3 = 0;
            goto L13332;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_2077X))) + 4)));
          arg0K1 = (1 + len_2078X);
          arg0K2 = slow_2079X;
          arg2K3 = 1;
          goto L13332;}}
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

 copy_continuation_from_heapB: {
  cont_409X = merged_arg0K0;
  stack_arg_count_410X = merged_arg0K1;{
  stack_size_2081X = -2 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_409X))) + -4))))>>8))))>>2));
  new_cont_2082X = (Sbottom_of_stackS) + (0 - (((stack_size_2081X)<<2)));
  if ((0 == stack_arg_count_410X)) {
    SstackS = new_cont_2082X;
    goto L13227;}
  else {
    new_stack_2083X = new_cont_2082X + (0 - (((stack_arg_count_410X)<<2)));
    if ((new_stack_2083X < (SstackS))) {
      memcpy((void *)new_stack_2083X, (void *)(SstackS),(((stack_arg_count_410X)<<2)));
      SstackS = new_stack_2083X;
      goto L13227;}
    else {
      goto L13227;}}}
 L13227: {
  ScontS = new_cont_2082X;
  memcpy((void *)(new_cont_2082X + 4), (void *)((((char *) (-3 + cont_409X))) + 12),(-4 + (((stack_size_2081X)<<2))));
  *((long *) new_cont_2082X) = (((long) ((((char *) (-3 + (*((long *) ((((char *) (-3 + cont_409X))) + 4)))))) + ((((*((long *) (((char *) (-3 + cont_409X))))))>>2)))));
  Sheap_continuationS = (*((long *) ((((char *) (-3 + cont_409X))) + 8)));
  copy_continuation_from_heapB0_return_value = new_cont_2082X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  case 1: goto copy_continuation_from_heapB_return_1;
  default: goto copy_continuation_from_heapB_return_2;
  }}

 get_error_string: {
  status_408X = merged_arg0K0;{
  string_2084X = ps_error_string(status_408X);
  x_2085X = strlen((char *) string_2084X);
  if ((x_2085X < 256)) {
    arg0K0 = x_2085X;
    goto L12463;}
  else {
    arg0K0 = 256;
    goto L12463;}}
 L12463: {
  len_2086X = arg0K0;
  len_2087X = 1 + len_2086X;
  addr_2088X = s48_allocate_small((4 + len_2087X));
  *((long *) addr_2088X) = (70 + (((len_2087X)<<8)));
  string_2089X = 3 + (((long) (addr_2088X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_2089X))) + len_2086X)) = 0;
  arg0K0 = 0;
  goto L12473;}
 L12473: {
  i_2090X = arg0K0;
  if ((i_2090X == len_2086X)) {
    get_error_string0_return_value = string_2089X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_2089X))) + i_2090X)) = ((*(string_2084X + i_2090X)));
    arg0K0 = (1 + i_2090X);
    goto L12473;}}
 get_error_string_return:
  switch (get_error_string_return_tag) {
  case 0: goto get_error_string_return_0;
  case 1: goto get_error_string_return_1;
  case 2: goto get_error_string_return_2;
  case 3: goto get_error_string_return_3;
  case 4: goto get_error_string_return_4;
  default: goto get_error_string_return_5;
  }}

 move_args_above_contB: {
  nargs_407X = merged_arg0K0;{
  top_of_args_2091X = SstackS;
  if (((ScontS) == (top_of_args_2091X + (((nargs_407X)<<2))))) {
    goto move_args_above_contB_return;}
  else {
    SstackS = (ScontS);
    arg1K0 = ((SstackS) + -4);
    arg1K1 = (top_of_args_2091X + (-4 + (((nargs_407X)<<2))));
    goto L5425;}}
 L5425: {
  loc_2092X = arg1K0;
  arg_2093X = arg1K1;
  if ((arg_2093X < top_of_args_2091X)) {
    SstackS = ((SstackS) + (0 - (((nargs_407X)<<2))));
    goto move_args_above_contB_return;}
  else {
    *((long *) loc_2092X) = (*((long *) arg_2093X));
    arg1K0 = (loc_2092X + -4);
    arg1K1 = (arg_2093X + -4);
    goto L5425;}}
 move_args_above_contB_return:
  switch (move_args_above_contB_return_tag) {
  case 0: goto move_args_above_contB_return_0;
  case 1: goto move_args_above_contB_return_1;
  case 2: goto move_args_above_contB_return_2;
  case 3: goto move_args_above_contB_return_3;
  case 4: goto move_args_above_contB_return_4;
  case 5: goto move_args_above_contB_return_5;
  case 6: goto move_args_above_contB_return_6;
  case 7: goto move_args_above_contB_return_7;
  case 8: goto move_args_above_contB_return_8;
  case 9: goto move_args_above_contB_return_9;
  case 10: goto move_args_above_contB_return_10;
  case 11: goto move_args_above_contB_return_11;
  case 12: goto move_args_above_contB_return_12;
  case 13: goto move_args_above_contB_return_13;
  default: goto move_args_above_contB_return_14;
  }}

}
long s48_call_startup_procedure(char **startup_vector_2094X, long startup_vector_length_2095X)
{
  long arg0K0;
  long merged_arg0K1;
  char *merged_arg3K0;

  int enter_string_return_tag;
  long enter_string0_return_value;
  char *string_2096X;
  long i_2136X;
  long string_2135X;
  char * addr_2134X;
  long len_2133X;
  long len_2132X;
  long v_2131X;
  long x_2130X;
  long x_2129X;
  char * addr_2128X;
  long b_2127X;
  long channel_2126X;
  long x_2125X;
  char * addr_2124X;
  long b_2123X;
  long channel_2122X;
  long x_2121X;
  char * addr_2120X;
  long b_2119X;
  long channel_2118X;
  long i_2117X;
  long length_2116X;
  long *v_2115X;
  long v_2114X;
  long v_2113X;
  long y_2112X;
  long x_2111X;
  long v_2110X;
  long x_2109X;
  long y_2108X;
  char * addr_2107X;
  long value_2106X;
  long x_2105X;
  long y_2104X;
  long i_2103X;
  long vector_2102X;
  long v_2101X;
  long vector_2100X;
  char * addr_2099X;
  long len_2098X;
  long code_2097X;
 {  SstackS = (Sbottom_of_stackS);
  Sheap_continuationS = 1;
  ScontS = (Sbottom_of_stackS);
  code_2097X = Sinterrupt_return_codeS;
  Slast_code_calledS = code_2097X;
  Scode_pointerS = (((char *) (-3 + code_2097X)));
  Slast_code_pointer_resumedS = (Scode_pointerS);
  SvalS = 13;
  Scurrent_threadS = 25;SHARED_SETB((Ssession_dataS), 25);SHARED_SETB((Sexception_handlersS), 25);SHARED_SETB((Sinterrupt_handlersS), 25);
  Senabled_interruptsS = 0;SHARED_SETB((Sfinalizer_alistS), 25);
  Sfinalize_theseS = 25;
  Spending_interruptsS = 0;
  s48_Spending_interruptPS = 0;
  Sinterrupted_templateS = 1;
  len_2098X = ((startup_vector_length_2095X)<<2);
  addr_2099X = s48_allocate_tracedAgc((4 + len_2098X));
  if ((addr_2099X == NULL)) {
    arg0K0 = 1;
    goto L17638;}
  else {
    *((long *) addr_2099X) = (10 + (((len_2098X)<<8)));
    arg0K0 = (3 + (((long) (addr_2099X + 4))));
    goto L17638;}}
 L17638: {
  vector_2100X = arg0K0;
  if ((1 == vector_2100X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_2101X;
    goto L17624;}
  else {
    arg0K0 = vector_2100X;
    goto L17624;}}
 L17624: {
  vector_2102X = arg0K0;
  arg0K0 = 0;
  goto L17661;}
 L17661: {
  i_2103X = arg0K0;
  if ((i_2103X == startup_vector_length_2095X)) {
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = vector_2102X;
    y_2104X = fileno((stderr));
    x_2105X = fileno((stdout));
    if ((x_2105X < y_2104X)) {
      arg0K0 = y_2104X;
      goto L18969;}
    else {
      arg0K0 = x_2105X;
      goto L18969;}}
  else {
    value_2106X = enter_stringAgc((*(startup_vector_2094X + i_2103X)));
    addr_2107X = (((char *) (-3 + vector_2102X))) + (((i_2103X)<<2));S48_WRITE_BARRIER(vector_2102X, addr_2107X, value_2106X);
    *((long *) addr_2107X) = value_2106X;
    arg0K0 = (1 + i_2103X);
    goto L17661;}}
 L18969: {
  y_2108X = arg0K0;
  x_2109X = fileno((stdin));
  if ((x_2109X < y_2108X)) {
    arg0K0 = y_2108X;
    goto L18971;}
  else {
    arg0K0 = x_2109X;
    goto L18971;}}
 L18971: {
  v_2110X = arg0K0;
  x_2111X = Snumber_of_channelsS;
  y_2112X = 1 + v_2110X;
  if ((x_2111X < y_2112X)) {
    arg0K0 = y_2112X;
    goto L18973;}
  else {
    arg0K0 = x_2111X;
    goto L18973;}}
 L18973: {
  v_2113X = arg0K0;
  Snumber_of_channelsS = v_2113X;
  v_2114X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L18995;}
  else {
    goto L18995;}}
 L18995: {
  v_2115X = Svm_channelsS;
  length_2116X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L19043;}
 L19043: {
  i_2117X = arg0K0;
  if ((i_2117X < length_2116X)) {
    *(v_2115X + i_2117X) = 1;
    arg0K0 = (1 + i_2117X);
    goto L19043;}
  else {s48_make_availableAgc(((((3 * (7 + ((((4 + (strlen((char *) "standard output"))))>>2)))))<<2)));
    channel_2118X = fileno((stdin));
    merged_arg3K0 = "standard input";
    merged_arg0K1 = 0;
    enter_string_return_tag = 0;
    goto enter_string;
   enter_string_return_0:
    b_2119X = enter_string0_return_value;
    addr_2120X = s48_allocate_small(24);
    *((long *) addr_2120X) = 5146;
    x_2121X = 3 + (((long) (addr_2120X + 4)));
    *((long *) (((char *) (-3 + x_2121X)))) = 4;
    *((long *) ((((char *) (-3 + x_2121X))) + 4)) = b_2119X;
    *((long *) ((((char *) (-3 + x_2121X))) + 8)) = (((channel_2118X)<<2));
    *((long *) ((((char *) (-3 + x_2121X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_2121X))) + 16)) = 1;
    *((Svm_channelsS) + channel_2118X) = x_2121X;
    channel_2122X = fileno((stderr));
    merged_arg3K0 = "standard error";
    merged_arg0K1 = 0;
    enter_string_return_tag = 1;
    goto enter_string;
   enter_string_return_1:
    b_2123X = enter_string0_return_value;
    addr_2124X = s48_allocate_small(24);
    *((long *) addr_2124X) = 5146;
    x_2125X = 3 + (((long) (addr_2124X + 4)));
    *((long *) (((char *) (-3 + x_2125X)))) = 8;
    *((long *) ((((char *) (-3 + x_2125X))) + 4)) = b_2123X;
    *((long *) ((((char *) (-3 + x_2125X))) + 8)) = (((channel_2122X)<<2));
    *((long *) ((((char *) (-3 + x_2125X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_2125X))) + 16)) = 1;
    *((Svm_channelsS) + channel_2122X) = x_2125X;
    channel_2126X = fileno((stdout));
    merged_arg3K0 = "standard output";
    merged_arg0K1 = 0;
    enter_string_return_tag = 2;
    goto enter_string;
   enter_string_return_2:
    b_2127X = enter_string0_return_value;
    addr_2128X = s48_allocate_small(24);
    *((long *) addr_2128X) = 5146;
    x_2129X = 3 + (((long) (addr_2128X + 4)));
    *((long *) (((char *) (-3 + x_2129X)))) = 8;
    *((long *) ((((char *) (-3 + x_2129X))) + 4)) = b_2127X;
    *((long *) ((((char *) (-3 + x_2129X))) + 8)) = (((channel_2126X)<<2));
    *((long *) ((((char *) (-3 + x_2129X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_2129X))) + 16)) = 1;
    *((Svm_channelsS) + channel_2126X) = x_2129X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2121X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2129X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2125X;
    x_2130X = s48_resumer_records();
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2130X;s48_initialization_completeB();
    v_2131X = s48_startup_procedure();
    return s48_restart(v_2131X, 5);}}
 enter_string: {
  string_2096X = merged_arg3K0;{
  len_2132X = strlen((char *) string_2096X);
  len_2133X = 1 + len_2132X;
  addr_2134X = s48_allocate_small((4 + len_2133X));
  *((long *) addr_2134X) = (70 + (((len_2133X)<<8)));
  string_2135X = 3 + (((long) (addr_2134X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_2135X))) + len_2132X)) = 0;
  arg0K0 = 0;
  goto L14893;}
 L14893: {
  i_2136X = arg0K0;
  if ((i_2136X == len_2132X)) {
    enter_string0_return_value = string_2135X;
    goto enter_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_2135X))) + i_2136X)) = ((*(string_2096X + i_2136X)));
    arg0K0 = (1 + i_2136X);
    goto L14893;}}
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
Spost_gc_cleanupS = HtopD10322;
Sgc_root_procS = HtopD10333;
s48_Scallback_return_stack_blockS = 1;
s48_Spending_eventsPS = 0;
}
