
/* Manual edits:
     JAR 2/9/94 fix multiply
     JAR 2/10/94 turn make-immutable! into an identity function
     JAR 2/10/94 closure opcode makes immutable closures
 */

#include <stdio.h>
#include "prescheme.h"
long goto_arg1K0;
long goto_arg1K4;
long goto_arg1K3;
long goto_arg1K2;
long goto_arg1K1;
char goto_arg0K0;
long Sminimum_recovered_spaceS;
long Sinterrupt_templateS;
long Spending_interruptsS;
long Sdynamic_stateS;
long Sinterrupt_handlersS;
long Sexception_handlerS;
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
long *Sopen_vm_portsS;
FILE * *Sopen_portsS;
long Snumber_of_portsS;
long Sweak_pointer_limitS;
long Sweak_pointer_hpS;
long Ssaved_limitS;
long Ssaved_hpS;
long Sgc_countS;
long Soldspace_endS;
long Soldspace_beginS;
long Snewspace_endS;
long Snewspace_beginS;
long SlimitS;
long ShpS;
long Smemory_endS;
long Smemory_beginS;
long SmemoryS;
FILE * Scurrent_output_portS;
FILE * Scurrent_input_portS;

long Tinterpret()
{
  extern void really_write_number(long, FILE *);
  extern long copy_weak_pointer(long);
  extern long Sinterrupt_templateS;
  extern unsigned char Sexception_space_usedPS;
  extern long extended_vm(long, long);
  extern long ps_real_time();
  extern long ps_run_time();
  extern long ps_ticks_per_second();
  extern unsigned char lookup_external_name(long, long);
  extern long ps_schedule_interrupt(long);
  extern long Sinterrupt_handlersS;
  extern long Sexception_handlerS;
  extern long Sdynamic_stateS;
  extern FILE * Scurrent_output_portS;
  extern long Sgc_countS;
  extern long Sminimum_recovered_spaceS;
  extern long Sweak_pointer_hpS;
  extern void do_gc();
  extern long Snewspace_endS;
  extern long Ssaved_limitS;
  extern long Ssaved_hpS;
  extern long Snewspace_beginS;
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern FILE * *Sopen_portsS;
  extern long Snumber_of_portsS;
  extern long *Sopen_vm_portsS;
  extern long Senabled_interruptsS;
  extern long Spending_interruptsS;
  extern long Sbottom_of_stackS;
  extern long ScontS;
  extern long Sstack_beginS;
  extern long StemplateS;
  extern long SenvS;
  extern long Sstack_limitS;
  extern long SstackS;
  extern long Sstack_endS;
  extern long SlimitS;
  extern long ShpS;
  extern long SvalS;
  extern long Scode_pointerS;
  extern long SnargsS;
  extern long Tinterpret();
  unsigned char arg3K3;
  unsigned char arg3K0;
  FILE * arg2K0;
  char arg0K0;
  long arg1K2;
  long arg1K1;
  long arg1K0;
  char merged_arg0K0;
  char merged_arg0K1;
  char merged_arg0K3;
  FILE * merged_arg2K1;
  long merged_arg1K4;
  long merged_arg1K3;
  long merged_arg1K2;
  long merged_arg1K1;
  long merged_arg1K0;
  long RSenvS = SenvS;
  long RSstackS = SstackS;
  long RSvalS = SvalS;
  long RScode_pointerS = Scode_pointerS;

  int extract_string_return_tag;
  unsigned char *extract_string_return_value;
  int stob_equalP_return_tag;
  unsigned char stob_equalP_return_value;
  int write_number_return_tag;
  int close_port_return_tag;
  int close_port_noisily_return_tag;
  int copy_object_return_tag;
  long copy_object_return_value;
  int copy_next_return_tag;
  long copy_next_return_value;
  int collect_saving_temp_return_tag;
  long collect_saving_temp_return_value;
  int pop_args_list_return_tag;
  long pop_args_list_return_value;
  int push_continuationB_return_tag;
  int push_exception_continuationB_return_tag;
  int copy_stob_return_tag;
  long copy_stob_return_value;
  int save_env_in_heap_return_tag;
  long save_env_in_heap_return_value;
  int copy_continuation_from_heap_return_tag;
  long copy_continuation_from_heap_return_value;
  int preserve_continuation_return_tag;
  long preserve_continuation_return_value;
  int copy_stack_into_heap_return_tag;
  unsigned char b_0X;
  unsigned char b_1X;
  unsigned char okayP_2X;
  unsigned char v_3X;
  long space_4X;
  long v_5X;
  unsigned char b_6X;
  long env_7X;
  unsigned char b_8X;
  long location_9X;
  unsigned char b_10X;
  long location_11X;
  unsigned char b_12X;
  unsigned char b_13X;
  unsigned char okayP_14X;
  unsigned char v_15X;
  long space_16X;
  long v_17X;
  long cont_18X;
  long list_19X;
  long proc_20X;
  long v_21X;
  long cont_22X;
  long pc_23X;
  long tem_24X;
  long cont_25X;
  long tem_26X;
  unsigned char b_27X;
  long tem_28X;
  unsigned char b_29X;
  unsigned char b_30X;
  unsigned char b_31X;
  unsigned char b_32X;
  unsigned char b_33X;
  long val_34X;
  long v_35X;
  long v_36X;
  long v_37X;
  long v_38X;
  long v_39X;
  long z_40X;
  long x_41X;
  long arg2_42X;
  long arg2_43X;
  long arg2_44X;
  long x_45X;
  long arg2_46X;
  long arg2_47X;
  long arg2_48X;
  long arg2_49X;
  long x_50X;
  long arg2_51X;
  long arg2_52X;
  long arg2_53X;
  long arg2_54X;
  long arg2_55X;
  long x_56X;
  long result_57X;
  long x_58X;
  long value_59X;
  long count_60X;
  long x_61X;
  long x_62X;
  unsigned char okayP_63X;
  unsigned char v_64X;
  long space_65X;
  long arg2_66X;
  unsigned char b_67X;
  long init_68X;
  long len_69X;
  unsigned char v_70X;
  long temp_71X;
  unsigned char v_72X;
  long space_73X;
  long arg2_74X;
  long v_75X;
  long x_76X;
  unsigned char b_77X;
  long x_78X;
  long v_79X;
  unsigned char b_80X;
  unsigned char b_81X;
  long x_82X;
  long v_83X;
  long len_84X;
  long index_85X;
  long len_86X;
  long index_87X;
  long len_88X;
  long index_89X;
  long index_90X;
  long len_91X;
  long v_92X;
  unsigned char b_93X;
  long value_94X;
  long stob_95X;
  long arg2_96X;
  long index_97X;
  long len_98X;
  long v_99X;
  long descriptor_100X;
  long x_101X;
  long obj_102X;
  long v_103X;
  long x_104X;
  unsigned char okayP_105X;
  unsigned char v_106X;
  long v_107X;
  long v_108X;
  long v_109X;
  FILE * v_110X;
  unsigned char v_111X;
  long p_112X;
  unsigned char Kchar_113X;
  long c_114X;
  long x_115X;
  long v_116X;
  FILE * v_117X;
  unsigned char v_118X;
  long p_119X;
  FILE * port_120X;
  long v_121X;
  long arg2_122X;
  long count_123X;
  FILE * port_124X;
  long v_125X;
  long v_126X;
  unsigned char v_127X;
  long p_128X;
  unsigned char v_129X;
  long a_130X;
  long v_131X;
  long v_132X;
  long bytes_133X;
  long old_134X;
  long arg2_135X;
  long v_136X;
  long start_hp_137X;
  long type_138X;
  long obj_139X;
  long v_140X;
  long obj_141X;
  long tem_142X;
  unsigned char v_143X;
  long name_144X;
  long value_145X;
  long x_146X;
  long obj_147X;
  long v_148X;
  long n_149X;
  long n_150X;
  long arg2_151X;
  long return_value_152X;
  long key_153X;
  long x_154X;
  long v_155X;
  long v_156X;
  long n_157X;
  long x_158X;
  long obj_159X;
  unsigned char okayP_160X;
  unsigned char v_161X;
  unsigned char b_162X;
  long space_163X;
  unsigned char v_164X;
  unsigned char okayP_165X;
  long v_166X;
  long arg2_167X;
  long obj_168X;
  unsigned char x_169X;
  long arg2_170X;
  long arg2_171X;
  long x_172X;
  long key_173X;
  long n_174X;
  long index_175X;
  long proc_176X;
  long v_177X;
  long v_178X;
  long v_179X;
  long p_180X;
  long n_181X;
  long temp_182X;
  long v_183X;
  long v_184X;
  long obj_185X;
  long x_186X;
  long start_hp_187X;
  long x_188X;
  long key_189X;
  long n_190X;
  long n_191X;
  long arg2_192X;
  long arg3_193X;
  long obj_194X;
  long x_195X;
  unsigned char *spec_196X;
  FILE * port_197X;
  long count_198X;
  long b_199X;
  long e_200X;
  long v_201X;
  long v_202X;
  FILE * port_203X;
  long arg2_204X;
  long p_205X;
  unsigned char v_206X;
  long p_207X;
  unsigned char v_208X;
  long v_209X;
  long x_210X;
  long x_211X;
  long p_212X;
  unsigned char v_213X;
  FILE * v_214X;
  long v_215X;
  long x_216X;
  long c_217X;
  unsigned char Kchar_218X;
  long x_219X;
  unsigned char v_220X;
  long x_221X;
  unsigned char v_222X;
  long obj_223X;
  long x_224X;
  long arg2_225X;
  long x_226X;
  long v_227X;
  long arg2_228X;
  long stob_229X;
  long value_230X;
  unsigned char b_231X;
  long v_232X;
  long len_233X;
  long index_234X;
  long arg2_235X;
  long stob_236X;
  long value_237X;
  unsigned char b_238X;
  long arg2_239X;
  long index_240X;
  unsigned char b_241X;
  long v_242X;
  long v_243X;
  long arg2_244X;
  long index_245X;
  unsigned char b_246X;
  long v_247X;
  long x_248X;
  long arg2_249X;
  long index_250X;
  unsigned char b_251X;
  long v_252X;
  long arg2_253X;
  long x_254X;
  unsigned char b_255X;
  unsigned char b_256X;
  long v_257X;
  long x_258X;
  unsigned char b_259X;
  long x_260X;
  long v_261X;
  long x_262X;
  unsigned char b_263X;
  long len_264X;
  long init_265X;
  unsigned char b_266X;
  long arg2_267X;
  long space_268X;
  unsigned char v_269X;
  long temp_270X;
  unsigned char v_271X;
  long len_272X;
  long init_273X;
  unsigned char b_274X;
  long space_275X;
  unsigned char v_276X;
  long temp_277X;
  unsigned char v_278X;
  long x_279X;
  long arg2_280X;
  long x_281X;
  long arg2_282X;
  long x_283X;
  long x_284X;
  long arg2_285X;
  long x_286X;
  long x_287X;
  long x_288X;
  long a_289X;
  long b_290X;
  long x_291X;
  long a_292X;
  long b_293X;
  long x_294X;
  long arg2_295X;
  long x_296X;
  long a_297X;
  long b_298X;
  long x_299X;
  long z_300X;
  long x_301X;
  long a_302X;
  long b_303X;
  long arg2_304X;
  long x_305X;
  long x_306X;
  long x_307X;
  long x_308X;
  long x_309X;
  long arg2_310X;
  long x_311X;
  unsigned char b_312X;
  unsigned char b_313X;
  unsigned char b_314X;
  unsigned char b_315X;
  long consumer_316X;
  long start_arg_317X;
  long start_loc_318X;
  unsigned char b_319X;
  unsigned char b_320X;
  unsigned char b_321X;
  unsigned char x_322X;
  long space_323X;
  unsigned char v_324X;
  unsigned char okayP_325X;
  unsigned char b_326X;
  long p_327X;
  long v_328X;
  long env_329X;
  unsigned char b_330X;
  long env_331X;
  unsigned char b_332X;
  long env_333X;
  unsigned char b_334X;
  unsigned char b_335X;
  long env_336X;
  unsigned char b_337X;
  long v_338X;
  unsigned char count_339X;
  unsigned char x_340X;
  long space_341X;
  unsigned char v_342X;
  unsigned char okayP_343X;
  long vector_344X;
  long addr_345X;
  unsigned char v_346X;
  long d_347X;
  long h_348X;
  long addr_349X;
  long h_350X;
  long d_351X;
  long val_352X;
  long i_353X;
  long h_354X;
  long bucket_355X;
  long index_356X;
  long v_357X;
  long b_358X;
  unsigned char v_359X;
  long new_360X;
  long new_361X;
  long i_362X;
  long h_363X;
  long bucket_364X;
  long index_365X;
  long v_366X;
  long b_367X;
  long new_368X;
  long stob_369X;
  long from_370X;
  long from_371X;
  long to_372X;
  long v_373X;
  long v_374X;
  long arg2_375X;
  long obj_376X;
  long table_377X;
  long v_378X;
  long n_379X;
  long v_380X;
  long arg2_381X;
  long n_382X;
  long new_383X;
  long obj_384X;
  long l_385X;
  long i_386X;
  long i_387X;
  long h_388X;
  long v_389X;
  char v_390X;
  long addr_391X;
  unsigned char v_392X;
  long d_393X;
  long obj_394X;
  long v_395X;
  long s_396X;
  long n_397X;
  long resume_proc_398X;
  long new_399X;
  long i_400X;
  long header_401X;
  long port_402X;
  long count_403X;
  long count_404X;
  long size_405X;
  long b_406X;
  long e_407X;
  char v_408X;
  long val_409X;
  long c_410X;
  long val_411X;
  long key_412X;
  long arg2_413X;
  long v_414X;
  long mode_415X;
  long i_416X;
  long x_417X;
  long i_418X;
  long x_419X;
  long i_420X;
  long i_421X;
  long index_422X;
  long new_count_423X;
  long old_count_424X;
  FILE * *new_ports_425X;
  long *new_vm_ports_426X;
  FILE * port_427X;
  long new_428X;
  long index_429X;
  long filename_430X;
  unsigned char *spec_431X;
  unsigned char *spec_432X;
  long filename_433X;
  char v_434X;
  long val_435X;
  long i_436X;
  unsigned char okayP_437X;
  long init_438X;
  long new_439X;
  long val_440X;
  long i_441X;
  unsigned char okayP_442X;
  long init_443X;
  long new_444X;
  long val_445X;
  long i_446X;
  unsigned char okayP_447X;
  long init_448X;
  long new_449X;
  long len_450X;
  long new_451X;
  long len_452X;
  unsigned char b_453X;
  unsigned char b_454X;
  long new_455X;
  long i_456X;
  long n_457X;
  long b_458X;
  long c_459X;
  long a_460X;
  long b_461X;
  long c_462X;
  long a_463X;
  long n_464X;
  long b_465X;
  long c_466X;
  long a_467X;
  long q_468X;
  long a_469X;
  long b_470X;
  long b_471X;
  long c_472X;
  long a_473X;
  long b_474X;
  long lo_a_475X;
  long lo_b_476X;
  long hi_a_477X;
  long hi_b_478X;
  long lo_c_479X;
  long mid_c_480X;
  long c_481X;
  long a_482X;
  long offset_483X;
  long list_484X;
  long a_485X;
  long new_486X;
  long l_487X;
  long last_488X;
  long new_489X;
  long a_490X;
  long i_491X;
  long l_492X;
  long space_493X;
  unsigned char v_494X;
  unsigned char okayP_495X;
  long overflow_496X;
  long args_497X;
  long fast_498X;
  long len_499X;
  long slow_500X;
  unsigned char move_slowP_501X;
  long nargs_502X;
  long args_503X;
  long v_504X;
  long args_505X;
  long loc_506X;
  long arg_507X;
  long v_508X;
  long key_509X;
  long v_510X;
  long space_511X;
  unsigned char v_512X;
  unsigned char okayP_513X;
  long key_514X;
  long p_515X;
  long v_516X;
  long env_517X;
  unsigned char v_518X;
  unsigned char okayP_519X;
  unsigned char b_520X;
  long a_521X;
  long new_522X;
  long env_523X;
  long i_524X;
  unsigned char b_525X;
  long env_526X;
  long i_527X;
  unsigned char b_528X;
  long args_529X;
  long new_530X;
  long len_531X;
  unsigned char b_532X;
  unsigned char b_533X;
  long v_534X;
  char v_535X;
  char v_536X;
  char v_537X;
  char v_538X;
  char v_539X;
  char v_540X;
  char v_541X;
  char v_542X;
  long r_543X;
  char v_544X;
  char v_545X;
  char v_546X;
  char v_547X;
  long v_548X;
  long key_549X;
  long v_550X;
  long cont_551X;
  long tem_552X;
  long pc_553X;
  long args_554X;
  long v_555X;
  long v_556X;
  long cont_557X;
  long v_558X;
  long v_559X;
  long op_560X;
  long v_561X;
  long next_562X;
  long cont_563X;
  long tem_564X;
  long pc_565X;
  long args_566X;
  long v_567X;
  long v_568X;
  long args_569X;
  long a1_570X;
  long v_571X;
  long obj_572X;
  unsigned char okayP_573X;
  unsigned char v_574X;
  long space_575X;
  unsigned char x_576X;
  long v_577X;
  unsigned char x_578X;
  long tem_579X;
  long obj_580X;
  long v_581X;
  unsigned char okayP_582X;
  unsigned char v_583X;
  long space_584X;
  unsigned char x_585X;
  long v_586X;
  long obj_587X;
  long i_588X;
  long m_589X;
  long obj_590X;
  long tem_591X;
  long v_592X;
  long v_593X;
  long key_594X;
  long n_595X;
  long i_596X;
  long i_597X;
  long vec_598X;
  long p_599X;
  long i_600X;
  long new_601X;
  long len_602X;
  long p_603X;
  long v_604X;
  long cont_605X;
  long previous_606X;
  long v_607X;
  long cont_608X;
  long p_609X;
  long new_610X;
  long v_611X;
  long end_612X;
  long cells_613X;
  long top_614X;
  long new_cont_615X;
  long v_616X;
  long from_617X;
  long from_618X;
  long to_619X;
  long new_620X;
  long env_621X;
  long p_622X;
  long env_623X;
  long new_624X;
  long cont_625X;
  long env_626X;
  long new_627X;
  long h_628X;
  long len_629X;
  long from_630X;
  long to_631X;
  long new_632X;
  long bytes_633X;
  long from_634X;
  unsigned char x_635X;
  unsigned char x_636X;
  long p_637X;
  long i_638X;
  long key_639X;
  long pc_640X;
  long template_641X;
  long cont_642X;
  long space_643X;
  unsigned char v_644X;
  unsigned char okayP_645X;
  long args_646X;
  long count_647X;
  long a_648X;
  long new_649X;
  long v_650X;
  long value_651X;
  long pc_652X;
  long e_653X;
  long b_654X;
  long a_655X;
  unsigned char v_656X;
  long i_657X;
  unsigned char x_658X;
  long port_659X;
  long header_660X;
  long new_661X;
  long value_662X;
  long cont_663X;
  long last_env_664X;
  long env_665X;
  long byte_count_666X;
  long v_667X;
  long a_668X;
  unsigned char v_669X;
  long env_670X;
  long byte_count_671X;
  long addr_672X;
  long v_673X;
  long addr_674X;
  long v_675X;
  long addr_676X;
  long v_677X;
  long env_678X;
  long start_679X;
  long end_680X;
  long v_681X;
  long v_682X;
  long value_683X;
  long a_684X;
  unsigned char v_685X;
  long v_686X;
  long v_687X;
  long value_688X;
  long a_689X;
  unsigned char v_690X;
  long v_691X;
  long v_692X;
  long value_693X;
  long a_694X;
  unsigned char v_695X;
  long v_696X;
  long v_697X;
  long value_698X;
  long a_699X;
  unsigned char v_700X;
  long v_701X;
  long v_702X;
  long value_703X;
  long a_704X;
  unsigned char v_705X;
  long tem_706X;
  long v_707X;
  long value_708X;
  long a_709X;
  unsigned char v_710X;
  long next_711X;
  long thing_712X;
  long value_713X;
  long a_714X;
  unsigned char v_715X;
  long v_716X;
  long h_717X;
  long new_718X;
  long x1_719X;
  long o_720X;
  long header_721X;
  long string_722X;
  FILE * port_723X;
  long count_724X;
  FILE * port_725X;
  long mode_726X;
  long index_727X;

  long v_728X;
  long z1_729X;
  long z2_730X;
  long i_731X;
  long z_732X;
  unsigned char *v_733X;
  long i_734X;
  long v_735X;
  unsigned char *c_736X;
  long string_737X;
  unsigned char c_738X;
  long stob1_739X;
  long stob2_740X;
  long x_741X;
  FILE * port_742X;
  long vm_port_743X;
  long port_744X;
  long c_745X;
  long thing_746X;
  long c_747X;
  long addr_748X;
  long c_749X;
  long value_750X;
  long c_751X;
  long start_752X;
  long count_753X;
  long code_pointer_754X;
  long size_755X;
  long c_756X;
  long stob_757X;
  long c_758X;
  long env_759X;
  long cont_760X;
  long key_761X;
  long c_762X;
  long cont_763X;
  long c_764X;
  long key_765X;
  char reason_766X;
  long key_767X;
  char c_768X;
  char c_769X;
  char bytes_consumed_770X;
  char c_771X;
  char nargs_772X;
  char c_773X;
  char byte_args_774X;
  char c_775X;
  char args_776X;
  long a1_777X;
  long a2_778X;
  long a3_779X;
  long a4_780X;
  char c_781X;
  char args_782X;
  long a1_783X;
  long a2_784X;
  long a3_785X;
  char c_786X;
  char args_787X;
  long a1_788X;
  long a2_789X;
  char c_790X;
  char args_791X;
  long a1_792X;
  char c_793X;
  long nargs_794X;
  char c_795X;
  char args_796X;
 START: {
  b_162X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  switch (b_162X) {
    case 0 : {
      b_0X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((SnargsS == ((long)b_0X))) {
        goto START;}
      else {
        merged_arg0K0 = 1;
        goto application_exception;}}
      break;
    case 1 : {
      b_1X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((SnargsS < ((long)b_1X))) {
        merged_arg0K0 = 1;
        goto application_exception;}
      else {
        goto START;}}
      break;
    case 2 : {
      RSvalS = (SnargsS << 2);
      goto START;}
      break;
    case 3 : {
      count_339X = *((unsigned char*)RScode_pointerS);
      x_340X = Sstack_limitS < (-8 + (RSstackS - (((long)count_339X) << 2)));
      if (x_340X) {
        goto L27529;}
      else {
        space_341X = 1 + ((Sstack_endS - RSstackS) >> 2);
        v_342X = (ShpS + (space_341X << 2)) < SlimitS;
        if (v_342X) {
          arg1K0 = 0;
          goto L27566;}
        else {
          merged_arg1K0 = 0;
          collect_saving_temp_return_tag = 0;
          goto collect_saving_temp;
         collect_saving_temp_return_0:
          okayP_343X = (ShpS + (space_341X << 2)) < SlimitS;
          if (okayP_343X) {
            arg1K0 = 0;
            goto L27566;}
          else {
            TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
            arg1K0 = 0;
            goto L27566;}}}}
      break;
    case 4 : {
      space_4X = 2 + ((long)(*((unsigned char*)RScode_pointerS)));
      v_3X = (ShpS + (space_4X << 2)) < SlimitS;
      if (v_3X) {
        goto L27603;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 1;
        goto collect_saving_temp;
       collect_saving_temp_return_1:
        okayP_2X = (ShpS + (space_4X << 2)) < SlimitS;
        if (okayP_2X) {
          goto L27603;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L27603;}}}
      break;
    case 5 : {
      RSenvS = (*((long*)(-3 + RSenvS)));
      goto START;}
      break;
    case 6 : {
      b_337X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((64 == SnargsS)) {
        RSstackS = (4 + RSstackS);
        merged_arg1K0 = (*((long*)RSstackS));
        merged_arg1K1 = ((-1 + SnargsS) - ((long)b_337X));
        pop_args_list_return_tag = 0;
        goto pop_args_list;
       pop_args_list_return_0:
        v_5X = pop_args_list_return_value;
        arg1K0 = v_5X;
        goto L27675;}
      else {
        merged_arg1K0 = 25;
        merged_arg1K1 = (SnargsS - ((long)b_337X));
        pop_args_list_return_tag = 1;
        goto pop_args_list;
       pop_args_list_return_1:
        v_338X = pop_args_list_return_value;
        arg1K0 = v_338X;
        goto L27675;}}
      break;
    case 7 : {
      b_6X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((-3 + StemplateS) + (((long)b_6X) << 2))));
      goto START;}
      break;
    case 8 : {
      b_335X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      env_336X = RSenvS;
      arg1K0 = env_336X;
      arg1K1 = ((long)b_335X);
      goto L27429;}
      break;
    case 9 : {
      env_333X = RSenvS;
      b_334X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((-3 + env_333X) + (((long)b_334X) << 2))));
      if ((529 == RSvalS)) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 10 : {
      env_331X = *((long*)(-3 + RSenvS));
      b_332X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((-3 + env_331X) + (((long)b_332X) << 2))));
      if ((529 == RSvalS)) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 11 : {
      env_329X = *((long*)(-3 + (*((long*)(-3 + RSenvS)))));
      b_330X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((-3 + env_329X) + (((long)b_330X) << 2))));
      if ((529 == RSvalS)) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 12 : {
      b_8X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      env_7X = RSenvS;
      arg1K0 = env_7X;
      arg1K1 = ((long)b_8X);
      goto L27747;}
      break;
    case 13 : {
      b_10X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      location_9X = *((long*)((-3 + StemplateS) + (((long)b_10X) << 2)));
      RSvalS = (*((long*)(-3 + location_9X)));
      if ((17 == (255 & RSvalS))) {
        merged_arg0K0 = 1;
        merged_arg1K1 = location_9X;
        goto raise_exception1;}
      else {
        goto START;}}
      break;
    case 14 : {
      b_12X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      location_11X = *((long*)((-3 + StemplateS) + (((long)b_12X) << 2)));
      if ((273 == (*((long*)(-3 + location_11X))))) {
        merged_arg0K0 = 1;
        merged_arg1K1 = location_11X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}
      else {
        *((long*)(-3 + location_11X)) = RSvalS;
        RSvalS = 13;
        goto START;}}
      break;
    case 15 : {
      p_327X = RSenvS;
      if ((p_327X < Sstack_beginS)) {
        arg1K0 = 0;
        goto L27845;}
      else {
        if ((Sstack_endS < p_327X)) {
          arg1K0 = 0;
          goto L27845;}
        else {
          v_328X = Sstack_endS - Sstack_beginS;
          arg1K0 = v_328X;
          goto L27845;}}}
      break;
    case 16 : {
      *((long*)RSstackS) = RSvalS;
      RSstackS = (-4 + RSstackS);
      goto START;}
      break;
    case 17 : {
      RSstackS = (4 + RSstackS);
      RSvalS = (*((long*)RSstackS));
      goto START;}
      break;
    case 18 : {
      b_13X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)(4 + (RSstackS + (((long)b_13X) << 2)))));
      goto START;}
      break;
    case 19 : {
      b_326X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      *((long*)(4 + (RSstackS + (((long)b_326X) << 2)))) = RSvalS;
      goto START;}
      break;
    case 20 : {
      b_319X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_320X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_321X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      x_322X = Sstack_limitS < (-276 + RSstackS);
      if (x_322X) {
        arg1K0 = 0;
        goto L28023;}
      else {
        space_323X = 1 + ((Sstack_endS - RSstackS) >> 2);
        v_324X = (ShpS + (space_323X << 2)) < SlimitS;
        if (v_324X) {
          arg1K0 = 0;
          goto L28044;}
        else {
          merged_arg1K0 = 0;
          collect_saving_temp_return_tag = 2;
          goto collect_saving_temp;
         collect_saving_temp_return_2:
          okayP_325X = (ShpS + (space_323X << 2)) < SlimitS;
          if (okayP_325X) {
            arg1K0 = 0;
            goto L28044;}
          else {
            TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
            arg1K0 = 0;
            goto L28044;}}}}
      break;
    case 21 : {
      space_16X = 1 + ((Sstack_endS - RSstackS) >> 2);
      v_15X = (ShpS + (space_16X << 2)) < SlimitS;
      if (v_15X) {
        arg1K0 = 0;
        goto L29687;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 3;
        goto collect_saving_temp;
       collect_saving_temp_return_3:
        okayP_14X = (ShpS + (space_16X << 2)) < SlimitS;
        if (okayP_14X) {
          arg1K0 = 0;
          goto L29687;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          arg1K0 = 0;
          goto L29687;}}}
      break;
    case 22 : {
      cont_18X = *((long*)(-3 + Sbottom_of_stackS));
      if ((1 == cont_18X)) {
        if ((0 == (3 & RSvalS))) {
          RSvalS = (RSvalS >> 2);
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          TTreturn_value = 0;
          return(0L);}
        else {
          RSstackS = (-11 + Sbottom_of_stackS);
          *((long*)(-3 + Sbottom_of_stackS)) = 1;
          merged_arg0K0 = 0;
          merged_arg1K1 = RSvalS;
          goto raise_exception1;}}
      else {
        if ((1 == cont_18X)) {
          RSstackS = (-11 + Sbottom_of_stackS);
          *((long*)(-3 + Sbottom_of_stackS)) = 1;
          arg1K0 = Sbottom_of_stackS;
          goto L28153;}
        else {
          merged_arg1K0 = cont_18X;
          copy_continuation_from_heap_return_tag = 0;
          goto copy_continuation_from_heap;
         copy_continuation_from_heap_return_0:
          v_17X = copy_continuation_from_heap_return_value;
          arg1K0 = v_17X;
          goto L28153;}}}
      break;
    case 23 : {
      SnargsS = ((long)(*((unsigned char*)RScode_pointerS)));
      merged_arg0K0 = 0;
      goto perform_application;}
      break;
    case 24 : {
      SnargsS = ((long)(*((unsigned char*)RScode_pointerS)));
      start_arg_317X = RSstackS + (SnargsS << 2);
      start_loc_318X = -11 + ScontS;
      if ((start_arg_317X < start_loc_318X)) {
        arg1K0 = start_loc_318X;
        arg1K1 = start_arg_317X;
        goto L15470;}
      else {
        goto L15453;}}
      break;
    case 25 : {
      RSstackS = (4 + RSstackS);
      proc_20X = *((long*)RSstackS);
      list_19X = RSvalS;
      arg1K0 = list_19X;
      arg1K1 = 0;
      arg1K2 = list_19X;
      arg3K3 = 0;
      goto L24287;}
      break;
    case 26 : {
      RSstackS = (4 + RSstackS);
      cont_22X = *((long*)RSstackS);
      if ((1 == cont_22X)) {
        RSstackS = (-11 + Sbottom_of_stackS);
        *((long*)(-3 + Sbottom_of_stackS)) = 1;
        arg1K0 = Sbottom_of_stackS;
        goto L29721;}
      else {
        merged_arg1K0 = cont_22X;
        copy_continuation_from_heap_return_tag = 1;
        goto copy_continuation_from_heap;
       copy_continuation_from_heap_return_1:
        v_21X = copy_continuation_from_heap_return_value;
        arg1K0 = v_21X;
        goto L29721;}}
      break;
    case 27 : {
      RSstackS = (4 + RSstackS);
      consumer_316X = *((long*)RSstackS);
      *((long*)RSstackS) = RSvalS;
      RSstackS = (-4 + RSstackS);
      RSvalS = consumer_316X;
      SnargsS = 1;
      merged_arg0K0 = 0;
      goto perform_application;}
      break;
    case 28 : {
      cont_25X = ScontS;
      tem_24X = *((long*)(5 + cont_25X));
      pc_23X = *((long*)(1 + cont_25X));
      StemplateS = tem_24X;
      RScode_pointerS = ((-3 + (*((long*)(-3 + tem_24X)))) + (pc_23X >> 2));
      RSenvS = (*((long*)(9 + cont_25X)));
      ScontS = (*((long*)(-3 + cont_25X)));
      RSstackS = (9 + cont_25X);
      goto START;}
      break;
    case 29 : {
      merged_arg1K0 = SnargsS;
      goto return_values;}
      break;
    case 30 : {
      merged_arg1K0 = ((long)(*((unsigned char*)RScode_pointerS)));
      goto return_values;}
      break;
    case 31 : {
      goto START;}
      break;
    case 32 : {
      b_27X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      tem_26X = *((long*)((-3 + StemplateS) + (((long)b_27X) << 2)));
      StemplateS = tem_26X;
      RScode_pointerS = (-3 + (*((long*)(-3 + tem_26X))));
      goto START;}
      break;
    case 33 : {
      b_30X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      SnargsS = ((long)b_30X);
      b_29X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      tem_28X = *((long*)((-3 + StemplateS) + (((long)b_29X) << 2)));
      StemplateS = tem_28X;
      RScode_pointerS = (-3 + (*((long*)(-3 + tem_28X))));
      if ((0 == (Spending_interruptsS & Senabled_interruptsS))) {
        goto START;}
      else {
        goto handle_interrupt;}}
      break;
    case 34 : {
      b_32X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_31X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((1 == RSvalS)) {
        RScode_pointerS = (RScode_pointerS + ((((long)b_32X) << 8) + ((long)b_31X)));
        goto START;}
      else {
        goto START;}}
      break;
    case 35 : {
      b_314X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_315X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RScode_pointerS = (RScode_pointerS + ((((long)b_314X) << 8) + ((long)b_315X)));
      goto START;}
      break;
    case 36 : {
      if ((0 == (3 & RSvalS))) {
        val_34X = RSvalS >> 2;
        b_33X = *((unsigned char*)RScode_pointerS);
        RScode_pointerS = (1 + RScode_pointerS);
        if ((val_34X < 0)) {
          arg1K0 = (((long)b_33X) << 1);
          goto L28358;}
        else {
          if ((val_34X < ((long)b_33X))) {
            RScode_pointerS = (RScode_pointerS + (val_34X << 1));
            b_312X = *((unsigned char*)RScode_pointerS);
            RScode_pointerS = (1 + RScode_pointerS);
            b_313X = *((unsigned char*)RScode_pointerS);
            RScode_pointerS = (1 + RScode_pointerS);
            arg1K0 = ((((long)b_312X) << 8) + ((long)b_313X));
            goto L28358;}
          else {
            arg1K0 = (((long)b_33X) << 1);
            goto L28358;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 37 : {
      RSstackS = (4 + RSstackS);
      arg2_310X = *((long*)RSstackS);
      x_311X = RSvalS;
      if ((arg2_310X == x_311X)) {
        arg0K0 = 5;
        goto L29842;}
      else {
        arg0K0 = 1;
        goto L29842;}}
      break;
    case 38 : {
      x_309X = RSvalS;
      if ((0 == (3 & x_309X))) {
        arg0K0 = 5;
        goto L29856;}
      else {
        if ((3 == (3 & x_309X))) {
          v_35X = 31 & ((*((long*)(-7 + x_309X))) >> 2);
          if ((9 == v_35X)) {
            arg0K0 = 5;
            goto L29856;}
          else {
            arg0K0 = 1;
            goto L29856;}}
        else {
          arg0K0 = 1;
          goto L29856;}}}
      break;
    case 39 : {
      x_308X = RSvalS;
      if ((0 == (3 & x_308X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_308X))) {
          v_36X = 31 & ((*((long*)(-7 + x_308X))) >> 2);
          if ((9 == v_36X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_308X;
            goto raise_exception1;}
          else {
            goto L29876;}}
        else {
          goto L29876;}}}
      break;
    case 40 : {
      x_307X = RSvalS;
      if ((0 == (3 & x_307X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_307X))) {
          v_37X = 31 & ((*((long*)(-7 + x_307X))) >> 2);
          if ((9 == v_37X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_307X;
            goto raise_exception1;}
          else {
            goto L29905;}}
        else {
          goto L29905;}}}
      break;
    case 41 : {
      x_306X = RSvalS;
      if ((0 == (3 & x_306X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_306X))) {
          v_38X = 31 & ((*((long*)(-7 + x_306X))) >> 2);
          if ((9 == v_38X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_306X;
            goto raise_exception1;}
          else {
            goto L29934;}}
        else {
          goto L29934;}}}
      break;
    case 42 : {
      x_305X = RSvalS;
      if ((0 == (3 & x_305X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_305X))) {
          v_39X = 31 & ((*((long*)(-7 + x_305X))) >> 2);
          if ((9 == v_39X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_305X;
            goto raise_exception1;}
          else {
            goto L29963;}}
        else {
          goto L29963;}}}
      break;
    case 43 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = 5;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 44 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 45 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 46 : {
      RSstackS = (4 + RSstackS);
      arg2_304X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_304X | RSvalS)))) {
        x_41X = RSvalS;
        z_40X = (arg2_304X >> 2) + (x_41X >> 2);
        if ((536870911 < z_40X)) {
          goto L23195;}
        else {
          if ((z_40X < -536870912)) {
            goto L23195;}
          else {
            RSvalS = (z_40X << 2);
            goto START;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_304X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 47 : {
      RSstackS = (4 + RSstackS);
      arg2_42X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_42X | RSvalS)))) {
        x_301X = RSvalS;
        a_302X = arg2_42X >> 2;
        b_303X = x_301X >> 2;
        if ((a_302X < 0)) {
          arg1K0 = (0 - a_302X);
          goto L22993;}
        else {
          arg1K0 = a_302X;
          goto L22993;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_42X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 48 : {
      RSstackS = (4 + RSstackS);
      arg2_43X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_43X | RSvalS)))) {
        x_299X = RSvalS;
        z_300X = (arg2_43X >> 2) - (x_299X >> 2);
        if ((536870911 < z_300X)) {
          goto L22878;}
        else {
          if ((z_300X < -536870912)) {
            goto L22878;}
          else {
            RSvalS = (z_300X << 2);
            goto START;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_43X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 49 : {
      RSstackS = (4 + RSstackS);
      arg2_44X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_44X | RSvalS)))) {
        x_296X = RSvalS;
        if ((0 == x_296X)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_44X;
          merged_arg1K2 = x_296X;
          goto raise_exception2;}
        else {
          a_297X = arg2_44X >> 2;
          b_298X = x_296X >> 2;
          if ((a_297X < 0)) {
            arg1K0 = (0 - a_297X);
            goto L22674;}
          else {
            arg1K0 = a_297X;
            goto L22674;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_44X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 50 : {
      RSstackS = (4 + RSstackS);
      arg2_295X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_295X | RSvalS)))) {
        x_45X = RSvalS;
        if ((arg2_295X == x_45X)) {
          arg0K0 = 5;
          goto L30048;}
        else {
          arg0K0 = 1;
          goto L30048;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_295X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 51 : {
      RSstackS = (4 + RSstackS);
      arg2_46X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_46X | RSvalS)))) {
        x_294X = RSvalS;
        if ((arg2_46X < x_294X)) {
          arg0K0 = 5;
          goto L30072;}
        else {
          arg0K0 = 1;
          goto L30072;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_46X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 52 : {
      RSstackS = (4 + RSstackS);
      arg2_47X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_47X | RSvalS)))) {
        x_291X = RSvalS;
        if ((0 == x_291X)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_47X;
          merged_arg1K2 = x_291X;
          goto raise_exception2;}
        else {
          a_292X = arg2_47X >> 2;
          b_293X = x_291X >> 2;
          if ((a_292X < 0)) {
            arg1K0 = (0 - a_292X);
            goto L22345;}
          else {
            arg1K0 = a_292X;
            goto L22345;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_47X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 53 : {
      RSstackS = (4 + RSstackS);
      arg2_48X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_48X | RSvalS)))) {
        x_288X = RSvalS;
        if ((0 == x_288X)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_48X;
          merged_arg1K2 = x_288X;
          goto raise_exception2;}
        else {
          a_289X = arg2_48X >> 2;
          b_290X = x_288X >> 2;
          if ((a_289X < 0)) {
            arg1K0 = (0 - a_289X);
            goto L22216;}
          else {
            arg1K0 = a_289X;
            goto L22216;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_48X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 54 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = RSvalS;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 55 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = RSvalS;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 56 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = 4;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 57 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = RSvalS;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 58 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = 0;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 59 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 60 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 61 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 62 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 63 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 64 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 65 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 66 : {
      RSstackS = (4 + RSstackS);
      arg2_49X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_49X | RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_49X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_49X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 67 : {
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 68 : {
      if ((0 == (3 & RSvalS))) {
        x_50X = RSvalS;
        if ((x_50X < 0)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = x_50X;
          goto raise_exception1;}
        else {
          RSvalS = 0;
          goto START;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 69 : {
      if ((0 == (3 & RSvalS))) {
        x_287X = RSvalS;
        if ((x_287X < 0)) {
          arg1K0 = (0 - x_287X);
          goto L30274;}
        else {
          arg1K0 = x_287X;
          goto L30274;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 70 : {
      RSstackS = (4 + RSstackS);
      arg2_51X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_51X | RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_51X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_51X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 71 : {
      RSstackS = (4 + RSstackS);
      arg2_52X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_52X | RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_52X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_52X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 72 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = ((~ (RSvalS >> 2)) << 2);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 73 : {
      RSstackS = (4 + RSstackS);
      arg2_53X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_53X | RSvalS)))) {
        RSvalS = (((arg2_53X >> 2) & (RSvalS >> 2)) << 2);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_53X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 74 : {
      RSstackS = (4 + RSstackS);
      arg2_54X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_54X | RSvalS)))) {
        RSvalS = (((arg2_54X >> 2) | (RSvalS >> 2)) << 2);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_54X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 75 : {
      RSstackS = (4 + RSstackS);
      arg2_55X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_55X | RSvalS)))) {
        RSvalS = (((arg2_55X >> 2) ^ (RSvalS >> 2)) << 2);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_55X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 76 : {
      RSstackS = (4 + RSstackS);
      arg2_285X = *((long*)RSstackS);
      if ((0 == (3 & (arg2_285X | RSvalS)))) {
        x_61X = RSvalS;
        count_60X = x_61X >> 2;
        value_59X = arg2_285X >> 2;
        if ((count_60X < 0)) {
          PS_SHIFT_RIGHT(value_59X, (0 - count_60X), x_286X)
          RSvalS = (x_286X << 2);
          goto START;}
        else {
          PS_SHIFT_LEFT(value_59X, count_60X, x_58X)
          result_57X = (x_58X << 2) >> 2;
          PS_SHIFT_RIGHT(result_57X, count_60X, x_56X)
          if ((value_59X == x_56X)) {
            if ((value_59X < 0)) {
              if ((result_57X < 0)) {
                goto L21679;}
              else {
                goto L21676;}}
            else {
              if ((result_57X < 0)) {
                goto L21676;}
              else {
                goto L21679;}}}
          else {
            goto L21676;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_285X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 77 : {
      x_284X = RSvalS;
      if ((9 == (255 & x_284X))) {
        arg0K0 = 5;
        goto L30388;}
      else {
        arg0K0 = 1;
        goto L30388;}}
      break;
    case 78 : {
      RSstackS = (4 + RSstackS);
      arg2_282X = *((long*)RSstackS);
      if ((9 == (255 & arg2_282X))) {
        if ((9 == (255 & RSvalS))) {
          x_283X = RSvalS;
          if ((arg2_282X == x_283X)) {
            arg0K0 = 5;
            goto L30415;}
          else {
            arg0K0 = 1;
            goto L30415;}}
        else {
          goto L30403;}}
      else {
        goto L30403;}}
      break;
    case 79 : {
      RSstackS = (4 + RSstackS);
      arg2_280X = *((long*)RSstackS);
      if ((9 == (255 & arg2_280X))) {
        if ((9 == (255 & RSvalS))) {
          x_281X = RSvalS;
          if ((arg2_280X < x_281X)) {
            arg0K0 = 5;
            goto L30444;}
          else {
            arg0K0 = 1;
            goto L30444;}}
        else {
          goto L30432;}}
      else {
        goto L30432;}}
      break;
    case 80 : {
      if ((9 == (255 & RSvalS))) {
        RSvalS = (((long)(((RSvalS >> 8)))) << 2);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 81 : {
      if ((0 == (3 & RSvalS))) {
        x_62X = RSvalS >> 2;
        if ((255 < x_62X)) {
          goto L30475;}
        else {
          if ((x_62X < 0)) {
            goto L30475;}
          else {
            RSvalS = (9 + (((long)((x_62X))) << 8));
            goto START;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 82 : {
      x_279X = RSvalS;
      if ((21 == x_279X)) {
        arg0K0 = 5;
        goto L30497;}
      else {
        arg0K0 = 1;
        goto L30497;}}
      break;
    case 83 : {
      space_65X = 4 + (((long)(*((unsigned char*)RScode_pointerS))) << 2);
      v_64X = (ShpS + (space_65X << 2)) < SlimitS;
      if (v_64X) {
        goto L16173;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 4;
        goto collect_saving_temp;
       collect_saving_temp_return_4:
        okayP_63X = (ShpS + (space_65X << 2)) < SlimitS;
        if (okayP_63X) {
          goto L16173;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L16173;}}}
      break;
    case 84 : {
      RSstackS = (4 + RSstackS);
      arg2_66X = *((long*)RSstackS);
      if ((0 == (3 & arg2_66X))) {
        len_272X = arg2_66X >> 2;
        init_273X = RSvalS;
        b_274X = *((unsigned char*)RScode_pointerS);
        RScode_pointerS = (1 + RScode_pointerS);
        if ((len_272X < 0)) {
          merged_arg0K0 = 1;
          merged_arg1K1 = (((long)b_274X) << 2);
          merged_arg1K2 = (len_272X << 2);
          merged_arg1K3 = init_273X;
          goto raise_exception3;}
        else {
          space_275X = 1 + len_272X;
          v_276X = (ShpS + (space_275X << 2)) < SlimitS;
          if (v_276X) {
            arg3K0 = 1;
            arg1K1 = init_273X;
            goto L24027;}
          else {
            merged_arg1K0 = init_273X;
            collect_saving_temp_return_tag = 5;
            goto collect_saving_temp;
           collect_saving_temp_return_5:
            temp_277X = collect_saving_temp_return_value;
            v_278X = (ShpS + (space_275X << 2)) < SlimitS;
            arg3K0 = v_278X;
            arg1K1 = temp_277X;
            goto L24027;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_66X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 85 : {
      RSstackS = (4 + RSstackS);
      arg2_267X = *((long*)RSstackS);
      if ((0 == (3 & arg2_267X))) {
        len_69X = arg2_267X >> 2;
        init_68X = RSvalS;
        b_67X = *((unsigned char*)RScode_pointerS);
        RScode_pointerS = (1 + RScode_pointerS);
        if ((0 == (3 & init_68X))) {
          if ((len_69X < 0)) {
            goto L23766;}
          else {
            space_268X = 1 + ((3 + len_69X) >> 2);
            v_269X = (ShpS + (space_268X << 2)) < SlimitS;
            if (v_269X) {
              arg3K0 = 1;
              arg1K1 = init_68X;
              goto L23773;}
            else {
              merged_arg1K0 = init_68X;
              collect_saving_temp_return_tag = 6;
              goto collect_saving_temp;
             collect_saving_temp_return_6:
              temp_270X = collect_saving_temp_return_value;
              v_271X = (ShpS + (space_268X << 2)) < SlimitS;
              arg3K0 = v_271X;
              arg1K1 = temp_270X;
              goto L23773;}}}
        else {
          goto L23766;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_267X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 86 : {
      RSstackS = (4 + RSstackS);
      arg2_74X = *((long*)RSstackS);
      if ((0 == (3 & arg2_74X))) {
        len_264X = arg2_74X >> 2;
        init_265X = RSvalS;
        b_266X = *((unsigned char*)RScode_pointerS);
        RScode_pointerS = (1 + RScode_pointerS);
        if ((9 == (255 & init_265X))) {
          if ((len_264X < 0)) {
            goto L23512;}
          else {
            space_73X = 1 + ((3 + len_264X) >> 2);
            v_72X = (ShpS + (space_73X << 2)) < SlimitS;
            if (v_72X) {
              arg3K0 = 1;
              arg1K1 = init_265X;
              goto L23519;}
            else {
              merged_arg1K0 = init_265X;
              collect_saving_temp_return_tag = 7;
              goto collect_saving_temp;
             collect_saving_temp_return_7:
              temp_71X = collect_saving_temp_return_value;
              v_70X = (ShpS + (space_73X << 2)) < SlimitS;
              arg3K0 = v_70X;
              arg1K1 = temp_71X;
              goto L23519;}}}
        else {
          goto L23512;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_74X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 87 : {
      x_262X = RSvalS;
      b_263X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_262X))) {
        v_75X = 31 & ((*((long*)(-7 + x_262X))) >> 2);
        if ((v_75X == ((long)b_263X))) {
          arg0K0 = 5;
          goto L30561;}
        else {
          arg0K0 = 1;
          goto L30561;}}
      else {
        arg0K0 = 1;
        goto L30561;}}
      break;
    case 88 : {
      x_78X = RSvalS;
      b_77X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_78X))) {
        v_261X = 31 & ((*((long*)(-7 + x_78X))) >> 2);
        if ((v_261X == ((long)b_77X))) {
          x_76X = (3 + ((*((long*)(-7 + x_78X))) >> 8)) >> 2;
          RSvalS = (x_76X << 2);
          goto START;}
        else {
          goto L21396;}}
      else {
        goto L21396;}}
      break;
    case 89 : {
      x_258X = RSvalS;
      b_259X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_258X))) {
        v_79X = 31 & ((*((long*)(-7 + x_258X))) >> 2);
        if ((v_79X == ((long)b_259X))) {
          x_260X = (*((long*)(-7 + x_258X))) >> 8;
          RSvalS = (x_260X << 2);
          goto START;}
        else {
          goto L21310;}}
      else {
        goto L21310;}}
      break;
    case 90 : {
      x_82X = RSvalS;
      b_81X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_80X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_82X))) {
        v_257X = 31 & ((*((long*)(-7 + x_82X))) >> 2);
        if ((v_257X == ((long)b_81X))) {
          RSvalS = (*((long*)((-3 + x_82X) + (((long)b_80X) << 2))));
          goto START;}
        else {
          goto L23406;}}
      else {
        goto L23406;}}
      break;
    case 91 : {
      RSstackS = (4 + RSstackS);
      arg2_253X = *((long*)RSstackS);
      x_254X = RSvalS;
      b_255X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_256X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & arg2_253X))) {
        v_83X = 31 & ((*((long*)(-7 + arg2_253X))) >> 2);
        if ((v_83X == ((long)b_255X))) {
          if ((3 == (3 & arg2_253X))) {
            if ((0 == (128 & (*((long*)(-7 + arg2_253X)))))) {
              *((long*)((-3 + arg2_253X) + (((long)b_256X) << 2))) = x_254X;
              RSvalS = 13;
              goto START;}
            else {
              goto L24993;}}
          else {
            goto L24993;}}
        else {
          goto L24993;}}
      else {
        goto L24993;}}
      break;
    case 92 : {
      RSstackS = (4 + RSstackS);
      arg2_249X = *((long*)RSstackS);
      index_250X = RSvalS;
      b_251X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & index_250X))) {
        index_85X = index_250X >> 2;
        if ((3 == (3 & arg2_249X))) {
          v_252X = 31 & ((*((long*)(-7 + arg2_249X))) >> 2);
          if ((v_252X == ((long)b_251X))) {
            len_84X = (3 + ((*((long*)(-7 + arg2_249X))) >> 8)) >> 2;
            if ((index_85X < 0)) {
              goto L21218;}
            else {
              if ((index_85X < len_84X)) {
                RSvalS = (*((long*)((-3 + arg2_249X) + (index_85X << 2))));
                goto START;}
              else {
                goto L21218;}}}
          else {
            goto L21218;}}
        else {
          goto L21218;}}
      else {
        merged_arg0K0 = 1;
        merged_arg1K1 = arg2_249X;
        merged_arg1K2 = (((long)b_251X) << 2);
        merged_arg1K3 = index_250X;
        goto raise_exception3;}}
      break;
    case 93 : {
      RSstackS = (4 + RSstackS);
      arg2_244X = *((long*)RSstackS);
      index_245X = RSvalS;
      b_246X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & index_245X))) {
        index_87X = index_245X >> 2;
        if ((3 == (3 & arg2_244X))) {
          v_247X = 31 & ((*((long*)(-7 + arg2_244X))) >> 2);
          if ((v_247X == ((long)b_246X))) {
            len_86X = (*((long*)(-7 + arg2_244X))) >> 8;
            if ((index_87X < 0)) {
              goto L21093;}
            else {
              if ((index_87X < len_86X)) {
                x_248X = (long)(*((unsigned char*)((-3 + arg2_244X) + index_87X)));
                RSvalS = (x_248X << 2);
                goto START;}
              else {
                goto L21093;}}}
          else {
            goto L21093;}}
        else {
          goto L21093;}}
      else {
        merged_arg0K0 = 1;
        merged_arg1K1 = arg2_244X;
        merged_arg1K2 = (((long)b_246X) << 2);
        merged_arg1K3 = index_245X;
        goto raise_exception3;}}
      break;
    case 94 : {
      RSstackS = (4 + RSstackS);
      arg2_239X = *((long*)RSstackS);
      index_240X = RSvalS;
      b_241X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((0 == (3 & index_240X))) {
        index_89X = index_240X >> 2;
        if ((3 == (3 & arg2_239X))) {
          v_242X = 31 & ((*((long*)(-7 + arg2_239X))) >> 2);
          if ((v_242X == ((long)b_241X))) {
            len_88X = (*((long*)(-7 + arg2_239X))) >> 8;
            if ((index_89X < 0)) {
              goto L20966;}
            else {
              if ((index_89X < len_88X)) {
                v_243X = (long)(*((unsigned char*)((-3 + arg2_239X) + index_89X)));
                RSvalS = (9 + (((long)((v_243X))) << 8));
                goto START;}
              else {
                goto L20966;}}}
          else {
            goto L20966;}}
        else {
          goto L20966;}}
      else {
        merged_arg0K0 = 1;
        merged_arg1K1 = arg2_239X;
        merged_arg1K2 = (((long)b_241X) << 2);
        merged_arg1K3 = index_240X;
        goto raise_exception3;}}
      break;
    case 95 : {
      RSstackS = (4 + RSstackS);
      arg2_235X = *((long*)RSstackS);
      RSstackS = (4 + RSstackS);
      stob_236X = *((long*)RSstackS);
      value_237X = RSvalS;
      b_238X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & stob_236X))) {
        v_92X = 31 & ((*((long*)(-7 + stob_236X))) >> 2);
        if ((v_92X == ((long)b_238X))) {
          if ((0 == (3 & arg2_235X))) {
            len_91X = (3 + ((*((long*)(-7 + stob_236X))) >> 8)) >> 2;
            index_90X = arg2_235X >> 2;
            if ((index_90X < 0)) {
              goto L24791;}
            else {
              if ((index_90X < len_91X)) {
                if ((3 == (3 & stob_236X))) {
                  if ((0 == (128 & (*((long*)(-7 + stob_236X)))))) {
                    *((long*)((-3 + stob_236X) + (-4 & arg2_235X))) = value_237X;
                    RSvalS = 13;
                    goto START;}
                  else {
                    goto L24791;}}
                else {
                  goto L24791;}}
              else {
                goto L24791;}}}
          else {
            goto L24791;}}
        else {
          goto L24791;}}
      else {
        goto L24791;}}
      break;
    case 96 : {
      RSstackS = (4 + RSstackS);
      arg2_96X = *((long*)RSstackS);
      RSstackS = (4 + RSstackS);
      stob_95X = *((long*)RSstackS);
      value_94X = RSvalS;
      b_93X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & stob_95X))) {
        v_232X = 31 & ((*((long*)(-7 + stob_95X))) >> 2);
        if ((v_232X == ((long)b_93X))) {
          if ((0 == (3 & arg2_96X))) {
            len_233X = (*((long*)(-7 + stob_95X))) >> 8;
            index_234X = arg2_96X >> 2;
            if ((index_234X < 0)) {
              goto L24583;}
            else {
              if ((index_234X < len_233X)) {
                if ((0 == (3 & value_94X))) {
                  if ((3 == (3 & stob_95X))) {
                    if ((0 == (128 & (*((long*)(-7 + stob_95X)))))) {
                      *((unsigned char*)((-3 + stob_95X) + (arg2_96X >> 2))) = ((unsigned char)(value_94X >> 2));
                      RSvalS = 13;
                      goto START;}
                    else {
                      goto L24583;}}
                  else {
                    goto L24583;}}
                else {
                  goto L24583;}}
              else {
                goto L24583;}}}
          else {
            goto L24583;}}
        else {
          goto L24583;}}
      else {
        goto L24583;}}
      break;
    case 97 : {
      RSstackS = (4 + RSstackS);
      arg2_228X = *((long*)RSstackS);
      RSstackS = (4 + RSstackS);
      stob_229X = *((long*)RSstackS);
      value_230X = RSvalS;
      b_231X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & stob_229X))) {
        v_99X = 31 & ((*((long*)(-7 + stob_229X))) >> 2);
        if ((v_99X == ((long)b_231X))) {
          if ((0 == (3 & arg2_228X))) {
            len_98X = (*((long*)(-7 + stob_229X))) >> 8;
            index_97X = arg2_228X >> 2;
            if ((index_97X < 0)) {
              goto L24375;}
            else {
              if ((index_97X < len_98X)) {
                if ((9 == (255 & value_230X))) {
                  if ((3 == (3 & stob_229X))) {
                    if ((0 == (128 & (*((long*)(-7 + stob_229X)))))) {
                      *((unsigned char*)((-3 + stob_229X) + (arg2_228X >> 2))) = ((unsigned char)((long)(((value_230X >> 8)))));
                      RSvalS = 13;
                      goto START;}
                    else {
                      goto L24375;}}
                  else {
                    goto L24375;}}
                else {
                  goto L24375;}}
              else {
                goto L24375;}}}
          else {
            goto L24375;}}
        else {
          goto L24375;}}
      else {
        goto L24375;}}
      break;
    case 98 : {
      obj_102X = RSvalS;
      if ((3 == (3 & obj_102X))) {
        v_227X = 31 & ((*((long*)(-7 + obj_102X))) >> 2);
        if ((4 == v_227X)) {
          x_101X = RSvalS;
          descriptor_100X = *((long*)(-3 + x_101X));
          if ((17 == (255 & descriptor_100X))) {
            if ((529 == (*((long*)(-3 + x_101X))))) {
              arg0K0 = 5;
              goto L17745;}
            else {
              arg0K0 = 1;
              goto L17745;}}
          else {
            arg0K0 = 5;
            goto L17745;}}
        else {
          goto L17736;}}
      else {
        goto L17736;}}
      break;
    case 99 : {
      RSstackS = (4 + RSstackS);
      arg2_225X = *((long*)RSstackS);
      if ((3 == (3 & arg2_225X))) {
        v_103X = 31 & ((*((long*)(-7 + arg2_225X))) >> 2);
        if ((4 == v_103X)) {
          x_226X = RSvalS;
          if ((1 == x_226X)) {
            goto L20752;}
          else {
            if ((5 == x_226X)) {
              goto L20752;}
            else {
              goto L20780;}}}
        else {
          goto L20780;}}
      else {
        goto L20780;}}
      break;
    case 100 : {
      x_104X = RSvalS;
      if ((3 == (3 & x_104X))) {
        if ((0 == (128 & (*((long*)(-7 + x_104X)))))) {
          arg0K0 = 1;
          goto L30645;}
        else {
          arg0K0 = 5;
          goto L30645;}}
      else {
        arg0K0 = 5;
        goto L30645;}}
      break;
    case 101 : {
      x_224X = RSvalS;
      if ((3 == (3 & x_224X))) {
        if ((0 == (128 & (*((long*)(-7 + x_224X)))))) {
          *((long*)(-7 + x_224X)) = (128 | (*((long*)(-7 + x_224X))));
          /* goto L30660; */}
        /* else {
          goto L30660;} */}
      /* else {
        goto L30660;} */
      goto START;}
      /* break; */
    case 102 : {
      v_106X = (20 + ShpS) < SlimitS;
      if (v_106X) {
        arg1K0 = 0;
        goto L20464;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 8;
        goto collect_saving_temp;
       collect_saving_temp_return_8:
        okayP_105X = (20 + ShpS) < SlimitS;
        if (okayP_105X) {
          arg1K0 = 0;
          goto L20464;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          arg1K0 = 0;
          goto L20464;}}}
      break;
    case 103 : {
      obj_223X = RSvalS;
      if ((3 == (3 & obj_223X))) {
        v_107X = 31 & ((*((long*)(-7 + obj_223X))) >> 2);
        if ((5 == v_107X)) {
          merged_arg1K0 = RSvalS;
          close_port_return_tag = 0;
          goto close_port;
         close_port_return_0:
          RSvalS = 13;
          goto START;}
        else {
          goto L30681;}}
      else {
        goto L30681;}}
      break;
    case 104 : {
      x_221X = RSvalS;
      if ((3 == (3 & x_221X))) {
        v_108X = 31 & ((*((long*)(-7 + x_221X))) >> 2);
        if ((5 == v_108X)) {
          v_222X = 4 == (*((long*)(-3 + x_221X)));
          if (v_222X) {
            arg0K0 = 5;
            goto L30705;}
          else {
            arg0K0 = 1;
            goto L30705;}}
        else {
          arg0K0 = 1;
          goto L30705;}}
      else {
        arg0K0 = 1;
        goto L30705;}}
      break;
    case 105 : {
      x_219X = RSvalS;
      if ((3 == (3 & x_219X))) {
        v_109X = 31 & ((*((long*)(-7 + x_219X))) >> 2);
        if ((5 == v_109X)) {
          v_220X = 8 == (*((long*)(-3 + x_219X)));
          if (v_220X) {
            arg0K0 = 5;
            goto L30727;}
          else {
            arg0K0 = 1;
            goto L30727;}}
        else {
          arg0K0 = 1;
          goto L30727;}}
      else {
        arg0K0 = 1;
        goto L30727;}}
      break;
    case 106 : {
      p_112X = RSvalS;
      if ((3 == (3 & p_112X))) {
        v_215X = 31 & ((*((long*)(-7 + p_112X))) >> 2);
        if ((5 == v_215X)) {
          v_111X = 4 == (*((long*)(-3 + p_112X)));
          if (v_111X) {
            if (((*((long*)(1 + p_112X))) < 0)) {
              goto L17326;}
            else {
              x_216X = RSvalS;
              c_217X = *((long*)(5 + x_216X));
              if ((1 == c_217X)) {
                v_110X = *(Sopen_portsS + ((*((long*)(1 + x_216X))) >> 2));
                { int TTchar;
                  PS_GETC(v_110X, TTchar);
                  if (EOF == TTchar) {
                    arg1K0 = 21;
                    goto L17323;}
                  else {
                    unsigned char Kchar_218X = TTchar;
                    arg1K0 = (9 + (((long)(Kchar_218X)) << 8));
                    goto L17323;}}}
              else {
                *((long*)(5 + x_216X)) = 1;
                arg1K0 = c_217X;
                goto L17323;}}}
          else {
            goto L17326;}}
        else {
          goto L17326;}}
      else {
        goto L17326;}}
      break;
    case 107 : {
      p_212X = RSvalS;
      if ((3 == (3 & p_212X))) {
        v_116X = 31 & ((*((long*)(-7 + p_212X))) >> 2);
        if ((5 == v_116X)) {
          v_213X = 4 == (*((long*)(-3 + p_212X)));
          if (v_213X) {
            if (((*((long*)(1 + p_212X))) < 0)) {
              goto L17212;}
            else {
              x_115X = RSvalS;
              c_114X = *((long*)(5 + x_115X));
              if ((1 == c_114X)) {
                v_214X = *(Sopen_portsS + ((*((long*)(1 + x_115X))) >> 2));
                { int TTchar;
                  PS_GETC(v_214X, TTchar);
                  if (EOF == TTchar) {
                    arg1K0 = 21;
                    goto L17204;}
                  else {
                    unsigned char Kchar_113X = TTchar;
                    arg1K0 = (9 + (((long)(Kchar_113X)) << 8));
                    goto L17204;}}}
              else {
                arg1K0 = c_114X;
                goto L17209;}}}
          else {
            goto L17212;}}
        else {
          goto L17212;}}
      else {
        goto L17212;}}
      break;
    case 108 : {
      p_119X = RSvalS;
      if ((3 == (3 & p_119X))) {
        v_209X = 31 & ((*((long*)(-7 + p_119X))) >> 2);
        if ((5 == v_209X)) {
          v_118X = 4 == (*((long*)(-3 + p_119X)));
          if (v_118X) {
            if (((*((long*)(1 + p_119X))) < 0)) {
              goto L17067;}
            else {
              x_210X = RSvalS;
              x_211X = *((long*)(5 + x_210X));
              if ((1 == x_211X)) {
                v_117X = *(Sopen_portsS + ((*((long*)(1 + x_210X))) >> 2));
                if ((char_ready_p(v_117X))) {
                  char_ready_p(v_117X);
                  arg0K0 = 5;
                  goto L17064;}
                else {
                  char_ready_p(v_117X);
                  arg0K0 = 1;
                  goto L17064;}}
              else {
                arg0K0 = 5;
                goto L17064;}}}
          else {
            goto L17067;}}
        else {
          goto L17067;}}
      else {
        goto L17067;}}
      break;
    case 109 : {
      RSstackS = (4 + RSstackS);
      arg2_122X = *((long*)RSstackS);
      if ((9 == (255 & arg2_122X))) {
        p_207X = RSvalS;
        if ((3 == (3 & p_207X))) {
          v_121X = 31 & ((*((long*)(-7 + p_207X))) >> 2);
          if ((5 == v_121X)) {
            v_208X = 8 == (*((long*)(-3 + p_207X)));
            if (v_208X) {
              if (((*((long*)(1 + p_207X))) < 0)) {
                goto L20379;}
              else {
                port_120X = *(Sopen_portsS + ((*((long*)(1 + RSvalS))) >> 2));
                putc(((arg2_122X >> 8)), port_120X);
                RSvalS = 13;
                goto START;}}
            else {
              goto L20379;}}
          else {
            goto L20379;}}
        else {
          goto L20379;}}
      else {
        goto L20379;}}
      break;
    case 110 : {
      RSstackS = (4 + RSstackS);
      arg2_204X = *((long*)RSstackS);
      if ((3 == (3 & arg2_204X))) {
        v_126X = 31 & ((*((long*)(-7 + arg2_204X))) >> 2);
        if ((15 == v_126X)) {
          p_205X = RSvalS;
          if ((3 == (3 & p_205X))) {
            v_125X = 31 & ((*((long*)(-7 + p_205X))) >> 2);
            if ((5 == v_125X)) {
              v_206X = 8 == (*((long*)(-3 + p_205X)));
              if (v_206X) {
                if (((*((long*)(1 + p_205X))) < 0)) {
                  goto L20247;}
                else {
                  port_124X = *(Sopen_portsS + ((*((long*)(1 + RSvalS))) >> 2));
                  count_123X = (*((long*)(-7 + arg2_204X))) >> 8;
                  fwrite((void *)(-3 + arg2_204X), sizeof(char), count_123X, port_124X);
                  RSvalS = 13;
                  goto START;}}
              else {
                goto L20247;}}
            else {
              goto L20247;}}
          else {
            goto L20247;}}
        else {
          goto L20247;}}
      else {
        goto L20247;}}
      break;
    case 111 : {
      p_128X = RSvalS;
      if ((3 == (3 & p_128X))) {
        v_202X = 31 & ((*((long*)(-7 + p_128X))) >> 2);
        if ((5 == v_202X)) {
          v_127X = 8 == (*((long*)(-3 + p_128X)));
          if (v_127X) {
            if (((*((long*)(1 + p_128X))) < 0)) {
              goto L16979;}
            else {
              port_203X = *(Sopen_portsS + ((*((long*)(1 + RSvalS))) >> 2));
              fflush(port_203X);
              RSvalS = 13;
              goto START;}}
          else {
            goto L16979;}}
        else {
          goto L16979;}}
      else {
        goto L16979;}}
      break;
    case 112 : {
      RSvalS = 529;
      goto START;}
      break;
    case 113 : {
      RSvalS = 13;
      goto START;}
      break;
    case 114 : {
      merged_arg0K0 = 0;
      merged_arg1K1 = RSvalS;
      goto raise_exception1;}
      break;
    case 115 : {
      RSvalS = 1;
      goto START;}
      break;
    case 116 : {
      RSstackS = (4 + RSstackS);
      arg2_192X = *((long*)RSstackS);
      RSstackS = (4 + RSstackS);
      arg3_193X = *((long*)RSstackS);
      if ((3 == (3 & arg3_193X))) {
        v_132X = 31 & ((*((long*)(-7 + arg3_193X))) >> 2);
        if ((15 == v_132X)) {
          obj_194X = RSvalS;
          if ((3 == (3 & obj_194X))) {
            v_131X = 31 & ((*((long*)(-7 + obj_194X))) >> 2);
            if ((15 == v_131X)) {
              x_195X = RSvalS;
              merged_arg1K0 = arg3_193X;
              extract_string_return_tag = 0;
              goto extract_string;
             extract_string_return_0:
              spec_196X = extract_string_return_value;
              port_197X = ps_open(spec_196X, "w");
              if ((NULL == port_197X)) {
                merged_arg0K0 = 0;
                merged_arg1K1 = arg3_193X;
                merged_arg1K2 = arg2_192X;
                goto raise_exception2;}
              else {
                count_198X = (*((long*)(-7 + x_195X))) >> 8;
                fwrite((void *)(-3 + x_195X), sizeof(char), count_198X, port_197X);
                b_199X = Snewspace_beginS;
                Snewspace_beginS = Soldspace_beginS;
                Soldspace_beginS = b_199X;
                e_200X = Snewspace_endS;
                Snewspace_endS = Soldspace_endS;
                Soldspace_endS = e_200X;
                Ssaved_limitS = SlimitS;
                Ssaved_hpS = ShpS;
                SlimitS = Snewspace_endS;
                ShpS = Snewspace_beginS;
                Sweak_pointer_hpS = -1;
                if ((3 == (3 & arg2_192X))) {
                  a_130X = -3 + arg2_192X;
                  if ((a_130X < Soldspace_beginS)) {
                    arg1K0 = arg2_192X;
                    goto L28512;}
                  else {
                    v_129X = a_130X < Soldspace_endS;
                    if (v_129X) {
                      merged_arg1K0 = arg2_192X;
                      copy_object_return_tag = 0;
                      goto copy_object;
                     copy_object_return_0:
                      v_201X = copy_object_return_value;
                      arg1K0 = v_201X;
                      goto L28512;}
                    else {
                      arg1K0 = arg2_192X;
                      goto L28512;}}}
                else {
                  arg1K0 = arg2_192X;
                  goto L28512;}}}
            else {
              goto L28528;}}
          else {
            goto L28528;}}
        else {
          goto L28528;}}
      else {
        goto L28528;}}
      break;
    case 117 : {
      RSvalS = 13;
      merged_arg1K0 = 0;
      collect_saving_temp_return_tag = 9;
      goto collect_saving_temp;
     collect_saving_temp_return_9:
      RSvalS = 13;
      goto START;}
      break;
    case 118 : {
      RSstackS = (4 + RSstackS);
      arg2_135X = *((long*)RSstackS);
      if ((0 == (3 & arg2_135X))) {
        x_188X = RSvalS;
        key_189X = arg2_135X >> 2;
        if ((0 == key_189X)) {
          n_191X = (SlimitS - ShpS) >> 2;
          RSvalS = (n_191X << 2);
          goto START;}
        else {
          if ((1 == key_189X)) {
            bytes_133X = Snewspace_endS - Snewspace_beginS;
            RSvalS = (-4 & (3 + bytes_133X));
            goto START;}
          else {
            if ((2 == key_189X)) {
              n_190X = Sstack_endS - Sstack_beginS;
              RSvalS = (n_190X << 2);
              goto START;}
            else {
              if ((3 == key_189X)) {
                if ((0 == (3 & x_188X))) {
                  old_134X = Sminimum_recovered_spaceS;
                  Sminimum_recovered_spaceS = (x_188X >> 2);
                  RSvalS = (old_134X << 2);
                  goto START;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (key_189X << 2);
                  merged_arg1K2 = x_188X;
                  goto raise_exception2;}}
              else {
                if ((4 == key_189X)) {
                  RSvalS = (Sgc_countS << 2);
                  goto START;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (key_189X << 2);
                  merged_arg1K2 = x_188X;
                  goto raise_exception2;}}}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_135X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 119 : {
      obj_185X = RSvalS;
      if ((3 == (3 & obj_185X))) {
        v_136X = 31 & ((*((long*)(-7 + obj_185X))) >> 2);
        if ((2 == v_136X)) {
          x_186X = RSvalS;
          start_hp_187X = ShpS;
          arg1K0 = Snewspace_beginS;
          goto L16862;}
        else {
          goto L16833;}}
      else {
        goto L16833;}}
      break;
    case 120 : {
      if ((0 == (3 & RSvalS))) {
        type_138X = RSvalS >> 2;
        start_hp_137X = ShpS;
        *((long*)ShpS) = 0;
        ShpS = (4 + ShpS);
        arg1K0 = Snewspace_beginS;
        goto L9053;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 121 : {
      RSvalS = Sdynamic_stateS;
      goto START;}
      break;
    case 122 : {
      Sdynamic_stateS = RSvalS;
      RSvalS = 13;
      goto START;}
      break;
    case 123 : {
      obj_139X = RSvalS;
      if ((3 == (3 & obj_139X))) {
        v_184X = 31 & ((*((long*)(-7 + obj_139X))) >> 2);
        if ((3 == v_184X)) {
          Sexception_handlerS = RSvalS;
          goto START;}
        else {
          goto L30852;}}
      else {
        goto L30852;}}
      break;
    case 124 : {
      obj_141X = RSvalS;
      if ((3 == (3 & obj_141X))) {
        v_183X = 31 & ((*((long*)(-7 + obj_141X))) >> 2);
        if ((2 == v_183X)) {
          v_140X = (3 + ((*((long*)(-7 + RSvalS))) >> 8)) >> 2;
          if ((v_140X < 3)) {
            goto L28783;}
          else {
            Sinterrupt_handlersS = RSvalS;
            goto START;}}
        else {
          goto L28783;}}
      else {
        goto L28783;}}
      break;
    case 125 : {
      temp_182X = Senabled_interruptsS;
      Senabled_interruptsS = (RSvalS >> 2);
      RSvalS = (temp_182X << 2);
      goto START;}
      break;
    case 126 : {
      RSstackS = (4 + RSstackS);
      Senabled_interruptsS = ((*((long*)RSstackS)) >> 2);
      RSstackS = (4 + RSstackS);
      SnargsS = ((*((long*)RSstackS)) >> 2);
      RSstackS = (4 + RSstackS);
      RSenvS = (*((long*)RSstackS));
      RSstackS = (4 + RSstackS);
      tem_142X = *((long*)RSstackS);
      StemplateS = tem_142X;
      RScode_pointerS = (-3 + (*((long*)(-3 + tem_142X))));
      goto START;}
      break;
    case 127 : {
      if ((0 == (3 & RSvalS))) {
        p_180X = RSvalS;
        Spending_interruptsS = (-2 & Spending_interruptsS);
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        n_181X = ps_schedule_interrupt((p_180X >> 2));
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSenvS = SenvS;
        RSvalS = (n_181X << 2);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 128 : {
      obj_147X = RSvalS;
      if ((3 == (3 & obj_147X))) {
        v_177X = 31 & ((*((long*)(-7 + obj_147X))) >> 2);
        if ((12 == v_177X)) {
          x_146X = RSvalS;
          value_145X = *((long*)(1 + x_146X));
          name_144X = *((long*)(-3 + x_146X));
          if ((3 == (3 & name_144X))) {
            v_178X = 31 & ((*((long*)(-7 + name_144X))) >> 2);
            if ((15 == v_178X)) {
              if ((3 == (3 & value_145X))) {
                v_179X = 31 & ((*((long*)(-7 + value_145X))) >> 2);
                if ((16 == v_179X)) {
                  SvalS = RSvalS;
                  SstackS = RSstackS;
                  Scode_pointerS = RScode_pointerS;
                  SenvS = RSenvS;
                  v_143X = lookup_external_name((-3 + name_144X), (-3 + value_145X));
                  RSvalS = SvalS;
                  RSstackS = SstackS;
                  RScode_pointerS = Scode_pointerS;
                  RSenvS = SenvS;
                  if (v_143X) {
                    RSvalS = 13;
                    goto START;}
                  else {
                    goto L16518;}}
                else {
                  goto L16518;}}
              else {
                goto L16518;}}
            else {
              goto L16518;}}
          else {
            goto L16518;}}
        else {
          goto L16521;}}
      else {
        goto L16521;}}
      break;
    case 129 : {
      index_175X = -1 + SnargsS;
      proc_176X = *((long*)(4 + (RSstackS + (index_175X << 2))));
      if ((3 == (3 & proc_176X))) {
        v_148X = 31 & ((*((long*)(-7 + proc_176X))) >> 2);
        if ((12 == v_148X)) {
          SnargsS = (-1 + SnargsS);
          RSvalS = proc_176X;
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          TTreturn_value = 1;
          return(0L);}
        else {
          goto L30907;}}
      else {
        goto L30907;}}
      break;
    case 130 : {
      RSstackS = (4 + RSstackS);
      arg2_151X = *((long*)RSstackS);
      if ((0 == (3 & arg2_151X))) {
        x_172X = RSvalS;
        key_173X = arg2_151X >> 2;
        if ((0 == key_173X)) {
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          n_149X = ps_ticks_per_second();
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          RSenvS = SenvS;
          RSvalS = (n_149X << 2);
          goto START;}
        else {
          if ((1 == key_173X)) {
            SvalS = RSvalS;
            SstackS = RSstackS;
            Scode_pointerS = RScode_pointerS;
            SenvS = RSenvS;
            n_174X = ps_run_time();
            RSvalS = SvalS;
            RSstackS = SstackS;
            RScode_pointerS = Scode_pointerS;
            RSenvS = SenvS;
            RSvalS = (n_174X << 2);
            goto START;}
          else {
            if ((2 == key_173X)) {
              SvalS = RSvalS;
              SstackS = RSstackS;
              Scode_pointerS = RScode_pointerS;
              SenvS = RSenvS;
              n_150X = ps_real_time();
              RSvalS = SvalS;
              RSstackS = SstackS;
              RScode_pointerS = Scode_pointerS;
              RSenvS = SenvS;
              RSvalS = (n_150X << 2);
              goto START;}
            else {
              merged_arg0K0 = 0;
              merged_arg1K1 = (key_173X << 2);
              merged_arg1K2 = x_172X;
              goto raise_exception2;}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_151X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 131 : {
      RSstackS = (4 + RSstackS);
      arg2_171X = *((long*)RSstackS);
      if ((0 == (3 & arg2_171X))) {
        x_154X = RSvalS;
        key_153X = arg2_171X >> 2;
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        return_value_152X = extended_vm(key_153X, x_154X);
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSenvS = SenvS;
        if ((17 == (255 & return_value_152X))) {
          merged_arg0K0 = 0;
          merged_arg1K1 = (key_153X << 2);
          merged_arg1K2 = x_154X;
          goto raise_exception2;}
        else {
          RSvalS = return_value_152X;
          goto START;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_171X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 132 : {
      RSstackS = (4 + RSstackS);
      arg2_170X = *((long*)RSstackS);
      if ((0 == (3 & arg2_170X))) {
        RSvalS = RSvalS;
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        TTreturn_value = 0;
        return(0L);}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_170X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 133 : {
      RSstackS = (4 + RSstackS);
      arg2_167X = *((long*)RSstackS);
      if ((3 == (3 & arg2_167X))) {
        v_156X = 31 & ((*((long*)(-7 + arg2_167X))) >> 2);
        if ((15 == v_156X)) {
          obj_168X = RSvalS;
          if ((3 == (3 & obj_168X))) {
            v_155X = 31 & ((*((long*)(-7 + obj_168X))) >> 2);
            if ((15 == v_155X)) {
              merged_arg1K0 = arg2_167X;
              merged_arg1K1 = RSvalS;
              stob_equalP_return_tag = 0;
              goto stob_equalP;
             stob_equalP_return_0:
              x_169X = stob_equalP_return_value;
              if (x_169X) {
                arg0K0 = 5;
                goto L19651;}
              else {
                arg0K0 = 1;
                goto L19651;}}
            else {
              goto L19642;}}
          else {
            goto L19642;}}
        else {
          goto L19642;}}
      else {
        goto L19642;}}
      break;
    case 134 : {
      obj_159X = RSvalS;
      if ((3 == (3 & obj_159X))) {
        v_166X = 31 & ((*((long*)(-7 + obj_159X))) >> 2);
        if ((15 == v_166X)) {
          x_158X = RSvalS;
          n_157X = (*((long*)(-7 + x_158X))) >> 8;
          arg1K0 = 0;
          arg1K1 = 0;
          goto L16368;}
        else {
          goto L16346;}}
      else {
        goto L16346;}}
      break;
    case 135 : {
      space_163X = 1 + ((3 + RSvalS) >> 2);
      v_164X = (ShpS + (space_163X << 2)) < SlimitS;
      if (v_164X) {
        goto L19375;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 10;
        goto collect_saving_temp;
       collect_saving_temp_return_10:
        okayP_165X = (ShpS + (space_163X << 2)) < SlimitS;
        if (okayP_165X) {
          goto L19375;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L19375;}}}
      break;
    case 136 : {
      v_161X = (20 + ShpS) < SlimitS;
      if (v_161X) {
        goto L19255;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 11;
        goto collect_saving_temp;
       collect_saving_temp_return_11:
        okayP_160X = (20 + ShpS) < SlimitS;
        if (okayP_160X) {
          goto L19255;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L19255;}}}
      break;
  }}
 L30821: {
  vector_344X = arg1K0;
  if ((1 == vector_344X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = (type_138X << 2);
    goto raise_exception1;}
  else {
    RSvalS = vector_344X;
    goto START;}}
 L8999: {
  ShpS = start_hp_137X;
  arg1K0 = 1;
  goto L30821;}
 L9053: {
  addr_345X = arg1K0;
  if ((addr_345X < start_hp_137X)) {
    d_347X = *((long*)addr_345X);
    if ((2 == (3 & d_347X))) {
      if ((type_138X == (31 & (d_347X >> 2)))) {
        v_346X = (16 + ShpS) < SlimitS;
        if (v_346X) {
          *((long*)ShpS) = (7 + addr_345X);
          ShpS = (4 + ShpS);
          arg1K0 = (4 + (addr_345X + (-4 & (3 + (d_347X >> 8)))));
          goto L9053;}
        else {
          goto L8999;}}
      else {
        arg1K0 = (4 + (addr_345X + (-4 & (3 + (d_347X >> 8)))));
        goto L9053;}}
    else {
      fputs("heap is in an inconsistent state.", Scurrent_output_portS);
      goto L8999;}}
  else {
    *((long*)start_hp_137X) = (-1014 + ((ShpS - start_hp_137X) << 8));
    arg1K0 = (7 + start_hp_137X);
    goto L30821;}}
 L8142: {
  h_348X = arg1K0;
  arg1K0 = (4 + (addr_349X + (-4 & (3 + (h_348X >> 8)))));
  goto L8120;}
 L8120: {
  addr_349X = arg1K0;
  if ((addr_349X < ShpS)) {
    d_351X = *((long*)addr_349X);
    if ((2 == (3 & d_351X))) {
      arg1K0 = d_351X;
      goto L8142;}
    else {
      h_350X = *((long*)(-7 + d_351X));
      *((long*)addr_349X) = h_350X;
      arg1K0 = h_350X;
      goto L8142;}}
  else {
    RSvalS = (size_405X << 2);
    goto START;}}
 L19277: {
  val_352X = arg1K0;
  RSvalS = val_352X;
  goto START;}
 L12298: {
  i_353X = arg1K0;
  h_354X = arg1K1;
  if ((i_353X < n_379X)) {
    v_357X = (long)(*((unsigned char*)((-3 + arg2_375X) + i_353X)));
    arg1K0 = (1 + i_353X);
    arg1K1 = (h_354X + ((long)((v_357X))));
    goto L12298;}
  else {
    index_356X = h_354X & (-1 + v_378X);
    bucket_355X = *((long*)((-3 + table_377X) + (index_356X << 2)));
    arg1K0 = bucket_355X;
    goto L12184;}}
 L12184: {
  b_358X = arg1K0;
  if ((25 == b_358X)) {
    *((long*)ShpS) = 1030;
    ShpS = (4 + ShpS);
    new_360X = 3 + ShpS;
    ShpS = (4 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_360X)) = arg2_375X;
    *((long*)ShpS) = 2050;
    ShpS = (4 + ShpS);
    new_361X = 3 + ShpS;
    ShpS = (8 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_361X)) = new_360X;
    *((long*)(1 + new_361X)) = bucket_355X;
    *((long*)((-3 + table_377X) + (index_356X << 2))) = new_361X;
    arg1K0 = new_360X;
    goto L19277;}
  else {
    merged_arg1K0 = arg2_375X;
    merged_arg1K1 = (*((long*)(-3 + (*((long*)(-3 + b_358X))))));
    stob_equalP_return_tag = 1;
    goto stob_equalP;
   stob_equalP_return_1:
    v_359X = stob_equalP_return_value;
    if (v_359X) {
      arg1K0 = (*((long*)(-3 + b_358X)));
      goto L19277;}
    else {
      arg1K0 = (*((long*)(1 + b_358X)));
      goto L12184;}}}
 L13252: {
  i_362X = arg1K0;
  h_363X = arg1K1;
  if ((i_362X < n_397X)) {
    v_366X = (long)(*((unsigned char*)((-3 + s_396X) + i_362X)));
    arg1K0 = (1 + i_362X);
    arg1K1 = (h_363X + ((long)((v_366X))));
    goto L13252;}
  else {
    index_365X = h_363X & (-1 + v_395X);
    bucket_364X = *((long*)((-3 + x_186X) + (index_365X << 2)));
    arg1K0 = bucket_364X;
    goto L13150;}}
 L13150: {
  b_367X = arg1K0;
  if ((25 == b_367X)) {
    *((long*)ShpS) = 2050;
    ShpS = (4 + ShpS);
    new_368X = 3 + ShpS;
    ShpS = (8 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_368X)) = obj_394X;
    *((long*)(1 + new_368X)) = bucket_364X;
    *((long*)((-3 + x_186X) + (index_365X << 2))) = new_368X;
    arg1K0 = (4 + (addr_391X + (-4 & (3 + (d_393X >> 8)))));
    goto L16862;}
  else {
    if ((obj_394X == (*((long*)(-3 + b_367X))))) {
      arg1K0 = (4 + (addr_391X + (-4 & (3 + (d_393X >> 8)))));
      goto L16862;}
    else {
      arg1K0 = (*((long*)(1 + b_367X)));
      goto L13150;}}}
 L9932: {
  stob_369X = arg1K0;
  from_370X = 4 + RSstackS;
  arg1K0 = from_370X;
  arg1K1 = (1 + stob_369X);
  goto L9971;}
 L9971: {
  from_371X = arg1K0;
  to_372X = arg1K1;
  if ((from_371X < (from_370X + (((long)b_532X) << 2)))) {
    *((long*)to_372X) = (*((long*)from_371X));
    arg1K0 = (4 + from_371X);
    arg1K1 = (4 + to_372X);
    goto L9971;}
  else {
    RSstackS = (RSstackS - ((0 - ((long)b_532X)) << 2));
    *((long*)(-3 + stob_369X)) = RSenvS;
    RSenvS = stob_369X;
    goto START;}}
 L19255: {
  RSstackS = (4 + RSstackS);
  arg2_375X = *((long*)RSstackS);
  if ((3 == (3 & arg2_375X))) {
    v_374X = 31 & ((*((long*)(-7 + arg2_375X))) >> 2);
    if ((15 == v_374X)) {
      obj_376X = RSvalS;
      if ((3 == (3 & obj_376X))) {
        v_373X = 31 & ((*((long*)(-7 + obj_376X))) >> 2);
        if ((2 == v_373X)) {
          table_377X = RSvalS;
          v_378X = (3 + ((*((long*)(-7 + table_377X))) >> 8)) >> 2;
          n_379X = (*((long*)(-7 + arg2_375X))) >> 8;
          arg1K0 = 0;
          arg1K1 = 0;
          goto L12298;}
        else {
          goto L19280;}}
      else {
        goto L19280;}}
    else {
      goto L19280;}}
  else {
    goto L19280;}}
 L19280: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_375X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L19375: {
  RSstackS = (4 + RSstackS);
  arg2_381X = *((long*)RSstackS);
  if ((0 == (3 & RSvalS))) {
    n_382X = RSvalS >> 2;
    if ((3 == (3 & arg2_381X))) {
      v_380X = 31 & ((*((long*)(-7 + arg2_381X))) >> 2);
      if ((0 == v_380X)) {
        goto L19499;}
      else {
        goto L19403;}}
    else {
      goto L19403;}}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_381X;
    merged_arg1K2 = RSvalS;
    goto raise_exception2;}}
 L19403: {
  if ((25 == arg2_381X)) {
    goto L19499;}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_381X;
    merged_arg1K2 = (n_382X << 2);
    goto raise_exception2;}}
 L19499: {
  *((long*)ShpS) = (62 + (n_382X << 8));
  ShpS = (4 + ShpS);
  new_383X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + n_382X)));
  if ((0 < n_382X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_383X;
    goto L19420;}
  else {
    arg1K0 = new_383X;
    goto L19420;}}
 L19420: {
  obj_384X = arg1K0;
  arg1K0 = arg2_381X;
  arg1K1 = (-1 + n_382X);
  goto L19422;}
 L19422: {
  l_385X = arg1K0;
  i_386X = arg1K1;
  if ((i_386X < 0)) {
    RSvalS = obj_384X;
    goto START;}
  else {
    *((unsigned char*)((-3 + obj_384X) + i_386X)) = ((unsigned char)((long)((((*((long*)(-3 + l_385X))) >> 8)))));
    arg1K0 = (*((long*)(1 + l_385X)));
    arg1K1 = (-1 + i_386X);
    goto L19422;}}
 L16346: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L16368: {
  i_387X = arg1K0;
  h_388X = arg1K1;
  if ((i_387X < n_157X)) {
    v_389X = (long)(*((unsigned char*)((-3 + x_158X) + i_387X)));
    arg1K0 = (1 + i_387X);
    arg1K1 = (h_388X + ((long)((v_389X))));
    goto L16368;}
  else {
    RSvalS = (h_388X << 2);
    goto START;}}
 L19642: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_167X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L19651: {
  v_390X = arg0K0;
  RSvalS = ((long)v_390X);
  goto START;}
 L16521: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L16518: {
  merged_arg0K0 = 0;
  merged_arg1K1 = x_146X;
  goto raise_exception1;}
 L28783: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L16833: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L16863: {
  ShpS = start_hp_187X;
  merged_arg0K0 = 0;
  goto raise_exception;}
 L16862: {
  addr_391X = arg1K0;
  if ((addr_391X < start_hp_187X)) {
    d_393X = *((long*)addr_391X);
    if ((2 == (3 & d_393X))) {
      if ((1 == (31 & (d_393X >> 2)))) {
        v_392X = (12 + ShpS) < SlimitS;
        if (v_392X) {
          obj_394X = 7 + addr_391X;
          v_395X = (3 + ((*((long*)(-7 + x_186X))) >> 8)) >> 2;
          s_396X = *((long*)(-3 + obj_394X));
          n_397X = (*((long*)(-7 + s_396X))) >> 8;
          arg1K0 = 0;
          arg1K1 = 0;
          goto L13252;}
        else {
          goto L16863;}}
      else {
        arg1K0 = (4 + (addr_391X + (-4 & (3 + (d_393X >> 8)))));
        goto L16862;}}
    else {
      fputs("heap is in an inconsistent state.", Scurrent_output_portS);
      goto L16863;}}
  else {
    RSvalS = 13;
    goto START;}}
 L28528: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_193X;
  merged_arg1K2 = arg2_192X;
  merged_arg1K3 = RSvalS;
  goto raise_exception3;}
 L28512: {
  resume_proc_398X = arg1K0;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  SenvS = RSenvS;
  do_gc();
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  RSenvS = SenvS;
  arg1K0 = 2;
  goto L28554;}
 L28565: {
  new_399X = arg1K0;
  *(Sopen_vm_portsS + i_400X) = new_399X;
  goto L28558;}
 L28558: {
  arg1K0 = (1 + i_400X);
  goto L28554;}
 L28554: {
  i_400X = arg1K0;
  if ((i_400X == Snumber_of_portsS)) {
    fputs("This is a Scheme48 heap image file.", port_197X);
    putc(10, port_197X);
    putc(12, port_197X);
    putc(10, port_197X);
    merged_arg1K0 = 14;
    merged_arg2K1 = port_197X;
    write_number_return_tag = 0;
    goto write_number;
   write_number_return_0:
    merged_arg1K0 = 4;
    merged_arg2K1 = port_197X;
    write_number_return_tag = 1;
    goto write_number;
   write_number_return_1:
    merged_arg1K0 = (Snewspace_beginS >> 2);
    merged_arg2K1 = port_197X;
    write_number_return_tag = 2;
    goto write_number;
   write_number_return_2:
    merged_arg1K0 = (ShpS >> 2);
    merged_arg2K1 = port_197X;
    write_number_return_tag = 3;
    goto write_number;
   write_number_return_3:
    merged_arg1K0 = resume_proc_398X;
    merged_arg2K1 = port_197X;
    write_number_return_tag = 4;
    goto write_number;
   write_number_return_4:
    putc(12, port_197X);
    *((long*)ShpS) = 1;
    count_403X = (4 + ShpS) - ShpS;
    fwrite((void *)ShpS, sizeof(char), count_403X, port_197X);
    count_404X = ShpS - Snewspace_beginS;
    fwrite((void *)Snewspace_beginS, sizeof(char), count_404X, port_197X);
    size_405X = ShpS - Snewspace_beginS;
    fclose(port_197X);
    b_406X = Snewspace_beginS;
    Snewspace_beginS = Soldspace_beginS;
    Soldspace_beginS = b_406X;
    e_407X = Snewspace_endS;
    Snewspace_endS = Soldspace_endS;
    Soldspace_endS = e_407X;
    SlimitS = Ssaved_limitS;
    ShpS = Ssaved_hpS;
    arg1K0 = Snewspace_beginS;
    goto L8120;}
  else {
    port_402X = *(Sopen_vm_portsS + i_400X);
    if ((1 == port_402X)) {
      goto L28558;}
    else {
      header_401X = *((long*)(-7 + port_402X));
      if ((3 == (3 & header_401X))) {
        arg1K0 = header_401X;
        goto L28565;}
      else {
        merged_arg1K0 = port_402X;
        close_port_noisily_return_tag = 0;
        goto close_port_noisily;
       close_port_noisily_return_0:
        arg1K0 = 1;
        goto L28565;}}}}
 L16979: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L20247: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_204X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L20379: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_122X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L17067: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17064: {
  v_408X = arg0K0;
  RSvalS = ((long)v_408X);
  goto START;}
 L17212: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17209: {
  val_409X = arg1K0;
  RSvalS = val_409X;
  goto START;}
 L17204: {
  c_410X = arg1K0;
  *((long*)(5 + x_115X)) = c_410X;
  arg1K0 = c_410X;
  goto L17209;}
 L17326: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17323: {
  val_411X = arg1K0;
  RSvalS = val_411X;
  goto START;}
 L20464: {
  key_412X = arg1K0;
  RSstackS = (4 + RSstackS);
  arg2_413X = *((long*)RSstackS);
  if ((3 == (3 & arg2_413X))) {
    v_414X = 31 & ((*((long*)(-7 + arg2_413X))) >> 2);
    if ((15 == v_414X)) {
      if ((0 == (3 & RSvalS))) {
        mode_415X = RSvalS >> 2;
        arg1K0 = 0;
        goto L20653;}
      else {
        goto L20502;}}
    else {
      goto L20502;}}
  else {
    goto L20502;}}
 L20502: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_413X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L20653: {
  i_416X = arg1K0;
  if ((i_416X < Snumber_of_portsS)) {
    x_417X = *(Sopen_vm_portsS + i_416X);
    if ((1 == x_417X)) {
      arg1K0 = i_416X;
      arg1K1 = arg2_413X;
      goto L20538;}
    else {
      arg1K0 = (1 + i_416X);
      goto L20653;}}
  else {
    arg1K0 = -1;
    arg1K1 = arg2_413X;
    goto L20538;}}
 L20640: {
  i_418X = arg1K0;
  if ((i_418X < Snumber_of_portsS)) {
    x_419X = *(Sopen_vm_portsS + i_418X);
    if ((1 == x_419X)) {
      arg1K0 = i_418X;
      goto L20582;}
    else {
      arg1K0 = (1 + i_418X);
      goto L20640;}}
  else {
    arg1K0 = -1;
    goto L20582;}}
 L20610: {
  i_420X = arg1K0;
  if ((i_420X < Snumber_of_portsS)) {
    *(new_ports_425X + i_420X) = (*(Sopen_portsS + i_420X));
    *(new_vm_ports_426X + i_420X) = (*(Sopen_vm_portsS + i_420X));
    arg1K0 = (1 + i_420X);
    goto L20610;}
  else {
    free(Sopen_portsS);
    free(Sopen_vm_portsS);
    Sopen_portsS = new_ports_425X;
    Sopen_vm_portsS = new_vm_ports_426X;
    Snumber_of_portsS = new_count_423X;
    arg1K0 = old_count_424X;
    arg1K1 = filename_433X;
    goto L20538;}}
 L20604: {
  i_421X = arg1K0;
  if ((i_421X < new_count_423X)) {
    *(new_vm_ports_426X + i_421X) = 1;
    arg1K0 = (1 + i_421X);
    goto L20604;}
  else {
    arg1K0 = 0;
    goto L20610;}}
 L20593: {
  merged_arg0K0 = 0;
  merged_arg1K1 = filename_433X;
  merged_arg1K2 = (key_412X << 2);
  goto raise_exception2;}
 L20582: {
  index_422X = arg1K0;
  if ((index_422X < 0)) {
    new_count_423X = 8 + Snumber_of_portsS;
    old_count_424X = Snumber_of_portsS;
    new_ports_425X = (FILE **)malloc(sizeof(FILE *) * new_count_423X);
    new_vm_ports_426X = (long*)malloc(sizeof(long) * new_count_423X);
    if ((NULL == new_ports_425X)) {
      goto L20593;}
    else {
      if ((NULL == new_vm_ports_426X)) {
        goto L20593;}
      else {
        arg1K0 = 0;
        goto L20604;}}}
  else {
    arg1K0 = index_422X;
    arg1K1 = filename_433X;
    goto L20538;}}
 L20541: {
  port_427X = arg2K0;
  if ((NULL == port_427X)) {
    RSvalS = 1;
    goto START;}
  else {
    *((long*)ShpS) = 4118;
    ShpS = (4 + ShpS);
    new_428X = 3 + ShpS;
    ShpS = (16 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_428X)) = (mode_415X << 2);
    *((long*)(1 + new_428X)) = (index_429X << 2);
    *((long*)(5 + new_428X)) = 1;
    *((long*)(9 + new_428X)) = filename_430X;
    *(Sopen_portsS + index_429X) = port_427X;
    *(Sopen_vm_portsS + index_429X) = new_428X;
    RSvalS = new_428X;
    goto START;}}
 L20538: {
  index_429X = arg1K0;
  filename_430X = arg1K1;
  if ((index_429X < 0)) {
    merged_arg1K0 = filename_430X;
    collect_saving_temp_return_tag = 12;
    goto collect_saving_temp;
   collect_saving_temp_return_12:
    filename_433X = collect_saving_temp_return_value;
    arg1K0 = 0;
    goto L20640;}
  else {
    if ((2 == mode_415X)) {
      merged_arg1K0 = filename_430X;
      extract_string_return_tag = 1;
      goto extract_string;
     extract_string_return_1:
      spec_431X = extract_string_return_value;
      arg2K0 = (ps_open(spec_431X, "w"));
      goto L20541;}
    else {
      merged_arg1K0 = filename_430X;
      extract_string_return_tag = 2;
      goto extract_string;
     extract_string_return_2:
      spec_432X = extract_string_return_value;
      arg2K0 = (ps_open(spec_432X, "r"));
      goto L20541;}}}
 L20752: {
  if ((1 == RSvalS)) {
    *((long*)(-3 + arg2_225X)) = 273;
    goto L20777;}
  else {
    if ((17 == (255 & (*((long*)(-3 + arg2_225X)))))) {
      *((long*)(-3 + arg2_225X)) = 529;
      goto L20777;}
    else {
      goto L20777;}}}
 L20780: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_225X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L20777: {
  RSvalS = 13;
  goto START;}
 L17736: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17745: {
  v_434X = arg0K0;
  RSvalS = ((long)v_434X);
  goto START;}
 L24375: {
  merged_arg0K0 = 1;
  merged_arg1K1 = stob_229X;
  merged_arg1K2 = (((long)b_231X) << 2);
  merged_arg1K3 = arg2_228X;
  merged_arg1K4 = value_230X;
  goto raise_exception4;}
 L24583: {
  merged_arg0K0 = 1;
  merged_arg1K1 = stob_95X;
  merged_arg1K2 = (((long)b_93X) << 2);
  merged_arg1K3 = arg2_96X;
  merged_arg1K4 = value_94X;
  goto raise_exception4;}
 L24791: {
  merged_arg0K0 = 1;
  merged_arg1K1 = stob_236X;
  merged_arg1K2 = (((long)b_238X) << 2);
  merged_arg1K3 = arg2_235X;
  merged_arg1K4 = value_237X;
  goto raise_exception4;}
 L20966: {
  merged_arg0K0 = 1;
  merged_arg1K1 = arg2_239X;
  merged_arg1K2 = (((long)b_241X) << 2);
  merged_arg1K3 = index_240X;
  goto raise_exception3;}
 L21093: {
  merged_arg0K0 = 1;
  merged_arg1K1 = arg2_244X;
  merged_arg1K2 = (((long)b_246X) << 2);
  merged_arg1K3 = index_245X;
  goto raise_exception3;}
 L21218: {
  merged_arg0K0 = 1;
  merged_arg1K1 = arg2_249X;
  merged_arg1K2 = (((long)b_251X) << 2);
  merged_arg1K3 = index_250X;
  goto raise_exception3;}
 L24993: {
  merged_arg0K0 = 2;
  merged_arg1K1 = arg2_253X;
  merged_arg1K2 = (((long)b_255X) << 2);
  merged_arg1K3 = (((long)b_256X) << 2);
  merged_arg1K4 = x_254X;
  goto raise_exception4;}
 L23406: {
  merged_arg0K0 = 2;
  merged_arg1K1 = x_82X;
  merged_arg1K2 = (((long)b_81X) << 2);
  merged_arg1K3 = (((long)b_80X) << 2);
  goto raise_exception3;}
 L21310: {
  merged_arg0K0 = 1;
  merged_arg1K1 = x_258X;
  merged_arg1K2 = (((long)b_259X) << 2);
  goto raise_exception2;}
 L21396: {
  merged_arg0K0 = 1;
  merged_arg1K1 = x_78X;
  merged_arg1K2 = (((long)b_77X) << 2);
  goto raise_exception2;}
 L23512: {
  merged_arg0K0 = 1;
  merged_arg1K1 = (((long)b_266X) << 2);
  merged_arg1K2 = (len_264X << 2);
  merged_arg1K3 = init_265X;
  goto raise_exception3;}
 L23533: {
  val_435X = arg1K0;
  arg1K0 = (-1 + len_264X);
  goto L23535;}
 L23535: {
  i_436X = arg1K0;
  if ((i_436X < 0)) {
    RSvalS = val_435X;
    goto START;}
  else {
    *((unsigned char*)((-3 + val_435X) + i_436X)) = ((unsigned char)((long)(((init_438X >> 8)))));
    arg1K0 = (-1 + i_436X);
    goto L23535;}}
 L23519: {
  okayP_437X = arg3K0;
  init_438X = arg1K1;
  if (okayP_437X) {
    *((long*)ShpS) = (2 + (((len_264X << 6) + ((long)b_266X)) << 2));
    ShpS = (4 + ShpS);
    new_439X = 3 + ShpS;
    ShpS = (ShpS + (-4 & (3 + len_264X)));
    if ((0 < len_264X)) {
      *((long*)(-4 + ShpS)) = 0;
      arg1K0 = new_439X;
      goto L23533;}
    else {
      arg1K0 = new_439X;
      goto L23533;}}
  else {
    merged_arg0K0 = 1;
    merged_arg1K1 = (((long)b_266X) << 2);
    merged_arg1K2 = (len_264X << 2);
    merged_arg1K3 = init_265X;
    goto raise_exception3;}}
 L23766: {
  merged_arg0K0 = 1;
  merged_arg1K1 = (((long)b_67X) << 2);
  merged_arg1K2 = (len_69X << 2);
  merged_arg1K3 = init_68X;
  goto raise_exception3;}
 L23787: {
  val_440X = arg1K0;
  arg1K0 = (-1 + len_69X);
  goto L23789;}
 L23789: {
  i_441X = arg1K0;
  if ((i_441X < 0)) {
    RSvalS = val_440X;
    goto START;}
  else {
    *((unsigned char*)((-3 + val_440X) + i_441X)) = ((unsigned char)(init_443X >> 2));
    arg1K0 = (-1 + i_441X);
    goto L23789;}}
 L23773: {
  okayP_442X = arg3K0;
  init_443X = arg1K1;
  if (okayP_442X) {
    *((long*)ShpS) = (2 + (((len_69X << 6) + ((long)b_67X)) << 2));
    ShpS = (4 + ShpS);
    new_444X = 3 + ShpS;
    ShpS = (ShpS + (-4 & (3 + len_69X)));
    if ((0 < len_69X)) {
      *((long*)(-4 + ShpS)) = 0;
      arg1K0 = new_444X;
      goto L23787;}
    else {
      arg1K0 = new_444X;
      goto L23787;}}
  else {
    merged_arg0K0 = 1;
    merged_arg1K1 = (((long)b_67X) << 2);
    merged_arg1K2 = (len_69X << 2);
    merged_arg1K3 = init_68X;
    goto raise_exception3;}}
 L24041: {
  val_445X = arg1K0;
  arg1K0 = (-1 + len_272X);
  goto L24043;}
 L24043: {
  i_446X = arg1K0;
  if ((i_446X < 0)) {
    RSvalS = val_445X;
    goto START;}
  else {
    *((long*)((-3 + val_445X) + (i_446X << 2))) = init_448X;
    arg1K0 = (-1 + i_446X);
    goto L24043;}}
 L24027: {
  okayP_447X = arg3K0;
  init_448X = arg1K1;
  if (okayP_447X) {
    len_450X = len_272X << 2;
    *((long*)ShpS) = (2 + (((len_450X << 6) + ((long)b_274X)) << 2));
    ShpS = (4 + ShpS);
    new_449X = 3 + ShpS;
    ShpS = (ShpS + (-4 & (3 + len_450X)));
    if ((0 < len_450X)) {
      *((long*)(-4 + ShpS)) = 0;
      arg1K0 = new_449X;
      goto L24041;}
    else {
      arg1K0 = new_449X;
      goto L24041;}}
  else {
    merged_arg0K0 = 1;
    merged_arg1K1 = (((long)b_274X) << 2);
    merged_arg1K2 = (len_272X << 2);
    merged_arg1K3 = init_273X;
    goto raise_exception3;}}
 L16173: {
  b_454X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  b_453X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  len_452X = ((long)b_454X) << 2;
  *((long*)ShpS) = (2 + (((len_452X << 6) + ((long)b_453X)) << 2));
  ShpS = (4 + ShpS);
  new_451X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_452X)));
  if ((0 < len_452X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_451X;
    goto L16181;}
  else {
    arg1K0 = new_451X;
    goto L16181;}}
 L16181: {
  new_455X = arg1K0;
  if ((((long)b_454X) < 1)) {
    goto L16212;}
  else {
    *((long*)(-7 + (new_455X + (((long)b_454X) << 2)))) = RSvalS;
    arg1K0 = (-2 + ((long)b_454X));
    goto L16190;}}
 L16212: {
  RSvalS = new_455X;
  goto START;}
 L16190: {
  i_456X = arg1K0;
  if ((i_456X < 0)) {
    goto L16212;}
  else {
    RSstackS = (4 + RSstackS);
    *((long*)((-3 + new_455X) + (i_456X << 2))) = (*((long*)RSstackS));
    arg1K0 = (-1 + i_456X);
    goto L16190;}}
 L21679: {
  RSvalS = (result_57X << 2);
  goto START;}
 L21676: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_285X;
  merged_arg1K2 = x_61X;
  goto raise_exception2;}
 L22222: {
  n_457X = arg1K0;
  RSvalS = (n_457X << 2);
  goto START;}
 L22218: {
  b_458X = arg1K0;
  c_459X = a_460X % b_458X;
  if ((a_289X < 0)) {
    arg1K0 = (0 - c_459X);
    goto L22222;}
  else {
    arg1K0 = c_459X;
    goto L22222;}}
 L22216: {
  a_460X = arg1K0;
  if ((b_290X < 0)) {
    arg1K0 = (0 - b_290X);
    goto L22218;}
  else {
    arg1K0 = b_290X;
    goto L22218;}}
 L22353: {
  if ((536870911 < c_462X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_47X;
    merged_arg1K2 = x_291X;
    goto raise_exception2;}
  else {
    RSvalS = (c_462X << 2);
    goto START;}}
 L22350: {
  RSvalS = ((0 - c_462X) << 2);
  goto START;}
 L22347: {
  b_461X = arg1K0;
  c_462X = a_463X / b_461X;
  if ((a_292X < 0)) {
    if ((b_293X < 0)) {
      goto L22353;}
    else {
      goto L22350;}}
  else {
    if ((b_293X < 0)) {
      goto L22350;}
    else {
      goto L22353;}}}
 L22345: {
  a_463X = arg1K0;
  if ((b_293X < 0)) {
    arg1K0 = (0 - b_293X);
    goto L22347;}
  else {
    arg1K0 = b_293X;
    goto L22347;}}
 L22717: {
  if ((536870911 < c_472X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_44X;
    merged_arg1K2 = x_296X;
    goto raise_exception2;}
  else {
    arg1K0 = (c_472X << 2);
    goto L22680;}}
 L22693: {
  n_464X = arg1K0;
  if ((0 == (n_464X << 2))) {
    RSvalS = q_468X;
    goto START;}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_44X;
    merged_arg1K2 = x_296X;
    goto raise_exception2;}}
 L22689: {
  b_465X = arg1K0;
  c_466X = a_467X % b_465X;
  if ((a_469X < 0)) {
    arg1K0 = (0 - c_466X);
    goto L22693;}
  else {
    arg1K0 = c_466X;
    goto L22693;}}
 L22687: {
  a_467X = arg1K0;
  if ((b_470X < 0)) {
    arg1K0 = (0 - b_470X);
    goto L22689;}
  else {
    arg1K0 = b_470X;
    goto L22689;}}
 L22680: {
  q_468X = arg1K0;
  if ((0 == x_296X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_44X;
    merged_arg1K2 = x_296X;
    goto raise_exception2;}
  else {
    a_469X = arg2_44X >> 2;
    b_470X = x_296X >> 2;
    if ((a_469X < 0)) {
      arg1K0 = (0 - a_469X);
      goto L22687;}
    else {
      arg1K0 = a_469X;
      goto L22687;}}}
 L22676: {
  b_471X = arg1K0;
  c_472X = a_473X / b_471X;
  if ((a_297X < 0)) {
    if ((b_298X < 0)) {
      goto L22717;}
    else {
      arg1K0 = ((0 - c_472X) << 2);
      goto L22680;}}
  else {
    if ((b_298X < 0)) {
      arg1K0 = ((0 - c_472X) << 2);
      goto L22680;}
    else {
      goto L22717;}}}
 L22674: {
  a_473X = arg1K0;
  if ((b_298X < 0)) {
    arg1K0 = (0 - b_298X);
    goto L22676;}
  else {
    arg1K0 = b_298X;
    goto L22676;}}
 L22878: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_43X;
  merged_arg1K2 = x_299X;
  goto raise_exception2;}
 L23025: {
  if ((536870911 < c_481X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_42X;
    merged_arg1K2 = x_301X;
    goto raise_exception2;}
  else {
    RSvalS = (c_481X << 2);
    goto START;}}
 L23018: {
  if ((536870912 < c_481X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_42X;
    merged_arg1K2 = x_301X;
    goto raise_exception2;}
  else {
    RSvalS = ((0 - c_481X) << 2);
    goto START;}}
 L23015: {
  if ((536870911 < lo_c_479X)) {
    goto L23010;}
  else {
    long Sign = ((a_302X < 0) == (b_303X < 0));	/* hack */
    if (((Sign ? 8191 : 8192) < mid_c_480X)) { /*  */
      goto L23010;}
    else {
      if ((a_302X < 0)) {
        if ((b_303X < 0)) {
          goto L23025;}
        else {
          goto L23018;}}
      else {
        if ((b_303X < 0)) {
          goto L23018;}
        else {
          goto L23025;}}}}}
 L23010: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_42X;
  merged_arg1K2 = x_301X;
  goto raise_exception2;}
 L22995: {
  b_474X = arg1K0;
  /* Patched manually! */
  lo_a_475X = 65535 & a_482X;	/*  */
  lo_b_476X = 65535 & b_474X;	/*  */
  hi_a_477X = 65535 & (a_482X >> 16); /*  */
  hi_b_478X = 65535 & (b_474X >> 16); /*  */
  lo_c_479X = lo_a_475X * lo_b_476X;
  mid_c_480X = (lo_a_475X * hi_b_478X) + (lo_b_476X * hi_a_477X);
  c_481X = lo_c_479X + (mid_c_480X << 16); /*  */
  if ((0 < hi_a_477X)) {
    if ((0 < hi_b_478X)) {
      goto L23010;}
    else {
      goto L23015;}}
  else {
    goto L23015;}}
 L22993: {
  a_482X = arg1K0;
  if ((b_303X < 0)) {
    arg1K0 = (0 - b_303X);
    goto L22995;}
  else {
    arg1K0 = b_303X;
    goto L22995;}}
 L23195: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_304X;
  merged_arg1K2 = x_41X;
  goto raise_exception2;}
 L28358: {
  offset_483X = arg1K0;
  RScode_pointerS = (RScode_pointerS + offset_483X);
  goto START;}
 L24255: {
  RSvalS = proc_20X;
  merged_arg0K0 = 0;
  goto perform_application;}
 L14486: {
  RSstackS = (4 + RSstackS);
  list_484X = *((long*)RSstackS);
  a_485X = *((long*)(-3 + list_484X));
  *((long*)ShpS) = 2050;
  ShpS = (4 + ShpS);
  new_486X = 3 + ShpS;
  ShpS = (8 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)(-3 + new_486X)) = a_485X;
  *((long*)(1 + new_486X)) = 25;
  arg1K0 = (*((long*)(1 + list_484X)));
  arg1K1 = new_486X;
  goto L14496;}
 L14496: {
  l_487X = arg1K0;
  last_488X = arg1K1;
  if ((25 == l_487X)) {
    *((long*)RSstackS) = new_486X;
    RSstackS = (-4 + RSstackS);
    SnargsS = 64;
    goto L24255;}
  else {
    a_490X = *((long*)(-3 + l_487X));
    *((long*)ShpS) = 2050;
    ShpS = (4 + ShpS);
    new_489X = 3 + ShpS;
    ShpS = (8 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_489X)) = a_490X;
    *((long*)(1 + new_489X)) = 25;
    *((long*)(1 + last_488X)) = new_489X;
    arg1K0 = (*((long*)(1 + l_487X)));
    arg1K1 = new_489X;
    goto L14496;}}
 L14667: {
  i_491X = arg1K0;
  l_492X = arg1K1;
  if ((0 < i_491X)) {
    *((long*)RSstackS) = (*((long*)(-3 + l_492X)));
    RSstackS = (-4 + RSstackS);
    arg1K0 = (-1 + i_491X);
    arg1K1 = (*((long*)(1 + l_492X)));
    goto L14667;}
  else {
    if ((0 == overflow_496X)) {
      SnargsS = nargs_502X;
      goto L24255;}
    else {
      *((long*)RSstackS) = l_492X;
      RSstackS = (-4 + RSstackS);
      space_493X = 3 * overflow_496X;
      v_494X = (ShpS + (space_493X << 2)) < SlimitS;
      if (v_494X) {
        goto L14486;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 13;
        goto collect_saving_temp;
       collect_saving_temp_return_13:
        okayP_495X = (ShpS + (space_493X << 2)) < SlimitS;
        if (okayP_495X) {
          goto L14486;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L14486;}}}}}
 L14631: {
  overflow_496X = arg1K0;
  arg1K0 = (len_499X - overflow_496X);
  arg1K1 = args_503X;
  goto L14667;}
 L24289: {
  merged_arg1K0 = 25;
  merged_arg1K1 = ((long)(*((unsigned char*)RScode_pointerS)));
  pop_args_list_return_tag = 2;
  goto pop_args_list;
 pop_args_list_return_2:
  args_497X = pop_args_list_return_value;
  merged_arg0K0 = 0;
  merged_arg1K1 = proc_20X;
  merged_arg1K2 = args_497X;
  merged_arg1K3 = RSvalS;
  goto raise_exception3;}
 L24287: {
  fast_498X = arg1K0;
  len_499X = arg1K1;
  slow_500X = arg1K2;
  move_slowP_501X = arg3K3;
  if ((25 == fast_498X)) {
    args_503X = RSvalS;
    nargs_502X = len_499X + ((long)(*((unsigned char*)RScode_pointerS)));
    if ((63 < nargs_502X)) {
      arg1K0 = (-63 + nargs_502X);
      goto L14631;}
    else {
      arg1K0 = 0;
      goto L14631;}}
  else {
    if ((3 == (3 & fast_498X))) {
      v_504X = 31 & ((*((long*)(-7 + fast_498X))) >> 2);
      if ((0 == v_504X)) {
        if (move_slowP_501X) {
          if ((fast_498X == slow_500X)) {
            merged_arg1K0 = 25;
            merged_arg1K1 = ((long)(*((unsigned char*)RScode_pointerS)));
            pop_args_list_return_tag = 3;
            goto pop_args_list;
           pop_args_list_return_3:
            args_505X = pop_args_list_return_value;
            merged_arg0K0 = 0;
            merged_arg1K1 = proc_20X;
            merged_arg1K2 = args_505X;
            merged_arg1K3 = RSvalS;
            goto raise_exception3;}
          else {
            arg1K0 = (*((long*)(1 + fast_498X)));
            arg1K1 = (1 + len_499X);
            arg1K2 = (*((long*)(1 + slow_500X)));
            arg3K3 = 0;
            goto L24287;}}
        else {
          arg1K0 = (*((long*)(1 + fast_498X)));
          arg1K1 = (1 + len_499X);
          arg1K2 = slow_500X;
          arg3K3 = 1;
          goto L24287;}}
      else {
        goto L24289;}}
    else {
      goto L24289;}}}
 L15453: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L15470: {
  loc_506X = arg1K0;
  arg_507X = arg1K1;
  if ((RSstackS < arg_507X)) {
    *((long*)loc_506X) = (*((long*)arg_507X));
    arg1K0 = (-4 + loc_506X);
    arg1K1 = (-4 + arg_507X);
    goto L15470;}
  else {
    RSstackS = loc_506X;
    goto L15453;}}
 L28153: {
  v_508X = arg1K0;
  ScontS = v_508X;
  goto START;}
 L28023: {
  key_509X = arg1K0;
  merged_arg1K0 = (RScode_pointerS + ((((long)b_319X) << 8) + ((long)b_320X)));
  merged_arg1K1 = ((long)b_321X);
  merged_arg1K2 = key_509X;
  push_continuationB_return_tag = 0;
  goto push_continuationB;
 push_continuationB_return_0:
  goto START;}
 L28044: {
  v_510X = arg1K0;
  merged_arg1K0 = v_510X;
  copy_stack_into_heap_return_tag = 0;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_0:
  arg1K0 = 0;
  goto L28023;}
 L27845: {
  space_511X = arg1K0;
  v_512X = (ShpS + (space_511X << 2)) < SlimitS;
  if (v_512X) {
    arg1K0 = 0;
    goto L27847;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 14;
    goto collect_saving_temp;
   collect_saving_temp_return_14:
    okayP_513X = (ShpS + (space_511X << 2)) < SlimitS;
    if (okayP_513X) {
      arg1K0 = 0;
      goto L27847;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      arg1K0 = 0;
      goto L27847;}}}
 L27847: {
  key_514X = arg1K0;
  p_515X = RSenvS;
  if ((p_515X < Sstack_beginS)) {
    arg1K0 = RSenvS;
    goto L27849;}
  else {
    if ((Sstack_endS < p_515X)) {
      arg1K0 = RSenvS;
      goto L27849;}
    else {
      merged_arg1K0 = RSenvS;
      merged_arg1K1 = ScontS;
      merged_arg1K2 = key_514X;
      merged_arg0K3 = 0;
      save_env_in_heap_return_tag = 0;
      goto save_env_in_heap;
     save_env_in_heap_return_0:
      v_516X = save_env_in_heap_return_value;
      RSenvS = v_516X;
      arg1K0 = RSenvS;
      goto L27849;}}}
 L27849: {
  env_517X = arg1K0;
  v_518X = (12 + ShpS) < SlimitS;
  if (v_518X) {
    goto L27851;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 15;
    goto collect_saving_temp;
   collect_saving_temp_return_15:
    okayP_519X = (12 + ShpS) < SlimitS;
    if (okayP_519X) {
      goto L27851;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      goto L27851;}}}
 L27851: {
  b_520X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  a_521X = *((long*)((-3 + StemplateS) + (((long)b_520X) << 2)));
  *((long*)ShpS) = 2062 + 128;	/* *** JAR HACK *** */
  ShpS = (4 + ShpS);
  new_522X = 3 + ShpS;
  ShpS = (8 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)(-3 + new_522X)) = a_521X;
  *((long*)(1 + new_522X)) = env_517X;
  RSvalS = new_522X;
  goto START;}
 L27747: {
  env_523X = arg1K0;
  i_524X = arg1K1;
  if ((0 == i_524X)) {
    b_525X = *((unsigned char*)RScode_pointerS);
    RScode_pointerS = (1 + RScode_pointerS);
    *((long*)((-3 + env_523X) + (((long)b_525X) << 2))) = RSvalS;
    RSvalS = 13;
    goto START;}
  else {
    arg1K0 = (*((long*)(-3 + env_523X)));
    arg1K1 = (-1 + i_524X);
    goto L27747;}}
 L27429: {
  env_526X = arg1K0;
  i_527X = arg1K1;
  if ((0 == i_527X)) {
    b_528X = *((unsigned char*)RScode_pointerS);
    RScode_pointerS = (1 + RScode_pointerS);
    RSvalS = (*((long*)((-3 + env_526X) + (((long)b_528X) << 2))));
    if ((529 == RSvalS)) {
      merged_arg0K0 = 2;
      goto raise_exception;}
    else {
      goto START;}}
  else {
    arg1K0 = (*((long*)(-3 + env_526X)));
    arg1K1 = (-1 + i_527X);
    goto L27429;}}
 L27675: {
  args_529X = arg1K0;
  RSvalS = args_529X;
  SnargsS = (1 + ((long)b_337X));
  goto START;}
 L27603: {
  b_532X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  len_531X = 4 + (((long)b_532X) << 2);
  *((long*)ShpS) = (10 + (len_531X << 8));
  ShpS = (4 + ShpS);
  new_530X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_531X)));
  if ((0 < len_531X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_530X;
    goto L9932;}
  else {
    arg1K0 = new_530X;
    goto L9932;}}
 L27529: {
  b_533X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  *((long*)RSstackS) = RSenvS;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = (1034 + (((long)b_533X) << 10));
  RSstackS = (-4 + RSstackS);
  RSenvS = (11 + RSstackS);
  goto START;}
 L27566: {
  v_534X = arg1K0;
  merged_arg1K0 = v_534X;
  copy_stack_into_heap_return_tag = 1;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_1:
  goto L27529;}
 L30907: {
  merged_arg0K0 = 0;
  merged_arg1K1 = proc_176X;
  goto raise_exception1;}
 L30852: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L30727: {
  v_535X = arg0K0;
  RSvalS = ((long)v_535X);
  goto START;}
 L30705: {
  v_536X = arg0K0;
  RSvalS = ((long)v_536X);
  goto START;}
 L30681: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L30660: {
  RSvalS = 13;
  goto START;}
 L30645: {
  v_537X = arg0K0;
  RSvalS = ((long)v_537X);
  goto START;}
 L30561: {
  v_538X = arg0K0;
  RSvalS = ((long)v_538X);
  goto START;}
 L30497: {
  v_539X = arg0K0;
  RSvalS = ((long)v_539X);
  goto START;}
 L30475: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (x_62X << 2);
  goto raise_exception1;}
 L30444: {
  v_540X = arg0K0;
  RSvalS = ((long)v_540X);
  goto START;}
 L30432: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_280X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L30415: {
  v_541X = arg0K0;
  RSvalS = ((long)v_541X);
  goto START;}
 L30403: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_282X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L30388: {
  v_542X = arg0K0;
  RSvalS = ((long)v_542X);
  goto START;}
 L30274: {
  r_543X = arg1K0;
  if ((536870911 < r_543X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = x_287X;
    goto raise_exception1;}
  else {
    RSvalS = (r_543X << 2);
    goto START;}}
 L30072: {
  v_544X = arg0K0;
  RSvalS = ((long)v_544X);
  goto START;}
 L30048: {
  v_545X = arg0K0;
  RSvalS = ((long)v_545X);
  goto START;}
 L29963: {
  RSvalS = 1;
  goto START;}
 L29934: {
  RSvalS = 1;
  goto START;}
 L29905: {
  RSvalS = 1;
  goto START;}
 L29876: {
  RSvalS = 1;
  goto START;}
 L29856: {
  v_546X = arg0K0;
  RSvalS = ((long)v_546X);
  goto START;}
 L29842: {
  v_547X = arg0K0;
  RSvalS = ((long)v_547X);
  goto START;}
 L29721: {
  v_548X = arg1K0;
  ScontS = v_548X;
  SnargsS = 0;
  merged_arg0K0 = 0;
  goto perform_application;}
 L29687: {
  key_549X = arg1K0;
  merged_arg1K0 = key_549X;
  merged_arg0K1 = 2;
  preserve_continuation_return_tag = 0;
  goto preserve_continuation;
 preserve_continuation_return_0:
  v_550X = preserve_continuation_return_value;
  RSvalS = v_550X;
  goto START;}
 raise_exception: {
  args_796X = merged_arg0K0;{
  push_exception_continuationB_return_tag = 0;
  goto push_exception_continuationB;
 push_exception_continuationB_return_0:
  *((long*)RSstackS) = (((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_796X)))))) << 2);
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 0;
  goto raise;}}

 return_values: {
  nargs_794X = merged_arg1K0;{
  if ((1 == nargs_794X)) {
    RSstackS = (4 + RSstackS);
    RSvalS = (*((long*)RSstackS));
    cont_551X = ScontS;
    tem_552X = *((long*)(5 + cont_551X));
    pc_553X = *((long*)(1 + cont_551X));
    StemplateS = tem_552X;
    RScode_pointerS = ((-3 + (*((long*)(-3 + tem_552X)))) + (pc_553X >> 2));
    RSenvS = (*((long*)(9 + cont_551X)));
    ScontS = (*((long*)(-3 + cont_551X)));
    RSstackS = (9 + cont_551X);
    goto START;}
  else {
    if ((ScontS == Sbottom_of_stackS)) {
      arg1K0 = (*((long*)(-3 + Sbottom_of_stackS)));
      goto L27143;}
    else {
      arg1K0 = ScontS;
      goto L27143;}}}
 L27181: {
  args_554X = arg1K0;
  merged_arg0K0 = 0;
  merged_arg1K1 = args_554X;
  goto raise_exception1;}
 L27151: {
  if ((64 == nargs_794X)) {
    RSstackS = (4 + RSstackS);
    merged_arg1K0 = (*((long*)RSstackS));
    merged_arg1K1 = 63;
    pop_args_list_return_tag = 4;
    goto pop_args_list;
   pop_args_list_return_4:
    v_556X = pop_args_list_return_value;
    arg1K0 = v_556X;
    goto L27181;}
  else {
    merged_arg1K0 = 25;
    merged_arg1K1 = nargs_794X;
    pop_args_list_return_tag = 5;
    goto pop_args_list;
   pop_args_list_return_5:
    v_555X = pop_args_list_return_value;
    arg1K0 = v_555X;
    goto L27181;}}
 L27143: {
  cont_557X = arg1K0;
  if ((3 == (3 & cont_557X))) {
    v_559X = 31 & ((*((long*)(-7 + cont_557X))) >> 2);
    if ((8 == v_559X)) {
      op_560X = (long)(*((unsigned char*)((-3 + (*((long*)(-3 + (*((long*)(5 + cont_557X))))))) + ((*((long*)(1 + cont_557X))) >> 2))));
      if ((31 == op_560X)) {
        cont_563X = ScontS;
        tem_564X = *((long*)(5 + cont_563X));
        pc_565X = *((long*)(1 + cont_563X));
        StemplateS = tem_564X;
        RScode_pointerS = ((-3 + (*((long*)(-3 + tem_564X)))) + (pc_565X >> 2));
        RSenvS = (*((long*)(9 + cont_563X)));
        ScontS = (*((long*)(-3 + cont_563X)));
        RSstackS = (9 + cont_563X);
        goto START;}
      else {
        if ((27 == op_560X)) {
          next_562X = *((long*)(-3 + ScontS));
          if ((ScontS == Sbottom_of_stackS)) {
            *((long*)(-3 + ScontS)) = (*((long*)(-3 + next_562X)));
            goto L27040;}
          else {
            ScontS = next_562X;
            goto L27040;}}
        else {
          if ((64 == nargs_794X)) {
            RSstackS = (4 + RSstackS);
            merged_arg1K0 = (*((long*)RSstackS));
            merged_arg1K1 = 63;
            pop_args_list_return_tag = 6;
            goto pop_args_list;
           pop_args_list_return_6:
            v_558X = pop_args_list_return_value;
            arg1K0 = v_558X;
            goto L27093;}
          else {
            merged_arg1K0 = 25;
            merged_arg1K1 = nargs_794X;
            pop_args_list_return_tag = 7;
            goto pop_args_list;
           pop_args_list_return_7:
            v_561X = pop_args_list_return_value;
            arg1K0 = v_561X;
            goto L27093;}}}}
    else {
      goto L27151;}}
  else {
    goto L27151;}}
 L27040: {
  SnargsS = nargs_794X;
  RSvalS = (*((long*)(13 + cont_557X)));
  merged_arg0K0 = 0;
  goto perform_application;}
 L27093: {
  args_566X = arg1K0;
  merged_arg0K0 = 0;
  merged_arg1K1 = args_566X;
  goto raise_exception1;}}

 raise_exception1: {
  args_791X = merged_arg0K0;
  a1_792X = merged_arg1K1;{
  push_exception_continuationB_return_tag = 1;
  goto push_exception_continuationB;
 push_exception_continuationB_return_1:
  *((long*)RSstackS) = (((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_791X)))))) << 2);
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_792X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 1;
  goto raise;}}

 raise_exception2: {
  args_787X = merged_arg0K0;
  a1_788X = merged_arg1K1;
  a2_789X = merged_arg1K2;{
  push_exception_continuationB_return_tag = 2;
  goto push_exception_continuationB;
 push_exception_continuationB_return_2:
  *((long*)RSstackS) = (((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_787X)))))) << 2);
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_788X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a2_789X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 2;
  goto raise;}}

 raise_exception3: {
  args_782X = merged_arg0K0;
  a1_783X = merged_arg1K1;
  a2_784X = merged_arg1K2;
  a3_785X = merged_arg1K3;{
  push_exception_continuationB_return_tag = 3;
  goto push_exception_continuationB;
 push_exception_continuationB_return_3:
  *((long*)RSstackS) = (((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_782X)))))) << 2);
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_783X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a2_784X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a3_785X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 3;
  goto raise;}}

 raise_exception4: {
  args_776X = merged_arg0K0;
  a1_777X = merged_arg1K1;
  a2_778X = merged_arg1K2;
  a3_779X = merged_arg1K3;
  a4_780X = merged_arg1K4;{
  push_exception_continuationB_return_tag = 4;
  goto push_exception_continuationB;
 push_exception_continuationB_return_4:
  *((long*)RSstackS) = (((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_776X)))))) << 2);
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_777X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a2_778X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a3_779X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a4_780X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 4;
  goto raise;}}

 application_exception: {
  byte_args_774X = merged_arg0K0;{
  if ((64 == SnargsS)) {
    RSstackS = (4 + RSstackS);
    merged_arg1K0 = (*((long*)RSstackS));
    merged_arg1K1 = 63;
    pop_args_list_return_tag = 8;
    goto pop_args_list;
   pop_args_list_return_8:
    v_568X = pop_args_list_return_value;
    arg1K0 = v_568X;
    goto L23353;}
  else {
    merged_arg1K0 = 25;
    merged_arg1K1 = SnargsS;
    pop_args_list_return_tag = 9;
    goto pop_args_list;
   pop_args_list_return_9:
    v_567X = pop_args_list_return_value;
    arg1K0 = v_567X;
    goto L23353;}}
 L23353: {
  args_569X = arg1K0;
  a1_570X = RSvalS;
  *((long*)RSstackS) = (((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)byte_args_774X)))))) << 2);
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_570X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = args_569X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 2;
  goto raise;}}

 raise: {
  nargs_772X = merged_arg0K0;{
  SnargsS = (1 + ((long)nargs_772X));
  RSvalS = Sexception_handlerS;
  obj_572X = RSvalS;
  if ((3 == (3 & obj_572X))) {
    v_571X = 31 & ((*((long*)(-7 + obj_572X))) >> 2);
    if ((3 == v_571X)) {
      goto L15513;}
    else {
      goto L15583;}}
  else {
    goto L15583;}}
 L15513: {
  x_576X = Sstack_limitS < (-296 + RSstackS);
  if (x_576X) {
    goto L15515;}
  else {
    space_575X = 1 + ((Sstack_endS - RSstackS) >> 2);
    v_574X = (ShpS + (space_575X << 2)) < SlimitS;
    if (v_574X) {
      arg1K0 = 0;
      goto L15553;}
    else {
      merged_arg1K0 = 0;
      collect_saving_temp_return_tag = 16;
      goto collect_saving_temp;
     collect_saving_temp_return_16:
      okayP_573X = (ShpS + (space_575X << 2)) < SlimitS;
      if (okayP_573X) {
        arg1K0 = 0;
        goto L15553;}
      else {
        TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
        arg1K0 = 0;
        goto L15553;}}}}
 L15528: {
  Sstack_limitS = (296 + Sstack_beginS);
  Sexception_space_usedPS = 0;
  goto L15517;}
 L15517: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L15553: {
  v_577X = arg1K0;
  merged_arg1K0 = v_577X;
  copy_stack_into_heap_return_tag = 2;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_2:
  goto L15515;}
 L15515: {
  if (Sexception_space_usedPS) {
    x_578X = Sstack_limitS < (-296 + RSstackS);
    if (x_578X) {
      goto L15528;}
    else {
      TTerror("no space on stack to reserve exception space", 0, 0, 0, 0);
      goto L15528;}}
  else {
    goto L15517;}}
 L15583: {
  TTerror("exception handler is not a closure", 0, 0, 0, 0);
  goto L15513;}}

 perform_application: {
  bytes_consumed_770X = merged_arg0K0;{
  obj_580X = RSvalS;
  if ((3 == (3 & obj_580X))) {
    v_581X = 31 & ((*((long*)(-7 + obj_580X))) >> 2);
    if ((3 == v_581X)) {
      RSenvS = (*((long*)(1 + RSvalS)));
      tem_579X = *((long*)(-3 + RSvalS));
      StemplateS = tem_579X;
      RScode_pointerS = (-3 + (*((long*)(-3 + tem_579X))));
      if ((0 == (Spending_interruptsS & Senabled_interruptsS))) {
        goto START;}
      else {
        goto handle_interrupt;}}
    else {
      goto L15328;}}
  else {
    goto L15328;}}
 L15328: {
  merged_arg0K0 = bytes_consumed_770X;
  goto application_exception;}}

 handle_interrupt: {
{ x_585X = Sstack_limitS < (-276 + RSstackS);
  if (x_585X) {
    arg1K0 = 0;
    goto L15623;}
  else {
    space_584X = 1 + ((Sstack_endS - RSstackS) >> 2);
    v_583X = (ShpS + (space_584X << 2)) < SlimitS;
    if (v_583X) {
      arg1K0 = 0;
      goto L15854;}
    else {
      merged_arg1K0 = 0;
      collect_saving_temp_return_tag = 17;
      goto collect_saving_temp;
     collect_saving_temp_return_17:
      okayP_582X = (ShpS + (space_584X << 2)) < SlimitS;
      if (okayP_582X) {
        arg1K0 = 0;
        goto L15854;}
      else {
        TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
        arg1K0 = 0;
        goto L15854;}}}}
 L15663: {
  RSvalS = (*((long*)((-3 + Sinterrupt_handlersS) + (i_588X << 2))));
  Senabled_interruptsS = 0;
  obj_587X = RSvalS;
  if ((3 == (3 & obj_587X))) {
    v_586X = 31 & ((*((long*)(-7 + obj_587X))) >> 2);
    if ((3 == v_586X)) {
      goto L15681;}
    else {
      goto L15691;}}
  else {
    goto L15691;}}
 L15681: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L15691: {
  TTerror("interrupt handler is not a closure", 1, i_588X, 0, 0);
  goto L15681;}
 L15737: {
  TTerror("interrupt handler is not a vector", 0, 0, 0, 0);
  goto L15663;}
 L15838: {
  i_588X = arg1K0;
  m_589X = arg1K1;
  if ((0 == (n_595X & m_589X))) {
    arg1K0 = (1 + i_588X);
    arg1K1 = (m_589X << 1);
    goto L15838;}
  else {
    Spending_interruptsS = (n_595X & (~ m_589X));
    *((long*)RSstackS) = StemplateS;
    RSstackS = (-4 + RSstackS);
    *((long*)RSstackS) = RSenvS;
    RSstackS = (-4 + RSstackS);
    *((long*)RSstackS) = (SnargsS << 2);
    RSstackS = (-4 + RSstackS);
    *((long*)RSstackS) = (Senabled_interruptsS << 2);
    RSstackS = (-4 + RSstackS);
    tem_591X = Sinterrupt_templateS;
    StemplateS = tem_591X;
    RScode_pointerS = (-3 + (*((long*)(-3 + tem_591X))));
    merged_arg1K0 = RScode_pointerS;
    merged_arg1K1 = (4 + SnargsS);
    merged_arg1K2 = key_594X;
    push_continuationB_return_tag = 1;
    goto push_continuationB;
   push_continuationB_return_1:
    *((long*)RSstackS) = (Senabled_interruptsS << 2);
    RSstackS = (-4 + RSstackS);
    SnargsS = 1;
    obj_590X = Sinterrupt_handlersS;
    if ((3 == (3 & obj_590X))) {
      v_592X = 31 & ((*((long*)(-7 + obj_590X))) >> 2);
      if ((2 == v_592X)) {
        goto L15663;}
      else {
        goto L15737;}}
    else {
      goto L15737;}}}
 L15854: {
  v_593X = arg1K0;
  merged_arg1K0 = v_593X;
  copy_stack_into_heap_return_tag = 3;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_3:
  arg1K0 = 0;
  goto L15623;}
 L15623: {
  key_594X = arg1K0;
  n_595X = Spending_interruptsS & Senabled_interruptsS;
  arg1K0 = 0;
  arg1K1 = 1;
  goto L15838;}}

 copy_stack_into_heap: {
  key_767X = merged_arg1K0;{
  arg1K0 = (4 + RSstackS);
  arg1K1 = 0;
  goto L14252;}
 L14122: {
  i_596X = arg1K0;
  if ((-1 < i_596X)) {
    RSstackS = (4 + RSstackS);
    *((long*)((-3 + vec_598X) + (i_596X << 2))) = (*((long*)RSstackS));
    arg1K0 = (-1 + i_596X);
    goto L14122;}
  else {
    merged_arg1K0 = key_767X;
    merged_arg0K1 = 1;
    preserve_continuation_return_tag = 1;
    goto preserve_continuation;
   preserve_continuation_return_1:
    arg1K0 = 0;
    goto L14146;}}
 L14146: {
  i_597X = arg1K0;
  if ((i_597X < i_600X)) {
    *((long*)RSstackS) = (*((long*)((-3 + vec_598X) + (i_597X << 2))));
    RSstackS = (-4 + RSstackS);
    arg1K0 = (1 + i_597X);
    goto L14146;}
  else {
    goto copy_stack_into_heap_return;}}
 L14120: {
  vec_598X = arg1K0;
  arg1K0 = (-1 + i_600X);
  goto L14122;}
 L14252: {
  p_599X = arg1K0;
  i_600X = arg1K1;
  if ((2 == (3 & (*((long*)p_599X))))) {
    len_602X = i_600X << 2;
    *((long*)ShpS) = (10 + (len_602X << 8));
    ShpS = (4 + ShpS);
    new_601X = 3 + ShpS;
    ShpS = (ShpS + (-4 & (3 + len_602X)));
    if ((0 < len_602X)) {
      *((long*)(-4 + ShpS)) = 0;
      arg1K0 = new_601X;
      goto L14120;}
    else {
      arg1K0 = new_601X;
      goto L14120;}}
  else {
    arg1K0 = (4 + p_599X);
    arg1K1 = (1 + i_600X);
    goto L14252;}}
 copy_stack_into_heap_return:
  switch (copy_stack_into_heap_return_tag) {
  case 0: goto copy_stack_into_heap_return_0;
  case 1: goto copy_stack_into_heap_return_1;
  case 2: goto copy_stack_into_heap_return_2;
  case 3: goto copy_stack_into_heap_return_3;
  }}

 preserve_continuation: {
  key_765X = merged_arg1K0;
  reason_766X = merged_arg0K1;{
  p_603X = RSenvS;
  if ((p_603X < Sstack_beginS)) {
    goto L13897;}
  else {
    if ((Sstack_endS < p_603X)) {
      goto L13897;}
    else {
      merged_arg1K0 = RSenvS;
      merged_arg1K1 = ScontS;
      merged_arg1K2 = key_765X;
      merged_arg0K3 = reason_766X;
      save_env_in_heap_return_tag = 1;
      goto save_env_in_heap;
     save_env_in_heap_return_1:
      v_604X = save_env_in_heap_return_value;
      RSenvS = v_604X;
      goto L13897;}}}
 L13902: {
  cont_605X = arg1K0;
  previous_606X = arg1K1;
  if ((cont_605X == Sbottom_of_stackS)) {
    *((long*)(-3 + previous_606X)) = end_612X;
    cont_608X = *((long*)(-3 + Sbottom_of_stackS));
    if ((1 == cont_608X)) {
      arg1K0 = Sbottom_of_stackS;
      goto L13955;}
    else {
      merged_arg1K0 = cont_608X;
      copy_continuation_from_heap_return_tag = 2;
      goto copy_continuation_from_heap;
     copy_continuation_from_heap_return_2:
      v_607X = copy_continuation_from_heap_return_value;
      arg1K0 = v_607X;
      goto L13955;}}
  else {
    p_609X = *((long*)(9 + cont_605X));
    if ((p_609X < Sstack_beginS)) {
      goto L13924;}
    else {
      if ((Sstack_endS < p_609X)) {
        goto L13924;}
      else {
        merged_arg1K0 = (*((long*)(9 + cont_605X)));
        merged_arg1K1 = cont_605X;
        merged_arg1K2 = key_765X;
        merged_arg0K3 = reason_766X;
        save_env_in_heap_return_tag = 2;
        goto save_env_in_heap;
       save_env_in_heap_return_2:
        goto L13924;}}}}
 L13924: {
  merged_arg1K0 = cont_605X;
  merged_arg1K1 = key_765X;
  copy_stob_return_tag = 0;
  goto copy_stob;
 copy_stob_return_0:
  new_610X = copy_stob_return_value;
  *((long*)(-3 + previous_606X)) = new_610X;
  arg1K0 = (*((long*)(-3 + new_610X)));
  arg1K1 = new_610X;
  goto L13902;}
 L13955: {
  v_611X = arg1K0;
  ScontS = v_611X;
  preserve_continuation_return_value = cont_608X;
  goto preserve_continuation_return;}
 L13897: {
  end_612X = *((long*)(-3 + Sbottom_of_stackS));
  arg1K0 = ScontS;
  arg1K1 = Sbottom_of_stackS;
  goto L13902;}
 preserve_continuation_return:
  switch (preserve_continuation_return_tag) {
  case 0: goto preserve_continuation_return_0;
  case 1: goto preserve_continuation_return_1;
  }}

 copy_continuation_from_heap: {
  cont_763X = merged_arg1K0;{
  cells_613X = (3 + ((*((long*)(-7 + cont_763X))) >> 8)) >> 2;
  top_614X = (-11 + Sbottom_of_stackS) - (cells_613X << 2);
  new_cont_615X = 7 + top_614X;
  RSstackS = (-4 + top_614X);
  v_616X = (3 + ((*((long*)(-7 + cont_763X))) >> 8)) >> 2;
  from_617X = -7 + cont_763X;
  arg1K0 = from_617X;
  arg1K1 = top_614X;
  goto L12021;}
 L12021: {
  from_618X = arg1K0;
  to_619X = arg1K1;
  if ((from_618X < (4 + (from_617X + (v_616X << 2))))) {
    *((long*)to_619X) = (*((long*)from_618X));
    arg1K0 = (4 + from_618X);
    arg1K1 = (4 + to_619X);
    goto L12021;}
  else {
    *((long*)(-3 + Sbottom_of_stackS)) = (*((long*)(-3 + new_cont_615X)));
    *((long*)(-3 + new_cont_615X)) = Sbottom_of_stackS;
    copy_continuation_from_heap_return_value = new_cont_615X;
    goto copy_continuation_from_heap_return;}}
 copy_continuation_from_heap_return:
  switch (copy_continuation_from_heap_return_tag) {
  case 0: goto copy_continuation_from_heap_return_0;
  case 1: goto copy_continuation_from_heap_return_1;
  case 2: goto copy_continuation_from_heap_return_2;
  }}

 save_env_in_heap: {
  env_759X = merged_arg1K0;
  cont_760X = merged_arg1K1;
  key_761X = merged_arg1K2;{
  merged_arg1K0 = env_759X;
  merged_arg1K1 = key_761X;
  copy_stob_return_tag = 1;
  goto copy_stob;
 copy_stob_return_1:
  new_620X = copy_stob_return_value;
  *((long*)(-7 + env_759X)) = new_620X;
  arg1K0 = new_620X;
  goto L12715;}
 L12715: {
  env_621X = arg1K0;
  p_622X = *((long*)(-3 + env_621X));
  if ((p_622X < Sstack_beginS)) {
    goto L12739;}
  else {
    if ((Sstack_endS < p_622X)) {
      goto L12739;}
    else {
      env_623X = *((long*)(-3 + env_621X));
      merged_arg1K0 = env_623X;
      merged_arg1K1 = key_761X;
      copy_stob_return_tag = 2;
      goto copy_stob;
     copy_stob_return_2:
      new_624X = copy_stob_return_value;
      *((long*)(-7 + env_623X)) = new_624X;
      *((long*)(-3 + env_621X)) = new_624X;
      arg1K0 = new_624X;
      goto L12715;}}}
 L12741: {
  cont_625X = arg1K0;
  env_626X = *((long*)(9 + cont_625X));
  if ((3 == (3 & env_626X))) {
    if ((3 == (3 & (*((long*)(-7 + env_626X)))))) {
      *((long*)(9 + cont_625X)) = (*((long*)(-7 + env_626X)));
      arg1K0 = (*((long*)(-3 + cont_625X)));
      goto L12741;}
    else {
      save_env_in_heap_return_value = new_620X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap_return_value = new_620X;
    goto save_env_in_heap_return;}}
 L12739: {
  arg1K0 = cont_760X;
  goto L12741;}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  case 2: goto save_env_in_heap_return_2;
  }}

 copy_stob: {
  stob_757X = merged_arg1K0;{
  len_629X = (*((long*)(-7 + stob_757X))) >> 8;
  h_628X = *((long*)(-7 + stob_757X));
  *((long*)ShpS) = (2 + (((len_629X << 6) + (31 & (h_628X >> 2))) << 2));
  ShpS = (4 + ShpS);
  new_627X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_629X)));
  if ((0 < len_629X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_627X;
    goto L9397;}
  else {
    arg1K0 = new_627X;
    goto L9397;}}
 L9418: {
  from_630X = arg1K0;
  to_631X = arg1K1;
  if ((from_630X < (from_634X + (-4 & (3 + bytes_633X))))) {
    *((long*)to_631X) = (*((long*)from_630X));
    arg1K0 = (4 + from_630X);
    arg1K1 = (4 + to_631X);
    goto L9418;}
  else {
    copy_stob_return_value = new_632X;
    goto copy_stob_return;}}
 L9397: {
  new_632X = arg1K0;
  bytes_633X = (*((long*)(-7 + stob_757X))) >> 8;
  from_634X = -3 + stob_757X;
  arg1K0 = from_634X;
  arg1K1 = (-3 + new_632X);
  goto L9418;}
 copy_stob_return:
  switch (copy_stob_return_tag) {
  case 0: goto copy_stob_return_0;
  case 1: goto copy_stob_return_1;
  case 2: goto copy_stob_return_2;
  }}

 push_exception_continuationB: {
{ x_635X = Sstack_limitS < (-296 + RSstackS);
  if (x_635X) {
    arg1K0 = 0;
    goto L13671;}
  else {
    Sstack_limitS = Sstack_beginS;
    Sexception_space_usedPS = 1;
    x_636X = Sstack_limitS < (-296 + RSstackS);
    if (x_636X) {
      arg1K0 = 0;
      goto L13671;}
    else {
      TTerror("insufficient space on stack for exception frame", 0, 0, 0, 0);
      arg1K0 = 0;
      goto L13671;}}}
 L13683: {
  p_637X = arg1K0;
  i_638X = arg1K1;
  if ((2 == (3 & (*((long*)p_637X))))) {
    merged_arg1K0 = RScode_pointerS;
    merged_arg1K1 = i_638X;
    merged_arg1K2 = key_639X;
    push_continuationB_return_tag = 2;
    goto push_continuationB;
   push_continuationB_return_2:
    goto push_exception_continuationB_return;}
  else {
    arg1K0 = (4 + p_637X);
    arg1K1 = (1 + i_638X);
    goto L13683;}}
 L13671: {
  key_639X = arg1K0;
  arg1K0 = (4 + RSstackS);
  arg1K1 = 0;
  goto L13683;}
 push_exception_continuationB_return:
  switch (push_exception_continuationB_return_tag) {
  case 0: goto push_exception_continuationB_return_0;
  case 1: goto push_exception_continuationB_return_1;
  case 2: goto push_exception_continuationB_return_2;
  case 3: goto push_exception_continuationB_return_3;
  case 4: goto push_exception_continuationB_return_4;
  }}

 push_continuationB: {
  code_pointer_754X = merged_arg1K0;
  size_755X = merged_arg1K1;{
  pc_640X = 12 + ((code_pointer_754X - (*((long*)(-3 + StemplateS)))) << 2);
  template_641X = StemplateS;
  RSstackS = (-20 + RSstackS);
  /*** JAR HACK -- IMMUTABLE CONTINUATIONS ***/
  *((long*)(4 + RSstackS)) = ((4130 + 128) + (size_755X << 10));
  cont_642X = 11 + RSstackS;
  *((long*)(1 + cont_642X)) = pc_640X;
  *((long*)(5 + cont_642X)) = template_641X;
  *((long*)(9 + cont_642X)) = RSenvS;
  *((long*)(-3 + cont_642X)) = ScontS;
  ScontS = cont_642X;
  goto push_continuationB_return;}
 push_continuationB_return:
  switch (push_continuationB_return_tag) {
  case 0: goto push_continuationB_return_0;
  case 1: goto push_continuationB_return_1;
  case 2: goto push_continuationB_return_2;
  }}

 pop_args_list: {
  start_752X = merged_arg1K0;
  count_753X = merged_arg1K1;{
  space_643X = 3 * SnargsS;
  v_644X = (ShpS + (space_643X << 2)) < SlimitS;
  if (v_644X) {
    goto L14390;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 18;
    goto collect_saving_temp;
   collect_saving_temp_return_18:
    okayP_645X = (ShpS + (space_643X << 2)) < SlimitS;
    if (okayP_645X) {
      goto L14390;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      goto L14390;}}}
 L14392: {
  args_646X = arg1K0;
  count_647X = arg1K1;
  if ((0 < count_647X)) {
    RSstackS = (4 + RSstackS);
    a_648X = *((long*)RSstackS);
    *((long*)ShpS) = 2050;
    ShpS = (4 + ShpS);
    new_649X = 3 + ShpS;
    ShpS = (8 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_649X)) = a_648X;
    *((long*)(1 + new_649X)) = args_646X;
    arg1K0 = new_649X;
    arg1K1 = (-1 + count_647X);
    goto L14392;}
  else {
    pop_args_list_return_value = args_646X;
    goto pop_args_list_return;}}
 L14390: {
  arg1K0 = start_752X;
  arg1K1 = count_753X;
  goto L14392;}
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
  case 8: goto pop_args_list_return_8;
  case 9: goto pop_args_list_return_9;
  }}

 collect_saving_temp: {
  value_750X = merged_arg1K0;{
  b_654X = Snewspace_beginS;
  Snewspace_beginS = Soldspace_beginS;
  Soldspace_beginS = b_654X;
  e_653X = Snewspace_endS;
  Snewspace_endS = Soldspace_endS;
  Soldspace_endS = e_653X;
  Ssaved_limitS = SlimitS;
  Ssaved_hpS = ShpS;
  SlimitS = Snewspace_endS;
  ShpS = Snewspace_beginS;
  Sweak_pointer_hpS = -1;
  pc_652X = 12 + ((RScode_pointerS - (*((long*)(-3 + StemplateS)))) << 2);
  value_651X = StemplateS;
  if ((3 == (3 & value_651X))) {
    a_655X = -3 + value_651X;
    if ((a_655X < Soldspace_beginS)) {
      arg1K0 = value_651X;
      goto L13430;}
    else {
      v_656X = a_655X < Soldspace_endS;
      if (v_656X) {
        merged_arg1K0 = value_651X;
        copy_object_return_tag = 1;
        goto copy_object;
       copy_object_return_1:
        v_650X = copy_object_return_value;
        arg1K0 = v_650X;
        goto L13430;}
      else {
        arg1K0 = value_651X;
        goto L13430;}}}
  else {
    arg1K0 = value_651X;
    goto L13430;}}
 L13803: {
  i_657X = arg1K0;
  if ((i_657X == Snumber_of_portsS)) {
    x_658X = (ShpS + (Sminimum_recovered_spaceS << 2)) < SlimitS;
    if (x_658X) {
      collect_saving_temp_return_value = value_662X;
      goto collect_saving_temp_return;}
    else {
      Spending_interruptsS = (4 | Spending_interruptsS);
      collect_saving_temp_return_value = value_662X;
      goto collect_saving_temp_return;}}
  else {
    port_659X = *(Sopen_vm_portsS + i_657X);
    if ((1 == port_659X)) {
      goto L13807;}
    else {
      header_660X = *((long*)(-7 + port_659X));
      if ((3 == (3 & header_660X))) {
        arg1K0 = header_660X;
        goto L13814;}
      else {
        merged_arg1K0 = port_659X;
        close_port_noisily_return_tag = 1;
        goto close_port_noisily;
       close_port_noisily_return_1:
        arg1K0 = 1;
        goto L13814;}}}}
 L13807: {
  arg1K0 = (1 + i_657X);
  goto L13803;}
 L13814: {
  new_661X = arg1K0;
  *(Sopen_vm_portsS + i_657X) = new_661X;
  goto L13807;}
 L13752: {
  value_662X = arg1K0;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  SenvS = RSenvS;
  do_gc();
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  RSenvS = SenvS;
  Sgc_countS = (1 + Sgc_countS);
  arg1K0 = 2;
  goto L13803;}
 L13471: {
  cont_663X = arg1K0;
  last_env_664X = arg1K1;
  env_665X = *((long*)(9 + cont_663X));
  byte_count_666X = (*((long*)(-7 + cont_663X))) >> 8;
  arg1K0 = (-3 + cont_663X);
  goto L13522;}
 L13478: {
  if ((cont_663X == Sbottom_of_stackS)) {
    if ((3 == (3 & value_750X))) {
      a_668X = -3 + value_750X;
      if ((a_668X < Soldspace_beginS)) {
        arg1K0 = value_750X;
        goto L13752;}
      else {
        v_669X = a_668X < Soldspace_endS;
        if (v_669X) {
          merged_arg1K0 = value_750X;
          copy_object_return_tag = 2;
          goto copy_object;
         copy_object_return_2:
          v_667X = copy_object_return_value;
          arg1K0 = v_667X;
          goto L13752;}
        else {
          arg1K0 = value_750X;
          goto L13752;}}}
    else {
      arg1K0 = value_750X;
      goto L13752;}}
  else {
    arg1K0 = (*((long*)(-3 + cont_663X)));
    arg1K1 = env_665X;
    goto L13471;}}
 L13488: {
  env_670X = arg1K0;
  if ((env_670X < Sstack_beginS)) {
    goto L13478;}
  else {
    if ((Sstack_endS < env_670X)) {
      goto L13478;}
    else {
      byte_count_671X = (*((long*)(-7 + env_670X))) >> 8;
      arg1K0 = (-3 + env_670X);
      goto L13508;}}}
 L13508: {
  addr_672X = arg1K0;
  if ((addr_672X < ((-3 + env_670X) + (-4 & (3 + byte_count_671X))))) {
    merged_arg1K0 = addr_672X;
    copy_next_return_tag = 0;
    goto copy_next;
   copy_next_return_0:
    v_673X = copy_next_return_value;
    arg1K0 = v_673X;
    goto L13508;}
  else {
    arg1K0 = (*((long*)(-3 + env_670X)));
    goto L13488;}}
 L13522: {
  addr_674X = arg1K0;
  if ((addr_674X < ((-3 + cont_663X) + (-4 & (3 + byte_count_666X))))) {
    merged_arg1K0 = addr_674X;
    copy_next_return_tag = 1;
    goto copy_next;
   copy_next_return_1:
    v_675X = copy_next_return_value;
    arg1K0 = v_675X;
    goto L13522;}
  else {
    if ((env_665X == last_env_664X)) {
      goto L13478;}
    else {
      arg1K0 = env_665X;
      goto L13488;}}}
 L13536: {
  addr_676X = arg1K0;
  if ((addr_676X < end_680X)) {
    merged_arg1K0 = addr_676X;
    copy_next_return_tag = 2;
    goto copy_next;
   copy_next_return_2:
    v_677X = copy_next_return_value;
    arg1K0 = v_677X;
    goto L13536;}
  else {
    arg1K0 = ScontS;
    arg1K1 = 0;
    goto L13471;}}
 L13456: {
  env_678X = arg1K0;
  RSenvS = env_678X;
  start_679X = 4 + RSstackS;
  end_680X = -7 + ScontS;
  arg1K0 = start_679X;
  goto L13536;}
 L13450: {
  v_681X = arg1K0;
  Sinterrupt_templateS = v_681X;
  value_683X = RSenvS;
  if ((3 == (3 & value_683X))) {
    a_684X = -3 + value_683X;
    if ((a_684X < Soldspace_beginS)) {
      arg1K0 = value_683X;
      goto L13456;}
    else {
      v_685X = a_684X < Soldspace_endS;
      if (v_685X) {
        merged_arg1K0 = value_683X;
        copy_object_return_tag = 3;
        goto copy_object;
       copy_object_return_3:
        v_682X = copy_object_return_value;
        arg1K0 = v_682X;
        goto L13456;}
      else {
        arg1K0 = value_683X;
        goto L13456;}}}
  else {
    arg1K0 = value_683X;
    goto L13456;}}
 L13446: {
  v_686X = arg1K0;
  Sinterrupt_handlersS = v_686X;
  value_688X = Sinterrupt_templateS;
  if ((3 == (3 & value_688X))) {
    a_689X = -3 + value_688X;
    if ((a_689X < Soldspace_beginS)) {
      arg1K0 = value_688X;
      goto L13450;}
    else {
      v_690X = a_689X < Soldspace_endS;
      if (v_690X) {
        merged_arg1K0 = value_688X;
        copy_object_return_tag = 4;
        goto copy_object;
       copy_object_return_4:
        v_687X = copy_object_return_value;
        arg1K0 = v_687X;
        goto L13450;}
      else {
        arg1K0 = value_688X;
        goto L13450;}}}
  else {
    arg1K0 = value_688X;
    goto L13450;}}
 L13442: {
  v_691X = arg1K0;
  Sexception_handlerS = v_691X;
  value_693X = Sinterrupt_handlersS;
  if ((3 == (3 & value_693X))) {
    a_694X = -3 + value_693X;
    if ((a_694X < Soldspace_beginS)) {
      arg1K0 = value_693X;
      goto L13446;}
    else {
      v_695X = a_694X < Soldspace_endS;
      if (v_695X) {
        merged_arg1K0 = value_693X;
        copy_object_return_tag = 5;
        goto copy_object;
       copy_object_return_5:
        v_692X = copy_object_return_value;
        arg1K0 = v_692X;
        goto L13446;}
      else {
        arg1K0 = value_693X;
        goto L13446;}}}
  else {
    arg1K0 = value_693X;
    goto L13446;}}
 L13438: {
  v_696X = arg1K0;
  Sdynamic_stateS = v_696X;
  value_698X = Sexception_handlerS;
  if ((3 == (3 & value_698X))) {
    a_699X = -3 + value_698X;
    if ((a_699X < Soldspace_beginS)) {
      arg1K0 = value_698X;
      goto L13442;}
    else {
      v_700X = a_699X < Soldspace_endS;
      if (v_700X) {
        merged_arg1K0 = value_698X;
        copy_object_return_tag = 6;
        goto copy_object;
       copy_object_return_6:
        v_697X = copy_object_return_value;
        arg1K0 = v_697X;
        goto L13442;}
      else {
        arg1K0 = value_698X;
        goto L13442;}}}
  else {
    arg1K0 = value_698X;
    goto L13442;}}
 L13434: {
  v_701X = arg1K0;
  RSvalS = v_701X;
  value_703X = Sdynamic_stateS;
  if ((3 == (3 & value_703X))) {
    a_704X = -3 + value_703X;
    if ((a_704X < Soldspace_beginS)) {
      arg1K0 = value_703X;
      goto L13438;}
    else {
      v_705X = a_704X < Soldspace_endS;
      if (v_705X) {
        merged_arg1K0 = value_703X;
        copy_object_return_tag = 7;
        goto copy_object;
       copy_object_return_7:
        v_702X = copy_object_return_value;
        arg1K0 = v_702X;
        goto L13438;}
      else {
        arg1K0 = value_703X;
        goto L13438;}}}
  else {
    arg1K0 = value_703X;
    goto L13438;}}
 L13430: {
  tem_706X = arg1K0;
  StemplateS = tem_706X;
  RScode_pointerS = ((-3 + (*((long*)(-3 + tem_706X)))) + (pc_652X >> 2));
  value_708X = RSvalS;
  if ((3 == (3 & value_708X))) {
    a_709X = -3 + value_708X;
    if ((a_709X < Soldspace_beginS)) {
      arg1K0 = value_708X;
      goto L13434;}
    else {
      v_710X = a_709X < Soldspace_endS;
      if (v_710X) {
        merged_arg1K0 = value_708X;
        copy_object_return_tag = 8;
        goto copy_object;
       copy_object_return_8:
        v_707X = copy_object_return_value;
        arg1K0 = v_707X;
        goto L13434;}
      else {
        arg1K0 = value_708X;
        goto L13434;}}}
  else {
    arg1K0 = value_708X;
    goto L13434;}}
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
  }}

 copy_next: {
  addr_748X = merged_arg1K0;{
  next_711X = 4 + addr_748X;
  thing_712X = *((long*)addr_748X);
  if ((2 == (3 & thing_712X))) {
    if (((31 & (thing_712X >> 2)) < 15)) {
      goto L11308;}
    else {
      copy_next_return_value = (next_711X + (-4 & (3 + (thing_712X >> 8))));
      goto copy_next_return;}}
  else {
    goto L11308;}}
 L11308: {
  if ((3 == (3 & thing_712X))) {
    a_714X = -3 + thing_712X;
    if ((a_714X < Soldspace_beginS)) {
      copy_next_return_value = next_711X;
      goto copy_next_return;}
    else {
      v_715X = a_714X < Soldspace_endS;
      if (v_715X) {
        merged_arg1K0 = thing_712X;
        copy_object_return_tag = 9;
        goto copy_object;
       copy_object_return_9:
        value_713X = copy_object_return_value;
        *((long*)addr_748X) = value_713X;
        copy_next_return_value = next_711X;
        goto copy_next_return;}
      else {
        copy_next_return_value = next_711X;
        goto copy_next_return;}}}
  else {
    copy_next_return_value = next_711X;
    goto copy_next_return;}}
 copy_next_return:
  switch (copy_next_return_tag) {
  case 0: goto copy_next_return_0;
  case 1: goto copy_next_return_1;
  case 2: goto copy_next_return_2;
  }}

 copy_object: {
  thing_746X = merged_arg1K0;{
  h_717X = *((long*)(-7 + thing_746X));
  if ((3 == (3 & h_717X))) {
    copy_object_return_value = h_717X;
    goto copy_object_return;}
  else {
    if ((1070 == h_717X)) {
      SvalS = RSvalS;
      SstackS = RSstackS;
      Scode_pointerS = RScode_pointerS;
      SenvS = RSenvS;
      v_716X = copy_weak_pointer(thing_746X);
      RSvalS = SvalS;
      RSstackS = SstackS;
      RScode_pointerS = Scode_pointerS;
      RSenvS = SenvS;
      copy_object_return_value = v_716X;
      goto copy_object_return;}
    else {
      *((long*)ShpS) = h_717X;
      ShpS = (4 + ShpS);
      new_718X = 3 + ShpS;
      *((long*)(-7 + thing_746X)) = new_718X;
      x1_719X = ShpS;
      arg1K0 = (-3 + thing_746X);
      goto L9826;}}}
 L9826: {
  o_720X = arg1K0;
  if ((ShpS < (x1_719X + (-4 & (3 + (h_717X >> 8)))))) {
    *((long*)ShpS) = (*((long*)o_720X));
    ShpS = (4 + ShpS);
    arg1K0 = (4 + o_720X);
    goto L9826;}
  else {
    copy_object_return_value = new_718X;
    goto copy_object_return;}}
 copy_object_return:
  switch (copy_object_return_tag) {
  case 0: goto copy_object_return_0;
  case 1: goto copy_object_return_1;
  case 2: goto copy_object_return_2;
  case 3: goto copy_object_return_3;
  case 4: goto copy_object_return_4;
  case 5: goto copy_object_return_5;
  case 6: goto copy_object_return_6;
  case 7: goto copy_object_return_7;
  case 8: goto copy_object_return_8;
  case 9: goto copy_object_return_9;
  }}

 close_port_noisily: {
  port_744X = merged_arg1K0;{
  header_721X = *((long*)(-7 + (*((long*)(9 + port_744X)))));
  if ((3 == (3 & header_721X))) {
    goto L12329;}
  else {
    goto L12329;}}
 L12329: {
  merged_arg1K0 = port_744X;
  close_port_return_tag = 1;
  goto close_port;
 close_port_return_1:
  fputs("Port closed: ", Scurrent_output_portS);
  string_722X = *((long*)(9 + port_744X));
  port_723X = Scurrent_output_portS;
  count_724X = (*((long*)(-7 + string_722X))) >> 8;
  fwrite((void *)(-3 + string_722X), sizeof(char), count_724X, port_723X);
  putc(10, Scurrent_output_portS);
  goto close_port_noisily_return;}
 close_port_noisily_return:
  switch (close_port_noisily_return_tag) {
  case 0: goto close_port_noisily_return_0;
  case 1: goto close_port_noisily_return_1;
  }}

 close_port: {
  vm_port_743X = merged_arg1K0;{
  if (((*((long*)(1 + vm_port_743X))) < 0)) {
    goto close_port_return;}
  else {
    index_727X = (*((long*)(1 + vm_port_743X))) >> 2;
    if ((0 == index_727X)) {
      goto close_port_return;}
    else {
      if ((1 == index_727X)) {
        goto close_port_return;}
      else {
        mode_726X = (*((long*)(-3 + vm_port_743X))) >> 2;
        port_725X = *(Sopen_portsS + ((*((long*)(1 + vm_port_743X))) >> 2));
        if ((1 == mode_726X)) {
          fclose(port_725X);
          goto L10973;}
        else {
          if ((2 == mode_726X)) {
            fclose(port_725X);
            goto L10973;}
          else {
            TTerror("this shouldn't happen when closing a port", 0, 0, 0, 0);
            goto L10973;}}}}}}
 L10973: {
  *((long*)(-3 + vm_port_743X)) = 0;
  *((long*)(1 + vm_port_743X)) = -4;
  *(Sopen_vm_portsS + index_727X) = 1;
  goto close_port_return;}
 close_port_return:
  switch (close_port_return_tag) {
  case 0: goto close_port_return_0;
  case 1: goto close_port_return_1;
  }}

 write_number: {
  x_741X = merged_arg1K0;
  port_742X = merged_arg2K1;{
  if ((x_741X < 0)) {
    putc(45, port_742X);
    arg1K0 = (0 - x_741X);
    goto L29080;}
  else {
    arg1K0 = x_741X;
    goto L29080;}}
 L29080: {
  v_728X = arg1K0;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  SenvS = RSenvS;
  really_write_number(v_728X, port_742X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  RSenvS = SenvS;
  putc(10, port_742X);
  goto write_number_return;}
 write_number_return:
  switch (write_number_return_tag) {
  case 0: goto write_number_return_0;
  case 1: goto write_number_return_1;
  case 2: goto write_number_return_2;
  case 3: goto write_number_return_3;
  case 4: goto write_number_return_4;
  }}

 stob_equalP: {
  stob1_739X = merged_arg1K0;
  stob2_740X = merged_arg1K1;{
  z2_730X = *((long*)(-7 + stob2_740X));
  z1_729X = *((long*)(-7 + stob1_739X));
  if (((128 | z1_729X) == (128 | z2_730X))) {
    arg1K0 = 0;
    goto L8031;}
  else {
    stob_equalP_return_value = 0;
    goto stob_equalP_return;}}
 L8031: {
  i_731X = arg1K0;
  if ((i_731X < ((3 + (z1_729X >> 8)) >> 2))) {
    if (((*((long*)((-3 + stob1_739X) + (i_731X << 2)))) == (*((long*)((-3 + stob2_740X) + (i_731X << 2)))))) {
      arg1K0 = (1 + i_731X);
      goto L8031;}
    else {
      stob_equalP_return_value = 0;
      goto stob_equalP_return;}}
  else {
    stob_equalP_return_value = 1;
    goto stob_equalP_return;}}
 stob_equalP_return:
  switch (stob_equalP_return_tag) {
  case 0: goto stob_equalP_return_0;
  case 1: goto stob_equalP_return_1;
  }}

 extract_string: {
  string_737X = merged_arg1K0;{
  z_732X = (*((long*)(-7 + string_737X))) >> 8;
  v_733X = (unsigned char *)calloc( 1, 1 + z_732X);
  arg1K0 = 0;
  goto L10649;}
 L10649: {
  i_734X = arg1K0;
  if ((i_734X < z_732X)) {
    v_735X = (long)(*((unsigned char*)((-3 + string_737X) + i_734X)));
    *(v_733X + i_734X) = (v_735X);
    arg1K0 = (1 + i_734X);
    goto L10649;}
  else {
    extract_string_return_value = v_733X;
    goto extract_string_return;}}
 extract_string_return:
  switch (extract_string_return_tag) {
  case 0: goto extract_string_return_0;
  case 1: goto extract_string_return_1;
  case 2: goto extract_string_return_2;
  }}
}

