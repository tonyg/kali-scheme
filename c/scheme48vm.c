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
static long current_code_vector(void);
static long channel_close_error(long, long, long);
static long make_registered_channel(long, long, long, long, long*);
static long really_preserve_continuation(long);
static long Hlookup2690(long, long, long);
static long Hlookup2671(long, long, long);
static void HtopD10322(void);
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
void s48_setup_external_exception(long, long);
void s48_copy_stack_into_heap(void);
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
    goto L2965;}
  else {
    arg0K0 = x_5X;
    goto L2965;}}
 L2965: {
  new_count_7X = arg0K0;
  new_vm_channels_8X = (long*)malloc(sizeof(long) * new_count_7X);
  if ((NULL == new_vm_channels_8X)) {
    return 0;}
  else {
    arg0K0 = 0;
    goto L2977;}}
 L2977: {
  i_9X = arg0K0;
  if ((i_9X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L3050;}
  else {
    *(new_vm_channels_8X + i_9X) = (*((Svm_channelsS) + i_9X));
    arg0K0 = (1 + i_9X);
    goto L2977;}}
 L3050: {
  i_10X = arg0K0;
  if ((i_10X == new_count_7X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_8X;
    Snumber_of_channelsS = new_count_7X;
    return 1;}
  else {
    *(new_vm_channels_8X + i_10X) = 1;
    arg0K0 = (1 + i_10X);
    goto L3050;}}
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
    goto L11233;}
  else {
    goto L11233;}}
 L11233: {
  v_29X = *((long *) (((char *) (-3 + channel_25X))));
  if ((4 == v_29X)) {
    goto L11248;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_25X))))))) {
      goto L11248;}
    else {
      v_30X = ps_close_fd(os_index_26X);
      arg0K0 = v_30X;
      goto L11255;}}}
 L11248: {
  v_31X = ps_close_fd(os_index_26X);
  arg0K0 = v_31X;
  goto L11255;}
 L11255: {
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
      goto L9568;}
    else {
      arg0K0 = (-1 + (((log_size_41X)<<1)));
      goto L9568;}}
  else {
    arg0K0 = proposal_40X;
    arg0K1 = stob_36X;
    arg0K2 = value_38X;
    goto L13151;}}
 L9568: {
  new_size_42X = arg0K0;
  len_43X = ((new_size_42X)<<2);
  addr_44X = s48_allocate_tracedAgc((4 + len_43X));
  if ((addr_44X == NULL)) {
    arg0K0 = 1;
    goto L9614;}
  else {
    *((long *) addr_44X) = (10 + (((len_43X)<<8)));
    arg0K0 = (3 + (((long) (addr_44X + 4))));
    goto L9614;}}
 L13151: {
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
    goto L13172;}
  else {
    arg0K0 = 29;
    goto L13172;}}
 L9614: {
  vector_52X = arg0K0;
  if ((1 == vector_52X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_53X;
    goto L9572;}
  else {
    arg0K0 = vector_52X;
    goto L9572;}}
 L13172: {
  value_54X = arg0K0;
  addr_55X = (((char *) (-3 + log_48X))) + (12 + (((i_35X)<<2)));S48_WRITE_BARRIER(log_48X, addr_55X, value_54X);
  *((long *) addr_55X) = value_54X;
  addr_56X = (((char *) (-3 + log_48X))) + (16 + (((i_35X)<<2)));S48_WRITE_BARRIER(log_48X, addr_56X, 1);
  *((long *) addr_56X) = 1;
  return value_47X;}
 L9572: {
  new_57X = arg0K0;
  proposal_58X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 < log_size_41X)) {
    stob_59X = *((long *) ((((char *) (-3 + proposal_58X))) + 4));
    memcpy((void *)(((char *) (-3 + new_57X))), (void *)(((char *) (-3 + stob_59X))),(-4 + (((log_size_41X)<<2))));
    goto L9592;}
  else {
    goto L9592;}}
 L9592: {
  arg0K0 = (4 + log_size_41X);
  goto L9596;}
 L9596: {
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
    goto L13151;}
  else {
    addr_65X = (((char *) (-3 + new_57X))) + (((i_60X)<<2));S48_WRITE_BARRIER(new_57X, addr_65X, 0);
    *((long *) addr_65X) = 0;
    arg0K0 = (1 + i_60X);
    goto L9596;}}
}
static long enter_bignum(char * external_bignum_66X)
{
  long desc_67X;
 {  desc_67X = 3 + (((long) external_bignum_66X));
  if ((3 == (3 & desc_67X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + desc_67X))) + -4))))>>2))))) {
      goto L13895;}
    else {
      goto L13909;}}
  else {
    goto L13909;}}
 L13895: {
  if ((3 == (3 & desc_67X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + desc_67X))) + -4)))))) {
      *((long *) ((((char *) (-3 + desc_67X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + desc_67X))) + -4))));
      return desc_67X;}
    else {
      return desc_67X;}}
  else {
    return desc_67X;}}
 L13909: {
  ps_error("not a bignum", 1, desc_67X);
  goto L13895;}
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
  goto L15051;}
 L15051: {
  i_73X = arg0K0;
  if ((i_73X == z_69X)) {
    return string_72X;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_72X))) + i_73X)) = ((*(string_68X + i_73X)));
    arg0K0 = (1 + i_73X);
    goto L15051;}}
}
static long current_code_vector(void)
{
  long arg0K0;
  long x_86X;
  long v_85X;
  long x_84X;
  long header_83X;
  char * start_82X;
  char * code_pointer_81X;
  long code_80X;
  char * pointer_79X;
  char * code_pointer_78X;
  long header_77X;
  char * start_76X;
  long code_75X;
  char * code_pointer_74X;
 {  code_pointer_74X = Scode_pointerS;
  code_75X = Slast_code_calledS;
  start_76X = ((char *) (-3 + code_75X));
  if ((code_pointer_74X < start_76X)) {
    goto L15607;}
  else {
    header_77X = *((long *) ((((char *) (-3 + code_75X))) + -4));
    if ((3 == (3 & header_77X))) {
      arg0K0 = header_77X;
      goto L15628;}
    else {
      arg0K0 = code_75X;
      goto L15628;}}}
 L15607: {
  code_pointer_78X = Slast_code_pointer_resumedS;
  pointer_79X = code_pointer_78X + -5;
  code_80X = 3 + (((long) (code_pointer_78X + (0 - (((((*((unsigned char *) pointer_79X)))<<8)) + (*((unsigned char *) (pointer_79X + 1))))))));
  code_pointer_81X = Scode_pointerS;
  start_82X = ((char *) (-3 + code_80X));
  if ((code_pointer_81X < start_82X)) {
    goto L15615;}
  else {
    header_83X = *((long *) ((((char *) (-3 + code_80X))) + -4));
    if ((3 == (3 & header_83X))) {
      arg0K0 = header_83X;
      goto L15649;}
    else {
      arg0K0 = code_80X;
      goto L15649;}}}
 L15628: {
  x_84X = arg0K0;
  if ((code_pointer_74X < (start_76X + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_84X))) + -4))))>>8))))) {
    return (Slast_code_calledS);}
  else {
    goto L15607;}}
 L15615: {
  ps_error("VM error: unable to locate current code vector", 0);
  return v_85X;}
 L15649: {
  x_86X = arg0K0;
  if ((code_pointer_81X < (start_82X + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_86X))) + -4))))>>8))))) {
    return code_80X;}
  else {
    goto L15615;}}
}
static long channel_close_error(long status_87X, long index_88X, long id_89X)
{
  long v_90X;
 {  ps_write_string("Error: ", (stderr));
  ps_write_string((ps_error_string(status_87X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(" while closing port ", (stderr));
  if ((3 == (3 & id_89X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + id_89X))) + -4))))>>2))))) {
      ps_write_string((((char *)(((char *) (-3 + id_89X))))), (stderr));
      goto L16003;}
    else {
      goto L15997;}}
  else {
    goto L15997;}}
 L16003: {
  PS_WRITE_CHAR(10, (stderr), v_90X)
  return v_90X;}
 L15997: {
  ps_write_integer((((index_88X)>>2)), (stderr));
  goto L16003;}
}
static long make_registered_channel(long mode_91X, long id_92X, long os_index_93X, long key_94X, long *TT0)
{
  long x_98X;
  char * addr_97X;
  char x_96X;
  char temp_95X;
 {  temp_95X = os_index_93X < (Snumber_of_channelsS);
  if (temp_95X) {
    goto L17610;}
  else {
    x_96X = add_more_channels(os_index_93X);
    if (x_96X) {
      goto L17610;}
    else {
      *TT0 = 9;
      return 1;}}}
 L17610: {
  if ((1 == (*((Svm_channelsS) + os_index_93X)))) {
    addr_97X = s48_allocate_small(24);
    *((long *) addr_97X) = 5146;
    x_98X = 3 + (((long) (addr_97X + 4)));
    *((long *) (((char *) (-3 + x_98X)))) = (((mode_91X)<<2));
    *((long *) ((((char *) (-3 + x_98X))) + 4)) = id_92X;
    *((long *) ((((char *) (-3 + x_98X))) + 8)) = (((os_index_93X)<<2));
    *((long *) ((((char *) (-3 + x_98X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_98X))) + 16)) = 1;
    *((Svm_channelsS) + os_index_93X) = x_98X;
    *TT0 = 9;
    return x_98X;}
  else {
    *TT0 = 11;
    return 1;}}
}
static long really_preserve_continuation(long key_99X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  char * next_112X;
  long pc_111X;
  char * pointer_110X;
  char * pointer_109X;
  long new_108X;
  char * addr_107X;
  long len_106X;
  long size_105X;
  long size_104X;
  char * pointer_103X;
  long previous_102X;
  char * cont_101X;
  long temp_100X;
 {  if (((ScontS) == (Sbottom_of_stackS))) {
    goto L19097;}
  else {
    temp_100X = Sheap_continuationS;
    arg1K0 = (ScontS);
    arg0K1 = 1;
    goto L19067;}}
 L19097: {
  return (Sheap_continuationS);}
 L19067: {
  cont_101X = arg1K0;
  previous_102X = arg0K1;
  if ((cont_101X == (Sbottom_of_stackS))) {
    *((long *) ((((char *) (-3 + previous_102X))) + 8)) = temp_100X;
    ScontS = (Sbottom_of_stackS);
    goto L19097;}
  else {
    pointer_103X = (((char *) (*((long *) cont_101X)))) + -2;
    size_104X = ((((*((unsigned char *) pointer_103X)))<<8)) + (*((unsigned char *) (pointer_103X + 1)));
    if ((65535 == size_104X)) {
      arg0K0 = ((((*((long *) (cont_101X + 4))))>>2));
      goto L17682;}
    else {
      arg0K0 = size_104X;
      goto L17682;}}}
 L17682: {
  size_105X = arg0K0;
  len_106X = 12 + (((size_105X)<<2));
  addr_107X = s48_allocate_small((4 + len_106X));
  *((long *) addr_107X) = (42 + (((len_106X)<<8)));
  new_108X = 3 + (((long) (addr_107X + 4)));
  pointer_109X = ((char *) (*((long *) cont_101X)));
  pointer_110X = pointer_109X + -5;
  pc_111X = ((((*((unsigned char *) pointer_110X)))<<8)) + (*((unsigned char *) (pointer_110X + 1)));
  memcpy((void *)((((char *) (-3 + new_108X))) + 12), (void *)(cont_101X + 4),(((size_105X)<<2)));
  *((long *) (((char *) (-3 + new_108X)))) = (((pc_111X)<<2));
  *((long *) ((((char *) (-3 + new_108X))) + 4)) = (3 + (((long) (pointer_109X + (0 - pc_111X)))));
  next_112X = cont_101X + (4 + (((size_105X)<<2)));
  if ((3 == (3 & previous_102X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + previous_102X))) + -4))))>>2))))) {
      *((long *) ((((char *) (-3 + previous_102X))) + 8)) = new_108X;
      arg1K0 = next_112X;
      arg0K1 = new_108X;
      goto L19067;}
    else {
      goto L19085;}}
  else {
    goto L19085;}}
 L19085: {
  Sheap_continuationS = new_108X;
  arg1K0 = next_112X;
  arg0K1 = new_108X;
  goto L19067;}
}
static long Hlookup2690(long table_113X, long string_114X, long key_115X)
{
  long arg0K1;
  long arg0K0;
  char * addr_130X;
  long value_129X;
  long link_128X;
  long x_127X;
  char * addr_126X;
  long next_125X;
  long len_124X;
  long s2_123X;
  long foo_122X;
  long bucket_121X;
  long link_120X;
  long index_119X;
  long h_118X;
  long i_117X;
  long n_116X;
 {  n_116X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_114X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L21029;}
 L21029: {
  i_117X = arg0K0;
  h_118X = arg0K1;
  if ((i_117X < n_116X)) {
    arg0K0 = (1 + i_117X);
    arg0K1 = (h_118X + (((*((unsigned char *) ((((char *) (-3 + string_114X))) + i_117X))))));
    goto L21029;}
  else {
    index_119X = 1023 & h_118X;
    link_120X = *((long *) ((((char *) (-3 + table_113X))) + (((index_119X)<<2))));
    if ((0 == (3 & link_120X))) {
      arg0K0 = (3 + (-4 & link_120X));
      goto L20985;}
    else {
      arg0K0 = link_120X;
      goto L20985;}}}
 L20985: {
  bucket_121X = arg0K0;
  arg0K0 = bucket_121X;
  goto L20991;}
 L20991: {
  foo_122X = arg0K0;
  if ((1 == foo_122X)) {
    if ((3 == (3 & bucket_121X))) {
      arg0K0 = (-4 & bucket_121X);
      goto L20996;}
    else {
      arg0K0 = bucket_121X;
      goto L20996;}}
  else {
    s2_123X = *((long *) (((char *) (-3 + foo_122X))));
    len_124X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_114X))) + -4))))>>8);
    if ((len_124X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_123X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_123X))), (void *)(((char *) (-3 + string_114X))),len_124X)))) {
        return foo_122X;}
      else {
        goto L21011;}}
    else {
      goto L21011;}}}
 L20996: {
  next_125X = arg0K0;
  addr_126X = s48_allocate_small(20);
  *((long *) addr_126X) = 4154;
  x_127X = 3 + (((long) (addr_126X + 4)));
  *((long *) (((char *) (-3 + x_127X)))) = string_114X;
  *((long *) ((((char *) (-3 + x_127X))) + 4)) = 1;
  *((long *) ((((char *) (-3 + x_127X))) + 8)) = 13;
  *((long *) ((((char *) (-3 + x_127X))) + 12)) = next_125X;
  if ((3 == (3 & x_127X))) {
    arg0K0 = (-4 & x_127X);
    goto L21002;}
  else {
    arg0K0 = x_127X;
    goto L21002;}}
 L21011: {
  link_128X = *((long *) ((((char *) (-3 + foo_122X))) + 12));
  if ((0 == (3 & link_128X))) {
    arg0K0 = (3 + (-4 & link_128X));
    goto L20991;}
  else {
    arg0K0 = link_128X;
    goto L20991;}}
 L21002: {
  value_129X = arg0K0;
  addr_130X = (((char *) (-3 + table_113X))) + (((index_119X)<<2));S48_WRITE_BARRIER(table_113X, addr_130X, value_129X);
  *((long *) addr_130X) = value_129X;
  return x_127X;}
}
static long Hlookup2671(long table_131X, long string_132X, long key_133X)
{
  long arg0K1;
  long arg0K0;
  char * addr_148X;
  long value_147X;
  long link_146X;
  long x_145X;
  char * addr_144X;
  long next_143X;
  long len_142X;
  long s2_141X;
  long foo_140X;
  long bucket_139X;
  long link_138X;
  long index_137X;
  long h_136X;
  long i_135X;
  long n_134X;
 {  n_134X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_132X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L21181;}
 L21181: {
  i_135X = arg0K0;
  h_136X = arg0K1;
  if ((i_135X < n_134X)) {
    arg0K0 = (1 + i_135X);
    arg0K1 = (h_136X + (((*((unsigned char *) ((((char *) (-3 + string_132X))) + i_135X))))));
    goto L21181;}
  else {
    index_137X = 1023 & h_136X;
    link_138X = *((long *) ((((char *) (-3 + table_131X))) + (((index_137X)<<2))));
    if ((0 == (3 & link_138X))) {
      arg0K0 = (3 + (-4 & link_138X));
      goto L21137;}
    else {
      arg0K0 = link_138X;
      goto L21137;}}}
 L21137: {
  bucket_139X = arg0K0;
  arg0K0 = bucket_139X;
  goto L21143;}
 L21143: {
  foo_140X = arg0K0;
  if ((1 == foo_140X)) {
    if ((3 == (3 & bucket_139X))) {
      arg0K0 = (-4 & bucket_139X);
      goto L21148;}
    else {
      arg0K0 = bucket_139X;
      goto L21148;}}
  else {
    s2_141X = *((long *) (((char *) (-3 + foo_140X))));
    len_142X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_132X))) + -4))))>>8);
    if ((len_142X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_141X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_141X))), (void *)(((char *) (-3 + string_132X))),len_142X)))) {
        return foo_140X;}
      else {
        goto L21163;}}
    else {
      goto L21163;}}}
 L21148: {
  next_143X = arg0K0;
  addr_144X = s48_allocate_small(20);
  *((long *) addr_144X) = 4154;
  x_145X = 3 + (((long) (addr_144X + 4)));
  *((long *) (((char *) (-3 + x_145X)))) = string_132X;
  *((long *) ((((char *) (-3 + x_145X))) + 4)) = 5;
  *((long *) ((((char *) (-3 + x_145X))) + 8)) = 13;
  *((long *) ((((char *) (-3 + x_145X))) + 12)) = next_143X;
  if ((3 == (3 & x_145X))) {
    arg0K0 = (-4 & x_145X);
    goto L21154;}
  else {
    arg0K0 = x_145X;
    goto L21154;}}
 L21163: {
  link_146X = *((long *) ((((char *) (-3 + foo_140X))) + 12));
  if ((0 == (3 & link_146X))) {
    arg0K0 = (3 + (-4 & link_146X));
    goto L21143;}
  else {
    arg0K0 = link_146X;
    goto L21143;}}
 L21154: {
  value_147X = arg0K0;
  addr_148X = (((char *) (-3 + table_131X))) + (((index_137X)<<2));S48_WRITE_BARRIER(table_131X, addr_148X, value_147X);
  *((long *) addr_148X) = value_147X;
  return x_145X;}
}
static void HtopD10322(void)
{
  long arg0K2;
  long arg0K1;
  long arg0K0;
  long new_186X;
  long status_185X;
  long id_184X;
  long v_183X;
  char v_182X;
  long channel_181X;
  long i_180X;
  char * addr_179X;
  long next_link_178X;
  long new_foo_177X;
  char v_176X;
  char * addr_175X;
  long foo_174X;
  char * addr_173X;
  long l_172X;
  long v_171X;
  long okay_link_170X;
  long foo_link_169X;
  char * addr_168X;
  char * addr_167X;
  char * addr_166X;
  char * addr_165X;
  long val_164X;
  char tracedP_163X;
  long next_162X;
  long thing_161X;
  long pair_160X;
  long alist_159X;
  long l2_158X;
  long goners_157X;
  long okay_156X;
  long alist_155X;
  long foo_link_154X;
  long v_153X;
  long pc_152X;
  long code_151X;
  long i_150X;
  long table_149X;
 {  table_149X = s48_trace_value((Sthe_symbol_tableS));
  arg0K0 = 0;
  goto L22137;}
 L22137: {
  i_150X = arg0K0;
  if ((1024 == i_150X)) {
    Sthe_symbol_tableS = table_149X;
    code_151X = Slast_code_calledS;
    pc_152X = Ssaved_pcS;
    Slast_code_calledS = code_151X;
    Scode_pointerS = ((((char *) (-3 + code_151X))) + pc_152X);
    Slast_code_pointer_resumedS = (Scode_pointerS);
    v_153X = SHARED_REF((Sfinalizer_alistS));
    arg0K0 = v_153X;
    arg0K1 = 25;
    arg0K2 = 25;
    goto L10847;}
  else {
    foo_link_154X = *((long *) ((((char *) (-3 + table_149X))) + (((i_150X)<<2))));
    arg0K0 = foo_link_154X;
    arg0K1 = 1;
    goto L14350;}}
 L10847: {
  alist_155X = arg0K0;
  okay_156X = arg0K1;
  goners_157X = arg0K2;
  if ((25 == alist_155X)) {SHARED_SETB((Sfinalizer_alistS), okay_156X);
    l2_158X = Sfinalize_theseS;
    if ((25 == goners_157X)) {
      arg0K0 = l2_158X;
      goto L10854;}
    else {
      arg0K0 = goners_157X;
      goto L10909;}}
  else {
    alist_159X = s48_trace_value(alist_155X);
    pair_160X = s48_trace_value((*((long *) (((char *) (-3 + alist_159X))))));
    thing_161X = *((long *) (((char *) (-3 + pair_160X))));
    next_162X = *((long *) ((((char *) (-3 + alist_159X))) + 4));
    tracedP_163X = s48_extantP(thing_161X);
    val_164X = s48_trace_value(thing_161X);
    addr_165X = ((char *) (-3 + pair_160X));S48_WRITE_BARRIER(pair_160X, addr_165X, val_164X);
    *((long *) addr_165X) = val_164X;
    addr_166X = ((char *) (-3 + alist_159X));S48_WRITE_BARRIER(alist_159X, addr_166X, pair_160X);
    *((long *) addr_166X) = pair_160X;
    if (tracedP_163X) {
      addr_167X = (((char *) (-3 + alist_159X))) + 4;S48_WRITE_BARRIER(alist_159X, addr_167X, okay_156X);
      *((long *) addr_167X) = okay_156X;
      arg0K0 = next_162X;
      arg0K1 = alist_159X;
      arg0K2 = goners_157X;
      goto L10847;}
    else {
      addr_168X = (((char *) (-3 + alist_159X))) + 4;S48_WRITE_BARRIER(alist_159X, addr_168X, goners_157X);
      *((long *) addr_168X) = goners_157X;
      arg0K0 = next_162X;
      arg0K1 = okay_156X;
      arg0K2 = alist_159X;
      goto L10847;}}}
 L14350: {
  foo_link_169X = arg0K0;
  okay_link_170X = arg0K1;
  if ((0 == (3 & foo_link_169X))) {
    arg0K0 = (3 + (-4 & foo_link_169X));
    goto L14352;}
  else {
    arg0K0 = foo_link_169X;
    goto L14352;}}
 L10854: {
  v_171X = arg0K0;
  Sfinalize_theseS = v_171X;
  arg0K0 = 0;
  goto L20563;}
 L10909: {
  l_172X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_172X))) + 4))))) {
    addr_173X = (((char *) (-3 + l_172X))) + 4;S48_WRITE_BARRIER(l_172X, addr_173X, l2_158X);
    *((long *) addr_173X) = l2_158X;
    arg0K0 = goners_157X;
    goto L10854;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_172X))) + 4)));
    goto L10909;}}
 L14352: {
  foo_174X = arg0K0;
  if ((1 == foo_174X)) {
    addr_175X = (((char *) (-3 + table_149X))) + (((i_150X)<<2));S48_WRITE_BARRIER(table_149X, addr_175X, okay_link_170X);
    *((long *) addr_175X) = okay_link_170X;
    arg0K0 = (1 + i_150X);
    goto L22137;}
  else {
    v_176X = s48_extantP(foo_174X);
    if (v_176X) {
      new_foo_177X = s48_trace_value(foo_174X);
      next_link_178X = *((long *) ((((char *) (-3 + new_foo_177X))) + 4));
      addr_179X = (((char *) (-3 + new_foo_177X))) + 4;S48_WRITE_BARRIER(new_foo_177X, addr_179X, okay_link_170X);
      *((long *) addr_179X) = okay_link_170X;
      if ((3 == (3 & new_foo_177X))) {
        arg0K0 = next_link_178X;
        arg0K1 = (-4 & new_foo_177X);
        goto L14350;}
      else {
        arg0K0 = next_link_178X;
        arg0K1 = new_foo_177X;
        goto L14350;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + foo_174X))) + 4)));
      arg0K1 = okay_link_170X;
      goto L14350;}}}
 L20563: {
  i_180X = arg0K0;
  if ((i_180X == (Snumber_of_channelsS))) {
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
    channel_181X = *((Svm_channelsS) + i_180X);
    if ((1 == channel_181X)) {
      goto L20597;}
    else {
      v_182X = s48_extantP(channel_181X);
      if (v_182X) {
        v_183X = s48_trace_value(channel_181X);
        arg0K0 = v_183X;
        goto L20590;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_181X))))))) {
          arg0K0 = 1;
          goto L20590;}
        else {
          id_184X = *((long *) ((((char *) (-3 + channel_181X))) + 4));
          status_185X = close_channelB(channel_181X);
          if ((status_185X == NO_ERRORS)) {
            goto L18557;}
          else {channel_close_error(status_185X, (*((long *) ((((char *) (-3 + channel_181X))) + 8))), id_184X);
            goto L18557;}}}}}}
 L20597: {
  arg0K0 = (1 + i_180X);
  goto L20563;}
 L20590: {
  new_186X = arg0K0;
  *((Svm_channelsS) + i_180X) = new_186X;
  goto L20597;}
 L18557: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_184X))) {
    ps_write_integer((((id_184X)>>2)), (stderr));
    goto L18575;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_184X))))), (stderr));
    goto L18575;}}
 L18575: {
  ps_write_string(" ", (stderr));
  ps_write_integer(((((*((long *) ((((char *) (-3 + channel_181X))) + 8))))>>2)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L20590;}
}
static void HtopD10333(void)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long merged_arg0K0;

  int Hentry_tracer861187_return_tag;
  long Hentry_tracer8611870_return_value;
  long foo_link_188X;
  char * addr_260X;
  long next_link_259X;
  long new_foo_258X;
  long foo_257X;
  long done_link_256X;
  long foo_link_255X;
  long v_254X;
  long size_253X;
  char * pointer_252X;
  long v_251X;
  long v_250X;
  long v_249X;
  long cells_248X;
  long size_247X;
  char * pointer_246X;
  char * contents_pointer_245X;
  long new_code_244X;
  long mask_size_243X;
  char * code_pointer_242X;
  long pc_241X;
  char * pointer_240X;
  char * pointer_239X;
  char * cont_238X;
  long unused_237X;
  char * a_236X;
  char * addr_235X;
  long val_234X;
  char * addr_233X;
  long val_232X;
  long channel_231X;
  long i_230X;
  char x_229X;
  long pair_228X;
  long v_227X;
  long v_226X;
  long alist_225X;
  char * addr_224X;
  long value_223X;
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
  long v_205X;
  long code_204X;
  long i_203X;
  char * addr_202X;
  long value_201X;
  long table_200X;
  long i_199X;
  long x2_198X;
  char * cell_197X;
  long i_196X;
  long x2_195X;
  char * cell_194X;
  long table_193X;
  char * frame_192X;
  long length_191X;
  char * frame_190X;
  long v_189X;
 {  v_189X = s48_trace_value((Sempty_logS));
  Sempty_logS = v_189X;
  arg1K0 = (Sexternal_root_stackS);
  goto L7645;}
 L7645: {
  frame_190X = arg1K0;
  if ((frame_190X == NULL)) {
    arg1K0 = (Spermanent_external_rootsS);
    goto L7671;}
  else {
    length_191X = *((long *) frame_190X);
    arg0K0 = 0;
    goto L7653;}}
 L7671: {
  frame_192X = arg1K0;
  if ((frame_192X == NULL)) {s48_initializing_gc_root();
    table_193X = s48_trace_value((Simported_bindingsS));
    arg0K0 = 0;
    goto L18898;}
  else {
    cell_194X = ((char *) (*((long *) (frame_192X + 4))));
    x2_195X = s48_trace_value((*((long *) cell_194X)));
    *((long *) cell_194X) = x2_195X;
    arg1K0 = (((char *) (*((long *) frame_192X))));
    goto L7671;}}
 L7653: {
  i_196X = arg0K0;
  if ((i_196X == length_191X)) {
    arg1K0 = (((char *) (*((long *) (frame_190X + 4)))));
    goto L7645;}
  else {
    cell_197X = ((char *) (*((long *) (frame_190X + (8 + (((i_196X)<<2)))))));
    x2_198X = s48_trace_value((*((long *) cell_197X)));
    *((long *) cell_197X) = x2_198X;
    arg0K0 = (1 + i_196X);
    goto L7653;}}
 L18898: {
  i_199X = arg0K0;
  if ((1024 == i_199X)) {
    Simported_bindingsS = table_193X;
    table_200X = s48_trace_value((Sexported_bindingsS));
    arg0K0 = 0;
    goto L18919;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_193X))) + (((i_199X)<<2)))));
    Hentry_tracer861187_return_tag = 0;
    goto Hentry_tracer861187;
   Hentry_tracer861187_return_0:
    value_201X = Hentry_tracer8611870_return_value;
    addr_202X = (((char *) (-3 + table_193X))) + (((i_199X)<<2));S48_WRITE_BARRIER(table_193X, addr_202X, value_201X);
    *((long *) addr_202X) = value_201X;
    arg0K0 = (1 + i_199X);
    goto L18898;}}
 L18919: {
  i_203X = arg0K0;
  if ((1024 == i_203X)) {
    Sexported_bindingsS = table_200X;
    code_204X = current_code_vector();
    Ssaved_pcS = ((Scode_pointerS) - (((char *) (-3 + code_204X))));
    v_205X = s48_trace_value(code_204X);
    Slast_code_calledS = v_205X;
    v_206X = s48_trace_value((SvalS));
    SvalS = v_206X;
    v_207X = s48_trace_value((Scurrent_threadS));
    Scurrent_threadS = v_207X;
    v_208X = s48_trace_value((Sinterrupt_return_codeS));
    Sinterrupt_return_codeS = v_208X;
    v_209X = s48_trace_value((Sexception_return_codeS));
    Sexception_return_codeS = v_209X;
    v_210X = s48_trace_value((Scall_with_values_return_codeS));
    Scall_with_values_return_codeS = v_210X;
    v_211X = s48_trace_value((Sinterrupted_templateS));
    Sinterrupted_templateS = v_211X;
    v_212X = s48_trace_value((Sos_signal_typeS));
    Sos_signal_typeS = v_212X;
    v_213X = s48_trace_value((Sos_signal_argumentS));
    Sos_signal_argumentS = v_213X;
    v_214X = SHARED_REF((Ssession_dataS));
    v_215X = s48_trace_value(v_214X);SHARED_SETB((Ssession_dataS), v_215X);
    v_216X = SHARED_REF((Sexception_handlersS));
    v_217X = s48_trace_value(v_216X);SHARED_SETB((Sexception_handlersS), v_217X);
    v_218X = SHARED_REF((Sinterrupt_handlersS));
    v_219X = s48_trace_value(v_218X);SHARED_SETB((Sinterrupt_handlersS), v_219X);
    v_220X = SHARED_REF((Sfinalize_theseS));
    v_221X = s48_trace_value(v_220X);SHARED_SETB((Sfinalize_theseS), v_221X);
    v_222X = SHARED_REF((Sfinalizer_alistS));
    arg0K0 = v_222X;
    goto L7711;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_200X))) + (((i_203X)<<2)))));
    Hentry_tracer861187_return_tag = 1;
    goto Hentry_tracer861187;
   Hentry_tracer861187_return_1:
    value_223X = Hentry_tracer8611870_return_value;
    addr_224X = (((char *) (-3 + table_200X))) + (((i_203X)<<2));S48_WRITE_BARRIER(table_200X, addr_224X, value_223X);
    *((long *) addr_224X) = value_223X;
    arg0K0 = (1 + i_203X);
    goto L18919;}}
 L7711: {
  alist_225X = arg0K0;
  if ((25 == alist_225X)) {
    v_226X = s48_trace_value((Spending_channels_headS));
    Spending_channels_headS = v_226X;
    v_227X = s48_trace_value((Spending_channels_tailS));
    Spending_channels_tailS = v_227X;
    arg0K0 = 0;
    goto L10979;}
  else {
    pair_228X = *((long *) (((char *) (-3 + alist_225X))));
    x_229X = s48_extantP((*((long *) (((char *) (-3 + pair_228X))))));
    if (x_229X) {
      goto L7736;}
    else {s48_trace_stob_contentsB((*((long *) (((char *) (-3 + pair_228X))))));
      goto L7736;}}}
 L10979: {
  i_230X = arg0K0;
  if ((i_230X == (Snumber_of_channelsS))) {
    if ((Sstack_warningPS)) {
      arg1K0 = (Sstack_beginS);
      goto L5296;}
    else {
      goto L14602;}}
  else {
    channel_231X = *((Svm_channelsS) + i_230X);
    if ((1 == channel_231X)) {
      goto L11007;}
    else {
      if ((0 == (*((long *) (((char *) (-3 + channel_231X))))))) {
        goto L11007;}
      else {
        val_232X = s48_trace_value((*((long *) ((((char *) (-3 + channel_231X))) + 4))));
        addr_233X = (((char *) (-3 + channel_231X))) + 4;S48_WRITE_BARRIER(channel_231X, addr_233X, val_232X);
        *((long *) addr_233X) = val_232X;
        goto L11007;}}}}
 L7736: {
  val_234X = s48_trace_value((*((long *) ((((char *) (-3 + pair_228X))) + 4))));
  addr_235X = (((char *) (-3 + pair_228X))) + 4;S48_WRITE_BARRIER(pair_228X, addr_235X, val_234X);
  *((long *) addr_235X) = val_234X;
  arg0K0 = (*((long *) ((((char *) (-3 + alist_225X))) + 4)));
  goto L7711;}
 L5296: {
  a_236X = arg1K0;
  if ((252645135 == (*((long *) a_236X)))) {
    arg1K0 = (a_236X + 4);
    goto L5296;}
  else {
    unused_237X = (((a_236X - (Sstack_beginS)))>>2);
    if ((unused_237X < 30)) {
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      ps_write_string("[Alert: stack overconsumption (", (stderr));
      ps_write_integer(unused_237X, (stderr));
      ps_write_string("); please inform the Scheme 48 implementors]", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sstack_warningPS = 0;
      goto L14602;}
    else {
      goto L14602;}}}
 L14602: {
s48_trace_locationsB((SstackS), ((SstackS) + (-4 & ((ScontS) - (SstackS)))));
  arg1K0 = (ScontS);
  goto L14612;}
 L11007: {
  arg0K0 = (1 + i_230X);
  goto L10979;}
 L14612: {
  cont_238X = arg1K0;
  pointer_239X = ((char *) (*((long *) cont_238X)));
  pointer_240X = pointer_239X + -5;
  pc_241X = ((((*((unsigned char *) pointer_240X)))<<8)) + (*((unsigned char *) (pointer_240X + 1)));
  code_pointer_242X = ((char *) (*((long *) cont_238X)));
  mask_size_243X = *((unsigned char *) (code_pointer_242X + -3));
  new_code_244X = s48_trace_value((3 + (((long) (pointer_239X + (0 - pc_241X))))));
  contents_pointer_245X = cont_238X + 4;
  *((long *) cont_238X) = (((long) ((((char *) (-3 + new_code_244X))) + pc_241X)));
  if ((0 == mask_size_243X)) {
    pointer_246X = (((char *) (*((long *) cont_238X)))) + -2;
    size_247X = ((((*((unsigned char *) pointer_246X)))<<8)) + (*((unsigned char *) (pointer_246X + 1)));
    if ((65535 == size_247X)) {
      arg0K0 = ((((*((long *) (cont_238X + 4))))>>2));
      goto L12132;}
    else {
      arg0K0 = size_247X;
      goto L12132;}}
  else {s48_trace_continuation_contentsB(contents_pointer_245X, code_pointer_242X, mask_size_243X);
    goto L14622;}}
 L12132: {
  cells_248X = arg0K0;s48_trace_locationsB(contents_pointer_245X, (contents_pointer_245X + (((cells_248X)<<2))));
  goto L14622;}
 L14622: {
  if ((cont_238X == (Sbottom_of_stackS))) {
    v_249X = s48_trace_value((Sheap_continuationS));
    Sheap_continuationS = v_249X;
    v_250X = s48_trace_value((Stemp0S));
    Stemp0S = v_250X;
    v_251X = s48_trace_value((Stemp1S));
    Stemp1S = v_251X;
    return;}
  else {
    pointer_252X = (((char *) (*((long *) cont_238X)))) + -2;
    size_253X = ((((*((unsigned char *) pointer_252X)))<<8)) + (*((unsigned char *) (pointer_252X + 1)));
    if ((65535 == size_253X)) {
      arg0K0 = ((((*((long *) (cont_238X + 4))))>>2));
      goto L14660;}
    else {
      arg0K0 = size_253X;
      goto L14660;}}}
 L14660: {
  v_254X = arg0K0;
  arg1K0 = (cont_238X + (4 + (((v_254X)<<2))));
  goto L14612;}
 Hentry_tracer861187: {
  foo_link_188X = merged_arg0K0;{
  arg0K0 = foo_link_188X;
  arg0K1 = 1;
  goto L14422;}
 L14422: {
  foo_link_255X = arg0K0;
  done_link_256X = arg0K1;
  if ((0 == (3 & foo_link_255X))) {
    arg0K0 = (3 + (-4 & foo_link_255X));
    goto L14424;}
  else {
    arg0K0 = foo_link_255X;
    goto L14424;}}
 L14424: {
  foo_257X = arg0K0;
  if ((1 == foo_257X)) {
    Hentry_tracer8611870_return_value = done_link_256X;
    goto Hentry_tracer861187_return;}
  else {
    new_foo_258X = s48_trace_value(foo_257X);
    next_link_259X = *((long *) ((((char *) (-3 + new_foo_258X))) + 12));
    addr_260X = (((char *) (-3 + new_foo_258X))) + 12;S48_WRITE_BARRIER(new_foo_258X, addr_260X, done_link_256X);
    *((long *) addr_260X) = done_link_256X;
    if ((3 == (3 & new_foo_258X))) {
      arg0K0 = next_link_259X;
      arg0K1 = (-4 & new_foo_258X);
      goto L14422;}
    else {
      arg0K0 = next_link_259X;
      arg0K1 = new_foo_258X;
      goto L14422;}}}
 Hentry_tracer861187_return:
  switch (Hentry_tracer861187_return_tag) {
  case 0: goto Hentry_tracer861187_return_0;
  default: goto Hentry_tracer861187_return_1;
  }}

}
void s48_set_native_protocolB(long protocol_261X)
{

 {  s48_Snative_protocolS = protocol_261X;
  return;}
}
void s48_set_extension_valueB(long value_262X)
{

 {  s48_Sextension_valueS = value_262X;
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
void s48_set_os_signal(long type_263X, long argument_264X)
{

 {  Sos_signal_typeS = type_263X;
  Sos_signal_argumentS = argument_264X;
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
  char * old_base_265X;
 {  old_base_265X = Sexternal_root_stack_baseS;
  Sexternal_root_stack_baseS = (Sexternal_root_stackS);
  return old_base_265X;}
}
char s48_release_gc_roots_baseB(char * old_base_266X)
{
  char okayP_267X;
 {  okayP_267X = (Sexternal_root_stackS) == (Sexternal_root_stack_baseS);
  Sexternal_root_stackS = old_base_266X;
  Sexternal_root_stack_baseS = old_base_266X;
  return okayP_267X;}
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
void s48_push_gc_rootsB(char * frame_268X, long n_269X)
{

 {  *((long *) frame_268X) = n_269X;
  *((long *) (frame_268X + 4)) = (((long) (Sexternal_root_stackS)));
  Sexternal_root_stackS = frame_268X;
  return;}
}
void s48_register_gc_rootB(char * loc_addr_270X)
{
  char * frame_271X;
 {  frame_271X = (char *)malloc(8);
  if ((frame_271X == NULL)) {
    ps_error("out of memory registering a global root", 0);
    goto L3887;}
  else {
    goto L3887;}}
 L3887: {
  *((long *) frame_271X) = (((long) (Spermanent_external_rootsS)));
  *((long *) (frame_271X + 4)) = (((long) loc_addr_270X));
  Spermanent_external_rootsS = frame_271X;
  return;}
}
void s48_stack_setB(long x_272X, long value_273X)
{

 {  *((long *) ((SstackS) + (((x_272X)<<2)))) = value_273X;
  return;}
}
long s48_stack_ref(long i_274X)
{

 {  return (*((long *) ((SstackS) + (((i_274X)<<2)))));}
}
void s48_push(long x_275X)
{

 {  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_275X;
  return;}
}
long s48_resetup_external_exception(long new_why_276X, long additional_nargs_277X)
{
  long old_why_279X;
  long old_nargs_278X;
 {  old_nargs_278X = Sexternal_exception_nargsS;
  old_why_279X = *((long *) ((SstackS) + (((old_nargs_278X)<<2))));
  *((long *) ((SstackS) + (((old_nargs_278X)<<2)))) = (((new_why_276X)<<2));
  Sexternal_exception_nargsS = (old_nargs_278X + additional_nargs_277X);
  return old_why_279X;}
}
char s48_pop_gc_rootsB(void)
{

 {  if (((Sexternal_root_stackS) == (Sexternal_root_stack_baseS))) {
    return 0;}
  else {
    Sexternal_root_stackS = (((char *) (*((long *) ((Sexternal_root_stackS) + 4)))));
    return 1;}}
}
char * s48_shorten_bignum(char * external_bignum_280X, long number_of_digits_281X)
{
  long waste_size_286X;
  long old_data_size_285X;
  long new_data_size_284X;
  long new_size_283X;
  long bignum_282X;
 {  bignum_282X = 3 + (((long) external_bignum_280X));
  new_size_283X = 8 + (((number_of_digits_281X)<<2));
  new_data_size_284X = -4 + new_size_283X;
  old_data_size_285X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + bignum_282X))) + -4))))>>8);
  waste_size_286X = old_data_size_285X - new_data_size_284X;
  if ((waste_size_286X < 0)) {
    ps_error("shorten bignum", 2, new_data_size_284X, old_data_size_285X);
    goto L7022;}
  else {
    goto L7022;}}
 L7022: {
  if ((waste_size_286X < 4)) {
    return external_bignum_280X;}
  else {
    *((long *) ((((char *) (-3 + bignum_282X))) + -4)) = (82 + (((new_data_size_284X)<<8)));
    *((long *) ((((char *) (((long) ((((char *) (-3 + bignum_282X))) + (-4 & (3 + new_size_283X))))))) + -4)) = (-942 + (((waste_size_286X)<<8)));
    return external_bignum_280X;}}
}
long s48_allocate_bignum(long size_287X)
{
  char * addr_288X;
 {  addr_288X = s48_allocate_small((4 + size_287X));
  *((long *) addr_288X) = (82 + (((size_287X)<<8)));
  return (3 + (((long) (addr_288X + 4))));}
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
long s48_set_channel_os_index(long channel_289X, long os_index_290X)
{
  char * addr_297X;
  long val_296X;
  long v_295X;
  long x_294X;
  long old_index_293X;
  char x_292X;
  char temp_291X;
 {  temp_291X = os_index_290X < (Snumber_of_channelsS);
  if (temp_291X) {
    goto L11138;}
  else {
    x_292X = add_more_channels(os_index_290X);
    if (x_292X) {
      goto L11138;}
    else {
      return 36;}}}
 L11138: {
  if ((1 == (*((Svm_channelsS) + os_index_290X)))) {
    old_index_293X = (((*((long *) ((((char *) (-3 + channel_289X))) + 8))))>>2);
    x_294X = *((long *) ((((char *) (-3 + channel_289X))) + 16));
    if ((5 == x_294X)) {
      v_295X = ps_abort_fd_op(old_index_293X);enqueue_channelB(old_index_293X, v_295X);
      goto L11124;}
    else {
      goto L11124;}}
  else {
    return 44;}}
 L11124: {
  *((Svm_channelsS) + old_index_293X) = 1;
  *((Svm_channelsS) + os_index_290X) = channel_289X;
  val_296X = ((os_index_290X)<<2);
  addr_297X = (((char *) (-3 + channel_289X))) + 8;S48_WRITE_BARRIER(channel_289X, addr_297X, val_296X);
  *((long *) addr_297X) = val_296X;
  return 5;}
}
void s48_close_channel(long os_index_298X)
{
  long obj_299X;
 {  if ((os_index_298X < 0)) {
    return;}
  else {
    if ((os_index_298X < (Snumber_of_channelsS))) {
      obj_299X = *((Svm_channelsS) + os_index_298X);
      if ((3 == (3 & obj_299X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_299X))) + -4))))>>2))))) {close_channelB((*((Svm_channelsS) + os_index_298X)));
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
  long link_307X;
  long name_306X;
  long entry_305X;
  long link_304X;
  char temp_303X;
  long i_302X;
  long table_301X;
  FILE * out_300X;
 {  out_300X = stderr;
  table_301X = Simported_bindingsS;
  arg0K0 = 0;
  goto L16898;}
 L16898: {
  i_302X = arg0K0;
  temp_303X = 1024 == i_302X;
  if (temp_303X) {
    return temp_303X;}
  else {
    link_304X = *((long *) ((((char *) (-3 + table_301X))) + (((i_302X)<<2))));
    if ((0 == (3 & link_304X))) {
      arg0K0 = (3 + (-4 & link_304X));
      goto L16872;}
    else {
      arg0K0 = link_304X;
      goto L16872;}}}
 L16872: {
  entry_305X = arg0K0;
  if ((1 == entry_305X)) {
    arg0K0 = (1 + i_302X);
    goto L16898;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + entry_305X))) + 8)))))) {
      name_306X = *((long *) (((char *) (-3 + entry_305X))));
      ps_write_string("undefined imported name ", out_300X);
      if ((3 == (3 & name_306X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_306X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + name_306X))))), out_300X);
          goto L18809;}
        else {
          goto L18807;}}
      else {
        goto L18807;}}
    else {
      goto L16879;}}}
 L18809: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_300X, ignoreXX) }
  goto L16879;}
 L18807: {
  ps_write_string("<invalid name>", out_300X);
  goto L18809;}
 L16879: {
  link_307X = *((long *) ((((char *) (-3 + entry_305X))) + 12));
  if ((0 == (3 & link_307X))) {
    arg0K0 = (3 + (-4 & link_307X));
    goto L16872;}
  else {
    arg0K0 = link_307X;
    goto L16872;}}
}
void check_stack(void)
{
  char * arg1K0;
  char * arg1K1;
  long arg0K0;
  long v_329X;
  long address_328X;
  long x_327X;
  char * ptr_326X;
  long mask_325X;
  long size_324X;
  char * pointer_323X;
  long address_322X;
  long x_321X;
  char * addr_320X;
  char * trace_ptr_319X;
  char * mask_ptr_318X;
  long v_317X;
  char * mask_pointer_316X;
  long size_315X;
  char * pointer_314X;
  char * contents_pointer_313X;
  long mask_size_312X;
  char * code_pointer_311X;
  char * cont_310X;
  char * v_309X;
  char * v_308X;
 {  v_308X = s48_oldspace_begin();
  Soldspace_beginS = (((long) v_308X));
  v_309X = s48_oldspace_end();
  Soldspace_endS = (((long) v_309X));
  arg1K0 = (ScontS);
  goto L18947;}
 L18947: {
  cont_310X = arg1K0;
  if ((cont_310X == (Sbottom_of_stackS))) {
    return;}
  else {
    code_pointer_311X = ((char *) (*((long *) cont_310X)));
    mask_size_312X = *((unsigned char *) (code_pointer_311X + -3));
    contents_pointer_313X = cont_310X + 4;
    if ((0 == mask_size_312X)) {
      pointer_314X = (((char *) (*((long *) cont_310X)))) + -2;
      size_315X = ((((*((unsigned char *) pointer_314X)))<<8)) + (*((unsigned char *) (pointer_314X + 1)));
      if ((65535 == size_315X)) {
        arg0K0 = ((((*((long *) (cont_310X + 4))))>>2));
        goto L14578;}
      else {
        arg0K0 = size_315X;
        goto L14578;}}
    else {
      mask_pointer_316X = code_pointer_311X + -7;
      arg1K0 = (mask_pointer_316X + (0 - mask_size_312X));
      arg1K1 = contents_pointer_313X;
      goto L5604;}}}
 L14578: {
  v_317X = arg0K0;
  arg1K0 = contents_pointer_313X;
  goto L5537;}
 L5604: {
  mask_ptr_318X = arg1K0;
  trace_ptr_319X = arg1K1;
  if ((mask_ptr_318X == mask_pointer_316X)) {
    goto L17117;}
  else {
    arg0K0 = (*((unsigned char *) mask_ptr_318X));
    arg1K1 = trace_ptr_319X;
    goto L5612;}}
 L5537: {
  addr_320X = arg1K0;
  if ((addr_320X < (cont_310X + (4 + (((v_317X)<<2)))))) {
    x_321X = *((long *) addr_320X);
    if ((2 == (3 & x_321X))) {
      goto L5557;}
    else {
      if ((3 == (3 & x_321X))) {
        address_322X = -3 + x_321X;
        if ((address_322X < (Soldspace_beginS))) {
          goto L5542;}
        else {
          if (((Soldspace_endS) < address_322X)) {
            goto L5542;}
          else {
            goto L5557;}}}
      else {
        goto L5542;}}}
  else {
    goto L17117;}}
 L17117: {
  pointer_323X = (((char *) (*((long *) cont_310X)))) + -2;
  size_324X = ((((*((unsigned char *) pointer_323X)))<<8)) + (*((unsigned char *) (pointer_323X + 1)));
  if ((65535 == size_324X)) {
    arg0K0 = ((((*((long *) (cont_310X + 4))))>>2));
    goto L17122;}
  else {
    arg0K0 = size_324X;
    goto L17122;}}
 L5612: {
  mask_325X = arg0K0;
  ptr_326X = arg1K1;
  if ((0 == mask_325X)) {
    arg1K0 = (mask_ptr_318X + 1);
    arg1K1 = (trace_ptr_319X + 32);
    goto L5604;}
  else {
    if ((1 == (1 & mask_325X))) {
      x_327X = *((long *) ptr_326X);
      if ((2 == (3 & x_327X))) {
        goto L5669;}
      else {
        if ((3 == (3 & x_327X))) {
          address_328X = -3 + x_327X;
          if ((address_328X < (Soldspace_beginS))) {
            goto L5628;}
          else {
            if (((Soldspace_endS) < address_328X)) {
              goto L5628;}
            else {
              goto L5669;}}}
        else {
          goto L5628;}}}
    else {
      goto L5628;}}}
 L5557: {
  ps_error("bad descriptor in stack", 1, x_321X);
  goto L5542;}
 L5542: {
  arg1K0 = (addr_320X + 4);
  goto L5537;}
 L17122: {
  v_329X = arg0K0;
  arg1K0 = (cont_310X + (4 + (((v_329X)<<2))));
  goto L18947;}
 L5669: {
  ps_error("bad descriptor in stack", 1, x_327X);
  goto L5628;}
 L5628: {
  arg0K0 = (((mask_325X)>>1));
  arg1K1 = (ptr_326X + 4);
  goto L5612;}
}
long s48_really_add_channel(long mode_330X, long id_331X, long os_index_332X)
{
  long status_334X;
  long channel_333X;
 {s48_make_availableAgc(24);
  channel_333X = make_registered_channel((((mode_330X)>>2)), id_331X, os_index_332X, 0, &status_334X);
  if ((3 == (3 & channel_333X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_333X))) + -4))))>>2))))) {
      return channel_333X;}
    else {
      goto L19018;}}
  else {
    goto L19018;}}
 L19018: {
  return (((status_334X)<<2));}
}
long s48_enter_integer(long x_335X)
{
  char * v_336X;
 {s48_make_availableAgc(16);
  if ((536870911 < x_335X)) {
    goto L20546;}
  else {
    if ((x_335X < -536870912)) {
      goto L20546;}
    else {
      return (((x_335X)<<2));}}}
 L20546: {
  v_336X = (char *) s48_long_to_bignum(x_335X);
  return enter_bignum(v_336X);}
}
void s48_setup_external_exception(long why_337X, long nargs_338X)
{
  long n_340X;
  long code_339X;
 {  code_339X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_339X)))))<<2)), code_339X, (((why_337X)<<2)), 4);
  n_340X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_340X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((why_337X)<<2));
  if ((10 < nargs_338X)) {
    ps_error("too many arguments from external exception", 0);
    goto L21440;}
  else {
    goto L21440;}}
 L21440: {
  Sexternal_exception_nargsS = nargs_338X;
  Sexternal_exceptionPS = 1;
  return;}
}
void s48_copy_stack_into_heap(void)
{
  char * arg1K1;
  char * arg1K0;
  char * arg_344X;
  char * loc_343X;
  char * top_342X;
  long arg_count_341X;
 {s48_make_availableAgc(((((-4 & ((Sstack_endS) - (SstackS))))<<2)));
  arg_count_341X = ((((ScontS) - (SstackS)))>>2);
  top_342X = SstackS;
  if ((1 == (((long) (ScontS))))) {
    goto L21540;}
  else {really_preserve_continuation(0);
    goto L21540;}}
 L21540: {
  SstackS = (ScontS);
  arg1K0 = ((SstackS) + -4);
  arg1K1 = (top_342X + (-4 + (((arg_count_341X)<<2))));
  goto L21569;}
 L21569: {
  loc_343X = arg1K0;
  arg_344X = arg1K1;
  if ((arg_344X < top_342X)) {
    SstackS = ((SstackS) + (0 - (((arg_count_341X)<<2))));
    return;}
  else {
    *((long *) loc_343X) = (*((long *) arg_344X));
    arg1K0 = (loc_343X + -4);
    arg1K1 = (arg_344X + -4);
    goto L21569;}}
}
long s48_get_imported_binding(char *name_345X)
{
  long value_347X;
  long value_346X;
 {  value_346X = enter_stringAgc(name_345X);
  Stemp0S = value_346X;s48_make_availableAgc(20);
  value_347X = Stemp0S;
  Stemp0S = 1;
  return Hlookup2690((Sexported_bindingsS), value_347X, 0);}
}
void s48_define_exported_binding(char *name_348X, long value_349X)
{
  char * addr_354X;
  long x_353X;
  long value_352X;
  long value_351X;
  long name_350X;
 {  Stemp0S = value_349X;
  name_350X = enter_stringAgc(name_348X);
  Stemp1S = name_350X;s48_make_availableAgc(20);
  value_351X = Stemp0S;
  Stemp0S = 1;
  value_352X = Stemp1S;
  Stemp1S = 1;
  x_353X = Hlookup2671((Simported_bindingsS), value_352X, 0);
  addr_354X = (((char *) (-3 + x_353X))) + 8;S48_WRITE_BARRIER(x_353X, addr_354X, value_351X);
  *((long *) addr_354X) = value_351X;
  return;}
}
void s48_initialize_vm(char * stack_begin_355X, long stack_size_356X)
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
  long protocol_357X;
  long opcode_358X;
  long size_359X;
  char * addr_403X;
  long i_402X;
  long table_401X;
  long v_400X;
  long vector_399X;
  char * addr_398X;
  long code_397X;
  char * addr_396X;
  char * addr_395X;
  long x_394X;
  long v_393X;
  long v_392X;
  long vector_391X;
  char * addr_390X;
  long v_389X;
  long v_388X;
  long v_387X;
  long code_386X;
  char * a_385X;
  char * addr_384X;
  long value_383X;
  long size_382X;
  char * start_381X;
  char * addr_380X;
  long val_379X;
  long index_378X;
  long h_377X;
  long i_376X;
  char * stack_375X;
  long n_374X;
  long string_373X;
  long foo_372X;
  long table_371X;
  long i_370X;
  long v_369X;
  long v_368X;
  long imported_bindings_367X;
  long exported_bindings_366X;
  long n_365X;
  long symbols_364X;
  long maybe_363X;
  long maybe_362X;
  long v_361X;
  long symbol_table_360X;
 {  symbol_table_360X = s48_initial_symbols();
  if ((symbol_table_360X == 1)) {
    make_hash_tableAgc_return_tag = 0;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_0:
    v_361X = make_hash_tableAgc0_return_value;
    Sthe_symbol_tableS = v_361X;
    maybe_362X = s48_find_all(1);
    if ((maybe_362X == 1)) {s48_collect();
      maybe_363X = s48_find_all(1);
      if ((maybe_363X == 1)) {
        ps_error("insufficient heap space to build symbol table", 0);
        arg0K0 = maybe_363X;
        goto L21620;}
      else {
        arg0K0 = maybe_363X;
        goto L21620;}}
    else {
      arg0K0 = maybe_362X;
      goto L21620;}}
  else {
    Sthe_symbol_tableS = symbol_table_360X;
    goto L22259;}}
 L21620: {
  symbols_364X = arg0K0;
  n_365X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_364X))) + -4))))>>8))))>>2);
  arg0K0 = 0;
  goto L21641;}
 L22259: {
  exported_bindings_366X = s48_initial_exported_bindings();
  imported_bindings_367X = s48_initial_imported_bindings();
  if ((1 == imported_bindings_367X)) {
    make_hash_tableAgc_return_tag = 1;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_1:
    v_368X = make_hash_tableAgc0_return_value;
    Simported_bindingsS = v_368X;
    make_hash_tableAgc_return_tag = 2;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_2:
    v_369X = make_hash_tableAgc0_return_value;
    Sexported_bindingsS = v_369X;
    goto L22265;}
  else {
    Simported_bindingsS = imported_bindings_367X;
    Sexported_bindingsS = exported_bindings_366X;
    goto L22265;}}
 L21641: {
  i_370X = arg0K0;
  if ((i_370X == n_365X)) {
    goto L22259;}
  else {
    table_371X = Sthe_symbol_tableS;
    foo_372X = *((long *) ((((char *) (-3 + symbols_364X))) + (((i_370X)<<2))));
    string_373X = *((long *) (((char *) (-3 + foo_372X))));
    n_374X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_373X))) + -4))))>>8));
    arg0K0 = 0;
    arg0K1 = 0;
    goto L20940;}}
 L22265: {
  if ((stack_size_356X < 8128)) {
    stack_375X = (char *)malloc(32512);
    if ((stack_375X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_375X;
      arg0K1 = 8128;
      goto L12885;}
    else {
      arg1K0 = stack_375X;
      arg0K1 = 8128;
      goto L12885;}}
  else {
    arg1K0 = stack_begin_355X;
    arg0K1 = stack_size_356X;
    goto L12885;}}
 L20940: {
  i_376X = arg0K0;
  h_377X = arg0K1;
  if ((i_376X < n_374X)) {
    arg0K0 = (1 + i_376X);
    arg0K1 = (h_377X + (((*((unsigned char *) ((((char *) (-3 + string_373X))) + i_376X))))));
    goto L20940;}
  else {
    index_378X = 1023 & h_377X;
    val_379X = *((long *) ((((char *) (-3 + table_371X))) + (((index_378X)<<2))));
    addr_380X = (((char *) (-3 + foo_372X))) + 4;S48_WRITE_BARRIER(foo_372X, addr_380X, val_379X);
    *((long *) addr_380X) = val_379X;
    if ((3 == (3 & foo_372X))) {
      arg0K0 = (-4 & foo_372X);
      goto L20926;}
    else {
      arg0K0 = foo_372X;
      goto L20926;}}}
 L12885: {
  start_381X = arg1K0;
  size_382X = arg0K1;
  Sstack_beginS = start_381X;
  Sstack_endS = (start_381X + (((size_382X)<<2)));
  Sreal_stack_limitS = ((Sstack_beginS) + 512);
  s48_Sstack_limitS = (Sreal_stack_limitS);
  SstackS = (Sstack_endS);
  ScontS = (((char *) 1));
  arg1K0 = start_381X;
  goto L12915;}
 L20926: {
  value_383X = arg0K0;
  addr_384X = (((char *) (-3 + table_371X))) + (((index_378X)<<2));S48_WRITE_BARRIER(table_371X, addr_384X, value_383X);
  *((long *) addr_384X) = value_383X;
  arg0K0 = (1 + i_370X);
  goto L21641;}
 L12915: {
  a_385X = arg1K0;
  if ((a_385X == (Sstack_endS))) {s48_make_availableAgc(20);
    merged_arg0K0 = 71;
    merged_arg0K1 = 0;
    merged_arg0K2 = 0;
    merged_arg0K3 = 0;
    make_return_code_return_tag = 0;
    goto make_return_code;
   make_return_code_return_0:
    code_386X = make_return_code0_return_value;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 1;
    ScontS = (SstackS);
    *((long *) (ScontS)) = (((long) ((((char *) (-3 + code_386X))) + 11)));
    Sbottom_of_stackS = (ScontS);
    Sheap_continuationS = 1;s48_make_availableAgc(60);
    merged_arg0K0 = 66;
    merged_arg0K1 = 150;
    merged_arg0K2 = 65535;
    merged_arg0K3 = 0;
    make_return_code_return_tag = 1;
    goto make_return_code;
   make_return_code_return_1:
    v_387X = make_return_code0_return_value;
    Sinterrupt_return_codeS = v_387X;
    merged_arg0K0 = 1;
    merged_arg0K1 = 147;
    merged_arg0K2 = 65535;
    merged_arg0K3 = 0;
    make_return_code_return_tag = 2;
    goto make_return_code;
   make_return_code_return_2:
    v_388X = make_return_code0_return_value;
    Sexception_return_codeS = v_388X;
    merged_arg0K0 = 70;
    merged_arg0K1 = 0;
    merged_arg0K2 = 1;
    merged_arg0K3 = 0;
    make_return_code_return_tag = 3;
    goto make_return_code;
   make_return_code_return_3:
    v_389X = make_return_code0_return_value;
    Scall_with_values_return_codeS = v_389X;s48_make_availableAgc(32);s48_bignum_make_cached_constants();
    addr_390X = s48_allocate_tracedAgc(8);
    if ((addr_390X == NULL)) {
      arg0K0 = 1;
      goto L22329;}
    else {
      *((long *) addr_390X) = 1034;
      arg0K0 = (3 + (((long) (addr_390X + 4))));
      goto L22329;}}
  else {
    *((long *) a_385X) = 252645135;
    arg1K0 = (a_385X + 4);
    goto L12915;}}
 L22329: {
  vector_391X = arg0K0;
  if ((1 == vector_391X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_392X;
    goto L22316;}
  else {
    arg0K0 = vector_391X;
    goto L22316;}}
 L22316: {
  v_393X = arg0K0;
  Sempty_logS = v_393X;
  x_394X = Sempty_logS;
  addr_395X = ((char *) (-3 + x_394X));S48_WRITE_BARRIER(x_394X, addr_395X, 1);
  *((long *) addr_395X) = 1;
  return;}
 make_return_code: {
  protocol_357X = merged_arg0K0;
  opcode_358X = merged_arg0K1;
  size_359X = merged_arg0K2;{
  addr_396X = s48_allocate_small(18);
  *((long *) addr_396X) = 3658;
  code_397X = 3 + (((long) (addr_396X + 4)));
  *((unsigned char *) (((char *) (-3 + code_397X)))) = 0;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 1)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 2)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 3)) = 23;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 4)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 5)) = 8;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 6)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 7)) = 11;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 8)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 9)) = (255 & (((size_359X)>>8)));
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 10)) = (255 & size_359X);
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 11)) = 0;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 12)) = protocol_357X;
  *((unsigned char *) ((((char *) (-3 + code_397X))) + 13)) = opcode_358X;
  make_return_code0_return_value = code_397X;
  goto make_return_code_return;}
 make_return_code_return:
  switch (make_return_code_return_tag) {
  case 0: goto make_return_code_return_0;
  case 1: goto make_return_code_return_1;
  case 2: goto make_return_code_return_2;
  default: goto make_return_code_return_3;
  }}

 make_hash_tableAgc: {
{ addr_398X = s48_allocate_tracedAgc(4100);
  if ((addr_398X == NULL)) {
    arg0K0 = 1;
    goto L9478;}
  else {
    *((long *) addr_398X) = 1048586;
    arg0K0 = (3 + (((long) (addr_398X + 4))));
    goto L9478;}}
 L9478: {
  vector_399X = arg0K0;
  if ((1 == vector_399X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_400X;
    goto L9466;}
  else {
    arg0K0 = vector_399X;
    goto L9466;}}
 L9466: {
  table_401X = arg0K0;
  arg0K0 = 0;
  goto L9501;}
 L9501: {
  i_402X = arg0K0;
  if ((1024 == i_402X)) {
    make_hash_tableAgc0_return_value = table_401X;
    goto make_hash_tableAgc_return;}
  else {
    addr_403X = (((char *) (-3 + table_401X))) + (((i_402X)<<2));S48_WRITE_BARRIER(table_401X, addr_403X, 1);
    *((long *) addr_403X) = 1;
    arg0K0 = (1 + i_402X);
    goto L9501;}}
 make_hash_tableAgc_return:
  switch (make_hash_tableAgc_return_tag) {
  case 0: goto make_hash_tableAgc_return_0;
  case 1: goto make_hash_tableAgc_return_1;
  default: goto make_hash_tableAgc_return_2;
  }}

}
void s48_post_gc_cleanup(void)
{

 {  (Spost_gc_cleanupS)();
  return;}
}
void s48_gc_root(void)
{

 {  (Sgc_root_procS)();
  return;}
}
long s48_restart(long proc_404X, long nargs_405X)
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
  FILE * merged_arg4K2;
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
  int loseD0_return_tag;
  long nargs_406X;
  long status_407X;
  long cont_408X;
  long stack_arg_count_409X;
  long list_410X;
  long marker_411X;
  long a_412X;
  long b_413X;
  long stob_414X;
  long index_415X;
  long stob_416X;
  long index_417X;
  long value_418X;
  long thing_419X;
  long index_420X;
  long count_421X;
  long x_422X;
  long n_423X;
  long list_424X;
  long length_425X;
  long start_426X;
  long count_427X;
  long need_428X;
  long list_429X;
  long count_430X;
  long wants_stack_args_431X;
  long stack_arg_count_432X;
  long list_args_433X;
  long list_arg_count_434X;
  char * start_435X;
  long count_436X;
  long code_vector_437X;
  long template_438X;
  char not_firstP_439X;
  FILE * out_440X;
  char *message_441X;
  char * arg_2090X;
  char * loc_2089X;
  char * top_of_args_2088X;
  long i_2087X;
  long string_2086X;
  char * addr_2085X;
  long len_2084X;
  long len_2083X;
  long x_2082X;
  char *string_2081X;
  char * new_stack_2080X;
  char * new_cont_2079X;
  long stack_size_2078X;
  char move_slowP_2077X;
  long slow_2076X;
  long len_2075X;
  long fast_2074X;
  long v_2073X;
  char *v_2072X;
  long v_2071X;
  long v_2070X;
  long obj_2069X;
  long env_2068X;
  long thread_2067X;
  long x_2066X;
  char * addr_2065X;
  long v_2064X;
  long status_2063X;
  long channel_2062X;
  long type_2061X;
  long v_2060X;
  long next_stob_2059X;
  long i_2058X;
  long log_2057X;
  char * addr_2056X;
  long next_stob_2055X;
  long i_2054X;
  long log_2053X;
  long v_2052X;
  long v_2051X;
  long size_2050X;
  char * pointer_2049X;
  char * cont_2048X;
  long v_2047X;
  long v_2046X;
  long n_2045X;
  long extra_2044X;
  long x_size_2043X;
  char * addr_2042X;
  long x_2041X;
  char * addr_2040X;
  long a_2039X;
  long last_2038X;
  long l_2037X;
  long x_2036X;
  char * addr_2035X;
  long a_2034X;
  long value_2033X;
  long x_2032X;
  char * addr_2031X;
  long a_2030X;
  long count_2029X;
  long args_2028X;
  long value_2027X;
  char interruptP_2026X;
  long x_2025X;
  long l_2024X;
  long i_2023X;
  long list_2022X;
  long x_2021X;
  long v_2020X;
  long x_2019X;
  long v_2018X;
  long count_2017X;
  long x_2016X;
  long next_2015X;
  long i_2014X;
  long obj_2013X;
  long obj_2012X;
  long name_2011X;
  long v_2010X;
  long why_2009X;
  long size_2008X;
  char * pointer_2007X;
  char v_2006X;
  long v_2005X;
  long v_2004X;
  char v_2003X;
  long v_2002X;
  char not_firstP_2001X;
  long cont_2000X;
  long size_1999X;
  char * pointer_1998X;
  long v_1997X;
  char * pointer_1996X;
  char * code_pointer_1995X;
  long cont_1994X;
  char not_firstP_1993X;
  char * cont_1992X;
  char not_firstP_1991X;
  long template_1990X;
  FILE * out_1989X;
  long current_code_1988X;
  long val_1987X;
  long val_1986X;
  long val_1985X;
  long val_1984X;
  long val_1983X;
  long val_1982X;
  long n_1981X;
  char v_1980X;
  char * external_bignum_1979X;
  char * x_1978X;
  long val_1977X;
  long n_1976X;
  char v_1975X;
  char * external_bignum_1974X;
  char * x_1973X;
  long val_1972X;
  long n_1971X;
  char v_1970X;
  char * external_bignum_1969X;
  char * x_1968X;
  long v_1967X;
  long n_1966X;
  char v_1965X;
  long v_1964X;
  long rem_1963X;
  long quot_1962X;
  char div_by_zeroP_1961X;
  long val_1960X;
  long n_1959X;
  char v_1958X;
  char * external_bignum_1957X;
  char * x_1956X;
  long val_1955X;
  long n_1954X;
  char v_1953X;
  char * external_bignum_1952X;
  char * x_1951X;
  long val_1950X;
  long n_1949X;
  char v_1948X;
  char * external_bignum_1947X;
  char * x_1946X;
  long link_1945X;
  long v_1944X;
  long v_1943X;
  long stob_1942X;
  long v_1941X;
  long stob_1940X;
  char * addr_1939X;
  long x_1938X;
  char * addr_1937X;
  char * addr_1936X;
  long value_1935X;
  char * addr_1934X;
  long value_1933X;
  long copies_1932X;
  char * v_1931X;
  long value_1930X;
  char * y_1929X;
  char * v_1928X;
  long value_1927X;
  char * y_1926X;
  char * v_1925X;
  long value_1924X;
  char * y_1923X;
  long val_1922X;
  long v_1921X;
  long n_1920X;
  char v_1919X;
  char * rem_1918X;
  char * quot_1917X;
  char div_by_zeroP_1916X;
  char * x_1915X;
  char * v_1914X;
  long value_1913X;
  char * y_1912X;
  char * v_1911X;
  long value_1910X;
  char * y_1909X;
  char * v_1908X;
  long value_1907X;
  char * y_1906X;
  char * addr_1905X;
  long val_1904X;
  char * addr_1903X;
  long value_1902X;
  long len_1901X;
  long s2_1900X;
  long foo_1899X;
  long previous_foo_1898X;
  long value_1897X;
  long verify_1896X;
  long copies_1895X;
  long stob_1894X;
  long i_1893X;
  long x_1892X;
  long x_1891X;
  long val_1890X;
  char * addr_1889X;
  long value_1888X;
  char * v_1887X;
  long value_1886X;
  long v_1885X;
  char * v_1884X;
  long value_1883X;
  long v_1882X;
  char * v_1881X;
  long value_1880X;
  long v_1879X;
  long val_1878X;
  long val_1877X;
  long n_1876X;
  char v_1875X;
  char * external_bignum_1874X;
  char * x_1873X;
  long val_1872X;
  long n_1871X;
  char v_1870X;
  char * external_bignum_1869X;
  char * x_1868X;
  char * v_1867X;
  long value_1866X;
  char * y_1865X;
  char * v_1864X;
  long value_1863X;
  long v_1862X;
  long val_1861X;
  long v_1860X;
  char * v_1859X;
  char * v_1858X;
  long value_1857X;
  long extra1_1856X;
  long length1_1855X;
  char * v_1854X;
  long value_1853X;
  long v_1852X;
  long bucket_1851X;
  char * addr_1850X;
  long value_1849X;
  long verify_1848X;
  long log_1847X;
  long stob_1846X;
  long i_1845X;
  long x_1844X;
  long n_1843X;
  long code_1842X;
  long status_1841X;
  char pendingP_1840X;
  char eofP_1839X;
  long got_1838X;
  long y_1837X;
  long link_1836X;
  long val_1835X;
  long x_1834X;
  char * addr_1833X;
  long b_1832X;
  long val_1831X;
  long extra1_1830X;
  long length1_1829X;
  long extra1_1828X;
  long length1_1827X;
  long extra1_1826X;
  long length1_1825X;
  long n_1824X;
  char * v_1823X;
  long val_1822X;
  long n_1821X;
  char v_1820X;
  char * external_bignum_1819X;
  char * v_1818X;
  long n_1817X;
  char v_1816X;
  char * external_bignum_1815X;
  char * v_1814X;
  long value_1813X;
  char * y_1812X;
  char * v_1811X;
  long value_1810X;
  char * y_1809X;
  char * v_1808X;
  long value_1807X;
  long extra1_1806X;
  long val_1805X;
  long v_1804X;
  char * v_1803X;
  long extra1_1802X;
  long length1_1801X;
  long val_1800X;
  long v_1799X;
  char * v_1798X;
  long x_1797X;
  long val_1796X;
  long v_1795X;
  char * v_1794X;
  long extra0_1793X;
  long length0_1792X;
  long extra1_1791X;
  long length1_1790X;
  char * addr_1789X;
  long value_1788X;
  long offset_1787X;
  long i_1786X;
  long count_1785X;
  char * addr_1784X;
  long value_1783X;
  long offset_1782X;
  long i_1781X;
  long count_1780X;
  char *v_1779X;
  long link_1778X;
  long index_1777X;
  long h_1776X;
  long i_1775X;
  char * addr_1774X;
  char * addr_1773X;
  long value_1772X;
  char * addr_1771X;
  char * addr_1770X;
  char * addr_1769X;
  long value_1768X;
  char * addr_1767X;
  char * addr_1766X;
  long value_1765X;
  long proposal_1764X;
  long entry_1763X;
  long thing_1762X;
  long log_1761X;
  long copies_1760X;
  char * addr_1759X;
  long x_1758X;
  long val_1757X;
  long n_1756X;
  long code_1755X;
  long status_1754X;
  long v_1753X;
  long len_1752X;
  long s2_1751X;
  long foo_1750X;
  long val_1749X;
  long v_1748X;
  long n_1747X;
  char v_1746X;
  char * external_bignum_1745X;
  char * x_1744X;
  long extra0_1743X;
  long length0_1742X;
  long extra0_1741X;
  long length0_1740X;
  long extra0_1739X;
  long length0_1738X;
  char * v_1737X;
  long value_1736X;
  long v_1735X;
  char * v_1734X;
  long value_1733X;
  long v_1732X;
  char * v_1731X;
  long v_1730X;
  char * x_1729X;
  char * v_1728X;
  long n_1727X;
  char * v_1726X;
  long value_1725X;
  long extra1_1724X;
  char * v_1723X;
  long value_1722X;
  long extra1_1721X;
  long val_1720X;
  long val_1719X;
  long val_1718X;
  long val_1717X;
  long val_1716X;
  long extra0_1715X;
  long length0_1714X;
  long val_1713X;
  long v_1712X;
  char * v_1711X;
  long x_1710X;
  long val_1709X;
  long v_1708X;
  char * v_1707X;
  long extra0_1706X;
  long length0_1705X;
  long y_1704X;
  long x_1703X;
  long extra0_1702X;
  long length0_1701X;
  long x_1700X;
  long n_1699X;
  long code_1698X;
  long bytes_used_1697X;
  long count_1696X;
  long index_1695X;
  long env_1694X;
  long offset_1693X;
  long i_1692X;
  long bytes_used_1691X;
  long count_1690X;
  long env_1689X;
  long offset_1688X;
  long i_1687X;
  long n_1686X;
  long x_1685X;
  long obj_1684X;
  long n_1683X;
  long code_1682X;
  long n_1681X;
  long code_1680X;
  long n_1679X;
  long code_1678X;
  long n_1677X;
  long code_1676X;
  long i_1675X;
  long l_1674X;
  long n_1673X;
  long table_1672X;
  long val_1671X;
  long n_1670X;
  long code_1669X;
  char x_1668X;
  char minutesP_1667X;
  long v_1666X;
  long vector_1665X;
  char * addr_1664X;
  long x_1663X;
  long value_1662X;
  long verify_1661X;
  long copies_1660X;
  long stob_1659X;
  long i_1658X;
  char * addr_1657X;
  long v_1656X;
  char * addr_1655X;
  char * addr_1654X;
  long val_1653X;
  long x_1652X;
  long n_1651X;
  long code_1650X;
  long status_1649X;
  char pendingP_1648X;
  long got_1647X;
  long y_1646X;
  char v_1645X;
  long count_1644X;
  long start_1643X;
  char waitP_1642X;
  long x_1641X;
  long n_1640X;
  long code_1639X;
  long status_1638X;
  long channel_1637X;
  long v_1636X;
  long v_1635X;
  long v_1634X;
  long reason_1633X;
  long channel_1632X;
  long index_1631X;
  long bucket_1630X;
  long i_1629X;
  long i_1628X;
  char * addr_1627X;
  long i_1626X;
  long rest_list_1625X;
  long i_1624X;
  long v_1623X;
  long n_1622X;
  char v_1621X;
  char * external_bignum_1620X;
  char * x_1619X;
  long val_1618X;
  long v_1617X;
  char * v_1616X;
  char * v_1615X;
  long value_1614X;
  long needed_1613X;
  long y_1612X;
  long y_1611X;
  long x_1610X;
  long val_1609X;
  long v_1608X;
  char * v_1607X;
  long val_1606X;
  long v_1605X;
  char * v_1604X;
  long extra_1603X;
  long length_1602X;
  long count_1601X;
  long x_1600X;
  long extra_1599X;
  long length_1598X;
  char * v_1597X;
  long value_1596X;
  long extra_1595X;
  long length_1594X;
  long x_1593X;
  long c_1592X;
  long b_1591X;
  long extra0_1590X;
  long length0_1589X;
  long c_1588X;
  long b_1587X;
  long extra0_1586X;
  long length0_1585X;
  long n_1584X;
  long code_1583X;
  long val_1582X;
  long v_1581X;
  long v_1580X;
  long v_1579X;
  long n_1578X;
  long code_1577X;
  long val_1576X;
  long v_1575X;
  long v_1574X;
  long v_1573X;
  long n_1572X;
  long code_1571X;
  long val_1570X;
  long v_1569X;
  long v_1568X;
  long v_1567X;
  long n_1566X;
  long code_1565X;
  long val_1564X;
  long v_1563X;
  long v_1562X;
  long v_1561X;
  long n_1560X;
  long code_1559X;
  long val_1558X;
  char b_1557X;
  long result_1556X;
  char x_1555X;
  long c_1554X;
  long b_1553X;
  long n_1552X;
  long code_1551X;
  long c_1550X;
  long mid_c_1549X;
  long v_1548X;
  long v_1547X;
  long lo_c_1546X;
  long hi_b_1545X;
  long hi_a_1544X;
  long lo_b_1543X;
  long lo_a_1542X;
  long b_1541X;
  long n_1540X;
  long code_1539X;
  long n_1538X;
  long code_1537X;
  long x_1536X;
  long v_1535X;
  long cont_1534X;
  char * addr_1533X;
  long value_1532X;
  long offset_1531X;
  long i_1530X;
  long count_1529X;
  char * addr_1528X;
  long value_1527X;
  long offset_1526X;
  long i_1525X;
  long count_1524X;
  char x_1523X;
  long channel_1522X;
  long handlers_1521X;
  long arg_count_1520X;
  char * arg_1519X;
  char * loc_1518X;
  long x_1517X;
  long l_1516X;
  long stack_nargs_1515X;
  long x_1514X;
  long v_1513X;
  long v_1512X;
  long v_1511X;
  long v_1510X;
  long obj_1509X;
  long type_1508X;
  long thing_1507X;
  long stuff_1506X;
  char * addr_1505X;
  long val_1504X;
  long n_1503X;
  long code_1502X;
  long i_1501X;
  long n_1500X;
  long code_1499X;
  long b_1498X;
  long port_1497X;
  long Kchar_1496X;
  long n_1495X;
  long code_1494X;
  long i_1493X;
  long p_1492X;
  long n_1491X;
  long code_1490X;
  long b_1489X;
  long port_1488X;
  char * addr_1487X;
  long val_1486X;
  long n_1485X;
  long code_1484X;
  long i_1483X;
  long p_1482X;
  long n_1481X;
  long code_1480X;
  long b_1479X;
  long port_1478X;
  long n_1477X;
  long code_1476X;
  char * addr_1475X;
  long n_1474X;
  long code_1473X;
  long n_1472X;
  long code_1471X;
  long value_1470X;
  long n_1469X;
  long code_1468X;
  long list_1467X;
  long head_1466X;
  char move_slowP_1465X;
  long slow_1464X;
  long list_1463X;
  long n_1462X;
  long code_1461X;
  long string_1460X;
  char * addr_1459X;
  long len_1458X;
  long x_1457X;
  long n_1456X;
  long code_1455X;
  long val_1454X;
  long n_1453X;
  long code_1452X;
  long mseconds_1451X;
  long seconds_1450X;
  long option_1449X;
  long x_1448X;
  long n_1447X;
  long code_1446X;
  long x_1445X;
  long n_1444X;
  long code_1443X;
  long v_1442X;
  long v_1441X;
  long x_1440X;
  long result_1439X;
  char * args_1438X;
  long name_1437X;
  long proc_1436X;
  long rest_list_1435X;
  long x_1434X;
  long n_1433X;
  long code_1432X;
  long x_1431X;
  long v_1430X;
  long code_1429X;
  long pc_1428X;
  char * addr_1427X;
  long x_1426X;
  long proposal_1425X;
  long x_1424X;
  long n_1423X;
  long code_1422X;
  long x_1421X;
  long n_1420X;
  long code_1419X;
  long n_1418X;
  long code_1417X;
  long value_1416X;
  long vector_1415X;
  long type_1414X;
  char firstP_1413X;
  long n_1412X;
  long code_1411X;
  long vector_1410X;
  char firstP_1409X;
  long n_1408X;
  long code_1407X;
  long x_1406X;
  long n_1405X;
  long code_1404X;
  long h_1403X;
  long i_1402X;
  long x_1401X;
  long n_1400X;
  long code_1399X;
  long x_1398X;
  long n_1397X;
  long code_1396X;
  long status_1395X;
  long reason_1394X;
  long x_1393X;
  long n_1392X;
  long code_1391X;
  long n_1390X;
  long code_1389X;
  char * addr_1388X;
  long next_stob_1387X;
  long i_1386X;
  long n_1385X;
  long code_1384X;
  long x_1383X;
  long n_1382X;
  long code_1381X;
  long v_1380X;
  long next_stob_1379X;
  long i_1378X;
  long value_1377X;
  long n_1376X;
  long code_1375X;
  char * addr_1374X;
  long count_1373X;
  long to_index_1372X;
  long from_index_1371X;
  long copies_1370X;
  long left_1369X;
  long n_1368X;
  long code_1367X;
  long n_1366X;
  long code_1365X;
  long value_1364X;
  long value_1363X;
  long verify_1362X;
  long log_1361X;
  long stob_1360X;
  long i_1359X;
  long n_1358X;
  long code_1357X;
  char * addr_1356X;
  long old_1355X;
  long x_1354X;
  char * addr_1353X;
  long channel_1352X;
  long res_1351X;
  long i_1350X;
  long x_1349X;
  long n_1348X;
  long code_1347X;
  long y_1346X;
  long n_1345X;
  char * addr_1344X;
  long prev_1343X;
  long ch_1342X;
  long val_1341X;
  long x_1340X;
  long n_1339X;
  long code_1338X;
  long val_1337X;
  long x_1336X;
  long n_1335X;
  long code_1334X;
  long n_1333X;
  long code_1332X;
  long x_1331X;
  long n_1330X;
  long code_1329X;
  long x_1328X;
  long n_1327X;
  long code_1326X;
  long n_1325X;
  long code_1324X;
  long reason_1323X;
  long v_1322X;
  long v_1321X;
  long val_1320X;
  long x_1319X;
  long n_1318X;
  long code_1317X;
  char * addr_1316X;
  char * addr_1315X;
  long x_1314X;
  long n_1313X;
  long code_1312X;
  long val_1311X;
  long x_1310X;
  long n_1309X;
  long code_1308X;
  long link_1307X;
  long index_1306X;
  long h_1305X;
  long i_1304X;
  long x_1303X;
  long n_1302X;
  long code_1301X;
  long n_1300X;
  long code_1299X;
  long n_1298X;
  long code_1297X;
  long x_1296X;
  long n_1295X;
  long code_1294X;
  long n_1293X;
  long code_1292X;
  long x_1291X;
  long n_1290X;
  long code_1289X;
  long x_1288X;
  long n_1287X;
  long code_1286X;
  long n_1285X;
  long code_1284X;
  long vector_1283X;
  long n_1282X;
  long code_1281X;
  long x_1280X;
  long n_1279X;
  long code_1278X;
  long n_1277X;
  long code_1276X;
  long n_1275X;
  long code_1274X;
  long x_1273X;
  long n_1272X;
  long code_1271X;
  long n_1270X;
  long code_1269X;
  long x_1268X;
  long n_1267X;
  long code_1266X;
  long n_1265X;
  long code_1264X;
  long vector_1263X;
  long n_1262X;
  long code_1261X;
  long n_1260X;
  long code_1259X;
  char * addr_1258X;
  long n_1257X;
  long code_1256X;
  long n_1255X;
  long code_1254X;
  long value_1253X;
  long n_1252X;
  long code_1251X;
  long n_1250X;
  long code_1249X;
  long value_1248X;
  long x_1247X;
  long n_1246X;
  long code_1245X;
  long n_1244X;
  long code_1243X;
  char * addr_1242X;
  long n_1241X;
  long code_1240X;
  long value_1239X;
  long i_1238X;
  long value_1237X;
  long i_1236X;
  long n_1235X;
  long code_1234X;
  long value_1233X;
  long val_1232X;
  long n_1231X;
  long code_1230X;
  long x_1229X;
  long n_1228X;
  long code_1227X;
  long val_1226X;
  long x_1225X;
  long n_1224X;
  long code_1223X;
  long val_1222X;
  long val_1221X;
  char * v_1220X;
  long value_1219X;
  long needed_1218X;
  long y_1217X;
  long x_1216X;
  long result_1215X;
  long x_1214X;
  long x_1213X;
  long x_1212X;
  long count_1211X;
  long value_1210X;
  long n_1209X;
  long code_1208X;
  long val_1207X;
  long val_1206X;
  long n_1205X;
  long code_1204X;
  long n_1203X;
  long code_1202X;
  long n_1201X;
  long code_1200X;
  long n_1199X;
  long code_1198X;
  long x_1197X;
  long n_1196X;
  long code_1195X;
  long x_1194X;
  long n_1193X;
  long code_1192X;
  long x_1191X;
  long n_1190X;
  long code_1189X;
  long n_1188X;
  long code_1187X;
  long n_1186X;
  long code_1185X;
  long n_1184X;
  long code_1183X;
  long n_1182X;
  long code_1181X;
  long n_1180X;
  long a_1179X;
  long y_1178X;
  long x_1177X;
  long a_1176X;
  long y_1175X;
  long x_1174X;
  long val_1173X;
  long val_1172X;
  char b_1171X;
  long val_1170X;
  char b_1169X;
  long val_1168X;
  char b_1167X;
  long val_1166X;
  long Kdouble_1165X;
  char * addr_1164X;
  double value_1163X;
  long value_1162X;
  double y_1161X;
  long value_1160X;
  long a_1159X;
  long n_1158X;
  long code_1157X;
  long y_1156X;
  long x_1155X;
  long Kdouble_1154X;
  char * addr_1153X;
  double x_1152X;
  long value_1151X;
  double y_1150X;
  long value_1149X;
  long val_1148X;
  long v_1147X;
  char * v_1146X;
  long Kdouble_1145X;
  char * addr_1144X;
  double x_1143X;
  long value_1142X;
  double y_1141X;
  long value_1140X;
  long a_1139X;
  long Kdouble_1138X;
  char * addr_1137X;
  double x_1136X;
  long value_1135X;
  double y_1134X;
  long value_1133X;
  long val_1132X;
  long v_1131X;
  char * v_1130X;
  long n_1129X;
  long code_1128X;
  long x_1127X;
  long n_1126X;
  long code_1125X;
  long val_1124X;
  long val_1123X;
  long delta_1122X;
  long delta_1121X;
  long offset_1120X;
  long index_1119X;
  long v_1118X;
  long x_1117X;
  long x_1116X;
  long x_1115X;
  long x_1114X;
  char * addr_1113X;
  long a_1112X;
  long wants_stack_args_1111X;
  long size_1110X;
  char * pointer_1109X;
  char * cont_1108X;
  long proc_1107X;
  long offset_1106X;
  long cont_1105X;
  long protocol_1104X;
  char * code_pointer_1103X;
  long obj_1102X;
  char * addr_1101X;
  long list_args_1100X;
  long follower_1099X;
  long list_1098X;
  long n_1097X;
  long code_1096X;
  long proc_1095X;
  long args_1094X;
  long list_arg_count_1093X;
  char okayP_1092X;
  long stack_nargs_1091X;
  long list_args_1090X;
  long obj_1089X;
  long v_1088X;
  long proc_1087X;
  long protocol_1086X;
  long code_1085X;
  long template_1084X;
  long obj_1083X;
  long stack_arg_count_1082X;
  long value_1081X;
  char * addr_1080X;
  long value_1079X;
  long offset_1078X;
  long i_1077X;
  long count_1076X;
  long total_count_1075X;
  long offset_1074X;
  long i_1073X;
  long new_env_1072X;
  char * addr_1071X;
  long value_1070X;
  long offset_1069X;
  long i_1068X;
  long count_1067X;
  long total_count_1066X;
  long offset_1065X;
  long i_1064X;
  long new_env_1063X;
  long n_1062X;
  long n_1061X;
  long x_1060X;
  long x_1059X;
  char * addr_1058X;
  long next_1057X;
  long channel_1056X;
  long n_1055X;
  long x_1054X;
  long n_1053X;
  long x_1052X;
  long m_1051X;
  long i_1050X;
  char * arg_top_1049X;
  long list_arg_count_1048X;
  long list_args_1047X;
  long stack_nargs_1046X;
  long bytes_used_1045X;
  long count_1044X;
  long v_1043X;
  char * arg_1042X;
  char * loc_1041X;
  long obj_1040X;
  long obj_1039X;
  long list_arg_count_1038X;
  long list_args_1037X;
  long stack_arg_count_1036X;
  long v_1035X;
  long v_1034X;
  long size_1033X;
  char * pointer_1032X;
  char * cont_1031X;
  long protocol_skip_1030X;
  long v_1029X;
  long bytes_used_1028X;
  long n_1027X;
  long code_1026X;
  long args_1025X;
  long list_args_1024X;
  long stack_nargs_1023X;
  long v_1022X;
  FILE * out_1021X;
  long x_1020X;
  long v_1019X;
  long v_1018X;
  long v_1017X;
  long v_1016X;
  long v_1015X;
  long v_1014X;
  long v_1013X;
  long v_1012X;
  long x_1011X;
  long n_1010X;
  long code_1009X;
  long v_1008X;
  long len_1007X;
  long value_1006X;
  long index_1005X;
  long arg4_1004X;
  long arg3_1003X;
  long arg2_1002X;
  long x_1001X;
  long n_1000X;
  long code_999X;
  long v_998X;
  long v_997X;
  long len_996X;
  long index_995X;
  long arg3_994X;
  long arg2_993X;
  long n_992X;
  long code_991X;
  long list_990X;
  long arg2_989X;
  long x_988X;
  long n_987X;
  long code_986X;
  long n_985X;
  long arg2_984X;
  long len_983X;
  long x_982X;
  long obj_981X;
  long arg2_980X;
  long x_979X;
  long arg2_978X;
  long x_977X;
  long n_976X;
  long code_975X;
  long n_974X;
  long code_973X;
  long n_972X;
  long code_971X;
  long status_970X;
  long value_969X;
  long key_968X;
  long arg2_967X;
  long x_966X;
  long n_965X;
  long code_964X;
  long n_963X;
  long code_962X;
  long mseconds_961X;
  long seconds_960X;
  long mseconds_959X;
  long seconds_958X;
  long x_957X;
  long other_956X;
  long option_955X;
  long arg2_954X;
  long x_953X;
  long arg2_952X;
  long x_951X;
  long arg2_950X;
  long x_949X;
  long n_948X;
  long code_947X;
  long rest_list_946X;
  long p_945X;
  long nargs_944X;
  long p_943X;
  long x_942X;
  long arg2_941X;
  long x_940X;
  long n_939X;
  long code_938X;
  long p_937X;
  long p_936X;
  long x_935X;
  long p_934X;
  long old_933X;
  long temp_932X;
  long obj_931X;
  long n_930X;
  long code_929X;
  long opcode_928X;
  long pc_927X;
  long size_926X;
  long exception_925X;
  long code_924X;
  long data_923X;
  long temp_922X;
  long obj_921X;
  long val_920X;
  long type_919X;
  long x_918X;
  long n_917X;
  long code_916X;
  long type_915X;
  long x_914X;
  long n_913X;
  long code_912X;
  long n_911X;
  long code_910X;
  long n_909X;
  long code_908X;
  long x_907X;
  long bytes_906X;
  long x_905X;
  long other_904X;
  long key_903X;
  long arg2_902X;
  long x_901X;
  char * addr_900X;
  long b_899X;
  long x_898X;
  char * addr_897X;
  long proc_896X;
  long arg2_895X;
  long n_894X;
  long x_893X;
  long obj_892X;
  long status_891X;
  long status_890X;
  long status_889X;
  long status_888X;
  long status_887X;
  long status_886X;
  FILE * port_885X;
  long undumpables_884X;
  long obj_883X;
  long arg4_882X;
  long arg3_881X;
  long arg2_880X;
  long n_879X;
  long code_878X;
  long arg_877X;
  long log_876X;
  long index_875X;
  long x_874X;
  long len_873X;
  long byte_872X;
  long index_871X;
  long arg3_870X;
  long arg2_869X;
  long log_868X;
  long index_867X;
  long x_866X;
  long len_865X;
  long index_864X;
  long arg2_863X;
  long x_862X;
  long n_861X;
  long code_860X;
  char v_859X;
  char v_858X;
  long v_857X;
  long count_856X;
  long to_index_855X;
  long from_index_854X;
  long arg5_853X;
  long arg4_852X;
  long arg3_851X;
  long arg2_850X;
  long v_849X;
  long x_848X;
  long type_847X;
  long offset_846X;
  long stob_845X;
  long log_844X;
  long n_843X;
  long code_842X;
  long proposal_841X;
  long proposal_840X;
  char * addr_839X;
  long next_838X;
  long channel_837X;
  long n_836X;
  char * addr_835X;
  long head_834X;
  long channel_833X;
  long obj_832X;
  long x_831X;
  long n_830X;
  long code_829X;
  long status_828X;
  char readyP_827X;
  long n_826X;
  long code_825X;
  long channel_824X;
  long obj_823X;
  char v_822X;
  long count_821X;
  long start_820X;
  long arg4_819X;
  long arg3_818X;
  long arg2_817X;
  long x_816X;
  long arg5_815X;
  long arg4_814X;
  long arg3_813X;
  long arg2_812X;
  long x_811X;
  long n_810X;
  long code_809X;
  long status_808X;
  long n_807X;
  long code_806X;
  long channel_805X;
  long obj_804X;
  long x_803X;
  long n_802X;
  long code_801X;
  long mode_800X;
  long arg2_799X;
  long x_798X;
  long x_797X;
  long x_796X;
  long arg2_795X;
  long descriptor_794X;
  long x_793X;
  long obj_792X;
  long n_791X;
  long string_790X;
  long table_789X;
  long obj_788X;
  long len_787X;
  char Kchar_786X;
  long index_785X;
  long arg3_784X;
  long arg2_783X;
  long len_782X;
  long index_781X;
  long arg2_780X;
  long obj_779X;
  char * addr_778X;
  long len_777X;
  char init_776X;
  long len_775X;
  long arg2_774X;
  long len_773X;
  long Kchar_772X;
  long index_771X;
  long arg3_770X;
  long arg2_769X;
  long len_768X;
  long index_767X;
  long arg2_766X;
  long obj_765X;
  long x_764X;
  long n_763X;
  long code_762X;
  char * addr_761X;
  long init_760X;
  long len_759X;
  long arg2_758X;
  long v_757X;
  long index_756X;
  long len_755X;
  long type_754X;
  long value_753X;
  long arg3_752X;
  long arg2_751X;
  long v_750X;
  long v_749X;
  long index_748X;
  long len_747X;
  long type_746X;
  long index_745X;
  long arg2_744X;
  long n_743X;
  long code_742X;
  char * addr_741X;
  long len_740X;
  long len_739X;
  long type_738X;
  long init_737X;
  long arg2_736X;
  long v_735X;
  long type_734X;
  long offset_733X;
  long value_732X;
  long arg2_731X;
  long type_730X;
  long offset_729X;
  long stob_728X;
  long rest_list_727X;
  long stack_nargs_726X;
  long p_725X;
  long new_724X;
  char * addr_723X;
  long len_722X;
  long type_721X;
  long len_720X;
  long p_719X;
  long new_718X;
  char * addr_717X;
  long len_716X;
  long type_715X;
  long len_714X;
  long type_713X;
  long stob_712X;
  long type_711X;
  long x_710X;
  long x_709X;
  long x_708X;
  long n_707X;
  long code_706X;
  long x_705X;
  long x_704X;
  long n_703X;
  long code_702X;
  long x_701X;
  long arg2_700X;
  long x_699X;
  long arg2_698X;
  long x_697X;
  long v_696X;
  long n_695X;
  long code_694X;
  long v_693X;
  long y_692X;
  long arg2_691X;
  long y_690X;
  long arg2_689X;
  long y_688X;
  long arg2_687X;
  long y_686X;
  long arg2_685X;
  long x_684X;
  long x_683X;
  long x_682X;
  long arg2_681X;
  long arg2_680X;
  long arg2_679X;
  long x_678X;
  long v_677X;
  long x_676X;
  long n_675X;
  long n_674X;
  long n_673X;
  long n_672X;
  long n_671X;
  long a_670X;
  long y_669X;
  long arg2_668X;
  long b_667X;
  long a_666X;
  long y_665X;
  long arg2_664X;
  long y_663X;
  long arg2_662X;
  long y_661X;
  long arg2_660X;
  long y_659X;
  long arg2_658X;
  long y_657X;
  long arg2_656X;
  long y_655X;
  long arg2_654X;
  long b_653X;
  long a_652X;
  long y_651X;
  long arg2_650X;
  long x_649X;
  long y_648X;
  long arg2_647X;
  long b_646X;
  long a_645X;
  long y_644X;
  long arg2_643X;
  long x_642X;
  long y_641X;
  long arg2_640X;
  long n_639X;
  long n_638X;
  long n_637X;
  long n_636X;
  long n_635X;
  long x_634X;
  long x_633X;
  long arg2_632X;
  long rest_list_631X;
  long x_630X;
  long rest_list_629X;
  long stack_nargs_628X;
  long arg0_627X;
  long arg1_626X;
  long rest_list_625X;
  long arg0_624X;
  long stack_nargs_623X;
  long x_622X;
  long n_621X;
  long code_620X;
  long index_619X;
  long val_618X;
  long max_617X;
  long p_616X;
  char * code_pointer_615X;
  long return_pointer_offset_614X;
  long nargs_613X;
  long code_612X;
  long template_611X;
  long rest_list_610X;
  long stack_nargs_609X;
  long p_608X;
  long p_607X;
  long cont_606X;
  long v_605X;
  long rest_list_604X;
  long proc_603X;
  long stack_nargs_602X;
  long p_601X;
  long p_600X;
  long x_599X;
  long n_598X;
  long code_597X;
  long args_596X;
  char * code_pointer_595X;
  long return_pointer_offset_594X;
  long stack_nargs_593X;
  long length_592X;
  char okayP_591X;
  long list_args_590X;
  char * code_pointer_589X;
  long return_pointer_offset_588X;
  long stack_arg_count_587X;
  char * code_pointer_586X;
  long stack_arg_count_585X;
  long v_584X;
  long x_583X;
  long x_582X;
  long x_581X;
  long x_580X;
  long value_579X;
  long x_578X;
  long template_577X;
  long new_env_576X;
  char * addr_575X;
  long len_574X;
  long closures_573X;
  long total_count_572X;
  long template_571X;
  long new_env_570X;
  char * addr_569X;
  long len_568X;
  long closures_567X;
  long total_count_566X;
  char * addr_565X;
  long val_564X;
  long x_563X;
  long n_562X;
  long code_561X;
  long location_560X;
  long index_559X;
  long template_558X;
  long n_557X;
  long code_556X;
  long location_555X;
  long index_554X;
  long template_553X;
  long x_552X;
  long x_551X;
  long n_550X;
  long n_549X;
  long code_548X;
  char * code_pointer_547X;
  long v_546X;
  long code_545X;
  long n_544X;
  char * addr_543X;
  long x_542X;
  long x_541X;
  long pc_540X;
  long code_539X;
  long x_538X;
  long v_537X;
  long v_536X;
  long v_535X;
  long n_534X;
  char * arg_top_533X;
  long count_532X;
  long size_531X;
  char * pointer_530X;
  char * cont_529X;
  long offset_528X;
  long cont_527X;
  long v_526X;
  long v_525X;
  long protocol_524X;
  char * code_pointer_523X;
  long list_arg_count_522X;
  long list_args_521X;
  long stack_nargs_520X;
  long v_519X;
  long proc_518X;
  long v_517X;
  char v_516X;
  char v_515X;
  char v_514X;
  long v_513X;
  long n_512X;
  long code_511X;
  long size_510X;
  char * pointer_509X;
  char * cont_508X;
  long code_507X;
  long n_506X;
  char * addr_505X;
  long x_504X;
  long x_503X;
  char v_502X;
  long obj_501X;
  long stack_arg_count_500X;
  long tag_499X;
  long final_stack_arg_count_498X;
  char interruptP_497X;
  long obj_496X;
  long x_495X;
  long x_494X;
  long envUtemplate_493X;
  long needed_stack_space_492X;
  long template_491X;
  long used_490X;
  long code_489X;
  long tag_488X;
  long skip_487X;
  long x_486X;
  long x_485X;
  long envUtemplate_484X;
  long template_483X;
  long used_482X;
  long envUtemp_offset_481X;
  long code_480X;
  long handlers_479X;
  long opcode_478X;
  long nargs_477X;
  long v_476X;
  long proc_475X;
  long v_474X;
  long proc_473X;
  long index_472X;
  long length_471X;
  long v_470X;
  long proc_469X;
  long v_468X;
  long proc_467X;
  long v_466X;
  long proc_465X;
  long wants_stack_args_464X;
  long skip_463X;
  char nativeP_462X;
  long stack_space_461X;
  long protocol_460X;
  long v_459X;
  long x_458X;
  long n_457X;
  long code_456X;
  long args_455X;
  long v_454X;
  long v_453X;
  long v_452X;
  long list_arg_count_451X;
  long list_args_450X;
  long stack_arg_count_449X;
  long exception_448X;
  long total_arg_count_447X;
  long code_446X;
  long list_arg_count_445X;
  long list_args_444X;
  long stack_arg_count_443X;
  long obj_442X;
 {  if ((3 == (3 & proc_404X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_404X))) + -4))))>>2))))) {
      SvalS = proc_404X;
      obj_442X = SvalS;
      if ((3 == (3 & obj_442X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_442X))) + -4))))>>2))))) {
          arg0K0 = nargs_405X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L26761;}
        else {
          arg0K0 = 3;
          arg0K1 = nargs_405X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L23361;}}
      else {
        arg0K0 = 3;
        arg0K1 = nargs_405X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L23361;}}
    else {
      goto L24307;}}
  else {
    goto L24307;}}
 L26761: {
  stack_arg_count_443X = arg0K0;
  list_args_444X = arg0K1;
  list_arg_count_445X = arg0K2;
  code_446X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  total_arg_count_447X = stack_arg_count_443X + list_arg_count_445X;
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_446X))) + 1)));
  arg0K1 = 64;
  arg2K2 = 0;
  goto L26777;}
 L23361: {
  exception_448X = arg0K0;
  stack_arg_count_449X = arg0K1;
  list_args_450X = arg0K2;
  list_arg_count_451X = arg0K3;
  if (((Scode_pointerS) == (((char *) 0)))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      return v_452X;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      return v_453X;}}
  else {
    merged_arg0K0 = list_args_450X;
    merged_arg0K1 = list_arg_count_451X;
    copy_listSAgc_return_tag = 0;
    goto copy_listSAgc;
   copy_listSAgc_return_0:
    v_454X = copy_listSAgc0_return_value;
    merged_arg0K0 = v_454X;
    merged_arg0K1 = stack_arg_count_449X;
    pop_args_GlistSAgc_return_tag = 0;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_0:
    args_455X = pop_args_GlistSAgc0_return_value;
    code_456X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_456X)))))<<2)), code_456X, (((exception_448X)<<2)), 0);
    n_457X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_457X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((exception_448X)<<2));
    x_458X = SvalS;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_458X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_455X;
    arg0K0 = 2;
    goto L22895;}}
 L24307: {
  ps_error("s48-restart called with non-procedure", 1, proc_404X);
  return v_459X;}
 L26777: {
  protocol_460X = arg0K0;
  stack_space_461X = arg0K1;
  nativeP_462X = arg2K2;
  if ((69 == protocol_460X)) {
    if ((total_arg_count_447X < 3)) {
      skip_463X = *((unsigned char *) ((((char *) (-3 + code_446X))) + (3 + total_arg_count_447X)));
      if ((0 == skip_463X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_443X;
        arg0K2 = list_args_444X;
        arg0K3 = list_arg_count_445X;
        goto L23361;}
      else {
        merged_arg0K0 = list_args_444X;
        merged_arg0K1 = list_arg_count_445X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = code_446X;
        arg0K1 = 6;
        arg0K2 = skip_463X;
        arg0K3 = (*((long *) (((char *) (-3 + (SvalS))))));
        goto L22456;}}
    else {
      if ((0 == (*((unsigned char *) ((((char *) (-3 + code_446X))) + 2))))) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_443X;
        arg0K2 = list_args_444X;
        arg0K3 = list_arg_count_445X;
        goto L23361;}
      else {
        arg0K0 = 6;
        goto L26816;}}}
  else {
    if ((63 < protocol_460X)) {
      if ((65 == protocol_460X)) {
        wants_stack_args_464X = ((((*((unsigned char *) ((((char *) (-3 + code_446X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_446X))) + 3)));
        if ((total_arg_count_447X < wants_stack_args_464X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_443X;
          arg0K2 = list_args_444X;
          arg0K3 = list_arg_count_445X;
          goto L23361;}
        else {
          merged_arg0K0 = wants_stack_args_464X;
          merged_arg0K1 = stack_arg_count_443X;
          merged_arg0K2 = list_args_444X;
          merged_arg0K3 = list_arg_count_445X;
          rest_list_setupAgc_return_tag = 0;
          goto rest_list_setupAgc;
         rest_list_setupAgc_return_0:
          if (nativeP_462X) {
            proc_465X = SvalS;
            merged_arg0K0 = (1 + wants_stack_args_464X);
            move_args_above_contB_return_tag = 0;
            goto move_args_above_contB;
           move_args_above_contB_return_0:
            v_466X = s48_call_native_procedure(proc_465X, 4);
            arg0K0 = v_466X;
            goto L24218;}
          else {
            arg0K0 = code_446X;
            arg0K1 = 4;
            arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
            arg0K3 = stack_space_461X;
            goto L25568;}}}
      else {
        if ((68 == protocol_460X)) {
          if ((total_arg_count_447X < (*((unsigned char *) ((((char *) (-3 + code_446X))) + 2))))) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_443X;
            arg0K2 = list_args_444X;
            arg0K3 = list_arg_count_445X;
            goto L23361;}
          else {
            arg0K0 = 3;
            goto L26816;}}
        else {
          if ((127 < protocol_460X)) {
            arg0K0 = (127 & protocol_460X);
            arg0K1 = stack_space_461X;
            arg2K2 = 1;
            goto L26777;}
          else {
            if ((64 == protocol_460X)) {
              if (((((((*((unsigned char *) ((((char *) (-3 + code_446X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_446X))) + 3)))) == total_arg_count_447X)) {
                if ((0 == list_arg_count_445X)) {
                  if (nativeP_462X) {
                    proc_467X = SvalS;
                    merged_arg0K0 = total_arg_count_447X;
                    move_args_above_contB_return_tag = 1;
                    goto move_args_above_contB;
                   move_args_above_contB_return_1:
                    v_468X = s48_call_native_procedure(proc_467X, 4);
                    arg0K0 = v_468X;
                    goto L24218;}
                  else {
                    arg0K0 = code_446X;
                    arg0K1 = 4;
                    arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
                    arg0K3 = stack_space_461X;
                    goto L25568;}}
                else {
                  merged_arg0K0 = list_args_444X;
                  merged_arg0K1 = list_arg_count_445X;
                  push_list_return_tag = 1;
                  goto push_list;
                 push_list_return_1:
                  if (nativeP_462X) {
                    proc_469X = SvalS;
                    merged_arg0K0 = total_arg_count_447X;
                    move_args_above_contB_return_tag = 2;
                    goto move_args_above_contB;
                   move_args_above_contB_return_2:
                    v_470X = s48_call_native_procedure(proc_469X, 4);
                    arg0K0 = v_470X;
                    goto L24218;}
                  else {
                    arg0K0 = code_446X;
                    arg0K1 = 4;
                    arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
                    arg0K3 = stack_space_461X;
                    goto L25568;}}}
              else {
                arg0K0 = 4;
                arg0K1 = stack_arg_count_443X;
                arg0K2 = list_args_444X;
                arg0K3 = list_arg_count_445X;
                goto L23361;}}
            else {
              if ((67 == protocol_460X)) {
                length_471X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_446X))) + -4))))>>8);
                index_472X = -2 + length_471X;
                arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_446X))) + (-3 + length_471X))));
                arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_446X))) + index_472X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_446X))) + (1 + index_472X)))));
                arg2K2 = nativeP_462X;
                goto L26777;}
              else {
                ps_error("unknown protocol", 1, protocol_460X);
                arg0K0 = 4;
                arg0K1 = stack_arg_count_443X;
                arg0K2 = list_args_444X;
                arg0K3 = list_arg_count_445X;
                goto L23361;}}}}}}
    else {
      if ((protocol_460X == total_arg_count_447X)) {
        if ((0 == list_arg_count_445X)) {
          if (nativeP_462X) {
            proc_473X = SvalS;
            merged_arg0K0 = total_arg_count_447X;
            move_args_above_contB_return_tag = 3;
            goto move_args_above_contB;
           move_args_above_contB_return_3:
            v_474X = s48_call_native_procedure(proc_473X, 2);
            arg0K0 = v_474X;
            goto L24218;}
          else {
            arg0K0 = code_446X;
            arg0K1 = 2;
            arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
            arg0K3 = stack_space_461X;
            goto L25568;}}
        else {
          merged_arg0K0 = list_args_444X;
          merged_arg0K1 = list_arg_count_445X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          if (nativeP_462X) {
            proc_475X = SvalS;
            merged_arg0K0 = total_arg_count_447X;
            move_args_above_contB_return_tag = 4;
            goto move_args_above_contB;
           move_args_above_contB_return_4:
            v_476X = s48_call_native_procedure(proc_475X, 2);
            arg0K0 = v_476X;
            goto L24218;}
          else {
            arg0K0 = code_446X;
            arg0K1 = 2;
            arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
            arg0K3 = stack_space_461X;
            goto L25568;}}}
      else {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_443X;
        arg0K2 = list_args_444X;
        arg0K3 = list_arg_count_445X;
        goto L23361;}}}}
 L22895: {
  nargs_477X = arg0K0;
  opcode_478X = (((*((long *) ((SstackS) + (4 + (((nargs_477X)<<2)))))))>>2);
  handlers_479X = SHARED_REF((Sexception_handlersS));
  if ((3 == (3 & handlers_479X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_479X))) + -4))))>>2))))) {
      goto L22960;}
    else {
      goto L23043;}}
  else {
    goto L23043;}}
 L22456: {
  code_480X = arg0K0;
  envUtemp_offset_481X = arg0K1;
  used_482X = arg0K2;
  template_483X = arg0K3;
  envUtemplate_484X = *((unsigned char *) ((((char *) (-3 + code_480X))) + envUtemp_offset_481X));
  if ((3 == envUtemplate_484X)) {
    x_485X = *((long *) ((((char *) (-3 + (SvalS)))) + 4));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_485X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = template_483X;
    goto L22460;}
  else {
    if ((1 == envUtemplate_484X)) {
      x_486X = *((long *) ((((char *) (-3 + (SvalS)))) + 4));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_486X;
      goto L22460;}
    else {
      if ((2 == envUtemplate_484X)) {
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = template_483X;
        goto L22460;}
      else {
        goto L22460;}}}}
 L26816: {
  skip_487X = arg0K0;
  if ((total_arg_count_447X < 3)) {
    arg0K0 = total_arg_count_447X;
    goto L26824;}
  else {
    if ((2 < stack_arg_count_443X)) {
      arg0K0 = stack_arg_count_443X;
      goto L26824;}
    else {
      arg0K0 = 2;
      goto L26824;}}}
 L24218: {
  tag_488X = arg0K0;
  arg0K0 = tag_488X;
  goto L24222;}
 L25568: {
  code_489X = arg0K0;
  used_490X = arg0K1;
  template_491X = arg0K2;
  needed_stack_space_492X = arg0K3;
  envUtemplate_493X = *((unsigned char *) ((((char *) (-3 + code_489X))) + used_490X));
  if ((3 == envUtemplate_493X)) {
    x_494X = *((long *) ((((char *) (-3 + (SvalS)))) + 4));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_494X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = template_491X;
    goto L25572;}
  else {
    if ((1 == envUtemplate_493X)) {
      x_495X = *((long *) ((((char *) (-3 + (SvalS)))) + 4));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_495X;
      goto L25572;}
    else {
      if ((2 == envUtemplate_493X)) {
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = template_491X;
        goto L25572;}
      else {
        goto L25572;}}}}
 L22960: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_479X))) + (((opcode_478X)<<2)))));
  obj_496X = SvalS;
  if ((3 == (3 & obj_496X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_496X))) + -4))))>>2))))) {
      goto L22977;}
    else {
      goto L23057;}}
  else {
    goto L23057;}}
 L23043: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L22960;}
 L22460: {
  Slast_code_calledS = code_480X;
  Scode_pointerS = ((((char *) (-3 + code_480X))) + used_482X);
  if (((SstackS) < (s48_Sstack_limitS))) {
    interruptP_497X = (s48_Sstack_limitS) == (((char *) -1));
    s48_Sstack_limitS = (Sreal_stack_limitS);
    if (((SstackS) < (Sreal_stack_limitS))) {s48_copy_stack_into_heap();
      if (((SstackS) < (Sreal_stack_limitS))) {
        ps_error("VM's stack is too small (how can this happen?)", 0);
        if (interruptP_497X) {
          goto L22467;}
        else {
          goto L22474;}}
      else {
        if (interruptP_497X) {
          goto L22467;}
        else {
          goto L22474;}}}
    else {
      if (interruptP_497X) {
        goto L22467;}
      else {
        goto L22474;}}}
  else {
    goto L22474;}}
 L26824: {
  final_stack_arg_count_498X = arg0K0;
  if ((stack_arg_count_443X < final_stack_arg_count_498X)) {
    arg0K0 = final_stack_arg_count_498X;
    goto L26828;}
  else {
    arg0K0 = stack_arg_count_443X;
    goto L26828;}}
 L24222: {
  tag_499X = arg0K0;
  if ((tag_499X == 0)) {
    arg0K0 = (s48_Snative_protocolS);
    arg0K1 = 25;
    arg0K2 = 0;
    goto L22608;}
  else {
    if ((tag_499X == 1)) {
      stack_arg_count_500X = s48_Snative_protocolS;
      obj_501X = SvalS;
      if ((3 == (3 & obj_501X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_501X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_500X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L26761;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_500X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L23361;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_500X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L23361;}}
    else {
      if ((tag_499X == 2)) {
        pending_interruptP_return_tag = 0;
        goto pending_interruptP;
       pending_interruptP_return_0:
        v_502X = pending_interruptP0_return_value;
        if (v_502X) {
          x_503X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_503X;
          x_504X = Scurrent_threadS;
          addr_505X = (((char *) (-3 + x_504X))) + 12;S48_WRITE_BARRIER(x_504X, addr_505X, 1);
          *((long *) addr_505X) = 1;
          n_506X = Senabled_interruptsS;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_506X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 12;
          code_507X = Sinterrupt_return_codeS;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((long) ((((char *) (-3 + code_507X))) + 11)));
          ScontS = (SstackS);
          goto L23139;}
        else {
          SstackS = (ScontS);
          cont_508X = ScontS;
          pointer_509X = (((char *) (*((long *) cont_508X)))) + -2;
          size_510X = ((((*((unsigned char *) pointer_509X)))<<8)) + (*((unsigned char *) (pointer_509X + 1)));
          if ((65535 == size_510X)) {
            arg0K0 = ((((*((long *) (cont_508X + 4))))>>2));
            goto L24265;}
          else {
            arg0K0 = size_510X;
            goto L24265;}}}
      else {
        if ((tag_499X == 3)) {
          code_511X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_511X)))))<<2)), code_511X, 108, 0);
          n_512X = *((unsigned char *) (Scode_pointerS));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_512X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 108;
          arg0K0 = 0;
          goto L22895;}
        else {
          ps_error("unexpected native return value", 1, tag_499X);
          return v_513X;}}}}}
 L25572: {
  Slast_code_calledS = code_489X;
  Scode_pointerS = ((((char *) (-3 + code_489X))) + (1 + used_490X));
  merged_arg0K0 = needed_stack_space_492X;
  ensure_stack_spaceB_return_tag = 0;
  goto ensure_stack_spaceB;
 ensure_stack_spaceB_return_0:
  v_514X = ensure_stack_spaceB0_return_value;
  if (v_514X) {
    pending_interruptP_return_tag = 1;
    goto pending_interruptP;
   pending_interruptP_return_1:
    v_515X = pending_interruptP0_return_value;
    if (v_515X) {
      goto L23857;}
    else {
      goto L25586;}}
  else {
    goto L25586;}}
 L22977: {
  Scode_pointerS = (((char *) 0));
  Slosing_opcodeS = opcode_478X;
  arg0K0 = (2 + nargs_477X);
  arg0K1 = 25;
  arg0K2 = 0;
  goto L26761;}
 L23057: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L22977;}
 L22467: {
  pending_interruptP_return_tag = 2;
  goto pending_interruptP;
 pending_interruptP_return_2:
  v_516X = pending_interruptP0_return_value;
  if (v_516X) {
    goto L23857;}
  else {
    goto L22474;}}
 L22474: {
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L26828: {
  v_517X = arg0K0;
  merged_arg0K0 = v_517X;
  merged_arg0K1 = stack_arg_count_443X;
  merged_arg0K2 = list_args_444X;
  merged_arg0K3 = list_arg_count_445X;
  rest_list_setupAgc_return_tag = 1;
  goto rest_list_setupAgc;
 rest_list_setupAgc_return_1:
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((final_stack_arg_count_498X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_447X)<<2));
  if (nativeP_462X) {
    proc_518X = SvalS;
    merged_arg0K0 = (3 + final_stack_arg_count_498X);
    move_args_above_contB_return_tag = 5;
    goto move_args_above_contB;
   move_args_above_contB_return_5:
    v_519X = s48_call_native_procedure(proc_518X, skip_487X);
    arg0K0 = v_519X;
    goto L24218;}
  else {
    arg0K0 = code_446X;
    arg0K1 = skip_487X;
    arg0K2 = (*((long *) (((char *) (-3 + (SvalS))))));
    arg0K3 = stack_space_461X;
    goto L25568;}}
 L22608: {
  stack_nargs_520X = arg0K0;
  list_args_521X = arg0K1;
  list_arg_count_522X = arg0K2;
  code_pointer_523X = ((char *) (*((long *) (ScontS))));
  protocol_524X = *((unsigned char *) (code_pointer_523X + 1));
  if ((1 == protocol_524X)) {
    if ((1 == (stack_nargs_520X + list_arg_count_522X))) {
      if ((1 == stack_nargs_520X)) {
        v_525X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_525X;
        goto L22697;}
      else {
        arg0K0 = (*((long *) (((char *) (-3 + list_args_521X)))));
        goto L22697;}}
    else {
      arg0K0 = stack_nargs_520X;
      arg0K1 = list_args_521X;
      goto L23534;}}
  else {
    if ((66 == protocol_524X)) {
      pop_continuationB_return_tag = 0;
      goto pop_continuationB;
     pop_continuationB_return_0:
      arg0K0 = 1;
      goto L25558;}
    else {
      if ((127 < protocol_524X)) {
        if ((129 == protocol_524X)) {
          if ((1 == (stack_nargs_520X + list_arg_count_522X))) {
            if ((1 == stack_nargs_520X)) {
              v_526X = *((long *) (SstackS));
              SstackS = ((SstackS) + 4);
              arg0K0 = v_526X;
              goto L25012;}
            else {
              arg0K0 = (*((long *) (((char *) (-3 + list_args_521X)))));
              goto L25012;}}
          else {
            arg0K0 = stack_nargs_520X;
            arg0K1 = list_args_521X;
            goto L23534;}}
        else {
          if ((194 == protocol_524X)) {
            arg0K0 = 2;
            goto L24724;}
          else {
            ps_error("unknown native return protocol", 1, protocol_524X);
            arg0K0 = stack_nargs_520X;
            arg0K1 = list_args_521X;
            goto L23534;}}}
      else {
        if ((71 == protocol_524X)) {
          cont_527X = Sheap_continuationS;
          if ((3 == (3 & cont_527X))) {
            if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_527X))) + -4))))>>2))))) {
              merged_arg0K0 = cont_527X;
              merged_arg0K1 = stack_nargs_520X;
              copy_continuation_from_heapB_return_tag = 0;
              goto copy_continuation_from_heapB;
             copy_continuation_from_heapB_return_0:
              arg0K0 = stack_nargs_520X;
              arg0K1 = list_args_521X;
              arg0K2 = list_arg_count_522X;
              goto L22608;}
            else {
              goto L22643;}}
          else {
            goto L22643;}}
        else {
          if ((70 == protocol_524X)) {
            SvalS = (*((long *) ((ScontS) + 4)));
            offset_528X = ((((*((unsigned char *) (code_pointer_523X + 2))))<<8)) + (*((unsigned char *) (code_pointer_523X + 3)));
            if ((0 == offset_528X)) {
              cont_529X = ScontS;
              pointer_530X = (((char *) (*((long *) cont_529X)))) + -2;
              size_531X = ((((*((unsigned char *) pointer_530X)))<<8)) + (*((unsigned char *) (pointer_530X + 1)));
              if ((65535 == size_531X)) {
                arg0K0 = ((((*((long *) (cont_529X + 4))))>>2));
                goto L22760;}
              else {
                arg0K0 = size_531X;
                goto L22760;}}
            else {
              ScontS = ((ScontS) + 4);
              *((long *) (ScontS)) = (((long) (code_pointer_523X + offset_528X)));
              merged_arg0K0 = stack_nargs_520X;
              move_args_above_contB_return_tag = 6;
              goto move_args_above_contB;
             move_args_above_contB_return_6:
              arg0K0 = stack_nargs_520X;
              arg0K1 = list_args_521X;
              arg0K2 = list_arg_count_522X;
              goto L32460;}}
          else {
            if ((63 < protocol_524X)) {
              if ((65 == protocol_524X)) {
                count_532X = ((((*((unsigned char *) (code_pointer_523X + 2))))<<8)) + (*((unsigned char *) (code_pointer_523X + 3)));
                if (((stack_nargs_520X + list_arg_count_522X) < count_532X)) {
                  arg0K0 = stack_nargs_520X;
                  arg0K1 = list_args_521X;
                  goto L23534;}
                else {
                  arg_top_533X = SstackS;
                  pop_continuationB_return_tag = 1;
                  goto pop_continuationB;
                 pop_continuationB_return_1:
                  arg1K0 = ((SstackS) + -4);
                  arg1K1 = (arg_top_533X + (-4 + (((stack_nargs_520X)<<2))));
                  goto L26509;}}
              else {
                if ((64 == protocol_524X)) {
                  arg0K0 = (((((*((unsigned char *) (code_pointer_523X + 2))))<<8)) + (*((unsigned char *) (code_pointer_523X + 3))));
                  arg0K1 = 3;
                  arg0K2 = stack_nargs_520X;
                  arg0K3 = list_args_521X;
                  arg0K4 = list_arg_count_522X;
                  goto L26375;}
                else {
                  ps_error("unknown protocol", 1, protocol_524X);
                  arg0K0 = stack_nargs_520X;
                  arg0K1 = list_args_521X;
                  goto L23534;}}}
            else {
              arg0K0 = protocol_524X;
              arg0K1 = 1;
              arg0K2 = stack_nargs_520X;
              arg0K3 = list_args_521X;
              arg0K4 = list_arg_count_522X;
              goto L26375;}}}}}}}
 L23139: {
  n_534X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L23192;}
 L24265: {
  v_535X = arg0K0;
  ScontS = (cont_508X + (4 + (((v_535X)<<2))));
  v_536X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  v_537X = s48_invoke_native_continuation((2 + (((long) (((char *) v_536X))))));
  arg0K0 = v_537X;
  goto L24222;}
 L23857: {
  x_538X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_538X;
  code_539X = current_code_vector();
  pc_540X = (Scode_pointerS) - (((char *) (-3 + code_539X)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = code_539X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((pc_540X)<<2));
  x_541X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_541X;
  x_542X = Scurrent_threadS;
  addr_543X = (((char *) (-3 + x_542X))) + 12;S48_WRITE_BARRIER(x_542X, addr_543X, 1);
  *((long *) addr_543X) = 1;
  n_544X = Senabled_interruptsS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_544X)<<2));
  code_545X = Sinterrupt_return_codeS;
  v_546X = ((((ScontS) - (SstackS)))>>2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (4 + (((v_546X)<<2)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((long) ((((char *) (-3 + code_545X))) + 11)));
  ScontS = (SstackS);
  goto L23139;}
 L25586: {
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L25070: {
  code_pointer_547X = arg1K0;
  switch ((*((unsigned char *) code_pointer_547X))) {
    case 0 : 
    case 23 : 
    case 27 : 
    case 34 : {
      code_548X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_548X)))))<<2)), code_548X, 56, 4);
      n_549X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_549X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 56;
      n_550X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_550X)<<2));
      arg0K0 = 1;
      goto L22895;}
      break;
    case 1 : {
      SvalS = (-512 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 2 : {
      x_551X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_551X;
      SvalS = (-512 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 3 : {
      x_552X = -512 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_552X;
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 4 : {
      template_553X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      index_554X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
      location_555X = *((long *) ((((char *) (-3 + template_553X))) + (((index_554X)<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_555X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        code_556X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_556X)))))<<2)), code_556X, 4, 20);
        n_557X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_557X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 4;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = location_555X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = template_553X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((index_554X)<<2));
        arg0K0 = 3;
        goto L22895;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 5);
        arg1K0 = (Scode_pointerS);
        goto L25070;}}
      break;
    case 5 : {
      template_558X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      index_559X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
      location_560X = *((long *) ((((char *) (-3 + template_558X))) + (((index_559X)<<2))));
      if ((273 == (*((long *) ((((char *) (-3 + location_560X))) + 4))))) {
        code_561X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_561X)))))<<2)), code_561X, 4, 20);
        n_562X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_562X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 4;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = location_560X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = template_558X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((index_559X)<<2));
        x_563X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_563X;
        arg0K0 = 4;
        goto L22895;}
      else {
        val_564X = SvalS;
        addr_565X = (((char *) (-3 + location_560X))) + 4;S48_WRITE_BARRIER(location_560X, addr_565X, val_564X);
        *((long *) addr_565X) = val_564X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 5);
        arg1K0 = (Scode_pointerS);
        goto L25070;}}
      break;
    case 6 : {
      total_count_566X = *((unsigned char *) ((Scode_pointerS) + 1));
      closures_567X = *((unsigned char *) ((Scode_pointerS) + 2));s48_make_availableAgc((((((1 + total_count_566X) + (3 * closures_567X)))<<2)));
      len_568X = ((total_count_566X)<<2);
      addr_569X = s48_allocate_small((4 + len_568X));
      *((long *) addr_569X) = (10 + (((len_568X)<<8)));
      new_env_570X = 3 + (((long) (addr_569X + 4)));
      if ((0 == closures_567X)) {
        arg0K0 = new_env_570X;
        arg0K1 = 0;
        arg0K2 = 2;
        arg0K3 = total_count_566X;
        goto L8220;}
      else {
        template_571X = *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<2))));
        arg0K0 = closures_567X;
        arg0K1 = 0;
        arg0K2 = 3;
        goto L17415;}}
      break;
    case 7 : {
      total_count_572X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      closures_573X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));s48_make_availableAgc((((((1 + total_count_572X) + (3 * closures_573X)))<<2)));
      len_574X = ((total_count_572X)<<2);
      addr_575X = s48_allocate_small((4 + len_574X));
      *((long *) addr_575X) = (10 + (((len_574X)<<8)));
      new_env_576X = 3 + (((long) (addr_575X + 4)));
      if ((0 == closures_573X)) {
        arg0K0 = new_env_576X;
        arg0K1 = 0;
        arg0K2 = 4;
        arg0K3 = total_count_572X;
        goto L10794;}
      else {
        template_577X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 5))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 6)))))<<2))));
        arg0K0 = closures_573X;
        arg0K1 = 0;
        arg0K2 = 6;
        goto L17476;}}
      break;
    case 8 : {
      x_578X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_578X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 9 : {
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 10 : {
      value_579X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SvalS = value_579X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 11 : {
      SstackS = ((SstackS) + (0 - ((((0 - (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))))<<2))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 12 : {
      SvalS = (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 13 : {
      x_580X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_580X;
      SvalS = (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 14 : {
      x_581X = *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_581X;
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 15 : {
      SvalS = (*((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 16 : {
      *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 17 : {
      *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 18 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 19 : {
      x_582X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_582X;
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 20 : {
      x_583X = *((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2))));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_583X;
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 21 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))))))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 22 : {s48_make_availableAgc(((((-4 & ((Sstack_endS) - (SstackS))))<<2)));
      if ((1 == (((long) (ScontS))))) {
        arg0K0 = 1;
        goto L50668;}
      else {
        v_584X = really_preserve_continuation(0);
        arg0K0 = v_584X;
        goto L50668;}}
      break;
    case 24 : {
      stack_arg_count_585X = *((unsigned char *) ((Scode_pointerS) + 3));
      code_pointer_586X = (Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      ScontS = ((SstackS) + (((stack_arg_count_585X)<<2)));
      *((long *) (ScontS)) = (((long) code_pointer_586X));
      arg0K0 = stack_arg_count_585X;
      goto L24907;}
      break;
    case 25 : {
      merged_arg0K0 = (*((unsigned char *) ((Scode_pointerS) + 1)));
      move_args_above_contB_return_tag = 7;
      goto move_args_above_contB;
     move_args_above_contB_return_7:
      arg0K0 = (*((unsigned char *) ((Scode_pointerS) + 1)));
      goto L24907;}
      break;
    case 26 : {
      stack_arg_count_587X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
      return_pointer_offset_588X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      if ((0 == return_pointer_offset_588X)) {
        merged_arg0K0 = stack_arg_count_587X;
        move_args_above_contB_return_tag = 8;
        goto move_args_above_contB;
       move_args_above_contB_return_8:
        goto L24184;}
      else {
        code_pointer_589X = (Scode_pointerS) + return_pointer_offset_588X;
        ScontS = ((SstackS) + (((stack_arg_count_587X)<<2)));
        *((long *) (ScontS)) = (((long) code_pointer_589X));
        goto L24184;}}
      break;
    case 28 : {
      list_args_590X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      merged_arg0K0 = list_args_590X;
      okay_argument_list_return_tag = 0;
      goto okay_argument_list;
     okay_argument_list_return_0:
      okayP_591X = okay_argument_list0_return_value;
      length_592X = okay_argument_list1_return_value;
      if (okayP_591X) {
        stack_nargs_593X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
        return_pointer_offset_594X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
        if ((0 == return_pointer_offset_594X)) {
          merged_arg0K0 = stack_nargs_593X;
          move_args_above_contB_return_tag = 9;
          goto move_args_above_contB;
         move_args_above_contB_return_9:
          arg0K0 = stack_nargs_593X;
          arg0K1 = list_args_590X;
          arg0K2 = length_592X;
          goto L32460;}
        else {
          code_pointer_595X = (Scode_pointerS) + return_pointer_offset_594X;
          ScontS = ((SstackS) + (((stack_nargs_593X)<<2)));
          *((long *) (ScontS)) = (((long) code_pointer_595X));
          arg0K0 = stack_nargs_593X;
          arg0K1 = list_args_590X;
          arg0K2 = length_592X;
          goto L32460;}}
      else {
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_args_590X;
        merged_arg0K0 = 25;
        merged_arg0K1 = (1 + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        pop_args_GlistSAgc_return_tag = 1;
        goto pop_args_GlistSAgc;
       pop_args_GlistSAgc_return_1:
        args_596X = pop_args_GlistSAgc0_return_value;
        code_597X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_597X)))))<<2)), code_597X, 20, 0);
        n_598X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_598X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_599X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_599X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = args_596X;
        arg0K0 = 2;
        goto L22895;}}
      break;
    case 29 : {
      p_600X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      p_601X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_602X = ((p_601X)>>2);
      proc_603X = *((long *) ((SstackS) + (((stack_nargs_602X)<<2))));
      merged_arg0K0 = (((p_600X)>>2));
      move_args_above_contB_return_tag = 10;
      goto move_args_above_contB;
     move_args_above_contB_return_10:
      rest_list_604X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((25 == rest_list_604X)) {
        v_605X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_605X;
        arg0K1 = (-2 + stack_nargs_602X);
        goto L15729;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_604X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_604X)))));
          arg0K1 = (-1 + stack_nargs_602X);
          goto L15729;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_604X))) + 4)))))) + 4)));
          arg0K1 = rest_list_604X;
          goto L15774;}}}
      break;
    case 30 : {
      cont_606X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & cont_606X))) {
        if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_606X))) + -4))))>>2))))) {
          merged_arg0K0 = cont_606X;
          merged_arg0K1 = 0;
          copy_continuation_from_heapB_return_tag = 1;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_1:
          goto L24142;}
        else {
          goto L24151;}}
      else {
        goto L24151;}}
      break;
    case 31 : {
      goto L40491;}
      break;
    case 32 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L22608;}
      break;
    case 33 : {
      p_607X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      p_608X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_609X = ((p_608X)>>2);
      rest_list_610X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = stack_nargs_609X;
      arg0K1 = rest_list_610X;
      arg0K2 = ((((p_607X)>>2)) - stack_nargs_609X);
      goto L22608;}
      break;
    case 35 : {
      template_611X = *((long *) ((((char *) (-3 + (*((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))))))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 5))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 6)))))<<2))));
      code_612X = *((long *) (((char *) (-3 + template_611X))));
      nargs_613X = *((unsigned char *) ((Scode_pointerS) + 7));
      return_pointer_offset_614X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      if ((0 == return_pointer_offset_614X)) {
        merged_arg0K0 = nargs_613X;
        move_args_above_contB_return_tag = 11;
        goto move_args_above_contB;
       move_args_above_contB_return_11:
        goto L26639;}
      else {
        code_pointer_615X = (Scode_pointerS) + return_pointer_offset_614X;
        ScontS = ((SstackS) + (((nargs_613X)<<2)));
        *((long *) (ScontS)) = (((long) code_pointer_615X));
        goto L26639;}}
      break;
    case 36 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L25070;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L25070;}}
      break;
    case 37 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 38 : {
      Scode_pointerS = ((Scode_pointerS) + (0 - (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))));
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 39 : {
      if ((0 == (3 & (SvalS)))) {
        p_616X = SvalS;
        max_617X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_618X = ((p_616X)>>2);
        if ((val_618X < 0)) {
          goto L25427;}
        else {
          if ((val_618X < max_617X)) {
            index_619X = 1 + (((val_618X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_619X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_619X)))));
            goto L25429;}
          else {
            goto L25427;}}}
      else {
        code_620X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_620X)))))<<2)), code_620X, 20, 0);
        n_621X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_621X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_622X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_622X;
        arg0K0 = 1;
        goto L22895;}}
      break;
    case 40 : {
      stack_nargs_623X = (((*((long *) (SstackS))))>>2);
      if ((0 == stack_nargs_623X)) {
        arg0_624X = *((long *) ((SstackS) + 8));
        rest_list_625X = *((long *) ((SstackS) + 4));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg0_624X;
        SvalS = (*((long *) (((char *) (-3 + rest_list_625X)))));
        goto L26012;}
      else {
        arg1_626X = *((long *) ((SstackS) + (((stack_nargs_623X)<<2))));
        arg0_627X = *((long *) ((SstackS) + (4 + (((stack_nargs_623X)<<2)))));
        *((long *) ((SstackS) + (4 + (((stack_nargs_623X)<<2))))) = 1;
        *((long *) (SstackS)) = (-4 + (((stack_nargs_623X)<<2)));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg0_627X;
        SvalS = arg1_626X;
        goto L26012;}}
      break;
    case 41 : {
      stack_nargs_628X = (((*((long *) (SstackS))))>>2);
      if ((stack_nargs_628X == 0)) {
        rest_list_629X = *((long *) ((SstackS) + 4));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_629X))) + 4))))) {
          arg0K0 = 1;
          goto L25325;}
        else {
          *((long *) ((SstackS) + 4)) = (*((long *) ((((char *) (-3 + rest_list_629X))) + 4)));
          *((long *) ((SstackS) + 8)) = (SvalS);
          arg0K0 = -2;
          goto L25325;}}
      else {
        if ((stack_nargs_628X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 4))))) {
            arg0K0 = 1;
            goto L25325;}
          else {
            *((long *) (SstackS)) = 0;
            *((long *) ((SstackS) + 8)) = (SvalS);
            arg0K0 = -2;
            goto L25325;}}
        else {
          *((long *) ((SstackS) + (4 + (((stack_nargs_628X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L25325;}}}
      break;
    case 42 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L25070;}
      else {
        x_630X = (((*((long *) (SstackS))))>>2);
        if ((x_630X == 0)) {
          rest_list_631X = *((long *) ((SstackS) + 4));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_631X))) + 4))))) {
            arg0K0 = 1;
            goto L25903;}
          else {
            *((long *) ((SstackS) + 4)) = (*((long *) ((((char *) (-3 + rest_list_631X))) + 4)));
            *((long *) ((SstackS) + 8)) = (*((long *) (((char *) (-3 + rest_list_631X)))));
            arg0K0 = -2;
            goto L25903;}}
        else {
          if ((x_630X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 4))))) {
              arg0K0 = 1;
              goto L25903;}
            else {
              *((long *) (SstackS)) = 0;
              arg0K0 = -2;
              goto L25903;}}
          else {
            arg0K0 = -2;
            goto L25903;}}}}
      break;
    case 43 : {
      arg2_632X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_633X = SvalS;
      if ((arg2_632X == x_633X)) {
        arg0K0 = 5;
        goto L50725;}
      else {
        arg0K0 = 1;
        goto L50725;}}
      break;
    case 44 : {
      x_634X = SvalS;
      if ((0 == (3 & x_634X))) {
        arg0K0 = 5;
        goto L50737;}
      else {
        if ((3 == (3 & x_634X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_634X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L50737;}
          else {
            goto L14221;}}
        else {
          goto L14221;}}}
      break;
    case 45 : {
      n_635X = SvalS;
      if ((0 == (3 & n_635X))) {
        goto L41786;}
      else {
        if ((3 == (3 & n_635X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_635X))) + -4))))>>2))))) {
            goto L41786;}
          else {
            goto L41787;}}
        else {
          goto L41787;}}}
      break;
    case 46 : {
      n_636X = SvalS;
      if ((0 == (3 & n_636X))) {
        goto L41935;}
      else {
        if ((3 == (3 & n_636X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_636X))) + -4))))>>2))))) {
            goto L41935;}
          else {
            goto L41910;}}
        else {
          goto L41910;}}}
      break;
    case 47 : {
      n_637X = SvalS;
      if ((0 == (3 & n_637X))) {
        goto L42126;}
      else {
        if ((3 == (3 & n_637X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_637X))) + -4))))>>2))))) {
            goto L42126;}
          else {
            goto L42101;}}
        else {
          goto L42101;}}}
      break;
    case 48 : {
      n_638X = SvalS;
      if ((0 == (3 & n_638X))) {
        goto L42317;}
      else {
        if ((3 == (3 & n_638X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_638X))) + -4))))>>2))))) {
            goto L42317;}
          else {
            goto L42292;}}
        else {
          goto L42292;}}}
      break;
    case 49 : {
      n_639X = SvalS;
      if ((0 == (3 & n_639X))) {
        goto L34017;}
      else {
        if ((3 == (3 & n_639X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_639X))) + -4))))>>2))))) {
            goto L34017;}
          else {
            goto L34018;}}
        else {
          goto L34018;}}}
      break;
    case 50 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 51 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 52 : {
      arg2_640X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_641X = SvalS;
      if ((0 == (3 & (arg2_640X | y_641X)))) {s48_make_availableAgc(16);
        x_642X = (((arg2_640X)>>2)) + (((y_641X)>>2));
        if ((536870911 < x_642X)) {
          goto L34159;}
        else {
          if ((x_642X < -536870912)) {
            goto L34159;}
          else {
            arg0K0 = (((x_642X)<<2));
            goto L34105;}}}
      else {
        if ((0 == (3 & arg2_640X))) {
          goto L34111;}
        else {
          if ((3 == (3 & arg2_640X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_640X))) + -4))))>>2))))) {
              goto L34111;}
            else {
              goto L34120;}}
          else {
            goto L34120;}}}}
      break;
    case 53 : {
      arg2_643X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_644X = SvalS;
      if ((0 == (3 & (arg2_643X | y_644X)))) {
        a_645X = ((arg2_643X)>>2);
        b_646X = ((y_644X)>>2);
        if ((a_645X < 0)) {
          arg0K0 = (0 - a_645X);
          goto L8385;}
        else {
          arg0K0 = a_645X;
          goto L8385;}}
      else {
        if ((0 == (3 & arg2_643X))) {
          goto L44190;}
        else {
          if ((3 == (3 & arg2_643X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_643X))) + -4))))>>2))))) {
              goto L44190;}
            else {
              goto L44199;}}
          else {
            goto L44199;}}}}
      break;
    case 54 : {
      arg2_647X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_648X = SvalS;
      if ((0 == (3 & (arg2_647X | y_648X)))) {s48_make_availableAgc(16);
        x_649X = (((arg2_647X)>>2)) - (((y_648X)>>2));
        if ((536870911 < x_649X)) {
          goto L34464;}
        else {
          if ((x_649X < -536870912)) {
            goto L34464;}
          else {
            arg0K0 = (((x_649X)<<2));
            goto L34410;}}}
      else {
        if ((0 == (3 & arg2_647X))) {
          goto L34416;}
        else {
          if ((3 == (3 & arg2_647X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_647X))) + -4))))>>2))))) {
              goto L34416;}
            else {
              goto L34425;}}
          else {
            goto L34425;}}}}
      break;
    case 55 : {
      arg2_650X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_651X = SvalS;
      if ((0 == y_651X)) {
        arg0K0 = arg2_650X;
        arg0K1 = y_651X;
        goto L23790;}
      else {
        if ((0 == (3 & (arg2_650X | y_651X)))) {
          if ((0 == y_651X)) {
            arg0K0 = arg2_650X;
            arg0K1 = y_651X;
            goto L23790;}
          else {
            a_652X = ((arg2_650X)>>2);
            b_653X = ((y_651X)>>2);
            if ((a_652X < 0)) {
              arg0K0 = (0 - a_652X);
              goto L8643;}
            else {
              arg0K0 = a_652X;
              goto L8643;}}}
        else {
          if ((0 == (3 & arg2_650X))) {
            goto L44433;}
          else {
            if ((3 == (3 & arg2_650X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_650X))) + -4))))>>2))))) {
                goto L44433;}
              else {
                goto L44466;}}
            else {
              goto L44466;}}}}}
      break;
    case 56 : {
      arg2_654X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_655X = SvalS;
      if ((0 == (3 & (arg2_654X | y_655X)))) {
        if ((arg2_654X == y_655X)) {
          arg0K0 = 5;
          goto L34711;}
        else {
          arg0K0 = 1;
          goto L34711;}}
      else {
        if ((0 == (3 & arg2_654X))) {
          goto L34717;}
        else {
          if ((3 == (3 & arg2_654X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_654X))) + -4))))>>2))))) {
              goto L34717;}
            else {
              goto L34728;}}
          else {
            goto L34728;}}}}
      break;
    case 57 : {
      arg2_656X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_657X = SvalS;
      if ((0 == (3 & (arg2_656X | y_657X)))) {
        if ((arg2_656X < y_657X)) {
          arg0K0 = 5;
          goto L34987;}
        else {
          arg0K0 = 1;
          goto L34987;}}
      else {
        if ((0 == (3 & arg2_656X))) {
          goto L34993;}
        else {
          if ((3 == (3 & arg2_656X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_656X))) + -4))))>>2))))) {
              goto L34993;}
            else {
              goto L35004;}}
          else {
            goto L35004;}}}}
      break;
    case 58 : {
      arg2_658X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_659X = SvalS;
      if ((0 == (3 & (arg2_658X | y_659X)))) {
        if ((y_659X < arg2_658X)) {
          arg0K0 = 5;
          goto L35326;}
        else {
          arg0K0 = 1;
          goto L35326;}}
      else {
        if ((0 == (3 & arg2_658X))) {
          goto L35332;}
        else {
          if ((3 == (3 & arg2_658X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_658X))) + -4))))>>2))))) {
              goto L35332;}
            else {
              goto L35343;}}
          else {
            goto L35343;}}}}
      break;
    case 59 : {
      arg2_660X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_661X = SvalS;
      if ((0 == (3 & (arg2_660X | y_661X)))) {
        if ((y_661X < arg2_660X)) {
          arg0K0 = 1;
          goto L35665;}
        else {
          arg0K0 = 5;
          goto L35665;}}
      else {
        if ((0 == (3 & arg2_660X))) {
          goto L35671;}
        else {
          if ((3 == (3 & arg2_660X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_660X))) + -4))))>>2))))) {
              goto L35671;}
            else {
              goto L35682;}}
          else {
            goto L35682;}}}}
      break;
    case 60 : {
      arg2_662X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_663X = SvalS;
      if ((0 == (3 & (arg2_662X | y_663X)))) {
        if ((arg2_662X < y_663X)) {
          arg0K0 = 1;
          goto L35975;}
        else {
          arg0K0 = 5;
          goto L35975;}}
      else {
        if ((0 == (3 & arg2_662X))) {
          goto L35981;}
        else {
          if ((3 == (3 & arg2_662X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_662X))) + -4))))>>2))))) {
              goto L35981;}
            else {
              goto L35992;}}
          else {
            goto L35992;}}}}
      break;
    case 61 : {
      arg2_664X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_665X = SvalS;
      if ((0 == y_665X)) {
        arg0K0 = arg2_664X;
        arg0K1 = y_665X;
        goto L23790;}
      else {
        if ((0 == (3 & (arg2_664X | y_665X)))) {
          if ((0 == y_665X)) {
            arg0K0 = arg2_664X;
            arg0K1 = y_665X;
            goto L20032;}
          else {
            a_666X = ((arg2_664X)>>2);
            b_667X = ((y_665X)>>2);
            if ((a_666X < 0)) {
              arg0K0 = (0 - a_666X);
              goto L9257;}
            else {
              arg0K0 = a_666X;
              goto L9257;}}}
        else {
          if ((0 == (3 & arg2_664X))) {
            goto L36297;}
          else {
            if ((3 == (3 & arg2_664X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_664X))) + -4))))>>2))))) {
                goto L36297;}
              else {
                arg0K0 = arg2_664X;
                arg0K1 = y_665X;
                goto L23790;}}
            else {
              arg0K0 = arg2_664X;
              arg0K1 = y_665X;
              goto L23790;}}}}}
      break;
    case 62 : {
      arg2_668X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_669X = SvalS;
      if ((0 == y_669X)) {
        arg0K0 = arg2_668X;
        arg0K1 = y_669X;
        goto L23790;}
      else {
        if ((0 == (3 & (arg2_668X | y_669X)))) {
          if ((0 == y_669X)) {
            arg0K0 = arg2_668X;
            arg0K1 = y_669X;
            goto L19941;}
          else {
            a_670X = ((arg2_668X)>>2);
            if ((a_670X < 0)) {
              arg0K0 = (0 - a_670X);
              goto L36477;}
            else {
              arg0K0 = a_670X;
              goto L36477;}}}
        else {
          if ((0 == (3 & arg2_668X))) {
            goto L36451;}
          else {
            if ((3 == (3 & arg2_668X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_668X))) + -4))))>>2))))) {
                goto L36451;}
              else {
                arg0K0 = arg2_668X;
                arg0K1 = y_669X;
                goto L23790;}}
            else {
              arg0K0 = arg2_668X;
              arg0K1 = y_669X;
              goto L23790;}}}}}
      break;
    case 63 : {
      n_671X = SvalS;
      if ((0 == (3 & n_671X))) {
        goto L36616;}
      else {
        if ((3 == (3 & n_671X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_671X))) + -4))))>>2))))) {
            goto L36616;}
          else {
            goto L36617;}}
        else {
          goto L36617;}}}
      break;
    case 64 : {
      n_672X = SvalS;
      if ((0 == (3 & n_672X))) {
        goto L36687;}
      else {
        if ((3 == (3 & n_672X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_672X))) + -4))))>>2))))) {
            goto L36687;}
          else {
            goto L36688;}}
        else {
          goto L36688;}}}
      break;
    case 65 : {
      n_673X = SvalS;
      if ((0 == (3 & n_673X))) {
        goto L36758;}
      else {
        if ((3 == (3 & n_673X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_673X))) + -4))))>>2))))) {
            goto L36758;}
          else {
            goto L36761;}}
        else {
          goto L36761;}}}
      break;
    case 66 : {
      n_674X = SvalS;
      if ((0 == (3 & n_674X))) {
        goto L36834;}
      else {
        if ((3 == (3 & n_674X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_674X))) + -4))))>>2))))) {
            goto L36834;}
          else {
            goto L36835;}}
        else {
          goto L36835;}}}
      break;
    case 67 : {
      n_675X = SvalS;
      if ((0 == (3 & n_675X))) {
        goto L36905;}
      else {
        if ((3 == (3 & n_675X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_675X))) + -4))))>>2))))) {
            goto L36905;}
          else {
            goto L36908;}}
        else {
          goto L36908;}}}
      break;
    case 68 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 69 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 70 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 71 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 72 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 73 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 74 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 75 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 76 : {
      x_676X = SvalS;
      if ((0 == (3 & x_676X))) {
        if ((x_676X < 0)) {
          arg0K0 = x_676X;
          goto L23759;}
        else {
          goto L50843;}}
      else {
        v_677X = s48_bignum_test((((char *) (-3 + x_676X))));
        if ((-1 == v_677X)) {
          arg0K0 = x_676X;
          goto L23759;}
        else {
          goto L50843;}}}
      break;
    case 77 : {
      x_678X = SvalS;
      if ((0 == (3 & x_678X))) {
        goto L23669;}
      else {
        if ((3 == (3 & x_678X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_678X))) + -4))))>>2))))) {
            goto L23669;}
          else {
            goto L23672;}}
        else {
          goto L23672;}}}
      break;
    case 78 : {
      arg0K0 = (SvalS);
      goto L23759;}
      break;
    case 79 : {
      arg2_679X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = arg2_679X;
      arg0K1 = (SvalS);
      goto L23790;}
      break;
    case 80 : {
      arg2_680X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = arg2_680X;
      arg0K1 = (SvalS);
      goto L23790;}
      break;
    case 81 : {
      arg2_681X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = arg2_681X;
      arg0K1 = (SvalS);
      goto L23790;}
      break;
    case 82 : {
      x_682X = SvalS;
      if ((0 == (3 & x_682X))) {
        SvalS = (~ (3 | x_682X));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L25070;}
      else {
        if ((0 == (3 & x_682X))) {
          goto L37068;}
        else {
          if ((3 == (3 & x_682X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_682X))) + -4))))>>2))))) {
              goto L37068;}
            else {
              goto L37071;}}
          else {
            goto L37071;}}}}
      break;
    case 83 : {
      x_683X = SvalS;
      if ((0 == (3 & x_683X))) {
        x_684X = ((x_683X)>>2);
        if ((x_684X < 0)) {
          arg0K0 = (~ x_684X);
          goto L37175;}
        else {
          arg0K0 = x_684X;
          goto L37175;}}
      else {
        if ((0 == (3 & x_683X))) {
          goto L37158;}
        else {
          if ((3 == (3 & x_683X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_683X))) + -4))))>>2))))) {
              goto L37158;}
            else {
              goto L37161;}}
          else {
            goto L37161;}}}}
      break;
    case 84 : {
      arg2_685X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_686X = SvalS;
      if ((0 == (3 & (arg2_685X | y_686X)))) {
        SvalS = (arg2_685X & y_686X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L25070;}
      else {
        if ((0 == (3 & arg2_685X))) {
          goto L37276;}
        else {
          if ((3 == (3 & arg2_685X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_685X))) + -4))))>>2))))) {
              goto L37276;}
            else {
              goto L37285;}}
          else {
            goto L37285;}}}}
      break;
    case 85 : {
      arg2_687X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_688X = SvalS;
      if ((0 == (3 & (arg2_687X | y_688X)))) {
        SvalS = (arg2_687X | y_688X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L25070;}
      else {
        if ((0 == (3 & arg2_687X))) {
          goto L37455;}
        else {
          if ((3 == (3 & arg2_687X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_687X))) + -4))))>>2))))) {
              goto L37455;}
            else {
              goto L37464;}}
          else {
            goto L37464;}}}}
      break;
    case 86 : {
      arg2_689X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_690X = SvalS;
      if ((0 == (3 & (arg2_689X | y_690X)))) {
        SvalS = (arg2_689X ^ y_690X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L25070;}
      else {
        if ((0 == (3 & arg2_689X))) {
          goto L37634;}
        else {
          if ((3 == (3 & arg2_689X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_689X))) + -4))))>>2))))) {
              goto L37634;}
            else {
              goto L37643;}}
          else {
            goto L37643;}}}}
      break;
    case 87 : {
      arg2_691X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_692X = SvalS;
      if ((3 == (3 & y_692X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_692X))) + -4))))>>2))))) {
          v_693X = s48_bignum_test((((char *) (-3 + y_692X))));
          if ((1 == v_693X)) {
            code_694X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_694X)))))<<2)), code_694X, 24, 4);
            n_695X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_695X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 24;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = arg2_691X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_692X;
            arg0K0 = 2;
            goto L22895;}
          else {
            if ((0 == (3 & arg2_691X))) {
              if ((arg2_691X < 0)) {
                arg0K0 = -4;
                goto L33749;}
              else {
                arg0K0 = 0;
                goto L33749;}}
            else {
              if ((3 == (3 & arg2_691X))) {
                if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_691X))) + -4))))>>2))))) {
                  v_696X = s48_bignum_test((((char *) (-3 + arg2_691X))));
                  if ((1 == v_696X)) {
                    arg0K0 = 0;
                    goto L33765;}
                  else {
                    arg0K0 = -4;
                    goto L33765;}}
                else {
                  goto L33766;}}
              else {
                goto L33766;}}}}
        else {
          goto L44715;}}
      else {
        goto L44715;}}
      break;
    case 88 : {
      x_697X = SvalS;
      if ((9 == (255 & x_697X))) {
        arg0K0 = 5;
        goto L50904;}
      else {
        arg0K0 = 1;
        goto L50904;}}
      break;
    case 89 : {
      arg2_698X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((9 == (255 & arg2_698X))) {
        if ((9 == (255 & (SvalS)))) {
          x_699X = SvalS;
          if ((arg2_698X == x_699X)) {
            arg0K0 = 5;
            goto L41527;}
          else {
            arg0K0 = 1;
            goto L41527;}}
        else {
          goto L41498;}}
      else {
        goto L41498;}}
      break;
    case 90 : {
      arg2_700X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((9 == (255 & arg2_700X))) {
        if ((9 == (255 & (SvalS)))) {
          x_701X = SvalS;
          if ((arg2_700X < x_701X)) {
            arg0K0 = 5;
            goto L41419;}
          else {
            arg0K0 = 1;
            goto L41419;}}
        else {
          goto L41390;}}
      else {
        goto L41390;}}
      break;
    case 91 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L25070;}
      else {
        code_702X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_702X)))))<<2)), code_702X, 20, 4);
        n_703X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_703X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_704X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_704X;
        arg0K0 = 1;
        goto L22895;}}
      break;
    case 92 : {
      if ((0 == (3 & (SvalS)))) {
        x_705X = (((SvalS))>>2);
        if ((255 < x_705X)) {
          goto L37804;}
        else {
          if ((x_705X < 0)) {
            goto L37804;}
          else {
            SvalS = (9 + (((((x_705X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L25070;}}}
      else {
        code_706X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_706X)))))<<2)), code_706X, 20, 4);
        n_707X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_707X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_708X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_708X;
        arg0K0 = 1;
        goto L22895;}}
      break;
    case 93 : {
      x_709X = SvalS;
      if ((21 == x_709X)) {
        arg0K0 = 5;
        goto L50924;}
      else {
        arg0K0 = 1;
        goto L50924;}}
      break;
    case 94 : {
      x_710X = SvalS;
      type_711X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_710X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_710X))) + -4))))>>2))) == type_711X)) {
          arg0K0 = 5;
          goto L50938;}
        else {
          arg0K0 = 1;
          goto L50938;}}
      else {
        arg0K0 = 1;
        goto L50938;}}
      break;
    case 95 : {
      stob_712X = SvalS;
      type_713X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_712X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_712X))) + -4))))>>2))) == type_713X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_712X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          goto L27861;}}
      else {
        goto L27861;}}
      break;
    case 96 : {
      len_714X = *((unsigned char *) ((Scode_pointerS) + 1));s48_make_availableAgc((4 + (((len_714X)<<2))));
      type_715X = *((unsigned char *) ((Scode_pointerS) + 2));
      len_716X = ((len_714X)<<2);
      addr_717X = s48_allocate_small((4 + len_716X));
      *((long *) addr_717X) = (2 + (((((((len_716X)<<6)) + type_715X))<<2)));
      new_718X = 3 + (((long) (addr_717X + 4)));
      if ((len_714X < 1)) {
        goto L27982;}
      else {
        *((long *) ((((char *) (-3 + new_718X))) + (-4 + (((len_714X)<<2))))) = (SvalS);
        arg0K0 = (-2 + len_714X);
        goto L27966;}}
      break;
    case 97 : {
      p_719X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      len_720X = ((p_719X)>>2);s48_make_availableAgc((4 + (((len_720X)<<2))));
      type_721X = *((unsigned char *) ((Scode_pointerS) + 1));
      len_722X = ((len_720X)<<2);
      addr_723X = s48_allocate_small((4 + len_722X));
      *((long *) addr_723X) = (2 + (((((((len_722X)<<6)) + type_721X))<<2)));
      new_724X = 3 + (((long) (addr_723X + 4)));
      p_725X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_726X = ((p_725X)>>2);
      rest_list_727X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = (-1 + stack_nargs_726X);
      goto L28100;}
      break;
    case 98 : {
      stob_728X = SvalS;
      offset_729X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_730X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_728X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_728X))) + -4))))>>2))) == type_730X)) {
          SvalS = (*((long *) ((((char *) (-3 + stob_728X))) + (((offset_729X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          goto L28268;}}
      else {
        goto L28268;}}
      break;
    case 99 : {
      arg2_731X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      value_732X = SvalS;
      offset_733X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_734X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & arg2_731X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + arg2_731X))) + -4))))>>2))) == type_734X)) {
          if ((3 == (3 & arg2_731X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + arg2_731X))) + -4)))))) {
              v_735X = *((unsigned char *) ((Scode_pointerS) + 3));
              if ((0 == v_735X)) {
                goto L28401;}
              else {
                if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                  goto L28401;}
                else {
                  merged_arg0K0 = arg2_731X;
                  merged_arg0K1 = (((offset_733X)<<2));
                  merged_arg0K2 = value_732X;
                  proposal_d_write_return_tag = 0;
                  goto proposal_d_write;
                 proposal_d_write_return_0:
                  goto L28410;}}}
            else {
              goto L28411;}}
          else {
            goto L28411;}}
        else {
          goto L28411;}}
      else {
        goto L28411;}}
      break;
    case 100 : {
      arg2_736X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      init_737X = SvalS;
      type_738X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_736X))) {
        len_739X = ((arg2_736X)>>2);
        if ((len_739X < 0)) {
          goto L28644;}
        else {
          if ((4194304 < len_739X)) {
            goto L28644;}
          else {
            Stemp0S = init_737X;
            len_740X = ((len_739X)<<2);
            addr_741X = s48_allocate_tracedAgc((4 + len_740X));
            if ((addr_741X == NULL)) {
              arg0K0 = 1;
              goto L28661;}
            else {
              *((long *) addr_741X) = (2 + (((((((len_740X)<<6)) + type_738X))<<2)));
              arg0K0 = (3 + (((long) (addr_741X + 4))));
              goto L28661;}}}}
      else {
        code_742X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_742X)))))<<2)), code_742X, 20, 8);
        n_743X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_743X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((type_738X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_736X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = init_737X;
        arg0K0 = 3;
        goto L22895;}}
      break;
    case 101 : {
      arg2_744X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      index_745X = SvalS;
      type_746X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & index_745X))) {
        if ((3 == (3 & arg2_744X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg2_744X))) + -4))))>>2))) == type_746X)) {
            len_747X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_744X))) + -4))))>>8))))>>2);
            index_748X = ((index_745X)>>2);
            if ((index_748X < 0)) {
              goto L28982;}
            else {
              if ((index_748X < len_747X)) {
                v_749X = *((unsigned char *) ((Scode_pointerS) + 2));
                if ((0 == v_749X)) {
                  goto L28972;}
                else {
                  if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                    goto L28972;}
                  else {
                    merged_arg0K0 = arg2_744X;
                    merged_arg0K1 = index_745X;
                    proposal_d_read_return_tag = 0;
                    goto proposal_d_read;
                   proposal_d_read_return_0:
                    v_750X = proposal_d_read0_return_value;
                    arg0K0 = v_750X;
                    goto L28981;}}}
              else {
                goto L28982;}}}
          else {
            goto L28937;}}
        else {
          goto L28937;}}
      else {
        goto L28937;}}
      break;
    case 102 : {
      arg2_751X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_752X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      value_753X = SvalS;
      type_754X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_751X))) {
        if ((3 == (3 & arg3_752X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg3_752X))) + -4))))>>2))) == type_754X)) {
            if ((3 == (3 & arg3_752X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_752X))) + -4)))))) {
                len_755X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_752X))) + -4))))>>8))))>>2);
                index_756X = ((arg2_751X)>>2);
                if ((index_756X < 0)) {
                  goto L29301;}
                else {
                  if ((index_756X < len_755X)) {
                    v_757X = *((unsigned char *) ((Scode_pointerS) + 2));
                    if ((0 == v_757X)) {
                      goto L29291;}
                    else {
                      if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                        goto L29291;}
                      else {
                        merged_arg0K0 = arg3_752X;
                        merged_arg0K1 = arg2_751X;
                        merged_arg0K2 = value_753X;
                        proposal_d_write_return_tag = 1;
                        goto proposal_d_write;
                       proposal_d_write_return_1:
                        goto L29300;}}}
                  else {
                    goto L29301;}}}
              else {
                goto L29254;}}
            else {
              goto L29254;}}
          else {
            goto L29254;}}
        else {
          goto L29254;}}
      else {
        goto L29254;}}
      break;
    case 103 : {
      arg2_758X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (arg2_758X | (SvalS))))) {
        len_759X = ((arg2_758X)>>2);
        init_760X = (((SvalS))>>2);
        if ((len_759X < 0)) {
          goto L37887;}
        else {
          if ((4194304 < ((((3 + len_759X))>>2)))) {
            goto L37887;}
          else {
            addr_761X = s48_allocate_untracedAgc((4 + len_759X));
            if ((addr_761X == NULL)) {
              arg0K0 = 1;
              goto L37900;}
            else {
              *((long *) addr_761X) = (74 + (((len_759X)<<8)));
              arg0K0 = (3 + (((long) (addr_761X + 4))));
              goto L37900;}}}}
      else {
        code_762X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_762X)))))<<2)), code_762X, 20, 4);
        n_763X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_763X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_758X;
        x_764X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_764X;
        arg0K0 = 2;
        goto L22895;}}
      break;
    case 104 : {
      obj_765X = SvalS;
      if ((3 == (3 & obj_765X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + obj_765X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          goto L50007;}}
      else {
        goto L50007;}}
      break;
    case 105 : {
      arg2_766X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_766X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_766X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_767X = (((SvalS))>>2);
            len_768X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_766X))) + -4))))>>8);
            if ((index_767X < 0)) {
              goto L46214;}
            else {
              if ((index_767X < len_768X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_766X))) + index_767X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L25070;}
              else {
                goto L46214;}}}
          else {
            goto L49568;}}
        else {
          goto L49568;}}
      else {
        goto L49568;}}
      break;
    case 106 : {
      arg2_769X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_770X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_770X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_770X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_769X | (SvalS))))) {
            index_771X = ((arg2_769X)>>2);
            Kchar_772X = (((SvalS))>>2);
            if ((3 == (3 & arg3_770X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_770X))) + -4)))))) {
                len_773X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_770X))) + -4))))>>8);
                if ((index_771X < 0)) {
                  goto L43284;}
                else {
                  if ((index_771X < len_773X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_770X))) + index_771X)) = Kchar_772X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L25070;}
                  else {
                    goto L43284;}}}
              else {
                goto L43263;}}
            else {
              goto L43263;}}
          else {
            goto L48116;}}
        else {
          goto L48116;}}
      else {
        goto L48116;}}
      break;
    case 107 : {
      arg2_774X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_774X))) {
        if ((9 == (255 & (SvalS)))) {
          len_775X = ((arg2_774X)>>2);
          init_776X = ((((SvalS))>>8));
          if ((len_775X < 0)) {
            goto L38099;}
          else {
            if ((4194304 < ((((4 + len_775X))>>2)))) {
              goto L38099;}
            else {
              len_777X = 1 + len_775X;
              addr_778X = s48_allocate_untracedAgc((4 + len_777X));
              if ((addr_778X == NULL)) {
                arg0K0 = 1;
                goto L38112;}
              else {
                *((long *) addr_778X) = (70 + (((len_777X)<<8)));
                arg0K0 = (3 + (((long) (addr_778X + 4))));
                goto L38112;}}}}
        else {
          goto L46311;}}
      else {
        goto L46311;}}
      break;
    case 108 : {
      obj_779X = SvalS;
      if ((3 == (3 & obj_779X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_779X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          goto L50073;}}
      else {
        goto L50073;}}
      break;
    case 109 : {
      arg2_780X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_780X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_780X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_781X = (((SvalS))>>2);
            len_782X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_780X))) + -4))))>>8));
            if ((index_781X < 0)) {
              goto L46392;}
            else {
              if ((index_781X < len_782X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_780X))) + index_781X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L25070;}
              else {
                goto L46392;}}}
          else {
            goto L49682;}}
        else {
          goto L49682;}}
      else {
        goto L49682;}}
      break;
    case 110 : {
      arg2_783X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_784X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_784X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_784X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_783X))) {
            if ((9 == (255 & (SvalS)))) {
              index_785X = ((arg2_783X)>>2);
              Kchar_786X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_784X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_784X))) + -4)))))) {
                  len_787X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_784X))) + -4))))>>8));
                  if ((index_785X < 0)) {
                    goto L43471;}
                  else {
                    if ((index_785X < len_787X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_784X))) + index_785X)) = (Kchar_786X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L25070;}
                    else {
                      goto L43471;}}}
                else {
                  goto L43450;}}
              else {
                goto L43450;}}
            else {
              goto L48262;}}
          else {
            goto L48262;}}
        else {
          goto L48262;}}
      else {
        goto L48262;}}
      break;
    case 111 : {s48_make_availableAgc(12);
      obj_788X = SvalS;
      if ((3 == (3 & obj_788X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_788X))) + -4))))>>2))))) {
          table_789X = Sthe_symbol_tableS;
          string_790X = SvalS;
          n_791X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_790X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L21333;}
        else {
          goto L33943;}}
      else {
        goto L33943;}}
      break;
    case 112 : {
      obj_792X = SvalS;
      if ((3 == (3 & obj_792X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_792X))) + -4))))>>2))))) {
          x_793X = SvalS;
          descriptor_794X = *((long *) ((((char *) (-3 + x_793X))) + 4));
          if ((17 == (255 & descriptor_794X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_793X))) + 4))))) {
              arg0K0 = 5;
              goto L47746;}
            else {
              arg0K0 = 1;
              goto L47746;}}
          else {
            arg0K0 = 5;
            goto L47746;}}
        else {
          goto L47727;}}
      else {
        goto L47727;}}
      break;
    case 113 : {
      arg2_795X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_795X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_795X))) + -4))))>>2))))) {
          x_796X = SvalS;
          if ((1 == x_796X)) {
            goto L43029;}
          else {
            if ((5 == x_796X)) {
              goto L43029;}
            else {
              goto L43036;}}}
        else {
          goto L43036;}}
      else {
        goto L43036;}}
      break;
    case 114 : {
      x_797X = SvalS;
      if ((3 == (3 & x_797X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_797X))) + -4)))))) {
          arg0K0 = 1;
          goto L51005;}
        else {
          arg0K0 = 5;
          goto L51005;}}
      else {
        arg0K0 = 5;
        goto L51005;}}
      break;
    case 115 : {
      x_798X = SvalS;
      if ((3 == (3 & x_798X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_798X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_798X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_798X))) + -4))));
          goto L46490;}
        else {
          goto L46490;}}
      else {
        goto L46490;}}
      break;
    case 116 : {s48_make_availableAgc(288);
      arg2_799X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        mode_800X = (((SvalS))>>2);
        if ((1 == mode_800X)) {
          goto L38554;}
        else {
          if ((2 == mode_800X)) {
            goto L38554;}
          else {
            if ((3 == mode_800X)) {
              goto L38554;}
            else {
              if ((4 == mode_800X)) {
                goto L38554;}
              else {
                arg0K0 = 5;
                goto L38328;}}}}}
      else {
        code_801X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_801X)))))<<2)), code_801X, 20, 4);
        n_802X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_802X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_799X;
        x_803X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_803X;
        arg0K0 = 2;
        goto L22895;}}
      break;
    case 117 : {s48_make_availableAgc(264);
      obj_804X = SvalS;
      if ((3 == (3 & obj_804X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_804X))) + -4))))>>2))))) {
          channel_805X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_805X))))))) {
            code_806X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_806X)))))<<2)), code_806X, 20, 4);
            n_807X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_807X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 20;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = channel_805X;
            arg0K0 = 1;
            goto L22895;}
          else {
            status_808X = close_channelB(channel_805X);
            if ((status_808X == NO_ERRORS)) {
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L25070;}
            else {
              code_809X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_809X)))))<<2)), code_809X, 96, 4);
              n_810X = *((unsigned char *) (Scode_pointerS));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((n_810X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = 96;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = channel_805X;
              merged_arg0K0 = status_808X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 0;
              goto get_error_string;
             get_error_string_return_0:
              x_811X = get_error_string0_return_value;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = x_811X;
              arg0K0 = 2;
              goto L22895;}}}
        else {
          goto L48390;}}
      else {
        goto L48390;}}
      break;
    case 118 : {s48_make_availableAgc(264);
      arg2_812X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_813X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_814X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_815X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg5_815X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg5_815X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg3_813X | arg2_812X)))) {
            x_816X = SvalS;
            if ((1 == x_816X)) {
              goto L46633;}
            else {
              if ((5 == x_816X)) {
                goto L46633;}
              else {
                goto L46644;}}}
          else {
            goto L46644;}}
        else {
          goto L46644;}}
      else {
        goto L46644;}}
      break;
    case 119 : {s48_make_availableAgc(264);
      arg2_817X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_818X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_819X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg4_819X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg4_819X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_817X | (SvalS))))) {
            start_820X = ((arg2_817X)>>2);
            count_821X = (((SvalS))>>2);
            v_822X = 8 == (*((long *) (((char *) (-3 + arg4_819X)))));
            if (v_822X) {
              if ((3 == (3 & arg3_818X))) {
                if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_818X))) + -4))))>>2))))) {
                  goto L39153;}
                else {
                  goto L39360;}}
              else {
                goto L39360;}}
            else {
              goto L39207;}}
          else {
            goto L46863;}}
        else {
          goto L46863;}}
      else {
        goto L46863;}}
      break;
    case 120 : {s48_make_availableAgc(264);
      obj_823X = SvalS;
      if ((3 == (3 & obj_823X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_823X))) + -4))))>>2))))) {
          channel_824X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_824X))))))) {
            code_825X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_825X)))))<<2)), code_825X, 20, 4);
            n_826X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_826X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 20;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = channel_824X;
            arg0K0 = 1;
            goto L22895;}
          else {
            readyP_827X = ps_check_fd(((((*((long *) ((((char *) (-3 + channel_824X))) + 8))))>>2)), (4 == (*((long *) (((char *) (-3 + channel_824X)))))), &status_828X);
            if ((status_828X == NO_ERRORS)) {
              if (readyP_827X) {
                arg0K0 = 5;
                goto L42627;}
              else {
                arg0K0 = 1;
                goto L42627;}}
            else {
              code_829X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_829X)))))<<2)), code_829X, 96, 4);
              n_830X = *((unsigned char *) (Scode_pointerS));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((n_830X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = 96;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = channel_824X;
              merged_arg0K0 = status_828X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 1;
              goto get_error_string;
             get_error_string_return_1:
              x_831X = get_error_string0_return_value;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = x_831X;
              arg0K0 = 2;
              goto L22895;}}}
        else {
          goto L47808;}}
      else {
        goto L47808;}}
      break;
    case 121 : {
      obj_832X = SvalS;
      if ((3 == (3 & obj_832X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_832X))) + -4))))>>2))))) {
          channel_833X = SvalS;
          head_834X = Spending_channels_headS;
          if ((1 == head_834X)) {
            addr_835X = (((char *) (-3 + channel_833X))) + 16;S48_WRITE_BARRIER(channel_833X, addr_835X, 1);
            *((long *) addr_835X) = 1;
            n_836X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_833X))) + 8))))>>2)));
            arg0K0 = (((n_836X)<<2));
            goto L47027;}
          else {
            if ((channel_833X == head_834X)) {
              channel_837X = Spending_channels_headS;
              next_838X = *((long *) ((((char *) (-3 + channel_837X))) + 12));
              Spending_channels_headS = next_838X;
              addr_839X = (((char *) (-3 + channel_837X))) + 12;S48_WRITE_BARRIER(channel_837X, addr_839X, 1);
              *((long *) addr_839X) = 1;
              if ((1 == next_838X)) {
                Spending_channels_tailS = 1;
                goto L11393;}
              else {
                goto L11393;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_834X))) + 12)));
              arg0K1 = head_834X;
              goto L11399;}}}
        else {
          goto L47010;}}
      else {
        goto L47010;}}
      break;
    case 122 : {s48_make_availableAgc(((((3 * (Snumber_of_channelsS)))<<2)));
      arg0K0 = (-1 + (Snumber_of_channelsS));
      arg0K1 = 25;
      goto L17145;}
      break;
    case 123 : {
      SvalS = (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 124 : {
      proposal_840X = SvalS;
      if ((1 == proposal_840X)) {
        goto L39493;}
      else {
        if ((3 == (3 & proposal_840X))) {
          if ((2 == (31 & ((((*((long *) ((((char *) (-3 + proposal_840X))) + -4))))>>2))))) {
            if ((4 == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proposal_840X))) + -4))))>>8))))>>2)))) {
              if ((1 == (*((long *) (((char *) (-3 + proposal_840X))))))) {
                goto L39493;}
              else {
                goto L39524;}}
            else {
              goto L39524;}}
          else {
            goto L39524;}}
        else {
          goto L39524;}}}
      break;
    case 125 : {
      proposal_841X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
      if ((1 == proposal_841X)) {
        code_842X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_842X)))))<<2)), code_842X, 104, 4);
        n_843X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_843X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 104;
        arg0K0 = 0;
        goto L22895;}
      else {GET_PROPOSAL_LOCK();
        log_844X = *((long *) ((((char *) (-3 + proposal_841X))) + 4));
        arg0K0 = 0;
        goto L10171;}}
      break;
    case 126 : {
      stob_845X = SvalS;
      offset_846X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_847X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_845X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_845X))) + -4))))>>2))) == type_847X)) {
          x_848X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
          if ((1 == x_848X)) {
            arg0K0 = (*((long *) ((((char *) (-3 + stob_845X))) + (((offset_846X)<<2)))));
            goto L29631;}
          else {
            merged_arg0K0 = stob_845X;
            merged_arg0K1 = (((offset_846X)<<2));
            proposal_d_read_return_tag = 1;
            goto proposal_d_read;
           proposal_d_read_return_1:
            v_849X = proposal_d_read0_return_value;
            arg0K0 = v_849X;
            goto L29631;}}
        else {
          goto L29632;}}
      else {
        goto L29632;}}
      break;
    case 127 : {
      arg2_850X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_851X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_852X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_853X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & ((arg4_852X | arg2_850X) | (SvalS))))) {
        from_index_854X = ((arg4_852X)>>2);
        to_index_855X = ((arg2_850X)>>2);
        count_856X = (((SvalS))>>2);
        v_857X = *((unsigned char *) ((Scode_pointerS) + 1));
        merged_arg0K0 = arg5_853X;
        merged_arg0K1 = from_index_854X;
        merged_arg0K2 = count_856X;
        okay_copy_argsP_return_tag = 0;
        goto okay_copy_argsP;
       okay_copy_argsP_return_0:
        v_858X = okay_copy_argsP0_return_value;
        if (v_858X) {
          merged_arg0K0 = arg3_851X;
          merged_arg0K1 = to_index_855X;
          merged_arg0K2 = count_856X;
          okay_copy_argsP_return_tag = 1;
          goto okay_copy_argsP;
         okay_copy_argsP_return_1:
          v_859X = okay_copy_argsP0_return_value;
          if (v_859X) {
            if ((3 == (3 & arg3_851X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_851X))) + -4)))))) {
                if ((count_856X < 0)) {
                  goto L29741;}
                else {
                  if ((0 == v_857X)) {
                    goto L29801;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L29801;}
                    else {
                      arg0K0 = 4096;
                      arg0K1 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 12)));
                      goto L29905;}}}}
              else {
                goto L29741;}}
            else {
              goto L29741;}}
          else {
            goto L29741;}}
        else {
          goto L29741;}}
      else {
        code_860X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_860X)))))<<2)), code_860X, 20, 4);
        n_861X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_861X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg5_853X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_852X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_851X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_850X;
        x_862X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_862X;
        arg0K0 = 5;
        goto L22895;}}
      break;
    case 128 : {
      arg2_863X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_863X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_863X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_864X = (((SvalS))>>2);
            len_865X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_863X))) + -4))))>>8);
            if ((index_864X < 0)) {
              goto L30114;}
            else {
              if ((index_864X < len_865X)) {
                x_866X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                if ((1 == x_866X)) {
                  arg0K0 = ((((*((unsigned char *) ((((char *) (-3 + arg2_863X))) + index_864X))))<<2));
                  goto L30113;}
                else {
                  index_867X = ((index_864X)<<2);
                  log_868X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                  arg0K0 = 0;
                  goto L15248;}}
              else {
                goto L30114;}}}
          else {
            goto L33137;}}
        else {
          goto L33137;}}
      else {
        goto L33137;}}
      break;
    case 129 : {
      arg2_869X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_870X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_870X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_870X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_869X))) {
            index_871X = ((arg2_869X)>>2);
            byte_872X = SvalS;
            if ((3 == (3 & arg3_870X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_870X))) + -4)))))) {
                if ((0 == (3 & byte_872X))) {
                  len_873X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_870X))) + -4))))>>8);
                  if ((index_871X < 0)) {
                    goto L30256;}
                  else {
                    if ((index_871X < len_873X)) {
                      x_874X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                      if ((1 == x_874X)) {
                        *((unsigned char *) ((((char *) (-3 + arg3_870X))) + index_871X)) = (((byte_872X)>>2));
                        goto L30255;}
                      else {
                        index_875X = ((index_871X)<<2);
                        log_876X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                        arg0K0 = 0;
                        goto L15424;}}
                    else {
                      goto L30256;}}}
                else {
                  goto L30221;}}
              else {
                goto L30221;}}
            else {
              goto L30221;}}
          else {
            goto L33258;}}
        else {
          goto L33258;}}
      else {
        goto L33258;}}
      break;
    case 130 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 131 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 132 : {
      arg_877X = SvalS;
      code_878X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_878X)))))<<2)), code_878X, 60, 4);
      n_879X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_879X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 60;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg_877X;
      arg0K0 = 1;
      goto L22895;}
      break;
    case 133 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 134 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 135 : {s48_make_availableAgc(264);
      arg2_880X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_881X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_882X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg4_882X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_882X))) + -4))))>>2))))) {
          if ((3 == (3 & arg2_880X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_880X))) + -4))))>>2))))) {
              obj_883X = SvalS;
              if ((3 == (3 & obj_883X))) {
                if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_883X))) + -4))))>>2))))) {
                  undumpables_884X = SvalS;
                  port_885X = ps_open_output_file((((char *)(((char *) (-3 + arg4_882X))))), &status_886X);
                  if ((status_886X == NO_ERRORS)) {
                    status_887X = ps_write_string((((char *)(((char *) (-3 + arg2_880X))))), port_885X);
                    if ((status_887X == NO_ERRORS)) {
                      status_888X = s48_write_image(arg3_881X, undumpables_884X, port_885X);
                      if ((status_888X == NO_ERRORS)) {
                        status_889X = ps_close(port_885X);
                        if ((status_889X == NO_ERRORS)) {
                          SvalS = 13;
                          Scode_pointerS = ((Scode_pointerS) + 1);
                          arg1K0 = (Scode_pointerS);
                          goto L25070;}
                        else {
                          arg0K0 = 24;
                          arg0K1 = status_889X;
                          goto L43763;}}
                      else {
                        status_890X = ps_close(port_885X);
                        if ((status_890X == NO_ERRORS)) {
                          arg0K0 = 24;
                          arg0K1 = status_888X;
                          goto L43763;}
                        else {
                          ps_write_string("Unable to close image file", (stderr));
                          { long ignoreXX;
                          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                          arg0K0 = 24;
                          arg0K1 = status_888X;
                          goto L43763;}}}
                    else {
                      status_891X = ps_close(port_885X);
                      if ((status_891X == NO_ERRORS)) {
                        arg0K0 = 24;
                        arg0K1 = status_887X;
                        goto L43763;}
                      else {
                        ps_write_string("Unable to close image file", (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg0K0 = 24;
                        arg0K1 = status_887X;
                        goto L43763;}}}
                  else {
                    arg0K0 = 10;
                    arg0K1 = status_886X;
                    goto L43763;}}
                else {
                  goto L48495;}}
              else {
                goto L48495;}}
            else {
              goto L48495;}}
          else {
            goto L48495;}}
        else {
          goto L48495;}}
      else {
        goto L48495;}}
      break;
    case 136 : {
      SvalS = 13;s48_collect();
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 137 : {
      obj_892X = SvalS;
      if ((3 == (3 & obj_892X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_892X))) + -4))))>>2))))) {
          x_893X = SvalS;
          n_894X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_893X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L49248;}
        else {
          goto L49225;}}
      else {
        goto L49225;}}
      break;
    case 138 : {s48_make_availableAgc(24);
      arg2_895X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      proc_896X = SvalS;
      if ((3 == (3 & arg2_895X))) {
        if ((3 == (3 & proc_896X))) {
          if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_896X))) + -4))))>>2))))) {GET_PROPOSAL_LOCK();
            addr_897X = s48_allocate_small(12);
            *((long *) addr_897X) = 2050;
            x_898X = 3 + (((long) (addr_897X + 4)));
            *((long *) (((char *) (-3 + x_898X)))) = arg2_895X;
            *((long *) ((((char *) (-3 + x_898X))) + 4)) = proc_896X;
            b_899X = SHARED_REF((Sfinalizer_alistS));
            addr_900X = s48_allocate_small(12);
            *((long *) addr_900X) = 2050;
            x_901X = 3 + (((long) (addr_900X + 4)));
            *((long *) (((char *) (-3 + x_901X)))) = x_898X;
            *((long *) ((((char *) (-3 + x_901X))) + 4)) = b_899X;SHARED_SETB((Sfinalizer_alistS), x_901X);RELEASE_PROPOSAL_LOCK();
            SvalS = 13;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L25070;}
          else {
            goto L44054;}}
        else {
          goto L44054;}}
      else {
        goto L44054;}}
      break;
    case 139 : {
      arg2_902X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_902X))) {
        key_903X = ((arg2_902X)>>2);
        other_904X = SvalS;
        if ((5 == key_903X)) {
          SvalS = (-4 & other_904X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          if ((0 == key_903X)) {
            x_905X = s48_available();
            SvalS = (((x_905X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L25070;}
          else {
            if ((1 == key_903X)) {
              bytes_906X = s48_heap_size();
              SvalS = (-4 & (3 + bytes_906X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L25070;}
            else {
              if ((2 == key_903X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L25070;}
              else {
                if ((3 == key_903X)) {
                  x_907X = s48_gc_count();
                  SvalS = (((x_907X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L25070;}
                else {
                  if ((4 == key_903X)) {
                    code_908X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_908X)))))<<2)), code_908X, 56, 4);
                    n_909X = *((unsigned char *) (Scode_pointerS));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((n_909X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = 56;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((key_903X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_904X;
                    arg0K0 = 2;
                    goto L22895;}
                  else {
                    code_910X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_910X)))))<<2)), code_910X, 68, 4);
                    n_911X = *((unsigned char *) (Scode_pointerS));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((n_911X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = 68;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((key_903X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_904X;
                    arg0K0 = 2;
                    goto L22895;}}}}}}}
      else {
        code_912X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_912X)))))<<2)), code_912X, 20, 4);
        n_913X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_913X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_902X;
        x_914X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_914X;
        arg0K0 = 2;
        goto L22895;}}
      break;
    case 140 : {
      if ((0 == (3 & (SvalS)))) {
        type_915X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L39957;}
      else {
        code_916X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_916X)))))<<2)), code_916X, 20, 4);
        n_917X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_917X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_918X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_918X;
        arg0K0 = 1;
        goto L22895;}}
      break;
    case 141 : {
      type_919X = SvalS;
      arg2K0 = 1;
      arg0K1 = type_919X;
      goto L40045;}
      break;
    case 142 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 143 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 144 : {
      val_920X = SHARED_REF((Ssession_dataS));
      SvalS = val_920X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 145 : {SHARED_SETB((Ssession_dataS), (SvalS));
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L25070;}
      break;
    case 146 : {
      obj_921X = SvalS;
      if ((3 == (3 & obj_921X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_921X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 169)) {
            goto L27193;}
          else {
            temp_922X = SHARED_REF((Sexception_handlersS));SHARED_SETB((Sexception_handlersS), (SvalS));
            SvalS = temp_922X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L25070;}}
        else {
          goto L27193;}}
      else {
        goto L27193;}}
      break;
    case 147 : {
      data_923X = 3 + (((long) (SstackS)));
      SstackS = ((SstackS) + 20);
      code_924X = *((long *) ((((char *) (-3 + data_923X))) + 8));
      exception_925X = *((long *) ((((char *) (-3 + data_923X))) + 12));
      size_926X = *((long *) ((((char *) (-3 + data_923X))) + 16));
      pc_927X = (((*((long *) ((((char *) (-3 + data_923X))) + 4))))>>2);
      opcode_928X = *((unsigned char *) ((((char *) (-3 + code_924X))) + pc_927X));
      if ((opcode_928X < 43)) {
        if ((4 == opcode_928X)) {
          goto L25184;}
        else {
          Slast_code_calledS = code_924X;
          Scode_pointerS = ((((char *) (-3 + code_924X))) + pc_927X);
          code_929X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_929X)))))<<2)), code_929X, 112, 4);
          n_930X = *((unsigned char *) (Scode_pointerS));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_930X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 112;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = exception_925X;
          arg0K0 = 1;
          goto L22895;}}
      else {
        goto L25184;}}
      break;
    case 148 : {
      obj_931X = SvalS;
      if ((3 == (3 & obj_931X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_931X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 5)) {
            goto L25739;}
          else {
            temp_932X = SHARED_REF((Sinterrupt_handlersS));SHARED_SETB((Sinterrupt_handlersS), (SvalS));
            SvalS = temp_932X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L25070;}}
        else {
          goto L25739;}}
      else {
        goto L25739;}}
      break;
    case 149 : {
      old_933X = Senabled_interruptsS;
      p_934X = SvalS;
      Senabled_interruptsS = (((p_934X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Sstack_limitS = (Sreal_stack_limitS);
        if ((s48_Spending_eventsPS)) {
          s48_Sstack_limitS = (((char *) -1));
          goto L51118;}
        else {
          goto L51118;}}
      else {
        s48_Sstack_limitS = (((char *) -1));
        goto L51118;}}
      break;
    case 150 : {
      x_935X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      p_936X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      Senabled_interruptsS = (((p_936X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Sstack_limitS = (Sreal_stack_limitS);
        if ((s48_Spending_eventsPS)) {
          s48_Sstack_limitS = (((char *) -1));
          goto L8942;}
        else {
          goto L8942;}}
      else {
        s48_Sstack_limitS = (((char *) -1));
        goto L8942;}}
      break;
    case 151 : {
      if ((0 == (3 & (SvalS)))) {
        p_937X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          s48_Sstack_limitS = (Sreal_stack_limitS);
          if ((s48_Spending_eventsPS)) {
            s48_Sstack_limitS = (((char *) -1));
            goto L50242;}
          else {
            goto L50242;}}
        else {
          s48_Sstack_limitS = (((char *) -1));
          goto L50242;}}
      else {
        code_938X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_938X)))))<<2)), code_938X, 20, 4);
        n_939X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_939X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        x_940X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_940X;
        arg0K0 = 1;
        goto L22895;}}
      break;
    case 152 : {
      arg2_941X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_941X))) {
        x_942X = SvalS;
        if ((1 == x_942X)) {
          goto L47979;}
        else {
          if ((5 == x_942X)) {
            goto L47979;}
          else {
            goto L47984;}}}
      else {
        goto L47984;}}
      break;
    case 153 : {
      p_943X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      nargs_944X = ((p_943X)>>2);
      p_945X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      rest_list_946X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((14 < nargs_944X)) {
        code_947X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_947X)))))<<2)), code_947X, 76, 4);
        n_948X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_948X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 76;
        x_949X = *((long *) ((SstackS) + (-4 + (-4 & p_945X))));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_949X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = nargs_944X;
        arg0K0 = 2;
        goto L22895;}
      else {
        arg0K0 = rest_list_946X;
        goto L30487;}}
      break;
    case 154 : {s48_make_availableAgc(20);
      arg2_950X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_950X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_950X))) + -4))))>>2))))) {
          x_951X = SvalS;
          if ((1 == x_951X)) {
            goto L47220;}
          else {
            if ((5 == x_951X)) {
              goto L47220;}
            else {
              goto L47225;}}}
        else {
          goto L47225;}}
      else {
        goto L47225;}}
      break;
    case 155 : {
      arg2_952X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_952X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_952X))) + -4))))>>2))))) {
          x_953X = SvalS;
          if ((1 == x_953X)) {
            goto L32319;}
          else {
            if ((5 == x_953X)) {
              goto L32319;}
            else {
              goto L32324;}}}
        else {
          goto L32324;}}
      else {
        goto L32324;}}
      break;
    case 156 : {
      arg2_954X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_954X))) {
        option_955X = ((arg2_954X)>>2);
        other_956X = SvalS;
        if ((2 == option_955X)) {
          x_957X = CHEAP_TIME();
          SvalS = (((x_957X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          if ((0 == option_955X)) {
            seconds_958X = s48_run_time(&mseconds_959X);
            arg0K0 = option_955X;
            arg0K1 = seconds_958X;
            arg0K2 = mseconds_959X;
            goto L49143;}
          else {
            if ((1 == option_955X)) {
              seconds_960X = s48_real_time(&mseconds_961X);
              arg0K0 = option_955X;
              arg0K1 = seconds_960X;
              arg0K2 = mseconds_961X;
              goto L49143;}
            else {
              code_962X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_962X)))))<<2)), code_962X, 68, 4);
              n_963X = *((unsigned char *) (Scode_pointerS));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((n_963X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = 68;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((option_955X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = other_956X;
              arg0K0 = 2;
              goto L22895;}}}}
      else {
        code_964X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_964X)))))<<2)), code_964X, 20, 4);
        n_965X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_965X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_954X;
        x_966X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_966X;
        arg0K0 = 2;
        goto L22895;}}
      break;
    case 157 : {
      arg2_967X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_967X))) {
        key_968X = ((arg2_967X)>>2);
        value_969X = SvalS;
        status_970X = s48_extended_vm(key_968X, value_969X);
        if ((0 == status_970X)) {
          SvalS = (s48_Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          if ((1 == status_970X)) {
            code_971X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_971X)))))<<2)), code_971X, 88, 4);
            n_972X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_972X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 88;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((key_968X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_969X;
            arg0K0 = 2;
            goto L22895;}
          else {
            code_973X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_973X)))))<<2)), code_973X, 92, 4);
            n_974X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_974X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 92;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((key_968X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_969X;
            arg0K0 = 2;
            goto L22895;}}}
      else {
        code_975X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_975X)))))<<2)), code_975X, 20, 4);
        n_976X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_976X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_967X;
        x_977X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_977X;
        arg0K0 = 2;
        goto L22895;}}
      break;
    case 158 : {
      arg2_978X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_979X = SvalS;
      Senabled_interruptsS = -1;
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Sstack_limitS = (Sreal_stack_limitS);
        if ((s48_Spending_eventsPS)) {
          s48_Sstack_limitS = (((char *) -1));
          goto L51152;}
        else {
          goto L51152;}}
      else {
        s48_Sstack_limitS = (((char *) -1));
        goto L51152;}}
      break;
    case 159 : {
      arg2_980X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_980X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_980X))) + -4))))>>2))))) {
          obj_981X = SvalS;
          if ((3 == (3 & obj_981X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_981X))) + -4))))>>2))))) {
              x_982X = SvalS;
              len_983X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_980X))) + -4))))>>8);
              if ((len_983X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_982X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_982X))), (void *)(((char *) (-3 + arg2_980X))),len_983X)))) {
                  arg0K0 = 5;
                  goto L41167;}
                else {
                  arg0K0 = 1;
                  goto L41167;}}
              else {
                arg0K0 = 1;
                goto L41167;}}
            else {
              goto L41132;}}
          else {
            goto L41132;}}
        else {
          goto L41132;}}
      else {
        goto L41132;}}
      break;
    case 160 : {s48_make_availableAgc((4 + (-4 & (4 + ((((SvalS))>>2))))));
      arg2_984X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        n_985X = (((SvalS))>>2);
        if ((3 == (3 & arg2_984X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_984X))) + -4))))>>2))))) {
            goto L40335;}
          else {
            goto L40287;}}
        else {
          goto L40287;}}
      else {
        code_986X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_986X)))))<<2)), code_986X, 20, 4);
        n_987X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_987X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_984X;
        x_988X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_988X;
        arg0K0 = 2;
        goto L22895;}}
      break;
    case 161 : {
      arg2_989X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      list_990X = SvalS;
      arg0K0 = list_990X;
      arg0K1 = list_990X;
      arg2K2 = 1;
      goto L42706;}
      break;
    case 162 : {
      if ((529 == (SvalS))) {
        code_991X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_991X)))))<<2)), code_991X, 0, 4);
        n_992X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_992X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 0;
        arg0K0 = 0;
        goto L22895;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L25070;}}
      break;
    case 163 : {
      arg2_993X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_994X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        index_995X = (((SvalS))>>2);
        if ((3 == (3 & arg3_994X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg3_994X))) + -4))))>>2))))) {
            if ((arg2_993X == (*((long *) (((char *) (-3 + arg3_994X))))))) {
              len_996X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_994X))) + -4))))>>8))))>>2);
              if ((index_995X < 0)) {
                goto L30877;}
              else {
                if ((index_995X < len_996X)) {
                  v_997X = *((unsigned char *) ((Scode_pointerS) + 1));
                  if ((0 == v_997X)) {
                    goto L30867;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L30867;}
                    else {
                      merged_arg0K0 = arg3_994X;
                      merged_arg0K1 = (((index_995X)<<2));
                      proposal_d_read_return_tag = 2;
                      goto proposal_d_read;
                     proposal_d_read_return_2:
                      v_998X = proposal_d_read0_return_value;
                      arg0K0 = v_998X;
                      goto L30876;}}}
                else {
                  goto L30877;}}}
            else {
              goto L30897;}}
          else {
            goto L30897;}}
        else {
          goto L30897;}}
      else {
        code_999X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_999X)))))<<2)), code_999X, 20, 4);
        n_1000X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_1000X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_994X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_993X;
        x_1001X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_1001X;
        arg0K0 = 3;
        goto L22895;}}
      break;
    case 164 : {
      arg2_1002X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_1003X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_1004X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_1002X))) {
        index_1005X = ((arg2_1002X)>>2);
        value_1006X = SvalS;
        if ((3 == (3 & arg4_1004X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg4_1004X))) + -4))))>>2))))) {
            if ((arg3_1003X == (*((long *) (((char *) (-3 + arg4_1004X))))))) {
              if ((3 == (3 & arg4_1004X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_1004X))) + -4)))))) {
                  len_1007X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_1004X))) + -4))))>>8))))>>2);
                  if ((index_1005X < 0)) {
                    goto L31167;}
                  else {
                    if ((index_1005X < len_1007X)) {
                      v_1008X = *((unsigned char *) ((Scode_pointerS) + 1));
                      if ((0 == v_1008X)) {
                        goto L31157;}
                      else {
                        if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                          goto L31157;}
                        else {
                          merged_arg0K0 = arg4_1004X;
                          merged_arg0K1 = (((index_1005X)<<2));
                          merged_arg0K2 = value_1006X;
                          proposal_d_write_return_tag = 2;
                          goto proposal_d_write;
                         proposal_d_write_return_2:
                          goto L31166;}}}
                    else {
                      goto L31167;}}}
                else {
                  goto L31189;}}
              else {
                goto L31189;}}
            else {
              goto L31189;}}
          else {
            goto L31189;}}
        else {
          goto L31189;}}
      else {
        code_1009X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1009X)))))<<2)), code_1009X, 20, 4);
        n_1010X = *((unsigned char *) (Scode_pointerS));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_1010X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = 20;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_1004X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_1003X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_1002X;
        x_1011X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_1011X;
        arg0K0 = 4;
        goto L22895;}}
      break;
    case 165 : {
      v_1012X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_1012X)) {
        arg0K0 = (SvalS);
        goto L31478;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_1013X = get_current_port0_return_value;
        arg0K0 = v_1013X;
        goto L31478;}}
      break;
    case 166 : {
      v_1014X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_1014X)) {
        arg0K0 = (SvalS);
        goto L31728;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_1015X = get_current_port0_return_value;
        arg0K0 = v_1015X;
        goto L31728;}}
      break;
    case 167 : {
      v_1016X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_1016X)) {
        v_1017X = SvalS;
        v_1018X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_1018X;
        arg0K1 = v_1017X;
        goto L31971;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_1019X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_1019X;
        goto L31971;}}
      break;
    case 168 : {
      x_1020X = SvalS;
      out_1021X = stderr;
      arg0K0 = x_1020X;
      goto L42936;}
      break;
  }}
 L22697: {
  v_1022X = arg0K0;
  SvalS = v_1022X;
  pop_continuationB_return_tag = 2;
  goto pop_continuationB;
 pop_continuationB_return_2:
  arg0K0 = 1;
  goto L25558;}
 L23534: {
  stack_nargs_1023X = arg0K0;
  list_args_1024X = arg0K1;
  merged_arg0K0 = list_args_1024X;
  merged_arg0K1 = stack_nargs_1023X;
  pop_args_GlistSAgc_return_tag = 2;
  goto pop_args_GlistSAgc;
 pop_args_GlistSAgc_return_2:
  args_1025X = pop_args_GlistSAgc0_return_value;
  code_1026X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1026X)))))<<2)), code_1026X, 16, 0);
  n_1027X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1027X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 16;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 1;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = args_1025X;
  arg0K0 = 2;
  goto L22895;}
 L25558: {
  bytes_used_1028X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1028X));
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L25012: {
  v_1029X = arg0K0;
  SvalS = v_1029X;
  arg0K0 = 2;
  goto L24724;}
 L24724: {
  protocol_skip_1030X = arg0K0;
  SstackS = (ScontS);
  cont_1031X = ScontS;
  pointer_1032X = (((char *) (*((long *) cont_1031X)))) + -2;
  size_1033X = ((((*((unsigned char *) pointer_1032X)))<<8)) + (*((unsigned char *) (pointer_1032X + 1)));
  if ((65535 == size_1033X)) {
    arg0K0 = ((((*((long *) (cont_1031X + 4))))>>2));
    goto L24735;}
  else {
    arg0K0 = size_1033X;
    goto L24735;}}
 L22643: {
  if ((1 == (stack_nargs_520X + list_arg_count_522X))) {
    if ((1 == stack_nargs_520X)) {
      v_1034X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = v_1034X;
      goto L22721;}
    else {
      arg0K0 = (*((long *) (((char *) (-3 + list_args_521X)))));
      goto L22721;}}
  else {
    arg0K0 = stack_nargs_520X;
    arg0K1 = list_args_521X;
    goto L23534;}}
 L22760: {
  v_1035X = arg0K0;
  ScontS = (cont_529X + (4 + (((v_1035X)<<2))));
  merged_arg0K0 = stack_nargs_520X;
  move_args_above_contB_return_tag = 12;
  goto move_args_above_contB;
 move_args_above_contB_return_12:
  arg0K0 = stack_nargs_520X;
  arg0K1 = list_args_521X;
  arg0K2 = list_arg_count_522X;
  goto L32460;}
 L32460: {
  stack_arg_count_1036X = arg0K0;
  list_args_1037X = arg0K1;
  list_arg_count_1038X = arg0K2;
  if ((0 == list_arg_count_1038X)) {
    obj_1039X = SvalS;
    if ((3 == (3 & obj_1039X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1039X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_1036X;
        arg0K1 = 25;
        arg0K2 = 0;
        goto L26761;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_1036X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L23361;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_1036X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L23361;}}
  else {
    obj_1040X = SvalS;
    if ((3 == (3 & obj_1040X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1040X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_1036X;
        arg0K1 = list_args_1037X;
        arg0K2 = list_arg_count_1038X;
        goto L26761;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_1036X;
        arg0K2 = list_args_1037X;
        arg0K3 = list_arg_count_1038X;
        goto L23361;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_1036X;
      arg0K2 = list_args_1037X;
      arg0K3 = list_arg_count_1038X;
      goto L23361;}}}
 L26509: {
  loc_1041X = arg1K0;
  arg_1042X = arg1K1;
  if ((arg_1042X < arg_top_533X)) {
    SstackS = ((SstackS) + (0 - (((stack_nargs_520X)<<2))));
    if ((count_532X < stack_nargs_520X)) {
      merged_arg0K0 = list_args_521X;
      merged_arg0K1 = (stack_nargs_520X - count_532X);
      pop_args_GlistSAgc_return_tag = 3;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_3:
      v_1043X = pop_args_GlistSAgc0_return_value;
      arg0K0 = v_1043X;
      goto L26488;}
    else {
      arg0K0 = stack_nargs_520X;
      arg0K1 = list_args_521X;
      goto L26470;}}
  else {
    *((long *) loc_1041X) = (*((long *) arg_1042X));
    arg1K0 = (loc_1041X + -4);
    arg1K1 = (arg_1042X + -4);
    goto L26509;}}
 L26375: {
  count_1044X = arg0K0;
  bytes_used_1045X = arg0K1;
  stack_nargs_1046X = arg0K2;
  list_args_1047X = arg0K3;
  list_arg_count_1048X = arg0K4;
  if ((count_1044X == (stack_nargs_1046X + list_arg_count_1048X))) {
    arg_top_1049X = SstackS;
    pop_continuationB_return_tag = 3;
    goto pop_continuationB;
   pop_continuationB_return_3:
    arg1K0 = ((SstackS) + -4);
    arg1K1 = (arg_top_1049X + (-4 + (((stack_nargs_1046X)<<2))));
    goto L26407;}
  else {
    arg0K0 = stack_nargs_1046X;
    arg0K1 = list_args_1047X;
    goto L23534;}}
 L23192: {
  i_1050X = arg0K0;
  m_1051X = arg0K1;
  if ((0 == (n_534X & m_1051X))) {
    arg0K0 = (1 + i_1050X);
    arg0K1 = (((m_1051X)<<1));
    goto L23192;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_1051X));
    if ((i_1050X == 0)) {
      x_1052X = Sinterrupted_templateS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1052X;
      Sinterrupted_templateS = 1;
      n_1053X = Senabled_interruptsS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1053X)<<2));
      arg0K0 = 2;
      goto L23145;}
    else {
      if ((i_1050X == 2)) {
        x_1054X = Sfinalize_theseS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_1054X;
        Sfinalize_theseS = 25;
        n_1055X = Senabled_interruptsS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_1055X)<<2));
        arg0K0 = 2;
        goto L23145;}
      else {
        if ((i_1050X == 3)) {
          channel_1056X = Spending_channels_headS;
          next_1057X = *((long *) ((((char *) (-3 + channel_1056X))) + 12));
          Spending_channels_headS = next_1057X;
          addr_1058X = (((char *) (-3 + channel_1056X))) + 12;S48_WRITE_BARRIER(channel_1056X, addr_1058X, 1);
          *((long *) addr_1058X) = 1;
          if ((1 == next_1057X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_1056X;
            goto L11584;}
          else {
            arg0K0 = channel_1056X;
            goto L11584;}}
        else {
          if ((i_1050X == 4)) {
            x_1059X = Sos_signal_typeS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_1059X;
            x_1060X = Sos_signal_argumentS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_1060X;
            Sos_signal_typeS = 1;
            Sos_signal_argumentS = 1;
            n_1061X = Senabled_interruptsS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1061X)<<2));
            arg0K0 = 3;
            goto L23145;}
          else {
            n_1062X = Senabled_interruptsS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1062X)<<2));
            arg0K0 = 1;
            goto L23145;}}}}}}
 L8220: {
  new_env_1063X = arg0K0;
  i_1064X = arg0K1;
  offset_1065X = arg0K2;
  total_count_1066X = arg0K3;
  arg0K0 = (*((unsigned char *) ((Scode_pointerS) + (1 + offset_1065X))));
  arg0K1 = i_1064X;
  arg0K2 = (1 + offset_1065X);
  goto L8226;}
 L17415: {
  count_1067X = arg0K0;
  i_1068X = arg0K1;
  offset_1069X = arg0K2;
  if ((0 == count_1067X)) {
    arg0K0 = new_env_570X;
    arg0K1 = i_1068X;
    arg0K2 = offset_1069X;
    arg0K3 = total_count_566X;
    goto L8220;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + template_571X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1069X)))))<<2)))));
    merged_arg0K1 = new_env_570X;
    merged_arg0K2 = 0;
    make_closure_return_tag = 0;
    goto make_closure;
   make_closure_return_0:
    value_1070X = make_closure0_return_value;
    addr_1071X = (((char *) (-3 + new_env_570X))) + (((i_1068X)<<2));S48_WRITE_BARRIER(new_env_570X, addr_1071X, value_1070X);
    *((long *) addr_1071X) = value_1070X;
    arg0K0 = (-1 + count_1067X);
    arg0K1 = (1 + i_1068X);
    arg0K2 = (1 + offset_1069X);
    goto L17415;}}
 L10794: {
  new_env_1072X = arg0K0;
  i_1073X = arg0K1;
  offset_1074X = arg0K2;
  total_count_1075X = arg0K3;
  arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1074X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1074X)))));
  arg0K1 = i_1073X;
  arg0K2 = (2 + offset_1074X);
  goto L10800;}
 L17476: {
  count_1076X = arg0K0;
  i_1077X = arg0K1;
  offset_1078X = arg0K2;
  if ((0 == count_1076X)) {
    arg0K0 = new_env_576X;
    arg0K1 = i_1077X;
    arg0K2 = offset_1078X;
    arg0K3 = total_count_572X;
    goto L10794;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + template_577X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1078X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1078X))))))<<2)))));
    merged_arg0K1 = new_env_576X;
    merged_arg0K2 = 0;
    make_closure_return_tag = 1;
    goto make_closure;
   make_closure_return_1:
    value_1079X = make_closure0_return_value;
    addr_1080X = (((char *) (-3 + new_env_576X))) + (((i_1077X)<<2));S48_WRITE_BARRIER(new_env_576X, addr_1080X, value_1079X);
    *((long *) addr_1080X) = value_1079X;
    arg0K0 = (-1 + count_1076X);
    arg0K1 = (1 + i_1077X);
    arg0K2 = (2 + offset_1078X);
    goto L17476;}}
 L50668: {
  value_1081X = arg0K0;
  SvalS = value_1081X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L24907: {
  stack_arg_count_1082X = arg0K0;
  obj_1083X = SvalS;
  if ((3 == (3 & obj_1083X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1083X))) + -4))))>>2))))) {
      template_1084X = *((long *) (((char *) (-3 + (SvalS)))));
      code_1085X = *((long *) (((char *) (-3 + template_1084X))));
      protocol_1086X = *((unsigned char *) ((((char *) (-3 + code_1085X))) + 1));
      if ((protocol_1086X == stack_arg_count_1082X)) {
        arg0K0 = code_1085X;
        arg0K1 = 2;
        arg0K2 = 3;
        arg0K3 = template_1084X;
        goto L22456;}
      else {
        if (((127 & protocol_1086X) == stack_arg_count_1082X)) {
          proc_1087X = SvalS;
          merged_arg0K0 = stack_arg_count_1082X;
          move_args_above_contB_return_tag = 13;
          goto move_args_above_contB;
         move_args_above_contB_return_13:
          v_1088X = s48_call_native_procedure(proc_1087X, 2);
          arg0K0 = v_1088X;
          goto L24218;}
        else {
          arg0K0 = stack_arg_count_1082X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L26761;}}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_1082X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L23361;}}
  else {
    arg0K0 = 3;
    arg0K1 = stack_arg_count_1082X;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L23361;}}
 L24184: {
  obj_1089X = SvalS;
  if ((3 == (3 & obj_1089X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1089X))) + -4))))>>2))))) {
      arg0K0 = stack_arg_count_587X;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L26761;}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_587X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L23361;}}
  else {
    arg0K0 = 3;
    arg0K1 = stack_arg_count_587X;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L23361;}}
 L15729: {
  list_args_1090X = arg0K0;
  stack_nargs_1091X = arg0K1;
  merged_arg0K0 = list_args_1090X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_1092X = okay_argument_list0_return_value;
  list_arg_count_1093X = okay_argument_list1_return_value;
  if (okayP_1092X) {
    SvalS = proc_603X;
    arg0K0 = stack_nargs_1091X;
    arg0K1 = list_args_1090X;
    arg0K2 = list_arg_count_1093X;
    goto L32460;}
  else {
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = list_args_1090X;
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_1091X);
    pop_args_GlistSAgc_return_tag = 4;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_4:
    args_1094X = pop_args_GlistSAgc0_return_value;
    proc_1095X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    code_1096X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1096X)))))<<2)), code_1096X, 20, 0);
    n_1097X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1097X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 20;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = proc_1095X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1094X;
    arg0K0 = 2;
    goto L22895;}}
 L15774: {
  list_1098X = arg0K0;
  follower_1099X = arg0K1;
  if ((25 == list_1098X)) {
    list_args_1100X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_1099X))) + 4)))))));
    addr_1101X = (((char *) (-3 + follower_1099X))) + 4;S48_WRITE_BARRIER(follower_1099X, addr_1101X, list_args_1100X);
    *((long *) addr_1101X) = list_args_1100X;
    arg0K0 = rest_list_604X;
    arg0K1 = (-1 + stack_nargs_602X);
    goto L15729;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_1098X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_1099X))) + 4)));
    goto L15774;}}
 L24142: {
  obj_1102X = SvalS;
  if ((3 == (3 & obj_1102X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1102X))) + -4))))>>2))))) {
      arg0K0 = 0;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L26761;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L23361;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L23361;}}
 L24151: {
  SstackS = (Sbottom_of_stackS);
  Sheap_continuationS = cont_606X;
  ScontS = (Sbottom_of_stackS);
  goto L24142;}
 L40491: {
  code_pointer_1103X = ((char *) (*((long *) (ScontS))));
  protocol_1104X = *((unsigned char *) (code_pointer_1103X + 1));
  if ((1 == protocol_1104X)) {
    goto L40508;}
  else {
    if ((66 == protocol_1104X)) {
      goto L40508;}
    else {
      if ((129 == protocol_1104X)) {
        arg0K0 = 2;
        goto L24724;}
      else {
        if ((194 == protocol_1104X)) {
          arg0K0 = 2;
          goto L24724;}
        else {
          if ((71 == protocol_1104X)) {
            cont_1105X = Sheap_continuationS;
            if ((3 == (3 & cont_1105X))) {
              if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_1105X))) + -4))))>>2))))) {
                merged_arg0K0 = cont_1105X;
                merged_arg0K1 = 0;
                copy_continuation_from_heapB_return_tag = 2;
                goto copy_continuation_from_heapB;
               copy_continuation_from_heapB_return_2:
                goto L40491;}
              else {
                arg0K0 = cont_1105X;
                goto L23435;}}
            else {
              arg0K0 = cont_1105X;
              goto L23435;}}
          else {
            if ((70 == protocol_1104X)) {
              offset_1106X = ((((*((unsigned char *) (code_pointer_1103X + 2))))<<8)) + (*((unsigned char *) (code_pointer_1103X + 3)));
              proc_1107X = *((long *) ((ScontS) + 4));
              if ((0 == offset_1106X)) {
                cont_1108X = ScontS;
                pointer_1109X = (((char *) (*((long *) cont_1108X)))) + -2;
                size_1110X = ((((*((unsigned char *) pointer_1109X)))<<8)) + (*((unsigned char *) (pointer_1109X + 1)));
                if ((65535 == size_1110X)) {
                  arg0K0 = ((((*((long *) (cont_1108X + 4))))>>2));
                  goto L40650;}
                else {
                  arg0K0 = size_1110X;
                  goto L40650;}}
              else {
                ScontS = ((ScontS) + 4);
                *((long *) (ScontS)) = (((long) (code_pointer_1103X + offset_1106X)));
                goto L40555;}}
            else {
              if ((65 == protocol_1104X)) {
                wants_stack_args_1111X = ((((*((unsigned char *) (code_pointer_1103X + 2))))<<8)) + (*((unsigned char *) (code_pointer_1103X + 3)));
                if ((0 == wants_stack_args_1111X)) {
                  pop_continuationB_return_tag = 4;
                  goto pop_continuationB;
                 pop_continuationB_return_4:s48_make_availableAgc(12);
                  a_1112X = SvalS;
                  addr_1113X = s48_allocate_small(12);
                  *((long *) addr_1113X) = 2050;
                  x_1114X = 3 + (((long) (addr_1113X + 4)));
                  *((long *) (((char *) (-3 + x_1114X)))) = a_1112X;
                  *((long *) ((((char *) (-3 + x_1114X))) + 4)) = 25;
                  SstackS = ((SstackS) + -4);
                  *((long *) (SstackS)) = x_1114X;
                  Scode_pointerS = ((Scode_pointerS) + 4);
                  arg1K0 = (Scode_pointerS);
                  goto L25070;}
                else {
                  if ((1 == wants_stack_args_1111X)) {
                    pop_continuationB_return_tag = 5;
                    goto pop_continuationB;
                   pop_continuationB_return_5:
                    x_1115X = SvalS;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_1115X;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = 25;
                    Scode_pointerS = ((Scode_pointerS) + 4);
                    arg1K0 = (Scode_pointerS);
                    goto L25070;}
                  else {
                    x_1116X = SvalS;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_1116X;
                    arg0K0 = 1;
                    arg0K1 = 25;
                    goto L23534;}}}
              else {
                x_1117X = SvalS;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_1117X;
                arg0K0 = 1;
                arg0K1 = 25;
                goto L23534;}}}}}}}}
 L26639: {
  if ((nargs_613X == (*((unsigned char *) ((((char *) (-3 + code_612X))) + 1))))) {
    arg0K0 = code_612X;
    arg0K1 = 2;
    arg0K2 = 3;
    arg0K3 = template_611X;
    goto L22456;}
  else {
    v_1118X = *((unsigned char *) ((((char *) (-3 + code_612X))) + 1));
    if ((67 == v_1118X)) {
      if ((nargs_613X == (*((unsigned char *) ((((char *) (-3 + code_612X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_612X))) + -4))))>>8)))))))) {
        index_1119X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_612X))) + -4))))>>8));
        arg0K0 = code_612X;
        arg0K1 = 6;
        arg0K2 = template_611X;
        arg0K3 = (((((*((unsigned char *) ((((char *) (-3 + code_612X))) + index_1119X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_612X))) + (1 + index_1119X)))));
        goto L25568;}
      else {
        goto L26661;}}
    else {
      goto L26661;}}}
 L25427: {
  arg0K0 = (2 + (((max_617X)<<1)));
  goto L25429;}
 L25429: {
  offset_1120X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_1120X);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L26012: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L25325: {
  delta_1121X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_1121X);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L25903: {
  delta_1122X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_1122X);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L50725: {
  val_1123X = arg0K0;
  SvalS = val_1123X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L50737: {
  val_1124X = arg0K0;
  SvalS = val_1124X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L14221: {
  if ((3 == (3 & x_634X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + x_634X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L50737;}
    else {
      goto L14227;}}
  else {
    goto L14227;}}
 L41786: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L41787: {
  if ((3 == (3 & n_635X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_635X))) + -4))))>>2))))) {
      arg0K0 = n_635X;
      goto L23759;}
    else {
      goto L41791;}}
  else {
    goto L41791;}}
 L41935: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L41910: {
  if ((3 == (3 & n_636X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_636X))) + -4))))>>2))))) {
      goto L41935;}
    else {
      goto L41924;}}
  else {
    goto L41924;}}
 L42126: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L42101: {
  if ((3 == (3 & n_637X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_637X))) + -4))))>>2))))) {
      goto L42126;}
    else {
      goto L42115;}}
  else {
    goto L42115;}}
 L42317: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L42292: {
  if ((3 == (3 & n_638X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_638X))) + -4))))>>2))))) {
      goto L42317;}
    else {
      goto L42306;}}
  else {
    goto L42306;}}
 L34017: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L34018: {
  code_1125X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1125X)))))<<2)), code_1125X, 20, 4);
  n_1126X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1126X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_639X;
  arg0K0 = 1;
  goto L22895;}
 L23759: {
  x_1127X = arg0K0;
  code_1128X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1128X)))))<<2)), code_1128X, 20, 4);
  n_1129X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1129X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1127X;
  arg0K0 = 1;
  goto L22895;}
 L34159: {
  v_1130X = (char *) s48_long_to_bignum(x_642X);
  v_1131X = enter_bignum(v_1130X);
  arg0K0 = v_1131X;
  goto L34105;}
 L34105: {
  val_1132X = arg0K0;
  SvalS = val_1132X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L34111: {
  if ((0 == (3 & y_641X))) {
    goto L34117;}
  else {
    if ((3 == (3 & y_641X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_641X))) + -4))))>>2))))) {
        goto L34117;}
      else {
        goto L34120;}}
    else {
      goto L34120;}}}
 L34120: {
  if ((3 == (3 & arg2_640X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_640X))) + -4))))>>2))))) {
      if ((3 == (3 & y_641X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_641X))) + -4))))>>2))))) {
          Stemp0S = arg2_640X;
          Stemp1S = y_641X;s48_make_availableAgc(12);
          value_1133X = Stemp1S;
          Stemp1S = 1;
          y_1134X = *((double *) (((char *) (-3 + value_1133X))));
          value_1135X = Stemp0S;
          Stemp0S = 1;
          x_1136X = *((double *) (((char *) (-3 + value_1135X))));
          addr_1137X = s48_allocate_small(12);
          *((long *) addr_1137X) = 2126;
          Kdouble_1138X = 3 + (((long) (addr_1137X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1138X)))) = (x_1136X + y_1134X);
          SvalS = Kdouble_1138X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          goto L34134;}}
      else {
        goto L34134;}}
    else {
      goto L34134;}}
  else {
    goto L34134;}}
 L8385: {
  a_1139X = arg0K0;
  if ((b_646X < 0)) {
    arg0K0 = (0 - b_646X);
    goto L8389;}
  else {
    arg0K0 = b_646X;
    goto L8389;}}
 L44190: {
  if ((0 == (3 & y_644X))) {
    goto L44196;}
  else {
    if ((3 == (3 & y_644X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_644X))) + -4))))>>2))))) {
        goto L44196;}
      else {
        goto L44199;}}
    else {
      goto L44199;}}}
 L44199: {
  if ((3 == (3 & arg2_643X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_643X))) + -4))))>>2))))) {
      if ((3 == (3 & y_644X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_644X))) + -4))))>>2))))) {
          Stemp0S = arg2_643X;
          Stemp1S = y_644X;s48_make_availableAgc(12);
          value_1140X = Stemp1S;
          Stemp1S = 1;
          y_1141X = *((double *) (((char *) (-3 + value_1140X))));
          value_1142X = Stemp0S;
          Stemp0S = 1;
          x_1143X = *((double *) (((char *) (-3 + value_1142X))));
          addr_1144X = s48_allocate_small(12);
          *((long *) addr_1144X) = 2126;
          Kdouble_1145X = 3 + (((long) (addr_1144X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1145X)))) = (x_1143X * y_1141X);
          SvalS = Kdouble_1145X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          arg0K0 = arg2_643X;
          arg0K1 = y_644X;
          goto L23790;}}
      else {
        arg0K0 = arg2_643X;
        arg0K1 = y_644X;
        goto L23790;}}
    else {
      arg0K0 = arg2_643X;
      arg0K1 = y_644X;
      goto L23790;}}
  else {
    arg0K0 = arg2_643X;
    arg0K1 = y_644X;
    goto L23790;}}
 L34464: {
  v_1146X = (char *) s48_long_to_bignum(x_649X);
  v_1147X = enter_bignum(v_1146X);
  arg0K0 = v_1147X;
  goto L34410;}
 L34410: {
  val_1148X = arg0K0;
  SvalS = val_1148X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L34416: {
  if ((0 == (3 & y_648X))) {
    goto L34422;}
  else {
    if ((3 == (3 & y_648X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_648X))) + -4))))>>2))))) {
        goto L34422;}
      else {
        goto L34425;}}
    else {
      goto L34425;}}}
 L34425: {
  if ((3 == (3 & arg2_647X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_647X))) + -4))))>>2))))) {
      if ((3 == (3 & y_648X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_648X))) + -4))))>>2))))) {
          Stemp0S = arg2_647X;
          Stemp1S = y_648X;s48_make_availableAgc(12);
          value_1149X = Stemp1S;
          Stemp1S = 1;
          y_1150X = *((double *) (((char *) (-3 + value_1149X))));
          value_1151X = Stemp0S;
          Stemp0S = 1;
          x_1152X = *((double *) (((char *) (-3 + value_1151X))));
          addr_1153X = s48_allocate_small(12);
          *((long *) addr_1153X) = 2126;
          Kdouble_1154X = 3 + (((long) (addr_1153X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1154X)))) = (x_1152X - y_1150X);
          SvalS = Kdouble_1154X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L25070;}
        else {
          goto L34439;}}
      else {
        goto L34439;}}
    else {
      goto L34439;}}
  else {
    goto L34439;}}
 L23790: {
  x_1155X = arg0K0;
  y_1156X = arg0K1;
  code_1157X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1157X)))))<<2)), code_1157X, 20, 4);
  n_1158X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1158X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1155X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_1156X;
  arg0K0 = 2;
  goto L22895;}
 L8643: {
  a_1159X = arg0K0;
  if ((b_653X < 0)) {
    arg0K0 = (0 - b_653X);
    goto L8647;}
  else {
    arg0K0 = b_653X;
    goto L8647;}}
 L44433: {
  if ((0 == (3 & y_651X))) {
    goto L44439;}
  else {
    if ((3 == (3 & y_651X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_651X))) + -4))))>>2))))) {
        goto L44439;}
      else {
        goto L44466;}}
    else {
      goto L44466;}}}
 L44466: {
  if ((3 == (3 & arg2_650X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_650X))) + -4))))>>2))))) {
      if ((3 == (3 & y_651X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_651X))) + -4))))>>2))))) {
          if ((0. == (*((double *) (((char *) (-3 + y_651X))))))) {
            arg0K0 = 1;
            goto L44479;}
          else {
            Stemp0S = arg2_650X;
            Stemp1S = y_651X;s48_make_availableAgc(12);
            value_1160X = Stemp1S;
            Stemp1S = 1;
            y_1161X = *((double *) (((char *) (-3 + value_1160X))));
            value_1162X = Stemp0S;
            Stemp0S = 1;
            value_1163X = (*((double *) (((char *) (-3 + value_1162X))))) / y_1161X;
            addr_1164X = s48_allocate_small(12);
            *((long *) addr_1164X) = 2126;
            Kdouble_1165X = 3 + (((long) (addr_1164X + 4)));
            *((double *) (((char *) (-3 + Kdouble_1165X)))) = value_1163X;
            arg0K0 = Kdouble_1165X;
            goto L44479;}}
        else {
          arg0K0 = arg2_650X;
          arg0K1 = y_651X;
          goto L23790;}}
      else {
        arg0K0 = arg2_650X;
        arg0K1 = y_651X;
        goto L23790;}}
    else {
      arg0K0 = arg2_650X;
      arg0K1 = y_651X;
      goto L23790;}}
  else {
    arg0K0 = arg2_650X;
    arg0K1 = y_651X;
    goto L23790;}}
 L34711: {
  val_1166X = arg0K0;
  SvalS = val_1166X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L34717: {
  if ((0 == (3 & y_655X))) {
    goto L34723;}
  else {
    if ((3 == (3 & y_655X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_655X))) + -4))))>>2))))) {
        goto L34723;}
      else {
        goto L34728;}}
    else {
      goto L34728;}}}
 L34728: {
  if ((3 == (3 & arg2_654X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_654X))) + -4))))>>2))))) {
      if ((3 == (3 & y_655X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_655X))) + -4))))>>2))))) {
          b_1167X = (*((double *) (((char *) (-3 + arg2_654X))))) == (*((double *) (((char *) (-3 + y_655X)))));
          if (b_1167X) {
            arg0K0 = 5;
            goto L34743;}
          else {
            arg0K0 = 1;
            goto L34743;}}
        else {
          goto L34744;}}
      else {
        goto L34744;}}
    else {
      goto L34744;}}
  else {
    goto L34744;}}
 L34987: {
  val_1168X = arg0K0;
  SvalS = val_1168X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L34993: {
  if ((0 == (3 & y_657X))) {
    goto L34999;}
  else {
    if ((3 == (3 & y_657X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_657X))) + -4))))>>2))))) {
        goto L34999;}
      else {
        goto L35004;}}
    else {
      goto L35004;}}}
 L35004: {
  if ((3 == (3 & arg2_656X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_656X))) + -4))))>>2))))) {
      if ((3 == (3 & y_657X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_657X))) + -4))))>>2))))) {
          b_1169X = (*((double *) (((char *) (-3 + arg2_656X))))) < (*((double *) (((char *) (-3 + y_657X)))));
          if (b_1169X) {
            arg0K0 = 5;
            goto L35019;}
          else {
            arg0K0 = 1;
            goto L35019;}}
        else {
          goto L35020;}}
      else {
        goto L35020;}}
    else {
      goto L35020;}}
  else {
    goto L35020;}}
 L35326: {
  val_1170X = arg0K0;
  SvalS = val_1170X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35332: {
  if ((0 == (3 & y_659X))) {
    goto L35338;}
  else {
    if ((3 == (3 & y_659X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_659X))) + -4))))>>2))))) {
        goto L35338;}
      else {
        goto L35343;}}
    else {
      goto L35343;}}}
 L35343: {
  if ((3 == (3 & arg2_658X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_658X))) + -4))))>>2))))) {
      if ((3 == (3 & y_659X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_659X))) + -4))))>>2))))) {
          b_1171X = (*((double *) (((char *) (-3 + y_659X))))) < (*((double *) (((char *) (-3 + arg2_658X)))));
          if (b_1171X) {
            arg0K0 = 5;
            goto L35358;}
          else {
            arg0K0 = 1;
            goto L35358;}}
        else {
          goto L35359;}}
      else {
        goto L35359;}}
    else {
      goto L35359;}}
  else {
    goto L35359;}}
 L35665: {
  val_1172X = arg0K0;
  SvalS = val_1172X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35671: {
  if ((0 == (3 & y_661X))) {
    goto L35677;}
  else {
    if ((3 == (3 & y_661X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_661X))) + -4))))>>2))))) {
        goto L35677;}
      else {
        goto L35682;}}
    else {
      goto L35682;}}}
 L35682: {
  if ((3 == (3 & arg2_660X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_660X))) + -4))))>>2))))) {
      if ((3 == (3 & y_661X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_661X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + y_661X))))) < (*((double *) (((char *) (-3 + arg2_660X))))))) {
            arg0K0 = 1;
            goto L35697;}
          else {
            arg0K0 = 5;
            goto L35697;}}
        else {
          goto L35698;}}
      else {
        goto L35698;}}
    else {
      goto L35698;}}
  else {
    goto L35698;}}
 L35975: {
  val_1173X = arg0K0;
  SvalS = val_1173X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35981: {
  if ((0 == (3 & y_663X))) {
    goto L35987;}
  else {
    if ((3 == (3 & y_663X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_663X))) + -4))))>>2))))) {
        goto L35987;}
      else {
        goto L35992;}}
    else {
      goto L35992;}}}
 L35992: {
  if ((3 == (3 & arg2_662X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_662X))) + -4))))>>2))))) {
      if ((3 == (3 & y_663X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_663X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + arg2_662X))))) < (*((double *) (((char *) (-3 + y_663X))))))) {
            arg0K0 = 1;
            goto L36007;}
          else {
            arg0K0 = 5;
            goto L36007;}}
        else {
          goto L36008;}}
      else {
        goto L36008;}}
    else {
      goto L36008;}}
  else {
    goto L36008;}}
 L20032: {
  x_1174X = arg0K0;
  y_1175X = arg0K1;
  if ((0 == (3 & x_1174X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L20062;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1174X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L20062;}}
 L9257: {
  a_1176X = arg0K0;
  if ((b_667X < 0)) {
    arg0K0 = (0 - b_667X);
    goto L9261;}
  else {
    arg0K0 = b_667X;
    goto L9261;}}
 L36297: {
  if ((0 == (3 & y_665X))) {
    goto L36303;}
  else {
    if ((3 == (3 & y_665X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_665X))) + -4))))>>2))))) {
        goto L36303;}
      else {
        arg0K0 = arg2_664X;
        arg0K1 = y_665X;
        goto L23790;}}
    else {
      arg0K0 = arg2_664X;
      arg0K1 = y_665X;
      goto L23790;}}}
 L19941: {
  x_1177X = arg0K0;
  y_1178X = arg0K1;
  if ((0 == (3 & x_1177X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19971;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1177X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19971;}}
 L36477: {
  a_1179X = arg0K0;
  n_1180X = ((y_669X)>>2);
  if ((n_1180X < 0)) {
    arg0K0 = (0 - n_1180X);
    goto L36479;}
  else {
    arg0K0 = n_1180X;
    goto L36479;}}
 L36451: {
  if ((0 == (3 & y_669X))) {
    goto L36457;}
  else {
    if ((3 == (3 & y_669X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_669X))) + -4))))>>2))))) {
        goto L36457;}
      else {
        arg0K0 = arg2_668X;
        arg0K1 = y_669X;
        goto L23790;}}
    else {
      arg0K0 = arg2_668X;
      arg0K1 = y_669X;
      goto L23790;}}}
 L36616: {
  SvalS = n_671X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L36617: {
  code_1181X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1181X)))))<<2)), code_1181X, 20, 4);
  n_1182X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1182X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_671X;
  arg0K0 = 1;
  goto L22895;}
 L36687: {
  SvalS = n_672X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L36688: {
  code_1183X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1183X)))))<<2)), code_1183X, 20, 4);
  n_1184X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1184X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_672X;
  arg0K0 = 1;
  goto L22895;}
 L36758: {
  SvalS = 4;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L36761: {
  code_1185X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1185X)))))<<2)), code_1185X, 20, 4);
  n_1186X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1186X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_673X;
  arg0K0 = 1;
  goto L22895;}
 L36834: {
  SvalS = n_674X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L36835: {
  code_1187X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1187X)))))<<2)), code_1187X, 20, 4);
  n_1188X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1188X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_674X;
  arg0K0 = 1;
  goto L22895;}
 L36905: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L36908: {
  code_1189X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1189X)))))<<2)), code_1189X, 20, 4);
  n_1190X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1190X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_675X;
  arg0K0 = 1;
  goto L22895;}
 L50843: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L23669: {
  x_1191X = SvalS;
  if ((0 == (3 & x_1191X))) {s48_make_availableAgc(16);
    if ((x_1191X < 0)) {
      arg0K0 = (0 - x_1191X);
      goto L23697;}
    else {
      arg0K0 = x_1191X;
      goto L23697;}}
  else {
    if ((0 == (3 & x_1191X))) {
      arg0K0 = 1;
      arg0K1 = 3;
      goto L19549;}
    else {
      arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1191X))) + -4))))>>8))))>>2)));
      arg0K1 = 0;
      goto L19549;}}}
 L23672: {
  code_1192X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1192X)))))<<2)), code_1192X, 20, 4);
  n_1193X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1193X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1194X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1194X;
  arg0K0 = 1;
  goto L22895;}
 L37068: {
  if ((0 == (3 & x_682X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19896;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_682X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19896;}}
 L37071: {
  code_1195X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1195X)))))<<2)), code_1195X, 20, 4);
  n_1196X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1196X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_682X;
  arg0K0 = 1;
  goto L22895;}
 L37175: {
  x_1197X = arg0K0;
  arg0K0 = x_1197X;
  arg0K1 = 0;
  goto L37181;}
 L37158: {
  if ((0 == (3 & x_683X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19838;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_683X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19838;}}
 L37161: {
  code_1198X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1198X)))))<<2)), code_1198X, 20, 4);
  n_1199X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1199X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_683X;
  arg0K0 = 1;
  goto L22895;}
 L37276: {
  if ((0 == (3 & y_686X))) {
    goto L37282;}
  else {
    if ((3 == (3 & y_686X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_686X))) + -4))))>>2))))) {
        goto L37282;}
      else {
        goto L37285;}}
    else {
      goto L37285;}}}
 L37285: {
  code_1200X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1200X)))))<<2)), code_1200X, 20, 4);
  n_1201X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1201X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_685X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_686X;
  arg0K0 = 2;
  goto L22895;}
 L37455: {
  if ((0 == (3 & y_688X))) {
    goto L37461;}
  else {
    if ((3 == (3 & y_688X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_688X))) + -4))))>>2))))) {
        goto L37461;}
      else {
        goto L37464;}}
    else {
      goto L37464;}}}
 L37464: {
  code_1202X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1202X)))))<<2)), code_1202X, 20, 4);
  n_1203X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1203X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_687X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_688X;
  arg0K0 = 2;
  goto L22895;}
 L37634: {
  if ((0 == (3 & y_690X))) {
    goto L37640;}
  else {
    if ((3 == (3 & y_690X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_690X))) + -4))))>>2))))) {
        goto L37640;}
      else {
        goto L37643;}}
    else {
      goto L37643;}}}
 L37643: {
  code_1204X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1204X)))))<<2)), code_1204X, 20, 4);
  n_1205X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1205X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_689X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_690X;
  arg0K0 = 2;
  goto L22895;}
 L33749: {
  val_1206X = arg0K0;
  SvalS = val_1206X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L33765: {
  val_1207X = arg0K0;
  SvalS = val_1207X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L33766: {
  code_1208X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1208X)))))<<2)), code_1208X, 24, 4);
  n_1209X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1209X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 24;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_691X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_692X;
  arg0K0 = 2;
  goto L22895;}
 L44715: {
  if ((0 == (3 & y_692X))) {
    if ((0 == (3 & arg2_691X))) {
      value_1210X = ((arg2_691X)>>2);
      count_1211X = ((y_692X)>>2);
      if ((count_1211X < 0)) {
        PS_SHIFT_RIGHT(value_1210X, (0 - count_1211X), x_1212X)
        x_1213X = x_1212X;s48_make_availableAgc(16);
        if ((536870911 < x_1213X)) {
          goto L44823;}
        else {
          if ((x_1213X < -536870912)) {
            goto L44823;}
          else {
            arg0K0 = (((x_1213X)<<2));
            goto L44818;}}}
      else {
        PS_SHIFT_LEFT(value_1210X, count_1211X, x_1214X)
        result_1215X = x_1214X;
        PS_SHIFT_RIGHT(result_1215X, count_1211X, x_1216X)
        if ((value_1210X == x_1216X)) {
          if ((value_1210X < 0)) {
            if ((result_1215X < 0)) {s48_make_availableAgc(16);
              if ((536870911 < result_1215X)) {
                goto L44845;}
              else {
                if ((result_1215X < -536870912)) {
                  goto L44845;}
                else {
                  arg0K0 = (((result_1215X)<<2));
                  goto L44840;}}}
            else {
              arg0K0 = arg2_691X;
              arg0K1 = y_692X;
              goto L44728;}}
          else {
            if ((result_1215X < 0)) {
              arg0K0 = arg2_691X;
              arg0K1 = y_692X;
              goto L44728;}
            else {s48_make_availableAgc(16);
              if ((536870911 < result_1215X)) {
                goto L44867;}
              else {
                if ((result_1215X < -536870912)) {
                  goto L44867;}
                else {
                  arg0K0 = (((result_1215X)<<2));
                  goto L44862;}}}}}
        else {
          arg0K0 = arg2_691X;
          arg0K1 = y_692X;
          goto L44728;}}}
    else {
      if ((3 == (3 & arg2_691X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_691X))) + -4))))>>2))))) {
          y_1217X = ((y_692X)>>2);
          merged_arg0K0 = arg2_691X;
          merged_arg0K1 = y_1217X;
          shift_space_return_tag = 0;
          goto shift_space;
         shift_space_return_0:
          needed_1218X = shift_space0_return_value;
          Stemp0S = arg2_691X;s48_make_availableAgc((((needed_1218X)<<2)));
          value_1219X = Stemp0S;
          Stemp0S = 1;
          if ((0 == (3 & value_1219X))) {
            v_1220X = (char *) s48_long_to_bignum((((value_1219X)>>2)));
            arg1K0 = v_1220X;
            goto L44898;}
          else {
            arg1K0 = (((char *) (-3 + value_1219X)));
            goto L44898;}}
        else {
          arg0K0 = arg2_691X;
          arg0K1 = y_692X;
          goto L23790;}}
      else {
        arg0K0 = arg2_691X;
        arg0K1 = y_692X;
        goto L23790;}}}
  else {
    arg0K0 = arg2_691X;
    arg0K1 = y_692X;
    goto L23790;}}
 L50904: {
  val_1221X = arg0K0;
  SvalS = val_1221X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L41527: {
  val_1222X = arg0K0;
  SvalS = val_1222X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L41498: {
  code_1223X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1223X)))))<<2)), code_1223X, 20, 4);
  n_1224X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1224X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_698X;
  x_1225X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1225X;
  arg0K0 = 2;
  goto L22895;}
 L41419: {
  val_1226X = arg0K0;
  SvalS = val_1226X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L41390: {
  code_1227X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1227X)))))<<2)), code_1227X, 20, 4);
  n_1228X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1228X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_700X;
  x_1229X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1229X;
  arg0K0 = 2;
  goto L22895;}
 L37804: {
  code_1230X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1230X)))))<<2)), code_1230X, 20, 4);
  n_1231X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1231X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((x_705X)<<2));
  arg0K0 = 1;
  goto L22895;}
 L50924: {
  val_1232X = arg0K0;
  SvalS = val_1232X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L50938: {
  value_1233X = arg0K0;
  SvalS = value_1233X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L27861: {
  code_1234X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1234X)))))<<2)), code_1234X, 20, 8);
  n_1235X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1235X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_712X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_713X)<<2));
  arg0K0 = 2;
  goto L22895;}
 L27982: {
  SvalS = new_718X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L27966: {
  i_1236X = arg0K0;
  if ((i_1236X < 0)) {
    goto L27982;}
  else {
    value_1237X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_718X))) + (((i_1236X)<<2)))) = value_1237X;
    arg0K0 = (-1 + i_1236X);
    goto L27966;}}
 L28100: {
  i_1238X = arg0K0;
  if ((i_1238X < 0)) {
    arg0K0 = stack_nargs_726X;
    arg0K1 = rest_list_727X;
    goto L28242;}
  else {
    value_1239X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_724X))) + (((i_1238X)<<2)))) = value_1239X;
    arg0K0 = (-1 + i_1238X);
    goto L28100;}}
 L28268: {
  code_1240X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1240X)))))<<2)), code_1240X, 20, 12);
  n_1241X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1241X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_728X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_730X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_729X)<<2));
  arg0K0 = 3;
  goto L22895;}
 L28401: {
  addr_1242X = (((char *) (-3 + arg2_731X))) + (((offset_733X)<<2));S48_WRITE_BARRIER(arg2_731X, addr_1242X, value_732X);
  *((long *) addr_1242X) = value_732X;
  goto L28410;}
 L28410: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L28411: {
  code_1243X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1243X)))))<<2)), code_1243X, 20, 16);
  n_1244X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1244X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_731X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_734X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_733X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_732X;
  arg0K0 = 4;
  goto L22895;}
 L28644: {
  code_1245X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1245X)))))<<2)), code_1245X, 20, 8);
  n_1246X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1246X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_738X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_739X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = init_737X;
  arg0K0 = 3;
  goto L22895;}
 L28661: {
  x_1247X = arg0K0;
  value_1248X = Stemp0S;
  Stemp0S = 1;
  if ((1 == x_1247X)) {
    code_1249X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1249X)))))<<2)), code_1249X, 32, 8);
    n_1250X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1250X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 32;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((type_738X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_739X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = value_1248X;
    arg0K0 = 3;
    goto L22895;}
  else {
    arg0K0 = (-1 + len_739X);
    goto L28687;}}
 L28982: {
  code_1251X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1251X)))))<<2)), code_1251X, 28, 12);
  n_1252X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1252X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_744X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_746X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_745X;
  arg0K0 = 3;
  goto L22895;}
 L28972: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg2_744X))) + (-4 & index_745X))));
  goto L28981;}
 L28981: {
  value_1253X = arg0K0;
  SvalS = value_1253X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L28937: {
  code_1254X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1254X)))))<<2)), code_1254X, 20, 12);
  n_1255X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1255X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_744X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_746X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_745X;
  arg0K0 = 3;
  goto L22895;}
 L29301: {
  code_1256X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1256X)))))<<2)), code_1256X, 28, 12);
  n_1257X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1257X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_752X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_754X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_751X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_753X;
  arg0K0 = 4;
  goto L22895;}
 L29291: {
  addr_1258X = (((char *) (-3 + arg3_752X))) + (-4 & arg2_751X);S48_WRITE_BARRIER(arg3_752X, addr_1258X, value_753X);
  *((long *) addr_1258X) = value_753X;
  goto L29300;}
 L29300: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L29254: {
  code_1259X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1259X)))))<<2)), code_1259X, 20, 12);
  n_1260X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1260X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_752X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_754X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_751X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_753X;
  arg0K0 = 4;
  goto L22895;}
 L37887: {
  code_1261X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1261X)))))<<2)), code_1261X, 20, 4);
  n_1262X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1262X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_759X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_760X)<<2));
  arg0K0 = 2;
  goto L22895;}
 L37900: {
  vector_1263X = arg0K0;
  if ((1 == vector_1263X)) {
    code_1264X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1264X)))))<<2)), code_1264X, 32, 4);
    n_1265X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1265X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 32;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_759X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((init_760X)<<2));
    arg0K0 = 2;
    goto L22895;}
  else {
    arg0K0 = (-1 + len_759X);
    goto L37922;}}
 L50007: {
  code_1266X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1266X)))))<<2)), code_1266X, 20, 4);
  n_1267X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1267X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1268X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1268X;
  arg0K0 = 1;
  goto L22895;}
 L46214: {
  code_1269X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1269X)))))<<2)), code_1269X, 28, 4);
  n_1270X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1270X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_766X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_767X)<<2));
  arg0K0 = 2;
  goto L22895;}
 L49568: {
  code_1271X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1271X)))))<<2)), code_1271X, 20, 4);
  n_1272X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1272X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_766X;
  x_1273X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1273X;
  arg0K0 = 2;
  goto L22895;}
 L43284: {
  code_1274X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1274X)))))<<2)), code_1274X, 28, 4);
  n_1275X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1275X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_770X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_771X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_772X)<<2));
  arg0K0 = 3;
  goto L22895;}
 L43263: {
  code_1276X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1276X)))))<<2)), code_1276X, 20, 4);
  n_1277X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1277X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_770X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_771X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_772X)<<2));
  arg0K0 = 3;
  goto L22895;}
 L48116: {
  code_1278X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1278X)))))<<2)), code_1278X, 20, 4);
  n_1279X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1279X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_770X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_769X;
  x_1280X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1280X;
  arg0K0 = 3;
  goto L22895;}
 L38099: {
  code_1281X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1281X)))))<<2)), code_1281X, 20, 4);
  n_1282X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1282X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_775X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_776X))<<8)));
  arg0K0 = 2;
  goto L22895;}
 L38112: {
  vector_1283X = arg0K0;
  if ((1 == vector_1283X)) {
    code_1284X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1284X)))))<<2)), code_1284X, 32, 4);
    n_1285X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1285X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 32;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_775X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (9 + ((((init_776X))<<8)));
    arg0K0 = 2;
    goto L22895;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1283X))) + len_775X)) = 0;
    arg0K0 = (-1 + len_775X);
    goto L38134;}}
 L46311: {
  code_1286X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1286X)))))<<2)), code_1286X, 20, 4);
  n_1287X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1287X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_774X;
  x_1288X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1288X;
  arg0K0 = 2;
  goto L22895;}
 L50073: {
  code_1289X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1289X)))))<<2)), code_1289X, 20, 4);
  n_1290X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1290X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1291X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1291X;
  arg0K0 = 1;
  goto L22895;}
 L46392: {
  code_1292X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1292X)))))<<2)), code_1292X, 28, 4);
  n_1293X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1293X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_780X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_781X)<<2));
  arg0K0 = 2;
  goto L22895;}
 L49682: {
  code_1294X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1294X)))))<<2)), code_1294X, 20, 4);
  n_1295X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1295X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_780X;
  x_1296X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1296X;
  arg0K0 = 2;
  goto L22895;}
 L43471: {
  code_1297X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1297X)))))<<2)), code_1297X, 28, 4);
  n_1298X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1298X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_784X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_785X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_786X))<<8)));
  arg0K0 = 3;
  goto L22895;}
 L43450: {
  code_1299X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1299X)))))<<2)), code_1299X, 20, 4);
  n_1300X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1300X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_784X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_785X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_786X))<<8)));
  arg0K0 = 3;
  goto L22895;}
 L48262: {
  code_1301X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1301X)))))<<2)), code_1301X, 20, 4);
  n_1302X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1302X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_784X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_783X;
  x_1303X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1303X;
  arg0K0 = 3;
  goto L22895;}
 L21333: {
  i_1304X = arg0K0;
  h_1305X = arg0K1;
  if ((i_1304X < n_791X)) {
    arg0K0 = (1 + i_1304X);
    arg0K1 = (h_1305X + (((*((unsigned char *) ((((char *) (-3 + string_790X))) + i_1304X))))));
    goto L21333;}
  else {
    index_1306X = 1023 & h_1305X;
    link_1307X = *((long *) ((((char *) (-3 + table_789X))) + (((index_1306X)<<2))));
    if ((0 == (3 & link_1307X))) {
      arg0K0 = (3 + (-4 & link_1307X));
      goto L21289;}
    else {
      arg0K0 = link_1307X;
      goto L21289;}}}
 L33943: {
  code_1308X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1308X)))))<<2)), code_1308X, 20, 4);
  n_1309X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1309X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1310X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1310X;
  arg0K0 = 1;
  goto L22895;}
 L47746: {
  val_1311X = arg0K0;
  SvalS = val_1311X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L47727: {
  code_1312X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1312X)))))<<2)), code_1312X, 20, 4);
  n_1313X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1313X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1314X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1314X;
  arg0K0 = 1;
  goto L22895;}
 L43029: {
  if ((1 == (SvalS))) {
    addr_1315X = (((char *) (-3 + arg2_795X))) + 4;S48_WRITE_BARRIER(arg2_795X, addr_1315X, 273);
    *((long *) addr_1315X) = 273;
    goto L43035;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_795X))) + 4)))))) {
      addr_1316X = (((char *) (-3 + arg2_795X))) + 4;S48_WRITE_BARRIER(arg2_795X, addr_1316X, 529);
      *((long *) addr_1316X) = 529;
      goto L43035;}
    else {
      goto L43035;}}}
 L43036: {
  code_1317X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1317X)))))<<2)), code_1317X, 20, 4);
  n_1318X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1318X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_795X;
  x_1319X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1319X;
  arg0K0 = 2;
  goto L22895;}
 L51005: {
  val_1320X = arg0K0;
  SvalS = val_1320X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L46490: {
  SvalS = x_798X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L38554: {
  if ((0 == (3 & arg2_799X))) {
    if (((((arg2_799X)>>2)) < 0)) {
      arg0K0 = 5;
      goto L38328;}
    else {
      arg0K0 = (((arg2_799X)>>2));
      goto L38358;}}
  else {
    if ((3 == (3 & arg2_799X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_799X))) + -4))))>>2))))) {
        if ((1 == mode_800X)) {
          goto L38416;}
        else {
          if ((3 == mode_800X)) {
            goto L38416;}
          else {
            v_1321X = ps_open_fd((((char *)(((char *) (-3 + arg2_799X))))), 0, &v_1322X);
            arg0K0 = v_1321X;
            arg0K1 = v_1322X;
            goto L38429;}}}
      else {
        arg0K0 = 5;
        goto L38328;}}
    else {
      arg0K0 = 5;
      goto L38328;}}}
 L38328: {
  reason_1323X = arg0K0;
  code_1324X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1324X)))))<<2)), code_1324X, (((reason_1323X)<<2)), 4);
  n_1325X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1325X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((reason_1323X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_799X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((mode_800X)<<2));
  arg0K0 = 2;
  goto L22895;}
 L48390: {
  code_1326X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1326X)))))<<2)), code_1326X, 20, 4);
  n_1327X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1327X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1328X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1328X;
  arg0K0 = 1;
  goto L22895;}
 L46633: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L46643;}
  else {
    arg2K0 = 1;
    goto L46643;}}
 L46644: {
  code_1329X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1329X)))))<<2)), code_1329X, 20, 4);
  n_1330X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1330X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_815X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_814X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_813X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_812X;
  x_1331X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1331X;
  arg0K0 = 5;
  goto L22895;}
 L39153: {
  if ((3 == (3 & arg3_818X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_818X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_818X))) + -4))))>>8)));
      goto L39155;}
    else {
      goto L39374;}}
  else {
    goto L39374;}}
 L39360: {
  if ((3 == (3 & arg3_818X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_818X))) + -4))))>>2))))) {
      goto L39153;}
    else {
      goto L39207;}}
  else {
    goto L39207;}}
 L39207: {
  code_1332X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1332X)))))<<2)), code_1332X, 20, 4);
  n_1333X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1333X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_819X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_818X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_820X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_821X)<<2));
  arg0K0 = 4;
  goto L22895;}
 L46863: {
  code_1334X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1334X)))))<<2)), code_1334X, 20, 4);
  n_1335X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1335X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_819X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_818X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_817X;
  x_1336X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1336X;
  arg0K0 = 4;
  goto L22895;}
 L42627: {
  val_1337X = arg0K0;
  SvalS = val_1337X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L47808: {
  code_1338X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1338X)))))<<2)), code_1338X, 20, 4);
  n_1339X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1339X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1340X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1340X;
  arg0K0 = 1;
  goto L22895;}
 L47027: {
  val_1341X = arg0K0;
  SvalS = val_1341X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L11393: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_833X))) + 16)));
  goto L47027;}
 L11399: {
  ch_1342X = arg0K0;
  prev_1343X = arg0K1;
  if ((1 == ch_1342X)) {
    addr_1344X = (((char *) (-3 + channel_833X))) + 16;S48_WRITE_BARRIER(channel_833X, addr_1344X, 1);
    *((long *) addr_1344X) = 1;
    n_1345X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_833X))) + 8))))>>2)));
    arg0K0 = (((n_1345X)<<2));
    goto L47027;}
  else {
    if ((ch_1342X == channel_833X)) {
      y_1346X = Spending_channels_tailS;
      if ((ch_1342X == y_1346X)) {
        Spending_channels_tailS = prev_1343X;
        goto L11423;}
      else {
        goto L11423;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_1342X))) + 12)));
      arg0K1 = ch_1342X;
      goto L11399;}}}
 L47010: {
  code_1347X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1347X)))))<<2)), code_1347X, 20, 4);
  n_1348X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1348X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1349X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1349X;
  arg0K0 = 1;
  goto L22895;}
 L17145: {
  i_1350X = arg0K0;
  res_1351X = arg0K1;
  if ((-1 == i_1350X)) {
    SvalS = res_1351X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    channel_1352X = *((Svm_channelsS) + i_1350X);
    if ((3 == (3 & channel_1352X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_1352X))) + -4))))>>2))))) {
        addr_1353X = s48_allocate_small(12);
        *((long *) addr_1353X) = 2050;
        x_1354X = 3 + (((long) (addr_1353X + 4)));
        *((long *) (((char *) (-3 + x_1354X)))) = channel_1352X;
        *((long *) ((((char *) (-3 + x_1354X))) + 4)) = res_1351X;
        arg0K0 = x_1354X;
        goto L17159;}
      else {
        arg0K0 = res_1351X;
        goto L17159;}}
    else {
      arg0K0 = res_1351X;
      goto L17159;}}}
 L39493: {
  old_1355X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 == old_1355X)) {
    goto L39509;}
  else {
    addr_1356X = ((char *) (-3 + old_1355X));S48_WRITE_BARRIER(old_1355X, addr_1356X, 1);
    *((long *) addr_1356X) = 1;
    goto L39509;}}
 L39524: {
  code_1357X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1357X)))))<<2)), code_1357X, 20, 4);
  n_1358X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1358X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proposal_840X;
  arg0K0 = 1;
  goto L22895;}
 L10171: {
  i_1359X = arg0K0;
  stob_1360X = *((long *) ((((char *) (-3 + log_844X))) + (((i_1359X)<<2))));
  if ((1 == stob_1360X)) {
    log_1361X = *((long *) ((((char *) (-3 + proposal_841X))) + 8));
    arg0K0 = 0;
    goto L10409;}
  else {
    verify_1362X = *((long *) ((((char *) (-3 + log_844X))) + (12 + (((i_1359X)<<2)))));
    value_1363X = *((long *) ((((char *) (-3 + log_844X))) + (8 + (((i_1359X)<<2)))));
    if ((29 == verify_1362X)) {
      if ((3 == (3 & stob_1360X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1360X))) + -4)))))) {
          goto L10223;}
        else {
          goto L39771;}}
      else {
        goto L39771;}}
    else {
      if ((verify_1362X == (*((long *) ((((char *) (-3 + stob_1360X))) + (-4 & (*((long *) ((((char *) (-3 + log_844X))) + (4 + (((i_1359X)<<2)))))))))))) {
        if ((verify_1362X == value_1363X)) {
          goto L10223;}
        else {
          if ((3 == (3 & stob_1360X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1360X))) + -4)))))) {
              goto L10223;}
            else {
              goto L39771;}}
          else {
            goto L39771;}}}
      else {
        goto L39771;}}}}
 L29631: {
  value_1364X = arg0K0;
  SvalS = value_1364X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L29632: {
  code_1365X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1365X)))))<<2)), code_1365X, 20, 12);
  n_1366X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1366X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_845X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_847X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_846X)<<2));
  arg0K0 = 3;
  goto L22895;}
 L29741: {
  code_1367X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1367X)))))<<2)), code_1367X, 20, 8);
  n_1368X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1368X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_853X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_854X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_851X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_855X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_856X)<<2));
  arg0K0 = 5;
  goto L22895;}
 L29801: {
  memcpy((void *)((((char *) (-3 + arg3_851X))) + to_index_855X), (void *)((((char *) (-3 + arg5_853X))) + from_index_854X),count_856X);
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L29905: {
  left_1369X = arg0K0;
  copies_1370X = arg0K1;
  if ((1 == copies_1370X)) {
    if ((left_1369X < count_856X)) {
      goto L29741;}
    else {
      from_index_1371X = ((from_index_854X)<<2);
      to_index_1372X = ((to_index_855X)<<2);
      count_1373X = ((count_856X)<<2);
      Stemp0S = arg5_853X;
      Stemp1S = arg3_851X;
      addr_1374X = s48_allocate_tracedAgc(28);
      if ((addr_1374X == NULL)) {
        arg0K0 = 1;
        goto L9737;}
      else {
        *((long *) addr_1374X) = 6154;
        arg0K0 = (3 + (((long) (addr_1374X + 4))));
        goto L9737;}}}
  else {
    arg0K0 = (left_1369X - ((((*((long *) ((((char *) (-3 + copies_1370X))) + 16))))>>2)));
    arg0K1 = (*((long *) ((((char *) (-3 + copies_1370X))) + 20)));
    goto L29905;}}
 L30114: {
  code_1375X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1375X)))))<<2)), code_1375X, 28, 4);
  n_1376X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1376X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_863X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_864X)<<2));
  arg0K0 = 2;
  goto L22895;}
 L30113: {
  value_1377X = arg0K0;
  SvalS = value_1377X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L15248: {
  i_1378X = arg0K0;
  next_stob_1379X = *((long *) ((((char *) (-3 + log_868X))) + (((i_1378X)<<2))));
  if ((1 == next_stob_1379X)) {
    v_1380X = add_log_entryAgc(2, i_1378X, arg2_863X, index_867X, ((((*((unsigned char *) ((((char *) (-3 + arg2_863X))) + (((index_867X)>>2))))))<<2)), 1);
    arg0K0 = v_1380X;
    goto L30113;}
  else {
    if ((arg2_863X == next_stob_1379X)) {
      if ((index_867X == (*((long *) ((((char *) (-3 + log_868X))) + (4 + (((i_1378X)<<2)))))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + log_868X))) + (8 + (((i_1378X)<<2))))));
        goto L30113;}
      else {
        goto L15270;}}
    else {
      goto L15270;}}}
 L33137: {
  code_1381X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1381X)))))<<2)), code_1381X, 20, 4);
  n_1382X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1382X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_863X;
  x_1383X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1383X;
  arg0K0 = 2;
  goto L22895;}
 L30256: {
  code_1384X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1384X)))))<<2)), code_1384X, 28, 4);
  n_1385X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1385X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_870X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_871X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_872X;
  arg0K0 = 3;
  goto L22895;}
 L30255: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L15424: {
  i_1386X = arg0K0;
  next_stob_1387X = *((long *) ((((char *) (-3 + log_876X))) + (((i_1386X)<<2))));
  if ((1 == next_stob_1387X)) {add_log_entryAgc(2, i_1386X, arg3_870X, index_875X, byte_872X, 0);
    goto L30255;}
  else {
    if ((arg3_870X == next_stob_1387X)) {
      if ((index_875X == (*((long *) ((((char *) (-3 + log_876X))) + (4 + (((i_1386X)<<2)))))))) {
        addr_1388X = (((char *) (-3 + log_876X))) + (8 + (((i_1386X)<<2)));S48_WRITE_BARRIER(log_876X, addr_1388X, byte_872X);
        *((long *) addr_1388X) = byte_872X;
        goto L30255;}
      else {
        goto L15444;}}
    else {
      goto L15444;}}}
 L30221: {
  code_1389X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1389X)))))<<2)), code_1389X, 20, 4);
  n_1390X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1390X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_870X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_871X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_872X;
  arg0K0 = 3;
  goto L22895;}
 L33258: {
  code_1391X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1391X)))))<<2)), code_1391X, 20, 4);
  n_1392X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1392X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_870X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_869X;
  x_1393X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1393X;
  arg0K0 = 3;
  goto L22895;}
 L43763: {
  reason_1394X = arg0K0;
  status_1395X = arg0K1;
  code_1396X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1396X)))))<<2)), code_1396X, (((reason_1394X)<<2)), 4);
  n_1397X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1397X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((reason_1394X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_882X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_881X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_880X;
  merged_arg0K0 = status_1395X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 2;
  goto get_error_string;
 get_error_string_return_2:
  x_1398X = get_error_string0_return_value;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1398X;
  arg0K0 = 4;
  goto L22895;}
 L48495: {
  code_1399X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1399X)))))<<2)), code_1399X, 20, 4);
  n_1400X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1400X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_882X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_881X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_880X;
  x_1401X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1401X;
  arg0K0 = 4;
  goto L22895;}
 L49248: {
  i_1402X = arg0K0;
  h_1403X = arg0K1;
  if ((i_1402X < n_894X)) {
    arg0K0 = (1 + i_1402X);
    arg0K1 = (h_1403X + (((*((unsigned char *) ((((char *) (-3 + x_893X))) + i_1402X))))));
    goto L49248;}
  else {
    SvalS = (((h_1403X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}}
 L49225: {
  code_1404X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1404X)))))<<2)), code_1404X, 20, 4);
  n_1405X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1405X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1406X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1406X;
  arg0K0 = 1;
  goto L22895;}
 L44054: {
  code_1407X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1407X)))))<<2)), code_1407X, 20, 4);
  n_1408X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1408X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_895X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_896X;
  arg0K0 = 2;
  goto L22895;}
 L39957: {
  firstP_1409X = arg2K0;
  vector_1410X = s48_find_all(type_915X);
  if ((1 == vector_1410X)) {
    if (firstP_1409X) {s48_collect();
      arg2K0 = 0;
      goto L39957;}
    else {
      code_1411X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1411X)))))<<2)), code_1411X, 32, 4);
      n_1412X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1412X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 32;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((type_915X)<<2));
      arg0K0 = 1;
      goto L22895;}}
  else {
    SvalS = vector_1410X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}}
 L40045: {
  firstP_1413X = arg2K0;
  type_1414X = arg0K1;
  vector_1415X = s48_find_all_records(type_1414X);
  if ((1 == vector_1415X)) {
    if (firstP_1413X) {
      Stemp0S = type_1414X;s48_collect();
      value_1416X = Stemp0S;
      Stemp0S = 1;
      arg2K0 = 0;
      arg0K1 = value_1416X;
      goto L40045;}
    else {
      code_1417X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1417X)))))<<2)), code_1417X, 32, 4);
      n_1418X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1418X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 32;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = type_1414X;
      arg0K0 = 1;
      goto L22895;}}
  else {
    SvalS = vector_1415X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}}
 L27193: {
  code_1419X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1419X)))))<<2)), code_1419X, 20, 4);
  n_1420X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1420X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1421X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1421X;
  arg0K0 = 1;
  goto L22895;}
 L25184: {
  Slast_code_calledS = code_924X;
  Scode_pointerS = ((((char *) (-3 + code_924X))) + (pc_927X + (((size_926X)>>2))));
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L25739: {
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
  goto L22895;}
 L51118: {
  SvalS = (((old_933X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L8942: {
  proposal_1425X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  x_1426X = Scurrent_threadS;
  addr_1427X = (((char *) (-3 + x_1426X))) + 12;S48_WRITE_BARRIER(x_1426X, addr_1427X, proposal_1425X);
  *((long *) addr_1427X) = proposal_1425X;
  if ((12 == x_935X)) {
    arg0K0 = 0;
    arg0K1 = 25;
    arg0K2 = 0;
    goto L22608;}
  else {
    pc_1428X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    code_1429X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    Slast_code_calledS = code_1429X;
    Scode_pointerS = ((((char *) (-3 + code_1429X))) + (((pc_1428X)>>2)));
    v_1430X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    SvalS = v_1430X;
    arg1K0 = (Scode_pointerS);
    goto L25070;}}
 L50242: {
  x_1431X = s48_schedule_alarm_interrupt((((p_937X)>>2)));
  SvalS = (((x_1431X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L47979: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L47983;}
  else {
    arg2K0 = 1;
    goto L47983;}}
 L47984: {
  code_1432X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1432X)))))<<2)), code_1432X, 20, 4);
  n_1433X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1433X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_941X;
  x_1434X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1434X;
  arg0K0 = 2;
  goto L22895;}
 L30487: {
  rest_list_1435X = arg0K0;
  if ((25 == rest_list_1435X)) {
    proc_1436X = *((long *) ((SstackS) + (-4 + (((nargs_944X)<<2)))));
    name_1437X = *((long *) ((SstackS) + (-8 + (((nargs_944X)<<2)))));
    args_1438X = SstackS;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (10 + (((nargs_944X)<<10)));
    if ((3 == (3 & name_1437X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_1437X))) + -4))))>>2))))) {
        if ((3 == (3 & proc_1436X))) {
          if ((18 == (31 & ((((*((long *) ((((char *) (-3 + proc_1436X))) + -4))))>>2))))) {
            if ((4 == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proc_1436X))) + -4))))>>8)))) {
              result_1439X = s48_external_call(proc_1436X, name_1437X, (-2 + nargs_944X), args_1438X);
              if ((Sexternal_exceptionPS)) {
                Sexternal_exceptionPS = 0;
                arg0K0 = (Sexternal_exception_nargsS);
                goto L22895;}
              else {
                SvalS = result_1439X;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L25070;}}
            else {
              goto L30791;}}
          else {
            goto L30791;}}
        else {
          goto L30791;}}
      else {
        goto L30791;}}
    else {
      goto L30791;}}
  else {
    x_1440X = *((long *) (((char *) (-3 + rest_list_1435X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1440X;
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_1435X))) + 4)));
    goto L30487;}}
 L47220: {
  if ((1 == (SvalS))) {
    v_1441X = Hlookup2690((Sexported_bindingsS), arg2_950X, 0);
    arg0K0 = v_1441X;
    goto L47261;}
  else {
    v_1442X = Hlookup2671((Simported_bindingsS), arg2_950X, 0);
    arg0K0 = v_1442X;
    goto L47261;}}
 L47225: {
  code_1443X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1443X)))))<<2)), code_1443X, 20, 4);
  n_1444X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1444X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_950X;
  x_1445X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1445X;
  arg0K0 = 2;
  goto L22895;}
 L32319: {
  if ((1 == (SvalS))) {
    arg0K0 = (Sexported_bindingsS);
    goto L32354;}
  else {
    arg0K0 = (Simported_bindingsS);
    goto L32354;}}
 L32324: {
  code_1446X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1446X)))))<<2)), code_1446X, 20, 4);
  n_1447X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1447X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_952X;
  x_1448X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1448X;
  arg0K0 = 2;
  goto L22895;}
 L49143: {
  option_1449X = arg0K0;
  seconds_1450X = arg0K1;
  mseconds_1451X = arg0K2;
  if ((536869 < seconds_1450X)) {
    code_1452X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1452X)))))<<2)), code_1452X, 24, 4);
    n_1453X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1453X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 24;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((option_1449X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_1450X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_1451X)<<2));
    arg0K0 = 3;
    goto L22895;}
  else {
    SvalS = (((((1000 * seconds_1450X) + mseconds_1451X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}}
 L51152: {
  s48_Scallback_return_stack_blockS = arg2_978X;
  return x_979X;}
 L41167: {
  val_1454X = arg0K0;
  SvalS = val_1454X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L41132: {
  code_1455X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1455X)))))<<2)), code_1455X, 20, 4);
  n_1456X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1456X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_980X;
  x_1457X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1457X;
  arg0K0 = 2;
  goto L22895;}
 L40335: {
  len_1458X = 1 + n_985X;
  addr_1459X = s48_allocate_small((4 + len_1458X));
  *((long *) addr_1459X) = (70 + (((len_1458X)<<8)));
  string_1460X = 3 + (((long) (addr_1459X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1460X))) + n_985X)) = 0;
  arg0K0 = arg2_984X;
  arg0K1 = (-1 + n_985X);
  goto L40312;}
 L40287: {
  if ((25 == arg2_984X)) {
    goto L40335;}
  else {
    code_1461X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1461X)))))<<2)), code_1461X, 20, 4);
    n_1462X = *((unsigned char *) (Scode_pointerS));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_1462X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 20;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = arg2_984X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_985X)<<2));
    arg0K0 = 2;
    goto L22895;}}
 L42706: {
  list_1463X = arg0K0;
  slow_1464X = arg0K1;
  move_slowP_1465X = arg2K2;
  if ((25 == list_1463X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    if ((3 == (3 & list_1463X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_1463X))) + -4))))>>2))))) {
        head_1466X = *((long *) (((char *) (-3 + list_1463X))));
        if ((3 == (3 & head_1466X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_1466X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_1466X))))) == arg2_989X)) {
              SvalS = head_1466X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L25070;}
            else {
              list_1467X = *((long *) ((((char *) (-3 + list_1463X))) + 4));
              if ((list_1467X == slow_1464X)) {
                goto L42692;}
              else {
                if (move_slowP_1465X) {
                  arg0K0 = list_1467X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_1464X))) + 4)));
                  arg2K2 = 0;
                  goto L42706;}
                else {
                  arg0K0 = list_1467X;
                  arg0K1 = slow_1464X;
                  arg2K2 = 1;
                  goto L42706;}}}}
          else {
            goto L42692;}}
        else {
          goto L42692;}}
      else {
        goto L42692;}}
    else {
      goto L42692;}}}
 L30877: {
  code_1468X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1468X)))))<<2)), code_1468X, 28, 8);
  n_1469X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1469X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_994X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_993X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_995X)<<2));
  arg0K0 = 3;
  goto L22895;}
 L30867: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg3_994X))) + (((index_995X)<<2)))));
  goto L30876;}
 L30876: {
  value_1470X = arg0K0;
  SvalS = value_1470X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L30897: {
  code_1471X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1471X)))))<<2)), code_1471X, 20, 8);
  n_1472X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1472X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_994X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_993X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_995X)<<2));
  arg0K0 = 3;
  goto L22895;}
 L31167: {
  code_1473X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1473X)))))<<2)), code_1473X, 28, 8);
  n_1474X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1474X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 28;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_1004X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_1003X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_1005X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_1006X;
  arg0K0 = 4;
  goto L22895;}
 L31157: {
  addr_1475X = (((char *) (-3 + arg4_1004X))) + (((index_1005X)<<2));S48_WRITE_BARRIER(arg4_1004X, addr_1475X, value_1006X);
  *((long *) addr_1475X) = value_1006X;
  goto L31166;}
 L31166: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L31189: {
  code_1476X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1476X)))))<<2)), code_1476X, 20, 8);
  n_1477X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1477X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_1004X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_1003X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_1005X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_1006X;
  arg0K0 = 4;
  goto L22895;}
 L31478: {
  port_1478X = arg0K0;
  if ((3 == (3 & port_1478X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1478X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1478X))) + 4))))>>2))))) {
        goto L31535;}
      else {
        b_1479X = *((long *) ((((char *) (-3 + port_1478X))) + 16));
        if ((1 == b_1479X)) {
          code_1480X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1480X)))))<<2)), code_1480X, 52, 8);
          n_1481X = *((unsigned char *) (Scode_pointerS));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_1481X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 52;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = port_1478X;
          arg0K0 = 1;
          goto L22895;}
        else {
          p_1482X = *((long *) ((((char *) (-3 + port_1478X))) + 24));
          i_1483X = (((*((long *) ((((char *) (-3 + port_1478X))) + 20))))>>2);
          if ((i_1483X == (((p_1482X)>>2)))) {
            code_1484X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1484X)))))<<2)), code_1484X, 52, 8);
            n_1485X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1485X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 52;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1478X;
            arg0K0 = 1;
            goto L22895;}
          else {
            val_1486X = 4 + (((i_1483X)<<2));
            addr_1487X = (((char *) (-3 + port_1478X))) + 20;S48_WRITE_BARRIER(port_1478X, addr_1487X, val_1486X);
            *((long *) addr_1487X) = val_1486X;
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1479X))) + i_1483X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L25070;}}}}
    else {
      goto L31535;}}
  else {
    goto L31535;}}
 L31728: {
  port_1488X = arg0K0;
  if ((3 == (3 & port_1488X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1488X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1488X))) + 4))))>>2))))) {
        goto L31785;}
      else {
        b_1489X = *((long *) ((((char *) (-3 + port_1488X))) + 16));
        if ((1 == b_1489X)) {
          code_1490X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1490X)))))<<2)), code_1490X, 52, 8);
          n_1491X = *((unsigned char *) (Scode_pointerS));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = (((n_1491X)<<2));
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = 52;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = port_1488X;
          arg0K0 = 1;
          goto L22895;}
        else {
          p_1492X = *((long *) ((((char *) (-3 + port_1488X))) + 24));
          i_1493X = (((*((long *) ((((char *) (-3 + port_1488X))) + 20))))>>2);
          if ((i_1493X == (((p_1492X)>>2)))) {
            code_1494X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1494X)))))<<2)), code_1494X, 52, 8);
            n_1495X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1495X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 52;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1488X;
            arg0K0 = 1;
            goto L22895;}
          else {
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1489X))) + i_1493X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L25070;}}}}
    else {
      goto L31785;}}
  else {
    goto L31785;}}
 L31971: {
  Kchar_1496X = arg0K0;
  port_1497X = arg0K1;
  if ((9 == (255 & Kchar_1496X))) {
    if ((3 == (3 & port_1497X))) {
      if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1497X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_1497X))) + 4))))>>2))))) {
          goto L32031;}
        else {
          b_1498X = *((long *) ((((char *) (-3 + port_1497X))) + 16));
          if ((1 == b_1498X)) {
            code_1499X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1499X)))))<<2)), code_1499X, 52, 8);
            n_1500X = *((unsigned char *) (Scode_pointerS));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_1500X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = 52;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = Kchar_1496X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1497X;
            arg0K0 = 2;
            goto L22895;}
          else {
            i_1501X = (((*((long *) ((((char *) (-3 + port_1497X))) + 20))))>>2);
            if ((i_1501X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_1498X))) + -4))))>>8)))) {
              code_1502X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1502X)))))<<2)), code_1502X, 52, 8);
              n_1503X = *((unsigned char *) (Scode_pointerS));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((n_1503X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = 52;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = Kchar_1496X;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = port_1497X;
              arg0K0 = 2;
              goto L22895;}
            else {
              val_1504X = 4 + (((i_1501X)<<2));
              addr_1505X = (((char *) (-3 + port_1497X))) + 20;S48_WRITE_BARRIER(port_1497X, addr_1505X, val_1504X);
              *((long *) addr_1505X) = val_1504X;
              *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_1497X))) + 16)))))) + i_1501X)) = (((((Kchar_1496X)>>8))));
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L25070;}}}}
      else {
        goto L32031;}}
    else {
      goto L32031;}}
  else {
    goto L32031;}}
 L42936: {
  stuff_1506X = arg0K0;
  if ((3 == (3 & stuff_1506X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_1506X))) + -4))))>>2))))) {
      thing_1507X = *((long *) (((char *) (-3 + stuff_1506X))));
      if ((0 == (3 & thing_1507X))) {
        ps_write_integer((((thing_1507X)>>2)), out_1021X);
        goto L42942;}
      else {
        if ((9 == (255 & thing_1507X))) {
          ps_write_string("#\\", out_1021X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_1507X)>>8))), out_1021X, ignoreXX) }
          goto L42942;}
        else {
          if ((3 == (3 & thing_1507X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>8))))>>2)))) {
                type_1508X = *((long *) (((char *) (-3 + thing_1507X))));
                if ((3 == (3 & type_1508X))) {
                  if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_1508X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_1508X))) + -4))))>>8))))>>2)))) {
                      obj_1509X = *((long *) ((((char *) (-3 + type_1508X))) + 8));
                      if ((3 == (3 & obj_1509X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_1509X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_1021X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_1507X)))))))) + 8))))))))))))), out_1021X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_1021X, ignoreXX) }
                          goto L42942;}
                        else {
                          goto L16422;}}
                      else {
                        goto L16422;}}
                    else {
                      goto L16422;}}
                  else {
                    goto L16422;}}
                else {
                  goto L16422;}}
              else {
                goto L16422;}}
            else {
              goto L16422;}}
          else {
            goto L16422;}}}}
    else {
      goto L42927;}}
  else {
    goto L42927;}}
 L24735: {
  v_1510X = arg0K0;
  ScontS = (cont_1031X + (4 + (((v_1510X)<<2))));
  v_1511X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  v_1512X = s48_invoke_native_continuation(((((long) (((char *) v_1511X)))) + protocol_skip_1030X));
  arg0K0 = v_1512X;
  goto L24218;}
 L22721: {
  v_1513X = arg0K0;
  SvalS = v_1513X;
  arg0K0 = cont_527X;
  goto L23435;}
 L26488: {
  x_1514X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1514X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L26470: {
  stack_nargs_1515X = arg0K0;
  l_1516X = arg0K1;
  if ((count_532X == stack_nargs_1515X)) {
    arg0K0 = l_1516X;
    goto L26488;}
  else {
    x_1517X = *((long *) (((char *) (-3 + l_1516X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1517X;
    arg0K0 = (1 + stack_nargs_1515X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1516X))) + 4)));
    goto L26470;}}
 L26407: {
  loc_1518X = arg1K0;
  arg_1519X = arg1K1;
  if ((arg_1519X < arg_top_1049X)) {
    SstackS = ((SstackS) + (0 - (((stack_nargs_1046X)<<2))));
    if ((0 == list_arg_count_1048X)) {
      goto L26394;}
    else {
      merged_arg0K0 = list_args_1047X;
      merged_arg0K1 = list_arg_count_1048X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      goto L26394;}}
  else {
    *((long *) loc_1518X) = (*((long *) arg_1519X));
    arg1K0 = (loc_1518X + -4);
    arg1K1 = (arg_1519X + -4);
    goto L26407;}}
 L23145: {
  arg_count_1520X = arg0K0;
  handlers_1521X = SHARED_REF((Sinterrupt_handlersS));
  if ((3 == (3 & handlers_1521X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_1521X))) + -4))))>>2))))) {
      goto L23163;}
    else {
      goto L23205;}}
  else {
    goto L23205;}}
 L11584: {
  channel_1522X = arg0K0;
  x_1523X = 1 == (Spending_channels_headS);
  if (x_1523X) {
    goto L11598;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Sstack_limitS = (Sreal_stack_limitS);
      if ((s48_Spending_eventsPS)) {
        s48_Sstack_limitS = (((char *) -1));
        goto L11598;}
      else {
        goto L11598;}}
    else {
      s48_Sstack_limitS = (((char *) -1));
      goto L11598;}}}
 L8226: {
  count_1524X = arg0K0;
  i_1525X = arg0K1;
  offset_1526X = arg0K2;
  if ((0 == count_1524X)) {
    if ((i_1525X < total_count_1066X)) {
      arg0K0 = i_1525X;
      arg0K1 = offset_1526X;
      goto L6858;}
    else {
      arg0K0 = offset_1526X;
      goto L52706;}}
  else {
    value_1527X = *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1526X)))))<<2))));
    addr_1528X = (((char *) (-3 + new_env_1063X))) + (((i_1525X)<<2));S48_WRITE_BARRIER(new_env_1063X, addr_1528X, value_1527X);
    *((long *) addr_1528X) = value_1527X;
    arg0K0 = (-1 + count_1524X);
    arg0K1 = (1 + i_1525X);
    arg0K2 = (1 + offset_1526X);
    goto L8226;}}
 L10800: {
  count_1529X = arg0K0;
  i_1530X = arg0K1;
  offset_1531X = arg0K2;
  if ((0 == count_1529X)) {
    if ((i_1530X < total_count_1075X)) {
      arg0K0 = i_1530X;
      arg0K1 = offset_1531X;
      goto L7571;}
    else {
      arg0K0 = offset_1531X;
      goto L52696;}}
  else {
    value_1532X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1531X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1531X))))))<<2))));
    addr_1533X = (((char *) (-3 + new_env_1072X))) + (((i_1530X)<<2));S48_WRITE_BARRIER(new_env_1072X, addr_1533X, value_1532X);
    *((long *) addr_1533X) = value_1532X;
    arg0K0 = (-1 + count_1529X);
    arg0K1 = (1 + i_1530X);
    arg0K2 = (2 + offset_1531X);
    goto L10800;}}
 L40508: {
  pop_continuationB_return_tag = 6;
  goto pop_continuationB;
 pop_continuationB_return_6:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L23435: {
  cont_1534X = arg0K0;
  if ((1 == cont_1534X)) {
    if ((0 == (3 & (SvalS)))) {
      s48_Scallback_return_stack_blockS = 1;
      return ((((SvalS))>>2));}
    else {
      goto L23450;}}
  else {
    goto L23450;}}
 L40650: {
  v_1535X = arg0K0;
  ScontS = (cont_1108X + (4 + (((v_1535X)<<2))));
  merged_arg0K0 = 0;
  move_args_above_contB_return_tag = 14;
  goto move_args_above_contB;
 move_args_above_contB_return_14:
  goto L40555;}
 L40555: {
  x_1536X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1536X;
  SvalS = proc_1107X;
  arg0K0 = 1;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L32460;}
 L26661: {
  code_1537X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1537X)))))<<2)), code_1537X, 20, 32);
  n_1538X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1538X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = template_611X;
  arg0K0 = 1;
  goto L22895;}
 L14227: {
  if ((3 == (3 & x_634X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_634X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L50737;}
    else {
      goto L14233;}}
  else {
    goto L14233;}}
 L41791: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L41924: {
  if ((3 == (3 & n_636X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_636X))) + -4))))>>2))))) {
      goto L41935;}
    else {
      goto L41936;}}
  else {
    goto L41936;}}
 L42115: {
  if ((3 == (3 & n_637X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_637X))) + -4))))>>2))))) {
      goto L42126;}
    else {
      goto L42127;}}
  else {
    goto L42127;}}
 L42306: {
  if ((3 == (3 & n_638X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_638X))) + -4))))>>2))))) {
      goto L42317;}
    else {
      goto L42318;}}
  else {
    goto L42318;}}
 L34117: {
  if ((0 == (3 & arg2_640X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18367;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_640X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18367;}}
 L34134: {
  code_1539X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1539X)))))<<2)), code_1539X, 20, 4);
  n_1540X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1540X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_640X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_641X;
  arg0K0 = 2;
  goto L22895;}
 L8389: {
  b_1541X = arg0K0;
  lo_a_1542X = 65535 & a_1139X;
  lo_b_1543X = 65535 & b_1541X;
  hi_a_1544X = 65535 & (((a_1139X)>>16));
  hi_b_1545X = 65535 & (((b_1541X)>>16));
  lo_c_1546X = SMALL_MULTIPLY(lo_a_1542X, lo_b_1543X);
  v_1547X = SMALL_MULTIPLY(lo_b_1543X, hi_a_1544X);
  v_1548X = SMALL_MULTIPLY(lo_a_1542X, hi_b_1545X);
  mid_c_1549X = v_1548X + v_1547X;
  c_1550X = lo_c_1546X + (((mid_c_1549X)<<16));
  if ((0 < hi_a_1544X)) {
    if ((0 < hi_b_1545X)) {
      arg0K0 = arg2_643X;
      arg0K1 = y_644X;
      goto L20123;}
    else {
      goto L8431;}}
  else {
    goto L8431;}}
 L44196: {
  arg0K0 = arg2_643X;
  arg0K1 = y_644X;
  goto L20123;}
 L34422: {
  if ((0 == (3 & arg2_647X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18324;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_647X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18324;}}
 L34439: {
  code_1551X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1551X)))))<<2)), code_1551X, 20, 4);
  n_1552X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1552X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_647X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_648X;
  arg0K0 = 2;
  goto L22895;}
 L8647: {
  b_1553X = arg0K0;
  c_1554X = a_1159X / b_1553X;
  x_1555X = 0 == (a_1159X % b_1553X);
  if (x_1555X) {
    if ((a_652X < 0)) {
      if ((b_653X < 0)) {s48_make_availableAgc(16);
        if ((536870911 < c_1554X)) {
          goto L51953;}
        else {
          if ((c_1554X < -536870912)) {
            goto L51953;}
          else {
            arg0K0 = (((c_1554X)<<2));
            goto L51948;}}}
      else {
        goto L8693;}}
    else {
      if ((b_653X < 0)) {
        goto L8693;}
      else {s48_make_availableAgc(16);
        if ((536870911 < c_1554X)) {
          goto L51975;}
        else {
          if ((c_1554X < -536870912)) {
            goto L51975;}
          else {
            arg0K0 = (((c_1554X)<<2));
            goto L51970;}}}}}
  else {
    arg0K0 = arg2_650X;
    arg0K1 = y_651X;
    goto L23790;}}
 L44439: {
  if ((0 == (3 & arg2_650X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17892;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_650X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17892;}}
 L44479: {
  result_1556X = arg0K0;
  if ((1 == result_1556X)) {
    arg0K0 = arg2_650X;
    arg0K1 = y_651X;
    goto L23790;}
  else {
    SvalS = result_1556X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}}
 L34723: {
  if ((0 == (3 & (arg2_654X | y_655X)))) {
    if ((arg2_654X == y_655X)) {
      arg0K0 = 5;
      goto L34727;}
    else {
      arg0K0 = 1;
      goto L34727;}}
  else {
    if ((3 == (3 & arg2_654X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_654X))) + -4))))>>2))))) {
        if ((3 == (3 & y_655X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_655X))) + -4))))>>2))))) {
            b_1557X = s48_bignum_equal_p((((char *) (-3 + arg2_654X))), (((char *) (-3 + y_655X))));
            if (b_1557X) {
              arg0K0 = 5;
              goto L34727;}
            else {
              arg0K0 = 1;
              goto L34727;}}
          else {
            arg0K0 = 1;
            goto L34727;}}
        else {
          arg0K0 = 1;
          goto L34727;}}
      else {
        arg0K0 = 1;
        goto L34727;}}
    else {
      arg0K0 = 1;
      goto L34727;}}}
 L34743: {
  val_1558X = arg0K0;
  SvalS = val_1558X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L34744: {
  code_1559X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1559X)))))<<2)), code_1559X, 20, 4);
  n_1560X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1560X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_654X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_655X;
  arg0K0 = 2;
  goto L22895;}
 L34999: {
  if ((0 == (3 & arg2_656X))) {
    if ((0 == (3 & y_657X))) {
      if ((arg2_656X < y_657X)) {
        arg0K0 = 5;
        goto L35003;}
      else {
        arg0K0 = 1;
        goto L35003;}}
    else {
      v_1561X = s48_bignum_test((((char *) (-3 + y_657X))));
      if ((1 == v_1561X)) {
        arg0K0 = 5;
        goto L35003;}
      else {
        arg0K0 = 1;
        goto L35003;}}}
  else {
    if ((0 == (3 & y_657X))) {
      v_1562X = s48_bignum_test((((char *) (-3 + arg2_656X))));
      if ((1 == v_1562X)) {
        arg0K0 = 1;
        goto L35003;}
      else {
        arg0K0 = 5;
        goto L35003;}}
    else {
      v_1563X = s48_bignum_compare((((char *) (-3 + arg2_656X))), (((char *) (-3 + y_657X))));
      if ((-1 == v_1563X)) {
        arg0K0 = 5;
        goto L35003;}
      else {
        arg0K0 = 1;
        goto L35003;}}}}
 L35019: {
  val_1564X = arg0K0;
  SvalS = val_1564X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35020: {
  code_1565X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1565X)))))<<2)), code_1565X, 20, 4);
  n_1566X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1566X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_656X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_657X;
  arg0K0 = 2;
  goto L22895;}
 L35338: {
  if ((0 == (3 & y_659X))) {
    if ((0 == (3 & arg2_658X))) {
      if ((y_659X < arg2_658X)) {
        arg0K0 = 5;
        goto L35342;}
      else {
        arg0K0 = 1;
        goto L35342;}}
    else {
      v_1567X = s48_bignum_test((((char *) (-3 + arg2_658X))));
      if ((1 == v_1567X)) {
        arg0K0 = 5;
        goto L35342;}
      else {
        arg0K0 = 1;
        goto L35342;}}}
  else {
    if ((0 == (3 & arg2_658X))) {
      v_1568X = s48_bignum_test((((char *) (-3 + y_659X))));
      if ((1 == v_1568X)) {
        arg0K0 = 1;
        goto L35342;}
      else {
        arg0K0 = 5;
        goto L35342;}}
    else {
      v_1569X = s48_bignum_compare((((char *) (-3 + y_659X))), (((char *) (-3 + arg2_658X))));
      if ((-1 == v_1569X)) {
        arg0K0 = 5;
        goto L35342;}
      else {
        arg0K0 = 1;
        goto L35342;}}}}
 L35358: {
  val_1570X = arg0K0;
  SvalS = val_1570X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35359: {
  code_1571X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1571X)))))<<2)), code_1571X, 20, 4);
  n_1572X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1572X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_658X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_659X;
  arg0K0 = 2;
  goto L22895;}
 L35677: {
  if ((0 == (3 & y_661X))) {
    if ((0 == (3 & arg2_660X))) {
      if ((y_661X < arg2_660X)) {
        arg0K0 = 1;
        goto L35681;}
      else {
        arg0K0 = 5;
        goto L35681;}}
    else {
      v_1573X = s48_bignum_test((((char *) (-3 + arg2_660X))));
      if ((1 == v_1573X)) {
        arg0K0 = 1;
        goto L35681;}
      else {
        arg0K0 = 5;
        goto L35681;}}}
  else {
    if ((0 == (3 & arg2_660X))) {
      v_1574X = s48_bignum_test((((char *) (-3 + y_661X))));
      if ((1 == v_1574X)) {
        arg0K0 = 5;
        goto L35681;}
      else {
        arg0K0 = 1;
        goto L35681;}}
    else {
      v_1575X = s48_bignum_compare((((char *) (-3 + y_661X))), (((char *) (-3 + arg2_660X))));
      if ((-1 == v_1575X)) {
        arg0K0 = 1;
        goto L35681;}
      else {
        arg0K0 = 5;
        goto L35681;}}}}
 L35697: {
  val_1576X = arg0K0;
  SvalS = val_1576X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35698: {
  code_1577X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1577X)))))<<2)), code_1577X, 20, 4);
  n_1578X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1578X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_660X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_661X;
  arg0K0 = 2;
  goto L22895;}
 L35987: {
  if ((0 == (3 & arg2_662X))) {
    if ((0 == (3 & y_663X))) {
      if ((arg2_662X < y_663X)) {
        arg0K0 = 1;
        goto L35991;}
      else {
        arg0K0 = 5;
        goto L35991;}}
    else {
      v_1579X = s48_bignum_test((((char *) (-3 + y_663X))));
      if ((1 == v_1579X)) {
        arg0K0 = 1;
        goto L35991;}
      else {
        arg0K0 = 5;
        goto L35991;}}}
  else {
    if ((0 == (3 & y_663X))) {
      v_1580X = s48_bignum_test((((char *) (-3 + arg2_662X))));
      if ((1 == v_1580X)) {
        arg0K0 = 5;
        goto L35991;}
      else {
        arg0K0 = 1;
        goto L35991;}}
    else {
      v_1581X = s48_bignum_compare((((char *) (-3 + arg2_662X))), (((char *) (-3 + y_663X))));
      if ((-1 == v_1581X)) {
        arg0K0 = 1;
        goto L35991;}
      else {
        arg0K0 = 5;
        goto L35991;}}}}
 L36007: {
  val_1582X = arg0K0;
  SvalS = val_1582X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L36008: {
  code_1583X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1583X)))))<<2)), code_1583X, 20, 4);
  n_1584X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1584X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_662X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_663X;
  arg0K0 = 2;
  goto L22895;}
 L20062: {
  length0_1585X = arg0K0;
  extra0_1586X = arg0K1;
  if ((0 == (3 & y_1175X))) {
    arg0K0 = 3;
    goto L20064;}
  else {
    arg0K0 = 0;
    goto L20064;}}
 L9261: {
  b_1587X = arg0K0;
  c_1588X = a_1176X / b_1587X;
  if ((a_666X < 0)) {
    if ((b_667X < 0)) {
      goto L9307;}
    else {
      goto L9306;}}
  else {
    if ((b_667X < 0)) {
      goto L9306;}
    else {
      goto L9307;}}}
 L36303: {
  arg0K0 = arg2_664X;
  arg0K1 = y_665X;
  goto L20032;}
 L19971: {
  length0_1589X = arg0K0;
  extra0_1590X = arg0K1;
  if ((0 == (3 & y_1178X))) {
    arg0K0 = 3;
    goto L19973;}
  else {
    arg0K0 = 0;
    goto L19973;}}
 L36479: {
  b_1591X = arg0K0;
  c_1592X = a_1179X % b_1591X;
  if ((a_670X < 0)) {
    arg0K0 = (0 - c_1592X);
    goto L36483;}
  else {
    arg0K0 = c_1592X;
    goto L36483;}}
 L36457: {
  arg0K0 = arg2_668X;
  arg0K1 = y_669X;
  goto L19941;}
 L23697: {
  x_1593X = arg0K0;
  if ((536870911 < x_1593X)) {
    goto L23698;}
  else {
    if ((x_1593X < -536870912)) {
      goto L23698;}
    else {
      return (((x_1593X)<<2));}}}
 L19549: {
  length_1594X = arg0K0;
  extra_1595X = arg0K1;
  Stemp0S = x_1191X;s48_make_availableAgc(((((length_1594X + extra_1595X))<<2)));
  value_1596X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1596X))) {
    v_1597X = (char *) s48_long_to_bignum((((value_1596X)>>2)));
    arg1K0 = v_1597X;
    goto L19541;}
  else {
    arg1K0 = (((char *) (-3 + value_1596X)));
    goto L19541;}}
 L19896: {
  length_1598X = arg0K0;
  extra_1599X = arg0K1;
  if ((length_1598X < 1)) {
    arg0K0 = 1;
    goto L19898;}
  else {
    arg0K0 = length_1598X;
    goto L19898;}}
 L37181: {
  x_1600X = arg0K0;
  count_1601X = arg0K1;
  if ((0 == x_1600X)) {
    SvalS = (((count_1601X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    arg0K0 = (((x_1600X)>>1));
    arg0K1 = (count_1601X + (1 & x_1600X));
    goto L37181;}}
 L19838: {
  length_1602X = arg0K0;
  extra_1603X = arg0K1;
  if ((length_1602X < 1)) {
    arg0K0 = 1;
    goto L19840;}
  else {
    arg0K0 = length_1602X;
    goto L19840;}}
 L37282: {
  if ((0 == (3 & arg2_685X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18106;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_685X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18106;}}
 L37461: {
  if ((0 == (3 & arg2_687X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18063;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_687X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18063;}}
 L37640: {
  if ((0 == (3 & arg2_689X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18020;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_689X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18020;}}
 L44823: {
  v_1604X = (char *) s48_long_to_bignum(x_1213X);
  v_1605X = enter_bignum(v_1604X);
  arg0K0 = v_1605X;
  goto L44818;}
 L44818: {
  val_1606X = arg0K0;
  SvalS = val_1606X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L44845: {
  v_1607X = (char *) s48_long_to_bignum(result_1215X);
  v_1608X = enter_bignum(v_1607X);
  arg0K0 = v_1608X;
  goto L44840;}
 L44840: {
  val_1609X = arg0K0;
  SvalS = val_1609X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L44728: {
  x_1610X = arg0K0;
  y_1611X = arg0K1;
  y_1612X = ((y_1611X)>>2);
  merged_arg0K0 = x_1610X;
  merged_arg0K1 = y_1612X;
  shift_space_return_tag = 1;
  goto shift_space;
 shift_space_return_1:
  needed_1613X = shift_space0_return_value;
  Stemp0S = x_1610X;s48_make_availableAgc((((needed_1613X)<<2)));
  value_1614X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1614X))) {
    v_1615X = (char *) s48_long_to_bignum((((value_1614X)>>2)));
    arg1K0 = v_1615X;
    goto L44790;}
  else {
    arg1K0 = (((char *) (-3 + value_1614X)));
    goto L44790;}}
 L44867: {
  v_1616X = (char *) s48_long_to_bignum(result_1215X);
  v_1617X = enter_bignum(v_1616X);
  arg0K0 = v_1617X;
  goto L44862;}
 L44862: {
  val_1618X = arg0K0;
  SvalS = val_1618X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L44898: {
  x_1619X = arg1K0;
  external_bignum_1620X = (char *) s48_bignum_arithmetic_shift(x_1619X, y_1217X);
  v_1621X = s48_bignum_fits_in_word_p(external_bignum_1620X, 30, 1);
  if (v_1621X) {
    n_1622X = s48_bignum_to_long(external_bignum_1620X);
    arg0K0 = (((n_1622X)<<2));
    goto L44736;}
  else {
    v_1623X = enter_bignum(external_bignum_1620X);
    arg0K0 = v_1623X;
    goto L44736;}}
 L28242: {
  i_1624X = arg0K0;
  rest_list_1625X = arg0K1;
  if ((25 == rest_list_1625X)) {
    SvalS = new_724X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    *((long *) ((((char *) (-3 + new_724X))) + (((i_1624X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_1625X)))));
    arg0K0 = (1 + i_1624X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_1625X))) + 4)));
    goto L28242;}}
 L28687: {
  i_1626X = arg0K0;
  if ((i_1626X < 0)) {
    SvalS = x_1247X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    addr_1627X = (((char *) (-3 + x_1247X))) + (((i_1626X)<<2));S48_WRITE_BARRIER(x_1247X, addr_1627X, value_1248X);
    *((long *) addr_1627X) = value_1248X;
    arg0K0 = (-1 + i_1626X);
    goto L28687;}}
 L37922: {
  i_1628X = arg0K0;
  if ((i_1628X < 0)) {
    SvalS = vector_1263X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1263X))) + i_1628X)) = init_760X;
    arg0K0 = (-1 + i_1628X);
    goto L37922;}}
 L38134: {
  i_1629X = arg0K0;
  if ((i_1629X < 0)) {
    SvalS = vector_1283X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1283X))) + i_1629X)) = (init_776X);
    arg0K0 = (-1 + i_1629X);
    goto L38134;}}
 L21289: {
  bucket_1630X = arg0K0;
  arg0K0 = bucket_1630X;
  goto L21295;}
 L43035: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L38358: {
  index_1631X = arg0K0;
  channel_1632X = make_registered_channel(mode_800X, arg2_799X, index_1631X, 0, &reason_1633X);
  if ((1 == channel_1632X)) {
    if ((3 == (3 & arg2_799X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_799X))) + -4))))>>2))))) {
        if ((1 == mode_800X)) {
          goto L38489;}
        else {
          if ((3 == mode_800X)) {
            goto L38489;}
          else {
            v_1634X = ps_close_fd(index_1631X);
            arg0K0 = v_1634X;
            goto L38484;}}}
      else {
        arg0K0 = reason_1633X;
        goto L38328;}}
    else {
      arg0K0 = reason_1633X;
      goto L38328;}}
  else {
    SvalS = channel_1632X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}}
 L38416: {
  v_1635X = ps_open_fd((((char *)(((char *) (-3 + arg2_799X))))), 1, &v_1636X);
  arg0K0 = v_1635X;
  arg0K1 = v_1636X;
  goto L38429;}
 L38429: {
  channel_1637X = arg0K0;
  status_1638X = arg0K1;
  if ((status_1638X == NO_ERRORS)) {
    arg0K0 = channel_1637X;
    goto L38358;}
  else {
    if ((status_1638X == ENOENT)) {
      arg0K0 = 10;
      goto L38328;}
    else {
      code_1639X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1639X)))))<<2)), code_1639X, 96, 4);
      n_1640X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1640X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 96;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_799X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_800X)<<2));
      merged_arg0K0 = status_1638X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_1641X = get_error_string0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1641X;
      arg0K0 = 3;
      goto L22895;}}}
 L46643: {
  waitP_1642X = arg2K0;
  start_1643X = ((arg3_813X)>>2);
  count_1644X = ((arg2_812X)>>2);
  v_1645X = 4 == (*((long *) (((char *) (-3 + arg5_815X)))));
  if (v_1645X) {
    if ((3 == (3 & arg4_814X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_814X))) + -4))))>>2))))) {
        goto L38744;}
      else {
        goto L38991;}}
    else {
      goto L38991;}}
  else {
    goto L38812;}}
 L39155: {
  y_1646X = arg0K0;
  if ((y_1646X < (start_820X + count_821X))) {
    goto L39207;}
  else {
    got_1647X = ps_write_fd(((((*((long *) ((((char *) (-3 + arg4_819X))) + 8))))>>2)), ((((char *) (-3 + arg3_818X))) + start_820X), count_821X, &pendingP_1648X, &status_1649X);
    if ((status_1649X == NO_ERRORS)) {
      if (pendingP_1648X) {
        arg0K0 = 1;
        goto L39206;}
      else {
        arg0K0 = (((got_1647X)<<2));
        goto L39206;}}
    else {
      code_1650X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1650X)))))<<2)), code_1650X, 96, 4);
      n_1651X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1651X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 96;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_819X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg3_818X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_820X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_821X)<<2));
      merged_arg0K0 = status_1649X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 4;
      goto get_error_string;
     get_error_string_return_4:
      x_1652X = get_error_string0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1652X;
      arg0K0 = 5;
      goto L22895;}}}
 L39374: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_818X))) + -4))))>>8));
  goto L39155;}
 L11423: {
  val_1653X = *((long *) ((((char *) (-3 + ch_1342X))) + 12));
  addr_1654X = (((char *) (-3 + prev_1343X))) + 12;S48_WRITE_BARRIER(prev_1343X, addr_1654X, val_1653X);
  *((long *) addr_1654X) = val_1653X;
  addr_1655X = (((char *) (-3 + ch_1342X))) + 12;S48_WRITE_BARRIER(ch_1342X, addr_1655X, 1);
  *((long *) addr_1655X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_1342X))) + 16)));
  goto L47027;}
 L17159: {
  v_1656X = arg0K0;
  arg0K0 = (-1 + i_1350X);
  arg0K1 = v_1656X;
  goto L17145;}
 L39509: {
  if ((1 == proposal_840X)) {
    goto L39521;}
  else {
    addr_1657X = ((char *) (-3 + proposal_840X));S48_WRITE_BARRIER(proposal_840X, addr_1657X, 5);
    *((long *) addr_1657X) = 5;
    goto L39521;}}
 L10409: {
  i_1658X = arg0K0;
  stob_1659X = *((long *) ((((char *) (-3 + log_1361X))) + (((i_1658X)<<2))));
  if ((1 == stob_1659X)) {
    copies_1660X = *((long *) ((((char *) (-3 + proposal_841X))) + 12));
    arg0K0 = copies_1660X;
    goto L39887;}
  else {
    verify_1661X = *((long *) ((((char *) (-3 + log_1361X))) + (12 + (((i_1658X)<<2)))));
    value_1662X = *((long *) ((((char *) (-3 + log_1361X))) + (8 + (((i_1658X)<<2)))));
    if ((29 == verify_1661X)) {
      if ((3 == (3 & stob_1659X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1659X))) + -4)))))) {
          goto L10461;}
        else {
          goto L39771;}}
      else {
        goto L39771;}}
    else {
      if ((verify_1661X == ((((*((unsigned char *) ((((char *) (-3 + stob_1659X))) + ((((*((long *) ((((char *) (-3 + log_1361X))) + (4 + (((i_1658X)<<2)))))))>>2))))))<<2)))) {
        if ((verify_1661X == value_1662X)) {
          goto L10461;}
        else {
          if ((3 == (3 & stob_1659X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1659X))) + -4)))))) {
              goto L10461;}
            else {
              goto L39771;}}
          else {
            goto L39771;}}}
      else {
        goto L39771;}}}}
 L10223: {
  arg0K0 = (4 + i_1359X);
  goto L10171;}
 L39771: {
RELEASE_PROPOSAL_LOCK();
  x_1663X = Scurrent_threadS;
  addr_1664X = (((char *) (-3 + x_1663X))) + 12;S48_WRITE_BARRIER(x_1663X, addr_1664X, 1);
  *((long *) addr_1664X) = 1;
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L9737: {
  vector_1665X = arg0K0;
  if ((1 == vector_1665X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1666X;
    goto L9700;}
  else {
    arg0K0 = vector_1665X;
    goto L9700;}}
 L15270: {
  arg0K0 = (4 + i_1378X);
  goto L15248;}
 L15444: {
  arg0K0 = (4 + i_1386X);
  goto L15424;}
 L47983: {
  minutesP_1667X = arg2K0;
  pending_interruptP_return_tag = 3;
  goto pending_interruptP;
 pending_interruptP_return_3:
  x_1668X = pending_interruptP0_return_value;
  if (x_1668X) {
    goto L48013;}
  else {
    if ((0 == (Spending_interruptsS))) {s48_wait_for_event((((arg2_941X)>>2)), minutesP_1667X);
      goto L48013;}
    else {
      goto L48013;}}}
 L30791: {
  code_1669X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1669X)))))<<2)), code_1669X, 20, 4);
  n_1670X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1670X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_1436X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = name_1437X;
  arg0K0 = 2;
  goto L22895;}
 L47261: {
  val_1671X = arg0K0;
  SvalS = val_1671X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L32354: {
  table_1672X = arg0K0;
  n_1673X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_952X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L20802;}
 L40312: {
  l_1674X = arg0K0;
  i_1675X = arg0K1;
  if ((i_1675X < 0)) {
    SvalS = string_1460X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1460X))) + i_1675X)) = ((((((*((long *) (((char *) (-3 + l_1674X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_1674X))) + 4)));
    arg0K1 = (-1 + i_1675X);
    goto L40312;}}
 L42692: {
  code_1676X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1676X)))))<<2)), code_1676X, 20, 4);
  n_1677X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1677X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_989X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = list_990X;
  arg0K0 = 2;
  goto L22895;}
 L31535: {
  code_1678X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1678X)))))<<2)), code_1678X, 20, 8);
  n_1679X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1679X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1478X;
  arg0K0 = 1;
  goto L22895;}
 L31785: {
  code_1680X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1680X)))))<<2)), code_1680X, 20, 8);
  n_1681X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1681X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1488X;
  arg0K0 = 1;
  goto L22895;}
 L32031: {
  code_1682X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1682X)))))<<2)), code_1682X, 20, 8);
  n_1683X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1683X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = Kchar_1496X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1497X;
  arg0K0 = 2;
  goto L22895;}
 L42942: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_1506X))) + 4)));
  goto L42936;}
 L16422: {
  if ((3 == (3 & thing_1507X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_1507X)))));
      goto L16480;}
    else {
      goto L16430;}}
  else {
    goto L16430;}}
 L42927: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_1021X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L26394: {
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1045X));
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L23163: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_1521X))) + (((i_1050X)<<2)))));
  obj_1684X = SvalS;
  if ((3 == (3 & obj_1684X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1684X))) + -4))))>>2))))) {
      goto L23180;}
    else {
      goto L23219;}}
  else {
    goto L23219;}}
 L23205: {
  ps_error("interrupt handler is not a vector", 0);
  goto L23163;}
 L11598: {
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = channel_1522X;
  x_1685X = *((long *) ((((char *) (-3 + channel_1522X))) + 16));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1685X;
  n_1686X = Senabled_interruptsS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1686X)<<2));
  arg0K0 = 3;
  goto L23145;}
 L6858: {
  i_1687X = arg0K0;
  offset_1688X = arg0K1;
  if ((i_1687X == total_count_1066X)) {
    arg0K0 = offset_1688X;
    goto L52706;}
  else {
    env_1689X = *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1688X)))))<<2))));
    count_1690X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_1688X)));
    arg0K0 = count_1690X;
    arg0K1 = i_1687X;
    arg0K2 = (2 + offset_1688X);
    goto L6875;}}
 L52706: {
  bytes_used_1691X = arg0K0;
  SvalS = new_env_570X;
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1691X));
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L7571: {
  i_1692X = arg0K0;
  offset_1693X = arg0K1;
  if ((i_1692X == total_count_1075X)) {
    arg0K0 = offset_1693X;
    goto L52696;}
  else {
    env_1694X = *((long *) ((SstackS) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1693X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1693X))))))<<2))));
    index_1695X = 2 + offset_1693X;
    count_1696X = ((((*((unsigned char *) ((Scode_pointerS) + (1 + index_1695X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_1695X))));
    arg0K0 = count_1696X;
    arg0K1 = i_1692X;
    arg0K2 = (4 + offset_1693X);
    goto L7588;}}
 L52696: {
  bytes_used_1697X = arg0K0;
  SvalS = new_env_576X;
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1697X));
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L23450: {
  SstackS = (Sbottom_of_stackS);
  Sheap_continuationS = 1;
  ScontS = (Sbottom_of_stackS);
  code_1698X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1698X)))))<<2)), code_1698X, 20, 0);
  n_1699X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1699X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  x_1700X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1700X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_1534X;
  arg0K0 = 2;
  goto L22895;}
 L14233: {
  if ((3 == (3 & x_634X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + x_634X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L50737;}
    else {
      arg0K0 = 1;
      goto L50737;}}
  else {
    arg0K0 = 1;
    goto L50737;}}
 L41936: {
  if ((3 == (3 & n_636X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_636X))) + -4))))>>2))))) {
      arg0K0 = n_636X;
      goto L23759;}
    else {
      goto L41940;}}
  else {
    goto L41940;}}
 L42127: {
  if ((3 == (3 & n_637X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_637X))) + -4))))>>2))))) {
      arg0K0 = n_637X;
      goto L23759;}
    else {
      goto L42131;}}
  else {
    goto L42131;}}
 L42318: {
  if ((3 == (3 & n_638X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_638X))) + -4))))>>2))))) {
      arg0K0 = n_638X;
      goto L23759;}
    else {
      goto L42322;}}
  else {
    goto L42322;}}
 L18367: {
  length0_1701X = arg0K0;
  extra0_1702X = arg0K1;
  if ((0 == (3 & y_641X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18375;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_641X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18375;}}
 L20123: {
  x_1703X = arg0K0;
  y_1704X = arg0K1;
  if ((0 == (3 & x_1703X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18287;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1703X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18287;}}
 L8431: {
  if ((536870911 < lo_c_1546X)) {
    arg0K0 = arg2_643X;
    arg0K1 = y_644X;
    goto L20123;}
  else {
    if ((lo_c_1546X < 0)) {
      arg0K0 = arg2_643X;
      arg0K1 = y_644X;
      goto L20123;}
    else {
      if ((8192 < mid_c_1549X)) {
        arg0K0 = arg2_643X;
        arg0K1 = y_644X;
        goto L20123;}
      else {
        if ((a_645X < 0)) {
          if ((b_646X < 0)) {s48_make_availableAgc(16);
            if ((536870911 < c_1550X)) {
              goto L51862;}
            else {
              if ((c_1550X < -536870912)) {
                goto L51862;}
              else {
                arg0K0 = (((c_1550X)<<2));
                goto L51857;}}}
          else {
            goto L8459;}}
        else {
          if ((b_646X < 0)) {
            goto L8459;}
          else {s48_make_availableAgc(16);
            if ((536870911 < c_1550X)) {
              goto L51884;}
            else {
              if ((c_1550X < -536870912)) {
                goto L51884;}
              else {
                arg0K0 = (((c_1550X)<<2));
                goto L51879;}}}}}}}}
 L18324: {
  length0_1705X = arg0K0;
  extra0_1706X = arg0K1;
  if ((0 == (3 & y_648X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18332;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_648X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18332;}}
 L51953: {
  v_1707X = (char *) s48_long_to_bignum(c_1554X);
  v_1708X = enter_bignum(v_1707X);
  arg0K0 = v_1708X;
  goto L51948;}
 L51948: {
  val_1709X = arg0K0;
  SvalS = val_1709X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L8693: {
  x_1710X = 0 - c_1554X;s48_make_availableAgc(16);
  if ((536870911 < x_1710X)) {
    goto L51931;}
  else {
    if ((x_1710X < -536870912)) {
      goto L51931;}
    else {
      arg0K0 = (((x_1710X)<<2));
      goto L51926;}}}
 L51975: {
  v_1711X = (char *) s48_long_to_bignum(c_1554X);
  v_1712X = enter_bignum(v_1711X);
  arg0K0 = v_1712X;
  goto L51970;}
 L51970: {
  val_1713X = arg0K0;
  SvalS = val_1713X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L17892: {
  length0_1714X = arg0K0;
  extra0_1715X = arg0K1;
  if ((0 == (3 & y_651X))) {
    arg0K0 = 3;
    goto L17900;}
  else {
    arg0K0 = 0;
    goto L17900;}}
 L34727: {
  val_1716X = arg0K0;
  SvalS = val_1716X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35003: {
  val_1717X = arg0K0;
  SvalS = val_1717X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35342: {
  val_1718X = arg0K0;
  SvalS = val_1718X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35681: {
  val_1719X = arg0K0;
  SvalS = val_1719X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L35991: {
  val_1720X = arg0K0;
  SvalS = val_1720X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L20064: {
  extra1_1721X = arg0K0;
  Stemp0S = x_1174X;
  Stemp1S = y_1175X;s48_make_availableAgc(((((((8 + (((length0_1585X)<<1))) + extra0_1586X) + extra1_1721X))<<2)));
  value_1722X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1722X))) {
    v_1723X = (char *) s48_long_to_bignum((((value_1722X)>>2)));
    arg1K0 = v_1723X;
    goto L20050;}
  else {
    arg1K0 = (((char *) (-3 + value_1722X)));
    goto L20050;}}
 L9307: {
  if ((536870911 < c_1588X)) {
    arg0K0 = arg2_664X;
    arg0K1 = y_665X;
    goto L20032;}
  else {
    SvalS = (((c_1588X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}}
 L9306: {
  SvalS = ((((0 - c_1588X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L19973: {
  extra1_1724X = arg0K0;
  Stemp0S = x_1177X;
  Stemp1S = y_1178X;s48_make_availableAgc(((((((8 + (((length0_1589X)<<1))) + extra0_1590X) + extra1_1724X))<<2)));
  value_1725X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1725X))) {
    v_1726X = (char *) s48_long_to_bignum((((value_1725X)>>2)));
    arg1K0 = v_1726X;
    goto L19959;}
  else {
    arg1K0 = (((char *) (-3 + value_1725X)));
    goto L19959;}}
 L36483: {
  n_1727X = arg0K0;
  SvalS = (((n_1727X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L23698: {
  v_1728X = (char *) s48_long_to_bignum(x_1593X);
  return enter_bignum(v_1728X);}
 L19541: {
  x_1729X = arg1K0;
  v_1730X = s48_bignum_test(x_1729X);
  if ((-1 == v_1730X)) {
    v_1731X = (char *) s48_bignum_negate(x_1729X);
    arg1K0 = v_1731X;
    goto L19543;}
  else {
    arg1K0 = x_1729X;
    goto L19543;}}
 L19898: {
  v_1732X = arg0K0;
  Stemp0S = x_682X;s48_make_availableAgc((((((3 + v_1732X) + extra_1599X))<<2)));
  value_1733X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1733X))) {
    v_1734X = (char *) s48_long_to_bignum((((value_1733X)>>2)));
    arg1K0 = v_1734X;
    goto L19888;}
  else {
    arg1K0 = (((char *) (-3 + value_1733X)));
    goto L19888;}}
 L19840: {
  v_1735X = arg0K0;
  Stemp0S = x_683X;s48_make_availableAgc((((((3 + v_1735X) + extra_1603X))<<2)));
  value_1736X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1736X))) {
    v_1737X = (char *) s48_long_to_bignum((((value_1736X)>>2)));
    arg1K0 = v_1737X;
    goto L19830;}
  else {
    arg1K0 = (((char *) (-3 + value_1736X)));
    goto L19830;}}
 L18106: {
  length0_1738X = arg0K0;
  extra0_1739X = arg0K1;
  if ((0 == (3 & y_686X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18114;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_686X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18114;}}
 L18063: {
  length0_1740X = arg0K0;
  extra0_1741X = arg0K1;
  if ((0 == (3 & y_688X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18071;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_688X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18071;}}
 L18020: {
  length0_1742X = arg0K0;
  extra0_1743X = arg0K1;
  if ((0 == (3 & y_690X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18028;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_690X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18028;}}
 L44790: {
  x_1744X = arg1K0;
  external_bignum_1745X = (char *) s48_bignum_arithmetic_shift(x_1744X, y_1612X);
  v_1746X = s48_bignum_fits_in_word_p(external_bignum_1745X, 30, 1);
  if (v_1746X) {
    n_1747X = s48_bignum_to_long(external_bignum_1745X);
    arg0K0 = (((n_1747X)<<2));
    goto L44730;}
  else {
    v_1748X = enter_bignum(external_bignum_1745X);
    arg0K0 = v_1748X;
    goto L44730;}}
 L44736: {
  val_1749X = arg0K0;
  SvalS = val_1749X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L21295: {
  foo_1750X = arg0K0;
  if ((1 == foo_1750X)) {
    if ((3 == (3 & bucket_1630X))) {
      arg0K0 = (-4 & bucket_1630X);
      goto L21300;}
    else {
      arg0K0 = bucket_1630X;
      goto L21300;}}
  else {
    s2_1751X = *((long *) (((char *) (-3 + foo_1750X))));
    len_1752X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_790X))) + -4))))>>8);
    if ((len_1752X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1751X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1751X))), (void *)(((char *) (-3 + string_790X))),len_1752X)))) {
        arg0K0 = foo_1750X;
        goto L33966;}
      else {
        goto L21315;}}
    else {
      goto L21315;}}}
 L38489: {
  v_1753X = ps_close_fd(index_1631X);
  arg0K0 = v_1753X;
  goto L38484;}
 L38484: {
  status_1754X = arg0K0;
  if ((status_1754X == NO_ERRORS)) {
    arg0K0 = reason_1633X;
    goto L38328;}
  else {channel_close_error(status_1754X, index_1631X, arg2_799X);
    arg0K0 = reason_1633X;
    goto L38328;}}
 L38744: {
  if ((3 == (3 & arg4_814X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_814X))) + -4)))))) {
      if ((3 == (3 & arg4_814X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_814X))) + -4))))>>2))))) {
          arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_814X))) + -4))))>>8)));
          goto L38751;}
        else {
          goto L39017;}}
      else {
        goto L39017;}}
    else {
      goto L38812;}}
  else {
    goto L38812;}}
 L38991: {
  if ((3 == (3 & arg4_814X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg4_814X))) + -4))))>>2))))) {
      goto L38744;}
    else {
      goto L38812;}}
  else {
    goto L38812;}}
 L38812: {
  code_1755X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1755X)))))<<2)), code_1755X, 20, 4);
  n_1756X = *((unsigned char *) (Scode_pointerS));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1756X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 20;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_815X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_814X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1643X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1644X)<<2));
  if (waitP_1642X) {
    arg0K0 = 5;
    goto L38828;}
  else {
    arg0K0 = 1;
    goto L38828;}}
 L39206: {
  val_1757X = arg0K0;
  SvalS = val_1757X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L39521: {
  x_1758X = Scurrent_threadS;
  addr_1759X = (((char *) (-3 + x_1758X))) + 12;S48_WRITE_BARRIER(x_1758X, addr_1759X, proposal_840X);
  *((long *) addr_1759X) = proposal_840X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L39887: {
  copies_1760X = arg0K0;
  if ((1 == copies_1760X)) {
    log_1761X = *((long *) ((((char *) (-3 + proposal_841X))) + 4));
    arg0K0 = 0;
    goto L9086;}
  else {
    thing_1762X = *((long *) ((((char *) (-3 + copies_1760X))) + 8));
    if ((3 == (3 & thing_1762X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + thing_1762X))) + -4)))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + copies_1760X))) + 20)));
        goto L39887;}
      else {
        goto L39771;}}
    else {
      goto L39771;}}}
 L10461: {
  arg0K0 = (4 + i_1658X);
  goto L10409;}
 L9700: {
  entry_1763X = arg0K0;
  proposal_1764X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  value_1765X = Stemp0S;
  Stemp0S = 1;
  addr_1766X = ((char *) (-3 + entry_1763X));S48_WRITE_BARRIER(entry_1763X, addr_1766X, value_1765X);
  *((long *) addr_1766X) = value_1765X;
  addr_1767X = (((char *) (-3 + entry_1763X))) + 4;S48_WRITE_BARRIER(entry_1763X, addr_1767X, from_index_1371X);
  *((long *) addr_1767X) = from_index_1371X;
  value_1768X = Stemp1S;
  Stemp1S = 1;
  addr_1769X = (((char *) (-3 + entry_1763X))) + 8;S48_WRITE_BARRIER(entry_1763X, addr_1769X, value_1768X);
  *((long *) addr_1769X) = value_1768X;
  addr_1770X = (((char *) (-3 + entry_1763X))) + 12;S48_WRITE_BARRIER(entry_1763X, addr_1770X, to_index_1372X);
  *((long *) addr_1770X) = to_index_1372X;
  addr_1771X = (((char *) (-3 + entry_1763X))) + 16;S48_WRITE_BARRIER(entry_1763X, addr_1771X, count_1373X);
  *((long *) addr_1771X) = count_1373X;
  value_1772X = *((long *) ((((char *) (-3 + proposal_1764X))) + 12));
  addr_1773X = (((char *) (-3 + entry_1763X))) + 20;S48_WRITE_BARRIER(entry_1763X, addr_1773X, value_1772X);
  *((long *) addr_1773X) = value_1772X;
  addr_1774X = (((char *) (-3 + proposal_1764X))) + 12;S48_WRITE_BARRIER(proposal_1764X, addr_1774X, entry_1763X);
  *((long *) addr_1774X) = entry_1763X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L48013: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L20802: {
  i_1775X = arg0K0;
  h_1776X = arg0K1;
  if ((i_1775X < n_1673X)) {
    arg0K0 = (1 + i_1775X);
    arg0K1 = (h_1776X + (((*((unsigned char *) ((((char *) (-3 + arg2_952X))) + i_1775X))))));
    goto L20802;}
  else {
    index_1777X = 1023 & h_1776X;
    link_1778X = *((long *) ((((char *) (-3 + table_1672X))) + (((index_1777X)<<2))));
    if ((0 == (3 & link_1778X))) {
      arg0K0 = (3 + (-4 & link_1778X));
      goto L20757;}
    else {
      arg0K0 = link_1778X;
      goto L20757;}}}
 L16480: {
  v_1779X = arg3K0;
  ps_write_string(v_1779X, out_1021X);
  goto L42942;}
 L16430: {
  if ((3 == (3 & thing_1507X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_1507X))))))))));
      goto L16480;}
    else {
      goto L16438;}}
  else {
    goto L16438;}}
 L23180: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Sstack_limitS = (Sreal_stack_limitS);
    if ((s48_Spending_eventsPS)) {
      s48_Sstack_limitS = (((char *) -1));
      goto L23182;}
    else {
      goto L23182;}}
  else {
    s48_Sstack_limitS = (((char *) -1));
    goto L23182;}}
 L23219: {
  ps_error("interrupt handler is not a closure", 1, i_1050X);
  goto L23180;}
 L6875: {
  count_1780X = arg0K0;
  i_1781X = arg0K1;
  offset_1782X = arg0K2;
  if ((0 == count_1780X)) {
    arg0K0 = i_1781X;
    arg0K1 = offset_1782X;
    goto L6858;}
  else {
    value_1783X = *((long *) ((((char *) (-3 + env_1689X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1782X)))))<<2))));
    addr_1784X = (((char *) (-3 + new_env_1063X))) + (((i_1781X)<<2));S48_WRITE_BARRIER(new_env_1063X, addr_1784X, value_1783X);
    *((long *) addr_1784X) = value_1783X;
    arg0K0 = (-1 + count_1780X);
    arg0K1 = (1 + i_1781X);
    arg0K2 = (1 + offset_1782X);
    goto L6875;}}
 L7588: {
  count_1785X = arg0K0;
  i_1786X = arg0K1;
  offset_1787X = arg0K2;
  if ((0 == count_1785X)) {
    arg0K0 = i_1786X;
    arg0K1 = offset_1787X;
    goto L7571;}
  else {
    value_1788X = *((long *) ((((char *) (-3 + env_1694X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1787X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1787X))))))<<2))));
    addr_1789X = (((char *) (-3 + new_env_1072X))) + (((i_1786X)<<2));S48_WRITE_BARRIER(new_env_1072X, addr_1789X, value_1788X);
    *((long *) addr_1789X) = value_1788X;
    arg0K0 = (-1 + count_1785X);
    arg0K1 = (1 + i_1786X);
    arg0K2 = (2 + offset_1787X);
    goto L7588;}}
 L41940: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L42131: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L42322: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L18375: {
  length1_1790X = arg0K0;
  extra1_1791X = arg0K1;
  if ((length0_1701X < length1_1790X)) {
    arg0K0 = length1_1790X;
    goto L18397;}
  else {
    arg0K0 = length0_1701X;
    goto L18397;}}
 L18287: {
  length0_1792X = arg0K0;
  extra0_1793X = arg0K1;
  if ((0 == (3 & y_1704X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18295;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_1704X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18295;}}
 L51862: {
  v_1794X = (char *) s48_long_to_bignum(c_1550X);
  v_1795X = enter_bignum(v_1794X);
  arg0K0 = v_1795X;
  goto L51857;}
 L51857: {
  val_1796X = arg0K0;
  SvalS = val_1796X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L8459: {
  x_1797X = 0 - c_1550X;s48_make_availableAgc(16);
  if ((536870911 < x_1797X)) {
    goto L51840;}
  else {
    if ((x_1797X < -536870912)) {
      goto L51840;}
    else {
      arg0K0 = (((x_1797X)<<2));
      goto L51835;}}}
 L51884: {
  v_1798X = (char *) s48_long_to_bignum(c_1550X);
  v_1799X = enter_bignum(v_1798X);
  arg0K0 = v_1799X;
  goto L51879;}
 L51879: {
  val_1800X = arg0K0;
  SvalS = val_1800X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L18332: {
  length1_1801X = arg0K0;
  extra1_1802X = arg0K1;
  if ((length0_1705X < length1_1801X)) {
    arg0K0 = length1_1801X;
    goto L18354;}
  else {
    arg0K0 = length0_1705X;
    goto L18354;}}
 L51931: {
  v_1803X = (char *) s48_long_to_bignum(x_1710X);
  v_1804X = enter_bignum(v_1803X);
  arg0K0 = v_1804X;
  goto L51926;}
 L51926: {
  val_1805X = arg0K0;
  SvalS = val_1805X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L17900: {
  extra1_1806X = arg0K0;
  Stemp0S = arg2_650X;
  Stemp1S = y_651X;s48_make_availableAgc(((((((8 + (((length0_1714X)<<1))) + extra0_1715X) + extra1_1806X))<<2)));
  value_1807X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1807X))) {
    v_1808X = (char *) s48_long_to_bignum((((value_1807X)>>2)));
    arg1K0 = v_1808X;
    goto L19377;}
  else {
    arg1K0 = (((char *) (-3 + value_1807X)));
    goto L19377;}}
 L20050: {
  y_1809X = arg1K0;
  value_1810X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1810X))) {
    v_1811X = (char *) s48_long_to_bignum((((value_1810X)>>2)));
    arg1K0 = v_1811X;
    goto L20046;}
  else {
    arg1K0 = (((char *) (-3 + value_1810X)));
    goto L20046;}}
 L19959: {
  y_1812X = arg1K0;
  value_1813X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1813X))) {
    v_1814X = (char *) s48_long_to_bignum((((value_1813X)>>2)));
    arg1K0 = v_1814X;
    goto L19955;}
  else {
    arg1K0 = (((char *) (-3 + value_1813X)));
    goto L19955;}}
 L19543: {
  external_bignum_1815X = arg1K0;
  v_1816X = s48_bignum_fits_in_word_p(external_bignum_1815X, 30, 1);
  if (v_1816X) {
    n_1817X = s48_bignum_to_long(external_bignum_1815X);
    return (((n_1817X)<<2));}
  else {
    return enter_bignum(external_bignum_1815X);}}
 L19888: {
  v_1818X = arg1K0;
  external_bignum_1819X = (char *) s48_bignum_bitwise_not(v_1818X);
  v_1820X = s48_bignum_fits_in_word_p(external_bignum_1819X, 30, 1);
  if (v_1820X) {
    n_1821X = s48_bignum_to_long(external_bignum_1819X);
    arg0K0 = (((n_1821X)<<2));
    goto L37070;}
  else {
    val_1822X = enter_bignum(external_bignum_1819X);
    arg0K0 = val_1822X;
    goto L37070;}}
 L19830: {
  v_1823X = arg1K0;
  n_1824X = s48_bignum_bit_count(v_1823X);
  SvalS = (((n_1824X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L18114: {
  length1_1825X = arg0K0;
  extra1_1826X = arg0K1;
  if ((length0_1738X < length1_1825X)) {
    arg0K0 = length1_1825X;
    goto L18136;}
  else {
    arg0K0 = length0_1738X;
    goto L18136;}}
 L18071: {
  length1_1827X = arg0K0;
  extra1_1828X = arg0K1;
  if ((length0_1740X < length1_1827X)) {
    arg0K0 = length1_1827X;
    goto L18093;}
  else {
    arg0K0 = length0_1740X;
    goto L18093;}}
 L18028: {
  length1_1829X = arg0K0;
  extra1_1830X = arg0K1;
  if ((length0_1742X < length1_1829X)) {
    arg0K0 = length1_1829X;
    goto L18050;}
  else {
    arg0K0 = length0_1742X;
    goto L18050;}}
 L44730: {
  val_1831X = arg0K0;
  SvalS = val_1831X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L21300: {
  b_1832X = arg0K0;
  addr_1833X = s48_allocate_small(12);
  *((long *) addr_1833X) = 2054;
  x_1834X = 3 + (((long) (addr_1833X + 4)));
  *((long *) (((char *) (-3 + x_1834X)))) = string_790X;
  *((long *) ((((char *) (-3 + x_1834X))) + 4)) = b_1832X;
  if ((3 == (3 & x_1834X))) {
    arg0K0 = (-4 & x_1834X);
    goto L21306;}
  else {
    arg0K0 = x_1834X;
    goto L21306;}}
 L33966: {
  val_1835X = arg0K0;
  SvalS = val_1835X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L21315: {
  link_1836X = *((long *) ((((char *) (-3 + foo_1750X))) + 4));
  if ((0 == (3 & link_1836X))) {
    arg0K0 = (3 + (-4 & link_1836X));
    goto L21295;}
  else {
    arg0K0 = link_1836X;
    goto L21295;}}
 L38751: {
  y_1837X = arg0K0;
  if ((y_1837X < (start_1643X + count_1644X))) {
    goto L38812;}
  else {
    got_1838X = ps_read_fd(((((*((long *) ((((char *) (-3 + arg5_815X))) + 8))))>>2)), ((((char *) (-3 + arg4_814X))) + start_1643X), count_1644X, waitP_1642X, &eofP_1839X, &pendingP_1840X, &status_1841X);
    if ((status_1841X == NO_ERRORS)) {
      if (eofP_1839X) {
        arg0K0 = 21;
        goto L38811;}
      else {
        if (pendingP_1840X) {
          arg0K0 = 1;
          goto L38811;}
        else {
          arg0K0 = (((got_1838X)<<2));
          goto L38811;}}}
    else {
      code_1842X = current_code_vector();push_exception_continuationB(((((char *) (-3 + (Sexception_return_codeS)))) + 11), (((((Scode_pointerS) - (((char *) (-3 + code_1842X)))))<<2)), code_1842X, 96, 4);
      n_1843X = *((unsigned char *) (Scode_pointerS));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_1843X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = 96;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg5_815X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_814X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1643X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1644X)<<2));
      if (waitP_1642X) {
        arg0K0 = 5;
        goto L38794;}
      else {
        arg0K0 = 1;
        goto L38794;}}}}
 L39017: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_814X))) + -4))))>>8));
  goto L38751;}
 L38828: {
  x_1844X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1844X;
  arg0K0 = 5;
  goto L22895;}
 L9086: {
  i_1845X = arg0K0;
  stob_1846X = *((long *) ((((char *) (-3 + log_1761X))) + (((i_1845X)<<2))));
  if ((1 == stob_1846X)) {
    log_1847X = *((long *) ((((char *) (-3 + proposal_841X))) + 8));
    arg0K0 = 0;
    goto L12313;}
  else {
    verify_1848X = *((long *) ((((char *) (-3 + log_1761X))) + (12 + (((i_1845X)<<2)))));
    value_1849X = *((long *) ((((char *) (-3 + log_1761X))) + (8 + (((i_1845X)<<2)))));
    if ((verify_1848X == value_1849X)) {
      goto L9118;}
    else {
      addr_1850X = (((char *) (-3 + stob_1846X))) + (-4 & (*((long *) ((((char *) (-3 + log_1761X))) + (4 + (((i_1845X)<<2)))))));S48_WRITE_BARRIER(stob_1846X, addr_1850X, value_1849X);
      *((long *) addr_1850X) = value_1849X;
      goto L9118;}}}
 L20757: {
  bucket_1851X = arg0K0;
  arg0K0 = 1;
  arg0K1 = bucket_1851X;
  goto L20764;}
 L16438: {
  if ((1 == thing_1507X)) {
    goto L16441;}
  else {
    if ((5 == thing_1507X)) {
      goto L16441;}
    else {
      if ((25 == thing_1507X)) {
        arg3K0 = "()";
        goto L16480;}
      else {
        if ((3 == (3 & thing_1507X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L16480;}
          else {
            goto L16454;}}
        else {
          goto L16454;}}}}}
 L23182: {
  Scode_pointerS = (((char *) 0));
  Slosing_opcodeS = (0 - i_1050X);
  arg0K0 = arg_count_1520X;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L26761;}
 L18397: {
  v_1852X = arg0K0;
  Stemp0S = arg2_640X;
  Stemp1S = y_641X;s48_make_availableAgc(((((((3 + v_1852X) + extra0_1702X) + extra1_1791X))<<2)));
  value_1853X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1853X))) {
    v_1854X = (char *) s48_long_to_bignum((((value_1853X)>>2)));
    arg1K0 = v_1854X;
    goto L20289;}
  else {
    arg1K0 = (((char *) (-3 + value_1853X)));
    goto L20289;}}
 L18295: {
  length1_1855X = arg0K0;
  extra1_1856X = arg0K1;
  Stemp0S = x_1703X;
  Stemp1S = y_1704X;s48_make_availableAgc(((((((2 + (length0_1792X + length1_1855X)) + extra0_1793X) + extra1_1856X))<<2)));
  value_1857X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1857X))) {
    v_1858X = (char *) s48_long_to_bignum((((value_1857X)>>2)));
    arg1K0 = v_1858X;
    goto L20141;}
  else {
    arg1K0 = (((char *) (-3 + value_1857X)));
    goto L20141;}}
 L51840: {
  v_1859X = (char *) s48_long_to_bignum(x_1797X);
  v_1860X = enter_bignum(v_1859X);
  arg0K0 = v_1860X;
  goto L51835;}
 L51835: {
  val_1861X = arg0K0;
  SvalS = val_1861X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L18354: {
  v_1862X = arg0K0;
  Stemp0S = arg2_647X;
  Stemp1S = y_648X;s48_make_availableAgc(((((((3 + v_1862X) + extra0_1706X) + extra1_1802X))<<2)));
  value_1863X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1863X))) {
    v_1864X = (char *) s48_long_to_bignum((((value_1863X)>>2)));
    arg1K0 = v_1864X;
    goto L20215;}
  else {
    arg1K0 = (((char *) (-3 + value_1863X)));
    goto L20215;}}
 L19377: {
  y_1865X = arg1K0;
  value_1866X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1866X))) {
    v_1867X = (char *) s48_long_to_bignum((((value_1866X)>>2)));
    arg1K0 = v_1867X;
    goto L19373;}
  else {
    arg1K0 = (((char *) (-3 + value_1866X)));
    goto L19373;}}
 L20046: {
  x_1868X = arg1K0;
  external_bignum_1869X = (char *)s48_bignum_quotient(x_1868X, y_1809X);
  v_1870X = s48_bignum_fits_in_word_p(external_bignum_1869X, 30, 1);
  if (v_1870X) {
    n_1871X = s48_bignum_to_long(external_bignum_1869X);
    arg0K0 = (((n_1871X)<<2));
    goto L52686;}
  else {
    val_1872X = enter_bignum(external_bignum_1869X);
    arg0K0 = val_1872X;
    goto L52686;}}
 L19955: {
  x_1873X = arg1K0;
  external_bignum_1874X = (char *)s48_bignum_remainder(x_1873X, y_1812X);
  v_1875X = s48_bignum_fits_in_word_p(external_bignum_1874X, 30, 1);
  if (v_1875X) {
    n_1876X = s48_bignum_to_long(external_bignum_1874X);
    arg0K0 = (((n_1876X)<<2));
    goto L52686;}
  else {
    val_1877X = enter_bignum(external_bignum_1874X);
    arg0K0 = val_1877X;
    goto L52686;}}
 L37070: {
  val_1878X = arg0K0;
  SvalS = val_1878X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L18136: {
  v_1879X = arg0K0;
  Stemp0S = arg2_685X;
  Stemp1S = y_686X;s48_make_availableAgc(((((((3 + v_1879X) + extra0_1739X) + extra1_1826X))<<2)));
  value_1880X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1880X))) {
    v_1881X = (char *) s48_long_to_bignum((((value_1880X)>>2)));
    arg1K0 = v_1881X;
    goto L19763;}
  else {
    arg1K0 = (((char *) (-3 + value_1880X)));
    goto L19763;}}
 L18093: {
  v_1882X = arg0K0;
  Stemp0S = arg2_687X;
  Stemp1S = y_688X;s48_make_availableAgc(((((((3 + v_1882X) + extra0_1741X) + extra1_1828X))<<2)));
  value_1883X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1883X))) {
    v_1884X = (char *) s48_long_to_bignum((((value_1883X)>>2)));
    arg1K0 = v_1884X;
    goto L19689;}
  else {
    arg1K0 = (((char *) (-3 + value_1883X)));
    goto L19689;}}
 L18050: {
  v_1885X = arg0K0;
  Stemp0S = arg2_689X;
  Stemp1S = y_690X;s48_make_availableAgc(((((((3 + v_1885X) + extra0_1743X) + extra1_1830X))<<2)));
  value_1886X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1886X))) {
    v_1887X = (char *) s48_long_to_bignum((((value_1886X)>>2)));
    arg1K0 = v_1887X;
    goto L19615;}
  else {
    arg1K0 = (((char *) (-3 + value_1886X)));
    goto L19615;}}
 L21306: {
  value_1888X = arg0K0;
  addr_1889X = (((char *) (-3 + table_789X))) + (((index_1306X)<<2));S48_WRITE_BARRIER(table_789X, addr_1889X, value_1888X);
  *((long *) addr_1889X) = value_1888X;
  arg0K0 = x_1834X;
  goto L33966;}
 L38811: {
  val_1890X = arg0K0;
  SvalS = val_1890X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L38794: {
  x_1891X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1891X;
  merged_arg0K0 = status_1841X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 5;
  goto get_error_string;
 get_error_string_return_5:
  x_1892X = get_error_string0_return_value;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1892X;
  arg0K0 = 6;
  goto L22895;}
 L12313: {
  i_1893X = arg0K0;
  stob_1894X = *((long *) ((((char *) (-3 + log_1847X))) + (((i_1893X)<<2))));
  if ((1 == stob_1894X)) {
    copies_1895X = *((long *) ((((char *) (-3 + proposal_841X))) + 12));
    arg0K0 = copies_1895X;
    goto L12228;}
  else {
    verify_1896X = *((long *) ((((char *) (-3 + log_1847X))) + (12 + (((i_1893X)<<2)))));
    value_1897X = *((long *) ((((char *) (-3 + log_1847X))) + (8 + (((i_1893X)<<2)))));
    if ((verify_1896X == value_1897X)) {
      goto L12345;}
    else {
      *((unsigned char *) ((((char *) (-3 + stob_1894X))) + ((((*((long *) ((((char *) (-3 + log_1847X))) + (4 + (((i_1893X)<<2)))))))>>2)))) = (((value_1897X)>>2));
      goto L12345;}}}
 L9118: {
  arg0K0 = (4 + i_1845X);
  goto L9086;}
 L20764: {
  previous_foo_1898X = arg0K0;
  foo_1899X = arg0K1;
  if ((1 == foo_1899X)) {
    goto L32356;}
  else {
    s2_1900X = *((long *) (((char *) (-3 + foo_1899X))));
    len_1901X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_952X))) + -4))))>>8);
    if ((len_1901X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1900X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1900X))), (void *)(((char *) (-3 + arg2_952X))),len_1901X)))) {
        if ((1 == previous_foo_1898X)) {
          value_1902X = *((long *) ((((char *) (-3 + foo_1899X))) + 12));
          addr_1903X = (((char *) (-3 + table_1672X))) + (((index_1777X)<<2));S48_WRITE_BARRIER(table_1672X, addr_1903X, value_1902X);
          *((long *) addr_1903X) = value_1902X;
          goto L32356;}
        else {
          val_1904X = *((long *) ((((char *) (-3 + foo_1899X))) + 12));
          addr_1905X = (((char *) (-3 + previous_foo_1898X))) + 12;S48_WRITE_BARRIER(previous_foo_1898X, addr_1905X, val_1904X);
          *((long *) addr_1905X) = val_1904X;
          goto L32356;}}
      else {
        goto L20832;}}
    else {
      goto L20832;}}}
 L16441: {
  if ((1 == thing_1507X)) {
    arg3K0 = "#f";
    goto L16480;}
  else {
    arg3K0 = "#t";
    goto L16480;}}
 L16454: {
  if ((3 == (3 & thing_1507X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L16480;}
    else {
      goto L16458;}}
  else {
    goto L16458;}}
 L20289: {
  y_1906X = arg1K0;
  value_1907X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1907X))) {
    v_1908X = (char *) s48_long_to_bignum((((value_1907X)>>2)));
    arg1K0 = v_1908X;
    goto L20285;}
  else {
    arg1K0 = (((char *) (-3 + value_1907X)));
    goto L20285;}}
 L20141: {
  y_1909X = arg1K0;
  value_1910X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1910X))) {
    v_1911X = (char *) s48_long_to_bignum((((value_1910X)>>2)));
    arg1K0 = v_1911X;
    goto L20137;}
  else {
    arg1K0 = (((char *) (-3 + value_1910X)));
    goto L20137;}}
 L20215: {
  y_1912X = arg1K0;
  value_1913X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1913X))) {
    v_1914X = (char *) s48_long_to_bignum((((value_1913X)>>2)));
    arg1K0 = v_1914X;
    goto L20211;}
  else {
    arg1K0 = (((char *) (-3 + value_1913X)));
    goto L20211;}}
 L19373: {
  x_1915X = arg1K0;
  div_by_zeroP_1916X = s48_bignum_divide(x_1915X, y_1865X, &quot_1917X, &rem_1918X);
  if (div_by_zeroP_1916X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = 0;
    goto L44444;}
  else {
    v_1919X = s48_bignum_fits_in_word_p(rem_1918X, 30, 1);
    if (v_1919X) {
      n_1920X = s48_bignum_to_long(rem_1918X);
      arg0K0 = (((n_1920X)<<2));
      goto L19400;}
    else {
      v_1921X = enter_bignum(rem_1918X);
      arg0K0 = v_1921X;
      goto L19400;}}}
 L52686: {
  val_1922X = arg0K0;
  SvalS = val_1922X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L19763: {
  y_1923X = arg1K0;
  value_1924X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1924X))) {
    v_1925X = (char *) s48_long_to_bignum((((value_1924X)>>2)));
    arg1K0 = v_1925X;
    goto L19759;}
  else {
    arg1K0 = (((char *) (-3 + value_1924X)));
    goto L19759;}}
 L19689: {
  y_1926X = arg1K0;
  value_1927X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1927X))) {
    v_1928X = (char *) s48_long_to_bignum((((value_1927X)>>2)));
    arg1K0 = v_1928X;
    goto L19685;}
  else {
    arg1K0 = (((char *) (-3 + value_1927X)));
    goto L19685;}}
 L19615: {
  y_1929X = arg1K0;
  value_1930X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1930X))) {
    v_1931X = (char *) s48_long_to_bignum((((value_1930X)>>2)));
    arg1K0 = v_1931X;
    goto L19611;}
  else {
    arg1K0 = (((char *) (-3 + value_1930X)));
    goto L19611;}}
 L12228: {
  copies_1932X = arg0K0;
  if ((1 == copies_1932X)) {
    value_1933X = Sempty_logS;
    addr_1934X = (((char *) (-3 + proposal_841X))) + 4;S48_WRITE_BARRIER(proposal_841X, addr_1934X, value_1933X);
    *((long *) addr_1934X) = value_1933X;
    value_1935X = Sempty_logS;
    addr_1936X = (((char *) (-3 + proposal_841X))) + 8;S48_WRITE_BARRIER(proposal_841X, addr_1936X, value_1935X);
    *((long *) addr_1936X) = value_1935X;
    addr_1937X = (((char *) (-3 + proposal_841X))) + 12;S48_WRITE_BARRIER(proposal_841X, addr_1937X, 1);
    *((long *) addr_1937X) = 1;RELEASE_PROPOSAL_LOCK();
    x_1938X = Scurrent_threadS;
    addr_1939X = (((char *) (-3 + x_1938X))) + 12;S48_WRITE_BARRIER(x_1938X, addr_1939X, 1);
    *((long *) addr_1939X) = 1;
    SvalS = 5;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L25070;}
  else {
    stob_1940X = *((long *) (((char *) (-3 + copies_1932X))));
    v_1941X = (((*((long *) ((((char *) (-3 + copies_1932X))) + 4))))>>2);
    stob_1942X = *((long *) ((((char *) (-3 + copies_1932X))) + 8));
    v_1943X = (((*((long *) ((((char *) (-3 + copies_1932X))) + 12))))>>2);
    v_1944X = (((*((long *) ((((char *) (-3 + copies_1932X))) + 16))))>>2);
    memcpy((void *)((((char *) (-3 + stob_1942X))) + v_1943X), (void *)((((char *) (-3 + stob_1940X))) + v_1941X),v_1944X);
    arg0K0 = (*((long *) ((((char *) (-3 + copies_1932X))) + 20)));
    goto L12228;}}
 L12345: {
  arg0K0 = (4 + i_1893X);
  goto L12313;}
 L32356: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L20832: {
  link_1945X = *((long *) ((((char *) (-3 + foo_1899X))) + 12));
  if ((0 == (3 & link_1945X))) {
    arg0K0 = foo_1899X;
    arg0K1 = (3 + (-4 & link_1945X));
    goto L20764;}
  else {
    arg0K0 = foo_1899X;
    arg0K1 = link_1945X;
    goto L20764;}}
 L16458: {
  if ((3 == (3 & thing_1507X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L16480;}
    else {
      goto L16462;}}
  else {
    goto L16462;}}
 L20285: {
  x_1946X = arg1K0;
  external_bignum_1947X = (char *)s48_bignum_add(x_1946X, y_1906X);
  v_1948X = s48_bignum_fits_in_word_p(external_bignum_1947X, 30, 1);
  if (v_1948X) {
    n_1949X = s48_bignum_to_long(external_bignum_1947X);
    arg0K0 = (((n_1949X)<<2));
    goto L34119;}
  else {
    val_1950X = enter_bignum(external_bignum_1947X);
    arg0K0 = val_1950X;
    goto L34119;}}
 L20137: {
  x_1951X = arg1K0;
  external_bignum_1952X = (char *)s48_bignum_multiply(x_1951X, y_1909X);
  v_1953X = s48_bignum_fits_in_word_p(external_bignum_1952X, 30, 1);
  if (v_1953X) {
    n_1954X = s48_bignum_to_long(external_bignum_1952X);
    arg0K0 = (((n_1954X)<<2));
    goto L52686;}
  else {
    val_1955X = enter_bignum(external_bignum_1952X);
    arg0K0 = val_1955X;
    goto L52686;}}
 L20211: {
  x_1956X = arg1K0;
  external_bignum_1957X = (char *)s48_bignum_subtract(x_1956X, y_1912X);
  v_1958X = s48_bignum_fits_in_word_p(external_bignum_1957X, 30, 1);
  if (v_1958X) {
    n_1959X = s48_bignum_to_long(external_bignum_1957X);
    arg0K0 = (((n_1959X)<<2));
    goto L34424;}
  else {
    val_1960X = enter_bignum(external_bignum_1957X);
    arg0K0 = val_1960X;
    goto L34424;}}
 L44444: {
  div_by_zeroP_1961X = arg2K0;
  quot_1962X = arg0K1;
  rem_1963X = arg0K2;
  if (div_by_zeroP_1961X) {
    arg0K0 = arg2_650X;
    arg0K1 = y_651X;
    goto L23790;}
  else {
    if ((0 == (3 & rem_1963X))) {
      if ((0 == rem_1963X)) {
        SvalS = quot_1962X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L25070;}
      else {
        arg0K0 = arg2_650X;
        arg0K1 = y_651X;
        goto L23790;}}
    else {
      arg0K0 = arg2_650X;
      arg0K1 = y_651X;
      goto L23790;}}}
 L19400: {
  v_1964X = arg0K0;
  v_1965X = s48_bignum_fits_in_word_p(quot_1917X, 30, 1);
  if (v_1965X) {
    n_1966X = s48_bignum_to_long(quot_1917X);
    arg2K0 = 0;
    arg0K1 = (((n_1966X)<<2));
    arg0K2 = v_1964X;
    goto L44444;}
  else {
    v_1967X = enter_bignum(quot_1917X);
    arg2K0 = 0;
    arg0K1 = v_1967X;
    arg0K2 = v_1964X;
    goto L44444;}}
 L19759: {
  x_1968X = arg1K0;
  external_bignum_1969X = (char *) s48_bignum_bitwise_and(x_1968X, y_1923X);
  v_1970X = s48_bignum_fits_in_word_p(external_bignum_1969X, 30, 1);
  if (v_1970X) {
    n_1971X = s48_bignum_to_long(external_bignum_1969X);
    arg0K0 = (((n_1971X)<<2));
    goto L37284;}
  else {
    val_1972X = enter_bignum(external_bignum_1969X);
    arg0K0 = val_1972X;
    goto L37284;}}
 L19685: {
  x_1973X = arg1K0;
  external_bignum_1974X = (char *) s48_bignum_bitwise_ior(x_1973X, y_1926X);
  v_1975X = s48_bignum_fits_in_word_p(external_bignum_1974X, 30, 1);
  if (v_1975X) {
    n_1976X = s48_bignum_to_long(external_bignum_1974X);
    arg0K0 = (((n_1976X)<<2));
    goto L37463;}
  else {
    val_1977X = enter_bignum(external_bignum_1974X);
    arg0K0 = val_1977X;
    goto L37463;}}
 L19611: {
  x_1978X = arg1K0;
  external_bignum_1979X = (char *) s48_bignum_bitwise_xor(x_1978X, y_1929X);
  v_1980X = s48_bignum_fits_in_word_p(external_bignum_1979X, 30, 1);
  if (v_1980X) {
    n_1981X = s48_bignum_to_long(external_bignum_1979X);
    arg0K0 = (((n_1981X)<<2));
    goto L37642;}
  else {
    val_1982X = enter_bignum(external_bignum_1979X);
    arg0K0 = val_1982X;
    goto L37642;}}
 L16462: {
  if ((3 == (3 & thing_1507X))) {
    if ((12 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
      arg3K0 = "#{template}";
      goto L16480;}
    else {
      goto L16466;}}
  else {
    goto L16466;}}
 L34119: {
  val_1983X = arg0K0;
  SvalS = val_1983X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L34424: {
  val_1984X = arg0K0;
  SvalS = val_1984X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L37284: {
  val_1985X = arg0K0;
  SvalS = val_1985X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L37463: {
  val_1986X = arg0K0;
  SvalS = val_1986X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L37642: {
  val_1987X = arg0K0;
  SvalS = val_1987X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L25070;}
 L16466: {
  if ((3 == (3 & thing_1507X))) {
    if ((4 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
      arg3K0 = "#{location}";
      goto L16480;}
    else {
      goto L16470;}}
  else {
    goto L16470;}}
 L16470: {
  if ((3 == (3 & thing_1507X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
      arg3K0 = "#{code-vector}";
      goto L16480;}
    else {
      goto L16474;}}
  else {
    goto L16474;}}
 L16474: {
  if ((3 == (3 & thing_1507X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + thing_1507X))) + -4))))>>2))))) {
      arg3K0 = "#{continuation}";
      goto L16480;}
    else {
      arg3K0 = "???";
      goto L16480;}}
  else {
    arg3K0 = "???";
    goto L16480;}}
 loseD0: {
  message_441X = merged_arg3K0;{
  ps_write_string("Template UIDs: ", (stderr));
  current_code_1988X = current_code_vector();
  out_1989X = stderr;
  merged_arg1K0 = (SstackS);
  merged_arg0K1 = (((((ScontS) - (SstackS)))>>2));
  merged_arg0K2 = current_code_1988X;
  find_template_return_tag = 0;
  goto find_template;
 find_template_return_0:
  template_1990X = find_template0_return_value;
  merged_arg0K0 = template_1990X;
  merged_arg2K1 = 0;
  merged_arg4K2 = out_1989X;
  maybe_write_template_return_tag = 0;
  goto maybe_write_template;
 maybe_write_template_return_0:
  not_firstP_1991X = maybe_write_template0_return_value;
  arg1K0 = (ScontS);
  arg2K1 = not_firstP_1991X;
  goto L20660;}
 L20660: {
  cont_1992X = arg1K0;
  not_firstP_1993X = arg2K1;
  if ((cont_1992X == (Sbottom_of_stackS))) {
    cont_1994X = Sheap_continuationS;
    arg0K0 = cont_1994X;
    arg2K1 = not_firstP_1993X;
    goto L18662;}
  else {
    code_pointer_1995X = ((char *) (*((long *) cont_1992X)));
    pointer_1996X = code_pointer_1995X + -5;
    v_1997X = 3 + (((long) (code_pointer_1995X + (0 - (((((*((unsigned char *) pointer_1996X)))<<8)) + (*((unsigned char *) (pointer_1996X + 1))))))));
    pointer_1998X = (((char *) (*((long *) cont_1992X)))) + -2;
    size_1999X = ((((*((unsigned char *) pointer_1998X)))<<8)) + (*((unsigned char *) (pointer_1998X + 1)));
    if ((65535 == size_1999X)) {
      arg0K0 = ((((*((long *) (cont_1992X + 4))))>>2));
      goto L15831;}
    else {
      arg0K0 = size_1999X;
      goto L15831;}}}
 L18662: {
  cont_2000X = arg0K0;
  not_firstP_2001X = arg2K1;
  if ((3 == (3 & cont_2000X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_2000X))) + -4))))>>2))))) {
      merged_arg1K0 = (((char *) (-3 + cont_2000X)));
      merged_arg0K1 = ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_2000X))) + -4))))>>8))))>>2));
      merged_arg0K2 = (*((long *) ((((char *) (-3 + cont_2000X))) + 4)));
      find_template_return_tag = 1;
      goto find_template;
     find_template_return_1:
      v_2002X = find_template0_return_value;
      merged_arg0K0 = v_2002X;
      merged_arg2K1 = not_firstP_2001X;
      merged_arg4K2 = out_1989X;
      maybe_write_template_return_tag = 1;
      goto maybe_write_template;
     maybe_write_template_return_1:
      v_2003X = maybe_write_template0_return_value;
      arg0K0 = (*((long *) ((((char *) (-3 + cont_2000X))) + 8)));
      arg2K1 = v_2003X;
      goto L18662;}
    else {
      goto L22993;}}
  else {
    goto L22993;}}
 L15831: {
  v_2004X = arg0K0;
  merged_arg1K0 = (cont_1992X + 4);
  merged_arg0K1 = v_2004X;
  merged_arg0K2 = v_1997X;
  find_template_return_tag = 2;
  goto find_template;
 find_template_return_2:
  v_2005X = find_template0_return_value;
  merged_arg0K0 = v_2005X;
  merged_arg2K1 = not_firstP_1993X;
  merged_arg4K2 = out_1989X;
  maybe_write_template_return_tag = 2;
  goto maybe_write_template;
 maybe_write_template_return_2:
  v_2006X = maybe_write_template0_return_value;
  pointer_2007X = (((char *) (*((long *) cont_1992X)))) + -2;
  size_2008X = ((((*((unsigned char *) pointer_2007X)))<<8)) + (*((unsigned char *) (pointer_2007X + 1)));
  if ((65535 == size_2008X)) {
    arg0K0 = ((((*((long *) (cont_1992X + 4))))>>2));
    goto L20678;}
  else {
    arg0K0 = size_2008X;
    goto L20678;}}
 L22993: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  why_2009X = (((*((long *) ((SstackS) + (((nargs_477X)<<2))))))>>2);
  if ((why_2009X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (-4 + (((nargs_477X)<<2)))))))))))))) {
      ps_error(message_441X, 3, opcode_478X, why_2009X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (-4 + (((nargs_477X)<<2))))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L22940;}}
  else {
    goto L22940;}}
 L20678: {
  v_2010X = arg0K0;
  arg1K0 = (cont_1992X + (4 + (((v_2010X)<<2))));
  arg2K1 = v_2006X;
  goto L20660;}
 L22940: {
  ps_error(message_441X, 2, opcode_478X, why_2009X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 maybe_write_template: {
  template_438X = merged_arg0K0;
  not_firstP_439X = merged_arg2K1;
  out_440X = merged_arg4K2;{
  if (not_firstP_439X) {
    ps_write_string(" <- ", out_440X);
    goto L16070;}
  else {
    goto L16070;}}
 L16070: {
  if ((3 == (3 & template_438X))) {
    if ((12 == (31 & ((((*((long *) ((((char *) (-3 + template_438X))) + -4))))>>2))))) {
      name_2011X = *((long *) ((((char *) (-3 + template_438X))) + 4));
      if ((0 == (3 & name_2011X))) {
        ps_write_integer((((name_2011X)>>2)), out_440X);
        maybe_write_template0_return_value = 1;
        goto maybe_write_template_return;}
      else {
        if ((3 == (3 & name_2011X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + name_2011X))) + -4))))>>2))))) {
            obj_2012X = *((long *) ((((char *) (-3 + name_2011X))) + 8));
            if ((3 == (3 & obj_2012X))) {
              if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_2012X))) + -4))))>>2))))) {
                ps_write_string((((char *)(((char *) (-3 + (*((long *) ((((char *) (-3 + name_2011X))) + 8)))))))), out_440X);
                maybe_write_template0_return_value = 1;
                goto maybe_write_template_return;}
              else {
                goto L16105;}}
            else {
              goto L16105;}}
          else {
            goto L16105;}}
        else {
          goto L16105;}}}
    else {
      goto L16128;}}
  else {
    goto L16128;}}
 L16105: {
  if ((3 == (3 & name_2011X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + name_2011X))) + -4))))>>2))))) {
      obj_2013X = *((long *) ((((char *) (-3 + name_2011X))) + 8));
      if ((3 == (3 & obj_2013X))) {
        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_2013X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + name_2011X))) + 8))))))))))))), out_440X);
          maybe_write_template0_return_value = 1;
          goto maybe_write_template_return;}
        else {
          goto L16125;}}
      else {
        goto L16125;}}
    else {
      goto L16125;}}
  else {
    goto L16125;}}
 L16128: {
  ps_write_string(" ?? ", out_440X);
  maybe_write_template0_return_value = 1;
  goto maybe_write_template_return;}
 L16125: {
  ps_write_string("?", out_440X);
  maybe_write_template0_return_value = 1;
  goto maybe_write_template_return;}
 maybe_write_template_return:
  switch (maybe_write_template_return_tag) {
  case 0: goto maybe_write_template_return_0;
  case 1: goto maybe_write_template_return_1;
  default: goto maybe_write_template_return_2;
  }}

 find_template: {
  start_435X = merged_arg1K0;
  count_436X = merged_arg0K1;
  code_vector_437X = merged_arg0K2;{
  arg0K0 = 0;
  goto L14124;}
 L14124: {
  i_2014X = arg0K0;
  if ((i_2014X == count_436X)) {
    find_template0_return_value = 1;
    goto find_template_return;}
  else {
    next_2015X = *((long *) (start_435X + (((i_2014X)<<2))));
    if ((3 == (3 & next_2015X))) {
      if ((12 == (31 & ((((*((long *) ((((char *) (-3 + next_2015X))) + -4))))>>2))))) {
        if (((*((long *) (((char *) (-3 + next_2015X))))) == code_vector_437X)) {
          find_template0_return_value = next_2015X;
          goto find_template_return;}
        else {
          goto L14146;}}
      else {
        goto L14146;}}
    else {
      goto L14146;}}}
 L14146: {
  arg0K0 = (1 + i_2014X);
  goto L14124;}
 find_template_return:
  switch (find_template_return_tag) {
  case 0: goto find_template_return_0;
  case 1: goto find_template_return_1;
  default: goto find_template_return_2;
  }}

 rest_list_setupAgc: {
  wants_stack_args_431X = merged_arg0K0;
  stack_arg_count_432X = merged_arg0K1;
  list_args_433X = merged_arg0K2;
  list_arg_count_434X = merged_arg0K3;{
  if ((stack_arg_count_432X == wants_stack_args_431X)) {
    merged_arg0K0 = list_args_433X;
    merged_arg0K1 = list_arg_count_434X;
    copy_listSAgc_return_tag = 1;
    goto copy_listSAgc;
   copy_listSAgc_return_1:
    x_2016X = copy_listSAgc0_return_value;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2016X;
    goto rest_list_setupAgc_return;}
  else {
    if ((stack_arg_count_432X < wants_stack_args_431X)) {
      count_2017X = wants_stack_args_431X - stack_arg_count_432X;
      merged_arg0K0 = list_args_433X;
      merged_arg0K1 = count_2017X;
      push_list_return_tag = 4;
      goto push_list;
     push_list_return_4:
      v_2018X = push_list0_return_value;
      merged_arg0K0 = v_2018X;
      merged_arg0K1 = (list_arg_count_434X - count_2017X);
      copy_listSAgc_return_tag = 2;
      goto copy_listSAgc;
     copy_listSAgc_return_2:
      x_2019X = copy_listSAgc0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_2019X;
      goto rest_list_setupAgc_return;}
    else {
      merged_arg0K0 = list_args_433X;
      merged_arg0K1 = list_arg_count_434X;
      copy_listSAgc_return_tag = 3;
      goto copy_listSAgc;
     copy_listSAgc_return_3:
      v_2020X = copy_listSAgc0_return_value;
      merged_arg0K0 = v_2020X;
      merged_arg0K1 = (stack_arg_count_432X - wants_stack_args_431X);
      pop_args_GlistSAgc_return_tag = 5;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_5:
      x_2021X = pop_args_GlistSAgc0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_2021X;
      goto rest_list_setupAgc_return;}}}
 rest_list_setupAgc_return:
  switch (rest_list_setupAgc_return_tag) {
  case 0: goto rest_list_setupAgc_return_0;
  default: goto rest_list_setupAgc_return_1;
  }}

 push_list: {
  list_429X = merged_arg0K0;
  count_430X = merged_arg0K1;{
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = list_429X;
  merged_arg0K0 = count_430X;
  ensure_stack_spaceB_return_tag = 1;
  goto ensure_stack_spaceB;
 ensure_stack_spaceB_return_1:
  list_2022X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg0K0 = count_430X;
  arg0K1 = list_2022X;
  goto L22171;}
 L22171: {
  i_2023X = arg0K0;
  l_2024X = arg0K1;
  if ((0 < i_2023X)) {
    x_2025X = *((long *) (((char *) (-3 + l_2024X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2025X;
    arg0K0 = (-1 + i_2023X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_2024X))) + 4)));
    goto L22171;}
  else {
    push_list0_return_value = l_2024X;
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
  need_428X = merged_arg0K0;{
  if ((((SstackS) + (256 - (((need_428X)<<2)))) < (s48_Sstack_limitS))) {
    interruptP_2026X = (s48_Sstack_limitS) == (((char *) -1));
    s48_Sstack_limitS = (Sreal_stack_limitS);
    if ((((SstackS) + (256 - (((need_428X)<<2)))) < (Sreal_stack_limitS))) {s48_copy_stack_into_heap();
      if ((((SstackS) + (256 - (((need_428X)<<2)))) < (Sreal_stack_limitS))) {
        ps_error("VM's stack is too small (how can this happen?)", 0);
        ensure_stack_spaceB0_return_value = interruptP_2026X;
        goto ensure_stack_spaceB_return;}
      else {
        ensure_stack_spaceB0_return_value = interruptP_2026X;
        goto ensure_stack_spaceB_return;}}
    else {
      ensure_stack_spaceB0_return_value = interruptP_2026X;
      goto ensure_stack_spaceB_return;}}
  else {
    ensure_stack_spaceB0_return_value = 0;
    goto ensure_stack_spaceB_return;}}
 ensure_stack_spaceB_return:
  switch (ensure_stack_spaceB_return_tag) {
  case 0: goto ensure_stack_spaceB_return_0;
  default: goto ensure_stack_spaceB_return_1;
  }}

 pop_args_GlistSAgc: {
  start_426X = merged_arg0K0;
  count_427X = merged_arg0K1;{
  Stemp0S = start_426X;s48_make_availableAgc(((((3 * count_427X))<<2)));
  value_2027X = Stemp0S;
  Stemp0S = 1;
  arg0K0 = value_2027X;
  arg0K1 = count_427X;
  goto L17348;}
 L17348: {
  args_2028X = arg0K0;
  count_2029X = arg0K1;
  if ((0 == count_2029X)) {
    pop_args_GlistSAgc0_return_value = args_2028X;
    goto pop_args_GlistSAgc_return;}
  else {
    a_2030X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    addr_2031X = s48_allocate_small(12);
    *((long *) addr_2031X) = 2050;
    x_2032X = 3 + (((long) (addr_2031X + 4)));
    *((long *) (((char *) (-3 + x_2032X)))) = a_2030X;
    *((long *) ((((char *) (-3 + x_2032X))) + 4)) = args_2028X;
    arg0K0 = x_2032X;
    arg0K1 = (-1 + count_2029X);
    goto L17348;}}
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
  list_424X = merged_arg0K0;
  length_425X = merged_arg0K1;{
  if ((0 == length_425X)) {
    copy_listSAgc0_return_value = 25;
    goto copy_listSAgc_return;}
  else {
    Stemp0S = list_424X;s48_make_availableAgc(((((3 * length_425X))<<2)));
    value_2033X = Stemp0S;
    Stemp0S = 1;
    a_2034X = *((long *) (((char *) (-3 + value_2033X))));
    addr_2035X = s48_allocate_small(12);
    *((long *) addr_2035X) = 2050;
    x_2036X = 3 + (((long) (addr_2035X + 4)));
    *((long *) (((char *) (-3 + x_2036X)))) = a_2034X;
    *((long *) ((((char *) (-3 + x_2036X))) + 4)) = 25;
    arg0K0 = (*((long *) ((((char *) (-3 + value_2033X))) + 4)));
    arg0K1 = x_2036X;
    goto L17236;}}
 L17236: {
  l_2037X = arg0K0;
  last_2038X = arg0K1;
  if ((25 == l_2037X)) {
    copy_listSAgc0_return_value = x_2036X;
    goto copy_listSAgc_return;}
  else {
    a_2039X = *((long *) (((char *) (-3 + l_2037X))));
    addr_2040X = s48_allocate_small(12);
    *((long *) addr_2040X) = 2050;
    x_2041X = 3 + (((long) (addr_2040X + 4)));
    *((long *) (((char *) (-3 + x_2041X)))) = a_2039X;
    *((long *) ((((char *) (-3 + x_2041X))) + 4)) = 25;
    addr_2042X = (((char *) (-3 + last_2038X))) + 4;S48_WRITE_BARRIER(last_2038X, addr_2042X, x_2041X);
    *((long *) addr_2042X) = x_2041X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_2037X))) + 4)));
    arg0K1 = x_2041X;
    goto L17236;}}
 copy_listSAgc_return:
  switch (copy_listSAgc_return_tag) {
  case 0: goto copy_listSAgc_return_0;
  case 1: goto copy_listSAgc_return_1;
  case 2: goto copy_listSAgc_return_2;
  default: goto copy_listSAgc_return_3;
  }}

 shift_space: {
  x_422X = merged_arg0K0;
  n_423X = merged_arg0K1;{
  if ((0 == (3 & x_422X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17925;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_422X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17925;}}
 L17925: {
  x_size_2043X = arg0K0;
  extra_2044X = arg0K1;
  if ((n_423X < 0)) {
    if ((x_size_2043X < 1)) {
      arg0K0 = 1;
      goto L17970;}
    else {
      arg0K0 = x_size_2043X;
      goto L17970;}}
  else {
    n_2045X = n_423X / 30;
    arg0K0 = (5 + (x_size_2043X + n_2045X));
    goto L17945;}}
 L17970: {
  v_2046X = arg0K0;
  arg0K0 = (9 + ((((v_2046X)<<1)) + x_size_2043X));
  goto L17945;}
 L17945: {
  v_2047X = arg0K0;
  shift_space0_return_value = (extra_2044X + v_2047X);
  goto shift_space_return;}
 shift_space_return:
  switch (shift_space_return_tag) {
  case 0: goto shift_space_return_0;
  default: goto shift_space_return_1;
  }}

 pop_continuationB: {
{ SstackS = (ScontS);
  cont_2048X = ScontS;
  pointer_2049X = (((char *) (*((long *) cont_2048X)))) + -2;
  size_2050X = ((((*((unsigned char *) pointer_2049X)))<<8)) + (*((unsigned char *) (pointer_2049X + 1)));
  if ((65535 == size_2050X)) {
    arg0K0 = ((((*((long *) (cont_2048X + 4))))>>2));
    goto L17080;}
  else {
    arg0K0 = size_2050X;
    goto L17080;}}
 L17080: {
  v_2051X = arg0K0;
  ScontS = (cont_2048X + (4 + (((v_2051X)<<2))));
  v_2052X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  Scode_pointerS = (((char *) v_2052X));
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
  thing_419X = merged_arg0K0;
  index_420X = merged_arg0K1;
  count_421X = merged_arg0K2;{
  if ((index_420X < 0)) {
    okay_copy_argsP0_return_value = 0;
    goto okay_copy_argsP_return;}
  else {
    if ((3 == (3 & thing_419X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_419X))) + -4))))>>2))))) {
        if (((-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_419X))) + -4))))>>8))) < (index_420X + count_421X))) {
          okay_copy_argsP0_return_value = 0;
          goto okay_copy_argsP_return;}
        else {
          okay_copy_argsP0_return_value = 1;
          goto okay_copy_argsP_return;}}
      else {
        goto L16769;}}
    else {
      goto L16769;}}}
 L16769: {
  if ((3 == (3 & thing_419X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + thing_419X))) + -4))))>>2))))) {
      if ((((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_419X))) + -4))))>>8)) < (index_420X + count_421X))) {
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
  stob_416X = merged_arg0K0;
  index_417X = merged_arg0K1;
  value_418X = merged_arg0K2;{
  log_2053X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L15337;}
 L15337: {
  i_2054X = arg0K0;
  next_stob_2055X = *((long *) ((((char *) (-3 + log_2053X))) + (((i_2054X)<<2))));
  if ((1 == next_stob_2055X)) {add_log_entryAgc(1, i_2054X, stob_416X, index_417X, value_418X, 0);
    goto proposal_d_write_return;}
  else {
    if ((stob_416X == next_stob_2055X)) {
      if ((index_417X == (*((long *) ((((char *) (-3 + log_2053X))) + (4 + (((i_2054X)<<2)))))))) {
        addr_2056X = (((char *) (-3 + log_2053X))) + (8 + (((i_2054X)<<2)));S48_WRITE_BARRIER(log_2053X, addr_2056X, value_418X);
        *((long *) addr_2056X) = value_418X;
        goto proposal_d_write_return;}
      else {
        goto L15357;}}
    else {
      goto L15357;}}}
 L15357: {
  arg0K0 = (4 + i_2054X);
  goto L15337;}
 proposal_d_write_return:
  switch (proposal_d_write_return_tag) {
  case 0: goto proposal_d_write_return_0;
  case 1: goto proposal_d_write_return_1;
  default: goto proposal_d_write_return_2;
  }}

 proposal_d_read: {
  stob_414X = merged_arg0K0;
  index_415X = merged_arg0K1;{
  log_2057X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L15160;}
 L15160: {
  i_2058X = arg0K0;
  next_stob_2059X = *((long *) ((((char *) (-3 + log_2057X))) + (((i_2058X)<<2))));
  if ((1 == next_stob_2059X)) {
    v_2060X = add_log_entryAgc(1, i_2058X, stob_414X, index_415X, (*((long *) ((((char *) (-3 + stob_414X))) + (-4 & index_415X)))), 1);
    proposal_d_read0_return_value = v_2060X;
    goto proposal_d_read_return;}
  else {
    if ((stob_414X == next_stob_2059X)) {
      if ((index_415X == (*((long *) ((((char *) (-3 + log_2057X))) + (4 + (((i_2058X)<<2)))))))) {
        proposal_d_read0_return_value = (*((long *) ((((char *) (-3 + log_2057X))) + (8 + (((i_2058X)<<2))))));
        goto proposal_d_read_return;}
      else {
        goto L15182;}}
    else {
      goto L15182;}}}
 L15182: {
  arg0K0 = (4 + i_2058X);
  goto L15160;}
 proposal_d_read_return:
  switch (proposal_d_read_return_tag) {
  case 0: goto proposal_d_read_return_0;
  case 1: goto proposal_d_read_return_1;
  default: goto proposal_d_read_return_2;
  }}

 pending_interruptP: {
{ if ((s48_Spending_eventsPS)) {
    s48_Spending_eventsPS = 0;
    goto L15131;}
  else {
    goto L15117;}}
 L15131: {
  type_2061X = s48_get_next_event(&channel_2062X, &status_2063X);
  if ((type_2061X == ALARM_EVENT)) {
    arg0K0 = 1;
    goto L15137;}
  else {
    if ((type_2061X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L15137;}
    else {
      if ((type_2061X == IO_COMPLETION_EVENT)) {enqueue_channelB(channel_2062X, status_2063X);
        arg0K0 = 8;
        goto L15137;}
      else {
        if ((type_2061X == OS_SIGNAL_EVENT)) {
          arg0K0 = 16;
          goto L15137;}
        else {
          if ((type_2061X == NO_EVENT)) {
            arg0K0 = 0;
            goto L15137;}
          else {
            if ((type_2061X == ERROR_EVENT)) {
              ps_write_string("OS error while getting event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              ps_write_string((ps_error_string(status_2063X)), (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L15137;}
            else {
              ps_write_string("unknown type of event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L15137;}}}}}}}
 L15117: {
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
 L15137: {
  v_2064X = arg0K0;
  Spending_interruptsS = (v_2064X | (Spending_interruptsS));
  if ((type_2061X == NO_EVENT)) {
    goto L15117;}
  else {
    goto L15131;}}
 pending_interruptP_return:
  switch (pending_interruptP_return_tag) {
  case 0: goto pending_interruptP_return_0;
  case 1: goto pending_interruptP_return_1;
  case 2: goto pending_interruptP_return_2;
  default: goto pending_interruptP_return_3;
  }}

 make_closure: {
  a_412X = merged_arg0K0;
  b_413X = merged_arg0K1;{
  addr_2065X = s48_allocate_small(12);
  *((long *) addr_2065X) = 2062;
  x_2066X = 3 + (((long) (addr_2065X + 4)));
  *((long *) (((char *) (-3 + x_2066X)))) = a_412X;
  *((long *) ((((char *) (-3 + x_2066X))) + 4)) = b_413X;
  if ((3 == (3 & x_2066X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_2066X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_2066X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_2066X))) + -4))));
      make_closure0_return_value = x_2066X;
      goto make_closure_return;}
    else {
      make_closure0_return_value = x_2066X;
      goto make_closure_return;}}
  else {
    make_closure0_return_value = x_2066X;
    goto make_closure_return;}}
 make_closure_return:
  switch (make_closure_return_tag) {
  case 0: goto make_closure_return_0;
  default: goto make_closure_return_1;
  }}

 get_current_port: {
  marker_411X = merged_arg0K0;{
  thread_2067X = Scurrent_threadS;
  if ((3 == (3 & thread_2067X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thread_2067X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_2067X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_2067X))) + 4)));
        goto L13578;}
      else {
        goto L13628;}}
    else {
      goto L13628;}}
  else {
    goto L13628;}}
 L13578: {
  env_2068X = arg0K0;
  if ((3 == (3 & env_2068X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_2068X))) + -4))))>>2))))) {
      obj_2069X = *((long *) (((char *) (-3 + env_2068X))));
      if ((3 == (3 & obj_2069X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_2069X))) + -4))))>>2))))) {
          if ((marker_411X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_2068X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_2068X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_2068X))) + 4)));
            goto L13578;}}
        else {
          goto L13650;}}
      else {
        goto L13650;}}
    else {
      goto L13650;}}
  else {
    goto L13650;}}
 L13628: {
  ps_error("current thread is not a record", 0);
  get_current_port0_return_value = v_2070X;
  goto get_current_port_return;}
 L13650: {
  if ((25 == env_2068X)) {
    if (((((marker_411X)>>2)) == 1)) {
      arg3K0 = "dynamic environment doesn't have current-output-port";
      goto L13604;}
    else {
      arg3K0 = "dynamic environment doesn't have current-input-port";
      goto L13604;}}
  else {
    ps_error("dynamic environment is not a proper list", 0);
    get_current_port0_return_value = v_2071X;
    goto get_current_port_return;}}
 L13604: {
  v_2072X = arg3K0;
  ps_error(v_2072X, 0);
  get_current_port0_return_value = v_2073X;
  goto get_current_port_return;}
 get_current_port_return:
  switch (get_current_port_return_tag) {
  case 0: goto get_current_port_return_0;
  case 1: goto get_current_port_return_1;
  default: goto get_current_port_return_2;
  }}

 okay_argument_list: {
  list_410X = merged_arg0K0;{
  arg0K0 = list_410X;
  arg0K1 = 0;
  arg0K2 = list_410X;
  arg2K3 = 0;
  goto L13445;}
 L13445: {
  fast_2074X = arg0K0;
  len_2075X = arg0K1;
  slow_2076X = arg0K2;
  move_slowP_2077X = arg2K3;
  if ((25 == fast_2074X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_2075X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_2074X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_2074X))) + -4))))>>2))))) {
        if (move_slowP_2077X) {
          if ((fast_2074X == slow_2076X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_2074X))) + 4)));
            arg0K1 = (1 + len_2075X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_2076X))) + 4)));
            arg2K3 = 0;
            goto L13445;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_2074X))) + 4)));
          arg0K1 = (1 + len_2075X);
          arg0K2 = slow_2076X;
          arg2K3 = 1;
          goto L13445;}}
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
  cont_408X = merged_arg0K0;
  stack_arg_count_409X = merged_arg0K1;{
  stack_size_2078X = -2 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_408X))) + -4))))>>8))))>>2));
  new_cont_2079X = (Sbottom_of_stackS) + (0 - (((stack_size_2078X)<<2)));
  if ((0 == stack_arg_count_409X)) {
    SstackS = new_cont_2079X;
    goto L13290;}
  else {
    new_stack_2080X = new_cont_2079X + (0 - (((stack_arg_count_409X)<<2)));
    if ((new_stack_2080X < (SstackS))) {
      memcpy((void *)new_stack_2080X, (void *)(SstackS),(((stack_arg_count_409X)<<2)));
      SstackS = new_stack_2080X;
      goto L13290;}
    else {
      goto L13290;}}}
 L13290: {
  ScontS = new_cont_2079X;
  memcpy((void *)(new_cont_2079X + 4), (void *)((((char *) (-3 + cont_408X))) + 12),(-4 + (((stack_size_2078X)<<2))));
  *((long *) new_cont_2079X) = (((long) ((((char *) (-3 + (*((long *) ((((char *) (-3 + cont_408X))) + 4)))))) + ((((*((long *) (((char *) (-3 + cont_408X))))))>>2)))));
  Sheap_continuationS = (*((long *) ((((char *) (-3 + cont_408X))) + 8)));
  copy_continuation_from_heapB0_return_value = new_cont_2079X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  case 1: goto copy_continuation_from_heapB_return_1;
  default: goto copy_continuation_from_heapB_return_2;
  }}

 get_error_string: {
  status_407X = merged_arg0K0;{
  string_2081X = ps_error_string(status_407X);
  x_2082X = strlen((char *) string_2081X);
  if ((x_2082X < 256)) {
    arg0K0 = x_2082X;
    goto L12526;}
  else {
    arg0K0 = 256;
    goto L12526;}}
 L12526: {
  len_2083X = arg0K0;
  len_2084X = 1 + len_2083X;
  addr_2085X = s48_allocate_small((4 + len_2084X));
  *((long *) addr_2085X) = (70 + (((len_2084X)<<8)));
  string_2086X = 3 + (((long) (addr_2085X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_2086X))) + len_2083X)) = 0;
  arg0K0 = 0;
  goto L12536;}
 L12536: {
  i_2087X = arg0K0;
  if ((i_2087X == len_2083X)) {
    get_error_string0_return_value = string_2086X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_2086X))) + i_2087X)) = ((*(string_2081X + i_2087X)));
    arg0K0 = (1 + i_2087X);
    goto L12536;}}
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
  nargs_406X = merged_arg0K0;{
  top_of_args_2088X = SstackS;
  if (((ScontS) == (top_of_args_2088X + (((nargs_406X)<<2))))) {
    goto move_args_above_contB_return;}
  else {
    SstackS = (ScontS);
    arg1K0 = ((SstackS) + -4);
    arg1K1 = (top_of_args_2088X + (-4 + (((nargs_406X)<<2))));
    goto L5463;}}
 L5463: {
  loc_2089X = arg1K0;
  arg_2090X = arg1K1;
  if ((arg_2090X < top_of_args_2088X)) {
    SstackS = ((SstackS) + (0 - (((nargs_406X)<<2))));
    goto move_args_above_contB_return;}
  else {
    *((long *) loc_2089X) = (*((long *) arg_2090X));
    arg1K0 = (loc_2089X + -4);
    arg1K1 = (arg_2090X + -4);
    goto L5463;}}
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
long s48_call_startup_procedure(char **startup_vector_2091X, long startup_vector_length_2092X)
{
  long arg0K0;
  long merged_arg0K1;
  char *merged_arg3K0;

  int enter_string_return_tag;
  long enter_string0_return_value;
  char *string_2093X;
  long i_2133X;
  long string_2132X;
  char * addr_2131X;
  long len_2130X;
  long len_2129X;
  long v_2128X;
  long x_2127X;
  long x_2126X;
  char * addr_2125X;
  long b_2124X;
  long channel_2123X;
  long x_2122X;
  char * addr_2121X;
  long b_2120X;
  long channel_2119X;
  long x_2118X;
  char * addr_2117X;
  long b_2116X;
  long channel_2115X;
  long i_2114X;
  long length_2113X;
  long *v_2112X;
  long v_2111X;
  long v_2110X;
  long y_2109X;
  long x_2108X;
  long v_2107X;
  long x_2106X;
  long y_2105X;
  char * addr_2104X;
  long value_2103X;
  long x_2102X;
  long y_2101X;
  long i_2100X;
  long vector_2099X;
  long v_2098X;
  long vector_2097X;
  char * addr_2096X;
  long len_2095X;
  long code_2094X;
 {  SstackS = (Sbottom_of_stackS);
  Sheap_continuationS = 1;
  ScontS = (Sbottom_of_stackS);
  code_2094X = Sinterrupt_return_codeS;
  Slast_code_calledS = code_2094X;
  Scode_pointerS = (((char *) (-3 + code_2094X)));
  Slast_code_pointer_resumedS = (Scode_pointerS);
  SvalS = 13;
  Scurrent_threadS = 25;SHARED_SETB((Ssession_dataS), 25);SHARED_SETB((Sexception_handlersS), 25);SHARED_SETB((Sinterrupt_handlersS), 25);
  Senabled_interruptsS = 0;SHARED_SETB((Sfinalizer_alistS), 25);
  Sfinalize_theseS = 25;
  Spending_interruptsS = 0;
  s48_Spending_interruptPS = 0;
  Sinterrupted_templateS = 1;
  len_2095X = ((startup_vector_length_2092X)<<2);
  addr_2096X = s48_allocate_tracedAgc((4 + len_2095X));
  if ((addr_2096X == NULL)) {
    arg0K0 = 1;
    goto L17839;}
  else {
    *((long *) addr_2096X) = (10 + (((len_2095X)<<8)));
    arg0K0 = (3 + (((long) (addr_2096X + 4))));
    goto L17839;}}
 L17839: {
  vector_2097X = arg0K0;
  if ((1 == vector_2097X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_2098X;
    goto L17825;}
  else {
    arg0K0 = vector_2097X;
    goto L17825;}}
 L17825: {
  vector_2099X = arg0K0;
  arg0K0 = 0;
  goto L17862;}
 L17862: {
  i_2100X = arg0K0;
  if ((i_2100X == startup_vector_length_2092X)) {
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = vector_2099X;
    y_2101X = fileno((stderr));
    x_2102X = fileno((stdout));
    if ((x_2102X < y_2101X)) {
      arg0K0 = y_2101X;
      goto L19183;}
    else {
      arg0K0 = x_2102X;
      goto L19183;}}
  else {
    value_2103X = enter_stringAgc((*(startup_vector_2091X + i_2100X)));
    addr_2104X = (((char *) (-3 + vector_2099X))) + (((i_2100X)<<2));S48_WRITE_BARRIER(vector_2099X, addr_2104X, value_2103X);
    *((long *) addr_2104X) = value_2103X;
    arg0K0 = (1 + i_2100X);
    goto L17862;}}
 L19183: {
  y_2105X = arg0K0;
  x_2106X = fileno((stdin));
  if ((x_2106X < y_2105X)) {
    arg0K0 = y_2105X;
    goto L19185;}
  else {
    arg0K0 = x_2106X;
    goto L19185;}}
 L19185: {
  v_2107X = arg0K0;
  x_2108X = Snumber_of_channelsS;
  y_2109X = 1 + v_2107X;
  if ((x_2108X < y_2109X)) {
    arg0K0 = y_2109X;
    goto L19187;}
  else {
    arg0K0 = x_2108X;
    goto L19187;}}
 L19187: {
  v_2110X = arg0K0;
  Snumber_of_channelsS = v_2110X;
  v_2111X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L19209;}
  else {
    goto L19209;}}
 L19209: {
  v_2112X = Svm_channelsS;
  length_2113X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L19257;}
 L19257: {
  i_2114X = arg0K0;
  if ((i_2114X < length_2113X)) {
    *(v_2112X + i_2114X) = 1;
    arg0K0 = (1 + i_2114X);
    goto L19257;}
  else {s48_make_availableAgc(((((3 * (7 + ((((4 + (strlen((char *) "standard output"))))>>2)))))<<2)));
    channel_2115X = fileno((stdin));
    merged_arg3K0 = "standard input";
    merged_arg0K1 = 0;
    enter_string_return_tag = 0;
    goto enter_string;
   enter_string_return_0:
    b_2116X = enter_string0_return_value;
    addr_2117X = s48_allocate_small(24);
    *((long *) addr_2117X) = 5146;
    x_2118X = 3 + (((long) (addr_2117X + 4)));
    *((long *) (((char *) (-3 + x_2118X)))) = 4;
    *((long *) ((((char *) (-3 + x_2118X))) + 4)) = b_2116X;
    *((long *) ((((char *) (-3 + x_2118X))) + 8)) = (((channel_2115X)<<2));
    *((long *) ((((char *) (-3 + x_2118X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_2118X))) + 16)) = 1;
    *((Svm_channelsS) + channel_2115X) = x_2118X;
    channel_2119X = fileno((stderr));
    merged_arg3K0 = "standard error";
    merged_arg0K1 = 0;
    enter_string_return_tag = 1;
    goto enter_string;
   enter_string_return_1:
    b_2120X = enter_string0_return_value;
    addr_2121X = s48_allocate_small(24);
    *((long *) addr_2121X) = 5146;
    x_2122X = 3 + (((long) (addr_2121X + 4)));
    *((long *) (((char *) (-3 + x_2122X)))) = 8;
    *((long *) ((((char *) (-3 + x_2122X))) + 4)) = b_2120X;
    *((long *) ((((char *) (-3 + x_2122X))) + 8)) = (((channel_2119X)<<2));
    *((long *) ((((char *) (-3 + x_2122X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_2122X))) + 16)) = 1;
    *((Svm_channelsS) + channel_2119X) = x_2122X;
    channel_2123X = fileno((stdout));
    merged_arg3K0 = "standard output";
    merged_arg0K1 = 0;
    enter_string_return_tag = 2;
    goto enter_string;
   enter_string_return_2:
    b_2124X = enter_string0_return_value;
    addr_2125X = s48_allocate_small(24);
    *((long *) addr_2125X) = 5146;
    x_2126X = 3 + (((long) (addr_2125X + 4)));
    *((long *) (((char *) (-3 + x_2126X)))) = 8;
    *((long *) ((((char *) (-3 + x_2126X))) + 4)) = b_2124X;
    *((long *) ((((char *) (-3 + x_2126X))) + 8)) = (((channel_2123X)<<2));
    *((long *) ((((char *) (-3 + x_2126X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_2126X))) + 16)) = 1;
    *((Svm_channelsS) + channel_2123X) = x_2126X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2118X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2126X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2122X;
    x_2127X = s48_resumer_records();
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_2127X;s48_initialization_completeB();
    v_2128X = s48_startup_procedure();
    return s48_restart(v_2128X, 5);}}
 enter_string: {
  string_2093X = merged_arg3K0;{
  len_2129X = strlen((char *) string_2093X);
  len_2130X = 1 + len_2129X;
  addr_2131X = s48_allocate_small((4 + len_2130X));
  *((long *) addr_2131X) = (70 + (((len_2130X)<<8)));
  string_2132X = 3 + (((long) (addr_2131X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_2132X))) + len_2129X)) = 0;
  arg0K0 = 0;
  goto L15006;}
 L15006: {
  i_2133X = arg0K0;
  if ((i_2133X == len_2129X)) {
    enter_string0_return_value = string_2132X;
    goto enter_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_2132X))) + i_2133X)) = ((*(string_2093X + i_2133X)));
    arg0K0 = (1 + i_2133X);
    goto L15006;}}
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
