/* PS_SMALL_MULTIPLY
   immutable closures
   immutable continuations */

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
char Simpure_area_countS;
char Spure_area_countS;
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

unsigned char hoistD0(long start_0X, long end_1X, long x_2X)
{
  extern FILE * Scurrent_output_portS;
  extern long SlimitS;
  extern long ShpS;
  long arg1K1;
  long arg1K0;
  long i_3X;
  long h_4X;
  long bucket_5X;
  long index_6X;
  unsigned char v_7X;
  long b_8X;
  long new_9X;
  long addr_10X;
  unsigned char v_11X;
  long d_12X;
  long obj_13X;
  long v_14X;
  long s_15X;
  long v_16X;

 {arg1K0 = start_0X;
  goto L17452;}
 L13710: {
  i_3X = arg1K0;
  h_4X = arg1K1;
  if ((i_3X < (-1 + v_16X))) {
    v_7X = ((long)(*((unsigned char*)((-3 + s_15X) + i_3X))));
    arg1K0 = (1 + i_3X);
    arg1K1 = (h_4X + ((long)(v_7X)));
    goto L13710;}
  else {
    index_6X = h_4X & (-1 + v_14X);
    bucket_5X = *((long*)((-3 + x_2X) + (((index_6X)<<2))));
    arg1K0 = bucket_5X;
    goto L13608;}}
 L13608: {
  b_8X = arg1K0;
  if ((25 == b_8X)) {
    *((long*)ShpS) = 2050;
    ShpS = (4 + ShpS);
    new_9X = 3 + ShpS;
    ShpS = (8 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_9X)) = obj_13X;
    *((long*)(1 + new_9X)) = bucket_5X;
    *((long*)((-3 + x_2X) + (((index_6X)<<2)))) = new_9X;
    arg1K0 = (4 + (addr_10X + (-4 & (3 + ((long)(((unsigned long)d_12X)>>8))))));
    goto L17452;}
  else {
    if ((obj_13X == (*((long*)(-3 + b_8X))))) {
      arg1K0 = (4 + (addr_10X + (-4 & (3 + ((long)(((unsigned long)d_12X)>>8))))));
      goto L17452;}
    else {
      arg1K0 = (*((long*)(1 + b_8X)));
      goto L13608;}}}
 L17452: {
  addr_10X = arg1K0;
  if ((addr_10X < end_1X)) {
    d_12X = *((long*)addr_10X);
    if ((2 == (3 & d_12X))) {
      if ((1 == (31 & (((d_12X)>>2))))) {
        v_11X = (12 + ShpS) < SlimitS;
        if (v_11X) {
          obj_13X = 7 + addr_10X;
          v_14X = (((3 + ((long)(((unsigned long)(*((long*)(-7 + x_2X))))>>8))))>>2);
          s_15X = *((long*)(-3 + obj_13X));
          v_16X = (long)(((unsigned long)(*((long*)(-7 + s_15X))))>>8);
          arg1K0 = 0;
          arg1K1 = 0;
          goto L13710;}
        else {
          return(0);}}
      else {
        arg1K0 = (4 + (addr_10X + (-4 & (3 + ((long)(((unsigned long)d_12X)>>8))))));
        goto L17452;}}
    else {
      fputs("heap is in an inconsistent state.", Scurrent_output_portS);
      return(0);}}
  else {
    return(1);}}}