long copy_weak_pointer(long weak_797X)
{
  extern long ShpS;
  extern long Sweak_pointer_limitS;
  extern long Sweak_pointer_hpS;
  unsigned char tempWW2427_798X;
  long old_799X;
  long new_800X;

 {tempWW2427_798X = -1 == Sweak_pointer_hpS;
  if (tempWW2427_798X) {
    goto L8493;}
  else {
    if ((Sweak_pointer_hpS < Sweak_pointer_limitS)) {
      goto L8498;}
    else {
      goto L8493;}}}
 L8493: {
  old_799X = Sweak_pointer_hpS;
  Sweak_pointer_hpS = ShpS;
  ShpS = (1024 + ShpS);
  Sweak_pointer_limitS = ShpS;
  *((long*)Sweak_pointer_hpS) = 261186;
  *((long*)(8 + Sweak_pointer_hpS)) = old_799X;
  goto L8498;}
 L8498: {
  new_800X = 7 + Sweak_pointer_hpS;
  *((long*)(4 + Sweak_pointer_hpS)) = (*((long*)(-3 + weak_797X)));
  Sweak_pointer_hpS = (8 + Sweak_pointer_hpS);
  *((long*)(-7 + weak_797X)) = new_800X;
  return(new_800X);}}

void really_write_number(long x_801X, FILE * port_802X)
{
  extern void really_write_number(long, FILE *);
  long v_803X;
  long v_804X;

 {if ((x_801X < 10)) {
    goto L29044;}
  else {
    v_803X = x_801X / 10;
    really_write_number(v_803X, port_802X);
    goto L29044;}}
 L29044: {
  v_804X = x_801X % 10;
  putc(((48 + v_804X)), port_802X);
  return;}}

