#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"
#include "scheme48heap.h"

long s48_write_image(long, long, FILE *);
static long Sundumpable_recordsS;
static long Sundumpable_countS;
static char * Sfrom_beginS;
static char * Sfrom_endS;
static char * Sto_beginS;
static char * Sto_endS;
static char * ShpS;
static long SstatusS;

long s48_write_image(long resume_proc_0X, long undumpables_1X, FILE * port_2X)
{
  char * arg1K1;
  char * arg1K0;
  long arg0K1;
  long arg0K0;
  char * merged_arg1K1;
  long merged_arg0K0;

  int real_copy_object_return_tag;
  long real_copy_object0_return_value;
  char * real_copy_object1_return_value;
  int gc_recordP_return_tag;
  char gc_recordP0_return_value;
  int note_undumpableB_return_tag;
  int trace_image_value_return_tag;
  long trace_image_value0_return_value;
  long thing_3X;
  char * frontier_4X;
  long x_5X;
  long thing_6X;
  long stob_7X;
  char * new_frontier_127X;
  long new_126X;
  char * data_addr_125X;
  long h_124X;
  long header_123X;
  char * addr_122X;
  long x_121X;
  long i_120X;
  long len_119X;
  long vector_118X;
  char * new_hp_117X;
  long new_thing_116X;
  char * a_115X;
  char v_114X;
  long type_113X;
  char v_112X;
  long stob_111X;
  long h_110X;
  long h_109X;
  long d_108X;
  char * addr_107X;
  long status_106X;
  long v_105X;
  long v_104X;
  long v_103X;
  long v_102X;
  long v_101X;
  long v_100X;
  long v_99X;
  char * v_98X;
  long v_97X;
  char * v_96X;
  long v_95X;
  long v_94X;
  long v_93X;
  long h_92X;
  long v_91X;
  long h_90X;
  long d_89X;
  char * addr_88X;
  char * addr_87X;
  long next_link_86X;
  long new_foo_85X;
  long v_84X;
  long resumer_records_83X;
  char * addr_82X;
  long next_link_81X;
  long new_foo_80X;
  char * a_79X;
  char * addr_78X;
  long foo_77X;
  long link_76X;
  char * addr_75X;
  long entry_74X;
  long type_73X;
  long record_72X;
  char * next_71X;
  long header_70X;
  char * addr_69X;
  char * a_68X;
  char * addr_67X;
  long foo_66X;
  long okay_link_65X;
  long foo_link_64X;
  long link_63X;
  char * end_62X;
  char * start_hp_61X;
  long i_60X;
  char * addr_59X;
  char * addr_58X;
  long okay_link_57X;
  long foo_link_56X;
  long foo_link_55X;
  long i_54X;
  long id_53X;
  long channel_52X;
  long foo_link_51X;
  long table_50X;
  long imported_bindings_49X;
  long i_48X;
  char * a_47X;
  long channel_46X;
  long table_45X;
  long table_44X;
  long i_43X;
  long x2_42X;
  long *channels_41X;
  long count_40X;
  char * a_39X;
  long contents_38X;
  long header_37X;
  char * addr_36X;
  char * frontier_35X;
  long new_thing_34X;
  char * a_33X;
  char * addr_32X;
  long next_link_31X;
  long new_foo_30X;
  char * addr_29X;
  long foo_28X;
  char v_27X;
  long type_26X;
  char v_25X;
  char * next_24X;
  long thing_23X;
  char * frontier_22X;
  char * addr_21X;
  long done_link_20X;
  long foo_link_19X;
  char * end_18X;
  char * start_17X;
  long foo_link_16X;
  long resume_proc_15X;
  long i_14X;
  long table_13X;
  long table_12X;
  char * v_11X;
  char * v_10X;
  char * v_9X;
  char * v_8X;
 {  v_8X = s48_heap_begin();
  Sfrom_beginS = v_8X;
  v_9X = s48_heap_pointer();
  Sfrom_endS = v_9X;
  v_10X = s48_oldspace_begin();
  Sto_beginS = v_10X;
  v_11X = s48_oldspace_end();
  Sto_endS = v_11X;
  ShpS = (Sto_beginS);
  Sundumpable_recordsS = undumpables_1X;
  Sundumpable_countS = 0;
  table_12X = s48_exported_bindings();
  merged_arg0K0 = table_12X;
  trace_image_value_return_tag = 0;
  goto trace_image_value;
 trace_image_value_return_0:
  table_13X = trace_image_value0_return_value;
  arg0K0 = 0;
  goto L3974;}
 L3974: {
  i_14X = arg0K0;
  if ((1024 == i_14X)) {
    merged_arg0K0 = resume_proc_0X;
    trace_image_value_return_tag = 1;
    goto trace_image_value;
   trace_image_value_return_1:
    resume_proc_15X = trace_image_value0_return_value;
    arg1K0 = (Sto_beginS);
    goto L3992;}
  else {
    foo_link_16X = *((long *) ((((char *) (-3 + table_13X))) + (((i_14X)<<2))));
    arg0K0 = foo_link_16X;
    arg0K1 = 1;
    goto L3407;}}
 L3992: {
  start_17X = arg1K0;
  end_18X = ShpS;
  arg1K0 = start_17X;
  arg1K1 = (ShpS);
  goto L2862;}
 L3407: {
  foo_link_19X = arg0K0;
  done_link_20X = arg0K1;
  if ((0 == (3 & foo_link_19X))) {
    arg0K0 = (3 + (-4 & foo_link_19X));
    goto L3409;}
  else {
    arg0K0 = foo_link_19X;
    goto L3409;}}
 L2862: {
  addr_21X = arg1K0;
  frontier_22X = arg1K1;
  if ((addr_21X < end_18X)) {
    thing_23X = *((long *) addr_21X);
    next_24X = addr_21X + 4;
    if ((2 == (3 & thing_23X))) {
      if ((2 == (3 & thing_23X))) {
        if (((31 & (((thing_23X)>>2))) < 17)) {
          goto L2883;}
        else {
          goto L2888;}}
      else {
        goto L2883;}}
    else {
      merged_arg0K0 = thing_23X;
      gc_recordP_return_tag = 0;
      goto gc_recordP;
     gc_recordP_return_0:
      v_25X = gc_recordP0_return_value;
      if (v_25X) {
        type_26X = *((long *) (((char *) (-3 + thing_23X))));
        merged_arg0K0 = type_26X;
        gc_recordP_return_tag = 1;
        goto gc_recordP;
       gc_recordP_return_1:
        v_27X = gc_recordP0_return_value;
        if (v_27X) {
          if ((1 == (*((long *) ((((char *) (-3 + type_26X))) + 4))))) {
            merged_arg0K0 = thing_23X;
            note_undumpableB_return_tag = 0;
            goto note_undumpableB;
           note_undumpableB_return_0:
            *((long *) addr_21X) = (*((long *) ((((char *) (-3 + thing_23X))) + 4)));
            arg1K0 = addr_21X;
            arg1K1 = frontier_22X;
            goto L2862;}
          else {
            goto L2904;}}
        else {
          goto L2904;}}
      else {
        goto L2904;}}}
  else {
    ShpS = frontier_22X;
    if (((ShpS) < (Sto_endS))) {
      if ((end_18X < (ShpS))) {
        arg1K0 = end_18X;
        goto L3992;}
      else {
        goto L2072;}}
    else {
      ps_error("Image writing error: ran out of space in new heap", 0);
      goto L2072;}}}
 L3409: {
  foo_28X = arg0K0;
  if ((1 == foo_28X)) {
    addr_29X = (((char *) (-3 + table_13X))) + (((i_14X)<<2));
    S48_WRITE_BARRIER(table_13X, addr_29X, done_link_20X);
    *((long *) addr_29X) = done_link_20X;
    arg0K0 = (1 + i_14X);
    goto L3974;}
  else {
    merged_arg0K0 = foo_28X;
    trace_image_value_return_tag = 2;
    goto trace_image_value;
   trace_image_value_return_2:
    new_foo_30X = trace_image_value0_return_value;
    next_link_31X = *((long *) ((((char *) (-3 + new_foo_30X))) + 12));
    addr_32X = (((char *) (-3 + new_foo_30X))) + 12;
    S48_WRITE_BARRIER(new_foo_30X, addr_32X, done_link_20X);
    *((long *) addr_32X) = done_link_20X;
    if ((3 == (3 & new_foo_30X))) {
      arg0K0 = next_link_31X;
      arg0K1 = (-4 & new_foo_30X);
      goto L3407;}
    else {
      arg0K0 = next_link_31X;
      arg0K1 = new_foo_30X;
      goto L3407;}}}
 L2883: {
  if ((1078 == thing_23X)) {
    goto L2888;}
  else {
    arg1K0 = next_24X;
    arg1K1 = frontier_22X;
    goto L2862;}}
 L2888: {
  arg1K0 = (next_24X + (-4 & (3 + ((long)(((unsigned long)thing_23X)>>8)))));
  arg1K1 = frontier_22X;
  goto L2862;}
 L2904: {
  if ((3 == (3 & thing_23X))) {
    a_33X = ((char *) (-3 + thing_23X));
    if ((a_33X < (Sfrom_beginS))) {
      arg1K0 = next_24X;
      arg1K1 = frontier_22X;
      goto L2862;}
    else {
      if ((a_33X < (Sfrom_endS))) {
        merged_arg0K0 = thing_23X;
        merged_arg1K1 = frontier_22X;
        real_copy_object_return_tag = 0;
        goto real_copy_object;
       real_copy_object_return_0:
        new_thing_34X = real_copy_object0_return_value;
        frontier_35X = real_copy_object1_return_value;
        *((long *) addr_21X) = new_thing_34X;
        arg1K0 = next_24X;
        arg1K1 = frontier_35X;
        goto L2862;}
      else {
        arg1K0 = next_24X;
        arg1K1 = frontier_22X;
        goto L2862;}}}
  else {
    arg1K0 = next_24X;
    arg1K1 = frontier_22X;
    goto L2862;}}
 L2072: {
  arg1K0 = (Sto_beginS);
  goto L2076;}
 L2076: {
  addr_36X = arg1K0;
  if ((addr_36X < (ShpS))) {
    header_37X = *((long *) addr_36X);
    if ((1078 == header_37X)) {
      contents_38X = *((long *) (addr_36X + 4));
      if ((3 == (3 & contents_38X))) {
        if ((3 == (3 & contents_38X))) {
          a_39X = ((char *) (-3 + contents_38X));
          if ((a_39X < (Sfrom_beginS))) {
            arg0K0 = 1;
            goto L2119;}
          else {
            if ((a_39X < (Sfrom_endS))) {
              if ((3 == (3 & (*((long *) ((((char *) (-3 + contents_38X))) + -4)))))) {
                arg0K0 = (*((long *) ((((char *) (-3 + contents_38X))) + -4)));
                goto L2119;}
              else {
                arg0K0 = 1;
                goto L2119;}}
            else {
              arg0K0 = 1;
              goto L2119;}}}
        else {
          arg0K0 = 1;
          goto L2119;}}
      else {
        goto L2127;}}
    else {
      goto L2127;}}
  else {
    count_40X = s48_channel_count();
    channels_41X = s48_channels();
    arg0K0 = 0;
    goto L3678;}}
 L2119: {
  x2_42X = arg0K0;
  *((long *) (addr_36X + 4)) = x2_42X;
  goto L2127;}
 L2127: {
  arg1K0 = (addr_36X + (4 + (-4 & (3 + ((long)(((unsigned long)header_37X)>>8))))));
  goto L2076;}
 L3678: {
  i_43X = arg0K0;
  if ((i_43X == count_40X)) {
    table_44X = s48_symbol_table();
    merged_arg0K0 = table_44X;
    trace_image_value_return_tag = 3;
    goto trace_image_value;
   trace_image_value_return_3:
    table_45X = trace_image_value0_return_value;
    arg0K0 = 0;
    goto L4017;}
  else {
    channel_46X = *(channels_41X + i_43X);
    if ((1 == channel_46X)) {
      goto L3706;}
    else {
      if ((3 == (3 & channel_46X))) {
        if ((3 == (3 & channel_46X))) {
          a_47X = ((char *) (-3 + channel_46X));
          if ((a_47X < (Sto_beginS))) {
            goto L3729;}
          else {
            if ((a_47X < (ShpS))) {
              goto L3699;}
            else {
              goto L3729;}}}
        else {
          goto L3729;}}
      else {
        goto L3699;}}}}
 L4017: {
  i_48X = arg0K0;
  if ((1024 == i_48X)) {
    imported_bindings_49X = s48_imported_bindings();
    merged_arg0K0 = imported_bindings_49X;
    trace_image_value_return_tag = 4;
    goto trace_image_value;
   trace_image_value_return_4:
    table_50X = trace_image_value0_return_value;
    arg0K0 = 0;
    goto L4038;}
  else {
    foo_link_51X = *((long *) ((((char *) (-3 + table_45X))) + (((i_48X)<<2))));
    arg0K0 = foo_link_51X;
    arg0K1 = 1;
    goto L3289;}}
 L3706: {
  arg0K0 = (1 + i_43X);
  goto L3678;}
 L3729: {
  if ((3 == (3 & (*((long *) ((((char *) (-3 + channel_46X))) + -4)))))) {
    goto L3699;}
  else {
    goto L3706;}}
 L3699: {
  merged_arg0K0 = channel_46X;
  trace_image_value_return_tag = 5;
  goto trace_image_value;
 trace_image_value_return_5:
  channel_52X = trace_image_value0_return_value;
  ps_write_string("Channel closed in dumped image: ", (stderr));
  id_53X = *((long *) ((((char *) (-3 + channel_52X))) + 4));
  if ((0 == (3 & id_53X))) {
    ps_write_integer((((id_53X)>>2)), (stderr));
    goto L1786;}
  else {
    ps_write_string((((char *)(((char *) (-3 + id_53X))))), (stderr));
    goto L1786;}}
 L4038: {
  i_54X = arg0K0;
  if ((1024 == i_54X)) {
    arg0K0 = 0;
    goto L2356;}
  else {
    foo_link_55X = *((long *) ((((char *) (-3 + table_50X))) + (((i_54X)<<2))));
    arg0K0 = foo_link_55X;
    arg0K1 = 1;
    goto L3170;}}
 L3289: {
  foo_link_56X = arg0K0;
  okay_link_57X = arg0K1;
  if ((0 == (3 & foo_link_56X))) {
    arg0K0 = (3 + (-4 & foo_link_56X));
    goto L3291;}
  else {
    arg0K0 = foo_link_56X;
    goto L3291;}}
 L1786: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  addr_58X = ((char *) (-3 + channel_52X));
  S48_WRITE_BARRIER(channel_52X, addr_58X, 0);
  *((long *) addr_58X) = 0;
  addr_59X = (((char *) (-3 + channel_52X))) + 8;
  S48_WRITE_BARRIER(channel_52X, addr_59X, -4);
  *((long *) addr_59X) = -4;
  goto L3706;}
 L2356: {
  i_60X = arg0K0;
  if ((1024 == i_60X)) {
    start_hp_61X = ShpS;
    *((long *) (ShpS)) = 0;
    end_62X = ShpS;
    arg1K0 = (Sto_beginS);
    goto L2470;}
  else {
    link_63X = *((long *) ((((char *) (-3 + table_50X))) + (((i_60X)<<2))));
    if ((0 == (3 & link_63X))) {
      arg0K0 = (3 + (-4 & link_63X));
      goto L2330;}
    else {
      arg0K0 = link_63X;
      goto L2330;}}}
 L3170: {
  foo_link_64X = arg0K0;
  okay_link_65X = arg0K1;
  if ((0 == (3 & foo_link_64X))) {
    arg0K0 = (3 + (-4 & foo_link_64X));
    goto L3172;}
  else {
    arg0K0 = foo_link_64X;
    goto L3172;}}
 L3291: {
  foo_66X = arg0K0;
  if ((1 == foo_66X)) {
    addr_67X = (((char *) (-3 + table_45X))) + (((i_48X)<<2));
    S48_WRITE_BARRIER(table_45X, addr_67X, okay_link_57X);
    *((long *) addr_67X) = okay_link_57X;
    arg0K0 = (1 + i_48X);
    goto L4017;}
  else {
    if ((3 == (3 & foo_66X))) {
      if ((3 == (3 & foo_66X))) {
        a_68X = ((char *) (-3 + foo_66X));
        if ((a_68X < (Sto_beginS))) {
          goto L3332;}
        else {
          if ((a_68X < (ShpS))) {
            goto L3300;}
          else {
            goto L3332;}}}
      else {
        goto L3332;}}
    else {
      goto L3300;}}}
 L2470: {
  addr_69X = arg1K0;
  if ((addr_69X < end_62X)) {
    header_70X = *((long *) addr_69X);
    next_71X = addr_69X + (4 + (-4 & (3 + ((long)(((unsigned long)header_70X)>>8)))));
    if ((2 == (3 & header_70X))) {
      if ((9 == (31 & (((header_70X)>>2))))) {
        record_72X = 3 + (((long) (addr_69X + 4)));
        type_73X = *((long *) (((char *) (-3 + record_72X))));
        if ((3 == (3 & type_73X))) {
          if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_73X))) + -4))))>>2))))) {
            if ((3 == (3 & (*((long *) ((((char *) (-3 + type_73X))) + 4)))))) {
              if ((4 < ((Sto_endS) - (ShpS)))) {
                *((long *) (ShpS)) = record_72X;
                ShpS = ((ShpS) + 4);
                arg1K0 = next_71X;
                goto L2470;}
              else {
                goto L4069;}}
            else {
              arg1K0 = next_71X;
              goto L2470;}}
          else {
            arg1K0 = next_71X;
            goto L2470;}}
        else {
          arg1K0 = next_71X;
          goto L2470;}}
      else {
        arg1K0 = next_71X;
        goto L2470;}}
    else {
      ps_write_string("new image is not in a consistent state", (stderr));
      goto L4069;}}
  else {
    *((long *) start_hp_61X) = (10 + (((((ShpS) - (start_hp_61X + 4)))<<8)));
    arg0K0 = (3 + (((long) (start_hp_61X + 4))));
    goto L3930;}}
 L2330: {
  entry_74X = arg0K0;
  if ((1 == entry_74X)) {
    arg0K0 = (1 + i_60X);
    goto L2356;}
  else {
    addr_75X = (((char *) (-3 + entry_74X))) + 8;
    S48_WRITE_BARRIER(entry_74X, addr_75X, 529);
    *((long *) addr_75X) = 529;
    link_76X = *((long *) ((((char *) (-3 + entry_74X))) + 12));
    if ((0 == (3 & link_76X))) {
      arg0K0 = (3 + (-4 & link_76X));
      goto L2330;}
    else {
      arg0K0 = link_76X;
      goto L2330;}}}
 L3172: {
  foo_77X = arg0K0;
  if ((1 == foo_77X)) {
    addr_78X = (((char *) (-3 + table_50X))) + (((i_54X)<<2));
    S48_WRITE_BARRIER(table_50X, addr_78X, okay_link_65X);
    *((long *) addr_78X) = okay_link_65X;
    arg0K0 = (1 + i_54X);
    goto L4038;}
  else {
    if ((3 == (3 & foo_77X))) {
      if ((3 == (3 & foo_77X))) {
        a_79X = ((char *) (-3 + foo_77X));
        if ((a_79X < (Sto_beginS))) {
          goto L3213;}
        else {
          if ((a_79X < (ShpS))) {
            goto L3181;}
          else {
            goto L3213;}}}
      else {
        goto L3213;}}
    else {
      goto L3181;}}}
 L3332: {
  if ((3 == (3 & (*((long *) ((((char *) (-3 + foo_66X))) + -4)))))) {
    goto L3300;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + foo_66X))) + 4)));
    arg0K1 = okay_link_57X;
    goto L3289;}}
 L3300: {
  merged_arg0K0 = foo_66X;
  trace_image_value_return_tag = 6;
  goto trace_image_value;
 trace_image_value_return_6:
  new_foo_80X = trace_image_value0_return_value;
  next_link_81X = *((long *) ((((char *) (-3 + new_foo_80X))) + 4));
  addr_82X = (((char *) (-3 + new_foo_80X))) + 4;
  S48_WRITE_BARRIER(new_foo_80X, addr_82X, okay_link_57X);
  *((long *) addr_82X) = okay_link_57X;
  if ((3 == (3 & new_foo_80X))) {
    arg0K0 = next_link_81X;
    arg0K1 = (-4 & new_foo_80X);
    goto L3289;}
  else {
    arg0K0 = next_link_81X;
    arg0K1 = new_foo_80X;
    goto L3289;}}
 L4069: {
  ShpS = start_hp_61X;
  arg0K0 = 1;
  goto L3930;}
 L3930: {
  resumer_records_83X = arg0K0;
  if ((1 == resumer_records_83X)) {
    arg1K0 = (Sfrom_beginS);
    goto L4083;}
  else {
    SstatusS = NO_ERRORS;
    if (((SstatusS) == NO_ERRORS)) {
      PS_WRITE_CHAR(10, port_2X, v_84X)
      SstatusS = v_84X;
      goto L1121;}
    else {
      goto L1121;}}}
 L3213: {
  if ((3 == (3 & (*((long *) ((((char *) (-3 + foo_77X))) + -4)))))) {
    goto L3181;}
  else {
    arg0K0 = (*((long *) ((((char *) (-3 + foo_77X))) + 12)));
    arg0K1 = okay_link_65X;
    goto L3170;}}
 L3181: {
  merged_arg0K0 = foo_77X;
  trace_image_value_return_tag = 7;
  goto trace_image_value;
 trace_image_value_return_7:
  new_foo_85X = trace_image_value0_return_value;
  next_link_86X = *((long *) ((((char *) (-3 + new_foo_85X))) + 12));
  addr_87X = (((char *) (-3 + new_foo_85X))) + 12;
  S48_WRITE_BARRIER(new_foo_85X, addr_87X, okay_link_65X);
  *((long *) addr_87X) = okay_link_65X;
  if ((3 == (3 & new_foo_85X))) {
    arg0K0 = next_link_86X;
    arg0K1 = (-4 & new_foo_85X);
    goto L3170;}
  else {
    arg0K0 = next_link_86X;
    arg0K1 = new_foo_85X;
    goto L3170;}}
 L4083: {
  addr_88X = arg1K0;
  if ((addr_88X < (Sfrom_endS))) {
    d_89X = *((long *) addr_88X);
    if ((2 == (3 & d_89X))) {
      arg0K0 = d_89X;
      goto L4089;}
    else {
      h_90X = *((long *) ((((char *) (-3 + d_89X))) + -4));
      *((long *) addr_88X) = h_90X;
      arg0K0 = h_90X;
      goto L4089;}}
  else {
    return ENOMEM;}}
 L1121: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_2X, v_91X)
    SstatusS = v_91X;
    goto L1130;}
  else {
    goto L1130;}}
 L4089: {
  h_92X = arg0K0;
  arg1K0 = (addr_88X + (4 + (-4 & (3 + ((long)(((unsigned long)h_92X)>>8))))));
  goto L4083;}
 L1130: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_93X)
    SstatusS = v_93X;
    goto L1139;}
  else {
    goto L1139;}}
 L1139: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string("Vanilla 22", port_2X));
    goto L1146;}
  else {
    goto L1146;}}
 L1146: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_94X)
    SstatusS = v_94X;
    goto L1155;}
  else {
    goto L1155;}}
 L1155: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, port_2X));
    goto L1225;}
  else {
    goto L1225;}}
 L1225: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_95X)
    SstatusS = v_95X;
    goto L1157;}
  else {
    goto L1157;}}
 L1157: {
  v_96X = Sto_beginS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(((((((long) v_96X)))>>2)), port_2X));
    goto L1245;}
  else {
    goto L1245;}}
 L1245: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_97X)
    SstatusS = v_97X;
    goto L1163;}
  else {
    goto L1163;}}
 L1163: {
  v_98X = ShpS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(((((((long) v_98X)))>>2)), port_2X));
    goto L1265;}
  else {
    goto L1265;}}
 L1265: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_99X)
    SstatusS = v_99X;
    goto L1169;}
  else {
    goto L1169;}}
 L1169: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(table_45X, port_2X));
    goto L1280;}
  else {
    goto L1280;}}
 L1280: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_100X)
    SstatusS = v_100X;
    goto L1171;}
  else {
    goto L1171;}}
 L1171: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(table_50X, port_2X));
    goto L1295;}
  else {
    goto L1295;}}
 L1295: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_101X)
    SstatusS = v_101X;
    goto L1173;}
  else {
    goto L1173;}}
 L1173: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(table_13X, port_2X));
    goto L1310;}
  else {
    goto L1310;}}
 L1310: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_102X)
    SstatusS = v_102X;
    goto L1175;}
  else {
    goto L1175;}}
 L1175: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resumer_records_83X, port_2X));
    goto L1325;}
  else {
    goto L1325;}}
 L1325: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_103X)
    SstatusS = v_103X;
    goto L1177;}
  else {
    goto L1177;}}
 L1177: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_15X, port_2X));
    goto L1340;}
  else {
    goto L1340;}}
 L1340: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_2X, v_104X)
    SstatusS = v_104X;
    goto L1179;}
  else {
    goto L1179;}}
 L1179: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, port_2X, v_105X)
    SstatusS = v_105X;
    goto L1188;}
  else {
    goto L1188;}}
 L1188: {
  *((long *) (ShpS)) = 1;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_2X, ((char *) (ShpS)), (((ShpS) + 4) - (ShpS))));
    goto L1207;}
  else {
    goto L1207;}}
 L1207: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block(port_2X, ((char *) (Sto_beginS)), ((ShpS) - (Sto_beginS))));
    goto L1220;}
  else {
    goto L1220;}}
 L1220: {
  status_106X = SstatusS;
  arg1K0 = (Sfrom_beginS);
  goto L4105;}
 L4105: {
  addr_107X = arg1K0;
  if ((addr_107X < (Sfrom_endS))) {
    d_108X = *((long *) addr_107X);
    if ((2 == (3 & d_108X))) {
      arg0K0 = d_108X;
      goto L4111;}
    else {
      h_109X = *((long *) ((((char *) (-3 + d_108X))) + -4));
      *((long *) addr_107X) = h_109X;
      arg0K0 = h_109X;
      goto L4111;}}
  else {
    return status_106X;}}
 L4111: {
  h_110X = arg0K0;
  arg1K0 = (addr_107X + (4 + (-4 & (3 + ((long)(((unsigned long)h_110X)>>8))))));
  goto L4105;}
 trace_image_value: {
  stob_7X = merged_arg0K0;{
  arg0K0 = stob_111X;
  goto L4204;}
 L4204: {
  stob_111X = arg0K0;
  merged_arg0K0 = stob_7X;
  gc_recordP_return_tag = 2;
  goto gc_recordP;
 gc_recordP_return_2:
  v_112X = gc_recordP0_return_value;
  if (v_112X) {
    type_113X = *((long *) (((char *) (-3 + stob_7X))));
    merged_arg0K0 = type_113X;
    gc_recordP_return_tag = 3;
    goto gc_recordP;
   gc_recordP_return_3:
    v_114X = gc_recordP0_return_value;
    if (v_114X) {
      if ((1 == (*((long *) ((((char *) (-3 + type_113X))) + 4))))) {
        merged_arg0K0 = stob_7X;
        note_undumpableB_return_tag = 1;
        goto note_undumpableB;
       note_undumpableB_return_1:
        arg0K0 = (*((long *) ((((char *) (-3 + stob_7X))) + 4)));
        goto L4204;}
      else {
        goto L3811;}}
    else {
      goto L3811;}}
  else {
    goto L3811;}}
 L3811: {
  if ((3 == (3 & stob_7X))) {
    a_115X = ((char *) (-3 + stob_7X));
    if ((a_115X < (Sfrom_beginS))) {
      trace_image_value0_return_value = stob_7X;
      goto trace_image_value_return;}
    else {
      if ((a_115X < (Sfrom_endS))) {
        merged_arg0K0 = stob_7X;
        merged_arg1K1 = (ShpS);
        real_copy_object_return_tag = 1;
        goto real_copy_object;
       real_copy_object_return_1:
        new_thing_116X = real_copy_object0_return_value;
        new_hp_117X = real_copy_object1_return_value;
        ShpS = new_hp_117X;
        trace_image_value0_return_value = new_thing_116X;
        goto trace_image_value_return;}
      else {
        trace_image_value0_return_value = stob_7X;
        goto trace_image_value_return;}}}
  else {
    trace_image_value0_return_value = stob_7X;
    goto trace_image_value_return;}}
 trace_image_value_return:
  switch (trace_image_value_return_tag) {
  case 0: goto trace_image_value_return_0;
  case 1: goto trace_image_value_return_1;
  case 2: goto trace_image_value_return_2;
  case 3: goto trace_image_value_return_3;
  case 4: goto trace_image_value_return_4;
  case 5: goto trace_image_value_return_5;
  case 6: goto trace_image_value_return_6;
  default: goto trace_image_value_return_7;
  }}

 note_undumpableB: {
  thing_6X = merged_arg0K0;{
  if (((Sundumpable_countS) < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (Sundumpable_recordsS)))) + -4))))>>8))))>>2)))) {
    vector_118X = Sundumpable_recordsS;
    len_119X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + vector_118X))) + -4))))>>8))))>>2);
    arg0K0 = 0;
    goto L2728;}
  else {
    goto note_undumpableB_return;}}
 L2728: {
  i_120X = arg0K0;
  if ((i_120X == len_119X)) {
    x_121X = Sundumpable_recordsS;
    addr_122X = (((char *) (-3 + x_121X))) + ((((Sundumpable_countS))<<2));
    S48_WRITE_BARRIER(x_121X, addr_122X, thing_6X);
    *((long *) addr_122X) = thing_6X;
    Sundumpable_countS = (1 + (Sundumpable_countS));
    goto note_undumpableB_return;}
  else {
    if (((*((long *) ((((char *) (-3 + vector_118X))) + (((i_120X)<<2))))) == thing_6X)) {
      goto note_undumpableB_return;}
    else {
      arg0K0 = (1 + i_120X);
      goto L2728;}}}
 note_undumpableB_return:
  switch (note_undumpableB_return_tag) {
  case 0: goto note_undumpableB_return_0;
  default: goto note_undumpableB_return_1;
  }}

 gc_recordP: {
  x_5X = merged_arg0K0;{
  if ((3 == (3 & x_5X))) {
    header_123X = *((long *) ((((char *) (-3 + x_5X))) + -4));
    if ((3 == (3 & header_123X))) {
      if ((3 == (3 & header_123X))) {
        gc_recordP0_return_value = (9 == (31 & ((((*((long *) ((((char *) (-3 + header_123X))) + -4))))>>2))));
        goto gc_recordP_return;}
      else {
        gc_recordP0_return_value = 0;
        goto gc_recordP_return;}}
    else {
      if ((3 == (3 & x_5X))) {
        gc_recordP0_return_value = (9 == (31 & ((((*((long *) ((((char *) (-3 + x_5X))) + -4))))>>2))));
        goto gc_recordP_return;}
      else {
        gc_recordP0_return_value = 0;
        goto gc_recordP_return;}}}
  else {
    gc_recordP0_return_value = 0;
    goto gc_recordP_return;}}
 gc_recordP_return:
  switch (gc_recordP_return_tag) {
  case 0: goto gc_recordP_return_0;
  case 1: goto gc_recordP_return_1;
  case 2: goto gc_recordP_return_2;
  default: goto gc_recordP_return_3;
  }}

 real_copy_object: {
  thing_3X = merged_arg0K0;
  frontier_4X = merged_arg1K1;{
  h_124X = *((long *) ((((char *) (-3 + thing_3X))) + -4));
  if ((3 == (3 & h_124X))) {
    real_copy_object0_return_value = h_124X;
    real_copy_object1_return_value = frontier_4X;
    goto real_copy_object_return;}
  else {
    data_addr_125X = frontier_4X + 4;
    new_126X = 3 + (((long) data_addr_125X));
    new_frontier_127X = data_addr_125X + (-4 & (3 + ((long)(((unsigned long)h_124X)>>8))));
    if ((new_frontier_127X < (Sto_endS))) {
      goto L2001;}
    else {
      ps_error("ran out of memory while writing image", 0);
      goto L2001;}}}
 L2001: {
  *((long *) ((((char *) (-3 + thing_3X))) + -4)) = new_126X;
  *((long *) frontier_4X) = h_124X;
  memcpy((void *)data_addr_125X, (void *)(((char *) (-3 + thing_3X))),((long)(((unsigned long)h_124X)>>8)));
  real_copy_object0_return_value = new_126X;
  real_copy_object1_return_value = new_frontier_127X;
  goto real_copy_object_return;}
 real_copy_object_return:
  switch (real_copy_object_return_tag) {
  case 0: goto real_copy_object_return_0;
  default: goto real_copy_object_return_1;
  }}

}void
s48_write_image_init(void)
{
SstatusS = NO_ERRORS;
}
