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
unsigned char Spending_eventsPS;
long Sextension_valueS;
long Sinterrupted_templateS;
long Sinterrupt_templateS;
unsigned char Spending_interruptPS;
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
unsigned char Sexception_space_usedPS;
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
  long addr_0X;
  long addr_1X;
  long i_2X;
  long descriptor_3X;
  long channel_4X;
  long header_5X;
 {arg0K0 = 0;
  goto L5775;}
 L5775: {
  i_2X = arg0K0;
  if ((i_2X == (Snumber_of_channelsS))) {
    return;}
  else {
    channel_4X = *((Svm_channelsS) + i_2X);
    if ((1 == channel_4X)) {
      goto L5835;}
    else {
      header_5X = *((long *) (-7 + channel_4X));
      if ((3 == (3 & header_5X))) {
        ps_write_string(((unsigned char *) "Channel closed in dumped image: "), (stderr));
        descriptor_3X = *((long *) (1 + channel_4X));
        if ((0 == (3 & descriptor_3X))) {
          ps_write_integer(((((*((long *) (1 + channel_4X))))>>2)), (stderr));
          goto L5821;}
        else {
          ps_write_string((((unsigned char *)(-3 + (*((long *) (1 + channel_4X)))))), (stderr));
          goto L5821;}}
      else {
        goto L5835;}}}}
 L5835: {
  arg0K0 = (1 + i_2X);
  goto L5775;}
 L5821: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  addr_0X = -3 + header_5X;
  WRITE_BARRIER(addr_0X, 0);
  *((long *) addr_0X) = 0;
  addr_1X = 5 + header_5X;
  WRITE_BARRIER(addr_1X, -4);
  *((long *) addr_1X) = -4;
  goto L5835;}}

void set_extension_valueB(long value_6X)
{

 {Sextension_valueS = value_6X;
  return;}}

void note_event(void)
{

 {Spending_eventsPS = 1;
  Spending_interruptPS = 1;
  return;}}

long required_init_space(unsigned char **startup_vector_7X, long startup_vector_length_8X)
{
  long arg0K1;
  long arg0K0;
  long i_9X;
  long s_10X;
 {arg0K0 = 0;
  arg0K1 = 0;
  goto L3888;}
 L3888: {
  i_9X = arg0K0;
  s_10X = arg0K1;
  if ((i_9X < startup_vector_length_8X)) {
    arg0K0 = (1 + i_9X);
    arg0K1 = (1 + (s_10X + ((((4 + (strlen((*(startup_vector_7X + i_9X))))))>>2))));
    goto L3888;}
  else {
    return (45 + s_10X);}}}

void initialize_vm(long stack_begin_11X, long stack_size_12X)
{
  long addr_13X;
  long addr_14X;
  long temp_15X;
  long code_16X;
  long addr_17X;
  long cont_18X;
  long addr_19X;
  long addr_20X;
  long temp_21X;
  long code_22X;
  long addr_23X;
 {Sstack_beginS = stack_begin_11X;
  Sstack_endS = (stack_begin_11X + (((stack_size_12X)<<2)));
  Sstack_limitS = (Sstack_beginS);
  SstackS = (-4 + (Sstack_endS));
  ScontS = 1;
  SenvS = 17;
  addr_13X = ALLOCATE_SPACE(17, 6);
  *((long *) addr_13X) = 582;
  addr_14X = ALLOCATE_SPACE(11, 12);
  *((long *) addr_14X) = 2094;
  temp_15X = 7 + addr_14X;
  code_16X = 7 + addr_13X;
  addr_17X = -3 + temp_15X;
  WRITE_BARRIER(addr_17X, code_16X);
  *((long *) addr_17X) = code_16X;
  *((unsigned char *) (-3 + code_16X)) = 23;
  *((unsigned char *) (-2 + code_16X)) = 29;
  SstackS = (-20 + (SstackS));
  *((long *) (4 + (SstackS))) = 4262;
  cont_18X = 11 + (SstackS);
  *((long *) (1 + cont_18X)) = 0;
  *((long *) (5 + cont_18X)) = temp_15X;
  *((long *) (9 + cont_18X)) = (SenvS);
  *((long *) (-3 + cont_18X)) = (ScontS);
  ScontS = cont_18X;
  Sbottom_of_stackS = (ScontS);
  addr_19X = ALLOCATE_SPACE(17, 6);
  *((long *) addr_19X) = 582;
  addr_20X = ALLOCATE_SPACE(11, 12);
  *((long *) addr_20X) = 2094;
  temp_21X = 7 + addr_20X;
  code_22X = 7 + addr_19X;
  addr_23X = -3 + temp_21X;
  WRITE_BARRIER(addr_23X, code_22X);
  *((long *) addr_23X) = code_22X;
  *((unsigned char *) (-3 + code_22X)) = 32;
  *((unsigned char *) (-2 + code_22X)) = 129;
  Sinterrupt_templateS = temp_21X;
  return;}}

