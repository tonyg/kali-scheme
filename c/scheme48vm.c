#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"

void mark_traced_channels_closedB(void);
void set_extension_valueB(long);
void note_event(void);
long required_init_space(unsigned char**, long);
void initialize_vm(long, long);
long restart(long);
long call_startup_procedure(long, unsigned char**, unsigned char);
char Spending_eventsPS;
long Sextension_valueS;
long Sinterrupted_templateS;
long Sinterrupt_templateS;
char Spending_interruptPS;
long Spending_interruptsS;
long Sfinalize_theseS;
long Sfinalizer_alistS;
long Ssession_dataS;
long Scurrent_threadS;
long Sinterrupt_handlersS;
long Sexception_handlersS;
long Senabled_interruptsS;
long SvalS;
long SnargsS;
long Scode_pointerS;
long StemplateS;
long SenvS;
char Sexception_space_usedPS;
long ScontS;
long Sbottom_of_stackS;
long Sstack_limitS;
long SstackS;
long Sstack_endS;
long Sstack_beginS;
long Spending_channels_tailS;
long Spending_channels_headS;
long *Svm_channelsS;
long Snumber_of_channelsS;

void mark_traced_channels_closedB(void)
{
  long arg0K0;
  long v_0X;
  long addr_1X;
  long addr_2X;
  long i_3X;
  long descriptor_4X;
  long channel_5X;
  long header_6X;
 {arg0K0 = 0;
  goto L5763;}
 L5763: {
  i_3X = arg0K0;
  if ((i_3X == (Snumber_of_channelsS))) {
    return;}
  else {
    channel_5X = *((Svm_channelsS) + i_3X);
    if ((1 == channel_5X)) {
      goto L5823;}
    else {
      header_6X = *((long *) (-7 + channel_5X));
      if ((3 == (3 & header_6X))) {
        ps_write_string(((unsigned char *) "Channel closed in dumped image: "), (stderr));
        descriptor_4X = *((long *) (1 + channel_5X));
        if ((0 == (3 & descriptor_4X))) {
          ps_write_integer(((((*((long *) (1 + channel_5X))))>>2)), (stderr));
          goto L5809;}
        else {
          ps_write_string((((unsigned char *)(-3 + (*((long *) (1 + channel_5X)))))), (stderr));
          goto L5809;}}
      else {
        goto L5823;}}}}
 L5823: {
  arg0K0 = (1 + i_3X);
  goto L5763;}
 L5809: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  addr_1X = -3 + header_6X;
  WRITE_BARRIER(addr_1X, 0);
  *((long *) addr_1X) = 0;
  addr_2X = 5 + header_6X;
  WRITE_BARRIER(addr_2X, -4);
  *((long *) addr_2X) = -4;
  goto L5823;}}

void set_extension_valueB(long value_7X)
{

 {Sextension_valueS = value_7X;
  return;}}

void note_event(void)
{

 {Spending_eventsPS = 1;
  Spending_interruptPS = 1;
  return;}}

long required_init_space(unsigned char **startup_vector_8X, long startup_vector_length_9X)
{
  long arg0K1;
  long arg0K0;
  long i_10X;
  long s_11X;
 {arg0K0 = 0;
  arg0K1 = 0;
  goto L3890;}
 L3890: {
  i_10X = arg0K0;
  s_11X = arg0K1;
  if ((i_10X < startup_vector_length_9X)) {
    arg0K0 = (1 + i_10X);
    arg0K1 = (1 + (s_11X + ((((4 + (strlen((char *) (*(startup_vector_8X + i_10X))))))>>2))));
    goto L3890;}
  else {
    return (45 + s_11X);}}}

void initialize_vm(long stack_begin_12X, long stack_size_13X)
{
  long addr_14X;
  long addr_15X;
  long temp_16X;
  long code_17X;
  long addr_18X;
  long cont_19X;
  long addr_20X;
  long addr_21X;
  long temp_22X;
  long code_23X;
  long addr_24X;
 {Sstack_beginS = stack_begin_12X;
  Sstack_endS = (stack_begin_12X + (((stack_size_13X)<<2)));
  Sstack_limitS = (Sstack_beginS);
  SstackS = (-4 + (Sstack_endS));
  ScontS = 1;
  SenvS = 17;
  addr_14X = ALLOCATE_SPACE(17, 6);
  *((long *) addr_14X) = 582;
  addr_15X = ALLOCATE_SPACE(11, 12);
  *((long *) addr_15X) = 2094;
  temp_16X = 7 + addr_15X;
  code_17X = 7 + addr_14X;
  addr_18X = -3 + temp_16X;
  WRITE_BARRIER(addr_18X, code_17X);
  *((long *) addr_18X) = code_17X;
  *((unsigned char *) (-3 + code_17X)) = 24;
  *((unsigned char *) (-2 + code_17X)) = 30;
  SstackS = (-20 + (SstackS));
  *((long *) (4 + (SstackS))) = 4262;
  cont_19X = 11 + (SstackS);
  *((long *) (1 + cont_19X)) = 0;
  *((long *) (5 + cont_19X)) = temp_16X;
  *((long *) (9 + cont_19X)) = (SenvS);
  *((long *) (-3 + cont_19X)) = (ScontS);
  ScontS = cont_19X;
  Sbottom_of_stackS = (ScontS);
  addr_20X = ALLOCATE_SPACE(17, 6);
  *((long *) addr_20X) = 582;
  addr_21X = ALLOCATE_SPACE(11, 12);
  *((long *) addr_21X) = 2094;
  temp_22X = 7 + addr_21X;
  code_23X = 7 + addr_20X;
  addr_24X = -3 + temp_22X;
  WRITE_BARRIER(addr_24X, code_23X);
  *((long *) addr_24X) = code_23X;
  *((unsigned char *) (-3 + code_23X)) = 33;
  *((unsigned char *) (-2 + code_23X)) = 130;
  Sinterrupt_templateS = temp_22X;
  return;}}

