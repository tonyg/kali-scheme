#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

static void mark_traced_channels_closedB(void);
static long enter_string(char*);
void set_extension_valueB(long);
void note_event(void);
long required_init_space(char**, long);
void initialize_vm(char *, long);
long restart(long);
long call_startup_procedure(long, char**, long);
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
static long Slosing_opcodeS;
static long Sexternal_call_nargsS;
char Spending_interruptPS;
long Sextension_valueS;
char Spending_eventsPS;

static void mark_traced_channels_closedB(void)
{
  long arg0K0;
  char * addr_5X;
  char * addr_4X;
  long descriptor_3X;
  long header_2X;
  long channel_1X;
  long i_0X;
 {  arg0K0 = 0;
  goto L5357;}
 L5357: {
  i_0X = arg0K0;
  if ((i_0X == (Snumber_of_channelsS))) {
    return;}
  else {
    channel_1X = *((Svm_channelsS) + i_0X);
    if ((1 == channel_1X)) {
      goto L5417;}
    else {
      header_2X = *((long *) ((((char *) (-3 + channel_1X))) + -4));
      if ((3 == (3 & header_2X))) {
        ps_write_string("Channel closed in dumped image: ", (stderr));
        descriptor_3X = *((long *) ((((char *) (-3 + channel_1X))) + 4));
        if ((0 == (3 & descriptor_3X))) {
          ps_write_integer(((((*((long *) ((((char *) (-3 + channel_1X))) + 4))))>>2)), (stderr));
          goto L5403;}
        else {
          ps_write_string((((char *)(((char *) (-3 + (*((long *) ((((char *) (-3 + channel_1X))) + 4)))))))), (stderr));
          goto L5403;}}
      else {
        goto L5417;}}}}
 L5417: {
  arg0K0 = (1 + i_0X);
  goto L5357;}
 L5403: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  addr_4X = ((char *) (-3 + header_2X));
  WRITE_BARRIER(addr_4X, 0);
  *((long *) addr_4X) = 0;
  addr_5X = (((char *) (-3 + header_2X))) + 8;
  WRITE_BARRIER(addr_5X, -4);
  *((long *) addr_5X) = -4;
  goto L5417;}
}
static long enter_string(char *string_6X)
{
  long arg0K0;
  long i_11X;
  long string_10X;
  char * addr_9X;
  long len_8X;
  long z_7X;
 {  z_7X = strlen((char *) string_6X);
  len_8X = 1 + z_7X;
  addr_9X = ALLOCATE_SPACE(16, (4 + len_8X));
  *((long *) addr_9X) = (66 + (((len_8X)<<8)));
  string_10X = 3 + (((long) (addr_9X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_10X))) + z_7X)) = 0;
  arg0K0 = 0;
  goto L7610;}
 L7610: {
  i_11X = arg0K0;
  if ((i_11X < z_7X)) {
    *((unsigned char *) ((((char *) (-3 + string_10X))) + i_11X)) = ((*(string_6X + i_11X)));
    arg0K0 = (1 + i_11X);
    goto L7610;}
  else {
    return string_10X;}}
}
void set_extension_valueB(long value_12X)
{

 {  Sextension_valueS = value_12X;
  return;}
}
void note_event(void)
{

 {  Spending_eventsPS = 1;
  Spending_interruptPS = 1;
  return;}
}
long required_init_space(char **startup_vector_13X, long startup_vector_length_14X)
{
  long arg0K1;
  long arg0K0;
  long s_16X;
  long i_15X;
 {  arg0K0 = 0;
  arg0K1 = 0;
  goto L3401;}
 L3401: {
  i_15X = arg0K0;
  s_16X = arg0K1;
  if ((i_15X < startup_vector_length_14X)) {
    arg0K0 = (1 + i_15X);
    arg0K1 = (1 + (s_16X + ((((4 + (strlen((char *) (*(startup_vector_13X + i_15X))))))>>2))));
    goto L3401;}
  else {
    return (50 + s_16X);}}
}
void initialize_vm(char * stack_begin_17X, long stack_size_18X)
{
  long arg0K1;
  char * arg1K0;
  char * addr_38X;
  long code_37X;
  long temp_36X;
  char * addr_35X;
  char * addr_34X;
  char * addr_33X;
  long code_32X;
  long temp_31X;
  char * addr_30X;
  char * addr_29X;
  long cont_28X;
  char * addr_27X;
  long code_26X;
  long temp_25X;
  char * addr_24X;
  char * addr_23X;
  char * a_22X;
  long size_21X;
  char * start_20X;
  char * stack_19X;
 {  if ((stack_size_18X < 8125)) {
    stack_19X = (char *)malloc(32500);
    if ((stack_19X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_19X;
      arg0K1 = 8125;
      goto L9389;}
    else {
      arg1K0 = stack_19X;
      arg0K1 = 8125;
      goto L9389;}}
  else {
    arg1K0 = stack_begin_17X;
    arg0K1 = stack_size_18X;
    goto L9389;}}
 L9389: {
  start_20X = arg1K0;
  size_21X = arg0K1;
  Sstack_beginS = start_20X;
  Sstack_endS = (start_20X + (((size_21X)<<2)));
  Sstack_limitS = ((Sstack_beginS) + 500);
  SstackS = ((Sstack_endS) + -4);
  ScontS = 1;
  SenvS = 17;
  arg1K0 = start_20X;
  goto L9421;}
 L9421: {
  a_22X = arg1K0;
  if ((a_22X == (Sstack_endS))) {
    addr_23X = ALLOCATE_SPACE(17, 6);
    *((long *) addr_23X) = 582;
    addr_24X = ALLOCATE_SPACE(11, 12);
    *((long *) addr_24X) = 2094;
    temp_25X = 3 + (((long) (addr_24X + 4)));
    code_26X = 3 + (((long) (addr_23X + 4)));
    addr_27X = ((char *) (-3 + temp_25X));
    WRITE_BARRIER(addr_27X, code_26X);
    *((long *) addr_27X) = code_26X;
    *((unsigned char *) (((char *) (-3 + code_26X)))) = 23;
    *((unsigned char *) ((((char *) (-3 + code_26X))) + 1)) = 31;
    SstackS = ((SstackS) + -20);
    *((long *) ((SstackS) + 4)) = 4262;
    cont_28X = 3 + (((long) ((SstackS) + 8)));
    *((long *) ((((char *) (-3 + cont_28X))) + 4)) = 0;
    *((long *) ((((char *) (-3 + cont_28X))) + 8)) = temp_25X;
    *((long *) ((((char *) (-3 + cont_28X))) + 12)) = (SenvS);
    *((long *) (((char *) (-3 + cont_28X)))) = (ScontS);
    ScontS = cont_28X;
    Sbottom_of_stackS = (ScontS);
    addr_29X = ALLOCATE_SPACE(17, 6);
    *((long *) addr_29X) = 582;
    addr_30X = ALLOCATE_SPACE(11, 12);
    *((long *) addr_30X) = 2094;
    temp_31X = 3 + (((long) (addr_30X + 4)));
    code_32X = 3 + (((long) (addr_29X + 4)));
    addr_33X = ((char *) (-3 + temp_31X));
    WRITE_BARRIER(addr_33X, code_32X);
    *((long *) addr_33X) = code_32X;
    *((unsigned char *) (((char *) (-3 + code_32X)))) = 34;
    *((unsigned char *) ((((char *) (-3 + code_32X))) + 1)) = 138;
    Sinterrupt_templateS = temp_31X;
    addr_34X = ALLOCATE_SPACE(17, 6);
    *((long *) addr_34X) = 582;
    addr_35X = ALLOCATE_SPACE(11, 12);
    *((long *) addr_35X) = 2094;
    temp_36X = 3 + (((long) (addr_35X + 4)));
    code_37X = 3 + (((long) (addr_34X + 4)));
    addr_38X = ((char *) (-3 + temp_36X));
    WRITE_BARRIER(addr_38X, code_37X);
    *((long *) addr_38X) = code_37X;
    *((unsigned char *) (((char *) (-3 + code_37X)))) = 135;
    *((unsigned char *) ((((char *) (-3 + code_37X))) + 1)) = 121;
    Sexception_templateS = temp_36X;
    return;}
  else {
    *((long *) a_22X) = 252645135;
    arg1K0 = (a_22X + 4);
    goto L9421;}}
}
long restart(long value_39X)
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
  char * merged_arg1K0;
  long merged_arg0K3;
  long merged_arg0K2;
  long merged_arg0K1;
  long merged_arg0K0;

  int pop_continuationB_return_tag;
  int make_closure_return_tag;
  long make_closure0_return_value;
  int get_error_string_return_tag;
  long get_error_string0_return_value;
  int push_continuationB_return_tag;
  int push_exception_continuationB_return_tag;
  int okay_argument_list_return_tag;
  char okay_argument_list0_return_value;
  long okay_argument_list1_return_value;
  int get_current_port_return_tag;
  long get_current_port0_return_value;
  int copy_continuation_from_heapB_return_tag;
  long copy_continuation_from_heapB0_return_value;
  int close_channelB_return_tag;
  long close_channelB0_return_value;
  int channel_close_error_return_tag;
  long channel_close_error0_return_value;
  int collect_saving_temp_return_tag;
  long collect_saving_temp0_return_value;
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
  long a_40X;
  long b_41X;
  long status_42X;
  char * code_pointer_43X;
  long size_44X;
  long exception_45X;
  long instruction_size_46X;
  long list_47X;
  long marker_48X;
  long cont_49X;
  long channel_50X;
  long status_51X;
  long index_52X;
  long id_53X;
  long value_54X;
  long list_55X;
  long length_56X;
  long start_57X;
  long count_58X;
  long env_59X;
  long cont_60X;
  long key_61X;
  long reason_62X;
  long env_63X;
  long key_64X;
  long reason_65X;
  long key_66X;
  long list_67X;
  long count_68X;
  long wants_stack_args_69X;
  long stack_arg_count_70X;
  long list_args_71X;
  long list_arg_count_72X;
  char *message_73X;
  long pc_1006X;
  long tem_1005X;
  long cont_1004X;
  long x_1003X;
  char * addr_1002X;
  long i_1001X;
  long string_1000X;
  char * addr_999X;
  long len_998X;
  long len_997X;
  long x_996X;
  char *string_995X;
  long cont_994X;
  long template_993X;
  long pc_992X;
  long v_991X;
  long i_990X;
  char * p_989X;
  long tem_988X;
  long opcode_987X;
  char move_slowP_986X;
  long slow_985X;
  long len_984X;
  long fast_983X;
  long v_982X;
  long v_981X;
  long obj_980X;
  long env_979X;
  long v_978X;
  long v_977X;
  long v_976X;
  long thread_975X;
  long v_974X;
  long new_cont_973X;
  char * top_972X;
  char * addr_971X;
  long status_970X;
  long v_969X;
  long v_968X;
  long v_967X;
  long os_index_966X;
  long v_965X;
  long status_964X;
  long id_963X;
  long new_962X;
  long id_961X;
  long header_960X;
  long channel_959X;
  long tem_958X;
  long i_957X;
  char * addr_956X;
  long l_955X;
  long v_954X;
  long env_953X;
  long l2_952X;
  long sickly_951X;
  long okay_950X;
  long value_949X;
  long env_948X;
  long last_env_947X;
  long cont_946X;
  long env_945X;
  long v_944X;
  long env_943X;
  long p_942X;
  long arg_count_941X;
  long i_940X;
  char * p_939X;
  long unused_938X;
  char * a_937X;
  long v_936X;
  long v_935X;
  long env_934X;
  long v_933X;
  long v_932X;
  long v_931X;
  long v_930X;
  long v_929X;
  long v_928X;
  long v_927X;
  long v_926X;
  long v_925X;
  long v_924X;
  long pc_923X;
  char * addr_922X;
  long x_921X;
  char * addr_920X;
  long a_919X;
  long last_918X;
  long l_917X;
  long x_916X;
  char * addr_915X;
  long a_914X;
  long list_913X;
  long temp_912X;
  char okayP_911X;
  char v_910X;
  long temp_909X;
  char v_908X;
  long space_907X;
  long x_906X;
  char * addr_905X;
  long a_904X;
  long count_903X;
  long args_902X;
  long start_901X;
  long temp_900X;
  char okayP_899X;
  char v_898X;
  long temp_897X;
  char v_896X;
  long space_895X;
  long env_894X;
  long cont_893X;
  char * addr_892X;
  long new_891X;
  long p_890X;
  long env_889X;
  long top_888X;
  char * addr_887X;
  long new_886X;
  char * data_addr_885X;
  char * addr_884X;
  long header_883X;
  long new_882X;
  char * data_addr_881X;
  char * addr_880X;
  long header_879X;
  long p_878X;
  long previous_877X;
  long cont_876X;
  long end_875X;
  long v_874X;
  long p_873X;
  char * arg_872X;
  char * loc_871X;
  long stob_870X;
  char * top_869X;
  long arg_count_868X;
  long i_867X;
  char * p_866X;
  long v_865X;
  long l_864X;
  long i_863X;
  long key_862X;
  char okayP_861X;
  long list_860X;
  char v_859X;
  char v_858X;
  long space_857X;
  long x_856X;
  long v_855X;
  long x_854X;
  long v_853X;
  long count_852X;
  long x_851X;
  long v_850X;
  char * addr_849X;
  char * addr_848X;
  long val_847X;
  long channel_846X;
  long status_845X;
  long channel_844X;
  long type_843X;
  char v_842X;
  long template_841X;
  char not_firstP_840X;
  long cont_839X;
  char not_firstP_838X;
  FILE * out_837X;
  long current_template_836X;
  long why_835X;
  long x_834X;
  long x_833X;
  long i_832X;
  long status_831X;
  long v_830X;
  long n_829X;
  long x_828X;
  long status_827X;
  char pendingP_826X;
  char eofP_825X;
  long got_824X;
  long length_823X;
  long i_822X;
  long v_821X;
  long reason_820X;
  long channel_819X;
  long c_818X;
  long b_817X;
  long v_816X;
  long obj_815X;
  long value_814X;
  long env_813X;
  long key_812X;
  char * addr_811X;
  long x_810X;
  char * addr_809X;
  long b_808X;
  long reason_807X;
  long *new_vm_channels_806X;
  long new_count_805X;
  long x_804X;
  char * addr_803X;
  long n_802X;
  long a_801X;
  long v_800X;
  long key_799X;
  char okayP_798X;
  char v_797X;
  long temp_796X;
  long key_795X;
  char okayP_794X;
  long y_793X;
  long val_792X;
  char * addr_791X;
  long x_790X;
  char * addr_789X;
  long new_788X;
  char * addr_787X;
  char * addr_786X;
  long symbols_785X;
  long table_784X;
  char * addr_783X;
  long i_782X;
  long bucket_781X;
  long index_780X;
  long h_779X;
  long i_778X;
  long x_777X;
  long status_776X;
  char pendingP_775X;
  long got_774X;
  long length_773X;
  long count_772X;
  long start_771X;
  char waitP_770X;
  long x_769X;
  long status_768X;
  long channel_767X;
  long v_766X;
  long v_765X;
  long y_764X;
  long x_763X;
  char temp_762X;
  long index_761X;
  long a_760X;
  long q_759X;
  long final_stack_arg_count_758X;
  char v_757X;
  char v_756X;
  long space_755X;
  long stack_arg_count_754X;
  long stack_slots_753X;
  char x_752X;
  long channel_751X;
  long obj_750X;
  long arg_count_749X;
  char * addr_748X;
  long value_747X;
  long i_746X;
  long env_745X;
  char v_744X;
  long temp_743X;
  char v_742X;
  long env_741X;
  char *v_740X;
  long y_739X;
  long len_738X;
  long s2_737X;
  long x_736X;
  char * addr_735X;
  long b_734X;
  long i_733X;
  long l_732X;
  long n_731X;
  long s_730X;
  long v_729X;
  long obj_728X;
  char v_727X;
  long i_726X;
  long x_725X;
  long status_724X;
  long reason_723X;
  long reason_722X;
  long x_721X;
  long obj_720X;
  long v_719X;
  long v_718X;
  long rest_list_717X;
  long i_716X;
  long n_715X;
  long args_714X;
  long skip_713X;
  long template_712X;
  long stack_arg_count_711X;
  long skip_710X;
  long skip_709X;
  long template_708X;
  long stack_arg_count_707X;
  long skip_706X;
  char * addr_705X;
  long next_704X;
  long channel_703X;
  long m_702X;
  long i_701X;
  long value_700X;
  long env_699X;
  long back_698X;
  long value_697X;
  long index_696X;
  long i_695X;
  long v_694X;
  long p_693X;
  long key_692X;
  long bucket_691X;
  long index_690X;
  long h_689X;
  long i_688X;
  long string_687X;
  char * addr_686X;
  long len_685X;
  char temp_684X;
  char minutesP_683X;
  char firstP_682X;
  long symbols_681X;
  long table_680X;
  long status_679X;
  long status_678X;
  long status_677X;
  long status_676X;
  long status_675X;
  long status_674X;
  FILE * port_673X;
  char x_672X;
  long x_671X;
  long obj_670X;
  long arg3_669X;
  long arg2_668X;
  long key_667X;
  char * addr_666X;
  char * addr_665X;
  long val_664X;
  char v_663X;
  long channel_662X;
  long count_661X;
  long start_660X;
  long obj_659X;
  long arg4_658X;
  long arg3_657X;
  long arg2_656X;
  long key_655X;
  long arg5_654X;
  long arg4_653X;
  long arg3_652X;
  long arg2_651X;
  long key_650X;
  long x_649X;
  long status_648X;
  long x_647X;
  long obj_646X;
  long key_645X;
  long mode_644X;
  long arg2_643X;
  long key_642X;
  long i_641X;
  long i_640X;
  char * addr_639X;
  long i_638X;
  long i_637X;
  long i_636X;
  long c_635X;
  long b_634X;
  long c_633X;
  long b_632X;
  long c_631X;
  long b_630X;
  long c_629X;
  long mid_c_628X;
  long v_627X;
  long v_626X;
  long lo_c_625X;
  long hi_b_624X;
  long hi_a_623X;
  long lo_b_622X;
  long lo_a_621X;
  long b_620X;
  long v_619X;
  long v_618X;
  long args_617X;
  long next_616X;
  long next_op_615X;
  long cont_614X;
  long index_613X;
  long length_612X;
  long wants_stack_args_611X;
  long skip_610X;
  long skip_609X;
  long stack_space_608X;
  long protocol_607X;
  long index_606X;
  long length_605X;
  long wants_stack_args_604X;
  long skip_603X;
  long skip_602X;
  long stack_space_601X;
  long protocol_600X;
  long n_599X;
  long tem_598X;
  long stack_arg_count_597X;
  long v_596X;
  long v_595X;
  long key_594X;
  long new_env_593X;
  char * addr_592X;
  long len_591X;
  long key_590X;
  long key_589X;
  char okayP_588X;
  long obj_587X;
  long obj_586X;
  long type_585X;
  long thing_584X;
  long stuff_583X;
  char * addr_582X;
  long val_581X;
  long x_580X;
  long i_579X;
  long b_578X;
  long p_577X;
  long port_576X;
  long Kchar_575X;
  long x_574X;
  long i_573X;
  long b_572X;
  long p_571X;
  long p_570X;
  long port_569X;
  char * addr_568X;
  long val_567X;
  long x_566X;
  long i_565X;
  long b_564X;
  long p_563X;
  long p_562X;
  long port_561X;
  long list_560X;
  long head_559X;
  char move_slowP_558X;
  long slow_557X;
  long list_556X;
  long n_555X;
  long v_554X;
  long table_553X;
  long obj_552X;
  long arg2_551X;
  long n_550X;
  long arg2_549X;
  long h_548X;
  long i_547X;
  long val_546X;
  long v_545X;
  long (*v_544X)(long, char *);
  long option_543X;
  long mseconds_542X;
  long seconds_541X;
  long option_540X;
  long proc_539X;
  long rest_list_538X;
  long x_537X;
  long tem_536X;
  long pc_535X;
  long v_534X;
  long vector_533X;
  long type_532X;
  char firstP_531X;
  long v_530X;
  long symbols_529X;
  char firstP_528X;
  long table_527X;
  long vector_526X;
  char firstP_525X;
  long x_524X;
  char * addr_523X;
  long b_522X;
  long x_521X;
  char * addr_520X;
  long x_519X;
  long arg2_518X;
  long key_517X;
  char okayP_516X;
  long y_515X;
  long n_514X;
  long prev_513X;
  long ch_512X;
  long val_511X;
  long key_510X;
  char okayP_509X;
  long key_508X;
  char okayP_507X;
  long key_506X;
  char okayP_505X;
  long key_504X;
  char okayP_503X;
  long val_502X;
  char * addr_501X;
  char * addr_500X;
  long val_499X;
  long string_498X;
  char * addr_497X;
  long len_496X;
  long vector_495X;
  char * addr_494X;
  long value_493X;
  char * addr_492X;
  long len_491X;
  long init_490X;
  char okayP_489X;
  long rest_list_488X;
  long stack_nargs_487X;
  long new_486X;
  char * addr_485X;
  long len_484X;
  long type_483X;
  long new_482X;
  char * addr_481X;
  long len_480X;
  long type_479X;
  long value_478X;
  long val_477X;
  long val_476X;
  long val_475X;
  long val_474X;
  long r_473X;
  long n_472X;
  long a_471X;
  long a_470X;
  long val_469X;
  long val_468X;
  long val_467X;
  long val_466X;
  long val_465X;
  long a_464X;
  long a_463X;
  long val_462X;
  long val_461X;
  long delta_460X;
  long delta_459X;
  long offset_458X;
  long key_457X;
  char okayP_456X;
  char v_455X;
  long tem_454X;
  long key_453X;
  char okayP_452X;
  long list_arg_count_451X;
  long list_args_450X;
  long stack_nargs_449X;
  long obj_448X;
  char * addr_447X;
  long list_args_446X;
  long follower_445X;
  long list_444X;
  long args_443X;
  long list_arg_count_442X;
  char okayP_441X;
  long stack_nargs_440X;
  long list_args_439X;
  long total_arg_count_438X;
  long code_437X;
  long obj_436X;
  long obj_435X;
  long list_arg_count_434X;
  long list_args_433X;
  long stack_arg_count_432X;
  long obj_431X;
  char * arg_430X;
  char * loc_429X;
  long args_428X;
  long v_427X;
  long v_426X;
  long v_425X;
  long list_arg_count_424X;
  long list_args_423X;
  long stack_arg_count_422X;
  long exception_421X;
  long code_420X;
  long stack_arg_count_419X;
  char v_418X;
  long key_417X;
  char okayP_416X;
  long key_415X;
  char okayP_414X;
  long key_413X;
  char okayP_412X;
  char v_411X;
  char v_410X;
  long space_409X;
  char * addr_408X;
  long value_407X;
  long index_406X;
  long i_405X;
  long env_404X;
  long i_403X;
  long env_402X;
  long i_401X;
  long env_400X;
  long obj_399X;
  long opcode_398X;
  long nargs_397X;
  FILE * out_396X;
  long x_395X;
  long v_394X;
  long v_393X;
  long v_392X;
  long v_391X;
  long v_390X;
  long v_389X;
  long count_388X;
  long to_index_387X;
  long from_index_386X;
  long arg5_385X;
  long arg4_384X;
  long arg3_383X;
  long arg2_382X;
  char * addr_381X;
  long len_380X;
  long index_379X;
  long x_378X;
  long arg4_377X;
  long arg3_376X;
  long arg2_375X;
  long len_374X;
  long index_373X;
  long arg3_372X;
  long arg2_371X;
  long x_370X;
  long arg2_369X;
  char v_368X;
  char v_367X;
  char v_366X;
  char v_365X;
  long space_364X;
  long n_363X;
  long x_362X;
  long obj_361X;
  long len_360X;
  long x_359X;
  long obj_358X;
  long arg2_357X;
  long arg2_356X;
  long status_355X;
  long key_354X;
  long x_353X;
  long arg2_352X;
  long mseconds_351X;
  long seconds_350X;
  long mseconds_349X;
  long seconds_348X;
  long x_347X;
  long option_346X;
  long x_345X;
  long arg2_344X;
  long rest_list_343X;
  long nargs_342X;
  char v_341X;
  long name_340X;
  long value_339X;
  long x_338X;
  long obj_337X;
  long x_336X;
  long arg2_335X;
  long p_334X;
  long p_333X;
  long p_332X;
  long old_331X;
  long temp_330X;
  long obj_329X;
  long p_328X;
  long template_327X;
  long p_326X;
  long temp_325X;
  long obj_324X;
  long x_323X;
  long x_322X;
  long obj_321X;
  long type_320X;
  long x_319X;
  long bytes_318X;
  long x_317X;
  long key_316X;
  long x_315X;
  long arg2_314X;
  char v_313X;
  char v_312X;
  char v_311X;
  char v_310X;
  long x_309X;
  char * addr_308X;
  long next_307X;
  long channel_306X;
  long n_305X;
  long head_304X;
  long channel_303X;
  long obj_302X;
  char v_301X;
  char v_300X;
  char v_299X;
  char v_298X;
  char v_297X;
  char v_296X;
  char v_295X;
  char v_294X;
  long x_293X;
  long x_292X;
  long x_291X;
  long arg2_290X;
  long descriptor_289X;
  long x_288X;
  long obj_287X;
  long len_286X;
  char Kchar_285X;
  long index_284X;
  long arg3_283X;
  long arg2_282X;
  long len_281X;
  long index_280X;
  long arg2_279X;
  long obj_278X;
  char v_277X;
  char v_276X;
  long size_275X;
  char init_274X;
  long len_273X;
  long arg2_272X;
  long len_271X;
  long Kchar_270X;
  long index_269X;
  long arg3_268X;
  long arg2_267X;
  long len_266X;
  long index_265X;
  long arg2_264X;
  long obj_263X;
  char v_262X;
  char v_261X;
  long size_260X;
  long init_259X;
  long len_258X;
  long arg2_257X;
  char * addr_256X;
  long index_255X;
  long len_254X;
  long type_253X;
  long x_252X;
  long arg3_251X;
  long arg2_250X;
  long index_249X;
  long len_248X;
  long type_247X;
  long x_246X;
  long arg2_245X;
  char v_244X;
  long temp_243X;
  char v_242X;
  long size_241X;
  long len_240X;
  long type_239X;
  long x_238X;
  long arg2_237X;
  char * addr_236X;
  long type_235X;
  long offset_234X;
  long x_233X;
  long arg2_232X;
  long type_231X;
  long offset_230X;
  long x_229X;
  char v_228X;
  char v_227X;
  long space_226X;
  long len_225X;
  char v_224X;
  char v_223X;
  long space_222X;
  long len_221X;
  long type_220X;
  long x_219X;
  long type_218X;
  long x_217X;
  long x_216X;
  long x_215X;
  long x_214X;
  long arg2_213X;
  long x_212X;
  long arg2_211X;
  long x_210X;
  long x_209X;
  long result_208X;
  long x_207X;
  long x_206X;
  long count_205X;
  long value_204X;
  long x_203X;
  long arg2_202X;
  long arg2_201X;
  long arg2_200X;
  long arg2_199X;
  long x_198X;
  long arg2_197X;
  long x_196X;
  long arg2_195X;
  long x_194X;
  long arg2_193X;
  long x_192X;
  long x_191X;
  long x_190X;
  long x_189X;
  long x_188X;
  long x_187X;
  long x_186X;
  long x_185X;
  long x_184X;
  long x_183X;
  long a_182X;
  long x_181X;
  long arg2_180X;
  long b_179X;
  long a_178X;
  long x_177X;
  long arg2_176X;
  long x_175X;
  long arg2_174X;
  long x_173X;
  long arg2_172X;
  long x_171X;
  long arg2_170X;
  long x_169X;
  long arg2_168X;
  long x_167X;
  long arg2_166X;
  long b_165X;
  long a_164X;
  long y_163X;
  long arg2_162X;
  long z_161X;
  long x_160X;
  long arg2_159X;
  long b_158X;
  long a_157X;
  long y_156X;
  long arg2_155X;
  long z_154X;
  long x_153X;
  long arg2_152X;
  long x_151X;
  long x_150X;
  long x_149X;
  long x_148X;
  long x_147X;
  long x_146X;
  long x_145X;
  long x_144X;
  long arg2_143X;
  long rest_list_142X;
  long x_141X;
  long rest_list_140X;
  long stack_nargs_139X;
  long arg0_138X;
  long arg1_137X;
  long rest_list_136X;
  long arg0_135X;
  long stack_nargs_134X;
  long index_133X;
  long val_132X;
  long max_131X;
  long p_130X;
  char v_129X;
  char v_128X;
  long space_127X;
  long space_126X;
  long index_125X;
  long v_124X;
  char v_123X;
  char v_122X;
  long space_121X;
  long code_120X;
  long tem_119X;
  long stack_nargs_118X;
  long p_117X;
  long obj_116X;
  long consumer_115X;
  long cont_114X;
  long rest_list_113X;
  long proc_112X;
  long stack_nargs_111X;
  long args_110X;
  long length_109X;
  char okayP_108X;
  long list_args_107X;
  long stob_106X;
  char * top_of_args_105X;
  long nargs_104X;
  long obj_103X;
  long stack_arg_count_102X;
  char v_101X;
  char v_100X;
  long space_99X;
  long code_98X;
  long template_97X;
  long obj_96X;
  long stack_arg_count_95X;
  long cont_94X;
  char v_93X;
  char v_92X;
  long space_91X;
  char v_90X;
  char v_89X;
  long space_88X;
  long count_87X;
  long p_86X;
  char * addr_85X;
  long val_84X;
  long location_83X;
  long location_82X;
  long env_81X;
  long back_80X;
  long env_79X;
  long back_78X;
  long env_77X;
  long back_76X;
  long count_75X;
  char * code_pointer_74X;
 {  SvalS = value_39X;
  goto L15449;}
 L15449: {
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L14545: {
  code_pointer_74X = arg1K0;
  switch ((*((unsigned char *) code_pointer_74X))) {
    case 0 : {
      merged_arg0K0 = 15;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 0;
      goto push_exception_continuationB;
     push_exception_continuationB_return_0:
      arg0K0 = 0;
      goto L12443;}
      break;
    case 1 : {
      count_75X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      *((long *) (SstackS)) = (SenvS);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (1034 + (((count_75X)<<10)));
      SstackS = ((SstackS) + -4);
      SenvS = (3 + (((long) ((SstackS) + 8))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 2 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 3 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 4 : {
      back_76X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_77X = SenvS;
      arg0K0 = env_77X;
      arg0K1 = back_76X;
      goto L19926;}
      break;
    case 5 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        merged_arg0K0 = 0;
        merged_arg0K1 = 2;
        push_exception_continuationB_return_tag = 1;
        goto push_exception_continuationB;
       push_exception_continuationB_return_1:
        arg0K0 = 0;
        goto L12443;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
      break;
    case 6 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        merged_arg0K0 = 0;
        merged_arg0K1 = 2;
        push_exception_continuationB_return_tag = 2;
        goto push_exception_continuationB;
       push_exception_continuationB_return_2:
        arg0K0 = 0;
        goto L12443;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
      break;
    case 7 : {
      SvalS = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SenvS)))))))))))))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        merged_arg0K0 = 0;
        merged_arg0K1 = 2;
        push_exception_continuationB_return_tag = 3;
        goto push_exception_continuationB;
       push_exception_continuationB_return_3:
        arg0K0 = 0;
        goto L12443;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
      break;
    case 8 : {
      back_78X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_79X = SenvS;
      arg0K0 = env_79X;
      arg0K1 = back_78X;
      goto L16311;}
      break;
    case 9 : {
      back_80X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_81X = SenvS;
      arg0K0 = env_81X;
      arg0K1 = back_80X;
      goto L16249;}
      break;
    case 10 : {
      location_82X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_82X))) + 4)));
      if ((17 == (255 & (SvalS)))) {
        merged_arg0K0 = 1;
        merged_arg0K1 = 3;
        push_exception_continuationB_return_tag = 4;
        goto push_exception_continuationB;
       push_exception_continuationB_return_4:
        *((long *) (SstackS)) = location_82X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
      break;
    case 11 : {
      location_83X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      if ((273 == (*((long *) ((((char *) (-3 + location_83X))) + 4))))) {
        merged_arg0K0 = 1;
        merged_arg0K1 = 3;
        push_exception_continuationB_return_tag = 5;
        goto push_exception_continuationB;
       push_exception_continuationB_return_5:
        *((long *) (SstackS)) = location_83X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}
      else {
        val_84X = SvalS;
        addr_85X = (((char *) (-3 + location_83X))) + 4;
        WRITE_BARRIER(addr_85X, val_84X);
        *((long *) addr_85X) = val_84X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
      break;
    case 12 : {
      p_86X = SenvS;
      if ((3 == (3 & p_86X))) {
        if ((p_86X < (((long) (Sstack_beginS))))) {
          arg0K0 = 0;
          goto L17042;}
        else {
          if (((((long) (Sstack_endS))) < p_86X)) {
            arg0K0 = 0;
            goto L17042;}
          else {
            arg0K0 = ((Sstack_endS) - (Sstack_beginS));
            goto L17042;}}}
      else {
        arg0K0 = 0;
        goto L17042;}}
      break;
    case 13 : {
      count_87X = *((unsigned char *) ((Scode_pointerS) + 1));
      space_88X = 4 + count_87X;
      v_89X = AVAILABLEp(space_88X);
      if (v_89X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L16959;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 0;
        goto collect_saving_temp;
       collect_saving_temp_return_0:
        v_90X = AVAILABLEp(space_88X);
        if (v_90X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L16959;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L16959;}}}
      break;
    case 14 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 15 : {
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      if ((529 == (SvalS))) {
        merged_arg0K0 = 0;
        merged_arg0K1 = 3;
        push_exception_continuationB_return_tag = 6;
        goto push_exception_continuationB;
       push_exception_continuationB_return_6:
        arg0K0 = 0;
        goto L12443;}
      else {
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
      break;
    case 16 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = (*((long *) ((((char *) (-3 + (SenvS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
      if ((529 == (SvalS))) {
        merged_arg0K0 = 0;
        merged_arg0K1 = 3;
        push_exception_continuationB_return_tag = 7;
        goto push_exception_continuationB;
       push_exception_continuationB_return_7:
        arg0K0 = 0;
        goto L12443;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
      break;
    case 17 : {
      SstackS = ((SstackS) + 4);
      SvalS = (*((long *) (SstackS)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 18 : {
      SvalS = (*((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 19 : {
      *((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 20 : {
      merged_arg1K0 = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      merged_arg0K1 = (*((unsigned char *) ((Scode_pointerS) + 3)));
      push_continuationB_return_tag = 0;
      goto push_continuationB;
     push_continuationB_return_0:
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 21 : {
      merged_arg1K0 = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      merged_arg0K1 = (((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4))));
      push_continuationB_return_tag = 1;
      goto push_continuationB;
     push_continuationB_return_1:
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 22 : {
      space_91X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
      v_92X = AVAILABLEp(space_91X);
      if (v_92X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L16837;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 1;
        goto collect_saving_temp;
       collect_saving_temp_return_1:
        v_93X = AVAILABLEp(space_91X);
        if (v_93X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L16837;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L16837;}}}
      break;
    case 23 : {
      cont_94X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
      if ((3 == (3 & cont_94X))) {
        if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_94X))) + -4))))>>2))))) {
          if ((3 == (3 & cont_94X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_94X))) + -4))))>>2))))) {
              merged_arg0K0 = cont_94X;
              copy_continuation_from_heapB_return_tag = 0;
              goto copy_continuation_from_heapB;
             copy_continuation_from_heapB_return_0:
              goto L15909;}
            else {
              goto L15946;}}
          else {
            goto L15946;}}
        else {
          goto L15910;}}
      else {
        goto L15910;}}
      break;
    case 24 : {
      stack_arg_count_95X = *((unsigned char *) ((Scode_pointerS) + 1));
      obj_96X = SvalS;
      if ((3 == (3 & obj_96X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_96X))) + -4))))>>2))))) {
          template_97X = *((long *) (((char *) (-3 + (SvalS)))));
          code_98X = *((long *) (((char *) (-3 + template_97X))));
          if ((stack_arg_count_95X == (*((unsigned char *) ((((char *) (-3 + code_98X))) + 1))))) {
            StemplateS = template_97X;
            SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
            Scode_pointerS = ((((char *) (-3 + code_98X))) + 2);
            if (((SstackS) < (Sstack_limitS))) {
              space_99X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
              v_100X = AVAILABLEp(space_99X);
              if (v_100X) {
                arg2K0 = 1;
                arg0K1 = 0;
                goto L15313;}
              else {
                merged_arg0K0 = 0;
                collect_saving_temp_return_tag = 2;
                goto collect_saving_temp;
               collect_saving_temp_return_2:
                v_101X = AVAILABLEp(space_99X);
                if (v_101X) {
                  arg2K0 = 1;
                  arg0K1 = 0;
                  goto L15313;}
                else {
                  arg2K0 = 0;
                  arg0K1 = 0;
                  goto L15313;}}}
            else {
              goto L15225;}}
          else {
            arg0K0 = stack_arg_count_95X;
            goto L11844;}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_95X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L12939;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_95X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L12939;}}
      break;
    case 25 : {
      stack_arg_count_102X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      obj_103X = SvalS;
      if ((3 == (3 & obj_103X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_103X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_102X;
          goto L11844;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_102X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L12939;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_102X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L12939;}}
      break;
    case 26 : {
      nargs_104X = *((unsigned char *) ((Scode_pointerS) + 1));
      top_of_args_105X = SstackS;
      stob_106X = ScontS;
      arg1K0 = ((((char *) (-3 + stob_106X))) + -8);
      arg1K1 = (top_of_args_105X + (((nargs_104X)<<2)));
      goto L14309;}
      break;
    case 27 : {
      SstackS = ((SstackS) + 4);
      list_args_107X = *((long *) (SstackS));
      merged_arg0K0 = list_args_107X;
      okay_argument_list_return_tag = 0;
      goto okay_argument_list;
     okay_argument_list_return_0:
      okayP_108X = okay_argument_list0_return_value;
      length_109X = okay_argument_list1_return_value;
      if (okayP_108X) {
        arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
        arg0K1 = list_args_107X;
        arg0K2 = length_109X;
        goto L14347;}
      else {
        *((long *) (SstackS)) = list_args_107X;
        SstackS = ((SstackS) + -4);
        merged_arg0K0 = 25;
        merged_arg0K1 = (1 + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        pop_args_GlistS_return_tag = 0;
        goto pop_args_GlistS;
       pop_args_GlistS_return_0:
        args_110X = pop_args_GlistS0_return_value;
        merged_arg0K0 = 5;
        merged_arg0K1 = 0;
        push_exception_continuationB_return_tag = 8;
        goto push_exception_continuationB;
       push_exception_continuationB_return_8:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = args_110X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 28 : {
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      stack_nargs_111X = (((*((long *) (SstackS))))>>2);
      proc_112X = *((long *) ((SstackS) + (4 + (((stack_nargs_111X)<<2)))));
      *((long *) ((SstackS) + (4 + (((stack_nargs_111X)<<2))))) = 1;
      SstackS = ((SstackS) + 4);
      rest_list_113X = *((long *) (SstackS));
      if ((25 == rest_list_113X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = (-2 + stack_nargs_111X);
        goto L8950;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_113X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_113X)))));
          arg0K1 = (-1 + stack_nargs_111X);
          goto L8950;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_113X))) + 4)))))) + 4)));
          arg0K1 = rest_list_113X;
          goto L8991;}}}
      break;
    case 29 : {
      SstackS = ((SstackS) + 4);
      cont_114X = *((long *) (SstackS));
      if ((3 == (3 & cont_114X))) {
        if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_114X))) + -4))))>>2))))) {
          merged_arg0K0 = cont_114X;
          copy_continuation_from_heapB_return_tag = 1;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_1:
          goto L14252;}
        else {
          goto L14259;}}
      else {
        goto L14259;}}
      break;
    case 30 : {
      SstackS = ((SstackS) + 4);
      consumer_115X = *((long *) (SstackS));
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = consumer_115X;
      obj_116X = SvalS;
      if ((3 == (3 & obj_116X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_116X))) + -4))))>>2))))) {
          arg0K0 = 1;
          goto L11844;}
        else {
          arg0K0 = 3;
          arg0K1 = 1;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L12939;}}
      else {
        arg0K0 = 3;
        arg0K1 = 1;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L12939;}}
      break;
    case 31 : {
      pop_continuationB_return_tag = 0;
      goto pop_continuationB;
     pop_continuationB_return_0:
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 32 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L16479;}
      break;
    case 33 : {
      SstackS = ((SstackS) + 4);
      p_117X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_118X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      arg0K0 = stack_nargs_118X;
      arg0K1 = (*((long *) (SstackS)));
      arg0K2 = ((((p_117X)>>2)) - stack_nargs_118X);
      goto L16479;}
      break;
    case 34 : {
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 35 : {
      tem_119X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      StemplateS = tem_119X;
      Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_119X))))))));
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 36 : {
      code_120X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))))))));
      if ((0 == (*((unsigned char *) ((((char *) (-3 + code_120X))) + 1))))) {
        if (((SstackS) < (Sstack_limitS))) {
          space_121X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
          v_122X = AVAILABLEp(space_121X);
          if (v_122X) {
            arg2K0 = 1;
            arg0K1 = 0;
            goto L15051;}
          else {
            merged_arg0K0 = 0;
            collect_saving_temp_return_tag = 3;
            goto collect_saving_temp;
           collect_saving_temp_return_3:
            v_123X = AVAILABLEp(space_121X);
            if (v_123X) {
              arg2K0 = 1;
              arg0K1 = 0;
              goto L15051;}
            else {
              arg2K0 = 0;
              arg0K1 = 0;
              goto L15051;}}}
        else {
          goto L14924;}}
      else {
        v_124X = *((unsigned char *) ((((char *) (-3 + code_120X))) + 1));
        if ((66 == v_124X)) {
          if ((0 == (*((unsigned char *) ((((char *) (-3 + code_120X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_120X))) + -4))))>>8)))))))) {
            index_125X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_120X))) + -4))))>>8));
            space_126X = ((((*((unsigned char *) ((((char *) (-3 + code_120X))) + index_125X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_120X))) + (1 + index_125X))));
            if ((space_126X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
              goto L14924;}
            else {
              space_127X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
              v_128X = AVAILABLEp(space_127X);
              if (v_128X) {
                arg2K0 = 1;
                arg0K1 = 0;
                goto L15106;}
              else {
                merged_arg0K0 = 0;
                collect_saving_temp_return_tag = 4;
                goto collect_saving_temp;
               collect_saving_temp_return_4:
                v_129X = AVAILABLEp(space_127X);
                if (v_129X) {
                  arg2K0 = 1;
                  arg0K1 = 0;
                  goto L15106;}
                else {
                  arg2K0 = 0;
                  arg0K1 = 0;
                  goto L15106;}}}}
          else {
            goto L14969;}}
        else {
          goto L14969;}}}
      break;
    case 37 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
      break;
    case 38 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 39 : {
      if ((0 == (3 & (SvalS)))) {
        p_130X = SvalS;
        max_131X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_132X = ((p_130X)>>2);
        if ((val_132X < 0)) {
          goto L14825;}
        else {
          if ((val_132X < max_131X)) {
            index_133X = 1 + (((val_132X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_133X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_133X)))));
            goto L14827;}
          else {
            goto L14825;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 0;
        push_exception_continuationB_return_tag = 9;
        goto push_exception_continuationB;
       push_exception_continuationB_return_9:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 40 : {
      stack_nargs_134X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((0 == stack_nargs_134X)) {
        arg0_135X = *((long *) ((SstackS) + 12));
        rest_list_136X = *((long *) ((SstackS) + 8));
        *((long *) (SstackS)) = arg0_135X;
        SstackS = ((SstackS) + -4);
        SvalS = (*((long *) (((char *) (-3 + rest_list_136X)))));
        goto L15812;}
      else {
        arg1_137X = *((long *) ((SstackS) + (4 + (((stack_nargs_134X)<<2)))));
        arg0_138X = *((long *) ((SstackS) + (8 + (((stack_nargs_134X)<<2)))));
        *((long *) ((SstackS) + (8 + (((stack_nargs_134X)<<2))))) = 1;
        *((long *) ((SstackS) + 4)) = (-4 + (((stack_nargs_134X)<<2)));
        *((long *) (SstackS)) = arg0_138X;
        SstackS = ((SstackS) + -4);
        SvalS = arg1_137X;
        goto L15812;}}
      break;
    case 41 : {
      stack_nargs_139X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((stack_nargs_139X == 0)) {
        rest_list_140X = *((long *) ((SstackS) + 8));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_140X))) + 4))))) {
          arg0K0 = 1;
          goto L14723;}
        else {
          *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_140X))) + 4)));
          *((long *) ((SstackS) + 12)) = (SvalS);
          arg0K0 = -2;
          goto L14723;}}
      else {
        if ((stack_nargs_139X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 8))))) {
            arg0K0 = 1;
            goto L14723;}
          else {
            *((long *) ((SstackS) + 4)) = 0;
            *((long *) ((SstackS) + 12)) = (SvalS);
            arg0K0 = -2;
            goto L14723;}}
        else {
          *((long *) ((SstackS) + (8 + (((stack_nargs_139X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L14723;}}}
      break;
    case 42 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        x_141X = (((*((long *) ((SstackS) + 4))))>>2);
        if ((x_141X == 0)) {
          rest_list_142X = *((long *) ((SstackS) + 8));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_142X))) + 4))))) {
            arg0K0 = 1;
            goto L15703;}
          else {
            *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_142X))) + 4)));
            *((long *) ((SstackS) + 12)) = (*((long *) (((char *) (-3 + rest_list_142X)))));
            arg0K0 = -2;
            goto L15703;}}
        else {
          if ((x_141X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 8))))) {
              arg0K0 = 1;
              goto L15703;}
            else {
              *((long *) ((SstackS) + 4)) = 0;
              arg0K0 = -2;
              goto L15703;}}
          else {
            arg0K0 = -2;
            goto L15703;}}}}
      break;
    case 43 : {
      SstackS = ((SstackS) + 4);
      arg2_143X = *((long *) (SstackS));
      x_144X = SvalS;
      if ((arg2_143X == x_144X)) {
        arg0K0 = 5;
        goto L32849;}
      else {
        arg0K0 = 1;
        goto L32849;}}
      break;
    case 44 : {
      x_145X = SvalS;
      if ((0 == (3 & x_145X))) {
        arg0K0 = 5;
        goto L24899;}
      else {
        if ((3 == (3 & x_145X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_145X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L24899;}
          else {
            goto L24854;}}
        else {
          goto L24854;}}}
      break;
    case 45 : {
      x_146X = SvalS;
      if ((0 == (3 & x_146X))) {
        goto L23276;}
      else {
        if ((3 == (3 & x_146X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_146X))) + -4))))>>2))))) {
            goto L23276;}
          else {
            goto L23279;}}
        else {
          goto L23279;}}}
      break;
    case 46 : {
      x_147X = SvalS;
      if ((0 == (3 & x_147X))) {
        goto L23085;}
      else {
        if ((3 == (3 & x_147X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_147X))) + -4))))>>2))))) {
            goto L23085;}
          else {
            goto L23068;}}
        else {
          goto L23068;}}}
      break;
    case 47 : {
      x_148X = SvalS;
      if ((0 == (3 & x_148X))) {
        goto L22874;}
      else {
        if ((3 == (3 & x_148X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_148X))) + -4))))>>2))))) {
            goto L22874;}
          else {
            goto L22857;}}
        else {
          goto L22857;}}}
      break;
    case 48 : {
      x_149X = SvalS;
      if ((0 == (3 & x_149X))) {
        goto L22663;}
      else {
        if ((3 == (3 & x_149X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + x_149X))) + -4))))>>2))))) {
            goto L22663;}
          else {
            goto L22646;}}
        else {
          goto L22646;}}}
      break;
    case 49 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = 5;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 10;
        goto push_exception_continuationB;
       push_exception_continuationB_return_10:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 50 : {
      if ((0 == (3 & (SvalS)))) {
        x_150X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 11;
        goto push_exception_continuationB;
       push_exception_continuationB_return_11:
        *((long *) (SstackS)) = x_150X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 12;
        goto push_exception_continuationB;
       push_exception_continuationB_return_12:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 51 : {
      if ((0 == (3 & (SvalS)))) {
        x_151X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 13;
        goto push_exception_continuationB;
       push_exception_continuationB_return_13:
        *((long *) (SstackS)) = x_151X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 14;
        goto push_exception_continuationB;
       push_exception_continuationB_return_14:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 52 : {
      SstackS = ((SstackS) + 4);
      arg2_152X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_152X | (SvalS))))) {
        x_153X = SvalS;
        z_154X = (((arg2_152X)>>2)) + (((x_153X)>>2));
        if ((536870911 < z_154X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 15;
          goto push_exception_continuationB;
         push_exception_continuationB_return_15:
          *((long *) (SstackS)) = arg2_152X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_153X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L12443;}
        else {
          if ((z_154X < -536870912)) {
            merged_arg0K0 = 6;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 16;
            goto push_exception_continuationB;
           push_exception_continuationB_return_16:
            *((long *) (SstackS)) = arg2_152X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_153X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L12443;}
          else {
            SvalS = (((z_154X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L14545;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 17;
        goto push_exception_continuationB;
       push_exception_continuationB_return_17:
        *((long *) (SstackS)) = arg2_152X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 53 : {
      SstackS = ((SstackS) + 4);
      arg2_155X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_155X | (SvalS))))) {
        y_156X = SvalS;
        a_157X = ((arg2_155X)>>2);
        b_158X = ((y_156X)>>2);
        if ((a_157X < 0)) {
          arg0K0 = (0 - a_157X);
          goto L5777;}
        else {
          arg0K0 = a_157X;
          goto L5777;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 18;
        goto push_exception_continuationB;
       push_exception_continuationB_return_18:
        *((long *) (SstackS)) = arg2_155X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 54 : {
      SstackS = ((SstackS) + 4);
      arg2_159X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_159X | (SvalS))))) {
        x_160X = SvalS;
        z_161X = (((arg2_159X)>>2)) - (((x_160X)>>2));
        if ((536870911 < z_161X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 19;
          goto push_exception_continuationB;
         push_exception_continuationB_return_19:
          *((long *) (SstackS)) = arg2_159X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_160X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L12443;}
        else {
          if ((z_161X < -536870912)) {
            merged_arg0K0 = 6;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 20;
            goto push_exception_continuationB;
           push_exception_continuationB_return_20:
            *((long *) (SstackS)) = arg2_159X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_160X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L12443;}
          else {
            SvalS = (((z_161X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L14545;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 21;
        goto push_exception_continuationB;
       push_exception_continuationB_return_21:
        *((long *) (SstackS)) = arg2_159X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 55 : {
      SstackS = ((SstackS) + 4);
      arg2_162X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_162X | (SvalS))))) {
        y_163X = SvalS;
        if ((0 == y_163X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 22;
          goto push_exception_continuationB;
         push_exception_continuationB_return_22:
          *((long *) (SstackS)) = arg2_162X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = y_163X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L12443;}
        else {
          a_164X = ((arg2_162X)>>2);
          b_165X = ((y_163X)>>2);
          if ((a_164X < 0)) {
            arg0K0 = (0 - a_164X);
            goto L7215;}
          else {
            arg0K0 = a_164X;
            goto L7215;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 23;
        goto push_exception_continuationB;
       push_exception_continuationB_return_23:
        *((long *) (SstackS)) = arg2_162X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 56 : {
      SstackS = ((SstackS) + 4);
      arg2_166X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_166X | (SvalS))))) {
        x_167X = SvalS;
        if ((arg2_166X == x_167X)) {
          arg0K0 = 5;
          goto L24750;}
        else {
          arg0K0 = 1;
          goto L24750;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 24;
        goto push_exception_continuationB;
       push_exception_continuationB_return_24:
        *((long *) (SstackS)) = arg2_166X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 57 : {
      SstackS = ((SstackS) + 4);
      arg2_168X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_168X | (SvalS))))) {
        x_169X = SvalS;
        if ((arg2_168X < x_169X)) {
          arg0K0 = 5;
          goto L24660;}
        else {
          arg0K0 = 1;
          goto L24660;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 25;
        goto push_exception_continuationB;
       push_exception_continuationB_return_25:
        *((long *) (SstackS)) = arg2_168X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 58 : {
      SstackS = ((SstackS) + 4);
      arg2_170X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_170X | (SvalS))))) {
        x_171X = SvalS;
        if ((x_171X < arg2_170X)) {
          arg0K0 = 5;
          goto L24570;}
        else {
          arg0K0 = 1;
          goto L24570;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 26;
        goto push_exception_continuationB;
       push_exception_continuationB_return_26:
        *((long *) (SstackS)) = arg2_170X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 59 : {
      SstackS = ((SstackS) + 4);
      arg2_172X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_172X | (SvalS))))) {
        x_173X = SvalS;
        if ((x_173X < arg2_172X)) {
          arg0K0 = 1;
          goto L24465;}
        else {
          arg0K0 = 5;
          goto L24465;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 27;
        goto push_exception_continuationB;
       push_exception_continuationB_return_27:
        *((long *) (SstackS)) = arg2_172X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 60 : {
      SstackS = ((SstackS) + 4);
      arg2_174X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_174X | (SvalS))))) {
        x_175X = SvalS;
        if ((arg2_174X < x_175X)) {
          arg0K0 = 1;
          goto L24358;}
        else {
          arg0K0 = 5;
          goto L24358;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 28;
        goto push_exception_continuationB;
       push_exception_continuationB_return_28:
        *((long *) (SstackS)) = arg2_174X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 61 : {
      SstackS = ((SstackS) + 4);
      arg2_176X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_176X | (SvalS))))) {
        x_177X = SvalS;
        if ((0 == x_177X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 29;
          goto push_exception_continuationB;
         push_exception_continuationB_return_29:
          *((long *) (SstackS)) = arg2_176X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_177X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L12443;}
        else {
          a_178X = ((arg2_176X)>>2);
          b_179X = ((x_177X)>>2);
          if ((a_178X < 0)) {
            arg0K0 = (0 - a_178X);
            goto L28479;}
          else {
            arg0K0 = a_178X;
            goto L28479;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 30;
        goto push_exception_continuationB;
       push_exception_continuationB_return_30:
        *((long *) (SstackS)) = arg2_176X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 62 : {
      SstackS = ((SstackS) + 4);
      arg2_180X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_180X | (SvalS))))) {
        x_181X = SvalS;
        if ((0 == x_181X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 31;
          goto push_exception_continuationB;
         push_exception_continuationB_return_31:
          *((long *) (SstackS)) = arg2_180X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_181X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L12443;}
        else {
          a_182X = ((arg2_180X)>>2);
          if ((a_182X < 0)) {
            arg0K0 = (0 - a_182X);
            goto L28347;}
          else {
            arg0K0 = a_182X;
            goto L28347;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 32;
        goto push_exception_continuationB;
       push_exception_continuationB_return_32:
        *((long *) (SstackS)) = arg2_180X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 63 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = (SvalS);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 33;
        goto push_exception_continuationB;
       push_exception_continuationB_return_33:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 64 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = (SvalS);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 34;
        goto push_exception_continuationB;
       push_exception_continuationB_return_34:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 65 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = 4;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 35;
        goto push_exception_continuationB;
       push_exception_continuationB_return_35:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 66 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = (SvalS);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 36;
        goto push_exception_continuationB;
       push_exception_continuationB_return_36:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 67 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = 0;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 37;
        goto push_exception_continuationB;
       push_exception_continuationB_return_37:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 68 : {
      if ((0 == (3 & (SvalS)))) {
        x_183X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 38;
        goto push_exception_continuationB;
       push_exception_continuationB_return_38:
        *((long *) (SstackS)) = x_183X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 39;
        goto push_exception_continuationB;
       push_exception_continuationB_return_39:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 69 : {
      if ((0 == (3 & (SvalS)))) {
        x_184X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 40;
        goto push_exception_continuationB;
       push_exception_continuationB_return_40:
        *((long *) (SstackS)) = x_184X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 41;
        goto push_exception_continuationB;
       push_exception_continuationB_return_41:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 70 : {
      if ((0 == (3 & (SvalS)))) {
        x_185X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 42;
        goto push_exception_continuationB;
       push_exception_continuationB_return_42:
        *((long *) (SstackS)) = x_185X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 43;
        goto push_exception_continuationB;
       push_exception_continuationB_return_43:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 71 : {
      if ((0 == (3 & (SvalS)))) {
        x_186X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 44;
        goto push_exception_continuationB;
       push_exception_continuationB_return_44:
        *((long *) (SstackS)) = x_186X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 45;
        goto push_exception_continuationB;
       push_exception_continuationB_return_45:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 72 : {
      if ((0 == (3 & (SvalS)))) {
        x_187X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 46;
        goto push_exception_continuationB;
       push_exception_continuationB_return_46:
        *((long *) (SstackS)) = x_187X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 47;
        goto push_exception_continuationB;
       push_exception_continuationB_return_47:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 73 : {
      if ((0 == (3 & (SvalS)))) {
        x_188X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 48;
        goto push_exception_continuationB;
       push_exception_continuationB_return_48:
        *((long *) (SstackS)) = x_188X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 49;
        goto push_exception_continuationB;
       push_exception_continuationB_return_49:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 74 : {
      if ((0 == (3 & (SvalS)))) {
        x_189X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 50;
        goto push_exception_continuationB;
       push_exception_continuationB_return_50:
        *((long *) (SstackS)) = x_189X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 51;
        goto push_exception_continuationB;
       push_exception_continuationB_return_51:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 75 : {
      if ((0 == (3 & (SvalS)))) {
        x_190X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 52;
        goto push_exception_continuationB;
       push_exception_continuationB_return_52:
        *((long *) (SstackS)) = x_190X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 53;
        goto push_exception_continuationB;
       push_exception_continuationB_return_53:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 76 : {
      if ((0 == (3 & (SvalS)))) {
        x_191X = SvalS;
        if ((x_191X < 0)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 54;
          goto push_exception_continuationB;
         push_exception_continuationB_return_54:
          *((long *) (SstackS)) = x_191X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 1;
          goto L12443;}
        else {
          SvalS = 0;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 55;
        goto push_exception_continuationB;
       push_exception_continuationB_return_55:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 77 : {
      if ((0 == (3 & (SvalS)))) {
        x_192X = SvalS;
        if ((x_192X < 0)) {
          arg0K0 = (0 - x_192X);
          goto L22424;}
        else {
          arg0K0 = x_192X;
          goto L22424;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 56;
        goto push_exception_continuationB;
       push_exception_continuationB_return_56:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 78 : {
      SstackS = ((SstackS) + 4);
      arg2_193X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_193X | (SvalS))))) {
        x_194X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 57;
        goto push_exception_continuationB;
       push_exception_continuationB_return_57:
        *((long *) (SstackS)) = arg2_193X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_194X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 58;
        goto push_exception_continuationB;
       push_exception_continuationB_return_58:
        *((long *) (SstackS)) = arg2_193X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 79 : {
      SstackS = ((SstackS) + 4);
      arg2_195X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_195X | (SvalS))))) {
        x_196X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 59;
        goto push_exception_continuationB;
       push_exception_continuationB_return_59:
        *((long *) (SstackS)) = arg2_195X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_196X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 60;
        goto push_exception_continuationB;
       push_exception_continuationB_return_60:
        *((long *) (SstackS)) = arg2_195X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 80 : {
      SstackS = ((SstackS) + 4);
      arg2_197X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_197X | (SvalS))))) {
        x_198X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 61;
        goto push_exception_continuationB;
       push_exception_continuationB_return_61:
        *((long *) (SstackS)) = arg2_197X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_198X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 62;
        goto push_exception_continuationB;
       push_exception_continuationB_return_62:
        *((long *) (SstackS)) = arg2_197X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 81 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = ((((~ ((((SvalS))>>2))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 63;
        goto push_exception_continuationB;
       push_exception_continuationB_return_63:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 82 : {
      SstackS = ((SstackS) + 4);
      arg2_199X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_199X | (SvalS))))) {
        SvalS = (((((((arg2_199X)>>2)) & ((((SvalS))>>2))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 64;
        goto push_exception_continuationB;
       push_exception_continuationB_return_64:
        *((long *) (SstackS)) = arg2_199X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 83 : {
      SstackS = ((SstackS) + 4);
      arg2_200X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_200X | (SvalS))))) {
        SvalS = (((((((arg2_200X)>>2)) | ((((SvalS))>>2))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 65;
        goto push_exception_continuationB;
       push_exception_continuationB_return_65:
        *((long *) (SstackS)) = arg2_200X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 84 : {
      SstackS = ((SstackS) + 4);
      arg2_201X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_201X | (SvalS))))) {
        SvalS = (((((((arg2_201X)>>2)) ^ ((((SvalS))>>2))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 66;
        goto push_exception_continuationB;
       push_exception_continuationB_return_66:
        *((long *) (SstackS)) = arg2_201X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 85 : {
      SstackS = ((SstackS) + 4);
      arg2_202X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_202X | (SvalS))))) {
        x_203X = SvalS;
        value_204X = ((arg2_202X)>>2);
        count_205X = ((x_203X)>>2);
        if ((count_205X < 0)) {
          PS_SHIFT_RIGHT(value_204X, (0 - count_205X), x_206X)
          SvalS = (((x_206X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          PS_SHIFT_LEFT(value_204X, count_205X, x_207X)
          result_208X = (((((x_207X)<<2)))>>2);
          PS_SHIFT_RIGHT(result_208X, count_205X, x_209X)
          if ((value_204X == x_209X)) {
            if ((value_204X < 0)) {
              if ((result_208X < 0)) {
                goto L28197;}
              else {
                merged_arg0K0 = 6;
                merged_arg0K1 = 1;
                push_exception_continuationB_return_tag = 67;
                goto push_exception_continuationB;
               push_exception_continuationB_return_67:
                *((long *) (SstackS)) = arg2_202X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_203X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L12443;}}
            else {
              if ((result_208X < 0)) {
                merged_arg0K0 = 6;
                merged_arg0K1 = 1;
                push_exception_continuationB_return_tag = 68;
                goto push_exception_continuationB;
               push_exception_continuationB_return_68:
                *((long *) (SstackS)) = arg2_202X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_203X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L12443;}
              else {
                goto L28197;}}}
          else {
            merged_arg0K0 = 6;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 69;
            goto push_exception_continuationB;
           push_exception_continuationB_return_69:
            *((long *) (SstackS)) = arg2_202X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_203X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L12443;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 70;
        goto push_exception_continuationB;
       push_exception_continuationB_return_70:
        *((long *) (SstackS)) = arg2_202X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 86 : {
      x_210X = SvalS;
      if ((9 == (255 & x_210X))) {
        arg0K0 = 5;
        goto L33192;}
      else {
        arg0K0 = 1;
        goto L33192;}}
      break;
    case 87 : {
      SstackS = ((SstackS) + 4);
      arg2_211X = *((long *) (SstackS));
      if ((9 == (255 & arg2_211X))) {
        if ((9 == (255 & (SvalS)))) {
          x_212X = SvalS;
          if ((arg2_211X == x_212X)) {
            arg0K0 = 5;
            goto L24230;}
          else {
            arg0K0 = 1;
            goto L24230;}}
        else {
          goto L24203;}}
      else {
        goto L24203;}}
      break;
    case 88 : {
      SstackS = ((SstackS) + 4);
      arg2_213X = *((long *) (SstackS));
      if ((9 == (255 & arg2_213X))) {
        if ((9 == (255 & (SvalS)))) {
          x_214X = SvalS;
          if ((arg2_213X < x_214X)) {
            arg0K0 = 5;
            goto L24140;}
          else {
            arg0K0 = 1;
            goto L24140;}}
        else {
          goto L24113;}}
      else {
        goto L24113;}}
      break;
    case 89 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 71;
        goto push_exception_continuationB;
       push_exception_continuationB_return_71:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 90 : {
      if ((0 == (3 & (SvalS)))) {
        x_215X = (((SvalS))>>2);
        if ((255 < x_215X)) {
          goto L22324;}
        else {
          if ((x_215X < 0)) {
            goto L22324;}
          else {
            SvalS = (9 + (((((x_215X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L14545;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 72;
        goto push_exception_continuationB;
       push_exception_continuationB_return_72:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 91 : {
      x_216X = SvalS;
      if ((21 == x_216X)) {
        arg0K0 = 5;
        goto L33221;}
      else {
        arg0K0 = 1;
        goto L33221;}}
      break;
    case 92 : {
      x_217X = SvalS;
      type_218X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_217X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_217X))) + -4))))>>2))) == type_218X)) {
          arg0K0 = 5;
          goto L33235;}
        else {
          arg0K0 = 1;
          goto L33235;}}
      else {
        arg0K0 = 1;
        goto L33235;}}
      break;
    case 93 : {
      x_219X = SvalS;
      type_220X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_219X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_219X))) + -4))))>>2))) == type_220X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_219X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          goto L19673;}}
      else {
        goto L19673;}}
      break;
    case 94 : {
      len_221X = *((unsigned char *) ((Scode_pointerS) + 1));
      space_222X = 4 + (((len_221X)<<2));
      v_223X = AVAILABLEp(space_222X);
      if (v_223X) {
        goto L19479;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 5;
        goto collect_saving_temp;
       collect_saving_temp_return_5:
        v_224X = AVAILABLEp(space_222X);
        if (v_224X) {
          goto L19479;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L19479;}}}
      break;
    case 95 : {
      SstackS = ((SstackS) + 4);
      len_225X = (((*((long *) (SstackS))))>>2);
      space_226X = 4 + (((len_225X)<<2));
      v_227X = AVAILABLEp(space_226X);
      if (v_227X) {
        goto L19270;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 6;
        goto collect_saving_temp;
       collect_saving_temp_return_6:
        v_228X = AVAILABLEp(space_226X);
        if (v_228X) {
          goto L19270;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L19270;}}}
      break;
    case 96 : {
      x_229X = SvalS;
      offset_230X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_231X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_229X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_229X))) + -4))))>>2))) == type_231X)) {
          SvalS = (*((long *) ((((char *) (-3 + x_229X))) + (((offset_230X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          goto L19168;}}
      else {
        goto L19168;}}
      break;
    case 97 : {
      SstackS = ((SstackS) + 4);
      arg2_232X = *((long *) (SstackS));
      x_233X = SvalS;
      offset_234X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_235X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & arg2_232X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + arg2_232X))) + -4))))>>2))) == type_235X)) {
          if ((3 == (3 & arg2_232X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + arg2_232X))) + -4)))))) {
              addr_236X = (((char *) (-3 + arg2_232X))) + (((offset_234X)<<2));
              WRITE_BARRIER(addr_236X, x_233X);
              *((long *) addr_236X) = x_233X;
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 3);
              arg1K0 = (Scode_pointerS);
              goto L14545;}
            else {
              goto L18956;}}
          else {
            goto L18956;}}
        else {
          goto L18956;}}
      else {
        goto L18956;}}
      break;
    case 98 : {
      SstackS = ((SstackS) + 4);
      arg2_237X = *((long *) (SstackS));
      x_238X = SvalS;
      type_239X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_237X))) {
        len_240X = ((arg2_237X)>>2);
        size_241X = 1 + len_240X;
        if ((len_240X < 0)) {
          goto L18628;}
        else {
          if ((4194305 < size_241X)) {
            goto L18628;}
          else {
            v_242X = AVAILABLEp(size_241X);
            if (v_242X) {
              arg2K0 = 1;
              arg0K1 = x_238X;
              goto L18646;}
            else {
              merged_arg0K0 = x_238X;
              collect_saving_temp_return_tag = 7;
              goto collect_saving_temp;
             collect_saving_temp_return_7:
              temp_243X = collect_saving_temp0_return_value;
              v_244X = AVAILABLEp(size_241X);
              if (v_244X) {
                arg2K0 = 1;
                arg0K1 = temp_243X;
                goto L18646;}
              else {
                arg2K0 = 0;
                arg0K1 = temp_243X;
                goto L18646;}}}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 2;
        push_exception_continuationB_return_tag = 73;
        goto push_exception_continuationB;
       push_exception_continuationB_return_73:
        *((long *) (SstackS)) = (((type_239X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_237X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_238X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L12443;}}
      break;
    case 99 : {
      SstackS = ((SstackS) + 4);
      arg2_245X = *((long *) (SstackS));
      x_246X = SvalS;
      type_247X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & x_246X))) {
        if ((3 == (3 & arg2_245X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg2_245X))) + -4))))>>2))) == type_247X)) {
            len_248X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_245X))) + -4))))>>8))))>>2);
            index_249X = ((x_246X)>>2);
            if ((index_249X < 0)) {
              goto L18365;}
            else {
              if ((index_249X < len_248X)) {
                SvalS = (*((long *) ((((char *) (-3 + arg2_245X))) + (-4 & x_246X))));
                Scode_pointerS = ((Scode_pointerS) + 2);
                arg1K0 = (Scode_pointerS);
                goto L14545;}
              else {
                goto L18365;}}}
          else {
            goto L18342;}}
        else {
          goto L18342;}}
      else {
        goto L18342;}}
      break;
    case 100 : {
      SstackS = ((SstackS) + 4);
      arg2_250X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_251X = *((long *) (SstackS));
      x_252X = SvalS;
      type_253X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_250X))) {
        if ((3 == (3 & arg3_251X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg3_251X))) + -4))))>>2))) == type_253X)) {
            if ((3 == (3 & arg3_251X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_251X))) + -4)))))) {
                len_254X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_251X))) + -4))))>>8))))>>2);
                index_255X = ((arg2_250X)>>2);
                if ((index_255X < 0)) {
                  goto L18006;}
                else {
                  if ((index_255X < len_254X)) {
                    addr_256X = (((char *) (-3 + arg3_251X))) + (-4 & arg2_250X);
                    WRITE_BARRIER(addr_256X, x_252X);
                    *((long *) addr_256X) = x_252X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 2);
                    arg1K0 = (Scode_pointerS);
                    goto L14545;}
                  else {
                    goto L18006;}}}
              else {
                goto L17981;}}
            else {
              goto L17981;}}
          else {
            goto L17981;}}
        else {
          goto L17981;}}
      else {
        goto L17981;}}
      break;
    case 101 : {
      SstackS = ((SstackS) + 4);
      arg2_257X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_257X | (SvalS))))) {
        len_258X = ((arg2_257X)>>2);
        init_259X = (((SvalS))>>2);
        size_260X = 1 + ((((3 + len_258X))>>2));
        if ((len_258X < 0)) {
          goto L22089;}
        else {
          if ((4194305 < size_260X)) {
            goto L22089;}
          else {
            v_261X = AVAILABLEp(size_260X);
            if (v_261X) {
              goto L22188;}
            else {
              merged_arg0K0 = 0;
              collect_saving_temp_return_tag = 8;
              goto collect_saving_temp;
             collect_saving_temp_return_8:
              v_262X = AVAILABLEp(size_260X);
              if (v_262X) {
                goto L22188;}
              else {
                merged_arg0K0 = 8;
                merged_arg0K1 = 1;
                push_exception_continuationB_return_tag = 74;
                goto push_exception_continuationB;
               push_exception_continuationB_return_74:
                *((long *) (SstackS)) = (((len_258X)<<2));
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = (((init_259X)<<2));
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L12443;}}}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 75;
        goto push_exception_continuationB;
       push_exception_continuationB_return_75:
        *((long *) (SstackS)) = arg2_257X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 102 : {
      obj_263X = SvalS;
      if ((3 == (3 & obj_263X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + obj_263X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          goto L30432;}}
      else {
        goto L30432;}}
      break;
    case 103 : {
      SstackS = ((SstackS) + 4);
      arg2_264X = *((long *) (SstackS));
      if ((3 == (3 & arg2_264X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg2_264X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_265X = (((SvalS))>>2);
            len_266X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_264X))) + -4))))>>8);
            if ((index_265X < 0)) {
              goto L29291;}
            else {
              if ((index_265X < len_266X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_264X))) + index_265X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L14545;}
              else {
                goto L29291;}}}
          else {
            goto L30991;}}
        else {
          goto L30991;}}
      else {
        goto L30991;}}
      break;
    case 104 : {
      SstackS = ((SstackS) + 4);
      arg2_267X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_268X = *((long *) (SstackS));
      if ((3 == (3 & arg3_268X))) {
        if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg3_268X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_267X | (SvalS))))) {
            index_269X = ((arg2_267X)>>2);
            Kchar_270X = (((SvalS))>>2);
            if ((3 == (3 & arg3_268X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_268X))) + -4)))))) {
                len_271X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_268X))) + -4))))>>8);
                if ((index_269X < 0)) {
                  goto L28009;}
                else {
                  if ((index_269X < len_271X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_268X))) + index_269X)) = Kchar_270X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L14545;}
                  else {
                    goto L28009;}}}
              else {
                goto L27988;}}
            else {
              goto L27988;}}
          else {
            goto L29839;}}
        else {
          goto L29839;}}
      else {
        goto L29839;}}
      break;
    case 105 : {
      SstackS = ((SstackS) + 4);
      arg2_272X = *((long *) (SstackS));
      if ((0 == (3 & arg2_272X))) {
        if ((9 == (255 & (SvalS)))) {
          len_273X = ((arg2_272X)>>2);
          init_274X = ((((SvalS))>>8));
          size_275X = 1 + ((((4 + len_273X))>>2));
          if ((len_273X < 0)) {
            goto L21850;}
          else {
            if ((4194305 < size_275X)) {
              goto L21850;}
            else {
              v_276X = AVAILABLEp(size_275X);
              if (v_276X) {
                goto L21949;}
              else {
                merged_arg0K0 = 0;
                collect_saving_temp_return_tag = 9;
                goto collect_saving_temp;
               collect_saving_temp_return_9:
                v_277X = AVAILABLEp(size_275X);
                if (v_277X) {
                  goto L21949;}
                else {
                  merged_arg0K0 = 8;
                  merged_arg0K1 = 1;
                  push_exception_continuationB_return_tag = 76;
                  goto push_exception_continuationB;
                 push_exception_continuationB_return_76:
                  *((long *) (SstackS)) = (((len_273X)<<2));
                  SstackS = ((SstackS) + -4);
                  *((long *) (SstackS)) = (9 + ((((init_274X))<<8)));
                  SstackS = ((SstackS) + -4);
                  arg0K0 = 2;
                  goto L12443;}}}}}
        else {
          goto L29071;}}
      else {
        goto L29071;}}
      break;
    case 106 : {
      obj_278X = SvalS;
      if ((3 == (3 & obj_278X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_278X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          goto L30375;}}
      else {
        goto L30375;}}
      break;
    case 107 : {
      SstackS = ((SstackS) + 4);
      arg2_279X = *((long *) (SstackS));
      if ((3 == (3 & arg2_279X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_279X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_280X = (((SvalS))>>2);
            len_281X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_279X))) + -4))))>>8));
            if ((index_280X < 0)) {
              goto L29362;}
            else {
              if ((index_280X < len_281X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_279X))) + index_280X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L14545;}
              else {
                goto L29362;}}}
          else {
            goto L31084;}}
        else {
          goto L31084;}}
      else {
        goto L31084;}}
      break;
    case 108 : {
      SstackS = ((SstackS) + 4);
      arg2_282X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_283X = *((long *) (SstackS));
      if ((3 == (3 & arg3_283X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg3_283X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_282X))) {
            if ((9 == (255 & (SvalS)))) {
              index_284X = ((arg2_282X)>>2);
              Kchar_285X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_283X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_283X))) + -4)))))) {
                  len_286X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_283X))) + -4))))>>8));
                  if ((index_284X < 0)) {
                    goto L27854;}
                  else {
                    if ((index_284X < len_286X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_283X))) + index_284X)) = (Kchar_285X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L14545;}
                    else {
                      goto L27854;}}}
                else {
                  goto L27833;}}
              else {
                goto L27833;}}
            else {
              goto L29713;}}
          else {
            goto L29713;}}
        else {
          goto L29713;}}
      else {
        goto L29713;}}
      break;
    case 109 : {
      obj_287X = SvalS;
      if ((3 == (3 & obj_287X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_287X))) + -4))))>>2))))) {
          x_288X = SvalS;
          descriptor_289X = *((long *) ((((char *) (-3 + x_288X))) + 4));
          if ((17 == (255 & descriptor_289X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_288X))) + 4))))) {
              arg0K0 = 5;
              goto L23984;}
            else {
              arg0K0 = 1;
              goto L23984;}}
          else {
            arg0K0 = 5;
            goto L23984;}}
        else {
          goto L23956;}}
      else {
        goto L23956;}}
      break;
    case 110 : {
      SstackS = ((SstackS) + 4);
      arg2_290X = *((long *) (SstackS));
      if ((3 == (3 & arg2_290X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_290X))) + -4))))>>2))))) {
          x_291X = SvalS;
          if ((1 == x_291X)) {
            goto L25449;}
          else {
            if ((5 == x_291X)) {
              goto L25449;}
            else {
              goto L25475;}}}
        else {
          goto L25475;}}
      else {
        goto L25475;}}
      break;
    case 111 : {
      x_292X = SvalS;
      if ((3 == (3 & x_292X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_292X))) + -4)))))) {
          arg0K0 = 1;
          goto L33298;}
        else {
          arg0K0 = 5;
          goto L33298;}}
      else {
        arg0K0 = 5;
        goto L33298;}}
      break;
    case 112 : {
      x_293X = SvalS;
      if ((3 == (3 & x_293X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_293X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_293X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_293X))) + -4))));
          goto L21799;}
        else {
          goto L21799;}}
      else {
        goto L21799;}}
      break;
    case 113 : {
      v_294X = AVAILABLEp(72);
      if (v_294X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L21398;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 10;
        goto collect_saving_temp;
       collect_saving_temp_return_10:
        v_295X = AVAILABLEp(72);
        if (v_295X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L21398;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L21398;}}}
      break;
    case 114 : {
      v_296X = AVAILABLEp(66);
      if (v_296X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L27715;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 11;
        goto collect_saving_temp;
       collect_saving_temp_return_11:
        v_297X = AVAILABLEp(66);
        if (v_297X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L27715;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L27715;}}}
      break;
    case 115 : {
      v_298X = AVAILABLEp(66);
      if (v_298X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L32217;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 12;
        goto collect_saving_temp;
       collect_saving_temp_return_12:
        v_299X = AVAILABLEp(66);
        if (v_299X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L32217;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L32217;}}}
      break;
    case 116 : {
      v_300X = AVAILABLEp(66);
      if (v_300X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L32432;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 13;
        goto collect_saving_temp;
       collect_saving_temp_return_13:
        v_301X = AVAILABLEp(66);
        if (v_301X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L32432;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L32432;}}}
      break;
    case 117 : {
      obj_302X = SvalS;
      if ((3 == (3 & obj_302X))) {
        if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_302X))) + -4))))>>2))))) {
          channel_303X = SvalS;
          head_304X = Spending_channels_headS;
          if ((1 == head_304X)) {
            n_305X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_303X))) + 8))))>>2)));
            arg0K0 = (((n_305X)<<2));
            goto L21199;}
          else {
            if ((channel_303X == head_304X)) {
              channel_306X = Spending_channels_headS;
              next_307X = *((long *) ((((char *) (-3 + channel_306X))) + 12));
              Spending_channels_headS = next_307X;
              addr_308X = (((char *) (-3 + channel_306X))) + 12;
              WRITE_BARRIER(addr_308X, 1);
              *((long *) addr_308X) = 1;
              if ((1 == next_307X)) {
                Spending_channels_tailS = 1;
                goto L6546;}
              else {
                goto L6546;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_304X))) + 12)));
              arg0K1 = head_304X;
              goto L6552;}}}
        else {
          goto L21200;}}
      else {
        goto L21200;}}
      break;
    case 118 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 119 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 120 : {
      x_309X = SvalS;
      merged_arg0K0 = 16;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 77;
      goto push_exception_continuationB;
     push_exception_continuationB_return_77:
      *((long *) (SstackS)) = x_309X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L12443;}
      break;
    case 121 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 122 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 123 : {
      v_310X = AVAILABLEp(66);
      if (v_310X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L27357;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 14;
        goto collect_saving_temp;
       collect_saving_temp_return_14:
        v_311X = AVAILABLEp(66);
        if (v_311X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L27357;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L27357;}}}
      break;
    case 124 : {
      SvalS = 13;
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 15;
      goto collect_saving_temp;
     collect_saving_temp_return_15:
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 125 : {
      v_312X = AVAILABLEp(6);
      if (v_312X) {
        goto L26975;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 16;
        goto collect_saving_temp;
       collect_saving_temp_return_16:
        v_313X = AVAILABLEp(6);
        if (v_313X) {
          goto L26975;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L26975;}}}
      break;
    case 126 : {
      SstackS = ((SstackS) + 4);
      arg2_314X = *((long *) (SstackS));
      if ((0 == (3 & arg2_314X))) {
        x_315X = SvalS;
        key_316X = ((arg2_314X)>>2);
        if ((5 == key_316X)) {
          SvalS = (-4 & x_315X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          if ((0 == key_316X)) {
            x_317X = available();
            SvalS = (((x_317X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L14545;}
          else {
            if ((1 == key_316X)) {
              bytes_318X = heap_size();
              SvalS = (-4 & (3 + bytes_318X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L14545;}
            else {
              if ((2 == key_316X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L14545;}
              else {
                if ((3 == key_316X)) {
                  x_319X = gc_count();
                  SvalS = (((x_319X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L14545;}
                else {
                  if ((4 == key_316X)) {
                    merged_arg0K0 = 15;
                    merged_arg0K1 = 1;
                    push_exception_continuationB_return_tag = 78;
                    goto push_exception_continuationB;
                   push_exception_continuationB_return_78:
                    *((long *) (SstackS)) = (((key_316X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_315X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L12443;}
                  else {
                    merged_arg0K0 = 18;
                    merged_arg0K1 = 1;
                    push_exception_continuationB_return_tag = 79;
                    goto push_exception_continuationB;
                   push_exception_continuationB_return_79:
                    *((long *) (SstackS)) = (((key_316X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_315X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L12443;}}}}}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 80;
        goto push_exception_continuationB;
       push_exception_continuationB_return_80:
        *((long *) (SstackS)) = arg2_314X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 127 : {
      if ((0 == (3 & (SvalS)))) {
        type_320X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L21074;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 81;
        goto push_exception_continuationB;
       push_exception_continuationB_return_81:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 128 : {
      obj_321X = SvalS;
      if ((3 == (3 & obj_321X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_321X))) + -4))))>>2))))) {
          x_322X = SvalS;
          arg0K0 = x_322X;
          arg2K1 = 1;
          goto L29573;}
        else {
          goto L29595;}}
      else {
        goto L29595;}}
      break;
    case 129 : {
      x_323X = SvalS;
      arg2K0 = 1;
      arg0K1 = x_323X;
      goto L33369;}
      break;
    case 130 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 131 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 132 : {
      SvalS = (Ssession_dataS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 133 : {
      Ssession_dataS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 134 : {
      obj_324X = SvalS;
      if ((3 == (3 & obj_324X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_324X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 158)) {
            goto L16715;}
          else {
            temp_325X = Sexception_handlersS;
            Sexception_handlersS = (SvalS);
            SvalS = temp_325X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L14545;}}
        else {
          goto L16715;}}
      else {
        goto L16715;}}
      break;
    case 135 : {
      SstackS = ((SstackS) + 4);
      p_326X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      template_327X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      p_328X = *((long *) (SstackS));
      StemplateS = template_327X;
      Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_327X)))))))) + ((((((((((p_326X)>>2)) + (((p_328X)>>2))))<<2)))>>2)));
      arg1K0 = (Scode_pointerS);
      goto L14545;}
      break;
    case 136 : {
      obj_329X = SvalS;
      if ((3 == (3 & obj_329X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_329X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 4)) {
            goto L15556;}
          else {
            temp_330X = Sinterrupt_handlersS;
            Sinterrupt_handlersS = (SvalS);
            SvalS = temp_330X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L14545;}}
        else {
          goto L15556;}}
      else {
        goto L15556;}}
      break;
    case 137 : {
      old_331X = Senabled_interruptsS;
      p_332X = SvalS;
      Senabled_interruptsS = (((p_332X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        Spending_interruptPS = 0;
        if ((Spending_eventsPS)) {
          Spending_interruptPS = 1;
          goto L33410;}
        else {
          goto L33410;}}
      else {
        Spending_interruptPS = 1;
        goto L33410;}}
      break;
    case 138 : {
      SstackS = ((SstackS) + 4);
      p_333X = *((long *) (SstackS));
      Senabled_interruptsS = (((p_333X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        Spending_interruptPS = 0;
        if ((Spending_eventsPS)) {
          Spending_interruptPS = 1;
          goto L14559;}
        else {
          goto L14559;}}
      else {
        Spending_interruptPS = 1;
        goto L14559;}}
      break;
    case 139 : {
      if ((0 == (3 & (SvalS)))) {
        p_334X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          Spending_interruptPS = 0;
          if ((Spending_eventsPS)) {
            Spending_interruptPS = 1;
            goto L30143;}
          else {
            goto L30143;}}
        else {
          Spending_interruptPS = 1;
          goto L30143;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 82;
        goto push_exception_continuationB;
       push_exception_continuationB_return_82:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}
      break;
    case 140 : {
      SstackS = ((SstackS) + 4);
      arg2_335X = *((long *) (SstackS));
      if ((0 == (3 & arg2_335X))) {
        x_336X = SvalS;
        if ((1 == x_336X)) {
          goto L25217;}
        else {
          if ((5 == x_336X)) {
            goto L25217;}
          else {
            goto L25237;}}}
      else {
        goto L25237;}}
      break;
    case 141 : {
      obj_337X = SvalS;
      if ((3 == (3 & obj_337X))) {
        if ((13 == (31 & ((((*((long *) ((((char *) (-3 + obj_337X))) + -4))))>>2))))) {
          x_338X = SvalS;
          value_339X = *((long *) ((((char *) (-3 + x_338X))) + 4));
          name_340X = *((long *) (((char *) (-3 + x_338X))));
          if ((3 == (3 & name_340X))) {
            if ((16 == (31 & ((((*((long *) ((((char *) (-3 + name_340X))) + -4))))>>2))))) {
              if ((3 == (3 & value_339X))) {
                if ((17 == (31 & ((((*((long *) ((((char *) (-3 + value_339X))) + -4))))>>2))))) {
                  v_341X = lookup_external_name((((char *) (-3 + name_340X))), (((char *) (-3 + value_339X))));
                  if (v_341X) {
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L14545;}
                  else {
                    goto L26831;}}
                else {
                  goto L26831;}}
              else {
                goto L26831;}}
            else {
              goto L26831;}}
          else {
            goto L26831;}}
        else {
          goto L26836;}}
      else {
        goto L26836;}}
      break;
    case 142 : {
      SstackS = ((SstackS) + 4);
      nargs_342X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      rest_list_343X = *((long *) (SstackS));
      arg0K0 = rest_list_343X;
      goto L13125;}
      break;
    case 143 : {
      SstackS = ((SstackS) + 4);
      arg2_344X = *((long *) (SstackS));
      if ((0 == (3 & arg2_344X))) {
        x_345X = SvalS;
        option_346X = ((arg2_344X)>>2);
        if ((2 == option_346X)) {
          x_347X = CHEAP_TIME();
          SvalS = (((x_347X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          if ((0 == option_346X)) {
            seconds_348X = run_time(&mseconds_349X);
            arg0K0 = option_346X;
            arg0K1 = seconds_348X;
            arg0K2 = mseconds_349X;
            goto L30010;}
          else {
            if ((1 == option_346X)) {
              seconds_350X = real_time(&mseconds_351X);
              arg0K0 = option_346X;
              arg0K1 = seconds_350X;
              arg0K2 = mseconds_351X;
              goto L30010;}
            else {
              merged_arg0K0 = 18;
              merged_arg0K1 = 1;
              push_exception_continuationB_return_tag = 83;
              goto push_exception_continuationB;
             push_exception_continuationB_return_83:
              *((long *) (SstackS)) = (((option_346X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = x_345X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L12443;}}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 84;
        goto push_exception_continuationB;
       push_exception_continuationB_return_84:
        *((long *) (SstackS)) = arg2_344X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 144 : {
      SstackS = ((SstackS) + 4);
      arg2_352X = *((long *) (SstackS));
      if ((0 == (3 & arg2_352X))) {
        x_353X = SvalS;
        key_354X = ((arg2_352X)>>2);
        status_355X = extended_vm(key_354X, x_353X);
        if ((0 == status_355X)) {
          SvalS = (Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          if ((1 == status_355X)) {
            merged_arg0K0 = 20;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 85;
            goto push_exception_continuationB;
           push_exception_continuationB_return_85:
            *((long *) (SstackS)) = (((key_354X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_353X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L12443;}
          else {
            merged_arg0K0 = 21;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 86;
            goto push_exception_continuationB;
           push_exception_continuationB_return_86:
            *((long *) (SstackS)) = (((key_354X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_353X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L12443;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 87;
        goto push_exception_continuationB;
       push_exception_continuationB_return_87:
        *((long *) (SstackS)) = arg2_352X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 145 : {
      SstackS = ((SstackS) + 4);
      arg2_356X = *((long *) (SstackS));
      if ((0 == (3 & arg2_356X))) {
        SvalS = (SvalS);
        arg0K0 = 0;
        goto L15451;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 88;
        goto push_exception_continuationB;
       push_exception_continuationB_return_88:
        *((long *) (SstackS)) = arg2_356X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
      break;
    case 146 : {
      SstackS = ((SstackS) + 4);
      arg2_357X = *((long *) (SstackS));
      if ((3 == (3 & arg2_357X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_357X))) + -4))))>>2))))) {
          obj_358X = SvalS;
          if ((3 == (3 & obj_358X))) {
            if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_358X))) + -4))))>>2))))) {
              x_359X = SvalS;
              len_360X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_357X))) + -4))))>>8);
              if ((len_360X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_359X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_359X))), (void *)(((char *) (-3 + arg2_357X))),len_360X)))) {
                  arg0K0 = 5;
                  goto L23782;}
                else {
                  arg0K0 = 1;
                  goto L23782;}}
              else {
                arg0K0 = 1;
                goto L23782;}}
            else {
              goto L23749;}}
          else {
            goto L23749;}}
        else {
          goto L23749;}}
      else {
        goto L23749;}}
      break;
    case 147 : {
      obj_361X = SvalS;
      if ((3 == (3 & obj_361X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_361X))) + -4))))>>2))))) {
          x_362X = SvalS;
          n_363X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_362X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L30099;}
        else {
          goto L30076;}}
      else {
        goto L30076;}}
      break;
    case 148 : {
      space_364X = 1 + ((((4 + ((((SvalS))>>2))))>>2));
      v_365X = AVAILABLEp(space_364X);
      if (v_365X) {
        goto L20548;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 17;
        goto collect_saving_temp;
       collect_saving_temp_return_17:
        v_366X = AVAILABLEp(space_364X);
        if (v_366X) {
          goto L20548;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L20548;}}}
      break;
    case 149 : {
      v_367X = AVAILABLEp(5);
      if (v_367X) {
        goto L20406;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 18;
        goto collect_saving_temp;
       collect_saving_temp_return_18:
        v_368X = AVAILABLEp(5);
        if (v_368X) {
          goto L20406;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L20406;}}}
      break;
    case 150 : {
      SstackS = ((SstackS) + 4);
      arg2_369X = *((long *) (SstackS));
      x_370X = SvalS;
      arg0K0 = x_370X;
      arg0K1 = x_370X;
      arg2K2 = 1;
      goto L23474;}
      break;
    case 151 : {
      SstackS = ((SstackS) + 4);
      arg2_371X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_372X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        index_373X = (((SvalS))>>2);
        if ((3 == (3 & arg3_372X))) {
          if ((8 == (31 & ((((*((long *) ((((char *) (-3 + arg3_372X))) + -4))))>>2))))) {
            if ((arg2_371X == (*((long *) (((char *) (-3 + arg3_372X))))))) {
              len_374X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_372X))) + -4))))>>8))))>>2);
              if ((index_373X < 0)) {
                goto L20192;}
              else {
                if ((index_373X < len_374X)) {
                  SvalS = (*((long *) ((((char *) (-3 + arg3_372X))) + (((index_373X)<<2)))));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L14545;}
                else {
                  goto L20192;}}}
            else {
              goto L20239;}}
          else {
            goto L20239;}}
        else {
          goto L20239;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 89;
        goto push_exception_continuationB;
       push_exception_continuationB_return_89:
        *((long *) (SstackS)) = arg3_372X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_371X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L12443;}}
      break;
    case 152 : {
      SstackS = ((SstackS) + 4);
      arg2_375X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_376X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_377X = *((long *) (SstackS));
      if ((0 == (3 & arg2_375X))) {
        x_378X = SvalS;
        index_379X = ((arg2_375X)>>2);
        if ((3 == (3 & arg4_377X))) {
          if ((8 == (31 & ((((*((long *) ((((char *) (-3 + arg4_377X))) + -4))))>>2))))) {
            if ((arg3_376X == (*((long *) (((char *) (-3 + arg4_377X))))))) {
              if ((3 == (3 & arg4_377X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_377X))) + -4)))))) {
                  len_380X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_377X))) + -4))))>>8))))>>2);
                  if ((index_379X < 0)) {
                    goto L26482;}
                  else {
                    if ((index_379X < len_380X)) {
                      addr_381X = (((char *) (-3 + arg4_377X))) + (((index_379X)<<2));
                      WRITE_BARRIER(addr_381X, x_378X);
                      *((long *) addr_381X) = x_378X;
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L14545;}
                    else {
                      goto L26482;}}}
                else {
                  goto L26539;}}
              else {
                goto L26539;}}
            else {
              goto L26539;}}
          else {
            goto L26539;}}
        else {
          goto L26539;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 90;
        goto push_exception_continuationB;
       push_exception_continuationB_return_90:
        *((long *) (SstackS)) = arg4_377X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_376X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_375X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 4;
        goto L12443;}}
      break;
    case 153 : {
      SstackS = ((SstackS) + 4);
      arg2_382X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_383X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_384X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_385X = *((long *) (SstackS));
      if ((0 == (3 & ((arg4_384X | arg2_382X) | (SvalS))))) {
        from_index_386X = ((arg4_384X)>>2);
        to_index_387X = ((arg2_382X)>>2);
        count_388X = (((SvalS))>>2);
        if ((3 == (3 & arg5_385X))) {
          if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_385X))) + -4))))>>2))))) {
            goto L25846;}
          else {
            goto L25841;}}
        else {
          goto L25841;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 91;
        goto push_exception_continuationB;
       push_exception_continuationB_return_91:
        *((long *) (SstackS)) = arg5_385X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_384X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_383X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_382X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 5;
        goto L12443;}}
      break;
    case 154 : {
      v_389X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_389X)) {
        arg0K0 = (SvalS);
        goto L17713;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_390X = get_current_port0_return_value;
        arg0K0 = v_390X;
        goto L17713;}}
      break;
    case 155 : {
      v_391X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_391X)) {
        arg0K0 = (SvalS);
        goto L17510;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_392X = get_current_port0_return_value;
        arg0K0 = v_392X;
        goto L17510;}}
      break;
    case 156 : {
      v_393X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_393X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = (SvalS);
        goto L17257;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_394X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_394X;
        goto L17257;}}
      break;
    case 157 : {
      x_395X = SvalS;
      out_396X = stderr;
      arg0K0 = x_395X;
      goto L25126;}
      break;
  }}
 L12443: {
  nargs_397X = arg0K0;
  opcode_398X = (((*((long *) ((SstackS) + (8 + (((nargs_397X)<<2)))))))>>2);
  obj_399X = Sexception_handlersS;
  if ((3 == (3 & obj_399X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_399X))) + -4))))>>2))))) {
      goto L12502;}
    else {
      goto L12575;}}
  else {
    goto L12575;}}
 L19926: {
  env_400X = arg0K0;
  i_401X = arg0K1;
  if ((0 == i_401X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_400X))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
    if ((529 == (SvalS))) {
      merged_arg0K0 = 0;
      merged_arg0K1 = 3;
      push_exception_continuationB_return_tag = 92;
      goto push_exception_continuationB;
     push_exception_continuationB_return_92:
      arg0K0 = 0;
      goto L12443;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L14545;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_400X)))));
    arg0K1 = (-1 + i_401X);
    goto L19926;}}
 L16311: {
  env_402X = arg0K0;
  i_403X = arg0K1;
  if ((0 == i_403X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_402X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 2))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 3)))))<<2)))));
    if ((529 == (SvalS))) {
      merged_arg0K0 = 0;
      merged_arg0K1 = 4;
      push_exception_continuationB_return_tag = 93;
      goto push_exception_continuationB;
     push_exception_continuationB_return_93:
      arg0K0 = 0;
      goto L12443;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L14545;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_402X)))));
    arg0K1 = (-1 + i_403X);
    goto L16311;}}
 L16249: {
  env_404X = arg0K0;
  i_405X = arg0K1;
  if ((0 == i_405X)) {
    index_406X = ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 3)));
    value_407X = SvalS;
    addr_408X = (((char *) (-3 + env_404X))) + (((index_406X)<<2));
    WRITE_BARRIER(addr_408X, value_407X);
    *((long *) addr_408X) = value_407X;
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 4);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_404X)))));
    arg0K1 = (-1 + i_405X);
    goto L16249;}}
 L17042: {
  space_409X = arg0K0;
  v_410X = AVAILABLEp(space_409X);
  if (v_410X) {
    arg2K0 = 1;
    arg0K1 = 0;
    goto L17076;}
  else {
    merged_arg0K0 = 0;
    collect_saving_temp_return_tag = 19;
    goto collect_saving_temp;
   collect_saving_temp_return_19:
    v_411X = AVAILABLEp(space_409X);
    if (v_411X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L17076;}
    else {
      arg2K0 = 0;
      arg0K1 = 0;
      goto L17076;}}}
 L16959: {
  okayP_412X = arg2K0;
  key_413X = arg0K1;
  if (okayP_412X) {
    arg0K0 = key_413X;
    goto L16910;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_413X;
    goto L16910;}}
 L16837: {
  okayP_414X = arg2K0;
  key_415X = arg0K1;
  if (okayP_414X) {
    arg0K0 = key_415X;
    goto L16825;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_415X;
    goto L16825;}}
 L15909: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L15946: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_94X;
  ScontS = (Sbottom_of_stackS);
  goto L15909;}
 L15910: {
  if ((1 == cont_94X)) {
    if ((0 == (3 & (SvalS)))) {
      SvalS = ((((SvalS))>>2));
      arg0K0 = 0;
      goto L15451;}
    else {
      goto L15927;}}
  else {
    goto L15927;}}
 L15313: {
  okayP_416X = arg2K0;
  key_417X = arg0K1;
  if (okayP_416X) {
    arg0K0 = key_417X;
    goto L15268;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_417X;
    goto L15268;}}
 L15225: {
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 0;
      goto check_events;
     check_events_return_0:
      v_418X = check_events0_return_value;
      if (v_418X) {
        arg0K0 = stack_arg_count_95X;
        goto L12672;}
      else {
        goto L15229;}}
    else {
      arg0K0 = stack_arg_count_95X;
      goto L12672;}}
  else {
    goto L15229;}}
 L11844: {
  stack_arg_count_419X = arg0K0;
  code_420X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_420X))) + 1)));
  arg0K1 = 64;
  goto L11858;}
 L12939: {
  exception_421X = arg0K0;
  stack_arg_count_422X = arg0K1;
  list_args_423X = arg0K2;
  list_arg_count_424X = arg0K3;
  if (((StemplateS) == (SvalS))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      arg0K0 = v_425X;
      goto L15451;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      arg0K0 = v_426X;
      goto L15451;}}
  else {
    merged_arg0K0 = list_args_423X;
    merged_arg0K1 = list_arg_count_424X;
    copy_listS_return_tag = 0;
    goto copy_listS;
   copy_listS_return_0:
    v_427X = copy_listS0_return_value;
    merged_arg0K0 = v_427X;
    merged_arg0K1 = stack_arg_count_422X;
    pop_args_GlistS_return_tag = 1;
    goto pop_args_GlistS;
   pop_args_GlistS_return_1:
    args_428X = pop_args_GlistS0_return_value;
    merged_arg0K0 = exception_421X;
    merged_arg0K1 = 0;
    push_exception_continuationB_return_tag = 94;
    goto push_exception_continuationB;
   push_exception_continuationB_return_94:
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_428X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}}
 L14309: {
  loc_429X = arg1K0;
  arg_430X = arg1K1;
  if ((top_of_args_105X < arg_430X)) {
    *((long *) loc_429X) = (*((long *) arg_430X));
    arg1K0 = (loc_429X + -4);
    arg1K1 = (arg_430X + -4);
    goto L14309;}
  else {
    SstackS = loc_429X;
    obj_431X = SvalS;
    if ((3 == (3 & obj_431X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_431X))) + -4))))>>2))))) {
        arg0K0 = nargs_104X;
        goto L11844;}
      else {
        arg0K0 = 3;
        arg0K1 = nargs_104X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L12939;}}
    else {
      arg0K0 = 3;
      arg0K1 = nargs_104X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L12939;}}}
 L14347: {
  stack_arg_count_432X = arg0K0;
  list_args_433X = arg0K1;
  list_arg_count_434X = arg0K2;
  if ((0 == list_arg_count_434X)) {
    obj_435X = SvalS;
    if ((3 == (3 & obj_435X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_435X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_432X;
        goto L11844;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_432X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L12939;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_432X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L12939;}}
  else {
    obj_436X = SvalS;
    if ((3 == (3 & obj_436X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_436X))) + -4))))>>2))))) {
        code_437X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
        total_arg_count_438X = stack_arg_count_432X + list_arg_count_434X;
        arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_437X))) + 1)));
        arg0K1 = 64;
        goto L12135;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_432X;
        arg0K2 = list_args_433X;
        arg0K3 = list_arg_count_434X;
        goto L12939;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_432X;
      arg0K2 = list_args_433X;
      arg0K3 = list_arg_count_434X;
      goto L12939;}}}
 L8950: {
  list_args_439X = arg0K0;
  stack_nargs_440X = arg0K1;
  merged_arg0K0 = list_args_439X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_441X = okay_argument_list0_return_value;
  list_arg_count_442X = okay_argument_list1_return_value;
  if (okayP_441X) {
    SvalS = proc_112X;
    arg0K0 = stack_nargs_440X;
    arg0K1 = list_args_439X;
    arg0K2 = list_arg_count_442X;
    goto L14347;}
  else {
    *((long *) (SstackS)) = list_args_439X;
    SstackS = ((SstackS) + -4);
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_440X);
    pop_args_GlistS_return_tag = 2;
    goto pop_args_GlistS;
   pop_args_GlistS_return_2:
    args_443X = pop_args_GlistS0_return_value;
    SstackS = ((SstackS) + 4);
    merged_arg0K0 = 5;
    merged_arg0K1 = 0;
    push_exception_continuationB_return_tag = 95;
    goto push_exception_continuationB;
   push_exception_continuationB_return_95:
    *((long *) (SstackS)) = proc_112X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_443X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}}
 L8991: {
  list_444X = arg0K0;
  follower_445X = arg0K1;
  if ((25 == list_444X)) {
    list_args_446X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_445X))) + 4)))))));
    addr_447X = (((char *) (-3 + follower_445X))) + 4;
    WRITE_BARRIER(addr_447X, list_args_446X);
    *((long *) addr_447X) = list_args_446X;
    arg0K0 = rest_list_113X;
    arg0K1 = (-1 + stack_nargs_111X);
    goto L8950;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_444X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_445X))) + 4)));
    goto L8991;}}
 L14252: {
  obj_448X = SvalS;
  if ((3 == (3 & obj_448X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_448X))) + -4))))>>2))))) {
      arg0K0 = 0;
      goto L11844;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L12939;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L12939;}}
 L14259: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_114X;
  ScontS = (Sbottom_of_stackS);
  goto L14252;}
 L16479: {
  stack_nargs_449X = arg0K0;
  list_args_450X = arg0K1;
  list_arg_count_451X = arg0K2;
  if ((1 == stack_nargs_449X)) {
    SstackS = ((SstackS) + 4);
    SvalS = (*((long *) (SstackS)));
    pop_continuationB_return_tag = 1;
    goto pop_continuationB;
   pop_continuationB_return_1:
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    if (((ScontS) == (Sbottom_of_stackS))) {
      arg0K0 = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
      goto L16490;}
    else {
      arg0K0 = (ScontS);
      goto L16490;}}}
 L15051: {
  okayP_452X = arg2K0;
  key_453X = arg0K1;
  if (okayP_452X) {
    arg0K0 = key_453X;
    goto L15039;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_453X;
    goto L15039;}}
 L14924: {
  tem_454X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  StemplateS = tem_454X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_454X)))))))) + 2);
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 1;
      goto check_events;
     check_events_return_1:
      v_455X = check_events0_return_value;
      if (v_455X) {
        goto L14933;}
      else {
        goto L14936;}}
    else {
      goto L14933;}}
  else {
    goto L14936;}}
 L15106: {
  okayP_456X = arg2K0;
  key_457X = arg0K1;
  if (okayP_456X) {
    arg0K0 = key_457X;
    goto L15093;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_457X;
    goto L15093;}}
 L14969: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 4;
  push_exception_continuationB_return_tag = 96;
  goto push_exception_continuationB;
 push_exception_continuationB_return_96:
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L14825: {
  arg0K0 = (2 + (((max_131X)<<1)));
  goto L14827;}
 L14827: {
  offset_458X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_458X);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L15812: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L14723: {
  delta_459X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_459X);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L15703: {
  delta_460X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_460X);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L32849: {
  val_461X = arg0K0;
  SvalS = val_461X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24899: {
  val_462X = arg0K0;
  SvalS = val_462X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24854: {
  if ((3 == (3 & x_145X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_145X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L24899;}
    else {
      goto L24862;}}
  else {
    goto L24862;}}
 L23276: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L23279: {
  if ((3 == (3 & x_146X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_146X))) + -4))))>>2))))) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 97;
      goto push_exception_continuationB;
     push_exception_continuationB_return_97:
      *((long *) (SstackS)) = x_146X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L12443;}
    else {
      goto L23287;}}
  else {
    goto L23287;}}
 L23085: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L23068: {
  if ((3 == (3 & x_147X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_147X))) + -4))))>>2))))) {
      goto L23085;}
    else {
      goto L23076;}}
  else {
    goto L23076;}}
 L22874: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L22857: {
  if ((3 == (3 & x_148X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_148X))) + -4))))>>2))))) {
      goto L22874;}
    else {
      goto L22865;}}
  else {
    goto L22865;}}
 L22663: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L22646: {
  if ((3 == (3 & x_149X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_149X))) + -4))))>>2))))) {
      goto L22663;}
    else {
      goto L22654;}}
  else {
    goto L22654;}}
 L5777: {
  a_463X = arg0K0;
  if ((b_158X < 0)) {
    arg0K0 = (0 - b_158X);
    goto L5781;}
  else {
    arg0K0 = b_158X;
    goto L5781;}}
 L7215: {
  a_464X = arg0K0;
  if ((b_165X < 0)) {
    arg0K0 = (0 - b_165X);
    goto L7217;}
  else {
    arg0K0 = b_165X;
    goto L7217;}}
 L24750: {
  val_465X = arg0K0;
  SvalS = val_465X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24660: {
  val_466X = arg0K0;
  SvalS = val_466X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24570: {
  val_467X = arg0K0;
  SvalS = val_467X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24465: {
  val_468X = arg0K0;
  SvalS = val_468X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24358: {
  val_469X = arg0K0;
  SvalS = val_469X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L28479: {
  a_470X = arg0K0;
  if ((b_179X < 0)) {
    arg0K0 = (0 - b_179X);
    goto L28481;}
  else {
    arg0K0 = b_179X;
    goto L28481;}}
 L28347: {
  a_471X = arg0K0;
  n_472X = ((x_181X)>>2);
  if ((n_472X < 0)) {
    arg0K0 = (0 - n_472X);
    goto L28349;}
  else {
    arg0K0 = n_472X;
    goto L28349;}}
 L22424: {
  r_473X = arg0K0;
  if ((536870911 < r_473X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 98;
    goto push_exception_continuationB;
   push_exception_continuationB_return_98:
    *((long *) (SstackS)) = x_192X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 1;
    goto L12443;}
  else {
    SvalS = (((r_473X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L28197: {
  SvalS = (((result_208X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L33192: {
  val_474X = arg0K0;
  SvalS = val_474X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24230: {
  val_475X = arg0K0;
  SvalS = val_475X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24203: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 99;
  goto push_exception_continuationB;
 push_exception_continuationB_return_99:
  *((long *) (SstackS)) = arg2_211X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L24140: {
  val_476X = arg0K0;
  SvalS = val_476X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L24113: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 100;
  goto push_exception_continuationB;
 push_exception_continuationB_return_100:
  *((long *) (SstackS)) = arg2_213X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L22324: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 101;
  goto push_exception_continuationB;
 push_exception_continuationB_return_101:
  *((long *) (SstackS)) = (((x_215X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L33221: {
  val_477X = arg0K0;
  SvalS = val_477X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L33235: {
  value_478X = arg0K0;
  SvalS = value_478X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L19673: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 102;
  goto push_exception_continuationB;
 push_exception_continuationB_return_102:
  *((long *) (SstackS)) = x_219X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_220X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L19479: {
  type_479X = *((unsigned char *) ((Scode_pointerS) + 2));
  len_480X = ((len_221X)<<2);
  addr_481X = ALLOCATE_SPACE(type_479X, (4 + len_480X));
  *((long *) addr_481X) = (2 + (((((((len_480X)<<6)) + type_479X))<<2)));
  new_482X = 3 + (((long) (addr_481X + 4)));
  if ((len_221X < 1)) {
    goto L19514;}
  else {
    *((long *) ((((char *) (-3 + new_482X))) + (-4 + (((len_221X)<<2))))) = (SvalS);
    arg0K0 = (-2 + len_221X);
    goto L19498;}}
 L19270: {
  type_483X = *((unsigned char *) ((Scode_pointerS) + 1));
  len_484X = ((len_225X)<<2);
  addr_485X = ALLOCATE_SPACE(type_483X, (4 + len_484X));
  *((long *) addr_485X) = (2 + (((((((len_484X)<<6)) + type_483X))<<2)));
  new_486X = 3 + (((long) (addr_485X + 4)));
  SstackS = ((SstackS) + 4);
  stack_nargs_487X = (((*((long *) (SstackS))))>>2);
  SstackS = ((SstackS) + 4);
  rest_list_488X = *((long *) (SstackS));
  arg0K0 = (-1 + stack_nargs_487X);
  goto L19292;}
 L19168: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 3;
  push_exception_continuationB_return_tag = 103;
  goto push_exception_continuationB;
 push_exception_continuationB_return_103:
  *((long *) (SstackS)) = x_229X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_231X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_230X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L18956: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 3;
  push_exception_continuationB_return_tag = 104;
  goto push_exception_continuationB;
 push_exception_continuationB_return_104:
  *((long *) (SstackS)) = arg2_232X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_235X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_234X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_233X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L12443;}
 L18628: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 105;
  goto push_exception_continuationB;
 push_exception_continuationB_return_105:
  *((long *) (SstackS)) = (((type_239X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_240X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_238X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L18646: {
  okayP_489X = arg2K0;
  init_490X = arg0K1;
  if (okayP_489X) {
    len_491X = ((len_240X)<<2);
    addr_492X = ALLOCATE_SPACE(type_239X, (4 + len_491X));
    *((long *) addr_492X) = (2 + (((((((len_491X)<<6)) + type_239X))<<2)));
    value_493X = 3 + (((long) (addr_492X + 4)));
    arg0K0 = (-1 + len_240X);
    goto L18675;}
  else {
    merged_arg0K0 = 8;
    merged_arg0K1 = 2;
    push_exception_continuationB_return_tag = 106;
    goto push_exception_continuationB;
   push_exception_continuationB_return_106:
    *((long *) (SstackS)) = (((type_239X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_240X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = init_490X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L12443;}}
 L18365: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 107;
  goto push_exception_continuationB;
 push_exception_continuationB_return_107:
  *((long *) (SstackS)) = arg2_245X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_247X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_246X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L18342: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 108;
  goto push_exception_continuationB;
 push_exception_continuationB_return_108:
  *((long *) (SstackS)) = arg2_245X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_247X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_246X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L18006: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 109;
  goto push_exception_continuationB;
 push_exception_continuationB_return_109:
  *((long *) (SstackS)) = arg3_251X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_253X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_250X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_252X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L12443;}
 L17981: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 110;
  goto push_exception_continuationB;
 push_exception_continuationB_return_110:
  *((long *) (SstackS)) = arg3_251X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_253X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_250X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_252X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L12443;}
 L22089: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 111;
  goto push_exception_continuationB;
 push_exception_continuationB_return_111:
  *((long *) (SstackS)) = (((len_258X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_259X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L22188: {
  addr_494X = ALLOCATE_SPACE(17, (4 + len_258X));
  *((long *) addr_494X) = (70 + (((len_258X)<<8)));
  vector_495X = 3 + (((long) (addr_494X + 4)));
  arg0K0 = (-1 + len_258X);
  goto L22134;}
 L30432: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 112;
  goto push_exception_continuationB;
 push_exception_continuationB_return_112:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L29291: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 113;
  goto push_exception_continuationB;
 push_exception_continuationB_return_113:
  *((long *) (SstackS)) = arg2_264X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_265X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L30991: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 114;
  goto push_exception_continuationB;
 push_exception_continuationB_return_114:
  *((long *) (SstackS)) = arg2_264X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L28009: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 115;
  goto push_exception_continuationB;
 push_exception_continuationB_return_115:
  *((long *) (SstackS)) = arg3_268X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_269X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_270X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L27988: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 116;
  goto push_exception_continuationB;
 push_exception_continuationB_return_116:
  *((long *) (SstackS)) = arg3_268X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_269X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_270X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L29839: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 117;
  goto push_exception_continuationB;
 push_exception_continuationB_return_117:
  *((long *) (SstackS)) = arg3_268X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_267X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L21850: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 118;
  goto push_exception_continuationB;
 push_exception_continuationB_return_118:
  *((long *) (SstackS)) = (((len_273X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_274X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L21949: {
  len_496X = 1 + len_273X;
  addr_497X = ALLOCATE_SPACE(16, (4 + len_496X));
  *((long *) addr_497X) = (66 + (((len_496X)<<8)));
  string_498X = 3 + (((long) (addr_497X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_498X))) + len_273X)) = 0;
  arg0K0 = (-1 + len_273X);
  goto L21895;}
 L29071: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 119;
  goto push_exception_continuationB;
 push_exception_continuationB_return_119:
  *((long *) (SstackS)) = arg2_272X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L30375: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 120;
  goto push_exception_continuationB;
 push_exception_continuationB_return_120:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L29362: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 121;
  goto push_exception_continuationB;
 push_exception_continuationB_return_121:
  *((long *) (SstackS)) = arg2_279X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_280X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L31084: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 122;
  goto push_exception_continuationB;
 push_exception_continuationB_return_122:
  *((long *) (SstackS)) = arg2_279X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L27854: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 123;
  goto push_exception_continuationB;
 push_exception_continuationB_return_123:
  *((long *) (SstackS)) = arg3_283X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_284X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_285X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L27833: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 124;
  goto push_exception_continuationB;
 push_exception_continuationB_return_124:
  *((long *) (SstackS)) = arg3_283X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_284X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_285X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L29713: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 125;
  goto push_exception_continuationB;
 push_exception_continuationB_return_125:
  *((long *) (SstackS)) = arg3_283X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_282X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L23984: {
  val_499X = arg0K0;
  SvalS = val_499X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L23956: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 126;
  goto push_exception_continuationB;
 push_exception_continuationB_return_126:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L25449: {
  if ((1 == (SvalS))) {
    addr_500X = (((char *) (-3 + arg2_290X))) + 4;
    WRITE_BARRIER(addr_500X, 273);
    *((long *) addr_500X) = 273;
    goto L25474;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_290X))) + 4)))))) {
      addr_501X = (((char *) (-3 + arg2_290X))) + 4;
      WRITE_BARRIER(addr_501X, 529);
      *((long *) addr_501X) = 529;
      goto L25474;}
    else {
      goto L25474;}}}
 L25475: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 127;
  goto push_exception_continuationB;
 push_exception_continuationB_return_127:
  *((long *) (SstackS)) = arg2_290X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L33298: {
  val_502X = arg0K0;
  SvalS = val_502X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L21799: {
  SvalS = x_293X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L21398: {
  okayP_503X = arg2K0;
  key_504X = arg0K1;
  if (okayP_503X) {
    arg0K0 = key_504X;
    goto L21247;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_504X;
    goto L21247;}}
 L27715: {
  okayP_505X = arg2K0;
  key_506X = arg0K1;
  if (okayP_505X) {
    arg0K0 = key_506X;
    goto L27670;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_506X;
    goto L27670;}}
 L32217: {
  okayP_507X = arg2K0;
  key_508X = arg0K1;
  if (okayP_507X) {
    arg0K0 = key_508X;
    goto L32147;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_508X;
    goto L32147;}}
 L32432: {
  okayP_509X = arg2K0;
  key_510X = arg0K1;
  if (okayP_509X) {
    arg0K0 = key_510X;
    goto L32368;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_510X;
    goto L32368;}}
 L21199: {
  val_511X = arg0K0;
  SvalS = val_511X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L6546: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_303X))) + 16)));
  goto L21199;}
 L6552: {
  ch_512X = arg0K0;
  prev_513X = arg0K1;
  if ((1 == ch_512X)) {
    n_514X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_303X))) + 8))))>>2)));
    arg0K0 = (((n_514X)<<2));
    goto L21199;}
  else {
    if ((ch_512X == channel_303X)) {
      y_515X = Spending_channels_tailS;
      if ((ch_512X == y_515X)) {
        Spending_channels_tailS = prev_513X;
        goto L6574;}
      else {
        goto L6574;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_512X))) + 12)));
      arg0K1 = ch_512X;
      goto L6552;}}}
 L21200: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 128;
  goto push_exception_continuationB;
 push_exception_continuationB_return_128:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L27357: {
  okayP_516X = arg2K0;
  key_517X = arg0K1;
  if (okayP_516X) {
    arg0K0 = key_517X;
    goto L27221;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_517X;
    goto L27221;}}
 L26975: {
  SstackS = ((SstackS) + 4);
  arg2_518X = *((long *) (SstackS));
  x_519X = SvalS;
  if ((3 == (3 & arg2_518X))) {
    if ((3 == (3 & x_519X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + x_519X))) + -4))))>>2))))) {
        addr_520X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_520X) = 2050;
        x_521X = 3 + (((long) (addr_520X + 4)));
        *((long *) (((char *) (-3 + x_521X)))) = arg2_518X;
        *((long *) ((((char *) (-3 + x_521X))) + 4)) = x_519X;
        b_522X = Sfinalizer_alistS;
        addr_523X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_523X) = 2050;
        x_524X = 3 + (((long) (addr_523X + 4)));
        *((long *) (((char *) (-3 + x_524X)))) = x_521X;
        *((long *) ((((char *) (-3 + x_524X))) + 4)) = b_522X;
        Sfinalizer_alistS = x_524X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        goto L27075;}}
    else {
      goto L27075;}}
  else {
    goto L27075;}}
 L21074: {
  firstP_525X = arg2K0;
  vector_526X = find_all(type_320X);
  if ((1 == vector_526X)) {
    if (firstP_525X) {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 20;
      goto collect_saving_temp;
     collect_saving_temp_return_20:
      arg2K0 = 0;
      goto L21074;}
    else {
      merged_arg0K0 = 8;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 129;
      goto push_exception_continuationB;
     push_exception_continuationB_return_129:
      *((long *) (SstackS)) = (((type_320X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L12443;}}
  else {
    SvalS = vector_526X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L29573: {
  table_527X = arg0K0;
  firstP_528X = arg2K1;
  symbols_529X = find_all(1);
  if ((1 == symbols_529X)) {
    if (firstP_528X) {
      merged_arg0K0 = table_527X;
      collect_saving_temp_return_tag = 21;
      goto collect_saving_temp;
     collect_saving_temp_return_21:
      v_530X = collect_saving_temp0_return_value;
      arg0K0 = v_530X;
      arg2K1 = 0;
      goto L29573;}
    else {
      merged_arg0K0 = 8;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 130;
      goto push_exception_continuationB;
     push_exception_continuationB_return_130:
      *((long *) (SstackS)) = table_527X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L12443;}}
  else {
    arg0K0 = table_527X;
    arg0K1 = symbols_529X;
    arg2K2 = 1;
    goto L25642;}}
 L29595: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 131;
  goto push_exception_continuationB;
 push_exception_continuationB_return_131:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L33369: {
  firstP_531X = arg2K0;
  type_532X = arg0K1;
  vector_533X = find_all_records(type_532X);
  if ((1 == vector_533X)) {
    if (firstP_531X) {
      merged_arg0K0 = type_532X;
      collect_saving_temp_return_tag = 22;
      goto collect_saving_temp;
     collect_saving_temp_return_22:
      v_534X = collect_saving_temp0_return_value;
      arg2K0 = 0;
      arg0K1 = v_534X;
      goto L33369;}
    else {
      merged_arg0K0 = 8;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 132;
      goto push_exception_continuationB;
     push_exception_continuationB_return_132:
      *((long *) (SstackS)) = type_532X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L12443;}}
  else {
    SvalS = vector_533X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L16715: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 133;
  goto push_exception_continuationB;
 push_exception_continuationB_return_133:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L15556: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 134;
  goto push_exception_continuationB;
 push_exception_continuationB_return_134:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L33410: {
  SvalS = (((old_331X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L14559: {
  SstackS = ((SstackS) + 4);
  pc_535X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tem_536X = *((long *) (SstackS));
  StemplateS = tem_536X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_536X)))))))) + (((pc_535X)>>2)));
  SstackS = ((SstackS) + 4);
  SvalS = (*((long *) (SstackS)));
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L30143: {
  x_537X = schedule_alarm_interrupt((((p_334X)>>2)));
  SvalS = (((x_537X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L25217: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L25221;}
  else {
    arg2K0 = 1;
    goto L25221;}}
 L25237: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 135;
  goto push_exception_continuationB;
 push_exception_continuationB_return_135:
  *((long *) (SstackS)) = arg2_335X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L26831: {
  merged_arg0K0 = 19;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 136;
  goto push_exception_continuationB;
 push_exception_continuationB_return_136:
  *((long *) (SstackS)) = x_338X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L26836: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 137;
  goto push_exception_continuationB;
 push_exception_continuationB_return_137:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L13125: {
  rest_list_538X = arg0K0;
  if ((25 == rest_list_538X)) {
    proc_539X = *((long *) ((SstackS) + (((nargs_342X)<<2))));
    if ((3 == (3 & proc_539X))) {
      if ((13 == (31 & ((((*((long *) ((((char *) (-3 + proc_539X))) + -4))))>>2))))) {
        Sexternal_call_nargsS = (-1 + nargs_342X);
        SvalS = proc_539X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg0K0 = 1;
        goto L15451;}
      else {
        goto L13195;}}
    else {
      goto L13195;}}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + rest_list_538X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_538X))) + 4)));
    goto L13125;}}
 L30010: {
  option_540X = arg0K0;
  seconds_541X = arg0K1;
  mseconds_542X = arg0K2;
  if ((536869 < seconds_541X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 138;
    goto push_exception_continuationB;
   push_exception_continuationB_return_138:
    *((long *) (SstackS)) = (((option_540X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_541X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_542X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L12443;}
  else {
    SvalS = (((((1000 * seconds_541X) + mseconds_542X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L15451: {
  option_543X = arg0K0;
  if ((0 == option_543X)) {
    return (SvalS);}
  else {
    if ((1 == option_543X)) {
      v_544X = (long(*)())((*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (SvalS)))) + 4)))))))));
      v_545X = (*v_544X)((Sexternal_call_nargsS), ((SstackS) + 4));
      SvalS = v_545X;
      SstackS = ((SstackS) + (0 - ((((-1 - (Sexternal_call_nargsS)))<<2))));
      goto L15449;}
    else {
      ps_error("unknown VM return option", 1, option_543X);
      return -1;}}}
 L23782: {
  val_546X = arg0K0;
  SvalS = val_546X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L23749: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 139;
  goto push_exception_continuationB;
 push_exception_continuationB_return_139:
  *((long *) (SstackS)) = arg2_357X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L30099: {
  i_547X = arg0K0;
  h_548X = arg0K1;
  if ((i_547X < n_363X)) {
    arg0K0 = (1 + i_547X);
    arg0K1 = (h_548X + (((*((unsigned char *) ((((char *) (-3 + x_362X))) + i_547X))))));
    goto L30099;}
  else {
    SvalS = (((h_548X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L30076: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 140;
  goto push_exception_continuationB;
 push_exception_continuationB_return_140:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L20548: {
  SstackS = ((SstackS) + 4);
  arg2_549X = *((long *) (SstackS));
  if ((0 == (3 & (SvalS)))) {
    n_550X = (((SvalS))>>2);
    if ((3 == (3 & arg2_549X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_549X))) + -4))))>>2))))) {
        goto L20676;}
      else {
        goto L20581;}}
    else {
      goto L20581;}}
  else {
    merged_arg0K0 = 5;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 141;
    goto push_exception_continuationB;
   push_exception_continuationB_return_141:
    *((long *) (SstackS)) = arg2_549X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}}
 L20406: {
  SstackS = ((SstackS) + 4);
  arg2_551X = *((long *) (SstackS));
  if ((3 == (3 & arg2_551X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_551X))) + -4))))>>2))))) {
      obj_552X = SvalS;
      if ((3 == (3 & obj_552X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_552X))) + -4))))>>2))))) {
          table_553X = SvalS;
          v_554X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_553X))) + -4))))>>8))))>>2);
          n_555X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_551X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L10052;}
        else {
          goto L20430;}}
      else {
        goto L20430;}}
    else {
      goto L20430;}}
  else {
    goto L20430;}}
 L23474: {
  list_556X = arg0K0;
  slow_557X = arg0K1;
  move_slowP_558X = arg2K2;
  if ((25 == list_556X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    if ((3 == (3 & list_556X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_556X))) + -4))))>>2))))) {
        head_559X = *((long *) (((char *) (-3 + list_556X))));
        if ((3 == (3 & head_559X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_559X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_559X))))) == arg2_369X)) {
              SvalS = head_559X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L14545;}
            else {
              list_560X = *((long *) ((((char *) (-3 + list_556X))) + 4));
              if ((list_560X == slow_557X)) {
                merged_arg0K0 = 5;
                merged_arg0K1 = 1;
                push_exception_continuationB_return_tag = 142;
                goto push_exception_continuationB;
               push_exception_continuationB_return_142:
                *((long *) (SstackS)) = arg2_369X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_370X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L12443;}
              else {
                if (move_slowP_558X) {
                  arg0K0 = list_560X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_557X))) + 4)));
                  arg2K2 = 0;
                  goto L23474;}
                else {
                  arg0K0 = list_560X;
                  arg0K1 = slow_557X;
                  arg2K2 = 1;
                  goto L23474;}}}}
          else {
            merged_arg0K0 = 5;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 143;
            goto push_exception_continuationB;
           push_exception_continuationB_return_143:
            *((long *) (SstackS)) = arg2_369X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_370X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L12443;}}
        else {
          merged_arg0K0 = 5;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 144;
          goto push_exception_continuationB;
         push_exception_continuationB_return_144:
          *((long *) (SstackS)) = arg2_369X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_370X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L12443;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 145;
        goto push_exception_continuationB;
       push_exception_continuationB_return_145:
        *((long *) (SstackS)) = arg2_369X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_370X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
    else {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 146;
      goto push_exception_continuationB;
     push_exception_continuationB_return_146:
      *((long *) (SstackS)) = arg2_369X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_370X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L12443;}}}
 L20192: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 147;
  goto push_exception_continuationB;
 push_exception_continuationB_return_147:
  *((long *) (SstackS)) = arg3_372X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_371X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_373X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L20239: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 148;
  goto push_exception_continuationB;
 push_exception_continuationB_return_148:
  *((long *) (SstackS)) = arg3_372X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_371X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_373X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L26482: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 149;
  goto push_exception_continuationB;
 push_exception_continuationB_return_149:
  *((long *) (SstackS)) = arg4_377X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_376X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_379X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_378X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L12443;}
 L26539: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 150;
  goto push_exception_continuationB;
 push_exception_continuationB_return_150:
  *((long *) (SstackS)) = arg4_377X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_376X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_379X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_378X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L12443;}
 L25846: {
  if ((3 == (3 & arg5_385X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_385X))) + -4))))>>2))))) {
      goto L25859;}
    else {
      goto L25854;}}
  else {
    goto L25854;}}
 L25841: {
  if ((3 == (3 & arg5_385X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg5_385X))) + -4))))>>2))))) {
      goto L25846;}
    else {
      goto L25913;}}
  else {
    goto L25913;}}
 L17713: {
  port_561X = arg0K0;
  if ((3 == (3 & port_561X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_561X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_561X))) + 4))))>>2))))) {
        goto L17773;}
      else {
        p_562X = *((long *) ((((char *) (-3 + port_561X))) + 24));
        p_563X = *((long *) ((((char *) (-3 + port_561X))) + 28));
        b_564X = *((long *) ((((char *) (-3 + port_561X))) + 20));
        i_565X = ((p_562X)>>2);
        x_566X = *((long *) ((((char *) (-3 + port_561X))) + 12));
        if ((5 == x_566X)) {
          goto L17753;}
        else {
          if ((i_565X == (((p_563X)>>2)))) {
            goto L17753;}
          else {
            val_567X = 4 + (((i_565X)<<2));
            addr_568X = (((char *) (-3 + port_561X))) + 24;
            WRITE_BARRIER(addr_568X, val_567X);
            *((long *) addr_568X) = val_567X;
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_564X))) + i_565X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L14545;}}}}
    else {
      goto L17773;}}
  else {
    goto L17773;}}
 L17510: {
  port_569X = arg0K0;
  if ((3 == (3 & port_569X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_569X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_569X))) + 4))))>>2))))) {
        goto L17570;}
      else {
        p_570X = *((long *) ((((char *) (-3 + port_569X))) + 24));
        p_571X = *((long *) ((((char *) (-3 + port_569X))) + 28));
        b_572X = *((long *) ((((char *) (-3 + port_569X))) + 20));
        i_573X = ((p_570X)>>2);
        x_574X = *((long *) ((((char *) (-3 + port_569X))) + 12));
        if ((5 == x_574X)) {
          goto L17550;}
        else {
          if ((i_573X == (((p_571X)>>2)))) {
            goto L17550;}
          else {
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_572X))) + i_573X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L14545;}}}}
    else {
      goto L17570;}}
  else {
    goto L17570;}}
 L17257: {
  Kchar_575X = arg0K0;
  port_576X = arg0K1;
  if ((9 == (255 & Kchar_575X))) {
    if ((3 == (3 & port_576X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_576X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_576X))) + 4))))>>2))))) {
          goto L17318;}
        else {
          p_577X = *((long *) ((((char *) (-3 + port_576X))) + 24));
          b_578X = *((long *) ((((char *) (-3 + port_576X))) + 20));
          i_579X = ((p_577X)>>2);
          x_580X = *((long *) ((((char *) (-3 + port_576X))) + 12));
          if ((5 == x_580X)) {
            goto L17300;}
          else {
            if ((i_579X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_578X))) + -4))))>>8)))) {
              goto L17300;}
            else {
              val_581X = 4 + (((i_579X)<<2));
              addr_582X = (((char *) (-3 + port_576X))) + 24;
              WRITE_BARRIER(addr_582X, val_581X);
              *((long *) addr_582X) = val_581X;
              *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_576X))) + 20)))))) + i_579X)) = (((((Kchar_575X)>>8))));
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L14545;}}}}
      else {
        goto L17318;}}
    else {
      goto L17318;}}
  else {
    goto L17318;}}
 L25126: {
  stuff_583X = arg0K0;
  if ((3 == (3 & stuff_583X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_583X))) + -4))))>>2))))) {
      thing_584X = *((long *) (((char *) (-3 + stuff_583X))));
      if ((0 == (3 & thing_584X))) {
        ps_write_integer((((thing_584X)>>2)), out_396X);
        goto L25133;}
      else {
        if ((9 == (255 & thing_584X))) {
          ps_write_string("#\\", out_396X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_584X)>>8))), out_396X, ignoreXX) }
          goto L25133;}
        else {
          if ((3 == (3 & thing_584X))) {
            if ((8 == (31 & ((((*((long *) ((((char *) (-3 + thing_584X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_584X))) + -4))))>>8))))>>2)))) {
                type_585X = *((long *) (((char *) (-3 + thing_584X))));
                if ((3 == (3 & type_585X))) {
                  if ((8 == (31 & ((((*((long *) ((((char *) (-3 + type_585X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_585X))) + -4))))>>8))))>>2)))) {
                      obj_586X = *((long *) ((((char *) (-3 + type_585X))) + 8));
                      if ((3 == (3 & obj_586X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_586X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_396X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_584X)))))))) + 8))))))))))))), out_396X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_396X, ignoreXX) }
                          goto L25133;}
                        else {
                          goto L9130;}}
                      else {
                        goto L9130;}}
                    else {
                      goto L9130;}}
                  else {
                    goto L9130;}}
                else {
                  goto L9130;}}
              else {
                goto L9130;}}
            else {
              goto L9130;}}
          else {
            goto L9130;}}}}
    else {
      goto L25141;}}
  else {
    goto L25141;}}
 L12502: {
  SvalS = (*((long *) ((((char *) (-3 + (Sexception_handlersS)))) + (((opcode_398X)<<2)))));
  obj_587X = SvalS;
  if ((3 == (3 & obj_587X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_587X))) + -4))))>>2))))) {
      goto L12519;}
    else {
      goto L12589;}}
  else {
    goto L12589;}}
 L12575: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L12502;}
 L17076: {
  okayP_588X = arg2K0;
  key_589X = arg0K1;
  if (okayP_588X) {
    arg0K0 = key_589X;
    goto L17044;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_589X;
    goto L17044;}}
 L16910: {
  key_590X = arg0K0;
  len_591X = ((count_87X)<<2);
  addr_592X = ALLOCATE_SPACE(2, (4 + len_591X));
  *((long *) addr_592X) = (10 + (((len_591X)<<8)));
  new_env_593X = 3 + (((long) (addr_592X + 4)));
  arg0K0 = 0;
  goto L16920;}
 L16825: {
  key_594X = arg0K0;
  if ((1 == (ScontS))) {
    arg0K0 = 1;
    goto L16829;}
  else {
    merged_arg0K0 = key_594X;
    merged_arg0K1 = 2;
    really_preserve_continuation_return_tag = 0;
    goto really_preserve_continuation;
   really_preserve_continuation_return_0:
    v_595X = really_preserve_continuation0_return_value;
    arg0K0 = v_595X;
    goto L16829;}}
 L15927: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 151;
  goto push_exception_continuationB;
 push_exception_continuationB_return_151:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_94X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L15268: {
  v_596X = arg0K0;
  merged_arg0K0 = v_596X;
  copy_stack_into_heap_return_tag = 0;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_0:
  if (((SstackS) < (Sstack_limitS))) {
    ps_error("Couldn't get default procedure space (how can this happen?)", 0);
    goto L15225;}
  else {
    goto L15225;}}
 L12672: {
  stack_arg_count_597X = arg0K0;
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (StemplateS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  tem_598X = Sinterrupt_templateS;
  StemplateS = tem_598X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_598X))))))));
  merged_arg1K0 = (Scode_pointerS);
  merged_arg0K1 = (4 + stack_arg_count_597X);
  push_continuationB_return_tag = 2;
  goto push_continuationB;
 push_continuationB_return_2:
  n_599X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L12777;}
 L15229: {
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L11858: {
  protocol_600X = arg0K0;
  stack_space_601X = arg0K1;
  if ((68 == protocol_600X)) {
    if ((stack_arg_count_419X < 3)) {
      skip_602X = *((unsigned char *) ((((char *) (-3 + code_420X))) + (2 + stack_arg_count_419X)));
      if ((0 == skip_602X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_419X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L12939;}
      else {
        arg0K0 = skip_602X;
        arg0K1 = stack_arg_count_419X;
        goto L11862;}}
    else {
      skip_603X = *((unsigned char *) ((((char *) (-3 + code_420X))) + 5));
      if ((0 == skip_603X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_419X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L12939;}
      else {
        arg0K0 = skip_603X;
        goto L11885;}}}
  else {
    if ((67 == protocol_600X)) {
      if ((stack_arg_count_419X < (*((unsigned char *) ((((char *) (-3 + code_420X))) + 2))))) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_419X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L12939;}
      else {
        arg0K0 = 3;
        goto L11885;}}
    else {
      if ((65 == protocol_600X)) {
        wants_stack_args_604X = ((((*((unsigned char *) ((((char *) (-3 + code_420X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_420X))) + 3)));
        if ((stack_arg_count_419X < wants_stack_args_604X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_419X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L12939;}
        else {
          merged_arg0K0 = wants_stack_args_604X;
          merged_arg0K1 = stack_arg_count_419X;
          merged_arg0K2 = 25;
          merged_arg0K3 = 0;
          rest_list_setup_return_tag = 0;
          goto rest_list_setup;
         rest_list_setup_return_0:
          arg0K0 = 4;
          arg0K1 = (1 + wants_stack_args_604X);
          goto L11862;}}
      else {
        if ((63 < protocol_600X)) {
          if ((64 == protocol_600X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_420X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_420X))) + 3)))) == stack_arg_count_419X)) {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_419X;
              goto L11862;}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_419X;
              arg0K2 = 25;
              arg0K3 = 0;
              goto L12939;}}
          else {
            if ((66 == protocol_600X)) {
              length_605X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_420X))) + -4))))>>8);
              index_606X = -2 + length_605X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_420X))) + (-3 + length_605X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_420X))) + index_606X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_420X))) + (1 + index_606X)))));
              goto L11858;}
            else {
              ps_error("unknown protocol", 1, protocol_600X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_419X;
              arg0K2 = 25;
              arg0K3 = 0;
              goto L12939;}}}
        else {
          if ((protocol_600X == stack_arg_count_419X)) {
            arg0K0 = 2;
            arg0K1 = stack_arg_count_419X;
            goto L11862;}
          else {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_419X;
            arg0K2 = 25;
            arg0K3 = 0;
            goto L12939;}}}}}}
 L12135: {
  protocol_607X = arg0K0;
  stack_space_608X = arg0K1;
  if ((68 == protocol_607X)) {
    if ((total_arg_count_438X < 3)) {
      skip_609X = *((unsigned char *) ((((char *) (-3 + code_437X))) + (2 + total_arg_count_438X)));
      if ((0 == skip_609X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_432X;
        arg0K2 = list_args_433X;
        arg0K3 = list_arg_count_434X;
        goto L12939;}
      else {
        merged_arg0K0 = list_args_433X;
        merged_arg0K1 = list_arg_count_434X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = skip_609X;
        arg0K1 = total_arg_count_438X;
        goto L12139;}}
    else {
      skip_610X = *((unsigned char *) ((((char *) (-3 + code_437X))) + 5));
      if ((0 == skip_610X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_432X;
        arg0K2 = list_args_433X;
        arg0K3 = list_arg_count_434X;
        goto L12939;}
      else {
        arg0K0 = skip_610X;
        goto L12164;}}}
  else {
    if ((67 == protocol_607X)) {
      if ((total_arg_count_438X < (*((unsigned char *) ((((char *) (-3 + code_437X))) + 2))))) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_432X;
        arg0K2 = list_args_433X;
        arg0K3 = list_arg_count_434X;
        goto L12939;}
      else {
        arg0K0 = 3;
        goto L12164;}}
    else {
      if ((63 < protocol_607X)) {
        if ((65 == protocol_607X)) {
          wants_stack_args_611X = ((((*((unsigned char *) ((((char *) (-3 + code_437X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_437X))) + 3)));
          if ((total_arg_count_438X < wants_stack_args_611X)) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_432X;
            arg0K2 = list_args_433X;
            arg0K3 = list_arg_count_434X;
            goto L12939;}
          else {
            merged_arg0K0 = wants_stack_args_611X;
            merged_arg0K1 = stack_arg_count_432X;
            merged_arg0K2 = list_args_433X;
            merged_arg0K3 = list_arg_count_434X;
            rest_list_setup_return_tag = 1;
            goto rest_list_setup;
           rest_list_setup_return_1:
            arg0K0 = 4;
            arg0K1 = (1 + wants_stack_args_611X);
            goto L12139;}}
        else {
          if ((64 == protocol_607X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_437X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_437X))) + 3)))) == total_arg_count_438X)) {
              merged_arg0K0 = list_args_433X;
              merged_arg0K1 = list_arg_count_434X;
              push_list_return_tag = 1;
              goto push_list;
             push_list_return_1:
              arg0K0 = 4;
              arg0K1 = total_arg_count_438X;
              goto L12139;}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_432X;
              arg0K2 = list_args_433X;
              arg0K3 = list_arg_count_434X;
              goto L12939;}}
          else {
            if ((66 == protocol_607X)) {
              length_612X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_437X))) + -4))))>>8);
              index_613X = -2 + length_612X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_437X))) + (-3 + length_612X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_437X))) + index_613X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_437X))) + (1 + index_613X)))));
              goto L12135;}
            else {
              ps_error("unknown protocol", 1, protocol_607X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_432X;
              arg0K2 = list_args_433X;
              arg0K3 = list_arg_count_434X;
              goto L12939;}}}}
      else {
        if ((protocol_607X == total_arg_count_438X)) {
          merged_arg0K0 = list_args_433X;
          merged_arg0K1 = list_arg_count_434X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          arg0K0 = 2;
          arg0K1 = total_arg_count_438X;
          goto L12139;}
        else {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_432X;
          arg0K2 = list_args_433X;
          arg0K3 = list_arg_count_434X;
          goto L12939;}}}}}
 L16490: {
  cont_614X = arg0K0;
  if ((3 == (3 & cont_614X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_614X))) + -4))))>>2))))) {
      next_op_615X = *((unsigned char *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_614X))) + 8))))))))))) + ((((*((long *) ((((char *) (-3 + cont_614X))) + 4))))>>2))));
      if ((34 == next_op_615X)) {
        pop_continuationB_return_tag = 2;
        goto pop_continuationB;
       pop_continuationB_return_2:
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        if ((30 == next_op_615X)) {
          next_616X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_616X)))));
            goto L15386;}
          else {
            ScontS = next_616X;
            goto L15386;}}
        else {
          merged_arg0K0 = list_args_450X;
          merged_arg0K1 = stack_nargs_449X;
          pop_args_GlistS_return_tag = 3;
          goto pop_args_GlistS;
         pop_args_GlistS_return_3:
          args_617X = pop_args_GlistS0_return_value;
          merged_arg0K0 = 4;
          merged_arg0K1 = 0;
          push_exception_continuationB_return_tag = 152;
          goto push_exception_continuationB;
         push_exception_continuationB_return_152:
          *((long *) (SstackS)) = 1;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = args_617X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L12443;}}}
    else {
      goto L16496;}}
  else {
    goto L16496;}}
 L15039: {
  v_618X = arg0K0;
  merged_arg0K0 = v_618X;
  copy_stack_into_heap_return_tag = 1;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_1:
  if (((SstackS) < (Sstack_limitS))) {
    ps_error("Couldn't get default procedure space (how can this happen?)", 0);
    goto L14924;}
  else {
    goto L14924;}}
 L14933: {
  arg0K0 = (*((unsigned char *) ((Scode_pointerS) + 3)));
  goto L12672;}
 L14936: {
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L15093: {
  v_619X = arg0K0;
  merged_arg0K0 = v_619X;
  copy_stack_into_heap_return_tag = 2;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_2:
  if ((space_126X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L14924;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L14924;}}
 L24862: {
  if ((3 == (3 & x_145X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + x_145X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L24899;}
    else {
      goto L24870;}}
  else {
    goto L24870;}}
 L23287: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L23076: {
  if ((3 == (3 & x_147X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + x_147X))) + -4))))>>2))))) {
      goto L23085;}
    else {
      goto L23088;}}
  else {
    goto L23088;}}
 L22865: {
  if ((3 == (3 & x_148X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + x_148X))) + -4))))>>2))))) {
      goto L22874;}
    else {
      goto L22877;}}
  else {
    goto L22877;}}
 L22654: {
  if ((3 == (3 & x_149X))) {
    if ((18 == (31 & ((((*((long *) ((((char *) (-3 + x_149X))) + -4))))>>2))))) {
      goto L22663;}
    else {
      goto L22666;}}
  else {
    goto L22666;}}
 L5781: {
  b_620X = arg0K0;
  lo_a_621X = 65535 & a_463X;
  lo_b_622X = 65535 & b_620X;
  hi_a_623X = 65535 & (((a_463X)>>16));
  hi_b_624X = 65535 & (((b_620X)>>16));
  lo_c_625X = SMALL_MULTIPLY(lo_a_621X, lo_b_622X);
  v_626X = SMALL_MULTIPLY(lo_b_622X, hi_a_623X);
  v_627X = SMALL_MULTIPLY(lo_a_621X, hi_b_624X);
  mid_c_628X = v_627X + v_626X;
  c_629X = lo_c_625X + (((mid_c_628X)<<16));
  if ((0 < hi_a_623X)) {
    if ((0 < hi_b_624X)) {
      merged_arg0K0 = 6;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 153;
      goto push_exception_continuationB;
     push_exception_continuationB_return_153:
      *((long *) (SstackS)) = arg2_155X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = y_156X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L12443;}
    else {
      goto L5823;}}
  else {
    goto L5823;}}
 L7217: {
  b_630X = arg0K0;
  c_631X = a_464X / b_630X;
  if ((a_164X < 0)) {
    if ((b_165X < 0)) {
      goto L7221;}
    else {
      goto L7220;}}
  else {
    if ((b_165X < 0)) {
      goto L7220;}
    else {
      goto L7221;}}}
 L28481: {
  b_632X = arg0K0;
  c_633X = a_470X / b_632X;
  if ((a_178X < 0)) {
    if ((b_179X < 0)) {
      goto L28485;}
    else {
      goto L28484;}}
  else {
    if ((b_179X < 0)) {
      goto L28484;}
    else {
      goto L28485;}}}
 L28349: {
  b_634X = arg0K0;
  c_635X = a_471X % b_634X;
  if ((a_182X < 0)) {
    arg0K0 = (0 - c_635X);
    goto L28353;}
  else {
    arg0K0 = c_635X;
    goto L28353;}}
 L19514: {
  SvalS = new_482X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L19498: {
  i_636X = arg0K0;
  if ((i_636X < 0)) {
    goto L19514;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_482X))) + (((i_636X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_636X);
    goto L19498;}}
 L19292: {
  i_637X = arg0K0;
  if ((i_637X < 0)) {
    arg0K0 = stack_nargs_487X;
    arg0K1 = rest_list_488X;
    goto L19310;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_486X))) + (((i_637X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_637X);
    goto L19292;}}
 L18675: {
  i_638X = arg0K0;
  if ((i_638X < 0)) {
    SvalS = value_493X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    addr_639X = (((char *) (-3 + value_493X))) + (((i_638X)<<2));
    WRITE_BARRIER(addr_639X, init_490X);
    *((long *) addr_639X) = init_490X;
    arg0K0 = (-1 + i_638X);
    goto L18675;}}
 L22134: {
  i_640X = arg0K0;
  if ((i_640X < 0)) {
    SvalS = vector_495X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_495X))) + i_640X)) = init_259X;
    arg0K0 = (-1 + i_640X);
    goto L22134;}}
 L21895: {
  i_641X = arg0K0;
  if ((i_641X < 0)) {
    SvalS = string_498X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_498X))) + i_641X)) = (init_274X);
    arg0K0 = (-1 + i_641X);
    goto L21895;}}
 L25474: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L21247: {
  key_642X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_643X = *((long *) (SstackS));
  if ((0 == (3 & (SvalS)))) {
    mode_644X = (((SvalS))>>2);
    if ((1 == mode_644X)) {
      goto L21526;}
    else {
      if ((2 == mode_644X)) {
        goto L21526;}
      else {
        if ((3 == mode_644X)) {
          goto L21526;}
        else {
          if ((4 == mode_644X)) {
            goto L21526;}
          else {
            merged_arg0K0 = 5;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 154;
            goto push_exception_continuationB;
           push_exception_continuationB_return_154:
            *((long *) (SstackS)) = arg2_643X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((mode_644X)<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L12443;}}}}}
  else {
    merged_arg0K0 = 5;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 155;
    goto push_exception_continuationB;
   push_exception_continuationB_return_155:
    *((long *) (SstackS)) = arg2_643X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}}
 L27670: {
  key_645X = arg0K0;
  obj_646X = SvalS;
  if ((3 == (3 & obj_646X))) {
    if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_646X))) + -4))))>>2))))) {
      x_647X = SvalS;
      if ((0 == (*((long *) (((char *) (-3 + x_647X))))))) {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 156;
        goto push_exception_continuationB;
       push_exception_continuationB_return_156:
        *((long *) (SstackS)) = x_647X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}
      else {
        merged_arg0K0 = x_647X;
        close_channelB_return_tag = 0;
        goto close_channelB;
       close_channelB_return_0:
        status_648X = close_channelB0_return_value;
        if ((status_648X == NO_ERRORS)) {
          SvalS = 13;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}
        else {
          merged_arg0K0 = 22;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 157;
          goto push_exception_continuationB;
         push_exception_continuationB_return_157:
          *((long *) (SstackS)) = x_647X;
          SstackS = ((SstackS) + -4);
          merged_arg0K0 = status_648X;
          merged_arg0K1 = key_645X;
          get_error_string_return_tag = 0;
          goto get_error_string;
         get_error_string_return_0:
          x_649X = get_error_string0_return_value;
          *((long *) (SstackS)) = x_649X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L12443;}}}
    else {
      goto L27705;}}
  else {
    goto L27705;}}
 L32147: {
  key_650X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_651X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_652X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg4_653X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg5_654X = *((long *) (SstackS));
  if ((0 == (3 & (arg4_653X | arg3_652X)))) {
    if ((1 == arg2_651X)) {
      goto L32179;}
    else {
      if ((5 == arg2_651X)) {
        goto L32179;}
      else {
        goto L32199;}}}
  else {
    goto L32199;}}
 L32368: {
  key_655X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_656X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_657X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg4_658X = *((long *) (SstackS));
  if ((0 == (3 & (arg3_657X | arg2_656X)))) {
    obj_659X = SvalS;
    if ((3 == (3 & obj_659X))) {
      if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_659X))) + -4))))>>2))))) {
        start_660X = ((arg3_657X)>>2);
        count_661X = ((arg2_656X)>>2);
        channel_662X = SvalS;
        v_663X = 8 == (*((long *) (((char *) (-3 + channel_662X)))));
        if (v_663X) {
          if ((3 == (3 & arg4_658X))) {
            if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg4_658X))) + -4))))>>2))))) {
              goto L31925;}
            else {
              goto L31917;}}
          else {
            goto L31917;}}
        else {
          arg0K0 = 5;
          goto L31808;}}
      else {
        goto L32416;}}
    else {
      goto L32416;}}
  else {
    goto L32416;}}
 L6574: {
  val_664X = *((long *) ((((char *) (-3 + ch_512X))) + 12));
  addr_665X = (((char *) (-3 + prev_513X))) + 12;
  WRITE_BARRIER(addr_665X, val_664X);
  *((long *) addr_665X) = val_664X;
  addr_666X = (((char *) (-3 + ch_512X))) + 12;
  WRITE_BARRIER(addr_666X, 1);
  *((long *) addr_666X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_512X))) + 16)));
  goto L21199;}
 L27221: {
  key_667X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_668X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_669X = *((long *) (SstackS));
  if ((3 == (3 & arg3_669X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg3_669X))) + -4))))>>2))))) {
      obj_670X = SvalS;
      if ((3 == (3 & obj_670X))) {
        if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_670X))) + -4))))>>2))))) {
          x_671X = SvalS;
          x_672X = image_writing_okayP();
          if (x_672X) {
            port_673X = ps_open_output_file((((char *)(((char *) (-3 + arg3_669X))))), &status_674X);
            if ((status_674X == NO_ERRORS)) {
              status_675X = ps_write_string((((char *)(((char *) (-3 + x_671X))))), port_673X);
              if ((status_675X == NO_ERRORS)) {
                status_676X = write_image(arg2_668X, port_673X, mark_traced_channels_closedB);
                if ((status_676X == NO_ERRORS)) {
                  status_677X = ps_close(port_673X);
                  if ((status_677X == NO_ERRORS)) {
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L14545;}
                  else {
                    arg0K0 = 22;
                    arg0K1 = status_677X;
                    goto L27261;}}
                else {
                  status_678X = ps_close(port_673X);
                  if ((status_678X == NO_ERRORS)) {
                    arg0K0 = 22;
                    arg0K1 = status_676X;
                    goto L27261;}
                  else {
                    ps_write_string("Unable to close image file", (stderr));
                    { long ignoreXX;
                    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                    arg0K0 = 22;
                    arg0K1 = status_676X;
                    goto L27261;}}}
              else {
                status_679X = ps_close(port_673X);
                if ((status_679X == NO_ERRORS)) {
                  arg0K0 = 22;
                  arg0K1 = status_675X;
                  goto L27261;}
                else {
                  ps_write_string("Unable to close image file", (stderr));
                  { long ignoreXX;
                  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                  arg0K0 = 22;
                  arg0K1 = status_675X;
                  goto L27261;}}}
            else {
              arg0K0 = 10;
              arg0K1 = status_674X;
              goto L27261;}}
          else {
            merged_arg0K0 = 15;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 158;
            goto push_exception_continuationB;
           push_exception_continuationB_return_158:
            *((long *) (SstackS)) = arg3_669X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = arg2_668X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_671X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 3;
            goto L12443;}}
        else {
          goto L27343;}}
      else {
        goto L27343;}}
    else {
      goto L27343;}}
  else {
    goto L27343;}}
 L27075: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 159;
  goto push_exception_continuationB;
 push_exception_continuationB_return_159:
  *((long *) (SstackS)) = arg2_518X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_519X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L25642: {
  table_680X = arg0K0;
  symbols_681X = arg0K1;
  firstP_682X = arg2K2;
  arg0K0 = 0;
  goto L25646;}
 L25221: {
  minutesP_683X = arg2K0;
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_684X = check_events0_return_value;
      if (temp_684X) {
        goto L25236;}
      else {
        goto L25260;}}
    else {
      goto L25236;}}
  else {
    goto L25260;}}
 L13195: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 160;
  goto push_exception_continuationB;
 push_exception_continuationB_return_160:
  *((long *) (SstackS)) = proc_539X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L20676: {
  len_685X = 1 + n_550X;
  addr_686X = ALLOCATE_SPACE(16, (4 + len_685X));
  *((long *) addr_686X) = (66 + (((len_685X)<<8)));
  string_687X = 3 + (((long) (addr_686X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_687X))) + n_550X)) = 0;
  arg0K0 = arg2_549X;
  arg0K1 = (-1 + n_550X);
  goto L20606;}
 L20581: {
  if ((25 == arg2_549X)) {
    goto L20676;}
  else {
    merged_arg0K0 = 5;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 161;
    goto push_exception_continuationB;
   push_exception_continuationB_return_161:
    *((long *) (SstackS)) = arg2_549X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_550X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}}
 L10052: {
  i_688X = arg0K0;
  h_689X = arg0K1;
  if ((i_688X < n_555X)) {
    arg0K0 = (1 + i_688X);
    arg0K1 = (h_689X + (((*((unsigned char *) ((((char *) (-3 + arg2_551X))) + i_688X))))));
    goto L10052;}
  else {
    index_690X = h_689X & (-1 + v_554X);
    bucket_691X = *((long *) ((((char *) (-3 + table_553X))) + (((index_690X)<<2))));
    arg0K0 = bucket_691X;
    goto L10015;}}
 L20430: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 162;
  goto push_exception_continuationB;
 push_exception_continuationB_return_162:
  *((long *) (SstackS)) = arg2_551X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L25859: {
  if ((from_index_386X < 0)) {
    goto L25913;}
  else {
    if ((to_index_387X < 0)) {
      goto L25913;}
    else {
      if ((count_388X < 0)) {
        goto L25913;}
      else {
        if ((3 == (3 & arg5_385X))) {
          if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_385X))) + -4))))>>2))))) {
            arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_385X))) + -4))))>>8)));
            goto L25880;}
          else {
            goto L25876;}}
        else {
          goto L25876;}}}}}
 L25854: {
  if ((3 == (3 & arg5_385X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg5_385X))) + -4))))>>2))))) {
      goto L25859;}
    else {
      goto L25913;}}
  else {
    goto L25913;}}
 L25913: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 163;
  goto push_exception_continuationB;
 push_exception_continuationB_return_163:
  *((long *) (SstackS)) = arg5_385X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_386X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_383X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_387X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_388X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L12443;}
 L17773: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 164;
  goto push_exception_continuationB;
 push_exception_continuationB_return_164:
  *((long *) (SstackS)) = port_561X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L17753: {
  merged_arg0K0 = 14;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 165;
  goto push_exception_continuationB;
 push_exception_continuationB_return_165:
  *((long *) (SstackS)) = port_561X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L17570: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 166;
  goto push_exception_continuationB;
 push_exception_continuationB_return_166:
  *((long *) (SstackS)) = port_569X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L17550: {
  merged_arg0K0 = 14;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 167;
  goto push_exception_continuationB;
 push_exception_continuationB_return_167:
  *((long *) (SstackS)) = port_569X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L17318: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 168;
  goto push_exception_continuationB;
 push_exception_continuationB_return_168:
  *((long *) (SstackS)) = Kchar_575X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_576X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L17300: {
  merged_arg0K0 = 14;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 169;
  goto push_exception_continuationB;
 push_exception_continuationB_return_169:
  *((long *) (SstackS)) = Kchar_575X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_576X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L25133: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_583X))) + 4)));
  goto L25126;}
 L9130: {
  if ((3 == (3 & thing_584X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + thing_584X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_584X)))));
      goto L9172;}
    else {
      goto L9138;}}
  else {
    goto L9138;}}
 L25141: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_396X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L12519: {
  StemplateS = (SvalS);
  Slosing_opcodeS = opcode_398X;
  arg0K0 = (2 + nargs_397X);
  goto L11844;}
 L12589: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L12519;}
 L17044: {
  key_692X = arg0K0;
  p_693X = SenvS;
  if ((3 == (3 & p_693X))) {
    if ((p_693X < (((long) (Sstack_beginS))))) {
      goto L17098;}
    else {
      if (((((long) (Sstack_endS))) < p_693X)) {
        goto L17098;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_692X;
        merged_arg0K3 = 0;
        save_env_in_heap_return_tag = 0;
        goto save_env_in_heap;
       save_env_in_heap_return_0:
        v_694X = save_env_in_heap0_return_value;
        SenvS = v_694X;
        goto L17098;}}}
  else {
    goto L17098;}}
 L16920: {
  i_695X = arg0K0;
  if ((i_695X == count_87X)) {
    index_696X = 1 + (((count_87X)<<1));
    merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + (1 + index_696X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_696X))))))<<2)))));
    merged_arg0K1 = new_env_593X;
    merged_arg0K2 = key_590X;
    make_closure_return_tag = 0;
    goto make_closure;
   make_closure_return_0:
    value_697X = make_closure0_return_value;
    SvalS = value_697X;
    Scode_pointerS = ((Scode_pointerS) + (4 + (((count_87X)<<1))));
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    back_698X = *((unsigned char *) ((Scode_pointerS) + (2 + (((i_695X)<<1)))));
    env_699X = SenvS;
    arg0K0 = env_699X;
    arg0K1 = back_698X;
    goto L17009;}}
 L16829: {
  value_700X = arg0K0;
  SvalS = value_700X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L12777: {
  i_701X = arg0K0;
  m_702X = arg0K1;
  if ((0 == (n_599X & m_702X))) {
    arg0K0 = (1 + i_701X);
    arg0K1 = (((m_702X)<<1));
    goto L12777;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_702X));
    if ((i_701X == 0)) {
      *((long *) (SstackS)) = (Sinterrupted_templateS);
      SstackS = ((SstackS) + -4);
      Sinterrupted_templateS = 1;
      *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L12696;}
    else {
      if ((i_701X == 2)) {
        *((long *) (SstackS)) = (Sfinalize_theseS);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
        SstackS = ((SstackS) + -4);
        Sfinalize_theseS = 25;
        arg0K0 = 2;
        goto L12696;}
      else {
        if ((i_701X == 3)) {
          channel_703X = Spending_channels_headS;
          next_704X = *((long *) ((((char *) (-3 + channel_703X))) + 12));
          Spending_channels_headS = next_704X;
          addr_705X = (((char *) (-3 + channel_703X))) + 12;
          WRITE_BARRIER(addr_705X, 1);
          *((long *) addr_705X) = 1;
          if ((1 == next_704X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_703X;
            goto L6715;}
          else {
            arg0K0 = channel_703X;
            goto L6715;}}
        else {
          *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
          SstackS = ((SstackS) + -4);
          arg0K0 = 1;
          goto L12696;}}}}}
 L11862: {
  skip_706X = arg0K0;
  stack_arg_count_707X = arg0K1;
  template_708X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_708X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_708X)))))))) + skip_706X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  arg0K0 = stack_space_601X;
  arg0K1 = stack_arg_count_707X;
  goto L11753;}
 L11885: {
  skip_709X = arg0K0;
  *((long *) (SstackS)) = 25;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((stack_arg_count_419X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((stack_arg_count_419X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_709X;
  arg0K1 = (3 + stack_arg_count_419X);
  goto L11862;}
 L12139: {
  skip_710X = arg0K0;
  stack_arg_count_711X = arg0K1;
  template_712X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_712X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_712X)))))))) + skip_710X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  arg0K0 = stack_space_608X;
  arg0K1 = stack_arg_count_711X;
  goto L11753;}
 L12164: {
  skip_713X = arg0K0;
  if ((total_arg_count_438X < 3)) {
    arg0K0 = total_arg_count_438X;
    goto L12172;}
  else {
    if ((2 < stack_arg_count_432X)) {
      arg0K0 = stack_arg_count_432X;
      goto L12172;}
    else {
      arg0K0 = 2;
      goto L12172;}}}
 L15386: {
  SvalS = (*((long *) ((((char *) (-3 + cont_614X))) + 16)));
  arg0K0 = stack_nargs_449X;
  arg0K1 = list_args_450X;
  arg0K2 = list_arg_count_451X;
  goto L14347;}
 L16496: {
  merged_arg0K0 = list_args_450X;
  merged_arg0K1 = stack_nargs_449X;
  pop_args_GlistS_return_tag = 4;
  goto pop_args_GlistS;
 pop_args_GlistS_return_4:
  args_714X = pop_args_GlistS0_return_value;
  merged_arg0K0 = 4;
  merged_arg0K1 = 0;
  push_exception_continuationB_return_tag = 170;
  goto push_exception_continuationB;
 push_exception_continuationB_return_170:
  *((long *) (SstackS)) = 1;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = args_714X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L12443;}
 L24870: {
  if ((3 == (3 & x_145X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_145X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L24899;}
    else {
      arg0K0 = 1;
      goto L24899;}}
  else {
    arg0K0 = 1;
    goto L24899;}}
 L23088: {
  if ((3 == (3 & x_147X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_147X))) + -4))))>>2))))) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 171;
      goto push_exception_continuationB;
     push_exception_continuationB_return_171:
      *((long *) (SstackS)) = x_147X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L12443;}
    else {
      goto L23096;}}
  else {
    goto L23096;}}
 L22877: {
  if ((3 == (3 & x_148X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_148X))) + -4))))>>2))))) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 172;
      goto push_exception_continuationB;
     push_exception_continuationB_return_172:
      *((long *) (SstackS)) = x_148X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L12443;}
    else {
      goto L22885;}}
  else {
    goto L22885;}}
 L22666: {
  if ((3 == (3 & x_149X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_149X))) + -4))))>>2))))) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 173;
      goto push_exception_continuationB;
     push_exception_continuationB_return_173:
      *((long *) (SstackS)) = x_149X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L12443;}
    else {
      goto L22674;}}
  else {
    goto L22674;}}
 L5823: {
  if ((536870911 < lo_c_625X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 174;
    goto push_exception_continuationB;
   push_exception_continuationB_return_174:
    *((long *) (SstackS)) = arg2_155X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_156X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}
  else {
    if ((lo_c_625X < 0)) {
      merged_arg0K0 = 6;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 175;
      goto push_exception_continuationB;
     push_exception_continuationB_return_175:
      *((long *) (SstackS)) = arg2_155X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = y_156X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L12443;}
    else {
      if ((8192 < mid_c_628X)) {
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 176;
        goto push_exception_continuationB;
       push_exception_continuationB_return_176:
        *((long *) (SstackS)) = arg2_155X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_156X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}
      else {
        if ((a_157X < 0)) {
          if ((b_158X < 0)) {
            goto L5850;}
          else {
            goto L5857;}}
        else {
          if ((b_158X < 0)) {
            goto L5857;}
          else {
            goto L5850;}}}}}}
 L7221: {
  if ((536870911 < c_631X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 177;
    goto push_exception_continuationB;
   push_exception_continuationB_return_177:
    *((long *) (SstackS)) = arg2_162X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_163X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}
  else {
    arg0K0 = (((c_631X)<<2));
    goto L7195;}}
 L7220: {
  arg0K0 = ((((0 - c_631X))<<2));
  goto L7195;}
 L28485: {
  if ((536870911 < c_633X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 178;
    goto push_exception_continuationB;
   push_exception_continuationB_return_178:
    *((long *) (SstackS)) = arg2_176X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_177X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}
  else {
    SvalS = (((c_633X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L28484: {
  SvalS = ((((0 - c_633X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L28353: {
  n_715X = arg0K0;
  SvalS = (((n_715X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L19310: {
  i_716X = arg0K0;
  rest_list_717X = arg0K1;
  if ((25 == rest_list_717X)) {
    SvalS = new_486X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    *((long *) ((((char *) (-3 + new_486X))) + (((i_716X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_717X)))));
    arg0K0 = (1 + i_716X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_717X))) + 4)));
    goto L19310;}}
 L21526: {
  if ((0 == (3 & arg2_643X))) {
    if (((((arg2_643X)>>2)) < 0)) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 179;
      goto push_exception_continuationB;
     push_exception_continuationB_return_179:
      *((long *) (SstackS)) = arg2_643X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_644X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L12443;}
    else {
      arg0K0 = (((arg2_643X)>>2));
      goto L21305;}}
  else {
    if ((3 == (3 & arg2_643X))) {
      if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_643X))) + -4))))>>2))))) {
        if ((1 == mode_644X)) {
          goto L21363;}
        else {
          if ((3 == mode_644X)) {
            goto L21363;}
          else {
            v_718X = ps_open_fd((((char *)(((char *) (-3 + arg2_643X))))), 0, &v_719X);
            arg0K0 = v_718X;
            arg0K1 = v_719X;
            goto L21376;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 180;
        goto push_exception_continuationB;
       push_exception_continuationB_return_180:
        *((long *) (SstackS)) = arg2_643X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_644X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
    else {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 181;
      goto push_exception_continuationB;
     push_exception_continuationB_return_181:
      *((long *) (SstackS)) = arg2_643X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_644X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L12443;}}}
 L27705: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 182;
  goto push_exception_continuationB;
 push_exception_continuationB_return_182:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L12443;}
 L32179: {
  obj_720X = SvalS;
  if ((3 == (3 & obj_720X))) {
    if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_720X))) + -4))))>>2))))) {
      x_721X = SvalS;
      if ((1 == arg2_651X)) {
        arg2K0 = 0;
        goto L32196;}
      else {
        arg2K0 = 1;
        goto L32196;}}
    else {
      goto L32199;}}
  else {
    goto L32199;}}
 L32199: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 183;
  goto push_exception_continuationB;
 push_exception_continuationB_return_183:
  *((long *) (SstackS)) = arg5_654X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_653X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_652X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_651X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L12443;}
 L31925: {
  if ((3 == (3 & arg4_658X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg4_658X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_658X))) + -4))))>>8)));
      goto L31937;}
    else {
      goto L31933;}}
  else {
    goto L31933;}}
 L31917: {
  if ((3 == (3 & arg4_658X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg4_658X))) + -4))))>>2))))) {
      goto L31925;}
    else {
      arg0K0 = 5;
      goto L31808;}}
  else {
    arg0K0 = 5;
    goto L31808;}}
 L31808: {
  reason_722X = arg0K0;
  merged_arg0K0 = reason_722X;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 184;
  goto push_exception_continuationB;
 push_exception_continuationB_return_184:
  *((long *) (SstackS)) = arg4_658X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_660X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_661X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = channel_662X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L12443;}
 L32416: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 185;
  goto push_exception_continuationB;
 push_exception_continuationB_return_185:
  *((long *) (SstackS)) = arg4_658X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_657X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_656X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L12443;}
 L27261: {
  reason_723X = arg0K0;
  status_724X = arg0K1;
  merged_arg0K0 = reason_723X;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 186;
  goto push_exception_continuationB;
 push_exception_continuationB_return_186:
  *((long *) (SstackS)) = arg3_669X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_668X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_671X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_724X;
  merged_arg0K1 = key_667X;
  get_error_string_return_tag = 1;
  goto get_error_string;
 get_error_string_return_1:
  x_725X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_725X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L12443;}
 L27343: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 187;
  goto push_exception_continuationB;
 push_exception_continuationB_return_187:
  *((long *) (SstackS)) = arg3_669X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_668X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12443;}
 L25646: {
  i_726X = arg0K0;
  if ((i_726X == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_681X))) + -4))))>>8))))>>2)))) {
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    v_727X = AVAILABLEp(3);
    if (v_727X) {
      obj_728X = *((long *) ((((char *) (-3 + symbols_681X))) + (((i_726X)<<2))));
      v_729X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_680X))) + -4))))>>8))))>>2);
      s_730X = *((long *) (((char *) (-3 + obj_728X))));
      n_731X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s_730X))) + -4))))>>8));
      arg0K0 = 0;
      arg0K1 = 0;
      goto L10777;}
    else {
      if (firstP_682X) {
        arg0K0 = 1;
        goto L25667;}
      else {
        merged_arg0K0 = 8;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 188;
        goto push_exception_continuationB;
       push_exception_continuationB_return_188:
        *((long *) (SstackS)) = table_680X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L12443;}}}}
 L25236: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L25260: {
  if ((0 == (Spending_interruptsS))) {
    wait_for_event((((arg2_335X)>>2)), minutesP_683X);
    goto L25236;}
  else {
    goto L25236;}}
 L20606: {
  l_732X = arg0K0;
  i_733X = arg0K1;
  if ((i_733X < 0)) {
    SvalS = string_687X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_687X))) + i_733X)) = ((((((*((long *) (((char *) (-3 + l_732X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_732X))) + 4)));
    arg0K1 = (-1 + i_733X);
    goto L20606;}}
 L10015: {
  b_734X = arg0K0;
  if ((25 == b_734X)) {
    addr_735X = ALLOCATE_SPACE(1, 8);
    *((long *) addr_735X) = 1030;
    x_736X = 3 + (((long) (addr_735X + 4)));
    *((long *) (((char *) (-3 + x_736X)))) = arg2_551X;
    if ((3 == (3 & x_736X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + x_736X))) + -4)))))) {
        *((long *) ((((char *) (-3 + x_736X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_736X))) + -4))));
        arg0K0 = x_736X;
        goto L10069;}
      else {
        arg0K0 = x_736X;
        goto L10069;}}
    else {
      arg0K0 = x_736X;
      goto L10069;}}
  else {
    s2_737X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + b_734X)))))))));
    len_738X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_551X))) + -4))))>>8);
    if ((len_738X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_737X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_737X))), (void *)(((char *) (-3 + arg2_551X))),len_738X)))) {
        arg0K0 = (*((long *) (((char *) (-3 + b_734X)))));
        goto L20429;}
      else {
        goto L10033;}}
    else {
      goto L10033;}}}
 L25880: {
  y_739X = arg0K0;
  if ((y_739X < (from_index_386X + count_388X))) {
    goto L25913;}
  else {
    if ((3 == (3 & arg3_383X))) {
      if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg3_383X))) + -4))))>>2))))) {
        arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_383X))) + -4))))>>8)));
        goto L25895;}
      else {
        goto L25891;}}
    else {
      goto L25891;}}}
 L25876: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_385X))) + -4))))>>8));
  goto L25880;}
 L9172: {
  v_740X = arg3K0;
  ps_write_string(v_740X, out_396X);
  goto L25133;}
 L9138: {
  if ((3 == (3 & thing_584X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_584X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) (((char *) (-3 + thing_584X))))))))));
      goto L9172;}
    else {
      goto L9146;}}
  else {
    goto L9146;}}
 L17098: {
  env_741X = SenvS;
  v_742X = AVAILABLEp(3);
  if (v_742X) {
    arg2K0 = 1;
    arg0K1 = 0;
    arg0K2 = env_741X;
    goto L17117;}
  else {
    merged_arg0K0 = env_741X;
    collect_saving_temp_return_tag = 23;
    goto collect_saving_temp;
   collect_saving_temp_return_23:
    temp_743X = collect_saving_temp0_return_value;
    v_744X = AVAILABLEp(3);
    if (v_744X) {
      arg2K0 = 1;
      arg0K1 = 0;
      arg0K2 = temp_743X;
      goto L17117;}
    else {
      arg2K0 = 0;
      arg0K1 = 0;
      arg0K2 = temp_743X;
      goto L17117;}}}
 L17009: {
  env_745X = arg0K0;
  i_746X = arg0K1;
  if ((0 == i_746X)) {
    value_747X = *((long *) ((((char *) (-3 + env_745X))) + ((((*((unsigned char *) ((Scode_pointerS) + (3 + (((i_695X)<<1)))))))<<2))));
    addr_748X = (((char *) (-3 + new_env_593X))) + (((i_695X)<<2));
    WRITE_BARRIER(addr_748X, value_747X);
    *((long *) addr_748X) = value_747X;
    arg0K0 = (1 + i_695X);
    goto L16920;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_745X)))));
    arg0K1 = (-1 + i_746X);
    goto L17009;}}
 L12696: {
  arg_count_749X = arg0K0;
  obj_750X = Sinterrupt_handlersS;
  if ((3 == (3 & obj_750X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_750X))) + -4))))>>2))))) {
      goto L12710;}
    else {
      goto L12790;}}
  else {
    goto L12790;}}
 L6715: {
  channel_751X = arg0K0;
  x_752X = 1 == (Spending_channels_headS);
  if (x_752X) {
    goto L6729;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        goto L6729;}
      else {
        goto L6729;}}
    else {
      Spending_interruptPS = 1;
      goto L6729;}}}
 L11753: {
  stack_slots_753X = arg0K0;
  stack_arg_count_754X = arg0K1;
  if ((stack_slots_753X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L11755;}
  else {
    space_755X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
    v_756X = AVAILABLEp(space_755X);
    if (v_756X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L11812;}
    else {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 24;
      goto collect_saving_temp;
     collect_saving_temp_return_24:
      v_757X = AVAILABLEp(space_755X);
      if (v_757X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L11812;}
      else {
        arg2K0 = 0;
        arg0K1 = 0;
        goto L11812;}}}}
 L12172: {
  final_stack_arg_count_758X = arg0K0;
  if ((stack_arg_count_432X < final_stack_arg_count_758X)) {
    arg0K0 = final_stack_arg_count_758X;
    goto L12176;}
  else {
    arg0K0 = stack_arg_count_432X;
    goto L12176;}}
 L23096: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L22885: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L22674: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L5850: {
  if ((536870911 < c_629X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 189;
    goto push_exception_continuationB;
   push_exception_continuationB_return_189:
    *((long *) (SstackS)) = arg2_155X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_156X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}
  else {
    SvalS = (((c_629X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L5857: {
  if ((536870912 < c_629X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 190;
    goto push_exception_continuationB;
   push_exception_continuationB_return_190:
    *((long *) (SstackS)) = arg2_155X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_156X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}
  else {
    SvalS = ((((0 - c_629X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L7195: {
  q_759X = arg0K0;
  if ((0 == y_163X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 191;
    goto push_exception_continuationB;
   push_exception_continuationB_return_191:
    *((long *) (SstackS)) = arg2_162X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_163X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}
  else {
    a_760X = ((arg2_162X)>>2);
    if ((a_760X < 0)) {
      arg0K0 = (0 - a_760X);
      goto L7252;}
    else {
      arg0K0 = a_760X;
      goto L7252;}}}
 L21305: {
  index_761X = arg0K0;
  temp_762X = index_761X < (Snumber_of_channelsS);
  if (temp_762X) {
    goto L8325;}
  else {
    x_763X = 1 + index_761X;
    y_764X = 8 + (Snumber_of_channelsS);
    if ((x_763X < y_764X)) {
      arg0K0 = y_764X;
      goto L1969;}
    else {
      arg0K0 = x_763X;
      goto L1969;}}}
 L21363: {
  v_765X = ps_open_fd((((char *)(((char *) (-3 + arg2_643X))))), 1, &v_766X);
  arg0K0 = v_765X;
  arg0K1 = v_766X;
  goto L21376;}
 L21376: {
  channel_767X = arg0K0;
  status_768X = arg0K1;
  if ((status_768X == NO_ERRORS)) {
    arg0K0 = channel_767X;
    goto L21305;}
  else {
    if ((status_768X == ENOENT)) {
      merged_arg0K0 = 10;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 192;
      goto push_exception_continuationB;
     push_exception_continuationB_return_192:
      *((long *) (SstackS)) = arg2_643X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_644X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L12443;}
    else {
      merged_arg0K0 = 22;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 193;
      goto push_exception_continuationB;
     push_exception_continuationB_return_193:
      *((long *) (SstackS)) = arg2_643X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_644X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_768X;
      merged_arg0K1 = key_642X;
      get_error_string_return_tag = 2;
      goto get_error_string;
     get_error_string_return_2:
      x_769X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_769X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 3;
      goto L12443;}}}
 L32196: {
  waitP_770X = arg2K0;
  start_771X = ((arg4_653X)>>2);
  count_772X = ((arg3_652X)>>2);
  if ((4 == (*((long *) (((char *) (-3 + x_721X))))))) {
    if ((3 == (3 & arg5_654X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + arg5_654X))) + -4)))))) {
        if ((3 == (3 & arg5_654X))) {
          if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_654X))) + -4))))>>2))))) {
            goto L31498;}
          else {
            goto L31490;}}
        else {
          goto L31490;}}
      else {
        arg0K0 = 5;
        goto L31381;}}
    else {
      arg0K0 = 5;
      goto L31381;}}
  else {
    arg0K0 = 5;
    goto L31381;}}
 L31937: {
  length_773X = arg0K0;
  if ((length_773X < (start_660X + count_661X))) {
    arg0K0 = 7;
    goto L31808;}
  else {
    got_774X = ps_write_fd(((((*((long *) ((((char *) (-3 + channel_662X))) + 8))))>>2)), ((((char *) (-3 + arg4_658X))) + start_660X), count_661X, &pendingP_775X, &status_776X);
    if ((status_776X == NO_ERRORS)) {
      if (pendingP_775X) {
        Spending_interruptPS = 0;
        Senabled_interruptsS = 0;
        arg0K0 = 13;
        goto L31808;}
      else {
        SvalS = (((got_774X)<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}}
    else {
      merged_arg0K0 = 22;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 194;
      goto push_exception_continuationB;
     push_exception_continuationB_return_194:
      *((long *) (SstackS)) = arg4_658X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_660X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_661X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = channel_662X;
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_776X;
      merged_arg0K1 = key_655X;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_777X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_777X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 5;
      goto L12443;}}}
 L31933: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_658X))) + -4))))>>8));
  goto L31937;}
 L10777: {
  i_778X = arg0K0;
  h_779X = arg0K1;
  if ((i_778X < n_731X)) {
    arg0K0 = (1 + i_778X);
    arg0K1 = (h_779X + (((*((unsigned char *) ((((char *) (-3 + s_730X))) + i_778X))))));
    goto L10777;}
  else {
    index_780X = h_779X & (-1 + v_729X);
    bucket_781X = *((long *) ((((char *) (-3 + table_680X))) + (((index_780X)<<2))));
    arg0K0 = bucket_781X;
    goto L10738;}}
 L25667: {
  i_782X = arg0K0;
  if ((i_782X == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_680X))) + -4))))>>8))))>>2)))) {
    addr_783X = ((char *) (-3 + table_680X));
    WRITE_BARRIER(addr_783X, symbols_681X);
    *((long *) addr_783X) = symbols_681X;
    merged_arg0K0 = table_680X;
    collect_saving_temp_return_tag = 25;
    goto collect_saving_temp;
   collect_saving_temp_return_25:
    table_784X = collect_saving_temp0_return_value;
    symbols_785X = *((long *) (((char *) (-3 + table_784X))));
    addr_786X = ((char *) (-3 + table_784X));
    WRITE_BARRIER(addr_786X, 25);
    *((long *) addr_786X) = 25;
    arg0K0 = table_784X;
    arg0K1 = symbols_785X;
    arg2K2 = 0;
    goto L25642;}
  else {
    addr_787X = (((char *) (-3 + table_680X))) + (((i_782X)<<2));
    WRITE_BARRIER(addr_787X, 25);
    *((long *) addr_787X) = 25;
    arg0K0 = (1 + i_782X);
    goto L25667;}}
 L10069: {
  new_788X = arg0K0;
  addr_789X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_789X) = 2050;
  x_790X = 3 + (((long) (addr_789X + 4)));
  *((long *) (((char *) (-3 + x_790X)))) = new_788X;
  *((long *) ((((char *) (-3 + x_790X))) + 4)) = bucket_691X;
  addr_791X = (((char *) (-3 + table_553X))) + (((index_690X)<<2));
  WRITE_BARRIER(addr_791X, x_790X);
  *((long *) addr_791X) = x_790X;
  arg0K0 = new_788X;
  goto L20429;}
 L20429: {
  val_792X = arg0K0;
  SvalS = val_792X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L10033: {
  arg0K0 = (*((long *) ((((char *) (-3 + b_734X))) + 4)));
  goto L10015;}
 L25895: {
  y_793X = arg0K0;
  if ((y_793X < (to_index_387X + count_388X))) {
    goto L25913;}
  else {
    memcpy((void *)((((char *) (-3 + arg3_383X))) + to_index_387X), (void *)((((char *) (-3 + arg5_385X))) + from_index_386X),count_388X);
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L25891: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_383X))) + -4))))>>8));
  goto L25895;}
 L9146: {
  if ((1 == thing_584X)) {
    goto L9149;}
  else {
    if ((5 == thing_584X)) {
      goto L9149;}
    else {
      if ((25 == thing_584X)) {
        arg3K0 = "()";
        goto L9172;}
      else {
        if ((3 == (3 & thing_584X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_584X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L9172;}
          else {
            goto L9162;}}
        else {
          goto L9162;}}}}}
 L17117: {
  okayP_794X = arg2K0;
  key_795X = arg0K1;
  temp_796X = arg0K2;
  if (okayP_794X) {
    arg0K0 = key_795X;
    arg0K1 = temp_796X;
    goto L17052;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_795X;
    arg0K1 = temp_796X;
    goto L17052;}}
 L12710: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    Spending_interruptPS = 0;
    if ((Spending_eventsPS)) {
      Spending_interruptPS = 1;
      goto L12712;}
    else {
      goto L12712;}}
  else {
    Spending_interruptPS = 1;
    goto L12712;}}
 L12790: {
  ps_error("interrupt handler is not a vector", 0);
  goto L12710;}
 L6729: {
  *((long *) (SstackS)) = channel_751X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + channel_751X))) + 16)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L12696;}
 L11755: {
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 3;
      goto check_events;
     check_events_return_3:
      v_797X = check_events0_return_value;
      if (v_797X) {
        arg0K0 = stack_arg_count_754X;
        goto L12672;}
      else {
        goto L11759;}}
    else {
      arg0K0 = stack_arg_count_754X;
      goto L12672;}}
  else {
    goto L11759;}}
 L11812: {
  okayP_798X = arg2K0;
  key_799X = arg0K1;
  if (okayP_798X) {
    arg0K0 = key_799X;
    goto L11766;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_799X;
    goto L11766;}}
 L12176: {
  v_800X = arg0K0;
  merged_arg0K0 = v_800X;
  merged_arg0K1 = stack_arg_count_432X;
  merged_arg0K2 = list_args_433X;
  merged_arg0K3 = list_arg_count_434X;
  rest_list_setup_return_tag = 2;
  goto rest_list_setup;
 rest_list_setup_return_2:
  *((long *) (SstackS)) = (((final_stack_arg_count_758X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_438X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_713X;
  arg0K1 = (3 + final_stack_arg_count_758X);
  goto L12139;}
 L7252: {
  a_801X = arg0K0;
  n_802X = ((y_163X)>>2);
  if ((n_802X < 0)) {
    arg0K0 = (0 - n_802X);
    goto L7254;}
  else {
    arg0K0 = n_802X;
    goto L7254;}}
 L8325: {
  if ((1 == (*((Svm_channelsS) + index_761X)))) {
    addr_803X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_803X) = 5142;
    x_804X = 3 + (((long) (addr_803X + 4)));
    *((long *) (((char *) (-3 + x_804X)))) = (((mode_644X)<<2));
    *((long *) ((((char *) (-3 + x_804X))) + 4)) = arg2_643X;
    *((long *) ((((char *) (-3 + x_804X))) + 8)) = (((index_761X)<<2));
    *((long *) ((((char *) (-3 + x_804X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_804X))) + 16)) = 1;
    *((Svm_channelsS) + index_761X) = x_804X;
    arg0K0 = x_804X;
    arg0K1 = 9;
    goto L21309;}
  else {
    arg0K0 = 1;
    arg0K1 = 11;
    goto L21309;}}
 L1969: {
  new_count_805X = arg0K0;
  new_vm_channels_806X = (long*)malloc(sizeof(long) * new_count_805X);
  if ((NULL == new_vm_channels_806X)) {
    arg0K0 = 1;
    arg0K1 = 9;
    goto L21309;}
  else {
    arg0K0 = 0;
    goto L1981;}}
 L31498: {
  if ((3 == (3 & arg5_654X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg5_654X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_654X))) + -4))))>>8)));
      goto L31510;}
    else {
      goto L31506;}}
  else {
    goto L31506;}}
 L31490: {
  if ((3 == (3 & arg5_654X))) {
    if ((17 == (31 & ((((*((long *) ((((char *) (-3 + arg5_654X))) + -4))))>>2))))) {
      goto L31498;}
    else {
      arg0K0 = 5;
      goto L31381;}}
  else {
    arg0K0 = 5;
    goto L31381;}}
 L31381: {
  reason_807X = arg0K0;
  merged_arg0K0 = reason_807X;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 195;
  goto push_exception_continuationB;
 push_exception_continuationB_return_195:
  *((long *) (SstackS)) = arg5_654X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_771X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_772X)<<2));
  SstackS = ((SstackS) + -4);
  if (waitP_770X) {
    arg0K0 = 5;
    goto L31396;}
  else {
    arg0K0 = 1;
    goto L31396;}}
 L10738: {
  b_808X = arg0K0;
  if ((25 == b_808X)) {
    addr_809X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_809X) = 2050;
    x_810X = 3 + (((long) (addr_809X + 4)));
    *((long *) (((char *) (-3 + x_810X)))) = obj_728X;
    *((long *) ((((char *) (-3 + x_810X))) + 4)) = bucket_781X;
    addr_811X = (((char *) (-3 + table_680X))) + (((index_780X)<<2));
    WRITE_BARRIER(addr_811X, x_810X);
    *((long *) addr_811X) = x_810X;
    goto L25661;}
  else {
    if ((obj_728X == (*((long *) (((char *) (-3 + b_808X))))))) {
      goto L25661;}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + b_808X))) + 4)));
      goto L10738;}}}
 L9149: {
  if ((1 == thing_584X)) {
    arg3K0 = "#f";
    goto L9172;}
  else {
    arg3K0 = "#t";
    goto L9172;}}
 L9162: {
  if ((3 == (3 & thing_584X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_584X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L9172;}
    else {
      goto L9166;}}
  else {
    goto L9166;}}
 L17052: {
  key_812X = arg0K0;
  env_813X = arg0K1;
  merged_arg0K0 = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  merged_arg0K1 = env_813X;
  merged_arg0K2 = key_812X;
  make_closure_return_tag = 1;
  goto make_closure;
 make_closure_return_1:
  value_814X = make_closure0_return_value;
  SvalS = value_814X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L12712: {
  SvalS = (*((long *) ((((char *) (-3 + (Sinterrupt_handlersS)))) + (((i_701X)<<2)))));
  obj_815X = SvalS;
  if ((3 == (3 & obj_815X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_815X))) + -4))))>>2))))) {
      goto L12729;}
    else {
      goto L12815;}}
  else {
    goto L12815;}}
 L11759: {
  arg1K0 = (Scode_pointerS);
  goto L14545;}
 L11766: {
  v_816X = arg0K0;
  merged_arg0K0 = v_816X;
  copy_stack_into_heap_return_tag = 3;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_3:
  if ((stack_slots_753X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L11755;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L11755;}}
 L7254: {
  b_817X = arg0K0;
  c_818X = a_801X % b_817X;
  if ((a_760X < 0)) {
    arg0K0 = (0 - c_818X);
    goto L7258;}
  else {
    arg0K0 = c_818X;
    goto L7258;}}
 L21309: {
  channel_819X = arg0K0;
  reason_820X = arg0K1;
  if ((1 == channel_819X)) {
    if ((3 == (3 & arg2_643X))) {
      if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg2_643X))) + -4))))>>2))))) {
        if ((1 == mode_644X)) {
          goto L21461;}
        else {
          if ((3 == mode_644X)) {
            goto L21461;}
          else {
            v_821X = ps_close_fd(index_761X);
            arg0K0 = v_821X;
            goto L21456;}}}
      else {
        merged_arg0K0 = reason_820X;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 196;
        goto push_exception_continuationB;
       push_exception_continuationB_return_196:
        *((long *) (SstackS)) = arg2_643X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_644X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L12443;}}
    else {
      merged_arg0K0 = reason_820X;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 197;
      goto push_exception_continuationB;
     push_exception_continuationB_return_197:
      *((long *) (SstackS)) = arg2_643X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_644X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L12443;}}
  else {
    SvalS = channel_819X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}}
 L1981: {
  i_822X = arg0K0;
  if ((i_822X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L1996;}
  else {
    *(new_vm_channels_806X + i_822X) = (*((Svm_channelsS) + i_822X));
    arg0K0 = (1 + i_822X);
    goto L1981;}}
 L31510: {
  length_823X = arg0K0;
  if ((length_823X < (start_771X + count_772X))) {
    arg0K0 = 7;
    goto L31381;}
  else {
    got_824X = ps_read_fd(((((*((long *) ((((char *) (-3 + x_721X))) + 8))))>>2)), ((((char *) (-3 + arg5_654X))) + start_771X), count_772X, waitP_770X, &eofP_825X, &pendingP_826X, &status_827X);
    if ((status_827X == NO_ERRORS)) {
      if (eofP_825X) {
        SvalS = 21;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L14545;}
      else {
        if (pendingP_826X) {
          Spending_interruptPS = 0;
          Senabled_interruptsS = 0;
          arg0K0 = 13;
          goto L31381;}
        else {
          SvalS = (((got_824X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L14545;}}}
    else {
      merged_arg0K0 = 22;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 198;
      goto push_exception_continuationB;
     push_exception_continuationB_return_198:
      *((long *) (SstackS)) = arg5_654X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_771X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_772X)<<2));
      SstackS = ((SstackS) + -4);
      if (waitP_770X) {
        arg0K0 = 5;
        goto L31433;}
      else {
        arg0K0 = 1;
        goto L31433;}}}}
 L31506: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_654X))) + -4))))>>8));
  goto L31510;}
 L31396: {
  x_828X = arg0K0;
  *((long *) (SstackS)) = x_828X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_721X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L12443;}
 L25661: {
  arg0K0 = (1 + i_726X);
  goto L25646;}
 L9166: {
  if ((3 == (3 & thing_584X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_584X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L9172;}
    else {
      arg3K0 = "???";
      goto L9172;}}
  else {
    arg3K0 = "???";
    goto L9172;}}
 L12729: {
  StemplateS = (SvalS);
  Slosing_opcodeS = (0 - i_701X);
  arg0K0 = arg_count_749X;
  goto L11844;}
 L12815: {
  ps_error("interrupt handler is not a closure", 1, i_701X);
  goto L12729;}
 L7258: {
  n_829X = arg0K0;
  if ((0 == (((n_829X)<<2)))) {
    SvalS = q_759X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L14545;}
  else {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 199;
    goto push_exception_continuationB;
   push_exception_continuationB_return_199:
    *((long *) (SstackS)) = arg2_162X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_163X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}}
 L21461: {
  v_830X = ps_close_fd(index_761X);
  arg0K0 = v_830X;
  goto L21456;}
 L21456: {
  status_831X = arg0K0;
  if ((status_831X == NO_ERRORS)) {
    merged_arg0K0 = reason_820X;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 200;
    goto push_exception_continuationB;
   push_exception_continuationB_return_200:
    *((long *) (SstackS)) = arg2_643X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_644X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}
  else {
    merged_arg0K0 = status_831X;
    merged_arg0K1 = index_761X;
    merged_arg0K2 = arg2_643X;
    channel_close_error_return_tag = 0;
    goto channel_close_error;
   channel_close_error_return_0:
    merged_arg0K0 = reason_820X;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 201;
    goto push_exception_continuationB;
   push_exception_continuationB_return_201:
    *((long *) (SstackS)) = arg2_643X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_644X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L12443;}}
 L1996: {
  i_832X = arg0K0;
  if ((i_832X == new_count_805X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_806X;
    Snumber_of_channelsS = new_count_805X;
    goto L8325;}
  else {
    *(new_vm_channels_806X + i_832X) = 1;
    arg0K0 = (1 + i_832X);
    goto L1996;}}
 L31433: {
  x_833X = arg0K0;
  *((long *) (SstackS)) = x_833X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_721X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_827X;
  merged_arg0K1 = key_650X;
  get_error_string_return_tag = 4;
  goto get_error_string;
 get_error_string_return_4:
  x_834X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_834X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 6;
  goto L12443;}
 loseD0: {
  message_73X = merged_arg3K0;{
  why_835X = (((*((long *) ((SstackS) + (4 + (((nargs_397X)<<2)))))))>>2);
  ps_write_string("Template UIDs: ", (stderr));
  current_template_836X = StemplateS;
  out_837X = stderr;
  if ((0 == (3 & (*((long *) ((((char *) (-3 + current_template_836X))) + 4)))))) {
    if ((current_template_836X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
      arg2K0 = 0;
      goto L8173;}
    else {
      ps_write_integer(((((*((long *) ((((char *) (-3 + current_template_836X))) + 4))))>>2)), out_837X);
      arg2K0 = 1;
      goto L8173;}}
  else {
    arg2K0 = 0;
    goto L8173;}}
 L8173: {
  not_firstP_838X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_838X;
  goto L8180;}
 L8180: {
  cont_839X = arg0K0;
  not_firstP_840X = arg2K1;
  if ((3 == (3 & cont_839X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_839X))) + -4))))>>2))))) {
      template_841X = *((long *) ((((char *) (-3 + cont_839X))) + 8));
      if ((0 == (3 & (*((long *) ((((char *) (-3 + template_841X))) + 4)))))) {
        if ((template_841X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
          arg2K0 = not_firstP_840X;
          goto L8189;}
        else {
          if (not_firstP_840X) {
            ps_write_string(" <- ", out_837X);
            goto L8228;}
          else {
            goto L8228;}}}
      else {
        arg2K0 = not_firstP_840X;
        goto L8189;}}
    else {
      goto L12462;}}
  else {
    goto L12462;}}
 L8189: {
  v_842X = arg2K0;
  arg0K0 = (*((long *) (((char *) (-3 + cont_839X)))));
  arg2K1 = v_842X;
  goto L8180;}
 L8228: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_841X))) + 4))))>>2)), out_837X);
  arg2K0 = 1;
  goto L8189;}
 L12462: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_835X == 1)) {
    if ((0 == (3 & (*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_397X)<<2))))))))))))) {
      ps_error(message_73X, 3, opcode_398X, why_835X, ((((*((long *) (((char *) (-3 + (*((long *) ((SstackS) + (((nargs_397X)<<2)))))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L12486;}}
  else {
    goto L12486;}}
 L12486: {
  ps_error(message_73X, 2, opcode_398X, why_835X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L35088;}
 L35088: {
  type_843X = get_next_event(&channel_844X, &status_845X);
  if ((type_843X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L11648;}
    else {
      arg0K0 = 1;
      goto L11648;}}
  else {
    if ((type_843X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L11648;}
    else {
      if ((type_843X == IO_COMPLETION_EVENT)) {
        channel_846X = *((Svm_channelsS) + channel_844X);
        val_847X = ((status_845X)<<2);
        addr_848X = (((char *) (-3 + channel_846X))) + 16;
        WRITE_BARRIER(addr_848X, val_847X);
        *((long *) addr_848X) = val_847X;
        if ((1 == (*((long *) ((((char *) (-3 + channel_846X))) + 12))))) {
          if ((1 == (Spending_channels_headS))) {
            Spending_channels_headS = channel_846X;
            Spending_channels_tailS = channel_846X;
            arg0K0 = 8;
            goto L11648;}
          else {
            addr_849X = (((char *) (-3 + (Spending_channels_tailS)))) + 12;
            WRITE_BARRIER(addr_849X, channel_846X);
            *((long *) addr_849X) = channel_846X;
            Spending_channels_tailS = channel_846X;
            arg0K0 = 8;
            goto L11648;}}
        else {
          arg0K0 = 8;
          goto L11648;}}
      else {
        if ((type_843X == NO_EVENT)) {
          arg0K0 = 0;
          goto L11648;}
        else {
          if ((type_843X == ERROR_EVENT)) {
            ps_write_string("OS error while getting event", (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            ps_write_string((ps_error_string(status_845X)), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = 0;
            goto L11648;}
          else {
            ps_write_string("unknown type of event", (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = 0;
            goto L11648;}}}}}}
 L11648: {
  v_850X = arg0K0;
  Spending_interruptsS = (v_850X | (Spending_interruptsS));
  if ((type_843X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        goto L11675;}
      else {
        goto L11675;}}
    else {
      Spending_interruptPS = 1;
      goto L11675;}}
  else {
    goto L35088;}}
 L11675: {
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      goto L35088;}
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
  wants_stack_args_69X = merged_arg0K0;
  stack_arg_count_70X = merged_arg0K1;
  list_args_71X = merged_arg0K2;
  list_arg_count_72X = merged_arg0K3;{
  if ((stack_arg_count_70X == wants_stack_args_69X)) {
    merged_arg0K0 = list_args_71X;
    merged_arg0K1 = list_arg_count_72X;
    copy_listS_return_tag = 1;
    goto copy_listS;
   copy_listS_return_1:
    x_851X = copy_listS0_return_value;
    *((long *) (SstackS)) = x_851X;
    SstackS = ((SstackS) + -4);
    goto rest_list_setup_return;}
  else {
    if ((stack_arg_count_70X < wants_stack_args_69X)) {
      count_852X = wants_stack_args_69X - stack_arg_count_70X;
      merged_arg0K0 = list_args_71X;
      merged_arg0K1 = count_852X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      v_853X = push_list0_return_value;
      merged_arg0K0 = v_853X;
      merged_arg0K1 = (list_arg_count_72X - count_852X);
      copy_listS_return_tag = 2;
      goto copy_listS;
     copy_listS_return_2:
      x_854X = copy_listS0_return_value;
      *((long *) (SstackS)) = x_854X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setup_return;}
    else {
      merged_arg0K0 = list_args_71X;
      merged_arg0K1 = list_arg_count_72X;
      copy_listS_return_tag = 3;
      goto copy_listS;
     copy_listS_return_3:
      v_855X = copy_listS0_return_value;
      merged_arg0K0 = v_855X;
      merged_arg0K1 = (stack_arg_count_70X - wants_stack_args_69X);
      pop_args_GlistS_return_tag = 5;
      goto pop_args_GlistS;
     pop_args_GlistS_return_5:
      x_856X = pop_args_GlistS0_return_value;
      *((long *) (SstackS)) = x_856X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setup_return;}}}
 rest_list_setup_return:
  switch (rest_list_setup_return_tag) {
  case 0: goto rest_list_setup_return_0;
  case 1: goto rest_list_setup_return_1;
  default: goto rest_list_setup_return_2;
  }}

 push_list: {
  list_67X = merged_arg0K0;
  count_68X = merged_arg0K1;{
  *((long *) (SstackS)) = list_67X;
  SstackS = ((SstackS) + -4);
  if ((count_68X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L11481;}
  else {
    space_857X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
    v_858X = AVAILABLEp(space_857X);
    if (v_858X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L11565;}
    else {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 26;
      goto collect_saving_temp;
     collect_saving_temp_return_26:
      v_859X = AVAILABLEp(space_857X);
      if (v_859X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L11565;}
      else {
        arg2K0 = 0;
        arg0K1 = 0;
        goto L11565;}}}}
 L11481: {
  SstackS = ((SstackS) + 4);
  list_860X = *((long *) (SstackS));
  arg0K0 = count_68X;
  arg0K1 = list_860X;
  goto L11490;}
 L11565: {
  okayP_861X = arg2K0;
  key_862X = arg0K1;
  if (okayP_861X) {
    arg0K0 = key_862X;
    goto L11516;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_862X;
    goto L11516;}}
 L11490: {
  i_863X = arg0K0;
  l_864X = arg0K1;
  if ((0 < i_863X)) {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_864X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (-1 + i_863X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_864X))) + 4)));
    goto L11490;}
  else {
    push_list0_return_value = l_864X;
    goto push_list_return;}}
 L11516: {
  v_865X = arg0K0;
  merged_arg0K0 = v_865X;
  copy_stack_into_heap_return_tag = 4;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_4:
  if ((count_68X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L11481;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L11481;}}
 push_list_return:
  switch (push_list_return_tag) {
  case 0: goto push_list_return_0;
  case 1: goto push_list_return_1;
  case 2: goto push_list_return_2;
  default: goto push_list_return_3;
  }}

 copy_stack_into_heap: {
  key_66X = merged_arg0K0;{
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L10987;}
 L10987: {
  p_866X = arg1K0;
  i_867X = arg0K1;
  if ((2 == (3 & (*((long *) p_866X))))) {
    if ((54 == (*((long *) p_866X)))) {
      arg0K0 = (-1 + i_867X);
      goto L10974;}
    else {
      arg0K0 = i_867X;
      goto L10974;}}
  else {
    arg1K0 = (p_866X + 4);
    arg0K1 = (1 + i_867X);
    goto L10987;}}
 L10974: {
  arg_count_868X = arg0K0;
  top_869X = SstackS;
  if ((1 == (ScontS))) {
    goto L10979;}
  else {
    merged_arg0K0 = key_66X;
    merged_arg0K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L10979;}}
 L10979: {
  stob_870X = ScontS;
  arg1K0 = ((((char *) (-3 + stob_870X))) + -8);
  arg1K1 = (top_869X + (((arg_count_868X)<<2)));
  goto L11012;}
 L11012: {
  loc_871X = arg1K0;
  arg_872X = arg1K1;
  if ((top_869X < arg_872X)) {
    *((long *) loc_871X) = (*((long *) arg_872X));
    arg1K0 = (loc_871X + -4);
    arg1K1 = (arg_872X + -4);
    goto L11012;}
  else {
    SstackS = loc_871X;
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
  key_64X = merged_arg0K0;
  reason_65X = merged_arg0K1;{
  p_873X = SenvS;
  if ((3 == (3 & p_873X))) {
    if ((p_873X < (((long) (Sstack_beginS))))) {
      goto L10582;}
    else {
      if (((((long) (Sstack_endS))) < p_873X)) {
        goto L10582;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_64X;
        merged_arg0K3 = reason_65X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_874X = save_env_in_heap0_return_value;
        SenvS = v_874X;
        goto L10582;}}}
  else {
    goto L10582;}}
 L10582: {
  end_875X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L10535;}
 L10535: {
  cont_876X = arg0K0;
  previous_877X = arg0K1;
  if ((cont_876X == (Sbottom_of_stackS))) {
    *((long *) (((char *) (-3 + previous_877X)))) = end_875X;
    ScontS = (Sbottom_of_stackS);
    really_preserve_continuation0_return_value = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
    goto really_preserve_continuation_return;}
  else {
    p_878X = *((long *) ((((char *) (-3 + cont_876X))) + 12));
    if ((3 == (3 & p_878X))) {
      if ((p_878X < (((long) (Sstack_beginS))))) {
        goto L10553;}
      else {
        if (((((long) (Sstack_endS))) < p_878X)) {
          goto L10553;}
        else {
          merged_arg0K0 = (*((long *) ((((char *) (-3 + cont_876X))) + 12)));
          merged_arg0K1 = cont_876X;
          merged_arg0K2 = key_64X;
          merged_arg0K3 = reason_65X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L10553;}}}
    else {
      goto L10553;}}}
 L10553: {
  header_879X = *((long *) ((((char *) (-3 + cont_876X))) + -4));
  addr_880X = ALLOCATE_SPACE((31 & (((header_879X)>>2))), (4 + ((long)(((unsigned long)header_879X)>>8))));
  data_addr_881X = addr_880X + 4;
  *((long *) addr_880X) = header_879X;
  memcpy((void *)data_addr_881X, (void *)(((char *) (-3 + cont_876X))),((long)(((unsigned long)header_879X)>>8)));
  new_882X = 3 + (((long) data_addr_881X));
  *((long *) (((char *) (-3 + previous_877X)))) = new_882X;
  arg0K0 = (*((long *) (((char *) (-3 + new_882X)))));
  arg0K1 = new_882X;
  goto L10535;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 copy_env: {
  env_63X = merged_arg0K0;{
  header_883X = *((long *) ((((char *) (-3 + env_63X))) + -4));
  addr_884X = ALLOCATE_SPACE((31 & (((header_883X)>>2))), (4 + ((long)(((unsigned long)header_883X)>>8))));
  data_addr_885X = addr_884X + 4;
  *((long *) addr_884X) = header_883X;
  memcpy((void *)data_addr_885X, (void *)(((char *) (-3 + env_63X))),((long)(((unsigned long)header_883X)>>8)));
  new_886X = 3 + (((long) data_addr_885X));
  addr_887X = ((char *) (-3 + env_63X));
  WRITE_BARRIER(addr_887X, 54);
  *((long *) addr_887X) = 54;
  *((long *) ((((char *) (-3 + env_63X))) + -4)) = new_886X;
  copy_env0_return_value = new_886X;
  goto copy_env_return;}
 copy_env_return:
  switch (copy_env_return_tag) {
  case 0: goto copy_env_return_0;
  default: goto copy_env_return_1;
  }}

 save_env_in_heap: {
  env_59X = merged_arg0K0;
  cont_60X = merged_arg0K1;
  key_61X = merged_arg0K2;
  reason_62X = merged_arg0K3;{
  merged_arg0K0 = env_59X;
  merged_arg0K1 = key_61X;
  merged_arg0K2 = reason_62X;
  copy_env_return_tag = 0;
  goto copy_env;
 copy_env_return_0:
  top_888X = copy_env0_return_value;
  arg0K0 = top_888X;
  goto L9784;}
 L9784: {
  env_889X = arg0K0;
  p_890X = *((long *) (((char *) (-3 + env_889X))));
  if ((3 == (3 & p_890X))) {
    if ((p_890X < (((long) (Sstack_beginS))))) {
      goto L9803;}
    else {
      if (((((long) (Sstack_endS))) < p_890X)) {
        goto L9803;}
      else {
        merged_arg0K0 = (*((long *) (((char *) (-3 + env_889X)))));
        merged_arg0K1 = key_61X;
        merged_arg0K2 = reason_62X;
        copy_env_return_tag = 1;
        goto copy_env;
       copy_env_return_1:
        new_891X = copy_env0_return_value;
        addr_892X = ((char *) (-3 + env_889X));
        WRITE_BARRIER(addr_892X, new_891X);
        *((long *) addr_892X) = new_891X;
        arg0K0 = new_891X;
        goto L9784;}}}
  else {
    goto L9803;}}
 L9803: {
  arg0K0 = cont_60X;
  goto L9807;}
 L9807: {
  cont_893X = arg0K0;
  env_894X = *((long *) ((((char *) (-3 + cont_893X))) + 12));
  if ((3 == (3 & env_894X))) {
    if ((3 == (3 & (*((long *) ((((char *) (-3 + env_894X))) + -4)))))) {
      *((long *) ((((char *) (-3 + cont_893X))) + 12)) = (*((long *) ((((char *) (-3 + env_894X))) + -4)));
      arg0K0 = (*((long *) (((char *) (-3 + cont_893X)))));
      goto L9807;}
    else {
      save_env_in_heap0_return_value = top_888X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = top_888X;
    goto save_env_in_heap_return;}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  default: goto save_env_in_heap_return_2;
  }}

 pop_args_GlistS: {
  start_57X = merged_arg0K0;
  count_58X = merged_arg0K1;{
  space_895X = 3 * count_58X;
  v_896X = AVAILABLEp(space_895X);
  if (v_896X) {
    arg2K0 = 1;
    arg0K1 = start_57X;
    goto L11425;}
  else {
    merged_arg0K0 = start_57X;
    collect_saving_temp_return_tag = 27;
    goto collect_saving_temp;
   collect_saving_temp_return_27:
    temp_897X = collect_saving_temp0_return_value;
    v_898X = AVAILABLEp(space_895X);
    if (v_898X) {
      arg2K0 = 1;
      arg0K1 = temp_897X;
      goto L11425;}
    else {
      arg2K0 = 0;
      arg0K1 = temp_897X;
      goto L11425;}}}
 L11425: {
  okayP_899X = arg2K0;
  temp_900X = arg0K1;
  if (okayP_899X) {
    arg0K0 = temp_900X;
    goto L11399;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_900X;
    goto L11399;}}
 L11399: {
  start_901X = arg0K0;
  arg0K0 = start_901X;
  arg0K1 = count_58X;
  goto L11408;}
 L11408: {
  args_902X = arg0K0;
  count_903X = arg0K1;
  if ((0 == count_903X)) {
    pop_args_GlistS0_return_value = args_902X;
    goto pop_args_GlistS_return;}
  else {
    SstackS = ((SstackS) + 4);
    a_904X = *((long *) (SstackS));
    addr_905X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_905X) = 2050;
    x_906X = 3 + (((long) (addr_905X + 4)));
    *((long *) (((char *) (-3 + x_906X)))) = a_904X;
    *((long *) ((((char *) (-3 + x_906X))) + 4)) = args_902X;
    arg0K0 = x_906X;
    arg0K1 = (-1 + count_903X);
    goto L11408;}}
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
  list_55X = merged_arg0K0;
  length_56X = merged_arg0K1;{
  if ((0 == length_56X)) {
    copy_listS0_return_value = 25;
    goto copy_listS_return;}
  else {
    space_907X = 3 * length_56X;
    v_908X = AVAILABLEp(space_907X);
    if (v_908X) {
      arg2K0 = 1;
      arg0K1 = list_55X;
      goto L11305;}
    else {
      merged_arg0K0 = list_55X;
      collect_saving_temp_return_tag = 28;
      goto collect_saving_temp;
     collect_saving_temp_return_28:
      temp_909X = collect_saving_temp0_return_value;
      v_910X = AVAILABLEp(space_907X);
      if (v_910X) {
        arg2K0 = 1;
        arg0K1 = temp_909X;
        goto L11305;}
      else {
        arg2K0 = 0;
        arg0K1 = temp_909X;
        goto L11305;}}}}
 L11305: {
  okayP_911X = arg2K0;
  temp_912X = arg0K1;
  if (okayP_911X) {
    arg0K0 = temp_912X;
    goto L11263;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_912X;
    goto L11263;}}
 L11263: {
  list_913X = arg0K0;
  a_914X = *((long *) (((char *) (-3 + list_913X))));
  addr_915X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_915X) = 2050;
  x_916X = 3 + (((long) (addr_915X + 4)));
  *((long *) (((char *) (-3 + x_916X)))) = a_914X;
  *((long *) ((((char *) (-3 + x_916X))) + 4)) = 25;
  arg0K0 = (*((long *) ((((char *) (-3 + list_913X))) + 4)));
  arg0K1 = x_916X;
  goto L11278;}
 L11278: {
  l_917X = arg0K0;
  last_918X = arg0K1;
  if ((25 == l_917X)) {
    copy_listS0_return_value = x_916X;
    goto copy_listS_return;}
  else {
    a_919X = *((long *) (((char *) (-3 + l_917X))));
    addr_920X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_920X) = 2050;
    x_921X = 3 + (((long) (addr_920X + 4)));
    *((long *) (((char *) (-3 + x_921X)))) = a_919X;
    *((long *) ((((char *) (-3 + x_921X))) + 4)) = 25;
    addr_922X = (((char *) (-3 + last_918X))) + 4;
    WRITE_BARRIER(addr_922X, x_921X);
    *((long *) addr_922X) = x_921X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_917X))) + 4)));
    arg0K1 = x_921X;
    goto L11278;}}
 copy_listS_return:
  switch (copy_listS_return_tag) {
  case 0: goto copy_listS_return_0;
  case 1: goto copy_listS_return_1;
  case 2: goto copy_listS_return_2;
  default: goto copy_listS_return_3;
  }}

 collect_saving_temp: {
  value_54X = merged_arg0K0;{
  pc_923X = ((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2);
  begin_collection();
  v_924X = trace_value((StemplateS));
  StemplateS = v_924X;
  v_925X = trace_value((SvalS));
  SvalS = v_925X;
  v_926X = trace_value((Scurrent_threadS));
  Scurrent_threadS = v_926X;
  v_927X = trace_value((Ssession_dataS));
  Ssession_dataS = v_927X;
  v_928X = trace_value((Sexception_handlersS));
  Sexception_handlersS = v_928X;
  v_929X = trace_value((Sexception_templateS));
  Sexception_templateS = v_929X;
  v_930X = trace_value((Sinterrupt_handlersS));
  Sinterrupt_handlersS = v_930X;
  v_931X = trace_value((Sinterrupt_templateS));
  Sinterrupt_templateS = v_931X;
  v_932X = trace_value((Sinterrupted_templateS));
  Sinterrupted_templateS = v_932X;
  v_933X = trace_value((Sfinalize_theseS));
  Sfinalize_theseS = v_933X;
  env_934X = trace_value((SenvS));
  SenvS = env_934X;
  v_935X = trace_value((Spending_channels_headS));
  Spending_channels_headS = v_935X;
  v_936X = trace_value((Spending_channels_tailS));
  Spending_channels_tailS = v_936X;
  if ((Sstack_warningPS)) {
    arg1K0 = (Sstack_beginS);
    goto L4878;}
  else {
    goto L4911;}}
 L4878: {
  a_937X = arg1K0;
  if ((252645135 == (*((long *) a_937X)))) {
    arg1K0 = (a_937X + 4);
    goto L4878;}
  else {
    unused_938X = (((a_937X - (Sstack_beginS)))>>2);
    if ((unused_938X < 30)) {
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      ps_write_string("[Alert: stack overconsumption (", (stderr));
      ps_write_integer(unused_938X, (stderr));
      ps_write_string("); please inform the Scheme 48 implementors]", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sstack_warningPS = 0;
      goto L4911;}
    else {
      goto L4911;}}}
 L4911: {
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L4976;}
 L4976: {
  p_939X = arg1K0;
  i_940X = arg0K1;
  if ((2 == (3 & (*((long *) p_939X))))) {
    if ((54 == (*((long *) p_939X)))) {
      arg0K0 = (-1 + i_940X);
      goto L4913;}
    else {
      arg0K0 = i_940X;
      goto L4913;}}
  else {
    arg1K0 = (p_939X + 4);
    arg0K1 = (1 + i_940X);
    goto L4976;}}
 L4913: {
  arg_count_941X = arg0K0;
  trace_locationsB(((SstackS) + 4), ((SstackS) + (4 + (((arg_count_941X)<<2)))));
  p_942X = SenvS;
  if ((3 == (3 & p_942X))) {
    if ((p_942X < (((long) (Sstack_beginS))))) {
      goto L4929;}
    else {
      if (((((long) (Sstack_endS))) < p_942X)) {
        goto L4929;}
      else {
        env_943X = SenvS;
        arg0K0 = env_943X;
        goto L5089;}}}
  else {
    goto L4929;}}
 L4929: {
  v_944X = trace_value((SenvS));
  SenvS = v_944X;
  goto L4934;}
 L5089: {
  env_945X = arg0K0;
  if ((3 == (3 & env_945X))) {
    if ((env_945X < (((long) (Sstack_beginS))))) {
      goto L4934;}
    else {
      if (((((long) (Sstack_endS))) < env_945X)) {
        goto L4934;}
      else {
        trace_stob_contentsB(env_945X);
        arg0K0 = (*((long *) (((char *) (-3 + env_945X)))));
        goto L5089;}}}
  else {
    goto L4934;}}
 L4934: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L4939;}
 L4939: {
  cont_946X = arg0K0;
  last_env_947X = arg0K1;
  env_948X = *((long *) ((((char *) (-3 + cont_946X))) + 12));
  trace_stob_contentsB(cont_946X);
  if ((env_948X == last_env_947X)) {
    goto L4957;}
  else {
    arg0K0 = env_948X;
    goto L5044;}}
 L4957: {
  if ((cont_946X == (Sbottom_of_stackS))) {
    value_949X = trace_value(value_54X);
    okay_950X = do_gc((Sfinalizer_alistS), &sickly_951X);
    Sfinalizer_alistS = okay_950X;
    l2_952X = Sfinalize_theseS;
    if ((25 == sickly_951X)) {
      arg0K0 = l2_952X;
      goto L10863;}
    else {
      arg0K0 = sickly_951X;
      goto L10886;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + cont_946X)))));
    arg0K1 = env_948X;
    goto L4939;}}
 L5044: {
  env_953X = arg0K0;
  if ((3 == (3 & env_953X))) {
    if ((env_953X < (((long) (Sstack_beginS))))) {
      goto L4957;}
    else {
      if (((((long) (Sstack_endS))) < env_953X)) {
        goto L4957;}
      else {
        trace_stob_contentsB(env_953X);
        arg0K0 = (*((long *) (((char *) (-3 + env_953X)))));
        goto L5044;}}}
  else {
    goto L4957;}}
 L10863: {
  v_954X = arg0K0;
  Sfinalize_theseS = v_954X;
  end_collection();
  arg0K0 = 0;
  goto L10424;}
 L10886: {
  l_955X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_955X))) + 4))))) {
    addr_956X = (((char *) (-3 + l_955X))) + 4;
    WRITE_BARRIER(addr_956X, l2_952X);
    *((long *) addr_956X) = l2_952X;
    arg0K0 = sickly_951X;
    goto L10863;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_955X))) + 4)));
    goto L10886;}}
 L10424: {
  i_957X = arg0K0;
  if ((i_957X == (Snumber_of_channelsS))) {
    tem_958X = StemplateS;
    StemplateS = tem_958X;
    Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_958X)))))))) + (((pc_923X)>>2)));
    Spending_interruptsS = (4 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        collect_saving_temp0_return_value = value_949X;
        goto collect_saving_temp_return;}
      else {
        collect_saving_temp0_return_value = value_949X;
        goto collect_saving_temp_return;}}
    else {
      Spending_interruptPS = 1;
      collect_saving_temp0_return_value = value_949X;
      goto collect_saving_temp_return;}}
  else {
    channel_959X = *((Svm_channelsS) + i_957X);
    if ((1 == channel_959X)) {
      goto L10460;}
    else {
      header_960X = *((long *) ((((char *) (-3 + channel_959X))) + -4));
      if ((3 == (3 & header_960X))) {
        arg0K0 = header_960X;
        goto L10453;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_959X))))))) {
          arg0K0 = 1;
          goto L10453;}
        else {
          id_961X = *((long *) ((((char *) (-3 + channel_959X))) + 4));
          if ((0 == (3 & id_961X))) {
            arg0K0 = id_961X;
            goto L10228;}
          else {
            if ((3 == (3 & (*((long *) ((((char *) (-3 + id_961X))) + -4)))))) {
              arg0K0 = (*((long *) ((((char *) (-3 + id_961X))) + -4)));
              goto L10228;}
            else {
              arg0K0 = id_961X;
              goto L10228;}}}}}}}
 L10460: {
  arg0K0 = (1 + i_957X);
  goto L10424;}
 L10453: {
  new_962X = arg0K0;
  *((Svm_channelsS) + i_957X) = new_962X;
  goto L10460;}
 L10228: {
  id_963X = arg0K0;
  merged_arg0K0 = channel_959X;
  close_channelB_return_tag = 1;
  goto close_channelB;
 close_channelB_return_1:
  status_964X = close_channelB0_return_value;
  if ((status_964X == NO_ERRORS)) {
    goto L10243;}
  else {
    merged_arg0K0 = status_964X;
    merged_arg0K1 = (*((long *) ((((char *) (-3 + channel_959X))) + 8)));
    merged_arg0K2 = id_963X;
    channel_close_error_return_tag = 1;
    goto channel_close_error;
   channel_close_error_return_1:
    goto L10243;}}
 L10243: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_963X))) {
    ps_write_integer((((id_963X)>>2)), (stderr));
    goto L10261;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_963X))))), (stderr));
    goto L10261;}}
 L10261: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L10453;}
 collect_saving_temp_return:
  switch (collect_saving_temp_return_tag) {
  case 0: goto collect_saving_temp_return_0;
  case 1: goto collect_saving_temp_return_1;
  case 2: goto collect_saving_temp_return_2;
  case 3: goto collect_saving_temp_return_3;
  case 4: goto collect_saving_temp_return_4;
  case 5: goto collect_saving_temp_return_5;
  case 6: goto collect_saving_temp_return_6;
  case 7: goto collect_saving_temp_return_7;
  case 8: goto collect_saving_temp_return_8;
  case 9: goto collect_saving_temp_return_9;
  case 10: goto collect_saving_temp_return_10;
  case 11: goto collect_saving_temp_return_11;
  case 12: goto collect_saving_temp_return_12;
  case 13: goto collect_saving_temp_return_13;
  case 14: goto collect_saving_temp_return_14;
  case 15: goto collect_saving_temp_return_15;
  case 16: goto collect_saving_temp_return_16;
  case 17: goto collect_saving_temp_return_17;
  case 18: goto collect_saving_temp_return_18;
  case 19: goto collect_saving_temp_return_19;
  case 20: goto collect_saving_temp_return_20;
  case 21: goto collect_saving_temp_return_21;
  case 22: goto collect_saving_temp_return_22;
  case 23: goto collect_saving_temp_return_23;
  case 24: goto collect_saving_temp_return_24;
  case 25: goto collect_saving_temp_return_25;
  case 26: goto collect_saving_temp_return_26;
  case 27: goto collect_saving_temp_return_27;
  default: goto collect_saving_temp_return_28;
  }}

 channel_close_error: {
  status_51X = merged_arg0K0;
  index_52X = merged_arg0K1;
  id_53X = merged_arg0K2;{
  ps_write_string("Error: ", (stderr));
  ps_write_string((ps_error_string(status_51X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(" while closing port ", (stderr));
  if ((3 == (3 & id_53X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + id_53X))) + -4))))>>2))))) {
      ps_write_string((((char *)(((char *) (-3 + id_53X))))), (stderr));
      goto L9040;}
    else {
      goto L9034;}}
  else {
    goto L9034;}}
 L9040: {
  PS_WRITE_CHAR(10, (stderr), v_965X)
  channel_close_error0_return_value = v_965X;
  goto channel_close_error_return;}
 L9034: {
  ps_write_integer((((index_52X)>>2)), (stderr));
  goto L9040;}
 channel_close_error_return:
  switch (channel_close_error_return_tag) {
  case 0: goto channel_close_error_return_0;
  default: goto channel_close_error_return_1;
  }}

 close_channelB: {
  channel_50X = merged_arg0K0;{
  os_index_966X = (((*((long *) ((((char *) (-3 + channel_50X))) + 8))))>>2);
  v_967X = *((long *) (((char *) (-3 + channel_50X))));
  if ((4 == v_967X)) {
    goto L5298;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_50X))))))) {
      goto L5298;}
    else {
      v_968X = ps_close_fd(os_index_966X);
      arg0K0 = v_968X;
      goto L5305;}}}
 L5298: {
  v_969X = ps_close_fd(os_index_966X);
  arg0K0 = v_969X;
  goto L5305;}
 L5305: {
  status_970X = arg0K0;
  *((Svm_channelsS) + os_index_966X) = 1;
  addr_971X = ((char *) (-3 + channel_50X));
  WRITE_BARRIER(addr_971X, 0);
  *((long *) addr_971X) = 0;
  close_channelB0_return_value = status_970X;
  goto close_channelB_return;}
 close_channelB_return:
  switch (close_channelB_return_tag) {
  case 0: goto close_channelB_return_0;
  default: goto close_channelB_return_1;
  }}

 copy_continuation_from_heapB: {
  cont_49X = merged_arg0K0;{
  top_972X = (((char *) (-3 + (Sbottom_of_stackS)))) + (-8 - (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_49X))) + -4))))>>8)))));
  new_cont_973X = 3 + (((long) (top_972X + 4)));
  SstackS = (top_972X + -4);
  ScontS = new_cont_973X;
  v_974X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_49X))) + -4))))>>8))))>>2);
  memcpy((void *)top_972X, (void *)((((char *) (-3 + cont_49X))) + -4),(4 + (((v_974X)<<2))));
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = (*((long *) (((char *) (-3 + new_cont_973X)))));
  *((long *) (((char *) (-3 + new_cont_973X)))) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_973X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  default: goto copy_continuation_from_heapB_return_1;
  }}

 get_current_port: {
  marker_48X = merged_arg0K0;{
  thread_975X = Scurrent_threadS;
  if ((3 == (3 & thread_975X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + thread_975X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_975X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_975X))) + 4)));
        goto L8515;}
      else {
        v_976X = enter_string("bad record");
        get_current_port0_return_value = v_976X;
        goto get_current_port_return;}}
    else {
      v_977X = enter_string("bad record");
      get_current_port0_return_value = v_977X;
      goto get_current_port_return;}}
  else {
    v_978X = enter_string("bad record");
    get_current_port0_return_value = v_978X;
    goto get_current_port_return;}}
 L8515: {
  env_979X = arg0K0;
  if ((3 == (3 & env_979X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_979X))) + -4))))>>2))))) {
      obj_980X = *((long *) (((char *) (-3 + env_979X))));
      if ((3 == (3 & obj_980X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_980X))) + -4))))>>2))))) {
          if ((marker_48X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_979X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_979X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_979X))) + 4)));
            goto L8515;}}
        else {
          goto L8571;}}
      else {
        goto L8571;}}
    else {
      goto L8571;}}
  else {
    goto L8571;}}
 L8571: {
  if ((25 == env_979X)) {
    v_981X = enter_string("null");
    get_current_port0_return_value = v_981X;
    goto get_current_port_return;}
  else {
    v_982X = enter_string("not pair");
    get_current_port0_return_value = v_982X;
    goto get_current_port_return;}}
 get_current_port_return:
  switch (get_current_port_return_tag) {
  case 0: goto get_current_port_return_0;
  case 1: goto get_current_port_return_1;
  default: goto get_current_port_return_2;
  }}

 okay_argument_list: {
  list_47X = merged_arg0K0;{
  arg0K0 = list_47X;
  arg0K1 = 0;
  arg0K2 = list_47X;
  arg2K3 = 0;
  goto L7965;}
 L7965: {
  fast_983X = arg0K0;
  len_984X = arg0K1;
  slow_985X = arg0K2;
  move_slowP_986X = arg2K3;
  if ((25 == fast_983X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_984X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_983X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_983X))) + -4))))>>2))))) {
        if (move_slowP_986X) {
          if ((fast_983X == slow_985X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_983X))) + 4)));
            arg0K1 = (1 + len_984X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_985X))) + 4)));
            arg2K3 = 0;
            goto L7965;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_983X))) + 4)));
          arg0K1 = (1 + len_984X);
          arg0K2 = slow_985X;
          arg2K3 = 1;
          goto L7965;}}
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

 push_exception_continuationB: {
  exception_45X = merged_arg0K0;
  instruction_size_46X = merged_arg0K1;{
  opcode_987X = *((unsigned char *) (Scode_pointerS));
  *((long *) (SstackS)) = (((instruction_size_46X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_45X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (StemplateS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  SstackS = ((SstackS) + -4);
  tem_988X = Sexception_templateS;
  StemplateS = tem_988X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_988X))))))));
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L7816;}
 L7816: {
  p_989X = arg1K0;
  i_990X = arg0K1;
  if ((2 == (3 & (*((long *) p_989X))))) {
    if ((54 == (*((long *) p_989X)))) {
      arg0K0 = (-1 + i_990X);
      goto L7757;}
    else {
      arg0K0 = i_990X;
      goto L7757;}}
  else {
    arg1K0 = (p_989X + 4);
    arg0K1 = (1 + i_990X);
    goto L7816;}}
 L7757: {
  v_991X = arg0K0;
  merged_arg1K0 = (Scode_pointerS);
  merged_arg0K1 = v_991X;
  push_continuationB_return_tag = 3;
  goto push_continuationB;
 push_continuationB_return_3:
  *((long *) (SstackS)) = (((opcode_987X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_45X)<<2));
  SstackS = ((SstackS) + -4);
  goto push_exception_continuationB_return;}
 push_exception_continuationB_return:
  switch (push_exception_continuationB_return_tag) {
  case 0: goto push_exception_continuationB_return_0;
  case 1: goto push_exception_continuationB_return_1;
  case 2: goto push_exception_continuationB_return_2;
  case 3: goto push_exception_continuationB_return_3;
  case 4: goto push_exception_continuationB_return_4;
  case 5: goto push_exception_continuationB_return_5;
  case 6: goto push_exception_continuationB_return_6;
  case 7: goto push_exception_continuationB_return_7;
  case 8: goto push_exception_continuationB_return_8;
  case 9: goto push_exception_continuationB_return_9;
  case 10: goto push_exception_continuationB_return_10;
  case 11: goto push_exception_continuationB_return_11;
  case 12: goto push_exception_continuationB_return_12;
  case 13: goto push_exception_continuationB_return_13;
  case 14: goto push_exception_continuationB_return_14;
  case 15: goto push_exception_continuationB_return_15;
  case 16: goto push_exception_continuationB_return_16;
  case 17: goto push_exception_continuationB_return_17;
  case 18: goto push_exception_continuationB_return_18;
  case 19: goto push_exception_continuationB_return_19;
  case 20: goto push_exception_continuationB_return_20;
  case 21: goto push_exception_continuationB_return_21;
  case 22: goto push_exception_continuationB_return_22;
  case 23: goto push_exception_continuationB_return_23;
  case 24: goto push_exception_continuationB_return_24;
  case 25: goto push_exception_continuationB_return_25;
  case 26: goto push_exception_continuationB_return_26;
  case 27: goto push_exception_continuationB_return_27;
  case 28: goto push_exception_continuationB_return_28;
  case 29: goto push_exception_continuationB_return_29;
  case 30: goto push_exception_continuationB_return_30;
  case 31: goto push_exception_continuationB_return_31;
  case 32: goto push_exception_continuationB_return_32;
  case 33: goto push_exception_continuationB_return_33;
  case 34: goto push_exception_continuationB_return_34;
  case 35: goto push_exception_continuationB_return_35;
  case 36: goto push_exception_continuationB_return_36;
  case 37: goto push_exception_continuationB_return_37;
  case 38: goto push_exception_continuationB_return_38;
  case 39: goto push_exception_continuationB_return_39;
  case 40: goto push_exception_continuationB_return_40;
  case 41: goto push_exception_continuationB_return_41;
  case 42: goto push_exception_continuationB_return_42;
  case 43: goto push_exception_continuationB_return_43;
  case 44: goto push_exception_continuationB_return_44;
  case 45: goto push_exception_continuationB_return_45;
  case 46: goto push_exception_continuationB_return_46;
  case 47: goto push_exception_continuationB_return_47;
  case 48: goto push_exception_continuationB_return_48;
  case 49: goto push_exception_continuationB_return_49;
  case 50: goto push_exception_continuationB_return_50;
  case 51: goto push_exception_continuationB_return_51;
  case 52: goto push_exception_continuationB_return_52;
  case 53: goto push_exception_continuationB_return_53;
  case 54: goto push_exception_continuationB_return_54;
  case 55: goto push_exception_continuationB_return_55;
  case 56: goto push_exception_continuationB_return_56;
  case 57: goto push_exception_continuationB_return_57;
  case 58: goto push_exception_continuationB_return_58;
  case 59: goto push_exception_continuationB_return_59;
  case 60: goto push_exception_continuationB_return_60;
  case 61: goto push_exception_continuationB_return_61;
  case 62: goto push_exception_continuationB_return_62;
  case 63: goto push_exception_continuationB_return_63;
  case 64: goto push_exception_continuationB_return_64;
  case 65: goto push_exception_continuationB_return_65;
  case 66: goto push_exception_continuationB_return_66;
  case 67: goto push_exception_continuationB_return_67;
  case 68: goto push_exception_continuationB_return_68;
  case 69: goto push_exception_continuationB_return_69;
  case 70: goto push_exception_continuationB_return_70;
  case 71: goto push_exception_continuationB_return_71;
  case 72: goto push_exception_continuationB_return_72;
  case 73: goto push_exception_continuationB_return_73;
  case 74: goto push_exception_continuationB_return_74;
  case 75: goto push_exception_continuationB_return_75;
  case 76: goto push_exception_continuationB_return_76;
  case 77: goto push_exception_continuationB_return_77;
  case 78: goto push_exception_continuationB_return_78;
  case 79: goto push_exception_continuationB_return_79;
  case 80: goto push_exception_continuationB_return_80;
  case 81: goto push_exception_continuationB_return_81;
  case 82: goto push_exception_continuationB_return_82;
  case 83: goto push_exception_continuationB_return_83;
  case 84: goto push_exception_continuationB_return_84;
  case 85: goto push_exception_continuationB_return_85;
  case 86: goto push_exception_continuationB_return_86;
  case 87: goto push_exception_continuationB_return_87;
  case 88: goto push_exception_continuationB_return_88;
  case 89: goto push_exception_continuationB_return_89;
  case 90: goto push_exception_continuationB_return_90;
  case 91: goto push_exception_continuationB_return_91;
  case 92: goto push_exception_continuationB_return_92;
  case 93: goto push_exception_continuationB_return_93;
  case 94: goto push_exception_continuationB_return_94;
  case 95: goto push_exception_continuationB_return_95;
  case 96: goto push_exception_continuationB_return_96;
  case 97: goto push_exception_continuationB_return_97;
  case 98: goto push_exception_continuationB_return_98;
  case 99: goto push_exception_continuationB_return_99;
  case 100: goto push_exception_continuationB_return_100;
  case 101: goto push_exception_continuationB_return_101;
  case 102: goto push_exception_continuationB_return_102;
  case 103: goto push_exception_continuationB_return_103;
  case 104: goto push_exception_continuationB_return_104;
  case 105: goto push_exception_continuationB_return_105;
  case 106: goto push_exception_continuationB_return_106;
  case 107: goto push_exception_continuationB_return_107;
  case 108: goto push_exception_continuationB_return_108;
  case 109: goto push_exception_continuationB_return_109;
  case 110: goto push_exception_continuationB_return_110;
  case 111: goto push_exception_continuationB_return_111;
  case 112: goto push_exception_continuationB_return_112;
  case 113: goto push_exception_continuationB_return_113;
  case 114: goto push_exception_continuationB_return_114;
  case 115: goto push_exception_continuationB_return_115;
  case 116: goto push_exception_continuationB_return_116;
  case 117: goto push_exception_continuationB_return_117;
  case 118: goto push_exception_continuationB_return_118;
  case 119: goto push_exception_continuationB_return_119;
  case 120: goto push_exception_continuationB_return_120;
  case 121: goto push_exception_continuationB_return_121;
  case 122: goto push_exception_continuationB_return_122;
  case 123: goto push_exception_continuationB_return_123;
  case 124: goto push_exception_continuationB_return_124;
  case 125: goto push_exception_continuationB_return_125;
  case 126: goto push_exception_continuationB_return_126;
  case 127: goto push_exception_continuationB_return_127;
  case 128: goto push_exception_continuationB_return_128;
  case 129: goto push_exception_continuationB_return_129;
  case 130: goto push_exception_continuationB_return_130;
  case 131: goto push_exception_continuationB_return_131;
  case 132: goto push_exception_continuationB_return_132;
  case 133: goto push_exception_continuationB_return_133;
  case 134: goto push_exception_continuationB_return_134;
  case 135: goto push_exception_continuationB_return_135;
  case 136: goto push_exception_continuationB_return_136;
  case 137: goto push_exception_continuationB_return_137;
  case 138: goto push_exception_continuationB_return_138;
  case 139: goto push_exception_continuationB_return_139;
  case 140: goto push_exception_continuationB_return_140;
  case 141: goto push_exception_continuationB_return_141;
  case 142: goto push_exception_continuationB_return_142;
  case 143: goto push_exception_continuationB_return_143;
  case 144: goto push_exception_continuationB_return_144;
  case 145: goto push_exception_continuationB_return_145;
  case 146: goto push_exception_continuationB_return_146;
  case 147: goto push_exception_continuationB_return_147;
  case 148: goto push_exception_continuationB_return_148;
  case 149: goto push_exception_continuationB_return_149;
  case 150: goto push_exception_continuationB_return_150;
  case 151: goto push_exception_continuationB_return_151;
  case 152: goto push_exception_continuationB_return_152;
  case 153: goto push_exception_continuationB_return_153;
  case 154: goto push_exception_continuationB_return_154;
  case 155: goto push_exception_continuationB_return_155;
  case 156: goto push_exception_continuationB_return_156;
  case 157: goto push_exception_continuationB_return_157;
  case 158: goto push_exception_continuationB_return_158;
  case 159: goto push_exception_continuationB_return_159;
  case 160: goto push_exception_continuationB_return_160;
  case 161: goto push_exception_continuationB_return_161;
  case 162: goto push_exception_continuationB_return_162;
  case 163: goto push_exception_continuationB_return_163;
  case 164: goto push_exception_continuationB_return_164;
  case 165: goto push_exception_continuationB_return_165;
  case 166: goto push_exception_continuationB_return_166;
  case 167: goto push_exception_continuationB_return_167;
  case 168: goto push_exception_continuationB_return_168;
  case 169: goto push_exception_continuationB_return_169;
  case 170: goto push_exception_continuationB_return_170;
  case 171: goto push_exception_continuationB_return_171;
  case 172: goto push_exception_continuationB_return_172;
  case 173: goto push_exception_continuationB_return_173;
  case 174: goto push_exception_continuationB_return_174;
  case 175: goto push_exception_continuationB_return_175;
  case 176: goto push_exception_continuationB_return_176;
  case 177: goto push_exception_continuationB_return_177;
  case 178: goto push_exception_continuationB_return_178;
  case 179: goto push_exception_continuationB_return_179;
  case 180: goto push_exception_continuationB_return_180;
  case 181: goto push_exception_continuationB_return_181;
  case 182: goto push_exception_continuationB_return_182;
  case 183: goto push_exception_continuationB_return_183;
  case 184: goto push_exception_continuationB_return_184;
  case 185: goto push_exception_continuationB_return_185;
  case 186: goto push_exception_continuationB_return_186;
  case 187: goto push_exception_continuationB_return_187;
  case 188: goto push_exception_continuationB_return_188;
  case 189: goto push_exception_continuationB_return_189;
  case 190: goto push_exception_continuationB_return_190;
  case 191: goto push_exception_continuationB_return_191;
  case 192: goto push_exception_continuationB_return_192;
  case 193: goto push_exception_continuationB_return_193;
  case 194: goto push_exception_continuationB_return_194;
  case 195: goto push_exception_continuationB_return_195;
  case 196: goto push_exception_continuationB_return_196;
  case 197: goto push_exception_continuationB_return_197;
  case 198: goto push_exception_continuationB_return_198;
  case 199: goto push_exception_continuationB_return_199;
  case 200: goto push_exception_continuationB_return_200;
  default: goto push_exception_continuationB_return_201;
  }}

 push_continuationB: {
  code_pointer_43X = merged_arg1K0;
  size_44X = merged_arg0K1;{
  pc_992X = (((code_pointer_43X - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2);
  template_993X = StemplateS;
  SstackS = ((SstackS) + -20);
  *((long *) ((SstackS) + 4)) = (128 | (4134 + (((size_44X)<<10))));
  cont_994X = 3 + (((long) ((SstackS) + 8)));
  *((long *) ((((char *) (-3 + cont_994X))) + 4)) = pc_992X;
  *((long *) ((((char *) (-3 + cont_994X))) + 8)) = template_993X;
  *((long *) ((((char *) (-3 + cont_994X))) + 12)) = (SenvS);
  *((long *) (((char *) (-3 + cont_994X)))) = (ScontS);
  ScontS = cont_994X;
  goto push_continuationB_return;}
 push_continuationB_return:
  switch (push_continuationB_return_tag) {
  case 0: goto push_continuationB_return_0;
  case 1: goto push_continuationB_return_1;
  case 2: goto push_continuationB_return_2;
  default: goto push_continuationB_return_3;
  }}

 get_error_string: {
  status_42X = merged_arg0K0;{
  string_995X = ps_error_string(status_42X);
  x_996X = strlen((char *) string_995X);
  if ((x_996X < 256)) {
    arg0K0 = x_996X;
    goto L7673;}
  else {
    arg0K0 = 256;
    goto L7673;}}
 L7673: {
  len_997X = arg0K0;
  len_998X = 1 + len_997X;
  addr_999X = ALLOCATE_SPACE(16, (4 + len_998X));
  *((long *) addr_999X) = (66 + (((len_998X)<<8)));
  string_1000X = 3 + (((long) (addr_999X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1000X))) + len_997X)) = 0;
  arg0K0 = 0;
  goto L7683;}
 L7683: {
  i_1001X = arg0K0;
  if ((i_1001X == len_997X)) {
    get_error_string0_return_value = string_1000X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1000X))) + i_1001X)) = ((*(string_995X + i_1001X)));
    arg0K0 = (1 + i_1001X);
    goto L7683;}}
 get_error_string_return:
  switch (get_error_string_return_tag) {
  case 0: goto get_error_string_return_0;
  case 1: goto get_error_string_return_1;
  case 2: goto get_error_string_return_2;
  case 3: goto get_error_string_return_3;
  default: goto get_error_string_return_4;
  }}

 make_closure: {
  a_40X = merged_arg0K0;
  b_41X = merged_arg0K1;{
  addr_1002X = ALLOCATE_SPACE(3, 12);
  *((long *) addr_1002X) = 2062;
  x_1003X = 3 + (((long) (addr_1002X + 4)));
  *((long *) (((char *) (-3 + x_1003X)))) = a_40X;
  *((long *) ((((char *) (-3 + x_1003X))) + 4)) = b_41X;
  if ((3 == (3 & x_1003X))) {
    if ((0 == (128 & (*((long *) ((((char *) (-3 + x_1003X))) + -4)))))) {
      *((long *) ((((char *) (-3 + x_1003X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_1003X))) + -4))));
      make_closure0_return_value = x_1003X;
      goto make_closure_return;}
    else {
      make_closure0_return_value = x_1003X;
      goto make_closure_return;}}
  else {
    make_closure0_return_value = x_1003X;
    goto make_closure_return;}}
 make_closure_return:
  switch (make_closure_return_tag) {
  case 0: goto make_closure_return_0;
  default: goto make_closure_return_1;
  }}

 pop_continuationB: {
{ cont_1004X = ScontS;
  tem_1005X = *((long *) ((((char *) (-3 + cont_1004X))) + 8));
  pc_1006X = *((long *) ((((char *) (-3 + cont_1004X))) + 4));
  StemplateS = tem_1005X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1005X)))))))) + (((pc_1006X)>>2)));
  SenvS = (*((long *) ((((char *) (-3 + cont_1004X))) + 12)));
  ScontS = (*((long *) (((char *) (-3 + cont_1004X)))));
  SstackS = ((((char *) (-3 + cont_1004X))) + 12);
  goto pop_continuationB_return;}
 pop_continuationB_return:
  switch (pop_continuationB_return_tag) {
  case 0: goto pop_continuationB_return_0;
  case 1: goto pop_continuationB_return_1;
  default: goto pop_continuationB_return_2;
  }}

}
long call_startup_procedure(long startup_proc_1007X, char **startup_vector_1008X, long startup_vector_length_1009X)
{
  long arg0K0;
  long code_1042X;
  char * addr_1041X;
  long x_1040X;
  char * addr_1039X;
  long b_1038X;
  long channel_1037X;
  long x_1036X;
  char * addr_1035X;
  long b_1034X;
  long channel_1033X;
  long x_1032X;
  char * addr_1031X;
  long b_1030X;
  long channel_1029X;
  long i_1028X;
  long length_1027X;
  long *v_1026X;
  long v_1025X;
  long v_1024X;
  long y_1023X;
  long x_1022X;
  long v_1021X;
  long x_1020X;
  long y_1019X;
  long x_1018X;
  long y_1017X;
  long tem_1016X;
  char * addr_1015X;
  long value_1014X;
  long i_1013X;
  long vector_1012X;
  char * addr_1011X;
  long len_1010X;
 {  len_1010X = ((startup_vector_length_1009X)<<2);
  addr_1011X = ALLOCATE_SPACE(2, (4 + len_1010X));
  *((long *) addr_1011X) = (10 + (((len_1010X)<<8)));
  vector_1012X = 3 + (((long) (addr_1011X + 4)));
  arg0K0 = 0;
  goto L16553;}
 L16553: {
  i_1013X = arg0K0;
  if ((i_1013X < startup_vector_length_1009X)) {
    value_1014X = enter_string((*(startup_vector_1008X + i_1013X)));
    addr_1015X = (((char *) (-3 + vector_1012X))) + (((i_1013X)<<2));
    WRITE_BARRIER(addr_1015X, value_1014X);
    *((long *) addr_1015X) = value_1014X;
    arg0K0 = (1 + i_1013X);
    goto L16553;}
  else {
    SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
    *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
    ScontS = (Sbottom_of_stackS);
    SenvS = 13;
    tem_1016X = Sinterrupt_templateS;
    StemplateS = tem_1016X;
    Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1016X))))))));
    SvalS = 13;
    Scurrent_threadS = 25;
    Ssession_dataS = 25;
    Sexception_handlersS = 25;
    Sinterrupt_handlersS = 25;
    Senabled_interruptsS = 0;
    Sfinalizer_alistS = 25;
    Sfinalize_theseS = 25;
    Spending_interruptsS = 0;
    Spending_interruptPS = 0;
    Sinterrupted_templateS = 1;
    *((long *) (SstackS)) = vector_1012X;
    SstackS = ((SstackS) + -4);
    y_1017X = fileno((stderr));
    x_1018X = fileno((stdout));
    if ((x_1018X < y_1017X)) {
      arg0K0 = y_1017X;
      goto L9617;}
    else {
      arg0K0 = x_1018X;
      goto L9617;}}}
 L9617: {
  y_1019X = arg0K0;
  x_1020X = fileno((stdin));
  if ((x_1020X < y_1019X)) {
    arg0K0 = y_1019X;
    goto L9619;}
  else {
    arg0K0 = x_1020X;
    goto L9619;}}
 L9619: {
  v_1021X = arg0K0;
  x_1022X = Snumber_of_channelsS;
  y_1023X = 1 + v_1021X;
  if ((x_1022X < y_1023X)) {
    arg0K0 = y_1023X;
    goto L9621;}
  else {
    arg0K0 = x_1022X;
    goto L9621;}}
 L9621: {
  v_1024X = arg0K0;
  Snumber_of_channelsS = v_1024X;
  v_1025X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L9643;}
  else {
    goto L9643;}}
 L9643: {
  v_1026X = Svm_channelsS;
  length_1027X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L9685;}
 L9685: {
  i_1028X = arg0K0;
  if ((i_1028X < length_1027X)) {
    *(v_1026X + i_1028X) = 1;
    arg0K0 = (1 + i_1028X);
    goto L9685;}
  else {
    channel_1029X = fileno((stdin));
    b_1030X = enter_string("standard input");
    addr_1031X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_1031X) = 5142;
    x_1032X = 3 + (((long) (addr_1031X + 4)));
    *((long *) (((char *) (-3 + x_1032X)))) = 4;
    *((long *) ((((char *) (-3 + x_1032X))) + 4)) = b_1030X;
    *((long *) ((((char *) (-3 + x_1032X))) + 8)) = (((channel_1029X)<<2));
    *((long *) ((((char *) (-3 + x_1032X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1032X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1029X) = x_1032X;
    channel_1033X = fileno((stderr));
    b_1034X = enter_string("standard error");
    addr_1035X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_1035X) = 5142;
    x_1036X = 3 + (((long) (addr_1035X + 4)));
    *((long *) (((char *) (-3 + x_1036X)))) = 8;
    *((long *) ((((char *) (-3 + x_1036X))) + 4)) = b_1034X;
    *((long *) ((((char *) (-3 + x_1036X))) + 8)) = (((channel_1033X)<<2));
    *((long *) ((((char *) (-3 + x_1036X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1036X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1033X) = x_1036X;
    channel_1037X = fileno((stdout));
    b_1038X = enter_string("standard output");
    addr_1039X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_1039X) = 5142;
    x_1040X = 3 + (((long) (addr_1039X + 4)));
    *((long *) (((char *) (-3 + x_1040X)))) = 8;
    *((long *) ((((char *) (-3 + x_1040X))) + 4)) = b_1038X;
    *((long *) ((((char *) (-3 + x_1040X))) + 8)) = (((channel_1037X)<<2));
    *((long *) ((((char *) (-3 + x_1040X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1040X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1037X) = x_1040X;
    *((long *) (SstackS)) = x_1032X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1040X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1036X;
    SstackS = ((SstackS) + -4);
    addr_1041X = ALLOCATE_SPACE(17, 6);
    *((long *) addr_1041X) = 582;
    code_1042X = 3 + (((long) (addr_1041X + 4)));
    *((unsigned char *) (((char *) (-3 + code_1042X)))) = 24;
    *((unsigned char *) ((((char *) (-3 + code_1042X))) + 1)) = 4;
    Scode_pointerS = (((char *) (-3 + code_1042X)));
    return restart(startup_proc_1007X);}}
}void
scheme48_init(void)
{
Snumber_of_channelsS = 100;
Spending_channels_headS = 1;
Spending_channels_tailS = 1;
Sstack_warningPS = 1;
Slosing_opcodeS = 0;
Spending_eventsPS = 0;
}
