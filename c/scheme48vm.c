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
static long Hlookup22920(long, long, long);
static long Hlookup22901(long, long, long);
static void HtopD30352(void);
static void HtopD30363(void);
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
    goto L2857;}
  else {
    arg0K0 = x_5X;
    goto L2857;}}
 L2857: {
  new_count_7X = arg0K0;
  new_vm_channels_8X = (long*)malloc(sizeof(long) * new_count_7X);
  if ((NULL == new_vm_channels_8X)) {
    return 0;}
  else {
    arg0K0 = 0;
    goto L2869;}}
 L2869: {
  i_9X = arg0K0;
  if ((i_9X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L2942;}
  else {
    *(new_vm_channels_8X + i_9X) = (*((Svm_channelsS) + i_9X));
    arg0K0 = (1 + i_9X);
    goto L2869;}}
 L2942: {
  i_10X = arg0K0;
  if ((i_10X == new_count_7X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_8X;
    Snumber_of_channelsS = new_count_7X;
    return 1;}
  else {
    *(new_vm_channels_8X + i_10X) = 1;
    arg0K0 = (1 + i_10X);
    goto L2942;}}
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
    goto L10451;}
  else {
    goto L10451;}}
 L10451: {
  v_22X = *((long *) (((char *) (-3 + channel_18X))));
  if ((4 == v_22X)) {
    goto L10466;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_18X))))))) {
      goto L10466;}
    else {
      v_23X = ps_close_fd(os_index_19X);
      arg0K0 = v_23X;
      goto L10473;}}}
 L10466: {
  v_24X = ps_close_fd(os_index_19X);
  arg0K0 = v_24X;
  goto L10473;}
 L10473: {
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
  goto L12013;}
 L12013: {
  p_36X = arg1K0;
  i_37X = arg0K1;
  if ((2 == (3 & (*((long *) p_36X))))) {
    if ((26 == (*((long *) p_36X)))) {
      arg0K0 = (-1 + i_37X);
      goto L11954;}
    else {
      arg0K0 = i_37X;
      goto L11954;}}
  else {
    arg1K0 = (p_36X + 4);
    arg0K1 = (1 + i_37X);
    goto L12013;}}
 L11954: {
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
      goto L9676;}
    else {
      arg0K0 = (-1 + (((log_size_46X)<<1)));
      goto L9676;}}
  else {
    arg0K0 = proposal_45X;
    arg0K1 = stob_41X;
    arg0K2 = value_43X;
    goto L12123;}}
 L9676: {
  new_size_47X = arg0K0;
  len_48X = ((new_size_47X)<<2);
  addr_49X = s48_allocate_tracedAgc((4 + len_48X));
  if ((addr_49X == NULL)) {
    arg0K0 = 1;
    goto L9722;}
  else {
    *((long *) addr_49X) = (10 + (((len_48X)<<8)));
    arg0K0 = (3 + (((long) (addr_49X + 4))));
    goto L9722;}}
 L12123: {
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
    goto L12144;}
  else {
    arg0K0 = 29;
    goto L12144;}}
 L9722: {
  vector_57X = arg0K0;
  if ((1 == vector_57X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_58X;
    goto L9680;}
  else {
    arg0K0 = vector_57X;
    goto L9680;}}
 L12144: {
  value_59X = arg0K0;
  addr_60X = (((char *) (-3 + log_53X))) + (12 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_60X, value_59X);
  *((long *) addr_60X) = value_59X;
  addr_61X = (((char *) (-3 + log_53X))) + (16 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_61X, 1);
  *((long *) addr_61X) = 1;
  return value_52X;}
 L9680: {
  new_62X = arg0K0;
  proposal_63X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 < log_size_46X)) {
    stob_64X = *((long *) ((((char *) (-3 + proposal_63X))) + 4));
    memcpy((void *)(((char *) (-3 + new_62X))), (void *)(((char *) (-3 + stob_64X))),(-4 + (((log_size_46X)<<2))));
    goto L9700;}
  else {
    goto L9700;}}
 L9700: {
  arg0K0 = (4 + log_size_46X);
  goto L9704;}
 L9704: {
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
    goto L12123;}
  else {
    addr_70X = (((char *) (-3 + new_62X))) + (((i_65X)<<2));
    S48_WRITE_BARRIER(new_62X, addr_70X, 0);
    *((long *) addr_70X) = 0;
    arg0K0 = (1 + i_65X);
    goto L9704;}}
}
static long enter_bignum(char * external_bignum_71X)
{
  long desc_72X;
 {  desc_72X = 3 + (((long) external_bignum_71X));
  if ((3 == (3 & desc_72X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + desc_72X))) + -4))))>>2))))) {
      goto L12757;}
    else {
      goto L12771;}}
  else {
    goto L12771;}}
 L12757: {
  if ((3 == (3 & desc_72X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + desc_72X))) + -4)))))) {
      *((long *) ((((char *) (-3 + desc_72X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + desc_72X))) + -4))));
      return desc_72X;}
    else {
      return desc_72X;}}
  else {
    return desc_72X;}}
 L12771: {
  ps_error("not a bignum", 1, desc_72X);
  goto L12757;}
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
  goto L13693;}
 L13693: {
  i_78X = arg0K0;
  if ((i_78X == z_74X)) {
    return string_77X;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_77X))) + i_78X)) = ((*(string_73X + i_78X)));
    arg0K0 = (1 + i_78X);
    goto L13693;}}
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
      goto L14621;}
    else {
      goto L14615;}}
  else {
    goto L14615;}}
 L14621: {
  PS_WRITE_CHAR(10, (stderr), v_82X)
  return v_82X;}
 L14615: {
  ps_write_integer((((index_80X)>>2)), (stderr));
  goto L14621;}
}
static long make_registered_channel(long mode_83X, long id_84X, long os_index_85X, long key_86X, long *TT0)
{
  long x_90X;
  char * addr_89X;
  char x_88X;
  char temp_87X;
 {  temp_87X = os_index_85X < (Snumber_of_channelsS);
  if (temp_87X) {
    goto L15673;}
  else {
    x_88X = add_more_channels(os_index_85X);
    if (x_88X) {
      goto L15673;}
    else {
      *TT0 = 9;
      return 1;}}}
 L15673: {
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
static long Hlookup22920(long table_91X, long string_92X, long key_93X)
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
  goto L19540;}
 L19540: {
  i_95X = arg0K0;
  h_96X = arg0K1;
  if ((i_95X < n_94X)) {
    arg0K0 = (1 + i_95X);
    arg0K1 = (h_96X + (((*((unsigned char *) ((((char *) (-3 + string_92X))) + i_95X))))));
    goto L19540;}
  else {
    index_97X = 1023 & h_96X;
    link_98X = *((long *) ((((char *) (-3 + table_91X))) + (((index_97X)<<2))));
    if ((0 == (3 & link_98X))) {
      arg0K0 = (3 + (-4 & link_98X));
      goto L19496;}
    else {
      arg0K0 = link_98X;
      goto L19496;}}}
 L19496: {
  bucket_99X = arg0K0;
  arg0K0 = bucket_99X;
  goto L19502;}
 L19502: {
  foo_100X = arg0K0;
  if ((1 == foo_100X)) {
    if ((3 == (3 & bucket_99X))) {
      arg0K0 = (-4 & bucket_99X);
      goto L19507;}
    else {
      arg0K0 = bucket_99X;
      goto L19507;}}
  else {
    s2_101X = *((long *) (((char *) (-3 + foo_100X))));
    len_102X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_92X))) + -4))))>>8);
    if ((len_102X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_101X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_101X))), (void *)(((char *) (-3 + string_92X))),len_102X)))) {
        return foo_100X;}
      else {
        goto L19522;}}
    else {
      goto L19522;}}}
 L19507: {
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
    goto L19513;}
  else {
    arg0K0 = x_105X;
    goto L19513;}}
 L19522: {
  link_106X = *((long *) ((((char *) (-3 + foo_100X))) + 12));
  if ((0 == (3 & link_106X))) {
    arg0K0 = (3 + (-4 & link_106X));
    goto L19502;}
  else {
    arg0K0 = link_106X;
    goto L19502;}}
 L19513: {
  value_107X = arg0K0;
  addr_108X = (((char *) (-3 + table_91X))) + (((index_97X)<<2));
  S48_WRITE_BARRIER(table_91X, addr_108X, value_107X);
  *((long *) addr_108X) = value_107X;
  return x_105X;}
}
static long Hlookup22901(long table_109X, long string_110X, long key_111X)
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
  goto L19692;}
 L19692: {
  i_113X = arg0K0;
  h_114X = arg0K1;
  if ((i_113X < n_112X)) {
    arg0K0 = (1 + i_113X);
    arg0K1 = (h_114X + (((*((unsigned char *) ((((char *) (-3 + string_110X))) + i_113X))))));
    goto L19692;}
  else {
    index_115X = 1023 & h_114X;
    link_116X = *((long *) ((((char *) (-3 + table_109X))) + (((index_115X)<<2))));
    if ((0 == (3 & link_116X))) {
      arg0K0 = (3 + (-4 & link_116X));
      goto L19648;}
    else {
      arg0K0 = link_116X;
      goto L19648;}}}
 L19648: {
  bucket_117X = arg0K0;
  arg0K0 = bucket_117X;
  goto L19654;}
 L19654: {
  foo_118X = arg0K0;
  if ((1 == foo_118X)) {
    if ((3 == (3 & bucket_117X))) {
      arg0K0 = (-4 & bucket_117X);
      goto L19659;}
    else {
      arg0K0 = bucket_117X;
      goto L19659;}}
  else {
    s2_119X = *((long *) (((char *) (-3 + foo_118X))));
    len_120X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_110X))) + -4))))>>8);
    if ((len_120X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_119X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_119X))), (void *)(((char *) (-3 + string_110X))),len_120X)))) {
        return foo_118X;}
      else {
        goto L19674;}}
    else {
      goto L19674;}}}
 L19659: {
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
    goto L19665;}
  else {
    arg0K0 = x_123X;
    goto L19665;}}
 L19674: {
  link_124X = *((long *) ((((char *) (-3 + foo_118X))) + 12));
  if ((0 == (3 & link_124X))) {
    arg0K0 = (3 + (-4 & link_124X));
    goto L19654;}
  else {
    arg0K0 = link_124X;
    goto L19654;}}
 L19665: {
  value_125X = arg0K0;
  addr_126X = (((char *) (-3 + table_109X))) + (((index_115X)<<2));
  S48_WRITE_BARRIER(table_109X, addr_126X, value_125X);
  *((long *) addr_126X) = value_125X;
  return x_123X;}
}
static void HtopD30352(void)
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
  goto L20325;}
 L20325: {
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
    goto L10065;}
  else {
    foo_link_132X = *((long *) ((((char *) (-3 + table_127X))) + (((i_128X)<<2))));
    arg0K0 = foo_link_132X;
    arg0K1 = 1;
    goto L13215;}}
 L10065: {
  alist_133X = arg0K0;
  okay_134X = arg0K1;
  goners_135X = arg0K2;
  if ((25 == alist_133X)) {
    SHARED_SETB((Sfinalizer_alistS), okay_134X);
    l2_136X = Sfinalize_theseS;
    if ((25 == goners_135X)) {
      arg0K0 = l2_136X;
      goto L10072;}
    else {
      arg0K0 = goners_135X;
      goto L10127;}}
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
      goto L10065;}
    else {
      addr_146X = (((char *) (-3 + alist_137X))) + 4;
      S48_WRITE_BARRIER(alist_137X, addr_146X, goners_135X);
      *((long *) addr_146X) = goners_135X;
      arg0K0 = next_140X;
      arg0K1 = okay_134X;
      arg0K2 = alist_137X;
      goto L10065;}}}
 L13215: {
  foo_link_147X = arg0K0;
  okay_link_148X = arg0K1;
  if ((0 == (3 & foo_link_147X))) {
    arg0K0 = (3 + (-4 & foo_link_147X));
    goto L13217;}
  else {
    arg0K0 = foo_link_147X;
    goto L13217;}}
 L10072: {
  v_149X = arg0K0;
  Sfinalize_theseS = v_149X;
  arg0K0 = 0;
  goto L18915;}
 L10127: {
  l_150X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_150X))) + 4))))) {
    addr_151X = (((char *) (-3 + l_150X))) + 4;
    S48_WRITE_BARRIER(l_150X, addr_151X, l2_136X);
    *((long *) addr_151X) = l2_136X;
    arg0K0 = goners_135X;
    goto L10072;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_150X))) + 4)));
    goto L10127;}}
 L13217: {
  foo_152X = arg0K0;
  if ((1 == foo_152X)) {
    addr_153X = (((char *) (-3 + table_127X))) + (((i_128X)<<2));
    S48_WRITE_BARRIER(table_127X, addr_153X, okay_link_148X);
    *((long *) addr_153X) = okay_link_148X;
    arg0K0 = (1 + i_128X);
    goto L20325;}
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
        goto L13215;}
      else {
        arg0K0 = next_link_156X;
        arg0K1 = new_foo_155X;
        goto L13215;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + foo_152X))) + 4)));
      arg0K1 = okay_link_148X;
      goto L13215;}}}
 L18915: {
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
      goto L18949;}
    else {
      v_160X = s48_extantP(channel_159X);
      if (v_160X) {
        v_161X = s48_trace_value(channel_159X);
        arg0K0 = v_161X;
        goto L18942;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_159X))))))) {
          arg0K0 = 1;
          goto L18942;}
        else {
          id_162X = *((long *) ((((char *) (-3 + channel_159X))) + 4));
          status_163X = close_channelB(channel_159X);
          if ((status_163X == NO_ERRORS)) {
            goto L16941;}
          else {
            channel_close_error(status_163X, (*((long *) ((((char *) (-3 + channel_159X))) + 8))), id_162X);
            goto L16941;}}}}}}
 L18949: {
  arg0K0 = (1 + i_158X);
  goto L18915;}
 L18942: {
  new_164X = arg0K0;
  *((Svm_channelsS) + i_158X) = new_164X;
  goto L18949;}
 L16941: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_162X))) {
    ps_write_integer((((id_162X)>>2)), (stderr));
    goto L16959;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_162X))))), (stderr));
    goto L16959;}}
 L16959: {
  ps_write_string(" ", (stderr));
  ps_write_integer(((((*((long *) ((((char *) (-3 + channel_159X))) + 8))))>>2)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L18942;}
}
static void HtopD30363(void)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long merged_arg0K0;

  int Hentry_tracer2867165_return_tag;
  long Hentry_tracer28671650_return_value;
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
  goto L6719;}
 L6719: {
  frame_168X = arg1K0;
  if ((frame_168X == NULL)) {
    arg1K0 = (Spermanent_external_rootsS);
    goto L6745;}
  else {
    length_169X = *((long *) frame_168X);
    arg0K0 = 0;
    goto L6727;}}
 L6745: {
  frame_170X = arg1K0;
  if ((frame_170X == NULL)) {
    s48_initializing_gc_root();
    table_171X = s48_trace_value((Simported_bindingsS));
    arg0K0 = 0;
    goto L17224;}
  else {
    cell_172X = ((char *) (*((long *) (frame_170X + 4))));
    x2_173X = s48_trace_value((*((long *) cell_172X)));
    *((long *) cell_172X) = x2_173X;
    arg1K0 = (((char *) (*((long *) frame_170X))));
    goto L6745;}}
 L6727: {
  i_174X = arg0K0;
  if ((i_174X == length_169X)) {
    arg1K0 = (((char *) (*((long *) (frame_168X + 4)))));
    goto L6719;}
  else {
    cell_175X = ((char *) (*((long *) (frame_168X + (8 + (((i_174X)<<2)))))));
    x2_176X = s48_trace_value((*((long *) cell_175X)));
    *((long *) cell_175X) = x2_176X;
    arg0K0 = (1 + i_174X);
    goto L6727;}}
 L17224: {
  i_177X = arg0K0;
  if ((1024 == i_177X)) {
    Simported_bindingsS = table_171X;
    table_178X = s48_trace_value((Sexported_bindingsS));
    arg0K0 = 0;
    goto L17245;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_171X))) + (((i_177X)<<2)))));
    Hentry_tracer2867165_return_tag = 0;
    goto Hentry_tracer2867165;
   Hentry_tracer2867165_return_0:
    value_179X = Hentry_tracer28671650_return_value;
    addr_180X = (((char *) (-3 + table_171X))) + (((i_177X)<<2));
    S48_WRITE_BARRIER(table_171X, addr_180X, value_179X);
    *((long *) addr_180X) = value_179X;
    arg0K0 = (1 + i_177X);
    goto L17224;}}
 L17245: {
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
    goto L6783;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_178X))) + (((i_181X)<<2)))));
    Hentry_tracer2867165_return_tag = 1;
    goto Hentry_tracer2867165;
   Hentry_tracer2867165_return_1:
    value_199X = Hentry_tracer28671650_return_value;
    addr_200X = (((char *) (-3 + table_178X))) + (((i_181X)<<2));
    S48_WRITE_BARRIER(table_178X, addr_200X, value_199X);
    *((long *) addr_200X) = value_199X;
    arg0K0 = (1 + i_181X);
    goto L17245;}}
 L6783: {
  alist_201X = arg0K0;
  if ((25 == alist_201X)) {
    env_202X = s48_trace_value((SenvS));
    SenvS = env_202X;
    v_203X = s48_trace_value((Spending_channels_headS));
    Spending_channels_headS = v_203X;
    v_204X = s48_trace_value((Spending_channels_tailS));
    Spending_channels_tailS = v_204X;
    arg0K0 = 0;
    goto L10197;}
  else {
    pair_205X = *((long *) (((char *) (-3 + alist_201X))));
    x_206X = s48_extantP((*((long *) (((char *) (-3 + pair_205X))))));
    if (x_206X) {
      goto L6808;}
    else {
      s48_trace_stob_contentsB((*((long *) (((char *) (-3 + pair_205X))))));
      goto L6808;}}}
 L10197: {
  i_207X = arg0K0;
  if ((i_207X == (Snumber_of_channelsS))) {
    if ((Sstack_warningPS)) {
      arg1K0 = (Sstack_beginS);
      goto L7331;}
    else {
      goto L7364;}}
  else {
    channel_208X = *((Svm_channelsS) + i_207X);
    if ((1 == channel_208X)) {
      goto L10225;}
    else {
      if ((0 == (*((long *) (((char *) (-3 + channel_208X))))))) {
        goto L10225;}
      else {
        val_209X = s48_trace_value((*((long *) ((((char *) (-3 + channel_208X))) + 4))));
        addr_210X = (((char *) (-3 + channel_208X))) + 4;
        S48_WRITE_BARRIER(channel_208X, addr_210X, val_209X);
        *((long *) addr_210X) = val_209X;
        goto L10225;}}}}
 L6808: {
  val_211X = s48_trace_value((*((long *) ((((char *) (-3 + pair_205X))) + 4))));
  addr_212X = (((char *) (-3 + pair_205X))) + 4;
  S48_WRITE_BARRIER(pair_205X, addr_212X, val_211X);
  *((long *) addr_212X) = val_211X;
  arg0K0 = (*((long *) ((((char *) (-3 + alist_201X))) + 4)));
  goto L6783;}
 L7331: {
  a_213X = arg1K0;
  if ((252645135 == (*((long *) a_213X)))) {
    arg1K0 = (a_213X + 4);
    goto L7331;}
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
      goto L7364;}
    else {
      goto L7364;}}}
 L7364: {
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L7429;}
 L10225: {
  arg0K0 = (1 + i_207X);
  goto L10197;}
 L7429: {
  p_215X = arg1K0;
  i_216X = arg0K1;
  if ((2 == (3 & (*((long *) p_215X))))) {
    if ((26 == (*((long *) p_215X)))) {
      arg0K0 = (-1 + i_216X);
      goto L7366;}
    else {
      arg0K0 = i_216X;
      goto L7366;}}
  else {
    arg1K0 = (p_215X + 4);
    arg0K1 = (1 + i_216X);
    goto L7429;}}
 L7366: {
  arg_count_217X = arg0K0;
  s48_trace_locationsB(((SstackS) + 4), ((SstackS) + (4 + (((arg_count_217X)<<2)))));
  p_218X = SenvS;
  if ((3 == (3 & p_218X))) {
    if ((p_218X < (((long) (Sstack_beginS))))) {
      goto L7382;}
    else {
      if (((((long) (Sstack_endS))) < p_218X)) {
        goto L7382;}
      else {
        env_219X = SenvS;
        arg0K0 = env_219X;
        goto L7542;}}}
  else {
    goto L7382;}}
 L7382: {
  v_220X = s48_trace_value((SenvS));
  SenvS = v_220X;
  goto L7387;}
 L7542: {
  env_221X = arg0K0;
  if ((3 == (3 & env_221X))) {
    if ((env_221X < (((long) (Sstack_beginS))))) {
      goto L7387;}
    else {
      if (((((long) (Sstack_endS))) < env_221X)) {
        goto L7387;}
      else {
        s48_trace_stob_contentsB(env_221X);
        arg0K0 = (*((long *) (((char *) (-3 + env_221X)))));
        goto L7542;}}}
  else {
    goto L7387;}}
 L7387: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L7392;}
 L7392: {
  cont_222X = arg0K0;
  last_env_223X = arg0K1;
  env_224X = *((long *) ((((char *) (-3 + cont_222X))) + 12));
  s48_trace_stob_contentsB(cont_222X);
  if ((env_224X == last_env_223X)) {
    goto L7410;}
  else {
    arg0K0 = env_224X;
    goto L7497;}}
 L7410: {
  if ((cont_222X == (Sbottom_of_stackS))) {
    v_225X = s48_trace_value((Stemp0S));
    Stemp0S = v_225X;
    v_226X = s48_trace_value((Stemp1S));
    Stemp1S = v_226X;
    return;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + cont_222X)))));
    arg0K1 = env_224X;
    goto L7392;}}
 L7497: {
  env_227X = arg0K0;
  if ((3 == (3 & env_227X))) {
    if ((env_227X < (((long) (Sstack_beginS))))) {
      goto L7410;}
    else {
      if (((((long) (Sstack_endS))) < env_227X)) {
        goto L7410;}
      else {
        s48_trace_stob_contentsB(env_227X);
        arg0K0 = (*((long *) (((char *) (-3 + env_227X)))));
        goto L7497;}}}
  else {
    goto L7410;}}
 Hentry_tracer2867165: {
  foo_link_166X = merged_arg0K0;{
  arg0K0 = foo_link_166X;
  arg0K1 = 1;
  goto L13287;}
 L13287: {
  foo_link_228X = arg0K0;
  done_link_229X = arg0K1;
  if ((0 == (3 & foo_link_228X))) {
    arg0K0 = (3 + (-4 & foo_link_228X));
    goto L13289;}
  else {
    arg0K0 = foo_link_228X;
    goto L13289;}}
 L13289: {
  foo_230X = arg0K0;
  if ((1 == foo_230X)) {
    Hentry_tracer28671650_return_value = done_link_229X;
    goto Hentry_tracer2867165_return;}
  else {
    new_foo_231X = s48_trace_value(foo_230X);
    next_link_232X = *((long *) ((((char *) (-3 + new_foo_231X))) + 12));
    addr_233X = (((char *) (-3 + new_foo_231X))) + 12;
    S48_WRITE_BARRIER(new_foo_231X, addr_233X, done_link_229X);
    *((long *) addr_233X) = done_link_229X;
    if ((3 == (3 & new_foo_231X))) {
      arg0K0 = next_link_232X;
      arg0K1 = (-4 & new_foo_231X);
      goto L13287;}
    else {
      arg0K0 = next_link_232X;
      arg0K1 = new_foo_231X;
      goto L13287;}}}
 Hentry_tracer2867165_return:
  switch (Hentry_tracer2867165_return_tag) {
  case 0: goto Hentry_tracer2867165_return_0;
  default: goto Hentry_tracer2867165_return_1;
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
    goto L3771;}
  else {
    goto L3771;}}
 L3771: {
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
    goto L6315;}
  else {
    goto L6315;}}
 L6315: {
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
    goto L10356;}
  else {
    x_262X = add_more_channels(os_index_260X);
    if (x_262X) {
      goto L10356;}
    else {
      return 36;}}}
 L10356: {
  if ((1 == (*((Svm_channelsS) + os_index_260X)))) {
    old_index_263X = (((*((long *) ((((char *) (-3 + channel_259X))) + 8))))>>2);
    x_264X = *((long *) ((((char *) (-3 + channel_259X))) + 16));
    if ((5 == x_264X)) {
      v_265X = ps_abort_fd_op(old_index_263X);
      enqueue_channelB(old_index_263X, v_265X);
      goto L10342;}
    else {
      goto L10342;}}
  else {
    return 44;}}
 L10342: {
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
    goto L13726;}
  else {
    goto L13726;}}
 L13726: {
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
  goto L15154;}
 L15154: {
  i_274X = arg0K0;
  temp_275X = 1024 == i_274X;
  if (temp_275X) {
    return temp_275X;}
  else {
    link_276X = *((long *) ((((char *) (-3 + table_273X))) + (((i_274X)<<2))));
    if ((0 == (3 & link_276X))) {
      arg0K0 = (3 + (-4 & link_276X));
      goto L15128;}
    else {
      arg0K0 = link_276X;
      goto L15128;}}}
 L15128: {
  entry_277X = arg0K0;
  if ((1 == entry_277X)) {
    arg0K0 = (1 + i_274X);
    goto L15154;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + entry_277X))) + 8)))))) {
      name_278X = *((long *) (((char *) (-3 + entry_277X))));
      ps_write_string("undefined imported name ", out_272X);
      if ((3 == (3 & name_278X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_278X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + name_278X))))), out_272X);
          goto L17135;}
        else {
          goto L17133;}}
      else {
        goto L17133;}}
    else {
      goto L15135;}}}
 L17135: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_272X, ignoreXX) }
  goto L15135;}
 L17133: {
  ps_write_string("<invalid name>", out_272X);
  goto L17135;}
 L15135: {
  link_279X = *((long *) ((((char *) (-3 + entry_277X))) + 12));
  if ((0 == (3 & link_279X))) {
    arg0K0 = (3 + (-4 & link_279X));
    goto L15128;}
  else {
    arg0K0 = link_279X;
    goto L15128;}}
}
long s48_really_add_channel(long mode_280X, long id_281X, long os_index_282X)
{
  long status_284X;
  long channel_283X;
 {  s48_make_availableAgc(24);
  channel_283X = make_registered_channel((((mode_280X)>>2)), id_281X, os_index_282X, 0, &status_284X);
  if ((3 == (3 & channel_283X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_283X))) + -4))))>>2))))) {
      return channel_283X;}
    else {
      goto L17278;}}
  else {
    goto L17278;}}
 L17278: {
  return (((status_284X)<<2));}
}
long s48_enter_integer(long x_285X)
{
  char * v_286X;
 {  s48_make_availableAgc(16);
  if ((536870911 < x_285X)) {
    goto L18898;}
  else {
    if ((x_285X < -536870912)) {
      goto L18898;}
    else {
      return (((x_285X)<<2));}}}
 L18898: {
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
  return Hlookup22920((Sexported_bindingsS), value_289X, 0);}
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
  x_295X = Hlookup22901((Simported_bindingsS), value_294X, 0);
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
        goto L20023;}
      else {
        arg0K0 = maybe_302X;
        goto L20023;}}
    else {
      arg0K0 = maybe_301X;
      goto L20023;}}
  else {
    Sthe_symbol_tableS = symbol_table_299X;
    goto L20494;}}
 L20023: {
  symbols_303X = arg0K0;
  n_304X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_303X))) + -4))))>>8))))>>2);
  arg0K0 = 0;
  goto L20044;}
 L20494: {
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
    goto L20500;}
  else {
    Simported_bindingsS = imported_bindings_306X;
    Sexported_bindingsS = exported_bindings_305X;
    goto L20500;}}
 L20044: {
  i_309X = arg0K0;
  if ((i_309X == n_304X)) {
    goto L20494;}
  else {
    table_310X = Sthe_symbol_tableS;
    foo_311X = *((long *) ((((char *) (-3 + symbols_303X))) + (((i_309X)<<2))));
    string_312X = *((long *) (((char *) (-3 + foo_311X))));
    n_313X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_312X))) + -4))))>>8));
    arg0K0 = 0;
    arg0K1 = 0;
    goto L19451;}}
 L20500: {
  if ((stack_size_298X < 8131)) {
    stack_314X = (char *)malloc(32524);
    if ((stack_314X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_314X;
      arg0K1 = 8131;
      goto L17342;}
    else {
      arg1K0 = stack_314X;
      arg0K1 = 8131;
      goto L17342;}}
  else {
    arg1K0 = stack_begin_297X;
    arg0K1 = stack_size_298X;
    goto L17342;}}
 L19451: {
  i_315X = arg0K0;
  h_316X = arg0K1;
  if ((i_315X < n_313X)) {
    arg0K0 = (1 + i_315X);
    arg0K1 = (h_316X + (((*((unsigned char *) ((((char *) (-3 + string_312X))) + i_315X))))));
    goto L19451;}
  else {
    index_317X = 1023 & h_316X;
    val_318X = *((long *) ((((char *) (-3 + table_310X))) + (((index_317X)<<2))));
    addr_319X = (((char *) (-3 + foo_311X))) + 4;
    S48_WRITE_BARRIER(foo_311X, addr_319X, val_318X);
    *((long *) addr_319X) = val_318X;
    if ((3 == (3 & foo_311X))) {
      arg0K0 = (-4 & foo_311X);
      goto L19437;}
    else {
      arg0K0 = foo_311X;
      goto L19437;}}}
 L17342: {
  start_320X = arg1K0;
  size_321X = arg0K1;
  Sstack_beginS = start_320X;
  Sstack_endS = (start_320X + (((size_321X)<<2)));
  Sstack_limitS = ((Sstack_beginS) + 524);
  SstackS = ((Sstack_endS) + -4);
  ScontS = 1;
  SenvS = 17;
  arg1K0 = start_320X;
  goto L17374;}
 L19437: {
  value_322X = arg0K0;
  addr_323X = (((char *) (-3 + table_310X))) + (((index_317X)<<2));
  S48_WRITE_BARRIER(table_310X, addr_323X, value_322X);
  *((long *) addr_323X) = value_322X;
  arg0K0 = (1 + i_309X);
  goto L20044;}
 L17374: {
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
      goto L20551;}
    else {
      *((long *) addr_341X) = 1034;
      arg0K0 = (3 + (((long) (addr_341X + 4))));
      goto L20551;}}
  else {
    *((long *) a_324X) = 252645135;
    arg1K0 = (a_324X + 4);
    goto L17374;}}
 L20551: {
  vector_342X = arg0K0;
  if ((1 == vector_342X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_343X;
    goto L20538;}
  else {
    arg0K0 = vector_342X;
    goto L20538;}}
 L20538: {
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
    goto L9586;}
  else {
    *((long *) addr_347X) = 1048586;
    arg0K0 = (3 + (((long) (addr_347X + 4))));
    goto L9586;}}
 L9586: {
  vector_348X = arg0K0;
  if ((1 == vector_348X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_349X;
    goto L9574;}
  else {
    arg0K0 = vector_348X;
    goto L9574;}}
 L9574: {
  table_350X = arg0K0;
  arg0K0 = 0;
  goto L9609;}
 L9609: {
  i_351X = arg0K0;
  if ((1024 == i_351X)) {
    make_hash_tableAgc0_return_value = table_350X;
    goto make_hash_tableAgc_return;}
  else {
    addr_352X = (((char *) (-3 + table_350X))) + (((i_351X)<<2));
    S48_WRITE_BARRIER(table_350X, addr_352X, 1);
    *((long *) addr_352X) = 1;
    arg0K0 = (1 + i_351X);
    goto L9609;}}
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
  long pc_1611X;
  long tem_1610X;
  long cont_1609X;
  long i_1608X;
  long string_1607X;
  char * addr_1606X;
  long len_1605X;
  long len_1604X;
  long x_1603X;
  char *string_1602X;
  char move_slowP_1601X;
  long slow_1600X;
  long len_1599X;
  long fast_1598X;
  long v_1597X;
  char *v_1596X;
  long v_1595X;
  long v_1594X;
  long obj_1593X;
  long env_1592X;
  long thread_1591X;
  long x_1590X;
  char * addr_1589X;
  long v_1588X;
  long next_stob_1587X;
  long i_1586X;
  long log_1585X;
  char * addr_1584X;
  long next_stob_1583X;
  long i_1582X;
  long log_1581X;
  long v_1580X;
  char * x_1579X;
  char * new_stack_1578X;
  long new_cont_1577X;
  char * top_1576X;
  long v_1575X;
  long v_1574X;
  long n_1573X;
  long extra_1572X;
  long x_size_1571X;
  char * addr_1570X;
  long x_1569X;
  char * addr_1568X;
  long a_1567X;
  long last_1566X;
  long l_1565X;
  long x_1564X;
  char * addr_1563X;
  long a_1562X;
  long value_1561X;
  long x_1560X;
  char * addr_1559X;
  long a_1558X;
  long count_1557X;
  long args_1556X;
  long value_1555X;
  long env_1554X;
  long cont_1553X;
  char * addr_1552X;
  char * addr_1551X;
  long new_1550X;
  char * data_addr_1549X;
  char * addr_1548X;
  long header_1547X;
  long env_1546X;
  long p_1545X;
  long env_1544X;
  char * addr_1543X;
  long new_1542X;
  char * data_addr_1541X;
  char * addr_1540X;
  long header_1539X;
  long new_1538X;
  char * data_addr_1537X;
  char * addr_1536X;
  long header_1535X;
  long p_1534X;
  long previous_1533X;
  long cont_1532X;
  long end_1531X;
  long v_1530X;
  long p_1529X;
  char * arg_1528X;
  char * loc_1527X;
  char * top_1526X;
  long arg_count_1525X;
  long i_1524X;
  char * p_1523X;
  long l_1522X;
  long i_1521X;
  long list_1520X;
  long x_1519X;
  long v_1518X;
  long x_1517X;
  long v_1516X;
  long count_1515X;
  long x_1514X;
  long v_1513X;
  long status_1512X;
  long channel_1511X;
  long type_1510X;
  char v_1509X;
  long template_1508X;
  char not_firstP_1507X;
  long cont_1506X;
  char not_firstP_1505X;
  FILE * out_1504X;
  long current_template_1503X;
  long why_1502X;
  long val_1501X;
  long val_1500X;
  long val_1499X;
  long val_1498X;
  long val_1497X;
  long val_1496X;
  long n_1495X;
  char v_1494X;
  char * external_bignum_1493X;
  char * x_1492X;
  long val_1491X;
  long n_1490X;
  char v_1489X;
  char * external_bignum_1488X;
  char * x_1487X;
  long val_1486X;
  long n_1485X;
  char v_1484X;
  char * external_bignum_1483X;
  char * x_1482X;
  long v_1481X;
  long n_1480X;
  char v_1479X;
  long v_1478X;
  long rem_1477X;
  long quot_1476X;
  char div_by_zeroP_1475X;
  long val_1474X;
  long n_1473X;
  char v_1472X;
  char * external_bignum_1471X;
  char * x_1470X;
  long val_1469X;
  long n_1468X;
  char v_1467X;
  char * external_bignum_1466X;
  char * x_1465X;
  long val_1464X;
  long n_1463X;
  char v_1462X;
  char * external_bignum_1461X;
  char * x_1460X;
  long link_1459X;
  long v_1458X;
  long v_1457X;
  long stob_1456X;
  long v_1455X;
  long stob_1454X;
  char * addr_1453X;
  long x_1452X;
  char * addr_1451X;
  char * addr_1450X;
  long value_1449X;
  char * addr_1448X;
  long value_1447X;
  long copies_1446X;
  char * v_1445X;
  long value_1444X;
  char * y_1443X;
  char * v_1442X;
  long value_1441X;
  char * y_1440X;
  char * v_1439X;
  long value_1438X;
  char * y_1437X;
  long val_1436X;
  long v_1435X;
  long n_1434X;
  char v_1433X;
  char * rem_1432X;
  char * quot_1431X;
  char div_by_zeroP_1430X;
  char * x_1429X;
  char * v_1428X;
  long value_1427X;
  char * y_1426X;
  char * v_1425X;
  long value_1424X;
  char * y_1423X;
  char * v_1422X;
  long value_1421X;
  char * y_1420X;
  long obj_1419X;
  char * addr_1418X;
  long val_1417X;
  char * addr_1416X;
  long value_1415X;
  long len_1414X;
  long s2_1413X;
  long foo_1412X;
  long previous_foo_1411X;
  long value_1410X;
  long verify_1409X;
  long copies_1408X;
  long stob_1407X;
  long i_1406X;
  long x_1405X;
  long x_1404X;
  long val_1403X;
  char * addr_1402X;
  long value_1401X;
  char * v_1400X;
  long value_1399X;
  long v_1398X;
  char * v_1397X;
  long value_1396X;
  long v_1395X;
  char * v_1394X;
  long value_1393X;
  long v_1392X;
  long val_1391X;
  long val_1390X;
  long n_1389X;
  char v_1388X;
  char * external_bignum_1387X;
  char * x_1386X;
  long val_1385X;
  long n_1384X;
  char v_1383X;
  char * external_bignum_1382X;
  char * x_1381X;
  char * v_1380X;
  long value_1379X;
  char * y_1378X;
  char * v_1377X;
  long value_1376X;
  long v_1375X;
  long val_1374X;
  long v_1373X;
  char * v_1372X;
  char * v_1371X;
  long value_1370X;
  long extra1_1369X;
  long length1_1368X;
  char * v_1367X;
  long value_1366X;
  long v_1365X;
  long v_1364X;
  long bucket_1363X;
  char * addr_1362X;
  long value_1361X;
  long verify_1360X;
  long log_1359X;
  long stob_1358X;
  long i_1357X;
  long x_1356X;
  long status_1355X;
  char pendingP_1354X;
  char eofP_1353X;
  long got_1352X;
  long y_1351X;
  long link_1350X;
  long val_1349X;
  long x_1348X;
  char * addr_1347X;
  long b_1346X;
  long val_1345X;
  long extra1_1344X;
  long length1_1343X;
  long extra1_1342X;
  long length1_1341X;
  long extra1_1340X;
  long length1_1339X;
  long n_1338X;
  char * v_1337X;
  long val_1336X;
  long n_1335X;
  char v_1334X;
  char * external_bignum_1333X;
  char * v_1332X;
  long n_1331X;
  char v_1330X;
  char * external_bignum_1329X;
  char * v_1328X;
  long value_1327X;
  char * y_1326X;
  char * v_1325X;
  long value_1324X;
  char * y_1323X;
  char * v_1322X;
  long value_1321X;
  long extra1_1320X;
  long val_1319X;
  long v_1318X;
  char * v_1317X;
  long extra1_1316X;
  long length1_1315X;
  long val_1314X;
  long v_1313X;
  char * v_1312X;
  long x_1311X;
  long val_1310X;
  long v_1309X;
  char * v_1308X;
  long extra0_1307X;
  long length0_1306X;
  long extra1_1305X;
  long length1_1304X;
  long final_stack_arg_count_1303X;
  char v_1302X;
  char x_1301X;
  long channel_1300X;
  long handlers_1299X;
  long arg_count_1298X;
  char * addr_1297X;
  long value_1296X;
  long offset_1295X;
  long i_1294X;
  long count_1293X;
  char * addr_1292X;
  long value_1291X;
  long offset_1290X;
  long i_1289X;
  long count_1288X;
  char *v_1287X;
  long link_1286X;
  long index_1285X;
  long h_1284X;
  long i_1283X;
  char * addr_1282X;
  char * addr_1281X;
  long value_1280X;
  char * addr_1279X;
  char * addr_1278X;
  char * addr_1277X;
  long value_1276X;
  char * addr_1275X;
  char * addr_1274X;
  long value_1273X;
  long proposal_1272X;
  long entry_1271X;
  long thing_1270X;
  long log_1269X;
  long copies_1268X;
  char * addr_1267X;
  long x_1266X;
  long val_1265X;
  long status_1264X;
  long v_1263X;
  long len_1262X;
  long s2_1261X;
  long foo_1260X;
  long val_1259X;
  long v_1258X;
  long n_1257X;
  char v_1256X;
  char * external_bignum_1255X;
  char * x_1254X;
  long extra0_1253X;
  long length0_1252X;
  long extra0_1251X;
  long length0_1250X;
  long extra0_1249X;
  long length0_1248X;
  char * v_1247X;
  long value_1246X;
  long v_1245X;
  char * v_1244X;
  long value_1243X;
  long v_1242X;
  char * v_1241X;
  long v_1240X;
  char * x_1239X;
  char * v_1238X;
  long n_1237X;
  char * v_1236X;
  long value_1235X;
  long extra1_1234X;
  char * v_1233X;
  long value_1232X;
  long extra1_1231X;
  long val_1230X;
  long val_1229X;
  long val_1228X;
  long val_1227X;
  long val_1226X;
  long extra0_1225X;
  long length0_1224X;
  long val_1223X;
  long v_1222X;
  char * v_1221X;
  long x_1220X;
  long val_1219X;
  long v_1218X;
  char * v_1217X;
  long extra0_1216X;
  long length0_1215X;
  long y_1214X;
  long x_1213X;
  long extra0_1212X;
  long length0_1211X;
  char * arg_1210X;
  char * loc_1209X;
  long l_1208X;
  long stack_nargs_1207X;
  long x_1206X;
  long v_1205X;
  long skip_1204X;
  long template_1203X;
  long stack_arg_count_1202X;
  long skip_1201X;
  char * addr_1200X;
  long next_1199X;
  long channel_1198X;
  long m_1197X;
  long i_1196X;
  long count_1195X;
  long index_1194X;
  long i_1193X;
  long env_1192X;
  long count_1191X;
  long i_1190X;
  long env_1189X;
  long i_1188X;
  long l_1187X;
  long n_1186X;
  long table_1185X;
  long val_1184X;
  char temp_1183X;
  char minutesP_1182X;
  long v_1181X;
  long vector_1180X;
  char * addr_1179X;
  long x_1178X;
  long value_1177X;
  long verify_1176X;
  long copies_1175X;
  long stob_1174X;
  long i_1173X;
  char * addr_1172X;
  long v_1171X;
  char * addr_1170X;
  char * addr_1169X;
  long val_1168X;
  long x_1167X;
  long status_1166X;
  char pendingP_1165X;
  long got_1164X;
  long y_1163X;
  char v_1162X;
  long count_1161X;
  long start_1160X;
  char waitP_1159X;
  long x_1158X;
  long status_1157X;
  long channel_1156X;
  long v_1155X;
  long v_1154X;
  long v_1153X;
  long reason_1152X;
  long channel_1151X;
  long index_1150X;
  long bucket_1149X;
  long i_1148X;
  long i_1147X;
  char * addr_1146X;
  long i_1145X;
  long rest_list_1144X;
  long i_1143X;
  long v_1142X;
  long n_1141X;
  char v_1140X;
  char * external_bignum_1139X;
  char * x_1138X;
  long val_1137X;
  long v_1136X;
  char * v_1135X;
  char * v_1134X;
  long value_1133X;
  long needed_1132X;
  long y_1131X;
  long y_1130X;
  long x_1129X;
  long val_1128X;
  long v_1127X;
  char * v_1126X;
  long val_1125X;
  long v_1124X;
  char * v_1123X;
  long extra_1122X;
  long length_1121X;
  long count_1120X;
  long x_1119X;
  long extra_1118X;
  long length_1117X;
  char * v_1116X;
  long value_1115X;
  long extra_1114X;
  long length_1113X;
  long x_1112X;
  long c_1111X;
  long b_1110X;
  long extra0_1109X;
  long length0_1108X;
  long c_1107X;
  long b_1106X;
  long extra0_1105X;
  long length0_1104X;
  long v_1103X;
  long v_1102X;
  long v_1101X;
  long v_1100X;
  long v_1099X;
  long v_1098X;
  long v_1097X;
  long v_1096X;
  long v_1095X;
  long v_1094X;
  long v_1093X;
  long v_1092X;
  char b_1091X;
  char x_1090X;
  long c_1089X;
  long b_1088X;
  long c_1087X;
  long mid_c_1086X;
  long v_1085X;
  long v_1084X;
  long lo_c_1083X;
  long hi_b_1082X;
  long hi_a_1081X;
  long lo_b_1080X;
  long lo_a_1079X;
  long b_1078X;
  long args_1077X;
  char * arg_top_1076X;
  long list_arg_count_1075X;
  long list_args_1074X;
  long stack_nargs_1073X;
  long bytes_used_1072X;
  long count_1071X;
  long v_1070X;
  char * arg_1069X;
  char * loc_1068X;
  long args_1067X;
  long v_1066X;
  long cont_1065X;
  long index_1064X;
  long length_1063X;
  long wants_stack_args_1062X;
  long skip_1061X;
  long skip_1060X;
  long stack_space_1059X;
  long protocol_1058X;
  long n_1057X;
  long tem_1056X;
  char * addr_1055X;
  long x_1054X;
  long stack_arg_count_1053X;
  long back_1052X;
  long env_1051X;
  long offset_1050X;
  long i_1049X;
  long back_1048X;
  long env_1047X;
  long offset_1046X;
  long i_1045X;
  long obj_1044X;
  long obj_1043X;
  long type_1042X;
  long thing_1041X;
  long stuff_1040X;
  char * addr_1039X;
  long val_1038X;
  long x_1037X;
  long i_1036X;
  long b_1035X;
  long p_1034X;
  long port_1033X;
  long Kchar_1032X;
  long x_1031X;
  long i_1030X;
  long b_1029X;
  long p_1028X;
  long p_1027X;
  long port_1026X;
  char * addr_1025X;
  long val_1024X;
  long x_1023X;
  long i_1022X;
  long b_1021X;
  long p_1020X;
  long p_1019X;
  long port_1018X;
  char * addr_1017X;
  long value_1016X;
  long list_1015X;
  long head_1014X;
  char move_slowP_1013X;
  long slow_1012X;
  long list_1011X;
  long string_1010X;
  char * addr_1009X;
  long len_1008X;
  long val_1007X;
  long mseconds_1006X;
  long seconds_1005X;
  long option_1004X;
  long v_1003X;
  long v_1002X;
  long result_1001X;
  char * args_1000X;
  long name_999X;
  long proc_998X;
  long rest_list_997X;
  long x_996X;
  long tem_995X;
  long pc_994X;
  char * addr_993X;
  long x_992X;
  long proposal_991X;
  long value_990X;
  long vector_989X;
  long type_988X;
  char firstP_987X;
  long vector_986X;
  char firstP_985X;
  long h_984X;
  long i_983X;
  long x_982X;
  long status_981X;
  long reason_980X;
  char * addr_979X;
  long next_stob_978X;
  long i_977X;
  long v_976X;
  long next_stob_975X;
  long i_974X;
  long value_973X;
  char * addr_972X;
  long count_971X;
  long to_index_970X;
  long from_index_969X;
  long copies_968X;
  long left_967X;
  long value_966X;
  long value_965X;
  long verify_964X;
  long log_963X;
  long stob_962X;
  long i_961X;
  char * addr_960X;
  long old_959X;
  long x_958X;
  char * addr_957X;
  long channel_956X;
  long res_955X;
  long i_954X;
  long y_953X;
  long n_952X;
  char * addr_951X;
  long prev_950X;
  long ch_949X;
  long val_948X;
  long val_947X;
  long v_946X;
  long v_945X;
  long val_944X;
  char * addr_943X;
  char * addr_942X;
  long val_941X;
  long link_940X;
  long index_939X;
  long h_938X;
  long i_937X;
  long vector_936X;
  long vector_935X;
  char * addr_934X;
  long value_933X;
  long value_932X;
  long x_931X;
  char * addr_930X;
  long i_929X;
  long i_928X;
  long value_927X;
  long val_926X;
  long val_925X;
  long val_924X;
  long val_923X;
  char * v_922X;
  long value_921X;
  long needed_920X;
  long y_919X;
  long x_918X;
  long result_917X;
  long x_916X;
  long x_915X;
  long x_914X;
  long count_913X;
  long value_912X;
  long val_911X;
  long val_910X;
  long x_909X;
  long x_908X;
  long n_907X;
  long a_906X;
  long y_905X;
  long x_904X;
  long a_903X;
  long y_902X;
  long x_901X;
  long val_900X;
  long val_899X;
  long val_898X;
  long val_897X;
  long val_896X;
  long a_895X;
  long val_894X;
  long v_893X;
  char * v_892X;
  long a_891X;
  long Kdouble_890X;
  char * addr_889X;
  double x_888X;
  long value_887X;
  double y_886X;
  long value_885X;
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
  goto L23193;}
 L23193: {
  code_pointer_391X = arg1K0;
  switch ((*((unsigned char *) code_pointer_391X))) {
    case 0 : {
      push_exception_continuationB(14, 1);
      arg0K0 = 0;
      goto L21289;}
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
      goto L23193;}
      break;
    case 2 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 3 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 4 : {
      back_393X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_394X = SenvS;
      arg0K0 = env_394X;
      arg0K1 = back_393X;
      goto L30465;}
      break;
    case 5 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21289;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23193;}}
      break;
    case 6 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21289;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23193;}}
      break;
    case 7 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS)))))))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21289;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23193;}}
      break;
    case 8 : {
      back_395X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_396X = SenvS;
      arg0K0 = env_396X;
      arg0K1 = back_395X;
      goto L25160;}
      break;
    case 9 : {
      back_397X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_398X = SenvS;
      arg0K0 = env_398X;
      arg0K1 = back_397X;
      goto L25098;}
      break;
    case 10 : {
      location_399X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_399X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        push_exception_continuationB(1, 3);
        *((long *) (SstackS)) = location_399X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21289;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23193;}}
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
        goto L21289;}
      else {
        val_401X = SvalS;
        addr_402X = (((char *) (-3 + location_400X))) + 4;
        S48_WRITE_BARRIER(location_400X, addr_402X, val_401X);
        *((long *) addr_402X) = val_401X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23193;}}
      break;
    case 12 : {
      v_403X = *((unsigned char *) ((Scode_pointerS) + 3));
      if ((0 == v_403X)) {
        p_404X = SenvS;
        if ((3 == (3 & p_404X))) {
          if ((p_404X < (((long) (Sstack_beginS))))) {
            arg0K0 = 0;
            goto L25835;}
          else {
            if (((((long) (Sstack_endS))) < p_404X)) {
              arg0K0 = 0;
              goto L25835;}
            else {
              arg0K0 = ((Sstack_endS) - (Sstack_beginS));
              goto L25835;}}}
        else {
          arg0K0 = 0;
          goto L25835;}}
      else {
        s48_make_availableAgc(12);
        arg0K0 = (SvalS);
        arg0K1 = 0;
        goto L25848;}}
      break;
    case 13 : {
      count_405X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      s48_make_availableAgc((((((2 + count_405X) + (3 * count_405X)))<<2)));
      len_406X = 4 + (((count_405X)<<2));
      addr_407X = s48_allocate_small((4 + len_406X));
      *((long *) addr_407X) = (10 + (((len_406X)<<8)));
      vector_408X = 3 + (((long) (addr_407X + 4)));
      arg0K0 = 1;
      goto L24885;}
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
        goto L26172;}
      else {
        value_414X = SvalS;
        addr_415X = ((char *) (-3 + new_env_412X));
        S48_WRITE_BARRIER(new_env_412X, addr_415X, value_414X);
        *((long *) addr_415X) = value_414X;
        arg0K0 = 1;
        goto L26172;}}
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
        goto L26023;}
      else {
        value_421X = SvalS;
        addr_422X = ((char *) (-3 + new_env_419X));
        S48_WRITE_BARRIER(new_env_419X, addr_422X, value_421X);
        *((long *) addr_422X) = value_421X;
        arg0K0 = 1;
        goto L26023;}}
      break;
    case 16 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 17 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L21289;}
      else {
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23193;}}
      break;
    case 18 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L21289;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23193;}}
      break;
    case 19 : {
      SstackS = ((SstackS) + 4);
      SvalS = (*((long *) (SstackS)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 20 : {
      SvalS = (*((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 21 : {
      *((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 22 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (*((unsigned char *) ((Scode_pointerS) + 3))));
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 23 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))));
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 24 : {
      s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
      if ((1 == (ScontS))) {
        arg0K0 = 1;
        goto L46412;}
      else {
        merged_arg0K0 = 0;
        merged_arg0K1 = 2;
        really_preserve_continuation_return_tag = 0;
        goto really_preserve_continuation;
       really_preserve_continuation_return_0:
        v_423X = really_preserve_continuation0_return_value;
        arg0K0 = v_423X;
        goto L46412;}}
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
                goto L23850;}
              else {
                goto L23850;}}
            else {
              goto L23850;}}
          else {
            if (((127 & protocol_428X) == stack_arg_count_424X)) {
              proc_429X = SvalS;
              StemplateS = (*((long *) (((char *) (-3 + proc_429X)))));
              Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS))))))))) + 2);
              top_of_args_430X = SstackS;
              top_of_cont_431X = (((char *) (-3 + (ScontS)))) + -8;
              if ((top_of_cont_431X == (top_of_args_430X + (((stack_arg_count_424X)<<2))))) {
                s48_call_native_code(proc_429X);
                goto L23860;}
              else {
                SstackS = top_of_cont_431X;
                arg1K0 = (SstackS);
                arg1K1 = (top_of_args_430X + (((stack_arg_count_424X)<<2)));
                goto L7700;}}
            else {
              arg0K0 = stack_arg_count_424X;
              arg0K1 = 25;
              arg0K2 = 0;
              goto L20919;}}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_424X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L21791;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_424X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L21791;}}
      break;
    case 26 : {
      stack_arg_count_432X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      obj_433X = SvalS;
      if ((3 == (3 & obj_433X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_433X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_432X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L20919;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_432X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L21791;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_432X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L21791;}}
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
        goto L22419;}
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
        goto L21289;}}
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
        goto L14402;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_440X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_440X)))));
          arg0K1 = (-1 + stack_nargs_438X);
          goto L14402;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_440X))) + 4)))))) + 4)));
          arg0K1 = rest_list_440X;
          goto L14443;}}}
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
          goto L22364;}
        else {
          goto L22371;}}
      else {
        goto L22371;}}
      break;
    case 30 : {
      goto L24460;}
      break;
    case 31 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L45876;}
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
      goto L45876;}
      break;
    case 33 : {
      tem_444X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      StemplateS = tem_444X;
      Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_444X))))))));
      arg1K0 = (Scode_pointerS);
      goto L23193;}
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
            goto L23590;}
          else {
            goto L23590;}}
        else {
          goto L23590;}}
      else {
        v_446X = *((unsigned char *) ((((char *) (-3 + code_445X))) + 1));
        if ((67 == v_446X)) {
          if ((0 == (*((unsigned char *) ((((char *) (-3 + code_445X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_445X))) + -4))))>>8)))))))) {
            index_447X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_445X))) + -4))))>>8));
            space_448X = ((((*((unsigned char *) ((((char *) (-3 + code_445X))) + index_447X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_445X))) + (1 + index_447X))));
            if ((space_448X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
              goto L23590;}
            else {
              s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
              merged_arg0K0 = 0;
              copy_stack_into_heap_return_tag = 2;
              goto copy_stack_into_heap;
             copy_stack_into_heap_return_2:
              if ((space_448X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
                goto L23590;}
              else {
                ps_error("VM's stack is too small (how can this happen?)", 0);
                goto L23590;}}}
          else {
            goto L23633;}}
        else {
          goto L23633;}}}
      break;
    case 35 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L23193;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23193;}}
      break;
    case 36 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 37 : {
      if ((0 == (3 & (SvalS)))) {
        p_449X = SvalS;
        max_450X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_451X = ((p_449X)>>2);
        if ((val_451X < 0)) {
          goto L23491;}
        else {
          if ((val_451X < max_450X)) {
            index_452X = 1 + (((val_451X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_452X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_452X)))));
            goto L23493;}
          else {
            goto L23491;}}}
      else {
        push_exception_continuationB(5, 0);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21289;}}
      break;
    case 38 : {
      stack_nargs_453X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((0 == stack_nargs_453X)) {
        arg0_454X = *((long *) ((SstackS) + 12));
        rest_list_455X = *((long *) ((SstackS) + 8));
        *((long *) (SstackS)) = arg0_454X;
        SstackS = ((SstackS) + -4);
        SvalS = (*((long *) (((char *) (-3 + rest_list_455X)))));
        goto L24376;}
      else {
        arg1_456X = *((long *) ((SstackS) + (4 + (((stack_nargs_453X)<<2)))));
        arg0_457X = *((long *) ((SstackS) + (8 + (((stack_nargs_453X)<<2)))));
        *((long *) ((SstackS) + (8 + (((stack_nargs_453X)<<2))))) = 1;
        *((long *) ((SstackS) + 4)) = (-4 + (((stack_nargs_453X)<<2)));
        *((long *) (SstackS)) = arg0_457X;
        SstackS = ((SstackS) + -4);
        SvalS = arg1_456X;
        goto L24376;}}
      break;
    case 39 : {
      stack_nargs_458X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((stack_nargs_458X == 0)) {
        rest_list_459X = *((long *) ((SstackS) + 8));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_459X))) + 4))))) {
          arg0K0 = 1;
          goto L23389;}
        else {
          *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_459X))) + 4)));
          *((long *) ((SstackS) + 12)) = (SvalS);
          arg0K0 = -2;
          goto L23389;}}
      else {
        if ((stack_nargs_458X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 8))))) {
            arg0K0 = 1;
            goto L23389;}
          else {
            *((long *) ((SstackS) + 4)) = 0;
            *((long *) ((SstackS) + 12)) = (SvalS);
            arg0K0 = -2;
            goto L23389;}}
        else {
          *((long *) ((SstackS) + (8 + (((stack_nargs_458X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L23389;}}}
      break;
    case 40 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23193;}
      else {
        x_460X = (((*((long *) ((SstackS) + 4))))>>2);
        if ((x_460X == 0)) {
          rest_list_461X = *((long *) ((SstackS) + 8));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_461X))) + 4))))) {
            arg0K0 = 1;
            goto L24267;}
          else {
            *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_461X))) + 4)));
            *((long *) ((SstackS) + 12)) = (*((long *) (((char *) (-3 + rest_list_461X)))));
            arg0K0 = -2;
            goto L24267;}}
        else {
          if ((x_460X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 8))))) {
              arg0K0 = 1;
              goto L24267;}
            else {
              *((long *) ((SstackS) + 4)) = 0;
              arg0K0 = -2;
              goto L24267;}}
          else {
            arg0K0 = -2;
            goto L24267;}}}}
      break;
    case 41 : {
      SstackS = ((SstackS) + 4);
      arg2_462X = *((long *) (SstackS));
      x_463X = SvalS;
      if ((arg2_462X == x_463X)) {
        arg0K0 = 5;
        goto L46465;}
      else {
        arg0K0 = 1;
        goto L46465;}}
      break;
    case 42 : {
      x_464X = SvalS;
      if ((0 == (3 & x_464X))) {
        arg0K0 = 5;
        goto L46477;}
      else {
        if ((3 == (3 & x_464X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L46477;}
          else {
            goto L13087;}}
        else {
          goto L13087;}}}
      break;
    case 43 : {
      n_465X = SvalS;
      if ((0 == (3 & n_465X))) {
        goto L38158;}
      else {
        if ((3 == (3 & n_465X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_465X))) + -4))))>>2))))) {
            goto L38158;}
          else {
            goto L38159;}}
        else {
          goto L38159;}}}
      break;
    case 44 : {
      n_466X = SvalS;
      if ((0 == (3 & n_466X))) {
        goto L38315;}
      else {
        if ((3 == (3 & n_466X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
            goto L38315;}
          else {
            goto L38290;}}
        else {
          goto L38290;}}}
      break;
    case 45 : {
      n_467X = SvalS;
      if ((0 == (3 & n_467X))) {
        goto L38514;}
      else {
        if ((3 == (3 & n_467X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
            goto L38514;}
          else {
            goto L38489;}}
        else {
          goto L38489;}}}
      break;
    case 46 : {
      n_468X = SvalS;
      if ((0 == (3 & n_468X))) {
        goto L38713;}
      else {
        if ((3 == (3 & n_468X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
            goto L38713;}
          else {
            goto L38688;}}
        else {
          goto L38688;}}}
      break;
    case 47 : {
      n_469X = SvalS;
      if ((0 == (3 & n_469X))) {
        goto L31961;}
      else {
        if ((3 == (3 & n_469X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_469X))) + -4))))>>2))))) {
            goto L31961;}
          else {
            goto L31962;}}
        else {
          goto L31962;}}}
      break;
    case 48 : {
      x_470X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_470X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 49 : {
      x_471X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_471X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 50 : {
      SstackS = ((SstackS) + 4);
      x_472X = *((long *) (SstackS));
      y_473X = SvalS;
      if ((0 == (3 & (x_472X | y_473X)))) {
        s48_make_availableAgc(16);
        x_474X = (((x_472X)>>2)) + (((y_473X)>>2));
        if ((536870911 < x_474X)) {
          goto L32087;}
        else {
          if ((x_474X < -536870912)) {
            goto L32087;}
          else {
            arg0K0 = (((x_474X)<<2));
            goto L32033;}}}
      else {
        if ((0 == (3 & x_472X))) {
          goto L32039;}
        else {
          if ((3 == (3 & x_472X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_472X))) + -4))))>>2))))) {
              goto L32039;}
            else {
              goto L32048;}}
          else {
            goto L32048;}}}}
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
          goto L7847;}
        else {
          arg0K0 = a_477X;
          goto L7847;}}
      else {
        if ((0 == (3 & x_475X))) {
          goto L40460;}
        else {
          if ((3 == (3 & x_475X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_475X))) + -4))))>>2))))) {
              goto L40460;}
            else {
              goto L40469;}}
          else {
            goto L40469;}}}}
      break;
    case 52 : {
      SstackS = ((SstackS) + 4);
      x_479X = *((long *) (SstackS));
      y_480X = SvalS;
      if ((0 == (3 & (x_479X | y_480X)))) {
        s48_make_availableAgc(16);
        x_481X = (((x_479X)>>2)) - (((y_480X)>>2));
        if ((536870911 < x_481X)) {
          goto L32362;}
        else {
          if ((x_481X < -536870912)) {
            goto L32362;}
          else {
            arg0K0 = (((x_481X)<<2));
            goto L32322;}}}
      else {
        if ((0 == (3 & x_479X))) {
          goto L32328;}
        else {
          if ((3 == (3 & x_479X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_479X))) + -4))))>>2))))) {
              goto L32328;}
            else {
              goto L32337;}}
          else {
            goto L32337;}}}}
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
        goto L21289;}
      else {
        if ((0 == (3 & (x_482X | y_483X)))) {
          if ((0 == y_483X)) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = x_482X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_483X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21289;}
          else {
            a_484X = ((x_482X)>>2);
            b_485X = ((y_483X)>>2);
            if ((a_484X < 0)) {
              arg0K0 = (0 - a_484X);
              goto L8105;}
            else {
              arg0K0 = a_484X;
              goto L8105;}}}
        else {
          if ((0 == (3 & x_482X))) {
            goto L40615;}
          else {
            if ((3 == (3 & x_482X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_482X))) + -4))))>>2))))) {
                goto L40615;}
              else {
                goto L40648;}}
            else {
              goto L40648;}}}}}
      break;
    case 54 : {
      SstackS = ((SstackS) + 4);
      x_486X = *((long *) (SstackS));
      y_487X = SvalS;
      if ((0 == (3 & (x_486X | y_487X)))) {
        if ((x_486X == y_487X)) {
          arg0K0 = 5;
          goto L32509;}
        else {
          arg0K0 = 1;
          goto L32509;}}
      else {
        if ((0 == (3 & x_486X))) {
          goto L32515;}
        else {
          if ((3 == (3 & x_486X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_486X))) + -4))))>>2))))) {
              goto L32515;}
            else {
              goto L32526;}}
          else {
            goto L32526;}}}}
      break;
    case 55 : {
      SstackS = ((SstackS) + 4);
      x_488X = *((long *) (SstackS));
      y_489X = SvalS;
      if ((0 == (3 & (x_488X | y_489X)))) {
        if ((x_488X < y_489X)) {
          arg0K0 = 5;
          goto L32686;}
        else {
          arg0K0 = 1;
          goto L32686;}}
      else {
        if ((0 == (3 & x_488X))) {
          goto L32692;}
        else {
          if ((3 == (3 & x_488X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_488X))) + -4))))>>2))))) {
              goto L32692;}
            else {
              goto L32703;}}
          else {
            goto L32703;}}}}
      break;
    case 56 : {
      SstackS = ((SstackS) + 4);
      x_490X = *((long *) (SstackS));
      y_491X = SvalS;
      if ((0 == (3 & (x_490X | y_491X)))) {
        if ((y_491X < x_490X)) {
          arg0K0 = 5;
          goto L32926;}
        else {
          arg0K0 = 1;
          goto L32926;}}
      else {
        if ((0 == (3 & x_490X))) {
          goto L32932;}
        else {
          if ((3 == (3 & x_490X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_490X))) + -4))))>>2))))) {
              goto L32932;}
            else {
              goto L32943;}}
          else {
            goto L32943;}}}}
      break;
    case 57 : {
      SstackS = ((SstackS) + 4);
      x_492X = *((long *) (SstackS));
      y_493X = SvalS;
      if ((0 == (3 & (x_492X | y_493X)))) {
        if ((y_493X < x_492X)) {
          arg0K0 = 1;
          goto L33166;}
        else {
          arg0K0 = 5;
          goto L33166;}}
      else {
        if ((0 == (3 & x_492X))) {
          goto L33172;}
        else {
          if ((3 == (3 & x_492X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_492X))) + -4))))>>2))))) {
              goto L33172;}
            else {
              goto L33183;}}
          else {
            goto L33183;}}}}
      break;
    case 58 : {
      SstackS = ((SstackS) + 4);
      x_494X = *((long *) (SstackS));
      y_495X = SvalS;
      if ((0 == (3 & (x_494X | y_495X)))) {
        if ((x_494X < y_495X)) {
          arg0K0 = 1;
          goto L33360;}
        else {
          arg0K0 = 5;
          goto L33360;}}
      else {
        if ((0 == (3 & x_494X))) {
          goto L33366;}
        else {
          if ((3 == (3 & x_494X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_494X))) + -4))))>>2))))) {
              goto L33366;}
            else {
              goto L33377;}}
          else {
            goto L33377;}}}}
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
        goto L21289;}
      else {
        if ((0 == (3 & (x_496X | y_497X)))) {
          if ((0 == y_497X)) {
            arg0K0 = x_496X;
            arg0K1 = y_497X;
            goto L18554;}
          else {
            a_498X = ((x_496X)>>2);
            b_499X = ((y_497X)>>2);
            if ((a_498X < 0)) {
              arg0K0 = (0 - a_498X);
              goto L8750;}
            else {
              arg0K0 = a_498X;
              goto L8750;}}}
        else {
          if ((0 == (3 & x_496X))) {
            goto L33566;}
          else {
            if ((3 == (3 & x_496X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_496X))) + -4))))>>2))))) {
                goto L33566;}
              else {
                goto L33575;}}
            else {
              goto L33575;}}}}}
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
        goto L21289;}
      else {
        if ((0 == (3 & (x_500X | y_501X)))) {
          if ((0 == y_501X)) {
            arg0K0 = x_500X;
            arg0K1 = y_501X;
            goto L18463;}
          else {
            a_502X = ((x_500X)>>2);
            if ((a_502X < 0)) {
              arg0K0 = (0 - a_502X);
              goto L33780;}
            else {
              arg0K0 = a_502X;
              goto L33780;}}}
        else {
          if ((0 == (3 & x_500X))) {
            goto L33742;}
          else {
            if ((3 == (3 & x_500X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_500X))) + -4))))>>2))))) {
                goto L33742;}
              else {
                goto L33751;}}
            else {
              goto L33751;}}}}}
      break;
    case 61 : {
      n_503X = SvalS;
      if ((0 == (3 & n_503X))) {
        goto L33929;}
      else {
        if ((3 == (3 & n_503X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_503X))) + -4))))>>2))))) {
            goto L33929;}
          else {
            goto L33930;}}
        else {
          goto L33930;}}}
      break;
    case 62 : {
      n_504X = SvalS;
      if ((0 == (3 & n_504X))) {
        goto L33984;}
      else {
        if ((3 == (3 & n_504X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_504X))) + -4))))>>2))))) {
            goto L33984;}
          else {
            goto L33985;}}
        else {
          goto L33985;}}}
      break;
    case 63 : {
      n_505X = SvalS;
      if ((0 == (3 & n_505X))) {
        goto L34039;}
      else {
        if ((3 == (3 & n_505X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_505X))) + -4))))>>2))))) {
            goto L34039;}
          else {
            goto L34042;}}
        else {
          goto L34042;}}}
      break;
    case 64 : {
      n_506X = SvalS;
      if ((0 == (3 & n_506X))) {
        goto L34099;}
      else {
        if ((3 == (3 & n_506X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_506X))) + -4))))>>2))))) {
            goto L34099;}
          else {
            goto L34100;}}
        else {
          goto L34100;}}}
      break;
    case 65 : {
      n_507X = SvalS;
      if ((0 == (3 & n_507X))) {
        goto L34154;}
      else {
        if ((3 == (3 & n_507X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_507X))) + -4))))>>2))))) {
            goto L34154;}
          else {
            goto L34157;}}
        else {
          goto L34157;}}}
      break;
    case 66 : {
      x_508X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_508X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 67 : {
      x_509X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_509X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 68 : {
      x_510X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_510X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 69 : {
      x_511X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_511X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 70 : {
      x_512X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_512X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 71 : {
      x_513X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_513X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 72 : {
      x_514X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_514X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 73 : {
      x_515X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_515X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 74 : {
      x_516X = SvalS;
      if ((0 == (3 & x_516X))) {
        if ((x_516X < 0)) {
          goto L46631;}
        else {
          goto L46637;}}
      else {
        v_517X = s48_bignum_test((((char *) (-3 + x_516X))));
        if ((-1 == v_517X)) {
          goto L46631;}
        else {
          goto L46637;}}}
      break;
    case 75 : {
      x_518X = SvalS;
      if ((0 == (3 & x_518X))) {
        goto L22040;}
      else {
        if ((3 == (3 & x_518X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_518X))) + -4))))>>2))))) {
            goto L22040;}
          else {
            goto L22043;}}
        else {
          goto L22043;}}}
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
      goto L21289;}
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
      goto L21289;}
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
      goto L21289;}
      break;
    case 79 : {
      x_525X = SvalS;
      if ((0 == (3 & x_525X))) {
        SvalS = (~ (3 | x_525X));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23193;}
      else {
        if ((0 == (3 & x_525X))) {
          goto L34308;}
        else {
          if ((3 == (3 & x_525X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_525X))) + -4))))>>2))))) {
              goto L34308;}
            else {
              goto L34311;}}
          else {
            goto L34311;}}}}
      break;
    case 80 : {
      x_526X = SvalS;
      if ((0 == (3 & x_526X))) {
        x_527X = ((x_526X)>>2);
        if ((x_527X < 0)) {
          arg0K0 = (~ x_527X);
          goto L34399;}
        else {
          arg0K0 = x_527X;
          goto L34399;}}
      else {
        if ((0 == (3 & x_526X))) {
          goto L34382;}
        else {
          if ((3 == (3 & x_526X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_526X))) + -4))))>>2))))) {
              goto L34382;}
            else {
              goto L34385;}}
          else {
            goto L34385;}}}}
      break;
    case 81 : {
      SstackS = ((SstackS) + 4);
      x_528X = *((long *) (SstackS));
      y_529X = SvalS;
      if ((0 == (3 & (x_528X | y_529X)))) {
        SvalS = (x_528X & y_529X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23193;}
      else {
        if ((0 == (3 & x_528X))) {
          goto L34484;}
        else {
          if ((3 == (3 & x_528X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_528X))) + -4))))>>2))))) {
              goto L34484;}
            else {
              goto L34493;}}
          else {
            goto L34493;}}}}
      break;
    case 82 : {
      SstackS = ((SstackS) + 4);
      x_530X = *((long *) (SstackS));
      y_531X = SvalS;
      if ((0 == (3 & (x_530X | y_531X)))) {
        SvalS = (x_530X | y_531X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23193;}
      else {
        if ((0 == (3 & x_530X))) {
          goto L34647;}
        else {
          if ((3 == (3 & x_530X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_530X))) + -4))))>>2))))) {
              goto L34647;}
            else {
              goto L34656;}}
          else {
            goto L34656;}}}}
      break;
    case 83 : {
      SstackS = ((SstackS) + 4);
      x_532X = *((long *) (SstackS));
      y_533X = SvalS;
      if ((0 == (3 & (x_532X | y_533X)))) {
        SvalS = (x_532X ^ y_533X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23193;}
      else {
        if ((0 == (3 & x_532X))) {
          goto L34810;}
        else {
          if ((3 == (3 & x_532X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_532X))) + -4))))>>2))))) {
              goto L34810;}
            else {
              goto L34819;}}
          else {
            goto L34819;}}}}
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
            goto L21289;}
          else {
            if ((0 == (3 & x_534X))) {
              if ((x_534X < 0)) {
                arg0K0 = -4;
                goto L31741;}
              else {
                arg0K0 = 0;
                goto L31741;}}
            else {
              if ((3 == (3 & x_534X))) {
                if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_534X))) + -4))))>>2))))) {
                  v_537X = s48_bignum_test((((char *) (-3 + x_534X))));
                  if ((1 == v_537X)) {
                    arg0K0 = 0;
                    goto L31757;}
                  else {
                    arg0K0 = -4;
                    goto L31757;}}
                else {
                  goto L31758;}}
              else {
                goto L31758;}}}}
        else {
          goto L40848;}}
      else {
        goto L40848;}}
      break;
    case 85 : {
      x_538X = SvalS;
      if ((9 == (255 & x_538X))) {
        arg0K0 = 5;
        goto L46719;}
      else {
        arg0K0 = 1;
        goto L46719;}}
      break;
    case 86 : {
      SstackS = ((SstackS) + 4);
      arg2_539X = *((long *) (SstackS));
      if ((9 == (255 & arg2_539X))) {
        if ((9 == (255 & (SvalS)))) {
          x_540X = SvalS;
          if ((arg2_539X == x_540X)) {
            arg0K0 = 5;
            goto L37933;}
          else {
            arg0K0 = 1;
            goto L37933;}}
        else {
          goto L37906;}}
      else {
        goto L37906;}}
      break;
    case 87 : {
      SstackS = ((SstackS) + 4);
      arg2_541X = *((long *) (SstackS));
      if ((9 == (255 & arg2_541X))) {
        if ((9 == (255 & (SvalS)))) {
          x_542X = SvalS;
          if ((arg2_541X < x_542X)) {
            arg0K0 = 5;
            goto L37843;}
          else {
            arg0K0 = 1;
            goto L37843;}}
        else {
          goto L37816;}}
      else {
        goto L37816;}}
      break;
    case 88 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23193;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21289;}}
      break;
    case 89 : {
      if ((0 == (3 & (SvalS)))) {
        x_543X = (((SvalS))>>2);
        if ((255 < x_543X)) {
          goto L42038;}
        else {
          if ((x_543X < 0)) {
            goto L42038;}
          else {
            SvalS = (9 + (((((x_543X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23193;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21289;}}
      break;
    case 90 : {
      x_544X = SvalS;
      if ((21 == x_544X)) {
        arg0K0 = 5;
        goto L46748;}
      else {
        arg0K0 = 1;
        goto L46748;}}
      break;
    case 91 : {
      x_545X = SvalS;
      type_546X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_545X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_545X))) + -4))))>>2))) == type_546X)) {
          arg0K0 = 5;
          goto L46762;}
        else {
          arg0K0 = 1;
          goto L46762;}}
      else {
        arg0K0 = 1;
        goto L46762;}}
      break;
    case 92 : {
      stob_547X = SvalS;
      type_548X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_547X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_547X))) + -4))))>>2))) == type_548X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_547X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L23193;}
        else {
          goto L26360;}}
      else {
        goto L26360;}}
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
        goto L26465;}
      else {
        *((long *) ((((char *) (-3 + new_553X))) + (-4 + (((len_549X)<<2))))) = (SvalS);
        arg0K0 = (-2 + len_549X);
        goto L26449;}}
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
      goto L26581;}
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
          goto L23193;}
        else {
          goto L26741;}}
      else {
        goto L26741;}}
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
                goto L26858;}
              else {
                if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                  goto L26858;}
                else {
                  merged_arg0K0 = stob_564X;
                  merged_arg0K1 = (((offset_566X)<<2));
                  merged_arg0K2 = value_565X;
                  proposal_d_write_return_tag = 0;
                  goto proposal_d_write;
                 proposal_d_write_return_0:
                  goto L26867;}}}
            else {
              goto L26868;}}
          else {
            goto L26868;}}
        else {
          goto L26868;}}
      else {
        goto L26868;}}
      break;
    case 97 : {
      SstackS = ((SstackS) + 4);
      len_569X = *((long *) (SstackS));
      init_570X = SvalS;
      type_571X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & len_569X))) {
        len_572X = ((len_569X)>>2);
        if ((len_572X < 0)) {
          goto L27085;}
        else {
          if ((4194304 < len_572X)) {
            goto L27085;}
          else {
            Stemp0S = init_570X;
            len_573X = ((len_572X)<<2);
            addr_574X = s48_allocate_tracedAgc((4 + len_573X));
            if ((addr_574X == NULL)) {
              arg0K0 = 1;
              goto L27102;}
            else {
              *((long *) addr_574X) = (2 + (((((((len_573X)<<6)) + type_571X))<<2)));
              arg0K0 = (3 + (((long) (addr_574X + 4))));
              goto L27102;}}}}
      else {
        push_exception_continuationB(5, 2);
        *((long *) (SstackS)) = (((type_571X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = len_569X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = init_570X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L21289;}}
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
              goto L27375;}
            else {
              if ((index_579X < len_578X)) {
                v_580X = *((unsigned char *) ((Scode_pointerS) + 2));
                if ((0 == v_580X)) {
                  goto L27365;}
                else {
                  if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                    goto L27365;}
                  else {
                    merged_arg0K0 = stob_575X;
                    merged_arg0K1 = index_576X;
                    proposal_d_read_return_tag = 0;
                    goto proposal_d_read;
                   proposal_d_read_return_0:
                    v_581X = proposal_d_read0_return_value;
                    arg0K0 = v_581X;
                    goto L27374;}}}
              else {
                goto L27375;}}}
          else {
            goto L27330;}}
        else {
          goto L27330;}}
      else {
        goto L27330;}}
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
                  goto L27662;}
                else {
                  if ((index_587X < len_586X)) {
                    v_588X = *((unsigned char *) ((Scode_pointerS) + 2));
                    if ((0 == v_588X)) {
                      goto L27652;}
                    else {
                      if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                        goto L27652;}
                      else {
                        merged_arg0K0 = stob_583X;
                        merged_arg0K1 = arg2_582X;
                        merged_arg0K2 = value_584X;
                        proposal_d_write_return_tag = 1;
                        goto proposal_d_write;
                       proposal_d_write_return_1:
                        goto L27661;}}}
                  else {
                    goto L27662;}}}
              else {
                goto L27615;}}
            else {
              goto L27615;}}
          else {
            goto L27615;}}
        else {
          goto L27615;}}
      else {
        goto L27615;}}
      break;
    case 100 : {
      SstackS = ((SstackS) + 4);
      arg2_589X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_589X | (SvalS))))) {
        len_590X = ((arg2_589X)>>2);
        init_591X = (((SvalS))>>2);
        if ((len_590X < 0)) {
          goto L35031;}
        else {
          if ((4194304 < ((((3 + len_590X))>>2)))) {
            goto L35031;}
          else {
            addr_592X = s48_allocate_untracedAgc((4 + len_590X));
            if ((addr_592X == NULL)) {
              arg0K0 = 1;
              goto L35044;}
            else {
              *((long *) addr_592X) = (74 + (((len_590X)<<8)));
              arg0K0 = (3 + (((long) (addr_592X + 4))));
              goto L35044;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_589X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
      break;
    case 101 : {
      obj_593X = SvalS;
      if ((3 == (3 & obj_593X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + obj_593X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23193;}
        else {
          goto L45543;}}
      else {
        goto L45543;}}
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
              goto L42144;}
            else {
              if ((index_595X < len_596X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_594X))) + index_595X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23193;}
              else {
                goto L42144;}}}
          else {
            goto L45059;}}
        else {
          goto L45059;}}
      else {
        goto L45059;}}
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
                  goto L39682;}
                else {
                  if ((index_599X < len_601X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_598X))) + index_599X)) = Kchar_600X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L23193;}
                  else {
                    goto L39682;}}}
              else {
                goto L39661;}}
            else {
              goto L39661;}}
          else {
            goto L43839;}}
        else {
          goto L43839;}}
      else {
        goto L43839;}}
      break;
    case 104 : {
      SstackS = ((SstackS) + 4);
      arg2_602X = *((long *) (SstackS));
      if ((0 == (3 & arg2_602X))) {
        if ((9 == (255 & (SvalS)))) {
          len_603X = ((arg2_602X)>>2);
          init_604X = ((((SvalS))>>8));
          if ((len_603X < 0)) {
            goto L35211;}
          else {
            if ((4194304 < ((((4 + len_603X))>>2)))) {
              goto L35211;}
            else {
              len_605X = 1 + len_603X;
              addr_606X = s48_allocate_untracedAgc((4 + len_605X));
              if ((addr_606X == NULL)) {
                arg0K0 = 1;
                goto L35224;}
              else {
                *((long *) addr_606X) = (70 + (((len_605X)<<8)));
                arg0K0 = (3 + (((long) (addr_606X + 4))));
                goto L35224;}}}}
        else {
          goto L42225;}}
      else {
        goto L42225;}}
      break;
    case 105 : {
      obj_607X = SvalS;
      if ((3 == (3 & obj_607X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_607X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23193;}
        else {
          goto L45593;}}
      else {
        goto L45593;}}
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
              goto L42288;}
            else {
              if ((index_609X < len_610X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_608X))) + index_609X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23193;}
              else {
                goto L42288;}}}
          else {
            goto L45155;}}
        else {
          goto L45155;}}
      else {
        goto L45155;}}
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
                    goto L39837;}
                  else {
                    if ((index_613X < len_615X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_612X))) + index_613X)) = (Kchar_614X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L23193;}
                    else {
                      goto L39837;}}}
                else {
                  goto L39816;}}
              else {
                goto L39816;}}
            else {
              goto L43965;}}
          else {
            goto L43965;}}
        else {
          goto L43965;}}
      else {
        goto L43965;}}
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
          goto L19844;}
        else {
          goto L31903;}}
      else {
        goto L31903;}}
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
              goto L43529;}
            else {
              arg0K0 = 1;
              goto L43529;}}
          else {
            arg0K0 = 5;
            goto L43529;}}
        else {
          goto L43510;}}
      else {
        goto L43510;}}
      break;
    case 110 : {
      SstackS = ((SstackS) + 4);
      arg2_623X = *((long *) (SstackS));
      if ((3 == (3 & arg2_623X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_623X))) + -4))))>>2))))) {
          x_624X = SvalS;
          if ((1 == x_624X)) {
            goto L39424;}
          else {
            if ((5 == x_624X)) {
              goto L39424;}
            else {
              goto L39431;}}}
        else {
          goto L39431;}}
      else {
        goto L39431;}}
      break;
    case 111 : {
      x_625X = SvalS;
      if ((3 == (3 & x_625X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_625X))) + -4)))))) {
          arg0K0 = 1;
          goto L46834;}
        else {
          arg0K0 = 5;
          goto L46834;}}
      else {
        arg0K0 = 5;
        goto L46834;}}
      break;
    case 112 : {
      x_626X = SvalS;
      if ((3 == (3 & x_626X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_626X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_626X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_626X))) + -4))));
          goto L42370;}
        else {
          goto L42370;}}
      else {
        goto L42370;}}
      break;
    case 113 : {
      s48_make_availableAgc(288);
      SstackS = ((SstackS) + 4);
      arg2_627X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        mode_628X = (((SvalS))>>2);
        if ((1 == mode_628X)) {
          goto L35618;}
        else {
          if ((2 == mode_628X)) {
            goto L35618;}
          else {
            if ((3 == mode_628X)) {
              goto L35618;}
            else {
              if ((4 == mode_628X)) {
                goto L35618;}
              else {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = arg2_627X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = (((mode_628X)<<2));
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21289;}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_627X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
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
            goto L21289;}
          else {
            status_631X = close_channelB(channel_630X);
            if ((status_631X == NO_ERRORS)) {
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23193;}
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
              goto L21289;}}}
        else {
          goto L46868;}}
      else {
        goto L46868;}}
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
              goto L42495;}
            else {
              if ((5 == x_637X)) {
                goto L42495;}
              else {
                goto L42506;}}}
          else {
            goto L42506;}}
        else {
          goto L42506;}}
      else {
        goto L42506;}}
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
                  goto L36257;}
                else {
                  goto L36432;}}
              else {
                goto L36432;}}
            else {
              goto L36311;}}
          else {
            goto L42701;}}
        else {
          goto L42701;}}
      else {
        goto L42701;}}
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
            goto L21289;}
          else {
            readyP_646X = ps_check_fd(((((*((long *) ((((char *) (-3 + channel_645X))) + 8))))>>2)), (4 == (*((long *) (((char *) (-3 + channel_645X)))))), &status_647X);
            if ((status_647X == NO_ERRORS)) {
              if (readyP_646X) {
                arg0K0 = 5;
                goto L39015;}
              else {
                arg0K0 = 1;
                goto L39015;}}
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
              goto L21289;}}}
        else {
          goto L46890;}}
      else {
        goto L46890;}}
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
            goto L42843;}
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
                goto L10628;}
              else {
                goto L10628;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_651X))) + 12)));
              arg0K1 = head_651X;
              goto L10634;}}}
        else {
          goto L42826;}}
      else {
        goto L42826;}}
      break;
    case 119 : {
      s48_make_availableAgc(((((3 * (Snumber_of_channelsS)))<<2)));
      arg0K0 = (-1 + (Snumber_of_channelsS));
      arg0K1 = 25;
      goto L15332;}
      break;
    case 120 : {
      SvalS = (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 121 : {
      proposal_657X = SvalS;
      if ((1 == proposal_657X)) {
        goto L36565;}
      else {
        if ((3 == (3 & proposal_657X))) {
          if ((2 == (31 & ((((*((long *) ((((char *) (-3 + proposal_657X))) + -4))))>>2))))) {
            if ((4 == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proposal_657X))) + -4))))>>8))))>>2)))) {
              if ((1 == (*((long *) (((char *) (-3 + proposal_657X))))))) {
                goto L36565;}
              else {
                goto L36596;}}
            else {
              goto L36596;}}
          else {
            goto L36596;}}
        else {
          goto L36596;}}}
      break;
    case 122 : {
      proposal_658X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
      if ((1 == proposal_658X)) {
        push_exception_continuationB(26, 1);
        arg0K0 = 0;
        goto L21289;}
      else {
        GET_PROPOSAL_LOCK();
        log_659X = *((long *) ((((char *) (-3 + proposal_658X))) + 4));
        arg0K0 = 0;
        goto L9037;}}
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
            goto L27960;}
          else {
            merged_arg0K0 = stob_660X;
            merged_arg0K1 = (((offset_661X)<<2));
            proposal_d_read_return_tag = 1;
            goto proposal_d_read;
           proposal_d_read_return_1:
            v_664X = proposal_d_read0_return_value;
            arg0K0 = v_664X;
            goto L27960;}}
        else {
          goto L27961;}}
      else {
        goto L27961;}}
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
                  goto L28054;}
                else {
                  if ((0 == v_672X)) {
                    goto L28114;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L28114;}
                    else {
                      arg0K0 = 4096;
                      arg0K1 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 12)));
                      goto L28202;}}}}
              else {
                goto L28054;}}
            else {
              goto L28054;}}
          else {
            goto L28054;}}
        else {
          goto L28054;}}
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
        goto L21289;}}
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
              goto L28411;}
            else {
              if ((index_676X < len_677X)) {
                x_678X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                if ((1 == x_678X)) {
                  arg0K0 = ((((*((unsigned char *) ((((char *) (-3 + arg2_675X))) + index_676X))))<<2));
                  goto L28410;}
                else {
                  index_679X = ((index_676X)<<2);
                  log_680X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                  arg0K0 = 0;
                  goto L13897;}}
              else {
                goto L28411;}}}
          else {
            goto L31209;}}
        else {
          goto L31209;}}
      else {
        goto L31209;}}
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
                    goto L28537;}
                  else {
                    if ((index_683X < len_685X)) {
                      x_686X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                      if ((1 == x_686X)) {
                        *((unsigned char *) ((((char *) (-3 + arg3_682X))) + index_683X)) = (((byte_684X)>>2));
                        goto L28536;}
                      else {
                        index_687X = ((index_683X)<<2);
                        log_688X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                        arg0K0 = 0;
                        goto L14073;}}
                    else {
                      goto L28537;}}}
                else {
                  goto L28502;}}
              else {
                goto L28502;}}
            else {
              goto L28502;}}
          else {
            goto L31312;}}
        else {
          goto L31312;}}
      else {
        goto L31312;}}
      break;
    case 127 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 128 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 129 : {
      x_689X = SvalS;
      push_exception_continuationB(15, 1);
      *((long *) (SstackS)) = x_689X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
      break;
    case 130 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 131 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
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
                          goto L23193;}
                        else {
                          arg0K0 = 24;
                          arg0K1 = status_699X;
                          goto L40065;}}
                      else {
                        status_700X = ps_close(port_695X);
                        if ((status_700X == NO_ERRORS)) {
                          arg0K0 = 24;
                          arg0K1 = status_698X;
                          goto L40065;}
                        else {
                          ps_write_string("Unable to close image file", (stderr));
                          { long ignoreXX;
                          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                          arg0K0 = 24;
                          arg0K1 = status_698X;
                          goto L40065;}}}
                    else {
                      status_701X = ps_close(port_695X);
                      if ((status_701X == NO_ERRORS)) {
                        arg0K0 = 24;
                        arg0K1 = status_697X;
                        goto L40065;}
                      else {
                        ps_write_string("Unable to close image file", (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg0K0 = 24;
                        arg0K1 = status_697X;
                        goto L40065;}}}
                  else {
                    arg0K0 = 10;
                    arg0K1 = status_696X;
                    goto L40065;}}
                else {
                  goto L44162;}}
              else {
                goto L44162;}}
            else {
              goto L44162;}}
          else {
            goto L44162;}}
        else {
          goto L44162;}}
      else {
        goto L44162;}}
      break;
    case 133 : {
      SvalS = 13;
      s48_collect();
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 134 : {
      obj_702X = SvalS;
      if ((3 == (3 & obj_702X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_702X))) + -4))))>>2))))) {
          x_703X = SvalS;
          n_704X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_703X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L44803;}
        else {
          goto L44780;}}
      else {
        goto L44780;}}
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
            goto L23193;}
          else {
            goto L40340;}}
        else {
          goto L40340;}}
      else {
        goto L40340;}}
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
          goto L23193;}
        else {
          if ((0 == key_713X)) {
            x_715X = s48_available();
            SvalS = (((x_715X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23193;}
          else {
            if ((1 == key_713X)) {
              bytes_716X = s48_heap_size();
              SvalS = (-4 & (3 + bytes_716X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23193;}
            else {
              if ((2 == key_713X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23193;}
              else {
                if ((3 == key_713X)) {
                  x_717X = s48_gc_count();
                  SvalS = (((x_717X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L23193;}
                else {
                  if ((4 == key_713X)) {
                    push_exception_continuationB(14, 1);
                    *((long *) (SstackS)) = (((key_713X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_714X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L21289;}
                  else {
                    push_exception_continuationB(17, 1);
                    *((long *) (SstackS)) = (((key_713X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_714X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L21289;}}}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_712X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
      break;
    case 137 : {
      if ((0 == (3 & (SvalS)))) {
        type_718X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L42962;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21289;}}
      break;
    case 138 : {
      x_719X = SvalS;
      arg2K0 = 1;
      arg0K1 = x_719X;
      goto L46992;}
      break;
    case 139 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 140 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 141 : {
      val_720X = SHARED_REF((Ssession_dataS));
      SvalS = val_720X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 142 : {
      SHARED_SETB((Ssession_dataS), (SvalS));
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
      break;
    case 143 : {
      obj_721X = SvalS;
      if ((3 == (3 & obj_721X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_721X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 165)) {
            goto L25655;}
          else {
            temp_722X = SHARED_REF((Sexception_handlersS));
            SHARED_SETB((Sexception_handlersS), (SvalS));
            SvalS = temp_722X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23193;}}
        else {
          goto L25655;}}
      else {
        goto L25655;}}
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
      goto L23193;}
      break;
    case 145 : {
      obj_726X = SvalS;
      if ((3 == (3 & obj_726X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_726X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 5)) {
            goto L24119;}
          else {
            temp_727X = SHARED_REF((Sinterrupt_handlersS));
            SHARED_SETB((Sinterrupt_handlersS), (SvalS));
            SvalS = temp_727X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23193;}}
        else {
          goto L24119;}}
      else {
        goto L24119;}}
      break;
    case 146 : {
      old_728X = Senabled_interruptsS;
      p_729X = SvalS;
      Senabled_interruptsS = (((p_729X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L47040;}
        else {
          goto L47040;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L47040;}}
      break;
    case 147 : {
      SstackS = ((SstackS) + 4);
      p_730X = *((long *) (SstackS));
      Senabled_interruptsS = (((p_730X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L23207;}
        else {
          goto L23207;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L23207;}}
      break;
    case 148 : {
      if ((0 == (3 & (SvalS)))) {
        p_731X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          s48_Spending_interruptPS = 0;
          if ((s48_Spending_eventsPS)) {
            s48_Spending_interruptPS = 1;
            goto L45728;}
          else {
            goto L45728;}}
        else {
          s48_Spending_interruptPS = 1;
          goto L45728;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21289;}}
      break;
    case 149 : {
      SstackS = ((SstackS) + 4);
      arg2_732X = *((long *) (SstackS));
      if ((0 == (3 & arg2_732X))) {
        x_733X = SvalS;
        if ((1 == x_733X)) {
          goto L43712;}
        else {
          if ((5 == x_733X)) {
            goto L43712;}
          else {
            goto L43717;}}}
      else {
        goto L43717;}}
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
        goto L21289;}
      else {
        arg0K0 = rest_list_736X;
        goto L28736;}}
      break;
    case 151 : {
      s48_make_availableAgc(20);
      SstackS = ((SstackS) + 4);
      arg2_737X = *((long *) (SstackS));
      if ((3 == (3 & arg2_737X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_737X))) + -4))))>>2))))) {
          x_738X = SvalS;
          if ((1 == x_738X)) {
            goto L43057;}
          else {
            if ((5 == x_738X)) {
              goto L43057;}
            else {
              goto L43062;}}}
        else {
          goto L43062;}}
      else {
        goto L43062;}}
      break;
    case 152 : {
      SstackS = ((SstackS) + 4);
      arg2_739X = *((long *) (SstackS));
      if ((3 == (3 & arg2_739X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_739X))) + -4))))>>2))))) {
          x_740X = SvalS;
          if ((1 == x_740X)) {
            goto L30547;}
          else {
            if ((5 == x_740X)) {
              goto L30547;}
            else {
              goto L30552;}}}
        else {
          goto L30552;}}
      else {
        goto L30552;}}
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
          goto L23193;}
        else {
          if ((0 == option_742X)) {
            seconds_745X = s48_run_time(&mseconds_746X);
            arg0K0 = option_742X;
            arg0K1 = seconds_745X;
            arg0K2 = mseconds_746X;
            goto L44714;}
          else {
            if ((1 == option_742X)) {
              seconds_747X = s48_real_time(&mseconds_748X);
              arg0K0 = option_742X;
              arg0K1 = seconds_747X;
              arg0K2 = mseconds_748X;
              goto L44714;}
            else {
              push_exception_continuationB(17, 1);
              *((long *) (SstackS)) = (((option_742X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = other_743X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21289;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_741X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
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
          goto L23193;}
        else {
          if ((1 == status_752X)) {
            push_exception_continuationB(22, 1);
            *((long *) (SstackS)) = (((key_750X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_751X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21289;}
          else {
            push_exception_continuationB(23, 1);
            *((long *) (SstackS)) = (((key_750X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_751X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21289;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_749X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
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
          goto L47100;}
        else {
          goto L47100;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L47100;}}
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
                  goto L37609;}
                else {
                  arg0K0 = 1;
                  goto L37609;}}
              else {
                arg0K0 = 1;
                goto L37609;}}
            else {
              goto L37576;}}
          else {
            goto L37576;}}
        else {
          goto L37576;}}
      else {
        goto L37576;}}
      break;
    case 157 : {
      s48_make_availableAgc((4 + (-4 & (4 + ((((SvalS))>>2))))));
      SstackS = ((SstackS) + 4);
      arg2_759X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        n_760X = (((SvalS))>>2);
        if ((3 == (3 & arg2_759X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_759X))) + -4))))>>2))))) {
            goto L37311;}
          else {
            goto L37263;}}
        else {
          goto L37263;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_759X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
      break;
    case 158 : {
      SstackS = ((SstackS) + 4);
      thing_761X = *((long *) (SstackS));
      list_762X = SvalS;
      arg0K0 = list_762X;
      arg0K1 = list_762X;
      arg2K2 = 1;
      goto L39078;}
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
                goto L29078;}
              else {
                if ((index_765X < len_766X)) {
                  v_767X = *((unsigned char *) ((Scode_pointerS) + 1));
                  if ((0 == v_767X)) {
                    goto L29068;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L29068;}
                    else {
                      merged_arg0K0 = arg3_764X;
                      merged_arg0K1 = (((index_765X)<<2));
                      proposal_d_read_return_tag = 2;
                      goto proposal_d_read;
                     proposal_d_read_return_2:
                      v_768X = proposal_d_read0_return_value;
                      arg0K0 = v_768X;
                      goto L29077;}}}
                else {
                  goto L29078;}}}
            else {
              goto L29098;}}
          else {
            goto L29098;}}
        else {
          goto L29098;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg3_764X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_763X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L21289;}}
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
                    goto L29336;}
                  else {
                    if ((index_772X < len_774X)) {
                      v_775X = *((unsigned char *) ((Scode_pointerS) + 1));
                      if ((0 == v_775X)) {
                        goto L29326;}
                      else {
                        if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                          goto L29326;}
                        else {
                          merged_arg0K0 = arg4_771X;
                          merged_arg0K1 = (((index_772X)<<2));
                          merged_arg0K2 = value_773X;
                          proposal_d_write_return_tag = 2;
                          goto proposal_d_write;
                         proposal_d_write_return_2:
                          goto L29335;}}}
                    else {
                      goto L29336;}}}
                else {
                  goto L29358;}}
              else {
                goto L29358;}}
            else {
              goto L29358;}}
          else {
            goto L29358;}}
        else {
          goto L29358;}}
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
        goto L21289;}}
      break;
    case 161 : {
      v_776X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_776X)) {
        arg0K0 = (SvalS);
        goto L29615;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_777X = get_current_port0_return_value;
        arg0K0 = v_777X;
        goto L29615;}}
      break;
    case 162 : {
      v_778X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_778X)) {
        arg0K0 = (SvalS);
        goto L29856;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_779X = get_current_port0_return_value;
        arg0K0 = v_779X;
        goto L29856;}}
      break;
    case 163 : {
      v_780X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_780X)) {
        v_781X = SvalS;
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = v_781X;
        goto L30090;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_782X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_782X;
        goto L30090;}}
      break;
    case 164 : {
      x_783X = SvalS;
      out_784X = stderr;
      arg0K0 = x_783X;
      goto L39331;}
      break;
  }}
 L21289: {
  nargs_785X = arg0K0;
  opcode_786X = (((*((long *) ((SstackS) + (8 + (((nargs_785X)<<2)))))))>>2);
  handlers_787X = SHARED_REF((Sexception_handlersS));
  if ((3 == (3 & handlers_787X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_787X))) + -4))))>>2))))) {
      goto L21352;}
    else {
      goto L21425;}}
  else {
    goto L21425;}}
 L30465: {
  env_788X = arg0K0;
  i_789X = arg0K1;
  if ((0 == i_789X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_788X))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 3);
      arg0K0 = 0;
      goto L21289;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23193;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_788X)))));
    arg0K1 = (-1 + i_789X);
    goto L30465;}}
 L25160: {
  env_790X = arg0K0;
  i_791X = arg0K1;
  if ((0 == i_791X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_790X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 5);
      arg0K0 = 0;
      goto L21289;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23193;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_790X)))));
    arg0K1 = (-1 + i_791X);
    goto L25160;}}
 L25098: {
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
    goto L23193;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_792X)))));
    arg0K1 = (-1 + i_793X);
    goto L25098;}}
 L25835: {
  v_797X = arg0K0;
  s48_make_availableAgc((12 + (((v_797X)<<2))));
  p_798X = SenvS;
  if ((3 == (3 & p_798X))) {
    if ((p_798X < (((long) (Sstack_beginS))))) {
      goto L25889;}
    else {
      if (((((long) (Sstack_endS))) < p_798X)) {
        goto L25889;}
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
        goto L25889;}}}
  else {
    goto L25889;}}
 L25848: {
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
  goto L23193;}
 L24885: {
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
    goto L23193;}
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
    goto L24885;}}
 L26172: {
  start_i_811X = arg0K0;
  arg0K0 = start_i_811X;
  arg0K1 = 2;
  arg0K2 = (SenvS);
  goto L26180;}
 L26023: {
  start_i_812X = arg0K0;
  arg0K0 = start_i_812X;
  arg0K1 = 3;
  arg0K2 = (SenvS);
  goto L26031;}
 L46412: {
  value_813X = arg0K0;
  SvalS = value_813X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L23850: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 0;
      goto check_events;
     check_events_return_0:
      v_814X = check_events0_return_value;
      if (v_814X) {
        arg0K0 = stack_arg_count_424X;
        goto L21522;}
      else {
        goto L23854;}}
    else {
      arg0K0 = stack_arg_count_424X;
      goto L21522;}}
  else {
    goto L23854;}}
 L23860: {
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L7700: {
  loc_815X = arg1K0;
  arg_816X = arg1K1;
  if ((top_of_args_430X < arg_816X)) {
    *((long *) loc_815X) = (*((long *) arg_816X));
    arg1K0 = (loc_815X + -4);
    arg1K1 = (arg_816X + -4);
    goto L7700;}
  else {
    SstackS = loc_815X;
    s48_call_native_code(proc_429X);
    goto L23860;}}
 L20919: {
  stack_arg_count_817X = arg0K0;
  list_args_818X = arg0K1;
  list_arg_count_819X = arg0K2;
  code_820X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  total_arg_count_821X = stack_arg_count_817X + list_arg_count_819X;
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_820X))) + 1)));
  arg0K1 = 64;
  goto L20934;}
 L21791: {
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
    goto L21289;}}
 L22419: {
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
        goto L20919;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_830X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L21791;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_830X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L21791;}}
  else {
    obj_834X = SvalS;
    if ((3 == (3 & obj_834X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_834X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_830X;
        arg0K1 = list_args_831X;
        arg0K2 = list_arg_count_832X;
        goto L20919;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_830X;
        arg0K2 = list_args_831X;
        arg0K3 = list_arg_count_832X;
        goto L21791;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_830X;
      arg0K2 = list_args_831X;
      arg0K3 = list_arg_count_832X;
      goto L21791;}}}
 L14402: {
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
    goto L22419;}
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
    goto L21289;}}
 L14443: {
  list_841X = arg0K0;
  follower_842X = arg0K1;
  if ((25 == list_841X)) {
    list_args_843X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_842X))) + 4)))))));
    addr_844X = (((char *) (-3 + follower_842X))) + 4;
    S48_WRITE_BARRIER(follower_842X, addr_844X, list_args_843X);
    *((long *) addr_844X) = list_args_843X;
    arg0K0 = rest_list_440X;
    arg0K1 = (-1 + stack_nargs_438X);
    goto L14402;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_841X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_842X))) + 4)));
    goto L14443;}}
 L22364: {
  obj_845X = SvalS;
  if ((3 == (3 & obj_845X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_845X))) + -4))))>>2))))) {
      arg0K0 = 0;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L20919;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L21791;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L21791;}}
 L22371: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_441X;
  ScontS = (Sbottom_of_stackS);
  goto L22364;}
 L24460: {
  cont_846X = ScontS;
  code_847X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_846X))) + 8)))))));
  pc_848X = (((*((long *) ((((char *) (-3 + cont_846X))) + 4))))>>2);
  protocol_849X = *((unsigned char *) ((((char *) (-3 + code_847X))) + (1 + pc_848X)));
  if ((1 == protocol_849X)) {
    goto L24489;}
  else {
    if ((66 == protocol_849X)) {
      goto L24489;}
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
            goto L24460;}
          else {
            arg0K0 = cont_850X;
            goto L21855;}}
        else {
          arg0K0 = cont_850X;
          goto L21855;}}
      else {
        if ((70 == protocol_849X)) {
          next_851X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_851X)))));
            goto L24507;}
          else {
            ScontS = next_851X;
            goto L24507;}}
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
              goto L23193;}
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
                goto L23193;}
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
                goto L21289;}}}
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
            goto L21289;}}}}}}
 L45876: {
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
        goto L45971;}
      else {
        arg0K0 = (*((long *) (((char *) (-3 + list_args_860X)))));
        goto L45971;}}
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
      goto L21289;}}
  else {
    if ((66 == protocol_865X)) {
      pop_continuationB_return_tag = 2;
      goto pop_continuationB;
     pop_continuationB_return_2:
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23193;}
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
            goto L45876;}
          else {
            goto L45919;}}
        else {
          goto L45919;}}
      else {
        if ((70 == protocol_865X)) {
          next_868X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_868X)))));
            goto L45927;}
          else {
            ScontS = next_868X;
            goto L45927;}}
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
                goto L21289;}
              else {
                arg_top_872X = SstackS;
                pop_continuationB_return_tag = 3;
                goto pop_continuationB;
               pop_continuationB_return_3:
                arg1K0 = (SstackS);
                arg1K1 = (arg_top_872X + (((stack_nargs_859X)<<2)));
                goto L25445;}}
            else {
              if ((64 == protocol_865X)) {
                index_873X = 2 + pc_864X;
                arg0K0 = (((((*((unsigned char *) ((((char *) (-3 + code_863X))) + index_873X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_863X))) + (1 + index_873X)))));
                arg0K1 = 3;
                arg0K2 = stack_nargs_859X;
                arg0K3 = list_args_860X;
                arg0K4 = list_arg_count_861X;
                goto L25297;}
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
                goto L21289;}}}
          else {
            arg0K0 = protocol_865X;
            arg0K1 = 1;
            arg0K2 = stack_nargs_859X;
            arg0K3 = list_args_860X;
            arg0K4 = list_arg_count_861X;
            goto L25297;}}}}}}
 L23590: {
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
        goto L21522;}
      else {
        goto L23600;}}
    else {
      arg0K0 = 0;
      goto L21522;}}
  else {
    goto L23600;}}
 L23633: {
  push_exception_continuationB(5, 4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L23491: {
  arg0K0 = (2 + (((max_450X)<<1)));
  goto L23493;}
 L23493: {
  offset_877X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_877X);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L24376: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L23389: {
  delta_878X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_878X);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L24267: {
  delta_879X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_879X);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L46465: {
  val_880X = arg0K0;
  SvalS = val_880X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L46477: {
  val_881X = arg0K0;
  SvalS = val_881X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L13087: {
  if ((3 == (3 & x_464X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L46477;}
    else {
      goto L13093;}}
  else {
    goto L13093;}}
 L38158: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L38159: {
  if ((3 == (3 & n_465X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_465X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_465X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
    else {
      goto L38163;}}
  else {
    goto L38163;}}
 L38315: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L38290: {
  if ((3 == (3 & n_466X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      goto L38315;}
    else {
      goto L38304;}}
  else {
    goto L38304;}}
 L38514: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L38489: {
  if ((3 == (3 & n_467X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      goto L38514;}
    else {
      goto L38503;}}
  else {
    goto L38503;}}
 L38713: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L38688: {
  if ((3 == (3 & n_468X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      goto L38713;}
    else {
      goto L38702;}}
  else {
    goto L38702;}}
 L31961: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L31962: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_469X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L32087: {
  v_882X = (char *) s48_long_to_bignum(x_474X);
  v_883X = enter_bignum(v_882X);
  arg0K0 = v_883X;
  goto L32033;}
 L32033: {
  val_884X = arg0K0;
  SvalS = val_884X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L32039: {
  if ((0 == (3 & y_473X))) {
    goto L32045;}
  else {
    if ((3 == (3 & y_473X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_473X))) + -4))))>>2))))) {
        goto L32045;}
      else {
        goto L32048;}}
    else {
      goto L32048;}}}
 L32048: {
  if ((3 == (3 & x_472X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_472X))) + -4))))>>2))))) {
      if ((3 == (3 & y_473X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_473X))) + -4))))>>2))))) {
          Stemp0S = x_472X;
          Stemp1S = y_473X;
          s48_make_availableAgc(12);
          value_885X = Stemp1S;
          Stemp1S = 1;
          y_886X = *((double *) (((char *) (-3 + value_885X))));
          value_887X = Stemp0S;
          Stemp0S = 1;
          x_888X = *((double *) (((char *) (-3 + value_887X))));
          addr_889X = s48_allocate_small(12);
          *((long *) addr_889X) = 2126;
          Kdouble_890X = 3 + (((long) (addr_889X + 4)));
          *((double *) (((char *) (-3 + Kdouble_890X)))) = (x_888X + y_886X);
          SvalS = Kdouble_890X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23193;}
        else {
          goto L32062;}}
      else {
        goto L32062;}}
    else {
      goto L32062;}}
  else {
    goto L32062;}}
 L7847: {
  a_891X = arg0K0;
  if ((b_478X < 0)) {
    arg0K0 = (0 - b_478X);
    goto L7851;}
  else {
    arg0K0 = b_478X;
    goto L7851;}}
 L40460: {
  if ((0 == (3 & y_476X))) {
    goto L40466;}
  else {
    if ((3 == (3 & y_476X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_476X))) + -4))))>>2))))) {
        goto L40466;}
      else {
        goto L40469;}}
    else {
      goto L40469;}}}
 L40469: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_475X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_476X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L32362: {
  v_892X = (char *) s48_long_to_bignum(x_481X);
  v_893X = enter_bignum(v_892X);
  arg0K0 = v_893X;
  goto L32322;}
 L32322: {
  val_894X = arg0K0;
  SvalS = val_894X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L32328: {
  if ((0 == (3 & y_480X))) {
    goto L32334;}
  else {
    if ((3 == (3 & y_480X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_480X))) + -4))))>>2))))) {
        goto L32334;}
      else {
        goto L32337;}}
    else {
      goto L32337;}}}
 L32337: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_479X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_480X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L8105: {
  a_895X = arg0K0;
  if ((b_485X < 0)) {
    arg0K0 = (0 - b_485X);
    goto L8109;}
  else {
    arg0K0 = b_485X;
    goto L8109;}}
 L40615: {
  if ((0 == (3 & y_483X))) {
    goto L40621;}
  else {
    if ((3 == (3 & y_483X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_483X))) + -4))))>>2))))) {
        goto L40621;}
      else {
        goto L40648;}}
    else {
      goto L40648;}}}
 L40648: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_482X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_483X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L32509: {
  val_896X = arg0K0;
  SvalS = val_896X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L32515: {
  if ((0 == (3 & y_487X))) {
    goto L32521;}
  else {
    if ((3 == (3 & y_487X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_487X))) + -4))))>>2))))) {
        goto L32521;}
      else {
        goto L32526;}}
    else {
      goto L32526;}}}
 L32526: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_486X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_487X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L32686: {
  val_897X = arg0K0;
  SvalS = val_897X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L32692: {
  if ((0 == (3 & y_489X))) {
    goto L32698;}
  else {
    if ((3 == (3 & y_489X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_489X))) + -4))))>>2))))) {
        goto L32698;}
      else {
        goto L32703;}}
    else {
      goto L32703;}}}
 L32703: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_488X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_489X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L32926: {
  val_898X = arg0K0;
  SvalS = val_898X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L32932: {
  if ((0 == (3 & y_491X))) {
    goto L32938;}
  else {
    if ((3 == (3 & y_491X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_491X))) + -4))))>>2))))) {
        goto L32938;}
      else {
        goto L32943;}}
    else {
      goto L32943;}}}
 L32943: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_490X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_491X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L33166: {
  val_899X = arg0K0;
  SvalS = val_899X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L33172: {
  if ((0 == (3 & y_493X))) {
    goto L33178;}
  else {
    if ((3 == (3 & y_493X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_493X))) + -4))))>>2))))) {
        goto L33178;}
      else {
        goto L33183;}}
    else {
      goto L33183;}}}
 L33183: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_492X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_493X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L33360: {
  val_900X = arg0K0;
  SvalS = val_900X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L33366: {
  if ((0 == (3 & y_495X))) {
    goto L33372;}
  else {
    if ((3 == (3 & y_495X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_495X))) + -4))))>>2))))) {
        goto L33372;}
      else {
        goto L33377;}}
    else {
      goto L33377;}}}
 L33377: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_494X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_495X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L18554: {
  x_901X = arg0K0;
  y_902X = arg0K1;
  if ((0 == (3 & x_901X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18584;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_901X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18584;}}
 L8750: {
  a_903X = arg0K0;
  if ((b_499X < 0)) {
    arg0K0 = (0 - b_499X);
    goto L8754;}
  else {
    arg0K0 = b_499X;
    goto L8754;}}
 L33566: {
  if ((0 == (3 & y_497X))) {
    goto L33572;}
  else {
    if ((3 == (3 & y_497X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_497X))) + -4))))>>2))))) {
        goto L33572;}
      else {
        goto L33575;}}
    else {
      goto L33575;}}}
 L33575: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_496X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_497X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L18463: {
  x_904X = arg0K0;
  y_905X = arg0K1;
  if ((0 == (3 & x_904X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18493;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_904X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18493;}}
 L33780: {
  a_906X = arg0K0;
  n_907X = ((y_501X)>>2);
  if ((n_907X < 0)) {
    arg0K0 = (0 - n_907X);
    goto L33782;}
  else {
    arg0K0 = n_907X;
    goto L33782;}}
 L33742: {
  if ((0 == (3 & y_501X))) {
    goto L33748;}
  else {
    if ((3 == (3 & y_501X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_501X))) + -4))))>>2))))) {
        goto L33748;}
      else {
        goto L33751;}}
    else {
      goto L33751;}}}
 L33751: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_500X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_501X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L33929: {
  SvalS = n_503X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L33930: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_503X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L33984: {
  SvalS = n_504X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L33985: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_504X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L34039: {
  SvalS = 4;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L34042: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_505X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L34099: {
  SvalS = n_506X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L34100: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_506X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L34154: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L34157: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_507X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L46631: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_516X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L46637: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L22040: {
  x_908X = SvalS;
  if ((0 == (3 & x_908X))) {
    s48_make_availableAgc(16);
    if ((x_908X < 0)) {
      arg0K0 = (0 - x_908X);
      goto L22068;}
    else {
      arg0K0 = x_908X;
      goto L22068;}}
  else {
    if ((0 == (3 & x_908X))) {
      arg0K0 = 1;
      arg0K1 = 3;
      goto L18071;}
    else {
      arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_908X))) + -4))))>>8))))>>2)));
      arg0K1 = 0;
      goto L18071;}}}
 L22043: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L34308: {
  if ((0 == (3 & x_525X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18418;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_525X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18418;}}
 L34311: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_525X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L34399: {
  x_909X = arg0K0;
  arg0K0 = x_909X;
  arg0K1 = 0;
  goto L34405;}
 L34382: {
  if ((0 == (3 & x_526X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18360;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_526X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18360;}}
 L34385: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_526X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L34484: {
  if ((0 == (3 & y_529X))) {
    goto L34490;}
  else {
    if ((3 == (3 & y_529X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_529X))) + -4))))>>2))))) {
        goto L34490;}
      else {
        goto L34493;}}
    else {
      goto L34493;}}}
 L34493: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_528X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_529X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L34647: {
  if ((0 == (3 & y_531X))) {
    goto L34653;}
  else {
    if ((3 == (3 & y_531X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_531X))) + -4))))>>2))))) {
        goto L34653;}
      else {
        goto L34656;}}
    else {
      goto L34656;}}}
 L34656: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_530X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_531X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L34810: {
  if ((0 == (3 & y_533X))) {
    goto L34816;}
  else {
    if ((3 == (3 & y_533X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_533X))) + -4))))>>2))))) {
        goto L34816;}
      else {
        goto L34819;}}
    else {
      goto L34819;}}}
 L34819: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_532X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_533X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L31741: {
  val_910X = arg0K0;
  SvalS = val_910X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L31757: {
  val_911X = arg0K0;
  SvalS = val_911X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L31758: {
  push_exception_continuationB(6, 1);
  *((long *) (SstackS)) = x_534X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_535X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L40848: {
  if ((0 == (3 & y_535X))) {
    if ((0 == (3 & x_534X))) {
      value_912X = ((x_534X)>>2);
      count_913X = ((y_535X)>>2);
      if ((count_913X < 0)) {
        PS_SHIFT_RIGHT(value_912X, (0 - count_913X), x_914X)
        x_915X = x_914X;
        s48_make_availableAgc(16);
        if ((536870911 < x_915X)) {
          goto L40968;}
        else {
          if ((x_915X < -536870912)) {
            goto L40968;}
          else {
            arg0K0 = (((x_915X)<<2));
            goto L40963;}}}
      else {
        PS_SHIFT_LEFT(value_912X, count_913X, x_916X)
        result_917X = x_916X;
        PS_SHIFT_RIGHT(result_917X, count_913X, x_918X)
        if ((value_912X == x_918X)) {
          if ((value_912X < 0)) {
            if ((result_917X < 0)) {
              s48_make_availableAgc(16);
              if ((536870911 < result_917X)) {
                goto L40990;}
              else {
                if ((result_917X < -536870912)) {
                  goto L40990;}
                else {
                  arg0K0 = (((result_917X)<<2));
                  goto L40985;}}}
            else {
              arg0K0 = x_534X;
              arg0K1 = y_535X;
              goto L40861;}}
          else {
            if ((result_917X < 0)) {
              arg0K0 = x_534X;
              arg0K1 = y_535X;
              goto L40861;}
            else {
              s48_make_availableAgc(16);
              if ((536870911 < result_917X)) {
                goto L41012;}
              else {
                if ((result_917X < -536870912)) {
                  goto L41012;}
                else {
                  arg0K0 = (((result_917X)<<2));
                  goto L41007;}}}}}
        else {
          arg0K0 = x_534X;
          arg0K1 = y_535X;
          goto L40861;}}}
    else {
      if ((3 == (3 & x_534X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_534X))) + -4))))>>2))))) {
          y_919X = ((y_535X)>>2);
          merged_arg0K0 = x_534X;
          merged_arg0K1 = y_919X;
          shift_space_return_tag = 0;
          goto shift_space;
         shift_space_return_0:
          needed_920X = shift_space0_return_value;
          Stemp0S = x_534X;
          s48_make_availableAgc((((needed_920X)<<2)));
          value_921X = Stemp0S;
          Stemp0S = 1;
          if ((0 == (3 & value_921X))) {
            v_922X = (char *) s48_long_to_bignum((((value_921X)>>2)));
            arg1K0 = v_922X;
            goto L41043;}
          else {
            arg1K0 = (((char *) (-3 + value_921X)));
            goto L41043;}}
        else {
          goto L40870;}}
      else {
        goto L40870;}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_534X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_535X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}}
 L46719: {
  val_923X = arg0K0;
  SvalS = val_923X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L37933: {
  val_924X = arg0K0;
  SvalS = val_924X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L37906: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_539X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L37843: {
  val_925X = arg0K0;
  SvalS = val_925X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L37816: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_541X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L42038: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((x_543X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L46748: {
  val_926X = arg0K0;
  SvalS = val_926X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L46762: {
  value_927X = arg0K0;
  SvalS = value_927X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L26360: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = stob_547X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_548X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L26465: {
  SvalS = new_553X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L26449: {
  i_928X = arg0K0;
  if ((i_928X < 0)) {
    goto L26465;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_553X))) + (((i_928X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_928X);
    goto L26449;}}
 L26581: {
  i_929X = arg0K0;
  if ((i_929X < 0)) {
    arg0K0 = stack_nargs_559X;
    arg0K1 = rest_list_560X;
    goto L26715;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_558X))) + (((i_929X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_929X);
    goto L26581;}}
 L26741: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_561X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_563X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_562X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L26858: {
  addr_930X = (((char *) (-3 + stob_564X))) + (((offset_566X)<<2));
  S48_WRITE_BARRIER(stob_564X, addr_930X, value_565X);
  *((long *) addr_930X) = value_565X;
  goto L26867;}
 L26867: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L26868: {
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
  goto L21289;}
 L27085: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = (((type_571X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_572X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = init_570X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L27102: {
  x_931X = arg0K0;
  value_932X = Stemp0S;
  Stemp0S = 1;
  if ((1 == x_931X)) {
    push_exception_continuationB(8, 2);
    *((long *) (SstackS)) = (((type_571X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_572X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = value_932X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L21289;}
  else {
    arg0K0 = (-1 + len_572X);
    goto L27128;}}
 L27375: {
  push_exception_continuationB(7, 3);
  *((long *) (SstackS)) = stob_575X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_577X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_576X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L27365: {
  arg0K0 = (*((long *) ((((char *) (-3 + stob_575X))) + (-4 & index_576X))));
  goto L27374;}
 L27374: {
  value_933X = arg0K0;
  SvalS = value_933X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L27330: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_575X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_577X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_576X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L27662: {
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
  goto L21289;}
 L27652: {
  addr_934X = (((char *) (-3 + stob_583X))) + (-4 & arg2_582X);
  S48_WRITE_BARRIER(stob_583X, addr_934X, value_584X);
  *((long *) addr_934X) = value_584X;
  goto L27661;}
 L27661: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L27615: {
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
  goto L21289;}
 L35031: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_590X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_591X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L35044: {
  vector_935X = arg0K0;
  if ((1 == vector_935X)) {
    push_exception_continuationB(8, 1);
    *((long *) (SstackS)) = (((len_590X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((init_591X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}
  else {
    arg0K0 = (-1 + len_590X);
    goto L35066;}}
 L45543: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L42144: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_594X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_595X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L45059: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_594X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L39682: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_598X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_599X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_600X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L39661: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_598X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_599X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_600X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L43839: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_598X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_597X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L35211: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_603X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_604X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L35224: {
  vector_936X = arg0K0;
  if ((1 == vector_936X)) {
    push_exception_continuationB(8, 1);
    *((long *) (SstackS)) = (((len_603X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (9 + ((((init_604X))<<8)));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_936X))) + len_603X)) = 0;
    arg0K0 = (-1 + len_603X);
    goto L35246;}}
 L42225: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_602X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L45593: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L42288: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_608X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_609X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L45155: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_608X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L39837: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_612X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_613X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_614X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L39816: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_612X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_613X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_614X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L43965: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_612X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_611X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L19844: {
  i_937X = arg0K0;
  h_938X = arg0K1;
  if ((i_937X < n_619X)) {
    arg0K0 = (1 + i_937X);
    arg0K1 = (h_938X + (((*((unsigned char *) ((((char *) (-3 + string_618X))) + i_937X))))));
    goto L19844;}
  else {
    index_939X = 1023 & h_938X;
    link_940X = *((long *) ((((char *) (-3 + table_617X))) + (((index_939X)<<2))));
    if ((0 == (3 & link_940X))) {
      arg0K0 = (3 + (-4 & link_940X));
      goto L19800;}
    else {
      arg0K0 = link_940X;
      goto L19800;}}}
 L31903: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L43529: {
  val_941X = arg0K0;
  SvalS = val_941X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L43510: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L39424: {
  if ((1 == (SvalS))) {
    addr_942X = (((char *) (-3 + arg2_623X))) + 4;
    S48_WRITE_BARRIER(arg2_623X, addr_942X, 273);
    *((long *) addr_942X) = 273;
    goto L39430;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_623X))) + 4)))))) {
      addr_943X = (((char *) (-3 + arg2_623X))) + 4;
      S48_WRITE_BARRIER(arg2_623X, addr_943X, 529);
      *((long *) addr_943X) = 529;
      goto L39430;}
    else {
      goto L39430;}}}
 L39431: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_623X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L46834: {
  val_944X = arg0K0;
  SvalS = val_944X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L42370: {
  SvalS = x_626X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L35618: {
  if ((0 == (3 & arg2_627X))) {
    if (((((arg2_627X)>>2)) < 0)) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21289;}
    else {
      arg0K0 = (((arg2_627X)>>2));
      goto L35438;}}
  else {
    if ((3 == (3 & arg2_627X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_627X))) + -4))))>>2))))) {
        if ((1 == mode_628X)) {
          goto L35496;}
        else {
          if ((3 == mode_628X)) {
            goto L35496;}
          else {
            v_945X = ps_open_fd((((char *)(((char *) (-3 + arg2_627X))))), 0, &v_946X);
            arg0K0 = v_945X;
            arg0K1 = v_946X;
            goto L35509;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_627X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_628X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21289;}}}
 L46868: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L42495: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L42505;}
  else {
    arg2K0 = 1;
    goto L42505;}}
 L42506: {
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
  goto L21289;}
 L36257: {
  if ((3 == (3 & arg3_639X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>8)));
      goto L36259;}
    else {
      goto L36446;}}
  else {
    goto L36446;}}
 L36432: {
  if ((3 == (3 & arg3_639X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>2))))) {
      goto L36257;}
    else {
      goto L36311;}}
  else {
    goto L36311;}}
 L36311: {
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
  goto L21289;}
 L42701: {
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
  goto L21289;}
 L39015: {
  val_947X = arg0K0;
  SvalS = val_947X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L46890: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L42843: {
  val_948X = arg0K0;
  SvalS = val_948X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L10628: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_650X))) + 16)));
  goto L42843;}
 L10634: {
  ch_949X = arg0K0;
  prev_950X = arg0K1;
  if ((1 == ch_949X)) {
    addr_951X = (((char *) (-3 + channel_650X))) + 16;
    S48_WRITE_BARRIER(channel_650X, addr_951X, 1);
    *((long *) addr_951X) = 1;
    n_952X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_650X))) + 8))))>>2)));
    arg0K0 = (((n_952X)<<2));
    goto L42843;}
  else {
    if ((ch_949X == channel_650X)) {
      y_953X = Spending_channels_tailS;
      if ((ch_949X == y_953X)) {
        Spending_channels_tailS = prev_950X;
        goto L10658;}
      else {
        goto L10658;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_949X))) + 12)));
      arg0K1 = ch_949X;
      goto L10634;}}}
 L42826: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L15332: {
  i_954X = arg0K0;
  res_955X = arg0K1;
  if ((-1 == i_954X)) {
    SvalS = res_955X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    channel_956X = *((Svm_channelsS) + i_954X);
    if ((3 == (3 & channel_956X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_956X))) + -4))))>>2))))) {
        addr_957X = s48_allocate_small(12);
        *((long *) addr_957X) = 2050;
        x_958X = 3 + (((long) (addr_957X + 4)));
        *((long *) (((char *) (-3 + x_958X)))) = channel_956X;
        *((long *) ((((char *) (-3 + x_958X))) + 4)) = res_955X;
        arg0K0 = x_958X;
        goto L15346;}
      else {
        arg0K0 = res_955X;
        goto L15346;}}
    else {
      arg0K0 = res_955X;
      goto L15346;}}}
 L36565: {
  old_959X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 == old_959X)) {
    goto L36581;}
  else {
    addr_960X = ((char *) (-3 + old_959X));
    S48_WRITE_BARRIER(old_959X, addr_960X, 1);
    *((long *) addr_960X) = 1;
    goto L36581;}}
 L36596: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proposal_657X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L9037: {
  i_961X = arg0K0;
  stob_962X = *((long *) ((((char *) (-3 + log_659X))) + (((i_961X)<<2))));
  if ((1 == stob_962X)) {
    log_963X = *((long *) ((((char *) (-3 + proposal_658X))) + 8));
    arg0K0 = 0;
    goto L9275;}
  else {
    verify_964X = *((long *) ((((char *) (-3 + log_659X))) + (12 + (((i_961X)<<2)))));
    value_965X = *((long *) ((((char *) (-3 + log_659X))) + (8 + (((i_961X)<<2)))));
    if ((29 == verify_964X)) {
      if ((3 == (3 & stob_962X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_962X))) + -4)))))) {
          goto L9089;}
        else {
          goto L36827;}}
      else {
        goto L36827;}}
    else {
      if ((verify_964X == (*((long *) ((((char *) (-3 + stob_962X))) + (-4 & (*((long *) ((((char *) (-3 + log_659X))) + (4 + (((i_961X)<<2)))))))))))) {
        if ((verify_964X == value_965X)) {
          goto L9089;}
        else {
          if ((3 == (3 & stob_962X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_962X))) + -4)))))) {
              goto L9089;}
            else {
              goto L36827;}}
          else {
            goto L36827;}}}
      else {
        goto L36827;}}}}
 L27960: {
  value_966X = arg0K0;
  SvalS = value_966X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L27961: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_660X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_662X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_661X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L28054: {
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
  goto L21289;}
 L28114: {
  memcpy((void *)((((char *) (-3 + arg3_666X))) + to_index_670X), (void *)((((char *) (-3 + arg5_668X))) + from_index_669X),count_671X);
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L28202: {
  left_967X = arg0K0;
  copies_968X = arg0K1;
  if ((1 == copies_968X)) {
    if ((left_967X < count_671X)) {
      goto L28054;}
    else {
      from_index_969X = ((from_index_669X)<<2);
      to_index_970X = ((to_index_670X)<<2);
      count_971X = ((count_671X)<<2);
      Stemp0S = arg5_668X;
      Stemp1S = arg3_666X;
      addr_972X = s48_allocate_tracedAgc(28);
      if ((addr_972X == NULL)) {
        arg0K0 = 1;
        goto L9845;}
      else {
        *((long *) addr_972X) = 6154;
        arg0K0 = (3 + (((long) (addr_972X + 4))));
        goto L9845;}}}
  else {
    arg0K0 = (left_967X - ((((*((long *) ((((char *) (-3 + copies_968X))) + 16))))>>2)));
    arg0K1 = (*((long *) ((((char *) (-3 + copies_968X))) + 20)));
    goto L28202;}}
 L28411: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_675X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_676X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L28410: {
  value_973X = arg0K0;
  SvalS = value_973X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L13897: {
  i_974X = arg0K0;
  next_stob_975X = *((long *) ((((char *) (-3 + log_680X))) + (((i_974X)<<2))));
  if ((1 == next_stob_975X)) {
    v_976X = add_log_entryAgc(2, i_974X, arg2_675X, index_679X, ((((*((unsigned char *) ((((char *) (-3 + arg2_675X))) + (((index_679X)>>2))))))<<2)), 1);
    arg0K0 = v_976X;
    goto L28410;}
  else {
    if ((arg2_675X == next_stob_975X)) {
      if ((index_679X == (*((long *) ((((char *) (-3 + log_680X))) + (4 + (((i_974X)<<2)))))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + log_680X))) + (8 + (((i_974X)<<2))))));
        goto L28410;}
      else {
        goto L13919;}}
    else {
      goto L13919;}}}
 L31209: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_675X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L28537: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_682X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_683X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_684X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L28536: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L14073: {
  i_977X = arg0K0;
  next_stob_978X = *((long *) ((((char *) (-3 + log_688X))) + (((i_977X)<<2))));
  if ((1 == next_stob_978X)) {
    add_log_entryAgc(2, i_977X, arg3_682X, index_687X, byte_684X, 0);
    goto L28536;}
  else {
    if ((arg3_682X == next_stob_978X)) {
      if ((index_687X == (*((long *) ((((char *) (-3 + log_688X))) + (4 + (((i_977X)<<2)))))))) {
        addr_979X = (((char *) (-3 + log_688X))) + (8 + (((i_977X)<<2)));
        S48_WRITE_BARRIER(log_688X, addr_979X, byte_684X);
        *((long *) addr_979X) = byte_684X;
        goto L28536;}
      else {
        goto L14093;}}
    else {
      goto L14093;}}}
 L28502: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_682X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_683X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_684X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L31312: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_682X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_681X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L40065: {
  reason_980X = arg0K0;
  status_981X = arg0K1;
  push_exception_continuationB(reason_980X, 1);
  *((long *) (SstackS)) = arg4_692X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_691X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_690X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_981X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 2;
  goto get_error_string;
 get_error_string_return_2:
  x_982X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_982X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21289;}
 L44162: {
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
  goto L21289;}
 L44803: {
  i_983X = arg0K0;
  h_984X = arg0K1;
  if ((i_983X < n_704X)) {
    arg0K0 = (1 + i_983X);
    arg0K1 = (h_984X + (((*((unsigned char *) ((((char *) (-3 + x_703X))) + i_983X))))));
    goto L44803;}
  else {
    SvalS = (((h_984X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}}
 L44780: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L40340: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = stob_705X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_706X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L42962: {
  firstP_985X = arg2K0;
  vector_986X = s48_find_all(type_718X);
  if ((1 == vector_986X)) {
    if (firstP_985X) {
      s48_collect();
      arg2K0 = 0;
      goto L42962;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = (((type_718X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}}
  else {
    SvalS = vector_986X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}}
 L46992: {
  firstP_987X = arg2K0;
  type_988X = arg0K1;
  vector_989X = s48_find_all_records(type_988X);
  if ((1 == vector_989X)) {
    if (firstP_987X) {
      Stemp0S = type_988X;
      s48_collect();
      value_990X = Stemp0S;
      Stemp0S = 1;
      arg2K0 = 0;
      arg0K1 = value_990X;
      goto L46992;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = type_988X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}}
  else {
    SvalS = vector_989X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}}
 L25655: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L24119: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L47040: {
  SvalS = (((old_728X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L23207: {
  SstackS = ((SstackS) + 4);
  proposal_991X = *((long *) (SstackS));
  x_992X = Scurrent_threadS;
  addr_993X = (((char *) (-3 + x_992X))) + 12;
  S48_WRITE_BARRIER(x_992X, addr_993X, proposal_991X);
  *((long *) addr_993X) = proposal_991X;
  SstackS = ((SstackS) + 4);
  pc_994X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tem_995X = *((long *) (SstackS));
  StemplateS = tem_995X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_995X)))))))) + (((pc_994X)>>2)));
  SstackS = ((SstackS) + 4);
  SvalS = (*((long *) (SstackS)));
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L45728: {
  x_996X = s48_schedule_alarm_interrupt((((p_731X)>>2)));
  SvalS = (((x_996X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L43712: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L43716;}
  else {
    arg2K0 = 1;
    goto L43716;}}
 L43717: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_732X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L28736: {
  rest_list_997X = arg0K0;
  if ((25 == rest_list_997X)) {
    proc_998X = *((long *) ((SstackS) + (((nargs_734X)<<2))));
    name_999X = *((long *) ((SstackS) + (-4 + (((nargs_734X)<<2)))));
    args_1000X = (SstackS) + 4;
    *((long *) (SstackS)) = (10 + (((nargs_734X)<<10)));
    SstackS = ((SstackS) + -4);
    if ((3 == (3 & name_999X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_999X))) + -4))))>>2))))) {
        if ((3 == (3 & proc_998X))) {
          if ((18 == (31 & ((((*((long *) ((((char *) (-3 + proc_998X))) + -4))))>>2))))) {
            if ((4 == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proc_998X))) + -4))))>>8)))) {
              result_1001X = s48_external_call(proc_998X, name_999X, (-2 + nargs_734X), args_1000X);
              if ((Sexternal_exceptionPS)) {
                Sexternal_exceptionPS = 0;
                arg0K0 = (Sexternal_exception_nargsS);
                goto L21289;}
              else {
                SvalS = result_1001X;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23193;}}
            else {
              goto L29002;}}
          else {
            goto L29002;}}
        else {
          goto L29002;}}
      else {
        goto L29002;}}
    else {
      goto L29002;}}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + rest_list_997X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_997X))) + 4)));
    goto L28736;}}
 L43057: {
  if ((1 == (SvalS))) {
    v_1002X = Hlookup22920((Sexported_bindingsS), arg2_737X, 0);
    arg0K0 = v_1002X;
    goto L43096;}
  else {
    v_1003X = Hlookup22901((Simported_bindingsS), arg2_737X, 0);
    arg0K0 = v_1003X;
    goto L43096;}}
 L43062: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_737X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L30547: {
  if ((1 == (SvalS))) {
    arg0K0 = (Sexported_bindingsS);
    goto L30580;}
  else {
    arg0K0 = (Simported_bindingsS);
    goto L30580;}}
 L30552: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_739X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L44714: {
  option_1004X = arg0K0;
  seconds_1005X = arg0K1;
  mseconds_1006X = arg0K2;
  if ((536869 < seconds_1005X)) {
    push_exception_continuationB(6, 1);
    *((long *) (SstackS)) = (((option_1004X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_1005X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_1006X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L21289;}
  else {
    SvalS = (((((1000 * seconds_1005X) + mseconds_1006X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}}
 L47100: {
  s48_Scallback_return_stack_blockS = arg2_753X;
  return x_754X;}
 L37609: {
  val_1007X = arg0K0;
  SvalS = val_1007X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L37576: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_755X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L37311: {
  len_1008X = 1 + n_760X;
  addr_1009X = s48_allocate_small((4 + len_1008X));
  *((long *) addr_1009X) = (70 + (((len_1008X)<<8)));
  string_1010X = 3 + (((long) (addr_1009X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1010X))) + n_760X)) = 0;
  arg0K0 = arg2_759X;
  arg0K1 = (-1 + n_760X);
  goto L37288;}
 L37263: {
  if ((25 == arg2_759X)) {
    goto L37311;}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_759X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_760X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}}
 L39078: {
  list_1011X = arg0K0;
  slow_1012X = arg0K1;
  move_slowP_1013X = arg2K2;
  if ((25 == list_1011X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    if ((3 == (3 & list_1011X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_1011X))) + -4))))>>2))))) {
        head_1014X = *((long *) (((char *) (-3 + list_1011X))));
        if ((3 == (3 & head_1014X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_1014X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_1014X))))) == thing_761X)) {
              SvalS = head_1014X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23193;}
            else {
              list_1015X = *((long *) ((((char *) (-3 + list_1011X))) + 4));
              if ((list_1015X == slow_1012X)) {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = thing_761X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = list_762X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21289;}
              else {
                if (move_slowP_1013X) {
                  arg0K0 = list_1015X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_1012X))) + 4)));
                  arg2K2 = 0;
                  goto L39078;}
                else {
                  arg0K0 = list_1015X;
                  arg0K1 = slow_1012X;
                  arg2K2 = 1;
                  goto L39078;}}}}
          else {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = thing_761X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = list_762X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21289;}}
        else {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = thing_761X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = list_762X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L21289;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = thing_761X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_762X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = thing_761X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = list_762X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21289;}}}
 L29078: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = arg3_764X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_763X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_765X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L29068: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg3_764X))) + (((index_765X)<<2)))));
  goto L29077;}
 L29077: {
  value_1016X = arg0K0;
  SvalS = value_1016X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L29098: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg3_764X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_763X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_765X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21289;}
 L29336: {
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
  goto L21289;}
 L29326: {
  addr_1017X = (((char *) (-3 + arg4_771X))) + (((index_772X)<<2));
  S48_WRITE_BARRIER(arg4_771X, addr_1017X, value_773X);
  *((long *) addr_1017X) = value_773X;
  goto L29335;}
 L29335: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L29358: {
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
  goto L21289;}
 L29615: {
  port_1018X = arg0K0;
  if ((3 == (3 & port_1018X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1018X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1018X))) + 4))))>>2))))) {
        goto L29685;}
      else {
        p_1019X = *((long *) ((((char *) (-3 + port_1018X))) + 24));
        p_1020X = *((long *) ((((char *) (-3 + port_1018X))) + 28));
        b_1021X = *((long *) ((((char *) (-3 + port_1018X))) + 20));
        i_1022X = ((p_1019X)>>2);
        x_1023X = *((long *) ((((char *) (-3 + port_1018X))) + 12));
        if ((5 == x_1023X)) {
          goto L29665;}
        else {
          if ((1 == b_1021X)) {
            goto L29665;}
          else {
            if ((i_1022X == (((p_1020X)>>2)))) {
              goto L29665;}
            else {
              val_1024X = 4 + (((i_1022X)<<2));
              addr_1025X = (((char *) (-3 + port_1018X))) + 24;
              S48_WRITE_BARRIER(port_1018X, addr_1025X, val_1024X);
              *((long *) addr_1025X) = val_1024X;
              SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1021X))) + i_1022X))))))<<8)));
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23193;}}}}}
    else {
      goto L29685;}}
  else {
    goto L29685;}}
 L29856: {
  port_1026X = arg0K0;
  if ((3 == (3 & port_1026X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1026X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1026X))) + 4))))>>2))))) {
        goto L29926;}
      else {
        p_1027X = *((long *) ((((char *) (-3 + port_1026X))) + 24));
        p_1028X = *((long *) ((((char *) (-3 + port_1026X))) + 28));
        b_1029X = *((long *) ((((char *) (-3 + port_1026X))) + 20));
        i_1030X = ((p_1027X)>>2);
        x_1031X = *((long *) ((((char *) (-3 + port_1026X))) + 12));
        if ((5 == x_1031X)) {
          goto L29906;}
        else {
          if ((1 == b_1029X)) {
            goto L29906;}
          else {
            if ((i_1030X == (((p_1028X)>>2)))) {
              goto L29906;}
            else {
              SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1029X))) + i_1030X))))))<<8)));
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23193;}}}}}
    else {
      goto L29926;}}
  else {
    goto L29926;}}
 L30090: {
  Kchar_1032X = arg0K0;
  port_1033X = arg0K1;
  if ((9 == (255 & Kchar_1032X))) {
    if ((3 == (3 & port_1033X))) {
      if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1033X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_1033X))) + 4))))>>2))))) {
          goto L30161;}
        else {
          p_1034X = *((long *) ((((char *) (-3 + port_1033X))) + 24));
          b_1035X = *((long *) ((((char *) (-3 + port_1033X))) + 20));
          i_1036X = ((p_1034X)>>2);
          x_1037X = *((long *) ((((char *) (-3 + port_1033X))) + 12));
          if ((5 == x_1037X)) {
            goto L30143;}
          else {
            if ((1 == b_1035X)) {
              goto L30143;}
            else {
              if ((i_1036X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_1035X))) + -4))))>>8)))) {
                goto L30143;}
              else {
                val_1038X = 4 + (((i_1036X)<<2));
                addr_1039X = (((char *) (-3 + port_1033X))) + 24;
                S48_WRITE_BARRIER(port_1033X, addr_1039X, val_1038X);
                *((long *) addr_1039X) = val_1038X;
                *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_1033X))) + 20)))))) + i_1036X)) = (((((Kchar_1032X)>>8))));
                SvalS = 13;
                Scode_pointerS = ((Scode_pointerS) + 2);
                arg1K0 = (Scode_pointerS);
                goto L23193;}}}}}
      else {
        goto L30161;}}
    else {
      goto L30161;}}
  else {
    goto L30161;}}
 L39331: {
  stuff_1040X = arg0K0;
  if ((3 == (3 & stuff_1040X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_1040X))) + -4))))>>2))))) {
      thing_1041X = *((long *) (((char *) (-3 + stuff_1040X))));
      if ((0 == (3 & thing_1041X))) {
        ps_write_integer((((thing_1041X)>>2)), out_784X);
        goto L39337;}
      else {
        if ((9 == (255 & thing_1041X))) {
          ps_write_string("#\\", out_784X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_1041X)>>8))), out_784X, ignoreXX) }
          goto L39337;}
        else {
          if ((3 == (3 & thing_1041X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thing_1041X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_1041X))) + -4))))>>8))))>>2)))) {
                type_1042X = *((long *) (((char *) (-3 + thing_1041X))));
                if ((3 == (3 & type_1042X))) {
                  if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_1042X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_1042X))) + -4))))>>8))))>>2)))) {
                      obj_1043X = *((long *) ((((char *) (-3 + type_1042X))) + 8));
                      if ((3 == (3 & obj_1043X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_1043X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_784X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_1041X)))))))) + 8))))))))))))), out_784X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_784X, ignoreXX) }
                          goto L39337;}
                        else {
                          goto L14782;}}
                      else {
                        goto L14782;}}
                    else {
                      goto L14782;}}
                  else {
                    goto L14782;}}
                else {
                  goto L14782;}}
              else {
                goto L14782;}}
            else {
              goto L14782;}}
          else {
            goto L14782;}}}}
    else {
      goto L39322;}}
  else {
    goto L39322;}}
 L21352: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_787X))) + (((opcode_786X)<<2)))));
  obj_1044X = SvalS;
  if ((3 == (3 & obj_1044X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1044X))) + -4))))>>2))))) {
      goto L21369;}
    else {
      goto L21439;}}
  else {
    goto L21439;}}
 L21425: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L21352;}
 L25889: {
  arg0K0 = (SenvS);
  arg0K1 = 0;
  goto L25848;}
 L26180: {
  i_1045X = arg0K0;
  offset_1046X = arg0K1;
  env_1047X = arg0K2;
  if ((i_1045X == total_count_409X)) {
    SvalS = new_env_412X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1046X));
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    back_1048X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1046X)));
    arg0K0 = env_1047X;
    arg0K1 = back_1048X;
    goto L26258;}}
 L26031: {
  i_1049X = arg0K0;
  offset_1050X = arg0K1;
  env_1051X = arg0K2;
  if ((i_1049X == total_count_416X)) {
    SvalS = new_env_419X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1050X));
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    back_1052X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1050X)));
    arg0K0 = env_1051X;
    arg0K1 = back_1052X;
    goto L26109;}}
 L21522: {
  stack_arg_count_1053X = arg0K0;
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
  x_1054X = Scurrent_threadS;
  addr_1055X = (((char *) (-3 + x_1054X))) + 12;
  S48_WRITE_BARRIER(x_1054X, addr_1055X, 1);
  *((long *) addr_1055X) = 1;
  tem_1056X = Sinterrupt_templateS;
  StemplateS = tem_1056X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1056X))))))));
  push_continuationB((Scode_pointerS), (5 + stack_arg_count_1053X));
  n_1057X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L21656;}
 L23854: {
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L20934: {
  protocol_1058X = arg0K0;
  stack_space_1059X = arg0K1;
  if ((69 == protocol_1058X)) {
    if ((total_arg_count_821X < 3)) {
      skip_1060X = *((unsigned char *) ((((char *) (-3 + code_820X))) + (2 + total_arg_count_821X)));
      if ((0 == skip_1060X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_817X;
        arg0K2 = list_args_818X;
        arg0K3 = list_arg_count_819X;
        goto L21791;}
      else {
        merged_arg0K0 = list_args_818X;
        merged_arg0K1 = list_arg_count_819X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = skip_1060X;
        arg0K1 = total_arg_count_821X;
        goto L20938;}}
    else {
      skip_1061X = *((unsigned char *) ((((char *) (-3 + code_820X))) + 5));
      if ((0 == skip_1061X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_817X;
        arg0K2 = list_args_818X;
        arg0K3 = list_arg_count_819X;
        goto L21791;}
      else {
        arg0K0 = skip_1061X;
        goto L20971;}}}
  else {
    if ((63 < protocol_1058X)) {
      if ((65 == protocol_1058X)) {
        wants_stack_args_1062X = ((((*((unsigned char *) ((((char *) (-3 + code_820X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_820X))) + 3)));
        if ((total_arg_count_821X < wants_stack_args_1062X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_817X;
          arg0K2 = list_args_818X;
          arg0K3 = list_arg_count_819X;
          goto L21791;}
        else {
          merged_arg0K0 = wants_stack_args_1062X;
          merged_arg0K1 = stack_arg_count_817X;
          merged_arg0K2 = list_args_818X;
          merged_arg0K3 = list_arg_count_819X;
          rest_list_setupAgc_return_tag = 0;
          goto rest_list_setupAgc;
         rest_list_setupAgc_return_0:
          arg0K0 = 4;
          arg0K1 = (1 + wants_stack_args_1062X);
          goto L20938;}}
      else {
        if ((68 == protocol_1058X)) {
          if ((total_arg_count_821X < (*((unsigned char *) ((((char *) (-3 + code_820X))) + 2))))) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_817X;
            arg0K2 = list_args_818X;
            arg0K3 = list_arg_count_819X;
            goto L21791;}
          else {
            arg0K0 = 3;
            goto L20971;}}
        else {
          if ((64 == protocol_1058X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_820X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_820X))) + 3)))) == total_arg_count_821X)) {
              if ((0 == list_arg_count_819X)) {
                arg0K0 = 4;
                arg0K1 = total_arg_count_821X;
                goto L20938;}
              else {
                merged_arg0K0 = list_args_818X;
                merged_arg0K1 = list_arg_count_819X;
                push_list_return_tag = 1;
                goto push_list;
               push_list_return_1:
                arg0K0 = 4;
                arg0K1 = total_arg_count_821X;
                goto L20938;}}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_817X;
              arg0K2 = list_args_818X;
              arg0K3 = list_arg_count_819X;
              goto L21791;}}
          else {
            if ((67 == protocol_1058X)) {
              length_1063X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_820X))) + -4))))>>8);
              index_1064X = -2 + length_1063X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_820X))) + (-3 + length_1063X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_820X))) + index_1064X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_820X))) + (1 + index_1064X)))));
              goto L20934;}
            else {
              ps_error("unknown protocol", 1, protocol_1058X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_817X;
              arg0K2 = list_args_818X;
              arg0K3 = list_arg_count_819X;
              goto L21791;}}}}}
    else {
      if ((protocol_1058X == total_arg_count_821X)) {
        if ((0 == list_arg_count_819X)) {
          arg0K0 = 2;
          arg0K1 = total_arg_count_821X;
          goto L20938;}
        else {
          merged_arg0K0 = list_args_818X;
          merged_arg0K1 = list_arg_count_819X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          arg0K0 = 2;
          arg0K1 = total_arg_count_821X;
          goto L20938;}}
      else {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_817X;
        arg0K2 = list_args_818X;
        arg0K3 = list_arg_count_819X;
        goto L21791;}}}}
 L24489: {
  pop_continuationB_return_tag = 4;
  goto pop_continuationB;
 pop_continuationB_return_4:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L21855: {
  cont_1065X = arg0K0;
  if ((1 == cont_1065X)) {
    if ((0 == (3 & (SvalS)))) {
      s48_Scallback_return_stack_blockS = 1;
      return ((((SvalS))>>2));}
    else {
      goto L21870;}}
  else {
    goto L21870;}}
 L24507: {
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  SvalS = (*((long *) ((((char *) (-3 + cont_846X))) + 16)));
  arg0K0 = 1;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L22419;}
 L45971: {
  v_1066X = arg0K0;
  SvalS = v_1066X;
  pop_continuationB_return_tag = 5;
  goto pop_continuationB;
 pop_continuationB_return_5:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L45919: {
  if ((1 == (stack_nargs_859X + list_arg_count_861X))) {
    if ((1 == stack_nargs_859X)) {
      SstackS = ((SstackS) + 4);
      arg0K0 = (*((long *) (SstackS)));
      goto L46012;}
    else {
      arg0K0 = (*((long *) (((char *) (-3 + list_args_860X)))));
      goto L46012;}}
  else {
    merged_arg0K0 = list_args_860X;
    merged_arg0K1 = stack_nargs_859X;
    pop_args_GlistSAgc_return_tag = 8;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_8:
    args_1067X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1067X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}}
 L45927: {
  SvalS = (*((long *) ((((char *) (-3 + cont_862X))) + 16)));
  arg0K0 = stack_nargs_859X;
  arg0K1 = list_args_860X;
  arg0K2 = list_arg_count_861X;
  goto L22419;}
 L25445: {
  loc_1068X = arg1K0;
  arg_1069X = arg1K1;
  if ((arg_top_872X < arg_1069X)) {
    *((long *) loc_1068X) = (*((long *) arg_1069X));
    arg1K0 = (loc_1068X + -4);
    arg1K1 = (arg_1069X + -4);
    goto L25445;}
  else {
    SstackS = loc_1068X;
    if ((count_870X < stack_nargs_859X)) {
      merged_arg0K0 = list_args_860X;
      merged_arg0K1 = (stack_nargs_859X - count_870X);
      pop_args_GlistSAgc_return_tag = 9;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_9:
      v_1070X = pop_args_GlistSAgc0_return_value;
      arg0K0 = v_1070X;
      goto L25424;}
    else {
      arg0K0 = stack_nargs_859X;
      arg0K1 = list_args_860X;
      goto L25406;}}}
 L25297: {
  count_1071X = arg0K0;
  bytes_used_1072X = arg0K1;
  stack_nargs_1073X = arg0K2;
  list_args_1074X = arg0K3;
  list_arg_count_1075X = arg0K4;
  if ((count_1071X == (stack_nargs_1073X + list_arg_count_1075X))) {
    arg_top_1076X = SstackS;
    pop_continuationB_return_tag = 6;
    goto pop_continuationB;
   pop_continuationB_return_6:
    arg1K0 = (SstackS);
    arg1K1 = (arg_top_1076X + (((stack_nargs_1073X)<<2)));
    goto L25329;}
  else {
    merged_arg0K0 = list_args_1074X;
    merged_arg0K1 = stack_nargs_1073X;
    pop_args_GlistSAgc_return_tag = 10;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_10:
    args_1077X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1077X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}}
 L23600: {
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L13093: {
  if ((3 == (3 & x_464X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L46477;}
    else {
      goto L13099;}}
  else {
    goto L13099;}}
 L38163: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L38304: {
  if ((3 == (3 & n_466X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      goto L38315;}
    else {
      goto L38316;}}
  else {
    goto L38316;}}
 L38503: {
  if ((3 == (3 & n_467X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      goto L38514;}
    else {
      goto L38515;}}
  else {
    goto L38515;}}
 L38702: {
  if ((3 == (3 & n_468X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      goto L38713;}
    else {
      goto L38714;}}
  else {
    goto L38714;}}
 L32045: {
  if ((0 == (3 & x_472X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16727;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_472X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16727;}}
 L32062: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_472X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_473X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L7851: {
  b_1078X = arg0K0;
  lo_a_1079X = 65535 & a_891X;
  lo_b_1080X = 65535 & b_1078X;
  hi_a_1081X = 65535 & (((a_891X)>>16));
  hi_b_1082X = 65535 & (((b_1078X)>>16));
  lo_c_1083X = SMALL_MULTIPLY(lo_a_1079X, lo_b_1080X);
  v_1084X = SMALL_MULTIPLY(lo_b_1080X, hi_a_1081X);
  v_1085X = SMALL_MULTIPLY(lo_a_1079X, hi_b_1082X);
  mid_c_1086X = v_1085X + v_1084X;
  c_1087X = lo_c_1083X + (((mid_c_1086X)<<16));
  if ((0 < hi_a_1081X)) {
    if ((0 < hi_b_1082X)) {
      arg0K0 = x_475X;
      arg0K1 = y_476X;
      goto L18645;}
    else {
      goto L7893;}}
  else {
    goto L7893;}}
 L40466: {
  arg0K0 = x_475X;
  arg0K1 = y_476X;
  goto L18645;}
 L32334: {
  if ((0 == (3 & x_479X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16684;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_479X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16684;}}
 L8109: {
  b_1088X = arg0K0;
  c_1089X = a_895X / b_1088X;
  x_1090X = 0 == (a_895X % b_1088X);
  if (x_1090X) {
    if ((a_484X < 0)) {
      if ((b_485X < 0)) {
        s48_make_availableAgc(16);
        if ((536870911 < c_1089X)) {
          goto L47861;}
        else {
          if ((c_1089X < -536870912)) {
            goto L47861;}
          else {
            arg0K0 = (((c_1089X)<<2));
            goto L47856;}}}
      else {
        goto L8155;}}
    else {
      if ((b_485X < 0)) {
        goto L8155;}
      else {
        s48_make_availableAgc(16);
        if ((536870911 < c_1089X)) {
          goto L47883;}
        else {
          if ((c_1089X < -536870912)) {
            goto L47883;}
          else {
            arg0K0 = (((c_1089X)<<2));
            goto L47878;}}}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_482X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_483X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}}
 L40621: {
  if ((0 == (3 & x_482X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16252;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_482X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16252;}}
 L32521: {
  if ((0 == (3 & (x_486X | y_487X)))) {
    if ((x_486X == y_487X)) {
      arg0K0 = 5;
      goto L32525;}
    else {
      arg0K0 = 1;
      goto L32525;}}
  else {
    if ((3 == (3 & x_486X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_486X))) + -4))))>>2))))) {
        if ((3 == (3 & y_487X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_487X))) + -4))))>>2))))) {
            b_1091X = s48_bignum_equal_p((((char *) (-3 + x_486X))), (((char *) (-3 + y_487X))));
            if (b_1091X) {
              arg0K0 = 5;
              goto L32525;}
            else {
              arg0K0 = 1;
              goto L32525;}}
          else {
            arg0K0 = 1;
            goto L32525;}}
        else {
          arg0K0 = 1;
          goto L32525;}}
      else {
        arg0K0 = 1;
        goto L32525;}}
    else {
      arg0K0 = 1;
      goto L32525;}}}
 L32698: {
  if ((0 == (3 & x_488X))) {
    if ((0 == (3 & y_489X))) {
      if ((x_488X < y_489X)) {
        arg0K0 = 5;
        goto L32702;}
      else {
        arg0K0 = 1;
        goto L32702;}}
    else {
      v_1092X = s48_bignum_test((((char *) (-3 + y_489X))));
      if ((1 == v_1092X)) {
        arg0K0 = 5;
        goto L32702;}
      else {
        arg0K0 = 1;
        goto L32702;}}}
  else {
    if ((0 == (3 & y_489X))) {
      v_1093X = s48_bignum_test((((char *) (-3 + x_488X))));
      if ((1 == v_1093X)) {
        arg0K0 = 1;
        goto L32702;}
      else {
        arg0K0 = 5;
        goto L32702;}}
    else {
      v_1094X = s48_bignum_compare((((char *) (-3 + x_488X))), (((char *) (-3 + y_489X))));
      if ((-1 == v_1094X)) {
        arg0K0 = 5;
        goto L32702;}
      else {
        arg0K0 = 1;
        goto L32702;}}}}
 L32938: {
  if ((0 == (3 & y_491X))) {
    if ((0 == (3 & x_490X))) {
      if ((y_491X < x_490X)) {
        arg0K0 = 5;
        goto L32942;}
      else {
        arg0K0 = 1;
        goto L32942;}}
    else {
      v_1095X = s48_bignum_test((((char *) (-3 + x_490X))));
      if ((1 == v_1095X)) {
        arg0K0 = 5;
        goto L32942;}
      else {
        arg0K0 = 1;
        goto L32942;}}}
  else {
    if ((0 == (3 & x_490X))) {
      v_1096X = s48_bignum_test((((char *) (-3 + y_491X))));
      if ((1 == v_1096X)) {
        arg0K0 = 1;
        goto L32942;}
      else {
        arg0K0 = 5;
        goto L32942;}}
    else {
      v_1097X = s48_bignum_compare((((char *) (-3 + y_491X))), (((char *) (-3 + x_490X))));
      if ((-1 == v_1097X)) {
        arg0K0 = 5;
        goto L32942;}
      else {
        arg0K0 = 1;
        goto L32942;}}}}
 L33178: {
  if ((0 == (3 & y_493X))) {
    if ((0 == (3 & x_492X))) {
      if ((y_493X < x_492X)) {
        arg0K0 = 1;
        goto L33182;}
      else {
        arg0K0 = 5;
        goto L33182;}}
    else {
      v_1098X = s48_bignum_test((((char *) (-3 + x_492X))));
      if ((1 == v_1098X)) {
        arg0K0 = 1;
        goto L33182;}
      else {
        arg0K0 = 5;
        goto L33182;}}}
  else {
    if ((0 == (3 & x_492X))) {
      v_1099X = s48_bignum_test((((char *) (-3 + y_493X))));
      if ((1 == v_1099X)) {
        arg0K0 = 5;
        goto L33182;}
      else {
        arg0K0 = 1;
        goto L33182;}}
    else {
      v_1100X = s48_bignum_compare((((char *) (-3 + y_493X))), (((char *) (-3 + x_492X))));
      if ((-1 == v_1100X)) {
        arg0K0 = 1;
        goto L33182;}
      else {
        arg0K0 = 5;
        goto L33182;}}}}
 L33372: {
  if ((0 == (3 & x_494X))) {
    if ((0 == (3 & y_495X))) {
      if ((x_494X < y_495X)) {
        arg0K0 = 1;
        goto L33376;}
      else {
        arg0K0 = 5;
        goto L33376;}}
    else {
      v_1101X = s48_bignum_test((((char *) (-3 + y_495X))));
      if ((1 == v_1101X)) {
        arg0K0 = 1;
        goto L33376;}
      else {
        arg0K0 = 5;
        goto L33376;}}}
  else {
    if ((0 == (3 & y_495X))) {
      v_1102X = s48_bignum_test((((char *) (-3 + x_494X))));
      if ((1 == v_1102X)) {
        arg0K0 = 5;
        goto L33376;}
      else {
        arg0K0 = 1;
        goto L33376;}}
    else {
      v_1103X = s48_bignum_compare((((char *) (-3 + x_494X))), (((char *) (-3 + y_495X))));
      if ((-1 == v_1103X)) {
        arg0K0 = 1;
        goto L33376;}
      else {
        arg0K0 = 5;
        goto L33376;}}}}
 L18584: {
  length0_1104X = arg0K0;
  extra0_1105X = arg0K1;
  if ((0 == (3 & y_902X))) {
    arg0K0 = 3;
    goto L18586;}
  else {
    arg0K0 = 0;
    goto L18586;}}
 L8754: {
  b_1106X = arg0K0;
  c_1107X = a_903X / b_1106X;
  if ((a_498X < 0)) {
    if ((b_499X < 0)) {
      goto L8800;}
    else {
      goto L8799;}}
  else {
    if ((b_499X < 0)) {
      goto L8799;}
    else {
      goto L8800;}}}
 L33572: {
  arg0K0 = x_496X;
  arg0K1 = y_497X;
  goto L18554;}
 L18493: {
  length0_1108X = arg0K0;
  extra0_1109X = arg0K1;
  if ((0 == (3 & y_905X))) {
    arg0K0 = 3;
    goto L18495;}
  else {
    arg0K0 = 0;
    goto L18495;}}
 L33782: {
  b_1110X = arg0K0;
  c_1111X = a_906X % b_1110X;
  if ((a_502X < 0)) {
    arg0K0 = (0 - c_1111X);
    goto L33786;}
  else {
    arg0K0 = c_1111X;
    goto L33786;}}
 L33748: {
  arg0K0 = x_500X;
  arg0K1 = y_501X;
  goto L18463;}
 L22068: {
  x_1112X = arg0K0;
  if ((536870911 < x_1112X)) {
    goto L22069;}
  else {
    if ((x_1112X < -536870912)) {
      goto L22069;}
    else {
      return (((x_1112X)<<2));}}}
 L18071: {
  length_1113X = arg0K0;
  extra_1114X = arg0K1;
  Stemp0S = x_908X;
  s48_make_availableAgc(((((length_1113X + extra_1114X))<<2)));
  value_1115X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1115X))) {
    v_1116X = (char *) s48_long_to_bignum((((value_1115X)>>2)));
    arg1K0 = v_1116X;
    goto L18063;}
  else {
    arg1K0 = (((char *) (-3 + value_1115X)));
    goto L18063;}}
 L18418: {
  length_1117X = arg0K0;
  extra_1118X = arg0K1;
  if ((length_1117X < 1)) {
    arg0K0 = 1;
    goto L18420;}
  else {
    arg0K0 = length_1117X;
    goto L18420;}}
 L34405: {
  x_1119X = arg0K0;
  count_1120X = arg0K1;
  if ((0 == x_1119X)) {
    SvalS = (((count_1120X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    arg0K0 = (((x_1119X)>>1));
    arg0K1 = (count_1120X + (1 & x_1119X));
    goto L34405;}}
 L18360: {
  length_1121X = arg0K0;
  extra_1122X = arg0K1;
  if ((length_1121X < 1)) {
    arg0K0 = 1;
    goto L18362;}
  else {
    arg0K0 = length_1121X;
    goto L18362;}}
 L34490: {
  if ((0 == (3 & x_528X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16466;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_528X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16466;}}
 L34653: {
  if ((0 == (3 & x_530X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16423;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_530X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16423;}}
 L34816: {
  if ((0 == (3 & x_532X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16380;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_532X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16380;}}
 L40968: {
  v_1123X = (char *) s48_long_to_bignum(x_915X);
  v_1124X = enter_bignum(v_1123X);
  arg0K0 = v_1124X;
  goto L40963;}
 L40963: {
  val_1125X = arg0K0;
  SvalS = val_1125X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L40990: {
  v_1126X = (char *) s48_long_to_bignum(result_917X);
  v_1127X = enter_bignum(v_1126X);
  arg0K0 = v_1127X;
  goto L40985;}
 L40985: {
  val_1128X = arg0K0;
  SvalS = val_1128X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L40861: {
  x_1129X = arg0K0;
  y_1130X = arg0K1;
  y_1131X = ((y_1130X)>>2);
  merged_arg0K0 = x_1129X;
  merged_arg0K1 = y_1131X;
  shift_space_return_tag = 1;
  goto shift_space;
 shift_space_return_1:
  needed_1132X = shift_space0_return_value;
  Stemp0S = x_1129X;
  s48_make_availableAgc((((needed_1132X)<<2)));
  value_1133X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1133X))) {
    v_1134X = (char *) s48_long_to_bignum((((value_1133X)>>2)));
    arg1K0 = v_1134X;
    goto L40935;}
  else {
    arg1K0 = (((char *) (-3 + value_1133X)));
    goto L40935;}}
 L41012: {
  v_1135X = (char *) s48_long_to_bignum(result_917X);
  v_1136X = enter_bignum(v_1135X);
  arg0K0 = v_1136X;
  goto L41007;}
 L41007: {
  val_1137X = arg0K0;
  SvalS = val_1137X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L41043: {
  x_1138X = arg1K0;
  external_bignum_1139X = (char *) s48_bignum_arithmetic_shift(x_1138X, y_919X);
  v_1140X = s48_bignum_fits_in_word_p(external_bignum_1139X, 30, 1);
  if (v_1140X) {
    n_1141X = s48_bignum_to_long(external_bignum_1139X);
    arg0K0 = (((n_1141X)<<2));
    goto L40869;}
  else {
    v_1142X = enter_bignum(external_bignum_1139X);
    arg0K0 = v_1142X;
    goto L40869;}}
 L40870: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_534X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_535X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L26715: {
  i_1143X = arg0K0;
  rest_list_1144X = arg0K1;
  if ((25 == rest_list_1144X)) {
    SvalS = new_558X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    *((long *) ((((char *) (-3 + new_558X))) + (((i_1143X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_1144X)))));
    arg0K0 = (1 + i_1143X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_1144X))) + 4)));
    goto L26715;}}
 L27128: {
  i_1145X = arg0K0;
  if ((i_1145X < 0)) {
    SvalS = x_931X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    addr_1146X = (((char *) (-3 + x_931X))) + (((i_1145X)<<2));
    S48_WRITE_BARRIER(x_931X, addr_1146X, value_932X);
    *((long *) addr_1146X) = value_932X;
    arg0K0 = (-1 + i_1145X);
    goto L27128;}}
 L35066: {
  i_1147X = arg0K0;
  if ((i_1147X < 0)) {
    SvalS = vector_935X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_935X))) + i_1147X)) = init_591X;
    arg0K0 = (-1 + i_1147X);
    goto L35066;}}
 L35246: {
  i_1148X = arg0K0;
  if ((i_1148X < 0)) {
    SvalS = vector_936X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_936X))) + i_1148X)) = (init_604X);
    arg0K0 = (-1 + i_1148X);
    goto L35246;}}
 L19800: {
  bucket_1149X = arg0K0;
  arg0K0 = bucket_1149X;
  goto L19806;}
 L39430: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L35438: {
  index_1150X = arg0K0;
  channel_1151X = make_registered_channel(mode_628X, arg2_627X, index_1150X, 0, &reason_1152X);
  if ((1 == channel_1151X)) {
    if ((3 == (3 & arg2_627X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_627X))) + -4))))>>2))))) {
        if ((1 == mode_628X)) {
          goto L35553;}
        else {
          if ((3 == mode_628X)) {
            goto L35553;}
          else {
            v_1153X = ps_close_fd(index_1150X);
            arg0K0 = v_1153X;
            goto L35548;}}}
      else {
        push_exception_continuationB(reason_1152X, 1);
        *((long *) (SstackS)) = arg2_627X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_628X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21289;}}
    else {
      push_exception_continuationB(reason_1152X, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21289;}}
  else {
    SvalS = channel_1151X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}}
 L35496: {
  v_1154X = ps_open_fd((((char *)(((char *) (-3 + arg2_627X))))), 1, &v_1155X);
  arg0K0 = v_1154X;
  arg0K1 = v_1155X;
  goto L35509;}
 L35509: {
  channel_1156X = arg0K0;
  status_1157X = arg0K1;
  if ((status_1157X == NO_ERRORS)) {
    arg0K0 = channel_1156X;
    goto L35438;}
  else {
    if ((status_1157X == ENOENT)) {
      push_exception_continuationB(10, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21289;}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg2_627X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_628X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1157X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_1158X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1158X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 3;
      goto L21289;}}}
 L42505: {
  waitP_1159X = arg2K0;
  start_1160X = ((arg3_634X)>>2);
  count_1161X = ((arg2_633X)>>2);
  v_1162X = 4 == (*((long *) (((char *) (-3 + arg5_636X)))));
  if (v_1162X) {
    if ((3 == (3 & arg4_635X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>2))))) {
        goto L35880;}
      else {
        goto L36095;}}
    else {
      goto L36095;}}
  else {
    goto L35948;}}
 L36259: {
  y_1163X = arg0K0;
  if ((y_1163X < (start_641X + count_642X))) {
    goto L36311;}
  else {
    got_1164X = ps_write_fd(((((*((long *) ((((char *) (-3 + arg4_640X))) + 8))))>>2)), ((((char *) (-3 + arg3_639X))) + start_641X), count_642X, &pendingP_1165X, &status_1166X);
    if ((status_1166X == NO_ERRORS)) {
      if (pendingP_1165X) {
        arg0K0 = 1;
        goto L36310;}
      else {
        arg0K0 = (((got_1164X)<<2));
        goto L36310;}}
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
      merged_arg0K0 = status_1166X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 4;
      goto get_error_string;
     get_error_string_return_4:
      x_1167X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1167X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 5;
      goto L21289;}}}
 L36446: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_639X))) + -4))))>>8));
  goto L36259;}
 L10658: {
  val_1168X = *((long *) ((((char *) (-3 + ch_949X))) + 12));
  addr_1169X = (((char *) (-3 + prev_950X))) + 12;
  S48_WRITE_BARRIER(prev_950X, addr_1169X, val_1168X);
  *((long *) addr_1169X) = val_1168X;
  addr_1170X = (((char *) (-3 + ch_949X))) + 12;
  S48_WRITE_BARRIER(ch_949X, addr_1170X, 1);
  *((long *) addr_1170X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_949X))) + 16)));
  goto L42843;}
 L15346: {
  v_1171X = arg0K0;
  arg0K0 = (-1 + i_954X);
  arg0K1 = v_1171X;
  goto L15332;}
 L36581: {
  if ((1 == proposal_657X)) {
    goto L36593;}
  else {
    addr_1172X = ((char *) (-3 + proposal_657X));
    S48_WRITE_BARRIER(proposal_657X, addr_1172X, 5);
    *((long *) addr_1172X) = 5;
    goto L36593;}}
 L9275: {
  i_1173X = arg0K0;
  stob_1174X = *((long *) ((((char *) (-3 + log_963X))) + (((i_1173X)<<2))));
  if ((1 == stob_1174X)) {
    copies_1175X = *((long *) ((((char *) (-3 + proposal_658X))) + 12));
    arg0K0 = copies_1175X;
    goto L36927;}
  else {
    verify_1176X = *((long *) ((((char *) (-3 + log_963X))) + (12 + (((i_1173X)<<2)))));
    value_1177X = *((long *) ((((char *) (-3 + log_963X))) + (8 + (((i_1173X)<<2)))));
    if ((29 == verify_1176X)) {
      if ((3 == (3 & stob_1174X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1174X))) + -4)))))) {
          goto L9327;}
        else {
          goto L36827;}}
      else {
        goto L36827;}}
    else {
      if ((verify_1176X == ((((*((unsigned char *) ((((char *) (-3 + stob_1174X))) + ((((*((long *) ((((char *) (-3 + log_963X))) + (4 + (((i_1173X)<<2)))))))>>2))))))<<2)))) {
        if ((verify_1176X == value_1177X)) {
          goto L9327;}
        else {
          if ((3 == (3 & stob_1174X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1174X))) + -4)))))) {
              goto L9327;}
            else {
              goto L36827;}}
          else {
            goto L36827;}}}
      else {
        goto L36827;}}}}
 L9089: {
  arg0K0 = (4 + i_961X);
  goto L9037;}
 L36827: {
  RELEASE_PROPOSAL_LOCK();
  x_1178X = Scurrent_threadS;
  addr_1179X = (((char *) (-3 + x_1178X))) + 12;
  S48_WRITE_BARRIER(x_1178X, addr_1179X, 1);
  *((long *) addr_1179X) = 1;
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L9845: {
  vector_1180X = arg0K0;
  if ((1 == vector_1180X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1181X;
    goto L9808;}
  else {
    arg0K0 = vector_1180X;
    goto L9808;}}
 L13919: {
  arg0K0 = (4 + i_974X);
  goto L13897;}
 L14093: {
  arg0K0 = (4 + i_977X);
  goto L14073;}
 L43716: {
  minutesP_1182X = arg2K0;
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_1183X = check_events0_return_value;
      if (temp_1183X) {
        goto L43742;}
      else {
        goto L43747;}}
    else {
      goto L43742;}}
  else {
    goto L43747;}}
 L29002: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proc_998X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = name_999X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L43096: {
  val_1184X = arg0K0;
  SvalS = val_1184X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L30580: {
  table_1185X = arg0K0;
  n_1186X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_739X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L19313;}
 L37288: {
  l_1187X = arg0K0;
  i_1188X = arg0K1;
  if ((i_1188X < 0)) {
    SvalS = string_1010X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1010X))) + i_1188X)) = ((((((*((long *) (((char *) (-3 + l_1187X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_1187X))) + 4)));
    arg0K1 = (-1 + i_1188X);
    goto L37288;}}
 L29685: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_1018X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L29665: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = port_1018X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L29926: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_1026X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L29906: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = port_1026X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21289;}
 L30161: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = Kchar_1032X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1033X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L30143: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = Kchar_1032X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1033X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L39337: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_1040X))) + 4)));
  goto L39331;}
 L14782: {
  if ((3 == (3 & thing_1041X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_1041X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_1041X)))));
      goto L14824;}
    else {
      goto L14790;}}
  else {
    goto L14790;}}
 L39322: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_784X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L21369: {
  StemplateS = (SvalS);
  Slosing_opcodeS = opcode_786X;
  arg0K0 = (2 + nargs_785X);
  arg0K1 = 25;
  arg0K2 = 0;
  goto L20919;}
 L21439: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L21369;}
 L26258: {
  env_1189X = arg0K0;
  i_1190X = arg0K1;
  if ((0 == i_1190X)) {
    count_1191X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_1046X)));
    arg0K0 = count_1191X;
    arg0K1 = i_1045X;
    arg0K2 = (2 + offset_1046X);
    goto L26197;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1189X)))));
    arg0K1 = (-1 + i_1190X);
    goto L26258;}}
 L26109: {
  env_1192X = arg0K0;
  i_1193X = arg0K1;
  if ((0 == i_1193X)) {
    index_1194X = 1 + offset_1050X;
    count_1195X = ((((*((unsigned char *) ((Scode_pointerS) + (1 + index_1194X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_1194X))));
    arg0K0 = count_1195X;
    arg0K1 = i_1049X;
    arg0K2 = (3 + offset_1050X);
    goto L26048;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1192X)))));
    arg0K1 = (-1 + i_1193X);
    goto L26109;}}
 L21656: {
  i_1196X = arg0K0;
  m_1197X = arg0K1;
  if ((0 == (n_1057X & m_1197X))) {
    arg0K0 = (1 + i_1196X);
    arg0K1 = (((m_1197X)<<1));
    goto L21656;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_1197X));
    if ((i_1196X == 0)) {
      *((long *) (SstackS)) = (Sinterrupted_templateS);
      SstackS = ((SstackS) + -4);
      Sinterrupted_templateS = 1;
      *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21552;}
    else {
      if ((i_1196X == 2)) {
        *((long *) (SstackS)) = (Sfinalize_theseS);
        SstackS = ((SstackS) + -4);
        Sfinalize_theseS = 25;
        *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21552;}
      else {
        if ((i_1196X == 3)) {
          channel_1198X = Spending_channels_headS;
          next_1199X = *((long *) ((((char *) (-3 + channel_1198X))) + 12));
          Spending_channels_headS = next_1199X;
          addr_1200X = (((char *) (-3 + channel_1198X))) + 12;
          S48_WRITE_BARRIER(channel_1198X, addr_1200X, 1);
          *((long *) addr_1200X) = 1;
          if ((1 == next_1199X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_1198X;
            goto L10819;}
          else {
            arg0K0 = channel_1198X;
            goto L10819;}}
        else {
          if ((i_1196X == 4)) {
            *((long *) (SstackS)) = (Sos_signal_typeS);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (Sos_signal_argumentS);
            SstackS = ((SstackS) + -4);
            Sos_signal_typeS = 1;
            Sos_signal_argumentS = 1;
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 3;
            goto L21552;}
          else {
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21552;}}}}}}
 L20938: {
  skip_1201X = arg0K0;
  stack_arg_count_1202X = arg0K1;
  template_1203X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_1203X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_1203X)))))))) + skip_1201X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  if ((stack_space_1059X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L20845;}
  else {
    s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
    merged_arg0K0 = 0;
    copy_stack_into_heap_return_tag = 3;
    goto copy_stack_into_heap;
   copy_stack_into_heap_return_3:
    if ((stack_space_1059X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L20845;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L20845;}}}
 L20971: {
  skip_1204X = arg0K0;
  if ((total_arg_count_821X < 3)) {
    arg0K0 = total_arg_count_821X;
    goto L20979;}
  else {
    if ((2 < stack_arg_count_817X)) {
      arg0K0 = stack_arg_count_817X;
      goto L20979;}
    else {
      arg0K0 = 2;
      goto L20979;}}}
 L21870: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  push_exception_continuationB(5, 0);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_1065X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L46012: {
  v_1205X = arg0K0;
  SvalS = v_1205X;
  arg0K0 = cont_867X;
  goto L21855;}
 L25424: {
  x_1206X = arg0K0;
  *((long *) (SstackS)) = x_1206X;
  SstackS = ((SstackS) + -4);
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L25406: {
  stack_nargs_1207X = arg0K0;
  l_1208X = arg0K1;
  if ((count_870X == stack_nargs_1207X)) {
    arg0K0 = l_1208X;
    goto L25424;}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1208X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (1 + stack_nargs_1207X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1208X))) + 4)));
    goto L25406;}}
 L25329: {
  loc_1209X = arg1K0;
  arg_1210X = arg1K1;
  if ((arg_top_1076X < arg_1210X)) {
    *((long *) loc_1209X) = (*((long *) arg_1210X));
    arg1K0 = (loc_1209X + -4);
    arg1K1 = (arg_1210X + -4);
    goto L25329;}
  else {
    SstackS = loc_1209X;
    if ((0 == list_arg_count_1075X)) {
      goto L25316;}
    else {
      merged_arg0K0 = list_args_1074X;
      merged_arg0K1 = list_arg_count_1075X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      goto L25316;}}}
 L13099: {
  if ((3 == (3 & x_464X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L46477;}
    else {
      arg0K0 = 1;
      goto L46477;}}
  else {
    arg0K0 = 1;
    goto L46477;}}
 L38316: {
  if ((3 == (3 & n_466X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_466X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
    else {
      goto L38320;}}
  else {
    goto L38320;}}
 L38515: {
  if ((3 == (3 & n_467X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_467X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
    else {
      goto L38519;}}
  else {
    goto L38519;}}
 L38714: {
  if ((3 == (3 & n_468X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_468X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21289;}
    else {
      goto L38718;}}
  else {
    goto L38718;}}
 L16727: {
  length0_1211X = arg0K0;
  extra0_1212X = arg0K1;
  if ((0 == (3 & y_473X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16735;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_473X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16735;}}
 L18645: {
  x_1213X = arg0K0;
  y_1214X = arg0K1;
  if ((0 == (3 & x_1213X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16647;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1213X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16647;}}
 L7893: {
  if ((536870911 < lo_c_1083X)) {
    arg0K0 = x_475X;
    arg0K1 = y_476X;
    goto L18645;}
  else {
    if ((lo_c_1083X < 0)) {
      arg0K0 = x_475X;
      arg0K1 = y_476X;
      goto L18645;}
    else {
      if ((8192 < mid_c_1086X)) {
        arg0K0 = x_475X;
        arg0K1 = y_476X;
        goto L18645;}
      else {
        if ((a_477X < 0)) {
          if ((b_478X < 0)) {
            s48_make_availableAgc(16);
            if ((536870911 < c_1087X)) {
              goto L47758;}
            else {
              if ((c_1087X < -536870912)) {
                goto L47758;}
              else {
                arg0K0 = (((c_1087X)<<2));
                goto L47753;}}}
          else {
            goto L7921;}}
        else {
          if ((b_478X < 0)) {
            goto L7921;}
          else {
            s48_make_availableAgc(16);
            if ((536870911 < c_1087X)) {
              goto L47780;}
            else {
              if ((c_1087X < -536870912)) {
                goto L47780;}
              else {
                arg0K0 = (((c_1087X)<<2));
                goto L47775;}}}}}}}}
 L16684: {
  length0_1215X = arg0K0;
  extra0_1216X = arg0K1;
  if ((0 == (3 & y_480X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16692;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_480X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16692;}}
 L47861: {
  v_1217X = (char *) s48_long_to_bignum(c_1089X);
  v_1218X = enter_bignum(v_1217X);
  arg0K0 = v_1218X;
  goto L47856;}
 L47856: {
  val_1219X = arg0K0;
  SvalS = val_1219X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L8155: {
  x_1220X = 0 - c_1089X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1220X)) {
    goto L47839;}
  else {
    if ((x_1220X < -536870912)) {
      goto L47839;}
    else {
      arg0K0 = (((x_1220X)<<2));
      goto L47834;}}}
 L47883: {
  v_1221X = (char *) s48_long_to_bignum(c_1089X);
  v_1222X = enter_bignum(v_1221X);
  arg0K0 = v_1222X;
  goto L47878;}
 L47878: {
  val_1223X = arg0K0;
  SvalS = val_1223X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L16252: {
  length0_1224X = arg0K0;
  extra0_1225X = arg0K1;
  if ((0 == (3 & y_483X))) {
    arg0K0 = 3;
    goto L16260;}
  else {
    arg0K0 = 0;
    goto L16260;}}
 L32525: {
  val_1226X = arg0K0;
  SvalS = val_1226X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L32702: {
  val_1227X = arg0K0;
  SvalS = val_1227X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L32942: {
  val_1228X = arg0K0;
  SvalS = val_1228X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L33182: {
  val_1229X = arg0K0;
  SvalS = val_1229X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L33376: {
  val_1230X = arg0K0;
  SvalS = val_1230X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L18586: {
  extra1_1231X = arg0K0;
  Stemp0S = x_901X;
  Stemp1S = y_902X;
  s48_make_availableAgc(((((((8 + (((length0_1104X)<<1))) + extra0_1105X) + extra1_1231X))<<2)));
  value_1232X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1232X))) {
    v_1233X = (char *) s48_long_to_bignum((((value_1232X)>>2)));
    arg1K0 = v_1233X;
    goto L18572;}
  else {
    arg1K0 = (((char *) (-3 + value_1232X)));
    goto L18572;}}
 L8800: {
  if ((536870911 < c_1107X)) {
    arg0K0 = x_496X;
    arg0K1 = y_497X;
    goto L18554;}
  else {
    SvalS = (((c_1107X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}}
 L8799: {
  SvalS = ((((0 - c_1107X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L18495: {
  extra1_1234X = arg0K0;
  Stemp0S = x_904X;
  Stemp1S = y_905X;
  s48_make_availableAgc(((((((8 + (((length0_1108X)<<1))) + extra0_1109X) + extra1_1234X))<<2)));
  value_1235X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1235X))) {
    v_1236X = (char *) s48_long_to_bignum((((value_1235X)>>2)));
    arg1K0 = v_1236X;
    goto L18481;}
  else {
    arg1K0 = (((char *) (-3 + value_1235X)));
    goto L18481;}}
 L33786: {
  n_1237X = arg0K0;
  SvalS = (((n_1237X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L22069: {
  v_1238X = (char *) s48_long_to_bignum(x_1112X);
  return enter_bignum(v_1238X);}
 L18063: {
  x_1239X = arg1K0;
  v_1240X = s48_bignum_test(x_1239X);
  if ((-1 == v_1240X)) {
    v_1241X = (char *) s48_bignum_negate(x_1239X);
    arg1K0 = v_1241X;
    goto L18065;}
  else {
    arg1K0 = x_1239X;
    goto L18065;}}
 L18420: {
  v_1242X = arg0K0;
  Stemp0S = x_525X;
  s48_make_availableAgc((((((3 + v_1242X) + extra_1118X))<<2)));
  value_1243X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1243X))) {
    v_1244X = (char *) s48_long_to_bignum((((value_1243X)>>2)));
    arg1K0 = v_1244X;
    goto L18410;}
  else {
    arg1K0 = (((char *) (-3 + value_1243X)));
    goto L18410;}}
 L18362: {
  v_1245X = arg0K0;
  Stemp0S = x_526X;
  s48_make_availableAgc((((((3 + v_1245X) + extra_1122X))<<2)));
  value_1246X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1246X))) {
    v_1247X = (char *) s48_long_to_bignum((((value_1246X)>>2)));
    arg1K0 = v_1247X;
    goto L18352;}
  else {
    arg1K0 = (((char *) (-3 + value_1246X)));
    goto L18352;}}
 L16466: {
  length0_1248X = arg0K0;
  extra0_1249X = arg0K1;
  if ((0 == (3 & y_529X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16474;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_529X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16474;}}
 L16423: {
  length0_1250X = arg0K0;
  extra0_1251X = arg0K1;
  if ((0 == (3 & y_531X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16431;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_531X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16431;}}
 L16380: {
  length0_1252X = arg0K0;
  extra0_1253X = arg0K1;
  if ((0 == (3 & y_533X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16388;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_533X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16388;}}
 L40935: {
  x_1254X = arg1K0;
  external_bignum_1255X = (char *) s48_bignum_arithmetic_shift(x_1254X, y_1131X);
  v_1256X = s48_bignum_fits_in_word_p(external_bignum_1255X, 30, 1);
  if (v_1256X) {
    n_1257X = s48_bignum_to_long(external_bignum_1255X);
    arg0K0 = (((n_1257X)<<2));
    goto L40863;}
  else {
    v_1258X = enter_bignum(external_bignum_1255X);
    arg0K0 = v_1258X;
    goto L40863;}}
 L40869: {
  val_1259X = arg0K0;
  SvalS = val_1259X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L19806: {
  foo_1260X = arg0K0;
  if ((1 == foo_1260X)) {
    if ((3 == (3 & bucket_1149X))) {
      arg0K0 = (-4 & bucket_1149X);
      goto L19811;}
    else {
      arg0K0 = bucket_1149X;
      goto L19811;}}
  else {
    s2_1261X = *((long *) (((char *) (-3 + foo_1260X))));
    len_1262X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_618X))) + -4))))>>8);
    if ((len_1262X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1261X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1261X))), (void *)(((char *) (-3 + string_618X))),len_1262X)))) {
        arg0K0 = foo_1260X;
        goto L31926;}
      else {
        goto L19826;}}
    else {
      goto L19826;}}}
 L35553: {
  v_1263X = ps_close_fd(index_1150X);
  arg0K0 = v_1263X;
  goto L35548;}
 L35548: {
  status_1264X = arg0K0;
  if ((status_1264X == NO_ERRORS)) {
    push_exception_continuationB(reason_1152X, 1);
    *((long *) (SstackS)) = arg2_627X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_628X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}
  else {
    channel_close_error(status_1264X, index_1150X, arg2_627X);
    push_exception_continuationB(reason_1152X, 1);
    *((long *) (SstackS)) = arg2_627X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_628X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21289;}}
 L35880: {
  if ((3 == (3 & arg4_635X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_635X))) + -4)))))) {
      if ((3 == (3 & arg4_635X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>2))))) {
          arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>8)));
          goto L35887;}
        else {
          goto L36121;}}
      else {
        goto L36121;}}
    else {
      goto L35948;}}
  else {
    goto L35948;}}
 L36095: {
  if ((3 == (3 & arg4_635X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>2))))) {
      goto L35880;}
    else {
      goto L35948;}}
  else {
    goto L35948;}}
 L35948: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_636X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_635X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1160X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1161X)<<2));
  SstackS = ((SstackS) + -4);
  if (waitP_1159X) {
    arg0K0 = 5;
    goto L35964;}
  else {
    arg0K0 = 1;
    goto L35964;}}
 L36310: {
  val_1265X = arg0K0;
  SvalS = val_1265X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L36593: {
  x_1266X = Scurrent_threadS;
  addr_1267X = (((char *) (-3 + x_1266X))) + 12;
  S48_WRITE_BARRIER(x_1266X, addr_1267X, proposal_657X);
  *((long *) addr_1267X) = proposal_657X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L36927: {
  copies_1268X = arg0K0;
  if ((1 == copies_1268X)) {
    log_1269X = *((long *) ((((char *) (-3 + proposal_658X))) + 4));
    arg0K0 = 0;
    goto L8579;}
  else {
    thing_1270X = *((long *) ((((char *) (-3 + copies_1268X))) + 8));
    if ((3 == (3 & thing_1270X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + thing_1270X))) + -4)))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + copies_1268X))) + 20)));
        goto L36927;}
      else {
        goto L36827;}}
    else {
      goto L36827;}}}
 L9327: {
  arg0K0 = (4 + i_1173X);
  goto L9275;}
 L9808: {
  entry_1271X = arg0K0;
  proposal_1272X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  value_1273X = Stemp0S;
  Stemp0S = 1;
  addr_1274X = ((char *) (-3 + entry_1271X));
  S48_WRITE_BARRIER(entry_1271X, addr_1274X, value_1273X);
  *((long *) addr_1274X) = value_1273X;
  addr_1275X = (((char *) (-3 + entry_1271X))) + 4;
  S48_WRITE_BARRIER(entry_1271X, addr_1275X, from_index_969X);
  *((long *) addr_1275X) = from_index_969X;
  value_1276X = Stemp1S;
  Stemp1S = 1;
  addr_1277X = (((char *) (-3 + entry_1271X))) + 8;
  S48_WRITE_BARRIER(entry_1271X, addr_1277X, value_1276X);
  *((long *) addr_1277X) = value_1276X;
  addr_1278X = (((char *) (-3 + entry_1271X))) + 12;
  S48_WRITE_BARRIER(entry_1271X, addr_1278X, to_index_970X);
  *((long *) addr_1278X) = to_index_970X;
  addr_1279X = (((char *) (-3 + entry_1271X))) + 16;
  S48_WRITE_BARRIER(entry_1271X, addr_1279X, count_971X);
  *((long *) addr_1279X) = count_971X;
  value_1280X = *((long *) ((((char *) (-3 + proposal_1272X))) + 12));
  addr_1281X = (((char *) (-3 + entry_1271X))) + 20;
  S48_WRITE_BARRIER(entry_1271X, addr_1281X, value_1280X);
  *((long *) addr_1281X) = value_1280X;
  addr_1282X = (((char *) (-3 + proposal_1272X))) + 12;
  S48_WRITE_BARRIER(proposal_1272X, addr_1282X, entry_1271X);
  *((long *) addr_1282X) = entry_1271X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L43742: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L43747: {
  if ((0 == (Spending_interruptsS))) {
    s48_wait_for_event((((arg2_732X)>>2)), minutesP_1182X);
    goto L43742;}
  else {
    goto L43742;}}
 L19313: {
  i_1283X = arg0K0;
  h_1284X = arg0K1;
  if ((i_1283X < n_1186X)) {
    arg0K0 = (1 + i_1283X);
    arg0K1 = (h_1284X + (((*((unsigned char *) ((((char *) (-3 + arg2_739X))) + i_1283X))))));
    goto L19313;}
  else {
    index_1285X = 1023 & h_1284X;
    link_1286X = *((long *) ((((char *) (-3 + table_1185X))) + (((index_1285X)<<2))));
    if ((0 == (3 & link_1286X))) {
      arg0K0 = (3 + (-4 & link_1286X));
      goto L19268;}
    else {
      arg0K0 = link_1286X;
      goto L19268;}}}
 L14824: {
  v_1287X = arg3K0;
  ps_write_string(v_1287X, out_784X);
  goto L39337;}
 L14790: {
  if ((3 == (3 & thing_1041X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_1041X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_1041X))))))))));
      goto L14824;}
    else {
      goto L14798;}}
  else {
    goto L14798;}}
 L26197: {
  count_1288X = arg0K0;
  i_1289X = arg0K1;
  offset_1290X = arg0K2;
  if ((0 == count_1288X)) {
    arg0K0 = i_1289X;
    arg0K1 = offset_1290X;
    arg0K2 = env_1189X;
    goto L26180;}
  else {
    value_1291X = *((long *) ((((char *) (-3 + env_1189X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1290X)))))<<2))));
    addr_1292X = (((char *) (-3 + new_env_412X))) + (((i_1289X)<<2));
    S48_WRITE_BARRIER(new_env_412X, addr_1292X, value_1291X);
    *((long *) addr_1292X) = value_1291X;
    arg0K0 = (-1 + count_1288X);
    arg0K1 = (1 + i_1289X);
    arg0K2 = (1 + offset_1290X);
    goto L26197;}}
 L26048: {
  count_1293X = arg0K0;
  i_1294X = arg0K1;
  offset_1295X = arg0K2;
  if ((0 == count_1293X)) {
    arg0K0 = i_1294X;
    arg0K1 = offset_1295X;
    arg0K2 = env_1192X;
    goto L26031;}
  else {
    value_1296X = *((long *) ((((char *) (-3 + env_1192X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1295X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1295X))))))<<2))));
    addr_1297X = (((char *) (-3 + new_env_419X))) + (((i_1294X)<<2));
    S48_WRITE_BARRIER(new_env_419X, addr_1297X, value_1296X);
    *((long *) addr_1297X) = value_1296X;
    arg0K0 = (-1 + count_1293X);
    arg0K1 = (1 + i_1294X);
    arg0K2 = (2 + offset_1295X);
    goto L26048;}}
 L21552: {
  arg_count_1298X = arg0K0;
  handlers_1299X = SHARED_REF((Sinterrupt_handlersS));
  if ((3 == (3 & handlers_1299X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_1299X))) + -4))))>>2))))) {
      goto L21570;}
    else {
      goto L21669;}}
  else {
    goto L21669;}}
 L10819: {
  channel_1300X = arg0K0;
  x_1301X = 1 == (Spending_channels_headS);
  if (x_1301X) {
    goto L10833;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L10833;}
      else {
        goto L10833;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L10833;}}}
 L20845: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 3;
      goto check_events;
     check_events_return_3:
      v_1302X = check_events0_return_value;
      if (v_1302X) {
        arg0K0 = stack_arg_count_1202X;
        goto L21522;}
      else {
        goto L20849;}}
    else {
      arg0K0 = stack_arg_count_1202X;
      goto L21522;}}
  else {
    goto L20849;}}
 L20979: {
  final_stack_arg_count_1303X = arg0K0;
  if ((stack_arg_count_817X < final_stack_arg_count_1303X)) {
    arg0K0 = final_stack_arg_count_1303X;
    goto L20983;}
  else {
    arg0K0 = stack_arg_count_817X;
    goto L20983;}}
 L25316: {
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1072X));
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L38320: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L38519: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L38718: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L16735: {
  length1_1304X = arg0K0;
  extra1_1305X = arg0K1;
  if ((length0_1211X < length1_1304X)) {
    arg0K0 = length1_1304X;
    goto L16757;}
  else {
    arg0K0 = length0_1211X;
    goto L16757;}}
 L16647: {
  length0_1306X = arg0K0;
  extra0_1307X = arg0K1;
  if ((0 == (3 & y_1214X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16655;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_1214X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16655;}}
 L47758: {
  v_1308X = (char *) s48_long_to_bignum(c_1087X);
  v_1309X = enter_bignum(v_1308X);
  arg0K0 = v_1309X;
  goto L47753;}
 L47753: {
  val_1310X = arg0K0;
  SvalS = val_1310X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L7921: {
  x_1311X = 0 - c_1087X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1311X)) {
    goto L47736;}
  else {
    if ((x_1311X < -536870912)) {
      goto L47736;}
    else {
      arg0K0 = (((x_1311X)<<2));
      goto L47731;}}}
 L47780: {
  v_1312X = (char *) s48_long_to_bignum(c_1087X);
  v_1313X = enter_bignum(v_1312X);
  arg0K0 = v_1313X;
  goto L47775;}
 L47775: {
  val_1314X = arg0K0;
  SvalS = val_1314X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L16692: {
  length1_1315X = arg0K0;
  extra1_1316X = arg0K1;
  if ((length0_1215X < length1_1315X)) {
    arg0K0 = length1_1315X;
    goto L16714;}
  else {
    arg0K0 = length0_1215X;
    goto L16714;}}
 L47839: {
  v_1317X = (char *) s48_long_to_bignum(x_1220X);
  v_1318X = enter_bignum(v_1317X);
  arg0K0 = v_1318X;
  goto L47834;}
 L47834: {
  val_1319X = arg0K0;
  SvalS = val_1319X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L16260: {
  extra1_1320X = arg0K0;
  Stemp0S = x_482X;
  Stemp1S = y_483X;
  s48_make_availableAgc(((((((8 + (((length0_1224X)<<1))) + extra0_1225X) + extra1_1320X))<<2)));
  value_1321X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1321X))) {
    v_1322X = (char *) s48_long_to_bignum((((value_1321X)>>2)));
    arg1K0 = v_1322X;
    goto L17899;}
  else {
    arg1K0 = (((char *) (-3 + value_1321X)));
    goto L17899;}}
 L18572: {
  y_1323X = arg1K0;
  value_1324X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1324X))) {
    v_1325X = (char *) s48_long_to_bignum((((value_1324X)>>2)));
    arg1K0 = v_1325X;
    goto L18568;}
  else {
    arg1K0 = (((char *) (-3 + value_1324X)));
    goto L18568;}}
 L18481: {
  y_1326X = arg1K0;
  value_1327X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1327X))) {
    v_1328X = (char *) s48_long_to_bignum((((value_1327X)>>2)));
    arg1K0 = v_1328X;
    goto L18477;}
  else {
    arg1K0 = (((char *) (-3 + value_1327X)));
    goto L18477;}}
 L18065: {
  external_bignum_1329X = arg1K0;
  v_1330X = s48_bignum_fits_in_word_p(external_bignum_1329X, 30, 1);
  if (v_1330X) {
    n_1331X = s48_bignum_to_long(external_bignum_1329X);
    return (((n_1331X)<<2));}
  else {
    return enter_bignum(external_bignum_1329X);}}
 L18410: {
  v_1332X = arg1K0;
  external_bignum_1333X = (char *) s48_bignum_bitwise_not(v_1332X);
  v_1334X = s48_bignum_fits_in_word_p(external_bignum_1333X, 30, 1);
  if (v_1334X) {
    n_1335X = s48_bignum_to_long(external_bignum_1333X);
    arg0K0 = (((n_1335X)<<2));
    goto L34310;}
  else {
    val_1336X = enter_bignum(external_bignum_1333X);
    arg0K0 = val_1336X;
    goto L34310;}}
 L18352: {
  v_1337X = arg1K0;
  n_1338X = s48_bignum_bit_count(v_1337X);
  SvalS = (((n_1338X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L16474: {
  length1_1339X = arg0K0;
  extra1_1340X = arg0K1;
  if ((length0_1248X < length1_1339X)) {
    arg0K0 = length1_1339X;
    goto L16496;}
  else {
    arg0K0 = length0_1248X;
    goto L16496;}}
 L16431: {
  length1_1341X = arg0K0;
  extra1_1342X = arg0K1;
  if ((length0_1250X < length1_1341X)) {
    arg0K0 = length1_1341X;
    goto L16453;}
  else {
    arg0K0 = length0_1250X;
    goto L16453;}}
 L16388: {
  length1_1343X = arg0K0;
  extra1_1344X = arg0K1;
  if ((length0_1252X < length1_1343X)) {
    arg0K0 = length1_1343X;
    goto L16410;}
  else {
    arg0K0 = length0_1252X;
    goto L16410;}}
 L40863: {
  val_1345X = arg0K0;
  SvalS = val_1345X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L19811: {
  b_1346X = arg0K0;
  addr_1347X = s48_allocate_small(12);
  *((long *) addr_1347X) = 2054;
  x_1348X = 3 + (((long) (addr_1347X + 4)));
  *((long *) (((char *) (-3 + x_1348X)))) = string_618X;
  *((long *) ((((char *) (-3 + x_1348X))) + 4)) = b_1346X;
  if ((3 == (3 & x_1348X))) {
    arg0K0 = (-4 & x_1348X);
    goto L19817;}
  else {
    arg0K0 = x_1348X;
    goto L19817;}}
 L31926: {
  val_1349X = arg0K0;
  SvalS = val_1349X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L19826: {
  link_1350X = *((long *) ((((char *) (-3 + foo_1260X))) + 4));
  if ((0 == (3 & link_1350X))) {
    arg0K0 = (3 + (-4 & link_1350X));
    goto L19806;}
  else {
    arg0K0 = link_1350X;
    goto L19806;}}
 L35887: {
  y_1351X = arg0K0;
  if ((y_1351X < (start_1160X + count_1161X))) {
    goto L35948;}
  else {
    got_1352X = ps_read_fd(((((*((long *) ((((char *) (-3 + arg5_636X))) + 8))))>>2)), ((((char *) (-3 + arg4_635X))) + start_1160X), count_1161X, waitP_1159X, &eofP_1353X, &pendingP_1354X, &status_1355X);
    if ((status_1355X == NO_ERRORS)) {
      if (eofP_1353X) {
        arg0K0 = 21;
        goto L35947;}
      else {
        if (pendingP_1354X) {
          arg0K0 = 1;
          goto L35947;}
        else {
          arg0K0 = (((got_1352X)<<2));
          goto L35947;}}}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg5_636X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_635X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1160X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1161X)<<2));
      SstackS = ((SstackS) + -4);
      if (waitP_1159X) {
        arg0K0 = 5;
        goto L35930;}
      else {
        arg0K0 = 1;
        goto L35930;}}}}
 L36121: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_635X))) + -4))))>>8));
  goto L35887;}
 L35964: {
  x_1356X = arg0K0;
  *((long *) (SstackS)) = x_1356X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21289;}
 L8579: {
  i_1357X = arg0K0;
  stob_1358X = *((long *) ((((char *) (-3 + log_1269X))) + (((i_1357X)<<2))));
  if ((1 == stob_1358X)) {
    log_1359X = *((long *) ((((char *) (-3 + proposal_658X))) + 8));
    arg0K0 = 0;
    goto L11486;}
  else {
    verify_1360X = *((long *) ((((char *) (-3 + log_1269X))) + (12 + (((i_1357X)<<2)))));
    value_1361X = *((long *) ((((char *) (-3 + log_1269X))) + (8 + (((i_1357X)<<2)))));
    if ((verify_1360X == value_1361X)) {
      goto L8611;}
    else {
      addr_1362X = (((char *) (-3 + stob_1358X))) + (-4 & (*((long *) ((((char *) (-3 + log_1269X))) + (4 + (((i_1357X)<<2)))))));
      S48_WRITE_BARRIER(stob_1358X, addr_1362X, value_1361X);
      *((long *) addr_1362X) = value_1361X;
      goto L8611;}}}
 L19268: {
  bucket_1363X = arg0K0;
  arg0K0 = 1;
  arg0K1 = bucket_1363X;
  goto L19275;}
 L14798: {
  if ((1 == thing_1041X)) {
    goto L14801;}
  else {
    if ((5 == thing_1041X)) {
      goto L14801;}
    else {
      if ((25 == thing_1041X)) {
        arg3K0 = "()";
        goto L14824;}
      else {
        if ((3 == (3 & thing_1041X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_1041X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L14824;}
          else {
            goto L14814;}}
        else {
          goto L14814;}}}}}
 L21570: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Spending_interruptPS = 0;
    if ((s48_Spending_eventsPS)) {
      s48_Spending_interruptPS = 1;
      goto L21572;}
    else {
      goto L21572;}}
  else {
    s48_Spending_interruptPS = 1;
    goto L21572;}}
 L21669: {
  ps_error("interrupt handler is not a vector", 0);
  goto L21570;}
 L10833: {
  *((long *) (SstackS)) = channel_1300X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + channel_1300X))) + 16)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21552;}
 L20849: {
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L20983: {
  v_1364X = arg0K0;
  merged_arg0K0 = v_1364X;
  merged_arg0K1 = stack_arg_count_817X;
  merged_arg0K2 = list_args_818X;
  merged_arg0K3 = list_arg_count_819X;
  rest_list_setupAgc_return_tag = 1;
  goto rest_list_setupAgc;
 rest_list_setupAgc_return_1:
  *((long *) (SstackS)) = (((final_stack_arg_count_1303X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_821X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_1204X;
  arg0K1 = (3 + final_stack_arg_count_1303X);
  goto L20938;}
 L16757: {
  v_1365X = arg0K0;
  Stemp0S = x_472X;
  Stemp1S = y_473X;
  s48_make_availableAgc(((((((3 + v_1365X) + extra0_1212X) + extra1_1305X))<<2)));
  value_1366X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1366X))) {
    v_1367X = (char *) s48_long_to_bignum((((value_1366X)>>2)));
    arg1K0 = v_1367X;
    goto L18811;}
  else {
    arg1K0 = (((char *) (-3 + value_1366X)));
    goto L18811;}}
 L16655: {
  length1_1368X = arg0K0;
  extra1_1369X = arg0K1;
  Stemp0S = x_1213X;
  Stemp1S = y_1214X;
  s48_make_availableAgc(((((((2 + (length0_1306X + length1_1368X)) + extra0_1307X) + extra1_1369X))<<2)));
  value_1370X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1370X))) {
    v_1371X = (char *) s48_long_to_bignum((((value_1370X)>>2)));
    arg1K0 = v_1371X;
    goto L18663;}
  else {
    arg1K0 = (((char *) (-3 + value_1370X)));
    goto L18663;}}
 L47736: {
  v_1372X = (char *) s48_long_to_bignum(x_1311X);
  v_1373X = enter_bignum(v_1372X);
  arg0K0 = v_1373X;
  goto L47731;}
 L47731: {
  val_1374X = arg0K0;
  SvalS = val_1374X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L16714: {
  v_1375X = arg0K0;
  Stemp0S = x_479X;
  Stemp1S = y_480X;
  s48_make_availableAgc(((((((3 + v_1375X) + extra0_1216X) + extra1_1316X))<<2)));
  value_1376X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1376X))) {
    v_1377X = (char *) s48_long_to_bignum((((value_1376X)>>2)));
    arg1K0 = v_1377X;
    goto L18737;}
  else {
    arg1K0 = (((char *) (-3 + value_1376X)));
    goto L18737;}}
 L17899: {
  y_1378X = arg1K0;
  value_1379X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1379X))) {
    v_1380X = (char *) s48_long_to_bignum((((value_1379X)>>2)));
    arg1K0 = v_1380X;
    goto L17895;}
  else {
    arg1K0 = (((char *) (-3 + value_1379X)));
    goto L17895;}}
 L18568: {
  x_1381X = arg1K0;
  external_bignum_1382X = (char *)s48_bignum_quotient(x_1381X, y_1323X);
  v_1383X = s48_bignum_fits_in_word_p(external_bignum_1382X, 30, 1);
  if (v_1383X) {
    n_1384X = s48_bignum_to_long(external_bignum_1382X);
    arg0K0 = (((n_1384X)<<2));
    goto L48540;}
  else {
    val_1385X = enter_bignum(external_bignum_1382X);
    arg0K0 = val_1385X;
    goto L48540;}}
 L18477: {
  x_1386X = arg1K0;
  external_bignum_1387X = (char *)s48_bignum_remainder(x_1386X, y_1326X);
  v_1388X = s48_bignum_fits_in_word_p(external_bignum_1387X, 30, 1);
  if (v_1388X) {
    n_1389X = s48_bignum_to_long(external_bignum_1387X);
    arg0K0 = (((n_1389X)<<2));
    goto L48540;}
  else {
    val_1390X = enter_bignum(external_bignum_1387X);
    arg0K0 = val_1390X;
    goto L48540;}}
 L34310: {
  val_1391X = arg0K0;
  SvalS = val_1391X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L16496: {
  v_1392X = arg0K0;
  Stemp0S = x_528X;
  Stemp1S = y_529X;
  s48_make_availableAgc(((((((3 + v_1392X) + extra0_1249X) + extra1_1340X))<<2)));
  value_1393X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1393X))) {
    v_1394X = (char *) s48_long_to_bignum((((value_1393X)>>2)));
    arg1K0 = v_1394X;
    goto L18285;}
  else {
    arg1K0 = (((char *) (-3 + value_1393X)));
    goto L18285;}}
 L16453: {
  v_1395X = arg0K0;
  Stemp0S = x_530X;
  Stemp1S = y_531X;
  s48_make_availableAgc(((((((3 + v_1395X) + extra0_1251X) + extra1_1342X))<<2)));
  value_1396X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1396X))) {
    v_1397X = (char *) s48_long_to_bignum((((value_1396X)>>2)));
    arg1K0 = v_1397X;
    goto L18211;}
  else {
    arg1K0 = (((char *) (-3 + value_1396X)));
    goto L18211;}}
 L16410: {
  v_1398X = arg0K0;
  Stemp0S = x_532X;
  Stemp1S = y_533X;
  s48_make_availableAgc(((((((3 + v_1398X) + extra0_1253X) + extra1_1344X))<<2)));
  value_1399X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1399X))) {
    v_1400X = (char *) s48_long_to_bignum((((value_1399X)>>2)));
    arg1K0 = v_1400X;
    goto L18137;}
  else {
    arg1K0 = (((char *) (-3 + value_1399X)));
    goto L18137;}}
 L19817: {
  value_1401X = arg0K0;
  addr_1402X = (((char *) (-3 + table_617X))) + (((index_939X)<<2));
  S48_WRITE_BARRIER(table_617X, addr_1402X, value_1401X);
  *((long *) addr_1402X) = value_1401X;
  arg0K0 = x_1348X;
  goto L31926;}
 L35947: {
  val_1403X = arg0K0;
  SvalS = val_1403X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L35930: {
  x_1404X = arg0K0;
  *((long *) (SstackS)) = x_1404X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_1355X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 5;
  goto get_error_string;
 get_error_string_return_5:
  x_1405X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_1405X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 6;
  goto L21289;}
 L11486: {
  i_1406X = arg0K0;
  stob_1407X = *((long *) ((((char *) (-3 + log_1359X))) + (((i_1406X)<<2))));
  if ((1 == stob_1407X)) {
    copies_1408X = *((long *) ((((char *) (-3 + proposal_658X))) + 12));
    arg0K0 = copies_1408X;
    goto L11401;}
  else {
    verify_1409X = *((long *) ((((char *) (-3 + log_1359X))) + (12 + (((i_1406X)<<2)))));
    value_1410X = *((long *) ((((char *) (-3 + log_1359X))) + (8 + (((i_1406X)<<2)))));
    if ((verify_1409X == value_1410X)) {
      goto L11518;}
    else {
      *((unsigned char *) ((((char *) (-3 + stob_1407X))) + ((((*((long *) ((((char *) (-3 + log_1359X))) + (4 + (((i_1406X)<<2)))))))>>2)))) = (((value_1410X)>>2));
      goto L11518;}}}
 L8611: {
  arg0K0 = (4 + i_1357X);
  goto L8579;}
 L19275: {
  previous_foo_1411X = arg0K0;
  foo_1412X = arg0K1;
  if ((1 == foo_1412X)) {
    goto L30582;}
  else {
    s2_1413X = *((long *) (((char *) (-3 + foo_1412X))));
    len_1414X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_739X))) + -4))))>>8);
    if ((len_1414X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1413X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1413X))), (void *)(((char *) (-3 + arg2_739X))),len_1414X)))) {
        if ((1 == previous_foo_1411X)) {
          value_1415X = *((long *) ((((char *) (-3 + foo_1412X))) + 12));
          addr_1416X = (((char *) (-3 + table_1185X))) + (((index_1285X)<<2));
          S48_WRITE_BARRIER(table_1185X, addr_1416X, value_1415X);
          *((long *) addr_1416X) = value_1415X;
          goto L30582;}
        else {
          val_1417X = *((long *) ((((char *) (-3 + foo_1412X))) + 12));
          addr_1418X = (((char *) (-3 + previous_foo_1411X))) + 12;
          S48_WRITE_BARRIER(previous_foo_1411X, addr_1418X, val_1417X);
          *((long *) addr_1418X) = val_1417X;
          goto L30582;}}
      else {
        goto L19343;}}
    else {
      goto L19343;}}}
 L14801: {
  if ((1 == thing_1041X)) {
    arg3K0 = "#f";
    goto L14824;}
  else {
    arg3K0 = "#t";
    goto L14824;}}
 L14814: {
  if ((3 == (3 & thing_1041X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_1041X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L14824;}
    else {
      goto L14818;}}
  else {
    goto L14818;}}
 L21572: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_1299X))) + (((i_1196X)<<2)))));
  obj_1419X = SvalS;
  if ((3 == (3 & obj_1419X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1419X))) + -4))))>>2))))) {
      goto L21589;}
    else {
      goto L21694;}}
  else {
    goto L21694;}}
 L18811: {
  y_1420X = arg1K0;
  value_1421X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1421X))) {
    v_1422X = (char *) s48_long_to_bignum((((value_1421X)>>2)));
    arg1K0 = v_1422X;
    goto L18807;}
  else {
    arg1K0 = (((char *) (-3 + value_1421X)));
    goto L18807;}}
 L18663: {
  y_1423X = arg1K0;
  value_1424X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1424X))) {
    v_1425X = (char *) s48_long_to_bignum((((value_1424X)>>2)));
    arg1K0 = v_1425X;
    goto L18659;}
  else {
    arg1K0 = (((char *) (-3 + value_1424X)));
    goto L18659;}}
 L18737: {
  y_1426X = arg1K0;
  value_1427X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1427X))) {
    v_1428X = (char *) s48_long_to_bignum((((value_1427X)>>2)));
    arg1K0 = v_1428X;
    goto L18733;}
  else {
    arg1K0 = (((char *) (-3 + value_1427X)));
    goto L18733;}}
 L17895: {
  x_1429X = arg1K0;
  div_by_zeroP_1430X = s48_bignum_divide(x_1429X, y_1378X, &quot_1431X, &rem_1432X);
  if (div_by_zeroP_1430X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = 0;
    goto L40626;}
  else {
    v_1433X = s48_bignum_fits_in_word_p(rem_1432X, 30, 1);
    if (v_1433X) {
      n_1434X = s48_bignum_to_long(rem_1432X);
      arg0K0 = (((n_1434X)<<2));
      goto L17922;}
    else {
      v_1435X = enter_bignum(rem_1432X);
      arg0K0 = v_1435X;
      goto L17922;}}}
 L48540: {
  val_1436X = arg0K0;
  SvalS = val_1436X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L18285: {
  y_1437X = arg1K0;
  value_1438X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1438X))) {
    v_1439X = (char *) s48_long_to_bignum((((value_1438X)>>2)));
    arg1K0 = v_1439X;
    goto L18281;}
  else {
    arg1K0 = (((char *) (-3 + value_1438X)));
    goto L18281;}}
 L18211: {
  y_1440X = arg1K0;
  value_1441X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1441X))) {
    v_1442X = (char *) s48_long_to_bignum((((value_1441X)>>2)));
    arg1K0 = v_1442X;
    goto L18207;}
  else {
    arg1K0 = (((char *) (-3 + value_1441X)));
    goto L18207;}}
 L18137: {
  y_1443X = arg1K0;
  value_1444X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1444X))) {
    v_1445X = (char *) s48_long_to_bignum((((value_1444X)>>2)));
    arg1K0 = v_1445X;
    goto L18133;}
  else {
    arg1K0 = (((char *) (-3 + value_1444X)));
    goto L18133;}}
 L11401: {
  copies_1446X = arg0K0;
  if ((1 == copies_1446X)) {
    value_1447X = Sempty_logS;
    addr_1448X = (((char *) (-3 + proposal_658X))) + 4;
    S48_WRITE_BARRIER(proposal_658X, addr_1448X, value_1447X);
    *((long *) addr_1448X) = value_1447X;
    value_1449X = Sempty_logS;
    addr_1450X = (((char *) (-3 + proposal_658X))) + 8;
    S48_WRITE_BARRIER(proposal_658X, addr_1450X, value_1449X);
    *((long *) addr_1450X) = value_1449X;
    addr_1451X = (((char *) (-3 + proposal_658X))) + 12;
    S48_WRITE_BARRIER(proposal_658X, addr_1451X, 1);
    *((long *) addr_1451X) = 1;
    RELEASE_PROPOSAL_LOCK();
    x_1452X = Scurrent_threadS;
    addr_1453X = (((char *) (-3 + x_1452X))) + 12;
    S48_WRITE_BARRIER(x_1452X, addr_1453X, 1);
    *((long *) addr_1453X) = 1;
    SvalS = 5;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23193;}
  else {
    stob_1454X = *((long *) (((char *) (-3 + copies_1446X))));
    v_1455X = (((*((long *) ((((char *) (-3 + copies_1446X))) + 4))))>>2);
    stob_1456X = *((long *) ((((char *) (-3 + copies_1446X))) + 8));
    v_1457X = (((*((long *) ((((char *) (-3 + copies_1446X))) + 12))))>>2);
    v_1458X = (((*((long *) ((((char *) (-3 + copies_1446X))) + 16))))>>2);
    memcpy((void *)((((char *) (-3 + stob_1456X))) + v_1457X), (void *)((((char *) (-3 + stob_1454X))) + v_1455X),v_1458X);
    arg0K0 = (*((long *) ((((char *) (-3 + copies_1446X))) + 20)));
    goto L11401;}}
 L11518: {
  arg0K0 = (4 + i_1406X);
  goto L11486;}
 L30582: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L19343: {
  link_1459X = *((long *) ((((char *) (-3 + foo_1412X))) + 12));
  if ((0 == (3 & link_1459X))) {
    arg0K0 = foo_1412X;
    arg0K1 = (3 + (-4 & link_1459X));
    goto L19275;}
  else {
    arg0K0 = foo_1412X;
    arg0K1 = link_1459X;
    goto L19275;}}
 L14818: {
  if ((3 == (3 & thing_1041X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_1041X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L14824;}
    else {
      arg3K0 = "???";
      goto L14824;}}
  else {
    arg3K0 = "???";
    goto L14824;}}
 L21589: {
  StemplateS = (SvalS);
  Slosing_opcodeS = (0 - i_1196X);
  arg0K0 = arg_count_1298X;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L20919;}
 L21694: {
  ps_error("interrupt handler is not a closure", 1, i_1196X);
  goto L21589;}
 L18807: {
  x_1460X = arg1K0;
  external_bignum_1461X = (char *)s48_bignum_add(x_1460X, y_1420X);
  v_1462X = s48_bignum_fits_in_word_p(external_bignum_1461X, 30, 1);
  if (v_1462X) {
    n_1463X = s48_bignum_to_long(external_bignum_1461X);
    arg0K0 = (((n_1463X)<<2));
    goto L32047;}
  else {
    val_1464X = enter_bignum(external_bignum_1461X);
    arg0K0 = val_1464X;
    goto L32047;}}
 L18659: {
  x_1465X = arg1K0;
  external_bignum_1466X = (char *)s48_bignum_multiply(x_1465X, y_1423X);
  v_1467X = s48_bignum_fits_in_word_p(external_bignum_1466X, 30, 1);
  if (v_1467X) {
    n_1468X = s48_bignum_to_long(external_bignum_1466X);
    arg0K0 = (((n_1468X)<<2));
    goto L48540;}
  else {
    val_1469X = enter_bignum(external_bignum_1466X);
    arg0K0 = val_1469X;
    goto L48540;}}
 L18733: {
  x_1470X = arg1K0;
  external_bignum_1471X = (char *)s48_bignum_subtract(x_1470X, y_1426X);
  v_1472X = s48_bignum_fits_in_word_p(external_bignum_1471X, 30, 1);
  if (v_1472X) {
    n_1473X = s48_bignum_to_long(external_bignum_1471X);
    arg0K0 = (((n_1473X)<<2));
    goto L32336;}
  else {
    val_1474X = enter_bignum(external_bignum_1471X);
    arg0K0 = val_1474X;
    goto L32336;}}
 L40626: {
  div_by_zeroP_1475X = arg2K0;
  quot_1476X = arg0K1;
  rem_1477X = arg0K2;
  if (div_by_zeroP_1475X) {
    goto L40647;}
  else {
    if ((0 == (3 & rem_1477X))) {
      if ((0 == rem_1477X)) {
        SvalS = quot_1476X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23193;}
      else {
        goto L40647;}}
    else {
      goto L40647;}}}
 L17922: {
  v_1478X = arg0K0;
  v_1479X = s48_bignum_fits_in_word_p(quot_1431X, 30, 1);
  if (v_1479X) {
    n_1480X = s48_bignum_to_long(quot_1431X);
    arg2K0 = 0;
    arg0K1 = (((n_1480X)<<2));
    arg0K2 = v_1478X;
    goto L40626;}
  else {
    v_1481X = enter_bignum(quot_1431X);
    arg2K0 = 0;
    arg0K1 = v_1481X;
    arg0K2 = v_1478X;
    goto L40626;}}
 L18281: {
  x_1482X = arg1K0;
  external_bignum_1483X = (char *) s48_bignum_bitwise_and(x_1482X, y_1437X);
  v_1484X = s48_bignum_fits_in_word_p(external_bignum_1483X, 30, 1);
  if (v_1484X) {
    n_1485X = s48_bignum_to_long(external_bignum_1483X);
    arg0K0 = (((n_1485X)<<2));
    goto L34492;}
  else {
    val_1486X = enter_bignum(external_bignum_1483X);
    arg0K0 = val_1486X;
    goto L34492;}}
 L18207: {
  x_1487X = arg1K0;
  external_bignum_1488X = (char *) s48_bignum_bitwise_ior(x_1487X, y_1440X);
  v_1489X = s48_bignum_fits_in_word_p(external_bignum_1488X, 30, 1);
  if (v_1489X) {
    n_1490X = s48_bignum_to_long(external_bignum_1488X);
    arg0K0 = (((n_1490X)<<2));
    goto L34655;}
  else {
    val_1491X = enter_bignum(external_bignum_1488X);
    arg0K0 = val_1491X;
    goto L34655;}}
 L18133: {
  x_1492X = arg1K0;
  external_bignum_1493X = (char *) s48_bignum_bitwise_xor(x_1492X, y_1443X);
  v_1494X = s48_bignum_fits_in_word_p(external_bignum_1493X, 30, 1);
  if (v_1494X) {
    n_1495X = s48_bignum_to_long(external_bignum_1493X);
    arg0K0 = (((n_1495X)<<2));
    goto L34818;}
  else {
    val_1496X = enter_bignum(external_bignum_1493X);
    arg0K0 = val_1496X;
    goto L34818;}}
 L32047: {
  val_1497X = arg0K0;
  SvalS = val_1497X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L32336: {
  val_1498X = arg0K0;
  SvalS = val_1498X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L40647: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_482X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_483X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21289;}
 L34492: {
  val_1499X = arg0K0;
  SvalS = val_1499X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L34655: {
  val_1500X = arg0K0;
  SvalS = val_1500X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 L34818: {
  val_1501X = arg0K0;
  SvalS = val_1501X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23193;}
 loseD0: {
  message_387X = merged_arg3K0;{
  why_1502X = (((*((long *) ((SstackS) + (4 + (((nargs_785X)<<2)))))))>>2);
  ps_write_string("Template UIDs: ", (stderr));
  current_template_1503X = StemplateS;
  out_1504X = stderr;
  if ((0 == (3 & (*((long *) ((((char *) (-3 + current_template_1503X))) + 4)))))) {
    if ((current_template_1503X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
      arg2K0 = 0;
      goto L12983;}
    else {
      ps_write_integer(((((*((long *) ((((char *) (-3 + current_template_1503X))) + 4))))>>2)), out_1504X);
      arg2K0 = 1;
      goto L12983;}}
  else {
    arg2K0 = 0;
    goto L12983;}}
 L12983: {
  not_firstP_1505X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_1505X;
  goto L12990;}
 L12990: {
  cont_1506X = arg0K0;
  not_firstP_1507X = arg2K1;
  if ((3 == (3 & cont_1506X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_1506X))) + -4))))>>2))))) {
      template_1508X = *((long *) ((((char *) (-3 + cont_1506X))) + 8));
      if ((0 == (3 & (*((long *) ((((char *) (-3 + template_1508X))) + 4)))))) {
        if ((template_1508X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
          arg2K0 = not_firstP_1507X;
          goto L12999;}
        else {
          if (not_firstP_1507X) {
            ps_write_string(" <- ", out_1504X);
            goto L13038;}
          else {
            goto L13038;}}}
      else {
        arg2K0 = not_firstP_1507X;
        goto L12999;}}
    else {
      goto L21308;}}
  else {
    goto L21308;}}
 L12999: {
  v_1509X = arg2K0;
  arg0K0 = (*((long *) (((char *) (-3 + cont_1506X)))));
  arg2K1 = v_1509X;
  goto L12990;}
 L13038: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_1508X))) + 4))))>>2)), out_1504X);
  arg2K0 = 1;
  goto L12999;}
 L21308: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_1502X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_785X)<<2))))))))))))) {
      ps_error(message_387X, 3, opcode_786X, why_1502X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_785X)<<2)))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L21332;}}
  else {
    goto L21332;}}
 L21332: {
  ps_error(message_387X, 2, opcode_786X, why_1502X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L48549;}
 L48549: {
  type_1510X = s48_get_next_event(&channel_1511X, &status_1512X);
  if ((type_1510X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L20738;}
    else {
      arg0K0 = 1;
      goto L20738;}}
  else {
    if ((type_1510X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L20738;}
    else {
      if ((type_1510X == IO_COMPLETION_EVENT)) {
        enqueue_channelB(channel_1511X, status_1512X);
        arg0K0 = 8;
        goto L20738;}
      else {
        if ((type_1510X == OS_SIGNAL_EVENT)) {
          arg0K0 = 16;
          goto L20738;}
        else {
          if ((type_1510X == NO_EVENT)) {
            arg0K0 = 0;
            goto L20738;}
          else {
            if ((type_1510X == ERROR_EVENT)) {
              ps_write_string("OS error while getting event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              ps_write_string((ps_error_string(status_1512X)), (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L20738;}
            else {
              ps_write_string("unknown type of event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L20738;}}}}}}}
 L20738: {
  v_1513X = arg0K0;
  Spending_interruptsS = (v_1513X | (Spending_interruptsS));
  if ((type_1510X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L20765;}
      else {
        goto L20765;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L20765;}}
  else {
    goto L48549;}}
 L20765: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      goto L48549;}
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
    x_1514X = copy_listSAgc0_return_value;
    *((long *) (SstackS)) = x_1514X;
    SstackS = ((SstackS) + -4);
    goto rest_list_setupAgc_return;}
  else {
    if ((stack_arg_count_384X < wants_stack_args_383X)) {
      count_1515X = wants_stack_args_383X - stack_arg_count_384X;
      merged_arg0K0 = list_args_385X;
      merged_arg0K1 = count_1515X;
      push_list_return_tag = 4;
      goto push_list;
     push_list_return_4:
      v_1516X = push_list0_return_value;
      merged_arg0K0 = v_1516X;
      merged_arg0K1 = (list_arg_count_386X - count_1515X);
      copy_listSAgc_return_tag = 2;
      goto copy_listSAgc;
     copy_listSAgc_return_2:
      x_1517X = copy_listSAgc0_return_value;
      *((long *) (SstackS)) = x_1517X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setupAgc_return;}
    else {
      merged_arg0K0 = list_args_385X;
      merged_arg0K1 = list_arg_count_386X;
      copy_listSAgc_return_tag = 3;
      goto copy_listSAgc;
     copy_listSAgc_return_3:
      v_1518X = copy_listSAgc0_return_value;
      merged_arg0K0 = v_1518X;
      merged_arg0K1 = (stack_arg_count_384X - wants_stack_args_383X);
      pop_args_GlistSAgc_return_tag = 11;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_11:
      x_1519X = pop_args_GlistSAgc0_return_value;
      *((long *) (SstackS)) = x_1519X;
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
    goto L20582;}
  else {
    s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
    merged_arg0K0 = 0;
    copy_stack_into_heap_return_tag = 4;
    goto copy_stack_into_heap;
   copy_stack_into_heap_return_4:
    if ((count_382X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L20582;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L20582;}}}
 L20582: {
  SstackS = ((SstackS) + 4);
  list_1520X = *((long *) (SstackS));
  arg0K0 = count_382X;
  arg0K1 = list_1520X;
  goto L20591;}
 L20591: {
  i_1521X = arg0K0;
  l_1522X = arg0K1;
  if ((0 < i_1521X)) {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1522X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (-1 + i_1521X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1522X))) + 4)));
    goto L20591;}
  else {
    push_list0_return_value = l_1522X;
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
  goto L20156;}
 L20156: {
  p_1523X = arg1K0;
  i_1524X = arg0K1;
  if ((2 == (3 & (*((long *) p_1523X))))) {
    if ((26 == (*((long *) p_1523X)))) {
      arg0K0 = (-1 + i_1524X);
      goto L20136;}
    else {
      arg0K0 = i_1524X;
      goto L20136;}}
  else {
    arg1K0 = (p_1523X + 4);
    arg0K1 = (1 + i_1524X);
    goto L20156;}}
 L20136: {
  arg_count_1525X = arg0K0;
  top_1526X = SstackS;
  if ((1 == (ScontS))) {
    goto L20141;}
  else {
    merged_arg0K0 = key_380X;
    merged_arg0K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L20141;}}
 L20141: {
  SstackS = ((((char *) (-3 + (ScontS)))) + -8);
  arg1K0 = (SstackS);
  arg1K1 = (top_1526X + (((arg_count_1525X)<<2)));
  goto L20185;}
 L20185: {
  loc_1527X = arg1K0;
  arg_1528X = arg1K1;
  if ((top_1526X < arg_1528X)) {
    *((long *) loc_1527X) = (*((long *) arg_1528X));
    arg1K0 = (loc_1527X + -4);
    arg1K1 = (arg_1528X + -4);
    goto L20185;}
  else {
    SstackS = loc_1527X;
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
  p_1529X = SenvS;
  if ((3 == (3 & p_1529X))) {
    if ((p_1529X < (((long) (Sstack_beginS))))) {
      goto L19080;}
    else {
      if (((((long) (Sstack_endS))) < p_1529X)) {
        goto L19080;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_378X;
        merged_arg0K3 = reason_379X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_1530X = save_env_in_heap0_return_value;
        SenvS = v_1530X;
        goto L19080;}}}
  else {
    goto L19080;}}
 L19080: {
  end_1531X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L19033;}
 L19033: {
  cont_1532X = arg0K0;
  previous_1533X = arg0K1;
  if ((cont_1532X == (Sbottom_of_stackS))) {
    *((long *) (((char *) (-3 + previous_1533X)))) = end_1531X;
    ScontS = (Sbottom_of_stackS);
    really_preserve_continuation0_return_value = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
    goto really_preserve_continuation_return;}
  else {
    p_1534X = *((long *) ((((char *) (-3 + cont_1532X))) + 12));
    if ((3 == (3 & p_1534X))) {
      if ((p_1534X < (((long) (Sstack_beginS))))) {
        goto L19051;}
      else {
        if (((((long) (Sstack_endS))) < p_1534X)) {
          goto L19051;}
        else {
          merged_arg0K0 = (*((long *) ((((char *) (-3 + cont_1532X))) + 12)));
          merged_arg0K1 = cont_1532X;
          merged_arg0K2 = key_378X;
          merged_arg0K3 = reason_379X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L19051;}}}
    else {
      goto L19051;}}}
 L19051: {
  header_1535X = *((long *) ((((char *) (-3 + cont_1532X))) + -4));
  addr_1536X = s48_allocate_small((4 + ((long)(((unsigned long)header_1535X)>>8))));
  data_addr_1537X = addr_1536X + 4;
  *((long *) addr_1536X) = header_1535X;
  memcpy((void *)data_addr_1537X, (void *)(((char *) (-3 + cont_1532X))),((long)(((unsigned long)header_1535X)>>8)));
  new_1538X = 3 + (((long) data_addr_1537X));
  *((long *) (((char *) (-3 + previous_1533X)))) = new_1538X;
  arg0K0 = (*((long *) (((char *) (-3 + new_1538X)))));
  arg0K1 = new_1538X;
  goto L19033;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 save_env_in_heap: {
  env_376X = merged_arg0K0;
  cont_377X = merged_arg0K1;{
  header_1539X = *((long *) ((((char *) (-3 + env_376X))) + -4));
  addr_1540X = s48_allocate_small((4 + ((long)(((unsigned long)header_1539X)>>8))));
  data_addr_1541X = addr_1540X + 4;
  *((long *) addr_1540X) = header_1539X;
  memcpy((void *)data_addr_1541X, (void *)(((char *) (-3 + env_376X))),((long)(((unsigned long)header_1539X)>>8)));
  new_1542X = 3 + (((long) data_addr_1541X));
  addr_1543X = ((char *) (-3 + env_376X));
  S48_WRITE_BARRIER(env_376X, addr_1543X, 26);
  *((long *) addr_1543X) = 26;
  *((long *) ((((char *) (-3 + env_376X))) + -4)) = new_1542X;
  arg0K0 = new_1542X;
  goto L15993;}
 L15993: {
  env_1544X = arg0K0;
  p_1545X = *((long *) (((char *) (-3 + env_1544X))));
  if ((3 == (3 & p_1545X))) {
    if ((p_1545X < (((long) (Sstack_beginS))))) {
      goto L16204;}
    else {
      if (((((long) (Sstack_endS))) < p_1545X)) {
        goto L16204;}
      else {
        env_1546X = *((long *) (((char *) (-3 + env_1544X))));
        header_1547X = *((long *) ((((char *) (-3 + env_1546X))) + -4));
        addr_1548X = s48_allocate_small((4 + ((long)(((unsigned long)header_1547X)>>8))));
        data_addr_1549X = addr_1548X + 4;
        *((long *) addr_1548X) = header_1547X;
        memcpy((void *)data_addr_1549X, (void *)(((char *) (-3 + env_1546X))),((long)(((unsigned long)header_1547X)>>8)));
        new_1550X = 3 + (((long) data_addr_1549X));
        addr_1551X = ((char *) (-3 + env_1546X));
        S48_WRITE_BARRIER(env_1546X, addr_1551X, 26);
        *((long *) addr_1551X) = 26;
        *((long *) ((((char *) (-3 + env_1546X))) + -4)) = new_1550X;
        addr_1552X = ((char *) (-3 + env_1544X));
        S48_WRITE_BARRIER(env_1544X, addr_1552X, new_1550X);
        *((long *) addr_1552X) = new_1550X;
        arg0K0 = new_1550X;
        goto L15993;}}}
  else {
    goto L16204;}}
 L16204: {
  arg0K0 = cont_377X;
  goto L16216;}
 L16216: {
  cont_1553X = arg0K0;
  env_1554X = *((long *) ((((char *) (-3 + cont_1553X))) + 12));
  if ((3 == (3 & env_1554X))) {
    if ((3 == (3 & (*((long *) ((((char *) (-3 + env_1554X))) + -4)))))) {
      *((long *) ((((char *) (-3 + cont_1553X))) + 12)) = (*((long *) ((((char *) (-3 + env_1554X))) + -4)));
      arg0K0 = (*((long *) (((char *) (-3 + cont_1553X)))));
      goto L16216;}
    else {
      save_env_in_heap0_return_value = new_1542X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = new_1542X;
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
  value_1555X = Stemp0S;
  Stemp0S = 1;
  arg0K0 = value_1555X;
  arg0K1 = count_375X;
  goto L15535;}
 L15535: {
  args_1556X = arg0K0;
  count_1557X = arg0K1;
  if ((0 == count_1557X)) {
    pop_args_GlistSAgc0_return_value = args_1556X;
    goto pop_args_GlistSAgc_return;}
  else {
    SstackS = ((SstackS) + 4);
    a_1558X = *((long *) (SstackS));
    addr_1559X = s48_allocate_small(12);
    *((long *) addr_1559X) = 2050;
    x_1560X = 3 + (((long) (addr_1559X + 4)));
    *((long *) (((char *) (-3 + x_1560X)))) = a_1558X;
    *((long *) ((((char *) (-3 + x_1560X))) + 4)) = args_1556X;
    arg0K0 = x_1560X;
    arg0K1 = (-1 + count_1557X);
    goto L15535;}}
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
    value_1561X = Stemp0S;
    Stemp0S = 1;
    a_1562X = *((long *) (((char *) (-3 + value_1561X))));
    addr_1563X = s48_allocate_small(12);
    *((long *) addr_1563X) = 2050;
    x_1564X = 3 + (((long) (addr_1563X + 4)));
    *((long *) (((char *) (-3 + x_1564X)))) = a_1562X;
    *((long *) ((((char *) (-3 + x_1564X))) + 4)) = 25;
    arg0K0 = (*((long *) ((((char *) (-3 + value_1561X))) + 4)));
    arg0K1 = x_1564X;
    goto L15423;}}
 L15423: {
  l_1565X = arg0K0;
  last_1566X = arg0K1;
  if ((25 == l_1565X)) {
    copy_listSAgc0_return_value = x_1564X;
    goto copy_listSAgc_return;}
  else {
    a_1567X = *((long *) (((char *) (-3 + l_1565X))));
    addr_1568X = s48_allocate_small(12);
    *((long *) addr_1568X) = 2050;
    x_1569X = 3 + (((long) (addr_1568X + 4)));
    *((long *) (((char *) (-3 + x_1569X)))) = a_1567X;
    *((long *) ((((char *) (-3 + x_1569X))) + 4)) = 25;
    addr_1570X = (((char *) (-3 + last_1566X))) + 4;
    S48_WRITE_BARRIER(last_1566X, addr_1570X, x_1569X);
    *((long *) addr_1570X) = x_1569X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_1565X))) + 4)));
    arg0K1 = x_1569X;
    goto L15423;}}
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
    goto L16285;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_370X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16285;}}
 L16285: {
  x_size_1571X = arg0K0;
  extra_1572X = arg0K1;
  if ((n_371X < 0)) {
    if ((x_size_1571X < 1)) {
      arg0K0 = 1;
      goto L16330;}
    else {
      arg0K0 = x_size_1571X;
      goto L16330;}}
  else {
    n_1573X = n_371X / 30;
    arg0K0 = (5 + (x_size_1571X + n_1573X));
    goto L16305;}}
 L16330: {
  v_1574X = arg0K0;
  arg0K0 = (9 + ((((v_1574X)<<1)) + x_size_1571X));
  goto L16305;}
 L16305: {
  v_1575X = arg0K0;
  shift_space0_return_value = (extra_1572X + v_1575X);
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
        goto L15025;}}
    else {
      goto L15025;}}}
 L15025: {
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
  top_1576X = (((char *) (-3 + (Sbottom_of_stackS)))) + (-8 - (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_365X))) + -4))))>>8)))));
  new_cont_1577X = 3 + (((long) (top_1576X + 4)));
  if ((0 == stack_arg_count_366X)) {
    SstackS = (top_1576X + -4);
    goto L14215;}
  else {
    new_stack_1578X = top_1576X + (-4 - (((stack_arg_count_366X)<<2)));
    if ((new_stack_1578X < (SstackS))) {
      x_1579X = SstackS;
      memcpy((void *)(new_stack_1578X + 4), (void *)(x_1579X + 4),(((stack_arg_count_366X)<<2)));
      SstackS = new_stack_1578X;
      goto L14215;}
    else {
      goto L14215;}}}
 L14215: {
  ScontS = new_cont_1577X;
  v_1580X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_365X))) + -4))))>>8))))>>2);
  memcpy((void *)top_1576X, (void *)((((char *) (-3 + cont_365X))) + -4),(4 + (((v_1580X)<<2))));
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = (*((long *) (((char *) (-3 + new_cont_1577X)))));
  *((long *) (((char *) (-3 + new_cont_1577X)))) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_1577X;
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
  log_1581X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L13986;}
 L13986: {
  i_1582X = arg0K0;
  next_stob_1583X = *((long *) ((((char *) (-3 + log_1581X))) + (((i_1582X)<<2))));
  if ((1 == next_stob_1583X)) {
    add_log_entryAgc(1, i_1582X, stob_362X, index_363X, value_364X, 0);
    goto proposal_d_write_return;}
  else {
    if ((stob_362X == next_stob_1583X)) {
      if ((index_363X == (*((long *) ((((char *) (-3 + log_1581X))) + (4 + (((i_1582X)<<2)))))))) {
        addr_1584X = (((char *) (-3 + log_1581X))) + (8 + (((i_1582X)<<2)));
        S48_WRITE_BARRIER(log_1581X, addr_1584X, value_364X);
        *((long *) addr_1584X) = value_364X;
        goto proposal_d_write_return;}
      else {
        goto L14006;}}
    else {
      goto L14006;}}}
 L14006: {
  arg0K0 = (4 + i_1582X);
  goto L13986;}
 proposal_d_write_return:
  switch (proposal_d_write_return_tag) {
  case 0: goto proposal_d_write_return_0;
  case 1: goto proposal_d_write_return_1;
  default: goto proposal_d_write_return_2;
  }}

 proposal_d_read: {
  stob_360X = merged_arg0K0;
  index_361X = merged_arg0K1;{
  log_1585X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L13809;}
 L13809: {
  i_1586X = arg0K0;
  next_stob_1587X = *((long *) ((((char *) (-3 + log_1585X))) + (((i_1586X)<<2))));
  if ((1 == next_stob_1587X)) {
    v_1588X = add_log_entryAgc(1, i_1586X, stob_360X, index_361X, (*((long *) ((((char *) (-3 + stob_360X))) + (-4 & index_361X)))), 1);
    proposal_d_read0_return_value = v_1588X;
    goto proposal_d_read_return;}
  else {
    if ((stob_360X == next_stob_1587X)) {
      if ((index_361X == (*((long *) ((((char *) (-3 + log_1585X))) + (4 + (((i_1586X)<<2)))))))) {
        proposal_d_read0_return_value = (*((long *) ((((char *) (-3 + log_1585X))) + (8 + (((i_1586X)<<2))))));
        goto proposal_d_read_return;}
      else {
        goto L13831;}}
    else {
      goto L13831;}}}
 L13831: {
  arg0K0 = (4 + i_1586X);
  goto L13809;}
 proposal_d_read_return:
  switch (proposal_d_read_return_tag) {
  case 0: goto proposal_d_read_return_0;
  case 1: goto proposal_d_read_return_1;
  default: goto proposal_d_read_return_2;
  }}

 make_closure: {
  a_358X = merged_arg0K0;
  b_359X = merged_arg0K1;{
  addr_1589X = s48_allocate_small(12);
  *((long *) addr_1589X) = 2062;
  x_1590X = 3 + (((long) (addr_1589X + 4)));
  *((long *) (((char *) (-3 + x_1590X)))) = a_358X;
  *((long *) ((((char *) (-3 + x_1590X))) + 4)) = b_359X;
  if ((3 == (3 & x_1590X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_1590X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_1590X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_1590X))) + -4))));
      make_closure0_return_value = x_1590X;
      goto make_closure_return;}
    else {
      make_closure0_return_value = x_1590X;
      goto make_closure_return;}}
  else {
    make_closure0_return_value = x_1590X;
    goto make_closure_return;}}
 make_closure_return:
  switch (make_closure_return_tag) {
  case 0: goto make_closure_return_0;
  default: goto make_closure_return_1;
  }}

 get_current_port: {
  marker_357X = merged_arg0K0;{
  thread_1591X = Scurrent_threadS;
  if ((3 == (3 & thread_1591X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thread_1591X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_1591X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_1591X))) + 4)));
        goto L12440;}
      else {
        goto L12490;}}
    else {
      goto L12490;}}
  else {
    goto L12490;}}
 L12440: {
  env_1592X = arg0K0;
  if ((3 == (3 & env_1592X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_1592X))) + -4))))>>2))))) {
      obj_1593X = *((long *) (((char *) (-3 + env_1592X))));
      if ((3 == (3 & obj_1593X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_1593X))) + -4))))>>2))))) {
          if ((marker_357X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_1592X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_1592X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_1592X))) + 4)));
            goto L12440;}}
        else {
          goto L12512;}}
      else {
        goto L12512;}}
    else {
      goto L12512;}}
  else {
    goto L12512;}}
 L12490: {
  ps_error("current thread is not a record", 0);
  get_current_port0_return_value = v_1594X;
  goto get_current_port_return;}
 L12512: {
  if ((25 == env_1592X)) {
    if (((((marker_357X)>>2)) == 1)) {
      arg3K0 = "dynamic environment doesn't have current-output-port";
      goto L12466;}
    else {
      arg3K0 = "dynamic environment doesn't have current-input-port";
      goto L12466;}}
  else {
    ps_error("dynamic environment is not a proper list", 0);
    get_current_port0_return_value = v_1595X;
    goto get_current_port_return;}}
 L12466: {
  v_1596X = arg3K0;
  ps_error(v_1596X, 0);
  get_current_port0_return_value = v_1597X;
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
  goto L12307;}
 L12307: {
  fast_1598X = arg0K0;
  len_1599X = arg0K1;
  slow_1600X = arg0K2;
  move_slowP_1601X = arg2K3;
  if ((25 == fast_1598X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_1599X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_1598X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_1598X))) + -4))))>>2))))) {
        if (move_slowP_1601X) {
          if ((fast_1598X == slow_1600X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_1598X))) + 4)));
            arg0K1 = (1 + len_1599X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_1600X))) + 4)));
            arg2K3 = 0;
            goto L12307;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_1598X))) + 4)));
          arg0K1 = (1 + len_1599X);
          arg0K2 = slow_1600X;
          arg2K3 = 1;
          goto L12307;}}
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
  string_1602X = ps_error_string(status_355X);
  x_1603X = strlen((char *) string_1602X);
  if ((x_1603X < 256)) {
    arg0K0 = x_1603X;
    goto L11699;}
  else {
    arg0K0 = 256;
    goto L11699;}}
 L11699: {
  len_1604X = arg0K0;
  len_1605X = 1 + len_1604X;
  addr_1606X = s48_allocate_small((4 + len_1605X));
  *((long *) addr_1606X) = (70 + (((len_1605X)<<8)));
  string_1607X = 3 + (((long) (addr_1606X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1607X))) + len_1604X)) = 0;
  arg0K0 = 0;
  goto L11709;}
 L11709: {
  i_1608X = arg0K0;
  if ((i_1608X == len_1604X)) {
    get_error_string0_return_value = string_1607X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1607X))) + i_1608X)) = ((*(string_1602X + i_1608X)));
    arg0K0 = (1 + i_1608X);
    goto L11709;}}
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
{ cont_1609X = ScontS;
  tem_1610X = *((long *) ((((char *) (-3 + cont_1609X))) + 8));
  pc_1611X = *((long *) ((((char *) (-3 + cont_1609X))) + 4));
  StemplateS = tem_1610X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1610X)))))))) + (((pc_1611X)>>2)));
  SenvS = (*((long *) ((((char *) (-3 + cont_1609X))) + 12)));
  ScontS = (*((long *) (((char *) (-3 + cont_1609X)))));
  SstackS = ((((char *) (-3 + cont_1609X))) + 12);
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
long s48_call_startup_procedure(char **startup_vector_1612X, long startup_vector_length_1613X)
{
  long arg0K0;
  long merged_arg0K1;
  char *merged_arg3K0;

  int enter_string_return_tag;
  long enter_string0_return_value;
  char *string_1614X;
  long i_1654X;
  long string_1653X;
  char * addr_1652X;
  long len_1651X;
  long len_1650X;
  long v_1649X;
  long x_1648X;
  long x_1647X;
  char * addr_1646X;
  long b_1645X;
  long channel_1644X;
  long x_1643X;
  char * addr_1642X;
  long b_1641X;
  long channel_1640X;
  long x_1639X;
  char * addr_1638X;
  long b_1637X;
  long channel_1636X;
  long i_1635X;
  long length_1634X;
  long *v_1633X;
  long v_1632X;
  long v_1631X;
  long y_1630X;
  long x_1629X;
  long v_1628X;
  long x_1627X;
  long y_1626X;
  char * addr_1625X;
  long value_1624X;
  long x_1623X;
  long y_1622X;
  long i_1621X;
  long vector_1620X;
  long v_1619X;
  long vector_1618X;
  char * addr_1617X;
  long len_1616X;
  long tem_1615X;
 {  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  SenvS = 13;
  tem_1615X = Sinterrupt_templateS;
  StemplateS = tem_1615X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1615X))))))));
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
  len_1616X = ((startup_vector_length_1613X)<<2);
  addr_1617X = s48_allocate_tracedAgc((4 + len_1616X));
  if ((addr_1617X == NULL)) {
    arg0K0 = 1;
    goto L15937;}
  else {
    *((long *) addr_1617X) = (10 + (((len_1616X)<<8)));
    arg0K0 = (3 + (((long) (addr_1617X + 4))));
    goto L15937;}}
 L15937: {
  vector_1618X = arg0K0;
  if ((1 == vector_1618X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1619X;
    goto L15923;}
  else {
    arg0K0 = vector_1618X;
    goto L15923;}}
 L15923: {
  vector_1620X = arg0K0;
  arg0K0 = 0;
  goto L15960;}
 L15960: {
  i_1621X = arg0K0;
  if ((i_1621X == startup_vector_length_1613X)) {
    *((long *) (SstackS)) = vector_1620X;
    SstackS = ((SstackS) + -4);
    y_1622X = fileno((stderr));
    x_1623X = fileno((stdout));
    if ((x_1623X < y_1622X)) {
      arg0K0 = y_1622X;
      goto L17647;}
    else {
      arg0K0 = x_1623X;
      goto L17647;}}
  else {
    value_1624X = enter_stringAgc((*(startup_vector_1612X + i_1621X)));
    addr_1625X = (((char *) (-3 + vector_1620X))) + (((i_1621X)<<2));
    S48_WRITE_BARRIER(vector_1620X, addr_1625X, value_1624X);
    *((long *) addr_1625X) = value_1624X;
    arg0K0 = (1 + i_1621X);
    goto L15960;}}
 L17647: {
  y_1626X = arg0K0;
  x_1627X = fileno((stdin));
  if ((x_1627X < y_1626X)) {
    arg0K0 = y_1626X;
    goto L17649;}
  else {
    arg0K0 = x_1627X;
    goto L17649;}}
 L17649: {
  v_1628X = arg0K0;
  x_1629X = Snumber_of_channelsS;
  y_1630X = 1 + v_1628X;
  if ((x_1629X < y_1630X)) {
    arg0K0 = y_1630X;
    goto L17651;}
  else {
    arg0K0 = x_1629X;
    goto L17651;}}
 L17651: {
  v_1631X = arg0K0;
  Snumber_of_channelsS = v_1631X;
  v_1632X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L17673;}
  else {
    goto L17673;}}
 L17673: {
  v_1633X = Svm_channelsS;
  length_1634X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L17721;}
 L17721: {
  i_1635X = arg0K0;
  if ((i_1635X < length_1634X)) {
    *(v_1633X + i_1635X) = 1;
    arg0K0 = (1 + i_1635X);
    goto L17721;}
  else {
    s48_make_availableAgc(((((3 * (7 + ((((4 + (strlen((char *) "standard output"))))>>2)))))<<2)));
    channel_1636X = fileno((stdin));
    merged_arg3K0 = "standard input";
    merged_arg0K1 = 0;
    enter_string_return_tag = 0;
    goto enter_string;
   enter_string_return_0:
    b_1637X = enter_string0_return_value;
    addr_1638X = s48_allocate_small(24);
    *((long *) addr_1638X) = 5146;
    x_1639X = 3 + (((long) (addr_1638X + 4)));
    *((long *) (((char *) (-3 + x_1639X)))) = 4;
    *((long *) ((((char *) (-3 + x_1639X))) + 4)) = b_1637X;
    *((long *) ((((char *) (-3 + x_1639X))) + 8)) = (((channel_1636X)<<2));
    *((long *) ((((char *) (-3 + x_1639X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1639X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1636X) = x_1639X;
    channel_1640X = fileno((stderr));
    merged_arg3K0 = "standard error";
    merged_arg0K1 = 0;
    enter_string_return_tag = 1;
    goto enter_string;
   enter_string_return_1:
    b_1641X = enter_string0_return_value;
    addr_1642X = s48_allocate_small(24);
    *((long *) addr_1642X) = 5146;
    x_1643X = 3 + (((long) (addr_1642X + 4)));
    *((long *) (((char *) (-3 + x_1643X)))) = 8;
    *((long *) ((((char *) (-3 + x_1643X))) + 4)) = b_1641X;
    *((long *) ((((char *) (-3 + x_1643X))) + 8)) = (((channel_1640X)<<2));
    *((long *) ((((char *) (-3 + x_1643X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1643X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1640X) = x_1643X;
    channel_1644X = fileno((stdout));
    merged_arg3K0 = "standard output";
    merged_arg0K1 = 0;
    enter_string_return_tag = 2;
    goto enter_string;
   enter_string_return_2:
    b_1645X = enter_string0_return_value;
    addr_1646X = s48_allocate_small(24);
    *((long *) addr_1646X) = 5146;
    x_1647X = 3 + (((long) (addr_1646X + 4)));
    *((long *) (((char *) (-3 + x_1647X)))) = 8;
    *((long *) ((((char *) (-3 + x_1647X))) + 4)) = b_1645X;
    *((long *) ((((char *) (-3 + x_1647X))) + 8)) = (((channel_1644X)<<2));
    *((long *) ((((char *) (-3 + x_1647X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1647X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1644X) = x_1647X;
    *((long *) (SstackS)) = x_1639X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1647X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1643X;
    SstackS = ((SstackS) + -4);
    x_1648X = s48_resumer_records();
    *((long *) (SstackS)) = x_1648X;
    SstackS = ((SstackS) + -4);
    s48_initialization_completeB();
    v_1649X = s48_startup_procedure();
    return s48_restart(v_1649X, 5);}}
 enter_string: {
  string_1614X = merged_arg3K0;{
  len_1650X = strlen((char *) string_1614X);
  len_1651X = 1 + len_1650X;
  addr_1652X = s48_allocate_small((4 + len_1651X));
  *((long *) addr_1652X) = (70 + (((len_1651X)<<8)));
  string_1653X = 3 + (((long) (addr_1652X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1653X))) + len_1650X)) = 0;
  arg0K0 = 0;
  goto L13648;}
 L13648: {
  i_1654X = arg0K0;
  if ((i_1654X == len_1650X)) {
    enter_string0_return_value = string_1653X;
    goto enter_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1653X))) + i_1654X)) = ((*(string_1614X + i_1654X)));
    arg0K0 = (1 + i_1654X);
    goto L13648;}}
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
Spost_gc_cleanupS = HtopD30352;
Sgc_root_procS = HtopD30363;
s48_Scallback_return_stack_blockS = 1;
s48_Spending_eventsPS = 0;
}