unsigned char hoistD1(long start_17X, long end_18X, long type_19X)
{
  extern FILE * Scurrent_output_portS;
  extern long SlimitS;
  extern long ShpS;
  long arg1K0;
  long addr_20X;
  unsigned char v_21X;
  long d_22X;

 {arg1K0 = start_17X;
  goto L10236;}
 L10236: {
  addr_20X = arg1K0;
  if ((addr_20X < end_18X)) {
    d_22X = *((long*)addr_20X);
    if ((2 == (3 & d_22X))) {
      if ((type_19X == (31 & (((d_22X)>>2))))) {
        v_21X = (16 + ShpS) < SlimitS;
        if (v_21X) {
          *((long*)ShpS) = (7 + addr_20X);
          ShpS = (4 + ShpS);
          arg1K0 = (4 + (addr_20X + (-4 & (3 + ((long)(((unsigned long)d_22X)>>8))))));
          goto L10236;}
        else {
          return(0);}}
      else {
        arg1K0 = (4 + (addr_20X + (-4 & (3 + ((long)(((unsigned long)d_22X)>>8))))));
        goto L10236;}}
    else {
      fputs("heap is in an inconsistent state.", Scurrent_output_portS);
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
  extern unsigned char hoistD1(long, long, long);
  extern long *Spure_sizesS;
  extern long *Spure_areasS;
  extern unsigned char hoistD0(long, long, long);
  extern long Sgc_countS;
  extern long Sminimum_recovered_spaceS;
  extern char Spure_area_countS;
  extern char Simpure_area_countS;
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
  unsigned char b_23X;
  unsigned char b_24X;
  unsigned char okayP_25X;
  unsigned char v_26X;
  long space_27X;
  long v_28X;
  unsigned char b_29X;
  long env_30X;
  unsigned char b_31X;
  long location_32X;
  unsigned char b_33X;
  long location_34X;
  unsigned char b_35X;
  unsigned char b_36X;
  unsigned char okayP_37X;
  unsigned char v_38X;
  long space_39X;
  long v_40X;
  long cont_41X;
  long list_42X;
  long proc_43X;
  long v_44X;
  long cont_45X;
  long pc_46X;
  long tem_47X;
  long cont_48X;
  long tem_49X;
  unsigned char b_50X;
  long tem_51X;
  unsigned char b_52X;
  unsigned char b_53X;
  unsigned char b_54X;
  unsigned char b_55X;
  unsigned char b_56X;
  long val_57X;
  long v_58X;
  long v_59X;
  long v_60X;
  long v_61X;
  long v_62X;
  long z_63X;
  long x_64X;
  long arg2_65X;
  long b_66X;
  long a_67X;
  long x_68X;
  long arg2_69X;
  unsigned char v_70X;
  long b_71X;
  long a_72X;
  long x_73X;
  long arg2_74X;
  long x_75X;
  long arg2_76X;
  long x_77X;
  long arg2_78X;
  unsigned char v_79X;
  long b_80X;
  long a_81X;
  long x_82X;
  long arg2_83X;
  long x_84X;
  unsigned char v_85X;
  long arg2_86X;
  unsigned char v_87X;
  long arg2_88X;
  unsigned char v_89X;
  long arg2_90X;
  long x_91X;
  long result_92X;
  long x_93X;
  long value_94X;
  long count_95X;
  long x_96X;
  long arg2_97X;
  long x_98X;
  long x_99X;
  long arg2_100X;
  long x_101X;
  long arg2_102X;
  long x_103X;
  long v_104X;
  long x_105X;
  unsigned char b_106X;
  long x_107X;
  unsigned char b_108X;
  unsigned char b_109X;
  long x_110X;
  long v_111X;
  long arg2_112X;
  long v_113X;
  unsigned char v_114X;
  long arg2_115X;
  long v_116X;
  long arg3_117X;
  long arg2_118X;
  unsigned char v_119X;
  long x_120X;
  long obj_121X;
  unsigned char v_122X;
  long arg2_123X;
  unsigned char v_124X;
  long v_125X;
  unsigned char okayP_126X;
  unsigned char v_127X;
  long size_128X;
  long len_129X;
  unsigned char init_130X;
  long arg2_131X;
  long v_132X;
  long v_133X;
  long index_134X;
  long v_135X;
  long v_136X;
  long index_137X;
  unsigned char Kchar_138X;
  long arg3_139X;
  long arg2_140X;
  unsigned char x_141X;
  long v_142X;
  long x_143X;
  long arg2_144X;
  long x_145X;
  long obj_146X;
  unsigned char x_147X;
  long x_148X;
  unsigned char x_149X;
  long x_150X;
  unsigned char Kchar_151X;
  long c_152X;
  long x_153X;
  long v_154X;
  FILE * v_155X;
  unsigned char v_156X;
  long p_157X;
  long x_158X;
  long x_159X;
  long v_160X;
  unsigned char v_161X;
  long p_162X;
  unsigned char v_163X;
  long p_164X;
  long arg2_165X;
  FILE * port_166X;
  long v_167X;
  long v_168X;
  long e_169X;
  long b_170X;
  long v_171X;
  FILE * port_172X;
  unsigned char *spec_173X;
  long x_174X;
  long obj_175X;
  long arg3_176X;
  long arg2_177X;
  long n_178X;
  long n_179X;
  long key_180X;
  long x_181X;
  char count_182X;
  long *sizes_183X;
  long *areas_184X;
  long v_185X;
  char count_186X;
  long *sizes_187X;
  long *areas_188X;
  long v_189X;
  long v_190X;
  long temp_191X;
  long n_192X;
  long p_193X;
  long v_194X;
  long v_195X;
  long v_196X;
  long proc_197X;
  long index_198X;
  long n_199X;
  long key_200X;
  long x_201X;
  long arg2_202X;
  long arg2_203X;
  unsigned char x_204X;
  long obj_205X;
  long arg2_206X;
  long v_207X;
  unsigned char okayP_208X;
  unsigned char v_209X;
  long space_210X;
  unsigned char b_211X;
  unsigned char v_212X;
  unsigned char okayP_213X;
  long obj_214X;
  long x_215X;
  long v_216X;
  long v_217X;
  long v_218X;
  long x_219X;
  long key_220X;
  long return_value_221X;
  long arg2_222X;
  long n_223X;
  long n_224X;
  long v_225X;
  long obj_226X;
  long x_227X;
  long value_228X;
  long name_229X;
  unsigned char v_230X;
  long tem_231X;
  long obj_232X;
  long v_233X;
  long obj_234X;
  long type_235X;
  long start_hp_236X;
  unsigned char v_237X;
  long obj_238X;
  long x_239X;
  long start_hp_240X;
  unsigned char v_241X;
  long arg2_242X;
  long old_243X;
  long bytes_244X;
  long v_245X;
  long v_246X;
  unsigned char x_247X;
  long a_248X;
  unsigned char v_249X;
  long p_250X;
  unsigned char v_251X;
  long v_252X;
  long v_253X;
  FILE * port_254X;
  long v_255X;
  long arg2_256X;
  long v_257X;
  FILE * port_258X;
  long p_259X;
  unsigned char v_260X;
  FILE * v_261X;
  unsigned char b_262X;
  long v_263X;
  long x_264X;
  long c_265X;
  unsigned char Kchar_266X;
  long p_267X;
  unsigned char v_268X;
  FILE * v_269X;
  long v_270X;
  long v_271X;
  long v_272X;
  unsigned char v_273X;
  unsigned char okayP_274X;
  long x_275X;
  long v_276X;
  long obj_277X;
  long x_278X;
  long descriptor_279X;
  long v_280X;
  unsigned char v_281X;
  long arg2_282X;
  unsigned char v_283X;
  unsigned char x_284X;
  long obj_285X;
  long v_286X;
  unsigned char v_287X;
  long arg2_288X;
  long arg3_289X;
  long value_290X;
  long index_291X;
  long len_292X;
  long v_293X;
  long index_294X;
  long len_295X;
  long v_296X;
  long arg2_297X;
  long init_298X;
  long len_299X;
  long size_300X;
  unsigned char v_301X;
  unsigned char okayP_302X;
  long x_303X;
  long index_304X;
  unsigned char b_305X;
  long len_306X;
  long index_307X;
  unsigned char b_308X;
  long len_309X;
  long x_310X;
  long len_311X;
  unsigned char b_312X;
  long size_313X;
  unsigned char v_314X;
  long temp_315X;
  unsigned char v_316X;
  long arg2_317X;
  long x_318X;
  unsigned char b_319X;
  unsigned char b_320X;
  long v_321X;
  long space_322X;
  unsigned char v_323X;
  unsigned char okayP_324X;
  long v_325X;
  long x_326X;
  unsigned char b_327X;
  long x_328X;
  unsigned char v_329X;
  unsigned char v_330X;
  unsigned char v_331X;
  long x_332X;
  unsigned char v_333X;
  long arg2_334X;
  unsigned char v_335X;
  long arg2_336X;
  unsigned char v_337X;
  long arg2_338X;
  long x_339X;
  unsigned char v_340X;
  long arg2_341X;
  long x_342X;
  long a_343X;
  long b_344X;
  unsigned char v_345X;
  unsigned char v_346X;
  unsigned char v_347X;
  long arg2_348X;
  long x_349X;
  long z_350X;
  unsigned char v_351X;
  unsigned char v_352X;
  long x_353X;
  long x_354X;
  long x_355X;
  long x_356X;
  long x_357X;
  long arg2_358X;
  long x_359X;
  unsigned char b_360X;
  unsigned char b_361X;
  unsigned char b_362X;
  unsigned char b_363X;
  long consumer_364X;
  long start_arg_365X;
  long start_loc_366X;
  unsigned char b_367X;
  unsigned char b_368X;
  unsigned char b_369X;
  unsigned char x_370X;
  long space_371X;
  unsigned char v_372X;
  unsigned char okayP_373X;
  unsigned char b_374X;
  long p_375X;
  long v_376X;
  long env_377X;
  unsigned char b_378X;
  long env_379X;
  unsigned char b_380X;
  long env_381X;
  unsigned char b_382X;
  unsigned char b_383X;
  long env_384X;
  unsigned char b_385X;
  long v_386X;
  unsigned char count_387X;
  unsigned char x_388X;
  long space_389X;
  unsigned char v_390X;
  unsigned char okayP_391X;
  long vector_392X;
  long i_393X;
  unsigned char v_394X;
  long *areas_395X;
  long *sizes_396X;
  char count_397X;
  long i_398X;
  unsigned char v_399X;
  long h_400X;
  long addr_401X;
  long h_402X;
  long d_403X;
  long new_404X;
  long new_405X;
  long val_406X;
  long i_407X;
  long h_408X;
  long bucket_409X;
  long index_410X;
  unsigned char v_411X;
  long b_412X;
  unsigned char v_413X;
  long new_414X;
  long stob_415X;
  long from_416X;
  long from_417X;
  long to_418X;
  long v_419X;
  long v_420X;
  long arg2_421X;
  long obj_422X;
  long table_423X;
  long v_424X;
  long v_425X;
  long v_426X;
  unsigned char v_427X;
  long arg2_428X;
  long n_429X;
  long new_430X;
  long len_431X;
  long obj_432X;
  long l_433X;
  long i_434X;
  long i_435X;
  long h_436X;
  unsigned char v_437X;
  char v_438X;
  long i_439X;
  unsigned char v_440X;
  long resume_proc_441X;
  long new_442X;
  long i_443X;
  long header_444X;
  long port_445X;
  long count_446X;
  long count_447X;
  long size_448X;
  long b_449X;
  long e_450X;
  char v_451X;
  long val_452X;
  long c_453X;
  long val_454X;
  long key_455X;
  unsigned char v_456X;
  long arg2_457X;
  long v_458X;
  long mode_459X;
  long i_460X;
  long x_461X;
  long i_462X;
  long x_463X;
  long i_464X;
  long i_465X;
  long index_466X;
  long new_count_467X;
  long old_count_468X;
  FILE * *new_ports_469X;
  long *new_vm_ports_470X;
  FILE * port_471X;
  long new_472X;
  long index_473X;
  long filename_474X;
  unsigned char *spec_475X;
  unsigned char *spec_476X;
  long filename_477X;
  char v_478X;
  long new_479X;
  long len_480X;
  long string_481X;
  long i_482X;
  long new_483X;
  long code_vector_484X;
  long i_485X;
  long val_486X;
  long i_487X;
  unsigned char okayP_488X;
  long init_489X;
  long new_490X;
  long len_491X;
  long new_492X;
  long len_493X;
  unsigned char b_494X;
  unsigned char b_495X;
  long new_496X;
  long i_497X;
  char v_498X;
  char v_499X;
  long n_500X;
  long b_501X;
  long c_502X;
  long a_503X;
  long b_504X;
  long c_505X;
  long a_506X;
  char v_507X;
  char v_508X;
  long n_509X;
  long b_510X;
  long c_511X;
  long a_512X;
  long q_513X;
  long a_514X;
  long b_515X;
  long b_516X;
  long c_517X;
  long a_518X;
  long b_519X;
  long lo_a_520X;
  long lo_b_521X;
  long hi_a_522X;
  long hi_b_523X;
  long lo_c_524X;
  long mid_c_525X;
  long c_526X;
  long a_527X;
  long offset_528X;
  long list_529X;
  long a_530X;
  long new_531X;
  long l_532X;
  long last_533X;
  long new_534X;
  long a_535X;
  long i_536X;
  long l_537X;
  long space_538X;
  unsigned char v_539X;
  unsigned char okayP_540X;
  long overflow_541X;
  long args_542X;
  long fast_543X;
  long len_544X;
  long slow_545X;
  unsigned char move_slowP_546X;
  long nargs_547X;
  long args_548X;
  long v_549X;
  long args_550X;
  long loc_551X;
  long arg_552X;
  long v_553X;
  long key_554X;
  long v_555X;
  long space_556X;
  unsigned char v_557X;
  unsigned char okayP_558X;
  long key_559X;
  long p_560X;
  long v_561X;
  long env_562X;
  unsigned char v_563X;
  unsigned char okayP_564X;
  unsigned char b_565X;
  long a_566X;
  long new_567X;
  long env_568X;
  long i_569X;
  unsigned char b_570X;
  long env_571X;
  long i_572X;
  unsigned char b_573X;
  long args_574X;
  long new_575X;
  long len_576X;
  unsigned char b_577X;
  unsigned char b_578X;
  long v_579X;
  char v_580X;
  char v_581X;
  char v_582X;
  char v_583X;
  char v_584X;
  char v_585X;
  long r_586X;
  char v_587X;
  char v_588X;
  long v_589X;
  long key_590X;
  long v_591X;
  long cont_592X;
  long tem_593X;
  long pc_594X;
  long args_595X;
  long v_596X;
  long v_597X;
  long cont_598X;
  long v_599X;
  long v_600X;
  unsigned char next_op_601X;
  long v_602X;
  long next_603X;
  long cont_604X;
  long tem_605X;
  long pc_606X;
  long args_607X;
  long v_608X;
  long v_609X;
  long args_610X;
  long a1_611X;
  long v_612X;
  long obj_613X;
  unsigned char okayP_614X;
  unsigned char v_615X;
  long space_616X;
  unsigned char x_617X;
  long v_618X;
  unsigned char x_619X;
  long tem_620X;
  long obj_621X;
  long v_622X;
  unsigned char okayP_623X;
  unsigned char v_624X;
  long space_625X;
  unsigned char x_626X;
  long v_627X;
  long obj_628X;
  long i_629X;
  long m_630X;
  long obj_631X;
  long tem_632X;
  long v_633X;
  long v_634X;
  long key_635X;
  long n_636X;
  long i_637X;
  long i_638X;
  long vec_639X;
  long p_640X;
  long i_641X;
  long new_642X;
  long len_643X;
  long p_644X;
  long v_645X;
  long cont_646X;
  long previous_647X;
  long v_648X;
  long cont_649X;
  long p_650X;
  long new_651X;
  long v_652X;
  long end_653X;
  long cells_654X;
  long top_655X;
  long new_cont_656X;
  long v_657X;
  long from_658X;
  long from_659X;
  long to_660X;
  long new_661X;
  long env_662X;
  long p_663X;
  long env_664X;
  long new_665X;
  long cont_666X;
  unsigned char v_667X;
  long env_668X;
  long new_669X;
  long h_670X;
  long len_671X;
  long from_672X;
  long to_673X;
  long new_674X;
  long bytes_675X;
  long from_676X;
  unsigned char x_677X;
  unsigned char x_678X;
  long p_679X;
  long i_680X;
  long key_681X;
  long pc_682X;
  long template_683X;
  long cont_684X;
  long space_685X;
  unsigned char v_686X;
  unsigned char okayP_687X;
  long args_688X;
  long count_689X;
  long a_690X;
  long new_691X;
  long v_692X;
  long value_693X;
  long pc_694X;
  long e_695X;
  long b_696X;
  long a_697X;
  unsigned char v_698X;
  long i_699X;
  unsigned char x_700X;
  long port_701X;
  long header_702X;
  long new_703X;
  long value_704X;
  long i_705X;
  unsigned char v_706X;
  long a_707X;
  long v_708X;
  long start_709X;
  long end_710X;
  long addr_711X;
  long v_712X;
  long cont_713X;
  long last_env_714X;
  long env_715X;
  long byte_count_716X;
  long *areas_717X;
  long *sizes_718X;
  char count_719X;
  long env_720X;
  long byte_count_721X;
  long addr_722X;
  long v_723X;
  long addr_724X;
  long v_725X;
  long addr_726X;
  long v_727X;
  long env_728X;
  long start_729X;
  long end_730X;
  long v_731X;
  long v_732X;
  long value_733X;
  long a_734X;
  unsigned char v_735X;
  long v_736X;
  long v_737X;
  long value_738X;
  long a_739X;
  unsigned char v_740X;
  long v_741X;
  long v_742X;
  long value_743X;
  long a_744X;
  unsigned char v_745X;
  long v_746X;
  long v_747X;
  long value_748X;
  long a_749X;
  unsigned char v_750X;
  long v_751X;
  long v_752X;
  long value_753X;
  long a_754X;
  unsigned char v_755X;
  long tem_756X;
  long v_757X;
  long value_758X;
  long a_759X;
  unsigned char v_760X;
  long next_761X;
  long thing_762X;
  long value_763X;
  long a_764X;
  unsigned char v_765X;
  unsigned char v_766X;
  long a_767X;
  long h_768X;
  long descriptor_769X;
  long v_770X;
  long o_771X;
  long new_772X;
  long x1_773X;
  long header_774X;
  long string_775X;
  FILE * port_776X;
  long v_777X;
  FILE * port_778X;
  long mode_779X;
  long index_780X;

  long v_781X;
  long z1_782X;
  long z2_783X;
  long i_784X;
  long v_785X;
  long z_786X;
  unsigned char *v_787X;
  long i_788X;
  unsigned char v_789X;
  unsigned char *c_790X;
  long string_791X;
  unsigned char c_792X;
  long stob1_793X;
  long stob2_794X;
  long x_795X;
  FILE * port_796X;
  long vm_port_797X;
  long port_798X;
  long c_799X;
  long thing_800X;
  long c_801X;
  long addr_802X;
  long c_803X;
  long value_804X;
  long c_805X;
  long start_806X;
  long count_807X;
  long code_pointer_808X;
  long size_809X;
  long c_810X;
  long stob_811X;
  long c_812X;
  long env_813X;
  long cont_814X;
  long key_815X;
  long c_816X;
  long cont_817X;
  long c_818X;
  long key_819X;
  char reason_820X;
  long key_821X;
  char c_822X;
  char c_823X;
  char bytes_consumed_824X;
  char c_825X;
  char nargs_826X;
  char c_827X;
  char byte_args_828X;
  char c_829X;
  char args_830X;
  long a1_831X;
  long a2_832X;
  long a3_833X;
  long a4_834X;
  char c_835X;
  char args_836X;
  long a1_837X;
  long a2_838X;
  long a3_839X;
  char c_840X;
  char args_841X;
  long a1_842X;
  long a2_843X;
  char c_844X;
  char args_845X;
  long a1_846X;
  char c_847X;
  long nargs_848X;
  char c_849X;
  char args_850X;
 START: {
  b_211X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  switch (b_211X) {
    case 0 : {
      b_23X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((SnargsS == ((long)b_23X))) {
        goto START;}
      else {
        merged_arg0K0 = 1;
        goto application_exception;}}
      break;
    case 1 : {
      b_24X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((SnargsS < ((long)b_24X))) {
        merged_arg0K0 = 1;
        goto application_exception;}
      else {
        goto START;}}
      break;
    case 2 : {
      RSvalS = (((SnargsS)<<2));
      goto START;}
      break;
    case 3 : {
      count_387X = *((unsigned char*)RScode_pointerS);
      x_388X = Sstack_limitS < (-8 + (RSstackS - (((((long)count_387X))<<2))));
      if (x_388X) {
        goto L28086;}
      else {
        space_389X = 1 + ((((Sstack_endS - RSstackS))>>2));
        v_390X = (ShpS + (((space_389X)<<2))) < SlimitS;
        if (v_390X) {
          arg1K0 = 0;
          goto L28123;}
        else {
          merged_arg1K0 = 0;
          collect_saving_temp_return_tag = 0;
          goto collect_saving_temp;
         collect_saving_temp_return_0:
          okayP_391X = (ShpS + (((space_389X)<<2))) < SlimitS;
          if (okayP_391X) {
            arg1K0 = 0;
            goto L28123;}
          else {
            TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
            arg1K0 = 0;
            goto L28123;}}}}
      break;
    case 4 : {
      space_27X = 2 + ((long)(*((unsigned char*)RScode_pointerS)));
      v_26X = (ShpS + (((space_27X)<<2))) < SlimitS;
      if (v_26X) {
        goto L28160;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 1;
        goto collect_saving_temp;
       collect_saving_temp_return_1:
        okayP_25X = (ShpS + (((space_27X)<<2))) < SlimitS;
        if (okayP_25X) {
          goto L28160;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L28160;}}}
      break;
    case 5 : {
      RSenvS = (*((long*)(-3 + RSenvS)));
      goto START;}
      break;
    case 6 : {
      b_385X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((64 == SnargsS)) {
        RSstackS = (4 + RSstackS);
        merged_arg1K0 = (*((long*)RSstackS));
        merged_arg1K1 = ((-1 + SnargsS) - ((long)b_385X));
        pop_args_list_return_tag = 0;
        goto pop_args_list;
       pop_args_list_return_0:
        v_28X = pop_args_list_return_value;
        arg1K0 = v_28X;
        goto L28232;}
      else {
        merged_arg1K0 = 25;
        merged_arg1K1 = (SnargsS - ((long)b_385X));
        pop_args_list_return_tag = 1;
        goto pop_args_list;
       pop_args_list_return_1:
        v_386X = pop_args_list_return_value;
        arg1K0 = v_386X;
        goto L28232;}}
      break;
    case 7 : {
      b_29X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((-3 + StemplateS) + (((((long)b_29X))<<2)))));
      goto START;}
      break;
    case 8 : {
      b_383X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      env_384X = RSenvS;
      arg1K0 = env_384X;
      arg1K1 = ((long)b_383X);
      goto L27984;}
      break;
    case 9 : {
      env_381X = RSenvS;
      b_382X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((-3 + env_381X) + (((((long)b_382X))<<2)))));
      if ((529 == RSvalS)) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 10 : {
      env_379X = *((long*)(-3 + RSenvS));
      b_380X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((-3 + env_379X) + (((((long)b_380X))<<2)))));
      if ((529 == RSvalS)) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 11 : {
      env_377X = *((long*)(-3 + (*((long*)(-3 + RSenvS)))));
      b_378X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)((-3 + env_377X) + (((((long)b_378X))<<2)))));
      if ((529 == RSvalS)) {
        merged_arg0K0 = 1;
        goto raise_exception;}
      else {
        goto START;}}
      break;
    case 12 : {
      b_31X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      env_30X = RSenvS;
      arg1K0 = env_30X;
      arg1K1 = ((long)b_31X);
      goto L28304;}
      break;
    case 13 : {
      b_33X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      location_32X = *((long*)((-3 + StemplateS) + (((((long)b_33X))<<2))));
      RSvalS = (*((long*)(-3 + location_32X)));
      if ((17 == (255 & RSvalS))) {
        merged_arg0K0 = 1;
        merged_arg1K1 = location_32X;
        goto raise_exception1;}
      else {
        goto START;}}
      break;
    case 14 : {
      b_35X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      location_34X = *((long*)((-3 + StemplateS) + (((((long)b_35X))<<2))));
      if ((273 == (*((long*)(-3 + location_34X))))) {
        merged_arg0K0 = 1;
        merged_arg1K1 = location_34X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}
      else {
        *((long*)(-3 + location_34X)) = RSvalS;
        RSvalS = 13;
        goto START;}}
      break;
    case 15 : {
      p_375X = RSenvS;
      if ((p_375X < Sstack_beginS)) {
        arg1K0 = 0;
        goto L28402;}
      else {
        if ((Sstack_endS < p_375X)) {
          arg1K0 = 0;
          goto L28402;}
        else {
          v_376X = Sstack_endS - Sstack_beginS;
          arg1K0 = v_376X;
          goto L28402;}}}
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
      b_36X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RSvalS = (*((long*)(4 + (RSstackS + (((((long)b_36X))<<2))))));
      goto START;}
      break;
    case 19 : {
      b_374X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      *((long*)(4 + (RSstackS + (((((long)b_374X))<<2))))) = RSvalS;
      goto START;}
      break;
    case 20 : {
      b_367X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_368X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_369X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      x_370X = Sstack_limitS < (-276 + RSstackS);
      if (x_370X) {
        arg1K0 = 0;
        goto L28580;}
      else {
        space_371X = 1 + ((((Sstack_endS - RSstackS))>>2));
        v_372X = (ShpS + (((space_371X)<<2))) < SlimitS;
        if (v_372X) {
          arg1K0 = 0;
          goto L28601;}
        else {
          merged_arg1K0 = 0;
          collect_saving_temp_return_tag = 2;
          goto collect_saving_temp;
         collect_saving_temp_return_2:
          okayP_373X = (ShpS + (((space_371X)<<2))) < SlimitS;
          if (okayP_373X) {
            arg1K0 = 0;
            goto L28601;}
          else {
            TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
            arg1K0 = 0;
            goto L28601;}}}}
      break;
    case 21 : {
      space_39X = 1 + ((((Sstack_endS - RSstackS))>>2));
      v_38X = (ShpS + (((space_39X)<<2))) < SlimitS;
      if (v_38X) {
        arg1K0 = 0;
        goto L30336;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 3;
        goto collect_saving_temp;
       collect_saving_temp_return_3:
        okayP_37X = (ShpS + (((space_39X)<<2))) < SlimitS;
        if (okayP_37X) {
          arg1K0 = 0;
          goto L30336;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          arg1K0 = 0;
          goto L30336;}}}
      break;
    case 22 : {
      cont_41X = *((long*)(-3 + Sbottom_of_stackS));
      if ((1 == cont_41X)) {
        if ((0 == (3 & RSvalS))) {
          RSvalS = (((RSvalS)>>2));
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
        if ((1 == cont_41X)) {
          RSstackS = (-11 + Sbottom_of_stackS);
          *((long*)(-3 + Sbottom_of_stackS)) = 1;
          arg1K0 = Sbottom_of_stackS;
          goto L28710;}
        else {
          merged_arg1K0 = cont_41X;
          copy_continuation_from_heap_return_tag = 0;
          goto copy_continuation_from_heap;
         copy_continuation_from_heap_return_0:
          v_40X = copy_continuation_from_heap_return_value;
          arg1K0 = v_40X;
          goto L28710;}}}
      break;
    case 23 : {
      SnargsS = ((long)(*((unsigned char*)RScode_pointerS)));
      merged_arg0K0 = 0;
      goto perform_application;}
      break;
    case 24 : {
      SnargsS = ((long)(*((unsigned char*)RScode_pointerS)));
      start_arg_365X = RSstackS + (((SnargsS)<<2));
      start_loc_366X = -11 + ScontS;
      if ((start_arg_365X < start_loc_366X)) {
        arg1K0 = start_loc_366X;
        arg1K1 = start_arg_365X;
        goto L16015;}
      else {
        goto L15998;}}
      break;
    case 25 : {
      RSstackS = (4 + RSstackS);
      proc_43X = *((long*)RSstackS);
      list_42X = RSvalS;
      arg1K0 = list_42X;
      arg1K1 = 0;
      arg1K2 = list_42X;
      arg3K3 = 0;
      goto L25684;}
      break;
    case 26 : {
      RSstackS = (4 + RSstackS);
      cont_45X = *((long*)RSstackS);
      if ((1 == cont_45X)) {
        RSstackS = (-11 + Sbottom_of_stackS);
        *((long*)(-3 + Sbottom_of_stackS)) = 1;
        arg1K0 = Sbottom_of_stackS;
        goto L30370;}
      else {
        merged_arg1K0 = cont_45X;
        copy_continuation_from_heap_return_tag = 1;
        goto copy_continuation_from_heap;
       copy_continuation_from_heap_return_1:
        v_44X = copy_continuation_from_heap_return_value;
        arg1K0 = v_44X;
        goto L30370;}}
      break;
    case 27 : {
      RSstackS = (4 + RSstackS);
      consumer_364X = *((long*)RSstackS);
      *((long*)RSstackS) = RSvalS;
      RSstackS = (-4 + RSstackS);
      RSvalS = consumer_364X;
      SnargsS = 1;
      merged_arg0K0 = 0;
      goto perform_application;}
      break;
    case 28 : {
      cont_48X = ScontS;
      tem_47X = *((long*)(5 + cont_48X));
      pc_46X = *((long*)(1 + cont_48X));
      StemplateS = tem_47X;
      RScode_pointerS = ((-3 + (*((long*)(-3 + tem_47X)))) + (((pc_46X)>>2)));
      RSenvS = (*((long*)(9 + cont_48X)));
      ScontS = (*((long*)(-3 + cont_48X)));
      RSstackS = (9 + cont_48X);
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
      b_50X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      tem_49X = *((long*)((-3 + StemplateS) + (((((long)b_50X))<<2))));
      StemplateS = tem_49X;
      RScode_pointerS = (-3 + (*((long*)(-3 + tem_49X))));
      goto START;}
      break;
    case 33 : {
      b_53X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      SnargsS = ((long)b_53X);
      b_52X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      tem_51X = *((long*)((-3 + StemplateS) + (((((long)b_52X))<<2))));
      StemplateS = tem_51X;
      RScode_pointerS = (-3 + (*((long*)(-3 + tem_51X))));
      if ((0 == (Spending_interruptsS & Senabled_interruptsS))) {
        goto START;}
      else {
        goto handle_interrupt;}}
      break;
    case 34 : {
      b_55X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_54X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((1 == RSvalS)) {
        RScode_pointerS = (RScode_pointerS + ((((((long)b_55X))<<8)) + ((long)b_54X)));
        goto START;}
      else {
        goto START;}}
      break;
    case 35 : {
      b_362X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_363X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      RScode_pointerS = (RScode_pointerS + ((((((long)b_362X))<<8)) + ((long)b_363X)));
      goto START;}
      break;
    case 36 : {
      if ((0 == (3 & RSvalS))) {
        val_57X = ((RSvalS)>>2);
        b_56X = *((unsigned char*)RScode_pointerS);
        RScode_pointerS = (1 + RScode_pointerS);
        if ((val_57X < 0)) {
          arg1K0 = (((((long)b_56X))<<1));
          goto L28917;}
        else {
          if ((val_57X < ((long)b_56X))) {
            RScode_pointerS = (RScode_pointerS + (((val_57X)<<1)));
            b_360X = *((unsigned char*)RScode_pointerS);
            RScode_pointerS = (1 + RScode_pointerS);
            b_361X = *((unsigned char*)RScode_pointerS);
            RScode_pointerS = (1 + RScode_pointerS);
            arg1K0 = ((((((long)b_360X))<<8)) + ((long)b_361X));
            goto L28917;}
          else {
            arg1K0 = (((((long)b_56X))<<1));
            goto L28917;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 37 : {
      RSstackS = (4 + RSstackS);
      arg2_358X = *((long*)RSstackS);
      x_359X = RSvalS;
      if ((arg2_358X == x_359X)) {
        arg0K0 = 5;
        goto L30491;}
      else {
        arg0K0 = 1;
        goto L30491;}}
      break;
    case 38 : {
      x_357X = RSvalS;
      if ((0 == (3 & x_357X))) {
        arg0K0 = 5;
        goto L30505;}
      else {
        if ((3 == (3 & x_357X))) {
          v_58X = 31 & ((((*((long*)(-7 + x_357X))))>>2));
          if ((9 == v_58X)) {
            arg0K0 = 5;
            goto L30505;}
          else {
            arg0K0 = 1;
            goto L30505;}}
        else {
          arg0K0 = 1;
          goto L30505;}}}
      break;
    case 39 : {
      x_356X = RSvalS;
      if ((0 == (3 & x_356X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_356X))) {
          v_59X = 31 & ((((*((long*)(-7 + x_356X))))>>2));
          if ((9 == v_59X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_356X;
            goto raise_exception1;}
          else {
            goto L30525;}}
        else {
          goto L30525;}}}
      break;
    case 40 : {
      x_355X = RSvalS;
      if ((0 == (3 & x_355X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_355X))) {
          v_60X = 31 & ((((*((long*)(-7 + x_355X))))>>2));
          if ((9 == v_60X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_355X;
            goto raise_exception1;}
          else {
            goto L30554;}}
        else {
          goto L30554;}}}
      break;
    case 41 : {
      x_354X = RSvalS;
      if ((0 == (3 & x_354X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_354X))) {
          v_61X = 31 & ((((*((long*)(-7 + x_354X))))>>2));
          if ((9 == v_61X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_354X;
            goto raise_exception1;}
          else {
            goto L30583;}}
        else {
          goto L30583;}}}
      break;
    case 42 : {
      x_353X = RSvalS;
      if ((0 == (3 & x_353X))) {
        RSvalS = 5;
        goto START;}
      else {
        if ((3 == (3 & x_353X))) {
          v_62X = 31 & ((((*((long*)(-7 + x_353X))))>>2));
          if ((9 == v_62X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = x_353X;
            goto raise_exception1;}
          else {
            goto L30612;}}
        else {
          goto L30612;}}}
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
      arg2_65X = *((long*)RSstackS);
      if ((0 == (3 & arg2_65X))) {
        v_352X = 0 == (3 & RSvalS);
        if (v_352X) {
          x_64X = RSvalS;
          z_63X = (((arg2_65X)>>2)) + (((x_64X)>>2));
          if ((536870911 < z_63X)) {
            goto L24535;}
          else {
            if ((z_63X < -536870912)) {
              goto L24535;}
            else {
              RSvalS = (((z_63X)<<2));
              goto START;}}}
        else {
          goto L24513;}}
      else {
        goto L24513;}}
      break;
    case 47 : {
      RSstackS = (4 + RSstackS);
      arg2_69X = *((long*)RSstackS);
      if ((0 == (3 & arg2_69X))) {
        v_351X = 0 == (3 & RSvalS);
        if (v_351X) {
          x_68X = RSvalS;
          a_67X = ((arg2_69X)>>2);
          b_66X = ((x_68X)>>2);
          if ((a_67X < 0)) {
            arg1K0 = (0 - a_67X);
            goto L24331;}
          else {
            arg1K0 = a_67X;
            goto L24331;}}
        else {
          goto L24306;}}
      else {
        goto L24306;}}
      break;
    case 48 : {
      RSstackS = (4 + RSstackS);
      arg2_348X = *((long*)RSstackS);
      if ((0 == (3 & arg2_348X))) {
        v_70X = 0 == (3 & RSvalS);
        if (v_70X) {
          x_349X = RSvalS;
          z_350X = (((arg2_348X)>>2)) - (((x_349X)>>2));
          if ((536870911 < z_350X)) {
            goto L24214;}
          else {
            if ((z_350X < -536870912)) {
              goto L24214;}
            else {
              RSvalS = (((z_350X)<<2));
              goto START;}}}
        else {
          goto L24192;}}
      else {
        goto L24192;}}
      break;
    case 49 : {
      RSstackS = (4 + RSstackS);
      arg2_74X = *((long*)RSstackS);
      if ((0 == (3 & arg2_74X))) {
        v_347X = 0 == (3 & RSvalS);
        if (v_347X) {
          x_73X = RSvalS;
          if ((0 == x_73X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = arg2_74X;
            merged_arg1K2 = x_73X;
            goto raise_exception2;}
          else {
            a_72X = ((arg2_74X)>>2);
            b_71X = ((x_73X)>>2);
            if ((a_72X < 0)) {
              arg1K0 = (0 - a_72X);
              goto L24008;}
            else {
              arg1K0 = a_72X;
              goto L24008;}}}
        else {
          goto L23982;}}
      else {
        goto L23982;}}
      break;
    case 50 : {
      RSstackS = (4 + RSstackS);
      arg2_76X = *((long*)RSstackS);
      if ((0 == (3 & arg2_76X))) {
        v_346X = 0 == (3 & RSvalS);
        if (v_346X) {
          x_75X = RSvalS;
          if ((arg2_76X == x_75X)) {
            arg0K0 = 5;
            goto L23907;}
          else {
            arg0K0 = 1;
            goto L23907;}}
        else {
          goto L23898;}}
      else {
        goto L23898;}}
      break;
    case 51 : {
      RSstackS = (4 + RSstackS);
      arg2_78X = *((long*)RSstackS);
      if ((0 == (3 & arg2_78X))) {
        v_345X = 0 == (3 & RSvalS);
        if (v_345X) {
          x_77X = RSvalS;
          if ((arg2_78X < x_77X)) {
            arg0K0 = 5;
            goto L23823;}
          else {
            arg0K0 = 1;
            goto L23823;}}
        else {
          goto L23814;}}
      else {
        goto L23814;}}
      break;
    case 52 : {
      RSstackS = (4 + RSstackS);
      arg2_341X = *((long*)RSstackS);
      if ((0 == (3 & arg2_341X))) {
        v_79X = 0 == (3 & RSvalS);
        if (v_79X) {
          x_342X = RSvalS;
          if ((0 == x_342X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = arg2_341X;
            merged_arg1K2 = x_342X;
            goto raise_exception2;}
          else {
            a_343X = ((arg2_341X)>>2);
            b_344X = ((x_342X)>>2);
            if ((a_343X < 0)) {
              arg1K0 = (0 - a_343X);
              goto L23673;}
            else {
              arg1K0 = a_343X;
              goto L23673;}}}
        else {
          goto L23647;}}
      else {
        goto L23647;}}
      break;
    case 53 : {
      RSstackS = (4 + RSstackS);
      arg2_83X = *((long*)RSstackS);
      if ((0 == (3 & arg2_83X))) {
        v_340X = 0 == (3 & RSvalS);
        if (v_340X) {
          x_82X = RSvalS;
          if ((0 == x_82X)) {
            merged_arg0K0 = 0;
            merged_arg1K1 = arg2_83X;
            merged_arg1K2 = x_82X;
            goto raise_exception2;}
          else {
            a_81X = ((arg2_83X)>>2);
            b_80X = ((x_82X)>>2);
            if ((a_81X < 0)) {
              arg1K0 = (0 - a_81X);
              goto L23542;}
            else {
              arg1K0 = a_81X;
              goto L23542;}}}
        else {
          goto L23516;}}
      else {
        goto L23516;}}
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
      if ((0 == (3 & RSvalS))) {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 67 : {
      if ((0 == (3 & RSvalS))) {
        x_84X = RSvalS;
        if ((x_84X < 0)) {
          merged_arg0K0 = 0;
          merged_arg1K1 = x_84X;
          goto raise_exception1;}
        else {
          RSvalS = 0;
          goto START;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 68 : {
      if ((0 == (3 & RSvalS))) {
        x_339X = RSvalS;
        if ((x_339X < 0)) {
          arg1K0 = (0 - x_339X);
          goto L30869;}
        else {
          arg1K0 = x_339X;
          goto L30869;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 69 : {
      RSstackS = (4 + RSstackS);
      arg2_338X = *((long*)RSstackS);
      if ((0 == (3 & arg2_338X))) {
        v_85X = 0 == (3 & RSvalS);
        if (v_85X) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_338X;
          merged_arg1K2 = RSvalS;
          goto raise_exception2;}
        else {
          goto L30892;}}
      else {
        goto L30892;}}
      break;
    case 70 : {
      RSstackS = (4 + RSstackS);
      arg2_86X = *((long*)RSstackS);
      if ((0 == (3 & arg2_86X))) {
        v_337X = 0 == (3 & RSvalS);
        if (v_337X) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_86X;
          merged_arg1K2 = RSvalS;
          goto raise_exception2;}
        else {
          goto L30913;}}
      else {
        goto L30913;}}
      break;
    case 71 : {
      RSstackS = (4 + RSstackS);
      arg2_336X = *((long*)RSstackS);
      if ((0 == (3 & arg2_336X))) {
        v_87X = 0 == (3 & RSvalS);
        if (v_87X) {
          merged_arg0K0 = 0;
          merged_arg1K1 = arg2_336X;
          merged_arg1K2 = RSvalS;
          goto raise_exception2;}
        else {
          goto L30934;}}
      else {
        goto L30934;}}
      break;
    case 72 : {
      if ((0 == (3 & RSvalS))) {
        RSvalS = ((((~ (((RSvalS)>>2))))<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 73 : {
      RSstackS = (4 + RSstackS);
      arg2_88X = *((long*)RSstackS);
      if ((0 == (3 & arg2_88X))) {
        v_335X = 0 == (3 & RSvalS);
        if (v_335X) {
          RSvalS = (((((((arg2_88X)>>2)) & (((RSvalS)>>2))))<<2));
          goto START;}
        else {
          goto L30967;}}
      else {
        goto L30967;}}
      break;
    case 74 : {
      RSstackS = (4 + RSstackS);
      arg2_334X = *((long*)RSstackS);
      if ((0 == (3 & arg2_334X))) {
        v_89X = 0 == (3 & RSvalS);
        if (v_89X) {
          RSvalS = (((((((arg2_334X)>>2)) | (((RSvalS)>>2))))<<2));
          goto START;}
        else {
          goto L30990;}}
      else {
        goto L30990;}}
      break;
    case 75 : {
      RSstackS = (4 + RSstackS);
      arg2_90X = *((long*)RSstackS);
      if ((0 == (3 & arg2_90X))) {
        v_333X = 0 == (3 & RSvalS);
        if (v_333X) {
          RSvalS = (((((((arg2_90X)>>2)) ^ (((RSvalS)>>2))))<<2));
          goto START;}
        else {
          goto L31013;}}
      else {
        goto L31013;}}
      break;
    case 76 : {
      RSstackS = (4 + RSstackS);
      arg2_97X = *((long*)RSstackS);
      if ((0 == (3 & arg2_97X))) {
        v_331X = 0 == (3 & RSvalS);
        if (v_331X) {
          x_96X = RSvalS;
          count_95X = ((x_96X)>>2);
          value_94X = ((arg2_97X)>>2);
          if ((count_95X < 0)) {
            PS_SHIFT_RIGHT(value_94X, (0 - count_95X), x_332X)
            RSvalS = (((x_332X)<<2));
            goto START;}
          else {
            PS_SHIFT_LEFT(value_94X, count_95X, x_93X)
            result_92X = (((((x_93X)<<2)))>>2);
            PS_SHIFT_RIGHT(result_92X, count_95X, x_91X)
            if ((value_94X == x_91X)) {
              if ((value_94X < 0)) {
                if ((result_92X < 0)) {
                  goto L22991;}
                else {
                  goto L22988;}}
              else {
                if ((result_92X < 0)) {
                  goto L22988;}
                else {
                  goto L22991;}}}
            else {
              goto L22988;}}}
        else {
          goto L22961;}}
      else {
        goto L22961;}}
      break;
    case 77 : {
      x_98X = RSvalS;
      if ((9 == (255 & x_98X))) {
        arg0K0 = 5;
        goto L31039;}
      else {
        arg0K0 = 1;
        goto L31039;}}
      break;
    case 78 : {
      RSstackS = (4 + RSstackS);
      arg2_100X = *((long*)RSstackS);
      if ((9 == (255 & arg2_100X))) {
        v_330X = 9 == (255 & RSvalS);
        if (v_330X) {
          x_99X = RSvalS;
          if ((arg2_100X == x_99X)) {
            arg0K0 = 5;
            goto L22886;}
          else {
            arg0K0 = 1;
            goto L22886;}}
        else {
          goto L22877;}}
      else {
        goto L22877;}}
      break;
    case 79 : {
      RSstackS = (4 + RSstackS);
      arg2_102X = *((long*)RSstackS);
      if ((9 == (255 & arg2_102X))) {
        v_329X = 9 == (255 & RSvalS);
        if (v_329X) {
          x_101X = RSvalS;
          if ((arg2_102X < x_101X)) {
            arg0K0 = 5;
            goto L22802;}
          else {
            arg0K0 = 1;
            goto L22802;}}
        else {
          goto L22793;}}
      else {
        goto L22793;}}
      break;
    case 80 : {
      if ((9 == (255 & RSvalS))) {
        RSvalS = (((((long)(((((RSvalS)>>8))))))<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 81 : {
      if ((0 == (3 & RSvalS))) {
        x_328X = ((RSvalS)>>2);
        if ((255 < x_328X)) {
          goto L31076;}
        else {
          if ((x_328X < 0)) {
            goto L31076;}
          else {
            RSvalS = (9 + (((((long)((x_328X))))<<8)));
            goto START;}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 82 : {
      x_103X = RSvalS;
      if ((21 == x_103X)) {
        arg0K0 = 5;
        goto L31098;}
      else {
        arg0K0 = 1;
        goto L31098;}}
      break;
    case 83 : {
      x_326X = RSvalS;
      b_327X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_326X))) {
        v_104X = 31 & ((((*((long*)(-7 + x_326X))))>>2));
        if ((v_104X == ((long)b_327X))) {
          arg0K0 = 5;
          goto L31116;}
        else {
          arg0K0 = 1;
          goto L31116;}}
      else {
        arg0K0 = 1;
        goto L31116;}}
      break;
    case 84 : {
      x_107X = RSvalS;
      b_106X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_107X))) {
        v_325X = 31 & ((((*((long*)(-7 + x_107X))))>>2));
        if ((v_325X == ((long)b_106X))) {
          x_105X = (((3 + ((long)(((unsigned long)(*((long*)(-7 + x_107X))))>>8))))>>2);
          RSvalS = (((x_105X)<<2));
          goto START;}
        else {
          goto L22702;}}
      else {
        goto L22702;}}
      break;
    case 85 : {
      space_322X = 4 + (((((long)(*((unsigned char*)RScode_pointerS))))<<2));
      v_323X = (ShpS + (((space_322X)<<2))) < SlimitS;
      if (v_323X) {
        goto L16739;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 4;
        goto collect_saving_temp;
       collect_saving_temp_return_4:
        okayP_324X = (ShpS + (((space_322X)<<2))) < SlimitS;
        if (okayP_324X) {
          goto L16739;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L16739;}}}
      break;
    case 86 : {
      x_110X = RSvalS;
      b_109X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_108X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & x_110X))) {
        v_321X = 31 & ((((*((long*)(-7 + x_110X))))>>2));
        if ((v_321X == ((long)b_109X))) {
          RSvalS = (*((long*)((-3 + x_110X) + (((((long)b_108X))<<2)))));
          goto START;}
        else {
          goto L25565;}}
      else {
        goto L25565;}}
      break;
    case 87 : {
      RSstackS = (4 + RSstackS);
      arg2_317X = *((long*)RSstackS);
      x_318X = RSvalS;
      b_319X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      b_320X = *((unsigned char*)RScode_pointerS);
      RScode_pointerS = (1 + RScode_pointerS);
      if ((3 == (3 & arg2_317X))) {
        v_111X = 31 & ((((*((long*)(-7 + arg2_317X))))>>2));
        if ((v_111X == ((long)b_319X))) {
          if ((3 == (3 & arg2_317X))) {
            if ((0 == (128 & (*((long*)(-7 + arg2_317X)))))) {
              *((long*)((-3 + arg2_317X) + (((((long)b_320X))<<2)))) = x_318X;
              RSvalS = 13;
              goto START;}
            else {
              goto L26044;}}
          else {
            goto L26044;}}
        else {
          goto L26044;}}
      else {
        goto L26044;}}
      break;
    case 88 : {
      RSstackS = (4 + RSstackS);
      arg2_112X = *((long*)RSstackS);
      if ((0 == (3 & arg2_112X))) {
        x_310X = RSvalS;
        len_311X = ((arg2_112X)>>2);
        b_312X = *((unsigned char*)RScode_pointerS);
        RScode_pointerS = (1 + RScode_pointerS);
        size_313X = 1 + len_311X;
        if ((size_313X < 0)) {
          goto L25432;}
        else {
          if ((4194304 < size_313X)) {
            goto L25432;}
          else {
            v_314X = (ShpS + (((size_313X)<<2))) < SlimitS;
            if (v_314X) {
              arg3K0 = 1;
              arg1K1 = x_310X;
              goto L25302;}
            else {
              merged_arg1K0 = x_310X;
              collect_saving_temp_return_tag = 5;
              goto collect_saving_temp;
             collect_saving_temp_return_5:
              temp_315X = collect_saving_temp_return_value;
              v_316X = (ShpS + (((size_313X)<<2))) < SlimitS;
              arg3K0 = v_316X;
              arg1K1 = temp_315X;
              goto L25302;}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_112X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 89 : {
      RSstackS = (4 + RSstackS);
      arg2_115X = *((long*)RSstackS);
      v_114X = 0 == (3 & RSvalS);
      if (v_114X) {
        index_307X = ((RSvalS)>>2);
        b_308X = *((unsigned char*)RScode_pointerS);
        RScode_pointerS = (1 + RScode_pointerS);
        if ((3 == (3 & arg2_115X))) {
          v_113X = 31 & ((((*((long*)(-7 + arg2_115X))))>>2));
          if ((v_113X == ((long)b_308X))) {
            len_309X = (((3 + ((long)(((unsigned long)(*((long*)(-7 + arg2_115X))))>>8))))>>2);
            if ((index_307X < 0)) {
              goto L25123;}
            else {
              if ((index_307X < len_309X)) {
                RSvalS = (*((long*)((-3 + arg2_115X) + (((index_307X)<<2)))));
                goto START;}
              else {
                goto L25123;}}}
          else {
            goto L25123;}}
        else {
          goto L25123;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_115X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 90 : {
      RSstackS = (4 + RSstackS);
      arg2_118X = *((long*)RSstackS);
      RSstackS = (4 + RSstackS);
      arg3_117X = *((long*)RSstackS);
      if ((0 == (3 & arg2_118X))) {
        x_303X = RSvalS;
        index_304X = ((arg2_118X)>>2);
        b_305X = *((unsigned char*)RScode_pointerS);
        RScode_pointerS = (1 + RScode_pointerS);
        if ((3 == (3 & arg3_117X))) {
          v_116X = 31 & ((((*((long*)(-7 + arg3_117X))))>>2));
          if ((v_116X == ((long)b_305X))) {
            len_306X = (((3 + ((long)(((unsigned long)(*((long*)(-7 + arg3_117X))))>>8))))>>2);
            if ((index_304X < 0)) {
              goto L25786;}
            else {
              if ((index_304X < len_306X)) {
                if ((3 == (3 & arg3_117X))) {
                  if ((0 == (128 & (*((long*)(-7 + arg3_117X)))))) {
                    *((long*)((-3 + arg3_117X) + (((index_304X)<<2)))) = x_303X;
                    RSvalS = 13;
                    goto START;}
                  else {
                    goto L25786;}}
                else {
                  goto L25786;}}
              else {
                goto L25786;}}}
          else {
            goto L25786;}}
        else {
          goto L25786;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg3_117X;
        merged_arg1K2 = arg2_118X;
        merged_arg1K3 = RSvalS;
        goto raise_exception3;}}
      break;
    case 91 : {
      RSstackS = (4 + RSstackS);
      arg2_297X = *((long*)RSstackS);
      if ((0 == (3 & arg2_297X))) {
        v_119X = 0 == (3 & RSvalS);
        if (v_119X) {
          init_298X = ((RSvalS)>>2);
          len_299X = ((arg2_297X)>>2);
          size_300X = 1 + ((((3 + len_299X))>>2));
          if ((size_300X < 0)) {
            goto L22560;}
          else {
            if ((4194304 < size_300X)) {
              goto L22560;}
            else {
              v_301X = (ShpS + (((size_300X)<<2))) < SlimitS;
              if (v_301X) {
                goto L22511;}
              else {
                merged_arg1K0 = 0;
                collect_saving_temp_return_tag = 6;
                goto collect_saving_temp;
               collect_saving_temp_return_6:
                okayP_302X = (ShpS + (((size_300X)<<2))) < SlimitS;
                if (okayP_302X) {
                  goto L22511;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (((len_299X)<<2));
                  merged_arg1K2 = (((init_298X)<<2));
                  goto raise_exception2;}}}}}
        else {
          goto L22462;}}
      else {
        goto L22462;}}
      break;
    case 92 : {
      obj_121X = RSvalS;
      if ((3 == (3 & obj_121X))) {
        v_296X = 31 & ((((*((long*)(-7 + obj_121X))))>>2));
        if ((16 == v_296X)) {
          x_120X = (long)(((unsigned long)(*((long*)(-7 + RSvalS))))>>8);
          RSvalS = (((x_120X)<<2));
          goto START;}
        else {
          goto L31165;}}
      else {
        goto L31165;}}
      break;
    case 93 : {
      RSstackS = (4 + RSstackS);
      arg2_123X = *((long*)RSstackS);
      if ((3 == (3 & arg2_123X))) {
        v_293X = 31 & ((((*((long*)(-7 + arg2_123X))))>>2));
        if ((16 == v_293X)) {
          v_122X = 0 == (3 & RSvalS);
          if (v_122X) {
            index_294X = ((RSvalS)>>2);
            len_295X = (long)(((unsigned long)(*((long*)(-7 + arg2_123X))))>>8);
            if ((index_294X < 0)) {
              goto L22272;}
            else {
              if ((index_294X < len_295X)) {
                RSvalS = (((((long)(*((unsigned char*)((-3 + arg2_123X) + index_294X)))))<<2));
                goto START;}
              else {
                goto L22272;}}}
          else {
            goto L22277;}}
        else {
          goto L22277;}}
      else {
        goto L22277;}}
      break;
    case 94 : {
      RSstackS = (4 + RSstackS);
      arg2_288X = *((long*)RSstackS);
      RSstackS = (4 + RSstackS);
      arg3_289X = *((long*)RSstackS);
      if ((3 == (3 & arg3_289X))) {
        v_125X = 31 & ((((*((long*)(-7 + arg3_289X))))>>2));
        if ((16 == v_125X)) {
          if ((0 == (3 & arg2_288X))) {
            v_124X = 0 == (3 & RSvalS);
            if (v_124X) {
              value_290X = ((RSvalS)>>2);
              index_291X = ((arg2_288X)>>2);
              len_292X = (long)(((unsigned long)(*((long*)(-7 + arg3_289X))))>>8);
              if ((index_291X < 0)) {
                goto L24947;}
              else {
                if ((index_291X < len_292X)) {
                  *((unsigned char*)((-3 + arg3_289X) + index_291X)) = ((unsigned char)value_290X);
                  RSvalS = 13;
                  goto START;}
                else {
                  goto L24947;}}}
            else {
              goto L24954;}}
          else {
            goto L24954;}}
        else {
          goto L24954;}}
      else {
        goto L24954;}}
      break;
    case 95 : {
      RSstackS = (4 + RSstackS);
      arg2_131X = *((long*)RSstackS);
      if ((0 == (3 & arg2_131X))) {
        v_287X = 9 == (255 & RSvalS);
        if (v_287X) {
          init_130X = (((RSvalS)>>8));
          len_129X = ((arg2_131X)>>2);
          size_128X = 1 + ((((4 + len_129X))>>2));
          if ((size_128X < 0)) {
            goto L22119;}
          else {
            if ((4194304 < size_128X)) {
              goto L22119;}
            else {
              v_127X = (ShpS + (((size_128X)<<2))) < SlimitS;
              if (v_127X) {
                goto L22070;}
              else {
                merged_arg1K0 = 0;
                collect_saving_temp_return_tag = 7;
                goto collect_saving_temp;
               collect_saving_temp_return_7:
                okayP_126X = (ShpS + (((size_128X)<<2))) < SlimitS;
                if (okayP_126X) {
                  goto L22070;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (((len_129X)<<2));
                  merged_arg1K2 = (9 + (((((long)(init_130X)))<<8)));
                  goto raise_exception2;}}}}}
        else {
          goto L22019;}}
      else {
        goto L22019;}}
      break;
    case 96 : {
      obj_285X = RSvalS;
      if ((3 == (3 & obj_285X))) {
        v_132X = 31 & ((((*((long*)(-7 + obj_285X))))>>2));
        if ((15 == v_132X)) {
          v_286X = (long)(((unsigned long)(*((long*)(-7 + RSvalS))))>>8);
          RSvalS = (-4 + (((v_286X)<<2)));
          goto START;}
        else {
          goto L31200;}}
      else {
        goto L31200;}}
      break;
    case 97 : {
      RSstackS = (4 + RSstackS);
      arg2_282X = *((long*)RSstackS);
      if ((3 == (3 & arg2_282X))) {
        v_135X = 31 & ((((*((long*)(-7 + arg2_282X))))>>2));
        if ((15 == v_135X)) {
          v_283X = 0 == (3 & RSvalS);
          if (v_283X) {
            index_134X = ((RSvalS)>>2);
            v_133X = (long)(((unsigned long)(*((long*)(-7 + arg2_282X))))>>8);
            if ((index_134X < 0)) {
              goto L21827;}
            else {
              if ((index_134X < (-1 + v_133X))) {
                x_284X = ((long)(*((unsigned char*)((-3 + arg2_282X) + index_134X))));
                RSvalS = (9 + (((((long)(x_284X)))<<8)));
                goto START;}
              else {
                goto L21827;}}}
          else {
            goto L21832;}}
        else {
          goto L21832;}}
      else {
        goto L21832;}}
      break;
    case 98 : {
      RSstackS = (4 + RSstackS);
      arg2_140X = *((long*)RSstackS);
      RSstackS = (4 + RSstackS);
      arg3_139X = *((long*)RSstackS);
      if ((3 == (3 & arg3_139X))) {
        v_280X = 31 & ((((*((long*)(-7 + arg3_139X))))>>2));
        if ((15 == v_280X)) {
          if ((0 == (3 & arg2_140X))) {
            v_281X = 9 == (255 & RSvalS);
            if (v_281X) {
              Kchar_138X = (((RSvalS)>>8));
              index_137X = ((arg2_140X)>>2);
              v_136X = (long)(((unsigned long)(*((long*)(-7 + arg3_139X))))>>8);
              if ((index_137X < 0)) {
                goto L24769;}
              else {
                if ((index_137X < (-1 + v_136X))) {
                  *((unsigned char*)((-3 + arg3_139X) + index_137X)) = ((unsigned char)((long)(Kchar_138X)));
                  RSvalS = 13;
                  goto START;}
                else {
                  goto L24769;}}}
            else {
              goto L24776;}}
          else {
            goto L24776;}}
        else {
          goto L24776;}}
      else {
        goto L24776;}}
      break;
    case 99 : {
      obj_277X = RSvalS;
      if ((3 == (3 & obj_277X))) {
        v_142X = 31 & ((((*((long*)(-7 + obj_277X))))>>2));
        if ((4 == v_142X)) {
          x_278X = RSvalS;
          descriptor_279X = *((long*)(-3 + x_278X));
          if ((17 == (255 & descriptor_279X))) {
            x_141X = 529 == (*((long*)(-3 + x_278X)));
            if (x_141X) {
              arg0K0 = 5;
              goto L18416;}
            else {
              arg0K0 = 1;
              goto L18416;}}
          else {
            arg0K0 = 5;
            goto L18416;}}
        else {
          goto L18407;}}
      else {
        goto L18407;}}
      break;
    case 100 : {
      RSstackS = (4 + RSstackS);
      arg2_144X = *((long*)RSstackS);
      if ((3 == (3 & arg2_144X))) {
        v_276X = 31 & ((((*((long*)(-7 + arg2_144X))))>>2));
        if ((4 == v_276X)) {
          x_143X = RSvalS;
          if ((1 == x_143X)) {
            goto L21625;}
          else {
            if ((5 == x_143X)) {
              goto L21625;}
            else {
              goto L21653;}}}
        else {
          goto L21653;}}
      else {
        goto L21653;}}
      break;
    case 101 : {
      x_275X = RSvalS;
      if ((3 == (3 & x_275X))) {
        if ((0 == (128 & (*((long*)(-7 + x_275X)))))) {
          arg0K0 = 1;
          goto L31240;}
        else {
          arg0K0 = 5;
          goto L31240;}}
      else {
        arg0K0 = 5;
        goto L31240;}}
      break;
    case 102 : {
      x_145X = RSvalS;
      if ((3 == (3 & x_145X))) {
        if ((0 == (128 & (*((long*)(-7 + x_145X)))))) {
          *((long*)(-7 + x_145X)) = (128 | (*((long*)(-7 + x_145X))));
          goto L31255;}
        else {
          goto L31255;}}
      else {
        goto L31255;}}
      break;
    case 103 : {
      v_273X = (20 + ShpS) < SlimitS;
      if (v_273X) {
        arg1K0 = 0;
        goto L21333;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 8;
        goto collect_saving_temp;
       collect_saving_temp_return_8:
        okayP_274X = (20 + ShpS) < SlimitS;
        if (okayP_274X) {
          arg1K0 = 0;
          goto L21333;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          arg1K0 = 0;
          goto L21333;}}}
      break;
    case 104 : {
      obj_146X = RSvalS;
      if ((3 == (3 & obj_146X))) {
        v_272X = 31 & ((((*((long*)(-7 + obj_146X))))>>2));
        if ((5 == v_272X)) {
          merged_arg1K0 = RSvalS;
          close_port_return_tag = 0;
          goto close_port;
         close_port_return_0:
          RSvalS = 13;
          goto START;}
        else {
          goto L31276;}}
      else {
        goto L31276;}}
      break;
    case 105 : {
      x_148X = RSvalS;
      if ((3 == (3 & x_148X))) {
        v_271X = 31 & ((((*((long*)(-7 + x_148X))))>>2));
        if ((5 == v_271X)) {
          x_147X = 4 == (*((long*)(-3 + x_148X)));
          if (x_147X) {
            arg0K0 = 5;
            goto L31300;}
          else {
            arg0K0 = 1;
            goto L31300;}}
        else {
          arg0K0 = 1;
          goto L31300;}}
      else {
        arg0K0 = 1;
        goto L31300;}}
      break;
    case 106 : {
      x_150X = RSvalS;
      if ((3 == (3 & x_150X))) {
        v_270X = 31 & ((((*((long*)(-7 + x_150X))))>>2));
        if ((5 == v_270X)) {
          x_149X = 8 == (*((long*)(-3 + x_150X)));
          if (x_149X) {
            arg0K0 = 5;
            goto L31322;}
          else {
            arg0K0 = 1;
            goto L31322;}}
        else {
          arg0K0 = 1;
          goto L31322;}}
      else {
        arg0K0 = 1;
        goto L31322;}}
      break;
    case 107 : {
      p_267X = RSvalS;
      if ((3 == (3 & p_267X))) {
        v_154X = 31 & ((((*((long*)(-7 + p_267X))))>>2));
        if ((5 == v_154X)) {
          v_268X = 4 == (*((long*)(-3 + p_267X)));
          if (v_268X) {
            if (((*((long*)(1 + p_267X))) < 0)) {
              goto L17974;}
            else {
              x_153X = RSvalS;
              c_152X = *((long*)(5 + x_153X));
              if ((1 == c_152X)) {
                v_269X = *(Sopen_portsS + ((((*((long*)(1 + x_153X))))>>2)));
                { int TTchar;
                  PS_GETC(v_269X, TTchar);
                  if (EOF == TTchar) {
                    arg1K0 = 21;
                    goto L17971;}
                  else {
                    unsigned char Kchar_151X = TTchar;
                    arg1K0 = (9 + (((((long)(Kchar_151X)))<<8)));
                    goto L17971;}}}
              else {
                *((long*)(5 + x_153X)) = 1;
                arg1K0 = c_152X;
                goto L17971;}}}
          else {
            goto L17974;}}
        else {
          goto L17974;}}
      else {
        goto L17974;}}
      break;
    case 108 : {
      p_157X = RSvalS;
      if ((3 == (3 & p_157X))) {
        v_263X = 31 & ((((*((long*)(-7 + p_157X))))>>2));
        if ((5 == v_263X)) {
          v_156X = 4 == (*((long*)(-3 + p_157X)));
          if (v_156X) {
            if (((*((long*)(1 + p_157X))) < 0)) {
              goto L17852;}
            else {
              x_264X = RSvalS;
              c_265X = *((long*)(5 + x_264X));
              if ((1 == c_265X)) {
                v_155X = *(Sopen_portsS + ((((*((long*)(1 + x_264X))))>>2)));
                { int TTchar;
                  PS_GETC(v_155X, TTchar);
                  if (EOF == TTchar) {
                    arg1K0 = 21;
                    goto L17844;}
                  else {
                    unsigned char Kchar_266X = TTchar;
                    arg1K0 = (9 + (((((long)(Kchar_266X)))<<8)));
                    goto L17844;}}}
              else {
                arg1K0 = c_265X;
                goto L17849;}}}
          else {
            goto L17852;}}
        else {
          goto L17852;}}
      else {
        goto L17852;}}
      break;
    case 109 : {
      p_259X = RSvalS;
      if ((3 == (3 & p_259X))) {
        v_160X = 31 & ((((*((long*)(-7 + p_259X))))>>2));
        if ((5 == v_160X)) {
          v_260X = 4 == (*((long*)(-3 + p_259X)));
          if (v_260X) {
            if (((*((long*)(1 + p_259X))) < 0)) {
              goto L17695;}
            else {
              x_159X = RSvalS;
              x_158X = *((long*)(5 + x_159X));
              if ((1 == x_158X)) {
                v_261X = *(Sopen_portsS + ((((*((long*)(1 + x_159X))))>>2)));
                b_262X = char_ready_p(v_261X);
                if (b_262X) {
                  arg0K0 = 5;
                  goto L17692;}
                else {
                  arg0K0 = 1;
                  goto L17692;}}
              else {
                arg0K0 = 5;
                goto L17692;}}}
          else {
            goto L17695;}}
        else {
          goto L17695;}}
      else {
        goto L17695;}}
      break;
    case 110 : {
      RSstackS = (4 + RSstackS);
      arg2_256X = *((long*)RSstackS);
      if ((9 == (255 & arg2_256X))) {
        p_162X = RSvalS;
        if ((3 == (3 & p_162X))) {
          v_257X = 31 & ((((*((long*)(-7 + p_162X))))>>2));
          if ((5 == v_257X)) {
            v_161X = 8 == (*((long*)(-3 + p_162X)));
            if (v_161X) {
              if (((*((long*)(1 + p_162X))) < 0)) {
                goto L21238;}
              else {
                port_258X = *(Sopen_portsS + ((((*((long*)(1 + RSvalS))))>>2)));
                putc(((((arg2_256X)>>8))), port_258X);
                RSvalS = 13;
                goto START;}}
            else {
              goto L21238;}}
          else {
            goto L21238;}}
        else {
          goto L21238;}}
      else {
        goto L21238;}}
      break;
    case 111 : {
      RSstackS = (4 + RSstackS);
      arg2_165X = *((long*)RSstackS);
      if ((3 == (3 & arg2_165X))) {
        v_252X = 31 & ((((*((long*)(-7 + arg2_165X))))>>2));
        if ((15 == v_252X)) {
          p_164X = RSvalS;
          if ((3 == (3 & p_164X))) {
            v_253X = 31 & ((((*((long*)(-7 + p_164X))))>>2));
            if ((5 == v_253X)) {
              v_163X = 8 == (*((long*)(-3 + p_164X)));
              if (v_163X) {
                if (((*((long*)(1 + p_164X))) < 0)) {
                  goto L21094;}
                else {
                  port_254X = *(Sopen_portsS + ((((*((long*)(1 + RSvalS))))>>2)));
                  v_255X = (long)(((unsigned long)(*((long*)(-7 + arg2_165X))))>>8);
                  fwrite((void *)(-3 + arg2_165X), sizeof(char), (-1 + v_255X), port_254X);
                  RSvalS = 13;
                  goto START;}}
              else {
                goto L21094;}}
            else {
              goto L21094;}}
          else {
            goto L21094;}}
        else {
          goto L21094;}}
      else {
        goto L21094;}}
      break;
    case 112 : {
      p_250X = RSvalS;
      if ((3 == (3 & p_250X))) {
        v_167X = 31 & ((((*((long*)(-7 + p_250X))))>>2));
        if ((5 == v_167X)) {
          v_251X = 8 == (*((long*)(-3 + p_250X)));
          if (v_251X) {
            if (((*((long*)(1 + p_250X))) < 0)) {
              goto L17599;}
            else {
              port_166X = *(Sopen_portsS + ((((*((long*)(1 + RSvalS))))>>2)));
              fflush(port_166X);
              RSvalS = 13;
              goto START;}}
          else {
            goto L17599;}}
        else {
          goto L17599;}}
      else {
        goto L17599;}}
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
      merged_arg1K1 = RSvalS;
      goto raise_exception1;}
      break;
    case 116 : {
      RSvalS = 1;
      goto START;}
      break;
    case 117 : {
      RSstackS = (4 + RSstackS);
      arg2_177X = *((long*)RSstackS);
      RSstackS = (4 + RSstackS);
      arg3_176X = *((long*)RSstackS);
      if ((3 == (3 & arg3_176X))) {
        v_245X = 31 & ((((*((long*)(-7 + arg3_176X))))>>2));
        if ((15 == v_245X)) {
          obj_175X = RSvalS;
          if ((3 == (3 & obj_175X))) {
            v_246X = 31 & ((((*((long*)(-7 + obj_175X))))>>2));
            if ((15 == v_246X)) {
              x_174X = RSvalS;
              if ((0 == ((long)Spure_area_countS))) {
                x_247X = 0 == ((long)Simpure_area_countS);
                if (x_247X) {
                  merged_arg1K0 = arg3_176X;
                  extract_string_return_tag = 0;
                  goto extract_string;
                 extract_string_return_0:
                  spec_173X = extract_string_return_value;
                  port_172X = ps_open(spec_173X, "w");
                  if ((NULL == port_172X)) {
                    merged_arg0K0 = 0;
                    merged_arg1K1 = arg3_176X;
                    merged_arg1K2 = arg2_177X;
                    goto raise_exception2;}
                  else {
                    v_171X = (long)(((unsigned long)(*((long*)(-7 + x_174X))))>>8);
                    fwrite((void *)(-3 + x_174X), sizeof(char), (-1 + v_171X), port_172X);
                    b_170X = Snewspace_beginS;
                    Snewspace_beginS = Soldspace_beginS;
                    Soldspace_beginS = b_170X;
                    e_169X = Snewspace_endS;
                    Snewspace_endS = Soldspace_endS;
                    Soldspace_endS = e_169X;
                    Ssaved_limitS = SlimitS;
                    Ssaved_hpS = ShpS;
                    SlimitS = Snewspace_endS;
                    ShpS = Snewspace_beginS;
                    Sweak_pointer_hpS = -1;
                    if ((3 == (3 & arg2_177X))) {
                      a_248X = -3 + arg2_177X;
                      if ((a_248X < Soldspace_beginS)) {
                        arg1K0 = arg2_177X;
                        goto L29087;}
                      else {
                        v_249X = a_248X < Soldspace_endS;
                        if (v_249X) {
                          merged_arg1K0 = arg2_177X;
                          copy_object_return_tag = 0;
                          goto copy_object;
                         copy_object_return_0:
                          v_168X = copy_object_return_value;
                          arg1K0 = v_168X;
                          goto L29087;}
                        else {
                          arg1K0 = arg2_177X;
                          goto L29087;}}}
                    else {
                      arg1K0 = arg2_177X;
                      goto L29087;}}}
                else {
                  goto L29205;}}
              else {
                goto L29205;}}
            else {
              goto L29103;}}
          else {
            goto L29103;}}
        else {
          goto L29103;}}
      else {
        goto L29103;}}
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
      RSstackS = (4 + RSstackS);
      arg2_242X = *((long*)RSstackS);
      if ((0 == (3 & arg2_242X))) {
        x_181X = RSvalS;
        key_180X = ((arg2_242X)>>2);
        if ((0 == key_180X)) {
          n_178X = (((SlimitS - ShpS))>>2);
          RSvalS = (((n_178X)<<2));
          goto START;}
        else {
          if ((1 == key_180X)) {
            bytes_244X = Snewspace_endS - Snewspace_beginS;
            RSvalS = (-4 & (3 + bytes_244X));
            goto START;}
          else {
            if ((2 == key_180X)) {
              n_179X = Sstack_endS - Sstack_beginS;
              RSvalS = (((n_179X)<<2));
              goto START;}
            else {
              if ((3 == key_180X)) {
                if ((0 == (3 & x_181X))) {
                  old_243X = Sminimum_recovered_spaceS;
                  Sminimum_recovered_spaceS = (((x_181X)>>2));
                  RSvalS = (((old_243X)<<2));
                  goto START;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (((key_180X)<<2));
                  merged_arg1K2 = x_181X;
                  goto raise_exception2;}}
              else {
                if ((4 == key_180X)) {
                  RSvalS = (((Sgc_countS)<<2));
                  goto START;}
                else {
                  merged_arg0K0 = 0;
                  merged_arg1K1 = (((key_180X)<<2));
                  merged_arg1K2 = x_181X;
                  goto raise_exception2;}}}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_242X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 120 : {
      obj_238X = RSvalS;
      if ((3 == (3 & obj_238X))) {
        v_185X = 31 & ((((*((long*)(-7 + obj_238X))))>>2));
        if ((2 == v_185X)) {
          x_239X = RSvalS;
          start_hp_240X = ShpS;
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          v_241X = hoistD0(Snewspace_beginS, ShpS, x_239X);
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          RSenvS = SenvS;
          if (v_241X) {
            areas_184X = Spure_areasS;
            sizes_183X = Spure_sizesS;
            count_182X = Spure_area_countS;
            arg1K0 = 0;
            goto L17489;}
          else {
            goto L17474;}}
        else {
          goto L17421;}}
      else {
        goto L17421;}}
      break;
    case 121 : {
      if ((0 == (3 & RSvalS))) {
        type_235X = ((RSvalS)>>2);
        start_hp_236X = ShpS;
        *((long*)ShpS) = 0;
        ShpS = (4 + ShpS);
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        v_237X = hoistD1(Snewspace_beginS, start_hp_236X, type_235X);
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSenvS = SenvS;
        if (v_237X) {
          areas_188X = Simpure_areasS;
          sizes_187X = Simpure_sizesS;
          count_186X = Simpure_area_countS;
          arg1K0 = 0;
          goto L10169;}
        else {
          goto L10107;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 122 : {
      RSvalS = Sdynamic_stateS;
      goto START;}
      break;
    case 123 : {
      Sdynamic_stateS = RSvalS;
      RSvalS = 13;
      goto START;}
      break;
    case 124 : {
      obj_234X = RSvalS;
      if ((3 == (3 & obj_234X))) {
        v_189X = 31 & ((((*((long*)(-7 + obj_234X))))>>2));
        if ((3 == v_189X)) {
          Sexception_handlerS = RSvalS;
          goto START;}
        else {
          goto L31447;}}
      else {
        goto L31447;}}
      break;
    case 125 : {
      obj_232X = RSvalS;
      if ((3 == (3 & obj_232X))) {
        v_190X = 31 & ((((*((long*)(-7 + obj_232X))))>>2));
        if ((2 == v_190X)) {
          v_233X = (((3 + ((long)(((unsigned long)(*((long*)(-7 + RSvalS))))>>8))))>>2);
          if ((v_233X < 3)) {
            goto L29413;}
          else {
            Sinterrupt_handlersS = RSvalS;
            goto START;}}
        else {
          goto L29413;}}
      else {
        goto L29413;}}
      break;
    case 126 : {
      temp_191X = Senabled_interruptsS;
      Senabled_interruptsS = (((RSvalS)>>2));
      RSvalS = (((temp_191X)<<2));
      goto START;}
      break;
    case 127 : {
      RSstackS = (4 + RSstackS);
      Senabled_interruptsS = ((((*((long*)RSstackS)))>>2));
      RSstackS = (4 + RSstackS);
      SnargsS = ((((*((long*)RSstackS)))>>2));
      RSstackS = (4 + RSstackS);
      RSenvS = (*((long*)RSstackS));
      RSstackS = (4 + RSstackS);
      tem_231X = *((long*)RSstackS);
      StemplateS = tem_231X;
      RScode_pointerS = (-3 + (*((long*)(-3 + tem_231X))));
      RSstackS = (4 + RSstackS);
      RSvalS = (*((long*)RSstackS));
      goto START;}
      break;
    case 128 : {
      if ((0 == (3 & RSvalS))) {
        p_193X = RSvalS;
        Spending_interruptsS = (-2 & Spending_interruptsS);
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        n_192X = ps_schedule_interrupt((((p_193X)>>2)));
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSenvS = SenvS;
        RSvalS = (((n_192X)<<2));
        goto START;}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = RSvalS;
        goto raise_exception1;}}
      break;
    case 129 : {
      obj_226X = RSvalS;
      if ((3 == (3 & obj_226X))) {
        v_196X = 31 & ((((*((long*)(-7 + obj_226X))))>>2));
        if ((12 == v_196X)) {
          x_227X = RSvalS;
          value_228X = *((long*)(1 + x_227X));
          name_229X = *((long*)(-3 + x_227X));
          if ((3 == (3 & name_229X))) {
            v_195X = 31 & ((((*((long*)(-7 + name_229X))))>>2));
            if ((15 == v_195X)) {
              if ((3 == (3 & value_228X))) {
                v_194X = 31 & ((((*((long*)(-7 + value_228X))))>>2));
                if ((16 == v_194X)) {
                  SvalS = RSvalS;
                  SstackS = RSstackS;
                  Scode_pointerS = RScode_pointerS;
                  SenvS = RSenvS;
                  v_230X = lookup_external_name((-3 + name_229X), (-3 + value_228X));
                  RSvalS = SvalS;
                  RSstackS = SstackS;
                  RScode_pointerS = Scode_pointerS;
                  RSenvS = SenvS;
                  if (v_230X) {
                    RSvalS = 13;
                    goto START;}
                  else {
                    goto L17092;}}
                else {
                  goto L17092;}}
              else {
                goto L17092;}}
            else {
              goto L17092;}}
          else {
            goto L17092;}}
        else {
          goto L17095;}}
      else {
        goto L17095;}}
      break;
    case 130 : {
      index_198X = -1 + SnargsS;
      proc_197X = *((long*)(4 + (RSstackS + (((index_198X)<<2)))));
      if ((3 == (3 & proc_197X))) {
        v_225X = 31 & ((((*((long*)(-7 + proc_197X))))>>2));
        if ((12 == v_225X)) {
          SnargsS = (-1 + SnargsS);
          RSvalS = proc_197X;
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          TTreturn_value = 1;
          return(0L);}
        else {
          goto L31502;}}
      else {
        goto L31502;}}
      break;
    case 131 : {
      RSstackS = (4 + RSstackS);
      arg2_222X = *((long*)RSstackS);
      if ((0 == (3 & arg2_222X))) {
        x_201X = RSvalS;
        key_200X = ((arg2_222X)>>2);
        if ((0 == key_200X)) {
          SvalS = RSvalS;
          SstackS = RSstackS;
          Scode_pointerS = RScode_pointerS;
          SenvS = RSenvS;
          n_224X = ps_ticks_per_second();
          RSvalS = SvalS;
          RSstackS = SstackS;
          RScode_pointerS = Scode_pointerS;
          RSenvS = SenvS;
          RSvalS = (((n_224X)<<2));
          goto START;}
        else {
          if ((1 == key_200X)) {
            SvalS = RSvalS;
            SstackS = RSstackS;
            Scode_pointerS = RScode_pointerS;
            SenvS = RSenvS;
            n_199X = ps_run_time();
            RSvalS = SvalS;
            RSstackS = SstackS;
            RScode_pointerS = Scode_pointerS;
            RSenvS = SenvS;
            RSvalS = (((n_199X)<<2));
            goto START;}
          else {
            if ((2 == key_200X)) {
              SvalS = RSvalS;
              SstackS = RSstackS;
              Scode_pointerS = RScode_pointerS;
              SenvS = RSenvS;
              n_223X = ps_real_time();
              RSvalS = SvalS;
              RSstackS = SstackS;
              RScode_pointerS = Scode_pointerS;
              RSenvS = SenvS;
              RSvalS = (((n_223X)<<2));
              goto START;}
            else {
              merged_arg0K0 = 0;
              merged_arg1K1 = (((key_200X)<<2));
              merged_arg1K2 = x_201X;
              goto raise_exception2;}}}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_222X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 132 : {
      RSstackS = (4 + RSstackS);
      arg2_202X = *((long*)RSstackS);
      if ((0 == (3 & arg2_202X))) {
        x_219X = RSvalS;
        key_220X = ((arg2_202X)>>2);
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        return_value_221X = extended_vm(key_220X, x_219X);
        RSvalS = SvalS;
        RSstackS = SstackS;
        RScode_pointerS = Scode_pointerS;
        RSenvS = SenvS;
        if ((17 == (255 & return_value_221X))) {
          merged_arg0K0 = 0;
          merged_arg1K1 = (((key_220X)<<2));
          merged_arg1K2 = x_219X;
          goto raise_exception2;}
        else {
          RSvalS = return_value_221X;
          goto START;}}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_202X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 133 : {
      RSstackS = (4 + RSstackS);
      arg2_203X = *((long*)RSstackS);
      if ((0 == (3 & arg2_203X))) {
        RSvalS = RSvalS;
        SvalS = RSvalS;
        SstackS = RSstackS;
        Scode_pointerS = RScode_pointerS;
        SenvS = RSenvS;
        TTreturn_value = 0;
        return(0L);}
      else {
        merged_arg0K0 = 0;
        merged_arg1K1 = arg2_203X;
        merged_arg1K2 = RSvalS;
        goto raise_exception2;}}
      break;
    case 134 : {
      RSstackS = (4 + RSstackS);
      arg2_206X = *((long*)RSstackS);
      if ((3 == (3 & arg2_206X))) {
        v_217X = 31 & ((((*((long*)(-7 + arg2_206X))))>>2));
        if ((15 == v_217X)) {
          obj_205X = RSvalS;
          if ((3 == (3 & obj_205X))) {
            v_218X = 31 & ((((*((long*)(-7 + obj_205X))))>>2));
            if ((15 == v_218X)) {
              merged_arg1K0 = arg2_206X;
              merged_arg1K1 = RSvalS;
              stob_equalP_return_tag = 0;
              goto stob_equalP;
             stob_equalP_return_0:
              x_204X = stob_equalP_return_value;
              if (x_204X) {
                arg0K0 = 5;
                goto L20484;}
              else {
                arg0K0 = 1;
                goto L20484;}}
            else {
              goto L20475;}}
          else {
            goto L20475;}}
        else {
          goto L20475;}}
      else {
        goto L20475;}}
      break;
    case 135 : {
      obj_214X = RSvalS;
      if ((3 == (3 & obj_214X))) {
        v_207X = 31 & ((((*((long*)(-7 + obj_214X))))>>2));
        if ((15 == v_207X)) {
          x_215X = RSvalS;
          v_216X = (long)(((unsigned long)(*((long*)(-7 + x_215X))))>>8);
          arg1K0 = 0;
          arg1K1 = 0;
          goto L16936;}
        else {
          goto L16914;}}
      else {
        goto L16914;}}
      break;
    case 136 : {
      space_210X = 1 + ((((4 + RSvalS))>>2));
      v_209X = (ShpS + (((space_210X)<<2))) < SlimitS;
      if (v_209X) {
        goto L20198;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 10;
        goto collect_saving_temp;
       collect_saving_temp_return_10:
        okayP_208X = (ShpS + (((space_210X)<<2))) < SlimitS;
        if (okayP_208X) {
          goto L20198;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L20198;}}}
      break;
    case 137 : {
      v_212X = (20 + ShpS) < SlimitS;
      if (v_212X) {
        goto L20072;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 11;
        goto collect_saving_temp;
       collect_saving_temp_return_11:
        okayP_213X = (20 + ShpS) < SlimitS;
        if (okayP_213X) {
          goto L20072;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L20072;}}}
      break;
  }}
 L31416: {
  vector_392X = arg1K0;
  if ((1 == vector_392X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = (((type_235X)<<2));
    goto raise_exception1;}
  else {
    RSvalS = vector_392X;
    goto START;}}
 L10107: {
  ShpS = start_hp_236X;
  arg1K0 = 1;
  goto L31416;}
 L10169: {
  i_393X = arg1K0;
  if ((i_393X < ((long)count_186X))) {
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    SenvS = RSenvS;
    v_394X = hoistD1((*(areas_188X + i_393X)), ((*(areas_188X + i_393X)) + (*(sizes_187X + i_393X))), type_235X);
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    RSenvS = SenvS;
    if (v_394X) {
      arg1K0 = (1 + i_393X);
      goto L10169;}
    else {
      goto L10107;}}
  else {
    areas_395X = Spure_areasS;
    sizes_396X = Spure_sizesS;
    count_397X = Spure_area_countS;
    arg1K0 = 0;
    goto L10148;}}
 L10148: {
  i_398X = arg1K0;
  if ((i_398X < ((long)count_397X))) {
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    SenvS = RSenvS;
    v_399X = hoistD1((*(areas_395X + i_398X)), ((*(areas_395X + i_398X)) + (*(sizes_396X + i_398X))), type_235X);
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    RSenvS = SenvS;
    if (v_399X) {
      arg1K0 = (1 + i_398X);
      goto L10148;}
    else {
      goto L10107;}}
  else {
    *((long*)start_hp_236X) = (-1014 + ((((ShpS - start_hp_236X))<<8)));
    arg1K0 = (7 + start_hp_236X);
    goto L31416;}}
 L8863: {
  h_400X = arg1K0;
  arg1K0 = (4 + (addr_401X + (-4 & (3 + ((long)(((unsigned long)h_400X)>>8))))));
  goto L8841;}
 L8841: {
  addr_401X = arg1K0;
  if ((addr_401X < ShpS)) {
    d_403X = *((long*)addr_401X);
    if ((2 == (3 & d_403X))) {
      arg1K0 = d_403X;
      goto L8863;}
    else {
      h_402X = *((long*)(-7 + d_403X));
      *((long*)addr_401X) = h_402X;
      arg1K0 = h_402X;
      goto L8863;}}
  else {
    RSvalS = (((size_448X)<<2));
    goto START;}}
 L12987: {
  new_404X = arg1K0;
  *((long*)ShpS) = 2050;
  ShpS = (4 + ShpS);
  new_405X = 3 + ShpS;
  ShpS = (8 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)(-3 + new_405X)) = new_404X;
  *((long*)(1 + new_405X)) = bucket_409X;
  *((long*)((-3 + table_423X) + (((index_410X)<<2)))) = new_405X;
  arg1K0 = new_404X;
  goto L20094;}
 L20094: {
  val_406X = arg1K0;
  RSvalS = val_406X;
  goto START;}
 L13077: {
  i_407X = arg1K0;
  h_408X = arg1K1;
  if ((i_407X < (-1 + v_425X))) {
    v_411X = ((long)(*((unsigned char*)((-3 + arg2_421X) + i_407X))));
    arg1K0 = (1 + i_407X);
    arg1K1 = (h_408X + ((long)(v_411X)));
    goto L13077;}
  else {
    index_410X = h_408X & (-1 + v_424X);
    bucket_409X = *((long*)((-3 + table_423X) + (((index_410X)<<2))));
    arg1K0 = bucket_409X;
    goto L12979;}}
 L12979: {
  b_412X = arg1K0;
  if ((25 == b_412X)) {
    *((long*)ShpS) = 1030;
    ShpS = (4 + ShpS);
    new_414X = 3 + ShpS;
    ShpS = (4 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_414X)) = arg2_421X;
    if ((3 == (3 & new_414X))) {
      if ((0 == (128 & (*((long*)(-7 + new_414X)))))) {
        *((long*)(-7 + new_414X)) = (128 | (*((long*)(-7 + new_414X))));
        arg1K0 = new_414X;
        goto L12987;}
      else {
        arg1K0 = new_414X;
        goto L12987;}}
    else {
      arg1K0 = new_414X;
      goto L12987;}}
  else {
    merged_arg1K0 = arg2_421X;
    merged_arg1K1 = (*((long*)(-3 + (*((long*)(-3 + b_412X))))));
    stob_equalP_return_tag = 1;
    goto stob_equalP;
   stob_equalP_return_1:
    v_413X = stob_equalP_return_value;
    if (v_413X) {
      arg1K0 = (*((long*)(-3 + b_412X)));
      goto L20094;}
    else {
      arg1K0 = (*((long*)(1 + b_412X)));
      goto L12979;}}}
 L9834: {
  stob_415X = arg1K0;
  from_416X = 4 + RSstackS;
  arg1K0 = from_416X;
  arg1K1 = (1 + stob_415X);
  goto L9873;}
 L9873: {
  from_417X = arg1K0;
  to_418X = arg1K1;
  if ((from_417X < (from_416X + (((((long)b_577X))<<2))))) {
    *((long*)to_418X) = (*((long*)from_417X));
    arg1K0 = (4 + from_417X);
    arg1K1 = (4 + to_418X);
    goto L9873;}
  else {
    RSstackS = (RSstackS - ((((0 - ((long)b_577X)))<<2)));
    *((long*)(-3 + stob_415X)) = RSenvS;
    RSenvS = stob_415X;
    goto START;}}
 L20072: {
  RSstackS = (4 + RSstackS);
  arg2_421X = *((long*)RSstackS);
  if ((3 == (3 & arg2_421X))) {
    v_420X = 31 & ((((*((long*)(-7 + arg2_421X))))>>2));
    if ((15 == v_420X)) {
      obj_422X = RSvalS;
      if ((3 == (3 & obj_422X))) {
        v_419X = 31 & ((((*((long*)(-7 + obj_422X))))>>2));
        if ((2 == v_419X)) {
          table_423X = RSvalS;
          v_424X = (((3 + ((long)(((unsigned long)(*((long*)(-7 + table_423X))))>>8))))>>2);
          v_425X = (long)(((unsigned long)(*((long*)(-7 + arg2_421X))))>>8);
          arg1K0 = 0;
          arg1K1 = 0;
          goto L13077;}
        else {
          goto L20097;}}
      else {
        goto L20097;}}
    else {
      goto L20097;}}
  else {
    goto L20097;}}
 L20097: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_421X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L20198: {
  RSstackS = (4 + RSstackS);
  arg2_428X = *((long*)RSstackS);
  v_427X = 0 == (3 & RSvalS);
  if (v_427X) {
    n_429X = ((RSvalS)>>2);
    if ((3 == (3 & arg2_428X))) {
      v_426X = 31 & ((((*((long*)(-7 + arg2_428X))))>>2));
      if ((0 == v_426X)) {
        goto L20324;}
      else {
        goto L20226;}}
    else {
      goto L20226;}}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_428X;
    merged_arg1K2 = RSvalS;
    goto raise_exception2;}}
 L20226: {
  if ((25 == arg2_428X)) {
    goto L20324;}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_428X;
    merged_arg1K2 = (((n_429X)<<2));
    goto raise_exception2;}}
 L20324: {
  len_431X = 1 + n_429X;
  *((long*)ShpS) = (62 + (((len_431X)<<8)));
  ShpS = (4 + ShpS);
  new_430X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_431X)));
  if ((0 < len_431X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_430X;
    goto L20243;}
  else {
    arg1K0 = new_430X;
    goto L20243;}}
 L20243: {
  obj_432X = arg1K0;
  arg1K0 = arg2_428X;
  arg1K1 = (-1 + n_429X);
  goto L20245;}
 L20245: {
  l_433X = arg1K0;
  i_434X = arg1K1;
  if ((i_434X < 0)) {
    RSvalS = obj_432X;
    goto START;}
  else {
    *((unsigned char*)((-3 + obj_432X) + i_434X)) = ((unsigned char)((long)((((((*((long*)(-3 + l_433X))))>>8))))));
    arg1K0 = (*((long*)(1 + l_433X)));
    arg1K1 = (-1 + i_434X);
    goto L20245;}}
 L16914: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L16936: {
  i_435X = arg1K0;
  h_436X = arg1K1;
  if ((i_435X < (-1 + v_216X))) {
    v_437X = ((long)(*((unsigned char*)((-3 + x_215X) + i_435X))));
    arg1K0 = (1 + i_435X);
    arg1K1 = (h_436X + ((long)(v_437X)));
    goto L16936;}
  else {
    RSvalS = (((h_436X)<<2));
    goto START;}}
 L20475: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_206X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L20484: {
  v_438X = arg0K0;
  RSvalS = ((long)v_438X);
  goto START;}
 L17095: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17092: {
  merged_arg0K0 = 0;
  merged_arg1K1 = x_227X;
  goto raise_exception1;}
 L29413: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17421: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17489: {
  i_439X = arg1K0;
  if ((i_439X < ((long)count_182X))) {
    SvalS = RSvalS;
    SstackS = RSstackS;
    Scode_pointerS = RScode_pointerS;
    SenvS = RSenvS;
    v_440X = hoistD0((*(areas_184X + i_439X)), ((*(areas_184X + i_439X)) + (*(sizes_183X + i_439X))), x_239X);
    RSvalS = SvalS;
    RSstackS = SstackS;
    RScode_pointerS = Scode_pointerS;
    RSenvS = SenvS;
    if (v_440X) {
      arg1K0 = (1 + i_439X);
      goto L17489;}
    else {
      goto L17474;}}
  else {
    RSvalS = 13;
    goto START;}}
 L17474: {
  ShpS = start_hp_240X;
  merged_arg0K0 = 0;
  goto raise_exception;}
 L29103: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_176X;
  merged_arg1K2 = arg2_177X;
  merged_arg1K3 = RSvalS;
  goto raise_exception3;}
 L29205: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_176X;
  merged_arg1K2 = arg2_177X;
  goto raise_exception2;}
 L29087: {
  resume_proc_441X = arg1K0;
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
  goto L29129;}
 L29140: {
  new_442X = arg1K0;
  *(Sopen_vm_portsS + i_443X) = new_442X;
  goto L29133;}
 L29133: {
  arg1K0 = (1 + i_443X);
  goto L29129;}
 L29129: {
  i_443X = arg1K0;
  if ((i_443X == Snumber_of_portsS)) {
    fputs("This is a Scheme48 heap image file.", port_172X);
    putc(10, port_172X);
    putc(12, port_172X);
    putc(10, port_172X);
    merged_arg1K0 = 15;
    merged_arg2K1 = port_172X;
    write_number_return_tag = 0;
    goto write_number;
   write_number_return_0:
    merged_arg1K0 = 4;
    merged_arg2K1 = port_172X;
    write_number_return_tag = 1;
    goto write_number;
   write_number_return_1:
    merged_arg1K0 = (((Snewspace_beginS)>>2));
    merged_arg2K1 = port_172X;
    write_number_return_tag = 2;
    goto write_number;
   write_number_return_2:
    merged_arg1K0 = (((ShpS)>>2));
    merged_arg2K1 = port_172X;
    write_number_return_tag = 3;
    goto write_number;
   write_number_return_3:
    merged_arg1K0 = resume_proc_441X;
    merged_arg2K1 = port_172X;
    write_number_return_tag = 4;
    goto write_number;
   write_number_return_4:
    putc(12, port_172X);
    *((long*)ShpS) = 1;
    count_446X = (4 + ShpS) - ShpS;
    fwrite((void *)ShpS, sizeof(char), count_446X, port_172X);
    count_447X = ShpS - Snewspace_beginS;
    fwrite((void *)Snewspace_beginS, sizeof(char), count_447X, port_172X);
    size_448X = ShpS - Snewspace_beginS;
    fclose(port_172X);
    b_449X = Snewspace_beginS;
    Snewspace_beginS = Soldspace_beginS;
    Soldspace_beginS = b_449X;
    e_450X = Snewspace_endS;
    Snewspace_endS = Soldspace_endS;
    Soldspace_endS = e_450X;
    SlimitS = Ssaved_limitS;
    ShpS = Ssaved_hpS;
    arg1K0 = Snewspace_beginS;
    goto L8841;}
  else {
    port_445X = *(Sopen_vm_portsS + i_443X);
    if ((1 == port_445X)) {
      goto L29133;}
    else {
      header_444X = *((long*)(-7 + port_445X));
      if ((3 == (3 & header_444X))) {
        arg1K0 = header_444X;
        goto L29140;}
      else {
        merged_arg1K0 = port_445X;
        close_port_noisily_return_tag = 0;
        goto close_port_noisily;
       close_port_noisily_return_0:
        arg1K0 = 1;
        goto L29140;}}}}
 L17599: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L21094: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_165X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L21238: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_256X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L17695: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17692: {
  v_451X = arg0K0;
  RSvalS = ((long)v_451X);
  goto START;}
 L17852: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17849: {
  val_452X = arg1K0;
  RSvalS = val_452X;
  goto START;}
 L17844: {
  c_453X = arg1K0;
  *((long*)(5 + x_264X)) = c_453X;
  arg1K0 = c_453X;
  goto L17849;}
 L17974: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L17971: {
  val_454X = arg1K0;
  RSvalS = val_454X;
  goto START;}
 L21333: {
  key_455X = arg1K0;
  RSstackS = (4 + RSstackS);
  arg2_457X = *((long*)RSstackS);
  if ((3 == (3 & arg2_457X))) {
    v_458X = 31 & ((((*((long*)(-7 + arg2_457X))))>>2));
    if ((15 == v_458X)) {
      v_456X = 0 == (3 & RSvalS);
      if (v_456X) {
        mode_459X = ((RSvalS)>>2);
        arg1K0 = 0;
        goto L21522;}
      else {
        goto L21371;}}
    else {
      goto L21371;}}
  else {
    goto L21371;}}
 L21371: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_457X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L21522: {
  i_460X = arg1K0;
  if ((i_460X < Snumber_of_portsS)) {
    x_461X = *(Sopen_vm_portsS + i_460X);
    if ((1 == x_461X)) {
      arg1K0 = i_460X;
      arg1K1 = arg2_457X;
      goto L21407;}
    else {
      arg1K0 = (1 + i_460X);
      goto L21522;}}
  else {
    arg1K0 = -1;
    arg1K1 = arg2_457X;
    goto L21407;}}
 L21509: {
  i_462X = arg1K0;
  if ((i_462X < Snumber_of_portsS)) {
    x_463X = *(Sopen_vm_portsS + i_462X);
    if ((1 == x_463X)) {
      arg1K0 = i_462X;
      goto L21451;}
    else {
      arg1K0 = (1 + i_462X);
      goto L21509;}}
  else {
    arg1K0 = -1;
    goto L21451;}}
 L21479: {
  i_464X = arg1K0;
  if ((i_464X < Snumber_of_portsS)) {
    *(new_ports_469X + i_464X) = (*(Sopen_portsS + i_464X));
    *(new_vm_ports_470X + i_464X) = (*(Sopen_vm_portsS + i_464X));
    arg1K0 = (1 + i_464X);
    goto L21479;}
  else {
    free(Sopen_portsS);
    free(Sopen_vm_portsS);
    Sopen_portsS = new_ports_469X;
    Sopen_vm_portsS = new_vm_ports_470X;
    Snumber_of_portsS = new_count_467X;
    arg1K0 = old_count_468X;
    arg1K1 = filename_477X;
    goto L21407;}}
 L21473: {
  i_465X = arg1K0;
  if ((i_465X < new_count_467X)) {
    *(new_vm_ports_470X + i_465X) = 1;
    arg1K0 = (1 + i_465X);
    goto L21473;}
  else {
    arg1K0 = 0;
    goto L21479;}}
 L21462: {
  merged_arg0K0 = 0;
  merged_arg1K1 = filename_477X;
  merged_arg1K2 = (((key_455X)<<2));
  goto raise_exception2;}
 L21451: {
  index_466X = arg1K0;
  if ((index_466X < 0)) {
    new_count_467X = 8 + Snumber_of_portsS;
    old_count_468X = Snumber_of_portsS;
    new_ports_469X = (FILE **)malloc(sizeof(FILE *) * new_count_467X);
    new_vm_ports_470X = (long*)malloc(sizeof(long) * new_count_467X);
    if ((NULL == new_ports_469X)) {
      goto L21462;}
    else {
      if ((NULL == new_vm_ports_470X)) {
        goto L21462;}
      else {
        arg1K0 = 0;
        goto L21473;}}}
  else {
    arg1K0 = index_466X;
    arg1K1 = filename_477X;
    goto L21407;}}
 L21410: {
  port_471X = arg2K0;
  if ((NULL == port_471X)) {
    RSvalS = 1;
    goto START;}
  else {
    *((long*)ShpS) = 4118;
    ShpS = (4 + ShpS);
    new_472X = 3 + ShpS;
    ShpS = (16 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_472X)) = (((mode_459X)<<2));
    *((long*)(1 + new_472X)) = (((index_473X)<<2));
    *((long*)(5 + new_472X)) = 1;
    *((long*)(9 + new_472X)) = filename_474X;
    *(Sopen_portsS + index_473X) = port_471X;
    *(Sopen_vm_portsS + index_473X) = new_472X;
    RSvalS = new_472X;
    goto START;}}
 L21407: {
  index_473X = arg1K0;
  filename_474X = arg1K1;
  if ((index_473X < 0)) {
    merged_arg1K0 = filename_474X;
    collect_saving_temp_return_tag = 12;
    goto collect_saving_temp;
   collect_saving_temp_return_12:
    filename_477X = collect_saving_temp_return_value;
    arg1K0 = 0;
    goto L21509;}
  else {
    if ((2 == mode_459X)) {
      merged_arg1K0 = filename_474X;
      extract_string_return_tag = 1;
      goto extract_string;
     extract_string_return_1:
      spec_475X = extract_string_return_value;
      arg2K0 = (ps_open(spec_475X, "w"));
      goto L21410;}
    else {
      merged_arg1K0 = filename_474X;
      extract_string_return_tag = 2;
      goto extract_string;
     extract_string_return_2:
      spec_476X = extract_string_return_value;
      arg2K0 = (ps_open(spec_476X, "r"));
      goto L21410;}}}
 L21625: {
  if ((1 == RSvalS)) {
    *((long*)(-3 + arg2_144X)) = 273;
    goto L21650;}
  else {
    if ((17 == (255 & (*((long*)(-3 + arg2_144X)))))) {
      *((long*)(-3 + arg2_144X)) = 529;
      goto L21650;}
    else {
      goto L21650;}}}
 L21653: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_144X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L21650: {
  RSvalS = 13;
  goto START;}
 L18407: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L18416: {
  v_478X = arg0K0;
  RSvalS = ((long)v_478X);
  goto START;}
 L24776: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_139X;
  merged_arg1K2 = arg2_140X;
  merged_arg1K3 = RSvalS;
  goto raise_exception3;}
 L24769: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_139X;
  merged_arg1K2 = (((index_137X)<<2));
  merged_arg1K3 = (9 + (((((long)(Kchar_138X)))<<8)));
  goto raise_exception3;}
 L21832: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_282X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L21827: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_282X;
  merged_arg1K2 = (((index_134X)<<2));
  goto raise_exception2;}
 L22070: {
  len_480X = 1 + len_129X;
  *((long*)ShpS) = (62 + (((len_480X)<<8)));
  ShpS = (4 + ShpS);
  new_479X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_480X)));
  if ((0 < len_480X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_479X;
    goto L21994;}
  else {
    arg1K0 = new_479X;
    goto L21994;}}
 L22019: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_131X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L22119: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (((len_129X)<<2));
  merged_arg1K2 = (9 + (((((long)(init_130X)))<<8)));
  goto raise_exception2;}
 L21994: {
  string_481X = arg1K0;
  arg1K0 = (-1 + len_129X);
  goto L21996;}
 L21996: {
  i_482X = arg1K0;
  if ((i_482X < 0)) {
    RSvalS = string_481X;
    goto START;}
  else {
    *((unsigned char*)((-3 + string_481X) + i_482X)) = ((unsigned char)((long)(init_130X)));
    arg1K0 = (-1 + i_482X);
    goto L21996;}}
 L24954: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_289X;
  merged_arg1K2 = arg2_288X;
  merged_arg1K3 = RSvalS;
  goto raise_exception3;}
 L24947: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg3_289X;
  merged_arg1K2 = (((index_291X)<<2));
  merged_arg1K3 = (((value_290X)<<2));
  goto raise_exception3;}
 L22277: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_123X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L22272: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_123X;
  merged_arg1K2 = (((index_294X)<<2));
  goto raise_exception2;}
 L22511: {
  *((long*)ShpS) = (66 + (((len_299X)<<8)));
  ShpS = (4 + ShpS);
  new_483X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_299X)));
  if ((0 < len_299X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_483X;
    goto L22437;}
  else {
    arg1K0 = new_483X;
    goto L22437;}}
 L22462: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_297X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L22560: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (((len_299X)<<2));
  merged_arg1K2 = (((init_298X)<<2));
  goto raise_exception2;}
 L22437: {
  code_vector_484X = arg1K0;
  arg1K0 = (-1 + len_299X);
  goto L22439;}
 L22439: {
  i_485X = arg1K0;
  if ((i_485X < 0)) {
    RSvalS = code_vector_484X;
    goto START;}
  else {
    *((unsigned char*)((-3 + code_vector_484X) + i_485X)) = ((unsigned char)init_298X);
    arg1K0 = (-1 + i_485X);
    goto L22439;}}
 L25786: {
  merged_arg0K0 = 1;
  merged_arg1K1 = arg3_117X;
  merged_arg1K2 = (((((long)b_305X))<<2));
  merged_arg1K3 = (((index_304X)<<2));
  merged_arg1K4 = x_303X;
  goto raise_exception4;}
 L25123: {
  merged_arg0K0 = 1;
  merged_arg1K1 = arg2_115X;
  merged_arg1K2 = (((((long)b_308X))<<2));
  merged_arg1K3 = (((index_307X)<<2));
  goto raise_exception3;}
 L25432: {
  merged_arg0K0 = 1;
  merged_arg1K1 = (((((long)b_312X))<<2));
  merged_arg1K2 = (((len_311X)<<2));
  merged_arg1K3 = x_310X;
  goto raise_exception3;}
 L25313: {
  val_486X = arg1K0;
  arg1K0 = (-1 + len_311X);
  goto L25315;}
 L25315: {
  i_487X = arg1K0;
  if ((i_487X < 0)) {
    RSvalS = val_486X;
    goto START;}
  else {
    *((long*)((-3 + val_486X) + (((i_487X)<<2)))) = init_489X;
    arg1K0 = (-1 + i_487X);
    goto L25315;}}
 L25302: {
  okayP_488X = arg3K0;
  init_489X = arg1K1;
  if (okayP_488X) {
    len_491X = ((len_311X)<<2);
    *((long*)ShpS) = (2 + (((((((len_491X)<<6)) + ((long)b_312X)))<<2)));
    ShpS = (4 + ShpS);
    new_490X = 3 + ShpS;
    ShpS = (ShpS + (-4 & (3 + len_491X)));
    if ((0 < len_491X)) {
      *((long*)(-4 + ShpS)) = 0;
      arg1K0 = new_490X;
      goto L25313;}
    else {
      arg1K0 = new_490X;
      goto L25313;}}
  else {
    merged_arg0K0 = 1;
    merged_arg1K1 = (((((long)b_312X))<<2));
    merged_arg1K2 = (((len_311X)<<2));
    merged_arg1K3 = init_489X;
    goto raise_exception3;}}
 L26044: {
  merged_arg0K0 = 2;
  merged_arg1K1 = arg2_317X;
  merged_arg1K2 = (((((long)b_319X))<<2));
  merged_arg1K3 = (((((long)b_320X))<<2));
  merged_arg1K4 = x_318X;
  goto raise_exception4;}
 L25565: {
  merged_arg0K0 = 2;
  merged_arg1K1 = x_110X;
  merged_arg1K2 = (((((long)b_109X))<<2));
  merged_arg1K3 = (((((long)b_108X))<<2));
  goto raise_exception3;}
 L16739: {
  b_495X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  b_494X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  len_493X = ((((long)b_495X))<<2);
  *((long*)ShpS) = (2 + (((((((len_493X)<<6)) + ((long)b_494X)))<<2)));
  ShpS = (4 + ShpS);
  new_492X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_493X)));
  if ((0 < len_493X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_492X;
    goto L16747;}
  else {
    arg1K0 = new_492X;
    goto L16747;}}
 L16747: {
  new_496X = arg1K0;
  if ((((long)b_495X) < 1)) {
    goto L16778;}
  else {
    *((long*)(-7 + (new_496X + (((((long)b_495X))<<2))))) = RSvalS;
    arg1K0 = (-2 + ((long)b_495X));
    goto L16756;}}
 L16778: {
  RSvalS = new_496X;
  goto START;}
 L16756: {
  i_497X = arg1K0;
  if ((i_497X < 0)) {
    goto L16778;}
  else {
    RSstackS = (4 + RSstackS);
    *((long*)((-3 + new_496X) + (((i_497X)<<2)))) = (*((long*)RSstackS));
    arg1K0 = (-1 + i_497X);
    goto L16756;}}
 L22702: {
  merged_arg0K0 = 1;
  merged_arg1K1 = x_107X;
  merged_arg1K2 = (((((long)b_106X))<<2));
  goto raise_exception2;}
 L22793: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_102X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L22802: {
  v_498X = arg0K0;
  RSvalS = ((long)v_498X);
  goto START;}
 L22877: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_100X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L22886: {
  v_499X = arg0K0;
  RSvalS = ((long)v_499X);
  goto START;}
 L22961: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_97X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L22991: {
  RSvalS = (((result_92X)<<2));
  goto START;}
 L22988: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_97X;
  merged_arg1K2 = x_96X;
  goto raise_exception2;}
 L23516: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_83X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L23548: {
  n_500X = arg1K0;
  RSvalS = (((n_500X)<<2));
  goto START;}
 L23544: {
  b_501X = arg1K0;
  c_502X = a_503X % b_501X;
  if ((a_81X < 0)) {
    arg1K0 = (0 - c_502X);
    goto L23548;}
  else {
    arg1K0 = c_502X;
    goto L23548;}}
 L23542: {
  a_503X = arg1K0;
  if ((b_80X < 0)) {
    arg1K0 = (0 - b_80X);
    goto L23544;}
  else {
    arg1K0 = b_80X;
    goto L23544;}}
 L23647: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_341X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L23681: {
  if ((536870911 < c_505X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_341X;
    merged_arg1K2 = x_342X;
    goto raise_exception2;}
  else {
    RSvalS = (((c_505X)<<2));
    goto START;}}
 L23678: {
  RSvalS = ((((0 - c_505X))<<2));
  goto START;}
 L23675: {
  b_504X = arg1K0;
  c_505X = a_506X / b_504X;
  if ((a_343X < 0)) {
    if ((b_344X < 0)) {
      goto L23681;}
    else {
      goto L23678;}}
  else {
    if ((b_344X < 0)) {
      goto L23678;}
    else {
      goto L23681;}}}
 L23673: {
  a_506X = arg1K0;
  if ((b_344X < 0)) {
    arg1K0 = (0 - b_344X);
    goto L23675;}
  else {
    arg1K0 = b_344X;
    goto L23675;}}
 L23814: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_78X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L23823: {
  v_507X = arg0K0;
  RSvalS = ((long)v_507X);
  goto START;}
 L23898: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_76X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L23907: {
  v_508X = arg0K0;
  RSvalS = ((long)v_508X);
  goto START;}
 L23982: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_74X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L24051: {
  if ((536870911 < c_517X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_74X;
    merged_arg1K2 = x_73X;
    goto raise_exception2;}
  else {
    arg1K0 = (((c_517X)<<2));
    goto L24014;}}
 L24027: {
  n_509X = arg1K0;
  if ((0 == (((n_509X)<<2)))) {
    RSvalS = q_513X;
    goto START;}
  else {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_74X;
    merged_arg1K2 = x_73X;
    goto raise_exception2;}}
 L24023: {
  b_510X = arg1K0;
  c_511X = a_512X % b_510X;
  if ((a_514X < 0)) {
    arg1K0 = (0 - c_511X);
    goto L24027;}
  else {
    arg1K0 = c_511X;
    goto L24027;}}
 L24021: {
  a_512X = arg1K0;
  if ((b_515X < 0)) {
    arg1K0 = (0 - b_515X);
    goto L24023;}
  else {
    arg1K0 = b_515X;
    goto L24023;}}
 L24014: {
  q_513X = arg1K0;
  if ((0 == x_73X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_74X;
    merged_arg1K2 = x_73X;
    goto raise_exception2;}
  else {
    a_514X = ((arg2_74X)>>2);
    b_515X = ((x_73X)>>2);
    if ((a_514X < 0)) {
      arg1K0 = (0 - a_514X);
      goto L24021;}
    else {
      arg1K0 = a_514X;
      goto L24021;}}}
 L24010: {
  b_516X = arg1K0;
  c_517X = a_518X / b_516X;
  if ((a_72X < 0)) {
    if ((b_71X < 0)) {
      goto L24051;}
    else {
      arg1K0 = ((((0 - c_517X))<<2));
      goto L24014;}}
  else {
    if ((b_71X < 0)) {
      arg1K0 = ((((0 - c_517X))<<2));
      goto L24014;}
    else {
      goto L24051;}}}
 L24008: {
  a_518X = arg1K0;
  if ((b_71X < 0)) {
    arg1K0 = (0 - b_71X);
    goto L24010;}
  else {
    arg1K0 = b_71X;
    goto L24010;}}
 L24192: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_348X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L24214: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_348X;
  merged_arg1K2 = x_349X;
  goto raise_exception2;}
 L24306: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_69X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L24363: {
  if ((536870911 < c_526X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_69X;
    merged_arg1K2 = x_68X;
    goto raise_exception2;}
  else {
    RSvalS = (((c_526X)<<2));
    goto START;}}
 L24356: {
  if ((536870912 < c_526X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = arg2_69X;
    merged_arg1K2 = x_68X;
    goto raise_exception2;}
  else {
    RSvalS = ((((0 - c_526X))<<2));
    goto START;}}
 L24353: {
  if ((536870911 < lo_c_524X)) {
    goto L24348;}
  else {
    if ((8192 < mid_c_525X)) {
      goto L24348;}
    else {
      if ((a_67X < 0)) {
        if ((b_66X < 0)) {
          goto L24363;}
        else {
          goto L24356;}}
      else {
        if ((b_66X < 0)) {
          goto L24356;}
        else {
          goto L24363;}}}}}
 L24348: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_69X;
  merged_arg1K2 = x_68X;
  goto raise_exception2;}
 L24333: {
  b_519X = arg1K0;
  lo_a_520X = 65535 & a_527X;
  lo_b_521X = 65535 & b_519X;
  hi_a_522X = 65535 & (((a_527X)>>16));
  hi_b_523X = 65535 & (((b_519X)>>16));
  lo_c_524X = PS_SMALL_MULTIPLY(lo_a_520X, lo_b_521X);
  mid_c_525X = PS_SMALL_MULTIPLY(lo_a_520X, hi_b_523X)
	     + PS_SMALL_MULTIPLY(lo_b_521X, hi_a_522X);
  c_526X = lo_c_524X + (((mid_c_525X)<<16));
  if ((0 < hi_a_522X)) {
    if ((0 < hi_b_523X)) {
      goto L24348;}
    else {
      goto L24353;}}
  else {
    goto L24353;}}
 L24331: {
  a_527X = arg1K0;
  if ((b_66X < 0)) {
    arg1K0 = (0 - b_66X);
    goto L24333;}
  else {
    arg1K0 = b_66X;
    goto L24333;}}
 L24513: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_65X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L24535: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_65X;
  merged_arg1K2 = x_64X;
  goto raise_exception2;}
 L28917: {
  offset_528X = arg1K0;
  RScode_pointerS = (RScode_pointerS + offset_528X);
  goto START;}
 L25652: {
  RSvalS = proc_43X;
  merged_arg0K0 = 0;
  goto perform_application;}
 L15228: {
  RSstackS = (4 + RSstackS);
  list_529X = *((long*)RSstackS);
  a_530X = *((long*)(-3 + list_529X));
  *((long*)ShpS) = 2050;
  ShpS = (4 + ShpS);
  new_531X = 3 + ShpS;
  ShpS = (8 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)(-3 + new_531X)) = a_530X;
  *((long*)(1 + new_531X)) = 25;
  arg1K0 = (*((long*)(1 + list_529X)));
  arg1K1 = new_531X;
  goto L15238;}
 L15238: {
  l_532X = arg1K0;
  last_533X = arg1K1;
  if ((25 == l_532X)) {
    *((long*)RSstackS) = new_531X;
    RSstackS = (-4 + RSstackS);
    SnargsS = 64;
    goto L25652;}
  else {
    a_535X = *((long*)(-3 + l_532X));
    *((long*)ShpS) = 2050;
    ShpS = (4 + ShpS);
    new_534X = 3 + ShpS;
    ShpS = (8 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_534X)) = a_535X;
    *((long*)(1 + new_534X)) = 25;
    *((long*)(1 + last_533X)) = new_534X;
    arg1K0 = (*((long*)(1 + l_532X)));
    arg1K1 = new_534X;
    goto L15238;}}
 L15458: {
  i_536X = arg1K0;
  l_537X = arg1K1;
  if ((0 < i_536X)) {
    *((long*)RSstackS) = (*((long*)(-3 + l_537X)));
    RSstackS = (-4 + RSstackS);
    arg1K0 = (-1 + i_536X);
    arg1K1 = (*((long*)(1 + l_537X)));
    goto L15458;}
  else {
    if ((0 == overflow_541X)) {
      SnargsS = nargs_547X;
      goto L25652;}
    else {
      *((long*)RSstackS) = l_537X;
      RSstackS = (-4 + RSstackS);
      space_538X = 3 * overflow_541X;
      v_539X = (ShpS + (((space_538X)<<2))) < SlimitS;
      if (v_539X) {
        goto L15228;}
      else {
        merged_arg1K0 = 0;
        collect_saving_temp_return_tag = 13;
        goto collect_saving_temp;
       collect_saving_temp_return_13:
        okayP_540X = (ShpS + (((space_538X)<<2))) < SlimitS;
        if (okayP_540X) {
          goto L15228;}
        else {
          TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
          goto L15228;}}}}}
 L15422: {
  overflow_541X = arg1K0;
  arg1K0 = (len_544X - overflow_541X);
  arg1K1 = args_548X;
  goto L15458;}
 L25686: {
  merged_arg1K0 = 25;
  merged_arg1K1 = ((long)(*((unsigned char*)RScode_pointerS)));
  pop_args_list_return_tag = 2;
  goto pop_args_list;
 pop_args_list_return_2:
  args_542X = pop_args_list_return_value;
  merged_arg0K0 = 0;
  merged_arg1K1 = proc_43X;
  merged_arg1K2 = args_542X;
  merged_arg1K3 = RSvalS;
  goto raise_exception3;}
 L25684: {
  fast_543X = arg1K0;
  len_544X = arg1K1;
  slow_545X = arg1K2;
  move_slowP_546X = arg3K3;
  if ((25 == fast_543X)) {
    args_548X = RSvalS;
    nargs_547X = len_544X + ((long)(*((unsigned char*)RScode_pointerS)));
    if ((63 < nargs_547X)) {
      arg1K0 = (-63 + nargs_547X);
      goto L15422;}
    else {
      arg1K0 = 0;
      goto L15422;}}
  else {
    if ((3 == (3 & fast_543X))) {
      v_549X = 31 & ((((*((long*)(-7 + fast_543X))))>>2));
      if ((0 == v_549X)) {
        if (move_slowP_546X) {
          if ((fast_543X == slow_545X)) {
            merged_arg1K0 = 25;
            merged_arg1K1 = ((long)(*((unsigned char*)RScode_pointerS)));
            pop_args_list_return_tag = 3;
            goto pop_args_list;
           pop_args_list_return_3:
            args_550X = pop_args_list_return_value;
            merged_arg0K0 = 0;
            merged_arg1K1 = proc_43X;
            merged_arg1K2 = args_550X;
            merged_arg1K3 = RSvalS;
            goto raise_exception3;}
          else {
            arg1K0 = (*((long*)(1 + fast_543X)));
            arg1K1 = (1 + len_544X);
            arg1K2 = (*((long*)(1 + slow_545X)));
            arg3K3 = 0;
            goto L25684;}}
        else {
          arg1K0 = (*((long*)(1 + fast_543X)));
          arg1K1 = (1 + len_544X);
          arg1K2 = slow_545X;
          arg3K3 = 1;
          goto L25684;}}
      else {
        goto L25686;}}
    else {
      goto L25686;}}}
 L15998: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L16015: {
  loc_551X = arg1K0;
  arg_552X = arg1K1;
  if ((RSstackS < arg_552X)) {
    *((long*)loc_551X) = (*((long*)arg_552X));
    arg1K0 = (-4 + loc_551X);
    arg1K1 = (-4 + arg_552X);
    goto L16015;}
  else {
    RSstackS = loc_551X;
    goto L15998;}}
 L28710: {
  v_553X = arg1K0;
  ScontS = v_553X;
  goto START;}
 L28580: {
  key_554X = arg1K0;
  merged_arg1K0 = (RScode_pointerS + ((((((long)b_367X))<<8)) + ((long)b_368X)));
  merged_arg1K1 = ((long)b_369X);
  merged_arg1K2 = key_554X;
  push_continuationB_return_tag = 0;
  goto push_continuationB;
 push_continuationB_return_0:
  goto START;}
 L28601: {
  v_555X = arg1K0;
  merged_arg1K0 = v_555X;
  copy_stack_into_heap_return_tag = 0;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_0:
  arg1K0 = 0;
  goto L28580;}
 L28402: {
  space_556X = arg1K0;
  v_557X = (ShpS + (((space_556X)<<2))) < SlimitS;
  if (v_557X) {
    arg1K0 = 0;
    goto L28404;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 14;
    goto collect_saving_temp;
   collect_saving_temp_return_14:
    okayP_558X = (ShpS + (((space_556X)<<2))) < SlimitS;
    if (okayP_558X) {
      arg1K0 = 0;
      goto L28404;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      arg1K0 = 0;
      goto L28404;}}}
 L28404: {
  key_559X = arg1K0;
  p_560X = RSenvS;
  if ((p_560X < Sstack_beginS)) {
    arg1K0 = RSenvS;
    goto L28406;}
  else {
    if ((Sstack_endS < p_560X)) {
      arg1K0 = RSenvS;
      goto L28406;}
    else {
      merged_arg1K0 = RSenvS;
      merged_arg1K1 = ScontS;
      merged_arg1K2 = key_559X;
      merged_arg0K3 = 0;
      save_env_in_heap_return_tag = 0;
      goto save_env_in_heap;
     save_env_in_heap_return_0:
      v_561X = save_env_in_heap_return_value;
      RSenvS = v_561X;
      arg1K0 = RSenvS;
      goto L28406;}}}
 L28406: {
  env_562X = arg1K0;
  v_563X = (12 + ShpS) < SlimitS;
  if (v_563X) {
    goto L28408;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 15;
    goto collect_saving_temp;
   collect_saving_temp_return_15:
    okayP_564X = (12 + ShpS) < SlimitS;
    if (okayP_564X) {
      goto L28408;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      goto L28408;}}}
 L28408: {
  b_565X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  a_566X = *((long*)((-3 + StemplateS) + (((((long)b_565X))<<2))));
  *((long*)ShpS) = (2062 | 128); /**** JAR HACK - IMMUTABLE CLOSURES ****/
  ShpS = (4 + ShpS);
  new_567X = 3 + ShpS;
  ShpS = (8 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)(-3 + new_567X)) = a_566X;
  *((long*)(1 + new_567X)) = env_562X;
  RSvalS = new_567X;
  goto START;}
 L28304: {
  env_568X = arg1K0;
  i_569X = arg1K1;
  if ((0 == i_569X)) {
    b_570X = *((unsigned char*)RScode_pointerS);
    RScode_pointerS = (1 + RScode_pointerS);
    *((long*)((-3 + env_568X) + (((((long)b_570X))<<2)))) = RSvalS;
    RSvalS = 13;
    goto START;}
  else {
    arg1K0 = (*((long*)(-3 + env_568X)));
    arg1K1 = (-1 + i_569X);
    goto L28304;}}
 L27984: {
  env_571X = arg1K0;
  i_572X = arg1K1;
  if ((0 == i_572X)) {
    b_573X = *((unsigned char*)RScode_pointerS);
    RScode_pointerS = (1 + RScode_pointerS);
    RSvalS = (*((long*)((-3 + env_571X) + (((((long)b_573X))<<2)))));
    if ((529 == RSvalS)) {
      merged_arg0K0 = 2;
      goto raise_exception;}
    else {
      goto START;}}
  else {
    arg1K0 = (*((long*)(-3 + env_571X)));
    arg1K1 = (-1 + i_572X);
    goto L27984;}}
 L28232: {
  args_574X = arg1K0;
  RSvalS = args_574X;
  SnargsS = (1 + ((long)b_385X));
  goto START;}
 L28160: {
  b_577X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  len_576X = 4 + (((((long)b_577X))<<2));
  *((long*)ShpS) = (10 + (((len_576X)<<8)));
  ShpS = (4 + ShpS);
  new_575X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_576X)));
  if ((0 < len_576X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_575X;
    goto L9834;}
  else {
    arg1K0 = new_575X;
    goto L9834;}}
 L28086: {
  b_578X = *((unsigned char*)RScode_pointerS);
  RScode_pointerS = (1 + RScode_pointerS);
  *((long*)RSstackS) = RSenvS;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = (1034 + (((((long)b_578X))<<10)));
  RSstackS = (-4 + RSstackS);
  RSenvS = (11 + RSstackS);
  goto START;}
 L28123: {
  v_579X = arg1K0;
  merged_arg1K0 = v_579X;
  copy_stack_into_heap_return_tag = 1;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_1:
  goto L28086;}
 L31502: {
  merged_arg0K0 = 0;
  merged_arg1K1 = proc_197X;
  goto raise_exception1;}
 L31447: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L31322: {
  v_580X = arg0K0;
  RSvalS = ((long)v_580X);
  goto START;}
 L31300: {
  v_581X = arg0K0;
  RSvalS = ((long)v_581X);
  goto START;}
 L31276: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L31255: {
  RSvalS = x_145X;
  goto START;}
 L31240: {
  v_582X = arg0K0;
  RSvalS = ((long)v_582X);
  goto START;}
 L31200: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L31165: {
  merged_arg0K0 = 0;
  merged_arg1K1 = RSvalS;
  goto raise_exception1;}
 L31116: {
  v_583X = arg0K0;
  RSvalS = ((long)v_583X);
  goto START;}
 L31098: {
  v_584X = arg0K0;
  RSvalS = ((long)v_584X);
  goto START;}
 L31076: {
  merged_arg0K0 = 0;
  merged_arg1K1 = (((x_328X)<<2));
  goto raise_exception1;}
 L31039: {
  v_585X = arg0K0;
  RSvalS = ((long)v_585X);
  goto START;}
 L31013: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_90X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L30990: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_334X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L30967: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_88X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L30934: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_336X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L30913: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_86X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L30892: {
  merged_arg0K0 = 0;
  merged_arg1K1 = arg2_338X;
  merged_arg1K2 = RSvalS;
  goto raise_exception2;}
 L30869: {
  r_586X = arg1K0;
  if ((536870911 < r_586X)) {
    merged_arg0K0 = 0;
    merged_arg1K1 = x_339X;
    goto raise_exception1;}
  else {
    RSvalS = (((r_586X)<<2));
    goto START;}}
 L30612: {
  RSvalS = 1;
  goto START;}
 L30583: {
  RSvalS = 1;
  goto START;}
 L30554: {
  RSvalS = 1;
  goto START;}
 L30525: {
  RSvalS = 1;
  goto START;}
 L30505: {
  v_587X = arg0K0;
  RSvalS = ((long)v_587X);
  goto START;}
 L30491: {
  v_588X = arg0K0;
  RSvalS = ((long)v_588X);
  goto START;}
 L30370: {
  v_589X = arg1K0;
  ScontS = v_589X;
  SnargsS = 0;
  merged_arg0K0 = 0;
  goto perform_application;}
 L30336: {
  key_590X = arg1K0;
  merged_arg1K0 = key_590X;
  merged_arg0K1 = 2;
  preserve_continuation_return_tag = 0;
  goto preserve_continuation;
 preserve_continuation_return_0:
  v_591X = preserve_continuation_return_value;
  RSvalS = v_591X;
  goto START;}
 raise_exception: {
  args_850X = merged_arg0K0;{
  push_exception_continuationB_return_tag = 0;
  goto push_exception_continuationB;
 push_exception_continuationB_return_0:
  *((long*)RSstackS) = (((((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_850X)))))))<<2));
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 0;
  goto raise;}}

 return_values: {
  nargs_848X = merged_arg1K0;{
  if ((1 == nargs_848X)) {
    RSstackS = (4 + RSstackS);
    RSvalS = (*((long*)RSstackS));
    cont_592X = ScontS;
    tem_593X = *((long*)(5 + cont_592X));
    pc_594X = *((long*)(1 + cont_592X));
    StemplateS = tem_593X;
    RScode_pointerS = ((-3 + (*((long*)(-3 + tem_593X)))) + (((pc_594X)>>2)));
    RSenvS = (*((long*)(9 + cont_592X)));
    ScontS = (*((long*)(-3 + cont_592X)));
    RSstackS = (9 + cont_592X);
    goto START;}
  else {
    if ((ScontS == Sbottom_of_stackS)) {
      arg1K0 = (*((long*)(-3 + Sbottom_of_stackS)));
      goto L27694;}
    else {
      arg1K0 = ScontS;
      goto L27694;}}}
 L27644: {
  args_607X = arg1K0;
  merged_arg0K0 = 0;
  merged_arg1K1 = args_607X;
  goto raise_exception1;}
 L27591: {
  SnargsS = nargs_848X;
  RSvalS = (*((long*)(13 + cont_598X)));
  merged_arg0K0 = 0;
  goto perform_application;}
 L27694: {
  cont_598X = arg1K0;
  if ((3 == (3 & cont_598X))) {
    v_600X = 31 & ((((*((long*)(-7 + cont_598X))))>>2));
    if ((8 == v_600X)) {
      next_op_601X = *((unsigned char*)((-3 + (*((long*)(-3 + (*((long*)(5 + cont_598X))))))) + ((((*((long*)(1 + cont_598X))))>>2))));
      if ((31 == ((long)next_op_601X))) {
        cont_604X = ScontS;
        tem_605X = *((long*)(5 + cont_604X));
        pc_606X = *((long*)(1 + cont_604X));
        StemplateS = tem_605X;
        RScode_pointerS = ((-3 + (*((long*)(-3 + tem_605X)))) + (((pc_606X)>>2)));
        RSenvS = (*((long*)(9 + cont_604X)));
        ScontS = (*((long*)(-3 + cont_604X)));
        RSstackS = (9 + cont_604X);
        goto START;}
      else {
        if ((27 == ((long)next_op_601X))) {
          next_603X = *((long*)(-3 + ScontS));
          if ((ScontS == Sbottom_of_stackS)) {
            *((long*)(-3 + ScontS)) = (*((long*)(-3 + next_603X)));
            goto L27591;}
          else {
            ScontS = next_603X;
            goto L27591;}}
        else {
          if ((64 == nargs_848X)) {
            RSstackS = (4 + RSstackS);
            merged_arg1K0 = (*((long*)RSstackS));
            merged_arg1K1 = 63;
            pop_args_list_return_tag = 4;
            goto pop_args_list;
           pop_args_list_return_4:
            v_599X = pop_args_list_return_value;
            arg1K0 = v_599X;
            goto L27644;}
          else {
            merged_arg1K0 = 25;
            merged_arg1K1 = nargs_848X;
            pop_args_list_return_tag = 5;
            goto pop_args_list;
           pop_args_list_return_5:
            v_602X = pop_args_list_return_value;
            arg1K0 = v_602X;
            goto L27644;}}}}
    else {
      goto L27702;}}
  else {
    goto L27702;}}
 L27702: {
  if ((64 == nargs_848X)) {
    RSstackS = (4 + RSstackS);
    merged_arg1K0 = (*((long*)RSstackS));
    merged_arg1K1 = 63;
    pop_args_list_return_tag = 6;
    goto pop_args_list;
   pop_args_list_return_6:
    v_597X = pop_args_list_return_value;
    arg1K0 = v_597X;
    goto L27732;}
  else {
    merged_arg1K0 = 25;
    merged_arg1K1 = nargs_848X;
    pop_args_list_return_tag = 7;
    goto pop_args_list;
   pop_args_list_return_7:
    v_596X = pop_args_list_return_value;
    arg1K0 = v_596X;
    goto L27732;}}
 L27732: {
  args_595X = arg1K0;
  merged_arg0K0 = 0;
  merged_arg1K1 = args_595X;
  goto raise_exception1;}}

 raise_exception1: {
  args_845X = merged_arg0K0;
  a1_846X = merged_arg1K1;{
  push_exception_continuationB_return_tag = 1;
  goto push_exception_continuationB;
 push_exception_continuationB_return_1:
  *((long*)RSstackS) = (((((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_845X)))))))<<2));
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_846X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 1;
  goto raise;}}

 raise_exception2: {
  args_841X = merged_arg0K0;
  a1_842X = merged_arg1K1;
  a2_843X = merged_arg1K2;{
  push_exception_continuationB_return_tag = 2;
  goto push_exception_continuationB;
 push_exception_continuationB_return_2:
  *((long*)RSstackS) = (((((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_841X)))))))<<2));
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_842X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a2_843X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 2;
  goto raise;}}

 raise_exception3: {
  args_836X = merged_arg0K0;
  a1_837X = merged_arg1K1;
  a2_838X = merged_arg1K2;
  a3_839X = merged_arg1K3;{
  push_exception_continuationB_return_tag = 3;
  goto push_exception_continuationB;
 push_exception_continuationB_return_3:
  *((long*)RSstackS) = (((((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_836X)))))))<<2));
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_837X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a2_838X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a3_839X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 3;
  goto raise;}}

 raise_exception4: {
  args_830X = merged_arg0K0;
  a1_831X = merged_arg1K1;
  a2_832X = merged_arg1K2;
  a3_833X = merged_arg1K3;
  a4_834X = merged_arg1K4;{
  push_exception_continuationB_return_tag = 4;
  goto push_exception_continuationB;
 push_exception_continuationB_return_4:
  *((long*)RSstackS) = (((((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)args_830X)))))))<<2));
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_831X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a2_832X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a3_833X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a4_834X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 4;
  goto raise;}}

 application_exception: {
  byte_args_828X = merged_arg0K0;{
  if ((64 == SnargsS)) {
    RSstackS = (4 + RSstackS);
    merged_arg1K0 = (*((long*)RSstackS));
    merged_arg1K1 = 63;
    pop_args_list_return_tag = 8;
    goto pop_args_list;
   pop_args_list_return_8:
    v_609X = pop_args_list_return_value;
    arg1K0 = v_609X;
    goto L24697;}
  else {
    merged_arg1K0 = 25;
    merged_arg1K1 = SnargsS;
    pop_args_list_return_tag = 9;
    goto pop_args_list;
   pop_args_list_return_9:
    v_608X = pop_args_list_return_value;
    arg1K0 = v_608X;
    goto L24697;}}
 L24697: {
  args_610X = arg1K0;
  a1_611X = RSvalS;
  *((long*)RSstackS) = (((((long)(*((unsigned char*)(-1 + (RScode_pointerS - ((long)byte_args_828X)))))))<<2));
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = a1_611X;
  RSstackS = (-4 + RSstackS);
  *((long*)RSstackS) = args_610X;
  RSstackS = (-4 + RSstackS);
  merged_arg0K0 = 2;
  goto raise;}}

 raise: {
  nargs_826X = merged_arg0K0;{
  SnargsS = (1 + ((long)nargs_826X));
  RSvalS = Sexception_handlerS;
  obj_613X = RSvalS;
  if ((3 == (3 & obj_613X))) {
    v_612X = 31 & ((((*((long*)(-7 + obj_613X))))>>2));
    if ((3 == v_612X)) {
      goto L16058;}
    else {
      goto L16128;}}
  else {
    goto L16128;}}
 L16128: {
  TTerror("exception handler is not a closure", 0, 0, 0, 0);
  goto L16058;}
 L16060: {
  if (Sexception_space_usedPS) {
    x_619X = Sstack_limitS < (-296 + RSstackS);
    if (x_619X) {
      goto L16073;}
    else {
      TTerror("no space on stack to reserve exception space", 0, 0, 0, 0);
      goto L16073;}}
  else {
    goto L16062;}}
 L16098: {
  v_618X = arg1K0;
  merged_arg1K0 = v_618X;
  copy_stack_into_heap_return_tag = 2;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_2:
  goto L16060;}
 L16062: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L16073: {
  Sstack_limitS = (296 + Sstack_beginS);
  Sexception_space_usedPS = 0;
  goto L16062;}
 L16058: {
  x_617X = Sstack_limitS < (-296 + RSstackS);
  if (x_617X) {
    goto L16060;}
  else {
    space_616X = 1 + ((((Sstack_endS - RSstackS))>>2));
    v_615X = (ShpS + (((space_616X)<<2))) < SlimitS;
    if (v_615X) {
      arg1K0 = 0;
      goto L16098;}
    else {
      merged_arg1K0 = 0;
      collect_saving_temp_return_tag = 16;
      goto collect_saving_temp;
     collect_saving_temp_return_16:
      okayP_614X = (ShpS + (((space_616X)<<2))) < SlimitS;
      if (okayP_614X) {
        arg1K0 = 0;
        goto L16098;}
      else {
        TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
        arg1K0 = 0;
        goto L16098;}}}}}

 perform_application: {
  bytes_consumed_824X = merged_arg0K0;{
  obj_621X = RSvalS;
  if ((3 == (3 & obj_621X))) {
    v_622X = 31 & ((((*((long*)(-7 + obj_621X))))>>2));
    if ((3 == v_622X)) {
      RSenvS = (*((long*)(1 + RSvalS)));
      tem_620X = *((long*)(-3 + RSvalS));
      StemplateS = tem_620X;
      RScode_pointerS = (-3 + (*((long*)(-3 + tem_620X))));
      if ((0 == (Spending_interruptsS & Senabled_interruptsS))) {
        goto START;}
      else {
        goto handle_interrupt;}}
    else {
      goto L15871;}}
  else {
    goto L15871;}}
 L15871: {
  merged_arg0K0 = bytes_consumed_824X;
  goto application_exception;}}

 handle_interrupt: {
{ x_626X = Sstack_limitS < (-276 + RSstackS);
  if (x_626X) {
    arg1K0 = 0;
    goto L16172;}
  else {
    space_625X = 1 + ((((Sstack_endS - RSstackS))>>2));
    v_624X = (ShpS + (((space_625X)<<2))) < SlimitS;
    if (v_624X) {
      arg1K0 = 0;
      goto L16420;}
    else {
      merged_arg1K0 = 0;
      collect_saving_temp_return_tag = 17;
      goto collect_saving_temp;
     collect_saving_temp_return_17:
      okayP_623X = (ShpS + (((space_625X)<<2))) < SlimitS;
      if (okayP_623X) {
        arg1K0 = 0;
        goto L16420;}
      else {
        TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
        arg1K0 = 0;
        goto L16420;}}}}
 L16172: {
  key_635X = arg1K0;
  n_636X = Spending_interruptsS & Senabled_interruptsS;
  arg1K0 = 0;
  arg1K1 = 1;
  goto L16404;}
 L16420: {
  v_634X = arg1K0;
  merged_arg1K0 = v_634X;
  copy_stack_into_heap_return_tag = 3;
  goto copy_stack_into_heap;
 copy_stack_into_heap_return_3:
  arg1K0 = 0;
  goto L16172;}
 L16404: {
  i_629X = arg1K0;
  m_630X = arg1K1;
  if ((0 == (n_636X & m_630X))) {
    arg1K0 = (1 + i_629X);
    arg1K1 = (((m_630X)<<1));
    goto L16404;}
  else {
    Spending_interruptsS = (n_636X & (~ m_630X));
    *((long*)RSstackS) = RSvalS;
    RSstackS = (-4 + RSstackS);
    *((long*)RSstackS) = StemplateS;
    RSstackS = (-4 + RSstackS);
    *((long*)RSstackS) = RSenvS;
    RSstackS = (-4 + RSstackS);
    *((long*)RSstackS) = (((SnargsS)<<2));
    RSstackS = (-4 + RSstackS);
    *((long*)RSstackS) = (((Senabled_interruptsS)<<2));
    RSstackS = (-4 + RSstackS);
    tem_632X = Sinterrupt_templateS;
    StemplateS = tem_632X;
    RScode_pointerS = (-3 + (*((long*)(-3 + tem_632X))));
    merged_arg1K0 = RScode_pointerS;
    merged_arg1K1 = (5 + SnargsS);  /* Patched per RK's message of 2/25/94 */
    merged_arg1K2 = key_635X;
    push_continuationB_return_tag = 1;
    goto push_continuationB;
   push_continuationB_return_1:
    *((long*)RSstackS) = (((Senabled_interruptsS)<<2));
    RSstackS = (-4 + RSstackS);
    SnargsS = 1;
    obj_631X = Sinterrupt_handlersS;
    if ((3 == (3 & obj_631X))) {
      v_633X = 31 & ((((*((long*)(-7 + obj_631X))))>>2));
      if ((2 == v_633X)) {
        goto L16214;}
      else {
        goto L16292;}}
    else {
      goto L16292;}}}
 L16292: {
  TTerror("interrupt handler is not a vector", 0, 0, 0, 0);
  goto L16214;}
 L16242: {
  TTerror("interrupt handler is not a closure", 1, i_629X, 0, 0);
  goto L16232;}
 L16232: {
  merged_arg0K0 = 0;
  goto perform_application;}
 L16214: {
  RSvalS = (*((long*)((-3 + Sinterrupt_handlersS) + (((i_629X)<<2)))));
  Senabled_interruptsS = 0;
  obj_628X = RSvalS;
  if ((3 == (3 & obj_628X))) {
    v_627X = 31 & ((((*((long*)(-7 + obj_628X))))>>2));
    if ((3 == v_627X)) {
      goto L16232;}
    else {
      goto L16242;}}
  else {
    goto L16242;}}}

 copy_stack_into_heap: {
  key_821X = merged_arg1K0;{
  arg1K0 = (4 + RSstackS);
  arg1K1 = 0;
  goto L15112;}
 L15112: {
  p_640X = arg1K0;
  i_641X = arg1K1;
  if ((2 == (3 & (*((long*)p_640X))))) {
    len_643X = ((i_641X)<<2);
    *((long*)ShpS) = (10 + (((len_643X)<<8)));
    ShpS = (4 + ShpS);
    new_642X = 3 + ShpS;
    ShpS = (ShpS + (-4 & (3 + len_643X)));
    if ((0 < len_643X)) {
      *((long*)(-4 + ShpS)) = 0;
      arg1K0 = new_642X;
      goto L14976;}
    else {
      arg1K0 = new_642X;
      goto L14976;}}
  else {
    arg1K0 = (4 + p_640X);
    arg1K1 = (1 + i_641X);
    goto L15112;}}
 L14976: {
  vec_639X = arg1K0;
  arg1K0 = (-1 + i_641X);
  goto L14978;}
 L15002: {
  i_638X = arg1K0;
  if ((i_638X < i_641X)) {
    *((long*)RSstackS) = (*((long*)((-3 + vec_639X) + (((i_638X)<<2)))));
    RSstackS = (-4 + RSstackS);
    arg1K0 = (1 + i_638X);
    goto L15002;}
  else {
    goto copy_stack_into_heap_return;}}
 L14978: {
  i_637X = arg1K0;
  if ((-1 < i_637X)) {
    RSstackS = (4 + RSstackS);
    *((long*)((-3 + vec_639X) + (((i_637X)<<2)))) = (*((long*)RSstackS));
    arg1K0 = (-1 + i_637X);
    goto L14978;}
  else {
    merged_arg1K0 = key_821X;
    merged_arg0K1 = 1;
    preserve_continuation_return_tag = 1;
    goto preserve_continuation;
   preserve_continuation_return_1:
    arg1K0 = 0;
    goto L15002;}}
 copy_stack_into_heap_return:
  switch (copy_stack_into_heap_return_tag) {
  case 0: goto copy_stack_into_heap_return_0;
  case 1: goto copy_stack_into_heap_return_1;
  case 2: goto copy_stack_into_heap_return_2;
  case 3: goto copy_stack_into_heap_return_3;
  }}

 preserve_continuation: {
  key_819X = merged_arg1K0;
  reason_820X = merged_arg0K1;{
  p_644X = RSenvS;
  if ((p_644X < Sstack_beginS)) {
    goto L14717;}
  else {
    if ((Sstack_endS < p_644X)) {
      goto L14717;}
    else {
      merged_arg1K0 = RSenvS;
      merged_arg1K1 = ScontS;
      merged_arg1K2 = key_819X;
      merged_arg0K3 = reason_820X;
      save_env_in_heap_return_tag = 1;
      goto save_env_in_heap;
     save_env_in_heap_return_1:
      v_645X = save_env_in_heap_return_value;
      RSenvS = v_645X;
      goto L14717;}}}
 L14717: {
  end_653X = *((long*)(-3 + Sbottom_of_stackS));
  arg1K0 = ScontS;
  arg1K1 = Sbottom_of_stackS;
  goto L14722;}
 L14775: {
  v_652X = arg1K0;
  ScontS = v_652X;
  preserve_continuation_return_value = cont_649X;
  goto preserve_continuation_return;}
 L14744: {
  merged_arg1K0 = cont_646X;
  merged_arg1K1 = key_819X;
  copy_stob_return_tag = 0;
  goto copy_stob;
 copy_stob_return_0:
  new_651X = copy_stob_return_value;
  *((long*)(-3 + previous_647X)) = new_651X;
  arg1K0 = (*((long*)(-3 + new_651X)));
  arg1K1 = new_651X;
  goto L14722;}
 L14722: {
  cont_646X = arg1K0;
  previous_647X = arg1K1;
  if ((cont_646X == Sbottom_of_stackS)) {
    *((long*)(-3 + previous_647X)) = end_653X;
    cont_649X = *((long*)(-3 + Sbottom_of_stackS));
    if ((1 == cont_649X)) {
      arg1K0 = Sbottom_of_stackS;
      goto L14775;}
    else {
      merged_arg1K0 = cont_649X;
      copy_continuation_from_heap_return_tag = 2;
      goto copy_continuation_from_heap;
     copy_continuation_from_heap_return_2:
      v_648X = copy_continuation_from_heap_return_value;
      arg1K0 = v_648X;
      goto L14775;}}
  else {
    p_650X = *((long*)(9 + cont_646X));
    if ((p_650X < Sstack_beginS)) {
      goto L14744;}
    else {
      if ((Sstack_endS < p_650X)) {
        goto L14744;}
      else {
        merged_arg1K0 = (*((long*)(9 + cont_646X)));
        merged_arg1K1 = cont_646X;
        merged_arg1K2 = key_819X;
        merged_arg0K3 = reason_820X;
        save_env_in_heap_return_tag = 2;
        goto save_env_in_heap;
       save_env_in_heap_return_2:
        goto L14744;}}}}
 preserve_continuation_return:
  switch (preserve_continuation_return_tag) {
  case 0: goto preserve_continuation_return_0;
  case 1: goto preserve_continuation_return_1;
  }}

 copy_continuation_from_heap: {
  cont_817X = merged_arg1K0;{
  cells_654X = (((3 + ((long)(((unsigned long)(*((long*)(-7 + cont_817X))))>>8))))>>2);
  top_655X = (-11 + Sbottom_of_stackS) - (((cells_654X)<<2));
  new_cont_656X = 7 + top_655X;
  RSstackS = (-4 + top_655X);
  v_657X = (((3 + ((long)(((unsigned long)(*((long*)(-7 + cont_817X))))>>8))))>>2);
  from_658X = -7 + cont_817X;
  arg1K0 = from_658X;
  arg1K1 = top_655X;
  goto L12927;}
 L12927: {
  from_659X = arg1K0;
  to_660X = arg1K1;
  if ((from_659X < (4 + (from_658X + (((v_657X)<<2)))))) {
    *((long*)to_660X) = (*((long*)from_659X));
    arg1K0 = (4 + from_659X);
    arg1K1 = (4 + to_660X);
    goto L12927;}
  else {
    *((long*)(-3 + Sbottom_of_stackS)) = (*((long*)(-3 + new_cont_656X)));
    *((long*)(-3 + new_cont_656X)) = Sbottom_of_stackS;
    copy_continuation_from_heap_return_value = new_cont_656X;
    goto copy_continuation_from_heap_return;}}
 copy_continuation_from_heap_return:
  switch (copy_continuation_from_heap_return_tag) {
  case 0: goto copy_continuation_from_heap_return_0;
  case 1: goto copy_continuation_from_heap_return_1;
  case 2: goto copy_continuation_from_heap_return_2;
  }}

 save_env_in_heap: {
  env_813X = merged_arg1K0;
  cont_814X = merged_arg1K1;
  key_815X = merged_arg1K2;{
  merged_arg1K0 = env_813X;
  merged_arg1K1 = key_815X;
  copy_stob_return_tag = 1;
  goto copy_stob;
 copy_stob_return_1:
  new_661X = copy_stob_return_value;
  *((long*)(-7 + env_813X)) = new_661X;
  arg1K0 = new_661X;
  goto L14215;}
 L14239: {
  arg1K0 = cont_814X;
  goto L14241;}
 L14241: {
  cont_666X = arg1K0;
  env_668X = *((long*)(9 + cont_666X));
  if ((3 == (3 & env_668X))) {
    v_667X = 3 == (3 & (*((long*)(-7 + env_668X))));
    if (v_667X) {
      *((long*)(9 + cont_666X)) = (*((long*)(-7 + env_668X)));
      arg1K0 = (*((long*)(-3 + cont_666X)));
      goto L14241;}
    else {
      save_env_in_heap_return_value = new_661X;
      goto save_env_in_heap_return;}}
  else {
    save_env_in_heap_return_value = new_661X;
    goto save_env_in_heap_return;}}
 L14215: {
  env_662X = arg1K0;
  p_663X = *((long*)(-3 + env_662X));
  if ((p_663X < Sstack_beginS)) {
    goto L14239;}
  else {
    if ((Sstack_endS < p_663X)) {
      goto L14239;}
    else {
      env_664X = *((long*)(-3 + env_662X));
      merged_arg1K0 = env_664X;
      merged_arg1K1 = key_815X;
      copy_stob_return_tag = 2;
      goto copy_stob;
     copy_stob_return_2:
      new_665X = copy_stob_return_value;
      *((long*)(-7 + env_664X)) = new_665X;
      *((long*)(-3 + env_662X)) = new_665X;
      arg1K0 = new_665X;
      goto L14215;}}}
 save_env_in_heap_return:
  switch (save_env_in_heap_return_tag) {
  case 0: goto save_env_in_heap_return_0;
  case 1: goto save_env_in_heap_return_1;
  case 2: goto save_env_in_heap_return_2;
  }}

 copy_stob: {
  stob_811X = merged_arg1K0;{
  len_671X = (long)(((unsigned long)(*((long*)(-7 + stob_811X))))>>8);
  h_670X = *((long*)(-7 + stob_811X));
  *((long*)ShpS) = (2 + (((((((len_671X)<<6)) + (31 & (((h_670X)>>2)))))<<2)));
  ShpS = (4 + ShpS);
  new_669X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_671X)));
  if ((0 < len_671X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_669X;
    goto L9221;}
  else {
    arg1K0 = new_669X;
    goto L9221;}}
 L9221: {
  new_674X = arg1K0;
  bytes_675X = (long)(((unsigned long)(*((long*)(-7 + stob_811X))))>>8);
  from_676X = -3 + stob_811X;
  arg1K0 = from_676X;
  arg1K1 = (-3 + new_674X);
  goto L9242;}
 L9242: {
  from_672X = arg1K0;
  to_673X = arg1K1;
  if ((from_672X < (from_676X + (-4 & (3 + bytes_675X))))) {
    *((long*)to_673X) = (*((long*)from_672X));
    arg1K0 = (4 + from_672X);
    arg1K1 = (4 + to_673X);
    goto L9242;}
  else {
    copy_stob_return_value = new_674X;
    goto copy_stob_return;}}
 copy_stob_return:
  switch (copy_stob_return_tag) {
  case 0: goto copy_stob_return_0;
  case 1: goto copy_stob_return_1;
  case 2: goto copy_stob_return_2;
  }}

 push_exception_continuationB: {
{ x_677X = Sstack_limitS < (-296 + RSstackS);
  if (x_677X) {
    arg1K0 = 0;
    goto L14175;}
  else {
    Sstack_limitS = Sstack_beginS;
    Sexception_space_usedPS = 1;
    x_678X = Sstack_limitS < (-296 + RSstackS);
    if (x_678X) {
      arg1K0 = 0;
      goto L14175;}
    else {
      TTerror("insufficient space on stack for exception frame", 0, 0, 0, 0);
      arg1K0 = 0;
      goto L14175;}}}
 L14175: {
  key_681X = arg1K0;
  arg1K0 = (4 + RSstackS);
  arg1K1 = 0;
  goto L14187;}
 L14187: {
  p_679X = arg1K0;
  i_680X = arg1K1;
  if ((2 == (3 & (*((long*)p_679X))))) {
    merged_arg1K0 = RScode_pointerS;
    merged_arg1K1 = i_680X;
    merged_arg1K2 = key_681X;
    push_continuationB_return_tag = 2;
    goto push_continuationB;
   push_continuationB_return_2:
    goto push_exception_continuationB_return;}
  else {
    arg1K0 = (4 + p_679X);
    arg1K1 = (1 + i_680X);
    goto L14187;}}
 push_exception_continuationB_return:
  switch (push_exception_continuationB_return_tag) {
  case 0: goto push_exception_continuationB_return_0;
  case 1: goto push_exception_continuationB_return_1;
  case 2: goto push_exception_continuationB_return_2;
  case 3: goto push_exception_continuationB_return_3;
  case 4: goto push_exception_continuationB_return_4;
  }}

 push_continuationB: {
  code_pointer_808X = merged_arg1K0;
  size_809X = merged_arg1K1;{
  pc_682X = 12 + ((((code_pointer_808X - (*((long*)(-3 + StemplateS)))))<<2));
  template_683X = StemplateS;
  RSstackS = (-20 + RSstackS);
  /*** JAR HACK -- IMMUTABLE CONTINUATIONS ***/
  *((long*)(4 + RSstackS)) = ((4130 | 128) + (((size_809X)<<10)));
  cont_684X = 11 + RSstackS;
  *((long*)(1 + cont_684X)) = pc_682X;
  *((long*)(5 + cont_684X)) = template_683X;
  *((long*)(9 + cont_684X)) = RSenvS;
  *((long*)(-3 + cont_684X)) = ScontS;
  ScontS = cont_684X;
  goto push_continuationB_return;}
 push_continuationB_return:
  switch (push_continuationB_return_tag) {
  case 0: goto push_continuationB_return_0;
  case 1: goto push_continuationB_return_1;
  case 2: goto push_continuationB_return_2;
  }}

 pop_args_list: {
  start_806X = merged_arg1K0;
  count_807X = merged_arg1K1;{
  space_685X = 3 * SnargsS;
  v_686X = (ShpS + (((space_685X)<<2))) < SlimitS;
  if (v_686X) {
    goto L15130;}
  else {
    merged_arg1K0 = 0;
    collect_saving_temp_return_tag = 18;
    goto collect_saving_temp;
   collect_saving_temp_return_18:
    okayP_687X = (ShpS + (((space_685X)<<2))) < SlimitS;
    if (okayP_687X) {
      goto L15130;}
    else {
      TTerror("Scheme48 heap overflow", 0, 0, 0, 0);
      goto L15130;}}}
 L15130: {
  arg1K0 = start_806X;
  arg1K1 = count_807X;
  goto L15132;}
 L15132: {
  args_688X = arg1K0;
  count_689X = arg1K1;
  if ((0 < count_689X)) {
    RSstackS = (4 + RSstackS);
    a_690X = *((long*)RSstackS);
    *((long*)ShpS) = 2050;
    ShpS = (4 + ShpS);
    new_691X = 3 + ShpS;
    ShpS = (8 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_691X)) = a_690X;
    *((long*)(1 + new_691X)) = args_688X;
    arg1K0 = new_691X;
    arg1K1 = (-1 + count_689X);
    goto L15132;}
  else {
    pop_args_list_return_value = args_688X;
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
  value_804X = merged_arg1K0;{
  b_696X = Snewspace_beginS;
  Snewspace_beginS = Soldspace_beginS;
  Soldspace_beginS = b_696X;
  e_695X = Snewspace_endS;
  Snewspace_endS = Soldspace_endS;
  Soldspace_endS = e_695X;
  Ssaved_limitS = SlimitS;
  Ssaved_hpS = ShpS;
  SlimitS = Snewspace_endS;
  ShpS = Snewspace_beginS;
  Sweak_pointer_hpS = -1;
  pc_694X = 12 + ((((RScode_pointerS - (*((long*)(-3 + StemplateS)))))<<2));
  value_693X = StemplateS;
  if ((3 == (3 & value_693X))) {
    a_697X = -3 + value_693X;
    if ((a_697X < Soldspace_beginS)) {
      arg1K0 = value_693X;
      goto L13934;}
    else {
      v_698X = a_697X < Soldspace_endS;
      if (v_698X) {
        merged_arg1K0 = value_693X;
        copy_object_return_tag = 1;
        goto copy_object;
       copy_object_return_1:
        v_692X = copy_object_return_value;
        arg1K0 = v_692X;
        goto L13934;}
      else {
        arg1K0 = value_693X;
        goto L13934;}}}
  else {
    arg1K0 = value_693X;
    goto L13934;}}
 L13934: {
  tem_756X = arg1K0;
  StemplateS = tem_756X;
  RScode_pointerS = ((-3 + (*((long*)(-3 + tem_756X)))) + (((pc_694X)>>2)));
  value_758X = RSvalS;
  if ((3 == (3 & value_758X))) {
    a_759X = -3 + value_758X;
    if ((a_759X < Soldspace_beginS)) {
      arg1K0 = value_758X;
      goto L13938;}
    else {
      v_760X = a_759X < Soldspace_endS;
      if (v_760X) {
        merged_arg1K0 = value_758X;
        copy_object_return_tag = 2;
        goto copy_object;
       copy_object_return_2:
        v_757X = copy_object_return_value;
        arg1K0 = v_757X;
        goto L13938;}
      else {
        arg1K0 = value_758X;
        goto L13938;}}}
  else {
    arg1K0 = value_758X;
    goto L13938;}}
 L13938: {
  v_751X = arg1K0;
  RSvalS = v_751X;
  value_753X = Sdynamic_stateS;
  if ((3 == (3 & value_753X))) {
    a_754X = -3 + value_753X;
    if ((a_754X < Soldspace_beginS)) {
      arg1K0 = value_753X;
      goto L13942;}
    else {
      v_755X = a_754X < Soldspace_endS;
      if (v_755X) {
        merged_arg1K0 = value_753X;
        copy_object_return_tag = 3;
        goto copy_object;
       copy_object_return_3:
        v_752X = copy_object_return_value;
        arg1K0 = v_752X;
        goto L13942;}
      else {
        arg1K0 = value_753X;
        goto L13942;}}}
  else {
    arg1K0 = value_753X;
    goto L13942;}}
 L13942: {
  v_746X = arg1K0;
  Sdynamic_stateS = v_746X;
  value_748X = Sexception_handlerS;
  if ((3 == (3 & value_748X))) {
    a_749X = -3 + value_748X;
    if ((a_749X < Soldspace_beginS)) {
      arg1K0 = value_748X;
      goto L13946;}
    else {
      v_750X = a_749X < Soldspace_endS;
      if (v_750X) {
        merged_arg1K0 = value_748X;
        copy_object_return_tag = 4;
        goto copy_object;
       copy_object_return_4:
        v_747X = copy_object_return_value;
        arg1K0 = v_747X;
        goto L13946;}
      else {
        arg1K0 = value_748X;
        goto L13946;}}}
  else {
    arg1K0 = value_748X;
    goto L13946;}}
 L13946: {
  v_741X = arg1K0;
  Sexception_handlerS = v_741X;
  value_743X = Sinterrupt_handlersS;
  if ((3 == (3 & value_743X))) {
    a_744X = -3 + value_743X;
    if ((a_744X < Soldspace_beginS)) {
      arg1K0 = value_743X;
      goto L13950;}
    else {
      v_745X = a_744X < Soldspace_endS;
      if (v_745X) {
        merged_arg1K0 = value_743X;
        copy_object_return_tag = 5;
        goto copy_object;
       copy_object_return_5:
        v_742X = copy_object_return_value;
        arg1K0 = v_742X;
        goto L13950;}
      else {
        arg1K0 = value_743X;
        goto L13950;}}}
  else {
    arg1K0 = value_743X;
    goto L13950;}}
 L13950: {
  v_736X = arg1K0;
  Sinterrupt_handlersS = v_736X;
  value_738X = Sinterrupt_templateS;
  if ((3 == (3 & value_738X))) {
    a_739X = -3 + value_738X;
    if ((a_739X < Soldspace_beginS)) {
      arg1K0 = value_738X;
      goto L13954;}
    else {
      v_740X = a_739X < Soldspace_endS;
      if (v_740X) {
        merged_arg1K0 = value_738X;
        copy_object_return_tag = 6;
        goto copy_object;
       copy_object_return_6:
        v_737X = copy_object_return_value;
        arg1K0 = v_737X;
        goto L13954;}
      else {
        arg1K0 = value_738X;
        goto L13954;}}}
  else {
    arg1K0 = value_738X;
    goto L13954;}}
 L13954: {
  v_731X = arg1K0;
  Sinterrupt_templateS = v_731X;
  value_733X = RSenvS;
  if ((3 == (3 & value_733X))) {
    a_734X = -3 + value_733X;
    if ((a_734X < Soldspace_beginS)) {
      arg1K0 = value_733X;
      goto L13960;}
    else {
      v_735X = a_734X < Soldspace_endS;
      if (v_735X) {
        merged_arg1K0 = value_733X;
        copy_object_return_tag = 7;
        goto copy_object;
       copy_object_return_7:
        v_732X = copy_object_return_value;
        arg1K0 = v_732X;
        goto L13960;}
      else {
        arg1K0 = value_733X;
        goto L13960;}}}
  else {
    arg1K0 = value_733X;
    goto L13960;}}
 L13960: {
  env_728X = arg1K0;
  RSenvS = env_728X;
  start_729X = 4 + RSstackS;
  end_730X = -7 + ScontS;
  arg1K0 = start_729X;
  goto L14040;}
 L14040: {
  addr_726X = arg1K0;
  if ((addr_726X < end_730X)) {
    merged_arg1K0 = addr_726X;
    copy_next_return_tag = 0;
    goto copy_next;
   copy_next_return_0:
    v_727X = copy_next_return_value;
    arg1K0 = v_727X;
    goto L14040;}
  else {
    arg1K0 = ScontS;
    arg1K1 = 0;
    goto L13975;}}
 L14026: {
  addr_724X = arg1K0;
  if ((addr_724X < ((-3 + cont_713X) + (-4 & (3 + byte_count_716X))))) {
    merged_arg1K0 = addr_724X;
    copy_next_return_tag = 1;
    goto copy_next;
   copy_next_return_1:
    v_725X = copy_next_return_value;
    arg1K0 = v_725X;
    goto L14026;}
  else {
    if ((env_715X == last_env_714X)) {
      goto L13982;}
    else {
      arg1K0 = env_715X;
      goto L13992;}}}
 L14012: {
  addr_722X = arg1K0;
  if ((addr_722X < ((-3 + env_720X) + (-4 & (3 + byte_count_721X))))) {
    merged_arg1K0 = addr_722X;
    copy_next_return_tag = 2;
    goto copy_next;
   copy_next_return_2:
    v_723X = copy_next_return_value;
    arg1K0 = v_723X;
    goto L14012;}
  else {
    arg1K0 = (*((long*)(-3 + env_720X)));
    goto L13992;}}
 L13992: {
  env_720X = arg1K0;
  if ((env_720X < Sstack_beginS)) {
    goto L13982;}
  else {
    if ((Sstack_endS < env_720X)) {
      goto L13982;}
    else {
      byte_count_721X = (long)(((unsigned long)(*((long*)(-7 + env_720X))))>>8);
      arg1K0 = (-3 + env_720X);
      goto L14012;}}}
 L13982: {
  if ((cont_713X == Sbottom_of_stackS)) {
    areas_717X = Simpure_areasS;
    sizes_718X = Simpure_sizesS;
    count_719X = Simpure_area_countS;
    arg1K0 = 0;
    goto L14562;}
  else {
    arg1K0 = (*((long*)(-3 + cont_713X)));
    arg1K1 = env_715X;
    goto L13975;}}
 L13975: {
  cont_713X = arg1K0;
  last_env_714X = arg1K1;
  env_715X = *((long*)(9 + cont_713X));
  byte_count_716X = (long)(((unsigned long)(*((long*)(-7 + cont_713X))))>>8);
  arg1K0 = (-3 + cont_713X);
  goto L14026;}
 L14572: {
  addr_711X = arg1K0;
  if ((addr_711X < end_710X)) {
    merged_arg1K0 = addr_711X;
    copy_next_return_tag = 3;
    goto copy_next;
   copy_next_return_3:
    v_712X = copy_next_return_value;
    arg1K0 = v_712X;
    goto L14572;}
  else {
    arg1K0 = (1 + i_705X);
    goto L14562;}}
 L14562: {
  i_705X = arg1K0;
  if ((i_705X < ((long)count_719X))) {
    start_709X = *(areas_717X + i_705X);
    end_710X = (*(areas_717X + i_705X)) + (*(sizes_718X + i_705X));
    arg1K0 = start_709X;
    goto L14572;}
  else {
    if ((3 == (3 & value_804X))) {
      a_707X = -3 + value_804X;
      if ((a_707X < Soldspace_beginS)) {
        arg1K0 = value_804X;
        goto L14457;}
      else {
        v_706X = a_707X < Soldspace_endS;
        if (v_706X) {
          merged_arg1K0 = value_804X;
          copy_object_return_tag = 8;
          goto copy_object;
         copy_object_return_8:
          v_708X = copy_object_return_value;
          arg1K0 = v_708X;
          goto L14457;}
        else {
          arg1K0 = value_804X;
          goto L14457;}}}
    else {
      arg1K0 = value_804X;
      goto L14457;}}}
 L14457: {
  value_704X = arg1K0;
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
  goto L14510;}
 L14521: {
  new_703X = arg1K0;
  *(Sopen_vm_portsS + i_699X) = new_703X;
  goto L14514;}
 L14514: {
  arg1K0 = (1 + i_699X);
  goto L14510;}
 L14510: {
  i_699X = arg1K0;
  if ((i_699X == Snumber_of_portsS)) {
    x_700X = (ShpS + (((Sminimum_recovered_spaceS)<<2))) < SlimitS;
    if (x_700X) {
      collect_saving_temp_return_value = value_704X;
      goto collect_saving_temp_return;}
    else {
      Spending_interruptsS = (4 | Spending_interruptsS);
      collect_saving_temp_return_value = value_704X;
      goto collect_saving_temp_return;}}
  else {
    port_701X = *(Sopen_vm_portsS + i_699X);
    if ((1 == port_701X)) {
      goto L14514;}
    else {
      header_702X = *((long*)(-7 + port_701X));
      if ((3 == (3 & header_702X))) {
        arg1K0 = header_702X;
        goto L14521;}
      else {
        merged_arg1K0 = port_701X;
        close_port_noisily_return_tag = 1;
        goto close_port_noisily;
       close_port_noisily_return_1:
        arg1K0 = 1;
        goto L14521;}}}}
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
  addr_802X = merged_arg1K0;{
  next_761X = 4 + addr_802X;
  thing_762X = *((long*)addr_802X);
  if ((2 == (3 & thing_762X))) {
    if (((31 & (((thing_762X)>>2))) < 15)) {
      goto L11576;}
    else {
      copy_next_return_value = (next_761X + (-4 & (3 + ((long)(((unsigned long)thing_762X)>>8)))));
      goto copy_next_return;}}
  else {
    goto L11576;}}
 L11576: {
  if ((3 == (3 & thing_762X))) {
    a_764X = -3 + thing_762X;
    if ((a_764X < Soldspace_beginS)) {
      copy_next_return_value = next_761X;
      goto copy_next_return;}
    else {
      v_765X = a_764X < Soldspace_endS;
      if (v_765X) {
        merged_arg1K0 = thing_762X;
        copy_object_return_tag = 9;
        goto copy_object;
       copy_object_return_9:
        value_763X = copy_object_return_value;
        *((long*)addr_802X) = value_763X;
        copy_next_return_value = next_761X;
        goto copy_next_return;}
      else {
        copy_next_return_value = next_761X;
        goto copy_next_return;}}}
  else {
    copy_next_return_value = next_761X;
    goto copy_next_return;}}
 copy_next_return:
  switch (copy_next_return_tag) {
  case 0: goto copy_next_return_0;
  case 1: goto copy_next_return_1;
  case 2: goto copy_next_return_2;
  case 3: goto copy_next_return_3;
  }}

 copy_object: {
  thing_800X = merged_arg1K0;{
  h_768X = *((long*)(-7 + thing_800X));
  if ((3 == (3 & h_768X))) {
    copy_object_return_value = h_768X;
    goto copy_object_return;}
  else {
    if ((1070 == h_768X)) {
      descriptor_769X = *((long*)(-3 + thing_800X));
      if ((3 == (3 & descriptor_769X))) {
        a_767X = -3 + descriptor_769X;
        if ((a_767X < Soldspace_beginS)) {
          goto L9664;}
        else {
          v_766X = a_767X < Soldspace_endS;
          if (v_766X) {
            SvalS = RSvalS;
            SstackS = RSstackS;
            Scode_pointerS = RScode_pointerS;
            SenvS = RSenvS;
            v_770X = copy_weak_pointer(thing_800X);
            RSvalS = SvalS;
            RSstackS = SstackS;
            RScode_pointerS = Scode_pointerS;
            RSenvS = SenvS;
            copy_object_return_value = v_770X;
            goto copy_object_return;}
          else {
            goto L9664;}}}
      else {
        goto L9664;}}
    else {
      goto L9664;}}}
 L9664: {
  *((long*)ShpS) = h_768X;
  ShpS = (4 + ShpS);
  new_772X = 3 + ShpS;
  *((long*)(-7 + thing_800X)) = new_772X;
  x1_773X = ShpS;
  arg1K0 = (-3 + thing_800X);
  goto L9678;}
 L9678: {
  o_771X = arg1K0;
  if ((ShpS < (x1_773X + (-4 & (3 + ((long)(((unsigned long)h_768X)>>8))))))) {
    *((long*)ShpS) = (*((long*)o_771X));
    ShpS = (4 + ShpS);
    arg1K0 = (4 + o_771X);
    goto L9678;}
  else {
    copy_object_return_value = new_772X;
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
  port_798X = merged_arg1K0;{
  header_774X = *((long*)(-7 + (*((long*)(9 + port_798X)))));
  if ((3 == (3 & header_774X))) {
    goto L12570;}
  else {
    goto L12570;}}
 L12570: {
  merged_arg1K0 = port_798X;
  close_port_return_tag = 1;
  goto close_port;
 close_port_return_1:
  fputs("Port closed: ", Scurrent_output_portS);
  string_775X = *((long*)(9 + port_798X));
  port_776X = Scurrent_output_portS;
  v_777X = (long)(((unsigned long)(*((long*)(-7 + string_775X))))>>8);
  fwrite((void *)(-3 + string_775X), sizeof(char), (-1 + v_777X), port_776X);
  putc(10, Scurrent_output_portS);
  goto close_port_noisily_return;}
 close_port_noisily_return:
  switch (close_port_noisily_return_tag) {
  case 0: goto close_port_noisily_return_0;
  case 1: goto close_port_noisily_return_1;
  }}

 close_port: {
  vm_port_797X = merged_arg1K0;{
  if (((*((long*)(1 + vm_port_797X))) < 0)) {
    goto close_port_return;}
  else {
    index_780X = (((*((long*)(1 + vm_port_797X))))>>2);
    if ((0 == index_780X)) {
      goto close_port_return;}
    else {
      if ((1 == index_780X)) {
        goto close_port_return;}
      else {
        mode_779X = (((*((long*)(-3 + vm_port_797X))))>>2);
        port_778X = *(Sopen_portsS + ((((*((long*)(1 + vm_port_797X))))>>2)));
        if ((1 == mode_779X)) {
          fclose(port_778X);
          goto L11231;}
        else {
          if ((2 == mode_779X)) {
            fclose(port_778X);
            goto L11231;}
          else {
            TTerror("this shouldn't happen when closing a port", 0, 0, 0, 0);
            goto L11231;}}}}}}
 L11231: {
  *((long*)(-3 + vm_port_797X)) = 0;
  *((long*)(1 + vm_port_797X)) = -4;
  *(Sopen_vm_portsS + index_780X) = 1;
  goto close_port_return;}
 close_port_return:
  switch (close_port_return_tag) {
  case 0: goto close_port_return_0;
  case 1: goto close_port_return_1;
  }}

 write_number: {
  x_795X = merged_arg1K0;
  port_796X = merged_arg2K1;{
  if ((x_795X < 0)) {
    putc(45, port_796X);
    arg1K0 = (0 - x_795X);
    goto L29726;}
  else {
    arg1K0 = x_795X;
    goto L29726;}}
 L29726: {
  v_781X = arg1K0;
  SvalS = RSvalS;
  SstackS = RSstackS;
  Scode_pointerS = RScode_pointerS;
  SenvS = RSenvS;
  really_write_number(v_781X, port_796X);
  RSvalS = SvalS;
  RSstackS = SstackS;
  RScode_pointerS = Scode_pointerS;
  RSenvS = SenvS;
  putc(10, port_796X);
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
  stob1_793X = merged_arg1K0;
  stob2_794X = merged_arg1K1;{
  z2_783X = *((long*)(-7 + stob2_794X));
  z1_782X = *((long*)(-7 + stob1_793X));
  if (((128 | z1_782X) == (128 | z2_783X))) {
    arg1K0 = 0;
    goto L8359;}
  else {
    stob_equalP_return_value = 0;
    goto stob_equalP_return;}}
 L8359: {
  i_784X = arg1K0;
  if ((i_784X < ((((3 + ((long)(((unsigned long)z1_782X)>>8))))>>2)))) {
    if (((*((long*)((-3 + stob1_793X) + (((i_784X)<<2))))) == (*((long*)((-3 + stob2_794X) + (((i_784X)<<2))))))) {
      arg1K0 = (1 + i_784X);
      goto L8359;}
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
  string_791X = merged_arg1K0;{
  v_785X = (long)(((unsigned long)(*((long*)(-7 + string_791X))))>>8);
  z_786X = -1 + v_785X;
  v_787X = (unsigned char *)calloc( 1, 1 + z_786X);
  arg1K0 = 0;
  goto L10905;}
 L10905: {
  i_788X = arg1K0;
  if ((i_788X < z_786X)) {
    v_789X = ((long)(*((unsigned char*)((-3 + string_791X) + i_788X))));
    *(v_787X + i_788X) = v_789X;
    arg1K0 = (1 + i_788X);
    goto L10905;}
  else {
    extract_string_return_value = v_787X;
    goto extract_string_return;}}
 extract_string_return:
  switch (extract_string_return_tag) {
  case 0: goto extract_string_return_0;
  case 1: goto extract_string_return_1;
  case 2: goto extract_string_return_2;
  }}
}

long enter_string(unsigned char *string_851X)
{
  extern long ShpS;
  long arg1K0;
  long new_852X;
  long len_853X;
  long z_854X;
  long s_855X;
  long i_856X;
  unsigned char c_857X;

 {z_854X = strlen(string_851X);
  len_853X = 1 + z_854X;
  *((long*)ShpS) = (62 + (((len_853X)<<8)));
  ShpS = (4 + ShpS);
  new_852X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_853X)));
  if ((0 < len_853X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_852X;
    goto L11987;}
  else {
    arg1K0 = new_852X;
    goto L11987;}}
 L11987: {
  s_855X = arg1K0;
  arg1K0 = 0;
  goto L11989;}
 L11989: {
  i_856X = arg1K0;
  if ((i_856X < z_854X)) {
    c_857X = *(string_851X + i_856X);
    *((unsigned char*)((-3 + s_855X) + i_856X)) = ((unsigned char)((long)(c_857X)));
    arg1K0 = (1 + i_856X);
    goto L11989;}
  else {
    return(s_855X);}}}

long copy_weak_pointer(long weak_858X)
{
  extern long ShpS;
  extern long Sweak_pointer_limitS;
  extern long Sweak_pointer_hpS;
  unsigned char tempWW3518_859X;
  long old_860X;
  long new_861X;

 {tempWW3518_859X = -1 == Sweak_pointer_hpS;
  if (tempWW3518_859X) {
    goto L8009;}
  else {
    if ((Sweak_pointer_hpS < Sweak_pointer_limitS)) {
      goto L8014;}
    else {
      goto L8009;}}}
 L8009: {
  old_860X = Sweak_pointer_hpS;
  Sweak_pointer_hpS = ShpS;
  ShpS = (1024 + ShpS);
  Sweak_pointer_limitS = ShpS;
  *((long*)Sweak_pointer_hpS) = 261186;
  *((long*)(8 + Sweak_pointer_hpS)) = old_860X;
  goto L8014;}
 L8014: {
  new_861X = 7 + Sweak_pointer_hpS;
  *((long*)(4 + Sweak_pointer_hpS)) = (*((long*)(-3 + weak_858X)));
  Sweak_pointer_hpS = (8 + Sweak_pointer_hpS);
  *((long*)(-7 + weak_858X)) = new_861X;
  return(new_861X);}}

void really_write_number(long x_862X, FILE * port_863X)
{
  extern void really_write_number(long, FILE *);
  long v_864X;
  long v_865X;

 {if ((x_862X < 10)) {
    goto L29688;}
  else {
    v_864X = x_862X / 10;
    really_write_number(v_864X, port_863X);
    goto L29688;}}
 L29688: {
  v_865X = x_862X % 10;
  putc(((48 + v_865X)), port_863X);
  return;}}

long read_number(FILE * port_866X)
{
  long arg1K0;
  unsigned char arg4K1;
  unsigned char arg4K0;
  unsigned char ch_867X;
  long r_868X;
  unsigned char ch_869X;
  long r_870X;
  unsigned char Kchar_871X;
  unsigned char ch_872X;
  unsigned char Kchar_873X;
  unsigned char ch_874X;

 {goto L4937;}
 L4966: {
  ch_867X = arg4K0;
  arg1K0 = 0;
  arg4K1 = ch_867X;
  goto L4968;}
 L4968: {
  r_868X = arg1K0;
  ch_869X = arg4K1;
  r_870X = (-48 + ((long)(ch_869X))) + (10 * r_868X);
  { int TTchar;
    PS_GETC(port_866X, TTchar);
    if (EOF == TTchar) {
      return(r_870X);}
    else {
      unsigned char Kchar_871X = TTchar;
      ch_872X = Kchar_871X;
      if ((((long)ch_872X) < 48)) {
        return(r_870X);}
      else {
        if ((57 < ((long)ch_872X))) {
          return(r_870X);}
        else {
          arg1K0 = r_870X;
          arg4K1 = Kchar_871X;
          goto L4968;}}}}}
 L4937: {
  { int TTchar;
    PS_GETC(port_866X, TTchar);
    if (EOF == TTchar) {
      arg4K0 = -1;
      goto L4966;}
    else {
      unsigned char Kchar_873X = TTchar;
      ch_874X = Kchar_873X;
      if ((((long)ch_874X) < 48)) {
        goto L4937;}
      else {
        if ((57 < ((long)ch_874X))) {
          goto L4937;}
        else {
          arg4K0 = Kchar_873X;
          goto L4966;}}}}}}

long call_startup_procedure(long startup_proc_875X, unsigned char **startup_vector_876X, long startup_vector_length_877X)
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
  extern long call_external_value(long, long, long);
  extern long SnargsS;
  extern long SvalS;
  extern long SstackS;
  extern long Sstack_beginS;
  extern long Sstack_limitS;
  extern unsigned char Sexception_space_usedPS;
  extern long enter_string(unsigned char*);
  extern long ShpS;
  long arg1K0;
  long new_878X;
  long len_879X;
  long tem_880X;
  long x_881X;
  long x_882X;
  long new_883X;
  long count_884X;
  long v_885X;
  long v_886X;
  long v_887X;
  char option_888X;
  long vector_889X;
  long i_890X;
  unsigned char x_891X;
  long val_892X;

 {len_879X = ((startup_vector_length_877X)<<2);
  *((long*)ShpS) = (10 + (((len_879X)<<8)));
  ShpS = (4 + ShpS);
  new_878X = 3 + ShpS;
  ShpS = (ShpS + (-4 & (3 + len_879X)));
  if ((0 < len_879X)) {
    *((long*)(-4 + ShpS)) = 0;
    arg1K0 = new_878X;
    goto L15557;}
  else {
    arg1K0 = new_878X;
    goto L15557;}}
 L12687: {
  SenvS = 13;
  tem_880X = Sinterrupt_templateS;
  StemplateS = tem_880X;
  Scode_pointerS = (-3 + (*((long*)(-3 + tem_880X))));
  SnargsS = 13;
  SvalS = 13;
  Sdynamic_stateS = 25;
  Sexception_handlerS = 25;
  Sinterrupt_handlersS = 25;
  Senabled_interruptsS = 0;
  Spending_interruptsS = 0;
  *((long*)SstackS) = vector_889X;
  SstackS = (-4 + SstackS);
  x_881X = *(Sopen_vm_portsS + 0);
  *((long*)SstackS) = x_881X;
  SstackS = (-4 + SstackS);
  x_882X = *(Sopen_vm_portsS + 1);
  *((long*)SstackS) = x_882X;
  SstackS = (-4 + SstackS);
  *((long*)ShpS) = 578;
  ShpS = (4 + ShpS);
  new_883X = 3 + ShpS;
  ShpS = (4 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((unsigned char*)(-3 + new_883X)) = 23;
  *((unsigned char*)(-2 + new_883X)) = 3;
  Scode_pointerS = (-3 + new_883X);
  SvalS = startup_proc_875X;
  goto L15491;}
 L12732: {
  Sstack_limitS = (296 + Sstack_beginS);
  Sexception_space_usedPS = 0;
  goto L12687;}
 L15491: {
  option_888X = TTrun_machine((long)Tinterpret);
  if ((0 == ((long)option_888X))) {
    return(SvalS);}
  else {
    if ((1 == ((long)option_888X))) {
      v_887X = *((long*)(-3 + (*((long*)(1 + SvalS)))));
      v_886X = 4 + SstackS;
      v_885X = call_external_value(v_887X, SnargsS, v_886X);
      SvalS = v_885X;
      count_884X = 1 + SnargsS;
      SstackS = (SstackS - ((((0 - count_884X))<<2)));
      goto L15491;}
    else {
      TTerror("unkown VM return option", 1, ((long)option_888X), 0, 0);
      return(-1);}}}
 L15557: {
  vector_889X = arg1K0;
  arg1K0 = 0;
  goto L15559;}
 L15559: {
  i_890X = arg1K0;
  if ((i_890X < startup_vector_length_877X)) {
    val_892X = enter_string((*(startup_vector_876X + i_890X)));
    *((long*)((-3 + vector_889X) + (((i_890X)<<2)))) = val_892X;
    arg1K0 = (1 + i_890X);
    goto L15559;}
  else {
    SstackS = (-11 + Sbottom_of_stackS);
    *((long*)(-3 + Sbottom_of_stackS)) = 1;
    if (Sexception_space_usedPS) {
      x_891X = Sstack_limitS < (-296 + SstackS);
      if (x_891X) {
        goto L12732;}
      else {
        TTerror("no space on stack to reserve exception space", 0, 0, 0, 0);
        goto L12732;}}
    else {
      goto L12687;}}}}

void initialize_vm(long memory_begin_893X, long memory_size_894X, long stack_begin_895X, long stack_size_896X)
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
  char merged_arg0K1;
  char merged_arg0K0;

  int make_template_containing_ops_return_tag;
  long make_template_containing_ops_return_value;
  long semisize_897X;
  FILE * *v_898X;
  long *v_899X;
  unsigned char tempWW3526_900X;
  unsigned char v_901X;
  long i_902X;
  long v_903X;
  long cont_904X;
  long template_905X;
  long new_906X;
  long d_907X;
  long new_908X;
  long d_909X;
  long *v_910X;
  long length_911X;
  long new_912X;
  long new_913X;
  long c_914X;
  char op1_915X;
  char op2_916X;
 {semisize_897X = (((-2 & memory_size_894X))<<1);
  Snewspace_beginS = memory_begin_893X;
  Snewspace_endS = (Snewspace_beginS + semisize_897X);
  Soldspace_beginS = Snewspace_endS;
  Soldspace_endS = (Soldspace_beginS + semisize_897X);
  ShpS = Snewspace_beginS;
  SlimitS = Snewspace_endS;
  v_898X = (FILE **)malloc(sizeof(FILE *) * Snumber_of_portsS);
  Sopen_portsS = v_898X;
  v_899X = (long*)malloc(sizeof(long) * Snumber_of_portsS);
  Sopen_vm_portsS = v_899X;
  tempWW3526_900X = NULL == Sopen_portsS;
  if (tempWW3526_900X) {
    goto L13110;}
  else {
    v_901X = NULL == Sopen_vm_portsS;
    if (v_901X) {
      goto L13110;}
    else {
      goto L13115;}}}
 L13110: {
  TTerror("out of memory, unable to continue", 0, 0, 0, 0);
  goto L13115;}
 L13222: {
  i_902X = arg1K0;
  if ((i_902X < length_911X)) {
    *(v_910X + i_902X) = 1;
    arg1K0 = (1 + i_902X);
    goto L13222;}
  else {
    *(Sopen_portsS + 0) = Scurrent_input_portS;
    *(Sopen_portsS + 1) = Scurrent_output_portS;
    d_909X = enter_string("so");
    *((long*)ShpS) = 4118;
    ShpS = (4 + ShpS);
    new_908X = 3 + ShpS;
    ShpS = (16 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_908X)) = 8;
    *((long*)(1 + new_908X)) = 4;
    *((long*)(5 + new_908X)) = 1;
    *((long*)(9 + new_908X)) = d_909X;
    d_907X = enter_string("si");
    *((long*)ShpS) = 4118;
    ShpS = (4 + ShpS);
    new_906X = 3 + ShpS;
    ShpS = (16 + ShpS);
    *((long*)(-4 + ShpS)) = 0;
    *((long*)(-3 + new_906X)) = 4;
    *((long*)(1 + new_906X)) = 0;
    *((long*)(5 + new_906X)) = 1;
    *((long*)(9 + new_906X)) = d_907X;
    *(Sopen_vm_portsS + 0) = new_906X;
    *(Sopen_vm_portsS + 1) = new_908X;
    Sstack_beginS = stack_begin_895X;
    Sstack_endS = (stack_begin_895X + (((stack_size_896X)<<2)));
    Sstack_limitS = Sstack_beginS;
    SstackS = (-4 + Sstack_endS);
    ScontS = 1;
    SenvS = 17;
    merged_arg0K0 = 22;
    merged_arg0K1 = 28;
    make_template_containing_ops_return_tag = 0;
    goto make_template_containing_ops;
   make_template_containing_ops_return_0:
    template_905X = make_template_containing_ops_return_value;
    SstackS = (-20 + SstackS);
    /*** JAR HACK -- IMMUTABLE CONTINUATIONS ***/
    *((long*)(4 + SstackS)) = (4130 | 128);
    cont_904X = 11 + SstackS;
    *((long*)(1 + cont_904X)) = 0;
    *((long*)(5 + cont_904X)) = template_905X;
    *((long*)(9 + cont_904X)) = SenvS;
    *((long*)(-3 + cont_904X)) = ScontS;
    ScontS = cont_904X;
    Sbottom_of_stackS = ScontS;
    merged_arg0K0 = 31;
    merged_arg0K1 = 127;
    make_template_containing_ops_return_tag = 1;
    goto make_template_containing_ops;
   make_template_containing_ops_return_1:
    v_903X = make_template_containing_ops_return_value;
    Sinterrupt_templateS = v_903X;
    return;}}
 L13115: {
  v_910X = Sopen_vm_portsS;
  length_911X = Snumber_of_portsS;
  arg1K0 = 0;
  goto L13222;}
 make_template_containing_ops: {
  op1_915X = merged_arg0K0;
  op2_916X = merged_arg0K1;{
  *((long*)ShpS) = 578;
  ShpS = (4 + ShpS);
  new_912X = 3 + ShpS;
  ShpS = (4 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)ShpS) = 2090;
  ShpS = (4 + ShpS);
  new_913X = 3 + ShpS;
  ShpS = (8 + ShpS);
  *((long*)(-4 + ShpS)) = 0;
  *((long*)(-3 + new_913X)) = new_912X;
  *((unsigned char*)(-3 + new_912X)) = ((unsigned char)((long)op1_915X));
  *((unsigned char*)(-2 + new_912X)) = ((unsigned char)((long)op2_916X));
  make_template_containing_ops_return_value = new_913X;
  goto make_template_containing_ops_return;}
 make_template_containing_ops_return:
  switch (make_template_containing_ops_return_tag) {
  case 0: goto make_template_containing_ops_return_0;
  case 1: goto make_template_containing_ops_return_1;
  }}
}

