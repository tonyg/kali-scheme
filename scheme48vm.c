#include <stdio.h>
#include "prescheme.h"
long goto_arg1K0;
long goto_arg1K4;
long goto_arg1K3;
long goto_arg1K2;
long goto_arg1K1;
unsigned char goto_arg0K0;
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
unsigned char Simpure_area_countS;
unsigned char Spure_area_countS;
long *Simpure_sizesS;
long *Spure_sizesS;
long *Simpure_areasS;
long *Spure_areasS;
long Soldspace_endS;
long Soldspace_beginS;
long Snewspace_endS;
long Snewspace_beginS;
long SlimitS;
long ShpS;
FILE * Scurrent_output_portS;
FILE * Scurrent_input_portS;

unsigned char hoistD0(long start_0X, long end_1X, long type_2X)
{
  extern FILE * Scurrent_output_portS;
  extern long SlimitS;
  extern long ShpS;
  long arg1K0;
  long addr_3X;
  long d_4X;

 {arg1K0 = start_0X;
  goto L10137;}
 L10137: {
  addr_3X = arg1K0;
  if ((addr_3X < end_1X)) {
    d_4X = *((long *) addr_3X);
    if ((2 == (3 & d_4X))) {
      if ((type_2X == (31 & (((d_4X)>>2))))) {
        if (((16 + (ShpS)) < (SlimitS))) {
          *((long *) (ShpS)) = (7 + addr_3X);
          ShpS = (4 + (ShpS));
          arg1K0 = (4 + (addr_3X + (-4 & (3 + ((long)(((unsigned long)d_4X)>>8))))));
          goto L10137;}
        else {
          return(0);}}
      else {
        arg1K0 = (4 + (addr_3X + (-4 & (3 + ((long)(((unsigned long)d_4X)>>8))))));
        goto L10137;}}
    else {
      fputs("heap is in an inconsistent state.", (Scurrent_output_portS));
      return(0);}}
  else {
    return(1);}}}

unsigned char hoistD1(long start_5X, long end_6X, long x_7X)
{
  extern FILE * Scurrent_output_portS;
  extern long SlimitS;
  extern long ShpS;
  long arg1K1;
  long arg1K0;
  long i_8X;
  long h_9X;
  long bucket_10X;
  long index_11X;
  long b_12X;
  long new_13X;
  long addr_14X;
  long d_15X;
  long obj_16X;
  long v_17X;
  long s_18X;
  long n_19X;

 {arg1K0 = start_5X;
  goto L17309;}
 L13654: {
  i_8X = arg1K0;
  h_9X = arg1K1;
  if ((i_8X < n_19X)) {
    arg1K0 = (1 + i_8X);
    arg1K1 = (h_9X + ((long)((((long)(*((unsigned char *) ((-3 + s_18X) + i_8X))))))));
    goto L13654;}
  else {
    index_11X = h_9X & (-1 + v_17X);
    bucket_10X = *((long *) ((-3 + x_7X) + (((index_11X)<<2))));
    arg1K0 = bucket_10X;
    goto L13552;}}
 L13552: {
  b_12X = arg1K0;
  if ((25 == b_12X)) {
    *((long *) (ShpS)) = 2050;
    ShpS = (4 + (ShpS));
    new_13X = 3 + (ShpS);
    ShpS = (8 + (ShpS));
    *((long *) (-4 + (ShpS))) = 0;
    *((long *) (-3 + new_13X)) = obj_16X;
    *((long *) (1 + new_13X)) = bucket_10X;
    *((long *) ((-3 + x_7X) + (((index_11X)<<2)))) = new_13X;
    arg1K0 = (4 + (addr_14X + (-4 & (3 + ((long)(((unsigned long)d_15X)>>8))))));
    goto L17309;}
  else {
    if ((obj_16X == (*((long *) (-3 + b_12X))))) {
      arg1K0 = (4 + (addr_14X + (-4 & (3 + ((long)(((unsigned long)d_15X)>>8))))));
      goto L17309;}
    else {
      arg1K0 = (*((long *) (1 + b_12X)));
      goto L13552;}}}
 L17309: {
  addr_14X = arg1K0;
  if ((addr_14X < end_6X)) {
    d_15X = *((long *) addr_14X);
    if ((2 == (3 & d_15X))) {
      if ((1 == (31 & (((d_15X)>>2))))) {
        if (((12 + (ShpS)) < (SlimitS))) {
          obj_16X = 7 + addr_14X;
          v_17X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + x_7X))))>>8))))>>2);
          s_18X = *((long *) (-3 + obj_16X));
          n_19X = -1 + ((long)(((unsigned long)(*((long *) (-7 + s_18X))))>>8));
          arg1K0 = 0;
          arg1K1 = 0;
          goto L13654;}
        else {
          return(0);}}
      else {
        arg1K0 = (4 + (addr_14X + (-4 & (3 + ((long)(((unsigned long)d_15X)>>8))))));
        goto L17309;}}
    else {
      fputs("heap is in an inconsistent state.", (Scurrent_output_portS));
      return(0);}}
  else {
    return(1);}}}