long resume(unsigned char *filename_805X, unsigned char **startup_vector_806X, long startup_vector_length_807X, long initial_memory_size_808X, long initial_stack_size_809X)
{
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern long SlimitS;
  extern long Snumber_of_portsS;
  extern FILE * Scurrent_input_portS;
  extern FILE * *Sopen_portsS;
  extern long Sstack_endS;
  extern long ScontS;
  extern long Sbottom_of_stackS;
  extern long SenvS;
  extern long Sinterrupt_templateS;
  extern long StemplateS;
  extern long Sdynamic_stateS;
  extern long Sexception_handlerS;
  extern long Sinterrupt_handlersS;
  extern long Senabled_interruptsS;
  extern long Spending_interruptsS;
  extern long Scode_pointerS;
  extern long Tinterpret();
  extern long call_external_value(long, long, long);
  extern long SnargsS;
  extern long SvalS;
  extern long SstackS;
  extern long Sstack_beginS;
  extern long Sstack_limitS;
  extern unsigned char Sexception_space_usedPS;
  extern long *Sopen_vm_portsS;
  extern long Snewspace_endS;
  extern long Snewspace_beginS;
  extern FILE * Scurrent_output_portS;
  extern long ShpS;
  extern long Smemory_beginS;
  extern long Smemory_endS;
  extern long SmemoryS;
  unsigned char arg4K0;
  unsigned char arg4K1;
  long arg1K1;
  long arg1K0;
  unsigned char arg3K0;
  FILE * merged_arg2K0;
  char merged_arg0K1;
  char merged_arg0K0;
  unsigned char *merged_arg5K0;

  int enter_string_return_tag;
  long enter_string_return_value;
  int make_template_containing_ops_return_tag;
  long make_template_containing_ops_return_value;
  int read_number_return_tag;
  long read_number_return_value;
  long size_810X;
  unsigned char x_811X;
  long v_812X;
  long start_813X;
  long semisize_814X;
  unsigned char reverseP_815X;
  long start_816X;
  long v_817X;
  long ptr_818X;
  long value_819X;
  unsigned char x_820X;
  unsigned char x_821X;
  long d_822X;
  long v_823X;
  long ptr_824X;
  long descriptor_825X;
  long result_826X;
  FILE * *v_827X;
  long *v_828X;
  unsigned char tempWW2435_829X;
  long old_level_830X;
  long old_bytes_per_cell_831X;
  long cells_832X;
  long old_begin_833X;
  long cells_834X;
  long old_hp_835X;
  long startup_proc_836X;
  unsigned char Kchar_837X;
  unsigned char Kchar_838X;
  long start_839X;
  long addr_840X;
  unsigned char x_841X;
  unsigned char x_842X;
  long new_limit_843X;
  long new_hp_844X;
  long delta_845X;
  long i_846X;
  long d_847X;
  long new_848X;
  long d_849X;
  long new_850X;
  long cells_851X;
  long start_852X;
  long template_853X;
  long cont_854X;
  long v_855X;
  long len_856X;
  long new_857X;
  long *v_858X;
  long length_859X;
  long tem_860X;
  long x_861X;
  long x_862X;
  long new_863X;
  long count_864X;
  long v_865X;
  long v_866X;
  long v_867X;
  char option_868X;
  long vector_869X;
  long i_870X;
  unsigned char x_871X;
  long val_872X;
  long i_873X;
  long s_874X;
  FILE * port_875X;
  long len_876X;
  unsigned char Kchar_877X;
  unsigned char ch_878X;
  long r_879X;
  unsigned char ch_880X;
  long r_881X;
  unsigned char Kchar_882X;
  unsigned char ch_883X;
  unsigned char ch_884X;
  long new_885X;
  long new_886X;
  long new_887X;
  long z_888X;
  long i_889X;
  unsigned char c_890X;
  long s_891X;
  long c_892X;
  unsigned char *string_893X;
  long c_894X;
  char op1_895X;
  char op2_896X;
  long c_897X;
  FILE * port_898X;
 {size_810X = -4 & initial_memory_size_808X;
  if ((size_810X == (Smemory_endS - Smemory_beginS))) {
    goto L14924;}
  else {
    x_811X = 0 == Smemory_endS;
    if (x_811X) {
      goto L4604;}
    else {
      free(SmemoryS);
      goto L4604;}}}
 L14924: {
  v_812X = (Smemory_endS - Smemory_beginS) >> 2;
  start_813X = Smemory_beginS;
  semisize_814X = (-2 & (v_812X - (initial_stack_size_809X >> 2))) << 1;
  Snewspace_beginS = start_813X;
  Snewspace_endS = (Snewspace_beginS + semisize_814X);
  Soldspace_beginS = Snewspace_endS;
  Soldspace_endS = (Soldspace_beginS + semisize_814X);
  ShpS = Snewspace_beginS;
  SlimitS = Snewspace_endS;
  arg1K0 = 0;
  arg1K1 = 0;
  goto L14935;}
 L4616: {
  Smemory_beginS = SmemoryS;
  Smemory_endS = (SmemoryS + size_810X);
  goto L14924;}
 L4604: {
  SmemoryS = ((long)malloc(size_810X));
  if ((-1 == SmemoryS)) {
    TTerror("out of memory, unable to continue", 0, 0, 0, 0);
    goto L4616;}
  else {
    goto L4616;}}
 L9175: {
  reverseP_815X = arg3K0;
  fread((void *)Snewspace_beginS, sizeof(char), (old_hp_835X - old_begin_833X), port_875X);
  if (reverseP_815X) {
    start_816X = Snewspace_beginS;
    fputs("Correcting byte order of resumed image.", Scurrent_output_portS);
    putc(10, Scurrent_output_portS);
    arg1K0 = start_816X;
    goto L7649;}
  else {
    goto L9185;}}
 L7675: {
  v_817X = arg1K0;
  arg1K0 = v_817X;
  goto L7649;}
 L7649: {
  ptr_818X = arg1K0;
  x_821X = *((unsigned char*)ptr_818X);
  *((unsigned char*)ptr_818X) = ((unsigned char)((long)(*((unsigned char*)(3 + ptr_818X)))));
  *((unsigned char*)(3 + ptr_818X)) = ((unsigned char)((long)x_821X));
  x_820X = *((unsigned char*)(1 + ptr_818X));
  *((unsigned char*)(1 + ptr_818X)) = ((unsigned char)((long)(*((unsigned char*)(2 + ptr_818X)))));
  *((unsigned char*)(2 + ptr_818X)) = ((unsigned char)((long)x_820X));
  value_819X = *((long*)ptr_818X);
  if ((ptr_818X < new_hp_844X)) {
    if ((2 == (3 & value_819X))) {
      if (((31 & (value_819X >> 2)) < 15)) {
        arg1K0 = (4 + ptr_818X);
        goto L7675;}
      else {
        arg1K0 = ((4 + ptr_818X) + (-4 & (3 + (value_819X >> 8))));
        goto L7675;}}
    else {
      arg1K0 = (4 + ptr_818X);
      goto L7675;}}
  else {
    goto L9185;}}
 L7770: {
  d_822X = arg1K0;
  *((long*)ptr_824X) = d_822X;
  if ((2 == (3 & d_822X))) {
    if (((31 & (d_822X >> 2)) < 15)) {
      arg1K0 = (4 + ptr_824X);
      goto L7788;}
    else {
      arg1K0 = ((4 + ptr_824X) + (-4 & (3 + (d_822X >> 8))));
      goto L7788;}}
  else {
    arg1K0 = (4 + ptr_824X);
    goto L7788;}}
 L7788: {
  v_823X = arg1K0;
  arg1K0 = v_823X;
  goto L7760;}
 L7760: {
  ptr_824X = arg1K0;
  if ((ptr_824X < new_hp_844X)) {
    descriptor_825X = *((long*)ptr_824X);
    if ((3 == (3 & descriptor_825X))) {
      arg1K0 = (3 + ((-3 + descriptor_825X) + delta_845X));
      goto L7770;}
    else {
      arg1K0 = descriptor_825X;
      goto L7770;}}
  else {
    goto L9195;}}
 L9357: {
  result_826X = arg1K0;
  fclose(port_875X);
  v_827X = (FILE **)malloc(sizeof(FILE *) * Snumber_of_portsS);
  Sopen_portsS = v_827X;
  v_828X = (long*)malloc(sizeof(long) * Snumber_of_portsS);
  Sopen_vm_portsS = v_828X;
  tempWW2435_829X = NULL == Sopen_portsS;
  if (tempWW2435_829X) {
    goto L10160;}
  else {
    if ((NULL == Sopen_vm_portsS)) {
      goto L10160;}
    else {
      goto L10165;}}}
 L9118: {
  merged_arg2K0 = port_875X;
  read_number_return_tag = 0;
  goto read_number;
 read_number_return_0:
  old_level_830X = read_number_return_value;
  merged_arg2K0 = port_875X;
  read_number_return_tag = 1;
  goto read_number;
 read_number_return_1:
  old_bytes_per_cell_831X = read_number_return_value;
  merged_arg2K0 = port_875X;
  read_number_return_tag = 2;
  goto read_number;
 read_number_return_2:
  cells_832X = read_number_return_value;
  old_begin_833X = cells_832X << 2;
  merged_arg2K0 = port_875X;
  read_number_return_tag = 3;
  goto read_number;
 read_number_return_3:
  cells_834X = read_number_return_value;
  old_hp_835X = cells_834X << 2;
  merged_arg2K0 = port_875X;
  read_number_return_tag = 4;
  goto read_number;
 read_number_return_4:
  startup_proc_836X = read_number_return_value;
  goto L9308;}
 L9333: {
  { int TTchar;
    PS_GETC(port_875X, TTchar);
    if (EOF == TTchar) {
      TTerror("end of file while looking for page break", 0, 0, 0, 0);
      goto L9118;}
    else {
      unsigned char Kchar_837X = TTchar;
      if ((12 == ((long)Kchar_837X))) {
        goto L9118;}
      else {
        goto L9333;}}}}
 L9139: {
  if ((14 == old_level_830X)) {
    goto L9149;}
  else {
    TTerror("format of image is incompatible with this version of system", 2, old_level_830X, 14, 0);
    goto L9149;}}
 L9308: {
  { int TTchar;
    PS_GETC(port_875X, TTchar);
    if (EOF == TTchar) {
      TTerror("end of file while looking for page break", 0, 0, 0, 0);
      goto L9139;}
    else {
      unsigned char Kchar_838X = TTchar;
      if ((12 == ((long)Kchar_838X))) {
        goto L9139;}
      else {
        goto L9308;}}}}
 L9195: {
  ShpS = new_hp_844X;
  if ((3 == (3 & startup_proc_836X))) {
    arg1K0 = (3 + ((-3 + startup_proc_836X) + delta_845X));
    goto L9357;}
  else {
    arg1K0 = startup_proc_836X;
    goto L9357;}}
 L9185: {
  if ((0 == delta_845X)) {
    goto L9195;}
  else {
    start_839X = Snewspace_beginS;
    arg1K0 = start_839X;
    goto L7760;}}
 L9172: {
  fread((void *)ShpS, sizeof(char), 4, port_875X);
  if ((1 == (*((long*)ShpS)))) {
    arg3K0 = 0;
    goto L9175;}
  else {
    addr_840X = ShpS;
    x_841X = *((unsigned char*)addr_840X);
    *((unsigned char*)addr_840X) = ((unsigned char)((long)(*((unsigned char*)(3 + addr_840X)))));
    *((unsigned char*)(3 + addr_840X)) = ((unsigned char)((long)x_841X));
    x_842X = *((unsigned char*)(1 + addr_840X));
    *((unsigned char*)(1 + addr_840X)) = ((unsigned char)((long)(*((unsigned char*)(2 + addr_840X)))));
    *((unsigned char*)(2 + addr_840X)) = ((unsigned char)((long)x_842X));
    if ((1 == (*((long*)ShpS)))) {
      arg3K0 = 1;
      goto L9175;}
    else {
      TTerror("unable to correct byte order", 1, (*((long*)ShpS)), 0, 0);
      arg3K0 = 0;
      goto L9175;}}}
 L9159: {
  delta_845X = Snewspace_beginS - old_begin_833X;
  new_hp_844X = old_hp_835X + delta_845X;
  new_limit_843X = Snewspace_endS;
  if ((((22 + s_874X) + new_hp_844X) < new_limit_843X)) {
    goto L9172;}
  else {
    TTerror("heap not big enough to restore this image", 2, new_hp_844X, new_limit_843X, 0);
    goto L9172;}}
 L9149: {
  if ((4 == old_bytes_per_cell_831X)) {
    goto L9159;}
  else {
    TTerror("incompatible bytes-per-cell", 2, old_bytes_per_cell_831X, 4, 0);
    goto L9159;}}
 L9116: {
  goto L9333;}
 L10160: {
  TTerror("out of memory, unable to continue", 0, 0, 0, 0);
  goto L10165;}
 L10272: {
  i_846X = arg1K0;
  if ((i_846X < length_859X)) {
    *(v_858X + i_846X) = 1;
    arg1K0 = (1 + i_846X);
    goto L10272;}
  else {
    *(Sopen_portsS + 0) = Scurrent_input_portS;
    *(Sopen_portsS + 1) = Scurrent_output_portS;
    merged_arg5K0 = "so";
    enter_string_return_tag = 0;
    goto enter_string;
   enter_string_return_0:
    d_847X = enter_string_return_value;
    *((long*)ShpS) = 4118;
    ShpS = (4 + ShpS);
    new_848X = 3 + ShpS;
    ShpS = (16 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_848X)) = 8;
    *((long*)(1 + new_848X)) = 4;
    *((long*)(5 + new_848X)) = 1;
    *((long*)(9 + new_848X)) = d_847X;
    merged_arg5K0 = "si";
    enter_string_return_tag = 1;
    goto enter_string;
   enter_string_return_1:
    d_849X = enter_string_return_value;
    *((long*)ShpS) = 4118;
    ShpS = (4 + ShpS);
    new_850X = 3 + ShpS;
    ShpS = (16 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_850X)) = 4;
    *((long*)(1 + new_850X)) = 0;
    *((long*)(5 + new_850X)) = 1;
    *((long*)(9 + new_850X)) = d_849X;
    *(Sopen_vm_portsS + 0) = new_850X;
    *(Sopen_vm_portsS + 1) = new_848X;
    cells_851X = (Smemory_endS - Smemory_beginS) >> 2;
    start_852X = Smemory_beginS + ((cells_851X << 2) - initial_stack_size_809X);
    Sstack_beginS = start_852X;
    Sstack_endS = (start_852X + (-4 & initial_stack_size_809X));
    Sstack_limitS = Sstack_beginS;
    SstackS = (-4 + Sstack_endS);
    ScontS = 1;
    SenvS = 17;
    merged_arg0K0 = 22;
    merged_arg0K1 = 28;
    make_template_containing_ops_return_tag = 0;
    goto make_template_containing_ops;
   make_template_containing_ops_return_0:
    template_853X = make_template_containing_ops_return_value;
    SstackS = (-20 + SstackS);
    *((long*)(4 + SstackS)) = 4130;
    cont_854X = 11 + SstackS;
    *((long*)(1 + cont_854X)) = 0;
    *((long*)(5 + cont_854X)) = template_853X;
    *((long*)(9 + cont_854X)) = SenvS;
    *((long*)(-3 + cont_854X)) = ScontS;
    ScontS = cont_854X;
    Sbottom_of_stackS = ScontS;
    merged_arg0K0 = 31;
    merged_arg0K1 = 126;
    make_template_containing_ops_return_tag = 1;
    goto make_template_containing_ops;
   make_template_containing_ops_return_1:
    v_855X = make_template_containing_ops_return_value;
    Sinterrupt_templateS = v_855X;
    len_856X = startup_vector_length_807X << 2;
    *((long*)ShpS) = (10 + (len_856X << 8));
    ShpS = (4 + ShpS);
    new_857X = 3 + ShpS;
    ShpS = (ShpS + (-4 & (3 + len_856X)));
    if ((0 < len_856X)) {
      *((long*)(-4 + ShpS)) = 0;
      arg1K0 = new_857X;
      goto L14764;}
    else {
      arg1K0 = new_857X;
      goto L14764;}}}
 L10165: {
  v_858X = Sopen_vm_portsS;
  length_859X = Snumber_of_portsS;
  arg1K0 = 0;
  goto L10272;}
 L12446: {
  SenvS = 13;
  tem_860X = Sinterrupt_templateS;
  StemplateS = tem_860X;
  Scode_pointerS = (-3 + (*((long*)(-3 + tem_860X))));
  SnargsS = 13;
  SvalS = 13;
  Sdynamic_stateS = 25;
  Sexception_handlerS = 25;
  Sinterrupt_handlersS = 25;
  Senabled_interruptsS = 0;
  Spending_interruptsS = 0;
  *((long*)SstackS) = vector_869X;
  SstackS = (-4 + SstackS);
  x_861X = *(Sopen_vm_portsS + 0);
  *((long*)SstackS) = x_861X;
  SstackS = (-4 + SstackS);
  x_862X = *(Sopen_vm_portsS + 1);
  *((long*)SstackS) = x_862X;
  SstackS = (-4 + SstackS);
  *((long*)ShpS) = 578;
  ShpS = (4 + ShpS);
  new_863X = 3 + ShpS;
  ShpS = (4 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((unsigned char*)(-3 + new_863X)) = 23;
  *((unsigned char*)(-2 + new_863X)) = 3;
  Scode_pointerS = (-3 + new_863X);
  SvalS = result_826X;
  goto L14698;}
 L12491: {
  Sstack_limitS = (296 + Sstack_beginS);
  Sexception_space_usedPS = 0;
  goto L12446;}
 L14698: {
  option_868X = TTrun_machine((long)Tinterpret);
  if ((0 == ((long)option_868X))) {
    return(SvalS);}
  else {
    if ((1 == ((long)option_868X))) {
      v_867X = *((long*)(-3 + (*((long*)(1 + SvalS)))));
      v_866X = 4 + SstackS;
      v_865X = call_external_value(v_867X, SnargsS, v_866X);
      SvalS = v_865X;
      count_864X = 1 + SnargsS;
      SstackS = (SstackS - ((0 - count_864X) << 2));
      goto L14698;}
    else {
      TTerror("unkown VM return option", 1, ((long)option_868X), 0, 0);
      return(-1);}}}
 L14764: {
  vector_869X = arg1K0;
  arg1K0 = 0;
  goto L14766;}
 L14766: {
  i_870X = arg1K0;
  if ((i_870X < startup_vector_length_807X)) {
    merged_arg5K0 = (*(startup_vector_806X + i_870X));
    enter_string_return_tag = 2;
    goto enter_string;
   enter_string_return_2:
    val_872X = enter_string_return_value;
    *((long*)((-3 + vector_869X) + (i_870X << 2))) = val_872X;
    arg1K0 = (1 + i_870X);
    goto L14766;}
  else {
    SstackS = (-11 + Sbottom_of_stackS);
    *((long*)(-3 + Sbottom_of_stackS)) = 1;
    if (Sexception_space_usedPS) {
      x_871X = Sstack_limitS < (-296 + SstackS);
      if (x_871X) {
        goto L12491;}
      else {
        TTerror("no space on stack to reserve exception space", 0, 0, 0, 0);
        goto L12491;}}
    else {
      goto L12446;}}}
 L14935: {
  i_873X = arg1K0;
  s_874X = arg1K1;
  if ((i_873X < startup_vector_length_807X)) {
    len_876X = strlen((*(startup_vector_806X + i_873X)));
    arg1K0 = (1 + i_873X);
    arg1K1 = (1 + (s_874X + ((3 + len_876X) >> 2)));
    goto L14935;}
  else {
    port_875X = ps_open(filename_805X, "r");
    if ((NULL == port_875X)) {
      TTerror("Can't open heap image file", 0, 0, 0, 0);
      goto L9116;}
    else {
      goto L9116;}}}
 read_number: {
  port_898X = merged_arg2K0;{
  goto L4859;}
 L4859: {
  { int TTchar;
    PS_GETC(port_898X, TTchar);
    if (EOF == TTchar) {
      arg4K0 = -1;
      goto L4888;}
    else {
      unsigned char Kchar_877X = TTchar;
      ch_878X = Kchar_877X;
      if ((((long)ch_878X) < 48)) {
        goto L4859;}
      else {
        if ((57 < ((long)ch_878X))) {
          goto L4859;}
        else {
          arg4K0 = Kchar_877X;
          goto L4888;}}}}}
 L4890: {
  r_879X = arg1K0;
  ch_880X = arg4K1;
  r_881X = (-48 + ((long)(ch_880X))) + (10 * r_879X);
  { int TTchar;
    PS_GETC(port_898X, TTchar);
    if (EOF == TTchar) {
      read_number_return_value = r_881X;
      goto read_number_return;}
    else {
      unsigned char Kchar_882X = TTchar;
      ch_883X = Kchar_882X;
      if ((((long)ch_883X) < 48)) {
        read_number_return_value = r_881X;
        goto read_number_return;}
      else {
        if ((57 < ((long)ch_883X))) {
          read_number_return_value = r_881X;
          goto read_number_return;}
        else {
          arg1K0 = r_881X;
          arg4K1 = Kchar_882X;
          goto L4890;}}}}}
 L4888: {
  ch_884X = arg4K0;
  arg1K0 = 0;
  arg4K1 = ch_884X;
  goto L4890;}
 read_number_return:
  switch (read_number_return_tag) {
  case 0: goto read_number_return_0;
  case 1: goto read_number_return_1;
  case 2: goto read_number_return_2;
  case 3: goto read_number_return_3;
  case 4: goto read_number_return_4;
  }}

 make_template_containing_ops: {
  op1_895X = merged_arg0K0;
  op2_896X = merged_arg0K1;{
  *((long*)ShpS) = 578;
  ShpS = (4 + ShpS);
  new_885X = 3 + ShpS;
  ShpS = (4 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)ShpS) = 2090;
  ShpS = (4 + ShpS);
  new_886X = 3 + ShpS;
  ShpS = (8 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)(-3 + new_886X)) = new_885X;
  *((unsigned char*)(-3 + new_885X)) = ((unsigned char)((long)op1_895X));
  *((unsigned char*)(-2 + new_885X)) = ((unsigned char)((long)op2_896X));
  make_template_containing_ops_return_value = new_886X;
  goto make_template_containing_ops_return;}
 make_template_containing_ops_return:
  switch (make_template_containing_ops_return_tag) {
  case 0: goto make_template_containing_ops_return_0;
  case 1: goto make_template_containing_ops_return_1;
  }}

 enter_string: {
  string_893X = merged_arg5K0;{
  z_888X = strlen(string_893X);
  *((long*)ShpS) = (62 + (z_888X << 8));
  ShpS = (4 + ShpS);
  new_887X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + z_888X)));
  if ((0 < z_888X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_887X;
    goto L8837;}
  else {
    arg1K0 = new_887X;
    goto L8837;}}
 L8839: {
  i_889X = arg1K0;
  if ((i_889X < z_888X)) {
    c_890X = *(string_893X + i_889X);
    *((unsigned char*)((-3 + s_891X) + i_889X)) = ((unsigned char)((long)(c_890X)));
    arg1K0 = (1 + i_889X);
    goto L8839;}
  else {
    enter_string_return_value = s_891X;
    goto enter_string_return;}}
 L8837: {
  s_891X = arg1K0;
  arg1K0 = 0;
  goto L8839;}
 enter_string_return:
  switch (enter_string_return_tag) {
  case 0: goto enter_string_return_0;
  case 1: goto enter_string_return_1;
  case 2: goto enter_string_return_2;
  }}
}

