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
static char * Stransmit_hpS;
static char * Smax_hpS;
static char * SstartS;
static char * Sother_hpS;
static char * Sheartbreak_hotelS;
static long Sour_address_spaceS;
static char * Snew_id_hotelS;
static char * Slosing_proxy_hotelS;
static long Sbad_count_proxiesS;
static long Snew_uidsS;
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
  goto L6712;}
 L6712: {
  i_0X = arg0K0;
  if ((i_0X == (Snumber_of_channelsS))) {
    return;}
  else {
    channel_1X = *((Svm_channelsS) + i_0X);
    if ((1 == channel_1X)) {
      goto L6772;}
    else {
      header_2X = *((long *) ((((char *) (-3 + channel_1X))) + -4));
      if ((3 == (3 & header_2X))) {
        ps_write_string("Channel closed in dumped image: ", (stderr));
        descriptor_3X = *((long *) ((((char *) (-3 + channel_1X))) + 4));
        if ((0 == (3 & descriptor_3X))) {
          ps_write_integer(((((*((long *) ((((char *) (-3 + channel_1X))) + 4))))>>2)), (stderr));
          goto L6758;}
        else {
          ps_write_string((((char *)(((char *) (-3 + (*((long *) ((((char *) (-3 + channel_1X))) + 4)))))))), (stderr));
          goto L6758;}}
      else {
        goto L6772;}}}}
 L6772: {
  arg0K0 = (1 + i_0X);
  goto L6712;}
 L6758: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  addr_4X = ((char *) (-3 + header_2X));
  WRITE_BARRIER(addr_4X, 0);
  *((long *) addr_4X) = 0;
  addr_5X = (((char *) (-3 + header_2X))) + 8;
  WRITE_BARRIER(addr_5X, -4);
  *((long *) addr_5X) = -4;
  goto L6772;}
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
  addr_9X = ALLOCATE_SPACE(19, (4 + len_8X));
  *((long *) addr_9X) = (78 + (((len_8X)<<8)));
  string_10X = 3 + (((long) (addr_9X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_10X))) + z_7X)) = 0;
  arg0K0 = 0;
  goto L9553;}
 L9553: {
  i_11X = arg0K0;
  if ((i_11X < z_7X)) {
    *((unsigned char *) ((((char *) (-3 + string_10X))) + i_11X)) = ((*(string_6X + i_11X)));
    arg0K0 = (1 + i_11X);
    goto L9553;}
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
  goto L3948;}
 L3948: {
  i_15X = arg0K0;
  s_16X = arg0K1;
  if ((i_15X < startup_vector_length_14X)) {
    arg0K0 = (1 + i_15X);
    arg0K1 = (1 + (s_16X + ((((4 + (strlen((char *) (*(startup_vector_13X + i_15X))))))>>2))));
    goto L3948;}
  else {
    return (53 + s_16X);}}
}
void initialize_vm(char * stack_begin_17X, long stack_size_18X)
{
  long arg0K1;
  char * arg1K0;
  long merged_arg0K1;
  long merged_arg0K0;

  int make_template_containing_ops_return_tag;
  long make_template_containing_ops0_return_value;
  long op1_19X;
  long op2_20X;
  char * addr_35X;
  char * addr_34X;
  char * addr_33X;
  long code_32X;
  long temp_31X;
  char * addr_30X;
  char * addr_29X;
  long v_28X;
  long v_27X;
  long cont_26X;
  long template_25X;
  char * a_24X;
  long size_23X;
  char * start_22X;
  char * stack_21X;
 {  if ((stack_size_18X < 8125)) {
    stack_21X = (char *)malloc(32500);
    if ((stack_21X == NULL)) {
      ps_error("out of memory, unable to continue", 0);
      arg1K0 = stack_21X;
      arg0K1 = 8125;
      goto L12265;}
    else {
      arg1K0 = stack_21X;
      arg0K1 = 8125;
      goto L12265;}}
  else {
    arg1K0 = stack_begin_17X;
    arg0K1 = stack_size_18X;
    goto L12265;}}
 L12265: {
  start_22X = arg1K0;
  size_23X = arg0K1;
  Sstack_beginS = start_22X;
  Sstack_endS = (start_22X + (((size_23X)<<2)));
  Sstack_limitS = ((Sstack_beginS) + 500);
  SstackS = ((Sstack_endS) + -4);
  ScontS = 1;
  SenvS = 17;
  arg1K0 = start_22X;
  goto L12297;}
 L12297: {
  a_24X = arg1K0;
  if ((a_24X == (Sstack_endS))) {
    merged_arg0K0 = 23;
    merged_arg0K1 = 31;
    make_template_containing_ops_return_tag = 0;
    goto make_template_containing_ops;
   make_template_containing_ops_return_0:
    template_25X = make_template_containing_ops0_return_value;
    SstackS = ((SstackS) + -20);
    *((long *) ((SstackS) + 4)) = 4134;
    cont_26X = 3 + (((long) ((SstackS) + 8)));
    *((long *) ((((char *) (-3 + cont_26X))) + 4)) = 0;
    *((long *) ((((char *) (-3 + cont_26X))) + 8)) = template_25X;
    *((long *) ((((char *) (-3 + cont_26X))) + 12)) = (SenvS);
    *((long *) (((char *) (-3 + cont_26X)))) = (ScontS);
    ScontS = cont_26X;
    Sbottom_of_stackS = (ScontS);
    merged_arg0K0 = 34;
    merged_arg0K1 = 141;
    make_template_containing_ops_return_tag = 1;
    goto make_template_containing_ops;
   make_template_containing_ops_return_1:
    v_27X = make_template_containing_ops0_return_value;
    Sinterrupt_templateS = v_27X;
    merged_arg0K0 = 138;
    merged_arg0K1 = 124;
    make_template_containing_ops_return_tag = 2;
    goto make_template_containing_ops;
   make_template_containing_ops_return_2:
    v_28X = make_template_containing_ops0_return_value;
    Sexception_templateS = v_28X;
    return;}
  else {
    *((long *) a_24X) = 252645135;
    arg1K0 = (a_24X + 4);
    goto L12297;}}
 make_template_containing_ops: {
  op1_19X = merged_arg0K0;
  op2_20X = merged_arg0K1;{
  addr_29X = ALLOCATE_SPACE(20, 6);
  *((long *) addr_29X) = 594;
  addr_30X = ALLOCATE_SPACE(11, 20);
  *((long *) addr_30X) = 4142;
  temp_31X = 3 + (((long) (addr_30X + 4)));
  code_32X = 3 + (((long) (addr_29X + 4)));
  addr_33X = ((char *) (-3 + temp_31X));
  WRITE_BARRIER(addr_33X, code_32X);
  *((long *) addr_33X) = code_32X;
  addr_34X = (((char *) (-3 + temp_31X))) + 8;
  WRITE_BARRIER(addr_34X, 0);
  *((long *) addr_34X) = 0;
  addr_35X = (((char *) (-3 + temp_31X))) + 12;
  WRITE_BARRIER(addr_35X, 1);
  *((long *) addr_35X) = 1;
  *((unsigned char *) (((char *) (-3 + code_32X)))) = op1_19X;
  *((unsigned char *) ((((char *) (-3 + code_32X))) + 1)) = op2_20X;
  make_template_containing_ops0_return_value = temp_31X;
  goto make_template_containing_ops_return;}
 make_template_containing_ops_return:
  switch (make_template_containing_ops_return_tag) {
  case 0: goto make_template_containing_ops_return_0;
  case 1: goto make_template_containing_ops_return_1;
  default: goto make_template_containing_ops_return_2;
  }}

}
long restart(long value_36X)
{
  char *arg3K0;
  char arg2K3;
  char arg2K1;
  char arg2K2;
  char arg2K0;
  char * arg1K1;
  char * arg1K0;
  char * arg1K4;
  char * arg1K3;
  long arg0K3;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  char *merged_arg3K0;
  char merged_arg2K2;
  char * merged_arg1K1;
  char * merged_arg1K0;
  long merged_arg0K3;
  long merged_arg0K2;
  long merged_arg0K1;
  long merged_arg0K0;

  int pop_continuationB_return_tag;
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
  int encode_return_tag;
  char encode0_return_value;
  int extend_decode_vector_return_tag;
  char extend_decode_vector0_return_value;
  int encode_object_return_tag;
  long encode_object0_return_value;
  int real_next_id_return_tag;
  long real_next_id0_return_value;
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
  long status_37X;
  char * code_pointer_38X;
  long size_39X;
  long exception_40X;
  long instruction_size_41X;
  long list_42X;
  long marker_43X;
  long cont_44X;
  long thing_45X;
  long address_space_46X;
  long pair_47X;
  long address_space_48X;
  char proxiesP_49X;
  long thing_50X;
  long thing_51X;
  long decode_vector_52X;
  long channel_53X;
  long status_54X;
  long index_55X;
  long id_56X;
  long value_57X;
  long list_58X;
  long length_59X;
  long start_60X;
  long count_61X;
  long env_62X;
  long cont_63X;
  long key_64X;
  long reason_65X;
  long env_66X;
  long key_67X;
  long reason_68X;
  long key_69X;
  long list_70X;
  long count_71X;
  long wants_stack_args_72X;
  long stack_arg_count_73X;
  long list_args_74X;
  long list_arg_count_75X;
  char *message_76X;
  long pc_1278X;
  long tem_1277X;
  long cont_1276X;
  long i_1275X;
  long string_1274X;
  char * addr_1273X;
  long len_1272X;
  long len_1271X;
  long x_1270X;
  char *string_1269X;
  long cont_1268X;
  long template_1267X;
  long pc_1266X;
  long v_1265X;
  long i_1264X;
  char * p_1263X;
  long tem_1262X;
  long opcode_1261X;
  char move_slowP_1260X;
  long slow_1259X;
  long len_1258X;
  long fast_1257X;
  long v_1256X;
  long v_1255X;
  long obj_1254X;
  long env_1253X;
  long v_1252X;
  long v_1251X;
  long v_1250X;
  long thread_1249X;
  long v_1248X;
  long new_cont_1247X;
  char * top_1246X;
  long v_1245X;
  long x_1244X;
  char * addr_1243X;
  long a_1242X;
  char v_1241X;
  char * addr_1240X;
  char * addr_1239X;
  long val_1238X;
  long data_1237X;
  char * ptr_1236X;
  char * start_1235X;
  char * addr_1234X;
  char * addr_1233X;
  char * addr_1232X;
  char * addr_1231X;
  char * addr_1230X;
  char * addr_1229X;
  char * addr_1228X;
  char * addr_1227X;
  long temp_1226X;
  long thing_1225X;
  char * ptr_1224X;
  char v_1223X;
  char * addr_1222X;
  char temp_1221X;
  char v_1220X;
  char proxy_okayP_1219X;
  char * start_1218X;
  long y_1217X;
  long p_1216X;
  long old_decode_1215X;
  long old_proxy_1214X;
  char * ptr_1213X;
  long x2_1212X;
  char * ptr_1211X;
  char * start_1210X;
  char * next_1209X;
  long thing_1208X;
  char * addr_1207X;
  char * start_1206X;
  char * end_1205X;
  char * start_1204X;
  long code_1203X;
  char * x_1202X;
  char * end_1201X;
  char * v_1200X;
  char * other_begin_1199X;
  char * current_end_1198X;
  char * v_1197X;
  char * start_hp_1196X;
  long v_1195X;
  char * addr_1194X;
  char is_proxyP_1193X;
  char * addr_1192X;
  long value_1191X;
  char * addr_1190X;
  char * addr_1189X;
  long i_1188X;
  long next_1187X;
  long data_1186X;
  char * ptr_1185X;
  char * addr_1184X;
  long value_1183X;
  char * start_1182X;
  long i_1181X;
  long next_uid_1180X;
  long old_length_1179X;
  long new_length_1178X;
  long p_1177X;
  long new_vector_1176X;
  char * addr_1175X;
  long len_1174X;
  long new_length_1173X;
  char v_1172X;
  char v_1171X;
  long want_length_1170X;
  long x_1169X;
  long minimum_new_length_1168X;
  long old_length_1167X;
  long decode_vector_1166X;
  char * addr_1165X;
  long val_1164X;
  long send_1163X;
  long descriptor_1162X;
  char * start_1161X;
  char * old_1160X;
  long count_1159X;
  long owner_1158X;
  long descriptor_1157X;
  char * o_1156X;
  char * addr_1155X;
  long uid_1154X;
  long uid_1153X;
  long descriptor_1152X;
  long new_1151X;
  long uid_1150X;
  long p_1149X;
  long p_1148X;
  long p_1147X;
  char * new_hp_1146X;
  char * start_1145X;
  char * old_1144X;
  long new_1143X;
  long owner_1142X;
  long new_1141X;
  long data_1140X;
  char * addr_1139X;
  long uid_1138X;
  long uid_1137X;
  char * addr_1136X;
  long uid_1135X;
  long uid_1134X;
  char * addr_1133X;
  long uid_1132X;
  long uid_1131X;
  char * addr_1130X;
  long uid_1129X;
  long uid_1128X;
  char * addr_1127X;
  long uid_1126X;
  long uid_1125X;
  long temp_1124X;
  long h_1123X;
  char * addr_1122X;
  long next_1121X;
  char * start_1120X;
  char * old_1119X;
  char * addr_1118X;
  long next_1117X;
  long v_1116X;
  long extracted_1115X;
  long next_available_1114X;
  char * addr_1113X;
  long status_1112X;
  long v_1111X;
  long v_1110X;
  long v_1109X;
  long os_index_1108X;
  long v_1107X;
  long status_1106X;
  long id_1105X;
  long new_1104X;
  long id_1103X;
  long header_1102X;
  long channel_1101X;
  long tem_1100X;
  long i_1099X;
  char * addr_1098X;
  long l_1097X;
  long v_1096X;
  long env_1095X;
  long l2_1094X;
  long sickly_1093X;
  long okay_1092X;
  long value_1091X;
  long env_1090X;
  long last_env_1089X;
  long cont_1088X;
  long env_1087X;
  long v_1086X;
  long env_1085X;
  long p_1084X;
  long arg_count_1083X;
  long i_1082X;
  char * p_1081X;
  long unused_1080X;
  char * a_1079X;
  long v_1078X;
  long v_1077X;
  long env_1076X;
  long v_1075X;
  long v_1074X;
  long v_1073X;
  long v_1072X;
  long v_1071X;
  long v_1070X;
  long v_1069X;
  long v_1068X;
  long v_1067X;
  long v_1066X;
  long pc_1065X;
  char * addr_1064X;
  long x_1063X;
  char * addr_1062X;
  long a_1061X;
  long last_1060X;
  long l_1059X;
  long x_1058X;
  char * addr_1057X;
  long a_1056X;
  long list_1055X;
  long temp_1054X;
  char okayP_1053X;
  char v_1052X;
  long temp_1051X;
  char v_1050X;
  long space_1049X;
  long x_1048X;
  char * addr_1047X;
  long a_1046X;
  long count_1045X;
  long args_1044X;
  long start_1043X;
  long temp_1042X;
  char okayP_1041X;
  char v_1040X;
  long temp_1039X;
  char v_1038X;
  long space_1037X;
  long env_1036X;
  long cont_1035X;
  char * addr_1034X;
  long new_1033X;
  long p_1032X;
  long env_1031X;
  long top_1030X;
  char * addr_1029X;
  long new_1028X;
  char * data_addr_1027X;
  char * addr_1026X;
  long header_1025X;
  long new_1024X;
  char * data_addr_1023X;
  char * addr_1022X;
  long header_1021X;
  long p_1020X;
  long previous_1019X;
  long cont_1018X;
  long end_1017X;
  long v_1016X;
  long p_1015X;
  char * arg_1014X;
  char * loc_1013X;
  long stob_1012X;
  char * top_1011X;
  long arg_count_1010X;
  long i_1009X;
  char * p_1008X;
  long v_1007X;
  long l_1006X;
  long i_1005X;
  long key_1004X;
  char okayP_1003X;
  long list_1002X;
  char v_1001X;
  char v_1000X;
  long space_999X;
  long x_998X;
  long v_997X;
  long x_996X;
  long v_995X;
  long count_994X;
  long x_993X;
  long v_992X;
  char * addr_991X;
  char * addr_990X;
  long val_989X;
  long channel_988X;
  long status_987X;
  long channel_986X;
  long type_985X;
  char v_984X;
  long template_983X;
  char not_firstP_982X;
  long cont_981X;
  char not_firstP_980X;
  FILE * out_979X;
  long current_template_978X;
  long why_977X;
  char * addr_976X;
  long x_975X;
  char * addr_974X;
  long x_973X;
  char * addr_972X;
  char * addr_971X;
  long x_970X;
  char * addr_969X;
  long x_968X;
  char * addr_967X;
  char * addr_966X;
  long x_965X;
  char * addr_964X;
  char * addr_963X;
  long val_962X;
  long thing_961X;
  long x_960X;
  char * addr_959X;
  long b_958X;
  char * addr_957X;
  long value_956X;
  char * addr_955X;
  long value_954X;
  char * addr_953X;
  char * addr_952X;
  long value_951X;
  char * addr_950X;
  long value_949X;
  long vector_948X;
  char * addr_947X;
  char * stob_start_946X;
  char * addr_945X;
  long count_944X;
  long uid_943X;
  long aspace_uid_942X;
  long x_941X;
  long x_940X;
  long i_939X;
  long status_938X;
  long v_937X;
  long n_936X;
  long x_935X;
  char * addr_934X;
  long b_933X;
  long x_932X;
  char * addr_931X;
  long b_930X;
  long x_929X;
  char * addr_928X;
  long count_927X;
  long proxy_926X;
  long thing_925X;
  long proxy_vector_924X;
  long aspace_923X;
  long decode_vector_922X;
  long aspace_921X;
  long thing_920X;
  long x2_919X;
  long x_918X;
  long status_917X;
  char pendingP_916X;
  char eofP_915X;
  long got_914X;
  long length_913X;
  long i_912X;
  long v_911X;
  long reason_910X;
  long channel_909X;
  long c_908X;
  long b_907X;
  long v_906X;
  long obj_905X;
  long decode_vector_904X;
  long count_903X;
  long uid_902X;
  long aspace_uid_901X;
  long p_900X;
  long p_899X;
  long p_898X;
  char * data_addr_897X;
  long decode_vector_896X;
  long uid_895X;
  long aspace_uid_894X;
  long p_893X;
  long p_892X;
  char * data_addr_891X;
  long decode_vector_890X;
  long temp_889X;
  long data_888X;
  long thing_887X;
  char * addr_886X;
  char * addr_885X;
  long x_884X;
  char * addr_883X;
  long b_882X;
  long reason_881X;
  long *new_vm_channels_880X;
  long new_count_879X;
  long x_878X;
  char * addr_877X;
  long n_876X;
  long a_875X;
  long v_874X;
  long key_873X;
  char okayP_872X;
  char v_871X;
  char * addr_870X;
  long value_869X;
  long offset_868X;
  long i_867X;
  long count_866X;
  long y_865X;
  long val_864X;
  char * addr_863X;
  char * addr_862X;
  char * addr_861X;
  long result_860X;
  char * addr_859X;
  long low_count_proxies_858X;
  long missing_uids_857X;
  long thing_856X;
  char * next_855X;
  long thing_854X;
  char * addr_853X;
  char * addr_852X;
  char * addr_851X;
  long symbols_850X;
  long table_849X;
  char * addr_848X;
  long i_847X;
  long bucket_846X;
  long index_845X;
  long h_844X;
  long i_843X;
  long val_842X;
  long x_841X;
  long status_840X;
  char pendingP_839X;
  long got_838X;
  long length_837X;
  long count_836X;
  long start_835X;
  char waitP_834X;
  long x_833X;
  long status_832X;
  long channel_831X;
  long v_830X;
  long v_829X;
  long y_828X;
  long x_827X;
  char temp_826X;
  long index_825X;
  long a_824X;
  long q_823X;
  long final_stack_arg_count_822X;
  char v_821X;
  char v_820X;
  long space_819X;
  long stack_arg_count_818X;
  long stack_slots_817X;
  char x_816X;
  long channel_815X;
  long obj_814X;
  long arg_count_813X;
  long count_812X;
  long i_811X;
  long env_810X;
  char *v_809X;
  long y_808X;
  long len_807X;
  long s2_806X;
  char * addr_805X;
  long x_804X;
  char * addr_803X;
  long x_802X;
  char * addr_801X;
  long b_800X;
  long i_799X;
  long l_798X;
  char * next_797X;
  long value_796X;
  long x_795X;
  long x_794X;
  char * ptr_793X;
  long n_792X;
  long s_791X;
  long v_790X;
  long obj_789X;
  char v_788X;
  long i_787X;
  char * addr_786X;
  long value_785X;
  char * addr_784X;
  long value_783X;
  long i_782X;
  long x_781X;
  long status_780X;
  long reason_779X;
  long reason_778X;
  long x_777X;
  long obj_776X;
  long v_775X;
  long v_774X;
  long rest_list_773X;
  long i_772X;
  long n_771X;
  long args_770X;
  long skip_769X;
  long template_768X;
  long stack_arg_count_767X;
  long skip_766X;
  long skip_765X;
  long template_764X;
  long stack_arg_count_763X;
  long skip_762X;
  char * addr_761X;
  long next_760X;
  long channel_759X;
  long m_758X;
  long i_757X;
  long value_756X;
  long back_755X;
  long env_754X;
  long offset_753X;
  long i_752X;
  long x_751X;
  char * addr_750X;
  long a_749X;
  long env_748X;
  long v_747X;
  long p_746X;
  long key_745X;
  long bucket_744X;
  long index_743X;
  long h_742X;
  long i_741X;
  long string_740X;
  char * addr_739X;
  long len_738X;
  char * start_737X;
  char * limit_736X;
  long x_735X;
  long obj_734X;
  char temp_733X;
  char minutesP_732X;
  char firstP_731X;
  long symbols_730X;
  long table_729X;
  long x_728X;
  char * addr_727X;
  char * addr_726X;
  long vector_725X;
  char * addr_724X;
  long map_723X;
  long i_722X;
  long status_721X;
  long status_720X;
  long status_719X;
  long status_718X;
  long status_717X;
  long status_716X;
  FILE * port_715X;
  char x_714X;
  long x_713X;
  long obj_712X;
  long arg3_711X;
  long arg2_710X;
  long key_709X;
  char * addr_708X;
  char * addr_707X;
  long val_706X;
  char v_705X;
  long channel_704X;
  long count_703X;
  long start_702X;
  long obj_701X;
  long arg4_700X;
  long arg3_699X;
  long arg2_698X;
  long key_697X;
  long arg5_696X;
  long arg4_695X;
  long arg3_694X;
  long arg2_693X;
  long key_692X;
  long x_691X;
  long status_690X;
  long x_689X;
  long obj_688X;
  long key_687X;
  long mode_686X;
  long arg2_685X;
  long key_684X;
  long i_683X;
  long i_682X;
  char * addr_681X;
  long i_680X;
  long i_679X;
  long i_678X;
  long c_677X;
  long b_676X;
  long c_675X;
  long b_674X;
  long c_673X;
  long b_672X;
  long c_671X;
  long mid_c_670X;
  long v_669X;
  long v_668X;
  long lo_c_667X;
  long hi_b_666X;
  long hi_a_665X;
  long lo_b_664X;
  long lo_a_663X;
  long b_662X;
  long v_661X;
  long v_660X;
  long args_659X;
  long next_658X;
  long next_op_657X;
  long cont_656X;
  long index_655X;
  long length_654X;
  long wants_stack_args_653X;
  long skip_652X;
  long skip_651X;
  long stack_space_650X;
  long protocol_649X;
  long index_648X;
  long length_647X;
  long wants_stack_args_646X;
  long skip_645X;
  long skip_644X;
  long stack_space_643X;
  long protocol_642X;
  long n_641X;
  long tem_640X;
  long stack_arg_count_639X;
  long v_638X;
  long v_637X;
  long key_636X;
  long start_i_635X;
  long temp_634X;
  char okayP_633X;
  long key_632X;
  char okayP_631X;
  long obj_630X;
  long obj_629X;
  long type_628X;
  long thing_627X;
  long stuff_626X;
  char * addr_625X;
  long val_624X;
  long x_623X;
  long i_622X;
  long b_621X;
  long p_620X;
  long port_619X;
  long Kchar_618X;
  long x_617X;
  long i_616X;
  long b_615X;
  long p_614X;
  long p_613X;
  long port_612X;
  char * addr_611X;
  long val_610X;
  long x_609X;
  long i_608X;
  long b_607X;
  long p_606X;
  long p_605X;
  long port_604X;
  long list_603X;
  long head_602X;
  char move_slowP_601X;
  long slow_600X;
  long list_599X;
  long n_598X;
  long v_597X;
  long table_596X;
  long obj_595X;
  long arg2_594X;
  long n_593X;
  long arg2_592X;
  long h_591X;
  long i_590X;
  long val_589X;
  long arg3_588X;
  long arg2_587X;
  char v_586X;
  long aspace_585X;
  long pair_584X;
  long thing_583X;
  char v_582X;
  long x_581X;
  char * addr_580X;
  long x_579X;
  long obj_578X;
  long arg2_577X;
  long v_576X;
  long (*v_575X)(long, char *);
  long option_574X;
  long mseconds_573X;
  long seconds_572X;
  long option_571X;
  long proc_570X;
  long rest_list_569X;
  long x_568X;
  long tem_567X;
  long pc_566X;
  long v_565X;
  long vector_564X;
  long type_563X;
  char firstP_562X;
  long v_561X;
  long symbols_560X;
  char firstP_559X;
  long table_558X;
  long vector_557X;
  char firstP_556X;
  long x_555X;
  char * addr_554X;
  long b_553X;
  long x_552X;
  char * addr_551X;
  long x_550X;
  long arg2_549X;
  long key_548X;
  char okayP_547X;
  long val_546X;
  long y_545X;
  long n_544X;
  long prev_543X;
  long ch_542X;
  long val_541X;
  long key_540X;
  char okayP_539X;
  long key_538X;
  char okayP_537X;
  long key_536X;
  char okayP_535X;
  long key_534X;
  char okayP_533X;
  long val_532X;
  char * addr_531X;
  char * addr_530X;
  long val_529X;
  long string_528X;
  char * addr_527X;
  long len_526X;
  long vector_525X;
  char * addr_524X;
  long value_523X;
  char * addr_522X;
  long len_521X;
  long init_520X;
  char okayP_519X;
  long rest_list_518X;
  long stack_nargs_517X;
  long new_516X;
  char * addr_515X;
  long len_514X;
  long type_513X;
  long new_512X;
  char * addr_511X;
  long len_510X;
  long type_509X;
  long value_508X;
  long val_507X;
  long val_506X;
  long val_505X;
  long val_504X;
  long r_503X;
  long n_502X;
  long a_501X;
  long a_500X;
  long val_499X;
  long val_498X;
  long val_497X;
  long val_496X;
  long val_495X;
  long a_494X;
  long a_493X;
  long val_492X;
  long val_491X;
  long delta_490X;
  long delta_489X;
  long offset_488X;
  long key_487X;
  char okayP_486X;
  char v_485X;
  long tem_484X;
  long key_483X;
  char okayP_482X;
  long list_arg_count_481X;
  long list_args_480X;
  long stack_nargs_479X;
  long obj_478X;
  char * addr_477X;
  long list_args_476X;
  long follower_475X;
  long list_474X;
  long args_473X;
  long list_arg_count_472X;
  char okayP_471X;
  long stack_nargs_470X;
  long list_args_469X;
  long total_arg_count_468X;
  long code_467X;
  long obj_466X;
  long obj_465X;
  long list_arg_count_464X;
  long list_args_463X;
  long stack_arg_count_462X;
  long obj_461X;
  char * arg_460X;
  char * loc_459X;
  long args_458X;
  long v_457X;
  long v_456X;
  long v_455X;
  long list_arg_count_454X;
  long list_args_453X;
  long stack_arg_count_452X;
  long exception_451X;
  long code_450X;
  long stack_arg_count_449X;
  char v_448X;
  long key_447X;
  char okayP_446X;
  long key_445X;
  char okayP_444X;
  char * addr_443X;
  long value_442X;
  long v_441X;
  long new_env_440X;
  char * addr_439X;
  long len_438X;
  char v_437X;
  long temp_436X;
  char v_435X;
  long env_434X;
  char v_433X;
  char v_432X;
  long space_431X;
  char * addr_430X;
  long value_429X;
  long index_428X;
  long i_427X;
  long env_426X;
  long i_425X;
  long env_424X;
  long i_423X;
  long env_422X;
  long obj_421X;
  long opcode_420X;
  long nargs_419X;
  FILE * out_418X;
  long x_417X;
  long v_416X;
  long v_415X;
  long v_414X;
  long v_413X;
  long v_412X;
  long v_411X;
  long count_410X;
  long to_index_409X;
  long from_index_408X;
  long arg5_407X;
  long arg4_406X;
  long arg3_405X;
  long arg2_404X;
  char * addr_403X;
  long len_402X;
  long index_401X;
  long x_400X;
  long arg4_399X;
  long arg3_398X;
  long arg2_397X;
  long len_396X;
  long index_395X;
  long arg3_394X;
  long arg2_393X;
  long x_392X;
  long arg2_391X;
  char v_390X;
  char v_389X;
  char v_388X;
  char v_387X;
  long space_386X;
  long n_385X;
  long x_384X;
  long obj_383X;
  long len_382X;
  long x_381X;
  long obj_380X;
  long arg2_379X;
  char v_378X;
  char v_377X;
  long space_376X;
  char v_375X;
  char v_374X;
  long arg2_373X;
  long status_372X;
  long key_371X;
  long x_370X;
  long arg2_369X;
  long mseconds_368X;
  long seconds_367X;
  long mseconds_366X;
  long seconds_365X;
  long x_364X;
  long option_363X;
  long x_362X;
  long arg2_361X;
  long rest_list_360X;
  long nargs_359X;
  char v_358X;
  long name_357X;
  long value_356X;
  long x_355X;
  long obj_354X;
  long x_353X;
  long arg2_352X;
  long p_351X;
  long p_350X;
  long p_349X;
  long old_348X;
  long temp_347X;
  long obj_346X;
  long p_345X;
  long template_344X;
  long p_343X;
  long temp_342X;
  long obj_341X;
  long x_340X;
  long x_339X;
  long obj_338X;
  long type_337X;
  char v_336X;
  char v_335X;
  long x_334X;
  long bytes_333X;
  long x_332X;
  long key_331X;
  long x_330X;
  long arg2_329X;
  char v_328X;
  char v_327X;
  char v_326X;
  char v_325X;
  long x_324X;
  long value_323X;
  long x_322X;
  long obj_321X;
  long x_320X;
  long obj_319X;
  char * addr_318X;
  long next_317X;
  long channel_316X;
  long n_315X;
  long head_314X;
  long channel_313X;
  long obj_312X;
  char v_311X;
  char v_310X;
  char v_309X;
  char v_308X;
  char v_307X;
  char v_306X;
  char v_305X;
  char v_304X;
  long x_303X;
  long x_302X;
  long x_301X;
  long arg2_300X;
  long descriptor_299X;
  long x_298X;
  long obj_297X;
  long len_296X;
  char Kchar_295X;
  long index_294X;
  long arg3_293X;
  long arg2_292X;
  long len_291X;
  long index_290X;
  long arg2_289X;
  long obj_288X;
  char v_287X;
  char v_286X;
  long size_285X;
  char init_284X;
  long len_283X;
  long arg2_282X;
  long len_281X;
  long Kchar_280X;
  long index_279X;
  long arg3_278X;
  long arg2_277X;
  long len_276X;
  long index_275X;
  long arg2_274X;
  long obj_273X;
  char v_272X;
  char v_271X;
  long size_270X;
  long init_269X;
  long len_268X;
  long arg2_267X;
  char * addr_266X;
  long len_265X;
  long index_264X;
  long x_263X;
  long arg3_262X;
  long arg2_261X;
  char * addr_260X;
  long index_259X;
  long len_258X;
  long type_257X;
  long x_256X;
  long arg3_255X;
  long arg2_254X;
  long index_253X;
  long len_252X;
  long type_251X;
  long x_250X;
  long arg2_249X;
  char v_248X;
  long temp_247X;
  char v_246X;
  long size_245X;
  long len_244X;
  long type_243X;
  long x_242X;
  long arg2_241X;
  char * addr_240X;
  long type_239X;
  long offset_238X;
  long x_237X;
  long arg2_236X;
  long type_235X;
  long offset_234X;
  long x_233X;
  char v_232X;
  char v_231X;
  long space_230X;
  long len_229X;
  char v_228X;
  char v_227X;
  long space_226X;
  long len_225X;
  long type_224X;
  long x_223X;
  long type_222X;
  long x_221X;
  long x_220X;
  long x_219X;
  long x_218X;
  long arg2_217X;
  long x_216X;
  long arg2_215X;
  long x_214X;
  long x_213X;
  long result_212X;
  long x_211X;
  long x_210X;
  long count_209X;
  long value_208X;
  long x_207X;
  long arg2_206X;
  long arg2_205X;
  long arg2_204X;
  long arg2_203X;
  long x_202X;
  long arg2_201X;
  long x_200X;
  long arg2_199X;
  long x_198X;
  long arg2_197X;
  long x_196X;
  long x_195X;
  long x_194X;
  long x_193X;
  long x_192X;
  long x_191X;
  long x_190X;
  long x_189X;
  long x_188X;
  long x_187X;
  long a_186X;
  long x_185X;
  long arg2_184X;
  long b_183X;
  long a_182X;
  long x_181X;
  long arg2_180X;
  long x_179X;
  long arg2_178X;
  long x_177X;
  long arg2_176X;
  long x_175X;
  long arg2_174X;
  long x_173X;
  long arg2_172X;
  long x_171X;
  long arg2_170X;
  long b_169X;
  long a_168X;
  long y_167X;
  long arg2_166X;
  long z_165X;
  long x_164X;
  long arg2_163X;
  long b_162X;
  long a_161X;
  long y_160X;
  long arg2_159X;
  long z_158X;
  long x_157X;
  long arg2_156X;
  long x_155X;
  long x_154X;
  long x_153X;
  long x_152X;
  long x_151X;
  long x_150X;
  long x_149X;
  long x_148X;
  long arg2_147X;
  long rest_list_146X;
  long x_145X;
  long rest_list_144X;
  long stack_nargs_143X;
  long arg0_142X;
  long arg1_141X;
  long rest_list_140X;
  long arg0_139X;
  long stack_nargs_138X;
  long index_137X;
  long val_136X;
  long max_135X;
  long p_134X;
  char v_133X;
  char v_132X;
  long space_131X;
  long space_130X;
  long index_129X;
  long v_128X;
  char v_127X;
  char v_126X;
  long space_125X;
  long code_124X;
  long tem_123X;
  long stack_nargs_122X;
  long p_121X;
  long obj_120X;
  long consumer_119X;
  long cont_118X;
  long rest_list_117X;
  long proc_116X;
  long stack_nargs_115X;
  long args_114X;
  long length_113X;
  char okayP_112X;
  long list_args_111X;
  long stob_110X;
  char * top_of_args_109X;
  long nargs_108X;
  long obj_107X;
  long stack_arg_count_106X;
  char v_105X;
  char v_104X;
  long space_103X;
  long code_102X;
  long template_101X;
  long obj_100X;
  long stack_arg_count_99X;
  long cont_98X;
  char v_97X;
  char v_96X;
  long space_95X;
  char v_94X;
  char v_93X;
  long space_92X;
  long total_count_91X;
  long p_90X;
  long v_89X;
  char * addr_88X;
  long val_87X;
  long location_86X;
  long location_85X;
  long env_84X;
  long back_83X;
  long env_82X;
  long back_81X;
  long env_80X;
  long back_79X;
  long count_78X;
  char * code_pointer_77X;
 {  SvalS = value_36X;
  goto L20074;}
 L20074: {
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L19170: {
  code_pointer_77X = arg1K0;
  switch ((*((unsigned char *) code_pointer_77X))) {
    case 0 : {
      merged_arg0K0 = 15;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 0;
      goto push_exception_continuationB;
     push_exception_continuationB_return_0:
      arg0K0 = 0;
      goto L17068;}
      break;
    case 1 : {
      count_78X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      *((long *) (SstackS)) = (SenvS);
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (1034 + (((count_78X)<<10)));
      SstackS = ((SstackS) + -4);
      SenvS = (3 + (((long) ((SstackS) + 8))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 2 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 3 : {
      SvalS = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2)))));
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 4 : {
      back_79X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_80X = SenvS;
      arg0K0 = env_80X;
      arg0K1 = back_79X;
      goto L24647;}
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
        goto L17068;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
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
        goto L17068;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
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
        goto L17068;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 2);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
      break;
    case 8 : {
      back_81X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_82X = SenvS;
      arg0K0 = env_82X;
      arg0K1 = back_81X;
      goto L20936;}
      break;
    case 9 : {
      back_83X = *((unsigned char *) ((Scode_pointerS) + 1));
      env_84X = SenvS;
      arg0K0 = env_84X;
      arg0K1 = back_83X;
      goto L20874;}
      break;
    case 10 : {
      location_85X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      SvalS = (*((long *) ((((char *) (-3 + location_85X))) + 8)));
      if ((17 == (255 & (SvalS)))) {
        merged_arg0K0 = 1;
        merged_arg0K1 = 3;
        push_exception_continuationB_return_tag = 4;
        goto push_exception_continuationB;
       push_exception_continuationB_return_4:
        *((long *) (SstackS)) = location_85X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
      break;
    case 11 : {
      location_86X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      if ((273 == (*((long *) ((((char *) (-3 + location_86X))) + 8))))) {
        merged_arg0K0 = 1;
        merged_arg0K1 = 3;
        push_exception_continuationB_return_tag = 5;
        goto push_exception_continuationB;
       push_exception_continuationB_return_5:
        *((long *) (SstackS)) = location_86X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}
      else {
        val_87X = SvalS;
        addr_88X = (((char *) (-3 + location_86X))) + 8;
        WRITE_BARRIER(addr_88X, val_87X);
        *((long *) addr_88X) = val_87X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
      break;
    case 12 : {
      v_89X = *((unsigned char *) ((Scode_pointerS) + 4));
      if ((0 == v_89X)) {
        p_90X = SenvS;
        if ((3 == (3 & p_90X))) {
          if ((p_90X < (((long) (Sstack_beginS))))) {
            arg0K0 = 0;
            goto L21732;}
          else {
            if (((((long) (Sstack_endS))) < p_90X)) {
              arg0K0 = 0;
              goto L21732;}
            else {
              arg0K0 = ((Sstack_endS) - (Sstack_beginS));
              goto L21732;}}}
        else {
          arg0K0 = 0;
          goto L21732;}}
      else {
        arg0K0 = (SvalS);
        goto L21739;}}
      break;
    case 13 : {
      total_count_91X = *((unsigned char *) ((Scode_pointerS) + 2));
      space_92X = 1 + total_count_91X;
      v_93X = AVAILABLEp(space_92X);
      if (v_93X) {
        goto L21535;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 0;
        goto collect_saving_temp;
       collect_saving_temp_return_0:
        v_94X = AVAILABLEp(space_92X);
        if (v_94X) {
          goto L21535;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L21535;}}}
      break;
    case 14 : {
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
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
        goto L17068;}
      else {
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
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
        goto L17068;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
      break;
    case 17 : {
      SstackS = ((SstackS) + 4);
      SvalS = (*((long *) (SstackS)));
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 18 : {
      SvalS = (*((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))));
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 19 : {
      *((long *) ((SstackS) + (4 + ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<2))))) = (SvalS);
      Scode_pointerS = ((Scode_pointerS) + 2);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 20 : {
      merged_arg1K0 = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      merged_arg0K1 = (*((unsigned char *) ((Scode_pointerS) + 3)));
      push_continuationB_return_tag = 0;
      goto push_continuationB;
     push_continuationB_return_0:
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 21 : {
      merged_arg1K0 = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      merged_arg0K1 = (((((*((unsigned char *) ((Scode_pointerS) + 3))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 4))));
      push_continuationB_return_tag = 1;
      goto push_continuationB;
     push_continuationB_return_1:
      Scode_pointerS = ((Scode_pointerS) + 5);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 22 : {
      space_95X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
      v_96X = AVAILABLEp(space_95X);
      if (v_96X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L21462;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 1;
        goto collect_saving_temp;
       collect_saving_temp_return_1:
        v_97X = AVAILABLEp(space_95X);
        if (v_97X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L21462;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L21462;}}}
      break;
    case 23 : {
      cont_98X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
      if ((3 == (3 & cont_98X))) {
        if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_98X))) + -4))))>>2))))) {
          if ((3 == (3 & cont_98X))) {
            if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_98X))) + -4))))>>2))))) {
              merged_arg0K0 = cont_98X;
              copy_continuation_from_heapB_return_tag = 0;
              goto copy_continuation_from_heapB;
             copy_continuation_from_heapB_return_0:
              goto L20534;}
            else {
              goto L20571;}}
          else {
            goto L20571;}}
        else {
          goto L20535;}}
      else {
        goto L20535;}}
      break;
    case 24 : {
      stack_arg_count_99X = *((unsigned char *) ((Scode_pointerS) + 1));
      obj_100X = SvalS;
      if ((3 == (3 & obj_100X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_100X))) + -4))))>>2))))) {
          template_101X = *((long *) (((char *) (-3 + (SvalS)))));
          code_102X = *((long *) (((char *) (-3 + template_101X))));
          if ((stack_arg_count_99X == (*((unsigned char *) ((((char *) (-3 + code_102X))) + 1))))) {
            StemplateS = template_101X;
            SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
            Scode_pointerS = ((((char *) (-3 + code_102X))) + 2);
            if (((SstackS) < (Sstack_limitS))) {
              space_103X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
              v_104X = AVAILABLEp(space_103X);
              if (v_104X) {
                arg2K0 = 1;
                arg0K1 = 0;
                goto L19938;}
              else {
                merged_arg0K0 = 0;
                collect_saving_temp_return_tag = 2;
                goto collect_saving_temp;
               collect_saving_temp_return_2:
                v_105X = AVAILABLEp(space_103X);
                if (v_105X) {
                  arg2K0 = 1;
                  arg0K1 = 0;
                  goto L19938;}
                else {
                  arg2K0 = 0;
                  arg0K1 = 0;
                  goto L19938;}}}
            else {
              goto L19850;}}
          else {
            arg0K0 = stack_arg_count_99X;
            goto L16469;}}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_99X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L17564;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_99X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17564;}}
      break;
    case 25 : {
      stack_arg_count_106X = ((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)));
      obj_107X = SvalS;
      if ((3 == (3 & obj_107X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_107X))) + -4))))>>2))))) {
          arg0K0 = stack_arg_count_106X;
          goto L16469;}
        else {
          arg0K0 = 3;
          arg0K1 = stack_arg_count_106X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L17564;}}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_106X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17564;}}
      break;
    case 26 : {
      nargs_108X = *((unsigned char *) ((Scode_pointerS) + 1));
      top_of_args_109X = SstackS;
      stob_110X = ScontS;
      arg1K0 = ((((char *) (-3 + stob_110X))) + -8);
      arg1K1 = (top_of_args_109X + (((nargs_108X)<<2)));
      goto L18934;}
      break;
    case 27 : {
      SstackS = ((SstackS) + 4);
      list_args_111X = *((long *) (SstackS));
      merged_arg0K0 = list_args_111X;
      okay_argument_list_return_tag = 0;
      goto okay_argument_list;
     okay_argument_list_return_0:
      okayP_112X = okay_argument_list0_return_value;
      length_113X = okay_argument_list1_return_value;
      if (okayP_112X) {
        arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
        arg0K1 = list_args_111X;
        arg0K2 = length_113X;
        goto L18972;}
      else {
        *((long *) (SstackS)) = list_args_111X;
        SstackS = ((SstackS) + -4);
        merged_arg0K0 = 25;
        merged_arg0K1 = (1 + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        pop_args_GlistS_return_tag = 0;
        goto pop_args_GlistS;
       pop_args_GlistS_return_0:
        args_114X = pop_args_GlistS0_return_value;
        merged_arg0K0 = 5;
        merged_arg0K1 = 0;
        push_exception_continuationB_return_tag = 8;
        goto push_exception_continuationB;
       push_exception_continuationB_return_8:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = args_114X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 28 : {
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      stack_nargs_115X = (((*((long *) (SstackS))))>>2);
      proc_116X = *((long *) ((SstackS) + (4 + (((stack_nargs_115X)<<2)))));
      *((long *) ((SstackS) + (4 + (((stack_nargs_115X)<<2))))) = 1;
      SstackS = ((SstackS) + 4);
      rest_list_117X = *((long *) (SstackS));
      if ((25 == rest_list_117X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = (-2 + stack_nargs_115X);
        goto L11608;}
      else {
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_117X))) + 4))))) {
          arg0K0 = (*((long *) (((char *) (-3 + rest_list_117X)))));
          arg0K1 = (-1 + stack_nargs_115X);
          goto L11608;}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + rest_list_117X))) + 4)))))) + 4)));
          arg0K1 = rest_list_117X;
          goto L11649;}}}
      break;
    case 29 : {
      SstackS = ((SstackS) + 4);
      cont_118X = *((long *) (SstackS));
      if ((3 == (3 & cont_118X))) {
        if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_118X))) + -4))))>>2))))) {
          merged_arg0K0 = cont_118X;
          copy_continuation_from_heapB_return_tag = 1;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_1:
          goto L18877;}
        else {
          goto L18884;}}
      else {
        goto L18884;}}
      break;
    case 30 : {
      SstackS = ((SstackS) + 4);
      consumer_119X = *((long *) (SstackS));
      *((long *) (SstackS)) = (SvalS);
      SstackS = ((SstackS) + -4);
      SvalS = consumer_119X;
      obj_120X = SvalS;
      if ((3 == (3 & obj_120X))) {
        if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_120X))) + -4))))>>2))))) {
          arg0K0 = 1;
          goto L16469;}
        else {
          arg0K0 = 3;
          arg0K1 = 1;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L17564;}}
      else {
        arg0K0 = 3;
        arg0K1 = 1;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17564;}}
      break;
    case 31 : {
      pop_continuationB_return_tag = 0;
      goto pop_continuationB;
     pop_continuationB_return_0:
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 32 : {
      arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2))));
      arg0K1 = 25;
      arg0K2 = 0;
      goto L21104;}
      break;
    case 33 : {
      SstackS = ((SstackS) + 4);
      p_121X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      stack_nargs_122X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      arg0K0 = stack_nargs_122X;
      arg0K1 = (*((long *) (SstackS)));
      arg0K2 = ((((p_121X)>>2)) - stack_nargs_122X);
      goto L21104;}
      break;
    case 34 : {
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 35 : {
      tem_123X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
      StemplateS = tem_123X;
      Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_123X))))))));
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 36 : {
      code_124X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))))))));
      if ((0 == (*((unsigned char *) ((((char *) (-3 + code_124X))) + 1))))) {
        if (((SstackS) < (Sstack_limitS))) {
          space_125X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
          v_126X = AVAILABLEp(space_125X);
          if (v_126X) {
            arg2K0 = 1;
            arg0K1 = 0;
            goto L19676;}
          else {
            merged_arg0K0 = 0;
            collect_saving_temp_return_tag = 3;
            goto collect_saving_temp;
           collect_saving_temp_return_3:
            v_127X = AVAILABLEp(space_125X);
            if (v_127X) {
              arg2K0 = 1;
              arg0K1 = 0;
              goto L19676;}
            else {
              arg2K0 = 0;
              arg0K1 = 0;
              goto L19676;}}}
        else {
          goto L19549;}}
      else {
        v_128X = *((unsigned char *) ((((char *) (-3 + code_124X))) + 1));
        if ((66 == v_128X)) {
          if ((0 == (*((unsigned char *) ((((char *) (-3 + code_124X))) + (-3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_124X))) + -4))))>>8)))))))) {
            index_129X = -2 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + code_124X))) + -4))))>>8));
            space_130X = ((((*((unsigned char *) ((((char *) (-3 + code_124X))) + index_129X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_124X))) + (1 + index_129X))));
            if ((space_130X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
              goto L19549;}
            else {
              space_131X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
              v_132X = AVAILABLEp(space_131X);
              if (v_132X) {
                arg2K0 = 1;
                arg0K1 = 0;
                goto L19731;}
              else {
                merged_arg0K0 = 0;
                collect_saving_temp_return_tag = 4;
                goto collect_saving_temp;
               collect_saving_temp_return_4:
                v_133X = AVAILABLEp(space_131X);
                if (v_133X) {
                  arg2K0 = 1;
                  arg0K1 = 0;
                  goto L19731;}
                else {
                  arg2K0 = 0;
                  arg0K1 = 0;
                  goto L19731;}}}}
          else {
            goto L19594;}}
        else {
          goto L19594;}}}
      break;
    case 37 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        Scode_pointerS = ((Scode_pointerS) + 3);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
      break;
    case 38 : {
      Scode_pointerS = ((Scode_pointerS) + (((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))));
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 39 : {
      if ((0 == (3 & (SvalS)))) {
        p_134X = SvalS;
        max_135X = *((unsigned char *) ((Scode_pointerS) + 1));
        val_136X = ((p_134X)>>2);
        if ((val_136X < 0)) {
          goto L19450;}
        else {
          if ((val_136X < max_135X)) {
            index_137X = 1 + (((val_136X)<<1));
            arg0K0 = (((((*((unsigned char *) ((Scode_pointerS) + (1 + index_137X)))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + (2 + index_137X)))));
            goto L19452;}
          else {
            goto L19450;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 0;
        push_exception_continuationB_return_tag = 9;
        goto push_exception_continuationB;
       push_exception_continuationB_return_9:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 40 : {
      stack_nargs_138X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((0 == stack_nargs_138X)) {
        arg0_139X = *((long *) ((SstackS) + 12));
        rest_list_140X = *((long *) ((SstackS) + 8));
        *((long *) (SstackS)) = arg0_139X;
        SstackS = ((SstackS) + -4);
        SvalS = (*((long *) (((char *) (-3 + rest_list_140X)))));
        goto L20437;}
      else {
        arg1_141X = *((long *) ((SstackS) + (4 + (((stack_nargs_138X)<<2)))));
        arg0_142X = *((long *) ((SstackS) + (8 + (((stack_nargs_138X)<<2)))));
        *((long *) ((SstackS) + (8 + (((stack_nargs_138X)<<2))))) = 1;
        *((long *) ((SstackS) + 4)) = (-4 + (((stack_nargs_138X)<<2)));
        *((long *) (SstackS)) = arg0_142X;
        SstackS = ((SstackS) + -4);
        SvalS = arg1_141X;
        goto L20437;}}
      break;
    case 41 : {
      stack_nargs_143X = (((*((long *) ((SstackS) + 4))))>>2);
      if ((stack_nargs_143X == 0)) {
        rest_list_144X = *((long *) ((SstackS) + 8));
        if ((25 == (*((long *) ((((char *) (-3 + rest_list_144X))) + 4))))) {
          arg0K0 = 1;
          goto L19348;}
        else {
          *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_144X))) + 4)));
          *((long *) ((SstackS) + 12)) = (SvalS);
          arg0K0 = -2;
          goto L19348;}}
      else {
        if ((stack_nargs_143X == 1)) {
          if ((25 == (*((long *) ((SstackS) + 8))))) {
            arg0K0 = 1;
            goto L19348;}
          else {
            *((long *) ((SstackS) + 4)) = 0;
            *((long *) ((SstackS) + 12)) = (SvalS);
            arg0K0 = -2;
            goto L19348;}}
        else {
          *((long *) ((SstackS) + (8 + (((stack_nargs_143X)<<2))))) = (SvalS);
          arg0K0 = -2;
          goto L19348;}}}
      break;
    case 42 : {
      if ((1 == (SvalS))) {
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        x_145X = (((*((long *) ((SstackS) + 4))))>>2);
        if ((x_145X == 0)) {
          rest_list_146X = *((long *) ((SstackS) + 8));
          if ((25 == (*((long *) ((((char *) (-3 + rest_list_146X))) + 4))))) {
            arg0K0 = 1;
            goto L20328;}
          else {
            *((long *) ((SstackS) + 8)) = (*((long *) ((((char *) (-3 + rest_list_146X))) + 4)));
            *((long *) ((SstackS) + 12)) = (*((long *) (((char *) (-3 + rest_list_146X)))));
            arg0K0 = -2;
            goto L20328;}}
        else {
          if ((x_145X == 1)) {
            if ((25 == (*((long *) ((SstackS) + 8))))) {
              arg0K0 = 1;
              goto L20328;}
            else {
              *((long *) ((SstackS) + 4)) = 0;
              arg0K0 = -2;
              goto L20328;}}
          else {
            arg0K0 = -2;
            goto L20328;}}}}
      break;
    case 43 : {
      SstackS = ((SstackS) + 4);
      arg2_147X = *((long *) (SstackS));
      x_148X = SvalS;
      if ((arg2_147X == x_148X)) {
        arg0K0 = 5;
        goto L38594;}
      else {
        arg0K0 = 1;
        goto L38594;}}
      break;
    case 44 : {
      x_149X = SvalS;
      if ((0 == (3 & x_149X))) {
        arg0K0 = 5;
        goto L30584;}
      else {
        if ((3 == (3 & x_149X))) {
          if ((22 == (31 & ((((*((long *) ((((char *) (-3 + x_149X))) + -4))))>>2))))) {
            arg0K0 = 5;
            goto L30584;}
          else {
            goto L30539;}}
        else {
          goto L30539;}}}
      break;
    case 45 : {
      x_150X = SvalS;
      if ((0 == (3 & x_150X))) {
        goto L28870;}
      else {
        if ((3 == (3 & x_150X))) {
          if ((22 == (31 & ((((*((long *) ((((char *) (-3 + x_150X))) + -4))))>>2))))) {
            goto L28870;}
          else {
            goto L28873;}}
        else {
          goto L28873;}}}
      break;
    case 46 : {
      x_151X = SvalS;
      if ((0 == (3 & x_151X))) {
        goto L28679;}
      else {
        if ((3 == (3 & x_151X))) {
          if ((22 == (31 & ((((*((long *) ((((char *) (-3 + x_151X))) + -4))))>>2))))) {
            goto L28679;}
          else {
            goto L28662;}}
        else {
          goto L28662;}}}
      break;
    case 47 : {
      x_152X = SvalS;
      if ((0 == (3 & x_152X))) {
        goto L28468;}
      else {
        if ((3 == (3 & x_152X))) {
          if ((22 == (31 & ((((*((long *) ((((char *) (-3 + x_152X))) + -4))))>>2))))) {
            goto L28468;}
          else {
            goto L28451;}}
        else {
          goto L28451;}}}
      break;
    case 48 : {
      x_153X = SvalS;
      if ((0 == (3 & x_153X))) {
        goto L28257;}
      else {
        if ((3 == (3 & x_153X))) {
          if ((22 == (31 & ((((*((long *) ((((char *) (-3 + x_153X))) + -4))))>>2))))) {
            goto L28257;}
          else {
            goto L28240;}}
        else {
          goto L28240;}}}
      break;
    case 49 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = 5;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 10;
        goto push_exception_continuationB;
       push_exception_continuationB_return_10:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 50 : {
      if ((0 == (3 & (SvalS)))) {
        x_154X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 11;
        goto push_exception_continuationB;
       push_exception_continuationB_return_11:
        *((long *) (SstackS)) = x_154X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 12;
        goto push_exception_continuationB;
       push_exception_continuationB_return_12:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 51 : {
      if ((0 == (3 & (SvalS)))) {
        x_155X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 13;
        goto push_exception_continuationB;
       push_exception_continuationB_return_13:
        *((long *) (SstackS)) = x_155X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 14;
        goto push_exception_continuationB;
       push_exception_continuationB_return_14:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 52 : {
      SstackS = ((SstackS) + 4);
      arg2_156X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_156X | (SvalS))))) {
        x_157X = SvalS;
        z_158X = (((arg2_156X)>>2)) + (((x_157X)>>2));
        if ((536870911 < z_158X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 15;
          goto push_exception_continuationB;
         push_exception_continuationB_return_15:
          *((long *) (SstackS)) = arg2_156X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_157X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}
        else {
          if ((z_158X < -536870912)) {
            merged_arg0K0 = 6;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 16;
            goto push_exception_continuationB;
           push_exception_continuationB_return_16:
            *((long *) (SstackS)) = arg2_156X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_157X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L17068;}
          else {
            SvalS = (((z_158X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L19170;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 17;
        goto push_exception_continuationB;
       push_exception_continuationB_return_17:
        *((long *) (SstackS)) = arg2_156X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 53 : {
      SstackS = ((SstackS) + 4);
      arg2_159X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_159X | (SvalS))))) {
        y_160X = SvalS;
        a_161X = ((arg2_159X)>>2);
        b_162X = ((y_160X)>>2);
        if ((a_161X < 0)) {
          arg0K0 = (0 - a_161X);
          goto L7132;}
        else {
          arg0K0 = a_161X;
          goto L7132;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 18;
        goto push_exception_continuationB;
       push_exception_continuationB_return_18:
        *((long *) (SstackS)) = arg2_159X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 54 : {
      SstackS = ((SstackS) + 4);
      arg2_163X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_163X | (SvalS))))) {
        x_164X = SvalS;
        z_165X = (((arg2_163X)>>2)) - (((x_164X)>>2));
        if ((536870911 < z_165X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 19;
          goto push_exception_continuationB;
         push_exception_continuationB_return_19:
          *((long *) (SstackS)) = arg2_163X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_164X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}
        else {
          if ((z_165X < -536870912)) {
            merged_arg0K0 = 6;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 20;
            goto push_exception_continuationB;
           push_exception_continuationB_return_20:
            *((long *) (SstackS)) = arg2_163X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_164X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L17068;}
          else {
            SvalS = (((z_165X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L19170;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 21;
        goto push_exception_continuationB;
       push_exception_continuationB_return_21:
        *((long *) (SstackS)) = arg2_163X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 55 : {
      SstackS = ((SstackS) + 4);
      arg2_166X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_166X | (SvalS))))) {
        y_167X = SvalS;
        if ((0 == y_167X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 22;
          goto push_exception_continuationB;
         push_exception_continuationB_return_22:
          *((long *) (SstackS)) = arg2_166X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = y_167X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}
        else {
          a_168X = ((arg2_166X)>>2);
          b_169X = ((y_167X)>>2);
          if ((a_168X < 0)) {
            arg0K0 = (0 - a_168X);
            goto L8910;}
          else {
            arg0K0 = a_168X;
            goto L8910;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 23;
        goto push_exception_continuationB;
       push_exception_continuationB_return_23:
        *((long *) (SstackS)) = arg2_166X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 56 : {
      SstackS = ((SstackS) + 4);
      arg2_170X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_170X | (SvalS))))) {
        x_171X = SvalS;
        if ((arg2_170X == x_171X)) {
          arg0K0 = 5;
          goto L30435;}
        else {
          arg0K0 = 1;
          goto L30435;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 24;
        goto push_exception_continuationB;
       push_exception_continuationB_return_24:
        *((long *) (SstackS)) = arg2_170X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 57 : {
      SstackS = ((SstackS) + 4);
      arg2_172X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_172X | (SvalS))))) {
        x_173X = SvalS;
        if ((arg2_172X < x_173X)) {
          arg0K0 = 5;
          goto L30345;}
        else {
          arg0K0 = 1;
          goto L30345;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 25;
        goto push_exception_continuationB;
       push_exception_continuationB_return_25:
        *((long *) (SstackS)) = arg2_172X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 58 : {
      SstackS = ((SstackS) + 4);
      arg2_174X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_174X | (SvalS))))) {
        x_175X = SvalS;
        if ((x_175X < arg2_174X)) {
          arg0K0 = 5;
          goto L30255;}
        else {
          arg0K0 = 1;
          goto L30255;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 26;
        goto push_exception_continuationB;
       push_exception_continuationB_return_26:
        *((long *) (SstackS)) = arg2_174X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 59 : {
      SstackS = ((SstackS) + 4);
      arg2_176X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_176X | (SvalS))))) {
        x_177X = SvalS;
        if ((x_177X < arg2_176X)) {
          arg0K0 = 1;
          goto L30150;}
        else {
          arg0K0 = 5;
          goto L30150;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 27;
        goto push_exception_continuationB;
       push_exception_continuationB_return_27:
        *((long *) (SstackS)) = arg2_176X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 60 : {
      SstackS = ((SstackS) + 4);
      arg2_178X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_178X | (SvalS))))) {
        x_179X = SvalS;
        if ((arg2_178X < x_179X)) {
          arg0K0 = 1;
          goto L30043;}
        else {
          arg0K0 = 5;
          goto L30043;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 28;
        goto push_exception_continuationB;
       push_exception_continuationB_return_28:
        *((long *) (SstackS)) = arg2_178X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 61 : {
      SstackS = ((SstackS) + 4);
      arg2_180X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_180X | (SvalS))))) {
        x_181X = SvalS;
        if ((0 == x_181X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 29;
          goto push_exception_continuationB;
         push_exception_continuationB_return_29:
          *((long *) (SstackS)) = arg2_180X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_181X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}
        else {
          a_182X = ((arg2_180X)>>2);
          b_183X = ((x_181X)>>2);
          if ((a_182X < 0)) {
            arg0K0 = (0 - a_182X);
            goto L34164;}
          else {
            arg0K0 = a_182X;
            goto L34164;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 30;
        goto push_exception_continuationB;
       push_exception_continuationB_return_30:
        *((long *) (SstackS)) = arg2_180X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 62 : {
      SstackS = ((SstackS) + 4);
      arg2_184X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_184X | (SvalS))))) {
        x_185X = SvalS;
        if ((0 == x_185X)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 31;
          goto push_exception_continuationB;
         push_exception_continuationB_return_31:
          *((long *) (SstackS)) = arg2_184X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_185X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}
        else {
          a_186X = ((arg2_184X)>>2);
          if ((a_186X < 0)) {
            arg0K0 = (0 - a_186X);
            goto L34032;}
          else {
            arg0K0 = a_186X;
            goto L34032;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 32;
        goto push_exception_continuationB;
       push_exception_continuationB_return_32:
        *((long *) (SstackS)) = arg2_184X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 63 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = (SvalS);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 33;
        goto push_exception_continuationB;
       push_exception_continuationB_return_33:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 64 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = (SvalS);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 34;
        goto push_exception_continuationB;
       push_exception_continuationB_return_34:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 65 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = 4;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 35;
        goto push_exception_continuationB;
       push_exception_continuationB_return_35:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 66 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = (SvalS);
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 36;
        goto push_exception_continuationB;
       push_exception_continuationB_return_36:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 67 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = 0;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 37;
        goto push_exception_continuationB;
       push_exception_continuationB_return_37:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 68 : {
      if ((0 == (3 & (SvalS)))) {
        x_187X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 38;
        goto push_exception_continuationB;
       push_exception_continuationB_return_38:
        *((long *) (SstackS)) = x_187X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 39;
        goto push_exception_continuationB;
       push_exception_continuationB_return_39:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 69 : {
      if ((0 == (3 & (SvalS)))) {
        x_188X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 40;
        goto push_exception_continuationB;
       push_exception_continuationB_return_40:
        *((long *) (SstackS)) = x_188X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 41;
        goto push_exception_continuationB;
       push_exception_continuationB_return_41:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 70 : {
      if ((0 == (3 & (SvalS)))) {
        x_189X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 42;
        goto push_exception_continuationB;
       push_exception_continuationB_return_42:
        *((long *) (SstackS)) = x_189X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 43;
        goto push_exception_continuationB;
       push_exception_continuationB_return_43:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 71 : {
      if ((0 == (3 & (SvalS)))) {
        x_190X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 44;
        goto push_exception_continuationB;
       push_exception_continuationB_return_44:
        *((long *) (SstackS)) = x_190X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 45;
        goto push_exception_continuationB;
       push_exception_continuationB_return_45:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 72 : {
      if ((0 == (3 & (SvalS)))) {
        x_191X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 46;
        goto push_exception_continuationB;
       push_exception_continuationB_return_46:
        *((long *) (SstackS)) = x_191X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 47;
        goto push_exception_continuationB;
       push_exception_continuationB_return_47:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 73 : {
      if ((0 == (3 & (SvalS)))) {
        x_192X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 48;
        goto push_exception_continuationB;
       push_exception_continuationB_return_48:
        *((long *) (SstackS)) = x_192X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 49;
        goto push_exception_continuationB;
       push_exception_continuationB_return_49:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 74 : {
      if ((0 == (3 & (SvalS)))) {
        x_193X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 50;
        goto push_exception_continuationB;
       push_exception_continuationB_return_50:
        *((long *) (SstackS)) = x_193X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 51;
        goto push_exception_continuationB;
       push_exception_continuationB_return_51:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 75 : {
      if ((0 == (3 & (SvalS)))) {
        x_194X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 52;
        goto push_exception_continuationB;
       push_exception_continuationB_return_52:
        *((long *) (SstackS)) = x_194X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 53;
        goto push_exception_continuationB;
       push_exception_continuationB_return_53:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 76 : {
      if ((0 == (3 & (SvalS)))) {
        x_195X = SvalS;
        if ((x_195X < 0)) {
          merged_arg0K0 = 6;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 54;
          goto push_exception_continuationB;
         push_exception_continuationB_return_54:
          *((long *) (SstackS)) = x_195X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 1;
          goto L17068;}
        else {
          SvalS = 0;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 55;
        goto push_exception_continuationB;
       push_exception_continuationB_return_55:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 77 : {
      if ((0 == (3 & (SvalS)))) {
        x_196X = SvalS;
        if ((x_196X < 0)) {
          arg0K0 = (0 - x_196X);
          goto L28018;}
        else {
          arg0K0 = x_196X;
          goto L28018;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 56;
        goto push_exception_continuationB;
       push_exception_continuationB_return_56:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 78 : {
      SstackS = ((SstackS) + 4);
      arg2_197X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_197X | (SvalS))))) {
        x_198X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 57;
        goto push_exception_continuationB;
       push_exception_continuationB_return_57:
        *((long *) (SstackS)) = arg2_197X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_198X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 58;
        goto push_exception_continuationB;
       push_exception_continuationB_return_58:
        *((long *) (SstackS)) = arg2_197X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 79 : {
      SstackS = ((SstackS) + 4);
      arg2_199X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_199X | (SvalS))))) {
        x_200X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 59;
        goto push_exception_continuationB;
       push_exception_continuationB_return_59:
        *((long *) (SstackS)) = arg2_199X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_200X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 60;
        goto push_exception_continuationB;
       push_exception_continuationB_return_60:
        *((long *) (SstackS)) = arg2_199X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 80 : {
      SstackS = ((SstackS) + 4);
      arg2_201X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_201X | (SvalS))))) {
        x_202X = SvalS;
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 61;
        goto push_exception_continuationB;
       push_exception_continuationB_return_61:
        *((long *) (SstackS)) = arg2_201X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_202X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 62;
        goto push_exception_continuationB;
       push_exception_continuationB_return_62:
        *((long *) (SstackS)) = arg2_201X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 81 : {
      if ((0 == (3 & (SvalS)))) {
        SvalS = ((((~ ((((SvalS))>>2))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 63;
        goto push_exception_continuationB;
       push_exception_continuationB_return_63:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 82 : {
      SstackS = ((SstackS) + 4);
      arg2_203X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_203X | (SvalS))))) {
        SvalS = (((((((arg2_203X)>>2)) & ((((SvalS))>>2))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 64;
        goto push_exception_continuationB;
       push_exception_continuationB_return_64:
        *((long *) (SstackS)) = arg2_203X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 83 : {
      SstackS = ((SstackS) + 4);
      arg2_204X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_204X | (SvalS))))) {
        SvalS = (((((((arg2_204X)>>2)) | ((((SvalS))>>2))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 65;
        goto push_exception_continuationB;
       push_exception_continuationB_return_65:
        *((long *) (SstackS)) = arg2_204X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 84 : {
      SstackS = ((SstackS) + 4);
      arg2_205X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_205X | (SvalS))))) {
        SvalS = (((((((arg2_205X)>>2)) ^ ((((SvalS))>>2))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 66;
        goto push_exception_continuationB;
       push_exception_continuationB_return_66:
        *((long *) (SstackS)) = arg2_205X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 85 : {
      SstackS = ((SstackS) + 4);
      arg2_206X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_206X | (SvalS))))) {
        x_207X = SvalS;
        value_208X = ((arg2_206X)>>2);
        count_209X = ((x_207X)>>2);
        if ((count_209X < 0)) {
          PS_SHIFT_RIGHT(value_208X, (0 - count_209X), x_210X)
          SvalS = (((x_210X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          PS_SHIFT_LEFT(value_208X, count_209X, x_211X)
          result_212X = (((((x_211X)<<2)))>>2);
          PS_SHIFT_RIGHT(result_212X, count_209X, x_213X)
          if ((value_208X == x_213X)) {
            if ((value_208X < 0)) {
              if ((result_212X < 0)) {
                goto L33882;}
              else {
                merged_arg0K0 = 6;
                merged_arg0K1 = 1;
                push_exception_continuationB_return_tag = 67;
                goto push_exception_continuationB;
               push_exception_continuationB_return_67:
                *((long *) (SstackS)) = arg2_206X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_207X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L17068;}}
            else {
              if ((result_212X < 0)) {
                merged_arg0K0 = 6;
                merged_arg0K1 = 1;
                push_exception_continuationB_return_tag = 68;
                goto push_exception_continuationB;
               push_exception_continuationB_return_68:
                *((long *) (SstackS)) = arg2_206X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_207X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L17068;}
              else {
                goto L33882;}}}
          else {
            merged_arg0K0 = 6;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 69;
            goto push_exception_continuationB;
           push_exception_continuationB_return_69:
            *((long *) (SstackS)) = arg2_206X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_207X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L17068;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 70;
        goto push_exception_continuationB;
       push_exception_continuationB_return_70:
        *((long *) (SstackS)) = arg2_206X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 86 : {
      x_214X = SvalS;
      if ((9 == (255 & x_214X))) {
        arg0K0 = 5;
        goto L38937;}
      else {
        arg0K0 = 1;
        goto L38937;}}
      break;
    case 87 : {
      SstackS = ((SstackS) + 4);
      arg2_215X = *((long *) (SstackS));
      if ((9 == (255 & arg2_215X))) {
        if ((9 == (255 & (SvalS)))) {
          x_216X = SvalS;
          if ((arg2_215X == x_216X)) {
            arg0K0 = 5;
            goto L29915;}
          else {
            arg0K0 = 1;
            goto L29915;}}
        else {
          goto L29888;}}
      else {
        goto L29888;}}
      break;
    case 88 : {
      SstackS = ((SstackS) + 4);
      arg2_217X = *((long *) (SstackS));
      if ((9 == (255 & arg2_217X))) {
        if ((9 == (255 & (SvalS)))) {
          x_218X = SvalS;
          if ((arg2_217X < x_218X)) {
            arg0K0 = 5;
            goto L29825;}
          else {
            arg0K0 = 1;
            goto L29825;}}
        else {
          goto L29798;}}
      else {
        goto L29798;}}
      break;
    case 89 : {
      if ((9 == (255 & (SvalS)))) {
        SvalS = (((((((((SvalS))>>8)))))<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 71;
        goto push_exception_continuationB;
       push_exception_continuationB_return_71:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 90 : {
      if ((0 == (3 & (SvalS)))) {
        x_219X = (((SvalS))>>2);
        if ((255 < x_219X)) {
          goto L27918;}
        else {
          if ((x_219X < 0)) {
            goto L27918;}
          else {
            SvalS = (9 + (((((x_219X)))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L19170;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 72;
        goto push_exception_continuationB;
       push_exception_continuationB_return_72:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 91 : {
      x_220X = SvalS;
      if ((21 == x_220X)) {
        arg0K0 = 5;
        goto L38966;}
      else {
        arg0K0 = 1;
        goto L38966;}}
      break;
    case 92 : {
      x_221X = SvalS;
      type_222X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_221X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_221X))) + -4))))>>2))) == type_222X)) {
          arg0K0 = 5;
          goto L38980;}
        else {
          arg0K0 = 1;
          goto L38980;}}
      else {
        arg0K0 = 1;
        goto L38980;}}
      break;
    case 93 : {
      x_223X = SvalS;
      type_224X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_223X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_223X))) + -4))))>>2))) == type_224X)) {
          SvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_223X))) + -4))))>>8))));
          Scode_pointerS = ((Scode_pointerS) + 2);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          goto L24394;}}
      else {
        goto L24394;}}
      break;
    case 94 : {
      len_225X = *((unsigned char *) ((Scode_pointerS) + 1));
      space_226X = 4 + (((len_225X)<<2));
      v_227X = AVAILABLEp(space_226X);
      if (v_227X) {
        goto L24200;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 5;
        goto collect_saving_temp;
       collect_saving_temp_return_5:
        v_228X = AVAILABLEp(space_226X);
        if (v_228X) {
          goto L24200;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L24200;}}}
      break;
    case 95 : {
      SstackS = ((SstackS) + 4);
      len_229X = (((*((long *) (SstackS))))>>2);
      space_230X = 4 + (((len_229X)<<2));
      v_231X = AVAILABLEp(space_230X);
      if (v_231X) {
        goto L23991;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 6;
        goto collect_saving_temp;
       collect_saving_temp_return_6:
        v_232X = AVAILABLEp(space_230X);
        if (v_232X) {
          goto L23991;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L23991;}}}
      break;
    case 96 : {
      x_233X = SvalS;
      offset_234X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_235X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & x_233X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + x_233X))) + -4))))>>2))) == type_235X)) {
          SvalS = (*((long *) ((((char *) (-3 + x_233X))) + (((offset_234X)<<2)))));
          Scode_pointerS = ((Scode_pointerS) + 3);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          goto L23889;}}
      else {
        goto L23889;}}
      break;
    case 97 : {
      SstackS = ((SstackS) + 4);
      arg2_236X = *((long *) (SstackS));
      x_237X = SvalS;
      offset_238X = *((unsigned char *) ((Scode_pointerS) + 2));
      type_239X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((3 == (3 & arg2_236X))) {
        if (((31 & ((((*((long *) ((((char *) (-3 + arg2_236X))) + -4))))>>2))) == type_239X)) {
          if ((3 == (3 & arg2_236X))) {
            if ((0 == (128 & (*((long *) ((((char *) (-3 + arg2_236X))) + -4)))))) {
              addr_240X = (((char *) (-3 + arg2_236X))) + (((offset_238X)<<2));
              WRITE_BARRIER(addr_240X, x_237X);
              *((long *) addr_240X) = x_237X;
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 3);
              arg1K0 = (Scode_pointerS);
              goto L19170;}
            else {
              goto L23677;}}
          else {
            goto L23677;}}
        else {
          goto L23677;}}
      else {
        goto L23677;}}
      break;
    case 98 : {
      SstackS = ((SstackS) + 4);
      arg2_241X = *((long *) (SstackS));
      x_242X = SvalS;
      type_243X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_241X))) {
        len_244X = ((arg2_241X)>>2);
        size_245X = 1 + len_244X;
        if ((len_244X < 0)) {
          goto L23349;}
        else {
          if ((4194305 < size_245X)) {
            goto L23349;}
          else {
            v_246X = AVAILABLEp(size_245X);
            if (v_246X) {
              arg2K0 = 1;
              arg0K1 = x_242X;
              goto L23367;}
            else {
              merged_arg0K0 = x_242X;
              collect_saving_temp_return_tag = 7;
              goto collect_saving_temp;
             collect_saving_temp_return_7:
              temp_247X = collect_saving_temp0_return_value;
              v_248X = AVAILABLEp(size_245X);
              if (v_248X) {
                arg2K0 = 1;
                arg0K1 = temp_247X;
                goto L23367;}
              else {
                arg2K0 = 0;
                arg0K1 = temp_247X;
                goto L23367;}}}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 2;
        push_exception_continuationB_return_tag = 73;
        goto push_exception_continuationB;
       push_exception_continuationB_return_73:
        *((long *) (SstackS)) = (((type_243X)<<2));
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_241X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_242X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L17068;}}
      break;
    case 99 : {
      SstackS = ((SstackS) + 4);
      arg2_249X = *((long *) (SstackS));
      x_250X = SvalS;
      type_251X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & x_250X))) {
        if ((3 == (3 & arg2_249X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg2_249X))) + -4))))>>2))) == type_251X)) {
            len_252X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_249X))) + -4))))>>8))))>>2);
            index_253X = ((x_250X)>>2);
            if ((index_253X < 0)) {
              goto L23086;}
            else {
              if ((index_253X < len_252X)) {
                SvalS = (*((long *) ((((char *) (-3 + arg2_249X))) + (-4 & x_250X))));
                Scode_pointerS = ((Scode_pointerS) + 2);
                arg1K0 = (Scode_pointerS);
                goto L19170;}
              else {
                goto L23086;}}}
          else {
            goto L23063;}}
        else {
          goto L23063;}}
      else {
        goto L23063;}}
      break;
    case 100 : {
      SstackS = ((SstackS) + 4);
      arg2_254X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_255X = *((long *) (SstackS));
      x_256X = SvalS;
      type_257X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == (3 & arg2_254X))) {
        if ((3 == (3 & arg3_255X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg3_255X))) + -4))))>>2))) == type_257X)) {
            if ((3 == (3 & arg3_255X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_255X))) + -4)))))) {
                len_258X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_255X))) + -4))))>>8))))>>2);
                index_259X = ((arg2_254X)>>2);
                if ((index_259X < 0)) {
                  goto L22727;}
                else {
                  if ((index_259X < len_258X)) {
                    addr_260X = (((char *) (-3 + arg3_255X))) + (-4 & arg2_254X);
                    WRITE_BARRIER(addr_260X, x_256X);
                    *((long *) addr_260X) = x_256X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 2);
                    arg1K0 = (Scode_pointerS);
                    goto L19170;}
                  else {
                    goto L22727;}}}
              else {
                goto L22702;}}
            else {
              goto L22702;}}
          else {
            goto L22702;}}
        else {
          goto L22702;}}
      else {
        goto L22702;}}
      break;
    case 101 : {
      SstackS = ((SstackS) + 4);
      arg2_261X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_262X = *((long *) (SstackS));
      if ((0 == (3 & arg2_261X))) {
        x_263X = SvalS;
        index_264X = ((arg2_261X)>>2);
        if ((3 == (3 & arg3_262X))) {
          if (((31 & ((((*((long *) ((((char *) (-3 + arg3_262X))) + -4))))>>2))) < 19)) {
            len_265X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_262X))) + -4))))>>8))))>>2);
            if ((index_264X < 0)) {
              goto L27679;}
            else {
              if ((index_264X < len_265X)) {
                addr_266X = (((char *) (-3 + arg3_262X))) + (((index_264X)<<2));
                WRITE_BARRIER(addr_266X, x_263X);
                *((long *) addr_266X) = x_263X;
                SvalS = 13;
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L19170;}
              else {
                goto L27679;}}}
          else {
            goto L27660;}}
        else {
          goto L27660;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 74;
        goto push_exception_continuationB;
       push_exception_continuationB_return_74:
        *((long *) (SstackS)) = arg3_262X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_261X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L17068;}}
      break;
    case 102 : {
      SstackS = ((SstackS) + 4);
      arg2_267X = *((long *) (SstackS));
      if ((0 == (3 & (arg2_267X | (SvalS))))) {
        len_268X = ((arg2_267X)>>2);
        init_269X = (((SvalS))>>2);
        size_270X = 1 + ((((3 + len_268X))>>2));
        if ((len_268X < 0)) {
          goto L27392;}
        else {
          if ((4194305 < size_270X)) {
            goto L27392;}
          else {
            v_271X = AVAILABLEp(size_270X);
            if (v_271X) {
              goto L27491;}
            else {
              merged_arg0K0 = 0;
              collect_saving_temp_return_tag = 8;
              goto collect_saving_temp;
             collect_saving_temp_return_8:
              v_272X = AVAILABLEp(size_270X);
              if (v_272X) {
                goto L27491;}
              else {
                merged_arg0K0 = 8;
                merged_arg0K1 = 1;
                push_exception_continuationB_return_tag = 75;
                goto push_exception_continuationB;
               push_exception_continuationB_return_75:
                *((long *) (SstackS)) = (((len_268X)<<2));
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = (((init_269X)<<2));
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L17068;}}}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 76;
        goto push_exception_continuationB;
       push_exception_continuationB_return_76:
        *((long *) (SstackS)) = arg2_267X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 103 : {
      obj_273X = SvalS;
      if ((3 == (3 & obj_273X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + obj_273X))) + -4))))>>2))))) {
          SvalS = (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          goto L36172;}}
      else {
        goto L36172;}}
      break;
    case 104 : {
      SstackS = ((SstackS) + 4);
      arg2_274X = *((long *) (SstackS));
      if ((3 == (3 & arg2_274X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg2_274X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_275X = (((SvalS))>>2);
            len_276X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_274X))) + -4))))>>8);
            if ((index_275X < 0)) {
              goto L34988;}
            else {
              if ((index_275X < len_276X)) {
                SvalS = ((((*((unsigned char *) ((((char *) (-3 + arg2_274X))) + index_275X))))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L19170;}
              else {
                goto L34988;}}}
          else {
            goto L36731;}}
        else {
          goto L36731;}}
      else {
        goto L36731;}}
      break;
    case 105 : {
      SstackS = ((SstackS) + 4);
      arg2_277X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_278X = *((long *) (SstackS));
      if ((3 == (3 & arg3_278X))) {
        if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg3_278X))) + -4))))>>2))))) {
          if ((0 == (3 & (arg2_277X | (SvalS))))) {
            index_279X = ((arg2_277X)>>2);
            Kchar_280X = (((SvalS))>>2);
            if ((3 == (3 & arg3_278X))) {
              if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_278X))) + -4)))))) {
                len_281X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_278X))) + -4))))>>8);
                if ((index_279X < 0)) {
                  goto L33694;}
                else {
                  if ((index_279X < len_281X)) {
                    *((unsigned char *) ((((char *) (-3 + arg3_278X))) + index_279X)) = Kchar_280X;
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L19170;}
                  else {
                    goto L33694;}}}
              else {
                goto L33673;}}
            else {
              goto L33673;}}
          else {
            goto L35539;}}
        else {
          goto L35539;}}
      else {
        goto L35539;}}
      break;
    case 106 : {
      SstackS = ((SstackS) + 4);
      arg2_282X = *((long *) (SstackS));
      if ((0 == (3 & arg2_282X))) {
        if ((9 == (255 & (SvalS)))) {
          len_283X = ((arg2_282X)>>2);
          init_284X = ((((SvalS))>>8));
          size_285X = 1 + ((((4 + len_283X))>>2));
          if ((len_283X < 0)) {
            goto L27153;}
          else {
            if ((4194305 < size_285X)) {
              goto L27153;}
            else {
              v_286X = AVAILABLEp(size_285X);
              if (v_286X) {
                goto L27252;}
              else {
                merged_arg0K0 = 0;
                collect_saving_temp_return_tag = 9;
                goto collect_saving_temp;
               collect_saving_temp_return_9:
                v_287X = AVAILABLEp(size_285X);
                if (v_287X) {
                  goto L27252;}
                else {
                  merged_arg0K0 = 8;
                  merged_arg0K1 = 1;
                  push_exception_continuationB_return_tag = 77;
                  goto push_exception_continuationB;
                 push_exception_continuationB_return_77:
                  *((long *) (SstackS)) = (((len_283X)<<2));
                  SstackS = ((SstackS) + -4);
                  *((long *) (SstackS)) = (9 + ((((init_284X))<<8)));
                  SstackS = ((SstackS) + -4);
                  arg0K0 = 2;
                  goto L17068;}}}}}
        else {
          goto L34765;}}
      else {
        goto L34765;}}
      break;
    case 107 : {
      obj_288X = SvalS;
      if ((3 == (3 & obj_288X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + obj_288X))) + -4))))>>2))))) {
          SvalS = (-4 + (((((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8)))<<2)));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          goto L36115;}}
      else {
        goto L36115;}}
      break;
    case 108 : {
      SstackS = ((SstackS) + 4);
      arg2_289X = *((long *) (SstackS));
      if ((3 == (3 & arg2_289X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_289X))) + -4))))>>2))))) {
          if ((0 == (3 & (SvalS)))) {
            index_290X = (((SvalS))>>2);
            len_291X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_289X))) + -4))))>>8));
            if ((index_290X < 0)) {
              goto L35059;}
            else {
              if ((index_290X < len_291X)) {
                SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + arg2_289X))) + index_290X))))))<<8)));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L19170;}
              else {
                goto L35059;}}}
          else {
            goto L36824;}}
        else {
          goto L36824;}}
      else {
        goto L36824;}}
      break;
    case 109 : {
      SstackS = ((SstackS) + 4);
      arg2_292X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_293X = *((long *) (SstackS));
      if ((3 == (3 & arg3_293X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg3_293X))) + -4))))>>2))))) {
          if ((0 == (3 & arg2_292X))) {
            if ((9 == (255 & (SvalS)))) {
              index_294X = ((arg2_292X)>>2);
              Kchar_295X = ((((SvalS))>>8));
              if ((3 == (3 & arg3_293X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg3_293X))) + -4)))))) {
                  len_296X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_293X))) + -4))))>>8));
                  if ((index_294X < 0)) {
                    goto L33539;}
                  else {
                    if ((index_294X < len_296X)) {
                      *((unsigned char *) ((((char *) (-3 + arg3_293X))) + index_294X)) = (Kchar_295X);
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L19170;}
                    else {
                      goto L33539;}}}
                else {
                  goto L33518;}}
              else {
                goto L33518;}}
            else {
              goto L35413;}}
          else {
            goto L35413;}}
        else {
          goto L35413;}}
      else {
        goto L35413;}}
      break;
    case 110 : {
      obj_297X = SvalS;
      if ((3 == (3 & obj_297X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + obj_297X))) + -4))))>>2))))) {
          x_298X = SvalS;
          descriptor_299X = *((long *) ((((char *) (-3 + x_298X))) + 8));
          if ((17 == (255 & descriptor_299X))) {
            if ((529 == (*((long *) ((((char *) (-3 + x_298X))) + 8))))) {
              arg0K0 = 5;
              goto L29669;}
            else {
              arg0K0 = 1;
              goto L29669;}}
          else {
            arg0K0 = 5;
            goto L29669;}}
        else {
          goto L29641;}}
      else {
        goto L29641;}}
      break;
    case 111 : {
      SstackS = ((SstackS) + 4);
      arg2_300X = *((long *) (SstackS));
      if ((3 == (3 & arg2_300X))) {
        if ((4 == (31 & ((((*((long *) ((((char *) (-3 + arg2_300X))) + -4))))>>2))))) {
          x_301X = SvalS;
          if ((1 == x_301X)) {
            goto L31134;}
          else {
            if ((5 == x_301X)) {
              goto L31134;}
            else {
              goto L31160;}}}
        else {
          goto L31160;}}
      else {
        goto L31160;}}
      break;
    case 112 : {
      x_302X = SvalS;
      if ((3 == (3 & x_302X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_302X))) + -4)))))) {
          arg0K0 = 1;
          goto L39045;}
        else {
          arg0K0 = 5;
          goto L39045;}}
      else {
        arg0K0 = 5;
        goto L39045;}}
      break;
    case 113 : {
      x_303X = SvalS;
      if ((3 == (3 & x_303X))) {
        if ((0 == (128 & (*((long *) ((((char *) (-3 + x_303X))) + -4)))))) {
          *((long *) ((((char *) (-3 + x_303X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + x_303X))) + -4))));
          goto L27102;}
        else {
          goto L27102;}}
      else {
        goto L27102;}}
      break;
    case 114 : {
      v_304X = AVAILABLEp(72);
      if (v_304X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L26701;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 10;
        goto collect_saving_temp;
       collect_saving_temp_return_10:
        v_305X = AVAILABLEp(72);
        if (v_305X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L26701;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L26701;}}}
      break;
    case 115 : {
      v_306X = AVAILABLEp(66);
      if (v_306X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L33400;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 11;
        goto collect_saving_temp;
       collect_saving_temp_return_11:
        v_307X = AVAILABLEp(66);
        if (v_307X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L33400;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L33400;}}}
      break;
    case 116 : {
      v_308X = AVAILABLEp(66);
      if (v_308X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L37957;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 12;
        goto collect_saving_temp;
       collect_saving_temp_return_12:
        v_309X = AVAILABLEp(66);
        if (v_309X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L37957;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L37957;}}}
      break;
    case 117 : {
      v_310X = AVAILABLEp(66);
      if (v_310X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L38172;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 13;
        goto collect_saving_temp;
       collect_saving_temp_return_13:
        v_311X = AVAILABLEp(66);
        if (v_311X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L38172;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L38172;}}}
      break;
    case 118 : {
      obj_312X = SvalS;
      if ((3 == (3 & obj_312X))) {
        if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_312X))) + -4))))>>2))))) {
          channel_313X = SvalS;
          head_314X = Spending_channels_headS;
          if ((1 == head_314X)) {
            n_315X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_313X))) + 8))))>>2)));
            arg0K0 = (((n_315X)<<2));
            goto L26502;}
          else {
            if ((channel_313X == head_314X)) {
              channel_316X = Spending_channels_headS;
              next_317X = *((long *) ((((char *) (-3 + channel_316X))) + 12));
              Spending_channels_headS = next_317X;
              addr_318X = (((char *) (-3 + channel_316X))) + 12;
              WRITE_BARRIER(addr_318X, 1);
              *((long *) addr_318X) = 1;
              if ((1 == next_317X)) {
                Spending_channels_tailS = 1;
                goto L8211;}
              else {
                goto L8211;}}
            else {
              arg0K0 = (*((long *) ((((char *) (-3 + head_314X))) + 12)));
              arg0K1 = head_314X;
              goto L8217;}}}
        else {
          goto L26503;}}
      else {
        goto L26503;}}
      break;
    case 119 : {
      obj_319X = SvalS;
      if ((3 == (3 & obj_319X))) {
        if ((15 == (31 & ((((*((long *) ((((char *) (-3 + obj_319X))) + -4))))>>2))))) {
          x_320X = *((long *) ((((char *) (-3 + (SvalS)))) + 8));
          if ((529 == x_320X)) {
            arg0K0 = 1;
            goto L29507;}
          else {
            arg0K0 = 5;
            goto L29507;}}
        else {
          goto L29478;}}
      else {
        goto L29478;}}
      break;
    case 120 : {
      obj_321X = SvalS;
      if ((3 == (3 & obj_321X))) {
        if ((15 == (31 & ((((*((long *) ((((char *) (-3 + obj_321X))) + -4))))>>2))))) {
          x_322X = SvalS;
          value_323X = *((long *) ((((char *) (-3 + x_322X))) + 8));
          if ((529 == value_323X)) {
            merged_arg0K0 = 23;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 78;
            goto push_exception_continuationB;
           push_exception_continuationB_return_78:
            *((long *) (SstackS)) = x_322X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 1;
            goto L17068;}
          else {
            SvalS = value_323X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L19170;}}
        else {
          goto L26436;}}
      else {
        goto L26436;}}
      break;
    case 121 : {
      SvalS = 529;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 122 : {
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 123 : {
      x_324X = SvalS;
      merged_arg0K0 = 16;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 79;
      goto push_exception_continuationB;
     push_exception_continuationB_return_79:
      *((long *) (SstackS)) = x_324X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L17068;}
      break;
    case 124 : {
      SvalS = 1;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 125 : {
      SvalS = 21;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 126 : {
      v_325X = AVAILABLEp(66);
      if (v_325X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L33042;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 14;
        goto collect_saving_temp;
       collect_saving_temp_return_14:
        v_326X = AVAILABLEp(66);
        if (v_326X) {
          arg2K0 = 1;
          arg0K1 = 0;
          goto L33042;}
        else {
          arg2K0 = 0;
          arg0K1 = 0;
          goto L33042;}}}
      break;
    case 127 : {
      SvalS = 13;
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 15;
      goto collect_saving_temp;
     collect_saving_temp_return_15:
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 128 : {
      v_327X = AVAILABLEp(6);
      if (v_327X) {
        goto L32660;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 16;
        goto collect_saving_temp;
       collect_saving_temp_return_16:
        v_328X = AVAILABLEp(6);
        if (v_328X) {
          goto L32660;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L32660;}}}
      break;
    case 129 : {
      SstackS = ((SstackS) + 4);
      arg2_329X = *((long *) (SstackS));
      if ((0 == (3 & arg2_329X))) {
        x_330X = SvalS;
        key_331X = ((arg2_329X)>>2);
        if ((5 == key_331X)) {
          SvalS = (-4 & x_330X);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          if ((0 == key_331X)) {
            x_332X = available();
            SvalS = (((x_332X)<<2));
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L19170;}
          else {
            if ((1 == key_331X)) {
              bytes_333X = heap_size();
              SvalS = (-4 & (3 + bytes_333X));
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L19170;}
            else {
              if ((2 == key_331X)) {
                SvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                Scode_pointerS = ((Scode_pointerS) + 1);
                arg1K0 = (Scode_pointerS);
                goto L19170;}
              else {
                if ((3 == key_331X)) {
                  x_334X = gc_count();
                  SvalS = (((x_334X)<<2));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L19170;}
                else {
                  if ((4 == key_331X)) {
                    merged_arg0K0 = 15;
                    merged_arg0K1 = 1;
                    push_exception_continuationB_return_tag = 80;
                    goto push_exception_continuationB;
                   push_exception_continuationB_return_80:
                    *((long *) (SstackS)) = (((key_331X)<<2));
                    SstackS = ((SstackS) + -4);
                    *((long *) (SstackS)) = x_330X;
                    SstackS = ((SstackS) + -4);
                    arg0K0 = 2;
                    goto L17068;}
                  else {
                    if ((6 == key_331X)) {
                      v_335X = AVAILABLEp(5);
                      if (v_335X) {
                        goto L35946;}
                      else {
                        merged_arg0K0 = 0;
                        collect_saving_temp_return_tag = 17;
                        goto collect_saving_temp;
                       collect_saving_temp_return_17:
                        v_336X = AVAILABLEp(5);
                        if (v_336X) {
                          goto L35946;}
                        else {
                          ps_error("Scheme48 heap overflow", 0);
                          goto L35946;}}}
                    else {
                      merged_arg0K0 = 18;
                      merged_arg0K1 = 1;
                      push_exception_continuationB_return_tag = 81;
                      goto push_exception_continuationB;
                     push_exception_continuationB_return_81:
                      *((long *) (SstackS)) = (((key_331X)<<2));
                      SstackS = ((SstackS) + -4);
                      *((long *) (SstackS)) = x_330X;
                      SstackS = ((SstackS) + -4);
                      arg0K0 = 2;
                      goto L17068;}}}}}}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 82;
        goto push_exception_continuationB;
       push_exception_continuationB_return_82:
        *((long *) (SstackS)) = arg2_329X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 130 : {
      if ((0 == (3 & (SvalS)))) {
        type_337X = (((SvalS))>>2);
        arg2K0 = 1;
        goto L26300;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 83;
        goto push_exception_continuationB;
       push_exception_continuationB_return_83:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 131 : {
      obj_338X = SvalS;
      if ((3 == (3 & obj_338X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_338X))) + -4))))>>2))))) {
          x_339X = SvalS;
          arg0K0 = x_339X;
          arg2K1 = 1;
          goto L35273;}
        else {
          goto L35295;}}
      else {
        goto L35295;}}
      break;
    case 132 : {
      x_340X = SvalS;
      arg2K0 = 1;
      arg0K1 = x_340X;
      goto L39120;}
      break;
    case 133 : {
      SvalS = (Scurrent_threadS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 134 : {
      Scurrent_threadS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 135 : {
      SvalS = (Ssession_dataS);
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 136 : {
      Ssession_dataS = (SvalS);
      SvalS = 13;
      Scode_pointerS = ((Scode_pointerS) + 1);
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 137 : {
      obj_341X = SvalS;
      if ((3 == (3 & obj_341X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_341X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 163)) {
            goto L21340;}
          else {
            temp_342X = Sexception_handlersS;
            Sexception_handlersS = (SvalS);
            SvalS = temp_342X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L19170;}}
        else {
          goto L21340;}}
      else {
        goto L21340;}}
      break;
    case 138 : {
      SstackS = ((SstackS) + 4);
      p_343X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      template_344X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      p_345X = *((long *) (SstackS));
      StemplateS = template_344X;
      Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_344X)))))))) + ((((((((((p_343X)>>2)) + (((p_345X)>>2))))<<2)))>>2)));
      arg1K0 = (Scode_pointerS);
      goto L19170;}
      break;
    case 139 : {
      obj_346X = SvalS;
      if ((3 == (3 & obj_346X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_346X))) + -4))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))))>>2)) < 4)) {
            goto L20181;}
          else {
            temp_347X = Sinterrupt_handlersS;
            Sinterrupt_handlersS = (SvalS);
            SvalS = temp_347X;
            Scode_pointerS = ((Scode_pointerS) + 1);
            arg1K0 = (Scode_pointerS);
            goto L19170;}}
        else {
          goto L20181;}}
      else {
        goto L20181;}}
      break;
    case 140 : {
      old_348X = Senabled_interruptsS;
      p_349X = SvalS;
      Senabled_interruptsS = (((p_349X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        Spending_interruptPS = 0;
        if ((Spending_eventsPS)) {
          Spending_interruptPS = 1;
          goto L39161;}
        else {
          goto L39161;}}
      else {
        Spending_interruptPS = 1;
        goto L39161;}}
      break;
    case 141 : {
      SstackS = ((SstackS) + 4);
      p_350X = *((long *) (SstackS));
      Senabled_interruptsS = (((p_350X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        Spending_interruptPS = 0;
        if ((Spending_eventsPS)) {
          Spending_interruptPS = 1;
          goto L19184;}
        else {
          goto L19184;}}
      else {
        Spending_interruptPS = 1;
        goto L19184;}}
      break;
    case 142 : {
      if ((0 == (3 & (SvalS)))) {
        p_351X = SvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          Spending_interruptPS = 0;
          if ((Spending_eventsPS)) {
            Spending_interruptPS = 1;
            goto L35843;}
          else {
            goto L35843;}}
        else {
          Spending_interruptPS = 1;
          goto L35843;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 84;
        goto push_exception_continuationB;
       push_exception_continuationB_return_84:
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}
      break;
    case 143 : {
      SstackS = ((SstackS) + 4);
      arg2_352X = *((long *) (SstackS));
      if ((0 == (3 & arg2_352X))) {
        x_353X = SvalS;
        if ((1 == x_353X)) {
          goto L30902;}
        else {
          if ((5 == x_353X)) {
            goto L30902;}
          else {
            goto L30922;}}}
      else {
        goto L30922;}}
      break;
    case 144 : {
      obj_354X = SvalS;
      if ((3 == (3 & obj_354X))) {
        if ((13 == (31 & ((((*((long *) ((((char *) (-3 + obj_354X))) + -4))))>>2))))) {
          x_355X = SvalS;
          value_356X = *((long *) ((((char *) (-3 + x_355X))) + 4));
          name_357X = *((long *) (((char *) (-3 + x_355X))));
          if ((3 == (3 & name_357X))) {
            if ((19 == (31 & ((((*((long *) ((((char *) (-3 + name_357X))) + -4))))>>2))))) {
              if ((3 == (3 & value_356X))) {
                if ((20 == (31 & ((((*((long *) ((((char *) (-3 + value_356X))) + -4))))>>2))))) {
                  v_358X = lookup_external_name((((char *) (-3 + name_357X))), (((char *) (-3 + value_356X))));
                  if (v_358X) {
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L19170;}
                  else {
                    goto L32516;}}
                else {
                  goto L32516;}}
              else {
                goto L32516;}}
            else {
              goto L32516;}}
          else {
            goto L32516;}}
        else {
          goto L32521;}}
      else {
        goto L32521;}}
      break;
    case 145 : {
      SstackS = ((SstackS) + 4);
      nargs_359X = (((*((long *) (SstackS))))>>2);
      SstackS = ((SstackS) + 4);
      SstackS = ((SstackS) + 4);
      rest_list_360X = *((long *) (SstackS));
      arg0K0 = rest_list_360X;
      goto L17750;}
      break;
    case 146 : {
      SstackS = ((SstackS) + 4);
      arg2_361X = *((long *) (SstackS));
      if ((0 == (3 & arg2_361X))) {
        x_362X = SvalS;
        option_363X = ((arg2_361X)>>2);
        if ((2 == option_363X)) {
          x_364X = CHEAP_TIME();
          SvalS = (((x_364X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          if ((0 == option_363X)) {
            seconds_365X = run_time(&mseconds_366X);
            arg0K0 = option_363X;
            arg0K1 = seconds_365X;
            arg0K2 = mseconds_366X;
            goto L35710;}
          else {
            if ((1 == option_363X)) {
              seconds_367X = real_time(&mseconds_368X);
              arg0K0 = option_363X;
              arg0K1 = seconds_367X;
              arg0K2 = mseconds_368X;
              goto L35710;}
            else {
              merged_arg0K0 = 18;
              merged_arg0K1 = 1;
              push_exception_continuationB_return_tag = 85;
              goto push_exception_continuationB;
             push_exception_continuationB_return_85:
              *((long *) (SstackS)) = (((option_363X)<<2));
              SstackS = ((SstackS) + -4);
              *((long *) (SstackS)) = x_362X;
              SstackS = ((SstackS) + -4);
              arg0K0 = 2;
              goto L17068;}}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 86;
        goto push_exception_continuationB;
       push_exception_continuationB_return_86:
        *((long *) (SstackS)) = arg2_361X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 147 : {
      SstackS = ((SstackS) + 4);
      arg2_369X = *((long *) (SstackS));
      if ((0 == (3 & arg2_369X))) {
        x_370X = SvalS;
        key_371X = ((arg2_369X)>>2);
        status_372X = extended_vm(key_371X, x_370X);
        if ((0 == status_372X)) {
          SvalS = (Sextension_valueS);
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          if ((1 == status_372X)) {
            merged_arg0K0 = 20;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 87;
            goto push_exception_continuationB;
           push_exception_continuationB_return_87:
            *((long *) (SstackS)) = (((key_371X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_370X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L17068;}
          else {
            merged_arg0K0 = 21;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 88;
            goto push_exception_continuationB;
           push_exception_continuationB_return_88:
            *((long *) (SstackS)) = (((key_371X)<<2));
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_370X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L17068;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 89;
        goto push_exception_continuationB;
       push_exception_continuationB_return_89:
        *((long *) (SstackS)) = arg2_369X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 148 : {
      SstackS = ((SstackS) + 4);
      arg2_373X = *((long *) (SstackS));
      if ((0 == (3 & arg2_373X))) {
        SvalS = (SvalS);
        arg0K0 = 0;
        goto L20076;}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 90;
        goto push_exception_continuationB;
       push_exception_continuationB_return_90:
        *((long *) (SstackS)) = arg2_373X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
      break;
    case 149 : {
      v_374X = AVAILABLEp(3);
      if (v_374X) {
        goto L25832;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 18;
        goto collect_saving_temp;
       collect_saving_temp_return_18:
        v_375X = AVAILABLEp(3);
        if (v_375X) {
          goto L25832;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L25832;}}}
      break;
    case 150 : {
      space_376X = 4 + (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (SvalS)))) + -4))))>>8))));
      v_377X = AVAILABLEp(space_376X);
      if (v_377X) {
        goto L25563;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 19;
        goto collect_saving_temp;
       collect_saving_temp_return_19:
        v_378X = AVAILABLEp(space_376X);
        if (v_378X) {
          goto L25563;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L25563;}}}
      break;
    case 151 : {
      SstackS = ((SstackS) + 4);
      arg2_379X = *((long *) (SstackS));
      if ((3 == (3 & arg2_379X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_379X))) + -4))))>>2))))) {
          obj_380X = SvalS;
          if ((3 == (3 & obj_380X))) {
            if ((19 == (31 & ((((*((long *) ((((char *) (-3 + obj_380X))) + -4))))>>2))))) {
              x_381X = SvalS;
              len_382X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_379X))) + -4))))>>8);
              if ((len_382X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_381X))) + -4))))>>8)))) {
                if (((!memcmp((void *)(((char *) (-3 + x_381X))), (void *)(((char *) (-3 + arg2_379X))),len_382X)))) {
                  arg0K0 = 5;
                  goto L29376;}
                else {
                  arg0K0 = 1;
                  goto L29376;}}
              else {
                arg0K0 = 1;
                goto L29376;}}
            else {
              goto L29343;}}
          else {
            goto L29343;}}
        else {
          goto L29343;}}
      else {
        goto L29343;}}
      break;
    case 152 : {
      obj_383X = SvalS;
      if ((3 == (3 & obj_383X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + obj_383X))) + -4))))>>2))))) {
          x_384X = SvalS;
          n_385X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_384X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L35799;}
        else {
          goto L35776;}}
      else {
        goto L35776;}}
      break;
    case 153 : {
      space_386X = 1 + ((((4 + ((((SvalS))>>2))))>>2));
      v_387X = AVAILABLEp(space_386X);
      if (v_387X) {
        goto L25269;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 20;
        goto collect_saving_temp;
       collect_saving_temp_return_20:
        v_388X = AVAILABLEp(space_386X);
        if (v_388X) {
          goto L25269;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L25269;}}}
      break;
    case 154 : {
      v_389X = AVAILABLEp(6);
      if (v_389X) {
        goto L25127;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 21;
        goto collect_saving_temp;
       collect_saving_temp_return_21:
        v_390X = AVAILABLEp(6);
        if (v_390X) {
          goto L25127;}
        else {
          ps_error("Scheme48 heap overflow", 0);
          goto L25127;}}}
      break;
    case 155 : {
      SstackS = ((SstackS) + 4);
      arg2_391X = *((long *) (SstackS));
      x_392X = SvalS;
      arg0K0 = x_392X;
      arg0K1 = x_392X;
      arg2K2 = 1;
      goto L29068;}
      break;
    case 156 : {
      SstackS = ((SstackS) + 4);
      arg2_393X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_394X = *((long *) (SstackS));
      if ((0 == (3 & (SvalS)))) {
        index_395X = (((SvalS))>>2);
        if ((3 == (3 & arg3_394X))) {
          if ((8 == (31 & ((((*((long *) ((((char *) (-3 + arg3_394X))) + -4))))>>2))))) {
            if ((arg2_393X == (*((long *) (((char *) (-3 + arg3_394X))))))) {
              len_396X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_394X))) + -4))))>>8))))>>2);
              if ((index_395X < 0)) {
                goto L24913;}
              else {
                if ((index_395X < len_396X)) {
                  SvalS = (*((long *) ((((char *) (-3 + arg3_394X))) + (((index_395X)<<2)))));
                  Scode_pointerS = ((Scode_pointerS) + 1);
                  arg1K0 = (Scode_pointerS);
                  goto L19170;}
                else {
                  goto L24913;}}}
            else {
              goto L24960;}}
          else {
            goto L24960;}}
        else {
          goto L24960;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 91;
        goto push_exception_continuationB;
       push_exception_continuationB_return_91:
        *((long *) (SstackS)) = arg3_394X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_393X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 3;
        goto L17068;}}
      break;
    case 157 : {
      SstackS = ((SstackS) + 4);
      arg2_397X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_398X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_399X = *((long *) (SstackS));
      if ((0 == (3 & arg2_397X))) {
        x_400X = SvalS;
        index_401X = ((arg2_397X)>>2);
        if ((3 == (3 & arg4_399X))) {
          if ((8 == (31 & ((((*((long *) ((((char *) (-3 + arg4_399X))) + -4))))>>2))))) {
            if ((arg3_398X == (*((long *) (((char *) (-3 + arg4_399X))))))) {
              if ((3 == (3 & arg4_399X))) {
                if ((0 == (128 & (*((long *) ((((char *) (-3 + arg4_399X))) + -4)))))) {
                  len_402X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_399X))) + -4))))>>8))))>>2);
                  if ((index_401X < 0)) {
                    goto L32167;}
                  else {
                    if ((index_401X < len_402X)) {
                      addr_403X = (((char *) (-3 + arg4_399X))) + (((index_401X)<<2));
                      WRITE_BARRIER(addr_403X, x_400X);
                      *((long *) addr_403X) = x_400X;
                      SvalS = 13;
                      Scode_pointerS = ((Scode_pointerS) + 1);
                      arg1K0 = (Scode_pointerS);
                      goto L19170;}
                    else {
                      goto L32167;}}}
                else {
                  goto L32224;}}
              else {
                goto L32224;}}
            else {
              goto L32224;}}
          else {
            goto L32224;}}
        else {
          goto L32224;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 92;
        goto push_exception_continuationB;
       push_exception_continuationB_return_92:
        *((long *) (SstackS)) = arg4_399X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_398X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_397X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 4;
        goto L17068;}}
      break;
    case 158 : {
      SstackS = ((SstackS) + 4);
      arg2_404X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg3_405X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg4_406X = *((long *) (SstackS));
      SstackS = ((SstackS) + 4);
      arg5_407X = *((long *) (SstackS));
      if ((0 == (3 & ((arg4_406X | arg2_404X) | (SvalS))))) {
        from_index_408X = ((arg4_406X)>>2);
        to_index_409X = ((arg2_404X)>>2);
        count_410X = (((SvalS))>>2);
        if ((3 == (3 & arg5_407X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg5_407X))) + -4))))>>2))))) {
            goto L31531;}
          else {
            goto L31526;}}
        else {
          goto L31526;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 93;
        goto push_exception_continuationB;
       push_exception_continuationB_return_93:
        *((long *) (SstackS)) = arg5_407X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg4_406X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg3_405X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = arg2_404X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (SvalS);
        SstackS = ((SstackS) + -4);
        arg0K0 = 5;
        goto L17068;}}
      break;
    case 159 : {
      v_411X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_411X)) {
        arg0K0 = (SvalS);
        goto L22434;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 0;
        goto get_current_port;
       get_current_port_return_0:
        v_412X = get_current_port0_return_value;
        arg0K0 = v_412X;
        goto L22434;}}
      break;
    case 160 : {
      v_413X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_413X)) {
        arg0K0 = (SvalS);
        goto L22231;}
      else {
        merged_arg0K0 = 0;
        get_current_port_return_tag = 1;
        goto get_current_port;
       get_current_port_return_1:
        v_414X = get_current_port0_return_value;
        arg0K0 = v_414X;
        goto L22231;}}
      break;
    case 161 : {
      v_415X = *((unsigned char *) ((Scode_pointerS) + 1));
      if ((0 == v_415X)) {
        SstackS = ((SstackS) + 4);
        arg0K0 = (*((long *) (SstackS)));
        arg0K1 = (SvalS);
        goto L21978;}
      else {
        merged_arg0K0 = 4;
        get_current_port_return_tag = 2;
        goto get_current_port;
       get_current_port_return_2:
        v_416X = get_current_port0_return_value;
        arg0K0 = (SvalS);
        arg0K1 = v_416X;
        goto L21978;}}
      break;
    case 162 : {
      x_417X = SvalS;
      out_418X = stderr;
      arg0K0 = x_417X;
      goto L30811;}
      break;
  }}
 L17068: {
  nargs_419X = arg0K0;
  opcode_420X = (((*((long *) ((SstackS) + (8 + (((nargs_419X)<<2)))))))>>2);
  obj_421X = Sexception_handlersS;
  if ((3 == (3 & obj_421X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_421X))) + -4))))>>2))))) {
      goto L17127;}
    else {
      goto L17200;}}
  else {
    goto L17200;}}
 L24647: {
  env_422X = arg0K0;
  i_423X = arg0K1;
  if ((0 == i_423X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_422X))) + ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<2)))));
    if ((529 == (SvalS))) {
      merged_arg0K0 = 0;
      merged_arg0K1 = 3;
      push_exception_continuationB_return_tag = 94;
      goto push_exception_continuationB;
     push_exception_continuationB_return_94:
      arg0K0 = 0;
      goto L17068;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 3);
      arg1K0 = (Scode_pointerS);
      goto L19170;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_422X)))));
    arg0K1 = (-1 + i_423X);
    goto L24647;}}
 L20936: {
  env_424X = arg0K0;
  i_425X = arg0K1;
  if ((0 == i_425X)) {
    SvalS = (*((long *) ((((char *) (-3 + env_424X))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 2))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 3)))))<<2)))));
    if ((529 == (SvalS))) {
      merged_arg0K0 = 0;
      merged_arg0K1 = 4;
      push_exception_continuationB_return_tag = 95;
      goto push_exception_continuationB;
     push_exception_continuationB_return_95:
      arg0K0 = 0;
      goto L17068;}
    else {
      Scode_pointerS = ((Scode_pointerS) + 4);
      arg1K0 = (Scode_pointerS);
      goto L19170;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_424X)))));
    arg0K1 = (-1 + i_425X);
    goto L20936;}}
 L20874: {
  env_426X = arg0K0;
  i_427X = arg0K1;
  if ((0 == i_427X)) {
    index_428X = ((((*((unsigned char *) ((Scode_pointerS) + 2))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 3)));
    value_429X = SvalS;
    addr_430X = (((char *) (-3 + env_426X))) + (((index_428X)<<2));
    WRITE_BARRIER(addr_430X, value_429X);
    *((long *) addr_430X) = value_429X;
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 4);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_426X)))));
    arg0K1 = (-1 + i_427X);
    goto L20874;}}
 L21732: {
  space_431X = arg0K0;
  v_432X = AVAILABLEp(space_431X);
  if (v_432X) {
    arg2K0 = 1;
    arg0K1 = 0;
    goto L21830;}
  else {
    merged_arg0K0 = 0;
    collect_saving_temp_return_tag = 22;
    goto collect_saving_temp;
   collect_saving_temp_return_22:
    v_433X = AVAILABLEp(space_431X);
    if (v_433X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L21830;}
    else {
      arg2K0 = 0;
      arg0K1 = 0;
      goto L21830;}}}
 L21739: {
  env_434X = arg0K0;
  v_435X = AVAILABLEp(3);
  if (v_435X) {
    arg2K0 = 1;
    arg0K1 = env_434X;
    goto L21764;}
  else {
    merged_arg0K0 = env_434X;
    collect_saving_temp_return_tag = 23;
    goto collect_saving_temp;
   collect_saving_temp_return_23:
    temp_436X = collect_saving_temp0_return_value;
    v_437X = AVAILABLEp(3);
    if (v_437X) {
      arg2K0 = 1;
      arg0K1 = temp_436X;
      goto L21764;}
    else {
      arg2K0 = 0;
      arg0K1 = temp_436X;
      goto L21764;}}}
 L21535: {
  len_438X = ((total_count_91X)<<2);
  addr_439X = ALLOCATE_SPACE(2, (4 + len_438X));
  *((long *) addr_439X) = (10 + (((len_438X)<<8)));
  new_env_440X = 3 + (((long) (addr_439X + 4)));
  v_441X = *((unsigned char *) ((Scode_pointerS) + 1));
  if ((0 == v_441X)) {
    arg0K0 = 0;
    goto L21549;}
  else {
    value_442X = SvalS;
    addr_443X = ((char *) (-3 + new_env_440X));
    WRITE_BARRIER(addr_443X, value_442X);
    *((long *) addr_443X) = value_442X;
    arg0K0 = 1;
    goto L21549;}}
 L21462: {
  okayP_444X = arg2K0;
  key_445X = arg0K1;
  if (okayP_444X) {
    arg0K0 = key_445X;
    goto L21450;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_445X;
    goto L21450;}}
 L20534: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L20571: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_98X;
  ScontS = (Sbottom_of_stackS);
  goto L20534;}
 L20535: {
  if ((1 == cont_98X)) {
    if ((0 == (3 & (SvalS)))) {
      SvalS = ((((SvalS))>>2));
      arg0K0 = 0;
      goto L20076;}
    else {
      goto L20552;}}
  else {
    goto L20552;}}
 L19938: {
  okayP_446X = arg2K0;
  key_447X = arg0K1;
  if (okayP_446X) {
    arg0K0 = key_447X;
    goto L19893;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_447X;
    goto L19893;}}
 L19850: {
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 0;
      goto check_events;
     check_events_return_0:
      v_448X = check_events0_return_value;
      if (v_448X) {
        arg0K0 = stack_arg_count_99X;
        goto L17297;}
      else {
        goto L19854;}}
    else {
      arg0K0 = stack_arg_count_99X;
      goto L17297;}}
  else {
    goto L19854;}}
 L16469: {
  stack_arg_count_449X = arg0K0;
  code_450X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
  arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_450X))) + 1)));
  arg0K1 = 64;
  goto L16483;}
 L17564: {
  exception_451X = arg0K0;
  stack_arg_count_452X = arg0K1;
  list_args_453X = arg0K2;
  list_arg_count_454X = arg0K3;
  if (((StemplateS) == (SvalS))) {
    if ((0 < (Slosing_opcodeS))) {
      ps_error("wrong number of arguments to exception handler", 1, (Slosing_opcodeS));
      arg0K0 = v_455X;
      goto L20076;}
    else {
      ps_error("wrong number of arguments to interrupt handler", 1, (0 - (Slosing_opcodeS)));
      arg0K0 = v_456X;
      goto L20076;}}
  else {
    merged_arg0K0 = list_args_453X;
    merged_arg0K1 = list_arg_count_454X;
    copy_listS_return_tag = 0;
    goto copy_listS;
   copy_listS_return_0:
    v_457X = copy_listS0_return_value;
    merged_arg0K0 = v_457X;
    merged_arg0K1 = stack_arg_count_452X;
    pop_args_GlistS_return_tag = 1;
    goto pop_args_GlistS;
   pop_args_GlistS_return_1:
    args_458X = pop_args_GlistS0_return_value;
    merged_arg0K0 = exception_451X;
    merged_arg0K1 = 0;
    push_exception_continuationB_return_tag = 96;
    goto push_exception_continuationB;
   push_exception_continuationB_return_96:
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_458X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}}
 L18934: {
  loc_459X = arg1K0;
  arg_460X = arg1K1;
  if ((top_of_args_109X < arg_460X)) {
    *((long *) loc_459X) = (*((long *) arg_460X));
    arg1K0 = (loc_459X + -4);
    arg1K1 = (arg_460X + -4);
    goto L18934;}
  else {
    SstackS = loc_459X;
    obj_461X = SvalS;
    if ((3 == (3 & obj_461X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_461X))) + -4))))>>2))))) {
        arg0K0 = nargs_108X;
        goto L16469;}
      else {
        arg0K0 = 3;
        arg0K1 = nargs_108X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17564;}}
    else {
      arg0K0 = 3;
      arg0K1 = nargs_108X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L17564;}}}
 L18972: {
  stack_arg_count_462X = arg0K0;
  list_args_463X = arg0K1;
  list_arg_count_464X = arg0K2;
  if ((0 == list_arg_count_464X)) {
    obj_465X = SvalS;
    if ((3 == (3 & obj_465X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_465X))) + -4))))>>2))))) {
        arg0K0 = stack_arg_count_462X;
        goto L16469;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_462X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17564;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_462X;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L17564;}}
  else {
    obj_466X = SvalS;
    if ((3 == (3 & obj_466X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_466X))) + -4))))>>2))))) {
        code_467X = *((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + (SvalS))))))))));
        total_arg_count_468X = stack_arg_count_462X + list_arg_count_464X;
        arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_467X))) + 1)));
        arg0K1 = 64;
        goto L16760;}
      else {
        arg0K0 = 3;
        arg0K1 = stack_arg_count_462X;
        arg0K2 = list_args_463X;
        arg0K3 = list_arg_count_464X;
        goto L17564;}}
    else {
      arg0K0 = 3;
      arg0K1 = stack_arg_count_462X;
      arg0K2 = list_args_463X;
      arg0K3 = list_arg_count_464X;
      goto L17564;}}}
 L11608: {
  list_args_469X = arg0K0;
  stack_nargs_470X = arg0K1;
  merged_arg0K0 = list_args_469X;
  okay_argument_list_return_tag = 1;
  goto okay_argument_list;
 okay_argument_list_return_1:
  okayP_471X = okay_argument_list0_return_value;
  list_arg_count_472X = okay_argument_list1_return_value;
  if (okayP_471X) {
    SvalS = proc_116X;
    arg0K0 = stack_nargs_470X;
    arg0K1 = list_args_469X;
    arg0K2 = list_arg_count_472X;
    goto L18972;}
  else {
    *((long *) (SstackS)) = list_args_469X;
    SstackS = ((SstackS) + -4);
    merged_arg0K0 = 25;
    merged_arg0K1 = (1 + stack_nargs_470X);
    pop_args_GlistS_return_tag = 2;
    goto pop_args_GlistS;
   pop_args_GlistS_return_2:
    args_473X = pop_args_GlistS0_return_value;
    SstackS = ((SstackS) + 4);
    merged_arg0K0 = 5;
    merged_arg0K1 = 0;
    push_exception_continuationB_return_tag = 97;
    goto push_exception_continuationB;
   push_exception_continuationB_return_97:
    *((long *) (SstackS)) = proc_116X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = args_473X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}}
 L11649: {
  list_474X = arg0K0;
  follower_475X = arg0K1;
  if ((25 == list_474X)) {
    list_args_476X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + follower_475X))) + 4)))))));
    addr_477X = (((char *) (-3 + follower_475X))) + 4;
    WRITE_BARRIER(addr_477X, list_args_476X);
    *((long *) addr_477X) = list_args_476X;
    arg0K0 = rest_list_117X;
    arg0K1 = (-1 + stack_nargs_115X);
    goto L11608;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + list_474X))) + 4)));
    arg0K1 = (*((long *) ((((char *) (-3 + follower_475X))) + 4)));
    goto L11649;}}
 L18877: {
  obj_478X = SvalS;
  if ((3 == (3 & obj_478X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_478X))) + -4))))>>2))))) {
      arg0K0 = 0;
      goto L16469;}
    else {
      arg0K0 = 3;
      arg0K1 = 0;
      arg0K2 = 25;
      arg0K3 = 0;
      goto L17564;}}
  else {
    arg0K0 = 3;
    arg0K1 = 0;
    arg0K2 = 25;
    arg0K3 = 0;
    goto L17564;}}
 L18884: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = cont_118X;
  ScontS = (Sbottom_of_stackS);
  goto L18877;}
 L21104: {
  stack_nargs_479X = arg0K0;
  list_args_480X = arg0K1;
  list_arg_count_481X = arg0K2;
  if ((1 == stack_nargs_479X)) {
    SstackS = ((SstackS) + 4);
    SvalS = (*((long *) (SstackS)));
    pop_continuationB_return_tag = 1;
    goto pop_continuationB;
   pop_continuationB_return_1:
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    if (((ScontS) == (Sbottom_of_stackS))) {
      arg0K0 = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
      goto L21115;}
    else {
      arg0K0 = (ScontS);
      goto L21115;}}}
 L19676: {
  okayP_482X = arg2K0;
  key_483X = arg0K1;
  if (okayP_482X) {
    arg0K0 = key_483X;
    goto L19664;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_483X;
    goto L19664;}}
 L19549: {
  tem_484X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  StemplateS = tem_484X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_484X)))))))) + 2);
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 1;
      goto check_events;
     check_events_return_1:
      v_485X = check_events0_return_value;
      if (v_485X) {
        goto L19558;}
      else {
        goto L19561;}}
    else {
      goto L19558;}}
  else {
    goto L19561;}}
 L19731: {
  okayP_486X = arg2K0;
  key_487X = arg0K1;
  if (okayP_486X) {
    arg0K0 = key_487X;
    goto L19718;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_487X;
    goto L19718;}}
 L19594: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 4;
  push_exception_continuationB_return_tag = 98;
  goto push_exception_continuationB;
 push_exception_continuationB_return_98:
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2)))));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L19450: {
  arg0K0 = (2 + (((max_135X)<<1)));
  goto L19452;}
 L19452: {
  offset_488X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + offset_488X);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L20437: {
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L19348: {
  delta_489X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_489X);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L20328: {
  delta_490X = arg0K0;
  Scode_pointerS = ((Scode_pointerS) + delta_490X);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L38594: {
  val_491X = arg0K0;
  SvalS = val_491X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L30584: {
  val_492X = arg0K0;
  SvalS = val_492X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L30539: {
  if ((3 == (3 & x_149X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_149X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L30584;}
    else {
      goto L30547;}}
  else {
    goto L30547;}}
 L28870: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L28873: {
  if ((3 == (3 & x_150X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_150X))) + -4))))>>2))))) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 99;
      goto push_exception_continuationB;
     push_exception_continuationB_return_99:
      *((long *) (SstackS)) = x_150X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L17068;}
    else {
      goto L28881;}}
  else {
    goto L28881;}}
 L28679: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L28662: {
  if ((3 == (3 & x_151X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_151X))) + -4))))>>2))))) {
      goto L28679;}
    else {
      goto L28670;}}
  else {
    goto L28670;}}
 L28468: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L28451: {
  if ((3 == (3 & x_152X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_152X))) + -4))))>>2))))) {
      goto L28468;}
    else {
      goto L28459;}}
  else {
    goto L28459;}}
 L28257: {
  SvalS = 5;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L28240: {
  if ((3 == (3 & x_153X))) {
    if ((7 == (31 & ((((*((long *) ((((char *) (-3 + x_153X))) + -4))))>>2))))) {
      goto L28257;}
    else {
      goto L28248;}}
  else {
    goto L28248;}}
 L7132: {
  a_493X = arg0K0;
  if ((b_162X < 0)) {
    arg0K0 = (0 - b_162X);
    goto L7136;}
  else {
    arg0K0 = b_162X;
    goto L7136;}}
 L8910: {
  a_494X = arg0K0;
  if ((b_169X < 0)) {
    arg0K0 = (0 - b_169X);
    goto L8912;}
  else {
    arg0K0 = b_169X;
    goto L8912;}}
 L30435: {
  val_495X = arg0K0;
  SvalS = val_495X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L30345: {
  val_496X = arg0K0;
  SvalS = val_496X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L30255: {
  val_497X = arg0K0;
  SvalS = val_497X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L30150: {
  val_498X = arg0K0;
  SvalS = val_498X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L30043: {
  val_499X = arg0K0;
  SvalS = val_499X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L34164: {
  a_500X = arg0K0;
  if ((b_183X < 0)) {
    arg0K0 = (0 - b_183X);
    goto L34166;}
  else {
    arg0K0 = b_183X;
    goto L34166;}}
 L34032: {
  a_501X = arg0K0;
  n_502X = ((x_185X)>>2);
  if ((n_502X < 0)) {
    arg0K0 = (0 - n_502X);
    goto L34034;}
  else {
    arg0K0 = n_502X;
    goto L34034;}}
 L28018: {
  r_503X = arg0K0;
  if ((536870911 < r_503X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 100;
    goto push_exception_continuationB;
   push_exception_continuationB_return_100:
    *((long *) (SstackS)) = x_196X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 1;
    goto L17068;}
  else {
    SvalS = (((r_503X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L33882: {
  SvalS = (((result_212X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L38937: {
  val_504X = arg0K0;
  SvalS = val_504X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L29915: {
  val_505X = arg0K0;
  SvalS = val_505X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L29888: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 101;
  goto push_exception_continuationB;
 push_exception_continuationB_return_101:
  *((long *) (SstackS)) = arg2_215X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L29825: {
  val_506X = arg0K0;
  SvalS = val_506X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L29798: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 102;
  goto push_exception_continuationB;
 push_exception_continuationB_return_102:
  *((long *) (SstackS)) = arg2_217X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L27918: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 103;
  goto push_exception_continuationB;
 push_exception_continuationB_return_103:
  *((long *) (SstackS)) = (((x_219X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L38966: {
  val_507X = arg0K0;
  SvalS = val_507X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L38980: {
  value_508X = arg0K0;
  SvalS = value_508X;
  Scode_pointerS = ((Scode_pointerS) + 2);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L24394: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 104;
  goto push_exception_continuationB;
 push_exception_continuationB_return_104:
  *((long *) (SstackS)) = x_223X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_224X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L24200: {
  type_509X = *((unsigned char *) ((Scode_pointerS) + 2));
  len_510X = ((len_225X)<<2);
  addr_511X = ALLOCATE_SPACE(type_509X, (4 + len_510X));
  *((long *) addr_511X) = (2 + (((((((len_510X)<<6)) + type_509X))<<2)));
  new_512X = 3 + (((long) (addr_511X + 4)));
  if ((len_225X < 1)) {
    goto L24235;}
  else {
    *((long *) ((((char *) (-3 + new_512X))) + (-4 + (((len_225X)<<2))))) = (SvalS);
    arg0K0 = (-2 + len_225X);
    goto L24219;}}
 L23991: {
  type_513X = *((unsigned char *) ((Scode_pointerS) + 1));
  len_514X = ((len_229X)<<2);
  addr_515X = ALLOCATE_SPACE(type_513X, (4 + len_514X));
  *((long *) addr_515X) = (2 + (((((((len_514X)<<6)) + type_513X))<<2)));
  new_516X = 3 + (((long) (addr_515X + 4)));
  SstackS = ((SstackS) + 4);
  stack_nargs_517X = (((*((long *) (SstackS))))>>2);
  SstackS = ((SstackS) + 4);
  rest_list_518X = *((long *) (SstackS));
  arg0K0 = (-1 + stack_nargs_517X);
  goto L24013;}
 L23889: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 3;
  push_exception_continuationB_return_tag = 105;
  goto push_exception_continuationB;
 push_exception_continuationB_return_105:
  *((long *) (SstackS)) = x_233X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_235X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_234X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L23677: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 3;
  push_exception_continuationB_return_tag = 106;
  goto push_exception_continuationB;
 push_exception_continuationB_return_106:
  *((long *) (SstackS)) = arg2_236X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_239X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((offset_238X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_237X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L17068;}
 L23349: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 107;
  goto push_exception_continuationB;
 push_exception_continuationB_return_107:
  *((long *) (SstackS)) = (((type_243X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((len_244X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_242X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L23367: {
  okayP_519X = arg2K0;
  init_520X = arg0K1;
  if (okayP_519X) {
    len_521X = ((len_244X)<<2);
    addr_522X = ALLOCATE_SPACE(type_243X, (4 + len_521X));
    *((long *) addr_522X) = (2 + (((((((len_521X)<<6)) + type_243X))<<2)));
    value_523X = 3 + (((long) (addr_522X + 4)));
    arg0K0 = (-1 + len_244X);
    goto L23396;}
  else {
    merged_arg0K0 = 8;
    merged_arg0K1 = 2;
    push_exception_continuationB_return_tag = 108;
    goto push_exception_continuationB;
   push_exception_continuationB_return_108:
    *((long *) (SstackS)) = (((type_243X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((len_244X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = init_520X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L17068;}}
 L23086: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 109;
  goto push_exception_continuationB;
 push_exception_continuationB_return_109:
  *((long *) (SstackS)) = arg2_249X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_251X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_250X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L23063: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 110;
  goto push_exception_continuationB;
 push_exception_continuationB_return_110:
  *((long *) (SstackS)) = arg2_249X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_251X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_250X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L22727: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 111;
  goto push_exception_continuationB;
 push_exception_continuationB_return_111:
  *((long *) (SstackS)) = arg3_255X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_257X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_254X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_256X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L17068;}
 L22702: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 112;
  goto push_exception_continuationB;
 push_exception_continuationB_return_112:
  *((long *) (SstackS)) = arg3_255X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((type_257X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_254X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_256X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L17068;}
 L27679: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 113;
  goto push_exception_continuationB;
 push_exception_continuationB_return_113:
  *((long *) (SstackS)) = arg3_262X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_264X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_263X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L27660: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 114;
  goto push_exception_continuationB;
 push_exception_continuationB_return_114:
  *((long *) (SstackS)) = arg3_262X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_264X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_263X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L27392: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 115;
  goto push_exception_continuationB;
 push_exception_continuationB_return_115:
  *((long *) (SstackS)) = (((len_268X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((init_269X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L27491: {
  addr_524X = ALLOCATE_SPACE(20, (4 + len_268X));
  *((long *) addr_524X) = (82 + (((len_268X)<<8)));
  vector_525X = 3 + (((long) (addr_524X + 4)));
  arg0K0 = (-1 + len_268X);
  goto L27437;}
 L36172: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 116;
  goto push_exception_continuationB;
 push_exception_continuationB_return_116:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L34988: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 117;
  goto push_exception_continuationB;
 push_exception_continuationB_return_117:
  *((long *) (SstackS)) = arg2_274X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_275X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L36731: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 118;
  goto push_exception_continuationB;
 push_exception_continuationB_return_118:
  *((long *) (SstackS)) = arg2_274X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L33694: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 119;
  goto push_exception_continuationB;
 push_exception_continuationB_return_119:
  *((long *) (SstackS)) = arg3_278X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_279X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_280X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L33673: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 120;
  goto push_exception_continuationB;
 push_exception_continuationB_return_120:
  *((long *) (SstackS)) = arg3_278X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_279X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((Kchar_280X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L35539: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 121;
  goto push_exception_continuationB;
 push_exception_continuationB_return_121:
  *((long *) (SstackS)) = arg3_278X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_277X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L27153: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 122;
  goto push_exception_continuationB;
 push_exception_continuationB_return_122:
  *((long *) (SstackS)) = (((len_283X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((init_284X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L27252: {
  len_526X = 1 + len_283X;
  addr_527X = ALLOCATE_SPACE(19, (4 + len_526X));
  *((long *) addr_527X) = (78 + (((len_526X)<<8)));
  string_528X = 3 + (((long) (addr_527X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_528X))) + len_283X)) = 0;
  arg0K0 = (-1 + len_283X);
  goto L27198;}
 L34765: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 123;
  goto push_exception_continuationB;
 push_exception_continuationB_return_123:
  *((long *) (SstackS)) = arg2_282X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L36115: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 124;
  goto push_exception_continuationB;
 push_exception_continuationB_return_124:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L35059: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 125;
  goto push_exception_continuationB;
 push_exception_continuationB_return_125:
  *((long *) (SstackS)) = arg2_289X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_290X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L36824: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 126;
  goto push_exception_continuationB;
 push_exception_continuationB_return_126:
  *((long *) (SstackS)) = arg2_289X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L33539: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 127;
  goto push_exception_continuationB;
 push_exception_continuationB_return_127:
  *((long *) (SstackS)) = arg3_293X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_294X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_295X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L33518: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 128;
  goto push_exception_continuationB;
 push_exception_continuationB_return_128:
  *((long *) (SstackS)) = arg3_293X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_294X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (9 + ((((Kchar_295X))<<8)));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L35413: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 129;
  goto push_exception_continuationB;
 push_exception_continuationB_return_129:
  *((long *) (SstackS)) = arg3_293X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_292X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L29669: {
  val_529X = arg0K0;
  SvalS = val_529X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L29641: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 130;
  goto push_exception_continuationB;
 push_exception_continuationB_return_130:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L31134: {
  if ((1 == (SvalS))) {
    addr_530X = (((char *) (-3 + arg2_300X))) + 8;
    WRITE_BARRIER(addr_530X, 273);
    *((long *) addr_530X) = 273;
    goto L31159;}
  else {
    if ((17 == (255 & (*((long *) ((((char *) (-3 + arg2_300X))) + 8)))))) {
      addr_531X = (((char *) (-3 + arg2_300X))) + 8;
      WRITE_BARRIER(addr_531X, 529);
      *((long *) addr_531X) = 529;
      goto L31159;}
    else {
      goto L31159;}}}
 L31160: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 131;
  goto push_exception_continuationB;
 push_exception_continuationB_return_131:
  *((long *) (SstackS)) = arg2_300X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L39045: {
  val_532X = arg0K0;
  SvalS = val_532X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L27102: {
  SvalS = x_303X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L26701: {
  okayP_533X = arg2K0;
  key_534X = arg0K1;
  if (okayP_533X) {
    arg0K0 = key_534X;
    goto L26550;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_534X;
    goto L26550;}}
 L33400: {
  okayP_535X = arg2K0;
  key_536X = arg0K1;
  if (okayP_535X) {
    arg0K0 = key_536X;
    goto L33355;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_536X;
    goto L33355;}}
 L37957: {
  okayP_537X = arg2K0;
  key_538X = arg0K1;
  if (okayP_537X) {
    arg0K0 = key_538X;
    goto L37887;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_538X;
    goto L37887;}}
 L38172: {
  okayP_539X = arg2K0;
  key_540X = arg0K1;
  if (okayP_539X) {
    arg0K0 = key_540X;
    goto L38108;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_540X;
    goto L38108;}}
 L26502: {
  val_541X = arg0K0;
  SvalS = val_541X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L8211: {
  arg0K0 = (*((long *) ((((char *) (-3 + channel_313X))) + 16)));
  goto L26502;}
 L8217: {
  ch_542X = arg0K0;
  prev_543X = arg0K1;
  if ((1 == ch_542X)) {
    n_544X = ps_abort_fd_op(((((*((long *) ((((char *) (-3 + channel_313X))) + 8))))>>2)));
    arg0K0 = (((n_544X)<<2));
    goto L26502;}
  else {
    if ((ch_542X == channel_313X)) {
      y_545X = Spending_channels_tailS;
      if ((ch_542X == y_545X)) {
        Spending_channels_tailS = prev_543X;
        goto L8239;}
      else {
        goto L8239;}}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + ch_542X))) + 12)));
      arg0K1 = ch_542X;
      goto L8217;}}}
 L26503: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 132;
  goto push_exception_continuationB;
 push_exception_continuationB_return_132:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L29507: {
  val_546X = arg0K0;
  SvalS = val_546X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L29478: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 133;
  goto push_exception_continuationB;
 push_exception_continuationB_return_133:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L26436: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 134;
  goto push_exception_continuationB;
 push_exception_continuationB_return_134:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L33042: {
  okayP_547X = arg2K0;
  key_548X = arg0K1;
  if (okayP_547X) {
    arg0K0 = key_548X;
    goto L32906;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_548X;
    goto L32906;}}
 L32660: {
  SstackS = ((SstackS) + 4);
  arg2_549X = *((long *) (SstackS));
  x_550X = SvalS;
  if ((3 == (3 & arg2_549X))) {
    if ((3 == (3 & x_550X))) {
      if ((3 == (31 & ((((*((long *) ((((char *) (-3 + x_550X))) + -4))))>>2))))) {
        addr_551X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_551X) = 2050;
        x_552X = 3 + (((long) (addr_551X + 4)));
        *((long *) (((char *) (-3 + x_552X)))) = arg2_549X;
        *((long *) ((((char *) (-3 + x_552X))) + 4)) = x_550X;
        b_553X = Sfinalizer_alistS;
        addr_554X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_554X) = 2050;
        x_555X = 3 + (((long) (addr_554X + 4)));
        *((long *) (((char *) (-3 + x_555X)))) = x_552X;
        *((long *) ((((char *) (-3 + x_555X))) + 4)) = b_553X;
        Sfinalizer_alistS = x_555X;
        SvalS = 13;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        goto L32760;}}
    else {
      goto L32760;}}
  else {
    goto L32760;}}
 L35946: {
  arg0K0 = 0;
  arg0K1 = 0;
  goto L9391;}
 L26300: {
  firstP_556X = arg2K0;
  vector_557X = find_all(type_337X);
  if ((1 == vector_557X)) {
    if (firstP_556X) {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 24;
      goto collect_saving_temp;
     collect_saving_temp_return_24:
      arg2K0 = 0;
      goto L26300;}
    else {
      merged_arg0K0 = 8;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 135;
      goto push_exception_continuationB;
     push_exception_continuationB_return_135:
      *((long *) (SstackS)) = (((type_337X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L17068;}}
  else {
    SvalS = vector_557X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L35273: {
  table_558X = arg0K0;
  firstP_559X = arg2K1;
  symbols_560X = find_all(1);
  if ((1 == symbols_560X)) {
    if (firstP_559X) {
      merged_arg0K0 = table_558X;
      collect_saving_temp_return_tag = 25;
      goto collect_saving_temp;
     collect_saving_temp_return_25:
      v_561X = collect_saving_temp0_return_value;
      arg0K0 = v_561X;
      arg2K1 = 0;
      goto L35273;}
    else {
      merged_arg0K0 = 8;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 136;
      goto push_exception_continuationB;
     push_exception_continuationB_return_136:
      *((long *) (SstackS)) = table_558X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L17068;}}
  else {
    arg0K0 = table_558X;
    arg0K1 = symbols_560X;
    arg2K2 = 1;
    goto L31327;}}
 L35295: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 137;
  goto push_exception_continuationB;
 push_exception_continuationB_return_137:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L39120: {
  firstP_562X = arg2K0;
  type_563X = arg0K1;
  vector_564X = find_all_records(type_563X);
  if ((1 == vector_564X)) {
    if (firstP_562X) {
      merged_arg0K0 = type_563X;
      collect_saving_temp_return_tag = 26;
      goto collect_saving_temp;
     collect_saving_temp_return_26:
      v_565X = collect_saving_temp0_return_value;
      arg2K0 = 0;
      arg0K1 = v_565X;
      goto L39120;}
    else {
      merged_arg0K0 = 8;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 138;
      goto push_exception_continuationB;
     push_exception_continuationB_return_138:
      *((long *) (SstackS)) = type_563X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L17068;}}
  else {
    SvalS = vector_564X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L21340: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 139;
  goto push_exception_continuationB;
 push_exception_continuationB_return_139:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L20181: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 140;
  goto push_exception_continuationB;
 push_exception_continuationB_return_140:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L39161: {
  SvalS = (((old_348X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L19184: {
  SstackS = ((SstackS) + 4);
  pc_566X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  tem_567X = *((long *) (SstackS));
  StemplateS = tem_567X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_567X)))))))) + (((pc_566X)>>2)));
  SstackS = ((SstackS) + 4);
  SvalS = (*((long *) (SstackS)));
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L35843: {
  x_568X = schedule_alarm_interrupt((((p_351X)>>2)));
  SvalS = (((x_568X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L30902: {
  if ((1 == (SvalS))) {
    arg2K0 = 0;
    goto L30906;}
  else {
    arg2K0 = 1;
    goto L30906;}}
 L30922: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 141;
  goto push_exception_continuationB;
 push_exception_continuationB_return_141:
  *((long *) (SstackS)) = arg2_352X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L32516: {
  merged_arg0K0 = 19;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 142;
  goto push_exception_continuationB;
 push_exception_continuationB_return_142:
  *((long *) (SstackS)) = x_355X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L32521: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 143;
  goto push_exception_continuationB;
 push_exception_continuationB_return_143:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L17750: {
  rest_list_569X = arg0K0;
  if ((25 == rest_list_569X)) {
    proc_570X = *((long *) ((SstackS) + (((nargs_359X)<<2))));
    if ((3 == (3 & proc_570X))) {
      if ((13 == (31 & ((((*((long *) ((((char *) (-3 + proc_570X))) + -4))))>>2))))) {
        Sexternal_call_nargsS = (-1 + nargs_359X);
        SvalS = proc_570X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg0K0 = 1;
        goto L20076;}
      else {
        goto L17820;}}
    else {
      goto L17820;}}
  else {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + rest_list_569X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (*((long *) ((((char *) (-3 + rest_list_569X))) + 4)));
    goto L17750;}}
 L35710: {
  option_571X = arg0K0;
  seconds_572X = arg0K1;
  mseconds_573X = arg0K2;
  if ((536869 < seconds_572X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 144;
    goto push_exception_continuationB;
   push_exception_continuationB_return_144:
    *((long *) (SstackS)) = (((option_571X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((seconds_572X)<<2));
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mseconds_573X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 3;
    goto L17068;}
  else {
    SvalS = (((((1000 * seconds_572X) + mseconds_573X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L20076: {
  option_574X = arg0K0;
  if ((0 == option_574X)) {
    return (SvalS);}
  else {
    if ((1 == option_574X)) {
      v_575X = (long(*)())((*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + (SvalS)))) + 4)))))))));
      v_576X = (*v_575X)((Sexternal_call_nargsS), ((SstackS) + 4));
      SvalS = v_576X;
      SstackS = ((SstackS) + (0 - ((((-1 - (Sexternal_call_nargsS)))<<2))));
      goto L20074;}
    else {
      ps_error("unknown VM return option", 1, option_574X);
      return -1;}}}
 L25832: {
  SstackS = ((SstackS) + 4);
  arg2_577X = *((long *) (SstackS));
  obj_578X = SvalS;
  if ((3 == (3 & obj_578X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + obj_578X))) + -4))))>>2))))) {
      x_579X = SvalS;
      addr_580X = ALLOCATE_SPACE(0, 12);
      *((long *) addr_580X) = 2050;
      x_581X = 3 + (((long) (addr_580X + 4)));
      *((long *) (((char *) (-3 + x_581X)))) = 1;
      *((long *) ((((char *) (-3 + x_581X))) + 4)) = 1;
      merged_arg0K0 = arg2_577X;
      merged_arg0K1 = x_579X;
      merged_arg0K2 = x_581X;
      encode_return_tag = 0;
      goto encode;
     encode_return_0:
      v_582X = encode0_return_value;
      if (v_582X) {
        SvalS = x_581X;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        *((long *) (SstackS)) = x_579X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_581X;
        SstackS = ((SstackS) + -4);
        merged_arg0K0 = arg2_577X;
        collect_saving_temp_return_tag = 27;
        goto collect_saving_temp;
       collect_saving_temp_return_27:
        thing_583X = collect_saving_temp0_return_value;
        SstackS = ((SstackS) + 4);
        pair_584X = *((long *) (SstackS));
        SstackS = ((SstackS) + 4);
        aspace_585X = *((long *) (SstackS));
        merged_arg0K0 = thing_583X;
        merged_arg0K1 = aspace_585X;
        merged_arg0K2 = pair_584X;
        encode_return_tag = 1;
        goto encode;
       encode_return_1:
        v_586X = encode0_return_value;
        if (v_586X) {
          SvalS = pair_584X;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          merged_arg0K0 = 8;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 145;
          goto push_exception_continuationB;
         push_exception_continuationB_return_145:
          *((long *) (SstackS)) = thing_583X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = aspace_585X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}}}
    else {
      goto L25892;}}
  else {
    goto L25892;}}
 L25563: {
  SstackS = ((SstackS) + 4);
  arg2_587X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_588X = *((long *) (SstackS));
  if ((3 == (3 & arg3_588X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + arg3_588X))) + -4))))>>2))))) {
      if ((1 == arg2_587X)) {
        goto L25581;}
      else {
        if ((5 == arg2_587X)) {
          goto L25581;}
        else {
          goto L25620;}}}
    else {
      goto L25620;}}
  else {
    goto L25620;}}
 L29376: {
  val_589X = arg0K0;
  SvalS = val_589X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L29343: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 146;
  goto push_exception_continuationB;
 push_exception_continuationB_return_146:
  *((long *) (SstackS)) = arg2_379X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L35799: {
  i_590X = arg0K0;
  h_591X = arg0K1;
  if ((i_590X < n_385X)) {
    arg0K0 = (1 + i_590X);
    arg0K1 = (h_591X + (((*((unsigned char *) ((((char *) (-3 + x_384X))) + i_590X))))));
    goto L35799;}
  else {
    SvalS = (((h_591X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L35776: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 147;
  goto push_exception_continuationB;
 push_exception_continuationB_return_147:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L25269: {
  SstackS = ((SstackS) + 4);
  arg2_592X = *((long *) (SstackS));
  if ((0 == (3 & (SvalS)))) {
    n_593X = (((SvalS))>>2);
    if ((3 == (3 & arg2_592X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + arg2_592X))) + -4))))>>2))))) {
        goto L25397;}
      else {
        goto L25302;}}
    else {
      goto L25302;}}
  else {
    merged_arg0K0 = 5;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 148;
    goto push_exception_continuationB;
   push_exception_continuationB_return_148:
    *((long *) (SstackS)) = arg2_592X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}}
 L25127: {
  SstackS = ((SstackS) + 4);
  arg2_594X = *((long *) (SstackS));
  if ((3 == (3 & arg2_594X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_594X))) + -4))))>>2))))) {
      obj_595X = SvalS;
      if ((3 == (3 & obj_595X))) {
        if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_595X))) + -4))))>>2))))) {
          table_596X = SvalS;
          v_597X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_596X))) + -4))))>>8))))>>2);
          n_598X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_594X))) + -4))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L12920;}
        else {
          goto L25151;}}
      else {
        goto L25151;}}
    else {
      goto L25151;}}
  else {
    goto L25151;}}
 L29068: {
  list_599X = arg0K0;
  slow_600X = arg0K1;
  move_slowP_601X = arg2K2;
  if ((25 == list_599X)) {
    SvalS = 1;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    if ((3 == (3 & list_599X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + list_599X))) + -4))))>>2))))) {
        head_602X = *((long *) (((char *) (-3 + list_599X))));
        if ((3 == (3 & head_602X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + head_602X))) + -4))))>>2))))) {
            if (((*((long *) (((char *) (-3 + head_602X))))) == arg2_391X)) {
              SvalS = head_602X;
              Scode_pointerS = ((Scode_pointerS) + 1);
              arg1K0 = (Scode_pointerS);
              goto L19170;}
            else {
              list_603X = *((long *) ((((char *) (-3 + list_599X))) + 4));
              if ((list_603X == slow_600X)) {
                merged_arg0K0 = 5;
                merged_arg0K1 = 1;
                push_exception_continuationB_return_tag = 149;
                goto push_exception_continuationB;
               push_exception_continuationB_return_149:
                *((long *) (SstackS)) = arg2_391X;
                SstackS = ((SstackS) + -4);
                *((long *) (SstackS)) = x_392X;
                SstackS = ((SstackS) + -4);
                arg0K0 = 2;
                goto L17068;}
              else {
                if (move_slowP_601X) {
                  arg0K0 = list_603X;
                  arg0K1 = (*((long *) ((((char *) (-3 + slow_600X))) + 4)));
                  arg2K2 = 0;
                  goto L29068;}
                else {
                  arg0K0 = list_603X;
                  arg0K1 = slow_600X;
                  arg2K2 = 1;
                  goto L29068;}}}}
          else {
            merged_arg0K0 = 5;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 150;
            goto push_exception_continuationB;
           push_exception_continuationB_return_150:
            *((long *) (SstackS)) = arg2_391X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_392X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L17068;}}
        else {
          merged_arg0K0 = 5;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 151;
          goto push_exception_continuationB;
         push_exception_continuationB_return_151:
          *((long *) (SstackS)) = arg2_391X;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = x_392X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 152;
        goto push_exception_continuationB;
       push_exception_continuationB_return_152:
        *((long *) (SstackS)) = arg2_391X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = x_392X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
    else {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 153;
      goto push_exception_continuationB;
     push_exception_continuationB_return_153:
      *((long *) (SstackS)) = arg2_391X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = x_392X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L17068;}}}
 L24913: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 154;
  goto push_exception_continuationB;
 push_exception_continuationB_return_154:
  *((long *) (SstackS)) = arg3_394X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_393X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_395X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L24960: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 155;
  goto push_exception_continuationB;
 push_exception_continuationB_return_155:
  *((long *) (SstackS)) = arg3_394X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_393X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_395X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L32167: {
  merged_arg0K0 = 7;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 156;
  goto push_exception_continuationB;
 push_exception_continuationB_return_156:
  *((long *) (SstackS)) = arg4_399X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_398X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_401X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_400X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L17068;}
 L32224: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 157;
  goto push_exception_continuationB;
 push_exception_continuationB_return_157:
  *((long *) (SstackS)) = arg4_399X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_398X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((index_401X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_400X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L17068;}
 L31531: {
  if ((3 == (3 & arg5_407X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg5_407X))) + -4))))>>2))))) {
      goto L31544;}
    else {
      goto L31539;}}
  else {
    goto L31539;}}
 L31526: {
  if ((3 == (3 & arg5_407X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg5_407X))) + -4))))>>2))))) {
      goto L31531;}
    else {
      goto L31598;}}
  else {
    goto L31598;}}
 L22434: {
  port_604X = arg0K0;
  if ((3 == (3 & port_604X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_604X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_604X))) + 4))))>>2))))) {
        goto L22494;}
      else {
        p_605X = *((long *) ((((char *) (-3 + port_604X))) + 24));
        p_606X = *((long *) ((((char *) (-3 + port_604X))) + 28));
        b_607X = *((long *) ((((char *) (-3 + port_604X))) + 20));
        i_608X = ((p_605X)>>2);
        x_609X = *((long *) ((((char *) (-3 + port_604X))) + 12));
        if ((5 == x_609X)) {
          goto L22474;}
        else {
          if ((i_608X == (((p_606X)>>2)))) {
            goto L22474;}
          else {
            val_610X = 4 + (((i_608X)<<2));
            addr_611X = (((char *) (-3 + port_604X))) + 24;
            WRITE_BARRIER(addr_611X, val_610X);
            *((long *) addr_611X) = val_610X;
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_607X))) + i_608X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L19170;}}}}
    else {
      goto L22494;}}
  else {
    goto L22494;}}
 L22231: {
  port_612X = arg0K0;
  if ((3 == (3 & port_612X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_612X))) + -4))))>>2))))) {
      if ((0 == (4 & ((((*((long *) ((((char *) (-3 + port_612X))) + 4))))>>2))))) {
        goto L22291;}
      else {
        p_613X = *((long *) ((((char *) (-3 + port_612X))) + 24));
        p_614X = *((long *) ((((char *) (-3 + port_612X))) + 28));
        b_615X = *((long *) ((((char *) (-3 + port_612X))) + 20));
        i_616X = ((p_613X)>>2);
        x_617X = *((long *) ((((char *) (-3 + port_612X))) + 12));
        if ((5 == x_617X)) {
          goto L22271;}
        else {
          if ((i_616X == (((p_614X)>>2)))) {
            goto L22271;}
          else {
            SvalS = (9 + ((((((*((unsigned char *) ((((char *) (-3 + b_615X))) + i_616X))))))<<8)));
            Scode_pointerS = ((Scode_pointerS) + 2);
            arg1K0 = (Scode_pointerS);
            goto L19170;}}}}
    else {
      goto L22291;}}
  else {
    goto L22291;}}
 L21978: {
  Kchar_618X = arg0K0;
  port_619X = arg0K1;
  if ((9 == (255 & Kchar_618X))) {
    if ((3 == (3 & port_619X))) {
      if ((6 == (31 & ((((*((long *) ((((char *) (-3 + port_619X))) + -4))))>>2))))) {
        if ((0 == (8 & ((((*((long *) ((((char *) (-3 + port_619X))) + 4))))>>2))))) {
          goto L22039;}
        else {
          p_620X = *((long *) ((((char *) (-3 + port_619X))) + 24));
          b_621X = *((long *) ((((char *) (-3 + port_619X))) + 20));
          i_622X = ((p_620X)>>2);
          x_623X = *((long *) ((((char *) (-3 + port_619X))) + 12));
          if ((5 == x_623X)) {
            goto L22021;}
          else {
            if ((i_622X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + b_621X))) + -4))))>>8)))) {
              goto L22021;}
            else {
              val_624X = 4 + (((i_622X)<<2));
              addr_625X = (((char *) (-3 + port_619X))) + 24;
              WRITE_BARRIER(addr_625X, val_624X);
              *((long *) addr_625X) = val_624X;
              *((unsigned char *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + port_619X))) + 20)))))) + i_622X)) = (((((Kchar_618X)>>8))));
              SvalS = 13;
              Scode_pointerS = ((Scode_pointerS) + 2);
              arg1K0 = (Scode_pointerS);
              goto L19170;}}}}
      else {
        goto L22039;}}
    else {
      goto L22039;}}
  else {
    goto L22039;}}
 L30811: {
  stuff_626X = arg0K0;
  if ((3 == (3 & stuff_626X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + stuff_626X))) + -4))))>>2))))) {
      thing_627X = *((long *) (((char *) (-3 + stuff_626X))));
      if ((0 == (3 & thing_627X))) {
        ps_write_integer((((thing_627X)>>2)), out_418X);
        goto L30818;}
      else {
        if ((9 == (255 & thing_627X))) {
          ps_write_string("#\\", out_418X);
          { long ignoreXX;
          PS_WRITE_CHAR(((((thing_627X)>>8))), out_418X, ignoreXX) }
          goto L30818;}
        else {
          if ((3 == (3 & thing_627X))) {
            if ((8 == (31 & ((((*((long *) ((((char *) (-3 + thing_627X))) + -4))))>>2))))) {
              if ((0 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thing_627X))) + -4))))>>8))))>>2)))) {
                type_628X = *((long *) (((char *) (-3 + thing_627X))));
                if ((3 == (3 & type_628X))) {
                  if ((8 == (31 & ((((*((long *) ((((char *) (-3 + type_628X))) + -4))))>>2))))) {
                    if ((2 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + type_628X))) + -4))))>>8))))>>2)))) {
                      obj_629X = *((long *) ((((char *) (-3 + type_628X))) + 8));
                      if ((3 == (3 & obj_629X))) {
                        if ((1 == (31 & ((((*((long *) ((((char *) (-3 + obj_629X))) + -4))))>>2))))) {
                          ps_write_string("#{", out_418X);
                          ps_write_string((((char *)(((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + thing_627X)))))))) + 8)))))) + 4)))))))), out_418X);
                          { long ignoreXX;
                          PS_WRITE_CHAR(125, out_418X, ignoreXX) }
                          goto L30818;}
                        else {
                          goto L11984;}}
                      else {
                        goto L11984;}}
                    else {
                      goto L11984;}}
                  else {
                    goto L11984;}}
                else {
                  goto L11984;}}
              else {
                goto L11984;}}
            else {
              goto L11984;}}
          else {
            goto L11984;}}}}
    else {
      goto L30826;}}
  else {
    goto L30826;}}
 L17127: {
  SvalS = (*((long *) ((((char *) (-3 + (Sexception_handlersS)))) + (((opcode_420X)<<2)))));
  obj_630X = SvalS;
  if ((3 == (3 & obj_630X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_630X))) + -4))))>>2))))) {
      goto L17144;}
    else {
      goto L17214;}}
  else {
    goto L17214;}}
 L17200: {
  merged_arg3K0 = "exception-handlers is not a vector";
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L17127;}
 L21830: {
  okayP_631X = arg2K0;
  key_632X = arg0K1;
  if (okayP_631X) {
    arg0K0 = key_632X;
    goto L21734;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_632X;
    goto L21734;}}
 L21764: {
  okayP_633X = arg2K0;
  temp_634X = arg0K1;
  if (okayP_633X) {
    arg0K0 = temp_634X;
    goto L21745;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_634X;
    goto L21745;}}
 L21549: {
  start_i_635X = arg0K0;
  arg0K0 = start_i_635X;
  arg0K1 = 2;
  arg0K2 = (SenvS);
  goto L21557;}
 L21450: {
  key_636X = arg0K0;
  if ((1 == (ScontS))) {
    arg0K0 = 1;
    goto L21454;}
  else {
    merged_arg0K0 = key_636X;
    merged_arg0K1 = 2;
    really_preserve_continuation_return_tag = 0;
    goto really_preserve_continuation;
   really_preserve_continuation_return_0:
    v_637X = really_preserve_continuation0_return_value;
    arg0K0 = v_637X;
    goto L21454;}}
 L20552: {
  SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
  ScontS = (Sbottom_of_stackS);
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 158;
  goto push_exception_continuationB;
 push_exception_continuationB_return_158:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = cont_98X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L19893: {
  v_638X = arg0K0;
  merged_arg0K0 = v_638X;
  copy_stack_into_heap_return_tag = 0;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_0:
  if (((SstackS) < (Sstack_limitS))) {
    ps_error("Couldn't get default procedure space (how can this happen?)", 0);
    goto L19850;}
  else {
    goto L19850;}}
 L17297: {
  stack_arg_count_639X = arg0K0;
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (StemplateS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  tem_640X = Sinterrupt_templateS;
  StemplateS = tem_640X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_640X))))))));
  merged_arg1K0 = (Scode_pointerS);
  merged_arg0K1 = (4 + stack_arg_count_639X);
  push_continuationB_return_tag = 2;
  goto push_continuationB;
 push_continuationB_return_2:
  n_641X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L17402;}
 L19854: {
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L16483: {
  protocol_642X = arg0K0;
  stack_space_643X = arg0K1;
  if ((68 == protocol_642X)) {
    if ((stack_arg_count_449X < 3)) {
      skip_644X = *((unsigned char *) ((((char *) (-3 + code_450X))) + (2 + stack_arg_count_449X)));
      if ((0 == skip_644X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_449X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17564;}
      else {
        arg0K0 = skip_644X;
        arg0K1 = stack_arg_count_449X;
        goto L16487;}}
    else {
      skip_645X = *((unsigned char *) ((((char *) (-3 + code_450X))) + 5));
      if ((0 == skip_645X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_449X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17564;}
      else {
        arg0K0 = skip_645X;
        goto L16510;}}}
  else {
    if ((67 == protocol_642X)) {
      if ((stack_arg_count_449X < (*((unsigned char *) ((((char *) (-3 + code_450X))) + 2))))) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_449X;
        arg0K2 = 25;
        arg0K3 = 0;
        goto L17564;}
      else {
        arg0K0 = 3;
        goto L16510;}}
    else {
      if ((65 == protocol_642X)) {
        wants_stack_args_646X = ((((*((unsigned char *) ((((char *) (-3 + code_450X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_450X))) + 3)));
        if ((stack_arg_count_449X < wants_stack_args_646X)) {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_449X;
          arg0K2 = 25;
          arg0K3 = 0;
          goto L17564;}
        else {
          merged_arg0K0 = wants_stack_args_646X;
          merged_arg0K1 = stack_arg_count_449X;
          merged_arg0K2 = 25;
          merged_arg0K3 = 0;
          rest_list_setup_return_tag = 0;
          goto rest_list_setup;
         rest_list_setup_return_0:
          arg0K0 = 4;
          arg0K1 = (1 + wants_stack_args_646X);
          goto L16487;}}
      else {
        if ((63 < protocol_642X)) {
          if ((64 == protocol_642X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_450X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_450X))) + 3)))) == stack_arg_count_449X)) {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_449X;
              goto L16487;}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_449X;
              arg0K2 = 25;
              arg0K3 = 0;
              goto L17564;}}
          else {
            if ((66 == protocol_642X)) {
              length_647X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_450X))) + -4))))>>8);
              index_648X = -2 + length_647X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_450X))) + (-3 + length_647X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_450X))) + index_648X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_450X))) + (1 + index_648X)))));
              goto L16483;}
            else {
              ps_error("unknown protocol", 1, protocol_642X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_449X;
              arg0K2 = 25;
              arg0K3 = 0;
              goto L17564;}}}
        else {
          if ((protocol_642X == stack_arg_count_449X)) {
            arg0K0 = 2;
            arg0K1 = stack_arg_count_449X;
            goto L16487;}
          else {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_449X;
            arg0K2 = 25;
            arg0K3 = 0;
            goto L17564;}}}}}}
 L16760: {
  protocol_649X = arg0K0;
  stack_space_650X = arg0K1;
  if ((68 == protocol_649X)) {
    if ((total_arg_count_468X < 3)) {
      skip_651X = *((unsigned char *) ((((char *) (-3 + code_467X))) + (2 + total_arg_count_468X)));
      if ((0 == skip_651X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_462X;
        arg0K2 = list_args_463X;
        arg0K3 = list_arg_count_464X;
        goto L17564;}
      else {
        merged_arg0K0 = list_args_463X;
        merged_arg0K1 = list_arg_count_464X;
        push_list_return_tag = 0;
        goto push_list;
       push_list_return_0:
        arg0K0 = skip_651X;
        arg0K1 = total_arg_count_468X;
        goto L16764;}}
    else {
      skip_652X = *((unsigned char *) ((((char *) (-3 + code_467X))) + 5));
      if ((0 == skip_652X)) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_462X;
        arg0K2 = list_args_463X;
        arg0K3 = list_arg_count_464X;
        goto L17564;}
      else {
        arg0K0 = skip_652X;
        goto L16789;}}}
  else {
    if ((67 == protocol_649X)) {
      if ((total_arg_count_468X < (*((unsigned char *) ((((char *) (-3 + code_467X))) + 2))))) {
        arg0K0 = 4;
        arg0K1 = stack_arg_count_462X;
        arg0K2 = list_args_463X;
        arg0K3 = list_arg_count_464X;
        goto L17564;}
      else {
        arg0K0 = 3;
        goto L16789;}}
    else {
      if ((63 < protocol_649X)) {
        if ((65 == protocol_649X)) {
          wants_stack_args_653X = ((((*((unsigned char *) ((((char *) (-3 + code_467X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_467X))) + 3)));
          if ((total_arg_count_468X < wants_stack_args_653X)) {
            arg0K0 = 4;
            arg0K1 = stack_arg_count_462X;
            arg0K2 = list_args_463X;
            arg0K3 = list_arg_count_464X;
            goto L17564;}
          else {
            merged_arg0K0 = wants_stack_args_653X;
            merged_arg0K1 = stack_arg_count_462X;
            merged_arg0K2 = list_args_463X;
            merged_arg0K3 = list_arg_count_464X;
            rest_list_setup_return_tag = 1;
            goto rest_list_setup;
           rest_list_setup_return_1:
            arg0K0 = 4;
            arg0K1 = (1 + wants_stack_args_653X);
            goto L16764;}}
        else {
          if ((64 == protocol_649X)) {
            if (((((((*((unsigned char *) ((((char *) (-3 + code_467X))) + 2))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_467X))) + 3)))) == total_arg_count_468X)) {
              merged_arg0K0 = list_args_463X;
              merged_arg0K1 = list_arg_count_464X;
              push_list_return_tag = 1;
              goto push_list;
             push_list_return_1:
              arg0K0 = 4;
              arg0K1 = total_arg_count_468X;
              goto L16764;}
            else {
              arg0K0 = 4;
              arg0K1 = stack_arg_count_462X;
              arg0K2 = list_args_463X;
              arg0K3 = list_arg_count_464X;
              goto L17564;}}
          else {
            if ((66 == protocol_649X)) {
              length_654X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + code_467X))) + -4))))>>8);
              index_655X = -2 + length_654X;
              arg0K0 = (*((unsigned char *) ((((char *) (-3 + code_467X))) + (-3 + length_654X))));
              arg0K1 = (((((*((unsigned char *) ((((char *) (-3 + code_467X))) + index_655X))))<<8)) + (*((unsigned char *) ((((char *) (-3 + code_467X))) + (1 + index_655X)))));
              goto L16760;}
            else {
              ps_error("unknown protocol", 1, protocol_649X);
              arg0K0 = 4;
              arg0K1 = stack_arg_count_462X;
              arg0K2 = list_args_463X;
              arg0K3 = list_arg_count_464X;
              goto L17564;}}}}
      else {
        if ((protocol_649X == total_arg_count_468X)) {
          merged_arg0K0 = list_args_463X;
          merged_arg0K1 = list_arg_count_464X;
          push_list_return_tag = 2;
          goto push_list;
         push_list_return_2:
          arg0K0 = 2;
          arg0K1 = total_arg_count_468X;
          goto L16764;}
        else {
          arg0K0 = 4;
          arg0K1 = stack_arg_count_462X;
          arg0K2 = list_args_463X;
          arg0K3 = list_arg_count_464X;
          goto L17564;}}}}}
 L21115: {
  cont_656X = arg0K0;
  if ((3 == (3 & cont_656X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_656X))) + -4))))>>2))))) {
      next_op_657X = *((unsigned char *) ((((char *) (-3 + (*((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + cont_656X))) + 8))))))))))) + ((((*((long *) ((((char *) (-3 + cont_656X))) + 4))))>>2))));
      if ((34 == next_op_657X)) {
        pop_continuationB_return_tag = 2;
        goto pop_continuationB;
       pop_continuationB_return_2:
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        if ((30 == next_op_657X)) {
          next_658X = *((long *) (((char *) (-3 + (ScontS)))));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (((char *) (-3 + (ScontS))))) = (*((long *) (((char *) (-3 + next_658X)))));
            goto L20011;}
          else {
            ScontS = next_658X;
            goto L20011;}}
        else {
          merged_arg0K0 = list_args_480X;
          merged_arg0K1 = stack_nargs_479X;
          pop_args_GlistS_return_tag = 3;
          goto pop_args_GlistS;
         pop_args_GlistS_return_3:
          args_659X = pop_args_GlistS0_return_value;
          merged_arg0K0 = 4;
          merged_arg0K1 = 0;
          push_exception_continuationB_return_tag = 159;
          goto push_exception_continuationB;
         push_exception_continuationB_return_159:
          *((long *) (SstackS)) = 1;
          SstackS = ((SstackS) + -4);
          *((long *) (SstackS)) = args_659X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}}}
    else {
      goto L21121;}}
  else {
    goto L21121;}}
 L19664: {
  v_660X = arg0K0;
  merged_arg0K0 = v_660X;
  copy_stack_into_heap_return_tag = 1;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_1:
  if (((SstackS) < (Sstack_limitS))) {
    ps_error("Couldn't get default procedure space (how can this happen?)", 0);
    goto L19549;}
  else {
    goto L19549;}}
 L19558: {
  arg0K0 = (*((unsigned char *) ((Scode_pointerS) + 3)));
  goto L17297;}
 L19561: {
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L19718: {
  v_661X = arg0K0;
  merged_arg0K0 = v_661X;
  copy_stack_into_heap_return_tag = 2;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_2:
  if ((space_130X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L19549;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L19549;}}
 L30547: {
  if ((3 == (3 & x_149X))) {
    if ((21 == (31 & ((((*((long *) ((((char *) (-3 + x_149X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L30584;}
    else {
      goto L30555;}}
  else {
    goto L30555;}}
 L28881: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L28670: {
  if ((3 == (3 & x_151X))) {
    if ((21 == (31 & ((((*((long *) ((((char *) (-3 + x_151X))) + -4))))>>2))))) {
      goto L28679;}
    else {
      goto L28682;}}
  else {
    goto L28682;}}
 L28459: {
  if ((3 == (3 & x_152X))) {
    if ((21 == (31 & ((((*((long *) ((((char *) (-3 + x_152X))) + -4))))>>2))))) {
      goto L28468;}
    else {
      goto L28471;}}
  else {
    goto L28471;}}
 L28248: {
  if ((3 == (3 & x_153X))) {
    if ((21 == (31 & ((((*((long *) ((((char *) (-3 + x_153X))) + -4))))>>2))))) {
      goto L28257;}
    else {
      goto L28260;}}
  else {
    goto L28260;}}
 L7136: {
  b_662X = arg0K0;
  lo_a_663X = 65535 & a_493X;
  lo_b_664X = 65535 & b_662X;
  hi_a_665X = 65535 & (((a_493X)>>16));
  hi_b_666X = 65535 & (((b_662X)>>16));
  lo_c_667X = SMALL_MULTIPLY(lo_a_663X, lo_b_664X);
  v_668X = SMALL_MULTIPLY(lo_b_664X, hi_a_665X);
  v_669X = SMALL_MULTIPLY(lo_a_663X, hi_b_666X);
  mid_c_670X = v_669X + v_668X;
  c_671X = lo_c_667X + (((mid_c_670X)<<16));
  if ((0 < hi_a_665X)) {
    if ((0 < hi_b_666X)) {
      merged_arg0K0 = 6;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 160;
      goto push_exception_continuationB;
     push_exception_continuationB_return_160:
      *((long *) (SstackS)) = arg2_159X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = y_160X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L17068;}
    else {
      goto L7178;}}
  else {
    goto L7178;}}
 L8912: {
  b_672X = arg0K0;
  c_673X = a_494X / b_672X;
  if ((a_168X < 0)) {
    if ((b_169X < 0)) {
      goto L8916;}
    else {
      goto L8915;}}
  else {
    if ((b_169X < 0)) {
      goto L8915;}
    else {
      goto L8916;}}}
 L34166: {
  b_674X = arg0K0;
  c_675X = a_500X / b_674X;
  if ((a_182X < 0)) {
    if ((b_183X < 0)) {
      goto L34170;}
    else {
      goto L34169;}}
  else {
    if ((b_183X < 0)) {
      goto L34169;}
    else {
      goto L34170;}}}
 L34034: {
  b_676X = arg0K0;
  c_677X = a_501X % b_676X;
  if ((a_186X < 0)) {
    arg0K0 = (0 - c_677X);
    goto L34038;}
  else {
    arg0K0 = c_677X;
    goto L34038;}}
 L24235: {
  SvalS = new_512X;
  Scode_pointerS = ((Scode_pointerS) + 3);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L24219: {
  i_678X = arg0K0;
  if ((i_678X < 0)) {
    goto L24235;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_512X))) + (((i_678X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_678X);
    goto L24219;}}
 L24013: {
  i_679X = arg0K0;
  if ((i_679X < 0)) {
    arg0K0 = stack_nargs_517X;
    arg0K1 = rest_list_518X;
    goto L24031;}
  else {
    SstackS = ((SstackS) + 4);
    *((long *) ((((char *) (-3 + new_516X))) + (((i_679X)<<2)))) = (*((long *) (SstackS)));
    arg0K0 = (-1 + i_679X);
    goto L24013;}}
 L23396: {
  i_680X = arg0K0;
  if ((i_680X < 0)) {
    SvalS = value_523X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    addr_681X = (((char *) (-3 + value_523X))) + (((i_680X)<<2));
    WRITE_BARRIER(addr_681X, init_520X);
    *((long *) addr_681X) = init_520X;
    arg0K0 = (-1 + i_680X);
    goto L23396;}}
 L27437: {
  i_682X = arg0K0;
  if ((i_682X < 0)) {
    SvalS = vector_525X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    *((unsigned char *) ((((char *) (-3 + vector_525X))) + i_682X)) = init_269X;
    arg0K0 = (-1 + i_682X);
    goto L27437;}}
 L27198: {
  i_683X = arg0K0;
  if ((i_683X < 0)) {
    SvalS = string_528X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_528X))) + i_683X)) = (init_284X);
    arg0K0 = (-1 + i_683X);
    goto L27198;}}
 L31159: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L26550: {
  key_684X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_685X = *((long *) (SstackS));
  if ((0 == (3 & (SvalS)))) {
    mode_686X = (((SvalS))>>2);
    if ((1 == mode_686X)) {
      goto L26829;}
    else {
      if ((2 == mode_686X)) {
        goto L26829;}
      else {
        if ((3 == mode_686X)) {
          goto L26829;}
        else {
          if ((4 == mode_686X)) {
            goto L26829;}
          else {
            merged_arg0K0 = 5;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 161;
            goto push_exception_continuationB;
           push_exception_continuationB_return_161:
            *((long *) (SstackS)) = arg2_685X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = (((mode_686X)<<2));
            SstackS = ((SstackS) + -4);
            arg0K0 = 2;
            goto L17068;}}}}}
  else {
    merged_arg0K0 = 5;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 162;
    goto push_exception_continuationB;
   push_exception_continuationB_return_162:
    *((long *) (SstackS)) = arg2_685X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (SvalS);
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}}
 L33355: {
  key_687X = arg0K0;
  obj_688X = SvalS;
  if ((3 == (3 & obj_688X))) {
    if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_688X))) + -4))))>>2))))) {
      x_689X = SvalS;
      if ((0 == (*((long *) (((char *) (-3 + x_689X))))))) {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 163;
        goto push_exception_continuationB;
       push_exception_continuationB_return_163:
        *((long *) (SstackS)) = x_689X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}
      else {
        merged_arg0K0 = x_689X;
        close_channelB_return_tag = 0;
        goto close_channelB;
       close_channelB_return_0:
        status_690X = close_channelB0_return_value;
        if ((status_690X == NO_ERRORS)) {
          SvalS = 13;
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}
        else {
          merged_arg0K0 = 22;
          merged_arg0K1 = 1;
          push_exception_continuationB_return_tag = 164;
          goto push_exception_continuationB;
         push_exception_continuationB_return_164:
          *((long *) (SstackS)) = x_689X;
          SstackS = ((SstackS) + -4);
          merged_arg0K0 = status_690X;
          merged_arg0K1 = key_687X;
          get_error_string_return_tag = 0;
          goto get_error_string;
         get_error_string_return_0:
          x_691X = get_error_string0_return_value;
          *((long *) (SstackS)) = x_691X;
          SstackS = ((SstackS) + -4);
          arg0K0 = 2;
          goto L17068;}}}
    else {
      goto L33390;}}
  else {
    goto L33390;}}
 L37887: {
  key_692X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_693X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_694X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg4_695X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg5_696X = *((long *) (SstackS));
  if ((0 == (3 & (arg4_695X | arg3_694X)))) {
    if ((1 == arg2_693X)) {
      goto L37919;}
    else {
      if ((5 == arg2_693X)) {
        goto L37919;}
      else {
        goto L37939;}}}
  else {
    goto L37939;}}
 L38108: {
  key_697X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_698X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_699X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg4_700X = *((long *) (SstackS));
  if ((0 == (3 & (arg3_699X | arg2_698X)))) {
    obj_701X = SvalS;
    if ((3 == (3 & obj_701X))) {
      if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_701X))) + -4))))>>2))))) {
        start_702X = ((arg3_699X)>>2);
        count_703X = ((arg2_698X)>>2);
        channel_704X = SvalS;
        v_705X = 8 == (*((long *) (((char *) (-3 + channel_704X)))));
        if (v_705X) {
          if ((3 == (3 & arg4_700X))) {
            if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg4_700X))) + -4))))>>2))))) {
              goto L37665;}
            else {
              goto L37657;}}
          else {
            goto L37657;}}
        else {
          arg0K0 = 5;
          goto L37548;}}
      else {
        goto L38156;}}
    else {
      goto L38156;}}
  else {
    goto L38156;}}
 L8239: {
  val_706X = *((long *) ((((char *) (-3 + ch_542X))) + 12));
  addr_707X = (((char *) (-3 + prev_543X))) + 12;
  WRITE_BARRIER(addr_707X, val_706X);
  *((long *) addr_707X) = val_706X;
  addr_708X = (((char *) (-3 + ch_542X))) + 12;
  WRITE_BARRIER(addr_708X, 1);
  *((long *) addr_708X) = 1;
  arg0K0 = (*((long *) ((((char *) (-3 + ch_542X))) + 16)));
  goto L26502;}
 L32906: {
  key_709X = arg0K0;
  SstackS = ((SstackS) + 4);
  arg2_710X = *((long *) (SstackS));
  SstackS = ((SstackS) + 4);
  arg3_711X = *((long *) (SstackS));
  if ((3 == (3 & arg3_711X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg3_711X))) + -4))))>>2))))) {
      obj_712X = SvalS;
      if ((3 == (3 & obj_712X))) {
        if ((19 == (31 & ((((*((long *) ((((char *) (-3 + obj_712X))) + -4))))>>2))))) {
          x_713X = SvalS;
          x_714X = image_writing_okayP();
          if (x_714X) {
            port_715X = ps_open_output_file((((char *)(((char *) (-3 + arg3_711X))))), &status_716X);
            if ((status_716X == NO_ERRORS)) {
              status_717X = ps_write_string((((char *)(((char *) (-3 + x_713X))))), port_715X);
              if ((status_717X == NO_ERRORS)) {
                status_718X = write_image(arg2_710X, port_715X, mark_traced_channels_closedB);
                if ((status_718X == NO_ERRORS)) {
                  status_719X = ps_close(port_715X);
                  if ((status_719X == NO_ERRORS)) {
                    SvalS = 13;
                    Scode_pointerS = ((Scode_pointerS) + 1);
                    arg1K0 = (Scode_pointerS);
                    goto L19170;}
                  else {
                    arg0K0 = 22;
                    arg0K1 = status_719X;
                    goto L32946;}}
                else {
                  status_720X = ps_close(port_715X);
                  if ((status_720X == NO_ERRORS)) {
                    arg0K0 = 22;
                    arg0K1 = status_718X;
                    goto L32946;}
                  else {
                    ps_write_string("Unable to close image file", (stderr));
                    { long ignoreXX;
                    PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                    arg0K0 = 22;
                    arg0K1 = status_718X;
                    goto L32946;}}}
              else {
                status_721X = ps_close(port_715X);
                if ((status_721X == NO_ERRORS)) {
                  arg0K0 = 22;
                  arg0K1 = status_717X;
                  goto L32946;}
                else {
                  ps_write_string("Unable to close image file", (stderr));
                  { long ignoreXX;
                  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                  arg0K0 = 22;
                  arg0K1 = status_717X;
                  goto L32946;}}}
            else {
              arg0K0 = 10;
              arg0K1 = status_716X;
              goto L32946;}}
          else {
            merged_arg0K0 = 15;
            merged_arg0K1 = 1;
            push_exception_continuationB_return_tag = 165;
            goto push_exception_continuationB;
           push_exception_continuationB_return_165:
            *((long *) (SstackS)) = arg3_711X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = arg2_710X;
            SstackS = ((SstackS) + -4);
            *((long *) (SstackS)) = x_713X;
            SstackS = ((SstackS) + -4);
            arg0K0 = 3;
            goto L17068;}}
        else {
          goto L33028;}}
      else {
        goto L33028;}}
    else {
      goto L33028;}}
  else {
    goto L33028;}}
 L32760: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 166;
  goto push_exception_continuationB;
 push_exception_continuationB_return_166:
  *((long *) (SstackS)) = arg2_549X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_550X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L9391: {
  i_722X = arg0K0;
  map_723X = arg0K1;
  if ((4 == i_722X)) {
    addr_724X = ALLOCATE_SPACE(2, 20);
    *((long *) addr_724X) = 4106;
    vector_725X = 3 + (((long) (addr_724X + 4)));
    addr_726X = ((char *) (-3 + vector_725X));
    WRITE_BARRIER(addr_726X, map_723X);
    *((long *) addr_726X) = map_723X;
    addr_727X = ((char *) (-3 + vector_725X));
    arg0K0 = 1;
    goto L9411;}
  else {
    PS_SHIFT_LEFT(i_722X, (((i_722X)<<3)), x_728X)
    arg0K0 = (1 + i_722X);
    arg0K1 = (map_723X | x_728X);
    goto L9391;}}
 L31327: {
  table_729X = arg0K0;
  symbols_730X = arg0K1;
  firstP_731X = arg2K2;
  arg0K0 = 0;
  goto L31331;}
 L30906: {
  minutesP_732X = arg2K0;
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_733X = check_events0_return_value;
      if (temp_733X) {
        goto L30921;}
      else {
        goto L30945;}}
    else {
      goto L30921;}}
  else {
    goto L30945;}}
 L17820: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 167;
  goto push_exception_continuationB;
 push_exception_continuationB_return_167:
  *((long *) (SstackS)) = proc_570X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L25892: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 168;
  goto push_exception_continuationB;
 push_exception_continuationB_return_168:
  *((long *) (SstackS)) = arg2_577X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L25581: {
  obj_734X = SvalS;
  if ((3 == (3 & obj_734X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + obj_734X))) + -4))))>>2))))) {
      x_735X = SvalS;
      limit_736X = (((char *) (-3 + x_735X))) + (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + x_735X))) + -4))))>>8))));
      start_737X = (((char *) (-3 + x_735X))) + -4;
      if ((1 == arg2_587X)) {
        goto L15346;}
      else {
        arg1K0 = (start_737X + 4);
        goto L6529;}}
    else {
      goto L25620;}}
  else {
    goto L25620;}}
 L25620: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 169;
  goto push_exception_continuationB;
 push_exception_continuationB_return_169:
  *((long *) (SstackS)) = arg3_588X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_587X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L25397: {
  len_738X = 1 + n_593X;
  addr_739X = ALLOCATE_SPACE(19, (4 + len_738X));
  *((long *) addr_739X) = (78 + (((len_738X)<<8)));
  string_740X = 3 + (((long) (addr_739X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_740X))) + n_593X)) = 0;
  arg0K0 = arg2_592X;
  arg0K1 = (-1 + n_593X);
  goto L25327;}
 L25302: {
  if ((25 == arg2_592X)) {
    goto L25397;}
  else {
    merged_arg0K0 = 5;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 170;
    goto push_exception_continuationB;
   push_exception_continuationB_return_170:
    *((long *) (SstackS)) = arg2_592X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((n_593X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}}
 L12920: {
  i_741X = arg0K0;
  h_742X = arg0K1;
  if ((i_741X < n_598X)) {
    arg0K0 = (1 + i_741X);
    arg0K1 = (h_742X + (((*((unsigned char *) ((((char *) (-3 + arg2_594X))) + i_741X))))));
    goto L12920;}
  else {
    index_743X = h_742X & (-1 + v_597X);
    bucket_744X = *((long *) ((((char *) (-3 + table_596X))) + (((index_743X)<<2))));
    arg0K0 = bucket_744X;
    goto L12883;}}
 L25151: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 171;
  goto push_exception_continuationB;
 push_exception_continuationB_return_171:
  *((long *) (SstackS)) = arg2_594X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L31544: {
  if ((from_index_408X < 0)) {
    goto L31598;}
  else {
    if ((to_index_409X < 0)) {
      goto L31598;}
    else {
      if ((count_410X < 0)) {
        goto L31598;}
      else {
        if ((3 == (3 & arg5_407X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg5_407X))) + -4))))>>2))))) {
            arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_407X))) + -4))))>>8)));
            goto L31565;}
          else {
            goto L31561;}}
        else {
          goto L31561;}}}}}
 L31539: {
  if ((3 == (3 & arg5_407X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg5_407X))) + -4))))>>2))))) {
      goto L31544;}
    else {
      goto L31598;}}
  else {
    goto L31598;}}
 L31598: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 172;
  goto push_exception_continuationB;
 push_exception_continuationB_return_172:
  *((long *) (SstackS)) = arg5_407X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((from_index_408X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_405X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((to_index_409X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_410X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L17068;}
 L22494: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 173;
  goto push_exception_continuationB;
 push_exception_continuationB_return_173:
  *((long *) (SstackS)) = port_604X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L22474: {
  merged_arg0K0 = 14;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 174;
  goto push_exception_continuationB;
 push_exception_continuationB_return_174:
  *((long *) (SstackS)) = port_604X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L22291: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 175;
  goto push_exception_continuationB;
 push_exception_continuationB_return_175:
  *((long *) (SstackS)) = port_612X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L22271: {
  merged_arg0K0 = 14;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 176;
  goto push_exception_continuationB;
 push_exception_continuationB_return_176:
  *((long *) (SstackS)) = port_612X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L22039: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 177;
  goto push_exception_continuationB;
 push_exception_continuationB_return_177:
  *((long *) (SstackS)) = Kchar_618X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_619X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L22021: {
  merged_arg0K0 = 14;
  merged_arg0K1 = 2;
  push_exception_continuationB_return_tag = 178;
  goto push_exception_continuationB;
 push_exception_continuationB_return_178:
  *((long *) (SstackS)) = Kchar_618X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = port_619X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L30818: {
  arg0K0 = (*((long *) ((((char *) (-3 + stuff_626X))) + 4)));
  goto L30811;}
 L11984: {
  if ((3 == (3 & thing_627X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + thing_627X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + thing_627X)))));
      goto L12026;}
    else {
      goto L11992;}}
  else {
    goto L11992;}}
 L30826: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, out_418X, ignoreXX) }
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L17144: {
  StemplateS = (SvalS);
  Slosing_opcodeS = opcode_420X;
  arg0K0 = (2 + nargs_419X);
  goto L16469;}
 L17214: {
  merged_arg3K0 = "exception handler is not a closure";
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L17144;}
 L21734: {
  key_745X = arg0K0;
  p_746X = SenvS;
  if ((3 == (3 & p_746X))) {
    if ((p_746X < (((long) (Sstack_beginS))))) {
      goto L21852;}
    else {
      if (((((long) (Sstack_endS))) < p_746X)) {
        goto L21852;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_745X;
        merged_arg0K3 = 0;
        save_env_in_heap_return_tag = 0;
        goto save_env_in_heap;
       save_env_in_heap_return_0:
        v_747X = save_env_in_heap0_return_value;
        SenvS = v_747X;
        goto L21852;}}}
  else {
    goto L21852;}}
 L21745: {
  env_748X = arg0K0;
  a_749X = *((long *) ((((char *) (-3 + (StemplateS)))) + ((((((((*((unsigned char *) ((Scode_pointerS) + 1))))<<8)) + (*((unsigned char *) ((Scode_pointerS) + 2)))))<<2))));
  addr_750X = ALLOCATE_SPACE(3, 12);
  *((long *) addr_750X) = 2062;
  x_751X = 3 + (((long) (addr_750X + 4)));
  *((long *) (((char *) (-3 + x_751X)))) = a_749X;
  *((long *) ((((char *) (-3 + x_751X))) + 4)) = env_748X;
  SvalS = x_751X;
  Scode_pointerS = ((Scode_pointerS) + 4);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L21557: {
  i_752X = arg0K0;
  offset_753X = arg0K1;
  env_754X = arg0K2;
  if ((i_752X == total_count_91X)) {
    SvalS = new_env_440X;
    Scode_pointerS = ((Scode_pointerS) + (1 + offset_753X));
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    back_755X = *((unsigned char *) ((Scode_pointerS) + (1 + offset_753X)));
    arg0K0 = env_754X;
    arg0K1 = back_755X;
    goto L21652;}}
 L21454: {
  value_756X = arg0K0;
  SvalS = value_756X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L17402: {
  i_757X = arg0K0;
  m_758X = arg0K1;
  if ((0 == (n_641X & m_758X))) {
    arg0K0 = (1 + i_757X);
    arg0K1 = (((m_758X)<<1));
    goto L17402;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_758X));
    if ((i_757X == 0)) {
      *((long *) (SstackS)) = (Sinterrupted_templateS);
      SstackS = ((SstackS) + -4);
      Sinterrupted_templateS = 1;
      *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L17321;}
    else {
      if ((i_757X == 2)) {
        *((long *) (SstackS)) = (Sfinalize_theseS);
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
        SstackS = ((SstackS) + -4);
        Sfinalize_theseS = 25;
        arg0K0 = 2;
        goto L17321;}
      else {
        if ((i_757X == 3)) {
          channel_759X = Spending_channels_headS;
          next_760X = *((long *) ((((char *) (-3 + channel_759X))) + 12));
          Spending_channels_headS = next_760X;
          addr_761X = (((char *) (-3 + channel_759X))) + 12;
          WRITE_BARRIER(addr_761X, 1);
          *((long *) addr_761X) = 1;
          if ((1 == next_760X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_759X;
            goto L8380;}
          else {
            arg0K0 = channel_759X;
            goto L8380;}}
        else {
          *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
          SstackS = ((SstackS) + -4);
          arg0K0 = 1;
          goto L17321;}}}}}
 L16487: {
  skip_762X = arg0K0;
  stack_arg_count_763X = arg0K1;
  template_764X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_764X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_764X)))))))) + skip_762X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  arg0K0 = stack_space_643X;
  arg0K1 = stack_arg_count_763X;
  goto L16378;}
 L16510: {
  skip_765X = arg0K0;
  *((long *) (SstackS)) = 25;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((stack_arg_count_449X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((stack_arg_count_449X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_765X;
  arg0K1 = (3 + stack_arg_count_449X);
  goto L16487;}
 L16764: {
  skip_766X = arg0K0;
  stack_arg_count_767X = arg0K1;
  template_768X = *((long *) (((char *) (-3 + (SvalS)))));
  StemplateS = template_768X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + template_768X)))))))) + skip_766X);
  SenvS = (*((long *) ((((char *) (-3 + (SvalS)))) + 4)));
  arg0K0 = stack_space_650X;
  arg0K1 = stack_arg_count_767X;
  goto L16378;}
 L16789: {
  skip_769X = arg0K0;
  if ((total_arg_count_468X < 3)) {
    arg0K0 = total_arg_count_468X;
    goto L16797;}
  else {
    if ((2 < stack_arg_count_462X)) {
      arg0K0 = stack_arg_count_462X;
      goto L16797;}
    else {
      arg0K0 = 2;
      goto L16797;}}}
 L20011: {
  SvalS = (*((long *) ((((char *) (-3 + cont_656X))) + 16)));
  arg0K0 = stack_nargs_479X;
  arg0K1 = list_args_480X;
  arg0K2 = list_arg_count_481X;
  goto L18972;}
 L21121: {
  merged_arg0K0 = list_args_480X;
  merged_arg0K1 = stack_nargs_479X;
  pop_args_GlistS_return_tag = 4;
  goto pop_args_GlistS;
 pop_args_GlistS_return_4:
  args_770X = pop_args_GlistS0_return_value;
  merged_arg0K0 = 4;
  merged_arg0K1 = 0;
  push_exception_continuationB_return_tag = 179;
  goto push_exception_continuationB;
 push_exception_continuationB_return_179:
  *((long *) (SstackS)) = 1;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = args_770X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 2;
  goto L17068;}
 L30555: {
  if ((3 == (3 & x_149X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_149X))) + -4))))>>2))))) {
      arg0K0 = 5;
      goto L30584;}
    else {
      arg0K0 = 1;
      goto L30584;}}
  else {
    arg0K0 = 1;
    goto L30584;}}
 L28682: {
  if ((3 == (3 & x_151X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_151X))) + -4))))>>2))))) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 180;
      goto push_exception_continuationB;
     push_exception_continuationB_return_180:
      *((long *) (SstackS)) = x_151X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L17068;}
    else {
      goto L28690;}}
  else {
    goto L28690;}}
 L28471: {
  if ((3 == (3 & x_152X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_152X))) + -4))))>>2))))) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 181;
      goto push_exception_continuationB;
     push_exception_continuationB_return_181:
      *((long *) (SstackS)) = x_152X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L17068;}
    else {
      goto L28479;}}
  else {
    goto L28479;}}
 L28260: {
  if ((3 == (3 & x_153X))) {
    if ((10 == (31 & ((((*((long *) ((((char *) (-3 + x_153X))) + -4))))>>2))))) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 182;
      goto push_exception_continuationB;
     push_exception_continuationB_return_182:
      *((long *) (SstackS)) = x_153X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 1;
      goto L17068;}
    else {
      goto L28268;}}
  else {
    goto L28268;}}
 L7178: {
  if ((536870911 < lo_c_667X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 183;
    goto push_exception_continuationB;
   push_exception_continuationB_return_183:
    *((long *) (SstackS)) = arg2_159X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_160X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}
  else {
    if ((lo_c_667X < 0)) {
      merged_arg0K0 = 6;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 184;
      goto push_exception_continuationB;
     push_exception_continuationB_return_184:
      *((long *) (SstackS)) = arg2_159X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = y_160X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L17068;}
    else {
      if ((8192 < mid_c_670X)) {
        merged_arg0K0 = 6;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 185;
        goto push_exception_continuationB;
       push_exception_continuationB_return_185:
        *((long *) (SstackS)) = arg2_159X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = y_160X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}
      else {
        if ((a_161X < 0)) {
          if ((b_162X < 0)) {
            goto L7205;}
          else {
            goto L7212;}}
        else {
          if ((b_162X < 0)) {
            goto L7212;}
          else {
            goto L7205;}}}}}}
 L8916: {
  if ((536870911 < c_673X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 186;
    goto push_exception_continuationB;
   push_exception_continuationB_return_186:
    *((long *) (SstackS)) = arg2_166X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_167X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}
  else {
    arg0K0 = (((c_673X)<<2));
    goto L8890;}}
 L8915: {
  arg0K0 = ((((0 - c_673X))<<2));
  goto L8890;}
 L34170: {
  if ((536870911 < c_675X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 187;
    goto push_exception_continuationB;
   push_exception_continuationB_return_187:
    *((long *) (SstackS)) = arg2_180X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_181X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}
  else {
    SvalS = (((c_675X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L34169: {
  SvalS = ((((0 - c_675X))<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L34038: {
  n_771X = arg0K0;
  SvalS = (((n_771X)<<2));
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L24031: {
  i_772X = arg0K0;
  rest_list_773X = arg0K1;
  if ((25 == rest_list_773X)) {
    SvalS = new_516X;
    Scode_pointerS = ((Scode_pointerS) + 2);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    *((long *) ((((char *) (-3 + new_516X))) + (((i_772X)<<2)))) = (*((long *) (((char *) (-3 + rest_list_773X)))));
    arg0K0 = (1 + i_772X);
    arg0K1 = (*((long *) ((((char *) (-3 + rest_list_773X))) + 4)));
    goto L24031;}}
 L26829: {
  if ((0 == (3 & arg2_685X))) {
    if (((((arg2_685X)>>2)) < 0)) {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 188;
      goto push_exception_continuationB;
     push_exception_continuationB_return_188:
      *((long *) (SstackS)) = arg2_685X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_686X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L17068;}
    else {
      arg0K0 = (((arg2_685X)>>2));
      goto L26608;}}
  else {
    if ((3 == (3 & arg2_685X))) {
      if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_685X))) + -4))))>>2))))) {
        if ((1 == mode_686X)) {
          goto L26666;}
        else {
          if ((3 == mode_686X)) {
            goto L26666;}
          else {
            v_774X = ps_open_fd((((char *)(((char *) (-3 + arg2_685X))))), 0, &v_775X);
            arg0K0 = v_774X;
            arg0K1 = v_775X;
            goto L26679;}}}
      else {
        merged_arg0K0 = 5;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 189;
        goto push_exception_continuationB;
       push_exception_continuationB_return_189:
        *((long *) (SstackS)) = arg2_685X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_686X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
    else {
      merged_arg0K0 = 5;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 190;
      goto push_exception_continuationB;
     push_exception_continuationB_return_190:
      *((long *) (SstackS)) = arg2_685X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_686X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L17068;}}}
 L33390: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 191;
  goto push_exception_continuationB;
 push_exception_continuationB_return_191:
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 1;
  goto L17068;}
 L37919: {
  obj_776X = SvalS;
  if ((3 == (3 & obj_776X))) {
    if ((5 == (31 & ((((*((long *) ((((char *) (-3 + obj_776X))) + -4))))>>2))))) {
      x_777X = SvalS;
      if ((1 == arg2_693X)) {
        arg2K0 = 0;
        goto L37936;}
      else {
        arg2K0 = 1;
        goto L37936;}}
    else {
      goto L37939;}}
  else {
    goto L37939;}}
 L37939: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 192;
  goto push_exception_continuationB;
 push_exception_continuationB_return_192:
  *((long *) (SstackS)) = arg5_696X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg4_695X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_694X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_693X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L17068;}
 L37665: {
  if ((3 == (3 & arg4_700X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg4_700X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_700X))) + -4))))>>8)));
      goto L37677;}
    else {
      goto L37673;}}
  else {
    goto L37673;}}
 L37657: {
  if ((3 == (3 & arg4_700X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg4_700X))) + -4))))>>2))))) {
      goto L37665;}
    else {
      arg0K0 = 5;
      goto L37548;}}
  else {
    arg0K0 = 5;
    goto L37548;}}
 L37548: {
  reason_778X = arg0K0;
  merged_arg0K0 = reason_778X;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 193;
  goto push_exception_continuationB;
 push_exception_continuationB_return_193:
  *((long *) (SstackS)) = arg4_700X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_702X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_703X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = channel_704X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L17068;}
 L38156: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 194;
  goto push_exception_continuationB;
 push_exception_continuationB_return_194:
  *((long *) (SstackS)) = arg4_700X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg3_699X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_698X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L17068;}
 L32946: {
  reason_779X = arg0K0;
  status_780X = arg0K1;
  merged_arg0K0 = reason_779X;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 195;
  goto push_exception_continuationB;
 push_exception_continuationB_return_195:
  *((long *) (SstackS)) = arg3_711X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_710X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_713X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_780X;
  merged_arg0K1 = key_709X;
  get_error_string_return_tag = 1;
  goto get_error_string;
 get_error_string_return_1:
  x_781X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_781X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 4;
  goto L17068;}
 L33028: {
  merged_arg0K0 = 5;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 196;
  goto push_exception_continuationB;
 push_exception_continuationB_return_196:
  *((long *) (SstackS)) = arg3_711X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = arg2_710X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (SvalS);
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17068;}
 L9411: {
  i_782X = arg0K0;
  if ((4 == i_782X)) {
    value_783X = (((*((unsigned char *) addr_727X)))<<2);
    addr_784X = ((char *) (-3 + vector_725X));
    WRITE_BARRIER(addr_784X, value_783X);
    *((long *) addr_784X) = value_783X;
    if ((3 == (3 & vector_725X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + vector_725X))) + -4)))))) {
        *((long *) ((((char *) (-3 + vector_725X))) + -4)) = (128 | (*((long *) ((((char *) (-3 + vector_725X))) + -4))));
        arg0K0 = vector_725X;
        goto L35950;}
      else {
        arg0K0 = vector_725X;
        goto L35950;}}
    else {
      arg0K0 = vector_725X;
      goto L35950;}}
  else {
    value_785X = (((*((unsigned char *) (addr_727X + i_782X))))<<2);
    addr_786X = (((char *) (-3 + vector_725X))) + (((i_782X)<<2));
    WRITE_BARRIER(addr_786X, value_785X);
    *((long *) addr_786X) = value_785X;
    arg0K0 = (1 + i_782X);
    goto L9411;}}
 L31331: {
  i_787X = arg0K0;
  if ((i_787X == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + symbols_730X))) + -4))))>>8))))>>2)))) {
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    v_788X = AVAILABLEp(3);
    if (v_788X) {
      obj_789X = *((long *) ((((char *) (-3 + symbols_730X))) + (((i_787X)<<2))));
      v_790X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_729X))) + -4))))>>8))))>>2);
      s_791X = *((long *) ((((char *) (-3 + obj_789X))) + 4));
      n_792X = -1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s_791X))) + -4))))>>8));
      arg0K0 = 0;
      arg0K1 = 0;
      goto L14764;}
    else {
      if (firstP_731X) {
        arg0K0 = 1;
        goto L31352;}
      else {
        merged_arg0K0 = 8;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 197;
        goto push_exception_continuationB;
       push_exception_continuationB_return_197:
        *((long *) (SstackS)) = table_729X;
        SstackS = ((SstackS) + -4);
        arg0K0 = 1;
        goto L17068;}}}}
 L30921: {
  SvalS = 13;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L30945: {
  if ((0 == (Spending_interruptsS))) {
    wait_for_event((((arg2_352X)>>2)), minutesP_732X);
    goto L30921;}
  else {
    goto L30921;}}
 L15346: {
  SstartS = start_737X;
  Snew_uidsS = 25;
  Sbad_count_proxiesS = 25;
  arg1K0 = (start_737X + 8);
  goto L15074;}
 L6529: {
  ptr_793X = arg1K0;
  if ((ptr_793X < limit_736X)) {
    x_794X = *((unsigned char *) ptr_793X);
    *((unsigned char *) ptr_793X) = (*((unsigned char *) (ptr_793X + 3)));
    *((unsigned char *) (ptr_793X + 3)) = x_794X;
    x_795X = *((unsigned char *) (ptr_793X + 1));
    *((unsigned char *) (ptr_793X + 1)) = (*((unsigned char *) (ptr_793X + 2)));
    *((unsigned char *) (ptr_793X + 2)) = x_795X;
    value_796X = *((long *) ptr_793X);
    next_797X = ptr_793X + 4;
    if ((2 == (3 & value_796X))) {
      if (((31 & (((value_796X)>>2))) < 19)) {
        arg1K0 = next_797X;
        goto L6529;}
      else {
        arg1K0 = (next_797X + (-4 & (3 + ((long)(((unsigned long)value_796X)>>8)))));
        goto L6529;}}
    else {
      arg1K0 = next_797X;
      goto L6529;}}
  else {
    goto L15346;}}
 L25327: {
  l_798X = arg0K0;
  i_799X = arg0K1;
  if ((i_799X < 0)) {
    SvalS = string_740X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_740X))) + i_799X)) = ((((((*((long *) (((char *) (-3 + l_798X))))))>>8))));
    arg0K0 = (*((long *) ((((char *) (-3 + l_798X))) + 4)));
    arg0K1 = (-1 + i_799X);
    goto L25327;}}
 L12883: {
  b_800X = arg0K0;
  if ((25 == b_800X)) {
    addr_801X = ALLOCATE_SPACE(1, 12);
    *((long *) addr_801X) = 2054;
    x_802X = 3 + (((long) (addr_801X + 4)));
    *((long *) (((char *) (-3 + x_802X)))) = 1;
    *((long *) ((((char *) (-3 + x_802X))) + 4)) = arg2_594X;
    addr_803X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_803X) = 2050;
    x_804X = 3 + (((long) (addr_803X + 4)));
    *((long *) (((char *) (-3 + x_804X)))) = x_802X;
    *((long *) ((((char *) (-3 + x_804X))) + 4)) = bucket_744X;
    addr_805X = (((char *) (-3 + table_596X))) + (((index_743X)<<2));
    WRITE_BARRIER(addr_805X, x_804X);
    *((long *) addr_805X) = x_804X;
    arg0K0 = x_802X;
    goto L25150;}
  else {
    s2_806X = *((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + b_800X)))))))) + 4));
    len_807X = (long)(((unsigned long)(*((long *) ((((char *) (-3 + arg2_594X))) + -4))))>>8);
    if ((len_807X == ((long)(((unsigned long)(*((long *) ((((char *) (-3 + s2_806X))) + -4))))>>8)))) {
      if (((!memcmp((void *)(((char *) (-3 + s2_806X))), (void *)(((char *) (-3 + arg2_594X))),len_807X)))) {
        arg0K0 = (*((long *) (((char *) (-3 + b_800X)))));
        goto L25150;}
      else {
        goto L12901;}}
    else {
      goto L12901;}}}
 L31565: {
  y_808X = arg0K0;
  if ((y_808X < (from_index_408X + count_410X))) {
    goto L31598;}
  else {
    if ((3 == (3 & arg3_405X))) {
      if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg3_405X))) + -4))))>>2))))) {
        arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_405X))) + -4))))>>8)));
        goto L31580;}
      else {
        goto L31576;}}
    else {
      goto L31576;}}}
 L31561: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_407X))) + -4))))>>8));
  goto L31565;}
 L12026: {
  v_809X = arg3K0;
  ps_write_string(v_809X, out_418X);
  goto L30818;}
 L11992: {
  if ((3 == (3 & thing_627X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + thing_627X))) + -4))))>>2))))) {
      arg3K0 = (((char *)(((char *) (-3 + (*((long *) ((((char *) (-3 + thing_627X))) + 4))))))));
      goto L12026;}
    else {
      goto L12000;}}
  else {
    goto L12000;}}
 L21852: {
  arg0K0 = (SenvS);
  goto L21739;}
 L21652: {
  env_810X = arg0K0;
  i_811X = arg0K1;
  if ((0 == i_811X)) {
    count_812X = *((unsigned char *) ((Scode_pointerS) + (2 + offset_753X)));
    arg0K0 = count_812X;
    arg0K1 = i_752X;
    arg0K2 = (2 + offset_753X);
    goto L21574;}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + env_810X)))));
    arg0K1 = (-1 + i_811X);
    goto L21652;}}
 L17321: {
  arg_count_813X = arg0K0;
  obj_814X = Sinterrupt_handlersS;
  if ((3 == (3 & obj_814X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + obj_814X))) + -4))))>>2))))) {
      goto L17335;}
    else {
      goto L17415;}}
  else {
    goto L17415;}}
 L8380: {
  channel_815X = arg0K0;
  x_816X = 1 == (Spending_channels_headS);
  if (x_816X) {
    goto L8394;}
  else {
    Spending_interruptsS = (8 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        goto L8394;}
      else {
        goto L8394;}}
    else {
      Spending_interruptPS = 1;
      goto L8394;}}}
 L16378: {
  stack_slots_817X = arg0K0;
  stack_arg_count_818X = arg0K1;
  if ((stack_slots_817X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L16380;}
  else {
    space_819X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
    v_820X = AVAILABLEp(space_819X);
    if (v_820X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L16437;}
    else {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 28;
      goto collect_saving_temp;
     collect_saving_temp_return_28:
      v_821X = AVAILABLEp(space_819X);
      if (v_821X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L16437;}
      else {
        arg2K0 = 0;
        arg0K1 = 0;
        goto L16437;}}}}
 L16797: {
  final_stack_arg_count_822X = arg0K0;
  if ((stack_arg_count_462X < final_stack_arg_count_822X)) {
    arg0K0 = final_stack_arg_count_822X;
    goto L16801;}
  else {
    arg0K0 = stack_arg_count_462X;
    goto L16801;}}
 L28690: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L28479: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L28268: {
  SvalS = 1;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L7205: {
  if ((536870911 < c_671X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 198;
    goto push_exception_continuationB;
   push_exception_continuationB_return_198:
    *((long *) (SstackS)) = arg2_159X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_160X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}
  else {
    SvalS = (((c_671X)<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L7212: {
  if ((536870912 < c_671X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 199;
    goto push_exception_continuationB;
   push_exception_continuationB_return_199:
    *((long *) (SstackS)) = arg2_159X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_160X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}
  else {
    SvalS = ((((0 - c_671X))<<2));
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L8890: {
  q_823X = arg0K0;
  if ((0 == y_167X)) {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 200;
    goto push_exception_continuationB;
   push_exception_continuationB_return_200:
    *((long *) (SstackS)) = arg2_166X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_167X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}
  else {
    a_824X = ((arg2_166X)>>2);
    if ((a_824X < 0)) {
      arg0K0 = (0 - a_824X);
      goto L8947;}
    else {
      arg0K0 = a_824X;
      goto L8947;}}}
 L26608: {
  index_825X = arg0K0;
  temp_826X = index_825X < (Snumber_of_channelsS);
  if (temp_826X) {
    goto L10939;}
  else {
    x_827X = 1 + index_825X;
    y_828X = 8 + (Snumber_of_channelsS);
    if ((x_827X < y_828X)) {
      arg0K0 = y_828X;
      goto L2160;}
    else {
      arg0K0 = x_827X;
      goto L2160;}}}
 L26666: {
  v_829X = ps_open_fd((((char *)(((char *) (-3 + arg2_685X))))), 1, &v_830X);
  arg0K0 = v_829X;
  arg0K1 = v_830X;
  goto L26679;}
 L26679: {
  channel_831X = arg0K0;
  status_832X = arg0K1;
  if ((status_832X == NO_ERRORS)) {
    arg0K0 = channel_831X;
    goto L26608;}
  else {
    if ((status_832X == ENOENT)) {
      merged_arg0K0 = 10;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 201;
      goto push_exception_continuationB;
     push_exception_continuationB_return_201:
      *((long *) (SstackS)) = arg2_685X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_686X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L17068;}
    else {
      merged_arg0K0 = 22;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 202;
      goto push_exception_continuationB;
     push_exception_continuationB_return_202:
      *((long *) (SstackS)) = arg2_685X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_686X)<<2));
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_832X;
      merged_arg0K1 = key_684X;
      get_error_string_return_tag = 2;
      goto get_error_string;
     get_error_string_return_2:
      x_833X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_833X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 3;
      goto L17068;}}}
 L37936: {
  waitP_834X = arg2K0;
  start_835X = ((arg4_695X)>>2);
  count_836X = ((arg3_694X)>>2);
  if ((4 == (*((long *) (((char *) (-3 + x_777X))))))) {
    if ((3 == (3 & arg5_696X))) {
      if ((0 == (128 & (*((long *) ((((char *) (-3 + arg5_696X))) + -4)))))) {
        if ((3 == (3 & arg5_696X))) {
          if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg5_696X))) + -4))))>>2))))) {
            goto L37238;}
          else {
            goto L37230;}}
        else {
          goto L37230;}}
      else {
        arg0K0 = 5;
        goto L37121;}}
    else {
      arg0K0 = 5;
      goto L37121;}}
  else {
    arg0K0 = 5;
    goto L37121;}}
 L37677: {
  length_837X = arg0K0;
  if ((length_837X < (start_702X + count_703X))) {
    arg0K0 = 7;
    goto L37548;}
  else {
    got_838X = ps_write_fd(((((*((long *) ((((char *) (-3 + channel_704X))) + 8))))>>2)), ((((char *) (-3 + arg4_700X))) + start_702X), count_703X, &pendingP_839X, &status_840X);
    if ((status_840X == NO_ERRORS)) {
      if (pendingP_839X) {
        Spending_interruptPS = 0;
        Senabled_interruptsS = 0;
        arg0K0 = 13;
        goto L37548;}
      else {
        SvalS = (((got_838X)<<2));
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}}
    else {
      merged_arg0K0 = 22;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 203;
      goto push_exception_continuationB;
     push_exception_continuationB_return_203:
      *((long *) (SstackS)) = arg4_700X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_702X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_703X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = channel_704X;
      SstackS = ((SstackS) + -4);
      merged_arg0K0 = status_840X;
      merged_arg0K1 = key_697X;
      get_error_string_return_tag = 3;
      goto get_error_string;
     get_error_string_return_3:
      x_841X = get_error_string0_return_value;
      *((long *) (SstackS)) = x_841X;
      SstackS = ((SstackS) + -4);
      arg0K0 = 5;
      goto L17068;}}}
 L37673: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg4_700X))) + -4))))>>8));
  goto L37677;}
 L35950: {
  val_842X = arg0K0;
  SvalS = val_842X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L14764: {
  i_843X = arg0K0;
  h_844X = arg0K1;
  if ((i_843X < n_792X)) {
    arg0K0 = (1 + i_843X);
    arg0K1 = (h_844X + (((*((unsigned char *) ((((char *) (-3 + s_791X))) + i_843X))))));
    goto L14764;}
  else {
    index_845X = h_844X & (-1 + v_790X);
    bucket_846X = *((long *) ((((char *) (-3 + table_729X))) + (((index_845X)<<2))));
    arg0K0 = bucket_846X;
    goto L14725;}}
 L31352: {
  i_847X = arg0K0;
  if ((i_847X == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_729X))) + -4))))>>8))))>>2)))) {
    addr_848X = ((char *) (-3 + table_729X));
    WRITE_BARRIER(addr_848X, symbols_730X);
    *((long *) addr_848X) = symbols_730X;
    merged_arg0K0 = table_729X;
    collect_saving_temp_return_tag = 29;
    goto collect_saving_temp;
   collect_saving_temp_return_29:
    table_849X = collect_saving_temp0_return_value;
    symbols_850X = *((long *) (((char *) (-3 + table_849X))));
    addr_851X = ((char *) (-3 + table_849X));
    WRITE_BARRIER(addr_851X, 25);
    *((long *) addr_851X) = 25;
    arg0K0 = table_849X;
    arg0K1 = symbols_850X;
    arg2K2 = 0;
    goto L31327;}
  else {
    addr_852X = (((char *) (-3 + table_729X))) + (((i_847X)<<2));
    WRITE_BARRIER(addr_852X, 25);
    *((long *) addr_852X) = 25;
    arg0K0 = (1 + i_847X);
    goto L31352;}}
 L15074: {
  addr_853X = arg1K0;
  if ((addr_853X < limit_736X)) {
    thing_854X = *((long *) addr_853X);
    next_855X = addr_853X + 4;
    if (((31 & (((thing_854X)>>2))) < 19)) {
      arg1K0 = next_855X;
      goto L15126;}
    else {
      goto L15094;}}
  else {
    *((long *) ((((char *) (-3 + x_735X))) + -4)) = 1106;
    thing_856X = 3 + (((long) ((SstartS) + ((((*((long *) (start_737X + 4))))>>8)))));
    missing_uids_857X = Snew_uidsS;
    low_count_proxies_858X = Sbad_count_proxiesS;
    addr_859X = ALLOCATE_SPACE(2, 20);
    *((long *) addr_859X) = 4106;
    result_860X = 3 + (((long) (addr_859X + 4)));
    addr_861X = ((char *) (-3 + result_860X));
    WRITE_BARRIER(addr_861X, thing_856X);
    *((long *) addr_861X) = thing_856X;
    addr_862X = (((char *) (-3 + result_860X))) + 4;
    WRITE_BARRIER(addr_862X, missing_uids_857X);
    *((long *) addr_862X) = missing_uids_857X;
    addr_863X = (((char *) (-3 + result_860X))) + 8;
    WRITE_BARRIER(addr_863X, low_count_proxies_858X);
    *((long *) addr_863X) = low_count_proxies_858X;
    SvalS = result_860X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L25150: {
  val_864X = arg0K0;
  SvalS = val_864X;
  Scode_pointerS = ((Scode_pointerS) + 1);
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L12901: {
  arg0K0 = (*((long *) ((((char *) (-3 + b_800X))) + 4)));
  goto L12883;}
 L31580: {
  y_865X = arg0K0;
  if ((y_865X < (to_index_409X + count_410X))) {
    goto L31598;}
  else {
    memcpy((void *)((((char *) (-3 + arg3_405X))) + to_index_409X), (void *)((((char *) (-3 + arg5_407X))) + from_index_408X),count_410X);
    SvalS = 13;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L31576: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg3_405X))) + -4))))>>8));
  goto L31580;}
 L12000: {
  if ((1 == thing_627X)) {
    goto L12003;}
  else {
    if ((5 == thing_627X)) {
      goto L12003;}
    else {
      if ((25 == thing_627X)) {
        arg3K0 = "()";
        goto L12026;}
      else {
        if ((3 == (3 & thing_627X))) {
          if ((0 == (31 & ((((*((long *) ((((char *) (-3 + thing_627X))) + -4))))>>2))))) {
            arg3K0 = "(...)";
            goto L12026;}
          else {
            goto L12016;}}
        else {
          goto L12016;}}}}}
 L21574: {
  count_866X = arg0K0;
  i_867X = arg0K1;
  offset_868X = arg0K2;
  if ((0 == count_866X)) {
    arg0K0 = i_867X;
    arg0K1 = offset_868X;
    arg0K2 = env_810X;
    goto L21557;}
  else {
    value_869X = *((long *) ((((char *) (-3 + env_810X))) + ((((*((unsigned char *) ((Scode_pointerS) + (1 + offset_868X)))))<<2))));
    addr_870X = (((char *) (-3 + new_env_440X))) + (((i_867X)<<2));
    WRITE_BARRIER(addr_870X, value_869X);
    *((long *) addr_870X) = value_869X;
    arg0K0 = (-1 + count_866X);
    arg0K1 = (1 + i_867X);
    arg0K2 = (1 + offset_868X);
    goto L21574;}}
 L17335: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    Spending_interruptPS = 0;
    if ((Spending_eventsPS)) {
      Spending_interruptPS = 1;
      goto L17337;}
    else {
      goto L17337;}}
  else {
    Spending_interruptPS = 1;
    goto L17337;}}
 L17415: {
  ps_error("interrupt handler is not a vector", 0);
  goto L17335;}
 L8394: {
  *((long *) (SstackS)) = channel_815X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (*((long *) ((((char *) (-3 + channel_815X))) + 16)));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = ((((Senabled_interruptsS))<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = 3;
  goto L17321;}
 L16380: {
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 3;
      goto check_events;
     check_events_return_3:
      v_871X = check_events0_return_value;
      if (v_871X) {
        arg0K0 = stack_arg_count_818X;
        goto L17297;}
      else {
        goto L16384;}}
    else {
      arg0K0 = stack_arg_count_818X;
      goto L17297;}}
  else {
    goto L16384;}}
 L16437: {
  okayP_872X = arg2K0;
  key_873X = arg0K1;
  if (okayP_872X) {
    arg0K0 = key_873X;
    goto L16391;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_873X;
    goto L16391;}}
 L16801: {
  v_874X = arg0K0;
  merged_arg0K0 = v_874X;
  merged_arg0K1 = stack_arg_count_462X;
  merged_arg0K2 = list_args_463X;
  merged_arg0K3 = list_arg_count_464X;
  rest_list_setup_return_tag = 2;
  goto rest_list_setup;
 rest_list_setup_return_2:
  *((long *) (SstackS)) = (((final_stack_arg_count_822X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((total_arg_count_468X)<<2));
  SstackS = ((SstackS) + -4);
  arg0K0 = skip_769X;
  arg0K1 = (3 + final_stack_arg_count_822X);
  goto L16764;}
 L8947: {
  a_875X = arg0K0;
  n_876X = ((y_167X)>>2);
  if ((n_876X < 0)) {
    arg0K0 = (0 - n_876X);
    goto L8949;}
  else {
    arg0K0 = n_876X;
    goto L8949;}}
 L10939: {
  if ((1 == (*((Svm_channelsS) + index_825X)))) {
    addr_877X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_877X) = 5142;
    x_878X = 3 + (((long) (addr_877X + 4)));
    *((long *) (((char *) (-3 + x_878X)))) = (((mode_686X)<<2));
    *((long *) ((((char *) (-3 + x_878X))) + 4)) = arg2_685X;
    *((long *) ((((char *) (-3 + x_878X))) + 8)) = (((index_825X)<<2));
    *((long *) ((((char *) (-3 + x_878X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_878X))) + 16)) = 1;
    *((Svm_channelsS) + index_825X) = x_878X;
    arg0K0 = x_878X;
    arg0K1 = 9;
    goto L26612;}
  else {
    arg0K0 = 1;
    arg0K1 = 11;
    goto L26612;}}
 L2160: {
  new_count_879X = arg0K0;
  new_vm_channels_880X = (long*)malloc(sizeof(long) * new_count_879X);
  if ((NULL == new_vm_channels_880X)) {
    arg0K0 = 1;
    arg0K1 = 9;
    goto L26612;}
  else {
    arg0K0 = 0;
    goto L2172;}}
 L37238: {
  if ((3 == (3 & arg5_696X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg5_696X))) + -4))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_696X))) + -4))))>>8)));
      goto L37250;}
    else {
      goto L37246;}}
  else {
    goto L37246;}}
 L37230: {
  if ((3 == (3 & arg5_696X))) {
    if ((20 == (31 & ((((*((long *) ((((char *) (-3 + arg5_696X))) + -4))))>>2))))) {
      goto L37238;}
    else {
      arg0K0 = 5;
      goto L37121;}}
  else {
    arg0K0 = 5;
    goto L37121;}}
 L37121: {
  reason_881X = arg0K0;
  merged_arg0K0 = reason_881X;
  merged_arg0K1 = 1;
  push_exception_continuationB_return_tag = 204;
  goto push_exception_continuationB;
 push_exception_continuationB_return_204:
  *((long *) (SstackS)) = arg5_696X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((start_835X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((count_836X)<<2));
  SstackS = ((SstackS) + -4);
  if (waitP_834X) {
    arg0K0 = 5;
    goto L37136;}
  else {
    arg0K0 = 1;
    goto L37136;}}
 L14725: {
  b_882X = arg0K0;
  if ((25 == b_882X)) {
    addr_883X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_883X) = 2050;
    x_884X = 3 + (((long) (addr_883X + 4)));
    *((long *) (((char *) (-3 + x_884X)))) = obj_789X;
    *((long *) ((((char *) (-3 + x_884X))) + 4)) = bucket_846X;
    addr_885X = (((char *) (-3 + table_729X))) + (((index_845X)<<2));
    WRITE_BARRIER(addr_885X, x_884X);
    *((long *) addr_885X) = x_884X;
    goto L31346;}
  else {
    if ((obj_789X == (*((long *) (((char *) (-3 + b_882X))))))) {
      goto L31346;}
    else {
      arg0K0 = (*((long *) ((((char *) (-3 + b_882X))) + 4)));
      goto L14725;}}}
 L15126: {
  addr_886X = arg1K0;
  if ((addr_886X == (next_855X + (-4 & (3 + ((long)(((unsigned long)thing_854X)>>8))))))) {
    goto L15094;}
  else {
    thing_887X = *((long *) addr_886X);
    if ((3 == (3 & thing_887X))) {
      data_888X = ((thing_887X)>>8);
      temp_889X = 63 & (((thing_887X)>>2));
      if ((0 == temp_889X)) {
        arg0K0 = (3 + (((long) ((SstartS) + data_888X))));
        goto L15138;}
      else {
        if ((1 == temp_889X)) {
          decode_vector_890X = *((long *) ((((char *) (-3 + arg3_588X))) + 4));
          if ((data_888X < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + decode_vector_890X))) + -4))))>>8))))>>2)))) {
            arg0K0 = (*((long *) ((((char *) (-3 + decode_vector_890X))) + (((data_888X)<<2)))));
            goto L14309;}
          else {
            arg0K0 = 1;
            goto L14309;}}
        else {
          if ((2 == temp_889X)) {
            data_addr_891X = (SstartS) + (-4 + data_888X);
            p_892X = *((long *) data_addr_891X);
            p_893X = *((long *) (data_addr_891X + 4));
            *((long *) data_addr_891X) = 1106;
            aspace_uid_894X = ((p_892X)>>2);
            uid_895X = ((p_893X)>>2);
            decode_vector_896X = *((long *) ((((char *) (-3 + arg3_588X))) + 4));
            if ((aspace_uid_894X < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + decode_vector_896X))) + -4))))>>8))))>>2)))) {
              arg0K0 = (*((long *) ((((char *) (-3 + decode_vector_896X))) + (((aspace_uid_894X)<<2)))));
              goto L14265;}
            else {
              arg0K0 = 1;
              goto L14265;}}
          else {
            data_addr_897X = (SstartS) + (-4 + data_888X);
            p_898X = *((long *) (data_addr_897X + 8));
            p_899X = *((long *) (data_addr_897X + 4));
            p_900X = *((long *) data_addr_897X);
            *((long *) data_addr_897X) = 2130;
            aspace_uid_901X = ((p_900X)>>2);
            uid_902X = ((p_899X)>>2);
            count_903X = ((p_898X)>>2);
            decode_vector_904X = *((long *) ((((char *) (-3 + arg3_588X))) + 4));
            if ((aspace_uid_901X < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + decode_vector_904X))) + -4))))>>8))))>>2)))) {
              arg0K0 = (*((long *) ((((char *) (-3 + decode_vector_904X))) + (((aspace_uid_901X)<<2)))));
              goto L14284;}
            else {
              arg0K0 = 1;
              goto L14284;}}}}}
    else {
      goto L15132;}}}
 L15094: {
  arg1K0 = (next_855X + (-4 & (3 + ((long)(((unsigned long)thing_854X)>>8)))));
  goto L15074;}
 L12003: {
  if ((1 == thing_627X)) {
    arg3K0 = "#f";
    goto L12026;}
  else {
    arg3K0 = "#t";
    goto L12026;}}
 L12016: {
  if ((3 == (3 & thing_627X))) {
    if ((2 == (31 & ((((*((long *) ((((char *) (-3 + thing_627X))) + -4))))>>2))))) {
      arg3K0 = "#(...)";
      goto L12026;}
    else {
      goto L12020;}}
  else {
    goto L12020;}}
 L17337: {
  SvalS = (*((long *) ((((char *) (-3 + (Sinterrupt_handlersS)))) + (((i_757X)<<2)))));
  obj_905X = SvalS;
  if ((3 == (3 & obj_905X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + obj_905X))) + -4))))>>2))))) {
      goto L17354;}
    else {
      goto L17440;}}
  else {
    goto L17440;}}
 L16384: {
  arg1K0 = (Scode_pointerS);
  goto L19170;}
 L16391: {
  v_906X = arg0K0;
  merged_arg0K0 = v_906X;
  copy_stack_into_heap_return_tag = 3;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_3:
  if ((stack_slots_817X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L16380;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L16380;}}
 L8949: {
  b_907X = arg0K0;
  c_908X = a_875X % b_907X;
  if ((a_824X < 0)) {
    arg0K0 = (0 - c_908X);
    goto L8953;}
  else {
    arg0K0 = c_908X;
    goto L8953;}}
 L26612: {
  channel_909X = arg0K0;
  reason_910X = arg0K1;
  if ((1 == channel_909X)) {
    if ((3 == (3 & arg2_685X))) {
      if ((19 == (31 & ((((*((long *) ((((char *) (-3 + arg2_685X))) + -4))))>>2))))) {
        if ((1 == mode_686X)) {
          goto L26764;}
        else {
          if ((3 == mode_686X)) {
            goto L26764;}
          else {
            v_911X = ps_close_fd(index_825X);
            arg0K0 = v_911X;
            goto L26759;}}}
      else {
        merged_arg0K0 = reason_910X;
        merged_arg0K1 = 1;
        push_exception_continuationB_return_tag = 205;
        goto push_exception_continuationB;
       push_exception_continuationB_return_205:
        *((long *) (SstackS)) = arg2_685X;
        SstackS = ((SstackS) + -4);
        *((long *) (SstackS)) = (((mode_686X)<<2));
        SstackS = ((SstackS) + -4);
        arg0K0 = 2;
        goto L17068;}}
    else {
      merged_arg0K0 = reason_910X;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 206;
      goto push_exception_continuationB;
     push_exception_continuationB_return_206:
      *((long *) (SstackS)) = arg2_685X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((mode_686X)<<2));
      SstackS = ((SstackS) + -4);
      arg0K0 = 2;
      goto L17068;}}
  else {
    SvalS = channel_909X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}}
 L2172: {
  i_912X = arg0K0;
  if ((i_912X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L2187;}
  else {
    *(new_vm_channels_880X + i_912X) = (*((Svm_channelsS) + i_912X));
    arg0K0 = (1 + i_912X);
    goto L2172;}}
 L37250: {
  length_913X = arg0K0;
  if ((length_913X < (start_835X + count_836X))) {
    arg0K0 = 7;
    goto L37121;}
  else {
    got_914X = ps_read_fd(((((*((long *) ((((char *) (-3 + x_777X))) + 8))))>>2)), ((((char *) (-3 + arg5_696X))) + start_835X), count_836X, waitP_834X, &eofP_915X, &pendingP_916X, &status_917X);
    if ((status_917X == NO_ERRORS)) {
      if (eofP_915X) {
        SvalS = 21;
        Scode_pointerS = ((Scode_pointerS) + 1);
        arg1K0 = (Scode_pointerS);
        goto L19170;}
      else {
        if (pendingP_916X) {
          Spending_interruptPS = 0;
          Senabled_interruptsS = 0;
          arg0K0 = 13;
          goto L37121;}
        else {
          SvalS = (((got_914X)<<2));
          Scode_pointerS = ((Scode_pointerS) + 1);
          arg1K0 = (Scode_pointerS);
          goto L19170;}}}
    else {
      merged_arg0K0 = 22;
      merged_arg0K1 = 1;
      push_exception_continuationB_return_tag = 207;
      goto push_exception_continuationB;
     push_exception_continuationB_return_207:
      *((long *) (SstackS)) = arg5_696X;
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((start_835X)<<2));
      SstackS = ((SstackS) + -4);
      *((long *) (SstackS)) = (((count_836X)<<2));
      SstackS = ((SstackS) + -4);
      if (waitP_834X) {
        arg0K0 = 5;
        goto L37173;}
      else {
        arg0K0 = 1;
        goto L37173;}}}}
 L37246: {
  arg0K0 = ((long)(((unsigned long)(*((long *) ((((char *) (-3 + arg5_696X))) + -4))))>>8));
  goto L37250;}
 L37136: {
  x_918X = arg0K0;
  *((long *) (SstackS)) = x_918X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_777X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 5;
  goto L17068;}
 L31346: {
  arg0K0 = (1 + i_787X);
  goto L31331;}
 L15138: {
  x2_919X = arg0K0;
  *((long *) addr_886X) = x2_919X;
  goto L15132;}
 L14309: {
  thing_920X = arg0K0;
  if ((3 == (3 & thing_920X))) {
    arg0K0 = thing_920X;
    goto L15138;}
  else {
    arg0K0 = 1;
    arg0K1 = data_888X;
    arg0K2 = 1;
    arg1K3 = addr_886X;
    arg1K4 = next_855X;
    goto L10704;}}
 L14265: {
  aspace_921X = arg0K0;
  if ((3 == (3 & aspace_921X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + aspace_921X))) + -4))))>>2))))) {
      decode_vector_922X = *((long *) ((((char *) (-3 + aspace_921X))) + 4));
      if ((uid_895X < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + decode_vector_922X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + decode_vector_922X))) + (((uid_895X)<<2)))));
        goto L14351;}
      else {
        arg0K0 = 1;
        goto L14351;}}
    else {
      arg0K0 = aspace_uid_894X;
      arg0K1 = uid_895X;
      arg0K2 = 1;
      arg1K3 = addr_886X;
      arg1K4 = next_855X;
      goto L10704;}}
  else {
    arg0K0 = aspace_uid_894X;
    arg0K1 = uid_895X;
    arg0K2 = 1;
    arg1K3 = addr_886X;
    arg1K4 = next_855X;
    goto L10704;}}
 L14284: {
  aspace_923X = arg0K0;
  if ((3 == (3 & aspace_923X))) {
    if ((16 == (31 & ((((*((long *) ((((char *) (-3 + aspace_923X))) + -4))))>>2))))) {
      proxy_vector_924X = *((long *) ((((char *) (-3 + aspace_923X))) + 8));
      if ((uid_902X < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + proxy_vector_924X))) + -4))))>>8))))>>2)))) {
        thing_925X = *((long *) ((((char *) (-3 + proxy_vector_924X))) + (((uid_902X)<<2))));
        if ((3 == (3 & thing_925X))) {
          if ((15 == (31 & ((((*((long *) ((((char *) (-3 + thing_925X))) + -4))))>>2))))) {
            proxy_926X = *((long *) (((char *) (-3 + (*((long *) ((((char *) (-3 + thing_925X))) + 16)))))));
            count_927X = (((*((long *) ((((char *) (-3 + thing_925X))) + 12))))>>2);
            if ((count_927X < 1)) {
              addr_928X = ALLOCATE_SPACE(0, 12);
              *((long *) addr_928X) = 2050;
              x_929X = 3 + (((long) (addr_928X + 4)));
              *((long *) (((char *) (-3 + x_929X)))) = thing_925X;
              *((long *) ((((char *) (-3 + x_929X))) + 4)) = count_903X;
              b_930X = Sbad_count_proxiesS;
              addr_931X = ALLOCATE_SPACE(0, 12);
              *((long *) addr_931X) = 2050;
              x_932X = 3 + (((long) (addr_931X + 4)));
              *((long *) (((char *) (-3 + x_932X)))) = x_929X;
              *((long *) ((((char *) (-3 + x_932X))) + 4)) = b_930X;
              Sbad_count_proxiesS = x_932X;
              goto L13415;}
            else {
              if ((131072 < (count_927X + count_903X))) {
                if ((1 == (*((long *) ((((char *) (-3 + thing_925X))) + 4))))) {
                  goto L10590;}
                else {
                  b_933X = Sbad_count_proxiesS;
                  addr_934X = ALLOCATE_SPACE(0, 12);
                  *((long *) addr_934X) = 2050;
                  x_935X = 3 + (((long) (addr_934X + 4)));
                  *((long *) (((char *) (-3 + x_935X)))) = thing_925X;
                  *((long *) ((((char *) (-3 + x_935X))) + 4)) = b_933X;
                  Sbad_count_proxiesS = x_935X;
                  goto L10590;}}
              else {
                goto L10590;}}}
          else {
            goto L13420;}}
        else {
          goto L13420;}}
      else {
        arg0K0 = aspace_uid_901X;
        arg0K1 = uid_902X;
        arg0K2 = (((count_903X)<<2));
        arg1K3 = addr_886X;
        arg1K4 = next_855X;
        goto L10704;}}
    else {
      goto L14290;}}
  else {
    goto L14290;}}
 L15132: {
  arg1K0 = (addr_886X + 4);
  goto L15126;}
 L12020: {
  if ((3 == (3 & thing_627X))) {
    if ((3 == (31 & ((((*((long *) ((((char *) (-3 + thing_627X))) + -4))))>>2))))) {
      arg3K0 = "#{procedure}";
      goto L12026;}
    else {
      arg3K0 = "???";
      goto L12026;}}
  else {
    arg3K0 = "???";
    goto L12026;}}
 L17354: {
  StemplateS = (SvalS);
  Slosing_opcodeS = (0 - i_757X);
  arg0K0 = arg_count_813X;
  goto L16469;}
 L17440: {
  ps_error("interrupt handler is not a closure", 1, i_757X);
  goto L17354;}
 L8953: {
  n_936X = arg0K0;
  if ((0 == (((n_936X)<<2)))) {
    SvalS = q_823X;
    Scode_pointerS = ((Scode_pointerS) + 1);
    arg1K0 = (Scode_pointerS);
    goto L19170;}
  else {
    merged_arg0K0 = 6;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 208;
    goto push_exception_continuationB;
   push_exception_continuationB_return_208:
    *((long *) (SstackS)) = arg2_166X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = y_167X;
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}}
 L26764: {
  v_937X = ps_close_fd(index_825X);
  arg0K0 = v_937X;
  goto L26759;}
 L26759: {
  status_938X = arg0K0;
  if ((status_938X == NO_ERRORS)) {
    merged_arg0K0 = reason_910X;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 209;
    goto push_exception_continuationB;
   push_exception_continuationB_return_209:
    *((long *) (SstackS)) = arg2_685X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_686X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}
  else {
    merged_arg0K0 = status_938X;
    merged_arg0K1 = index_825X;
    merged_arg0K2 = arg2_685X;
    channel_close_error_return_tag = 0;
    goto channel_close_error;
   channel_close_error_return_0:
    merged_arg0K0 = reason_910X;
    merged_arg0K1 = 1;
    push_exception_continuationB_return_tag = 210;
    goto push_exception_continuationB;
   push_exception_continuationB_return_210:
    *((long *) (SstackS)) = arg2_685X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = (((mode_686X)<<2));
    SstackS = ((SstackS) + -4);
    arg0K0 = 2;
    goto L17068;}}
 L2187: {
  i_939X = arg0K0;
  if ((i_939X == new_count_879X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_880X;
    Snumber_of_channelsS = new_count_879X;
    goto L10939;}
  else {
    *(new_vm_channels_880X + i_939X) = 1;
    arg0K0 = (1 + i_939X);
    goto L2187;}}
 L37173: {
  x_940X = arg0K0;
  *((long *) (SstackS)) = x_940X;
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = x_777X;
  SstackS = ((SstackS) + -4);
  merged_arg0K0 = status_917X;
  merged_arg0K1 = key_692X;
  get_error_string_return_tag = 4;
  goto get_error_string;
 get_error_string_return_4:
  x_941X = get_error_string0_return_value;
  *((long *) (SstackS)) = x_941X;
  SstackS = ((SstackS) + -4);
  arg0K0 = 6;
  goto L17068;}
 L10704: {
  aspace_uid_942X = arg0K0;
  uid_943X = arg0K1;
  count_944X = arg0K2;
  addr_945X = arg1K3;
  stob_start_946X = arg1K4;
  addr_947X = ALLOCATE_SPACE(2, 24);
  *((long *) addr_947X) = 5130;
  vector_948X = 3 + (((long) (addr_947X + 4)));
  value_949X = ((aspace_uid_942X)<<2);
  addr_950X = ((char *) (-3 + vector_948X));
  WRITE_BARRIER(addr_950X, value_949X);
  *((long *) addr_950X) = value_949X;
  value_951X = ((uid_943X)<<2);
  addr_952X = (((char *) (-3 + vector_948X))) + 4;
  WRITE_BARRIER(addr_952X, value_951X);
  *((long *) addr_952X) = value_951X;
  addr_953X = (((char *) (-3 + vector_948X))) + 8;
  WRITE_BARRIER(addr_953X, count_944X);
  *((long *) addr_953X) = count_944X;
  value_954X = 3 + (((long) stob_start_946X));
  addr_955X = (((char *) (-3 + vector_948X))) + 12;
  WRITE_BARRIER(addr_955X, value_954X);
  *((long *) addr_955X) = value_954X;
  value_956X = -4 & (addr_945X - stob_start_946X);
  addr_957X = (((char *) (-3 + vector_948X))) + 16;
  WRITE_BARRIER(addr_957X, value_956X);
  *((long *) addr_957X) = value_956X;
  b_958X = Snew_uidsS;
  addr_959X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_959X) = 2050;
  x_960X = 3 + (((long) (addr_959X + 4)));
  *((long *) (((char *) (-3 + x_960X)))) = vector_948X;
  *((long *) ((((char *) (-3 + x_960X))) + 4)) = b_958X;
  Snew_uidsS = x_960X;
  arg0K0 = 1;
  goto L15138;}
 L14351: {
  thing_961X = arg0K0;
  if ((3 == (3 & thing_961X))) {
    arg0K0 = thing_961X;
    goto L15138;}
  else {
    arg0K0 = aspace_uid_894X;
    arg0K1 = uid_895X;
    arg0K2 = 1;
    arg1K3 = addr_886X;
    arg1K4 = next_855X;
    goto L10704;}}
 L13415: {
  if ((3 == (3 & proxy_926X))) {
    if ((14 == (31 & ((((*((long *) ((((char *) (-3 + proxy_926X))) + -4))))>>2))))) {
      arg0K0 = proxy_926X;
      goto L15138;}
    else {
      goto L13419;}}
  else {
    goto L13419;}}
 L10590: {
  val_962X = (((count_927X + count_903X))<<2);
  addr_963X = (((char *) (-3 + thing_925X))) + 12;
  WRITE_BARRIER(addr_963X, val_962X);
  *((long *) addr_963X) = val_962X;
  goto L13415;}
 L13420: {
  addr_964X = ALLOCATE_SPACE(15, 28);
  *((long *) addr_964X) = 6206;
  x_965X = 3 + (((long) (addr_964X + 4)));
  *((long *) (((char *) (-3 + x_965X)))) = (((uid_902X)<<2));
  *((long *) ((((char *) (-3 + x_965X))) + 4)) = aspace_923X;
  *((long *) ((((char *) (-3 + x_965X))) + 8)) = 529;
  *((long *) ((((char *) (-3 + x_965X))) + 12)) = (((count_903X)<<2));
  *((long *) ((((char *) (-3 + x_965X))) + 16)) = 1;
  *((long *) ((((char *) (-3 + x_965X))) + 20)) = 25;
  addr_966X = (((char *) (-3 + proxy_vector_924X))) + (((uid_902X)<<2));
  WRITE_BARRIER(addr_966X, x_965X);
  *((long *) addr_966X) = x_965X;
  addr_967X = ALLOCATE_SPACE(14, 8);
  *((long *) addr_967X) = 1082;
  x_968X = 3 + (((long) (addr_967X + 4)));
  *((long *) (((char *) (-3 + x_968X)))) = x_965X;
  addr_969X = ALLOCATE_SPACE(12, 8);
  *((long *) addr_969X) = 1074;
  x_970X = 3 + (((long) (addr_969X + 4)));
  *((long *) (((char *) (-3 + x_970X)))) = x_968X;
  addr_971X = (((char *) (-3 + x_965X))) + 16;
  WRITE_BARRIER(addr_971X, x_970X);
  *((long *) addr_971X) = x_970X;
  arg0K0 = x_968X;
  goto L15138;}
 L14290: {
  arg0K0 = aspace_uid_901X;
  arg0K1 = uid_902X;
  arg0K2 = (((count_903X)<<2));
  arg1K3 = addr_886X;
  arg1K4 = next_855X;
  goto L10704;}
 L13419: {
  addr_972X = ALLOCATE_SPACE(14, 8);
  *((long *) addr_972X) = 1082;
  x_973X = 3 + (((long) (addr_972X + 4)));
  *((long *) (((char *) (-3 + x_973X)))) = thing_925X;
  addr_974X = ALLOCATE_SPACE(12, 8);
  *((long *) addr_974X) = 1074;
  x_975X = 3 + (((long) (addr_974X + 4)));
  *((long *) (((char *) (-3 + x_975X)))) = x_973X;
  addr_976X = (((char *) (-3 + thing_925X))) + 16;
  WRITE_BARRIER(addr_976X, x_975X);
  *((long *) addr_976X) = x_975X;
  arg0K0 = x_973X;
  goto L15138;}
 loseD0: {
  message_76X = merged_arg3K0;{
  why_977X = (((*((long *) ((SstackS) + (4 + (((nargs_419X)<<2)))))))>>2);
  ps_write_string("Template UIDs: ", (stderr));
  current_template_978X = StemplateS;
  out_979X = stderr;
  if ((0 == (3 & (*((long *) ((((char *) (-3 + current_template_978X))) + 4)))))) {
    if ((current_template_978X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
      arg2K0 = 0;
      goto L10384;}
    else {
      ps_write_integer(((((*((long *) ((((char *) (-3 + current_template_978X))) + 4))))>>2)), out_979X);
      arg2K0 = 1;
      goto L10384;}}
  else {
    arg2K0 = 0;
    goto L10384;}}
 L10384: {
  not_firstP_980X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_980X;
  goto L10391;}
 L10391: {
  cont_981X = arg0K0;
  not_firstP_982X = arg2K1;
  if ((3 == (3 & cont_981X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + cont_981X))) + -4))))>>2))))) {
      template_983X = *((long *) ((((char *) (-3 + cont_981X))) + 8));
      if ((0 == (3 & (*((long *) ((((char *) (-3 + template_983X))) + 4)))))) {
        if ((template_983X == (*((long *) ((((char *) (-3 + (Sbottom_of_stackS)))) + 8))))) {
          arg2K0 = not_firstP_982X;
          goto L10400;}
        else {
          if (not_firstP_982X) {
            ps_write_string(" <- ", out_979X);
            goto L10439;}
          else {
            goto L10439;}}}
      else {
        arg2K0 = not_firstP_982X;
        goto L10400;}}
    else {
      goto L17087;}}
  else {
    goto L17087;}}
 L10400: {
  v_984X = arg2K0;
  arg0K0 = (*((long *) (((char *) (-3 + cont_981X)))));
  arg2K1 = v_984X;
  goto L10391;}
 L10439: {
  ps_write_integer(((((*((long *) ((((char *) (-3 + template_983X))) + 4))))>>2)), out_979X);
  arg2K0 = 1;
  goto L10400;}
 L17087: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_977X == 1)) {
    if ((0 == (3 & (*((long *) ((((char *) (-3 + (*((long *) ((SstackS) + (((nargs_419X)<<2)))))))) + 4)))))) {
      ps_error(message_76X, 3, opcode_420X, why_977X, ((((*((long *) ((((char *) (-3 + (*((long *) ((SstackS) + (((nargs_419X)<<2)))))))) + 4))))>>2)));
      goto loseD0_return;}
    else {
      goto L17111;}}
  else {
    goto L17111;}}
 L17111: {
  ps_error(message_76X, 2, opcode_420X, why_977X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L41125;}
 L41125: {
  type_985X = get_next_event(&channel_986X, &status_987X);
  if ((type_985X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L16273;}
    else {
      arg0K0 = 1;
      goto L16273;}}
  else {
    if ((type_985X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L16273;}
    else {
      if ((type_985X == IO_COMPLETION_EVENT)) {
        channel_988X = *((Svm_channelsS) + channel_986X);
        val_989X = ((status_987X)<<2);
        addr_990X = (((char *) (-3 + channel_988X))) + 16;
        WRITE_BARRIER(addr_990X, val_989X);
        *((long *) addr_990X) = val_989X;
        if ((1 == (*((long *) ((((char *) (-3 + channel_988X))) + 12))))) {
          if ((1 == (Spending_channels_headS))) {
            Spending_channels_headS = channel_988X;
            Spending_channels_tailS = channel_988X;
            arg0K0 = 8;
            goto L16273;}
          else {
            addr_991X = (((char *) (-3 + (Spending_channels_tailS)))) + 12;
            WRITE_BARRIER(addr_991X, channel_988X);
            *((long *) addr_991X) = channel_988X;
            Spending_channels_tailS = channel_988X;
            arg0K0 = 8;
            goto L16273;}}
        else {
          arg0K0 = 8;
          goto L16273;}}
      else {
        if ((type_985X == NO_EVENT)) {
          arg0K0 = 0;
          goto L16273;}
        else {
          if ((type_985X == ERROR_EVENT)) {
            ps_write_string("OS error while getting event", (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            ps_write_string((ps_error_string(status_987X)), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = 0;
            goto L16273;}
          else {
            ps_write_string("unknown type of event", (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = 0;
            goto L16273;}}}}}}
 L16273: {
  v_992X = arg0K0;
  Spending_interruptsS = (v_992X | (Spending_interruptsS));
  if ((type_985X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        goto L16300;}
      else {
        goto L16300;}}
    else {
      Spending_interruptPS = 1;
      goto L16300;}}
  else {
    goto L41125;}}
 L16300: {
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      goto L41125;}
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
  wants_stack_args_72X = merged_arg0K0;
  stack_arg_count_73X = merged_arg0K1;
  list_args_74X = merged_arg0K2;
  list_arg_count_75X = merged_arg0K3;{
  if ((stack_arg_count_73X == wants_stack_args_72X)) {
    merged_arg0K0 = list_args_74X;
    merged_arg0K1 = list_arg_count_75X;
    copy_listS_return_tag = 1;
    goto copy_listS;
   copy_listS_return_1:
    x_993X = copy_listS0_return_value;
    *((long *) (SstackS)) = x_993X;
    SstackS = ((SstackS) + -4);
    goto rest_list_setup_return;}
  else {
    if ((stack_arg_count_73X < wants_stack_args_72X)) {
      count_994X = wants_stack_args_72X - stack_arg_count_73X;
      merged_arg0K0 = list_args_74X;
      merged_arg0K1 = count_994X;
      push_list_return_tag = 3;
      goto push_list;
     push_list_return_3:
      v_995X = push_list0_return_value;
      merged_arg0K0 = v_995X;
      merged_arg0K1 = (list_arg_count_75X - count_994X);
      copy_listS_return_tag = 2;
      goto copy_listS;
     copy_listS_return_2:
      x_996X = copy_listS0_return_value;
      *((long *) (SstackS)) = x_996X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setup_return;}
    else {
      merged_arg0K0 = list_args_74X;
      merged_arg0K1 = list_arg_count_75X;
      copy_listS_return_tag = 3;
      goto copy_listS;
     copy_listS_return_3:
      v_997X = copy_listS0_return_value;
      merged_arg0K0 = v_997X;
      merged_arg0K1 = (stack_arg_count_73X - wants_stack_args_72X);
      pop_args_GlistS_return_tag = 5;
      goto pop_args_GlistS;
     pop_args_GlistS_return_5:
      x_998X = pop_args_GlistS0_return_value;
      *((long *) (SstackS)) = x_998X;
      SstackS = ((SstackS) + -4);
      goto rest_list_setup_return;}}}
 rest_list_setup_return:
  switch (rest_list_setup_return_tag) {
  case 0: goto rest_list_setup_return_0;
  case 1: goto rest_list_setup_return_1;
  default: goto rest_list_setup_return_2;
  }}

 push_list: {
  list_70X = merged_arg0K0;
  count_71X = merged_arg0K1;{
  *((long *) (SstackS)) = list_70X;
  SstackS = ((SstackS) + -4);
  if ((count_71X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L16106;}
  else {
    space_999X = 1 + (((((Sstack_endS) - (SstackS)))>>2));
    v_1000X = AVAILABLEp(space_999X);
    if (v_1000X) {
      arg2K0 = 1;
      arg0K1 = 0;
      goto L16190;}
    else {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 30;
      goto collect_saving_temp;
     collect_saving_temp_return_30:
      v_1001X = AVAILABLEp(space_999X);
      if (v_1001X) {
        arg2K0 = 1;
        arg0K1 = 0;
        goto L16190;}
      else {
        arg2K0 = 0;
        arg0K1 = 0;
        goto L16190;}}}}
 L16106: {
  SstackS = ((SstackS) + 4);
  list_1002X = *((long *) (SstackS));
  arg0K0 = count_71X;
  arg0K1 = list_1002X;
  goto L16115;}
 L16190: {
  okayP_1003X = arg2K0;
  key_1004X = arg0K1;
  if (okayP_1003X) {
    arg0K0 = key_1004X;
    goto L16141;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = key_1004X;
    goto L16141;}}
 L16115: {
  i_1005X = arg0K0;
  l_1006X = arg0K1;
  if ((0 < i_1005X)) {
    *((long *) (SstackS)) = (*((long *) (((char *) (-3 + l_1006X)))));
    SstackS = ((SstackS) + -4);
    arg0K0 = (-1 + i_1005X);
    arg0K1 = (*((long *) ((((char *) (-3 + l_1006X))) + 4)));
    goto L16115;}
  else {
    push_list0_return_value = l_1006X;
    goto push_list_return;}}
 L16141: {
  v_1007X = arg0K0;
  merged_arg0K0 = v_1007X;
  copy_stack_into_heap_return_tag = 4;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_4:
  if ((count_71X < (64 + (((((SstackS) - (Sstack_limitS)))>>2))))) {
    goto L16106;}
  else {
    ps_error("VM's stack is too small (how can this happen?)", 0);
    goto L16106;}}
 push_list_return:
  switch (push_list_return_tag) {
  case 0: goto push_list_return_0;
  case 1: goto push_list_return_1;
  case 2: goto push_list_return_2;
  default: goto push_list_return_3;
  }}

 copy_stack_into_heap: {
  key_69X = merged_arg0K0;{
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L15168;}
 L15168: {
  p_1008X = arg1K0;
  i_1009X = arg0K1;
  if ((2 == (3 & (*((long *) p_1008X))))) {
    if ((54 == (*((long *) p_1008X)))) {
      arg0K0 = (-1 + i_1009X);
      goto L15155;}
    else {
      arg0K0 = i_1009X;
      goto L15155;}}
  else {
    arg1K0 = (p_1008X + 4);
    arg0K1 = (1 + i_1009X);
    goto L15168;}}
 L15155: {
  arg_count_1010X = arg0K0;
  top_1011X = SstackS;
  if ((1 == (ScontS))) {
    goto L15160;}
  else {
    merged_arg0K0 = key_69X;
    merged_arg0K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L15160;}}
 L15160: {
  stob_1012X = ScontS;
  arg1K0 = ((((char *) (-3 + stob_1012X))) + -8);
  arg1K1 = (top_1011X + (((arg_count_1010X)<<2)));
  goto L15193;}
 L15193: {
  loc_1013X = arg1K0;
  arg_1014X = arg1K1;
  if ((top_1011X < arg_1014X)) {
    *((long *) loc_1013X) = (*((long *) arg_1014X));
    arg1K0 = (loc_1013X + -4);
    arg1K1 = (arg_1014X + -4);
    goto L15193;}
  else {
    SstackS = loc_1013X;
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
  key_67X = merged_arg0K0;
  reason_68X = merged_arg0K1;{
  p_1015X = SenvS;
  if ((3 == (3 & p_1015X))) {
    if ((p_1015X < (((long) (Sstack_beginS))))) {
      goto L14498;}
    else {
      if (((((long) (Sstack_endS))) < p_1015X)) {
        goto L14498;}
      else {
        merged_arg0K0 = (SenvS);
        merged_arg0K1 = (ScontS);
        merged_arg0K2 = key_67X;
        merged_arg0K3 = reason_68X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_1016X = save_env_in_heap0_return_value;
        SenvS = v_1016X;
        goto L14498;}}}
  else {
    goto L14498;}}
 L14498: {
  end_1017X = *((long *) (((char *) (-3 + (Sbottom_of_stackS)))));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L14451;}
 L14451: {
  cont_1018X = arg0K0;
  previous_1019X = arg0K1;
  if ((cont_1018X == (Sbottom_of_stackS))) {
    *((long *) (((char *) (-3 + previous_1019X)))) = end_1017X;
    ScontS = (Sbottom_of_stackS);
    really_preserve_continuation0_return_value = (*((long *) (((char *) (-3 + (Sbottom_of_stackS))))));
    goto really_preserve_continuation_return;}
  else {
    p_1020X = *((long *) ((((char *) (-3 + cont_1018X))) + 12));
    if ((3 == (3 & p_1020X))) {
      if ((p_1020X < (((long) (Sstack_beginS))))) {
        goto L14469;}
      else {
        if (((((long) (Sstack_endS))) < p_1020X)) {
          goto L14469;}
        else {
          merged_arg0K0 = (*((long *) ((((char *) (-3 + cont_1018X))) + 12)));
          merged_arg0K1 = cont_1018X;
          merged_arg0K2 = key_67X;
          merged_arg0K3 = reason_68X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L14469;}}}
    else {
      goto L14469;}}}
 L14469: {
  header_1021X = *((long *) ((((char *) (-3 + cont_1018X))) + -4));
  addr_1022X = ALLOCATE_SPACE((31 & (((header_1021X)>>2))), (4 + ((long)(((unsigned long)header_1021X)>>8))));
  data_addr_1023X = addr_1022X + 4;
  *((long *) addr_1022X) = header_1021X;
  memcpy((void *)data_addr_1023X, (void *)(((char *) (-3 + cont_1018X))),((long)(((unsigned long)header_1021X)>>8)));
  new_1024X = 3 + (((long) data_addr_1023X));
  *((long *) (((char *) (-3 + previous_1019X)))) = new_1024X;
  arg0K0 = (*((long *) (((char *) (-3 + new_1024X)))));
  arg0K1 = new_1024X;
  goto L14451;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 copy_env: {
  env_66X = merged_arg0K0;{
  header_1025X = *((long *) ((((char *) (-3 + env_66X))) + -4));
  addr_1026X = ALLOCATE_SPACE((31 & (((header_1025X)>>2))), (4 + ((long)(((unsigned long)header_1025X)>>8))));
  data_addr_1027X = addr_1026X + 4;
  *((long *) addr_1026X) = header_1025X;
  memcpy((void *)data_addr_1027X, (void *)(((char *) (-3 + env_66X))),((long)(((unsigned long)header_1025X)>>8)));
  new_1028X = 3 + (((long) data_addr_1027X));
  addr_1029X = ((char *) (-3 + env_66X));
  WRITE_BARRIER(addr_1029X, 54);
  *((long *) addr_1029X) = 54;
  *((long *) ((((char *) (-3 + env_66X))) + -4)) = new_1028X;
  copy_env0_return_value = new_1028X;
  goto copy_env_return;}
 copy_env_return:
  switch (copy_env_return_tag) {
  case 0: goto copy_env_return_0;
  default: goto copy_env_return_1;
  }}

 save_env_in_heap: {
  env_62X = merged_arg0K0;
  cont_63X = merged_arg0K1;
  key_64X = merged_arg0K2;
  reason_65X = merged_arg0K3;{
  merged_arg0K0 = env_62X;
  merged_arg0K1 = key_64X;
  merged_arg0K2 = reason_65X;
  copy_env_return_tag = 0;
  goto copy_env;
 copy_env_return_0:
  top_1030X = copy_env0_return_value;
  arg0K0 = top_1030X;
  goto L12585;}
 L12585: {
  env_1031X = arg0K0;
  p_1032X = *((long *) (((char *) (-3 + env_1031X))));
  if ((3 == (3 & p_1032X))) {
    if ((p_1032X < (((long) (Sstack_beginS))))) {
      goto L12604;}
    else {
      if (((((long) (Sstack_endS))) < p_1032X)) {
        goto L12604;}
      else {
        merged_arg0K0 = (*((long *) (((char *) (-3 + env_1031X)))));
        merged_arg0K1 = key_64X;
        merged_arg0K2 = reason_65X;
        copy_env_return_tag = 1;
        goto copy_env;
       copy_env_return_1:
        new_1033X = copy_env0_return_value;
        addr_1034X = ((char *) (-3 + env_1031X));
        WRITE_BARRIER(addr_1034X, new_1033X);
        *((long *) addr_1034X) = new_1033X;
        arg0K0 = new_1033X;
        goto L12585;}}}
  else {
    goto L12604;}}
 L12604: {
  arg0K0 = cont_63X;
  goto L12608;}
 L12608: {
  cont_1035X = arg0K0;
  env_1036X = *((long *) ((((char *) (-3 + cont_1035X))) + 12));
  if ((3 == (3 & env_1036X))) {
    if ((3 == (3 & (*((long *) ((((char *) (-3 + env_1036X))) + -4)))))) {
      *((long *) ((((char *) (-3 + cont_1035X))) + 12)) = (*((long *) ((((char *) (-3 + env_1036X))) + -4)));
      arg0K0 = (*((long *) (((char *) (-3 + cont_1035X)))));
      goto L12608;}
    else {
      save_env_in_heap0_return_value = top_1030X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = top_1030X;
    goto save_env_in_heap_return;}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  default: goto save_env_in_heap_return_2;
  }}

 pop_args_GlistS: {
  start_60X = merged_arg0K0;
  count_61X = merged_arg0K1;{
  space_1037X = 3 * count_61X;
  v_1038X = AVAILABLEp(space_1037X);
  if (v_1038X) {
    arg2K0 = 1;
    arg0K1 = start_60X;
    goto L16050;}
  else {
    merged_arg0K0 = start_60X;
    collect_saving_temp_return_tag = 31;
    goto collect_saving_temp;
   collect_saving_temp_return_31:
    temp_1039X = collect_saving_temp0_return_value;
    v_1040X = AVAILABLEp(space_1037X);
    if (v_1040X) {
      arg2K0 = 1;
      arg0K1 = temp_1039X;
      goto L16050;}
    else {
      arg2K0 = 0;
      arg0K1 = temp_1039X;
      goto L16050;}}}
 L16050: {
  okayP_1041X = arg2K0;
  temp_1042X = arg0K1;
  if (okayP_1041X) {
    arg0K0 = temp_1042X;
    goto L16024;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_1042X;
    goto L16024;}}
 L16024: {
  start_1043X = arg0K0;
  arg0K0 = start_1043X;
  arg0K1 = count_61X;
  goto L16033;}
 L16033: {
  args_1044X = arg0K0;
  count_1045X = arg0K1;
  if ((0 == count_1045X)) {
    pop_args_GlistS0_return_value = args_1044X;
    goto pop_args_GlistS_return;}
  else {
    SstackS = ((SstackS) + 4);
    a_1046X = *((long *) (SstackS));
    addr_1047X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_1047X) = 2050;
    x_1048X = 3 + (((long) (addr_1047X + 4)));
    *((long *) (((char *) (-3 + x_1048X)))) = a_1046X;
    *((long *) ((((char *) (-3 + x_1048X))) + 4)) = args_1044X;
    arg0K0 = x_1048X;
    arg0K1 = (-1 + count_1045X);
    goto L16033;}}
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
  list_58X = merged_arg0K0;
  length_59X = merged_arg0K1;{
  if ((0 == length_59X)) {
    copy_listS0_return_value = 25;
    goto copy_listS_return;}
  else {
    space_1049X = 3 * length_59X;
    v_1050X = AVAILABLEp(space_1049X);
    if (v_1050X) {
      arg2K0 = 1;
      arg0K1 = list_58X;
      goto L15930;}
    else {
      merged_arg0K0 = list_58X;
      collect_saving_temp_return_tag = 32;
      goto collect_saving_temp;
     collect_saving_temp_return_32:
      temp_1051X = collect_saving_temp0_return_value;
      v_1052X = AVAILABLEp(space_1049X);
      if (v_1052X) {
        arg2K0 = 1;
        arg0K1 = temp_1051X;
        goto L15930;}
      else {
        arg2K0 = 0;
        arg0K1 = temp_1051X;
        goto L15930;}}}}
 L15930: {
  okayP_1053X = arg2K0;
  temp_1054X = arg0K1;
  if (okayP_1053X) {
    arg0K0 = temp_1054X;
    goto L15888;}
  else {
    ps_error("Scheme48 heap overflow", 0);
    arg0K0 = temp_1054X;
    goto L15888;}}
 L15888: {
  list_1055X = arg0K0;
  a_1056X = *((long *) (((char *) (-3 + list_1055X))));
  addr_1057X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_1057X) = 2050;
  x_1058X = 3 + (((long) (addr_1057X + 4)));
  *((long *) (((char *) (-3 + x_1058X)))) = a_1056X;
  *((long *) ((((char *) (-3 + x_1058X))) + 4)) = 25;
  arg0K0 = (*((long *) ((((char *) (-3 + list_1055X))) + 4)));
  arg0K1 = x_1058X;
  goto L15903;}
 L15903: {
  l_1059X = arg0K0;
  last_1060X = arg0K1;
  if ((25 == l_1059X)) {
    copy_listS0_return_value = x_1058X;
    goto copy_listS_return;}
  else {
    a_1061X = *((long *) (((char *) (-3 + l_1059X))));
    addr_1062X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_1062X) = 2050;
    x_1063X = 3 + (((long) (addr_1062X + 4)));
    *((long *) (((char *) (-3 + x_1063X)))) = a_1061X;
    *((long *) ((((char *) (-3 + x_1063X))) + 4)) = 25;
    addr_1064X = (((char *) (-3 + last_1060X))) + 4;
    WRITE_BARRIER(addr_1064X, x_1063X);
    *((long *) addr_1064X) = x_1063X;
    arg0K0 = (*((long *) ((((char *) (-3 + l_1059X))) + 4)));
    arg0K1 = x_1063X;
    goto L15903;}}
 copy_listS_return:
  switch (copy_listS_return_tag) {
  case 0: goto copy_listS_return_0;
  case 1: goto copy_listS_return_1;
  case 2: goto copy_listS_return_2;
  default: goto copy_listS_return_3;
  }}

 collect_saving_temp: {
  value_57X = merged_arg0K0;{
  pc_1065X = ((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2);
  begin_collection();
  v_1066X = trace_value((StemplateS));
  StemplateS = v_1066X;
  v_1067X = trace_value((SvalS));
  SvalS = v_1067X;
  v_1068X = trace_value((Scurrent_threadS));
  Scurrent_threadS = v_1068X;
  v_1069X = trace_value((Ssession_dataS));
  Ssession_dataS = v_1069X;
  v_1070X = trace_value((Sexception_handlersS));
  Sexception_handlersS = v_1070X;
  v_1071X = trace_value((Sexception_templateS));
  Sexception_templateS = v_1071X;
  v_1072X = trace_value((Sinterrupt_handlersS));
  Sinterrupt_handlersS = v_1072X;
  v_1073X = trace_value((Sinterrupt_templateS));
  Sinterrupt_templateS = v_1073X;
  v_1074X = trace_value((Sinterrupted_templateS));
  Sinterrupted_templateS = v_1074X;
  v_1075X = trace_value((Sfinalize_theseS));
  Sfinalize_theseS = v_1075X;
  env_1076X = trace_value((SenvS));
  SenvS = env_1076X;
  v_1077X = trace_value((Spending_channels_headS));
  Spending_channels_headS = v_1077X;
  v_1078X = trace_value((Spending_channels_tailS));
  Spending_channels_tailS = v_1078X;
  if ((Sstack_warningPS)) {
    arg1K0 = (Sstack_beginS);
    goto L5992;}
  else {
    goto L6025;}}
 L5992: {
  a_1079X = arg1K0;
  if ((252645135 == (*((long *) a_1079X)))) {
    arg1K0 = (a_1079X + 4);
    goto L5992;}
  else {
    unused_1080X = (((a_1079X - (Sstack_beginS)))>>2);
    if ((unused_1080X < 30)) {
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      ps_write_string("[Alert: stack overconsumption (", (stderr));
      ps_write_integer(unused_1080X, (stderr));
      ps_write_string("); please inform the Scheme 48 implementors]", (stderr));
      { long ignoreXX;
      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
      Sstack_warningPS = 0;
      goto L6025;}
    else {
      goto L6025;}}}
 L6025: {
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L6090;}
 L6090: {
  p_1081X = arg1K0;
  i_1082X = arg0K1;
  if ((2 == (3 & (*((long *) p_1081X))))) {
    if ((54 == (*((long *) p_1081X)))) {
      arg0K0 = (-1 + i_1082X);
      goto L6027;}
    else {
      arg0K0 = i_1082X;
      goto L6027;}}
  else {
    arg1K0 = (p_1081X + 4);
    arg0K1 = (1 + i_1082X);
    goto L6090;}}
 L6027: {
  arg_count_1083X = arg0K0;
  trace_locationsB(((SstackS) + 4), ((SstackS) + (4 + (((arg_count_1083X)<<2)))));
  p_1084X = SenvS;
  if ((3 == (3 & p_1084X))) {
    if ((p_1084X < (((long) (Sstack_beginS))))) {
      goto L6043;}
    else {
      if (((((long) (Sstack_endS))) < p_1084X)) {
        goto L6043;}
      else {
        env_1085X = SenvS;
        arg0K0 = env_1085X;
        goto L6203;}}}
  else {
    goto L6043;}}
 L6043: {
  v_1086X = trace_value((SenvS));
  SenvS = v_1086X;
  goto L6048;}
 L6203: {
  env_1087X = arg0K0;
  if ((3 == (3 & env_1087X))) {
    if ((env_1087X < (((long) (Sstack_beginS))))) {
      goto L6048;}
    else {
      if (((((long) (Sstack_endS))) < env_1087X)) {
        goto L6048;}
      else {
        trace_stob_contentsB(env_1087X);
        arg0K0 = (*((long *) (((char *) (-3 + env_1087X)))));
        goto L6203;}}}
  else {
    goto L6048;}}
 L6048: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L6053;}
 L6053: {
  cont_1088X = arg0K0;
  last_env_1089X = arg0K1;
  env_1090X = *((long *) ((((char *) (-3 + cont_1088X))) + 12));
  trace_stob_contentsB(cont_1088X);
  if ((env_1090X == last_env_1089X)) {
    goto L6071;}
  else {
    arg0K0 = env_1090X;
    goto L6158;}}
 L6071: {
  if ((cont_1088X == (Sbottom_of_stackS))) {
    value_1091X = trace_value(value_57X);
    okay_1092X = do_gc((Sfinalizer_alistS), &sickly_1093X);
    Sfinalizer_alistS = okay_1092X;
    l2_1094X = Sfinalize_theseS;
    if ((25 == sickly_1093X)) {
      arg0K0 = l2_1094X;
      goto L14850;}
    else {
      arg0K0 = sickly_1093X;
      goto L14873;}}
  else {
    arg0K0 = (*((long *) (((char *) (-3 + cont_1088X)))));
    arg0K1 = env_1090X;
    goto L6053;}}
 L6158: {
  env_1095X = arg0K0;
  if ((3 == (3 & env_1095X))) {
    if ((env_1095X < (((long) (Sstack_beginS))))) {
      goto L6071;}
    else {
      if (((((long) (Sstack_endS))) < env_1095X)) {
        goto L6071;}
      else {
        trace_stob_contentsB(env_1095X);
        arg0K0 = (*((long *) (((char *) (-3 + env_1095X)))));
        goto L6158;}}}
  else {
    goto L6071;}}
 L14850: {
  v_1096X = arg0K0;
  Sfinalize_theseS = v_1096X;
  end_collection();
  arg0K0 = 0;
  goto L14139;}
 L14873: {
  l_1097X = arg0K0;
  if ((25 == (*((long *) ((((char *) (-3 + l_1097X))) + 4))))) {
    addr_1098X = (((char *) (-3 + l_1097X))) + 4;
    WRITE_BARRIER(addr_1098X, l2_1094X);
    *((long *) addr_1098X) = l2_1094X;
    arg0K0 = sickly_1093X;
    goto L14850;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + l_1097X))) + 4)));
    goto L14873;}}
 L14139: {
  i_1099X = arg0K0;
  if ((i_1099X == (Snumber_of_channelsS))) {
    tem_1100X = StemplateS;
    StemplateS = tem_1100X;
    Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1100X)))))))) + (((pc_1065X)>>2)));
    Spending_interruptsS = (4 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        collect_saving_temp0_return_value = value_1091X;
        goto collect_saving_temp_return;}
      else {
        collect_saving_temp0_return_value = value_1091X;
        goto collect_saving_temp_return;}}
    else {
      Spending_interruptPS = 1;
      collect_saving_temp0_return_value = value_1091X;
      goto collect_saving_temp_return;}}
  else {
    channel_1101X = *((Svm_channelsS) + i_1099X);
    if ((1 == channel_1101X)) {
      goto L14175;}
    else {
      header_1102X = *((long *) ((((char *) (-3 + channel_1101X))) + -4));
      if ((3 == (3 & header_1102X))) {
        arg0K0 = header_1102X;
        goto L14168;}
      else {
        if ((0 == (*((long *) (((char *) (-3 + channel_1101X))))))) {
          arg0K0 = 1;
          goto L14168;}
        else {
          id_1103X = *((long *) ((((char *) (-3 + channel_1101X))) + 4));
          if ((0 == (3 & id_1103X))) {
            arg0K0 = id_1103X;
            goto L13247;}
          else {
            if ((3 == (3 & (*((long *) ((((char *) (-3 + id_1103X))) + -4)))))) {
              arg0K0 = (*((long *) ((((char *) (-3 + id_1103X))) + -4)));
              goto L13247;}
            else {
              arg0K0 = id_1103X;
              goto L13247;}}}}}}}
 L14175: {
  arg0K0 = (1 + i_1099X);
  goto L14139;}
 L14168: {
  new_1104X = arg0K0;
  *((Svm_channelsS) + i_1099X) = new_1104X;
  goto L14175;}
 L13247: {
  id_1105X = arg0K0;
  merged_arg0K0 = channel_1101X;
  close_channelB_return_tag = 1;
  goto close_channelB;
 close_channelB_return_1:
  status_1106X = close_channelB0_return_value;
  if ((status_1106X == NO_ERRORS)) {
    goto L13262;}
  else {
    merged_arg0K0 = status_1106X;
    merged_arg0K1 = (*((long *) ((((char *) (-3 + channel_1101X))) + 8)));
    merged_arg0K2 = id_1105X;
    channel_close_error_return_tag = 1;
    goto channel_close_error;
   channel_close_error_return_1:
    goto L13262;}}
 L13262: {
  ps_write_string("Channel closed: ", (stderr));
  if ((0 == (3 & id_1105X))) {
    ps_write_integer((((id_1105X)>>2)), (stderr));
    goto L13280;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_1105X))))), (stderr));
    goto L13280;}}
 L13280: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L14168;}
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
  case 28: goto collect_saving_temp_return_28;
  case 29: goto collect_saving_temp_return_29;
  case 30: goto collect_saving_temp_return_30;
  case 31: goto collect_saving_temp_return_31;
  default: goto collect_saving_temp_return_32;
  }}

 channel_close_error: {
  status_54X = merged_arg0K0;
  index_55X = merged_arg0K1;
  id_56X = merged_arg0K2;{
  ps_write_string("Error: ", (stderr));
  ps_write_string((ps_error_string(status_54X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(" while closing port ", (stderr));
  if ((3 == (3 & id_56X))) {
    if ((19 == (31 & ((((*((long *) ((((char *) (-3 + id_56X))) + -4))))>>2))))) {
      ps_write_string((((char *)(((char *) (-3 + id_56X))))), (stderr));
      goto L11894;}
    else {
      goto L11888;}}
  else {
    goto L11888;}}
 L11894: {
  PS_WRITE_CHAR(10, (stderr), v_1107X)
  channel_close_error0_return_value = v_1107X;
  goto channel_close_error_return;}
 L11888: {
  ps_write_integer((((index_55X)>>2)), (stderr));
  goto L11894;}
 channel_close_error_return:
  switch (channel_close_error_return_tag) {
  case 0: goto channel_close_error_return_0;
  default: goto channel_close_error_return_1;
  }}

 close_channelB: {
  channel_53X = merged_arg0K0;{
  os_index_1108X = (((*((long *) ((((char *) (-3 + channel_53X))) + 8))))>>2);
  v_1109X = *((long *) (((char *) (-3 + channel_53X))));
  if ((4 == v_1109X)) {
    goto L6653;}
  else {
    if ((12 == (*((long *) (((char *) (-3 + channel_53X))))))) {
      goto L6653;}
    else {
      v_1110X = ps_close_fd(os_index_1108X);
      arg0K0 = v_1110X;
      goto L6660;}}}
 L6653: {
  v_1111X = ps_close_fd(os_index_1108X);
  arg0K0 = v_1111X;
  goto L6660;}
 L6660: {
  status_1112X = arg0K0;
  *((Svm_channelsS) + os_index_1108X) = 1;
  addr_1113X = ((char *) (-3 + channel_53X));
  WRITE_BARRIER(addr_1113X, 0);
  *((long *) addr_1113X) = 0;
  close_channelB0_return_value = status_1112X;
  goto close_channelB_return;}
 close_channelB_return:
  switch (close_channelB_return_tag) {
  case 0: goto close_channelB_return_0;
  default: goto close_channelB_return_1;
  }}

 real_next_id: {
  thing_51X = merged_arg0K0;
  decode_vector_52X = merged_arg0K1;{
  next_available_1114X = *((long *) (((char *) (-3 + decode_vector_52X))));
  extracted_1115X = ((next_available_1114X)>>2);
  v_1116X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + decode_vector_52X))) + -4))))>>8))))>>2);
  if ((extracted_1115X < v_1116X)) {
    next_1117X = *((long *) ((((char *) (-3 + decode_vector_52X))) + (((extracted_1115X)<<2))));
    addr_1118X = (((char *) (-3 + decode_vector_52X))) + (((extracted_1115X)<<2));
    WRITE_BARRIER(addr_1118X, thing_51X);
    *((long *) addr_1118X) = thing_51X;
    arg0K0 = next_1117X;
    goto L9867;}
  else {
    old_1119X = Snew_id_hotelS;
    start_1120X = Sother_hpS;
    *((long *) (start_1120X + 4)) = (((long) old_1119X));
    Sother_hpS = (start_1120X + 8);
    *((long *) start_1120X) = thing_51X;
    Snew_id_hotelS = start_1120X;
    arg0K0 = (4 + (((extracted_1115X)<<2)));
    goto L9867;}}
 L9867: {
  next_1121X = arg0K0;
  addr_1122X = ((char *) (-3 + decode_vector_52X));
  WRITE_BARRIER(addr_1122X, next_1121X);
  *((long *) addr_1122X) = next_1121X;
  real_next_id0_return_value = next_available_1114X;
  goto real_next_id_return;}
 real_next_id_return:
  switch (real_next_id_return_tag) {
  case 0: goto real_next_id_return_0;
  case 1: goto real_next_id_return_1;
  case 2: goto real_next_id_return_2;
  case 3: goto real_next_id_return_3;
  case 4: goto real_next_id_return_4;
  default: goto real_next_id_return_5;
  }}

 encode_object: {
  thing_50X = merged_arg0K0;{
  h_1123X = *((long *) ((((char *) (-3 + thing_50X))) + -4));
  if ((3 == (3 & h_1123X))) {
    encode_object0_return_value = h_1123X;
    goto encode_object_return;}
  else {
    temp_1124X = 31 & (((h_1123X)>>2));
    if ((1 == temp_1124X)) {
      uid_1125X = *((long *) (((char *) (-3 + thing_50X))));
      if ((1 == uid_1125X)) {
        merged_arg0K0 = thing_50X;
        merged_arg0K1 = (*((long *) ((((char *) (-3 + (Sour_address_spaceS)))) + 4)));
        real_next_id_return_tag = 0;
        goto real_next_id;
       real_next_id_return_0:
        uid_1126X = real_next_id0_return_value;
        addr_1127X = ((char *) (-3 + thing_50X));
        WRITE_BARRIER(addr_1127X, uid_1126X);
        *((long *) addr_1127X) = uid_1126X;
        arg0K0 = uid_1126X;
        goto L13687;}
      else {
        arg0K0 = uid_1125X;
        goto L13687;}}
    else {
      if ((13 == temp_1124X)) {
        uid_1128X = *((long *) ((((char *) (-3 + thing_50X))) + 8));
        if ((1 == uid_1128X)) {
          merged_arg0K0 = thing_50X;
          merged_arg0K1 = (*((long *) ((((char *) (-3 + (Sour_address_spaceS)))) + 4)));
          real_next_id_return_tag = 1;
          goto real_next_id;
         real_next_id_return_1:
          uid_1129X = real_next_id0_return_value;
          addr_1130X = (((char *) (-3 + thing_50X))) + 8;
          WRITE_BARRIER(addr_1130X, uid_1129X);
          *((long *) addr_1130X) = uid_1129X;
          arg0K0 = uid_1129X;
          goto L13693;}
        else {
          arg0K0 = uid_1128X;
          goto L13693;}}
      else {
        if ((16 == temp_1124X)) {
          uid_1131X = *((long *) (((char *) (-3 + thing_50X))));
          if ((1 == uid_1131X)) {
            merged_arg0K0 = thing_50X;
            merged_arg0K1 = (*((long *) ((((char *) (-3 + (Sour_address_spaceS)))) + 4)));
            real_next_id_return_tag = 2;
            goto real_next_id;
           real_next_id_return_2:
            uid_1132X = real_next_id0_return_value;
            addr_1133X = ((char *) (-3 + thing_50X));
            WRITE_BARRIER(addr_1133X, uid_1132X);
            *((long *) addr_1133X) = uid_1132X;
            arg0K0 = uid_1132X;
            goto L13699;}
          else {
            arg0K0 = uid_1131X;
            goto L13699;}}
        else {
          if ((11 == temp_1124X)) {
            uid_1134X = *((long *) ((((char *) (-3 + thing_50X))) + 8));
            if ((1 == uid_1134X)) {
              merged_arg0K0 = thing_50X;
              merged_arg0K1 = (*((long *) ((((char *) (-3 + (Sour_address_spaceS)))) + 4)));
              real_next_id_return_tag = 3;
              goto real_next_id;
             real_next_id_return_3:
              uid_1135X = real_next_id0_return_value;
              addr_1136X = (((char *) (-3 + thing_50X))) + 8;
              WRITE_BARRIER(addr_1136X, uid_1135X);
              *((long *) addr_1136X) = uid_1135X;
              arg0K0 = uid_1135X;
              goto L10043;}
            else {
              arg0K0 = uid_1134X;
              goto L10043;}}
          else {
            if ((4 == temp_1124X)) {
              uid_1137X = *((long *) (((char *) (-3 + thing_50X))));
              if ((1 == uid_1137X)) {
                merged_arg0K0 = thing_50X;
                merged_arg0K1 = (*((long *) ((((char *) (-3 + (Sour_address_spaceS)))) + 4)));
                real_next_id_return_tag = 4;
                goto real_next_id;
               real_next_id_return_4:
                uid_1138X = real_next_id0_return_value;
                addr_1139X = ((char *) (-3 + thing_50X));
                WRITE_BARRIER(addr_1139X, uid_1138X);
                *((long *) addr_1139X) = uid_1138X;
                arg0K0 = uid_1138X;
                goto L10043;}
              else {
                arg0K0 = uid_1137X;
                goto L10043;}}
            else {
              if ((14 == temp_1124X)) {
                if ((((Stransmit_hpS) + 12) < (Smax_hpS))) {
                  data_1140X = *((long *) (((char *) (-3 + thing_50X))));
                  new_1141X = 15 + (((((Stransmit_hpS) - (SstartS)))<<8));
                  owner_1142X = *((long *) ((((char *) (-3 + data_1140X))) + 4));
                  if ((1 == owner_1142X)) {
                    arg0K0 = 4;
                    goto L13740;}
                  else {
                    arg0K0 = (*((long *) (((char *) (-3 + owner_1142X)))));
                    goto L13740;}}
                else {
                  Stransmit_hpS = (Smax_hpS);
                  encode_object0_return_value = 0;
                  goto encode_object_return;}}
              else {
                if ((((Stransmit_hpS) + (-4 & (3 + ((long)(((unsigned long)h_1123X)>>8))))) < (Smax_hpS))) {
                  new_1143X = 3 + (((((Stransmit_hpS) - (SstartS)))<<8));
                  *((long *) (Stransmit_hpS)) = h_1123X;
                  Stransmit_hpS = ((Stransmit_hpS) + 4);
                  *((long *) ((((char *) (-3 + thing_50X))) + -4)) = new_1143X;
                  old_1144X = Sheartbreak_hotelS;
                  start_1145X = Sother_hpS;
                  *((long *) (start_1145X + 8)) = (((long) old_1144X));
                  Sother_hpS = (start_1145X + 12);
                  *((long *) start_1145X) = thing_50X;
                  *((long *) (start_1145X + 4)) = h_1123X;
                  Sheartbreak_hotelS = start_1145X;
                  new_hp_1146X = (Stransmit_hpS) + (-4 & (3 + ((long)(((unsigned long)h_1123X)>>8))));
                  arg1K0 = (((char *) (-3 + thing_50X)));
                  goto L6393;}
                else {
                  Stransmit_hpS = (Smax_hpS);
                  encode_object0_return_value = 0;
                  goto encode_object_return;}}}}}}}}}
 L13687: {
  p_1147X = arg0K0;
  encode_object0_return_value = (7 + ((((-4 & p_1147X))<<6)));
  goto encode_object_return;}
 L13693: {
  p_1148X = arg0K0;
  encode_object0_return_value = (7 + ((((-4 & p_1148X))<<6)));
  goto encode_object_return;}
 L13699: {
  p_1149X = arg0K0;
  encode_object0_return_value = (7 + ((((-4 & p_1149X))<<6)));
  goto encode_object_return;}
 L10043: {
  uid_1150X = arg0K0;
  if ((((Stransmit_hpS) + 8) < (Smax_hpS))) {
    new_1151X = 11 + (((((Stransmit_hpS) - (SstartS)))<<8));
    if ((3 == (3 & uid_1150X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + uid_1150X))) + -4))))>>2))))) {
        *((long *) (Stransmit_hpS)) = (*((long *) (((char *) (-3 + uid_1150X)))));
        Stransmit_hpS = ((Stransmit_hpS) + 4);
        *((long *) (Stransmit_hpS)) = (*((long *) ((((char *) (-3 + uid_1150X))) + 4)));
        Stransmit_hpS = ((Stransmit_hpS) + 4);
        encode_object0_return_value = new_1151X;
        goto encode_object_return;}
      else {
        goto L10067;}}
    else {
      goto L10067;}}
  else {
    Stransmit_hpS = (Smax_hpS);
    encode_object0_return_value = 0;
    goto encode_object_return;}}
 L13740: {
  descriptor_1152X = arg0K0;
  *((long *) (Stransmit_hpS)) = descriptor_1152X;
  Stransmit_hpS = ((Stransmit_hpS) + 4);
  uid_1153X = *((long *) (((char *) (-3 + data_1140X))));
  if ((1 == uid_1153X)) {
    merged_arg0K0 = data_1140X;
    merged_arg0K1 = (*((long *) ((((char *) (-3 + (Sour_address_spaceS)))) + 8)));
    real_next_id_return_tag = 5;
    goto real_next_id;
   real_next_id_return_5:
    uid_1154X = real_next_id0_return_value;
    addr_1155X = ((char *) (-3 + data_1140X));
    WRITE_BARRIER(addr_1155X, uid_1154X);
    *((long *) addr_1155X) = uid_1154X;
    arg0K0 = uid_1154X;
    goto L13744;}
  else {
    arg0K0 = uid_1153X;
    goto L13744;}}
 L6393: {
  o_1156X = arg1K0;
  if (((Stransmit_hpS) < new_hp_1146X)) {
    *((long *) (Stransmit_hpS)) = (*((long *) o_1156X));
    Stransmit_hpS = ((Stransmit_hpS) + 4);
    arg1K0 = (o_1156X + 4);
    goto L6393;}
  else {
    encode_object0_return_value = new_1143X;
    goto encode_object_return;}}
 L10067: {
  *((long *) (Stransmit_hpS)) = 4;
  Stransmit_hpS = ((Stransmit_hpS) + 4);
  *((long *) (Stransmit_hpS)) = uid_1150X;
  Stransmit_hpS = ((Stransmit_hpS) + 4);
  encode_object0_return_value = new_1151X;
  goto encode_object_return;}
 L13744: {
  descriptor_1157X = arg0K0;
  *((long *) (Stransmit_hpS)) = descriptor_1157X;
  Stransmit_hpS = ((Stransmit_hpS) + 4);
  owner_1158X = *((long *) ((((char *) (-3 + data_1140X))) + 4));
  if ((owner_1158X == 1)) {
    goto L7757;}
  else {
    if ((8 == (*((long *) (((char *) (-3 + owner_1158X))))))) {
      arg0K0 = 0;
      goto L13748;}
    else {
      goto L7757;}}}
 L7757: {
  count_1159X = (((*((long *) ((((char *) (-3 + data_1140X))) + 12))))>>2);
  if ((1 < count_1159X)) {
    if ((count_1159X < 8192)) {
      arg0K0 = ((long)(((unsigned long)count_1159X)>>1));
      goto L7782;}
    else {
      arg0K0 = 4096;
      goto L7782;}}
  else {
    old_1160X = Slosing_proxy_hotelS;
    start_1161X = Sother_hpS;
    *((long *) (start_1161X + 4)) = (((long) old_1160X));
    Sother_hpS = (start_1161X + 8);
    *((long *) start_1161X) = data_1140X;
    Slosing_proxy_hotelS = start_1161X;
    arg0K0 = 1;
    goto L7782;}}
 L13748: {
  descriptor_1162X = arg0K0;
  *((long *) (Stransmit_hpS)) = descriptor_1162X;
  Stransmit_hpS = ((Stransmit_hpS) + 4);
  encode_object0_return_value = new_1141X;
  goto encode_object_return;}
 L7782: {
  send_1163X = arg0K0;
  val_1164X = (((count_1159X - send_1163X))<<2);
  addr_1165X = (((char *) (-3 + data_1140X))) + 12;
  WRITE_BARRIER(addr_1165X, val_1164X);
  *((long *) addr_1165X) = val_1164X;
  arg0K0 = (((send_1163X)<<2));
  goto L13748;}
 encode_object_return:
  switch (encode_object_return_tag) {
  case 0: goto encode_object_return_0;
  default: goto encode_object_return_1;
  }}

 extend_decode_vector: {
  address_space_48X = merged_arg0K0;
  proxiesP_49X = merged_arg2K2;{
  if (proxiesP_49X) {
    arg0K0 = (*((long *) ((((char *) (-3 + address_space_48X))) + 8)));
    goto L13026;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + address_space_48X))) + 4)));
    goto L13026;}}
 L13026: {
  decode_vector_1166X = arg0K0;
  old_length_1167X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + decode_vector_1166X))) + -4))))>>8))))>>2);
  minimum_new_length_1168X = (((*((long *) (((char *) (-3 + decode_vector_1166X))))))>>2);
  x_1169X = ((old_length_1167X)<<1);
  if ((x_1169X < minimum_new_length_1168X)) {
    arg0K0 = minimum_new_length_1168X;
    goto L13042;}
  else {
    arg0K0 = x_1169X;
    goto L13042;}}
 L13042: {
  want_length_1170X = arg0K0;
  v_1171X = AVAILABLEp((1 + want_length_1170X));
  if (v_1171X) {
    arg0K0 = want_length_1170X;
    goto L13060;}
  else {
    v_1172X = AVAILABLEp((1 + minimum_new_length_1168X));
    if (v_1172X) {
      arg0K0 = minimum_new_length_1168X;
      goto L13060;}
    else {
      arg0K0 = 0;
      goto L13060;}}}
 L13060: {
  new_length_1173X = arg0K0;
  if ((0 == new_length_1173X)) {
    extend_decode_vector0_return_value = 0;
    goto extend_decode_vector_return;}
  else {
    len_1174X = ((new_length_1173X)<<2);
    addr_1175X = ALLOCATE_SPACE(2, (4 + len_1174X));
    *((long *) addr_1175X) = (10 + (((len_1174X)<<8)));
    new_vector_1176X = 3 + (((long) (addr_1175X + 4)));
    p_1177X = *((long *) (((char *) (-3 + decode_vector_1166X))));
    new_length_1178X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + new_vector_1176X))) + -4))))>>8))))>>2);
    old_length_1179X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + decode_vector_1166X))) + -4))))>>8))))>>2);
    next_uid_1180X = ((p_1177X)>>2);
    arg0K0 = 0;
    goto L11691;}}
 L11691: {
  i_1181X = arg0K0;
  if ((i_1181X == old_length_1179X)) {
    start_1182X = Snew_id_hotelS;
    arg1K0 = start_1182X;
    arg0K1 = (-1 + next_uid_1180X);
    goto L11770;}
  else {
    value_1183X = *((long *) ((((char *) (-3 + decode_vector_1166X))) + (((i_1181X)<<2))));
    addr_1184X = (((char *) (-3 + new_vector_1176X))) + (((i_1181X)<<2));
    WRITE_BARRIER(addr_1184X, value_1183X);
    *((long *) addr_1184X) = value_1183X;
    arg0K0 = (1 + i_1181X);
    goto L11691;}}
 L11770: {
  ptr_1185X = arg1K0;
  data_1186X = arg0K1;
  if ((ptr_1185X == NULL)) {
    arg0K0 = next_uid_1180X;
    goto L11734;}
  else {
    next_1187X = *((long *) ptr_1185X);
    if ((3 == (3 & next_1187X))) {
      arg2K0 = (15 == (31 & ((((*((long *) ((((char *) (-3 + next_1187X))) + -4))))>>2))));
      goto L11714;}
    else {
      arg2K0 = 0;
      goto L11714;}}}
 L11734: {
  i_1188X = arg0K0;
  if ((i_1188X == new_length_1178X)) {
    if (proxiesP_49X) {
      addr_1189X = (((char *) (-3 + address_space_48X))) + 8;
      WRITE_BARRIER(addr_1189X, new_vector_1176X);
      *((long *) addr_1189X) = new_vector_1176X;
      extend_decode_vector0_return_value = 1;
      goto extend_decode_vector_return;}
    else {
      addr_1190X = (((char *) (-3 + address_space_48X))) + 4;
      WRITE_BARRIER(addr_1190X, new_vector_1176X);
      *((long *) addr_1190X) = new_vector_1176X;
      extend_decode_vector0_return_value = 1;
      goto extend_decode_vector_return;}}
  else {
    value_1191X = 4 + (((i_1188X)<<2));
    addr_1192X = (((char *) (-3 + new_vector_1176X))) + (((i_1188X)<<2));
    WRITE_BARRIER(addr_1192X, value_1191X);
    *((long *) addr_1192X) = value_1191X;
    arg0K0 = (1 + i_1188X);
    goto L11734;}}
 L11714: {
  is_proxyP_1193X = arg2K0;
  if (proxiesP_49X) {
    if (is_proxyP_1193X) {
      goto L11725;}
    else {
      arg0K0 = data_1186X;
      goto L11773;}}
  else {
    if (is_proxyP_1193X) {
      arg0K0 = data_1186X;
      goto L11773;}
    else {
      goto L11725;}}}
 L11725: {
  addr_1194X = (((char *) (-3 + new_vector_1176X))) + (((data_1186X)<<2));
  WRITE_BARRIER(addr_1194X, next_1187X);
  *((long *) addr_1194X) = next_1187X;
  arg0K0 = (-1 + data_1186X);
  goto L11773;}
 L11773: {
  v_1195X = arg0K0;
  arg1K0 = (((char *) (*((long *) (ptr_1185X + 4)))));
  arg0K1 = v_1195X;
  goto L11770;}
 extend_decode_vector_return:
  switch (extend_decode_vector_return_tag) {
  case 0: goto extend_decode_vector_return_0;
  case 1: goto extend_decode_vector_return_1;
  default: goto extend_decode_vector_return_2;
  }}

 encode: {
  thing_45X = merged_arg0K0;
  address_space_46X = merged_arg0K1;
  pair_47X = merged_arg0K2;{
  start_hp_1196X = heap_limits(&v_1197X, &current_end_1198X, &other_begin_1199X, &v_1200X);
  Sour_address_spaceS = address_space_46X;
  SstartS = start_hp_1196X;
  Stransmit_hpS = (start_hp_1196X + 12);
  end_1201X = start_hp_1196X + 16777220;
  if ((end_1201X < current_end_1198X)) {
    arg1K0 = end_1201X;
    goto L15563;}
  else {
    arg1K0 = current_end_1198X;
    goto L15563;}}
 L15563: {
  x_1202X = arg1K0;
  Smax_hpS = (x_1202X + -4);
  Sother_hpS = other_begin_1199X;
  Sheartbreak_hotelS = NULL;
  Snew_id_hotelS = NULL;
  Slosing_proxy_hotelS = NULL;
  merged_arg0K0 = thing_45X;
  encode_object_return_tag = 0;
  goto encode_object;
 encode_object_return_0:
  code_1203X = encode_object0_return_value;
  *((long *) (start_hp_1196X + 8)) = code_1203X;
  if (((Stransmit_hpS) < (Smax_hpS))) {
    arg1K0 = (start_hp_1196X + 12);
    goto L15757;}
  else {
    goto L15607;}}
 L15757: {
  start_1204X = arg1K0;
  end_1205X = Stransmit_hpS;
  arg1K0 = start_1204X;
  goto L14994;}
 L15607: {
  start_1206X = Sheartbreak_hotelS;
  arg1K0 = start_1206X;
  goto L15682;}
 L14994: {
  addr_1207X = arg1K0;
  if ((addr_1207X < end_1205X)) {
    thing_1208X = *((long *) addr_1207X);
    next_1209X = addr_1207X + 4;
    if ((2 == (3 & thing_1208X))) {
      if (((31 & (((thing_1208X)>>2))) < 19)) {
        goto L15009;}
      else {
        arg1K0 = (next_1209X + (-4 & (3 + ((long)(((unsigned long)thing_1208X)>>8)))));
        goto L14994;}}
    else {
      goto L15009;}}
  else {
    if (((Stransmit_hpS) < (Smax_hpS))) {
      if ((end_1205X < (Stransmit_hpS))) {
        arg1K0 = end_1205X;
        goto L15757;}
      else {
        *((long *) start_hp_1196X) = (82 + (((((Stransmit_hpS) - (start_hp_1196X + 4)))<<8)));
        *((long *) (start_hp_1196X + 4)) = ((Stransmit_hpS) - (start_hp_1196X + 4));
        set_heap_pointerB((Stransmit_hpS));
        start_1210X = Sheartbreak_hotelS;
        arg1K0 = start_1210X;
        goto L15716;}}
    else {
      goto L15607;}}}
 L15682: {
  ptr_1211X = arg1K0;
  if ((ptr_1211X == NULL)) {
    goto L5572;}
  else {
    *((long *) ((((char *) (-3 + (*((long *) ptr_1211X))))) + -4)) = (*((long *) (ptr_1211X + 4)));
    arg1K0 = (((char *) (*((long *) (ptr_1211X + 8)))));
    goto L15682;}}
 L15009: {
  if ((3 == (3 & thing_1208X))) {
    merged_arg0K0 = thing_1208X;
    encode_object_return_tag = 1;
    goto encode_object;
   encode_object_return_1:
    x2_1212X = encode_object0_return_value;
    *((long *) addr_1207X) = x2_1212X;
    arg1K0 = next_1209X;
    goto L14994;}
  else {
    arg1K0 = next_1209X;
    goto L14994;}}
 L15716: {
  ptr_1213X = arg1K0;
  if ((ptr_1213X == NULL)) {
    old_proxy_1214X = *((long *) ((((char *) (-3 + address_space_46X))) + 8));
    old_decode_1215X = *((long *) ((((char *) (-3 + address_space_46X))) + 4));
    p_1216X = *((long *) (((char *) (-3 + old_proxy_1214X))));
    y_1217X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + old_proxy_1214X))) + -4))))>>8))))>>2);
    if ((y_1217X < (((p_1216X)>>2)))) {
      arg2K0 = 0;
      goto L14044;}
    else {
      arg2K0 = 1;
      goto L14044;}}
  else {
    *((long *) ((((char *) (-3 + (*((long *) ptr_1213X))))) + -4)) = (*((long *) (ptr_1213X + 4)));
    arg1K0 = (((char *) (*((long *) (ptr_1213X + 8)))));
    goto L15716;}}
 L5572: {
  start_1218X = Snew_id_hotelS;
  arg1K0 = start_1218X;
  goto L5615;}
 L14044: {
  proxy_okayP_1219X = arg2K0;
  if ((((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + old_decode_1215X))) + -4))))>>8))))>>2)) < ((((*((long *) (((char *) (-3 + old_decode_1215X))))))>>2)))) {
    merged_arg0K0 = address_space_46X;
    merged_arg1K1 = other_begin_1199X;
    merged_arg2K2 = 0;
    extend_decode_vector_return_tag = 0;
    goto extend_decode_vector;
   extend_decode_vector_return_0:
    v_1220X = extend_decode_vector0_return_value;
    if (v_1220X) {
      if (proxy_okayP_1219X) {
        goto L15629;}
      else {
        merged_arg0K0 = address_space_46X;
        merged_arg1K1 = other_begin_1199X;
        merged_arg2K2 = 1;
        extend_decode_vector_return_tag = 1;
        goto extend_decode_vector;
       extend_decode_vector_return_1:
        temp_1221X = extend_decode_vector0_return_value;
        if (temp_1221X) {
          goto L15629;}
        else {
          addr_1222X = (((char *) (-3 + address_space_46X))) + 8;
          WRITE_BARRIER(addr_1222X, old_proxy_1214X);
          *((long *) addr_1222X) = old_proxy_1214X;
          goto L5572;}}}
    else {
      goto L5572;}}
  else {
    if (proxy_okayP_1219X) {
      goto L15629;}
    else {
      merged_arg0K0 = address_space_46X;
      merged_arg1K1 = other_begin_1199X;
      merged_arg2K2 = 1;
      extend_decode_vector_return_tag = 2;
      goto extend_decode_vector;
     extend_decode_vector_return_2:
      v_1223X = extend_decode_vector0_return_value;
      if (v_1223X) {
        goto L15629;}
      else {
        goto L5572;}}}}
 L5615: {
  ptr_1224X = arg1K0;
  if ((ptr_1224X == NULL)) {
    encode0_return_value = 0;
    goto encode_return;}
  else {
    thing_1225X = *((long *) ptr_1224X);
    temp_1226X = 31 & ((((*((long *) ((((char *) (-3 + thing_1225X))) + -4))))>>2));
    if ((1 == temp_1226X)) {
      addr_1227X = ((char *) (-3 + thing_1225X));
      WRITE_BARRIER(addr_1227X, 1);
      *((long *) addr_1227X) = 1;
      goto L5620;}
    else {
      if ((11 == temp_1226X)) {
        addr_1228X = (((char *) (-3 + thing_1225X))) + 8;
        WRITE_BARRIER(addr_1228X, 1);
        *((long *) addr_1228X) = 1;
        goto L5620;}
      else {
        if ((13 == temp_1226X)) {
          addr_1229X = (((char *) (-3 + thing_1225X))) + 8;
          WRITE_BARRIER(addr_1229X, 1);
          *((long *) addr_1229X) = 1;
          goto L5620;}
        else {
          if ((16 == temp_1226X)) {
            addr_1230X = ((char *) (-3 + thing_1225X));
            WRITE_BARRIER(addr_1230X, 1);
            *((long *) addr_1230X) = 1;
            goto L5620;}
          else {
            if ((16 == temp_1226X)) {
              addr_1231X = ((char *) (-3 + thing_1225X));
              WRITE_BARRIER(addr_1231X, 1);
              *((long *) addr_1231X) = 1;
              goto L5620;}
            else {
              if ((4 == temp_1226X)) {
                addr_1232X = ((char *) (-3 + thing_1225X));
                WRITE_BARRIER(addr_1232X, 1);
                *((long *) addr_1232X) = 1;
                goto L5620;}
              else {
                if ((14 == temp_1226X)) {
                  addr_1233X = ((char *) (-3 + (*((long *) (((char *) (-3 + thing_1225X)))))));
                  WRITE_BARRIER(addr_1233X, 1);
                  *((long *) addr_1233X) = 1;
                  addr_1234X = (((char *) (-3 + (*((long *) (((char *) (-3 + thing_1225X)))))))) + 12;
                  WRITE_BARRIER(addr_1234X, 67108864);
                  *((long *) addr_1234X) = 67108864;
                  goto L5620;}
                else {
                  goto L5620;}}}}}}}}}
 L15629: {
  start_1235X = Slosing_proxy_hotelS;
  arg1K0 = start_1235X;
  arg0K1 = 25;
  goto L10524;}
 L5620: {
  arg1K0 = (((char *) (*((long *) (ptr_1224X + 4)))));
  goto L5615;}
 L10524: {
  ptr_1236X = arg1K0;
  data_1237X = arg0K1;
  if ((ptr_1236X == NULL)) {
    if ((1 == data_1237X)) {
      encode0_return_value = 0;
      goto encode_return;}
    else {
      val_1238X = 3 + (((long) (start_hp_1196X + 4)));
      addr_1239X = ((char *) (-3 + pair_47X));
      WRITE_BARRIER(addr_1239X, val_1238X);
      *((long *) addr_1239X) = val_1238X;
      addr_1240X = (((char *) (-3 + pair_47X))) + 4;
      WRITE_BARRIER(addr_1240X, data_1237X);
      *((long *) addr_1240X) = data_1237X;
      encode0_return_value = 1;
      goto encode_return;}}
  else {
    v_1241X = AVAILABLEp(3);
    if (v_1241X) {
      a_1242X = *((long *) ptr_1236X);
      addr_1243X = ALLOCATE_SPACE(0, 12);
      *((long *) addr_1243X) = 2050;
      x_1244X = 3 + (((long) (addr_1243X + 4)));
      *((long *) (((char *) (-3 + x_1244X)))) = a_1242X;
      *((long *) ((((char *) (-3 + x_1244X))) + 4)) = data_1237X;
      arg0K0 = x_1244X;
      goto L10527;}
    else {
      arg0K0 = 1;
      goto L10527;}}}
 L10527: {
  v_1245X = arg0K0;
  arg1K0 = (((char *) (*((long *) (ptr_1236X + 4)))));
  arg0K1 = v_1245X;
  goto L10524;}
 encode_return:
  switch (encode_return_tag) {
  case 0: goto encode_return_0;
  default: goto encode_return_1;
  }}

 copy_continuation_from_heapB: {
  cont_44X = merged_arg0K0;{
  top_1246X = (((char *) (-3 + (Sbottom_of_stackS)))) + (-8 - (-4 & (3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_44X))) + -4))))>>8)))));
  new_cont_1247X = 3 + (((long) (top_1246X + 4)));
  SstackS = (top_1246X + -4);
  ScontS = new_cont_1247X;
  v_1248X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + cont_44X))) + -4))))>>8))))>>2);
  memcpy((void *)top_1246X, (void *)((((char *) (-3 + cont_44X))) + -4),(4 + (((v_1248X)<<2))));
  *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = (*((long *) (((char *) (-3 + new_cont_1247X)))));
  *((long *) (((char *) (-3 + new_cont_1247X)))) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_1247X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  default: goto copy_continuation_from_heapB_return_1;
  }}

 get_current_port: {
  marker_43X = merged_arg0K0;{
  thread_1249X = Scurrent_threadS;
  if ((3 == (3 & thread_1249X))) {
    if ((8 == (31 & ((((*((long *) ((((char *) (-3 + thread_1249X))) + -4))))>>2))))) {
      if ((1 < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + thread_1249X))) + -4))))>>8))))>>2)))) {
        arg0K0 = (*((long *) ((((char *) (-3 + thread_1249X))) + 4)));
        goto L11155;}
      else {
        v_1250X = enter_string("bad record");
        get_current_port0_return_value = v_1250X;
        goto get_current_port_return;}}
    else {
      v_1251X = enter_string("bad record");
      get_current_port0_return_value = v_1251X;
      goto get_current_port_return;}}
  else {
    v_1252X = enter_string("bad record");
    get_current_port0_return_value = v_1252X;
    goto get_current_port_return;}}
 L11155: {
  env_1253X = arg0K0;
  if ((3 == (3 & env_1253X))) {
    if ((0 == (31 & ((((*((long *) ((((char *) (-3 + env_1253X))) + -4))))>>2))))) {
      obj_1254X = *((long *) (((char *) (-3 + env_1253X))));
      if ((3 == (3 & obj_1254X))) {
        if ((0 == (31 & ((((*((long *) ((((char *) (-3 + obj_1254X))) + -4))))>>2))))) {
          if ((marker_43X == (*((long *) (((char *) (-3 + (*((long *) (((char *) (-3 + env_1253X)))))))))))) {
            get_current_port0_return_value = (*((long *) ((((char *) (-3 + (*((long *) (((char *) (-3 + env_1253X)))))))) + 4)));
            goto get_current_port_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + env_1253X))) + 4)));
            goto L11155;}}
        else {
          goto L11211;}}
      else {
        goto L11211;}}
    else {
      goto L11211;}}
  else {
    goto L11211;}}
 L11211: {
  if ((25 == env_1253X)) {
    v_1255X = enter_string("null");
    get_current_port0_return_value = v_1255X;
    goto get_current_port_return;}
  else {
    v_1256X = enter_string("not pair");
    get_current_port0_return_value = v_1256X;
    goto get_current_port_return;}}
 get_current_port_return:
  switch (get_current_port_return_tag) {
  case 0: goto get_current_port_return_0;
  case 1: goto get_current_port_return_1;
  default: goto get_current_port_return_2;
  }}

 okay_argument_list: {
  list_42X = merged_arg0K0;{
  arg0K0 = list_42X;
  arg0K1 = 0;
  arg0K2 = list_42X;
  arg2K3 = 0;
  goto L10176;}
 L10176: {
  fast_1257X = arg0K0;
  len_1258X = arg0K1;
  slow_1259X = arg0K2;
  move_slowP_1260X = arg2K3;
  if ((25 == fast_1257X)) {
    okay_argument_list0_return_value = 1;
    okay_argument_list1_return_value = len_1258X;
    goto okay_argument_list_return;}
  else {
    if ((3 == (3 & fast_1257X))) {
      if ((0 == (31 & ((((*((long *) ((((char *) (-3 + fast_1257X))) + -4))))>>2))))) {
        if (move_slowP_1260X) {
          if ((fast_1257X == slow_1259X)) {
            okay_argument_list0_return_value = 0;
            okay_argument_list1_return_value = 0;
            goto okay_argument_list_return;}
          else {
            arg0K0 = (*((long *) ((((char *) (-3 + fast_1257X))) + 4)));
            arg0K1 = (1 + len_1258X);
            arg0K2 = (*((long *) ((((char *) (-3 + slow_1259X))) + 4)));
            arg2K3 = 0;
            goto L10176;}}
        else {
          arg0K0 = (*((long *) ((((char *) (-3 + fast_1257X))) + 4)));
          arg0K1 = (1 + len_1258X);
          arg0K2 = slow_1259X;
          arg2K3 = 1;
          goto L10176;}}
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
  exception_40X = merged_arg0K0;
  instruction_size_41X = merged_arg0K1;{
  opcode_1261X = *((unsigned char *) (Scode_pointerS));
  *((long *) (SstackS)) = (((instruction_size_41X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_40X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (StemplateS);
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((((Scode_pointerS) - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2));
  SstackS = ((SstackS) + -4);
  tem_1262X = Sexception_templateS;
  StemplateS = tem_1262X;
  Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1262X))))))));
  arg1K0 = ((SstackS) + 4);
  arg0K1 = 0;
  goto L9759;}
 L9759: {
  p_1263X = arg1K0;
  i_1264X = arg0K1;
  if ((2 == (3 & (*((long *) p_1263X))))) {
    if ((54 == (*((long *) p_1263X)))) {
      arg0K0 = (-1 + i_1264X);
      goto L9700;}
    else {
      arg0K0 = i_1264X;
      goto L9700;}}
  else {
    arg1K0 = (p_1263X + 4);
    arg0K1 = (1 + i_1264X);
    goto L9759;}}
 L9700: {
  v_1265X = arg0K0;
  merged_arg1K0 = (Scode_pointerS);
  merged_arg0K1 = v_1265X;
  push_continuationB_return_tag = 3;
  goto push_continuationB;
 push_continuationB_return_3:
  *((long *) (SstackS)) = (((opcode_1261X)<<2));
  SstackS = ((SstackS) + -4);
  *((long *) (SstackS)) = (((exception_40X)<<2));
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
  case 201: goto push_exception_continuationB_return_201;
  case 202: goto push_exception_continuationB_return_202;
  case 203: goto push_exception_continuationB_return_203;
  case 204: goto push_exception_continuationB_return_204;
  case 205: goto push_exception_continuationB_return_205;
  case 206: goto push_exception_continuationB_return_206;
  case 207: goto push_exception_continuationB_return_207;
  case 208: goto push_exception_continuationB_return_208;
  case 209: goto push_exception_continuationB_return_209;
  default: goto push_exception_continuationB_return_210;
  }}

 push_continuationB: {
  code_pointer_38X = merged_arg1K0;
  size_39X = merged_arg0K1;{
  pc_1266X = (((code_pointer_38X - (((char *) (-3 + (*((long *) (((char *) (-3 + (StemplateS)))))))))))<<2);
  template_1267X = StemplateS;
  SstackS = ((SstackS) + -20);
  *((long *) ((SstackS) + 4)) = (4134 + (((size_39X)<<10)));
  cont_1268X = 3 + (((long) ((SstackS) + 8)));
  *((long *) ((((char *) (-3 + cont_1268X))) + 4)) = pc_1266X;
  *((long *) ((((char *) (-3 + cont_1268X))) + 8)) = template_1267X;
  *((long *) ((((char *) (-3 + cont_1268X))) + 12)) = (SenvS);
  *((long *) (((char *) (-3 + cont_1268X)))) = (ScontS);
  ScontS = cont_1268X;
  goto push_continuationB_return;}
 push_continuationB_return:
  switch (push_continuationB_return_tag) {
  case 0: goto push_continuationB_return_0;
  case 1: goto push_continuationB_return_1;
  case 2: goto push_continuationB_return_2;
  default: goto push_continuationB_return_3;
  }}

 get_error_string: {
  status_37X = merged_arg0K0;{
  string_1269X = ps_error_string(status_37X);
  x_1270X = strlen((char *) string_1269X);
  if ((x_1270X < 256)) {
    arg0K0 = x_1270X;
    goto L9616;}
  else {
    arg0K0 = 256;
    goto L9616;}}
 L9616: {
  len_1271X = arg0K0;
  len_1272X = 1 + len_1271X;
  addr_1273X = ALLOCATE_SPACE(19, (4 + len_1272X));
  *((long *) addr_1273X) = (78 + (((len_1272X)<<8)));
  string_1274X = 3 + (((long) (addr_1273X + 4)));
  *((unsigned char *) ((((char *) (-3 + string_1274X))) + len_1271X)) = 0;
  arg0K0 = 0;
  goto L9626;}
 L9626: {
  i_1275X = arg0K0;
  if ((i_1275X == len_1271X)) {
    get_error_string0_return_value = string_1274X;
    goto get_error_string_return;}
  else {
    *((unsigned char *) ((((char *) (-3 + string_1274X))) + i_1275X)) = ((*(string_1269X + i_1275X)));
    arg0K0 = (1 + i_1275X);
    goto L9626;}}
 get_error_string_return:
  switch (get_error_string_return_tag) {
  case 0: goto get_error_string_return_0;
  case 1: goto get_error_string_return_1;
  case 2: goto get_error_string_return_2;
  case 3: goto get_error_string_return_3;
  default: goto get_error_string_return_4;
  }}

 pop_continuationB: {
{ cont_1276X = ScontS;
  tem_1277X = *((long *) ((((char *) (-3 + cont_1276X))) + 8));
  pc_1278X = *((long *) ((((char *) (-3 + cont_1276X))) + 4));
  StemplateS = tem_1277X;
  Scode_pointerS = ((((char *) (-3 + (*((long *) (((char *) (-3 + tem_1277X)))))))) + (((pc_1278X)>>2)));
  SenvS = (*((long *) ((((char *) (-3 + cont_1276X))) + 12)));
  ScontS = (*((long *) (((char *) (-3 + cont_1276X)))));
  SstackS = ((((char *) (-3 + cont_1276X))) + 12);
  goto pop_continuationB_return;}
 pop_continuationB_return:
  switch (pop_continuationB_return_tag) {
  case 0: goto pop_continuationB_return_0;
  case 1: goto pop_continuationB_return_1;
  default: goto pop_continuationB_return_2;
  }}

}
long call_startup_procedure(long startup_proc_1279X, char **startup_vector_1280X, long startup_vector_length_1281X)
{
  long arg0K0;
  long code_1314X;
  char * addr_1313X;
  long x_1312X;
  char * addr_1311X;
  long b_1310X;
  long channel_1309X;
  long x_1308X;
  char * addr_1307X;
  long b_1306X;
  long channel_1305X;
  long x_1304X;
  char * addr_1303X;
  long b_1302X;
  long channel_1301X;
  long i_1300X;
  long length_1299X;
  long *v_1298X;
  long v_1297X;
  long v_1296X;
  long y_1295X;
  long x_1294X;
  long v_1293X;
  long x_1292X;
  long y_1291X;
  long x_1290X;
  long y_1289X;
  long tem_1288X;
  char * addr_1287X;
  long value_1286X;
  long i_1285X;
  long vector_1284X;
  char * addr_1283X;
  long len_1282X;
 {  len_1282X = ((startup_vector_length_1281X)<<2);
  addr_1283X = ALLOCATE_SPACE(2, (4 + len_1282X));
  *((long *) addr_1283X) = (10 + (((len_1282X)<<8)));
  vector_1284X = 3 + (((long) (addr_1283X + 4)));
  arg0K0 = 0;
  goto L21178;}
 L21178: {
  i_1285X = arg0K0;
  if ((i_1285X < startup_vector_length_1281X)) {
    value_1286X = enter_string((*(startup_vector_1280X + i_1285X)));
    addr_1287X = (((char *) (-3 + vector_1284X))) + (((i_1285X)<<2));
    WRITE_BARRIER(addr_1287X, value_1286X);
    *((long *) addr_1287X) = value_1286X;
    arg0K0 = (1 + i_1285X);
    goto L21178;}
  else {
    SstackS = ((((char *) (-3 + (Sbottom_of_stackS)))) + -8);
    *((long *) (((char *) (-3 + (Sbottom_of_stackS))))) = 1;
    ScontS = (Sbottom_of_stackS);
    SenvS = 13;
    tem_1288X = Sinterrupt_templateS;
    StemplateS = tem_1288X;
    Scode_pointerS = (((char *) (-3 + (*((long *) (((char *) (-3 + tem_1288X))))))));
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
    *((long *) (SstackS)) = vector_1284X;
    SstackS = ((SstackS) + -4);
    y_1289X = fileno((stderr));
    x_1290X = fileno((stdout));
    if ((x_1290X < y_1289X)) {
      arg0K0 = y_1289X;
      goto L12418;}
    else {
      arg0K0 = x_1290X;
      goto L12418;}}}
 L12418: {
  y_1291X = arg0K0;
  x_1292X = fileno((stdin));
  if ((x_1292X < y_1291X)) {
    arg0K0 = y_1291X;
    goto L12420;}
  else {
    arg0K0 = x_1292X;
    goto L12420;}}
 L12420: {
  v_1293X = arg0K0;
  x_1294X = Snumber_of_channelsS;
  y_1295X = 1 + v_1293X;
  if ((x_1294X < y_1295X)) {
    arg0K0 = y_1295X;
    goto L12422;}
  else {
    arg0K0 = x_1294X;
    goto L12422;}}
 L12422: {
  v_1296X = arg0K0;
  Snumber_of_channelsS = v_1296X;
  v_1297X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error("out of memory, unable to continue", 0);
    goto L12444;}
  else {
    goto L12444;}}
 L12444: {
  v_1298X = Svm_channelsS;
  length_1299X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L12486;}
 L12486: {
  i_1300X = arg0K0;
  if ((i_1300X < length_1299X)) {
    *(v_1298X + i_1300X) = 1;
    arg0K0 = (1 + i_1300X);
    goto L12486;}
  else {
    channel_1301X = fileno((stdin));
    b_1302X = enter_string("standard input");
    addr_1303X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_1303X) = 5142;
    x_1304X = 3 + (((long) (addr_1303X + 4)));
    *((long *) (((char *) (-3 + x_1304X)))) = 4;
    *((long *) ((((char *) (-3 + x_1304X))) + 4)) = b_1302X;
    *((long *) ((((char *) (-3 + x_1304X))) + 8)) = (((channel_1301X)<<2));
    *((long *) ((((char *) (-3 + x_1304X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1304X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1301X) = x_1304X;
    channel_1305X = fileno((stderr));
    b_1306X = enter_string("standard error");
    addr_1307X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_1307X) = 5142;
    x_1308X = 3 + (((long) (addr_1307X + 4)));
    *((long *) (((char *) (-3 + x_1308X)))) = 8;
    *((long *) ((((char *) (-3 + x_1308X))) + 4)) = b_1306X;
    *((long *) ((((char *) (-3 + x_1308X))) + 8)) = (((channel_1305X)<<2));
    *((long *) ((((char *) (-3 + x_1308X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1308X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1305X) = x_1308X;
    channel_1309X = fileno((stdout));
    b_1310X = enter_string("standard output");
    addr_1311X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_1311X) = 5142;
    x_1312X = 3 + (((long) (addr_1311X + 4)));
    *((long *) (((char *) (-3 + x_1312X)))) = 8;
    *((long *) ((((char *) (-3 + x_1312X))) + 4)) = b_1310X;
    *((long *) ((((char *) (-3 + x_1312X))) + 8)) = (((channel_1309X)<<2));
    *((long *) ((((char *) (-3 + x_1312X))) + 12)) = 1;
    *((long *) ((((char *) (-3 + x_1312X))) + 16)) = 1;
    *((Svm_channelsS) + channel_1309X) = x_1312X;
    *((long *) (SstackS)) = x_1304X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1312X;
    SstackS = ((SstackS) + -4);
    *((long *) (SstackS)) = x_1308X;
    SstackS = ((SstackS) + -4);
    addr_1313X = ALLOCATE_SPACE(20, 6);
    *((long *) addr_1313X) = 594;
    code_1314X = 3 + (((long) (addr_1313X + 4)));
    *((unsigned char *) (((char *) (-3 + code_1314X)))) = 24;
    *((unsigned char *) ((((char *) (-3 + code_1314X))) + 1)) = 4;
    Scode_pointerS = (((char *) (-3 + code_1314X)));
    return restart(startup_proc_1279X);}}
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
