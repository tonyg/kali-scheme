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
    goto L2914;}
  else {
    arg0K0 = x_5X;
    goto L2914;}}
 L2914: {
  new_count_7X = arg0K0;
  new_vm_channels_8X = (long*)malloc(sizeof(long) * new_count_7X);
  if ((NULL == new_vm_channels_8X)) {
    return 0;}
  else {
    arg0K0 = 0;
    goto L2926;}}
 L2926: {
  i_9X = arg0K0;
  if ((i_9X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L2999;}
  else {
    *(new_vm_channels_8X + i_9X) = (*((Svm_channelsS) + i_9X));
    arg0K0 = (1 + i_9X);
    goto L2926;}}
 L2999: {
  i_10X = arg0K0;
  if ((i_10X == new_count_7X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_8X;
    Snumber_of_channelsS = new_count_7X;
    return 1;}
  else {
    *(new_vm_channels_8X + i_10X) = 1;
    arg0K0 = (1 + i_10X);
    goto L2999;}}
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
    goto L10644;}
  else {
    goto L10644;}}
 L10644: {
  v_22X = *((long *) (((char *) (-3 + channel_18X))));
  if ((4 == v_22X)) {
    goto L10659;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_18X))))))) {
      goto L10659;}
    else {
      v_23X = ps_close_fd(os_index_19X);
      arg0K0 = v_23X;
      goto L10666;}}}
 L10659: {
  v_24X = ps_close_fd(os_index_19X);
  arg0K0 = v_24X;
  goto L10666;}
 L10666: {
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
  goto L12409;}
 L12409: {
  p_36X = arg1K0;
  i_37X = arg0K1;
  if ((2 == (3 & (*((long *) p_36X))))) {
    if ((26 == (*((long *) p_36X)))) {
      arg0K0 = (-1 + i_37X);
      goto L12350;}
    else {
      arg0K0 = i_37X;
      goto L12350;}}
  else {
    arg1K0 = (p_36X + 4);
    arg0K1 = (1 + i_37X);
    goto L12409;}}
 L12350: {
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
      goto L9869;}
    else {
      arg0K0 = (-1 + (((log_size_46X)<<1)));
      goto L9869;}}
  else {
    arg0K0 = proposal_45X;
    arg0K1 = stob_41X;
    arg0K2 = value_43X;
    goto L12519;}}
 L9869: {
  new_size_47X = arg0K0;
  len_48X = ((new_size_47X)<<2);
  addr_49X = s48_allocate_tracedAgc((4 + len_48X));
  if ((addr_49X == NULL)) {
    arg0K0 = 1;
    goto L9915;}
  else {
    *((long *) addr_49X) = (10 + (((len_48X)<<8)));
    arg0K0 = (3 + (((long) (addr_49X + 4))));
    goto L9915;}}
 L12519: {
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
    goto L12540;}
  else {
    arg0K0 = 29;
    goto L12540;}}
 L9915: {
  vector_57X = arg0K0;
  if ((1 == vector_57X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_58X;
    goto L9873;}
  else {
    arg0K0 = vector_57X;
    goto L9873;}}
 L12540: {
  value_59X = arg0K0;
  addr_60X = (((char *) (-3 + log_53X))) + (12 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_60X, value_59X);
  *((long *) addr_60X) = value_59X;
  addr_61X = (((char *) (-3 + log_53X))) + (16 + (((i_40X)<<2)));
  S48_WRITE_BARRIER(log_53X, addr_61X, 1);
  *((long *) addr_61X) = 1;
  return value_52X;}
 L9873: {
  new_62X = arg0K0;
  proposal_63X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 < log_size_46X)) {
    stob_64X = *((long *) ((((char *) (-3 + proposal_63X))) + 4));
    memcpy((void *)(((char *) (-3 + new_62X))), (void *)(((char *) (-3 + stob_64X))),(-4 + (((log_size_46X)<<2))));
    goto L9893;}
  else {
    goto L9893;}}
 L9893: {
  arg0K0 = (4 + log_size_46X);
  goto L9897;}
 L9897: {
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
    goto L12519;}
  else {
    addr_70X = (((char *) (-3 + new_62X))) + (((i_65X)<<2));
    S48_WRITE_BARRIER(new_62X, addr_70X, 0);
    *((long *) addr_70X) = 0;
    arg0K0 = (1 + i_65X);
    goto L9897;}}
}
static long enter_bignum(char * external_bignum_71X)
{
  long desc_72X;
 {  desc_72X = 3 + (((long) external_bignum_71X));
  if ((3 == (3 & desc_72X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + desc_72X))) + -4))))>>2))))) {
      goto L13153;}
    else {
      goto L13167;}}
  else {
    goto L13167;}}
 L13153: {
  if ((3 == (3 & desc_72X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + desc_72X))) + -4)))))) {
      *((long *) ((((char *) (-3 + desc_72X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + desc_72X))) + -4))));
      return desc_72X;}
    else {
      return desc_72X;}}
  else {
    return desc_72X;}}
 L13167: {
  ps_error("not a bignum", 1, desc_72X);
  goto L13153;}
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
  goto L14089;}
 L14089: {
  i_78X = arg0K0;
  if ((i_78X == z_74X)) {
    return string_77X;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_77X))) + i_78X)) = ((*(string_73X + i_78X)));
    arg0K0 = (1 + i_78X);
    goto L14089;}}
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
      goto L15064;}
    else {
      goto L15058;}}
  else {
    goto L15058;}}
 L15064: {
  PS_WRITE_CHAR(10, (stderr), v_82X)
  return v_82X;}
 L15058: {
  ps_write_integer((((index_80X)>>2)), (stderr));
  goto L15064;}
}
static long make_registered_channel(long mode_83X, long id_84X, long os_index_85X, long key_86X, long *TT0)
{
  long x_90X;
  char * addr_89X;
  char x_88X;
  char temp_87X;
 {  temp_87X = os_index_85X < (Snumber_of_channelsS);
  if (temp_87X) {
    goto L16116;}
  else {
    x_88X = add_more_channels(os_index_85X);
    if (x_88X) {
      goto L16116;}
    else {
      *TT0 = 9;
      return 1;}}}
 L16116: {
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
  goto L19983;}
 L19983: {
  i_95X = arg0K0;
  h_96X = arg0K1;
  if ((i_95X < n_94X)) {
    arg0K0 = (1 + i_95X);
    arg0K1 = (h_96X + (((*((unsigned char *) ((((char *) (-3 + string_92X))) + i_95X))))));
    goto L19983;}
  else {
    index_97X = 1023 & h_96X;
    link_98X = *((long *) ((((char *) (-3 + table_91X))) + (((index_97X)<<2))));
    if ((0 == (3 & link_98X))) {
      arg0K0 = (3 + (-4 & link_98X));
      goto L19939;}
    else {
      arg0K0 = link_98X;
      goto L19939;}}}
 L19939: {
  bucket_99X = arg0K0;
  arg0K0 = bucket_99X;
  goto L19945;}
 L19945: {
  foo_100X = arg0K0;
  if ((1 == foo_100X)) {
    if ((3 == (3 & bucket_99X))) {
      arg0K0 = (-4 & bucket_99X);
      goto L19950;}
    else {
      arg0K0 = bucket_99X;
      goto L19950;}}
  else {
    s2_101X = *((long *) (((char *) (-3 + foo_100X))));
    len_102X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_92X))) + -4))))>>8);
    if ((len_102X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_101X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_101X))), (void *)(((char *) (-3 + string_92X))),len_102X)))) {
        return foo_100X;}
      else {
        goto L19965;}}
    else {
      goto L19965;}}}
 L19950: {
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
    goto L19956;}
  else {
    arg0K0 = x_105X;
    goto L19956;}}
 L19965: {
  link_106X = *((long *) ((((char *) (-3 + foo_100X))) + 12));
  if ((0 == (3 & link_106X))) {
    arg0K0 = (3 + (-4 & link_106X));
    goto L19945;}
  else {
    arg0K0 = link_106X;
    goto L19945;}}
 L19956: {
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
  goto L20135;}
 L20135: {
  i_113X = arg0K0;
  h_114X = arg0K1;
  if ((i_113X < n_112X)) {
    arg0K0 = (1 + i_113X);
    arg0K1 = (h_114X + (((*((unsigned char *) ((((char *) (-3 + string_110X))) + i_113X))))));
    goto L20135;}
  else {
    index_115X = 1023 & h_114X;
    link_116X = *((long *) ((((char *) (-3 + table_109X))) + (((index_115X)<<2))));
    if ((0 == (3 & link_116X))) {
      arg0K0 = (3 + (-4 & link_116X));
      goto L20091;}
    else {
      arg0K0 = link_116X;
      goto L20091;}}}
 L20091: {
  bucket_117X = arg0K0;
  arg0K0 = bucket_117X;
  goto L20097;}
 L20097: {
  foo_118X = arg0K0;
  if ((1 == foo_118X)) {
    if ((3 == (3 & bucket_117X))) {
      arg0K0 = (-4 & bucket_117X);
      goto L20102;}
    else {
      arg0K0 = bucket_117X;
      goto L20102;}}
  else {
    s2_119X = *((long *) (((char *) (-3 + foo_118X))));
    len_120X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_110X))) + -4))))>>8);
    if ((len_120X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_119X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_119X))), (void *)(((char *) (-3 + string_110X))),len_120X)))) {
        return foo_118X;}
      else {
        goto L20117;}}
    else {
      goto L20117;}}}
 L20102: {
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
    goto L20108;}
  else {
    arg0K0 = x_123X;
    goto L20108;}}
 L20117: {
  link_124X = *((long *) ((((char *) (-3 + foo_118X))) + 12));
  if ((0 == (3 & link_124X))) {
    arg0K0 = (3 + (-4 & link_124X));
    goto L20097;}
  else {
    arg0K0 = link_124X;
    goto L20097;}}
 L20108: {
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
  goto L20768;}
 L20768: {
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
    goto L10258;}
  else {
    foo_link_132X = *((long *) ((((char *) (-3 + table_127X))) + (((i_128X)<<2))));
    arg0K0 = foo_link_132X;
    arg0K1 = 1;
    goto L13611;}}
 L10258: {
  alist_133X = arg0K0;
  okay_134X = arg0K1;
  goners_135X = arg0K2;
  if ((25 == alist_133X)) {
    SHARED_SETB((Sfinalizer_alistS), okay_134X);
    l2_136X = Sfinalize_theseS;
    if ((25 == goners_135X)) {
      arg0K0 = l2_136X;
      goto L10265;}
    else {
      arg0K0 = goners_135X;
      goto L10320;}}
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
      goto L10258;}
    else {
      addr_146X = (((char *) (-3 + alist_137X))) + 4;
      S48_WRITE_BARRIER(alist_137X, addr_146X, goners_135X);
      *((long *) addr_146X) = goners_135X;
      arg0K0 = next_140X;
      arg0K1 = okay_134X;
      arg0K2 = alist_137X;
      goto L10258;}}}
 L13611: {
  foo_link_147X = arg0K0;
  okay_link_148X = arg0K1;
  if ((0 == (3 & foo_link_147X))) {
    arg0K0 = (3 + (-4 & foo_link_147X));
    goto L13613;}
  else {
    arg0K0 = foo_link_147X;
    goto L13613;}}
 L10265: {
  v_149X = arg0K0;
  Sfinalize_theseS = v_149X;
  arg0K0 = 0;
  goto L19358;}
 L10320: {
  l_150X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_150X))) + 4))))) {
    addr_151X = (((char *) (-3 + l_150X))) + 4;
    S48_WRITE_BARRIER(l_150X, addr_151X, l2_136X);
    *((long *) addr_151X) = l2_136X;
    arg0K0 = goners_135X;
    goto L10265;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_150X))) + 4)));
    goto L10320;}}
 L13613: {
  foo_152X = arg0K0;
  if ((1 == foo_152X)) {
    addr_153X = (((char *) (-3 + table_127X))) + (((i_128X)<<2));
    S48_WRITE_BARRIER(table_127X, addr_153X, okay_link_148X);
    *((long *) addr_153X) = okay_link_148X;
    arg0K0 = (1 + i_128X);
    goto L20768;}
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
        goto L13611;}
      else {
        arg0K0 = next_link_156X;
        arg0K1 = new_foo_155X;
        goto L13611;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + foo_152X))) + 4)));
      arg0K1 = okay_link_148X;
      goto L13611;}}}
 L19358: {
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
      goto L19392;}
    else {
      v_160X = s48_extantP(channel_159X);
      if (v_160X) {
        v_161X = s48_trace_value(channel_159X);
        arg0K0 = v_161X;
        goto L19385;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_159X))))))) {
          arg0K0 = 1;
          goto L19385;}
        else {
          id_162X = *((long *) ((((char *) (-3 + channel_159X))) + 4));
          status_163X = close_channelB(channel_159X);
          if ((status_163X == NO_ERRORS)) {
            goto L17384;}
          else {
            channel_close_error(status_163X, (*((long *) ((((char *) (-3 + channel_159X))) + 8))), id_162X);
            goto L17384;}}}}}}
 L19392: {
  arg0K0 = (1 + i_158X);
  goto L19358;}
 L19385: {
  new_164X = arg0K0;
  *((Svm_channelsS) + i_158X) = new_164X;
  goto L19392;}
 L17384: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_162X))) {
    ps_write_integer((((id_162X)>>2)), (stderr));
    goto L17402;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_162X))))), (stderr));
    goto L17402;}}
 L17402: {
  ps_write_string(" ", (stderr));
  ps_write_integer(((((*((long *) ((((char *) (-3 + channel_159X))) + 8))))>>2)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L19385;}
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
  goto L6912;}
 L6912: {
  frame_168X = arg1K0;
  if ((frame_168X == NULL)) {
    arg1K0 = (Spermanent_external_rootsS);
    goto L6938;}
  else {
    length_169X = *((long *) frame_168X);
    arg0K0 = 0;
    goto L6920;}}
 L6938: {
  frame_170X = arg1K0;
  if ((frame_170X == NULL)) {
    s48_initializing_gc_root();
    table_171X = s48_trace_value((Simported_bindingsS));
    arg0K0 = 0;
    goto L17667;}
  else {
    cell_172X = ((char *) (*((long *) (frame_170X + 4))));
    x2_173X = s48_trace_value((*((long *) cell_172X)));
    *((long *) cell_172X) = x2_173X;
    arg1K0 = (((char *) (*((long *) frame_170X))));
    goto L6938;}}
 L6920: {
  i_174X = arg0K0;
  if ((i_174X == length_169X)) {
    arg1K0 = (((char *) (*((long *) (frame_168X + 4)))));
    goto L6912;}
  else {
    cell_175X = ((char *) (*((long *) (frame_168X + (8 + (((i_174X)<<2)))))));
    x2_176X = s48_trace_value((*((long *) cell_175X)));
    *((long *) cell_175X) = x2_176X;
    arg0K0 = (1 + i_174X);
    goto L6920;}}
 L17667: {
  i_177X = arg0K0;
  if ((1024 == i_177X)) {
    Simported_bindingsS = table_171X;
    table_178X = s48_trace_value((Sexported_bindingsS));
    arg0K0 = 0;
    goto L17688;}
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
    goto L17667;}}
 L17688: {
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
    goto L6976;}
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
    goto L17688;}}
 L6976: {
  alist_201X = arg0K0;
  if ((25 == alist_201X)) {
    env_202X = s48_trace_value((SenvS));
    SenvS = env_202X;
    v_203X = s48_trace_value((Spending_channels_headS));
    Spending_channels_headS = v_203X;
    v_204X = s48_trace_value((Spending_channels_tailS));
    Spending_channels_tailS = v_204X;
    arg0K0 = 0;
    goto L10390;}
  else {
    pair_205X = *((long *) (((char *) (-3 + alist_201X))));
    x_206X = s48_extantP((*((long *) (((char *) (-3 + pair_205X))))));
    if (x_206X) {
      goto L7001;}
    else {
      s48_trace_stob_contentsB((*((long *) (((char *) (-3 + pair_205X))))));
      goto L7001;}}}
 L10390: {
  i_207X = arg0K0;
  if ((i_207X == (Snumber_of_channelsS))) {
    if ((Sstack_warningPS)) {
      arg1K0 = (Sstack_beginS);
      goto L7524;}
    else {
      goto L7557;}}
  else {
    channel_208X = *((Svm_channelsS) + i_207X);
    if ((1 == channel_208X)) {
      goto L10418;}
    else {
      if ((0 == (*((long *) (((char *) (-3 + channel_208X))))))) {
        goto L10418;}
      else {
        val_209X = s48_trace_value((*((long *) ((((char *) (-3 + channel_208X))) + 4))));
        addr_210X = (((char *) (-3 + channel_208X))) + 4;
        S48_WRITE_BARRIER(channel_208X, addr_210X, val_209X);
        *((long *) addr_210X) = val_209X;
        goto L10418;}}}}
 L7001: {
  val_211X = s48_trace_value((*((long *) ((((char *) (-3 + pair_205X))) + 4))));
  addr_212X = (((char *) (-3 + pair_205X))) + 4;
  S48_WRITE_BARRIER(pair_205X, addr_212X, val_211X);
  *((long *) addr_212X) = val_211X;
  arg0K0 = (*((long *) ((((char *) (-3 + alist_201X))) + 4)));
  goto L6976;}
 L7524: {
  a_213X = arg1K0;
  if ((252645135 == (*((long *) a_213X)))) {
    arg1K0 = (a_213X + 4);
    goto L7524;}
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
      goto L7557;}
    else {
      goto L7557;}}}
 L7557: {
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L7622;}
 L10418: {
  arg0K0 = (1 + i_207X);
  goto L10390;}
 L7622: {
  p_215X = arg1K0;
  i_216X = arg0K1;
  if ((2 == (3 & (*((long *) p_215X))))) {
    if ((26 == (*((long *) p_215X)))) {
      arg0K0 = (-1 + i_216X);
      goto L7559;}
    else {
      arg0K0 = i_216X;
      goto L7559;}}
  else {
    arg1K0 = (p_215X + 4);
    arg0K1 = (1 + i_216X);
    goto L7622;}}
 L7559: {
  arg_count_217X = arg0K0;
  s48_trace_locationsB(((SstackS) + 4), ((SstackS) + (4 + (((arg_count_217X)<<2)))));
  p_218X = SenvS;
  if ((3 == (3 & p_218X))) {
    if ((p_218X < (((long) (Sstack_beginS))))) {
      goto L7575;}
    else {
      if (((((long) (Sstack_endS))) < p_218X)) {
        goto L7575;}
      else {
        env_219X = SenvS;
        arg0K0 = env_219X;
        goto L7735;}}}
  else {
    goto L7575;}}
 L7575: {
  v_220X = s48_trace_value((SenvS));
  SenvS = v_220X;
  goto L7580;}
 L7735: {
  env_221X = arg0K0;
  if ((3 == (3 & env_221X))) {
    if ((env_221X < (((long) (Sstack_beginS))))) {
      goto L7580;}
    else {
      if (((((long) (Sstack_endS))) < env_221X)) {
        goto L7580;}
      else {
        s48_trace_stob_contentsB(env_221X);
        arg0K0 = (*((long *) (((char *) (-3 + env_221X)))));
        goto L7735;}}}
  else {
    goto L7580;}}
 L7580: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L7585;}
 L7585: {
  cont_222X = arg0K0;
  last_env_223X = arg0K1;
  env_224X = *((long *) ((((char *) (-3 + cont_222X))) + 12));
  s48_trace_stob_contentsB(cont_222X);
  if ((env_224X == last_env_223X)) {
    goto L7603;}
  else {
    arg0K0 = env_224X;
    goto L7690;}}
 L7603: {
  if ((cont_222X == (Sbottom_of_stackS))) {
    v_225X = s48_trace_value((Stemp0S));
    Stemp0S = v_225X;
    v_226X = s48_trace_value((Stemp1S));
    Stemp1S = v_226X;
    return;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + cont_222X)))));
    arg0K1 = env_224X;
    goto L7585;}}
 L7690: {
  env_227X = arg0K0;
  if ((3 == (3 & env_227X))) {
    if ((env_227X < (((long) (Sstack_beginS))))) {
      goto L7603;}
    else {
      if (((((long) (Sstack_endS))) < env_227X)) {
        goto L7603;}
      else {
        s48_trace_stob_contentsB(env_227X);
        arg0K0 = (*((long *) (((char *) (-3 + env_227X)))));
        goto L7690;}}}
  else {
    goto L7603;}}
 Hentry_tracer850165: {
  foo_link_166X = merged_arg0K0;{
  arg0K0 = foo_link_166X;
  arg0K1 = 1;
  goto L13683;}
 L13683: {
  foo_link_228X = arg0K0;
  done_link_229X = arg0K1;
  if ((0 == (3 & foo_link_228X))) {
    arg0K0 = (3 + (-4 & foo_link_228X));
    goto L13685;}
  else {
    arg0K0 = foo_link_228X;
    goto L13685;}}
 L13685: {
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
      goto L13683;}
    else {
      arg0K0 = next_link_232X;
      arg0K1 = new_foo_231X;
      goto L13683;}}}
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
    goto L3855;}
  else {
    goto L3855;}}
 L3855: {
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
    goto L6498;}
  else {
    goto L6498;}}
 L6498: {
  if (((4 + new_size_255X) < old_size_256X)) {
    *((long *) ((((char *) (-3 + bignum_254X))) + -4)) = (82 + (((new_size_255X)<<8)));
    *((long *) ((((char *) (((long) ((((char *) (-3 + bignum_254X))) + (-4 + (-4 & (3 + new_size_255X)))))))) + -4)) = (-942 + ((((old_size_256X - new_size_255X))<<8)));
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
    goto L10549;}
  else {
    x_262X = add_more_channels(os_index_260X);
    if (x_262X) {
      goto L10549;}
    else {
      return 36;}}}
 L10549: {
  if ((1 == (*((Svm_channelsS) + os_index_260X)))) {
    old_index_263X = (((*((long *) ((((char *) (-3 + channel_259X))) + 8))))>>2);
    x_264X = *((long *) ((((char *) (-3 + channel_259X))) + 16));
    if ((5 == x_264X)) {
      v_265X = ps_abort_fd_op(old_index_263X);
      enqueue_channelB(old_index_263X, v_265X);
      goto L10535;}
    else {
      goto L10535;}}
  else {
    return 44;}}
 L10535: {
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
    goto L14169;}
  else {
    goto L14169;}}
 L14169: {
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
  goto L15597;}
 L15597: {
  i_274X = arg0K0;
  temp_275X = 1024 == i_274X;
  if (temp_275X) {
    return temp_275X;}
  else {
    link_276X = *((long *) ((((char *) (-3 + table_273X))) + (((i_274X)<<2))));
    if ((0 == (3 & link_276X))) {
      arg0K0 = (3 + (-4 & link_276X));
      goto L15571;}
    else {
      arg0K0 = link_276X;
      goto L15571;}}}
 L15571: {
  entry_277X = arg0K0;
  if ((1 == entry_277X)) {
    arg0K0 = (1 + i_274X);
    goto L15597;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + entry_277X))) + 8)))))) {
      name_278X = *((long *) (((char *) (-3 + entry_277X))));
      ps_write_string("undefined imported name ", out_272X);
      if ((3 == (3 & name_278X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_278X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + name_278X))))), out_272X);
          goto L17578;}
        else {
          goto L17576;}}
      else {
        goto L17576;}}
    else {
      goto L15578;}}}
 L17578: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_272X, ignoreXX) }
  goto L15578;}
 L17576: {
  ps_write_string("<invalid name>", out_272X);
  goto L17578;}
 L15578: {
  link_279X = *((long *) ((((char *) (-3 + entry_277X))) + 12));
  if ((0 == (3 & link_279X))) {
    arg0K0 = (3 + (-4 & link_279X));
    goto L15571;}
  else {
    arg0K0 = link_279X;
    goto L15571;}}
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
      goto L17721;}}
  else {
    goto L17721;}}
 L17721: {
  return (((status_284X)<<2));}
}
long s48_enter_integer(long x_285X)
{
  char * v_286X;
 {  s48_make_availableAgc(16);
  if ((536870911 < x_285X)) {
    goto L19341;}
  else {
    if ((x_285X < -536870912)) {
      goto L19341;}
    else {
      return (((x_285X)<<2));}}}
 L19341: {
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
  return Hlookup2670((Sexported_bindingsS), value_289X, 0);}
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
  x_295X = Hlookup2651((Simported_bindingsS), value_294X, 0);
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
        goto L20466;}
      else {
        arg0K0 = maybe_302X;
        goto L20466;}}
    else {
      arg0K0 = maybe_301X;
      goto L20466;}}
  else {
    Sthe_symbol_tableS = symbol_table_299X;
    goto L20937;}}
 L20466: {
  symbols_303X = arg0K0;
  n_304X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_303X))) + -4))))>>8))))>>2);
  arg0K0 = 0;
  goto L20487;}
 L20937: {
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
    goto L20943;}
  else {
    Simported_bindingsS = imported_bindings_306X;
    Sexported_bindingsS = exported_bindings_305X;
    goto L20943;}}
 L20487: {
  i_309X = arg0K0;
  if ((i_309X == n_304X)) {
    goto L20937;}
  else {
    table_310X = Sthe_symbol_tableS;
    foo_311X = *((long *) ((((char *) (-3 + symbols_303X))) + (((i_309X)<<2))));
    string_312X = *((long *) (((char *) (-3 + foo_311X))));
    n_313X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_312X))) + -4))))>>8));
    arg0K0 = 0;
    arg0K1 = 0;
    goto L19894;}}
 L20943: {
  if ((stack_size_298X < 8131)) {
    stack_314X = (char *)malloc(32524);
    if ((stack_314X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_314X;
      arg0K1 = 8131;
      goto L17785;}
    else {
      arg1K0 = stack_314X;
      arg0K1 = 8131;
      goto L17785;}}
  else {
    arg1K0 = stack_begin_297X;
    arg0K1 = stack_size_298X;
    goto L17785;}}
 L19894: {
  i_315X = arg0K0;
  h_316X = arg0K1;
  if ((i_315X < n_313X)) {
    arg0K0 = (1 + i_315X);
    arg0K1 = (h_316X + (((*((unsigned char *) ((((char *) (-3 + string_312X))) + i_315X))))));
    goto L19894;}
  else {
    index_317X = 1023 & h_316X;
    val_318X = *((long *) ((((char *) (-3 + table_310X))) + (((index_317X)<<2))));
    addr_319X = (((char *) (-3 + foo_311X))) + 4;
    S48_WRITE_BARRIER(foo_311X, addr_319X, val_318X);
    *((long *) addr_319X) = val_318X;
    if ((3 == (3 & foo_311X))) {
      arg0K0 = (-4 & foo_311X);
      goto L19880;}
    else {
      arg0K0 = foo_311X;
      goto L19880;}}}
 L17785: {
  start_320X = arg1K0;
  size_321X = arg0K1;
  Sstack_beginS = start_320X;
  Sstack_endS = (start_320X + (((size_321X)<<2)));
  Sstack_limitS = ((Sstack_beginS) + 524);
  SstackS = ((Sstack_endS) + -4);
  ScontS = 1;
  SenvS = 17;
  arg1K0 = start_320X;
  goto L17817;}
 L19880: {
  value_322X = arg0K0;
  addr_323X = (((char *) (-3 + table_310X))) + (((index_317X)<<2));
  S48_WRITE_BARRIER(table_310X, addr_323X, value_322X);
  *((long *) addr_323X) = value_322X;
  arg0K0 = (1 + i_309X);
  goto L20487;}
 L17817: {
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
    *((unsigned char *) ((((char *) (-3 + code_334X))) + 2)) = 148;
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
    *((unsigned char *) ((((char *) (-3 + code_339X))) + 2)) = 145;
    Sexception_templateS = temp_338X;
    s48_make_availableAgc(32);
    s48_bignum_make_cached_constants();
    addr_341X = s48_allocate_tracedAgc(8);
    if ((addr_341X == NULL)) {
      arg0K0 = 1;
      goto L20994;}
    else {
      *((long *) addr_341X) = 1034;
      arg0K0 = (3 + (((long) (addr_341X + 4))));
      goto L20994;}}
  else {
    *((long *) a_324X) = 252645135;
    arg1K0 = (a_324X + 4);
    goto L17817;}}
 L20994: {
  vector_342X = arg0K0;
  if ((1 == vector_342X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_343X;
    goto L20981;}
  else {
    arg0K0 = vector_342X;
    goto L20981;}}
 L20981: {
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
    goto L9779;}
  else {
    *((long *) addr_347X) = 1048586;
    arg0K0 = (3 + (((long) (addr_347X + 4))));
    goto L9779;}}
 L9779: {
  vector_348X = arg0K0;
  if ((1 == vector_348X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_349X;
    goto L9767;}
  else {
    arg0K0 = vector_348X;
    goto L9767;}}
 L9767: {
  table_350X = arg0K0;
  arg0K0 = 0;
  goto L9802;}
 L9802: {
  i_351X = arg0K0;
  if ((1024 == i_351X)) {
    make_hash_tableAgc0_return_value = table_350X;
    goto make_hash_tableAgc_return;}
  else {
    addr_352X = (((char *) (-3 + table_350X))) + (((i_351X)<<2));
    S48_WRITE_BARRIER(table_350X, addr_352X, 1);
    *((long *) addr_352X) = 1;
    arg0K0 = (1 + i_351X);
    goto L9802;}}
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
  long pc_1639X;
  long tem_1638X;
  long cont_1637X;
  long i_1636X;
  long string_1635X;
  char * addr_1634X;
  long len_1633X;
  long len_1632X;
  long x_1631X;
  char *string_1630X;
  char move_slowP_1629X;
  long slow_1628X;
  long len_1627X;
  long fast_1626X;
  long v_1625X;
  char *v_1624X;
  long v_1623X;
  long v_1622X;
  long obj_1621X;
  long env_1620X;
  long thread_1619X;
  long x_1618X;
  char * addr_1617X;
  long v_1616X;
  long next_stob_1615X;
  long i_1614X;
  long log_1613X;
  char * addr_1612X;
  long next_stob_1611X;
  long i_1610X;
  long log_1609X;
  long v_1608X;
  char * x_1607X;
  char * new_stack_1606X;
  long new_cont_1605X;
  char * top_1604X;
  long v_1603X;
  long v_1602X;
  long n_1601X;
  long extra_1600X;
  long x_size_1599X;
  char * addr_1598X;
  long x_1597X;
  char * addr_1596X;
  long a_1595X;
  long last_1594X;
  long l_1593X;
  long x_1592X;
  char * addr_1591X;
  long a_1590X;
  long value_1589X;
  long x_1588X;
  char * addr_1587X;
  long a_1586X;
  long count_1585X;
  long args_1584X;
  long value_1583X;
  long env_1582X;
  long cont_1581X;
  char * addr_1580X;
  char * addr_1579X;
  long new_1578X;
  char * data_addr_1577X;
  char * addr_1576X;
  long header_1575X;
  long env_1574X;
  long p_1573X;
  long env_1572X;
  char * addr_1571X;
  long new_1570X;
  char * data_addr_1569X;
  char * addr_1568X;
  long header_1567X;
  long new_1566X;
  char * data_addr_1565X;
  char * addr_1564X;
  long header_1563X;
  long p_1562X;
  long previous_1561X;
  long cont_1560X;
  long end_1559X;
  long v_1558X;
  long p_1557X;
  char * arg_1556X;
  char * loc_1555X;
  char * top_1554X;
  long arg_count_1553X;
  long i_1552X;
  char * p_1551X;
  long l_1550X;
  long i_1549X;
  long list_1548X;
  long x_1547X;
  long v_1546X;
  long x_1545X;
  long v_1544X;
  long count_1543X;
  long x_1542X;
  long v_1541X;
  long status_1540X;
  long channel_1539X;
  long type_1538X;
  char v_1537X;
  long template_1536X;
  char not_firstP_1535X;
  long cont_1534X;
  char not_firstP_1533X;
  FILE * out_1532X;
  long current_template_1531X;
  long why_1530X;
  long val_1529X;
  long val_1528X;
  long val_1527X;
  long val_1526X;
  long val_1525X;
  long val_1524X;
  long n_1523X;
  char v_1522X;
  char * external_bignum_1521X;
  char * x_1520X;
  long val_1519X;
  long n_1518X;
  char v_1517X;
  char * external_bignum_1516X;
  char * x_1515X;
  long val_1514X;
  long n_1513X;
  char v_1512X;
  char * external_bignum_1511X;
  char * x_1510X;
  long v_1509X;
  long n_1508X;
  char v_1507X;
  long v_1506X;
  long rem_1505X;
  long quot_1504X;
  char div_by_zeroP_1503X;
  long val_1502X;
  long n_1501X;
  char v_1500X;
  char * external_bignum_1499X;
  char * x_1498X;
  long val_1497X;
  long n_1496X;
  char v_1495X;
  char * external_bignum_1494X;
  char * x_1493X;
  long val_1492X;
  long n_1491X;
  char v_1490X;
  char * external_bignum_1489X;
  char * x_1488X;
  long link_1487X;
  long v_1486X;
  long v_1485X;
  long stob_1484X;
  long v_1483X;
  long stob_1482X;
  char * addr_1481X;
  long x_1480X;
  char * addr_1479X;
  char * addr_1478X;
  long value_1477X;
  char * addr_1476X;
  long value_1475X;
  long copies_1474X;
  char * v_1473X;
  long value_1472X;
  char * y_1471X;
  char * v_1470X;
  long value_1469X;
  char * y_1468X;
  char * v_1467X;
  long value_1466X;
  char * y_1465X;
  long val_1464X;
  long v_1463X;
  long n_1462X;
  char v_1461X;
  char * rem_1460X;
  char * quot_1459X;
  char div_by_zeroP_1458X;
  char * x_1457X;
  char * v_1456X;
  long value_1455X;
  char * y_1454X;
  char * v_1453X;
  long value_1452X;
  char * y_1451X;
  char * v_1450X;
  long value_1449X;
  char * y_1448X;
  long obj_1447X;
  char * addr_1446X;
  long val_1445X;
  char * addr_1444X;
  long value_1443X;
  long len_1442X;
  long s2_1441X;
  long foo_1440X;
  long previous_foo_1439X;
  long value_1438X;
  long verify_1437X;
  long copies_1436X;
  long stob_1435X;
  long i_1434X;
  long x_1433X;
  long x_1432X;
  long val_1431X;
  char * addr_1430X;
  long value_1429X;
  char * v_1428X;
  long value_1427X;
  long v_1426X;
  char * v_1425X;
  long value_1424X;
  long v_1423X;
  char * v_1422X;
  long value_1421X;
  long v_1420X;
  long val_1419X;
  long val_1418X;
  long n_1417X;
  char v_1416X;
  char * external_bignum_1415X;
  char * x_1414X;
  long val_1413X;
  long n_1412X;
  char v_1411X;
  char * external_bignum_1410X;
  char * x_1409X;
  char * v_1408X;
  long value_1407X;
  char * y_1406X;
  char * v_1405X;
  long value_1404X;
  long v_1403X;
  long val_1402X;
  long v_1401X;
  char * v_1400X;
  char * v_1399X;
  long value_1398X;
  long extra1_1397X;
  long length1_1396X;
  char * v_1395X;
  long value_1394X;
  long v_1393X;
  long v_1392X;
  long bucket_1391X;
  char * addr_1390X;
  long value_1389X;
  long verify_1388X;
  long log_1387X;
  long stob_1386X;
  long i_1385X;
  long x_1384X;
  long status_1383X;
  char pendingP_1382X;
  char eofP_1381X;
  long got_1380X;
  long y_1379X;
  long link_1378X;
  long val_1377X;
  long x_1376X;
  char * addr_1375X;
  long b_1374X;
  long val_1373X;
  long extra1_1372X;
  long length1_1371X;
  long extra1_1370X;
  long length1_1369X;
  long extra1_1368X;
  long length1_1367X;
  long n_1366X;
  char * v_1365X;
  long val_1364X;
  long n_1363X;
  char v_1362X;
  char * external_bignum_1361X;
  char * v_1360X;
  long n_1359X;
  char v_1358X;
  char * external_bignum_1357X;
  char * v_1356X;
  long value_1355X;
  char * y_1354X;
  char * v_1353X;
  long value_1352X;
  char * y_1351X;
  char * v_1350X;
  long value_1349X;
  long extra1_1348X;
  long val_1347X;
  long v_1346X;
  char * v_1345X;
  long extra1_1344X;
  long length1_1343X;
  long val_1342X;
  long v_1341X;
  char * v_1340X;
  long x_1339X;
  long val_1338X;
  long v_1337X;
  char * v_1336X;
  long extra0_1335X;
  long length0_1334X;
  long extra1_1333X;
  long length1_1332X;
  long final_stack_arg_count_1331X;
  char v_1330X;
  char x_1329X;
  long channel_1328X;
  long handlers_1327X;
  long arg_count_1326X;
  char * addr_1325X;
  long value_1324X;
  long offset_1323X;
  long i_1322X;
  long count_1321X;
  char * addr_1320X;
  long value_1319X;
  long offset_1318X;
  long i_1317X;
  long count_1316X;
  char *v_1315X;
  long link_1314X;
  long index_1313X;
  long h_1312X;
  long i_1311X;
  char * addr_1310X;
  char * addr_1309X;
  long value_1308X;
  char * addr_1307X;
  char * addr_1306X;
  char * addr_1305X;
  long value_1304X;
  char * addr_1303X;
  char * addr_1302X;
  long value_1301X;
  long proposal_1300X;
  long entry_1299X;
  long thing_1298X;
  long log_1297X;
  long copies_1296X;
  char * addr_1295X;
  long x_1294X;
  long val_1293X;
  long status_1292X;
  long v_1291X;
  long len_1290X;
  long s2_1289X;
  long foo_1288X;
  long val_1287X;
  long v_1286X;
  long n_1285X;
  char v_1284X;
  char * external_bignum_1283X;
  char * x_1282X;
  long extra0_1281X;
  long length0_1280X;
  long extra0_1279X;
  long length0_1278X;
  long extra0_1277X;
  long length0_1276X;
  char * v_1275X;
  long value_1274X;
  long v_1273X;
  char * v_1272X;
  long value_1271X;
  long v_1270X;
  char * v_1269X;
  long v_1268X;
  char * x_1267X;
  char * v_1266X;
  long n_1265X;
  char * v_1264X;
  long value_1263X;
  long extra1_1262X;
  char * v_1261X;
  long value_1260X;
  long extra1_1259X;
  long val_1258X;
  long val_1257X;
  long val_1256X;
  long val_1255X;
  long val_1254X;
  long extra0_1253X;
  long length0_1252X;
  long val_1251X;
  long v_1250X;
  char * v_1249X;
  long x_1248X;
  long val_1247X;
  long v_1246X;
  char * v_1245X;
  long extra0_1244X;
  long length0_1243X;
  long y_1242X;
  long x_1241X;
  long extra0_1240X;
  long length0_1239X;
  char * arg_1238X;
  char * loc_1237X;
  long l_1236X;
  long stack_nargs_1235X;
  long x_1234X;
  long v_1233X;
  long skip_1232X;
  long template_1231X;
  long stack_arg_count_1230X;
  long skip_1229X;
  char * addr_1228X;
  long next_1227X;
  long channel_1226X;
  long m_1225X;
  long i_1224X;
  long count_1223X;
  long index_1222X;
  long i_1221X;
  long env_1220X;
  long count_1219X;
  long i_1218X;
  long env_1217X;
  long i_1216X;
  long l_1215X;
  long n_1214X;
  long table_1213X;
  long val_1212X;
  char temp_1211X;
  char minutesP_1210X;
  long v_1209X;
  long vector_1208X;
  char * addr_1207X;
  long x_1206X;
  long value_1205X;
  long verify_1204X;
  long copies_1203X;
  long stob_1202X;
  long i_1201X;
  char * addr_1200X;
  long v_1199X;
  char * addr_1198X;
  char * addr_1197X;
  long val_1196X;
  long x_1195X;
  long status_1194X;
  char pendingP_1193X;
  long got_1192X;
  long y_1191X;
  char v_1190X;
  long count_1189X;
  long start_1188X;
  char waitP_1187X;
  long x_1186X;
  long status_1185X;
  long channel_1184X;
  long v_1183X;
  long v_1182X;
  long v_1181X;
  long reason_1180X;
  long channel_1179X;
  long index_1178X;
  long bucket_1177X;
  long i_1176X;
  long i_1175X;
  char * addr_1174X;
  long i_1173X;
  long rest_list_1172X;
  long i_1171X;
  long v_1170X;
  long n_1169X;
  char v_1168X;
  char * external_bignum_1167X;
  char * x_1166X;
  long val_1165X;
  long v_1164X;
  char * v_1163X;
  char * v_1162X;
  long value_1161X;
  long needed_1160X;
  long y_1159X;
  long y_1158X;
  long x_1157X;
  long val_1156X;
  long v_1155X;
  char * v_1154X;
  long val_1153X;
  long v_1152X;
  char * v_1151X;
  long extra_1150X;
  long length_1149X;
  long count_1148X;
  long x_1147X;
  long extra_1146X;
  long length_1145X;
  char * v_1144X;
  long value_1143X;
  long extra_1142X;
  long length_1141X;
  long x_1140X;
  long c_1139X;
  long b_1138X;
  long extra0_1137X;
  long length0_1136X;
  long c_1135X;
  long b_1134X;
  long extra0_1133X;
  long length0_1132X;
  long val_1131X;
  long v_1130X;
  long v_1129X;
  long v_1128X;
  long val_1127X;
  long v_1126X;
  long v_1125X;
  long v_1124X;
  long val_1123X;
  long v_1122X;
  long v_1121X;
  long v_1120X;
  long val_1119X;
  long v_1118X;
  long v_1117X;
  long v_1116X;
  long val_1115X;
  char b_1114X;
  long result_1113X;
  char x_1112X;
  long c_1111X;
  long b_1110X;
  long c_1109X;
  long mid_c_1108X;
  long v_1107X;
  long v_1106X;
  long lo_c_1105X;
  long hi_b_1104X;
  long hi_a_1103X;
  long lo_b_1102X;
  long lo_a_1101X;
  long b_1100X;
  long args_1099X;
  char * arg_top_1098X;
  long list_arg_count_1097X;
  long list_args_1096X;
  long stack_nargs_1095X;
  long bytes_used_1094X;
  long count_1093X;
  long v_1092X;
  char * arg_1091X;
  char * loc_1090X;
  long args_1089X;
  long v_1088X;
  long cont_1087X;
  long index_1086X;
  long length_1085X;
  long wants_stack_args_1084X;
  long skip_1083X;
  long skip_1082X;
  long stack_space_1081X;
  long protocol_1080X;
  long n_1079X;
  long tem_1078X;
  char * addr_1077X;
  long x_1076X;
  long stack_arg_count_1075X;
  long back_1074X;
  long env_1073X;
  long offset_1072X;
  long i_1071X;
  long back_1070X;
  long env_1069X;
  long offset_1068X;
  long i_1067X;
  long obj_1066X;
  long obj_1065X;
  long type_1064X;
  long thing_1063X;
  long stuff_1062X;
  char * addr_1061X;
  long val_1060X;
  long x_1059X;
  long i_1058X;
  long b_1057X;
  long p_1056X;
  long port_1055X;
  long Kchar_1054X;
  long x_1053X;
  long i_1052X;
  long b_1051X;
  long p_1050X;
  long p_1049X;
  long port_1048X;
  char * addr_1047X;
  long val_1046X;
  long x_1045X;
  long i_1044X;
  long b_1043X;
  long p_1042X;
  long p_1041X;
  long port_1040X;
  char * addr_1039X;
  long value_1038X;
  long list_1037X;
  long head_1036X;
  char move_slowP_1035X;
  long slow_1034X;
  long list_1033X;
  long string_1032X;
  char * addr_1031X;
  long len_1030X;
  long val_1029X;
  long mseconds_1028X;
  long seconds_1027X;
  long option_1026X;
  long v_1025X;
  long v_1024X;
  long result_1023X;
  char * args_1022X;
  long name_1021X;
  long proc_1020X;
  long rest_list_1019X;
  long x_1018X;
  long tem_1017X;
  long pc_1016X;
  char * addr_1015X;
  long x_1014X;
  long proposal_1013X;
  long value_1012X;
  long vector_1011X;
  long type_1010X;
  char firstP_1009X;
  long vector_1008X;
  char firstP_1007X;
  long h_1006X;
  long i_1005X;
  long x_1004X;
  long status_1003X;
  long reason_1002X;
  char * addr_1001X;
  long next_stob_1000X;
  long i_999X;
  long v_998X;
  long next_stob_997X;
  long i_996X;
  long value_995X;
  char * addr_994X;
  long count_993X;
  long to_index_992X;
  long from_index_991X;
  long copies_990X;
  long left_989X;
  long value_988X;
  long value_987X;
  long verify_986X;
  long log_985X;
  long stob_984X;
  long i_983X;
  char * addr_982X;
  long old_981X;
  long x_980X;
  char * addr_979X;
  long channel_978X;
  long res_977X;
  long i_976X;
  long y_975X;
  long n_974X;
  char * addr_973X;
  long prev_972X;
  long ch_971X;
  long val_970X;
  long val_969X;
  long v_968X;
  long v_967X;
  long val_966X;
  char * addr_965X;
  char * addr_964X;
  long val_963X;
  long link_962X;
  long index_961X;
  long h_960X;
  long i_959X;
  long vector_958X;
  long vector_957X;
  char * addr_956X;
  long value_955X;
  long value_954X;
  long x_953X;
  char * addr_952X;
  long i_951X;
  long i_950X;
  long value_949X;
  long val_948X;
  long val_947X;
  long val_946X;
  long val_945X;
  char * v_944X;
  long value_943X;
  long needed_942X;
  long y_941X;
  long x_940X;
  long result_939X;
  long x_938X;
  long x_937X;
  long x_936X;
  long count_935X;
  long value_934X;
  long val_933X;
  long val_932X;
  long x_931X;
  long x_930X;
  long n_929X;
  long a_928X;
  long y_927X;
  long x_926X;
  long a_925X;
  long y_924X;
  long x_923X;
  long val_922X;
  long val_921X;
  char b_920X;
  long val_919X;
  char b_918X;
  long val_917X;
  char b_916X;
  long val_915X;
  long Kdouble_914X;
  char * addr_913X;
  double value_912X;
  long value_911X;
  double y_910X;
  long value_909X;
  long a_908X;
  long Kdouble_907X;
  char * addr_906X;
  double x_905X;
  long value_904X;
  double y_903X;
  long value_902X;
  long val_901X;
  long v_900X;
  char * v_899X;
  long Kdouble_898X;
  char * addr_897X;
  double x_896X;
  long value_895X;
  double y_894X;
  long value_893X;
  long a_892X;
  long Kdouble_891X;
  char * addr_890X;
  double x_889X;
  long value_888X;
  double y_887X;
  long value_886X;
  long val_885X;
  long v_884X;
  char * v_883X;
  long val_882X;
  long val_881X;
  long delta_880X;
  long delta_879X;
  long offset_878X;
  char v_877X;
  long tem_876X;
  long args_875X;
  long index_874X;
  char * arg_top_873X;
  long args_872X;
  long count_871X;
  long index_870X;
  long next_869X;
  long cont_868X;
  long args_867X;
  long protocol_866X;
  long pc_865X;
  long code_864X;
  long cont_863X;
  long list_arg_count_862X;
  long list_args_861X;
  long stack_nargs_860X;
  long args_859X;
  long args_858X;
  long x_857X;
  char * addr_856X;
  long a_855X;
  long wants_stack_args_854X;
  long index_853X;
  long next_852X;
  long cont_851X;
  long protocol_850X;
  long pc_849X;
  long code_848X;
  long cont_847X;
  long obj_846X;
  char * addr_845X;
  long list_args_844X;
  long follower_843X;
  long list_842X;
  long proc_841X;
  long args_840X;
  long list_arg_count_839X;
  char okayP_838X;
  long stack_nargs_837X;
  long list_args_836X;
  long obj_835X;
  long obj_834X;
  long list_arg_count_833X;
  long list_args_832X;
  long stack_arg_count_831X;
  long args_830X;
  long v_829X;
  long v_828X;
  long v_827X;
  long list_arg_count_826X;
  long list_args_825X;
  long stack_arg_count_824X;
  long exception_823X;
  long total_arg_count_822X;
  long code_821X;
  long list_arg_count_820X;
  long list_args_819X;
  long stack_arg_count_818X;
  char * arg_817X;
  char * loc_816X;
  char v_815X;
  long value_814X;
  long start_i_813X;
  long start_i_812X;
  char * addr_811X;
  long closure_810X;
  long index_809X;
  char * addr_808X;
  long x_807X;
  char * addr_806X;
  long value_805X;
  long i_804X;
  long value_803X;
  long key_802X;
  long env_801X;
  long v_800X;
  long p_799X;
  long v_798X;
  char * addr_797X;
  long value_796X;
  long index_795X;
  long i_794X;
  long env_793X;
  long i_792X;
  long env_791X;
  long i_790X;
  long env_789X;
  long handlers_788X;
  long opcode_787X;
  long nargs_786X;
  FILE * out_785X;
  long x_784X;
  long v_783X;
  long v_782X;
  long v_781X;
  long v_780X;
  long v_779X;
  long v_778X;
  long v_777X;
  long v_776X;
  long len_775X;
  long value_774X;
  long index_773X;
  long arg4_772X;
  long arg3_771X;
  long arg2_770X;
  long v_769X;
  long v_768X;
  long len_767X;
  long index_766X;
  long arg3_765X;
  long arg2_764X;
  long list_763X;
  long thing_762X;
  long n_761X;
  long arg2_760X;
  long len_759X;
  long x_758X;
  long obj_757X;
  long arg2_756X;
  long x_755X;
  long arg2_754X;
  long status_753X;
  long value_752X;
  long key_751X;
  long arg2_750X;
  long mseconds_749X;
  long seconds_748X;
  long mseconds_747X;
  long seconds_746X;
  long x_745X;
  long other_744X;
  long option_743X;
  long arg2_742X;
  long x_741X;
  long arg2_740X;
  long x_739X;
  long arg2_738X;
  long rest_list_737X;
  long p_736X;
  long nargs_735X;
  long x_734X;
  long arg2_733X;
  long p_732X;
  long p_731X;
  long p_730X;
  long old_729X;
  long temp_728X;
  long obj_727X;
  long p_726X;
  long template_725X;
  long p_724X;
  long temp_723X;
  long obj_722X;
  long val_721X;
  long x_720X;
  long type_719X;
  long x_718X;
  long bytes_717X;
  long x_716X;
  long other_715X;
  long key_714X;
  long arg2_713X;
  long x_712X;
  char * addr_711X;
  long b_710X;
  long x_709X;
  char * addr_708X;
  long proc_707X;
  long stob_706X;
  long n_705X;
  long x_704X;
  long obj_703X;
  long status_702X;
  long status_701X;
  long status_700X;
  long status_699X;
  long status_698X;
  long status_697X;
  FILE * port_696X;
  long undumpables_695X;
  long obj_694X;
  long arg4_693X;
  long arg3_692X;
  long arg2_691X;
  long x_690X;
  long log_689X;
  long index_688X;
  long x_687X;
  long len_686X;
  long byte_685X;
  long index_684X;
  long arg3_683X;
  long arg2_682X;
  long log_681X;
  long index_680X;
  long x_679X;
  long len_678X;
  long index_677X;
  long arg2_676X;
  char v_675X;
  char v_674X;
  long v_673X;
  long count_672X;
  long to_index_671X;
  long from_index_670X;
  long arg5_669X;
  long arg4_668X;
  long arg3_667X;
  long arg2_666X;
  long v_665X;
  long x_664X;
  long type_663X;
  long offset_662X;
  long stob_661X;
  long log_660X;
  long proposal_659X;
  long proposal_658X;
  char * addr_657X;
  long next_656X;
  long channel_655X;
  long n_654X;
  char * addr_653X;
  long head_652X;
  long channel_651X;
  long obj_650X;
  long x_649X;
  long status_648X;
  char readyP_647X;
  long channel_646X;
  long obj_645X;
  char v_644X;
  long count_643X;
  long start_642X;
  long arg4_641X;
  long arg3_640X;
  long arg2_639X;
  long x_638X;
  long arg5_637X;
  long arg4_636X;
  long arg3_635X;
  long arg2_634X;
  long x_633X;
  long status_632X;
  long channel_631X;
  long obj_630X;
  long mode_629X;
  long arg2_628X;
  long x_627X;
  long x_626X;
  long x_625X;
  long arg2_624X;
  long descriptor_623X;
  long x_622X;
  long obj_621X;
  long n_620X;
  long string_619X;
  long table_618X;
  long obj_617X;
  long len_616X;
  char Kchar_615X;
  long index_614X;
  long arg3_613X;
  long arg2_612X;
  long len_611X;
  long index_610X;
  long arg2_609X;
  long obj_608X;
  char * addr_607X;
  long len_606X;
  char init_605X;
  long len_604X;
  long arg2_603X;
  long len_602X;
  long Kchar_601X;
  long index_600X;
  long arg3_599X;
  long arg2_598X;
  long len_597X;
  long index_596X;
  long arg2_595X;
  long obj_594X;
  char * addr_593X;
  long init_592X;
  long len_591X;
  long arg2_590X;
  long v_589X;
  long index_588X;
  long len_587X;
  long type_586X;
  long value_585X;
  long stob_584X;
  long arg2_583X;
  long v_582X;
  long v_581X;
  long index_580X;
  long len_579X;
  long type_578X;
  long index_577X;
  long stob_576X;
  char * addr_575X;
  long len_574X;
  long len_573X;
  long type_572X;
  long init_571X;
  long len_570X;
  long v_569X;
  long type_568X;
  long offset_567X;
  long value_566X;
  long stob_565X;
  long type_564X;
  long offset_563X;
  long stob_562X;
  long rest_list_561X;
  long stack_nargs_560X;
  long new_559X;
  char * addr_558X;
  long len_557X;
  long type_556X;
  long len_555X;
  long new_554X;
  char * addr_553X;
  long len_552X;
  long type_551X;
  long len_550X;
  long type_549X;
  long stob_548X;
  long type_547X;
  long x_546X;
  long x_545X;
  long x_544X;
  long x_543X;
  long arg2_542X;
  long x_541X;
  long arg2_540X;
  long x_539X;
  long v_538X;
  long v_537X;
  long y_536X;
  long x_535X;
  long y_534X;
  long x_533X;
  long y_532X;
  long x_531X;
  long y_530X;
  long x_529X;
  long x_528X;
  long x_527X;
  long x_526X;
  long x_525X;
  long arg2_524X;
  long x_523X;
  long arg2_522X;
  long x_521X;
  long arg2_520X;
  long x_519X;
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
  goto L23677;}
 L23677: {
  code_pointer_391X = arg1K0;
  switch ((*((unsigned char *) code_pointer_391X))) {
    case 0 : {
      push_exception_continuationB(14, 1);
      arg0K0 = 0;
      goto L21732;}
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
      goto L23677;}
      break;
    case 2 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 3 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 4 : {
      back_393X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_394X = SenvS;
      arg0K0 = env_394X;
      arg0K1 = back_393X;
      goto L30949;}
      break;
    case 5 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21732;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23677;}}
      break;
    case 6 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21732;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23677;}}
      break;
    case 7 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS)))))))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L21732;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23677;}}
      break;
    case 8 : {
      back_395X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_396X = SenvS;
      arg0K0 = env_396X;
      arg0K1 = back_395X;
      goto L25644;}
      break;
    case 9 : {
      back_397X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_398X = SenvS;
      arg0K0 = env_398X;
      arg0K1 = back_397X;
      goto L25582;}
      break;
    case 10 : {
      location_399X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_399X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        push_exception_continuationB(1, 3);
        *((long *) (SstackS)) = location_399X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21732;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23677;}}
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
        goto L21732;}
      else {
        val_401X = SvalS;
        addr_402X = (((char *) (-3 + location_400X))) + 4;
        S48_WRITE_BARRIER(location_400X, addr_402X, val_401X);
        *((long *) addr_402X) = val_401X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23677;}}
      break;
    case 12 : {
      v_403X = *((unsigned char *) ((Scode_pointerS) + 3));
      if ((0 == v_403X)) {
        p_404X = SenvS;
        if ((3 == (3 & p_404X))) {
          if ((p_404X < (((long) (Sstack_beginS))))) {
            arg0K0 = 0;
            goto L26319;}
          else {
            if (((((long) (Sstack_endS))) < p_404X)) {
              arg0K0 = 0;
              goto L26319;}
            else {
              arg0K0 = ((Sstack_endS) - (Sstack_beginS));
              goto L26319;}}}
        else {
          arg0K0 = 0;
          goto L26319;}}
      else {
        s48_make_availableAgc(12);
        arg0K0 = (SvalS);
        arg0K1 = 0;
        goto L26332;}}
      break;
    case 13 : {
      count_405X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      s48_make_availableAgc((((((2 + count_405X) + (3 * count_405X)))<<2)));
      len_406X = 4 + (((count_405X)<<2));
      addr_407X = s48_allocate_small((4 + len_406X));
      *((long *) addr_407X) = (10 + (((len_406X)<<8)));
      vector_408X = 3 + (((long) (addr_407X + 4)));
      arg0K0 = 1;
      goto L25369;}
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
        goto L26656;}
      else {
        value_414X = SvalS;
        addr_415X = ((char *) (-3 + new_env_412X));
        S48_WRITE_BARRIER(new_env_412X, addr_415X, value_414X);
        *((long *) addr_415X) = value_414X;
        arg0K0 = 1;
        goto L26656;}}
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
        goto L26507;}
      else {
        value_421X = SvalS;
        addr_422X = ((char *) (-3 + new_env_419X));
        S48_WRITE_BARRIER(new_env_419X, addr_422X, value_421X);
        *((long *) addr_422X) = value_421X;
        arg0K0 = 1;
        goto L26507;}}
      break;
    case 16 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 17 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L21732;}
      else {
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23677;}}
      break;
    case 18 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L21732;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23677;}}
      break;
    case 19 : {
      SstackS = ((SstackS) + 4);
      SvalS = (*((long *) (SstackS)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 20 : {
      SvalS = (*((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 21 : {
      *((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 22 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (*((unsigned char *) ((Scode_pointerS) + 3))));
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 23 : {
      push_continuationB(((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))))), (((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))));
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 24 : {
      s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
      if ((1 == (ScontS))) {
        arg0K0 = 1;
        goto L47635;}
      else {
        merged_arg0K0 = 0;
        merged_arg0K1 = 2;
        really_preserve_continuation_return_tag = 0;
        goto really_preserve_continuation;
       really_preserve_continuation_return_0:
        v_423X = really_preserve_continuation0_return_value;
        arg0K0 = v_423X;
        goto L47635;}}
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
                goto L24334;}
              else {
                goto L24334;}}
            else {
              goto L24334;}}
          else {
            if (((127 & protocol_428X) == stack_arg_count_424X)) {
              proc_429X = SvalS;
              StemplateS = (*((long *) (((char *) (-3 + proc_429X)))));
              Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS))))))))) + 2);
              top_of_args_430X = SstackS;
              top_of_cont_431X = (((char *) (-3 + (ScontS)))) + -8;
              if ((top_of_cont_431X == (top_of_args_430X + (((stack_arg_count_424X)<<2))))) {
                s48_call_native_code(proc_429X);
                goto L24344;}
              else {
                SstackS = top_of_cont_431X;
                arg1K0 = (SstackS);
                arg1K1 = (top_of_args_430X + (((stack_arg_count_424X)<<2)));
                goto L7893;}}
            else {
              arg0K0 = stack_arg_count_424X;
              arg0K1 = 25;
              arg0K2 = 0;
              goto L21362;}}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_424X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22234;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_424X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22234;}}
      break;
    case 26 : {
      stack_arg_count_432X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      obj_433X = SvalS;
      if ((3 == (3 & obj_433X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_433X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_432X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L21362;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_432X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22234;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_432X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22234;}}
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
        goto L22863;}
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
        goto L21732;}}
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
        goto L14845;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_440X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_440X)))));
          arg0K1 = (-1 + stack_nargs_438X);
          goto L14845;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_440X))) + 4)))))) + 4)));
          arg0K1 = rest_list_440X;
          goto L14886;}}}
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
          goto L22808;}
        else {
          goto L22815;}}
      else {
        goto L22815;}}
      break;
    case 30 : {
      goto L24944;}
      break;
    case 31 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L47098;}
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
      goto L47098;}
      break;
    case 33 : {
      tem_444X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      StemplateS = tem_444X;
      Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_444X))))))));
      arg1K0 = (Scode_pointerS);
      goto L23677;}
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
            goto L24074;}
          else {
            goto L24074;}}
        else {
          goto L24074;}}
      else {
        v_446X = *((unsigned char *) ((((char *) (-3 + code_445X))) + 1));
        if ((67 == v_446X)) {
          if ((0 == (*((unsigned char *) ((((char *) (-3 + code_445X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_445X))) + -4))))>>8)))))))) {
            index_447X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_445X))) + -4))))>>8));
            space_448X = ((((*((unsigned char *) ((((char *) (-3 + code_445X))) + index_447X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_445X))) + (1 + index_447X))));
            if ((space_448X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
              goto L24074;}
            else {
              s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
              merged_arg0K0 = 0;
              copy_stack_into_heap_return_tag = 2;
              goto copy_stack_into_heap;
             copy_stack_into_heap_return_2:
              if ((space_448X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
                goto L24074;}
              else {
                ps_error("VM's stack is too small (how can this happen?)", 0);
                goto L24074;}}}
          else {
            goto L24117;}}
        else {
          goto L24117;}}}
      break;
    case 35 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L23677;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23677;}}
      break;
    case 36 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 37 : {
      if ((0 == (3 & (SvalS)))) {
        p_449X = SvalS;
        max_450X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_451X = ((p_449X)>>2);
        if ((val_451X < 0)) {
          goto L23975;}
        else {
          if ((val_451X < max_450X)) {
            index_452X = 1 + (((val_451X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_452X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_452X)))));
            goto L23977;}
          else {
            goto L23975;}}}
      else {
        push_exception_continuationB(5, 0);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21732;}}
      break;
    case 38 : {
      stack_nargs_453X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((0 == stack_nargs_453X)) {
        arg0_454X = *((long *) ((SstackS) + 12));
        rest_list_455X = *((long *) ((SstackS) + 8));
        *((long *) (SstackS)) = arg0_454X;
        SstackS = ((SstackS) + -4);
        SvalS = (*((long *) (((char *) (-3 + rest_list_455X)))));
        goto L24860;}
      else {
        arg1_456X = *((long *) ((SstackS) + (4 + (((stack_nargs_453X)<<2)))));
        arg0_457X = *((long *) ((SstackS) + (8 + (((stack_nargs_453X)<<2)))));
        *((long *) ((SstackS) + (8 + (((stack_nargs_453X)<<2))))) = 1;
        *((long *) ((SstackS) + 4)) = (-4 + (((stack_nargs_453X)<<2)));
        *((long *) (SstackS)) = arg0_457X;
        SstackS = ((SstackS) + -4);
        SvalS = arg1_456X;
        goto L24860;}}
      break;
    case 39 : {
      stack_nargs_458X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((stack_nargs_458X == 0)) {
        rest_list_459X = *((long *) ((SstackS) + 8));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_459X))) + 4))))) {
          arg0K0 = 1;
          goto L23873;}
        else {
          *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_459X))) + 4)));
          *((long *) ((SstackS) + 12)) = (SvalS);
          arg0K0 = -2;
          goto L23873;}}
      else {
        if ((stack_nargs_458X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 8))))) {
            arg0K0 = 1;
            goto L23873;}
          else {
            *((long *) ((SstackS) + 4)) = 0;
            *((long *) ((SstackS) + 12)) = (SvalS);
            arg0K0 = -2;
            goto L23873;}}
        else {
          *((long *) ((SstackS) + (8 + (((stack_nargs_458X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L23873;}}}
      break;
    case 40 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23677;}
      else {
        x_460X = (((*((long *) ((SstackS) + 4))))>>2);
        if ((x_460X == 0)) {
          rest_list_461X = *((long *) ((SstackS) + 8));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_461X))) + 4))))) {
            arg0K0 = 1;
            goto L24751;}
          else {
            *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_461X))) + 4)));
            *((long *) ((SstackS) + 12)) = (*((long *) (((char *) (-3 + rest_list_461X)))));
            arg0K0 = -2;
            goto L24751;}}
        else {
          if ((x_460X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 8))))) {
              arg0K0 = 1;
              goto L24751;}
            else {
              *((long *) ((SstackS) + 4)) = 0;
              arg0K0 = -2;
              goto L24751;}}
          else {
            arg0K0 = -2;
            goto L24751;}}}}
      break;
    case 41 : {
      SstackS = ((SstackS) + 4);
      arg2_462X = *((long *) (SstackS));
      x_463X = SvalS;
      if ((arg2_462X == x_463X)) {
        arg0K0 = 5;
        goto L47688;}
      else {
        arg0K0 = 1;
        goto L47688;}}
      break;
    case 42 : {
      x_464X = SvalS;
      if ((0 == (3 & x_464X))) {
        arg0K0 = 5;
        goto L47700;}
      else {
        if ((3 == (3 & x_464X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L47700;}
          else {
            goto L13483;}}
        else {
          goto L13483;}}}
      break;
    case 43 : {
      n_465X = SvalS;
      if ((0 == (3 & n_465X))) {
        goto L39189;}
      else {
        if ((3 == (3 & n_465X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_465X))) + -4))))>>2))))) {
            goto L39189;}
          else {
            goto L39190;}}
        else {
          goto L39190;}}}
      break;
    case 44 : {
      n_466X = SvalS;
      if ((0 == (3 & n_466X))) {
        goto L39346;}
      else {
        if ((3 == (3 & n_466X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
            goto L39346;}
          else {
            goto L39321;}}
        else {
          goto L39321;}}}
      break;
    case 45 : {
      n_467X = SvalS;
      if ((0 == (3 & n_467X))) {
        goto L39545;}
      else {
        if ((3 == (3 & n_467X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
            goto L39545;}
          else {
            goto L39520;}}
        else {
          goto L39520;}}}
      break;
    case 46 : {
      n_468X = SvalS;
      if ((0 == (3 & n_468X))) {
        goto L39744;}
      else {
        if ((3 == (3 & n_468X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
            goto L39744;}
          else {
            goto L39719;}}
        else {
          goto L39719;}}}
      break;
    case 47 : {
      n_469X = SvalS;
      if ((0 == (3 & n_469X))) {
        goto L32445;}
      else {
        if ((3 == (3 & n_469X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_469X))) + -4))))>>2))))) {
            goto L32445;}
          else {
            goto L32446;}}
        else {
          goto L32446;}}}
      break;
    case 48 : {
      x_470X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_470X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 49 : {
      x_471X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_471X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 50 : {
      SstackS = ((SstackS) + 4);
      x_472X = *((long *) (SstackS));
      y_473X = SvalS;
      if ((0 == (3 & (x_472X | y_473X)))) {
        s48_make_availableAgc(16);
        x_474X = (((x_472X)>>2)) + (((y_473X)>>2));
        if ((536870911 < x_474X)) {
          goto L32571;}
        else {
          if ((x_474X < -536870912)) {
            goto L32571;}
          else {
            arg0K0 = (((x_474X)<<2));
            goto L32517;}}}
      else {
        if ((0 == (3 & x_472X))) {
          goto L32523;}
        else {
          if ((3 == (3 & x_472X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_472X))) + -4))))>>2))))) {
              goto L32523;}
            else {
              goto L32532;}}
          else {
            goto L32532;}}}}
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
          goto L8040;}
        else {
          arg0K0 = a_477X;
          goto L8040;}}
      else {
        if ((0 == (3 & x_475X))) {
          goto L41491;}
        else {
          if ((3 == (3 & x_475X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_475X))) + -4))))>>2))))) {
              goto L41491;}
            else {
              goto L41500;}}
          else {
            goto L41500;}}}}
      break;
    case 52 : {
      SstackS = ((SstackS) + 4);
      x_479X = *((long *) (SstackS));
      y_480X = SvalS;
      if ((0 == (3 & (x_479X | y_480X)))) {
        s48_make_availableAgc(16);
        x_481X = (((x_479X)>>2)) - (((y_480X)>>2));
        if ((536870911 < x_481X)) {
          goto L32860;}
        else {
          if ((x_481X < -536870912)) {
            goto L32860;}
          else {
            arg0K0 = (((x_481X)<<2));
            goto L32806;}}}
      else {
        if ((0 == (3 & x_479X))) {
          goto L32812;}
        else {
          if ((3 == (3 & x_479X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_479X))) + -4))))>>2))))) {
              goto L32812;}
            else {
              goto L32821;}}
          else {
            goto L32821;}}}}
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
        goto L21732;}
      else {
        if ((0 == (3 & (x_482X | y_483X)))) {
          if ((0 == y_483X)) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = x_482X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_483X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21732;}
          else {
            a_484X = ((x_482X)>>2);
            b_485X = ((y_483X)>>2);
            if ((a_484X < 0)) {
              arg0K0 = (0 - a_484X);
              goto L8298;}
            else {
              arg0K0 = a_484X;
              goto L8298;}}}
        else {
          if ((0 == (3 & x_482X))) {
            goto L41744;}
          else {
            if ((3 == (3 & x_482X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_482X))) + -4))))>>2))))) {
                goto L41744;}
              else {
                goto L41777;}}
            else {
              goto L41777;}}}}}
      break;
    case 54 : {
      SstackS = ((SstackS) + 4);
      x_486X = *((long *) (SstackS));
      y_487X = SvalS;
      if ((0 == (3 & (x_486X | y_487X)))) {
        if ((x_486X == y_487X)) {
          arg0K0 = 5;
          goto L33091;}
        else {
          arg0K0 = 1;
          goto L33091;}}
      else {
        if ((0 == (3 & x_486X))) {
          goto L33097;}
        else {
          if ((3 == (3 & x_486X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_486X))) + -4))))>>2))))) {
              goto L33097;}
            else {
              goto L33108;}}
          else {
            goto L33108;}}}}
      break;
    case 55 : {
      SstackS = ((SstackS) + 4);
      x_488X = *((long *) (SstackS));
      y_489X = SvalS;
      if ((0 == (3 & (x_488X | y_489X)))) {
        if ((x_488X < y_489X)) {
          arg0K0 = 5;
          goto L33351;}
        else {
          arg0K0 = 1;
          goto L33351;}}
      else {
        if ((0 == (3 & x_488X))) {
          goto L33357;}
        else {
          if ((3 == (3 & x_488X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_488X))) + -4))))>>2))))) {
              goto L33357;}
            else {
              goto L33368;}}
          else {
            goto L33368;}}}}
      break;
    case 56 : {
      SstackS = ((SstackS) + 4);
      x_490X = *((long *) (SstackS));
      y_491X = SvalS;
      if ((0 == (3 & (x_490X | y_491X)))) {
        if ((y_491X < x_490X)) {
          arg0K0 = 5;
          goto L33674;}
        else {
          arg0K0 = 1;
          goto L33674;}}
      else {
        if ((0 == (3 & x_490X))) {
          goto L33680;}
        else {
          if ((3 == (3 & x_490X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_490X))) + -4))))>>2))))) {
              goto L33680;}
            else {
              goto L33691;}}
          else {
            goto L33691;}}}}
      break;
    case 57 : {
      SstackS = ((SstackS) + 4);
      x_492X = *((long *) (SstackS));
      y_493X = SvalS;
      if ((0 == (3 & (x_492X | y_493X)))) {
        if ((y_493X < x_492X)) {
          arg0K0 = 1;
          goto L33997;}
        else {
          arg0K0 = 5;
          goto L33997;}}
      else {
        if ((0 == (3 & x_492X))) {
          goto L34003;}
        else {
          if ((3 == (3 & x_492X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_492X))) + -4))))>>2))))) {
              goto L34003;}
            else {
              goto L34014;}}
          else {
            goto L34014;}}}}
      break;
    case 58 : {
      SstackS = ((SstackS) + 4);
      x_494X = *((long *) (SstackS));
      y_495X = SvalS;
      if ((0 == (3 & (x_494X | y_495X)))) {
        if ((x_494X < y_495X)) {
          arg0K0 = 1;
          goto L34291;}
        else {
          arg0K0 = 5;
          goto L34291;}}
      else {
        if ((0 == (3 & x_494X))) {
          goto L34297;}
        else {
          if ((3 == (3 & x_494X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_494X))) + -4))))>>2))))) {
              goto L34297;}
            else {
              goto L34308;}}
          else {
            goto L34308;}}}}
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
        goto L21732;}
      else {
        if ((0 == (3 & (x_496X | y_497X)))) {
          if ((0 == y_497X)) {
            arg0K0 = x_496X;
            arg0K1 = y_497X;
            goto L18997;}
          else {
            a_498X = ((x_496X)>>2);
            b_499X = ((y_497X)>>2);
            if ((a_498X < 0)) {
              arg0K0 = (0 - a_498X);
              goto L8943;}
            else {
              arg0K0 = a_498X;
              goto L8943;}}}
        else {
          if ((0 == (3 & x_496X))) {
            goto L34597;}
          else {
            if ((3 == (3 & x_496X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_496X))) + -4))))>>2))))) {
                goto L34597;}
              else {
                goto L34606;}}
            else {
              goto L34606;}}}}}
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
        goto L21732;}
      else {
        if ((0 == (3 & (x_500X | y_501X)))) {
          if ((0 == y_501X)) {
            arg0K0 = x_500X;
            arg0K1 = y_501X;
            goto L18906;}
          else {
            a_502X = ((x_500X)>>2);
            if ((a_502X < 0)) {
              arg0K0 = (0 - a_502X);
              goto L34811;}
            else {
              arg0K0 = a_502X;
              goto L34811;}}}
        else {
          if ((0 == (3 & x_500X))) {
            goto L34773;}
          else {
            if ((3 == (3 & x_500X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_500X))) + -4))))>>2))))) {
                goto L34773;}
              else {
                goto L34782;}}
            else {
              goto L34782;}}}}}
      break;
    case 61 : {
      n_503X = SvalS;
      if ((0 == (3 & n_503X))) {
        goto L34960;}
      else {
        if ((3 == (3 & n_503X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_503X))) + -4))))>>2))))) {
            goto L34960;}
          else {
            goto L34961;}}
        else {
          goto L34961;}}}
      break;
    case 62 : {
      n_504X = SvalS;
      if ((0 == (3 & n_504X))) {
        goto L35015;}
      else {
        if ((3 == (3 & n_504X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_504X))) + -4))))>>2))))) {
            goto L35015;}
          else {
            goto L35016;}}
        else {
          goto L35016;}}}
      break;
    case 63 : {
      n_505X = SvalS;
      if ((0 == (3 & n_505X))) {
        goto L35070;}
      else {
        if ((3 == (3 & n_505X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_505X))) + -4))))>>2))))) {
            goto L35070;}
          else {
            goto L35073;}}
        else {
          goto L35073;}}}
      break;
    case 64 : {
      n_506X = SvalS;
      if ((0 == (3 & n_506X))) {
        goto L35130;}
      else {
        if ((3 == (3 & n_506X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_506X))) + -4))))>>2))))) {
            goto L35130;}
          else {
            goto L35131;}}
        else {
          goto L35131;}}}
      break;
    case 65 : {
      n_507X = SvalS;
      if ((0 == (3 & n_507X))) {
        goto L35185;}
      else {
        if ((3 == (3 & n_507X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_507X))) + -4))))>>2))))) {
            goto L35185;}
          else {
            goto L35188;}}
        else {
          goto L35188;}}}
      break;
    case 66 : {
      x_508X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_508X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 67 : {
      x_509X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_509X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 68 : {
      x_510X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_510X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 69 : {
      x_511X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_511X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 70 : {
      x_512X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_512X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 71 : {
      x_513X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_513X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 72 : {
      x_514X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_514X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 73 : {
      x_515X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_515X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 74 : {
      x_516X = SvalS;
      if ((0 == (3 & x_516X))) {
        if ((x_516X < 0)) {
          goto L47854;}
        else {
          goto L47860;}}
      else {
        v_517X = s48_bignum_test((((char *) (-3 + x_516X))));
        if ((-1 == v_517X)) {
          goto L47854;}
        else {
          goto L47860;}}}
      break;
    case 75 : {
      x_518X = SvalS;
      if ((0 == (3 & x_518X))) {
        goto L22483;}
      else {
        if ((3 == (3 & x_518X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_518X))) + -4))))>>2))))) {
            goto L22483;}
          else {
            goto L22486;}}
        else {
          goto L22486;}}}
      break;
    case 76 : {
      x_519X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = x_519X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 77 : {
      SstackS = ((SstackS) + 4);
      arg2_520X = *((long *) (SstackS));
      x_521X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_520X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_521X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}
      break;
    case 78 : {
      SstackS = ((SstackS) + 4);
      arg2_522X = *((long *) (SstackS));
      x_523X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_522X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_523X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}
      break;
    case 79 : {
      SstackS = ((SstackS) + 4);
      arg2_524X = *((long *) (SstackS));
      x_525X = SvalS;
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_524X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_525X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}
      break;
    case 80 : {
      x_526X = SvalS;
      if ((0 == (3 & x_526X))) {
        SvalS = (~ (3 | x_526X));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23677;}
      else {
        if ((0 == (3 & x_526X))) {
          goto L35339;}
        else {
          if ((3 == (3 & x_526X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_526X))) + -4))))>>2))))) {
              goto L35339;}
            else {
              goto L35342;}}
          else {
            goto L35342;}}}}
      break;
    case 81 : {
      x_527X = SvalS;
      if ((0 == (3 & x_527X))) {
        x_528X = ((x_527X)>>2);
        if ((x_528X < 0)) {
          arg0K0 = (~ x_528X);
          goto L35430;}
        else {
          arg0K0 = x_528X;
          goto L35430;}}
      else {
        if ((0 == (3 & x_527X))) {
          goto L35413;}
        else {
          if ((3 == (3 & x_527X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_527X))) + -4))))>>2))))) {
              goto L35413;}
            else {
              goto L35416;}}
          else {
            goto L35416;}}}}
      break;
    case 82 : {
      SstackS = ((SstackS) + 4);
      x_529X = *((long *) (SstackS));
      y_530X = SvalS;
      if ((0 == (3 & (x_529X | y_530X)))) {
        SvalS = (x_529X & y_530X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23677;}
      else {
        if ((0 == (3 & x_529X))) {
          goto L35515;}
        else {
          if ((3 == (3 & x_529X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_529X))) + -4))))>>2))))) {
              goto L35515;}
            else {
              goto L35524;}}
          else {
            goto L35524;}}}}
      break;
    case 83 : {
      SstackS = ((SstackS) + 4);
      x_531X = *((long *) (SstackS));
      y_532X = SvalS;
      if ((0 == (3 & (x_531X | y_532X)))) {
        SvalS = (x_531X | y_532X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23677;}
      else {
        if ((0 == (3 & x_531X))) {
          goto L35678;}
        else {
          if ((3 == (3 & x_531X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_531X))) + -4))))>>2))))) {
              goto L35678;}
            else {
              goto L35687;}}
          else {
            goto L35687;}}}}
      break;
    case 84 : {
      SstackS = ((SstackS) + 4);
      x_533X = *((long *) (SstackS));
      y_534X = SvalS;
      if ((0 == (3 & (x_533X | y_534X)))) {
        SvalS = (x_533X ^ y_534X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23677;}
      else {
        if ((0 == (3 & x_533X))) {
          goto L35841;}
        else {
          if ((3 == (3 & x_533X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_533X))) + -4))))>>2))))) {
              goto L35841;}
            else {
              goto L35850;}}
          else {
            goto L35850;}}}}
      break;
    case 85 : {
      SstackS = ((SstackS) + 4);
      x_535X = *((long *) (SstackS));
      y_536X = SvalS;
      if ((3 == (3 & y_536X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_536X))) + -4))))>>2))))) {
          v_537X = s48_bignum_test((((char *) (-3 + y_536X))));
          if ((1 == v_537X)) {
            push_exception_continuationB(6, 1);
            *((long *) (SstackS)) = x_535X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_536X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21732;}
          else {
            if ((0 == (3 & x_535X))) {
              if ((x_535X < 0)) {
                arg0K0 = -4;
                goto L32225;}
              else {
                arg0K0 = 0;
                goto L32225;}}
            else {
              if ((3 == (3 & x_535X))) {
                if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_535X))) + -4))))>>2))))) {
                  v_538X = s48_bignum_test((((char *) (-3 + x_535X))));
                  if ((1 == v_538X)) {
                    arg0K0 = 0;
                    goto L32241;}
                  else {
                    arg0K0 = -4;
                    goto L32241;}}
                else {
                  goto L32242;}}
              else {
                goto L32242;}}}}
        else {
          goto L42070;}}
      else {
        goto L42070;}}
      break;
    case 86 : {
      x_539X = SvalS;
      if ((9 == (255 & x_539X))) {
        arg0K0 = 5;
        goto L47951;}
      else {
        arg0K0 = 1;
        goto L47951;}}
      break;
    case 87 : {
      SstackS = ((SstackS) + 4);
      arg2_540X = *((long *) (SstackS));
      if ((9 == (255 & arg2_540X))) {
        if ((9 == (255 & (SvalS)))) {
          x_541X = SvalS;
          if ((arg2_540X == x_541X)) {
            arg0K0 = 5;
            goto L38964;}
          else {
            arg0K0 = 1;
            goto L38964;}}
        else {
          goto L38937;}}
      else {
        goto L38937;}}
      break;
    case 88 : {
      SstackS = ((SstackS) + 4);
      arg2_542X = *((long *) (SstackS));
      if ((9 == (255 & arg2_542X))) {
        if ((9 == (255 & (SvalS)))) {
          x_543X = SvalS;
          if ((arg2_542X < x_543X)) {
            arg0K0 = 5;
            goto L38874;}
          else {
            arg0K0 = 1;
            goto L38874;}}
        else {
          goto L38847;}}
      else {
        goto L38847;}}
      break;
    case 89 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23677;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21732;}}
      break;
    case 90 : {
      if ((0 == (3 & (SvalS)))) {
        x_544X = (((SvalS))>>2);
        if ((255 < x_544X)) {
          goto L43260;}
        else {
          if ((x_544X < 0)) {
            goto L43260;}
          else {
            SvalS = (9 + (((((x_544X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23677;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21732;}}
      break;
    case 91 : {
      x_545X = SvalS;
      if ((21 == x_545X)) {
        arg0K0 = 5;
        goto L47980;}
      else {
        arg0K0 = 1;
        goto L47980;}}
      break;
    case 92 : {
      x_546X = SvalS;
      type_547X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_546X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_546X))) + -4))))>>2))) == type_547X)) {
          arg0K0 = 5;
          goto L47994;}
        else {
          arg0K0 = 1;
          goto L47994;}}
      else {
        arg0K0 = 1;
        goto L47994;}}
      break;
    case 93 : {
      stob_548X = SvalS;
      type_549X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_548X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_548X))) + -4))))>>2))) == type_549X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_548X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          goto L26844;}}
      else {
        goto L26844;}}
      break;
    case 94 : {
      len_550X = *((unsigned char *) ((Scode_pointerS) + 1));
      s48_make_availableAgc((4 + (((len_550X)<<2))));
      type_551X = *((unsigned char *) ((Scode_pointerS) + 2));
      len_552X = ((len_550X)<<2);
      addr_553X = s48_allocate_small((4 + len_552X));
      *((long *) addr_553X) = (2 + (((((((len_552X)<<6)) + type_551X))<<2)));
      new_554X = 3 + (((long) (addr_553X + 4)));
      if ((len_550X < 1)) {
        goto L26949;}
      else {
        *((long *) ((((char *) (-3 + new_554X))) + (-4 + (((len_550X)<<2))))) = (SvalS);
        arg0K0 = (-2 + len_550X);
        goto L26933;}}
      break;
    case 95 : {
      SstackS = ((SstackS) + 4);
      len_555X = (((*((long *) (SstackS))))>>2);
      s48_make_availableAgc((4 + (((len_555X)<<2))));
      type_556X = *((unsigned char *) ((Scode_pointerS) + 1));
      len_557X = ((len_555X)<<2);
      addr_558X = s48_allocate_small((4 + len_557X));
      *((long *) addr_558X) = (2 + (((((((len_557X)<<6)) + type_556X))<<2)));
      new_559X = 3 + (((long) (addr_558X + 4)));
      SstackS = ((SstackS) + 4);
      stack_nargs_560X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      rest_list_561X = *((long *) (SstackS));
      arg0K0 = (-1 + stack_nargs_560X);
      goto L27065;}
      break;
    case 96 : {
      stob_562X = SvalS;
      offset_563X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_564X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_562X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_562X))) + -4))))>>2))) == type_564X)) {
          SvalS = (*((long *) ((((char *) (-3 + stob_562X))) + (((offset_563X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          goto L27225;}}
      else {
        goto L27225;}}
      break;
    case 97 : {
      SstackS = ((SstackS) + 4);
      stob_565X = *((long *) (SstackS));
      value_566X = SvalS;
      offset_567X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_568X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_565X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_565X))) + -4))))>>2))) == type_568X)) {
          if ((3 == (3 & stob_565X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_565X))) + -4)))))) {
              v_569X = *((unsigned char *) ((Scode_pointerS) + 3));
              if ((0 == v_569X)) {
                goto L27342;}
              else {
                if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                  goto L27342;}
                else {
                  merged_arg0K0 = stob_565X;
                  merged_arg0K1 = (((offset_567X)<<2));
                  merged_arg0K2 = value_566X;
                  proposal_d_write_return_tag = 0;
                  goto proposal_d_write;
                 proposal_d_write_return_0:
                  goto L27351;}}}
            else {
              goto L27352;}}
          else {
            goto L27352;}}
        else {
          goto L27352;}}
      else {
        goto L27352;}}
      break;
    case 98 : {
      SstackS = ((SstackS) + 4);
      len_570X = *((long *) (SstackS));
      init_571X = SvalS;
      type_572X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & len_570X))) {
        len_573X = ((len_570X)>>2);
        if ((len_573X < 0)) {
          goto L27569;}
        else {
          if ((4194304 < len_573X)) {
            goto L27569;}
          else {
            Stemp0S = init_571X;
            len_574X = ((len_573X)<<2);
            addr_575X = s48_allocate_tracedAgc((4 + len_574X));
            if ((addr_575X == NULL)) {
              arg0K0 = 1;
              goto L27586;}
            else {
              *((long *) addr_575X) = (2 + (((((((len_574X)<<6)) + type_572X))<<2)));
              arg0K0 = (3 + (((long) (addr_575X + 4))));
              goto L27586;}}}}
      else {
        push_exception_continuationB(5, 2);
        *((long *) (SstackS)) = (((type_572X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = len_570X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = init_571X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L21732;}}
      break;
    case 99 : {
      SstackS = ((SstackS) + 4);
      stob_576X = *((long *) (SstackS));
      index_577X = SvalS;
      type_578X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & index_577X))) {
        if ((3 == (3 & stob_576X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + stob_576X))) + -4))))>>2))) == type_578X)) {
            len_579X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_576X))) + -4))))>>8))))>>2);
            index_580X = ((index_577X)>>2);
            if ((index_580X < 0)) {
              goto L27859;}
            else {
              if ((index_580X < len_579X)) {
                v_581X = *((unsigned char *) ((Scode_pointerS) + 2));
                if ((0 == v_581X)) {
                  goto L27849;}
                else {
                  if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                    goto L27849;}
                  else {
                    merged_arg0K0 = stob_576X;
                    merged_arg0K1 = index_577X;
                    proposal_d_read_return_tag = 0;
                    goto proposal_d_read;
                   proposal_d_read_return_0:
                    v_582X = proposal_d_read0_return_value;
                    arg0K0 = v_582X;
                    goto L27858;}}}
              else {
                goto L27859;}}}
          else {
            goto L27814;}}
        else {
          goto L27814;}}
      else {
        goto L27814;}}
      break;
    case 100 : {
      SstackS = ((SstackS) + 4);
      arg2_583X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stob_584X = *((long *) (SstackS));
      value_585X = SvalS;
      type_586X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_583X))) {
        if ((3 == (3 & stob_584X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + stob_584X))) + -4))))>>2))) == type_586X)) {
            if ((3 == (3 & stob_584X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_584X))) + -4)))))) {
                len_587X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_584X))) + -4))))>>8))))>>2);
                index_588X = ((arg2_583X)>>2);
                if ((index_588X < 0)) {
                  goto L28146;}
                else {
                  if ((index_588X < len_587X)) {
                    v_589X = *((unsigned char *) ((Scode_pointerS) + 2));
                    if ((0 == v_589X)) {
                      goto L28136;}
                    else {
                      if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                        goto L28136;}
                      else {
                        merged_arg0K0 = stob_584X;
                        merged_arg0K1 = arg2_583X;
                        merged_arg0K2 = value_585X;
                        proposal_d_write_return_tag = 1;
                        goto proposal_d_write;
                       proposal_d_write_return_1:
                        goto L28145;}}}
                  else {
                    goto L28146;}}}
              else {
                goto L28099;}}
            else {
              goto L28099;}}
          else {
            goto L28099;}}
        else {
          goto L28099;}}
      else {
        goto L28099;}}
      break;
    case 101 : {
      SstackS = ((SstackS) + 4);
      arg2_590X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_590X | (SvalS))))) {
        len_591X = ((arg2_590X)>>2);
        init_592X = (((SvalS))>>2);
        if ((len_591X < 0)) {
          goto L36062;}
        else {
          if ((4194304 < ((((3 + len_591X))>>2)))) {
            goto L36062;}
          else {
            addr_593X = s48_allocate_untracedAgc((4 + len_591X));
            if ((addr_593X == NULL)) {
              arg0K0 = 1;
              goto L36075;}
            else {
              *((long *) addr_593X) = (74 + (((len_591X)<<8)));
              arg0K0 = (3 + (((long) (addr_593X + 4))));
              goto L36075;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_590X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
      break;
    case 102 : {
      obj_594X = SvalS;
      if ((3 == (3 & obj_594X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + obj_594X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          goto L46765;}}
      else {
        goto L46765;}}
      break;
    case 103 : {
      SstackS = ((SstackS) + 4);
      arg2_595X = *((long *) (SstackS));
      if ((3 == (3 & arg2_595X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_595X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_596X = (((SvalS))>>2);
            len_597X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_595X))) + -4))))>>8);
            if ((index_596X < 0)) {
              goto L43366;}
            else {
              if ((index_596X < len_597X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_595X))) + index_596X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23677;}
              else {
                goto L43366;}}}
          else {
            goto L46281;}}
        else {
          goto L46281;}}
      else {
        goto L46281;}}
      break;
    case 104 : {
      SstackS = ((SstackS) + 4);
      arg2_598X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_599X = *((long *) (SstackS));
      if ((3 == (3 & arg3_599X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_599X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_598X | (SvalS))))) {
            index_600X = ((arg2_598X)>>2);
            Kchar_601X = (((SvalS))>>2);
            if ((3 == (3 & arg3_599X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_599X))) + -4)))))) {
                len_602X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_599X))) + -4))))>>8);
                if ((index_600X < 0)) {
                  goto L40713;}
                else {
                  if ((index_600X < len_602X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_599X))) + index_600X)) = Kchar_601X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L23677;}
                  else {
                    goto L40713;}}}
              else {
                goto L40692;}}
            else {
              goto L40692;}}
          else {
            goto L45061;}}
        else {
          goto L45061;}}
      else {
        goto L45061;}}
      break;
    case 105 : {
      SstackS = ((SstackS) + 4);
      arg2_603X = *((long *) (SstackS));
      if ((0 == (3 & arg2_603X))) {
        if ((9 == (255 & (SvalS)))) {
          len_604X = ((arg2_603X)>>2);
          init_605X = ((((SvalS))>>8));
          if ((len_604X < 0)) {
            goto L36242;}
          else {
            if ((4194304 < ((((4 + len_604X))>>2)))) {
              goto L36242;}
            else {
              len_606X = 1 + len_604X;
              addr_607X = s48_allocate_untracedAgc((4 + len_606X));
              if ((addr_607X == NULL)) {
                arg0K0 = 1;
                goto L36255;}
              else {
                *((long *) addr_607X) = (70 + (((len_606X)<<8)));
                arg0K0 = (3 + (((long) (addr_607X + 4))));
                goto L36255;}}}}
        else {
          goto L43447;}}
      else {
        goto L43447;}}
      break;
    case 106 : {
      obj_608X = SvalS;
      if ((3 == (3 & obj_608X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_608X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          goto L46815;}}
      else {
        goto L46815;}}
      break;
    case 107 : {
      SstackS = ((SstackS) + 4);
      arg2_609X = *((long *) (SstackS));
      if ((3 == (3 & arg2_609X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_609X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_610X = (((SvalS))>>2);
            len_611X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_609X))) + -4))))>>8));
            if ((index_610X < 0)) {
              goto L43510;}
            else {
              if ((index_610X < len_611X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_609X))) + index_610X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23677;}
              else {
                goto L43510;}}}
          else {
            goto L46377;}}
        else {
          goto L46377;}}
      else {
        goto L46377;}}
      break;
    case 108 : {
      SstackS = ((SstackS) + 4);
      arg2_612X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_613X = *((long *) (SstackS));
      if ((3 == (3 & arg3_613X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_613X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_612X))) {
            if ((9 == (255 & (SvalS)))) {
              index_614X = ((arg2_612X)>>2);
              Kchar_615X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_613X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_613X))) + -4)))))) {
                  len_616X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_613X))) + -4))))>>8));
                  if ((index_614X < 0)) {
                    goto L40868;}
                  else {
                    if ((index_614X < len_616X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_613X))) + index_614X)) = (Kchar_615X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L23677;}
                    else {
                      goto L40868;}}}
                else {
                  goto L40847;}}
              else {
                goto L40847;}}
            else {
              goto L45187;}}
          else {
            goto L45187;}}
        else {
          goto L45187;}}
      else {
        goto L45187;}}
      break;
    case 109 : {
      s48_make_availableAgc(12);
      obj_617X = SvalS;
      if ((3 == (3 & obj_617X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_617X))) + -4))))>>2))))) {
          table_618X = Sthe_symbol_tableS;
          string_619X = SvalS;
          n_620X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_619X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L20287;}
        else {
          goto L32387;}}
      else {
        goto L32387;}}
      break;
    case 110 : {
      obj_621X = SvalS;
      if ((3 == (3 & obj_621X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_621X))) + -4))))>>2))))) {
          x_622X = SvalS;
          descriptor_623X = *((long *) ((((char *) (-3 + x_622X))) + 4));
          if ((17 == (255 & descriptor_623X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_622X))) + 4))))) {
              arg0K0 = 5;
              goto L44751;}
            else {
              arg0K0 = 1;
              goto L44751;}}
          else {
            arg0K0 = 5;
            goto L44751;}}
        else {
          goto L44732;}}
      else {
        goto L44732;}}
      break;
    case 111 : {
      SstackS = ((SstackS) + 4);
      arg2_624X = *((long *) (SstackS));
      if ((3 == (3 & arg2_624X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_624X))) + -4))))>>2))))) {
          x_625X = SvalS;
          if ((1 == x_625X)) {
            goto L40455;}
          else {
            if ((5 == x_625X)) {
              goto L40455;}
            else {
              goto L40462;}}}
        else {
          goto L40462;}}
      else {
        goto L40462;}}
      break;
    case 112 : {
      x_626X = SvalS;
      if ((3 == (3 & x_626X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_626X))) + -4)))))) {
          arg0K0 = 1;
          goto L48066;}
        else {
          arg0K0 = 5;
          goto L48066;}}
      else {
        arg0K0 = 5;
        goto L48066;}}
      break;
    case 113 : {
      x_627X = SvalS;
      if ((3 == (3 & x_627X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_627X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_627X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_627X))) + -4))));
          goto L43592;}
        else {
          goto L43592;}}
      else {
        goto L43592;}}
      break;
    case 114 : {
      s48_make_availableAgc(288);
      SstackS = ((SstackS) + 4);
      arg2_628X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        mode_629X = (((SvalS))>>2);
        if ((1 == mode_629X)) {
          goto L36649;}
        else {
          if ((2 == mode_629X)) {
            goto L36649;}
          else {
            if ((3 == mode_629X)) {
              goto L36649;}
            else {
              if ((4 == mode_629X)) {
                goto L36649;}
              else {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = arg2_628X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = (((mode_629X)<<2));
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21732;}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_628X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
      break;
    case 115 : {
      s48_make_availableAgc(264);
      obj_630X = SvalS;
      if ((3 == (3 & obj_630X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_630X))) + -4))))>>2))))) {
          channel_631X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_631X))))))) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = channel_631X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21732;}
          else {
            status_632X = close_channelB(channel_631X);
            if ((status_632X == NO_ERRORS)) {
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23677;}
            else {
              push_exception_continuationB(24, 1);
              *((long *) (SstackS)) = channel_631X;
              SstackS = ((SstackS) + -4);
              merged_arg0K0 = status_632X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 0;
              goto get_error_string;
             get_error_string_return_0:
              x_633X = get_error_string0_return_value;
              *((long *) (SstackS)) = x_633X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21732;}}}
        else {
          goto L48100;}}
      else {
        goto L48100;}}
      break;
    case 116 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_634X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_635X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_636X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_637X = *((long *) (SstackS));
      if ((3 == (3 & arg5_637X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg5_637X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg3_635X | arg2_634X)))) {
            x_638X = SvalS;
            if ((1 == x_638X)) {
              goto L43717;}
            else {
              if ((5 == x_638X)) {
                goto L43717;}
              else {
                goto L43728;}}}
          else {
            goto L43728;}}
        else {
          goto L43728;}}
      else {
        goto L43728;}}
      break;
    case 117 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_639X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_640X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_641X = *((long *) (SstackS));
      if ((3 == (3 & arg4_641X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg4_641X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_639X | (SvalS))))) {
            start_642X = ((arg2_639X)>>2);
            count_643X = (((SvalS))>>2);
            v_644X = 8 == (*((long *) (((char *) (-3 + arg4_641X)))));
            if (v_644X) {
              if ((3 == (3 & arg3_640X))) {
                if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_640X))) + -4))))>>2))))) {
                  goto L37288;}
                else {
                  goto L37463;}}
              else {
                goto L37463;}}
            else {
              goto L37342;}}
          else {
            goto L43923;}}
        else {
          goto L43923;}}
      else {
        goto L43923;}}
      break;
    case 118 : {
      s48_make_availableAgc(264);
      obj_645X = SvalS;
      if ((3 == (3 & obj_645X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_645X))) + -4))))>>2))))) {
          channel_646X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_646X))))))) {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = channel_646X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21732;}
          else {
            readyP_647X = ps_check_fd(((((*((long *) ((((char *) (-3 + channel_646X))) + 8))))>>2)), (4 == (*((long *) (((char *) (-3 + channel_646X)))))), &status_648X);
            if ((status_648X == NO_ERRORS)) {
              if (readyP_647X) {
                arg0K0 = 5;
                goto L40046;}
              else {
                arg0K0 = 1;
                goto L40046;}}
            else {
              push_exception_continuationB(24, 1);
              *((long *) (SstackS)) = channel_646X;
              SstackS = ((SstackS) + -4);
              merged_arg0K0 = status_648X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 1;
              goto get_error_string;
             get_error_string_return_1:
              x_649X = get_error_string0_return_value;
              *((long *) (SstackS)) = x_649X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21732;}}}
        else {
          goto L48122;}}
      else {
        goto L48122;}}
      break;
    case 119 : {
      obj_650X = SvalS;
      if ((3 == (3 & obj_650X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_650X))) + -4))))>>2))))) {
          channel_651X = SvalS;
          head_652X = Spending_channels_headS;
          if ((1 == head_652X)) {
            addr_653X = (((char *) (-3 + channel_651X))) + 16;
            S48_WRITE_BARRIER(channel_651X, addr_653X, 1);
            *((long *) addr_653X) = 1;
            n_654X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_651X))) + 8))))>>2)));
            arg0K0 = (((n_654X)<<2));
            goto L44065;}
          else {
            if ((channel_651X == head_652X)) {
              channel_655X = Spending_channels_headS;
              next_656X = *((long *) ((((char *) (-3 + channel_655X))) + 12));
              Spending_channels_headS = next_656X;
              addr_657X = (((char *) (-3 + channel_655X))) + 12;
              S48_WRITE_BARRIER(channel_655X, addr_657X, 1);
              *((long *) addr_657X) = 1;
              if ((1 == next_656X)) {
                Spending_channels_tailS = 1;
                goto L10821;}
              else {
                goto L10821;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_652X))) + 12)));
              arg0K1 = head_652X;
              goto L10827;}}}
        else {
          goto L44048;}}
      else {
        goto L44048;}}
      break;
    case 120 : {
      s48_make_availableAgc(((((3 * (Snumber_of_channelsS)))<<2)));
      arg0K0 = (-1 + (Snumber_of_channelsS));
      arg0K1 = 25;
      goto L15775;}
      break;
    case 121 : {
      SvalS = (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 122 : {
      proposal_658X = SvalS;
      if ((1 == proposal_658X)) {
        goto L37596;}
      else {
        if ((3 == (3 & proposal_658X))) {
          if ((2 == (31 & ((((*((long *) ((((char *) (-3 + proposal_658X))) + -4))))>>2))))) {
            if ((4 == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proposal_658X))) + -4))))>>8))))>>2)))) {
              if ((1 == (*((long *) (((char *) (-3 + proposal_658X))))))) {
                goto L37596;}
              else {
                goto L37627;}}
            else {
              goto L37627;}}
          else {
            goto L37627;}}
        else {
          goto L37627;}}}
      break;
    case 123 : {
      proposal_659X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
      if ((1 == proposal_659X)) {
        push_exception_continuationB(26, 1);
        arg0K0 = 0;
        goto L21732;}
      else {
        GET_PROPOSAL_LOCK();
        log_660X = *((long *) ((((char *) (-3 + proposal_659X))) + 4));
        arg0K0 = 0;
        goto L9230;}}
      break;
    case 124 : {
      stob_661X = SvalS;
      offset_662X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_663X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_661X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_661X))) + -4))))>>2))) == type_663X)) {
          x_664X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
          if ((1 == x_664X)) {
            arg0K0 = (*((long *) ((((char *) (-3 + stob_661X))) + (((offset_662X)<<2)))));
            goto L28444;}
          else {
            merged_arg0K0 = stob_661X;
            merged_arg0K1 = (((offset_662X)<<2));
            proposal_d_read_return_tag = 1;
            goto proposal_d_read;
           proposal_d_read_return_1:
            v_665X = proposal_d_read0_return_value;
            arg0K0 = v_665X;
            goto L28444;}}
        else {
          goto L28445;}}
      else {
        goto L28445;}}
      break;
    case 125 : {
      SstackS = ((SstackS) + 4);
      arg2_666X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_667X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_668X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_669X = *((long *) (SstackS));
      if ((0 == (3 & ((arg4_668X | arg2_666X) | (SvalS))))) {
        from_index_670X = ((arg4_668X)>>2);
        to_index_671X = ((arg2_666X)>>2);
        count_672X = (((SvalS))>>2);
        v_673X = *((unsigned char *) ((Scode_pointerS) + 1));
        merged_arg0K0 = arg5_669X;
        merged_arg0K1 = from_index_670X;
        merged_arg0K2 = count_672X;
        okay_copy_argsP_return_tag = 0;
        goto okay_copy_argsP;
       okay_copy_argsP_return_0:
        v_674X = okay_copy_argsP0_return_value;
        if (v_674X) {
          merged_arg0K0 = arg3_667X;
          merged_arg0K1 = to_index_671X;
          merged_arg0K2 = count_672X;
          okay_copy_argsP_return_tag = 1;
          goto okay_copy_argsP;
         okay_copy_argsP_return_1:
          v_675X = okay_copy_argsP0_return_value;
          if (v_675X) {
            if ((3 == (3 & arg3_667X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_667X))) + -4)))))) {
                if ((count_672X < 0)) {
                  goto L28538;}
                else {
                  if ((0 == v_673X)) {
                    goto L28598;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L28598;}
                    else {
                      arg0K0 = 4096;
                      arg0K1 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 12)));
                      goto L28686;}}}}
              else {
                goto L28538;}}
            else {
              goto L28538;}}
          else {
            goto L28538;}}
        else {
          goto L28538;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg5_669X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_668X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_667X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_666X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 5;
        goto L21732;}}
      break;
    case 126 : {
      SstackS = ((SstackS) + 4);
      arg2_676X = *((long *) (SstackS));
      if ((3 == (3 & arg2_676X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_676X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_677X = (((SvalS))>>2);
            len_678X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_676X))) + -4))))>>8);
            if ((index_677X < 0)) {
              goto L28895;}
            else {
              if ((index_677X < len_678X)) {
                x_679X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                if ((1 == x_679X)) {
                  arg0K0 = ((((*((unsigned char *) ((((char *) (-3 + arg2_676X))) + index_677X))))<<2));
                  goto L28894;}
                else {
                  index_680X = ((index_677X)<<2);
                  log_681X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                  arg0K0 = 0;
                  goto L14340;}}
              else {
                goto L28895;}}}
          else {
            goto L31693;}}
        else {
          goto L31693;}}
      else {
        goto L31693;}}
      break;
    case 127 : {
      SstackS = ((SstackS) + 4);
      arg2_682X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_683X = *((long *) (SstackS));
      if ((3 == (3 & arg3_683X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_683X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_682X))) {
            index_684X = ((arg2_682X)>>2);
            byte_685X = SvalS;
            if ((3 == (3 & arg3_683X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_683X))) + -4)))))) {
                if ((0 == (3 & byte_685X))) {
                  len_686X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_683X))) + -4))))>>8);
                  if ((index_684X < 0)) {
                    goto L29021;}
                  else {
                    if ((index_684X < len_686X)) {
                      x_687X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                      if ((1 == x_687X)) {
                        *((unsigned char *) ((((char *) (-3 + arg3_683X))) + index_684X)) = (((byte_685X)>>2));
                        goto L29020;}
                      else {
                        index_688X = ((index_684X)<<2);
                        log_689X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                        arg0K0 = 0;
                        goto L14516;}}
                    else {
                      goto L29021;}}}
                else {
                  goto L28986;}}
              else {
                goto L28986;}}
            else {
              goto L28986;}}
          else {
            goto L31796;}}
        else {
          goto L31796;}}
      else {
        goto L31796;}}
      break;
    case 128 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 129 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 130 : {
      x_690X = SvalS;
      push_exception_continuationB(15, 1);
      *((long *) (SstackS)) = x_690X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
      break;
    case 131 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 132 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 133 : {
      s48_make_availableAgc(264);
      SstackS = ((SstackS) + 4);
      arg2_691X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_692X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_693X = *((long *) (SstackS));
      if ((3 == (3 & arg4_693X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_693X))) + -4))))>>2))))) {
          if ((3 == (3 & arg2_691X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_691X))) + -4))))>>2))))) {
              obj_694X = SvalS;
              if ((3 == (3 & obj_694X))) {
                if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_694X))) + -4))))>>2))))) {
                  undumpables_695X = SvalS;
                  port_696X = ps_open_output_file((((char *)(((char *) (-3 + arg4_693X))))), &status_697X);
                  if ((status_697X == NO_ERRORS)) {
                    status_698X = ps_write_string((((char *)(((char *) (-3 + arg2_691X))))), port_696X);
                    if ((status_698X == NO_ERRORS)) {
                      status_699X = s48_write_image(arg3_692X, undumpables_695X, port_696X);
                      if ((status_699X == NO_ERRORS)) {
                        status_700X = ps_close(port_696X);
                        if ((status_700X == NO_ERRORS)) {
                          SvalS = 13;
                          Scode_pointerS = ((Scode_pointerS) + 1);
                          arg1K0 = (Scode_pointerS);
                          goto L23677;}
                        else {
                          arg0K0 = 24;
                          arg0K1 = status_700X;
                          goto L41096;}}
                      else {
                        status_701X = ps_close(port_696X);
                        if ((status_701X == NO_ERRORS)) {
                          arg0K0 = 24;
                          arg0K1 = status_699X;
                          goto L41096;}
                        else {
                          ps_write_string("Unable to close image file", (stderr));
                          { long ignoreXX;
                          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                          arg0K0 = 24;
                          arg0K1 = status_699X;
                          goto L41096;}}}
                    else {
                      status_702X = ps_close(port_696X);
                      if ((status_702X == NO_ERRORS)) {
                        arg0K0 = 24;
                        arg0K1 = status_698X;
                        goto L41096;}
                      else {
                        ps_write_string("Unable to close image file", (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg0K0 = 24;
                        arg0K1 = status_698X;
                        goto L41096;}}}
                  else {
                    arg0K0 = 10;
                    arg0K1 = status_697X;
                    goto L41096;}}
                else {
                  goto L45384;}}
              else {
                goto L45384;}}
            else {
              goto L45384;}}
          else {
            goto L45384;}}
        else {
          goto L45384;}}
      else {
        goto L45384;}}
      break;
    case 134 : {
      SvalS = 13;
      s48_collect();
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 135 : {
      obj_703X = SvalS;
      if ((3 == (3 & obj_703X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_703X))) + -4))))>>2))))) {
          x_704X = SvalS;
          n_705X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_704X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L46025;}
        else {
          goto L46002;}}
      else {
        goto L46002;}}
      break;
    case 136 : {
      s48_make_availableAgc(24);
      SstackS = ((SstackS) + 4);
      stob_706X = *((long *) (SstackS));
      proc_707X = SvalS;
      if ((3 == (3 & stob_706X))) {
        if ((3 == (3 & proc_707X))) {
          if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_707X))) + -4))))>>2))))) {
            GET_PROPOSAL_LOCK();
            addr_708X = s48_allocate_small(12);
            *((long *) addr_708X) = 2050;
            x_709X = 3 + (((long) (addr_708X + 4)));
            *((long *) (((char *) (-3 + x_709X)))) = stob_706X;
            *((long *) ((((char *) (-3 + x_709X))) + 4)) = proc_707X;
            b_710X = SHARED_REF((Sfinalizer_alistS));
            addr_711X = s48_allocate_small(12);
            *((long *) addr_711X) = 2050;
            x_712X = 3 + (((long) (addr_711X + 4)));
            *((long *) (((char *) (-3 + x_712X)))) = x_709X;
            *((long *) ((((char *) (-3 + x_712X))) + 4)) = b_710X;
            SHARED_SETB((Sfinalizer_alistS), x_712X);
            RELEASE_PROPOSAL_LOCK();
            SvalS = 13;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23677;}
          else {
            goto L41371;}}
        else {
          goto L41371;}}
      else {
        goto L41371;}}
      break;
    case 137 : {
      SstackS = ((SstackS) + 4);
      arg2_713X = *((long *) (SstackS));
      if ((0 == (3 & arg2_713X))) {
        key_714X = ((arg2_713X)>>2);
        other_715X = SvalS;
        if ((5 == key_714X)) {
          SvalS = (-4 & other_715X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          if ((0 == key_714X)) {
            x_716X = s48_available();
            SvalS = (((x_716X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23677;}
          else {
            if ((1 == key_714X)) {
              bytes_717X = s48_heap_size();
              SvalS = (-4 & (3 + bytes_717X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23677;}
            else {
              if ((2 == key_714X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23677;}
              else {
                if ((3 == key_714X)) {
                  x_718X = s48_gc_count();
                  SvalS = (((x_718X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L23677;}
                else {
                  if ((4 == key_714X)) {
                    push_exception_continuationB(14, 1);
                    *((long *) (SstackS)) = (((key_714X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_715X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L21732;}
                  else {
                    push_exception_continuationB(17, 1);
                    *((long *) (SstackS)) = (((key_714X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_715X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L21732;}}}}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_713X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
      break;
    case 138 : {
      if ((0 == (3 & (SvalS)))) {
        type_719X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L44184;}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21732;}}
      break;
    case 139 : {
      x_720X = SvalS;
      arg2K0 = 1;
      arg0K1 = x_720X;
      goto L48224;}
      break;
    case 140 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 141 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 142 : {
      val_721X = SHARED_REF((Ssession_dataS));
      SvalS = val_721X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 143 : {
      SHARED_SETB((Ssession_dataS), (SvalS));
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 144 : {
      obj_722X = SvalS;
      if ((3 == (3 & obj_722X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_722X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 166)) {
            goto L26139;}
          else {
            temp_723X = SHARED_REF((Sexception_handlersS));
            SHARED_SETB((Sexception_handlersS), (SvalS));
            SvalS = temp_723X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23677;}}
        else {
          goto L26139;}}
      else {
        goto L26139;}}
      break;
    case 145 : {
      SstackS = ((SstackS) + 4);
      p_724X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      template_725X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      p_726X = *((long *) (SstackS));
      StemplateS = template_725X;
      Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_725X)))))))) + ((((((((((p_724X)>>2)) + (((p_726X)>>2))))<<2)))>>2)));
      arg1K0 = (Scode_pointerS);
      goto L23677;}
      break;
    case 146 : {
      obj_727X = SvalS;
      if ((3 == (3 & obj_727X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_727X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 5)) {
            goto L24603;}
          else {
            temp_728X = SHARED_REF((Sinterrupt_handlersS));
            SHARED_SETB((Sinterrupt_handlersS), (SvalS));
            SvalS = temp_728X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23677;}}
        else {
          goto L24603;}}
      else {
        goto L24603;}}
      break;
    case 147 : {
      old_729X = Senabled_interruptsS;
      p_730X = SvalS;
      Senabled_interruptsS = (((p_730X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L48272;}
        else {
          goto L48272;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L48272;}}
      break;
    case 148 : {
      SstackS = ((SstackS) + 4);
      p_731X = *((long *) (SstackS));
      Senabled_interruptsS = (((p_731X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L23691;}
        else {
          goto L23691;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L23691;}}
      break;
    case 149 : {
      if ((0 == (3 & (SvalS)))) {
        p_732X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          s48_Spending_interruptPS = 0;
          if ((s48_Spending_eventsPS)) {
            s48_Spending_interruptPS = 1;
            goto L46950;}
          else {
            goto L46950;}}
        else {
          s48_Spending_interruptPS = 1;
          goto L46950;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L21732;}}
      break;
    case 150 : {
      SstackS = ((SstackS) + 4);
      arg2_733X = *((long *) (SstackS));
      if ((0 == (3 & arg2_733X))) {
        x_734X = SvalS;
        if ((1 == x_734X)) {
          goto L44934;}
        else {
          if ((5 == x_734X)) {
            goto L44934;}
          else {
            goto L44939;}}}
      else {
        goto L44939;}}
      break;
    case 151 : {
      SstackS = ((SstackS) + 4);
      nargs_735X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      p_736X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      rest_list_737X = *((long *) (SstackS));
      if ((14 < nargs_735X)) {
        push_exception_continuationB(19, 1);
        *((long *) (SstackS)) = (*((long *) ((SstackS) + (-4 & p_736X))));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = nargs_735X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}
      else {
        arg0K0 = rest_list_737X;
        goto L29220;}}
      break;
    case 152 : {
      s48_make_availableAgc(20);
      SstackS = ((SstackS) + 4);
      arg2_738X = *((long *) (SstackS));
      if ((3 == (3 & arg2_738X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_738X))) + -4))))>>2))))) {
          x_739X = SvalS;
          if ((1 == x_739X)) {
            goto L44279;}
          else {
            if ((5 == x_739X)) {
              goto L44279;}
            else {
              goto L44284;}}}
        else {
          goto L44284;}}
      else {
        goto L44284;}}
      break;
    case 153 : {
      SstackS = ((SstackS) + 4);
      arg2_740X = *((long *) (SstackS));
      if ((3 == (3 & arg2_740X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_740X))) + -4))))>>2))))) {
          x_741X = SvalS;
          if ((1 == x_741X)) {
            goto L31031;}
          else {
            if ((5 == x_741X)) {
              goto L31031;}
            else {
              goto L31036;}}}
        else {
          goto L31036;}}
      else {
        goto L31036;}}
      break;
    case 154 : {
      SstackS = ((SstackS) + 4);
      arg2_742X = *((long *) (SstackS));
      if ((0 == (3 & arg2_742X))) {
        option_743X = ((arg2_742X)>>2);
        other_744X = SvalS;
        if ((2 == option_743X)) {
          x_745X = CHEAP_TIME();
          SvalS = (((x_745X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          if ((0 == option_743X)) {
            seconds_746X = s48_run_time(&mseconds_747X);
            arg0K0 = option_743X;
            arg0K1 = seconds_746X;
            arg0K2 = mseconds_747X;
            goto L45936;}
          else {
            if ((1 == option_743X)) {
              seconds_748X = s48_real_time(&mseconds_749X);
              arg0K0 = option_743X;
              arg0K1 = seconds_748X;
              arg0K2 = mseconds_749X;
              goto L45936;}
            else {
              push_exception_continuationB(17, 1);
              *((long *) (SstackS)) = (((option_743X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = other_744X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L21732;}}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_742X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
      break;
    case 155 : {
      SstackS = ((SstackS) + 4);
      arg2_750X = *((long *) (SstackS));
      if ((0 == (3 & arg2_750X))) {
        key_751X = ((arg2_750X)>>2);
        value_752X = SvalS;
        status_753X = s48_extended_vm(key_751X, value_752X);
        if ((0 == status_753X)) {
          SvalS = (s48_Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          if ((1 == status_753X)) {
            push_exception_continuationB(22, 1);
            *((long *) (SstackS)) = (((key_751X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_752X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21732;}
          else {
            push_exception_continuationB(23, 1);
            *((long *) (SstackS)) = (((key_751X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_752X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21732;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_750X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
      break;
    case 156 : {
      SstackS = ((SstackS) + 4);
      arg2_754X = *((long *) (SstackS));
      x_755X = SvalS;
      Senabled_interruptsS = -1;
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L48332;}
        else {
          goto L48332;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L48332;}}
      break;
    case 157 : {
      SstackS = ((SstackS) + 4);
      arg2_756X = *((long *) (SstackS));
      if ((3 == (3 & arg2_756X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_756X))) + -4))))>>2))))) {
          obj_757X = SvalS;
          if ((3 == (3 & obj_757X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_757X))) + -4))))>>2))))) {
              x_758X = SvalS;
              len_759X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_756X))) + -4))))>>8);
              if ((len_759X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_758X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_758X))), (void *)(((char *) (-3 + arg2_756X))),len_759X)))) {
                  arg0K0 = 5;
                  goto L38640;}
                else {
                  arg0K0 = 1;
                  goto L38640;}}
              else {
                arg0K0 = 1;
                goto L38640;}}
            else {
              goto L38607;}}
          else {
            goto L38607;}}
        else {
          goto L38607;}}
      else {
        goto L38607;}}
      break;
    case 158 : {
      s48_make_availableAgc((4 + (-4 & (4 + ((((SvalS))>>2))))));
      SstackS = ((SstackS) + 4);
      arg2_760X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        n_761X = (((SvalS))>>2);
        if ((3 == (3 & arg2_760X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_760X))) + -4))))>>2))))) {
            goto L38342;}
          else {
            goto L38294;}}
        else {
          goto L38294;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_760X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
      break;
    case 159 : {
      SstackS = ((SstackS) + 4);
      thing_762X = *((long *) (SstackS));
      list_763X = SvalS;
      arg0K0 = list_763X;
      arg0K1 = list_763X;
      arg2K2 = 1;
      goto L40109;}
      break;
    case 160 : {
      SstackS = ((SstackS) + 4);
      arg2_764X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_765X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        index_766X = (((SvalS))>>2);
        if ((3 == (3 & arg3_765X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg3_765X))) + -4))))>>2))))) {
            if ((arg2_764X == (*((long *) (((char *) (-3 + arg3_765X))))))) {
              len_767X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_765X))) + -4))))>>8))))>>2);
              if ((index_766X < 0)) {
                goto L29562;}
              else {
                if ((index_766X < len_767X)) {
                  v_768X = *((unsigned char *) ((Scode_pointerS) + 1));
                  if ((0 == v_768X)) {
                    goto L29552;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L29552;}
                    else {
                      merged_arg0K0 = arg3_765X;
                      merged_arg0K1 = (((index_766X)<<2));
                      proposal_d_read_return_tag = 2;
                      goto proposal_d_read;
                     proposal_d_read_return_2:
                      v_769X = proposal_d_read0_return_value;
                      arg0K0 = v_769X;
                      goto L29561;}}}
                else {
                  goto L29562;}}}
            else {
              goto L29582;}}
          else {
            goto L29582;}}
        else {
          goto L29582;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg3_765X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_764X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L21732;}}
      break;
    case 161 : {
      SstackS = ((SstackS) + 4);
      arg2_770X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_771X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_772X = *((long *) (SstackS));
      if ((0 == (3 & arg2_770X))) {
        index_773X = ((arg2_770X)>>2);
        value_774X = SvalS;
        if ((3 == (3 & arg4_772X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg4_772X))) + -4))))>>2))))) {
            if ((arg3_771X == (*((long *) (((char *) (-3 + arg4_772X))))))) {
              if ((3 == (3 & arg4_772X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_772X))) + -4)))))) {
                  len_775X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_772X))) + -4))))>>8))))>>2);
                  if ((index_773X < 0)) {
                    goto L29820;}
                  else {
                    if ((index_773X < len_775X)) {
                      v_776X = *((unsigned char *) ((Scode_pointerS) + 1));
                      if ((0 == v_776X)) {
                        goto L29810;}
                      else {
                        if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                          goto L29810;}
                        else {
                          merged_arg0K0 = arg4_772X;
                          merged_arg0K1 = (((index_773X)<<2));
                          merged_arg0K2 = value_774X;
                          proposal_d_write_return_tag = 2;
                          goto proposal_d_write;
                         proposal_d_write_return_2:
                          goto L29819;}}}
                    else {
                      goto L29820;}}}
                else {
                  goto L29842;}}
              else {
                goto L29842;}}
            else {
              goto L29842;}}
          else {
            goto L29842;}}
        else {
          goto L29842;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg4_772X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_771X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_770X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 4;
        goto L21732;}}
      break;
    case 162 : {
      v_777X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_777X)) {
        arg0K0 = (SvalS);
        goto L30099;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_778X = get_current_port0_return_value;
        arg0K0 = v_778X;
        goto L30099;}}
      break;
    case 163 : {
      v_779X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_779X)) {
        arg0K0 = (SvalS);
        goto L30340;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_780X = get_current_port0_return_value;
        arg0K0 = v_780X;
        goto L30340;}}
      break;
    case 164 : {
      v_781X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_781X)) {
        v_782X = SvalS;
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = v_782X;
        goto L30574;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_783X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_783X;
        goto L30574;}}
      break;
    case 165 : {
      x_784X = SvalS;
      out_785X = stderr;
      arg0K0 = x_784X;
      goto L40362;}
      break;
  }}
 L21732: {
  nargs_786X = arg0K0;
  opcode_787X = (((*((long *) ((SstackS) + (8 + (((nargs_786X)<<2)))))))>>2);
  handlers_788X = SHARED_REF((Sexception_handlersS));
  if ((3 == (3 & handlers_788X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_788X))) + -4))))>>2))))) {
      goto L21795;}
    else {
      goto L21868;}}
  else {
    goto L21868;}}
 L30949: {
  env_789X = arg0K0;
  i_790X = arg0K1;
  if ((0 == i_790X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_789X))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 3);
      arg0K0 = 0;
      goto L21732;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23677;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_789X)))));
    arg0K1 = (-1 + i_790X);
    goto L30949;}}
 L25644: {
  env_791X = arg0K0;
  i_792X = arg0K1;
  if ((0 == i_792X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_791X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 5);
      arg0K0 = 0;
      goto L21732;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23677;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_791X)))));
    arg0K1 = (-1 + i_792X);
    goto L25644;}}
 L25582: {
  env_793X = arg0K0;
  i_794X = arg0K1;
  if ((0 == i_794X)) {
    index_795X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
    value_796X = SvalS;
    addr_797X = (((char *) (-3 + env_793X))) + (((index_795X)<<2));
    S48_WRITE_BARRIER(env_793X, addr_797X, value_796X);
    *((long *) addr_797X) = value_796X;
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 5);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_793X)))));
    arg0K1 = (-1 + i_794X);
    goto L25582;}}
 L26319: {
  v_798X = arg0K0;
  s48_make_availableAgc((12 + (((v_798X)<<2))));
  p_799X = SenvS;
  if ((3 == (3 & p_799X))) {
    if ((p_799X < (((long) (Sstack_beginS))))) {
      goto L26373;}
    else {
      if (((((long) (Sstack_endS))) < p_799X)) {
        goto L26373;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = 0;
        merged_arg0K3 = 0;
        save_env_in_heap_return_tag = 0;
        goto save_env_in_heap;
       save_env_in_heap_return_0:
        v_800X = save_env_in_heap0_return_value;
        SenvS = v_800X;
        goto L26373;}}}
  else {
    goto L26373;}}
 L26332: {
  env_801X = arg0K0;
  key_802X = arg0K1;
  merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  merged_arg0K1 = env_801X;
  merged_arg0K2 = key_802X;
  make_closure_return_tag = 0;
  goto make_closure;
 make_closure_return_0:
  value_803X = make_closure0_return_value;
  SvalS = value_803X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L25369: {
  i_804X = arg0K0;
  if ((count_405X < i_804X)) {
    value_805X = *((long *) (((char *) (-3 + (SvalS)))));
    addr_806X = ((char *) (-3 + vector_408X));
    S48_WRITE_BARRIER(vector_408X, addr_806X, value_805X);
    *((long *) addr_806X) = value_805X;
    x_807X = SvalS;
    addr_808X = ((char *) (-3 + x_807X));
    S48_WRITE_BARRIER(x_807X, addr_808X, vector_408X);
    *((long *) addr_808X) = vector_408X;
    *((long *) (SstackS)) = (SenvS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (1034 + (((count_405X)<<10)));
    SstackS = ((SstackS) + -4);
    SenvS = (3 + (((long) ((SstackS) + 8))));
    Scode_pointerS = ((Scode_pointerS) + (3 + (((count_405X)<<1))));
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    index_809X = ((i_804X)<<1);
    merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + index_809X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_809X))))))<<2)))));
    merged_arg0K1 = (SvalS);
    merged_arg0K2 = 0;
    make_closure_return_tag = 1;
    goto make_closure;
   make_closure_return_1:
    closure_810X = make_closure0_return_value;
    addr_811X = (((char *) (-3 + vector_408X))) + (((i_804X)<<2));
    S48_WRITE_BARRIER(vector_408X, addr_811X, closure_810X);
    *((long *) addr_811X) = closure_810X;
    *((long *) (SstackS)) = closure_810X;
    SstackS = ((SstackS) + -4);
    arg0K0 = (1 + i_804X);
    goto L25369;}}
 L26656: {
  start_i_812X = arg0K0;
  arg0K0 = start_i_812X;
  arg0K1 = 2;
  arg0K2 = (SenvS);
  goto L26664;}
 L26507: {
  start_i_813X = arg0K0;
  arg0K0 = start_i_813X;
  arg0K1 = 3;
  arg0K2 = (SenvS);
  goto L26515;}
 L47635: {
  value_814X = arg0K0;
  SvalS = value_814X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L24334: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 0;
      goto check_events;
     check_events_return_0:
      v_815X = check_events0_return_value;
      if (v_815X) {
        arg0K0 = stack_arg_count_424X;
        goto L21965;}
      else {
        goto L24338;}}
    else {
      arg0K0 = stack_arg_count_424X;
      goto L21965;}}
  else {
    goto L24338;}}
 L24344: {
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L7893: {
  loc_816X = arg1K0;
  arg_817X = arg1K1;
  if ((top_of_args_430X < arg_817X)) {
    *((long *) loc_816X) = (*((long *) arg_817X));
    arg1K0 = (loc_816X + -4);
    arg1K1 = (arg_817X + -4);
    goto L7893;}
  else {
    SstackS = loc_816X;
    s48_call_native_code(proc_429X);
    goto L24344;}}
 L21362: {
  stack_arg_count_818X = arg0K0;
  list_args_819X = arg0K1;
  list_arg_count_820X = arg0K2;
  code_821X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  total_arg_count_822X = stack_arg_count_818X + list_arg_count_820X;
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_821X))) + 1)));
  arg0K1 = 64;
  goto L21377;}
 L22234: {
  exception_823X = arg0K0;
  stack_arg_count_824X = arg0K1;
  list_args_825X = arg0K2;
  list_arg_count_826X = arg0K3;
  if (((StemplateS) == (SvalS))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      return v_827X;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      return v_828X;}}
  else {
    merged_arg0K0 = list_args_825X;
    merged_arg0K1 = list_arg_count_826X;
    copy_listSAgc_return_tag = 0;
    goto copy_listSAgc;
   copy_listSAgc_return_0:
    v_829X = copy_listSAgc0_return_value;
    merged_arg0K0 = v_829X;
    merged_arg0K1 = stack_arg_count_824X;
    pop_args_GlistSAgc_return_tag = 1;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_1:
    args_830X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(exception_823X, 0);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_830X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}}
 L22863: {
  stack_arg_count_831X = arg0K0;
  list_args_832X = arg0K1;
  list_arg_count_833X = arg0K2;
  if ((0 == list_arg_count_833X)) {
    obj_834X = SvalS;
    if ((3 == (3 & obj_834X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_834X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_831X;
        arg0K1 = 25;
        arg0K2 = 0;
        goto L21362;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_831X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22234;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_831X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L22234;}}
  else {
    obj_835X = SvalS;
    if ((3 == (3 & obj_835X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_835X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_831X;
        arg0K1 = list_args_832X;
        arg0K2 = list_arg_count_833X;
        goto L21362;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_831X;
        arg0K2 = list_args_832X;
        arg0K3 = list_arg_count_833X;
        goto L22234;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_831X;
      arg0K2 = list_args_832X;
      arg0K3 = list_arg_count_833X;
      goto L22234;}}}
 L14845: {
  list_args_836X = arg0K0;
  stack_nargs_837X = arg0K1;
  merged_arg0K0 = list_args_836X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_838X = okay_argument_list0_return_value;
  list_arg_count_839X = okay_argument_list1_return_value;
  if (okayP_838X) {
    SvalS = proc_439X;
    arg0K0 = stack_nargs_837X;
    arg0K1 = list_args_836X;
    arg0K2 = list_arg_count_839X;
    goto L22863;}
  else {
    *((long *) (SstackS)) = list_args_836X;
    SstackS = ((SstackS) + -4);
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_837X);
    pop_args_GlistSAgc_return_tag = 2;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_2:
    args_840X = pop_args_GlistSAgc0_return_value;
    SstackS = ((SstackS) + 4);
    proc_841X = *((long *) (SstackS));
    push_exception_continuationB(5, 0);
    *((long *) (SstackS)) = proc_841X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_840X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}}
 L14886: {
  list_842X = arg0K0;
  follower_843X = arg0K1;
  if ((25 == list_842X)) {
    list_args_844X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_843X))) + 4)))))));
    addr_845X = (((char *) (-3 + follower_843X))) + 4;
    S48_WRITE_BARRIER(follower_843X, addr_845X, list_args_844X);
    *((long *) addr_845X) = list_args_844X;
    arg0K0 = rest_list_440X;
    arg0K1 = (-1 + stack_nargs_438X);
    goto L14845;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_842X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_843X))) + 4)));
    goto L14886;}}
 L22808: {
  obj_846X = SvalS;
  if ((3 == (3 & obj_846X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_846X))) + -4))))>>2))))) {
      arg0K0 = 0;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L21362;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L22234;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L22234;}}
 L22815: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_441X;
  ScontS = (Sbottom_of_stackS);
  goto L22808;}
 L24944: {
  cont_847X = ScontS;
  code_848X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_847X))) + 8)))))));
  pc_849X = (((*((long *) ((((char *) (-3 + cont_847X))) + 4))))>>2);
  protocol_850X = *((unsigned char *) ((((char *) (-3 + code_848X))) + (1 + pc_849X)));
  if ((1 == protocol_850X)) {
    goto L24973;}
  else {
    if ((66 == protocol_850X)) {
      goto L24973;}
    else {
      if ((71 == protocol_850X)) {
        cont_851X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
        if ((3 == (3 & cont_851X))) {
          if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_851X))) + -4))))>>2))))) {
            merged_arg0K0 = cont_851X;
            merged_arg0K1 = 0;
            copy_continuation_from_heapB_return_tag = 1;
            goto copy_continuation_from_heapB;
           copy_continuation_from_heapB_return_1:
            goto L24944;}
          else {
            arg0K0 = cont_851X;
            goto L22298;}}
        else {
          arg0K0 = cont_851X;
          goto L22298;}}
      else {
        if ((70 == protocol_850X)) {
          next_852X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_852X)))));
            goto L24991;}
          else {
            ScontS = next_852X;
            goto L24991;}}
        else {
          if ((65 == protocol_850X)) {
            index_853X = 2 + pc_849X;
            wants_stack_args_854X = ((((*((unsigned char *) ((((char *) (-3 + code_848X))) + index_853X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_848X))) + (1 + index_853X))));
            if ((0 == wants_stack_args_854X)) {
              pop_continuationB_return_tag = 0;
              goto pop_continuationB;
             pop_continuationB_return_0:
              s48_make_availableAgc(12);
              a_855X = SvalS;
              addr_856X = s48_allocate_small(12);
              *((long *) addr_856X) = 2050;
              x_857X = 3 + (((long) (addr_856X + 4)));
              *((long *) (((char *) (-3 + x_857X)))) = a_855X;
              *((long *) ((((char *) (-3 + x_857X))) + 4)) = 25;
              *((long *) (SstackS)) = x_857X;
              SstackS = ((SstackS) + -4);
              Scode_pointerS = ((Scode_pointerS) + 4);
              arg1K0 = (Scode_pointerS);
              goto L23677;}
            else {
              if ((1 == wants_stack_args_854X)) {
                pop_continuationB_return_tag = 1;
                goto pop_continuationB;
               pop_continuationB_return_1:
                *((long *) (SstackS)) = (SvalS);
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = 25;
                SstackS = ((SstackS) + -4);
                Scode_pointerS = ((Scode_pointerS) + 4);
                arg1K0 = (Scode_pointerS);
                goto L23677;}
              else {
                *((long *) (SstackS)) = (SvalS);
                SstackS = ((SstackS) + -4);
                merged_arg0K0 = 25;
                merged_arg0K1 = 1;
                pop_args_GlistSAgc_return_tag = 3;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_3:
                args_858X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_858X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21732;}}}
          else {
            *((long *) (SstackS)) = (SvalS);
            SstackS = ((SstackS) + -4);
            merged_arg0K0 = 25;
            merged_arg0K1 = 1;
            pop_args_GlistSAgc_return_tag = 4;
            goto pop_args_GlistSAgc;
           pop_args_GlistSAgc_return_4:
            args_859X = pop_args_GlistSAgc0_return_value;
            push_exception_continuationB(4, 0);
            *((long *) (SstackS)) = 1;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = args_859X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21732;}}}}}}
 L47098: {
  stack_nargs_860X = arg0K0;
  list_args_861X = arg0K1;
  list_arg_count_862X = arg0K2;
  cont_863X = ScontS;
  code_864X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_863X))) + 8)))))));
  pc_865X = (((*((long *) ((((char *) (-3 + cont_863X))) + 4))))>>2);
  protocol_866X = *((unsigned char *) ((((char *) (-3 + code_864X))) + (1 + pc_865X)));
  if ((1 == protocol_866X)) {
    if ((1 == (stack_nargs_860X + list_arg_count_862X))) {
      if ((1 == stack_nargs_860X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        goto L47193;}
      else {
        arg0K0 = (*((long *) (((char *) (-3 + list_args_861X)))));
        goto L47193;}}
    else {
      merged_arg0K0 = list_args_861X;
      merged_arg0K1 = stack_nargs_860X;
      pop_args_GlistSAgc_return_tag = 5;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_5:
      args_867X = pop_args_GlistSAgc0_return_value;
      push_exception_continuationB(4, 0);
      *((long *) (SstackS)) = 1;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = args_867X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}}
  else {
    if ((66 == protocol_866X)) {
      pop_continuationB_return_tag = 2;
      goto pop_continuationB;
     pop_continuationB_return_2:
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23677;}
    else {
      if ((71 == protocol_866X)) {
        cont_868X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
        if ((3 == (3 & cont_868X))) {
          if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_868X))) + -4))))>>2))))) {
            merged_arg0K0 = cont_868X;
            merged_arg0K1 = stack_nargs_860X;
            copy_continuation_from_heapB_return_tag = 2;
            goto copy_continuation_from_heapB;
           copy_continuation_from_heapB_return_2:
            arg0K0 = stack_nargs_860X;
            arg0K1 = list_args_861X;
            arg0K2 = list_arg_count_862X;
            goto L47098;}
          else {
            goto L47141;}}
        else {
          goto L47141;}}
      else {
        if ((70 == protocol_866X)) {
          next_869X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_869X)))));
            goto L47149;}
          else {
            ScontS = next_869X;
            goto L47149;}}
        else {
          if ((63 < protocol_866X)) {
            if ((65 == protocol_866X)) {
              index_870X = 2 + pc_865X;
              count_871X = ((((*((unsigned char *) ((((char *) (-3 + code_864X))) + index_870X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_864X))) + (1 + index_870X))));
              if (((stack_nargs_860X + list_arg_count_862X) < count_871X)) {
                merged_arg0K0 = list_args_861X;
                merged_arg0K1 = stack_nargs_860X;
                pop_args_GlistSAgc_return_tag = 6;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_6:
                args_872X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_872X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21732;}
              else {
                arg_top_873X = SstackS;
                pop_continuationB_return_tag = 3;
                goto pop_continuationB;
               pop_continuationB_return_3:
                arg1K0 = (SstackS);
                arg1K1 = (arg_top_873X + (((stack_nargs_860X)<<2)));
                goto L25929;}}
            else {
              if ((64 == protocol_866X)) {
                index_874X = 2 + pc_865X;
                arg0K0 = (((((*((unsigned char *) ((((char *) (-3 + code_864X))) + index_874X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_864X))) + (1 + index_874X)))));
                arg0K1 = 3;
                arg0K2 = stack_nargs_860X;
                arg0K3 = list_args_861X;
                arg0K4 = list_arg_count_862X;
                goto L25781;}
              else {
                ps_error("unknown protocol", 1, protocol_866X);
                merged_arg0K0 = list_args_861X;
                merged_arg0K1 = stack_nargs_860X;
                pop_args_GlistSAgc_return_tag = 7;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_7:
                args_875X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_875X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21732;}}}
          else {
            arg0K0 = protocol_866X;
            arg0K1 = 1;
            arg0K2 = stack_nargs_860X;
            arg0K3 = list_args_861X;
            arg0K4 = list_arg_count_862X;
            goto L25781;}}}}}}
 L24074: {
  tem_876X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  StemplateS = tem_876X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_876X)))))))) + 2);
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 1;
      goto check_events;
     check_events_return_1:
      v_877X = check_events0_return_value;
      if (v_877X) {
        arg0K0 = 0;
        goto L21965;}
      else {
        goto L24084;}}
    else {
      arg0K0 = 0;
      goto L21965;}}
  else {
    goto L24084;}}
 L24117: {
  push_exception_continuationB(5, 4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L23975: {
  arg0K0 = (2 + (((max_450X)<<1)));
  goto L23977;}
 L23977: {
  offset_878X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_878X);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L24860: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L23873: {
  delta_879X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_879X);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L24751: {
  delta_880X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_880X);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L47688: {
  val_881X = arg0K0;
  SvalS = val_881X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L47700: {
  val_882X = arg0K0;
  SvalS = val_882X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L13483: {
  if ((3 == (3 & x_464X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L47700;}
    else {
      goto L13489;}}
  else {
    goto L13489;}}
 L39189: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L39190: {
  if ((3 == (3 & n_465X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_465X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_465X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
    else {
      goto L39194;}}
  else {
    goto L39194;}}
 L39346: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L39321: {
  if ((3 == (3 & n_466X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      goto L39346;}
    else {
      goto L39335;}}
  else {
    goto L39335;}}
 L39545: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L39520: {
  if ((3 == (3 & n_467X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      goto L39545;}
    else {
      goto L39534;}}
  else {
    goto L39534;}}
 L39744: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L39719: {
  if ((3 == (3 & n_468X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      goto L39744;}
    else {
      goto L39733;}}
  else {
    goto L39733;}}
 L32445: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L32446: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_469X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L32571: {
  v_883X = (char *) s48_long_to_bignum(x_474X);
  v_884X = enter_bignum(v_883X);
  arg0K0 = v_884X;
  goto L32517;}
 L32517: {
  val_885X = arg0K0;
  SvalS = val_885X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L32523: {
  if ((0 == (3 & y_473X))) {
    goto L32529;}
  else {
    if ((3 == (3 & y_473X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_473X))) + -4))))>>2))))) {
        goto L32529;}
      else {
        goto L32532;}}
    else {
      goto L32532;}}}
 L32532: {
  if ((3 == (3 & x_472X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_472X))) + -4))))>>2))))) {
      if ((3 == (3 & y_473X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_473X))) + -4))))>>2))))) {
          Stemp0S = x_472X;
          Stemp1S = y_473X;
          s48_make_availableAgc(12);
          value_886X = Stemp1S;
          Stemp1S = 1;
          y_887X = *((double *) (((char *) (-3 + value_886X))));
          value_888X = Stemp0S;
          Stemp0S = 1;
          x_889X = *((double *) (((char *) (-3 + value_888X))));
          addr_890X = s48_allocate_small(12);
          *((long *) addr_890X) = 2126;
          Kdouble_891X = 3 + (((long) (addr_890X + 4)));
          *((double *) (((char *) (-3 + Kdouble_891X)))) = (x_889X + y_887X);
          SvalS = Kdouble_891X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          goto L32546;}}
      else {
        goto L32546;}}
    else {
      goto L32546;}}
  else {
    goto L32546;}}
 L8040: {
  a_892X = arg0K0;
  if ((b_478X < 0)) {
    arg0K0 = (0 - b_478X);
    goto L8044;}
  else {
    arg0K0 = b_478X;
    goto L8044;}}
 L41491: {
  if ((0 == (3 & y_476X))) {
    goto L41497;}
  else {
    if ((3 == (3 & y_476X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_476X))) + -4))))>>2))))) {
        goto L41497;}
      else {
        goto L41500;}}
    else {
      goto L41500;}}}
 L41500: {
  if ((3 == (3 & x_475X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_475X))) + -4))))>>2))))) {
      if ((3 == (3 & y_476X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_476X))) + -4))))>>2))))) {
          Stemp0S = x_475X;
          Stemp1S = y_476X;
          s48_make_availableAgc(12);
          value_893X = Stemp1S;
          Stemp1S = 1;
          y_894X = *((double *) (((char *) (-3 + value_893X))));
          value_895X = Stemp0S;
          Stemp0S = 1;
          x_896X = *((double *) (((char *) (-3 + value_895X))));
          addr_897X = s48_allocate_small(12);
          *((long *) addr_897X) = 2126;
          Kdouble_898X = 3 + (((long) (addr_897X + 4)));
          *((double *) (((char *) (-3 + Kdouble_898X)))) = (x_896X * y_894X);
          SvalS = Kdouble_898X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          goto L41514;}}
      else {
        goto L41514;}}
    else {
      goto L41514;}}
  else {
    goto L41514;}}
 L32860: {
  v_899X = (char *) s48_long_to_bignum(x_481X);
  v_900X = enter_bignum(v_899X);
  arg0K0 = v_900X;
  goto L32806;}
 L32806: {
  val_901X = arg0K0;
  SvalS = val_901X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L32812: {
  if ((0 == (3 & y_480X))) {
    goto L32818;}
  else {
    if ((3 == (3 & y_480X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_480X))) + -4))))>>2))))) {
        goto L32818;}
      else {
        goto L32821;}}
    else {
      goto L32821;}}}
 L32821: {
  if ((3 == (3 & x_479X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_479X))) + -4))))>>2))))) {
      if ((3 == (3 & y_480X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_480X))) + -4))))>>2))))) {
          Stemp0S = x_479X;
          Stemp1S = y_480X;
          s48_make_availableAgc(12);
          value_902X = Stemp1S;
          Stemp1S = 1;
          y_903X = *((double *) (((char *) (-3 + value_902X))));
          value_904X = Stemp0S;
          Stemp0S = 1;
          x_905X = *((double *) (((char *) (-3 + value_904X))));
          addr_906X = s48_allocate_small(12);
          *((long *) addr_906X) = 2126;
          Kdouble_907X = 3 + (((long) (addr_906X + 4)));
          *((double *) (((char *) (-3 + Kdouble_907X)))) = (x_905X - y_903X);
          SvalS = Kdouble_907X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23677;}
        else {
          goto L32835;}}
      else {
        goto L32835;}}
    else {
      goto L32835;}}
  else {
    goto L32835;}}
 L8298: {
  a_908X = arg0K0;
  if ((b_485X < 0)) {
    arg0K0 = (0 - b_485X);
    goto L8302;}
  else {
    arg0K0 = b_485X;
    goto L8302;}}
 L41744: {
  if ((0 == (3 & y_483X))) {
    goto L41750;}
  else {
    if ((3 == (3 & y_483X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_483X))) + -4))))>>2))))) {
        goto L41750;}
      else {
        goto L41777;}}
    else {
      goto L41777;}}}
 L41777: {
  if ((3 == (3 & x_482X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_482X))) + -4))))>>2))))) {
      if ((3 == (3 & y_483X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_483X))) + -4))))>>2))))) {
          if ((0. == (*((double *) (((char *) (-3 + y_483X))))))) {
            arg0K0 = 1;
            goto L41790;}
          else {
            Stemp0S = x_482X;
            Stemp1S = y_483X;
            s48_make_availableAgc(12);
            value_909X = Stemp1S;
            Stemp1S = 1;
            y_910X = *((double *) (((char *) (-3 + value_909X))));
            value_911X = Stemp0S;
            Stemp0S = 1;
            value_912X = (*((double *) (((char *) (-3 + value_911X))))) / y_910X;
            addr_913X = s48_allocate_small(12);
            *((long *) addr_913X) = 2126;
            Kdouble_914X = 3 + (((long) (addr_913X + 4)));
            *((double *) (((char *) (-3 + Kdouble_914X)))) = value_912X;
            arg0K0 = Kdouble_914X;
            goto L41790;}}
        else {
          goto L41797;}}
      else {
        goto L41797;}}
    else {
      goto L41797;}}
  else {
    goto L41797;}}
 L33091: {
  val_915X = arg0K0;
  SvalS = val_915X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L33097: {
  if ((0 == (3 & y_487X))) {
    goto L33103;}
  else {
    if ((3 == (3 & y_487X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_487X))) + -4))))>>2))))) {
        goto L33103;}
      else {
        goto L33108;}}
    else {
      goto L33108;}}}
 L33108: {
  if ((3 == (3 & x_486X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_486X))) + -4))))>>2))))) {
      if ((3 == (3 & y_487X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_487X))) + -4))))>>2))))) {
          b_916X = (*((double *) (((char *) (-3 + x_486X))))) == (*((double *) (((char *) (-3 + y_487X)))));
          if (b_916X) {
            arg0K0 = 5;
            goto L33123;}
          else {
            arg0K0 = 1;
            goto L33123;}}
        else {
          goto L33124;}}
      else {
        goto L33124;}}
    else {
      goto L33124;}}
  else {
    goto L33124;}}
 L33351: {
  val_917X = arg0K0;
  SvalS = val_917X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L33357: {
  if ((0 == (3 & y_489X))) {
    goto L33363;}
  else {
    if ((3 == (3 & y_489X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_489X))) + -4))))>>2))))) {
        goto L33363;}
      else {
        goto L33368;}}
    else {
      goto L33368;}}}
 L33368: {
  if ((3 == (3 & x_488X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_488X))) + -4))))>>2))))) {
      if ((3 == (3 & y_489X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_489X))) + -4))))>>2))))) {
          b_918X = (*((double *) (((char *) (-3 + x_488X))))) < (*((double *) (((char *) (-3 + y_489X)))));
          if (b_918X) {
            arg0K0 = 5;
            goto L33383;}
          else {
            arg0K0 = 1;
            goto L33383;}}
        else {
          goto L33384;}}
      else {
        goto L33384;}}
    else {
      goto L33384;}}
  else {
    goto L33384;}}
 L33674: {
  val_919X = arg0K0;
  SvalS = val_919X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L33680: {
  if ((0 == (3 & y_491X))) {
    goto L33686;}
  else {
    if ((3 == (3 & y_491X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_491X))) + -4))))>>2))))) {
        goto L33686;}
      else {
        goto L33691;}}
    else {
      goto L33691;}}}
 L33691: {
  if ((3 == (3 & x_490X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_490X))) + -4))))>>2))))) {
      if ((3 == (3 & y_491X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_491X))) + -4))))>>2))))) {
          b_920X = (*((double *) (((char *) (-3 + y_491X))))) < (*((double *) (((char *) (-3 + x_490X)))));
          if (b_920X) {
            arg0K0 = 5;
            goto L33706;}
          else {
            arg0K0 = 1;
            goto L33706;}}
        else {
          goto L33707;}}
      else {
        goto L33707;}}
    else {
      goto L33707;}}
  else {
    goto L33707;}}
 L33997: {
  val_921X = arg0K0;
  SvalS = val_921X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L34003: {
  if ((0 == (3 & y_493X))) {
    goto L34009;}
  else {
    if ((3 == (3 & y_493X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_493X))) + -4))))>>2))))) {
        goto L34009;}
      else {
        goto L34014;}}
    else {
      goto L34014;}}}
 L34014: {
  if ((3 == (3 & x_492X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_492X))) + -4))))>>2))))) {
      if ((3 == (3 & y_493X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_493X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + y_493X))))) < (*((double *) (((char *) (-3 + x_492X))))))) {
            arg0K0 = 1;
            goto L34029;}
          else {
            arg0K0 = 5;
            goto L34029;}}
        else {
          goto L34030;}}
      else {
        goto L34030;}}
    else {
      goto L34030;}}
  else {
    goto L34030;}}
 L34291: {
  val_922X = arg0K0;
  SvalS = val_922X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L34297: {
  if ((0 == (3 & y_495X))) {
    goto L34303;}
  else {
    if ((3 == (3 & y_495X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_495X))) + -4))))>>2))))) {
        goto L34303;}
      else {
        goto L34308;}}
    else {
      goto L34308;}}}
 L34308: {
  if ((3 == (3 & x_494X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_494X))) + -4))))>>2))))) {
      if ((3 == (3 & y_495X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_495X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + x_494X))))) < (*((double *) (((char *) (-3 + y_495X))))))) {
            arg0K0 = 1;
            goto L34323;}
          else {
            arg0K0 = 5;
            goto L34323;}}
        else {
          goto L34324;}}
      else {
        goto L34324;}}
    else {
      goto L34324;}}
  else {
    goto L34324;}}
 L18997: {
  x_923X = arg0K0;
  y_924X = arg0K1;
  if ((0 == (3 & x_923X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19027;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_923X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19027;}}
 L8943: {
  a_925X = arg0K0;
  if ((b_499X < 0)) {
    arg0K0 = (0 - b_499X);
    goto L8947;}
  else {
    arg0K0 = b_499X;
    goto L8947;}}
 L34597: {
  if ((0 == (3 & y_497X))) {
    goto L34603;}
  else {
    if ((3 == (3 & y_497X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_497X))) + -4))))>>2))))) {
        goto L34603;}
      else {
        goto L34606;}}
    else {
      goto L34606;}}}
 L34606: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_496X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_497X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L18906: {
  x_926X = arg0K0;
  y_927X = arg0K1;
  if ((0 == (3 & x_926X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18936;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_926X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18936;}}
 L34811: {
  a_928X = arg0K0;
  n_929X = ((y_501X)>>2);
  if ((n_929X < 0)) {
    arg0K0 = (0 - n_929X);
    goto L34813;}
  else {
    arg0K0 = n_929X;
    goto L34813;}}
 L34773: {
  if ((0 == (3 & y_501X))) {
    goto L34779;}
  else {
    if ((3 == (3 & y_501X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_501X))) + -4))))>>2))))) {
        goto L34779;}
      else {
        goto L34782;}}
    else {
      goto L34782;}}}
 L34782: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_500X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_501X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L34960: {
  SvalS = n_503X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L34961: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_503X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L35015: {
  SvalS = n_504X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L35016: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_504X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L35070: {
  SvalS = 4;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L35073: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_505X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L35130: {
  SvalS = n_506X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L35131: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_506X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L35185: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L35188: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = n_507X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L47854: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_516X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L47860: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L22483: {
  x_930X = SvalS;
  if ((0 == (3 & x_930X))) {
    s48_make_availableAgc(16);
    if ((x_930X < 0)) {
      arg0K0 = (0 - x_930X);
      goto L22511;}
    else {
      arg0K0 = x_930X;
      goto L22511;}}
  else {
    if ((0 == (3 & x_930X))) {
      arg0K0 = 1;
      arg0K1 = 3;
      goto L18514;}
    else {
      arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_930X))) + -4))))>>8))))>>2)));
      arg0K1 = 0;
      goto L18514;}}}
 L22486: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L35339: {
  if ((0 == (3 & x_526X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18861;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_526X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18861;}}
 L35342: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_526X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L35430: {
  x_931X = arg0K0;
  arg0K0 = x_931X;
  arg0K1 = 0;
  goto L35436;}
 L35413: {
  if ((0 == (3 & x_527X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L18803;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_527X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L18803;}}
 L35416: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_527X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L35515: {
  if ((0 == (3 & y_530X))) {
    goto L35521;}
  else {
    if ((3 == (3 & y_530X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_530X))) + -4))))>>2))))) {
        goto L35521;}
      else {
        goto L35524;}}
    else {
      goto L35524;}}}
 L35524: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_529X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_530X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L35678: {
  if ((0 == (3 & y_532X))) {
    goto L35684;}
  else {
    if ((3 == (3 & y_532X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_532X))) + -4))))>>2))))) {
        goto L35684;}
      else {
        goto L35687;}}
    else {
      goto L35687;}}}
 L35687: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_531X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_532X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L35841: {
  if ((0 == (3 & y_534X))) {
    goto L35847;}
  else {
    if ((3 == (3 & y_534X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_534X))) + -4))))>>2))))) {
        goto L35847;}
      else {
        goto L35850;}}
    else {
      goto L35850;}}}
 L35850: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_533X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_534X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L32225: {
  val_932X = arg0K0;
  SvalS = val_932X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L32241: {
  val_933X = arg0K0;
  SvalS = val_933X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L32242: {
  push_exception_continuationB(6, 1);
  *((long *) (SstackS)) = x_535X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_536X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L42070: {
  if ((0 == (3 & y_536X))) {
    if ((0 == (3 & x_535X))) {
      value_934X = ((x_535X)>>2);
      count_935X = ((y_536X)>>2);
      if ((count_935X < 0)) {
        PS_SHIFT_RIGHT(value_934X, (0 - count_935X), x_936X)
        x_937X = x_936X;
        s48_make_availableAgc(16);
        if ((536870911 < x_937X)) {
          goto L42190;}
        else {
          if ((x_937X < -536870912)) {
            goto L42190;}
          else {
            arg0K0 = (((x_937X)<<2));
            goto L42185;}}}
      else {
        PS_SHIFT_LEFT(value_934X, count_935X, x_938X)
        result_939X = x_938X;
        PS_SHIFT_RIGHT(result_939X, count_935X, x_940X)
        if ((value_934X == x_940X)) {
          if ((value_934X < 0)) {
            if ((result_939X < 0)) {
              s48_make_availableAgc(16);
              if ((536870911 < result_939X)) {
                goto L42212;}
              else {
                if ((result_939X < -536870912)) {
                  goto L42212;}
                else {
                  arg0K0 = (((result_939X)<<2));
                  goto L42207;}}}
            else {
              arg0K0 = x_535X;
              arg0K1 = y_536X;
              goto L42083;}}
          else {
            if ((result_939X < 0)) {
              arg0K0 = x_535X;
              arg0K1 = y_536X;
              goto L42083;}
            else {
              s48_make_availableAgc(16);
              if ((536870911 < result_939X)) {
                goto L42234;}
              else {
                if ((result_939X < -536870912)) {
                  goto L42234;}
                else {
                  arg0K0 = (((result_939X)<<2));
                  goto L42229;}}}}}
        else {
          arg0K0 = x_535X;
          arg0K1 = y_536X;
          goto L42083;}}}
    else {
      if ((3 == (3 & x_535X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_535X))) + -4))))>>2))))) {
          y_941X = ((y_536X)>>2);
          merged_arg0K0 = x_535X;
          merged_arg0K1 = y_941X;
          shift_space_return_tag = 0;
          goto shift_space;
         shift_space_return_0:
          needed_942X = shift_space0_return_value;
          Stemp0S = x_535X;
          s48_make_availableAgc((((needed_942X)<<2)));
          value_943X = Stemp0S;
          Stemp0S = 1;
          if ((0 == (3 & value_943X))) {
            v_944X = (char *) s48_long_to_bignum((((value_943X)>>2)));
            arg1K0 = v_944X;
            goto L42265;}
          else {
            arg1K0 = (((char *) (-3 + value_943X)));
            goto L42265;}}
        else {
          goto L42092;}}
      else {
        goto L42092;}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_535X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_536X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}}
 L47951: {
  val_945X = arg0K0;
  SvalS = val_945X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L38964: {
  val_946X = arg0K0;
  SvalS = val_946X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L38937: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_540X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L38874: {
  val_947X = arg0K0;
  SvalS = val_947X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L38847: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_542X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L43260: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((x_544X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L47980: {
  val_948X = arg0K0;
  SvalS = val_948X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L47994: {
  value_949X = arg0K0;
  SvalS = value_949X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L26844: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = stob_548X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_549X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L26949: {
  SvalS = new_554X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L26933: {
  i_950X = arg0K0;
  if ((i_950X < 0)) {
    goto L26949;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_554X))) + (((i_950X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_950X);
    goto L26933;}}
 L27065: {
  i_951X = arg0K0;
  if ((i_951X < 0)) {
    arg0K0 = stack_nargs_560X;
    arg0K1 = rest_list_561X;
    goto L27199;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_559X))) + (((i_951X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_951X);
    goto L27065;}}
 L27225: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_562X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_564X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_563X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L27342: {
  addr_952X = (((char *) (-3 + stob_565X))) + (((offset_567X)<<2));
  S48_WRITE_BARRIER(stob_565X, addr_952X, value_566X);
  *((long *) addr_952X) = value_566X;
  goto L27351;}
 L27351: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L27352: {
  push_exception_continuationB(5, 4);
  *((long *) (SstackS)) = stob_565X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_568X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_567X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_566X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L27569: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = (((type_572X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_573X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = init_571X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L27586: {
  x_953X = arg0K0;
  value_954X = Stemp0S;
  Stemp0S = 1;
  if ((1 == x_953X)) {
    push_exception_continuationB(8, 2);
    *((long *) (SstackS)) = (((type_572X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_573X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = value_954X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L21732;}
  else {
    arg0K0 = (-1 + len_573X);
    goto L27612;}}
 L27859: {
  push_exception_continuationB(7, 3);
  *((long *) (SstackS)) = stob_576X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_578X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_577X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L27849: {
  arg0K0 = (*((long *) ((((char *) (-3 + stob_576X))) + (-4 & index_577X))));
  goto L27858;}
 L27858: {
  value_955X = arg0K0;
  SvalS = value_955X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L27814: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_576X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_578X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_577X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L28146: {
  push_exception_continuationB(7, 3);
  *((long *) (SstackS)) = stob_584X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_586X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_583X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_585X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L28136: {
  addr_956X = (((char *) (-3 + stob_584X))) + (-4 & arg2_583X);
  S48_WRITE_BARRIER(stob_584X, addr_956X, value_585X);
  *((long *) addr_956X) = value_585X;
  goto L28145;}
 L28145: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L28099: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_584X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_586X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_583X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_585X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L36062: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_591X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_592X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L36075: {
  vector_957X = arg0K0;
  if ((1 == vector_957X)) {
    push_exception_continuationB(8, 1);
    *((long *) (SstackS)) = (((len_591X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((init_592X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}
  else {
    arg0K0 = (-1 + len_591X);
    goto L36097;}}
 L46765: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L43366: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_595X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_596X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L46281: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_595X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L40713: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_599X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_600X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_601X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L40692: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_599X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_600X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_601X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L45061: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_599X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_598X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L36242: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (((len_604X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_605X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L36255: {
  vector_958X = arg0K0;
  if ((1 == vector_958X)) {
    push_exception_continuationB(8, 1);
    *((long *) (SstackS)) = (((len_604X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (9 + ((((init_605X))<<8)));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_958X))) + len_604X)) = 0;
    arg0K0 = (-1 + len_604X);
    goto L36277;}}
 L43447: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_603X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L46815: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L43510: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_609X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_610X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L46377: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_609X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L40868: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_613X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_614X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_615X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L40847: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_613X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_614X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_615X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L45187: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_613X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_612X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L20287: {
  i_959X = arg0K0;
  h_960X = arg0K1;
  if ((i_959X < n_620X)) {
    arg0K0 = (1 + i_959X);
    arg0K1 = (h_960X + (((*((unsigned char *) ((((char *) (-3 + string_619X))) + i_959X))))));
    goto L20287;}
  else {
    index_961X = 1023 & h_960X;
    link_962X = *((long *) ((((char *) (-3 + table_618X))) + (((index_961X)<<2))));
    if ((0 == (3 & link_962X))) {
      arg0K0 = (3 + (-4 & link_962X));
      goto L20243;}
    else {
      arg0K0 = link_962X;
      goto L20243;}}}
 L32387: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L44751: {
  val_963X = arg0K0;
  SvalS = val_963X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L44732: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L40455: {
  if ((1 == (SvalS))) {
    addr_964X = (((char *) (-3 + arg2_624X))) + 4;
    S48_WRITE_BARRIER(arg2_624X, addr_964X, 273);
    *((long *) addr_964X) = 273;
    goto L40461;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_624X))) + 4)))))) {
      addr_965X = (((char *) (-3 + arg2_624X))) + 4;
      S48_WRITE_BARRIER(arg2_624X, addr_965X, 529);
      *((long *) addr_965X) = 529;
      goto L40461;}
    else {
      goto L40461;}}}
 L40462: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_624X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L48066: {
  val_966X = arg0K0;
  SvalS = val_966X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L43592: {
  SvalS = x_627X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L36649: {
  if ((0 == (3 & arg2_628X))) {
    if (((((arg2_628X)>>2)) < 0)) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_628X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_629X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}
    else {
      arg0K0 = (((arg2_628X)>>2));
      goto L36469;}}
  else {
    if ((3 == (3 & arg2_628X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_628X))) + -4))))>>2))))) {
        if ((1 == mode_629X)) {
          goto L36527;}
        else {
          if ((3 == mode_629X)) {
            goto L36527;}
          else {
            v_967X = ps_open_fd((((char *)(((char *) (-3 + arg2_628X))))), 0, &v_968X);
            arg0K0 = v_967X;
            arg0K1 = v_968X;
            goto L36540;}}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = arg2_628X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_629X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = arg2_628X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_629X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}}}
 L48100: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L43717: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L43727;}
  else {
    arg2K0 = 1;
    goto L43727;}}
 L43728: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_637X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_636X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_635X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_634X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21732;}
 L37288: {
  if ((3 == (3 & arg3_640X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_640X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_640X))) + -4))))>>8)));
      goto L37290;}
    else {
      goto L37477;}}
  else {
    goto L37477;}}
 L37463: {
  if ((3 == (3 & arg3_640X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_640X))) + -4))))>>2))))) {
      goto L37288;}
    else {
      goto L37342;}}
  else {
    goto L37342;}}
 L37342: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_641X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_640X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_642X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_643X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L43923: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_641X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_640X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_639X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L40046: {
  val_969X = arg0K0;
  SvalS = val_969X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L48122: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L44065: {
  val_970X = arg0K0;
  SvalS = val_970X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L10821: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_651X))) + 16)));
  goto L44065;}
 L10827: {
  ch_971X = arg0K0;
  prev_972X = arg0K1;
  if ((1 == ch_971X)) {
    addr_973X = (((char *) (-3 + channel_651X))) + 16;
    S48_WRITE_BARRIER(channel_651X, addr_973X, 1);
    *((long *) addr_973X) = 1;
    n_974X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_651X))) + 8))))>>2)));
    arg0K0 = (((n_974X)<<2));
    goto L44065;}
  else {
    if ((ch_971X == channel_651X)) {
      y_975X = Spending_channels_tailS;
      if ((ch_971X == y_975X)) {
        Spending_channels_tailS = prev_972X;
        goto L10851;}
      else {
        goto L10851;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_971X))) + 12)));
      arg0K1 = ch_971X;
      goto L10827;}}}
 L44048: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L15775: {
  i_976X = arg0K0;
  res_977X = arg0K1;
  if ((-1 == i_976X)) {
    SvalS = res_977X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    channel_978X = *((Svm_channelsS) + i_976X);
    if ((3 == (3 & channel_978X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_978X))) + -4))))>>2))))) {
        addr_979X = s48_allocate_small(12);
        *((long *) addr_979X) = 2050;
        x_980X = 3 + (((long) (addr_979X + 4)));
        *((long *) (((char *) (-3 + x_980X)))) = channel_978X;
        *((long *) ((((char *) (-3 + x_980X))) + 4)) = res_977X;
        arg0K0 = x_980X;
        goto L15789;}
      else {
        arg0K0 = res_977X;
        goto L15789;}}
    else {
      arg0K0 = res_977X;
      goto L15789;}}}
 L37596: {
  old_981X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 == old_981X)) {
    goto L37612;}
  else {
    addr_982X = ((char *) (-3 + old_981X));
    S48_WRITE_BARRIER(old_981X, addr_982X, 1);
    *((long *) addr_982X) = 1;
    goto L37612;}}
 L37627: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proposal_658X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L9230: {
  i_983X = arg0K0;
  stob_984X = *((long *) ((((char *) (-3 + log_660X))) + (((i_983X)<<2))));
  if ((1 == stob_984X)) {
    log_985X = *((long *) ((((char *) (-3 + proposal_659X))) + 8));
    arg0K0 = 0;
    goto L9468;}
  else {
    verify_986X = *((long *) ((((char *) (-3 + log_660X))) + (12 + (((i_983X)<<2)))));
    value_987X = *((long *) ((((char *) (-3 + log_660X))) + (8 + (((i_983X)<<2)))));
    if ((29 == verify_986X)) {
      if ((3 == (3 & stob_984X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_984X))) + -4)))))) {
          goto L9282;}
        else {
          goto L37858;}}
      else {
        goto L37858;}}
    else {
      if ((verify_986X == (*((long *) ((((char *) (-3 + stob_984X))) + (-4 & (*((long *) ((((char *) (-3 + log_660X))) + (4 + (((i_983X)<<2)))))))))))) {
        if ((verify_986X == value_987X)) {
          goto L9282;}
        else {
          if ((3 == (3 & stob_984X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_984X))) + -4)))))) {
              goto L9282;}
            else {
              goto L37858;}}
          else {
            goto L37858;}}}
      else {
        goto L37858;}}}}
 L28444: {
  value_988X = arg0K0;
  SvalS = value_988X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L28445: {
  push_exception_continuationB(5, 3);
  *((long *) (SstackS)) = stob_661X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_663X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_662X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L28538: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg5_669X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_670X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_667X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_671X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_672X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21732;}
 L28598: {
  memcpy((void *)((((char *) (-3 + arg3_667X))) + to_index_671X), (void *)((((char *) (-3 + arg5_669X))) + from_index_670X),count_672X);
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L28686: {
  left_989X = arg0K0;
  copies_990X = arg0K1;
  if ((1 == copies_990X)) {
    if ((left_989X < count_672X)) {
      goto L28538;}
    else {
      from_index_991X = ((from_index_670X)<<2);
      to_index_992X = ((to_index_671X)<<2);
      count_993X = ((count_672X)<<2);
      Stemp0S = arg5_669X;
      Stemp1S = arg3_667X;
      addr_994X = s48_allocate_tracedAgc(28);
      if ((addr_994X == NULL)) {
        arg0K0 = 1;
        goto L10038;}
      else {
        *((long *) addr_994X) = 6154;
        arg0K0 = (3 + (((long) (addr_994X + 4))));
        goto L10038;}}}
  else {
    arg0K0 = (left_989X - ((((*((long *) ((((char *) (-3 + copies_990X))) + 16))))>>2)));
    arg0K1 = (*((long *) ((((char *) (-3 + copies_990X))) + 20)));
    goto L28686;}}
 L28895: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg2_676X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_677X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L28894: {
  value_995X = arg0K0;
  SvalS = value_995X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L14340: {
  i_996X = arg0K0;
  next_stob_997X = *((long *) ((((char *) (-3 + log_681X))) + (((i_996X)<<2))));
  if ((1 == next_stob_997X)) {
    v_998X = add_log_entryAgc(2, i_996X, arg2_676X, index_680X, ((((*((unsigned char *) ((((char *) (-3 + arg2_676X))) + (((index_680X)>>2))))))<<2)), 1);
    arg0K0 = v_998X;
    goto L28894;}
  else {
    if ((arg2_676X == next_stob_997X)) {
      if ((index_680X == (*((long *) ((((char *) (-3 + log_681X))) + (4 + (((i_996X)<<2)))))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + log_681X))) + (8 + (((i_996X)<<2))))));
        goto L28894;}
      else {
        goto L14362;}}
    else {
      goto L14362;}}}
 L31693: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_676X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L29021: {
  push_exception_continuationB(7, 1);
  *((long *) (SstackS)) = arg3_683X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_684X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_685X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L29020: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L14516: {
  i_999X = arg0K0;
  next_stob_1000X = *((long *) ((((char *) (-3 + log_689X))) + (((i_999X)<<2))));
  if ((1 == next_stob_1000X)) {
    add_log_entryAgc(2, i_999X, arg3_683X, index_688X, byte_685X, 0);
    goto L29020;}
  else {
    if ((arg3_683X == next_stob_1000X)) {
      if ((index_688X == (*((long *) ((((char *) (-3 + log_689X))) + (4 + (((i_999X)<<2)))))))) {
        addr_1001X = (((char *) (-3 + log_689X))) + (8 + (((i_999X)<<2)));
        S48_WRITE_BARRIER(log_689X, addr_1001X, byte_685X);
        *((long *) addr_1001X) = byte_685X;
        goto L29020;}
      else {
        goto L14536;}}
    else {
      goto L14536;}}}
 L28986: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_683X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_684X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_685X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L31796: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg3_683X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_682X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L41096: {
  reason_1002X = arg0K0;
  status_1003X = arg0K1;
  push_exception_continuationB(reason_1002X, 1);
  *((long *) (SstackS)) = arg4_693X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_692X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_691X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_1003X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 2;
  goto get_error_string;
 get_error_string_return_2:
  x_1004X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_1004X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L45384: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg4_693X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_692X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_691X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L46025: {
  i_1005X = arg0K0;
  h_1006X = arg0K1;
  if ((i_1005X < n_705X)) {
    arg0K0 = (1 + i_1005X);
    arg0K1 = (h_1006X + (((*((unsigned char *) ((((char *) (-3 + x_704X))) + i_1005X))))));
    goto L46025;}
  else {
    SvalS = (((h_1006X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}}
 L46002: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L41371: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = stob_706X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_707X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L44184: {
  firstP_1007X = arg2K0;
  vector_1008X = s48_find_all(type_719X);
  if ((1 == vector_1008X)) {
    if (firstP_1007X) {
      s48_collect();
      arg2K0 = 0;
      goto L44184;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = (((type_719X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}}
  else {
    SvalS = vector_1008X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}}
 L48224: {
  firstP_1009X = arg2K0;
  type_1010X = arg0K1;
  vector_1011X = s48_find_all_records(type_1010X);
  if ((1 == vector_1011X)) {
    if (firstP_1009X) {
      Stemp0S = type_1010X;
      s48_collect();
      value_1012X = Stemp0S;
      Stemp0S = 1;
      arg2K0 = 0;
      arg0K1 = value_1012X;
      goto L48224;}
    else {
      push_exception_continuationB(8, 1);
      *((long *) (SstackS)) = type_1010X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}}
  else {
    SvalS = vector_1011X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}}
 L26139: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L24603: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L48272: {
  SvalS = (((old_729X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L23691: {
  SstackS = ((SstackS) + 4);
  proposal_1013X = *((long *) (SstackS));
  x_1014X = Scurrent_threadS;
  addr_1015X = (((char *) (-3 + x_1014X))) + 12;
  S48_WRITE_BARRIER(x_1014X, addr_1015X, proposal_1013X);
  *((long *) addr_1015X) = proposal_1013X;
  SstackS = ((SstackS) + 4);
  pc_1016X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tem_1017X = *((long *) (SstackS));
  StemplateS = tem_1017X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1017X)))))))) + (((pc_1016X)>>2)));
  SstackS = ((SstackS) + 4);
  SvalS = (*((long *) (SstackS)));
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L46950: {
  x_1018X = s48_schedule_alarm_interrupt((((p_732X)>>2)));
  SvalS = (((x_1018X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L44934: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L44938;}
  else {
    arg2K0 = 1;
    goto L44938;}}
 L44939: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_733X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L29220: {
  rest_list_1019X = arg0K0;
  if ((25 == rest_list_1019X)) {
    proc_1020X = *((long *) ((SstackS) + (((nargs_735X)<<2))));
    name_1021X = *((long *) ((SstackS) + (-4 + (((nargs_735X)<<2)))));
    args_1022X = (SstackS) + 4;
    *((long *) (SstackS)) = (10 + (((nargs_735X)<<10)));
    SstackS = ((SstackS) + -4);
    if ((3 == (3 & name_1021X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_1021X))) + -4))))>>2))))) {
        if ((3 == (3 & proc_1020X))) {
          if ((18 == (31 & ((((*((long *) ((((char *) (-3 + proc_1020X))) + -4))))>>2))))) {
            if ((4 == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proc_1020X))) + -4))))>>8)))) {
              result_1023X = s48_external_call(proc_1020X, name_1021X, (-2 + nargs_735X), args_1022X);
              if ((Sexternal_exceptionPS)) {
                Sexternal_exceptionPS = 0;
                arg0K0 = (Sexternal_exception_nargsS);
                goto L21732;}
              else {
                SvalS = result_1023X;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23677;}}
            else {
              goto L29486;}}
          else {
            goto L29486;}}
        else {
          goto L29486;}}
      else {
        goto L29486;}}
    else {
      goto L29486;}}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + rest_list_1019X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_1019X))) + 4)));
    goto L29220;}}
 L44279: {
  if ((1 == (SvalS))) {
    v_1024X = Hlookup2670((Sexported_bindingsS), arg2_738X, 0);
    arg0K0 = v_1024X;
    goto L44318;}
  else {
    v_1025X = Hlookup2651((Simported_bindingsS), arg2_738X, 0);
    arg0K0 = v_1025X;
    goto L44318;}}
 L44284: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_738X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L31031: {
  if ((1 == (SvalS))) {
    arg0K0 = (Sexported_bindingsS);
    goto L31064;}
  else {
    arg0K0 = (Simported_bindingsS);
    goto L31064;}}
 L31036: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_740X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L45936: {
  option_1026X = arg0K0;
  seconds_1027X = arg0K1;
  mseconds_1028X = arg0K2;
  if ((536869 < seconds_1027X)) {
    push_exception_continuationB(6, 1);
    *((long *) (SstackS)) = (((option_1026X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_1027X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_1028X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L21732;}
  else {
    SvalS = (((((1000 * seconds_1027X) + mseconds_1028X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}}
 L48332: {
  s48_Scallback_return_stack_blockS = arg2_754X;
  return x_755X;}
 L38640: {
  val_1029X = arg0K0;
  SvalS = val_1029X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L38607: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg2_756X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L38342: {
  len_1030X = 1 + n_761X;
  addr_1031X = s48_allocate_small((4 + len_1030X));
  *((long *) addr_1031X) = (70 + (((len_1030X)<<8)));
  string_1032X = 3 + (((long) (addr_1031X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1032X))) + n_761X)) = 0;
  arg0K0 = arg2_760X;
  arg0K1 = (-1 + n_761X);
  goto L38319;}
 L38294: {
  if ((25 == arg2_760X)) {
    goto L38342;}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = arg2_760X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_761X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}}
 L40109: {
  list_1033X = arg0K0;
  slow_1034X = arg0K1;
  move_slowP_1035X = arg2K2;
  if ((25 == list_1033X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    if ((3 == (3 & list_1033X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_1033X))) + -4))))>>2))))) {
        head_1036X = *((long *) (((char *) (-3 + list_1033X))));
        if ((3 == (3 & head_1036X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_1036X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_1036X))))) == thing_762X)) {
              SvalS = head_1036X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23677;}
            else {
              list_1037X = *((long *) ((((char *) (-3 + list_1033X))) + 4));
              if ((list_1037X == slow_1034X)) {
                push_exception_continuationB(5, 1);
                *((long *) (SstackS)) = thing_762X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = list_763X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L21732;}
              else {
                if (move_slowP_1035X) {
                  arg0K0 = list_1037X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_1034X))) + 4)));
                  arg2K2 = 0;
                  goto L40109;}
                else {
                  arg0K0 = list_1037X;
                  arg0K1 = slow_1034X;
                  arg2K2 = 1;
                  goto L40109;}}}}
          else {
            push_exception_continuationB(5, 1);
            *((long *) (SstackS)) = thing_762X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = list_763X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L21732;}}
        else {
          push_exception_continuationB(5, 1);
          *((long *) (SstackS)) = thing_762X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = list_763X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L21732;}}
      else {
        push_exception_continuationB(5, 1);
        *((long *) (SstackS)) = thing_762X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_763X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
    else {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = thing_762X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = list_763X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}}}
 L29562: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = arg3_765X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_764X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_766X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L29552: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg3_765X))) + (((index_766X)<<2)))));
  goto L29561;}
 L29561: {
  value_1038X = arg0K0;
  SvalS = value_1038X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L29582: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg3_765X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_764X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_766X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21732;}
 L29820: {
  push_exception_continuationB(7, 2);
  *((long *) (SstackS)) = arg4_772X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_771X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_773X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_774X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L29810: {
  addr_1039X = (((char *) (-3 + arg4_772X))) + (((index_773X)<<2));
  S48_WRITE_BARRIER(arg4_772X, addr_1039X, value_774X);
  *((long *) addr_1039X) = value_774X;
  goto L29819;}
 L29819: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L29842: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = arg4_772X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_771X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_773X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_774X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L21732;}
 L30099: {
  port_1040X = arg0K0;
  if ((3 == (3 & port_1040X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1040X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1040X))) + 4))))>>2))))) {
        goto L30169;}
      else {
        p_1041X = *((long *) ((((char *) (-3 + port_1040X))) + 24));
        p_1042X = *((long *) ((((char *) (-3 + port_1040X))) + 28));
        b_1043X = *((long *) ((((char *) (-3 + port_1040X))) + 20));
        i_1044X = ((p_1041X)>>2);
        x_1045X = *((long *) ((((char *) (-3 + port_1040X))) + 12));
        if ((5 == x_1045X)) {
          goto L30149;}
        else {
          if ((1 == b_1043X)) {
            goto L30149;}
          else {
            if ((i_1044X == (((p_1042X)>>2)))) {
              goto L30149;}
            else {
              val_1046X = 4 + (((i_1044X)<<2));
              addr_1047X = (((char *) (-3 + port_1040X))) + 24;
              S48_WRITE_BARRIER(port_1040X, addr_1047X, val_1046X);
              *((long *) addr_1047X) = val_1046X;
              SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1043X))) + i_1044X))))))<<8)));
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23677;}}}}}
    else {
      goto L30169;}}
  else {
    goto L30169;}}
 L30340: {
  port_1048X = arg0K0;
  if ((3 == (3 & port_1048X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1048X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1048X))) + 4))))>>2))))) {
        goto L30410;}
      else {
        p_1049X = *((long *) ((((char *) (-3 + port_1048X))) + 24));
        p_1050X = *((long *) ((((char *) (-3 + port_1048X))) + 28));
        b_1051X = *((long *) ((((char *) (-3 + port_1048X))) + 20));
        i_1052X = ((p_1049X)>>2);
        x_1053X = *((long *) ((((char *) (-3 + port_1048X))) + 12));
        if ((5 == x_1053X)) {
          goto L30390;}
        else {
          if ((1 == b_1051X)) {
            goto L30390;}
          else {
            if ((i_1052X == (((p_1050X)>>2)))) {
              goto L30390;}
            else {
              SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1051X))) + i_1052X))))))<<8)));
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23677;}}}}}
    else {
      goto L30410;}}
  else {
    goto L30410;}}
 L30574: {
  Kchar_1054X = arg0K0;
  port_1055X = arg0K1;
  if ((9 == (255 & Kchar_1054X))) {
    if ((3 == (3 & port_1055X))) {
      if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1055X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_1055X))) + 4))))>>2))))) {
          goto L30645;}
        else {
          p_1056X = *((long *) ((((char *) (-3 + port_1055X))) + 24));
          b_1057X = *((long *) ((((char *) (-3 + port_1055X))) + 20));
          i_1058X = ((p_1056X)>>2);
          x_1059X = *((long *) ((((char *) (-3 + port_1055X))) + 12));
          if ((5 == x_1059X)) {
            goto L30627;}
          else {
            if ((1 == b_1057X)) {
              goto L30627;}
            else {
              if ((i_1058X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_1057X))) + -4))))>>8)))) {
                goto L30627;}
              else {
                val_1060X = 4 + (((i_1058X)<<2));
                addr_1061X = (((char *) (-3 + port_1055X))) + 24;
                S48_WRITE_BARRIER(port_1055X, addr_1061X, val_1060X);
                *((long *) addr_1061X) = val_1060X;
                *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_1055X))) + 20)))))) + i_1058X)) = (((((Kchar_1054X)>>8))));
                SvalS = 13;
                Scode_pointerS = ((Scode_pointerS) + 2);
                arg1K0 = (Scode_pointerS);
                goto L23677;}}}}}
      else {
        goto L30645;}}
    else {
      goto L30645;}}
  else {
    goto L30645;}}
 L40362: {
  stuff_1062X = arg0K0;
  if ((3 == (3 & stuff_1062X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_1062X))) + -4))))>>2))))) {
      thing_1063X = *((long *) (((char *) (-3 + stuff_1062X))));
      if ((0 == (3 & thing_1063X))) {
        ps_write_integer((((thing_1063X)>>2)), out_785X);
        goto L40368;}
      else {
        if ((9 == (255 & thing_1063X))) {
          ps_write_string("#\\", out_785X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_1063X)>>8))), out_785X, ignoreXX) }
          goto L40368;}
        else {
          if ((3 == (3 & thing_1063X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thing_1063X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_1063X))) + -4))))>>8))))>>2)))) {
                type_1064X = *((long *) (((char *) (-3 + thing_1063X))));
                if ((3 == (3 & type_1064X))) {
                  if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_1064X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_1064X))) + -4))))>>8))))>>2)))) {
                      obj_1065X = *((long *) ((((char *) (-3 + type_1064X))) + 8));
                      if ((3 == (3 & obj_1065X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_1065X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_785X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_1063X)))))))) + 8))))))))))))), out_785X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_785X, ignoreXX) }
                          goto L40368;}
                        else {
                          goto L15225;}}
                      else {
                        goto L15225;}}
                    else {
                      goto L15225;}}
                  else {
                    goto L15225;}}
                else {
                  goto L15225;}}
              else {
                goto L15225;}}
            else {
              goto L15225;}}
          else {
            goto L15225;}}}}
    else {
      goto L40353;}}
  else {
    goto L40353;}}
 L21795: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_788X))) + (((opcode_787X)<<2)))));
  obj_1066X = SvalS;
  if ((3 == (3 & obj_1066X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1066X))) + -4))))>>2))))) {
      goto L21812;}
    else {
      goto L21882;}}
  else {
    goto L21882;}}
 L21868: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L21795;}
 L26373: {
  arg0K0 = (SenvS);
  arg0K1 = 0;
  goto L26332;}
 L26664: {
  i_1067X = arg0K0;
  offset_1068X = arg0K1;
  env_1069X = arg0K2;
  if ((i_1067X == total_count_409X)) {
    SvalS = new_env_412X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1068X));
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    back_1070X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1068X)));
    arg0K0 = env_1069X;
    arg0K1 = back_1070X;
    goto L26742;}}
 L26515: {
  i_1071X = arg0K0;
  offset_1072X = arg0K1;
  env_1073X = arg0K2;
  if ((i_1071X == total_count_416X)) {
    SvalS = new_env_419X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1072X));
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    back_1074X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1072X)));
    arg0K0 = env_1073X;
    arg0K1 = back_1074X;
    goto L26593;}}
 L21965: {
  stack_arg_count_1075X = arg0K0;
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
  x_1076X = Scurrent_threadS;
  addr_1077X = (((char *) (-3 + x_1076X))) + 12;
  S48_WRITE_BARRIER(x_1076X, addr_1077X, 1);
  *((long *) addr_1077X) = 1;
  tem_1078X = Sinterrupt_templateS;
  StemplateS = tem_1078X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1078X))))))));
  push_continuationB((Scode_pointerS), (5 + stack_arg_count_1075X));
  n_1079X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L22099;}
 L24338: {
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L21377: {
  protocol_1080X = arg0K0;
  stack_space_1081X = arg0K1;
  if ((69 == protocol_1080X)) {
    if ((total_arg_count_822X < 3)) {
      skip_1082X = *((unsigned char *) ((((char *) (-3 + code_821X))) + (2 + total_arg_count_822X)));
      if ((0 == skip_1082X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_818X;
        arg0K2 = list_args_819X;
        arg0K3 = list_arg_count_820X;
        goto L22234;}
      else {
        merged_arg0K0 = list_args_819X;
        merged_arg0K1 = list_arg_count_820X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = skip_1082X;
        arg0K1 = total_arg_count_822X;
        goto L21381;}}
    else {
      skip_1083X = *((unsigned char *) ((((char *) (-3 + code_821X))) + 5));
      if ((0 == skip_1083X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_818X;
        arg0K2 = list_args_819X;
        arg0K3 = list_arg_count_820X;
        goto L22234;}
      else {
        arg0K0 = skip_1083X;
        goto L21414;}}}
  else {
    if ((63 < protocol_1080X)) {
      if ((65 == protocol_1080X)) {
        wants_stack_args_1084X = ((((*((unsigned char *) ((((char *) (-3 + code_821X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_821X))) + 3)));
        if ((total_arg_count_822X < wants_stack_args_1084X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_818X;
          arg0K2 = list_args_819X;
          arg0K3 = list_arg_count_820X;
          goto L22234;}
        else {
          merged_arg0K0 = wants_stack_args_1084X;
          merged_arg0K1 = stack_arg_count_818X;
          merged_arg0K2 = list_args_819X;
          merged_arg0K3 = list_arg_count_820X;
          rest_list_setupAgc_return_tag = 0;
          goto rest_list_setupAgc;
         rest_list_setupAgc_return_0:
          arg0K0 = 4;
          arg0K1 = (1 + wants_stack_args_1084X);
          goto L21381;}}
      else {
        if ((68 == protocol_1080X)) {
          if ((total_arg_count_822X < (*((unsigned char *) ((((char *) (-3 + code_821X))) + 2))))) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_818X;
            arg0K2 = list_args_819X;
            arg0K3 = list_arg_count_820X;
            goto L22234;}
          else {
            arg0K0 = 3;
            goto L21414;}}
        else {
          if ((64 == protocol_1080X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_821X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_821X))) + 3)))) == total_arg_count_822X)) {
              if ((0 == list_arg_count_820X)) {
                arg0K0 = 4;
                arg0K1 = total_arg_count_822X;
                goto L21381;}
              else {
                merged_arg0K0 = list_args_819X;
                merged_arg0K1 = list_arg_count_820X;
                push_list_return_tag = 1;
                goto push_list;
               push_list_return_1:
                arg0K0 = 4;
                arg0K1 = total_arg_count_822X;
                goto L21381;}}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_818X;
              arg0K2 = list_args_819X;
              arg0K3 = list_arg_count_820X;
              goto L22234;}}
          else {
            if ((67 == protocol_1080X)) {
              length_1085X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_821X))) + -4))))>>8);
              index_1086X = -2 + length_1085X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_821X))) + (-3 + length_1085X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_821X))) + index_1086X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_821X))) + (1 + index_1086X)))));
              goto L21377;}
            else {
              ps_error("unknown protocol", 1, protocol_1080X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_818X;
              arg0K2 = list_args_819X;
              arg0K3 = list_arg_count_820X;
              goto L22234;}}}}}
    else {
      if ((protocol_1080X == total_arg_count_822X)) {
        if ((0 == list_arg_count_820X)) {
          arg0K0 = 2;
          arg0K1 = total_arg_count_822X;
          goto L21381;}
        else {
          merged_arg0K0 = list_args_819X;
          merged_arg0K1 = list_arg_count_820X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          arg0K0 = 2;
          arg0K1 = total_arg_count_822X;
          goto L21381;}}
      else {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_818X;
        arg0K2 = list_args_819X;
        arg0K3 = list_arg_count_820X;
        goto L22234;}}}}
 L24973: {
  pop_continuationB_return_tag = 4;
  goto pop_continuationB;
 pop_continuationB_return_4:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L22298: {
  cont_1087X = arg0K0;
  if ((1 == cont_1087X)) {
    if ((0 == (3 & (SvalS)))) {
      s48_Scallback_return_stack_blockS = 1;
      return ((((SvalS))>>2));}
    else {
      goto L22313;}}
  else {
    goto L22313;}}
 L24991: {
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  SvalS = (*((long *) ((((char *) (-3 + cont_847X))) + 16)));
  arg0K0 = 1;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L22863;}
 L47193: {
  v_1088X = arg0K0;
  SvalS = v_1088X;
  pop_continuationB_return_tag = 5;
  goto pop_continuationB;
 pop_continuationB_return_5:
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L47141: {
  if ((1 == (stack_nargs_860X + list_arg_count_862X))) {
    if ((1 == stack_nargs_860X)) {
      SstackS = ((SstackS) + 4);
      arg0K0 = (*((long *) (SstackS)));
      goto L47234;}
    else {
      arg0K0 = (*((long *) (((char *) (-3 + list_args_861X)))));
      goto L47234;}}
  else {
    merged_arg0K0 = list_args_861X;
    merged_arg0K1 = stack_nargs_860X;
    pop_args_GlistSAgc_return_tag = 8;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_8:
    args_1089X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1089X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}}
 L47149: {
  SvalS = (*((long *) ((((char *) (-3 + cont_863X))) + 16)));
  arg0K0 = stack_nargs_860X;
  arg0K1 = list_args_861X;
  arg0K2 = list_arg_count_862X;
  goto L22863;}
 L25929: {
  loc_1090X = arg1K0;
  arg_1091X = arg1K1;
  if ((arg_top_873X < arg_1091X)) {
    *((long *) loc_1090X) = (*((long *) arg_1091X));
    arg1K0 = (loc_1090X + -4);
    arg1K1 = (arg_1091X + -4);
    goto L25929;}
  else {
    SstackS = loc_1090X;
    if ((count_871X < stack_nargs_860X)) {
      merged_arg0K0 = list_args_861X;
      merged_arg0K1 = (stack_nargs_860X - count_871X);
      pop_args_GlistSAgc_return_tag = 9;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_9:
      v_1092X = pop_args_GlistSAgc0_return_value;
      arg0K0 = v_1092X;
      goto L25908;}
    else {
      arg0K0 = stack_nargs_860X;
      arg0K1 = list_args_861X;
      goto L25890;}}}
 L25781: {
  count_1093X = arg0K0;
  bytes_used_1094X = arg0K1;
  stack_nargs_1095X = arg0K2;
  list_args_1096X = arg0K3;
  list_arg_count_1097X = arg0K4;
  if ((count_1093X == (stack_nargs_1095X + list_arg_count_1097X))) {
    arg_top_1098X = SstackS;
    pop_continuationB_return_tag = 6;
    goto pop_continuationB;
   pop_continuationB_return_6:
    arg1K0 = (SstackS);
    arg1K1 = (arg_top_1098X + (((stack_nargs_1095X)<<2)));
    goto L25813;}
  else {
    merged_arg0K0 = list_args_1096X;
    merged_arg0K1 = stack_nargs_1095X;
    pop_args_GlistSAgc_return_tag = 10;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_10:
    args_1099X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1099X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}}
 L24084: {
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L13489: {
  if ((3 == (3 & x_464X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L47700;}
    else {
      goto L13495;}}
  else {
    goto L13495;}}
 L39194: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L39335: {
  if ((3 == (3 & n_466X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      goto L39346;}
    else {
      goto L39347;}}
  else {
    goto L39347;}}
 L39534: {
  if ((3 == (3 & n_467X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      goto L39545;}
    else {
      goto L39546;}}
  else {
    goto L39546;}}
 L39733: {
  if ((3 == (3 & n_468X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      goto L39744;}
    else {
      goto L39745;}}
  else {
    goto L39745;}}
 L32529: {
  if ((0 == (3 & x_472X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17170;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_472X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17170;}}
 L32546: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_472X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_473X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L8044: {
  b_1100X = arg0K0;
  lo_a_1101X = 65535 & a_892X;
  lo_b_1102X = 65535 & b_1100X;
  hi_a_1103X = 65535 & (((a_892X)>>16));
  hi_b_1104X = 65535 & (((b_1100X)>>16));
  lo_c_1105X = SMALL_MULTIPLY(lo_a_1101X, lo_b_1102X);
  v_1106X = SMALL_MULTIPLY(lo_b_1102X, hi_a_1103X);
  v_1107X = SMALL_MULTIPLY(lo_a_1101X, hi_b_1104X);
  mid_c_1108X = v_1107X + v_1106X;
  c_1109X = lo_c_1105X + (((mid_c_1108X)<<16));
  if ((0 < hi_a_1103X)) {
    if ((0 < hi_b_1104X)) {
      arg0K0 = x_475X;
      arg0K1 = y_476X;
      goto L19088;}
    else {
      goto L8086;}}
  else {
    goto L8086;}}
 L41497: {
  arg0K0 = x_475X;
  arg0K1 = y_476X;
  goto L19088;}
 L41514: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_475X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_476X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L32818: {
  if ((0 == (3 & x_479X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17127;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_479X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17127;}}
 L32835: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_479X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_480X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L8302: {
  b_1110X = arg0K0;
  c_1111X = a_908X / b_1110X;
  x_1112X = 0 == (a_908X % b_1110X);
  if (x_1112X) {
    if ((a_484X < 0)) {
      if ((b_485X < 0)) {
        s48_make_availableAgc(16);
        if ((536870911 < c_1111X)) {
          goto L49095;}
        else {
          if ((c_1111X < -536870912)) {
            goto L49095;}
          else {
            arg0K0 = (((c_1111X)<<2));
            goto L49090;}}}
      else {
        goto L8348;}}
    else {
      if ((b_485X < 0)) {
        goto L8348;}
      else {
        s48_make_availableAgc(16);
        if ((536870911 < c_1111X)) {
          goto L49117;}
        else {
          if ((c_1111X < -536870912)) {
            goto L49117;}
          else {
            arg0K0 = (((c_1111X)<<2));
            goto L49112;}}}}}
  else {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_482X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_483X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}}
 L41750: {
  if ((0 == (3 & x_482X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16695;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_482X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16695;}}
 L41790: {
  result_1113X = arg0K0;
  if ((1 == result_1113X)) {
    push_exception_continuationB(5, 1);
    *((long *) (SstackS)) = x_482X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_483X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}
  else {
    SvalS = result_1113X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}}
 L41797: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_482X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_483X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L33103: {
  if ((0 == (3 & (x_486X | y_487X)))) {
    if ((x_486X == y_487X)) {
      arg0K0 = 5;
      goto L33107;}
    else {
      arg0K0 = 1;
      goto L33107;}}
  else {
    if ((3 == (3 & x_486X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_486X))) + -4))))>>2))))) {
        if ((3 == (3 & y_487X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_487X))) + -4))))>>2))))) {
            b_1114X = s48_bignum_equal_p((((char *) (-3 + x_486X))), (((char *) (-3 + y_487X))));
            if (b_1114X) {
              arg0K0 = 5;
              goto L33107;}
            else {
              arg0K0 = 1;
              goto L33107;}}
          else {
            arg0K0 = 1;
            goto L33107;}}
        else {
          arg0K0 = 1;
          goto L33107;}}
      else {
        arg0K0 = 1;
        goto L33107;}}
    else {
      arg0K0 = 1;
      goto L33107;}}}
 L33123: {
  val_1115X = arg0K0;
  SvalS = val_1115X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L33124: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_486X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_487X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L33363: {
  if ((0 == (3 & x_488X))) {
    if ((0 == (3 & y_489X))) {
      if ((x_488X < y_489X)) {
        arg0K0 = 5;
        goto L33367;}
      else {
        arg0K0 = 1;
        goto L33367;}}
    else {
      v_1116X = s48_bignum_test((((char *) (-3 + y_489X))));
      if ((1 == v_1116X)) {
        arg0K0 = 5;
        goto L33367;}
      else {
        arg0K0 = 1;
        goto L33367;}}}
  else {
    if ((0 == (3 & y_489X))) {
      v_1117X = s48_bignum_test((((char *) (-3 + x_488X))));
      if ((1 == v_1117X)) {
        arg0K0 = 1;
        goto L33367;}
      else {
        arg0K0 = 5;
        goto L33367;}}
    else {
      v_1118X = s48_bignum_compare((((char *) (-3 + x_488X))), (((char *) (-3 + y_489X))));
      if ((-1 == v_1118X)) {
        arg0K0 = 5;
        goto L33367;}
      else {
        arg0K0 = 1;
        goto L33367;}}}}
 L33383: {
  val_1119X = arg0K0;
  SvalS = val_1119X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L33384: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_488X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_489X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L33686: {
  if ((0 == (3 & y_491X))) {
    if ((0 == (3 & x_490X))) {
      if ((y_491X < x_490X)) {
        arg0K0 = 5;
        goto L33690;}
      else {
        arg0K0 = 1;
        goto L33690;}}
    else {
      v_1120X = s48_bignum_test((((char *) (-3 + x_490X))));
      if ((1 == v_1120X)) {
        arg0K0 = 5;
        goto L33690;}
      else {
        arg0K0 = 1;
        goto L33690;}}}
  else {
    if ((0 == (3 & x_490X))) {
      v_1121X = s48_bignum_test((((char *) (-3 + y_491X))));
      if ((1 == v_1121X)) {
        arg0K0 = 1;
        goto L33690;}
      else {
        arg0K0 = 5;
        goto L33690;}}
    else {
      v_1122X = s48_bignum_compare((((char *) (-3 + y_491X))), (((char *) (-3 + x_490X))));
      if ((-1 == v_1122X)) {
        arg0K0 = 5;
        goto L33690;}
      else {
        arg0K0 = 1;
        goto L33690;}}}}
 L33706: {
  val_1123X = arg0K0;
  SvalS = val_1123X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L33707: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_490X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_491X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L34009: {
  if ((0 == (3 & y_493X))) {
    if ((0 == (3 & x_492X))) {
      if ((y_493X < x_492X)) {
        arg0K0 = 1;
        goto L34013;}
      else {
        arg0K0 = 5;
        goto L34013;}}
    else {
      v_1124X = s48_bignum_test((((char *) (-3 + x_492X))));
      if ((1 == v_1124X)) {
        arg0K0 = 1;
        goto L34013;}
      else {
        arg0K0 = 5;
        goto L34013;}}}
  else {
    if ((0 == (3 & x_492X))) {
      v_1125X = s48_bignum_test((((char *) (-3 + y_493X))));
      if ((1 == v_1125X)) {
        arg0K0 = 5;
        goto L34013;}
      else {
        arg0K0 = 1;
        goto L34013;}}
    else {
      v_1126X = s48_bignum_compare((((char *) (-3 + y_493X))), (((char *) (-3 + x_492X))));
      if ((-1 == v_1126X)) {
        arg0K0 = 1;
        goto L34013;}
      else {
        arg0K0 = 5;
        goto L34013;}}}}
 L34029: {
  val_1127X = arg0K0;
  SvalS = val_1127X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L34030: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_492X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_493X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L34303: {
  if ((0 == (3 & x_494X))) {
    if ((0 == (3 & y_495X))) {
      if ((x_494X < y_495X)) {
        arg0K0 = 1;
        goto L34307;}
      else {
        arg0K0 = 5;
        goto L34307;}}
    else {
      v_1128X = s48_bignum_test((((char *) (-3 + y_495X))));
      if ((1 == v_1128X)) {
        arg0K0 = 1;
        goto L34307;}
      else {
        arg0K0 = 5;
        goto L34307;}}}
  else {
    if ((0 == (3 & y_495X))) {
      v_1129X = s48_bignum_test((((char *) (-3 + x_494X))));
      if ((1 == v_1129X)) {
        arg0K0 = 5;
        goto L34307;}
      else {
        arg0K0 = 1;
        goto L34307;}}
    else {
      v_1130X = s48_bignum_compare((((char *) (-3 + x_494X))), (((char *) (-3 + y_495X))));
      if ((-1 == v_1130X)) {
        arg0K0 = 1;
        goto L34307;}
      else {
        arg0K0 = 5;
        goto L34307;}}}}
 L34323: {
  val_1131X = arg0K0;
  SvalS = val_1131X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L34324: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_494X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_495X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L19027: {
  length0_1132X = arg0K0;
  extra0_1133X = arg0K1;
  if ((0 == (3 & y_924X))) {
    arg0K0 = 3;
    goto L19029;}
  else {
    arg0K0 = 0;
    goto L19029;}}
 L8947: {
  b_1134X = arg0K0;
  c_1135X = a_925X / b_1134X;
  if ((a_498X < 0)) {
    if ((b_499X < 0)) {
      goto L8993;}
    else {
      goto L8992;}}
  else {
    if ((b_499X < 0)) {
      goto L8992;}
    else {
      goto L8993;}}}
 L34603: {
  arg0K0 = x_496X;
  arg0K1 = y_497X;
  goto L18997;}
 L18936: {
  length0_1136X = arg0K0;
  extra0_1137X = arg0K1;
  if ((0 == (3 & y_927X))) {
    arg0K0 = 3;
    goto L18938;}
  else {
    arg0K0 = 0;
    goto L18938;}}
 L34813: {
  b_1138X = arg0K0;
  c_1139X = a_928X % b_1138X;
  if ((a_502X < 0)) {
    arg0K0 = (0 - c_1139X);
    goto L34817;}
  else {
    arg0K0 = c_1139X;
    goto L34817;}}
 L34779: {
  arg0K0 = x_500X;
  arg0K1 = y_501X;
  goto L18906;}
 L22511: {
  x_1140X = arg0K0;
  if ((536870911 < x_1140X)) {
    goto L22512;}
  else {
    if ((x_1140X < -536870912)) {
      goto L22512;}
    else {
      return (((x_1140X)<<2));}}}
 L18514: {
  length_1141X = arg0K0;
  extra_1142X = arg0K1;
  Stemp0S = x_930X;
  s48_make_availableAgc(((((length_1141X + extra_1142X))<<2)));
  value_1143X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1143X))) {
    v_1144X = (char *) s48_long_to_bignum((((value_1143X)>>2)));
    arg1K0 = v_1144X;
    goto L18506;}
  else {
    arg1K0 = (((char *) (-3 + value_1143X)));
    goto L18506;}}
 L18861: {
  length_1145X = arg0K0;
  extra_1146X = arg0K1;
  if ((length_1145X < 1)) {
    arg0K0 = 1;
    goto L18863;}
  else {
    arg0K0 = length_1145X;
    goto L18863;}}
 L35436: {
  x_1147X = arg0K0;
  count_1148X = arg0K1;
  if ((0 == x_1147X)) {
    SvalS = (((count_1148X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    arg0K0 = (((x_1147X)>>1));
    arg0K1 = (count_1148X + (1 & x_1147X));
    goto L35436;}}
 L18803: {
  length_1149X = arg0K0;
  extra_1150X = arg0K1;
  if ((length_1149X < 1)) {
    arg0K0 = 1;
    goto L18805;}
  else {
    arg0K0 = length_1149X;
    goto L18805;}}
 L35521: {
  if ((0 == (3 & x_529X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16909;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_529X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16909;}}
 L35684: {
  if ((0 == (3 & x_531X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16866;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_531X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16866;}}
 L35847: {
  if ((0 == (3 & x_533X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16823;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_533X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16823;}}
 L42190: {
  v_1151X = (char *) s48_long_to_bignum(x_937X);
  v_1152X = enter_bignum(v_1151X);
  arg0K0 = v_1152X;
  goto L42185;}
 L42185: {
  val_1153X = arg0K0;
  SvalS = val_1153X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L42212: {
  v_1154X = (char *) s48_long_to_bignum(result_939X);
  v_1155X = enter_bignum(v_1154X);
  arg0K0 = v_1155X;
  goto L42207;}
 L42207: {
  val_1156X = arg0K0;
  SvalS = val_1156X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L42083: {
  x_1157X = arg0K0;
  y_1158X = arg0K1;
  y_1159X = ((y_1158X)>>2);
  merged_arg0K0 = x_1157X;
  merged_arg0K1 = y_1159X;
  shift_space_return_tag = 1;
  goto shift_space;
 shift_space_return_1:
  needed_1160X = shift_space0_return_value;
  Stemp0S = x_1157X;
  s48_make_availableAgc((((needed_1160X)<<2)));
  value_1161X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1161X))) {
    v_1162X = (char *) s48_long_to_bignum((((value_1161X)>>2)));
    arg1K0 = v_1162X;
    goto L42157;}
  else {
    arg1K0 = (((char *) (-3 + value_1161X)));
    goto L42157;}}
 L42234: {
  v_1163X = (char *) s48_long_to_bignum(result_939X);
  v_1164X = enter_bignum(v_1163X);
  arg0K0 = v_1164X;
  goto L42229;}
 L42229: {
  val_1165X = arg0K0;
  SvalS = val_1165X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L42265: {
  x_1166X = arg1K0;
  external_bignum_1167X = (char *) s48_bignum_arithmetic_shift(x_1166X, y_941X);
  v_1168X = s48_bignum_fits_in_word_p(external_bignum_1167X, 30, 1);
  if (v_1168X) {
    n_1169X = s48_bignum_to_long(external_bignum_1167X);
    arg0K0 = (((n_1169X)<<2));
    goto L42091;}
  else {
    v_1170X = enter_bignum(external_bignum_1167X);
    arg0K0 = v_1170X;
    goto L42091;}}
 L42092: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_535X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_536X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L27199: {
  i_1171X = arg0K0;
  rest_list_1172X = arg0K1;
  if ((25 == rest_list_1172X)) {
    SvalS = new_559X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    *((long *) ((((char *) (-3 + new_559X))) + (((i_1171X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_1172X)))));
    arg0K0 = (1 + i_1171X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_1172X))) + 4)));
    goto L27199;}}
 L27612: {
  i_1173X = arg0K0;
  if ((i_1173X < 0)) {
    SvalS = x_953X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    addr_1174X = (((char *) (-3 + x_953X))) + (((i_1173X)<<2));
    S48_WRITE_BARRIER(x_953X, addr_1174X, value_954X);
    *((long *) addr_1174X) = value_954X;
    arg0K0 = (-1 + i_1173X);
    goto L27612;}}
 L36097: {
  i_1175X = arg0K0;
  if ((i_1175X < 0)) {
    SvalS = vector_957X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_957X))) + i_1175X)) = init_592X;
    arg0K0 = (-1 + i_1175X);
    goto L36097;}}
 L36277: {
  i_1176X = arg0K0;
  if ((i_1176X < 0)) {
    SvalS = vector_958X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_958X))) + i_1176X)) = (init_605X);
    arg0K0 = (-1 + i_1176X);
    goto L36277;}}
 L20243: {
  bucket_1177X = arg0K0;
  arg0K0 = bucket_1177X;
  goto L20249;}
 L40461: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L36469: {
  index_1178X = arg0K0;
  channel_1179X = make_registered_channel(mode_629X, arg2_628X, index_1178X, 0, &reason_1180X);
  if ((1 == channel_1179X)) {
    if ((3 == (3 & arg2_628X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_628X))) + -4))))>>2))))) {
        if ((1 == mode_629X)) {
          goto L36584;}
        else {
          if ((3 == mode_629X)) {
            goto L36584;}
          else {
            v_1181X = ps_close_fd(index_1178X);
            arg0K0 = v_1181X;
            goto L36579;}}}
      else {
        push_exception_continuationB(reason_1180X, 1);
        *((long *) (SstackS)) = arg2_628X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_629X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21732;}}
    else {
      push_exception_continuationB(reason_1180X, 1);
      *((long *) (SstackS)) = arg2_628X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_629X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}}
  else {
    SvalS = channel_1179X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}}
 L36527: {
  v_1182X = ps_open_fd((((char *)(((char *) (-3 + arg2_628X))))), 1, &v_1183X);
  arg0K0 = v_1182X;
  arg0K1 = v_1183X;
  goto L36540;}
 L36540: {
  channel_1184X = arg0K0;
  status_1185X = arg0K1;
  if ((status_1185X == NO_ERRORS)) {
    arg0K0 = channel_1184X;
    goto L36469;}
  else {
    if ((status_1185X == ENOENT)) {
      push_exception_continuationB(10, 1);
      *((long *) (SstackS)) = arg2_628X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_629X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21732;}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg2_628X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_629X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1185X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_1186X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1186X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 3;
      goto L21732;}}}
 L43727: {
  waitP_1187X = arg2K0;
  start_1188X = ((arg3_635X)>>2);
  count_1189X = ((arg2_634X)>>2);
  v_1190X = 4 == (*((long *) (((char *) (-3 + arg5_637X)))));
  if (v_1190X) {
    if ((3 == (3 & arg4_636X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_636X))) + -4))))>>2))))) {
        goto L36911;}
      else {
        goto L37126;}}
    else {
      goto L37126;}}
  else {
    goto L36979;}}
 L37290: {
  y_1191X = arg0K0;
  if ((y_1191X < (start_642X + count_643X))) {
    goto L37342;}
  else {
    got_1192X = ps_write_fd(((((*((long *) ((((char *) (-3 + arg4_641X))) + 8))))>>2)), ((((char *) (-3 + arg3_640X))) + start_642X), count_643X, &pendingP_1193X, &status_1194X);
    if ((status_1194X == NO_ERRORS)) {
      if (pendingP_1193X) {
        arg0K0 = 1;
        goto L37341;}
      else {
        arg0K0 = (((got_1192X)<<2));
        goto L37341;}}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg4_641X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg3_640X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_642X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_643X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_1194X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 4;
      goto get_error_string;
     get_error_string_return_4:
      x_1195X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_1195X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 5;
      goto L21732;}}}
 L37477: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_640X))) + -4))))>>8));
  goto L37290;}
 L10851: {
  val_1196X = *((long *) ((((char *) (-3 + ch_971X))) + 12));
  addr_1197X = (((char *) (-3 + prev_972X))) + 12;
  S48_WRITE_BARRIER(prev_972X, addr_1197X, val_1196X);
  *((long *) addr_1197X) = val_1196X;
  addr_1198X = (((char *) (-3 + ch_971X))) + 12;
  S48_WRITE_BARRIER(ch_971X, addr_1198X, 1);
  *((long *) addr_1198X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_971X))) + 16)));
  goto L44065;}
 L15789: {
  v_1199X = arg0K0;
  arg0K0 = (-1 + i_976X);
  arg0K1 = v_1199X;
  goto L15775;}
 L37612: {
  if ((1 == proposal_658X)) {
    goto L37624;}
  else {
    addr_1200X = ((char *) (-3 + proposal_658X));
    S48_WRITE_BARRIER(proposal_658X, addr_1200X, 5);
    *((long *) addr_1200X) = 5;
    goto L37624;}}
 L9468: {
  i_1201X = arg0K0;
  stob_1202X = *((long *) ((((char *) (-3 + log_985X))) + (((i_1201X)<<2))));
  if ((1 == stob_1202X)) {
    copies_1203X = *((long *) ((((char *) (-3 + proposal_659X))) + 12));
    arg0K0 = copies_1203X;
    goto L37958;}
  else {
    verify_1204X = *((long *) ((((char *) (-3 + log_985X))) + (12 + (((i_1201X)<<2)))));
    value_1205X = *((long *) ((((char *) (-3 + log_985X))) + (8 + (((i_1201X)<<2)))));
    if ((29 == verify_1204X)) {
      if ((3 == (3 & stob_1202X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1202X))) + -4)))))) {
          goto L9520;}
        else {
          goto L37858;}}
      else {
        goto L37858;}}
    else {
      if ((verify_1204X == ((((*((unsigned char *) ((((char *) (-3 + stob_1202X))) + ((((*((long *) ((((char *) (-3 + log_985X))) + (4 + (((i_1201X)<<2)))))))>>2))))))<<2)))) {
        if ((verify_1204X == value_1205X)) {
          goto L9520;}
        else {
          if ((3 == (3 & stob_1202X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1202X))) + -4)))))) {
              goto L9520;}
            else {
              goto L37858;}}
          else {
            goto L37858;}}}
      else {
        goto L37858;}}}}
 L9282: {
  arg0K0 = (4 + i_983X);
  goto L9230;}
 L37858: {
  RELEASE_PROPOSAL_LOCK();
  x_1206X = Scurrent_threadS;
  addr_1207X = (((char *) (-3 + x_1206X))) + 12;
  S48_WRITE_BARRIER(x_1206X, addr_1207X, 1);
  *((long *) addr_1207X) = 1;
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L10038: {
  vector_1208X = arg0K0;
  if ((1 == vector_1208X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1209X;
    goto L10001;}
  else {
    arg0K0 = vector_1208X;
    goto L10001;}}
 L14362: {
  arg0K0 = (4 + i_996X);
  goto L14340;}
 L14536: {
  arg0K0 = (4 + i_999X);
  goto L14516;}
 L44938: {
  minutesP_1210X = arg2K0;
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_1211X = check_events0_return_value;
      if (temp_1211X) {
        goto L44964;}
      else {
        goto L44969;}}
    else {
      goto L44964;}}
  else {
    goto L44969;}}
 L29486: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = proc_1020X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = name_1021X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L44318: {
  val_1212X = arg0K0;
  SvalS = val_1212X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L31064: {
  table_1213X = arg0K0;
  n_1214X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_740X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L19756;}
 L38319: {
  l_1215X = arg0K0;
  i_1216X = arg0K1;
  if ((i_1216X < 0)) {
    SvalS = string_1032X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1032X))) + i_1216X)) = ((((((*((long *) (((char *) (-3 + l_1215X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_1215X))) + 4)));
    arg0K1 = (-1 + i_1216X);
    goto L38319;}}
 L30169: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_1040X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L30149: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = port_1040X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L30410: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = port_1048X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L30390: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = port_1048X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L21732;}
 L30645: {
  push_exception_continuationB(5, 2);
  *((long *) (SstackS)) = Kchar_1054X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1055X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L30627: {
  push_exception_continuationB(13, 2);
  *((long *) (SstackS)) = Kchar_1054X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1055X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L40368: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_1062X))) + 4)));
  goto L40362;}
 L15225: {
  if ((3 == (3 & thing_1063X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_1063X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_1063X)))));
      goto L15267;}
    else {
      goto L15233;}}
  else {
    goto L15233;}}
 L40353: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_785X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L21812: {
  StemplateS = (SvalS);
  Slosing_opcodeS = opcode_787X;
  arg0K0 = (2 + nargs_786X);
  arg0K1 = 25;
  arg0K2 = 0;
  goto L21362;}
 L21882: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L21812;}
 L26742: {
  env_1217X = arg0K0;
  i_1218X = arg0K1;
  if ((0 == i_1218X)) {
    count_1219X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_1068X)));
    arg0K0 = count_1219X;
    arg0K1 = i_1067X;
    arg0K2 = (2 + offset_1068X);
    goto L26681;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1217X)))));
    arg0K1 = (-1 + i_1218X);
    goto L26742;}}
 L26593: {
  env_1220X = arg0K0;
  i_1221X = arg0K1;
  if ((0 == i_1221X)) {
    index_1222X = 1 + offset_1072X;
    count_1223X = ((((*((unsigned char *) ((Scode_pointerS) + (1 + index_1222X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_1222X))));
    arg0K0 = count_1223X;
    arg0K1 = i_1071X;
    arg0K2 = (3 + offset_1072X);
    goto L26532;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1220X)))));
    arg0K1 = (-1 + i_1221X);
    goto L26593;}}
 L22099: {
  i_1224X = arg0K0;
  m_1225X = arg0K1;
  if ((0 == (n_1079X & m_1225X))) {
    arg0K0 = (1 + i_1224X);
    arg0K1 = (((m_1225X)<<1));
    goto L22099;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_1225X));
    if ((i_1224X == 0)) {
      *((long *) (SstackS)) = (Sinterrupted_templateS);
      SstackS = ((SstackS) + -4);
      Sinterrupted_templateS = 1;
      *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L21995;}
    else {
      if ((i_1224X == 2)) {
        *((long *) (SstackS)) = (Sfinalize_theseS);
        SstackS = ((SstackS) + -4);
        Sfinalize_theseS = 25;
        *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L21995;}
      else {
        if ((i_1224X == 3)) {
          channel_1226X = Spending_channels_headS;
          next_1227X = *((long *) ((((char *) (-3 + channel_1226X))) + 12));
          Spending_channels_headS = next_1227X;
          addr_1228X = (((char *) (-3 + channel_1226X))) + 12;
          S48_WRITE_BARRIER(channel_1226X, addr_1228X, 1);
          *((long *) addr_1228X) = 1;
          if ((1 == next_1227X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_1226X;
            goto L11012;}
          else {
            arg0K0 = channel_1226X;
            goto L11012;}}
        else {
          if ((i_1224X == 4)) {
            *((long *) (SstackS)) = (Sos_signal_typeS);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (Sos_signal_argumentS);
            SstackS = ((SstackS) + -4);
            Sos_signal_typeS = 1;
            Sos_signal_argumentS = 1;
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 3;
            goto L21995;}
          else {
            *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L21995;}}}}}}
 L21381: {
  skip_1229X = arg0K0;
  stack_arg_count_1230X = arg0K1;
  template_1231X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_1231X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_1231X)))))))) + skip_1229X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  if ((stack_space_1081X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L21288;}
  else {
    s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
    merged_arg0K0 = 0;
    copy_stack_into_heap_return_tag = 3;
    goto copy_stack_into_heap;
   copy_stack_into_heap_return_3:
    if ((stack_space_1081X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L21288;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L21288;}}}
 L21414: {
  skip_1232X = arg0K0;
  if ((total_arg_count_822X < 3)) {
    arg0K0 = total_arg_count_822X;
    goto L21422;}
  else {
    if ((2 < stack_arg_count_818X)) {
      arg0K0 = stack_arg_count_818X;
      goto L21422;}
    else {
      arg0K0 = 2;
      goto L21422;}}}
 L22313: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  push_exception_continuationB(5, 0);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_1087X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L47234: {
  v_1233X = arg0K0;
  SvalS = v_1233X;
  arg0K0 = cont_868X;
  goto L22298;}
 L25908: {
  x_1234X = arg0K0;
  *((long *) (SstackS)) = x_1234X;
  SstackS = ((SstackS) + -4);
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L25890: {
  stack_nargs_1235X = arg0K0;
  l_1236X = arg0K1;
  if ((count_871X == stack_nargs_1235X)) {
    arg0K0 = l_1236X;
    goto L25908;}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1236X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (1 + stack_nargs_1235X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1236X))) + 4)));
    goto L25890;}}
 L25813: {
  loc_1237X = arg1K0;
  arg_1238X = arg1K1;
  if ((arg_top_1098X < arg_1238X)) {
    *((long *) loc_1237X) = (*((long *) arg_1238X));
    arg1K0 = (loc_1237X + -4);
    arg1K1 = (arg_1238X + -4);
    goto L25813;}
  else {
    SstackS = loc_1237X;
    if ((0 == list_arg_count_1097X)) {
      goto L25800;}
    else {
      merged_arg0K0 = list_args_1096X;
      merged_arg0K1 = list_arg_count_1097X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      goto L25800;}}}
 L13495: {
  if ((3 == (3 & x_464X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + x_464X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L47700;}
    else {
      arg0K0 = 1;
      goto L47700;}}
  else {
    arg0K0 = 1;
    goto L47700;}}
 L39347: {
  if ((3 == (3 & n_466X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_466X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_466X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
    else {
      goto L39351;}}
  else {
    goto L39351;}}
 L39546: {
  if ((3 == (3 & n_467X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_467X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_467X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
    else {
      goto L39550;}}
  else {
    goto L39550;}}
 L39745: {
  if ((3 == (3 & n_468X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_468X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      *((long *) (SstackS)) = n_468X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L21732;}
    else {
      goto L39749;}}
  else {
    goto L39749;}}
 L17170: {
  length0_1239X = arg0K0;
  extra0_1240X = arg0K1;
  if ((0 == (3 & y_473X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17178;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_473X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17178;}}
 L19088: {
  x_1241X = arg0K0;
  y_1242X = arg0K1;
  if ((0 == (3 & x_1241X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17090;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1241X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17090;}}
 L8086: {
  if ((536870911 < lo_c_1105X)) {
    arg0K0 = x_475X;
    arg0K1 = y_476X;
    goto L19088;}
  else {
    if ((lo_c_1105X < 0)) {
      arg0K0 = x_475X;
      arg0K1 = y_476X;
      goto L19088;}
    else {
      if ((8192 < mid_c_1108X)) {
        arg0K0 = x_475X;
        arg0K1 = y_476X;
        goto L19088;}
      else {
        if ((a_477X < 0)) {
          if ((b_478X < 0)) {
            s48_make_availableAgc(16);
            if ((536870911 < c_1109X)) {
              goto L48992;}
            else {
              if ((c_1109X < -536870912)) {
                goto L48992;}
              else {
                arg0K0 = (((c_1109X)<<2));
                goto L48987;}}}
          else {
            goto L8114;}}
        else {
          if ((b_478X < 0)) {
            goto L8114;}
          else {
            s48_make_availableAgc(16);
            if ((536870911 < c_1109X)) {
              goto L49014;}
            else {
              if ((c_1109X < -536870912)) {
                goto L49014;}
              else {
                arg0K0 = (((c_1109X)<<2));
                goto L49009;}}}}}}}}
 L17127: {
  length0_1243X = arg0K0;
  extra0_1244X = arg0K1;
  if ((0 == (3 & y_480X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17135;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_480X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17135;}}
 L49095: {
  v_1245X = (char *) s48_long_to_bignum(c_1111X);
  v_1246X = enter_bignum(v_1245X);
  arg0K0 = v_1246X;
  goto L49090;}
 L49090: {
  val_1247X = arg0K0;
  SvalS = val_1247X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L8348: {
  x_1248X = 0 - c_1111X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1248X)) {
    goto L49073;}
  else {
    if ((x_1248X < -536870912)) {
      goto L49073;}
    else {
      arg0K0 = (((x_1248X)<<2));
      goto L49068;}}}
 L49117: {
  v_1249X = (char *) s48_long_to_bignum(c_1111X);
  v_1250X = enter_bignum(v_1249X);
  arg0K0 = v_1250X;
  goto L49112;}
 L49112: {
  val_1251X = arg0K0;
  SvalS = val_1251X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L16695: {
  length0_1252X = arg0K0;
  extra0_1253X = arg0K1;
  if ((0 == (3 & y_483X))) {
    arg0K0 = 3;
    goto L16703;}
  else {
    arg0K0 = 0;
    goto L16703;}}
 L33107: {
  val_1254X = arg0K0;
  SvalS = val_1254X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L33367: {
  val_1255X = arg0K0;
  SvalS = val_1255X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L33690: {
  val_1256X = arg0K0;
  SvalS = val_1256X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L34013: {
  val_1257X = arg0K0;
  SvalS = val_1257X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L34307: {
  val_1258X = arg0K0;
  SvalS = val_1258X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L19029: {
  extra1_1259X = arg0K0;
  Stemp0S = x_923X;
  Stemp1S = y_924X;
  s48_make_availableAgc(((((((8 + (((length0_1132X)<<1))) + extra0_1133X) + extra1_1259X))<<2)));
  value_1260X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1260X))) {
    v_1261X = (char *) s48_long_to_bignum((((value_1260X)>>2)));
    arg1K0 = v_1261X;
    goto L19015;}
  else {
    arg1K0 = (((char *) (-3 + value_1260X)));
    goto L19015;}}
 L8993: {
  if ((536870911 < c_1135X)) {
    arg0K0 = x_496X;
    arg0K1 = y_497X;
    goto L18997;}
  else {
    SvalS = (((c_1135X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}}
 L8992: {
  SvalS = ((((0 - c_1135X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L18938: {
  extra1_1262X = arg0K0;
  Stemp0S = x_926X;
  Stemp1S = y_927X;
  s48_make_availableAgc(((((((8 + (((length0_1136X)<<1))) + extra0_1137X) + extra1_1262X))<<2)));
  value_1263X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1263X))) {
    v_1264X = (char *) s48_long_to_bignum((((value_1263X)>>2)));
    arg1K0 = v_1264X;
    goto L18924;}
  else {
    arg1K0 = (((char *) (-3 + value_1263X)));
    goto L18924;}}
 L34817: {
  n_1265X = arg0K0;
  SvalS = (((n_1265X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L22512: {
  v_1266X = (char *) s48_long_to_bignum(x_1140X);
  return enter_bignum(v_1266X);}
 L18506: {
  x_1267X = arg1K0;
  v_1268X = s48_bignum_test(x_1267X);
  if ((-1 == v_1268X)) {
    v_1269X = (char *) s48_bignum_negate(x_1267X);
    arg1K0 = v_1269X;
    goto L18508;}
  else {
    arg1K0 = x_1267X;
    goto L18508;}}
 L18863: {
  v_1270X = arg0K0;
  Stemp0S = x_526X;
  s48_make_availableAgc((((((3 + v_1270X) + extra_1146X))<<2)));
  value_1271X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1271X))) {
    v_1272X = (char *) s48_long_to_bignum((((value_1271X)>>2)));
    arg1K0 = v_1272X;
    goto L18853;}
  else {
    arg1K0 = (((char *) (-3 + value_1271X)));
    goto L18853;}}
 L18805: {
  v_1273X = arg0K0;
  Stemp0S = x_527X;
  s48_make_availableAgc((((((3 + v_1273X) + extra_1150X))<<2)));
  value_1274X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1274X))) {
    v_1275X = (char *) s48_long_to_bignum((((value_1274X)>>2)));
    arg1K0 = v_1275X;
    goto L18795;}
  else {
    arg1K0 = (((char *) (-3 + value_1274X)));
    goto L18795;}}
 L16909: {
  length0_1276X = arg0K0;
  extra0_1277X = arg0K1;
  if ((0 == (3 & y_530X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16917;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_530X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16917;}}
 L16866: {
  length0_1278X = arg0K0;
  extra0_1279X = arg0K1;
  if ((0 == (3 & y_532X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16874;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_532X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16874;}}
 L16823: {
  length0_1280X = arg0K0;
  extra0_1281X = arg0K1;
  if ((0 == (3 & y_534X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L16831;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_534X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16831;}}
 L42157: {
  x_1282X = arg1K0;
  external_bignum_1283X = (char *) s48_bignum_arithmetic_shift(x_1282X, y_1159X);
  v_1284X = s48_bignum_fits_in_word_p(external_bignum_1283X, 30, 1);
  if (v_1284X) {
    n_1285X = s48_bignum_to_long(external_bignum_1283X);
    arg0K0 = (((n_1285X)<<2));
    goto L42085;}
  else {
    v_1286X = enter_bignum(external_bignum_1283X);
    arg0K0 = v_1286X;
    goto L42085;}}
 L42091: {
  val_1287X = arg0K0;
  SvalS = val_1287X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L20249: {
  foo_1288X = arg0K0;
  if ((1 == foo_1288X)) {
    if ((3 == (3 & bucket_1177X))) {
      arg0K0 = (-4 & bucket_1177X);
      goto L20254;}
    else {
      arg0K0 = bucket_1177X;
      goto L20254;}}
  else {
    s2_1289X = *((long *) (((char *) (-3 + foo_1288X))));
    len_1290X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_619X))) + -4))))>>8);
    if ((len_1290X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1289X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1289X))), (void *)(((char *) (-3 + string_619X))),len_1290X)))) {
        arg0K0 = foo_1288X;
        goto L32410;}
      else {
        goto L20269;}}
    else {
      goto L20269;}}}
 L36584: {
  v_1291X = ps_close_fd(index_1178X);
  arg0K0 = v_1291X;
  goto L36579;}
 L36579: {
  status_1292X = arg0K0;
  if ((status_1292X == NO_ERRORS)) {
    push_exception_continuationB(reason_1180X, 1);
    *((long *) (SstackS)) = arg2_628X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_629X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}
  else {
    channel_close_error(status_1292X, index_1178X, arg2_628X);
    push_exception_continuationB(reason_1180X, 1);
    *((long *) (SstackS)) = arg2_628X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_629X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L21732;}}
 L36911: {
  if ((3 == (3 & arg4_636X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_636X))) + -4)))))) {
      if ((3 == (3 & arg4_636X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_636X))) + -4))))>>2))))) {
          arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_636X))) + -4))))>>8)));
          goto L36918;}
        else {
          goto L37152;}}
      else {
        goto L37152;}}
    else {
      goto L36979;}}
  else {
    goto L36979;}}
 L37126: {
  if ((3 == (3 & arg4_636X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg4_636X))) + -4))))>>2))))) {
      goto L36911;}
    else {
      goto L36979;}}
  else {
    goto L36979;}}
 L36979: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = arg5_637X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_636X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1188X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1189X)<<2));
  SstackS = ((SstackS) + -4);
  if (waitP_1187X) {
    arg0K0 = 5;
    goto L36995;}
  else {
    arg0K0 = 1;
    goto L36995;}}
 L37341: {
  val_1293X = arg0K0;
  SvalS = val_1293X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L37624: {
  x_1294X = Scurrent_threadS;
  addr_1295X = (((char *) (-3 + x_1294X))) + 12;
  S48_WRITE_BARRIER(x_1294X, addr_1295X, proposal_658X);
  *((long *) addr_1295X) = proposal_658X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L37958: {
  copies_1296X = arg0K0;
  if ((1 == copies_1296X)) {
    log_1297X = *((long *) ((((char *) (-3 + proposal_659X))) + 4));
    arg0K0 = 0;
    goto L8772;}
  else {
    thing_1298X = *((long *) ((((char *) (-3 + copies_1296X))) + 8));
    if ((3 == (3 & thing_1298X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + thing_1298X))) + -4)))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + copies_1296X))) + 20)));
        goto L37958;}
      else {
        goto L37858;}}
    else {
      goto L37858;}}}
 L9520: {
  arg0K0 = (4 + i_1201X);
  goto L9468;}
 L10001: {
  entry_1299X = arg0K0;
  proposal_1300X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  value_1301X = Stemp0S;
  Stemp0S = 1;
  addr_1302X = ((char *) (-3 + entry_1299X));
  S48_WRITE_BARRIER(entry_1299X, addr_1302X, value_1301X);
  *((long *) addr_1302X) = value_1301X;
  addr_1303X = (((char *) (-3 + entry_1299X))) + 4;
  S48_WRITE_BARRIER(entry_1299X, addr_1303X, from_index_991X);
  *((long *) addr_1303X) = from_index_991X;
  value_1304X = Stemp1S;
  Stemp1S = 1;
  addr_1305X = (((char *) (-3 + entry_1299X))) + 8;
  S48_WRITE_BARRIER(entry_1299X, addr_1305X, value_1304X);
  *((long *) addr_1305X) = value_1304X;
  addr_1306X = (((char *) (-3 + entry_1299X))) + 12;
  S48_WRITE_BARRIER(entry_1299X, addr_1306X, to_index_992X);
  *((long *) addr_1306X) = to_index_992X;
  addr_1307X = (((char *) (-3 + entry_1299X))) + 16;
  S48_WRITE_BARRIER(entry_1299X, addr_1307X, count_993X);
  *((long *) addr_1307X) = count_993X;
  value_1308X = *((long *) ((((char *) (-3 + proposal_1300X))) + 12));
  addr_1309X = (((char *) (-3 + entry_1299X))) + 20;
  S48_WRITE_BARRIER(entry_1299X, addr_1309X, value_1308X);
  *((long *) addr_1309X) = value_1308X;
  addr_1310X = (((char *) (-3 + proposal_1300X))) + 12;
  S48_WRITE_BARRIER(proposal_1300X, addr_1310X, entry_1299X);
  *((long *) addr_1310X) = entry_1299X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L44964: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L44969: {
  if ((0 == (Spending_interruptsS))) {
    s48_wait_for_event((((arg2_733X)>>2)), minutesP_1210X);
    goto L44964;}
  else {
    goto L44964;}}
 L19756: {
  i_1311X = arg0K0;
  h_1312X = arg0K1;
  if ((i_1311X < n_1214X)) {
    arg0K0 = (1 + i_1311X);
    arg0K1 = (h_1312X + (((*((unsigned char *) ((((char *) (-3 + arg2_740X))) + i_1311X))))));
    goto L19756;}
  else {
    index_1313X = 1023 & h_1312X;
    link_1314X = *((long *) ((((char *) (-3 + table_1213X))) + (((index_1313X)<<2))));
    if ((0 == (3 & link_1314X))) {
      arg0K0 = (3 + (-4 & link_1314X));
      goto L19711;}
    else {
      arg0K0 = link_1314X;
      goto L19711;}}}
 L15267: {
  v_1315X = arg3K0;
  ps_write_string(v_1315X, out_785X);
  goto L40368;}
 L15233: {
  if ((3 == (3 & thing_1063X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_1063X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_1063X))))))))));
      goto L15267;}
    else {
      goto L15241;}}
  else {
    goto L15241;}}
 L26681: {
  count_1316X = arg0K0;
  i_1317X = arg0K1;
  offset_1318X = arg0K2;
  if ((0 == count_1316X)) {
    arg0K0 = i_1317X;
    arg0K1 = offset_1318X;
    arg0K2 = env_1217X;
    goto L26664;}
  else {
    value_1319X = *((long *) ((((char *) (-3 + env_1217X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1318X)))))<<2))));
    addr_1320X = (((char *) (-3 + new_env_412X))) + (((i_1317X)<<2));
    S48_WRITE_BARRIER(new_env_412X, addr_1320X, value_1319X);
    *((long *) addr_1320X) = value_1319X;
    arg0K0 = (-1 + count_1316X);
    arg0K1 = (1 + i_1317X);
    arg0K2 = (1 + offset_1318X);
    goto L26681;}}
 L26532: {
  count_1321X = arg0K0;
  i_1322X = arg0K1;
  offset_1323X = arg0K2;
  if ((0 == count_1321X)) {
    arg0K0 = i_1322X;
    arg0K1 = offset_1323X;
    arg0K2 = env_1220X;
    goto L26515;}
  else {
    value_1324X = *((long *) ((((char *) (-3 + env_1220X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1323X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1323X))))))<<2))));
    addr_1325X = (((char *) (-3 + new_env_419X))) + (((i_1322X)<<2));
    S48_WRITE_BARRIER(new_env_419X, addr_1325X, value_1324X);
    *((long *) addr_1325X) = value_1324X;
    arg0K0 = (-1 + count_1321X);
    arg0K1 = (1 + i_1322X);
    arg0K2 = (2 + offset_1323X);
    goto L26532;}}
 L21995: {
  arg_count_1326X = arg0K0;
  handlers_1327X = SHARED_REF((Sinterrupt_handlersS));
  if ((3 == (3 & handlers_1327X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_1327X))) + -4))))>>2))))) {
      goto L22013;}
    else {
      goto L22112;}}
  else {
    goto L22112;}}
 L11012: {
  channel_1328X = arg0K0;
  x_1329X = 1 == (Spending_channels_headS);
  if (x_1329X) {
    goto L11026;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L11026;}
      else {
        goto L11026;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L11026;}}}
 L21288: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 3;
      goto check_events;
     check_events_return_3:
      v_1330X = check_events0_return_value;
      if (v_1330X) {
        arg0K0 = stack_arg_count_1230X;
        goto L21965;}
      else {
        goto L21292;}}
    else {
      arg0K0 = stack_arg_count_1230X;
      goto L21965;}}
  else {
    goto L21292;}}
 L21422: {
  final_stack_arg_count_1331X = arg0K0;
  if ((stack_arg_count_818X < final_stack_arg_count_1331X)) {
    arg0K0 = final_stack_arg_count_1331X;
    goto L21426;}
  else {
    arg0K0 = stack_arg_count_818X;
    goto L21426;}}
 L25800: {
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_1094X));
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L39351: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L39550: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L39749: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L17178: {
  length1_1332X = arg0K0;
  extra1_1333X = arg0K1;
  if ((length0_1239X < length1_1332X)) {
    arg0K0 = length1_1332X;
    goto L17200;}
  else {
    arg0K0 = length0_1239X;
    goto L17200;}}
 L17090: {
  length0_1334X = arg0K0;
  extra0_1335X = arg0K1;
  if ((0 == (3 & y_1242X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17098;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_1242X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17098;}}
 L48992: {
  v_1336X = (char *) s48_long_to_bignum(c_1109X);
  v_1337X = enter_bignum(v_1336X);
  arg0K0 = v_1337X;
  goto L48987;}
 L48987: {
  val_1338X = arg0K0;
  SvalS = val_1338X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L8114: {
  x_1339X = 0 - c_1109X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1339X)) {
    goto L48970;}
  else {
    if ((x_1339X < -536870912)) {
      goto L48970;}
    else {
      arg0K0 = (((x_1339X)<<2));
      goto L48965;}}}
 L49014: {
  v_1340X = (char *) s48_long_to_bignum(c_1109X);
  v_1341X = enter_bignum(v_1340X);
  arg0K0 = v_1341X;
  goto L49009;}
 L49009: {
  val_1342X = arg0K0;
  SvalS = val_1342X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L17135: {
  length1_1343X = arg0K0;
  extra1_1344X = arg0K1;
  if ((length0_1243X < length1_1343X)) {
    arg0K0 = length1_1343X;
    goto L17157;}
  else {
    arg0K0 = length0_1243X;
    goto L17157;}}
 L49073: {
  v_1345X = (char *) s48_long_to_bignum(x_1248X);
  v_1346X = enter_bignum(v_1345X);
  arg0K0 = v_1346X;
  goto L49068;}
 L49068: {
  val_1347X = arg0K0;
  SvalS = val_1347X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L16703: {
  extra1_1348X = arg0K0;
  Stemp0S = x_482X;
  Stemp1S = y_483X;
  s48_make_availableAgc(((((((8 + (((length0_1252X)<<1))) + extra0_1253X) + extra1_1348X))<<2)));
  value_1349X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1349X))) {
    v_1350X = (char *) s48_long_to_bignum((((value_1349X)>>2)));
    arg1K0 = v_1350X;
    goto L18342;}
  else {
    arg1K0 = (((char *) (-3 + value_1349X)));
    goto L18342;}}
 L19015: {
  y_1351X = arg1K0;
  value_1352X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1352X))) {
    v_1353X = (char *) s48_long_to_bignum((((value_1352X)>>2)));
    arg1K0 = v_1353X;
    goto L19011;}
  else {
    arg1K0 = (((char *) (-3 + value_1352X)));
    goto L19011;}}
 L18924: {
  y_1354X = arg1K0;
  value_1355X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1355X))) {
    v_1356X = (char *) s48_long_to_bignum((((value_1355X)>>2)));
    arg1K0 = v_1356X;
    goto L18920;}
  else {
    arg1K0 = (((char *) (-3 + value_1355X)));
    goto L18920;}}
 L18508: {
  external_bignum_1357X = arg1K0;
  v_1358X = s48_bignum_fits_in_word_p(external_bignum_1357X, 30, 1);
  if (v_1358X) {
    n_1359X = s48_bignum_to_long(external_bignum_1357X);
    return (((n_1359X)<<2));}
  else {
    return enter_bignum(external_bignum_1357X);}}
 L18853: {
  v_1360X = arg1K0;
  external_bignum_1361X = (char *) s48_bignum_bitwise_not(v_1360X);
  v_1362X = s48_bignum_fits_in_word_p(external_bignum_1361X, 30, 1);
  if (v_1362X) {
    n_1363X = s48_bignum_to_long(external_bignum_1361X);
    arg0K0 = (((n_1363X)<<2));
    goto L35341;}
  else {
    val_1364X = enter_bignum(external_bignum_1361X);
    arg0K0 = val_1364X;
    goto L35341;}}
 L18795: {
  v_1365X = arg1K0;
  n_1366X = s48_bignum_bit_count(v_1365X);
  SvalS = (((n_1366X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L16917: {
  length1_1367X = arg0K0;
  extra1_1368X = arg0K1;
  if ((length0_1276X < length1_1367X)) {
    arg0K0 = length1_1367X;
    goto L16939;}
  else {
    arg0K0 = length0_1276X;
    goto L16939;}}
 L16874: {
  length1_1369X = arg0K0;
  extra1_1370X = arg0K1;
  if ((length0_1278X < length1_1369X)) {
    arg0K0 = length1_1369X;
    goto L16896;}
  else {
    arg0K0 = length0_1278X;
    goto L16896;}}
 L16831: {
  length1_1371X = arg0K0;
  extra1_1372X = arg0K1;
  if ((length0_1280X < length1_1371X)) {
    arg0K0 = length1_1371X;
    goto L16853;}
  else {
    arg0K0 = length0_1280X;
    goto L16853;}}
 L42085: {
  val_1373X = arg0K0;
  SvalS = val_1373X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L20254: {
  b_1374X = arg0K0;
  addr_1375X = s48_allocate_small(12);
  *((long *) addr_1375X) = 2054;
  x_1376X = 3 + (((long) (addr_1375X + 4)));
  *((long *) (((char *) (-3 + x_1376X)))) = string_619X;
  *((long *) ((((char *) (-3 + x_1376X))) + 4)) = b_1374X;
  if ((3 == (3 & x_1376X))) {
    arg0K0 = (-4 & x_1376X);
    goto L20260;}
  else {
    arg0K0 = x_1376X;
    goto L20260;}}
 L32410: {
  val_1377X = arg0K0;
  SvalS = val_1377X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L20269: {
  link_1378X = *((long *) ((((char *) (-3 + foo_1288X))) + 4));
  if ((0 == (3 & link_1378X))) {
    arg0K0 = (3 + (-4 & link_1378X));
    goto L20249;}
  else {
    arg0K0 = link_1378X;
    goto L20249;}}
 L36918: {
  y_1379X = arg0K0;
  if ((y_1379X < (start_1188X + count_1189X))) {
    goto L36979;}
  else {
    got_1380X = ps_read_fd(((((*((long *) ((((char *) (-3 + arg5_637X))) + 8))))>>2)), ((((char *) (-3 + arg4_636X))) + start_1188X), count_1189X, waitP_1187X, &eofP_1381X, &pendingP_1382X, &status_1383X);
    if ((status_1383X == NO_ERRORS)) {
      if (eofP_1381X) {
        arg0K0 = 21;
        goto L36978;}
      else {
        if (pendingP_1382X) {
          arg0K0 = 1;
          goto L36978;}
        else {
          arg0K0 = (((got_1380X)<<2));
          goto L36978;}}}
    else {
      push_exception_continuationB(24, 1);
      *((long *) (SstackS)) = arg5_637X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_636X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1188X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1189X)<<2));
      SstackS = ((SstackS) + -4);
      if (waitP_1187X) {
        arg0K0 = 5;
        goto L36961;}
      else {
        arg0K0 = 1;
        goto L36961;}}}}
 L37152: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_636X))) + -4))))>>8));
  goto L36918;}
 L36995: {
  x_1384X = arg0K0;
  *((long *) (SstackS)) = x_1384X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L21732;}
 L8772: {
  i_1385X = arg0K0;
  stob_1386X = *((long *) ((((char *) (-3 + log_1297X))) + (((i_1385X)<<2))));
  if ((1 == stob_1386X)) {
    log_1387X = *((long *) ((((char *) (-3 + proposal_659X))) + 8));
    arg0K0 = 0;
    goto L11679;}
  else {
    verify_1388X = *((long *) ((((char *) (-3 + log_1297X))) + (12 + (((i_1385X)<<2)))));
    value_1389X = *((long *) ((((char *) (-3 + log_1297X))) + (8 + (((i_1385X)<<2)))));
    if ((verify_1388X == value_1389X)) {
      goto L8804;}
    else {
      addr_1390X = (((char *) (-3 + stob_1386X))) + (-4 & (*((long *) ((((char *) (-3 + log_1297X))) + (4 + (((i_1385X)<<2)))))));
      S48_WRITE_BARRIER(stob_1386X, addr_1390X, value_1389X);
      *((long *) addr_1390X) = value_1389X;
      goto L8804;}}}
 L19711: {
  bucket_1391X = arg0K0;
  arg0K0 = 1;
  arg0K1 = bucket_1391X;
  goto L19718;}
 L15241: {
  if ((1 == thing_1063X)) {
    goto L15244;}
  else {
    if ((5 == thing_1063X)) {
      goto L15244;}
    else {
      if ((25 == thing_1063X)) {
        arg3K0 = "()";
        goto L15267;}
      else {
        if ((3 == (3 & thing_1063X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_1063X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L15267;}
          else {
            goto L15257;}}
        else {
          goto L15257;}}}}}
 L22013: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Spending_interruptPS = 0;
    if ((s48_Spending_eventsPS)) {
      s48_Spending_interruptPS = 1;
      goto L22015;}
    else {
      goto L22015;}}
  else {
    s48_Spending_interruptPS = 1;
    goto L22015;}}
 L22112: {
  ps_error("interrupt handler is not a vector", 0);
  goto L22013;}
 L11026: {
  *((long *) (SstackS)) = channel_1328X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + channel_1328X))) + 16)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L21995;}
 L21292: {
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L21426: {
  v_1392X = arg0K0;
  merged_arg0K0 = v_1392X;
  merged_arg0K1 = stack_arg_count_818X;
  merged_arg0K2 = list_args_819X;
  merged_arg0K3 = list_arg_count_820X;
  rest_list_setupAgc_return_tag = 1;
  goto rest_list_setupAgc;
 rest_list_setupAgc_return_1:
  *((long *) (SstackS)) = (((final_stack_arg_count_1331X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_822X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_1232X;
  arg0K1 = (3 + final_stack_arg_count_1331X);
  goto L21381;}
 L17200: {
  v_1393X = arg0K0;
  Stemp0S = x_472X;
  Stemp1S = y_473X;
  s48_make_availableAgc(((((((3 + v_1393X) + extra0_1240X) + extra1_1333X))<<2)));
  value_1394X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1394X))) {
    v_1395X = (char *) s48_long_to_bignum((((value_1394X)>>2)));
    arg1K0 = v_1395X;
    goto L19254;}
  else {
    arg1K0 = (((char *) (-3 + value_1394X)));
    goto L19254;}}
 L17098: {
  length1_1396X = arg0K0;
  extra1_1397X = arg0K1;
  Stemp0S = x_1241X;
  Stemp1S = y_1242X;
  s48_make_availableAgc(((((((2 + (length0_1334X + length1_1396X)) + extra0_1335X) + extra1_1397X))<<2)));
  value_1398X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1398X))) {
    v_1399X = (char *) s48_long_to_bignum((((value_1398X)>>2)));
    arg1K0 = v_1399X;
    goto L19106;}
  else {
    arg1K0 = (((char *) (-3 + value_1398X)));
    goto L19106;}}
 L48970: {
  v_1400X = (char *) s48_long_to_bignum(x_1339X);
  v_1401X = enter_bignum(v_1400X);
  arg0K0 = v_1401X;
  goto L48965;}
 L48965: {
  val_1402X = arg0K0;
  SvalS = val_1402X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L17157: {
  v_1403X = arg0K0;
  Stemp0S = x_479X;
  Stemp1S = y_480X;
  s48_make_availableAgc(((((((3 + v_1403X) + extra0_1244X) + extra1_1344X))<<2)));
  value_1404X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1404X))) {
    v_1405X = (char *) s48_long_to_bignum((((value_1404X)>>2)));
    arg1K0 = v_1405X;
    goto L19180;}
  else {
    arg1K0 = (((char *) (-3 + value_1404X)));
    goto L19180;}}
 L18342: {
  y_1406X = arg1K0;
  value_1407X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1407X))) {
    v_1408X = (char *) s48_long_to_bignum((((value_1407X)>>2)));
    arg1K0 = v_1408X;
    goto L18338;}
  else {
    arg1K0 = (((char *) (-3 + value_1407X)));
    goto L18338;}}
 L19011: {
  x_1409X = arg1K0;
  external_bignum_1410X = (char *)s48_bignum_quotient(x_1409X, y_1351X);
  v_1411X = s48_bignum_fits_in_word_p(external_bignum_1410X, 30, 1);
  if (v_1411X) {
    n_1412X = s48_bignum_to_long(external_bignum_1410X);
    arg0K0 = (((n_1412X)<<2));
    goto L49775;}
  else {
    val_1413X = enter_bignum(external_bignum_1410X);
    arg0K0 = val_1413X;
    goto L49775;}}
 L18920: {
  x_1414X = arg1K0;
  external_bignum_1415X = (char *)s48_bignum_remainder(x_1414X, y_1354X);
  v_1416X = s48_bignum_fits_in_word_p(external_bignum_1415X, 30, 1);
  if (v_1416X) {
    n_1417X = s48_bignum_to_long(external_bignum_1415X);
    arg0K0 = (((n_1417X)<<2));
    goto L49775;}
  else {
    val_1418X = enter_bignum(external_bignum_1415X);
    arg0K0 = val_1418X;
    goto L49775;}}
 L35341: {
  val_1419X = arg0K0;
  SvalS = val_1419X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L16939: {
  v_1420X = arg0K0;
  Stemp0S = x_529X;
  Stemp1S = y_530X;
  s48_make_availableAgc(((((((3 + v_1420X) + extra0_1277X) + extra1_1368X))<<2)));
  value_1421X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1421X))) {
    v_1422X = (char *) s48_long_to_bignum((((value_1421X)>>2)));
    arg1K0 = v_1422X;
    goto L18728;}
  else {
    arg1K0 = (((char *) (-3 + value_1421X)));
    goto L18728;}}
 L16896: {
  v_1423X = arg0K0;
  Stemp0S = x_531X;
  Stemp1S = y_532X;
  s48_make_availableAgc(((((((3 + v_1423X) + extra0_1279X) + extra1_1370X))<<2)));
  value_1424X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1424X))) {
    v_1425X = (char *) s48_long_to_bignum((((value_1424X)>>2)));
    arg1K0 = v_1425X;
    goto L18654;}
  else {
    arg1K0 = (((char *) (-3 + value_1424X)));
    goto L18654;}}
 L16853: {
  v_1426X = arg0K0;
  Stemp0S = x_533X;
  Stemp1S = y_534X;
  s48_make_availableAgc(((((((3 + v_1426X) + extra0_1281X) + extra1_1372X))<<2)));
  value_1427X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1427X))) {
    v_1428X = (char *) s48_long_to_bignum((((value_1427X)>>2)));
    arg1K0 = v_1428X;
    goto L18580;}
  else {
    arg1K0 = (((char *) (-3 + value_1427X)));
    goto L18580;}}
 L20260: {
  value_1429X = arg0K0;
  addr_1430X = (((char *) (-3 + table_618X))) + (((index_961X)<<2));
  S48_WRITE_BARRIER(table_618X, addr_1430X, value_1429X);
  *((long *) addr_1430X) = value_1429X;
  arg0K0 = x_1376X;
  goto L32410;}
 L36978: {
  val_1431X = arg0K0;
  SvalS = val_1431X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L36961: {
  x_1432X = arg0K0;
  *((long *) (SstackS)) = x_1432X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_1383X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 5;
  goto get_error_string;
 get_error_string_return_5:
  x_1433X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_1433X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 6;
  goto L21732;}
 L11679: {
  i_1434X = arg0K0;
  stob_1435X = *((long *) ((((char *) (-3 + log_1387X))) + (((i_1434X)<<2))));
  if ((1 == stob_1435X)) {
    copies_1436X = *((long *) ((((char *) (-3 + proposal_659X))) + 12));
    arg0K0 = copies_1436X;
    goto L11594;}
  else {
    verify_1437X = *((long *) ((((char *) (-3 + log_1387X))) + (12 + (((i_1434X)<<2)))));
    value_1438X = *((long *) ((((char *) (-3 + log_1387X))) + (8 + (((i_1434X)<<2)))));
    if ((verify_1437X == value_1438X)) {
      goto L11711;}
    else {
      *((unsigned char *) ((((char *) (-3 + stob_1435X))) + ((((*((long *) ((((char *) (-3 + log_1387X))) + (4 + (((i_1434X)<<2)))))))>>2)))) = (((value_1438X)>>2));
      goto L11711;}}}
 L8804: {
  arg0K0 = (4 + i_1385X);
  goto L8772;}
 L19718: {
  previous_foo_1439X = arg0K0;
  foo_1440X = arg0K1;
  if ((1 == foo_1440X)) {
    goto L31066;}
  else {
    s2_1441X = *((long *) (((char *) (-3 + foo_1440X))));
    len_1442X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_740X))) + -4))))>>8);
    if ((len_1442X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1441X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1441X))), (void *)(((char *) (-3 + arg2_740X))),len_1442X)))) {
        if ((1 == previous_foo_1439X)) {
          value_1443X = *((long *) ((((char *) (-3 + foo_1440X))) + 12));
          addr_1444X = (((char *) (-3 + table_1213X))) + (((index_1313X)<<2));
          S48_WRITE_BARRIER(table_1213X, addr_1444X, value_1443X);
          *((long *) addr_1444X) = value_1443X;
          goto L31066;}
        else {
          val_1445X = *((long *) ((((char *) (-3 + foo_1440X))) + 12));
          addr_1446X = (((char *) (-3 + previous_foo_1439X))) + 12;
          S48_WRITE_BARRIER(previous_foo_1439X, addr_1446X, val_1445X);
          *((long *) addr_1446X) = val_1445X;
          goto L31066;}}
      else {
        goto L19786;}}
    else {
      goto L19786;}}}
 L15244: {
  if ((1 == thing_1063X)) {
    arg3K0 = "#f";
    goto L15267;}
  else {
    arg3K0 = "#t";
    goto L15267;}}
 L15257: {
  if ((3 == (3 & thing_1063X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_1063X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L15267;}
    else {
      goto L15261;}}
  else {
    goto L15261;}}
 L22015: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_1327X))) + (((i_1224X)<<2)))));
  obj_1447X = SvalS;
  if ((3 == (3 & obj_1447X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1447X))) + -4))))>>2))))) {
      goto L22032;}
    else {
      goto L22137;}}
  else {
    goto L22137;}}
 L19254: {
  y_1448X = arg1K0;
  value_1449X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1449X))) {
    v_1450X = (char *) s48_long_to_bignum((((value_1449X)>>2)));
    arg1K0 = v_1450X;
    goto L19250;}
  else {
    arg1K0 = (((char *) (-3 + value_1449X)));
    goto L19250;}}
 L19106: {
  y_1451X = arg1K0;
  value_1452X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1452X))) {
    v_1453X = (char *) s48_long_to_bignum((((value_1452X)>>2)));
    arg1K0 = v_1453X;
    goto L19102;}
  else {
    arg1K0 = (((char *) (-3 + value_1452X)));
    goto L19102;}}
 L19180: {
  y_1454X = arg1K0;
  value_1455X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1455X))) {
    v_1456X = (char *) s48_long_to_bignum((((value_1455X)>>2)));
    arg1K0 = v_1456X;
    goto L19176;}
  else {
    arg1K0 = (((char *) (-3 + value_1455X)));
    goto L19176;}}
 L18338: {
  x_1457X = arg1K0;
  div_by_zeroP_1458X = s48_bignum_divide(x_1457X, y_1406X, &quot_1459X, &rem_1460X);
  if (div_by_zeroP_1458X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = 0;
    goto L41755;}
  else {
    v_1461X = s48_bignum_fits_in_word_p(rem_1460X, 30, 1);
    if (v_1461X) {
      n_1462X = s48_bignum_to_long(rem_1460X);
      arg0K0 = (((n_1462X)<<2));
      goto L18365;}
    else {
      v_1463X = enter_bignum(rem_1460X);
      arg0K0 = v_1463X;
      goto L18365;}}}
 L49775: {
  val_1464X = arg0K0;
  SvalS = val_1464X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L18728: {
  y_1465X = arg1K0;
  value_1466X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1466X))) {
    v_1467X = (char *) s48_long_to_bignum((((value_1466X)>>2)));
    arg1K0 = v_1467X;
    goto L18724;}
  else {
    arg1K0 = (((char *) (-3 + value_1466X)));
    goto L18724;}}
 L18654: {
  y_1468X = arg1K0;
  value_1469X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1469X))) {
    v_1470X = (char *) s48_long_to_bignum((((value_1469X)>>2)));
    arg1K0 = v_1470X;
    goto L18650;}
  else {
    arg1K0 = (((char *) (-3 + value_1469X)));
    goto L18650;}}
 L18580: {
  y_1471X = arg1K0;
  value_1472X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1472X))) {
    v_1473X = (char *) s48_long_to_bignum((((value_1472X)>>2)));
    arg1K0 = v_1473X;
    goto L18576;}
  else {
    arg1K0 = (((char *) (-3 + value_1472X)));
    goto L18576;}}
 L11594: {
  copies_1474X = arg0K0;
  if ((1 == copies_1474X)) {
    value_1475X = Sempty_logS;
    addr_1476X = (((char *) (-3 + proposal_659X))) + 4;
    S48_WRITE_BARRIER(proposal_659X, addr_1476X, value_1475X);
    *((long *) addr_1476X) = value_1475X;
    value_1477X = Sempty_logS;
    addr_1478X = (((char *) (-3 + proposal_659X))) + 8;
    S48_WRITE_BARRIER(proposal_659X, addr_1478X, value_1477X);
    *((long *) addr_1478X) = value_1477X;
    addr_1479X = (((char *) (-3 + proposal_659X))) + 12;
    S48_WRITE_BARRIER(proposal_659X, addr_1479X, 1);
    *((long *) addr_1479X) = 1;
    RELEASE_PROPOSAL_LOCK();
    x_1480X = Scurrent_threadS;
    addr_1481X = (((char *) (-3 + x_1480X))) + 12;
    S48_WRITE_BARRIER(x_1480X, addr_1481X, 1);
    *((long *) addr_1481X) = 1;
    SvalS = 5;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23677;}
  else {
    stob_1482X = *((long *) (((char *) (-3 + copies_1474X))));
    v_1483X = (((*((long *) ((((char *) (-3 + copies_1474X))) + 4))))>>2);
    stob_1484X = *((long *) ((((char *) (-3 + copies_1474X))) + 8));
    v_1485X = (((*((long *) ((((char *) (-3 + copies_1474X))) + 12))))>>2);
    v_1486X = (((*((long *) ((((char *) (-3 + copies_1474X))) + 16))))>>2);
    memcpy((void *)((((char *) (-3 + stob_1484X))) + v_1485X), (void *)((((char *) (-3 + stob_1482X))) + v_1483X),v_1486X);
    arg0K0 = (*((long *) ((((char *) (-3 + copies_1474X))) + 20)));
    goto L11594;}}
 L11711: {
  arg0K0 = (4 + i_1434X);
  goto L11679;}
 L31066: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L19786: {
  link_1487X = *((long *) ((((char *) (-3 + foo_1440X))) + 12));
  if ((0 == (3 & link_1487X))) {
    arg0K0 = foo_1440X;
    arg0K1 = (3 + (-4 & link_1487X));
    goto L19718;}
  else {
    arg0K0 = foo_1440X;
    arg0K1 = link_1487X;
    goto L19718;}}
 L15261: {
  if ((3 == (3 & thing_1063X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_1063X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L15267;}
    else {
      arg3K0 = "???";
      goto L15267;}}
  else {
    arg3K0 = "???";
    goto L15267;}}
 L22032: {
  StemplateS = (SvalS);
  Slosing_opcodeS = (0 - i_1224X);
  arg0K0 = arg_count_1326X;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L21362;}
 L22137: {
  ps_error("interrupt handler is not a closure", 1, i_1224X);
  goto L22032;}
 L19250: {
  x_1488X = arg1K0;
  external_bignum_1489X = (char *)s48_bignum_add(x_1488X, y_1448X);
  v_1490X = s48_bignum_fits_in_word_p(external_bignum_1489X, 30, 1);
  if (v_1490X) {
    n_1491X = s48_bignum_to_long(external_bignum_1489X);
    arg0K0 = (((n_1491X)<<2));
    goto L32531;}
  else {
    val_1492X = enter_bignum(external_bignum_1489X);
    arg0K0 = val_1492X;
    goto L32531;}}
 L19102: {
  x_1493X = arg1K0;
  external_bignum_1494X = (char *)s48_bignum_multiply(x_1493X, y_1451X);
  v_1495X = s48_bignum_fits_in_word_p(external_bignum_1494X, 30, 1);
  if (v_1495X) {
    n_1496X = s48_bignum_to_long(external_bignum_1494X);
    arg0K0 = (((n_1496X)<<2));
    goto L49775;}
  else {
    val_1497X = enter_bignum(external_bignum_1494X);
    arg0K0 = val_1497X;
    goto L49775;}}
 L19176: {
  x_1498X = arg1K0;
  external_bignum_1499X = (char *)s48_bignum_subtract(x_1498X, y_1454X);
  v_1500X = s48_bignum_fits_in_word_p(external_bignum_1499X, 30, 1);
  if (v_1500X) {
    n_1501X = s48_bignum_to_long(external_bignum_1499X);
    arg0K0 = (((n_1501X)<<2));
    goto L32820;}
  else {
    val_1502X = enter_bignum(external_bignum_1499X);
    arg0K0 = val_1502X;
    goto L32820;}}
 L41755: {
  div_by_zeroP_1503X = arg2K0;
  quot_1504X = arg0K1;
  rem_1505X = arg0K2;
  if (div_by_zeroP_1503X) {
    goto L41776;}
  else {
    if ((0 == (3 & rem_1505X))) {
      if ((0 == rem_1505X)) {
        SvalS = quot_1504X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23677;}
      else {
        goto L41776;}}
    else {
      goto L41776;}}}
 L18365: {
  v_1506X = arg0K0;
  v_1507X = s48_bignum_fits_in_word_p(quot_1459X, 30, 1);
  if (v_1507X) {
    n_1508X = s48_bignum_to_long(quot_1459X);
    arg2K0 = 0;
    arg0K1 = (((n_1508X)<<2));
    arg0K2 = v_1506X;
    goto L41755;}
  else {
    v_1509X = enter_bignum(quot_1459X);
    arg2K0 = 0;
    arg0K1 = v_1509X;
    arg0K2 = v_1506X;
    goto L41755;}}
 L18724: {
  x_1510X = arg1K0;
  external_bignum_1511X = (char *) s48_bignum_bitwise_and(x_1510X, y_1465X);
  v_1512X = s48_bignum_fits_in_word_p(external_bignum_1511X, 30, 1);
  if (v_1512X) {
    n_1513X = s48_bignum_to_long(external_bignum_1511X);
    arg0K0 = (((n_1513X)<<2));
    goto L35523;}
  else {
    val_1514X = enter_bignum(external_bignum_1511X);
    arg0K0 = val_1514X;
    goto L35523;}}
 L18650: {
  x_1515X = arg1K0;
  external_bignum_1516X = (char *) s48_bignum_bitwise_ior(x_1515X, y_1468X);
  v_1517X = s48_bignum_fits_in_word_p(external_bignum_1516X, 30, 1);
  if (v_1517X) {
    n_1518X = s48_bignum_to_long(external_bignum_1516X);
    arg0K0 = (((n_1518X)<<2));
    goto L35686;}
  else {
    val_1519X = enter_bignum(external_bignum_1516X);
    arg0K0 = val_1519X;
    goto L35686;}}
 L18576: {
  x_1520X = arg1K0;
  external_bignum_1521X = (char *) s48_bignum_bitwise_xor(x_1520X, y_1471X);
  v_1522X = s48_bignum_fits_in_word_p(external_bignum_1521X, 30, 1);
  if (v_1522X) {
    n_1523X = s48_bignum_to_long(external_bignum_1521X);
    arg0K0 = (((n_1523X)<<2));
    goto L35849;}
  else {
    val_1524X = enter_bignum(external_bignum_1521X);
    arg0K0 = val_1524X;
    goto L35849;}}
 L32531: {
  val_1525X = arg0K0;
  SvalS = val_1525X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L32820: {
  val_1526X = arg0K0;
  SvalS = val_1526X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L41776: {
  push_exception_continuationB(5, 1);
  *((long *) (SstackS)) = x_482X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_483X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L21732;}
 L35523: {
  val_1527X = arg0K0;
  SvalS = val_1527X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L35686: {
  val_1528X = arg0K0;
  SvalS = val_1528X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 L35849: {
  val_1529X = arg0K0;
  SvalS = val_1529X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23677;}
 loseD0: {
  message_387X = merged_arg3K0;{
  why_1530X = (((*((long *) ((SstackS) + (4 + (((nargs_786X)<<2)))))))>>2);
  ps_write_string("Template UIDs: ", (stderr));
  current_template_1531X = StemplateS;
  out_1532X = stderr;
  if ((0 == (3 & (*((long *) ((((char *) (-3 + current_template_1531X))) + 4)))))) {
    if ((current_template_1531X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
      arg2K0 = 0;
      goto L13379;}
    else {
      ps_write_integer(((((*((long *) ((((char *) (-3 + current_template_1531X))) + 4))))>>2)), out_1532X);
      arg2K0 = 1;
      goto L13379;}}
  else {
    arg2K0 = 0;
    goto L13379;}}
 L13379: {
  not_firstP_1533X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_1533X;
  goto L13386;}
 L13386: {
  cont_1534X = arg0K0;
  not_firstP_1535X = arg2K1;
  if ((3 == (3 & cont_1534X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_1534X))) + -4))))>>2))))) {
      template_1536X = *((long *) ((((char *) (-3 + cont_1534X))) + 8));
      if ((0 == (3 & (*((long *) ((((char *) (-3 + template_1536X))) + 4)))))) {
        if ((template_1536X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
          arg2K0 = not_firstP_1535X;
          goto L13395;}
        else {
          if (not_firstP_1535X) {
            ps_write_string(" <- ", out_1532X);
            goto L13434;}
          else {
            goto L13434;}}}
      else {
        arg2K0 = not_firstP_1535X;
        goto L13395;}}
    else {
      goto L21751;}}
  else {
    goto L21751;}}
 L13395: {
  v_1537X = arg2K0;
  arg0K0 = (*((long *) (((char *) (-3 + cont_1534X)))));
  arg2K1 = v_1537X;
  goto L13386;}
 L13434: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_1536X))) + 4))))>>2)), out_1532X);
  arg2K0 = 1;
  goto L13395;}
 L21751: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_1530X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_786X)<<2))))))))))))) {
      ps_error(message_387X, 3, opcode_787X, why_1530X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_786X)<<2)))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L21775;}}
  else {
    goto L21775;}}
 L21775: {
  ps_error(message_387X, 2, opcode_787X, why_1530X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L49784;}
 L49784: {
  type_1538X = s48_get_next_event(&channel_1539X, &status_1540X);
  if ((type_1538X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L21181;}
    else {
      arg0K0 = 1;
      goto L21181;}}
  else {
    if ((type_1538X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L21181;}
    else {
      if ((type_1538X == IO_COMPLETION_EVENT)) {
        enqueue_channelB(channel_1539X, status_1540X);
        arg0K0 = 8;
        goto L21181;}
      else {
        if ((type_1538X == OS_SIGNAL_EVENT)) {
          arg0K0 = 16;
          goto L21181;}
        else {
          if ((type_1538X == NO_EVENT)) {
            arg0K0 = 0;
            goto L21181;}
          else {
            if ((type_1538X == ERROR_EVENT)) {
              ps_write_string("OS error while getting event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              ps_write_string((ps_error_string(status_1540X)), (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L21181;}
            else {
              ps_write_string("unknown type of event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L21181;}}}}}}}
 L21181: {
  v_1541X = arg0K0;
  Spending_interruptsS = (v_1541X | (Spending_interruptsS));
  if ((type_1538X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L21208;}
      else {
        goto L21208;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L21208;}}
  else {
    goto L49784;}}
 L21208: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      goto L49784;}
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
    x_1542X = copy_listSAgc0_return_value;
    *((long *) (SstackS)) = x_1542X;
    SstackS = ((SstackS) + -4);
    goto rest_list_setupAgc_return;}
  else {
    if ((stack_arg_count_384X < wants_stack_args_383X)) {
      count_1543X = wants_stack_args_383X - stack_arg_count_384X;
      merged_arg0K0 = list_args_385X;
      merged_arg0K1 = count_1543X;
      push_list_return_tag = 4;
      goto push_list;
     push_list_return_4:
      v_1544X = push_list0_return_value;
      merged_arg0K0 = v_1544X;
      merged_arg0K1 = (list_arg_count_386X - count_1543X);
      copy_listSAgc_return_tag = 2;
      goto copy_listSAgc;
     copy_listSAgc_return_2:
      x_1545X = copy_listSAgc0_return_value;
      *((long *) (SstackS)) = x_1545X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setupAgc_return;}
    else {
      merged_arg0K0 = list_args_385X;
      merged_arg0K1 = list_arg_count_386X;
      copy_listSAgc_return_tag = 3;
      goto copy_listSAgc;
     copy_listSAgc_return_3:
      v_1546X = copy_listSAgc0_return_value;
      merged_arg0K0 = v_1546X;
      merged_arg0K1 = (stack_arg_count_384X - wants_stack_args_383X);
      pop_args_GlistSAgc_return_tag = 11;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_11:
      x_1547X = pop_args_GlistSAgc0_return_value;
      *((long *) (SstackS)) = x_1547X;
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
    goto L21025;}
  else {
    s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
    merged_arg0K0 = 0;
    copy_stack_into_heap_return_tag = 4;
    goto copy_stack_into_heap;
   copy_stack_into_heap_return_4:
    if ((count_382X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L21025;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L21025;}}}
 L21025: {
  SstackS = ((SstackS) + 4);
  list_1548X = *((long *) (SstackS));
  arg0K0 = count_382X;
  arg0K1 = list_1548X;
  goto L21034;}
 L21034: {
  i_1549X = arg0K0;
  l_1550X = arg0K1;
  if ((0 < i_1549X)) {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1550X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (-1 + i_1549X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1550X))) + 4)));
    goto L21034;}
  else {
    push_list0_return_value = l_1550X;
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
  goto L20599;}
 L20599: {
  p_1551X = arg1K0;
  i_1552X = arg0K1;
  if ((2 == (3 & (*((long *) p_1551X))))) {
    if ((26 == (*((long *) p_1551X)))) {
      arg0K0 = (-1 + i_1552X);
      goto L20579;}
    else {
      arg0K0 = i_1552X;
      goto L20579;}}
  else {
    arg1K0 = (p_1551X + 4);
    arg0K1 = (1 + i_1552X);
    goto L20599;}}
 L20579: {
  arg_count_1553X = arg0K0;
  top_1554X = SstackS;
  if ((1 == (ScontS))) {
    goto L20584;}
  else {
    merged_arg0K0 = key_380X;
    merged_arg0K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L20584;}}
 L20584: {
  SstackS = ((((char *) (-3 + (ScontS)))) + -8);
  arg1K0 = (SstackS);
  arg1K1 = (top_1554X + (((arg_count_1553X)<<2)));
  goto L20628;}
 L20628: {
  loc_1555X = arg1K0;
  arg_1556X = arg1K1;
  if ((top_1554X < arg_1556X)) {
    *((long *) loc_1555X) = (*((long *) arg_1556X));
    arg1K0 = (loc_1555X + -4);
    arg1K1 = (arg_1556X + -4);
    goto L20628;}
  else {
    SstackS = loc_1555X;
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
  p_1557X = SenvS;
  if ((3 == (3 & p_1557X))) {
    if ((p_1557X < (((long) (Sstack_beginS))))) {
      goto L19523;}
    else {
      if (((((long) (Sstack_endS))) < p_1557X)) {
        goto L19523;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_378X;
        merged_arg0K3 = reason_379X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_1558X = save_env_in_heap0_return_value;
        SenvS = v_1558X;
        goto L19523;}}}
  else {
    goto L19523;}}
 L19523: {
  end_1559X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L19476;}
 L19476: {
  cont_1560X = arg0K0;
  previous_1561X = arg0K1;
  if ((cont_1560X == (Sbottom_of_stackS))) {
    *((long *) (((char *) (-3 + previous_1561X)))) = end_1559X;
    ScontS = (Sbottom_of_stackS);
    really_preserve_continuation0_return_value = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
    goto really_preserve_continuation_return;}
  else {
    p_1562X = *((long *) ((((char *) (-3 + cont_1560X))) + 12));
    if ((3 == (3 & p_1562X))) {
      if ((p_1562X < (((long) (Sstack_beginS))))) {
        goto L19494;}
      else {
        if (((((long) (Sstack_endS))) < p_1562X)) {
          goto L19494;}
        else {
          merged_arg0K0 = (*((long *) ((((char *) (-3 + cont_1560X))) + 12)));
          merged_arg0K1 = cont_1560X;
          merged_arg0K2 = key_378X;
          merged_arg0K3 = reason_379X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L19494;}}}
    else {
      goto L19494;}}}
 L19494: {
  header_1563X = *((long *) ((((char *) (-3 + cont_1560X))) + -4));
  addr_1564X = s48_allocate_small((4 + ((long)(((unsigned long)header_1563X)>>8))));
  data_addr_1565X = addr_1564X + 4;
  *((long *) addr_1564X) = header_1563X;
  memcpy((void *)data_addr_1565X, (void *)(((char *) (-3 + cont_1560X))),((long)(((unsigned long)header_1563X)>>8)));
  new_1566X = 3 + (((long) data_addr_1565X));
  *((long *) (((char *) (-3 + previous_1561X)))) = new_1566X;
  arg0K0 = (*((long *) (((char *) (-3 + new_1566X)))));
  arg0K1 = new_1566X;
  goto L19476;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 save_env_in_heap: {
  env_376X = merged_arg0K0;
  cont_377X = merged_arg0K1;{
  header_1567X = *((long *) ((((char *) (-3 + env_376X))) + -4));
  addr_1568X = s48_allocate_small((4 + ((long)(((unsigned long)header_1567X)>>8))));
  data_addr_1569X = addr_1568X + 4;
  *((long *) addr_1568X) = header_1567X;
  memcpy((void *)data_addr_1569X, (void *)(((char *) (-3 + env_376X))),((long)(((unsigned long)header_1567X)>>8)));
  new_1570X = 3 + (((long) data_addr_1569X));
  addr_1571X = ((char *) (-3 + env_376X));
  S48_WRITE_BARRIER(env_376X, addr_1571X, 26);
  *((long *) addr_1571X) = 26;
  *((long *) ((((char *) (-3 + env_376X))) + -4)) = new_1570X;
  arg0K0 = new_1570X;
  goto L16436;}
 L16436: {
  env_1572X = arg0K0;
  p_1573X = *((long *) (((char *) (-3 + env_1572X))));
  if ((3 == (3 & p_1573X))) {
    if ((p_1573X < (((long) (Sstack_beginS))))) {
      goto L16647;}
    else {
      if (((((long) (Sstack_endS))) < p_1573X)) {
        goto L16647;}
      else {
        env_1574X = *((long *) (((char *) (-3 + env_1572X))));
        header_1575X = *((long *) ((((char *) (-3 + env_1574X))) + -4));
        addr_1576X = s48_allocate_small((4 + ((long)(((unsigned long)header_1575X)>>8))));
        data_addr_1577X = addr_1576X + 4;
        *((long *) addr_1576X) = header_1575X;
        memcpy((void *)data_addr_1577X, (void *)(((char *) (-3 + env_1574X))),((long)(((unsigned long)header_1575X)>>8)));
        new_1578X = 3 + (((long) data_addr_1577X));
        addr_1579X = ((char *) (-3 + env_1574X));
        S48_WRITE_BARRIER(env_1574X, addr_1579X, 26);
        *((long *) addr_1579X) = 26;
        *((long *) ((((char *) (-3 + env_1574X))) + -4)) = new_1578X;
        addr_1580X = ((char *) (-3 + env_1572X));
        S48_WRITE_BARRIER(env_1572X, addr_1580X, new_1578X);
        *((long *) addr_1580X) = new_1578X;
        arg0K0 = new_1578X;
        goto L16436;}}}
  else {
    goto L16647;}}
 L16647: {
  arg0K0 = cont_377X;
  goto L16659;}
 L16659: {
  cont_1581X = arg0K0;
  env_1582X = *((long *) ((((char *) (-3 + cont_1581X))) + 12));
  if ((3 == (3 & env_1582X))) {
    if ((3 == (3 & (*((long *) ((((char *) (-3 + env_1582X))) + -4)))))) {
      *((long *) ((((char *) (-3 + cont_1581X))) + 12)) = (*((long *) ((((char *) (-3 + env_1582X))) + -4)));
      arg0K0 = (*((long *) (((char *) (-3 + cont_1581X)))));
      goto L16659;}
    else {
      save_env_in_heap0_return_value = new_1570X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = new_1570X;
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
  value_1583X = Stemp0S;
  Stemp0S = 1;
  arg0K0 = value_1583X;
  arg0K1 = count_375X;
  goto L15978;}
 L15978: {
  args_1584X = arg0K0;
  count_1585X = arg0K1;
  if ((0 == count_1585X)) {
    pop_args_GlistSAgc0_return_value = args_1584X;
    goto pop_args_GlistSAgc_return;}
  else {
    SstackS = ((SstackS) + 4);
    a_1586X = *((long *) (SstackS));
    addr_1587X = s48_allocate_small(12);
    *((long *) addr_1587X) = 2050;
    x_1588X = 3 + (((long) (addr_1587X + 4)));
    *((long *) (((char *) (-3 + x_1588X)))) = a_1586X;
    *((long *) ((((char *) (-3 + x_1588X))) + 4)) = args_1584X;
    arg0K0 = x_1588X;
    arg0K1 = (-1 + count_1585X);
    goto L15978;}}
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
    value_1589X = Stemp0S;
    Stemp0S = 1;
    a_1590X = *((long *) (((char *) (-3 + value_1589X))));
    addr_1591X = s48_allocate_small(12);
    *((long *) addr_1591X) = 2050;
    x_1592X = 3 + (((long) (addr_1591X + 4)));
    *((long *) (((char *) (-3 + x_1592X)))) = a_1590X;
    *((long *) ((((char *) (-3 + x_1592X))) + 4)) = 25;
    arg0K0 = (*((long *) ((((char *) (-3 + value_1589X))) + 4)));
    arg0K1 = x_1592X;
    goto L15866;}}
 L15866: {
  l_1593X = arg0K0;
  last_1594X = arg0K1;
  if ((25 == l_1593X)) {
    copy_listSAgc0_return_value = x_1592X;
    goto copy_listSAgc_return;}
  else {
    a_1595X = *((long *) (((char *) (-3 + l_1593X))));
    addr_1596X = s48_allocate_small(12);
    *((long *) addr_1596X) = 2050;
    x_1597X = 3 + (((long) (addr_1596X + 4)));
    *((long *) (((char *) (-3 + x_1597X)))) = a_1595X;
    *((long *) ((((char *) (-3 + x_1597X))) + 4)) = 25;
    addr_1598X = (((char *) (-3 + last_1594X))) + 4;
    S48_WRITE_BARRIER(last_1594X, addr_1598X, x_1597X);
    *((long *) addr_1598X) = x_1597X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_1593X))) + 4)));
    arg0K1 = x_1597X;
    goto L15866;}}
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
    goto L16728;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_370X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L16728;}}
 L16728: {
  x_size_1599X = arg0K0;
  extra_1600X = arg0K1;
  if ((n_371X < 0)) {
    if ((x_size_1599X < 1)) {
      arg0K0 = 1;
      goto L16773;}
    else {
      arg0K0 = x_size_1599X;
      goto L16773;}}
  else {
    n_1601X = n_371X / 30;
    arg0K0 = (5 + (x_size_1599X + n_1601X));
    goto L16748;}}
 L16773: {
  v_1602X = arg0K0;
  arg0K0 = (9 + ((((v_1602X)<<1)) + x_size_1599X));
  goto L16748;}
 L16748: {
  v_1603X = arg0K0;
  shift_space0_return_value = (extra_1600X + v_1603X);
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
        goto L15468;}}
    else {
      goto L15468;}}}
 L15468: {
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
  top_1604X = (((char *) (-3 + (Sbottom_of_stackS)))) + (-8 - (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_365X))) + -4))))>>8)))));
  new_cont_1605X = 3 + (((long) (top_1604X + 4)));
  if ((0 == stack_arg_count_366X)) {
    SstackS = (top_1604X + -4);
    goto L14658;}
  else {
    new_stack_1606X = top_1604X + (-4 - (((stack_arg_count_366X)<<2)));
    if ((new_stack_1606X < (SstackS))) {
      x_1607X = SstackS;
      memcpy((void *)(new_stack_1606X + 4), (void *)(x_1607X + 4),(((stack_arg_count_366X)<<2)));
      SstackS = new_stack_1606X;
      goto L14658;}
    else {
      goto L14658;}}}
 L14658: {
  ScontS = new_cont_1605X;
  v_1608X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_365X))) + -4))))>>8))))>>2);
  memcpy((void *)top_1604X, (void *)((((char *) (-3 + cont_365X))) + -4),(4 + (((v_1608X)<<2))));
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = (*((long *) (((char *) (-3 + new_cont_1605X)))));
  *((long *) (((char *) (-3 + new_cont_1605X)))) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_1605X;
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
  log_1609X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L14429;}
 L14429: {
  i_1610X = arg0K0;
  next_stob_1611X = *((long *) ((((char *) (-3 + log_1609X))) + (((i_1610X)<<2))));
  if ((1 == next_stob_1611X)) {
    add_log_entryAgc(1, i_1610X, stob_362X, index_363X, value_364X, 0);
    goto proposal_d_write_return;}
  else {
    if ((stob_362X == next_stob_1611X)) {
      if ((index_363X == (*((long *) ((((char *) (-3 + log_1609X))) + (4 + (((i_1610X)<<2)))))))) {
        addr_1612X = (((char *) (-3 + log_1609X))) + (8 + (((i_1610X)<<2)));
        S48_WRITE_BARRIER(log_1609X, addr_1612X, value_364X);
        *((long *) addr_1612X) = value_364X;
        goto proposal_d_write_return;}
      else {
        goto L14449;}}
    else {
      goto L14449;}}}
 L14449: {
  arg0K0 = (4 + i_1610X);
  goto L14429;}
 proposal_d_write_return:
  switch (proposal_d_write_return_tag) {
  case 0: goto proposal_d_write_return_0;
  case 1: goto proposal_d_write_return_1;
  default: goto proposal_d_write_return_2;
  }}

 proposal_d_read: {
  stob_360X = merged_arg0K0;
  index_361X = merged_arg0K1;{
  log_1613X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L14252;}
 L14252: {
  i_1614X = arg0K0;
  next_stob_1615X = *((long *) ((((char *) (-3 + log_1613X))) + (((i_1614X)<<2))));
  if ((1 == next_stob_1615X)) {
    v_1616X = add_log_entryAgc(1, i_1614X, stob_360X, index_361X, (*((long *) ((((char *) (-3 + stob_360X))) + (-4 & index_361X)))), 1);
    proposal_d_read0_return_value = v_1616X;
    goto proposal_d_read_return;}
  else {
    if ((stob_360X == next_stob_1615X)) {
      if ((index_361X == (*((long *) ((((char *) (-3 + log_1613X))) + (4 + (((i_1614X)<<2)))))))) {
        proposal_d_read0_return_value = (*((long *) ((((char *) (-3 + log_1613X))) + (8 + (((i_1614X)<<2))))));
        goto proposal_d_read_return;}
      else {
        goto L14274;}}
    else {
      goto L14274;}}}
 L14274: {
  arg0K0 = (4 + i_1614X);
  goto L14252;}
 proposal_d_read_return:
  switch (proposal_d_read_return_tag) {
  case 0: goto proposal_d_read_return_0;
  case 1: goto proposal_d_read_return_1;
  default: goto proposal_d_read_return_2;
  }}

 make_closure: {
  a_358X = merged_arg0K0;
  b_359X = merged_arg0K1;{
  addr_1617X = s48_allocate_small(12);
  *((long *) addr_1617X) = 2062;
  x_1618X = 3 + (((long) (addr_1617X + 4)));
  *((long *) (((char *) (-3 + x_1618X)))) = a_358X;
  *((long *) ((((char *) (-3 + x_1618X))) + 4)) = b_359X;
  if ((3 == (3 & x_1618X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_1618X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_1618X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_1618X))) + -4))));
      make_closure0_return_value = x_1618X;
      goto make_closure_return;}
    else {
      make_closure0_return_value = x_1618X;
      goto make_closure_return;}}
  else {
    make_closure0_return_value = x_1618X;
    goto make_closure_return;}}
 make_closure_return:
  switch (make_closure_return_tag) {
  case 0: goto make_closure_return_0;
  default: goto make_closure_return_1;
  }}

 get_current_port: {
  marker_357X = merged_arg0K0;{
  thread_1619X = Scurrent_threadS;
  if ((3 == (3 & thread_1619X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thread_1619X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_1619X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_1619X))) + 4)));
        goto L12836;}
      else {
        goto L12886;}}
    else {
      goto L12886;}}
  else {
    goto L12886;}}
 L12836: {
  env_1620X = arg0K0;
  if ((3 == (3 & env_1620X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_1620X))) + -4))))>>2))))) {
      obj_1621X = *((long *) (((char *) (-3 + env_1620X))));
      if ((3 == (3 & obj_1621X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_1621X))) + -4))))>>2))))) {
          if ((marker_357X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_1620X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_1620X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_1620X))) + 4)));
            goto L12836;}}
        else {
          goto L12908;}}
      else {
        goto L12908;}}
    else {
      goto L12908;}}
  else {
    goto L12908;}}
 L12886: {
  ps_error("current thread is not a record", 0);
  get_current_port0_return_value = v_1622X;
  goto get_current_port_return;}
 L12908: {
  if ((25 == env_1620X)) {
    if (((((marker_357X)>>2)) == 1)) {
      arg3K0 = "dynamic environment doesn't have current-output-port";
      goto L12862;}
    else {
      arg3K0 = "dynamic environment doesn't have current-input-port";
      goto L12862;}}
  else {
    ps_error("dynamic environment is not a proper list", 0);
    get_current_port0_return_value = v_1623X;
    goto get_current_port_return;}}
 L12862: {
  v_1624X = arg3K0;
  ps_error(v_1624X, 0);
  get_current_port0_return_value = v_1625X;
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
  goto L12703;}
 L12703: {
  fast_1626X = arg0K0;
  len_1627X = arg0K1;
  slow_1628X = arg0K2;
  move_slowP_1629X = arg2K3;
  if ((25 == fast_1626X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_1627X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_1626X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_1626X))) + -4))))>>2))))) {
        if (move_slowP_1629X) {
          if ((fast_1626X == slow_1628X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_1626X))) + 4)));
            arg0K1 = (1 + len_1627X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_1628X))) + 4)));
            arg2K3 = 0;
            goto L12703;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_1626X))) + 4)));
          arg0K1 = (1 + len_1627X);
          arg0K2 = slow_1628X;
          arg2K3 = 1;
          goto L12703;}}
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
  string_1630X = ps_error_string(status_355X);
  x_1631X = strlen((char *) string_1630X);
  if ((x_1631X < 256)) {
    arg0K0 = x_1631X;
    goto L11892;}
  else {
    arg0K0 = 256;
    goto L11892;}}
 L11892: {
  len_1632X = arg0K0;
  len_1633X = 1 + len_1632X;
  addr_1634X = s48_allocate_small((4 + len_1633X));
  *((long *) addr_1634X) = (70 + (((len_1633X)<<8)));
  string_1635X = 3 + (((long) (addr_1634X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1635X))) + len_1632X)) = 0;
  arg0K0 = 0;
  goto L11902;}
 L11902: {
  i_1636X = arg0K0;
  if ((i_1636X == len_1632X)) {
    get_error_string0_return_value = string_1635X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1635X))) + i_1636X)) = ((*(string_1630X + i_1636X)));
    arg0K0 = (1 + i_1636X);
    goto L11902;}}
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
{ cont_1637X = ScontS;
  tem_1638X = *((long *) ((((char *) (-3 + cont_1637X))) + 8));
  pc_1639X = *((long *) ((((char *) (-3 + cont_1637X))) + 4));
  StemplateS = tem_1638X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1638X)))))))) + (((pc_1639X)>>2)));
  SenvS = (*((long *) ((((char *) (-3 + cont_1637X))) + 12)));
  ScontS = (*((long *) (((char *) (-3 + cont_1637X)))));
  SstackS = ((((char *) (-3 + cont_1637X))) + 12);
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
long s48_call_startup_procedure(char **startup_vector_1640X, long startup_vector_length_1641X)
{
  long arg0K0;
  long merged_arg0K1;
  char *merged_arg3K0;

  int enter_string_return_tag;
  long enter_string0_return_value;
  char *string_1642X;
  long i_1682X;
  long string_1681X;
  char * addr_1680X;
  long len_1679X;
  long len_1678X;
  long v_1677X;
  long x_1676X;
  long x_1675X;
  char * addr_1674X;
  long b_1673X;
  long channel_1672X;
  long x_1671X;
  char * addr_1670X;
  long b_1669X;
  long channel_1668X;
  long x_1667X;
  char * addr_1666X;
  long b_1665X;
  long channel_1664X;
  long i_1663X;
  long length_1662X;
  long *v_1661X;
  long v_1660X;
  long v_1659X;
  long y_1658X;
  long x_1657X;
  long v_1656X;
  long x_1655X;
  long y_1654X;
  char * addr_1653X;
  long value_1652X;
  long x_1651X;
  long y_1650X;
  long i_1649X;
  long vector_1648X;
  long v_1647X;
  long vector_1646X;
  char * addr_1645X;
  long len_1644X;
  long tem_1643X;
 {  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  SenvS = 13;
  tem_1643X = Sinterrupt_templateS;
  StemplateS = tem_1643X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1643X))))))));
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
  len_1644X = ((startup_vector_length_1641X)<<2);
  addr_1645X = s48_allocate_tracedAgc((4 + len_1644X));
  if ((addr_1645X == NULL)) {
    arg0K0 = 1;
    goto L16380;}
  else {
    *((long *) addr_1645X) = (10 + (((len_1644X)<<8)));
    arg0K0 = (3 + (((long) (addr_1645X + 4))));
    goto L16380;}}
 L16380: {
  vector_1646X = arg0K0;
  if ((1 == vector_1646X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1647X;
    goto L16366;}
  else {
    arg0K0 = vector_1646X;
    goto L16366;}}
 L16366: {
  vector_1648X = arg0K0;
  arg0K0 = 0;
  goto L16403;}
 L16403: {
  i_1649X = arg0K0;
  if ((i_1649X == startup_vector_length_1641X)) {
    *((long *) (SstackS)) = vector_1648X;
    SstackS = ((SstackS) + -4);
    y_1650X = fileno((stderr));
    x_1651X = fileno((stdout));
    if ((x_1651X < y_1650X)) {
      arg0K0 = y_1650X;
      goto L18090;}
    else {
      arg0K0 = x_1651X;
      goto L18090;}}
  else {
    value_1652X = enter_stringAgc((*(startup_vector_1640X + i_1649X)));
    addr_1653X = (((char *) (-3 + vector_1648X))) + (((i_1649X)<<2));
    S48_WRITE_BARRIER(vector_1648X, addr_1653X, value_1652X);
    *((long *) addr_1653X) = value_1652X;
    arg0K0 = (1 + i_1649X);
    goto L16403;}}
 L18090: {
  y_1654X = arg0K0;
  x_1655X = fileno((stdin));
  if ((x_1655X < y_1654X)) {
    arg0K0 = y_1654X;
    goto L18092;}
  else {
    arg0K0 = x_1655X;
    goto L18092;}}
 L18092: {
  v_1656X = arg0K0;
  x_1657X = Snumber_of_channelsS;
  y_1658X = 1 + v_1656X;
  if ((x_1657X < y_1658X)) {
    arg0K0 = y_1658X;
    goto L18094;}
  else {
    arg0K0 = x_1657X;
    goto L18094;}}
 L18094: {
  v_1659X = arg0K0;
  Snumber_of_channelsS = v_1659X;
  v_1660X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L18116;}
  else {
    goto L18116;}}
 L18116: {
  v_1661X = Svm_channelsS;
  length_1662X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L18164;}
 L18164: {
  i_1663X = arg0K0;
  if ((i_1663X < length_1662X)) {
    *(v_1661X + i_1663X) = 1;
    arg0K0 = (1 + i_1663X);
    goto L18164;}
  else {
    s48_make_availableAgc(((((3 * (7 + ((((4 + (strlen((char *) "standard output"))))>>2)))))<<2)));
    channel_1664X = fileno((stdin));
    merged_arg3K0 = "standard input";
    merged_arg0K1 = 0;
    enter_string_return_tag = 0;
    goto enter_string;
   enter_string_return_0:
    b_1665X = enter_string0_return_value;
    addr_1666X = s48_allocate_small(24);
    *((long *) addr_1666X) = 5146;
    x_1667X = 3 + (((long) (addr_1666X + 4)));
    *((long *) (((char *) (-3 + x_1667X)))) = 4;
    *((long *) ((((char *) (-3 + x_1667X))) + 4)) = b_1665X;
    *((long *) ((((char *) (-3 + x_1667X))) + 8)) = (((channel_1664X)<<2));
    *((long *) ((((char *) (-3 + x_1667X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1667X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1664X) = x_1667X;
    channel_1668X = fileno((stderr));
    merged_arg3K0 = "standard error";
    merged_arg0K1 = 0;
    enter_string_return_tag = 1;
    goto enter_string;
   enter_string_return_1:
    b_1669X = enter_string0_return_value;
    addr_1670X = s48_allocate_small(24);
    *((long *) addr_1670X) = 5146;
    x_1671X = 3 + (((long) (addr_1670X + 4)));
    *((long *) (((char *) (-3 + x_1671X)))) = 8;
    *((long *) ((((char *) (-3 + x_1671X))) + 4)) = b_1669X;
    *((long *) ((((char *) (-3 + x_1671X))) + 8)) = (((channel_1668X)<<2));
    *((long *) ((((char *) (-3 + x_1671X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1671X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1668X) = x_1671X;
    channel_1672X = fileno((stdout));
    merged_arg3K0 = "standard output";
    merged_arg0K1 = 0;
    enter_string_return_tag = 2;
    goto enter_string;
   enter_string_return_2:
    b_1673X = enter_string0_return_value;
    addr_1674X = s48_allocate_small(24);
    *((long *) addr_1674X) = 5146;
    x_1675X = 3 + (((long) (addr_1674X + 4)));
    *((long *) (((char *) (-3 + x_1675X)))) = 8;
    *((long *) ((((char *) (-3 + x_1675X))) + 4)) = b_1673X;
    *((long *) ((((char *) (-3 + x_1675X))) + 8)) = (((channel_1672X)<<2));
    *((long *) ((((char *) (-3 + x_1675X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1675X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1672X) = x_1675X;
    *((long *) (SstackS)) = x_1667X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1675X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1671X;
    SstackS = ((SstackS) + -4);
    x_1676X = s48_resumer_records();
    *((long *) (SstackS)) = x_1676X;
    SstackS = ((SstackS) + -4);
    s48_initialization_completeB();
    v_1677X = s48_startup_procedure();
    return s48_restart(v_1677X, 5);}}
 enter_string: {
  string_1642X = merged_arg3K0;{
  len_1678X = strlen((char *) string_1642X);
  len_1679X = 1 + len_1678X;
  addr_1680X = s48_allocate_small((4 + len_1679X));
  *((long *) addr_1680X) = (70 + (((len_1679X)<<8)));
  string_1681X = 3 + (((long) (addr_1680X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1681X))) + len_1678X)) = 0;
  arg0K0 = 0;
  goto L14044;}
 L14044: {
  i_1682X = arg0K0;
  if ((i_1682X == len_1678X)) {
    enter_string0_return_value = string_1681X;
    goto enter_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1681X))) + i_1682X)) = ((*(string_1642X + i_1682X)));
    arg0K0 = (1 + i_1682X);
    goto L14044;}}
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