long restart(long value_24X)
{
  unsigned char arg2K3;
  unsigned char arg2K1;
  unsigned char arg2K2;
  unsigned char arg2K0;
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
  unsigned char check_events0_return_value;
  int loseD0_return_tag;
  unsigned char option_25X;
  long v_26X;
  long (*v_27X)(long, long);
  long b_28X;
  long c_29X;
  long a_30X;
  long n_31X;
  long b_32X;
  long c_33X;
  long a_34X;
  long n_35X;
  long len_36X;
  long addr_37X;
  long string_38X;
  long i_39X;
  unsigned char v_40X;
  long length_41X;
  long status_42X;
  unsigned char pendingP_43X;
  unsigned char eofP_44X;
  long got_45X;
  unsigned char reason_46X;
  long x_47X;
  long obj_48X;
  unsigned char waitP_49X;
  long count_50X;
  long start_51X;
  long length_52X;
  long got_53X;
  unsigned char pendingP_54X;
  long status_55X;
  unsigned char reason_56X;
  long env_57X;
  long i_58X;
  unsigned char b_59X;
  long i_60X;
  long h_61X;
  long bucket_62X;
  long index_63X;
  long b_64X;
  long addr_65X;
  long x_66X;
  long addr_67X;
  long i_68X;
  long addr_69X;
  long addr_70X;
  long table_71X;
  long symbols_72X;
  long addr_73X;
  long i_74X;
  long n_75X;
  long s_76X;
  long v_77X;
  long obj_78X;
  unsigned char v_79X;
  long table_80X;
  long symbols_81X;
  unsigned char firstP_82X;
  long table_83X;
  unsigned char firstP_84X;
  long symbols_85X;
  long v_86X;
  unsigned char v_87X;
  unsigned char v_88X;
  unsigned char v_89X;
  long r_90X;
  unsigned char v_91X;
  unsigned char v_92X;
  long new_93X;
  long addr_94X;
  long len_95X;
  unsigned char b_96X;
  unsigned char b_97X;
  long i_98X;
  unsigned char b_99X;
  long len_100X;
  long addr_101X;
  long new_102X;
  long i_103X;
  long i_104X;
  long addr_105X;
  unsigned char okayP_106X;
  long init_107X;
  long len_108X;
  long addr_109X;
  long val_110X;
  long addr_111X;
  long vector_112X;
  long i_113X;
  unsigned char v_114X;
  long addr_115X;
  long addr_116X;
  long channel_117X;
  unsigned char reason_118X;
  long v_119X;
  long new_count_120X;
  long *new_vm_channels_121X;
  long i_122X;
  long i_123X;
  long addr_124X;
  long x_125X;
  long v_126X;
  long v_127X;
  long v_128X;
  long v_129X;
  long v_130X;
  long status_131X;
  long arg2_132X;
  long mode_133X;
  long channel_134X;
  long status_135X;
  long index_136X;
  unsigned char temp_137X;
  long x_138X;
  long y_139X;
  long v_140X;
  long status_141X;
  long addr_142X;
  unsigned char reason_143X;
  long x_144X;
  long arg2_145X;
  long addr_146X;
  long x_147X;
  long b_148X;
  long addr_149X;
  long x_150X;
  unsigned char firstP_151X;
  long vector_152X;
  long n_153X;
  unsigned char minutesP_154X;
  unsigned char temp_155X;
  unsigned char v_156X;
  long i_157X;
  long h_158X;
  long len_159X;
  long addr_160X;
  long string_161X;
  long arg2_162X;
  long n_163X;
  long l_164X;
  long i_165X;
  long val_166X;
  long new_167X;
  long addr_168X;
  long x_169X;
  long addr_170X;
  long i_171X;
  long h_172X;
  long bucket_173X;
  long index_174X;
  long b_175X;
  long len_176X;
  long s2_177X;
  long addr_178X;
  long x_179X;
  long arg2_180X;
  long obj_181X;
  long table_182X;
  long v_183X;
  long n_184X;
  long list_185X;
  long slow_186X;
  unsigned char move_slowP_187X;
  long head_188X;
  long list_189X;
  long y_190X;
  long y_191X;
  long list_192X;
  long a_193X;
  long addr_194X;
  long x_195X;
  long l_196X;
  long last_197X;
  long addr_198X;
  long x_199X;
  long addr_200X;
  long a_201X;
  long i_202X;
  long l_203X;
  long space_204X;
  unsigned char v_205X;
  unsigned char okayP_206X;
  long overflow_207X;
  long fast_208X;
  long len_209X;
  long slow_210X;
  unsigned char move_slowP_211X;
  long nargs_212X;
  long args_213X;
  long args_214X;
  long offset_215X;
  unsigned char v_216X;
  unsigned char b_217X;
  long args_218X;
  long env_219X;
  long i_220X;
  unsigned char b_221X;
  long value_222X;
  long addr_223X;
  long v_224X;
  unsigned char b_225X;
  long a_226X;
  long addr_227X;
  long x_228X;
  long env_229X;
  unsigned char v_230X;
  unsigned char okayP_231X;
  unsigned char key_232X;
  long p_233X;
  long v_234X;
  long space_235X;
  unsigned char v_236X;
  unsigned char okayP_237X;
  long env_238X;
  long i_239X;
  unsigned char b_240X;
  long value_241X;
  long addr_242X;
  long v_243X;
  long len_244X;
  long addr_245X;
  long new_env_246X;
  long i_247X;
  long env_248X;
  unsigned char b_249X;
  unsigned char b_250X;
  long a_251X;
  long addr_252X;
  long x_253X;
  unsigned char v_254X;
  long key_255X;
  long tem_256X;
  long n_257X;
  long b_258X;
  long c_259X;
  long a_260X;
  long n_261X;
  long b_262X;
  long c_263X;
  long a_264X;
  long q_265X;
  long a_266X;
  long val_267X;
  long val_268X;
  long addr_269X;
  long addr_270X;
  long ch_271X;
  long prev_272X;
  long y_273X;
  long n_274X;
  long b_275X;
  long lo_a_276X;
  long lo_b_277X;
  long hi_a_278X;
  long hi_b_279X;
  long lo_c_280X;
  long v_281X;
  long v_282X;
  long mid_c_283X;
  long c_284X;
  long a_285X;
  unsigned char firstP_286X;
  long type_287X;
  long vector_288X;
  long v_289X;
  unsigned char v_290X;
  unsigned char v_291X;
  unsigned char v_292X;
  unsigned char v_293X;
  unsigned char v_294X;
  long loc_295X;
  long arg_296X;
  long v_297X;
  unsigned char key_298X;
  long v_299X;
  unsigned char b_300X;
  long len_301X;
  long addr_302X;
  long stob_303X;
  long x_304X;
  long value_305X;
  long addr_306X;
  unsigned char b_307X;
  unsigned char b_308X;
  unsigned char okayP_309X;
  unsigned char v_310X;
  long space_311X;
  long v_312X;
  unsigned char b_313X;
  long env_314X;
  unsigned char b_315X;
  long location_316X;
  unsigned char b_317X;
  long addr_318X;
  long val_319X;
  long location_320X;
  unsigned char b_321X;
  long p_322X;
  unsigned char b_323X;
  unsigned char okayP_324X;
  unsigned char v_325X;
  long space_326X;
  long list_327X;
  long proc_328X;
  long consumer_329X;
  unsigned char b_330X;
  unsigned char b_331X;
  unsigned char b_332X;
  unsigned char b_333X;
  long x_334X;
  long arg2_335X;
  long x_336X;
  long x_337X;
  long x_338X;
  long x_339X;
  long x_340X;
  long arg2_341X;
  long b_342X;
  long a_343X;
  long y_344X;
  long z_345X;
  long x_346X;
  long b_347X;
  long a_348X;
  long y_349X;
  long arg2_350X;
  long x_351X;
  long b_352X;
  long a_353X;
  long x_354X;
  long a_355X;
  long x_356X;
  long x_357X;
  long x_358X;
  long x_359X;
  long x_360X;
  long x_361X;
  long x_362X;
  long x_363X;
  long x_364X;
  long x_365X;
  long x_366X;
  long x_367X;
  long x_368X;
  long x_369X;
  long arg2_370X;
  long x_371X;
  long x_372X;
  long arg2_373X;
  long x_374X;
  long arg2_375X;
  long x_376X;
  unsigned char b_377X;
  long x_378X;
  unsigned char okayP_379X;
  unsigned char v_380X;
  long space_381X;
  long len_382X;
  long addr_383X;
  unsigned char b_384X;
  unsigned char b_385X;
  long x_386X;
  long arg2_387X;
  unsigned char v_388X;
  long temp_389X;
  unsigned char v_390X;
  long size_391X;
  long len_392X;
  long index_393X;
  long len_394X;
  long index_395X;
  long len_396X;
  unsigned char okayP_397X;
  unsigned char v_398X;
  long size_399X;
  long init_400X;
  long len_401X;
  long obj_402X;
  long arg2_403X;
  long arg3_404X;
  long arg2_405X;
  long obj_406X;
  long arg2_407X;
  long descriptor_408X;
  long x_409X;
  long obj_410X;
  long x_411X;
  unsigned char okayP_412X;
  unsigned char v_413X;
  long v_414X;
  long v_415X;
  long os_index_416X;
  long x_417X;
  long obj_418X;
  long arg5_419X;
  long arg4_420X;
  long arg3_421X;
  long arg2_422X;
  unsigned char v_423X;
  long channel_424X;
  long count_425X;
  long start_426X;
  long obj_427X;
  long n_428X;
  long addr_429X;
  long next_430X;
  long channel_431X;
  long x_432X;
  long status_433X;
  long status_434X;
  long status_435X;
  long status_436X;
  long status_437X;
  unsigned char x_438X;
  long x_439X;
  long obj_440X;
  long arg3_441X;
  long arg2_442X;
  unsigned char okayP_443X;
  unsigned char v_444X;
  long n_445X;
  long arg2_446X;
  long x_447X;
  long temp_448X;
  long obj_449X;
  long temp_450X;
  long obj_451X;
  long p_452X;
  long arg2_453X;
  unsigned char v_454X;
  long name_455X;
  long value_456X;
  long x_457X;
  long obj_458X;
  long n_459X;
  long seconds_460X;
  long mseconds_461X;
  long seconds_462X;
  long mseconds_463X;
  long arg2_464X;
  long arg2_465X;
  long arg2_466X;
  long n_467X;
  long x_468X;
  long obj_469X;
  unsigned char okayP_470X;
  unsigned char v_471X;
  long len_472X;
  long arg3_473X;
  long arg2_474X;
  long addr_475X;
  long index_476X;
  long x_477X;
  long count_478X;
  long to_index_479X;
  long from_index_480X;
  long arg2_481X;
  unsigned char b_482X;
  long x_483X;
  long x_484X;
  unsigned char Kchar_485X;
  long p_486X;
  long b_487X;
  long i_488X;
  long x_489X;
  long val_490X;
  long addr_491X;
  long x_492X;
  long port_493X;
  long p_494X;
  long p_495X;
  long b_496X;
  long i_497X;
  long x_498X;
  long x_499X;
  long port_500X;
  long p_501X;
  long p_502X;
  long b_503X;
  long i_504X;
  long x_505X;
  long val_506X;
  long addr_507X;
  long arg2_508X;
  long arg3_509X;
  long arg4_510X;
  long arg5_511X;
  long arg2_512X;
  long arg3_513X;
  long arg4_514X;
  long len_515X;
  long index_516X;
  long arg2_517X;
  long x_518X;
  long space_519X;
  unsigned char v_520X;
  unsigned char okayP_521X;
  long arg2_522X;
  long obj_523X;
  long x_524X;
  long len_525X;
  long x_526X;
  long key_527X;
  long status_528X;
  long x_529X;
  long option_530X;
  long proc_531X;
  long x_532X;
  long p_533X;
  long old_534X;
  long p_535X;
  long obj_536X;
  long x_537X;
  long type_538X;
  long x_539X;
  long key_540X;
  long n_541X;
  long bytes_542X;
  long status_543X;
  FILE * port_544X;
  long obj_545X;
  long channel_546X;
  long head_547X;
  long arg2_548X;
  long arg3_549X;
  long arg4_550X;
  long x_551X;
  long arg2_552X;
  long x_553X;
  long arg2_554X;
  long arg3_555X;
  long index_556X;
  unsigned char Kchar_557X;
  long len_558X;
  long index_559X;
  long len_560X;
  long arg2_561X;
  long len_562X;
  unsigned char init_563X;
  long size_564X;
  unsigned char v_565X;
  unsigned char okayP_566X;
  long index_567X;
  long Kchar_568X;
  long len_569X;
  long index_570X;
  long len_571X;
  long arg2_572X;
  long arg2_573X;
  long arg3_574X;
  long x_575X;
  unsigned char b_576X;
  long addr_577X;
  long arg2_578X;
  long x_579X;
  unsigned char b_580X;
  long arg2_581X;
  long x_582X;
  unsigned char b_583X;
  long x_584X;
  unsigned char b_585X;
  unsigned char b_586X;
  long space_587X;
  unsigned char v_588X;
  unsigned char okayP_589X;
  long x_590X;
  unsigned char b_591X;
  long x_592X;
  long x_593X;
  long value_594X;
  long count_595X;
  long x_596X;
  long result_597X;
  long x_598X;
  long arg2_599X;
  long arg2_600X;
  long arg2_601X;
  long arg2_602X;
  long arg2_603X;
  long arg2_604X;
  long x_605X;
  long arg2_606X;
  long arg2_607X;
  long arg2_608X;
  long x_609X;
  long arg2_610X;
  long arg2_611X;
  long arg2_612X;
  long x_613X;
  long z_614X;
  long x_615X;
  long x_616X;
  long p_617X;
  unsigned char b_618X;
  long val_619X;
  unsigned char b_620X;
  unsigned char b_621X;
  unsigned char b_622X;
  unsigned char b_623X;
  long tem_624X;
  unsigned char v_625X;
  unsigned char b_626X;
  long tem_627X;
  long cont_628X;
  long tem_629X;
  long pc_630X;
  long cont_631X;
  long nargs_632X;
  long top_of_args_633X;
  long stob_634X;
  long cont_635X;
  unsigned char b_636X;
  unsigned char b_637X;
  unsigned char b_638X;
  long space_639X;
  unsigned char v_640X;
  unsigned char okayP_641X;
  unsigned char b_642X;
  unsigned char b_643X;
  long space_644X;
  unsigned char v_645X;
  unsigned char okayP_646X;
  long env_647X;
  unsigned char b_648X;
  long env_649X;
  unsigned char b_650X;
  long env_651X;
  unsigned char b_652X;
  unsigned char b_653X;
  long env_654X;
  unsigned char b_655X;
  long v_656X;
  unsigned char count_657X;
  long space_658X;
  unsigned char v_659X;
  unsigned char okayP_660X;
  long args_661X;
  long v_662X;
  long v_663X;
  long args_664X;
  long cont_665X;
  long v_666X;
  unsigned char next_op_667X;
  long v_668X;
  long next_669X;
  long cont_670X;
  long tem_671X;
  long pc_672X;
  long nargs_673X;
  long cont_674X;
  long tem_675X;
  long pc_676X;
  unsigned char v_677X;
  unsigned char x_678X;
  unsigned char okayP_679X;
  unsigned char v_680X;
  long space_681X;
  long obj_682X;
  unsigned char nargs_683X;
  unsigned char args_684X;
  long obj_685X;
  unsigned char bytes_consumed_686X;
  unsigned char v_687X;
  long tem_688X;
  long obj_689X;
  long args_690X;
  long a1_691X;
  unsigned char byte_args_692X;
  unsigned char exception_693X;
  long v_694X;
  long v_695X;
  unsigned char v_696X;
  long obj_697X;
  long channel_698X;
  unsigned char x_699X;
  long obj_700X;
  unsigned char v_701X;
  long key_702X;
  long tem_703X;
  long n_704X;
  long args_on_stack_705X;
  unsigned char okayP_706X;
  unsigned char v_707X;
  long space_708X;
  long p_709X;
  long i_710X;
  long i_711X;
  long m_712X;
  long channel_713X;
  long next_714X;
  long addr_715X;
  long opcode_716X;
  long why_717X;
  long current_template_718X;
  FILE * out_719X;
  unsigned char v_720X;
  unsigned char not_firstP_721X;
  long cont_722X;
  unsigned char not_firstP_723X;
  long template_724X;
  long addr_725X;
  long val_726X;
  long channel_727X;
  long status_728X;
  long channel_729X;
  long type_730X;
  long addr_731X;
  long v_732X;
  long space_733X;
  unsigned char v_734X;
  unsigned char okayP_735X;
  long args_736X;
  long count_737X;
  long a_738X;
  long addr_739X;
  long x_740X;
  long pc_741X;
  long v_742X;
  long v_743X;
  long v_744X;
  long v_745X;
  long v_746X;
  long v_747X;
  long v_748X;
  long v_749X;
  long v_750X;
  long env_751X;
  long v_752X;
  long v_753X;
  long v_754X;
  long status_755X;
  long addr_756X;
  long id_757X;
  long os_index_758X;
  long v_759X;
  long v_760X;
  long new_761X;
  long i_762X;
  long channel_763X;
  long header_764X;
  long id_765X;
  long tem_766X;
  long v_767X;
  long env_768X;
  long env_769X;
  long arg_count_770X;
  long p_771X;
  long env_772X;
  long p_773X;
  long i_774X;
  long l2_775X;
  long okay_776X;
  long sickly_777X;
  long value_778X;
  long cont_779X;
  long last_env_780X;
  long env_781X;
  long v_782X;
  long l_783X;
  long addr_784X;
  long v_785X;
  long loc_786X;
  long arg_787X;
  long stob_788X;
  long arg_count_789X;
  long top_790X;
  long p_791X;
  long i_792X;
  long p_793X;
  long v_794X;
  long end_795X;
  long header_796X;
  long addr_797X;
  long data_addr_798X;
  long new_799X;
  long cont_800X;
  long previous_801X;
  long cont_802X;
  long p_803X;
  long header_804X;
  long addr_805X;
  long data_addr_806X;
  long new_807X;
  long addr_808X;
  long cont_809X;
  long env_810X;
  long env_811X;
  long p_812X;
  long env_813X;
  long header_814X;
  long addr_815X;
  long data_addr_816X;
  long new_817X;
  long addr_818X;
  long addr_819X;
  long top_820X;
  long new_cont_821X;
  long v_822X;
  long v_823X;
  long p_824X;
  long i_825X;
  long key_826X;
  long pc_827X;
  long template_828X;
  long cont_829X;
  long code_pointer_830X;
  long size_831X;
  unsigned char byte_args_832X;
  unsigned char exception_833X;
  long cont_834X;
  long env_835X;
  long cont_836X;
  unsigned char key_837X;
  unsigned char reason_838X;
  unsigned char key_839X;
  long status_840X;
  long index_841X;
  long id_842X;
  long value_843X;
  long start_844X;
  long count_845X;
  unsigned char *message_846X;
 {RSvalS = value_24X;
  goto L12059;}
 L12059: {
  goto L11156;}
 L11156: {
  b_482X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  switch ((((unsigned long) b_482X))) {
    case 0 : {
      b_307X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((SnargsS) == (((unsigned long) b_307X)))) {
        goto L11156;}
      else {
        arg1K0 = 1;
        arg1K1 = 4;
        goto L26390;}}
      break;
    case 1 : {
      b_308X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((SnargsS) < (((unsigned long) b_308X)))) {
        arg1K0 = 1;
        arg1K1 = 4;
        goto L26390;}
      else {
        goto L11156;}}
      break;
    case 2 : {
      RSvalS = ((((SnargsS))<<2));
      goto L11156;}
      break;
    case 3 : {
      count_657X = *((unsigned char *) (RScode_pointerS));
      if (((Sstack_limitS) < (-8 + ((RSstackS) - ((((((unsigned long) count_657X)))<<2)))))) {
        goto L11982;}
      else {
        space_658X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
        v_659X = AVAILABLEp(space_658X);
        if (v_659X) {
          arg1K0 = 0;
          goto L12000;}
        else {
          merged_arg0K0 = 0;
          collect_saving_temp_return_tag = 0;
          goto collect_saving_temp;
         collect_saving_temp_return_0:
          okayP_660X = AVAILABLEp(space_658X);
          if (okayP_660X) {
            arg1K0 = 0;
            goto L12000;}
          else {
            ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
            arg1K0 = 0;
            goto L12000;}}}}
      break;
    case 4 : {
      space_311X = 2 + (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
      v_310X = AVAILABLEp(space_311X);
      if (v_310X) {
        goto L29663;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 1;
        goto collect_saving_temp;
       collect_saving_temp_return_1:
        okayP_309X = AVAILABLEp(space_311X);
        if (okayP_309X) {
          goto L29663;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L29663;}}}
      break;
    case 5 : {
      RSenvS = (*((long *) (-3 + (RSenvS))));
      goto L11156;}
      break;
    case 6 : {
      b_655X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((64 == (SnargsS))) {
        RSstackS = (4 + (RSstackS));
        merged_arg0K0 = (*((long *) (RSstackS)));
        merged_arg0K1 = ((-1 + (SnargsS)) - (((unsigned long) b_655X)));
        pop_args_list_return_tag = 0;
        goto pop_args_list;
       pop_args_list_return_0:
        v_312X = pop_args_list0_return_value;
        arg0K0 = v_312X;
        goto L11899;}
      else {
        merged_arg0K0 = 25;
        merged_arg0K1 = ((SnargsS) - (((unsigned long) b_655X)));
        pop_args_list_return_tag = 1;
        goto pop_args_list;
       pop_args_list_return_1:
        v_656X = pop_args_list0_return_value;
        arg0K0 = v_656X;
        goto L11899;}}
      break;
    case 7 : {
      b_313X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + (StemplateS)) + ((((((unsigned long) b_313X)))<<2)))));
      goto L11156;}
      break;
    case 8 : {
      b_653X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      env_654X = RSenvS;
      arg0K0 = env_654X;
      arg0K1 = (((unsigned long) b_653X));
      goto L26620;}
      break;
    case 9 : {
      env_651X = RSenvS;
      b_652X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_651X) + ((((((unsigned long) b_652X)))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg1K0 = 1;
        merged_arg1K1 = 0;
        push_exception_continuationB_return_tag = 0;
        goto push_exception_continuationB;
       push_exception_continuationB_return_0:
        arg1K0 = 0;
        arg1K1 = 1;
        goto L12935;}
      else {
        goto L11156;}}
      break;
    case 10 : {
      env_649X = *((long *) (-3 + (RSenvS)));
      b_650X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_649X) + ((((((unsigned long) b_650X)))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg1K0 = 1;
        merged_arg1K1 = 0;
        push_exception_continuationB_return_tag = 1;
        goto push_exception_continuationB;
       push_exception_continuationB_return_1:
        arg1K0 = 0;
        arg1K1 = 1;
        goto L12935;}
      else {
        goto L11156;}}
      break;
    case 11 : {
      env_647X = *((long *) (-3 + (*((long *) (-3 + (RSenvS))))));
      b_648X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_647X) + ((((((unsigned long) b_648X)))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg1K0 = 1;
        merged_arg1K1 = 0;
        push_exception_continuationB_return_tag = 2;
        goto push_exception_continuationB;
       push_exception_continuationB_return_2:
        arg1K0 = 0;
        arg1K1 = 1;
        goto L12935;}
      else {
        goto L11156;}}
      break;
    case 12 : {
      b_315X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      env_314X = RSenvS;
      arg0K0 = env_314X;
      arg0K1 = (((unsigned long) b_315X));
      goto L11848;}
      break;
    case 13 : {
      b_317X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      location_316X = *((long *) ((-3 + (StemplateS)) + ((((((unsigned long) b_317X)))<<2))));
      RSvalS = (*((long *) (1 + location_316X)));
      if ((17 == (255 & (RSvalS)))) {
        merged_arg1K0 = 1;
        merged_arg1K1 = 1;
        push_exception_continuationB_return_tag = 3;
        goto push_exception_continuationB;
       push_exception_continuationB_return_3:
        *((long *) (RSstackS)) = location_316X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 1;
        goto L12935;}
      else {
        goto L11156;}}
      break;
    case 14 : {
      b_321X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      location_320X = *((long *) ((-3 + (StemplateS)) + ((((((unsigned long) b_321X)))<<2))));
      if ((273 == (*((long *) (1 + location_320X))))) {
        merged_arg1K0 = 1;
        merged_arg1K1 = 1;
        push_exception_continuationB_return_tag = 4;
        goto push_exception_continuationB;
       push_exception_continuationB_return_4:
        *((long *) (RSstackS)) = location_320X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 1;
        goto L12935;}
      else {
        val_319X = RSvalS;
        addr_318X = 1 + location_320X;
        WRITE_BARRIER(addr_318X, val_319X);
        *((long *) addr_318X) = val_319X;
        RSvalS = 13;
        goto L11156;}}
      break;
    case 15 : {
      p_322X = RSenvS;
      if ((3 == (3 & p_322X))) {
        if ((p_322X < (Sstack_beginS))) {
          arg0K0 = 0;
          goto L11708;}
        else {
          if (((Sstack_endS) < p_322X)) {
            arg0K0 = 0;
            goto L11708;}
          else {
            arg0K0 = ((Sstack_endS) - (Sstack_beginS));
            goto L11708;}}}
      else {
        arg0K0 = 0;
        goto L11708;}}
      break;
    case 16 : {
      b_643X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      space_644X = 4 + (((unsigned long) b_643X));
      v_645X = AVAILABLEp(space_644X);
      if (v_645X) {
        goto L11551;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 2;
        goto collect_saving_temp;
       collect_saving_temp_return_2:
        okayP_646X = AVAILABLEp(space_644X);
        if (okayP_646X) {
          goto L11551;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L11551;}}}
      break;
    case 17 : {
      *((long *) (RSstackS)) = (RSvalS);
      RSstackS = (-4 + (RSstackS));
      goto L11156;}
      break;
    case 18 : {
      RSstackS = (4 + (RSstackS));
      RSvalS = (*((long *) (RSstackS)));
      goto L11156;}
      break;
    case 19 : {
      b_323X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) (4 + ((RSstackS) + ((((((unsigned long) b_323X)))<<2))))));
      goto L11156;}
      break;
    case 20 : {
      b_642X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      *((long *) (4 + ((RSstackS) + ((((((unsigned long) b_642X)))<<2))))) = (RSvalS);
      goto L11156;}
      break;
    case 21 : {
      b_636X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_637X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_638X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((Sstack_limitS) < (-276 + (RSstackS)))) {
        arg0K0 = 0;
        goto L11434;}
      else {
        space_639X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
        v_640X = AVAILABLEp(space_639X);
        if (v_640X) {
          arg1K0 = 0;
          goto L11460;}
        else {
          merged_arg0K0 = 0;
          collect_saving_temp_return_tag = 3;
          goto collect_saving_temp;
         collect_saving_temp_return_3:
          okayP_641X = AVAILABLEp(space_639X);
          if (okayP_641X) {
            arg1K0 = 0;
            goto L11460;}
          else {
            ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
            arg1K0 = 0;
            goto L11460;}}}}
      break;
    case 22 : {
      space_326X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
      v_325X = AVAILABLEp(space_326X);
      if (v_325X) {
        arg1K0 = 0;
        goto L29771;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 4;
        goto collect_saving_temp;
       collect_saving_temp_return_4:
        okayP_324X = AVAILABLEp(space_326X);
        if (okayP_324X) {
          arg1K0 = 0;
          goto L29771;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          arg1K0 = 0;
          goto L29771;}}}
      break;
    case 23 : {
      cont_635X = *((long *) (-3 + (Sbottom_of_stackS)));
      if ((3 == (3 & cont_635X))) {
        if ((9 == (31 & ((((*((long *) (-7 + cont_635X))))>>2))))) {
          if ((3 == (3 & cont_635X))) {
            if ((9 == (31 & ((((*((long *) (-7 + cont_635X))))>>2))))) {
              merged_arg0K0 = cont_635X;
              copy_continuation_from_heapB_return_tag = 0;
              goto copy_continuation_from_heapB;
             copy_continuation_from_heapB_return_0:
              goto L11156;}
            else {
              goto L13676;}}
          else {
            goto L13676;}}
        else {
          goto L13640;}}
      else {
        goto L13640;}}
      break;
    case 24 : {
      SnargsS = (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
      arg1K0 = 0;
      goto L12589;}
      break;
    case 25 : {
      SnargsS = (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
      nargs_632X = SnargsS;
      top_of_args_633X = RSstackS;
      stob_634X = ScontS;
      arg0K0 = (-11 + stob_634X);
      arg0K1 = (top_of_args_633X + (((nargs_632X)<<2)));
      goto L29812;}
      break;
    case 26 : {
      RSstackS = (4 + (RSstackS));
      proc_328X = *((long *) (RSstackS));
      list_327X = RSvalS;
      arg0K0 = list_327X;
      arg0K1 = 0;
      arg0K2 = list_327X;
      arg2K3 = 0;
      goto L8316;}
      break;
    case 27 : {
      RSstackS = (4 + (RSstackS));
      cont_631X = *((long *) (RSstackS));
      if ((3 == (3 & cont_631X))) {
        if ((9 == (31 & ((((*((long *) (-7 + cont_631X))))>>2))))) {
          merged_arg0K0 = cont_631X;
          copy_continuation_from_heapB_return_tag = 1;
          goto copy_continuation_from_heapB;
         copy_continuation_from_heapB_return_1:
          goto L12709;}
        else {
          goto L12719;}}
      else {
        goto L12719;}}
      break;
    case 28 : {
      RSstackS = (4 + (RSstackS));
      consumer_329X = *((long *) (RSstackS));
      *((long *) (RSstackS)) = (RSvalS);
      RSstackS = (-4 + (RSstackS));
      RSvalS = consumer_329X;
      SnargsS = 1;
      arg1K0 = 0;
      goto L12589;}
      break;
    case 29 : {
      cont_628X = ScontS;
      tem_629X = *((long *) (5 + cont_628X));
      pc_630X = *((long *) (1 + cont_628X));
      StemplateS = tem_629X;
      RScode_pointerS = ((-3 + (*((long *) (-3 + tem_629X)))) + (((pc_630X)>>2)));
      RSenvS = (*((long *) (9 + cont_628X)));
      ScontS = (*((long *) (-3 + cont_628X)));
      RSstackS = (9 + cont_628X);
      goto L11156;}
      break;
    case 30 : {
      arg0K0 = (SnargsS);
      goto L29309;}
      break;
    case 31 : {
      arg0K0 = (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
      goto L29309;}
      break;
    case 32 : {
      goto L11156;}
      break;
    case 33 : {
      b_626X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      tem_627X = *((long *) ((-3 + (StemplateS)) + ((((((unsigned long) b_626X)))<<2))));
      StemplateS = tem_627X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_627X))));
      goto L11156;}
      break;
    case 34 : {
      b_622X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      SnargsS = (((unsigned long) b_622X));
      b_623X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      tem_624X = *((long *) ((-3 + (StemplateS)) + ((((((unsigned long) b_623X)))<<2))));
      StemplateS = tem_624X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_624X))));
      if ((Spending_interruptPS)) {
        if ((Spending_eventsPS)) {
          Spending_eventsPS = 0;
          check_events_return_tag = 0;
          goto check_events;
         check_events_return_0:
          v_625X = check_events0_return_value;
          if (v_625X) {
            goto L13219;}
          else {
            goto L11156;}}
        else {
          goto L13219;}}
      else {
        goto L11156;}}
      break;
    case 35 : {
      b_620X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_621X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((1 == (RSvalS))) {
        RScode_pointerS = ((RScode_pointerS) + (((((((unsigned long) b_620X)))<<8)) + (((unsigned long) b_621X))));
        goto L11156;}
      else {
        goto L11156;}}
      break;
    case 36 : {
      b_331X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_330X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RScode_pointerS = ((RScode_pointerS) + (((((((unsigned long) b_331X)))<<8)) + (((unsigned long) b_330X))));
      goto L11156;}
      break;
    case 37 : {
      if ((0 == (3 & (RSvalS)))) {
        p_617X = RSvalS;
        b_618X = *((unsigned char *) (RScode_pointerS));
        RScode_pointerS = (1 + (RScode_pointerS));
        val_619X = ((p_617X)>>2);
        if ((val_619X < 0)) {
          goto L13471;}
        else {
          if ((val_619X < (((unsigned long) b_618X)))) {
            RScode_pointerS = ((RScode_pointerS) + (((val_619X)<<1)));
            b_333X = *((unsigned char *) (RScode_pointerS));
            RScode_pointerS = (1 + (RScode_pointerS));
            b_332X = *((unsigned char *) (RScode_pointerS));
            RScode_pointerS = (1 + (RScode_pointerS));
            arg0K0 = (((((((unsigned long) b_333X)))<<8)) + (((unsigned long) b_332X)));
            goto L13473;}
          else {
            goto L13471;}}}
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
        goto L12935;}}
      break;
    case 38 : {
      RSstackS = (4 + (RSstackS));
      arg2_335X = *((long *) (RSstackS));
      x_334X = RSvalS;
      if ((arg2_335X == x_334X)) {
        arg1K0 = 5;
        goto L29884;}
      else {
        arg1K0 = 1;
        goto L29884;}}
      break;
    case 39 : {
      x_336X = RSvalS;
      if ((0 == (3 & x_336X))) {
        arg1K0 = 5;
        goto L25825;}
      else {
        if ((3 == (3 & x_336X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_336X))))>>2))))) {
            arg1K0 = 5;
            goto L25825;}
          else {
            goto L25780;}}
        else {
          goto L25780;}}}
      break;
    case 40 : {
      x_337X = RSvalS;
      if ((0 == (3 & x_337X))) {
        goto L25650;}
      else {
        if ((3 == (3 & x_337X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_337X))))>>2))))) {
            goto L25650;}
          else {
            goto L25653;}}
        else {
          goto L25653;}}}
      break;
    case 41 : {
      x_338X = RSvalS;
      if ((0 == (3 & x_338X))) {
        goto L25461;}
      else {
        if ((3 == (3 & x_338X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_338X))))>>2))))) {
            goto L25461;}
          else {
            goto L25444;}}
        else {
          goto L25444;}}}
      break;
    case 42 : {
      x_339X = RSvalS;
      if ((0 == (3 & x_339X))) {
        goto L25252;}
      else {
        if ((3 == (3 & x_339X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_339X))))>>2))))) {
            goto L25252;}
          else {
            goto L25235;}}
        else {
          goto L25235;}}}
      break;
    case 43 : {
      x_340X = RSvalS;
      if ((0 == (3 & x_340X))) {
        goto L25043;}
      else {
        if ((3 == (3 & x_340X))) {
          if ((19 == (31 & ((((*((long *) (-7 + x_340X))))>>2))))) {
            goto L25043;}
          else {
            goto L25026;}}
        else {
          goto L25026;}}}
      break;
    case 44 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 5;
        goto L11156;}
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
        goto L12935;}}
      break;
    case 45 : {
      if ((0 == (3 & (RSvalS)))) {
        x_616X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 7;
        goto push_exception_continuationB;
       push_exception_continuationB_return_7:
        *((long *) (RSstackS)) = x_616X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 46 : {
      if ((0 == (3 & (RSvalS)))) {
        x_615X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 9;
        goto push_exception_continuationB;
       push_exception_continuationB_return_9:
        *((long *) (RSstackS)) = x_615X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 47 : {
      RSstackS = (4 + (RSstackS));
      arg2_341X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_341X | (RSvalS))))) {
        x_613X = RSvalS;
        z_614X = (((arg2_341X)>>2)) + (((x_613X)>>2));
        if ((536870911 < z_614X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 11;
          goto push_exception_continuationB;
         push_exception_continuationB_return_11:
          *((long *) (RSstackS)) = arg2_341X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_613X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12935;}
        else {
          if ((z_614X < -536870912)) {
            merged_arg1K0 = 0;
            merged_arg1K1 = 6;
            push_exception_continuationB_return_tag = 12;
            goto push_exception_continuationB;
           push_exception_continuationB_return_12:
            *((long *) (RSstackS)) = arg2_341X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_613X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12935;}
          else {
            RSvalS = (((z_614X)<<2));
            goto L11156;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 13;
        goto push_exception_continuationB;
       push_exception_continuationB_return_13:
        *((long *) (RSstackS)) = arg2_341X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 48 : {
      RSstackS = (4 + (RSstackS));
      arg2_612X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_612X | (RSvalS))))) {
        y_344X = RSvalS;
        a_343X = ((arg2_612X)>>2);
        b_342X = ((y_344X)>>2);
        if ((a_343X < 0)) {
          arg0K0 = (0 - a_343X);
          goto L5384;}
        else {
          arg0K0 = a_343X;
          goto L5384;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 14;
        goto push_exception_continuationB;
       push_exception_continuationB_return_14:
        *((long *) (RSstackS)) = arg2_612X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 49 : {
      RSstackS = (4 + (RSstackS));
      arg2_611X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_611X | (RSvalS))))) {
        x_346X = RSvalS;
        z_345X = (((arg2_611X)>>2)) - (((x_346X)>>2));
        if ((536870911 < z_345X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 15;
          goto push_exception_continuationB;
         push_exception_continuationB_return_15:
          *((long *) (RSstackS)) = arg2_611X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_346X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12935;}
        else {
          if ((z_345X < -536870912)) {
            merged_arg1K0 = 0;
            merged_arg1K1 = 6;
            push_exception_continuationB_return_tag = 16;
            goto push_exception_continuationB;
           push_exception_continuationB_return_16:
            *((long *) (RSstackS)) = arg2_611X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_346X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12935;}
          else {
            RSvalS = (((z_345X)<<2));
            goto L11156;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 17;
        goto push_exception_continuationB;
       push_exception_continuationB_return_17:
        *((long *) (RSstackS)) = arg2_611X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 50 : {
      RSstackS = (4 + (RSstackS));
      arg2_610X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_610X | (RSvalS))))) {
        y_349X = RSvalS;
        if ((0 == y_349X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 18;
          goto push_exception_continuationB;
         push_exception_continuationB_return_18:
          *((long *) (RSstackS)) = arg2_610X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = y_349X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12935;}
        else {
          a_348X = ((arg2_610X)>>2);
          b_347X = ((y_349X)>>2);
          if ((a_348X < 0)) {
            arg0K0 = (0 - a_348X);
            goto L7142;}
          else {
            arg0K0 = a_348X;
            goto L7142;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 19;
        goto push_exception_continuationB;
       push_exception_continuationB_return_19:
        *((long *) (RSstackS)) = arg2_610X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 51 : {
      RSstackS = (4 + (RSstackS));
      arg2_350X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_350X | (RSvalS))))) {
        x_609X = RSvalS;
        if ((arg2_350X == x_609X)) {
          arg1K0 = 5;
          goto L24848;}
        else {
          arg1K0 = 1;
          goto L24848;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 20;
        goto push_exception_continuationB;
       push_exception_continuationB_return_20:
        *((long *) (RSstackS)) = arg2_350X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 52 : {
      RSstackS = (4 + (RSstackS));
      arg2_608X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_608X | (RSvalS))))) {
        x_351X = RSvalS;
        if ((arg2_608X < x_351X)) {
          arg1K0 = 5;
          goto L24759;}
        else {
          arg1K0 = 1;
          goto L24759;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 21;
        goto push_exception_continuationB;
       push_exception_continuationB_return_21:
        *((long *) (RSstackS)) = arg2_608X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 53 : {
      RSstackS = (4 + (RSstackS));
      arg2_607X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_607X | (RSvalS))))) {
        x_354X = RSvalS;
        if ((0 == x_354X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 22;
          goto push_exception_continuationB;
         push_exception_continuationB_return_22:
          *((long *) (RSstackS)) = arg2_607X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_354X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12935;}
        else {
          a_353X = ((arg2_607X)>>2);
          b_352X = ((x_354X)>>2);
          if ((a_353X < 0)) {
            arg0K0 = (0 - a_353X);
            goto L28760;}
          else {
            arg0K0 = a_353X;
            goto L28760;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 23;
        goto push_exception_continuationB;
       push_exception_continuationB_return_23:
        *((long *) (RSstackS)) = arg2_607X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 54 : {
      RSstackS = (4 + (RSstackS));
      arg2_606X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_606X | (RSvalS))))) {
        x_356X = RSvalS;
        if ((0 == x_356X)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 24;
          goto push_exception_continuationB;
         push_exception_continuationB_return_24:
          *((long *) (RSstackS)) = arg2_606X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_356X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12935;}
        else {
          a_355X = ((arg2_606X)>>2);
          if ((a_355X < 0)) {
            arg0K0 = (0 - a_355X);
            goto L28629;}
          else {
            arg0K0 = a_355X;
            goto L28629;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 25;
        goto push_exception_continuationB;
       push_exception_continuationB_return_25:
        *((long *) (RSstackS)) = arg2_606X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 55 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto L11156;}
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
        goto L12935;}}
      break;
    case 56 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto L11156;}
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
        goto L12935;}}
      break;
    case 57 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 4;
        goto L11156;}
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
        goto L12935;}}
      break;
    case 58 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto L11156;}
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
        goto L12935;}}
      break;
    case 59 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 0;
        goto L11156;}
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
        goto L12935;}}
      break;
    case 60 : {
      if ((0 == (3 & (RSvalS)))) {
        x_357X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 31;
        goto push_exception_continuationB;
       push_exception_continuationB_return_31:
        *((long *) (RSstackS)) = x_357X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 61 : {
      if ((0 == (3 & (RSvalS)))) {
        x_358X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 33;
        goto push_exception_continuationB;
       push_exception_continuationB_return_33:
        *((long *) (RSstackS)) = x_358X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 62 : {
      if ((0 == (3 & (RSvalS)))) {
        x_359X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 35;
        goto push_exception_continuationB;
       push_exception_continuationB_return_35:
        *((long *) (RSstackS)) = x_359X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 63 : {
      if ((0 == (3 & (RSvalS)))) {
        x_360X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 37;
        goto push_exception_continuationB;
       push_exception_continuationB_return_37:
        *((long *) (RSstackS)) = x_360X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 64 : {
      if ((0 == (3 & (RSvalS)))) {
        x_361X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 39;
        goto push_exception_continuationB;
       push_exception_continuationB_return_39:
        *((long *) (RSstackS)) = x_361X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 65 : {
      if ((0 == (3 & (RSvalS)))) {
        x_362X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 41;
        goto push_exception_continuationB;
       push_exception_continuationB_return_41:
        *((long *) (RSstackS)) = x_362X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 66 : {
      if ((0 == (3 & (RSvalS)))) {
        x_363X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 43;
        goto push_exception_continuationB;
       push_exception_continuationB_return_43:
        *((long *) (RSstackS)) = x_363X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 67 : {
      if ((0 == (3 & (RSvalS)))) {
        x_364X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 45;
        goto push_exception_continuationB;
       push_exception_continuationB_return_45:
        *((long *) (RSstackS)) = x_364X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}
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
        goto L12935;}}
      break;
    case 68 : {
      if ((0 == (3 & (RSvalS)))) {
        x_605X = RSvalS;
        if ((x_605X < 0)) {
          merged_arg1K0 = 0;
          merged_arg1K1 = 6;
          push_exception_continuationB_return_tag = 47;
          goto push_exception_continuationB;
         push_exception_continuationB_return_47:
          *((long *) (RSstackS)) = x_605X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 1;
          arg1K1 = 0;
          goto L12935;}
        else {
          RSvalS = 0;
          goto L11156;}}
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
        goto L12935;}}
      break;
    case 69 : {
      if ((0 == (3 & (RSvalS)))) {
        x_365X = RSvalS;
        if ((x_365X < 0)) {
          arg0K0 = (0 - x_365X);
          goto L24139;}
        else {
          arg0K0 = x_365X;
          goto L24139;}}
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
        goto L12935;}}
      break;
    case 70 : {
      RSstackS = (4 + (RSstackS));
      arg2_604X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_604X | (RSvalS))))) {
        x_366X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 50;
        goto push_exception_continuationB;
       push_exception_continuationB_return_50:
        *((long *) (RSstackS)) = arg2_604X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_366X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 51;
        goto push_exception_continuationB;
       push_exception_continuationB_return_51:
        *((long *) (RSstackS)) = arg2_604X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 71 : {
      RSstackS = (4 + (RSstackS));
      arg2_603X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_603X | (RSvalS))))) {
        x_367X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 52;
        goto push_exception_continuationB;
       push_exception_continuationB_return_52:
        *((long *) (RSstackS)) = arg2_603X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_367X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 53;
        goto push_exception_continuationB;
       push_exception_continuationB_return_53:
        *((long *) (RSstackS)) = arg2_603X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 72 : {
      RSstackS = (4 + (RSstackS));
      arg2_602X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_602X | (RSvalS))))) {
        x_368X = RSvalS;
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 54;
        goto push_exception_continuationB;
       push_exception_continuationB_return_54:
        *((long *) (RSstackS)) = arg2_602X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_368X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 55;
        goto push_exception_continuationB;
       push_exception_continuationB_return_55:
        *((long *) (RSstackS)) = arg2_602X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 73 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = ((((~ ((((RSvalS))>>2))))<<2));
        goto L11156;}
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
        goto L12935;}}
      break;
    case 74 : {
      RSstackS = (4 + (RSstackS));
      arg2_601X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_601X | (RSvalS))))) {
        RSvalS = (((((((arg2_601X)>>2)) & ((((RSvalS))>>2))))<<2));
        goto L11156;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 57;
        goto push_exception_continuationB;
       push_exception_continuationB_return_57:
        *((long *) (RSstackS)) = arg2_601X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 75 : {
      RSstackS = (4 + (RSstackS));
      arg2_600X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_600X | (RSvalS))))) {
        RSvalS = (((((((arg2_600X)>>2)) | ((((RSvalS))>>2))))<<2));
        goto L11156;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 58;
        goto push_exception_continuationB;
       push_exception_continuationB_return_58:
        *((long *) (RSstackS)) = arg2_600X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 76 : {
      RSstackS = (4 + (RSstackS));
      arg2_599X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_599X | (RSvalS))))) {
        RSvalS = (((((((arg2_599X)>>2)) ^ ((((RSvalS))>>2))))<<2));
        goto L11156;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 59;
        goto push_exception_continuationB;
       push_exception_continuationB_return_59:
        *((long *) (RSstackS)) = arg2_599X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 77 : {
      RSstackS = (4 + (RSstackS));
      arg2_370X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_370X | (RSvalS))))) {
        x_593X = RSvalS;
        value_594X = ((arg2_370X)>>2);
        count_595X = ((x_593X)>>2);
        if ((count_595X < 0)) {
          PS_SHIFT_RIGHT(value_594X, (0 - count_595X), x_369X)
          RSvalS = (((x_369X)<<2));
          goto L11156;}
        else {
          PS_SHIFT_LEFT(value_594X, count_595X, x_596X)
          result_597X = (((((x_596X)<<2)))>>2);
          PS_SHIFT_RIGHT(result_597X, count_595X, x_598X)
          if ((value_594X == x_598X)) {
            if ((value_594X < 0)) {
              if ((result_597X < 0)) {
                goto L28481;}
              else {
                merged_arg1K0 = 0;
                merged_arg1K1 = 6;
                push_exception_continuationB_return_tag = 60;
                goto push_exception_continuationB;
               push_exception_continuationB_return_60:
                *((long *) (RSstackS)) = arg2_370X;
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = x_593X;
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 2;
                arg1K1 = 0;
                goto L12935;}}
            else {
              if ((result_597X < 0)) {
                merged_arg1K0 = 0;
                merged_arg1K1 = 6;
                push_exception_continuationB_return_tag = 61;
                goto push_exception_continuationB;
               push_exception_continuationB_return_61:
                *((long *) (RSstackS)) = arg2_370X;
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = x_593X;
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 2;
                arg1K1 = 0;
                goto L12935;}
              else {
                goto L28481;}}}
          else {
            merged_arg1K0 = 0;
            merged_arg1K1 = 6;
            push_exception_continuationB_return_tag = 62;
            goto push_exception_continuationB;
           push_exception_continuationB_return_62:
            *((long *) (RSstackS)) = arg2_370X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_593X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12935;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 63;
        goto push_exception_continuationB;
       push_exception_continuationB_return_63:
        *((long *) (RSstackS)) = arg2_370X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 78 : {
      x_371X = RSvalS;
      if ((9 == (255 & x_371X))) {
        arg1K0 = 5;
        goto L30212;}
      else {
        arg1K0 = 1;
        goto L30212;}}
      break;
    case 79 : {
      RSstackS = (4 + (RSstackS));
      arg2_373X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_373X))) {
        if ((9 == (255 & (RSvalS)))) {
          x_372X = RSvalS;
          if ((arg2_373X == x_372X)) {
            arg1K0 = 5;
            goto L23488;}
          else {
            arg1K0 = 1;
            goto L23488;}}
        else {
          goto L23461;}}
      else {
        goto L23461;}}
      break;
    case 80 : {
      RSstackS = (4 + (RSstackS));
      arg2_375X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_375X))) {
        if ((9 == (255 & (RSvalS)))) {
          x_374X = RSvalS;
          if ((arg2_375X < x_374X)) {
            arg1K0 = 5;
            goto L23399;}
          else {
            arg1K0 = 1;
            goto L23399;}}
        else {
          goto L23372;}}
      else {
        goto L23372;}}
      break;
    case 81 : {
      if ((9 == (255 & (RSvalS)))) {
        RSvalS = ((((((unsigned long) ((((RSvalS))>>8)))))<<2));
        goto L11156;}
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
        goto L12935;}}
      break;
    case 82 : {
      if ((0 == (3 & (RSvalS)))) {
        x_592X = (((RSvalS))>>2);
        if ((255 < x_592X)) {
          goto L23254;}
        else {
          if ((x_592X < 0)) {
            goto L23254;}
          else {
            RSvalS = (9 + ((((((unsigned long) x_592X)))<<8)));
            goto L11156;}}}
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
        goto L12935;}}
      break;
    case 83 : {
      x_376X = RSvalS;
      if ((21 == x_376X)) {
        arg1K0 = 5;
        goto L30239;}
      else {
        arg1K0 = 1;
        goto L30239;}}
      break;
    case 84 : {
      x_590X = RSvalS;
      b_591X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_590X))) {
        if (((31 & ((((*((long *) (-7 + x_590X))))>>2))) == (((unsigned long) b_591X)))) {
          arg1K0 = 5;
          goto L30253;}
        else {
          arg1K0 = 1;
          goto L30253;}}
      else {
        arg1K0 = 1;
        goto L30253;}}
      break;
    case 85 : {
      x_378X = RSvalS;
      b_377X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_378X))) {
        if (((31 & ((((*((long *) (-7 + x_378X))))>>2))) == (((unsigned long) b_377X)))) {
          RSvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) (-7 + x_378X))))>>8))));
          goto L11156;}
        else {
          goto L23060;}}
      else {
        goto L23060;}}
      break;
    case 86 : {
      space_587X = 4 + ((((((unsigned long) (*((unsigned char *) (RScode_pointerS))))))<<2));
      v_588X = AVAILABLEp(space_587X);
      if (v_588X) {
        goto L22893;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 5;
        goto collect_saving_temp;
       collect_saving_temp_return_5:
        okayP_589X = AVAILABLEp(space_587X);
        if (okayP_589X) {
          goto L22893;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L22893;}}}
      break;
    case 87 : {
      len_382X = SnargsS;
      space_381X = 4 + (((len_382X)<<2));
      v_380X = AVAILABLEp(space_381X);
      if (v_380X) {
        goto L22788;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 6;
        goto collect_saving_temp;
       collect_saving_temp_return_6:
        okayP_379X = AVAILABLEp(space_381X);
        if (okayP_379X) {
          goto L22788;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L22788;}}}
      break;
    case 88 : {
      x_584X = RSvalS;
      b_585X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_586X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_584X))) {
        if (((31 & ((((*((long *) (-7 + x_584X))))>>2))) == (((unsigned long) b_585X)))) {
          RSvalS = (*((long *) ((-3 + x_584X) + ((((((unsigned long) b_586X)))<<2)))));
          goto L11156;}
        else {
          goto L22688;}}
      else {
        goto L22688;}}
      break;
    case 89 : {
      RSstackS = (4 + (RSstackS));
      arg2_387X = *((long *) (RSstackS));
      x_386X = RSvalS;
      b_385X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_384X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & arg2_387X))) {
        if (((31 & ((((*((long *) (-7 + arg2_387X))))>>2))) == (((unsigned long) b_385X)))) {
          if ((3 == (3 & arg2_387X))) {
            if ((0 == (128 & (*((long *) (-7 + arg2_387X)))))) {
              addr_383X = (-3 + arg2_387X) + ((((((unsigned long) b_384X)))<<2));
              WRITE_BARRIER(addr_383X, x_386X);
              *((long *) addr_383X) = x_386X;
              RSvalS = 13;
              goto L11156;}
            else {
              goto L22473;}}
          else {
            goto L22473;}}
        else {
          goto L22473;}}
      else {
        goto L22473;}}
      break;
    case 90 : {
      RSstackS = (4 + (RSstackS));
      arg2_581X = *((long *) (RSstackS));
      x_582X = RSvalS;
      b_583X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((0 == (3 & arg2_581X))) {
        len_392X = ((arg2_581X)>>2);
        size_391X = 1 + len_392X;
        if ((len_392X < 0)) {
          goto L22148;}
        else {
          if ((4194305 < size_391X)) {
            goto L22148;}
          else {
            v_390X = AVAILABLEp(size_391X);
            if (v_390X) {
              arg2K0 = 1;
              arg0K1 = x_582X;
              goto L22166;}
            else {
              merged_arg0K0 = x_582X;
              collect_saving_temp_return_tag = 7;
              goto collect_saving_temp;
             collect_saving_temp_return_7:
              temp_389X = collect_saving_temp0_return_value;
              v_388X = AVAILABLEp(size_391X);
              arg2K0 = v_388X;
              arg0K1 = temp_389X;
              goto L22166;}}}}
      else {
        merged_arg1K0 = 1;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 66;
        goto push_exception_continuationB;
       push_exception_continuationB_return_66:
        *((long *) (RSstackS)) = ((((((unsigned long) b_583X)))<<2));
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg2_581X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_582X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 3;
        arg1K1 = 1;
        goto L12935;}}
      break;
    case 91 : {
      RSstackS = (4 + (RSstackS));
      arg2_578X = *((long *) (RSstackS));
      x_579X = RSvalS;
      b_580X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((0 == (3 & x_579X))) {
        if ((3 == (3 & arg2_578X))) {
          if (((31 & ((((*((long *) (-7 + arg2_578X))))>>2))) == (((unsigned long) b_580X)))) {
            len_394X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg2_578X))))>>8))))>>2);
            index_393X = ((x_579X)>>2);
            if ((index_393X < 0)) {
              goto L21885;}
            else {
              if ((index_393X < len_394X)) {
                RSvalS = (*((long *) ((-3 + arg2_578X) + (-4 & x_579X))));
                goto L11156;}
              else {
                goto L21885;}}}
          else {
            goto L21862;}}
        else {
          goto L21862;}}
      else {
        goto L21862;}}
      break;
    case 92 : {
      RSstackS = (4 + (RSstackS));
      arg2_573X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_574X = *((long *) (RSstackS));
      x_575X = RSvalS;
      b_576X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((0 == (3 & arg2_573X))) {
        if ((3 == (3 & arg3_574X))) {
          if (((31 & ((((*((long *) (-7 + arg3_574X))))>>2))) == (((unsigned long) b_576X)))) {
            if ((3 == (3 & arg3_574X))) {
              if ((0 == (128 & (*((long *) (-7 + arg3_574X)))))) {
                len_396X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg3_574X))))>>8))))>>2);
                index_395X = ((arg2_573X)>>2);
                if ((index_395X < 0)) {
                  goto L21526;}
                else {
                  if ((index_395X < len_396X)) {
                    addr_577X = (-3 + arg3_574X) + (-4 & arg2_573X);
                    WRITE_BARRIER(addr_577X, x_575X);
                    *((long *) addr_577X) = x_575X;
                    RSvalS = 13;
                    goto L11156;}
                  else {
                    goto L21526;}}}
              else {
                goto L21501;}}
            else {
              goto L21501;}}
          else {
            goto L21501;}}
        else {
          goto L21501;}}
      else {
        goto L21501;}}
      break;
    case 93 : {
      RSstackS = (4 + (RSstackS));
      arg2_572X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_572X | (RSvalS))))) {
        len_401X = ((arg2_572X)>>2);
        init_400X = (((RSvalS))>>2);
        size_399X = 1 + ((((3 + len_401X))>>2));
        if ((len_401X < 0)) {
          goto L21232;}
        else {
          if ((4194305 < size_399X)) {
            goto L21232;}
          else {
            v_398X = AVAILABLEp(size_399X);
            if (v_398X) {
              goto L21325;}
            else {
              merged_arg0K0 = 0;
              collect_saving_temp_return_tag = 8;
              goto collect_saving_temp;
             collect_saving_temp_return_8:
              okayP_397X = AVAILABLEp(size_399X);
              if (okayP_397X) {
                goto L21325;}
              else {
                merged_arg1K0 = 0;
                merged_arg1K1 = 8;
                push_exception_continuationB_return_tag = 67;
                goto push_exception_continuationB;
               push_exception_continuationB_return_67:
                *((long *) (RSstackS)) = (((len_401X)<<2));
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = (((init_400X)<<2));
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 2;
                arg1K1 = 0;
                goto L12935;}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 68;
        goto push_exception_continuationB;
       push_exception_continuationB_return_68:
        *((long *) (RSstackS)) = arg2_572X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 94 : {
      obj_402X = RSvalS;
      if ((3 == (3 & obj_402X))) {
        if ((17 == (31 & ((((*((long *) (-7 + obj_402X))))>>2))))) {
          RSvalS = (((((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8)))<<2));
          goto L11156;}
        else {
          goto L30296;}}
      else {
        goto L30296;}}
      break;
    case 95 : {
      RSstackS = (4 + (RSstackS));
      arg2_403X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_403X))) {
        if ((17 == (31 & ((((*((long *) (-7 + arg2_403X))))>>2))))) {
          if ((0 == (3 & (RSvalS)))) {
            index_570X = (((RSvalS))>>2);
            len_571X = (long)(((unsigned long)(*((long *) (-7 + arg2_403X))))>>8);
            if ((index_570X < 0)) {
              goto L21099;}
            else {
              if ((index_570X < len_571X)) {
                RSvalS = ((((((unsigned long) (*((unsigned char *) ((-3 + arg2_403X) + index_570X))))))<<2));
                goto L11156;}
              else {
                goto L21099;}}}
          else {
            goto L27794;}}
        else {
          goto L27794;}}
      else {
        goto L27794;}}
      break;
    case 96 : {
      RSstackS = (4 + (RSstackS));
      arg2_405X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_404X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_404X))) {
        if ((17 == (31 & ((((*((long *) (-7 + arg3_404X))))>>2))))) {
          if ((0 == (3 & (arg2_405X | (RSvalS))))) {
            index_567X = ((arg2_405X)>>2);
            Kchar_568X = (((RSvalS))>>2);
            if ((3 == (3 & arg3_404X))) {
              if ((0 == (128 & (*((long *) (-7 + arg3_404X)))))) {
                len_569X = (long)(((unsigned long)(*((long *) (-7 + arg3_404X))))>>8);
                if ((index_567X < 0)) {
                  goto L20961;}
                else {
                  if ((index_567X < len_569X)) {
                    *((unsigned char *) ((-3 + arg3_404X) + index_567X)) = Kchar_568X;
                    RSvalS = 13;
                    goto L11156;}
                  else {
                    goto L20961;}}}
              else {
                goto L20940;}}
            else {
              goto L20940;}}
          else {
            goto L27678;}}
        else {
          goto L27678;}}
      else {
        goto L27678;}}
      break;
    case 97 : {
      RSstackS = (4 + (RSstackS));
      arg2_561X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_561X))) {
        if ((9 == (255 & (RSvalS)))) {
          len_562X = ((arg2_561X)>>2);
          init_563X = (((RSvalS))>>8);
          size_564X = 1 + ((((4 + len_562X))>>2));
          if ((len_562X < 0)) {
            goto L20723;}
          else {
            if ((4194305 < size_564X)) {
              goto L20723;}
            else {
              v_565X = AVAILABLEp(size_564X);
              if (v_565X) {
                goto L20816;}
              else {
                merged_arg0K0 = 0;
                collect_saving_temp_return_tag = 9;
                goto collect_saving_temp;
               collect_saving_temp_return_9:
                okayP_566X = AVAILABLEp(size_564X);
                if (okayP_566X) {
                  goto L20816;}
                else {
                  merged_arg1K0 = 0;
                  merged_arg1K1 = 8;
                  push_exception_continuationB_return_tag = 69;
                  goto push_exception_continuationB;
                 push_exception_continuationB_return_69:
                  *((long *) (RSstackS)) = (((len_562X)<<2));
                  RSstackS = (-4 + (RSstackS));
                  *((long *) (RSstackS)) = (9 + ((((((unsigned long) init_563X)))<<8)));
                  RSstackS = (-4 + (RSstackS));
                  arg1K0 = 2;
                  arg1K1 = 0;
                  goto L12935;}}}}}
        else {
          goto L27595;}}
      else {
        goto L27595;}}
      break;
    case 98 : {
      obj_406X = RSvalS;
      if ((3 == (3 & obj_406X))) {
        if ((16 == (31 & ((((*((long *) (-7 + obj_406X))))>>2))))) {
          RSvalS = (-4 + (((((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8)))<<2)));
          goto L11156;}
        else {
          goto L30319;}}
      else {
        goto L30319;}}
      break;
    case 99 : {
      RSstackS = (4 + (RSstackS));
      arg2_407X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_407X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg2_407X))))>>2))))) {
          if ((0 == (3 & (RSvalS)))) {
            index_559X = (((RSvalS))>>2);
            len_560X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg2_407X))))>>8));
            if ((index_559X < 0)) {
              goto L20590;}
            else {
              if ((index_559X < len_560X)) {
                RSvalS = (9 + ((((((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + arg2_407X) + index_559X))))))))<<8)));
                goto L11156;}
              else {
                goto L20590;}}}
          else {
            goto L27482;}}
        else {
          goto L27482;}}
      else {
        goto L27482;}}
      break;
    case 100 : {
      RSstackS = (4 + (RSstackS));
      arg2_554X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_555X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_555X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg3_555X))))>>2))))) {
          if ((0 == (3 & arg2_554X))) {
            if ((9 == (255 & (RSvalS)))) {
              index_556X = ((arg2_554X)>>2);
              Kchar_557X = (((RSvalS))>>8);
              if ((3 == (3 & arg3_555X))) {
                if ((0 == (128 & (*((long *) (-7 + arg3_555X)))))) {
                  len_558X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg3_555X))))>>8));
                  if ((index_556X < 0)) {
                    goto L20452;}
                  else {
                    if ((index_556X < len_558X)) {
                      *((unsigned char *) ((-3 + arg3_555X) + index_556X)) = (((unsigned long) Kchar_557X));
                      RSvalS = 13;
                      goto L11156;}
                    else {
                      goto L20452;}}}
                else {
                  goto L20431;}}
              else {
                goto L20431;}}
            else {
              goto L27366;}}
          else {
            goto L27366;}}
        else {
          goto L27366;}}
      else {
        goto L27366;}}
      break;
    case 101 : {
      obj_410X = RSvalS;
      if ((3 == (3 & obj_410X))) {
        if ((4 == (31 & ((((*((long *) (-7 + obj_410X))))>>2))))) {
          x_409X = RSvalS;
          descriptor_408X = *((long *) (1 + x_409X));
          if ((17 == (255 & descriptor_408X))) {
            if ((529 == (*((long *) (1 + x_409X))))) {
              arg1K0 = 5;
              goto L20358;}
            else {
              arg1K0 = 1;
              goto L20358;}}
          else {
            arg1K0 = 5;
            goto L20358;}}
        else {
          goto L20330;}}
      else {
        goto L20330;}}
      break;
    case 102 : {
      RSstackS = (4 + (RSstackS));
      arg2_552X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_552X))) {
        if ((4 == (31 & ((((*((long *) (-7 + arg2_552X))))>>2))))) {
          x_553X = RSvalS;
          if ((1 == x_553X)) {
            goto L20121;}
          else {
            if ((5 == x_553X)) {
              goto L20121;}
            else {
              goto L20147;}}}
        else {
          goto L20147;}}
      else {
        goto L20147;}}
      break;
    case 103 : {
      x_411X = RSvalS;
      if ((3 == (3 & x_411X))) {
        if ((0 == (128 & (*((long *) (-7 + x_411X)))))) {
          arg1K0 = 1;
          goto L30345;}
        else {
          arg1K0 = 5;
          goto L30345;}}
      else {
        arg1K0 = 5;
        goto L30345;}}
      break;
    case 104 : {
      x_551X = RSvalS;
      if ((3 == (3 & x_551X))) {
        if ((0 == (128 & (*((long *) (-7 + x_551X)))))) {
          *((long *) (-7 + x_551X)) = (128 | (*((long *) (-7 + x_551X))));
          goto L30357;}
        else {
          goto L30357;}}
      else {
        goto L30357;}}
      break;
    case 105 : {
      v_413X = AVAILABLEp(6);
      if (v_413X) {
        goto L19495;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 10;
        goto collect_saving_temp;
       collect_saving_temp_return_10:
        okayP_412X = AVAILABLEp(6);
        if (okayP_412X) {
          goto L19495;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L19495;}}}
      break;
    case 106 : {
      obj_418X = RSvalS;
      if ((3 == (3 & obj_418X))) {
        if ((5 == (31 & ((((*((long *) (-7 + obj_418X))))>>2))))) {
          x_417X = RSvalS;
          if ((0 == (*((long *) (-3 + x_417X))))) {
            merged_arg1K0 = 0;
            merged_arg1K1 = 5;
            push_exception_continuationB_return_tag = 70;
            goto push_exception_continuationB;
           push_exception_continuationB_return_70:
            *((long *) (RSstackS)) = x_417X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 1;
            arg1K1 = 0;
            goto L12935;}
          else {
            os_index_416X = (((*((long *) (5 + x_417X))))>>2);
            v_415X = *((long *) (-3 + x_417X));
            if ((4 == v_415X)) {
              goto L19404;}
            else {
              if ((12 == (*((long *) (-3 + x_417X))))) {
                goto L19404;}
              else {
                v_414X = ps_close_fd(os_index_416X);
                arg0K0 = v_414X;
                goto L19393;}}}}
        else {
          goto L19367;}}
      else {
        goto L19367;}}
      break;
    case 107 : {
      RSstackS = (4 + (RSstackS));
      arg2_422X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_421X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg4_420X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg5_419X = *((long *) (RSstackS));
      if ((0 == (3 & (arg4_420X | arg3_421X)))) {
        if ((1 == arg2_422X)) {
          goto L27128;}
        else {
          if ((5 == arg2_422X)) {
            goto L27128;}
          else {
            goto L27148;}}}
      else {
        goto L27148;}}
      break;
    case 108 : {
      RSstackS = (4 + (RSstackS));
      arg2_548X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_549X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg4_550X = *((long *) (RSstackS));
      if ((0 == (3 & (arg3_549X | arg2_548X)))) {
        obj_427X = RSvalS;
        if ((3 == (3 & obj_427X))) {
          if ((5 == (31 & ((((*((long *) (-7 + obj_427X))))>>2))))) {
            start_426X = ((arg3_549X)>>2);
            count_425X = ((arg2_548X)>>2);
            channel_424X = RSvalS;
            v_423X = 8 == (*((long *) (-3 + channel_424X)));
            if (v_423X) {
              if ((3 == (3 & arg4_550X))) {
                if ((16 == (31 & ((((*((long *) (-7 + arg4_550X))))>>2))))) {
                  goto L18839;}
                else {
                  goto L18831;}}
              else {
                goto L18831;}}
            else {
              arg1K0 = 5;
              goto L18768;}}
          else {
            goto L27001;}}
        else {
          goto L27001;}}
      else {
        goto L27001;}}
      break;
    case 109 : {
      obj_545X = RSvalS;
      if ((3 == (3 & obj_545X))) {
        if ((5 == (31 & ((((*((long *) (-7 + obj_545X))))>>2))))) {
          channel_546X = RSvalS;
          head_547X = Spending_channels_headS;
          if ((1 == head_547X)) {
            n_428X = ps_abort_fd_op((*((long *) (5 + channel_546X))));
            arg0K0 = (((n_428X)<<2));
            goto L30392;}
          else {
            if ((channel_546X == head_547X)) {
              channel_431X = Spending_channels_headS;
              next_430X = *((long *) (9 + channel_431X));
              Spending_channels_headS = next_430X;
              addr_429X = 9 + channel_431X;
              WRITE_BARRIER(addr_429X, 1);
              *((long *) addr_429X) = 1;
              if ((1 == next_430X)) {
                Spending_channels_tailS = 1;
                goto L6612;}
              else {
                goto L6612;}}
            else {
              arg0K0 = (*((long *) (9 + head_547X)));
              arg0K1 = head_547X;
              goto L6618;}}}
        else {
          goto L30379;}}
      else {
        goto L30379;}}
      break;
    case 110 : {
      RSvalS = 529;
      goto L11156;}
      break;
    case 111 : {
      RSvalS = 13;
      goto L11156;}
      break;
    case 112 : {
      x_432X = RSvalS;
      merged_arg1K0 = 0;
      merged_arg1K1 = 16;
      push_exception_continuationB_return_tag = 71;
      goto push_exception_continuationB;
     push_exception_continuationB_return_71:
      *((long *) (RSstackS)) = x_432X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12935;}
      break;
    case 113 : {
      RSvalS = 1;
      goto L11156;}
      break;
    case 114 : {
      RSvalS = 21;
      goto L11156;}
      break;
    case 115 : {
      RSstackS = (4 + (RSstackS));
      arg2_442X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_441X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_441X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg3_441X))))>>2))))) {
          obj_440X = RSvalS;
          if ((3 == (3 & obj_440X))) {
            if ((16 == (31 & ((((*((long *) (-7 + obj_440X))))>>2))))) {
              x_439X = RSvalS;
              x_438X = image_writing_okayP();
              if (x_438X) {
                port_544X = ps_open_output_file((((unsigned char *)(-3 + arg3_441X))), &status_543X);
                if ((status_543X == NO_ERRORS)) {
                  status_437X = ps_write_string((((unsigned char *)(-3 + x_439X))), port_544X);
                  if ((status_437X == NO_ERRORS)) {
                    status_435X = write_image(arg2_442X, port_544X, ((void(*)(void))mark_traced_channels_closedB));
                    if ((status_435X == NO_ERRORS)) {
                      status_433X = ps_close(port_544X);
                      if ((status_433X == NO_ERRORS)) {
                        RSvalS = 13;
                        goto L11156;}
                      else {
                        arg1K0 = 21;
                        goto L18310;}}
                    else {
                      status_434X = ps_close(port_544X);
                      if ((status_434X == NO_ERRORS)) {
                        arg1K0 = 21;
                        goto L18310;}
                      else {
                        ps_write_string(((unsigned char *) "Unable to close image file"), (stderr));
                        { long ignoreXX;
                        PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                        arg1K0 = 21;
                        goto L18310;}}}
                  else {
                    status_436X = ps_close(port_544X);
                    if ((status_436X == NO_ERRORS)) {
                      arg1K0 = 21;
                      goto L18310;}
                    else {
                      ps_write_string(((unsigned char *) "Unable to close image file"), (stderr));
                      { long ignoreXX;
                      PS_WRITE_CHAR(10, (stderr), ignoreXX) }
                      arg1K0 = 21;
                      goto L18310;}}}
                else {
                  arg1K0 = 10;
                  goto L18310;}}
              else {
                arg1K0 = 15;
                goto L18310;}}
            else {
              goto L18373;}}
          else {
            goto L18373;}}
        else {
          goto L18373;}}
      else {
        goto L18373;}}
      break;
    case 116 : {
      RSvalS = 13;
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 11;
      goto collect_saving_temp;
     collect_saving_temp_return_11:
      RSvalS = 13;
      goto L11156;}
      break;
    case 117 : {
      v_444X = AVAILABLEp(6);
      if (v_444X) {
        goto L18043;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 12;
        goto collect_saving_temp;
       collect_saving_temp_return_12:
        okayP_443X = AVAILABLEp(6);
        if (okayP_443X) {
          goto L18043;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L18043;}}}
      break;
    case 118 : {
      RSstackS = (4 + (RSstackS));
      arg2_446X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_446X))) {
        x_539X = RSvalS;
        key_540X = ((arg2_446X)>>2);
        if ((5 == key_540X)) {
          RSvalS = (-4 & x_539X);
          goto L11156;}
        else {
          if ((0 == key_540X)) {
            n_445X = available();
            RSvalS = (((n_445X)<<2));
            goto L11156;}
          else {
            if ((1 == key_540X)) {
              bytes_542X = heap_size();
              RSvalS = (-4 & (3 + bytes_542X));
              goto L11156;}
            else {
              if ((2 == key_540X)) {
                RSvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
                goto L11156;}
              else {
                if ((3 == key_540X)) {
                  n_541X = gc_count();
                  RSvalS = (((n_541X)<<2));
                  goto L11156;}
                else {
                  if ((4 == key_540X)) {
                    merged_arg1K0 = 0;
                    merged_arg1K1 = 15;
                    push_exception_continuationB_return_tag = 72;
                    goto push_exception_continuationB;
                   push_exception_continuationB_return_72:
                    *((long *) (RSstackS)) = (((key_540X)<<2));
                    RSstackS = (-4 + (RSstackS));
                    *((long *) (RSstackS)) = x_539X;
                    RSstackS = (-4 + (RSstackS));
                    arg1K0 = 2;
                    arg1K1 = 0;
                    goto L12935;}
                  else {
                    merged_arg1K0 = 0;
                    merged_arg1K1 = 17;
                    push_exception_continuationB_return_tag = 73;
                    goto push_exception_continuationB;
                   push_exception_continuationB_return_73:
                    *((long *) (RSstackS)) = (((key_540X)<<2));
                    RSstackS = (-4 + (RSstackS));
                    *((long *) (RSstackS)) = x_539X;
                    RSstackS = (-4 + (RSstackS));
                    arg1K0 = 2;
                    arg1K1 = 0;
                    goto L12935;}}}}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 74;
        goto push_exception_continuationB;
       push_exception_continuationB_return_74:
        *((long *) (RSstackS)) = arg2_446X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 119 : {
      if ((0 == (3 & (RSvalS)))) {
        type_538X = (((RSvalS))>>2);
        arg2K0 = 1;
        goto L17761;}
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
        goto L12935;}}
      break;
    case 120 : {
      obj_536X = RSvalS;
      if ((3 == (3 & obj_536X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_536X))))>>2))))) {
          x_537X = RSvalS;
          arg0K0 = x_537X;
          arg2K1 = 1;
          goto L26288;}
        else {
          goto L26310;}}
      else {
        goto L26310;}}
      break;
    case 121 : {
      x_447X = RSvalS;
      arg2K0 = 1;
      arg0K1 = x_447X;
      goto L30440;}
      break;
    case 122 : {
      RSvalS = (Scurrent_threadS);
      goto L11156;}
      break;
    case 123 : {
      Scurrent_threadS = (RSvalS);
      RSvalS = 13;
      goto L11156;}
      break;
    case 124 : {
      RSvalS = (Ssession_dataS);
      goto L11156;}
      break;
    case 125 : {
      Ssession_dataS = (RSvalS);
      RSvalS = 13;
      goto L11156;}
      break;
    case 126 : {
      obj_449X = RSvalS;
      if ((3 == (3 & obj_449X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_449X))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8))))>>2)) < 148)) {
            goto L13355;}
          else {
            temp_448X = Sexception_handlersS;
            Sexception_handlersS = (RSvalS);
            RSvalS = temp_448X;
            goto L11156;}}
        else {
          goto L13355;}}
      else {
        goto L13355;}}
      break;
    case 127 : {
      obj_451X = RSvalS;
      if ((3 == (3 & obj_451X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_451X))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8))))>>2)) < 5)) {
            goto L13255;}
          else {
            temp_450X = Sinterrupt_handlersS;
            Sinterrupt_handlersS = (RSvalS);
            RSvalS = temp_450X;
            goto L11156;}}
        else {
          goto L13255;}}
      else {
        goto L13255;}}
      break;
    case 128 : {
      old_534X = Senabled_interruptsS;
      p_535X = RSvalS;
      Senabled_interruptsS = (((p_535X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        Spending_interruptPS = 0;
        if ((Spending_eventsPS)) {
          Spending_interruptPS = 1;
          goto L30476;}
        else {
          goto L30476;}}
      else {
        Spending_interruptPS = 1;
        goto L30476;}}
      break;
    case 129 : {
      RSstackS = (4 + (RSstackS));
      p_452X = *((long *) (RSstackS));
      Senabled_interruptsS = (((p_452X)>>2));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        Spending_interruptPS = 0;
        if ((Spending_eventsPS)) {
          Spending_interruptPS = 1;
          goto L11174;}
        else {
          goto L11174;}}
      else {
        Spending_interruptPS = 1;
        goto L11174;}}
      break;
    case 130 : {
      if ((0 == (3 & (RSvalS)))) {
        p_533X = RSvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
          Spending_interruptPS = 0;
          if ((Spending_eventsPS)) {
            Spending_interruptPS = 1;
            goto L17533;}
          else {
            goto L17533;}}
        else {
          Spending_interruptPS = 1;
          goto L17533;}}
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
        goto L12935;}}
      break;
    case 131 : {
      RSstackS = (4 + (RSstackS));
      arg2_453X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_453X))) {
        x_532X = RSvalS;
        if ((1 == x_532X)) {
          goto L17369;}
        else {
          if ((5 == x_532X)) {
            goto L17369;}
          else {
            goto L17389;}}}
      else {
        goto L17389;}}
      break;
    case 132 : {
      obj_458X = RSvalS;
      if ((3 == (3 & obj_458X))) {
        if ((13 == (31 & ((((*((long *) (-7 + obj_458X))))>>2))))) {
          x_457X = RSvalS;
          value_456X = *((long *) (1 + x_457X));
          name_455X = *((long *) (-3 + x_457X));
          if ((3 == (3 & name_455X))) {
            if ((16 == (31 & ((((*((long *) (-7 + name_455X))))>>2))))) {
              if ((3 == (3 & value_456X))) {
                if ((17 == (31 & ((((*((long *) (-7 + value_456X))))>>2))))) {
                  v_454X = lookup_external_name((-3 + name_455X), (-3 + value_456X));
                  if (v_454X) {
                    RSvalS = 13;
                    goto L11156;}
                  else {
                    goto L17214;}}
                else {
                  goto L17214;}}
              else {
                goto L17214;}}
            else {
              goto L17214;}}
          else {
            goto L17214;}}
        else {
          goto L17219;}}
      else {
        goto L17219;}}
      break;
    case 133 : {
      proc_531X = *((long *) ((RSstackS) + ((((SnargsS))<<2))));
      if ((3 == (3 & proc_531X))) {
        if ((13 == (31 & ((((*((long *) (-7 + proc_531X))))>>2))))) {
          SnargsS = (-1 + (SnargsS));
          RSvalS = proc_531X;
          arg1K0 = 1;
          goto L12061;}
        else {
          goto L30498;}}
      else {
        goto L30498;}}
      break;
    case 134 : {
      RSstackS = (4 + (RSstackS));
      arg2_464X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_464X))) {
        x_529X = RSvalS;
        option_530X = ((arg2_464X)>>2);
        if ((2 == option_530X)) {
          n_459X = CHEAP_TIME();
          RSvalS = (((n_459X)<<2));
          goto L11156;}
        else {
          if ((0 == option_530X)) {
            seconds_460X = run_time(&mseconds_461X);
            if ((536869 < seconds_460X)) {
              merged_arg1K0 = 0;
              merged_arg1K1 = 6;
              push_exception_continuationB_return_tag = 77;
              goto push_exception_continuationB;
             push_exception_continuationB_return_77:
              *((long *) (RSstackS)) = (((option_530X)<<2));
              RSstackS = (-4 + (RSstackS));
              *((long *) (RSstackS)) = (((seconds_460X)<<2));
              RSstackS = (-4 + (RSstackS));
              *((long *) (RSstackS)) = (((mseconds_461X)<<2));
              RSstackS = (-4 + (RSstackS));
              arg1K0 = 3;
              arg1K1 = 0;
              goto L12935;}
            else {
              RSvalS = (((((1000 * seconds_460X) + mseconds_461X))<<2));
              goto L11156;}}
          else {
            if ((1 == option_530X)) {
              seconds_462X = real_time(&mseconds_463X);
              if ((536869 < seconds_462X)) {
                merged_arg1K0 = 0;
                merged_arg1K1 = 6;
                push_exception_continuationB_return_tag = 78;
                goto push_exception_continuationB;
               push_exception_continuationB_return_78:
                *((long *) (RSstackS)) = (((option_530X)<<2));
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = (((seconds_462X)<<2));
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = (((mseconds_463X)<<2));
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 3;
                arg1K1 = 0;
                goto L12935;}
              else {
                RSvalS = (((((1000 * seconds_462X) + mseconds_463X))<<2));
                goto L11156;}}
            else {
              merged_arg1K0 = 0;
              merged_arg1K1 = 17;
              push_exception_continuationB_return_tag = 79;
              goto push_exception_continuationB;
             push_exception_continuationB_return_79:
              *((long *) (RSstackS)) = (((option_530X)<<2));
              RSstackS = (-4 + (RSstackS));
              *((long *) (RSstackS)) = x_529X;
              RSstackS = (-4 + (RSstackS));
              arg1K0 = 2;
              arg1K1 = 0;
              goto L12935;}}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 80;
        goto push_exception_continuationB;
       push_exception_continuationB_return_80:
        *((long *) (RSstackS)) = arg2_464X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 135 : {
      RSstackS = (4 + (RSstackS));
      arg2_465X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_465X))) {
        x_526X = RSvalS;
        key_527X = ((arg2_465X)>>2);
        status_528X = extended_vm(key_527X, x_526X);
        if ((0 == status_528X)) {
          RSvalS = (Sextension_valueS);
          goto L11156;}
        else {
          if ((1 == status_528X)) {
            merged_arg1K0 = 0;
            merged_arg1K1 = 19;
            push_exception_continuationB_return_tag = 81;
            goto push_exception_continuationB;
           push_exception_continuationB_return_81:
            *((long *) (RSstackS)) = (((key_527X)<<2));
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_526X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12935;}
          else {
            merged_arg1K0 = 0;
            merged_arg1K1 = 20;
            push_exception_continuationB_return_tag = 82;
            goto push_exception_continuationB;
           push_exception_continuationB_return_82:
            *((long *) (RSstackS)) = (((key_527X)<<2));
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_526X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12935;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 83;
        goto push_exception_continuationB;
       push_exception_continuationB_return_83:
        *((long *) (RSstackS)) = arg2_465X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 136 : {
      RSstackS = (4 + (RSstackS));
      arg2_466X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_466X))) {
        RSvalS = (RSvalS);
        arg1K0 = 0;
        goto L12061;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 84;
        goto push_exception_continuationB;
       push_exception_continuationB_return_84:
        *((long *) (RSstackS)) = arg2_466X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 137 : {
      RSstackS = (4 + (RSstackS));
      arg2_522X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_522X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg2_522X))))>>2))))) {
          obj_523X = RSvalS;
          if ((3 == (3 & obj_523X))) {
            if ((16 == (31 & ((((*((long *) (-7 + obj_523X))))>>2))))) {
              x_524X = RSvalS;
              len_525X = (long)(((unsigned long)(*((long *) (-7 + arg2_522X))))>>8);
              if ((len_525X == ((long)(((unsigned long)(*((long *) (-7 + x_524X))))>>8)))) {
                if (((!memcmp((void *)(-3 + x_524X), (void *)(-3 + arg2_522X),len_525X)))) {
                  arg1K0 = 5;
                  goto L16792;}
                else {
                  arg1K0 = 1;
                  goto L16792;}}
              else {
                arg1K0 = 1;
                goto L16792;}}
            else {
              goto L16759;}}
          else {
            goto L16759;}}
        else {
          goto L16759;}}
      else {
        goto L16759;}}
      break;
    case 138 : {
      obj_469X = RSvalS;
      if ((3 == (3 & obj_469X))) {
        if ((16 == (31 & ((((*((long *) (-7 + obj_469X))))>>2))))) {
          x_468X = RSvalS;
          n_467X = -1 + ((long)(((unsigned long)(*((long *) (-7 + x_468X))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L16704;}
        else {
          goto L16681;}}
      else {
        goto L16681;}}
      break;
    case 139 : {
      space_519X = 1 + ((((4 + ((((RSvalS))>>2))))>>2));
      v_520X = AVAILABLEp(space_519X);
      if (v_520X) {
        goto L16422;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 13;
        goto collect_saving_temp;
       collect_saving_temp_return_13:
        okayP_521X = AVAILABLEp(space_519X);
        if (okayP_521X) {
          goto L16422;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L16422;}}}
      break;
    case 140 : {
      v_471X = AVAILABLEp(5);
      if (v_471X) {
        goto L16288;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 14;
        goto collect_saving_temp;
       collect_saving_temp_return_14:
        okayP_470X = AVAILABLEp(5);
        if (okayP_470X) {
          goto L16288;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L16288;}}}
      break;
    case 141 : {
      RSstackS = (4 + (RSstackS));
      arg2_517X = *((long *) (RSstackS));
      x_518X = RSvalS;
      arg0K0 = x_518X;
      arg0K1 = x_518X;
      arg2K2 = 1;
      goto L16033;}
      break;
    case 142 : {
      RSstackS = (4 + (RSstackS));
      arg2_474X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_473X = *((long *) (RSstackS));
      if ((0 == (3 & (RSvalS)))) {
        index_516X = (((RSvalS))>>2);
        if ((3 == (3 & arg3_473X))) {
          if ((8 == (31 & ((((*((long *) (-7 + arg3_473X))))>>2))))) {
            if ((arg2_474X == (*((long *) (-3 + arg3_473X))))) {
              len_472X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg3_473X))))>>8))))>>2);
              if ((index_516X < 0)) {
                goto L15786;}
              else {
                if ((index_516X < len_472X)) {
                  RSvalS = (*((long *) ((-3 + arg3_473X) + (((index_516X)<<2)))));
                  goto L11156;}
                else {
                  goto L15786;}}}
            else {
              goto L15833;}}
          else {
            goto L15833;}}
        else {
          goto L15833;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 85;
        goto push_exception_continuationB;
       push_exception_continuationB_return_85:
        *((long *) (RSstackS)) = arg3_473X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg2_474X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 3;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 143 : {
      RSstackS = (4 + (RSstackS));
      arg2_512X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_513X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg4_514X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_512X))) {
        x_477X = RSvalS;
        index_476X = ((arg2_512X)>>2);
        if ((3 == (3 & arg4_514X))) {
          if ((8 == (31 & ((((*((long *) (-7 + arg4_514X))))>>2))))) {
            if ((arg3_513X == (*((long *) (-3 + arg4_514X))))) {
              if ((3 == (3 & arg4_514X))) {
                if ((0 == (128 & (*((long *) (-7 + arg4_514X)))))) {
                  len_515X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg4_514X))))>>8))))>>2);
                  if ((index_476X < 0)) {
                    goto L15405;}
                  else {
                    if ((index_476X < len_515X)) {
                      addr_475X = (-3 + arg4_514X) + (((index_476X)<<2));
                      WRITE_BARRIER(addr_475X, x_477X);
                      *((long *) addr_475X) = x_477X;
                      RSvalS = 13;
                      goto L11156;}
                    else {
                      goto L15405;}}}
                else {
                  goto L15462;}}
              else {
                goto L15462;}}
            else {
              goto L15462;}}
          else {
            goto L15462;}}
        else {
          goto L15462;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 86;
        goto push_exception_continuationB;
       push_exception_continuationB_return_86:
        *((long *) (RSstackS)) = arg4_514X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg3_513X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg2_512X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 4;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 144 : {
      RSstackS = (4 + (RSstackS));
      arg2_508X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_509X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg4_510X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg5_511X = *((long *) (RSstackS));
      if ((0 == (3 & ((arg4_510X | arg2_508X) | (RSvalS))))) {
        from_index_480X = ((arg4_510X)>>2);
        to_index_479X = ((arg2_508X)>>2);
        count_478X = (((RSvalS))>>2);
        if ((3 == (3 & arg5_511X))) {
          if ((16 == (31 & ((((*((long *) (-7 + arg5_511X))))>>2))))) {
            goto L14745;}
          else {
            goto L14740;}}
        else {
          goto L14740;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 87;
        goto push_exception_continuationB;
       push_exception_continuationB_return_87:
        *((long *) (RSstackS)) = arg5_511X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg4_510X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg3_509X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = arg2_508X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (RSvalS);
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 5;
        arg1K1 = 0;
        goto L12935;}}
      break;
    case 145 : {
      x_499X = RSvalS;
      if ((3 == (3 & x_499X))) {
        if ((6 == (31 & ((((*((long *) (-7 + x_499X))))>>2))))) {
          if ((0 == (4 & ((((*((long *) (1 + x_499X))))>>2))))) {
            goto L26707;}
          else {
            port_500X = RSvalS;
            p_501X = *((long *) (21 + port_500X));
            p_502X = *((long *) (25 + port_500X));
            b_503X = *((long *) (17 + port_500X));
            i_504X = ((p_501X)>>2);
            x_505X = *((long *) (9 + port_500X));
            if ((5 == x_505X)) {
              goto L14587;}
            else {
              if ((i_504X == (((p_502X)>>2)))) {
                goto L14587;}
              else {
                val_506X = 4 + (((i_504X)<<2));
                addr_507X = 21 + port_500X;
                WRITE_BARRIER(addr_507X, val_506X);
                *((long *) addr_507X) = val_506X;
                RSvalS = (9 + ((((((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + b_503X) + i_504X))))))))<<8)));
                goto L11156;}}}}
        else {
          goto L26707;}}
      else {
        goto L26707;}}
      break;
    case 146 : {
      x_492X = RSvalS;
      if ((3 == (3 & x_492X))) {
        if ((6 == (31 & ((((*((long *) (-7 + x_492X))))>>2))))) {
          if ((0 == (4 & ((((*((long *) (1 + x_492X))))>>2))))) {
            goto L26648;}
          else {
            port_493X = RSvalS;
            p_494X = *((long *) (21 + port_493X));
            p_495X = *((long *) (25 + port_493X));
            b_496X = *((long *) (17 + port_493X));
            i_497X = ((p_494X)>>2);
            x_498X = *((long *) (9 + port_493X));
            if ((5 == x_498X)) {
              goto L14484;}
            else {
              if ((i_497X == (((p_495X)>>2)))) {
                goto L14484;}
              else {
                RSvalS = (9 + ((((((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + b_496X) + i_497X))))))))<<8)));
                goto L11156;}}}}
        else {
          goto L26648;}}
      else {
        goto L26648;}}
      break;
    case 147 : {
      RSstackS = (4 + (RSstackS));
      arg2_481X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_481X))) {
        x_483X = RSvalS;
        if ((3 == (3 & x_483X))) {
          if ((6 == (31 & ((((*((long *) (-7 + x_483X))))>>2))))) {
            if ((0 == (8 & ((((*((long *) (1 + x_483X))))>>2))))) {
              goto L14297;}
            else {
              x_484X = RSvalS;
              Kchar_485X = ((arg2_481X)>>8);
              p_486X = *((long *) (21 + x_484X));
              b_487X = *((long *) (17 + x_484X));
              i_488X = ((p_486X)>>2);
              x_489X = *((long *) (9 + x_484X));
              if ((5 == x_489X)) {
                goto L14279;}
              else {
                if ((i_488X == ((long)(((unsigned long)(*((long *) (-7 + b_487X))))>>8)))) {
                  goto L14279;}
                else {
                  val_490X = 4 + (((i_488X)<<2));
                  addr_491X = 21 + x_484X;
                  WRITE_BARRIER(addr_491X, val_490X);
                  *((long *) addr_491X) = val_490X;
                  *((unsigned char *) ((-3 + (*((long *) (17 + x_484X)))) + i_488X)) = (((unsigned long) Kchar_485X));
                  RSvalS = 13;
                  goto L11156;}}}}
          else {
            goto L14297;}}
        else {
          goto L14297;}}
      else {
        goto L14297;}}
      break;
  }}
 L26390: {
  byte_args_692X = arg1K0;
  exception_693X = arg1K1;
  if ((64 == (SnargsS))) {
    RSstackS = (4 + (RSstackS));
    merged_arg0K0 = (*((long *) (RSstackS)));
    merged_arg0K1 = 63;
    pop_args_list_return_tag = 2;
    goto pop_args_list;
   pop_args_list_return_2:
    v_695X = pop_args_list0_return_value;
    arg0K0 = v_695X;
    goto L26402;}
  else {
    merged_arg0K0 = 25;
    merged_arg0K1 = (SnargsS);
    pop_args_list_return_tag = 3;
    goto pop_args_list;
   pop_args_list_return_3:
    v_694X = pop_args_list0_return_value;
    arg0K0 = v_694X;
    goto L26402;}}
 L11982: {
  b_217X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  *((long *) (RSstackS)) = (RSenvS);
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (1034 + ((((((unsigned long) b_217X)))<<10)));
  RSstackS = (-4 + (RSstackS));
  RSenvS = (11 + (RSstackS));
  goto L11156;}
 L12000: {
  v_216X = arg1K0;
  merged_arg1K0 = v_216X;
  copy_stack_into_heap_return_tag = 0;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_0:
  goto L11982;}
 L29663: {
  b_300X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  len_301X = 4 + ((((((unsigned long) b_300X)))<<2));
  addr_302X = ALLOCATE_SPACE(2, (4 + len_301X));
  *((long *) addr_302X) = (10 + (((len_301X)<<8)));
  stob_303X = 7 + addr_302X;
  x_304X = RSstackS;
  memcpy((void *)(1 + stob_303X), (void *)(4 + x_304X),((((((unsigned long) b_300X)))<<2)));
  RSstackS = ((RSstackS) - ((((0 - (((unsigned long) b_300X))))<<2)));
  value_305X = RSenvS;
  addr_306X = -3 + stob_303X;
  WRITE_BARRIER(addr_306X, value_305X);
  *((long *) addr_306X) = value_305X;
  RSenvS = stob_303X;
  goto L11156;}
 L11899: {
  args_218X = arg0K0;
  RSvalS = args_218X;
  SnargsS = (1 + (((unsigned long) b_655X)));
  goto L11156;}
 L26620: {
  env_57X = arg0K0;
  i_58X = arg0K1;
  if ((0 == i_58X)) {
    b_59X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    RSvalS = (*((long *) ((-3 + env_57X) + ((((((unsigned long) b_59X)))<<2)))));
    if ((529 == (RSvalS))) {
      merged_arg1K0 = 2;
      merged_arg1K1 = 0;
      push_exception_continuationB_return_tag = 88;
      goto push_exception_continuationB;
     push_exception_continuationB_return_88:
      arg1K0 = 0;
      arg1K1 = 2;
      goto L12935;}
    else {
      goto L11156;}}
  else {
    arg0K0 = (*((long *) (-3 + env_57X)));
    arg0K1 = (-1 + i_58X);
    goto L26620;}}
 L12935: {
  nargs_683X = arg1K0;
  args_684X = arg1K1;
  SnargsS = (2 + (((unsigned long) nargs_683X)));
  obj_685X = Sexception_handlersS;
  if ((3 == (3 & obj_685X))) {
    if ((2 == (31 & ((((*((long *) (-7 + obj_685X))))>>2))))) {
      goto L12996;}
    else {
      goto L13075;}}
  else {
    goto L13075;}}
 L11848: {
  env_219X = arg0K0;
  i_220X = arg0K1;
  if ((0 == i_220X)) {
    b_221X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    value_222X = RSvalS;
    addr_223X = (-3 + env_219X) + ((((((unsigned long) b_221X)))<<2));
    WRITE_BARRIER(addr_223X, value_222X);
    *((long *) addr_223X) = value_222X;
    RSvalS = 13;
    goto L11156;}
  else {
    arg0K0 = (*((long *) (-3 + env_219X)));
    arg0K1 = (-1 + i_220X);
    goto L11848;}}
 L11708: {
  space_235X = arg0K0;
  v_236X = AVAILABLEp(space_235X);
  if (v_236X) {
    arg1K0 = 0;
    goto L11710;}
  else {
    merged_arg0K0 = 0;
    collect_saving_temp_return_tag = 15;
    goto collect_saving_temp;
   collect_saving_temp_return_15:
    okayP_237X = AVAILABLEp(space_235X);
    if (okayP_237X) {
      arg1K0 = 0;
      goto L11710;}
    else {
      ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
      arg1K0 = 0;
      goto L11710;}}}
 L11551: {
  len_244X = (((((unsigned long) b_643X)))<<2);
  addr_245X = ALLOCATE_SPACE(2, (4 + len_244X));
  *((long *) addr_245X) = (10 + (((len_244X)<<8)));
  new_env_246X = 7 + addr_245X;
  arg0K0 = 0;
  goto L11561;}
 L11434: {
  key_255X = arg0K0;
  merged_arg0K0 = ((RScode_pointerS) + (((((((unsigned long) b_636X)))<<8)) + (((unsigned long) b_637X))));
  merged_arg0K1 = (((unsigned long) b_638X));
  merged_arg0K2 = key_255X;
  push_continuationB_return_tag = 0;
  goto push_continuationB;
 push_continuationB_return_0:
  goto L11156;}
 L11460: {
  v_254X = arg1K0;
  merged_arg1K0 = v_254X;
  copy_stack_into_heap_return_tag = 1;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_1:
  arg0K0 = 0;
  goto L11434;}
 L29771: {
  key_298X = arg1K0;
  if ((1 == (ScontS))) {
    arg0K0 = 1;
    goto L29773;}
  else {
    merged_arg1K0 = key_298X;
    merged_arg1K1 = 2;
    really_preserve_continuation_return_tag = 0;
    goto really_preserve_continuation;
   really_preserve_continuation_return_0:
    v_299X = really_preserve_continuation0_return_value;
    arg0K0 = v_299X;
    goto L29773;}}
 L13676: {
  RSstackS = (-11 + (Sbottom_of_stackS));
  *((long *) (-3 + (Sbottom_of_stackS))) = cont_635X;
  ScontS = (Sbottom_of_stackS);
  goto L11156;}
 L13640: {
  if ((1 == cont_635X)) {
    if ((0 == (3 & (RSvalS)))) {
      RSvalS = ((((RSvalS))>>2));
      arg1K0 = 0;
      goto L12061;}
    else {
      goto L13657;}}
  else {
    goto L13657;}}
 L12589: {
  bytes_consumed_686X = arg1K0;
  obj_689X = RSvalS;
  if ((3 == (3 & obj_689X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_689X))))>>2))))) {
      RSenvS = (*((long *) (1 + (RSvalS))));
      tem_688X = *((long *) (-3 + (RSvalS)));
      StemplateS = tem_688X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_688X))));
      if ((Spending_interruptPS)) {
        if ((Spending_eventsPS)) {
          Spending_eventsPS = 0;
          check_events_return_tag = 1;
          goto check_events;
         check_events_return_1:
          v_687X = check_events0_return_value;
          if (v_687X) {
            goto L13219;}
          else {
            goto L11156;}}
        else {
          goto L13219;}}
      else {
        goto L11156;}}
    else {
      arg1K0 = bytes_consumed_686X;
      arg1K1 = 3;
      goto L26390;}}
  else {
    arg1K0 = bytes_consumed_686X;
    arg1K1 = 3;
    goto L26390;}}
 L29812: {
  loc_295X = arg0K0;
  arg_296X = arg0K1;
  if ((top_of_args_633X < arg_296X)) {
    *((long *) loc_295X) = (*((long *) arg_296X));
    arg0K0 = (-4 + loc_295X);
    arg0K1 = (-4 + arg_296X);
    goto L29812;}
  else {
    RSstackS = loc_295X;
    arg1K0 = 0;
    goto L12589;}}
 L8316: {
  fast_208X = arg0K0;
  len_209X = arg0K1;
  slow_210X = arg0K2;
  move_slowP_211X = arg2K3;
  if ((25 == fast_208X)) {
    args_213X = RSvalS;
    nargs_212X = len_209X + (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
    if ((63 < nargs_212X)) {
      arg0K0 = (-63 + nargs_212X);
      goto L11094;}
    else {
      arg0K0 = 0;
      goto L11094;}}
  else {
    if ((3 == (3 & fast_208X))) {
      if ((0 == (31 & ((((*((long *) (-7 + fast_208X))))>>2))))) {
        if (move_slowP_211X) {
          if ((fast_208X == slow_210X)) {
            goto L13588;}
          else {
            arg0K0 = (*((long *) (1 + fast_208X)));
            arg0K1 = (1 + len_209X);
            arg0K2 = (*((long *) (1 + slow_210X)));
            arg2K3 = 0;
            goto L8316;}}
        else {
          arg0K0 = (*((long *) (1 + fast_208X)));
          arg0K1 = (1 + len_209X);
          arg0K2 = slow_210X;
          arg2K3 = 1;
          goto L8316;}}
      else {
        goto L13588;}}
    else {
      goto L13588;}}}
 L12709: {
  SnargsS = 0;
  arg1K0 = 0;
  goto L12589;}
 L12719: {
  RSstackS = (-11 + (Sbottom_of_stackS));
  *((long *) (-3 + (Sbottom_of_stackS))) = cont_631X;
  ScontS = (Sbottom_of_stackS);
  goto L12709;}
 L29309: {
  nargs_673X = arg0K0;
  if ((1 == nargs_673X)) {
    RSstackS = (4 + (RSstackS));
    RSvalS = (*((long *) (RSstackS)));
    cont_674X = ScontS;
    tem_675X = *((long *) (5 + cont_674X));
    pc_676X = *((long *) (1 + cont_674X));
    StemplateS = tem_675X;
    RScode_pointerS = ((-3 + (*((long *) (-3 + tem_675X)))) + (((pc_676X)>>2)));
    RSenvS = (*((long *) (9 + cont_674X)));
    ScontS = (*((long *) (-3 + cont_674X)));
    RSstackS = (9 + cont_674X);
    goto L11156;}
  else {
    if (((ScontS) == (Sbottom_of_stackS))) {
      arg0K0 = (*((long *) (-3 + (Sbottom_of_stackS))));
      goto L29320;}
    else {
      arg0K0 = (ScontS);
      goto L29320;}}}
 L13219: {
  arg0K0 = (4 + (RSstackS));
  arg0K1 = 0;
  goto L11009;}
 L13471: {
  arg0K0 = ((((((unsigned long) b_618X)))<<1));
  goto L13473;}
 L13473: {
  offset_215X = arg0K0;
  RScode_pointerS = ((RScode_pointerS) + offset_215X);
  goto L11156;}
 L29884: {
  v_294X = arg1K0;
  RSvalS = (((unsigned long) v_294X));
  goto L11156;}
 L25825: {
  v_87X = arg1K0;
  RSvalS = (((unsigned long) v_87X));
  goto L11156;}
 L25780: {
  if ((3 == (3 & x_336X))) {
    if ((7 == (31 & ((((*((long *) (-7 + x_336X))))>>2))))) {
      arg1K0 = 5;
      goto L25825;}
    else {
      goto L25788;}}
  else {
    goto L25788;}}
 L25650: {
  RSvalS = 5;
  goto L11156;}
 L25653: {
  if ((3 == (3 & x_337X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_337X))))>>2))))) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 89;
      goto push_exception_continuationB;
     push_exception_continuationB_return_89:
      *((long *) (RSstackS)) = x_337X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12935;}
    else {
      goto L25661;}}
  else {
    goto L25661;}}
 L25461: {
  RSvalS = 5;
  goto L11156;}
 L25444: {
  if ((3 == (3 & x_338X))) {
    if ((7 == (31 & ((((*((long *) (-7 + x_338X))))>>2))))) {
      goto L25461;}
    else {
      goto L25452;}}
  else {
    goto L25452;}}
 L25252: {
  RSvalS = 5;
  goto L11156;}
 L25235: {
  if ((3 == (3 & x_339X))) {
    if ((7 == (31 & ((((*((long *) (-7 + x_339X))))>>2))))) {
      goto L25252;}
    else {
      goto L25243;}}
  else {
    goto L25243;}}
 L25043: {
  RSvalS = 5;
  goto L11156;}
 L25026: {
  if ((3 == (3 & x_340X))) {
    if ((7 == (31 & ((((*((long *) (-7 + x_340X))))>>2))))) {
      goto L25043;}
    else {
      goto L25034;}}
  else {
    goto L25034;}}
 L5384: {
  a_285X = arg0K0;
  if ((b_342X < 0)) {
    arg0K0 = (0 - b_342X);
    goto L5388;}
  else {
    arg0K0 = b_342X;
    goto L5388;}}
 L7142: {
  a_264X = arg0K0;
  if ((b_347X < 0)) {
    arg0K0 = (0 - b_347X);
    goto L7144;}
  else {
    arg0K0 = b_347X;
    goto L7144;}}
 L24848: {
  v_88X = arg1K0;
  RSvalS = (((unsigned long) v_88X));
  goto L11156;}
 L24759: {
  v_89X = arg1K0;
  RSvalS = (((unsigned long) v_89X));
  goto L11156;}
 L28760: {
  a_30X = arg0K0;
  if ((b_352X < 0)) {
    arg0K0 = (0 - b_352X);
    goto L28762;}
  else {
    arg0K0 = b_352X;
    goto L28762;}}
 L28629: {
  a_34X = arg0K0;
  n_35X = ((x_356X)>>2);
  if ((n_35X < 0)) {
    arg0K0 = (0 - n_35X);
    goto L28631;}
  else {
    arg0K0 = n_35X;
    goto L28631;}}
 L24139: {
  r_90X = arg0K0;
  if ((536870911 < r_90X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 90;
    goto push_exception_continuationB;
   push_exception_continuationB_return_90:
    *((long *) (RSstackS)) = x_365X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 1;
    arg1K1 = 0;
    goto L12935;}
  else {
    RSvalS = (((r_90X)<<2));
    goto L11156;}}
 L28481: {
  RSvalS = (((result_597X)<<2));
  goto L11156;}
 L30212: {
  v_293X = arg1K0;
  RSvalS = (((unsigned long) v_293X));
  goto L11156;}
 L23488: {
  v_91X = arg1K0;
  RSvalS = (((unsigned long) v_91X));
  goto L11156;}
 L23461: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 91;
  goto push_exception_continuationB;
 push_exception_continuationB_return_91:
  *((long *) (RSstackS)) = arg2_373X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L23399: {
  v_92X = arg1K0;
  RSvalS = (((unsigned long) v_92X));
  goto L11156;}
 L23372: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 92;
  goto push_exception_continuationB;
 push_exception_continuationB_return_92:
  *((long *) (RSstackS)) = arg2_375X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L23254: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 93;
  goto push_exception_continuationB;
 push_exception_continuationB_return_93:
  *((long *) (RSstackS)) = (((x_592X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L30239: {
  v_292X = arg1K0;
  RSvalS = (((unsigned long) v_292X));
  goto L11156;}
 L30253: {
  v_291X = arg1K0;
  RSvalS = (((unsigned long) v_291X));
  goto L11156;}
 L23060: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 94;
  goto push_exception_continuationB;
 push_exception_continuationB_return_94:
  *((long *) (RSstackS)) = x_378X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_377X)))<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 1;
  goto L12935;}
 L22893: {
  b_97X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  b_96X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  len_95X = (((((unsigned long) b_97X)))<<2);
  addr_94X = ALLOCATE_SPACE((((unsigned long) b_96X)), (4 + len_95X));
  *((long *) addr_94X) = (2 + (((((((len_95X)<<6)) + (((unsigned long) b_96X))))<<2)));
  new_93X = 7 + addr_94X;
  if (((((unsigned long) b_97X)) < 1)) {
    goto L22932;}
  else {
    *((long *) (-7 + (new_93X + ((((((unsigned long) b_97X)))<<2))))) = (RSvalS);
    arg0K0 = (-2 + (((unsigned long) b_97X)));
    goto L22916;}}
 L22788: {
  b_99X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  len_100X = ((len_382X)<<2);
  addr_101X = ALLOCATE_SPACE((((unsigned long) b_99X)), (4 + len_100X));
  *((long *) addr_101X) = (2 + (((((((len_100X)<<6)) + (((unsigned long) b_99X))))<<2)));
  new_102X = 7 + addr_101X;
  arg0K0 = (-1 + len_382X);
  goto L22800;}
 L22688: {
  merged_arg1K0 = 2;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 95;
  goto push_exception_continuationB;
 push_exception_continuationB_return_95:
  *((long *) (RSstackS)) = x_584X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_585X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_586X)))<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 2;
  goto L12935;}
 L22473: {
  merged_arg1K0 = 2;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 96;
  goto push_exception_continuationB;
 push_exception_continuationB_return_96:
  *((long *) (RSstackS)) = arg2_387X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_385X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_384X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_386X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 2;
  goto L12935;}
 L22148: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 97;
  goto push_exception_continuationB;
 push_exception_continuationB_return_97:
  *((long *) (RSstackS)) = ((((((unsigned long) b_583X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((len_392X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_582X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 1;
  goto L12935;}
 L22166: {
  okayP_106X = arg2K0;
  init_107X = arg0K1;
  if (okayP_106X) {
    len_108X = ((len_392X)<<2);
    addr_109X = ALLOCATE_SPACE((((unsigned long) b_583X)), (4 + len_108X));
    *((long *) addr_109X) = (2 + (((((((len_108X)<<6)) + (((unsigned long) b_583X))))<<2)));
    val_110X = 7 + addr_109X;
    arg0K0 = (-1 + len_392X);
    goto L22190;}
  else {
    merged_arg1K0 = 1;
    merged_arg1K1 = 8;
    push_exception_continuationB_return_tag = 98;
    goto push_exception_continuationB;
   push_exception_continuationB_return_98:
    *((long *) (RSstackS)) = ((((((unsigned long) b_583X)))<<2));
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (((len_392X)<<2));
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = init_107X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 3;
    arg1K1 = 1;
    goto L12935;}}
 L21885: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 99;
  goto push_exception_continuationB;
 push_exception_continuationB_return_99:
  *((long *) (RSstackS)) = arg2_578X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_580X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_579X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 1;
  goto L12935;}
 L21862: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 100;
  goto push_exception_continuationB;
 push_exception_continuationB_return_100:
  *((long *) (RSstackS)) = arg2_578X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_580X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_579X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 1;
  goto L12935;}
 L21526: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 101;
  goto push_exception_continuationB;
 push_exception_continuationB_return_101:
  *((long *) (RSstackS)) = arg3_574X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_576X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_573X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_575X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 1;
  goto L12935;}
 L21501: {
  merged_arg1K0 = 1;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 102;
  goto push_exception_continuationB;
 push_exception_continuationB_return_102:
  *((long *) (RSstackS)) = arg3_574X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) b_576X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_573X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_575X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 1;
  goto L12935;}
 L21232: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 103;
  goto push_exception_continuationB;
 push_exception_continuationB_return_103:
  *((long *) (RSstackS)) = (((len_401X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((init_400X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L21325: {
  addr_111X = ALLOCATE_SPACE(17, (4 + len_401X));
  *((long *) addr_111X) = (70 + (((len_401X)<<8)));
  vector_112X = 7 + addr_111X;
  arg0K0 = (-1 + len_401X);
  goto L21272;}
 L30296: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 104;
  goto push_exception_continuationB;
 push_exception_continuationB_return_104:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L21099: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 105;
  goto push_exception_continuationB;
 push_exception_continuationB_return_105:
  *((long *) (RSstackS)) = arg2_403X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_570X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L27794: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 106;
  goto push_exception_continuationB;
 push_exception_continuationB_return_106:
  *((long *) (RSstackS)) = arg2_403X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L20961: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 107;
  goto push_exception_continuationB;
 push_exception_continuationB_return_107:
  *((long *) (RSstackS)) = arg3_404X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_567X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((Kchar_568X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L20940: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 108;
  goto push_exception_continuationB;
 push_exception_continuationB_return_108:
  *((long *) (RSstackS)) = arg3_404X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_567X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((Kchar_568X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L27678: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 109;
  goto push_exception_continuationB;
 push_exception_continuationB_return_109:
  *((long *) (RSstackS)) = arg3_404X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_405X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L20723: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 110;
  goto push_exception_continuationB;
 push_exception_continuationB_return_110:
  *((long *) (RSstackS)) = (((len_562X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (9 + ((((((unsigned long) init_563X)))<<8)));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L20816: {
  len_36X = 1 + len_562X;
  addr_37X = ALLOCATE_SPACE(16, (4 + len_36X));
  *((long *) addr_37X) = (66 + (((len_36X)<<8)));
  string_38X = 7 + addr_37X;
  *((unsigned char *) ((-3 + string_38X) + len_562X)) = 0;
  arg0K0 = (-1 + len_562X);
  goto L20763;}
 L27595: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 111;
  goto push_exception_continuationB;
 push_exception_continuationB_return_111:
  *((long *) (RSstackS)) = arg2_561X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L30319: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 112;
  goto push_exception_continuationB;
 push_exception_continuationB_return_112:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L20590: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 113;
  goto push_exception_continuationB;
 push_exception_continuationB_return_113:
  *((long *) (RSstackS)) = arg2_407X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_559X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L27482: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 114;
  goto push_exception_continuationB;
 push_exception_continuationB_return_114:
  *((long *) (RSstackS)) = arg2_407X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L20452: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 115;
  goto push_exception_continuationB;
 push_exception_continuationB_return_115:
  *((long *) (RSstackS)) = arg3_555X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_556X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (9 + ((((((unsigned long) Kchar_557X)))<<8)));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L20431: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 116;
  goto push_exception_continuationB;
 push_exception_continuationB_return_116:
  *((long *) (RSstackS)) = arg3_555X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_556X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (9 + ((((((unsigned long) Kchar_557X)))<<8)));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L27366: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 117;
  goto push_exception_continuationB;
 push_exception_continuationB_return_117:
  *((long *) (RSstackS)) = arg3_555X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_554X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L20358: {
  v_114X = arg1K0;
  RSvalS = (((unsigned long) v_114X));
  goto L11156;}
 L20330: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 118;
  goto push_exception_continuationB;
 push_exception_continuationB_return_118:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L20121: {
  if ((1 == (RSvalS))) {
    addr_116X = 1 + arg2_552X;
    WRITE_BARRIER(addr_116X, 273);
    *((long *) addr_116X) = 273;
    goto L20146;}
  else {
    if ((17 == (255 & (*((long *) (1 + arg2_552X)))))) {
      addr_115X = 1 + arg2_552X;
      WRITE_BARRIER(addr_115X, 529);
      *((long *) addr_115X) = 529;
      goto L20146;}
    else {
      goto L20146;}}}
 L20147: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 119;
  goto push_exception_continuationB;
 push_exception_continuationB_return_119:
  *((long *) (RSstackS)) = arg2_552X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L30345: {
  v_290X = arg1K0;
  RSvalS = (((unsigned long) v_290X));
  goto L11156;}
 L30357: {
  RSvalS = x_551X;
  goto L11156;}
 L19495: {
  RSstackS = (4 + (RSstackS));
  arg2_132X = *((long *) (RSstackS));
  if ((0 == (3 & (RSvalS)))) {
    mode_133X = (((RSvalS))>>2);
    if ((1 == mode_133X)) {
      goto L19749;}
    else {
      if ((2 == mode_133X)) {
        goto L19749;}
      else {
        if ((3 == mode_133X)) {
          goto L19749;}
        else {
          if ((4 == mode_133X)) {
            goto L19749;}
          else {
            merged_arg1K0 = 0;
            merged_arg1K1 = 5;
            push_exception_continuationB_return_tag = 120;
            goto push_exception_continuationB;
           push_exception_continuationB_return_120:
            *((long *) (RSstackS)) = arg2_132X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = (((mode_133X)<<2));
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12935;}}}}}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 5;
    push_exception_continuationB_return_tag = 121;
    goto push_exception_continuationB;
   push_exception_continuationB_return_121:
    *((long *) (RSstackS)) = arg2_132X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (RSvalS);
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}}
 L19404: {
  v_140X = ps_close_fd(os_index_416X);
  arg0K0 = v_140X;
  goto L19393;}
 L19393: {
  status_141X = arg0K0;
  *((Svm_channelsS) + os_index_416X) = 1;
  addr_142X = -3 + x_417X;
  WRITE_BARRIER(addr_142X, 0);
  *((long *) addr_142X) = 0;
  if ((status_141X == NO_ERRORS)) {
    RSvalS = 13;
    goto L11156;}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 21;
    push_exception_continuationB_return_tag = 122;
    goto push_exception_continuationB;
   push_exception_continuationB_return_122:
    *((long *) (RSstackS)) = x_417X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 1;
    arg1K1 = 0;
    goto L12935;}}
 L19367: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 123;
  goto push_exception_continuationB;
 push_exception_continuationB_return_123:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L27128: {
  obj_48X = RSvalS;
  if ((3 == (3 & obj_48X))) {
    if ((5 == (31 & ((((*((long *) (-7 + obj_48X))))>>2))))) {
      x_47X = RSvalS;
      if ((1 == arg2_422X)) {
        arg2K0 = 0;
        goto L27145;}
      else {
        arg2K0 = 1;
        goto L27145;}}
    else {
      goto L27148;}}
  else {
    goto L27148;}}
 L27148: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 124;
  goto push_exception_continuationB;
 push_exception_continuationB_return_124:
  *((long *) (RSstackS)) = arg5_419X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg4_420X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_421X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_422X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 5;
  arg1K1 = 0;
  goto L12935;}
 L18839: {
  if ((3 == (3 & arg4_550X))) {
    if ((16 == (31 & ((((*((long *) (-7 + arg4_550X))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg4_550X))))>>8)));
      goto L18851;}
    else {
      goto L18847;}}
  else {
    goto L18847;}}
 L18831: {
  if ((3 == (3 & arg4_550X))) {
    if ((17 == (31 & ((((*((long *) (-7 + arg4_550X))))>>2))))) {
      goto L18839;}
    else {
      arg1K0 = 5;
      goto L18768;}}
  else {
    arg1K0 = 5;
    goto L18768;}}
 L18768: {
  reason_56X = arg1K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = reason_56X;
  push_exception_continuationB_return_tag = 125;
  goto push_exception_continuationB;
 push_exception_continuationB_return_125:
  *((long *) (RSstackS)) = arg4_550X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((start_426X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((count_425X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = channel_424X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 0;
  goto L12935;}
 L27001: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 126;
  goto push_exception_continuationB;
 push_exception_continuationB_return_126:
  *((long *) (RSstackS)) = arg4_550X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_549X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_548X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 0;
  goto L12935;}
 L30392: {
  val_267X = arg0K0;
  RSvalS = val_267X;
  goto L11156;}
 L6612: {
  arg0K0 = (*((long *) (13 + channel_546X)));
  goto L30392;}
 L6618: {
  ch_271X = arg0K0;
  prev_272X = arg0K1;
  if ((1 == ch_271X)) {
    n_274X = ps_abort_fd_op((*((long *) (5 + channel_546X))));
    arg0K0 = (((n_274X)<<2));
    goto L30392;}
  else {
    if ((ch_271X == channel_546X)) {
      y_273X = Spending_channels_tailS;
      if ((ch_271X == y_273X)) {
        Spending_channels_tailS = prev_272X;
        goto L6638;}
      else {
        goto L6638;}}
    else {
      arg0K0 = (*((long *) (9 + ch_271X)));
      arg0K1 = ch_271X;
      goto L6618;}}}
 L30379: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 127;
  goto push_exception_continuationB;
 push_exception_continuationB_return_127:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L18310: {
  reason_143X = arg1K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = reason_143X;
  push_exception_continuationB_return_tag = 128;
  goto push_exception_continuationB;
 push_exception_continuationB_return_128:
  *((long *) (RSstackS)) = arg3_441X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_442X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_439X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L18373: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 129;
  goto push_exception_continuationB;
 push_exception_continuationB_return_129:
  *((long *) (RSstackS)) = arg3_441X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_442X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L18043: {
  RSstackS = (4 + (RSstackS));
  arg2_145X = *((long *) (RSstackS));
  x_144X = RSvalS;
  if ((3 == (3 & arg2_145X))) {
    if ((3 == (3 & x_144X))) {
      if ((3 == (31 & ((((*((long *) (-7 + x_144X))))>>2))))) {
        addr_146X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_146X) = 2050;
        x_147X = 7 + addr_146X;
        *((long *) (-3 + x_147X)) = arg2_145X;
        *((long *) (1 + x_147X)) = x_144X;
        b_148X = Sfinalizer_alistS;
        addr_149X = ALLOCATE_SPACE(0, 12);
        *((long *) addr_149X) = 2050;
        x_150X = 7 + addr_149X;
        *((long *) (-3 + x_150X)) = x_147X;
        *((long *) (1 + x_150X)) = b_148X;
        Sfinalizer_alistS = x_150X;
        RSvalS = 13;
        goto L11156;}
      else {
        goto L18136;}}
    else {
      goto L18136;}}
  else {
    goto L18136;}}
 L17761: {
  firstP_151X = arg2K0;
  vector_152X = find_all(type_538X);
  if ((1 == vector_152X)) {
    if (firstP_151X) {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 16;
      goto collect_saving_temp;
     collect_saving_temp_return_16:
      arg2K0 = 0;
      goto L17761;}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 8;
      push_exception_continuationB_return_tag = 130;
      goto push_exception_continuationB;
     push_exception_continuationB_return_130:
      *((long *) (RSstackS)) = (((type_538X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12935;}}
  else {
    RSvalS = vector_152X;
    goto L11156;}}
 L26288: {
  table_83X = arg0K0;
  firstP_84X = arg2K1;
  symbols_85X = find_all(1);
  if ((1 == symbols_85X)) {
    if (firstP_84X) {
      merged_arg0K0 = table_83X;
      collect_saving_temp_return_tag = 17;
      goto collect_saving_temp;
     collect_saving_temp_return_17:
      v_86X = collect_saving_temp0_return_value;
      arg0K0 = v_86X;
      arg2K1 = 0;
      goto L26288;}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 8;
      push_exception_continuationB_return_tag = 131;
      goto push_exception_continuationB;
     push_exception_continuationB_return_131:
      *((long *) (RSstackS)) = table_83X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12935;}}
  else {
    arg0K0 = table_83X;
    arg0K1 = symbols_85X;
    arg2K2 = 1;
    goto L13938;}}
 L26310: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 132;
  goto push_exception_continuationB;
 push_exception_continuationB_return_132:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L30440: {
  firstP_286X = arg2K0;
  type_287X = arg0K1;
  vector_288X = find_all_records(type_287X);
  if ((1 == vector_288X)) {
    if (firstP_286X) {
      merged_arg0K0 = type_287X;
      collect_saving_temp_return_tag = 18;
      goto collect_saving_temp;
     collect_saving_temp_return_18:
      v_289X = collect_saving_temp0_return_value;
      arg2K0 = 0;
      arg0K1 = v_289X;
      goto L30440;}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 8;
      push_exception_continuationB_return_tag = 133;
      goto push_exception_continuationB;
     push_exception_continuationB_return_133:
      *((long *) (RSstackS)) = type_287X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12935;}}
  else {
    RSvalS = vector_288X;
    goto L11156;}}
 L13355: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 134;
  goto push_exception_continuationB;
 push_exception_continuationB_return_134:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L13255: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 135;
  goto push_exception_continuationB;
 push_exception_continuationB_return_135:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L30476: {
  RSvalS = (((old_534X)<<2));
  goto L11156;}
 L11174: {
  RSstackS = (4 + (RSstackS));
  SnargsS = ((((*((long *) (RSstackS))))>>2));
  RSstackS = (4 + (RSstackS));
  tem_256X = *((long *) (RSstackS));
  StemplateS = tem_256X;
  RScode_pointerS = (-3 + (*((long *) (-3 + tem_256X))));
  RSstackS = (4 + (RSstackS));
  RSvalS = (*((long *) (RSstackS)));
  goto L11156;}
 L17533: {
  n_153X = schedule_alarm_interrupt((((p_533X)>>2)));
  RSvalS = (((n_153X)<<2));
  goto L11156;}
 L17369: {
  if ((1 == (RSvalS))) {
    arg2K0 = 0;
    goto L17373;}
  else {
    arg2K0 = 1;
    goto L17373;}}
 L17389: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 136;
  goto push_exception_continuationB;
 push_exception_continuationB_return_136:
  *((long *) (RSstackS)) = arg2_453X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L17214: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 18;
  push_exception_continuationB_return_tag = 137;
  goto push_exception_continuationB;
 push_exception_continuationB_return_137:
  *((long *) (RSstackS)) = x_457X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L17219: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 138;
  goto push_exception_continuationB;
 push_exception_continuationB_return_138:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L12061: {
  option_25X = arg1K0;
  if ((0 == (((unsigned long) option_25X)))) {
    SenvS = RSenvS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    SvalS = RSvalS;
    return (RSvalS);}
  else {
    if ((1 == (((unsigned long) option_25X)))) {
      v_27X = (long(*)())((*((long *) (-3 + (*((long *) (1 + (RSvalS))))))));
      v_26X = v_27X((SnargsS), (4 + (RSstackS)));
      RSvalS = v_26X;
      RSstackS = ((RSstackS) - ((((-1 - (SnargsS)))<<2)));
      goto L12059;}
    else {
      ps_error(((unsigned char *) "unknown VM return option"), 1, (((unsigned long) option_25X)));
      SenvS = RSenvS;
      SstackS = RSstackS;
      Scode_pointerS = RScode_pointerS;
      SvalS = RSvalS;
      return -1;}}}
 L30498: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 139;
  goto push_exception_continuationB;
 push_exception_continuationB_return_139:
  *((long *) (RSstackS)) = proc_531X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L16792: {
  v_156X = arg1K0;
  RSvalS = (((unsigned long) v_156X));
  goto L11156;}
 L16759: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 140;
  goto push_exception_continuationB;
 push_exception_continuationB_return_140:
  *((long *) (RSstackS)) = arg2_522X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L16704: {
  i_157X = arg0K0;
  h_158X = arg0K1;
  if ((i_157X < n_467X)) {
    arg0K0 = (1 + i_157X);
    arg0K1 = (h_158X + (((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + x_468X) + i_157X))))))));
    goto L16704;}
  else {
    RSvalS = (((h_158X)<<2));
    goto L11156;}}
 L16681: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 141;
  goto push_exception_continuationB;
 push_exception_continuationB_return_141:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L16422: {
  RSstackS = (4 + (RSstackS));
  arg2_162X = *((long *) (RSstackS));
  if ((0 == (3 & (RSvalS)))) {
    n_163X = (((RSvalS))>>2);
    if ((3 == (3 & arg2_162X))) {
      if ((0 == (31 & ((((*((long *) (-7 + arg2_162X))))>>2))))) {
        goto L16543;}
      else {
        goto L16455;}}
    else {
      goto L16455;}}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 5;
    push_exception_continuationB_return_tag = 142;
    goto push_exception_continuationB;
   push_exception_continuationB_return_142:
    *((long *) (RSstackS)) = arg2_162X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (RSvalS);
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}}
 L16288: {
  RSstackS = (4 + (RSstackS));
  arg2_180X = *((long *) (RSstackS));
  if ((3 == (3 & arg2_180X))) {
    if ((16 == (31 & ((((*((long *) (-7 + arg2_180X))))>>2))))) {
      obj_181X = RSvalS;
      if ((3 == (3 & obj_181X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_181X))))>>2))))) {
          table_182X = RSvalS;
          v_183X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + table_182X))))>>8))))>>2);
          n_184X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg2_180X))))>>8));
          arg0K0 = 0;
          arg0K1 = 0;
          goto L9819;}
        else {
          goto L16312;}}
      else {
        goto L16312;}}
    else {
      goto L16312;}}
  else {
    goto L16312;}}
 L16033: {
  list_185X = arg0K0;
  slow_186X = arg0K1;
  move_slowP_187X = arg2K2;
  if ((25 == list_185X)) {
    RSvalS = 1;
    goto L11156;}
  else {
    if ((3 == (3 & list_185X))) {
      if ((0 == (31 & ((((*((long *) (-7 + list_185X))))>>2))))) {
        head_188X = *((long *) (-3 + list_185X));
        if ((3 == (3 & head_188X))) {
          if ((0 == (31 & ((((*((long *) (-7 + head_188X))))>>2))))) {
            if (((*((long *) (-3 + head_188X))) == arg2_517X)) {
              RSvalS = head_188X;
              goto L11156;}
            else {
              list_189X = *((long *) (1 + list_185X));
              if ((list_189X == slow_186X)) {
                merged_arg1K0 = 0;
                merged_arg1K1 = 5;
                push_exception_continuationB_return_tag = 143;
                goto push_exception_continuationB;
               push_exception_continuationB_return_143:
                *((long *) (RSstackS)) = arg2_517X;
                RSstackS = (-4 + (RSstackS));
                *((long *) (RSstackS)) = x_518X;
                RSstackS = (-4 + (RSstackS));
                arg1K0 = 2;
                arg1K1 = 0;
                goto L12935;}
              else {
                if (move_slowP_187X) {
                  arg0K0 = list_189X;
                  arg0K1 = (*((long *) (1 + slow_186X)));
                  arg2K2 = 0;
                  goto L16033;}
                else {
                  arg0K0 = list_189X;
                  arg0K1 = slow_186X;
                  arg2K2 = 1;
                  goto L16033;}}}}
          else {
            merged_arg1K0 = 0;
            merged_arg1K1 = 5;
            push_exception_continuationB_return_tag = 144;
            goto push_exception_continuationB;
           push_exception_continuationB_return_144:
            *((long *) (RSstackS)) = arg2_517X;
            RSstackS = (-4 + (RSstackS));
            *((long *) (RSstackS)) = x_518X;
            RSstackS = (-4 + (RSstackS));
            arg1K0 = 2;
            arg1K1 = 0;
            goto L12935;}}
        else {
          merged_arg1K0 = 0;
          merged_arg1K1 = 5;
          push_exception_continuationB_return_tag = 145;
          goto push_exception_continuationB;
         push_exception_continuationB_return_145:
          *((long *) (RSstackS)) = arg2_517X;
          RSstackS = (-4 + (RSstackS));
          *((long *) (RSstackS)) = x_518X;
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 2;
          arg1K1 = 0;
          goto L12935;}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 146;
        goto push_exception_continuationB;
       push_exception_continuationB_return_146:
        *((long *) (RSstackS)) = arg2_517X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = x_518X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 147;
      goto push_exception_continuationB;
     push_exception_continuationB_return_147:
      *((long *) (RSstackS)) = arg2_517X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = x_518X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12935;}}}
 L15786: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 148;
  goto push_exception_continuationB;
 push_exception_continuationB_return_148:
  *((long *) (RSstackS)) = arg3_473X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_474X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_516X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L15833: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 149;
  goto push_exception_continuationB;
 push_exception_continuationB_return_149:
  *((long *) (RSstackS)) = arg3_473X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg2_474X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_516X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L15405: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 7;
  push_exception_continuationB_return_tag = 150;
  goto push_exception_continuationB;
 push_exception_continuationB_return_150:
  *((long *) (RSstackS)) = arg4_514X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_513X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_476X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_477X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 0;
  goto L12935;}
 L15462: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 151;
  goto push_exception_continuationB;
 push_exception_continuationB_return_151:
  *((long *) (RSstackS)) = arg4_514X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_513X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((index_476X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_477X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 4;
  arg1K1 = 0;
  goto L12935;}
 L14745: {
  if ((3 == (3 & arg5_511X))) {
    if ((16 == (31 & ((((*((long *) (-7 + arg5_511X))))>>2))))) {
      goto L14758;}
    else {
      goto L14753;}}
  else {
    goto L14753;}}
 L14740: {
  if ((3 == (3 & arg5_511X))) {
    if ((17 == (31 & ((((*((long *) (-7 + arg5_511X))))>>2))))) {
      goto L14745;}
    else {
      goto L14820;}}
  else {
    goto L14820;}}
 L26707: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 152;
  goto push_exception_continuationB;
 push_exception_continuationB_return_152:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L14587: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 14;
  push_exception_continuationB_return_tag = 153;
  goto push_exception_continuationB;
 push_exception_continuationB_return_153:
  *((long *) (RSstackS)) = port_500X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L26648: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 154;
  goto push_exception_continuationB;
 push_exception_continuationB_return_154:
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L14484: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 14;
  push_exception_continuationB_return_tag = 155;
  goto push_exception_continuationB;
 push_exception_continuationB_return_155:
  *((long *) (RSstackS)) = port_493X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L14297: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 156;
  goto push_exception_continuationB;
 push_exception_continuationB_return_156:
  *((long *) (RSstackS)) = arg2_481X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L14279: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 14;
  push_exception_continuationB_return_tag = 157;
  goto push_exception_continuationB;
 push_exception_continuationB_return_157:
  *((long *) (RSstackS)) = (9 + ((((((unsigned long) Kchar_485X)))<<8)));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_484X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L26402: {
  args_690X = arg0K0;
  a1_691X = RSvalS;
  *((long *) (RSstackS)) = ((((((unsigned long) (*((unsigned char *) (-1 + ((RScode_pointerS) - (((unsigned long) byte_args_692X)))))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) exception_693X)))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a1_691X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = args_690X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = byte_args_692X;
  goto L12935;}
 L12996: {
  RSvalS = (*((long *) ((-3 + (Sexception_handlersS)) + ((((((unsigned long) (*((unsigned char *) (-1 + ((RScode_pointerS) - (((unsigned long) args_684X)))))))))<<2)))));
  obj_682X = RSvalS;
  if ((3 == (3 & obj_682X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_682X))))>>2))))) {
      goto L13015;}
    else {
      goto L13092;}}
  else {
    goto L13092;}}
 L13075: {
  merged_arg3K0 = ((unsigned char *) "exception-handlers is not a vector");
  loseD0_return_tag = 0;
  goto loseD0;
 loseD0_return_0:
  goto L12996;}
 L11710: {
  key_232X = arg1K0;
  p_233X = RSenvS;
  if ((3 == (3 & p_233X))) {
    if ((p_233X < (Sstack_beginS))) {
      goto L11755;}
    else {
      if (((Sstack_endS) < p_233X)) {
        goto L11755;}
      else {
        merged_arg0K0 = (RSenvS);
        merged_arg0K1 = (ScontS);
        merged_arg1K2 = key_232X;
        merged_arg1K3 = 0;
        save_env_in_heap_return_tag = 0;
        goto save_env_in_heap;
       save_env_in_heap_return_0:
        v_234X = save_env_in_heap0_return_value;
        RSenvS = v_234X;
        goto L11755;}}}
  else {
    goto L11755;}}
 L11561: {
  i_247X = arg0K0;
  if ((i_247X == (((unsigned long) b_643X)))) {
    b_250X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    a_251X = *((long *) ((-3 + (StemplateS)) + ((((((unsigned long) b_250X)))<<2))));
    addr_252X = ALLOCATE_SPACE(3, 12);
    *((long *) addr_252X) = 2062;
    x_253X = 7 + addr_252X;
    *((long *) (-3 + x_253X)) = a_251X;
    *((long *) (1 + x_253X)) = new_env_246X;
    if ((3 == (3 & x_253X))) {
      if ((0 == (128 & (*((long *) (-7 + x_253X)))))) {
        *((long *) (-7 + x_253X)) = (128 | (*((long *) (-7 + x_253X))));
        arg0K0 = x_253X;
        goto L11588;}
      else {
        arg0K0 = x_253X;
        goto L11588;}}
    else {
      arg0K0 = x_253X;
      goto L11588;}}
  else {
    b_249X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    env_248X = RSenvS;
    arg0K0 = env_248X;
    arg0K1 = (((unsigned long) b_249X));
    goto L11667;}}
 L29773: {
  v_297X = arg0K0;
  RSvalS = v_297X;
  goto L11156;}
 L13657: {
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
  *((long *) (RSstackS)) = cont_635X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L11094: {
  overflow_207X = arg0K0;
  arg0K0 = (len_209X - overflow_207X);
  arg0K1 = args_213X;
  goto L11124;}
 L13588: {
  merged_arg0K0 = 25;
  merged_arg0K1 = (((unsigned long) (*((unsigned char *) (RScode_pointerS)))));
  pop_args_list_return_tag = 4;
  goto pop_args_list;
 pop_args_list_return_4:
  args_214X = pop_args_list0_return_value;
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 159;
  goto push_exception_continuationB;
 push_exception_continuationB_return_159:
  *((long *) (RSstackS)) = proc_328X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = args_214X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  arg1K1 = 0;
  goto L12935;}
 L29320: {
  cont_665X = arg0K0;
  if ((3 == (3 & cont_665X))) {
    if ((9 == (31 & ((((*((long *) (-7 + cont_665X))))>>2))))) {
      next_op_667X = *((unsigned char *) ((-3 + (*((long *) (-3 + (*((long *) (5 + cont_665X))))))) + ((((*((long *) (1 + cont_665X))))>>2))));
      if ((32 == (((unsigned long) next_op_667X)))) {
        cont_670X = ScontS;
        tem_671X = *((long *) (5 + cont_670X));
        pc_672X = *((long *) (1 + cont_670X));
        StemplateS = tem_671X;
        RScode_pointerS = ((-3 + (*((long *) (-3 + tem_671X)))) + (((pc_672X)>>2)));
        RSenvS = (*((long *) (9 + cont_670X)));
        ScontS = (*((long *) (-3 + cont_670X)));
        RSstackS = (9 + cont_670X);
        goto L11156;}
      else {
        if ((28 == (((unsigned long) next_op_667X)))) {
          next_669X = *((long *) (-3 + (ScontS)));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (-3 + (ScontS))) = (*((long *) (-3 + next_669X)));
            goto L26449;}
          else {
            ScontS = next_669X;
            goto L26449;}}
        else {
          if ((64 == nargs_673X)) {
            RSstackS = (4 + (RSstackS));
            merged_arg0K0 = (*((long *) (RSstackS)));
            merged_arg0K1 = 63;
            pop_args_list_return_tag = 5;
            goto pop_args_list;
           pop_args_list_return_5:
            v_666X = pop_args_list0_return_value;
            arg0K0 = v_666X;
            goto L26508;}
          else {
            merged_arg0K0 = 25;
            merged_arg0K1 = nargs_673X;
            pop_args_list_return_tag = 6;
            goto pop_args_list;
           pop_args_list_return_6:
            v_668X = pop_args_list0_return_value;
            arg0K0 = v_668X;
            goto L26508;}}}}
    else {
      goto L29326;}}
  else {
    goto L29326;}}
 L11009: {
  p_709X = arg0K0;
  i_710X = arg0K1;
  if ((2 == (3 & (*((long *) p_709X))))) {
    if ((54 == (*((long *) p_709X)))) {
      arg0K0 = (-1 + i_710X);
      goto L10979;}
    else {
      arg0K0 = i_710X;
      goto L10979;}}
  else {
    arg0K0 = (4 + p_709X);
    arg0K1 = (1 + i_710X);
    goto L11009;}}
 L25788: {
  if ((3 == (3 & x_336X))) {
    if ((18 == (31 & ((((*((long *) (-7 + x_336X))))>>2))))) {
      arg1K0 = 5;
      goto L25825;}
    else {
      goto L25796;}}
  else {
    goto L25796;}}
 L25661: {
  RSvalS = 1;
  goto L11156;}
 L25452: {
  if ((3 == (3 & x_338X))) {
    if ((18 == (31 & ((((*((long *) (-7 + x_338X))))>>2))))) {
      goto L25461;}
    else {
      goto L25464;}}
  else {
    goto L25464;}}
 L25243: {
  if ((3 == (3 & x_339X))) {
    if ((18 == (31 & ((((*((long *) (-7 + x_339X))))>>2))))) {
      goto L25252;}
    else {
      goto L25255;}}
  else {
    goto L25255;}}
 L25034: {
  if ((3 == (3 & x_340X))) {
    if ((18 == (31 & ((((*((long *) (-7 + x_340X))))>>2))))) {
      goto L25043;}
    else {
      goto L25046;}}
  else {
    goto L25046;}}
 L5388: {
  b_275X = arg0K0;
  lo_a_276X = 65535 & a_285X;
  lo_b_277X = 65535 & b_275X;
  hi_a_278X = 65535 & (((a_285X)>>16));
  hi_b_279X = 65535 & (((b_275X)>>16));
  lo_c_280X = SMALL_MULTIPLY(lo_a_276X, lo_b_277X);
  v_281X = SMALL_MULTIPLY(lo_b_277X, hi_a_278X);
  v_282X = SMALL_MULTIPLY(lo_a_276X, hi_b_279X);
  mid_c_283X = v_282X + v_281X;
  c_284X = lo_c_280X + (((mid_c_283X)<<16));
  if ((0 < hi_a_278X)) {
    if ((0 < hi_b_279X)) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 6;
      push_exception_continuationB_return_tag = 160;
      goto push_exception_continuationB;
     push_exception_continuationB_return_160:
      *((long *) (RSstackS)) = arg2_612X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = y_344X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12935;}
    else {
      goto L5430;}}
  else {
    goto L5430;}}
 L7144: {
  b_262X = arg0K0;
  c_263X = a_264X / b_262X;
  if ((a_348X < 0)) {
    if ((b_347X < 0)) {
      goto L7148;}
    else {
      goto L7147;}}
  else {
    if ((b_347X < 0)) {
      goto L7147;}
    else {
      goto L7148;}}}
 L28762: {
  b_28X = arg0K0;
  c_29X = a_30X / b_28X;
  if ((a_353X < 0)) {
    if ((b_352X < 0)) {
      goto L28766;}
    else {
      goto L28765;}}
  else {
    if ((b_352X < 0)) {
      goto L28765;}
    else {
      goto L28766;}}}
 L28631: {
  b_32X = arg0K0;
  c_33X = a_34X % b_32X;
  if ((a_355X < 0)) {
    arg0K0 = (0 - c_33X);
    goto L28635;}
  else {
    arg0K0 = c_33X;
    goto L28635;}}
 L22932: {
  RSvalS = new_93X;
  goto L11156;}
 L22916: {
  i_98X = arg0K0;
  if ((i_98X < 0)) {
    goto L22932;}
  else {
    RSstackS = (4 + (RSstackS));
    *((long *) ((-3 + new_93X) + (((i_98X)<<2)))) = (*((long *) (RSstackS)));
    arg0K0 = (-1 + i_98X);
    goto L22916;}}
 L22800: {
  i_103X = arg0K0;
  if ((i_103X < 0)) {
    RSvalS = new_102X;
    goto L11156;}
  else {
    RSstackS = (4 + (RSstackS));
    *((long *) ((-3 + new_102X) + (((i_103X)<<2)))) = (*((long *) (RSstackS)));
    arg0K0 = (-1 + i_103X);
    goto L22800;}}
 L22190: {
  i_104X = arg0K0;
  if ((i_104X < 0)) {
    RSvalS = val_110X;
    goto L11156;}
  else {
    addr_105X = (-3 + val_110X) + (((i_104X)<<2));
    WRITE_BARRIER(addr_105X, init_107X);
    *((long *) addr_105X) = init_107X;
    arg0K0 = (-1 + i_104X);
    goto L22190;}}
 L21272: {
  i_113X = arg0K0;
  if ((i_113X < 0)) {
    RSvalS = vector_112X;
    goto L11156;}
  else {
    *((unsigned char *) ((-3 + vector_112X) + i_113X)) = init_400X;
    arg0K0 = (-1 + i_113X);
    goto L21272;}}
 L20763: {
  i_39X = arg0K0;
  if ((i_39X < 0)) {
    RSvalS = string_38X;
    goto L11156;}
  else {
    *((unsigned char *) ((-3 + string_38X) + i_39X)) = (((unsigned long) init_563X));
    arg0K0 = (-1 + i_39X);
    goto L20763;}}
 L20146: {
  RSvalS = 13;
  goto L11156;}
 L19749: {
  if ((0 == (3 & arg2_132X))) {
    if (((((arg2_132X)>>2)) < 0)) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 161;
      goto push_exception_continuationB;
     push_exception_continuationB_return_161:
      *((long *) (RSstackS)) = arg2_132X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_133X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12935;}
    else {
      arg0K0 = (((arg2_132X)>>2));
      goto L19536;}}
  else {
    if ((3 == (3 & arg2_132X))) {
      if ((16 == (31 & ((((*((long *) (-7 + arg2_132X))))>>2))))) {
        if ((1 == mode_133X)) {
          goto L19594;}
        else {
          if ((3 == mode_133X)) {
            goto L19594;}
          else {
            v_127X = ps_open_fd((((unsigned char *)(-3 + arg2_132X))), 0, &v_126X);
            arg0K0 = v_127X;
            arg0K1 = v_126X;
            goto L19607;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 5;
        push_exception_continuationB_return_tag = 162;
        goto push_exception_continuationB;
       push_exception_continuationB_return_162:
        *((long *) (RSstackS)) = arg2_132X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (((mode_133X)<<2));
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 163;
      goto push_exception_continuationB;
     push_exception_continuationB_return_163:
      *((long *) (RSstackS)) = arg2_132X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_133X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12935;}}}
 L27145: {
  waitP_49X = arg2K0;
  start_51X = ((arg4_420X)>>2);
  count_50X = ((arg3_421X)>>2);
  if ((4 == (*((long *) (-3 + x_47X))))) {
    if ((3 == (3 & arg5_419X))) {
      if ((0 == (128 & (*((long *) (-7 + arg5_419X)))))) {
        if ((3 == (3 & arg5_419X))) {
          if ((16 == (31 & ((((*((long *) (-7 + arg5_419X))))>>2))))) {
            goto L19089;}
          else {
            goto L19081;}}
        else {
          goto L19081;}}
      else {
        arg1K0 = 5;
        goto L19018;}}
    else {
      arg1K0 = 5;
      goto L19018;}}
  else {
    arg1K0 = 5;
    goto L19018;}}
 L18851: {
  length_52X = arg0K0;
  if ((length_52X < (start_426X + count_425X))) {
    arg1K0 = 7;
    goto L18768;}
  else {
    got_53X = ps_write_fd(((((*((long *) (5 + channel_424X))))>>2)), ((-3 + arg4_550X) + start_426X), count_425X, &pendingP_54X, &status_55X);
    if ((status_55X == NO_ERRORS)) {
      if (pendingP_54X) {
        Spending_interruptPS = 0;
        Senabled_interruptsS = 0;
        arg1K0 = 13;
        goto L18768;}
      else {
        RSvalS = (((got_53X)<<2));
        goto L11156;}}
    else {
      arg1K0 = 21;
      goto L18768;}}}
 L18847: {
  arg0K0 = ((long)(((unsigned long)(*((long *) (-7 + arg4_550X))))>>8));
  goto L18851;}
 L6638: {
  val_268X = *((long *) (9 + ch_271X));
  addr_269X = 9 + prev_272X;
  WRITE_BARRIER(addr_269X, val_268X);
  *((long *) addr_269X) = val_268X;
  addr_270X = 9 + ch_271X;
  WRITE_BARRIER(addr_270X, 1);
  *((long *) addr_270X) = 1;
  arg0K0 = (*((long *) (13 + ch_271X)));
  goto L30392;}
 L18136: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 164;
  goto push_exception_continuationB;
 push_exception_continuationB_return_164:
  *((long *) (RSstackS)) = arg2_145X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_144X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L13938: {
  table_80X = arg0K0;
  symbols_81X = arg0K1;
  firstP_82X = arg2K2;
  arg0K0 = 0;
  goto L13942;}
 L17373: {
  minutesP_154X = arg2K0;
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      check_events_return_tag = 2;
      goto check_events;
     check_events_return_2:
      temp_155X = check_events0_return_value;
      if (temp_155X) {
        goto L17388;}
      else {
        goto L17412;}}
    else {
      goto L17388;}}
  else {
    goto L17412;}}
 L16543: {
  len_159X = 1 + n_163X;
  addr_160X = ALLOCATE_SPACE(16, (4 + len_159X));
  *((long *) addr_160X) = (66 + (((len_159X)<<8)));
  string_161X = 7 + addr_160X;
  *((unsigned char *) ((-3 + string_161X) + n_163X)) = 0;
  arg0K0 = arg2_162X;
  arg0K1 = (-1 + n_163X);
  goto L16480;}
 L16455: {
  if ((25 == arg2_162X)) {
    goto L16543;}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 5;
    push_exception_continuationB_return_tag = 165;
    goto push_exception_continuationB;
   push_exception_continuationB_return_165:
    *((long *) (RSstackS)) = arg2_162X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (((n_163X)<<2));
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}}
 L9819: {
  i_171X = arg0K0;
  h_172X = arg0K1;
  if ((i_171X < n_184X)) {
    arg0K0 = (1 + i_171X);
    arg0K1 = (h_172X + (((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + arg2_180X) + i_171X))))))));
    goto L9819;}
  else {
    index_174X = h_172X & (-1 + v_183X);
    bucket_173X = *((long *) ((-3 + table_182X) + (((index_174X)<<2))));
    arg0K0 = bucket_173X;
    goto L9782;}}
 L16312: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 166;
  goto push_exception_continuationB;
 push_exception_continuationB_return_166:
  *((long *) (RSstackS)) = arg2_180X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 2;
  arg1K1 = 0;
  goto L12935;}
 L14758: {
  if ((from_index_480X < 0)) {
    goto L14820;}
  else {
    if ((to_index_479X < 0)) {
      goto L14820;}
    else {
      if ((count_478X < 0)) {
        goto L14820;}
      else {
        if ((3 == (3 & arg5_511X))) {
          if ((16 == (31 & ((((*((long *) (-7 + arg5_511X))))>>2))))) {
            arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg5_511X))))>>8)));
            goto L14781;}
          else {
            goto L14777;}}
        else {
          goto L14777;}}}}}
 L14753: {
  if ((3 == (3 & arg5_511X))) {
    if ((17 == (31 & ((((*((long *) (-7 + arg5_511X))))>>2))))) {
      goto L14758;}
    else {
      goto L14820;}}
  else {
    goto L14820;}}
 L14820: {
  merged_arg1K0 = 0;
  merged_arg1K1 = 5;
  push_exception_continuationB_return_tag = 167;
  goto push_exception_continuationB;
 push_exception_continuationB_return_167:
  *((long *) (RSstackS)) = arg5_511X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((from_index_480X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = arg3_509X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((to_index_479X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((count_478X)<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 5;
  arg1K1 = 0;
  goto L12935;}
 L13015: {
  if (((Sstack_limitS) < (-296 + (RSstackS)))) {
    goto L13017;}
  else {
    space_681X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
    v_680X = AVAILABLEp(space_681X);
    if (v_680X) {
      arg1K0 = 0;
      goto L13100;}
    else {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 19;
      goto collect_saving_temp;
     collect_saving_temp_return_19:
      okayP_679X = AVAILABLEp(space_681X);
      if (okayP_679X) {
        arg1K0 = 0;
        goto L13100;}
      else {
        ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
        arg1K0 = 0;
        goto L13100;}}}}
 L13092: {
  merged_arg3K0 = ((unsigned char *) "exception handler is not a closure");
  loseD0_return_tag = 1;
  goto loseD0;
 loseD0_return_1:
  goto L13015;}
 L11755: {
  env_229X = RSenvS;
  v_230X = AVAILABLEp(3);
  if (v_230X) {
    goto L11714;}
  else {
    merged_arg0K0 = 0;
    collect_saving_temp_return_tag = 20;
    goto collect_saving_temp;
   collect_saving_temp_return_20:
    okayP_231X = AVAILABLEp(3);
    if (okayP_231X) {
      goto L11714;}
    else {
      ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
      goto L11714;}}}
 L11588: {
  v_243X = arg0K0;
  RSvalS = v_243X;
  goto L11156;}
 L11667: {
  env_238X = arg0K0;
  i_239X = arg0K1;
  if ((0 == i_239X)) {
    b_240X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    value_241X = *((long *) ((-3 + env_238X) + ((((((unsigned long) b_240X)))<<2))));
    addr_242X = (-3 + new_env_246X) + (((i_247X)<<2));
    WRITE_BARRIER(addr_242X, value_241X);
    *((long *) addr_242X) = value_241X;
    arg0K0 = (1 + i_247X);
    goto L11561;}
  else {
    arg0K0 = (*((long *) (-3 + env_238X)));
    arg0K1 = (-1 + i_239X);
    goto L11667;}}
 L11124: {
  i_202X = arg0K0;
  l_203X = arg0K1;
  if ((0 < i_202X)) {
    *((long *) (RSstackS)) = (*((long *) (-3 + l_203X)));
    RSstackS = (-4 + (RSstackS));
    arg0K0 = (-1 + i_202X);
    arg0K1 = (*((long *) (1 + l_203X)));
    goto L11124;}
  else {
    if ((0 == overflow_207X)) {
      SnargsS = nargs_212X;
      goto L13583;}
    else {
      *((long *) (RSstackS)) = l_203X;
      RSstackS = (-4 + (RSstackS));
      space_204X = 3 * overflow_207X;
      v_205X = AVAILABLEp(space_204X);
      if (v_205X) {
        goto L10761;}
      else {
        merged_arg0K0 = 0;
        collect_saving_temp_return_tag = 21;
        goto collect_saving_temp;
       collect_saving_temp_return_21:
        okayP_206X = AVAILABLEp(space_204X);
        if (okayP_206X) {
          goto L10761;}
        else {
          ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
          goto L10761;}}}}}
 L26449: {
  SnargsS = nargs_673X;
  RSvalS = (*((long *) (13 + cont_665X)));
  arg1K0 = 0;
  goto L12589;}
 L26508: {
  args_661X = arg0K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = 4;
  push_exception_continuationB_return_tag = 168;
  goto push_exception_continuationB;
 push_exception_continuationB_return_168:
  *((long *) (RSstackS)) = args_661X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L29326: {
  if ((64 == nargs_673X)) {
    RSstackS = (4 + (RSstackS));
    merged_arg0K0 = (*((long *) (RSstackS)));
    merged_arg0K1 = 63;
    pop_args_list_return_tag = 7;
    goto pop_args_list;
   pop_args_list_return_7:
    v_663X = pop_args_list0_return_value;
    arg0K0 = v_663X;
    goto L29355;}
  else {
    merged_arg0K0 = 25;
    merged_arg0K1 = nargs_673X;
    pop_args_list_return_tag = 8;
    goto pop_args_list;
   pop_args_list_return_8:
    v_662X = pop_args_list0_return_value;
    arg0K0 = v_662X;
    goto L29355;}}
 L10979: {
  args_on_stack_705X = arg0K0;
  if (((Sstack_limitS) < (-276 + (RSstackS)))) {
    arg0K0 = 0;
    goto L10983;}
  else {
    space_708X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
    v_707X = AVAILABLEp(space_708X);
    if (v_707X) {
      arg1K0 = 0;
      goto L11021;}
    else {
      merged_arg0K0 = 0;
      collect_saving_temp_return_tag = 22;
      goto collect_saving_temp;
     collect_saving_temp_return_22:
      okayP_706X = AVAILABLEp(space_708X);
      if (okayP_706X) {
        arg1K0 = 0;
        goto L11021;}
      else {
        ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
        arg1K0 = 0;
        goto L11021;}}}}
 L25796: {
  if ((3 == (3 & x_336X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_336X))))>>2))))) {
      arg1K0 = 5;
      goto L25825;}
    else {
      arg1K0 = 1;
      goto L25825;}}
  else {
    arg1K0 = 1;
    goto L25825;}}
 L25464: {
  if ((3 == (3 & x_338X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_338X))))>>2))))) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 169;
      goto push_exception_continuationB;
     push_exception_continuationB_return_169:
      *((long *) (RSstackS)) = x_338X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12935;}
    else {
      goto L25472;}}
  else {
    goto L25472;}}
 L25255: {
  if ((3 == (3 & x_339X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_339X))))>>2))))) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 170;
      goto push_exception_continuationB;
     push_exception_continuationB_return_170:
      *((long *) (RSstackS)) = x_339X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12935;}
    else {
      goto L25263;}}
  else {
    goto L25263;}}
 L25046: {
  if ((3 == (3 & x_340X))) {
    if ((10 == (31 & ((((*((long *) (-7 + x_340X))))>>2))))) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 5;
      push_exception_continuationB_return_tag = 171;
      goto push_exception_continuationB;
     push_exception_continuationB_return_171:
      *((long *) (RSstackS)) = x_340X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 1;
      arg1K1 = 0;
      goto L12935;}
    else {
      goto L25054;}}
  else {
    goto L25054;}}
 L5430: {
  if ((536870911 < lo_c_280X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 172;
    goto push_exception_continuationB;
   push_exception_continuationB_return_172:
    *((long *) (RSstackS)) = arg2_612X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_344X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}
  else {
    if ((lo_c_280X < 0)) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 6;
      push_exception_continuationB_return_tag = 173;
      goto push_exception_continuationB;
     push_exception_continuationB_return_173:
      *((long *) (RSstackS)) = arg2_612X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = y_344X;
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12935;}
    else {
      if ((8192 < mid_c_283X)) {
        merged_arg1K0 = 0;
        merged_arg1K1 = 6;
        push_exception_continuationB_return_tag = 174;
        goto push_exception_continuationB;
       push_exception_continuationB_return_174:
        *((long *) (RSstackS)) = arg2_612X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = y_344X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}
      else {
        if ((a_343X < 0)) {
          if ((b_342X < 0)) {
            goto L5457;}
          else {
            goto L5464;}}
        else {
          if ((b_342X < 0)) {
            goto L5464;}
          else {
            goto L5457;}}}}}}
 L7148: {
  if ((536870911 < c_263X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 175;
    goto push_exception_continuationB;
   push_exception_continuationB_return_175:
    *((long *) (RSstackS)) = arg2_610X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_349X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}
  else {
    arg0K0 = (((c_263X)<<2));
    goto L7122;}}
 L7147: {
  arg0K0 = ((((0 - c_263X))<<2));
  goto L7122;}
 L28766: {
  if ((536870911 < c_29X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 176;
    goto push_exception_continuationB;
   push_exception_continuationB_return_176:
    *((long *) (RSstackS)) = arg2_607X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = x_354X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}
  else {
    RSvalS = (((c_29X)<<2));
    goto L11156;}}
 L28765: {
  RSvalS = ((((0 - c_29X))<<2));
  goto L11156;}
 L28635: {
  n_31X = arg0K0;
  RSvalS = (((n_31X)<<2));
  goto L11156;}
 L19536: {
  index_136X = arg0K0;
  temp_137X = index_136X < (Snumber_of_channelsS);
  if (temp_137X) {
    goto L8690;}
  else {
    x_138X = 1 + index_136X;
    y_139X = 8 + (Snumber_of_channelsS);
    if ((x_138X < y_139X)) {
      arg0K0 = y_139X;
      goto L1927;}
    else {
      arg0K0 = x_138X;
      goto L1927;}}}
 L19594: {
  v_129X = ps_open_fd((((unsigned char *)(-3 + arg2_132X))), 1, &v_128X);
  arg0K0 = v_129X;
  arg0K1 = v_128X;
  goto L19607;}
 L19607: {
  channel_134X = arg0K0;
  status_135X = arg0K1;
  if ((status_135X == NO_ERRORS)) {
    arg0K0 = channel_134X;
    goto L19536;}
  else {
    if ((status_135X == ENOENT)) {
      merged_arg1K0 = 0;
      merged_arg1K1 = 10;
      push_exception_continuationB_return_tag = 177;
      goto push_exception_continuationB;
     push_exception_continuationB_return_177:
      *((long *) (RSstackS)) = arg2_132X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_133X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12935;}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = 21;
      push_exception_continuationB_return_tag = 178;
      goto push_exception_continuationB;
     push_exception_continuationB_return_178:
      *((long *) (RSstackS)) = arg2_132X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_133X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12935;}}}
 L19089: {
  if ((3 == (3 & arg5_419X))) {
    if ((16 == (31 & ((((*((long *) (-7 + arg5_419X))))>>2))))) {
      arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg5_419X))))>>8)));
      goto L19101;}
    else {
      goto L19097;}}
  else {
    goto L19097;}}
 L19081: {
  if ((3 == (3 & arg5_419X))) {
    if ((17 == (31 & ((((*((long *) (-7 + arg5_419X))))>>2))))) {
      goto L19089;}
    else {
      arg1K0 = 5;
      goto L19018;}}
  else {
    arg1K0 = 5;
    goto L19018;}}
 L19018: {
  reason_46X = arg1K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = reason_46X;
  push_exception_continuationB_return_tag = 179;
  goto push_exception_continuationB;
 push_exception_continuationB_return_179:
  *((long *) (RSstackS)) = arg5_419X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((start_51X)<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (((count_50X)<<2));
  RSstackS = (-4 + (RSstackS));
  if (waitP_49X) {
    arg1K0 = 5;
    goto L19033;}
  else {
    arg1K0 = 1;
    goto L19033;}}
 L13942: {
  i_74X = arg0K0;
  if ((i_74X == ((((3 + ((long)(((unsigned long)(*((long *) (-7 + symbols_81X))))>>8))))>>2)))) {
    RSvalS = 13;
    goto L11156;}
  else {
    v_79X = AVAILABLEp(3);
    if (v_79X) {
      obj_78X = *((long *) ((-3 + symbols_81X) + (((i_74X)<<2))));
      v_77X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + table_80X))))>>8))))>>2);
      s_76X = *((long *) (-3 + obj_78X));
      n_75X = -1 + ((long)(((unsigned long)(*((long *) (-7 + s_76X))))>>8));
      arg0K0 = 0;
      arg0K1 = 0;
      goto L10462;}
    else {
      if (firstP_82X) {
        arg0K0 = 1;
        goto L13963;}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = 8;
        push_exception_continuationB_return_tag = 180;
        goto push_exception_continuationB;
       push_exception_continuationB_return_180:
        *((long *) (RSstackS)) = table_80X;
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 1;
        arg1K1 = 0;
        goto L12935;}}}}
 L17388: {
  RSvalS = 13;
  goto L11156;}
 L17412: {
  if ((0 < (Spending_interruptsS))) {
    goto L17388;}
  else {
    wait_for_event((((arg2_453X)>>2)), minutesP_154X);
    goto L17388;}}
 L16480: {
  l_164X = arg0K0;
  i_165X = arg0K1;
  if ((i_165X < 0)) {
    RSvalS = string_161X;
    goto L11156;}
  else {
    *((unsigned char *) ((-3 + string_161X) + i_165X)) = (((unsigned long) ((((*((long *) (-3 + l_164X))))>>8))));
    arg0K0 = (*((long *) (1 + l_164X)));
    arg0K1 = (-1 + i_165X);
    goto L16480;}}
 L9782: {
  b_175X = arg0K0;
  if ((25 == b_175X)) {
    addr_178X = ALLOCATE_SPACE(1, 8);
    *((long *) addr_178X) = 1030;
    x_179X = 7 + addr_178X;
    *((long *) (-3 + x_179X)) = arg2_180X;
    if ((3 == (3 & x_179X))) {
      if ((0 == (128 & (*((long *) (-7 + x_179X)))))) {
        *((long *) (-7 + x_179X)) = (128 | (*((long *) (-7 + x_179X))));
        arg0K0 = x_179X;
        goto L9787;}
      else {
        arg0K0 = x_179X;
        goto L9787;}}
    else {
      arg0K0 = x_179X;
      goto L9787;}}
  else {
    s2_177X = *((long *) (-3 + (*((long *) (-3 + b_175X)))));
    len_176X = (long)(((unsigned long)(*((long *) (-7 + arg2_180X))))>>8);
    if ((len_176X == ((long)(((unsigned long)(*((long *) (-7 + s2_177X))))>>8)))) {
      if (((!memcmp((void *)(-3 + s2_177X), (void *)(-3 + arg2_180X),len_176X)))) {
        arg0K0 = (*((long *) (-3 + b_175X)));
        goto L16311;}
      else {
        goto L9800;}}
    else {
      goto L9800;}}}
 L14781: {
  y_191X = arg0K0;
  if ((y_191X < (from_index_480X + count_478X))) {
    goto L14820;}
  else {
    if ((3 == (3 & arg3_509X))) {
      if ((16 == (31 & ((((*((long *) (-7 + arg3_509X))))>>2))))) {
        arg0K0 = (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg3_509X))))>>8)));
        goto L14798;}
      else {
        goto L14794;}}
    else {
      goto L14794;}}}
 L14777: {
  arg0K0 = ((long)(((unsigned long)(*((long *) (-7 + arg5_511X))))>>8));
  goto L14781;}
 L13017: {
  if ((Sexception_space_usedPS)) {
    x_678X = (Sstack_limitS) < (-296 + (RSstackS));
    if (x_678X) {
      goto L13112;}
    else {
      ps_error(((unsigned char *) "no space on stack to reserve exception space"), 0);
      goto L13112;}}
  else {
    arg1K0 = 0;
    goto L12589;}}
 L13100: {
  v_677X = arg1K0;
  merged_arg1K0 = v_677X;
  copy_stack_into_heap_return_tag = 2;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_2:
  goto L13017;}
 L11714: {
  b_225X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  a_226X = *((long *) ((-3 + (StemplateS)) + ((((((unsigned long) b_225X)))<<2))));
  addr_227X = ALLOCATE_SPACE(3, 12);
  *((long *) addr_227X) = 2062;
  x_228X = 7 + addr_227X;
  *((long *) (-3 + x_228X)) = a_226X;
  *((long *) (1 + x_228X)) = env_229X;
  if ((3 == (3 & x_228X))) {
    if ((0 == (128 & (*((long *) (-7 + x_228X)))))) {
      *((long *) (-7 + x_228X)) = (128 | (*((long *) (-7 + x_228X))));
      arg0K0 = x_228X;
      goto L11721;}
    else {
      arg0K0 = x_228X;
      goto L11721;}}
  else {
    arg0K0 = x_228X;
    goto L11721;}}
 L13583: {
  RSvalS = proc_328X;
  arg1K0 = 0;
  goto L12589;}
 L10761: {
  RSstackS = (4 + (RSstackS));
  list_192X = *((long *) (RSstackS));
  a_193X = *((long *) (-3 + list_192X));
  addr_194X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_194X) = 2050;
  x_195X = 7 + addr_194X;
  *((long *) (-3 + x_195X)) = a_193X;
  *((long *) (1 + x_195X)) = 25;
  arg0K0 = (*((long *) (1 + list_192X)));
  arg0K1 = x_195X;
  goto L10778;}
 L29355: {
  args_664X = arg0K0;
  merged_arg1K0 = 0;
  merged_arg1K1 = 4;
  push_exception_continuationB_return_tag = 181;
  goto push_exception_continuationB;
 push_exception_continuationB_return_181:
  *((long *) (RSstackS)) = args_664X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 1;
  arg1K1 = 0;
  goto L12935;}
 L10983: {
  key_702X = arg0K0;
  *((long *) (RSstackS)) = (RSvalS);
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (StemplateS);
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((SnargsS))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
  RSstackS = (-4 + (RSstackS));
  tem_703X = Sinterrupt_templateS;
  StemplateS = tem_703X;
  RScode_pointerS = (-3 + (*((long *) (-3 + tem_703X))));
  merged_arg0K0 = (RScode_pointerS);
  merged_arg0K1 = (5 + args_on_stack_705X);
  merged_arg0K2 = key_702X;
  push_continuationB_return_tag = 1;
  goto push_continuationB;
 push_continuationB_return_1:
  n_704X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg0K0 = 0;
  arg0K1 = 1;
  goto L13233;}
 L11021: {
  v_701X = arg1K0;
  merged_arg1K0 = v_701X;
  copy_stack_into_heap_return_tag = 3;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_3:
  arg0K0 = 0;
  goto L10983;}
 L25472: {
  RSvalS = 1;
  goto L11156;}
 L25263: {
  RSvalS = 1;
  goto L11156;}
 L25054: {
  RSvalS = 1;
  goto L11156;}
 L5457: {
  if ((536870911 < c_284X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 182;
    goto push_exception_continuationB;
   push_exception_continuationB_return_182:
    *((long *) (RSstackS)) = arg2_612X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_344X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}
  else {
    RSvalS = (((c_284X)<<2));
    goto L11156;}}
 L5464: {
  if ((536870912 < c_284X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 183;
    goto push_exception_continuationB;
   push_exception_continuationB_return_183:
    *((long *) (RSstackS)) = arg2_612X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_344X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}
  else {
    RSvalS = ((((0 - c_284X))<<2));
    goto L11156;}}
 L7122: {
  q_265X = arg0K0;
  if ((0 == y_349X)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 184;
    goto push_exception_continuationB;
   push_exception_continuationB_return_184:
    *((long *) (RSstackS)) = arg2_610X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_349X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}
  else {
    a_266X = ((arg2_610X)>>2);
    if ((a_266X < 0)) {
      arg0K0 = (0 - a_266X);
      goto L7179;}
    else {
      arg0K0 = a_266X;
      goto L7179;}}}
 L8690: {
  if ((1 == (*((Svm_channelsS) + index_136X)))) {
    addr_124X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_124X) = 5142;
    x_125X = 7 + addr_124X;
    *((long *) (-3 + x_125X)) = (((mode_133X)<<2));
    *((long *) (1 + x_125X)) = arg2_132X;
    *((long *) (5 + x_125X)) = (((index_136X)<<2));
    *((long *) (9 + x_125X)) = 1;
    *((long *) (13 + x_125X)) = 1;
    *((Svm_channelsS) + index_136X) = x_125X;
    arg0K0 = x_125X;
    arg1K1 = 9;
    goto L19540;}
  else {
    arg0K0 = 1;
    arg1K1 = 11;
    goto L19540;}}
 L1927: {
  new_count_120X = arg0K0;
  new_vm_channels_121X = (long*)malloc(sizeof(long) * new_count_120X);
  if ((NULL == new_vm_channels_121X)) {
    arg0K0 = 1;
    arg1K1 = 9;
    goto L19540;}
  else {
    arg0K0 = 0;
    goto L1939;}}
 L19101: {
  length_41X = arg0K0;
  if ((length_41X < (start_51X + count_50X))) {
    arg1K0 = 7;
    goto L19018;}
  else {
    got_45X = ps_read_fd(((((*((long *) (5 + x_47X))))>>2)), ((-3 + arg5_419X) + start_51X), count_50X, waitP_49X, &eofP_44X, &pendingP_43X, &status_42X);
    if ((status_42X == NO_ERRORS)) {
      if (eofP_44X) {
        RSvalS = 21;
        goto L11156;}
      else {
        if (pendingP_43X) {
          Spending_interruptPS = 0;
          Senabled_interruptsS = 0;
          arg1K0 = 13;
          goto L19018;}
        else {
          RSvalS = (((got_45X)<<2));
          goto L11156;}}}
    else {
      arg1K0 = 21;
      goto L19018;}}}
 L19097: {
  arg0K0 = ((long)(((unsigned long)(*((long *) (-7 + arg5_419X))))>>8));
  goto L19101;}
 L19033: {
  v_40X = arg1K0;
  *((long *) (RSstackS)) = (((unsigned long) v_40X));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = x_47X;
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 5;
  arg1K1 = 0;
  goto L12935;}
 L10462: {
  i_60X = arg0K0;
  h_61X = arg0K1;
  if ((i_60X < n_75X)) {
    arg0K0 = (1 + i_60X);
    arg0K1 = (h_61X + (((unsigned long) (((unsigned long) (*((unsigned char *) ((-3 + s_76X) + i_60X))))))));
    goto L10462;}
  else {
    index_63X = h_61X & (-1 + v_77X);
    bucket_62X = *((long *) ((-3 + table_80X) + (((index_63X)<<2))));
    arg0K0 = bucket_62X;
    goto L10423;}}
 L13963: {
  i_68X = arg0K0;
  if ((i_68X == ((((3 + ((long)(((unsigned long)(*((long *) (-7 + table_80X))))>>8))))>>2)))) {
    addr_70X = -3 + table_80X;
    WRITE_BARRIER(addr_70X, symbols_81X);
    *((long *) addr_70X) = symbols_81X;
    merged_arg0K0 = table_80X;
    collect_saving_temp_return_tag = 23;
    goto collect_saving_temp;
   collect_saving_temp_return_23:
    table_71X = collect_saving_temp0_return_value;
    symbols_72X = *((long *) (-3 + table_71X));
    addr_73X = -3 + table_71X;
    WRITE_BARRIER(addr_73X, 25);
    *((long *) addr_73X) = 25;
    arg0K0 = table_71X;
    arg0K1 = symbols_72X;
    arg2K2 = 0;
    goto L13938;}
  else {
    addr_69X = (-3 + table_80X) + (((i_68X)<<2));
    WRITE_BARRIER(addr_69X, 25);
    *((long *) addr_69X) = 25;
    arg0K0 = (1 + i_68X);
    goto L13963;}}
 L9787: {
  new_167X = arg0K0;
  addr_168X = ALLOCATE_SPACE(0, 12);
  *((long *) addr_168X) = 2050;
  x_169X = 7 + addr_168X;
  *((long *) (-3 + x_169X)) = new_167X;
  *((long *) (1 + x_169X)) = bucket_173X;
  addr_170X = (-3 + table_182X) + (((index_174X)<<2));
  WRITE_BARRIER(addr_170X, x_169X);
  *((long *) addr_170X) = x_169X;
  arg0K0 = new_167X;
  goto L16311;}
 L16311: {
  val_166X = arg0K0;
  RSvalS = val_166X;
  goto L11156;}
 L9800: {
  arg0K0 = (*((long *) (1 + b_175X)));
  goto L9782;}
 L14798: {
  y_190X = arg0K0;
  if ((y_190X < (to_index_479X + count_478X))) {
    goto L14820;}
  else {
    memcpy((void *)((-3 + arg3_509X) + to_index_479X), (void *)((-3 + arg5_511X) + from_index_480X),count_478X);
    RSvalS = 13;
    goto L11156;}}
 L14794: {
  arg0K0 = ((long)(((unsigned long)(*((long *) (-7 + arg3_509X))))>>8));
  goto L14798;}
 L13112: {
  Sstack_limitS = (296 + (Sstack_beginS));
  Sexception_space_usedPS = 0;
  arg1K0 = 0;
  goto L12589;}
 L11721: {
  v_224X = arg0K0;
  RSvalS = v_224X;
  goto L11156;}
 L10778: {
  l_196X = arg0K0;
  last_197X = arg0K1;
  if ((25 == l_196X)) {
    *((long *) (RSstackS)) = x_195X;
    RSstackS = (-4 + (RSstackS));
    SnargsS = 64;
    goto L13583;}
  else {
    a_201X = *((long *) (-3 + l_196X));
    addr_200X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_200X) = 2050;
    x_199X = 7 + addr_200X;
    *((long *) (-3 + x_199X)) = a_201X;
    *((long *) (1 + x_199X)) = 25;
    addr_198X = 1 + last_197X;
    WRITE_BARRIER(addr_198X, x_199X);
    *((long *) addr_198X) = x_199X;
    arg0K0 = (*((long *) (1 + l_196X)));
    arg0K1 = x_199X;
    goto L10778;}}
 L13233: {
  i_711X = arg0K0;
  m_712X = arg0K1;
  if ((0 == (n_704X & m_712X))) {
    arg0K0 = (1 + i_711X);
    arg0K1 = (((m_712X)<<1));
    goto L13233;}
  else {
    Spending_interruptsS = ((Spending_interruptsS) & (~ m_712X));
    if ((i_711X == 0)) {
      *((long *) (RSstackS)) = (Sinterrupted_templateS);
      RSstackS = (-4 + (RSstackS));
      Sinterrupted_templateS = 1;
      *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      goto L12782;}
    else {
      if ((i_711X == 2)) {
        *((long *) (RSstackS)) = (Sfinalize_theseS);
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
        RSstackS = (-4 + (RSstackS));
        Sfinalize_theseS = 25;
        arg1K0 = 2;
        goto L12782;}
      else {
        if ((i_711X == 4)) {
          channel_713X = Spending_channels_headS;
          next_714X = *((long *) (9 + channel_713X));
          Spending_channels_headS = next_714X;
          addr_715X = 9 + channel_713X;
          WRITE_BARRIER(addr_715X, 1);
          *((long *) addr_715X) = 1;
          if ((1 == next_714X)) {
            Spending_channels_tailS = 1;
            arg0K0 = channel_713X;
            goto L6440;}
          else {
            arg0K0 = channel_713X;
            goto L6440;}}
        else {
          *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
          RSstackS = (-4 + (RSstackS));
          arg1K0 = 1;
          goto L12782;}}}}}
 L7179: {
  a_260X = arg0K0;
  n_261X = ((y_349X)>>2);
  if ((n_261X < 0)) {
    arg0K0 = (0 - n_261X);
    goto L7181;}
  else {
    arg0K0 = n_261X;
    goto L7181;}}
 L19540: {
  channel_117X = arg0K0;
  reason_118X = arg1K1;
  if ((1 == channel_117X)) {
    if ((3 == (3 & arg2_132X))) {
      if ((16 == (31 & ((((*((long *) (-7 + arg2_132X))))>>2))))) {
        if ((1 == mode_133X)) {
          goto L19685;}
        else {
          if ((3 == mode_133X)) {
            goto L19685;}
          else {
            v_119X = ps_close_fd(index_136X);
            arg0K0 = v_119X;
            goto L19680;}}}
      else {
        merged_arg1K0 = 0;
        merged_arg1K1 = reason_118X;
        push_exception_continuationB_return_tag = 185;
        goto push_exception_continuationB;
       push_exception_continuationB_return_185:
        *((long *) (RSstackS)) = arg2_132X;
        RSstackS = (-4 + (RSstackS));
        *((long *) (RSstackS)) = (((mode_133X)<<2));
        RSstackS = (-4 + (RSstackS));
        arg1K0 = 2;
        arg1K1 = 0;
        goto L12935;}}
    else {
      merged_arg1K0 = 0;
      merged_arg1K1 = reason_118X;
      push_exception_continuationB_return_tag = 186;
      goto push_exception_continuationB;
     push_exception_continuationB_return_186:
      *((long *) (RSstackS)) = arg2_132X;
      RSstackS = (-4 + (RSstackS));
      *((long *) (RSstackS)) = (((mode_133X)<<2));
      RSstackS = (-4 + (RSstackS));
      arg1K0 = 2;
      arg1K1 = 0;
      goto L12935;}}
  else {
    RSvalS = channel_117X;
    goto L11156;}}
 L1939: {
  i_123X = arg0K0;
  if ((i_123X == (Snumber_of_channelsS))) {
    arg0K0 = (Snumber_of_channelsS);
    goto L1954;}
  else {
    *(new_vm_channels_121X + i_123X) = (*((Svm_channelsS) + i_123X));
    arg0K0 = (1 + i_123X);
    goto L1939;}}
 L10423: {
  b_64X = arg0K0;
  if ((25 == b_64X)) {
    addr_65X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_65X) = 2050;
    x_66X = 7 + addr_65X;
    *((long *) (-3 + x_66X)) = obj_78X;
    *((long *) (1 + x_66X)) = bucket_62X;
    addr_67X = (-3 + table_80X) + (((index_63X)<<2));
    WRITE_BARRIER(addr_67X, x_66X);
    *((long *) addr_67X) = x_66X;
    goto L13957;}
  else {
    if ((obj_78X == (*((long *) (-3 + b_64X))))) {
      goto L13957;}
    else {
      arg0K0 = (*((long *) (1 + b_64X)));
      goto L10423;}}}
 L12782: {
  v_696X = arg1K0;
  SnargsS = (((unsigned long) v_696X));
  obj_697X = Sinterrupt_handlersS;
  if ((3 == (3 & obj_697X))) {
    if ((2 == (31 & ((((*((long *) (-7 + obj_697X))))>>2))))) {
      goto L12797;}
    else {
      goto L12825;}}
  else {
    goto L12825;}}
 L6440: {
  channel_698X = arg0K0;
  x_699X = 1 == (Spending_channels_headS);
  if (x_699X) {
    goto L6454;}
  else {
    Spending_interruptsS = (16 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        goto L6454;}
      else {
        goto L6454;}}
    else {
      Spending_interruptPS = 1;
      goto L6454;}}}
 L7181: {
  b_258X = arg0K0;
  c_259X = a_260X % b_258X;
  if ((a_266X < 0)) {
    arg0K0 = (0 - c_259X);
    goto L7185;}
  else {
    arg0K0 = c_259X;
    goto L7185;}}
 L19685: {
  v_130X = ps_close_fd(index_136X);
  arg0K0 = v_130X;
  goto L19680;}
 L19680: {
  status_131X = arg0K0;
  if ((status_131X == NO_ERRORS)) {
    merged_arg1K0 = 0;
    merged_arg1K1 = reason_118X;
    push_exception_continuationB_return_tag = 187;
    goto push_exception_continuationB;
   push_exception_continuationB_return_187:
    *((long *) (RSstackS)) = arg2_132X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (((mode_133X)<<2));
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}
  else {
    merged_arg0K0 = status_131X;
    merged_arg0K1 = index_136X;
    merged_arg0K2 = arg2_132X;
    channel_close_error_return_tag = 0;
    goto channel_close_error;
   channel_close_error_return_0:
    merged_arg1K0 = 0;
    merged_arg1K1 = reason_118X;
    push_exception_continuationB_return_tag = 188;
    goto push_exception_continuationB;
   push_exception_continuationB_return_188:
    *((long *) (RSstackS)) = arg2_132X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (((mode_133X)<<2));
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}}
 L1954: {
  i_122X = arg0K0;
  if ((i_122X == new_count_120X)) {
    free((Svm_channelsS));
    Svm_channelsS = new_vm_channels_121X;
    Snumber_of_channelsS = new_count_120X;
    goto L8690;}
  else {
    *(new_vm_channels_121X + i_122X) = 1;
    arg0K0 = (1 + i_122X);
    goto L1954;}}
 L13957: {
  arg0K0 = (1 + i_74X);
  goto L13942;}
 L12797: {
  Senabled_interruptsS = 0;
  if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
    Spending_interruptPS = 0;
    if ((Spending_eventsPS)) {
      Spending_interruptPS = 1;
      goto L12799;}
    else {
      goto L12799;}}
  else {
    Spending_interruptPS = 1;
    goto L12799;}}
 L12825: {
  ps_error(((unsigned char *) "interrupt handler is not a vector"), 0);
  goto L12797;}
 L6454: {
  *((long *) (RSstackS)) = channel_698X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (*((long *) (13 + channel_698X)));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
  RSstackS = (-4 + (RSstackS));
  arg1K0 = 3;
  goto L12782;}
 L7185: {
  n_257X = arg0K0;
  if ((0 == (((n_257X)<<2)))) {
    RSvalS = q_265X;
    goto L11156;}
  else {
    merged_arg1K0 = 0;
    merged_arg1K1 = 6;
    push_exception_continuationB_return_tag = 189;
    goto push_exception_continuationB;
   push_exception_continuationB_return_189:
    *((long *) (RSstackS)) = arg2_610X;
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = y_349X;
    RSstackS = (-4 + (RSstackS));
    arg1K0 = 2;
    arg1K1 = 0;
    goto L12935;}}
 L12799: {
  RSvalS = (*((long *) ((-3 + (Sinterrupt_handlersS)) + (((i_711X)<<2)))));
  obj_700X = RSvalS;
  if ((3 == (3 & obj_700X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_700X))))>>2))))) {
      arg1K0 = 0;
      goto L12589;}
    else {
      goto L12850;}}
  else {
    goto L12850;}}
 L12850: {
  ps_error(((unsigned char *) "interrupt handler is not a closure"), 1, i_711X);
  arg1K0 = 0;
  goto L12589;}
 loseD0: {
  message_846X = merged_arg3K0;{
  opcode_716X = (((*((long *) (8 + ((RSstackS) + ((((((unsigned long) nargs_683X)))<<2)))))))>>2);
  why_717X = (((*((long *) (4 + ((RSstackS) + ((((((unsigned long) nargs_683X)))<<2)))))))>>2);
  ps_write_string(((unsigned char *) "Template UIDs: "), (stderr));
  current_template_718X = StemplateS;
  out_719X = stderr;
  if ((0 == (3 & (*((long *) (1 + current_template_718X)))))) {
    if ((current_template_718X == (*((long *) (5 + (Sbottom_of_stackS)))))) {
      arg2K0 = 0;
      goto L8141;}
    else {
      ps_write_integer(((((*((long *) (1 + current_template_718X))))>>2)), out_719X);
      arg2K0 = 1;
      goto L8141;}}
  else {
    arg2K0 = 0;
    goto L8141;}}
 L8141: {
  not_firstP_721X = arg2K0;
  arg0K0 = (ScontS);
  arg2K1 = not_firstP_721X;
  goto L8148;}
 L8148: {
  cont_722X = arg0K0;
  not_firstP_723X = arg2K1;
  if ((3 == (3 & cont_722X))) {
    if ((9 == (31 & ((((*((long *) (-7 + cont_722X))))>>2))))) {
      template_724X = *((long *) (5 + cont_722X));
      if ((0 == (3 & (*((long *) (1 + template_724X)))))) {
        if ((template_724X == (*((long *) (5 + (Sbottom_of_stackS)))))) {
          arg2K0 = not_firstP_723X;
          goto L8157;}
        else {
          if (not_firstP_723X) {
            ps_write_string(((unsigned char *) " <- "), out_719X);
            goto L8196;}
          else {
            goto L8196;}}}
      else {
        arg2K0 = not_firstP_723X;
        goto L8157;}}
    else {
      goto L12953;}}
  else {
    goto L12953;}}
 L8157: {
  v_720X = arg2K0;
  arg0K0 = (*((long *) (-3 + cont_722X)));
  arg2K1 = v_720X;
  goto L8148;}
 L8196: {
  ps_write_integer(((((*((long *) (1 + template_724X))))>>2)), out_719X);
  arg2K0 = 1;
  goto L8157;}
 L12953: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  if ((why_717X == 1)) {
    if ((0 == (3 & (*((long *) (-3 + (*((long *) ((RSstackS) + ((((((unsigned long) nargs_683X)))<<2))))))))))) {
      ps_error(message_846X, 3, opcode_716X, why_717X, ((((*((long *) (-3 + (*((long *) ((RSstackS) + ((((((unsigned long) nargs_683X)))<<2)))))))))>>2)));
      goto loseD0_return;}
    else {
      goto L12977;}}
  else {
    goto L12977;}}
 L12977: {
  ps_error(message_846X, 2, opcode_716X, why_717X);
  goto loseD0_return;}
 loseD0_return:
  switch (loseD0_return_tag) {
  case 0: goto loseD0_return_0;
  default: goto loseD0_return_1;
  }}

 check_events: {
{ goto L31934;}
 L31934: {
  type_730X = get_next_event(&channel_729X, &status_728X);
  if ((type_730X == ALARM_EVENT)) {
    if ((1 == (Sinterrupted_templateS))) {
      Sinterrupted_templateS = (StemplateS);
      arg0K0 = 1;
      goto L12490;}
    else {
      arg0K0 = 1;
      goto L12490;}}
  else {
    if ((type_730X == KEYBOARD_INTERRUPT_EVENT)) {
      arg0K0 = 2;
      goto L12490;}
    else {
      if ((type_730X == IO_COMPLETION_EVENT)) {
        channel_727X = *((Svm_channelsS) + channel_729X);
        val_726X = ((status_728X)<<2);
        addr_725X = 13 + channel_727X;
        WRITE_BARRIER(addr_725X, val_726X);
        *((long *) addr_725X) = val_726X;
        if ((1 == (*((long *) (9 + channel_727X))))) {
          if ((1 == (Spending_channels_headS))) {
            Spending_channels_headS = channel_727X;
            Spending_channels_tailS = channel_727X;
            arg0K0 = 16;
            goto L12490;}
          else {
            addr_731X = 9 + (Spending_channels_tailS);
            WRITE_BARRIER(addr_731X, channel_727X);
            *((long *) addr_731X) = channel_727X;
            Spending_channels_tailS = channel_727X;
            arg0K0 = 16;
            goto L12490;}}
        else {
          arg0K0 = 16;
          goto L12490;}}
      else {
        if ((type_730X == NO_EVENT)) {
          arg0K0 = 0;
          goto L12490;}
        else {
          if ((type_730X == ERROR_EVENT)) {
            ps_write_string(((unsigned char *) "OS error while getting event"), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            ps_write_string((ps_error_string(status_728X)), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = 0;
            goto L12490;}
          else {
            ps_write_string(((unsigned char *) "unknown type of event"), (stderr));
            { long ignoreXX;
            PS_WRITE_CHAR(10, (stderr), ignoreXX) }
            arg0K0 = 0;
            goto L12490;}}}}}}
 L12490: {
  v_732X = arg0K0;
  Spending_interruptsS = (v_732X | (Spending_interruptsS));
  if ((type_730X == NO_EVENT)) {
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        goto L12517;}
      else {
        goto L12517;}}
    else {
      Spending_interruptPS = 1;
      goto L12517;}}
  else {
    goto L31934;}}
 L12517: {
  if ((Spending_interruptPS)) {
    if ((Spending_eventsPS)) {
      Spending_eventsPS = 0;
      goto L31934;}
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
  start_844X = merged_arg0K0;
  count_845X = merged_arg0K1;{
  space_733X = 3 * (SnargsS);
  v_734X = AVAILABLEp(space_733X);
  if (v_734X) {
    goto L10893;}
  else {
    merged_arg0K0 = 0;
    collect_saving_temp_return_tag = 24;
    goto collect_saving_temp;
   collect_saving_temp_return_24:
    okayP_735X = AVAILABLEp(space_733X);
    if (okayP_735X) {
      goto L10893;}
    else {
      ps_error(((unsigned char *) "Scheme48 heap overflow"), 0);
      goto L10893;}}}
 L10893: {
  arg0K0 = start_844X;
  arg0K1 = count_845X;
  goto L10900;}
 L10900: {
  args_736X = arg0K0;
  count_737X = arg0K1;
  if ((0 < count_737X)) {
    RSstackS = (4 + (RSstackS));
    a_738X = *((long *) (RSstackS));
    addr_739X = ALLOCATE_SPACE(0, 12);
    *((long *) addr_739X) = 2050;
    x_740X = 7 + addr_739X;
    *((long *) (-3 + x_740X)) = a_738X;
    *((long *) (1 + x_740X)) = args_736X;
    arg0K0 = x_740X;
    arg0K1 = (-1 + count_737X);
    goto L10900;}
  else {
    pop_args_list0_return_value = args_736X;
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
  value_843X = merged_arg0K0;{
  pc_741X = 12 + (((((RScode_pointerS) - (*((long *) (-3 + (StemplateS))))))<<2));
  begin_collection();
  v_742X = trace_value((StemplateS));
  StemplateS = v_742X;
  v_743X = trace_value((RSvalS));
  RSvalS = v_743X;
  v_744X = trace_value((Scurrent_threadS));
  Scurrent_threadS = v_744X;
  v_745X = trace_value((Ssession_dataS));
  Ssession_dataS = v_745X;
  v_746X = trace_value((Sexception_handlersS));
  Sexception_handlersS = v_746X;
  v_747X = trace_value((Sinterrupt_handlersS));
  Sinterrupt_handlersS = v_747X;
  v_748X = trace_value((Sinterrupt_templateS));
  Sinterrupt_templateS = v_748X;
  v_749X = trace_value((Sinterrupted_templateS));
  Sinterrupted_templateS = v_749X;
  v_750X = trace_value((Sfinalize_theseS));
  Sfinalize_theseS = v_750X;
  env_751X = trace_value((RSenvS));
  RSenvS = env_751X;
  v_752X = trace_value((Spending_channels_headS));
  Spending_channels_headS = v_752X;
  v_753X = trace_value((Spending_channels_tailS));
  Spending_channels_tailS = v_753X;
  arg0K0 = (4 + (RSstackS));
  arg0K1 = 0;
  goto L6925;}
 L6925: {
  p_773X = arg0K0;
  i_774X = arg0K1;
  if ((2 == (3 & (*((long *) p_773X))))) {
    if ((54 == (*((long *) p_773X)))) {
      arg0K0 = (-1 + i_774X);
      goto L6860;}
    else {
      arg0K0 = i_774X;
      goto L6860;}}
  else {
    arg0K0 = (4 + p_773X);
    arg0K1 = (1 + i_774X);
    goto L6925;}}
 L6860: {
  arg_count_770X = arg0K0;
  trace_locationsB((4 + (RSstackS)), (4 + ((RSstackS) + (((arg_count_770X)<<2)))));
  p_771X = RSenvS;
  if ((3 == (3 & p_771X))) {
    if ((p_771X < (Sstack_beginS))) {
      goto L6878;}
    else {
      if (((Sstack_endS) < p_771X)) {
        goto L6878;}
      else {
        env_772X = RSenvS;
        arg0K0 = env_772X;
        goto L7042;}}}
  else {
    goto L6878;}}
 L6878: {
  v_767X = trace_value((RSenvS));
  RSenvS = v_767X;
  goto L6883;}
 L7042: {
  env_768X = arg0K0;
  if ((3 == (3 & env_768X))) {
    if ((env_768X < (Sstack_beginS))) {
      goto L6883;}
    else {
      if (((Sstack_endS) < env_768X)) {
        goto L6883;}
      else {
        trace_stob_contentsB(env_768X);
        arg0K0 = (*((long *) (-3 + env_768X)));
        goto L7042;}}}
  else {
    goto L6883;}}
 L6883: {
  arg0K0 = (ScontS);
  arg0K1 = 0;
  goto L6888;}
 L6888: {
  cont_779X = arg0K0;
  last_env_780X = arg0K1;
  env_781X = *((long *) (9 + cont_779X));
  trace_stob_contentsB(cont_779X);
  if ((env_781X == last_env_780X)) {
    goto L6906;}
  else {
    arg0K0 = env_781X;
    goto L6997;}}
 L6906: {
  if ((cont_779X == (Sbottom_of_stackS))) {
    value_778X = trace_value(value_843X);
    okay_776X = do_gc((Sfinalizer_alistS), &sickly_777X);
    Sfinalizer_alistS = okay_776X;
    l2_775X = Sfinalize_theseS;
    if ((25 == sickly_777X)) {
      arg0K0 = l2_775X;
      goto L10345;}
    else {
      arg0K0 = sickly_777X;
      goto L10368;}}
  else {
    arg0K0 = (*((long *) (-3 + cont_779X)));
    arg0K1 = env_781X;
    goto L6888;}}
 L6997: {
  env_769X = arg0K0;
  if ((3 == (3 & env_769X))) {
    if ((env_769X < (Sstack_beginS))) {
      goto L6906;}
    else {
      if (((Sstack_endS) < env_769X)) {
        goto L6906;}
      else {
        trace_stob_contentsB(env_769X);
        arg0K0 = (*((long *) (-3 + env_769X)));
        goto L6997;}}}
  else {
    goto L6906;}}
 L10345: {
  v_782X = arg0K0;
  Sfinalize_theseS = v_782X;
  end_collection();
  arg0K0 = 0;
  goto L9979;}
 L10368: {
  l_783X = arg0K0;
  if ((25 == (*((long *) (1 + l_783X))))) {
    addr_784X = 1 + l_783X;
    WRITE_BARRIER(addr_784X, l2_775X);
    *((long *) addr_784X) = l2_775X;
    arg0K0 = sickly_777X;
    goto L10345;}
  else {
    arg0K0 = (*((long *) (1 + l_783X)));
    goto L10368;}}
 L9979: {
  i_762X = arg0K0;
  if ((i_762X == (Snumber_of_channelsS))) {
    tem_766X = StemplateS;
    StemplateS = tem_766X;
    RScode_pointerS = ((-3 + (*((long *) (-3 + tem_766X)))) + (((pc_741X)>>2)));
    Spending_interruptsS = (4 | (Spending_interruptsS));
    if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
      Spending_interruptPS = 0;
      if ((Spending_eventsPS)) {
        Spending_interruptPS = 1;
        collect_saving_temp0_return_value = value_778X;
        goto collect_saving_temp_return;}
      else {
        collect_saving_temp0_return_value = value_778X;
        goto collect_saving_temp_return;}}
    else {
      Spending_interruptPS = 1;
      collect_saving_temp0_return_value = value_778X;
      goto collect_saving_temp_return;}}
  else {
    channel_763X = *((Svm_channelsS) + i_762X);
    if ((1 == channel_763X)) {
      goto L10015;}
    else {
      header_764X = *((long *) (-7 + channel_763X));
      if ((3 == (3 & header_764X))) {
        arg0K0 = header_764X;
        goto L10008;}
      else {
        if ((0 == (*((long *) (-3 + channel_763X))))) {
          arg0K0 = 1;
          goto L10008;}
        else {
          id_765X = *((long *) (1 + channel_763X));
          if ((0 == (3 & id_765X))) {
            arg0K0 = id_765X;
            goto L9563;}
          else {
            if ((3 == (3 & (*((long *) (-7 + id_765X)))))) {
              arg0K0 = (*((long *) (-7 + id_765X)));
              goto L9563;}
            else {
              arg0K0 = id_765X;
              goto L9563;}}}}}}}
 L10015: {
  arg0K0 = (1 + i_762X);
  goto L9979;}
 L10008: {
  new_761X = arg0K0;
  *((Svm_channelsS) + i_762X) = new_761X;
  goto L10015;}
 L9563: {
  id_757X = arg0K0;
  os_index_758X = (((*((long *) (5 + channel_763X))))>>2);
  v_759X = *((long *) (-3 + channel_763X));
  if ((4 == v_759X)) {
    goto L9622;}
  else {
    if ((12 == (*((long *) (-3 + channel_763X))))) {
      goto L9622;}
    else {
      v_760X = ps_close_fd(os_index_758X);
      arg0K0 = v_760X;
      goto L9611;}}}
 L9622: {
  v_754X = ps_close_fd(os_index_758X);
  arg0K0 = v_754X;
  goto L9611;}
 L9611: {
  status_755X = arg0K0;
  *((Svm_channelsS) + os_index_758X) = 1;
  addr_756X = -3 + channel_763X;
  WRITE_BARRIER(addr_756X, 0);
  *((long *) addr_756X) = 0;
  if ((status_755X == NO_ERRORS)) {
    goto L9578;}
  else {
    merged_arg0K0 = status_755X;
    merged_arg0K1 = (*((long *) (5 + channel_763X)));
    merged_arg0K2 = id_757X;
    channel_close_error_return_tag = 1;
    goto channel_close_error;
   channel_close_error_return_1:
    goto L9578;}}
 L9578: {
  ps_write_string(((unsigned char *) "Channel closed: "), (stderr));
  if ((0 == (3 & id_757X))) {
    ps_write_integer((((id_757X)>>2)), (stderr));
    goto L9596;}
  else {
    ps_write_string((((unsigned char *)(-3 + id_757X))), (stderr));
    goto L9596;}}
 L9596: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  arg0K0 = 1;
  goto L10008;}
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
  status_840X = merged_arg0K0;
  index_841X = merged_arg0K1;
  id_842X = merged_arg0K2;{
  ps_write_string(((unsigned char *) "Error: "), (stderr));
  ps_write_string((ps_error_string(status_840X)), (stderr));
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  ps_write_string(((unsigned char *) " while closing port "), (stderr));
  if ((3 == (3 & id_842X))) {
    if ((16 == (31 & ((((*((long *) (-7 + id_842X))))>>2))))) {
      ps_write_string((((unsigned char *)(-3 + id_842X))), (stderr));
      goto L9035;}
    else {
      goto L9029;}}
  else {
    goto L9029;}}
 L9035: {
  PS_WRITE_CHAR(10, (stderr), v_785X)
  channel_close_error0_return_value = v_785X;
  goto channel_close_error_return;}
 L9029: {
  ps_write_integer((((index_841X)>>2)), (stderr));
  goto L9035;}
 channel_close_error_return:
  switch (channel_close_error_return_tag) {
  case 0: goto channel_close_error_return_0;
  default: goto channel_close_error_return_1;
  }}

 copy_stack_into_heap: {
  key_839X = merged_arg1K0;{
  arg0K0 = (4 + (RSstackS));
  arg0K1 = 0;
  goto L10589;}
 L10589: {
  p_791X = arg0K0;
  i_792X = arg0K1;
  if ((2 == (3 & (*((long *) p_791X))))) {
    if ((54 == (*((long *) p_791X)))) {
      arg0K0 = (-1 + i_792X);
      goto L10576;}
    else {
      arg0K0 = i_792X;
      goto L10576;}}
  else {
    arg0K0 = (4 + p_791X);
    arg0K1 = (1 + i_792X);
    goto L10589;}}
 L10576: {
  arg_count_789X = arg0K0;
  top_790X = RSstackS;
  if ((1 == (ScontS))) {
    goto L10581;}
  else {
    merged_arg1K0 = key_839X;
    merged_arg1K1 = 1;
    really_preserve_continuation_return_tag = 1;
    goto really_preserve_continuation;
   really_preserve_continuation_return_1:
    goto L10581;}}
 L10581: {
  stob_788X = ScontS;
  arg0K0 = (-11 + stob_788X);
  arg0K1 = (top_790X + (((arg_count_789X)<<2)));
  goto L10614;}
 L10614: {
  loc_786X = arg0K0;
  arg_787X = arg0K1;
  if ((top_790X < arg_787X)) {
    *((long *) loc_786X) = (*((long *) arg_787X));
    arg0K0 = (-4 + loc_786X);
    arg0K1 = (-4 + arg_787X);
    goto L10614;}
  else {
    RSstackS = loc_786X;
    goto copy_stack_into_heap_return;}}
 copy_stack_into_heap_return:
  switch (copy_stack_into_heap_return_tag) {
  case 0: goto copy_stack_into_heap_return_0;
  case 1: goto copy_stack_into_heap_return_1;
  case 2: goto copy_stack_into_heap_return_2;
  default: goto copy_stack_into_heap_return_3;
  }}

 really_preserve_continuation: {
  key_837X = merged_arg1K0;
  reason_838X = merged_arg1K1;{
  p_793X = RSenvS;
  if ((3 == (3 & p_793X))) {
    if ((p_793X < (Sstack_beginS))) {
      goto L10194;}
    else {
      if (((Sstack_endS) < p_793X)) {
        goto L10194;}
      else {
        merged_arg0K0 = (RSenvS);
        merged_arg0K1 = (ScontS);
        merged_arg1K2 = key_837X;
        merged_arg1K3 = reason_838X;
        save_env_in_heap_return_tag = 1;
        goto save_env_in_heap;
       save_env_in_heap_return_1:
        v_794X = save_env_in_heap0_return_value;
        RSenvS = v_794X;
        goto L10194;}}}
  else {
    goto L10194;}}
 L10194: {
  end_795X = *((long *) (-3 + (Sbottom_of_stackS)));
  arg0K0 = (ScontS);
  arg0K1 = (Sbottom_of_stackS);
  goto L10135;}
 L10135: {
  cont_800X = arg0K0;
  previous_801X = arg0K1;
  if ((cont_800X == (Sbottom_of_stackS))) {
    *((long *) (-3 + previous_801X)) = end_795X;
    cont_802X = *((long *) (-3 + (Sbottom_of_stackS)));
    if ((1 == cont_802X)) {
      ScontS = (Sbottom_of_stackS);
      really_preserve_continuation0_return_value = cont_802X;
      goto really_preserve_continuation_return;}
    else {
      merged_arg0K0 = cont_802X;
      copy_continuation_from_heapB_return_tag = 2;
      goto copy_continuation_from_heapB;
     copy_continuation_from_heapB_return_2:
      really_preserve_continuation0_return_value = cont_802X;
      goto really_preserve_continuation_return;}}
  else {
    p_803X = *((long *) (9 + cont_800X));
    if ((3 == (3 & p_803X))) {
      if ((p_803X < (Sstack_beginS))) {
        goto L10153;}
      else {
        if (((Sstack_endS) < p_803X)) {
          goto L10153;}
        else {
          merged_arg0K0 = (*((long *) (9 + cont_800X)));
          merged_arg0K1 = cont_800X;
          merged_arg1K2 = key_837X;
          merged_arg1K3 = reason_838X;
          save_env_in_heap_return_tag = 2;
          goto save_env_in_heap;
         save_env_in_heap_return_2:
          goto L10153;}}}
    else {
      goto L10153;}}}
 L10153: {
  header_796X = *((long *) (-7 + cont_800X));
  addr_797X = ALLOCATE_SPACE((31 & (((header_796X)>>2))), (4 + ((long)(((unsigned long)header_796X)>>8))));
  data_addr_798X = 4 + addr_797X;
  *((long *) addr_797X) = header_796X;
  memcpy((void *)data_addr_798X, (void *)(-3 + cont_800X),((long)(((unsigned long)header_796X)>>8)));
  new_799X = 3 + data_addr_798X;
  *((long *) (-3 + previous_801X)) = new_799X;
  arg0K0 = (*((long *) (-3 + new_799X)));
  arg0K1 = new_799X;
  goto L10135;}
 really_preserve_continuation_return:
  switch (really_preserve_continuation_return_tag) {
  case 0: goto really_preserve_continuation_return_0;
  default: goto really_preserve_continuation_return_1;
  }}

 save_env_in_heap: {
  env_835X = merged_arg0K0;
  cont_836X = merged_arg0K1;{
  header_804X = *((long *) (-7 + env_835X));
  addr_805X = ALLOCATE_SPACE((31 & (((header_804X)>>2))), (4 + ((long)(((unsigned long)header_804X)>>8))));
  data_addr_806X = 4 + addr_805X;
  *((long *) addr_805X) = header_804X;
  memcpy((void *)data_addr_806X, (void *)(-3 + env_835X),((long)(((unsigned long)header_804X)>>8)));
  new_807X = 3 + data_addr_806X;
  addr_808X = -3 + env_835X;
  WRITE_BARRIER(addr_808X, 54);
  *((long *) addr_808X) = 54;
  *((long *) (-7 + env_835X)) = new_807X;
  arg0K0 = new_807X;
  goto L9314;}
 L9314: {
  env_811X = arg0K0;
  p_812X = *((long *) (-3 + env_811X));
  if ((3 == (3 & p_812X))) {
    if ((p_812X < (Sstack_beginS))) {
      goto L9333;}
    else {
      if (((Sstack_endS) < p_812X)) {
        goto L9333;}
      else {
        env_813X = *((long *) (-3 + env_811X));
        header_814X = *((long *) (-7 + env_813X));
        addr_815X = ALLOCATE_SPACE((31 & (((header_814X)>>2))), (4 + ((long)(((unsigned long)header_814X)>>8))));
        data_addr_816X = 4 + addr_815X;
        *((long *) addr_815X) = header_814X;
        memcpy((void *)data_addr_816X, (void *)(-3 + env_813X),((long)(((unsigned long)header_814X)>>8)));
        new_817X = 3 + data_addr_816X;
        addr_818X = -3 + env_813X;
        WRITE_BARRIER(addr_818X, 54);
        *((long *) addr_818X) = 54;
        *((long *) (-7 + env_813X)) = new_817X;
        addr_819X = -3 + env_811X;
        WRITE_BARRIER(addr_819X, new_817X);
        *((long *) addr_819X) = new_817X;
        arg0K0 = new_817X;
        goto L9314;}}}
  else {
    goto L9333;}}
 L9333: {
  arg0K0 = cont_836X;
  goto L9337;}
 L9337: {
  cont_809X = arg0K0;
  env_810X = *((long *) (9 + cont_809X));
  if ((3 == (3 & env_810X))) {
    if ((3 == (3 & (*((long *) (-7 + env_810X)))))) {
      *((long *) (9 + cont_809X)) = (*((long *) (-7 + env_810X)));
      arg0K0 = (*((long *) (-3 + cont_809X)));
      goto L9337;}
    else {
      save_env_in_heap0_return_value = new_807X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap0_return_value = new_807X;
    goto save_env_in_heap_return;}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  default: goto save_env_in_heap_return_2;
  }}

 copy_continuation_from_heapB: {
  cont_834X = merged_arg0K0;{
  top_820X = (-11 + (Sbottom_of_stackS)) - (-4 & (3 + ((long)(((unsigned long)(*((long *) (-7 + cont_834X))))>>8))));
  new_cont_821X = 7 + top_820X;
  RSstackS = (-4 + top_820X);
  ScontS = new_cont_821X;
  v_822X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + cont_834X))))>>8))))>>2);
  memcpy((void *)top_820X, (void *)(-7 + cont_834X),(4 + (((v_822X)<<2))));
  *((long *) (-3 + (Sbottom_of_stackS))) = (*((long *) (-3 + new_cont_821X)));
  *((long *) (-3 + new_cont_821X)) = (Sbottom_of_stackS);
  copy_continuation_from_heapB0_return_value = new_cont_821X;
  goto copy_continuation_from_heapB_return;}
 copy_continuation_from_heapB_return:
  switch (copy_continuation_from_heapB_return_tag) {
  case 0: goto copy_continuation_from_heapB_return_0;
  case 1: goto copy_continuation_from_heapB_return_1;
  default: goto copy_continuation_from_heapB_return_2;
  }}

 push_exception_continuationB: {
  byte_args_832X = merged_arg1K0;
  exception_833X = merged_arg1K1;{
  if (((Sstack_limitS) < (-296 + (RSstackS)))) {
    arg0K0 = 0;
    goto L8472;}
  else {
    Sstack_limitS = (Sstack_beginS);
    Sexception_space_usedPS = 1;
    if (((Sstack_limitS) < (-296 + (RSstackS)))) {
      arg0K0 = 0;
      goto L8472;}
    else {
      ps_error(((unsigned char *) "insufficient space on stack for exception frame"), 0);
      arg0K0 = 0;
      goto L8472;}}}
 L8472: {
  key_826X = arg0K0;
  arg0K0 = (4 + (RSstackS));
  arg0K1 = 0;
  goto L8505;}
 L8505: {
  p_824X = arg0K0;
  i_825X = arg0K1;
  if ((2 == (3 & (*((long *) p_824X))))) {
    if ((54 == (*((long *) p_824X)))) {
      arg0K0 = (-1 + i_825X);
      goto L8476;}
    else {
      arg0K0 = i_825X;
      goto L8476;}}
  else {
    arg0K0 = (4 + p_824X);
    arg0K1 = (1 + i_825X);
    goto L8505;}}
 L8476: {
  v_823X = arg0K0;
  merged_arg0K0 = (RScode_pointerS);
  merged_arg0K1 = v_823X;
  merged_arg0K2 = key_826X;
  push_continuationB_return_tag = 2;
  goto push_continuationB;
 push_continuationB_return_2:
  *((long *) (RSstackS)) = ((((((unsigned long) (*((unsigned char *) (-1 + ((RScode_pointerS) - (((unsigned long) byte_args_832X)))))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = ((((((unsigned long) exception_833X)))<<2));
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
  code_pointer_830X = merged_arg0K0;
  size_831X = merged_arg0K1;{
  pc_827X = 12 + ((((code_pointer_830X - (*((long *) (-3 + (StemplateS))))))<<2));
  template_828X = StemplateS;
  RSstackS = (-20 + (RSstackS));
  *((long *) (4 + (RSstackS))) = (128 | (4134 + (((size_831X)<<10))));
  cont_829X = 11 + (RSstackS);
  *((long *) (1 + cont_829X)) = pc_827X;
  *((long *) (5 + cont_829X)) = template_828X;
  *((long *) (9 + cont_829X)) = (RSenvS);
  *((long *) (-3 + cont_829X)) = (ScontS);
  ScontS = cont_829X;
  goto push_continuationB_return;}
 push_continuationB_return:
  switch (push_continuationB_return_tag) {
  case 0: goto push_continuationB_return_0;
  case 1: goto push_continuationB_return_1;
  default: goto push_continuationB_return_2;
  }}
}

long call_startup_procedure(long startup_proc_847X, unsigned char **startup_vector_848X, unsigned char startup_vector_length_849X)
{
  long arg0K0;
  unsigned char *merged_arg3K2;
  unsigned char merged_arg1K1;
  long merged_arg0K0;

  int make_initial_channel_return_tag;
  long make_initial_channel0_return_value;
  long len_850X;
  long addr_851X;
  long vector_852X;
  long i_853X;
  long code_854X;
  long addr_855X;
  long output_856X;
  long v_857X;
  long error_858X;
  long v_859X;
  long input_860X;
  long v_861X;
  long v_862X;
  long v_863X;
  long v_864X;
  long y_865X;
  long x_866X;
  long y_867X;
  long x_868X;
  long *v_869X;
  long length_870X;
  long x_871X;
  long y_872X;
  long tem_873X;
  long i_874X;
  long addr_875X;
  long i_876X;
  unsigned char x_877X;
  unsigned char *string_878X;
  long z_879X;
  long len_880X;
  long addr_881X;
  long string_882X;
  long z_883X;
  long len_884X;
  long addr_885X;
  long string_886X;
  long i_887X;
  long x_888X;
  long addr_889X;
  long channel_890X;
  unsigned char status_891X;
  unsigned char *name_892X;
 {len_850X = (((((unsigned long) startup_vector_length_849X)))<<2);
  addr_851X = ALLOCATE_SPACE(2, (4 + len_850X));
  *((long *) addr_851X) = (10 + (((len_850X)<<8)));
  vector_852X = 7 + addr_851X;
  arg0K0 = 0;
  goto L12128;}
 L12128: {
  i_876X = arg0K0;
  if ((i_876X < (((unsigned long) startup_vector_length_849X)))) {
    string_878X = *(startup_vector_848X + i_876X);
    z_879X = strlen(string_878X);
    len_880X = 1 + z_879X;
    addr_881X = ALLOCATE_SPACE(16, (4 + len_880X));
    *((long *) addr_881X) = (66 + (((len_880X)<<8)));
    string_882X = 7 + addr_881X;
    *((unsigned char *) ((-3 + string_882X) + z_879X)) = 0;
    arg0K0 = 0;
    goto L12261;}
  else {
    SstackS = (-11 + (Sbottom_of_stackS));
    *((long *) (-3 + (Sbottom_of_stackS))) = 1;
    ScontS = (Sbottom_of_stackS);
    if ((Sexception_space_usedPS)) {
      x_877X = (Sstack_limitS) < (-296 + (SstackS));
      if (x_877X) {
        goto L7493;}
      else {
        ps_error(((unsigned char *) "no space on stack to reserve exception space"), 0);
        goto L7493;}}
    else {
      goto L7435;}}}
 L12261: {
  i_874X = arg0K0;
  if ((i_874X < z_879X)) {
    *((unsigned char *) ((-3 + string_882X) + i_874X)) = (((unsigned long) (*(string_878X + i_874X))));
    arg0K0 = (1 + i_874X);
    goto L12261;}
  else {
    addr_875X = (-3 + vector_852X) + (((i_876X)<<2));
    WRITE_BARRIER(addr_875X, string_882X);
    *((long *) addr_875X) = string_882X;
    arg0K0 = (1 + i_876X);
    goto L12128;}}
 L7493: {
  Sstack_limitS = (296 + (Sstack_beginS));
  Sexception_space_usedPS = 0;
  goto L7435;}
 L7435: {
  SenvS = 13;
  tem_873X = Sinterrupt_templateS;
  StemplateS = tem_873X;
  Scode_pointerS = (-3 + (*((long *) (-3 + tem_873X))));
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
  *((long *) (SstackS)) = vector_852X;
  SstackS = (-4 + (SstackS));
  y_872X = fileno((stderr));
  x_871X = fileno((stdout));
  if ((x_871X < y_872X)) {
    arg0K0 = y_872X;
    goto L9185;}
  else {
    arg0K0 = x_871X;
    goto L9185;}}
 L9185: {
  y_867X = arg0K0;
  x_868X = fileno((stdin));
  if ((x_868X < y_867X)) {
    arg0K0 = y_867X;
    goto L9187;}
  else {
    arg0K0 = x_868X;
    goto L9187;}}
 L9187: {
  v_864X = arg0K0;
  x_866X = Snumber_of_channelsS;
  y_865X = 1 + v_864X;
  if ((x_866X < y_865X)) {
    arg0K0 = y_865X;
    goto L9189;}
  else {
    arg0K0 = x_866X;
    goto L9189;}}
 L9189: {
  v_862X = arg0K0;
  Snumber_of_channelsS = v_862X;
  v_863X = fileno((stdin));
  Svm_channelsS = ((long*)malloc(sizeof(long) * (Snumber_of_channelsS)));
  Spending_channels_headS = 1;
  Spending_channels_tailS = 1;
  if ((NULL == (Svm_channelsS))) {
    ps_error(((unsigned char *) "out of memory, unable to continue"), 0);
    goto L9211;}
  else {
    goto L9211;}}
 L9211: {
  v_869X = Svm_channelsS;
  length_870X = Snumber_of_channelsS;
  arg0K0 = 0;
  goto L9253;}
 L9253: {
  i_853X = arg0K0;
  if ((i_853X < length_870X)) {
    *(v_869X + i_853X) = 1;
    arg0K0 = (1 + i_853X);
    goto L9253;}
  else {
    v_861X = fileno((stdin));
    merged_arg0K0 = v_861X;
    merged_arg1K1 = 4;
    merged_arg3K2 = ((unsigned char *) "standard input");
    make_initial_channel_return_tag = 0;
    goto make_initial_channel;
   make_initial_channel_return_0:
    input_860X = make_initial_channel0_return_value;
    v_859X = fileno((stderr));
    merged_arg0K0 = v_859X;
    merged_arg1K1 = 8;
    merged_arg3K2 = ((unsigned char *) "standard error");
    make_initial_channel_return_tag = 1;
    goto make_initial_channel;
   make_initial_channel_return_1:
    error_858X = make_initial_channel0_return_value;
    v_857X = fileno((stdout));
    merged_arg0K0 = v_857X;
    merged_arg1K1 = 8;
    merged_arg3K2 = ((unsigned char *) "standard output");
    make_initial_channel_return_tag = 2;
    goto make_initial_channel;
   make_initial_channel_return_2:
    output_856X = make_initial_channel0_return_value;
    *((long *) (SstackS)) = input_860X;
    SstackS = (-4 + (SstackS));
    *((long *) (SstackS)) = output_856X;
    SstackS = (-4 + (SstackS));
    *((long *) (SstackS)) = error_858X;
    SstackS = (-4 + (SstackS));
    addr_855X = ALLOCATE_SPACE(17, 6);
    *((long *) addr_855X) = 582;
    code_854X = 7 + addr_855X;
    *((unsigned char *) (-3 + code_854X)) = 24;
    *((unsigned char *) (-2 + code_854X)) = 4;
    Scode_pointerS = (-3 + code_854X);
    return restart(startup_proc_847X);}}
 make_initial_channel: {
  channel_890X = merged_arg0K0;
  status_891X = merged_arg1K1;
  name_892X = merged_arg3K2;{
  z_883X = strlen(name_892X);
  len_884X = 1 + z_883X;
  addr_885X = ALLOCATE_SPACE(16, (4 + len_884X));
  *((long *) addr_885X) = (66 + (((len_884X)<<8)));
  string_886X = 7 + addr_885X;
  *((unsigned char *) ((-3 + string_886X) + z_883X)) = 0;
  arg0K0 = 0;
  goto L8794;}
 L8794: {
  i_887X = arg0K0;
  if ((i_887X < z_883X)) {
    *((unsigned char *) ((-3 + string_886X) + i_887X)) = (((unsigned long) (*(name_892X + i_887X))));
    arg0K0 = (1 + i_887X);
    goto L8794;}
  else {
    addr_889X = ALLOCATE_SPACE(5, 24);
    *((long *) addr_889X) = 5142;
    x_888X = 7 + addr_889X;
    *((long *) (-3 + x_888X)) = (((unsigned long) status_891X));
    *((long *) (1 + x_888X)) = string_886X;
    *((long *) (5 + x_888X)) = (((channel_890X)<<2));
    *((long *) (9 + x_888X)) = 1;
    *((long *) (13 + x_888X)) = 1;
    *((Svm_channelsS) + channel_890X) = x_888X;
    make_initial_channel0_return_value = x_888X;
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
