#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm-prelude.h"

static char add_more_channels(long);
static void enqueue_channelB(long, long);
static long close_channelB(long);
static void push_exception_continuationB(long, long);
static long add_log_entryAgc(long, long, long, long, long, char);
static long enter_bignum(char *);
static long enter_stringAgc(char*);
static long channel_close_error(long, long, long);
static long make_registered_channel(long, long, long, long, long*);
static long save_env_in_heap(long, char *, long, long);
static long really_preserve_continuation(long, long);
static long Hlookup2720(long, long, long);
static long Hlookup2701(long, long, long);
static void HtopD10292(void);
static void HtopD10303(void);
void s48_set_native_protocolB(long);
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
static char * Sbottom_of_stackS;
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
static char * *Simpure_areasS;
static long *Simpure_sizesS;
static long Simpure_area_countS;
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
char * ScontS;
long SenvS;
long StemplateS;
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
    goto L3021;}
  else {
    arg0K0 = x_5X;
    goto L3021;}}
 L3021: {
  new_count_7X = arg0K0;
  new_vm_channels_8X = (long*)malloc(sizeof(long) * new_count_7X);
  if ((NULL == new_vm_channels_8X)) {
    return 0;}
  else {
    arg0K0 = 0;
    goto L3033;}}
 L3033: {
  i_9X = arg0K0;
  if ((i_9X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L3106;}
  else {
    *(new_vm_channels_8X + i_9X) = (*((Svm_channelsS) + i_9X));
    arg0K0 = (1 + i_9X);
    goto L3033;}}
 L3106: {
  i_10X = arg0K0;
  if ((i_10X == new_count_7X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_8X;
    Snumber_of_channelsS = new_count_7X;
    return 1;}
  else {
    *(new_vm_channels_8X + i_10X) = 1;
    arg0K0 = (1 + i_10X);
    goto L3106;}}
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
    goto L10806;}
  else {
    goto L10806;}}
 L10806: {
  v_22X = *((long *) (((char *) (-3 + channel_18X))));
  if ((4 == v_22X)) {
    goto L10821;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_18X))))))) {
      goto L10821;}
    else {
      v_23X = ps_close_fd(os_index_19X);
      arg0K0 = v_23X;
      goto L10828;}}}
 L10821: {
  v_24X = ps_close_fd(os_index_19X);
  arg0K0 = v_24X;
  goto L10828;}
 L10828: {
  status_25X = arg0K0;
  *((Svm_channelsS) + os_index_19X) = 1;
  addr_26X = ((char *) (-3 + channel_18X));
  S48_WRITE_BARRIER(channel_18X, addr_26X, 0);
  *((long *) addr_26X) = 0;
  return status_25X;}
}
static void push_exception_continuationB(long exception_27X, long instruction_size_28X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  char * cont_39X;
  long template_38X;
  char * code_pointer_37X;
  long tem_36X;
  long data_35X;
  long v_34X;
  long i_33X;
  char * p_32X;
  long pc_31X;
  long template_30X;
  long opcode_29X;
 {  opcode_29X = *((unsigned char *) (Scode_pointerS));
  template_30X = StemplateS;
  pc_31X = ((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2);
  arg1K0 = (SstackS);
  arg0K1 = 0;
  goto L5576;}
 L5576: {
  p_32X = arg1K0;
  i_33X = arg0K1;
  if ((p_32X == (ScontS))) {
    arg0K0 = i_33X;
    goto L5550;}
  else {
    if ((26 == (*((long *) p_32X)))) {
      arg0K0 = (-1 + i_33X);
      goto L5550;}
    else {
      if ((2 == (3 & (*((long *) p_32X))))) {
        arg0K0 = i_33X;
        goto L5550;}
      else {
        arg1K0 = (p_32X + 4);
        arg0K1 = (1 + i_33X);
        goto L5576;}}}}
 L5550: {
  v_34X = arg0K0;
  SstackS = ((SstackS) + -20);
  data_35X = 3 + (((long) (SstackS)));
  *((long *) (((char *) (-3 + data_35X)))) = (36 + (((v_34X)<<2)));
  *((long *) ((((char *) (-3 + data_35X))) + 4)) = pc_31X;
  *((long *) ((((char *) (-3 + data_35X))) + 8)) = template_30X;
  *((long *) ((((char *) (-3 + data_35X))) + 12)) = (((exception_27X)<<2));
  *((long *) ((((char *) (-3 + data_35X))) + 16)) = (((instruction_size_28X)<<2));
  tem_36X = Sexception_templateS;
  StemplateS = tem_36X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_36X)))))))) + 3);
  code_pointer_37X = Scode_pointerS;
  template_38X = StemplateS;
  SstackS = ((SstackS) + -16);
  cont_39X = SstackS;
  *((long *) cont_39X) = (((long) code_pointer_37X));
  *((long *) (cont_39X + 8)) = template_38X;
  *((long *) (cont_39X + 12)) = (SenvS);
  *((long *) (cont_39X + 4)) = (((long) (ScontS)));
  ScontS = cont_39X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((opcode_29X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_27X)<<2));
  return;}
}
static long add_log_entryAgc(long proposal_index_40X, long i_41X, long stob_42X, long index_43X, long value_44X, char verifyP_45X)
{
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char * addr_71X;
  long value_70X;
  long value_69X;
  long v_68X;
  char * addr_67X;
  long i_66X;
  long stob_65X;
  long proposal_64X;
  long new_63X;
  char * addr_62X;
  char * addr_61X;
  long value_60X;
  long v_59X;
  long vector_58X;
  char * addr_57X;
  char * addr_56X;
  char * addr_55X;
  long log_54X;
  long value_53X;
  long stob_52X;
  long proposal_51X;
  char * addr_50X;
  long len_49X;
  long new_size_48X;
  long log_size_47X;
  long proposal_46X;
 {  proposal_46X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  log_size_47X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + proposal_46X))) + (((proposal_index_40X)<<2)))))))) + -4))))>>8))))>>2);
  if ((i_41X == (-1 + log_size_47X))) {
    Stemp0S = stob_42X;
    Stemp1S = value_44X;
    if ((1 == log_size_47X)) {
      arg0K0 = 17;
      goto L9416;}
    else {
      arg0K0 = (-1 + (((log_size_47X)<<1)));
      goto L9416;}}
  else {
    arg0K0 = proposal_46X;
    arg0K1 = stob_42X;
    arg0K2 = value_44X;
    goto L12622;}}
 L9416: {
  new_size_48X = arg0K0;
  len_49X = ((new_size_48X)<<2);
  addr_50X = s48_allocate_tracedAgc((4 + len_49X));
  if ((addr_50X == NULL)) {
    arg0K0 = 1;
    goto L9462;}
  else {
    *((long *) addr_50X) = (10 + (((len_49X)<<8)));
    arg0K0 = (3 + (((long) (addr_50X + 4))));
    goto L9462;}}
 L12622: {
  proposal_51X = arg0K0;
  stob_52X = arg0K1;
  value_53X = arg0K2;
  log_54X = *((long *) ((((char *) (-3 + proposal_51X))) + (((proposal_index_40X)<<2))));
  addr_55X = (((char *) (-3 + log_54X))) + (((i_41X)<<2));
  S48_WRITE_BARRIER(log_54X, addr_55X, stob_52X);
  *((long *) addr_55X) = stob_52X;
  addr_56X = (((char *) (-3 + log_54X))) + (4 + (((i_41X)<<2)));
  S48_WRITE_BARRIER(log_54X, addr_56X, index_43X);
  *((long *) addr_56X) = index_43X;
  addr_57X = (((char *) (-3 + log_54X))) + (8 + (((i_41X)<<2)));
  S48_WRITE_BARRIER(log_54X, addr_57X, value_53X);
  *((long *) addr_57X) = value_53X;
  if (verifyP_45X) {
    arg0K0 = value_53X;
    goto L12643;}
  else {
    arg0K0 = 29;
    goto L12643;}}
 L9462: {
  vector_58X = arg0K0;
  if ((1 == vector_58X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_59X;
    goto L9420;}
  else {
    arg0K0 = vector_58X;
    goto L9420;}}
 L12643: {
  value_60X = arg0K0;
  addr_61X = (((char *) (-3 + log_54X))) + (12 + (((i_41X)<<2)));
  S48_WRITE_BARRIER(log_54X, addr_61X, value_60X);
  *((long *) addr_61X) = value_60X;
  addr_62X = (((char *) (-3 + log_54X))) + (16 + (((i_41X)<<2)));
  S48_WRITE_BARRIER(log_54X, addr_62X, 1);
  *((long *) addr_62X) = 1;
  return value_53X;}
 L9420: {
  new_63X = arg0K0;
  proposal_64X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 < log_size_47X)) {
    stob_65X = *((long *) ((((char *) (-3 + proposal_64X))) + 4));
    memcpy((void *)(((char *) (-3 + new_63X))), (void *)(((char *) (-3 + stob_65X))),(-4 + (((log_size_47X)<<2))));
    goto L9440;}
  else {
    goto L9440;}}
 L9440: {
  arg0K0 = (4 + log_size_47X);
  goto L9444;}
 L9444: {
  i_66X = arg0K0;
  if ((i_66X == new_size_48X)) {
    addr_67X = (((char *) (-3 + proposal_64X))) + (((proposal_index_40X)<<2));
    S48_WRITE_BARRIER(proposal_64X, addr_67X, new_63X);
    *((long *) addr_67X) = new_63X;
    v_68X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
    value_69X = Stemp1S;
    Stemp1S = 1;
    value_70X = Stemp0S;
    Stemp0S = 1;
    arg0K0 = v_68X;
    arg0K1 = value_70X;
    arg0K2 = value_69X;
    goto L12622;}
  else {
    addr_71X = (((char *) (-3 + new_63X))) + (((i_66X)<<2));
    S48_WRITE_BARRIER(new_63X, addr_71X, 0);
    *((long *) addr_71X) = 0;
    arg0K0 = (1 + i_66X);
    goto L9444;}}
}
static long enter_bignum(char * external_bignum_72X)
{
  long desc_73X;
 {  desc_73X = 3 + (((long) external_bignum_72X));
  if ((3 == (3 & desc_73X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + desc_73X))) + -4))))>>2))))) {
      goto L13373;}
    else {
      goto L13387;}}
  else {
    goto L13387;}}
 L13373: {
  if ((3 == (3 & desc_73X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + desc_73X))) + -4)))))) {
      *((long *) ((((char *) (-3 + desc_73X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + desc_73X))) + -4))));
      return desc_73X;}
    else {
      return desc_73X;}}
  else {
    return desc_73X;}}
 L13387: {
  ps_error("not a bignum", 1, desc_73X);
  goto L13373;}
}
static long enter_stringAgc(char *string_74X)
{
  long arg0K0;
  long i_79X;
  long string_78X;
  char * addr_77X;
  long len_76X;
  long z_75X;
 {  z_75X = strlen((char *) string_74X);
  s48_make_availableAgc((4 + (-4 & (4 + z_75X))));
  len_76X = 1 + z_75X;
  addr_77X = s48_allocate_small((4 + len_76X));
  *((long *) addr_77X) = (70 + (((len_76X)<<8)));
  string_78X = 3 + (((long) (addr_77X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_78X))) + z_75X)) = 0;
  arg0K0 = 0;
  goto L14702;}
 L14702: {
  i_79X = arg0K0;
  if ((i_79X == z_75X)) {
    return string_78X;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_78X))) + i_79X)) = ((*(string_74X + i_79X)));
    arg0K0 = (1 + i_79X);
    goto L14702;}}
}
static long channel_close_error(long status_80X, long index_81X, long id_82X)
{
  long v_83X;
 {  ps_write_string("Error: ", (stderr));
  ps_write_string((ps_error_string(status_80X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(" while closing port ", (stderr));
  if ((3 == (3 & id_82X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + id_82X))) + -4))))>>2))))) {
      ps_write_string((((char *)(((char *) (-3 + id_82X))))), (stderr));
      goto L15557;}
    else {
      goto L15551;}}
  else {
    goto L15551;}}
 L15557: {
  PS_WRITE_CHAR(10, (stderr), v_83X)
  return v_83X;}
 L15551: {
  ps_write_integer((((index_81X)>>2)), (stderr));
  goto L15557;}
}
static long make_registered_channel(long mode_84X, long id_85X, long os_index_86X, long key_87X, long *TT0)
{
  long x_91X;
  char * addr_90X;
  char x_89X;
  char temp_88X;
 {  temp_88X = os_index_86X < (Snumber_of_channelsS);
  if (temp_88X) {
    goto L16712;}
  else {
    x_89X = add_more_channels(os_index_86X);
    if (x_89X) {
      goto L16712;}
    else {
      *TT0 = 9;
      return 1;}}}
 L16712: {
  if ((1 == (*((Svm_channelsS) + os_index_86X)))) {
    addr_90X = s48_allocate_small(24);
    *((long *) addr_90X) = 5146;
    x_91X = 3 + (((long) (addr_90X + 4)));
    *((long *) (((char *) (-3 + x_91X)))) = (((mode_84X)<<2));
    *((long *) ((((char *) (-3 + x_91X))) + 4)) = id_85X;
    *((long *) ((((char *) (-3 + x_91X))) + 8)) = (((os_index_86X)<<2));
    *((long *) ((((char *) (-3 + x_91X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_91X))) + 16)) = 1;
    *((Svm_channelsS) + os_index_86X) = x_91X;
    *TT0 = 9;
    return x_91X;}
  else {
    *TT0 = 11;
    return 1;}}
}
static long save_env_in_heap(long env_92X, char * cont_93X, long key_94X, long reason_95X)
{
  long arg0K0;
  char * arg1K0;
  long env_111X;
  char * cont_110X;
  char * addr_109X;
  char * addr_108X;
  long new_107X;
  char * data_addr_106X;
  char * addr_105X;
  long header_104X;
  long env_103X;
  long p_102X;
  long env_101X;
  char * addr_100X;
  long new_99X;
  char * data_addr_98X;
  char * addr_97X;
  long header_96X;
 {  header_96X = *((long *) ((((char *) (-3 + env_92X))) + -4));
  addr_97X = s48_allocate_small((4 + ((long)(((unsigned long)header_96X)>>8))));
  data_addr_98X = addr_97X + 4;
  *((long *) addr_97X) = header_96X;
  memcpy((void *)data_addr_98X, (void *)(((char *) (-3 + env_92X))),((long)(((unsigned long)header_96X)>>8)));
  new_99X = 3 + (((long) data_addr_98X));
  addr_100X = ((char *) (-3 + env_92X));
  S48_WRITE_BARRIER(env_92X, addr_100X, 26);
  *((long *) addr_100X) = 26;
  *((long *) ((((char *) (-3 + env_92X))) + -4)) = new_99X;
  arg0K0 = new_99X;
  goto L17067;}
 L17067: {
  env_101X = arg0K0;
  p_102X = *((long *) (((char *) (-3 + env_101X))));
  if ((3 == (3 & p_102X))) {
    if ((p_102X < (((long) (Sstack_beginS))))) {
      goto L17278;}
    else {
      if (((((long) (Sstack_endS))) < p_102X)) {
        goto L17278;}
      else {
        env_103X = *((long *) (((char *) (-3 + env_101X))));
        header_104X = *((long *) ((((char *) (-3 + env_103X))) + -4));
        addr_105X = s48_allocate_small((4 + ((long)(((unsigned long)header_104X)>>8))));
        data_addr_106X = addr_105X + 4;
        *((long *) addr_105X) = header_104X;
        memcpy((void *)data_addr_106X, (void *)(((char *) (-3 + env_103X))),((long)(((unsigned long)header_104X)>>8)));
        new_107X = 3 + (((long) data_addr_106X));
        addr_108X = ((char *) (-3 + env_103X));
        S48_WRITE_BARRIER(env_103X, addr_108X, 26);
        *((long *) addr_108X) = 26;
        *((long *) ((((char *) (-3 + env_103X))) + -4)) = new_107X;
        addr_109X = ((char *) (-3 + env_101X));
        S48_WRITE_BARRIER(env_101X, addr_109X, new_107X);
        *((long *) addr_109X) = new_107X;
        arg0K0 = new_107X;
        goto L17067;}}}
  else {
    goto L17278;}}
 L17278: {
  arg1K0 = cont_93X;
  goto L17290;}
 L17290: {
  cont_110X = arg1K0;
  env_111X = *((long *) (cont_110X + 12));
  if ((3 == (3 & env_111X))) {
    if ((3 == (3 & (*((long *) ((((char *) (-3 + env_111X))) + -4)))))) {
      *((long *) (cont_110X + 12)) = (*((long *) ((((char *) (-3 + env_111X))) + -4)));
      arg1K0 = (((char *) (*((long *) (cont_110X + 4)))));
      goto L17290;}
    else {
      return new_99X;}}
  else {
    return new_99X;}}
}
static long really_preserve_continuation(long key_112X, long reason_113X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long new_125X;
  char * addr_124X;
  long len_123X;
  long size_122X;
  long size_121X;
  char * code_pointer_120X;
  long p_119X;
  long previous_118X;
  char * cont_117X;
  long end_116X;
  long v_115X;
  long p_114X;
 {  p_114X = SenvS;
  if ((3 == (3 & p_114X))) {
    if ((p_114X < (((long) (Sstack_beginS))))) {
      goto L20052;}
    else {
      if (((((long) (Sstack_endS))) < p_114X)) {
        goto L20052;}
      else {
        v_115X = save_env_in_heap((SenvS), (ScontS), key_112X, reason_113X);
        SenvS = v_115X;
        goto L20052;}}}
  else {
    goto L20052;}}
 L20052: {
  end_116X = *((long *) ((Sbottom_of_stackS) + 4));
  arg1K0 = (ScontS);
  arg0K1 = 1;
  goto L19997;}
 L19997: {
  cont_117X = arg1K0;
  previous_118X = arg0K1;
  if ((cont_117X == (Sbottom_of_stackS))) {
    if ((3 == (3 & previous_118X))) {
      if ((10 == (31 & ((((*((long *) ((((char *) (-3 + previous_118X))) + -4))))>>2))))) {
        *((long *) ((((char *) (-3 + previous_118X))) + 4)) = end_116X;
        goto L20196;}
      else {
        goto L20196;}}
    else {
      goto L20196;}}
  else {
    p_119X = *((long *) (cont_117X + 12));
    if ((3 == (3 & p_119X))) {
      if ((p_119X < (((long) (Sstack_beginS))))) {
        goto L20017;}
      else {
        if (((((long) (Sstack_endS))) < p_119X)) {
          goto L20017;}
        else {
          save_env_in_heap((*((long *) (cont_117X + 12))), cont_117X, key_112X, reason_113X);
          goto L20017;}}}
    else {
      goto L20017;}}}
 L20196: {
  ScontS = (Sbottom_of_stackS);
  return (*((long *) ((Sbottom_of_stackS) + 4)));}
 L20017: {
  code_pointer_120X = ((char *) (*((long *) cont_117X)));
  size_121X = ((((*((unsigned char *) (code_pointer_120X + -2))))<<8)) + (*((unsigned char *) (code_pointer_120X + -1)));
  if ((0 == size_121X)) {
    arg0K0 = ((((*((long *) (cont_117X + 16))))>>2));
    goto L16886;}
  else {
    arg0K0 = size_121X;
    goto L16886;}}
 L16886: {
  size_122X = arg0K0;
  len_123X = ((size_122X)<<2);
  addr_124X = s48_allocate_small((4 + len_123X));
  *((long *) addr_124X) = (42 + (((len_123X)<<8)));
  new_125X = 3 + (((long) (addr_124X + 4)));
  memcpy((void *)((((char *) (-3 + new_125X))) + 4), (void *)(cont_117X + 4),(-4 + (((size_122X)<<2))));
  *((long *) (((char *) (-3 + new_125X)))) = (((((((char *) (*((long *) cont_117X)))) - (((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) (cont_117X + 8)))))))))))))<<2));
  if ((3 == (3 & previous_118X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + previous_118X))) + -4))))>>2))))) {
      *((long *) ((((char *) (-3 + previous_118X))) + 4)) = new_125X;
      goto L20035;}
    else {
      goto L20031;}}
  else {
    goto L20031;}}
 L20035: {
  arg1K0 = (((char *) (*((long *) ((((char *) (-3 + new_125X))) + 4)))));
  arg0K1 = new_125X;
  goto L19997;}
 L20031: {
  *((long *) ((Sbottom_of_stackS) + 4)) = new_125X;
  goto L20035;}
}
static long Hlookup2720(long table_126X, long string_127X, long key_128X)
{
  long arg0K1;
  long arg0K0;
  char * addr_143X;
  long value_142X;
  long link_141X;
  long x_140X;
  char * addr_139X;
  long next_138X;
  long len_137X;
  long s2_136X;
  long foo_135X;
  long bucket_134X;
  long link_133X;
  long index_132X;
  long h_131X;
  long i_130X;
  long n_129X;
 {  n_129X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_127X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L20550;}
 L20550: {
  i_130X = arg0K0;
  h_131X = arg0K1;
  if ((i_130X < n_129X)) {
    arg0K0 = (1 + i_130X);
    arg0K1 = (h_131X + (((*((unsigned char *) ((((char *) (-3 + string_127X))) + i_130X))))));
    goto L20550;}
  else {
    index_132X = 1023 & h_131X;
    link_133X = *((long *) ((((char *) (-3 + table_126X))) + (((index_132X)<<2))));
    if ((0 == (3 & link_133X))) {
      arg0K0 = (3 + (-4 & link_133X));
      goto L20506;}
    else {
      arg0K0 = link_133X;
      goto L20506;}}}
 L20506: {
  bucket_134X = arg0K0;
  arg0K0 = bucket_134X;
  goto L20512;}
 L20512: {
  foo_135X = arg0K0;
  if ((1 == foo_135X)) {
    if ((3 == (3 & bucket_134X))) {
      arg0K0 = (-4 & bucket_134X);
      goto L20517;}
    else {
      arg0K0 = bucket_134X;
      goto L20517;}}
  else {
    s2_136X = *((long *) (((char *) (-3 + foo_135X))));
    len_137X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_127X))) + -4))))>>8);
    if ((len_137X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_136X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_136X))), (void *)(((char *) (-3 + string_127X))),len_137X)))) {
        return foo_135X;}
      else {
        goto L20532;}}
    else {
      goto L20532;}}}
 L20517: {
  next_138X = arg0K0;
  addr_139X = s48_allocate_small(20);
  *((long *) addr_139X) = 4154;
  x_140X = 3 + (((long) (addr_139X + 4)));
  *((long *) (((char *) (-3 + x_140X)))) = string_127X;
  *((long *) ((((char *) (-3 + x_140X))) + 4)) = 1;
  *((long *) ((((char *) (-3 + x_140X))) + 8)) = 13;
  *((long *) ((((char *) (-3 + x_140X))) + 12)) = next_138X;
  if ((3 == (3 & x_140X))) {
    arg0K0 = (-4 & x_140X);
    goto L20523;}
  else {
    arg0K0 = x_140X;
    goto L20523;}}
 L20532: {
  link_141X = *((long *) ((((char *) (-3 + foo_135X))) + 12));
  if ((0 == (3 & link_141X))) {
    arg0K0 = (3 + (-4 & link_141X));
    goto L20512;}
  else {
    arg0K0 = link_141X;
    goto L20512;}}
 L20523: {
  value_142X = arg0K0;
  addr_143X = (((char *) (-3 + table_126X))) + (((index_132X)<<2));
  S48_WRITE_BARRIER(table_126X, addr_143X, value_142X);
  *((long *) addr_143X) = value_142X;
  return x_140X;}
}
static long Hlookup2701(long table_144X, long string_145X, long key_146X)
{
  long arg0K1;
  long arg0K0;
  char * addr_161X;
  long value_160X;
  long link_159X;
  long x_158X;
  char * addr_157X;
  long next_156X;
  long len_155X;
  long s2_154X;
  long foo_153X;
  long bucket_152X;
  long link_151X;
  long index_150X;
  long h_149X;
  long i_148X;
  long n_147X;
 {  n_147X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_145X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L20702;}
 L20702: {
  i_148X = arg0K0;
  h_149X = arg0K1;
  if ((i_148X < n_147X)) {
    arg0K0 = (1 + i_148X);
    arg0K1 = (h_149X + (((*((unsigned char *) ((((char *) (-3 + string_145X))) + i_148X))))));
    goto L20702;}
  else {
    index_150X = 1023 & h_149X;
    link_151X = *((long *) ((((char *) (-3 + table_144X))) + (((index_150X)<<2))));
    if ((0 == (3 & link_151X))) {
      arg0K0 = (3 + (-4 & link_151X));
      goto L20658;}
    else {
      arg0K0 = link_151X;
      goto L20658;}}}
 L20658: {
  bucket_152X = arg0K0;
  arg0K0 = bucket_152X;
  goto L20664;}
 L20664: {
  foo_153X = arg0K0;
  if ((1 == foo_153X)) {
    if ((3 == (3 & bucket_152X))) {
      arg0K0 = (-4 & bucket_152X);
      goto L20669;}
    else {
      arg0K0 = bucket_152X;
      goto L20669;}}
  else {
    s2_154X = *((long *) (((char *) (-3 + foo_153X))));
    len_155X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_145X))) + -4))))>>8);
    if ((len_155X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_154X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_154X))), (void *)(((char *) (-3 + string_145X))),len_155X)))) {
        return foo_153X;}
      else {
        goto L20684;}}
    else {
      goto L20684;}}}
 L20669: {
  next_156X = arg0K0;
  addr_157X = s48_allocate_small(20);
  *((long *) addr_157X) = 4154;
  x_158X = 3 + (((long) (addr_157X + 4)));
  *((long *) (((char *) (-3 + x_158X)))) = string_145X;
  *((long *) ((((char *) (-3 + x_158X))) + 4)) = 5;
  *((long *) ((((char *) (-3 + x_158X))) + 8)) = 13;
  *((long *) ((((char *) (-3 + x_158X))) + 12)) = next_156X;
  if ((3 == (3 & x_158X))) {
    arg0K0 = (-4 & x_158X);
    goto L20675;}
  else {
    arg0K0 = x_158X;
    goto L20675;}}
 L20684: {
  link_159X = *((long *) ((((char *) (-3 + foo_153X))) + 12));
  if ((0 == (3 & link_159X))) {
    arg0K0 = (3 + (-4 & link_159X));
    goto L20664;}
  else {
    arg0K0 = link_159X;
    goto L20664;}}
 L20675: {
  value_160X = arg0K0;
  addr_161X = (((char *) (-3 + table_144X))) + (((index_150X)<<2));
  S48_WRITE_BARRIER(table_144X, addr_161X, value_160X);
  *((long *) addr_161X) = value_160X;
  return x_158X;}
}
static void HtopD10292(void)
{
  long arg0K2;
  long arg0K1;
  long arg0K0;
  long new_199X;
  long status_198X;
  long id_197X;
  long v_196X;
  char v_195X;
  long channel_194X;
  long i_193X;
  char * addr_192X;
  long next_link_191X;
  long new_foo_190X;
  char v_189X;
  char * addr_188X;
  long foo_187X;
  char * addr_186X;
  long l_185X;
  long v_184X;
  long okay_link_183X;
  long foo_link_182X;
  char * addr_181X;
  char * addr_180X;
  char * addr_179X;
  char * addr_178X;
  long val_177X;
  char tracedP_176X;
  long next_175X;
  long thing_174X;
  long pair_173X;
  long alist_172X;
  long l2_171X;
  long goners_170X;
  long okay_169X;
  long alist_168X;
  long foo_link_167X;
  long v_166X;
  long pc_165X;
  long tem_164X;
  long i_163X;
  long table_162X;
 {  table_162X = s48_trace_value((Sthe_symbol_tableS));
  arg0K0 = 0;
  goto L21341;}
 L21341: {
  i_163X = arg0K0;
  if ((1024 == i_163X)) {
    Sthe_symbol_tableS = table_162X;
    tem_164X = StemplateS;
    pc_165X = Ssaved_pcS;
    StemplateS = tem_164X;
    Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_164X)))))))) + (((pc_165X)>>2)));
    v_166X = SHARED_REF((Sfinalizer_alistS));
    arg0K0 = v_166X;
    arg0K1 = 25;
    arg0K2 = 25;
    goto L10420;}
  else {
    foo_link_167X = *((long *) ((((char *) (-3 + table_162X))) + (((i_163X)<<2))));
    arg0K0 = foo_link_167X;
    arg0K1 = 1;
    goto L13870;}}
 L10420: {
  alist_168X = arg0K0;
  okay_169X = arg0K1;
  goners_170X = arg0K2;
  if ((25 == alist_168X)) {
    SHARED_SETB((Sfinalizer_alistS), okay_169X);
    l2_171X = Sfinalize_theseS;
    if ((25 == goners_170X)) {
      arg0K0 = l2_171X;
      goto L10427;}
    else {
      arg0K0 = goners_170X;
      goto L10482;}}
  else {
    alist_172X = s48_trace_value(alist_168X);
    pair_173X = s48_trace_value((*((long *) (((char *) (-3 + alist_172X))))));
    thing_174X = *((long *) (((char *) (-3 + pair_173X))));
    next_175X = *((long *) ((((char *) (-3 + alist_172X))) + 4));
    tracedP_176X = s48_extantP(thing_174X);
    val_177X = s48_trace_value(thing_174X);
    addr_178X = ((char *) (-3 + pair_173X));
    S48_WRITE_BARRIER(pair_173X, addr_178X, val_177X);
    *((long *) addr_178X) = val_177X;
    addr_179X = ((char *) (-3 + alist_172X));
    S48_WRITE_BARRIER(alist_172X, addr_179X, pair_173X);
    *((long *) addr_179X) = pair_173X;
    if (tracedP_176X) {
      addr_180X = (((char *) (-3 + alist_172X))) + 4;
      S48_WRITE_BARRIER(alist_172X, addr_180X, okay_169X);
      *((long *) addr_180X) = okay_169X;
      arg0K0 = next_175X;
      arg0K1 = alist_172X;
      arg0K2 = goners_170X;
      goto L10420;}
    else {
      addr_181X = (((char *) (-3 + alist_172X))) + 4;
      S48_WRITE_BARRIER(alist_172X, addr_181X, goners_170X);
      *((long *) addr_181X) = goners_170X;
      arg0K0 = next_175X;
      arg0K1 = okay_169X;
      arg0K2 = alist_172X;
      goto L10420;}}}
 L13870: {
  foo_link_182X = arg0K0;
  okay_link_183X = arg0K1;
  if ((0 == (3 & foo_link_182X))) {
    arg0K0 = (3 + (-4 & foo_link_182X));
    goto L13872;}
  else {
    arg0K0 = foo_link_182X;
    goto L13872;}}
 L10427: {
  v_184X = arg0K0;
  Sfinalize_theseS = v_184X;
  arg0K0 = 0;
  goto L19857;}
 L10482: {
  l_185X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_185X))) + 4))))) {
    addr_186X = (((char *) (-3 + l_185X))) + 4;
    S48_WRITE_BARRIER(l_185X, addr_186X, l2_171X);
    *((long *) addr_186X) = l2_171X;
    arg0K0 = goners_170X;
    goto L10427;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_185X))) + 4)));
    goto L10482;}}
 L13872: {
  foo_187X = arg0K0;
  if ((1 == foo_187X)) {
    addr_188X = (((char *) (-3 + table_162X))) + (((i_163X)<<2));
    S48_WRITE_BARRIER(table_162X, addr_188X, okay_link_183X);
    *((long *) addr_188X) = okay_link_183X;
    arg0K0 = (1 + i_163X);
    goto L21341;}
  else {
    v_189X = s48_extantP(foo_187X);
    if (v_189X) {
      new_foo_190X = s48_trace_value(foo_187X);
      next_link_191X = *((long *) ((((char *) (-3 + new_foo_190X))) + 4));
      addr_192X = (((char *) (-3 + new_foo_190X))) + 4;
      S48_WRITE_BARRIER(new_foo_190X, addr_192X, okay_link_183X);
      *((long *) addr_192X) = okay_link_183X;
      if ((3 == (3 & new_foo_190X))) {
        arg0K0 = next_link_191X;
        arg0K1 = (-4 & new_foo_190X);
        goto L13870;}
      else {
        arg0K0 = next_link_191X;
        arg0K1 = new_foo_190X;
        goto L13870;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + foo_187X))) + 4)));
      arg0K1 = okay_link_183X;
      goto L13870;}}}
 L19857: {
  i_193X = arg0K0;
  if ((i_193X == (Snumber_of_channelsS))) {
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
    channel_194X = *((Svm_channelsS) + i_193X);
    if ((1 == channel_194X)) {
      goto L19891;}
    else {
      v_195X = s48_extantP(channel_194X);
      if (v_195X) {
        v_196X = s48_trace_value(channel_194X);
        arg0K0 = v_196X;
        goto L19884;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_194X))))))) {
          arg0K0 = 1;
          goto L19884;}
        else {
          id_197X = *((long *) ((((char *) (-3 + channel_194X))) + 4));
          status_198X = close_channelB(channel_194X);
          if ((status_198X == NO_ERRORS)) {
            goto L17978;}
          else {
            channel_close_error(status_198X, (*((long *) ((((char *) (-3 + channel_194X))) + 8))), id_197X);
            goto L17978;}}}}}}
 L19891: {
  arg0K0 = (1 + i_193X);
  goto L19857;}
 L19884: {
  new_199X = arg0K0;
  *((Svm_channelsS) + i_193X) = new_199X;
  goto L19891;}
 L17978: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_197X))) {
    ps_write_integer((((id_197X)>>2)), (stderr));
    goto L17996;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_197X))))), (stderr));
    goto L17996;}}
 L17996: {
  ps_write_string(" ", (stderr));
  ps_write_integer(((((*((long *) ((((char *) (-3 + channel_194X))) + 8))))>>2)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L19884;}
}
static void HtopD10303(void)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long merged_arg0K0;

  int Hentry_tracer859200_return_tag;
  long Hentry_tracer8592000_return_value;
  long foo_link_201X;
  char * addr_281X;
  long next_link_280X;
  long new_foo_279X;
  long foo_278X;
  long done_link_277X;
  long foo_link_276X;
  long env_275X;
  long v_274X;
  long v_273X;
  long val_272X;
  long cells_271X;
  long size_270X;
  char * code_pointer_269X;
  long val_268X;
  long val_267X;
  long new_code_266X;
  long v_265X;
  long code_264X;
  long env_263X;
  long last_env_262X;
  char * cont_261X;
  long env_260X;
  long v_259X;
  long env_258X;
  long p_257X;
  long arg_count_256X;
  long i_255X;
  char * p_254X;
  long unused_253X;
  char * a_252X;
  char * addr_251X;
  long val_250X;
  char * addr_249X;
  long val_248X;
  long channel_247X;
  long i_246X;
  char x_245X;
  long pair_244X;
  long v_243X;
  long v_242X;
  long env_241X;
  long alist_240X;
  long v_239X;
  long v_238X;
  long v_237X;
  long v_236X;
  long v_235X;
  long v_234X;
  long v_233X;
  long v_232X;
  long v_231X;
  long v_230X;
  long v_229X;
  long v_228X;
  long v_227X;
  long v_226X;
  long v_225X;
  long v_224X;
  long v_223X;
  long i_222X;
  char * addr_221X;
  long value_220X;
  long count_219X;
  long *sizes_218X;
  char * *areas_217X;
  long i_216X;
  char * addr_215X;
  long value_214X;
  long table_213X;
  long i_212X;
  long x2_211X;
  char * cell_210X;
  long i_209X;
  long x2_208X;
  char * cell_207X;
  long table_206X;
  char * frame_205X;
  long length_204X;
  char * frame_203X;
  long v_202X;
 {  v_202X = s48_trace_value((Sempty_logS));
  Sempty_logS = v_202X;
  arg1K0 = (Sexternal_root_stackS);
  goto L7459;}
 L7459: {
  frame_203X = arg1K0;
  if ((frame_203X == NULL)) {
    arg1K0 = (Spermanent_external_rootsS);
    goto L7485;}
  else {
    length_204X = *((long *) frame_203X);
    arg0K0 = 0;
    goto L7467;}}
 L7485: {
  frame_205X = arg1K0;
  if ((frame_205X == NULL)) {
    s48_initializing_gc_root();
    table_206X = s48_trace_value((Simported_bindingsS));
    arg0K0 = 0;
    goto L18261;}
  else {
    cell_207X = ((char *) (*((long *) (frame_205X + 4))));
    x2_208X = s48_trace_value((*((long *) cell_207X)));
    *((long *) cell_207X) = x2_208X;
    arg1K0 = (((char *) (*((long *) frame_205X))));
    goto L7485;}}
 L7467: {
  i_209X = arg0K0;
  if ((i_209X == length_204X)) {
    arg1K0 = (((char *) (*((long *) (frame_203X + 4)))));
    goto L7459;}
  else {
    cell_210X = ((char *) (*((long *) (frame_203X + (8 + (((i_209X)<<2)))))));
    x2_211X = s48_trace_value((*((long *) cell_210X)));
    *((long *) cell_210X) = x2_211X;
    arg0K0 = (1 + i_209X);
    goto L7467;}}
 L18261: {
  i_212X = arg0K0;
  if ((1024 == i_212X)) {
    Simported_bindingsS = table_206X;
    table_213X = s48_trace_value((Sexported_bindingsS));
    arg0K0 = 0;
    goto L18282;}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_206X))) + (((i_212X)<<2)))));
    Hentry_tracer859200_return_tag = 0;
    goto Hentry_tracer859200;
   Hentry_tracer859200_return_0:
    value_214X = Hentry_tracer8592000_return_value;
    addr_215X = (((char *) (-3 + table_206X))) + (((i_212X)<<2));
    S48_WRITE_BARRIER(table_206X, addr_215X, value_214X);
    *((long *) addr_215X) = value_214X;
    arg0K0 = (1 + i_212X);
    goto L18261;}}
 L18282: {
  i_216X = arg0K0;
  if ((1024 == i_216X)) {
    Sexported_bindingsS = table_213X;
    if ((0 < (Simpure_area_countS))) {
      areas_217X = Simpure_areasS;
      sizes_218X = Simpure_sizesS;
      count_219X = Simpure_area_countS;
      arg0K0 = 0;
      goto L21001;}
    else {
      goto L20980;}}
  else {
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_213X))) + (((i_216X)<<2)))));
    Hentry_tracer859200_return_tag = 1;
    goto Hentry_tracer859200;
   Hentry_tracer859200_return_1:
    value_220X = Hentry_tracer8592000_return_value;
    addr_221X = (((char *) (-3 + table_213X))) + (((i_216X)<<2));
    S48_WRITE_BARRIER(table_213X, addr_221X, value_220X);
    *((long *) addr_221X) = value_220X;
    arg0K0 = (1 + i_216X);
    goto L18282;}}
 L21001: {
  i_222X = arg0K0;
  if ((i_222X < count_219X)) {
    s48_trace_locationsB((*(areas_217X + i_222X)), ((*(areas_217X + i_222X)) + (*(sizes_218X + i_222X))));
    arg0K0 = (1 + i_222X);
    goto L21001;}
  else {
    goto L20980;}}
 L20980: {
  Ssaved_pcS = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  v_223X = s48_trace_value((StemplateS));
  StemplateS = v_223X;
  v_224X = s48_trace_value((SvalS));
  SvalS = v_224X;
  v_225X = s48_trace_value((Scurrent_threadS));
  Scurrent_threadS = v_225X;
  v_226X = s48_trace_value((Sinterrupt_templateS));
  Sinterrupt_templateS = v_226X;
  v_227X = s48_trace_value((Sexception_templateS));
  Sexception_templateS = v_227X;
  v_228X = s48_trace_value((Sinterrupted_templateS));
  Sinterrupted_templateS = v_228X;
  v_229X = s48_trace_value((Sos_signal_typeS));
  Sos_signal_typeS = v_229X;
  v_230X = s48_trace_value((Sos_signal_argumentS));
  Sos_signal_argumentS = v_230X;
  v_231X = SHARED_REF((Ssession_dataS));
  v_232X = s48_trace_value(v_231X);
  SHARED_SETB((Ssession_dataS), v_232X);
  v_233X = SHARED_REF((Sexception_handlersS));
  v_234X = s48_trace_value(v_233X);
  SHARED_SETB((Sexception_handlersS), v_234X);
  v_235X = SHARED_REF((Sinterrupt_handlersS));
  v_236X = s48_trace_value(v_235X);
  SHARED_SETB((Sinterrupt_handlersS), v_236X);
  v_237X = SHARED_REF((Sfinalize_theseS));
  v_238X = s48_trace_value(v_237X);
  SHARED_SETB((Sfinalize_theseS), v_238X);
  v_239X = SHARED_REF((Sfinalizer_alistS));
  arg0K0 = v_239X;
  goto L7525;}
 L7525: {
  alist_240X = arg0K0;
  if ((25 == alist_240X)) {
    env_241X = s48_trace_value((SenvS));
    SenvS = env_241X;
    v_242X = s48_trace_value((Spending_channels_headS));
    Spending_channels_headS = v_242X;
    v_243X = s48_trace_value((Spending_channels_tailS));
    Spending_channels_tailS = v_243X;
    arg0K0 = 0;
    goto L10552;}
  else {
    pair_244X = *((long *) (((char *) (-3 + alist_240X))));
    x_245X = s48_extantP((*((long *) (((char *) (-3 + pair_244X))))));
    if (x_245X) {
      goto L7550;}
    else {
      s48_trace_stob_contentsB((*((long *) (((char *) (-3 + pair_244X))))));
      goto L7550;}}}
 L10552: {
  i_246X = arg0K0;
  if ((i_246X == (Snumber_of_channelsS))) {
    if ((Sstack_warningPS)) {
      arg1K0 = (Sstack_beginS);
      goto L14009;}
    else {
      goto L14042;}}
  else {
    channel_247X = *((Svm_channelsS) + i_246X);
    if ((1 == channel_247X)) {
      goto L10580;}
    else {
      if ((0 == (*((long *) (((char *) (-3 + channel_247X))))))) {
        goto L10580;}
      else {
        val_248X = s48_trace_value((*((long *) ((((char *) (-3 + channel_247X))) + 4))));
        addr_249X = (((char *) (-3 + channel_247X))) + 4;
        S48_WRITE_BARRIER(channel_247X, addr_249X, val_248X);
        *((long *) addr_249X) = val_248X;
        goto L10580;}}}}
 L7550: {
  val_250X = s48_trace_value((*((long *) ((((char *) (-3 + pair_244X))) + 4))));
  addr_251X = (((char *) (-3 + pair_244X))) + 4;
  S48_WRITE_BARRIER(pair_244X, addr_251X, val_250X);
  *((long *) addr_251X) = val_250X;
  arg0K0 = (*((long *) ((((char *) (-3 + alist_240X))) + 4)));
  goto L7525;}
 L14009: {
  a_252X = arg1K0;
  if ((252645135 == (*((long *) a_252X)))) {
    arg1K0 = (a_252X + 4);
    goto L14009;}
  else {
    unused_253X = (((a_252X - (Sstack_beginS)))>>2);
    if ((unused_253X < 30)) {
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      ps_write_string("[Alert: stack overconsumption (", (stderr));
      ps_write_integer(unused_253X, (stderr));
      ps_write_string("); please inform the Scheme 48 implementors]", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sstack_warningPS = 0;
      goto L14042;}
    else {
      goto L14042;}}}
 L14042: {
  arg1K0 = (SstackS);
  arg0K1 = 0;
  goto L14109;}
 L10580: {
  arg0K0 = (1 + i_246X);
  goto L10552;}
 L14109: {
  p_254X = arg1K0;
  i_255X = arg0K1;
  if ((p_254X == (ScontS))) {
    arg0K0 = i_255X;
    goto L14044;}
  else {
    if ((26 == (*((long *) p_254X)))) {
      arg0K0 = (-1 + i_255X);
      goto L14044;}
    else {
      if ((2 == (3 & (*((long *) p_254X))))) {
        arg0K0 = i_255X;
        goto L14044;}
      else {
        arg1K0 = (p_254X + 4);
        arg0K1 = (1 + i_255X);
        goto L14109;}}}}
 L14044: {
  arg_count_256X = arg0K0;
  s48_trace_locationsB((SstackS), ((SstackS) + (((arg_count_256X)<<2))));
  p_257X = SenvS;
  if ((3 == (3 & p_257X))) {
    if ((p_257X < (((long) (Sstack_beginS))))) {
      goto L14058;}
    else {
      if (((((long) (Sstack_endS))) < p_257X)) {
        goto L14058;}
      else {
        env_258X = SenvS;
        arg0K0 = env_258X;
        goto L14226;}}}
  else {
    goto L14058;}}
 L14058: {
  v_259X = s48_trace_value((SenvS));
  SenvS = v_259X;
  goto L14063;}
 L14226: {
  env_260X = arg0K0;
  if ((3 == (3 & env_260X))) {
    if ((env_260X < (((long) (Sstack_beginS))))) {
      goto L14063;}
    else {
      if (((((long) (Sstack_endS))) < env_260X)) {
        goto L14063;}
      else {
        s48_trace_stob_contentsB(env_260X);
        arg0K0 = (*((long *) (((char *) (-3 + env_260X)))));
        goto L14226;}}}
  else {
    goto L14063;}}
 L14063: {
  arg1K0 = (ScontS);
  arg0K1 = 0;
  goto L14068;}
 L14068: {
  cont_261X = arg1K0;
  last_env_262X = arg0K1;
  env_263X = *((long *) (cont_261X + 12));
  code_264X = *((long *) (((char *) (-3 + (*((long *) (cont_261X + 8)))))));
  v_265X = *((long *) cont_261X);
  new_code_266X = s48_trace_value(code_264X);
  *((long *) cont_261X) = (((long) ((((char *) (-3 + new_code_266X))) + (v_265X - (((long) (((char *) (-3 + code_264X)))))))));
  val_267X = s48_trace_value((*((long *) (cont_261X + 8))));
  *((long *) (cont_261X + 8)) = val_267X;
  val_268X = s48_trace_value((*((long *) (cont_261X + 12))));
  *((long *) (cont_261X + 12)) = val_268X;
  code_pointer_269X = ((char *) (*((long *) cont_261X)));
  size_270X = ((((*((unsigned char *) (code_pointer_269X + -2))))<<8)) + (*((unsigned char *) (code_pointer_269X + -1)));
  if ((0 == size_270X)) {
    arg0K0 = ((((*((long *) (cont_261X + 16))))>>2));
    goto L11661;}
  else {
    arg0K0 = size_270X;
    goto L11661;}}
 L11661: {
  cells_271X = arg0K0;
  s48_trace_locationsB((cont_261X + 16), (cont_261X + (((cells_271X)<<2))));
  if ((env_263X == last_env_262X)) {
    goto L14086;}
  else {
    arg0K0 = env_263X;
    goto L14181;}}
 L14086: {
  if ((cont_261X == (Sbottom_of_stackS))) {
    val_272X = s48_trace_value((*((long *) ((Sbottom_of_stackS) + 4))));
    *((long *) ((Sbottom_of_stackS) + 4)) = val_272X;
    v_273X = s48_trace_value((Stemp0S));
    Stemp0S = v_273X;
    v_274X = s48_trace_value((Stemp1S));
    Stemp1S = v_274X;
    return;}
  else {
    arg1K0 = (((char *) (*((long *) (cont_261X + 4)))));
    arg0K1 = env_263X;
    goto L14068;}}
 L14181: {
  env_275X = arg0K0;
  if ((3 == (3 & env_275X))) {
    if ((env_275X < (((long) (Sstack_beginS))))) {
      goto L14086;}
    else {
      if (((((long) (Sstack_endS))) < env_275X)) {
        goto L14086;}
      else {
        s48_trace_stob_contentsB(env_275X);
        arg0K0 = (*((long *) (((char *) (-3 + env_275X)))));
        goto L14181;}}}
  else {
    goto L14086;}}
 Hentry_tracer859200: {
  foo_link_201X = merged_arg0K0;{
  arg0K0 = foo_link_201X;
  arg0K1 = 1;
  goto L13942;}
 L13942: {
  foo_link_276X = arg0K0;
  done_link_277X = arg0K1;
  if ((0 == (3 & foo_link_276X))) {
    arg0K0 = (3 + (-4 & foo_link_276X));
    goto L13944;}
  else {
    arg0K0 = foo_link_276X;
    goto L13944;}}
 L13944: {
  foo_278X = arg0K0;
  if ((1 == foo_278X)) {
    Hentry_tracer8592000_return_value = done_link_277X;
    goto Hentry_tracer859200_return;}
  else {
    new_foo_279X = s48_trace_value(foo_278X);
    next_link_280X = *((long *) ((((char *) (-3 + new_foo_279X))) + 12));
    addr_281X = (((char *) (-3 + new_foo_279X))) + 12;
    S48_WRITE_BARRIER(new_foo_279X, addr_281X, done_link_277X);
    *((long *) addr_281X) = done_link_277X;
    if ((3 == (3 & new_foo_279X))) {
      arg0K0 = next_link_280X;
      arg0K1 = (-4 & new_foo_279X);
      goto L13942;}
    else {
      arg0K0 = next_link_280X;
      arg0K1 = new_foo_279X;
      goto L13942;}}}
 Hentry_tracer859200_return:
  switch (Hentry_tracer859200_return_tag) {
  case 0: goto Hentry_tracer859200_return_0;
  default: goto Hentry_tracer859200_return_1;
  }}

}
void s48_set_native_protocolB(long protocol_282X)
{

 {  s48_Snative_protocolS = protocol_282X;
  return;}
}
void s48_set_extension_valueB(long value_283X)
{

 {  s48_Sextension_valueS = value_283X;
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
void s48_set_os_signal(long type_284X, long argument_285X)
{

 {  Sos_signal_typeS = type_284X;
  Sos_signal_argumentS = argument_285X;
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
  char * old_base_286X;
 {  old_base_286X = Sexternal_root_stack_baseS;
  Sexternal_root_stack_baseS = (Sexternal_root_stackS);
  return old_base_286X;}
}
char s48_release_gc_roots_baseB(char * old_base_287X)
{
  char okayP_288X;
 {  okayP_288X = (Sexternal_root_stackS) == (Sexternal_root_stack_baseS);
  Sexternal_root_stackS = old_base_287X;
  Sexternal_root_stack_baseS = old_base_287X;
  return okayP_288X;}
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
void s48_push_gc_rootsB(char * frame_289X, long n_290X)
{

 {  *((long *) frame_289X) = n_290X;
  *((long *) (frame_289X + 4)) = (((long) (Sexternal_root_stackS)));
  Sexternal_root_stackS = frame_289X;
  return;}
}
void s48_register_gc_rootB(char * loc_addr_291X)
{
  char * frame_292X;
 {  frame_292X = (char *)malloc(8);
  if ((frame_292X == NULL)) {
    ps_error("out of memory registering a global root", 0);
    goto L3950;}
  else {
    goto L3950;}}
 L3950: {
  *((long *) frame_292X) = (((long) (Spermanent_external_rootsS)));
  *((long *) (frame_292X + 4)) = (((long) loc_addr_291X));
  Spermanent_external_rootsS = frame_292X;
  return;}
}
void s48_stack_setB(long x_293X, long value_294X)
{

 {  *((long *) ((SstackS) + (((x_293X)<<2)))) = value_294X;
  return;}
}
long s48_stack_ref(long i_295X)
{

 {  return (*((long *) ((SstackS) + (((i_295X)<<2)))));}
}
void s48_push(long x_296X)
{

 {  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_296X;
  return;}
}
long s48_resetup_external_exception(long new_why_297X, long additional_nargs_298X)
{
  long old_why_300X;
  long old_nargs_299X;
 {  old_nargs_299X = Sexternal_exception_nargsS;
  old_why_300X = *((long *) ((SstackS) + (((old_nargs_299X)<<2))));
  *((long *) ((SstackS) + (((old_nargs_299X)<<2)))) = (((new_why_297X)<<2));
  Sexternal_exception_nargsS = (old_nargs_299X + additional_nargs_298X);
  return old_why_300X;}
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
char * s48_shorten_bignum(char * external_bignum_301X, long number_of_digits_302X)
{
  long waste_size_307X;
  long old_data_size_306X;
  long new_data_size_305X;
  long new_size_304X;
  long bignum_303X;
 {  bignum_303X = 3 + (((long) external_bignum_301X));
  new_size_304X = 8 + (((number_of_digits_302X)<<2));
  new_data_size_305X = -4 + new_size_304X;
  old_data_size_306X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + bignum_303X))) + -4))))>>8);
  waste_size_307X = old_data_size_306X - new_data_size_305X;
  if ((waste_size_307X < 0)) {
    ps_error("shorten bignum", 2, new_data_size_305X, old_data_size_306X);
    goto L7036;}
  else {
    goto L7036;}}
 L7036: {
  if ((waste_size_307X < 4)) {
    return external_bignum_301X;}
  else {
    *((long *) ((((char *) (-3 + bignum_303X))) + -4)) = (82 + (((new_data_size_305X)<<8)));
    *((long *) ((((char *) (((long) ((((char *) (-3 + bignum_303X))) + (-4 & (3 + new_size_304X))))))) + -4)) = (-942 + (((waste_size_307X)<<8)));
    return external_bignum_301X;}}
}
long s48_allocate_bignum(long size_308X)
{
  char * addr_309X;
 {  addr_309X = s48_allocate_small((4 + size_308X));
  *((long *) addr_309X) = (82 + (((size_308X)<<8)));
  return (3 + (((long) (addr_309X + 4))));}
}
long s48_set_channel_os_index(long channel_310X, long os_index_311X)
{
  char * addr_318X;
  long val_317X;
  long v_316X;
  long x_315X;
  long old_index_314X;
  char x_313X;
  char temp_312X;
 {  temp_312X = os_index_311X < (Snumber_of_channelsS);
  if (temp_312X) {
    goto L10711;}
  else {
    x_313X = add_more_channels(os_index_311X);
    if (x_313X) {
      goto L10711;}
    else {
      return 36;}}}
 L10711: {
  if ((1 == (*((Svm_channelsS) + os_index_311X)))) {
    old_index_314X = (((*((long *) ((((char *) (-3 + channel_310X))) + 8))))>>2);
    x_315X = *((long *) ((((char *) (-3 + channel_310X))) + 16));
    if ((5 == x_315X)) {
      v_316X = ps_abort_fd_op(old_index_314X);
      enqueue_channelB(old_index_314X, v_316X);
      goto L10697;}
    else {
      goto L10697;}}
  else {
    return 44;}}
 L10697: {
  *((Svm_channelsS) + old_index_314X) = 1;
  *((Svm_channelsS) + os_index_311X) = channel_310X;
  val_317X = ((os_index_311X)<<2);
  addr_318X = (((char *) (-3 + channel_310X))) + 8;
  S48_WRITE_BARRIER(channel_310X, addr_318X, val_317X);
  *((long *) addr_318X) = val_317X;
  return 5;}
}
void s48_setup_external_exception(long why_319X, long nargs_320X)
{

 {  push_exception_continuationB(why_319X, 1);
  if ((10 < nargs_320X)) {
    ps_error("too many arguments from external exception", 0);
    goto L14769;}
  else {
    goto L14769;}}
 L14769: {
  Sexternal_exception_nargsS = nargs_320X;
  Sexternal_exceptionPS = 1;
  return;}
}
void s48_close_channel(long os_index_321X)
{
  long obj_322X;
 {  if ((os_index_321X < 0)) {
    return;}
  else {
    if ((os_index_321X < (Snumber_of_channelsS))) {
      obj_322X = *((Svm_channelsS) + os_index_321X);
      if ((3 == (3 & obj_322X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_322X))) + -4))))>>2))))) {
          close_channelB((*((Svm_channelsS) + os_index_321X)));
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
  long link_330X;
  long name_329X;
  long entry_328X;
  long link_327X;
  char temp_326X;
  long i_325X;
  long table_324X;
  FILE * out_323X;
 {  out_323X = stderr;
  table_324X = Simported_bindingsS;
  arg0K0 = 0;
  goto L16090;}
 L16090: {
  i_325X = arg0K0;
  temp_326X = 1024 == i_325X;
  if (temp_326X) {
    return temp_326X;}
  else {
    link_327X = *((long *) ((((char *) (-3 + table_324X))) + (((i_325X)<<2))));
    if ((0 == (3 & link_327X))) {
      arg0K0 = (3 + (-4 & link_327X));
      goto L16064;}
    else {
      arg0K0 = link_327X;
      goto L16064;}}}
 L16064: {
  entry_328X = arg0K0;
  if ((1 == entry_328X)) {
    arg0K0 = (1 + i_325X);
    goto L16090;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + entry_328X))) + 8)))))) {
      name_329X = *((long *) (((char *) (-3 + entry_328X))));
      ps_write_string("undefined imported name ", out_323X);
      if ((3 == (3 & name_329X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_329X))) + -4))))>>2))))) {
          ps_write_string((((char *)(((char *) (-3 + name_329X))))), out_323X);
          goto L18172;}
        else {
          goto L18170;}}
      else {
        goto L18170;}}
    else {
      goto L16071;}}}
 L18172: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_323X, ignoreXX) }
  goto L16071;}
 L18170: {
  ps_write_string("<invalid name>", out_323X);
  goto L18172;}
 L16071: {
  link_330X = *((long *) ((((char *) (-3 + entry_328X))) + 12));
  if ((0 == (3 & link_330X))) {
    arg0K0 = (3 + (-4 & link_330X));
    goto L16064;}
  else {
    arg0K0 = link_330X;
    goto L16064;}}
}
long s48_really_add_channel(long mode_331X, long id_332X, long os_index_333X)
{
  long status_335X;
  long channel_334X;
 {  s48_make_availableAgc(24);
  channel_334X = make_registered_channel((((mode_331X)>>2)), id_332X, os_index_333X, 0, &status_335X);
  if ((3 == (3 & channel_334X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_334X))) + -4))))>>2))))) {
      return channel_334X;}
    else {
      goto L18328;}}
  else {
    goto L18328;}}
 L18328: {
  return (((status_335X)<<2));}
}
long s48_enter_integer(long x_336X)
{
  char * v_337X;
 {  s48_make_availableAgc(16);
  if ((536870911 < x_336X)) {
    goto L19840;}
  else {
    if ((x_336X < -536870912)) {
      goto L19840;}
    else {
      return (((x_336X)<<2));}}}
 L19840: {
  v_337X = (char *) s48_long_to_bignum(x_336X);
  return enter_bignum(v_337X);}
}
void s48_copy_stack_into_heap(void)
{
  long arg0K1;
  long arg0K0;
  char * arg1K1;
  char * arg1K0;
  char * arg_343X;
  char * loc_342X;
  char * top_341X;
  long arg_count_340X;
  long i_339X;
  char * p_338X;
 {  s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
  arg1K0 = (SstackS);
  arg0K1 = 0;
  goto L21176;}
 L21176: {
  p_338X = arg1K0;
  i_339X = arg0K1;
  if ((p_338X == (ScontS))) {
    arg0K0 = i_339X;
    goto L21152;}
  else {
    if ((26 == (*((long *) p_338X)))) {
      arg0K0 = (-1 + i_339X);
      goto L21152;}
    else {
      if ((2 == (3 & (*((long *) p_338X))))) {
        arg0K0 = i_339X;
        goto L21152;}
      else {
        arg1K0 = (p_338X + 4);
        arg0K1 = (1 + i_339X);
        goto L21176;}}}}
 L21152: {
  arg_count_340X = arg0K0;
  top_341X = SstackS;
  if ((1 == (((long) (ScontS))))) {
    goto L21158;}
  else {
    really_preserve_continuation(0, 1);
    goto L21158;}}
 L21158: {
  SstackS = (ScontS);
  arg1K0 = ((SstackS) + -4);
  arg1K1 = (top_341X + (-4 + (((arg_count_340X)<<2))));
  goto L21201;}
 L21201: {
  loc_342X = arg1K0;
  arg_343X = arg1K1;
  if ((arg_343X < top_341X)) {
    SstackS = ((SstackS) + (0 - (((arg_count_340X)<<2))));
    return;}
  else {
    *((long *) loc_342X) = (*((long *) arg_343X));
    arg1K0 = (loc_342X + -4);
    arg1K1 = (arg_343X + -4);
    goto L21201;}}
}
long s48_get_imported_binding(char *name_344X)
{
  long value_346X;
  long value_345X;
 {  value_345X = enter_stringAgc(name_344X);
  Stemp0S = value_345X;
  s48_make_availableAgc(20);
  value_346X = Stemp0S;
  Stemp0S = 1;
  return Hlookup2720((Sexported_bindingsS), value_346X, 0);}
}
void s48_define_exported_binding(char *name_347X, long value_348X)
{
  char * addr_353X;
  long x_352X;
  long value_351X;
  long value_350X;
  long name_349X;
 {  Stemp0S = value_348X;
  name_349X = enter_stringAgc(name_347X);
  Stemp1S = name_349X;
  s48_make_availableAgc(20);
  value_350X = Stemp0S;
  Stemp0S = 1;
  value_351X = Stemp1S;
  Stemp1S = 1;
  x_352X = Hlookup2701((Simported_bindingsS), value_351X, 0);
  addr_353X = (((char *) (-3 + x_352X))) + 8;
  S48_WRITE_BARRIER(x_352X, addr_353X, value_350X);
  *((long *) addr_353X) = value_350X;
  return;}
}
void s48_initialize_vm(char * stack_begin_354X, long stack_size_355X)
{
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  long merged_arg0K6;
  long merged_arg0K5;
  long merged_arg0K4;
  long merged_arg0K3;
  long merged_arg0K2;
  long merged_arg0K1;
  long merged_arg0K0;

  int make_hash_tableAgc_return_tag;
  long make_hash_tableAgc0_return_value;
  int make_template_containing_six_ops_return_tag;
  long make_template_containing_six_ops0_return_value;
  long op0_356X;
  long op1_357X;
  long op2_358X;
  long op3_359X;
  long op4_360X;
  long op5_361X;
  char * addr_409X;
  long i_408X;
  long table_407X;
  long v_406X;
  long vector_405X;
  char * addr_404X;
  char * addr_403X;
  long code_402X;
  long temp_401X;
  char * addr_400X;
  char * addr_399X;
  char * addr_398X;
  long x_397X;
  long v_396X;
  long v_395X;
  long vector_394X;
  char * addr_393X;
  long v_392X;
  long v_391X;
  char * cont_390X;
  char * code_pointer_389X;
  long template_388X;
  char * a_387X;
  char * addr_386X;
  long value_385X;
  long size_384X;
  char * start_383X;
  char * addr_382X;
  long val_381X;
  long index_380X;
  long h_379X;
  long i_378X;
  char * stack_377X;
  long n_376X;
  long string_375X;
  long foo_374X;
  long table_373X;
  long i_372X;
  long v_371X;
  long v_370X;
  long imported_bindings_369X;
  long exported_bindings_368X;
  long n_367X;
  long symbols_366X;
  long maybe_365X;
  long maybe_364X;
  long v_363X;
  long symbol_table_362X;
 {  symbol_table_362X = s48_initial_symbols();
  if ((symbol_table_362X == 1)) {
    make_hash_tableAgc_return_tag = 0;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_0:
    v_363X = make_hash_tableAgc0_return_value;
    Sthe_symbol_tableS = v_363X;
    maybe_364X = s48_find_all(1);
    if ((maybe_364X == 1)) {
      s48_collect();
      maybe_365X = s48_find_all(1);
      if ((maybe_365X == 1)) {
        ps_error("insufficient heap space to build symbol table", 0);
        arg0K0 = maybe_365X;
        goto L21050;}
      else {
        arg0K0 = maybe_365X;
        goto L21050;}}
    else {
      arg0K0 = maybe_364X;
      goto L21050;}}
  else {
    Sthe_symbol_tableS = symbol_table_362X;
    goto L21479;}}
 L21050: {
  symbols_366X = arg0K0;
  n_367X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_366X))) + -4))))>>8))))>>2);
  arg0K0 = 0;
  goto L21071;}
 L21479: {
  exported_bindings_368X = s48_initial_exported_bindings();
  imported_bindings_369X = s48_initial_imported_bindings();
  if ((1 == imported_bindings_369X)) {
    make_hash_tableAgc_return_tag = 1;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_1:
    v_370X = make_hash_tableAgc0_return_value;
    Simported_bindingsS = v_370X;
    make_hash_tableAgc_return_tag = 2;
    goto make_hash_tableAgc;
   make_hash_tableAgc_return_2:
    v_371X = make_hash_tableAgc0_return_value;
    Sexported_bindingsS = v_371X;
    goto L21485;}
  else {
    Simported_bindingsS = imported_bindings_369X;
    Sexported_bindingsS = exported_bindings_368X;
    goto L21485;}}
 L21071: {
  i_372X = arg0K0;
  if ((i_372X == n_367X)) {
    goto L21479;}
  else {
    table_373X = Sthe_symbol_tableS;
    foo_374X = *((long *) ((((char *) (-3 + symbols_366X))) + (((i_372X)<<2))));
    string_375X = *((long *) (((char *) (-3 + foo_374X))));
    n_376X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_375X))) + -4))))>>8));
    arg0K0 = 0;
    arg0K1 = 0;
    goto L20461;}}
 L21485: {
  if ((stack_size_355X < 8131)) {
    stack_377X = (char *)malloc(32524);
    if ((stack_377X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_377X;
      arg0K1 = 8131;
      goto L18392;}
    else {
      arg1K0 = stack_377X;
      arg0K1 = 8131;
      goto L18392;}}
  else {
    arg1K0 = stack_begin_354X;
    arg0K1 = stack_size_355X;
    goto L18392;}}
 L20461: {
  i_378X = arg0K0;
  h_379X = arg0K1;
  if ((i_378X < n_376X)) {
    arg0K0 = (1 + i_378X);
    arg0K1 = (h_379X + (((*((unsigned char *) ((((char *) (-3 + string_375X))) + i_378X))))));
    goto L20461;}
  else {
    index_380X = 1023 & h_379X;
    val_381X = *((long *) ((((char *) (-3 + table_373X))) + (((index_380X)<<2))));
    addr_382X = (((char *) (-3 + foo_374X))) + 4;
    S48_WRITE_BARRIER(foo_374X, addr_382X, val_381X);
    *((long *) addr_382X) = val_381X;
    if ((3 == (3 & foo_374X))) {
      arg0K0 = (-4 & foo_374X);
      goto L20447;}
    else {
      arg0K0 = foo_374X;
      goto L20447;}}}
 L18392: {
  start_383X = arg1K0;
  size_384X = arg0K1;
  Sstack_beginS = start_383X;
  Sstack_endS = (start_383X + (((size_384X)<<2)));
  Sstack_limitS = ((Sstack_beginS) + 524);
  SstackS = (Sstack_endS);
  ScontS = (((char *) 1));
  SenvS = 17;
  arg1K0 = start_383X;
  goto L18422;}
 L20447: {
  value_385X = arg0K0;
  addr_386X = (((char *) (-3 + table_373X))) + (((index_380X)<<2));
  S48_WRITE_BARRIER(table_373X, addr_386X, value_385X);
  *((long *) addr_386X) = value_385X;
  arg0K0 = (1 + i_372X);
  goto L21071;}
 L18422: {
  a_387X = arg1K0;
  if ((a_387X == (Sstack_endS))) {
    s48_make_availableAgc(24);
    merged_arg0K0 = 24;
    merged_arg0K1 = 0;
    merged_arg0K2 = 4;
    merged_arg0K3 = 0;
    merged_arg0K4 = 71;
    merged_arg0K5 = 0;
    merged_arg0K6 = 0;
    make_template_containing_six_ops_return_tag = 0;
    goto make_template_containing_six_ops;
   make_template_containing_six_ops_return_0:
    template_388X = make_template_containing_six_ops0_return_value;
    code_pointer_389X = (((char *) (-3 + (*((long *) (((char *) (-3 + template_388X)))))))) + 3;
    SstackS = ((SstackS) + -16);
    cont_390X = SstackS;
    *((long *) cont_390X) = (((long) code_pointer_389X));
    *((long *) (cont_390X + 8)) = template_388X;
    *((long *) (cont_390X + 12)) = (SenvS);
    *((long *) (cont_390X + 4)) = (((long) (ScontS)));
    ScontS = cont_390X;
    Sbottom_of_stackS = (ScontS);
    s48_make_availableAgc(48);
    merged_arg0K0 = 24;
    merged_arg0K1 = 0;
    merged_arg0K2 = 0;
    merged_arg0K3 = 0;
    merged_arg0K4 = 66;
    merged_arg0K5 = 148;
    merged_arg0K6 = 0;
    make_template_containing_six_ops_return_tag = 1;
    goto make_template_containing_six_ops;
   make_template_containing_six_ops_return_1:
    v_391X = make_template_containing_six_ops0_return_value;
    Sinterrupt_templateS = v_391X;
    merged_arg0K0 = 24;
    merged_arg0K1 = 0;
    merged_arg0K2 = 0;
    merged_arg0K3 = 0;
    merged_arg0K4 = 1;
    merged_arg0K5 = 145;
    merged_arg0K6 = 0;
    make_template_containing_six_ops_return_tag = 2;
    goto make_template_containing_six_ops;
   make_template_containing_six_ops_return_2:
    v_392X = make_template_containing_six_ops0_return_value;
    Sexception_templateS = v_392X;
    s48_make_availableAgc(32);
    s48_bignum_make_cached_constants();
    addr_393X = s48_allocate_tracedAgc(8);
    if ((addr_393X == NULL)) {
      arg0K0 = 1;
      goto L21546;}
    else {
      *((long *) addr_393X) = 1034;
      arg0K0 = (3 + (((long) (addr_393X + 4))));
      goto L21546;}}
  else {
    *((long *) a_387X) = 252645135;
    arg1K0 = (a_387X + 4);
    goto L18422;}}
 L21546: {
  vector_394X = arg0K0;
  if ((1 == vector_394X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_395X;
    goto L21533;}
  else {
    arg0K0 = vector_394X;
    goto L21533;}}
 L21533: {
  v_396X = arg0K0;
  Sempty_logS = v_396X;
  x_397X = Sempty_logS;
  addr_398X = ((char *) (-3 + x_397X));
  S48_WRITE_BARRIER(x_397X, addr_398X, 1);
  *((long *) addr_398X) = 1;
  return;}
 make_template_containing_six_ops: {
  op0_356X = merged_arg0K0;
  op1_357X = merged_arg0K1;
  op2_358X = merged_arg0K2;
  op3_359X = merged_arg0K3;
  op4_360X = merged_arg0K4;
  op5_361X = merged_arg0K5;{
  addr_399X = s48_allocate_small(10);
  *((long *) addr_399X) = 1610;
  addr_400X = s48_allocate_small(12);
  *((long *) addr_400X) = 2098;
  temp_401X = 3 + (((long) (addr_400X + 4)));
  code_402X = 3 + (((long) (addr_399X + 4)));
  addr_403X = ((char *) (-3 + temp_401X));
  S48_WRITE_BARRIER(temp_401X, addr_403X, code_402X);
  *((long *) addr_403X) = code_402X;
  *((unsigned char *) (((char *) (-3 + code_402X)))) = op0_356X;
  *((unsigned char *) ((((char *) (-3 + code_402X))) + 1)) = op1_357X;
  *((unsigned char *) ((((char *) (-3 + code_402X))) + 2)) = op2_358X;
  *((unsigned char *) ((((char *) (-3 + code_402X))) + 3)) = op3_359X;
  *((unsigned char *) ((((char *) (-3 + code_402X))) + 4)) = op4_360X;
  *((unsigned char *) ((((char *) (-3 + code_402X))) + 5)) = op5_361X;
  make_template_containing_six_ops0_return_value = temp_401X;
  goto make_template_containing_six_ops_return;}
 make_template_containing_six_ops_return:
  switch (make_template_containing_six_ops_return_tag) {
  case 0: goto make_template_containing_six_ops_return_0;
  case 1: goto make_template_containing_six_ops_return_1;
  default: goto make_template_containing_six_ops_return_2;
  }}

 make_hash_tableAgc: {
{ addr_404X = s48_allocate_tracedAgc(4100);
  if ((addr_404X == NULL)) {
    arg0K0 = 1;
    goto L9326;}
  else {
    *((long *) addr_404X) = 1048586;
    arg0K0 = (3 + (((long) (addr_404X + 4))));
    goto L9326;}}
 L9326: {
  vector_405X = arg0K0;
  if ((1 == vector_405X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_406X;
    goto L9314;}
  else {
    arg0K0 = vector_405X;
    goto L9314;}}
 L9314: {
  table_407X = arg0K0;
  arg0K0 = 0;
  goto L9349;}
 L9349: {
  i_408X = arg0K0;
  if ((1024 == i_408X)) {
    make_hash_tableAgc0_return_value = table_407X;
    goto make_hash_tableAgc_return;}
  else {
    addr_409X = (((char *) (-3 + table_407X))) + (((i_408X)<<2));
    S48_WRITE_BARRIER(table_407X, addr_409X, 1);
    *((long *) addr_409X) = 1;
    arg0K0 = (1 + i_408X);
    goto L9349;}}
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
long s48_restart(long proc_410X, long nargs_411X)
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
  int proposal_d_read_return_tag;
  long proposal_d_read0_return_value;
  int proposal_d_write_return_tag;
  int okay_copy_argsP_return_tag;
  char okay_copy_argsP0_return_value;
  int shift_space_return_tag;
  long shift_space0_return_value;
  int copy_listSAgc_return_tag;
  long copy_listSAgc0_return_value;
  int pop_args_GlistSAgc_return_tag;
  long pop_args_GlistSAgc0_return_value;
  int push_list_return_tag;
  long push_list0_return_value;
  int rest_list_setupAgc_return_tag;
  int check_events_return_tag;
  char check_events0_return_value;
  int loseD0_return_tag;
  long nargs_412X;
  long status_413X;
  long cont_414X;
  long stack_arg_count_415X;
  long list_416X;
  long marker_417X;
  long a_418X;
  long b_419X;
  long stob_420X;
  long index_421X;
  long stob_422X;
  long index_423X;
  long value_424X;
  long thing_425X;
  long index_426X;
  long count_427X;
  long x_428X;
  long n_429X;
  long list_430X;
  long length_431X;
  long start_432X;
  long count_433X;
  long list_434X;
  long count_435X;
  long wants_stack_args_436X;
  long stack_arg_count_437X;
  long list_args_438X;
  long list_arg_count_439X;
  char *message_440X;
  char * arg_1772X;
  char * loc_1771X;
  char * top_of_args_1770X;
  long i_1769X;
  long string_1768X;
  char * addr_1767X;
  long len_1766X;
  long len_1765X;
  long x_1764X;
  char *string_1763X;
  long v_1762X;
  char * new_stack_1761X;
  char * new_cont_1760X;
  char move_slowP_1759X;
  long slow_1758X;
  long len_1757X;
  long fast_1756X;
  long v_1755X;
  char *v_1754X;
  long v_1753X;
  long v_1752X;
  long obj_1751X;
  long env_1750X;
  long thread_1749X;
  long x_1748X;
  char * addr_1747X;
  long v_1746X;
  long next_stob_1745X;
  long i_1744X;
  long log_1743X;
  char * addr_1742X;
  long next_stob_1741X;
  long i_1740X;
  long log_1739X;
  long v_1738X;
  long v_1737X;
  long n_1736X;
  long extra_1735X;
  long x_size_1734X;
  char * addr_1733X;
  long x_1732X;
  char * addr_1731X;
  long a_1730X;
  long last_1729X;
  long l_1728X;
  long x_1727X;
  char * addr_1726X;
  long a_1725X;
  long value_1724X;
  long x_1723X;
  char * addr_1722X;
  long a_1721X;
  long count_1720X;
  long args_1719X;
  long value_1718X;
  long x_1717X;
  long l_1716X;
  long i_1715X;
  long list_1714X;
  long x_1713X;
  long v_1712X;
  long x_1711X;
  long v_1710X;
  long count_1709X;
  long x_1708X;
  long v_1707X;
  long status_1706X;
  long channel_1705X;
  long type_1704X;
  char v_1703X;
  char v_1702X;
  long template_1701X;
  char not_firstP_1700X;
  long cont_1699X;
  long template_1698X;
  char not_firstP_1697X;
  char * cont_1696X;
  char not_firstP_1695X;
  FILE * out_1694X;
  long current_template_1693X;
  long why_1692X;
  long val_1691X;
  long val_1690X;
  long val_1689X;
  long val_1688X;
  long val_1687X;
  long val_1686X;
  long n_1685X;
  char v_1684X;
  char * external_bignum_1683X;
  char * x_1682X;
  long val_1681X;
  long n_1680X;
  char v_1679X;
  char * external_bignum_1678X;
  char * x_1677X;
  long val_1676X;
  long n_1675X;
  char v_1674X;
  char * external_bignum_1673X;
  char * x_1672X;
  long v_1671X;
  long n_1670X;
  char v_1669X;
  long v_1668X;
  long rem_1667X;
  long quot_1666X;
  char div_by_zeroP_1665X;
  long val_1664X;
  long n_1663X;
  char v_1662X;
  char * external_bignum_1661X;
  char * x_1660X;
  long val_1659X;
  long n_1658X;
  char v_1657X;
  char * external_bignum_1656X;
  char * x_1655X;
  long val_1654X;
  long n_1653X;
  char v_1652X;
  char * external_bignum_1651X;
  char * x_1650X;
  long link_1649X;
  long v_1648X;
  long v_1647X;
  long stob_1646X;
  long v_1645X;
  long stob_1644X;
  char * addr_1643X;
  long x_1642X;
  char * addr_1641X;
  char * addr_1640X;
  long value_1639X;
  char * addr_1638X;
  long value_1637X;
  long copies_1636X;
  char * v_1635X;
  long value_1634X;
  char * y_1633X;
  char * v_1632X;
  long value_1631X;
  char * y_1630X;
  char * v_1629X;
  long value_1628X;
  char * y_1627X;
  long val_1626X;
  long v_1625X;
  long n_1624X;
  char v_1623X;
  char * rem_1622X;
  char * quot_1621X;
  char div_by_zeroP_1620X;
  char * x_1619X;
  char * v_1618X;
  long value_1617X;
  char * y_1616X;
  char * v_1615X;
  long value_1614X;
  char * y_1613X;
  char * v_1612X;
  long value_1611X;
  char * y_1610X;
  char * addr_1609X;
  long val_1608X;
  char * addr_1607X;
  long value_1606X;
  long len_1605X;
  long s2_1604X;
  long foo_1603X;
  long previous_foo_1602X;
  long value_1601X;
  long verify_1600X;
  long copies_1599X;
  long stob_1598X;
  long i_1597X;
  long x_1596X;
  long x_1595X;
  long val_1594X;
  char * addr_1593X;
  long value_1592X;
  char * v_1591X;
  long value_1590X;
  long v_1589X;
  char * v_1588X;
  long value_1587X;
  long v_1586X;
  char * v_1585X;
  long value_1584X;
  long v_1583X;
  long val_1582X;
  long val_1581X;
  long n_1580X;
  char v_1579X;
  char * external_bignum_1578X;
  char * x_1577X;
  long val_1576X;
  long n_1575X;
  char v_1574X;
  char * external_bignum_1573X;
  char * x_1572X;
  char * v_1571X;
  long value_1570X;
  char * y_1569X;
  char * v_1568X;
  long value_1567X;
  long v_1566X;
  long val_1565X;
  long v_1564X;
  char * v_1563X;
  char * v_1562X;
  long value_1561X;
  long extra1_1560X;
  long length1_1559X;
  char * v_1558X;
  long value_1557X;
  long v_1556X;
  long bucket_1555X;
  char * addr_1554X;
  long value_1553X;
  long verify_1552X;
  long log_1551X;
  long stob_1550X;
  long i_1549X;
  long x_1548X;
  long status_1547X;
  char pendingP_1546X;
  char eofP_1545X;
  long got_1544X;
  long y_1543X;
  long link_1542X;
  long val_1541X;
  long x_1540X;
  char * addr_1539X;
  long b_1538X;
  long val_1537X;
  long extra1_1536X;
  long length1_1535X;
  long extra1_1534X;
  long length1_1533X;
  long extra1_1532X;
  long length1_1531X;
  long n_1530X;
  char * v_1529X;
  long val_1528X;
  long n_1527X;
  char v_1526X;
  char * external_bignum_1525X;
  char * v_1524X;
  long n_1523X;
  char v_1522X;
  char * external_bignum_1521X;
  char * v_1520X;
  long value_1519X;
  char * y_1518X;
  char * v_1517X;
  long value_1516X;
  char * y_1515X;
  char * v_1514X;
  long value_1513X;
  long extra1_1512X;
  long val_1511X;
  long v_1510X;
  char * v_1509X;
  long extra1_1508X;
  long length1_1507X;
  long val_1506X;
  long v_1505X;
  char * v_1504X;
  long x_1503X;
  long val_1502X;
  long v_1501X;
  char * v_1500X;
  long extra0_1499X;
  long length0_1498X;
  long extra1_1497X;
  long length1_1496X;
  char * addr_1495X;
  long value_1494X;
  long offset_1493X;
  long i_1492X;
  long count_1491X;
  char * addr_1490X;
  long value_1489X;
  long offset_1488X;
  long i_1487X;
  long count_1486X;
  long obj_1485X;
  char *v_1484X;
  long link_1483X;
  long index_1482X;
  long h_1481X;
  long i_1480X;
  char * addr_1479X;
  char * addr_1478X;
  long value_1477X;
  char * addr_1476X;
  char * addr_1475X;
  char * addr_1474X;
  long value_1473X;
  char * addr_1472X;
  char * addr_1471X;
  long value_1470X;
  long proposal_1469X;
  long entry_1468X;
  long thing_1467X;
  long log_1466X;
  long copies_1465X;
  char * addr_1464X;
  long x_1463X;
  long val_1462X;
  long status_1461X;
  long v_1460X;
  long len_1459X;
  long s2_1458X;
  long foo_1457X;
  long val_1456X;
  long v_1455X;
  long n_1454X;
  char v_1453X;
  char * external_bignum_1452X;
  char * x_1451X;
  long extra0_1450X;
  long length0_1449X;
  long extra0_1448X;
  long length0_1447X;
  long extra0_1446X;
  long length0_1445X;
  char * v_1444X;
  long value_1443X;
  long v_1442X;
  char * v_1441X;
  long value_1440X;
  long v_1439X;
  char * v_1438X;
  long v_1437X;
  char * x_1436X;
  char * v_1435X;
  long n_1434X;
  char * v_1433X;
  long value_1432X;
  long extra1_1431X;
  char * v_1430X;
  long value_1429X;
  long extra1_1428X;
  long val_1427X;
  long val_1426X;
  long val_1425X;
  long val_1424X;
  long val_1423X;
  long extra0_1422X;
  long length0_1421X;
  long val_1420X;
  long v_1419X;
  char * v_1418X;
  long x_1417X;
  long val_1416X;
  long v_1415X;
  char * v_1414X;
  long extra0_1413X;
  long length0_1412X;
  long y_1411X;
  long x_1410X;
  long extra0_1409X;
  long length0_1408X;
  long count_1407X;
  long index_1406X;
  long i_1405X;
  long env_1404X;
  long count_1403X;
  long i_1402X;
  long env_1401X;
  long n_1400X;
  long x_1399X;
  long x_1398X;
  long i_1397X;
  long l_1396X;
  long n_1395X;
  long table_1394X;
  long val_1393X;
  char temp_1392X;
  char minutesP_1391X;
  long v_1390X;
  long vector_1389X;
  char * addr_1388X;
  long x_1387X;
  long value_1386X;
  long verify_1385X;
  long copies_1384X;
  long stob_1383X;
  long i_1382X;
  char * addr_1381X;
  long v_1380X;
  char * addr_1379X;
  char * addr_1378X;
  long val_1377X;
  long x_1376X;
  long status_1375X;
  char pendingP_1374X;
  long got_1373X;
  long y_1372X;
  char v_1371X;
  long count_1370X;
  long start_1369X;
  char waitP_1368X;
  long x_1367X;
  long status_1366X;
  long channel_1365X;
  long v_1364X;
  long v_1363X;
  long v_1362X;
  long reason_1361X;
  long channel_1360X;
  long index_1359X;
  long bucket_1358X;
  long i_1357X;
  long i_1356X;
  char * addr_1355X;
  long i_1354X;
  long rest_list_1353X;
  long i_1352X;
  long v_1351X;
  long n_1350X;
  char v_1349X;
  char * external_bignum_1348X;
  char * x_1347X;
  long val_1346X;
  long v_1345X;
  char * v_1344X;
  char * v_1343X;
  long value_1342X;
  long needed_1341X;
  long y_1340X;
  long y_1339X;
  long x_1338X;
  long val_1337X;
  long v_1336X;
  char * v_1335X;
  long val_1334X;
  long v_1333X;
  char * v_1332X;
  long extra_1331X;
  long length_1330X;
  long count_1329X;
  long x_1328X;
  long extra_1327X;
  long length_1326X;
  char * v_1325X;
  long value_1324X;
  long extra_1323X;
  long length_1322X;
  long x_1321X;
  long c_1320X;
  long b_1319X;
  long extra0_1318X;
  long length0_1317X;
  long c_1316X;
  long b_1315X;
  long extra0_1314X;
  long length0_1313X;
  long val_1312X;
  long v_1311X;
  long v_1310X;
  long v_1309X;
  long val_1308X;
  long v_1307X;
  long v_1306X;
  long v_1305X;
  long val_1304X;
  long v_1303X;
  long v_1302X;
  long v_1301X;
  long val_1300X;
  long v_1299X;
  long v_1298X;
  long v_1297X;
  long val_1296X;
  char b_1295X;
  long result_1294X;
  char x_1293X;
  long c_1292X;
  long b_1291X;
  long c_1290X;
  long mid_c_1289X;
  long v_1288X;
  long v_1287X;
  long lo_c_1286X;
  long hi_b_1285X;
  long hi_a_1284X;
  long lo_b_1283X;
  long lo_a_1282X;
  long b_1281X;
  long x_1280X;
  long v_1279X;
  long obj_1278X;
  long stack_arg_count_1277X;
  long tag_1276X;
  long v_1275X;
  long template_1274X;
  char * cont_1273X;
  long back_1272X;
  long env_1271X;
  long offset_1270X;
  long i_1269X;
  long back_1268X;
  long env_1267X;
  long offset_1266X;
  long i_1265X;
  char x_1264X;
  long channel_1263X;
  long handlers_1262X;
  long arg_count_1261X;
  long cont_1260X;
  long obj_1259X;
  long type_1258X;
  long thing_1257X;
  long stuff_1256X;
  char * addr_1255X;
  long val_1254X;
  long i_1253X;
  long b_1252X;
  long port_1251X;
  long Kchar_1250X;
  long i_1249X;
  long p_1248X;
  long b_1247X;
  long port_1246X;
  char * addr_1245X;
  long val_1244X;
  long i_1243X;
  long p_1242X;
  long b_1241X;
  long port_1240X;
  char * addr_1239X;
  long value_1238X;
  long list_1237X;
  long head_1236X;
  char move_slowP_1235X;
  long slow_1234X;
  long list_1233X;
  long string_1232X;
  char * addr_1231X;
  long len_1230X;
  long x_1229X;
  long val_1228X;
  long mseconds_1227X;
  long seconds_1226X;
  long option_1225X;
  long x_1224X;
  long x_1223X;
  long v_1222X;
  long v_1221X;
  long x_1220X;
  long result_1219X;
  char * args_1218X;
  long name_1217X;
  long proc_1216X;
  long rest_list_1215X;
  long x_1214X;
  long x_1213X;
  long v_1212X;
  long tem_1211X;
  long pc_1210X;
  char * addr_1209X;
  long x_1208X;
  long proposal_1207X;
  long x_1206X;
  long x_1205X;
  long value_1204X;
  long vector_1203X;
  long type_1202X;
  char firstP_1201X;
  long vector_1200X;
  char firstP_1199X;
  long x_1198X;
  long h_1197X;
  long i_1196X;
  long x_1195X;
  long x_1194X;
  long status_1193X;
  long reason_1192X;
  long x_1191X;
  char * addr_1190X;
  long next_stob_1189X;
  long i_1188X;
  long x_1187X;
  long v_1186X;
  long next_stob_1185X;
  long i_1184X;
  long value_1183X;
  char * addr_1182X;
  long count_1181X;
  long to_index_1180X;
  long from_index_1179X;
  long copies_1178X;
  long left_1177X;
  long value_1176X;
  long value_1175X;
  long verify_1174X;
  long log_1173X;
  long stob_1172X;
  long i_1171X;
  char * addr_1170X;
  long old_1169X;
  long x_1168X;
  char * addr_1167X;
  long channel_1166X;
  long res_1165X;
  long i_1164X;
  long x_1163X;
  long y_1162X;
  long n_1161X;
  char * addr_1160X;
  long prev_1159X;
  long ch_1158X;
  long val_1157X;
  long x_1156X;
  long val_1155X;
  long x_1154X;
  long x_1153X;
  long x_1152X;
  long v_1151X;
  long v_1150X;
  long val_1149X;
  long x_1148X;
  char * addr_1147X;
  char * addr_1146X;
  long x_1145X;
  long val_1144X;
  long x_1143X;
  long link_1142X;
  long index_1141X;
  long h_1140X;
  long i_1139X;
  long x_1138X;
  long x_1137X;
  long x_1136X;
  long x_1135X;
  long vector_1134X;
  long x_1133X;
  long x_1132X;
  long x_1131X;
  long vector_1130X;
  char * addr_1129X;
  long value_1128X;
  long value_1127X;
  long x_1126X;
  char * addr_1125X;
  long value_1124X;
  long i_1123X;
  long value_1122X;
  long i_1121X;
  long value_1120X;
  long val_1119X;
  long x_1118X;
  long val_1117X;
  long x_1116X;
  long val_1115X;
  long val_1114X;
  char * v_1113X;
  long value_1112X;
  long needed_1111X;
  long y_1110X;
  long x_1109X;
  long result_1108X;
  long x_1107X;
  long x_1106X;
  long x_1105X;
  long count_1104X;
  long value_1103X;
  long val_1102X;
  long val_1101X;
  long x_1100X;
  long x_1099X;
  long x_1098X;
  long n_1097X;
  long a_1096X;
  long y_1095X;
  long x_1094X;
  long a_1093X;
  long y_1092X;
  long x_1091X;
  long val_1090X;
  long val_1089X;
  char b_1088X;
  long val_1087X;
  char b_1086X;
  long val_1085X;
  char b_1084X;
  long val_1083X;
  long Kdouble_1082X;
  char * addr_1081X;
  double value_1080X;
  long value_1079X;
  double y_1078X;
  long value_1077X;
  long a_1076X;
  long Kdouble_1075X;
  char * addr_1074X;
  double x_1073X;
  long value_1072X;
  double y_1071X;
  long value_1070X;
  long val_1069X;
  long v_1068X;
  char * v_1067X;
  long Kdouble_1066X;
  char * addr_1065X;
  double x_1064X;
  long value_1063X;
  double y_1062X;
  long value_1061X;
  long a_1060X;
  long Kdouble_1059X;
  char * addr_1058X;
  double x_1057X;
  long value_1056X;
  double y_1055X;
  long value_1054X;
  long val_1053X;
  long v_1052X;
  char * v_1051X;
  long val_1050X;
  long val_1049X;
  long delta_1048X;
  long delta_1047X;
  long offset_1046X;
  long x_1045X;
  char v_1044X;
  long tem_1043X;
  long args_1042X;
  long x_1041X;
  long args_1040X;
  long x_1039X;
  long x_1038X;
  long template_1037X;
  char * cont_1036X;
  long x_1035X;
  char * addr_1034X;
  long a_1033X;
  long template_1032X;
  char * cont_1031X;
  long wants_stack_args_1030X;
  long next_1029X;
  long consumer_1028X;
  long cont_1027X;
  long protocol_1026X;
  char * code_pointer_1025X;
  long obj_1024X;
  char * addr_1023X;
  long list_args_1022X;
  long follower_1021X;
  long list_1020X;
  long proc_1019X;
  long args_1018X;
  long list_arg_count_1017X;
  char okayP_1016X;
  long stack_nargs_1015X;
  long list_args_1014X;
  char v_1013X;
  long value_1012X;
  long start_i_1011X;
  long start_i_1010X;
  char * addr_1009X;
  long closure_1008X;
  long index_1007X;
  long x_1006X;
  char * addr_1005X;
  long x_1004X;
  char * addr_1003X;
  long value_1002X;
  long i_1001X;
  long value_1000X;
  long key_999X;
  long env_998X;
  long v_997X;
  long p_996X;
  long v_995X;
  char * addr_994X;
  long value_993X;
  long index_992X;
  long i_991X;
  long env_990X;
  long i_989X;
  long env_988X;
  long i_987X;
  long env_986X;
  long n_985X;
  long n_984X;
  long x_983X;
  long x_982X;
  char * addr_981X;
  long next_980X;
  long channel_979X;
  long n_978X;
  long x_977X;
  long n_976X;
  long x_975X;
  long m_974X;
  long i_973X;
  char * arg_972X;
  char * loc_971X;
  long x_970X;
  long l_969X;
  long stack_nargs_968X;
  long x_967X;
  long obj_966X;
  long obj_965X;
  long list_arg_count_964X;
  long list_args_963X;
  long stack_arg_count_962X;
  long v_961X;
  FILE * out_960X;
  long x_959X;
  long v_958X;
  long v_957X;
  long v_956X;
  long v_955X;
  long v_954X;
  long v_953X;
  long v_952X;
  long v_951X;
  long x_950X;
  long v_949X;
  long len_948X;
  long value_947X;
  long index_946X;
  long arg4_945X;
  long arg3_944X;
  long arg2_943X;
  long x_942X;
  long v_941X;
  long v_940X;
  long len_939X;
  long index_938X;
  long arg3_937X;
  long arg2_936X;
  long list_935X;
  long arg2_934X;
  long x_933X;
  long n_932X;
  long arg2_931X;
  long len_930X;
  long x_929X;
  long obj_928X;
  long arg2_927X;
  long x_926X;
  long arg2_925X;
  long x_924X;
  long status_923X;
  long value_922X;
  long key_921X;
  long arg2_920X;
  long x_919X;
  long mseconds_918X;
  long seconds_917X;
  long mseconds_916X;
  long seconds_915X;
  long x_914X;
  long other_913X;
  long option_912X;
  long arg2_911X;
  long x_910X;
  long arg2_909X;
  long x_908X;
  long arg2_907X;
  long x_906X;
  long rest_list_905X;
  long p_904X;
  long nargs_903X;
  long p_902X;
  long x_901X;
  long arg2_900X;
  long x_899X;
  long p_898X;
  long p_897X;
  long p_896X;
  long old_895X;
  long temp_894X;
  long obj_893X;
  long size_892X;
  long template_891X;
  long pc_890X;
  long data_889X;
  long temp_888X;
  long obj_887X;
  long val_886X;
  long x_885X;
  long x_884X;
  long type_883X;
  long x_882X;
  long x_881X;
  long bytes_880X;
  long x_879X;
  long other_878X;
  long key_877X;
  long arg2_876X;
  long x_875X;
  char * addr_874X;
  long b_873X;
  long x_872X;
  char * addr_871X;
  long proc_870X;
  long arg2_869X;
  long n_868X;
  long x_867X;
  long obj_866X;
  long status_865X;
  long status_864X;
  long status_863X;
  long status_862X;
  long status_861X;
  long status_860X;
  FILE * port_859X;
  long undumpables_858X;
  long obj_857X;
  long arg4_856X;
  long arg3_855X;
  long arg2_854X;
  long x_853X;
  long log_852X;
  long index_851X;
  long x_850X;
  long len_849X;
  long byte_848X;
  long index_847X;
  long arg3_846X;
  long arg2_845X;
  long log_844X;
  long index_843X;
  long x_842X;
  long len_841X;
  long index_840X;
  long arg2_839X;
  long x_838X;
  char v_837X;
  char v_836X;
  long v_835X;
  long count_834X;
  long to_index_833X;
  long from_index_832X;
  long arg5_831X;
  long arg4_830X;
  long arg3_829X;
  long arg2_828X;
  long v_827X;
  long x_826X;
  long type_825X;
  long offset_824X;
  long stob_823X;
  long log_822X;
  long proposal_821X;
  long proposal_820X;
  char * addr_819X;
  long next_818X;
  long channel_817X;
  long n_816X;
  char * addr_815X;
  long head_814X;
  long channel_813X;
  long obj_812X;
  long x_811X;
  long status_810X;
  char readyP_809X;
  long channel_808X;
  long obj_807X;
  char v_806X;
  long count_805X;
  long start_804X;
  long arg4_803X;
  long arg3_802X;
  long arg2_801X;
  long x_800X;
  long arg5_799X;
  long arg4_798X;
  long arg3_797X;
  long arg2_796X;
  long x_795X;
  long status_794X;
  long channel_793X;
  long obj_792X;
  long x_791X;
  long mode_790X;
  long arg2_789X;
  long x_788X;
  long x_787X;
  long x_786X;
  long arg2_785X;
  long descriptor_784X;
  long x_783X;
  long obj_782X;
  long n_781X;
  long string_780X;
  long table_779X;
  long obj_778X;
  long len_777X;
  char Kchar_776X;
  long index_775X;
  long arg3_774X;
  long arg2_773X;
  long len_772X;
  long index_771X;
  long arg2_770X;
  long obj_769X;
  char * addr_768X;
  long len_767X;
  char init_766X;
  long len_765X;
  long arg2_764X;
  long len_763X;
  long Kchar_762X;
  long index_761X;
  long arg3_760X;
  long arg2_759X;
  long len_758X;
  long index_757X;
  long arg2_756X;
  long obj_755X;
  long x_754X;
  char * addr_753X;
  long init_752X;
  long len_751X;
  long arg2_750X;
  long v_749X;
  long index_748X;
  long len_747X;
  long type_746X;
  long value_745X;
  long arg3_744X;
  long arg2_743X;
  long v_742X;
  long v_741X;
  long index_740X;
  long len_739X;
  long type_738X;
  long index_737X;
  long arg2_736X;
  char * addr_735X;
  long len_734X;
  long len_733X;
  long type_732X;
  long init_731X;
  long arg2_730X;
  long v_729X;
  long type_728X;
  long offset_727X;
  long value_726X;
  long arg2_725X;
  long type_724X;
  long offset_723X;
  long stob_722X;
  long rest_list_721X;
  long stack_nargs_720X;
  long p_719X;
  long new_718X;
  char * addr_717X;
  long len_716X;
  long type_715X;
  long len_714X;
  long p_713X;
  long new_712X;
  char * addr_711X;
  long len_710X;
  long type_709X;
  long len_708X;
  long type_707X;
  long stob_706X;
  long type_705X;
  long x_704X;
  long x_703X;
  long x_702X;
  long x_701X;
  long x_700X;
  long x_699X;
  long arg2_698X;
  long x_697X;
  long arg2_696X;
  long x_695X;
  long v_694X;
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
  long x_681X;
  long arg2_680X;
  long x_679X;
  long arg2_678X;
  long x_677X;
  long arg2_676X;
  long x_675X;
  long x_674X;
  long v_673X;
  long x_672X;
  long x_671X;
  long x_670X;
  long x_669X;
  long x_668X;
  long x_667X;
  long x_666X;
  long x_665X;
  long x_664X;
  long n_663X;
  long n_662X;
  long n_661X;
  long n_660X;
  long n_659X;
  long a_658X;
  long y_657X;
  long arg2_656X;
  long b_655X;
  long a_654X;
  long y_653X;
  long arg2_652X;
  long y_651X;
  long arg2_650X;
  long y_649X;
  long arg2_648X;
  long y_647X;
  long arg2_646X;
  long y_645X;
  long arg2_644X;
  long y_643X;
  long arg2_642X;
  long b_641X;
  long a_640X;
  long y_639X;
  long arg2_638X;
  long x_637X;
  long y_636X;
  long arg2_635X;
  long b_634X;
  long a_633X;
  long y_632X;
  long arg2_631X;
  long x_630X;
  long y_629X;
  long arg2_628X;
  long x_627X;
  long x_626X;
  long n_625X;
  long n_624X;
  long n_623X;
  long n_622X;
  long n_621X;
  long x_620X;
  long x_619X;
  long arg2_618X;
  long rest_list_617X;
  long x_616X;
  long rest_list_615X;
  long stack_nargs_614X;
  long arg0_613X;
  long arg1_612X;
  long rest_list_611X;
  long arg0_610X;
  long stack_nargs_609X;
  long x_608X;
  long index_607X;
  long val_606X;
  long max_605X;
  long p_604X;
  long space_603X;
  long index_602X;
  long v_601X;
  long code_600X;
  long tem_599X;
  long rest_list_598X;
  long stack_nargs_597X;
  long p_596X;
  long p_595X;
  long cont_594X;
  long v_593X;
  long rest_list_592X;
  long proc_591X;
  long stack_nargs_590X;
  long p_589X;
  long x_588X;
  long args_587X;
  long length_586X;
  char okayP_585X;
  long list_args_584X;
  long obj_583X;
  long stack_arg_count_582X;
  long v_581X;
  long obj_580X;
  long stack_arg_count_579X;
  long tag_578X;
  long proc_577X;
  long protocol_576X;
  long code_575X;
  long template_574X;
  long obj_573X;
  long stack_arg_count_572X;
  long v_571X;
  char * cont_570X;
  long template_569X;
  char * code_pointer_568X;
  long value_567X;
  long x_566X;
  long x_565X;
  long x_564X;
  char * addr_563X;
  long value_562X;
  long v_561X;
  long new_env_560X;
  char * addr_559X;
  long len_558X;
  long total_count_557X;
  char * addr_556X;
  long value_555X;
  long v_554X;
  long new_env_553X;
  char * addr_552X;
  long len_551X;
  long total_count_550X;
  long vector_549X;
  char * addr_548X;
  long len_547X;
  long count_546X;
  long p_545X;
  long v_544X;
  char * addr_543X;
  long val_542X;
  long x_541X;
  long location_540X;
  long location_539X;
  long env_538X;
  long back_537X;
  long env_536X;
  long back_535X;
  long env_534X;
  long back_533X;
  long x_532X;
  long count_531X;
  char * code_pointer_530X;
  long v_529X;
  long n_528X;
  char * cont_527X;
  long template_526X;
  char * code_pointer_525X;
  long tem_524X;
  char * addr_523X;
  long x_522X;
  long n_521X;
  long x_520X;
  long x_519X;
  long x_518X;
  long x_517X;
  long stack_arg_count_516X;
  long args_515X;
  long template_514X;
  char * cont_513X;
  char * arg_top_512X;
  long list_arg_count_511X;
  long list_args_510X;
  long stack_nargs_509X;
  long bytes_used_508X;
  long count_507X;
  long v_506X;
  char * arg_505X;
  char * loc_504X;
  long v_503X;
  long bytes_used_502X;
  long args_501X;
  long list_args_500X;
  long stack_nargs_499X;
  long template_498X;
  char * cont_497X;
  long v_496X;
  long final_stack_arg_count_495X;
  char v_494X;
  long template_493X;
  char * cont_492X;
  char * arg_top_491X;
  long args_490X;
  long count_489X;
  long next_488X;
  long consumer_487X;
  long cont_486X;
  long template_485X;
  char * cont_484X;
  long v_483X;
  long protocol_482X;
  char * code_pointer_481X;
  long list_arg_count_480X;
  long list_args_479X;
  long stack_nargs_478X;
  long obj_477X;
  long skip_476X;
  long template_475X;
  long v_474X;
  long obj_473X;
  long stack_arg_count_472X;
  long tag_471X;
  long proc_470X;
  long stack_arg_count_469X;
  long skip_468X;
  long handlers_467X;
  long opcode_466X;
  long nargs_465X;
  long index_464X;
  long length_463X;
  long wants_stack_args_462X;
  long skip_461X;
  long skip_460X;
  char nativeP_459X;
  long stack_space_458X;
  long protocol_457X;
  long v_456X;
  long x_455X;
  long args_454X;
  long v_453X;
  long v_452X;
  long v_451X;
  long list_arg_count_450X;
  long list_args_449X;
  long stack_arg_count_448X;
  long exception_447X;
  long total_arg_count_446X;
  long code_445X;
  long list_arg_count_444X;
  long list_args_443X;
  long stack_arg_count_442X;
  long obj_441X;
 {  if ((3 == (3 & proc_410X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_410X))) + -4))))>>2))))) {
      SvalS = proc_410X;
      obj_441X = SvalS;
      if ((3 == (3 & obj_441X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_441X))) + -4))))>>2))))) {
          arg0K0 = nargs_411X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L47051;}
        else {
          arg0K0 = 3;
          arg0K1 = nargs_411X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22536;}}
      else {
        arg0K0 = 3;
        arg0K1 = nargs_411X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22536;}}
    else {
      goto L23203;}}
  else {
    goto L23203;}}
 L47051: {
  stack_arg_count_442X = arg0K0;
  list_args_443X = arg0K1;
  list_arg_count_444X = arg0K2;
  code_445X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  total_arg_count_446X = stack_arg_count_442X + list_arg_count_444X;
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_445X))) + 1)));
  arg0K1 = 64;
  arg2K2 = 0;
  goto L47067;}
 L22536: {
  exception_447X = arg0K0;
  stack_arg_count_448X = arg0K1;
  list_args_449X = arg0K2;
  list_arg_count_450X = arg0K3;
  if (((StemplateS) == (SvalS))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      return v_451X;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      return v_452X;}}
  else {
    merged_arg0K0 = list_args_449X;
    merged_arg0K1 = list_arg_count_450X;
    copy_listSAgc_return_tag = 0;
    goto copy_listSAgc;
   copy_listSAgc_return_0:
    v_453X = copy_listSAgc0_return_value;
    merged_arg0K0 = v_453X;
    merged_arg0K1 = stack_arg_count_448X;
    pop_args_GlistSAgc_return_tag = 0;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_0:
    args_454X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(exception_447X, 0);
    x_455X = SvalS;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_455X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_454X;
    arg0K0 = 2;
    goto L22004;}}
 L23203: {
  ps_error("s48-restart called with non-procedure", 1, proc_410X);
  return v_456X;}
 L47067: {
  protocol_457X = arg0K0;
  stack_space_458X = arg0K1;
  nativeP_459X = arg2K2;
  if ((69 == protocol_457X)) {
    if ((total_arg_count_446X < 3)) {
      skip_460X = *((unsigned char *) ((((char *) (-3 + code_445X))) + (2 + total_arg_count_446X)));
      if ((0 == skip_460X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_442X;
        arg0K2 = list_args_443X;
        arg0K3 = list_arg_count_444X;
        goto L22536;}
      else {
        merged_arg0K0 = list_args_443X;
        merged_arg0K1 = list_arg_count_444X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = skip_460X;
        arg0K1 = total_arg_count_446X;
        goto L47071;}}
    else {
      skip_461X = *((unsigned char *) ((((char *) (-3 + code_445X))) + 5));
      if ((0 == skip_461X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_442X;
        arg0K2 = list_args_443X;
        arg0K3 = list_arg_count_444X;
        goto L22536;}
      else {
        arg0K0 = skip_461X;
        goto L47106;}}}
  else {
    if ((63 < protocol_457X)) {
      if ((65 == protocol_457X)) {
        wants_stack_args_462X = ((((*((unsigned char *) ((((char *) (-3 + code_445X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_445X))) + 3)));
        if ((total_arg_count_446X < wants_stack_args_462X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_442X;
          arg0K2 = list_args_443X;
          arg0K3 = list_arg_count_444X;
          goto L22536;}
        else {
          merged_arg0K0 = wants_stack_args_462X;
          merged_arg0K1 = stack_arg_count_442X;
          merged_arg0K2 = list_args_443X;
          merged_arg0K3 = list_arg_count_444X;
          rest_list_setupAgc_return_tag = 0;
          goto rest_list_setupAgc;
         rest_list_setupAgc_return_0:
          arg0K0 = 4;
          arg0K1 = (1 + wants_stack_args_462X);
          goto L47071;}}
      else {
        if ((68 == protocol_457X)) {
          if ((total_arg_count_446X < (*((unsigned char *) ((((char *) (-3 + code_445X))) + 2))))) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_442X;
            arg0K2 = list_args_443X;
            arg0K3 = list_arg_count_444X;
            goto L22536;}
          else {
            arg0K0 = 3;
            goto L47106;}}
        else {
          if ((127 < protocol_457X)) {
            arg0K0 = (127 & protocol_457X);
            arg0K1 = stack_space_458X;
            arg2K2 = 1;
            goto L47067;}
          else {
            if ((64 == protocol_457X)) {
              if (((((((*((unsigned char *) ((((char *) (-3 + code_445X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_445X))) + 3)))) == total_arg_count_446X)) {
                if ((0 == list_arg_count_444X)) {
                  arg0K0 = 4;
                  arg0K1 = total_arg_count_446X;
                  goto L47071;}
                else {
                  merged_arg0K0 = list_args_443X;
                  merged_arg0K1 = list_arg_count_444X;
                  push_list_return_tag = 1;
                  goto push_list;
                 push_list_return_1:
                  arg0K0 = 4;
                  arg0K1 = total_arg_count_446X;
                  goto L47071;}}
              else {
                arg0K0 = 4;
                arg0K1 = stack_arg_count_442X;
                arg0K2 = list_args_443X;
                arg0K3 = list_arg_count_444X;
                goto L22536;}}
            else {
              if ((67 == protocol_457X)) {
                length_463X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_445X))) + -4))))>>8);
                index_464X = -2 + length_463X;
                arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_445X))) + (-3 + length_463X))));
                arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_445X))) + index_464X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_445X))) + (1 + index_464X)))));
                arg2K2 = nativeP_459X;
                goto L47067;}
              else {
                ps_error("unknown protocol", 1, protocol_457X);
                arg0K0 = 4;
                arg0K1 = stack_arg_count_442X;
                arg0K2 = list_args_443X;
                arg0K3 = list_arg_count_444X;
                goto L22536;}}}}}}
    else {
      if ((protocol_457X == total_arg_count_446X)) {
        if ((0 == list_arg_count_444X)) {
          arg0K0 = 2;
          arg0K1 = total_arg_count_446X;
          goto L47071;}
        else {
          merged_arg0K0 = list_args_443X;
          merged_arg0K1 = list_arg_count_444X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          arg0K0 = 2;
          arg0K1 = total_arg_count_446X;
          goto L47071;}}
      else {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_442X;
        arg0K2 = list_args_443X;
        arg0K3 = list_arg_count_444X;
        goto L22536;}}}}
 L22004: {
  nargs_465X = arg0K0;
  opcode_466X = (((*((long *) ((SstackS) + (4 + (((nargs_465X)<<2)))))))>>2);
  handlers_467X = SHARED_REF((Sexception_handlersS));
  if ((3 == (3 & handlers_467X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_467X))) + -4))))>>2))))) {
      goto L22067;}
    else {
      goto L22140;}}
  else {
    goto L22140;}}
 L47071: {
  skip_468X = arg0K0;
  stack_arg_count_469X = arg0K1;
  if (nativeP_459X) {
    proc_470X = SvalS;
    merged_arg0K0 = stack_arg_count_469X;
    move_args_above_contB_return_tag = 0;
    goto move_args_above_contB;
   move_args_above_contB_return_0:
    tag_471X = s48_call_native_procedure(proc_470X, skip_468X);
    if ((tag_471X == 0)) {
      arg0K0 = (s48_Snative_protocolS);
      arg0K1 = 25;
      arg0K2 = 0;
      goto L21734;}
    else {
      if ((tag_471X == 1)) {
        stack_arg_count_472X = s48_Snative_protocolS;
        obj_473X = SvalS;
        if ((3 == (3 & obj_473X))) {
          if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_473X))) + -4))))>>2))))) {
            arg0K0 = stack_arg_count_472X;
            arg0K1 = 25;
            arg0K2 = 0;
            goto L47051;}
          else {
            arg0K0 = 3;
            arg0K1 = stack_arg_count_472X;
            arg0K2 = 25;
            arg0K3 = 0;
            goto L22536;}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_472X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22536;}}
      else {
        ps_error("unexpected native return value", 1, tag_471X);
        return v_474X;}}}
  else {
    template_475X = *((long *) (((char *) (-3 + (SvalS)))));
    StemplateS = template_475X;
    Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_475X)))))))) + skip_468X);
    SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
    if ((stack_space_458X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L46947;}
    else {
      s48_copy_stack_into_heap();
      if ((stack_space_458X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
        goto L46947;}
      else {
        ps_error("VM's stack is too small (how can this happen?)", 0);
        goto L46947;}}}}
 L47106: {
  skip_476X = arg0K0;
  if ((total_arg_count_446X < 3)) {
    arg0K0 = total_arg_count_446X;
    goto L47114;}
  else {
    if ((2 < stack_arg_count_442X)) {
      arg0K0 = stack_arg_count_442X;
      goto L47114;}
    else {
      arg0K0 = 2;
      goto L47114;}}}
 L22067: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_467X))) + (((opcode_466X)<<2)))));
  obj_477X = SvalS;
  if ((3 == (3 & obj_477X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_477X))) + -4))))>>2))))) {
      goto L22084;}
    else {
      goto L22154;}}
  else {
    goto L22154;}}
 L22140: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L22067;}
 L21734: {
  stack_nargs_478X = arg0K0;
  list_args_479X = arg0K1;
  list_arg_count_480X = arg0K2;
  code_pointer_481X = ((char *) (*((long *) (ScontS))));
  protocol_482X = *((unsigned char *) (code_pointer_481X + 1));
  if ((1 == protocol_482X)) {
    if ((1 == (stack_nargs_478X + list_arg_count_480X))) {
      if ((1 == stack_nargs_478X)) {
        v_483X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_483X;
        goto L21807;}
      else {
        arg0K0 = (*((long *) (((char *) (-3 + list_args_479X)))));
        goto L21807;}}
    else {
      arg0K0 = stack_nargs_478X;
      arg0K1 = list_args_479X;
      goto L22684;}}
  else {
    if ((66 == protocol_482X)) {
      cont_484X = ScontS;
      template_485X = *((long *) (cont_484X + 8));
      Scode_pointerS = (((char *) (*((long *) cont_484X))));
      StemplateS = template_485X;
      SenvS = (*((long *) (cont_484X + 12)));
      ScontS = (((char *) (*((long *) (cont_484X + 4)))));
      SstackS = (cont_484X + 16);
      arg0K0 = 1;
      goto L24379;}
    else {
      if ((71 == protocol_482X)) {
        cont_486X = *((long *) ((Sbottom_of_stackS) + 4));
        if ((3 == (3 & cont_486X))) {
          if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_486X))) + -4))))>>2))))) {
            merged_arg0K0 = cont_486X;
            merged_arg0K1 = stack_nargs_478X;
            copy_continuation_from_heapB_return_tag = 0;
            goto copy_continuation_from_heapB;
           copy_continuation_from_heapB_return_0:
            arg0K0 = stack_nargs_478X;
            arg0K1 = list_args_479X;
            arg0K2 = list_arg_count_480X;
            goto L21734;}
          else {
            goto L21765;}}
        else {
          goto L21765;}}
      else {
        if ((70 == protocol_482X)) {
          consumer_487X = *((long *) ((ScontS) + 16));
          next_488X = *((long *) ((ScontS) + 4));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) ((ScontS) + 4)) = (*((long *) ((((char *) (-3 + next_488X))) + 4)));
            goto L21777;}
          else {
            ScontS = (((char *) next_488X));
            goto L21777;}}
        else {
          if ((63 < protocol_482X)) {
            if ((65 == protocol_482X)) {
              count_489X = ((((*((unsigned char *) (code_pointer_481X + 2))))<<8)) + (*((unsigned char *) (code_pointer_481X + 3)));
              if (((stack_nargs_478X + list_arg_count_480X) < count_489X)) {
                merged_arg0K0 = list_args_479X;
                merged_arg0K1 = stack_nargs_478X;
                pop_args_GlistSAgc_return_tag = 1;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_1:
                args_490X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_490X;
                arg0K0 = 2;
                goto L22004;}
              else {
                arg_top_491X = SstackS;
                cont_492X = ScontS;
                template_493X = *((long *) (cont_492X + 8));
                Scode_pointerS = (((char *) (*((long *) cont_492X))));
                StemplateS = template_493X;
                SenvS = (*((long *) (cont_492X + 12)));
                ScontS = (((char *) (*((long *) (cont_492X + 4)))));
                SstackS = (cont_492X + 16);
                arg1K0 = ((SstackS) + -4);
                arg1K1 = (arg_top_491X + (-4 + (((stack_nargs_478X)<<2))));
                goto L25516;}}
            else {
              if ((64 == protocol_482X)) {
                arg0K0 = (((((*((unsigned char *) (code_pointer_481X + 2))))<<8)) + (*((unsigned char *) (code_pointer_481X + 3))));
                arg0K1 = 3;
                arg0K2 = stack_nargs_478X;
                arg0K3 = list_args_479X;
                arg0K4 = list_arg_count_480X;
                goto L25346;}
              else {
                ps_error("unknown protocol", 1, protocol_482X);
                arg0K0 = stack_nargs_478X;
                arg0K1 = list_args_479X;
                goto L22684;}}}
          else {
            arg0K0 = protocol_482X;
            arg0K1 = 1;
            arg0K2 = stack_nargs_478X;
            arg0K3 = list_args_479X;
            arg0K4 = list_arg_count_480X;
            goto L25346;}}}}}}
 L46947: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 0;
      goto check_events;
     check_events_return_0:
      v_494X = check_events0_return_value;
      if (v_494X) {
        arg0K0 = stack_arg_count_469X;
        goto L22237;}
      else {
        goto L46951;}}
    else {
      arg0K0 = stack_arg_count_469X;
      goto L22237;}}
  else {
    goto L46951;}}
 L47114: {
  final_stack_arg_count_495X = arg0K0;
  if ((stack_arg_count_442X < final_stack_arg_count_495X)) {
    arg0K0 = final_stack_arg_count_495X;
    goto L47118;}
  else {
    arg0K0 = stack_arg_count_442X;
    goto L47118;}}
 L22084: {
  StemplateS = (SvalS);
  Slosing_opcodeS = opcode_466X;
  arg0K0 = (2 + nargs_465X);
  arg0K1 = 25;
  arg0K2 = 0;
  goto L47051;}
 L22154: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L22084;}
 L21807: {
  v_496X = arg0K0;
  SvalS = v_496X;
  cont_497X = ScontS;
  template_498X = *((long *) (cont_497X + 8));
  Scode_pointerS = (((char *) (*((long *) cont_497X))));
  StemplateS = template_498X;
  SenvS = (*((long *) (cont_497X + 12)));
  ScontS = (((char *) (*((long *) (cont_497X + 4)))));
  SstackS = (cont_497X + 16);
  arg0K0 = 1;
  goto L24379;}
 L22684: {
  stack_nargs_499X = arg0K0;
  list_args_500X = arg0K1;
  merged_arg0K0 = list_args_500X;
  merged_arg0K1 = stack_nargs_499X;
  pop_args_GlistSAgc_return_tag = 2;
  goto pop_args_GlistSAgc;
 pop_args_GlistSAgc_return_2:
  args_501X = pop_args_GlistSAgc0_return_value;
  push_exception_continuationB(4, 0);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = 1;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = args_501X;
  arg0K0 = 2;
  goto L22004;}
 L24379: {
  bytes_used_502X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_502X));
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L21765: {
  if ((1 == (stack_nargs_478X + list_arg_count_480X))) {
    if ((1 == stack_nargs_478X)) {
      v_503X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = v_503X;
      goto L21850;}
    else {
      arg0K0 = (*((long *) (((char *) (-3 + list_args_479X)))));
      goto L21850;}}
  else {
    arg0K0 = stack_nargs_478X;
    arg0K1 = list_args_479X;
    goto L22684;}}
 L21777: {
  SvalS = consumer_487X;
  arg0K0 = stack_nargs_478X;
  arg0K1 = list_args_479X;
  arg0K2 = list_arg_count_480X;
  goto L47520;}
 L25516: {
  loc_504X = arg1K0;
  arg_505X = arg1K1;
  if ((arg_505X < arg_top_491X)) {
    SstackS = ((SstackS) + (0 - (((stack_nargs_478X)<<2))));
    if ((count_489X < stack_nargs_478X)) {
      merged_arg0K0 = list_args_479X;
      merged_arg0K1 = (stack_nargs_478X - count_489X);
      pop_args_GlistSAgc_return_tag = 3;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_3:
      v_506X = pop_args_GlistSAgc0_return_value;
      arg0K0 = v_506X;
      goto L25484;}
    else {
      arg0K0 = stack_nargs_478X;
      arg0K1 = list_args_479X;
      goto L25466;}}
  else {
    *((long *) loc_504X) = (*((long *) arg_505X));
    arg1K0 = (loc_504X + -4);
    arg1K1 = (arg_505X + -4);
    goto L25516;}}
 L25346: {
  count_507X = arg0K0;
  bytes_used_508X = arg0K1;
  stack_nargs_509X = arg0K2;
  list_args_510X = arg0K3;
  list_arg_count_511X = arg0K4;
  if ((count_507X == (stack_nargs_509X + list_arg_count_511X))) {
    arg_top_512X = SstackS;
    cont_513X = ScontS;
    template_514X = *((long *) (cont_513X + 8));
    Scode_pointerS = (((char *) (*((long *) cont_513X))));
    StemplateS = template_514X;
    SenvS = (*((long *) (cont_513X + 12)));
    ScontS = (((char *) (*((long *) (cont_513X + 4)))));
    SstackS = (cont_513X + 16);
    arg1K0 = ((SstackS) + -4);
    arg1K1 = (arg_top_512X + (-4 + (((stack_nargs_509X)<<2))));
    goto L25389;}
  else {
    merged_arg0K0 = list_args_510X;
    merged_arg0K1 = stack_nargs_509X;
    pop_args_GlistSAgc_return_tag = 4;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_4:
    args_515X = pop_args_GlistSAgc0_return_value;
    push_exception_continuationB(4, 0);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = 1;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_515X;
    arg0K0 = 2;
    goto L22004;}}
 L22237: {
  stack_arg_count_516X = arg0K0;
  x_517X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_517X;
  x_518X = StemplateS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_518X;
  x_519X = ((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_519X;
  x_520X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_520X;
  n_521X = Senabled_interruptsS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_521X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (40 + (((stack_arg_count_516X)<<2)));
  x_522X = Scurrent_threadS;
  addr_523X = (((char *) (-3 + x_522X))) + 12;
  S48_WRITE_BARRIER(x_522X, addr_523X, 1);
  *((long *) addr_523X) = 1;
  tem_524X = Sinterrupt_templateS;
  StemplateS = tem_524X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_524X)))))))) + 3);
  code_pointer_525X = Scode_pointerS;
  template_526X = StemplateS;
  SstackS = ((SstackS) + -16);
  cont_527X = SstackS;
  *((long *) cont_527X) = (((long) code_pointer_525X));
  *((long *) (cont_527X + 8)) = template_526X;
  *((long *) (cont_527X + 12)) = (SenvS);
  *((long *) (cont_527X + 4)) = (((long) (ScontS)));
  ScontS = cont_527X;
  n_528X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L22401;}
 L46951: {
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L47118: {
  v_529X = arg0K0;
  merged_arg0K0 = v_529X;
  merged_arg0K1 = stack_arg_count_442X;
  merged_arg0K2 = list_args_443X;
  merged_arg0K3 = list_arg_count_444X;
  rest_list_setupAgc_return_tag = 1;
  goto rest_list_setupAgc;
 rest_list_setupAgc_return_1:
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((final_stack_arg_count_495X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_446X)<<2));
  arg0K0 = skip_476X;
  arg0K1 = (3 + final_stack_arg_count_495X);
  goto L47071;}
 L23956: {
  code_pointer_530X = arg1K0;
  switch ((*((unsigned char *) code_pointer_530X))) {
    case 0 : 
    case 24 : {
      push_exception_continuationB(14, 1);
      arg0K0 = 0;
      goto L22004;}
      break;
    case 1 : {
      count_531X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      x_532X = SenvS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_532X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (1034 + (((count_531X)<<10)));
      SenvS = (3 + (((long) ((SstackS) + 4))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 2 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 3 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 4 : {
      back_533X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_534X = SenvS;
      arg0K0 = env_534X;
      arg0K1 = back_533X;
      goto L30382;}
      break;
    case 5 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L22004;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23956;}}
      break;
    case 6 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L22004;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23956;}}
      break;
    case 7 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS)))))))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 2);
        arg0K0 = 0;
        goto L22004;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L23956;}}
      break;
    case 8 : {
      back_535X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_536X = SenvS;
      arg0K0 = env_536X;
      arg0K1 = back_535X;
      goto L25207;}
      break;
    case 9 : {
      back_537X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      env_538X = SenvS;
      arg0K0 = env_538X;
      arg0K1 = back_537X;
      goto L25145;}
      break;
    case 10 : {
      location_539X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_539X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        push_exception_continuationB(1, 3);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = location_539X;
        arg0K0 = 1;
        goto L22004;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23956;}}
      break;
    case 11 : {
      location_540X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      if ((273 == (*((long *) ((((char *) (-3 + location_540X))) + 4))))) {
        push_exception_continuationB(1, 3);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = location_540X;
        x_541X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_541X;
        arg0K0 = 2;
        goto L22004;}
      else {
        val_542X = SvalS;
        addr_543X = (((char *) (-3 + location_540X))) + 4;
        S48_WRITE_BARRIER(location_540X, addr_543X, val_542X);
        *((long *) addr_543X) = val_542X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23956;}}
      break;
    case 12 : {
      v_544X = *((unsigned char *) ((Scode_pointerS) + 3));
      if ((0 == v_544X)) {
        p_545X = SenvS;
        if ((3 == (3 & p_545X))) {
          if ((p_545X < (((long) (Sstack_beginS))))) {
            arg0K0 = 0;
            goto L25843;}
          else {
            if (((((long) (Sstack_endS))) < p_545X)) {
              arg0K0 = 0;
              goto L25843;}
            else {
              arg0K0 = ((Sstack_endS) - (Sstack_beginS));
              goto L25843;}}}
        else {
          arg0K0 = 0;
          goto L25843;}}
      else {
        s48_make_availableAgc(12);
        arg0K0 = (SvalS);
        arg0K1 = 0;
        goto L25856;}}
      break;
    case 13 : {
      count_546X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      s48_make_availableAgc((((((2 + count_546X) + (3 * count_546X)))<<2)));
      len_547X = 4 + (((count_546X)<<2));
      addr_548X = s48_allocate_small((4 + len_547X));
      *((long *) addr_548X) = (10 + (((len_547X)<<8)));
      vector_549X = 3 + (((long) (addr_548X + 4)));
      arg0K0 = 1;
      goto L24928;}
      break;
    case 14 : {
      total_count_550X = *((unsigned char *) ((Scode_pointerS) + 2));
      s48_make_availableAgc((4 + (((total_count_550X)<<2))));
      len_551X = ((total_count_550X)<<2);
      addr_552X = s48_allocate_small((4 + len_551X));
      *((long *) addr_552X) = (10 + (((len_551X)<<8)));
      new_env_553X = 3 + (((long) (addr_552X + 4)));
      v_554X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_554X)) {
        arg0K0 = 0;
        goto L26180;}
      else {
        value_555X = SvalS;
        addr_556X = ((char *) (-3 + new_env_553X));
        S48_WRITE_BARRIER(new_env_553X, addr_556X, value_555X);
        *((long *) addr_556X) = value_555X;
        arg0K0 = 1;
        goto L26180;}}
      break;
    case 15 : {
      total_count_557X = ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 3)));
      s48_make_availableAgc((4 + (((total_count_557X)<<2))));
      len_558X = ((total_count_557X)<<2);
      addr_559X = s48_allocate_small((4 + len_558X));
      *((long *) addr_559X) = (10 + (((len_558X)<<8)));
      new_env_560X = 3 + (((long) (addr_559X + 4)));
      v_561X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_561X)) {
        arg0K0 = 0;
        goto L26031;}
      else {
        value_562X = SvalS;
        addr_563X = ((char *) (-3 + new_env_560X));
        S48_WRITE_BARRIER(new_env_560X, addr_563X, value_562X);
        *((long *) addr_563X) = value_562X;
        arg0K0 = 1;
        goto L26031;}}
      break;
    case 16 : {
      x_564X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_564X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 17 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L22004;}
      else {
        x_565X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_565X;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23956;}}
      break;
    case 18 : {
      x_566X = SvalS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_566X;
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      if ((529 == (SvalS))) {
        push_exception_continuationB(0, 3);
        arg0K0 = 0;
        goto L22004;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23956;}}
      break;
    case 19 : {
      value_567X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SvalS = value_567X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 20 : {
      SvalS = (*((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 21 : {
      *((long *) ((SstackS) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 22 : {
      code_pointer_568X = (Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      template_569X = StemplateS;
      SstackS = ((SstackS) + -16);
      cont_570X = SstackS;
      *((long *) cont_570X) = (((long) code_pointer_568X));
      *((long *) (cont_570X + 8)) = template_569X;
      *((long *) (cont_570X + 12)) = (SenvS);
      *((long *) (cont_570X + 4)) = (((long) (ScontS)));
      ScontS = cont_570X;
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 23 : {
      s48_make_availableAgc((4 + (-4 & ((Sstack_endS) - (SstackS)))));
      if ((1 == (((long) (ScontS))))) {
        arg0K0 = 1;
        goto L48444;}
      else {
        v_571X = really_preserve_continuation(0, 2);
        arg0K0 = v_571X;
        goto L48444;}}
      break;
    case 25 : {
      stack_arg_count_572X = *((unsigned char *) ((Scode_pointerS) + 1));
      obj_573X = SvalS;
      if ((3 == (3 & obj_573X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_573X))) + -4))))>>2))))) {
          template_574X = *((long *) (((char *) (-3 + (SvalS)))));
          code_575X = *((long *) (((char *) (-3 + template_574X))));
          protocol_576X = *((unsigned char *) ((((char *) (-3 + code_575X))) + 1));
          if ((protocol_576X == stack_arg_count_572X)) {
            StemplateS = template_574X;
            SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
            Scode_pointerS = ((((char *) (-3 + code_575X))) + 2);
            if (((SstackS) < (Sstack_limitS))) {
              s48_copy_stack_into_heap();
              if (((SstackS) < (Sstack_limitS))) {
                ps_error("Couldn't get default procedure space (how can this happen?)", 0);
                goto L46804;}
              else {
                goto L46804;}}
            else {
              goto L46804;}}
          else {
            if (((127 & protocol_576X) == stack_arg_count_572X)) {
              proc_577X = SvalS;
              merged_arg0K0 = stack_arg_count_572X;
              move_args_above_contB_return_tag = 1;
              goto move_args_above_contB;
             move_args_above_contB_return_1:
              tag_578X = s48_call_native_procedure(proc_577X, 2);
              if ((tag_578X == 0)) {
                arg0K0 = (s48_Snative_protocolS);
                arg0K1 = 25;
                arg0K2 = 0;
                goto L21734;}
              else {
                if ((tag_578X == 1)) {
                  stack_arg_count_579X = s48_Snative_protocolS;
                  obj_580X = SvalS;
                  if ((3 == (3 & obj_580X))) {
                    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_580X))) + -4))))>>2))))) {
                      arg0K0 = stack_arg_count_579X;
                      arg0K1 = 25;
                      arg0K2 = 0;
                      goto L47051;}
                    else {
                      arg0K0 = 3;
                      arg0K1 = stack_arg_count_579X;
                      arg0K2 = 25;
                      arg0K3 = 0;
                      goto L22536;}}
                  else {
                    arg0K0 = 3;
                    arg0K1 = stack_arg_count_579X;
                    arg0K2 = 25;
                    arg0K3 = 0;
                    goto L22536;}}
                else {
                  ps_error("unexpected native return value", 1, tag_578X);
                  return v_581X;}}}
            else {
              arg0K0 = stack_arg_count_572X;
              arg0K1 = 25;
              arg0K2 = 0;
              goto L47051;}}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_572X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22536;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_572X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22536;}}
      break;
    case 26 : {
      stack_arg_count_582X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      obj_583X = SvalS;
      if ((3 == (3 & obj_583X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_583X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_582X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L47051;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_582X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22536;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_582X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22536;}}
      break;
    case 27 : {
      list_args_584X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      merged_arg0K0 = list_args_584X;
      okay_argument_list_return_tag = 0;
      goto okay_argument_list;
     okay_argument_list_return_0:
      okayP_585X = okay_argument_list0_return_value;
      length_586X = okay_argument_list1_return_value;
      if (okayP_585X) {
        arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
        arg0K1 = list_args_584X;
        arg0K2 = length_586X;
        goto L47520;}
      else {
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_args_584X;
        merged_arg0K0 = 25;
        merged_arg0K1 = (1 + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        pop_args_GlistSAgc_return_tag = 5;
        goto pop_args_GlistSAgc;
       pop_args_GlistSAgc_return_5:
        args_587X = pop_args_GlistSAgc0_return_value;
        push_exception_continuationB(5, 0);
        x_588X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_588X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = args_587X;
        arg0K0 = 2;
        goto L22004;}}
      break;
    case 28 : {
      SstackS = ((SstackS) + 4);
      p_589X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_590X = ((p_589X)>>2);
      proc_591X = *((long *) ((SstackS) + (((stack_nargs_590X)<<2))));
      *((long *) ((SstackS) + (((stack_nargs_590X)<<2)))) = 1;
      rest_list_592X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((25 == rest_list_592X)) {
        v_593X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_593X;
        arg0K1 = (-2 + stack_nargs_590X);
        goto L15334;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_592X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_592X)))));
          arg0K1 = (-1 + stack_nargs_590X);
          goto L15334;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_592X))) + 4)))))) + 4)));
          arg0K1 = rest_list_592X;
          goto L15379;}}}
      break;
    case 29 : {
      cont_594X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & cont_594X))) {
        if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_594X))) + -4))))>>2))))) {
          merged_arg0K0 = cont_594X;
          merged_arg0K1 = 0;
          copy_continuation_from_heapB_return_tag = 1;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_1:
          goto L23120;}
        else {
          goto L23129;}}
      else {
        goto L23129;}}
      break;
    case 30 : {
      goto L47678;}
      break;
    case 31 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L21734;}
      break;
    case 32 : {
      p_595X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      p_596X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_597X = ((p_596X)>>2);
      rest_list_598X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = stack_nargs_597X;
      arg0K1 = rest_list_598X;
      arg0K2 = ((((p_595X)>>2)) - stack_nargs_597X);
      goto L21734;}
      break;
    case 33 : {
      tem_599X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      StemplateS = tem_599X;
      Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_599X))))))));
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 34 : {
      code_600X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))))))));
      if ((0 == (*((unsigned char *) ((((char *) (-3 + code_600X))) + 1))))) {
        if (((SstackS) < (Sstack_limitS))) {
          s48_copy_stack_into_heap();
          if (((SstackS) < (Sstack_limitS))) {
            ps_error("Couldn't get default procedure space (how can this happen?)", 0);
            goto L46581;}
          else {
            goto L46581;}}
        else {
          goto L46581;}}
      else {
        v_601X = *((unsigned char *) ((((char *) (-3 + code_600X))) + 1));
        if ((67 == v_601X)) {
          if ((0 == (*((unsigned char *) ((((char *) (-3 + code_600X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_600X))) + -4))))>>8)))))))) {
            index_602X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_600X))) + -4))))>>8));
            space_603X = ((((*((unsigned char *) ((((char *) (-3 + code_600X))) + index_602X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_600X))) + (1 + index_602X))));
            if ((space_603X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
              goto L46581;}
            else {
              s48_copy_stack_into_heap();
              if ((space_603X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
                goto L46581;}
              else {
                ps_error("VM's stack is too small (how can this happen?)", 0);
                goto L46581;}}}
          else {
            goto L46624;}}
        else {
          goto L46624;}}}
      break;
    case 35 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L23956;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L23956;}}
      break;
    case 36 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 37 : {
      if ((0 == (3 & (SvalS)))) {
        p_604X = SvalS;
        max_605X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_606X = ((p_604X)>>2);
        if ((val_606X < 0)) {
          goto L24260;}
        else {
          if ((val_606X < max_605X)) {
            index_607X = 1 + (((val_606X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_607X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_607X)))));
            goto L24262;}
          else {
            goto L24260;}}}
      else {
        push_exception_continuationB(5, 0);
        x_608X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_608X;
        arg0K0 = 1;
        goto L22004;}}
      break;
    case 38 : {
      stack_nargs_609X = (((*((long *) (SstackS))))>>2);
      if ((0 == stack_nargs_609X)) {
        arg0_610X = *((long *) ((SstackS) + 8));
        rest_list_611X = *((long *) ((SstackS) + 4));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg0_610X;
        SvalS = (*((long *) (((char *) (-3 + rest_list_611X)))));
        goto L24698;}
      else {
        arg1_612X = *((long *) ((SstackS) + (((stack_nargs_609X)<<2))));
        arg0_613X = *((long *) ((SstackS) + (4 + (((stack_nargs_609X)<<2)))));
        *((long *) ((SstackS) + (4 + (((stack_nargs_609X)<<2))))) = 1;
        *((long *) (SstackS)) = (-4 + (((stack_nargs_609X)<<2)));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg0_613X;
        SvalS = arg1_612X;
        goto L24698;}}
      break;
    case 39 : {
      stack_nargs_614X = (((*((long *) (SstackS))))>>2);
      if ((stack_nargs_614X == 0)) {
        rest_list_615X = *((long *) ((SstackS) + 4));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_615X))) + 4))))) {
          arg0K0 = 1;
          goto L24158;}
        else {
          *((long *) ((SstackS) + 4)) = (*((long *) ((((char *) (-3 + rest_list_615X))) + 4)));
          *((long *) ((SstackS) + 8)) = (SvalS);
          arg0K0 = -2;
          goto L24158;}}
      else {
        if ((stack_nargs_614X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 4))))) {
            arg0K0 = 1;
            goto L24158;}
          else {
            *((long *) (SstackS)) = 0;
            *((long *) ((SstackS) + 8)) = (SvalS);
            arg0K0 = -2;
            goto L24158;}}
        else {
          *((long *) ((SstackS) + (4 + (((stack_nargs_614X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L24158;}}}
      break;
    case 40 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23956;}
      else {
        x_616X = (((*((long *) (SstackS))))>>2);
        if ((x_616X == 0)) {
          rest_list_617X = *((long *) ((SstackS) + 4));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_617X))) + 4))))) {
            arg0K0 = 1;
            goto L24589;}
          else {
            *((long *) ((SstackS) + 4)) = (*((long *) ((((char *) (-3 + rest_list_617X))) + 4)));
            *((long *) ((SstackS) + 8)) = (*((long *) (((char *) (-3 + rest_list_617X)))));
            arg0K0 = -2;
            goto L24589;}}
        else {
          if ((x_616X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 4))))) {
              arg0K0 = 1;
              goto L24589;}
            else {
              *((long *) (SstackS)) = 0;
              arg0K0 = -2;
              goto L24589;}}
          else {
            arg0K0 = -2;
            goto L24589;}}}}
      break;
    case 41 : {
      arg2_618X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_619X = SvalS;
      if ((arg2_618X == x_619X)) {
        arg0K0 = 5;
        goto L48501;}
      else {
        arg0K0 = 1;
        goto L48501;}}
      break;
    case 42 : {
      x_620X = SvalS;
      if ((0 == (3 & x_620X))) {
        arg0K0 = 5;
        goto L48513;}
      else {
        if ((3 == (3 & x_620X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_620X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L48513;}
          else {
            goto L13742;}}
        else {
          goto L13742;}}}
      break;
    case 43 : {
      n_621X = SvalS;
      if ((0 == (3 & n_621X))) {
        goto L38674;}
      else {
        if ((3 == (3 & n_621X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_621X))) + -4))))>>2))))) {
            goto L38674;}
          else {
            goto L38675;}}
        else {
          goto L38675;}}}
      break;
    case 44 : {
      n_622X = SvalS;
      if ((0 == (3 & n_622X))) {
        goto L38831;}
      else {
        if ((3 == (3 & n_622X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_622X))) + -4))))>>2))))) {
            goto L38831;}
          else {
            goto L38806;}}
        else {
          goto L38806;}}}
      break;
    case 45 : {
      n_623X = SvalS;
      if ((0 == (3 & n_623X))) {
        goto L39030;}
      else {
        if ((3 == (3 & n_623X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_623X))) + -4))))>>2))))) {
            goto L39030;}
          else {
            goto L39005;}}
        else {
          goto L39005;}}}
      break;
    case 46 : {
      n_624X = SvalS;
      if ((0 == (3 & n_624X))) {
        goto L39229;}
      else {
        if ((3 == (3 & n_624X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_624X))) + -4))))>>2))))) {
            goto L39229;}
          else {
            goto L39204;}}
        else {
          goto L39204;}}}
      break;
    case 47 : {
      n_625X = SvalS;
      if ((0 == (3 & n_625X))) {
        goto L31914;}
      else {
        if ((3 == (3 & n_625X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_625X))) + -4))))>>2))))) {
            goto L31914;}
          else {
            goto L31915;}}
        else {
          goto L31915;}}}
      break;
    case 48 : {
      x_626X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_626X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 49 : {
      x_627X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_627X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 50 : {
      arg2_628X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_629X = SvalS;
      if ((0 == (3 & (arg2_628X | y_629X)))) {
        s48_make_availableAgc(16);
        x_630X = (((arg2_628X)>>2)) + (((y_629X)>>2));
        if ((536870911 < x_630X)) {
          goto L32040;}
        else {
          if ((x_630X < -536870912)) {
            goto L32040;}
          else {
            arg0K0 = (((x_630X)<<2));
            goto L31986;}}}
      else {
        if ((0 == (3 & arg2_628X))) {
          goto L31992;}
        else {
          if ((3 == (3 & arg2_628X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_628X))) + -4))))>>2))))) {
              goto L31992;}
            else {
              goto L32001;}}
          else {
            goto L32001;}}}}
      break;
    case 51 : {
      arg2_631X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_632X = SvalS;
      if ((0 == (3 & (arg2_631X | y_632X)))) {
        a_633X = ((arg2_631X)>>2);
        b_634X = ((y_632X)>>2);
        if ((a_633X < 0)) {
          arg0K0 = (0 - a_633X);
          goto L8204;}
        else {
          arg0K0 = a_633X;
          goto L8204;}}
      else {
        if ((0 == (3 & arg2_631X))) {
          goto L40887;}
        else {
          if ((3 == (3 & arg2_631X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_631X))) + -4))))>>2))))) {
              goto L40887;}
            else {
              goto L40896;}}
          else {
            goto L40896;}}}}
      break;
    case 52 : {
      arg2_635X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_636X = SvalS;
      if ((0 == (3 & (arg2_635X | y_636X)))) {
        s48_make_availableAgc(16);
        x_637X = (((arg2_635X)>>2)) - (((y_636X)>>2));
        if ((536870911 < x_637X)) {
          goto L32329;}
        else {
          if ((x_637X < -536870912)) {
            goto L32329;}
          else {
            arg0K0 = (((x_637X)<<2));
            goto L32275;}}}
      else {
        if ((0 == (3 & arg2_635X))) {
          goto L32281;}
        else {
          if ((3 == (3 & arg2_635X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_635X))) + -4))))>>2))))) {
              goto L32281;}
            else {
              goto L32290;}}
          else {
            goto L32290;}}}}
      break;
    case 53 : {
      arg2_638X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_639X = SvalS;
      if ((0 == y_639X)) {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_638X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_639X;
        arg0K0 = 2;
        goto L22004;}
      else {
        if ((0 == (3 & (arg2_638X | y_639X)))) {
          if ((0 == y_639X)) {
            push_exception_continuationB(5, 1);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = arg2_638X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_639X;
            arg0K0 = 2;
            goto L22004;}
          else {
            a_640X = ((arg2_638X)>>2);
            b_641X = ((y_639X)>>2);
            if ((a_640X < 0)) {
              arg0K0 = (0 - a_640X);
              goto L8462;}
            else {
              arg0K0 = a_640X;
              goto L8462;}}}
        else {
          if ((0 == (3 & arg2_638X))) {
            goto L41140;}
          else {
            if ((3 == (3 & arg2_638X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_638X))) + -4))))>>2))))) {
                goto L41140;}
              else {
                goto L41173;}}
            else {
              goto L41173;}}}}}
      break;
    case 54 : {
      arg2_642X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_643X = SvalS;
      if ((0 == (3 & (arg2_642X | y_643X)))) {
        if ((arg2_642X == y_643X)) {
          arg0K0 = 5;
          goto L32560;}
        else {
          arg0K0 = 1;
          goto L32560;}}
      else {
        if ((0 == (3 & arg2_642X))) {
          goto L32566;}
        else {
          if ((3 == (3 & arg2_642X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_642X))) + -4))))>>2))))) {
              goto L32566;}
            else {
              goto L32577;}}
          else {
            goto L32577;}}}}
      break;
    case 55 : {
      arg2_644X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_645X = SvalS;
      if ((0 == (3 & (arg2_644X | y_645X)))) {
        if ((arg2_644X < y_645X)) {
          arg0K0 = 5;
          goto L32820;}
        else {
          arg0K0 = 1;
          goto L32820;}}
      else {
        if ((0 == (3 & arg2_644X))) {
          goto L32826;}
        else {
          if ((3 == (3 & arg2_644X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_644X))) + -4))))>>2))))) {
              goto L32826;}
            else {
              goto L32837;}}
          else {
            goto L32837;}}}}
      break;
    case 56 : {
      arg2_646X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_647X = SvalS;
      if ((0 == (3 & (arg2_646X | y_647X)))) {
        if ((y_647X < arg2_646X)) {
          arg0K0 = 5;
          goto L33143;}
        else {
          arg0K0 = 1;
          goto L33143;}}
      else {
        if ((0 == (3 & arg2_646X))) {
          goto L33149;}
        else {
          if ((3 == (3 & arg2_646X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_646X))) + -4))))>>2))))) {
              goto L33149;}
            else {
              goto L33160;}}
          else {
            goto L33160;}}}}
      break;
    case 57 : {
      arg2_648X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_649X = SvalS;
      if ((0 == (3 & (arg2_648X | y_649X)))) {
        if ((y_649X < arg2_648X)) {
          arg0K0 = 1;
          goto L33466;}
        else {
          arg0K0 = 5;
          goto L33466;}}
      else {
        if ((0 == (3 & arg2_648X))) {
          goto L33472;}
        else {
          if ((3 == (3 & arg2_648X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_648X))) + -4))))>>2))))) {
              goto L33472;}
            else {
              goto L33483;}}
          else {
            goto L33483;}}}}
      break;
    case 58 : {
      arg2_650X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_651X = SvalS;
      if ((0 == (3 & (arg2_650X | y_651X)))) {
        if ((arg2_650X < y_651X)) {
          arg0K0 = 1;
          goto L33760;}
        else {
          arg0K0 = 5;
          goto L33760;}}
      else {
        if ((0 == (3 & arg2_650X))) {
          goto L33766;}
        else {
          if ((3 == (3 & arg2_650X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_650X))) + -4))))>>2))))) {
              goto L33766;}
            else {
              goto L33777;}}
          else {
            goto L33777;}}}}
      break;
    case 59 : {
      arg2_652X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_653X = SvalS;
      if ((0 == y_653X)) {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_652X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_653X;
        arg0K0 = 2;
        goto L22004;}
      else {
        if ((0 == (3 & (arg2_652X | y_653X)))) {
          if ((0 == y_653X)) {
            arg0K0 = arg2_652X;
            arg0K1 = y_653X;
            goto L19496;}
          else {
            a_654X = ((arg2_652X)>>2);
            b_655X = ((y_653X)>>2);
            if ((a_654X < 0)) {
              arg0K0 = (0 - a_654X);
              goto L9105;}
            else {
              arg0K0 = a_654X;
              goto L9105;}}}
        else {
          if ((0 == (3 & arg2_652X))) {
            goto L34066;}
          else {
            if ((3 == (3 & arg2_652X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_652X))) + -4))))>>2))))) {
                goto L34066;}
              else {
                goto L34075;}}
            else {
              goto L34075;}}}}}
      break;
    case 60 : {
      arg2_656X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_657X = SvalS;
      if ((0 == y_657X)) {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_656X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_657X;
        arg0K0 = 2;
        goto L22004;}
      else {
        if ((0 == (3 & (arg2_656X | y_657X)))) {
          if ((0 == y_657X)) {
            arg0K0 = arg2_656X;
            arg0K1 = y_657X;
            goto L19405;}
          else {
            a_658X = ((arg2_656X)>>2);
            if ((a_658X < 0)) {
              arg0K0 = (0 - a_658X);
              goto L34280;}
            else {
              arg0K0 = a_658X;
              goto L34280;}}}
        else {
          if ((0 == (3 & arg2_656X))) {
            goto L34242;}
          else {
            if ((3 == (3 & arg2_656X))) {
              if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_656X))) + -4))))>>2))))) {
                goto L34242;}
              else {
                goto L34251;}}
            else {
              goto L34251;}}}}}
      break;
    case 61 : {
      n_659X = SvalS;
      if ((0 == (3 & n_659X))) {
        goto L34429;}
      else {
        if ((3 == (3 & n_659X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_659X))) + -4))))>>2))))) {
            goto L34429;}
          else {
            goto L34430;}}
        else {
          goto L34430;}}}
      break;
    case 62 : {
      n_660X = SvalS;
      if ((0 == (3 & n_660X))) {
        goto L34484;}
      else {
        if ((3 == (3 & n_660X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_660X))) + -4))))>>2))))) {
            goto L34484;}
          else {
            goto L34485;}}
        else {
          goto L34485;}}}
      break;
    case 63 : {
      n_661X = SvalS;
      if ((0 == (3 & n_661X))) {
        goto L34539;}
      else {
        if ((3 == (3 & n_661X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_661X))) + -4))))>>2))))) {
            goto L34539;}
          else {
            goto L34542;}}
        else {
          goto L34542;}}}
      break;
    case 64 : {
      n_662X = SvalS;
      if ((0 == (3 & n_662X))) {
        goto L34599;}
      else {
        if ((3 == (3 & n_662X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_662X))) + -4))))>>2))))) {
            goto L34599;}
          else {
            goto L34600;}}
        else {
          goto L34600;}}}
      break;
    case 65 : {
      n_663X = SvalS;
      if ((0 == (3 & n_663X))) {
        goto L34654;}
      else {
        if ((3 == (3 & n_663X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + n_663X))) + -4))))>>2))))) {
            goto L34654;}
          else {
            goto L34657;}}
        else {
          goto L34657;}}}
      break;
    case 66 : {
      x_664X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_664X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 67 : {
      x_665X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_665X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 68 : {
      x_666X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_666X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 69 : {
      x_667X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_667X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 70 : {
      x_668X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_668X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 71 : {
      x_669X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_669X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 72 : {
      x_670X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_670X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 73 : {
      x_671X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_671X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 74 : {
      x_672X = SvalS;
      if ((0 == (3 & x_672X))) {
        if ((x_672X < 0)) {
          goto L48689;}
        else {
          goto L48695;}}
      else {
        v_673X = s48_bignum_test((((char *) (-3 + x_672X))));
        if ((-1 == v_673X)) {
          goto L48689;}
        else {
          goto L48695;}}}
      break;
    case 75 : {
      x_674X = SvalS;
      if ((0 == (3 & x_674X))) {
        goto L22787;}
      else {
        if ((3 == (3 & x_674X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_674X))) + -4))))>>2))))) {
            goto L22787;}
          else {
            goto L22790;}}
        else {
          goto L22790;}}}
      break;
    case 76 : {
      x_675X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_675X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 77 : {
      arg2_676X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_677X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_676X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_677X;
      arg0K0 = 2;
      goto L22004;}
      break;
    case 78 : {
      arg2_678X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_679X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_678X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_679X;
      arg0K0 = 2;
      goto L22004;}
      break;
    case 79 : {
      arg2_680X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_681X = SvalS;
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_680X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_681X;
      arg0K0 = 2;
      goto L22004;}
      break;
    case 80 : {
      x_682X = SvalS;
      if ((0 == (3 & x_682X))) {
        SvalS = (~ (3 | x_682X));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23956;}
      else {
        if ((0 == (3 & x_682X))) {
          goto L34808;}
        else {
          if ((3 == (3 & x_682X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_682X))) + -4))))>>2))))) {
              goto L34808;}
            else {
              goto L34811;}}
          else {
            goto L34811;}}}}
      break;
    case 81 : {
      x_683X = SvalS;
      if ((0 == (3 & x_683X))) {
        x_684X = ((x_683X)>>2);
        if ((x_684X < 0)) {
          arg0K0 = (~ x_684X);
          goto L34899;}
        else {
          arg0K0 = x_684X;
          goto L34899;}}
      else {
        if ((0 == (3 & x_683X))) {
          goto L34882;}
        else {
          if ((3 == (3 & x_683X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + x_683X))) + -4))))>>2))))) {
              goto L34882;}
            else {
              goto L34885;}}
          else {
            goto L34885;}}}}
      break;
    case 82 : {
      arg2_685X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_686X = SvalS;
      if ((0 == (3 & (arg2_685X | y_686X)))) {
        SvalS = (arg2_685X & y_686X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23956;}
      else {
        if ((0 == (3 & arg2_685X))) {
          goto L34984;}
        else {
          if ((3 == (3 & arg2_685X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_685X))) + -4))))>>2))))) {
              goto L34984;}
            else {
              goto L34993;}}
          else {
            goto L34993;}}}}
      break;
    case 83 : {
      arg2_687X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_688X = SvalS;
      if ((0 == (3 & (arg2_687X | y_688X)))) {
        SvalS = (arg2_687X | y_688X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23956;}
      else {
        if ((0 == (3 & arg2_687X))) {
          goto L35147;}
        else {
          if ((3 == (3 & arg2_687X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_687X))) + -4))))>>2))))) {
              goto L35147;}
            else {
              goto L35156;}}
          else {
            goto L35156;}}}}
      break;
    case 84 : {
      arg2_689X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_690X = SvalS;
      if ((0 == (3 & (arg2_689X | y_690X)))) {
        SvalS = (arg2_689X ^ y_690X);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23956;}
      else {
        if ((0 == (3 & arg2_689X))) {
          goto L35310;}
        else {
          if ((3 == (3 & arg2_689X))) {
            if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_689X))) + -4))))>>2))))) {
              goto L35310;}
            else {
              goto L35319;}}
          else {
            goto L35319;}}}}
      break;
    case 85 : {
      arg2_691X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      y_692X = SvalS;
      if ((3 == (3 & y_692X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_692X))) + -4))))>>2))))) {
          v_693X = s48_bignum_test((((char *) (-3 + y_692X))));
          if ((1 == v_693X)) {
            push_exception_continuationB(6, 1);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = arg2_691X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = y_692X;
            arg0K0 = 2;
            goto L22004;}
          else {
            if ((0 == (3 & arg2_691X))) {
              if ((arg2_691X < 0)) {
                arg0K0 = -4;
                goto L31694;}
              else {
                arg0K0 = 0;
                goto L31694;}}
            else {
              if ((3 == (3 & arg2_691X))) {
                if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_691X))) + -4))))>>2))))) {
                  v_694X = s48_bignum_test((((char *) (-3 + arg2_691X))));
                  if ((1 == v_694X)) {
                    arg0K0 = 0;
                    goto L31710;}
                  else {
                    arg0K0 = -4;
                    goto L31710;}}
                else {
                  goto L31711;}}
              else {
                goto L31711;}}}}
        else {
          goto L41466;}}
      else {
        goto L41466;}}
      break;
    case 86 : {
      x_695X = SvalS;
      if ((9 == (255 & x_695X))) {
        arg0K0 = 5;
        goto L48794;}
      else {
        arg0K0 = 1;
        goto L48794;}}
      break;
    case 87 : {
      arg2_696X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((9 == (255 & arg2_696X))) {
        if ((9 == (255 & (SvalS)))) {
          x_697X = SvalS;
          if ((arg2_696X == x_697X)) {
            arg0K0 = 5;
            goto L38447;}
          else {
            arg0K0 = 1;
            goto L38447;}}
        else {
          goto L38418;}}
      else {
        goto L38418;}}
      break;
    case 88 : {
      arg2_698X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((9 == (255 & arg2_698X))) {
        if ((9 == (255 & (SvalS)))) {
          x_699X = SvalS;
          if ((arg2_698X < x_699X)) {
            arg0K0 = 5;
            goto L38355;}
          else {
            arg0K0 = 1;
            goto L38355;}}
        else {
          goto L38326;}}
      else {
        goto L38326;}}
      break;
    case 89 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23956;}
      else {
        push_exception_continuationB(5, 1);
        x_700X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_700X;
        arg0K0 = 1;
        goto L22004;}}
      break;
    case 90 : {
      if ((0 == (3 & (SvalS)))) {
        x_701X = (((SvalS))>>2);
        if ((255 < x_701X)) {
          goto L42680;}
        else {
          if ((x_701X < 0)) {
            goto L42680;}
          else {
            SvalS = (9 + (((((x_701X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23956;}}}
      else {
        push_exception_continuationB(5, 1);
        x_702X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_702X;
        arg0K0 = 1;
        goto L22004;}}
      break;
    case 91 : {
      x_703X = SvalS;
      if ((21 == x_703X)) {
        arg0K0 = 5;
        goto L48825;}
      else {
        arg0K0 = 1;
        goto L48825;}}
      break;
    case 92 : {
      x_704X = SvalS;
      type_705X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_704X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_704X))) + -4))))>>2))) == type_705X)) {
          arg0K0 = 5;
          goto L48839;}
        else {
          arg0K0 = 1;
          goto L48839;}}
      else {
        arg0K0 = 1;
        goto L48839;}}
      break;
    case 93 : {
      stob_706X = SvalS;
      type_707X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_706X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_706X))) + -4))))>>2))) == type_707X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + stob_706X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          goto L26368;}}
      else {
        goto L26368;}}
      break;
    case 94 : {
      len_708X = *((unsigned char *) ((Scode_pointerS) + 1));
      s48_make_availableAgc((4 + (((len_708X)<<2))));
      type_709X = *((unsigned char *) ((Scode_pointerS) + 2));
      len_710X = ((len_708X)<<2);
      addr_711X = s48_allocate_small((4 + len_710X));
      *((long *) addr_711X) = (2 + (((((((len_710X)<<6)) + type_709X))<<2)));
      new_712X = 3 + (((long) (addr_711X + 4)));
      if ((len_708X < 1)) {
        goto L26473;}
      else {
        *((long *) ((((char *) (-3 + new_712X))) + (-4 + (((len_708X)<<2))))) = (SvalS);
        arg0K0 = (-2 + len_708X);
        goto L26457;}}
      break;
    case 95 : {
      p_713X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      len_714X = ((p_713X)>>2);
      s48_make_availableAgc((4 + (((len_714X)<<2))));
      type_715X = *((unsigned char *) ((Scode_pointerS) + 1));
      len_716X = ((len_714X)<<2);
      addr_717X = s48_allocate_small((4 + len_716X));
      *((long *) addr_717X) = (2 + (((((((len_716X)<<6)) + type_715X))<<2)));
      new_718X = 3 + (((long) (addr_717X + 4)));
      p_719X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_720X = ((p_719X)>>2);
      rest_list_721X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg0K0 = (-1 + stack_nargs_720X);
      goto L26591;}
      break;
    case 96 : {
      stob_722X = SvalS;
      offset_723X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_724X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_722X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_722X))) + -4))))>>2))) == type_724X)) {
          SvalS = (*((long *) ((((char *) (-3 + stob_722X))) + (((offset_723X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          goto L26759;}}
      else {
        goto L26759;}}
      break;
    case 97 : {
      arg2_725X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      value_726X = SvalS;
      offset_727X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_728X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & arg2_725X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + arg2_725X))) + -4))))>>2))) == type_728X)) {
          if ((3 == (3 & arg2_725X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + arg2_725X))) + -4)))))) {
              v_729X = *((unsigned char *) ((Scode_pointerS) + 3));
              if ((0 == v_729X)) {
                goto L26876;}
              else {
                if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                  goto L26876;}
                else {
                  merged_arg0K0 = arg2_725X;
                  merged_arg0K1 = (((offset_727X)<<2));
                  merged_arg0K2 = value_726X;
                  proposal_d_write_return_tag = 0;
                  goto proposal_d_write;
                 proposal_d_write_return_0:
                  goto L26885;}}}
            else {
              goto L26886;}}
          else {
            goto L26886;}}
        else {
          goto L26886;}}
      else {
        goto L26886;}}
      break;
    case 98 : {
      arg2_730X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      init_731X = SvalS;
      type_732X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_730X))) {
        len_733X = ((arg2_730X)>>2);
        if ((len_733X < 0)) {
          goto L27103;}
        else {
          if ((4194304 < len_733X)) {
            goto L27103;}
          else {
            Stemp0S = init_731X;
            len_734X = ((len_733X)<<2);
            addr_735X = s48_allocate_tracedAgc((4 + len_734X));
            if ((addr_735X == NULL)) {
              arg0K0 = 1;
              goto L27120;}
            else {
              *((long *) addr_735X) = (2 + (((((((len_734X)<<6)) + type_732X))<<2)));
              arg0K0 = (3 + (((long) (addr_735X + 4))));
              goto L27120;}}}}
      else {
        push_exception_continuationB(5, 2);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((type_732X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_730X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = init_731X;
        arg0K0 = 3;
        goto L22004;}}
      break;
    case 99 : {
      arg2_736X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      index_737X = SvalS;
      type_738X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & index_737X))) {
        if ((3 == (3 & arg2_736X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg2_736X))) + -4))))>>2))) == type_738X)) {
            len_739X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_736X))) + -4))))>>8))))>>2);
            index_740X = ((index_737X)>>2);
            if ((index_740X < 0)) {
              goto L27393;}
            else {
              if ((index_740X < len_739X)) {
                v_741X = *((unsigned char *) ((Scode_pointerS) + 2));
                if ((0 == v_741X)) {
                  goto L27383;}
                else {
                  if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                    goto L27383;}
                  else {
                    merged_arg0K0 = arg2_736X;
                    merged_arg0K1 = index_737X;
                    proposal_d_read_return_tag = 0;
                    goto proposal_d_read;
                   proposal_d_read_return_0:
                    v_742X = proposal_d_read0_return_value;
                    arg0K0 = v_742X;
                    goto L27392;}}}
              else {
                goto L27393;}}}
          else {
            goto L27348;}}
        else {
          goto L27348;}}
      else {
        goto L27348;}}
      break;
    case 100 : {
      arg2_743X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_744X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      value_745X = SvalS;
      type_746X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_743X))) {
        if ((3 == (3 & arg3_744X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg3_744X))) + -4))))>>2))) == type_746X)) {
            if ((3 == (3 & arg3_744X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_744X))) + -4)))))) {
                len_747X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_744X))) + -4))))>>8))))>>2);
                index_748X = ((arg2_743X)>>2);
                if ((index_748X < 0)) {
                  goto L27680;}
                else {
                  if ((index_748X < len_747X)) {
                    v_749X = *((unsigned char *) ((Scode_pointerS) + 2));
                    if ((0 == v_749X)) {
                      goto L27670;}
                    else {
                      if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                        goto L27670;}
                      else {
                        merged_arg0K0 = arg3_744X;
                        merged_arg0K1 = arg2_743X;
                        merged_arg0K2 = value_745X;
                        proposal_d_write_return_tag = 1;
                        goto proposal_d_write;
                       proposal_d_write_return_1:
                        goto L27679;}}}
                  else {
                    goto L27680;}}}
              else {
                goto L27633;}}
            else {
              goto L27633;}}
          else {
            goto L27633;}}
        else {
          goto L27633;}}
      else {
        goto L27633;}}
      break;
    case 101 : {
      arg2_750X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (arg2_750X | (SvalS))))) {
        len_751X = ((arg2_750X)>>2);
        init_752X = (((SvalS))>>2);
        if ((len_751X < 0)) {
          goto L35531;}
        else {
          if ((4194304 < ((((3 + len_751X))>>2)))) {
            goto L35531;}
          else {
            addr_753X = s48_allocate_untracedAgc((4 + len_751X));
            if ((addr_753X == NULL)) {
              arg0K0 = 1;
              goto L35544;}
            else {
              *((long *) addr_753X) = (74 + (((len_751X)<<8)));
              arg0K0 = (3 + (((long) (addr_753X + 4))));
              goto L35544;}}}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_750X;
        x_754X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_754X;
        arg0K0 = 2;
        goto L22004;}}
      break;
    case 102 : {
      obj_755X = SvalS;
      if ((3 == (3 & obj_755X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + obj_755X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          goto L46170;}}
      else {
        goto L46170;}}
      break;
    case 103 : {
      arg2_756X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_756X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_756X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_757X = (((SvalS))>>2);
            len_758X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_756X))) + -4))))>>8);
            if ((index_757X < 0)) {
              goto L42788;}
            else {
              if ((index_757X < len_758X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_756X))) + index_757X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23956;}
              else {
                goto L42788;}}}
          else {
            goto L45665;}}
        else {
          goto L45665;}}
      else {
        goto L45665;}}
      break;
    case 104 : {
      arg2_759X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_760X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_760X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_760X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_759X | (SvalS))))) {
            index_761X = ((arg2_759X)>>2);
            Kchar_762X = (((SvalS))>>2);
            if ((3 == (3 & arg3_760X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_760X))) + -4)))))) {
                len_763X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_760X))) + -4))))>>8);
                if ((index_761X < 0)) {
                  goto L40109;}
                else {
                  if ((index_761X < len_763X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_760X))) + index_761X)) = Kchar_762X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L23956;}
                  else {
                    goto L40109;}}}
              else {
                goto L40088;}}
            else {
              goto L40088;}}
          else {
            goto L44397;}}
        else {
          goto L44397;}}
      else {
        goto L44397;}}
      break;
    case 105 : {
      arg2_764X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_764X))) {
        if ((9 == (255 & (SvalS)))) {
          len_765X = ((arg2_764X)>>2);
          init_766X = ((((SvalS))>>8));
          if ((len_765X < 0)) {
            goto L35711;}
          else {
            if ((4194304 < ((((4 + len_765X))>>2)))) {
              goto L35711;}
            else {
              len_767X = 1 + len_765X;
              addr_768X = s48_allocate_untracedAgc((4 + len_767X));
              if ((addr_768X == NULL)) {
                arg0K0 = 1;
                goto L35724;}
              else {
                *((long *) addr_768X) = (70 + (((len_767X)<<8)));
                arg0K0 = (3 + (((long) (addr_768X + 4))));
                goto L35724;}}}}
        else {
          goto L42869;}}
      else {
        goto L42869;}}
      break;
    case 106 : {
      obj_769X = SvalS;
      if ((3 == (3 & obj_769X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_769X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          goto L46220;}}
      else {
        goto L46220;}}
      break;
    case 107 : {
      arg2_770X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_770X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_770X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_771X = (((SvalS))>>2);
            len_772X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_770X))) + -4))))>>8));
            if ((index_771X < 0)) {
              goto L42934;}
            else {
              if ((index_771X < len_772X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_770X))) + index_771X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23956;}
              else {
                goto L42934;}}}
          else {
            goto L45763;}}
        else {
          goto L45763;}}
      else {
        goto L45763;}}
      break;
    case 108 : {
      arg2_773X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_774X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_774X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_774X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_773X))) {
            if ((9 == (255 & (SvalS)))) {
              index_775X = ((arg2_773X)>>2);
              Kchar_776X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_774X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_774X))) + -4)))))) {
                  len_777X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_774X))) + -4))))>>8));
                  if ((index_775X < 0)) {
                    goto L40264;}
                  else {
                    if ((index_775X < len_777X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_774X))) + index_775X)) = (Kchar_776X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L23956;}
                    else {
                      goto L40264;}}}
                else {
                  goto L40243;}}
              else {
                goto L40243;}}
            else {
              goto L44527;}}
          else {
            goto L44527;}}
        else {
          goto L44527;}}
      else {
        goto L44527;}}
      break;
    case 109 : {
      s48_make_availableAgc(12);
      obj_778X = SvalS;
      if ((3 == (3 & obj_778X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_778X))) + -4))))>>2))))) {
          table_779X = Sthe_symbol_tableS;
          string_780X = SvalS;
          n_781X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + string_780X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L20854;}
        else {
          goto L31856;}}
      else {
        goto L31856;}}
      break;
    case 110 : {
      obj_782X = SvalS;
      if ((3 == (3 & obj_782X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_782X))) + -4))))>>2))))) {
          x_783X = SvalS;
          descriptor_784X = *((long *) ((((char *) (-3 + x_783X))) + 4));
          if ((17 == (255 & descriptor_784X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_783X))) + 4))))) {
              arg0K0 = 5;
              goto L44197;}
            else {
              arg0K0 = 1;
              goto L44197;}}
          else {
            arg0K0 = 5;
            goto L44197;}}
        else {
          goto L44178;}}
      else {
        goto L44178;}}
      break;
    case 111 : {
      arg2_785X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_785X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_785X))) + -4))))>>2))))) {
          x_786X = SvalS;
          if ((1 == x_786X)) {
            goto L39940;}
          else {
            if ((5 == x_786X)) {
              goto L39940;}
            else {
              goto L39947;}}}
        else {
          goto L39947;}}
      else {
        goto L39947;}}
      break;
    case 112 : {
      x_787X = SvalS;
      if ((3 == (3 & x_787X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_787X))) + -4)))))) {
          arg0K0 = 1;
          goto L48921;}
        else {
          arg0K0 = 5;
          goto L48921;}}
      else {
        arg0K0 = 5;
        goto L48921;}}
      break;
    case 113 : {
      x_788X = SvalS;
      if ((3 == (3 & x_788X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_788X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_788X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_788X))) + -4))));
          goto L43016;}
        else {
          goto L43016;}}
      else {
        goto L43016;}}
      break;
    case 114 : {
      s48_make_availableAgc(288);
      arg2_789X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        mode_790X = (((SvalS))>>2);
        if ((1 == mode_790X)) {
          goto L36118;}
        else {
          if ((2 == mode_790X)) {
            goto L36118;}
          else {
            if ((3 == mode_790X)) {
              goto L36118;}
            else {
              if ((4 == mode_790X)) {
                goto L36118;}
              else {
                push_exception_continuationB(5, 1);
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = arg2_789X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = (((mode_790X)<<2));
                arg0K0 = 2;
                goto L22004;}}}}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_789X;
        x_791X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_791X;
        arg0K0 = 2;
        goto L22004;}}
      break;
    case 115 : {
      s48_make_availableAgc(264);
      obj_792X = SvalS;
      if ((3 == (3 & obj_792X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_792X))) + -4))))>>2))))) {
          channel_793X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_793X))))))) {
            push_exception_continuationB(5, 1);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = channel_793X;
            arg0K0 = 1;
            goto L22004;}
          else {
            status_794X = close_channelB(channel_793X);
            if ((status_794X == NO_ERRORS)) {
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23956;}
            else {
              push_exception_continuationB(24, 1);
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = channel_793X;
              merged_arg0K0 = status_794X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 0;
              goto get_error_string;
             get_error_string_return_0:
              x_795X = get_error_string0_return_value;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = x_795X;
              arg0K0 = 2;
              goto L22004;}}}
        else {
          goto L48957;}}
      else {
        goto L48957;}}
      break;
    case 116 : {
      s48_make_availableAgc(264);
      arg2_796X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_797X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_798X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_799X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg5_799X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg5_799X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg3_797X | arg2_796X)))) {
            x_800X = SvalS;
            if ((1 == x_800X)) {
              goto L43143;}
            else {
              if ((5 == x_800X)) {
                goto L43143;}
              else {
                goto L43154;}}}
          else {
            goto L43154;}}
        else {
          goto L43154;}}
      else {
        goto L43154;}}
      break;
    case 117 : {
      s48_make_availableAgc(264);
      arg2_801X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_802X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_803X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg4_803X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + arg4_803X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_801X | (SvalS))))) {
            start_804X = ((arg2_801X)>>2);
            count_805X = (((SvalS))>>2);
            v_806X = 8 == (*((long *) (((char *) (-3 + arg4_803X)))));
            if (v_806X) {
              if ((3 == (3 & arg3_802X))) {
                if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_802X))) + -4))))>>2))))) {
                  goto L36757;}
                else {
                  goto L36932;}}
              else {
                goto L36932;}}
            else {
              goto L36811;}}
          else {
            goto L43357;}}
        else {
          goto L43357;}}
      else {
        goto L43357;}}
      break;
    case 118 : {
      s48_make_availableAgc(264);
      obj_807X = SvalS;
      if ((3 == (3 & obj_807X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_807X))) + -4))))>>2))))) {
          channel_808X = SvalS;
          if ((0 == (*((long *) (((char *) (-3 + channel_808X))))))) {
            push_exception_continuationB(5, 1);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = channel_808X;
            arg0K0 = 1;
            goto L22004;}
          else {
            readyP_809X = ps_check_fd(((((*((long *) ((((char *) (-3 + channel_808X))) + 8))))>>2)), (4 == (*((long *) (((char *) (-3 + channel_808X)))))), &status_810X);
            if ((status_810X == NO_ERRORS)) {
              if (readyP_809X) {
                arg0K0 = 5;
                goto L39531;}
              else {
                arg0K0 = 1;
                goto L39531;}}
            else {
              push_exception_continuationB(24, 1);
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = channel_808X;
              merged_arg0K0 = status_810X;
              merged_arg0K1 = 0;
              get_error_string_return_tag = 1;
              goto get_error_string;
             get_error_string_return_1:
              x_811X = get_error_string0_return_value;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = x_811X;
              arg0K0 = 2;
              goto L22004;}}}
        else {
          goto L48981;}}
      else {
        goto L48981;}}
      break;
    case 119 : {
      obj_812X = SvalS;
      if ((3 == (3 & obj_812X))) {
        if ((6 == (31 & ((((*((long *) ((((char *) (-3 + obj_812X))) + -4))))>>2))))) {
          channel_813X = SvalS;
          head_814X = Spending_channels_headS;
          if ((1 == head_814X)) {
            addr_815X = (((char *) (-3 + channel_813X))) + 16;
            S48_WRITE_BARRIER(channel_813X, addr_815X, 1);
            *((long *) addr_815X) = 1;
            n_816X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_813X))) + 8))))>>2)));
            arg0K0 = (((n_816X)<<2));
            goto L43505;}
          else {
            if ((channel_813X == head_814X)) {
              channel_817X = Spending_channels_headS;
              next_818X = *((long *) ((((char *) (-3 + channel_817X))) + 12));
              Spending_channels_headS = next_818X;
              addr_819X = (((char *) (-3 + channel_817X))) + 12;
              S48_WRITE_BARRIER(channel_817X, addr_819X, 1);
              *((long *) addr_819X) = 1;
              if ((1 == next_818X)) {
                Spending_channels_tailS = 1;
                goto L10983;}
              else {
                goto L10983;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_814X))) + 12)));
              arg0K1 = head_814X;
              goto L10989;}}}
        else {
          goto L43488;}}
      else {
        goto L43488;}}
      break;
    case 120 : {
      s48_make_availableAgc(((((3 * (Snumber_of_channelsS)))<<2)));
      arg0K0 = (-1 + (Snumber_of_channelsS));
      arg0K1 = 25;
      goto L16369;}
      break;
    case 121 : {
      SvalS = (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 122 : {
      proposal_820X = SvalS;
      if ((1 == proposal_820X)) {
        goto L37065;}
      else {
        if ((3 == (3 & proposal_820X))) {
          if ((2 == (31 & ((((*((long *) ((((char *) (-3 + proposal_820X))) + -4))))>>2))))) {
            if ((4 == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proposal_820X))) + -4))))>>8))))>>2)))) {
              if ((1 == (*((long *) (((char *) (-3 + proposal_820X))))))) {
                goto L37065;}
              else {
                goto L37096;}}
            else {
              goto L37096;}}
          else {
            goto L37096;}}
        else {
          goto L37096;}}}
      break;
    case 123 : {
      proposal_821X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
      if ((1 == proposal_821X)) {
        push_exception_continuationB(26, 1);
        arg0K0 = 0;
        goto L22004;}
      else {
        GET_PROPOSAL_LOCK();
        log_822X = *((long *) ((((char *) (-3 + proposal_821X))) + 4));
        arg0K0 = 0;
        goto L9864;}}
      break;
    case 124 : {
      stob_823X = SvalS;
      offset_824X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_825X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & stob_823X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + stob_823X))) + -4))))>>2))) == type_825X)) {
          x_826X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
          if ((1 == x_826X)) {
            arg0K0 = (*((long *) ((((char *) (-3 + stob_823X))) + (((offset_824X)<<2)))));
            goto L27978;}
          else {
            merged_arg0K0 = stob_823X;
            merged_arg0K1 = (((offset_824X)<<2));
            proposal_d_read_return_tag = 1;
            goto proposal_d_read;
           proposal_d_read_return_1:
            v_827X = proposal_d_read0_return_value;
            arg0K0 = v_827X;
            goto L27978;}}
        else {
          goto L27979;}}
      else {
        goto L27979;}}
      break;
    case 125 : {
      arg2_828X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_829X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_830X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_831X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & ((arg4_830X | arg2_828X) | (SvalS))))) {
        from_index_832X = ((arg4_830X)>>2);
        to_index_833X = ((arg2_828X)>>2);
        count_834X = (((SvalS))>>2);
        v_835X = *((unsigned char *) ((Scode_pointerS) + 1));
        merged_arg0K0 = arg5_831X;
        merged_arg0K1 = from_index_832X;
        merged_arg0K2 = count_834X;
        okay_copy_argsP_return_tag = 0;
        goto okay_copy_argsP;
       okay_copy_argsP_return_0:
        v_836X = okay_copy_argsP0_return_value;
        if (v_836X) {
          merged_arg0K0 = arg3_829X;
          merged_arg0K1 = to_index_833X;
          merged_arg0K2 = count_834X;
          okay_copy_argsP_return_tag = 1;
          goto okay_copy_argsP;
         okay_copy_argsP_return_1:
          v_837X = okay_copy_argsP0_return_value;
          if (v_837X) {
            if ((3 == (3 & arg3_829X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_829X))) + -4)))))) {
                if ((count_834X < 0)) {
                  goto L28072;}
                else {
                  if ((0 == v_835X)) {
                    goto L28132;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L28132;}
                    else {
                      arg0K0 = 4096;
                      arg0K1 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 12)));
                      goto L28220;}}}}
              else {
                goto L28072;}}
            else {
              goto L28072;}}
          else {
            goto L28072;}}
        else {
          goto L28072;}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg5_831X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_830X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_829X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_828X;
        x_838X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_838X;
        arg0K0 = 5;
        goto L22004;}}
      break;
    case 126 : {
      arg2_839X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_839X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg2_839X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_840X = (((SvalS))>>2);
            len_841X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_839X))) + -4))))>>8);
            if ((index_840X < 0)) {
              goto L28429;}
            else {
              if ((index_840X < len_841X)) {
                x_842X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                if ((1 == x_842X)) {
                  arg0K0 = ((((*((unsigned char *) ((((char *) (-3 + arg2_839X))) + index_840X))))<<2));
                  goto L28428;}
                else {
                  index_843X = ((index_840X)<<2);
                  log_844X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                  arg0K0 = 0;
                  goto L14940;}}
              else {
                goto L28429;}}}
          else {
            goto L31146;}}
        else {
          goto L31146;}}
      else {
        goto L31146;}}
      break;
    case 127 : {
      arg2_845X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_846X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg3_846X))) {
        if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_846X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_845X))) {
            index_847X = ((arg2_845X)>>2);
            byte_848X = SvalS;
            if ((3 == (3 & arg3_846X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_846X))) + -4)))))) {
                if ((0 == (3 & byte_848X))) {
                  len_849X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_846X))) + -4))))>>8);
                  if ((index_847X < 0)) {
                    goto L28555;}
                  else {
                    if ((index_847X < len_849X)) {
                      x_850X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
                      if ((1 == x_850X)) {
                        *((unsigned char *) ((((char *) (-3 + arg3_846X))) + index_847X)) = (((byte_848X)>>2));
                        goto L28554;}
                      else {
                        index_851X = ((index_847X)<<2);
                        log_852X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 8));
                        arg0K0 = 0;
                        goto L15116;}}
                    else {
                      goto L28555;}}}
                else {
                  goto L28520;}}
              else {
                goto L28520;}}
            else {
              goto L28520;}}
          else {
            goto L31251;}}
        else {
          goto L31251;}}
      else {
        goto L31251;}}
      break;
    case 128 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 129 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 130 : {
      x_853X = SvalS;
      push_exception_continuationB(15, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_853X;
      arg0K0 = 1;
      goto L22004;}
      break;
    case 131 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 132 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 133 : {
      s48_make_availableAgc(264);
      arg2_854X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_855X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_856X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg4_856X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_856X))) + -4))))>>2))))) {
          if ((3 == (3 & arg2_854X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_854X))) + -4))))>>2))))) {
              obj_857X = SvalS;
              if ((3 == (3 & obj_857X))) {
                if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_857X))) + -4))))>>2))))) {
                  undumpables_858X = SvalS;
                  port_859X = ps_open_output_file((((char *)(((char *) (-3 + arg4_856X))))), &status_860X);
                  if ((status_860X == NO_ERRORS)) {
                    status_861X = ps_write_string((((char *)(((char *) (-3 + arg2_854X))))), port_859X);
                    if ((status_861X == NO_ERRORS)) {
                      status_862X = s48_write_image(arg3_855X, undumpables_858X, port_859X);
                      if ((status_862X == NO_ERRORS)) {
                        status_863X = ps_close(port_859X);
                        if ((status_863X == NO_ERRORS)) {
                          SvalS = 13;
                          Scode_pointerS = ((Scode_pointerS) + 1);
                          arg1K0 = (Scode_pointerS);
                          goto L23956;}
                        else {
                          arg0K0 = 24;
                          arg0K1 = status_863X;
                          goto L40492;}}
                      else {
                        status_864X = ps_close(port_859X);
                        if ((status_864X == NO_ERRORS)) {
                          arg0K0 = 24;
                          arg0K1 = status_862X;
                          goto L40492;}
                        else {
                          ps_write_string("Unable to close image file", (stderr));
                          { long ignoreXX;
                          PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                          arg0K0 = 24;
                          arg0K1 = status_862X;
                          goto L40492;}}}
                    else {
                      status_865X = ps_close(port_859X);
                      if ((status_865X == NO_ERRORS)) {
                        arg0K0 = 24;
                        arg0K1 = status_861X;
                        goto L40492;}
                      else {
                        ps_write_string("Unable to close image file", (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg0K0 = 24;
                        arg0K1 = status_861X;
                        goto L40492;}}}
                  else {
                    arg0K0 = 10;
                    arg0K1 = status_860X;
                    goto L40492;}}
                else {
                  goto L44728;}}
              else {
                goto L44728;}}
            else {
              goto L44728;}}
          else {
            goto L44728;}}
        else {
          goto L44728;}}
      else {
        goto L44728;}}
      break;
    case 134 : {
      SvalS = 13;
      s48_collect();
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 135 : {
      obj_866X = SvalS;
      if ((3 == (3 & obj_866X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_866X))) + -4))))>>2))))) {
          x_867X = SvalS;
          n_868X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_867X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L45409;}
        else {
          goto L45386;}}
      else {
        goto L45386;}}
      break;
    case 136 : {
      s48_make_availableAgc(24);
      arg2_869X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      proc_870X = SvalS;
      if ((3 == (3 & arg2_869X))) {
        if ((3 == (3 & proc_870X))) {
          if ((3 == (31 & ((((*((long *) ((((char *) (-3 + proc_870X))) + -4))))>>2))))) {
            GET_PROPOSAL_LOCK();
            addr_871X = s48_allocate_small(12);
            *((long *) addr_871X) = 2050;
            x_872X = 3 + (((long) (addr_871X + 4)));
            *((long *) (((char *) (-3 + x_872X)))) = arg2_869X;
            *((long *) ((((char *) (-3 + x_872X))) + 4)) = proc_870X;
            b_873X = SHARED_REF((Sfinalizer_alistS));
            addr_874X = s48_allocate_small(12);
            *((long *) addr_874X) = 2050;
            x_875X = 3 + (((long) (addr_874X + 4)));
            *((long *) (((char *) (-3 + x_875X)))) = x_872X;
            *((long *) ((((char *) (-3 + x_875X))) + 4)) = b_873X;
            SHARED_SETB((Sfinalizer_alistS), x_875X);
            RELEASE_PROPOSAL_LOCK();
            SvalS = 13;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23956;}
          else {
            goto L40767;}}
        else {
          goto L40767;}}
      else {
        goto L40767;}}
      break;
    case 137 : {
      arg2_876X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_876X))) {
        key_877X = ((arg2_876X)>>2);
        other_878X = SvalS;
        if ((5 == key_877X)) {
          SvalS = (-4 & other_878X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          if ((0 == key_877X)) {
            x_879X = s48_available();
            SvalS = (((x_879X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23956;}
          else {
            if ((1 == key_877X)) {
              bytes_880X = s48_heap_size();
              SvalS = (-4 & (3 + bytes_880X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23956;}
            else {
              if ((2 == key_877X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23956;}
              else {
                if ((3 == key_877X)) {
                  x_881X = s48_gc_count();
                  SvalS = (((x_881X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L23956;}
                else {
                  if ((4 == key_877X)) {
                    push_exception_continuationB(14, 1);
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((key_877X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_878X;
                    arg0K0 = 2;
                    goto L22004;}
                  else {
                    push_exception_continuationB(17, 1);
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = (((key_877X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = other_878X;
                    arg0K0 = 2;
                    goto L22004;}}}}}}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_876X;
        x_882X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_882X;
        arg0K0 = 2;
        goto L22004;}}
      break;
    case 138 : {
      if ((0 == (3 & (SvalS)))) {
        type_883X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L43624;}
      else {
        push_exception_continuationB(5, 1);
        x_884X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_884X;
        arg0K0 = 1;
        goto L22004;}}
      break;
    case 139 : {
      x_885X = SvalS;
      arg2K0 = 1;
      arg0K1 = x_885X;
      goto L49089;}
      break;
    case 140 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 141 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 142 : {
      val_886X = SHARED_REF((Ssession_dataS));
      SvalS = val_886X;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 143 : {
      SHARED_SETB((Ssession_dataS), (SvalS));
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 144 : {
      obj_887X = SvalS;
      if ((3 == (3 & obj_887X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_887X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 166)) {
            goto L25661;}
          else {
            temp_888X = SHARED_REF((Sexception_handlersS));
            SHARED_SETB((Sexception_handlersS), (SvalS));
            SvalS = temp_888X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23956;}}
        else {
          goto L25661;}}
      else {
        goto L25661;}}
      break;
    case 145 : {
      data_889X = 3 + (((long) (SstackS)));
      SstackS = ((SstackS) + 20);
      pc_890X = *((long *) ((((char *) (-3 + data_889X))) + 4));
      template_891X = *((long *) ((((char *) (-3 + data_889X))) + 8));
      size_892X = *((long *) ((((char *) (-3 + data_889X))) + 16));
      StemplateS = template_891X;
      Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_891X)))))))) + ((((((((((pc_890X)>>2)) + (((size_892X)>>2))))<<2)))>>2)));
      arg1K0 = (Scode_pointerS);
      goto L23956;}
      break;
    case 146 : {
      obj_893X = SvalS;
      if ((3 == (3 & obj_893X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_893X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 5)) {
            goto L24441;}
          else {
            temp_894X = SHARED_REF((Sinterrupt_handlersS));
            SHARED_SETB((Sinterrupt_handlersS), (SvalS));
            SvalS = temp_894X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L23956;}}
        else {
          goto L24441;}}
      else {
        goto L24441;}}
      break;
    case 147 : {
      old_895X = Senabled_interruptsS;
      p_896X = SvalS;
      Senabled_interruptsS = (((p_896X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L49137;}
        else {
          goto L49137;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L49137;}}
      break;
    case 148 : {
      SstackS = ((SstackS) + 4);
      p_897X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      Senabled_interruptsS = (((p_897X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L23972;}
        else {
          goto L23972;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L23972;}}
      break;
    case 149 : {
      if ((0 == (3 & (SvalS)))) {
        p_898X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          s48_Spending_interruptPS = 0;
          if ((s48_Spending_eventsPS)) {
            s48_Spending_interruptPS = 1;
            goto L46357;}
          else {
            goto L46357;}}
        else {
          s48_Spending_interruptPS = 1;
          goto L46357;}}
      else {
        push_exception_continuationB(5, 1);
        x_899X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_899X;
        arg0K0 = 1;
        goto L22004;}}
      break;
    case 150 : {
      arg2_900X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_900X))) {
        x_901X = SvalS;
        if ((1 == x_901X)) {
          goto L47576;}
        else {
          if ((5 == x_901X)) {
            goto L47576;}
          else {
            goto L47581;}}}
      else {
        goto L47581;}}
      break;
    case 151 : {
      p_902X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      nargs_903X = ((p_902X)>>2);
      p_904X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      rest_list_905X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((14 < nargs_903X)) {
        push_exception_continuationB(19, 1);
        x_906X = *((long *) ((SstackS) + (-4 + (-4 & p_904X))));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_906X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = nargs_903X;
        arg0K0 = 2;
        goto L22004;}
      else {
        arg0K0 = rest_list_905X;
        goto L28754;}}
      break;
    case 152 : {
      s48_make_availableAgc(20);
      arg2_907X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_907X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_907X))) + -4))))>>2))))) {
          x_908X = SvalS;
          if ((1 == x_908X)) {
            goto L43719;}
          else {
            if ((5 == x_908X)) {
              goto L43719;}
            else {
              goto L43724;}}}
        else {
          goto L43724;}}
      else {
        goto L43724;}}
      break;
    case 153 : {
      arg2_909X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_909X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_909X))) + -4))))>>2))))) {
          x_910X = SvalS;
          if ((1 == x_910X)) {
            goto L30464;}
          else {
            if ((5 == x_910X)) {
              goto L30464;}
            else {
              goto L30469;}}}
        else {
          goto L30469;}}
      else {
        goto L30469;}}
      break;
    case 154 : {
      arg2_911X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_911X))) {
        option_912X = ((arg2_911X)>>2);
        other_913X = SvalS;
        if ((2 == option_912X)) {
          x_914X = CHEAP_TIME();
          SvalS = (((x_914X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          if ((0 == option_912X)) {
            seconds_915X = s48_run_time(&mseconds_916X);
            arg0K0 = option_912X;
            arg0K1 = seconds_915X;
            arg0K2 = mseconds_916X;
            goto L45320;}
          else {
            if ((1 == option_912X)) {
              seconds_917X = s48_real_time(&mseconds_918X);
              arg0K0 = option_912X;
              arg0K1 = seconds_917X;
              arg0K2 = mseconds_918X;
              goto L45320;}
            else {
              push_exception_continuationB(17, 1);
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = (((option_912X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = other_913X;
              arg0K0 = 2;
              goto L22004;}}}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_911X;
        x_919X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_919X;
        arg0K0 = 2;
        goto L22004;}}
      break;
    case 155 : {
      arg2_920X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_920X))) {
        key_921X = ((arg2_920X)>>2);
        value_922X = SvalS;
        status_923X = s48_extended_vm(key_921X, value_922X);
        if ((0 == status_923X)) {
          SvalS = (s48_Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          if ((1 == status_923X)) {
            push_exception_continuationB(22, 1);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((key_921X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_922X;
            arg0K0 = 2;
            goto L22004;}
          else {
            push_exception_continuationB(23, 1);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((key_921X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = value_922X;
            arg0K0 = 2;
            goto L22004;}}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_920X;
        x_924X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_924X;
        arg0K0 = 2;
        goto L22004;}}
      break;
    case 156 : {
      arg2_925X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      x_926X = SvalS;
      Senabled_interruptsS = -1;
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        s48_Spending_interruptPS = 0;
        if ((s48_Spending_eventsPS)) {
          s48_Spending_interruptPS = 1;
          goto L49201;}
        else {
          goto L49201;}}
      else {
        s48_Spending_interruptPS = 1;
        goto L49201;}}
      break;
    case 157 : {
      arg2_927X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((3 == (3 & arg2_927X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_927X))) + -4))))>>2))))) {
          obj_928X = SvalS;
          if ((3 == (3 & obj_928X))) {
            if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_928X))) + -4))))>>2))))) {
              x_929X = SvalS;
              len_930X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_927X))) + -4))))>>8);
              if ((len_930X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_929X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_929X))), (void *)(((char *) (-3 + arg2_927X))),len_930X)))) {
                  arg0K0 = 5;
                  goto L38119;}
                else {
                  arg0K0 = 1;
                  goto L38119;}}
              else {
                arg0K0 = 1;
                goto L38119;}}
            else {
              goto L38084;}}
          else {
            goto L38084;}}
        else {
          goto L38084;}}
      else {
        goto L38084;}}
      break;
    case 158 : {
      s48_make_availableAgc((4 + (-4 & (4 + ((((SvalS))>>2))))));
      arg2_931X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        n_932X = (((SvalS))>>2);
        if ((3 == (3 & arg2_931X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_931X))) + -4))))>>2))))) {
            goto L37811;}
          else {
            goto L37763;}}
        else {
          goto L37763;}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_931X;
        x_933X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_933X;
        arg0K0 = 2;
        goto L22004;}}
      break;
    case 159 : {
      arg2_934X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      list_935X = SvalS;
      arg0K0 = list_935X;
      arg0K1 = list_935X;
      arg2K2 = 1;
      goto L39594;}
      break;
    case 160 : {
      arg2_936X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_937X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & (SvalS)))) {
        index_938X = (((SvalS))>>2);
        if ((3 == (3 & arg3_937X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg3_937X))) + -4))))>>2))))) {
            if ((arg2_936X == (*((long *) (((char *) (-3 + arg3_937X))))))) {
              len_939X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_937X))) + -4))))>>8))))>>2);
              if ((index_938X < 0)) {
                goto L29102;}
              else {
                if ((index_938X < len_939X)) {
                  v_940X = *((unsigned char *) ((Scode_pointerS) + 1));
                  if ((0 == v_940X)) {
                    goto L29092;}
                  else {
                    if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                      goto L29092;}
                    else {
                      merged_arg0K0 = arg3_937X;
                      merged_arg0K1 = (((index_938X)<<2));
                      proposal_d_read_return_tag = 2;
                      goto proposal_d_read;
                     proposal_d_read_return_2:
                      v_941X = proposal_d_read0_return_value;
                      arg0K0 = v_941X;
                      goto L29101;}}}
                else {
                  goto L29102;}}}
            else {
              goto L29122;}}
          else {
            goto L29122;}}
        else {
          goto L29122;}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_937X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_936X;
        x_942X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_942X;
        arg0K0 = 3;
        goto L22004;}}
      break;
    case 161 : {
      arg2_943X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_944X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_945X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      if ((0 == (3 & arg2_943X))) {
        index_946X = ((arg2_943X)>>2);
        value_947X = SvalS;
        if ((3 == (3 & arg4_945X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + arg4_945X))) + -4))))>>2))))) {
            if ((arg3_944X == (*((long *) (((char *) (-3 + arg4_945X))))))) {
              if ((3 == (3 & arg4_945X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_945X))) + -4)))))) {
                  len_948X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_945X))) + -4))))>>8))))>>2);
                  if ((index_946X < 0)) {
                    goto L29360;}
                  else {
                    if ((index_946X < len_948X)) {
                      v_949X = *((unsigned char *) ((Scode_pointerS) + 1));
                      if ((0 == v_949X)) {
                        goto L29350;}
                      else {
                        if ((1 == (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12))))) {
                          goto L29350;}
                        else {
                          merged_arg0K0 = arg4_945X;
                          merged_arg0K1 = (((index_946X)<<2));
                          merged_arg0K2 = value_947X;
                          proposal_d_write_return_tag = 2;
                          goto proposal_d_write;
                         proposal_d_write_return_2:
                          goto L29359;}}}
                    else {
                      goto L29360;}}}
                else {
                  goto L29382;}}
              else {
                goto L29382;}}
            else {
              goto L29382;}}
          else {
            goto L29382;}}
        else {
          goto L29382;}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_945X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_944X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_943X;
        x_950X = SvalS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_950X;
        arg0K0 = 4;
        goto L22004;}}
      break;
    case 162 : {
      v_951X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_951X)) {
        arg0K0 = (SvalS);
        goto L29639;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_952X = get_current_port0_return_value;
        arg0K0 = v_952X;
        goto L29639;}}
      break;
    case 163 : {
      v_953X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_953X)) {
        arg0K0 = (SvalS);
        goto L29841;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_954X = get_current_port0_return_value;
        arg0K0 = v_954X;
        goto L29841;}}
      break;
    case 164 : {
      v_955X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_955X)) {
        v_956X = SvalS;
        v_957X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        arg0K0 = v_957X;
        arg0K1 = v_956X;
        goto L30036;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_958X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_958X;
        goto L30036;}}
      break;
    case 165 : {
      x_959X = SvalS;
      out_960X = stderr;
      arg0K0 = x_959X;
      goto L39847;}
      break;
  }}
 L21850: {
  v_961X = arg0K0;
  SvalS = v_961X;
  arg0K0 = cont_486X;
  goto L22600;}
 L47520: {
  stack_arg_count_962X = arg0K0;
  list_args_963X = arg0K1;
  list_arg_count_964X = arg0K2;
  if ((0 == list_arg_count_964X)) {
    obj_965X = SvalS;
    if ((3 == (3 & obj_965X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_965X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_962X;
        arg0K1 = 25;
        arg0K2 = 0;
        goto L47051;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_962X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22536;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_962X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L22536;}}
  else {
    obj_966X = SvalS;
    if ((3 == (3 & obj_966X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_966X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_962X;
        arg0K1 = list_args_963X;
        arg0K2 = list_arg_count_964X;
        goto L47051;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_962X;
        arg0K2 = list_args_963X;
        arg0K3 = list_arg_count_964X;
        goto L22536;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_962X;
      arg0K2 = list_args_963X;
      arg0K3 = list_arg_count_964X;
      goto L22536;}}}
 L25484: {
  x_967X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_967X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L25466: {
  stack_nargs_968X = arg0K0;
  l_969X = arg0K1;
  if ((count_489X == stack_nargs_968X)) {
    arg0K0 = l_969X;
    goto L25484;}
  else {
    x_970X = *((long *) (((char *) (-3 + l_969X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_970X;
    arg0K0 = (1 + stack_nargs_968X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_969X))) + 4)));
    goto L25466;}}
 L25389: {
  loc_971X = arg1K0;
  arg_972X = arg1K1;
  if ((arg_972X < arg_top_512X)) {
    SstackS = ((SstackS) + (0 - (((stack_nargs_509X)<<2))));
    if ((0 == list_arg_count_511X)) {
      goto L25365;}
    else {
      merged_arg0K0 = list_args_510X;
      merged_arg0K1 = list_arg_count_511X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      goto L25365;}}
  else {
    *((long *) loc_971X) = (*((long *) arg_972X));
    arg1K0 = (loc_971X + -4);
    arg1K1 = (arg_972X + -4);
    goto L25389;}}
 L22401: {
  i_973X = arg0K0;
  m_974X = arg0K1;
  if ((0 == (n_528X & m_974X))) {
    arg0K0 = (1 + i_973X);
    arg0K1 = (((m_974X)<<1));
    goto L22401;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_974X));
    if ((i_973X == 0)) {
      x_975X = Sinterrupted_templateS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_975X;
      Sinterrupted_templateS = 1;
      n_976X = Senabled_interruptsS;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((n_976X)<<2));
      arg0K0 = 2;
      goto L22271;}
    else {
      if ((i_973X == 2)) {
        x_977X = Sfinalize_theseS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_977X;
        Sfinalize_theseS = 25;
        n_978X = Senabled_interruptsS;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((n_978X)<<2));
        arg0K0 = 2;
        goto L22271;}
      else {
        if ((i_973X == 3)) {
          channel_979X = Spending_channels_headS;
          next_980X = *((long *) ((((char *) (-3 + channel_979X))) + 12));
          Spending_channels_headS = next_980X;
          addr_981X = (((char *) (-3 + channel_979X))) + 12;
          S48_WRITE_BARRIER(channel_979X, addr_981X, 1);
          *((long *) addr_981X) = 1;
          if ((1 == next_980X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_979X;
            goto L11174;}
          else {
            arg0K0 = channel_979X;
            goto L11174;}}
        else {
          if ((i_973X == 4)) {
            x_982X = Sos_signal_typeS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_982X;
            x_983X = Sos_signal_argumentS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_983X;
            Sos_signal_typeS = 1;
            Sos_signal_argumentS = 1;
            n_984X = Senabled_interruptsS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_984X)<<2));
            arg0K0 = 3;
            goto L22271;}
          else {
            n_985X = Senabled_interruptsS;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((n_985X)<<2));
            arg0K0 = 1;
            goto L22271;}}}}}}
 L30382: {
  env_986X = arg0K0;
  i_987X = arg0K1;
  if ((0 == i_987X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_986X))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 3);
      arg0K0 = 0;
      goto L22004;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L23956;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_986X)))));
    arg0K1 = (-1 + i_987X);
    goto L30382;}}
 L25207: {
  env_988X = arg0K0;
  i_989X = arg0K1;
  if ((0 == i_989X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_988X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)))))<<2)))));
    if ((529 == (SvalS))) {
      push_exception_continuationB(0, 5);
      arg0K0 = 0;
      goto L22004;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L23956;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_988X)))));
    arg0K1 = (-1 + i_989X);
    goto L25207;}}
 L25145: {
  env_990X = arg0K0;
  i_991X = arg0K1;
  if ((0 == i_991X)) {
    index_992X = ((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4)));
    value_993X = SvalS;
    addr_994X = (((char *) (-3 + env_990X))) + (((index_992X)<<2));
    S48_WRITE_BARRIER(env_990X, addr_994X, value_993X);
    *((long *) addr_994X) = value_993X;
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 5);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_990X)))));
    arg0K1 = (-1 + i_991X);
    goto L25145;}}
 L25843: {
  v_995X = arg0K0;
  s48_make_availableAgc((12 + (((v_995X)<<2))));
  p_996X = SenvS;
  if ((3 == (3 & p_996X))) {
    if ((p_996X < (((long) (Sstack_beginS))))) {
      goto L25897;}
    else {
      if (((((long) (Sstack_endS))) < p_996X)) {
        goto L25897;}
      else {
        v_997X = save_env_in_heap((SenvS), (ScontS), 0, 0);
        SenvS = v_997X;
        goto L25897;}}}
  else {
    goto L25897;}}
 L25856: {
  env_998X = arg0K0;
  key_999X = arg0K1;
  merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  merged_arg0K1 = env_998X;
  merged_arg0K2 = key_999X;
  make_closure_return_tag = 0;
  goto make_closure;
 make_closure_return_0:
  value_1000X = make_closure0_return_value;
  SvalS = value_1000X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L24928: {
  i_1001X = arg0K0;
  if ((count_546X < i_1001X)) {
    value_1002X = *((long *) (((char *) (-3 + (SvalS)))));
    addr_1003X = ((char *) (-3 + vector_549X));
    S48_WRITE_BARRIER(vector_549X, addr_1003X, value_1002X);
    *((long *) addr_1003X) = value_1002X;
    x_1004X = SvalS;
    addr_1005X = ((char *) (-3 + x_1004X));
    S48_WRITE_BARRIER(x_1004X, addr_1005X, vector_549X);
    *((long *) addr_1005X) = vector_549X;
    x_1006X = SenvS;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1006X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (1034 + (((count_546X)<<10)));
    SenvS = (3 + (((long) ((SstackS) + 4))));
    Scode_pointerS = ((Scode_pointerS) + (3 + (((count_546X)<<1))));
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    index_1007X = ((i_1001X)<<1);
    merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + index_1007X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_1007X))))))<<2)))));
    merged_arg0K1 = (SvalS);
    merged_arg0K2 = 0;
    make_closure_return_tag = 1;
    goto make_closure;
   make_closure_return_1:
    closure_1008X = make_closure0_return_value;
    addr_1009X = (((char *) (-3 + vector_549X))) + (((i_1001X)<<2));
    S48_WRITE_BARRIER(vector_549X, addr_1009X, closure_1008X);
    *((long *) addr_1009X) = closure_1008X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = closure_1008X;
    arg0K0 = (1 + i_1001X);
    goto L24928;}}
 L26180: {
  start_i_1010X = arg0K0;
  arg0K0 = start_i_1010X;
  arg0K1 = 2;
  arg0K2 = (SenvS);
  goto L26188;}
 L26031: {
  start_i_1011X = arg0K0;
  arg0K0 = start_i_1011X;
  arg0K1 = 3;
  arg0K2 = (SenvS);
  goto L26039;}
 L48444: {
  value_1012X = arg0K0;
  SvalS = value_1012X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L46804: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 1;
      goto check_events;
     check_events_return_1:
      v_1013X = check_events0_return_value;
      if (v_1013X) {
        arg0K0 = stack_arg_count_572X;
        goto L22237;}
      else {
        goto L46808;}}
    else {
      arg0K0 = stack_arg_count_572X;
      goto L22237;}}
  else {
    goto L46808;}}
 L15334: {
  list_args_1014X = arg0K0;
  stack_nargs_1015X = arg0K1;
  merged_arg0K0 = list_args_1014X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_1016X = okay_argument_list0_return_value;
  list_arg_count_1017X = okay_argument_list1_return_value;
  if (okayP_1016X) {
    SvalS = proc_591X;
    arg0K0 = stack_nargs_1015X;
    arg0K1 = list_args_1014X;
    arg0K2 = list_arg_count_1017X;
    goto L47520;}
  else {
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = list_args_1014X;
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_1015X);
    pop_args_GlistSAgc_return_tag = 6;
    goto pop_args_GlistSAgc;
   pop_args_GlistSAgc_return_6:
    args_1018X = pop_args_GlistSAgc0_return_value;
    proc_1019X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    push_exception_continuationB(5, 0);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = proc_1019X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_1018X;
    arg0K0 = 2;
    goto L22004;}}
 L15379: {
  list_1020X = arg0K0;
  follower_1021X = arg0K1;
  if ((25 == list_1020X)) {
    list_args_1022X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_1021X))) + 4)))))));
    addr_1023X = (((char *) (-3 + follower_1021X))) + 4;
    S48_WRITE_BARRIER(follower_1021X, addr_1023X, list_args_1022X);
    *((long *) addr_1023X) = list_args_1022X;
    arg0K0 = rest_list_592X;
    arg0K1 = (-1 + stack_nargs_590X);
    goto L15334;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_1020X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_1021X))) + 4)));
    goto L15379;}}
 L23120: {
  obj_1024X = SvalS;
  if ((3 == (3 & obj_1024X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1024X))) + -4))))>>2))))) {
      arg0K0 = 0;
      arg0K1 = 25;
      arg0K2 = 0;
      goto L47051;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L22536;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L22536;}}
 L23129: {
  SstackS = (Sbottom_of_stackS);
  *((long *) ((Sbottom_of_stackS) + 4)) = cont_594X;
  ScontS = (Sbottom_of_stackS);
  goto L23120;}
 L47678: {
  code_pointer_1025X = ((char *) (*((long *) (ScontS))));
  protocol_1026X = *((unsigned char *) (code_pointer_1025X + 1));
  if ((1 == protocol_1026X)) {
    goto L47695;}
  else {
    if ((66 == protocol_1026X)) {
      goto L47695;}
    else {
      if ((129 == protocol_1026X)) {
        goto L47711;}
      else {
        if ((194 == protocol_1026X)) {
          goto L47711;}
        else {
          if ((71 == protocol_1026X)) {
            cont_1027X = *((long *) ((Sbottom_of_stackS) + 4));
            if ((3 == (3 & cont_1027X))) {
              if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_1027X))) + -4))))>>2))))) {
                merged_arg0K0 = cont_1027X;
                merged_arg0K1 = 0;
                copy_continuation_from_heapB_return_tag = 2;
                goto copy_continuation_from_heapB;
               copy_continuation_from_heapB_return_2:
                goto L47678;}
              else {
                arg0K0 = cont_1027X;
                goto L22600;}}
            else {
              arg0K0 = cont_1027X;
              goto L22600;}}
          else {
            if ((70 == protocol_1026X)) {
              consumer_1028X = *((long *) ((ScontS) + 16));
              next_1029X = *((long *) ((ScontS) + 4));
              if (((ScontS) == (Sbottom_of_stackS))) {
                *((long *) ((ScontS) + 4)) = (*((long *) ((((char *) (-3 + next_1029X))) + 4)));
                goto L47735;}
              else {
                ScontS = (((char *) next_1029X));
                goto L47735;}}
            else {
              if ((65 == protocol_1026X)) {
                wants_stack_args_1030X = ((((*((unsigned char *) (code_pointer_1025X + 2))))<<8)) + (*((unsigned char *) (code_pointer_1025X + 3)));
                if ((0 == wants_stack_args_1030X)) {
                  cont_1031X = ScontS;
                  template_1032X = *((long *) (cont_1031X + 8));
                  Scode_pointerS = (((char *) (*((long *) cont_1031X))));
                  StemplateS = template_1032X;
                  SenvS = (*((long *) (cont_1031X + 12)));
                  ScontS = (((char *) (*((long *) (cont_1031X + 4)))));
                  SstackS = (cont_1031X + 16);
                  s48_make_availableAgc(12);
                  a_1033X = SvalS;
                  addr_1034X = s48_allocate_small(12);
                  *((long *) addr_1034X) = 2050;
                  x_1035X = 3 + (((long) (addr_1034X + 4)));
                  *((long *) (((char *) (-3 + x_1035X)))) = a_1033X;
                  *((long *) ((((char *) (-3 + x_1035X))) + 4)) = 25;
                  SstackS = ((SstackS) + -4);
                  *((long *) (SstackS)) = x_1035X;
                  Scode_pointerS = ((Scode_pointerS) + 4);
                  arg1K0 = (Scode_pointerS);
                  goto L23956;}
                else {
                  if ((1 == wants_stack_args_1030X)) {
                    cont_1036X = ScontS;
                    template_1037X = *((long *) (cont_1036X + 8));
                    Scode_pointerS = (((char *) (*((long *) cont_1036X))));
                    StemplateS = template_1037X;
                    SenvS = (*((long *) (cont_1036X + 12)));
                    ScontS = (((char *) (*((long *) (cont_1036X + 4)))));
                    SstackS = (cont_1036X + 16);
                    x_1038X = SvalS;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_1038X;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = 25;
                    Scode_pointerS = ((Scode_pointerS) + 4);
                    arg1K0 = (Scode_pointerS);
                    goto L23956;}
                  else {
                    x_1039X = SvalS;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_1039X;
                    merged_arg0K0 = 25;
                    merged_arg0K1 = 1;
                    pop_args_GlistSAgc_return_tag = 7;
                    goto pop_args_GlistSAgc;
                   pop_args_GlistSAgc_return_7:
                    args_1040X = pop_args_GlistSAgc0_return_value;
                    push_exception_continuationB(4, 0);
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = 1;
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = args_1040X;
                    arg0K0 = 2;
                    goto L22004;}}}
              else {
                x_1041X = SvalS;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_1041X;
                merged_arg0K0 = 25;
                merged_arg0K1 = 1;
                pop_args_GlistSAgc_return_tag = 8;
                goto pop_args_GlistSAgc;
               pop_args_GlistSAgc_return_8:
                args_1042X = pop_args_GlistSAgc0_return_value;
                push_exception_continuationB(4, 0);
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = 1;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = args_1042X;
                arg0K0 = 2;
                goto L22004;}}}}}}}}
 L46581: {
  tem_1043X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  StemplateS = tem_1043X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1043X)))))))) + 2);
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      v_1044X = check_events0_return_value;
      if (v_1044X) {
        arg0K0 = 0;
        goto L22237;}
      else {
        goto L46591;}}
    else {
      arg0K0 = 0;
      goto L22237;}}
  else {
    goto L46591;}}
 L46624: {
  push_exception_continuationB(5, 4);
  x_1045X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1045X;
  arg0K0 = 1;
  goto L22004;}
 L24260: {
  arg0K0 = (2 + (((max_605X)<<1)));
  goto L24262;}
 L24262: {
  offset_1046X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_1046X);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L24698: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L24158: {
  delta_1047X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_1047X);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L24589: {
  delta_1048X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_1048X);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L48501: {
  val_1049X = arg0K0;
  SvalS = val_1049X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L48513: {
  val_1050X = arg0K0;
  SvalS = val_1050X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L13742: {
  if ((3 == (3 & x_620X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + x_620X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L48513;}
    else {
      goto L13748;}}
  else {
    goto L13748;}}
 L38674: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L38675: {
  if ((3 == (3 & n_621X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_621X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = n_621X;
      arg0K0 = 1;
      goto L22004;}
    else {
      goto L38679;}}
  else {
    goto L38679;}}
 L38831: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L38806: {
  if ((3 == (3 & n_622X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_622X))) + -4))))>>2))))) {
      goto L38831;}
    else {
      goto L38820;}}
  else {
    goto L38820;}}
 L39030: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L39005: {
  if ((3 == (3 & n_623X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_623X))) + -4))))>>2))))) {
      goto L39030;}
    else {
      goto L39019;}}
  else {
    goto L39019;}}
 L39229: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L39204: {
  if ((3 == (3 & n_624X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + n_624X))) + -4))))>>2))))) {
      goto L39229;}
    else {
      goto L39218;}}
  else {
    goto L39218;}}
 L31914: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L31915: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_625X;
  arg0K0 = 1;
  goto L22004;}
 L32040: {
  v_1051X = (char *) s48_long_to_bignum(x_630X);
  v_1052X = enter_bignum(v_1051X);
  arg0K0 = v_1052X;
  goto L31986;}
 L31986: {
  val_1053X = arg0K0;
  SvalS = val_1053X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L31992: {
  if ((0 == (3 & y_629X))) {
    goto L31998;}
  else {
    if ((3 == (3 & y_629X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_629X))) + -4))))>>2))))) {
        goto L31998;}
      else {
        goto L32001;}}
    else {
      goto L32001;}}}
 L32001: {
  if ((3 == (3 & arg2_628X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_628X))) + -4))))>>2))))) {
      if ((3 == (3 & y_629X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_629X))) + -4))))>>2))))) {
          Stemp0S = arg2_628X;
          Stemp1S = y_629X;
          s48_make_availableAgc(12);
          value_1054X = Stemp1S;
          Stemp1S = 1;
          y_1055X = *((double *) (((char *) (-3 + value_1054X))));
          value_1056X = Stemp0S;
          Stemp0S = 1;
          x_1057X = *((double *) (((char *) (-3 + value_1056X))));
          addr_1058X = s48_allocate_small(12);
          *((long *) addr_1058X) = 2126;
          Kdouble_1059X = 3 + (((long) (addr_1058X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1059X)))) = (x_1057X + y_1055X);
          SvalS = Kdouble_1059X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          goto L32015;}}
      else {
        goto L32015;}}
    else {
      goto L32015;}}
  else {
    goto L32015;}}
 L8204: {
  a_1060X = arg0K0;
  if ((b_634X < 0)) {
    arg0K0 = (0 - b_634X);
    goto L8208;}
  else {
    arg0K0 = b_634X;
    goto L8208;}}
 L40887: {
  if ((0 == (3 & y_632X))) {
    goto L40893;}
  else {
    if ((3 == (3 & y_632X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_632X))) + -4))))>>2))))) {
        goto L40893;}
      else {
        goto L40896;}}
    else {
      goto L40896;}}}
 L40896: {
  if ((3 == (3 & arg2_631X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_631X))) + -4))))>>2))))) {
      if ((3 == (3 & y_632X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_632X))) + -4))))>>2))))) {
          Stemp0S = arg2_631X;
          Stemp1S = y_632X;
          s48_make_availableAgc(12);
          value_1061X = Stemp1S;
          Stemp1S = 1;
          y_1062X = *((double *) (((char *) (-3 + value_1061X))));
          value_1063X = Stemp0S;
          Stemp0S = 1;
          x_1064X = *((double *) (((char *) (-3 + value_1063X))));
          addr_1065X = s48_allocate_small(12);
          *((long *) addr_1065X) = 2126;
          Kdouble_1066X = 3 + (((long) (addr_1065X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1066X)))) = (x_1064X * y_1062X);
          SvalS = Kdouble_1066X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          goto L40910;}}
      else {
        goto L40910;}}
    else {
      goto L40910;}}
  else {
    goto L40910;}}
 L32329: {
  v_1067X = (char *) s48_long_to_bignum(x_637X);
  v_1068X = enter_bignum(v_1067X);
  arg0K0 = v_1068X;
  goto L32275;}
 L32275: {
  val_1069X = arg0K0;
  SvalS = val_1069X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L32281: {
  if ((0 == (3 & y_636X))) {
    goto L32287;}
  else {
    if ((3 == (3 & y_636X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_636X))) + -4))))>>2))))) {
        goto L32287;}
      else {
        goto L32290;}}
    else {
      goto L32290;}}}
 L32290: {
  if ((3 == (3 & arg2_635X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_635X))) + -4))))>>2))))) {
      if ((3 == (3 & y_636X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_636X))) + -4))))>>2))))) {
          Stemp0S = arg2_635X;
          Stemp1S = y_636X;
          s48_make_availableAgc(12);
          value_1070X = Stemp1S;
          Stemp1S = 1;
          y_1071X = *((double *) (((char *) (-3 + value_1070X))));
          value_1072X = Stemp0S;
          Stemp0S = 1;
          x_1073X = *((double *) (((char *) (-3 + value_1072X))));
          addr_1074X = s48_allocate_small(12);
          *((long *) addr_1074X) = 2126;
          Kdouble_1075X = 3 + (((long) (addr_1074X + 4)));
          *((double *) (((char *) (-3 + Kdouble_1075X)))) = (x_1073X - y_1071X);
          SvalS = Kdouble_1075X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L23956;}
        else {
          goto L32304;}}
      else {
        goto L32304;}}
    else {
      goto L32304;}}
  else {
    goto L32304;}}
 L8462: {
  a_1076X = arg0K0;
  if ((b_641X < 0)) {
    arg0K0 = (0 - b_641X);
    goto L8466;}
  else {
    arg0K0 = b_641X;
    goto L8466;}}
 L41140: {
  if ((0 == (3 & y_639X))) {
    goto L41146;}
  else {
    if ((3 == (3 & y_639X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_639X))) + -4))))>>2))))) {
        goto L41146;}
      else {
        goto L41173;}}
    else {
      goto L41173;}}}
 L41173: {
  if ((3 == (3 & arg2_638X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_638X))) + -4))))>>2))))) {
      if ((3 == (3 & y_639X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_639X))) + -4))))>>2))))) {
          if ((0. == (*((double *) (((char *) (-3 + y_639X))))))) {
            arg0K0 = 1;
            goto L41186;}
          else {
            Stemp0S = arg2_638X;
            Stemp1S = y_639X;
            s48_make_availableAgc(12);
            value_1077X = Stemp1S;
            Stemp1S = 1;
            y_1078X = *((double *) (((char *) (-3 + value_1077X))));
            value_1079X = Stemp0S;
            Stemp0S = 1;
            value_1080X = (*((double *) (((char *) (-3 + value_1079X))))) / y_1078X;
            addr_1081X = s48_allocate_small(12);
            *((long *) addr_1081X) = 2126;
            Kdouble_1082X = 3 + (((long) (addr_1081X + 4)));
            *((double *) (((char *) (-3 + Kdouble_1082X)))) = value_1080X;
            arg0K0 = Kdouble_1082X;
            goto L41186;}}
        else {
          goto L41193;}}
      else {
        goto L41193;}}
    else {
      goto L41193;}}
  else {
    goto L41193;}}
 L32560: {
  val_1083X = arg0K0;
  SvalS = val_1083X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L32566: {
  if ((0 == (3 & y_643X))) {
    goto L32572;}
  else {
    if ((3 == (3 & y_643X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_643X))) + -4))))>>2))))) {
        goto L32572;}
      else {
        goto L32577;}}
    else {
      goto L32577;}}}
 L32577: {
  if ((3 == (3 & arg2_642X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_642X))) + -4))))>>2))))) {
      if ((3 == (3 & y_643X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_643X))) + -4))))>>2))))) {
          b_1084X = (*((double *) (((char *) (-3 + arg2_642X))))) == (*((double *) (((char *) (-3 + y_643X)))));
          if (b_1084X) {
            arg0K0 = 5;
            goto L32592;}
          else {
            arg0K0 = 1;
            goto L32592;}}
        else {
          goto L32593;}}
      else {
        goto L32593;}}
    else {
      goto L32593;}}
  else {
    goto L32593;}}
 L32820: {
  val_1085X = arg0K0;
  SvalS = val_1085X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L32826: {
  if ((0 == (3 & y_645X))) {
    goto L32832;}
  else {
    if ((3 == (3 & y_645X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_645X))) + -4))))>>2))))) {
        goto L32832;}
      else {
        goto L32837;}}
    else {
      goto L32837;}}}
 L32837: {
  if ((3 == (3 & arg2_644X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_644X))) + -4))))>>2))))) {
      if ((3 == (3 & y_645X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_645X))) + -4))))>>2))))) {
          b_1086X = (*((double *) (((char *) (-3 + arg2_644X))))) < (*((double *) (((char *) (-3 + y_645X)))));
          if (b_1086X) {
            arg0K0 = 5;
            goto L32852;}
          else {
            arg0K0 = 1;
            goto L32852;}}
        else {
          goto L32853;}}
      else {
        goto L32853;}}
    else {
      goto L32853;}}
  else {
    goto L32853;}}
 L33143: {
  val_1087X = arg0K0;
  SvalS = val_1087X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33149: {
  if ((0 == (3 & y_647X))) {
    goto L33155;}
  else {
    if ((3 == (3 & y_647X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_647X))) + -4))))>>2))))) {
        goto L33155;}
      else {
        goto L33160;}}
    else {
      goto L33160;}}}
 L33160: {
  if ((3 == (3 & arg2_646X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_646X))) + -4))))>>2))))) {
      if ((3 == (3 & y_647X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_647X))) + -4))))>>2))))) {
          b_1088X = (*((double *) (((char *) (-3 + y_647X))))) < (*((double *) (((char *) (-3 + arg2_646X)))));
          if (b_1088X) {
            arg0K0 = 5;
            goto L33175;}
          else {
            arg0K0 = 1;
            goto L33175;}}
        else {
          goto L33176;}}
      else {
        goto L33176;}}
    else {
      goto L33176;}}
  else {
    goto L33176;}}
 L33466: {
  val_1089X = arg0K0;
  SvalS = val_1089X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33472: {
  if ((0 == (3 & y_649X))) {
    goto L33478;}
  else {
    if ((3 == (3 & y_649X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_649X))) + -4))))>>2))))) {
        goto L33478;}
      else {
        goto L33483;}}
    else {
      goto L33483;}}}
 L33483: {
  if ((3 == (3 & arg2_648X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_648X))) + -4))))>>2))))) {
      if ((3 == (3 & y_649X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_649X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + y_649X))))) < (*((double *) (((char *) (-3 + arg2_648X))))))) {
            arg0K0 = 1;
            goto L33498;}
          else {
            arg0K0 = 5;
            goto L33498;}}
        else {
          goto L33499;}}
      else {
        goto L33499;}}
    else {
      goto L33499;}}
  else {
    goto L33499;}}
 L33760: {
  val_1090X = arg0K0;
  SvalS = val_1090X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33766: {
  if ((0 == (3 & y_651X))) {
    goto L33772;}
  else {
    if ((3 == (3 & y_651X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_651X))) + -4))))>>2))))) {
        goto L33772;}
      else {
        goto L33777;}}
    else {
      goto L33777;}}}
 L33777: {
  if ((3 == (3 & arg2_650X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_650X))) + -4))))>>2))))) {
      if ((3 == (3 & y_651X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + y_651X))) + -4))))>>2))))) {
          if (((*((double *) (((char *) (-3 + arg2_650X))))) < (*((double *) (((char *) (-3 + y_651X))))))) {
            arg0K0 = 1;
            goto L33792;}
          else {
            arg0K0 = 5;
            goto L33792;}}
        else {
          goto L33793;}}
      else {
        goto L33793;}}
    else {
      goto L33793;}}
  else {
    goto L33793;}}
 L19496: {
  x_1091X = arg0K0;
  y_1092X = arg0K1;
  if ((0 == (3 & x_1091X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19526;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1091X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19526;}}
 L9105: {
  a_1093X = arg0K0;
  if ((b_655X < 0)) {
    arg0K0 = (0 - b_655X);
    goto L9109;}
  else {
    arg0K0 = b_655X;
    goto L9109;}}
 L34066: {
  if ((0 == (3 & y_653X))) {
    goto L34072;}
  else {
    if ((3 == (3 & y_653X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_653X))) + -4))))>>2))))) {
        goto L34072;}
      else {
        goto L34075;}}
    else {
      goto L34075;}}}
 L34075: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_652X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_653X;
  arg0K0 = 2;
  goto L22004;}
 L19405: {
  x_1094X = arg0K0;
  y_1095X = arg0K1;
  if ((0 == (3 & x_1094X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19435;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1094X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19435;}}
 L34280: {
  a_1096X = arg0K0;
  n_1097X = ((y_657X)>>2);
  if ((n_1097X < 0)) {
    arg0K0 = (0 - n_1097X);
    goto L34282;}
  else {
    arg0K0 = n_1097X;
    goto L34282;}}
 L34242: {
  if ((0 == (3 & y_657X))) {
    goto L34248;}
  else {
    if ((3 == (3 & y_657X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_657X))) + -4))))>>2))))) {
        goto L34248;}
      else {
        goto L34251;}}
    else {
      goto L34251;}}}
 L34251: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_656X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_657X;
  arg0K0 = 2;
  goto L22004;}
 L34429: {
  SvalS = n_659X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L34430: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_659X;
  arg0K0 = 1;
  goto L22004;}
 L34484: {
  SvalS = n_660X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L34485: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_660X;
  arg0K0 = 1;
  goto L22004;}
 L34539: {
  SvalS = 4;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L34542: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_661X;
  arg0K0 = 1;
  goto L22004;}
 L34599: {
  SvalS = n_662X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L34600: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_662X;
  arg0K0 = 1;
  goto L22004;}
 L34654: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L34657: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = n_663X;
  arg0K0 = 1;
  goto L22004;}
 L48689: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_672X;
  arg0K0 = 1;
  goto L22004;}
 L48695: {
  SvalS = 0;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L22787: {
  x_1098X = SvalS;
  if ((0 == (3 & x_1098X))) {
    s48_make_availableAgc(16);
    if ((x_1098X < 0)) {
      arg0K0 = (0 - x_1098X);
      goto L22815;}
    else {
      arg0K0 = x_1098X;
      goto L22815;}}
  else {
    if ((0 == (3 & x_1098X))) {
      arg0K0 = 1;
      arg0K1 = 3;
      goto L19013;}
    else {
      arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1098X))) + -4))))>>8))))>>2)));
      arg0K1 = 0;
      goto L19013;}}}
 L22790: {
  push_exception_continuationB(5, 1);
  x_1099X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1099X;
  arg0K0 = 1;
  goto L22004;}
 L34808: {
  if ((0 == (3 & x_682X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19360;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_682X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19360;}}
 L34811: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_682X;
  arg0K0 = 1;
  goto L22004;}
 L34899: {
  x_1100X = arg0K0;
  arg0K0 = x_1100X;
  arg0K1 = 0;
  goto L34905;}
 L34882: {
  if ((0 == (3 & x_683X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L19302;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_683X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L19302;}}
 L34885: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_683X;
  arg0K0 = 1;
  goto L22004;}
 L34984: {
  if ((0 == (3 & y_686X))) {
    goto L34990;}
  else {
    if ((3 == (3 & y_686X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_686X))) + -4))))>>2))))) {
        goto L34990;}
      else {
        goto L34993;}}
    else {
      goto L34993;}}}
 L34993: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_685X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_686X;
  arg0K0 = 2;
  goto L22004;}
 L35147: {
  if ((0 == (3 & y_688X))) {
    goto L35153;}
  else {
    if ((3 == (3 & y_688X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_688X))) + -4))))>>2))))) {
        goto L35153;}
      else {
        goto L35156;}}
    else {
      goto L35156;}}}
 L35156: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_687X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_688X;
  arg0K0 = 2;
  goto L22004;}
 L35310: {
  if ((0 == (3 & y_690X))) {
    goto L35316;}
  else {
    if ((3 == (3 & y_690X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_690X))) + -4))))>>2))))) {
        goto L35316;}
      else {
        goto L35319;}}
    else {
      goto L35319;}}}
 L35319: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_689X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_690X;
  arg0K0 = 2;
  goto L22004;}
 L31694: {
  val_1101X = arg0K0;
  SvalS = val_1101X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L31710: {
  val_1102X = arg0K0;
  SvalS = val_1102X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L31711: {
  push_exception_continuationB(6, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_691X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_692X;
  arg0K0 = 2;
  goto L22004;}
 L41466: {
  if ((0 == (3 & y_692X))) {
    if ((0 == (3 & arg2_691X))) {
      value_1103X = ((arg2_691X)>>2);
      count_1104X = ((y_692X)>>2);
      if ((count_1104X < 0)) {
        PS_SHIFT_RIGHT(value_1103X, (0 - count_1104X), x_1105X)
        x_1106X = x_1105X;
        s48_make_availableAgc(16);
        if ((536870911 < x_1106X)) {
          goto L41586;}
        else {
          if ((x_1106X < -536870912)) {
            goto L41586;}
          else {
            arg0K0 = (((x_1106X)<<2));
            goto L41581;}}}
      else {
        PS_SHIFT_LEFT(value_1103X, count_1104X, x_1107X)
        result_1108X = x_1107X;
        PS_SHIFT_RIGHT(result_1108X, count_1104X, x_1109X)
        if ((value_1103X == x_1109X)) {
          if ((value_1103X < 0)) {
            if ((result_1108X < 0)) {
              s48_make_availableAgc(16);
              if ((536870911 < result_1108X)) {
                goto L41608;}
              else {
                if ((result_1108X < -536870912)) {
                  goto L41608;}
                else {
                  arg0K0 = (((result_1108X)<<2));
                  goto L41603;}}}
            else {
              arg0K0 = arg2_691X;
              arg0K1 = y_692X;
              goto L41479;}}
          else {
            if ((result_1108X < 0)) {
              arg0K0 = arg2_691X;
              arg0K1 = y_692X;
              goto L41479;}
            else {
              s48_make_availableAgc(16);
              if ((536870911 < result_1108X)) {
                goto L41630;}
              else {
                if ((result_1108X < -536870912)) {
                  goto L41630;}
                else {
                  arg0K0 = (((result_1108X)<<2));
                  goto L41625;}}}}}
        else {
          arg0K0 = arg2_691X;
          arg0K1 = y_692X;
          goto L41479;}}}
    else {
      if ((3 == (3 & arg2_691X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_691X))) + -4))))>>2))))) {
          y_1110X = ((y_692X)>>2);
          merged_arg0K0 = arg2_691X;
          merged_arg0K1 = y_1110X;
          shift_space_return_tag = 0;
          goto shift_space;
         shift_space_return_0:
          needed_1111X = shift_space0_return_value;
          Stemp0S = arg2_691X;
          s48_make_availableAgc((((needed_1111X)<<2)));
          value_1112X = Stemp0S;
          Stemp0S = 1;
          if ((0 == (3 & value_1112X))) {
            v_1113X = (char *) s48_long_to_bignum((((value_1112X)>>2)));
            arg1K0 = v_1113X;
            goto L41661;}
          else {
            arg1K0 = (((char *) (-3 + value_1112X)));
            goto L41661;}}
        else {
          goto L41488;}}
      else {
        goto L41488;}}}
  else {
    push_exception_continuationB(5, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = arg2_691X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_692X;
    arg0K0 = 2;
    goto L22004;}}
 L48794: {
  val_1114X = arg0K0;
  SvalS = val_1114X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L38447: {
  val_1115X = arg0K0;
  SvalS = val_1115X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L38418: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_696X;
  x_1116X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1116X;
  arg0K0 = 2;
  goto L22004;}
 L38355: {
  val_1117X = arg0K0;
  SvalS = val_1117X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L38326: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_698X;
  x_1118X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1118X;
  arg0K0 = 2;
  goto L22004;}
 L42680: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((x_701X)<<2));
  arg0K0 = 1;
  goto L22004;}
 L48825: {
  val_1119X = arg0K0;
  SvalS = val_1119X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L48839: {
  value_1120X = arg0K0;
  SvalS = value_1120X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L26368: {
  push_exception_continuationB(5, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_706X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_707X)<<2));
  arg0K0 = 2;
  goto L22004;}
 L26473: {
  SvalS = new_712X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L26457: {
  i_1121X = arg0K0;
  if ((i_1121X < 0)) {
    goto L26473;}
  else {
    value_1122X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_712X))) + (((i_1121X)<<2)))) = value_1122X;
    arg0K0 = (-1 + i_1121X);
    goto L26457;}}
 L26591: {
  i_1123X = arg0K0;
  if ((i_1123X < 0)) {
    arg0K0 = stack_nargs_720X;
    arg0K1 = rest_list_721X;
    goto L26733;}
  else {
    value_1124X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_718X))) + (((i_1123X)<<2)))) = value_1124X;
    arg0K0 = (-1 + i_1123X);
    goto L26591;}}
 L26759: {
  push_exception_continuationB(5, 3);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_722X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_724X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_723X)<<2));
  arg0K0 = 3;
  goto L22004;}
 L26876: {
  addr_1125X = (((char *) (-3 + arg2_725X))) + (((offset_727X)<<2));
  S48_WRITE_BARRIER(arg2_725X, addr_1125X, value_726X);
  *((long *) addr_1125X) = value_726X;
  goto L26885;}
 L26885: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L26886: {
  push_exception_continuationB(5, 4);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_725X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_728X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_727X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_726X;
  arg0K0 = 4;
  goto L22004;}
 L27103: {
  push_exception_continuationB(5, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_732X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_733X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = init_731X;
  arg0K0 = 3;
  goto L22004;}
 L27120: {
  x_1126X = arg0K0;
  value_1127X = Stemp0S;
  Stemp0S = 1;
  if ((1 == x_1126X)) {
    push_exception_continuationB(8, 2);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((type_732X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_733X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = value_1127X;
    arg0K0 = 3;
    goto L22004;}
  else {
    arg0K0 = (-1 + len_733X);
    goto L27146;}}
 L27393: {
  push_exception_continuationB(7, 3);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_736X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_738X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_737X;
  arg0K0 = 3;
  goto L22004;}
 L27383: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg2_736X))) + (-4 & index_737X))));
  goto L27392;}
 L27392: {
  value_1128X = arg0K0;
  SvalS = value_1128X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L27348: {
  push_exception_continuationB(5, 3);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_736X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_738X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = index_737X;
  arg0K0 = 3;
  goto L22004;}
 L27680: {
  push_exception_continuationB(7, 3);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_744X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_746X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_743X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_745X;
  arg0K0 = 4;
  goto L22004;}
 L27670: {
  addr_1129X = (((char *) (-3 + arg3_744X))) + (-4 & arg2_743X);
  S48_WRITE_BARRIER(arg3_744X, addr_1129X, value_745X);
  *((long *) addr_1129X) = value_745X;
  goto L27679;}
 L27679: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L27633: {
  push_exception_continuationB(5, 3);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_744X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_746X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_743X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_745X;
  arg0K0 = 4;
  goto L22004;}
 L35531: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_751X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_752X)<<2));
  arg0K0 = 2;
  goto L22004;}
 L35544: {
  vector_1130X = arg0K0;
  if ((1 == vector_1130X)) {
    push_exception_continuationB(8, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_751X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((init_752X)<<2));
    arg0K0 = 2;
    goto L22004;}
  else {
    arg0K0 = (-1 + len_751X);
    goto L35566;}}
 L46170: {
  push_exception_continuationB(5, 1);
  x_1131X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1131X;
  arg0K0 = 1;
  goto L22004;}
 L42788: {
  push_exception_continuationB(7, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_756X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_757X)<<2));
  arg0K0 = 2;
  goto L22004;}
 L45665: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_756X;
  x_1132X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1132X;
  arg0K0 = 2;
  goto L22004;}
 L40109: {
  push_exception_continuationB(7, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_760X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_761X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_762X)<<2));
  arg0K0 = 3;
  goto L22004;}
 L40088: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_760X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_761X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_762X)<<2));
  arg0K0 = 3;
  goto L22004;}
 L44397: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_760X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_759X;
  x_1133X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1133X;
  arg0K0 = 3;
  goto L22004;}
 L35711: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_765X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_766X))<<8)));
  arg0K0 = 2;
  goto L22004;}
 L35724: {
  vector_1134X = arg0K0;
  if ((1 == vector_1134X)) {
    push_exception_continuationB(8, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_765X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (9 + ((((init_766X))<<8)));
    arg0K0 = 2;
    goto L22004;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1134X))) + len_765X)) = 0;
    arg0K0 = (-1 + len_765X);
    goto L35746;}}
 L42869: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_764X;
  x_1135X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1135X;
  arg0K0 = 2;
  goto L22004;}
 L46220: {
  push_exception_continuationB(5, 1);
  x_1136X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1136X;
  arg0K0 = 1;
  goto L22004;}
 L42934: {
  push_exception_continuationB(7, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_770X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_771X)<<2));
  arg0K0 = 2;
  goto L22004;}
 L45763: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_770X;
  x_1137X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1137X;
  arg0K0 = 2;
  goto L22004;}
 L40264: {
  push_exception_continuationB(7, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_774X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_775X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_776X))<<8)));
  arg0K0 = 3;
  goto L22004;}
 L40243: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_774X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_775X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_776X))<<8)));
  arg0K0 = 3;
  goto L22004;}
 L44527: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_774X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_773X;
  x_1138X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1138X;
  arg0K0 = 3;
  goto L22004;}
 L20854: {
  i_1139X = arg0K0;
  h_1140X = arg0K1;
  if ((i_1139X < n_781X)) {
    arg0K0 = (1 + i_1139X);
    arg0K1 = (h_1140X + (((*((unsigned char *) ((((char *) (-3 + string_780X))) + i_1139X))))));
    goto L20854;}
  else {
    index_1141X = 1023 & h_1140X;
    link_1142X = *((long *) ((((char *) (-3 + table_779X))) + (((index_1141X)<<2))));
    if ((0 == (3 & link_1142X))) {
      arg0K0 = (3 + (-4 & link_1142X));
      goto L20810;}
    else {
      arg0K0 = link_1142X;
      goto L20810;}}}
 L31856: {
  push_exception_continuationB(5, 1);
  x_1143X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1143X;
  arg0K0 = 1;
  goto L22004;}
 L44197: {
  val_1144X = arg0K0;
  SvalS = val_1144X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L44178: {
  push_exception_continuationB(5, 1);
  x_1145X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1145X;
  arg0K0 = 1;
  goto L22004;}
 L39940: {
  if ((1 == (SvalS))) {
    addr_1146X = (((char *) (-3 + arg2_785X))) + 4;
    S48_WRITE_BARRIER(arg2_785X, addr_1146X, 273);
    *((long *) addr_1146X) = 273;
    goto L39946;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_785X))) + 4)))))) {
      addr_1147X = (((char *) (-3 + arg2_785X))) + 4;
      S48_WRITE_BARRIER(arg2_785X, addr_1147X, 529);
      *((long *) addr_1147X) = 529;
      goto L39946;}
    else {
      goto L39946;}}}
 L39947: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_785X;
  x_1148X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1148X;
  arg0K0 = 2;
  goto L22004;}
 L48921: {
  val_1149X = arg0K0;
  SvalS = val_1149X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L43016: {
  SvalS = x_788X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L36118: {
  if ((0 == (3 & arg2_789X))) {
    if (((((arg2_789X)>>2)) < 0)) {
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_789X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_790X)<<2));
      arg0K0 = 2;
      goto L22004;}
    else {
      arg0K0 = (((arg2_789X)>>2));
      goto L35938;}}
  else {
    if ((3 == (3 & arg2_789X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_789X))) + -4))))>>2))))) {
        if ((1 == mode_790X)) {
          goto L35996;}
        else {
          if ((3 == mode_790X)) {
            goto L35996;}
          else {
            v_1150X = ps_open_fd((((char *)(((char *) (-3 + arg2_789X))))), 0, &v_1151X);
            arg0K0 = v_1150X;
            arg0K1 = v_1151X;
            goto L36009;}}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_789X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_790X)<<2));
        arg0K0 = 2;
        goto L22004;}}
    else {
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_789X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_790X)<<2));
      arg0K0 = 2;
      goto L22004;}}}
 L48957: {
  push_exception_continuationB(5, 1);
  x_1152X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1152X;
  arg0K0 = 1;
  goto L22004;}
 L43143: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L43153;}
  else {
    arg2K0 = 1;
    goto L43153;}}
 L43154: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_799X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_798X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_797X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_796X;
  x_1153X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1153X;
  arg0K0 = 5;
  goto L22004;}
 L36757: {
  if ((3 == (3 & arg3_802X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_802X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_802X))) + -4))))>>8)));
      goto L36759;}
    else {
      goto L36946;}}
  else {
    goto L36946;}}
 L36932: {
  if ((3 == (3 & arg3_802X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg3_802X))) + -4))))>>2))))) {
      goto L36757;}
    else {
      goto L36811;}}
  else {
    goto L36811;}}
 L36811: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_803X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_802X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_804X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_805X)<<2));
  arg0K0 = 4;
  goto L22004;}
 L43357: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_803X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_802X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_801X;
  x_1154X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1154X;
  arg0K0 = 4;
  goto L22004;}
 L39531: {
  val_1155X = arg0K0;
  SvalS = val_1155X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L48981: {
  push_exception_continuationB(5, 1);
  x_1156X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1156X;
  arg0K0 = 1;
  goto L22004;}
 L43505: {
  val_1157X = arg0K0;
  SvalS = val_1157X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L10983: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_813X))) + 16)));
  goto L43505;}
 L10989: {
  ch_1158X = arg0K0;
  prev_1159X = arg0K1;
  if ((1 == ch_1158X)) {
    addr_1160X = (((char *) (-3 + channel_813X))) + 16;
    S48_WRITE_BARRIER(channel_813X, addr_1160X, 1);
    *((long *) addr_1160X) = 1;
    n_1161X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_813X))) + 8))))>>2)));
    arg0K0 = (((n_1161X)<<2));
    goto L43505;}
  else {
    if ((ch_1158X == channel_813X)) {
      y_1162X = Spending_channels_tailS;
      if ((ch_1158X == y_1162X)) {
        Spending_channels_tailS = prev_1159X;
        goto L11013;}
      else {
        goto L11013;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_1158X))) + 12)));
      arg0K1 = ch_1158X;
      goto L10989;}}}
 L43488: {
  push_exception_continuationB(5, 1);
  x_1163X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1163X;
  arg0K0 = 1;
  goto L22004;}
 L16369: {
  i_1164X = arg0K0;
  res_1165X = arg0K1;
  if ((-1 == i_1164X)) {
    SvalS = res_1165X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    channel_1166X = *((Svm_channelsS) + i_1164X);
    if ((3 == (3 & channel_1166X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + channel_1166X))) + -4))))>>2))))) {
        addr_1167X = s48_allocate_small(12);
        *((long *) addr_1167X) = 2050;
        x_1168X = 3 + (((long) (addr_1167X + 4)));
        *((long *) (((char *) (-3 + x_1168X)))) = channel_1166X;
        *((long *) ((((char *) (-3 + x_1168X))) + 4)) = res_1165X;
        arg0K0 = x_1168X;
        goto L16383;}
      else {
        arg0K0 = res_1165X;
        goto L16383;}}
    else {
      arg0K0 = res_1165X;
      goto L16383;}}}
 L37065: {
  old_1169X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  if ((1 == old_1169X)) {
    goto L37081;}
  else {
    addr_1170X = ((char *) (-3 + old_1169X));
    S48_WRITE_BARRIER(old_1169X, addr_1170X, 1);
    *((long *) addr_1170X) = 1;
    goto L37081;}}
 L37096: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proposal_820X;
  arg0K0 = 1;
  goto L22004;}
 L9864: {
  i_1171X = arg0K0;
  stob_1172X = *((long *) ((((char *) (-3 + log_822X))) + (((i_1171X)<<2))));
  if ((1 == stob_1172X)) {
    log_1173X = *((long *) ((((char *) (-3 + proposal_821X))) + 8));
    arg0K0 = 0;
    goto L10102;}
  else {
    verify_1174X = *((long *) ((((char *) (-3 + log_822X))) + (12 + (((i_1171X)<<2)))));
    value_1175X = *((long *) ((((char *) (-3 + log_822X))) + (8 + (((i_1171X)<<2)))));
    if ((29 == verify_1174X)) {
      if ((3 == (3 & stob_1172X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1172X))) + -4)))))) {
          goto L9916;}
        else {
          goto L37327;}}
      else {
        goto L37327;}}
    else {
      if ((verify_1174X == (*((long *) ((((char *) (-3 + stob_1172X))) + (-4 & (*((long *) ((((char *) (-3 + log_822X))) + (4 + (((i_1171X)<<2)))))))))))) {
        if ((verify_1174X == value_1175X)) {
          goto L9916;}
        else {
          if ((3 == (3 & stob_1172X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1172X))) + -4)))))) {
              goto L9916;}
            else {
              goto L37327;}}
          else {
            goto L37327;}}}
      else {
        goto L37327;}}}}
 L27978: {
  value_1176X = arg0K0;
  SvalS = value_1176X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L27979: {
  push_exception_continuationB(5, 3);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = stob_823X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_825X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_824X)<<2));
  arg0K0 = 3;
  goto L22004;}
 L28072: {
  push_exception_continuationB(5, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_831X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_832X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_829X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_833X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_834X)<<2));
  arg0K0 = 5;
  goto L22004;}
 L28132: {
  memcpy((void *)((((char *) (-3 + arg3_829X))) + to_index_833X), (void *)((((char *) (-3 + arg5_831X))) + from_index_832X),count_834X);
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L28220: {
  left_1177X = arg0K0;
  copies_1178X = arg0K1;
  if ((1 == copies_1178X)) {
    if ((left_1177X < count_834X)) {
      goto L28072;}
    else {
      from_index_1179X = ((from_index_832X)<<2);
      to_index_1180X = ((to_index_833X)<<2);
      count_1181X = ((count_834X)<<2);
      Stemp0S = arg5_831X;
      Stemp1S = arg3_829X;
      addr_1182X = s48_allocate_tracedAgc(28);
      if ((addr_1182X == NULL)) {
        arg0K0 = 1;
        goto L9585;}
      else {
        *((long *) addr_1182X) = 6154;
        arg0K0 = (3 + (((long) (addr_1182X + 4))));
        goto L9585;}}}
  else {
    arg0K0 = (left_1177X - ((((*((long *) ((((char *) (-3 + copies_1178X))) + 16))))>>2)));
    arg0K1 = (*((long *) ((((char *) (-3 + copies_1178X))) + 20)));
    goto L28220;}}
 L28429: {
  push_exception_continuationB(7, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_839X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_840X)<<2));
  arg0K0 = 2;
  goto L22004;}
 L28428: {
  value_1183X = arg0K0;
  SvalS = value_1183X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L14940: {
  i_1184X = arg0K0;
  next_stob_1185X = *((long *) ((((char *) (-3 + log_844X))) + (((i_1184X)<<2))));
  if ((1 == next_stob_1185X)) {
    v_1186X = add_log_entryAgc(2, i_1184X, arg2_839X, index_843X, ((((*((unsigned char *) ((((char *) (-3 + arg2_839X))) + (((index_843X)>>2))))))<<2)), 1);
    arg0K0 = v_1186X;
    goto L28428;}
  else {
    if ((arg2_839X == next_stob_1185X)) {
      if ((index_843X == (*((long *) ((((char *) (-3 + log_844X))) + (4 + (((i_1184X)<<2)))))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + log_844X))) + (8 + (((i_1184X)<<2))))));
        goto L28428;}
      else {
        goto L14962;}}
    else {
      goto L14962;}}}
 L31146: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_839X;
  x_1187X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1187X;
  arg0K0 = 2;
  goto L22004;}
 L28555: {
  push_exception_continuationB(7, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_846X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_847X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_848X;
  arg0K0 = 3;
  goto L22004;}
 L28554: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L15116: {
  i_1188X = arg0K0;
  next_stob_1189X = *((long *) ((((char *) (-3 + log_852X))) + (((i_1188X)<<2))));
  if ((1 == next_stob_1189X)) {
    add_log_entryAgc(2, i_1188X, arg3_846X, index_851X, byte_848X, 0);
    goto L28554;}
  else {
    if ((arg3_846X == next_stob_1189X)) {
      if ((index_851X == (*((long *) ((((char *) (-3 + log_852X))) + (4 + (((i_1188X)<<2)))))))) {
        addr_1190X = (((char *) (-3 + log_852X))) + (8 + (((i_1188X)<<2)));
        S48_WRITE_BARRIER(log_852X, addr_1190X, byte_848X);
        *((long *) addr_1190X) = byte_848X;
        goto L28554;}
      else {
        goto L15136;}}
    else {
      goto L15136;}}}
 L28520: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_846X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_847X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = byte_848X;
  arg0K0 = 3;
  goto L22004;}
 L31251: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_846X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_845X;
  x_1191X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1191X;
  arg0K0 = 3;
  goto L22004;}
 L40492: {
  reason_1192X = arg0K0;
  status_1193X = arg0K1;
  push_exception_continuationB(reason_1192X, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_856X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_855X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_854X;
  merged_arg0K0 = status_1193X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 2;
  goto get_error_string;
 get_error_string_return_2:
  x_1194X = get_error_string0_return_value;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1194X;
  arg0K0 = 4;
  goto L22004;}
 L44728: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_856X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_855X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_854X;
  x_1195X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1195X;
  arg0K0 = 4;
  goto L22004;}
 L45409: {
  i_1196X = arg0K0;
  h_1197X = arg0K1;
  if ((i_1196X < n_868X)) {
    arg0K0 = (1 + i_1196X);
    arg0K1 = (h_1197X + (((*((unsigned char *) ((((char *) (-3 + x_867X))) + i_1196X))))));
    goto L45409;}
  else {
    SvalS = (((h_1197X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}}
 L45386: {
  push_exception_continuationB(5, 1);
  x_1198X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1198X;
  arg0K0 = 1;
  goto L22004;}
 L40767: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_869X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_870X;
  arg0K0 = 2;
  goto L22004;}
 L43624: {
  firstP_1199X = arg2K0;
  vector_1200X = s48_find_all(type_883X);
  if ((1 == vector_1200X)) {
    if (firstP_1199X) {
      s48_collect();
      arg2K0 = 0;
      goto L43624;}
    else {
      push_exception_continuationB(8, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((type_883X)<<2));
      arg0K0 = 1;
      goto L22004;}}
  else {
    SvalS = vector_1200X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}}
 L49089: {
  firstP_1201X = arg2K0;
  type_1202X = arg0K1;
  vector_1203X = s48_find_all_records(type_1202X);
  if ((1 == vector_1203X)) {
    if (firstP_1201X) {
      Stemp0S = type_1202X;
      s48_collect();
      value_1204X = Stemp0S;
      Stemp0S = 1;
      arg2K0 = 0;
      arg0K1 = value_1204X;
      goto L49089;}
    else {
      push_exception_continuationB(8, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = type_1202X;
      arg0K0 = 1;
      goto L22004;}}
  else {
    SvalS = vector_1203X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}}
 L25661: {
  push_exception_continuationB(5, 1);
  x_1205X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1205X;
  arg0K0 = 1;
  goto L22004;}
 L24441: {
  push_exception_continuationB(5, 1);
  x_1206X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1206X;
  arg0K0 = 1;
  goto L22004;}
 L49137: {
  SvalS = (((old_895X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L23972: {
  proposal_1207X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  x_1208X = Scurrent_threadS;
  addr_1209X = (((char *) (-3 + x_1208X))) + 12;
  S48_WRITE_BARRIER(x_1208X, addr_1209X, proposal_1207X);
  *((long *) addr_1209X) = proposal_1207X;
  pc_1210X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tem_1211X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  StemplateS = tem_1211X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1211X)))))))) + (((pc_1210X)>>2)));
  v_1212X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  SvalS = v_1212X;
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L46357: {
  x_1213X = s48_schedule_alarm_interrupt((((p_898X)>>2)));
  SvalS = (((x_1213X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L47576: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L47580;}
  else {
    arg2K0 = 1;
    goto L47580;}}
 L47581: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_900X;
  x_1214X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1214X;
  arg0K0 = 2;
  goto L22004;}
 L28754: {
  rest_list_1215X = arg0K0;
  if ((25 == rest_list_1215X)) {
    proc_1216X = *((long *) ((SstackS) + (-4 + (((nargs_903X)<<2)))));
    name_1217X = *((long *) ((SstackS) + (-8 + (((nargs_903X)<<2)))));
    args_1218X = SstackS;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (10 + (((nargs_903X)<<10)));
    if ((3 == (3 & name_1217X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + name_1217X))) + -4))))>>2))))) {
        if ((3 == (3 & proc_1216X))) {
          if ((18 == (31 & ((((*((long *) ((((char *) (-3 + proc_1216X))) + -4))))>>2))))) {
            if ((4 == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proc_1216X))) + -4))))>>8)))) {
              result_1219X = s48_external_call(proc_1216X, name_1217X, (-2 + nargs_903X), args_1218X);
              if ((Sexternal_exceptionPS)) {
                Sexternal_exceptionPS = 0;
                arg0K0 = (Sexternal_exception_nargsS);
                goto L22004;}
              else {
                SvalS = result_1219X;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L23956;}}
            else {
              goto L29026;}}
          else {
            goto L29026;}}
        else {
          goto L29026;}}
      else {
        goto L29026;}}
    else {
      goto L29026;}}
  else {
    x_1220X = *((long *) (((char *) (-3 + rest_list_1215X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1220X;
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_1215X))) + 4)));
    goto L28754;}}
 L43719: {
  if ((1 == (SvalS))) {
    v_1221X = Hlookup2720((Sexported_bindingsS), arg2_907X, 0);
    arg0K0 = v_1221X;
    goto L43760;}
  else {
    v_1222X = Hlookup2701((Simported_bindingsS), arg2_907X, 0);
    arg0K0 = v_1222X;
    goto L43760;}}
 L43724: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_907X;
  x_1223X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1223X;
  arg0K0 = 2;
  goto L22004;}
 L30464: {
  if ((1 == (SvalS))) {
    arg0K0 = (Sexported_bindingsS);
    goto L30499;}
  else {
    arg0K0 = (Simported_bindingsS);
    goto L30499;}}
 L30469: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_909X;
  x_1224X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1224X;
  arg0K0 = 2;
  goto L22004;}
 L45320: {
  option_1225X = arg0K0;
  seconds_1226X = arg0K1;
  mseconds_1227X = arg0K2;
  if ((536869 < seconds_1226X)) {
    push_exception_continuationB(6, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((option_1225X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_1226X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_1227X)<<2));
    arg0K0 = 3;
    goto L22004;}
  else {
    SvalS = (((((1000 * seconds_1226X) + mseconds_1227X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}}
 L49201: {
  s48_Scallback_return_stack_blockS = arg2_925X;
  return x_926X;}
 L38119: {
  val_1228X = arg0K0;
  SvalS = val_1228X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L38084: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_927X;
  x_1229X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1229X;
  arg0K0 = 2;
  goto L22004;}
 L37811: {
  len_1230X = 1 + n_932X;
  addr_1231X = s48_allocate_small((4 + len_1230X));
  *((long *) addr_1231X) = (70 + (((len_1230X)<<8)));
  string_1232X = 3 + (((long) (addr_1231X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1232X))) + n_932X)) = 0;
  arg0K0 = arg2_931X;
  arg0K1 = (-1 + n_932X);
  goto L37788;}
 L37763: {
  if ((25 == arg2_931X)) {
    goto L37811;}
  else {
    push_exception_continuationB(5, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = arg2_931X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_932X)<<2));
    arg0K0 = 2;
    goto L22004;}}
 L39594: {
  list_1233X = arg0K0;
  slow_1234X = arg0K1;
  move_slowP_1235X = arg2K2;
  if ((25 == list_1233X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    if ((3 == (3 & list_1233X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_1233X))) + -4))))>>2))))) {
        head_1236X = *((long *) (((char *) (-3 + list_1233X))));
        if ((3 == (3 & head_1236X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_1236X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_1236X))))) == arg2_934X)) {
              SvalS = head_1236X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L23956;}
            else {
              list_1237X = *((long *) ((((char *) (-3 + list_1233X))) + 4));
              if ((list_1237X == slow_1234X)) {
                push_exception_continuationB(5, 1);
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = arg2_934X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = list_935X;
                arg0K0 = 2;
                goto L22004;}
              else {
                if (move_slowP_1235X) {
                  arg0K0 = list_1237X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_1234X))) + 4)));
                  arg2K2 = 0;
                  goto L39594;}
                else {
                  arg0K0 = list_1237X;
                  arg0K1 = slow_1234X;
                  arg2K2 = 1;
                  goto L39594;}}}}
          else {
            push_exception_continuationB(5, 1);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = arg2_934X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = list_935X;
            arg0K0 = 2;
            goto L22004;}}
        else {
          push_exception_continuationB(5, 1);
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = arg2_934X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = list_935X;
          arg0K0 = 2;
          goto L22004;}}
      else {
        push_exception_continuationB(5, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_934X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = list_935X;
        arg0K0 = 2;
        goto L22004;}}
    else {
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_934X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = list_935X;
      arg0K0 = 2;
      goto L22004;}}}
 L29102: {
  push_exception_continuationB(7, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_937X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_936X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_938X)<<2));
  arg0K0 = 3;
  goto L22004;}
 L29092: {
  arg0K0 = (*((long *) ((((char *) (-3 + arg3_937X))) + (((index_938X)<<2)))));
  goto L29101;}
 L29101: {
  value_1238X = arg0K0;
  SvalS = value_1238X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L29122: {
  push_exception_continuationB(5, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_937X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_936X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_938X)<<2));
  arg0K0 = 3;
  goto L22004;}
 L29360: {
  push_exception_continuationB(7, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_945X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_944X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_946X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_947X;
  arg0K0 = 4;
  goto L22004;}
 L29350: {
  addr_1239X = (((char *) (-3 + arg4_945X))) + (((index_946X)<<2));
  S48_WRITE_BARRIER(arg4_945X, addr_1239X, value_947X);
  *((long *) addr_1239X) = value_947X;
  goto L29359;}
 L29359: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L29382: {
  push_exception_continuationB(5, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_945X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_944X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_946X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = value_947X;
  arg0K0 = 4;
  goto L22004;}
 L29639: {
  port_1240X = arg0K0;
  if ((3 == (3 & port_1240X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1240X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1240X))) + 4))))>>2))))) {
        goto L29696;}
      else {
        b_1241X = *((long *) ((((char *) (-3 + port_1240X))) + 16));
        if ((1 == b_1241X)) {
          push_exception_continuationB(13, 2);
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = port_1240X;
          arg0K0 = 1;
          goto L22004;}
        else {
          p_1242X = *((long *) ((((char *) (-3 + port_1240X))) + 24));
          i_1243X = (((*((long *) ((((char *) (-3 + port_1240X))) + 20))))>>2);
          if ((i_1243X == (((p_1242X)>>2)))) {
            push_exception_continuationB(13, 2);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1240X;
            arg0K0 = 1;
            goto L22004;}
          else {
            val_1244X = 4 + (((i_1243X)<<2));
            addr_1245X = (((char *) (-3 + port_1240X))) + 20;
            S48_WRITE_BARRIER(port_1240X, addr_1245X, val_1244X);
            *((long *) addr_1245X) = val_1244X;
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1241X))) + i_1243X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L23956;}}}}
    else {
      goto L29696;}}
  else {
    goto L29696;}}
 L29841: {
  port_1246X = arg0K0;
  if ((3 == (3 & port_1246X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1246X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_1246X))) + 4))))>>2))))) {
        goto L29898;}
      else {
        b_1247X = *((long *) ((((char *) (-3 + port_1246X))) + 16));
        if ((1 == b_1247X)) {
          push_exception_continuationB(13, 2);
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = port_1246X;
          arg0K0 = 1;
          goto L22004;}
        else {
          p_1248X = *((long *) ((((char *) (-3 + port_1246X))) + 24));
          i_1249X = (((*((long *) ((((char *) (-3 + port_1246X))) + 20))))>>2);
          if ((i_1249X == (((p_1248X)>>2)))) {
            push_exception_continuationB(13, 2);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1246X;
            arg0K0 = 1;
            goto L22004;}
          else {
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_1247X))) + i_1249X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L23956;}}}}
    else {
      goto L29898;}}
  else {
    goto L29898;}}
 L30036: {
  Kchar_1250X = arg0K0;
  port_1251X = arg0K1;
  if ((9 == (255 & Kchar_1250X))) {
    if ((3 == (3 & port_1251X))) {
      if ((7 == (31 & ((((*((long *) ((((char *) (-3 + port_1251X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_1251X))) + 4))))>>2))))) {
          goto L30096;}
        else {
          b_1252X = *((long *) ((((char *) (-3 + port_1251X))) + 16));
          if ((1 == b_1252X)) {
            push_exception_continuationB(13, 2);
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = Kchar_1250X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = port_1251X;
            arg0K0 = 2;
            goto L22004;}
          else {
            i_1253X = (((*((long *) ((((char *) (-3 + port_1251X))) + 20))))>>2);
            if ((i_1253X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_1252X))) + -4))))>>8)))) {
              push_exception_continuationB(13, 2);
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = Kchar_1250X;
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = port_1251X;
              arg0K0 = 2;
              goto L22004;}
            else {
              val_1254X = 4 + (((i_1253X)<<2));
              addr_1255X = (((char *) (-3 + port_1251X))) + 20;
              S48_WRITE_BARRIER(port_1251X, addr_1255X, val_1254X);
              *((long *) addr_1255X) = val_1254X;
              *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_1251X))) + 16)))))) + i_1253X)) = (((((Kchar_1250X)>>8))));
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L23956;}}}}
      else {
        goto L30096;}}
    else {
      goto L30096;}}
  else {
    goto L30096;}}
 L39847: {
  stuff_1256X = arg0K0;
  if ((3 == (3 & stuff_1256X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_1256X))) + -4))))>>2))))) {
      thing_1257X = *((long *) (((char *) (-3 + stuff_1256X))));
      if ((0 == (3 & thing_1257X))) {
        ps_write_integer((((thing_1257X)>>2)), out_960X);
        goto L39853;}
      else {
        if ((9 == (255 & thing_1257X))) {
          ps_write_string("#\\", out_960X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_1257X)>>8))), out_960X, ignoreXX) }
          goto L39853;}
        else {
          if ((3 == (3 & thing_1257X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thing_1257X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_1257X))) + -4))))>>8))))>>2)))) {
                type_1258X = *((long *) (((char *) (-3 + thing_1257X))));
                if ((3 == (3 & type_1258X))) {
                  if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_1258X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_1258X))) + -4))))>>8))))>>2)))) {
                      obj_1259X = *((long *) ((((char *) (-3 + type_1258X))) + 8));
                      if ((3 == (3 & obj_1259X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_1259X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_960X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_1257X)))))))) + 8))))))))))))), out_960X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_960X, ignoreXX) }
                          goto L39853;}
                        else {
                          goto L15718;}}
                      else {
                        goto L15718;}}
                    else {
                      goto L15718;}}
                  else {
                    goto L15718;}}
                else {
                  goto L15718;}}
              else {
                goto L15718;}}
            else {
              goto L15718;}}
          else {
            goto L15718;}}}}
    else {
      goto L39838;}}
  else {
    goto L39838;}}
 L22600: {
  cont_1260X = arg0K0;
  if ((1 == cont_1260X)) {
    if ((0 == (3 & (SvalS)))) {
      s48_Scallback_return_stack_blockS = 1;
      return ((((SvalS))>>2));}
    else {
      goto L22615;}}
  else {
    goto L22615;}}
 L25365: {
  Scode_pointerS = ((Scode_pointerS) + (1 + bytes_used_508X));
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L22271: {
  arg_count_1261X = arg0K0;
  handlers_1262X = SHARED_REF((Sinterrupt_handlersS));
  if ((3 == (3 & handlers_1262X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + handlers_1262X))) + -4))))>>2))))) {
      goto L22289;}
    else {
      goto L22414;}}
  else {
    goto L22414;}}
 L11174: {
  channel_1263X = arg0K0;
  x_1264X = 1 == (Spending_channels_headS);
  if (x_1264X) {
    goto L11188;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L11188;}
      else {
        goto L11188;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L11188;}}}
 L25897: {
  arg0K0 = (SenvS);
  arg0K1 = 0;
  goto L25856;}
 L26188: {
  i_1265X = arg0K0;
  offset_1266X = arg0K1;
  env_1267X = arg0K2;
  if ((i_1265X == total_count_550X)) {
    SvalS = new_env_553X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1266X));
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    back_1268X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1266X)));
    arg0K0 = env_1267X;
    arg0K1 = back_1268X;
    goto L26266;}}
 L26039: {
  i_1269X = arg0K0;
  offset_1270X = arg0K1;
  env_1271X = arg0K2;
  if ((i_1269X == total_count_557X)) {
    SvalS = new_env_560X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_1270X));
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    back_1272X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_1270X)));
    arg0K0 = env_1271X;
    arg0K1 = back_1272X;
    goto L26117;}}
 L46808: {
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L47695: {
  cont_1273X = ScontS;
  template_1274X = *((long *) (cont_1273X + 8));
  Scode_pointerS = (((char *) (*((long *) cont_1273X))));
  StemplateS = template_1274X;
  SenvS = (*((long *) (cont_1273X + 12)));
  ScontS = (((char *) (*((long *) (cont_1273X + 4)))));
  SstackS = (cont_1273X + 16);
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L47711: {
  SstackS = (ScontS);
  v_1275X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tag_1276X = s48_invoke_native_continuation((2 + v_1275X));
  if ((tag_1276X == 0)) {
    arg0K0 = (s48_Snative_protocolS);
    arg0K1 = 25;
    arg0K2 = 0;
    goto L21734;}
  else {
    if ((tag_1276X == 1)) {
      stack_arg_count_1277X = s48_Snative_protocolS;
      obj_1278X = SvalS;
      if ((3 == (3 & obj_1278X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1278X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_1277X;
          arg0K1 = 25;
          arg0K2 = 0;
          goto L47051;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_1277X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L22536;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_1277X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L22536;}}
    else {
      ps_error("unexpected native return value", 1, tag_1276X);
      return v_1279X;}}}
 L47735: {
  x_1280X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1280X;
  SvalS = consumer_1028X;
  arg0K0 = 1;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L47520;}
 L46591: {
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L13748: {
  if ((3 == (3 & x_620X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_620X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L48513;}
    else {
      goto L13754;}}
  else {
    goto L13754;}}
 L38679: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L38820: {
  if ((3 == (3 & n_622X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_622X))) + -4))))>>2))))) {
      goto L38831;}
    else {
      goto L38832;}}
  else {
    goto L38832;}}
 L39019: {
  if ((3 == (3 & n_623X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_623X))) + -4))))>>2))))) {
      goto L39030;}
    else {
      goto L39031;}}
  else {
    goto L39031;}}
 L39218: {
  if ((3 == (3 & n_624X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + n_624X))) + -4))))>>2))))) {
      goto L39229;}
    else {
      goto L39230;}}
  else {
    goto L39230;}}
 L31998: {
  if ((0 == (3 & arg2_628X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17801;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_628X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17801;}}
 L32015: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_628X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_629X;
  arg0K0 = 2;
  goto L22004;}
 L8208: {
  b_1281X = arg0K0;
  lo_a_1282X = 65535 & a_1060X;
  lo_b_1283X = 65535 & b_1281X;
  hi_a_1284X = 65535 & (((a_1060X)>>16));
  hi_b_1285X = 65535 & (((b_1281X)>>16));
  lo_c_1286X = SMALL_MULTIPLY(lo_a_1282X, lo_b_1283X);
  v_1287X = SMALL_MULTIPLY(lo_b_1283X, hi_a_1284X);
  v_1288X = SMALL_MULTIPLY(lo_a_1282X, hi_b_1285X);
  mid_c_1289X = v_1288X + v_1287X;
  c_1290X = lo_c_1286X + (((mid_c_1289X)<<16));
  if ((0 < hi_a_1284X)) {
    if ((0 < hi_b_1285X)) {
      arg0K0 = arg2_631X;
      arg0K1 = y_632X;
      goto L19587;}
    else {
      goto L8250;}}
  else {
    goto L8250;}}
 L40893: {
  arg0K0 = arg2_631X;
  arg0K1 = y_632X;
  goto L19587;}
 L40910: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_631X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_632X;
  arg0K0 = 2;
  goto L22004;}
 L32287: {
  if ((0 == (3 & arg2_635X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17758;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_635X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17758;}}
 L32304: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_635X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_636X;
  arg0K0 = 2;
  goto L22004;}
 L8466: {
  b_1291X = arg0K0;
  c_1292X = a_1076X / b_1291X;
  x_1293X = 0 == (a_1076X % b_1291X);
  if (x_1293X) {
    if ((a_640X < 0)) {
      if ((b_641X < 0)) {
        s48_make_availableAgc(16);
        if ((536870911 < c_1292X)) {
          goto L49965;}
        else {
          if ((c_1292X < -536870912)) {
            goto L49965;}
          else {
            arg0K0 = (((c_1292X)<<2));
            goto L49960;}}}
      else {
        goto L8512;}}
    else {
      if ((b_641X < 0)) {
        goto L8512;}
      else {
        s48_make_availableAgc(16);
        if ((536870911 < c_1292X)) {
          goto L49987;}
        else {
          if ((c_1292X < -536870912)) {
            goto L49987;}
          else {
            arg0K0 = (((c_1292X)<<2));
            goto L49982;}}}}}
  else {
    push_exception_continuationB(5, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = arg2_638X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_639X;
    arg0K0 = 2;
    goto L22004;}}
 L41146: {
  if ((0 == (3 & arg2_638X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17326;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_638X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17326;}}
 L41186: {
  result_1294X = arg0K0;
  if ((1 == result_1294X)) {
    push_exception_continuationB(5, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = arg2_638X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_639X;
    arg0K0 = 2;
    goto L22004;}
  else {
    SvalS = result_1294X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}}
 L41193: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_638X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_639X;
  arg0K0 = 2;
  goto L22004;}
 L32572: {
  if ((0 == (3 & (arg2_642X | y_643X)))) {
    if ((arg2_642X == y_643X)) {
      arg0K0 = 5;
      goto L32576;}
    else {
      arg0K0 = 1;
      goto L32576;}}
  else {
    if ((3 == (3 & arg2_642X))) {
      if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_642X))) + -4))))>>2))))) {
        if ((3 == (3 & y_643X))) {
          if ((20 == (31 & ((((*((long *) ((((char *) (-3 + y_643X))) + -4))))>>2))))) {
            b_1295X = s48_bignum_equal_p((((char *) (-3 + arg2_642X))), (((char *) (-3 + y_643X))));
            if (b_1295X) {
              arg0K0 = 5;
              goto L32576;}
            else {
              arg0K0 = 1;
              goto L32576;}}
          else {
            arg0K0 = 1;
            goto L32576;}}
        else {
          arg0K0 = 1;
          goto L32576;}}
      else {
        arg0K0 = 1;
        goto L32576;}}
    else {
      arg0K0 = 1;
      goto L32576;}}}
 L32592: {
  val_1296X = arg0K0;
  SvalS = val_1296X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L32593: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_642X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_643X;
  arg0K0 = 2;
  goto L22004;}
 L32832: {
  if ((0 == (3 & arg2_644X))) {
    if ((0 == (3 & y_645X))) {
      if ((arg2_644X < y_645X)) {
        arg0K0 = 5;
        goto L32836;}
      else {
        arg0K0 = 1;
        goto L32836;}}
    else {
      v_1297X = s48_bignum_test((((char *) (-3 + y_645X))));
      if ((1 == v_1297X)) {
        arg0K0 = 5;
        goto L32836;}
      else {
        arg0K0 = 1;
        goto L32836;}}}
  else {
    if ((0 == (3 & y_645X))) {
      v_1298X = s48_bignum_test((((char *) (-3 + arg2_644X))));
      if ((1 == v_1298X)) {
        arg0K0 = 1;
        goto L32836;}
      else {
        arg0K0 = 5;
        goto L32836;}}
    else {
      v_1299X = s48_bignum_compare((((char *) (-3 + arg2_644X))), (((char *) (-3 + y_645X))));
      if ((-1 == v_1299X)) {
        arg0K0 = 5;
        goto L32836;}
      else {
        arg0K0 = 1;
        goto L32836;}}}}
 L32852: {
  val_1300X = arg0K0;
  SvalS = val_1300X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L32853: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_644X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_645X;
  arg0K0 = 2;
  goto L22004;}
 L33155: {
  if ((0 == (3 & y_647X))) {
    if ((0 == (3 & arg2_646X))) {
      if ((y_647X < arg2_646X)) {
        arg0K0 = 5;
        goto L33159;}
      else {
        arg0K0 = 1;
        goto L33159;}}
    else {
      v_1301X = s48_bignum_test((((char *) (-3 + arg2_646X))));
      if ((1 == v_1301X)) {
        arg0K0 = 5;
        goto L33159;}
      else {
        arg0K0 = 1;
        goto L33159;}}}
  else {
    if ((0 == (3 & arg2_646X))) {
      v_1302X = s48_bignum_test((((char *) (-3 + y_647X))));
      if ((1 == v_1302X)) {
        arg0K0 = 1;
        goto L33159;}
      else {
        arg0K0 = 5;
        goto L33159;}}
    else {
      v_1303X = s48_bignum_compare((((char *) (-3 + y_647X))), (((char *) (-3 + arg2_646X))));
      if ((-1 == v_1303X)) {
        arg0K0 = 5;
        goto L33159;}
      else {
        arg0K0 = 1;
        goto L33159;}}}}
 L33175: {
  val_1304X = arg0K0;
  SvalS = val_1304X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33176: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_646X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_647X;
  arg0K0 = 2;
  goto L22004;}
 L33478: {
  if ((0 == (3 & y_649X))) {
    if ((0 == (3 & arg2_648X))) {
      if ((y_649X < arg2_648X)) {
        arg0K0 = 1;
        goto L33482;}
      else {
        arg0K0 = 5;
        goto L33482;}}
    else {
      v_1305X = s48_bignum_test((((char *) (-3 + arg2_648X))));
      if ((1 == v_1305X)) {
        arg0K0 = 1;
        goto L33482;}
      else {
        arg0K0 = 5;
        goto L33482;}}}
  else {
    if ((0 == (3 & arg2_648X))) {
      v_1306X = s48_bignum_test((((char *) (-3 + y_649X))));
      if ((1 == v_1306X)) {
        arg0K0 = 5;
        goto L33482;}
      else {
        arg0K0 = 1;
        goto L33482;}}
    else {
      v_1307X = s48_bignum_compare((((char *) (-3 + y_649X))), (((char *) (-3 + arg2_648X))));
      if ((-1 == v_1307X)) {
        arg0K0 = 1;
        goto L33482;}
      else {
        arg0K0 = 5;
        goto L33482;}}}}
 L33498: {
  val_1308X = arg0K0;
  SvalS = val_1308X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33499: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_648X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_649X;
  arg0K0 = 2;
  goto L22004;}
 L33772: {
  if ((0 == (3 & arg2_650X))) {
    if ((0 == (3 & y_651X))) {
      if ((arg2_650X < y_651X)) {
        arg0K0 = 1;
        goto L33776;}
      else {
        arg0K0 = 5;
        goto L33776;}}
    else {
      v_1309X = s48_bignum_test((((char *) (-3 + y_651X))));
      if ((1 == v_1309X)) {
        arg0K0 = 1;
        goto L33776;}
      else {
        arg0K0 = 5;
        goto L33776;}}}
  else {
    if ((0 == (3 & y_651X))) {
      v_1310X = s48_bignum_test((((char *) (-3 + arg2_650X))));
      if ((1 == v_1310X)) {
        arg0K0 = 5;
        goto L33776;}
      else {
        arg0K0 = 1;
        goto L33776;}}
    else {
      v_1311X = s48_bignum_compare((((char *) (-3 + arg2_650X))), (((char *) (-3 + y_651X))));
      if ((-1 == v_1311X)) {
        arg0K0 = 1;
        goto L33776;}
      else {
        arg0K0 = 5;
        goto L33776;}}}}
 L33792: {
  val_1312X = arg0K0;
  SvalS = val_1312X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33793: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_650X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_651X;
  arg0K0 = 2;
  goto L22004;}
 L19526: {
  length0_1313X = arg0K0;
  extra0_1314X = arg0K1;
  if ((0 == (3 & y_1092X))) {
    arg0K0 = 3;
    goto L19528;}
  else {
    arg0K0 = 0;
    goto L19528;}}
 L9109: {
  b_1315X = arg0K0;
  c_1316X = a_1093X / b_1315X;
  if ((a_654X < 0)) {
    if ((b_655X < 0)) {
      goto L9155;}
    else {
      goto L9154;}}
  else {
    if ((b_655X < 0)) {
      goto L9154;}
    else {
      goto L9155;}}}
 L34072: {
  arg0K0 = arg2_652X;
  arg0K1 = y_653X;
  goto L19496;}
 L19435: {
  length0_1317X = arg0K0;
  extra0_1318X = arg0K1;
  if ((0 == (3 & y_1095X))) {
    arg0K0 = 3;
    goto L19437;}
  else {
    arg0K0 = 0;
    goto L19437;}}
 L34282: {
  b_1319X = arg0K0;
  c_1320X = a_1096X % b_1319X;
  if ((a_658X < 0)) {
    arg0K0 = (0 - c_1320X);
    goto L34286;}
  else {
    arg0K0 = c_1320X;
    goto L34286;}}
 L34248: {
  arg0K0 = arg2_656X;
  arg0K1 = y_657X;
  goto L19405;}
 L22815: {
  x_1321X = arg0K0;
  if ((536870911 < x_1321X)) {
    goto L22816;}
  else {
    if ((x_1321X < -536870912)) {
      goto L22816;}
    else {
      return (((x_1321X)<<2));}}}
 L19013: {
  length_1322X = arg0K0;
  extra_1323X = arg0K1;
  Stemp0S = x_1098X;
  s48_make_availableAgc(((((length_1322X + extra_1323X))<<2)));
  value_1324X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1324X))) {
    v_1325X = (char *) s48_long_to_bignum((((value_1324X)>>2)));
    arg1K0 = v_1325X;
    goto L19005;}
  else {
    arg1K0 = (((char *) (-3 + value_1324X)));
    goto L19005;}}
 L19360: {
  length_1326X = arg0K0;
  extra_1327X = arg0K1;
  if ((length_1326X < 1)) {
    arg0K0 = 1;
    goto L19362;}
  else {
    arg0K0 = length_1326X;
    goto L19362;}}
 L34905: {
  x_1328X = arg0K0;
  count_1329X = arg0K1;
  if ((0 == x_1328X)) {
    SvalS = (((count_1329X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    arg0K0 = (((x_1328X)>>1));
    arg0K1 = (count_1329X + (1 & x_1328X));
    goto L34905;}}
 L19302: {
  length_1330X = arg0K0;
  extra_1331X = arg0K1;
  if ((length_1330X < 1)) {
    arg0K0 = 1;
    goto L19304;}
  else {
    arg0K0 = length_1330X;
    goto L19304;}}
 L34990: {
  if ((0 == (3 & arg2_685X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17540;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_685X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17540;}}
 L35153: {
  if ((0 == (3 & arg2_687X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17497;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_687X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17497;}}
 L35316: {
  if ((0 == (3 & arg2_689X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17454;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_689X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17454;}}
 L41586: {
  v_1332X = (char *) s48_long_to_bignum(x_1106X);
  v_1333X = enter_bignum(v_1332X);
  arg0K0 = v_1333X;
  goto L41581;}
 L41581: {
  val_1334X = arg0K0;
  SvalS = val_1334X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L41608: {
  v_1335X = (char *) s48_long_to_bignum(result_1108X);
  v_1336X = enter_bignum(v_1335X);
  arg0K0 = v_1336X;
  goto L41603;}
 L41603: {
  val_1337X = arg0K0;
  SvalS = val_1337X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L41479: {
  x_1338X = arg0K0;
  y_1339X = arg0K1;
  y_1340X = ((y_1339X)>>2);
  merged_arg0K0 = x_1338X;
  merged_arg0K1 = y_1340X;
  shift_space_return_tag = 1;
  goto shift_space;
 shift_space_return_1:
  needed_1341X = shift_space0_return_value;
  Stemp0S = x_1338X;
  s48_make_availableAgc((((needed_1341X)<<2)));
  value_1342X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1342X))) {
    v_1343X = (char *) s48_long_to_bignum((((value_1342X)>>2)));
    arg1K0 = v_1343X;
    goto L41553;}
  else {
    arg1K0 = (((char *) (-3 + value_1342X)));
    goto L41553;}}
 L41630: {
  v_1344X = (char *) s48_long_to_bignum(result_1108X);
  v_1345X = enter_bignum(v_1344X);
  arg0K0 = v_1345X;
  goto L41625;}
 L41625: {
  val_1346X = arg0K0;
  SvalS = val_1346X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L41661: {
  x_1347X = arg1K0;
  external_bignum_1348X = (char *) s48_bignum_arithmetic_shift(x_1347X, y_1110X);
  v_1349X = s48_bignum_fits_in_word_p(external_bignum_1348X, 30, 1);
  if (v_1349X) {
    n_1350X = s48_bignum_to_long(external_bignum_1348X);
    arg0K0 = (((n_1350X)<<2));
    goto L41487;}
  else {
    v_1351X = enter_bignum(external_bignum_1348X);
    arg0K0 = v_1351X;
    goto L41487;}}
 L41488: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_691X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_692X;
  arg0K0 = 2;
  goto L22004;}
 L26733: {
  i_1352X = arg0K0;
  rest_list_1353X = arg0K1;
  if ((25 == rest_list_1353X)) {
    SvalS = new_718X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    *((long *) ((((char *) (-3 + new_718X))) + (((i_1352X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_1353X)))));
    arg0K0 = (1 + i_1352X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_1353X))) + 4)));
    goto L26733;}}
 L27146: {
  i_1354X = arg0K0;
  if ((i_1354X < 0)) {
    SvalS = x_1126X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    addr_1355X = (((char *) (-3 + x_1126X))) + (((i_1354X)<<2));
    S48_WRITE_BARRIER(x_1126X, addr_1355X, value_1127X);
    *((long *) addr_1355X) = value_1127X;
    arg0K0 = (-1 + i_1354X);
    goto L27146;}}
 L35566: {
  i_1356X = arg0K0;
  if ((i_1356X < 0)) {
    SvalS = vector_1130X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1130X))) + i_1356X)) = init_752X;
    arg0K0 = (-1 + i_1356X);
    goto L35566;}}
 L35746: {
  i_1357X = arg0K0;
  if ((i_1357X < 0)) {
    SvalS = vector_1134X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_1134X))) + i_1357X)) = (init_766X);
    arg0K0 = (-1 + i_1357X);
    goto L35746;}}
 L20810: {
  bucket_1358X = arg0K0;
  arg0K0 = bucket_1358X;
  goto L20816;}
 L39946: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L35938: {
  index_1359X = arg0K0;
  channel_1360X = make_registered_channel(mode_790X, arg2_789X, index_1359X, 0, &reason_1361X);
  if ((1 == channel_1360X)) {
    if ((3 == (3 & arg2_789X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_789X))) + -4))))>>2))))) {
        if ((1 == mode_790X)) {
          goto L36053;}
        else {
          if ((3 == mode_790X)) {
            goto L36053;}
          else {
            v_1362X = ps_close_fd(index_1359X);
            arg0K0 = v_1362X;
            goto L36048;}}}
      else {
        push_exception_continuationB(reason_1361X, 1);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_789X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_790X)<<2));
        arg0K0 = 2;
        goto L22004;}}
    else {
      push_exception_continuationB(reason_1361X, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_789X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_790X)<<2));
      arg0K0 = 2;
      goto L22004;}}
  else {
    SvalS = channel_1360X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}}
 L35996: {
  v_1363X = ps_open_fd((((char *)(((char *) (-3 + arg2_789X))))), 1, &v_1364X);
  arg0K0 = v_1363X;
  arg0K1 = v_1364X;
  goto L36009;}
 L36009: {
  channel_1365X = arg0K0;
  status_1366X = arg0K1;
  if ((status_1366X == NO_ERRORS)) {
    arg0K0 = channel_1365X;
    goto L35938;}
  else {
    if ((status_1366X == ENOENT)) {
      push_exception_continuationB(10, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_789X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_790X)<<2));
      arg0K0 = 2;
      goto L22004;}
    else {
      push_exception_continuationB(24, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg2_789X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_790X)<<2));
      merged_arg0K0 = status_1366X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_1367X = get_error_string0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1367X;
      arg0K0 = 3;
      goto L22004;}}}
 L43153: {
  waitP_1368X = arg2K0;
  start_1369X = ((arg3_797X)>>2);
  count_1370X = ((arg2_796X)>>2);
  v_1371X = 4 == (*((long *) (((char *) (-3 + arg5_799X)))));
  if (v_1371X) {
    if ((3 == (3 & arg4_798X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_798X))) + -4))))>>2))))) {
        goto L36380;}
      else {
        goto L36595;}}
    else {
      goto L36595;}}
  else {
    goto L36448;}}
 L36759: {
  y_1372X = arg0K0;
  if ((y_1372X < (start_804X + count_805X))) {
    goto L36811;}
  else {
    got_1373X = ps_write_fd(((((*((long *) ((((char *) (-3 + arg4_803X))) + 8))))>>2)), ((((char *) (-3 + arg3_802X))) + start_804X), count_805X, &pendingP_1374X, &status_1375X);
    if ((status_1375X == NO_ERRORS)) {
      if (pendingP_1374X) {
        arg0K0 = 1;
        goto L36810;}
      else {
        arg0K0 = (((got_1373X)<<2));
        goto L36810;}}
    else {
      push_exception_continuationB(24, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_803X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg3_802X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_804X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_805X)<<2));
      merged_arg0K0 = status_1375X;
      merged_arg0K1 = 0;
      get_error_string_return_tag = 4;
      goto get_error_string;
     get_error_string_return_4:
      x_1376X = get_error_string0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1376X;
      arg0K0 = 5;
      goto L22004;}}}
 L36946: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_802X))) + -4))))>>8));
  goto L36759;}
 L11013: {
  val_1377X = *((long *) ((((char *) (-3 + ch_1158X))) + 12));
  addr_1378X = (((char *) (-3 + prev_1159X))) + 12;
  S48_WRITE_BARRIER(prev_1159X, addr_1378X, val_1377X);
  *((long *) addr_1378X) = val_1377X;
  addr_1379X = (((char *) (-3 + ch_1158X))) + 12;
  S48_WRITE_BARRIER(ch_1158X, addr_1379X, 1);
  *((long *) addr_1379X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_1158X))) + 16)));
  goto L43505;}
 L16383: {
  v_1380X = arg0K0;
  arg0K0 = (-1 + i_1164X);
  arg0K1 = v_1380X;
  goto L16369;}
 L37081: {
  if ((1 == proposal_820X)) {
    goto L37093;}
  else {
    addr_1381X = ((char *) (-3 + proposal_820X));
    S48_WRITE_BARRIER(proposal_820X, addr_1381X, 5);
    *((long *) addr_1381X) = 5;
    goto L37093;}}
 L10102: {
  i_1382X = arg0K0;
  stob_1383X = *((long *) ((((char *) (-3 + log_1173X))) + (((i_1382X)<<2))));
  if ((1 == stob_1383X)) {
    copies_1384X = *((long *) ((((char *) (-3 + proposal_821X))) + 12));
    arg0K0 = copies_1384X;
    goto L37427;}
  else {
    verify_1385X = *((long *) ((((char *) (-3 + log_1173X))) + (12 + (((i_1382X)<<2)))));
    value_1386X = *((long *) ((((char *) (-3 + log_1173X))) + (8 + (((i_1382X)<<2)))));
    if ((29 == verify_1385X)) {
      if ((3 == (3 & stob_1383X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1383X))) + -4)))))) {
          goto L10154;}
        else {
          goto L37327;}}
      else {
        goto L37327;}}
    else {
      if ((verify_1385X == ((((*((unsigned char *) ((((char *) (-3 + stob_1383X))) + ((((*((long *) ((((char *) (-3 + log_1173X))) + (4 + (((i_1382X)<<2)))))))>>2))))))<<2)))) {
        if ((verify_1385X == value_1386X)) {
          goto L10154;}
        else {
          if ((3 == (3 & stob_1383X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + stob_1383X))) + -4)))))) {
              goto L10154;}
            else {
              goto L37327;}}
          else {
            goto L37327;}}}
      else {
        goto L37327;}}}}
 L9916: {
  arg0K0 = (4 + i_1171X);
  goto L9864;}
 L37327: {
  RELEASE_PROPOSAL_LOCK();
  x_1387X = Scurrent_threadS;
  addr_1388X = (((char *) (-3 + x_1387X))) + 12;
  S48_WRITE_BARRIER(x_1387X, addr_1388X, 1);
  *((long *) addr_1388X) = 1;
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L9585: {
  vector_1389X = arg0K0;
  if ((1 == vector_1389X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1390X;
    goto L9548;}
  else {
    arg0K0 = vector_1389X;
    goto L9548;}}
 L14962: {
  arg0K0 = (4 + i_1184X);
  goto L14940;}
 L15136: {
  arg0K0 = (4 + i_1188X);
  goto L15116;}
 L47580: {
  minutesP_1391X = arg2K0;
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      check_events_return_tag = 3;
      goto check_events;
     check_events_return_3:
      temp_1392X = check_events0_return_value;
      if (temp_1392X) {
        goto L47608;}
      else {
        goto L47613;}}
    else {
      goto L47608;}}
  else {
    goto L47613;}}
 L29026: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = proc_1216X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = name_1217X;
  arg0K0 = 2;
  goto L22004;}
 L43760: {
  val_1393X = arg0K0;
  SvalS = val_1393X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L30499: {
  table_1394X = arg0K0;
  n_1395X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_909X))) + -4))))>>8));
  arg0K0 = 0;
  arg0K1 = 0;
  goto L20323;}
 L37788: {
  l_1396X = arg0K0;
  i_1397X = arg0K1;
  if ((i_1397X < 0)) {
    SvalS = string_1232X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1232X))) + i_1397X)) = ((((((*((long *) (((char *) (-3 + l_1396X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_1396X))) + 4)));
    arg0K1 = (-1 + i_1397X);
    goto L37788;}}
 L29696: {
  push_exception_continuationB(5, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1240X;
  arg0K0 = 1;
  goto L22004;}
 L29898: {
  push_exception_continuationB(5, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1246X;
  arg0K0 = 1;
  goto L22004;}
 L30096: {
  push_exception_continuationB(5, 2);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = Kchar_1250X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_1251X;
  arg0K0 = 2;
  goto L22004;}
 L39853: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_1256X))) + 4)));
  goto L39847;}
 L15718: {
  if ((3 == (3 & thing_1257X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_1257X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_1257X)))));
      goto L15760;}
    else {
      goto L15726;}}
  else {
    goto L15726;}}
 L39838: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_960X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L22615: {
  SstackS = (Sbottom_of_stackS);
  *((long *) ((Sbottom_of_stackS) + 4)) = 1;
  ScontS = (Sbottom_of_stackS);
  push_exception_continuationB(5, 0);
  x_1398X = SvalS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1398X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_1260X;
  arg0K0 = 2;
  goto L22004;}
 L22289: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    s48_Spending_interruptPS = 0;
    if ((s48_Spending_eventsPS)) {
      s48_Spending_interruptPS = 1;
      goto L22291;}
    else {
      goto L22291;}}
  else {
    s48_Spending_interruptPS = 1;
    goto L22291;}}
 L22414: {
  ps_error("interrupt handler is not a vector", 0);
  goto L22289;}
 L11188: {
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = channel_1263X;
  x_1399X = *((long *) ((((char *) (-3 + channel_1263X))) + 16));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1399X;
  n_1400X = Senabled_interruptsS;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((n_1400X)<<2));
  arg0K0 = 3;
  goto L22271;}
 L26266: {
  env_1401X = arg0K0;
  i_1402X = arg0K1;
  if ((0 == i_1402X)) {
    count_1403X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_1266X)));
    arg0K0 = count_1403X;
    arg0K1 = i_1265X;
    arg0K2 = (2 + offset_1266X);
    goto L26205;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1401X)))));
    arg0K1 = (-1 + i_1402X);
    goto L26266;}}
 L26117: {
  env_1404X = arg0K0;
  i_1405X = arg0K1;
  if ((0 == i_1405X)) {
    index_1406X = 1 + offset_1270X;
    count_1407X = ((((*((unsigned char *) ((Scode_pointerS) + (1 + index_1406X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_1406X))));
    arg0K0 = count_1407X;
    arg0K1 = i_1269X;
    arg0K2 = (3 + offset_1270X);
    goto L26056;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_1404X)))));
    arg0K1 = (-1 + i_1405X);
    goto L26117;}}
 L13754: {
  if ((3 == (3 & x_620X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + x_620X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L48513;}
    else {
      arg0K0 = 1;
      goto L48513;}}
  else {
    arg0K0 = 1;
    goto L48513;}}
 L38832: {
  if ((3 == (3 & n_622X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_622X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = n_622X;
      arg0K0 = 1;
      goto L22004;}
    else {
      goto L38836;}}
  else {
    goto L38836;}}
 L39031: {
  if ((3 == (3 & n_623X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_623X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = n_623X;
      arg0K0 = 1;
      goto L22004;}
    else {
      goto L39035;}}
  else {
    goto L39035;}}
 L39230: {
  if ((3 == (3 & n_624X))) {
    if ((11 == (31 & ((((*((long *) ((((char *) (-3 + n_624X))) + -4))))>>2))))) {
      push_exception_continuationB(5, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = n_624X;
      arg0K0 = 1;
      goto L22004;}
    else {
      goto L39234;}}
  else {
    goto L39234;}}
 L17801: {
  length0_1408X = arg0K0;
  extra0_1409X = arg0K1;
  if ((0 == (3 & y_629X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17809;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_629X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17809;}}
 L19587: {
  x_1410X = arg0K0;
  y_1411X = arg0K1;
  if ((0 == (3 & x_1410X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17721;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_1410X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17721;}}
 L8250: {
  if ((536870911 < lo_c_1286X)) {
    arg0K0 = arg2_631X;
    arg0K1 = y_632X;
    goto L19587;}
  else {
    if ((lo_c_1286X < 0)) {
      arg0K0 = arg2_631X;
      arg0K1 = y_632X;
      goto L19587;}
    else {
      if ((8192 < mid_c_1289X)) {
        arg0K0 = arg2_631X;
        arg0K1 = y_632X;
        goto L19587;}
      else {
        if ((a_633X < 0)) {
          if ((b_634X < 0)) {
            s48_make_availableAgc(16);
            if ((536870911 < c_1290X)) {
              goto L49862;}
            else {
              if ((c_1290X < -536870912)) {
                goto L49862;}
              else {
                arg0K0 = (((c_1290X)<<2));
                goto L49857;}}}
          else {
            goto L8278;}}
        else {
          if ((b_634X < 0)) {
            goto L8278;}
          else {
            s48_make_availableAgc(16);
            if ((536870911 < c_1290X)) {
              goto L49884;}
            else {
              if ((c_1290X < -536870912)) {
                goto L49884;}
              else {
                arg0K0 = (((c_1290X)<<2));
                goto L49879;}}}}}}}}
 L17758: {
  length0_1412X = arg0K0;
  extra0_1413X = arg0K1;
  if ((0 == (3 & y_636X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17766;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_636X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17766;}}
 L49965: {
  v_1414X = (char *) s48_long_to_bignum(c_1292X);
  v_1415X = enter_bignum(v_1414X);
  arg0K0 = v_1415X;
  goto L49960;}
 L49960: {
  val_1416X = arg0K0;
  SvalS = val_1416X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L8512: {
  x_1417X = 0 - c_1292X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1417X)) {
    goto L49943;}
  else {
    if ((x_1417X < -536870912)) {
      goto L49943;}
    else {
      arg0K0 = (((x_1417X)<<2));
      goto L49938;}}}
 L49987: {
  v_1418X = (char *) s48_long_to_bignum(c_1292X);
  v_1419X = enter_bignum(v_1418X);
  arg0K0 = v_1419X;
  goto L49982;}
 L49982: {
  val_1420X = arg0K0;
  SvalS = val_1420X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L17326: {
  length0_1421X = arg0K0;
  extra0_1422X = arg0K1;
  if ((0 == (3 & y_639X))) {
    arg0K0 = 3;
    goto L17334;}
  else {
    arg0K0 = 0;
    goto L17334;}}
 L32576: {
  val_1423X = arg0K0;
  SvalS = val_1423X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L32836: {
  val_1424X = arg0K0;
  SvalS = val_1424X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33159: {
  val_1425X = arg0K0;
  SvalS = val_1425X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33482: {
  val_1426X = arg0K0;
  SvalS = val_1426X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L33776: {
  val_1427X = arg0K0;
  SvalS = val_1427X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L19528: {
  extra1_1428X = arg0K0;
  Stemp0S = x_1091X;
  Stemp1S = y_1092X;
  s48_make_availableAgc(((((((8 + (((length0_1313X)<<1))) + extra0_1314X) + extra1_1428X))<<2)));
  value_1429X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1429X))) {
    v_1430X = (char *) s48_long_to_bignum((((value_1429X)>>2)));
    arg1K0 = v_1430X;
    goto L19514;}
  else {
    arg1K0 = (((char *) (-3 + value_1429X)));
    goto L19514;}}
 L9155: {
  if ((536870911 < c_1316X)) {
    arg0K0 = arg2_652X;
    arg0K1 = y_653X;
    goto L19496;}
  else {
    SvalS = (((c_1316X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}}
 L9154: {
  SvalS = ((((0 - c_1316X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L19437: {
  extra1_1431X = arg0K0;
  Stemp0S = x_1094X;
  Stemp1S = y_1095X;
  s48_make_availableAgc(((((((8 + (((length0_1317X)<<1))) + extra0_1318X) + extra1_1431X))<<2)));
  value_1432X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1432X))) {
    v_1433X = (char *) s48_long_to_bignum((((value_1432X)>>2)));
    arg1K0 = v_1433X;
    goto L19423;}
  else {
    arg1K0 = (((char *) (-3 + value_1432X)));
    goto L19423;}}
 L34286: {
  n_1434X = arg0K0;
  SvalS = (((n_1434X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L22816: {
  v_1435X = (char *) s48_long_to_bignum(x_1321X);
  return enter_bignum(v_1435X);}
 L19005: {
  x_1436X = arg1K0;
  v_1437X = s48_bignum_test(x_1436X);
  if ((-1 == v_1437X)) {
    v_1438X = (char *) s48_bignum_negate(x_1436X);
    arg1K0 = v_1438X;
    goto L19007;}
  else {
    arg1K0 = x_1436X;
    goto L19007;}}
 L19362: {
  v_1439X = arg0K0;
  Stemp0S = x_682X;
  s48_make_availableAgc((((((3 + v_1439X) + extra_1327X))<<2)));
  value_1440X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1440X))) {
    v_1441X = (char *) s48_long_to_bignum((((value_1440X)>>2)));
    arg1K0 = v_1441X;
    goto L19352;}
  else {
    arg1K0 = (((char *) (-3 + value_1440X)));
    goto L19352;}}
 L19304: {
  v_1442X = arg0K0;
  Stemp0S = x_683X;
  s48_make_availableAgc((((((3 + v_1442X) + extra_1331X))<<2)));
  value_1443X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1443X))) {
    v_1444X = (char *) s48_long_to_bignum((((value_1443X)>>2)));
    arg1K0 = v_1444X;
    goto L19294;}
  else {
    arg1K0 = (((char *) (-3 + value_1443X)));
    goto L19294;}}
 L17540: {
  length0_1445X = arg0K0;
  extra0_1446X = arg0K1;
  if ((0 == (3 & y_686X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17548;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_686X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17548;}}
 L17497: {
  length0_1447X = arg0K0;
  extra0_1448X = arg0K1;
  if ((0 == (3 & y_688X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17505;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_688X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17505;}}
 L17454: {
  length0_1449X = arg0K0;
  extra0_1450X = arg0K1;
  if ((0 == (3 & y_690X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17462;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_690X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17462;}}
 L41553: {
  x_1451X = arg1K0;
  external_bignum_1452X = (char *) s48_bignum_arithmetic_shift(x_1451X, y_1340X);
  v_1453X = s48_bignum_fits_in_word_p(external_bignum_1452X, 30, 1);
  if (v_1453X) {
    n_1454X = s48_bignum_to_long(external_bignum_1452X);
    arg0K0 = (((n_1454X)<<2));
    goto L41481;}
  else {
    v_1455X = enter_bignum(external_bignum_1452X);
    arg0K0 = v_1455X;
    goto L41481;}}
 L41487: {
  val_1456X = arg0K0;
  SvalS = val_1456X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L20816: {
  foo_1457X = arg0K0;
  if ((1 == foo_1457X)) {
    if ((3 == (3 & bucket_1358X))) {
      arg0K0 = (-4 & bucket_1358X);
      goto L20821;}
    else {
      arg0K0 = bucket_1358X;
      goto L20821;}}
  else {
    s2_1458X = *((long *) (((char *) (-3 + foo_1457X))));
    len_1459X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + string_780X))) + -4))))>>8);
    if ((len_1459X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1458X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1458X))), (void *)(((char *) (-3 + string_780X))),len_1459X)))) {
        arg0K0 = foo_1457X;
        goto L31879;}
      else {
        goto L20836;}}
    else {
      goto L20836;}}}
 L36053: {
  v_1460X = ps_close_fd(index_1359X);
  arg0K0 = v_1460X;
  goto L36048;}
 L36048: {
  status_1461X = arg0K0;
  if ((status_1461X == NO_ERRORS)) {
    push_exception_continuationB(reason_1361X, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = arg2_789X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_790X)<<2));
    arg0K0 = 2;
    goto L22004;}
  else {
    channel_close_error(status_1461X, index_1359X, arg2_789X);
    push_exception_continuationB(reason_1361X, 1);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = arg2_789X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_790X)<<2));
    arg0K0 = 2;
    goto L22004;}}
 L36380: {
  if ((3 == (3 & arg4_798X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_798X))) + -4)))))) {
      if ((3 == (3 & arg4_798X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_798X))) + -4))))>>2))))) {
          arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_798X))) + -4))))>>8)));
          goto L36387;}
        else {
          goto L36621;}}
      else {
        goto L36621;}}
    else {
      goto L36448;}}
  else {
    goto L36448;}}
 L36595: {
  if ((3 == (3 & arg4_798X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + arg4_798X))) + -4))))>>2))))) {
      goto L36380;}
    else {
      goto L36448;}}
  else {
    goto L36448;}}
 L36448: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg5_799X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_798X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_1369X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_1370X)<<2));
  if (waitP_1368X) {
    arg0K0 = 5;
    goto L36464;}
  else {
    arg0K0 = 1;
    goto L36464;}}
 L36810: {
  val_1462X = arg0K0;
  SvalS = val_1462X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L37093: {
  x_1463X = Scurrent_threadS;
  addr_1464X = (((char *) (-3 + x_1463X))) + 12;
  S48_WRITE_BARRIER(x_1463X, addr_1464X, proposal_820X);
  *((long *) addr_1464X) = proposal_820X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L37427: {
  copies_1465X = arg0K0;
  if ((1 == copies_1465X)) {
    log_1466X = *((long *) ((((char *) (-3 + proposal_821X))) + 4));
    arg0K0 = 0;
    goto L8934;}
  else {
    thing_1467X = *((long *) ((((char *) (-3 + copies_1465X))) + 8));
    if ((3 == (3 & thing_1467X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + thing_1467X))) + -4)))))) {
        arg0K0 = (*((long *) ((((char *) (-3 + copies_1465X))) + 20)));
        goto L37427;}
      else {
        goto L37327;}}
    else {
      goto L37327;}}}
 L10154: {
  arg0K0 = (4 + i_1382X);
  goto L10102;}
 L9548: {
  entry_1468X = arg0K0;
  proposal_1469X = *((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12));
  value_1470X = Stemp0S;
  Stemp0S = 1;
  addr_1471X = ((char *) (-3 + entry_1468X));
  S48_WRITE_BARRIER(entry_1468X, addr_1471X, value_1470X);
  *((long *) addr_1471X) = value_1470X;
  addr_1472X = (((char *) (-3 + entry_1468X))) + 4;
  S48_WRITE_BARRIER(entry_1468X, addr_1472X, from_index_1179X);
  *((long *) addr_1472X) = from_index_1179X;
  value_1473X = Stemp1S;
  Stemp1S = 1;
  addr_1474X = (((char *) (-3 + entry_1468X))) + 8;
  S48_WRITE_BARRIER(entry_1468X, addr_1474X, value_1473X);
  *((long *) addr_1474X) = value_1473X;
  addr_1475X = (((char *) (-3 + entry_1468X))) + 12;
  S48_WRITE_BARRIER(entry_1468X, addr_1475X, to_index_1180X);
  *((long *) addr_1475X) = to_index_1180X;
  addr_1476X = (((char *) (-3 + entry_1468X))) + 16;
  S48_WRITE_BARRIER(entry_1468X, addr_1476X, count_1181X);
  *((long *) addr_1476X) = count_1181X;
  value_1477X = *((long *) ((((char *) (-3 + proposal_1469X))) + 12));
  addr_1478X = (((char *) (-3 + entry_1468X))) + 20;
  S48_WRITE_BARRIER(entry_1468X, addr_1478X, value_1477X);
  *((long *) addr_1478X) = value_1477X;
  addr_1479X = (((char *) (-3 + proposal_1469X))) + 12;
  S48_WRITE_BARRIER(proposal_1469X, addr_1479X, entry_1468X);
  *((long *) addr_1479X) = entry_1468X;
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L47608: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L47613: {
  if ((0 == (Spending_interruptsS))) {
    s48_wait_for_event((((arg2_900X)>>2)), minutesP_1391X);
    goto L47608;}
  else {
    goto L47608;}}
 L20323: {
  i_1480X = arg0K0;
  h_1481X = arg0K1;
  if ((i_1480X < n_1395X)) {
    arg0K0 = (1 + i_1480X);
    arg0K1 = (h_1481X + (((*((unsigned char *) ((((char *) (-3 + arg2_909X))) + i_1480X))))));
    goto L20323;}
  else {
    index_1482X = 1023 & h_1481X;
    link_1483X = *((long *) ((((char *) (-3 + table_1394X))) + (((index_1482X)<<2))));
    if ((0 == (3 & link_1483X))) {
      arg0K0 = (3 + (-4 & link_1483X));
      goto L20278;}
    else {
      arg0K0 = link_1483X;
      goto L20278;}}}
 L15760: {
  v_1484X = arg3K0;
  ps_write_string(v_1484X, out_960X);
  goto L39853;}
 L15726: {
  if ((3 == (3 & thing_1257X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_1257X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_1257X))))))))));
      goto L15760;}
    else {
      goto L15734;}}
  else {
    goto L15734;}}
 L22291: {
  SvalS = (*((long *) ((((char *) (-3 + handlers_1262X))) + (((i_973X)<<2)))));
  obj_1485X = SvalS;
  if ((3 == (3 & obj_1485X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_1485X))) + -4))))>>2))))) {
      goto L22308;}
    else {
      goto L22439;}}
  else {
    goto L22439;}}
 L26205: {
  count_1486X = arg0K0;
  i_1487X = arg0K1;
  offset_1488X = arg0K2;
  if ((0 == count_1486X)) {
    arg0K0 = i_1487X;
    arg0K1 = offset_1488X;
    arg0K2 = env_1401X;
    goto L26188;}
  else {
    value_1489X = *((long *) ((((char *) (-3 + env_1401X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1488X)))))<<2))));
    addr_1490X = (((char *) (-3 + new_env_553X))) + (((i_1487X)<<2));
    S48_WRITE_BARRIER(new_env_553X, addr_1490X, value_1489X);
    *((long *) addr_1490X) = value_1489X;
    arg0K0 = (-1 + count_1486X);
    arg0K1 = (1 + i_1487X);
    arg0K2 = (1 + offset_1488X);
    goto L26205;}}
 L26056: {
  count_1491X = arg0K0;
  i_1492X = arg0K1;
  offset_1493X = arg0K2;
  if ((0 == count_1491X)) {
    arg0K0 = i_1492X;
    arg0K1 = offset_1493X;
    arg0K2 = env_1404X;
    goto L26039;}
  else {
    value_1494X = *((long *) ((((char *) (-3 + env_1404X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_1493X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + offset_1493X))))))<<2))));
    addr_1495X = (((char *) (-3 + new_env_560X))) + (((i_1492X)<<2));
    S48_WRITE_BARRIER(new_env_560X, addr_1495X, value_1494X);
    *((long *) addr_1495X) = value_1494X;
    arg0K0 = (-1 + count_1491X);
    arg0K1 = (1 + i_1492X);
    arg0K2 = (2 + offset_1493X);
    goto L26056;}}
 L38836: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L39035: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L39234: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L17809: {
  length1_1496X = arg0K0;
  extra1_1497X = arg0K1;
  if ((length0_1408X < length1_1496X)) {
    arg0K0 = length1_1496X;
    goto L17831;}
  else {
    arg0K0 = length0_1408X;
    goto L17831;}}
 L17721: {
  length0_1498X = arg0K0;
  extra0_1499X = arg0K1;
  if ((0 == (3 & y_1411X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17729;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + y_1411X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17729;}}
 L49862: {
  v_1500X = (char *) s48_long_to_bignum(c_1290X);
  v_1501X = enter_bignum(v_1500X);
  arg0K0 = v_1501X;
  goto L49857;}
 L49857: {
  val_1502X = arg0K0;
  SvalS = val_1502X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L8278: {
  x_1503X = 0 - c_1290X;
  s48_make_availableAgc(16);
  if ((536870911 < x_1503X)) {
    goto L49840;}
  else {
    if ((x_1503X < -536870912)) {
      goto L49840;}
    else {
      arg0K0 = (((x_1503X)<<2));
      goto L49835;}}}
 L49884: {
  v_1504X = (char *) s48_long_to_bignum(c_1290X);
  v_1505X = enter_bignum(v_1504X);
  arg0K0 = v_1505X;
  goto L49879;}
 L49879: {
  val_1506X = arg0K0;
  SvalS = val_1506X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L17766: {
  length1_1507X = arg0K0;
  extra1_1508X = arg0K1;
  if ((length0_1412X < length1_1507X)) {
    arg0K0 = length1_1507X;
    goto L17788;}
  else {
    arg0K0 = length0_1412X;
    goto L17788;}}
 L49943: {
  v_1509X = (char *) s48_long_to_bignum(x_1417X);
  v_1510X = enter_bignum(v_1509X);
  arg0K0 = v_1510X;
  goto L49938;}
 L49938: {
  val_1511X = arg0K0;
  SvalS = val_1511X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L17334: {
  extra1_1512X = arg0K0;
  Stemp0S = arg2_638X;
  Stemp1S = y_639X;
  s48_make_availableAgc(((((((8 + (((length0_1421X)<<1))) + extra0_1422X) + extra1_1512X))<<2)));
  value_1513X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1513X))) {
    v_1514X = (char *) s48_long_to_bignum((((value_1513X)>>2)));
    arg1K0 = v_1514X;
    goto L18841;}
  else {
    arg1K0 = (((char *) (-3 + value_1513X)));
    goto L18841;}}
 L19514: {
  y_1515X = arg1K0;
  value_1516X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1516X))) {
    v_1517X = (char *) s48_long_to_bignum((((value_1516X)>>2)));
    arg1K0 = v_1517X;
    goto L19510;}
  else {
    arg1K0 = (((char *) (-3 + value_1516X)));
    goto L19510;}}
 L19423: {
  y_1518X = arg1K0;
  value_1519X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1519X))) {
    v_1520X = (char *) s48_long_to_bignum((((value_1519X)>>2)));
    arg1K0 = v_1520X;
    goto L19419;}
  else {
    arg1K0 = (((char *) (-3 + value_1519X)));
    goto L19419;}}
 L19007: {
  external_bignum_1521X = arg1K0;
  v_1522X = s48_bignum_fits_in_word_p(external_bignum_1521X, 30, 1);
  if (v_1522X) {
    n_1523X = s48_bignum_to_long(external_bignum_1521X);
    return (((n_1523X)<<2));}
  else {
    return enter_bignum(external_bignum_1521X);}}
 L19352: {
  v_1524X = arg1K0;
  external_bignum_1525X = (char *) s48_bignum_bitwise_not(v_1524X);
  v_1526X = s48_bignum_fits_in_word_p(external_bignum_1525X, 30, 1);
  if (v_1526X) {
    n_1527X = s48_bignum_to_long(external_bignum_1525X);
    arg0K0 = (((n_1527X)<<2));
    goto L34810;}
  else {
    val_1528X = enter_bignum(external_bignum_1525X);
    arg0K0 = val_1528X;
    goto L34810;}}
 L19294: {
  v_1529X = arg1K0;
  n_1530X = s48_bignum_bit_count(v_1529X);
  SvalS = (((n_1530X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L17548: {
  length1_1531X = arg0K0;
  extra1_1532X = arg0K1;
  if ((length0_1445X < length1_1531X)) {
    arg0K0 = length1_1531X;
    goto L17570;}
  else {
    arg0K0 = length0_1445X;
    goto L17570;}}
 L17505: {
  length1_1533X = arg0K0;
  extra1_1534X = arg0K1;
  if ((length0_1447X < length1_1533X)) {
    arg0K0 = length1_1533X;
    goto L17527;}
  else {
    arg0K0 = length0_1447X;
    goto L17527;}}
 L17462: {
  length1_1535X = arg0K0;
  extra1_1536X = arg0K1;
  if ((length0_1449X < length1_1535X)) {
    arg0K0 = length1_1535X;
    goto L17484;}
  else {
    arg0K0 = length0_1449X;
    goto L17484;}}
 L41481: {
  val_1537X = arg0K0;
  SvalS = val_1537X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L20821: {
  b_1538X = arg0K0;
  addr_1539X = s48_allocate_small(12);
  *((long *) addr_1539X) = 2054;
  x_1540X = 3 + (((long) (addr_1539X + 4)));
  *((long *) (((char *) (-3 + x_1540X)))) = string_780X;
  *((long *) ((((char *) (-3 + x_1540X))) + 4)) = b_1538X;
  if ((3 == (3 & x_1540X))) {
    arg0K0 = (-4 & x_1540X);
    goto L20827;}
  else {
    arg0K0 = x_1540X;
    goto L20827;}}
 L31879: {
  val_1541X = arg0K0;
  SvalS = val_1541X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L20836: {
  link_1542X = *((long *) ((((char *) (-3 + foo_1457X))) + 4));
  if ((0 == (3 & link_1542X))) {
    arg0K0 = (3 + (-4 & link_1542X));
    goto L20816;}
  else {
    arg0K0 = link_1542X;
    goto L20816;}}
 L36387: {
  y_1543X = arg0K0;
  if ((y_1543X < (start_1369X + count_1370X))) {
    goto L36448;}
  else {
    got_1544X = ps_read_fd(((((*((long *) ((((char *) (-3 + arg5_799X))) + 8))))>>2)), ((((char *) (-3 + arg4_798X))) + start_1369X), count_1370X, waitP_1368X, &eofP_1545X, &pendingP_1546X, &status_1547X);
    if ((status_1547X == NO_ERRORS)) {
      if (eofP_1545X) {
        arg0K0 = 21;
        goto L36447;}
      else {
        if (pendingP_1546X) {
          arg0K0 = 1;
          goto L36447;}
        else {
          arg0K0 = (((got_1544X)<<2));
          goto L36447;}}}
    else {
      push_exception_continuationB(24, 1);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg5_799X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = arg4_798X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_1369X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_1370X)<<2));
      if (waitP_1368X) {
        arg0K0 = 5;
        goto L36430;}
      else {
        arg0K0 = 1;
        goto L36430;}}}}
 L36621: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_798X))) + -4))))>>8));
  goto L36387;}
 L36464: {
  x_1548X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1548X;
  arg0K0 = 5;
  goto L22004;}
 L8934: {
  i_1549X = arg0K0;
  stob_1550X = *((long *) ((((char *) (-3 + log_1466X))) + (((i_1549X)<<2))));
  if ((1 == stob_1550X)) {
    log_1551X = *((long *) ((((char *) (-3 + proposal_821X))) + 8));
    arg0K0 = 0;
    goto L11910;}
  else {
    verify_1552X = *((long *) ((((char *) (-3 + log_1466X))) + (12 + (((i_1549X)<<2)))));
    value_1553X = *((long *) ((((char *) (-3 + log_1466X))) + (8 + (((i_1549X)<<2)))));
    if ((verify_1552X == value_1553X)) {
      goto L8966;}
    else {
      addr_1554X = (((char *) (-3 + stob_1550X))) + (-4 & (*((long *) ((((char *) (-3 + log_1466X))) + (4 + (((i_1549X)<<2)))))));
      S48_WRITE_BARRIER(stob_1550X, addr_1554X, value_1553X);
      *((long *) addr_1554X) = value_1553X;
      goto L8966;}}}
 L20278: {
  bucket_1555X = arg0K0;
  arg0K0 = 1;
  arg0K1 = bucket_1555X;
  goto L20285;}
 L15734: {
  if ((1 == thing_1257X)) {
    goto L15737;}
  else {
    if ((5 == thing_1257X)) {
      goto L15737;}
    else {
      if ((25 == thing_1257X)) {
        arg3K0 = "()";
        goto L15760;}
      else {
        if ((3 == (3 & thing_1257X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_1257X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L15760;}
          else {
            goto L15750;}}
        else {
          goto L15750;}}}}}
 L22308: {
  StemplateS = (SvalS);
  Slosing_opcodeS = (0 - i_973X);
  arg0K0 = arg_count_1261X;
  arg0K1 = 25;
  arg0K2 = 0;
  goto L47051;}
 L22439: {
  ps_error("interrupt handler is not a closure", 1, i_973X);
  goto L22308;}
 L17831: {
  v_1556X = arg0K0;
  Stemp0S = arg2_628X;
  Stemp1S = y_629X;
  s48_make_availableAgc(((((((3 + v_1556X) + extra0_1409X) + extra1_1497X))<<2)));
  value_1557X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1557X))) {
    v_1558X = (char *) s48_long_to_bignum((((value_1557X)>>2)));
    arg1K0 = v_1558X;
    goto L19753;}
  else {
    arg1K0 = (((char *) (-3 + value_1557X)));
    goto L19753;}}
 L17729: {
  length1_1559X = arg0K0;
  extra1_1560X = arg0K1;
  Stemp0S = x_1410X;
  Stemp1S = y_1411X;
  s48_make_availableAgc(((((((2 + (length0_1498X + length1_1559X)) + extra0_1499X) + extra1_1560X))<<2)));
  value_1561X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1561X))) {
    v_1562X = (char *) s48_long_to_bignum((((value_1561X)>>2)));
    arg1K0 = v_1562X;
    goto L19605;}
  else {
    arg1K0 = (((char *) (-3 + value_1561X)));
    goto L19605;}}
 L49840: {
  v_1563X = (char *) s48_long_to_bignum(x_1503X);
  v_1564X = enter_bignum(v_1563X);
  arg0K0 = v_1564X;
  goto L49835;}
 L49835: {
  val_1565X = arg0K0;
  SvalS = val_1565X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L17788: {
  v_1566X = arg0K0;
  Stemp0S = arg2_635X;
  Stemp1S = y_636X;
  s48_make_availableAgc(((((((3 + v_1566X) + extra0_1413X) + extra1_1508X))<<2)));
  value_1567X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1567X))) {
    v_1568X = (char *) s48_long_to_bignum((((value_1567X)>>2)));
    arg1K0 = v_1568X;
    goto L19679;}
  else {
    arg1K0 = (((char *) (-3 + value_1567X)));
    goto L19679;}}
 L18841: {
  y_1569X = arg1K0;
  value_1570X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1570X))) {
    v_1571X = (char *) s48_long_to_bignum((((value_1570X)>>2)));
    arg1K0 = v_1571X;
    goto L18837;}
  else {
    arg1K0 = (((char *) (-3 + value_1570X)));
    goto L18837;}}
 L19510: {
  x_1572X = arg1K0;
  external_bignum_1573X = (char *)s48_bignum_quotient(x_1572X, y_1515X);
  v_1574X = s48_bignum_fits_in_word_p(external_bignum_1573X, 30, 1);
  if (v_1574X) {
    n_1575X = s48_bignum_to_long(external_bignum_1573X);
    arg0K0 = (((n_1575X)<<2));
    goto L50658;}
  else {
    val_1576X = enter_bignum(external_bignum_1573X);
    arg0K0 = val_1576X;
    goto L50658;}}
 L19419: {
  x_1577X = arg1K0;
  external_bignum_1578X = (char *)s48_bignum_remainder(x_1577X, y_1518X);
  v_1579X = s48_bignum_fits_in_word_p(external_bignum_1578X, 30, 1);
  if (v_1579X) {
    n_1580X = s48_bignum_to_long(external_bignum_1578X);
    arg0K0 = (((n_1580X)<<2));
    goto L50658;}
  else {
    val_1581X = enter_bignum(external_bignum_1578X);
    arg0K0 = val_1581X;
    goto L50658;}}
 L34810: {
  val_1582X = arg0K0;
  SvalS = val_1582X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L17570: {
  v_1583X = arg0K0;
  Stemp0S = arg2_685X;
  Stemp1S = y_686X;
  s48_make_availableAgc(((((((3 + v_1583X) + extra0_1446X) + extra1_1532X))<<2)));
  value_1584X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1584X))) {
    v_1585X = (char *) s48_long_to_bignum((((value_1584X)>>2)));
    arg1K0 = v_1585X;
    goto L19227;}
  else {
    arg1K0 = (((char *) (-3 + value_1584X)));
    goto L19227;}}
 L17527: {
  v_1586X = arg0K0;
  Stemp0S = arg2_687X;
  Stemp1S = y_688X;
  s48_make_availableAgc(((((((3 + v_1586X) + extra0_1448X) + extra1_1534X))<<2)));
  value_1587X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1587X))) {
    v_1588X = (char *) s48_long_to_bignum((((value_1587X)>>2)));
    arg1K0 = v_1588X;
    goto L19153;}
  else {
    arg1K0 = (((char *) (-3 + value_1587X)));
    goto L19153;}}
 L17484: {
  v_1589X = arg0K0;
  Stemp0S = arg2_689X;
  Stemp1S = y_690X;
  s48_make_availableAgc(((((((3 + v_1589X) + extra0_1450X) + extra1_1536X))<<2)));
  value_1590X = Stemp1S;
  Stemp1S = 1;
  if ((0 == (3 & value_1590X))) {
    v_1591X = (char *) s48_long_to_bignum((((value_1590X)>>2)));
    arg1K0 = v_1591X;
    goto L19079;}
  else {
    arg1K0 = (((char *) (-3 + value_1590X)));
    goto L19079;}}
 L20827: {
  value_1592X = arg0K0;
  addr_1593X = (((char *) (-3 + table_779X))) + (((index_1141X)<<2));
  S48_WRITE_BARRIER(table_779X, addr_1593X, value_1592X);
  *((long *) addr_1593X) = value_1592X;
  arg0K0 = x_1540X;
  goto L31879;}
 L36447: {
  val_1594X = arg0K0;
  SvalS = val_1594X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L36430: {
  x_1595X = arg0K0;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1595X;
  merged_arg0K0 = status_1547X;
  merged_arg0K1 = 0;
  get_error_string_return_tag = 5;
  goto get_error_string;
 get_error_string_return_5:
  x_1596X = get_error_string0_return_value;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_1596X;
  arg0K0 = 6;
  goto L22004;}
 L11910: {
  i_1597X = arg0K0;
  stob_1598X = *((long *) ((((char *) (-3 + log_1551X))) + (((i_1597X)<<2))));
  if ((1 == stob_1598X)) {
    copies_1599X = *((long *) ((((char *) (-3 + proposal_821X))) + 12));
    arg0K0 = copies_1599X;
    goto L11825;}
  else {
    verify_1600X = *((long *) ((((char *) (-3 + log_1551X))) + (12 + (((i_1597X)<<2)))));
    value_1601X = *((long *) ((((char *) (-3 + log_1551X))) + (8 + (((i_1597X)<<2)))));
    if ((verify_1600X == value_1601X)) {
      goto L11942;}
    else {
      *((unsigned char *) ((((char *) (-3 + stob_1598X))) + ((((*((long *) ((((char *) (-3 + log_1551X))) + (4 + (((i_1597X)<<2)))))))>>2)))) = (((value_1601X)>>2));
      goto L11942;}}}
 L8966: {
  arg0K0 = (4 + i_1549X);
  goto L8934;}
 L20285: {
  previous_foo_1602X = arg0K0;
  foo_1603X = arg0K1;
  if ((1 == foo_1603X)) {
    goto L30501;}
  else {
    s2_1604X = *((long *) (((char *) (-3 + foo_1603X))));
    len_1605X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_909X))) + -4))))>>8);
    if ((len_1605X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_1604X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_1604X))), (void *)(((char *) (-3 + arg2_909X))),len_1605X)))) {
        if ((1 == previous_foo_1602X)) {
          value_1606X = *((long *) ((((char *) (-3 + foo_1603X))) + 12));
          addr_1607X = (((char *) (-3 + table_1394X))) + (((index_1482X)<<2));
          S48_WRITE_BARRIER(table_1394X, addr_1607X, value_1606X);
          *((long *) addr_1607X) = value_1606X;
          goto L30501;}
        else {
          val_1608X = *((long *) ((((char *) (-3 + foo_1603X))) + 12));
          addr_1609X = (((char *) (-3 + previous_foo_1602X))) + 12;
          S48_WRITE_BARRIER(previous_foo_1602X, addr_1609X, val_1608X);
          *((long *) addr_1609X) = val_1608X;
          goto L30501;}}
      else {
        goto L20353;}}
    else {
      goto L20353;}}}
 L15737: {
  if ((1 == thing_1257X)) {
    arg3K0 = "#f";
    goto L15760;}
  else {
    arg3K0 = "#t";
    goto L15760;}}
 L15750: {
  if ((3 == (3 & thing_1257X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_1257X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L15760;}
    else {
      goto L15754;}}
  else {
    goto L15754;}}
 L19753: {
  y_1610X = arg1K0;
  value_1611X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1611X))) {
    v_1612X = (char *) s48_long_to_bignum((((value_1611X)>>2)));
    arg1K0 = v_1612X;
    goto L19749;}
  else {
    arg1K0 = (((char *) (-3 + value_1611X)));
    goto L19749;}}
 L19605: {
  y_1613X = arg1K0;
  value_1614X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1614X))) {
    v_1615X = (char *) s48_long_to_bignum((((value_1614X)>>2)));
    arg1K0 = v_1615X;
    goto L19601;}
  else {
    arg1K0 = (((char *) (-3 + value_1614X)));
    goto L19601;}}
 L19679: {
  y_1616X = arg1K0;
  value_1617X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1617X))) {
    v_1618X = (char *) s48_long_to_bignum((((value_1617X)>>2)));
    arg1K0 = v_1618X;
    goto L19675;}
  else {
    arg1K0 = (((char *) (-3 + value_1617X)));
    goto L19675;}}
 L18837: {
  x_1619X = arg1K0;
  div_by_zeroP_1620X = s48_bignum_divide(x_1619X, y_1569X, &quot_1621X, &rem_1622X);
  if (div_by_zeroP_1620X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = 0;
    goto L41151;}
  else {
    v_1623X = s48_bignum_fits_in_word_p(rem_1622X, 30, 1);
    if (v_1623X) {
      n_1624X = s48_bignum_to_long(rem_1622X);
      arg0K0 = (((n_1624X)<<2));
      goto L18864;}
    else {
      v_1625X = enter_bignum(rem_1622X);
      arg0K0 = v_1625X;
      goto L18864;}}}
 L50658: {
  val_1626X = arg0K0;
  SvalS = val_1626X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L19227: {
  y_1627X = arg1K0;
  value_1628X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1628X))) {
    v_1629X = (char *) s48_long_to_bignum((((value_1628X)>>2)));
    arg1K0 = v_1629X;
    goto L19223;}
  else {
    arg1K0 = (((char *) (-3 + value_1628X)));
    goto L19223;}}
 L19153: {
  y_1630X = arg1K0;
  value_1631X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1631X))) {
    v_1632X = (char *) s48_long_to_bignum((((value_1631X)>>2)));
    arg1K0 = v_1632X;
    goto L19149;}
  else {
    arg1K0 = (((char *) (-3 + value_1631X)));
    goto L19149;}}
 L19079: {
  y_1633X = arg1K0;
  value_1634X = Stemp0S;
  Stemp0S = 1;
  if ((0 == (3 & value_1634X))) {
    v_1635X = (char *) s48_long_to_bignum((((value_1634X)>>2)));
    arg1K0 = v_1635X;
    goto L19075;}
  else {
    arg1K0 = (((char *) (-3 + value_1634X)));
    goto L19075;}}
 L11825: {
  copies_1636X = arg0K0;
  if ((1 == copies_1636X)) {
    value_1637X = Sempty_logS;
    addr_1638X = (((char *) (-3 + proposal_821X))) + 4;
    S48_WRITE_BARRIER(proposal_821X, addr_1638X, value_1637X);
    *((long *) addr_1638X) = value_1637X;
    value_1639X = Sempty_logS;
    addr_1640X = (((char *) (-3 + proposal_821X))) + 8;
    S48_WRITE_BARRIER(proposal_821X, addr_1640X, value_1639X);
    *((long *) addr_1640X) = value_1639X;
    addr_1641X = (((char *) (-3 + proposal_821X))) + 12;
    S48_WRITE_BARRIER(proposal_821X, addr_1641X, 1);
    *((long *) addr_1641X) = 1;
    RELEASE_PROPOSAL_LOCK();
    x_1642X = Scurrent_threadS;
    addr_1643X = (((char *) (-3 + x_1642X))) + 12;
    S48_WRITE_BARRIER(x_1642X, addr_1643X, 1);
    *((long *) addr_1643X) = 1;
    SvalS = 5;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L23956;}
  else {
    stob_1644X = *((long *) (((char *) (-3 + copies_1636X))));
    v_1645X = (((*((long *) ((((char *) (-3 + copies_1636X))) + 4))))>>2);
    stob_1646X = *((long *) ((((char *) (-3 + copies_1636X))) + 8));
    v_1647X = (((*((long *) ((((char *) (-3 + copies_1636X))) + 12))))>>2);
    v_1648X = (((*((long *) ((((char *) (-3 + copies_1636X))) + 16))))>>2);
    memcpy((void *)((((char *) (-3 + stob_1646X))) + v_1647X), (void *)((((char *) (-3 + stob_1644X))) + v_1645X),v_1648X);
    arg0K0 = (*((long *) ((((char *) (-3 + copies_1636X))) + 20)));
    goto L11825;}}
 L11942: {
  arg0K0 = (4 + i_1597X);
  goto L11910;}
 L30501: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L20353: {
  link_1649X = *((long *) ((((char *) (-3 + foo_1603X))) + 12));
  if ((0 == (3 & link_1649X))) {
    arg0K0 = foo_1603X;
    arg0K1 = (3 + (-4 & link_1649X));
    goto L20285;}
  else {
    arg0K0 = foo_1603X;
    arg0K1 = link_1649X;
    goto L20285;}}
 L15754: {
  if ((3 == (3 & thing_1257X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_1257X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L15760;}
    else {
      arg3K0 = "???";
      goto L15760;}}
  else {
    arg3K0 = "???";
    goto L15760;}}
 L19749: {
  x_1650X = arg1K0;
  external_bignum_1651X = (char *)s48_bignum_add(x_1650X, y_1610X);
  v_1652X = s48_bignum_fits_in_word_p(external_bignum_1651X, 30, 1);
  if (v_1652X) {
    n_1653X = s48_bignum_to_long(external_bignum_1651X);
    arg0K0 = (((n_1653X)<<2));
    goto L32000;}
  else {
    val_1654X = enter_bignum(external_bignum_1651X);
    arg0K0 = val_1654X;
    goto L32000;}}
 L19601: {
  x_1655X = arg1K0;
  external_bignum_1656X = (char *)s48_bignum_multiply(x_1655X, y_1613X);
  v_1657X = s48_bignum_fits_in_word_p(external_bignum_1656X, 30, 1);
  if (v_1657X) {
    n_1658X = s48_bignum_to_long(external_bignum_1656X);
    arg0K0 = (((n_1658X)<<2));
    goto L50658;}
  else {
    val_1659X = enter_bignum(external_bignum_1656X);
    arg0K0 = val_1659X;
    goto L50658;}}
 L19675: {
  x_1660X = arg1K0;
  external_bignum_1661X = (char *)s48_bignum_subtract(x_1660X, y_1616X);
  v_1662X = s48_bignum_fits_in_word_p(external_bignum_1661X, 30, 1);
  if (v_1662X) {
    n_1663X = s48_bignum_to_long(external_bignum_1661X);
    arg0K0 = (((n_1663X)<<2));
    goto L32289;}
  else {
    val_1664X = enter_bignum(external_bignum_1661X);
    arg0K0 = val_1664X;
    goto L32289;}}
 L41151: {
  div_by_zeroP_1665X = arg2K0;
  quot_1666X = arg0K1;
  rem_1667X = arg0K2;
  if (div_by_zeroP_1665X) {
    goto L41172;}
  else {
    if ((0 == (3 & rem_1667X))) {
      if ((0 == rem_1667X)) {
        SvalS = quot_1666X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L23956;}
      else {
        goto L41172;}}
    else {
      goto L41172;}}}
 L18864: {
  v_1668X = arg0K0;
  v_1669X = s48_bignum_fits_in_word_p(quot_1621X, 30, 1);
  if (v_1669X) {
    n_1670X = s48_bignum_to_long(quot_1621X);
    arg2K0 = 0;
    arg0K1 = (((n_1670X)<<2));
    arg0K2 = v_1668X;
    goto L41151;}
  else {
    v_1671X = enter_bignum(quot_1621X);
    arg2K0 = 0;
    arg0K1 = v_1671X;
    arg0K2 = v_1668X;
    goto L41151;}}
 L19223: {
  x_1672X = arg1K0;
  external_bignum_1673X = (char *) s48_bignum_bitwise_and(x_1672X, y_1627X);
  v_1674X = s48_bignum_fits_in_word_p(external_bignum_1673X, 30, 1);
  if (v_1674X) {
    n_1675X = s48_bignum_to_long(external_bignum_1673X);
    arg0K0 = (((n_1675X)<<2));
    goto L34992;}
  else {
    val_1676X = enter_bignum(external_bignum_1673X);
    arg0K0 = val_1676X;
    goto L34992;}}
 L19149: {
  x_1677X = arg1K0;
  external_bignum_1678X = (char *) s48_bignum_bitwise_ior(x_1677X, y_1630X);
  v_1679X = s48_bignum_fits_in_word_p(external_bignum_1678X, 30, 1);
  if (v_1679X) {
    n_1680X = s48_bignum_to_long(external_bignum_1678X);
    arg0K0 = (((n_1680X)<<2));
    goto L35155;}
  else {
    val_1681X = enter_bignum(external_bignum_1678X);
    arg0K0 = val_1681X;
    goto L35155;}}
 L19075: {
  x_1682X = arg1K0;
  external_bignum_1683X = (char *) s48_bignum_bitwise_xor(x_1682X, y_1633X);
  v_1684X = s48_bignum_fits_in_word_p(external_bignum_1683X, 30, 1);
  if (v_1684X) {
    n_1685X = s48_bignum_to_long(external_bignum_1683X);
    arg0K0 = (((n_1685X)<<2));
    goto L35318;}
  else {
    val_1686X = enter_bignum(external_bignum_1683X);
    arg0K0 = val_1686X;
    goto L35318;}}
 L32000: {
  val_1687X = arg0K0;
  SvalS = val_1687X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L32289: {
  val_1688X = arg0K0;
  SvalS = val_1688X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L41172: {
  push_exception_continuationB(5, 1);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_638X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = y_639X;
  arg0K0 = 2;
  goto L22004;}
 L34992: {
  val_1689X = arg0K0;
  SvalS = val_1689X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L35155: {
  val_1690X = arg0K0;
  SvalS = val_1690X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 L35318: {
  val_1691X = arg0K0;
  SvalS = val_1691X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L23956;}
 loseD0: {
  message_440X = merged_arg3K0;{
  why_1692X = (((*((long *) ((SstackS) + (((nargs_465X)<<2))))))>>2);
  ps_write_string("Template UIDs: ", (stderr));
  current_template_1693X = StemplateS;
  out_1694X = stderr;
  if ((0 == (3 & (*((long *) ((((char *) (-3 + current_template_1693X))) + 4)))))) {
    ps_write_integer(((((*((long *) ((((char *) (-3 + current_template_1693X))) + 4))))>>2)), out_1694X);
    arg2K0 = 1;
    goto L13599;}
  else {
    arg2K0 = 0;
    goto L13599;}}
 L13599: {
  not_firstP_1695X = arg2K0;
  arg1K0 = (ScontS);
  arg2K1 = not_firstP_1695X;
  goto L13606;}
 L13606: {
  cont_1696X = arg1K0;
  not_firstP_1697X = arg2K1;
  if ((cont_1696X == (Sbottom_of_stackS))) {
    arg0K0 = (*((long *) (cont_1696X + 4)));
    arg2K1 = not_firstP_1697X;
    goto L13612;}
  else {
    template_1698X = *((long *) (cont_1696X + 8));
    if ((0 == (3 & (*((long *) ((((char *) (-3 + template_1698X))) + 4)))))) {
      if (not_firstP_1697X) {
        ps_write_string(" <- ", out_1694X);
        goto L13712;}
      else {
        goto L13712;}}
    else {
      arg2K0 = not_firstP_1697X;
      goto L13634;}}}
 L13612: {
  cont_1699X = arg0K0;
  not_firstP_1700X = arg2K1;
  if ((3 == (3 & cont_1699X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + cont_1699X))) + -4))))>>2))))) {
      template_1701X = *((long *) ((((char *) (-3 + cont_1699X))) + 8));
      if ((0 == (3 & (*((long *) ((((char *) (-3 + template_1701X))) + 4)))))) {
        if (not_firstP_1700X) {
          ps_write_string(" <- ", out_1694X);
          goto L13672;}
        else {
          goto L13672;}}
      else {
        arg2K0 = not_firstP_1700X;
        goto L13621;}}
    else {
      goto L22023;}}
  else {
    goto L22023;}}
 L13712: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_1698X))) + 4))))>>2)), out_1694X);
  arg2K0 = 1;
  goto L13634;}
 L13634: {
  v_1702X = arg2K0;
  arg1K0 = (((char *) (*((long *) (cont_1696X + 4)))));
  arg2K1 = v_1702X;
  goto L13606;}
 L13672: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_1701X))) + 4))))>>2)), out_1694X);
  arg2K0 = 1;
  goto L13621;}
 L13621: {
  v_1703X = arg2K0;
  arg0K0 = (*((long *) ((((char *) (-3 + cont_1699X))) + 4)));
  arg2K1 = v_1703X;
  goto L13612;}
 L22023: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_1692X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (-4 + (((nargs_465X)<<2)))))))))))))) {
      ps_error(message_440X, 3, opcode_466X, why_1692X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (-4 + (((nargs_465X)<<2))))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L22047;}}
  else {
    goto L22047;}}
 L22047: {
  ps_error(message_440X, 2, opcode_466X, why_1692X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L50667;}
 L50667: {
  type_1704X = s48_get_next_event(&channel_1705X, &status_1706X);
  if ((type_1704X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L46520;}
    else {
      arg0K0 = 1;
      goto L46520;}}
  else {
    if ((type_1704X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L46520;}
    else {
      if ((type_1704X == IO_COMPLETION_EVENT)) {
        enqueue_channelB(channel_1705X, status_1706X);
        arg0K0 = 8;
        goto L46520;}
      else {
        if ((type_1704X == OS_SIGNAL_EVENT)) {
          arg0K0 = 16;
          goto L46520;}
        else {
          if ((type_1704X == NO_EVENT)) {
            arg0K0 = 0;
            goto L46520;}
          else {
            if ((type_1704X == ERROR_EVENT)) {
              ps_write_string("OS error while getting event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              ps_write_string((ps_error_string(status_1706X)), (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L46520;}
            else {
              ps_write_string("unknown type of event", (stderr));
              { long ignoreXX;
              PS_WRITE_CHAR(10, (stderr), ignoreXX) }
              arg0K0 = 0;
              goto L46520;}}}}}}}
 L46520: {
  v_1707X = arg0K0;
  Spending_interruptsS = (v_1707X | (Spending_interruptsS));
  if ((type_1704X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      s48_Spending_interruptPS = 0;
      if ((s48_Spending_eventsPS)) {
        s48_Spending_interruptPS = 1;
        goto L46547;}
      else {
        goto L46547;}}
    else {
      s48_Spending_interruptPS = 1;
      goto L46547;}}
  else {
    goto L50667;}}
 L46547: {
  if ((s48_Spending_interruptPS)) {
    if ((s48_Spending_eventsPS)) {
      s48_Spending_eventsPS = 0;
      goto L50667;}
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
  wants_stack_args_436X = merged_arg0K0;
  stack_arg_count_437X = merged_arg0K1;
  list_args_438X = merged_arg0K2;
  list_arg_count_439X = merged_arg0K3;{
  if ((stack_arg_count_437X == wants_stack_args_436X)) {
    merged_arg0K0 = list_args_438X;
    merged_arg0K1 = list_arg_count_439X;
    copy_listSAgc_return_tag = 1;
    goto copy_listSAgc;
   copy_listSAgc_return_1:
    x_1708X = copy_listSAgc0_return_value;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1708X;
    goto rest_list_setupAgc_return;}
  else {
    if ((stack_arg_count_437X < wants_stack_args_436X)) {
      count_1709X = wants_stack_args_436X - stack_arg_count_437X;
      merged_arg0K0 = list_args_438X;
      merged_arg0K1 = count_1709X;
      push_list_return_tag = 4;
      goto push_list;
     push_list_return_4:
      v_1710X = push_list0_return_value;
      merged_arg0K0 = v_1710X;
      merged_arg0K1 = (list_arg_count_439X - count_1709X);
      copy_listSAgc_return_tag = 2;
      goto copy_listSAgc;
     copy_listSAgc_return_2:
      x_1711X = copy_listSAgc0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1711X;
      goto rest_list_setupAgc_return;}
    else {
      merged_arg0K0 = list_args_438X;
      merged_arg0K1 = list_arg_count_439X;
      copy_listSAgc_return_tag = 3;
      goto copy_listSAgc;
     copy_listSAgc_return_3:
      v_1712X = copy_listSAgc0_return_value;
      merged_arg0K0 = v_1712X;
      merged_arg0K1 = (stack_arg_count_437X - wants_stack_args_436X);
      pop_args_GlistSAgc_return_tag = 9;
      goto pop_args_GlistSAgc;
     pop_args_GlistSAgc_return_9:
      x_1713X = pop_args_GlistSAgc0_return_value;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_1713X;
      goto rest_list_setupAgc_return;}}}
 rest_list_setupAgc_return:
  switch (rest_list_setupAgc_return_tag) {
  case 0: goto rest_list_setupAgc_return_0;
  default: goto rest_list_setupAgc_return_1;
  }}

 push_list: {
  list_434X = merged_arg0K0;
  count_435X = merged_arg0K1;{
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = list_434X;
  if ((count_435X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L21592;}
  else {
    s48_copy_stack_into_heap();
    if ((count_435X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
      goto L21592;}
    else {
      ps_error("VM's stack is too small (how can this happen?)", 0);
      goto L21592;}}}
 L21592: {
  list_1714X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg0K0 = count_435X;
  arg0K1 = list_1714X;
  goto L21601;}
 L21601: {
  i_1715X = arg0K0;
  l_1716X = arg0K1;
  if ((0 < i_1715X)) {
    x_1717X = *((long *) (((char *) (-3 + l_1716X))));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1717X;
    arg0K0 = (-1 + i_1715X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1716X))) + 4)));
    goto L21601;}
  else {
    push_list0_return_value = l_1716X;
    goto push_list_return;}}
 push_list_return:
  switch (push_list_return_tag) {
  case 0: goto push_list_return_0;
  case 1: goto push_list_return_1;
  case 2: goto push_list_return_2;
  case 3: goto push_list_return_3;
  default: goto push_list_return_4;
  }}

 pop_args_GlistSAgc: {
  start_432X = merged_arg0K0;
  count_433X = merged_arg0K1;{
  Stemp0S = start_432X;
  s48_make_availableAgc(((((3 * count_433X))<<2)));
  value_1718X = Stemp0S;
  Stemp0S = 1;
  arg0K0 = value_1718X;
  arg0K1 = count_433X;
  goto L16572;}
 L16572: {
  args_1719X = arg0K0;
  count_1720X = arg0K1;
  if ((0 == count_1720X)) {
    pop_args_GlistSAgc0_return_value = args_1719X;
    goto pop_args_GlistSAgc_return;}
  else {
    a_1721X = *((long *) (SstackS));
    SstackS = ((SstackS) + 4);
    addr_1722X = s48_allocate_small(12);
    *((long *) addr_1722X) = 2050;
    x_1723X = 3 + (((long) (addr_1722X + 4)));
    *((long *) (((char *) (-3 + x_1723X)))) = a_1721X;
    *((long *) ((((char *) (-3 + x_1723X))) + 4)) = args_1719X;
    arg0K0 = x_1723X;
    arg0K1 = (-1 + count_1720X);
    goto L16572;}}
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
  default: goto pop_args_GlistSAgc_return_9;
  }}

 copy_listSAgc: {
  list_430X = merged_arg0K0;
  length_431X = merged_arg0K1;{
  if ((0 == length_431X)) {
    copy_listSAgc0_return_value = 25;
    goto copy_listSAgc_return;}
  else {
    Stemp0S = list_430X;
    s48_make_availableAgc(((((3 * length_431X))<<2)));
    value_1724X = Stemp0S;
    Stemp0S = 1;
    a_1725X = *((long *) (((char *) (-3 + value_1724X))));
    addr_1726X = s48_allocate_small(12);
    *((long *) addr_1726X) = 2050;
    x_1727X = 3 + (((long) (addr_1726X + 4)));
    *((long *) (((char *) (-3 + x_1727X)))) = a_1725X;
    *((long *) ((((char *) (-3 + x_1727X))) + 4)) = 25;
    arg0K0 = (*((long *) ((((char *) (-3 + value_1724X))) + 4)));
    arg0K1 = x_1727X;
    goto L16460;}}
 L16460: {
  l_1728X = arg0K0;
  last_1729X = arg0K1;
  if ((25 == l_1728X)) {
    copy_listSAgc0_return_value = x_1727X;
    goto copy_listSAgc_return;}
  else {
    a_1730X = *((long *) (((char *) (-3 + l_1728X))));
    addr_1731X = s48_allocate_small(12);
    *((long *) addr_1731X) = 2050;
    x_1732X = 3 + (((long) (addr_1731X + 4)));
    *((long *) (((char *) (-3 + x_1732X)))) = a_1730X;
    *((long *) ((((char *) (-3 + x_1732X))) + 4)) = 25;
    addr_1733X = (((char *) (-3 + last_1729X))) + 4;
    S48_WRITE_BARRIER(last_1729X, addr_1733X, x_1732X);
    *((long *) addr_1733X) = x_1732X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_1728X))) + 4)));
    arg0K1 = x_1732X;
    goto L16460;}}
 copy_listSAgc_return:
  switch (copy_listSAgc_return_tag) {
  case 0: goto copy_listSAgc_return_0;
  case 1: goto copy_listSAgc_return_1;
  case 2: goto copy_listSAgc_return_2;
  default: goto copy_listSAgc_return_3;
  }}

 shift_space: {
  x_428X = merged_arg0K0;
  n_429X = merged_arg0K1;{
  if ((0 == (3 & x_428X))) {
    arg0K0 = 1;
    arg0K1 = 3;
    goto L17359;}
  else {
    arg0K0 = (-1 + ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_428X))) + -4))))>>8))))>>2)));
    arg0K1 = 0;
    goto L17359;}}
 L17359: {
  x_size_1734X = arg0K0;
  extra_1735X = arg0K1;
  if ((n_429X < 0)) {
    if ((x_size_1734X < 1)) {
      arg0K0 = 1;
      goto L17404;}
    else {
      arg0K0 = x_size_1734X;
      goto L17404;}}
  else {
    n_1736X = n_429X / 30;
    arg0K0 = (5 + (x_size_1734X + n_1736X));
    goto L17379;}}
 L17404: {
  v_1737X = arg0K0;
  arg0K0 = (9 + ((((v_1737X)<<1)) + x_size_1734X));
  goto L17379;}
 L17379: {
  v_1738X = arg0K0;
  shift_space0_return_value = (extra_1735X + v_1738X);
  goto shift_space_return;}
 shift_space_return:
  switch (shift_space_return_tag) {
  case 0: goto shift_space_return_0;
  default: goto shift_space_return_1;
  }}

 okay_copy_argsP: {
  thing_425X = merged_arg0K0;
  index_426X = merged_arg0K1;
  count_427X = merged_arg0K2;{
  if ((index_426X < 0)) {
    okay_copy_argsP0_return_value = 0;
    goto okay_copy_argsP_return;}
  else {
    if ((3 == (3 & thing_425X))) {
      if ((17 == (31 & ((((*((long *) ((((char *) (-3 + thing_425X))) + -4))))>>2))))) {
        if (((-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_425X))) + -4))))>>8))) < (index_426X + count_427X))) {
          okay_copy_argsP0_return_value = 0;
          goto okay_copy_argsP_return;}
        else {
          okay_copy_argsP0_return_value = 1;
          goto okay_copy_argsP_return;}}
      else {
        goto L15961;}}
    else {
      goto L15961;}}}
 L15961: {
  if ((3 == (3 & thing_425X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + thing_425X))) + -4))))>>2))))) {
      if ((((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_425X))) + -4))))>>8)) < (index_426X + count_427X))) {
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
  stob_422X = merged_arg0K0;
  index_423X = merged_arg0K1;
  value_424X = merged_arg0K2;{
  log_1739X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L15029;}
 L15029: {
  i_1740X = arg0K0;
  next_stob_1741X = *((long *) ((((char *) (-3 + log_1739X))) + (((i_1740X)<<2))));
  if ((1 == next_stob_1741X)) {
    add_log_entryAgc(1, i_1740X, stob_422X, index_423X, value_424X, 0);
    goto proposal_d_write_return;}
  else {
    if ((stob_422X == next_stob_1741X)) {
      if ((index_423X == (*((long *) ((((char *) (-3 + log_1739X))) + (4 + (((i_1740X)<<2)))))))) {
        addr_1742X = (((char *) (-3 + log_1739X))) + (8 + (((i_1740X)<<2)));
        S48_WRITE_BARRIER(log_1739X, addr_1742X, value_424X);
        *((long *) addr_1742X) = value_424X;
        goto proposal_d_write_return;}
      else {
        goto L15049;}}
    else {
      goto L15049;}}}
 L15049: {
  arg0K0 = (4 + i_1740X);
  goto L15029;}
 proposal_d_write_return:
  switch (proposal_d_write_return_tag) {
  case 0: goto proposal_d_write_return_0;
  case 1: goto proposal_d_write_return_1;
  default: goto proposal_d_write_return_2;
  }}

 proposal_d_read: {
  stob_420X = merged_arg0K0;
  index_421X = merged_arg0K1;{
  log_1743X = *((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (Scurrent_threadS)))) + 12)))))) + 4));
  arg0K0 = 0;
  goto L14852;}
 L14852: {
  i_1744X = arg0K0;
  next_stob_1745X = *((long *) ((((char *) (-3 + log_1743X))) + (((i_1744X)<<2))));
  if ((1 == next_stob_1745X)) {
    v_1746X = add_log_entryAgc(1, i_1744X, stob_420X, index_421X, (*((long *) ((((char *) (-3 + stob_420X))) + (-4 & index_421X)))), 1);
    proposal_d_read0_return_value = v_1746X;
    goto proposal_d_read_return;}
  else {
    if ((stob_420X == next_stob_1745X)) {
      if ((index_421X == (*((long *) ((((char *) (-3 + log_1743X))) + (4 + (((i_1744X)<<2)))))))) {
        proposal_d_read0_return_value = (*((long *) ((((char *) (-3 + log_1743X))) + (8 + (((i_1744X)<<2))))));
        goto proposal_d_read_return;}
      else {
        goto L14874;}}
    else {
      goto L14874;}}}
 L14874: {
  arg0K0 = (4 + i_1744X);
  goto L14852;}
 proposal_d_read_return:
  switch (proposal_d_read_return_tag) {
  case 0: goto proposal_d_read_return_0;
  case 1: goto proposal_d_read_return_1;
  default: goto proposal_d_read_return_2;
  }}

 make_closure: {
  a_418X = merged_arg0K0;
  b_419X = merged_arg0K1;{
  addr_1747X = s48_allocate_small(12);
  *((long *) addr_1747X) = 2062;
  x_1748X = 3 + (((long) (addr_1747X + 4)));
  *((long *) (((char *) (-3 + x_1748X)))) = a_418X;
  *((long *) ((((char *) (-3 + x_1748X))) + 4)) = b_419X;
  if ((3 == (3 & x_1748X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_1748X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_1748X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_1748X))) + -4))));
      make_closure0_return_value = x_1748X;
      goto make_closure_return;}
    else {
      make_closure0_return_value = x_1748X;
      goto make_closure_return;}}
  else {
    make_closure0_return_value = x_1748X;
    goto make_closure_return;}}
 make_closure_return:
  switch (make_closure_return_tag) {
  case 0: goto make_closure_return_0;
  default: goto make_closure_return_1;
  }}

 get_current_port: {
  marker_417X = merged_arg0K0;{
  thread_1749X = Scurrent_threadS;
  if ((3 == (3 & thread_1749X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + thread_1749X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_1749X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_1749X))) + 4)));
        goto L13056;}
      else {
        goto L13106;}}
    else {
      goto L13106;}}
  else {
    goto L13106;}}
 L13056: {
  env_1750X = arg0K0;
  if ((3 == (3 & env_1750X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_1750X))) + -4))))>>2))))) {
      obj_1751X = *((long *) (((char *) (-3 + env_1750X))));
      if ((3 == (3 & obj_1751X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_1751X))) + -4))))>>2))))) {
          if ((marker_417X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_1750X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_1750X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_1750X))) + 4)));
            goto L13056;}}
        else {
          goto L13128;}}
      else {
        goto L13128;}}
    else {
      goto L13128;}}
  else {
    goto L13128;}}
 L13106: {
  ps_error("current thread is not a record", 0);
  get_current_port0_return_value = v_1752X;
  goto get_current_port_return;}
 L13128: {
  if ((25 == env_1750X)) {
    if (((((marker_417X)>>2)) == 1)) {
      arg3K0 = "dynamic environment doesn't have current-output-port";
      goto L13082;}
    else {
      arg3K0 = "dynamic environment doesn't have current-input-port";
      goto L13082;}}
  else {
    ps_error("dynamic environment is not a proper list", 0);
    get_current_port0_return_value = v_1753X;
    goto get_current_port_return;}}
 L13082: {
  v_1754X = arg3K0;
  ps_error(v_1754X, 0);
  get_current_port0_return_value = v_1755X;
  goto get_current_port_return;}
 get_current_port_return:
  switch (get_current_port_return_tag) {
  case 0: goto get_current_port_return_0;
  case 1: goto get_current_port_return_1;
  default: goto get_current_port_return_2;
  }}

 okay_argument_list: {
  list_416X = merged_arg0K0;{
  arg0K0 = list_416X;
  arg0K1 = 0;
  arg0K2 = list_416X;
  arg2K3 = 0;
  goto L12923;}
 L12923: {
  fast_1756X = arg0K0;
  len_1757X = arg0K1;
  slow_1758X = arg0K2;
  move_slowP_1759X = arg2K3;
  if ((25 == fast_1756X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_1757X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_1756X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_1756X))) + -4))))>>2))))) {
        if (move_slowP_1759X) {
          if ((fast_1756X == slow_1758X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_1756X))) + 4)));
            arg0K1 = (1 + len_1757X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_1758X))) + 4)));
            arg2K3 = 0;
            goto L12923;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_1756X))) + 4)));
          arg0K1 = (1 + len_1757X);
          arg0K2 = slow_1758X;
          arg2K3 = 1;
          goto L12923;}}
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
  cont_414X = merged_arg0K0;
  stack_arg_count_415X = merged_arg0K1;{
  new_cont_1760X = (Sbottom_of_stackS) + (0 - (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_414X))) + -4))))>>8)))));
  if ((0 == stack_arg_count_415X)) {
    SstackS = new_cont_1760X;
    goto L12781;}
  else {
    new_stack_1761X = new_cont_1760X + (0 - (((stack_arg_count_415X)<<2)));
    if ((new_stack_1761X < (SstackS))) {
      memcpy((void *)new_stack_1761X, (void *)(SstackS),(((stack_arg_count_415X)<<2)));
      SstackS = new_stack_1761X;
      goto L12781;}
    else {
      goto L12781;}}}
 L12781: {
  ScontS = new_cont_1760X;
  v_1762X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_414X))) + -4))))>>8))))>>2);
  memcpy((void *)(new_cont_1760X + 4), (void *)((((char *) (-3 + cont_414X))) + 4),(-4 + (((v_1762X)<<2))));
  *((long *) new_cont_1760X) = (((long) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_414X))) + 8))))))))))) + ((((*((long *) (((char *) (-3 + cont_414X))))))>>2)))));
  *((long *) ((Sbottom_of_stackS) + 4)) = (*((long *) (new_cont_1760X + 4)));
  *((long *) (new_cont_1760X + 4)) = (((long) (Sbottom_of_stackS)));
  copy_continuation_from_heapB0_return_value = new_cont_1760X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  case 1: goto copy_continuation_from_heapB_return_1;
  default: goto copy_continuation_from_heapB_return_2;
  }}

 get_error_string: {
  status_413X = merged_arg0K0;{
  string_1763X = ps_error_string(status_413X);
  x_1764X = strlen((char *) string_1763X);
  if ((x_1764X < 256)) {
    arg0K0 = x_1764X;
    goto L12124;}
  else {
    arg0K0 = 256;
    goto L12124;}}
 L12124: {
  len_1765X = arg0K0;
  len_1766X = 1 + len_1765X;
  addr_1767X = s48_allocate_small((4 + len_1766X));
  *((long *) addr_1767X) = (70 + (((len_1766X)<<8)));
  string_1768X = 3 + (((long) (addr_1767X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1768X))) + len_1765X)) = 0;
  arg0K0 = 0;
  goto L12134;}
 L12134: {
  i_1769X = arg0K0;
  if ((i_1769X == len_1765X)) {
    get_error_string0_return_value = string_1768X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1768X))) + i_1769X)) = ((*(string_1763X + i_1769X)));
    arg0K0 = (1 + i_1769X);
    goto L12134;}}
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
  nargs_412X = merged_arg0K0;{
  top_of_args_1770X = SstackS;
  if (((ScontS) == (top_of_args_1770X + (((nargs_412X)<<2))))) {
    goto move_args_above_contB_return;}
  else {
    SstackS = (ScontS);
    arg1K0 = ((SstackS) + -4);
    arg1K1 = (top_of_args_1770X + (-4 + (((nargs_412X)<<2))));
    goto L5476;}}
 L5476: {
  loc_1771X = arg1K0;
  arg_1772X = arg1K1;
  if ((arg_1772X < top_of_args_1770X)) {
    SstackS = ((SstackS) + (0 - (((nargs_412X)<<2))));
    goto move_args_above_contB_return;}
  else {
    *((long *) loc_1771X) = (*((long *) arg_1772X));
    arg1K0 = (loc_1771X + -4);
    arg1K1 = (arg_1772X + -4);
    goto L5476;}}
 move_args_above_contB_return:
  switch (move_args_above_contB_return_tag) {
  case 0: goto move_args_above_contB_return_0;
  default: goto move_args_above_contB_return_1;
  }}

}
long s48_call_startup_procedure(char **startup_vector_1773X, long startup_vector_length_1774X)
{
  long arg0K0;
  long merged_arg0K1;
  char *merged_arg3K0;

  int enter_string_return_tag;
  long enter_string0_return_value;
  char *string_1775X;
  long i_1815X;
  long string_1814X;
  char * addr_1813X;
  long len_1812X;
  long len_1811X;
  long v_1810X;
  long x_1809X;
  long x_1808X;
  char * addr_1807X;
  long b_1806X;
  long channel_1805X;
  long x_1804X;
  char * addr_1803X;
  long b_1802X;
  long channel_1801X;
  long x_1800X;
  char * addr_1799X;
  long b_1798X;
  long channel_1797X;
  long i_1796X;
  long length_1795X;
  long *v_1794X;
  long v_1793X;
  long v_1792X;
  long y_1791X;
  long x_1790X;
  long v_1789X;
  long x_1788X;
  long y_1787X;
  char * addr_1786X;
  long value_1785X;
  long x_1784X;
  long y_1783X;
  long i_1782X;
  long vector_1781X;
  long v_1780X;
  long vector_1779X;
  char * addr_1778X;
  long len_1777X;
  long tem_1776X;
 {  SstackS = (Sbottom_of_stackS);
  *((long *) ((Sbottom_of_stackS) + 4)) = 1;
  ScontS = (Sbottom_of_stackS);
  SenvS = 13;
  tem_1776X = Sinterrupt_templateS;
  StemplateS = tem_1776X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1776X))))))));
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
  len_1777X = ((startup_vector_length_1774X)<<2);
  addr_1778X = s48_allocate_tracedAgc((4 + len_1777X));
  if ((addr_1778X == NULL)) {
    arg0K0 = 1;
    goto L17011;}
  else {
    *((long *) addr_1778X) = (10 + (((len_1777X)<<8)));
    arg0K0 = (3 + (((long) (addr_1778X + 4))));
    goto L17011;}}
 L17011: {
  vector_1779X = arg0K0;
  if ((1 == vector_1779X)) {
    ps_error("Out of space, unable to allocate", 0);
    arg0K0 = v_1780X;
    goto L16997;}
  else {
    arg0K0 = vector_1779X;
    goto L16997;}}
 L16997: {
  vector_1781X = arg0K0;
  arg0K0 = 0;
  goto L17034;}
 L17034: {
  i_1782X = arg0K0;
  if ((i_1782X == startup_vector_length_1774X)) {
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = vector_1781X;
    y_1783X = fileno((stderr));
    x_1784X = fileno((stdout));
    if ((x_1784X < y_1783X)) {
      arg0K0 = y_1783X;
      goto L18589;}
    else {
      arg0K0 = x_1784X;
      goto L18589;}}
  else {
    value_1785X = enter_stringAgc((*(startup_vector_1773X + i_1782X)));
    addr_1786X = (((char *) (-3 + vector_1781X))) + (((i_1782X)<<2));
    S48_WRITE_BARRIER(vector_1781X, addr_1786X, value_1785X);
    *((long *) addr_1786X) = value_1785X;
    arg0K0 = (1 + i_1782X);
    goto L17034;}}
 L18589: {
  y_1787X = arg0K0;
  x_1788X = fileno((stdin));
  if ((x_1788X < y_1787X)) {
    arg0K0 = y_1787X;
    goto L18591;}
  else {
    arg0K0 = x_1788X;
    goto L18591;}}
 L18591: {
  v_1789X = arg0K0;
  x_1790X = Snumber_of_channelsS;
  y_1791X = 1 + v_1789X;
  if ((x_1790X < y_1791X)) {
    arg0K0 = y_1791X;
    goto L18593;}
  else {
    arg0K0 = x_1790X;
    goto L18593;}}
 L18593: {
  v_1792X = arg0K0;
  Snumber_of_channelsS = v_1792X;
  v_1793X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L18615;}
  else {
    goto L18615;}}
 L18615: {
  v_1794X = Svm_channelsS;
  length_1795X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L18663;}
 L18663: {
  i_1796X = arg0K0;
  if ((i_1796X < length_1795X)) {
    *(v_1794X + i_1796X) = 1;
    arg0K0 = (1 + i_1796X);
    goto L18663;}
  else {
    s48_make_availableAgc(((((3 * (7 + ((((4 + (strlen((char *) "standard output"))))>>2)))))<<2)));
    channel_1797X = fileno((stdin));
    merged_arg3K0 = "standard input";
    merged_arg0K1 = 0;
    enter_string_return_tag = 0;
    goto enter_string;
   enter_string_return_0:
    b_1798X = enter_string0_return_value;
    addr_1799X = s48_allocate_small(24);
    *((long *) addr_1799X) = 5146;
    x_1800X = 3 + (((long) (addr_1799X + 4)));
    *((long *) (((char *) (-3 + x_1800X)))) = 4;
    *((long *) ((((char *) (-3 + x_1800X))) + 4)) = b_1798X;
    *((long *) ((((char *) (-3 + x_1800X))) + 8)) = (((channel_1797X)<<2));
    *((long *) ((((char *) (-3 + x_1800X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1800X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1797X) = x_1800X;
    channel_1801X = fileno((stderr));
    merged_arg3K0 = "standard error";
    merged_arg0K1 = 0;
    enter_string_return_tag = 1;
    goto enter_string;
   enter_string_return_1:
    b_1802X = enter_string0_return_value;
    addr_1803X = s48_allocate_small(24);
    *((long *) addr_1803X) = 5146;
    x_1804X = 3 + (((long) (addr_1803X + 4)));
    *((long *) (((char *) (-3 + x_1804X)))) = 8;
    *((long *) ((((char *) (-3 + x_1804X))) + 4)) = b_1802X;
    *((long *) ((((char *) (-3 + x_1804X))) + 8)) = (((channel_1801X)<<2));
    *((long *) ((((char *) (-3 + x_1804X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1804X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1801X) = x_1804X;
    channel_1805X = fileno((stdout));
    merged_arg3K0 = "standard output";
    merged_arg0K1 = 0;
    enter_string_return_tag = 2;
    goto enter_string;
   enter_string_return_2:
    b_1806X = enter_string0_return_value;
    addr_1807X = s48_allocate_small(24);
    *((long *) addr_1807X) = 5146;
    x_1808X = 3 + (((long) (addr_1807X + 4)));
    *((long *) (((char *) (-3 + x_1808X)))) = 8;
    *((long *) ((((char *) (-3 + x_1808X))) + 4)) = b_1806X;
    *((long *) ((((char *) (-3 + x_1808X))) + 8)) = (((channel_1805X)<<2));
    *((long *) ((((char *) (-3 + x_1808X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1808X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1805X) = x_1808X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1800X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1808X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1804X;
    x_1809X = s48_resumer_records();
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1809X;
    s48_initialization_completeB();
    v_1810X = s48_startup_procedure();
    return s48_restart(v_1810X, 5);}}
 enter_string: {
  string_1775X = merged_arg3K0;{
  len_1811X = strlen((char *) string_1775X);
  len_1812X = 1 + len_1811X;
  addr_1813X = s48_allocate_small((4 + len_1812X));
  *((long *) addr_1813X) = (70 + (((len_1812X)<<8)));
  string_1814X = 3 + (((long) (addr_1813X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1814X))) + len_1811X)) = 0;
  arg0K0 = 0;
  goto L14657;}
 L14657: {
  i_1815X = arg0K0;
  if ((i_1815X == len_1811X)) {
    enter_string0_return_value = string_1814X;
    goto enter_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1814X))) + i_1815X)) = ((*(string_1775X + i_1815X)));
    arg0K0 = (1 + i_1815X);
    goto L14657;}}
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
Simpure_area_countS = 0;
Sexternal_exceptionPS = 0;
Simported_bindingsS = 1;
Sexported_bindingsS = 1;
Sexternal_root_stackS = NULL;
Sexternal_root_stack_baseS = NULL;
Spermanent_external_rootsS = NULL;
Spost_gc_cleanupS = HtopD10292;
Sgc_root_procS = HtopD10303;
s48_Scallback_return_stack_blockS = 1;
s48_Spending_eventsPS = 0;
}