long restart(long value_25X)
{
  char arg2K3;
  char arg2K1;
  char arg2K2;
  char arg2K0;
  long arg0K2;
  long arg0K1;
  long arg0K0;
  unsigned char arg1K1;
  unsigned char arg1K0;
  unsigned char *merged_arg3K0;
  unsigned char merged_arg1K3;
  unsigned char merged_arg1K2;
  unsigned char merged_arg1K1;
  unsigned char merged_arg1K0;
  long merged_arg0K2;
  long merged_arg0K1;
  long merged_arg0K0;
  long RSenvS = SenvS;
  long RSstackS = SstackS;
  long RScode_pointerS = Scode_pointerS;
  long RSvalS = SvalS;

  int push_continuationB_return_tag;
  int push_exception_continuationB_return_tag;
  int copy_continuation_from_heapB_return_tag;
  long copy_continuation_from_heapB0_return_value;
  int save_env_in_heap_return_tag;
  long save_env_in_heap0_return_value;
  int really_preserve_continuation_return_tag;
  long really_preserve_continuation0_return_value;
  int copy_stack_into_heap_return_tag;
  int channel_close_error_return_tag;
  long channel_close_error0_return_value;
  int collect_saving_temp_return_tag;
  long collect_saving_temp0_return_value;
  int pop_args_list_return_tag;
  long pop_args_list0_return_value;
  int check_events_return_tag;
  char check_events0_return_value;
  int loseD0_return_tag;
  unsigned char option_26X;
  long v_27X;
  long (*v_28X)(long, long);
  long b_29X;
  long c_30X;
  long a_31X;
  long n_32X;
  long b_33X;
  long c_34X;
  long a_35X;
  long n_36X;
  long len_37X;
  long addr_38X;
  long string_39X;
  long i_40X;
  unsigned char v_41X;
  long length_42X;
  long status_43X;
  char pendingP_44X;
  char eofP_45X;
  long got_46X;
  unsigned char reason_47X;
  long x_48X;
  long obj_49X;
  char waitP_50X;
  long count_51X;
  long start_52X;
  long length_53X;
  long got_54X;
  char pendingP_55X;
  long status_56X;
  unsigned char reason_57X;
  long env_58X;
  long i_59X;
  unsigned char b_60X;
  long i_61X;
  long h_62X;
  long bucket_63X;
  long index_64X;
  long b_65X;
  long addr_66X;
  long x_67X;
  long addr_68X;
  long i_69X;
  long addr_70X;
  long addr_71X;
  long table_72X;
  long symbols_73X;
  long addr_74X;
  long i_75X;
  long n_76X;
  long s_77X;
  long v_78X;
  long obj_79X;
  char v_80X;
  long table_81X;
  long symbols_82X;
  char firstP_83X;
  long table_84X;
  char firstP_85X;
  long symbols_86X;
  long v_87X;
  unsigned char v_88X;
  unsigned char v_89X;
  unsigned char v_90X;
  long r_91X;
  unsigned char v_92X;
  unsigned char v_93X;
  long new_94X;
  long addr_95X;
  long len_96X;
  unsigned char b_97X;
  unsigned char b_98X;
  long i_99X;
  unsigned char b_100X;
  long len_101X;
  long addr_102X;
  long new_103X;
  long i_104X;
  long i_105X;
  long addr_106X;
  char okayP_107X;
  long init_108X;
  long len_109X;
  long addr_110X;
  long val_111X;
  long addr_112X;
  long vector_113X;
  long i_114X;
  unsigned char v_115X;
  long addr_116X;
  long addr_117X;
  long channel_118X;
  unsigned char reason_119X;
  long v_120X;
  long new_count_121X;
  long *new_vm_channels_122X;
  long i_123X;
  long i_124X;
  long addr_125X;
  long x_126X;
  long v_127X;
  long v_128X;
  long v_129X;
  long v_130X;
  long v_131X;
  long status_132X;
  long arg2_133X;
  long mode_134X;
  long channel_135X;
  long status_136X;
  long index_137X;
  char temp_138X;
  long x_139X;
  long y_140X;
  long v_141X;
  long status_142X;
  long addr_143X;
  unsigned char reason_144X;
  long x_145X;
  long arg2_146X;
  long addr_147X;
  long x_148X;
  long b_149X;
  long addr_150X;
  long x_151X;
  char firstP_152X;
  long vector_153X;
  long n_154X;
  char minutesP_155X;
  char temp_156X;
  unsigned char v_157X;
  long i_158X;
  long h_159X;
  long len_160X;
  long addr_161X;
  long string_162X;
  long arg2_163X;
  long n_164X;
  long l_165X;
  long i_166X;
  long val_167X;
  long new_168X;
  long addr_169X;
  long x_170X;
  long addr_171X;
  long i_172X;
  long h_173X;
  long bucket_174X;
  long index_175X;
  long b_176X;
  long len_177X;
  long s2_178X;
  long addr_179X;
  long x_180X;
  long arg2_181X;
  long obj_182X;
  long table_183X;
  long v_184X;
  long n_185X;
  long list_186X;
  long slow_187X;
  char move_slowP_188X;
  long head_189X;
  long list_190X;
  long y_191X;
  long y_192X;
  long list_193X;
  long a_194X;
  long addr_195X;
  long x_196X;
  long l_197X;
  long last_198X;
  long addr_199X;
  long x_200X;
  long addr_201X;
  long a_202X;
  long i_203X;
  long l_204X;
  long space_205X;
  char v_206X;
  char okayP_207X;
  long overflow_208X;
  long fast_209X;
  long len_210X;
  long slow_211X;
  char move_slowP_212X;
  long nargs_213X;
  long args_214X;
  long args_215X;
  long offset_216X;
  unsigned char v_217X;
  unsigned char b_218X;
  long args_219X;
  long env_220X;
  long i_221X;
  unsigned char b_222X;
  long value_223X;
  long addr_224X;
  long v_225X;
  unsigned char b_226X;
  unsigned char b_227X;
  long a_228X;
  long addr_229X;
  long x_230X;
  long env_231X;
  char v_232X;
  char okayP_233X;
  unsigned char key_234X;
  long p_235X;
  long v_236X;
  long space_237X;
  char v_238X;
  char okayP_239X;
  long env_240X;
  long i_241X;
  unsigned char b_242X;
  long value_243X;
  long addr_244X;
  long v_245X;
  long len_246X;
  long addr_247X;
  long new_env_248X;
  long i_249X;
  long env_250X;
  unsigned char b_251X;
  unsigned char b_252X;
  unsigned char b_253X;
  long a_254X;
  long addr_255X;
  long x_256X;
  unsigned char v_257X;
  long key_258X;
  long tem_259X;
  long n_260X;
  long b_261X;
  long c_262X;
  long a_263X;
  long n_264X;
  long b_265X;
  long c_266X;
  long a_267X;
  long q_268X;
  long a_269X;
  long val_270X;
  long val_271X;
  long addr_272X;
  long addr_273X;
  long ch_274X;
  long prev_275X;
  long y_276X;
  long n_277X;
  long b_278X;
  long lo_a_279X;
  long lo_b_280X;
  long hi_a_281X;
  long hi_b_282X;
  long lo_c_283X;
  long v_284X;
  long v_285X;
  long mid_c_286X;
  long c_287X;
  long a_288X;
  char firstP_289X;
  long type_290X;
  long vector_291X;
  long v_292X;
  unsigned char v_293X;
  unsigned char v_294X;
  unsigned char v_295X;
  unsigned char v_296X;
  unsigned char v_297X;
  long loc_298X;
  long arg_299X;
  long v_300X;
  unsigned char key_301X;
  long v_302X;
  unsigned char b_303X;
  long len_304X;
  long addr_305X;
  long stob_306X;
  long x_307X;
  long value_308X;
  long addr_309X;
  unsigned char b_310X;
  unsigned char b_311X;
  char okayP_312X;
  char v_313X;
  long space_314X;
  long v_315X;
  unsigned char b_316X;
  unsigned char b_317X;
  long env_318X;
  unsigned char b_319X;
  unsigned char b_320X;
  long env_321X;
  unsigned char b_322X;
  long env_323X;
  unsigned char b_324X;
  long env_325X;
  char okayP_326X;
  char v_327X;
  long space_328X;
  unsigned char b_329X;
  unsigned char b_330X;
  char okayP_331X;
  char v_332X;
  long space_333X;
  unsigned char b_334X;
  unsigned char b_335X;
  unsigned char b_336X;
  long cont_337X;
  long stob_338X;
  long top_of_args_339X;
  long nargs_340X;
  long cont_341X;
  long pc_342X;
  long tem_343X;
  long cont_344X;
  long tem_345X;
  unsigned char b_346X;
  unsigned char b_347X;
  char v_348X;
  long tem_349X;
  unsigned char b_350X;
  unsigned char b_351X;
  unsigned char b_352X;
  unsigned char b_353X;
  unsigned char b_354X;
  long val_355X;
  unsigned char b_356X;
  long p_357X;
  long x_358X;
  long x_359X;
  long z_360X;
  long x_361X;
  long arg2_362X;
  long arg2_363X;
  long arg2_364X;
  long x_365X;
  long arg2_366X;
  long arg2_367X;
  long arg2_368X;
  long x_369X;
  long arg2_370X;
  long arg2_371X;
  long arg2_372X;
  long arg2_373X;
  long arg2_374X;
  long arg2_375X;
  long x_376X;
  long result_377X;
  long x_378X;
  long count_379X;
  long value_380X;
  long x_381X;
  long x_382X;
  unsigned char b_383X;
  long x_384X;
  char okayP_385X;
  char v_386X;
  long space_387X;
  unsigned char b_388X;
  unsigned char b_389X;
  long x_390X;
  unsigned char b_391X;
  long x_392X;
  long arg2_393X;
  unsigned char b_394X;
  long x_395X;
  long arg2_396X;
  long addr_397X;
  unsigned char b_398X;
  long x_399X;
  long arg3_400X;
  long arg2_401X;
  long arg2_402X;
  long len_403X;
  long index_404X;
  long len_405X;
  long Kchar_406X;
  long index_407X;
  char okayP_408X;
  char v_409X;
  long size_410X;
  unsigned char init_411X;
  long len_412X;
  long arg2_413X;
  long len_414X;
  long index_415X;
  long len_416X;
  unsigned char Kchar_417X;
  long index_418X;
  long arg3_419X;
  long arg2_420X;
  long x_421X;
  long arg2_422X;
  long x_423X;
  long arg4_424X;
  long arg3_425X;
  long arg2_426X;
  long head_427X;
  long channel_428X;
  long obj_429X;
  FILE * port_430X;
  long status_431X;
  long bytes_432X;
  long n_433X;
  long key_434X;
  long x_435X;
  long type_436X;
  long x_437X;
  long obj_438X;
  long p_439X;
  long old_440X;
  long p_441X;
  long x_442X;
  long proc_443X;
  long option_444X;
  long x_445X;
  long status_446X;
  long key_447X;
  long x_448X;
  long len_449X;
  long x_450X;
  long obj_451X;
  long arg2_452X;
  char okayP_453X;
  char v_454X;
  long space_455X;
  long x_456X;
  long arg2_457X;
  long index_458X;
  long len_459X;
  long arg4_460X;
  long arg3_461X;
  long arg2_462X;
  long arg5_463X;
  long arg4_464X;
  long arg3_465X;
  long arg2_466X;
  long addr_467X;
  long val_468X;
  long x_469X;
  long i_470X;
  long b_471X;
  long p_472X;
  long p_473X;
  long port_474X;
  long x_475X;
  long x_476X;
  long i_477X;
  long b_478X;
  long p_479X;
  long p_480X;
  long port_481X;
  long x_482X;
  long addr_483X;
  long val_484X;
  long x_485X;
  long i_486X;
  long b_487X;
  long p_488X;
  unsigned char Kchar_489X;
  long x_490X;
  long x_491X;
  unsigned char b_492X;
  long arg2_493X;
  long from_index_494X;
  long to_index_495X;
  long count_496X;
  long x_497X;
  long index_498X;
  long addr_499X;
  long arg2_500X;
  long arg3_501X;
  long len_502X;
  char v_503X;
  char okayP_504X;
  long obj_505X;
  long x_506X;
  long n_507X;
  long arg2_508X;
  long arg2_509X;
  long arg2_510X;
  long mseconds_511X;
  long seconds_512X;
  long mseconds_513X;
  long seconds_514X;
  long n_515X;
  long obj_516X;
  long x_517X;
  long value_518X;
  long name_519X;
  char v_520X;
  long arg2_521X;
  long p_522X;
  long obj_523X;
  long temp_524X;
  long obj_525X;
  long temp_526X;
  long x_527X;
  long arg2_528X;
  long n_529X;
  char v_530X;
  char okayP_531X;
  long arg2_532X;
  long arg3_533X;
  long obj_534X;
  long x_535X;
  char x_536X;
  long status_537X;
  long status_538X;
  long v_539X;
  long status_540X;
  long status_541X;
  long v_542X;
  long status_543X;
  long x_544X;
  long channel_545X;
  long next_546X;
  long addr_547X;
  long n_548X;
  long obj_549X;
  long start_550X;
  long count_551X;
  long channel_552X;
  char v_553X;
  long arg2_554X;
  long arg3_555X;
  long arg4_556X;
  long arg5_557X;
  long obj_558X;
  long x_559X;
  long os_index_560X;
  long v_561X;
  long v_562X;
  char v_563X;
  char okayP_564X;
  long x_565X;
  long obj_566X;
  long x_567X;
  long descriptor_568X;
  long arg2_569X;
  long obj_570X;
  long arg2_571X;
  long arg3_572X;
  long arg2_573X;
  long obj_574X;
  long len_575X;
  long init_576X;
  long size_577X;
  char v_578X;
  char okayP_579X;
  long len_580X;
  long index_581X;
  long len_582X;
  long index_583X;
  long len_584X;
  long size_585X;
  char v_586X;
  long temp_587X;
  char v_588X;
  long arg2_589X;
  long x_590X;
  unsigned char b_591X;
  unsigned char b_592X;
  long addr_593X;
  long len_594X;
  long space_595X;
  char v_596X;
  char okayP_597X;
  long x_598X;
  unsigned char b_599X;
  long x_600X;
  long arg2_601X;
  long x_602X;
  long arg2_603X;
  long x_604X;
  long x_605X;
  long arg2_606X;
  long x_607X;
  long x_608X;
  long x_609X;
  long x_610X;
  long x_611X;
  long x_612X;
  long x_613X;
  long x_614X;
  long x_615X;
  long x_616X;
  long x_617X;
  long x_618X;
  long x_619X;
  long x_620X;
  long a_621X;
  long x_622X;
  long a_623X;
  long b_624X;
  long x_625X;
  long arg2_626X;
  long y_627X;
  long a_628X;
  long b_629X;
  long x_630X;
  long z_631X;
  long y_632X;
  long a_633X;
  long b_634X;
  long arg2_635X;
  long x_636X;
  long x_637X;
  long x_638X;
  long x_639X;
  long x_640X;
  long arg2_641X;
  long x_642X;
  unsigned char b_643X;
  unsigned char b_644X;
  unsigned char b_645X;
  unsigned char b_646X;
  long consumer_647X;
  long proc_648X;
  long list_649X;
  long space_650X;
  char v_651X;
  char okayP_652X;
  unsigned char b_653X;
  long p_654X;
  unsigned char b_655X;
  unsigned char b_656X;
  long location_657X;
  long val_658X;
  long addr_659X;
  unsigned char b_660X;
  unsigned char b_661X;
  long location_662X;
  unsigned char b_663X;
  long env_664X;
  unsigned char b_665X;
  long v_666X;
  unsigned char count_667X;
  long space_668X;
  char v_669X;
  char okayP_670X;
  long args_671X;
  long v_672X;
  long v_673X;
  long args_674X;
  long cont_675X;
  long v_676X;
  unsigned char next_op_677X;
  long v_678X;
  long next_679X;
  long cont_680X;
  long tem_681X;
  long pc_682X;
  long nargs_683X;
  long cont_684X;
  long tem_685X;
  long pc_686X;
  unsigned char v_687X;
  char x_688X;
  char okayP_689X;
  char v_690X;
  long space_691X;
  long obj_692X;
  unsigned char nargs_693X;
  unsigned char args_694X;
  long obj_695X;
  unsigned char bytes_consumed_696X;
  char v_697X;
  long tem_698X;
  long obj_699X;
  long args_700X;
  long a1_701X;
  unsigned char byte_args_702X;
  unsigned char exception_703X;
  long v_704X;
  long v_705X;
  unsigned char v_706X;
  long obj_707X;
  long channel_708X;
  char x_709X;
  long obj_710X;
  unsigned char v_711X;
  long key_712X;
  long tem_713X;
  long n_714X;
  long args_on_stack_715X;
  char okayP_716X;
  char v_717X;
  long space_718X;
  long p_719X;
  long i_720X;
  long i_721X;
  long m_722X;
  long channel_723X;
  long next_724X;
  long addr_725X;
  long opcode_726X;
  long why_727X;
  long current_template_728X;
  FILE * out_729X;
  long v_730X;
  char v_731X;
  char not_firstP_732X;
  long cont_733X;
  char not_firstP_734X;
  long template_735X;
  long addr_736X;
  long val_737X;
  long channel_738X;
  long v_739X;
  long v_740X;
  long status_741X;
  long channel_742X;
  long type_743X;
  long v_744X;
  long addr_745X;
  long v_746X;
  long space_747X;
  char v_748X;
  char okayP_749X;
  long args_750X;
  long count_751X;
  long a_752X;
  long addr_753X;
  long x_754X;
  long pc_755X;
  long v_756X;
  long v_757X;
  long v_758X;
  long v_759X;
  long v_760X;
  long v_761X;
  long v_762X;
  long v_763X;
  long v_764X;
  long env_765X;
  long v_766X;
  long v_767X;
  long v_768X;
  long status_769X;
  long addr_770X;
  long v_771X;
  long id_772X;
  long os_index_773X;
  long v_774X;
  long v_775X;
  long new_776X;
  long i_777X;
  long channel_778X;
  long header_779X;
  long id_780X;
  long tem_781X;
  long v_782X;
  long env_783X;
  long env_784X;
  long arg_count_785X;
  long p_786X;
  long env_787X;
  long p_788X;
  long i_789X;
  long l2_790X;
  long okay_791X;
  long sickly_792X;
  long value_793X;
  long cont_794X;
  long last_env_795X;
  long env_796X;
  long v_797X;
  long l_798X;
  long addr_799X;
  long v_800X;
  long v_801X;
  long loc_802X;
  long arg_803X;
  long stob_804X;
  long arg_count_805X;
  long top_806X;
  long p_807X;
  long i_808X;
  long p_809X;
  long v_810X;
  long end_811X;
  long header_812X;
  long addr_813X;
  long data_addr_814X;
  long new_815X;
  long cont_816X;
  long previous_817X;
  long cont_818X;
  long p_819X;
  long header_820X;
  long addr_821X;
  long data_addr_822X;
  long new_823X;
  long addr_824X;
  long cont_825X;
  long env_826X;
  long env_827X;
  long p_828X;
  long env_829X;
  long header_830X;
  long addr_831X;
  long data_addr_832X;
  long new_833X;
  long addr_834X;
  long addr_835X;
  long top_836X;
  long new_cont_837X;
  long v_838X;
  long v_839X;
  long p_840X;
  long i_841X;
  long key_842X;
  long pc_843X;
  long template_844X;
  long cont_845X;
  long code_pointer_846X;
  long size_847X;
  unsigned char byte_args_848X;
  unsigned char exception_849X;
  long cont_850X;
  long env_851X;
  long cont_852X;
  unsigned char key_853X;
  unsigned char reason_854X;
  unsigned char key_855X;
  long status_856X;
  long index_857X;
  long id_858X;
  long value_859X;
  long start_860X;
  long count_861X;
  unsigned char *message_862X;
 {RSvalS = value_25X;
  goto L12101;}
 L12101: {
  goto L11161;}
 L11161: {
  b_492X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  switch ((((unsigned long) b_492X))) {
    case 0 : {
      b_310X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((SnargsS) == (((unsigned long) b_310X)))) {
        goto L11161;}
      else {
        arg1K0 = 1;
        arg1K1 = 4;
        goto L26441;}}
      break;
    case 1 : {
      b_311X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((SnargsS) < (((unsigned long) b_311X)))) {
        arg1K0 = 1;
        arg1K1 = 4;
        goto L26441;}
      else {
        goto L11161;}}
      break;
    case 2 : {
      RSvalS = ((((SnargsS))<<2));
      goto L11161;}
      break;
    case 3 : {
      count_667X = *((unsigned char *) (RScode_pointerS));
      if (((Sstack_limitS) < (-264 + ((RSstackS) - ((((((unsigned long) count_667X)))<<2)))))) {
        goto L12024;}
      else {
        space_668X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
        v_669X = AVAILABLEp(space_668X);
        if (v_669X) {
          arg1K0 = 0;
          goto L12042;}
        else {
          merged_arg0K0 = 0;
          collect_saving_temp_return_tag = 0;
          goto collect_saving_temp;
         collect_saving_temp_return_0:
          okayP_670X = AVAILABLEp(space_668X);
          if (okayP_670X) {
            arg1K0 = 0;
            goto L12042;}
          else {
            ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
            arg1K0 = 0;
            goto L12042;}}}}
      break;
    case 4 : {
      space_314X = 2 + (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
      v_313X = AVAILABLEp(space_314X);
      if (v_313X) {
        goto L29715;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 1;
        goto collect_saving_temp;
       collect_saving_temp_return_1:
        okayP_312X = AVAILABLEp(space_314X);
        if (okayP_312X) {
          goto L29715;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L29715;}}}
      break;
    case 5 : {
      RSenvS = (*((long *) (-3 + (RSenvS))));
      goto L11161;}
      break;
    case 6 : {
      b_665X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((64 == (SnargsS))) {
        RSstackS = (4 + (RSstackS));
        merged_arg0K0 = (*((long *) (RSstackS)));
        merged_arg0K1 = ((-1 + (SnargsS)) - (((unsigned long) b_665X)));
        pop_args_list_return_tag = 0;
        goto pop_args_list;
       pop_args_list_return_0:
        v_315X = pop_args_list0_return_value;
        arg0K0 = v_315X;
        goto L11941;}
      else {
        merged_arg0K0 = 25;
        merged_arg0K1 = ((SnargsS) - (((unsigned long) b_665X)));
        pop_args_list_return_tag = 1;
        goto pop_args_list;
       pop_args_list_return_1:
        v_666X = pop_args_list0_return_value;
        arg0K0 = v_666X;
        goto L11941;}}
      break;
    case 7 : {
      b_317X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_316X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + (StemplateS)) + ((((((((((unsigned long) b_317X)))<<8)) + (((unsigned long) b_316X))))<<2)))));
      goto L11161;}
      break;
    case 8 : {
      RSvalS = (*((long *) ((-3 + (StemplateS)) + ((((((unsigned long) (*((unsigned char *) (RScode_pointerS))))))<<2)))));
      RScode_pointerS = (2 + (RScode_pointerS));
      goto L11161;}
      break;
    case 9 : {
      b_319X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      env_318X = RSenvS;
      arg0K0 = env_318X;
      arg0K1 = (((unsigned long) b_319X));
      goto L26671;}
      break;
    case 10 : {
      env_321X = RSenvS;
      b_320X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_321X) + ((((((unsigned long) b_320X)))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg1K0 = 1;
        merged_arg1K1 = 0;
        push_exception_continuationB_return_tag = 0;
        goto push_exception_continuationB;
       push_exception_continuationB_return_0:
        arg1K0 = 0;
        arg1K1 = 1;
        goto L12977;}
      else {
        goto L11161;}}
      break;
    case 11 : {
      env_323X = *((long *) (-3 + (RSenvS)));
      b_322X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_323X) + ((((((unsigned long) b_322X)))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg1K0 = 1;
        merged_arg1K1 = 0;
        push_exception_continuationB_return_tag = 1;
        goto push_exception_continuationB;
       push_exception_continuationB_return_1:
        arg1K0 = 0;
        arg1K1 = 1;
        goto L12977;}
      else {
        goto L11161;}}
      break;
    case 12 : {
      env_325X = *((long *) (-3 + (*((long *) (-3 + (RSenvS))))));
      b_324X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_325X) + ((((((unsigned long) b_324X)))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg1K0 = 1;
        merged_arg1K1 = 0;
        push_exception_continuationB_return_tag = 2;
        goto push_exception_continuationB;
       push_exception_continuationB_return_2:
        arg1K0 = 0;
        arg1K1 = 1;
        goto L12977;}
      else {
        goto L11161;}}
      break;
    case 13 : {
      b_663X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      env_664X = RSenvS;
      arg0K0 = env_664X;
      arg0K1 = (((unsigned long) b_663X));
      goto L11862;}
      break;
    case 14 : {
      b_660X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_661X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      location_662X = *((long *) ((-3 + (StemplateS)) + ((((((((((unsigned long) b_660X)))<<8)) + (((unsigned long) b_661X))))<<2))));
      RSvalS = (*((long *) (1 + location_662X)));
      if ((17 == (255 & (RSvalS)))) {
        merged_arg1K0 = 2;
        merged_arg1K1 = 1;
        push_exception_continuationB_return_tag = 3;
        goto push_exception_continuationB;
       push_exception_continuationB_return_3:
        *((long *) (RSstackS)) = location_662X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 2;
        goto L12977;}
      else {
        goto L11161;}}
      break;
    case 15 : {
      b_655X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_656X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      location_657X = *((long *) ((-3 + (StemplateS)) + ((((((((((unsigned long) b_655X)))<<8)) + (((unsigned long) b_656X))))<<2))));
      if ((273 == (*((long *) (1 + location_657X))))) {
        merged_arg1K0 = 2;
        merged_arg1K1 = 1;
        push_exception_continuationB_return_tag = 4;
        goto push_exception_continuationB;
       push_exception_continuationB_return_4:
        *((long *) (RSstackS)) = location_657X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 2;
        goto L12977;}
      else {
        val_658X = RSvalS;
        addr_659X = 1 + location_657X;
        WRITE_BARRIER(addr_659X, val_658X);
        *((long *) addr_659X) = val_658X;
        RSvalS = 13;
        goto L11161;}}
      break;
    case 16 : {
      p_654X = RSenvS;
      if ((3 == (3 & p_654X))) {
        if ((p_654X < (Sstack_beginS))) {
          arg0K0 = 0;
          goto L11719;}
        else {
          if (((Sstack_endS) < p_654X)) {
            arg0K0 = 0;
            goto L11719;}
          else {
            arg0K0 = ((Sstack_endS) - (Sstack_beginS));
            goto L11719;}}}
      else {
        arg0K0 = 0;
        goto L11719;}}
      break;
    case 17 : {
      b_329X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      space_328X = 4 + (((unsigned long) b_329X));
      v_327X = AVAILABLEp(space_328X);
      if (v_327X) {
        goto L11559;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 2;
        goto collect_saving_temp;
       collect_saving_temp_return_2:
        okayP_326X = AVAILABLEp(space_328X);
        if (okayP_326X) {
          goto L11559;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L11559;}}}
      break;
    case 18 : {
      *((long *) (RSstackS)) = (RSvalS);
      RSstackS = (-4 + (RSstackS));
      goto L11161;}
      break;
    case 19 : {
      RSstackS = (4 + (RSstackS));
      RSvalS = (*((long *) (RSstackS)));
      goto L11161;}
      break;
    case 20 : {
      b_653X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) (4 + ((RSstackS) + ((((((unsigned long) b_653X)))<<2))))));
      goto L11161;}
      break;
    case 21 : {
      b_330X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      *((long *) (4 + ((RSstackS) + ((((((unsigned long) b_330X)))<<2))))) = (RSvalS);
      goto L11161;}
      break;
    case 22 : {
      b_336X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_335X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_334X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((Sstack_limitS) < (-276 + (RSstackS)))) {
        arg0K0 = 0;
        goto L11442;}
      else {
        space_333X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
        v_332X = AVAILABLEp(space_333X);
        if (v_332X) {
          arg1K0 = 0;
          goto L11468;}
        else {
          merged_arg0K0 = 0;
          collect_saving_temp_return_tag = 3;
          goto collect_saving_temp;
         collect_saving_temp_return_3:
          okayP_331X = AVAILABLEp(space_333X);
          if (okayP_331X) {
            arg1K0 = 0;
            goto L11468;}
          else {
            ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
            arg1K0 = 0;
            goto L11468;}}}}
      break;
    case 23 : {
      space_650X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
      v_651X = AVAILABLEp(space_650X);
      if (v_651X) {
        arg1K0 = 0;
        goto L29815;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 4;
        goto collect_saving_temp;
       collect_saving_temp_return_4:
        okayP_652X = AVAILABLEp(space_650X);
        if (okayP_652X) {
          arg1K0 = 0;
          goto L29815;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          arg1K0 = 0;
          goto L29815;}}}
      break;
    case 24 : {
      cont_337X = *((long *) (-3 + (Sbottom_of_stackS)));
      if ((3 == (3 & cont_337X))) {
        if ((9 == (31 & ((((*((long *) (-7 + cont_337X))))>>2))))) {
          if ((3 == (3 & cont_337X))) {
            if ((9 == (31 & ((((*((long *) (-7 + cont_337X))))>>2))))) {
              merged_arg0K0 = cont_337X;
              copy_continuation_from_heapB_return_tag = 0;
              goto copy_continuation_from_heapB;
             copy_continuation_from_heapB_return_0:
              goto L11161;}
            else {
              goto L13718;}}
          else {
            goto L13718;}}
        else {
          goto L13682;}}
      else {
        goto L13682;}}
      break;
    case 25 : {
      SnargsS = (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
      arg1K0 = 0;
      goto L12631;}
      break;
    case 26 : {
      SnargsS = (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
      nargs_340X = SnargsS;
      top_of_args_339X = RSstackS;
      stob_338X = ScontS;
      arg0K0 = (-11 + stob_338X);
      arg0K1 = (top_of_args_339X + (((nargs_340X)<<2)));
      goto L29856;}
      break;
    case 27 : {
      RSstackS = (4 + (RSstackS));
      proc_648X = *((long *) (RSstackS));
      list_649X = RSvalS;
      arg0K0 = list_649X;
      arg0K1 = 0;
      arg0K2 = list_649X;
      arg2K3 = 0;
      goto L8321;}
      break;
    case 28 : {
      RSstackS = (4 + (RSstackS));
      cont_341X = *((long *) (RSstackS));
      if ((3 == (3 & cont_341X))) {
        if ((9 == (31 & ((((*((long *) (-7 + cont_341X))))>>2))))) {
          merged_arg0K0 = cont_341X;
          copy_continuation_from_heapB_return_tag = 1;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_1:
          goto L12751;}
        else {
          goto L12761;}}
      else {
        goto L12761;}}
      break;
    case 29 : {
      RSstackS = (4 + (RSstackS));
      consumer_647X = *((long *) (RSstackS));
      *((long *) (RSstackS)) = (RSvalS);
      RSstackS = (-4 + (RSstackS));
      RSvalS = consumer_647X;
      SnargsS = 1;
      arg1K0 = 0;
      goto L12631;}
      break;
    case 30 : {
      cont_344X = ScontS;
      tem_343X = *((long *) (5 + cont_344X));
      pc_342X = *((long *) (1 + cont_344X));
      StemplateS = tem_343X;
      RScode_pointerS = ((-3 + (*((long *) (-3 + tem_343X)))) + (((pc_342X)>>2)));
      RSenvS = (*((long *) (9 + cont_344X)));
      ScontS = (*((long *) (-3 + cont_344X)));
      RSstackS = (9 + cont_344X);
      goto L11161;}
      break;
    case 31 : {
      arg0K0 = (SnargsS);
      goto L29360;}
      break;
    case 32 : {
      arg0K0 = (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
      goto L29360;}
      break;
    case 33 : {
      goto L11161;}
      break;
    case 34 : {
      b_347X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_346X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      tem_345X = *((long *) ((-3 + (StemplateS)) + ((((((((((unsigned long) b_347X)))<<8)) + (((unsigned long) b_346X))))<<2))));
      StemplateS = tem_345X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_345X))));
      goto L11161;}
      break;
    case 35 : {
      b_352X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      SnargsS = (((unsigned long) b_352X));
      b_351X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_350X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      tem_349X = *((long *) ((-3 + (StemplateS)) + ((((((((((unsigned long) b_351X)))<<8)) + (((unsigned long) b_350X))))<<2))));
      StemplateS = tem_349X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_349X))));
      if ((Spending_interruptPS)) {
        if ((Spending_eventsPS)) {
          Spending_eventsPS = 0;
          check_events_return_tag = 0;
          goto check_events;
         check_events_return_0:
          v_348X = check_events0_return_value;
          if (v_348X) {
            goto L13261;}
          else {
            goto L11161;}}
        else {
          goto L13261;}}
      else {
        goto L11161;}}
      break;
    case 36 : {
      b_354X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_353X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((1 == (RSvalS))) {
        RScode_pointerS = ((RScode_pointerS) + (((((((unsigned long) b_354X)))<<8)) + (((unsigned long) b_353X))));
        goto L11161;}
      else {
        goto L11161;}}
      break;
    case 37 : {
      b_645X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_646X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RScode_pointerS = ((RScode_pointerS) + (((((((unsigned long) b_645X)))<<8)) + (((unsigned long) b_646X))));
      goto L11161;}
      break;
    case 38 : {
      if ((0 == (3 & (RSvalS)))) {
        p_357X = RSvalS;
        b_356X = *((unsigned char *) (RScode_pointerS));
        RScode_pointerS = (1 + (RScode_pointerS));
        val_355X = ((p_357X)>>2);
        if ((val_355X < 0)) {
          goto L13513;}
        else {
          if ((val_355X < (((unsigned long) b_356X)))) {
            RScode_pointerS = ((RScode_pointerS) + (((val_355X)<<1)));
            b_643X = *((unsigned char *) (RScode_pointerS));
            RScode_pointerS = (1 + (RScode_pointerS));
            b_644X = *((unsigned char *) (RScode_pointerS));
            RScode_pointerS = (1 + (RScode_pointerS));
            arg0K0 = (((((((unsigned long) b_643X)))<<8)) + (((unsigned long) b_644X)));
            goto L13515;}
          else {
            goto L13513;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 5;
        goto push_exception_continuationB;
       push_exception_continuationB_return_5:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 39 : {
      RSstackS = (4 + (RSstackS));
      arg2_641X = *((long *) (RSstackS));
      x_642X = RSvalS;
      if ((arg2_641X == x_642X)) {
        arg1K0 = 5;
        goto L29931;}
      else {
        arg1K0 = 1;
        goto L29931;}}
      break;
    case 40 : {
      x_640X = RSvalS;
      if ((0 == (3 & x_640X))) {
        arg1K0 = 5;
        goto L25873;}
      else {
        if ((3 == (3 & x_640X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_640X))))>>2))))) {
            arg1K0 = 5;
            goto L25873;}
          else {
            goto L25828;}}
        else {
          goto L25828;}}}
      break;
    case 41 : {
      x_639X = RSvalS;
      if ((0 == (3 & x_639X))) {
        goto L25698;}
      else {
        if ((3 == (3 & x_639X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_639X))))>>2))))) {
            goto L25698;}
          else {
            goto L25701;}}
        else {
          goto L25701;}}}
      break;
    case 42 : {
      x_638X = RSvalS;
      if ((0 == (3 & x_638X))) {
        goto L25509;}
      else {
        if ((3 == (3 & x_638X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_638X))))>>2))))) {
            goto L25509;}
          else {
            goto L25492;}}
        else {
          goto L25492;}}}
      break;
    case 43 : {
      x_637X = RSvalS;
      if ((0 == (3 & x_637X))) {
        goto L25300;}
      else {
        if ((3 == (3 & x_637X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_637X))))>>2))))) {
            goto L25300;}
          else {
            goto L25283;}}
        else {
          goto L25283;}}}
      break;
    case 44 : {
      x_636X = RSvalS;
      if ((0 == (3 & x_636X))) {
        goto L25091;}
      else {
        if ((3 == (3 & x_636X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_636X))))>>2))))) {
            goto L25091;}
          else {
            goto L25074;}}
        else {
          goto L25074;}}}
      break;
    case 45 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 5;
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 6;
        goto push_exception_continuationB;
       push_exception_continuationB_return_6:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 46 : {
      if ((0 == (3 & (RSvalS)))) {
        x_358X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 7;
        goto push_exception_continuationB;
       push_exception_continuationB_return_7:
        *((long *) (RSstackS)) = x_358X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 8;
        goto push_exception_continuationB;
       push_exception_continuationB_return_8:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 47 : {
      if ((0 == (3 & (RSvalS)))) {
        x_359X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 9;
        goto push_exception_continuationB;
       push_exception_continuationB_return_9:
        *((long *) (RSstackS)) = x_359X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 10;
        goto push_exception_continuationB;
       push_exception_continuationB_return_10:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 48 : {
      RSstackS = (4 + (RSstackS));
      arg2_635X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_635X | (RSvalS))))) {
        x_361X = RSvalS;
        z_360X = (((arg2_635X)>>2)) + (((x_361X)>>2));
        if ((536870911 < z_360X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 11;
          goto push_exception_continuationB;
         push_exception_continuationB_return_11:
          *((long *) (RSstackS)) = arg2_635X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_361X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12977;}
        else {
          if ((z_360X < -536870912)) {
            merged_arg1K0 = 0;
            merged_arg1K1 = 6;
            push_exception_continuationB_return_tag = 12;
            goto push_exception_continuationB;
           push_exception_continuationB_return_12:
            *((long *) (RSstackS)) = arg2_635X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_361X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12977;}
          else {
            RSvalS = (((z_360X)<<2));
            goto L11161;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 13;
        goto push_exception_continuationB;
       push_exception_continuationB_return_13:
        *((long *) (RSstackS)) = arg2_635X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 49 : {
      RSstackS = (4 + (RSstackS));
      arg2_362X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_362X | (RSvalS))))) {
        y_632X = RSvalS;
        a_633X = ((arg2_362X)>>2);
        b_634X = ((y_632X)>>2);
        if ((a_633X < 0)) {
          arg0K0 = (0 - a_633X);
          goto L5372;}
        else {
          arg0K0 = a_633X;
          goto L5372;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 14;
        goto push_exception_continuationB;
       push_exception_continuationB_return_14:
        *((long *) (RSstackS)) = arg2_362X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 50 : {
      RSstackS = (4 + (RSstackS));
      arg2_363X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_363X | (RSvalS))))) {
        x_630X = RSvalS;
        z_631X = (((arg2_363X)>>2)) - (((x_630X)>>2));
        if ((536870911 < z_631X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 15;
          goto push_exception_continuationB;
         push_exception_continuationB_return_15:
          *((long *) (RSstackS)) = arg2_363X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_630X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12977;}
        else {
          if ((z_631X < -536870912)) {
            merged_arg1K0 = 0;
            merged_arg1K1 = 6;
            push_exception_continuationB_return_tag = 16;
            goto push_exception_continuationB;
           push_exception_continuationB_return_16:
            *((long *) (RSstackS)) = arg2_363X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_630X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12977;}
          else {
            RSvalS = (((z_631X)<<2));
            goto L11161;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 17;
        goto push_exception_continuationB;
       push_exception_continuationB_return_17:
        *((long *) (RSstackS)) = arg2_363X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 51 : {
      RSstackS = (4 + (RSstackS));
      arg2_364X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_364X | (RSvalS))))) {
        y_627X = RSvalS;
        if ((0 == y_627X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 18;
          goto push_exception_continuationB;
         push_exception_continuationB_return_18:
          *((long *) (RSstackS)) = arg2_364X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = y_627X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12977;}
        else {
          a_628X = ((arg2_364X)>>2);
          b_629X = ((y_627X)>>2);
          if ((a_628X < 0)) {
            arg0K0 = (0 - a_628X);
            goto L7147;}
          else {
            arg0K0 = a_628X;
            goto L7147;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 19;
        goto push_exception_continuationB;
       push_exception_continuationB_return_19:
        *((long *) (RSstackS)) = arg2_364X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 52 : {
      RSstackS = (4 + (RSstackS));
      arg2_626X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_626X | (RSvalS))))) {
        x_365X = RSvalS;
        if ((arg2_626X == x_365X)) {
          arg1K0 = 5;
          goto L24896;}
        else {
          arg1K0 = 1;
          goto L24896;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 20;
        goto push_exception_continuationB;
       push_exception_continuationB_return_20:
        *((long *) (RSstackS)) = arg2_626X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 53 : {
      RSstackS = (4 + (RSstackS));
      arg2_366X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_366X | (RSvalS))))) {
        x_625X = RSvalS;
        if ((arg2_366X < x_625X)) {
          arg1K0 = 5;
          goto L24807;}
        else {
          arg1K0 = 1;
          goto L24807;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 21;
        goto push_exception_continuationB;
       push_exception_continuationB_return_21:
        *((long *) (RSstackS)) = arg2_366X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 54 : {
      RSstackS = (4 + (RSstackS));
      arg2_367X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_367X | (RSvalS))))) {
        x_622X = RSvalS;
        if ((0 == x_622X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 22;
          goto push_exception_continuationB;
         push_exception_continuationB_return_22:
          *((long *) (RSstackS)) = arg2_367X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_622X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12977;}
        else {
          a_623X = ((arg2_367X)>>2);
          b_624X = ((x_622X)>>2);
          if ((a_623X < 0)) {
            arg0K0 = (0 - a_623X);
            goto L28811;}
          else {
            arg0K0 = a_623X;
            goto L28811;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 23;
        goto push_exception_continuationB;
       push_exception_continuationB_return_23:
        *((long *) (RSstackS)) = arg2_367X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 55 : {
      RSstackS = (4 + (RSstackS));
      arg2_368X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_368X | (RSvalS))))) {
        x_620X = RSvalS;
        if ((0 == x_620X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 24;
          goto push_exception_continuationB;
         push_exception_continuationB_return_24:
          *((long *) (RSstackS)) = arg2_368X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_620X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12977;}
        else {
          a_621X = ((arg2_368X)>>2);
          if ((a_621X < 0)) {
            arg0K0 = (0 - a_621X);
            goto L28680;}
          else {
            arg0K0 = a_621X;
            goto L28680;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 25;
        goto push_exception_continuationB;
       push_exception_continuationB_return_25:
        *((long *) (RSstackS)) = arg2_368X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 56 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 26;
        goto push_exception_continuationB;
       push_exception_continuationB_return_26:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 57 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 27;
        goto push_exception_continuationB;
       push_exception_continuationB_return_27:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 58 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 4;
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 28;
        goto push_exception_continuationB;
       push_exception_continuationB_return_28:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 59 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 29;
        goto push_exception_continuationB;
       push_exception_continuationB_return_29:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 60 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 0;
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 30;
        goto push_exception_continuationB;
       push_exception_continuationB_return_30:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 61 : {
      if ((0 == (3 & (RSvalS)))) {
        x_619X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 31;
        goto push_exception_continuationB;
       push_exception_continuationB_return_31:
        *((long *) (RSstackS)) = x_619X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 32;
        goto push_exception_continuationB;
       push_exception_continuationB_return_32:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 62 : {
      if ((0 == (3 & (RSvalS)))) {
        x_618X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 33;
        goto push_exception_continuationB;
       push_exception_continuationB_return_33:
        *((long *) (RSstackS)) = x_618X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 34;
        goto push_exception_continuationB;
       push_exception_continuationB_return_34:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 63 : {
      if ((0 == (3 & (RSvalS)))) {
        x_617X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 35;
        goto push_exception_continuationB;
       push_exception_continuationB_return_35:
        *((long *) (RSstackS)) = x_617X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 36;
        goto push_exception_continuationB;
       push_exception_continuationB_return_36:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 64 : {
      if ((0 == (3 & (RSvalS)))) {
        x_616X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 37;
        goto push_exception_continuationB;
       push_exception_continuationB_return_37:
        *((long *) (RSstackS)) = x_616X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 38;
        goto push_exception_continuationB;
       push_exception_continuationB_return_38:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 65 : {
      if ((0 == (3 & (RSvalS)))) {
        x_615X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 39;
        goto push_exception_continuationB;
       push_exception_continuationB_return_39:
        *((long *) (RSstackS)) = x_615X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 40;
        goto push_exception_continuationB;
       push_exception_continuationB_return_40:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 66 : {
      if ((0 == (3 & (RSvalS)))) {
        x_614X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 41;
        goto push_exception_continuationB;
       push_exception_continuationB_return_41:
        *((long *) (RSstackS)) = x_614X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 42;
        goto push_exception_continuationB;
       push_exception_continuationB_return_42:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 67 : {
      if ((0 == (3 & (RSvalS)))) {
        x_613X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 43;
        goto push_exception_continuationB;
       push_exception_continuationB_return_43:
        *((long *) (RSstackS)) = x_613X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 44;
        goto push_exception_continuationB;
       push_exception_continuationB_return_44:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 68 : {
      if ((0 == (3 & (RSvalS)))) {
        x_612X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 45;
        goto push_exception_continuationB;
       push_exception_continuationB_return_45:
        *((long *) (RSstackS)) = x_612X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 46;
        goto push_exception_continuationB;
       push_exception_continuationB_return_46:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 69 : {
      if ((0 == (3 & (RSvalS)))) {
        x_369X = RSvalS;
        if ((x_369X < 0)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 47;
          goto push_exception_continuationB;
         push_exception_continuationB_return_47:
          *((long *) (RSstackS)) = x_369X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 1;
          arg1K1 = 0;
          goto L12977;}
        else {
          RSvalS = 0;
          goto L11161;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 48;
        goto push_exception_continuationB;
       push_exception_continuationB_return_48:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 70 : {
      if ((0 == (3 & (RSvalS)))) {
        x_611X = RSvalS;
        if ((x_611X < 0)) {
          arg0K0 = (0 - x_611X);
          goto L24187;}
        else {
          arg0K0 = x_611X;
          goto L24187;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 49;
        goto push_exception_continuationB;
       push_exception_continuationB_return_49:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 71 : {
      RSstackS = (4 + (RSstackS));
      arg2_370X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_370X | (RSvalS))))) {
        x_610X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 50;
        goto push_exception_continuationB;
       push_exception_continuationB_return_50:
        *((long *) (RSstackS)) = arg2_370X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_610X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 51;
        goto push_exception_continuationB;
       push_exception_continuationB_return_51:
        *((long *) (RSstackS)) = arg2_370X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 72 : {
      RSstackS = (4 + (RSstackS));
      arg2_371X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_371X | (RSvalS))))) {
        x_609X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 52;
        goto push_exception_continuationB;
       push_exception_continuationB_return_52:
        *((long *) (RSstackS)) = arg2_371X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_609X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 53;
        goto push_exception_continuationB;
       push_exception_continuationB_return_53:
        *((long *) (RSstackS)) = arg2_371X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 73 : {
      RSstackS = (4 + (RSstackS));
      arg2_372X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_372X | (RSvalS))))) {
        x_608X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 54;
        goto push_exception_continuationB;
       push_exception_continuationB_return_54:
        *((long *) (RSstackS)) = arg2_372X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_608X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 55;
        goto push_exception_continuationB;
       push_exception_continuationB_return_55:
        *((long *) (RSstackS)) = arg2_372X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 74 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = ((((~ ((((RSvalS))>>2))))<<2));
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 56;
        goto push_exception_continuationB;
       push_exception_continuationB_return_56:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 75 : {
      RSstackS = (4 + (RSstackS));
      arg2_373X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_373X | (RSvalS))))) {
        RSvalS = (((((((arg2_373X)>>2)) & ((((RSvalS))>>2))))<<2));
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 57;
        goto push_exception_continuationB;
       push_exception_continuationB_return_57:
        *((long *) (RSstackS)) = arg2_373X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 76 : {
      RSstackS = (4 + (RSstackS));
      arg2_374X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_374X | (RSvalS))))) {
        RSvalS = (((((((arg2_374X)>>2)) | ((((RSvalS))>>2))))<<2));
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 58;
        goto push_exception_continuationB;
       push_exception_continuationB_return_58:
        *((long *) (RSstackS)) = arg2_374X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 77 : {
      RSstackS = (4 + (RSstackS));
      arg2_375X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_375X | (RSvalS))))) {
        RSvalS = (((((((arg2_375X)>>2)) ^ ((((RSvalS))>>2))))<<2));
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 59;
        goto push_exception_continuationB;
       push_exception_continuationB_return_59:
        *((long *) (RSstackS)) = arg2_375X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 78 : {
      RSstackS = (4 + (RSstackS));
      arg2_606X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_606X | (RSvalS))))) {
        x_381X = RSvalS;
        value_380X = ((arg2_606X)>>2);
        count_379X = ((x_381X)>>2);
        if ((count_379X < 0)) {
          PS_SHIFT_RIGHT(value_380X, (0 - count_379X), x_607X)
          RSvalS = (((x_607X)<<2));
          goto L11161;}
        else {
          PS_SHIFT_LEFT(value_380X, count_379X, x_378X)
          result_377X = (((((x_378X)<<2)))>>2);
          PS_SHIFT_RIGHT(result_377X, count_379X, x_376X)
          if ((value_380X == x_376X)) {
            if ((value_380X < 0)) {
              if ((result_377X < 0)) {
                goto L28532;}
              else {
                merged_arg1K0 = 0;
                merged_arg1K1 = 6;
                push_exception_continuationB_return_tag = 60;
                goto push_exception_continuationB;
               push_exception_continuationB_return_60:
                *((long *) (RSstackS)) = arg2_606X;
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = x_381X;
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 2;
                arg1K1 = 0;
                goto L12977;}}
            else {
              if ((result_377X < 0)) {
                merged_arg1K0 = 0;
                merged_arg1K1 = 6;
                push_exception_continuationB_return_tag = 61;
                goto push_exception_continuationB;
               push_exception_continuationB_return_61:
                *((long *) (RSstackS)) = arg2_606X;
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = x_381X;
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 2;
                arg1K1 = 0;
                goto L12977;}
              else {
                goto L28532;}}}
          else {
            merged_arg1K0 = 0;
            merged_arg1K1 = 6;
            push_exception_continuationB_return_tag = 62;
            goto push_exception_continuationB;
           push_exception_continuationB_return_62:
            *((long *) (RSstackS)) = arg2_606X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_381X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12977;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 63;
        goto push_exception_continuationB;
       push_exception_continuationB_return_63:
        *((long *) (RSstackS)) = arg2_606X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 79 : {
      x_605X = RSvalS;
      if ((9 == (255 & x_605X))) {
        arg1K0 = 5;
        goto L30259;}
      else {
        arg1K0 = 1;
        goto L30259;}}
      break;
    case 80 : {
      RSstackS = (4 + (RSstackS));
      arg2_603X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_603X))) {
        if ((9 == (255 & (RSvalS)))) {
          x_604X = RSvalS;
          if ((arg2_603X == x_604X)) {
            arg1K0 = 5;
            goto L23536;}
          else {
            arg1K0 = 1;
            goto L23536;}}
        else {
          goto L23509;}}
      else {
        goto L23509;}}
      break;
    case 81 : {
      RSstackS = (4 + (RSstackS));
      arg2_601X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_601X))) {
        if ((9 == (255 & (RSvalS)))) {
          x_602X = RSvalS;
          if ((arg2_601X < x_602X)) {
            arg1K0 = 5;
            goto L23447;}
          else {
            arg1K0 = 1;
            goto L23447;}}
        else {
          goto L23420;}}
      else {
        goto L23420;}}
      break;
    case 82 : {
      if ((9 == (255 & (RSvalS)))) {
        RSvalS = ((((((unsigned long) ((((RSvalS))>>8)))))<<2));
        goto L11161;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 64;
        goto push_exception_continuationB;
       push_exception_continuationB_return_64:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 83 : {
      if ((0 == (3 & (RSvalS)))) {
        x_382X = (((RSvalS))>>2);
        if ((255 < x_382X)) {
          goto L23302;}
        else {
          if ((x_382X < 0)) {
            goto L23302;}
          else {
            RSvalS = (9 + ((((((unsigned long) x_382X)))<<8)));
            goto L11161;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 65;
        goto push_exception_continuationB;
       push_exception_continuationB_return_65:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 84 : {
      x_600X = RSvalS;
      if ((21 == x_600X)) {
        arg1K0 = 5;
        goto L30286;}
      else {
        arg1K0 = 1;
        goto L30286;}}
      break;
    case 85 : {
      x_384X = RSvalS;
      b_383X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_384X))) {
        if (((31 & ((((*((long *) (-7 + x_384X))))>>2))) == (((unsigned long) b_383X)))) {
          arg1K0 = 5;
          goto L30300;}
        else {
          arg1K0 = 1;
          goto L30300;}}
      else {
        arg1K0 = 1;
        goto L30300;}}
      break;
    case 86 : {
      x_598X = RSvalS;
      b_599X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_598X))) {
        if (((31 & ((((*((long *) (-7 + x_598X))))>>2))) == (((unsigned long) b_599X)))) {
          RSvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) (-7 + x_598X))))>>8))));
          goto L11161;}
        else {
          goto L23108;}}
      else {
        goto L23108;}}
      break;
    case 87 : {
      space_387X = 4 + ((((((unsigned long) (*((unsigned char *) (RScode_pointerS))))))<<2));
      v_386X = AVAILABLEp(space_387X);
      if (v_386X) {
        goto L22941;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 5;
        goto collect_saving_temp;
       collect_saving_temp_return_5:
        okayP_385X = AVAILABLEp(space_387X);
        if (okayP_385X) {
          goto L22941;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L22941;}}}
      break;
    case 88 : {
      len_594X = SnargsS;
      space_595X = 4 + (((len_594X)<<2));
      v_596X = AVAILABLEp(space_595X);
      if (v_596X) {
        goto L22836;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 6;
        goto collect_saving_temp;
       collect_saving_temp_return_6:
        okayP_597X = AVAILABLEp(space_595X);
        if (okayP_597X) {
          goto L22836;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L22836;}}}
      break;
    case 89 : {
      x_390X = RSvalS;
      b_389X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_388X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_390X))) {
        if (((31 & ((((*((long *) (-7 + x_390X))))>>2))) == (((unsigned long) b_389X)))) {
          RSvalS = (*((long *) ((-3 + x_390X) + ((((((unsigned long) b_388X)))<<2)))));
          goto L11161;}
        else {
          goto L22736;}}
      else {
        goto L22736;}}
      break;
    case 90 : {
      RSstackS = (4 + (RSstackS));
      arg2_589X = *((long *) (RSstackS));
      x_590X = RSvalS;
      b_591X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_592X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & arg2_589X))) {
        if (((31 & ((((*((long *) (-7 + arg2_589X))))>>2))) == (((unsigned long) b_591X)))) {
          if ((3 == (3 & arg2_589X))) {
            if ((0 == (128 & (*((long *) (-7 + arg2_589X)))))) {
              addr_593X = (-3 + arg2_589X) + ((((((unsigned long) b_592X)))<<2));
              WRITE_BARRIER(addr_593X, x_590X);
              *((long *) addr_593X) = x_590X;
              RSvalS = 13;
              goto L11161;}
            else {
              goto L22521;}}
          else {
            goto L22521;}}
        else {
          goto L22521;}}
      else {
        goto L22521;}}
      break;
    case 91 : {
      RSstackS = (4 + (RSstackS));
      arg2_393X = *((long *) (RSstackS));
      x_392X = RSvalS;
      b_391X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((0 == (3 & arg2_393X))) {
        len_584X = ((arg2_393X)>>2);
        size_585X = 1 + len_584X;
        if ((len_584X < 0)) {
          goto L22196;}
        else {
          if ((4194305 < size_585X)) {
            goto L22196;}
          else {
            v_586X = AVAILABLEp(size_585X);
            if (v_586X) {
              arg2K0 = 1;
              arg0K1 = x_392X;
              goto L22214;}
            else {
              merged_arg0K0 = x_392X;
              collect_saving_temp_return_tag = 7;
              goto collect_saving_temp;
             collect_saving_temp_return_7:
              temp_587X = collect_saving_temp0_return_value;
              v_588X = AVAILABLEp(size_585X);
              arg2K0 = v_588X;
              arg0K1 = temp_587X;
              goto L22214;}}}}
      else {
        merged_arg1K0 = 1;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 66;
        goto push_exception_continuationB;
       push_exception_continuationB_return_66:
        *((long *) (RSstackS)) = ((((((unsigned long) b_391X)))<<2));
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg2_393X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_392X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 3;
        arg1K1 = 1;
        goto L12977;}}
      break;
    case 92 : {
      RSstackS = (4 + (RSstackS));
      arg2_396X = *((long *) (RSstackS));
      x_395X = RSvalS;
      b_394X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((0 == (3 & x_395X))) {
        if ((3 == (3 & arg2_396X))) {
          if (((31 & ((((*((long *) (-7 + arg2_396X))))>>2))) == (((unsigned long) b_394X)))) {
            len_582X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg2_396X))))>>8))))>>2);
            index_583X = ((x_395X)>>2);
            if ((index_583X < 0)) {
              goto L21933;}
            else {
              if ((index_583X < len_582X)) {
                RSvalS = (*((long *) ((-3 + arg2_396X) + (-4 & x_395X))));
                goto L11161;}
              else {
                goto L21933;}}}
          else {
            goto L21910;}}
        else {
          goto L21910;}}
      else {
        goto L21910;}}
      break;
    case 93 : {
      RSstackS = (4 + (RSstackS));
      arg2_401X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_400X = *((long *) (RSstackS));
      x_399X = RSvalS;
      b_398X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((0 == (3 & arg2_401X))) {
        if ((3 == (3 & arg3_400X))) {
          if (((31 & ((((*((long *) (-7 + arg3_400X))))>>2))) == (((unsigned long) b_398X)))) {
            if ((3 == (3 & arg3_400X))) {
              if ((0 == (128 & (*((long *) (-7 + arg3_400X)))))) {
                len_580X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg3_400X))))>>8))))>>2);
                index_581X = ((arg2_401X)>>2);
                if ((index_581X < 0)) {
                  goto L21574;}
                else {
                  if ((index_581X < len_580X)) {
                    addr_397X = (-3 + arg3_400X) + (-4 & arg2_401X);
                    WRITE_BARRIER(addr_397X, x_399X);
                    *((long *) addr_397X) = x_399X;
                    RSvalS = 13;
                    goto L11161;}
                  else {
                    goto L21574;}}}
              else {
                goto L21549;}}
            else {
              goto L21549;}}
          else {
            goto L21549;}}
        else {
          goto L21549;}}
      else {
        goto L21549;}}
      break;
    case 94 : {
      RSstackS = (4 + (RSstackS));
      arg2_402X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_402X | (RSvalS))))) {
        len_575X = ((arg2_402X)>>2);
        init_576X = (((RSvalS))>>2);
        size_577X = 1 + ((((3 + len_575X))>>2));
        if ((len_575X < 0)) {
          goto L21280;}
        else {
          if ((4194305 < size_577X)) {
            goto L21280;}
          else {
            v_578X = AVAILABLEp(size_577X);
            if (v_578X) {
              goto L21373;}
            else {
              merged_arg0K0 = 0;
              collect_saving_temp_return_tag = 8;
              goto collect_saving_temp;
             collect_saving_temp_return_8:
              okayP_579X = AVAILABLEp(size_577X);
              if (okayP_579X) {
                goto L21373;}
              else {
                merged_arg1K0 = 0;
                merged_arg1K1 = 8;
                push_exception_continuationB_return_tag = 67;
                goto push_exception_continuationB;
               push_exception_continuationB_return_67:
                *((long *) (RSstackS)) = (((len_575X)<<2));
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = (((init_576X)<<2));
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 2;
                arg1K1 = 0;
                goto L12977;}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 68;
        goto push_exception_continuationB;
       push_exception_continuationB_return_68:
        *((long *) (RSstackS)) = arg2_402X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 95 : {
      obj_574X = RSvalS;
      if ((3 == (3 & obj_574X))) {
        if ((17 == (31 & ((((*((long *) (-7 + obj_574X))))>>2))))) {
          RSvalS = (((((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8)))<<2));
          goto L11161;}
        else {
          goto L30343;}}
      else {
        goto L30343;}}
      break;
    case 96 : {
      RSstackS = (4 + (RSstackS));
      arg2_573X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_573X))) {
        if ((17 == (31 & ((((*((long *) (-7 + arg2_573X))))>>2))))) {
          if ((0 == (3 & (RSvalS)))) {
            index_404X = (((RSvalS))>>2);
            len_403X = (long)(((unsigned long)(*((long *) (-7 + arg2_573X))))>>8);
            if ((index_404X < 0)) {
              goto L21147;}
            else {
              if ((index_404X < len_403X)) {
                RSvalS = ((((((unsigned long) (*((unsigned char *) ((-3 + arg2_573X) + index_404X))))))<<2));
                goto L11161;}
              else {
                goto L21147;}}}
          else {
            goto L27845;}}
        else {
          goto L27845;}}
      else {
        goto L27845;}}
      break;
    case 97 : {
      RSstackS = (4 + (RSstackS));
      arg2_571X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_572X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_572X))) {
        if ((17 == (31 & ((((*((long *) (-7 + arg3_572X))))>>2))))) {
          if ((0 == (3 & (arg2_571X | (RSvalS))))) {
            index_407X = ((arg2_571X)>>2);
            Kchar_406X = (((RSvalS))>>2);
            if ((3 == (3 & arg3_572X))) {
              if ((0 == (128 & (*((long *) (-7 + arg3_572X)))))) {
                len_405X = (long)(((unsigned long)(*((long *) (-7 + arg3_572X))))>>8);
                if ((index_407X < 0)) {
                  goto L21009;}
                else {
                  if ((index_407X < len_405X)) {
                    *((unsigned char *) ((-3 + arg3_572X) + index_407X)) = Kchar_406X;
                    RSvalS = 13;
                    goto L11161;}
                  else {
                    goto L21009;}}}
              else {
                goto L20988;}}
            else {
              goto L20988;}}
          else {
            goto L27729;}}
        else {
          goto L27729;}}
      else {
        goto L27729;}}
      break;
    case 98 : {
      RSstackS = (4 + (RSstackS));
      arg2_413X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_413X))) {
        if ((9 == (255 & (RSvalS)))) {
          len_412X = ((arg2_413X)>>2);
          init_411X = (((RSvalS))>>8);
          size_410X = 1 + ((((4 + len_412X))>>2));
          if ((len_412X < 0)) {
            goto L20771;}
          else {
            if ((4194305 < size_410X)) {
              goto L20771;}
            else {
              v_409X = AVAILABLEp(size_410X);
              if (v_409X) {
                goto L20864;}
              else {
                merged_arg0K0 = 0;
                collect_saving_temp_return_tag = 9;
                goto collect_saving_temp;
               collect_saving_temp_return_9:
                okayP_408X = AVAILABLEp(size_410X);
                if (okayP_408X) {
                  goto L20864;}
                else {
                  merged_arg1K0 = 0;
                  merged_arg1K1 = 8;
                  push_exception_continuationB_return_tag = 69;
                  goto push_exception_continuationB;
                 push_exception_continuationB_return_69:
                  *((long *) (RSstackS)) = (((len_412X)<<2));
                  RSstackS = (-4 + (RSstackS));
                  *((long *) (RSstackS)) = (9 + ((((((unsigned long) init_411X)))<<8)));
                  RSstackS = (-4 + (RSstackS));
                  arg1K0 = 2;
                  arg1K1 = 0;
                  goto L12977;}}}}}
        else {
          goto L27646;}}
      else {
        goto L27646;}}
      break;
    case 99 : {
      obj_570X = RSvalS;
      if ((3 == (3 & obj_570X))) {
        if ((16 == (31 & ((((*((long *) (-7 + obj_570X))))>>2))))) {
          RSvalS = (-4 + (((((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8)))<<2)));
          goto L11161;}
        else {
          goto L30366;}}
      else {
        goto L30366;}}
      break;
    case 100 : {
      RSstackS = (4 + (RSstackS));
      arg2_569X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_569X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg2_569X))))>>2))))) {
          if ((0 == (3 & (RSvalS)))) {
            index_415X = (((RSvalS))>>2);
            len_414X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg2_569X))))>>8));
            if ((index_415X < 0)) {
              goto L20638;}
            else {
              if ((index_415X < len_414X)) {
                RSvalS = (9 + ((((((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + arg2_569X) + index_415X))))))))<<8)));
                goto L11161;}
              else {
                goto L20638;}}}
          else {
            goto L27533;}}
        else {
          goto L27533;}}
      else {
        goto L27533;}}
      break;
    case 101 : {
      RSstackS = (4 + (RSstackS));
      arg2_420X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_419X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_419X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg3_419X))))>>2))))) {
          if ((0 == (3 & arg2_420X))) {
            if ((9 == (255 & (RSvalS)))) {
              index_418X = ((arg2_420X)>>2);
              Kchar_417X = (((RSvalS))>>8);
              if ((3 == (3 & arg3_419X))) {
                if ((0 == (128 & (*((long *) (-7 + arg3_419X)))))) {
                  len_416X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg3_419X))))>>8));
                  if ((index_418X < 0)) {
                    goto L20500;}
                  else {
                    if ((index_418X < len_416X)) {
                      *((unsigned char *) ((-3 + arg3_419X) + index_418X)) = (((unsigned long) Kchar_417X));
                      RSvalS = 13;
                      goto L11161;}
                    else {
                      goto L20500;}}}
                else {
                  goto L20479;}}
              else {
                goto L20479;}}
            else {
              goto L27417;}}
          else {
            goto L27417;}}
        else {
          goto L27417;}}
      else {
        goto L27417;}}
      break;
    case 102 : {
      obj_566X = RSvalS;
      if ((3 == (3 & obj_566X))) {
        if ((4 == (31 & ((((*((long *) (-7 + obj_566X))))>>2))))) {
          x_567X = RSvalS;
          descriptor_568X = *((long *) (1 + x_567X));
          if ((17 == (255 & descriptor_568X))) {
            if ((529 == (*((long *) (1 + x_567X))))) {
              arg1K0 = 5;
              goto L20406;}
            else {
              arg1K0 = 1;
              goto L20406;}}
          else {
            arg1K0 = 5;
            goto L20406;}}
        else {
          goto L20378;}}
      else {
        goto L20378;}}
      break;
    case 103 : {
      RSstackS = (4 + (RSstackS));
      arg2_422X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_422X))) {
        if ((4 == (31 & ((((*((long *) (-7 + arg2_422X))))>>2))))) {
          x_421X = RSvalS;
          if ((1 == x_421X)) {
            goto L20169;}
          else {
            if ((5 == x_421X)) {
              goto L20169;}
            else {
              goto L20195;}}}
        else {
          goto L20195;}}
      else {
        goto L20195;}}
      break;
    case 104 : {
      x_565X = RSvalS;
      if ((3 == (3 & x_565X))) {
        if ((0 == (128 & (*((long *) (-7 + x_565X)))))) {
          arg1K0 = 1;
          goto L30392;}
        else {
          arg1K0 = 5;
          goto L30392;}}
      else {
        arg1K0 = 5;
        goto L30392;}}
      break;
    case 105 : {
      x_423X = RSvalS;
      if ((3 == (3 & x_423X))) {
        if ((0 == (128 & (*((long *) (-7 + x_423X)))))) {
          *((long *) (-7 + x_423X)) = (128 | (*((long *) (-7 + x_423X))));
          goto L30404;}
        else {
          goto L30404;}}
      else {
        goto L30404;}}
      break;
    case 106 : {
      v_563X = AVAILABLEp(6);
      if (v_563X) {
        goto L19543;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 10;
        goto collect_saving_temp;
       collect_saving_temp_return_10:
        okayP_564X = AVAILABLEp(6);
        if (okayP_564X) {
          goto L19543;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L19543;}}}
      break;
    case 107 : {
      obj_558X = RSvalS;
      if ((3 == (3 & obj_558X))) {
        if ((5 == (31 & ((((*((long *) (-7 + obj_558X))))>>2))))) {
          x_559X = RSvalS;
          if ((0 == (*((long *) (-3 + x_559X))))) {
            merged_arg1K0 = 0;
            merged_arg1K1 = 5;
            push_exception_continuationB_return_tag = 70;
            goto push_exception_continuationB;
           push_exception_continuationB_return_70:
            *((long *) (RSstackS)) = x_559X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 1;
            arg1K1 = 0;
            goto L12977;}
          else {
            os_index_560X = (((*((long *) (5 + x_559X))))>>2);
            v_561X = *((long *) (-3 + x_559X));
            if ((4 == v_561X)) {
              goto L19452;}
            else {
              if ((12 == (*((long *) (-3 + x_559X))))) {
                goto L19452;}
              else {
                v_562X = ps_close_fd(os_index_560X);
                arg0K0 = v_562X;
                goto L19441;}}}}
        else {
          goto L19415;}}
      else {
        goto L19415;}}
      break;
    case 108 : {
      RSstackS = (4 + (RSstackS));
      arg2_554X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_555X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg4_556X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg5_557X = *((long *) (RSstackS));
      if ((0 == (3 & (arg4_556X | arg3_555X)))) {
        if ((1 == arg2_554X)) {
          goto L27179;}
        else {
          if ((5 == arg2_554X)) {
            goto L27179;}
          else {
            goto L27199;}}}
      else {
        goto L27199;}}
      break;
    case 109 : {
      RSstackS = (4 + (RSstackS));
      arg2_426X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_425X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg4_424X = *((long *) (RSstackS));
      if ((0 == (3 & (arg3_425X | arg2_426X)))) {
        obj_549X = RSvalS;
        if ((3 == (3 & obj_549X))) {
          if ((5 == (31 & ((((*((long *) (-7 + obj_549X))))>>2))))) {
            start_550X = ((arg3_425X)>>2);
            count_551X = ((arg2_426X)>>2);
            channel_552X = RSvalS;
            v_553X = 8 == (*((long *) (-3 + channel_552X)));
            if (v_553X) {
              if ((3 == (3 & arg4_424X))) {
                if ((16 == (31 & ((((*((long *) (-7 + arg4_424X))))>>2))))) {
                  goto L18887;}
                else {
                  goto L18879;}}
              else {
                goto L18879;}}
            else {
              arg1K0 = 5;
              goto L18816;}}
          else {
            goto L27052;}}
        else {
          goto L27052;}}
      else {
        goto L27052;}}
      break;
    case 110 : {
      obj_429X = RSvalS;
      if ((3 == (3 & obj_429X))) {
        if ((5 == (31 & ((((*((long *) (-7 + obj_429X))))>>2))))) {
          channel_428X = RSvalS;
          head_427X = Spending_channels_headS;
          if ((1 == head_427X)) {
            n_548X = ps_abort_fd_op((*((long *) (5 + channel_428X))));
            arg0K0 = (((n_548X)<<2));
            goto L30439;}
          else {
            if ((channel_428X == head_427X)) {
              channel_545X = Spending_channels_headS;
              next_546X = *((long *) (9 + channel_545X));
              Spending_channels_headS = next_546X;
              addr_547X = 9 + channel_545X;
              WRITE_BARRIER(addr_547X, 1);
              *((long *) addr_547X) = 1;
              if ((1 == next_546X)) {
                Spending_channels_tailS = 1;
                goto L6617;}
              else {
                goto L6617;}}
            else {
              arg0K0 = (*((long *) (9 + head_427X)));
              arg0K1 = head_427X;
              goto L6623;}}}
        else {
          goto L30426;}}
      else {
        goto L30426;}}
      break;
    case 111 : {
      RSvalS = 529;
      goto L11161;}
      break;
    case 112 : {
      RSvalS = 13;
      goto L11161;}
      break;
    case 113 : {
      x_544X = RSvalS;
      merged_arg1K0 = 0;
      merged_arg1K1 = 16;
      push_exception_continuationB_return_tag = 71;
      goto push_exception_continuationB;
     push_exception_continuationB_return_71:
      *((long *) (RSstackS)) = x_544X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12977;}
      break;
    case 114 : {
      RSvalS = 1;
      goto L11161;}
      break;
    case 115 : {
      RSvalS = 21;
      goto L11161;}
      break;
    case 116 : {
      RSstackS = (4 + (RSstackS));
      arg2_532X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_533X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_533X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg3_533X))))>>2))))) {
          obj_534X = RSvalS;
          if ((3 == (3 & obj_534X))) {
            if ((16 == (31 & ((((*((long *) (-7 + obj_534X))))>>2))))) {
              x_535X = RSvalS;
              x_536X = image_writing_okayP();
              if (x_536X) {
                port_430X = ps_open_output_file((((unsigned char *)(-3 + arg3_533X))), &status_431X);
                if ((status_431X == NO_ERRORS)) {
                  status_537X = ps_write_string((((unsigned char *)(-3 + x_535X))), port_430X);
                  if ((status_537X == NO_ERRORS)) {
                    status_540X = write_image(arg2_532X, port_430X, ((void(*)(void))mark_traced_channels_closedB));
                    if ((status_540X == NO_ERRORS)) {
                      status_543X = ps_close(port_430X);
                      if ((status_543X == NO_ERRORS)) {
                        RSvalS = 13;
                        goto L11161;}
                      else {
                        arg1K0 = 21;
                        goto L18358;}}
                    else {
                      status_541X = ps_close(port_430X);
                      if ((status_541X == NO_ERRORS)) {
                        arg1K0 = 21;
                        goto L18358;}
                      else {
                        ps_write_string(((unsigned char *) "Unable to close image file"), (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg1K0 = 21;
                        goto L18358;}}}
                  else {
                    status_538X = ps_close(port_430X);
                    if ((status_538X == NO_ERRORS)) {
                      arg1K0 = 21;
                      goto L18358;}
                    else {
                      ps_write_string(((unsigned char *) "Unable to close image file"), (stderr));
                      { long ignoreXX;
                      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                      arg1K0 = 21;
                      goto L18358;}}}
                else {
                  arg1K0 = 10;
                  goto L18358;}}
              else {
                arg1K0 = 15;
                goto L18358;}}
            else {
              goto L18421;}}
          else {
            goto L18421;}}
        else {
          goto L18421;}}
      else {
        goto L18421;}}
      break;
    case 117 : {
      RSvalS = 13;
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 11;
      goto collect_saving_temp;
     collect_saving_temp_return_11:
      RSvalS = 13;
      goto L11161;}
      break;
    case 118 : {
      v_530X = AVAILABLEp(6);
      if (v_530X) {
        goto L18091;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 12;
        goto collect_saving_temp;
       collect_saving_temp_return_12:
        okayP_531X = AVAILABLEp(6);
        if (okayP_531X) {
          goto L18091;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L18091;}}}
      break;
    case 119 : {
      RSstackS = (4 + (RSstackS));
      arg2_528X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_528X))) {
        x_435X = RSvalS;
        key_434X = ((arg2_528X)>>2);
        if ((5 == key_434X)) {
          RSvalS = (-4 & x_435X);
          goto L11161;}
        else {
          if ((0 == key_434X)) {
            n_529X = available();
            RSvalS = (((n_529X)<<2));
            goto L11161;}
          else {
            if ((1 == key_434X)) {
              bytes_432X = heap_size();
              RSvalS = (-4 & (3 + bytes_432X));
              goto L11161;}
            else {
              if ((2 == key_434X)) {
                RSvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                goto L11161;}
              else {
                if ((3 == key_434X)) {
                  n_433X = gc_count();
                  RSvalS = (((n_433X)<<2));
                  goto L11161;}
                else {
                  if ((4 == key_434X)) {
                    merged_arg1K0 = 0;
                    merged_arg1K1 = 15;
                    push_exception_continuationB_return_tag = 72;
                    goto push_exception_continuationB;
                   push_exception_continuationB_return_72:
                    *((long *) (RSstackS)) = (((key_434X)<<2));
                    RSstackS = (-4 + (RSstackS));
                    *((long *) (RSstackS)) = x_435X;
                    RSstackS = (-4 + (RSstackS));
                    arg1K0 = 2;
                    arg1K1 = 0;
                    goto L12977;}
                  else {
                    merged_arg1K0 = 0;
                    merged_arg1K1 = 17;
                    push_exception_continuationB_return_tag = 73;
                    goto push_exception_continuationB;
                   push_exception_continuationB_return_73:
                    *((long *) (RSstackS)) = (((key_434X)<<2));
                    RSstackS = (-4 + (RSstackS));
                    *((long *) (RSstackS)) = x_435X;
                    RSstackS = (-4 + (RSstackS));
                    arg1K0 = 2;
                    arg1K1 = 0;
                    goto L12977;}}}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 74;
        goto push_exception_continuationB;
       push_exception_continuationB_return_74:
        *((long *) (RSstackS)) = arg2_528X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 120 : {
      if ((0 == (3 & (RSvalS)))) {
        type_436X = (((RSvalS))>>2);
        arg2K0 = 1;
        goto L17809;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 75;
        goto push_exception_continuationB;
       push_exception_continuationB_return_75:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 121 : {
      obj_438X = RSvalS;
      if ((3 == (3 & obj_438X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_438X))))>>2))))) {
          x_437X = RSvalS;
          arg0K0 = x_437X;
          arg2K1 = 1;
          goto L26339;}
        else {
          goto L26361;}}
      else {
        goto L26361;}}
      break;
    case 122 : {
      x_527X = RSvalS;
      arg2K0 = 1;
      arg0K1 = x_527X;
      goto L30487;}
      break;
    case 123 : {
      RSvalS = (Scurrent_threadS);
      goto L11161;}
      break;
    case 124 : {
      Scurrent_threadS = (RSvalS);
      RSvalS = 13;
      goto L11161;}
      break;
    case 125 : {
      RSvalS = (Ssession_dataS);
      goto L11161;}
      break;
    case 126 : {
      Ssession_dataS = (RSvalS);
      RSvalS = 13;
      goto L11161;}
      break;
    case 127 : {
      obj_525X = RSvalS;
      if ((3 == (3 & obj_525X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_525X))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8))))>>2)) < 149)) {
            goto L13397;}
          else {
            temp_526X = Sexception_handlersS;
            Sexception_handlersS = (RSvalS);
            RSvalS = temp_526X;
            goto L11161;}}
        else {
          goto L13397;}}
      else {
        goto L13397;}}
      break;
    case 128 : {
      obj_523X = RSvalS;
      if ((3 == (3 & obj_523X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_523X))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8))))>>2)) < 5)) {
            goto L13297;}
          else {
            temp_524X = Sinterrupt_handlersS;
            Sinterrupt_handlersS = (RSvalS);
            RSvalS = temp_524X;
            goto L11161;}}
        else {
          goto L13297;}}
      else {
        goto L13297;}}
      break;
    case 129 : {
      old_440X = Senabled_interruptsS;
      p_439X = RSvalS;
      Senabled_interruptsS = (((p_439X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        Spending_interruptPS = 0;
        if ((Spending_eventsPS)) {
          Spending_interruptPS = 1;
          goto L30523;}
        else {
          goto L30523;}}
      else {
        Spending_interruptPS = 1;
        goto L30523;}}
      break;
    case 130 : {
      RSstackS = (4 + (RSstackS));
      p_522X = *((long *) (RSstackS));
      Senabled_interruptsS = (((p_522X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        Spending_interruptPS = 0;
        if ((Spending_eventsPS)) {
          Spending_interruptPS = 1;
          goto L11179;}
        else {
          goto L11179;}}
      else {
        Spending_interruptPS = 1;
        goto L11179;}}
      break;
    case 131 : {
      if ((0 == (3 & (RSvalS)))) {
        p_441X = RSvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          Spending_interruptPS = 0;
          if ((Spending_eventsPS)) {
            Spending_interruptPS = 1;
            goto L17581;}
          else {
            goto L17581;}}
        else {
          Spending_interruptPS = 1;
          goto L17581;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 76;
        goto push_exception_continuationB;
       push_exception_continuationB_return_76:
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 132 : {
      RSstackS = (4 + (RSstackS));
      arg2_521X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_521X))) {
        x_442X = RSvalS;
        if ((1 == x_442X)) {
          goto L17417;}
        else {
          if ((5 == x_442X)) {
            goto L17417;}
          else {
            goto L17437;}}}
      else {
        goto L17437;}}
      break;
    case 133 : {
      obj_516X = RSvalS;
      if ((3 == (3 & obj_516X))) {
        if ((13 == (31 & ((((*((long *) (-7 + obj_516X))))>>2))))) {
          x_517X = RSvalS;
          value_518X = *((long *) (1 + x_517X));
          name_519X = *((long *) (-3 + x_517X));
          if ((3 == (3 & name_519X))) {
            if ((16 == (31 & ((((*((long *) (-7 + name_519X))))>>2))))) {
              if ((3 == (3 & value_518X))) {
                if ((17 == (31 & ((((*((long *) (-7 + value_518X))))>>2))))) {
                  v_520X = lookup_external_name((-3 + name_519X), (-3 + value_518X));
                  if (v_520X) {
                    RSvalS = 13;
                    goto L11161;}
                  else {
                    goto L17262;}}
                else {
                  goto L17262;}}
              else {
                goto L17262;}}
            else {
              goto L17262;}}
          else {
            goto L17262;}}
        else {
          goto L17267;}}
      else {
        goto L17267;}}
      break;
    case 134 : {
      proc_443X = *((long *) ((RSstackS) + ((((SnargsS))<<2))));
      if ((3 == (3 & proc_443X))) {
        if ((13 == (31 & ((((*((long *) (-7 + proc_443X))))>>2))))) {
          SnargsS = (-1 + (SnargsS));
          RSvalS = proc_443X;
          arg1K0 = 1;
          goto L12103;}
        else {
          goto L30545;}}
      else {
        goto L30545;}}
      break;
    case 135 : {
      RSstackS = (4 + (RSstackS));
      arg2_510X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_510X))) {
        x_445X = RSvalS;
        option_444X = ((arg2_510X)>>2);
        if ((2 == option_444X)) {
          n_515X = CHEAP_TIME();
          RSvalS = (((n_515X)<<2));
          goto L11161;}
        else {
          if ((0 == option_444X)) {
            seconds_514X = run_time(&mseconds_513X);
            if ((536869 < seconds_514X)) {
              merged_arg1K0 = 0;
              merged_arg1K1 = 6;
              push_exception_continuationB_return_tag = 77;
              goto push_exception_continuationB;
             push_exception_continuationB_return_77:
              *((long *) (RSstackS)) = (((option_444X)<<2));
              RSstackS = (-4 + (RSstackS));
              *((long *) (RSstackS)) = (((seconds_514X)<<2));
              RSstackS = (-4 + (RSstackS));
              *((long *) (RSstackS)) = (((mseconds_513X)<<2));
              RSstackS = (-4 + (RSstackS));
              arg1K0 = 3;
              arg1K1 = 0;
              goto L12977;}
            else {
              RSvalS = (((((1000 * seconds_514X) + mseconds_513X))<<2));
              goto L11161;}}
          else {
            if ((1 == option_444X)) {
              seconds_512X = real_time(&mseconds_511X);
              if ((536869 < seconds_512X)) {
                merged_arg1K0 = 0;
                merged_arg1K1 = 6;
                push_exception_continuationB_return_tag = 78;
                goto push_exception_continuationB;
               push_exception_continuationB_return_78:
                *((long *) (RSstackS)) = (((option_444X)<<2));
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = (((seconds_512X)<<2));
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = (((mseconds_511X)<<2));
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 3;
                arg1K1 = 0;
                goto L12977;}
              else {
                RSvalS = (((((1000 * seconds_512X) + mseconds_511X))<<2));
                goto L11161;}}
            else {
              merged_arg1K0 = 0;
              merged_arg1K1 = 17;
              push_exception_continuationB_return_tag = 79;
              goto push_exception_continuationB;
             push_exception_continuationB_return_79:
              *((long *) (RSstackS)) = (((option_444X)<<2));
              RSstackS = (-4 + (RSstackS));
              *((long *) (RSstackS)) = x_445X;
              RSstackS = (-4 + (RSstackS));
              arg1K0 = 2;
              arg1K1 = 0;
              goto L12977;}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 80;
        goto push_exception_continuationB;
       push_exception_continuationB_return_80:
        *((long *) (RSstackS)) = arg2_510X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 136 : {
      RSstackS = (4 + (RSstackS));
      arg2_509X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_509X))) {
        x_448X = RSvalS;
        key_447X = ((arg2_509X)>>2);
        status_446X = extended_vm(key_447X, x_448X);
        if ((0 == status_446X)) {
          RSvalS = (Sextension_valueS);
          goto L11161;}
        else {
          if ((1 == status_446X)) {
            merged_arg1K0 = 0;
            merged_arg1K1 = 19;
            push_exception_continuationB_return_tag = 81;
            goto push_exception_continuationB;
           push_exception_continuationB_return_81:
            *((long *) (RSstackS)) = (((key_447X)<<2));
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_448X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12977;}
          else {
            merged_arg1K0 = 0;
            merged_arg1K1 = 20;
            push_exception_continuationB_return_tag = 82;
            goto push_exception_continuationB;
           push_exception_continuationB_return_82:
            *((long *) (RSstackS)) = (((key_447X)<<2));
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_448X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12977;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 83;
        goto push_exception_continuationB;
       push_exception_continuationB_return_83:
        *((long *) (RSstackS)) = arg2_509X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 137 : {
      RSstackS = (4 + (RSstackS));
      arg2_508X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_508X))) {
        RSvalS = (RSvalS);
        arg1K0 = 0;
        goto L12103;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 84;
        goto push_exception_continuationB;
       push_exception_continuationB_return_84:
        *((long *) (RSstackS)) = arg2_508X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 138 : {
      RSstackS = (4 + (RSstackS));
      arg2_452X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_452X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg2_452X))))>>2))))) {
          obj_451X = RSvalS;
          if ((3 == (3 & obj_451X))) {
            if ((16 == (31 & ((((*((long *) (-7 + obj_451X))))>>2))))) {
              x_450X = RSvalS;
              len_449X = (long)(((unsigned long)(*((long *) (-7 + arg2_452X))))>>8);
              if ((len_449X == ((long)(((unsigned long)(*((long *) (-7 + x_450X))))>>8)))) {
                if (((!memcmp((void *)(-3 + x_450X), (void *)(-3 + arg2_452X),len_449X)))) {
                  arg1K0 = 5;
                  goto L16840;}
                else {
                  arg1K0 = 1;
                  goto L16840;}}
              else {
                arg1K0 = 1;
                goto L16840;}}
            else {
              goto L16807;}}
          else {
            goto L16807;}}
        else {
          goto L16807;}}
      else {
        goto L16807;}}
      break;
    case 139 : {
      obj_505X = RSvalS;
      if ((3 == (3 & obj_505X))) {
        if ((16 == (31 & ((((*((long *) (-7 + obj_505X))))>>2))))) {
          x_506X = RSvalS;
          n_507X = -1 + ((long)(((unsigned long)(*((long *) (-7 + x_506X))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L16752;}
        else {
          goto L16729;}}
      else {
        goto L16729;}}
      break;
    case 140 : {
      space_455X = 1 + ((((4 + ((((RSvalS))>>2))))>>2));
      v_454X = AVAILABLEp(space_455X);
      if (v_454X) {
        goto L16470;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 13;
        goto collect_saving_temp;
       collect_saving_temp_return_13:
        okayP_453X = AVAILABLEp(space_455X);
        if (okayP_453X) {
          goto L16470;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L16470;}}}
      break;
    case 141 : {
      v_503X = AVAILABLEp(5);
      if (v_503X) {
        goto L16336;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 14;
        goto collect_saving_temp;
       collect_saving_temp_return_14:
        okayP_504X = AVAILABLEp(5);
        if (okayP_504X) {
          goto L16336;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L16336;}}}
      break;
    case 142 : {
      RSstackS = (4 + (RSstackS));
      arg2_457X = *((long *) (RSstackS));
      x_456X = RSvalS;
      arg0K0 = x_456X;
      arg0K1 = x_456X;
      arg2K2 = 1;
      goto L16081;}
      break;
    case 143 : {
      RSstackS = (4 + (RSstackS));
      arg2_500X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_501X = *((long *) (RSstackS));
      if ((0 == (3 & (RSvalS)))) {
        index_458X = (((RSvalS))>>2);
        if ((3 == (3 & arg3_501X))) {
          if ((8 == (31 & ((((*((long *) (-7 + arg3_501X))))>>2))))) {
            if ((arg2_500X == (*((long *) (-3 + arg3_501X))))) {
              len_502X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg3_501X))))>>8))))>>2);
              if ((index_458X < 0)) {
                goto L15834;}
              else {
                if ((index_458X < len_502X)) {
                  RSvalS = (*((long *) ((-3 + arg3_501X) + (((index_458X)<<2)))));
                  goto L11161;}
                else {
                  goto L15834;}}}
            else {
              goto L15881;}}
          else {
            goto L15881;}}
        else {
          goto L15881;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 85;
        goto push_exception_continuationB;
       push_exception_continuationB_return_85:
        *((long *) (RSstackS)) = arg3_501X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg2_500X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 3;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 144 : {
      RSstackS = (4 + (RSstackS));
      arg2_462X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_461X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg4_460X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_462X))) {
        x_497X = RSvalS;
        index_498X = ((arg2_462X)>>2);
        if ((3 == (3 & arg4_460X))) {
          if ((8 == (31 & ((((*((long *) (-7 + arg4_460X))))>>2))))) {
            if ((arg3_461X == (*((long *) (-3 + arg4_460X))))) {
              if ((3 == (3 & arg4_460X))) {
                if ((0 == (128 & (*((long *) (-7 + arg4_460X)))))) {
                  len_459X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg4_460X))))>>8))))>>2);
                  if ((index_498X < 0)) {
                    goto L15453;}
                  else {
                    if ((index_498X < len_459X)) {
                      addr_499X = (-3 + arg4_460X) + (((index_498X)<<2));
                      WRITE_BARRIER(addr_499X, x_497X);
                      *((long *) addr_499X) = x_497X;
                      RSvalS = 13;
                      goto L11161;}
                    else {
                      goto L15453;}}}
                else {
                  goto L15510;}}
              else {
                goto L15510;}}
            else {
              goto L15510;}}
          else {
            goto L15510;}}
        else {
          goto L15510;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 86;
        goto push_exception_continuationB;
       push_exception_continuationB_return_86:
        *((long *) (RSstackS)) = arg4_460X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg3_461X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg2_462X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 4;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 145 : {
      RSstackS = (4 + (RSstackS));
      arg2_466X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_465X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg4_464X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg5_463X = *((long *) (RSstackS));
      if ((0 == (3 & ((arg4_464X | arg2_466X) | (RSvalS))))) {
        from_index_494X = ((arg4_464X)>>2);
        to_index_495X = ((arg2_466X)>>2);
        count_496X = (((RSvalS))>>2);
        if ((3 == (3 & arg5_463X))) {
          if ((16 == (31 & ((((*((long *) (-7 + arg5_463X))))>>2))))) {
            goto L14793;}
          else {
            goto L14788;}}
        else {
          goto L14788;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 87;
        goto push_exception_continuationB;
       push_exception_continuationB_return_87:
        *((long *) (RSstackS)) = arg5_463X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg4_464X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg3_465X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg2_466X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 5;
        arg1K1 = 0;
        goto L12977;}}
      break;
    case 146 : {
      x_475X = RSvalS;
      if ((3 == (3 & x_475X))) {
        if ((6 == (31 & ((((*((long *) (-7 + x_475X))))>>2))))) {
          if ((0 == (4 & ((((*((long *) (1 + x_475X))))>>2))))) {
            goto L26758;}
          else {
            port_474X = RSvalS;
            p_473X = *((long *) (21 + port_474X));
            p_472X = *((long *) (25 + port_474X));
            b_471X = *((long *) (17 + port_474X));
            i_470X = ((p_473X)>>2);
            x_469X = *((long *) (9 + port_474X));
            if ((5 == x_469X)) {
              goto L14635;}
            else {
              if ((i_470X == (((p_472X)>>2)))) {
                goto L14635;}
              else {
                val_468X = 4 + (((i_470X)<<2));
                addr_467X = 21 + port_474X;
                WRITE_BARRIER(addr_467X, val_468X);
                *((long *) addr_467X) = val_468X;
                RSvalS = (9 + ((((((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + b_471X) + i_470X))))))))<<8)));
                goto L11161;}}}}
        else {
          goto L26758;}}
      else {
        goto L26758;}}
      break;
    case 147 : {
      x_482X = RSvalS;
      if ((3 == (3 & x_482X))) {
        if ((6 == (31 & ((((*((long *) (-7 + x_482X))))>>2))))) {
          if ((0 == (4 & ((((*((long *) (1 + x_482X))))>>2))))) {
            goto L26699;}
          else {
            port_481X = RSvalS;
            p_480X = *((long *) (21 + port_481X));
            p_479X = *((long *) (25 + port_481X));
            b_478X = *((long *) (17 + port_481X));
            i_477X = ((p_480X)>>2);
            x_476X = *((long *) (9 + port_481X));
            if ((5 == x_476X)) {
              goto L14532;}
            else {
              if ((i_477X == (((p_479X)>>2)))) {
                goto L14532;}
              else {
                RSvalS = (9 + ((((((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + b_478X) + i_477X))))))))<<8)));
                goto L11161;}}}}
        else {
          goto L26699;}}
      else {
        goto L26699;}}
      break;
    case 148 : {
      RSstackS = (4 + (RSstackS));
      arg2_493X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_493X))) {
        x_491X = RSvalS;
        if ((3 == (3 & x_491X))) {
          if ((6 == (31 & ((((*((long *) (-7 + x_491X))))>>2))))) {
            if ((0 == (8 & ((((*((long *) (1 + x_491X))))>>2))))) {
              goto L14345;}
            else {
              x_490X = RSvalS;
              Kchar_489X = ((arg2_493X)>>8);
              p_488X = *((long *) (21 + x_490X));
              b_487X = *((long *) (17 + x_490X));
              i_486X = ((p_488X)>>2);
              x_485X = *((long *) (9 + x_490X));
              if ((5 == x_485X)) {
                goto L14327;}
              else {
                if ((i_486X == ((long)(((unsigned long)(*((long *) (-7 + b_487X))))>>8)))) {
                  goto L14327;}
                else {
                  val_484X = 4 + (((i_486X)<<2));
                  addr_483X = 21 + x_490X;
                  WRITE_BARRIER(addr_483X, val_484X);
                  *((long *) addr_483X) = val_484X;
                  *((unsigned char *) ((-3 + (*((long *) (17 + x_490X)))) + i_486X)) = (((unsigned long) Kchar_489X));
                  RSvalS = 13;
                  goto L11161;}}}}
          else {
            goto L14345;}}
        else {
          goto L14345;}}
      else {
        goto L14345;}}
      break;
  }}
 L26441: {
  byte_args_702X = arg1K0;
  exception_703X = arg1K1;
  if ((64 == (SnargsS))) {
    RSstackS = (4 + (RSstackS));
    merged_arg0K0 = (*((long *) (RSstackS)));
    merged_arg0K1 = 63;
    pop_args_list_return_tag = 2;
    goto pop_args_list;
   pop_args_list_return_2:
    v_705X = pop_args_list0_return_value;
    arg0K0 = v_705X;
    goto L26453;}
  else {
    merged_arg0K0 = 25;
    merged_arg0K1 = (SnargsS);
    pop_args_list_return_tag = 3;
    goto pop_args_list;
   pop_args_list_return_3:
    v_704X = pop_args_list0_return_value;
    arg0K0 = v_704X;
    goto L26453;}}
 L12024: {
  b_218X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  *((long *) (RSstackS)) = (RSenvS);
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (1034 + ((((((unsigned long) b_218X)))<<10)));
  RSstackS = (-4 + (RSstackS));
  RSenvS = (11 + (RSstackS));
  goto L11161;}
 L12042: {
  v_217X = arg1K0;
  merged_arg1K0 = v_217X;
  copy_stack_into_heap_return_tag = 0;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_0:
  goto L12024;}
 L29715: {
  b_303X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  len_304X = 4 + ((((((unsigned long) b_303X)))<<2));
  addr_305X = ALLOCATE_SPACE(2, (4 + len_304X));
  *((long *) addr_305X) = (10 + (((len_304X)<<8)));
  stob_306X = 7 + addr_305X;
  x_307X = RSstackS;
  memcpy((void *)(1 + stob_306X), (void *)(4 + x_307X),((((((unsigned long) b_303X)))<<2)));
  RSstackS = ((RSstackS) - ((((0 - (((unsigned long) b_303X))))<<2)));
  value_308X = RSenvS;
  addr_309X = -3 + stob_306X;
  WRITE_BARRIER(addr_309X, value_308X);
  *((long *) addr_309X) = value_308X;
  RSenvS = stob_306X;
  goto L11161;}
 L11941: {
  args_219X = arg0K0;
  RSvalS = args_219X;
  SnargsS = (1 + (((unsigned long) b_665X)));
  goto L11161;}
 L26671: {
  env_58X = arg0K0;
  i_59X = arg0K1;
  if ((0 == i_59X)) {
    b_60X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    RSvalS = (*((long *) ((-3 + env_58X) + ((((((unsigned long) b_60X)))<<2)))));
    if ((529 == (RSvalS))) {
      merged_arg1K0 = 2;
      merged_arg1K1 = 0;
      push_exception_continuationB_return_tag = 88;
      goto push_exception_continuationB;
     push_exception_continuationB_return_88:
      arg1K0 = 0;
      arg1K1 = 2;
      goto L12977;}
    else {
      goto L11161;}}
  else {
    arg0K0 = (*((long *) (-3 + env_58X)));
    arg0K1 = (-1 + i_59X);
    goto L26671;}}
 L12977: {
  nargs_693X = arg1K0;
  args_694X = arg1K1;
  SnargsS = (2 + (((unsigned long) nargs_693X)));
  obj_695X = Sexception_handlersS;
  if ((3 == (3 & obj_695X))) {
    if ((2 == (31 & ((((*((long *) (-7 + obj_695X))))>>2))))) {
      goto L13038;}
    else {
      goto L13117;}}
  else {
    goto L13117;}}
 L11862: {
  env_220X = arg0K0;
  i_221X = arg0K1;
  if ((0 == i_221X)) {
    b_222X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    value_223X = RSvalS;
    addr_224X = (-3 + env_220X) + ((((((unsigned long) b_222X)))<<2));
    WRITE_BARRIER(addr_224X, value_223X);
    *((long *) addr_224X) = value_223X;
    RSvalS = 13;
    goto L11161;}
  else {
    arg0K0 = (*((long *) (-3 + env_220X)));
    arg0K1 = (-1 + i_221X);
    goto L11862;}}
 L11719: {
  space_237X = arg0K0;
  v_238X = AVAILABLEp(space_237X);
  if (v_238X) {
    arg1K0 = 0;
    goto L11721;}
  else {
    merged_arg0K0 = 0;
    collect_saving_temp_return_tag = 15;
    goto collect_saving_temp;
   collect_saving_temp_return_15:
    okayP_239X = AVAILABLEp(space_237X);
    if (okayP_239X) {
      arg1K0 = 0;
      goto L11721;}
    else {
      ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
      arg1K0 = 0;
      goto L11721;}}}
 L11559: {
  len_246X = (((((unsigned long) b_329X)))<<2);
  addr_247X = ALLOCATE_SPACE(2, (4 + len_246X));
  *((long *) addr_247X) = (10 + (((len_246X)<<8)));
  new_env_248X = 7 + addr_247X;
  arg0K0 = 0;
  goto L11569;}
 L11442: {
  key_258X = arg0K0;
  merged_arg0K0 = ((RScode_pointerS) + (((((((unsigned long) b_336X)))<<8)) + (((unsigned long) b_335X))));
  merged_arg0K1 = (((unsigned long) b_334X));
  merged_arg0K2 = key_258X;
  push_continuationB_return_tag = 0;
  goto push_continuationB;
 push_continuationB_return_0:
  goto L11161;}
 L11468: {
  v_257X = arg1K0;
  merged_arg1K0 = v_257X;
  copy_stack_into_heap_return_tag = 1;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_1:
  arg0K0 = 0;
  goto L11442;}
 L29815: {
  key_301X = arg1K0;
  if ((1 == (ScontS))) {
    arg0K0 = 1;
    goto L29817;}
  else {
    merged_arg1K0 = key_301X;
    merged_arg1K1 = 2;
    really_preserve_continuation_return_tag = 0;
    goto really_preserve_continuation;
   really_preserve_continuation_return_0:
    v_302X = really_preserve_continuation0_return_value;
    arg0K0 = v_302X;
    goto L29817;}}
 L13718: {
  RSstackS = (-11 + (Sbottom_of_stackS));
  *((long *) (-3 + (Sbottom_of_stackS))) = cont_337X;
  ScontS = (Sbottom_of_stackS);
  goto L11161;}
 L13682: {
  if ((1 == cont_337X)) {
    if ((0 == (3 & (RSvalS)))) {
      RSvalS = ((((RSvalS))>>2));
      arg1K0 = 0;
      goto L12103;}
    else {
      goto L13699;}}
  else {
    goto L13699;}}
 L12631: {
  bytes_consumed_696X = arg1K0;
  obj_699X = RSvalS;
  if ((3 == (3 & obj_699X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_699X))))>>2))))) {
      RSenvS = (*((long *) (1 + (RSvalS))));
      tem_698X = *((long *) (-3 + (RSvalS)));
      StemplateS = tem_698X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_698X))));
      if ((Spending_interruptPS)) {
        if ((Spending_eventsPS)) {
          Spending_eventsPS = 0;
          check_events_return_tag = 1;
          goto check_events;
         check_events_return_1:
          v_697X = check_events0_return_value;
          if (v_697X) {
            goto L13261;}
          else {
            goto L11161;}}
        else {
          goto L13261;}}
      else {
        goto L11161;}}
    else {
      arg1K0 = bytes_consumed_696X;
      arg1K1 = 3;
      goto L26441;}}
  else {
    arg1K0 = bytes_consumed_696X;
    arg1K1 = 3;
    goto L26441;}}
 L29856: {
  loc_298X = arg0K0;
  arg_299X = arg0K1;
  if ((top_of_args_339X < arg_299X)) {
    *((long *) loc_298X) = (*((long *) arg_299X));
    arg0K0 = (-4 + loc_298X);
    arg0K1 = (-4 + arg_299X);
    goto L29856;}
  else {
    RSstackS = loc_298X;
    arg1K0 = 0;
    goto L12631;}}
 L8321: {
  fast_209X = arg0K0;
  len_210X = arg0K1;
  slow_211X = arg0K2;
  move_slowP_212X = arg2K3;
  if ((25 == fast_209X)) {
    args_214X = RSvalS;
    nargs_213X = len_210X + (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
    if ((63 < nargs_213X)) {
      arg0K0 = (-63 + nargs_213X);
      goto L11099;}
    else {
      arg0K0 = 0;
      goto L11099;}}
  else {
    if ((3 == (3 & fast_209X))) {
      if ((0 == (31 & ((((*((long *) (-7 + fast_209X))))>>2))))) {
        if (move_slowP_212X) {
          if ((fast_209X == slow_211X)) {
            goto L13630;}
          else {
            arg0K0 = (*((long *) (1 + fast_209X)));
            arg0K1 = (1 + len_210X);
            arg0K2 = (*((long *) (1 + slow_211X)));
            arg2K3 = 0;
            goto L8321;}}
        else {
          arg0K0 = (*((long *) (1 + fast_209X)));
          arg0K1 = (1 + len_210X);
          arg0K2 = slow_211X;
          arg2K3 = 1;
          goto L8321;}}
      else {
        goto L13630;}}
    else {
      goto L13630;}}}
 L12751: {
  SnargsS = 0;
  arg1K0 = 0;
  goto L12631;}
 L12761: {
  RSstackS = (-11 + (Sbottom_of_stackS));
  *((long *) (-3 + (Sbottom_of_stackS))) = cont_341X;
  ScontS = (Sbottom_of_stackS);
  goto L12751;}
 L29360: {
  nargs_683X = arg0K0;
  if ((1 == nargs_683X)) {
    RSstackS = (4 + (RSstackS));
    RSvalS = (*((long *) (RSstackS)));
    cont_684X = ScontS;
    tem_685X = *((long *) (5 + cont_684X));
    pc_686X = *((long *) (1 + cont_684X));
    StemplateS = tem_685X;
    RScode_pointerS = ((-3 + (*((long *) (-3 + tem_685X)))) + (((pc_686X)>>2)));
    RSenvS = (*((long *) (9 + cont_684X)));
    ScontS = (*((long *) (-3 + cont_684X)));
    RSstackS = (9 + cont_684X);
    goto L11161;}
  else {
    if (((ScontS) == (Sbottom_of_stackS))) {
      arg0K0 = (*((long *) (-3 + (Sbottom_of_stackS))));
      goto L29371;}
    else {
      arg0K0 = (ScontS);
      goto L29371;}}}
 L13261: {
  arg0K0 = (4 + (RSstackS));
  arg0K1 = 0;
  goto L11014;}
 L13513: {
  arg0K0 = ((((((unsigned long) b_356X)))<<1));
  goto L13515;}
 L13515: {
  offset_216X = arg0K0;
  RScode_pointerS = ((RScode_pointerS) + offset_216X);
  goto L11161;}
 L29931: {
  v_297X = arg1K0;
  RSvalS = (((unsigned long) v_297X));
  goto L11161;}
 L25873: {
  v_88X = arg1K0;
  RSvalS = (((unsigned long) v_88X));
  goto L11161;}
 L25828: {
  if ((3 == (3 & x_640X))) {
    if ((7 == (31 & ((((*((long *) (-7 + x_640X))))>>2))))) {
      arg1K0 = 5;
      goto L25873;}
    else {
      goto L25836;}}
  else {
    goto L25836;}}
 L25698: {
  RSvalS = 5;
  goto L11161;}
 L25701: {
  if ((3 == (3 & x_639X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_639X))))>>2))))) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 89;
      goto push_exception_continuationB;
     push_exception_continuationB_return_89:
      *((long *) (RSstackS)) = x_639X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12977;}
    else {
      goto L25709;}}
  else {
    goto L25709;}}
 L25509: {
  RSvalS = 5;
  goto L11161;}
 L25492: {
  if ((3 == (3 & x_638X))) {
    if ((7 == (31 & ((((*((long *) (-7 + x_638X))))>>2))))) {
      goto L25509;}
    else {
      goto L25500;}}
  else {
    goto L25500;}}
 L25300: {
  RSvalS = 5;
  goto L11161;}
 L25283: {
  if ((3 == (3 & x_637X))) {
    if ((7 == (31 & ((((*((long *) (-7 + x_637X))))>>2))))) {
      goto L25300;}
    else {
      goto L25291;}}
  else {
    goto L25291;}}
 L25091: {
  RSvalS = 5;
  goto L11161;}
 L25074: {
  if ((3 == (3 & x_636X))) {
    if ((7 == (31 & ((((*((long *) (-7 + x_636X))))>>2))))) {
      goto L25091;}
    else {
      goto L25082;}}
  else {
    goto L25082;}}
 L5372: {
  a_288X = arg0K0;
  if ((b_634X < 0)) {
    arg0K0 = (0 - b_634X);
    goto L5376;}
  else {
    arg0K0 = b_634X;
    goto L5376;}}
 L7147: {
  a_267X = arg0K0;
  if ((b_629X < 0)) {
    arg0K0 = (0 - b_629X);
    goto L7149;}
  else {
    arg0K0 = b_629X;
    goto L7149;}}
 L24896: {
  v_89X = arg1K0;
  RSvalS = (((unsigned long) v_89X));
  goto L11161;}
 L24807: {
  v_90X = arg1K0;
  RSvalS = (((unsigned long) v_90X));
  goto L11161;}
 L28811: {
  a_31X = arg0K0;
  if ((b_624X < 0)) {
    arg0K0 = (0 - b_624X);
    goto L28813;}
  else {
    arg0K0 = b_624X;
    goto L28813;}}
 L28680: {
  a_35X = arg0K0;
  n_36X = ((x_620X)>>2);
  if ((n_36X < 0)) {
    arg0K0 = (0 - n_36X);
    goto L28682;}
  else {
    arg0K0 = n_36X;
    goto L28682;}}
 L24187: {
  r_91X = arg0K0;
  if ((536870911 < r_91X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 90;
    goto push_exception_continuationB;
   push_exception_continuationB_return_90:
    *((long *) (RSstackS)) = x_611X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 1;
    arg1K1 = 0;
    goto L12977;}
  else {
    RSvalS = (((r_91X)<<2));
    goto L11161;}}
 L28532: {
  RSvalS = (((result_377X)<<2));
  goto L11161;}
 L30259: {
  v_296X = arg1K0;
  RSvalS = (((unsigned long) v_296X));
  goto L11161;}
 L23536: {
  v_92X = arg1K0;
  RSvalS = (((unsigned long) v_92X));
  goto L11161;}
 L23509: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 91;
  goto push_exception_continuationB;
 push_exception_continuationB_return_91:
  *((long *) (RSstackS)) = arg2_603X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L23447: {
  v_93X = arg1K0;
  RSvalS = (((unsigned long) v_93X));
  goto L11161;}
 L23420: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 92;
  goto push_exception_continuationB;
 push_exception_continuationB_return_92:
  *((long *) (RSstackS)) = arg2_601X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L23302: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 93;
  goto push_exception_continuationB;
 push_exception_continuationB_return_93:
  *((long *) (RSstackS)) = (((x_382X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L30286: {
  v_295X = arg1K0;
  RSvalS = (((unsigned long) v_295X));
  goto L11161;}
 L30300: {
  v_294X = arg1K0;
  RSvalS = (((unsigned long) v_294X));
  goto L11161;}
 L23108: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 94;
  goto push_exception_continuationB;
 push_exception_continuationB_return_94:
  *((long *) (RSstackS)) = x_598X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_599X)))<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 1;
  goto L12977;}
 L22941: {
  b_98X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  b_97X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  len_96X = (((((unsigned long) b_98X)))<<2);
  addr_95X = ALLOCATE_SPACE((((unsigned long) b_97X)), (4 + len_96X));
  *((long *) addr_95X) = (2 + (((((((len_96X)<<6)) + (((unsigned long) b_97X))))<<2)));
  new_94X = 7 + addr_95X;
  if (((((unsigned long) b_98X)) < 1)) {
    goto L22980;}
  else {
    *((long *) (-7 + (new_94X + ((((((unsigned long) b_98X)))<<2))))) = (RSvalS);
    arg0K0 = (-2 + (((unsigned long) b_98X)));
    goto L22964;}}
 L22836: {
  b_100X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  len_101X = ((len_594X)<<2);
  addr_102X = ALLOCATE_SPACE((((unsigned long) b_100X)), (4 + len_101X));
  *((long *) addr_102X) = (2 + (((((((len_101X)<<6)) + (((unsigned long) b_100X))))<<2)));
  new_103X = 7 + addr_102X;
  arg0K0 = (-1 + len_594X);
  goto L22848;}
 L22736: {
  merged_arg1K0 = 2;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 95;
  goto push_exception_continuationB;
 push_exception_continuationB_return_95:
  *((long *) (RSstackS)) = x_390X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_389X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_388X)))<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 2;
  goto L12977;}
 L22521: {
  merged_arg1K0 = 2;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 96;
  goto push_exception_continuationB;
 push_exception_continuationB_return_96:
  *((long *) (RSstackS)) = arg2_589X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_591X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_592X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_590X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 2;
  goto L12977;}
 L22196: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 97;
  goto push_exception_continuationB;
 push_exception_continuationB_return_97:
  *((long *) (RSstackS)) = ((((((unsigned long) b_391X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((len_584X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_392X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 1;
  goto L12977;}
 L22214: {
  okayP_107X = arg2K0;
  init_108X = arg0K1;
  if (okayP_107X) {
    len_109X = ((len_584X)<<2);
    addr_110X = ALLOCATE_SPACE((((unsigned long) b_391X)), (4 + len_109X));
    *((long *) addr_110X) = (2 + (((((((len_109X)<<6)) + (((unsigned long) b_391X))))<<2)));
    val_111X = 7 + addr_110X;
    arg0K0 = (-1 + len_584X);
    goto L22238;}
  else {
    merged_arg1K0 = 1;
    merged_arg1K1 = 8;
    push_exception_continuationB_return_tag = 98;
    goto push_exception_continuationB;
   push_exception_continuationB_return_98:
    *((long *) (RSstackS)) = ((((((unsigned long) b_391X)))<<2));
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (((len_584X)<<2));
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = init_108X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 3;
    arg1K1 = 1;
    goto L12977;}}
 L21933: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 99;
  goto push_exception_continuationB;
 push_exception_continuationB_return_99:
  *((long *) (RSstackS)) = arg2_396X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_394X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_395X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 1;
  goto L12977;}
 L21910: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 100;
  goto push_exception_continuationB;
 push_exception_continuationB_return_100:
  *((long *) (RSstackS)) = arg2_396X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_394X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_395X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 1;
  goto L12977;}
 L21574: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 101;
  goto push_exception_continuationB;
 push_exception_continuationB_return_101:
  *((long *) (RSstackS)) = arg3_400X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_398X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_401X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_399X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 1;
  goto L12977;}
 L21549: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 102;
  goto push_exception_continuationB;
 push_exception_continuationB_return_102:
  *((long *) (RSstackS)) = arg3_400X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_398X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_401X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_399X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 1;
  goto L12977;}
 L21280: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 103;
  goto push_exception_continuationB;
 push_exception_continuationB_return_103:
  *((long *) (RSstackS)) = (((len_575X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((init_576X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L21373: {
  addr_112X = ALLOCATE_SPACE(17, (4 + len_575X));
  *((long *) addr_112X) = (70 + (((len_575X)<<8)));
  vector_113X = 7 + addr_112X;
  arg0K0 = (-1 + len_575X);
  goto L21320;}
 L30343: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 104;
  goto push_exception_continuationB;
 push_exception_continuationB_return_104:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L21147: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 105;
  goto push_exception_continuationB;
 push_exception_continuationB_return_105:
  *((long *) (RSstackS)) = arg2_573X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_404X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L27845: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 106;
  goto push_exception_continuationB;
 push_exception_continuationB_return_106:
  *((long *) (RSstackS)) = arg2_573X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L21009: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 107;
  goto push_exception_continuationB;
 push_exception_continuationB_return_107:
  *((long *) (RSstackS)) = arg3_572X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_407X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((Kchar_406X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L20988: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 108;
  goto push_exception_continuationB;
 push_exception_continuationB_return_108:
  *((long *) (RSstackS)) = arg3_572X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_407X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((Kchar_406X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L27729: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 109;
  goto push_exception_continuationB;
 push_exception_continuationB_return_109:
  *((long *) (RSstackS)) = arg3_572X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_571X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L20771: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 110;
  goto push_exception_continuationB;
 push_exception_continuationB_return_110:
  *((long *) (RSstackS)) = (((len_412X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (9 + ((((((unsigned long) init_411X)))<<8)));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L20864: {
  len_37X = 1 + len_412X;
  addr_38X = ALLOCATE_SPACE(16, (4 + len_37X));
  *((long *) addr_38X) = (66 + (((len_37X)<<8)));
  string_39X = 7 + addr_38X;
  *((unsigned char *) ((-3 + string_39X) + len_412X)) = 0;
  arg0K0 = (-1 + len_412X);
  goto L20811;}
 L27646: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 111;
  goto push_exception_continuationB;
 push_exception_continuationB_return_111:
  *((long *) (RSstackS)) = arg2_413X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L30366: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 112;
  goto push_exception_continuationB;
 push_exception_continuationB_return_112:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L20638: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 113;
  goto push_exception_continuationB;
 push_exception_continuationB_return_113:
  *((long *) (RSstackS)) = arg2_569X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_415X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L27533: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 114;
  goto push_exception_continuationB;
 push_exception_continuationB_return_114:
  *((long *) (RSstackS)) = arg2_569X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L20500: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 115;
  goto push_exception_continuationB;
 push_exception_continuationB_return_115:
  *((long *) (RSstackS)) = arg3_419X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_418X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (9 + ((((((unsigned long) Kchar_417X)))<<8)));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L20479: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 116;
  goto push_exception_continuationB;
 push_exception_continuationB_return_116:
  *((long *) (RSstackS)) = arg3_419X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_418X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (9 + ((((((unsigned long) Kchar_417X)))<<8)));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L27417: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 117;
  goto push_exception_continuationB;
 push_exception_continuationB_return_117:
  *((long *) (RSstackS)) = arg3_419X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_420X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L20406: {
  v_115X = arg1K0;
  RSvalS = (((unsigned long) v_115X));
  goto L11161;}
 L20378: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 118;
  goto push_exception_continuationB;
 push_exception_continuationB_return_118:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L20169: {
  if ((1 == (RSvalS))) {
    addr_117X = 1 + arg2_422X;
    WRITE_BARRIER(addr_117X, 273);
    *((long *) addr_117X) = 273;
    goto L20194;}
  else {
    if ((17 == (255 & (*((long *) (1 + arg2_422X)))))) {
      addr_116X = 1 + arg2_422X;
      WRITE_BARRIER(addr_116X, 529);
      *((long *) addr_116X) = 529;
      goto L20194;}
    else {
      goto L20194;}}}
 L20195: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 119;
  goto push_exception_continuationB;
 push_exception_continuationB_return_119:
  *((long *) (RSstackS)) = arg2_422X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L30392: {
  v_293X = arg1K0;
  RSvalS = (((unsigned long) v_293X));
  goto L11161;}
 L30404: {
  RSvalS = x_423X;
  goto L11161;}
 L19543: {
  RSstackS = (4 + (RSstackS));
  arg2_133X = *((long *) (RSstackS));
  if ((0 == (3 & (RSvalS)))) {
    mode_134X = (((RSvalS))>>2);
    if ((1 == mode_134X)) {
      goto L19797;}
    else {
      if ((2 == mode_134X)) {
        goto L19797;}
      else {
        if ((3 == mode_134X)) {
          goto L19797;}
        else {
          if ((4 == mode_134X)) {
            goto L19797;}
          else {
            merged_arg1K0 = 0;
            merged_arg1K1 = 5;
            push_exception_continuationB_return_tag = 120;
            goto push_exception_continuationB;
           push_exception_continuationB_return_120:
            *((long *) (RSstackS)) = arg2_133X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = (((mode_134X)<<2));
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12977;}}}}}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 5;
    push_exception_continuationB_return_tag = 121;
    goto push_exception_continuationB;
   push_exception_continuationB_return_121:
    *((long *) (RSstackS)) = arg2_133X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (RSvalS);
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}}
 L19452: {
  v_141X = ps_close_fd(os_index_560X);
  arg0K0 = v_141X;
  goto L19441;}
 L19441: {
  status_142X = arg0K0;
  *((Svm_channelsS) + os_index_560X) = 1;
  addr_143X = -3 + x_559X;
  WRITE_BARRIER(addr_143X, 0);
  *((long *) addr_143X) = 0;
  if ((status_142X == NO_ERRORS)) {
    RSvalS = 13;
    goto L11161;}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 21;
    push_exception_continuationB_return_tag = 122;
    goto push_exception_continuationB;
   push_exception_continuationB_return_122:
    *((long *) (RSstackS)) = x_559X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 1;
    arg1K1 = 0;
    goto L12977;}}
 L19415: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 123;
  goto push_exception_continuationB;
 push_exception_continuationB_return_123:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L27179: {
  obj_49X = RSvalS;
  if ((3 == (3 & obj_49X))) {
    if ((5 == (31 & ((((*((long *) (-7 + obj_49X))))>>2))))) {
      x_48X = RSvalS;
      if ((1 == arg2_554X)) {
        arg2K0 = 0;
        goto L27196;}
      else {
        arg2K0 = 1;
        goto L27196;}}
    else {
      goto L27199;}}
  else {
    goto L27199;}}
 L27199: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 124;
  goto push_exception_continuationB;
 push_exception_continuationB_return_124:
  *((long *) (RSstackS)) = arg5_557X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg4_556X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_555X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_554X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 5;
  arg1K1 = 0;
  goto L12977;}
 L18887: {
  if ((3 == (3 & arg4_424X))) {
    if ((16 == (31 & ((((*((long *) (-7 + arg4_424X))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg4_424X))))>>8)));
      goto L18899;}
    else {
      goto L18895;}}
  else {
    goto L18895;}}
 L18879: {
  if ((3 == (3 & arg4_424X))) {
    if ((17 == (31 & ((((*((long *) (-7 + arg4_424X))))>>2))))) {
      goto L18887;}
    else {
      arg1K0 = 5;
      goto L18816;}}
  else {
    arg1K0 = 5;
    goto L18816;}}
 L18816: {
  reason_57X = arg1K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = reason_57X;
  push_exception_continuationB_return_tag = 125;
  goto push_exception_continuationB;
 push_exception_continuationB_return_125:
  *((long *) (RSstackS)) = arg4_424X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((start_550X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((count_551X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = channel_552X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 0;
  goto L12977;}
 L27052: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 126;
  goto push_exception_continuationB;
 push_exception_continuationB_return_126:
  *((long *) (RSstackS)) = arg4_424X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_425X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_426X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 0;
  goto L12977;}
 L30439: {
  val_270X = arg0K0;
  RSvalS = val_270X;
  goto L11161;}
 L6617: {
  arg0K0 = (*((long *) (13 + channel_428X)));
  goto L30439;}
 L6623: {
  ch_274X = arg0K0;
  prev_275X = arg0K1;
  if ((1 == ch_274X)) {
    n_277X = ps_abort_fd_op((*((long *) (5 + channel_428X))));
    arg0K0 = (((n_277X)<<2));
    goto L30439;}
  else {
    if ((ch_274X == channel_428X)) {
      y_276X = Spending_channels_tailS;
      if ((ch_274X == y_276X)) {
        Spending_channels_tailS = prev_275X;
        goto L6643;}
      else {
        goto L6643;}}
    else {
      arg0K0 = (*((long *) (9 + ch_274X)));
      arg0K1 = ch_274X;
      goto L6623;}}}
 L30426: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 127;
  goto push_exception_continuationB;
 push_exception_continuationB_return_127:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L18358: {
  reason_144X = arg1K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = reason_144X;
  push_exception_continuationB_return_tag = 128;
  goto push_exception_continuationB;
 push_exception_continuationB_return_128:
  *((long *) (RSstackS)) = arg3_533X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_532X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_535X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L18421: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 129;
  goto push_exception_continuationB;
 push_exception_continuationB_return_129:
  *((long *) (RSstackS)) = arg3_533X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_532X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L18091: {
  RSstackS = (4 + (RSstackS));
  arg2_146X = *((long *) (RSstackS));
  x_145X = RSvalS;
  if ((3 == (3 & arg2_146X))) {
    if ((3 == (3 & x_145X))) {
      if ((3 == (31 & ((((*((long *) (-7 + x_145X))))>>2))))) {
        addr_147X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_147X) = 2050;
        x_148X = 7 + addr_147X;
        *((long *) (-3 + x_148X)) = arg2_146X;
        *((long *) (1 + x_148X)) = x_145X;
        b_149X = Sfinalizer_alistS;
        addr_150X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_150X) = 2050;
        x_151X = 7 + addr_150X;
        *((long *) (-3 + x_151X)) = x_148X;
        *((long *) (1 + x_151X)) = b_149X;
        Sfinalizer_alistS = x_151X;
        RSvalS = 13;
        goto L11161;}
      else {
        goto L18184;}}
    else {
      goto L18184;}}
  else {
    goto L18184;}}
 L17809: {
  firstP_152X = arg2K0;
  vector_153X = find_all(type_436X);
  if ((1 == vector_153X)) {
    if (firstP_152X) {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 16;
      goto collect_saving_temp;
     collect_saving_temp_return_16:
      arg2K0 = 0;
      goto L17809;}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 8;
      push_exception_continuationB_return_tag = 130;
      goto push_exception_continuationB;
     push_exception_continuationB_return_130:
      *((long *) (RSstackS)) = (((type_436X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12977;}}
  else {
    RSvalS = vector_153X;
    goto L11161;}}
 L26339: {
  table_84X = arg0K0;
  firstP_85X = arg2K1;
  symbols_86X = find_all(1);
  if ((1 == symbols_86X)) {
    if (firstP_85X) {
      merged_arg0K0 = table_84X;
      collect_saving_temp_return_tag = 17;
      goto collect_saving_temp;
     collect_saving_temp_return_17:
      v_87X = collect_saving_temp0_return_value;
      arg0K0 = v_87X;
      arg2K1 = 0;
      goto L26339;}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 8;
      push_exception_continuationB_return_tag = 131;
      goto push_exception_continuationB;
     push_exception_continuationB_return_131:
      *((long *) (RSstackS)) = table_84X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12977;}}
  else {
    arg0K0 = table_84X;
    arg0K1 = symbols_86X;
    arg2K2 = 1;
    goto L13986;}}
 L26361: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 132;
  goto push_exception_continuationB;
 push_exception_continuationB_return_132:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L30487: {
  firstP_289X = arg2K0;
  type_290X = arg0K1;
  vector_291X = find_all_records(type_290X);
  if ((1 == vector_291X)) {
    if (firstP_289X) {
      merged_arg0K0 = type_290X;
      collect_saving_temp_return_tag = 18;
      goto collect_saving_temp;
     collect_saving_temp_return_18:
      v_292X = collect_saving_temp0_return_value;
      arg2K0 = 0;
      arg0K1 = v_292X;
      goto L30487;}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 8;
      push_exception_continuationB_return_tag = 133;
      goto push_exception_continuationB;
     push_exception_continuationB_return_133:
      *((long *) (RSstackS)) = type_290X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12977;}}
  else {
    RSvalS = vector_291X;
    goto L11161;}}
 L13397: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 134;
  goto push_exception_continuationB;
 push_exception_continuationB_return_134:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L13297: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 135;
  goto push_exception_continuationB;
 push_exception_continuationB_return_135:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L30523: {
  RSvalS = (((old_440X)<<2));
  goto L11161;}
 L11179: {
  RSstackS = (4 + (RSstackS));
  SnargsS = ((((*((long *) (RSstackS))))>>2));
  RSstackS = (4 + (RSstackS));
  tem_259X = *((long *) (RSstackS));
  StemplateS = tem_259X;
  RScode_pointerS = (-3 + (*((long *) (-3 + tem_259X))));
  RSstackS = (4 + (RSstackS));
  RSvalS = (*((long *) (RSstackS)));
  goto L11161;}
 L17581: {
  n_154X = schedule_alarm_interrupt((((p_441X)>>2)));
  RSvalS = (((n_154X)<<2));
  goto L11161;}
 L17417: {
  if ((1 == (RSvalS))) {
    arg2K0 = 0;
    goto L17421;}
  else {
    arg2K0 = 1;
    goto L17421;}}
 L17437: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 136;
  goto push_exception_continuationB;
 push_exception_continuationB_return_136:
  *((long *) (RSstackS)) = arg2_521X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L17262: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 18;
  push_exception_continuationB_return_tag = 137;
  goto push_exception_continuationB;
 push_exception_continuationB_return_137:
  *((long *) (RSstackS)) = x_517X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L17267: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 138;
  goto push_exception_continuationB;
 push_exception_continuationB_return_138:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L12103: {
  option_26X = arg1K0;
  if ((0 == (((unsigned long) option_26X)))) {
    SenvS = RSenvS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    SvalS = RSvalS;
    return (RSvalS);}
  else {
    if ((1 == (((unsigned long) option_26X)))) {
      v_28X = (long(*)())((*((long *) (-3 + (*((long *) (1 + (RSvalS))))))));
      v_27X = v_28X((SnargsS), (4 + (RSstackS)));
      RSvalS = v_27X;
      RSstackS = ((RSstackS) - ((((-1 - (SnargsS)))<<2)));
      goto L12101;}
    else {
      ps_error(((unsigned char *) "unknown VM return option"), 1, (((unsigned long) option_26X)));
      SenvS = RSenvS;
      SstackS = RSstackS;
      Scode_pointerS = RScode_pointerS;
      SvalS = RSvalS;
      return -1;}}}
 L30545: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 139;
  goto push_exception_continuationB;
 push_exception_continuationB_return_139:
  *((long *) (RSstackS)) = proc_443X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L16840: {
  v_157X = arg1K0;
  RSvalS = (((unsigned long) v_157X));
  goto L11161;}
 L16807: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 140;
  goto push_exception_continuationB;
 push_exception_continuationB_return_140:
  *((long *) (RSstackS)) = arg2_452X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L16752: {
  i_158X = arg0K0;
  h_159X = arg0K1;
  if ((i_158X < n_507X)) {
    arg0K0 = (1 + i_158X);
    arg0K1 = (h_159X + (((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + x_506X) + i_158X))))))));
    goto L16752;}
  else {
    RSvalS = (((h_159X)<<2));
    goto L11161;}}
 L16729: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 141;
  goto push_exception_continuationB;
 push_exception_continuationB_return_141:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L16470: {
  RSstackS = (4 + (RSstackS));
  arg2_163X = *((long *) (RSstackS));
  if ((0 == (3 & (RSvalS)))) {
    n_164X = (((RSvalS))>>2);
    if ((3 == (3 & arg2_163X))) {
      if ((0 == (31 & ((((*((long *) (-7 + arg2_163X))))>>2))))) {
        goto L16591;}
      else {
        goto L16503;}}
    else {
      goto L16503;}}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 5;
    push_exception_continuationB_return_tag = 142;
    goto push_exception_continuationB;
   push_exception_continuationB_return_142:
    *((long *) (RSstackS)) = arg2_163X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (RSvalS);
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}}
 L16336: {
  RSstackS = (4 + (RSstackS));
  arg2_181X = *((long *) (RSstackS));
  if ((3 == (3 & arg2_181X))) {
    if ((16 == (31 & ((((*((long *) (-7 + arg2_181X))))>>2))))) {
      obj_182X = RSvalS;
      if ((3 == (3 & obj_182X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_182X))))>>2))))) {
          table_183X = RSvalS;
          v_184X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + table_183X))))>>8))))>>2);
          n_185X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg2_181X))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L9824;}
        else {
          goto L16360;}}
      else {
        goto L16360;}}
    else {
      goto L16360;}}
  else {
    goto L16360;}}
 L16081: {
  list_186X = arg0K0;
  slow_187X = arg0K1;
  move_slowP_188X = arg2K2;
  if ((25 == list_186X)) {
    RSvalS = 1;
    goto L11161;}
  else {
    if ((3 == (3 & list_186X))) {
      if ((0 == (31 & ((((*((long *) (-7 + list_186X))))>>2))))) {
        head_189X = *((long *) (-3 + list_186X));
        if ((3 == (3 & head_189X))) {
          if ((0 == (31 & ((((*((long *) (-7 + head_189X))))>>2))))) {
            if (((*((long *) (-3 + head_189X))) == arg2_457X)) {
              RSvalS = head_189X;
              goto L11161;}
            else {
              list_190X = *((long *) (1 + list_186X));
              if ((list_190X == slow_187X)) {
                merged_arg1K0 = 0;
                merged_arg1K1 = 5;
                push_exception_continuationB_return_tag = 143;
                goto push_exception_continuationB;
               push_exception_continuationB_return_143:
                *((long *) (RSstackS)) = arg2_457X;
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = x_456X;
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 2;
                arg1K1 = 0;
                goto L12977;}
              else {
                if (move_slowP_188X) {
                  arg0K0 = list_190X;
                  arg0K1 = (*((long *) (1 + slow_187X)));
                  arg2K2 = 0;
                  goto L16081;}
                else {
                  arg0K0 = list_190X;
                  arg0K1 = slow_187X;
                  arg2K2 = 1;
                  goto L16081;}}}}
          else {
            merged_arg1K0 = 0;
            merged_arg1K1 = 5;
            push_exception_continuationB_return_tag = 144;
            goto push_exception_continuationB;
           push_exception_continuationB_return_144:
            *((long *) (RSstackS)) = arg2_457X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_456X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12977;}}
        else {
          merged_arg1K0 = 0;
          merged_arg1K1 = 5;
          push_exception_continuationB_return_tag = 145;
          goto push_exception_continuationB;
         push_exception_continuationB_return_145:
          *((long *) (RSstackS)) = arg2_457X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_456X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12977;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 146;
        goto push_exception_continuationB;
       push_exception_continuationB_return_146:
        *((long *) (RSstackS)) = arg2_457X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_456X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 147;
      goto push_exception_continuationB;
     push_exception_continuationB_return_147:
      *((long *) (RSstackS)) = arg2_457X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = x_456X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12977;}}}
 L15834: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 148;
  goto push_exception_continuationB;
 push_exception_continuationB_return_148:
  *((long *) (RSstackS)) = arg3_501X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_500X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_458X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L15881: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 149;
  goto push_exception_continuationB;
 push_exception_continuationB_return_149:
  *((long *) (RSstackS)) = arg3_501X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_500X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_458X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L15453: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 150;
  goto push_exception_continuationB;
 push_exception_continuationB_return_150:
  *((long *) (RSstackS)) = arg4_460X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_461X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_498X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_497X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 0;
  goto L12977;}
 L15510: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 151;
  goto push_exception_continuationB;
 push_exception_continuationB_return_151:
  *((long *) (RSstackS)) = arg4_460X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_461X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_498X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_497X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 0;
  goto L12977;}
 L14793: {
  if ((3 == (3 & arg5_463X))) {
    if ((16 == (31 & ((((*((long *) (-7 + arg5_463X))))>>2))))) {
      goto L14806;}
    else {
      goto L14801;}}
  else {
    goto L14801;}}
 L14788: {
  if ((3 == (3 & arg5_463X))) {
    if ((17 == (31 & ((((*((long *) (-7 + arg5_463X))))>>2))))) {
      goto L14793;}
    else {
      goto L14868;}}
  else {
    goto L14868;}}
 L26758: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 152;
  goto push_exception_continuationB;
 push_exception_continuationB_return_152:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L14635: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 14;
  push_exception_continuationB_return_tag = 153;
  goto push_exception_continuationB;
 push_exception_continuationB_return_153:
  *((long *) (RSstackS)) = port_474X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L26699: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 154;
  goto push_exception_continuationB;
 push_exception_continuationB_return_154:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L14532: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 14;
  push_exception_continuationB_return_tag = 155;
  goto push_exception_continuationB;
 push_exception_continuationB_return_155:
  *((long *) (RSstackS)) = port_481X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L14345: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 156;
  goto push_exception_continuationB;
 push_exception_continuationB_return_156:
  *((long *) (RSstackS)) = arg2_493X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L14327: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 14;
  push_exception_continuationB_return_tag = 157;
  goto push_exception_continuationB;
 push_exception_continuationB_return_157:
  *((long *) (RSstackS)) = (9 + ((((((unsigned long) Kchar_489X)))<<8)));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_490X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L26453: {
  args_700X = arg0K0;
  a1_701X = RSvalS;
  *((long *) (RSstackS)) = ((((((unsigned long) (*((unsigned char *) (-1 + ((RScode_pointerS) - (((unsigned long) byte_args_702X)))))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) exception_703X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a1_701X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = args_700X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = byte_args_702X;
  goto L12977;}
 L13038: {
  RSvalS = (*((long *) ((-3 + (Sexception_handlersS)) + ((((((unsigned long) (*((unsigned char *) (-1 + ((RScode_pointerS) - (((unsigned long) args_694X)))))))))<<2)))));
  obj_692X = RSvalS;
  if ((3 == (3 & obj_692X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_692X))))>>2))))) {
      goto L13057;}
    else {
      goto L13134;}}
  else {
    goto L13134;}}
 L13117: {
  merged_arg3K0 = ((unsigned char *) "exception-handlers is not a vector");
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L13038;}
 L11721: {
  key_234X = arg1K0;
  p_235X = RSenvS;
  if ((3 == (3 & p_235X))) {
    if ((p_235X < (Sstack_beginS))) {
      goto L11766;}
    else {
      if (((Sstack_endS) < p_235X)) {
        goto L11766;}
      else {
        merged_arg0K0 = (RSenvS);
        merged_arg0K1 = (ScontS);
        merged_arg1K2 = key_234X;
        merged_arg1K3 = 0;
        save_env_in_heap_return_tag = 0;
        goto save_env_in_heap;
       save_env_in_heap_return_0:
        v_236X = save_env_in_heap0_return_value;
        RSenvS = v_236X;
        goto L11766;}}}
  else {
    goto L11766;}}
 L11569: {
  i_249X = arg0K0;
  if ((i_249X == (((unsigned long) b_329X)))) {
    b_252X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    b_253X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    a_254X = *((long *) ((-3 + (StemplateS)) + ((((((((((unsigned long) b_252X)))<<8)) + (((unsigned long) b_253X))))<<2))));
    addr_255X = ALLOCATE_SPACE(3, 12);
    *((long *) addr_255X) = 2062;
    x_256X = 7 + addr_255X;
    *((long *) (-3 + x_256X)) = a_254X;
    *((long *) (1 + x_256X)) = new_env_248X;
    if ((3 == (3 & x_256X))) {
      if ((0 == (128 & (*((long *) (-7 + x_256X)))))) {
        *((long *) (-7 + x_256X)) = (128 | (*((long *) (-7 + x_256X))));
        arg0K0 = x_256X;
        goto L11596;}
      else {
        arg0K0 = x_256X;
        goto L11596;}}
    else {
      arg0K0 = x_256X;
      goto L11596;}}
  else {
    b_251X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    env_250X = RSenvS;
    arg0K0 = env_250X;
    arg0K1 = (((unsigned long) b_251X));
    goto L11678;}}
 L29817: {
  v_300X = arg0K0;
  RSvalS = v_300X;
  goto L11161;}
 L13699: {
  RSstackS = (-11 + (Sbottom_of_stackS));
  *((long *) (-3 + (Sbottom_of_stackS))) = 1;
  ScontS = (Sbottom_of_stackS);
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 158;
  goto push_exception_continuationB;
 push_exception_continuationB_return_158:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = cont_337X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L11099: {
  overflow_208X = arg0K0;
  arg0K0 = (len_210X - overflow_208X);
  arg0K1 = args_214X;
  goto L11129;}
 L13630: {
  merged_arg0K0 = 25;
  merged_arg0K1 = (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
  pop_args_list_return_tag = 4;
  goto pop_args_list;
 pop_args_list_return_4:
  args_215X = pop_args_list0_return_value;
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 159;
  goto push_exception_continuationB;
 push_exception_continuationB_return_159:
  *((long *) (RSstackS)) = proc_648X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = args_215X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12977;}
 L29371: {
  cont_675X = arg0K0;
  if ((3 == (3 & cont_675X))) {
    if ((9 == (31 & ((((*((long *) (-7 + cont_675X))))>>2))))) {
      next_op_677X = *((unsigned char *) ((-3 + (*((long *) (-3 + (*((long *) (5 + cont_675X))))))) + ((((*((long *) (1 + cont_675X))))>>2))));
      if ((33 == (((unsigned long) next_op_677X)))) {
        cont_680X = ScontS;
        tem_681X = *((long *) (5 + cont_680X));
        pc_682X = *((long *) (1 + cont_680X));
        StemplateS = tem_681X;
        RScode_pointerS = ((-3 + (*((long *) (-3 + tem_681X)))) + (((pc_682X)>>2)));
        RSenvS = (*((long *) (9 + cont_680X)));
        ScontS = (*((long *) (-3 + cont_680X)));
        RSstackS = (9 + cont_680X);
        goto L11161;}
      else {
        if ((29 == (((unsigned long) next_op_677X)))) {
          next_679X = *((long *) (-3 + (ScontS)));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (-3 + (ScontS))) = (*((long *) (-3 + next_679X)));
            goto L26500;}
          else {
            ScontS = next_679X;
            goto L26500;}}
        else {
          if ((64 == nargs_683X)) {
            RSstackS = (4 + (RSstackS));
            merged_arg0K0 = (*((long *) (RSstackS)));
            merged_arg0K1 = 63;
            pop_args_list_return_tag = 5;
            goto pop_args_list;
           pop_args_list_return_5:
            v_676X = pop_args_list0_return_value;
            arg0K0 = v_676X;
            goto L26559;}
          else {
            merged_arg0K0 = 25;
            merged_arg0K1 = nargs_683X;
            pop_args_list_return_tag = 6;
            goto pop_args_list;
           pop_args_list_return_6:
            v_678X = pop_args_list0_return_value;
            arg0K0 = v_678X;
            goto L26559;}}}}
    else {
      goto L29377;}}
  else {
    goto L29377;}}
 L11014: {
  p_719X = arg0K0;
  i_720X = arg0K1;
  if ((2 == (3 & (*((long *) p_719X))))) {
    if ((54 == (*((long *) p_719X)))) {
      arg0K0 = (-1 + i_720X);
      goto L10984;}
    else {
      arg0K0 = i_720X;
      goto L10984;}}
  else {
    arg0K0 = (4 + p_719X);
    arg0K1 = (1 + i_720X);
    goto L11014;}}
 L25836: {
  if ((3 == (3 & x_640X))) {
    if ((18 == (31 & ((((*((long *) (-7 + x_640X))))>>2))))) {
      arg1K0 = 5;
      goto L25873;}
    else {
      goto L25844;}}
  else {
    goto L25844;}}
 L25709: {
  RSvalS = 1;
  goto L11161;}
 L25500: {
  if ((3 == (3 & x_638X))) {
    if ((18 == (31 & ((((*((long *) (-7 + x_638X))))>>2))))) {
      goto L25509;}
    else {
      goto L25512;}}
  else {
    goto L25512;}}
 L25291: {
  if ((3 == (3 & x_637X))) {
    if ((18 == (31 & ((((*((long *) (-7 + x_637X))))>>2))))) {
      goto L25300;}
    else {
      goto L25303;}}
  else {
    goto L25303;}}
 L25082: {
  if ((3 == (3 & x_636X))) {
    if ((18 == (31 & ((((*((long *) (-7 + x_636X))))>>2))))) {
      goto L25091;}
    else {
      goto L25094;}}
  else {
    goto L25094;}}
 L5376: {
  b_278X = arg0K0;
  lo_a_279X = 65535 & a_288X;
  lo_b_280X = 65535 & b_278X;
  hi_a_281X = 65535 & (((a_288X)>>16));
  hi_b_282X = 65535 & (((b_278X)>>16));
  lo_c_283X = SMALL_MULTIPLY(lo_a_279X, lo_b_280X);
  v_284X = SMALL_MULTIPLY(lo_b_280X, hi_a_281X);
  v_285X = SMALL_MULTIPLY(lo_a_279X, hi_b_282X);
  mid_c_286X = v_285X + v_284X;
  c_287X = lo_c_283X + (((mid_c_286X)<<16));
  if ((0 < hi_a_281X)) {
    if ((0 < hi_b_282X)) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 6;
      push_exception_continuationB_return_tag = 160;
      goto push_exception_continuationB;
     push_exception_continuationB_return_160:
      *((long *) (RSstackS)) = arg2_362X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = y_632X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12977;}
    else {
      goto L5418;}}
  else {
    goto L5418;}}
 L7149: {
  b_265X = arg0K0;
  c_266X = a_267X / b_265X;
  if ((a_628X < 0)) {
    if ((b_629X < 0)) {
      goto L7153;}
    else {
      goto L7152;}}
  else {
    if ((b_629X < 0)) {
      goto L7152;}
    else {
      goto L7153;}}}
 L28813: {
  b_29X = arg0K0;
  c_30X = a_31X / b_29X;
  if ((a_623X < 0)) {
    if ((b_624X < 0)) {
      goto L28817;}
    else {
      goto L28816;}}
  else {
    if ((b_624X < 0)) {
      goto L28816;}
    else {
      goto L28817;}}}
 L28682: {
  b_33X = arg0K0;
  c_34X = a_35X % b_33X;
  if ((a_621X < 0)) {
    arg0K0 = (0 - c_34X);
    goto L28686;}
  else {
    arg0K0 = c_34X;
    goto L28686;}}
 L22980: {
  RSvalS = new_94X;
  goto L11161;}
 L22964: {
  i_99X = arg0K0;
  if ((i_99X < 0)) {
    goto L22980;}
  else {
    RSstackS = (4 + (RSstackS));
    *((long *) ((-3 + new_94X) + (((i_99X)<<2)))) = (*((long *) (RSstackS)));
    arg0K0 = (-1 + i_99X);
    goto L22964;}}
 L22848: {
  i_104X = arg0K0;
  if ((i_104X < 0)) {
    RSvalS = new_103X;
    goto L11161;}
  else {
    RSstackS = (4 + (RSstackS));
    *((long *) ((-3 + new_103X) + (((i_104X)<<2)))) = (*((long *) (RSstackS)));
    arg0K0 = (-1 + i_104X);
    goto L22848;}}
 L22238: {
  i_105X = arg0K0;
  if ((i_105X < 0)) {
    RSvalS = val_111X;
    goto L11161;}
  else {
    addr_106X = (-3 + val_111X) + (((i_105X)<<2));
    WRITE_BARRIER(addr_106X, init_108X);
    *((long *) addr_106X) = init_108X;
    arg0K0 = (-1 + i_105X);
    goto L22238;}}
 L21320: {
  i_114X = arg0K0;
  if ((i_114X < 0)) {
    RSvalS = vector_113X;
    goto L11161;}
  else {
    *((unsigned char *) ((-3 + vector_113X) + i_114X)) = init_576X;
    arg0K0 = (-1 + i_114X);
    goto L21320;}}
 L20811: {
  i_40X = arg0K0;
  if ((i_40X < 0)) {
    RSvalS = string_39X;
    goto L11161;}
  else {
    *((unsigned char *) ((-3 + string_39X) + i_40X)) = (((unsigned long) init_411X));
    arg0K0 = (-1 + i_40X);
    goto L20811;}}
 L20194: {
  RSvalS = 13;
  goto L11161;}
 L19797: {
  if ((0 == (3 & arg2_133X))) {
    if (((((arg2_133X)>>2)) < 0)) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 161;
      goto push_exception_continuationB;
     push_exception_continuationB_return_161:
      *((long *) (RSstackS)) = arg2_133X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_134X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12977;}
    else {
      arg0K0 = (((arg2_133X)>>2));
      goto L19584;}}
  else {
    if ((3 == (3 & arg2_133X))) {
      if ((16 == (31 & ((((*((long *) (-7 + arg2_133X))))>>2))))) {
        if ((1 == mode_134X)) {
          goto L19642;}
        else {
          if ((3 == mode_134X)) {
            goto L19642;}
          else {
            v_128X = ps_open_fd((((unsigned char *)(-3 + arg2_133X))), 0, &v_127X);
            arg0K0 = v_128X;
            arg0K1 = v_127X;
            goto L19655;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 162;
        goto push_exception_continuationB;
       push_exception_continuationB_return_162:
        *((long *) (RSstackS)) = arg2_133X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (((mode_134X)<<2));
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 163;
      goto push_exception_continuationB;
     push_exception_continuationB_return_163:
      *((long *) (RSstackS)) = arg2_133X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_134X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12977;}}}
 L27196: {
  waitP_50X = arg2K0;
  start_52X = ((arg4_556X)>>2);
  count_51X = ((arg3_555X)>>2);
  if ((4 == (*((long *) (-3 + x_48X))))) {
    if ((3 == (3 & arg5_557X))) {
      if ((0 == (128 & (*((long *) (-7 + arg5_557X)))))) {
        if ((3 == (3 & arg5_557X))) {
          if ((16 == (31 & ((((*((long *) (-7 + arg5_557X))))>>2))))) {
            goto L19137;}
          else {
            goto L19129;}}
        else {
          goto L19129;}}
      else {
        arg1K0 = 5;
        goto L19066;}}
    else {
      arg1K0 = 5;
      goto L19066;}}
  else {
    arg1K0 = 5;
    goto L19066;}}
 L18899: {
  length_53X = arg0K0;
  if ((length_53X < (start_550X + count_551X))) {
    arg1K0 = 7;
    goto L18816;}
  else {
    got_54X = ps_write_fd(((((*((long *) (5 + channel_552X))))>>2)), ((-3 + arg4_424X) + start_550X), count_551X, &pendingP_55X, &status_56X);
    if ((status_56X == NO_ERRORS)) {
      if (pendingP_55X) {
        Spending_interruptPS = 0;
        Senabled_interruptsS = 0;
        arg1K0 = 13;
        goto L18816;}
      else {
        RSvalS = (((got_54X)<<2));
        goto L11161;}}
    else {
      arg1K0 = 21;
      goto L18816;}}}
 L18895: {
  arg0K0 = ((long)(((unsigned long)(*((long *) (-7 + arg4_424X))))>>8));
  goto L18899;}
 L6643: {
  val_271X = *((long *) (9 + ch_274X));
  addr_272X = 9 + prev_275X;
  WRITE_BARRIER(addr_272X, val_271X);
  *((long *) addr_272X) = val_271X;
  addr_273X = 9 + ch_274X;
  WRITE_BARRIER(addr_273X, 1);
  *((long *) addr_273X) = 1;
  arg0K0 = (*((long *) (13 + ch_274X)));
  goto L30439;}
 L18184: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 164;
  goto push_exception_continuationB;
 push_exception_continuationB_return_164:
  *((long *) (RSstackS)) = arg2_146X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_145X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L13986: {
  table_81X = arg0K0;
  symbols_82X = arg0K1;
  firstP_83X = arg2K2;
  arg0K0 = 0;
  goto L13990;}
 L17421: {
  minutesP_155X = arg2K0;
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_156X = check_events0_return_value;
      if (temp_156X) {
        goto L17436;}
      else {
        goto L17460;}}
    else {
      goto L17436;}}
  else {
    goto L17460;}}
 L16591: {
  len_160X = 1 + n_164X;
  addr_161X = ALLOCATE_SPACE(16, (4 + len_160X));
  *((long *) addr_161X) = (66 + (((len_160X)<<8)));
  string_162X = 7 + addr_161X;
  *((unsigned char *) ((-3 + string_162X) + n_164X)) = 0;
  arg0K0 = arg2_163X;
  arg0K1 = (-1 + n_164X);
  goto L16528;}
 L16503: {
  if ((25 == arg2_163X)) {
    goto L16591;}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 5;
    push_exception_continuationB_return_tag = 165;
    goto push_exception_continuationB;
   push_exception_continuationB_return_165:
    *((long *) (RSstackS)) = arg2_163X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (((n_164X)<<2));
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}}
 L9824: {
  i_172X = arg0K0;
  h_173X = arg0K1;
  if ((i_172X < n_185X)) {
    arg0K0 = (1 + i_172X);
    arg0K1 = (h_173X + (((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + arg2_181X) + i_172X))))))));
    goto L9824;}
  else {
    index_175X = h_173X & (-1 + v_184X);
    bucket_174X = *((long *) ((-3 + table_183X) + (((index_175X)<<2))));
    arg0K0 = bucket_174X;
    goto L9787;}}
 L16360: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 166;
  goto push_exception_continuationB;
 push_exception_continuationB_return_166:
  *((long *) (RSstackS)) = arg2_181X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12977;}
 L14806: {
  if ((from_index_494X < 0)) {
    goto L14868;}
  else {
    if ((to_index_495X < 0)) {
      goto L14868;}
    else {
      if ((count_496X < 0)) {
        goto L14868;}
      else {
        if ((3 == (3 & arg5_463X))) {
          if ((16 == (31 & ((((*((long *) (-7 + arg5_463X))))>>2))))) {
            arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg5_463X))))>>8)));
            goto L14829;}
          else {
            goto L14825;}}
        else {
          goto L14825;}}}}}
 L14801: {
  if ((3 == (3 & arg5_463X))) {
    if ((17 == (31 & ((((*((long *) (-7 + arg5_463X))))>>2))))) {
      goto L14806;}
    else {
      goto L14868;}}
  else {
    goto L14868;}}
 L14868: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 167;
  goto push_exception_continuationB;
 push_exception_continuationB_return_167:
  *((long *) (RSstackS)) = arg5_463X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((from_index_494X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_465X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((to_index_495X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((count_496X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 5;
  arg1K1 = 0;
  goto L12977;}
 L13057: {
  if (((Sstack_limitS) < (-296 + (RSstackS)))) {
    goto L13059;}
  else {
    space_691X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
    v_690X = AVAILABLEp(space_691X);
    if (v_690X) {
      arg1K0 = 0;
      goto L13142;}
    else {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 19;
      goto collect_saving_temp;
     collect_saving_temp_return_19:
      okayP_689X = AVAILABLEp(space_691X);
      if (okayP_689X) {
        arg1K0 = 0;
        goto L13142;}
      else {
        ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
        arg1K0 = 0;
        goto L13142;}}}}
 L13134: {
  merged_arg3K0 = ((unsigned char *) "exception handler is not a closure");
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L13057;}
 L11766: {
  env_231X = RSenvS;
  v_232X = AVAILABLEp(3);
  if (v_232X) {
    goto L11725;}
  else {
    merged_arg0K0 = 0;
    collect_saving_temp_return_tag = 20;
    goto collect_saving_temp;
   collect_saving_temp_return_20:
    okayP_233X = AVAILABLEp(3);
    if (okayP_233X) {
      goto L11725;}
    else {
      ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
      goto L11725;}}}
 L11596: {
  v_245X = arg0K0;
  RSvalS = v_245X;
  goto L11161;}
 L11678: {
  env_240X = arg0K0;
  i_241X = arg0K1;
  if ((0 == i_241X)) {
    b_242X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    value_243X = *((long *) ((-3 + env_240X) + ((((((unsigned long) b_242X)))<<2))));
    addr_244X = (-3 + new_env_248X) + (((i_249X)<<2));
    WRITE_BARRIER(addr_244X, value_243X);
    *((long *) addr_244X) = value_243X;
    arg0K0 = (1 + i_249X);
    goto L11569;}
  else {
    arg0K0 = (*((long *) (-3 + env_240X)));
    arg0K1 = (-1 + i_241X);
    goto L11678;}}
 L11129: {
  i_203X = arg0K0;
  l_204X = arg0K1;
  if ((0 < i_203X)) {
    *((long *) (RSstackS)) = (*((long *) (-3 + l_204X)));
    RSstackS = (-4 + (RSstackS));
    arg0K0 = (-1 + i_203X);
    arg0K1 = (*((long *) (1 + l_204X)));
    goto L11129;}
  else {
    if ((0 == overflow_208X)) {
      SnargsS = nargs_213X;
      goto L13625;}
    else {
      *((long *) (RSstackS)) = l_204X;
      RSstackS = (-4 + (RSstackS));
      space_205X = 3 * overflow_208X;
      v_206X = AVAILABLEp(space_205X);
      if (v_206X) {
        goto L10766;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 21;
        goto collect_saving_temp;
       collect_saving_temp_return_21:
        okayP_207X = AVAILABLEp(space_205X);
        if (okayP_207X) {
          goto L10766;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L10766;}}}}}
 L26500: {
  SnargsS = nargs_683X;
  RSvalS = (*((long *) (13 + cont_675X)));
  arg1K0 = 0;
  goto L12631;}
 L26559: {
  args_671X = arg0K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = 4;
  push_exception_continuationB_return_tag = 168;
  goto push_exception_continuationB;
 push_exception_continuationB_return_168:
  *((long *) (RSstackS)) = args_671X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L29377: {
  if ((64 == nargs_683X)) {
    RSstackS = (4 + (RSstackS));
    merged_arg0K0 = (*((long *) (RSstackS)));
    merged_arg0K1 = 63;
    pop_args_list_return_tag = 7;
    goto pop_args_list;
   pop_args_list_return_7:
    v_673X = pop_args_list0_return_value;
    arg0K0 = v_673X;
    goto L29406;}
  else {
    merged_arg0K0 = 25;
    merged_arg0K1 = nargs_683X;
    pop_args_list_return_tag = 8;
    goto pop_args_list;
   pop_args_list_return_8:
    v_672X = pop_args_list0_return_value;
    arg0K0 = v_672X;
    goto L29406;}}
 L10984: {
  args_on_stack_715X = arg0K0;
  if (((Sstack_limitS) < (-276 + (RSstackS)))) {
    arg0K0 = 0;
    goto L10988;}
  else {
    space_718X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
    v_717X = AVAILABLEp(space_718X);
    if (v_717X) {
      arg1K0 = 0;
      goto L11026;}
    else {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 22;
      goto collect_saving_temp;
     collect_saving_temp_return_22:
      okayP_716X = AVAILABLEp(space_718X);
      if (okayP_716X) {
        arg1K0 = 0;
        goto L11026;}
      else {
        ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
        arg1K0 = 0;
        goto L11026;}}}}
 L25844: {
  if ((3 == (3 & x_640X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_640X))))>>2))))) {
      arg1K0 = 5;
      goto L25873;}
    else {
      arg1K0 = 1;
      goto L25873;}}
  else {
    arg1K0 = 1;
    goto L25873;}}
 L25512: {
  if ((3 == (3 & x_638X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_638X))))>>2))))) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 169;
      goto push_exception_continuationB;
     push_exception_continuationB_return_169:
      *((long *) (RSstackS)) = x_638X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12977;}
    else {
      goto L25520;}}
  else {
    goto L25520;}}
 L25303: {
  if ((3 == (3 & x_637X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_637X))))>>2))))) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 170;
      goto push_exception_continuationB;
     push_exception_continuationB_return_170:
      *((long *) (RSstackS)) = x_637X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12977;}
    else {
      goto L25311;}}
  else {
    goto L25311;}}
 L25094: {
  if ((3 == (3 & x_636X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_636X))))>>2))))) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 171;
      goto push_exception_continuationB;
     push_exception_continuationB_return_171:
      *((long *) (RSstackS)) = x_636X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12977;}
    else {
      goto L25102;}}
  else {
    goto L25102;}}
 L5418: {
  if ((536870911 < lo_c_283X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 172;
    goto push_exception_continuationB;
   push_exception_continuationB_return_172:
    *((long *) (RSstackS)) = arg2_362X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_632X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}
  else {
    if ((lo_c_283X < 0)) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 6;
      push_exception_continuationB_return_tag = 173;
      goto push_exception_continuationB;
     push_exception_continuationB_return_173:
      *((long *) (RSstackS)) = arg2_362X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = y_632X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12977;}
    else {
      if ((8192 < mid_c_286X)) {
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 174;
        goto push_exception_continuationB;
       push_exception_continuationB_return_174:
        *((long *) (RSstackS)) = arg2_362X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = y_632X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}
      else {
        if ((a_633X < 0)) {
          if ((b_634X < 0)) {
            goto L5445;}
          else {
            goto L5452;}}
        else {
          if ((b_634X < 0)) {
            goto L5452;}
          else {
            goto L5445;}}}}}}
 L7153: {
  if ((536870911 < c_266X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 175;
    goto push_exception_continuationB;
   push_exception_continuationB_return_175:
    *((long *) (RSstackS)) = arg2_364X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_627X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}
  else {
    arg0K0 = (((c_266X)<<2));
    goto L7127;}}
 L7152: {
  arg0K0 = ((((0 - c_266X))<<2));
  goto L7127;}
 L28817: {
  if ((536870911 < c_30X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 176;
    goto push_exception_continuationB;
   push_exception_continuationB_return_176:
    *((long *) (RSstackS)) = arg2_367X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = x_622X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}
  else {
    RSvalS = (((c_30X)<<2));
    goto L11161;}}
 L28816: {
  RSvalS = ((((0 - c_30X))<<2));
  goto L11161;}
 L28686: {
  n_32X = arg0K0;
  RSvalS = (((n_32X)<<2));
  goto L11161;}
 L19584: {
  index_137X = arg0K0;
  temp_138X = index_137X < (Snumber_of_channelsS);
  if (temp_138X) {
    goto L8695;}
  else {
    x_139X = 1 + index_137X;
    y_140X = 8 + (Snumber_of_channelsS);
    if ((x_139X < y_140X)) {
      arg0K0 = y_140X;
      goto L1929;}
    else {
      arg0K0 = x_139X;
      goto L1929;}}}
 L19642: {
  v_130X = ps_open_fd((((unsigned char *)(-3 + arg2_133X))), 1, &v_129X);
  arg0K0 = v_130X;
  arg0K1 = v_129X;
  goto L19655;}
 L19655: {
  channel_135X = arg0K0;
  status_136X = arg0K1;
  if ((status_136X == NO_ERRORS)) {
    arg0K0 = channel_135X;
    goto L19584;}
  else {
    if ((status_136X == ENOENT)) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 10;
      push_exception_continuationB_return_tag = 177;
      goto push_exception_continuationB;
     push_exception_continuationB_return_177:
      *((long *) (RSstackS)) = arg2_133X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_134X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12977;}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 21;
      push_exception_continuationB_return_tag = 178;
      goto push_exception_continuationB;
     push_exception_continuationB_return_178:
      *((long *) (RSstackS)) = arg2_133X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_134X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12977;}}}
 L19137: {
  if ((3 == (3 & arg5_557X))) {
    if ((16 == (31 & ((((*((long *) (-7 + arg5_557X))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg5_557X))))>>8)));
      goto L19149;}
    else {
      goto L19145;}}
  else {
    goto L19145;}}
 L19129: {
  if ((3 == (3 & arg5_557X))) {
    if ((17 == (31 & ((((*((long *) (-7 + arg5_557X))))>>2))))) {
      goto L19137;}
    else {
      arg1K0 = 5;
      goto L19066;}}
  else {
    arg1K0 = 5;
    goto L19066;}}
 L19066: {
  reason_47X = arg1K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = reason_47X;
  push_exception_continuationB_return_tag = 179;
  goto push_exception_continuationB;
 push_exception_continuationB_return_179:
  *((long *) (RSstackS)) = arg5_557X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((start_52X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((count_51X)<<2));
  RSstackS = (-4 + (RSstackS));
  if (waitP_50X) {
    arg1K0 = 5;
    goto L19081;}
  else {
    arg1K0 = 1;
    goto L19081;}}
 L13990: {
  i_75X = arg0K0;
  if ((i_75X == ((((3 + ((long)(((unsigned long)(*((long *) (-7 + symbols_82X))))>>8))))>>2)))) {
    RSvalS = 13;
    goto L11161;}
  else {
    v_80X = AVAILABLEp(3);
    if (v_80X) {
      obj_79X = *((long *) ((-3 + symbols_82X) + (((i_75X)<<2))));
      v_78X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + table_81X))))>>8))))>>2);
      s_77X = *((long *) (-3 + obj_79X));
      n_76X = -1 + ((long)(((unsigned long)(*((long *) (-7 + s_77X))))>>8));
      arg0K0 = 0;
      arg0K1 = 0;
      goto L10467;}
    else {
      if (firstP_83X) {
        arg0K0 = 1;
        goto L14011;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 8;
        push_exception_continuationB_return_tag = 180;
        goto push_exception_continuationB;
       push_exception_continuationB_return_180:
        *((long *) (RSstackS)) = table_81X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12977;}}}}
 L17436: {
  RSvalS = 13;
  goto L11161;}
 L17460: {
  if ((0 < (Spending_interruptsS))) {
    goto L17436;}
  else {
    wait_for_event((((arg2_521X)>>2)), minutesP_155X);
    goto L17436;}}
 L16528: {
  l_165X = arg0K0;
  i_166X = arg0K1;
  if ((i_166X < 0)) {
    RSvalS = string_162X;
    goto L11161;}
  else {
    *((unsigned char *) ((-3 + string_162X) + i_166X)) = (((unsigned long) ((((*((long *) (-3 + l_165X))))>>8))));
    arg0K0 = (*((long *) (1 + l_165X)));
    arg0K1 = (-1 + i_166X);
    goto L16528;}}
 L9787: {
  b_176X = arg0K0;
  if ((25 == b_176X)) {
    addr_179X = ALLOCATE_SPACE(1, 8);
    *((long *) addr_179X) = 1030;
    x_180X = 7 + addr_179X;
    *((long *) (-3 + x_180X)) = arg2_181X;
    if ((3 == (3 & x_180X))) {
      if ((0 == (128 & (*((long *) (-7 + x_180X)))))) {
        *((long *) (-7 + x_180X)) = (128 | (*((long *) (-7 + x_180X))));
        arg0K0 = x_180X;
        goto L9792;}
      else {
        arg0K0 = x_180X;
        goto L9792;}}
    else {
      arg0K0 = x_180X;
      goto L9792;}}
  else {
    s2_178X = *((long *) (-3 + (*((long *) (-3 + b_176X)))));
    len_177X = (long)(((unsigned long)(*((long *) (-7 + arg2_181X))))>>8);
    if ((len_177X == ((long)(((unsigned long)(*((long *) (-7 + s2_178X))))>>8)))) {
      if (((!memcmp((void *)(-3 + s2_178X), (void *)(-3 + arg2_181X),len_177X)))) {
        arg0K0 = (*((long *) (-3 + b_176X)));
        goto L16359;}
      else {
        goto L9805;}}
    else {
      goto L9805;}}}
 L14829: {
  y_192X = arg0K0;
  if ((y_192X < (from_index_494X + count_496X))) {
    goto L14868;}
  else {
    if ((3 == (3 & arg3_465X))) {
      if ((16 == (31 & ((((*((long *) (-7 + arg3_465X))))>>2))))) {
        arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg3_465X))))>>8)));
        goto L14846;}
      else {
        goto L14842;}}
    else {
      goto L14842;}}}
 L14825: {
  arg0K0 = ((long)(((unsigned long)(*((long *) (-7 + arg5_463X))))>>8));
  goto L14829;}
 L13059: {
  if ((Sexception_space_usedPS)) {
    x_688X = (Sstack_limitS) < (-296 + (RSstackS));
    if (x_688X) {
      goto L13154;}
    else {
      ps_error(((unsigned char *) "no space on stack to reserve exception space"), 0);
      goto L13154;}}
  else {
    arg1K0 = 0;
    goto L12631;}}
 L13142: {
  v_687X = arg1K0;
  merged_arg1K0 = v_687X;
  copy_stack_into_heap_return_tag = 2;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_2:
  goto L13059;}
 L11725: {
  b_226X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  b_227X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  a_228X = *((long *) ((-3 + (StemplateS)) + ((((((((((unsigned long) b_226X)))<<8)) + (((unsigned long) b_227X))))<<2))));
  addr_229X = ALLOCATE_SPACE(3, 12);
  *((long *) addr_229X) = 2062;
  x_230X = 7 + addr_229X;
  *((long *) (-3 + x_230X)) = a_228X;
  *((long *) (1 + x_230X)) = env_231X;
  if ((3 == (3 & x_230X))) {
    if ((0 == (128 & (*((long *) (-7 + x_230X)))))) {
      *((long *) (-7 + x_230X)) = (128 | (*((long *) (-7 + x_230X))));
      arg0K0 = x_230X;
      goto L11732;}
    else {
      arg0K0 = x_230X;
      goto L11732;}}
  else {
    arg0K0 = x_230X;
    goto L11732;}}
 L13625: {
  RSvalS = proc_648X;
  arg1K0 = 0;
  goto L12631;}
 L10766: {
  RSstackS = (4 + (RSstackS));
  list_193X = *((long *) (RSstackS));
  a_194X = *((long *) (-3 + list_193X));
  addr_195X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_195X) = 2050;
  x_196X = 7 + addr_195X;
  *((long *) (-3 + x_196X)) = a_194X;
  *((long *) (1 + x_196X)) = 25;
  arg0K0 = (*((long *) (1 + list_193X)));
  arg0K1 = x_196X;
  goto L10783;}
 L29406: {
  args_674X = arg0K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = 4;
  push_exception_continuationB_return_tag = 181;
  goto push_exception_continuationB;
 push_exception_continuationB_return_181:
  *((long *) (RSstackS)) = args_674X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12977;}
 L10988: {
  key_712X = arg0K0;
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (StemplateS);
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((SnargsS))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
  RSstackS = (-4 + (RSstackS));
  tem_713X = Sinterrupt_templateS;
  StemplateS = tem_713X;
  RScode_pointerS = (-3 + (*((long *) (-3 + tem_713X))));
  merged_arg0K0 = (RScode_pointerS);
  merged_arg0K1 = (5 + args_on_stack_715X);
  merged_arg0K2 = key_712X;
  push_continuationB_return_tag = 1;
  goto push_continuationB;
 push_continuationB_return_1:
  n_714X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L13275;}
 L11026: {
  v_711X = arg1K0;
  merged_arg1K0 = v_711X;
  copy_stack_into_heap_return_tag = 3;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_3:
  arg0K0 = 0;
  goto L10988;}
 L25520: {
  RSvalS = 1;
  goto L11161;}
 L25311: {
  RSvalS = 1;
  goto L11161;}
 L25102: {
  RSvalS = 1;
  goto L11161;}
 L5445: {
  if ((536870911 < c_287X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 182;
    goto push_exception_continuationB;
   push_exception_continuationB_return_182:
    *((long *) (RSstackS)) = arg2_362X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_632X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}
  else {
    RSvalS = (((c_287X)<<2));
    goto L11161;}}
 L5452: {
  if ((536870912 < c_287X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 183;
    goto push_exception_continuationB;
   push_exception_continuationB_return_183:
    *((long *) (RSstackS)) = arg2_362X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_632X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}
  else {
    RSvalS = ((((0 - c_287X))<<2));
    goto L11161;}}
 L7127: {
  q_268X = arg0K0;
  if ((0 == y_627X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 184;
    goto push_exception_continuationB;
   push_exception_continuationB_return_184:
    *((long *) (RSstackS)) = arg2_364X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_627X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}
  else {
    a_269X = ((arg2_364X)>>2);
    if ((a_269X < 0)) {
      arg0K0 = (0 - a_269X);
      goto L7184;}
    else {
      arg0K0 = a_269X;
      goto L7184;}}}
 L8695: {
  if ((1 == (*((Svm_channelsS) + index_137X)))) {
    addr_125X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_125X) = 5142;
    x_126X = 7 + addr_125X;
    *((long *) (-3 + x_126X)) = (((mode_134X)<<2));
    *((long *) (1 + x_126X)) = arg2_133X;
    *((long *) (5 + x_126X)) = (((index_137X)<<2));
    *((long *) (9 + x_126X)) = 1;
    *((long *) (13 + x_126X)) = 1;
    *((Svm_channelsS) + index_137X) = x_126X;
    arg0K0 = x_126X;
    arg1K1 = 9;
    goto L19588;}
  else {
    arg0K0 = 1;
    arg1K1 = 11;
    goto L19588;}}
 L1929: {
  new_count_121X = arg0K0;
  new_vm_channels_122X = (long*)malloc(sizeof(long) * new_count_121X);
  if ((NULL == new_vm_channels_122X)) {
    arg0K0 = 1;
    arg1K1 = 9;
    goto L19588;}
  else {
    arg0K0 = 0;
    goto L1941;}}
 L19149: {
  length_42X = arg0K0;
  if ((length_42X < (start_52X + count_51X))) {
    arg1K0 = 7;
    goto L19066;}
  else {
    got_46X = ps_read_fd(((((*((long *) (5 + x_48X))))>>2)), ((-3 + arg5_557X) + start_52X), count_51X, waitP_50X, &eofP_45X, &pendingP_44X, &status_43X);
    if ((status_43X == NO_ERRORS)) {
      if (eofP_45X) {
        RSvalS = 21;
        goto L11161;}
      else {
        if (pendingP_44X) {
          Spending_interruptPS = 0;
          Senabled_interruptsS = 0;
          arg1K0 = 13;
          goto L19066;}
        else {
          RSvalS = (((got_46X)<<2));
          goto L11161;}}}
    else {
      arg1K0 = 21;
      goto L19066;}}}
 L19145: {
  arg0K0 = ((long)(((unsigned long)(*((long *) (-7 + arg5_557X))))>>8));
  goto L19149;}
 L19081: {
  v_41X = arg1K0;
  *((long *) (RSstackS)) = (((unsigned long) v_41X));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_48X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 5;
  arg1K1 = 0;
  goto L12977;}
 L10467: {
  i_61X = arg0K0;
  h_62X = arg0K1;
  if ((i_61X < n_76X)) {
    arg0K0 = (1 + i_61X);
    arg0K1 = (h_62X + (((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + s_77X) + i_61X))))))));
    goto L10467;}
  else {
    index_64X = h_62X & (-1 + v_78X);
    bucket_63X = *((long *) ((-3 + table_81X) + (((index_64X)<<2))));
    arg0K0 = bucket_63X;
    goto L10428;}}
 L14011: {
  i_69X = arg0K0;
  if ((i_69X == ((((3 + ((long)(((unsigned long)(*((long *) (-7 + table_81X))))>>8))))>>2)))) {
    addr_71X = -3 + table_81X;
    WRITE_BARRIER(addr_71X, symbols_82X);
    *((long *) addr_71X) = symbols_82X;
    merged_arg0K0 = table_81X;
    collect_saving_temp_return_tag = 23;
    goto collect_saving_temp;
   collect_saving_temp_return_23:
    table_72X = collect_saving_temp0_return_value;
    symbols_73X = *((long *) (-3 + table_72X));
    addr_74X = -3 + table_72X;
    WRITE_BARRIER(addr_74X, 25);
    *((long *) addr_74X) = 25;
    arg0K0 = table_72X;
    arg0K1 = symbols_73X;
    arg2K2 = 0;
    goto L13986;}
  else {
    addr_70X = (-3 + table_81X) + (((i_69X)<<2));
    WRITE_BARRIER(addr_70X, 25);
    *((long *) addr_70X) = 25;
    arg0K0 = (1 + i_69X);
    goto L14011;}}
 L9792: {
  new_168X = arg0K0;
  addr_169X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_169X) = 2050;
  x_170X = 7 + addr_169X;
  *((long *) (-3 + x_170X)) = new_168X;
  *((long *) (1 + x_170X)) = bucket_174X;
  addr_171X = (-3 + table_183X) + (((index_175X)<<2));
  WRITE_BARRIER(addr_171X, x_170X);
  *((long *) addr_171X) = x_170X;
  arg0K0 = new_168X;
  goto L16359;}
 L16359: {
  val_167X = arg0K0;
  RSvalS = val_167X;
  goto L11161;}
 L9805: {
  arg0K0 = (*((long *) (1 + b_176X)));
  goto L9787;}
 L14846: {
  y_191X = arg0K0;
  if ((y_191X < (to_index_495X + count_496X))) {
    goto L14868;}
  else {
    memcpy((void *)((-3 + arg3_465X) + to_index_495X), (void *)((-3 + arg5_463X) + from_index_494X),count_496X);
    RSvalS = 13;
    goto L11161;}}
 L14842: {
  arg0K0 = ((long)(((unsigned long)(*((long *) (-7 + arg3_465X))))>>8));
  goto L14846;}
 L13154: {
  Sstack_limitS = (296 + (Sstack_beginS));
  Sexception_space_usedPS = 0;
  arg1K0 = 0;
  goto L12631;}
 L11732: {
  v_225X = arg0K0;
  RSvalS = v_225X;
  goto L11161;}
 L10783: {
  l_197X = arg0K0;
  last_198X = arg0K1;
  if ((25 == l_197X)) {
    *((long *) (RSstackS)) = x_196X;
    RSstackS = (-4 + (RSstackS));
    SnargsS = 64;
    goto L13625;}
  else {
    a_202X = *((long *) (-3 + l_197X));
    addr_201X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_201X) = 2050;
    x_200X = 7 + addr_201X;
    *((long *) (-3 + x_200X)) = a_202X;
    *((long *) (1 + x_200X)) = 25;
    addr_199X = 1 + last_198X;
    WRITE_BARRIER(addr_199X, x_200X);
    *((long *) addr_199X) = x_200X;
    arg0K0 = (*((long *) (1 + l_197X)));
    arg0K1 = x_200X;
    goto L10783;}}
 L13275: {
  i_721X = arg0K0;
  m_722X = arg0K1;
  if ((0 == (n_714X & m_722X))) {
    arg0K0 = (1 + i_721X);
    arg0K1 = (((m_722X)<<1));
    goto L13275;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_722X));
    if ((i_721X == 0)) {
      *((long *) (RSstackS)) = (Sinterrupted_templateS);
      RSstackS = (-4 + (RSstackS));
      Sinterrupted_templateS = 1;
      *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      goto L12824;}
    else {
      if ((i_721X == 2)) {
        *((long *) (RSstackS)) = (Sfinalize_theseS);
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
        RSstackS = (-4 + (RSstackS));
        Sfinalize_theseS = 25;
        arg1K0 = 2;
        goto L12824;}
      else {
        if ((i_721X == 4)) {
          channel_723X = Spending_channels_headS;
          next_724X = *((long *) (9 + channel_723X));
          Spending_channels_headS = next_724X;
          addr_725X = 9 + channel_723X;
          WRITE_BARRIER(addr_725X, 1);
          *((long *) addr_725X) = 1;
          if ((1 == next_724X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_723X;
            goto L6445;}
          else {
            arg0K0 = channel_723X;
            goto L6445;}}
        else {
          *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 1;
          goto L12824;}}}}}
 L7184: {
  a_263X = arg0K0;
  n_264X = ((y_627X)>>2);
  if ((n_264X < 0)) {
    arg0K0 = (0 - n_264X);
    goto L7186;}
  else {
    arg0K0 = n_264X;
    goto L7186;}}
 L19588: {
  channel_118X = arg0K0;
  reason_119X = arg1K1;
  if ((1 == channel_118X)) {
    if ((3 == (3 & arg2_133X))) {
      if ((16 == (31 & ((((*((long *) (-7 + arg2_133X))))>>2))))) {
        if ((1 == mode_134X)) {
          goto L19733;}
        else {
          if ((3 == mode_134X)) {
            goto L19733;}
          else {
            v_120X = ps_close_fd(index_137X);
            arg0K0 = v_120X;
            goto L19728;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = reason_119X;
        push_exception_continuationB_return_tag = 185;
        goto push_exception_continuationB;
       push_exception_continuationB_return_185:
        *((long *) (RSstackS)) = arg2_133X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (((mode_134X)<<2));
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12977;}}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = reason_119X;
      push_exception_continuationB_return_tag = 186;
      goto push_exception_continuationB;
     push_exception_continuationB_return_186:
      *((long *) (RSstackS)) = arg2_133X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_134X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12977;}}
  else {
    RSvalS = channel_118X;
    goto L11161;}}
 L1941: {
  i_124X = arg0K0;
  if ((i_124X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L1956;}
  else {
    *(new_vm_channels_122X + i_124X) = (*((Svm_channelsS) + i_124X));
    arg0K0 = (1 + i_124X);
    goto L1941;}}
 L10428: {
  b_65X = arg0K0;
  if ((25 == b_65X)) {
    addr_66X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_66X) = 2050;
    x_67X = 7 + addr_66X;
    *((long *) (-3 + x_67X)) = obj_79X;
    *((long *) (1 + x_67X)) = bucket_63X;
    addr_68X = (-3 + table_81X) + (((index_64X)<<2));
    WRITE_BARRIER(addr_68X, x_67X);
    *((long *) addr_68X) = x_67X;
    goto L14005;}
  else {
    if ((obj_79X == (*((long *) (-3 + b_65X))))) {
      goto L14005;}
    else {
      arg0K0 = (*((long *) (1 + b_65X)));
      goto L10428;}}}
 L12824: {
  v_706X = arg1K0;
  SnargsS = (((unsigned long) v_706X));
  obj_707X = Sinterrupt_handlersS;
  if ((3 == (3 & obj_707X))) {
    if ((2 == (31 & ((((*((long *) (-7 + obj_707X))))>>2))))) {
      goto L12839;}
    else {
      goto L12867;}}
  else {
    goto L12867;}}
 L6445: {
  channel_708X = arg0K0;
  x_709X = 1 == (Spending_channels_headS);
  if (x_709X) {
    goto L6459;}
  else {
    Spending_interruptsS = (16 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        goto L6459;}
      else {
        goto L6459;}}
    else {
      Spending_interruptPS = 1;
      goto L6459;}}}
 L7186: {
  b_261X = arg0K0;
  c_262X = a_263X % b_261X;
  if ((a_269X < 0)) {
    arg0K0 = (0 - c_262X);
    goto L7190;}
  else {
    arg0K0 = c_262X;
    goto L7190;}}
 L19733: {
  v_131X = ps_close_fd(index_137X);
  arg0K0 = v_131X;
  goto L19728;}
 L19728: {
  status_132X = arg0K0;
  if ((status_132X == NO_ERRORS)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = reason_119X;
    push_exception_continuationB_return_tag = 187;
    goto push_exception_continuationB;
   push_exception_continuationB_return_187:
    *((long *) (RSstackS)) = arg2_133X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (((mode_134X)<<2));
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}
  else {
    merged_arg0K0 = status_132X;
    merged_arg0K1 = index_137X;
    merged_arg0K2 = arg2_133X;
    channel_close_error_return_tag = 0;
    goto channel_close_error;
   channel_close_error_return_0:
    merged_arg1K0 = 0;
    merged_arg1K1 = reason_119X;
    push_exception_continuationB_return_tag = 188;
    goto push_exception_continuationB;
   push_exception_continuationB_return_188:
    *((long *) (RSstackS)) = arg2_133X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (((mode_134X)<<2));
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}}
 L1956: {
  i_123X = arg0K0;
  if ((i_123X == new_count_121X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_122X;
    Snumber_of_channelsS = new_count_121X;
    goto L8695;}
  else {
    *(new_vm_channels_122X + i_123X) = 1;
    arg0K0 = (1 + i_123X);
    goto L1956;}}
 L14005: {
  arg0K0 = (1 + i_75X);
  goto L13990;}
 L12839: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    Spending_interruptPS = 0;
    if ((Spending_eventsPS)) {
      Spending_interruptPS = 1;
      goto L12841;}
    else {
      goto L12841;}}
  else {
    Spending_interruptPS = 1;
    goto L12841;}}
 L12867: {
  ps_error(((unsigned char *) "interrupt handler is not a vector"), 0);
  goto L12839;}
 L6459: {
  *((long *) (RSstackS)) = channel_708X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (*((long *) (13 + channel_708X)));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  goto L12824;}
 L7190: {
  n_260X = arg0K0;
  if ((0 == (((n_260X)<<2)))) {
    RSvalS = q_268X;
    goto L11161;}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 189;
    goto push_exception_continuationB;
   push_exception_continuationB_return_189:
    *((long *) (RSstackS)) = arg2_364X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_627X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12977;}}
 L12841: {
  RSvalS = (*((long *) ((-3 + (Sinterrupt_handlersS)) + (((i_721X)<<2)))));
  obj_710X = RSvalS;
  if ((3 == (3 & obj_710X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_710X))))>>2))))) {
      arg1K0 = 0;
      goto L12631;}
    else {
      goto L12892;}}
  else {
    goto L12892;}}
 L12892: {
  ps_error(((unsigned char *) "interrupt handler is not a closure"), 1, i_721X);
  arg1K0 = 0;
  goto L12631;}
 loseD0: {
  message_862X = merged_arg3K0;{
  opcode_726X = (((*((long *) (8 + ((RSstackS) + ((((((unsigned long) nargs_693X)))<<2)))))))>>2);
  why_727X = (((*((long *) (4 + ((RSstackS) + ((((((unsigned long) nargs_693X)))<<2)))))))>>2);
  ps_write_string(((unsigned char *) "Template UIDs: "), (stderr));
  current_template_728X = StemplateS;
  out_729X = stderr;
  if ((0 == (3 & (*((long *) (1 + current_template_728X)))))) {
    if ((current_template_728X == (*((long *) (5 + (Sbottom_of_stackS)))))) {
      arg2K0 = 0;
      goto L8146;}
    else {
      ps_write_integer(((((*((long *) (1 + current_template_728X))))>>2)), out_729X);
      arg2K0 = 1;
      goto L8146;}}
  else {
    arg2K0 = 0;
    goto L8146;}}
 L8146: {
  not_firstP_732X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_732X;
  goto L8153;}
 L8153: {
  cont_733X = arg0K0;
  not_firstP_734X = arg2K1;
  if ((3 == (3 & cont_733X))) {
    if ((9 == (31 & ((((*((long *) (-7 + cont_733X))))>>2))))) {
      template_735X = *((long *) (5 + cont_733X));
      if ((0 == (3 & (*((long *) (1 + template_735X)))))) {
        if ((template_735X == (*((long *) (5 + (Sbottom_of_stackS)))))) {
          arg2K0 = not_firstP_734X;
          goto L8162;}
        else {
          if (not_firstP_734X) {
            ps_write_string(((unsigned char *) " <- "), out_729X);
            goto L8201;}
          else {
            goto L8201;}}}
      else {
        arg2K0 = not_firstP_734X;
        goto L8162;}}
    else {
      goto L12995;}}
  else {
    goto L12995;}}
 L8162: {
  v_731X = arg2K0;
  arg0K0 = (*((long *) (-3 + cont_733X)));
  arg2K1 = v_731X;
  goto L8153;}
 L8201: {
  ps_write_integer(((((*((long *) (1 + template_735X))))>>2)), out_729X);
  arg2K0 = 1;
  goto L8162;}
 L12995: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_727X == 1)) {
    if ((0 == (3 & (*((long *) (-3 + (*((long *) ((RSstackS) + ((((((unsigned long) nargs_693X)))<<2))))))))))) {
      ps_error(message_862X, 3, opcode_726X, why_727X, ((((*((long *) (-3 + (*((long *) ((RSstackS) + ((((((unsigned long) nargs_693X)))<<2)))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L13019;}}
  else {
    goto L13019;}}
 L13019: {
  ps_error(message_862X, 2, opcode_726X, why_727X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L31985;}
 L31985: {
  type_743X = get_next_event(&channel_742X, &status_741X);
  if ((type_743X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L12532;}
    else {
      arg0K0 = 1;
      goto L12532;}}
  else {
    if ((type_743X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L12532;}
    else {
      if ((type_743X == IO_COMPLETION_EVENT)) {
        channel_738X = *((Svm_channelsS) + channel_742X);
        val_737X = ((status_741X)<<2);
        addr_736X = 13 + channel_738X;
        WRITE_BARRIER(addr_736X, val_737X);
        *((long *) addr_736X) = val_737X;
        if ((1 == (*((long *) (9 + channel_738X))))) {
          if ((1 == (Spending_channels_headS))) {
            Spending_channels_headS = channel_738X;
            Spending_channels_tailS = channel_738X;
            arg0K0 = 16;
            goto L12532;}
          else {
            addr_745X = 9 + (Spending_channels_tailS);
            WRITE_BARRIER(addr_745X, channel_738X);
            *((long *) addr_745X) = channel_738X;
            Spending_channels_tailS = channel_738X;
            arg0K0 = 16;
            goto L12532;}}
        else {
          arg0K0 = 16;
          goto L12532;}}
      else {
        if ((type_743X == NO_EVENT)) {
          arg0K0 = 0;
          goto L12532;}
        else {
          if ((type_743X == ERROR_EVENT)) {
            ps_write_string(((unsigned char *) "OS error while getting event"), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            ps_write_string((ps_error_string(status_741X)), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = 0;
            goto L12532;}
          else {
            ps_write_string(((unsigned char *) "unknown type of event"), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = 0;
            goto L12532;}}}}}}
 L12532: {
  v_746X = arg0K0;
  Spending_interruptsS = (v_746X | (Spending_interruptsS));
  if ((type_743X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        goto L12559;}
      else {
        goto L12559;}}
    else {
      Spending_interruptPS = 1;
      goto L12559;}}
  else {
    goto L31985;}}
 L12559: {
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      goto L31985;}
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
  default: goto check_events_return_2;
  }}

 pop_args_list: {
  start_860X = merged_arg0K0;
  count_861X = merged_arg0K1;{
  space_747X = 3 * (SnargsS);
  v_748X = AVAILABLEp(space_747X);
  if (v_748X) {
    goto L10898;}
  else {
    merged_arg0K0 = 0;
    collect_saving_temp_return_tag = 24;
    goto collect_saving_temp;
   collect_saving_temp_return_24:
    okayP_749X = AVAILABLEp(space_747X);
    if (okayP_749X) {
      goto L10898;}
    else {
      ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
      goto L10898;}}}
 L10898: {
  arg0K0 = start_860X;
  arg0K1 = count_861X;
  goto L10905;}
 L10905: {
  args_750X = arg0K0;
  count_751X = arg0K1;
  if ((0 < count_751X)) {
    RSstackS = (4 + (RSstackS));
    a_752X = *((long *) (RSstackS));
    addr_753X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_753X) = 2050;
    x_754X = 7 + addr_753X;
    *((long *) (-3 + x_754X)) = a_752X;
    *((long *) (1 + x_754X)) = args_750X;
    arg0K0 = x_754X;
    arg0K1 = (-1 + count_751X);
    goto L10905;}
  else {
    pop_args_list0_return_value = args_750X;
    goto pop_args_list_return;}}
 pop_args_list_return:
  switch (pop_args_list_return_tag) {
  case 0: goto pop_args_list_return_0;
  case 1: goto pop_args_list_return_1;
  case 2: goto pop_args_list_return_2;
  case 3: goto pop_args_list_return_3;
  case 4: goto pop_args_list_return_4;
  case 5: goto pop_args_list_return_5;
  case 6: goto pop_args_list_return_6;
  case 7: goto pop_args_list_return_7;
  default: goto pop_args_list_return_8;
  }}

 collect_saving_temp: {
  value_859X = merged_arg0K0;{
  pc_755X = 12 + (((((RScode_pointerS) - (*((long *) (-3 + (StemplateS))))))<<2));
  begin_collection();
  v_756X = trace_value((StemplateS));
  StemplateS = v_756X;
  v_757X = trace_value((RSvalS));
  RSvalS = v_757X;
  v_758X = trace_value((Scurrent_threadS));
  Scurrent_threadS = v_758X;
  v_759X = trace_value((Ssession_dataS));
  Ssession_dataS = v_759X;
  v_760X = trace_value((Sexception_handlersS));
  Sexception_handlersS = v_760X;
  v_761X = trace_value((Sinterrupt_handlersS));
  Sinterrupt_handlersS = v_761X;
  v_762X = trace_value((Sinterrupt_templateS));
  Sinterrupt_templateS = v_762X;
  v_763X = trace_value((Sinterrupted_templateS));
  Sinterrupted_templateS = v_763X;
  v_764X = trace_value((Sfinalize_theseS));
  Sfinalize_theseS = v_764X;
  env_765X = trace_value((RSenvS));
  RSenvS = env_765X;
  v_766X = trace_value((Spending_channels_headS));
  Spending_channels_headS = v_766X;
  v_767X = trace_value((Spending_channels_tailS));
  Spending_channels_tailS = v_767X;
  arg0K0 = (4 + (RSstackS));
  arg0K1 = 0;
  goto L6930;}
 L6930: {
  p_788X = arg0K0;
  i_789X = arg0K1;
  if ((2 == (3 & (*((long *) p_788X))))) {
    if ((54 == (*((long *) p_788X)))) {
      arg0K0 = (-1 + i_789X);
      goto L6865;}
    else {
      arg0K0 = i_789X;
      goto L6865;}}
  else {
    arg0K0 = (4 + p_788X);
    arg0K1 = (1 + i_789X);
    goto L6930;}}
 L6865: {
  arg_count_785X = arg0K0;
  trace_locationsB((4 + (RSstackS)), (4 + ((RSstackS) + (((arg_count_785X)<<2)))));
  p_786X = RSenvS;
  if ((3 == (3 & p_786X))) {
    if ((p_786X < (Sstack_beginS))) {
      goto L6883;}
    else {
      if (((Sstack_endS) < p_786X)) {
        goto L6883;}
      else {
        env_787X = RSenvS;
        arg0K0 = env_787X;
        goto L7047;}}}
  else {
    goto L6883;}}
 L6883: {
  v_782X = trace_value((RSenvS));
  RSenvS = v_782X;
  goto L6888;}
 L7047: {
  env_783X = arg0K0;
  if ((3 == (3 & env_783X))) {
    if ((env_783X < (Sstack_beginS))) {
      goto L6888;}
    else {
      if (((Sstack_endS) < env_783X)) {
        goto L6888;}
      else {
        trace_stob_contentsB(env_783X);
        arg0K0 = (*((long *) (-3 + env_783X)));
        goto L7047;}}}
  else {
    goto L6888;}}
 L6888: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L6893;}
 L6893: {
  cont_794X = arg0K0;
  last_env_795X = arg0K1;
  env_796X = *((long *) (9 + cont_794X));
  trace_stob_contentsB(cont_794X);
  if ((env_796X == last_env_795X)) {
    goto L6911;}
  else {
    arg0K0 = env_796X;
    goto L7002;}}
 L6911: {
  if ((cont_794X == (Sbottom_of_stackS))) {
    value_793X = trace_value(value_859X);
    okay_791X = do_gc((Sfinalizer_alistS), &sickly_792X);
    Sfinalizer_alistS = okay_791X;
    l2_790X = Sfinalize_theseS;
    if ((25 == sickly_792X)) {
      arg0K0 = l2_790X;
      goto L10350;}
    else {
      arg0K0 = sickly_792X;
      goto L10373;}}
  else {
    arg0K0 = (*((long *) (-3 + cont_794X)));
    arg0K1 = env_796X;
    goto L6893;}}
 L7002: {
  env_784X = arg0K0;
  if ((3 == (3 & env_784X))) {
    if ((env_784X < (Sstack_beginS))) {
      goto L6911;}
    else {
      if (((Sstack_endS) < env_784X)) {
        goto L6911;}
      else {
        trace_stob_contentsB(env_784X);
        arg0K0 = (*((long *) (-3 + env_784X)));
        goto L7002;}}}
  else {
    goto L6911;}}
 L10350: {
  v_797X = arg0K0;
  Sfinalize_theseS = v_797X;
  end_collection();
  arg0K0 = 0;
  goto L9984;}
 L10373: {
  l_798X = arg0K0;
  if ((25 == (*((long *) (1 + l_798X))))) {
    addr_799X = 1 + l_798X;
    WRITE_BARRIER(addr_799X, l2_790X);
    *((long *) addr_799X) = l2_790X;
    arg0K0 = sickly_792X;
    goto L10350;}
  else {
    arg0K0 = (*((long *) (1 + l_798X)));
    goto L10373;}}
 L9984: {
  i_777X = arg0K0;
  if ((i_777X == (Snumber_of_channelsS))) {
    tem_781X = StemplateS;
    StemplateS = tem_781X;
    RScode_pointerS = ((-3 + (*((long *) (-3 + tem_781X)))) + (((pc_755X)>>2)));
    Spending_interruptsS = (4 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        collect_saving_temp0_return_value = value_793X;
        goto collect_saving_temp_return;}
      else {
        collect_saving_temp0_return_value = value_793X;
        goto collect_saving_temp_return;}}
    else {
      Spending_interruptPS = 1;
      collect_saving_temp0_return_value = value_793X;
      goto collect_saving_temp_return;}}
  else {
    channel_778X = *((Svm_channelsS) + i_777X);
    if ((1 == channel_778X)) {
      goto L10020;}
    else {
      header_779X = *((long *) (-7 + channel_778X));
      if ((3 == (3 & header_779X))) {
        arg0K0 = header_779X;
        goto L10013;}
      else {
        if ((0 == (*((long *) (-3 + channel_778X))))) {
          arg0K0 = 1;
          goto L10013;}
        else {
          id_780X = *((long *) (1 + channel_778X));
          if ((0 == (3 & id_780X))) {
            arg0K0 = id_780X;
            goto L9568;}
          else {
            if ((3 == (3 & (*((long *) (-7 + id_780X)))))) {
              arg0K0 = (*((long *) (-7 + id_780X)));
              goto L9568;}
            else {
              arg0K0 = id_780X;
              goto L9568;}}}}}}}
 L10020: {
  arg0K0 = (1 + i_777X);
  goto L9984;}
 L10013: {
  new_776X = arg0K0;
  *((Svm_channelsS) + i_777X) = new_776X;
  goto L10020;}
 L9568: {
  id_772X = arg0K0;
  os_index_773X = (((*((long *) (5 + channel_778X))))>>2);
  v_774X = *((long *) (-3 + channel_778X));
  if ((4 == v_774X)) {
    goto L9627;}
  else {
    if ((12 == (*((long *) (-3 + channel_778X))))) {
      goto L9627;}
    else {
      v_775X = ps_close_fd(os_index_773X);
      arg0K0 = v_775X;
      goto L9616;}}}
 L9627: {
  v_768X = ps_close_fd(os_index_773X);
  arg0K0 = v_768X;
  goto L9616;}
 L9616: {
  status_769X = arg0K0;
  *((Svm_channelsS) + os_index_773X) = 1;
  addr_770X = -3 + channel_778X;
  WRITE_BARRIER(addr_770X, 0);
  *((long *) addr_770X) = 0;
  if ((status_769X == NO_ERRORS)) {
    goto L9583;}
  else {
    merged_arg0K0 = status_769X;
    merged_arg0K1 = (*((long *) (5 + channel_778X)));
    merged_arg0K2 = id_772X;
    channel_close_error_return_tag = 1;
    goto channel_close_error;
   channel_close_error_return_1:
    goto L9583;}}
 L9583: {
  ps_write_string(((unsigned char *) "Channel closed: "), (stderr));
  if ((0 == (3 & id_772X))) {
    ps_write_integer((((id_772X)>>2)), (stderr));
    goto L9601;}
  else {
    ps_write_string((((unsigned char *)(-3 + id_772X))), (stderr));
    goto L9601;}}
 L9601: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L10013;}
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
  default: goto collect_saving_temp_return_24;
  }}

 channel_close_error: {
  status_856X = merged_arg0K0;
  index_857X = merged_arg0K1;
  id_858X = merged_arg0K2;{
  ps_write_string(((unsigned char *) "Error: "), (stderr));
  ps_write_string((ps_error_string(status_856X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(((unsigned char *) " while closing port "), (stderr));
  if ((3 == (3 & id_858X))) {
    if ((16 == (31 & ((((*((long *) (-7 + id_858X))))>>2))))) {
      ps_write_string((((unsigned char *)(-3 + id_858X))), (stderr));
      goto L9040;}
    else {
      goto L9034;}}
  else {
    goto L9034;}}
 L9040: {
  PS_WRITE_CHAR(10, (stderr), v_801X)
  channel_close_error0_return_value = v_801X;
  goto channel_close_error_return;}
 L9034: {
  ps_write_integer((((index_857X)>>2)), (stderr));
  goto L9040;}
 channel_close_error_return:
  switch (channel_close_error_return_tag) {
  case 0: goto channel_close_error_return_0;
  default: goto channel_close_error_return_1;
  }}

 copy_stack_into_heap: {
  key_855X = merged_arg1K0;{
  arg0K0 = (4 + (RSstackS));
  arg0K1 = 0;
  goto L10594;}
 L10594: {
  p_807X = arg0K0;
  i_808X = arg0K1;
  if ((2 == (3 & (*((long *) p_807X))))) {
    if ((54 == (*((long *) p_807X)))) {
      arg0K0 = (-1 + i_808X);
      goto L10581;}
    else {
      arg0K0 = i_808X;
      goto L10581;}}
  else {
    arg0K0 = (4 + p_807X);
    arg0K1 = (1 + i_808X);
    goto L10594;}}
 L10581: {
  arg_count_805X = arg0K0;
  top_806X = RSstackS;
  if ((1 == (ScontS))) {
    goto L10586;}
  else {
    merged_arg1K0 = key_855X;
    merged_arg1K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L10586;}}
 L10586: {
  stob_804X = ScontS;
  arg0K0 = (-11 + stob_804X);
  arg0K1 = (top_806X + (((arg_count_805X)<<2)));
  goto L10619;}
 L10619: {
  loc_802X = arg0K0;
  arg_803X = arg0K1;
  if ((top_806X < arg_803X)) {
    *((long *) loc_802X) = (*((long *) arg_803X));
    arg0K0 = (-4 + loc_802X);
    arg0K1 = (-4 + arg_803X);
    goto L10619;}
  else {
    RSstackS = loc_802X;
    goto copy_stack_into_heap_return;}}
 copy_stack_into_heap_return:
  switch (copy_stack_into_heap_return_tag) {
  case 0: goto copy_stack_into_heap_return_0;
  case 1: goto copy_stack_into_heap_return_1;
  case 2: goto copy_stack_into_heap_return_2;
  default: goto copy_stack_into_heap_return_3;
  }}

 really_preserve_continuation: {
  key_853X = merged_arg1K0;
  reason_854X = merged_arg1K1;{
  p_809X = RSenvS;
  if ((3 == (3 & p_809X))) {
    if ((p_809X < (Sstack_beginS))) {
      goto L10199;}
    else {
      if (((Sstack_endS) < p_809X)) {
        goto L10199;}
      else {
        merged_arg0K0 = (RSenvS);
        merged_arg0K1 = (ScontS);
        merged_arg1K2 = key_853X;
        merged_arg1K3 = reason_854X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_810X = save_env_in_heap0_return_value;
        RSenvS = v_810X;
        goto L10199;}}}
  else {
    goto L10199;}}
 L10199: {
  end_811X = *((long *) (-3 + (Sbottom_of_stackS)));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L10140;}
 L10140: {
  cont_816X = arg0K0;
  previous_817X = arg0K1;
  if ((cont_816X == (Sbottom_of_stackS))) {
    *((long *) (-3 + previous_817X)) = end_811X;
    cont_818X = *((long *) (-3 + (Sbottom_of_stackS)));
    if ((1 == cont_818X)) {
      ScontS = (Sbottom_of_stackS);
      really_preserve_continuation0_return_value = cont_818X;
      goto really_preserve_continuation_return;}
    else {
      merged_arg0K0 = cont_818X;
      copy_continuation_from_heapB_return_tag = 2;
      goto copy_continuation_from_heapB;
     copy_continuation_from_heapB_return_2:
      really_preserve_continuation0_return_value = cont_818X;
      goto really_preserve_continuation_return;}}
  else {
    p_819X = *((long *) (9 + cont_816X));
    if ((3 == (3 & p_819X))) {
      if ((p_819X < (Sstack_beginS))) {
        goto L10158;}
      else {
        if (((Sstack_endS) < p_819X)) {
          goto L10158;}
        else {
          merged_arg0K0 = (*((long *) (9 + cont_816X)));
          merged_arg0K1 = cont_816X;
          merged_arg1K2 = key_853X;
          merged_arg1K3 = reason_854X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L10158;}}}
    else {
      goto L10158;}}}
 L10158: {
  header_812X = *((long *) (-7 + cont_816X));
  addr_813X = ALLOCATE_SPACE((31 & (((header_812X)>>2))), (4 + ((long)(((unsigned long)header_812X)>>8))));
  data_addr_814X = 4 + addr_813X;
  *((long *) addr_813X) = header_812X;
  memcpy((void *)data_addr_814X, (void *)(-3 + cont_816X),((long)(((unsigned long)header_812X)>>8)));
  new_815X = 3 + data_addr_814X;
  *((long *) (-3 + previous_817X)) = new_815X;
  arg0K0 = (*((long *) (-3 + new_815X)));
  arg0K1 = new_815X;
  goto L10140;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 save_env_in_heap: {
  env_851X = merged_arg0K0;
  cont_852X = merged_arg0K1;{
  header_820X = *((long *) (-7 + env_851X));
  addr_821X = ALLOCATE_SPACE((31 & (((header_820X)>>2))), (4 + ((long)(((unsigned long)header_820X)>>8))));
  data_addr_822X = 4 + addr_821X;
  *((long *) addr_821X) = header_820X;
  memcpy((void *)data_addr_822X, (void *)(-3 + env_851X),((long)(((unsigned long)header_820X)>>8)));
  new_823X = 3 + data_addr_822X;
  addr_824X = -3 + env_851X;
  WRITE_BARRIER(addr_824X, 54);
  *((long *) addr_824X) = 54;
  *((long *) (-7 + env_851X)) = new_823X;
  arg0K0 = new_823X;
  goto L9319;}
 L9319: {
  env_827X = arg0K0;
  p_828X = *((long *) (-3 + env_827X));
  if ((3 == (3 & p_828X))) {
    if ((p_828X < (Sstack_beginS))) {
      goto L9338;}
    else {
      if (((Sstack_endS) < p_828X)) {
        goto L9338;}
      else {
        env_829X = *((long *) (-3 + env_827X));
        header_830X = *((long *) (-7 + env_829X));
        addr_831X = ALLOCATE_SPACE((31 & (((header_830X)>>2))), (4 + ((long)(((unsigned long)header_830X)>>8))));
        data_addr_832X = 4 + addr_831X;
        *((long *) addr_831X) = header_830X;
        memcpy((void *)data_addr_832X, (void *)(-3 + env_829X),((long)(((unsigned long)header_830X)>>8)));
        new_833X = 3 + data_addr_832X;
        addr_834X = -3 + env_829X;
        WRITE_BARRIER(addr_834X, 54);
        *((long *) addr_834X) = 54;
        *((long *) (-7 + env_829X)) = new_833X;
        addr_835X = -3 + env_827X;
        WRITE_BARRIER(addr_835X, new_833X);
        *((long *) addr_835X) = new_833X;
        arg0K0 = new_833X;
        goto L9319;}}}
  else {
    goto L9338;}}
 L9338: {
  arg0K0 = cont_852X;
  goto L9342;}
 L9342: {
  cont_825X = arg0K0;
  env_826X = *((long *) (9 + cont_825X));
  if ((3 == (3 & env_826X))) {
    if ((3 == (3 & (*((long *) (-7 + env_826X)))))) {
      *((long *) (9 + cont_825X)) = (*((long *) (-7 + env_826X)));
      arg0K0 = (*((long *) (-3 + cont_825X)));
      goto L9342;}
    else {
      save_env_in_heap0_return_value = new_823X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = new_823X;
    goto save_env_in_heap_return;}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  default: goto save_env_in_heap_return_2;
  }}

 copy_continuation_from_heapB: {
  cont_850X = merged_arg0K0;{
  top_836X = (-11 + (Sbottom_of_stackS)) - (-4 & (3 + ((long)(((unsigned long)(*((long *) (-7 + cont_850X))))>>8))));
  new_cont_837X = 7 + top_836X;
  RSstackS = (-4 + top_836X);
  ScontS = new_cont_837X;
  v_838X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + cont_850X))))>>8))))>>2);
  memcpy((void *)top_836X, (void *)(-7 + cont_850X),(4 + (((v_838X)<<2))));
  *((long *) (-3 + (Sbottom_of_stackS))) = (*((long *) (-3 + new_cont_837X)));
  *((long *) (-3 + new_cont_837X)) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_837X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  case 1: goto copy_continuation_from_heapB_return_1;
  default: goto copy_continuation_from_heapB_return_2;
  }}

 push_exception_continuationB: {
  byte_args_848X = merged_arg1K0;
  exception_849X = merged_arg1K1;{
  if (((Sstack_limitS) < (-296 + (RSstackS)))) {
    arg0K0 = 0;
    goto L8477;}
  else {
    Sstack_limitS = (Sstack_beginS);
    Sexception_space_usedPS = 1;
    if (((Sstack_limitS) < (-296 + (RSstackS)))) {
      arg0K0 = 0;
      goto L8477;}
    else {
      ps_error(((unsigned char *) "insufficient space on stack for exception frame"), 0);
      arg0K0 = 0;
      goto L8477;}}}
 L8477: {
  key_842X = arg0K0;
  arg0K0 = (4 + (RSstackS));
  arg0K1 = 0;
  goto L8510;}
 L8510: {
  p_840X = arg0K0;
  i_841X = arg0K1;
  if ((2 == (3 & (*((long *) p_840X))))) {
    if ((54 == (*((long *) p_840X)))) {
      arg0K0 = (-1 + i_841X);
      goto L8481;}
    else {
      arg0K0 = i_841X;
      goto L8481;}}
  else {
    arg0K0 = (4 + p_840X);
    arg0K1 = (1 + i_841X);
    goto L8510;}}
 L8481: {
  v_839X = arg0K0;
  merged_arg0K0 = (RScode_pointerS);
  merged_arg0K1 = v_839X;
  merged_arg0K2 = key_842X;
  push_continuationB_return_tag = 2;
  goto push_continuationB;
 push_continuationB_return_2:
  *((long *) (RSstackS)) = ((((((unsigned long) (*((unsigned char *) (-1 + ((RScode_pointerS) - (((unsigned long) byte_args_848X)))))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) exception_849X)))<<2));
  RSstackS = (-4 + (RSstackS));
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
  default: goto push_exception_continuationB_return_189;
  }}

 push_continuationB: {
  code_pointer_846X = merged_arg0K0;
  size_847X = merged_arg0K1;{
  pc_843X = 12 + ((((code_pointer_846X - (*((long *) (-3 + (StemplateS))))))<<2));
  template_844X = StemplateS;
  RSstackS = (-20 + (RSstackS));
  *((long *) (4 + (RSstackS))) = (128 | (4134 + (((size_847X)<<10))));
  cont_845X = 11 + (RSstackS);
  *((long *) (1 + cont_845X)) = pc_843X;
  *((long *) (5 + cont_845X)) = template_844X;
  *((long *) (9 + cont_845X)) = (RSenvS);
  *((long *) (-3 + cont_845X)) = (ScontS);
  ScontS = cont_845X;
  goto push_continuationB_return;}
 push_continuationB_return:
  switch (push_continuationB_return_tag) {
  case 0: goto push_continuationB_return_0;
  case 1: goto push_continuationB_return_1;
  default: goto push_continuationB_return_2;
  }}
}

long call_startup_procedure(long startup_proc_863X, unsigned char **startup_vector_864X, unsigned char startup_vector_length_865X)
{
  long arg0K0;
  unsigned char *merged_arg3K2;
  unsigned char merged_arg1K1;
  long merged_arg0K0;

  int make_initial_channel_return_tag;
  long make_initial_channel0_return_value;
  long len_866X;
  long addr_867X;
  long vector_868X;
  long i_869X;
  long code_870X;
  long addr_871X;
  long output_872X;
  long v_873X;
  long error_874X;
  long v_875X;
  long input_876X;
  long v_877X;
  long v_878X;
  long v_879X;
  long v_880X;
  long y_881X;
  long x_882X;
  long y_883X;
  long x_884X;
  long *v_885X;
  long length_886X;
  long x_887X;
  long y_888X;
  long tem_889X;
  long i_890X;
  long addr_891X;
  long i_892X;
  char x_893X;
  unsigned char *string_894X;
  long z_895X;
  long len_896X;
  long addr_897X;
  long string_898X;
  long z_899X;
  long len_900X;
  long addr_901X;
  long string_902X;
  long i_903X;
  long x_904X;
  long addr_905X;
  long channel_906X;
  unsigned char status_907X;
  unsigned char *name_908X;
 {len_866X = (((((unsigned long) startup_vector_length_865X)))<<2);
  addr_867X = ALLOCATE_SPACE(2, (4 + len_866X));
  *((long *) addr_867X) = (10 + (((len_866X)<<8)));
  vector_868X = 7 + addr_867X;
  arg0K0 = 0;
  goto L12170;}
 L12170: {
  i_892X = arg0K0;
  if ((i_892X < (((unsigned long) startup_vector_length_865X)))) {
    string_894X = *(startup_vector_864X + i_892X);
    z_895X = strlen((char *) string_894X);
    len_896X = 1 + z_895X;
    addr_897X = ALLOCATE_SPACE(16, (4 + len_896X));
    *((long *) addr_897X) = (66 + (((len_896X)<<8)));
    string_898X = 7 + addr_897X;
    *((unsigned char *) ((-3 + string_898X) + z_895X)) = 0;
    arg0K0 = 0;
    goto L12303;}
  else {
    SstackS = (-11 + (Sbottom_of_stackS));
    *((long *) (-3 + (Sbottom_of_stackS))) = 1;
    ScontS = (Sbottom_of_stackS);
    if ((Sexception_space_usedPS)) {
      x_893X = (Sstack_limitS) < (-296 + (SstackS));
      if (x_893X) {
        goto L7498;}
      else {
        ps_error(((unsigned char *) "no space on stack to reserve exception space"), 0);
        goto L7498;}}
    else {
      goto L7440;}}}
 L12303: {
  i_890X = arg0K0;
  if ((i_890X < z_895X)) {
    *((unsigned char *) ((-3 + string_898X) + i_890X)) = (((unsigned long) (*(string_894X + i_890X))));
    arg0K0 = (1 + i_890X);
    goto L12303;}
  else {
    addr_891X = (-3 + vector_868X) + (((i_892X)<<2));
    WRITE_BARRIER(addr_891X, string_898X);
    *((long *) addr_891X) = string_898X;
    arg0K0 = (1 + i_892X);
    goto L12170;}}
 L7498: {
  Sstack_limitS = (296 + (Sstack_beginS));
  Sexception_space_usedPS = 0;
  goto L7440;}
 L7440: {
  SenvS = 13;
  tem_889X = Sinterrupt_templateS;
  StemplateS = tem_889X;
  Scode_pointerS = (-3 + (*((long *) (-3 + tem_889X))));
  SnargsS = 13;
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
  *((long *) (SstackS)) = vector_868X;
  SstackS = (-4 + (SstackS));
  y_888X = fileno((stderr));
  x_887X = fileno((stdout));
  if ((x_887X < y_888X)) {
    arg0K0 = y_888X;
    goto L9190;}
  else {
    arg0K0 = x_887X;
    goto L9190;}}
 L9190: {
  y_883X = arg0K0;
  x_884X = fileno((stdin));
  if ((x_884X < y_883X)) {
    arg0K0 = y_883X;
    goto L9192;}
  else {
    arg0K0 = x_884X;
    goto L9192;}}
 L9192: {
  v_880X = arg0K0;
  x_882X = Snumber_of_channelsS;
  y_881X = 1 + v_880X;
  if ((x_882X < y_881X)) {
    arg0K0 = y_881X;
    goto L9194;}
  else {
    arg0K0 = x_882X;
    goto L9194;}}
 L9194: {
  v_878X = arg0K0;
  Snumber_of_channelsS = v_878X;
  v_879X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error(((unsigned char *) "out of memory, unable to continue"), 0);
    goto L9216;}
  else {
    goto L9216;}}
 L9216: {
  v_885X = Svm_channelsS;
  length_886X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L9258;}
 L9258: {
  i_869X = arg0K0;
  if ((i_869X < length_886X)) {
    *(v_885X + i_869X) = 1;
    arg0K0 = (1 + i_869X);
    goto L9258;}
  else {
    v_877X = fileno((stdin));
    merged_arg0K0 = v_877X;
    merged_arg1K1 = 4;
    merged_arg3K2 = ((unsigned char *) "standard input");
    make_initial_channel_return_tag = 0;
    goto make_initial_channel;
   make_initial_channel_return_0:
    input_876X = make_initial_channel0_return_value;
    v_875X = fileno((stderr));
    merged_arg0K0 = v_875X;
    merged_arg1K1 = 8;
    merged_arg3K2 = ((unsigned char *) "standard error");
    make_initial_channel_return_tag = 1;
    goto make_initial_channel;
   make_initial_channel_return_1:
    error_874X = make_initial_channel0_return_value;
    v_873X = fileno((stdout));
    merged_arg0K0 = v_873X;
    merged_arg1K1 = 8;
    merged_arg3K2 = ((unsigned char *) "standard output");
    make_initial_channel_return_tag = 2;
    goto make_initial_channel;
   make_initial_channel_return_2:
    output_872X = make_initial_channel0_return_value;
    *((long *) (SstackS)) = input_876X;
    SstackS = (-4 + (SstackS));
    *((long *) (SstackS)) = output_872X;
    SstackS = (-4 + (SstackS));
    *((long *) (SstackS)) = error_874X;
    SstackS = (-4 + (SstackS));
    addr_871X = ALLOCATE_SPACE(17, 6);
    *((long *) addr_871X) = 582;
    code_870X = 7 + addr_871X;
    *((unsigned char *) (-3 + code_870X)) = 25;
    *((unsigned char *) (-2 + code_870X)) = 4;
    Scode_pointerS = (-3 + code_870X);
    return restart(startup_proc_863X);}}
 make_initial_channel: {
  channel_906X = merged_arg0K0;
  status_907X = merged_arg1K1;
  name_908X = merged_arg3K2;{
  z_899X = strlen((char *) name_908X);
  len_900X = 1 + z_899X;
  addr_901X = ALLOCATE_SPACE(16, (4 + len_900X));
  *((long *) addr_901X) = (66 + (((len_900X)<<8)));
  string_902X = 7 + addr_901X;
  *((unsigned char *) ((-3 + string_902X) + z_899X)) = 0;
  arg0K0 = 0;
  goto L8799;}
 L8799: {
  i_903X = arg0K0;
  if ((i_903X < z_899X)) {
    *((unsigned char *) ((-3 + string_902X) + i_903X)) = (((unsigned long) (*(name_908X + i_903X))));
    arg0K0 = (1 + i_903X);
    goto L8799;}
  else {
    addr_905X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_905X) = 5142;
    x_904X = 7 + addr_905X;
    *((long *) (-3 + x_904X)) = (((unsigned long) status_907X));
    *((long *) (1 + x_904X)) = string_902X;
    *((long *) (5 + x_904X)) = (((channel_906X)<<2));
    *((long *) (9 + x_904X)) = 1;
    *((long *) (13 + x_904X)) = 1;
    *((Svm_channelsS) + channel_906X) = x_904X;
    make_initial_channel0_return_value = x_904X;
    goto make_initial_channel_return;}}
 make_initial_channel_return:
  switch (make_initial_channel_return_tag) {
  case 0: goto make_initial_channel_return_0;
  case 1: goto make_initial_channel_return_1;
  default: goto make_initial_channel_return_2;
  }}
}
void
scheme48_init(void)
{
Spending_eventsPS = 0;
Sexception_space_usedPS = 1;
Spending_channels_tailS = 1;
Spending_channels_headS = 1;
Snumber_of_channelsS = 100;
}