void do_gc()
{
  extern long Sweak_pointer_hpS;
  extern long Sweak_pointer_limitS;
  extern long SlimitS;
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern long ShpS;
  extern long copy_weak_pointer(long);
  extern long Snewspace_beginS;
  long arg1K1;
  long arg1K0;
  long RShpS = ShpS;
  long value_899X;
  long scan_900X;
  long value_901X;
  long a_902X;
  unsigned char v_903X;
  long h_904X;
  long start_905X;
  long end_906X;
  long next_907X;
  long value_908X;
  long o_909X;
  long x1_910X;
  long new_911X;
  long h_912X;
  long a_913X;
  unsigned char v_914X;
  long v_915X;
  long end_916X;
  long x1_917X;
  unsigned char x_918X;
  long addr_919X;
  long next_920X;
  long thing_921X;
  long start_922X;
  long end_923X;

 {arg1K0 = Snewspace_beginS;
  goto L13347;}
 L8255: {
  arg1K0 = (8 + scan_900X);
  goto L8210;}
 L8250: {
  value_899X = arg1K0;
  *((long*)(4 + scan_900X)) = value_899X;
  goto L8255;}
 L8210: {
  scan_900X = arg1K0;
  if ((scan_900X < end_906X)) {
    *((long*)scan_900X) = 1070;
    value_901X = *((long*)(4 + scan_900X));
    if ((3 == (3 & value_901X))) {
      a_902X = -3 + value_901X;
      if ((a_902X < Soldspace_beginS)) {
        goto L8255;}
      else {
        v_903X = a_902X < Soldspace_endS;
        if (v_903X) {
          if ((3 == (3 & value_901X))) {
            h_904X = *((long*)(-7 + value_901X));
            if ((3 == (3 & h_904X))) {
              arg1K0 = h_904X;
              goto L8250;}
            else {
              arg1K0 = 1;
              goto L8250;}}
          else {
            goto L8255;}}
        else {
          goto L8255;}}}
    else {
      goto L8255;}}
  else {
    if ((-1 == next_907X)) {
      if ((end_916X < Sweak_pointer_limitS)) {
        *((long*)end_916X) = (66 + ((-4 & (-4 + (Sweak_pointer_limitS - end_916X))) << 8));
        ShpS = RShpS;
        return;}
      else {
        ShpS = RShpS;
        return;}}
    else {
      arg1K0 = (-1024 + next_907X);
      arg1K1 = next_907X;
      goto L10894;}}}
 L10894: {
  start_905X = arg1K0;
  end_906X = arg1K1;
  next_907X = *((long*)(8 + start_905X));
  arg1K0 = start_905X;
  goto L8210;}
 L31406: {
  value_908X = arg1K0;
  *((long*)addr_919X) = value_908X;
  arg1K0 = next_920X;
  goto L13411;}
 L31446: {
  o_909X = arg1K0;
  if ((RShpS < (x1_910X + (-4 & (3 + (h_912X >> 8)))))) {
    *((long*)RShpS) = (*((long*)o_909X));
    RShpS = (4 + RShpS);
    arg1K0 = (4 + o_909X);
    goto L31446;}
  else {
    arg1K0 = new_911X;
    goto L31406;}}
 L31396: {
  if ((3 == (3 & thing_921X))) {
    a_913X = -3 + thing_921X;
    if ((a_913X < Soldspace_beginS)) {
      arg1K0 = next_920X;
      goto L13411;}
    else {
      v_914X = a_913X < Soldspace_endS;
      if (v_914X) {
        h_912X = *((long*)(-7 + thing_921X));
        if ((3 == (3 & h_912X))) {
          arg1K0 = h_912X;
          goto L31406;}
        else {
          if ((1070 == h_912X)) {
            ShpS = RShpS;
            v_915X = copy_weak_pointer(thing_921X);
            RShpS = ShpS;
            arg1K0 = v_915X;
            goto L31406;}
          else {
            *((long*)RShpS) = h_912X;
            RShpS = (4 + RShpS);
            new_911X = 3 + RShpS;
            *((long*)(-7 + thing_921X)) = new_911X;
            x1_910X = RShpS;
            arg1K0 = (-3 + thing_921X);
            goto L31446;}}}
      else {
        arg1K0 = next_920X;
        goto L13411;}}}
  else {
    arg1K0 = next_920X;
    goto L13411;}}
 L13373: {
  x_918X = -1 == Sweak_pointer_hpS;
  if (x_918X) {
    ShpS = RShpS;
    return;}
  else {
    x1_917X = Sweak_pointer_limitS;
    end_916X = Sweak_pointer_hpS;
    arg1K0 = (-1024 + x1_917X);
    arg1K1 = end_916X;
    goto L10894;}}
 L13411: {
  addr_919X = arg1K0;
  if ((addr_919X < end_923X)) {
    next_920X = 4 + addr_919X;
    thing_921X = *((long*)addr_919X);
    if ((2 == (3 & thing_921X))) {
      if (((31 & (thing_921X >> 2)) < 15)) {
        goto L31396;}
      else {
        arg1K0 = (next_920X + (-4 & (3 + (thing_921X >> 8))));
        goto L13411;}}
    else {
      goto L31396;}}
  else {
    if ((RShpS < SlimitS)) {
      if ((end_923X < RShpS)) {
        arg1K0 = end_923X;
        goto L13347;}
      else {
        goto L13373;}}
    else {
      TTerror("GC error: ran out of space in new heap", 0, 0, 0, 0);
      goto L13373;}}}
 L13347: {
  start_922X = arg1K0;
  end_923X = RShpS;
  arg1K0 = start_922X;
  goto L13411;}}
scheme48_init()
{
Sminimum_recovered_spaceS = 0;
Sexception_space_usedPS = 1;
Snumber_of_portsS = 100;
Sweak_pointer_limitS = 0;
Sweak_pointer_hpS = -1;
Ssaved_limitS = 0;
Ssaved_hpS = 0;
Sgc_countS = 0;
SlimitS = 0;
ShpS = 0;
Smemory_endS = 0;
Smemory_beginS = 0;
Scurrent_output_portS = stdout;
Scurrent_input_portS = stdin;
}