long Tinterpret()
{
  extern void really_write_number(long, FILE *);
  extern FILE * Scurrent_output_portS;
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
  extern long *Simpure_sizesS;
  extern long *Simpure_areasS;
  extern unsigned char hoistD0(long, long, long);
  extern long *Spure_sizesS;
  extern long *Spure_areasS;
  extern unsigned char hoistD1(long, long, long);
  extern long Sgc_countS;
  extern long Sminimum_recovered_spaceS;
  extern unsigned char Spure_area_countS;
  extern unsigned char Simpure_area_countS;
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
  unsigned char arg0K0;
  long arg1K2;
  long arg1K1;
  long arg1K0;
  unsigned char merged_arg0K0;
  unsigned char merged_arg0K1;
  unsigned char merged_arg0K3;
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
  int close_untraced_portsB_return_tag;
  unsigned char close_untraced_portsB_return_value;
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
  unsigned char b_20X;
  unsigned char b_21X;
  long space_22X;
  long v_23X;
  unsigned char b_24X;
  long env_25X;
  unsigned char b_26X;
  long location_27X;
  unsigned char b_28X;
  long location_29X;
  unsigned char b_30X;
  unsigned char b_31X;
  long space_32X;
  long v_33X;
  long cont_34X;
  long list_35X;
  long proc_36X;
  long v_37X;
  long cont_38X;
  long pc_39X;
  long tem_40X;
  long cont_41X;
  long tem_42X;
  unsigned char b_43X;
  long tem_44X;
  unsigned char b_45X;
  unsigned char b_46X;
  unsigned char b_47X;
  unsigned char b_48X;
  unsigned char b_49X;
  long val_50X;
  long z_51X;
  long x_52X;
  long arg2_53X;
  long arg2_54X;
  long arg2_55X;
  long x_56X;
  long arg2_57X;
  long arg2_58X;
  long arg2_59X;
  long x_60X;
  long arg2_61X;
  long arg2_62X;
  long arg2_63X;
  long arg2_64X;
  long arg2_65X;
  long arg2_66X;
  long x_67X;
  long result_68X;
  long x_69X;
  long value_70X;
  long count_71X;
  long x_72X;
  long x_73X;
  unsigned char b_74X;
  long x_75X;
  long space_76X;
  unsigned char b_77X;
  unsigned char b_78X;
  long x_79X;
  long arg2_80X;
  long temp_81X;
  long size_82X;
  unsigned char b_83X;
  long len_84X;
  long x_85X;
  long len_86X;
  unsigned char b_87X;
  long index_88X;
  long len_89X;
  unsigned char b_90X;
  long index_91X;
  long x_92X;
  long size_93X;
  long len_94X;
  long init_95X;
  long obj_96X;
  long arg2_97X;
  long arg3_98X;
  long arg2_99X;
  long obj_100X;
  long arg2_101X;
  long descriptor_102X;
  long x_103X;
  long obj_104X;
  long x_105X;
  long p_106X;
  unsigned char Kchar_107X;
  long c_108X;
  long x_109X;
  unsigned char b_110X;
  long p_111X;
  long arg2_112X;
  long p_113X;
  long a_114X;
  long old_115X;
  long arg2_116X;
  unsigned char v_117X;
  long start_hp_118X;
  long x_119X;
  long obj_120X;
  unsigned char v_121X;
  long start_hp_122X;
  long type_123X;
  long obj_124X;
  long obj_125X;
  long tem_126X;
  unsigned char v_127X;
  long name_128X;
  long value_129X;
  long x_130X;
  long obj_131X;
  long n_132X;
  long n_133X;
  long arg2_134X;
  long return_value_135X;
  long key_136X;
  long x_137X;
  long n_138X;
  long x_139X;
  long obj_140X;
  unsigned char b_141X;
  long space_142X;
  long arg2_143X;
  long obj_144X;
  unsigned char x_145X;
  long arg2_146X;
  long arg2_147X;
  long x_148X;
  long key_149X;
  long n_150X;
  long proc_151X;
  long p_152X;
  long n_153X;
  long temp_154X;
  long *areas_155X;
  long *sizes_156X;
  unsigned char count_157X;
  long *areas_158X;
  long *sizes_159X;
  unsigned char count_160X;
  long x_161X;
  long key_162X;
  long arg2_163X;
  long arg3_164X;
  long obj_165X;
  long x_166X;
  unsigned char *spec_167X;
  FILE * port_168X;
  long b_169X;
  long e_170X;
  long v_171X;
  long arg2_172X;
  long p_173X;
  long p_174X;
  long x_175X;
  long x_176X;
  long p_177X;
  FILE * v_178X;
  long x_179X;
  long c_180X;
  unsigned char Kchar_181X;
  long x_182X;
  long x_183X;
  long obj_184X;
  long x_185X;
  long arg2_186X;
  long x_187X;
  long arg2_188X;
  long arg3_189X;
  unsigned char Kchar_190X;
  long index_191X;
  long len_192X;
  long index_193X;
  long len_194X;
  long arg2_195X;
  unsigned char init_196X;
  long len_197X;
  long size_198X;
  long value_199X;
  long index_200X;
  long len_201X;
  long index_202X;
  long len_203X;
  long arg2_204X;
  long arg2_205X;
  long arg3_206X;
  long arg2_207X;
  long arg2_208X;
  long x_209X;
  unsigned char b_210X;
  unsigned char b_211X;
  long x_212X;
  unsigned char b_213X;
  long x_214X;
  long arg2_215X;
  long x_216X;
  long arg2_217X;
  long x_218X;
  long x_219X;
  long arg2_220X;
  long x_221X;
  long x_222X;
  long x_223X;
  long a_224X;
  long b_225X;
  long x_226X;
  long a_227X;
  long b_228X;
  long x_229X;
  long arg2_230X;
  long x_231X;
  long a_232X;
  long b_233X;
  long x_234X;
  long z_235X;
  long x_236X;
  long a_237X;
  long b_238X;
  long arg2_239X;
  long x_240X;
  long x_241X;
  long x_242X;
  long x_243X;
  long x_244X;
  long arg2_245X;
  long x_246X;
  unsigned char b_247X;
  unsigned char b_248X;
  unsigned char b_249X;
  unsigned char b_250X;
  long consumer_251X;
  long start_arg_252X;
  long start_loc_253X;
  unsigned char b_254X;
  unsigned char b_255X;
  unsigned char b_256X;
  long space_257X;
  unsigned char b_258X;
  long p_259X;
  long env_260X;
  unsigned char b_261X;
  long env_262X;
  unsigned char b_263X;
  long env_264X;
  unsigned char b_265X;
  unsigned char b_266X;
  long env_267X;
  unsigned char b_268X;
  long v_269X;
  unsigned char count_270X;
  long space_271X;
  long vector_272X;
  long i_273X;
  unsigned char v_274X;
  long *areas_275X;
  long *sizes_276X;
  unsigned char count_277X;
  long i_278X;
  unsigned char v_279X;
  long h_280X;
  long addr_281X;
  long h_282X;
  long d_283X;
  long sym_284X;
  long v_285X;
  long val_286X;
  long i_287X;
  long h_288X;
  long bucket_289X;
  long index_290X;
  long new_291X;
  long new_292X;
  long b_293X;
  unsigned char v_294X;
  long new_295X;
  long stob_296X;
  long from_297X;
  long from_298X;
  long to_299X;
  long arg2_300X;
  long obj_301X;
  long table_302X;
  long v_303X;
  long n_304X;
  long arg2_305X;
  long n_306X;
  long new_307X;
  long len_308X;
  long obj_309X;
  long l_310X;
  long i_311X;
  long i_312X;
  long h_313X;
  unsigned char v_314X;
  long i_315X;
  unsigned char v_316X;
  long resume_proc_317X;
  long size_318X;
  long b_319X;
  long e_320X;
  unsigned char v_321X;
  long val_322X;
  long c_323X;
  long val_324X;
  long key_325X;
  long arg2_326X;
  long mode_327X;
  long i_328X;
  long i_329X;
  long i_330X;
  long i_331X;
  long index_332X;
  long new_count_333X;
  long old_count_334X;
  FILE * *new_ports_335X;
  long *new_vm_ports_336X;
  FILE * port_337X;
  long new_338X;
  long index_339X;
  long filename_340X;
  unsigned char *spec_341X;
  unsigned char *spec_342X;
  long filename_343X;
  unsigned char v_344X;
  long new_345X;
  long len_346X;
  long string_347X;
  long i_348X;
  long new_349X;
  long code_vector_350X;
  long i_351X;
  long val_352X;
  long i_353X;
  unsigned char okayP_354X;
  long init_355X;
  long new_356X;
  long len_357X;
  long new_358X;
  long len_359X;
  unsigned char b_360X;
  unsigned char b_361X;
  long new_362X;
  long i_363X;
  long n_364X;
  long b_365X;
  long c_366X;
  long a_367X;
  long b_368X;
  long c_369X;
  long a_370X;
  long n_371X;
  long b_372X;
  long c_373X;
  long a_374X;
  long q_375X;
  long a_376X;
  long b_377X;
  long b_378X;
  long c_379X;
  long a_380X;
  long b_381X;
  long lo_a_382X;
  long lo_b_383X;
  long hi_a_384X;
  long hi_b_385X;
  long lo_c_386X;
  long mid_c_387X;
  long c_388X;
  long a_389X;
  long offset_390X;
  long list_391X;
  long a_392X;
  long new_393X;
  long l_394X;
  long last_395X;
  long new_396X;
  long a_397X;
  long i_398X;
  long l_399X;
  long space_400X;
  long overflow_401X;
  long args_402X;
  long fast_403X;
  long len_404X;
  long slow_405X;
  unsigned char move_slowP_406X;
  long nargs_407X;
  long args_408X;
  long args_409X;
  long loc_410X;
  long arg_411X;
  long v_412X;
  long key_413X;
  long v_414X;
  long space_415X;
  long key_416X;
  long p_417X;
  long v_418X;
  long env_419X;
  unsigned char b_420X;
  long a_421X;
  long new_422X;
  long env_423X;
  long i_424X;
  unsigned char b_425X;
  long env_426X;
  long i_427X;
  unsigned char b_428X;
  long args_429X;
  long new_430X;
  long len_431X;
  unsigned char b_432X;
  unsigned char b_433X;
  long v_434X;
  unsigned char v_435X;
  unsigned char v_436X;
  unsigned char v_437X;
  unsigned char v_438X;
  unsigned char v_439X;
  unsigned char v_440X;
  unsigned char v_441X;
  unsigned char v_442X;
  long r_443X;
  unsigned char v_444X;
  unsigned char v_445X;
  unsigned char v_446X;
  unsigned char v_447X;
  long v_448X;
  long key_449X;
  long v_450X;
  long cont_451X;
  long tem_452X;
  long pc_453X;
  long args_454X;
  long v_455X;
  long v_456X;
  long cont_457X;
  long v_458X;
  unsigned char next_op_459X;
  long v_460X;
  long next_461X;
  long cont_462X;
  long tem_463X;
  long pc_464X;
  long args_465X;
  long v_466X;
  long v_467X;
  long args_468X;
  long a1_469X;
  long obj_470X;
  long space_471X;
  long v_472X;
  unsigned char x_473X;
  long tem_474X;
  long obj_475X;
  long space_476X;
  long obj_477X;
  long i_478X;
  long m_479X;
  long obj_480X;
  long tem_481X;
  long v_482X;
  long key_483X;
  long n_484X;
  long i_485X;
  long i_486X;
  long vec_487X;
  long p_488X;
  long i_489X;
  long new_490X;
  long len_491X;
  long p_492X;
  long v_493X;
  long cont_494X;
  long previous_495X;
  long v_496X;
  long cont_497X;
  long p_498X;
  long new_499X;
  long v_500X;
  long end_501X;
  long top_502X;
  long new_cont_503X;
  long v_504X;
  long from_505X;
  long from_506X;
  long to_507X;
  long new_508X;
  long env_509X;
  long p_510X;
  long env_511X;
  long new_512X;
  long cont_513X;
  long env_514X;
  long new_515X;
  long len_516X;
  long from_517X;
  long to_518X;
  long new_519X;
  long bytes_520X;
  long from_521X;
  long p_522X;
  long i_523X;
  long key_524X;
  long pc_525X;
  long template_526X;
  long cont_527X;
  long space_528X;
  long args_529X;
  long count_530X;
  long a_531X;
  long new_532X;
  long v_533X;
  long value_534X;
  long pc_535X;
  long e_536X;
  long b_537X;
  long a_538X;
  long value_539X;
  long i_540X;
  long a_541X;
  long v_542X;
  long start_543X;
  long end_544X;
  long addr_545X;
  long v_546X;
  long cont_547X;
  long last_env_548X;
  long env_549X;
  long end_550X;
  long *areas_551X;
  long *sizes_552X;
  unsigned char count_553X;
  long env_554X;
  long end_555X;
  long addr_556X;
  long v_557X;
  long addr_558X;
  long v_559X;
  long addr_560X;
  long v_561X;
  long env_562X;
  long start_563X;
  long end_564X;
  long v_565X;
  long v_566X;
  long value_567X;
  long a_568X;
  long v_569X;
  long v_570X;
  long value_571X;
  long a_572X;
  long v_573X;
  long v_574X;
  long value_575X;
  long a_576X;
  long v_577X;
  long v_578X;
  long value_579X;
  long a_580X;
  long v_581X;
  long v_582X;
  long value_583X;
  long a_584X;
  long tem_585X;
  long v_586X;
  long value_587X;
  long a_588X;
  long next_589X;
  long thing_590X;
  long value_591X;
  long a_592X;
  long a_593X;
  long h_594X;
  long descriptor_595X;
  long v_596X;
  long o_597X;
  long new_598X;
  long x1_599X;
  long i_600X;
  long port_601X;
  long header_602X;
  long header_603X;
  long new_604X;
  long string_605X;
  FILE * port_606X;
  long mode_607X;
  long index_608X;

  long v_609X;
  long z1_610X;
  long z2_611X;
  long i_612X;
  long z_613X;
  unsigned char *v_614X;
  long i_615X;
  unsigned char *c_616X;
  long string_617X;
  unsigned char c_618X;
  long stob1_619X;
  long stob2_620X;
  long x_621X;
  FILE * port_622X;
  long vm_port_623X;
  unsigned char c_624X;
  long c_625X;
  long thing_626X;
  long c_627X;
  long addr_628X;
  long c_629X;
  long value_630X;
  long c_631X;
  long start_632X;
  long count_633X;
  long code_pointer_634X;
  long size_635X;
  long c_636X;
  long old_stob_637X;
  long c_638X;
  long env_639X;
  long cont_640X;
  long key_641X;
  long c_642X;
  long cont_643X;
  long c_644X;
  long key_645X;
  unsigned char reason_646X;
  long key_647X;
  unsigned char c_648X;
  unsigned char c_649X;
  unsigned char bytes_consumed_650X;
  unsigned char c_651X;
  unsigned char nargs_652X;
  unsigned char c_653X;
  unsigned char byte_args_654X;
  unsigned char c_655X;
  unsigned char args_656X;
  long a1_657X;
  long a2_658X;
  long a3_659X;
  long a4_660X;
  unsigned char c_661X;
  unsigned char args_662X;
  long a1_663X;
  long a2_664X;
  long a3_665X;
  unsigned char c_666X;
  unsigned char args_667X;
  long a1_668X;
  long a2_669X;
  unsigned char c_670X;
  unsigned char args_671X;
  long a1_672X;
  unsigned char c_673X;
  long nargs_674X;
  unsigned char c_675X;
  unsigned char args_676X;
 START: {
  b_141X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  switch (b_141X) {
    case 0 : {
      b_20X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((SnargsS) == ((long)b_20X))) {
        goto START;}
      else {
        merged_arg0K0 = 1;
        goto application_exception;}}
      break;
    case 1 : {
      b_21X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((SnargsS) < ((long)b_21X))) {
        merged_arg0K0 = 1;
        goto application_exception;}
      else {
        goto START;}}
      break;
    case 2 : {
      RSvalS = ((((SnargsS))<<2));
      goto START;}
      break;
    case 3 : {
      count_270X = *((unsigned char *) (RScode_pointerS));
      if (((Sstack_limitS) < (-8 + ((RSstackS) - (((((long)count_270X))<<2)))))) {
        goto L27779;}
      else {
        space_271X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
        if ((((ShpS) + (((space_271X)<<2))) < (SlimitS))) {
          arg1K0 = 0;
          goto L27812;}
        else {
          merged_arg1K0 = 0;
          collect_saving_temp_return_tag = 0;
          goto collect_saving_temp;
         collect_saving_temp_return_0:
          if ((((ShpS) + (((space_271X)<<2))) < (SlimitS))) {
            arg1K0 = 0;
            goto L27812;}
          else {
            TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
            arg1K0 = 0;
            goto L27812;}}}}
      break;
    case 4 : {
      space_22X = 2 + ((long)(*((unsigned char *) (RScode_pointerS))));
      if ((((ShpS) + (((space_22X)<<2))) < (SlimitS))) {
        goto L27845;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 1;
        goto collect_saving_temp;
       collect_saving_temp_return_1:
        if ((((ShpS) + (((space_22X)<<2))) < (SlimitS))) {
          goto L27845;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L27845;}}}
      break;
    case 5 : {
      RSenvS = (*((long *) (-3 + (RSenvS))));
      goto START;}
      break;
    case 6 : {
      b_268X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((64 == (SnargsS))) {
        RSstackS = (4 + (RSstackS));
        merged_arg1K0 = (*((long *) (RSstackS)));
        merged_arg1K1 = ((-1 + (SnargsS)) - ((long)b_268X));
        pop_args_list_return_tag = 0;
        goto pop_args_list;
       pop_args_list_return_0:
        v_23X = pop_args_list_return_value;
        arg1K0 = v_23X;
        goto L27913;}
      else {
        merged_arg1K0 = 25;
        merged_arg1K1 = ((SnargsS) - ((long)b_268X));
        pop_args_list_return_tag = 1;
        goto pop_args_list;
       pop_args_list_return_1:
        v_269X = pop_args_list_return_value;
        arg1K0 = v_269X;
        goto L27913;}}
      break;
    case 7 : {
      b_24X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + (StemplateS)) + (((((long)b_24X))<<2)))));
      goto START;}
      break;
    case 8 : {
      b_266X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      env_267X = RSenvS;
      arg1K0 = env_267X;
      arg1K1 = ((long)b_266X);
      goto L27677;}
      break;
    case 9 : {
      env_264X = RSenvS;
      b_265X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_264X) + (((((long)b_265X))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 10 : {
      env_262X = *((long *) (-3 + (RSenvS)));
      b_263X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_262X) + (((((long)b_263X))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 11 : {
      env_260X = *((long *) (-3 + (*((long *) (-3 + (RSenvS))))));
      b_261X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) ((-3 + env_260X) + (((((long)b_261X))<<2)))));
      if ((529 == (RSvalS))) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 12 : {
      b_26X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      env_25X = RSenvS;
      arg1K0 = env_25X;
      arg1K1 = ((long)b_26X);
      goto L27985;}
      break;
    case 13 : {
      b_28X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      location_27X = *((long *) ((-3 + (StemplateS)) + (((((long)b_28X))<<2))));
      RSvalS = (*((long *) (-3 + location_27X)));
      if ((17 == (255 & (RSvalS)))) {
        merged_arg0K0 = 1;
        merged_arg1K1 = location_27X;
        goto raise_exception1;}
      else {
        goto START;}}
      break;
    case 14 : {
      b_30X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      location_29X = *((long *) ((-3 + (StemplateS)) + (((((long)b_30X))<<2))));
      if ((273 == (*((long *) (-3 + location_29X))))) {
        merged_arg0K0 = 1;
        merged_arg1K1 = location_29X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}
      else {
        *((long *) (-3 + location_29X)) = (RSvalS);
        RSvalS = 13;
        goto START;}}
      break;
    case 15 : {
      p_259X = RSenvS;
      if ((p_259X < (Sstack_beginS))) {
        arg1K0 = 0;
        goto L28083;}
      else {
        if (((Sstack_endS) < p_259X)) {
          arg1K0 = 0;
          goto L28083;}
        else {
          arg1K0 = ((Sstack_endS) - (Sstack_beginS));
          goto L28083;}}}
      break;
    case 16 : {
      *((long *) (RSstackS)) = (RSvalS);
      RSstackS = (-4 + (RSstackS));
      goto START;}
      break;
    case 17 : {
      RSstackS = (4 + (RSstackS));
      RSvalS = (*((long *) (RSstackS)));
      goto START;}
      break;
    case 18 : {
      b_31X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RSvalS = (*((long *) (4 + ((RSstackS) + (((((long)b_31X))<<2))))));
      goto START;}
      break;
    case 19 : {
      b_258X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      *((long *) (4 + ((RSstackS) + (((((long)b_258X))<<2))))) = (RSvalS);
      goto START;}
      break;
    case 20 : {
      b_254X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_255X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_256X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if (((Sstack_limitS) < (-276 + (RSstackS)))) {
        arg1K0 = 0;
        goto L28226;}
      else {
        space_257X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
        if ((((ShpS) + (((space_257X)<<2))) < (SlimitS))) {
          arg1K0 = 0;
          goto L28243;}
        else {
          merged_arg1K0 = 0;
          collect_saving_temp_return_tag = 2;
          goto collect_saving_temp;
         collect_saving_temp_return_2:
          if ((((ShpS) + (((space_257X)<<2))) < (SlimitS))) {
            arg1K0 = 0;
            goto L28243;}
          else {
            TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
            arg1K0 = 0;
            goto L28243;}}}}
      break;
    case 21 : {
      space_32X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
      if ((((ShpS) + (((space_32X)<<2))) < (SlimitS))) {
        arg1K0 = 0;
        goto L29927;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 3;
        goto collect_saving_temp;
       collect_saving_temp_return_3:
        if ((((ShpS) + (((space_32X)<<2))) < (SlimitS))) {
          arg1K0 = 0;
          goto L29927;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          arg1K0 = 0;
          goto L29927;}}}
      break;
    case 22 : {
      cont_34X = *((long *) (-3 + (Sbottom_of_stackS)));
      if ((1 == cont_34X)) {
        if ((0 == (3 & (RSvalS)))) {
          RSvalS = ((((RSvalS))>>2));
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          TTreturn_value = 0;
          return(0L);}
        else {
          RSstackS = (-11 + (Sbottom_of_stackS));
          *((long *) (-3 + (Sbottom_of_stackS))) = 1;
          merged_arg0K0 = 0;
          merged_arg1K1 = (RSvalS);
          goto raise_exception1;}}
      else {
        if ((1 == cont_34X)) {
          RSstackS = (-11 + (Sbottom_of_stackS));
          *((long *) (-3 + (Sbottom_of_stackS))) = 1;
          arg1K0 = (Sbottom_of_stackS);
          goto L28344;}
        else {
          merged_arg1K0 = cont_34X;
          copy_continuation_from_heap_return_tag = 0;
          goto copy_continuation_from_heap;
         copy_continuation_from_heap_return_0:
          v_33X = copy_continuation_from_heap_return_value;
          arg1K0 = v_33X;
          goto L28344;}}}
      break;
    case 23 : {
      SnargsS = ((long)(*((unsigned char *) (RScode_pointerS))));
      merged_arg0K0 = 0;
      goto perform_application;}
      break;
    case 24 : {
      SnargsS = ((long)(*((unsigned char *) (RScode_pointerS))));
      start_arg_252X = (RSstackS) + ((((SnargsS))<<2));
      start_loc_253X = -11 + (ScontS);
      if ((start_arg_252X < start_loc_253X)) {
        arg1K0 = start_loc_253X;
        arg1K1 = start_arg_252X;
        goto L15910;}
      else {
        goto L15893;}}
      break;
    case 25 : {
      RSstackS = (4 + (RSstackS));
      proc_36X = *((long *) (RSstackS));
      list_35X = RSvalS;
      arg1K0 = list_35X;
      arg1K1 = 0;
      arg1K2 = list_35X;
      arg3K3 = 0;
      goto L25355;}
      break;
    case 26 : {
      RSstackS = (4 + (RSstackS));
      cont_38X = *((long *) (RSstackS));
      if ((1 == cont_38X)) {
        RSstackS = (-11 + (Sbottom_of_stackS));
        *((long *) (-3 + (Sbottom_of_stackS))) = 1;
        arg1K0 = (Sbottom_of_stackS);
        goto L29957;}
      else {
        merged_arg1K0 = cont_38X;
        copy_continuation_from_heap_return_tag = 1;
        goto copy_continuation_from_heap;
       copy_continuation_from_heap_return_1:
        v_37X = copy_continuation_from_heap_return_value;
        arg1K0 = v_37X;
        goto L29957;}}
      break;
    case 27 : {
      RSstackS = (4 + (RSstackS));
      consumer_251X = *((long *) (RSstackS));
      *((long *) (RSstackS)) = (RSvalS);
      RSstackS = (-4 + (RSstackS));
      RSvalS = consumer_251X;
      SnargsS = 1;
      merged_arg0K0 = 0;
      goto perform_application;}
      break;
    case 28 : {
      cont_41X = ScontS;
      tem_40X = *((long *) (5 + cont_41X));
      pc_39X = *((long *) (1 + cont_41X));
      StemplateS = tem_40X;
      RScode_pointerS = ((-3 + (*((long *) (-3 + tem_40X)))) + (((pc_39X)>>2)));
      RSenvS = (*((long *) (9 + cont_41X)));
      ScontS = (*((long *) (-3 + cont_41X)));
      RSstackS = (9 + cont_41X);
      goto START;}
      break;
    case 29 : {
      merged_arg1K0 = (SnargsS);
      goto return_values;}
      break;
    case 30 : {
      merged_arg1K0 = ((long)(*((unsigned char *) (RScode_pointerS))));
      goto return_values;}
      break;
    case 31 : {
      goto START;}
      break;
    case 32 : {
      b_43X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      tem_42X = *((long *) ((-3 + (StemplateS)) + (((((long)b_43X))<<2))));
      StemplateS = tem_42X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_42X))));
      goto START;}
      break;
    case 33 : {
      b_46X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      SnargsS = ((long)b_46X);
      b_45X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      tem_44X = *((long *) ((-3 + (StemplateS)) + (((((long)b_45X))<<2))));
      StemplateS = tem_44X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_44X))));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        goto START;}
      else {
        goto handle_interrupt;}}
      break;
    case 34 : {
      b_48X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_47X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((1 == (RSvalS))) {
        RScode_pointerS = ((RScode_pointerS) + ((((((long)b_48X))<<8)) + ((long)b_47X)));
        goto START;}
      else {
        goto START;}}
      break;
    case 35 : {
      b_249X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_250X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      RScode_pointerS = ((RScode_pointerS) + ((((((long)b_249X))<<8)) + ((long)b_250X)));
      goto START;}
      break;
    case 36 : {
      if ((0 == (3 & (RSvalS)))) {
        val_50X = (((RSvalS))>>2);
        b_49X = *((unsigned char *) (RScode_pointerS));
        RScode_pointerS = (1 + (RScode_pointerS));
        if ((val_50X < 0)) {
          arg1K0 = (((((long)b_49X))<<1));
          goto L28551;}
        else {
          if ((val_50X < ((long)b_49X))) {
            RScode_pointerS = ((RScode_pointerS) + (((val_50X)<<1)));
            b_247X = *((unsigned char *) (RScode_pointerS));
            RScode_pointerS = (1 + (RScode_pointerS));
            b_248X = *((unsigned char *) (RScode_pointerS));
            RScode_pointerS = (1 + (RScode_pointerS));
            arg1K0 = ((((((long)b_247X))<<8)) + ((long)b_248X));
            goto L28551;}
          else {
            arg1K0 = (((((long)b_49X))<<1));
            goto L28551;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 37 : {
      RSstackS = (4 + (RSstackS));
      arg2_245X = *((long *) (RSstackS));
      x_246X = RSvalS;
      if ((arg2_245X == x_246X)) {
        arg0K0 = 5;
        goto L30078;}
      else {
        arg0K0 = 1;
        goto L30078;}}
      break;
    case 38 : {
      x_244X = RSvalS;
      if ((0 == (3 & x_244X))) {
        arg0K0 = 5;
        goto L30092;}
      else {
        if ((3 == (3 & x_244X))) {
          if ((9 == (31 & ((((*((long *) (-7 + x_244X))))>>2))))) {
            arg0K0 = 5;
            goto L30092;}
          else {
            arg0K0 = 1;
            goto L30092;}}
        else {
          arg0K0 = 1;
          goto L30092;}}}
      break;
    case 39 : {
      x_243X = RSvalS;
      if ((0 == (3 & x_243X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_243X))) {
          if ((9 == (31 & ((((*((long *) (-7 + x_243X))))>>2))))) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_243X;
            goto raise_exception1;}
          else {
            goto L30110;}}
        else {
          goto L30110;}}}
      break;
    case 40 : {
      x_242X = RSvalS;
      if ((0 == (3 & x_242X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_242X))) {
          if ((9 == (31 & ((((*((long *) (-7 + x_242X))))>>2))))) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_242X;
            goto raise_exception1;}
          else {
            goto L30137;}}
        else {
          goto L30137;}}}
      break;
    case 41 : {
      x_241X = RSvalS;
      if ((0 == (3 & x_241X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_241X))) {
          if ((9 == (31 & ((((*((long *) (-7 + x_241X))))>>2))))) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_241X;
            goto raise_exception1;}
          else {
            goto L30164;}}
        else {
          goto L30164;}}}
      break;
    case 42 : {
      x_240X = RSvalS;
      if ((0 == (3 & x_240X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_240X))) {
          if ((9 == (31 & ((((*((long *) (-7 + x_240X))))>>2))))) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_240X;
            goto raise_exception1;}
          else {
            goto L30191;}}
        else {
          goto L30191;}}}
      break;
    case 43 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 5;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 44 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 45 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 46 : {
      RSstackS = (4 + (RSstackS));
      arg2_239X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_239X | (RSvalS))))) {
        x_52X = RSvalS;
        z_51X = (((arg2_239X)>>2)) + (((x_52X)>>2));
        if ((536870911 < z_51X)) {
          goto L24242;}
        else {
          if ((z_51X < -536870912)) {
            goto L24242;}
          else {
            RSvalS = (((z_51X)<<2));
            goto START;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_239X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 47 : {
      RSstackS = (4 + (RSstackS));
      arg2_53X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_53X | (RSvalS))))) {
        x_236X = RSvalS;
        a_237X = ((arg2_53X)>>2);
        b_238X = ((x_236X)>>2);
        if ((a_237X < 0)) {
          arg1K0 = (0 - a_237X);
          goto L24038;}
        else {
          arg1K0 = a_237X;
          goto L24038;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_53X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 48 : {
      RSstackS = (4 + (RSstackS));
      arg2_54X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_54X | (RSvalS))))) {
        x_234X = RSvalS;
        z_235X = (((arg2_54X)>>2)) - (((x_234X)>>2));
        if ((536870911 < z_235X)) {
          goto L23921;}
        else {
          if ((z_235X < -536870912)) {
            goto L23921;}
          else {
            RSvalS = (((z_235X)<<2));
            goto START;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_54X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 49 : {
      RSstackS = (4 + (RSstackS));
      arg2_55X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_55X | (RSvalS))))) {
        x_231X = RSvalS;
        if ((0 == x_231X)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_55X;
          merged_arg1K2 = x_231X;
          goto raise_exception2;}
        else {
          a_232X = ((arg2_55X)>>2);
          b_233X = ((x_231X)>>2);
          if ((a_232X < 0)) {
            arg1K0 = (0 - a_232X);
            goto L23715;}
          else {
            arg1K0 = a_232X;
            goto L23715;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_55X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 50 : {
      RSstackS = (4 + (RSstackS));
      arg2_230X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_230X | (RSvalS))))) {
        x_56X = RSvalS;
        if ((arg2_230X == x_56X)) {
          arg0K0 = 5;
          goto L30274;}
        else {
          arg0K0 = 1;
          goto L30274;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_230X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 51 : {
      RSstackS = (4 + (RSstackS));
      arg2_57X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_57X | (RSvalS))))) {
        x_229X = RSvalS;
        if ((arg2_57X < x_229X)) {
          arg0K0 = 5;
          goto L30298;}
        else {
          arg0K0 = 1;
          goto L30298;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_57X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 52 : {
      RSstackS = (4 + (RSstackS));
      arg2_58X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_58X | (RSvalS))))) {
        x_226X = RSvalS;
        if ((0 == x_226X)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_58X;
          merged_arg1K2 = x_226X;
          goto raise_exception2;}
        else {
          a_227X = ((arg2_58X)>>2);
          b_228X = ((x_226X)>>2);
          if ((a_227X < 0)) {
            arg1K0 = (0 - a_227X);
            goto L23380;}
          else {
            arg1K0 = a_227X;
            goto L23380;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_58X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 53 : {
      RSstackS = (4 + (RSstackS));
      arg2_59X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_59X | (RSvalS))))) {
        x_223X = RSvalS;
        if ((0 == x_223X)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_59X;
          merged_arg1K2 = x_223X;
          goto raise_exception2;}
        else {
          a_224X = ((arg2_59X)>>2);
          b_225X = ((x_223X)>>2);
          if ((a_224X < 0)) {
            arg1K0 = (0 - a_224X);
            goto L23249;}
          else {
            arg1K0 = a_224X;
            goto L23249;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_59X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 54 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 55 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 56 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 4;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 57 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = (RSvalS);
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 58 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = 0;
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 59 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 60 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 61 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 62 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 63 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 64 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 65 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 66 : {
      if ((0 == (3 & (RSvalS)))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 67 : {
      if ((0 == (3 & (RSvalS)))) {
        x_60X = RSvalS;
        if ((x_60X < 0)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = x_60X;
          goto raise_exception1;}
        else {
          RSvalS = 0;
          goto START;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 68 : {
      if ((0 == (3 & (RSvalS)))) {
        x_222X = RSvalS;
        if ((x_222X < 0)) {
          arg1K0 = (0 - x_222X);
          goto L30486;}
        else {
          arg1K0 = x_222X;
          goto L30486;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 69 : {
      RSstackS = (4 + (RSstackS));
      arg2_61X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_61X | (RSvalS))))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_61X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_61X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 70 : {
      RSstackS = (4 + (RSstackS));
      arg2_62X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_62X | (RSvalS))))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_62X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_62X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 71 : {
      RSstackS = (4 + (RSstackS));
      arg2_63X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_63X | (RSvalS))))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_63X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_63X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 72 : {
      if ((0 == (3 & (RSvalS)))) {
        RSvalS = ((((~ ((((RSvalS))>>2))))<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 73 : {
      RSstackS = (4 + (RSstackS));
      arg2_64X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_64X | (RSvalS))))) {
        RSvalS = (((((((arg2_64X)>>2)) & ((((RSvalS))>>2))))<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_64X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 74 : {
      RSstackS = (4 + (RSstackS));
      arg2_65X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_65X | (RSvalS))))) {
        RSvalS = (((((((arg2_65X)>>2)) | ((((RSvalS))>>2))))<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_65X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 75 : {
      RSstackS = (4 + (RSstackS));
      arg2_66X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_66X | (RSvalS))))) {
        RSvalS = (((((((arg2_66X)>>2)) ^ ((((RSvalS))>>2))))<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_66X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 76 : {
      RSstackS = (4 + (RSstackS));
      arg2_220X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_220X | (RSvalS))))) {
        x_72X = RSvalS;
        count_71X = ((x_72X)>>2);
        value_70X = ((arg2_220X)>>2);
        if ((count_71X < 0)) {
          PS_SHIFT_RIGHT(value_70X, (0 - count_71X), x_221X)
          RSvalS = (((x_221X)<<2));
          goto START;}
        else {
          PS_SHIFT_LEFT(value_70X, count_71X, x_69X)
          result_68X = (((((x_69X)<<2)))>>2);
          PS_SHIFT_RIGHT(result_68X, count_71X, x_67X)
          if ((value_70X == x_67X)) {
            if ((value_70X < 0)) {
              if ((result_68X < 0)) {
                goto L22698;}
              else {
                goto L22695;}}
            else {
              if ((result_68X < 0)) {
                goto L22695;}
              else {
                goto L22698;}}}
          else {
            goto L22695;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_220X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 77 : {
      x_219X = RSvalS;
      if ((9 == (255 & x_219X))) {
        arg0K0 = 5;
        goto L30614;}
      else {
        arg0K0 = 1;
        goto L30614;}}
      break;
    case 78 : {
      RSstackS = (4 + (RSstackS));
      arg2_217X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_217X))) {
        if ((9 == (255 & (RSvalS)))) {
          x_218X = RSvalS;
          if ((arg2_217X == x_218X)) {
            arg0K0 = 5;
            goto L30641;}
          else {
            arg0K0 = 1;
            goto L30641;}}
        else {
          goto L30629;}}
      else {
        goto L30629;}}
      break;
    case 79 : {
      RSstackS = (4 + (RSstackS));
      arg2_215X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_215X))) {
        if ((9 == (255 & (RSvalS)))) {
          x_216X = RSvalS;
          if ((arg2_215X < x_216X)) {
            arg0K0 = 5;
            goto L30670;}
          else {
            arg0K0 = 1;
            goto L30670;}}
        else {
          goto L30658;}}
      else {
        goto L30658;}}
      break;
    case 80 : {
      if ((9 == (255 & (RSvalS)))) {
        RSvalS = (((((long)((((((RSvalS))>>8))))))<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 81 : {
      if ((0 == (3 & (RSvalS)))) {
        x_73X = (((RSvalS))>>2);
        if ((255 < x_73X)) {
          goto L30701;}
        else {
          if ((x_73X < 0)) {
            goto L30701;}
          else {
            RSvalS = (9 + (((((long)((x_73X))))<<8)));
            goto START;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 82 : {
      x_214X = RSvalS;
      if ((21 == x_214X)) {
        arg0K0 = 5;
        goto L30723;}
      else {
        arg0K0 = 1;
        goto L30723;}}
      break;
    case 83 : {
      x_75X = RSvalS;
      b_74X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_75X))) {
        if (((31 & ((((*((long *) (-7 + x_75X))))>>2))) == ((long)b_74X))) {
          arg0K0 = 5;
          goto L30741;}
        else {
          arg0K0 = 1;
          goto L30741;}}
      else {
        arg0K0 = 1;
        goto L30741;}}
      break;
    case 84 : {
      x_212X = RSvalS;
      b_213X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_212X))) {
        if (((31 & ((((*((long *) (-7 + x_212X))))>>2))) == ((long)b_213X))) {
          RSvalS = (-4 & (3 + ((long)(((unsigned long)(*((long *) (-7 + x_212X))))>>8))));
          goto START;}
        else {
          goto L22411;}}
      else {
        goto L22411;}}
      break;
    case 85 : {
      space_76X = 4 + (((((long)(*((unsigned char *) (RScode_pointerS)))))<<2));
      if ((((ShpS) + (((space_76X)<<2))) < (SlimitS))) {
        goto L16612;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 4;
        goto collect_saving_temp;
       collect_saving_temp_return_4:
        if ((((ShpS) + (((space_76X)<<2))) < (SlimitS))) {
          goto L16612;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L16612;}}}
      break;
    case 86 : {
      x_209X = RSvalS;
      b_210X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_211X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & x_209X))) {
        if (((31 & ((((*((long *) (-7 + x_209X))))>>2))) == ((long)b_210X))) {
          RSvalS = (*((long *) ((-3 + x_209X) + (((((long)b_211X))<<2)))));
          goto START;}
        else {
          goto L25238;}}
      else {
        goto L25238;}}
      break;
    case 87 : {
      RSstackS = (4 + (RSstackS));
      arg2_80X = *((long *) (RSstackS));
      x_79X = RSvalS;
      b_78X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      b_77X = *((unsigned char *) (RScode_pointerS));
      RScode_pointerS = (1 + (RScode_pointerS));
      if ((3 == (3 & arg2_80X))) {
        if (((31 & ((((*((long *) (-7 + arg2_80X))))>>2))) == ((long)b_78X))) {
          if ((3 == (3 & arg2_80X))) {
            if ((0 == (128 & (*((long *) (-7 + arg2_80X)))))) {
              *((long *) ((-3 + arg2_80X) + (((((long)b_77X))<<2)))) = x_79X;
              RSvalS = 13;
              goto START;}
            else {
              goto L25711;}}
          else {
            goto L25711;}}
        else {
          goto L25711;}}
      else {
        goto L25711;}}
      break;
    case 88 : {
      RSstackS = (4 + (RSstackS));
      arg2_208X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_208X))) {
        x_85X = RSvalS;
        len_84X = ((arg2_208X)>>2);
        b_83X = *((unsigned char *) (RScode_pointerS));
        RScode_pointerS = (1 + (RScode_pointerS));
        size_82X = 1 + len_84X;
        if ((len_84X < 0)) {
          goto L25001;}
        else {
          if ((4194304 < size_82X)) {
            goto L25001;}
          else {
            if ((((ShpS) + (((size_82X)<<2))) < (SlimitS))) {
              arg3K0 = 1;
              arg1K1 = x_85X;
              goto L25006;}
            else {
              merged_arg1K0 = x_85X;
              collect_saving_temp_return_tag = 5;
              goto collect_saving_temp;
             collect_saving_temp_return_5:
              temp_81X = collect_saving_temp_return_value;
              arg3K0 = (((ShpS) + (((size_82X)<<2))) < (SlimitS));
              arg1K1 = temp_81X;
              goto L25006;}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_208X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 89 : {
      RSstackS = (4 + (RSstackS));
      arg2_207X = *((long *) (RSstackS));
      if ((0 == (3 & (RSvalS)))) {
        index_88X = (((RSvalS))>>2);
        b_87X = *((unsigned char *) (RScode_pointerS));
        RScode_pointerS = (1 + (RScode_pointerS));
        if ((3 == (3 & arg2_207X))) {
          if (((31 & ((((*((long *) (-7 + arg2_207X))))>>2))) == ((long)b_87X))) {
            len_86X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg2_207X))))>>8))))>>2);
            if ((index_88X < 0)) {
              goto L24824;}
            else {
              if ((index_88X < len_86X)) {
                RSvalS = (*((long *) ((-3 + arg2_207X) + (((index_88X)<<2)))));
                goto START;}
              else {
                goto L24824;}}}
          else {
            goto L24824;}}
        else {
          goto L24824;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_207X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 90 : {
      RSstackS = (4 + (RSstackS));
      arg2_205X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_206X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_205X))) {
        x_92X = RSvalS;
        index_91X = ((arg2_205X)>>2);
        b_90X = *((unsigned char *) (RScode_pointerS));
        RScode_pointerS = (1 + (RScode_pointerS));
        if ((3 == (3 & arg3_206X))) {
          if (((31 & ((((*((long *) (-7 + arg3_206X))))>>2))) == ((long)b_90X))) {
            len_89X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + arg3_206X))))>>8))))>>2);
            if ((index_91X < 0)) {
              goto L25455;}
            else {
              if ((index_91X < len_89X)) {
                if ((3 == (3 & arg3_206X))) {
                  if ((0 == (128 & (*((long *) (-7 + arg3_206X)))))) {
                    *((long *) ((-3 + arg3_206X) + (((index_91X)<<2)))) = x_92X;
                    RSvalS = 13;
                    goto START;}
                  else {
                    goto L25455;}}
                else {
                  goto L25455;}}
              else {
                goto L25455;}}}
          else {
            goto L25455;}}
        else {
          goto L25455;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg3_206X;
        merged_arg1K2 = arg2_205X;
        merged_arg1K3 = (RSvalS);
        goto raise_exception3;}}
      break;
    case 91 : {
      RSstackS = (4 + (RSstackS));
      arg2_204X = *((long *) (RSstackS));
      if ((0 == (3 & (arg2_204X | (RSvalS))))) {
        init_95X = (((RSvalS))>>2);
        len_94X = ((arg2_204X)>>2);
        size_93X = 1 + ((((3 + len_94X))>>2));
        if ((len_94X < 0)) {
          goto L22159;}
        else {
          if ((4194304 < size_93X)) {
            goto L22159;}
          else {
            if ((((ShpS) + (((size_93X)<<2))) < (SlimitS))) {
              goto L22251;}
            else {
              merged_arg1K0 = 0;
              collect_saving_temp_return_tag = 6;
              goto collect_saving_temp;
             collect_saving_temp_return_6:
              if ((((ShpS) + (((size_93X)<<2))) < (SlimitS))) {
                goto L22251;}
              else {
                merged_arg0K0 = 0;
                merged_arg1K1 = (((len_94X)<<2));
                merged_arg1K2 = (((init_95X)<<2));
                goto raise_exception2;}}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_204X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 92 : {
      obj_96X = RSvalS;
      if ((3 == (3 & obj_96X))) {
        if ((16 == (31 & ((((*((long *) (-7 + obj_96X))))>>2))))) {
          RSvalS = (((((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8)))<<2));
          goto START;}
        else {
          goto L30788;}}
      else {
        goto L30788;}}
      break;
    case 93 : {
      RSstackS = (4 + (RSstackS));
      arg2_97X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_97X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg2_97X))))>>2))))) {
          if ((0 == (3 & (RSvalS)))) {
            index_202X = (((RSvalS))>>2);
            len_203X = (long)(((unsigned long)(*((long *) (-7 + arg2_97X))))>>8);
            if ((index_202X < 0)) {
              goto L22009;}
            else {
              if ((index_202X < len_203X)) {
                RSvalS = (((((long)(*((unsigned char *) ((-3 + arg2_97X) + index_202X)))))<<2));
                goto START;}
              else {
                goto L22009;}}}
          else {
            goto L22014;}}
        else {
          goto L22014;}}
      else {
        goto L22014;}}
      break;
    case 94 : {
      RSstackS = (4 + (RSstackS));
      arg2_99X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_98X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_98X))) {
        if ((16 == (31 & ((((*((long *) (-7 + arg3_98X))))>>2))))) {
          if ((0 == (3 & (arg2_99X | (RSvalS))))) {
            value_199X = (((RSvalS))>>2);
            index_200X = ((arg2_99X)>>2);
            len_201X = (long)(((unsigned long)(*((long *) (-7 + arg3_98X))))>>8);
            if ((index_200X < 0)) {
              goto L24650;}
            else {
              if ((index_200X < len_201X)) {
                *((unsigned char *) ((-3 + arg3_98X) + index_200X)) = ((unsigned char)value_199X);
                RSvalS = 13;
                goto START;}
              else {
                goto L24650;}}}
          else {
            goto L24657;}}
        else {
          goto L24657;}}
      else {
        goto L24657;}}
      break;
    case 95 : {
      RSstackS = (4 + (RSstackS));
      arg2_195X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_195X))) {
        if ((9 == (255 & (RSvalS)))) {
          init_196X = ((((RSvalS))>>8));
          len_197X = ((arg2_195X)>>2);
          size_198X = 1 + ((((4 + len_197X))>>2));
          if ((len_197X < 0)) {
            goto L21744;}
          else {
            if ((4194304 < size_198X)) {
              goto L21744;}
            else {
              if ((((ShpS) + (((size_198X)<<2))) < (SlimitS))) {
                goto L21838;}
              else {
                merged_arg1K0 = 0;
                collect_saving_temp_return_tag = 7;
                goto collect_saving_temp;
               collect_saving_temp_return_7:
                if ((((ShpS) + (((size_198X)<<2))) < (SlimitS))) {
                  goto L21838;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (((len_197X)<<2));
                  merged_arg1K2 = (9 + (((((long)(init_196X)))<<8)));
                  goto raise_exception2;}}}}}
        else {
          goto L21787;}}
      else {
        goto L21787;}}
      break;
    case 96 : {
      obj_100X = RSvalS;
      if ((3 == (3 & obj_100X))) {
        if ((15 == (31 & ((((*((long *) (-7 + obj_100X))))>>2))))) {
          RSvalS = (-4 + (((((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8)))<<2)));
          goto START;}
        else {
          goto L30819;}}
      else {
        goto L30819;}}
      break;
    case 97 : {
      RSstackS = (4 + (RSstackS));
      arg2_101X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_101X))) {
        if ((15 == (31 & ((((*((long *) (-7 + arg2_101X))))>>2))))) {
          if ((0 == (3 & (RSvalS)))) {
            index_193X = (((RSvalS))>>2);
            len_194X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg2_101X))))>>8));
            if ((index_193X < 0)) {
              goto L21594;}
            else {
              if ((index_193X < len_194X)) {
                RSvalS = (9 + (((((long)((((long)(*((unsigned char *) ((-3 + arg2_101X) + index_193X))))))))<<8)));
                goto START;}
              else {
                goto L21594;}}}
          else {
            goto L21599;}}
        else {
          goto L21599;}}
      else {
        goto L21599;}}
      break;
    case 98 : {
      RSstackS = (4 + (RSstackS));
      arg2_188X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_189X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_189X))) {
        if ((15 == (31 & ((((*((long *) (-7 + arg3_189X))))>>2))))) {
          if ((0 == (3 & arg2_188X))) {
            if ((9 == (255 & (RSvalS)))) {
              Kchar_190X = ((((RSvalS))>>8));
              index_191X = ((arg2_188X)>>2);
              len_192X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg3_189X))))>>8));
              if ((index_191X < 0)) {
                goto L24476;}
              else {
                if ((index_191X < len_192X)) {
                  *((unsigned char *) ((-3 + arg3_189X) + index_191X)) = ((unsigned char)((long)(Kchar_190X)));
                  RSvalS = 13;
                  goto START;}
                else {
                  goto L24476;}}}
            else {
              goto L24483;}}
          else {
            goto L24483;}}
        else {
          goto L24483;}}
      else {
        goto L24483;}}
      break;
    case 99 : {
      obj_104X = RSvalS;
      if ((3 == (3 & obj_104X))) {
        if ((4 == (31 & ((((*((long *) (-7 + obj_104X))))>>2))))) {
          x_103X = RSvalS;
          descriptor_102X = *((long *) (-3 + x_103X));
          if ((17 == (255 & descriptor_102X))) {
            if ((529 == (*((long *) (-3 + x_103X))))) {
              arg0K0 = 5;
              goto L18245;}
            else {
              arg0K0 = 1;
              goto L18245;}}
          else {
            arg0K0 = 5;
            goto L18245;}}
        else {
          goto L18236;}}
      else {
        goto L18236;}}
      break;
    case 100 : {
      RSstackS = (4 + (RSstackS));
      arg2_186X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_186X))) {
        if ((4 == (31 & ((((*((long *) (-7 + arg2_186X))))>>2))))) {
          x_187X = RSvalS;
          if ((1 == x_187X)) {
            goto L21394;}
          else {
            if ((5 == x_187X)) {
              goto L21394;}
            else {
              goto L21422;}}}
        else {
          goto L21422;}}
      else {
        goto L21422;}}
      break;
    case 101 : {
      x_105X = RSvalS;
      if ((3 == (3 & x_105X))) {
        if ((0 == (128 & (*((long *) (-7 + x_105X)))))) {
          arg0K0 = 1;
          goto L30855;}
        else {
          arg0K0 = 5;
          goto L30855;}}
      else {
        arg0K0 = 5;
        goto L30855;}}
      break;
    case 102 : {
      x_185X = RSvalS;
      if ((3 == (3 & x_185X))) {
        if ((0 == (128 & (*((long *) (-7 + x_185X)))))) {
          *((long *) (-7 + x_185X)) = (128 | (*((long *) (-7 + x_185X))));
          goto L30870;}
        else {
          goto L30870;}}
      else {
        goto L30870;}}
      break;
    case 103 : {
      if (((20 + (ShpS)) < (SlimitS))) {
        arg1K0 = 0;
        goto L21112;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 8;
        goto collect_saving_temp;
       collect_saving_temp_return_8:
        if (((20 + (ShpS)) < (SlimitS))) {
          arg1K0 = 0;
          goto L21112;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          arg1K0 = 0;
          goto L21112;}}}
      break;
    case 104 : {
      obj_184X = RSvalS;
      if ((3 == (3 & obj_184X))) {
        if ((5 == (31 & ((((*((long *) (-7 + obj_184X))))>>2))))) {
          merged_arg1K0 = (RSvalS);
          close_port_return_tag = 0;
          goto close_port;
         close_port_return_0:
          RSvalS = 13;
          goto START;}
        else {
          goto L30891;}}
      else {
        goto L30891;}}
      break;
    case 105 : {
      x_183X = RSvalS;
      if ((3 == (3 & x_183X))) {
        if ((5 == (31 & ((((*((long *) (-7 + x_183X))))>>2))))) {
          if ((4 == (*((long *) (-3 + x_183X))))) {
            arg0K0 = 5;
            goto L30913;}
          else {
            arg0K0 = 1;
            goto L30913;}}
        else {
          arg0K0 = 1;
          goto L30913;}}
      else {
        arg0K0 = 1;
        goto L30913;}}
      break;
    case 106 : {
      x_182X = RSvalS;
      if ((3 == (3 & x_182X))) {
        if ((5 == (31 & ((((*((long *) (-7 + x_182X))))>>2))))) {
          if ((8 == (*((long *) (-3 + x_182X))))) {
            arg0K0 = 5;
            goto L30931;}
          else {
            arg0K0 = 1;
            goto L30931;}}
        else {
          arg0K0 = 1;
          goto L30931;}}
      else {
        arg0K0 = 1;
        goto L30931;}}
      break;
    case 107 : {
      p_106X = RSvalS;
      if ((3 == (3 & p_106X))) {
        if ((5 == (31 & ((((*((long *) (-7 + p_106X))))>>2))))) {
          if ((4 == (*((long *) (-3 + p_106X))))) {
            if (((*((long *) (1 + p_106X))) < 0)) {
              goto L17817;}
            else {
              x_179X = RSvalS;
              c_180X = *((long *) (5 + x_179X));
              if ((1 == c_180X)) {
                { int TTchar;
                  PS_GETC((*((Sopen_portsS) + ((((*((long *) (1 + x_179X))))>>2)))), TTchar);
                  if (EOF == TTchar) {
                    arg1K0 = 21;
                    goto L17814;}
                  else {
                    unsigned char Kchar_181X = TTchar;
                    arg1K0 = (9 + (((((long)(Kchar_181X)))<<8)));
                    goto L17814;}}}
              else {
                *((long *) (5 + x_179X)) = 1;
                arg1K0 = c_180X;
                goto L17814;}}}
          else {
            goto L17817;}}
        else {
          goto L17817;}}
      else {
        goto L17817;}}
      break;
    case 108 : {
      p_177X = RSvalS;
      if ((3 == (3 & p_177X))) {
        if ((5 == (31 & ((((*((long *) (-7 + p_177X))))>>2))))) {
          if ((4 == (*((long *) (-3 + p_177X))))) {
            if (((*((long *) (1 + p_177X))) < 0)) {
              goto L17699;}
            else {
              x_109X = RSvalS;
              c_108X = *((long *) (5 + x_109X));
              if ((1 == c_108X)) {
                v_178X = *((Sopen_portsS) + ((((*((long *) (1 + x_109X))))>>2)));
                { int TTchar;
                  PS_GETC(v_178X, TTchar);
                  if (EOF == TTchar) {
                    arg1K0 = 21;
                    goto L17691;}
                  else {
                    unsigned char Kchar_107X = TTchar;
                    arg1K0 = (9 + (((((long)(Kchar_107X)))<<8)));
                    goto L17691;}}}
              else {
                arg1K0 = c_108X;
                goto L17696;}}}
          else {
            goto L17699;}}
        else {
          goto L17699;}}
      else {
        goto L17699;}}
      break;
    case 109 : {
      p_111X = RSvalS;
      if ((3 == (3 & p_111X))) {
        if ((5 == (31 & ((((*((long *) (-7 + p_111X))))>>2))))) {
          if ((4 == (*((long *) (-3 + p_111X))))) {
            if (((*((long *) (1 + p_111X))) < 0)) {
              goto L17546;}
            else {
              x_175X = RSvalS;
              x_176X = *((long *) (5 + x_175X));
              if ((1 == x_176X)) {
                b_110X = char_ready_p((*((Sopen_portsS) + ((((*((long *) (1 + x_175X))))>>2)))));
                if (b_110X) {
                  arg0K0 = 5;
                  goto L17543;}
                else {
                  arg0K0 = 1;
                  goto L17543;}}
              else {
                arg0K0 = 5;
                goto L17543;}}}
          else {
            goto L17546;}}
        else {
          goto L17546;}}
      else {
        goto L17546;}}
      break;
    case 110 : {
      RSstackS = (4 + (RSstackS));
      arg2_112X = *((long *) (RSstackS));
      if ((9 == (255 & arg2_112X))) {
        p_174X = RSvalS;
        if ((3 == (3 & p_174X))) {
          if ((5 == (31 & ((((*((long *) (-7 + p_174X))))>>2))))) {
            if ((8 == (*((long *) (-3 + p_174X))))) {
              if (((*((long *) (1 + p_174X))) < 0)) {
                goto L21021;}
              else {
                putc(((((arg2_112X)>>8))), (*((Sopen_portsS) + ((((*((long *) (1 + (RSvalS)))))>>2)))));
                RSvalS = 13;
                goto START;}}
            else {
              goto L21021;}}
          else {
            goto L21021;}}
        else {
          goto L21021;}}
      else {
        goto L21021;}}
      break;
    case 111 : {
      RSstackS = (4 + (RSstackS));
      arg2_172X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_172X))) {
        if ((15 == (31 & ((((*((long *) (-7 + arg2_172X))))>>2))))) {
          p_173X = RSvalS;
          if ((3 == (3 & p_173X))) {
            if ((5 == (31 & ((((*((long *) (-7 + p_173X))))>>2))))) {
              if ((8 == (*((long *) (-3 + p_173X))))) {
                if (((*((long *) (1 + p_173X))) < 0)) {
                  goto L20885;}
                else {
                  fwrite((void *)(-3 + arg2_172X), sizeof(char), (-1 + ((long)(((unsigned long)(*((long *) (-7 + arg2_172X))))>>8))), (*((Sopen_portsS) + ((((*((long *) (1 + (RSvalS)))))>>2)))));
                  RSvalS = 13;
                  goto START;}}
              else {
                goto L20885;}}
            else {
              goto L20885;}}
          else {
            goto L20885;}}
        else {
          goto L20885;}}
      else {
        goto L20885;}}
      break;
    case 112 : {
      p_113X = RSvalS;
      if ((3 == (3 & p_113X))) {
        if ((5 == (31 & ((((*((long *) (-7 + p_113X))))>>2))))) {
          if ((8 == (*((long *) (-3 + p_113X))))) {
            if (((*((long *) (1 + p_113X))) < 0)) {
              goto L17454;}
            else {
              fflush((*((Sopen_portsS) + ((((*((long *) (1 + (RSvalS)))))>>2)))));
              RSvalS = 13;
              goto START;}}
          else {
            goto L17454;}}
        else {
          goto L17454;}}
      else {
        goto L17454;}}
      break;
    case 113 : {
      RSvalS = 529;
      goto START;}
      break;
    case 114 : {
      RSvalS = 13;
      goto START;}
      break;
    case 115 : {
      merged_arg0K0 = 0;
      merged_arg1K1 = (RSvalS);
      goto raise_exception1;}
      break;
    case 116 : {
      RSvalS = 1;
      goto START;}
      break;
    case 117 : {
      RSstackS = (4 + (RSstackS));
      arg2_163X = *((long *) (RSstackS));
      RSstackS = (4 + (RSstackS));
      arg3_164X = *((long *) (RSstackS));
      if ((3 == (3 & arg3_164X))) {
        if ((15 == (31 & ((((*((long *) (-7 + arg3_164X))))>>2))))) {
          obj_165X = RSvalS;
          if ((3 == (3 & obj_165X))) {
            if ((15 == (31 & ((((*((long *) (-7 + obj_165X))))>>2))))) {
              x_166X = RSvalS;
              if ((0 == ((long)(Spure_area_countS)))) {
                if ((0 == ((long)(Simpure_area_countS)))) {
                  merged_arg1K0 = arg3_164X;
                  extract_string_return_tag = 0;
                  goto extract_string;
                 extract_string_return_0:
                  spec_167X = extract_string_return_value;
                  port_168X = ps_open(spec_167X, "w");
                  if ((NULL == port_168X)) {
                    merged_arg0K0 = 0;
                    merged_arg1K1 = arg3_164X;
                    merged_arg1K2 = arg2_163X;
                    goto raise_exception2;}
                  else {
                    fwrite((void *)(-3 + x_166X), sizeof(char), (-1 + ((long)(((unsigned long)(*((long *) (-7 + x_166X))))>>8))), port_168X);
                    b_169X = Snewspace_beginS;
                    Snewspace_beginS = (Soldspace_beginS);
                    Soldspace_beginS = b_169X;
                    e_170X = Snewspace_endS;
                    Snewspace_endS = (Soldspace_endS);
                    Soldspace_endS = e_170X;
                    Ssaved_limitS = (SlimitS);
                    Ssaved_hpS = (ShpS);
                    SlimitS = (Snewspace_endS);
                    ShpS = (Snewspace_beginS);
                    Sweak_pointer_hpS = -1;
                    if ((3 == (3 & arg2_163X))) {
                      a_114X = -3 + arg2_163X;
                      if ((a_114X < (Soldspace_beginS))) {
                        arg1K0 = arg2_163X;
                        goto L28721;}
                      else {
                        if ((a_114X < (Soldspace_endS))) {
                          merged_arg1K0 = arg2_163X;
                          copy_object_return_tag = 0;
                          goto copy_object;
                         copy_object_return_0:
                          v_171X = copy_object_return_value;
                          arg1K0 = v_171X;
                          goto L28721;}
                        else {
                          arg1K0 = arg2_163X;
                          goto L28721;}}}
                    else {
                      arg1K0 = arg2_163X;
                      goto L28721;}}}
                else {
                  goto L28806;}}
              else {
                goto L28806;}}
            else {
              goto L28737;}}
          else {
            goto L28737;}}
        else {
          goto L28737;}}
      else {
        goto L28737;}}
      break;
    case 118 : {
      RSvalS = 13;
      merged_arg1K0 = 0;
      collect_saving_temp_return_tag = 9;
      goto collect_saving_temp;
     collect_saving_temp_return_9:
      RSvalS = 13;
      goto START;}
      break;
    case 119 : {
      RSstackS = (4 + (RSstackS));
      arg2_116X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_116X))) {
        x_161X = RSvalS;
        key_162X = ((arg2_116X)>>2);
        if ((0 == key_162X)) {
          RSvalS = (-4 & ((SlimitS) - (ShpS)));
          goto START;}
        else {
          if ((1 == key_162X)) {
            RSvalS = (-4 & (3 + ((Snewspace_endS) - (Snewspace_beginS))));
            goto START;}
          else {
            if ((2 == key_162X)) {
              RSvalS = (((((Sstack_endS) - (Sstack_beginS)))<<2));
              goto START;}
            else {
              if ((3 == key_162X)) {
                if ((0 == (3 & x_161X))) {
                  old_115X = Sminimum_recovered_spaceS;
                  Sminimum_recovered_spaceS = (((x_161X)>>2));
                  RSvalS = (((old_115X)<<2));
                  goto START;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (((key_162X)<<2));
                  merged_arg1K2 = x_161X;
                  goto raise_exception2;}}
              else {
                if ((4 == key_162X)) {
                  RSvalS = ((((Sgc_countS))<<2));
                  goto START;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (((key_162X)<<2));
                  merged_arg1K2 = x_161X;
                  goto raise_exception2;}}}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_116X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 120 : {
      obj_120X = RSvalS;
      if ((3 == (3 & obj_120X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_120X))))>>2))))) {
          x_119X = RSvalS;
          start_hp_118X = ShpS;
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          v_117X = hoistD1((Snewspace_beginS), (ShpS), x_119X);
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          RSenvS = SenvS;
          if (v_117X) {
            areas_158X = Spure_areasS;
            sizes_159X = Spure_sizesS;
            count_160X = Spure_area_countS;
            arg1K0 = 0;
            goto L17346;}
          else {
            goto L17331;}}
        else {
          goto L17278;}}
      else {
        goto L17278;}}
      break;
    case 121 : {
      if ((0 == (3 & (RSvalS)))) {
        type_123X = (((RSvalS))>>2);
        start_hp_122X = ShpS;
        *((long *) (ShpS)) = 0;
        ShpS = (4 + (ShpS));
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        v_121X = hoistD0((Snewspace_beginS), start_hp_122X, type_123X);
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSenvS = SenvS;
        if (v_121X) {
          areas_155X = Simpure_areasS;
          sizes_156X = Simpure_sizesS;
          count_157X = Simpure_area_countS;
          arg1K0 = 0;
          goto L10070;}
        else {
          goto L10008;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 122 : {
      RSvalS = (Sdynamic_stateS);
      goto START;}
      break;
    case 123 : {
      Sdynamic_stateS = (RSvalS);
      RSvalS = 13;
      goto START;}
      break;
    case 124 : {
      obj_124X = RSvalS;
      if ((3 == (3 & obj_124X))) {
        if ((3 == (31 & ((((*((long *) (-7 + obj_124X))))>>2))))) {
          Sexception_handlerS = (RSvalS);
          goto START;}
        else {
          goto L31052;}}
      else {
        goto L31052;}}
      break;
    case 125 : {
      obj_125X = RSvalS;
      if ((3 == (3 & obj_125X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_125X))))>>2))))) {
          if ((((((3 + ((long)(((unsigned long)(*((long *) (-7 + (RSvalS)))))>>8))))>>2)) < 3)) {
            goto L31071;}
          else {
            Sinterrupt_handlersS = (RSvalS);
            goto START;}}
        else {
          goto L31071;}}
      else {
        goto L31071;}}
      break;
    case 126 : {
      temp_154X = Senabled_interruptsS;
      Senabled_interruptsS = ((((RSvalS))>>2));
      RSvalS = (((temp_154X)<<2));
      goto START;}
      break;
    case 127 : {
      RSstackS = (4 + (RSstackS));
      Senabled_interruptsS = ((((*((long *) (RSstackS))))>>2));
      RSstackS = (4 + (RSstackS));
      SnargsS = ((((*((long *) (RSstackS))))>>2));
      RSstackS = (4 + (RSstackS));
      RSenvS = (*((long *) (RSstackS)));
      RSstackS = (4 + (RSstackS));
      tem_126X = *((long *) (RSstackS));
      StemplateS = tem_126X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_126X))));
      RSstackS = (4 + (RSstackS));
      RSvalS = (*((long *) (RSstackS)));
      goto START;}
      break;
    case 128 : {
      if ((0 == (3 & (RSvalS)))) {
        p_152X = RSvalS;
        Spending_interruptsS = (-2 & (Spending_interruptsS));
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        n_153X = ps_schedule_interrupt((((p_152X)>>2)));
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSenvS = SenvS;
        RSvalS = (((n_153X)<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = (RSvalS);
        goto raise_exception1;}}
      break;
    case 129 : {
      obj_131X = RSvalS;
      if ((3 == (3 & obj_131X))) {
        if ((12 == (31 & ((((*((long *) (-7 + obj_131X))))>>2))))) {
          x_130X = RSvalS;
          value_129X = *((long *) (1 + x_130X));
          name_128X = *((long *) (-3 + x_130X));
          if ((3 == (3 & name_128X))) {
            if ((15 == (31 & ((((*((long *) (-7 + name_128X))))>>2))))) {
              if ((3 == (3 & value_129X))) {
                if ((16 == (31 & ((((*((long *) (-7 + value_129X))))>>2))))) {
                  SvalS = RSvalS;
                  SstackS = RSstackS;
                  Scode_pointerS = RScode_pointerS;
                  SenvS = RSenvS;
                  v_127X = lookup_external_name((-3 + name_128X), (-3 + value_129X));
                  RSvalS = SvalS;
                  RSstackS = SstackS;
                  RScode_pointerS = Scode_pointerS;
                  RSenvS = SenvS;
                  if (v_127X) {
                    RSvalS = 13;
                    goto START;}
                  else {
                    goto L16955;}}
                else {
                  goto L16955;}}
              else {
                goto L16955;}}
            else {
              goto L16955;}}
          else {
            goto L16955;}}
        else {
          goto L16958;}}
      else {
        goto L16958;}}
      break;
    case 130 : {
      proc_151X = *((long *) ((RSstackS) + ((((SnargsS))<<2))));
      if ((3 == (3 & proc_151X))) {
        if ((12 == (31 & ((((*((long *) (-7 + proc_151X))))>>2))))) {
          SnargsS = (-1 + (SnargsS));
          RSvalS = proc_151X;
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          TTreturn_value = 1;
          return(0L);}
        else {
          goto L31124;}}
      else {
        goto L31124;}}
      break;
    case 131 : {
      RSstackS = (4 + (RSstackS));
      arg2_134X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_134X))) {
        x_148X = RSvalS;
        key_149X = ((arg2_134X)>>2);
        if ((0 == key_149X)) {
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          n_132X = ps_ticks_per_second();
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          RSenvS = SenvS;
          RSvalS = (((n_132X)<<2));
          goto START;}
        else {
          if ((1 == key_149X)) {
            SvalS = RSvalS;
            SstackS = RSstackS;
            Scode_pointerS = RScode_pointerS;
            SenvS = RSenvS;
            n_150X = ps_run_time();
            RSvalS = SvalS;
            RSstackS = SstackS;
            RScode_pointerS = Scode_pointerS;
            RSenvS = SenvS;
            RSvalS = (((n_150X)<<2));
            goto START;}
          else {
            if ((2 == key_149X)) {
              SvalS = RSvalS;
              SstackS = RSstackS;
              Scode_pointerS = RScode_pointerS;
              SenvS = RSenvS;
              n_133X = ps_real_time();
              RSvalS = SvalS;
              RSstackS = SstackS;
              RScode_pointerS = Scode_pointerS;
              RSenvS = SenvS;
              RSvalS = (((n_133X)<<2));
              goto START;}
            else {
              merged_arg0K0 = 0;
              merged_arg1K1 = (((key_149X)<<2));
              merged_arg1K2 = x_148X;
              goto raise_exception2;}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_134X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 132 : {
      RSstackS = (4 + (RSstackS));
      arg2_147X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_147X))) {
        x_137X = RSvalS;
        key_136X = ((arg2_147X)>>2);
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        return_value_135X = extended_vm(key_136X, x_137X);
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSenvS = SenvS;
        if ((17 == (255 & return_value_135X))) {
          merged_arg0K0 = 0;
          merged_arg1K1 = (((key_136X)<<2));
          merged_arg1K2 = x_137X;
          goto raise_exception2;}
        else {
          RSvalS = return_value_135X;
          goto START;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_147X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 133 : {
      RSstackS = (4 + (RSstackS));
      arg2_146X = *((long *) (RSstackS));
      if ((0 == (3 & arg2_146X))) {
        RSvalS = (RSvalS);
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        TTreturn_value = 0;
        return(0L);}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_146X;
        merged_arg1K2 = (RSvalS);
        goto raise_exception2;}}
      break;
    case 134 : {
      RSstackS = (4 + (RSstackS));
      arg2_143X = *((long *) (RSstackS));
      if ((3 == (3 & arg2_143X))) {
        if ((15 == (31 & ((((*((long *) (-7 + arg2_143X))))>>2))))) {
          obj_144X = RSvalS;
          if ((3 == (3 & obj_144X))) {
            if ((15 == (31 & ((((*((long *) (-7 + obj_144X))))>>2))))) {
              merged_arg1K0 = arg2_143X;
              merged_arg1K1 = (RSvalS);
              stob_equalP_return_tag = 0;
              goto stob_equalP;
             stob_equalP_return_0:
              x_145X = stob_equalP_return_value;
              if (x_145X) {
                arg0K0 = 5;
                goto L20279;}
              else {
                arg0K0 = 1;
                goto L20279;}}
            else {
              goto L20270;}}
          else {
            goto L20270;}}
        else {
          goto L20270;}}
      else {
        goto L20270;}}
      break;
    case 135 : {
      obj_140X = RSvalS;
      if ((3 == (3 & obj_140X))) {
        if ((15 == (31 & ((((*((long *) (-7 + obj_140X))))>>2))))) {
          x_139X = RSvalS;
          n_138X = -1 + ((long)(((unsigned long)(*((long *) (-7 + x_139X))))>>8));
          arg1K0 = 0;
          arg1K1 = 0;
          goto L16805;}
        else {
          goto L16783;}}
      else {
        goto L16783;}}
      break;
    case 136 : {
      space_142X = 1 + ((((4 + (RSvalS)))>>2));
      if ((((ShpS) + (((space_142X)<<2))) < (SlimitS))) {
        goto L19999;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 10;
        goto collect_saving_temp;
       collect_saving_temp_return_10:
        if ((((ShpS) + (((space_142X)<<2))) < (SlimitS))) {
          goto L19999;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L19999;}}}
      break;
    case 137 : {
      if (((20 + (ShpS)) < (SlimitS))) {
        goto L19881;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 11;
        goto collect_saving_temp;
       collect_saving_temp_return_11:
        if (((20 + (ShpS)) < (SlimitS))) {
          goto L19881;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L19881;}}}
      break;
  }}
 L31021: {
  vector_272X = arg1K0;
  if ((1 == vector_272X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = (((type_123X)<<2));
    goto raise_exception1;}
  else {
    RSvalS = vector_272X;
    goto START;}}
 L10008: {
  ShpS = start_hp_122X;
  arg1K0 = 1;
  goto L31021;}
 L10070: {
  i_273X = arg1K0;
  if ((i_273X < ((long)count_157X))) {
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    SenvS = RSenvS;
    v_274X = hoistD0((*(areas_155X + i_273X)), ((*(areas_155X + i_273X)) + (*(sizes_156X + i_273X))), type_123X);
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    RSenvS = SenvS;
    if (v_274X) {
      arg1K0 = (1 + i_273X);
      goto L10070;}
    else {
      goto L10008;}}
  else {
    areas_275X = Spure_areasS;
    sizes_276X = Spure_sizesS;
    count_277X = Spure_area_countS;
    arg1K0 = 0;
    goto L10049;}}
 L10049: {
  i_278X = arg1K0;
  if ((i_278X < ((long)count_277X))) {
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    SenvS = RSenvS;
    v_279X = hoistD0((*(areas_275X + i_278X)), ((*(areas_275X + i_278X)) + (*(sizes_276X + i_278X))), type_123X);
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    RSenvS = SenvS;
    if (v_279X) {
      arg1K0 = (1 + i_278X);
      goto L10049;}
    else {
      goto L10008;}}
  else {
    *((long *) start_hp_122X) = (-1014 + (((((ShpS) - start_hp_122X))<<8)));
    arg1K0 = (7 + start_hp_122X);
    goto L31021;}}
 L8840: {
  h_280X = arg1K0;
  arg1K0 = (4 + (addr_281X + (-4 & (3 + ((long)(((unsigned long)h_280X)>>8))))));
  goto L8818;}
 L8818: {
  addr_281X = arg1K0;
  if ((addr_281X < (ShpS))) {
    d_283X = *((long *) addr_281X);
    if ((2 == (3 & d_283X))) {
      arg1K0 = d_283X;
      goto L8840;}
    else {
      h_282X = *((long *) (-7 + d_283X));
      *((long *) addr_281X) = h_282X;
      arg1K0 = h_282X;
      goto L8840;}}
  else {
    RSvalS = (((size_318X)<<2));
    goto START;}}
 L13252: {
  sym_284X = arg1K0;
  if ((3 == (3 & sym_284X))) {
    if ((0 == (128 & (*((long *) (-7 + sym_284X)))))) {
      *((long *) (-7 + sym_284X)) = (128 | (*((long *) (-7 + sym_284X))));
      arg1K0 = sym_284X;
      goto L13194;}
    else {
      arg1K0 = sym_284X;
      goto L13194;}}
  else {
    arg1K0 = sym_284X;
    goto L13194;}}
 L28093: {
  v_285X = arg1K0;
  RSvalS = v_285X;
  goto START;}
 L19903: {
  val_286X = arg1K0;
  RSvalS = val_286X;
  goto START;}
 L13296: {
  i_287X = arg1K0;
  h_288X = arg1K1;
  if ((i_287X < n_304X)) {
    arg1K0 = (1 + i_287X);
    arg1K1 = (h_288X + ((long)((((long)(*((unsigned char *) ((-3 + arg2_300X) + i_287X))))))));
    goto L13296;}
  else {
    index_290X = h_288X & (-1 + v_303X);
    bucket_289X = *((long *) ((-3 + table_302X) + (((index_290X)<<2))));
    arg1K0 = bucket_289X;
    goto L13186;}}
 L13194: {
  new_291X = arg1K0;
  *((long *) (ShpS)) = 2050;
  ShpS = (4 + (ShpS));
  new_292X = 3 + (ShpS);
  ShpS = (8 + (ShpS));
  *((long *) (-4 + (ShpS))) = 0;
  *((long *) (-3 + new_292X)) = new_291X;
  *((long *) (1 + new_292X)) = bucket_289X;
  *((long *) ((-3 + table_302X) + (((index_290X)<<2)))) = new_292X;
  arg1K0 = new_291X;
  goto L19903;}
 L13186: {
  b_293X = arg1K0;
  if ((25 == b_293X)) {
    *((long *) (ShpS)) = 1030;
    ShpS = (4 + (ShpS));
    new_295X = 3 + (ShpS);
    ShpS = (4 + (ShpS));
    *((long *) (-4 + (ShpS))) = 0;
    *((long *) (-3 + new_295X)) = arg2_300X;
    if ((3 == (3 & new_295X))) {
      if ((0 == (128 & (*((long *) (-7 + new_295X)))))) {
        *((long *) (-7 + new_295X)) = (128 | (*((long *) (-7 + new_295X))));
        arg1K0 = new_295X;
        goto L13252;}
      else {
        arg1K0 = new_295X;
        goto L13252;}}
    else {
      arg1K0 = new_295X;
      goto L13252;}}
  else {
    merged_arg1K0 = arg2_300X;
    merged_arg1K1 = (*((long *) (-3 + (*((long *) (-3 + b_293X))))));
    stob_equalP_return_tag = 1;
    goto stob_equalP;
   stob_equalP_return_1:
    v_294X = stob_equalP_return_value;
    if (v_294X) {
      arg1K0 = (*((long *) (-3 + b_293X)));
      goto L19903;}
    else {
      arg1K0 = (*((long *) (1 + b_293X)));
      goto L13186;}}}
 L9735: {
  stob_296X = arg1K0;
  from_297X = 4 + (RSstackS);
  arg1K0 = from_297X;
  arg1K1 = (1 + stob_296X);
  goto L9774;}
 L9774: {
  from_298X = arg1K0;
  to_299X = arg1K1;
  if ((from_298X < (from_297X + (((((long)b_432X))<<2))))) {
    *((long *) to_299X) = (*((long *) from_298X));
    arg1K0 = (4 + from_298X);
    arg1K1 = (4 + to_299X);
    goto L9774;}
  else {
    RSstackS = ((RSstackS) - ((((0 - ((long)b_432X)))<<2)));
    *((long *) (-3 + stob_296X)) = (RSenvS);
    RSenvS = stob_296X;
    goto START;}}
 L19881: {
  RSstackS = (4 + (RSstackS));
  arg2_300X = *((long *) (RSstackS));
  if ((3 == (3 & arg2_300X))) {
    if ((15 == (31 & ((((*((long *) (-7 + arg2_300X))))>>2))))) {
      obj_301X = RSvalS;
      if ((3 == (3 & obj_301X))) {
        if ((2 == (31 & ((((*((long *) (-7 + obj_301X))))>>2))))) {
          table_302X = RSvalS;
          v_303X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + table_302X))))>>8))))>>2);
          n_304X = -1 + ((long)(((unsigned long)(*((long *) (-7 + arg2_300X))))>>8));
          arg1K0 = 0;
          arg1K1 = 0;
          goto L13296;}
        else {
          goto L19906;}}
      else {
        goto L19906;}}
    else {
      goto L19906;}}
  else {
    goto L19906;}}
 L19906: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_300X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L19999: {
  RSstackS = (4 + (RSstackS));
  arg2_305X = *((long *) (RSstackS));
  if ((0 == (3 & (RSvalS)))) {
    n_306X = (((RSvalS))>>2);
    if ((3 == (3 & arg2_305X))) {
      if ((0 == (31 & ((((*((long *) (-7 + arg2_305X))))>>2))))) {
        goto L20125;}
      else {
        goto L20027;}}
    else {
      goto L20027;}}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_305X;
    merged_arg1K2 = (RSvalS);
    goto raise_exception2;}}
 L20027: {
  if ((25 == arg2_305X)) {
    goto L20125;}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_305X;
    merged_arg1K2 = (((n_306X)<<2));
    goto raise_exception2;}}
 L20125: {
  len_308X = 1 + n_306X;
  *((long *) (ShpS)) = (62 + (((len_308X)<<8)));
  ShpS = (4 + (ShpS));
  new_307X = 3 + (ShpS);
  ShpS = ((ShpS) + (-4 & (3 + len_308X)));
  if ((0 < len_308X)) {
    *((long *) (-4 + (ShpS))) = 0;
    arg1K0 = new_307X;
    goto L20044;}
  else {
    arg1K0 = new_307X;
    goto L20044;}}
 L20044: {
  obj_309X = arg1K0;
  arg1K0 = arg2_305X;
  arg1K1 = (-1 + n_306X);
  goto L20046;}
 L20046: {
  l_310X = arg1K0;
  i_311X = arg1K1;
  if ((i_311X < 0)) {
    RSvalS = obj_309X;
    goto START;}
  else {
    *((unsigned char *) ((-3 + obj_309X) + i_311X)) = ((unsigned char)((long)((((((*((long *) (-3 + l_310X))))>>8))))));
    arg1K0 = (*((long *) (1 + l_310X)));
    arg1K1 = (-1 + i_311X);
    goto L20046;}}
 L16783: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L16805: {
  i_312X = arg1K0;
  h_313X = arg1K1;
  if ((i_312X < n_138X)) {
    arg1K0 = (1 + i_312X);
    arg1K1 = (h_313X + ((long)((((long)(*((unsigned char *) ((-3 + x_139X) + i_312X))))))));
    goto L16805;}
  else {
    RSvalS = (((h_313X)<<2));
    goto START;}}
 L20270: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_143X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L20279: {
  v_314X = arg0K0;
  RSvalS = ((long)v_314X);
  goto START;}
 L16958: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L16955: {
  merged_arg0K0 = 0;
  merged_arg1K1 = x_130X;
  goto raise_exception1;}
 L17278: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L17346: {
  i_315X = arg1K0;
  if ((i_315X < ((long)count_160X))) {
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    SenvS = RSenvS;
    v_316X = hoistD1((*(areas_158X + i_315X)), ((*(areas_158X + i_315X)) + (*(sizes_159X + i_315X))), x_119X);
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    RSenvS = SenvS;
    if (v_316X) {
      arg1K0 = (1 + i_315X);
      goto L17346;}
    else {
      goto L17331;}}
  else {
    RSvalS = 13;
    goto START;}}
 L17331: {
  ShpS = start_hp_118X;
  merged_arg0K0 = 0;
  goto raise_exception;}
 L28737: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_164X;
  merged_arg1K2 = arg2_163X;
  merged_arg1K3 = (RSvalS);
  goto raise_exception3;}
 L28806: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_164X;
  merged_arg1K2 = arg2_163X;
  goto raise_exception2;}
 L28721: {
  resume_proc_317X = arg1K0;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  SenvS = RSenvS;
  do_gc();
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  RSenvS = SenvS;
  close_untraced_portsB_return_tag = 0;
  goto close_untraced_portsB;
 close_untraced_portsB_return_0:
  fputs("This is a Scheme48 heap image file.", port_168X);
  putc(10, port_168X);
  putc(12, port_168X);
  putc(10, port_168X);
  merged_arg1K0 = 15;
  merged_arg2K1 = port_168X;
  write_number_return_tag = 0;
  goto write_number;
 write_number_return_0:
  merged_arg1K0 = 4;
  merged_arg2K1 = port_168X;
  write_number_return_tag = 1;
  goto write_number;
 write_number_return_1:
  merged_arg1K0 = ((((Snewspace_beginS))>>2));
  merged_arg2K1 = port_168X;
  write_number_return_tag = 2;
  goto write_number;
 write_number_return_2:
  merged_arg1K0 = ((((ShpS))>>2));
  merged_arg2K1 = port_168X;
  write_number_return_tag = 3;
  goto write_number;
 write_number_return_3:
  merged_arg1K0 = resume_proc_317X;
  merged_arg2K1 = port_168X;
  write_number_return_tag = 4;
  goto write_number;
 write_number_return_4:
  putc(12, port_168X);
  *((long *) (ShpS)) = 1;
  fwrite((void *)(ShpS), sizeof(char), ((4 + (ShpS)) - (ShpS)), port_168X);
  fwrite((void *)(Snewspace_beginS), sizeof(char), ((ShpS) - (Snewspace_beginS)), port_168X);
  size_318X = (ShpS) - (Snewspace_beginS);
  fclose(port_168X);
  b_319X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_319X;
  e_320X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_320X;
  SlimitS = (Ssaved_limitS);
  ShpS = (Ssaved_hpS);
  arg1K0 = (Snewspace_beginS);
  goto L8818;}
 L17454: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L20885: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_172X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L21021: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_112X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L17546: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L17543: {
  v_321X = arg0K0;
  RSvalS = ((long)v_321X);
  goto START;}
 L17699: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L17696: {
  val_322X = arg1K0;
  RSvalS = val_322X;
  goto START;}
 L17691: {
  c_323X = arg1K0;
  *((long *) (5 + x_109X)) = c_323X;
  arg1K0 = c_323X;
  goto L17696;}
 L17817: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L17814: {
  val_324X = arg1K0;
  RSvalS = val_324X;
  goto START;}
 L21112: {
  key_325X = arg1K0;
  RSstackS = (4 + (RSstackS));
  arg2_326X = *((long *) (RSstackS));
  if ((3 == (3 & arg2_326X))) {
    if ((15 == (31 & ((((*((long *) (-7 + arg2_326X))))>>2))))) {
      if ((0 == (3 & (RSvalS)))) {
        mode_327X = (((RSvalS))>>2);
        arg1K0 = 0;
        goto L21299;}
      else {
        goto L21150;}}
    else {
      goto L21150;}}
  else {
    goto L21150;}}
 L21150: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_326X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L21299: {
  i_328X = arg1K0;
  if ((i_328X < (Snumber_of_portsS))) {
    if ((1 == (*((Sopen_vm_portsS) + i_328X)))) {
      arg1K0 = i_328X;
      arg1K1 = arg2_326X;
      goto L21186;}
    else {
      arg1K0 = (1 + i_328X);
      goto L21299;}}
  else {
    arg1K0 = -1;
    arg1K1 = arg2_326X;
    goto L21186;}}
 L21288: {
  i_329X = arg1K0;
  if ((i_329X < (Snumber_of_portsS))) {
    if ((1 == (*((Sopen_vm_portsS) + i_329X)))) {
      arg1K0 = i_329X;
      goto L21230;}
    else {
      arg1K0 = (1 + i_329X);
      goto L21288;}}
  else {
    arg1K0 = -1;
    goto L21230;}}
 L21258: {
  i_330X = arg1K0;
  if ((i_330X < (Snumber_of_portsS))) {
    *(new_ports_335X + i_330X) = (*((Sopen_portsS) + i_330X));
    *(new_vm_ports_336X + i_330X) = (*((Sopen_vm_portsS) + i_330X));
    arg1K0 = (1 + i_330X);
    goto L21258;}
  else {
    free((Sopen_portsS));
    free((Sopen_vm_portsS));
    Sopen_portsS = new_ports_335X;
    Sopen_vm_portsS = new_vm_ports_336X;
    Snumber_of_portsS = new_count_333X;
    arg1K0 = old_count_334X;
    arg1K1 = filename_343X;
    goto L21186;}}
 L21252: {
  i_331X = arg1K0;
  if ((i_331X < new_count_333X)) {
    *(new_vm_ports_336X + i_331X) = 1;
    arg1K0 = (1 + i_331X);
    goto L21252;}
  else {
    arg1K0 = 0;
    goto L21258;}}
 L21241: {
  merged_arg0K0 = 0;
  merged_arg1K1 = filename_343X;
  merged_arg1K2 = (((key_325X)<<2));
  goto raise_exception2;}
 L21230: {
  index_332X = arg1K0;
  if ((index_332X < 0)) {
    new_count_333X = 8 + (Snumber_of_portsS);
    old_count_334X = Snumber_of_portsS;
    new_ports_335X = (FILE **)malloc(sizeof(FILE *) * new_count_333X);
    new_vm_ports_336X = (long*)malloc(sizeof(long) * new_count_333X);
    if ((NULL == new_ports_335X)) {
      goto L21241;}
    else {
      if ((NULL == new_vm_ports_336X)) {
        goto L21241;}
      else {
        arg1K0 = 0;
        goto L21252;}}}
  else {
    arg1K0 = index_332X;
    arg1K1 = filename_343X;
    goto L21186;}}
 L21189: {
  port_337X = arg2K0;
  if ((NULL == port_337X)) {
    RSvalS = 1;
    goto START;}
  else {
    *((long *) (ShpS)) = 4118;
    ShpS = (4 + (ShpS));
    new_338X = 3 + (ShpS);
    ShpS = (16 + (ShpS));
    *((long *) (-4 + (ShpS))) = 0;
    *((long *) (-3 + new_338X)) = (((mode_327X)<<2));
    *((long *) (1 + new_338X)) = (((index_339X)<<2));
    *((long *) (5 + new_338X)) = 1;
    *((long *) (9 + new_338X)) = filename_340X;
    *((Sopen_portsS) + index_339X) = port_337X;
    *((Sopen_vm_portsS) + index_339X) = new_338X;
    RSvalS = new_338X;
    goto START;}}
 L21186: {
  index_339X = arg1K0;
  filename_340X = arg1K1;
  if ((index_339X < 0)) {
    merged_arg1K0 = filename_340X;
    collect_saving_temp_return_tag = 12;
    goto collect_saving_temp;
   collect_saving_temp_return_12:
    filename_343X = collect_saving_temp_return_value;
    arg1K0 = 0;
    goto L21288;}
  else {
    if ((2 == mode_327X)) {
      merged_arg1K0 = filename_340X;
      extract_string_return_tag = 1;
      goto extract_string;
     extract_string_return_1:
      spec_341X = extract_string_return_value;
      arg2K0 = (ps_open(spec_341X, "w"));
      goto L21189;}
    else {
      merged_arg1K0 = filename_340X;
      extract_string_return_tag = 2;
      goto extract_string;
     extract_string_return_2:
      spec_342X = extract_string_return_value;
      arg2K0 = (ps_open(spec_342X, "r"));
      goto L21189;}}}
 L21394: {
  if ((1 == (RSvalS))) {
    *((long *) (-3 + arg2_186X)) = 273;
    goto L21419;}
  else {
    if ((17 == (255 & (*((long *) (-3 + arg2_186X)))))) {
      *((long *) (-3 + arg2_186X)) = 529;
      goto L21419;}
    else {
      goto L21419;}}}
 L21422: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_186X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L21419: {
  RSvalS = 13;
  goto START;}
 L18236: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L18245: {
  v_344X = arg0K0;
  RSvalS = ((long)v_344X);
  goto START;}
 L24483: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_189X;
  merged_arg1K2 = arg2_188X;
  merged_arg1K3 = (RSvalS);
  goto raise_exception3;}
 L24476: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_189X;
  merged_arg1K2 = (((index_191X)<<2));
  merged_arg1K3 = (9 + (((((long)(Kchar_190X)))<<8)));
  goto raise_exception3;}
 L21599: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_101X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L21594: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_101X;
  merged_arg1K2 = (((index_193X)<<2));
  goto raise_exception2;}
 L21838: {
  len_346X = 1 + len_197X;
  *((long *) (ShpS)) = (62 + (((len_346X)<<8)));
  ShpS = (4 + (ShpS));
  new_345X = 3 + (ShpS);
  ShpS = ((ShpS) + (-4 & (3 + len_346X)));
  if ((0 < len_346X)) {
    *((long *) (-4 + (ShpS))) = 0;
    arg1K0 = new_345X;
    goto L21762;}
  else {
    arg1K0 = new_345X;
    goto L21762;}}
 L21787: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_195X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L21744: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (((len_197X)<<2));
  merged_arg1K2 = (9 + (((((long)(init_196X)))<<8)));
  goto raise_exception2;}
 L21762: {
  string_347X = arg1K0;
  arg1K0 = (-1 + len_197X);
  goto L21764;}
 L21764: {
  i_348X = arg1K0;
  if ((i_348X < 0)) {
    RSvalS = string_347X;
    goto START;}
  else {
    *((unsigned char *) ((-3 + string_347X) + i_348X)) = ((unsigned char)((long)(init_196X)));
    arg1K0 = (-1 + i_348X);
    goto L21764;}}
 L24657: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_98X;
  merged_arg1K2 = arg2_99X;
  merged_arg1K3 = (RSvalS);
  goto raise_exception3;}
 L24650: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_98X;
  merged_arg1K2 = (((index_200X)<<2));
  merged_arg1K3 = (((value_199X)<<2));
  goto raise_exception3;}
 L22014: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_97X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L22009: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_97X;
  merged_arg1K2 = (((index_202X)<<2));
  goto raise_exception2;}
 L22251: {
  *((long *) (ShpS)) = (66 + (((len_94X)<<8)));
  ShpS = (4 + (ShpS));
  new_349X = 3 + (ShpS);
  ShpS = ((ShpS) + (-4 & (3 + len_94X)));
  if ((0 < len_94X)) {
    *((long *) (-4 + (ShpS))) = 0;
    arg1K0 = new_349X;
    goto L22177;}
  else {
    arg1K0 = new_349X;
    goto L22177;}}
 L22159: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (((len_94X)<<2));
  merged_arg1K2 = (((init_95X)<<2));
  goto raise_exception2;}
 L22177: {
  code_vector_350X = arg1K0;
  arg1K0 = (-1 + len_94X);
  goto L22179;}
 L22179: {
  i_351X = arg1K0;
  if ((i_351X < 0)) {
    RSvalS = code_vector_350X;
    goto START;}
  else {
    *((unsigned char *) ((-3 + code_vector_350X) + i_351X)) = ((unsigned char)init_95X);
    arg1K0 = (-1 + i_351X);
    goto L22179;}}
 L25455: {
  merged_arg0K0 = 1;
  merged_arg1K1 = arg3_206X;
  merged_arg1K2 = (((((long)b_90X))<<2));
  merged_arg1K3 = (((index_91X)<<2));
  merged_arg1K4 = x_92X;
  goto raise_exception4;}
 L24824: {
  merged_arg0K0 = 1;
  merged_arg1K1 = arg2_207X;
  merged_arg1K2 = (((((long)b_87X))<<2));
  merged_arg1K3 = (((index_88X)<<2));
  goto raise_exception3;}
 L25001: {
  merged_arg0K0 = 1;
  merged_arg1K1 = (((((long)b_83X))<<2));
  merged_arg1K2 = (((len_84X)<<2));
  merged_arg1K3 = x_85X;
  goto raise_exception3;}
 L25017: {
  val_352X = arg1K0;
  arg1K0 = (-1 + len_84X);
  goto L25019;}
 L25019: {
  i_353X = arg1K0;
  if ((i_353X < 0)) {
    RSvalS = val_352X;
    goto START;}
  else {
    *((long *) ((-3 + val_352X) + (((i_353X)<<2)))) = init_355X;
    arg1K0 = (-1 + i_353X);
    goto L25019;}}
 L25006: {
  okayP_354X = arg3K0;
  init_355X = arg1K1;
  if (okayP_354X) {
    len_357X = ((len_84X)<<2);
    *((long *) (ShpS)) = (2 + (((((((len_357X)<<6)) + ((long)b_83X)))<<2)));
    ShpS = (4 + (ShpS));
    new_356X = 3 + (ShpS);
    ShpS = ((ShpS) + (-4 & (3 + len_357X)));
    if ((0 < len_357X)) {
      *((long *) (-4 + (ShpS))) = 0;
      arg1K0 = new_356X;
      goto L25017;}
    else {
      arg1K0 = new_356X;
      goto L25017;}}
  else {
    merged_arg0K0 = 1;
    merged_arg1K1 = (((((long)b_83X))<<2));
    merged_arg1K2 = (((len_84X)<<2));
    merged_arg1K3 = init_355X;
    goto raise_exception3;}}
 L25711: {
  merged_arg0K0 = 2;
  merged_arg1K1 = arg2_80X;
  merged_arg1K2 = (((((long)b_78X))<<2));
  merged_arg1K3 = (((((long)b_77X))<<2));
  merged_arg1K4 = x_79X;
  goto raise_exception4;}
 L25238: {
  merged_arg0K0 = 2;
  merged_arg1K1 = x_209X;
  merged_arg1K2 = (((((long)b_210X))<<2));
  merged_arg1K3 = (((((long)b_211X))<<2));
  goto raise_exception3;}
 L16612: {
  b_361X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  b_360X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  len_359X = ((((long)b_361X))<<2);
  *((long *) (ShpS)) = (2 + (((((((len_359X)<<6)) + ((long)b_360X)))<<2)));
  ShpS = (4 + (ShpS));
  new_358X = 3 + (ShpS);
  ShpS = ((ShpS) + (-4 & (3 + len_359X)));
  if ((0 < len_359X)) {
    *((long *) (-4 + (ShpS))) = 0;
    arg1K0 = new_358X;
    goto L16620;}
  else {
    arg1K0 = new_358X;
    goto L16620;}}
 L16620: {
  new_362X = arg1K0;
  if ((((long)b_361X) < 1)) {
    goto L16651;}
  else {
    *((long *) (-7 + (new_362X + (((((long)b_361X))<<2))))) = (RSvalS);
    arg1K0 = (-2 + ((long)b_361X));
    goto L16629;}}
 L16651: {
  RSvalS = new_362X;
  goto START;}
 L16629: {
  i_363X = arg1K0;
  if ((i_363X < 0)) {
    goto L16651;}
  else {
    RSstackS = (4 + (RSstackS));
    *((long *) ((-3 + new_362X) + (((i_363X)<<2)))) = (*((long *) (RSstackS)));
    arg1K0 = (-1 + i_363X);
    goto L16629;}}
 L22411: {
  merged_arg0K0 = 1;
  merged_arg1K1 = x_212X;
  merged_arg1K2 = (((((long)b_213X))<<2));
  goto raise_exception2;}
 L22698: {
  RSvalS = (((result_68X)<<2));
  goto START;}
 L22695: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_220X;
  merged_arg1K2 = x_72X;
  goto raise_exception2;}
 L23255: {
  n_364X = arg1K0;
  RSvalS = (((n_364X)<<2));
  goto START;}
 L23251: {
  b_365X = arg1K0;
  c_366X = a_367X % b_365X;
  if ((a_224X < 0)) {
    arg1K0 = (0 - c_366X);
    goto L23255;}
  else {
    arg1K0 = c_366X;
    goto L23255;}}
 L23249: {
  a_367X = arg1K0;
  if ((b_225X < 0)) {
    arg1K0 = (0 - b_225X);
    goto L23251;}
  else {
    arg1K0 = b_225X;
    goto L23251;}}
 L23388: {
  if ((536870911 < c_369X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_58X;
    merged_arg1K2 = x_226X;
    goto raise_exception2;}
  else {
    RSvalS = (((c_369X)<<2));
    goto START;}}
 L23385: {
  RSvalS = ((((0 - c_369X))<<2));
  goto START;}
 L23382: {
  b_368X = arg1K0;
  c_369X = a_370X / b_368X;
  if ((a_227X < 0)) {
    if ((b_228X < 0)) {
      goto L23388;}
    else {
      goto L23385;}}
  else {
    if ((b_228X < 0)) {
      goto L23385;}
    else {
      goto L23388;}}}
 L23380: {
  a_370X = arg1K0;
  if ((b_228X < 0)) {
    arg1K0 = (0 - b_228X);
    goto L23382;}
  else {
    arg1K0 = b_228X;
    goto L23382;}}
 L23758: {
  if ((536870911 < c_379X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_55X;
    merged_arg1K2 = x_231X;
    goto raise_exception2;}
  else {
    arg1K0 = (((c_379X)<<2));
    goto L23721;}}
 L23734: {
  n_371X = arg1K0;
  if ((0 == (((n_371X)<<2)))) {
    RSvalS = q_375X;
    goto START;}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_55X;
    merged_arg1K2 = x_231X;
    goto raise_exception2;}}
 L23730: {
  b_372X = arg1K0;
  c_373X = a_374X % b_372X;
  if ((a_376X < 0)) {
    arg1K0 = (0 - c_373X);
    goto L23734;}
  else {
    arg1K0 = c_373X;
    goto L23734;}}
 L23728: {
  a_374X = arg1K0;
  if ((b_377X < 0)) {
    arg1K0 = (0 - b_377X);
    goto L23730;}
  else {
    arg1K0 = b_377X;
    goto L23730;}}
 L23721: {
  q_375X = arg1K0;
  if ((0 == x_231X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_55X;
    merged_arg1K2 = x_231X;
    goto raise_exception2;}
  else {
    a_376X = ((arg2_55X)>>2);
    b_377X = ((x_231X)>>2);
    if ((a_376X < 0)) {
      arg1K0 = (0 - a_376X);
      goto L23728;}
    else {
      arg1K0 = a_376X;
      goto L23728;}}}
 L23717: {
  b_378X = arg1K0;
  c_379X = a_380X / b_378X;
  if ((a_232X < 0)) {
    if ((b_233X < 0)) {
      goto L23758;}
    else {
      arg1K0 = ((((0 - c_379X))<<2));
      goto L23721;}}
  else {
    if ((b_233X < 0)) {
      arg1K0 = ((((0 - c_379X))<<2));
      goto L23721;}
    else {
      goto L23758;}}}
 L23715: {
  a_380X = arg1K0;
  if ((b_233X < 0)) {
    arg1K0 = (0 - b_233X);
    goto L23717;}
  else {
    arg1K0 = b_233X;
    goto L23717;}}
 L23921: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_54X;
  merged_arg1K2 = x_234X;
  goto raise_exception2;}
 L24070: {
  if ((536870911 < c_388X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_53X;
    merged_arg1K2 = x_236X;
    goto raise_exception2;}
  else {
    RSvalS = (((c_388X)<<2));
    goto START;}}
 L24063: {
  if ((536870912 < c_388X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_53X;
    merged_arg1K2 = x_236X;
    goto raise_exception2;}
  else {
    RSvalS = ((((0 - c_388X))<<2));
    goto START;}}
 L24060: {
  if ((536870911 < lo_c_386X)) {
    goto L24055;}
  else {
    if ((8192 < mid_c_387X)) {
      goto L24055;}
    else {
      if ((a_237X < 0)) {
        if ((b_238X < 0)) {
          goto L24070;}
        else {
          goto L24063;}}
      else {
        if ((b_238X < 0)) {
          goto L24063;}
        else {
          goto L24070;}}}}}
 L24055: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_53X;
  merged_arg1K2 = x_236X;
  goto raise_exception2;}
 L24040: {
  b_381X = arg1K0;
  lo_a_382X = 65535 & a_389X;
  lo_b_383X = 65535 & b_381X;
  hi_a_384X = 65535 & (((a_389X)>>16));
  hi_b_385X = 65535 & (((b_381X)>>16));
  lo_c_386X = PS_SMALL_MULTIPLY(lo_a_382X, lo_b_383X);
  mid_c_387X = (PS_SMALL_MULTIPLY(lo_a_382X, hi_b_385X)) + (PS_SMALL_MULTIPLY(lo_b_383X, hi_a_384X));
  c_388X = lo_c_386X + (((mid_c_387X)<<16));
  if ((0 < hi_a_384X)) {
    if ((0 < hi_b_385X)) {
      goto L24055;}
    else {
      goto L24060;}}
  else {
    goto L24060;}}
 L24038: {
  a_389X = arg1K0;
  if ((b_238X < 0)) {
    arg1K0 = (0 - b_238X);
    goto L24040;}
  else {
    arg1K0 = b_238X;
    goto L24040;}}
 L24242: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_239X;
  merged_arg1K2 = x_52X;
  goto raise_exception2;}
 L28551: {
  offset_390X = arg1K0;
  RScode_pointerS = ((RScode_pointerS) + offset_390X);
  goto START;}
 L25323: {
  RSvalS = proc_36X;
  merged_arg0K0 = 0;
  goto perform_application;}
 L15129: {
  RSstackS = (4 + (RSstackS));
  list_391X = *((long *) (RSstackS));
  a_392X = *((long *) (-3 + list_391X));
  *((long *) (ShpS)) = 2050;
  ShpS = (4 + (ShpS));
  new_393X = 3 + (ShpS);
  ShpS = (8 + (ShpS));
  *((long *) (-4 + (ShpS))) = 0;
  *((long *) (-3 + new_393X)) = a_392X;
  *((long *) (1 + new_393X)) = 25;
  arg1K0 = (*((long *) (1 + list_391X)));
  arg1K1 = new_393X;
  goto L15139;}
 L15139: {
  l_394X = arg1K0;
  last_395X = arg1K1;
  if ((25 == l_394X)) {
    *((long *) (RSstackS)) = new_393X;
    RSstackS = (-4 + (RSstackS));
    SnargsS = 64;
    goto L25323;}
  else {
    a_397X = *((long *) (-3 + l_394X));
    *((long *) (ShpS)) = 2050;
    ShpS = (4 + (ShpS));
    new_396X = 3 + (ShpS);
    ShpS = (8 + (ShpS));
    *((long *) (-4 + (ShpS))) = 0;
    *((long *) (-3 + new_396X)) = a_397X;
    *((long *) (1 + new_396X)) = 25;
    *((long *) (1 + last_395X)) = new_396X;
    arg1K0 = (*((long *) (1 + l_394X)));
    arg1K1 = new_396X;
    goto L15139;}}
 L15355: {
  i_398X = arg1K0;
  l_399X = arg1K1;
  if ((0 < i_398X)) {
    *((long *) (RSstackS)) = (*((long *) (-3 + l_399X)));
    RSstackS = (-4 + (RSstackS));
    arg1K0 = (-1 + i_398X);
    arg1K1 = (*((long *) (1 + l_399X)));
    goto L15355;}
  else {
    if ((0 == overflow_401X)) {
      SnargsS = nargs_407X;
      goto L25323;}
    else {
      *((long *) (RSstackS)) = l_399X;
      RSstackS = (-4 + (RSstackS));
      space_400X = 3 * overflow_401X;
      if ((((ShpS) + (((space_400X)<<2))) < (SlimitS))) {
        goto L15129;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 13;
        goto collect_saving_temp;
       collect_saving_temp_return_13:
        if ((((ShpS) + (((space_400X)<<2))) < (SlimitS))) {
          goto L15129;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L15129;}}}}}
 L15319: {
  overflow_401X = arg1K0;
  arg1K0 = (len_404X - overflow_401X);
  arg1K1 = args_408X;
  goto L15355;}
 L25357: {
  merged_arg1K0 = 25;
  merged_arg1K1 = ((long)(*((unsigned char *) (RScode_pointerS))));
  pop_args_list_return_tag = 2;
  goto pop_args_list;
 pop_args_list_return_2:
  args_402X = pop_args_list_return_value;
  merged_arg0K0 = 0;
  merged_arg1K1 = proc_36X;
  merged_arg1K2 = args_402X;
  merged_arg1K3 = (RSvalS);
  goto raise_exception3;}
 L25355: {
  fast_403X = arg1K0;
  len_404X = arg1K1;
  slow_405X = arg1K2;
  move_slowP_406X = arg3K3;
  if ((25 == fast_403X)) {
    args_408X = RSvalS;
    nargs_407X = len_404X + ((long)(*((unsigned char *) (RScode_pointerS))));
    if ((63 < nargs_407X)) {
      arg1K0 = (-63 + nargs_407X);
      goto L15319;}
    else {
      arg1K0 = 0;
      goto L15319;}}
  else {
    if ((3 == (3 & fast_403X))) {
      if ((0 == (31 & ((((*((long *) (-7 + fast_403X))))>>2))))) {
        if (move_slowP_406X) {
          if ((fast_403X == slow_405X)) {
            merged_arg1K0 = 25;
            merged_arg1K1 = ((long)(*((unsigned char *) (RScode_pointerS))));
            pop_args_list_return_tag = 3;
            goto pop_args_list;
           pop_args_list_return_3:
            args_409X = pop_args_list_return_value;
            merged_arg0K0 = 0;
            merged_arg1K1 = proc_36X;
            merged_arg1K2 = args_409X;
            merged_arg1K3 = (RSvalS);
            goto raise_exception3;}
          else {
            arg1K0 = (*((long *) (1 + fast_403X)));
            arg1K1 = (1 + len_404X);
            arg1K2 = (*((long *) (1 + slow_405X)));
            arg3K3 = 0;
            goto L25355;}}
        else {
          arg1K0 = (*((long *) (1 + fast_403X)));
          arg1K1 = (1 + len_404X);
          arg1K2 = slow_405X;
          arg3K3 = 1;
          goto L25355;}}
      else {
        goto L25357;}}
    else {
      goto L25357;}}}
 L15893: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L15910: {
  loc_410X = arg1K0;
  arg_411X = arg1K1;
  if (((RSstackS) < arg_411X)) {
    *((long *) loc_410X) = (*((long *) arg_411X));
    arg1K0 = (-4 + loc_410X);
    arg1K1 = (-4 + arg_411X);
    goto L15910;}
  else {
    RSstackS = loc_410X;
    goto L15893;}}
 L28344: {
  v_412X = arg1K0;
  ScontS = v_412X;
  goto START;}
 L28226: {
  key_413X = arg1K0;
  merged_arg1K0 = ((RScode_pointerS) + ((((((long)b_254X))<<8)) + ((long)b_255X)));
  merged_arg1K1 = ((long)b_256X);
  merged_arg1K2 = key_413X;
  push_continuationB_return_tag = 0;
  goto push_continuationB;
 push_continuationB_return_0:
  goto START;}
 L28243: {
  v_414X = arg1K0;
  merged_arg1K0 = v_414X;
  copy_stack_into_heap_return_tag = 0;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_0:
  arg1K0 = 0;
  goto L28226;}
 L28083: {
  space_415X = arg1K0;
  if ((((ShpS) + (((space_415X)<<2))) < (SlimitS))) {
    arg1K0 = 0;
    goto L28085;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 14;
    goto collect_saving_temp;
   collect_saving_temp_return_14:
    if ((((ShpS) + (((space_415X)<<2))) < (SlimitS))) {
      arg1K0 = 0;
      goto L28085;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      arg1K0 = 0;
      goto L28085;}}}
 L28085: {
  key_416X = arg1K0;
  p_417X = RSenvS;
  if ((p_417X < (Sstack_beginS))) {
    arg1K0 = (RSenvS);
    goto L28087;}
  else {
    if (((Sstack_endS) < p_417X)) {
      arg1K0 = (RSenvS);
      goto L28087;}
    else {
      merged_arg1K0 = (RSenvS);
      merged_arg1K1 = (ScontS);
      merged_arg1K2 = key_416X;
      merged_arg0K3 = 0;
      save_env_in_heap_return_tag = 0;
      goto save_env_in_heap;
     save_env_in_heap_return_0:
      v_418X = save_env_in_heap_return_value;
      RSenvS = v_418X;
      arg1K0 = (RSenvS);
      goto L28087;}}}
 L28087: {
  env_419X = arg1K0;
  if (((12 + (ShpS)) < (SlimitS))) {
    goto L28089;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 15;
    goto collect_saving_temp;
   collect_saving_temp_return_15:
    if (((12 + (ShpS)) < (SlimitS))) {
      goto L28089;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      goto L28089;}}}
 L28089: {
  b_420X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  a_421X = *((long *) ((-3 + (StemplateS)) + (((((long)b_420X))<<2))));
  *((long *) (ShpS)) = 2062;
  ShpS = (4 + (ShpS));
  new_422X = 3 + (ShpS);
  ShpS = (8 + (ShpS));
  *((long *) (-4 + (ShpS))) = 0;
  *((long *) (-3 + new_422X)) = a_421X;
  *((long *) (1 + new_422X)) = env_419X;
  if ((3 == (3 & new_422X))) {
    if ((0 == (128 & (*((long *) (-7 + new_422X)))))) {
      *((long *) (-7 + new_422X)) = (128 | (*((long *) (-7 + new_422X))));
      arg1K0 = new_422X;
      goto L28093;}
    else {
      arg1K0 = new_422X;
      goto L28093;}}
  else {
    arg1K0 = new_422X;
    goto L28093;}}
 L27985: {
  env_423X = arg1K0;
  i_424X = arg1K1;
  if ((0 == i_424X)) {
    b_425X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    *((long *) ((-3 + env_423X) + (((((long)b_425X))<<2)))) = (RSvalS);
    RSvalS = 13;
    goto START;}
  else {
    arg1K0 = (*((long *) (-3 + env_423X)));
    arg1K1 = (-1 + i_424X);
    goto L27985;}}
 L27677: {
  env_426X = arg1K0;
  i_427X = arg1K1;
  if ((0 == i_427X)) {
    b_428X = *((unsigned char *) (RScode_pointerS));
    RScode_pointerS = (1 + (RScode_pointerS));
    RSvalS = (*((long *) ((-3 + env_426X) + (((((long)b_428X))<<2)))));
    if ((529 == (RSvalS))) {
      merged_arg0K0 = 2;
      goto raise_exception;}
    else {
      goto START;}}
  else {
    arg1K0 = (*((long *) (-3 + env_426X)));
    arg1K1 = (-1 + i_427X);
    goto L27677;}}
 L27913: {
  args_429X = arg1K0;
  RSvalS = args_429X;
  SnargsS = (1 + ((long)b_268X));
  goto START;}
 L27845: {
  b_432X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  len_431X = 4 + (((((long)b_432X))<<2));
  *((long *) (ShpS)) = (10 + (((len_431X)<<8)));
  ShpS = (4 + (ShpS));
  new_430X = 3 + (ShpS);
  ShpS = ((ShpS) + (-4 & (3 + len_431X)));
  if ((0 < len_431X)) {
    *((long *) (-4 + (ShpS))) = 0;
    arg1K0 = new_430X;
    goto L9735;}
  else {
    arg1K0 = new_430X;
    goto L9735;}}
 L27779: {
  b_433X = *((unsigned char *) (RScode_pointerS));
  RScode_pointerS = (1 + (RScode_pointerS));
  *((long *) (RSstackS)) = (RSenvS);
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = (1034 + (((((long)b_433X))<<10)));
  RSstackS = (-4 + (RSstackS));
  RSenvS = (11 + (RSstackS));
  goto START;}
 L27812: {
  v_434X = arg1K0;
  merged_arg1K0 = v_434X;
  copy_stack_into_heap_return_tag = 1;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_1:
  goto L27779;}
 L31124: {
  merged_arg0K0 = 0;
  merged_arg1K1 = proc_151X;
  goto raise_exception1;}
 L31071: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L31052: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L30931: {
  v_435X = arg0K0;
  RSvalS = ((long)v_435X);
  goto START;}
 L30913: {
  v_436X = arg0K0;
  RSvalS = ((long)v_436X);
  goto START;}
 L30891: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L30870: {
  RSvalS = x_185X;
  goto START;}
 L30855: {
  v_437X = arg0K0;
  RSvalS = ((long)v_437X);
  goto START;}
 L30819: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L30788: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (RSvalS);
  goto raise_exception1;}
 L30741: {
  v_438X = arg0K0;
  RSvalS = ((long)v_438X);
  goto START;}
 L30723: {
  v_439X = arg0K0;
  RSvalS = ((long)v_439X);
  goto START;}
 L30701: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (((x_73X)<<2));
  goto raise_exception1;}
 L30670: {
  v_440X = arg0K0;
  RSvalS = ((long)v_440X);
  goto START;}
 L30658: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_215X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L30641: {
  v_441X = arg0K0;
  RSvalS = ((long)v_441X);
  goto START;}
 L30629: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_217X;
  merged_arg1K2 = (RSvalS);
  goto raise_exception2;}
 L30614: {
  v_442X = arg0K0;
  RSvalS = ((long)v_442X);
  goto START;}
 L30486: {
  r_443X = arg1K0;
  if ((536870911 < r_443X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = x_222X;
    goto raise_exception1;}
  else {
    RSvalS = (((r_443X)<<2));
    goto START;}}
 L30298: {
  v_444X = arg0K0;
  RSvalS = ((long)v_444X);
  goto START;}
 L30274: {
  v_445X = arg0K0;
  RSvalS = ((long)v_445X);
  goto START;}
 L30191: {
  RSvalS = 1;
  goto START;}
 L30164: {
  RSvalS = 1;
  goto START;}
 L30137: {
  RSvalS = 1;
  goto START;}
 L30110: {
  RSvalS = 1;
  goto START;}
 L30092: {
  v_446X = arg0K0;
  RSvalS = ((long)v_446X);
  goto START;}
 L30078: {
  v_447X = arg0K0;
  RSvalS = ((long)v_447X);
  goto START;}
 L29957: {
  v_448X = arg1K0;
  ScontS = v_448X;
  SnargsS = 0;
  merged_arg0K0 = 0;
  goto perform_application;}
 L29927: {
  key_449X = arg1K0;
  merged_arg1K0 = key_449X;
  merged_arg0K1 = 2;
  preserve_continuation_return_tag = 0;
  goto preserve_continuation;
 preserve_continuation_return_0:
  v_450X = preserve_continuation_return_value;
  RSvalS = v_450X;
  goto START;}
 raise_exception: {
  args_676X = merged_arg0K0;{
  push_exception_continuationB_return_tag = 0;
  goto push_exception_continuationB;
 push_exception_continuationB_return_0:
  *((long *) (RSstackS)) = (((((long)(*((unsigned char *) (-1 + ((RScode_pointerS) - ((long)args_676X)))))))<<2));
  RSstackS = (-4 + (RSstackS));
  merged_arg0K0 = 0;
  goto raise;}}

 return_values: {
  nargs_674X = merged_arg1K0;{
  if ((1 == nargs_674X)) {
    RSstackS = (4 + (RSstackS));
    RSvalS = (*((long *) (RSstackS)));
    cont_451X = ScontS;
    tem_452X = *((long *) (5 + cont_451X));
    pc_453X = *((long *) (1 + cont_451X));
    StemplateS = tem_452X;
    RScode_pointerS = ((-3 + (*((long *) (-3 + tem_452X)))) + (((pc_453X)>>2)));
    RSenvS = (*((long *) (9 + cont_451X)));
    ScontS = (*((long *) (-3 + cont_451X)));
    RSstackS = (9 + cont_451X);
    goto START;}
  else {
    if (((ScontS) == (Sbottom_of_stackS))) {
      arg1K0 = (*((long *) (-3 + (Sbottom_of_stackS))));
      goto L27389;}
    else {
      arg1K0 = (ScontS);
      goto L27389;}}}
 L27339: {
  args_465X = arg1K0;
  merged_arg0K0 = 0;
  merged_arg1K1 = args_465X;
  goto raise_exception1;}
 L27286: {
  SnargsS = nargs_674X;
  RSvalS = (*((long *) (13 + cont_457X)));
  merged_arg0K0 = 0;
  goto perform_application;}
 L27389: {
  cont_457X = arg1K0;
  if ((3 == (3 & cont_457X))) {
    if ((8 == (31 & ((((*((long *) (-7 + cont_457X))))>>2))))) {
      next_op_459X = *((unsigned char *) ((-3 + (*((long *) (-3 + (*((long *) (5 + cont_457X))))))) + ((((*((long *) (1 + cont_457X))))>>2))));
      if ((31 == ((long)next_op_459X))) {
        cont_462X = ScontS;
        tem_463X = *((long *) (5 + cont_462X));
        pc_464X = *((long *) (1 + cont_462X));
        StemplateS = tem_463X;
        RScode_pointerS = ((-3 + (*((long *) (-3 + tem_463X)))) + (((pc_464X)>>2)));
        RSenvS = (*((long *) (9 + cont_462X)));
        ScontS = (*((long *) (-3 + cont_462X)));
        RSstackS = (9 + cont_462X);
        goto START;}
      else {
        if ((27 == ((long)next_op_459X))) {
          next_461X = *((long *) (-3 + (ScontS)));
          if (((ScontS) == (Sbottom_of_stackS))) {
            *((long *) (-3 + (ScontS))) = (*((long *) (-3 + next_461X)));
            goto L27286;}
          else {
            ScontS = next_461X;
            goto L27286;}}
        else {
          if ((64 == nargs_674X)) {
            RSstackS = (4 + (RSstackS));
            merged_arg1K0 = (*((long *) (RSstackS)));
            merged_arg1K1 = 63;
            pop_args_list_return_tag = 4;
            goto pop_args_list;
           pop_args_list_return_4:
            v_458X = pop_args_list_return_value;
            arg1K0 = v_458X;
            goto L27339;}
          else {
            merged_arg1K0 = 25;
            merged_arg1K1 = nargs_674X;
            pop_args_list_return_tag = 5;
            goto pop_args_list;
           pop_args_list_return_5:
            v_460X = pop_args_list_return_value;
            arg1K0 = v_460X;
            goto L27339;}}}}
    else {
      goto L27397;}}
  else {
    goto L27397;}}
 L27397: {
  if ((64 == nargs_674X)) {
    RSstackS = (4 + (RSstackS));
    merged_arg1K0 = (*((long *) (RSstackS)));
    merged_arg1K1 = 63;
    pop_args_list_return_tag = 6;
    goto pop_args_list;
   pop_args_list_return_6:
    v_456X = pop_args_list_return_value;
    arg1K0 = v_456X;
    goto L27427;}
  else {
    merged_arg1K0 = 25;
    merged_arg1K1 = nargs_674X;
    pop_args_list_return_tag = 7;
    goto pop_args_list;
   pop_args_list_return_7:
    v_455X = pop_args_list_return_value;
    arg1K0 = v_455X;
    goto L27427;}}
 L27427: {
  args_454X = arg1K0;
  merged_arg0K0 = 0;
  merged_arg1K1 = args_454X;
  goto raise_exception1;}}

 raise_exception1: {
  args_671X = merged_arg0K0;
  a1_672X = merged_arg1K1;{
  push_exception_continuationB_return_tag = 1;
  goto push_exception_continuationB;
 push_exception_continuationB_return_1:
  *((long *) (RSstackS)) = (((((long)(*((unsigned char *) (-1 + ((RScode_pointerS) - ((long)args_671X)))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a1_672X;
  RSstackS = (-4 + (RSstackS));
  merged_arg0K0 = 1;
  goto raise;}}

 raise_exception2: {
  args_667X = merged_arg0K0;
  a1_668X = merged_arg1K1;
  a2_669X = merged_arg1K2;{
  push_exception_continuationB_return_tag = 2;
  goto push_exception_continuationB;
 push_exception_continuationB_return_2:
  *((long *) (RSstackS)) = (((((long)(*((unsigned char *) (-1 + ((RScode_pointerS) - ((long)args_667X)))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a1_668X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a2_669X;
  RSstackS = (-4 + (RSstackS));
  merged_arg0K0 = 2;
  goto raise;}}

 raise_exception3: {
  args_662X = merged_arg0K0;
  a1_663X = merged_arg1K1;
  a2_664X = merged_arg1K2;
  a3_665X = merged_arg1K3;{
  push_exception_continuationB_return_tag = 3;
  goto push_exception_continuationB;
 push_exception_continuationB_return_3:
  *((long *) (RSstackS)) = (((((long)(*((unsigned char *) (-1 + ((RScode_pointerS) - ((long)args_662X)))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a1_663X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a2_664X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a3_665X;
  RSstackS = (-4 + (RSstackS));
  merged_arg0K0 = 3;
  goto raise;}}

 raise_exception4: {
  args_656X = merged_arg0K0;
  a1_657X = merged_arg1K1;
  a2_658X = merged_arg1K2;
  a3_659X = merged_arg1K3;
  a4_660X = merged_arg1K4;{
  push_exception_continuationB_return_tag = 4;
  goto push_exception_continuationB;
 push_exception_continuationB_return_4:
  *((long *) (RSstackS)) = (((((long)(*((unsigned char *) (-1 + ((RScode_pointerS) - ((long)args_656X)))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a1_657X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a2_658X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a3_659X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a4_660X;
  RSstackS = (-4 + (RSstackS));
  merged_arg0K0 = 4;
  goto raise;}}

 application_exception: {
  byte_args_654X = merged_arg0K0;{
  if ((64 == (SnargsS))) {
    RSstackS = (4 + (RSstackS));
    merged_arg1K0 = (*((long *) (RSstackS)));
    merged_arg1K1 = 63;
    pop_args_list_return_tag = 8;
    goto pop_args_list;
   pop_args_list_return_8:
    v_467X = pop_args_list_return_value;
    arg1K0 = v_467X;
    goto L24404;}
  else {
    merged_arg1K0 = 25;
    merged_arg1K1 = (SnargsS);
    pop_args_list_return_tag = 9;
    goto pop_args_list;
   pop_args_list_return_9:
    v_466X = pop_args_list_return_value;
    arg1K0 = v_466X;
    goto L24404;}}
 L24404: {
  args_468X = arg1K0;
  a1_469X = RSvalS;
  *((long *) (RSstackS)) = (((((long)(*((unsigned char *) (-1 + ((RScode_pointerS) - ((long)byte_args_654X)))))))<<2));
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = a1_469X;
  RSstackS = (-4 + (RSstackS));
  *((long *) (RSstackS)) = args_468X;
  RSstackS = (-4 + (RSstackS));
  merged_arg0K0 = 2;
  goto raise;}}

 raise: {
  nargs_652X = merged_arg0K0;{
  SnargsS = (1 + ((long)nargs_652X));
  RSvalS = (Sexception_handlerS);
  obj_470X = RSvalS;
  if ((3 == (3 & obj_470X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_470X))))>>2))))) {
      goto L15953;}
    else {
      goto L16015;}}
  else {
    goto L16015;}}
 L16015: {
  TTerror("exception handler is not a closure", 0, 0, 0, 0);
  goto L15953;}
 L15955: {
  if ((Sexception_space_usedPS)) {
    x_473X = (Sstack_limitS) < (-296 + (RSstackS));
    if (x_473X) {
      goto L15968;}
    else {
      TTerror("no space on stack to reserve exception space", 0, 0, 0, 0);
      goto L15968;}}
  else {
    goto L15957;}}
 L15989: {
  v_472X = arg1K0;
  merged_arg1K0 = v_472X;
  copy_stack_into_heap_return_tag = 2;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_2:
  goto L15955;}
 L15957: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L15968: {
  Sstack_limitS = (296 + (Sstack_beginS));
  Sexception_space_usedPS = 0;
  goto L15957;}
 L15953: {
  if (((Sstack_limitS) < (-296 + (RSstackS)))) {
    goto L15955;}
  else {
    space_471X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
    if ((((ShpS) + (((space_471X)<<2))) < (SlimitS))) {
      arg1K0 = 0;
      goto L15989;}
    else {
      merged_arg1K0 = 0;
      collect_saving_temp_return_tag = 16;
      goto collect_saving_temp;
     collect_saving_temp_return_16:
      if ((((ShpS) + (((space_471X)<<2))) < (SlimitS))) {
        arg1K0 = 0;
        goto L15989;}
      else {
        TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
        arg1K0 = 0;
        goto L15989;}}}}}

 perform_application: {
  bytes_consumed_650X = merged_arg0K0;{
  obj_475X = RSvalS;
  if ((3 == (3 & obj_475X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_475X))))>>2))))) {
      RSenvS = (*((long *) (1 + (RSvalS))));
      tem_474X = *((long *) (-3 + (RSvalS)));
      StemplateS = tem_474X;
      RScode_pointerS = (-3 + (*((long *) (-3 + tem_474X))));
      if ((0 == ((Spending_interruptsS) & (Senabled_interruptsS)))) {
        goto START;}
      else {
        goto handle_interrupt;}}
    else {
      goto L15768;}}
  else {
    goto L15768;}}
 L15768: {
  merged_arg0K0 = bytes_consumed_650X;
  goto application_exception;}}

 handle_interrupt: {
{ if (((Sstack_limitS) < (-276 + (RSstackS)))) {
    arg1K0 = 0;
    goto L16057;}
  else {
    space_476X = 1 + (((((Sstack_endS) - (RSstackS)))>>2));
    if ((((ShpS) + (((space_476X)<<2))) < (SlimitS))) {
      arg1K0 = 0;
      goto L16297;}
    else {
      merged_arg1K0 = 0;
      collect_saving_temp_return_tag = 17;
      goto collect_saving_temp;
     collect_saving_temp_return_17:
      if ((((ShpS) + (((space_476X)<<2))) < (SlimitS))) {
        arg1K0 = 0;
        goto L16297;}
      else {
        TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
        arg1K0 = 0;
        goto L16297;}}}}
 L16057: {
  key_483X = arg1K0;
  n_484X = (Spending_interruptsS) & (Senabled_interruptsS);
  arg1K0 = 0;
  arg1K1 = 1;
  goto L16285;}
 L16297: {
  v_482X = arg1K0;
  merged_arg1K0 = v_482X;
  copy_stack_into_heap_return_tag = 3;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_3:
  arg1K0 = 0;
  goto L16057;}
 L16285: {
  i_478X = arg1K0;
  m_479X = arg1K1;
  if ((0 == (n_484X & m_479X))) {
    arg1K0 = (1 + i_478X);
    arg1K1 = (((m_479X)<<1));
    goto L16285;}
  else {
    Spending_interruptsS = (n_484X & (~ m_479X));
    *((long *) (RSstackS)) = (RSvalS);
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (StemplateS);
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = (RSenvS);
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = ((((SnargsS))<<2));
    RSstackS = (-4 + (RSstackS));
    *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
    RSstackS = (-4 + (RSstackS));
    tem_481X = Sinterrupt_templateS;
    StemplateS = tem_481X;
    RScode_pointerS = (-3 + (*((long *) (-3 + tem_481X))));
    merged_arg1K0 = (RScode_pointerS);
    merged_arg1K1 = (5 + (SnargsS));
    merged_arg1K2 = key_483X;
    push_continuationB_return_tag = 1;
    goto push_continuationB;
   push_continuationB_return_1:
    *((long *) (RSstackS)) = ((((Senabled_interruptsS))<<2));
    RSstackS = (-4 + (RSstackS));
    SnargsS = 1;
    obj_480X = Sinterrupt_handlersS;
    if ((3 == (3 & obj_480X))) {
      if ((2 == (31 & ((((*((long *) (-7 + obj_480X))))>>2))))) {
        goto L16099;}
      else {
        goto L16175;}}
    else {
      goto L16175;}}}
 L16175: {
  TTerror("interrupt handler is not a vector", 0, 0, 0, 0);
  goto L16099;}
 L16127: {
  TTerror("interrupt handler is not a closure", 1, i_478X, 0, 0);
  goto L16117;}
 L16117: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L16099: {
  RSvalS = (*((long *) ((-3 + (Sinterrupt_handlersS)) + (((i_478X)<<2)))));
  Senabled_interruptsS = 0;
  obj_477X = RSvalS;
  if ((3 == (3 & obj_477X))) {
    if ((3 == (31 & ((((*((long *) (-7 + obj_477X))))>>2))))) {
      goto L16117;}
    else {
      goto L16127;}}
  else {
    goto L16127;}}}

 copy_stack_into_heap: {
  key_647X = merged_arg1K0;{
  arg1K0 = (4 + (RSstackS));
  arg1K1 = 0;
  goto L15017;}
 L15017: {
  p_488X = arg1K0;
  i_489X = arg1K1;
  if ((2 == (3 & (*((long *) p_488X))))) {
    len_491X = ((i_489X)<<2);
    *((long *) (ShpS)) = (10 + (((len_491X)<<8)));
    ShpS = (4 + (ShpS));
    new_490X = 3 + (ShpS);
    ShpS = ((ShpS) + (-4 & (3 + len_491X)));
    if ((0 < len_491X)) {
      *((long *) (-4 + (ShpS))) = 0;
      arg1K0 = new_490X;
      goto L14881;}
    else {
      arg1K0 = new_490X;
      goto L14881;}}
  else {
    arg1K0 = (4 + p_488X);
    arg1K1 = (1 + i_489X);
    goto L15017;}}
 L14881: {
  vec_487X = arg1K0;
  arg1K0 = (-1 + i_489X);
  goto L14883;}
 L14907: {
  i_486X = arg1K0;
  if ((i_486X < i_489X)) {
    *((long *) (RSstackS)) = (*((long *) ((-3 + vec_487X) + (((i_486X)<<2)))));
    RSstackS = (-4 + (RSstackS));
    arg1K0 = (1 + i_486X);
    goto L14907;}
  else {
    goto copy_stack_into_heap_return;}}
 L14883: {
  i_485X = arg1K0;
  if ((-1 < i_485X)) {
    RSstackS = (4 + (RSstackS));
    *((long *) ((-3 + vec_487X) + (((i_485X)<<2)))) = (*((long *) (RSstackS)));
    arg1K0 = (-1 + i_485X);
    goto L14883;}
  else {
    merged_arg1K0 = key_647X;
    merged_arg0K1 = 1;
    preserve_continuation_return_tag = 1;
    goto preserve_continuation;
   preserve_continuation_return_1:
    arg1K0 = 0;
    goto L14907;}}
 copy_stack_into_heap_return:
  switch (copy_stack_into_heap_return_tag) {
  case 0: goto copy_stack_into_heap_return_0;
  case 1: goto copy_stack_into_heap_return_1;
  case 2: goto copy_stack_into_heap_return_2;
  case 3: goto copy_stack_into_heap_return_3;
  }}

 preserve_continuation: {
  key_645X = merged_arg1K0;
  reason_646X = merged_arg0K1;{
  p_492X = RSenvS;
  if ((p_492X < (Sstack_beginS))) {
    goto L14626;}
  else {
    if (((Sstack_endS) < p_492X)) {
      goto L14626;}
    else {
      merged_arg1K0 = (RSenvS);
      merged_arg1K1 = (ScontS);
      merged_arg1K2 = key_645X;
      merged_arg0K3 = reason_646X;
      save_env_in_heap_return_tag = 1;
      goto save_env_in_heap;
     save_env_in_heap_return_1:
      v_493X = save_env_in_heap_return_value;
      RSenvS = v_493X;
      goto L14626;}}}
 L14626: {
  end_501X = *((long *) (-3 + (Sbottom_of_stackS)));
  arg1K0 = (ScontS);
  arg1K1 = (Sbottom_of_stackS);
  goto L14631;}
 L14684: {
  v_500X = arg1K0;
  ScontS = v_500X;
  preserve_continuation_return_value = cont_497X;
  goto preserve_continuation_return;}
 L14653: {
  merged_arg1K0 = cont_494X;
  merged_arg1K1 = key_645X;
  copy_stob_return_tag = 0;
  goto copy_stob;
 copy_stob_return_0:
  new_499X = copy_stob_return_value;
  *((long *) (-3 + previous_495X)) = new_499X;
  arg1K0 = (*((long *) (-3 + new_499X)));
  arg1K1 = new_499X;
  goto L14631;}
 L14631: {
  cont_494X = arg1K0;
  previous_495X = arg1K1;
  if ((cont_494X == (Sbottom_of_stackS))) {
    *((long *) (-3 + previous_495X)) = end_501X;
    cont_497X = *((long *) (-3 + (Sbottom_of_stackS)));
    if ((1 == cont_497X)) {
      arg1K0 = (Sbottom_of_stackS);
      goto L14684;}
    else {
      merged_arg1K0 = cont_497X;
      copy_continuation_from_heap_return_tag = 2;
      goto copy_continuation_from_heap;
     copy_continuation_from_heap_return_2:
      v_496X = copy_continuation_from_heap_return_value;
      arg1K0 = v_496X;
      goto L14684;}}
  else {
    p_498X = *((long *) (9 + cont_494X));
    if ((p_498X < (Sstack_beginS))) {
      goto L14653;}
    else {
      if (((Sstack_endS) < p_498X)) {
        goto L14653;}
      else {
        merged_arg1K0 = (*((long *) (9 + cont_494X)));
        merged_arg1K1 = cont_494X;
        merged_arg1K2 = key_645X;
        merged_arg0K3 = reason_646X;
        save_env_in_heap_return_tag = 2;
        goto save_env_in_heap;
       save_env_in_heap_return_2:
        goto L14653;}}}}
 preserve_continuation_return:
  switch (preserve_continuation_return_tag) {
  case 0: goto preserve_continuation_return_0;
  case 1: goto preserve_continuation_return_1;
  }}

 copy_continuation_from_heap: {
  cont_643X = merged_arg1K0;{
  top_502X = (-11 + (Sbottom_of_stackS)) - (-4 & (3 + ((long)(((unsigned long)(*((long *) (-7 + cont_643X))))>>8))));
  new_cont_503X = 7 + top_502X;
  RSstackS = (-4 + top_502X);
  v_504X = (((3 + ((long)(((unsigned long)(*((long *) (-7 + cont_643X))))>>8))))>>2);
  from_505X = -7 + cont_643X;
  arg1K0 = from_505X;
  arg1K1 = top_502X;
  goto L12873;}
 L12873: {
  from_506X = arg1K0;
  to_507X = arg1K1;
  if ((from_506X < (4 + (from_505X + (((v_504X)<<2)))))) {
    *((long *) to_507X) = (*((long *) from_506X));
    arg1K0 = (4 + from_506X);
    arg1K1 = (4 + to_507X);
    goto L12873;}
  else {
    *((long *) (-3 + (Sbottom_of_stackS))) = (*((long *) (-3 + new_cont_503X)));
    *((long *) (-3 + new_cont_503X)) = (Sbottom_of_stackS);
    copy_continuation_from_heap_return_value = new_cont_503X;
    goto copy_continuation_from_heap_return;}}
 copy_continuation_from_heap_return:
  switch (copy_continuation_from_heap_return_tag) {
  case 0: goto copy_continuation_from_heap_return_0;
  case 1: goto copy_continuation_from_heap_return_1;
  case 2: goto copy_continuation_from_heap_return_2;
  }}

 save_env_in_heap: {
  env_639X = merged_arg1K0;
  cont_640X = merged_arg1K1;
  key_641X = merged_arg1K2;{
  merged_arg1K0 = env_639X;
  merged_arg1K1 = key_641X;
  copy_stob_return_tag = 1;
  goto copy_stob;
 copy_stob_return_1:
  new_508X = copy_stob_return_value;
  *((long *) (-7 + env_639X)) = new_508X;
  arg1K0 = new_508X;
  goto L14155;}
 L14179: {
  arg1K0 = cont_640X;
  goto L14181;}
 L14181: {
  cont_513X = arg1K0;
  env_514X = *((long *) (9 + cont_513X));
  if ((3 == (3 & env_514X))) {
    if ((3 == (3 & (*((long *) (-7 + env_514X)))))) {
      *((long *) (9 + cont_513X)) = (*((long *) (-7 + env_514X)));
      arg1K0 = (*((long *) (-3 + cont_513X)));
      goto L14181;}
    else {
      save_env_in_heap_return_value = new_508X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap_return_value = new_508X;
    goto save_env_in_heap_return;}}
 L14155: {
  env_509X = arg1K0;
  p_510X = *((long *) (-3 + env_509X));
  if ((p_510X < (Sstack_beginS))) {
    goto L14179;}
  else {
    if (((Sstack_endS) < p_510X)) {
      goto L14179;}
    else {
      env_511X = *((long *) (-3 + env_509X));
      merged_arg1K0 = env_511X;
      merged_arg1K1 = key_641X;
      copy_stob_return_tag = 2;
      goto copy_stob;
     copy_stob_return_2:
      new_512X = copy_stob_return_value;
      *((long *) (-7 + env_511X)) = new_512X;
      *((long *) (-3 + env_509X)) = new_512X;
      arg1K0 = new_512X;
      goto L14155;}}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  case 2: goto save_env_in_heap_return_2;
  }}

 copy_stob: {
  old_stob_637X = merged_arg1K0;{
  len_516X = (long)(((unsigned long)(*((long *) (-7 + old_stob_637X))))>>8);
  *((long *) (ShpS)) = (66 + (((len_516X)<<8)));
  ShpS = (4 + (ShpS));
  new_515X = 3 + (ShpS);
  ShpS = ((ShpS) + (-4 & (3 + len_516X)));
  if ((0 < len_516X)) {
    *((long *) (-4 + (ShpS))) = 0;
    arg1K0 = new_515X;
    goto L9117;}
  else {
    arg1K0 = new_515X;
    goto L9117;}}
 L9117: {
  new_519X = arg1K0;
  *((long *) (-7 + new_519X)) = (*((long *) (-7 + old_stob_637X)));
  bytes_520X = (long)(((unsigned long)(*((long *) (-7 + old_stob_637X))))>>8);
  from_521X = -3 + old_stob_637X;
  arg1K0 = from_521X;
  arg1K1 = (-3 + new_519X);
  goto L9142;}
 L9142: {
  from_517X = arg1K0;
  to_518X = arg1K1;
  if ((from_517X < (from_521X + (-4 & (3 + bytes_520X))))) {
    *((long *) to_518X) = (*((long *) from_517X));
    arg1K0 = (4 + from_517X);
    arg1K1 = (4 + to_518X);
    goto L9142;}
  else {
    copy_stob_return_value = new_519X;
    goto copy_stob_return;}}
 copy_stob_return:
  switch (copy_stob_return_tag) {
  case 0: goto copy_stob_return_0;
  case 1: goto copy_stob_return_1;
  case 2: goto copy_stob_return_2;
  }}

 push_exception_continuationB: {
{ if (((Sstack_limitS) < (-296 + (RSstackS)))) {
    arg1K0 = 0;
    goto L14119;}
  else {
    Sstack_limitS = (Sstack_beginS);
    Sexception_space_usedPS = 1;
    if (((Sstack_limitS) < (-296 + (RSstackS)))) {
      arg1K0 = 0;
      goto L14119;}
    else {
      TTerror("insufficient space on stack for exception frame", 0, 0, 0, 0);
      arg1K0 = 0;
      goto L14119;}}}
 L14119: {
  key_524X = arg1K0;
  arg1K0 = (4 + (RSstackS));
  arg1K1 = 0;
  goto L14131;}
 L14131: {
  p_522X = arg1K0;
  i_523X = arg1K1;
  if ((2 == (3 & (*((long *) p_522X))))) {
    merged_arg1K0 = (RScode_pointerS);
    merged_arg1K1 = i_523X;
    merged_arg1K2 = key_524X;
    push_continuationB_return_tag = 2;
    goto push_continuationB;
   push_continuationB_return_2:
    goto push_exception_continuationB_return;}
  else {
    arg1K0 = (4 + p_522X);
    arg1K1 = (1 + i_523X);
    goto L14131;}}
 push_exception_continuationB_return:
  switch (push_exception_continuationB_return_tag) {
  case 0: goto push_exception_continuationB_return_0;
  case 1: goto push_exception_continuationB_return_1;
  case 2: goto push_exception_continuationB_return_2;
  case 3: goto push_exception_continuationB_return_3;
  case 4: goto push_exception_continuationB_return_4;
  }}

 push_continuationB: {
  code_pointer_634X = merged_arg1K0;
  size_635X = merged_arg1K1;{
  pc_525X = 12 + ((((code_pointer_634X - (*((long *) (-3 + (StemplateS))))))<<2));
  template_526X = StemplateS;
  RSstackS = (-20 + (RSstackS));
  *((long *) (4 + (RSstackS))) = (128 | (4130 + (((size_635X)<<10))));
  cont_527X = 11 + (RSstackS);
  *((long *) (1 + cont_527X)) = pc_525X;
  *((long *) (5 + cont_527X)) = template_526X;
  *((long *) (9 + cont_527X)) = (RSenvS);
  *((long *) (-3 + cont_527X)) = (ScontS);
  ScontS = cont_527X;
  goto push_continuationB_return;}
 push_continuationB_return:
  switch (push_continuationB_return_tag) {
  case 0: goto push_continuationB_return_0;
  case 1: goto push_continuationB_return_1;
  case 2: goto push_continuationB_return_2;
  }}

 pop_args_list: {
  start_632X = merged_arg1K0;
  count_633X = merged_arg1K1;{
  space_528X = 3 * (SnargsS);
  if ((((ShpS) + (((space_528X)<<2))) < (SlimitS))) {
    goto L15035;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 18;
    goto collect_saving_temp;
   collect_saving_temp_return_18:
    if ((((ShpS) + (((space_528X)<<2))) < (SlimitS))) {
      goto L15035;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      goto L15035;}}}
 L15035: {
  arg1K0 = start_632X;
  arg1K1 = count_633X;
  goto L15037;}
 L15037: {
  args_529X = arg1K0;
  count_530X = arg1K1;
  if ((0 < count_530X)) {
    RSstackS = (4 + (RSstackS));
    a_531X = *((long *) (RSstackS));
    *((long *) (ShpS)) = 2050;
    ShpS = (4 + (ShpS));
    new_532X = 3 + (ShpS);
    ShpS = (8 + (ShpS));
    *((long *) (-4 + (ShpS))) = 0;
    *((long *) (-3 + new_532X)) = a_531X;
    *((long *) (1 + new_532X)) = args_529X;
    arg1K0 = new_532X;
    arg1K1 = (-1 + count_530X);
    goto L15037;}
  else {
    pop_args_list_return_value = args_529X;
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
  case 8: goto pop_args_list_return_8;
  case 9: goto pop_args_list_return_9;
  }}

 collect_saving_temp: {
  value_630X = merged_arg1K0;{
  b_537X = Snewspace_beginS;
  Snewspace_beginS = (Soldspace_beginS);
  Soldspace_beginS = b_537X;
  e_536X = Snewspace_endS;
  Snewspace_endS = (Soldspace_endS);
  Soldspace_endS = e_536X;
  Ssaved_limitS = (SlimitS);
  Ssaved_hpS = (ShpS);
  SlimitS = (Snewspace_endS);
  ShpS = (Snewspace_beginS);
  Sweak_pointer_hpS = -1;
  pc_535X = 12 + (((((RScode_pointerS) - (*((long *) (-3 + (StemplateS))))))<<2));
  value_534X = StemplateS;
  if ((3 == (3 & value_534X))) {
    a_538X = -3 + value_534X;
    if ((a_538X < (Soldspace_beginS))) {
      arg1K0 = value_534X;
      goto L13896;}
    else {
      if ((a_538X < (Soldspace_endS))) {
        merged_arg1K0 = value_534X;
        copy_object_return_tag = 1;
        goto copy_object;
       copy_object_return_1:
        v_533X = copy_object_return_value;
        arg1K0 = v_533X;
        goto L13896;}
      else {
        arg1K0 = value_534X;
        goto L13896;}}}
  else {
    arg1K0 = value_534X;
    goto L13896;}}
 L13896: {
  tem_585X = arg1K0;
  StemplateS = tem_585X;
  RScode_pointerS = ((-3 + (*((long *) (-3 + tem_585X)))) + (((pc_535X)>>2)));
  value_587X = RSvalS;
  if ((3 == (3 & value_587X))) {
    a_588X = -3 + value_587X;
    if ((a_588X < (Soldspace_beginS))) {
      arg1K0 = value_587X;
      goto L13900;}
    else {
      if ((a_588X < (Soldspace_endS))) {
        merged_arg1K0 = value_587X;
        copy_object_return_tag = 2;
        goto copy_object;
       copy_object_return_2:
        v_586X = copy_object_return_value;
        arg1K0 = v_586X;
        goto L13900;}
      else {
        arg1K0 = value_587X;
        goto L13900;}}}
  else {
    arg1K0 = value_587X;
    goto L13900;}}
 L13900: {
  v_581X = arg1K0;
  RSvalS = v_581X;
  value_583X = Sdynamic_stateS;
  if ((3 == (3 & value_583X))) {
    a_584X = -3 + value_583X;
    if ((a_584X < (Soldspace_beginS))) {
      arg1K0 = value_583X;
      goto L13904;}
    else {
      if ((a_584X < (Soldspace_endS))) {
        merged_arg1K0 = value_583X;
        copy_object_return_tag = 3;
        goto copy_object;
       copy_object_return_3:
        v_582X = copy_object_return_value;
        arg1K0 = v_582X;
        goto L13904;}
      else {
        arg1K0 = value_583X;
        goto L13904;}}}
  else {
    arg1K0 = value_583X;
    goto L13904;}}
 L13904: {
  v_577X = arg1K0;
  Sdynamic_stateS = v_577X;
  value_579X = Sexception_handlerS;
  if ((3 == (3 & value_579X))) {
    a_580X = -3 + value_579X;
    if ((a_580X < (Soldspace_beginS))) {
      arg1K0 = value_579X;
      goto L13908;}
    else {
      if ((a_580X < (Soldspace_endS))) {
        merged_arg1K0 = value_579X;
        copy_object_return_tag = 4;
        goto copy_object;
       copy_object_return_4:
        v_578X = copy_object_return_value;
        arg1K0 = v_578X;
        goto L13908;}
      else {
        arg1K0 = value_579X;
        goto L13908;}}}
  else {
    arg1K0 = value_579X;
    goto L13908;}}
 L13908: {
  v_573X = arg1K0;
  Sexception_handlerS = v_573X;
  value_575X = Sinterrupt_handlersS;
  if ((3 == (3 & value_575X))) {
    a_576X = -3 + value_575X;
    if ((a_576X < (Soldspace_beginS))) {
      arg1K0 = value_575X;
      goto L13912;}
    else {
      if ((a_576X < (Soldspace_endS))) {
        merged_arg1K0 = value_575X;
        copy_object_return_tag = 5;
        goto copy_object;
       copy_object_return_5:
        v_574X = copy_object_return_value;
        arg1K0 = v_574X;
        goto L13912;}
      else {
        arg1K0 = value_575X;
        goto L13912;}}}
  else {
    arg1K0 = value_575X;
    goto L13912;}}
 L13912: {
  v_569X = arg1K0;
  Sinterrupt_handlersS = v_569X;
  value_571X = Sinterrupt_templateS;
  if ((3 == (3 & value_571X))) {
    a_572X = -3 + value_571X;
    if ((a_572X < (Soldspace_beginS))) {
      arg1K0 = value_571X;
      goto L13916;}
    else {
      if ((a_572X < (Soldspace_endS))) {
        merged_arg1K0 = value_571X;
        copy_object_return_tag = 6;
        goto copy_object;
       copy_object_return_6:
        v_570X = copy_object_return_value;
        arg1K0 = v_570X;
        goto L13916;}
      else {
        arg1K0 = value_571X;
        goto L13916;}}}
  else {
    arg1K0 = value_571X;
    goto L13916;}}
 L13916: {
  v_565X = arg1K0;
  Sinterrupt_templateS = v_565X;
  value_567X = RSenvS;
  if ((3 == (3 & value_567X))) {
    a_568X = -3 + value_567X;
    if ((a_568X < (Soldspace_beginS))) {
      arg1K0 = value_567X;
      goto L13922;}
    else {
      if ((a_568X < (Soldspace_endS))) {
        merged_arg1K0 = value_567X;
        copy_object_return_tag = 7;
        goto copy_object;
       copy_object_return_7:
        v_566X = copy_object_return_value;
        arg1K0 = v_566X;
        goto L13922;}
      else {
        arg1K0 = value_567X;
        goto L13922;}}}
  else {
    arg1K0 = value_567X;
    goto L13922;}}
 L13922: {
  env_562X = arg1K0;
  RSenvS = env_562X;
  start_563X = 4 + (RSstackS);
  end_564X = -7 + (ScontS);
  arg1K0 = start_563X;
  goto L13998;}
 L13998: {
  addr_560X = arg1K0;
  if ((addr_560X < end_564X)) {
    merged_arg1K0 = addr_560X;
    copy_next_return_tag = 0;
    goto copy_next;
   copy_next_return_0:
    v_561X = copy_next_return_value;
    arg1K0 = v_561X;
    goto L13998;}
  else {
    arg1K0 = (ScontS);
    arg1K1 = 0;
    goto L13937;}}
 L13984: {
  addr_558X = arg1K0;
  if ((addr_558X < end_550X)) {
    merged_arg1K0 = addr_558X;
    copy_next_return_tag = 1;
    goto copy_next;
   copy_next_return_1:
    v_559X = copy_next_return_value;
    arg1K0 = v_559X;
    goto L13984;}
  else {
    if ((env_549X == last_env_548X)) {
      goto L13942;}
    else {
      arg1K0 = env_549X;
      goto L13952;}}}
 L13970: {
  addr_556X = arg1K0;
  if ((addr_556X < end_555X)) {
    merged_arg1K0 = addr_556X;
    copy_next_return_tag = 2;
    goto copy_next;
   copy_next_return_2:
    v_557X = copy_next_return_value;
    arg1K0 = v_557X;
    goto L13970;}
  else {
    arg1K0 = (*((long *) (-3 + env_554X)));
    goto L13952;}}
 L13952: {
  env_554X = arg1K0;
  if ((env_554X < (Sstack_beginS))) {
    goto L13942;}
  else {
    if (((Sstack_endS) < env_554X)) {
      goto L13942;}
    else {
      end_555X = (-3 + env_554X) + (-4 & (3 + ((long)(((unsigned long)(*((long *) (-7 + env_554X))))>>8))));
      arg1K0 = (-3 + env_554X);
      goto L13970;}}}
 L13942: {
  if ((cont_547X == (Sbottom_of_stackS))) {
    areas_551X = Simpure_areasS;
    sizes_552X = Simpure_sizesS;
    count_553X = Simpure_area_countS;
    arg1K0 = 0;
    goto L14471;}
  else {
    arg1K0 = (*((long *) (-3 + cont_547X)));
    arg1K1 = env_549X;
    goto L13937;}}
 L13937: {
  cont_547X = arg1K0;
  last_env_548X = arg1K1;
  env_549X = *((long *) (9 + cont_547X));
  end_550X = (-3 + cont_547X) + (-4 & (3 + ((long)(((unsigned long)(*((long *) (-7 + cont_547X))))>>8))));
  arg1K0 = (-3 + cont_547X);
  goto L13984;}
 L14481: {
  addr_545X = arg1K0;
  if ((addr_545X < end_544X)) {
    merged_arg1K0 = addr_545X;
    copy_next_return_tag = 3;
    goto copy_next;
   copy_next_return_3:
    v_546X = copy_next_return_value;
    arg1K0 = v_546X;
    goto L14481;}
  else {
    arg1K0 = (1 + i_540X);
    goto L14471;}}
 L14471: {
  i_540X = arg1K0;
  if ((i_540X < ((long)count_553X))) {
    start_543X = *(areas_551X + i_540X);
    end_544X = (*(areas_551X + i_540X)) + (*(sizes_552X + i_540X));
    arg1K0 = start_543X;
    goto L14481;}
  else {
    if ((3 == (3 & value_630X))) {
      a_541X = -3 + value_630X;
      if ((a_541X < (Soldspace_beginS))) {
        arg1K0 = value_630X;
        goto L14397;}
      else {
        if ((a_541X < (Soldspace_endS))) {
          merged_arg1K0 = value_630X;
          copy_object_return_tag = 8;
          goto copy_object;
         copy_object_return_8:
          v_542X = copy_object_return_value;
          arg1K0 = v_542X;
          goto L14397;}
        else {
          arg1K0 = value_630X;
          goto L14397;}}}
    else {
      arg1K0 = value_630X;
      goto L14397;}}}
 L14397: {
  value_539X = arg1K0;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  SenvS = RSenvS;
  do_gc();
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  RSenvS = SenvS;
  Sgc_countS = (1 + (Sgc_countS));
  close_untraced_portsB_return_tag = 1;
  goto close_untraced_portsB;
 close_untraced_portsB_return_1:
  if ((((ShpS) + ((((Sminimum_recovered_spaceS))<<2))) < (SlimitS))) {
    collect_saving_temp_return_value = value_539X;
    goto collect_saving_temp_return;}
  else {
    Spending_interruptsS = (4 | (Spending_interruptsS));
    collect_saving_temp_return_value = value_539X;
    goto collect_saving_temp_return;}}
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
  addr_628X = merged_arg1K0;{
  next_589X = 4 + addr_628X;
  thing_590X = *((long *) addr_628X);
  if ((2 == (3 & thing_590X))) {
    if (((31 & (((thing_590X)>>2))) < 15)) {
      goto L11517;}
    else {
      copy_next_return_value = (next_589X + (-4 & (3 + ((long)(((unsigned long)thing_590X)>>8)))));
      goto copy_next_return;}}
  else {
    goto L11517;}}
 L11517: {
  if ((3 == (3 & thing_590X))) {
    a_592X = -3 + thing_590X;
    if ((a_592X < (Soldspace_beginS))) {
      copy_next_return_value = next_589X;
      goto copy_next_return;}
    else {
      if ((a_592X < (Soldspace_endS))) {
        merged_arg1K0 = thing_590X;
        copy_object_return_tag = 9;
        goto copy_object;
       copy_object_return_9:
        value_591X = copy_object_return_value;
        *((long *) addr_628X) = value_591X;
        copy_next_return_value = next_589X;
        goto copy_next_return;}
      else {
        copy_next_return_value = next_589X;
        goto copy_next_return;}}}
  else {
    copy_next_return_value = next_589X;
    goto copy_next_return;}}
 copy_next_return:
  switch (copy_next_return_tag) {
  case 0: goto copy_next_return_0;
  case 1: goto copy_next_return_1;
  case 2: goto copy_next_return_2;
  case 3: goto copy_next_return_3;
  }}

 copy_object: {
  thing_626X = merged_arg1K0;{
  h_594X = *((long *) (-7 + thing_626X));
  if ((3 == (3 & h_594X))) {
    copy_object_return_value = h_594X;
    goto copy_object_return;}
  else {
    if ((1070 == h_594X)) {
      descriptor_595X = *((long *) (-3 + thing_626X));
      if ((3 == (3 & descriptor_595X))) {
        a_593X = -3 + descriptor_595X;
        if ((a_593X < (Soldspace_beginS))) {
          goto L9567;}
        else {
          if ((a_593X < (Soldspace_endS))) {
            SvalS = RSvalS;
            SstackS = RSstackS;
            Scode_pointerS = RScode_pointerS;
            SenvS = RSenvS;
            v_596X = copy_weak_pointer(thing_626X);
            RSvalS = SvalS;
            RSstackS = SstackS;
            RScode_pointerS = Scode_pointerS;
            RSenvS = SenvS;
            copy_object_return_value = v_596X;
            goto copy_object_return;}
          else {
            goto L9567;}}}
      else {
        goto L9567;}}
    else {
      goto L9567;}}}
 L9567: {
  *((long *) (ShpS)) = h_594X;
  ShpS = (4 + (ShpS));
  new_598X = 3 + (ShpS);
  *((long *) (-7 + thing_626X)) = new_598X;
  x1_599X = ShpS;
  arg1K0 = (-3 + thing_626X);
  goto L9581;}
 L9581: {
  o_597X = arg1K0;
  if (((ShpS) < (x1_599X + (-4 & (3 + ((long)(((unsigned long)h_594X)>>8))))))) {
    *((long *) (ShpS)) = (*((long *) o_597X));
    ShpS = (4 + (ShpS));
    arg1K0 = (4 + o_597X);
    goto L9581;}
  else {
    copy_object_return_value = new_598X;
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

 close_untraced_portsB: {
{ arg1K0 = 2;
  goto L13669;}
 L13715: {
  merged_arg1K0 = port_601X;
  close_port_return_tag = 1;
  goto close_port;
 close_port_return_1:
  fputs("Port closed: ", (Scurrent_output_portS));
  string_605X = *((long *) (9 + port_601X));
  fwrite((void *)(-3 + string_605X), sizeof(char), (-1 + ((long)(((unsigned long)(*((long *) (-7 + string_605X))))>>8))), (Scurrent_output_portS));
  putc(10, (Scurrent_output_portS));
  arg1K0 = 1;
  goto L13695;}
 L13700: {
  arg1K0 = (1 + i_600X);
  goto L13669;}
 L13695: {
  new_604X = arg1K0;
  *((Sopen_vm_portsS) + i_600X) = new_604X;
  goto L13700;}
 L13669: {
  i_600X = arg1K0;
  if ((i_600X == (Snumber_of_portsS))) {
    close_untraced_portsB_return_value = 0;
    goto close_untraced_portsB_return;}
  else {
    port_601X = *((Sopen_vm_portsS) + i_600X);
    if ((1 == port_601X)) {
      goto L13700;}
    else {
      header_602X = *((long *) (-7 + port_601X));
      if ((3 == (3 & header_602X))) {
        arg1K0 = header_602X;
        goto L13695;}
      else {
        header_603X = *((long *) (-7 + (*((long *) (9 + port_601X)))));
        if ((3 == (3 & header_603X))) {
          goto L13715;}
        else {
          goto L13715;}}}}}
 close_untraced_portsB_return:
  switch (close_untraced_portsB_return_tag) {
  case 0: goto close_untraced_portsB_return_0;
  case 1: goto close_untraced_portsB_return_1;
  }}

 close_port: {
  vm_port_623X = merged_arg1K0;{
  if (((*((long *) (1 + vm_port_623X))) < 0)) {
    goto close_port_return;}
  else {
    index_608X = (((*((long *) (1 + vm_port_623X))))>>2);
    if ((0 == index_608X)) {
      goto close_port_return;}
    else {
      if ((1 == index_608X)) {
        goto close_port_return;}
      else {
        mode_607X = (((*((long *) (-3 + vm_port_623X))))>>2);
        port_606X = *((Sopen_portsS) + ((((*((long *) (1 + vm_port_623X))))>>2)));
        if ((1 == mode_607X)) {
          fclose(port_606X);
          goto L11174;}
        else {
          if ((2 == mode_607X)) {
            fclose(port_606X);
            goto L11174;}
          else {
            TTerror("this shouldn't happen when closing a port", 0, 0, 0, 0);
            goto L11174;}}}}}}
 L11174: {
  *((long *) (-3 + vm_port_623X)) = 0;
  *((long *) (1 + vm_port_623X)) = -4;
  *((Sopen_vm_portsS) + index_608X) = 1;
  goto close_port_return;}
 close_port_return:
  switch (close_port_return_tag) {
  case 0: goto close_port_return_0;
  case 1: goto close_port_return_1;
  }}

 write_number: {
  x_621X = merged_arg1K0;
  port_622X = merged_arg2K1;{
  if ((x_621X < 0)) {
    putc(45, port_622X);
    arg1K0 = (0 - x_621X);
    goto L29317;}
  else {
    arg1K0 = x_621X;
    goto L29317;}}
 L29317: {
  v_609X = arg1K0;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  SenvS = RSenvS;
  really_write_number(v_609X, port_622X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  RSenvS = SenvS;
  putc(10, port_622X);
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
  stob1_619X = merged_arg1K0;
  stob2_620X = merged_arg1K1;{
  z2_611X = *((long *) (-7 + stob2_620X));
  z1_610X = *((long *) (-7 + stob1_619X));
  if (((128 | z1_610X) == (128 | z2_611X))) {
    arg1K0 = 0;
    goto L8336;}
  else {
    stob_equalP_return_value = 0;
    goto stob_equalP_return;}}
 L8336: {
  i_612X = arg1K0;
  if ((i_612X < ((((3 + ((long)(((unsigned long)z1_610X)>>8))))>>2)))) {
    if (((*((long *) ((-3 + stob1_619X) + (((i_612X)<<2))))) == (*((long *) ((-3 + stob2_620X) + (((i_612X)<<2))))))) {
      arg1K0 = (1 + i_612X);
      goto L8336;}
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
  string_617X = merged_arg1K0;{
  z_613X = -1 + ((long)(((unsigned long)(*((long *) (-7 + string_617X))))>>8));
  v_614X = (unsigned char *)calloc( 1, 1 + z_613X);
  arg1K0 = 0;
  goto L10854;}
 L10854: {
  i_615X = arg1K0;
  if ((i_615X < z_613X)) {
    *(v_614X + i_615X) = (((long)(*((unsigned char *) ((-3 + string_617X) + i_615X)))));
    arg1K0 = (1 + i_615X);
    goto L10854;}
  else {
    extract_string_return_value = v_614X;
    goto extract_string_return;}}
 extract_string_return:
  switch (extract_string_return_tag) {
  case 0: goto extract_string_return_0;
  case 1: goto extract_string_return_1;
  case 2: goto extract_string_return_2;
  }}
}

long enter_string(unsigned char *string_677X)
{
  extern long ShpS;
  long arg1K0;
  long new_678X;
  long len_679X;
  long z_680X;
  long s_681X;
  long i_682X;

 {z_680X = strlen(string_677X);
  len_679X = 1 + z_680X;
  *((long *) (ShpS)) = (62 + (((len_679X)<<8)));
  ShpS = (4 + (ShpS));
  new_678X = 3 + (ShpS);
  ShpS = ((ShpS) + (-4 & (3 + len_679X)));
  if ((0 < len_679X)) {
    *((long *) (-4 + (ShpS))) = 0;
    arg1K0 = new_678X;
    goto L11926;}
  else {
    arg1K0 = new_678X;
    goto L11926;}}
 L11926: {
  s_681X = arg1K0;
  arg1K0 = 0;
  goto L11928;}
 L11928: {
  i_682X = arg1K0;
  if ((i_682X < z_680X)) {
    *((unsigned char *) ((-3 + s_681X) + i_682X)) = ((unsigned char)((long)((*(string_677X + i_682X)))));
    arg1K0 = (1 + i_682X);
    goto L11928;}
  else {
    return(s_681X);}}}

long copy_weak_pointer(long weak_683X)
{
  extern long ShpS;
  extern long Sweak_pointer_limitS;
  extern long Sweak_pointer_hpS;
  unsigned char tempWW817_684X;
  long old_685X;
  long new_686X;

 {tempWW817_684X = -1 == (Sweak_pointer_hpS);
  if (tempWW817_684X) {
    goto L7986;}
  else {
    if (((Sweak_pointer_hpS) < (Sweak_pointer_limitS))) {
      goto L7991;}
    else {
      goto L7986;}}}
 L7986: {
  old_685X = Sweak_pointer_hpS;
  Sweak_pointer_hpS = (ShpS);
  ShpS = (1024 + (ShpS));
  Sweak_pointer_limitS = (ShpS);
  *((long *) (Sweak_pointer_hpS)) = 261186;
  *((long *) (8 + (Sweak_pointer_hpS))) = old_685X;
  goto L7991;}
 L7991: {
  new_686X = 7 + (Sweak_pointer_hpS);
  *((long *) (4 + (Sweak_pointer_hpS))) = (*((long *) (-3 + weak_683X)));
  Sweak_pointer_hpS = (8 + (Sweak_pointer_hpS));
  *((long *) (-7 + weak_683X)) = new_686X;
  return(new_686X);}}

void really_write_number(long x_687X, FILE * port_688X)
{
  extern void really_write_number(long, FILE *);
  long v_689X;
  long v_690X;

 {if ((x_687X < 10)) {
    goto L29279;}
  else {
    v_689X = x_687X / 10;
    really_write_number(v_689X, port_688X);
    goto L29279;}}
 L29279: {
  v_690X = x_687X % 10;
  putc(((48 + v_690X)), port_688X);
  return;}}

long read_number(FILE * port_691X)
{
  long arg1K0;
  unsigned char arg4K1;
  unsigned char arg4K0;
  unsigned char ch_692X;
  long r_693X;
  unsigned char ch_694X;
  long r_695X;
  unsigned char Kchar_696X;
  unsigned char ch_697X;
  unsigned char Kchar_698X;
  unsigned char ch_699X;

 {goto L4911;}
 L4940: {
  ch_692X = arg4K0;
  arg1K0 = 0;
  arg4K1 = ch_692X;
  goto L4942;}
 L4942: {
  r_693X = arg1K0;
  ch_694X = arg4K1;
  r_695X = (-48 + ((long)(ch_694X))) + (10 * r_693X);
  { int TTchar;
    PS_GETC(port_691X, TTchar);
    if (EOF == TTchar) {
      return(r_695X);}
    else {
      unsigned char Kchar_696X = TTchar;
      ch_697X = Kchar_696X;
      if ((((long)ch_697X) < 48)) {
        return(r_695X);}
      else {
        if ((57 < ((long)ch_697X))) {
          return(r_695X);}
        else {
          arg1K0 = r_695X;
          arg4K1 = Kchar_696X;
          goto L4942;}}}}}
 L4911: {
  { int TTchar;
    PS_GETC(port_691X, TTchar);
    if (EOF == TTchar) {
      arg4K0 = -1;
      goto L4940;}
    else {
      unsigned char Kchar_698X = TTchar;
      ch_699X = Kchar_698X;
      if ((((long)ch_699X) < 48)) {
        goto L4911;}
      else {
        if ((57 < ((long)ch_699X))) {
          goto L4911;}
        else {
          arg4K0 = Kchar_698X;
          goto L4940;}}}}}}

long call_startup_procedure(long startup_proc_700X, unsigned char **startup_vector_701X, long startup_vector_length_702X)
{
  extern long Sbottom_of_stackS;
  extern long SenvS;
  extern long Sinterrupt_templateS;
  extern long StemplateS;
  extern long Sdynamic_stateS;
  extern long Sexception_handlerS;
  extern long Sinterrupt_handlersS;
  extern long Senabled_interruptsS;
  extern long Spending_interruptsS;
  extern long *Sopen_vm_portsS;
  extern long Scode_pointerS;
  extern long Tinterpret();
  extern long SnargsS;
  extern long SvalS;
  extern long SstackS;
  extern long Sstack_beginS;
  extern long Sstack_limitS;
  extern unsigned char Sexception_space_usedPS;
  extern long enter_string(unsigned char*);
  extern long ShpS;
  long arg1K0;
  long new_703X;
  long len_704X;
  long tem_705X;
  long new_706X;
  long v_707X;
  unsigned char option_708X;
  long vector_709X;
  long i_710X;
  unsigned char x_711X;
  long val_712X;

 {len_704X = ((startup_vector_length_702X)<<2);
  *((long *) (ShpS)) = (10 + (((len_704X)<<8)));
  ShpS = (4 + (ShpS));
  new_703X = 3 + (ShpS);
  ShpS = ((ShpS) + (-4 & (3 + len_704X)));
  if ((0 < len_704X)) {
    *((long *) (-4 + (ShpS))) = 0;
    arg1K0 = new_703X;
    goto L15454;}
  else {
    arg1K0 = new_703X;
    goto L15454;}}
 L12633: {
  SenvS = 13;
  tem_705X = Sinterrupt_templateS;
  StemplateS = tem_705X;
  Scode_pointerS = (-3 + (*((long *) (-3 + tem_705X))));
  SnargsS = 0;
  SvalS = 13;
  Sdynamic_stateS = 25;
  Sexception_handlerS = 25;
  Sinterrupt_handlersS = 25;
  Senabled_interruptsS = 0;
  Spending_interruptsS = 0;
  *((long *) (SstackS)) = vector_709X;
  SstackS = (-4 + (SstackS));
  *((long *) (SstackS)) = (*((Sopen_vm_portsS) + 0));
  SstackS = (-4 + (SstackS));
  *((long *) (SstackS)) = (*((Sopen_vm_portsS) + 1));
  SstackS = (-4 + (SstackS));
  *((long *) (ShpS)) = 578;
  ShpS = (4 + (ShpS));
  new_706X = 3 + (ShpS);
  ShpS = (4 + (ShpS));
  *((long *) (-4 + (ShpS))) = 0;
  *((unsigned char *) (-3 + new_706X)) = 23;
  *((unsigned char *) (-2 + new_706X)) = 3;
  Scode_pointerS = (-3 + new_706X);
  SvalS = startup_proc_700X;
  goto L15388;}
 L12678: {
  Sstack_limitS = (296 + (Sstack_beginS));
  Sexception_space_usedPS = 0;
  goto L12633;}
 L15388: {
  option_708X = TTrun_machine((long)Tinterpret);
  if ((0 == ((long)option_708X))) {
    return((SvalS));}
  else {
    if ((1 == ((long)option_708X))) {
      v_707X = ((long(*)())(*((long *) (-3 + (*((long *) (1 + (SvalS))))))))((SnargsS), (4 + (SstackS)));
      SvalS = v_707X;
      SstackS = ((SstackS) - ((((-1 - (SnargsS)))<<2)));
      goto L15388;}
    else {
      TTerror("unkown VM return option", 1, ((long)option_708X), 0, 0);
      return(-1);}}}
 L15454: {
  vector_709X = arg1K0;
  arg1K0 = 0;
  goto L15456;}
 L15456: {
  i_710X = arg1K0;
  if ((i_710X < startup_vector_length_702X)) {
    val_712X = enter_string((*(startup_vector_701X + i_710X)));
    *((long *) ((-3 + vector_709X) + (((i_710X)<<2)))) = val_712X;
    arg1K0 = (1 + i_710X);
    goto L15456;}
  else {
    SstackS = (-11 + (Sbottom_of_stackS));
    *((long *) (-3 + (Sbottom_of_stackS))) = 1;
    if ((Sexception_space_usedPS)) {
      x_711X = (Sstack_limitS) < (-296 + (SstackS));
      if (x_711X) {
        goto L12678;}
      else {
        TTerror("no space on stack to reserve exception space", 0, 0, 0, 0);
        goto L12678;}}
    else {
      goto L12633;}}}}

void initialize_vm(long memory_begin_713X, long memory_size_714X, long stack_begin_715X, long stack_size_716X)
{
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern long Snewspace_beginS;
  extern long Snewspace_endS;
  extern long SlimitS;
  extern long Snumber_of_portsS;
  extern FILE * Scurrent_input_portS;
  extern FILE * Scurrent_output_portS;
  extern FILE * *Sopen_portsS;
  extern long enter_string(unsigned char*);
  extern long ShpS;
  extern long Sstack_beginS;
  extern long Sstack_limitS;
  extern long Sstack_endS;
  extern long SstackS;
  extern long SenvS;
  extern long ScontS;
  extern long Sbottom_of_stackS;
  extern long Sinterrupt_templateS;
  extern long *Sopen_vm_portsS;
  long arg1K0;
  unsigned char merged_arg0K1;
  unsigned char merged_arg0K0;

  int make_template_containing_ops_return_tag;
  long make_template_containing_ops_return_value;
  long semisize_717X;
  unsigned char tempWW825_718X;
  long i_719X;
  long v_720X;
  long cont_721X;
  long template_722X;
  long new_723X;
  long d_724X;
  long new_725X;
  long d_726X;
  long *v_727X;
  long length_728X;
  long new_729X;
  long new_730X;
  long c_731X;
  unsigned char op1_732X;
  unsigned char op2_733X;
 {semisize_717X = (((-2 & memory_size_714X))<<1);
  Snewspace_beginS = memory_begin_713X;
  Snewspace_endS = ((Snewspace_beginS) + semisize_717X);
  Soldspace_beginS = (Snewspace_endS);
  Soldspace_endS = ((Soldspace_beginS) + semisize_717X);
  ShpS = (Snewspace_beginS);
  SlimitS = (Snewspace_endS);
  Sopen_portsS = ((FILE **)malloc(sizeof(FILE *) * (Snumber_of_portsS)));
  Sopen_vm_portsS = ((long*)malloc(sizeof(long) * (Snumber_of_portsS)));
  tempWW825_718X = NULL == (Sopen_portsS);
  if (tempWW825_718X) {
    goto L12933;}
  else {
    if ((NULL == (Sopen_vm_portsS))) {
      goto L12933;}
    else {
      goto L12938;}}}
 L12933: {
  TTerror("out of memory, unable to continue", 0, 0, 0, 0);
  goto L12938;}
 L13045: {
  i_719X = arg1K0;
  if ((i_719X < length_728X)) {
    *(v_727X + i_719X) = 1;
    arg1K0 = (1 + i_719X);
    goto L13045;}
  else {
    *((Sopen_portsS) + 0) = (Scurrent_input_portS);
    *((Sopen_portsS) + 1) = (Scurrent_output_portS);
    d_726X = enter_string("so");
    *((long *) (ShpS)) = 4118;
    ShpS = (4 + (ShpS));
    new_725X = 3 + (ShpS);
    ShpS = (16 + (ShpS));
    *((long *) (-4 + (ShpS))) = 0;
    *((long *) (-3 + new_725X)) = 8;
    *((long *) (1 + new_725X)) = 4;
    *((long *) (5 + new_725X)) = 1;
    *((long *) (9 + new_725X)) = d_726X;
    d_724X = enter_string("si");
    *((long *) (ShpS)) = 4118;
    ShpS = (4 + (ShpS));
    new_723X = 3 + (ShpS);
    ShpS = (16 + (ShpS));
    *((long *) (-4 + (ShpS))) = 0;
    *((long *) (-3 + new_723X)) = 4;
    *((long *) (1 + new_723X)) = 0;
    *((long *) (5 + new_723X)) = 1;
    *((long *) (9 + new_723X)) = d_724X;
    *((Sopen_vm_portsS) + 0) = new_723X;
    *((Sopen_vm_portsS) + 1) = new_725X;
    Sstack_beginS = stack_begin_715X;
    Sstack_endS = (stack_begin_715X + (((stack_size_716X)<<2)));
    Sstack_limitS = (Sstack_beginS);
    SstackS = (-4 + (Sstack_endS));
    ScontS = 1;
    SenvS = 17;
    merged_arg0K0 = 22;
    merged_arg0K1 = 28;
    make_template_containing_ops_return_tag = 0;
    goto make_template_containing_ops;
   make_template_containing_ops_return_0:
    template_722X = make_template_containing_ops_return_value;
    SstackS = (-20 + (SstackS));
    *((long *) (4 + (SstackS))) = 4258;
    cont_721X = 11 + (SstackS);
    *((long *) (1 + cont_721X)) = 0;
    *((long *) (5 + cont_721X)) = template_722X;
    *((long *) (9 + cont_721X)) = (SenvS);
    *((long *) (-3 + cont_721X)) = (ScontS);
    ScontS = cont_721X;
    Sbottom_of_stackS = (ScontS);
    merged_arg0K0 = 31;
    merged_arg0K1 = 127;
    make_template_containing_ops_return_tag = 1;
    goto make_template_containing_ops;
   make_template_containing_ops_return_1:
    v_720X = make_template_containing_ops_return_value;
    Sinterrupt_templateS = v_720X;
    return;}}
 L12938: {
  v_727X = Sopen_vm_portsS;
  length_728X = Snumber_of_portsS;
  arg1K0 = 0;
  goto L13045;}
 make_template_containing_ops: {
  op1_732X = merged_arg0K0;
  op2_733X = merged_arg0K1;{
  *((long *) (ShpS)) = 578;
  ShpS = (4 + (ShpS));
  new_729X = 3 + (ShpS);
  ShpS = (4 + (ShpS));
  *((long *) (-4 + (ShpS))) = 0;
  *((long *) (ShpS)) = 2090;
  ShpS = (4 + (ShpS));
  new_730X = 3 + (ShpS);
  ShpS = (8 + (ShpS));
  *((long *) (-4 + (ShpS))) = 0;
  *((long *) (-3 + new_730X)) = new_729X;
  *((unsigned char *) (-3 + new_729X)) = ((unsigned char)((long)op1_732X));
  *((unsigned char *) (-2 + new_729X)) = ((unsigned char)((long)op2_733X));
  make_template_containing_ops_return_value = new_730X;
  goto make_template_containing_ops_return;}
 make_template_containing_ops_return:
  switch (make_template_containing_ops_return_tag) {
  case 0: goto make_template_containing_ops_return_0;
  case 1: goto make_template_containing_ops_return_1;
  }}
}

long required_init_space(unsigned char **startup_vector_734X, long startup_vector_length_735X)
{
  long arg1K1;
  long arg1K0;
  long i_736X;
  long s_737X;

 {arg1K0 = 0;
  arg1K1 = 0;
  goto L5371;}
 L5371: {
  i_736X = arg1K0;
  s_737X = arg1K1;
  if ((i_736X < startup_vector_length_735X)) {
    arg1K0 = (1 + i_736X);
    arg1K1 = (1 + (s_737X + ((((4 + (strlen((*(startup_vector_734X + i_736X))))))>>2))));
    goto L5371;}
  else {
    return((22 + s_737X));}}}

void note_interruptB(unsigned char interrupt_738X)
{
  extern long Spending_interruptsS;
  long x_739X;

 {PS_SHIFT_LEFT(1, ((long)interrupt_738X), x_739X)
  Spending_interruptsS = ((Spending_interruptsS) | x_739X);
  return;}}

void do_gc()
{
  extern long Sweak_pointer_hpS;
  extern long Sweak_pointer_limitS;
  extern long SlimitS;
  extern long ShpS;
  extern long Soldspace_beginS;
  extern long Soldspace_endS;
  extern long copy_weak_pointer(long);
  extern long Snewspace_beginS;
  long arg1K1;
  long arg1K0;
  long RShpS = ShpS;
  long value_740X;
  long scan_741X;
  long value_742X;
  long a_743X;
  long h_744X;
  long start_745X;
  long end_746X;
  long next_747X;
  long value_748X;
  long o_749X;
  long new_750X;
  long x1_751X;
  long v_752X;
  long descriptor_753X;
  long a_754X;
  long h_755X;
  long a_756X;
  long end_757X;
  long x1_758X;
  long addr_759X;
  long next_760X;
  long thing_761X;
  long start_762X;
  long end_763X;

 {arg1K0 = (Snewspace_beginS);
  goto L13811;}
 L7742: {
  arg1K0 = (8 + scan_741X);
  goto L7697;}
 L7737: {
  value_740X = arg1K0;
  *((long *) (4 + scan_741X)) = value_740X;
  goto L7742;}
 L7697: {
  scan_741X = arg1K0;
  if ((scan_741X < end_746X)) {
    *((long *) scan_741X) = 1070;
    value_742X = *((long *) (4 + scan_741X));
    if ((3 == (3 & value_742X))) {
      a_743X = -3 + value_742X;
      if ((a_743X < (Soldspace_beginS))) {
        goto L7742;}
      else {
        if ((a_743X < (Soldspace_endS))) {
          if ((3 == (3 & value_742X))) {
            h_744X = *((long *) (-7 + value_742X));
            if ((3 == (3 & h_744X))) {
              arg1K0 = h_744X;
              goto L7737;}
            else {
              arg1K0 = 1;
              goto L7737;}}
          else {
            goto L7742;}}
        else {
          goto L7742;}}}
    else {
      goto L7742;}}
  else {
    if ((-1 == next_747X)) {
      if ((end_757X < (Sweak_pointer_limitS))) {
        *((long *) end_757X) = (66 + ((((-4 & (-4 + ((Sweak_pointer_limitS) - end_757X))))<<8)));
        ShpS = RShpS;
        return;}
      else {
        ShpS = RShpS;
        return;}}
    else {
      arg1K0 = (-1024 + next_747X);
      arg1K1 = next_747X;
      goto L11093;}}}
 L11093: {
  start_745X = arg1K0;
  end_746X = arg1K1;
  next_747X = *((long *) (8 + start_745X));
  arg1K0 = start_745X;
  goto L7697;}
 L31621: {
  value_748X = arg1K0;
  *((long *) addr_759X) = value_748X;
  arg1K0 = next_760X;
  goto L13877;}
 L31674: {
  o_749X = arg1K0;
  if (((RShpS) < (x1_751X + (-4 & (3 + ((long)(((unsigned long)h_755X)>>8))))))) {
    *((long *) (RShpS)) = (*((long *) o_749X));
    RShpS = (4 + (RShpS));
    arg1K0 = (4 + o_749X);
    goto L31674;}
  else {
    arg1K0 = new_750X;
    goto L31621;}}
 L31658: {
  *((long *) (RShpS)) = h_755X;
  RShpS = (4 + (RShpS));
  new_750X = 3 + (RShpS);
  *((long *) (-7 + thing_761X)) = new_750X;
  x1_751X = RShpS;
  arg1K0 = (-3 + thing_761X);
  goto L31674;}
 L31613: {
  if ((3 == (3 & thing_761X))) {
    a_754X = -3 + thing_761X;
    if ((a_754X < (Soldspace_beginS))) {
      arg1K0 = next_760X;
      goto L13877;}
    else {
      if ((a_754X < (Soldspace_endS))) {
        h_755X = *((long *) (-7 + thing_761X));
        if ((3 == (3 & h_755X))) {
          arg1K0 = h_755X;
          goto L31621;}
        else {
          if ((1070 == h_755X)) {
            descriptor_753X = *((long *) (-3 + thing_761X));
            if ((3 == (3 & descriptor_753X))) {
              a_756X = -3 + descriptor_753X;
              if ((a_756X < (Soldspace_beginS))) {
                goto L31658;}
              else {
                if ((a_756X < (Soldspace_endS))) {
                  ShpS = RShpS;
                  v_752X = copy_weak_pointer(thing_761X);
                  RShpS = ShpS;
                  arg1K0 = v_752X;
                  goto L31621;}
                else {
                  goto L31658;}}}
            else {
              goto L31658;}}
          else {
            goto L31658;}}}
      else {
        arg1K0 = next_760X;
        goto L13877;}}}
  else {
    arg1K0 = next_760X;
    goto L13877;}}
 L13837: {
  if ((-1 == (Sweak_pointer_hpS))) {
    ShpS = RShpS;
    return;}
  else {
    x1_758X = Sweak_pointer_limitS;
    end_757X = Sweak_pointer_hpS;
    arg1K0 = (-1024 + x1_758X);
    arg1K1 = end_757X;
    goto L11093;}}
 L13877: {
  addr_759X = arg1K0;
  if ((addr_759X < end_763X)) {
    next_760X = 4 + addr_759X;
    thing_761X = *((long *) addr_759X);
    if ((2 == (3 & thing_761X))) {
      if (((31 & (((thing_761X)>>2))) < 15)) {
        goto L31613;}
      else {
        arg1K0 = (next_760X + (-4 & (3 + ((long)(((unsigned long)thing_761X)>>8)))));
        goto L13877;}}
    else {
      goto L31613;}}
  else {
    if (((RShpS) < (SlimitS))) {
      if ((end_763X < (RShpS))) {
        arg1K0 = end_763X;
        goto L13811;}
      else {
        goto L13837;}}
    else {
      TTerror("GC error: ran out of space in new heap", 0, 0, 0, 0);
      goto L13837;}}}
 L13811: {
  start_762X = arg1K0;
  end_763X = RShpS;
  arg1K0 = start_762X;
  goto L13877;}}

long read_image(unsigned char *filename_764X, long startup_space_765X)
{
  extern long read_number(FILE *);
  extern long Snewspace_endS;
  extern FILE * Scurrent_output_portS;
  extern long ShpS;
  long arg1K0;
  unsigned char arg3K0;
  FILE * port_766X;
  unsigned char reverseP_767X;
  long v_768X;
  long ptr_769X;
  long value_770X;
  unsigned char x_771X;
  unsigned char x_772X;
  long d_773X;
  long v_774X;
  long ptr_775X;
  long descriptor_776X;
  long result_777X;
  long old_level_778X;
  long old_bytes_per_cell_779X;
  long cells_780X;
  long old_begin_781X;
  long cells_782X;
  long old_hp_783X;
  long startup_proc_784X;
  unsigned char Kchar_785X;
  unsigned char Kchar_786X;
  long addr_787X;
  unsigned char x_788X;
  unsigned char x_789X;
  long start_790X;
  long new_limit_791X;
  long new_hp_792X;
  long delta_793X;

 {port_766X = ps_open(filename_764X, "r");
  if ((NULL == port_766X)) {
    TTerror("Can't open heap image file", 0, 0, 0, 0);
    goto L10187;}
  else {
    goto L10187;}}
 L10247: {
  reverseP_767X = arg3K0;
  fread((void *)(ShpS), sizeof(char), (old_hp_783X - old_begin_781X), port_766X);
  if (reverseP_767X) {
    fputs("Correcting byte order of resumed image.", (Scurrent_output_portS));
    putc(10, (Scurrent_output_portS));
    arg1K0 = start_790X;
    goto L8592;}
  else {
    goto L10257;}}
 L8618: {
  v_768X = arg1K0;
  arg1K0 = v_768X;
  goto L8592;}
 L8592: {
  ptr_769X = arg1K0;
  x_772X = *((unsigned char *) ptr_769X);
  *((unsigned char *) ptr_769X) = ((unsigned char)((long)(*((unsigned char *) (3 + ptr_769X)))));
  *((unsigned char *) (3 + ptr_769X)) = ((unsigned char)((long)x_772X));
  x_771X = *((unsigned char *) (1 + ptr_769X));
  *((unsigned char *) (1 + ptr_769X)) = ((unsigned char)((long)(*((unsigned char *) (2 + ptr_769X)))));
  *((unsigned char *) (2 + ptr_769X)) = ((unsigned char)((long)x_771X));
  value_770X = *((long *) ptr_769X);
  if ((ptr_769X < new_hp_792X)) {
    if ((2 == (3 & value_770X))) {
      if (((31 & (((value_770X)>>2))) < 15)) {
        arg1K0 = (4 + ptr_769X);
        goto L8618;}
      else {
        arg1K0 = ((4 + ptr_769X) + (-4 & (3 + ((long)(((unsigned long)value_770X)>>8)))));
        goto L8618;}}
    else {
      arg1K0 = (4 + ptr_769X);
      goto L8618;}}
  else {
    goto L10257;}}
 L8717: {
  d_773X = arg1K0;
  *((long *) ptr_775X) = d_773X;
  if ((2 == (3 & d_773X))) {
    if (((31 & (((d_773X)>>2))) < 15)) {
      arg1K0 = (4 + ptr_775X);
      goto L8735;}
    else {
      arg1K0 = ((4 + ptr_775X) + (-4 & (3 + ((long)(((unsigned long)d_773X)>>8)))));
      goto L8735;}}
  else {
    arg1K0 = (4 + ptr_775X);
    goto L8735;}}
 L8735: {
  v_774X = arg1K0;
  arg1K0 = v_774X;
  goto L8707;}
 L8707: {
  ptr_775X = arg1K0;
  if ((ptr_775X < new_hp_792X)) {
    descriptor_776X = *((long *) ptr_775X);
    if ((3 == (3 & descriptor_776X))) {
      arg1K0 = (3 + ((-3 + descriptor_776X) + delta_793X));
      goto L8717;}
    else {
      arg1K0 = descriptor_776X;
      goto L8717;}}
  else {
    goto L10267;}}
 L10437: {
  result_777X = arg1K0;
  fclose(port_766X);
  return(result_777X);}
 L10189: {
  old_level_778X = read_number(port_766X);
  old_bytes_per_cell_779X = read_number(port_766X);
  cells_780X = read_number(port_766X);
  old_begin_781X = ((cells_780X)<<2);
  cells_782X = read_number(port_766X);
  old_hp_783X = ((cells_782X)<<2);
  startup_proc_784X = read_number(port_766X);
  goto L10388;}
 L10413: {
  { int TTchar;
    PS_GETC(port_766X, TTchar);
    if (EOF == TTchar) {
      TTerror("end of file while looking for page break", 0, 0, 0, 0);
      goto L10189;}
    else {
      unsigned char Kchar_785X = TTchar;
      if ((12 == ((long)Kchar_785X))) {
        goto L10189;}
      else {
        goto L10413;}}}}
 L10210: {
  if ((15 == old_level_778X)) {
    goto L10220;}
  else {
    TTerror("format of image is incompatible with this version of system", 2, old_level_778X, 15, 0);
    goto L10220;}}
 L10388: {
  { int TTchar;
    PS_GETC(port_766X, TTchar);
    if (EOF == TTchar) {
      TTerror("end of file while looking for page break", 0, 0, 0, 0);
      goto L10210;}
    else {
      unsigned char Kchar_786X = TTchar;
      if ((12 == ((long)Kchar_786X))) {
        goto L10210;}
      else {
        goto L10388;}}}}
 L10267: {
  ShpS = new_hp_792X;
  if ((3 == (3 & startup_proc_784X))) {
    arg1K0 = (3 + ((-3 + startup_proc_784X) + delta_793X));
    goto L10437;}
  else {
    arg1K0 = startup_proc_784X;
    goto L10437;}}
 L10257: {
  if ((0 == delta_793X)) {
    goto L10267;}
  else {
    arg1K0 = start_790X;
    goto L8707;}}
 L10244: {
  fread((void *)(ShpS), sizeof(char), 4, port_766X);
  if ((1 == (*((long *) (ShpS))))) {
    arg3K0 = 0;
    goto L10247;}
  else {
    addr_787X = ShpS;
    x_788X = *((unsigned char *) addr_787X);
    *((unsigned char *) addr_787X) = ((unsigned char)((long)(*((unsigned char *) (3 + addr_787X)))));
    *((unsigned char *) (3 + addr_787X)) = ((unsigned char)((long)x_788X));
    x_789X = *((unsigned char *) (1 + addr_787X));
    *((unsigned char *) (1 + addr_787X)) = ((unsigned char)((long)(*((unsigned char *) (2 + addr_787X)))));
    *((unsigned char *) (2 + addr_787X)) = ((unsigned char)((long)x_789X));
    if ((1 == (*((long *) (ShpS))))) {
      arg3K0 = 1;
      goto L10247;}
    else {
      TTerror("unable to correct byte order", 1, (*((long *) (ShpS))), 0, 0);
      arg3K0 = 0;
      goto L10247;}}}
 L10230: {
  delta_793X = (ShpS) - old_begin_781X;
  new_hp_792X = old_hp_783X + delta_793X;
  new_limit_791X = Snewspace_endS;
  start_790X = ShpS;
  if (((startup_space_765X + new_hp_792X) < new_limit_791X)) {
    goto L10244;}
  else {
    TTerror("heap not big enough to restore this image", 2, new_hp_792X, new_limit_791X, 0);
    goto L10244;}}
 L10220: {
  if ((4 == old_bytes_per_cell_779X)) {
    goto L10230;}
  else {
    TTerror("incompatible bytes-per-cell", 2, old_bytes_per_cell_779X, 4, 0);
    goto L10230;}}
 L10187: {
  goto L10413;}}

long check_image_header(unsigned char *filename_794X)
{
  extern long read_number(FILE *);
  extern FILE * Scurrent_output_portS;
  long arg1K0;
  FILE * port_795X;
  FILE * out_796X;
  long result_797X;
  FILE * out_798X;
  long old_level_799X;
  long old_bytes_per_cell_800X;
  long cells_801X;
  long cells_802X;
  FILE * out_803X;
  unsigned char Kchar_804X;

 {port_795X = ps_open(filename_794X, "r");
  if ((NULL == port_795X)) {
    out_796X = Scurrent_output_portS;
    fputs("Can't open heap image file", out_796X);
    putc(10, out_796X);
    arg1K0 = -1;
    goto L6545;}
  else {
    goto L6501;}}
 L6545: {
  result_797X = arg1K0;
  fclose(port_795X);
  return(result_797X);}
 L6407: {
  old_level_799X = read_number(port_795X);
  old_bytes_per_cell_800X = read_number(port_795X);
  cells_801X = read_number(port_795X);
  cells_802X = read_number(port_795X);
  if ((15 == old_level_799X)) {
    if ((4 == old_bytes_per_cell_800X)) {
      arg1K0 = ((((cells_802X)<<2)) - (((cells_801X)<<2)));
      goto L6545;}
    else {
      out_798X = Scurrent_output_portS;
      fputs("incompatible bytes-per-cell in image", out_798X);
      putc(10, out_798X);
      arg1K0 = -1;
      goto L6545;}}
  else {
    out_803X = Scurrent_output_portS;
    fputs("format of image is incompatible with this version of system", out_803X);
    putc(10, out_803X);
    arg1K0 = -1;
    goto L6545;}}
 L6501: {
  { int TTchar;
    PS_GETC(port_795X, TTchar);
    if (EOF == TTchar) {
      TTerror("end of file while looking for page break", 0, 0, 0, 0);
      goto L6407;}
    else {
      unsigned char Kchar_804X = TTchar;
      if ((12 == ((long)Kchar_804X))) {
        goto L6407;}
      else {
        goto L6501;}}}}}

void register_static_areas(unsigned char pure_count_805X, long *pure_areas_806X, long *pure_sizes_807X, unsigned char impure_count_808X, long *impure_areas_809X, long *impure_sizes_810X)
{
  extern unsigned char Spure_area_countS;
  extern long *Spure_areasS;
  extern long *Spure_sizesS;
  extern unsigned char Simpure_area_countS;
  extern long *Simpure_areasS;
  extern long *Simpure_sizesS;

 {Spure_area_countS = pure_count_805X;
  Spure_areasS = pure_areas_806X;
  Spure_sizesS = pure_sizes_807X;
  Simpure_area_countS = impure_count_808X;
  Simpure_areasS = impure_areas_809X;
  Simpure_sizesS = impure_sizes_810X;
  return;}}
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
Simpure_area_countS = 0;
Spure_area_countS = 0;
SlimitS = 0;
ShpS = 0;
Scurrent_output_portS = stdout;
Scurrent_input_portS = stdin;
}
