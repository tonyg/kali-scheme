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
static long Hlookup2550(long, long, long);
static long Hlookup2531(long, long, long);
static void HtopD9982(void);
static void HtopD9993(void);
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
long s48_add_channel(long, long, long);
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
    goto L2829;}
  else {
    arg0K0 = x_5X;
    goto L2829;}}
 L2829: {
  new_count_7X = arg0K0;
  new_vm_channels_8X = (long*)malloc(sizeof(long) * new_count_7X);
  if ((NULL == new_vm_channels_8X)) {
    return 0;}
  else {
    arg0K0 = 0;
    goto L2841;}}
 L2841: {
  i_9X = arg0K0;
  if ((i_9X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L2914;}
  else {
    *(new_vm_channels_8X + i_9X) = (*((Svm_channelsS) + i_9X));
    arg0K0 = (1 + i_9X);
    goto L2841;}}
 L2914: {
  i_10X = arg0K0;
  if ((i_10X == new_count_7X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_8X;
    Snumber_of_channelsS = new_count_7X;
    return 1;}
  else {
    *(new_vm_channels_8X + i_10X) = 1;
    arg0K0 = (1 + i_10X);
    goto L2914;}}
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
    goto L10378;}
  else {
    goto L10378;}}
 L10378: {
  v_22X = *((long *) (((char *) (-3 + channel_18X))));
  if ((4 == v_22X)) {
    goto L10393;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_18X))))))) {
      goto L10393;}
    else {
      v_23X = ps_close_fd(os_index_19X);
      arg0K0 = v_23X;
      goto L10400;}}}
 L10393: {
  v_24X = ps_close_fd(os_index_19X);
  arg0K0 = v_24X;
  goto L10400;}
 L10400: {
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
  goto L11872;}
 L11872: {
  p_36X = arg1K0;
  i_37X = arg0K1;
  if ((2 == (3 & (*((long *) p_36X))))) {
    if ((26 == (*((long *) p_36X)))) {
      arg0K0 = (-1 + i_37X);
      goto L11813;}
    else {
      arg0K0 = i_37X;
      goto L11813;}}
  else {
    arg1K0 = (p_36X + 4);
    arg0K1 = (1 + i_37X);
    goto L11872;}}
 L11813: {
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
      goto L9634;}
    else {
      arg0K0 = (-1 + (((log_size_46X)<<1)));
      goto L9634;}}
  else {
    arg0K0 = proposal_45X;
    arg0K1 = stob_41X;
    arg0K2 = value_43X;
    goto L11982;}}
 L9634: {
  new_size_47X = arg0K0;
  len_48X = ((new_size_47X)<<2);
  addr_49X = s48_allocate_tracedAgc((4 + len_48X));
  if ((addr_49X == NULL)) {
    arg0K0 = 1;
    goto L9680;}
  else {
    *((long *) addr_49X) = (10 + (((len_48X)<<8)));
    arg0K0 = (3 + (((long) (addr_49X + 4))));
    goto L9680;}}
 L11982: {
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
    goto L12003;}
  else {
    arg0K0 = 29;
    goto L12003;}}
 L9680: {
  vector_57X = arg0K0;
  if ((1 == vector_57X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_58X;
    goto L9638;}
  else {
    arg0K0 = vector_57X;
    goto L9638;}}
 L12003: {
  value_59X = arg0K0;
  addr_60X = (((char *) (-3 + log_53X))) + (12 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_60X, value_59X);
  *((long *) addr_60X) = value_59X;
  addr_61X = (((char *) (-3 + log_53X))) + (16 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_61X, 1);
  *((long *) addr_61X) = 1;
  return value_52X;}
 L9638: {
  new_62X = arg0K0;
  proposal_63X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 < log_size_46X)) {
    stob_64X = *((long *) ((((char *) (-3 + proposal_63X))) + 4));
    memcpy((void *)(((char *) (-3 + new_62X))), (void *)(((char *) (-3 + stob_64X))),(-4 + (((log_size_46X)<<2))));
    goto L9658;}
  else {
    goto L9658;}}
 L9658: {
  arg0K0 = (4 + log_size_46X);
  goto L9662;}
 L9662: {
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
    goto L11982;}
  else {
    addr_70X = (((char *) (-3 + new_62X))) + (((i_65X)<<2));
    S48_WRITE_BARRIER(new_62X, addr_70X, 0);
    *((long *) addr_70X) = 0;
    arg0K0 = (1 + i_65X);
    goto L9662;}}
}
static long enter_bignum(char * external_bignum_71X)
{
  long desc_72X;
 {  desc_72X = 3 + (((long) external_bignum_71X));
  if ((3 == (3 & desc_72X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + desc_72X))) + -4))))>>2))))) {
      goto L12616;}
    else {
      goto L12630;}}
  else {
    goto L12630;}}
 L12616: {
  if ((3 == (3 & desc_72X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + desc_72X))) + -4)))))) {
      *((long *) ((((char *) (-3 + desc_72X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + desc_72X))) + -4))));
      return desc_72X;}
    else {
      return desc_72X;}}
  else {
    return desc_72X;}}
 L12630: {
  ps_error("not a bignum", 1, desc_72X);
  goto L12616;}
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
  goto L13552;}
 L13552: {
  i_78X = arg0K0;
  if ((i_78X == z_74X)) {
    return string_77X;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_77X))) + i_78X)) = ((*(string_73X + i_78X)));
    arg0K0 = (1 + i_78X);
    goto L13552;}}
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
      goto L14480;}
    else {
      goto L14474;}}
  else {
    goto L14474;}}
 L14480: {
  PS_WRITE_CHAR(10, (stderr), v_82X)
  return v_82X;}
 L14474: {
  ps_write_integer((((index_80X)>>2)), (stderr));
  goto L14480;}
}
static long make_registered_channel(long mode_83X, long id_84X, long os_index_85X, long key_86X, long *TT0)
{
  long x_90X;
  char * addr_89X;
  char x_88X;
  char temp_87X;
 {  temp_87X = os_index_85X < (Snumber_of_channelsS);
  if (temp_87X) {
    goto L15532;}
  else {
    x_88X = add_more_channels(os_index_85X);
    if (x_88X) {
      goto L15532;}
    else {
      *TT0 = 9;
      return 1;}}}
 L15532: {
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
static long Hlookup2550(long table_91X, long string_92X, long key_93X)
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
  goto L19399;}
 L19399: {
  i_95X = arg0K0;
  h_96X = arg0K1;
  if ((i_95X < n_94X)) {
    arg0K0 = (1 + i_95X);
    arg0K1 = (h_96X + (((*((unsigned char *) ((((char *) (-3 + string_92X))) + i_95X))))));
    goto L19399;}
  else {
    index_97X = 1023 & h_96X;
    link_98X = *((long *) ((((char *) (-3 + table_91X))) + (((index_97X)<<2))));
    if ((0 == (3 & link_98X))) {
      arg0K0 = (3 + (-4 & link_98X));
      goto L19355;}
    else {
      arg0K0 = link_98X;
      goto L19355;}}}
 L19355: {
  bucket_99X = arg0K0;
  arg0K0 = bucket_99X;
  goto L19361;}
 L19361: {
  foo_100X = arg0K0;
  if ((1 == foo_100X)) {
    if ((3 == (3 & bucket_99X))) {
      arg0K0 = (-4 & bucket_99X);
      goto L19366;}
    else {
      arg0K0 = bucket_99X;
      goto L19366;}}
  else {
    s2_101X = *((long *) (((char *) (-3 + foo_100X))));
    len_102X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_92X))) + -4))))>>8);
    if ((len_102X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_101X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_101X))), (void *)(((char *) (-3 + string_92X))),len_102X)))) {
        return foo_100X;}
      else {
        goto L19381;}}
    else {
      goto L19381;}}}
 L19366: {
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
    goto L19372;}
  else {
    arg0K0 = x_105X;
    goto L19372;}}
 L19381: {
  link_106X = *((long *) ((((char *) (-3 + foo_100X))) + 12));
  if ((0 == (3 & link_106X))) {
    arg0K0 = (3 + (-4 & link_106X));
    goto L19361;}
  else {
    arg0K0 = link_106X;
    goto L19361;}}
 L19372: {
  value_107X = arg0K0;
  addr_108X = (((char *) (-3 + table_91X))) + (((index_97X)<<2));
  S48_WRITE_BARRIER(table_91X, addr_108X, value_107X);
  *((long *) addr_108X) = value_107X;
  return x_105X;}
}
static long Hlookup2531(long table_109X, long string_110X, long key_111X)
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
  goto L19551;}
 L19551: {
  i_113X = arg0K0;
  h_114X = arg0K1;
  if ((i_113X < n_112X)) {
    arg0K0 = (1 + i_113X);
    arg0K1 = (h_114X + (((*((unsigned char *) ((((char *) (-3 + string_110X))) + i_113X))))));
    goto L19551;}
  else {
    index_115X = 1023 & h_114X;
    link_116X = *((long *) ((((char *) (-3 + table_109X))) + (((index_115X)<<2))));
    if ((0 == (3 & link_116X))) {
      arg0K0 = (3 + (-4 & link_116X));
      goto L19507;}
    else {
      arg0K0 = link_116X;
      goto L19507;}}}
 L19507: {
  bucket_117X = arg0K0;
  arg0K0 = bucket_117X;
  goto L19513;}
 L19513: {
  foo_118X = arg0K0;
  if ((1 == foo_118X)) {
    if ((3 == (3 & bucket_117X))) {
      arg0K0 = (-4 & bucket_117X);
      goto L19518;}
    else {
      arg0K0 = bucket_117X;
      goto L19518;}}
  else {
    s2_119X = *((long *) (((char *) (-3 + foo_118X))));
    len_120X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_110X))) + -4))))>>8);
    if ((len_120X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_119X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_119X))), (void *)(((char *) (-3 + string_110X))),len_120X)))) {
        return foo_118X;}
      else {
        goto L19533;}}
    else {
      goto L19533;}}}
 L19518: {
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
    goto L19524;}
  else {
    arg0K0 = x_123X;
    goto L19524;}}
 L19533: {
  link_124X = *((long *) ((((char *) (-3 + foo_118X))) + 12));
  if ((0 == (3 & link_124X))) {
    arg0K0 = (3 + (-4 & link_124X));
    goto L19513;}
  else {
    arg0K0 = link_124X;
    goto L19513;}}
 L19524: {
  value_125X = arg0K0;
  addr_126X = (((char *) (-3 + table_109X))) + (((index_115X)<<2));
  S48_WRITE_BARRIER(table_109X, addr_126X, value_125X);
  *((long *) addr_126X) = value_125X;
  return x_123X;}
}
static void HtopD9982(void)
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
  goto L20184;}
 L20184: {
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
    goto L9992;}
  else {
    foo_link_132X = *((long *) ((((char *) (-3 + table_127X))) + (((i_128X)<<2))));
    arg0K0 = foo_link_132X;
    arg0K1 = 1;
    goto L13074;}}
 L9992: {
  alist_133X = arg0K0;
  okay_134X = arg0K1;
  goners_135X = arg0K2;
  if ((25 == alist_133X)) {
    SHARED_SETB((Sfinalizer_alistS), okay_134X);
    l2_136X = Sfinalize_theseS;
    if ((25 == goners_135X)) {
      arg0K0 = l2_136X;
      goto L9999;}
    else {
      arg0K0 = goners_135X;
      goto L10054;}}
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
      goto L9992;}
    else {
      addr_146X = (((char *) (-3 + alist_137X))) + 4;
      S48_WRITE_BARRIER(alist_137X, addr_146X, goners_135X);
      *((long *) addr_146X) = goners_135X;
      arg0K0 = next_140X;
      arg0K1 = okay_134X;
      arg0K2 = alist_137X;
      goto L9992;}}}
 L13074: {
  foo_link_147X = arg0K0;
  okay_link_148X = arg0K1;
  if ((0 == (3 & foo_link_147X))) {
    arg0K0 = (3 + (-4 & foo_link_147X));
    goto L13076;}
  else {
    arg0K0 = foo_link_147X;
    goto L13076;}}
 L9999: {
  v_149X = arg0K0;
  Sfinalize_theseS = v_149X;
  arg0K0 = 0;
  goto L18774;}
 L10054: {
  l_150X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_150X))) + 4))))) {
    addr_151X = (((char *) (-3 + l_150X))) + 4;
    S48_WRITE_BARRIER(l_150X, addr_151X, l2_136X);
    *((long *) addr_151X) = l2_136X;
    arg0K0 = goners_135X;
    goto L9999;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_150X))) + 4)));
    goto L10054;}}
 L13076: {
  foo_152X = arg0K0;
  if ((1 == foo_152X)) {
    addr_153X = (((char *) (-3 + table_127X))) + (((i_128X)<<2));
    S48_WRITE_BARRIER(table_127X, addr_153X, okay_link_148X);
    *((long *) addr_153X) = okay_link_148X;
    arg0K0 = (1 + i_128X);
    goto L20184;}
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
        goto L13074;}
      else {
        arg0K0 = next_link_156X;
        arg0K1 = new_foo_155X;
        goto L13074;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + foo_152X))) + 4)));
      arg0K1 = okay_link_148X;
      goto L13074;}}}
 L18774: {
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
      goto L18808;}
    else {
      v_160X = s48_extantP(channel_159X);
      if (v_160X) {
        v_161X = s48_trace_value(channel_159X);
        arg0K0 = v_161X;
        goto L18801;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_159X))))))) {
          arg0K0 = 1;
          goto L18801;}
        else {
          id_162X = *((long *) ((((char *) (-3 + channel_159X))) + 4));
          status_163X = close_channelB(channel_159X);
          if ((status_163X == NO_ERRORS)) {
            goto L16800;}
          else {
            channel_close_error(status_163X, (*((long *) ((((char *) (-3 + channel_159X))) + 8))), id_162X);
            goto L16800;}}}}}}
 L18808: {
  arg0K0 = (1 + i_158X);
  goto L18774;}
 L18801: {
  new_164X = arg0K0;
  *((Svm_channelsS) + i_158X) = new_164X;
  goto L18808;}
 L16800: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_162X))) {
    ps_write_integer((((id_162X)>>2)), (stderr));
    goto L16818;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_162X))))), (stderr));
    goto L16818;}}
 L16818: {
  ps_write_string(" ", (stderr));
  ps_write_integer(((((*((long *) ((((char *) (-3 + channel_159X))) + 8))))>>2)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L18801;}
}
static void HtopD9993(void)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long merged_arg0K0;

  int Hentry_tracer830165_return_tag;
  long Hentry_tracer8301650_return_value;
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
  goto L6677;}
 L6677: {
  frame_168X = arg1K0;
  if ((frame_168X == NULL)) {
    arg1K0 = (Spermanent_external_rootsS);
    goto L6703;}
  else {
    length_169X = *((long *) frame_168X);
    arg0K0 = 0;
    goto L6685;}}
 L6703: {
  frame_170X = arg1K0;
  if ((frame_170X == NULL)) {
    s48_initializing_gc_root();
    table_171X = s48_trace_value((Simported_bindingsS));
    arg0K0 = 0;
    goto L17083;}
  else {
    cell_172X = ((char *) (*((long *) (frame_170X + 4))));
    x2_173X = s48_trace_value((*((long *) cell_172X)));
    *((long *) cell_172X) = x2_173X;
    arg1K0 = (((char *) (*((long *) frame_170X))));
    goto L6703;}}
 L6685: {
  i_174X = arg0K0;
  if ((i_174X == length_169X)) {
    arg1K0 = (((char *) (*((long *) (frame_168X + 4)))));
    goto L6677;}
  else {
    cell_175X = ((char *) (*((long *) (frame_168X + (8 + (((i_174X)<<2)))))));
    x2_176X = s48_trace_value((*((long *) cell_175X)));
    *((long *) cell_175X) = x2_176X;
    arg0K0 = (1 + i_174X);
    goto L6685;}}
 L17083: {
  i_177X = arg0K0;
  if ((1024 == i_177X)) {
    Simported_bindingsS = table_171X;
    table_178X = s48_trace_value((Sexported_bindingsS));
    arg0K0 = 0;
    goto L17104;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_171X))) + (((i_177X)<<2)))));
    Hentry_tracer830165_return_tag = 0;
    goto Hentry_tracer830165;
   Hentry_tracer830165_return_0:
    value_179X = Hentry_tracer8301650_return_value;
    addr_180X = (((char *) (-3 + table_171X))) + (((i_177X)<<2));
    S48_WRITE_BARRIER(table_171X, addr_180X, value_179X);
    *((long *) addr_180X) = value_179X;
    arg0K0 = (1 + i_177X);
    goto L17083;}}
 L17104: {
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
    goto L6741;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_178X))) + (((i_181X)<<2)))));
    Hentry_tracer830165_return_tag = 1;
    goto Hentry_tracer830165;
   Hentry_tracer830165_return_1:
    value_199X = Hentry_tracer8301650_return_value;
    addr_200X = (((char *) (-3 + table_178X))) + (((i_181X)<<2));
    S48_WRITE_BARRIER(table_178X, addr_200X, value_199X);
    *((long *) addr_200X) = value_199X;
    arg0K0 = (1 + i_181X);
    goto L17104;}}
 L6741: {
  alist_201X = arg0K0;
  if ((25 == alist_201X)) {
    env_202X = s48_trace_value((SenvS));
    SenvS = env_202X;
    v_203X = s48_trace_value((Spending_channels_headS));
    Spending_channels_headS = v_203X;
    v_204X = s48_trace_value((Spending_channels_tailS));
    Spending_channels_tailS = v_204X;
    arg0K0 = 0;
    goto L10124;}
  else {
    pair_205X = *((long *) (((char *) (-3 + alist_201X))));
    x_206X = s48_extantP((*((long *) (((char *) (-3 + pair_205X))))));
    if (x_206X) {
      goto L6766;}
    else {
      s48_trace_stob_contentsB((*((long *) (((char *) (-3 + pair_205X))))));
      goto L6766;}}}
 L10124: {
  i_207X = arg0K0;
  if ((i_207X == (Snumber_of_channelsS))) {
    if ((Sstack_warningPS)) {
      arg1K0 = (Sstack_beginS);
      goto L7289;}
    else {
      goto L7322;}}
  else {
    channel_208X = *((Svm_channelsS) + i_207X);
    if ((1 == channel_208X)) {
      goto L10152;}
    else {
      if ((0 == (*((long *) (((char *) (-3 + channel_208X))))))) {
        goto L10152;}
      else {
        val_209X = s48_trace_value((*((long *) ((((char *) (-3 + channel_208X))) + 4))));
        addr_210X = (((char *) (-3 + channel_208X))) + 4;
        S48_WRITE_BARRIER(channel_208X, addr_210X, val_209X);
        *((long *) addr_210X) = val_209X;
        goto L10152;}}}}
 L6766: {
  val_211X = s48_trace_value((*((long *) ((((char *) (-3 + pair_205X))) + 4))));
  addr_212X = (((char *) (-3 + pair_205X))) + 4;
  S48_WRITE_BARRIER(pair_205X, addr_212X, val_211X);
  *((long *) addr_212X) = val_211X;
  arg0K0 = (*((long *) ((((char *) (-3 + alist_201X))) + 4)));
  goto L6741;}
 L7289: {
  a_213X = arg1K0;
  if ((252645135 == (*((long *) a_213X)))) {
    arg1K0 = (a_213X + 4);
    goto L7289;}
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
      goto L7322;}
    else {
      goto L7322;}}}
 L7322: {
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L7387;}
 L10152: {
  arg0K0 = (1 + i_207X);
  goto L10124;}
 L7387: {
  p_215X = arg1K0;
  i_216X = arg0K1;
  if ((2 == (3 & (*((long *) p_215X))))) {
    if ((26 == (*((long *) p_215X)))) {
      arg0K0 = (-1 + i_216X);
      goto L7324;}
    else {
      arg0K0 = i_216X;
      goto L7324;}}
  else {
    arg1K0 = (p_215X + 4);
    arg0K1 = (1 + i_216X);
    goto L7387;}}
 L7324: {
  arg_count_217X = arg0K0;
  s48_trace_locationsB(((SstackS) + 4), ((SstackS) + (4 + (((arg_count_217X)<<2)))));
  p_218X = SenvS;
  if ((3 == (3 & p_218X))) {
    if ((p_218X < (((long) (Sstack_beginS))))) {
      goto L7340;}
    else {
      if (((((long) (Sstack_endS))) < p_218X)) {
        goto L7340;}
      else {
        env_219X = SenvS;
        arg0K0 = env_219X;
        goto L7500;}}}
  else {
    goto L7340;}}
 L7340: {
  v_220X = s48_trace_value((SenvS));
  SenvS = v_220X;
  goto L7345;}
 L7500: {
  env_221X = arg0K0;
  if ((3 == (3 & env_221X))) {
    if ((env_221X < (((long) (Sstack_beginS))))) {
      goto L7345;}
    else {
      if (((((long) (Sstack_endS))) < env_221X)) {
        goto L7345;}
      else {
        s48_trace_stob_contentsB(env_221X);
        arg0K0 = (*((long *) (((char *) (-3 + env_221X)))));
        goto L7500;}}}
  else {
    goto L7345;}}
 L7345: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L7350;}
 L7350: {
  cont_222X = arg0K0;
  last_env_223X = arg0K1;
  env_224X = *((long *) ((((char *) (-3 + cont_222X))) + 12));
  s48_trace_stob_contentsB(cont_222X);
  if ((env_224X == last_env_223X)) {
    goto L7368;}
  else {
    arg0K0 = env_224X;
    goto L7455;}}
 L7368: {
  if ((cont_222X == (Sbottom_of_stackS))) {
    v_225X = s48_trace_value((Stemp0S));
    Stemp0S = v_225X;
    v_226X = s48_trace_value((Stemp1S));
    Stemp1S = v_226X;
    return;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + cont_222X)))));
    arg0K1 = env_224X;
    goto L7350;}}
 L7455: {
  env_227X = arg0K0;
  if ((3 == (3 & env_227X))) {
    if ((env_227X < (((long) (Sstack_beginS))))) {
      goto L7368;}
    else {
      if (((((long) (Sstack_endS))) < env_227X)) {
        goto L7368;}
      else {
        s48_trace_stob_contentsB(env_227X);
        arg0K0 = (*((long *) (((char *) (-3 + env_227X)))));
        goto L7455;}}}
  else {
    goto L7368;}}
 Hentry_tracer830165: {
  foo_link_166X = merged_arg0K0;{
  arg0K0 = foo_link_166X;
  arg0K1 = 1;
  goto L13146;}
 L13146: {
  foo_link_228X = arg0K0;
  done_link_229X = arg0K1;
  if ((0 == (3 & foo_link_228X))) {
    arg0K0 = (3 + (-4 & foo_link_228X));
    goto L13148;}
  else {
    arg0K0 = foo_link_228X;
    goto L13148;}}
 L13148: {
  foo_230X = arg0K0;
  if ((1 == foo_230X)) {
    Hentry_tracer8301650_return_value = done_link_229X;
    goto Hentry_tracer830165_return;}
  else {
    new_foo_231X = s48_trace_value(foo_230X);
    next_link_232X = *((long *) ((((char *) (-3 + new_foo_231X))) + 12));
    addr_233X = (((char *) (-3 + new_foo_231X))) + 12;
    S48_WRITE_BARRIER(new_foo_231X, addr_233X, done_link_229X);
    *((long *) addr_233X) = done_link_229X;
    if ((3 == (3 & new_foo_231X))) {
      arg0K0 = next_link_232X;
      arg0K1 = (-4 & new_foo_231X);
      goto L13146;}
    else {
      arg0K0 = next_link_232X;
      arg0K1 = new_foo_231X;
      goto L13146;}}}
 Hentry_tracer830165_return:
  switch (Hentry_tracer830165_return_tag) {
  case 0: goto Hentry_tracer830165_return_0;
  default: goto Hentry_tracer830165_return_1;
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
    goto L3741;}
  else {
    goto L3741;}}
 L3741: {
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
  long old_size_256X;
  long new_size_255X;
  long bignum_254X;
 {  bignum_254X = 3 + (((long) external_bignum_252X));
  new_size_255X = 8 + (((number_of_digits_253X)<<2));
  old_size_256X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + bignum_254X))) + -4))))>>8);
  if ((old_size_256X < new_size_255X)) {
    ps_error("shorten bignum", 2, new_size_255X, old_size_256X);
    goto L6273;}
  else {
    goto L6273;}}
 L6273: {
  if ((new_size_255X < old_size_256X)) {
    *((long *) ((((char *) (-3 + bignum_254X))) + -4)) = (82 + (((new_size_255X)<<8)));
    *((long *) ((((char *) (((long) ((((char *) (-3 + bignum_254X))) + (1 + (-4 & (3 + new_size_255X)))))))) + -4)) = (82 + ((((old_size_256X - new_size_255X))<<8)));
    return external_bignum_252X;}
  else {
    return external_bignum_252X;}}
}
long s48_allocate_bignum(long size_257X)
{
  char * addr_258X;
 {  addr_258X = s48_allocate_small((4 + size_257X));
  *((long *) addr_258X) = (82 + (((size_257X)<<8)));
  return (3 + (((long) (addr_258X + 4))));}
}
long s48_set_channel_os_index(long channel_259X, long os_index_260X)
{
  char * addr_267X;
  long val_266X;
  long v_265X;
  long x_264X;
  long old_index_263X;
  char x_262X;
  char temp_261X;
 {  temp_261X = os_index_260X < (Snumber_of_channelsS);
  if (temp_261X) {
    goto L10283;}
  else {
    x_262X = add_more_channels(os_index_260X);
    if (x_262X) {
      goto L10283;}
    else {
      return 36;}}}
 L10283: {
  if ((1 == (*((Svm_channelsS) + os_index_260X)))) {
    old_index_263X = (((*((long *) ((((char *) (-3 + channel_259X))) + 8))))>>2);
    x_264X = *((long *) ((((char *) (-3 + channel_259X))) + 16));
    if ((5 == x_264X)) {
      v_265X = ps_abort_fd_op(old_index_263X);
      enqueue_channelB(old_index_263X, v_265X);
      goto L10269;}
    else {
      goto L10269;}}
  else {
    return 44;}}
 L10269: {
  *((Svm_channelsS) + old_index_263X) = 1;
  *((Svm_channelsS) + os_index_260X) = channel_259X;
  val_266X = ((os_index_260X)<<2);
  addr_267X = (((char *) (-3 + channel_259X))) + 8;
  S48_WRITE_BARRIER(channel_259X, addr_267X, val_266X);
  *((long *) addr_267X) = val_266X;
  return 5;}
}
void s48_setup_external_exception(long why_268X, long nargs_269X)
{

 {  push_exception_continuationB(why_268X, 1);
  if ((10 < nargs_269X)) {
    ps_error("too many arguments from external exception", 0);
    goto L13585;}
  else {
    goto L13585;}}
 L13585: {
  Sexternal_exception_nargsS = nargs_269X;
  Sexternal_exceptionPS = 1;
  return;}
}
void s48_close_channel(long os_index_270X)
{
  long obj_271X;
 {  if ((os_index_270X < 0)) {
    return;}
  else {
    if ((os_index_270X < (Snumber_of_channelsS))) {
      obj_271X = *((Svm_channelsS) + os_index_270X);
      if ((3 == (3 & obj_271X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_271X))) + -4))))>>2))))) {
          close_channelB((*((Svm_channelsS) + os_index_270X)));
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
  long link_279X;
  long name_278X;
  long entry_277X;
  long link_276X;
  char temp_275X;
  long i_274X;
  long table_273X;
  FILE * out_272X;
 {  out_272X = stderr;
  table_273X = Simported_bindingsS;
  arg0K0 = 0;
  goto L15013;}
 L15013: {
  i_274X = arg0K0;
  temp_275X = 1024 == i_274X;
  if (temp_275X) {
    return temp_275X;}
  else {
    link_276X = *((long *) ((((char *) (-3 + table_273X))) + (((i_274X)<<2))));
    if ((0 == (3 & link_276X))) {
      arg0K0 = (3 + (-4 & link_276X));
      goto L14987;}
    else {
      arg0K0 = link_276X;
      goto L14987;}}}
 L14987: {
  entry_277X = arg0K0;
  if ((1 == entry_277X)) {
    arg0K0 = (1 + i_274X);
    goto L15013;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + entry_277X))) + 8)))))) {
      name_278X = *((long *) (((char *) (-3 + entry_277X))));
      ps_write_string("undefined imported name ", out_272X);
      if ((3 == (3 & name_278X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_278X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + name_278X))))), out_272X);
          goto L16994;}
        else {
          goto L16992;}}
      else {
        goto L16992;}}
    else {
      goto L14994;}}}
 L16994: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_272X, ignoreXX) }
  goto L14994;}
 L16992: {
  ps_write_string("<invalid name>", out_272X);
  goto L16994;}
 L14994: {
  link_279X = *((long *) ((((char *) (-3 + entry_277X))) + 12));
  if ((0 == (3 & link_279X))) {
    arg0K0 = (3 + (-4 & link_279X));
    goto L14987;}
  else {
    arg0K0 = link_279X;
    goto L14987;}}
}
long s48_add_channel(long mode_280X, long id_281X, long os_index_282X)
{
  long status_284X;
  long channel_283X;
 {  s48_make_availableAgc(24);
  channel_283X = make_registered_channel((((mode_280X)>>2)), id_281X, os_index_282X, 0, &status_284X);
  if ((3 == (3 & channel_283X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_283X))) + -4))))>>2))))) {
      return channel_283X;}
    else {
      goto L17137;}}
  else {
    goto L17137;}}
 L17137: {
  return (((status_284X)<<2));}
}
long s48_enter_integer(long x_285X)
{
  char * v_286X;
 {  s48_make_availableAgc(16);
  if ((536870911 < x_285X)) {
    goto L18757;}
  else {
    if ((x_285X < -536870912)) {
      goto L18757;}
    else {
      return (((x_285X)<<2));}}}
 L18757: {
  v_286X = (char *) s48_long_to_bignum(x_285X);
  return enter_bignum(v_286X);}
}
long s48_get_imported_binding(char *name_287X)
{
  long value_289X;
  long value_288X;
 {  value_288X = enter_stringAgc(name_287X);
  Stemp0S = value_288X;
  s48_make_availableAgc(20);
  value_289X = Stemp0S;
  Stemp0S = 1;
  return Hlookup2550((Sexported_bindingsS), value_289X, 0);}
}
void s48_define_exported_binding(char *name_290X, long value_291X)
{
  char * addr_296X;
  long x_295X;
  long value_294X;
  long value_293X;
  long name_292X;
 {  Stemp0S = value_291X;
  name_292X = enter_stringAgc(name_290X);
  Stemp1S = name_292X;
  s48_make_availableAgc(20);
  value_293X = Stemp0S;
  Stemp0S = 1;
  value_294X = Stemp1S;
  Stemp1S = 1;
  x_295X = Hlookup2531((Simported_bindingsS), value_294X, 0);
  addr_296X = (((char *) (-3 + x_295X))) + 8;
  S48_WRITE_BARRIER(x_295X, addr_296X, value_293X);
  *((long *) addr_296X) = value_293X;
  return;}
}
void s48_initialize_vm(char * stack_begin_297X, long stack_size_298X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;

  int make_hash_tableAgc_return_tag;
  long make_hash_tableAgc0_return_value;
  char * addr_352X;
  long i_351X;
  long table_350X;
  long v_349X;
  long vector_348X;
  char * addr_347X;
  char * addr_346X;
  long x_345X;
  long v_344X;
  long v_343X;
  long vector_342X;
  char * addr_341X;
  char * addr_340X;
  long code_339X;
  long temp_338X;
  char * addr_337X;
  char * addr_336X;
  char * addr_335X;
  long code_334X;
  long temp_333X;
  char * addr_332X;
  char * addr_331X;
  long cont_330X;
  char * addr_329X;
  long code_328X;
  long temp_327X;
  char * addr_326X;
  char * addr_325X;
  char * a_324X;
  char * addr_323X;
  long value_322X;
  long size_321X;
  char * start_320X;
  char * addr_319X;
  long val_318X;
  long index_317X;
  long h_316X;
  long i_315X;
  char * stack_314X;
  long n_313X;
  long string_312X;
  long foo_311X;
  long table_310X;
  long i_309X;
  long v_308X;
  long v_307X;
  long imported_bindings_306X;
  long exported_bindings_305X;
  long n_304X;
  long symbols_303X;
  long maybe_302X;
  long maybe_301X;
  long v_300X;
  long symbol_table_299X;
 {  symbol_table_299X = s48_initial_symbols();
  if ((symbol_table_299X == 1)) {
    make_hash_tableAgc_return_tag = 0;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_0:
    v_300X = make_hash_tableAgc0_return_value;
    Sthe_symbol_tableS = v_300X;
    maybe_301X = s48_find_all(1);
    if ((maybe_301X == 1)) {
      s48_collect();
      maybe_302X = s48_find_all(1);
      if ((maybe_302X == 1)) {
        ps_error("insufficient heap space to build symbol table", 0);
        arg0K0 = maybe_302X;
        goto L19882;}
      else {
        arg0K0 = maybe_302X;
        goto L19882;}}
    else {
      arg0K0 = maybe_301X;
      goto L19882;}}
  else {
    Sthe_symbol_tableS = symbol_table_299X;
    goto L20353;}}
 L19882: {
  symbols_303X = arg0K0;
  n_304X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_303X))) + -4))))>>8))))>>2);
  arg0K0 = 0;
  goto L19903;}
 L20353: {
  exported_bindings_305X = s48_initial_exported_bindings();
  imported_bindings_306X = s48_initial_imported_bindings();
  if ((1 == imported_bindings_306X)) {
    make_hash_tableAgc_return_tag = 1;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_1:
    v_307X = make_hash_tableAgc0_return_value;
    Simported_bindingsS = v_307X;
    make_hash_tableAgc_return_tag = 2;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_2:
    v_308X = make_hash_tableAgc0_return_value;
    Sexported_bindingsS = v_308X;
    goto L20359;}
  else {
    Simported_bindingsS = imported_bindings_306X;
    Sexported_bindingsS = exported_bindings_305X;
    goto L20359;}}
 L19903: {
  i_309X = arg0K0;
  if ((i_309X == n_304X)) {
    goto L20353;}
  else {
    table_310X = Sthe_symbol_tableS;
    foo_311X = *((long *) ((((char *) (-3 + symbols_303X))) + (((i_309X)<<2))));
    string_312X = *((long *) (((char *) (-3 + foo_311X))));
    n_313X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_312X))) + -4))))>>8));
    arg0K0 = 0;
    arg0K1 = 0;
    goto L19310;}}
 L20359: {
  if ((stack_size_298X < 8131)) {
    stack_314X = (char *)malloc(32524);
    if ((stack_314X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_314X;
      arg0K1 = 8131;
      goto L17201;}
    else {
      arg1K0 = stack_314X;
      arg0K1 = 8131;
      goto L17201;}}
  else {
    arg1K0 = stack_begin_297X;
    arg0K1 = stack_size_298X;
    goto L17201;}}
 L19310: {
  i_315X = arg0K0;
  h_316X = arg0K1;
  if ((i_315X < n_313X)) {
    arg0K0 = (1 + i_315X);
    arg0K1 = (h_316X + (((*((unsigned char *) ((((char *) (-3 + string_312X))) + i_315X))))));
    goto L19310;}
  else {
    index_317X = 1023 & h_316X;
    val_318X = *((long *) ((((char *) (-3 + table_310X))) + (((index_317X)<<2))));
    addr_319X = (((char *) (-3 + foo_311X))) + 4;
    S48_WRITE_BARRIER(foo_311X, addr_319X, val_318X);
    *((long *) addr_319X) = val_318X;
    if ((3 == (3 & foo_311X))) {
      arg0K0 = (-4 & foo_311X);
      goto L19296;}
    else {
      arg0K0 = foo_311X;
      goto L19296;}}}
 L17201: {
  start_320X = arg1K0;
  size_321X = arg0K1;
  Sstack_beginS = start_320X;
  Sstack_endS = (start_320X + (((size_321X)<<2)));
  Sstack_limitS = ((Sstack_beginS) + 524);
  SstackS = ((Sstack_endS) + -4);
  ScontS = 1;
  SenvS = 17;
  arg1K0 = start_320X;
  goto L17233;}
 L19296: {
  value_322X = arg0K0;
  addr_323X = (((char *) (-3 + table_310X))) + (((index_317X)<<2));
  S48_WRITE_BARRIER(table_310X, addr_323X, value_322X);
  *((long *) addr_323X) = value_322X;
  arg0K0 = (1 + i_309X);
  goto L19903;}
 L17233: {
  a_324X = arg1K0;
  if ((a_324X == (Sstack_endS))) {
    s48_make_availableAgc(20);
    addr_325X = s48_allocate_small(6);
    *((long *) addr_325X) = 586;
    addr_326X = s48_allocate_small(12);
    *((long *) addr_326X) = 2098;
    temp_327X = 3 + (((long) (addr_326X + 4)));
    code_328X = 3 + (((long) (addr_325X + 4)));
    addr_329X = ((char *) (-3 + temp_327X));
    S48_WRITE_BARRIER(temp_327X, addr_329X, code_328X);
    *((long *) addr_329X) = code_328X;
    *((unsigned char *) (((char *) (-3 + code_328X)))) = 0;
    *((unsigned char *) ((((char *) (-3 + code_328X))) + 1)) = 71;
    SstackS = ((SstackS) + -20);
    *((long *) ((SstackS) + 4)) = 4266;
    cont_330X = 3 + (((long) ((SstackS) + 8)));
    *((long *) ((((char *) (-3 + cont_330X))) + 4)) = 0;
    *((long *) ((((char *) (-3 + cont_330X))) + 8)) = temp_327X;
    *((long *) ((((char *) (-3 + cont_330X))) + 12)) = (SenvS);
    *((long *) (((char *) (-3 + cont_330X)))) = (ScontS);
    ScontS = cont_330X;
    Sbottom_of_stackS = (ScontS);
    s48_make_availableAgc(40);
    addr_331X = s48_allocate_small(7);
    *((long *) addr_331X) = 842;
    addr_332X = s48_allocate_small(12);
    *((long *) addr_332X) = 2098;
    temp_333X = 3 + (((long) (addr_332X + 4)));
    code_334X = 3 + (((long) (addr_331X + 4)));
    addr_335X = ((char *) (-3 + temp_333X));
    S48_WRITE_BARRIER(temp_333X, addr_335X, code_334X);
    *((long *) addr_335X) = code_334X;
    *((unsigned char *) (((char *) (-3 + code_334X)))) = 0;
    *((unsigned char *) ((((char *) (-3 + code_334X))) + 1)) = 66;
    *((unsigned char *) ((((char *) (-3 + code_334X))) + 2)) = 147;
    Sinterrupt_templateS = temp_333X;
    addr_336X = s48_allocate_small(7);
    *((long *) addr_336X) = 842;
    addr_337X = s48_allocate_small(12);
    *((long *) addr_337X) = 2098;
    temp_338X = 3 + (((long) (addr_337X + 4)));
    code_339X = 3 + (((long) (addr_336X + 4)));
    addr_340X = ((char *) (-3 + temp_338X));
    S48_WRITE_BARRIER(temp_338X, addr_340X, code_339X);
    *((long *) addr_340X) = code_339X;
    *((unsigned char *) (((char *) (-3 + code_339X)))) = 0;
    *((unsigned char *) ((((char *) (-3 + code_339X))) + 1)) = 1;
    *((unsigned char *) ((((char *) (-3 + code_339X))) + 2)) = 144;
    Sexception_templateS = temp_338X;
    s48_make_availableAgc(32);
    s48_bignum_make_cached_constants();
    addr_341X = s48_allocate_tracedAgc(8);
    if ((addr_341X == NULL)) {
      arg0K0 = 1;
      goto L20410;}
    else {
      *((long *) addr_341X) = 1034;
      arg0K0 = (3 + (((long) (addr_341X + 4))));
      goto L20410;}}
  else {
    *((long *) a_324X) = 252645135;
    arg1K0 = (a_324X + 4);
    goto L17233;}}
 L20410: {
  vector_342X = arg0K0;
  if ((1 == vector_342X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_343X;
    goto L20397;}
  else {
    arg0K0 = vector_342X;
    goto L20397;}}
 L20397: {
  v_344X = arg0K0;
  Sempty_logS = v_344X;
  x_345X = Sempty_logS;
  addr_346X = ((char *) (-3 + x_345X));
  S48_WRITE_BARRIER(x_345X, addr_346X, 1);
  *((long *) addr_346X) = 1;
  return;}
 make_hash_tableAgc: {
{ addr_347X = s48_allocate_tracedAgc(4100);
  if ((addr_347X == NULL)) {
    arg0K0 = 1;
    goto L9544;}
  else {
    *((long *) addr_347X) = 1048586;
    arg0K0 = (3 + (((long) (addr_347X + 4))));
    goto L9544;}}
 L9544: {
  vector_348X = arg0K0;
  if ((1 == vector_348X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_349X;
    goto L9532;}
  else {
    arg0K0 = vector_348X;
    goto L9532;}}
 L9532: {
  table_350X = arg0K0;
  arg0K0 = 0;
  goto L9567;}
 L9567: {
  i_351X = arg0K0;
  if ((1024 == i_351X)) {
    make_hash_tableAgc0_return_value = table_350X;
    goto make_hash_tableAgc_return;}
  else {
    addr_352X = (((char *) (-3 + table_350X))) + (((i_351X)<<2));
    S48_WRITE_BARRIER(table_350X, addr_352X, 1);
    *((long *) addr_352X) = 1;
    arg0K0 = (1 + i_351X);
    goto L9567;}}
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
long s48_restart(long proc_353X, long nargs_354X)
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
  long status_355X;
  long list_356X;
  long marker_357X;
  long a_358X;
  long b_359X;
  long stob_360X;
  long index_361X;
  long stob_362X;
  long index_363X;
  long value_364X;
  long cont_365X;
  long stack_arg_count_366X;
  long thing_367X;
  long index_368X;
  long count_369X;
  long x_370X;
  long n_371X;
  long list_372X;
  long length_373X;
  long start_374X;
  long count_375X;
  long env_376X;
  long cont_377X;
  long key_378X;
  long reason_379X;
  long key_380X;
  long list_381X;
  long count_382X;
  long wants_stack_args_383X;
  long stack_arg_count_384X;
  long list_args_385X;
  long list_arg_count_386X;
  char *message_387X;
  long pc_1605X;
  long tem_1604X;
  long cont_1603X;
  long i_1602X;
  long string_1601X;
  char * addr_1600X;
  long len_1599X;
  long len_1598X;
  long x_1597X;
  char *string_1596X;
  char move_slowP_1595X;
  long slow_1594X;
  long len_1593X;
  long fast_1592X;
  long v_1591X;
  char *v_1590X;
  long v_1589X;
  long v_1588X;
  long obj_1587X;
  long env_1586X;
  long thread_1585X;
  long x_1584X;
  char * addr_1583X;
  long v_1582X;
  long next_stob_1581X;
  long i_1580X;
  long log_1579X;
  char * addr_1578X;
  long next_stob_1577X;
  long i_1576X;
  long log_1575X;
  long v_1574X;
  char * x_1573X;
  char * new_stack_1572X;
  long new_cont_1571X;
  char * top_1570X;
  long v_1569X;
  long v_1568X;
  long n_1567X;
  long extra_1566X;
  long x_size_1565X;
  char * addr_1564X;
  long x_1563X;
  char * addr_1562X;
  long a_1561X;
  long last_1560X;
  long l_1559X;
  long x_1558X;
  char * addr_1557X;
  long a_1556X;
  long value_1555X;
  long x_1554X;
  char * addr_1553X;
  long a_1552X;
  long count_1551X;
  long args_1550X;
  long value_1549X;
  long env_1548X;
  long cont_1547X;
  char * addr_1546X;
  char * addr_1545X;
  long new_1544X;
  char * data_addr_1543X;
  char * addr_1542X;
  long header_1541X;
  long env_1540X;
  long p_1539X;
  long env_1538X;
  char * addr_1537X;
  long new_1536X;
  char * data_addr_1535X;
  char * addr_1534X;
  long header_1533X;
  long new_1532X;
  char * data_addr_1531X;
  char * addr_1530X;
  long header_1529X;
  long p_1528X;
  long previous_1527X;
  long cont_1526X;
  long end_1525X;
  long v_1524X;
  long p_1523X;
  char * arg_1522X;
  char * loc_1521X;
  char * top_1520X;
  long arg_count_1519X;
  long i_1518X;
  char * p_1517X;
  long l_1516X;
  long i_1515X;
  long list_1514X;
  long x_1513X;
  long v_1512X;
  long x_1511X;
  long v_1510X;
  long count_1509X;
  long x_1508X;
  long v_1507X;
  long status_1506X;
  long channel_1505X;
  long type_1504X;
  char v_1503X;
  long template_1502X;
  char not_firstP_1501X;
  long cont_1500X;
  char not_firstP_1499X;
  FILE * out_1498X;
  long current_template_1497X;
  long why_1496X;
  long val_1495X;
  long val_1494X;
  long val_1493X;
  long val_1492X;
  long val_1491X;
  long val_1490X;
  long n_1489X;
  char v_1488X;
  char * external_bignum_1487X;
  char * x_1486X;
  long val_1485X;
  long n_1484X;
  char v_1483X;
  char * external_bignum_1482X;
  char * x_1481X;
  long val_1480X;
  long n_1479X;
  char v_1478X;
  char * external_bignum_1477X;
  char * x_1476X;
  long v_1475X;
  long n_1474X;
  char v_1473X;
  long v_1472X;
  long rem_1471X;
  long quot_1470X;
  char div_by_zeroP_1469X;
  long val_1468X;
  long n_1467X;
  char v_1466X;
  char * external_bignum_1465X;
  char * x_1464X;
  long val_1463X;
  long n_1462X;
  char v_1461X;
  char * external_bignum_1460X;
  char * x_1459X;
  long val_1458X;
  long n_1457X;
  char v_1456X;
  char * external_bignum_1455X;
  char * x_1454X;
  long link_1453X;
  long v_1452X;
  long v_1451X;
  long stob_1450X;
  long v_1449X;
  long stob_1448X;
  char * addr_1447X;
  long x_1446X;
  char * addr_1445X;
  char * addr_1444X;
  long value_1443X;
  char * addr_1442X;
  long value_1441X;
  long copies_1440X;
  char * v_1439X;
  long value_1438X;
  char * y_1437X;
  char * v_1436X;
  long value_1435X;
  char * y_1434X;
  char * v_1433X;
  long value_1432X;
  char * y_1431X;
  long val_1430X;
  long v_1429X;
  long n_1428X;
  char v_1427X;
  char * rem_1426X;
  char * quot_1425X;
  char div_by_zeroP_1424X;
  char * x_1423X;
  char * v_1422X;
  long value_1421X;
  char * y_1420X;
  char * v_1419X;
  long value_1418X;
  char * y_1417X;
  char * v_1416X;
  long value_1415X;
  char * y_1414X;
  long obj_1413X;
  char * addr_1412X;
  long val_1411X;
  char * addr_1410X;
  long value_1409X;
  long len_1408X;
  long s2_1407X;
  long foo_1406X;
  long previous_foo_1405X;
  long value_1404X;
  long verify_1403X;
  long copies_1402X;
  long stob_1401X;
  long i_1400X;
  long x_1399X;
  long x_1398X;
  long val_1397X;
  char * addr_1396X;
  long value_1395X;
  char * v_1394X;
  long value_1393X;
  long v_1392X;
  char * v_1391X;
  long value_1390X;
  long v_1389X;
  char * v_1388X;
  long value_1387X;
  long v_1386X;
  long val_1385X;
  long val_1384X;
  long n_1383X;
  char v_1382X;
  char * external_bignum_1381X;
  char * x_1380X;
  long val_1379X;
  long n_1378X;
  char v_1377X;
  char * external_bignum_1376X;
  char * x_1375X;
  char * v_1374X;
  long value_1373X;
  char * y_1372X;
  char * v_1371X;
  long value_1370X;
  long v_1369X;
  long val_1368X;
  long v_1367X;
  char * v_1366X;
  char * v_1365X;
  long value_1364X;
  long extra1_1363X;
  long length1_1362X;
  char * v_1361X;
  long value_1360X;
  long v_1359X;
  long v_1358X;
  long bucket_1357X;
  char * addr_1356X;
  long value_1355X;
  long verify_1354X;
  long log_1353X;
  long stob_1352X;
  long i_1351X;
  long x_1350X;
  long status_1349X;
  char pendingP_1348X;
  char eofP_1347X;
  long got_1346X;
  long y_1345X;
  long link_1344X;
  long val_1343X;
  long x_1342X;
  char * addr_1341X;
  long b_1340X;
  long val_1339X;
  long extra1_1338X;
  long length1_1337X;
  long extra1_1336X;
  long length1_1335X;
  long extra1_1334X;
  long length1_1333X;
  long n_1332X;
  char * v_1331X;
  long val_1330X;
  long n_1329X;
  char v_1328X;
  char * external_bignum_1327X;
  char * v_1326X;
  long n_1325X;
  char v_1324X;
  char * external_bignum_1323X;
  char * v_1322X;
  long value_1321X;
  char * y_1320X;
  char * v_1319X;
  long value_1318X;
  char * y_1317X;
  char * v_1316X;
  long value_1315X;
  long extra1_1314X;
  long val_1313X;
  long v_1312X;
  char * v_1311X;
  long extra1_1310X;
  long length1_1309X;
  long val_1308X;
  long v_1307X;
  char * v_1306X;
  long x_1305X;
  long val_1304X;
  long v_1303X;
  char * v_1302X;
  long extra0_1301X;
  long length0_1300X;
  long extra1_1299X;
  long length1_1298X;
  long final_stack_arg_count_1297X;
  char v_1296X;
  char x_1295X;
  long channel_1294X;
  long handlers_1293X;
  long arg_count_1292X;
  char * addr_1291X;
  long value_1290X;
  long offset_1289X;
  long i_1288X;
  long count_1287X;
  char * addr_1286X;
  long value_1285X;
  long offset_1284X;
  long i_1283X;
  long count_1282X;
  char *v_1281X;
  long link_1280X;
  long index_1279X;
  long h_1278X;
  long i_1277X;
  char * addr_1276X;
  char * addr_1275X;
  long value_1274X;
  char * addr_1273X;
  char * addr_1272X;
  char * addr_1271X;
  long value_1270X;
  char * addr_1269X;
  char * addr_1268X;
  long value_1267X;
  long proposal_1266X;
  long entry_1265X;
  long thing_1264X;
  long log_1263X;
  long copies_1262X;
  char * addr_1261X;
  long x_1260X;
  long val_1259X;
  long status_1258X;
  long v_1257X;
  long len_1256X;
  long s2_1255X;
  long foo_1254X;
  long val_1253X;
  long v_1252X;
  long n_1251X;
  char v_1250X;
  char * external_bignum_1249X;
  char * x_1248X;
  long extra0_1247X;
  long length0_1246X;
  long extra0_1245X;
  long length0_1244X;
  long extra0_1243X;
  long length0_1242X;
  char * v_1241X;
  long value_1240X;
  long v_1239X;
  char * v_1238X;
  long value_1237X;
  long v_1236X;
  char * v_1235X;
  long v_1234X;
  char * x_1233X;
  char * v_1232X;
  long n_1231X;
  char * v_1230X;
  long value_1229X;
  long extra1_1228X;
  char * v_1227X;
  long value_1226X;
  long extra1_1225X;
  long val_1224X;
  long val_1223X;
  long val_1222X;
  long val_1221X;
  long val_1220X;
  long extra0_1219X;
  long length0_1218X;
  long val_1217X;
  long v_1216X;
  char * v_1215X;
  long x_1214X;
  long val_1213X;
  long v_1212X;
  char * v_1211X;
  long extra0_1210X;
  long length0_1209X;
  long y_1208X;
  long x_1207X;
  long extra0_1206X;
  long length0_1205X;
  char * arg_1204X;
  char * loc_1203X;
  long l_1202X;
  long stack_nargs_1201X;
  long x_1200X;
  long v_1199X;
  long skip_1198X;
  long template_1197X;
  long stack_arg_count_1196X;
  long skip_1195X;
  char * addr_1194X;
  long next_1193X;
  long channel_1192X;
  long m_1191X;
  long i_1190X;
  long count_1189X;
  long index_1188X;
  long i_1187X;
  long env_1186X;
  long count_1185X;
  long i_1184X;
  long env_1183X;
  long i_1182X;
  long l_1181X;
  long n_1180X;
  long table_1179X;
  long val_1178X;
  char temp_1177X;
  char minutesP_1176X;
  long v_1175X;
  long vector_1174X;
  char * addr_1173X;
  long x_1172X;
  long value_1171X;
  long verify_1170X;
  long copies_1169X;
  long stob_1168X;
  long i_1167X;
  char * addr_1166X;
  long v_1165X;
  char * addr_1164X;
  char * addr_1163X;
  long val_1162X;
  long x_1161X;
  long status_1160X;
  char pendingP_1159X;
  long got_1158X;
  long y_1157X;
  char v_1156X;
  long count_1155X;
  long start_1154X;
  char waitP_1153X;
  long x_1152X;
  long status_1151X;
  long channel_1150X;
  long v_1149X;
  long v_1148X;
  long v_1147X;
  long reason_1146X;
  long channel_1145X;
  long index_1144X;
  long bucket_1143X;
  long i_1142X;
  long i_1141X;
  char * addr_1140X;
  long i_1139X;
  long rest_list_1138X;
  long i_1137X;
  long v_1136X;
  long n_1135X;
  char v_1134X;
  char * external_bignum_1133X;
  char * x_1132X;
  long val_1131X;
  long v_1130X;
  char * v_1129X;
  char * v_1128X;
  long value_1127X;
  long needed_1126X;
  long y_1125X;
  long y_1124X;
  long x_1123X;
  long val_1122X;
  long v_1121X;
  char * v_1120X;
  long val_1119X;
  long v_1118X;
  char * v_1117X;
  long extra_1116X;
  long length_1115X;
  long count_1114X;
  long x_1113X;
  long extra_1112X;
  long length_1111X;
  char * v_1110X;
  long value_1109X;
  long extra_1108X;
  long length_1107X;
  long x_1106X;
  long c_1105X;
  long b_1104X;
  long extra0_1103X;
  long length0_1102X;
  long c_1101X;
  long b_1100X;
  long extra0_1099X;
  long length0_1098X;
  long v_1097X;
  long v_1096X;
  long v_1095X;
  long v_1094X;
  long v_1093X;
  long v_1092X;
  long v_1091X;
  long v_1090X;
  long v_1089X;
  long v_1088X;
  long v_1087X;
  long v_1086X;
  char b_1085X;
  char x_1084X;
  long c_1083X;
  long b_1082X;
  long c_1081X;
  long mid_c_1080X;
  long v_1079X;
  long v_1078X;
  long lo_c_1077X;
  long hi_b_1076X;
  long hi_a_1075X;
  long lo_b_1074X;
  long lo_a_1073X;
  long b_1072X;
  long args_1071X;
  char * arg_top_1070X;
  long list_arg_count_1069X;
  long list_args_1068X;
  long stack_nargs_1067X;
  long bytes_used_1066X;
  long count_1065X;
  long v_1064X;
  char * arg_1063X;
  char * loc_1062X;
  long args_1061X;
  long v_1060X;
  long cont_1059X;
  long index_1058X;
  long length_1057X;
  long wants_stack_args_1056X;
  long skip_1055X;
  long skip_1054X;
  long stack_space_1053X;
  long protocol_1052X;
  long n_1051X;
  long tem_1050X;
  char * addr_1049X;
  long x_1048X;
  long stack_arg_count_1047X;
  long back_1046X;
  long env_1045X;
  long offset_1044X;
  long i_1043X;
  long back_1042X;
  long env_1041X;
  long offset_1040X;
  long i_1039X;
  long obj_1038X;
  long obj_1037X;
  long type_1036X;
  long thing_1035X;
  long stuff_1034X;
  char * addr_1033X;
  long val_1032X;
  long x_1031X;
  long i_1030X;
  long b_1029X;
  long p_1028X;
  long port_1027X;
  long Kchar_1026X;
  long x_1025X;
  long i_1024X;
  long b_1023X;
  long p_1022X;
  long p_1021X;
  long port_1020X;
  char * addr_1019X;
  long val_1018X;
  long x_1017X;
  long i_1016X;
  long b_1015X;
  long p_1014X;
  long p_1013X;
  long port_1012X;
  char * addr_1011X;
  long value_1010X;
  long list_1009X;
  long head_1008X;
  char move_slowP_1007X;
  long slow_1006X;
  long list_1005X;
  long string_1004X;
  char * addr_1003X;
  long len_1002X;
  long val_1001X;
  long mseconds_1000X;
  long seconds_999X;
  long option_998X;
  long v_997X;
  long v_996X;
  long result_995X;
  char * args_994X;
  long name_993X;
  long proc_992X;
  long rest_list_991X;
  long x_990X;
  long tem_989X;
  long pc_988X;
  char * addr_987X;
  long x_986X;
  long proposal_985X;
  long value_984X;
  long vector_983X;
  long type_982X;
  char firstP_981X;
  long vector_980X;
  char firstP_979X;
  long h_978X;
  long i_977X;
  long x_976X;
  long status_975X;
  long reason_974X;
  char * addr_973X;
  long next_stob_972X;
  long i_971X;
  long v_970X;
  long next_stob_969X;
  long i_968X;
  long value_967X;
  char * addr_966X;
  long count_965X;
  long to_index_964X;
  long from_index_963X;
  long copies_962X;
  long left_961X;
  long value_960X;
  long value_959X;
  long verify_958X;
  long log_957X;
  long stob_956X;
  long i_955X;
  char * addr_954X;
  long old_953X;
  long x_952X;
  char * addr_951X;
  long channel_950X;
  long res_949X;
  long i_948X;
  long y_947X;
  long n_946X;
  char * addr_945X;
  long prev_944X;
  long ch_943X;
  long val_942X;
  long val_941X;
  long v_940X;
  long v_939X;
  long val_938X;
  char * addr_937X;
  char * addr_936X;
  long val_935X;
  long link_934X;
  long index_933X;
  long h_932X;
  long i_931X;
  long vector_930X;
  long vector_929X;
  char * addr_928X;
  long value_927X;
  long value_926X;
  long x_925X;
  char * addr_924X;
  long i_923X;
  long i_922X;
  long value_921X;
  long val_920X;
  long val_919X;
  long val_918X;
  long val_917X;
  char * v_916X;
  long value_915X;
  long needed_914X;
  long y_913X;
  long x_912X;
  long result_911X;
  long x_910X;
  long x_909X;
  long x_908X;
  long count_907X;
  long value_906X;
  long val_905X;
  long val_904X;
  long x_903X;
  long x_902X;
  long n_901X;
  long a_900X;
  long y_899X;
  long x_898X;
  long a_897X;
  long y_896X;
  long x_895X;
  long val_894X;
  long val_893X;
  long val_892X;
  long val_891X;
  long val_890X;
  long a_889X;
  long val_888X;
  long v_887X;
  char * v_886X;
  long a_885X;
  long val_884X;
  long v_883X;
  char * v_882X;
  long val_881X;
  long val_880X;
  long delta_879X;
  long delta_878X;
  long offset_877X;
  char v_876X;
  long tem_875X;
  long args_874X;
  long index_873X;
  char * arg_top_872X;
  long args_871X;
  long count_870X;
  long index_869X;
  long next_868X;
  long cont_867X;
  long args_866X;
  long protocol_865X;
  long pc_864X;
  long code_863X;
  long cont_862X;
  long list_arg_count_861X;
  long list_args_860X;
  long stack_nargs_859X;
  long args_858X;
  long args_857X;
  long x_856X;
  char * addr_855X;
  long a_854X;
  long wants_stack_args_853X;
  long index_852X;
  long next_851X;
  long cont_850X;
  long protocol_849X;
  long pc_848X;
  long code_847X;
  long cont_846X;
  long obj_845X;
  char * addr_844X;
  long list_args_843X;
  long follower_842X;
  long list_841X;
  long proc_840X;
  long args_839X;
  long list_arg_count_838X;
  char okayP_837X;
  long stack_nargs_836X;
  long list_args_835X;
  long obj_834X;
  long obj_833X;
  long list_arg_count_832X;
  long list_args_831X;
  long stack_arg_count_830X;
  long args_829X;
  long v_828X;
  long v_827X;
  long v_826X;
  long list_arg_count_825X;
  long list_args_824X;
  long stack_arg_count_823X;
  long exception_822X;
  long total_arg_count_821X;
  long code_820X;
  long list_arg_count_819X;
  long list_args_818X;
  long stack_arg_count_817X;
  char * arg_816X;
  char * loc_815X;
  char v_814X;
  long value_813X;
  long start_i_812X;
  long start_i_811X;
  char * addr_810X;
  long closure_809X;
  long index_808X;
  char * addr_807X;
  long x_806X;
  char * addr_805X;
  long value_804X;
  long i_803X;
  long value_802X;
  long key_801X;
  long env_800X;
  long v_799X;
  long p_798X;
  long v_797X;
  char * addr_796X;
  long value_795X;
  long index_794X;
  long i_793X;
  long env_792X;
  long i_791X;
  long env_790X;
  long i_789X;
  long env_788X;
  long handlers_787X;
  long opcode_786X;
  long nargs_785X;
  FILE * out_784X;
  long x_783X;
  long v_782X;
  long v_781X;
  long v_780X;
  long v_779X;
  long v_778X;
  long v_777X;
  long v_776X;
  long v_775X;
  long len_774X;
  long value_773X;
  long index_772X;
  long arg4_771X;
  long arg3_770X;
  long arg2_769X;
  long v_768X;
  long v_767X;
  long len_766X;
  long index_765X;
  long arg3_764X;
  long arg2_763X;
  long list_762X;
  long thing_761X;
  long n_760X;
  long arg2_759X;
  long len_758X;
  long x_757X;
  long obj_756X;
  long arg2_755X;
  long x_754X;
  long arg2_753X;
  long status_752X;
  long value_751X;
  long key_750X;
  long arg2_749X;
  long mseconds_748X;
  long seconds_747X;
  long mseconds_746X;
  long seconds_745X;
  long x_744X;
  long other_743X;
  long option_742X;
  long arg2_741X;
  long x_740X;
  long arg2_739X;
  long x_738X;
  long arg2_737X;
  long rest_list_736X;
  long p_735X;
  long nargs_734X;
  long x_733X;
  long arg2_732X;
  long p_731X;
  long p_730X;
  long p_729X;
  long old_728X;
  long temp_727X;
  long obj_726X;
  long p_725X;
  long template_724X;
  long p_723X;
  long temp_722X;
  long obj_721X;
  long val_720X;
  long x_719X;
  long type_718X;
  long x_717X;
  long bytes_716X;
  long x_715X;
  long other_714X;
  long key_713X;
  long arg2_712X;
  long x_711X;
  char * addr_710X;
  long b_709X;
  long x_708X;
  char * addr_707X;
  long proc_706X;
  long stob_705X;
  long n_704X;
  long x_703X;
  long obj_702X;
  long status_701X;
  long status_700X;
  long status_699X;
  long status_698X;
  long status_697X;
  long status_696X;
  FILE * port_695X;
  long undumpables_694X;
  long obj_693X;
  long arg4_692X;
  long arg3_691X;
  long arg2_690X;
  long x_689X;
  long log_688X;
  long index_687X;
  long x_686X;
  long len_685X;
  long byte_684X;
  long index_683X;
  long arg3_682X;
  long arg2_681X;
  long log_680X;
  long index_679X;
  long x_678X;
  long len_677X;
  long index_676X;
  long arg2_675X;
  char v_674X;
  char v_673X;
  long v_672X;
  long count_671X;
  long to_index_670X;
  long from_index_669X;
  long arg5_668X;
  long arg4_667X;
  long arg3_666X;
  long arg2_665X;
  long v_664X;
  long x_663X;
  long type_662X;
  long offset_661X;
  long stob_660X;
  long log_659X;
  long proposal_658X;
  long proposal_657X;
  char * addr_656X;
  long next_655X;
  long channel_654X;
  long n_653X;
  char * addr_652X;
  long head_651X;
  long channel_650X;
  long obj_649X;
  long x_648X;
  long status_647X;
  char readyP_646X;
  long channel_645X;
  long obj_644X;
  char v_643X;
  long count_642X;
  long start_641X;
  long arg4_640X;
  long arg3_639X;
  long arg2_638X;
  long x_637X;
  long arg5_636X;
  long arg4_635X;
  long arg3_634X;
  long arg2_633X;
  long x_632X;
  long status_631X;
  long channel_630X;
  long obj_629X;
  long mode_628X;
  long arg2_627X;
  long x_626X;
  long x_625X;
  long x_624X;
  long arg2_623X;
  long descriptor_622X;
  long x_621X;
  long obj_620X;
  long n_619X;
  long string_618X;
  long table_617X;
  long obj_616X;
  long len_615X;
  char Kchar_614X;
  long index_613X;
  long arg3_612X;
  long arg2_611X;
  long len_610X;
  long index_609X;
  long arg2_608X;
  long obj_607X;
  char * addr_606X;
  long len_605X;
  char init_604X;
  long len_603X;
  long arg2_602X;
  long len_601X;
  long Kchar_600X;
  long index_599X;
  long arg3_598X;
  long arg2_597X;
  long len_596X;
  long index_595X;
  long arg2_594X;
  long obj_593X;
  char * addr_592X;
  long init_591X;
  long len_590X;
  long arg2_589X;
  long v_588X;
  long index_587X;
  long len_586X;
  long type_585X;
  long value_584X;
  long stob_583X;
  long arg2_582X;
  long v_581X;
  long v_580X;
  long index_579X;
  long len_578X;
  long type_577X;
  long index_576X;
  long stob_575X;
  char * addr_574X;
  long len_573X;
  long len_572X;
  long type_571X;
  long init_570X;
  long len_569X;
  long v_568X;
  long type_567X;
  long offset_566X;
  long value_565X;
  long stob_564X;
  long type_563X;
  long offset_562X;
  long stob_561X;
  long rest_list_560X;
  long stack_nargs_559X;
  long new_558X;
  char * addr_557X;
  long len_556X;
  long type_555X;
  long len_554X;
  long new_553X;
  char * addr_552X;
  long len_551X;
  long type_550X;
  long len_549X;
  long type_548X;
  long stob_547X;
  long type_546X;
  long x_545X;
  long x_544X;
  long x_543X;
  long x_542X;
  long arg2_541X;
  long x_540X;
  long arg2_539X;
  long x_538X;
  long v_537X;
  long v_536X;
  long y_535X;
  long x_534X;
  long y_533X;
  long x_532X;
  long y_531X;
  long x_530X;
  long y_529X;
  long x_528X;
  long x_527X;
  long x_526X;
  long x_525X;
  long x_524X;
  long arg2_523X;
  long x_522X;
  long arg2_521X;
  long x_520X;
  long arg2_519X;
  long x_518X;
  long v_517X;
  long x_516X;
  long x_515X;
  long x_514X;
  long x_513X;
  long x_512X;
  long x_511X;
  long x_510X;
  long x_509X;
  long x_508X;
  long n_507X;
  long n_506X;
  long n_505X;
  long n_504X;
  long n_503X;
  long a_502X;
  long y_501X;
  long x_500X;
  long b_499X;
  long a_498X;
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
  long y_487X;
  long x_486X;
  long b_485X;
  long a_484X;
  long y_483X;
  long x_482X;
  long x_481X;
  long y_480X;
  long x_479X;
  long b_478X;
  long a_477X;
  long y_476X;
  long x_475X;
  long x_474X;
  long y_473X;
  long x_472X;
  long x_471X;
  long x_470X;
  long n_469X;
  long n_468X;
  long n_467X;
  long n_466X;
  long n_465X;
  long x_464X;
  long x_463X;
  long arg2_462X;
  long rest_list_461X;
  long x_460X;
  long rest_list_459X;
  long stack_nargs_458X;
  long arg0_457X;
  long arg1_456X;
  long rest_list_455X;
  long arg0_454X;
  long stack_nargs_453X;
  long index_452X;
  long val_451X;
  long max_450X;
  long p_449X;
  long space_448X;
  long index_447X;
  long v_446X;
  long code_445X;
  long tem_444X;
  long stack_nargs_443X;
  long p_442X;
  long cont_441X;
  long rest_list_440X;
  long proc_439X;
  long stack_nargs_438X;
  long args_437X;
  long length_436X;
  char okayP_435X;
  long list_args_434X;
  long obj_433X;
  long stack_arg_count_432X;
  char * top_of_cont_431X;
  char * top_of_args_430X;
  long proc_429X;
  long protocol_428X;
  long code_427X;
  long template_426X;
  long obj_425X;
  long stack_arg_count_424X;
  long v_423X;
  char * addr_422X;
  long value_421X;
  long v_420X;
  long new_env_419X;
  char * addr_418X;
  long len_417X;
  long total_count_416X;
  char * addr_415X;
  long value_414X;
  long v_413X;
  long new_env_412X;
  char * addr_411X;
  long len_410X;
  long total_count_409X;
  long vector_408X;
  char * addr_407X;
  long len_406X;
  long count_405X;
  long p_404X;
  long v_403X;
  char * addr_402X;
  long val_401X;
  long location_400X;
  long location_399X;
  long env_398X;
  long back_397X;
  long env_396X;
  long back_395X;
  long env_394X;
  long back_393X;
  long count_392X;
  char * code_pointer_391X;
  long code_390X;
  long value_389X;
  char * addr_388X;
 {  Stemp0S = proc_353X;
  s48_make_availableAgc(8);
  addr_388X = s48_allocate_small(6);
  *((long *) addr_388X) = 586;
  value_389X = Stemp0S;
  Stemp0S = 1;
  code_390X = 3 + (((long) (addr_388X + 4)));
  *((unsigned char *) (((char *) (-3 + code_390X)))) = 25;
  *((unsigned char *) ((((char *) (-3 + code_390X))) + 1)) = nargs_354X;
  Scode_pointerS = (((char *) (-3 + code_390X)));
  SvalS = value_389X;
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L23052: {
  code_pointer_391X = arg1K0;
  switch ((*((unsigned char *) code_pointer_391X))) {
    case 0 : {
      push_exception_continuationB(14, 1);
      arg0K0 = 0;
      goto L21148;}
      break;
    case 1 : {
      count_392X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      *((long *) (SstackS)) = (SenvS);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (1034 + (((count_392X)<<10)));
      SstackS = ((SstackS) + -4);
      SenvS = (3 + (((long) ((SstackS) + 8))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 2 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 3 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 4 : {
      back_393X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_394X = SenvS;
      arg0K0 = env_394X;
      arg0K1 = back_393X;
      goto L30324;}
      break;
    case 5 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21148;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23052;}}
      break;
    case 6 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21148;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23052;}}
      break;
    case 7 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS)))))))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21148;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23052;}}
      break;
    case 8 : {
      back_395X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_396X = SenvS;
      arg0K0 = env_396X;
      arg0K1 = back_395X;
      goto L25019;}
      break;
    case 9 : {
      back_397X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_398X = SenvS;
      arg0K0 = env_398X;
      arg0K1 = back_397X;
      goto L24957;}
      break;
    case 10 : {
      location_399X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_399X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        push_exception_continuationB(1, 3);
        *((long *) (SstackS)) = location_399X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21148;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23052;}}
      break;
    case 11 : {
      location_400X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      if ((273 == (*((long *) ((((char *) (-3 + location_400X))) + 4))))) {
        push_exception_continuationB(1, 3);
        *((long *) (SstackS)) = location_400X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}
      else {
        val_401X = SvalS;
        addr_402X = (((char *) (-3 + location_400X))) + 4;
        S48_WRITE_BARRIER(location_400X, addr_402X, val_401X);
        *((long *) addr_402X) = val_401X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23052;}}
      break;
    case 12 : {
      v_403X = *((unsigned char *) ((Scode_pointerS) + 3));
      if ((0 == v_403X)) {
        p_404X = SenvS;
        if ((3 == (3 & p_404X))) {
          if ((p_404X < (((long) (Sstack_beginS))))) {
            arg0K0 = 0;
            goto L25694;}
          else {
            if (((((long) (Sstack_endS))) < p_404X)) {
              arg0K0 = 0;
              goto L25694;}
            else {
              arg0K0 = ((Sstack_endS) - (Sstack_beginS));
              goto L25694;}}}
        else {
          arg0K0 = 0;
          goto L25694;}}
      else {
        s48_make_availableAgc(12);
        arg0K0 = (SvalS);
        arg0K1 = 0;
        goto L25707;}}
      break;
    case 13 : {
      count_405X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      s48_make_availableAgc((((((2 + count_405X) + (3 * count_405X)))<<2)));
      len_406X = 4 + (((count_405X)<<2));
      addr_407X = s48_allocate_small((4 + len_406X));
      *((long *) addr_407X) = (10 + (((len_406X)<<8)));
      vector_408X = 3 + (((long) (addr_407X + 4)));
      arg0K0 = 1;
      goto L24744;}
      break;
    case 14 : {
      total_count_409X = *((unsigned char *) ((Scode_pointerS) + 2));
      s48_make_availableAgc((4 + (((total_count_409X)<<2))));
      len_410X = ((total_count_409X)<<2);
      addr_411X = s48_allocate_small((4 + len_410X));
      *((long *) addr_411X) = (10 + (((len_410X)<<8)));
      new_env_412X = 3 + (((long) (addr_411X + 4)));
      v_413X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_413X)) {
        arg0K0 = 0;
        goto L26031;}
      else {
        value_414X = SvalS;
        addr_415X = ((char *) (-3 + new_env_412X));
        S48_WRITE_BARRIER(new_env_412X, addr_415X, value_414X);
        *((long *) addr_415X) = value_414X;
        arg0K0 = 1;
        goto L26031;}}
      break;
    case 15 : {
      total_count_416X = ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 3)));
      s48_make_availableAgc((4 + (((total_count_416X)<<2))));
      len_417X = ((total_count_416X)<<2);
      addr_418X = s48_allocate_small((4 + len_417X));
      *((long *) addr_418X) = (10 + (((len_417X)<<8)));
      new_env_419X = 3 + (((long) (addr_418X + 4)));
      v_420X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_420X)) {
        arg0K0 = 0;
        goto L25882;}
      else {
        value_421X = SvalS;
        addr_422X = ((char *) (-3 + new_env_419X));
        S48_WRITE_BARRIER(new_env_419X, addr_422X, value_421X);
        *((long *) addr_422X) = value_421X;
        arg0K0 = 1;
        goto L25882;}}
      break;
    case 16 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 17 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L21148;}
      else {
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23052;}}
      break;
    case 18 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L21148;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23052;}}
      break;
    case 19 : {
      SstackS = ((SstackS) + 4);
      SvalS = (*((long *) (SstackS)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 20 : {
      SvalS = (*((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 21 : {
      *((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 22 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (*((unsigned char *) ((Scode_pointerS) + 3))));
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 23 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))));
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 24 : {
      s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
      if ((1 == (ScontS))) {
        arg0K0 = 1;
        goto L46173;}
      else {
        merged_arg0K0 = 0;
        merged_arg0K1 = 2;
        really_preserve_continuation_return_tag = 0;
        goto really_preserve_continuation;
       really_preserve_continuation_return_0:
        v_423X = really_preserve_continuation0_return_value;
        arg0K0 = v_423X;
        goto L46173;}}
      break;
    case 25 : {
      stack_arg_count_424X = *((unsigned char *) ((Scode_pointerS) + 1));
      obj_425X = SvalS;
      if ((3 == (3 & obj_425X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_425X))) + -4))))>>2))))) {
          template_426X = *((long *) (((char *) (-3 + (SvalS)))));
          code_427X = *((long *) (((char *) (-3 + template_426X))));
          protocol_428X = *((unsigned char *) ((((char *) (-3 + code_427X))) + 1));
          if ((protocol_428X == stack_arg_count_424X)) {
            StemplateS = template_426X;
            SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
            Scode_pointerS = ((((char *) (-3 + code_427X))) + 2);
            if (((SstackS) < (Sstack_limitS))) {
              s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
              merged_arg0K0 = 0;
              copy_stack_into_heap_return_tag = 0;
              goto copy_stack_into_heap;
             copy_stack_into_heap_return_0:
              if (((SstackS) < (Sstack_limitS))) {
                ps_error("Couldn't get default procedure space (how can this happen?)", 0);
                goto L23709;}
              else {
                goto L23709;}}
            else {
              goto L23709;}}
          else {
            if (((127 & protocol_428X) == stack_arg_count_424X)) {
              proc_429X = SvalS;
              StemplateS = (*((long *) (((char *) (-3 + proc_429X)))));
              Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS))))))))) + 2);
              top_of_args_430X = SstackS;
              top_of_cont_431X = (((char *) (-3 + (ScontS)))) + -8;
              if ((top_of_cont_431X == (top_of_args_430X + (((stack_arg_count_424X)<<2))))) {
                s48_call_native_code(proc_429X);
                goto L23719;}
              else {
                SstackS = top_of_cont_431X;
                arg1K0 = (SstackS);
                arg1K1 = (top_of_args_430X + (((stack_arg_count_424X)<<2)));
                goto L7658;}}
            else {
              arg0K0 = stack_arg_count_424X;
              arg0K1 = 25;
              arg0K2 = 0;
              goto L20778;}}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_424X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L21650;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_424X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L21650;}}
      break;
    case 26 : {
      stack_arg_count_432X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      obj_433X = SvalS;
      if ((3 == (3 & obj_433X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_433X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_432X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L20778;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_432X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L21650;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_432X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L21650;}}
      break;
    case 27 : {
      SstackS = ((SstackS) + 4);
      list_args_434X = *((long *) (SstackS));
      merged_arg0K0 = list_args_434X;
      okay_argument_list_return_tag = 0;
      goto okay_argument_list;
     okay_argument_list_return_0:
      okayP_435X = okay_argument_list0_return_value;
      length_436X = okay_argument_list1_return_value;
      if (okayP_435X) {
        arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
        arg0K1 = list_args_434X;
        arg0K2 = length_436X;
        goto L22278;}
      else {
        *((long *) (SstackS)) = list_args_434X;
        SstackS = ((SstackS) + -4);
        merged_arg0K0 = 25;
        merged_arg0K1 = (1 + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        pop_args_GlistSAgc_return_tag = 0;
        goto pop_args_GlistSAgc;
       pop_args_GlistSAgc_return_0:
        args_437X = pop_args_GlistSAgc0_return_value;
        push_exception_continuationB(5, 0);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = args_437X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
      break;
    case 28 : {
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      stack_nargs_438X = (((*((long *) (SstackS))))>>2);
      proc_439X = *((long *) ((SstackS) + (4 + (((stack_nargs_438X)<<2)))));
      *((long *) ((SstackS) + (4 + (((stack_nargs_438X)<<2))))) = 1;
      SstackS = ((SstackS) + 4);
      rest_list_440X = *((long *) (SstackS));
      if ((25 == rest_list_440X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = (-2 + stack_nargs_438X);
        goto L14261;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_440X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_440X)))));
          arg0K1 = (-1 + stack_nargs_438X);
          goto L14261;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_440X))) + 4)))))) + 4)));
          arg0K1 = rest_list_440X;
          goto L14302;}}}
      break;
    case 29 : {
      SstackS = ((SstackS) + 4);
      cont_441X = *((long *) (SstackS));
      if ((3 == (3 & cont_441X))) {
        if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_441X))) + -4))))>>2))))) {
          merged_arg0K0 = cont_441X;
          merged_arg0K1 = 0;
          copy_continuation_from_heapB_return_tag = 0;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_0:
          goto L22223;}
        else {
          goto L22230;}}
      else {
        goto L22230;}}
      break;
    case 30 : {
      goto L24319;}
      break;
    case 31 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L45637;}
      break;
    case 32 : {
      SstackS = ((SstackS) + 4);
      p_442X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_443X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      arg0K0 = stack_nargs_443X;
      arg0K1 = (*((long *) (SstackS)));
      arg0K2 = ((((p_442X)>>2)) - stack_nargs_443X);
      goto L45637;}
      break;
    case 33 : {
      tem_444X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      StemplateS = tem_444X;
      Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_444X))))))));
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 34 : {
      code_445X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))))))));
      if ((0 == (*((unsigned char *) ((((char *) (-3 + code_445X))) + 1))))) {
        if (((SstackS) < (Sstack_limitS))) {
          s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
          merged_arg0K0 = 0;
          copy_stack_into_heap_return_tag = 1;
          goto copy_stack_into_heap;
         copy_stack_into_heap_return_1:
          if (((SstackS) < (Sstack_limitS))) {
            ps_error("Couldn't get default procedure space (how can this happen?)", 0);
            goto L23449;}
          else {
            goto L23449;}}
        else {
          goto L23449;}}
      else {
        v_446X = *((unsigned char *) ((((char *) (-3 + code_445X))) + 1));
        if ((67 == v_446X)) {
          if ((0 == (*((unsigned char *) ((((char *) (-3 + code_445X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_445X))) + -4))))>>8)))))))) {
            index_447X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_445X))) + -4))))>>8));
            space_448X = ((((*((unsigned char *) ((((char *) (-3 + code_445X))) + index_447X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_445X))) + (1 + index_447X))));
            if ((space_448X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
              goto L23449;}
            else {
              s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
              merged_arg0K0 = 0;
              copy_stack_into_heap_return_tag = 2;
              goto copy_stack_into_heap;
             copy_stack_into_heap_return_2:
              if ((space_448X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
                goto L23449;}
              else {
                ps_error("VM's stack is too small (how can this happen?)", 0);
                goto L23449;}}}
          else {
            goto L23492;}}
        else {
          goto L23492;}}}
      break;
    case 35 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L23052;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23052;}}
      break;
    case 36 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 37 : {
      if ((0 == (3 & (SvalS)))) {
        p_449X = SvalS;
        max_450X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_451X = ((p_449X)>>2);
        if ((val_451X < 0)) {
          goto L23350;}
        else {
          if ((val_451X < max_450X)) {
            index_452X = 1 + (((val_451X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_452X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_452X)))));
            goto L23352;}
          else {
            goto L23350;}}}
      else {
        push_exception_continuationB(5, 0);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21148;}}
      break;
    case 38 : {
      stack_nargs_453X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((0 == stack_nargs_453X)) {
        arg0_454X = *((long *) ((SstackS) + 12));
        rest_list_455X = *((long *) ((SstackS) + 8));
        *((long *) (SstackS)) = arg0_454X;
        SstackS = ((SstackS) + -4);
        SvalS = (*((long *) (((char *) (-3 + rest_list_455X)))));
        goto L24235;}
      else {
        arg1_456X = *((long *) ((SstackS) + (4 + (((stack_nargs_453X)<<2)))));
        arg0_457X = *((long *) ((SstackS) + (8 + (((stack_nargs_453X)<<2)))));
        *((long *) ((SstackS) + (8 + (((stack_nargs_453X)<<2))))) = 1;
        *((long *) ((SstackS) + 4)) = (-4 + (((stack_nargs_453X)<<2)));
        *((long *) (SstackS)) = arg0_457X;
        SstackS = ((SstackS) + -4);
        SvalS = arg1_456X;
        goto L24235;}}
      break;
    case 39 : {
      stack_nargs_458X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((stack_nargs_458X == 0)) {
        rest_list_459X = *((long *) ((SstackS) + 8));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_459X))) + 4))))) {
          arg0K0 = 1;
          goto L23248;}
        else {
          *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_459X))) + 4)));
          *((long *) ((SstackS) + 12)) = (SvalS);
          arg0K0 = -2;
          goto L23248;}}
      else {
        if ((stack_nargs_458X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 8))))) {
            arg0K0 = 1;
            goto L23248;}
          else {
            *((long *) ((SstackS) + 4)) = 0;
            *((long *) ((SstackS) + 12)) = (SvalS);
            arg0K0 = -2;
            goto L23248;}}
        else {
          *((long *) ((SstackS) + (8 + (((stack_nargs_458X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L23248;}}}
      break;
    case 40 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23052;}
      else {
        x_460X = (((*((long *) ((SstackS) + 4))))>>2);
        if ((x_460X == 0)) {
          rest_list_461X = *((long *) ((SstackS) + 8));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_461X))) + 4))))) {
            arg0K0 = 1;
            goto L24126;}
          else {
            *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_461X))) + 4)));
            *((long *) ((SstackS) + 12)) = (*((long *) (((char *) (-3 + rest_list_461X)))));
            arg0K0 = -2;
            goto L24126;}}
        else {
          if ((x_460X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 8))))) {
              arg0K0 = 1;
              goto L24126;}
            else {
              *((long *) ((SstackS) + 4)) = 0;
              arg0K0 = -2;
              goto L24126;}}
          else {
            arg0K0 = -2;
            goto L24126;}}}}
      break;
    case 41 : {
      SstackS = ((SstackS) + 4);
      arg2_462X = *((long *) (SstackS));
      x_463X = SvalS;
      if ((arg2_462X == x_463X)) {
        arg0K0 = 5;
        goto L46226;}
      else {
        arg0K0 = 1;
        goto L46226;}}
      break;
    case 42 : {
      x_464X = SvalS;
      if ((0 == (3 & x_464X))) {
        arg0K0 = 5;
        goto L46238;}
      else {
        if ((3 == (3 & x_464X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L46238;}
          else {
            goto L12946;}}
        else {
          goto L12946;}}}
      break;
    case 43 : {
      n_465X = SvalS;
      if ((0 == (3 & n_465X))) {
        goto L37919;}
      else {
        if ((3 == (3 & n_465X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_465X))) + -4))))>>2))))) {
            goto L37919;}
          else {
            goto L37920;}}
        else {
          goto L37920;}}}
      break;
    case 44 : {
      n_466X = SvalS;
      if ((0 == (3 & n_466X))) {
        goto L38076;}
      else {
        if ((3 == (3 & n_466X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
            goto L38076;}
          else {
            goto L38051;}}
        else {
          goto L38051;}}}
      break;
    case 45 : {
      n_467X = SvalS;
      if ((0 == (3 & n_467X))) {
        goto L38275;}
      else {
        if ((3 == (3 & n_467X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
            goto L38275;}
          else {
            goto L38250;}}
        else {
          goto L38250;}}}
      break;
    case 46 : {
      n_468X = SvalS;
      if ((0 == (3 & n_468X))) {
        goto L38474;}
      else {
        if ((3 == (3 & n_468X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
            goto L38474;}
          else {
            goto L38449;}}
        else {
          goto L38449;}}}
      break;
    case 47 : {
      n_469X = SvalS;
      if ((0 == (3 & n_469X))) {
        goto L31820;}
      else {
        if ((3 == (3 & n_469X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_469X))) + -4))))>>2))))) {
            goto L31820;}
          else {
            goto L31821;}}
        else {
          goto L31821;}}}
      break;
    case 48 : {
      x_470X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_470X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 49 : {
      x_471X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_471X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 50 : {
      SstackS = ((SstackS) + 4);
      x_472X = *((long *) (SstackS));
      y_473X = SvalS;
      if ((0 == (3 & (x_472X | y_473X)))) {
        s48_make_availableAgc(16);
        x_474X = (((x_472X)>>2)) + (((y_473X)>>2));
        if ((536870911 < x_474X)) {
          goto L31932;}
        else {
          if ((x_474X < -536870912)) {
            goto L31932;}
          else {
            arg0K0 = (((x_474X)<<2));
            goto L31892;}}}
      else {
        if ((0 == (3 & x_472X))) {
          goto L31898;}
        else {
          if ((3 == (3 & x_472X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_472X))) + -4))))>>2))))) {
              goto L31898;}
            else {
              goto L31907;}}
          else {
            goto L31907;}}}}
      break;
    case 51 : {
      SstackS = ((SstackS) + 4);
      x_475X = *((long *) (SstackS));
      y_476X = SvalS;
      if ((0 == (3 & (x_475X | y_476X)))) {
        a_477X = ((x_475X)>>2);
        b_478X = ((y_476X)>>2);
        if ((a_477X < 0)) {
          arg0K0 = (0 - a_477X);
          goto L7805;}
        else {
          arg0K0 = a_477X;
          goto L7805;}}
      else {
        if ((0 == (3 & x_475X))) {
          goto L40221;}
        else {
          if ((3 == (3 & x_475X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_475X))) + -4))))>>2))))) {
              goto L40221;}
            else {
              goto L40230;}}
          else {
            goto L40230;}}}}
      break;
    case 52 : {
      SstackS = ((SstackS) + 4);
      x_479X = *((long *) (SstackS));
      y_480X = SvalS;
      if ((0 == (3 & (x_479X | y_480X)))) {
        s48_make_availableAgc(16);
        x_481X = (((x_479X)>>2)) - (((y_480X)>>2));
        if ((536870911 < x_481X)) {
          goto L32123;}
        else {
          if ((x_481X < -536870912)) {
            goto L32123;}
          else {
            arg0K0 = (((x_481X)<<2));
            goto L32083;}}}
      else {
        if ((0 == (3 & x_479X))) {
          goto L32089;}
        else {
          if ((3 == (3 & x_479X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_479X))) + -4))))>>2))))) {
              goto L32089;}
            else {
              goto L32098;}}
          else {
            goto L32098;}}}}
      break;
    case 53 : {
      SstackS = ((SstackS) + 4);
      x_482X = *((long *) (SstackS));
      y_483X = SvalS;
      if ((0 == y_483X)) {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_482X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_483X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}
      else {
        if ((0 == (3 & (x_482X | y_483X)))) {
          if ((0 == y_483X)) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = x_482X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_483X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21148;}
          else {
            a_484X = ((x_482X)>>2);
            b_485X = ((y_483X)>>2);
            if ((a_484X < 0)) {
              arg0K0 = (0 - a_484X);
              goto L8063;}
            else {
              arg0K0 = a_484X;
              goto L8063;}}}
        else {
          if ((0 == (3 & x_482X))) {
            goto L40376;}
          else {
            if ((3 == (3 & x_482X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_482X))) + -4))))>>2))))) {
                goto L40376;}
              else {
                goto L40409;}}
            else {
              goto L40409;}}}}}
      break;
    case 54 : {
      SstackS = ((SstackS) + 4);
      x_486X = *((long *) (SstackS));
      y_487X = SvalS;
      if ((0 == (3 & (x_486X | y_487X)))) {
        if ((x_486X == y_487X)) {
          arg0K0 = 5;
          goto L32270;}
        else {
          arg0K0 = 1;
          goto L32270;}}
      else {
        if ((0 == (3 & x_486X))) {
          goto L32276;}
        else {
          if ((3 == (3 & x_486X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_486X))) + -4))))>>2))))) {
              goto L32276;}
            else {
              goto L32287;}}
          else {
            goto L32287;}}}}
      break;
    case 55 : {
      SstackS = ((SstackS) + 4);
      x_488X = *((long *) (SstackS));
      y_489X = SvalS;
      if ((0 == (3 & (x_488X | y_489X)))) {
        if ((x_488X < y_489X)) {
          arg0K0 = 5;
          goto L32447;}
        else {
          arg0K0 = 1;
          goto L32447;}}
      else {
        if ((0 == (3 & x_488X))) {
          goto L32453;}
        else {
          if ((3 == (3 & x_488X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_488X))) + -4))))>>2))))) {
              goto L32453;}
            else {
              goto L32464;}}
          else {
            goto L32464;}}}}
      break;
    case 56 : {
      SstackS = ((SstackS) + 4);
      x_490X = *((long *) (SstackS));
      y_491X = SvalS;
      if ((0 == (3 & (x_490X | y_491X)))) {
        if ((y_491X < x_490X)) {
          arg0K0 = 5;
          goto L32687;}
        else {
          arg0K0 = 1;
          goto L32687;}}
      else {
        if ((0 == (3 & x_490X))) {
          goto L32693;}
        else {
          if ((3 == (3 & x_490X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_490X))) + -4))))>>2))))) {
              goto L32693;}
            else {
              goto L32704;}}
          else {
            goto L32704;}}}}
      break;
    case 57 : {
      SstackS = ((SstackS) + 4);
      x_492X = *((long *) (SstackS));
      y_493X = SvalS;
      if ((0 == (3 & (x_492X | y_493X)))) {
        if ((y_493X < x_492X)) {
          arg0K0 = 1;
          goto L32927;}
        else {
          arg0K0 = 5;
          goto L32927;}}
      else {
        if ((0 == (3 & x_492X))) {
          goto L32933;}
        else {
          if ((3 == (3 & x_492X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_492X))) + -4))))>>2))))) {
              goto L32933;}
            else {
              goto L32944;}}
          else {
            goto L32944;}}}}
      break;
    case 58 : {
      SstackS = ((SstackS) + 4);
      x_494X = *((long *) (SstackS));
      y_495X = SvalS;
      if ((0 == (3 & (x_494X | y_495X)))) {
        if ((x_494X < y_495X)) {
          arg0K0 = 1;
          goto L33121;}
        else {
          arg0K0 = 5;
          goto L33121;}}
      else {
        if ((0 == (3 & x_494X))) {
          goto L33127;}
        else {
          if ((3 == (3 & x_494X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_494X))) + -4))))>>2))))) {
              goto L33127;}
            else {
              goto L33138;}}
          else {
            goto L33138;}}}}
      break;
    case 59 : {
      SstackS = ((SstackS) + 4);
      x_496X = *((long *) (SstackS));
      y_497X = SvalS;
      if ((0 == y_497X)) {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_496X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_497X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}
      else {
        if ((0 == (3 & (x_496X | y_497X)))) {
          if ((0 == y_497X)) {
            arg0K0 = x_496X;
            arg0K1 = y_497X;
            goto L18413;}
          else {
            a_498X = ((x_496X)>>2);
            b_499X = ((y_497X)>>2);
            if ((a_498X < 0)) {
              arg0K0 = (0 - a_498X);
              goto L8708;}
            else {
              arg0K0 = a_498X;
              goto L8708;}}}
        else {
          if ((0 == (3 & x_496X))) {
            goto L33327;}
          else {
            if ((3 == (3 & x_496X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_496X))) + -4))))>>2))))) {
                goto L33327;}
              else {
                goto L33336;}}
            else {
              goto L33336;}}}}}
      break;
    case 60 : {
      SstackS = ((SstackS) + 4);
      x_500X = *((long *) (SstackS));
      y_501X = SvalS;
      if ((0 == y_501X)) {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = x_500X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_501X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}
      else {
        if ((0 == (3 & (x_500X | y_501X)))) {
          if ((0 == y_501X)) {
            arg0K0 = x_500X;
            arg0K1 = y_501X;
            goto L18322;}
          else {
            a_502X = ((x_500X)>>2);
            if ((a_502X < 0)) {
              arg0K0 = (0 - a_502X);
              goto L33541;}
            else {
              arg0K0 = a_502X;
              goto L33541;}}}
        else {
          if ((0 == (3 & x_500X))) {
            goto L33503;}
          else {
            if ((3 == (3 & x_500X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_500X))) + -4))))>>2))))) {
                goto L33503;}
              else {
                goto L33512;}}
            else {
              goto L33512;}}}}}
      break;
    case 61 : {
      n_503X = SvalS;
      if ((0 == (3 & n_503X))) {
        goto L33690;}
      else {
        if ((3 == (3 & n_503X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_503X))) + -4))))>>2))))) {
            goto L33690;}
          else {
            goto L33691;}}
        else {
          goto L33691;}}}
      break;
    case 62 : {
      n_504X = SvalS;
      if ((0 == (3 & n_504X))) {
        goto L33745;}
      else {
        if ((3 == (3 & n_504X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_504X))) + -4))))>>2))))) {
            goto L33745;}
          else {
            goto L33746;}}
        else {
          goto L33746;}}}
      break;
    case 63 : {
      n_505X = SvalS;
      if ((0 == (3 & n_505X))) {
        goto L33800;}
      else {
        if ((3 == (3 & n_505X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_505X))) + -4))))>>2))))) {
            goto L33800;}
          else {
            goto L33803;}}
        else {
          goto L33803;}}}
      break;
    case 64 : {
      n_506X = SvalS;
      if ((0 == (3 & n_506X))) {
        goto L33860;}
      else {
        if ((3 == (3 & n_506X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_506X))) + -4))))>>2))))) {
            goto L33860;}
          else {
            goto L33861;}}
        else {
          goto L33861;}}}
      break;
    case 65 : {
      n_507X = SvalS;
      if ((0 == (3 & n_507X))) {
        goto L33915;}
      else {
        if ((3 == (3 & n_507X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_507X))) + -4))))>>2))))) {
            goto L33915;}
          else {
            goto L33918;}}
        else {
          goto L33918;}}}
      break;
    case 66 : {
      x_508X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_508X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 67 : {
      x_509X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_509X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 68 : {
      x_510X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_510X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 69 : {
      x_511X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_511X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 70 : {
      x_512X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_512X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 71 : {
      x_513X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_513X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 72 : {
      x_514X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_514X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 73 : {
      x_515X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_515X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 74 : {
      x_516X = SvalS;
      if ((0 == (3 & x_516X))) {
        if ((x_516X < 0)) {
          goto L46392;}
        else {
          goto L46398;}}
      else {
        v_517X = s48_bignum_test((((char *) (-3 + x_516X))));
        if ((-1 == v_517X)) {
          goto L46392;}
        else {
          goto L46398;}}}
      break;
    case 75 : {
      x_518X = SvalS;
      if ((0 == (3 & x_518X))) {
        goto L21899;}
      else {
        if ((3 == (3 & x_518X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_518X))) + -4))))>>2))))) {
            goto L21899;}
          else {
            goto L21902;}}
        else {
          goto L21902;}}}
      break;
    case 76 : {
      SstackS = ((SstackS) + 4);
      arg2_519X = *((long *) (SstackS));
      x_520X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_519X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_520X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}
      break;
    case 77 : {
      SstackS = ((SstackS) + 4);
      arg2_521X = *((long *) (SstackS));
      x_522X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_521X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_522X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}
      break;
    case 78 : {
      SstackS = ((SstackS) + 4);
      arg2_523X = *((long *) (SstackS));
      x_524X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_523X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_524X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}
      break;
    case 79 : {
      x_525X = SvalS;
      if ((0 == (3 & x_525X))) {
        SvalS = (~ (3 | x_525X));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23052;}
      else {
        if ((0 == (3 & x_525X))) {
          goto L34069;}
        else {
          if ((3 == (3 & x_525X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_525X))) + -4))))>>2))))) {
              goto L34069;}
            else {
              goto L34072;}}
          else {
            goto L34072;}}}}
      break;
    case 80 : {
      x_526X = SvalS;
      if ((0 == (3 & x_526X))) {
        x_527X = ((x_526X)>>2);
        if ((x_527X < 0)) {
          arg0K0 = (~ x_527X);
          goto L34160;}
        else {
          arg0K0 = x_527X;
          goto L34160;}}
      else {
        if ((0 == (3 & x_526X))) {
          goto L34143;}
        else {
          if ((3 == (3 & x_526X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_526X))) + -4))))>>2))))) {
              goto L34143;}
            else {
              goto L34146;}}
          else {
            goto L34146;}}}}
      break;
    case 81 : {
      SstackS = ((SstackS) + 4);
      x_528X = *((long *) (SstackS));
      y_529X = SvalS;
      if ((0 == (3 & (x_528X | y_529X)))) {
        SvalS = (x_528X & y_529X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23052;}
      else {
        if ((0 == (3 & x_528X))) {
          goto L34245;}
        else {
          if ((3 == (3 & x_528X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_528X))) + -4))))>>2))))) {
              goto L34245;}
            else {
              goto L34254;}}
          else {
            goto L34254;}}}}
      break;
    case 82 : {
      SstackS = ((SstackS) + 4);
      x_530X = *((long *) (SstackS));
      y_531X = SvalS;
      if ((0 == (3 & (x_530X | y_531X)))) {
        SvalS = (x_530X | y_531X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23052;}
      else {
        if ((0 == (3 & x_530X))) {
          goto L34408;}
        else {
          if ((3 == (3 & x_530X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_530X))) + -4))))>>2))))) {
              goto L34408;}
            else {
              goto L34417;}}
          else {
            goto L34417;}}}}
      break;
    case 83 : {
      SstackS = ((SstackS) + 4);
      x_532X = *((long *) (SstackS));
      y_533X = SvalS;
      if ((0 == (3 & (x_532X | y_533X)))) {
        SvalS = (x_532X ^ y_533X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23052;}
      else {
        if ((0 == (3 & x_532X))) {
          goto L34571;}
        else {
          if ((3 == (3 & x_532X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_532X))) + -4))))>>2))))) {
              goto L34571;}
            else {
              goto L34580;}}
          else {
            goto L34580;}}}}
      break;
    case 84 : {
      SstackS = ((SstackS) + 4);
      x_534X = *((long *) (SstackS));
      y_535X = SvalS;
      if ((3 == (3 & y_535X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_535X))) + -4))))>>2))))) {
          v_536X = s48_bignum_test((((char *) (-3 + y_535X))));
          if ((1 == v_536X)) {
            push_exception_continuationB(6, 1);
            *((long *) (SstackS)) = x_534X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_535X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21148;}
          else {
            if ((0 == (3 & x_534X))) {
              if ((x_534X < 0)) {
                arg0K0 = -4;
                goto L31600;}
              else {
                arg0K0 = 0;
                goto L31600;}}
            else {
              if ((3 == (3 & x_534X))) {
                if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_534X))) + -4))))>>2))))) {
                  v_537X = s48_bignum_test((((char *) (-3 + x_534X))));
                  if ((1 == v_537X)) {
                    arg0K0 = 0;
                    goto L31616;}
                  else {
                    arg0K0 = -4;
                    goto L31616;}}
                else {
                  goto L31617;}}
              else {
                goto L31617;}}}}
        else {
          goto L40609;}}
      else {
        goto L40609;}}
      break;
    case 85 : {
      x_538X = SvalS;
      if ((9 == (255 & x_538X))) {
        arg0K0 = 5;
        goto L46480;}
      else {
        arg0K0 = 1;
        goto L46480;}}
      break;
    case 86 : {
      SstackS = ((SstackS) + 4);
      arg2_539X = *((long *) (SstackS));
      if ((9 == (255 & arg2_539X))) {
        if ((9 == (255 & (SvalS)))) {
          x_540X = SvalS;
          if ((arg2_539X == x_540X)) {
            arg0K0 = 5;
            goto L37694;}
          else {
            arg0K0 = 1;
            goto L37694;}}
        else {
          goto L37667;}}
      else {
        goto L37667;}}
      break;
    case 87 : {
      SstackS = ((SstackS) + 4);
      arg2_541X = *((long *) (SstackS));
      if ((9 == (255 & arg2_541X))) {
        if ((9 == (255 & (SvalS)))) {
          x_542X = SvalS;
          if ((arg2_541X < x_542X)) {
            arg0K0 = 5;
            goto L37604;}
          else {
            arg0K0 = 1;
            goto L37604;}}
        else {
          goto L37577;}}
      else {
        goto L37577;}}
      break;
    case 88 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23052;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21148;}}
      break;
    case 89 : {
      if ((0 == (3 & (SvalS)))) {
        x_543X = (((SvalS))>>2);
        if ((255 < x_543X)) {
          goto L41799;}
        else {
          if ((x_543X < 0)) {
            goto L41799;}
          else {
            SvalS = (9 + (((((x_543X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23052;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21148;}}
      break;
    case 90 : {
      x_544X = SvalS;
      if ((21 == x_544X)) {
        arg0K0 = 5;
        goto L46509;}
      else {
        arg0K0 = 1;
        goto L46509;}}
      break;
    case 91 : {
      x_545X = SvalS;
      type_546X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_545X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_545X))) + -4))))>>2))) == type_546X)) {
          arg0K0 = 5;
          goto L46523;}
        else {
          arg0K0 = 1;
          goto L46523;}}
      else {
        arg0K0 = 1;
        goto L46523;}}
      break;
    case 92 : {
      stob_547X = SvalS;
      type_548X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_547X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_547X))) + -4))))>>2))) == type_548X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_547X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L23052;}
        else {
          goto L26219;}}
      else {
        goto L26219;}}
      break;
    case 93 : {
      len_549X = *((unsigned char *) ((Scode_pointerS) + 1));
      s48_make_availableAgc((4 + (((len_549X)<<2))));
      type_550X = *((unsigned char *) ((Scode_pointerS) + 2));
      len_551X = ((len_549X)<<2);
      addr_552X = s48_allocate_small((4 + len_551X));
      *((long *) addr_552X) = (2 + (((((((len_551X)<<6)) + type_550X))<<2)));
      new_553X = 3 + (((long) (addr_552X + 4)));
      if ((len_549X < 1)) {
        goto L26324;}
      else {
        *((long *) ((((char *) (-3 + new_553X))) + (-4 + (((len_549X)<<2))))) = (SvalS);
        arg0K0 = (-2 + len_549X);
        goto L26308;}}
      break;
    case 94 : {
      SstackS = ((SstackS) + 4);
      len_554X = (((*((long *) (SstackS))))>>2);
      s48_make_availableAgc((4 + (((len_554X)<<2))));
      type_555X = *((unsigned char *) ((Scode_pointerS) + 1));
      len_556X = ((len_554X)<<2);
      addr_557X = s48_allocate_small((4 + len_556X));
      *((long *) addr_557X) = (2 + (((((((len_556X)<<6)) + type_555X))<<2)));
      new_558X = 3 + (((long) (addr_557X + 4)));
      SstackS = ((SstackS) + 4);
      stack_nargs_559X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      rest_list_560X = *((long *) (SstackS));
      arg0K0 = (-1 + stack_nargs_559X);
      goto L26440;}
      break;
    case 95 : {
      stob_561X = SvalS;
      offset_562X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_563X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_561X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_561X))) + -4))))>>2))) == type_563X)) {
          SvalS = (*((long *) ((((char *) (-3 + stob_561X))) + (((offset_562X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L23052;}
        else {
          goto L26600;}}
      else {
        goto L26600;}}
      break;
    case 96 : {
      SstackS = ((SstackS) + 4);
      stob_564X = *((long *) (SstackS));
      value_565X = SvalS;
      offset_566X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_567X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_564X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_564X))) + -4))))>>2))) == type_567X)) {
          if ((3 == (3 & stob_564X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_564X))) + -4)))))) {
              v_568X = *((unsigned char *) ((Scode_pointerS) + 3));
              if ((0 == v_568X)) {
                goto L26717;}
              else {
                if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                  goto L26717;}
                else {
                  merged_arg0K0 = stob_564X;
                  merged_arg0K1 = (((offset_566X)<<2));
                  merged_arg0K2 = value_565X;
                  proposal_d_write_return_tag = 0;
                  goto proposal_d_write;
                 proposal_d_write_return_0:
                  goto L26726;}}}
            else {
              goto L26727;}}
          else {
            goto L26727;}}
        else {
          goto L26727;}}
      else {
        goto L26727;}}
      break;
    case 97 : {
      SstackS = ((SstackS) + 4);
      len_569X = *((long *) (SstackS));
      init_570X = SvalS;
      type_571X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & len_569X))) {
        len_572X = ((len_569X)>>2);
        if ((len_572X < 0)) {
          goto L26944;}
        else {
          if ((4194304 < len_572X)) {
            goto L26944;}
          else {
            Stemp0S = init_570X;
            len_573X = ((len_572X)<<2);
            addr_574X = s48_allocate_tracedAgc((4 + len_573X));
            if ((addr_574X == NULL)) {
              arg0K0 = 1;
              goto L26961;}
            else {
              *((long *) addr_574X) = (2 + (((((((len_573X)<<6)) + type_571X))<<2)));
              arg0K0 = (3 + (((long) (addr_574X + 4))));
              goto L26961;}}}}
      else {
        push_exception_continuationB(5, 2);
        *((long *) (SstackS)) = (((type_571X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = len_569X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = init_570X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L21148;}}
      break;
    case 98 : {
      SstackS = ((SstackS) + 4);
      stob_575X = *((long *) (SstackS));
      index_576X = SvalS;
      type_577X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & index_576X))) {
        if ((3 == (3 & stob_575X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + stob_575X))) + -4))))>>2))) == type_577X)) {
            len_578X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_575X))) + -4))))>>8))))>>2);
            index_579X = ((index_576X)>>2);
            if ((index_579X < 0)) {
              goto L27234;}
            else {
              if ((index_579X < len_578X)) {
                v_580X = *((unsigned char *) ((Scode_pointerS) + 2));
                if ((0 == v_580X)) {
                  goto L27224;}
                else {
                  if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                    goto L27224;}
                  else {
                    merged_arg0K0 = stob_575X;
                    merged_arg0K1 = index_576X;
                    proposal_d_read_return_tag = 0;
                    goto proposal_d_read;
                   proposal_d_read_return_0:
                    v_581X = proposal_d_read0_return_value;
                    arg0K0 = v_581X;
                    goto L27233;}}}
              else {
                goto L27234;}}}
          else {
            goto L27189;}}
        else {
          goto L27189;}}
      else {
        goto L27189;}}
      break;
    case 99 : {
      SstackS = ((SstackS) + 4);
      arg2_582X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stob_583X = *((long *) (SstackS));
      value_584X = SvalS;
      type_585X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_582X))) {
        if ((3 == (3 & stob_583X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + stob_583X))) + -4))))>>2))) == type_585X)) {
            if ((3 == (3 & stob_583X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_583X))) + -4)))))) {
                len_586X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_583X))) + -4))))>>8))))>>2);
                index_587X = ((arg2_582X)>>2);
                if ((index_587X < 0)) {
                  goto L27521;}
                else {
                  if ((index_587X < len_586X)) {
                    v_588X = *((unsigned char *) ((Scode_pointerS) + 2));
                    if ((0 == v_588X)) {
                      goto L27511;}
                    else {
                      if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                        goto L27511;}
                      else {
                        merged_arg0K0 = stob_583X;
                        merged_arg0K1 = arg2_582X;
                        merged_arg0K2 = value_584X;
                        proposal_d_write_return_tag = 1;
                        goto proposal_d_write;
                       proposal_d_write_return_1:
                        goto L27520;}}}
                  else {
                    goto L27521;}}}
              else {
                goto L27474;}}
            else {
              goto L27474;}}
          else {
            goto L27474;}}
        else {
          goto L27474;}}
      else {
        goto L27474;}}
      break;
    case 100 : {
      SstackS = ((SstackS) + 4);
      arg2_589X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_589X | (SvalS))))) {
        len_590X = ((arg2_589X)>>2);
        init_591X = (((SvalS))>>2);
        if ((len_590X < 0)) {
          goto L34792;}
        else {
          if ((4194304 < ((((3 + len_590X))>>2)))) {
            goto L34792;}
          else {
            addr_592X = s48_allocate_untracedAgc((4 + len_590X));
            if ((addr_592X == NULL)) {
              arg0K0 = 1;
              goto L34805;}
            else {
              *((long *) addr_592X) = (74 + (((len_590X)<<8)));
              arg0K0 = (3 + (((long) (addr_592X + 4))));
              goto L34805;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_589X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
      break;
    case 101 : {
      obj_593X = SvalS;
      if ((3 == (3 & obj_593X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + obj_593X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23052;}
        else {
          goto L45304;}}
      else {
        goto L45304;}}
      break;
    case 102 : {
      SstackS = ((SstackS) + 4);
      arg2_594X = *((long *) (SstackS));
      if ((3 == (3 & arg2_594X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_594X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_595X = (((SvalS))>>2);
            len_596X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_594X))) + -4))))>>8);
            if ((index_595X < 0)) {
              goto L41905;}
            else {
              if ((index_595X < len_596X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_594X))) + index_595X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23052;}
              else {
                goto L41905;}}}
          else {
            goto L44820;}}
        else {
          goto L44820;}}
      else {
        goto L44820;}}
      break;
    case 103 : {
      SstackS = ((SstackS) + 4);
      arg2_597X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_598X = *((long *) (SstackS));
      if ((3 == (3 & arg3_598X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_598X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_597X | (SvalS))))) {
            index_599X = ((arg2_597X)>>2);
            Kchar_600X = (((SvalS))>>2);
            if ((3 == (3 & arg3_598X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_598X))) + -4)))))) {
                len_601X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_598X))) + -4))))>>8);
                if ((index_599X < 0)) {
                  goto L39443;}
                else {
                  if ((index_599X < len_601X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_598X))) + index_599X)) = Kchar_600X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L23052;}
                  else {
                    goto L39443;}}}
              else {
                goto L39422;}}
            else {
              goto L39422;}}
          else {
            goto L43600;}}
        else {
          goto L43600;}}
      else {
        goto L43600;}}
      break;
    case 104 : {
      SstackS = ((SstackS) + 4);
      arg2_602X = *((long *) (SstackS));
      if ((0 == (3 & arg2_602X))) {
        if ((9 == (255 & (SvalS)))) {
          len_603X = ((arg2_602X)>>2);
          init_604X = ((((SvalS))>>8));
          if ((len_603X < 0)) {
            goto L34972;}
          else {
            if ((4194304 < ((((4 + len_603X))>>2)))) {
              goto L34972;}
            else {
              len_605X = 1 + len_603X;
              addr_606X = s48_allocate_untracedAgc((4 + len_605X));
              if ((addr_606X == NULL)) {
                arg0K0 = 1;
                goto L34985;}
              else {
                *((long *) addr_606X) = (70 + (((len_605X)<<8)));
                arg0K0 = (3 + (((long) (addr_606X + 4))));
                goto L34985;}}}}
        else {
          goto L41986;}}
      else {
        goto L41986;}}
      break;
    case 105 : {
      obj_607X = SvalS;
      if ((3 == (3 & obj_607X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_607X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23052;}
        else {
          goto L45354;}}
      else {
        goto L45354;}}
      break;
    case 106 : {
      SstackS = ((SstackS) + 4);
      arg2_608X = *((long *) (SstackS));
      if ((3 == (3 & arg2_608X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_608X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_609X = (((SvalS))>>2);
            len_610X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_608X))) + -4))))>>8));
            if ((index_609X < 0)) {
              goto L42049;}
            else {
              if ((index_609X < len_610X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_608X))) + index_609X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23052;}
              else {
                goto L42049;}}}
          else {
            goto L44916;}}
        else {
          goto L44916;}}
      else {
        goto L44916;}}
      break;
    case 107 : {
      SstackS = ((SstackS) + 4);
      arg2_611X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_612X = *((long *) (SstackS));
      if ((3 == (3 & arg3_612X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_612X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_611X))) {
            if ((9 == (255 & (SvalS)))) {
              index_613X = ((arg2_611X)>>2);
              Kchar_614X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_612X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_612X))) + -4)))))) {
                  len_615X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_612X))) + -4))))>>8));
                  if ((index_613X < 0)) {
                    goto L39598;}
                  else {
                    if ((index_613X < len_615X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_612X))) + index_613X)) = (Kchar_614X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L23052;}
                    else {
                      goto L39598;}}}
                else {
                  goto L39577;}}
              else {
                goto L39577;}}
            else {
              goto L43726;}}
          else {
            goto L43726;}}
        else {
          goto L43726;}}
      else {
        goto L43726;}}
      break;
    case 108 : {
      s48_make_availableAgc(12);
      obj_616X = SvalS;
      if ((3 == (3 & obj_616X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_616X))) + -4))))>>2))))) {
          table_617X = Sthe_symbol_tableS;
          string_618X = SvalS;
          n_619X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_618X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L19703;}
        else {
          goto L31762;}}
      else {
        goto L31762;}}
      break;
    case 109 : {
      obj_620X = SvalS;
      if ((3 == (3 & obj_620X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_620X))) + -4))))>>2))))) {
          x_621X = SvalS;
          descriptor_622X = *((long *) ((((char *) (-3 + x_621X))) + 4));
          if ((17 == (255 & descriptor_622X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_621X))) + 4))))) {
              arg0K0 = 5;
              goto L43290;}
            else {
              arg0K0 = 1;
              goto L43290;}}
          else {
            arg0K0 = 5;
            goto L43290;}}
        else {
          goto L43271;}}
      else {
        goto L43271;}}
      break;
    case 110 : {
      SstackS = ((SstackS) + 4);
      arg2_623X = *((long *) (SstackS));
      if ((3 == (3 & arg2_623X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_623X))) + -4))))>>2))))) {
          x_624X = SvalS;
          if ((1 == x_624X)) {
            goto L39185;}
          else {
            if ((5 == x_624X)) {
              goto L39185;}
            else {
              goto L39192;}}}
        else {
          goto L39192;}}
      else {
        goto L39192;}}
      break;
    case 111 : {
      x_625X = SvalS;
      if ((3 == (3 & x_625X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_625X))) + -4)))))) {
          arg0K0 = 1;
          goto L46595;}
        else {
          arg0K0 = 5;
          goto L46595;}}
      else {
        arg0K0 = 5;
        goto L46595;}}
      break;
    case 112 : {
      x_626X = SvalS;
      if ((3 == (3 & x_626X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_626X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_626X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_626X))) + -4))));
          goto L42131;}
        else {
          goto L42131;}}
      else {
        goto L42131;}}
      break;
    case 113 : {
      s48_make_availableAgc(288);
      SstackS = ((SstackS) + 4);
      arg2_627X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        mode_628X = (((SvalS))>>2);
        if ((1 == mode_628X)) {
          goto L35379;}
        else {
          if ((2 == mode_628X)) {
            goto L35379;}
          else {
            if ((3 == mode_628X)) {
              goto L35379;}
            else {
              if ((4 == mode_628X)) {
                goto L35379;}
              else {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = arg2_627X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = (((mode_628X)<<2));
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21148;}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_627X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
      break;
    case 114 : {
      s48_make_availableAgc(264);
      obj_629X = SvalS;
      if ((3 == (3 & obj_629X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_629X))) + -4))))>>2))))) {
          channel_630X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_630X))))))) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = channel_630X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21148;}
          else {
            status_631X = close_channelB(channel_630X);
            if ((status_631X == NO_ERRORS)) {
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23052;}
            else {
              push_exception_continuationB(24, 1);
              *((long *) (SstackS)) = channel_630X;
              SstackS = ((SstackS) + -4);
              merged_arg0K0 = status_631X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 0;
              goto get_error_string;
             get_error_string_return_0:
              x_632X = get_error_string0_return_value;
              *((long *) (SstackS)) = x_632X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21148;}}}
        else {
          goto L46629;}}
      else {
        goto L46629;}}
      break;
    case 115 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_633X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_634X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_635X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_636X = *((long *) (SstackS));
      if ((3 == (3 & arg5_636X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg5_636X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg3_634X | arg2_633X)))) {
            x_637X = SvalS;
            if ((1 == x_637X)) {
              goto L42256;}
            else {
              if ((5 == x_637X)) {
                goto L42256;}
              else {
                goto L42267;}}}
          else {
            goto L42267;}}
        else {
          goto L42267;}}
      else {
        goto L42267;}}
      break;
    case 116 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_638X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_639X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_640X = *((long *) (SstackS));
      if ((3 == (3 & arg4_640X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg4_640X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_638X | (SvalS))))) {
            start_641X = ((arg2_638X)>>2);
            count_642X = (((SvalS))>>2);
            v_643X = 8 == (*((long *) (((char *) (-3 + arg4_640X)))));
            if (v_643X) {
              if ((3 == (3 & arg3_639X))) {
                if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>2))))) {
                  goto L36018;}
                else {
                  goto L36193;}}
              else {
                goto L36193;}}
            else {
              goto L36072;}}
          else {
            goto L42462;}}
        else {
          goto L42462;}}
      else {
        goto L42462;}}
      break;
    case 117 : {
      s48_make_availableAgc(264);
      obj_644X = SvalS;
      if ((3 == (3 & obj_644X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_644X))) + -4))))>>2))))) {
          channel_645X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_645X))))))) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = channel_645X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21148;}
          else {
            readyP_646X = ps_check_fd(((((*((long *) ((((char *) (-3 + channel_645X))) + 8))))>>2)), (4 == (*((long *) (((char *) (-3 + channel_645X)))))), &status_647X);
            if ((status_647X == NO_ERRORS)) {
              if (readyP_646X) {
                arg0K0 = 5;
                goto L38776;}
              else {
                arg0K0 = 1;
                goto L38776;}}
            else {
              push_exception_continuationB(24, 1);
              *((long *) (SstackS)) = channel_645X;
              SstackS = ((SstackS) + -4);
              merged_arg0K0 = status_647X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 1;
              goto get_error_string;
             get_error_string_return_1:
              x_648X = get_error_string0_return_value;
              *((long *) (SstackS)) = x_648X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21148;}}}
        else {
          goto L46651;}}
      else {
        goto L46651;}}
      break;
    case 118 : {
      obj_649X = SvalS;
      if ((3 == (3 & obj_649X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_649X))) + -4))))>>2))))) {
          channel_650X = SvalS;
          head_651X = Spending_channels_headS;
          if ((1 == head_651X)) {
            addr_652X = (((char *) (-3 + channel_650X))) + 16;
            S48_WRITE_BARRIER(channel_650X, addr_652X, 1);
            *((long *) addr_652X) = 1;
            n_653X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_650X))) + 8))))>>2)));
            arg0K0 = (((n_653X)<<2));
            goto L42604;}
          else {
            if ((channel_650X == head_651X)) {
              channel_654X = Spending_channels_headS;
              next_655X = *((long *) ((((char *) (-3 + channel_654X))) + 12));
              Spending_channels_headS = next_655X;
              addr_656X = (((char *) (-3 + channel_654X))) + 12;
              S48_WRITE_BARRIER(channel_654X, addr_656X, 1);
              *((long *) addr_656X) = 1;
              if ((1 == next_655X)) {
                Spending_channels_tailS = 1;
                goto L10555;}
              else {
                goto L10555;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_651X))) + 12)));
              arg0K1 = head_651X;
              goto L10561;}}}
        else {
          goto L42587;}}
      else {
        goto L42587;}}
      break;
    case 119 : {
      s48_make_availableAgc(((((3 * (Snumber_of_channelsS)))<<2)));
      arg0K0 = (-1 + (Snumber_of_channelsS));
      arg0K1 = 25;
      goto L15191;}
      break;
    case 120 : {
      SvalS = (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 121 : {
      proposal_657X = SvalS;
      if ((1 == proposal_657X)) {
        goto L36326;}
      else {
        if ((3 == (3 & proposal_657X))) {
          if ((2 == (31 & ((((*((long *) ((((char *) (-3 + proposal_657X))) + -4))))>>2))))) {
            if ((4 == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proposal_657X))) + -4))))>>8))))>>2)))) {
              if ((1 == (*((long *) (((char *) (-3 + proposal_657X))))))) {
                goto L36326;}
              else {
                goto L36357;}}
            else {
              goto L36357;}}
          else {
            goto L36357;}}
        else {
          goto L36357;}}}
      break;
    case 122 : {
      proposal_658X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
      if ((1 == proposal_658X)) {
        push_exception_continuationB(26, 1);
        arg0K0 = 0;
        goto L21148;}
      else {
        GET_PROPOSAL_LOCK();
        log_659X = *((long *) ((((char *) (-3 + proposal_658X))) + 4));
        arg0K0 = 0;
        goto L8995;}}
      break;
    case 123 : {
      stob_660X = SvalS;
      offset_661X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_662X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_660X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_660X))) + -4))))>>2))) == type_662X)) {
          x_663X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
          if ((1 == x_663X)) {
            arg0K0 = (*((long *) ((((char *) (-3 + stob_660X))) + (((offset_661X)<<2)))));
            goto L27819;}
          else {
            merged_arg0K0 = stob_660X;
            merged_arg0K1 = (((offset_661X)<<2));
            proposal_d_read_return_tag = 1;
            goto proposal_d_read;
           proposal_d_read_return_1:
            v_664X = proposal_d_read0_return_value;
            arg0K0 = v_664X;
            goto L27819;}}
        else {
          goto L27820;}}
      else {
        goto L27820;}}
      break;
    case 124 : {
      SstackS = ((SstackS) + 4);
      arg2_665X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_666X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_667X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_668X = *((long *) (SstackS));
      if ((0 == (3 & ((arg4_667X | arg2_665X) | (SvalS))))) {
        from_index_669X = ((arg4_667X)>>2);
        to_index_670X = ((arg2_665X)>>2);
        count_671X = (((SvalS))>>2);
        v_672X = *((unsigned char *) ((Scode_pointerS) + 1));
        merged_arg0K0 = arg5_668X;
        merged_arg0K1 = from_index_669X;
        merged_arg0K2 = count_671X;
        okay_copy_argsP_return_tag = 0;
        goto okay_copy_argsP;
       okay_copy_argsP_return_0:
        v_673X = okay_copy_argsP0_return_value;
        if (v_673X) {
          merged_arg0K0 = arg3_666X;
          merged_arg0K1 = to_index_670X;
          merged_arg0K2 = count_671X;
          okay_copy_argsP_return_tag = 1;
          goto okay_copy_argsP;
         okay_copy_argsP_return_1:
          v_674X = okay_copy_argsP0_return_value;
          if (v_674X) {
            if ((3 == (3 & arg3_666X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_666X))) + -4)))))) {
                if ((count_671X < 0)) {
                  goto L27913;}
                else {
                  if ((0 == v_672X)) {
                    goto L27973;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L27973;}
                    else {
                      arg0K0 = 4096;
                      arg0K1 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 12)));
                      goto L28061;}}}}
              else {
                goto L27913;}}
            else {
              goto L27913;}}
          else {
            goto L27913;}}
        else {
          goto L27913;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg5_668X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_667X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_666X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_665X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 5;
        goto L21148;}}
      break;
    case 125 : {
      SstackS = ((SstackS) + 4);
      arg2_675X = *((long *) (SstackS));
      if ((3 == (3 & arg2_675X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_675X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_676X = (((SvalS))>>2);
            len_677X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_675X))) + -4))))>>8);
            if ((index_676X < 0)) {
              goto L28270;}
            else {
              if ((index_676X < len_677X)) {
                x_678X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                if ((1 == x_678X)) {
                  arg0K0 = ((((*((unsigned char *) ((((char *) (-3 + arg2_675X))) + index_676X))))<<2));
                  goto L28269;}
                else {
                  index_679X = ((index_676X)<<2);
                  log_680X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                  arg0K0 = 0;
                  goto L13756;}}
              else {
                goto L28270;}}}
          else {
            goto L31068;}}
        else {
          goto L31068;}}
      else {
        goto L31068;}}
      break;
    case 126 : {
      SstackS = ((SstackS) + 4);
      arg2_681X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_682X = *((long *) (SstackS));
      if ((3 == (3 & arg3_682X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_682X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_681X))) {
            index_683X = ((arg2_681X)>>2);
            byte_684X = SvalS;
            if ((3 == (3 & arg3_682X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_682X))) + -4)))))) {
                if ((0 == (3 & byte_684X))) {
                  len_685X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_682X))) + -4))))>>8);
                  if ((index_683X < 0)) {
                    goto L28396;}
                  else {
                    if ((index_683X < len_685X)) {
                      x_686X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                      if ((1 == x_686X)) {
                        *((unsigned char *) ((((char *) (-3 + arg3_682X))) + index_683X)) = (((byte_684X)>>2));
                        goto L28395;}
                      else {
                        index_687X = ((index_683X)<<2);
                        log_688X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                        arg0K0 = 0;
                        goto L13932;}}
                    else {
                      goto L28396;}}}
                else {
                  goto L28361;}}
              else {
                goto L28361;}}
            else {
              goto L28361;}}
          else {
            goto L31171;}}
        else {
          goto L31171;}}
      else {
        goto L31171;}}
      break;
    case 127 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 128 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 129 : {
      x_689X = SvalS;
      push_exception_continuationB(15, 1);
      *((long *) (SstackS)) = x_689X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
      break;
    case 130 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 131 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 132 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_690X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_691X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_692X = *((long *) (SstackS));
      if ((3 == (3 & arg4_692X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_692X))) + -4))))>>2))))) {
          if ((3 == (3 & arg2_690X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_690X))) + -4))))>>2))))) {
              obj_693X = SvalS;
              if ((3 == (3 & obj_693X))) {
                if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_693X))) + -4))))>>2))))) {
                  undumpables_694X = SvalS;
                  port_695X = ps_open_output_file((((char *)(((char *) (-3 + arg4_692X))))), &status_696X);
                  if ((status_696X == NO_ERRORS)) {
                    status_697X = ps_write_string((((char *)(((char *) (-3 + arg2_690X))))), port_695X);
                    if ((status_697X == NO_ERRORS)) {
                      status_698X = s48_write_image(arg3_691X, undumpables_694X, port_695X);
                      if ((status_698X == NO_ERRORS)) {
                        status_699X = ps_close(port_695X);
                        if ((status_699X == NO_ERRORS)) {
                          SvalS = 13;
                          Scode_pointerS = ((Scode_pointerS) + 1);
                          arg1K0 = (Scode_pointerS);
                          goto L23052;}
                        else {
                          arg0K0 = 24;
                          arg0K1 = status_699X;
                          goto L39826;}}
                      else {
                        status_700X = ps_close(port_695X);
                        if ((status_700X == NO_ERRORS)) {
                          arg0K0 = 24;
                          arg0K1 = status_698X;
                          goto L39826;}
                        else {
                          ps_write_string("Unable to close image file", (stderr));
                          { long ignoreXX;
                          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                          arg0K0 = 24;
                          arg0K1 = status_698X;
                          goto L39826;}}}
                    else {
                      status_701X = ps_close(port_695X);
                      if ((status_701X == NO_ERRORS)) {
                        arg0K0 = 24;
                        arg0K1 = status_697X;
                        goto L39826;}
                      else {
                        ps_write_string("Unable to close image file", (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg0K0 = 24;
                        arg0K1 = status_697X;
                        goto L39826;}}}
                  else {
                    arg0K0 = 10;
                    arg0K1 = status_696X;
                    goto L39826;}}
                else {
                  goto L43923;}}
              else {
                goto L43923;}}
            else {
              goto L43923;}}
          else {
            goto L43923;}}
        else {
          goto L43923;}}
      else {
        goto L43923;}}
      break;
    case 133 : {
      SvalS = 13;
      s48_collect();
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 134 : {
      obj_702X = SvalS;
      if ((3 == (3 & obj_702X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_702X))) + -4))))>>2))))) {
          x_703X = SvalS;
          n_704X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_703X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L44564;}
        else {
          goto L44541;}}
      else {
        goto L44541;}}
      break;
    case 135 : {
      s48_make_availableAgc(24);
      SstackS = ((SstackS) + 4);
      stob_705X = *((long *) (SstackS));
      proc_706X = SvalS;
      if ((3 == (3 & stob_705X))) {
        if ((3 == (3 & proc_706X))) {
          if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_706X))) + -4))))>>2))))) {
            GET_PROPOSAL_LOCK();
            addr_707X = s48_allocate_small(12);
            *((long *) addr_707X) = 2050;
            x_708X = 3 + (((long) (addr_707X + 4)));
            *((long *) (((char *) (-3 + x_708X)))) = stob_705X;
            *((long *) ((((char *) (-3 + x_708X))) + 4)) = proc_706X;
            b_709X = SHARED_REF((Sfinalizer_alistS));
            addr_710X = s48_allocate_small(12);
            *((long *) addr_710X) = 2050;
            x_711X = 3 + (((long) (addr_710X + 4)));
            *((long *) (((char *) (-3 + x_711X)))) = x_708X;
            *((long *) ((((char *) (-3 + x_711X))) + 4)) = b_709X;
            SHARED_SETB((Sfinalizer_alistS), x_711X);
            RELEASE_PROPOSAL_LOCK();
            SvalS = 13;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23052;}
          else {
            goto L40101;}}
        else {
          goto L40101;}}
      else {
        goto L40101;}}
      break;
    case 136 : {
      SstackS = ((SstackS) + 4);
      arg2_712X = *((long *) (SstackS));
      if ((0 == (3 & arg2_712X))) {
        key_713X = ((arg2_712X)>>2);
        other_714X = SvalS;
        if ((5 == key_713X)) {
          SvalS = (-4 & other_714X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23052;}
        else {
          if ((0 == key_713X)) {
            x_715X = s48_available();
            SvalS = (((x_715X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23052;}
          else {
            if ((1 == key_713X)) {
              bytes_716X = s48_heap_size();
              SvalS = (-4 & (3 + bytes_716X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23052;}
            else {
              if ((2 == key_713X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23052;}
              else {
                if ((3 == key_713X)) {
                  x_717X = s48_gc_count();
                  SvalS = (((x_717X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L23052;}
                else {
                  if ((4 == key_713X)) {
                    push_exception_continuationB(14, 1);
                    *((long *) (SstackS)) = (((key_713X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_714X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L21148;}
                  else {
                    push_exception_continuationB(17, 1);
                    *((long *) (SstackS)) = (((key_713X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_714X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L21148;}}}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_712X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
      break;
    case 137 : {
      if ((0 == (3 & (SvalS)))) {
        type_718X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L42723;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21148;}}
      break;
    case 138 : {
      x_719X = SvalS;
      arg2K0 = 1;
      arg0K1 = x_719X;
      goto L46753;}
      break;
    case 139 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 140 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 141 : {
      val_720X = SHARED_REF((Ssession_dataS));
      SvalS = val_720X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 142 : {
      SHARED_SETB((Ssession_dataS), (SvalS));
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 143 : {
      obj_721X = SvalS;
      if ((3 == (3 & obj_721X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_721X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 165)) {
            goto L25514;}
          else {
            temp_722X = SHARED_REF((Sexception_handlersS));
            SHARED_SETB((Sexception_handlersS), (SvalS));
            SvalS = temp_722X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23052;}}
        else {
          goto L25514;}}
      else {
        goto L25514;}}
      break;
    case 144 : {
      SstackS = ((SstackS) + 4);
      p_723X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      template_724X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      p_725X = *((long *) (SstackS));
      StemplateS = template_724X;
      Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_724X)))))))) + ((((((((((p_723X)>>2)) + (((p_725X)>>2))))<<2)))>>2)));
      arg1K0 = (Scode_pointerS);
      goto L23052;}
      break;
    case 145 : {
      obj_726X = SvalS;
      if ((3 == (3 & obj_726X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_726X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 5)) {
            goto L23978;}
          else {
            temp_727X = SHARED_REF((Sinterrupt_handlersS));
            SHARED_SETB((Sinterrupt_handlersS), (SvalS));
            SvalS = temp_727X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23052;}}
        else {
          goto L23978;}}
      else {
        goto L23978;}}
      break;
    case 146 : {
      old_728X = Senabled_interruptsS;
      p_729X = SvalS;
      Senabled_interruptsS = (((p_729X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L46801;}
        else {
          goto L46801;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L46801;}}
      break;
    case 147 : {
      SstackS = ((SstackS) + 4);
      p_730X = *((long *) (SstackS));
      Senabled_interruptsS = (((p_730X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L23066;}
        else {
          goto L23066;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L23066;}}
      break;
    case 148 : {
      if ((0 == (3 & (SvalS)))) {
        p_731X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          s48_Spending_interruptPS = 0;
          if ((s48_Spending_eventsPS)) {
            s48_Spending_interruptPS = 1;
            goto L45489;}
          else {
            goto L45489;}}
        else {
          s48_Spending_interruptPS = 1;
          goto L45489;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21148;}}
      break;
    case 149 : {
      SstackS = ((SstackS) + 4);
      arg2_732X = *((long *) (SstackS));
      if ((0 == (3 & arg2_732X))) {
        x_733X = SvalS;
        if ((1 == x_733X)) {
          goto L43473;}
        else {
          if ((5 == x_733X)) {
            goto L43473;}
          else {
            goto L43478;}}}
      else {
        goto L43478;}}
      break;
    case 150 : {
      SstackS = ((SstackS) + 4);
      nargs_734X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      p_735X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      rest_list_736X = *((long *) (SstackS));
      if ((14 < nargs_734X)) {
        push_exception_continuationB(19, 1);
        *((long *) (SstackS)) = (*((long *) ((SstackS) + (-4 & p_735X))));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = nargs_734X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}
      else {
        arg0K0 = rest_list_736X;
        goto L28595;}}
      break;
    case 151 : {
      s48_make_availableAgc(20);
      SstackS = ((SstackS) + 4);
      arg2_737X = *((long *) (SstackS));
      if ((3 == (3 & arg2_737X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_737X))) + -4))))>>2))))) {
          x_738X = SvalS;
          if ((1 == x_738X)) {
            goto L42818;}
          else {
            if ((5 == x_738X)) {
              goto L42818;}
            else {
              goto L42823;}}}
        else {
          goto L42823;}}
      else {
        goto L42823;}}
      break;
    case 152 : {
      SstackS = ((SstackS) + 4);
      arg2_739X = *((long *) (SstackS));
      if ((3 == (3 & arg2_739X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_739X))) + -4))))>>2))))) {
          x_740X = SvalS;
          if ((1 == x_740X)) {
            goto L30406;}
          else {
            if ((5 == x_740X)) {
              goto L30406;}
            else {
              goto L30411;}}}
        else {
          goto L30411;}}
      else {
        goto L30411;}}
      break;
    case 153 : {
      SstackS = ((SstackS) + 4);
      arg2_741X = *((long *) (SstackS));
      if ((0 == (3 & arg2_741X))) {
        option_742X = ((arg2_741X)>>2);
        other_743X = SvalS;
        if ((2 == option_742X)) {
          x_744X = CHEAP_TIME();
          SvalS = (((x_744X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23052;}
        else {
          if ((0 == option_742X)) {
            seconds_745X = s48_run_time(&mseconds_746X);
            arg0K0 = option_742X;
            arg0K1 = seconds_745X;
            arg0K2 = mseconds_746X;
            goto L44475;}
          else {
            if ((1 == option_742X)) {
              seconds_747X = s48_real_time(&mseconds_748X);
              arg0K0 = option_742X;
              arg0K1 = seconds_747X;
              arg0K2 = mseconds_748X;
              goto L44475;}
            else {
              push_exception_continuationB(17, 1);
              *((long *) (SstackS)) = (((option_742X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = other_743X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21148;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_741X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
      break;
    case 154 : {
      SstackS = ((SstackS) + 4);
      arg2_749X = *((long *) (SstackS));
      if ((0 == (3 & arg2_749X))) {
        key_750X = ((arg2_749X)>>2);
        value_751X = SvalS;
        status_752X = s48_extended_vm(key_750X, value_751X);
        if ((0 == status_752X)) {
          SvalS = (s48_Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23052;}
        else {
          if ((1 == status_752X)) {
            push_exception_continuationB(22, 1);
            *((long *) (SstackS)) = (((key_750X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_751X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21148;}
          else {
            push_exception_continuationB(23, 1);
            *((long *) (SstackS)) = (((key_750X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_751X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21148;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_749X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
      break;
    case 155 : {
      SstackS = ((SstackS) + 4);
      arg2_753X = *((long *) (SstackS));
      x_754X = SvalS;
      Senabled_interruptsS = -1;
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L46861;}
        else {
          goto L46861;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L46861;}}
      break;
    case 156 : {
      SstackS = ((SstackS) + 4);
      arg2_755X = *((long *) (SstackS));
      if ((3 == (3 & arg2_755X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_755X))) + -4))))>>2))))) {
          obj_756X = SvalS;
          if ((3 == (3 & obj_756X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_756X))) + -4))))>>2))))) {
              x_757X = SvalS;
              len_758X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_755X))) + -4))))>>8);
              if ((len_758X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_757X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_757X))), (void *)(((char *) (-3 + arg2_755X))),len_758X)))) {
                  arg0K0 = 5;
                  goto L37370;}
                else {
                  arg0K0 = 1;
                  goto L37370;}}
              else {
                arg0K0 = 1;
                goto L37370;}}
            else {
              goto L37337;}}
          else {
            goto L37337;}}
        else {
          goto L37337;}}
      else {
        goto L37337;}}
      break;
    case 157 : {
      s48_make_availableAgc((4 + (-4 & (4 + ((((SvalS))>>2))))));
      SstackS = ((SstackS) + 4);
      arg2_759X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        n_760X = (((SvalS))>>2);
        if ((3 == (3 & arg2_759X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_759X))) + -4))))>>2))))) {
            goto L37072;}
          else {
            goto L37024;}}
        else {
          goto L37024;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_759X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
      break;
    case 158 : {
      SstackS = ((SstackS) + 4);
      thing_761X = *((long *) (SstackS));
      list_762X = SvalS;
      arg0K0 = list_762X;
      arg0K1 = list_762X;
      arg2K2 = 1;
      goto L38839;}
      break;
    case 159 : {
      SstackS = ((SstackS) + 4);
      arg2_763X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_764X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        index_765X = (((SvalS))>>2);
        if ((3 == (3 & arg3_764X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg3_764X))) + -4))))>>2))))) {
            if ((arg2_763X == (*((long *) (((char *) (-3 + arg3_764X))))))) {
              len_766X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_764X))) + -4))))>>8))))>>2);
              if ((index_765X < 0)) {
                goto L28937;}
              else {
                if ((index_765X < len_766X)) {
                  v_767X = *((unsigned char *) ((Scode_pointerS) + 1));
                  if ((0 == v_767X)) {
                    goto L28927;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L28927;}
                    else {
                      merged_arg0K0 = arg3_764X;
                      merged_arg0K1 = (((index_765X)<<2));
                      proposal_d_read_return_tag = 2;
                      goto proposal_d_read;
                     proposal_d_read_return_2:
                      v_768X = proposal_d_read0_return_value;
                      arg0K0 = v_768X;
                      goto L28936;}}}
                else {
                  goto L28937;}}}
            else {
              goto L28957;}}
          else {
            goto L28957;}}
        else {
          goto L28957;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg3_764X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_763X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L21148;}}
      break;
    case 160 : {
      SstackS = ((SstackS) + 4);
      arg2_769X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_770X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_771X = *((long *) (SstackS));
      if ((0 == (3 & arg2_769X))) {
        index_772X = ((arg2_769X)>>2);
        value_773X = SvalS;
        if ((3 == (3 & arg4_771X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg4_771X))) + -4))))>>2))))) {
            if ((arg3_770X == (*((long *) (((char *) (-3 + arg4_771X))))))) {
              if ((3 == (3 & arg4_771X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_771X))) + -4)))))) {
                  len_774X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_771X))) + -4))))>>8))))>>2);
                  if ((index_772X < 0)) {
                    goto L29195;}
                  else {
                    if ((index_772X < len_774X)) {
                      v_775X = *((unsigned char *) ((Scode_pointerS) + 1));
                      if ((0 == v_775X)) {
                        goto L29185;}
                      else {
                        if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                          goto L29185;}
                        else {
                          merged_arg0K0 = arg4_771X;
                          merged_arg0K1 = (((index_772X)<<2));
                          merged_arg0K2 = value_773X;
                          proposal_d_write_return_tag = 2;
                          goto proposal_d_write;
                         proposal_d_write_return_2:
                          goto L29194;}}}
                    else {
                      goto L29195;}}}
                else {
                  goto L29217;}}
              else {
                goto L29217;}}
            else {
              goto L29217;}}
          else {
            goto L29217;}}
        else {
          goto L29217;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg4_771X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_770X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_769X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 4;
        goto L21148;}}
      break;
    case 161 : {
      v_776X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_776X)) {
        arg0K0 = (SvalS);
        goto L29474;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_777X = get_current_port0_return_value;
        arg0K0 = v_777X;
        goto L29474;}}
      break;
    case 162 : {
      v_778X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_778X)) {
        arg0K0 = (SvalS);
        goto L29715;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_779X = get_current_port0_return_value;
        arg0K0 = v_779X;
        goto L29715;}}
      break;
    case 163 : {
      v_780X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_780X)) {
        v_781X = SvalS;
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = v_781X;
        goto L29949;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_782X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_782X;
        goto L29949;}}
      break;
    case 164 : {
      x_783X = SvalS;
      out_784X = stderr;
      arg0K0 = x_783X;
      goto L39092;}
      break;
  }}
 L21148: {
  nargs_785X = arg0K0;
  opcode_786X = (((*((long *) ((SstackS) + (8 + (((nargs_785X)<<2)))))))>>2);
  handlers_787X = SHARED_REF((Sexception_handlersS));
  if ((3 == (3 & handlers_787X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_787X))) + -4))))>>2))))) {
      goto L21211;}
    else {
      goto L21284;}}
  else {
    goto L21284;}}
 L30324: {
  env_788X = arg0K0;
  i_789X = arg0K1;
  if ((0 == i_789X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_788X))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 3);
      arg0K0 = 0;
      goto L21148;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23052;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_788X)))));
    arg0K1 = (-1 + i_789X);
    goto L30324;}}
 L25019: {
  env_790X = arg0K0;
  i_791X = arg0K1;
  if ((0 == i_791X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_790X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 5);
      arg0K0 = 0;
      goto L21148;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23052;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_790X)))));
    arg0K1 = (-1 + i_791X);
    goto L25019;}}
 L24957: {
  env_792X = arg0K0;
  i_793X = arg0K1;
  if ((0 == i_793X)) {
    index_794X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
    value_795X = SvalS;
    addr_796X = (((char *) (-3 + env_792X))) + (((index_794X)<<2));
    S48_WRITE_BARRIER(env_792X, addr_796X, value_795X);
    *((long *) addr_796X) = value_795X;
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 5);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_792X)))));
    arg0K1 = (-1 + i_793X);
    goto L24957;}}
 L25694: {
  v_797X = arg0K0;
  s48_make_availableAgc((12 + (((v_797X)<<2))));
  p_798X = SenvS;
  if ((3 == (3 & p_798X))) {
    if ((p_798X < (((long) (Sstack_beginS))))) {
      goto L25748;}
    else {
      if (((((long) (Sstack_endS))) < p_798X)) {
        goto L25748;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = 0;
        merged_arg0K3 = 0;
        save_env_in_heap_return_tag = 0;
        goto save_env_in_heap;
       save_env_in_heap_return_0:
        v_799X = save_env_in_heap0_return_value;
        SenvS = v_799X;
        goto L25748;}}}
  else {
    goto L25748;}}
 L25707: {
  env_800X = arg0K0;
  key_801X = arg0K1;
  merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  merged_arg0K1 = env_800X;
  merged_arg0K2 = key_801X;
  make_closure_return_tag = 0;
  goto make_closure;
 make_closure_return_0:
  value_802X = make_closure0_return_value;
  SvalS = value_802X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L24744: {
  i_803X = arg0K0;
  if ((count_405X < i_803X)) {
    value_804X = *((long *) (((char *) (-3 + (SvalS)))));
    addr_805X = ((char *) (-3 + vector_408X));
    S48_WRITE_BARRIER(vector_408X, addr_805X, value_804X);
    *((long *) addr_805X) = value_804X;
    x_806X = SvalS;
    addr_807X = ((char *) (-3 + x_806X));
    S48_WRITE_BARRIER(x_806X, addr_807X, vector_408X);
    *((long *) addr_807X) = vector_408X;
    *((long *) (SstackS)) = (SenvS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (1034 + (((count_405X)<<10)));
    SstackS = ((SstackS) + -4);
    SenvS = (3 + (((long) ((SstackS) + 8))));
    Scode_pointerS = ((Scode_pointerS) + (3 + (((count_405X)<<1))));
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    index_808X = ((i_803X)<<1);
    merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + index_808X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_808X))))))<<2)))));
    merged_arg0K1 = (SvalS);
    merged_arg0K2 = 0;
    make_closure_return_tag = 1;
    goto make_closure;
   make_closure_return_1:
    closure_809X = make_closure0_return_value;
    addr_810X = (((char *) (-3 + vector_408X))) + (((i_803X)<<2));
    S48_WRITE_BARRIER(vector_408X, addr_810X, closure_809X);
    *((long *) addr_810X) = closure_809X;
    *((long *) (SstackS)) = closure_809X;
    SstackS = ((SstackS) + -4);
    arg0K0 = (1 + i_803X);
    goto L24744;}}
 L26031: {
  start_i_811X = arg0K0;
  arg0K0 = start_i_811X;
  arg0K1 = 2;
  arg0K2 = (SenvS);
  goto L26039;}
 L25882: {
  start_i_812X = arg0K0;
  arg0K0 = start_i_812X;
  arg0K1 = 3;
  arg0K2 = (SenvS);
  goto L25890;}
 L46173: {
  value_813X = arg0K0;
  SvalS = value_813X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L23709: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 0;
      goto check_events;
     check_events_return_0:
      v_814X = check_events0_return_value;
      if (v_814X) {
        arg0K0 = stack_arg_count_424X;
        goto L21381;}
      else {
        goto L23713;}}
    else {
      arg0K0 = stack_arg_count_424X;
      goto L21381;}}
  else {
    goto L23713;}}
 L23719: {
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L7658: {
  loc_815X = arg1K0;
  arg_816X = arg1K1;
  if ((top_of_args_430X < arg_816X)) {
    *((long *) loc_815X) = (*((long *) arg_816X));
    arg1K0 = (loc_815X + -4);
    arg1K1 = (arg_816X + -4);
    goto L7658;}
  else {
    SstackS = loc_815X;
    s48_call_native_code(proc_429X);
    goto L23719;}}
 L20778: {
  stack_arg_count_817X = arg0K0;
  list_args_818X = arg0K1;
  list_arg_count_819X = arg0K2;
  code_820X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  total_arg_count_821X = stack_arg_count_817X + list_arg_count_819X;
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_820X))) + 1)));
  arg0K1 = 64;
  goto L20793;}
 L21650: {
  exception_822X = arg0K0;
  stack_arg_count_823X = arg0K1;
  list_args_824X = arg0K2;
  list_arg_count_825X = arg0K3;
  if (((StemplateS) == (SvalS))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      return v_826X;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      return v_827X;}}
  else {
    merged_arg0K0 = list_args_824X;
    merged_arg0K1 = list_arg_count_825X;
    copy_listSAgc_return_tag = 0;
    goto copy_listSAgc;
   copy_listSAgc_return_0:
    v_828X = copy_listSAgc0_return_value;
    merged_arg0K0 = v_828X;
    merged_arg0K1 = stack_arg_count_823X;
    pop_args_GlistSAgc_return_tag = 1;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_1:
    args_829X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(exception_822X, 0);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_829X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}}
 L22278: {
  stack_arg_count_830X = arg0K0;
  list_args_831X = arg0K1;
  list_arg_count_832X = arg0K2;
  if ((0 == list_arg_count_832X)) {
    obj_833X = SvalS;
    if ((3 == (3 & obj_833X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_833X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_830X;
        arg0K1 = 25;
        arg0K2 = 0;
        goto L20778;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_830X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L21650;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_830X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L21650;}}
  else {
    obj_834X = SvalS;
    if ((3 == (3 & obj_834X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_834X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_830X;
        arg0K1 = list_args_831X;
        arg0K2 = list_arg_count_832X;
        goto L20778;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_830X;
        arg0K2 = list_args_831X;
        arg0K3 = list_arg_count_832X;
        goto L21650;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_830X;
      arg0K2 = list_args_831X;
      arg0K3 = list_arg_count_832X;
      goto L21650;}}}
 L14261: {
  list_args_835X = arg0K0;
  stack_nargs_836X = arg0K1;
  merged_arg0K0 = list_args_835X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_837X = okay_argument_list0_return_value;
  list_arg_count_838X = okay_argument_list1_return_value;
  if (okayP_837X) {
    SvalS = proc_439X;
    arg0K0 = stack_nargs_836X;
    arg0K1 = list_args_835X;
    arg0K2 = list_arg_count_838X;
    goto L22278;}
  else {
    *((long *) (SstackS)) = list_args_835X;
    SstackS = ((SstackS) + -4);
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_836X);
    pop_args_GlistSAgc_return_tag = 2;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_2:
    args_839X = pop_args_GlistSAgc0_return_value;
    SstackS = ((SstackS) + 4);
    proc_840X = *((long *) (SstackS));
    push_exception_continuationB(5, 0);
    *((long *) (SstackS)) = proc_840X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_839X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}}
 L14302: {
  list_841X = arg0K0;
  follower_842X = arg0K1;
  if ((25 == list_841X)) {
    list_args_843X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_842X))) + 4)))))));
    addr_844X = (((char *) (-3 + follower_842X))) + 4;
    S48_WRITE_BARRIER(follower_842X, addr_844X, list_args_843X);
    *((long *) addr_844X) = list_args_843X;
    arg0K0 = rest_list_440X;
    arg0K1 = (-1 + stack_nargs_438X);
    goto L14261;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_841X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_842X))) + 4)));
    goto L14302;}}
 L22223: {
  obj_845X = SvalS;
  if ((3 == (3 & obj_845X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_845X))) + -4))))>>2))))) {
      arg0K0 = 0;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L20778;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L21650;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L21650;}}
 L22230: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_441X;
  ScontS = (Sbottom_of_stackS);
  goto L22223;}
 L24319: {
  cont_846X = ScontS;
  code_847X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_846X))) + 8)))))));
  pc_848X = (((*((long *) ((((char *) (-3 + cont_846X))) + 4))))>>2);
  protocol_849X = *((unsigned char *) ((((char *) (-3 + code_847X))) + (1 + pc_848X)));
  if ((1 == protocol_849X)) {
    goto L24348;}
  else {
    if ((66 == protocol_849X)) {
      goto L24348;}
    else {
      if ((71 == protocol_849X)) {
        cont_850X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
        if ((3 == (3 & cont_850X))) {
          if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_850X))) + -4))))>>2))))) {
            merged_arg0K0 = cont_850X;
            merged_arg0K1 = 0;
            copy_continuation_from_heapB_return_tag = 1;
            goto copy_continuation_from_heapB;
           copy_continuation_from_heapB_return_1:
            goto L24319;}
          else {
            arg0K0 = cont_850X;
            goto L21714;}}
        else {
          arg0K0 = cont_850X;
          goto L21714;}}
      else {
        if ((70 == protocol_849X)) {
          next_851X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_851X)))));
            goto L24366;}
          else {
            ScontS = next_851X;
            goto L24366;}}
        else {
          if ((65 == protocol_849X)) {
            index_852X = 2 + pc_848X;
            wants_stack_args_853X = ((((*((unsigned char *) ((((char *) (-3 + code_847X))) + index_852X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_847X))) + (1 + index_852X))));
            if ((0 == wants_stack_args_853X)) {
              pop_continuationB_return_tag = 0;
              goto pop_continuationB;
             pop_continuationB_return_0:
              s48_make_availableAgc(12);
              a_854X = SvalS;
              addr_855X = s48_allocate_small(12);
              *((long *) addr_855X) = 2050;
              x_856X = 3 + (((long) (addr_855X + 4)));
              *((long *) (((char *) (-3 + x_856X)))) = a_854X;
              *((long *) ((((char *) (-3 + x_856X))) + 4)) = 25;
              *((long *) (SstackS)) = x_856X;
              SstackS = ((SstackS) + -4);
              Scode_pointerS = ((Scode_pointerS) + 4);
              arg1K0 = (Scode_pointerS);
              goto L23052;}
            else {
              if ((1 == wants_stack_args_853X)) {
                pop_continuationB_return_tag = 1;
                goto pop_continuationB;
               pop_continuationB_return_1:
                *((long *) (SstackS)) = (SvalS);
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = 25;
                SstackS = ((SstackS) + -4);
                Scode_pointerS = ((Scode_pointerS) + 4);
                arg1K0 = (Scode_pointerS);
                goto L23052;}
              else {
                *((long *) (SstackS)) = (SvalS);
                SstackS = ((SstackS) + -4);
                merged_arg0K0 = 25;
                merged_arg0K1 = 1;
                pop_args_GlistSAgc_return_tag = 3;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_3:
                args_857X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_857X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21148;}}}
          else {
            *((long *) (SstackS)) = (SvalS);
            SstackS = ((SstackS) + -4);
            merged_arg0K0 = 25;
            merged_arg0K1 = 1;
            pop_args_GlistSAgc_return_tag = 4;
            goto pop_args_GlistSAgc;
           pop_args_GlistSAgc_return_4:
            args_858X = pop_args_GlistSAgc0_return_value;
            push_exception_continuationB(4, 0);
            *((long *) (SstackS)) = 1;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = args_858X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21148;}}}}}}
 L45637: {
  stack_nargs_859X = arg0K0;
  list_args_860X = arg0K1;
  list_arg_count_861X = arg0K2;
  cont_862X = ScontS;
  code_863X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_862X))) + 8)))))));
  pc_864X = (((*((long *) ((((char *) (-3 + cont_862X))) + 4))))>>2);
  protocol_865X = *((unsigned char *) ((((char *) (-3 + code_863X))) + (1 + pc_864X)));
  if ((1 == protocol_865X)) {
    if ((1 == (stack_nargs_859X + list_arg_count_861X))) {
      if ((1 == stack_nargs_859X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        goto L45732;}
      else {
        arg0K0 = (*((long *) (((char *) (-3 + list_args_860X)))));
        goto L45732;}}
    else {
      merged_arg0K0 = list_args_860X;
      merged_arg0K1 = stack_nargs_859X;
      pop_args_GlistSAgc_return_tag = 5;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_5:
      args_866X = pop_args_GlistSAgc0_return_value;
      push_exception_continuationB(4, 0);
      *((long *) (SstackS)) = 1;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = args_866X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}}
  else {
    if ((66 == protocol_865X)) {
      pop_continuationB_return_tag = 2;
      goto pop_continuationB;
     pop_continuationB_return_2:
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23052;}
    else {
      if ((71 == protocol_865X)) {
        cont_867X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
        if ((3 == (3 & cont_867X))) {
          if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_867X))) + -4))))>>2))))) {
            merged_arg0K0 = cont_867X;
            merged_arg0K1 = stack_nargs_859X;
            copy_continuation_from_heapB_return_tag = 2;
            goto copy_continuation_from_heapB;
           copy_continuation_from_heapB_return_2:
            arg0K0 = stack_nargs_859X;
            arg0K1 = list_args_860X;
            arg0K2 = list_arg_count_861X;
            goto L45637;}
          else {
            goto L45680;}}
        else {
          goto L45680;}}
      else {
        if ((70 == protocol_865X)) {
          next_868X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_868X)))));
            goto L45688;}
          else {
            ScontS = next_868X;
            goto L45688;}}
        else {
          if ((63 < protocol_865X)) {
            if ((65 == protocol_865X)) {
              index_869X = 2 + pc_864X;
              count_870X = ((((*((unsigned char *) ((((char *) (-3 + code_863X))) + index_869X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_863X))) + (1 + index_869X))));
              if (((stack_nargs_859X + list_arg_count_861X) < count_870X)) {
                merged_arg0K0 = list_args_860X;
                merged_arg0K1 = stack_nargs_859X;
                pop_args_GlistSAgc_return_tag = 6;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_6:
                args_871X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_871X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21148;}
              else {
                arg_top_872X = SstackS;
                pop_continuationB_return_tag = 3;
                goto pop_continuationB;
               pop_continuationB_return_3:
                arg1K0 = (SstackS);
                arg1K1 = (arg_top_872X + (((stack_nargs_859X)<<2)));
                goto L25304;}}
            else {
              if ((64 == protocol_865X)) {
                index_873X = 2 + pc_864X;
                arg0K0 = (((((*((unsigned char *) ((((char *) (-3 + code_863X))) + index_873X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_863X))) + (1 + index_873X)))));
                arg0K1 = 3;
                arg0K2 = stack_nargs_859X;
                arg0K3 = list_args_860X;
                arg0K4 = list_arg_count_861X;
                goto L25156;}
              else {
                ps_error("unknown protocol", 1, protocol_865X);
                merged_arg0K0 = list_args_860X;
                merged_arg0K1 = stack_nargs_859X;
                pop_args_GlistSAgc_return_tag = 7;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_7:
                args_874X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_874X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21148;}}}
          else {
            arg0K0 = protocol_865X;
            arg0K1 = 1;
            arg0K2 = stack_nargs_859X;
            arg0K3 = list_args_860X;
            arg0K4 = list_arg_count_861X;
            goto L25156;}}}}}}
 L23449: {
  tem_875X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  StemplateS = tem_875X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_875X)))))))) + 2);
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 1;
      goto check_events;
     check_events_return_1:
      v_876X = check_events0_return_value;
      if (v_876X) {
        arg0K0 = 0;
        goto L21381;}
      else {
        goto L23459;}}
    else {
      arg0K0 = 0;
      goto L21381;}}
  else {
    goto L23459;}}
 L23492: {
  push_exception_continuationB(5, 4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L23350: {
  arg0K0 = (2 + (((max_450X)<<1)));
  goto L23352;}
 L23352: {
  offset_877X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_877X);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L24235: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L23248: {
  delta_878X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_878X);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L24126: {
  delta_879X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_879X);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L46226: {
  val_880X = arg0K0;
  SvalS = val_880X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L46238: {
  val_881X = arg0K0;
  SvalS = val_881X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L12946: {
  if ((3 == (3 & x_464X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L46238;}
    else {
      goto L12952;}}
  else {
    goto L12952;}}
 L37919: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L37920: {
  if ((3 == (3 & n_465X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_465X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_465X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
    else {
      goto L37924;}}
  else {
    goto L37924;}}
 L38076: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L38051: {
  if ((3 == (3 & n_466X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      goto L38076;}
    else {
      goto L38065;}}
  else {
    goto L38065;}}
 L38275: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L38250: {
  if ((3 == (3 & n_467X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      goto L38275;}
    else {
      goto L38264;}}
  else {
    goto L38264;}}
 L38474: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L38449: {
  if ((3 == (3 & n_468X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      goto L38474;}
    else {
      goto L38463;}}
  else {
    goto L38463;}}
 L31820: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L31821: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_469X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L31932: {
  v_882X = (char *) s48_long_to_bignum(x_474X);
  v_883X = enter_bignum(v_882X);
  arg0K0 = v_883X;
  goto L31892;}
 L31892: {
  val_884X = arg0K0;
  SvalS = val_884X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L31898: {
  if ((0 == (3 & y_473X))) {
    goto L31904;}
  else {
    if ((3 == (3 & y_473X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_473X))) + -4))))>>2))))) {
        goto L31904;}
      else {
        goto L31907;}}
    else {
      goto L31907;}}}
 L31907: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_472X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_473X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L7805: {
  a_885X = arg0K0;
  if ((b_478X < 0)) {
    arg0K0 = (0 - b_478X);
    goto L7809;}
  else {
    arg0K0 = b_478X;
    goto L7809;}}
 L40221: {
  if ((0 == (3 & y_476X))) {
    goto L40227;}
  else {
    if ((3 == (3 & y_476X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_476X))) + -4))))>>2))))) {
        goto L40227;}
      else {
        goto L40230;}}
    else {
      goto L40230;}}}
 L40230: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_475X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_476X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L32123: {
  v_886X = (char *) s48_long_to_bignum(x_481X);
  v_887X = enter_bignum(v_886X);
  arg0K0 = v_887X;
  goto L32083;}
 L32083: {
  val_888X = arg0K0;
  SvalS = val_888X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32089: {
  if ((0 == (3 & y_480X))) {
    goto L32095;}
  else {
    if ((3 == (3 & y_480X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_480X))) + -4))))>>2))))) {
        goto L32095;}
      else {
        goto L32098;}}
    else {
      goto L32098;}}}
 L32098: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_479X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_480X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L8063: {
  a_889X = arg0K0;
  if ((b_485X < 0)) {
    arg0K0 = (0 - b_485X);
    goto L8067;}
  else {
    arg0K0 = b_485X;
    goto L8067;}}
 L40376: {
  if ((0 == (3 & y_483X))) {
    goto L40382;}
  else {
    if ((3 == (3 & y_483X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_483X))) + -4))))>>2))))) {
        goto L40382;}
      else {
        goto L40409;}}
    else {
      goto L40409;}}}
 L40409: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_482X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_483X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L32270: {
  val_890X = arg0K0;
  SvalS = val_890X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32276: {
  if ((0 == (3 & y_487X))) {
    goto L32282;}
  else {
    if ((3 == (3 & y_487X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_487X))) + -4))))>>2))))) {
        goto L32282;}
      else {
        goto L32287;}}
    else {
      goto L32287;}}}
 L32287: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_486X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_487X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L32447: {
  val_891X = arg0K0;
  SvalS = val_891X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32453: {
  if ((0 == (3 & y_489X))) {
    goto L32459;}
  else {
    if ((3 == (3 & y_489X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_489X))) + -4))))>>2))))) {
        goto L32459;}
      else {
        goto L32464;}}
    else {
      goto L32464;}}}
 L32464: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_488X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_489X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L32687: {
  val_892X = arg0K0;
  SvalS = val_892X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32693: {
  if ((0 == (3 & y_491X))) {
    goto L32699;}
  else {
    if ((3 == (3 & y_491X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_491X))) + -4))))>>2))))) {
        goto L32699;}
      else {
        goto L32704;}}
    else {
      goto L32704;}}}
 L32704: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_490X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_491X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L32927: {
  val_893X = arg0K0;
  SvalS = val_893X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32933: {
  if ((0 == (3 & y_493X))) {
    goto L32939;}
  else {
    if ((3 == (3 & y_493X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_493X))) + -4))))>>2))))) {
        goto L32939;}
      else {
        goto L32944;}}
    else {
      goto L32944;}}}
 L32944: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_492X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_493X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L33121: {
  val_894X = arg0K0;
  SvalS = val_894X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L33127: {
  if ((0 == (3 & y_495X))) {
    goto L33133;}
  else {
    if ((3 == (3 & y_495X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_495X))) + -4))))>>2))))) {
        goto L33133;}
      else {
        goto L33138;}}
    else {
      goto L33138;}}}
 L33138: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_494X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_495X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L18413: {
  x_895X = arg0K0;
  y_896X = arg0K1;
  if ((0 == (3 & x_895X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18443;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_895X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18443;}}
 L8708: {
  a_897X = arg0K0;
  if ((b_499X < 0)) {
    arg0K0 = (0 - b_499X);
    goto L8712;}
  else {
    arg0K0 = b_499X;
    goto L8712;}}
 L33327: {
  if ((0 == (3 & y_497X))) {
    goto L33333;}
  else {
    if ((3 == (3 & y_497X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_497X))) + -4))))>>2))))) {
        goto L33333;}
      else {
        goto L33336;}}
    else {
      goto L33336;}}}
 L33336: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_496X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_497X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L18322: {
  x_898X = arg0K0;
  y_899X = arg0K1;
  if ((0 == (3 & x_898X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18352;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_898X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18352;}}
 L33541: {
  a_900X = arg0K0;
  n_901X = ((y_501X)>>2);
  if ((n_901X < 0)) {
    arg0K0 = (0 - n_901X);
    goto L33543;}
  else {
    arg0K0 = n_901X;
    goto L33543;}}
 L33503: {
  if ((0 == (3 & y_501X))) {
    goto L33509;}
  else {
    if ((3 == (3 & y_501X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_501X))) + -4))))>>2))))) {
        goto L33509;}
      else {
        goto L33512;}}
    else {
      goto L33512;}}}
 L33512: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_500X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_501X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L33690: {
  SvalS = n_503X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L33691: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_503X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L33745: {
  SvalS = n_504X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L33746: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_504X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L33800: {
  SvalS = 4;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L33803: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_505X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L33860: {
  SvalS = n_506X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L33861: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_506X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L33915: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L33918: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_507X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L46392: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_516X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L46398: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L21899: {
  x_902X = SvalS;
  if ((0 == (3 & x_902X))) {
    s48_make_availableAgc(16);
    if ((x_902X < 0)) {
      arg0K0 = (0 - x_902X);
      goto L21927;}
    else {
      arg0K0 = x_902X;
      goto L21927;}}
  else {
    if ((0 == (3 & x_902X))) {
      arg0K0 = 1;
      arg0K1 = 3;
      goto L17930;}
    else {
      arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_902X))) + -4))))>>8))))>>2)));
      arg0K1 = 0;
      goto L17930;}}}
 L21902: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L34069: {
  if ((0 == (3 & x_525X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18277;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_525X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18277;}}
 L34072: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_525X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L34160: {
  x_903X = arg0K0;
  arg0K0 = x_903X;
  arg0K1 = 0;
  goto L34166;}
 L34143: {
  if ((0 == (3 & x_526X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18219;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_526X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18219;}}
 L34146: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_526X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L34245: {
  if ((0 == (3 & y_529X))) {
    goto L34251;}
  else {
    if ((3 == (3 & y_529X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_529X))) + -4))))>>2))))) {
        goto L34251;}
      else {
        goto L34254;}}
    else {
      goto L34254;}}}
 L34254: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_528X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_529X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L34408: {
  if ((0 == (3 & y_531X))) {
    goto L34414;}
  else {
    if ((3 == (3 & y_531X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_531X))) + -4))))>>2))))) {
        goto L34414;}
      else {
        goto L34417;}}
    else {
      goto L34417;}}}
 L34417: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_530X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_531X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L34571: {
  if ((0 == (3 & y_533X))) {
    goto L34577;}
  else {
    if ((3 == (3 & y_533X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_533X))) + -4))))>>2))))) {
        goto L34577;}
      else {
        goto L34580;}}
    else {
      goto L34580;}}}
 L34580: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_532X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_533X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L31600: {
  val_904X = arg0K0;
  SvalS = val_904X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L31616: {
  val_905X = arg0K0;
  SvalS = val_905X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L31617: {
  push_exception_continuationB(6, 1);
  *((long *) (SstackS)) = x_534X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_535X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L40609: {
  if ((0 == (3 & y_535X))) {
    if ((0 == (3 & x_534X))) {
      value_906X = ((x_534X)>>2);
      count_907X = ((y_535X)>>2);
      if ((count_907X < 0)) {
        PS_SHIFT_RIGHT(value_906X, (0 - count_907X), x_908X)
        x_909X = x_908X;
        s48_make_availableAgc(16);
        if ((536870911 < x_909X)) {
          goto L40729;}
        else {
          if ((x_909X < -536870912)) {
            goto L40729;}
          else {
            arg0K0 = (((x_909X)<<2));
            goto L40724;}}}
      else {
        PS_SHIFT_LEFT(value_906X, count_907X, x_910X)
        result_911X = x_910X;
        PS_SHIFT_RIGHT(result_911X, count_907X, x_912X)
        if ((value_906X == x_912X)) {
          if ((value_906X < 0)) {
            if ((result_911X < 0)) {
              s48_make_availableAgc(16);
              if ((536870911 < result_911X)) {
                goto L40751;}
              else {
                if ((result_911X < -536870912)) {
                  goto L40751;}
                else {
                  arg0K0 = (((result_911X)<<2));
                  goto L40746;}}}
            else {
              arg0K0 = x_534X;
              arg0K1 = y_535X;
              goto L40622;}}
          else {
            if ((result_911X < 0)) {
              arg0K0 = x_534X;
              arg0K1 = y_535X;
              goto L40622;}
            else {
              s48_make_availableAgc(16);
              if ((536870911 < result_911X)) {
                goto L40773;}
              else {
                if ((result_911X < -536870912)) {
                  goto L40773;}
                else {
                  arg0K0 = (((result_911X)<<2));
                  goto L40768;}}}}}
        else {
          arg0K0 = x_534X;
          arg0K1 = y_535X;
          goto L40622;}}}
    else {
      if ((3 == (3 & x_534X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_534X))) + -4))))>>2))))) {
          y_913X = ((y_535X)>>2);
          merged_arg0K0 = x_534X;
          merged_arg0K1 = y_913X;
          shift_space_return_tag = 0;
          goto shift_space;
         shift_space_return_0:
          needed_914X = shift_space0_return_value;
          Stemp0S = x_534X;
          s48_make_availableAgc((((needed_914X)<<2)));
          value_915X = Stemp0S;
          Stemp0S = 1;
          if ((0 == (3 & value_915X))) {
            v_916X = (char *) s48_long_to_bignum((((value_915X)>>2)));
            arg1K0 = v_916X;
            goto L40804;}
          else {
            arg1K0 = (((char *) (-3 + value_915X)));
            goto L40804;}}
        else {
          goto L40631;}}
      else {
        goto L40631;}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_534X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_535X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}}
 L46480: {
  val_917X = arg0K0;
  SvalS = val_917X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L37694: {
  val_918X = arg0K0;
  SvalS = val_918X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L37667: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_539X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L37604: {
  val_919X = arg0K0;
  SvalS = val_919X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L37577: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_541X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L41799: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((x_543X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L46509: {
  val_920X = arg0K0;
  SvalS = val_920X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L46523: {
  value_921X = arg0K0;
  SvalS = value_921X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L26219: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = stob_547X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_548X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L26324: {
  SvalS = new_553X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L26308: {
  i_922X = arg0K0;
  if ((i_922X < 0)) {
    goto L26324;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_553X))) + (((i_922X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_922X);
    goto L26308;}}
 L26440: {
  i_923X = arg0K0;
  if ((i_923X < 0)) {
    arg0K0 = stack_nargs_559X;
    arg0K1 = rest_list_560X;
    goto L26574;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_558X))) + (((i_923X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_923X);
    goto L26440;}}
 L26600: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_561X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_563X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_562X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L26717: {
  addr_924X = (((char *) (-3 + stob_564X))) + (((offset_566X)<<2));
  S48_WRITE_BARRIER(stob_564X, addr_924X, value_565X);
  *((long *) addr_924X) = value_565X;
  goto L26726;}
 L26726: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L26727: {
  push_exception_continuationB(5, 4);
  *((long *) (SstackS)) = stob_564X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_567X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_566X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_565X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L26944: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = (((type_571X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_572X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = init_570X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L26961: {
  x_925X = arg0K0;
  value_926X = Stemp0S;
  Stemp0S = 1;
  if ((1 == x_925X)) {
    push_exception_continuationB(8, 2);
    *((long *) (SstackS)) = (((type_571X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_572X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = value_926X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L21148;}
  else {
    arg0K0 = (-1 + len_572X);
    goto L26987;}}
 L27234: {
  push_exception_continuationB(7, 3);
  *((long *) (SstackS)) = stob_575X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_577X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_576X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L27224: {
  arg0K0 = (*((long *) ((((char *) (-3 + stob_575X))) + (-4 & index_576X))));
  goto L27233;}
 L27233: {
  value_927X = arg0K0;
  SvalS = value_927X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L27189: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_575X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_577X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_576X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L27521: {
  push_exception_continuationB(7, 3);
  *((long *) (SstackS)) = stob_583X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_585X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_582X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_584X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L27511: {
  addr_928X = (((char *) (-3 + stob_583X))) + (-4 & arg2_582X);
  S48_WRITE_BARRIER(stob_583X, addr_928X, value_584X);
  *((long *) addr_928X) = value_584X;
  goto L27520;}
 L27520: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L27474: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_583X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_585X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_582X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_584X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L34792: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_590X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_591X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L34805: {
  vector_929X = arg0K0;
  if ((1 == vector_929X)) {
    push_exception_continuationB(8, 1);
    *((long *) (SstackS)) = (((len_590X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((init_591X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}
  else {
    arg0K0 = (-1 + len_590X);
    goto L34827;}}
 L45304: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L41905: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_594X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_595X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L44820: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_594X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L39443: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_598X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_599X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_600X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L39422: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_598X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_599X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_600X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L43600: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_598X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_597X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L34972: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_603X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_604X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L34985: {
  vector_930X = arg0K0;
  if ((1 == vector_930X)) {
    push_exception_continuationB(8, 1);
    *((long *) (SstackS)) = (((len_603X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (9 + ((((init_604X))<<8)));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_930X))) + len_603X)) = 0;
    arg0K0 = (-1 + len_603X);
    goto L35007;}}
 L41986: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_602X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L45354: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L42049: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_608X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_609X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L44916: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_608X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L39598: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_612X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_613X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_614X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L39577: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_612X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_613X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_614X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L43726: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_612X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_611X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L19703: {
  i_931X = arg0K0;
  h_932X = arg0K1;
  if ((i_931X < n_619X)) {
    arg0K0 = (1 + i_931X);
    arg0K1 = (h_932X + (((*((unsigned char *) ((((char *) (-3 + string_618X))) + i_931X))))));
    goto L19703;}
  else {
    index_933X = 1023 & h_932X;
    link_934X = *((long *) ((((char *) (-3 + table_617X))) + (((index_933X)<<2))));
    if ((0 == (3 & link_934X))) {
      arg0K0 = (3 + (-4 & link_934X));
      goto L19659;}
    else {
      arg0K0 = link_934X;
      goto L19659;}}}
 L31762: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L43290: {
  val_935X = arg0K0;
  SvalS = val_935X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L43271: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L39185: {
  if ((1 == (SvalS))) {
    addr_936X = (((char *) (-3 + arg2_623X))) + 4;
    S48_WRITE_BARRIER(arg2_623X, addr_936X, 273);
    *((long *) addr_936X) = 273;
    goto L39191;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_623X))) + 4)))))) {
      addr_937X = (((char *) (-3 + arg2_623X))) + 4;
      S48_WRITE_BARRIER(arg2_623X, addr_937X, 529);
      *((long *) addr_937X) = 529;
      goto L39191;}
    else {
      goto L39191;}}}
 L39192: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_623X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L46595: {
  val_938X = arg0K0;
  SvalS = val_938X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L42131: {
  SvalS = x_626X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L35379: {
  if ((0 == (3 & arg2_627X))) {
    if (((((arg2_627X)>>2)) < 0)) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}
    else {
      arg0K0 = (((arg2_627X)>>2));
      goto L35199;}}
  else {
    if ((3 == (3 & arg2_627X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_627X))) + -4))))>>2))))) {
        if ((1 == mode_628X)) {
          goto L35257;}
        else {
          if ((3 == mode_628X)) {
            goto L35257;}
          else {
            v_939X = ps_open_fd((((char *)(((char *) (-3 + arg2_627X))))), 0, &v_940X);
            arg0K0 = v_939X;
            arg0K1 = v_940X;
            goto L35270;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_627X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_628X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}}}
 L46629: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L42256: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L42266;}
  else {
    arg2K0 = 1;
    goto L42266;}}
 L42267: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_636X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_635X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_634X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_633X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21148;}
 L36018: {
  if ((3 == (3 & arg3_639X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>8)));
      goto L36020;}
    else {
      goto L36207;}}
  else {
    goto L36207;}}
 L36193: {
  if ((3 == (3 & arg3_639X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>2))))) {
      goto L36018;}
    else {
      goto L36072;}}
  else {
    goto L36072;}}
 L36072: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_640X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_639X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_641X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_642X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L42462: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_640X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_639X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_638X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L38776: {
  val_941X = arg0K0;
  SvalS = val_941X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L46651: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L42604: {
  val_942X = arg0K0;
  SvalS = val_942X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L10555: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_650X))) + 16)));
  goto L42604;}
 L10561: {
  ch_943X = arg0K0;
  prev_944X = arg0K1;
  if ((1 == ch_943X)) {
    addr_945X = (((char *) (-3 + channel_650X))) + 16;
    S48_WRITE_BARRIER(channel_650X, addr_945X, 1);
    *((long *) addr_945X) = 1;
    n_946X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_650X))) + 8))))>>2)));
    arg0K0 = (((n_946X)<<2));
    goto L42604;}
  else {
    if ((ch_943X == channel_650X)) {
      y_947X = Spending_channels_tailS;
      if ((ch_943X == y_947X)) {
        Spending_channels_tailS = prev_944X;
        goto L10585;}
      else {
        goto L10585;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_943X))) + 12)));
      arg0K1 = ch_943X;
      goto L10561;}}}
 L42587: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L15191: {
  i_948X = arg0K0;
  res_949X = arg0K1;
  if ((-1 == i_948X)) {
    SvalS = res_949X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    channel_950X = *((Svm_channelsS) + i_948X);
    if ((3 == (3 & channel_950X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_950X))) + -4))))>>2))))) {
        addr_951X = s48_allocate_small(12);
        *((long *) addr_951X) = 2050;
        x_952X = 3 + (((long) (addr_951X + 4)));
        *((long *) (((char *) (-3 + x_952X)))) = channel_950X;
        *((long *) ((((char *) (-3 + x_952X))) + 4)) = res_949X;
        arg0K0 = x_952X;
        goto L15205;}
      else {
        arg0K0 = res_949X;
        goto L15205;}}
    else {
      arg0K0 = res_949X;
      goto L15205;}}}
 L36326: {
  old_953X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 == old_953X)) {
    goto L36342;}
  else {
    addr_954X = ((char *) (-3 + old_953X));
    S48_WRITE_BARRIER(old_953X, addr_954X, 1);
    *((long *) addr_954X) = 1;
    goto L36342;}}
 L36357: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proposal_657X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L8995: {
  i_955X = arg0K0;
  stob_956X = *((long *) ((((char *) (-3 + log_659X))) + (((i_955X)<<2))));
  if ((1 == stob_956X)) {
    log_957X = *((long *) ((((char *) (-3 + proposal_658X))) + 8));
    arg0K0 = 0;
    goto L9233;}
  else {
    verify_958X = *((long *) ((((char *) (-3 + log_659X))) + (12 + (((i_955X)<<2)))));
    value_959X = *((long *) ((((char *) (-3 + log_659X))) + (8 + (((i_955X)<<2)))));
    if ((29 == verify_958X)) {
      if ((3 == (3 & stob_956X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_956X))) + -4)))))) {
          goto L9047;}
        else {
          goto L36588;}}
      else {
        goto L36588;}}
    else {
      if ((verify_958X == (*((long *) ((((char *) (-3 + stob_956X))) + (-4 & (*((long *) ((((char *) (-3 + log_659X))) + (4 + (((i_955X)<<2)))))))))))) {
        if ((verify_958X == value_959X)) {
          goto L9047;}
        else {
          if ((3 == (3 & stob_956X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_956X))) + -4)))))) {
              goto L9047;}
            else {
              goto L36588;}}
          else {
            goto L36588;}}}
      else {
        goto L36588;}}}}
 L27819: {
  value_960X = arg0K0;
  SvalS = value_960X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L27820: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_660X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_662X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_661X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L27913: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg5_668X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_669X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_666X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_670X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_671X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21148;}
 L27973: {
  memcpy((void *)((((char *) (-3 + arg3_666X))) + to_index_670X), (void *)((((char *) (-3 + arg5_668X))) + from_index_669X),count_671X);
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L28061: {
  left_961X = arg0K0;
  copies_962X = arg0K1;
  if ((1 == copies_962X)) {
    if ((left_961X < count_671X)) {
      goto L27913;}
    else {
      from_index_963X = ((from_index_669X)<<2);
      to_index_964X = ((to_index_670X)<<2);
      count_965X = ((count_671X)<<2);
      Stemp0S = arg5_668X;
      Stemp1S = arg3_666X;
      addr_966X = s48_allocate_tracedAgc(28);
      if ((addr_966X == NULL)) {
        arg0K0 = 1;
        goto L9803;}
      else {
        *((long *) addr_966X) = 6154;
        arg0K0 = (3 + (((long) (addr_966X + 4))));
        goto L9803;}}}
  else {
    arg0K0 = (left_961X - ((((*((long *) ((((char *) (-3 + copies_962X))) + 16))))>>2)));
    arg0K1 = (*((long *) ((((char *) (-3 + copies_962X))) + 20)));
    goto L28061;}}
 L28270: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_675X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_676X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L28269: {
  value_967X = arg0K0;
  SvalS = value_967X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L13756: {
  i_968X = arg0K0;
  next_stob_969X = *((long *) ((((char *) (-3 + log_680X))) + (((i_968X)<<2))));
  if ((1 == next_stob_969X)) {
    v_970X = add_log_entryAgc(2, i_968X, arg2_675X, index_679X, ((((*((unsigned char *) ((((char *) (-3 + arg2_675X))) + (((index_679X)>>2))))))<<2)), 1);
    arg0K0 = v_970X;
    goto L28269;}
  else {
    if ((arg2_675X == next_stob_969X)) {
      if ((index_679X == (*((long *) ((((char *) (-3 + log_680X))) + (4 + (((i_968X)<<2)))))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + log_680X))) + (8 + (((i_968X)<<2))))));
        goto L28269;}
      else {
        goto L13778;}}
    else {
      goto L13778;}}}
 L31068: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_675X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L28396: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_682X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_683X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_684X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L28395: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L13932: {
  i_971X = arg0K0;
  next_stob_972X = *((long *) ((((char *) (-3 + log_688X))) + (((i_971X)<<2))));
  if ((1 == next_stob_972X)) {
    add_log_entryAgc(2, i_971X, arg3_682X, index_687X, byte_684X, 0);
    goto L28395;}
  else {
    if ((arg3_682X == next_stob_972X)) {
      if ((index_687X == (*((long *) ((((char *) (-3 + log_688X))) + (4 + (((i_971X)<<2)))))))) {
        addr_973X = (((char *) (-3 + log_688X))) + (8 + (((i_971X)<<2)));
        S48_WRITE_BARRIER(log_688X, addr_973X, byte_684X);
        *((long *) addr_973X) = byte_684X;
        goto L28395;}
      else {
        goto L13952;}}
    else {
      goto L13952;}}}
 L28361: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_682X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_683X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_684X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L31171: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_682X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_681X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L39826: {
  reason_974X = arg0K0;
  status_975X = arg0K1;
  push_exception_continuationB(reason_974X, 1);
  *((long *) (SstackS)) = arg4_692X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_691X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_690X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_975X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 2;
  goto get_error_string;
 get_error_string_return_2:
  x_976X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_976X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L43923: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_692X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_691X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_690X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L44564: {
  i_977X = arg0K0;
  h_978X = arg0K1;
  if ((i_977X < n_704X)) {
    arg0K0 = (1 + i_977X);
    arg0K1 = (h_978X + (((*((unsigned char *) ((((char *) (-3 + x_703X))) + i_977X))))));
    goto L44564;}
  else {
    SvalS = (((h_978X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}}
 L44541: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L40101: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = stob_705X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_706X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L42723: {
  firstP_979X = arg2K0;
  vector_980X = s48_find_all(type_718X);
  if ((1 == vector_980X)) {
    if (firstP_979X) {
      s48_collect();
      arg2K0 = 0;
      goto L42723;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = (((type_718X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}}
  else {
    SvalS = vector_980X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}}
 L46753: {
  firstP_981X = arg2K0;
  type_982X = arg0K1;
  vector_983X = s48_find_all_records(type_982X);
  if ((1 == vector_983X)) {
    if (firstP_981X) {
      Stemp0S = type_982X;
      s48_collect();
      value_984X = Stemp0S;
      Stemp0S = 1;
      arg2K0 = 0;
      arg0K1 = value_984X;
      goto L46753;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = type_982X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}}
  else {
    SvalS = vector_983X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}}
 L25514: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L23978: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L46801: {
  SvalS = (((old_728X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L23066: {
  SstackS = ((SstackS) + 4);
  proposal_985X = *((long *) (SstackS));
  x_986X = Scurrent_threadS;
  addr_987X = (((char *) (-3 + x_986X))) + 12;
  S48_WRITE_BARRIER(x_986X, addr_987X, proposal_985X);
  *((long *) addr_987X) = proposal_985X;
  SstackS = ((SstackS) + 4);
  pc_988X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tem_989X = *((long *) (SstackS));
  StemplateS = tem_989X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_989X)))))))) + (((pc_988X)>>2)));
  SstackS = ((SstackS) + 4);
  SvalS = (*((long *) (SstackS)));
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L45489: {
  x_990X = s48_schedule_alarm_interrupt((((p_731X)>>2)));
  SvalS = (((x_990X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L43473: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L43477;}
  else {
    arg2K0 = 1;
    goto L43477;}}
 L43478: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_732X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L28595: {
  rest_list_991X = arg0K0;
  if ((25 == rest_list_991X)) {
    proc_992X = *((long *) ((SstackS) + (((nargs_734X)<<2))));
    name_993X = *((long *) ((SstackS) + (-4 + (((nargs_734X)<<2)))));
    args_994X = (SstackS) + 4;
    *((long *) (SstackS)) = (10 + (((nargs_734X)<<10)));
    SstackS = ((SstackS) + -4);
    if ((3 == (3 & name_993X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_993X))) + -4))))>>2))))) {
        if ((3 == (3 & proc_992X))) {
          if ((18 == (31 & ((((*((long *) ((((char *) (-3 + proc_992X))) + -4))))>>2))))) {
            if ((4 == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proc_992X))) + -4))))>>8)))) {
              result_995X = s48_external_call(proc_992X, name_993X, (-2 + nargs_734X), args_994X);
              if ((Sexternal_exceptionPS)) {
                Sexternal_exceptionPS = 0;
                arg0K0 = (Sexternal_exception_nargsS);
                goto L21148;}
              else {
                SvalS = result_995X;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23052;}}
            else {
              goto L28861;}}
          else {
            goto L28861;}}
        else {
          goto L28861;}}
      else {
        goto L28861;}}
    else {
      goto L28861;}}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + rest_list_991X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_991X))) + 4)));
    goto L28595;}}
 L42818: {
  if ((1 == (SvalS))) {
    v_996X = Hlookup2550((Sexported_bindingsS), arg2_737X, 0);
    arg0K0 = v_996X;
    goto L42857;}
  else {
    v_997X = Hlookup2531((Simported_bindingsS), arg2_737X, 0);
    arg0K0 = v_997X;
    goto L42857;}}
 L42823: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_737X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L30406: {
  if ((1 == (SvalS))) {
    arg0K0 = (Sexported_bindingsS);
    goto L30439;}
  else {
    arg0K0 = (Simported_bindingsS);
    goto L30439;}}
 L30411: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_739X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L44475: {
  option_998X = arg0K0;
  seconds_999X = arg0K1;
  mseconds_1000X = arg0K2;
  if ((536869 < seconds_999X)) {
    push_exception_continuationB(6, 1);
    *((long *) (SstackS)) = (((option_998X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_999X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_1000X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L21148;}
  else {
    SvalS = (((((1000 * seconds_999X) + mseconds_1000X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}}
 L46861: {
  s48_Scallback_return_stack_blockS = arg2_753X;
  return x_754X;}
 L37370: {
  val_1001X = arg0K0;
  SvalS = val_1001X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L37337: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_755X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L37072: {
  len_1002X = 1 + n_760X;
  addr_1003X = s48_allocate_small((4 + len_1002X));
  *((long *) addr_1003X) = (70 + (((len_1002X)<<8)));
  string_1004X = 3 + (((long) (addr_1003X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1004X))) + n_760X)) = 0;
  arg0K0 = arg2_759X;
  arg0K1 = (-1 + n_760X);
  goto L37049;}
 L37024: {
  if ((25 == arg2_759X)) {
    goto L37072;}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_759X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_760X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}}
 L38839: {
  list_1005X = arg0K0;
  slow_1006X = arg0K1;
  move_slowP_1007X = arg2K2;
  if ((25 == list_1005X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    if ((3 == (3 & list_1005X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_1005X))) + -4))))>>2))))) {
        head_1008X = *((long *) (((char *) (-3 + list_1005X))));
        if ((3 == (3 & head_1008X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_1008X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_1008X))))) == thing_761X)) {
              SvalS = head_1008X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23052;}
            else {
              list_1009X = *((long *) ((((char *) (-3 + list_1005X))) + 4));
              if ((list_1009X == slow_1006X)) {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = thing_761X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = list_762X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21148;}
              else {
                if (move_slowP_1007X) {
                  arg0K0 = list_1009X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_1006X))) + 4)));
                  arg2K2 = 0;
                  goto L38839;}
                else {
                  arg0K0 = list_1009X;
                  arg0K1 = slow_1006X;
                  arg2K2 = 1;
                  goto L38839;}}}}
          else {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = thing_761X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = list_762X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21148;}}
        else {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = thing_761X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = list_762X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L21148;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = thing_761X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_762X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = thing_761X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = list_762X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}}}
 L28937: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = arg3_764X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_763X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_765X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L28927: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg3_764X))) + (((index_765X)<<2)))));
  goto L28936;}
 L28936: {
  value_1010X = arg0K0;
  SvalS = value_1010X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L28957: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg3_764X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_763X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_765X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21148;}
 L29195: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = arg4_771X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_770X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_772X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_773X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L29185: {
  addr_1011X = (((char *) (-3 + arg4_771X))) + (((index_772X)<<2));
  S48_WRITE_BARRIER(arg4_771X, addr_1011X, value_773X);
  *((long *) addr_1011X) = value_773X;
  goto L29194;}
 L29194: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L29217: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg4_771X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_770X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_772X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_773X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21148;}
 L29474: {
  port_1012X = arg0K0;
  if ((3 == (3 & port_1012X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1012X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1012X))) + 4))))>>2))))) {
        goto L29544;}
      else {
        p_1013X = *((long *) ((((char *) (-3 + port_1012X))) + 24));
        p_1014X = *((long *) ((((char *) (-3 + port_1012X))) + 28));
        b_1015X = *((long *) ((((char *) (-3 + port_1012X))) + 20));
        i_1016X = ((p_1013X)>>2);
        x_1017X = *((long *) ((((char *) (-3 + port_1012X))) + 12));
        if ((5 == x_1017X)) {
          goto L29524;}
        else {
          if ((1 == b_1015X)) {
            goto L29524;}
          else {
            if ((i_1016X == (((p_1014X)>>2)))) {
              goto L29524;}
            else {
              val_1018X = 4 + (((i_1016X)<<2));
              addr_1019X = (((char *) (-3 + port_1012X))) + 24;
              S48_WRITE_BARRIER(port_1012X, addr_1019X, val_1018X);
              *((long *) addr_1019X) = val_1018X;
              SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1015X))) + i_1016X))))))<<8)));
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23052;}}}}}
    else {
      goto L29544;}}
  else {
    goto L29544;}}
 L29715: {
  port_1020X = arg0K0;
  if ((3 == (3 & port_1020X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1020X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1020X))) + 4))))>>2))))) {
        goto L29785;}
      else {
        p_1021X = *((long *) ((((char *) (-3 + port_1020X))) + 24));
        p_1022X = *((long *) ((((char *) (-3 + port_1020X))) + 28));
        b_1023X = *((long *) ((((char *) (-3 + port_1020X))) + 20));
        i_1024X = ((p_1021X)>>2);
        x_1025X = *((long *) ((((char *) (-3 + port_1020X))) + 12));
        if ((5 == x_1025X)) {
          goto L29765;}
        else {
          if ((1 == b_1023X)) {
            goto L29765;}
          else {
            if ((i_1024X == (((p_1022X)>>2)))) {
              goto L29765;}
            else {
              SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1023X))) + i_1024X))))))<<8)));
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23052;}}}}}
    else {
      goto L29785;}}
  else {
    goto L29785;}}
 L29949: {
  Kchar_1026X = arg0K0;
  port_1027X = arg0K1;
  if ((9 == (255 & Kchar_1026X))) {
    if ((3 == (3 & port_1027X))) {
      if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1027X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_1027X))) + 4))))>>2))))) {
          goto L30020;}
        else {
          p_1028X = *((long *) ((((char *) (-3 + port_1027X))) + 24));
          b_1029X = *((long *) ((((char *) (-3 + port_1027X))) + 20));
          i_1030X = ((p_1028X)>>2);
          x_1031X = *((long *) ((((char *) (-3 + port_1027X))) + 12));
          if ((5 == x_1031X)) {
            goto L30002;}
          else {
            if ((1 == b_1029X)) {
              goto L30002;}
            else {
              if ((i_1030X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_1029X))) + -4))))>>8)))) {
                goto L30002;}
              else {
                val_1032X = 4 + (((i_1030X)<<2));
                addr_1033X = (((char *) (-3 + port_1027X))) + 24;
                S48_WRITE_BARRIER(port_1027X, addr_1033X, val_1032X);
                *((long *) addr_1033X) = val_1032X;
                *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_1027X))) + 20)))))) + i_1030X)) = (((((Kchar_1026X)>>8))));
                SvalS = 13;
                Scode_pointerS = ((Scode_pointerS) + 2);
                arg1K0 = (Scode_pointerS);
                goto L23052;}}}}}
      else {
        goto L30020;}}
    else {
      goto L30020;}}
  else {
    goto L30020;}}
 L39092: {
  stuff_1034X = arg0K0;
  if ((3 == (3 & stuff_1034X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_1034X))) + -4))))>>2))))) {
      thing_1035X = *((long *) (((char *) (-3 + stuff_1034X))));
      if ((0 == (3 & thing_1035X))) {
        ps_write_integer((((thing_1035X)>>2)), out_784X);
        goto L39098;}
      else {
        if ((9 == (255 & thing_1035X))) {
          ps_write_string("#\\", out_784X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_1035X)>>8))), out_784X, ignoreXX) }
          goto L39098;}
        else {
          if ((3 == (3 & thing_1035X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thing_1035X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_1035X))) + -4))))>>8))))>>2)))) {
                type_1036X = *((long *) (((char *) (-3 + thing_1035X))));
                if ((3 == (3 & type_1036X))) {
                  if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_1036X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_1036X))) + -4))))>>8))))>>2)))) {
                      obj_1037X = *((long *) ((((char *) (-3 + type_1036X))) + 8));
                      if ((3 == (3 & obj_1037X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_1037X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_784X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_1035X)))))))) + 8))))))))))))), out_784X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_784X, ignoreXX) }
                          goto L39098;}
                        else {
                          goto L14641;}}
                      else {
                        goto L14641;}}
                    else {
                      goto L14641;}}
                  else {
                    goto L14641;}}
                else {
                  goto L14641;}}
              else {
                goto L14641;}}
            else {
              goto L14641;}}
          else {
            goto L14641;}}}}
    else {
      goto L39083;}}
  else {
    goto L39083;}}
 L21211: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_787X))) + (((opcode_786X)<<2)))));
  obj_1038X = SvalS;
  if ((3 == (3 & obj_1038X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1038X))) + -4))))>>2))))) {
      goto L21228;}
    else {
      goto L21298;}}
  else {
    goto L21298;}}
 L21284: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L21211;}
 L25748: {
  arg0K0 = (SenvS);
  arg0K1 = 0;
  goto L25707;}
 L26039: {
  i_1039X = arg0K0;
  offset_1040X = arg0K1;
  env_1041X = arg0K2;
  if ((i_1039X == total_count_409X)) {
    SvalS = new_env_412X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1040X));
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    back_1042X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1040X)));
    arg0K0 = env_1041X;
    arg0K1 = back_1042X;
    goto L26117;}}
 L25890: {
  i_1043X = arg0K0;
  offset_1044X = arg0K1;
  env_1045X = arg0K2;
  if ((i_1043X == total_count_416X)) {
    SvalS = new_env_419X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1044X));
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    back_1046X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1044X)));
    arg0K0 = env_1045X;
    arg0K1 = back_1046X;
    goto L25968;}}
 L21381: {
  stack_arg_count_1047X = arg0K0;
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
  x_1048X = Scurrent_threadS;
  addr_1049X = (((char *) (-3 + x_1048X))) + 12;
  S48_WRITE_BARRIER(x_1048X, addr_1049X, 1);
  *((long *) addr_1049X) = 1;
  tem_1050X = Sinterrupt_templateS;
  StemplateS = tem_1050X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1050X))))))));
  push_continuationB((Scode_pointerS), (5 + stack_arg_count_1047X));
  n_1051X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L21515;}
 L23713: {
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L20793: {
  protocol_1052X = arg0K0;
  stack_space_1053X = arg0K1;
  if ((69 == protocol_1052X)) {
    if ((total_arg_count_821X < 3)) {
      skip_1054X = *((unsigned char *) ((((char *) (-3 + code_820X))) + (2 + total_arg_count_821X)));
      if ((0 == skip_1054X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_817X;
        arg0K2 = list_args_818X;
        arg0K3 = list_arg_count_819X;
        goto L21650;}
      else {
        merged_arg0K0 = list_args_818X;
        merged_arg0K1 = list_arg_count_819X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = skip_1054X;
        arg0K1 = total_arg_count_821X;
        goto L20797;}}
    else {
      skip_1055X = *((unsigned char *) ((((char *) (-3 + code_820X))) + 5));
      if ((0 == skip_1055X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_817X;
        arg0K2 = list_args_818X;
        arg0K3 = list_arg_count_819X;
        goto L21650;}
      else {
        arg0K0 = skip_1055X;
        goto L20830;}}}
  else {
    if ((63 < protocol_1052X)) {
      if ((65 == protocol_1052X)) {
        wants_stack_args_1056X = ((((*((unsigned char *) ((((char *) (-3 + code_820X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_820X))) + 3)));
        if ((total_arg_count_821X < wants_stack_args_1056X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_817X;
          arg0K2 = list_args_818X;
          arg0K3 = list_arg_count_819X;
          goto L21650;}
        else {
          merged_arg0K0 = wants_stack_args_1056X;
          merged_arg0K1 = stack_arg_count_817X;
          merged_arg0K2 = list_args_818X;
          merged_arg0K3 = list_arg_count_819X;
          rest_list_setupAgc_return_tag = 0;
          goto rest_list_setupAgc;
         rest_list_setupAgc_return_0:
          arg0K0 = 4;
          arg0K1 = (1 + wants_stack_args_1056X);
          goto L20797;}}
      else {
        if ((68 == protocol_1052X)) {
          if ((total_arg_count_821X < (*((unsigned char *) ((((char *) (-3 + code_820X))) + 2))))) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_817X;
            arg0K2 = list_args_818X;
            arg0K3 = list_arg_count_819X;
            goto L21650;}
          else {
            arg0K0 = 3;
            goto L20830;}}
        else {
          if ((64 == protocol_1052X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_820X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_820X))) + 3)))) == total_arg_count_821X)) {
              if ((0 == list_arg_count_819X)) {
                arg0K0 = 4;
                arg0K1 = total_arg_count_821X;
                goto L20797;}
              else {
                merged_arg0K0 = list_args_818X;
                merged_arg0K1 = list_arg_count_819X;
                push_list_return_tag = 1;
                goto push_list;
               push_list_return_1:
                arg0K0 = 4;
                arg0K1 = total_arg_count_821X;
                goto L20797;}}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_817X;
              arg0K2 = list_args_818X;
              arg0K3 = list_arg_count_819X;
              goto L21650;}}
          else {
            if ((67 == protocol_1052X)) {
              length_1057X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_820X))) + -4))))>>8);
              index_1058X = -2 + length_1057X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_820X))) + (-3 + length_1057X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_820X))) + index_1058X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_820X))) + (1 + index_1058X)))));
              goto L20793;}
            else {
              ps_error("unknown protocol", 1, protocol_1052X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_817X;
              arg0K2 = list_args_818X;
              arg0K3 = list_arg_count_819X;
              goto L21650;}}}}}
    else {
      if ((protocol_1052X == total_arg_count_821X)) {
        if ((0 == list_arg_count_819X)) {
          arg0K0 = 2;
          arg0K1 = total_arg_count_821X;
          goto L20797;}
        else {
          merged_arg0K0 = list_args_818X;
          merged_arg0K1 = list_arg_count_819X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          arg0K0 = 2;
          arg0K1 = total_arg_count_821X;
          goto L20797;}}
      else {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_817X;
        arg0K2 = list_args_818X;
        arg0K3 = list_arg_count_819X;
        goto L21650;}}}}
 L24348: {
  pop_continuationB_return_tag = 4;
  goto pop_continuationB;
 pop_continuationB_return_4:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L21714: {
  cont_1059X = arg0K0;
  if ((1 == cont_1059X)) {
    if ((0 == (3 & (SvalS)))) {
      s48_Scallback_return_stack_blockS = 1;
      return ((((SvalS))>>2));}
    else {
      goto L21729;}}
  else {
    goto L21729;}}
 L24366: {
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  SvalS = (*((long *) ((((char *) (-3 + cont_846X))) + 16)));
  arg0K0 = 1;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L22278;}
 L45732: {
  v_1060X = arg0K0;
  SvalS = v_1060X;
  pop_continuationB_return_tag = 5;
  goto pop_continuationB;
 pop_continuationB_return_5:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L45680: {
  if ((1 == (stack_nargs_859X + list_arg_count_861X))) {
    if ((1 == stack_nargs_859X)) {
      SstackS = ((SstackS) + 4);
      arg0K0 = (*((long *) (SstackS)));
      goto L45773;}
    else {
      arg0K0 = (*((long *) (((char *) (-3 + list_args_860X)))));
      goto L45773;}}
  else {
    merged_arg0K0 = list_args_860X;
    merged_arg0K1 = stack_nargs_859X;
    pop_args_GlistSAgc_return_tag = 8;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_8:
    args_1061X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1061X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}}
 L45688: {
  SvalS = (*((long *) ((((char *) (-3 + cont_862X))) + 16)));
  arg0K0 = stack_nargs_859X;
  arg0K1 = list_args_860X;
  arg0K2 = list_arg_count_861X;
  goto L22278;}
 L25304: {
  loc_1062X = arg1K0;
  arg_1063X = arg1K1;
  if ((arg_top_872X < arg_1063X)) {
    *((long *) loc_1062X) = (*((long *) arg_1063X));
    arg1K0 = (loc_1062X + -4);
    arg1K1 = (arg_1063X + -4);
    goto L25304;}
  else {
    SstackS = loc_1062X;
    if ((count_870X < stack_nargs_859X)) {
      merged_arg0K0 = list_args_860X;
      merged_arg0K1 = (stack_nargs_859X - count_870X);
      pop_args_GlistSAgc_return_tag = 9;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_9:
      v_1064X = pop_args_GlistSAgc0_return_value;
      arg0K0 = v_1064X;
      goto L25283;}
    else {
      arg0K0 = stack_nargs_859X;
      arg0K1 = list_args_860X;
      goto L25265;}}}
 L25156: {
  count_1065X = arg0K0;
  bytes_used_1066X = arg0K1;
  stack_nargs_1067X = arg0K2;
  list_args_1068X = arg0K3;
  list_arg_count_1069X = arg0K4;
  if ((count_1065X == (stack_nargs_1067X + list_arg_count_1069X))) {
    arg_top_1070X = SstackS;
    pop_continuationB_return_tag = 6;
    goto pop_continuationB;
   pop_continuationB_return_6:
    arg1K0 = (SstackS);
    arg1K1 = (arg_top_1070X + (((stack_nargs_1067X)<<2)));
    goto L25188;}
  else {
    merged_arg0K0 = list_args_1068X;
    merged_arg0K1 = stack_nargs_1067X;
    pop_args_GlistSAgc_return_tag = 10;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_10:
    args_1071X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1071X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}}
 L23459: {
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L12952: {
  if ((3 == (3 & x_464X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L46238;}
    else {
      goto L12958;}}
  else {
    goto L12958;}}
 L37924: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L38065: {
  if ((3 == (3 & n_466X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      goto L38076;}
    else {
      goto L38077;}}
  else {
    goto L38077;}}
 L38264: {
  if ((3 == (3 & n_467X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      goto L38275;}
    else {
      goto L38276;}}
  else {
    goto L38276;}}
 L38463: {
  if ((3 == (3 & n_468X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      goto L38474;}
    else {
      goto L38475;}}
  else {
    goto L38475;}}
 L31904: {
  if ((0 == (3 & x_472X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16586;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_472X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16586;}}
 L7809: {
  b_1072X = arg0K0;
  lo_a_1073X = 65535 & a_885X;
  lo_b_1074X = 65535 & b_1072X;
  hi_a_1075X = 65535 & (((a_885X)>>16));
  hi_b_1076X = 65535 & (((b_1072X)>>16));
  lo_c_1077X = SMALL_MULTIPLY(lo_a_1073X, lo_b_1074X);
  v_1078X = SMALL_MULTIPLY(lo_b_1074X, hi_a_1075X);
  v_1079X = SMALL_MULTIPLY(lo_a_1073X, hi_b_1076X);
  mid_c_1080X = v_1079X + v_1078X;
  c_1081X = lo_c_1077X + (((mid_c_1080X)<<16));
  if ((0 < hi_a_1075X)) {
    if ((0 < hi_b_1076X)) {
      arg0K0 = x_475X;
      arg0K1 = y_476X;
      goto L18504;}
    else {
      goto L7851;}}
  else {
    goto L7851;}}
 L40227: {
  arg0K0 = x_475X;
  arg0K1 = y_476X;
  goto L18504;}
 L32095: {
  if ((0 == (3 & x_479X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16543;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_479X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16543;}}
 L8067: {
  b_1082X = arg0K0;
  c_1083X = a_889X / b_1082X;
  x_1084X = 0 == (a_889X % b_1082X);
  if (x_1084X) {
    if ((a_484X < 0)) {
      if ((b_485X < 0)) {
        s48_make_availableAgc(16);
        if ((536870911 < c_1083X)) {
          goto L47622;}
        else {
          if ((c_1083X < -536870912)) {
            goto L47622;}
          else {
            arg0K0 = (((c_1083X)<<2));
            goto L47617;}}}
      else {
        goto L8113;}}
    else {
      if ((b_485X < 0)) {
        goto L8113;}
      else {
        s48_make_availableAgc(16);
        if ((536870911 < c_1083X)) {
          goto L47644;}
        else {
          if ((c_1083X < -536870912)) {
            goto L47644;}
          else {
            arg0K0 = (((c_1083X)<<2));
            goto L47639;}}}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_482X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_483X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}}
 L40382: {
  if ((0 == (3 & x_482X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16111;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_482X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16111;}}
 L32282: {
  if ((0 == (3 & (x_486X | y_487X)))) {
    if ((x_486X == y_487X)) {
      arg0K0 = 5;
      goto L32286;}
    else {
      arg0K0 = 1;
      goto L32286;}}
  else {
    if ((3 == (3 & x_486X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_486X))) + -4))))>>2))))) {
        if ((3 == (3 & y_487X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_487X))) + -4))))>>2))))) {
            b_1085X = s48_bignum_equal_p((((char *) (-3 + x_486X))), (((char *) (-3 + y_487X))));
            if (b_1085X) {
              arg0K0 = 5;
              goto L32286;}
            else {
              arg0K0 = 1;
              goto L32286;}}
          else {
            arg0K0 = 1;
            goto L32286;}}
        else {
          arg0K0 = 1;
          goto L32286;}}
      else {
        arg0K0 = 1;
        goto L32286;}}
    else {
      arg0K0 = 1;
      goto L32286;}}}
 L32459: {
  if ((0 == (3 & x_488X))) {
    if ((0 == (3 & y_489X))) {
      if ((x_488X < y_489X)) {
        arg0K0 = 5;
        goto L32463;}
      else {
        arg0K0 = 1;
        goto L32463;}}
    else {
      v_1086X = s48_bignum_test((((char *) (-3 + y_489X))));
      if ((1 == v_1086X)) {
        arg0K0 = 5;
        goto L32463;}
      else {
        arg0K0 = 1;
        goto L32463;}}}
  else {
    if ((0 == (3 & y_489X))) {
      v_1087X = s48_bignum_test((((char *) (-3 + x_488X))));
      if ((1 == v_1087X)) {
        arg0K0 = 1;
        goto L32463;}
      else {
        arg0K0 = 5;
        goto L32463;}}
    else {
      v_1088X = s48_bignum_compare((((char *) (-3 + x_488X))), (((char *) (-3 + y_489X))));
      if ((-1 == v_1088X)) {
        arg0K0 = 5;
        goto L32463;}
      else {
        arg0K0 = 1;
        goto L32463;}}}}
 L32699: {
  if ((0 == (3 & y_491X))) {
    if ((0 == (3 & x_490X))) {
      if ((y_491X < x_490X)) {
        arg0K0 = 5;
        goto L32703;}
      else {
        arg0K0 = 1;
        goto L32703;}}
    else {
      v_1089X = s48_bignum_test((((char *) (-3 + x_490X))));
      if ((1 == v_1089X)) {
        arg0K0 = 5;
        goto L32703;}
      else {
        arg0K0 = 1;
        goto L32703;}}}
  else {
    if ((0 == (3 & x_490X))) {
      v_1090X = s48_bignum_test((((char *) (-3 + y_491X))));
      if ((1 == v_1090X)) {
        arg0K0 = 1;
        goto L32703;}
      else {
        arg0K0 = 5;
        goto L32703;}}
    else {
      v_1091X = s48_bignum_compare((((char *) (-3 + y_491X))), (((char *) (-3 + x_490X))));
      if ((-1 == v_1091X)) {
        arg0K0 = 5;
        goto L32703;}
      else {
        arg0K0 = 1;
        goto L32703;}}}}
 L32939: {
  if ((0 == (3 & y_493X))) {
    if ((0 == (3 & x_492X))) {
      if ((y_493X < x_492X)) {
        arg0K0 = 1;
        goto L32943;}
      else {
        arg0K0 = 5;
        goto L32943;}}
    else {
      v_1092X = s48_bignum_test((((char *) (-3 + x_492X))));
      if ((1 == v_1092X)) {
        arg0K0 = 1;
        goto L32943;}
      else {
        arg0K0 = 5;
        goto L32943;}}}
  else {
    if ((0 == (3 & x_492X))) {
      v_1093X = s48_bignum_test((((char *) (-3 + y_493X))));
      if ((1 == v_1093X)) {
        arg0K0 = 5;
        goto L32943;}
      else {
        arg0K0 = 1;
        goto L32943;}}
    else {
      v_1094X = s48_bignum_compare((((char *) (-3 + y_493X))), (((char *) (-3 + x_492X))));
      if ((-1 == v_1094X)) {
        arg0K0 = 1;
        goto L32943;}
      else {
        arg0K0 = 5;
        goto L32943;}}}}
 L33133: {
  if ((0 == (3 & x_494X))) {
    if ((0 == (3 & y_495X))) {
      if ((x_494X < y_495X)) {
        arg0K0 = 1;
        goto L33137;}
      else {
        arg0K0 = 5;
        goto L33137;}}
    else {
      v_1095X = s48_bignum_test((((char *) (-3 + y_495X))));
      if ((1 == v_1095X)) {
        arg0K0 = 1;
        goto L33137;}
      else {
        arg0K0 = 5;
        goto L33137;}}}
  else {
    if ((0 == (3 & y_495X))) {
      v_1096X = s48_bignum_test((((char *) (-3 + x_494X))));
      if ((1 == v_1096X)) {
        arg0K0 = 5;
        goto L33137;}
      else {
        arg0K0 = 1;
        goto L33137;}}
    else {
      v_1097X = s48_bignum_compare((((char *) (-3 + x_494X))), (((char *) (-3 + y_495X))));
      if ((-1 == v_1097X)) {
        arg0K0 = 1;
        goto L33137;}
      else {
        arg0K0 = 5;
        goto L33137;}}}}
 L18443: {
  length0_1098X = arg0K0;
  extra0_1099X = arg0K1;
  if ((0 == (3 & y_896X))) {
    arg0K0 = 3;
    goto L18445;}
  else {
    arg0K0 = 0;
    goto L18445;}}
 L8712: {
  b_1100X = arg0K0;
  c_1101X = a_897X / b_1100X;
  if ((a_498X < 0)) {
    if ((b_499X < 0)) {
      goto L8758;}
    else {
      goto L8757;}}
  else {
    if ((b_499X < 0)) {
      goto L8757;}
    else {
      goto L8758;}}}
 L33333: {
  arg0K0 = x_496X;
  arg0K1 = y_497X;
  goto L18413;}
 L18352: {
  length0_1102X = arg0K0;
  extra0_1103X = arg0K1;
  if ((0 == (3 & y_899X))) {
    arg0K0 = 3;
    goto L18354;}
  else {
    arg0K0 = 0;
    goto L18354;}}
 L33543: {
  b_1104X = arg0K0;
  c_1105X = a_900X % b_1104X;
  if ((a_502X < 0)) {
    arg0K0 = (0 - c_1105X);
    goto L33547;}
  else {
    arg0K0 = c_1105X;
    goto L33547;}}
 L33509: {
  arg0K0 = x_500X;
  arg0K1 = y_501X;
  goto L18322;}
 L21927: {
  x_1106X = arg0K0;
  if ((536870911 < x_1106X)) {
    goto L21928;}
  else {
    if ((x_1106X < -536870912)) {
      goto L21928;}
    else {
      return (((x_1106X)<<2));}}}
 L17930: {
  length_1107X = arg0K0;
  extra_1108X = arg0K1;
  Stemp0S = x_902X;
  s48_make_availableAgc(((((length_1107X + extra_1108X))<<2)));
  value_1109X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1109X))) {
    v_1110X = (char *) s48_long_to_bignum((((value_1109X)>>2)));
    arg1K0 = v_1110X;
    goto L17922;}
  else {
    arg1K0 = (((char *) (-3 + value_1109X)));
    goto L17922;}}
 L18277: {
  length_1111X = arg0K0;
  extra_1112X = arg0K1;
  if ((length_1111X < 1)) {
    arg0K0 = 1;
    goto L18279;}
  else {
    arg0K0 = length_1111X;
    goto L18279;}}
 L34166: {
  x_1113X = arg0K0;
  count_1114X = arg0K1;
  if ((0 == x_1113X)) {
    SvalS = (((count_1114X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    arg0K0 = (((x_1113X)>>1));
    arg0K1 = (count_1114X + (1 & x_1113X));
    goto L34166;}}
 L18219: {
  length_1115X = arg0K0;
  extra_1116X = arg0K1;
  if ((length_1115X < 1)) {
    arg0K0 = 1;
    goto L18221;}
  else {
    arg0K0 = length_1115X;
    goto L18221;}}
 L34251: {
  if ((0 == (3 & x_528X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16325;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_528X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16325;}}
 L34414: {
  if ((0 == (3 & x_530X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16282;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_530X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16282;}}
 L34577: {
  if ((0 == (3 & x_532X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16239;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_532X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16239;}}
 L40729: {
  v_1117X = (char *) s48_long_to_bignum(x_909X);
  v_1118X = enter_bignum(v_1117X);
  arg0K0 = v_1118X;
  goto L40724;}
 L40724: {
  val_1119X = arg0K0;
  SvalS = val_1119X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L40751: {
  v_1120X = (char *) s48_long_to_bignum(result_911X);
  v_1121X = enter_bignum(v_1120X);
  arg0K0 = v_1121X;
  goto L40746;}
 L40746: {
  val_1122X = arg0K0;
  SvalS = val_1122X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L40622: {
  x_1123X = arg0K0;
  y_1124X = arg0K1;
  y_1125X = ((y_1124X)>>2);
  merged_arg0K0 = x_1123X;
  merged_arg0K1 = y_1125X;
  shift_space_return_tag = 1;
  goto shift_space;
 shift_space_return_1:
  needed_1126X = shift_space0_return_value;
  Stemp0S = x_1123X;
  s48_make_availableAgc((((needed_1126X)<<2)));
  value_1127X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1127X))) {
    v_1128X = (char *) s48_long_to_bignum((((value_1127X)>>2)));
    arg1K0 = v_1128X;
    goto L40696;}
  else {
    arg1K0 = (((char *) (-3 + value_1127X)));
    goto L40696;}}
 L40773: {
  v_1129X = (char *) s48_long_to_bignum(result_911X);
  v_1130X = enter_bignum(v_1129X);
  arg0K0 = v_1130X;
  goto L40768;}
 L40768: {
  val_1131X = arg0K0;
  SvalS = val_1131X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L40804: {
  x_1132X = arg1K0;
  external_bignum_1133X = (char *) s48_bignum_arithmetic_shift(x_1132X, y_913X);
  v_1134X = s48_bignum_fits_in_word_p(external_bignum_1133X, 30, 1);
  if (v_1134X) {
    n_1135X = s48_bignum_to_long(external_bignum_1133X);
    arg0K0 = (((n_1135X)<<2));
    goto L40630;}
  else {
    v_1136X = enter_bignum(external_bignum_1133X);
    arg0K0 = v_1136X;
    goto L40630;}}
 L40631: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_534X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_535X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L26574: {
  i_1137X = arg0K0;
  rest_list_1138X = arg0K1;
  if ((25 == rest_list_1138X)) {
    SvalS = new_558X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    *((long *) ((((char *) (-3 + new_558X))) + (((i_1137X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_1138X)))));
    arg0K0 = (1 + i_1137X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_1138X))) + 4)));
    goto L26574;}}
 L26987: {
  i_1139X = arg0K0;
  if ((i_1139X < 0)) {
    SvalS = x_925X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    addr_1140X = (((char *) (-3 + x_925X))) + (((i_1139X)<<2));
    S48_WRITE_BARRIER(x_925X, addr_1140X, value_926X);
    *((long *) addr_1140X) = value_926X;
    arg0K0 = (-1 + i_1139X);
    goto L26987;}}
 L34827: {
  i_1141X = arg0K0;
  if ((i_1141X < 0)) {
    SvalS = vector_929X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_929X))) + i_1141X)) = init_591X;
    arg0K0 = (-1 + i_1141X);
    goto L34827;}}
 L35007: {
  i_1142X = arg0K0;
  if ((i_1142X < 0)) {
    SvalS = vector_930X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_930X))) + i_1142X)) = (init_604X);
    arg0K0 = (-1 + i_1142X);
    goto L35007;}}
 L19659: {
  bucket_1143X = arg0K0;
  arg0K0 = bucket_1143X;
  goto L19665;}
 L39191: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L35199: {
  index_1144X = arg0K0;
  channel_1145X = make_registered_channel(mode_628X, arg2_627X, index_1144X, 0, &reason_1146X);
  if ((1 == channel_1145X)) {
    if ((3 == (3 & arg2_627X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_627X))) + -4))))>>2))))) {
        if ((1 == mode_628X)) {
          goto L35314;}
        else {
          if ((3 == mode_628X)) {
            goto L35314;}
          else {
            v_1147X = ps_close_fd(index_1144X);
            arg0K0 = v_1147X;
            goto L35309;}}}
      else {
        push_exception_continuationB(reason_1146X, 1);
        *((long *) (SstackS)) = arg2_627X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_628X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21148;}}
    else {
      push_exception_continuationB(reason_1146X, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}}
  else {
    SvalS = channel_1145X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}}
 L35257: {
  v_1148X = ps_open_fd((((char *)(((char *) (-3 + arg2_627X))))), 1, &v_1149X);
  arg0K0 = v_1148X;
  arg0K1 = v_1149X;
  goto L35270;}
 L35270: {
  channel_1150X = arg0K0;
  status_1151X = arg0K1;
  if ((status_1151X == NO_ERRORS)) {
    arg0K0 = channel_1150X;
    goto L35199;}
  else {
    if ((status_1151X == ENOENT)) {
      push_exception_continuationB(10, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21148;}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1151X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_1152X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1152X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 3;
      goto L21148;}}}
 L42266: {
  waitP_1153X = arg2K0;
  start_1154X = ((arg3_634X)>>2);
  count_1155X = ((arg2_633X)>>2);
  v_1156X = 4 == (*((long *) (((char *) (-3 + arg5_636X)))));
  if (v_1156X) {
    if ((3 == (3 & arg4_635X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>2))))) {
        goto L35641;}
      else {
        goto L35856;}}
    else {
      goto L35856;}}
  else {
    goto L35709;}}
 L36020: {
  y_1157X = arg0K0;
  if ((y_1157X < (start_641X + count_642X))) {
    goto L36072;}
  else {
    got_1158X = ps_write_fd(((((*((long *) ((((char *) (-3 + arg4_640X))) + 8))))>>2)), ((((char *) (-3 + arg3_639X))) + start_641X), count_642X, &pendingP_1159X, &status_1160X);
    if ((status_1160X == NO_ERRORS)) {
      if (pendingP_1159X) {
        arg0K0 = 1;
        goto L36071;}
      else {
        arg0K0 = (((got_1158X)<<2));
        goto L36071;}}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg4_640X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg3_639X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_641X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_642X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1160X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 4;
      goto get_error_string;
     get_error_string_return_4:
      x_1161X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1161X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 5;
      goto L21148;}}}
 L36207: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>8));
  goto L36020;}
 L10585: {
  val_1162X = *((long *) ((((char *) (-3 + ch_943X))) + 12));
  addr_1163X = (((char *) (-3 + prev_944X))) + 12;
  S48_WRITE_BARRIER(prev_944X, addr_1163X, val_1162X);
  *((long *) addr_1163X) = val_1162X;
  addr_1164X = (((char *) (-3 + ch_943X))) + 12;
  S48_WRITE_BARRIER(ch_943X, addr_1164X, 1);
  *((long *) addr_1164X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_943X))) + 16)));
  goto L42604;}
 L15205: {
  v_1165X = arg0K0;
  arg0K0 = (-1 + i_948X);
  arg0K1 = v_1165X;
  goto L15191;}
 L36342: {
  if ((1 == proposal_657X)) {
    goto L36354;}
  else {
    addr_1166X = ((char *) (-3 + proposal_657X));
    S48_WRITE_BARRIER(proposal_657X, addr_1166X, 5);
    *((long *) addr_1166X) = 5;
    goto L36354;}}
 L9233: {
  i_1167X = arg0K0;
  stob_1168X = *((long *) ((((char *) (-3 + log_957X))) + (((i_1167X)<<2))));
  if ((1 == stob_1168X)) {
    copies_1169X = *((long *) ((((char *) (-3 + proposal_658X))) + 12));
    arg0K0 = copies_1169X;
    goto L36688;}
  else {
    verify_1170X = *((long *) ((((char *) (-3 + log_957X))) + (12 + (((i_1167X)<<2)))));
    value_1171X = *((long *) ((((char *) (-3 + log_957X))) + (8 + (((i_1167X)<<2)))));
    if ((29 == verify_1170X)) {
      if ((3 == (3 & stob_1168X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1168X))) + -4)))))) {
          goto L9285;}
        else {
          goto L36588;}}
      else {
        goto L36588;}}
    else {
      if ((verify_1170X == ((((*((unsigned char *) ((((char *) (-3 + stob_1168X))) + ((((*((long *) ((((char *) (-3 + log_957X))) + (4 + (((i_1167X)<<2)))))))>>2))))))<<2)))) {
        if ((verify_1170X == value_1171X)) {
          goto L9285;}
        else {
          if ((3 == (3 & stob_1168X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1168X))) + -4)))))) {
              goto L9285;}
            else {
              goto L36588;}}
          else {
            goto L36588;}}}
      else {
        goto L36588;}}}}
 L9047: {
  arg0K0 = (4 + i_955X);
  goto L8995;}
 L36588: {
  RELEASE_PROPOSAL_LOCK();
  x_1172X = Scurrent_threadS;
  addr_1173X = (((char *) (-3 + x_1172X))) + 12;
  S48_WRITE_BARRIER(x_1172X, addr_1173X, 1);
  *((long *) addr_1173X) = 1;
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L9803: {
  vector_1174X = arg0K0;
  if ((1 == vector_1174X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1175X;
    goto L9766;}
  else {
    arg0K0 = vector_1174X;
    goto L9766;}}
 L13778: {
  arg0K0 = (4 + i_968X);
  goto L13756;}
 L13952: {
  arg0K0 = (4 + i_971X);
  goto L13932;}
 L43477: {
  minutesP_1176X = arg2K0;
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_1177X = check_events0_return_value;
      if (temp_1177X) {
        goto L43503;}
      else {
        goto L43508;}}
    else {
      goto L43503;}}
  else {
    goto L43508;}}
 L28861: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proc_992X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = name_993X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L42857: {
  val_1178X = arg0K0;
  SvalS = val_1178X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L30439: {
  table_1179X = arg0K0;
  n_1180X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_739X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L19172;}
 L37049: {
  l_1181X = arg0K0;
  i_1182X = arg0K1;
  if ((i_1182X < 0)) {
    SvalS = string_1004X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1004X))) + i_1182X)) = ((((((*((long *) (((char *) (-3 + l_1181X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_1181X))) + 4)));
    arg0K1 = (-1 + i_1182X);
    goto L37049;}}
 L29544: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_1012X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L29524: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = port_1012X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L29785: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_1020X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L29765: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = port_1020X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21148;}
 L30020: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = Kchar_1026X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1027X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L30002: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = Kchar_1026X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1027X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L39098: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_1034X))) + 4)));
  goto L39092;}
 L14641: {
  if ((3 == (3 & thing_1035X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_1035X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_1035X)))));
      goto L14683;}
    else {
      goto L14649;}}
  else {
    goto L14649;}}
 L39083: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_784X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L21228: {
  StemplateS = (SvalS);
  Slosing_opcodeS = opcode_786X;
  arg0K0 = (2 + nargs_785X);
  arg0K1 = 25;
  arg0K2 = 0;
  goto L20778;}
 L21298: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L21228;}
 L26117: {
  env_1183X = arg0K0;
  i_1184X = arg0K1;
  if ((0 == i_1184X)) {
    count_1185X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_1040X)));
    arg0K0 = count_1185X;
    arg0K1 = i_1039X;
    arg0K2 = (2 + offset_1040X);
    goto L26056;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1183X)))));
    arg0K1 = (-1 + i_1184X);
    goto L26117;}}
 L25968: {
  env_1186X = arg0K0;
  i_1187X = arg0K1;
  if ((0 == i_1187X)) {
    index_1188X = 1 + offset_1044X;
    count_1189X = ((((*((unsigned char *) ((Scode_pointerS) + (1 + index_1188X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_1188X))));
    arg0K0 = count_1189X;
    arg0K1 = i_1043X;
    arg0K2 = (3 + offset_1044X);
    goto L25907;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1186X)))));
    arg0K1 = (-1 + i_1187X);
    goto L25968;}}
 L21515: {
  i_1190X = arg0K0;
  m_1191X = arg0K1;
  if ((0 == (n_1051X & m_1191X))) {
    arg0K0 = (1 + i_1190X);
    arg0K1 = (((m_1191X)<<1));
    goto L21515;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_1191X));
    if ((i_1190X == 0)) {
      *((long *) (SstackS)) = (Sinterrupted_templateS);
      SstackS = ((SstackS) + -4);
      Sinterrupted_templateS = 1;
      *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21411;}
    else {
      if ((i_1190X == 2)) {
        *((long *) (SstackS)) = (Sfinalize_theseS);
        SstackS = ((SstackS) + -4);
        Sfinalize_theseS = 25;
        *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21411;}
      else {
        if ((i_1190X == 3)) {
          channel_1192X = Spending_channels_headS;
          next_1193X = *((long *) ((((char *) (-3 + channel_1192X))) + 12));
          Spending_channels_headS = next_1193X;
          addr_1194X = (((char *) (-3 + channel_1192X))) + 12;
          S48_WRITE_BARRIER(channel_1192X, addr_1194X, 1);
          *((long *) addr_1194X) = 1;
          if ((1 == next_1193X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_1192X;
            goto L10746;}
          else {
            arg0K0 = channel_1192X;
            goto L10746;}}
        else {
          if ((i_1190X == 4)) {
            *((long *) (SstackS)) = (Sos_signal_typeS);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (Sos_signal_argumentS);
            SstackS = ((SstackS) + -4);
            Sos_signal_typeS = 1;
            Sos_signal_argumentS = 1;
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 3;
            goto L21411;}
          else {
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21411;}}}}}}
 L20797: {
  skip_1195X = arg0K0;
  stack_arg_count_1196X = arg0K1;
  template_1197X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_1197X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_1197X)))))))) + skip_1195X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  if ((stack_space_1053X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L20704;}
  else {
    s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
    merged_arg0K0 = 0;
    copy_stack_into_heap_return_tag = 3;
    goto copy_stack_into_heap;
   copy_stack_into_heap_return_3:
    if ((stack_space_1053X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L20704;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L20704;}}}
 L20830: {
  skip_1198X = arg0K0;
  if ((total_arg_count_821X < 3)) {
    arg0K0 = total_arg_count_821X;
    goto L20838;}
  else {
    if ((2 < stack_arg_count_817X)) {
      arg0K0 = stack_arg_count_817X;
      goto L20838;}
    else {
      arg0K0 = 2;
      goto L20838;}}}
 L21729: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  push_exception_continuationB(5, 0);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_1059X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L45773: {
  v_1199X = arg0K0;
  SvalS = v_1199X;
  arg0K0 = cont_867X;
  goto L21714;}
 L25283: {
  x_1200X = arg0K0;
  *((long *) (SstackS)) = x_1200X;
  SstackS = ((SstackS) + -4);
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L25265: {
  stack_nargs_1201X = arg0K0;
  l_1202X = arg0K1;
  if ((count_870X == stack_nargs_1201X)) {
    arg0K0 = l_1202X;
    goto L25283;}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1202X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (1 + stack_nargs_1201X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1202X))) + 4)));
    goto L25265;}}
 L25188: {
  loc_1203X = arg1K0;
  arg_1204X = arg1K1;
  if ((arg_top_1070X < arg_1204X)) {
    *((long *) loc_1203X) = (*((long *) arg_1204X));
    arg1K0 = (loc_1203X + -4);
    arg1K1 = (arg_1204X + -4);
    goto L25188;}
  else {
    SstackS = loc_1203X;
    if ((0 == list_arg_count_1069X)) {
      goto L25175;}
    else {
      merged_arg0K0 = list_args_1068X;
      merged_arg0K1 = list_arg_count_1069X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      goto L25175;}}}
 L12958: {
  if ((3 == (3 & x_464X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L46238;}
    else {
      arg0K0 = 1;
      goto L46238;}}
  else {
    arg0K0 = 1;
    goto L46238;}}
 L38077: {
  if ((3 == (3 & n_466X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_466X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
    else {
      goto L38081;}}
  else {
    goto L38081;}}
 L38276: {
  if ((3 == (3 & n_467X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_467X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
    else {
      goto L38280;}}
  else {
    goto L38280;}}
 L38475: {
  if ((3 == (3 & n_468X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_468X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21148;}
    else {
      goto L38479;}}
  else {
    goto L38479;}}
 L16586: {
  length0_1205X = arg0K0;
  extra0_1206X = arg0K1;
  if ((0 == (3 & y_473X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16594;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_473X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16594;}}
 L18504: {
  x_1207X = arg0K0;
  y_1208X = arg0K1;
  if ((0 == (3 & x_1207X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16506;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1207X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16506;}}
 L7851: {
  if ((536870911 < lo_c_1077X)) {
    arg0K0 = x_475X;
    arg0K1 = y_476X;
    goto L18504;}
  else {
    if ((lo_c_1077X < 0)) {
      arg0K0 = x_475X;
      arg0K1 = y_476X;
      goto L18504;}
    else {
      if ((8192 < mid_c_1080X)) {
        arg0K0 = x_475X;
        arg0K1 = y_476X;
        goto L18504;}
      else {
        if ((a_477X < 0)) {
          if ((b_478X < 0)) {
            s48_make_availableAgc(16);
            if ((536870911 < c_1081X)) {
              goto L47519;}
            else {
              if ((c_1081X < -536870912)) {
                goto L47519;}
              else {
                arg0K0 = (((c_1081X)<<2));
                goto L47514;}}}
          else {
            goto L7879;}}
        else {
          if ((b_478X < 0)) {
            goto L7879;}
          else {
            s48_make_availableAgc(16);
            if ((536870911 < c_1081X)) {
              goto L47541;}
            else {
              if ((c_1081X < -536870912)) {
                goto L47541;}
              else {
                arg0K0 = (((c_1081X)<<2));
                goto L47536;}}}}}}}}
 L16543: {
  length0_1209X = arg0K0;
  extra0_1210X = arg0K1;
  if ((0 == (3 & y_480X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16551;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_480X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16551;}}
 L47622: {
  v_1211X = (char *) s48_long_to_bignum(c_1083X);
  v_1212X = enter_bignum(v_1211X);
  arg0K0 = v_1212X;
  goto L47617;}
 L47617: {
  val_1213X = arg0K0;
  SvalS = val_1213X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L8113: {
  x_1214X = 0 - c_1083X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1214X)) {
    goto L47600;}
  else {
    if ((x_1214X < -536870912)) {
      goto L47600;}
    else {
      arg0K0 = (((x_1214X)<<2));
      goto L47595;}}}
 L47644: {
  v_1215X = (char *) s48_long_to_bignum(c_1083X);
  v_1216X = enter_bignum(v_1215X);
  arg0K0 = v_1216X;
  goto L47639;}
 L47639: {
  val_1217X = arg0K0;
  SvalS = val_1217X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L16111: {
  length0_1218X = arg0K0;
  extra0_1219X = arg0K1;
  if ((0 == (3 & y_483X))) {
    arg0K0 = 3;
    goto L16119;}
  else {
    arg0K0 = 0;
    goto L16119;}}
 L32286: {
  val_1220X = arg0K0;
  SvalS = val_1220X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32463: {
  val_1221X = arg0K0;
  SvalS = val_1221X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32703: {
  val_1222X = arg0K0;
  SvalS = val_1222X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32943: {
  val_1223X = arg0K0;
  SvalS = val_1223X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L33137: {
  val_1224X = arg0K0;
  SvalS = val_1224X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L18445: {
  extra1_1225X = arg0K0;
  Stemp0S = x_895X;
  Stemp1S = y_896X;
  s48_make_availableAgc(((((((8 + (((length0_1098X)<<1))) + extra0_1099X) + extra1_1225X))<<2)));
  value_1226X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1226X))) {
    v_1227X = (char *) s48_long_to_bignum((((value_1226X)>>2)));
    arg1K0 = v_1227X;
    goto L18431;}
  else {
    arg1K0 = (((char *) (-3 + value_1226X)));
    goto L18431;}}
 L8758: {
  if ((536870911 < c_1101X)) {
    arg0K0 = x_496X;
    arg0K1 = y_497X;
    goto L18413;}
  else {
    SvalS = (((c_1101X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}}
 L8757: {
  SvalS = ((((0 - c_1101X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L18354: {
  extra1_1228X = arg0K0;
  Stemp0S = x_898X;
  Stemp1S = y_899X;
  s48_make_availableAgc(((((((8 + (((length0_1102X)<<1))) + extra0_1103X) + extra1_1228X))<<2)));
  value_1229X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1229X))) {
    v_1230X = (char *) s48_long_to_bignum((((value_1229X)>>2)));
    arg1K0 = v_1230X;
    goto L18340;}
  else {
    arg1K0 = (((char *) (-3 + value_1229X)));
    goto L18340;}}
 L33547: {
  n_1231X = arg0K0;
  SvalS = (((n_1231X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L21928: {
  v_1232X = (char *) s48_long_to_bignum(x_1106X);
  return enter_bignum(v_1232X);}
 L17922: {
  x_1233X = arg1K0;
  v_1234X = s48_bignum_test(x_1233X);
  if ((-1 == v_1234X)) {
    v_1235X = (char *) s48_bignum_negate(x_1233X);
    arg1K0 = v_1235X;
    goto L17924;}
  else {
    arg1K0 = x_1233X;
    goto L17924;}}
 L18279: {
  v_1236X = arg0K0;
  Stemp0S = x_525X;
  s48_make_availableAgc((((((3 + v_1236X) + extra_1112X))<<2)));
  value_1237X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1237X))) {
    v_1238X = (char *) s48_long_to_bignum((((value_1237X)>>2)));
    arg1K0 = v_1238X;
    goto L18269;}
  else {
    arg1K0 = (((char *) (-3 + value_1237X)));
    goto L18269;}}
 L18221: {
  v_1239X = arg0K0;
  Stemp0S = x_526X;
  s48_make_availableAgc((((((3 + v_1239X) + extra_1116X))<<2)));
  value_1240X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1240X))) {
    v_1241X = (char *) s48_long_to_bignum((((value_1240X)>>2)));
    arg1K0 = v_1241X;
    goto L18211;}
  else {
    arg1K0 = (((char *) (-3 + value_1240X)));
    goto L18211;}}
 L16325: {
  length0_1242X = arg0K0;
  extra0_1243X = arg0K1;
  if ((0 == (3 & y_529X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16333;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_529X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16333;}}
 L16282: {
  length0_1244X = arg0K0;
  extra0_1245X = arg0K1;
  if ((0 == (3 & y_531X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16290;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_531X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16290;}}
 L16239: {
  length0_1246X = arg0K0;
  extra0_1247X = arg0K1;
  if ((0 == (3 & y_533X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16247;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_533X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16247;}}
 L40696: {
  x_1248X = arg1K0;
  external_bignum_1249X = (char *) s48_bignum_arithmetic_shift(x_1248X, y_1125X);
  v_1250X = s48_bignum_fits_in_word_p(external_bignum_1249X, 30, 1);
  if (v_1250X) {
    n_1251X = s48_bignum_to_long(external_bignum_1249X);
    arg0K0 = (((n_1251X)<<2));
    goto L40624;}
  else {
    v_1252X = enter_bignum(external_bignum_1249X);
    arg0K0 = v_1252X;
    goto L40624;}}
 L40630: {
  val_1253X = arg0K0;
  SvalS = val_1253X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L19665: {
  foo_1254X = arg0K0;
  if ((1 == foo_1254X)) {
    if ((3 == (3 & bucket_1143X))) {
      arg0K0 = (-4 & bucket_1143X);
      goto L19670;}
    else {
      arg0K0 = bucket_1143X;
      goto L19670;}}
  else {
    s2_1255X = *((long *) (((char *) (-3 + foo_1254X))));
    len_1256X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_618X))) + -4))))>>8);
    if ((len_1256X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1255X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1255X))), (void *)(((char *) (-3 + string_618X))),len_1256X)))) {
        arg0K0 = foo_1254X;
        goto L31785;}
      else {
        goto L19685;}}
    else {
      goto L19685;}}}
 L35314: {
  v_1257X = ps_close_fd(index_1144X);
  arg0K0 = v_1257X;
  goto L35309;}
 L35309: {
  status_1258X = arg0K0;
  if ((status_1258X == NO_ERRORS)) {
    push_exception_continuationB(reason_1146X, 1);
    *((long *) (SstackS)) = arg2_627X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_628X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}
  else {
    channel_close_error(status_1258X, index_1144X, arg2_627X);
    push_exception_continuationB(reason_1146X, 1);
    *((long *) (SstackS)) = arg2_627X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_628X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21148;}}
 L35641: {
  if ((3 == (3 & arg4_635X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_635X))) + -4)))))) {
      if ((3 == (3 & arg4_635X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>2))))) {
          arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>8)));
          goto L35648;}
        else {
          goto L35882;}}
      else {
        goto L35882;}}
    else {
      goto L35709;}}
  else {
    goto L35709;}}
 L35856: {
  if ((3 == (3 & arg4_635X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>2))))) {
      goto L35641;}
    else {
      goto L35709;}}
  else {
    goto L35709;}}
 L35709: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_636X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_635X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1154X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1155X)<<2));
  SstackS = ((SstackS) + -4);
  if (waitP_1153X) {
    arg0K0 = 5;
    goto L35725;}
  else {
    arg0K0 = 1;
    goto L35725;}}
 L36071: {
  val_1259X = arg0K0;
  SvalS = val_1259X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L36354: {
  x_1260X = Scurrent_threadS;
  addr_1261X = (((char *) (-3 + x_1260X))) + 12;
  S48_WRITE_BARRIER(x_1260X, addr_1261X, proposal_657X);
  *((long *) addr_1261X) = proposal_657X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L36688: {
  copies_1262X = arg0K0;
  if ((1 == copies_1262X)) {
    log_1263X = *((long *) ((((char *) (-3 + proposal_658X))) + 4));
    arg0K0 = 0;
    goto L8537;}
  else {
    thing_1264X = *((long *) ((((char *) (-3 + copies_1262X))) + 8));
    if ((3 == (3 & thing_1264X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + thing_1264X))) + -4)))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + copies_1262X))) + 20)));
        goto L36688;}
      else {
        goto L36588;}}
    else {
      goto L36588;}}}
 L9285: {
  arg0K0 = (4 + i_1167X);
  goto L9233;}
 L9766: {
  entry_1265X = arg0K0;
  proposal_1266X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  value_1267X = Stemp0S;
  Stemp0S = 1;
  addr_1268X = ((char *) (-3 + entry_1265X));
  S48_WRITE_BARRIER(entry_1265X, addr_1268X, value_1267X);
  *((long *) addr_1268X) = value_1267X;
  addr_1269X = (((char *) (-3 + entry_1265X))) + 4;
  S48_WRITE_BARRIER(entry_1265X, addr_1269X, from_index_963X);
  *((long *) addr_1269X) = from_index_963X;
  value_1270X = Stemp1S;
  Stemp1S = 1;
  addr_1271X = (((char *) (-3 + entry_1265X))) + 8;
  S48_WRITE_BARRIER(entry_1265X, addr_1271X, value_1270X);
  *((long *) addr_1271X) = value_1270X;
  addr_1272X = (((char *) (-3 + entry_1265X))) + 12;
  S48_WRITE_BARRIER(entry_1265X, addr_1272X, to_index_964X);
  *((long *) addr_1272X) = to_index_964X;
  addr_1273X = (((char *) (-3 + entry_1265X))) + 16;
  S48_WRITE_BARRIER(entry_1265X, addr_1273X, count_965X);
  *((long *) addr_1273X) = count_965X;
  value_1274X = *((long *) ((((char *) (-3 + proposal_1266X))) + 12));
  addr_1275X = (((char *) (-3 + entry_1265X))) + 20;
  S48_WRITE_BARRIER(entry_1265X, addr_1275X, value_1274X);
  *((long *) addr_1275X) = value_1274X;
  addr_1276X = (((char *) (-3 + proposal_1266X))) + 12;
  S48_WRITE_BARRIER(proposal_1266X, addr_1276X, entry_1265X);
  *((long *) addr_1276X) = entry_1265X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L43503: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L43508: {
  if ((0 == (Spending_interruptsS))) {
    s48_wait_for_event((((arg2_732X)>>2)), minutesP_1176X);
    goto L43503;}
  else {
    goto L43503;}}
 L19172: {
  i_1277X = arg0K0;
  h_1278X = arg0K1;
  if ((i_1277X < n_1180X)) {
    arg0K0 = (1 + i_1277X);
    arg0K1 = (h_1278X + (((*((unsigned char *) ((((char *) (-3 + arg2_739X))) + i_1277X))))));
    goto L19172;}
  else {
    index_1279X = 1023 & h_1278X;
    link_1280X = *((long *) ((((char *) (-3 + table_1179X))) + (((index_1279X)<<2))));
    if ((0 == (3 & link_1280X))) {
      arg0K0 = (3 + (-4 & link_1280X));
      goto L19127;}
    else {
      arg0K0 = link_1280X;
      goto L19127;}}}
 L14683: {
  v_1281X = arg3K0;
  ps_write_string(v_1281X, out_784X);
  goto L39098;}
 L14649: {
  if ((3 == (3 & thing_1035X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_1035X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_1035X))))))))));
      goto L14683;}
    else {
      goto L14657;}}
  else {
    goto L14657;}}
 L26056: {
  count_1282X = arg0K0;
  i_1283X = arg0K1;
  offset_1284X = arg0K2;
  if ((0 == count_1282X)) {
    arg0K0 = i_1283X;
    arg0K1 = offset_1284X;
    arg0K2 = env_1183X;
    goto L26039;}
  else {
    value_1285X = *((long *) ((((char *) (-3 + env_1183X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1284X)))))<<2))));
    addr_1286X = (((char *) (-3 + new_env_412X))) + (((i_1283X)<<2));
    S48_WRITE_BARRIER(new_env_412X, addr_1286X, value_1285X);
    *((long *) addr_1286X) = value_1285X;
    arg0K0 = (-1 + count_1282X);
    arg0K1 = (1 + i_1283X);
    arg0K2 = (1 + offset_1284X);
    goto L26056;}}
 L25907: {
  count_1287X = arg0K0;
  i_1288X = arg0K1;
  offset_1289X = arg0K2;
  if ((0 == count_1287X)) {
    arg0K0 = i_1288X;
    arg0K1 = offset_1289X;
    arg0K2 = env_1186X;
    goto L25890;}
  else {
    value_1290X = *((long *) ((((char *) (-3 + env_1186X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1289X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1289X))))))<<2))));
    addr_1291X = (((char *) (-3 + new_env_419X))) + (((i_1288X)<<2));
    S48_WRITE_BARRIER(new_env_419X, addr_1291X, value_1290X);
    *((long *) addr_1291X) = value_1290X;
    arg0K0 = (-1 + count_1287X);
    arg0K1 = (1 + i_1288X);
    arg0K2 = (2 + offset_1289X);
    goto L25907;}}
 L21411: {
  arg_count_1292X = arg0K0;
  handlers_1293X = SHARED_REF((Sinterrupt_handlersS));
  if ((3 == (3 & handlers_1293X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_1293X))) + -4))))>>2))))) {
      goto L21429;}
    else {
      goto L21528;}}
  else {
    goto L21528;}}
 L10746: {
  channel_1294X = arg0K0;
  x_1295X = 1 == (Spending_channels_headS);
  if (x_1295X) {
    goto L10760;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L10760;}
      else {
        goto L10760;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L10760;}}}
 L20704: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 3;
      goto check_events;
     check_events_return_3:
      v_1296X = check_events0_return_value;
      if (v_1296X) {
        arg0K0 = stack_arg_count_1196X;
        goto L21381;}
      else {
        goto L20708;}}
    else {
      arg0K0 = stack_arg_count_1196X;
      goto L21381;}}
  else {
    goto L20708;}}
 L20838: {
  final_stack_arg_count_1297X = arg0K0;
  if ((stack_arg_count_817X < final_stack_arg_count_1297X)) {
    arg0K0 = final_stack_arg_count_1297X;
    goto L20842;}
  else {
    arg0K0 = stack_arg_count_817X;
    goto L20842;}}
 L25175: {
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1066X));
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L38081: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L38280: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L38479: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L16594: {
  length1_1298X = arg0K0;
  extra1_1299X = arg0K1;
  if ((length0_1205X < length1_1298X)) {
    arg0K0 = length1_1298X;
    goto L16616;}
  else {
    arg0K0 = length0_1205X;
    goto L16616;}}
 L16506: {
  length0_1300X = arg0K0;
  extra0_1301X = arg0K1;
  if ((0 == (3 & y_1208X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16514;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_1208X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16514;}}
 L47519: {
  v_1302X = (char *) s48_long_to_bignum(c_1081X);
  v_1303X = enter_bignum(v_1302X);
  arg0K0 = v_1303X;
  goto L47514;}
 L47514: {
  val_1304X = arg0K0;
  SvalS = val_1304X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L7879: {
  x_1305X = 0 - c_1081X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1305X)) {
    goto L47497;}
  else {
    if ((x_1305X < -536870912)) {
      goto L47497;}
    else {
      arg0K0 = (((x_1305X)<<2));
      goto L47492;}}}
 L47541: {
  v_1306X = (char *) s48_long_to_bignum(c_1081X);
  v_1307X = enter_bignum(v_1306X);
  arg0K0 = v_1307X;
  goto L47536;}
 L47536: {
  val_1308X = arg0K0;
  SvalS = val_1308X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L16551: {
  length1_1309X = arg0K0;
  extra1_1310X = arg0K1;
  if ((length0_1209X < length1_1309X)) {
    arg0K0 = length1_1309X;
    goto L16573;}
  else {
    arg0K0 = length0_1209X;
    goto L16573;}}
 L47600: {
  v_1311X = (char *) s48_long_to_bignum(x_1214X);
  v_1312X = enter_bignum(v_1311X);
  arg0K0 = v_1312X;
  goto L47595;}
 L47595: {
  val_1313X = arg0K0;
  SvalS = val_1313X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L16119: {
  extra1_1314X = arg0K0;
  Stemp0S = x_482X;
  Stemp1S = y_483X;
  s48_make_availableAgc(((((((8 + (((length0_1218X)<<1))) + extra0_1219X) + extra1_1314X))<<2)));
  value_1315X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1315X))) {
    v_1316X = (char *) s48_long_to_bignum((((value_1315X)>>2)));
    arg1K0 = v_1316X;
    goto L17758;}
  else {
    arg1K0 = (((char *) (-3 + value_1315X)));
    goto L17758;}}
 L18431: {
  y_1317X = arg1K0;
  value_1318X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1318X))) {
    v_1319X = (char *) s48_long_to_bignum((((value_1318X)>>2)));
    arg1K0 = v_1319X;
    goto L18427;}
  else {
    arg1K0 = (((char *) (-3 + value_1318X)));
    goto L18427;}}
 L18340: {
  y_1320X = arg1K0;
  value_1321X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1321X))) {
    v_1322X = (char *) s48_long_to_bignum((((value_1321X)>>2)));
    arg1K0 = v_1322X;
    goto L18336;}
  else {
    arg1K0 = (((char *) (-3 + value_1321X)));
    goto L18336;}}
 L17924: {
  external_bignum_1323X = arg1K0;
  v_1324X = s48_bignum_fits_in_word_p(external_bignum_1323X, 30, 1);
  if (v_1324X) {
    n_1325X = s48_bignum_to_long(external_bignum_1323X);
    return (((n_1325X)<<2));}
  else {
    return enter_bignum(external_bignum_1323X);}}
 L18269: {
  v_1326X = arg1K0;
  external_bignum_1327X = (char *) s48_bignum_bitwise_not(v_1326X);
  v_1328X = s48_bignum_fits_in_word_p(external_bignum_1327X, 30, 1);
  if (v_1328X) {
    n_1329X = s48_bignum_to_long(external_bignum_1327X);
    arg0K0 = (((n_1329X)<<2));
    goto L34071;}
  else {
    val_1330X = enter_bignum(external_bignum_1327X);
    arg0K0 = val_1330X;
    goto L34071;}}
 L18211: {
  v_1331X = arg1K0;
  n_1332X = s48_bignum_bit_count(v_1331X);
  SvalS = (((n_1332X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L16333: {
  length1_1333X = arg0K0;
  extra1_1334X = arg0K1;
  if ((length0_1242X < length1_1333X)) {
    arg0K0 = length1_1333X;
    goto L16355;}
  else {
    arg0K0 = length0_1242X;
    goto L16355;}}
 L16290: {
  length1_1335X = arg0K0;
  extra1_1336X = arg0K1;
  if ((length0_1244X < length1_1335X)) {
    arg0K0 = length1_1335X;
    goto L16312;}
  else {
    arg0K0 = length0_1244X;
    goto L16312;}}
 L16247: {
  length1_1337X = arg0K0;
  extra1_1338X = arg0K1;
  if ((length0_1246X < length1_1337X)) {
    arg0K0 = length1_1337X;
    goto L16269;}
  else {
    arg0K0 = length0_1246X;
    goto L16269;}}
 L40624: {
  val_1339X = arg0K0;
  SvalS = val_1339X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L19670: {
  b_1340X = arg0K0;
  addr_1341X = s48_allocate_small(12);
  *((long *) addr_1341X) = 2054;
  x_1342X = 3 + (((long) (addr_1341X + 4)));
  *((long *) (((char *) (-3 + x_1342X)))) = string_618X;
  *((long *) ((((char *) (-3 + x_1342X))) + 4)) = b_1340X;
  if ((3 == (3 & x_1342X))) {
    arg0K0 = (-4 & x_1342X);
    goto L19676;}
  else {
    arg0K0 = x_1342X;
    goto L19676;}}
 L31785: {
  val_1343X = arg0K0;
  SvalS = val_1343X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L19685: {
  link_1344X = *((long *) ((((char *) (-3 + foo_1254X))) + 4));
  if ((0 == (3 & link_1344X))) {
    arg0K0 = (3 + (-4 & link_1344X));
    goto L19665;}
  else {
    arg0K0 = link_1344X;
    goto L19665;}}
 L35648: {
  y_1345X = arg0K0;
  if ((y_1345X < (start_1154X + count_1155X))) {
    goto L35709;}
  else {
    got_1346X = ps_read_fd(((((*((long *) ((((char *) (-3 + arg5_636X))) + 8))))>>2)), ((((char *) (-3 + arg4_635X))) + start_1154X), count_1155X, waitP_1153X, &eofP_1347X, &pendingP_1348X, &status_1349X);
    if ((status_1349X == NO_ERRORS)) {
      if (eofP_1347X) {
        arg0K0 = 21;
        goto L35708;}
      else {
        if (pendingP_1348X) {
          arg0K0 = 1;
          goto L35708;}
        else {
          arg0K0 = (((got_1346X)<<2));
          goto L35708;}}}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg5_636X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_635X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1154X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1155X)<<2));
      SstackS = ((SstackS) + -4);
      if (waitP_1153X) {
        arg0K0 = 5;
        goto L35691;}
      else {
        arg0K0 = 1;
        goto L35691;}}}}
 L35882: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>8));
  goto L35648;}
 L35725: {
  x_1350X = arg0K0;
  *((long *) (SstackS)) = x_1350X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21148;}
 L8537: {
  i_1351X = arg0K0;
  stob_1352X = *((long *) ((((char *) (-3 + log_1263X))) + (((i_1351X)<<2))));
  if ((1 == stob_1352X)) {
    log_1353X = *((long *) ((((char *) (-3 + proposal_658X))) + 8));
    arg0K0 = 0;
    goto L11413;}
  else {
    verify_1354X = *((long *) ((((char *) (-3 + log_1263X))) + (12 + (((i_1351X)<<2)))));
    value_1355X = *((long *) ((((char *) (-3 + log_1263X))) + (8 + (((i_1351X)<<2)))));
    if ((verify_1354X == value_1355X)) {
      goto L8569;}
    else {
      addr_1356X = (((char *) (-3 + stob_1352X))) + (-4 & (*((long *) ((((char *) (-3 + log_1263X))) + (4 + (((i_1351X)<<2)))))));
      S48_WRITE_BARRIER(stob_1352X, addr_1356X, value_1355X);
      *((long *) addr_1356X) = value_1355X;
      goto L8569;}}}
 L19127: {
  bucket_1357X = arg0K0;
  arg0K0 = 1;
  arg0K1 = bucket_1357X;
  goto L19134;}
 L14657: {
  if ((1 == thing_1035X)) {
    goto L14660;}
  else {
    if ((5 == thing_1035X)) {
      goto L14660;}
    else {
      if ((25 == thing_1035X)) {
        arg3K0 = "()";
        goto L14683;}
      else {
        if ((3 == (3 & thing_1035X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_1035X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L14683;}
          else {
            goto L14673;}}
        else {
          goto L14673;}}}}}
 L21429: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Spending_interruptPS = 0;
    if ((s48_Spending_eventsPS)) {
      s48_Spending_interruptPS = 1;
      goto L21431;}
    else {
      goto L21431;}}
  else {
    s48_Spending_interruptPS = 1;
    goto L21431;}}
 L21528: {
  ps_error("interrupt handler is not a vector", 0);
  goto L21429;}
 L10760: {
  *((long *) (SstackS)) = channel_1294X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + channel_1294X))) + 16)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21411;}
 L20708: {
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L20842: {
  v_1358X = arg0K0;
  merged_arg0K0 = v_1358X;
  merged_arg0K1 = stack_arg_count_817X;
  merged_arg0K2 = list_args_818X;
  merged_arg0K3 = list_arg_count_819X;
  rest_list_setupAgc_return_tag = 1;
  goto rest_list_setupAgc;
 rest_list_setupAgc_return_1:
  *((long *) (SstackS)) = (((final_stack_arg_count_1297X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_821X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_1198X;
  arg0K1 = (3 + final_stack_arg_count_1297X);
  goto L20797;}
 L16616: {
  v_1359X = arg0K0;
  Stemp0S = x_472X;
  Stemp1S = y_473X;
  s48_make_availableAgc(((((((3 + v_1359X) + extra0_1206X) + extra1_1299X))<<2)));
  value_1360X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1360X))) {
    v_1361X = (char *) s48_long_to_bignum((((value_1360X)>>2)));
    arg1K0 = v_1361X;
    goto L18670;}
  else {
    arg1K0 = (((char *) (-3 + value_1360X)));
    goto L18670;}}
 L16514: {
  length1_1362X = arg0K0;
  extra1_1363X = arg0K1;
  Stemp0S = x_1207X;
  Stemp1S = y_1208X;
  s48_make_availableAgc(((((((2 + (length0_1300X + length1_1362X)) + extra0_1301X) + extra1_1363X))<<2)));
  value_1364X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1364X))) {
    v_1365X = (char *) s48_long_to_bignum((((value_1364X)>>2)));
    arg1K0 = v_1365X;
    goto L18522;}
  else {
    arg1K0 = (((char *) (-3 + value_1364X)));
    goto L18522;}}
 L47497: {
  v_1366X = (char *) s48_long_to_bignum(x_1305X);
  v_1367X = enter_bignum(v_1366X);
  arg0K0 = v_1367X;
  goto L47492;}
 L47492: {
  val_1368X = arg0K0;
  SvalS = val_1368X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L16573: {
  v_1369X = arg0K0;
  Stemp0S = x_479X;
  Stemp1S = y_480X;
  s48_make_availableAgc(((((((3 + v_1369X) + extra0_1210X) + extra1_1310X))<<2)));
  value_1370X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1370X))) {
    v_1371X = (char *) s48_long_to_bignum((((value_1370X)>>2)));
    arg1K0 = v_1371X;
    goto L18596;}
  else {
    arg1K0 = (((char *) (-3 + value_1370X)));
    goto L18596;}}
 L17758: {
  y_1372X = arg1K0;
  value_1373X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1373X))) {
    v_1374X = (char *) s48_long_to_bignum((((value_1373X)>>2)));
    arg1K0 = v_1374X;
    goto L17754;}
  else {
    arg1K0 = (((char *) (-3 + value_1373X)));
    goto L17754;}}
 L18427: {
  x_1375X = arg1K0;
  external_bignum_1376X = (char *)s48_bignum_quotient(x_1375X, y_1317X);
  v_1377X = s48_bignum_fits_in_word_p(external_bignum_1376X, 30, 1);
  if (v_1377X) {
    n_1378X = s48_bignum_to_long(external_bignum_1376X);
    arg0K0 = (((n_1378X)<<2));
    goto L48301;}
  else {
    val_1379X = enter_bignum(external_bignum_1376X);
    arg0K0 = val_1379X;
    goto L48301;}}
 L18336: {
  x_1380X = arg1K0;
  external_bignum_1381X = (char *)s48_bignum_remainder(x_1380X, y_1320X);
  v_1382X = s48_bignum_fits_in_word_p(external_bignum_1381X, 30, 1);
  if (v_1382X) {
    n_1383X = s48_bignum_to_long(external_bignum_1381X);
    arg0K0 = (((n_1383X)<<2));
    goto L48301;}
  else {
    val_1384X = enter_bignum(external_bignum_1381X);
    arg0K0 = val_1384X;
    goto L48301;}}
 L34071: {
  val_1385X = arg0K0;
  SvalS = val_1385X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L16355: {
  v_1386X = arg0K0;
  Stemp0S = x_528X;
  Stemp1S = y_529X;
  s48_make_availableAgc(((((((3 + v_1386X) + extra0_1243X) + extra1_1334X))<<2)));
  value_1387X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1387X))) {
    v_1388X = (char *) s48_long_to_bignum((((value_1387X)>>2)));
    arg1K0 = v_1388X;
    goto L18144;}
  else {
    arg1K0 = (((char *) (-3 + value_1387X)));
    goto L18144;}}
 L16312: {
  v_1389X = arg0K0;
  Stemp0S = x_530X;
  Stemp1S = y_531X;
  s48_make_availableAgc(((((((3 + v_1389X) + extra0_1245X) + extra1_1336X))<<2)));
  value_1390X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1390X))) {
    v_1391X = (char *) s48_long_to_bignum((((value_1390X)>>2)));
    arg1K0 = v_1391X;
    goto L18070;}
  else {
    arg1K0 = (((char *) (-3 + value_1390X)));
    goto L18070;}}
 L16269: {
  v_1392X = arg0K0;
  Stemp0S = x_532X;
  Stemp1S = y_533X;
  s48_make_availableAgc(((((((3 + v_1392X) + extra0_1247X) + extra1_1338X))<<2)));
  value_1393X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1393X))) {
    v_1394X = (char *) s48_long_to_bignum((((value_1393X)>>2)));
    arg1K0 = v_1394X;
    goto L17996;}
  else {
    arg1K0 = (((char *) (-3 + value_1393X)));
    goto L17996;}}
 L19676: {
  value_1395X = arg0K0;
  addr_1396X = (((char *) (-3 + table_617X))) + (((index_933X)<<2));
  S48_WRITE_BARRIER(table_617X, addr_1396X, value_1395X);
  *((long *) addr_1396X) = value_1395X;
  arg0K0 = x_1342X;
  goto L31785;}
 L35708: {
  val_1397X = arg0K0;
  SvalS = val_1397X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L35691: {
  x_1398X = arg0K0;
  *((long *) (SstackS)) = x_1398X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_1349X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 5;
  goto get_error_string;
 get_error_string_return_5:
  x_1399X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_1399X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 6;
  goto L21148;}
 L11413: {
  i_1400X = arg0K0;
  stob_1401X = *((long *) ((((char *) (-3 + log_1353X))) + (((i_1400X)<<2))));
  if ((1 == stob_1401X)) {
    copies_1402X = *((long *) ((((char *) (-3 + proposal_658X))) + 12));
    arg0K0 = copies_1402X;
    goto L11328;}
  else {
    verify_1403X = *((long *) ((((char *) (-3 + log_1353X))) + (12 + (((i_1400X)<<2)))));
    value_1404X = *((long *) ((((char *) (-3 + log_1353X))) + (8 + (((i_1400X)<<2)))));
    if ((verify_1403X == value_1404X)) {
      goto L11445;}
    else {
      *((unsigned char *) ((((char *) (-3 + stob_1401X))) + ((((*((long *) ((((char *) (-3 + log_1353X))) + (4 + (((i_1400X)<<2)))))))>>2)))) = (((value_1404X)>>2));
      goto L11445;}}}
 L8569: {
  arg0K0 = (4 + i_1351X);
  goto L8537;}
 L19134: {
  previous_foo_1405X = arg0K0;
  foo_1406X = arg0K1;
  if ((1 == foo_1406X)) {
    goto L30441;}
  else {
    s2_1407X = *((long *) (((char *) (-3 + foo_1406X))));
    len_1408X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_739X))) + -4))))>>8);
    if ((len_1408X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1407X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1407X))), (void *)(((char *) (-3 + arg2_739X))),len_1408X)))) {
        if ((1 == previous_foo_1405X)) {
          value_1409X = *((long *) ((((char *) (-3 + foo_1406X))) + 12));
          addr_1410X = (((char *) (-3 + table_1179X))) + (((index_1279X)<<2));
          S48_WRITE_BARRIER(table_1179X, addr_1410X, value_1409X);
          *((long *) addr_1410X) = value_1409X;
          goto L30441;}
        else {
          val_1411X = *((long *) ((((char *) (-3 + foo_1406X))) + 12));
          addr_1412X = (((char *) (-3 + previous_foo_1405X))) + 12;
          S48_WRITE_BARRIER(previous_foo_1405X, addr_1412X, val_1411X);
          *((long *) addr_1412X) = val_1411X;
          goto L30441;}}
      else {
        goto L19202;}}
    else {
      goto L19202;}}}
 L14660: {
  if ((1 == thing_1035X)) {
    arg3K0 = "#f";
    goto L14683;}
  else {
    arg3K0 = "#t";
    goto L14683;}}
 L14673: {
  if ((3 == (3 & thing_1035X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_1035X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L14683;}
    else {
      goto L14677;}}
  else {
    goto L14677;}}
 L21431: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_1293X))) + (((i_1190X)<<2)))));
  obj_1413X = SvalS;
  if ((3 == (3 & obj_1413X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1413X))) + -4))))>>2))))) {
      goto L21448;}
    else {
      goto L21553;}}
  else {
    goto L21553;}}
 L18670: {
  y_1414X = arg1K0;
  value_1415X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1415X))) {
    v_1416X = (char *) s48_long_to_bignum((((value_1415X)>>2)));
    arg1K0 = v_1416X;
    goto L18666;}
  else {
    arg1K0 = (((char *) (-3 + value_1415X)));
    goto L18666;}}
 L18522: {
  y_1417X = arg1K0;
  value_1418X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1418X))) {
    v_1419X = (char *) s48_long_to_bignum((((value_1418X)>>2)));
    arg1K0 = v_1419X;
    goto L18518;}
  else {
    arg1K0 = (((char *) (-3 + value_1418X)));
    goto L18518;}}
 L18596: {
  y_1420X = arg1K0;
  value_1421X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1421X))) {
    v_1422X = (char *) s48_long_to_bignum((((value_1421X)>>2)));
    arg1K0 = v_1422X;
    goto L18592;}
  else {
    arg1K0 = (((char *) (-3 + value_1421X)));
    goto L18592;}}
 L17754: {
  x_1423X = arg1K0;
  div_by_zeroP_1424X = s48_bignum_divide(x_1423X, y_1372X, &quot_1425X, &rem_1426X);
  if (div_by_zeroP_1424X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = 0;
    goto L40387;}
  else {
    v_1427X = s48_bignum_fits_in_word_p(rem_1426X, 30, 1);
    if (v_1427X) {
      n_1428X = s48_bignum_to_long(rem_1426X);
      arg0K0 = (((n_1428X)<<2));
      goto L17781;}
    else {
      v_1429X = enter_bignum(rem_1426X);
      arg0K0 = v_1429X;
      goto L17781;}}}
 L48301: {
  val_1430X = arg0K0;
  SvalS = val_1430X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L18144: {
  y_1431X = arg1K0;
  value_1432X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1432X))) {
    v_1433X = (char *) s48_long_to_bignum((((value_1432X)>>2)));
    arg1K0 = v_1433X;
    goto L18140;}
  else {
    arg1K0 = (((char *) (-3 + value_1432X)));
    goto L18140;}}
 L18070: {
  y_1434X = arg1K0;
  value_1435X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1435X))) {
    v_1436X = (char *) s48_long_to_bignum((((value_1435X)>>2)));
    arg1K0 = v_1436X;
    goto L18066;}
  else {
    arg1K0 = (((char *) (-3 + value_1435X)));
    goto L18066;}}
 L17996: {
  y_1437X = arg1K0;
  value_1438X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1438X))) {
    v_1439X = (char *) s48_long_to_bignum((((value_1438X)>>2)));
    arg1K0 = v_1439X;
    goto L17992;}
  else {
    arg1K0 = (((char *) (-3 + value_1438X)));
    goto L17992;}}
 L11328: {
  copies_1440X = arg0K0;
  if ((1 == copies_1440X)) {
    value_1441X = Sempty_logS;
    addr_1442X = (((char *) (-3 + proposal_658X))) + 4;
    S48_WRITE_BARRIER(proposal_658X, addr_1442X, value_1441X);
    *((long *) addr_1442X) = value_1441X;
    value_1443X = Sempty_logS;
    addr_1444X = (((char *) (-3 + proposal_658X))) + 8;
    S48_WRITE_BARRIER(proposal_658X, addr_1444X, value_1443X);
    *((long *) addr_1444X) = value_1443X;
    addr_1445X = (((char *) (-3 + proposal_658X))) + 12;
    S48_WRITE_BARRIER(proposal_658X, addr_1445X, 1);
    *((long *) addr_1445X) = 1;
    RELEASE_PROPOSAL_LOCK();
    x_1446X = Scurrent_threadS;
    addr_1447X = (((char *) (-3 + x_1446X))) + 12;
    S48_WRITE_BARRIER(x_1446X, addr_1447X, 1);
    *((long *) addr_1447X) = 1;
    SvalS = 5;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23052;}
  else {
    stob_1448X = *((long *) (((char *) (-3 + copies_1440X))));
    v_1449X = (((*((long *) ((((char *) (-3 + copies_1440X))) + 4))))>>2);
    stob_1450X = *((long *) ((((char *) (-3 + copies_1440X))) + 8));
    v_1451X = (((*((long *) ((((char *) (-3 + copies_1440X))) + 12))))>>2);
    v_1452X = (((*((long *) ((((char *) (-3 + copies_1440X))) + 16))))>>2);
    memcpy((void *)((((char *) (-3 + stob_1450X))) + v_1451X), (void *)((((char *) (-3 + stob_1448X))) + v_1449X),v_1452X);
    arg0K0 = (*((long *) ((((char *) (-3 + copies_1440X))) + 20)));
    goto L11328;}}
 L11445: {
  arg0K0 = (4 + i_1400X);
  goto L11413;}
 L30441: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L19202: {
  link_1453X = *((long *) ((((char *) (-3 + foo_1406X))) + 12));
  if ((0 == (3 & link_1453X))) {
    arg0K0 = foo_1406X;
    arg0K1 = (3 + (-4 & link_1453X));
    goto L19134;}
  else {
    arg0K0 = foo_1406X;
    arg0K1 = link_1453X;
    goto L19134;}}
 L14677: {
  if ((3 == (3 & thing_1035X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_1035X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L14683;}
    else {
      arg3K0 = "???";
      goto L14683;}}
  else {
    arg3K0 = "???";
    goto L14683;}}
 L21448: {
  StemplateS = (SvalS);
  Slosing_opcodeS = (0 - i_1190X);
  arg0K0 = arg_count_1292X;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L20778;}
 L21553: {
  ps_error("interrupt handler is not a closure", 1, i_1190X);
  goto L21448;}
 L18666: {
  x_1454X = arg1K0;
  external_bignum_1455X = (char *)s48_bignum_add(x_1454X, y_1414X);
  v_1456X = s48_bignum_fits_in_word_p(external_bignum_1455X, 30, 1);
  if (v_1456X) {
    n_1457X = s48_bignum_to_long(external_bignum_1455X);
    arg0K0 = (((n_1457X)<<2));
    goto L31906;}
  else {
    val_1458X = enter_bignum(external_bignum_1455X);
    arg0K0 = val_1458X;
    goto L31906;}}
 L18518: {
  x_1459X = arg1K0;
  external_bignum_1460X = (char *)s48_bignum_multiply(x_1459X, y_1417X);
  v_1461X = s48_bignum_fits_in_word_p(external_bignum_1460X, 30, 1);
  if (v_1461X) {
    n_1462X = s48_bignum_to_long(external_bignum_1460X);
    arg0K0 = (((n_1462X)<<2));
    goto L48301;}
  else {
    val_1463X = enter_bignum(external_bignum_1460X);
    arg0K0 = val_1463X;
    goto L48301;}}
 L18592: {
  x_1464X = arg1K0;
  external_bignum_1465X = (char *)s48_bignum_subtract(x_1464X, y_1420X);
  v_1466X = s48_bignum_fits_in_word_p(external_bignum_1465X, 30, 1);
  if (v_1466X) {
    n_1467X = s48_bignum_to_long(external_bignum_1465X);
    arg0K0 = (((n_1467X)<<2));
    goto L32097;}
  else {
    val_1468X = enter_bignum(external_bignum_1465X);
    arg0K0 = val_1468X;
    goto L32097;}}
 L40387: {
  div_by_zeroP_1469X = arg2K0;
  quot_1470X = arg0K1;
  rem_1471X = arg0K2;
  if (div_by_zeroP_1469X) {
    goto L40408;}
  else {
    if ((0 == (3 & rem_1471X))) {
      if ((0 == rem_1471X)) {
        SvalS = quot_1470X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23052;}
      else {
        goto L40408;}}
    else {
      goto L40408;}}}
 L17781: {
  v_1472X = arg0K0;
  v_1473X = s48_bignum_fits_in_word_p(quot_1425X, 30, 1);
  if (v_1473X) {
    n_1474X = s48_bignum_to_long(quot_1425X);
    arg2K0 = 0;
    arg0K1 = (((n_1474X)<<2));
    arg0K2 = v_1472X;
    goto L40387;}
  else {
    v_1475X = enter_bignum(quot_1425X);
    arg2K0 = 0;
    arg0K1 = v_1475X;
    arg0K2 = v_1472X;
    goto L40387;}}
 L18140: {
  x_1476X = arg1K0;
  external_bignum_1477X = (char *) s48_bignum_bitwise_and(x_1476X, y_1431X);
  v_1478X = s48_bignum_fits_in_word_p(external_bignum_1477X, 30, 1);
  if (v_1478X) {
    n_1479X = s48_bignum_to_long(external_bignum_1477X);
    arg0K0 = (((n_1479X)<<2));
    goto L34253;}
  else {
    val_1480X = enter_bignum(external_bignum_1477X);
    arg0K0 = val_1480X;
    goto L34253;}}
 L18066: {
  x_1481X = arg1K0;
  external_bignum_1482X = (char *) s48_bignum_bitwise_ior(x_1481X, y_1434X);
  v_1483X = s48_bignum_fits_in_word_p(external_bignum_1482X, 30, 1);
  if (v_1483X) {
    n_1484X = s48_bignum_to_long(external_bignum_1482X);
    arg0K0 = (((n_1484X)<<2));
    goto L34416;}
  else {
    val_1485X = enter_bignum(external_bignum_1482X);
    arg0K0 = val_1485X;
    goto L34416;}}
 L17992: {
  x_1486X = arg1K0;
  external_bignum_1487X = (char *) s48_bignum_bitwise_xor(x_1486X, y_1437X);
  v_1488X = s48_bignum_fits_in_word_p(external_bignum_1487X, 30, 1);
  if (v_1488X) {
    n_1489X = s48_bignum_to_long(external_bignum_1487X);
    arg0K0 = (((n_1489X)<<2));
    goto L34579;}
  else {
    val_1490X = enter_bignum(external_bignum_1487X);
    arg0K0 = val_1490X;
    goto L34579;}}
 L31906: {
  val_1491X = arg0K0;
  SvalS = val_1491X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L32097: {
  val_1492X = arg0K0;
  SvalS = val_1492X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L40408: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_482X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_483X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21148;}
 L34253: {
  val_1493X = arg0K0;
  SvalS = val_1493X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L34416: {
  val_1494X = arg0K0;
  SvalS = val_1494X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 L34579: {
  val_1495X = arg0K0;
  SvalS = val_1495X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23052;}
 loseD0: {
  message_387X = merged_arg3K0;{
  why_1496X = (((*((long *) ((SstackS) + (4 + (((nargs_785X)<<2)))))))>>2);
  ps_write_string("Template UIDs: ", (stderr));
  current_template_1497X = StemplateS;
  out_1498X = stderr;
  if ((0 == (3 & (*((long *) ((((char *) (-3 + current_template_1497X))) + 4)))))) {
    if ((current_template_1497X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
      arg2K0 = 0;
      goto L12842;}
    else {
      ps_write_integer(((((*((long *) ((((char *) (-3 + current_template_1497X))) + 4))))>>2)), out_1498X);
      arg2K0 = 1;
      goto L12842;}}
  else {
    arg2K0 = 0;
    goto L12842;}}
 L12842: {
  not_firstP_1499X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_1499X;
  goto L12849;}
 L12849: {
  cont_1500X = arg0K0;
  not_firstP_1501X = arg2K1;
  if ((3 == (3 & cont_1500X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_1500X))) + -4))))>>2))))) {
      template_1502X = *((long *) ((((char *) (-3 + cont_1500X))) + 8));
      if ((0 == (3 & (*((long *) ((((char *) (-3 + template_1502X))) + 4)))))) {
        if ((template_1502X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
          arg2K0 = not_firstP_1501X;
          goto L12858;}
        else {
          if (not_firstP_1501X) {
            ps_write_string(" <- ", out_1498X);
            goto L12897;}
          else {
            goto L12897;}}}
      else {
        arg2K0 = not_firstP_1501X;
        goto L12858;}}
    else {
      goto L21167;}}
  else {
    goto L21167;}}
 L12858: {
  v_1503X = arg2K0;
  arg0K0 = (*((long *) (((char *) (-3 + cont_1500X)))));
  arg2K1 = v_1503X;
  goto L12849;}
 L12897: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_1502X))) + 4))))>>2)), out_1498X);
  arg2K0 = 1;
  goto L12858;}
 L21167: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_1496X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_785X)<<2))))))))))))) {
      ps_error(message_387X, 3, opcode_786X, why_1496X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_785X)<<2)))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L21191;}}
  else {
    goto L21191;}}
 L21191: {
  ps_error(message_387X, 2, opcode_786X, why_1496X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L48310;}
 L48310: {
  type_1504X = s48_get_next_event(&channel_1505X, &status_1506X);
  if ((type_1504X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L20597;}
    else {
      arg0K0 = 1;
      goto L20597;}}
  else {
    if ((type_1504X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L20597;}
    else {
      if ((type_1504X == IO_COMPLETION_EVENT)) {
        enqueue_channelB(channel_1505X, status_1506X);
        arg0K0 = 8;
        goto L20597;}
      else {
        if ((type_1504X == OS_SIGNAL_EVENT)) {
          arg0K0 = 16;
          goto L20597;}
        else {
          if ((type_1504X == NO_EVENT)) {
            arg0K0 = 0;
            goto L20597;}
          else {
            if ((type_1504X == ERROR_EVENT)) {
              ps_write_string("OS error while getting event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              ps_write_string((ps_error_string(status_1506X)), (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L20597;}
            else {
              ps_write_string("unknown type of event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L20597;}}}}}}}
 L20597: {
  v_1507X = arg0K0;
  Spending_interruptsS = (v_1507X | (Spending_interruptsS));
  if ((type_1504X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L20624;}
      else {
        goto L20624;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L20624;}}
  else {
    goto L48310;}}
 L20624: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      goto L48310;}
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
  wants_stack_args_383X = merged_arg0K0;
  stack_arg_count_384X = merged_arg0K1;
  list_args_385X = merged_arg0K2;
  list_arg_count_386X = merged_arg0K3;{
  if ((stack_arg_count_384X == wants_stack_args_383X)) {
    merged_arg0K0 = list_args_385X;
    merged_arg0K1 = list_arg_count_386X;
    copy_listSAgc_return_tag = 1;
    goto copy_listSAgc;
   copy_listSAgc_return_1:
    x_1508X = copy_listSAgc0_return_value;
    *((long *) (SstackS)) = x_1508X;
    SstackS = ((SstackS) + -4);
    goto rest_list_setupAgc_return;}
  else {
    if ((stack_arg_count_384X < wants_stack_args_383X)) {
      count_1509X = wants_stack_args_383X - stack_arg_count_384X;
      merged_arg0K0 = list_args_385X;
      merged_arg0K1 = count_1509X;
      push_list_return_tag = 4;
      goto push_list;
     push_list_return_4:
      v_1510X = push_list0_return_value;
      merged_arg0K0 = v_1510X;
      merged_arg0K1 = (list_arg_count_386X - count_1509X);
      copy_listSAgc_return_tag = 2;
      goto copy_listSAgc;
     copy_listSAgc_return_2:
      x_1511X = copy_listSAgc0_return_value;
      *((long *) (SstackS)) = x_1511X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setupAgc_return;}
    else {
      merged_arg0K0 = list_args_385X;
      merged_arg0K1 = list_arg_count_386X;
      copy_listSAgc_return_tag = 3;
      goto copy_listSAgc;
     copy_listSAgc_return_3:
      v_1512X = copy_listSAgc0_return_value;
      merged_arg0K0 = v_1512X;
      merged_arg0K1 = (stack_arg_count_384X - wants_stack_args_383X);
      pop_args_GlistSAgc_return_tag = 11;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_11:
      x_1513X = pop_args_GlistSAgc0_return_value;
      *((long *) (SstackS)) = x_1513X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setupAgc_return;}}}
 rest_list_setupAgc_return:
  switch (rest_list_setupAgc_return_tag) {
  case 0: goto rest_list_setupAgc_return_0;
  default: goto rest_list_setupAgc_return_1;
  }}

 push_list: {
  list_381X = merged_arg0K0;
  count_382X = merged_arg0K1;{
  *((long *) (SstackS)) = list_381X;
  SstackS = ((SstackS) + -4);
  if ((count_382X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L20441;}
  else {
    s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
    merged_arg0K0 = 0;
    copy_stack_into_heap_return_tag = 4;
    goto copy_stack_into_heap;
   copy_stack_into_heap_return_4:
    if ((count_382X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L20441;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L20441;}}}
 L20441: {
  SstackS = ((SstackS) + 4);
  list_1514X = *((long *) (SstackS));
  arg0K0 = count_382X;
  arg0K1 = list_1514X;
  goto L20450;}
 L20450: {
  i_1515X = arg0K0;
  l_1516X = arg0K1;
  if ((0 < i_1515X)) {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1516X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (-1 + i_1515X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1516X))) + 4)));
    goto L20450;}
  else {
    push_list0_return_value = l_1516X;
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
  key_380X = merged_arg0K0;{
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L20015;}
 L20015: {
  p_1517X = arg1K0;
  i_1518X = arg0K1;
  if ((2 == (3 & (*((long *) p_1517X))))) {
    if ((26 == (*((long *) p_1517X)))) {
      arg0K0 = (-1 + i_1518X);
      goto L19995;}
    else {
      arg0K0 = i_1518X;
      goto L19995;}}
  else {
    arg1K0 = (p_1517X + 4);
    arg0K1 = (1 + i_1518X);
    goto L20015;}}
 L19995: {
  arg_count_1519X = arg0K0;
  top_1520X = SstackS;
  if ((1 == (ScontS))) {
    goto L20000;}
  else {
    merged_arg0K0 = key_380X;
    merged_arg0K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L20000;}}
 L20000: {
  SstackS = ((((char *) (-3 + (ScontS)))) + -8);
  arg1K0 = (SstackS);
  arg1K1 = (top_1520X + (((arg_count_1519X)<<2)));
  goto L20044;}
 L20044: {
  loc_1521X = arg1K0;
  arg_1522X = arg1K1;
  if ((top_1520X < arg_1522X)) {
    *((long *) loc_1521X) = (*((long *) arg_1522X));
    arg1K0 = (loc_1521X + -4);
    arg1K1 = (arg_1522X + -4);
    goto L20044;}
  else {
    SstackS = loc_1521X;
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
  key_378X = merged_arg0K0;
  reason_379X = merged_arg0K1;{
  p_1523X = SenvS;
  if ((3 == (3 & p_1523X))) {
    if ((p_1523X < (((long) (Sstack_beginS))))) {
      goto L18939;}
    else {
      if (((((long) (Sstack_endS))) < p_1523X)) {
        goto L18939;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_378X;
        merged_arg0K3 = reason_379X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_1524X = save_env_in_heap0_return_value;
        SenvS = v_1524X;
        goto L18939;}}}
  else {
    goto L18939;}}
 L18939: {
  end_1525X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L18892;}
 L18892: {
  cont_1526X = arg0K0;
  previous_1527X = arg0K1;
  if ((cont_1526X == (Sbottom_of_stackS))) {
    *((long *) (((char *) (-3 + previous_1527X)))) = end_1525X;
    ScontS = (Sbottom_of_stackS);
    really_preserve_continuation0_return_value = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
    goto really_preserve_continuation_return;}
  else {
    p_1528X = *((long *) ((((char *) (-3 + cont_1526X))) + 12));
    if ((3 == (3 & p_1528X))) {
      if ((p_1528X < (((long) (Sstack_beginS))))) {
        goto L18910;}
      else {
        if (((((long) (Sstack_endS))) < p_1528X)) {
          goto L18910;}
        else {
          merged_arg0K0 = (*((long *) ((((char *) (-3 + cont_1526X))) + 12)));
          merged_arg0K1 = cont_1526X;
          merged_arg0K2 = key_378X;
          merged_arg0K3 = reason_379X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L18910;}}}
    else {
      goto L18910;}}}
 L18910: {
  header_1529X = *((long *) ((((char *) (-3 + cont_1526X))) + -4));
  addr_1530X = s48_allocate_small((4 + ((long)(((unsigned long)header_1529X)>>8))));
  data_addr_1531X = addr_1530X + 4;
  *((long *) addr_1530X) = header_1529X;
  memcpy((void *)data_addr_1531X, (void *)(((char *) (-3 + cont_1526X))),((long)(((unsigned long)header_1529X)>>8)));
  new_1532X = 3 + (((long) data_addr_1531X));
  *((long *) (((char *) (-3 + previous_1527X)))) = new_1532X;
  arg0K0 = (*((long *) (((char *) (-3 + new_1532X)))));
  arg0K1 = new_1532X;
  goto L18892;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 save_env_in_heap: {
  env_376X = merged_arg0K0;
  cont_377X = merged_arg0K1;{
  header_1533X = *((long *) ((((char *) (-3 + env_376X))) + -4));
  addr_1534X = s48_allocate_small((4 + ((long)(((unsigned long)header_1533X)>>8))));
  data_addr_1535X = addr_1534X + 4;
  *((long *) addr_1534X) = header_1533X;
  memcpy((void *)data_addr_1535X, (void *)(((char *) (-3 + env_376X))),((long)(((unsigned long)header_1533X)>>8)));
  new_1536X = 3 + (((long) data_addr_1535X));
  addr_1537X = ((char *) (-3 + env_376X));
  S48_WRITE_BARRIER(env_376X, addr_1537X, 26);
  *((long *) addr_1537X) = 26;
  *((long *) ((((char *) (-3 + env_376X))) + -4)) = new_1536X;
  arg0K0 = new_1536X;
  goto L15852;}
 L15852: {
  env_1538X = arg0K0;
  p_1539X = *((long *) (((char *) (-3 + env_1538X))));
  if ((3 == (3 & p_1539X))) {
    if ((p_1539X < (((long) (Sstack_beginS))))) {
      goto L16063;}
    else {
      if (((((long) (Sstack_endS))) < p_1539X)) {
        goto L16063;}
      else {
        env_1540X = *((long *) (((char *) (-3 + env_1538X))));
        header_1541X = *((long *) ((((char *) (-3 + env_1540X))) + -4));
        addr_1542X = s48_allocate_small((4 + ((long)(((unsigned long)header_1541X)>>8))));
        data_addr_1543X = addr_1542X + 4;
        *((long *) addr_1542X) = header_1541X;
        memcpy((void *)data_addr_1543X, (void *)(((char *) (-3 + env_1540X))),((long)(((unsigned long)header_1541X)>>8)));
        new_1544X = 3 + (((long) data_addr_1543X));
        addr_1545X = ((char *) (-3 + env_1540X));
        S48_WRITE_BARRIER(env_1540X, addr_1545X, 26);
        *((long *) addr_1545X) = 26;
        *((long *) ((((char *) (-3 + env_1540X))) + -4)) = new_1544X;
        addr_1546X = ((char *) (-3 + env_1538X));
        S48_WRITE_BARRIER(env_1538X, addr_1546X, new_1544X);
        *((long *) addr_1546X) = new_1544X;
        arg0K0 = new_1544X;
        goto L15852;}}}
  else {
    goto L16063;}}
 L16063: {
  arg0K0 = cont_377X;
  goto L16075;}
 L16075: {
  cont_1547X = arg0K0;
  env_1548X = *((long *) ((((char *) (-3 + cont_1547X))) + 12));
  if ((3 == (3 & env_1548X))) {
    if ((3 == (3 & (*((long *) ((((char *) (-3 + env_1548X))) + -4)))))) {
      *((long *) ((((char *) (-3 + cont_1547X))) + 12)) = (*((long *) ((((char *) (-3 + env_1548X))) + -4)));
      arg0K0 = (*((long *) (((char *) (-3 + cont_1547X)))));
      goto L16075;}
    else {
      save_env_in_heap0_return_value = new_1536X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = new_1536X;
    goto save_env_in_heap_return;}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  default: goto save_env_in_heap_return_2;
  }}

 pop_args_GlistSAgc: {
  start_374X = merged_arg0K0;
  count_375X = merged_arg0K1;{
  Stemp0S = start_374X;
  s48_make_availableAgc(((((3 * count_375X))<<2)));
  value_1549X = Stemp0S;
  Stemp0S = 1;
  arg0K0 = value_1549X;
  arg0K1 = count_375X;
  goto L15394;}
 L15394: {
  args_1550X = arg0K0;
  count_1551X = arg0K1;
  if ((0 == count_1551X)) {
    pop_args_GlistSAgc0_return_value = args_1550X;
    goto pop_args_GlistSAgc_return;}
  else {
    SstackS = ((SstackS) + 4);
    a_1552X = *((long *) (SstackS));
    addr_1553X = s48_allocate_small(12);
    *((long *) addr_1553X) = 2050;
    x_1554X = 3 + (((long) (addr_1553X + 4)));
    *((long *) (((char *) (-3 + x_1554X)))) = a_1552X;
    *((long *) ((((char *) (-3 + x_1554X))) + 4)) = args_1550X;
    arg0K0 = x_1554X;
    arg0K1 = (-1 + count_1551X);
    goto L15394;}}
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
  list_372X = merged_arg0K0;
  length_373X = merged_arg0K1;{
  if ((0 == length_373X)) {
    copy_listSAgc0_return_value = 25;
    goto copy_listSAgc_return;}
  else {
    Stemp0S = list_372X;
    s48_make_availableAgc(((((3 * length_373X))<<2)));
    value_1555X = Stemp0S;
    Stemp0S = 1;
    a_1556X = *((long *) (((char *) (-3 + value_1555X))));
    addr_1557X = s48_allocate_small(12);
    *((long *) addr_1557X) = 2050;
    x_1558X = 3 + (((long) (addr_1557X + 4)));
    *((long *) (((char *) (-3 + x_1558X)))) = a_1556X;
    *((long *) ((((char *) (-3 + x_1558X))) + 4)) = 25;
    arg0K0 = (*((long *) ((((char *) (-3 + value_1555X))) + 4)));
    arg0K1 = x_1558X;
    goto L15282;}}
 L15282: {
  l_1559X = arg0K0;
  last_1560X = arg0K1;
  if ((25 == l_1559X)) {
    copy_listSAgc0_return_value = x_1558X;
    goto copy_listSAgc_return;}
  else {
    a_1561X = *((long *) (((char *) (-3 + l_1559X))));
    addr_1562X = s48_allocate_small(12);
    *((long *) addr_1562X) = 2050;
    x_1563X = 3 + (((long) (addr_1562X + 4)));
    *((long *) (((char *) (-3 + x_1563X)))) = a_1561X;
    *((long *) ((((char *) (-3 + x_1563X))) + 4)) = 25;
    addr_1564X = (((char *) (-3 + last_1560X))) + 4;
    S48_WRITE_BARRIER(last_1560X, addr_1564X, x_1563X);
    *((long *) addr_1564X) = x_1563X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_1559X))) + 4)));
    arg0K1 = x_1563X;
    goto L15282;}}
 copy_listSAgc_return:
  switch (copy_listSAgc_return_tag) {
  case 0: goto copy_listSAgc_return_0;
  case 1: goto copy_listSAgc_return_1;
  case 2: goto copy_listSAgc_return_2;
  default: goto copy_listSAgc_return_3;
  }}

 shift_space: {
  x_370X = merged_arg0K0;
  n_371X = merged_arg0K1;{
  if ((0 == (3 & x_370X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16144;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_370X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16144;}}
 L16144: {
  x_size_1565X = arg0K0;
  extra_1566X = arg0K1;
  if ((n_371X < 0)) {
    if ((x_size_1565X < 1)) {
      arg0K0 = 1;
      goto L16189;}
    else {
      arg0K0 = x_size_1565X;
      goto L16189;}}
  else {
    n_1567X = n_371X / 30;
    arg0K0 = (5 + (x_size_1565X + n_1567X));
    goto L16164;}}
 L16189: {
  v_1568X = arg0K0;
  arg0K0 = (9 + ((((v_1568X)<<1)) + x_size_1565X));
  goto L16164;}
 L16164: {
  v_1569X = arg0K0;
  shift_space0_return_value = (extra_1566X + v_1569X);
  goto shift_space_return;}
 shift_space_return:
  switch (shift_space_return_tag) {
  case 0: goto shift_space_return_0;
  default: goto shift_space_return_1;
  }}

 okay_copy_argsP: {
  thing_367X = merged_arg0K0;
  index_368X = merged_arg0K1;
  count_369X = merged_arg0K2;{
  if ((index_368X < 0)) {
    okay_copy_argsP0_return_value = 0;
    goto okay_copy_argsP_return;}
  else {
    if ((3 == (3 & thing_367X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_367X))) + -4))))>>2))))) {
        if (((-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_367X))) + -4))))>>8))) < (index_368X + count_369X))) {
          okay_copy_argsP0_return_value = 0;
          goto okay_copy_argsP_return;}
        else {
          okay_copy_argsP0_return_value = 1;
          goto okay_copy_argsP_return;}}
      else {
        goto L14884;}}
    else {
      goto L14884;}}}
 L14884: {
  if ((3 == (3 & thing_367X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + thing_367X))) + -4))))>>2))))) {
      if ((((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_367X))) + -4))))>>8)) < (index_368X + count_369X))) {
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
  cont_365X = merged_arg0K0;
  stack_arg_count_366X = merged_arg0K1;{
  top_1570X = (((char *) (-3 + (Sbottom_of_stackS)))) + (-8 - (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_365X))) + -4))))>>8)))));
  new_cont_1571X = 3 + (((long) (top_1570X + 4)));
  if ((0 == stack_arg_count_366X)) {
    SstackS = (top_1570X + -4);
    goto L14074;}
  else {
    new_stack_1572X = top_1570X + (-4 - (((stack_arg_count_366X)<<2)));
    if ((new_stack_1572X < (SstackS))) {
      x_1573X = SstackS;
      memcpy((void *)(new_stack_1572X + 4), (void *)(x_1573X + 4),(((stack_arg_count_366X)<<2)));
      SstackS = new_stack_1572X;
      goto L14074;}
    else {
      goto L14074;}}}
 L14074: {
  ScontS = new_cont_1571X;
  v_1574X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_365X))) + -4))))>>8))))>>2);
  memcpy((void *)top_1570X, (void *)((((char *) (-3 + cont_365X))) + -4),(4 + (((v_1574X)<<2))));
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = (*((long *) (((char *) (-3 + new_cont_1571X)))));
  *((long *) (((char *) (-3 + new_cont_1571X)))) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_1571X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  case 1: goto copy_continuation_from_heapB_return_1;
  default: goto copy_continuation_from_heapB_return_2;
  }}

 proposal_d_write: {
  stob_362X = merged_arg0K0;
  index_363X = merged_arg0K1;
  value_364X = merged_arg0K2;{
  log_1575X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L13845;}
 L13845: {
  i_1576X = arg0K0;
  next_stob_1577X = *((long *) ((((char *) (-3 + log_1575X))) + (((i_1576X)<<2))));
  if ((1 == next_stob_1577X)) {
    add_log_entryAgc(1, i_1576X, stob_362X, index_363X, value_364X, 0);
    goto proposal_d_write_return;}
  else {
    if ((stob_362X == next_stob_1577X)) {
      if ((index_363X == (*((long *) ((((char *) (-3 + log_1575X))) + (4 + (((i_1576X)<<2)))))))) {
        addr_1578X = (((char *) (-3 + log_1575X))) + (8 + (((i_1576X)<<2)));
        S48_WRITE_BARRIER(log_1575X, addr_1578X, value_364X);
        *((long *) addr_1578X) = value_364X;
        goto proposal_d_write_return;}
      else {
        goto L13865;}}
    else {
      goto L13865;}}}
 L13865: {
  arg0K0 = (4 + i_1576X);
  goto L13845;}
 proposal_d_write_return:
  switch (proposal_d_write_return_tag) {
  case 0: goto proposal_d_write_return_0;
  case 1: goto proposal_d_write_return_1;
  default: goto proposal_d_write_return_2;
  }}

 proposal_d_read: {
  stob_360X = merged_arg0K0;
  index_361X = merged_arg0K1;{
  log_1579X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L13668;}
 L13668: {
  i_1580X = arg0K0;
  next_stob_1581X = *((long *) ((((char *) (-3 + log_1579X))) + (((i_1580X)<<2))));
  if ((1 == next_stob_1581X)) {
    v_1582X = add_log_entryAgc(1, i_1580X, stob_360X, index_361X, (*((long *) ((((char *) (-3 + stob_360X))) + (-4 & index_361X)))), 1);
    proposal_d_read0_return_value = v_1582X;
    goto proposal_d_read_return;}
  else {
    if ((stob_360X == next_stob_1581X)) {
      if ((index_361X == (*((long *) ((((char *) (-3 + log_1579X))) + (4 + (((i_1580X)<<2)))))))) {
        proposal_d_read0_return_value = (*((long *) ((((char *) (-3 + log_1579X))) + (8 + (((i_1580X)<<2))))));
        goto proposal_d_read_return;}
      else {
        goto L13690;}}
    else {
      goto L13690;}}}
 L13690: {
  arg0K0 = (4 + i_1580X);
  goto L13668;}
 proposal_d_read_return:
  switch (proposal_d_read_return_tag) {
  case 0: goto proposal_d_read_return_0;
  case 1: goto proposal_d_read_return_1;
  default: goto proposal_d_read_return_2;
  }}

 make_closure: {
  a_358X = merged_arg0K0;
  b_359X = merged_arg0K1;{
  addr_1583X = s48_allocate_small(12);
  *((long *) addr_1583X) = 2062;
  x_1584X = 3 + (((long) (addr_1583X + 4)));
  *((long *) (((char *) (-3 + x_1584X)))) = a_358X;
  *((long *) ((((char *) (-3 + x_1584X))) + 4)) = b_359X;
  if ((3 == (3 & x_1584X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_1584X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_1584X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_1584X))) + -4))));
      make_closure0_return_value = x_1584X;
      goto make_closure_return;}
    else {
      make_closure0_return_value = x_1584X;
      goto make_closure_return;}}
  else {
    make_closure0_return_value = x_1584X;
    goto make_closure_return;}}
 make_closure_return:
  switch (make_closure_return_tag) {
  case 0: goto make_closure_return_0;
  default: goto make_closure_return_1;
  }}

 get_current_port: {
  marker_357X = merged_arg0K0;{
  thread_1585X = Scurrent_threadS;
  if ((3 == (3 & thread_1585X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thread_1585X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_1585X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_1585X))) + 4)));
        goto L12299;}
      else {
        goto L12349;}}
    else {
      goto L12349;}}
  else {
    goto L12349;}}
 L12299: {
  env_1586X = arg0K0;
  if ((3 == (3 & env_1586X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_1586X))) + -4))))>>2))))) {
      obj_1587X = *((long *) (((char *) (-3 + env_1586X))));
      if ((3 == (3 & obj_1587X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_1587X))) + -4))))>>2))))) {
          if ((marker_357X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_1586X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_1586X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_1586X))) + 4)));
            goto L12299;}}
        else {
          goto L12371;}}
      else {
        goto L12371;}}
    else {
      goto L12371;}}
  else {
    goto L12371;}}
 L12349: {
  ps_error("current thread is not a record", 0);
  get_current_port0_return_value = v_1588X;
  goto get_current_port_return;}
 L12371: {
  if ((25 == env_1586X)) {
    if (((((marker_357X)>>2)) == 1)) {
      arg3K0 = "dynamic environment doesn't have current-output-port";
      goto L12325;}
    else {
      arg3K0 = "dynamic environment doesn't have current-input-port";
      goto L12325;}}
  else {
    ps_error("dynamic environment is not a proper list", 0);
    get_current_port0_return_value = v_1589X;
    goto get_current_port_return;}}
 L12325: {
  v_1590X = arg3K0;
  ps_error(v_1590X, 0);
  get_current_port0_return_value = v_1591X;
  goto get_current_port_return;}
 get_current_port_return:
  switch (get_current_port_return_tag) {
  case 0: goto get_current_port_return_0;
  case 1: goto get_current_port_return_1;
  default: goto get_current_port_return_2;
  }}

 okay_argument_list: {
  list_356X = merged_arg0K0;{
  arg0K0 = list_356X;
  arg0K1 = 0;
  arg0K2 = list_356X;
  arg2K3 = 0;
  goto L12166;}
 L12166: {
  fast_1592X = arg0K0;
  len_1593X = arg0K1;
  slow_1594X = arg0K2;
  move_slowP_1595X = arg2K3;
  if ((25 == fast_1592X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_1593X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_1592X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_1592X))) + -4))))>>2))))) {
        if (move_slowP_1595X) {
          if ((fast_1592X == slow_1594X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_1592X))) + 4)));
            arg0K1 = (1 + len_1593X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_1594X))) + 4)));
            arg2K3 = 0;
            goto L12166;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_1592X))) + 4)));
          arg0K1 = (1 + len_1593X);
          arg0K2 = slow_1594X;
          arg2K3 = 1;
          goto L12166;}}
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
  status_355X = merged_arg0K0;{
  string_1596X = ps_error_string(status_355X);
  x_1597X = strlen((char *) string_1596X);
  if ((x_1597X < 256)) {
    arg0K0 = x_1597X;
    goto L11626;}
  else {
    arg0K0 = 256;
    goto L11626;}}
 L11626: {
  len_1598X = arg0K0;
  len_1599X = 1 + len_1598X;
  addr_1600X = s48_allocate_small((4 + len_1599X));
  *((long *) addr_1600X) = (70 + (((len_1599X)<<8)));
  string_1601X = 3 + (((long) (addr_1600X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1601X))) + len_1598X)) = 0;
  arg0K0 = 0;
  goto L11636;}
 L11636: {
  i_1602X = arg0K0;
  if ((i_1602X == len_1598X)) {
    get_error_string0_return_value = string_1601X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1601X))) + i_1602X)) = ((*(string_1596X + i_1602X)));
    arg0K0 = (1 + i_1602X);
    goto L11636;}}
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
{ cont_1603X = ScontS;
  tem_1604X = *((long *) ((((char *) (-3 + cont_1603X))) + 8));
  pc_1605X = *((long *) ((((char *) (-3 + cont_1603X))) + 4));
  StemplateS = tem_1604X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1604X)))))))) + (((pc_1605X)>>2)));
  SenvS = (*((long *) ((((char *) (-3 + cont_1603X))) + 12)));
  ScontS = (*((long *) (((char *) (-3 + cont_1603X)))));
  SstackS = ((((char *) (-3 + cont_1603X))) + 12);
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
long s48_call_startup_procedure(char **startup_vector_1606X, long startup_vector_length_1607X)
{
  long arg0K0;
  long merged_arg0K1;
  char *merged_arg3K0;

  int enter_string_return_tag;
  long enter_string0_return_value;
  char *string_1608X;
  long i_1648X;
  long string_1647X;
  char * addr_1646X;
  long len_1645X;
  long len_1644X;
  long v_1643X;
  long x_1642X;
  long x_1641X;
  char * addr_1640X;
  long b_1639X;
  long channel_1638X;
  long x_1637X;
  char * addr_1636X;
  long b_1635X;
  long channel_1634X;
  long x_1633X;
  char * addr_1632X;
  long b_1631X;
  long channel_1630X;
  long i_1629X;
  long length_1628X;
  long *v_1627X;
  long v_1626X;
  long v_1625X;
  long y_1624X;
  long x_1623X;
  long v_1622X;
  long x_1621X;
  long y_1620X;
  char * addr_1619X;
  long value_1618X;
  long x_1617X;
  long y_1616X;
  long i_1615X;
  long vector_1614X;
  long v_1613X;
  long vector_1612X;
  char * addr_1611X;
  long len_1610X;
  long tem_1609X;
 {  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  SenvS = 13;
  tem_1609X = Sinterrupt_templateS;
  StemplateS = tem_1609X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1609X))))))));
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
  len_1610X = ((startup_vector_length_1607X)<<2);
  addr_1611X = s48_allocate_tracedAgc((4 + len_1610X));
  if ((addr_1611X == NULL)) {
    arg0K0 = 1;
    goto L15796;}
  else {
    *((long *) addr_1611X) = (10 + (((len_1610X)<<8)));
    arg0K0 = (3 + (((long) (addr_1611X + 4))));
    goto L15796;}}
 L15796: {
  vector_1612X = arg0K0;
  if ((1 == vector_1612X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1613X;
    goto L15782;}
  else {
    arg0K0 = vector_1612X;
    goto L15782;}}
 L15782: {
  vector_1614X = arg0K0;
  arg0K0 = 0;
  goto L15819;}
 L15819: {
  i_1615X = arg0K0;
  if ((i_1615X == startup_vector_length_1607X)) {
    *((long *) (SstackS)) = vector_1614X;
    SstackS = ((SstackS) + -4);
    y_1616X = fileno((stderr));
    x_1617X = fileno((stdout));
    if ((x_1617X < y_1616X)) {
      arg0K0 = y_1616X;
      goto L17506;}
    else {
      arg0K0 = x_1617X;
      goto L17506;}}
  else {
    value_1618X = enter_stringAgc((*(startup_vector_1606X + i_1615X)));
    addr_1619X = (((char *) (-3 + vector_1614X))) + (((i_1615X)<<2));
    S48_WRITE_BARRIER(vector_1614X, addr_1619X, value_1618X);
    *((long *) addr_1619X) = value_1618X;
    arg0K0 = (1 + i_1615X);
    goto L15819;}}
 L17506: {
  y_1620X = arg0K0;
  x_1621X = fileno((stdin));
  if ((x_1621X < y_1620X)) {
    arg0K0 = y_1620X;
    goto L17508;}
  else {
    arg0K0 = x_1621X;
    goto L17508;}}
 L17508: {
  v_1622X = arg0K0;
  x_1623X = Snumber_of_channelsS;
  y_1624X = 1 + v_1622X;
  if ((x_1623X < y_1624X)) {
    arg0K0 = y_1624X;
    goto L17510;}
  else {
    arg0K0 = x_1623X;
    goto L17510;}}
 L17510: {
  v_1625X = arg0K0;
  Snumber_of_channelsS = v_1625X;
  v_1626X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L17532;}
  else {
    goto L17532;}}
 L17532: {
  v_1627X = Svm_channelsS;
  length_1628X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L17580;}
 L17580: {
  i_1629X = arg0K0;
  if ((i_1629X < length_1628X)) {
    *(v_1627X + i_1629X) = 1;
    arg0K0 = (1 + i_1629X);
    goto L17580;}
  else {
    s48_make_availableAgc(((((3 * (7 + ((((4 + (strlen((char *) "standard output"))))>>2)))))<<2)));
    channel_1630X = fileno((stdin));
    merged_arg3K0 = "standard input";
    merged_arg0K1 = 0;
    enter_string_return_tag = 0;
    goto enter_string;
   enter_string_return_0:
    b_1631X = enter_string0_return_value;
    addr_1632X = s48_allocate_small(24);
    *((long *) addr_1632X) = 5146;
    x_1633X = 3 + (((long) (addr_1632X + 4)));
    *((long *) (((char *) (-3 + x_1633X)))) = 4;
    *((long *) ((((char *) (-3 + x_1633X))) + 4)) = b_1631X;
    *((long *) ((((char *) (-3 + x_1633X))) + 8)) = (((channel_1630X)<<2));
    *((long *) ((((char *) (-3 + x_1633X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1633X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1630X) = x_1633X;
    channel_1634X = fileno((stderr));
    merged_arg3K0 = "standard error";
    merged_arg0K1 = 0;
    enter_string_return_tag = 1;
    goto enter_string;
   enter_string_return_1:
    b_1635X = enter_string0_return_value;
    addr_1636X = s48_allocate_small(24);
    *((long *) addr_1636X) = 5146;
    x_1637X = 3 + (((long) (addr_1636X + 4)));
    *((long *) (((char *) (-3 + x_1637X)))) = 8;
    *((long *) ((((char *) (-3 + x_1637X))) + 4)) = b_1635X;
    *((long *) ((((char *) (-3 + x_1637X))) + 8)) = (((channel_1634X)<<2));
    *((long *) ((((char *) (-3 + x_1637X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1637X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1634X) = x_1637X;
    channel_1638X = fileno((stdout));
    merged_arg3K0 = "standard output";
    merged_arg0K1 = 0;
    enter_string_return_tag = 2;
    goto enter_string;
   enter_string_return_2:
    b_1639X = enter_string0_return_value;
    addr_1640X = s48_allocate_small(24);
    *((long *) addr_1640X) = 5146;
    x_1641X = 3 + (((long) (addr_1640X + 4)));
    *((long *) (((char *) (-3 + x_1641X)))) = 8;
    *((long *) ((((char *) (-3 + x_1641X))) + 4)) = b_1639X;
    *((long *) ((((char *) (-3 + x_1641X))) + 8)) = (((channel_1638X)<<2));
    *((long *) ((((char *) (-3 + x_1641X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1641X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1638X) = x_1641X;
    *((long *) (SstackS)) = x_1633X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1641X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1637X;
    SstackS = ((SstackS) + -4);
    x_1642X = s48_resumer_records();
    *((long *) (SstackS)) = x_1642X;
    SstackS = ((SstackS) + -4);
    s48_initialization_completeB();
    v_1643X = s48_startup_procedure();
    return s48_restart(v_1643X, 5);}}
 enter_string: {
  string_1608X = merged_arg3K0;{
  len_1644X = strlen((char *) string_1608X);
  len_1645X = 1 + len_1644X;
  addr_1646X = s48_allocate_small((4 + len_1645X));
  *((long *) addr_1646X) = (70 + (((len_1645X)<<8)));
  string_1647X = 3 + (((long) (addr_1646X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1647X))) + len_1644X)) = 0;
  arg0K0 = 0;
  goto L13507;}
 L13507: {
  i_1648X = arg0K0;
  if ((i_1648X == len_1644X)) {
    enter_string0_return_value = string_1647X;
    goto enter_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1647X))) + i_1648X)) = ((*(string_1608X + i_1648X)));
    arg0K0 = (1 + i_1648X);
    goto L13507;}}
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
Spost_gc_cleanupS = HtopD9982;
Sgc_root_procS = HtopD9993;
s48_Scallback_return_stack_blockS = 1;
s48_Spending_eventsPS = 0;
}