long required_init_space(unsigned char **startup_vector_917X, long startup_vector_length_918X)
{
  long arg1K1;
  long arg1K0;
  long i_919X;
  long s_920X;
  long length_921X;

 {arg1K0 = 0;
  arg1K1 = 0;
  goto L5397;}
 L5397: {
  i_919X = arg1K0;
  s_920X = arg1K1;
  if ((i_919X < startup_vector_length_918X)) {
    length_921X = strlen((*(startup_vector_917X + i_919X)));
    arg1K0 = (1 + i_919X);
    arg1K1 = (1 + (s_920X + ((((4 + length_921X))>>2))));
    goto L5397;}
  else {
    return((22 + s_920X));}}}

void note_interruptB(char interrupt_922X)
{
  extern long Spending_interruptsS;
  long x_923X;

 {PS_SHIFT_LEFT(1, ((long)interrupt_922X), x_923X)
  Spending_interruptsS = (Spending_interruptsS | x_923X);
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
  long value_924X;
  long scan_925X;
  long value_926X;
  long a_927X;
  unsigned char v_928X;
  long h_929X;
  long start_930X;
  long end_931X;
  long next_932X;
  long value_933X;
  long o_934X;
  long new_935X;
  long x1_936X;
  long v_937X;
  long descriptor_938X;
  unsigned char v_939X;
  long a_940X;
  long h_941X;
  long a_942X;
  unsigned char v_943X;
  long end_944X;
  long x1_945X;
  unsigned char x_946X;
  long addr_947X;
  long next_948X;
  long thing_949X;
  long start_950X;
  long end_951X;

 {arg1K0 = Snewspace_beginS;
  goto L13849;}
 L7763: {
  arg1K0 = (8 + scan_925X);
  goto L7718;}
 L7758: {
  value_924X = arg1K0;
  *((long*)(4 + scan_925X)) = value_924X;
  goto L7763;}
 L7718: {
  scan_925X = arg1K0;
  if ((scan_925X < end_931X)) {
    *((long*)scan_925X) = 1070;
    value_926X = *((long*)(4 + scan_925X));
    if ((3 == (3 & value_926X))) {
      a_927X = -3 + value_926X;
      if ((a_927X < Soldspace_beginS)) {
        goto L7763;}
      else {
        v_928X = a_927X < Soldspace_endS;
        if (v_928X) {
          if ((3 == (3 & value_926X))) {
            h_929X = *((long*)(-7 + value_926X));
            if ((3 == (3 & h_929X))) {
              arg1K0 = h_929X;
              goto L7758;}
            else {
              arg1K0 = 1;
              goto L7758;}}
          else {
            goto L7763;}}
        else {
          goto L7763;}}}
    else {
      goto L7763;}}
  else {
    if ((-1 == next_932X)) {
      if ((end_944X < Sweak_pointer_limitS)) {
        *((long*)end_944X) = (66 + ((((-4 & (-4 + (Sweak_pointer_limitS - end_944X))))<<8)));
        ShpS = RShpS;
        return;}
      else {
        ShpS = RShpS;
        return;}}
    else {
      arg1K0 = (-1024 + next_932X);
      arg1K1 = next_932X;
      goto L11150;}}}
 L11150: {
  start_930X = arg1K0;
  end_931X = arg1K1;
  next_932X = *((long*)(8 + start_930X));
  arg1K0 = start_930X;
  goto L7718;}
 L31993: {
  value_933X = arg1K0;
  *((long*)addr_947X) = value_933X;
  arg1K0 = next_948X;
  goto L13915;}
 L32048: {
  o_934X = arg1K0;
  if ((RShpS < (x1_936X + (-4 & (3 + ((long)(((unsigned long)h_941X)>>8))))))) {
    *((long*)RShpS) = (*((long*)o_934X));
    RShpS = (4 + RShpS);
    arg1K0 = (4 + o_934X);
    goto L32048;}
  else {
    arg1K0 = new_935X;
    goto L31993;}}
 L32032: {
  *((long*)RShpS) = h_941X;
  RShpS = (4 + RShpS);
  new_935X = 3 + RShpS;
  *((long*)(-7 + thing_949X)) = new_935X;
  x1_936X = RShpS;
  arg1K0 = (-3 + thing_949X);
  goto L32048;}
 L31983: {
  if ((3 == (3 & thing_949X))) {
    a_940X = -3 + thing_949X;
    if ((a_940X < Soldspace_beginS)) {
      arg1K0 = next_948X;
      goto L13915;}
    else {
      v_939X = a_940X < Soldspace_endS;
      if (v_939X) {
        h_941X = *((long*)(-7 + thing_949X));
        if ((3 == (3 & h_941X))) {
          arg1K0 = h_941X;
          goto L31993;}
        else {
          if ((1070 == h_941X)) {
            descriptor_938X = *((long*)(-3 + thing_949X));
            if ((3 == (3 & descriptor_938X))) {
              a_942X = -3 + descriptor_938X;
              if ((a_942X < Soldspace_beginS)) {
                goto L32032;}
              else {
                v_943X = a_942X < Soldspace_endS;
                if (v_943X) {
                  ShpS = RShpS;
                  v_937X = copy_weak_pointer(thing_949X);
                  RShpS = ShpS;
                  arg1K0 = v_937X;
                  goto L31993;}
                else {
                  goto L32032;}}}
            else {
              goto L32032;}}
          else {
            goto L32032;}}}
      else {
        arg1K0 = next_948X;
        goto L13915;}}}
  else {
    arg1K0 = next_948X;
    goto L13915;}}
 L13875: {
  x_946X = -1 == Sweak_pointer_hpS;
  if (x_946X) {
    ShpS = RShpS;
    return;}
  else {
    x1_945X = Sweak_pointer_limitS;
    end_944X = Sweak_pointer_hpS;
    arg1K0 = (-1024 + x1_945X);
    arg1K1 = end_944X;
    goto L11150;}}
 L13915: {
  addr_947X = arg1K0;
  if ((addr_947X < end_951X)) {
    next_948X = 4 + addr_947X;
    thing_949X = *((long*)addr_947X);
    if ((2 == (3 & thing_949X))) {
      if (((31 & (((thing_949X)>>2))) < 15)) {
        goto L31983;}
      else {
        arg1K0 = (next_948X + (-4 & (3 + ((long)(((unsigned long)thing_949X)>>8)))));
        goto L13915;}}
    else {
      goto L31983;}}
  else {
    if ((RShpS < SlimitS)) {
      if ((end_951X < RShpS)) {
        arg1K0 = end_951X;
        goto L13849;}
      else {
        goto L13875;}}
    else {
      TTerror("GC error: ran out of space in new heap", 0, 0, 0, 0);
      goto L13875;}}}
 L13849: {
  start_950X = arg1K0;
  end_951X = RShpS;
  arg1K0 = start_950X;
  goto L13915;}}

long read_image(unsigned char *filename_952X, long startup_space_953X)
{
  extern long read_number(FILE *);
  extern long Snewspace_endS;
  extern FILE * Scurrent_output_portS;
  extern long ShpS;
  long arg1K0;
  unsigned char arg3K0;
  FILE * port_954X;
  unsigned char reverseP_955X;
  long v_956X;
  long ptr_957X;
  long value_958X;
  unsigned char x_959X;
  unsigned char x_960X;
  long d_961X;
  long v_962X;
  long ptr_963X;
  long descriptor_964X;
  long result_965X;
  long old_level_966X;
  long old_bytes_per_cell_967X;
  long cells_968X;
  long old_begin_969X;
  long cells_970X;
  long old_hp_971X;
  long startup_proc_972X;
  unsigned char Kchar_973X;
  unsigned char Kchar_974X;
  long addr_975X;
  unsigned char x_976X;
  unsigned char x_977X;
  long start_978X;
  long new_limit_979X;
  long new_hp_980X;
  long delta_981X;

 {port_954X = ps_open(filename_952X, "r");
  if ((NULL == port_954X)) {
    TTerror("Can't open heap image file", 0, 0, 0, 0);
    goto L10288;}
  else {
    goto L10288;}}
 L10348: {
  reverseP_955X = arg3K0;
  fread((void *)ShpS, sizeof(char), (old_hp_971X - old_begin_969X), port_954X);
  if (reverseP_955X) {
    fputs("Correcting byte order of resumed image.", Scurrent_output_portS);
    putc(10, Scurrent_output_portS);
    arg1K0 = start_978X;
    goto L8615;}
  else {
    goto L10358;}}
 L8641: {
  v_956X = arg1K0;
  arg1K0 = v_956X;
  goto L8615;}
 L8615: {
  ptr_957X = arg1K0;
  x_960X = *((unsigned char*)ptr_957X);
  *((unsigned char*)ptr_957X) = ((unsigned char)((long)(*((unsigned char*)(3 + ptr_957X)))));
  *((unsigned char*)(3 + ptr_957X)) = ((unsigned char)((long)x_960X));
  x_959X = *((unsigned char*)(1 + ptr_957X));
  *((unsigned char*)(1 + ptr_957X)) = ((unsigned char)((long)(*((unsigned char*)(2 + ptr_957X)))));
  *((unsigned char*)(2 + ptr_957X)) = ((unsigned char)((long)x_959X));
  value_958X = *((long*)ptr_957X);
  if ((ptr_957X < new_hp_980X)) {
    if ((2 == (3 & value_958X))) {
      if (((31 & (((value_958X)>>2))) < 15)) {
        arg1K0 = (4 + ptr_957X);
        goto L8641;}
      else {
        arg1K0 = ((4 + ptr_957X) + (-4 & (3 + ((long)(((unsigned long)value_958X)>>8)))));
        goto L8641;}}
    else {
      arg1K0 = (4 + ptr_957X);
      goto L8641;}}
  else {
    goto L10358;}}
 L8740: {
  d_961X = arg1K0;
  *((long*)ptr_963X) = d_961X;
  if ((2 == (3 & d_961X))) {
    if (((31 & (((d_961X)>>2))) < 15)) {
      arg1K0 = (4 + ptr_963X);
      goto L8758;}
    else {
      arg1K0 = ((4 + ptr_963X) + (-4 & (3 + ((long)(((unsigned long)d_961X)>>8)))));
      goto L8758;}}
  else {
    arg1K0 = (4 + ptr_963X);
    goto L8758;}}
 L8758: {
  v_962X = arg1K0;
  arg1K0 = v_962X;
  goto L8730;}
 L8730: {
  ptr_963X = arg1K0;
  if ((ptr_963X < new_hp_980X)) {
    descriptor_964X = *((long*)ptr_963X);
    if ((3 == (3 & descriptor_964X))) {
      arg1K0 = (3 + ((-3 + descriptor_964X) + delta_981X));
      goto L8740;}
    else {
      arg1K0 = descriptor_964X;
      goto L8740;}}
  else {
    goto L10368;}}
 L10538: {
  result_965X = arg1K0;
  fclose(port_954X);
  return(result_965X);}
 L10290: {
  old_level_966X = read_number(port_954X);
  old_bytes_per_cell_967X = read_number(port_954X);
  cells_968X = read_number(port_954X);
  old_begin_969X = ((cells_968X)<<2);
  cells_970X = read_number(port_954X);
  old_hp_971X = ((cells_970X)<<2);
  startup_proc_972X = read_number(port_954X);
  goto L10489;}
 L10514: {
  { int TTchar;
    PS_GETC(port_954X, TTchar);
    if (EOF == TTchar) {
      TTerror("end of file while looking for page break", 0, 0, 0, 0);
      goto L10290;}
    else {
      unsigned char Kchar_973X = TTchar;
      if ((12 == ((long)Kchar_973X))) {
        goto L10290;}
      else {
        goto L10514;}}}}
 L10311: {
  if ((15 == old_level_966X)) {
    goto L10321;}
  else {
    TTerror("format of image is incompatible with this version of system", 2, old_level_966X, 15, 0);
    goto L10321;}}
 L10489: {
  { int TTchar;
    PS_GETC(port_954X, TTchar);
    if (EOF == TTchar) {
      TTerror("end of file while looking for page break", 0, 0, 0, 0);
      goto L10311;}
    else {
      unsigned char Kchar_974X = TTchar;
      if ((12 == ((long)Kchar_974X))) {
        goto L10311;}
      else {
        goto L10489;}}}}
 L10368: {
  ShpS = new_hp_980X;
  if ((3 == (3 & startup_proc_972X))) {
    arg1K0 = (3 + ((-3 + startup_proc_972X) + delta_981X));
    goto L10538;}
  else {
    arg1K0 = startup_proc_972X;
    goto L10538;}}
 L10358: {
  if ((0 == delta_981X)) {
    goto L10368;}
  else {
    arg1K0 = start_978X;
    goto L8730;}}
 L10345: {
  fread((void *)ShpS, sizeof(char), 4, port_954X);
  if ((1 == (*((long*)ShpS)))) {
    arg3K0 = 0;
    goto L10348;}
  else {
    addr_975X = ShpS;
    x_976X = *((unsigned char*)addr_975X);
    *((unsigned char*)addr_975X) = ((unsigned char)((long)(*((unsigned char*)(3 + addr_975X)))));
    *((unsigned char*)(3 + addr_975X)) = ((unsigned char)((long)x_976X));
    x_977X = *((unsigned char*)(1 + addr_975X));
    *((unsigned char*)(1 + addr_975X)) = ((unsigned char)((long)(*((unsigned char*)(2 + addr_975X)))));
    *((unsigned char*)(2 + addr_975X)) = ((unsigned char)((long)x_977X));
    if ((1 == (*((long*)ShpS)))) {
      arg3K0 = 1;
      goto L10348;}
    else {
      TTerror("unable to correct byte order", 1, (*((long*)ShpS)), 0, 0);
      arg3K0 = 0;
      goto L10348;}}}
 L10331: {
  delta_981X = ShpS - old_begin_969X;
  new_hp_980X = old_hp_971X + delta_981X;
  new_limit_979X = Snewspace_endS;
  start_978X = ShpS;
  if (((startup_space_953X + new_hp_980X) < new_limit_979X)) {
    goto L10345;}
  else {
    TTerror("heap not big enough to restore this image", 2, new_hp_980X, new_limit_979X, 0);
    goto L10345;}}
 L10321: {
  if ((4 == old_bytes_per_cell_967X)) {
    goto L10331;}
  else {
    TTerror("incompatible bytes-per-cell", 2, old_bytes_per_cell_967X, 4, 0);
    goto L10331;}}
 L10288: {
  goto L10514;}}

long check_image_header(unsigned char *filename_982X)
{
  extern long read_number(FILE *);
  extern FILE * Scurrent_output_portS;
  long arg1K0;
  FILE * port_983X;
  FILE * out_984X;
  long result_985X;
  FILE * out_986X;
  long old_level_987X;
  long old_bytes_per_cell_988X;
  long cells_989X;
  long cells_990X;
  FILE * out_991X;
  unsigned char Kchar_992X;

 {port_983X = ps_open(filename_982X, "r");
  if ((NULL == port_983X)) {
    out_984X = Scurrent_output_portS;
    fputs("Can't open heap image file", out_984X);
    putc(10, out_984X);
    arg1K0 = -1;
    goto L6566;}
  else {
    goto L6522;}}
 L6566: {
  result_985X = arg1K0;
  fclose(port_983X);
  return(result_985X);}
 L6428: {
  old_level_987X = read_number(port_983X);
  old_bytes_per_cell_988X = read_number(port_983X);
  cells_989X = read_number(port_983X);
  cells_990X = read_number(port_983X);
  if ((15 == old_level_987X)) {
    if ((4 == old_bytes_per_cell_988X)) {
      arg1K0 = ((((cells_990X)<<2)) - (((cells_989X)<<2)));
      goto L6566;}
    else {
      out_986X = Scurrent_output_portS;
      fputs("incompatible bytes-per-cell in image", out_986X);
      putc(10, out_986X);
      arg1K0 = -1;
      goto L6566;}}
  else {
    out_991X = Scurrent_output_portS;
    fputs("format of image is incompatible with this version of system", out_991X);
    putc(10, out_991X);
    arg1K0 = -1;
    goto L6566;}}
 L6522: {
  { int TTchar;
    PS_GETC(port_983X, TTchar);
    if (EOF == TTchar) {
      TTerror("end of file while looking for page break", 0, 0, 0, 0);
      goto L6428;}
    else {
      unsigned char Kchar_992X = TTchar;
      if ((12 == ((long)Kchar_992X))) {
        goto L6428;}
      else {
        goto L6522;}}}}}

void register_static_areas(char pure_count_993X, long *pure_areas_994X, long *pure_sizes_995X, char impure_count_996X, long *impure_areas_997X, long *impure_sizes_998X)
{
  extern char Spure_area_countS;
  extern long *Spure_areasS;
  extern long *Spure_sizesS;
  extern char Simpure_area_countS;
  extern long *Simpure_areasS;
  extern long *Simpure_sizesS;

 {Spure_area_countS = pure_count_993X;
  Spure_areasS = pure_areas_994X;
  Spure_sizesS = pure_sizes_995X;
  Simpure_area_countS = impure_count_996X;
  Simpure_areasS = impure_areas_997X;
  Simpure_sizesS = impure_sizes_998X;
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
