#include <stdio.h>
#include "prescheme.h"
#include "scheme48vm.h"
#include "scheme48heap.h"

struct image_location {
  long new_descriptor;
  long next;
};
struct table {
  long *keys;
  struct image_location **values;
  long count;
  long size;
};
static struct image_location *table_ref(struct table*, long);
static char resumer_recordP(long);
static long trace_image_value(long);
long s48_write_image(long, long, FILE *);
static long SstatusS;
static FILE * Simage_portS;
static char * Simage_bufferS;
static char * Simage_buffer_pointerS;
static struct table *Sstob_tableS;
static long Sfirst_stobS;
static struct image_location *Slast_stobS;
static char * Simage_beginS;
static char * Simage_hpS;
static long Sresumer_countS;
static long Sresumer_recordsS;
static long Sundumpable_recordsS;
static long Sundumpable_countS;

static struct image_location *table_ref(struct table *table_0X, long key_1X)
{
  long arg0K0;
  long next_5X;
  long i_4X;
  long *keys_3X;
  long size_2X;
 {  if ((0 < (table_0X->size))) {
    size_2X = table_0X->size;
    keys_3X = table_0X->keys;
    arg0K0 = ((key_1X ^ ((((key_1X)<<1)) ^ (((key_1X)>>10)))) & (-1 + size_2X));
    goto L1565;}
  else {
    return (NULL);}}
 L1565: {
  i_4X = arg0K0;
  next_5X = *(keys_3X + i_4X);
  if ((key_1X == next_5X)) {
    return (*((table_0X->values) + i_4X));}
  else {
    if ((0 == next_5X)) {
      if ((i_4X == (table_0X->size))) {
        arg0K0 = 0;
        goto L1565;}
      else {
        return (NULL);}}
    else {
      arg0K0 = (1 + i_4X);
      goto L1565;}}}
}
static char resumer_recordP(long stob_6X)
{
  long type_7X;
 {  if ((3 == (3 & stob_6X))) {
    if ((9 == (31 & ((((*((long *) ((((char *) (-3 + stob_6X))) + -4))))>>2))))) {
      type_7X = *((long *) (((char *) (-3 + stob_6X))));
      if ((3 == (3 & type_7X))) {
        if ((9 == (31 & ((((*((long *) ((((char *) (-3 + type_7X))) + -4))))>>2))))) {
          return (3 == (3 & (*((long *) ((((char *) (-3 + type_7X))) + 4)))));}
        else {
          return 0;}}
      else {
        return 0;}}
    else {
      return 0;}}
  else {
    return 0;}}
}
static long trace_image_value(long thing_8X)
{
  long arg0K0;
  struct image_location *arg1K0;
  struct image_location *merged_arg1K2;
  long merged_arg0K0;
  long merged_arg0K1;
  struct table *merged_arg2K0;

  int table_setB_return_tag;
  int gc_recordP_return_tag;
  char gc_recordP0_return_value;
  struct table *table_9X;
  long key_10X;
  struct image_location *value_11X;
  long x_12X;
  long i_46X;
  struct image_location *value_45X;
  long key_44X;
  long i_43X;
  long i_42X;
  struct image_location **new_values_41X;
  long *new_keys_40X;
  long new_size_39X;
  long old_size_38X;
  long *old_keys_37X;
  struct image_location **old_values_36X;
  long next_35X;
  long i_34X;
  long *keys_33X;
  long size_32X;
  long header_31X;
  char v_30X;
  long x_29X;
  struct image_location *new_28X;
  struct image_location *new_27X;
  struct image_location *image_location_26X;
  long new_descriptor_25X;
  char * data_addr_24X;
  long h_23X;
  long stob_22X;
  struct image_location *image_location_21X;
  long new_alias_20X;
  long i_19X;
  long len_18X;
  long vector_17X;
  char v_16X;
  long type_15X;
  char v_14X;
  struct image_location *have_13X;
 {  if ((3 == (3 & thing_8X))) {
    have_13X = table_ref((Sstob_tableS), thing_8X);
    if ((NULL == have_13X)) {
      merged_arg0K0 = thing_8X;
      gc_recordP_return_tag = 0;
      goto gc_recordP;
     gc_recordP_return_0:
      v_14X = gc_recordP0_return_value;
      if (v_14X) {
        type_15X = *((long *) (((char *) (-3 + thing_8X))));
        merged_arg0K0 = type_15X;
        gc_recordP_return_tag = 1;
        goto gc_recordP;
       gc_recordP_return_1:
        v_16X = gc_recordP0_return_value;
        if (v_16X) {
          if ((1 == (*((long *) ((((char *) (-3 + type_15X))) + 4))))) {
            if (((Sundumpable_countS) < ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + (Sundumpable_recordsS)))) + -4))))>>8))))>>2)))) {
              vector_17X = Sundumpable_recordsS;
              len_18X = (((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + vector_17X))) + -4))))>>8))))>>2);
              arg0K0 = 0;
              goto L3778;}
            else {
              goto L5021;}}
          else {
            arg0K0 = thing_8X;
            goto L4129;}}
        else {
          arg0K0 = thing_8X;
          goto L4129;}}
      else {
        arg0K0 = thing_8X;
        goto L4129;}}
    else {
      return (have_13X->new_descriptor);}}
  else {
    return thing_8X;}}
 L3778: {
  i_19X = arg0K0;
  if ((i_19X == len_18X)) {
    *((long *) ((((char *) (-3 + (Sundumpable_recordsS)))) + ((((Sundumpable_countS))<<2)))) = thing_8X;
    Sundumpable_countS = (1 + (Sundumpable_countS));
    goto L5021;}
  else {
    if (((*((long *) ((((char *) (-3 + vector_17X))) + (((i_19X)<<2))))) == thing_8X)) {
      goto L5021;}
    else {
      arg0K0 = (1 + i_19X);
      goto L3778;}}}
 L5021: {
  new_alias_20X = trace_image_value((*((long *) ((((char *) (-3 + thing_8X))) + 4))));
  image_location_21X = (struct image_location*)malloc(sizeof(struct image_location));
  if ((NULL == image_location_21X)) {
    arg1K0 = image_location_21X;
    goto L5025;}
  else {
    image_location_21X->new_descriptor = new_alias_20X;
    image_location_21X->next = 0;
    arg1K0 = image_location_21X;
    goto L5025;}}
 L4129: {
  stob_22X = arg0K0;
  h_23X = *((long *) ((((char *) (-3 + stob_22X))) + -4));
  data_addr_24X = (Simage_hpS) + 4;
  Simage_hpS = (data_addr_24X + (-4 & (3 + ((long)(((unsigned long)h_23X)>>8)))));
  new_descriptor_25X = 3 + (((long) data_addr_24X));
  image_location_26X = (struct image_location*)malloc(sizeof(struct image_location));
  if ((NULL == image_location_26X)) {
    arg1K0 = image_location_26X;
    goto L4139;}
  else {
    image_location_26X->new_descriptor = new_descriptor_25X;
    image_location_26X->next = 0;
    arg1K0 = image_location_26X;
    goto L4139;}}
 L5025: {
  new_27X = arg1K0;
  if ((NULL == new_27X)) {
    (Sstob_tableS)->size = 0;
    return new_alias_20X;}
  else {
    merged_arg2K0 = (Sstob_tableS);
    merged_arg0K1 = thing_8X;
    merged_arg1K2 = new_27X;
    table_setB_return_tag = 0;
    goto table_setB;
   table_setB_return_0:
    return new_alias_20X;}}
 L4139: {
  new_28X = arg1K0;
  if ((NULL == new_28X)) {
    (Sstob_tableS)->size = 0;
    return new_descriptor_25X;}
  else {
    x_29X = Sfirst_stobS;
    if ((1 == x_29X)) {
      Sfirst_stobS = stob_22X;
      goto L4158;}
    else {
      (Slast_stobS)->next = stob_22X;
      goto L4158;}}}
 L4158: {
  Slast_stobS = new_28X;
  new_28X->next = 1;
  merged_arg2K0 = (Sstob_tableS);
  merged_arg0K1 = stob_22X;
  merged_arg1K2 = new_28X;
  table_setB_return_tag = 1;
  goto table_setB;
 table_setB_return_1:
  v_30X = resumer_recordP(stob_22X);
  if (v_30X) {
    Sresumer_countS = (1 + (Sresumer_countS));
    return new_descriptor_25X;}
  else {
    return new_descriptor_25X;}}
 gc_recordP: {
  x_12X = merged_arg0K0;{
  if ((3 == (3 & x_12X))) {
    header_31X = *((long *) ((((char *) (-3 + x_12X))) + -4));
    if ((3 == (3 & header_31X))) {
      if ((3 == (3 & header_31X))) {
        gc_recordP0_return_value = (9 == (31 & ((((*((long *) ((((char *) (-3 + header_31X))) + -4))))>>2))));
        goto gc_recordP_return;}
      else {
        gc_recordP0_return_value = 0;
        goto gc_recordP_return;}}
    else {
      if ((3 == (3 & x_12X))) {
        gc_recordP0_return_value = (9 == (31 & ((((*((long *) ((((char *) (-3 + x_12X))) + -4))))>>2))));
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
  default: goto gc_recordP_return_1;
  }}

 table_setB: {
  table_9X = merged_arg2K0;
  key_10X = merged_arg0K1;
  value_11X = merged_arg1K2;{
  if ((0 < (table_9X->size))) {
    size_32X = table_9X->size;
    keys_33X = table_9X->keys;
    arg0K0 = ((key_10X ^ ((((key_10X)<<1)) ^ (((key_10X)>>10)))) & (-1 + size_32X));
    goto L2660;}
  else {
    goto table_setB_return;}}
 L2660: {
  i_34X = arg0K0;
  next_35X = *(keys_33X + i_34X);
  if ((key_10X == next_35X)) {
    *((table_9X->values) + i_34X) = value_11X;
    goto table_setB_return;}
  else {
    if ((0 == next_35X)) {
      if ((i_34X == (table_9X->size))) {
        arg0K0 = 0;
        goto L2660;}
      else {
        *((table_9X->keys) + i_34X) = key_10X;
        *((table_9X->values) + i_34X) = value_11X;
        table_9X->count = (1 + (table_9X->count));
        if (((table_9X->count) == ((table_9X->size) / 3))) {
          old_values_36X = table_9X->values;
          old_keys_37X = table_9X->keys;
          old_size_38X = table_9X->size;
          new_size_39X = (((table_9X->size))<<1);
          new_keys_40X = (long*)malloc(sizeof(long) * (1 + new_size_39X));
          new_values_41X = (struct image_location**)malloc(sizeof(struct image_location*) * new_size_39X);
          if ((NULL == new_keys_40X)) {
            goto L1838;}
          else {
            if ((NULL == new_values_41X)) {
              goto L1838;}
            else {
              table_9X->keys = new_keys_40X;
              table_9X->values = new_values_41X;
              table_9X->size = new_size_39X;
              table_9X->count = 0;
              arg0K0 = 0;
              goto L1991;}}}
        else {
          goto table_setB_return;}}}
    else {
      arg0K0 = (1 + i_34X);
      goto L2660;}}}
 L1838: {
  if ((NULL == new_keys_40X)) {
    goto L1846;}
  else {
    free(new_keys_40X);
    goto L1846;}}
 L1991: {
  i_42X = arg0K0;
  if ((i_42X < (1 + new_size_39X))) {
    *(new_keys_40X + i_42X) = 0;
    arg0K0 = (1 + i_42X);
    goto L1991;}
  else {
    arg0K0 = 0;
    goto L1869;}}
 L1846: {
  if ((NULL == new_values_41X)) {
    goto L1854;}
  else {
    free(new_values_41X);
    goto L1854;}}
 L1869: {
  i_43X = arg0K0;
  if ((i_43X == old_size_38X)) {
    free(old_keys_37X);
    free(old_values_36X);
    goto table_setB_return;}
  else {
    key_44X = *(old_keys_37X + i_43X);
    if ((0 == key_44X)) {
      goto L1885;}
    else {
      value_45X = *(old_values_36X + i_43X);
      arg0K0 = ((key_44X ^ ((((key_44X)<<1)) ^ (((key_44X)>>10)))) & (-1 + new_size_39X));
      goto L2013;}}}
 L1854: {
  table_9X->size = 0;
  goto table_setB_return;}
 L1885: {
  arg0K0 = (1 + i_43X);
  goto L1869;}
 L2013: {
  i_46X = arg0K0;
  if ((0 == (*(new_keys_40X + i_46X)))) {
    if ((i_46X == new_size_39X)) {
      arg0K0 = 0;
      goto L2013;}
    else {
      *(new_keys_40X + i_46X) = key_44X;
      *(new_values_41X + i_46X) = value_45X;
      goto L1885;}}
  else {
    arg0K0 = (1 + i_46X);
    goto L2013;}}
 table_setB_return:
  switch (table_setB_return_tag) {
  case 0: goto table_setB_return_0;
  default: goto table_setB_return_1;
  }}

}
long s48_write_image(long resume_proc_47X, long undumpables_48X, FILE * port_49X)
{
  struct table *arg2K0;
  long arg0K0;
  char * arg3K0;
  long merged_arg0K0;
  long merged_arg0K1;
  char * merged_arg3K0;

  int copy_image_data_return_tag;
  int write_descriptor_return_tag;
  int write_shared_table_return_tag;
  char write_shared_table0_return_value;
  char * start_50X;
  long size_51X;
  long descriptor_52X;
  long table_53X;
  long have_192X;
  long have_191X;
  long v_190X;
  long value_189X;
  struct image_location *have_188X;
  long thing_187X;
  long link_186X;
  struct image_location *v_185X;
  long next_184X;
  long shared_183X;
  long link_182X;
  char temp_181X;
  long i_180X;
  long value_179X;
  long v_178X;
  struct image_location *have_177X;
  long v_176X;
  long v_175X;
  struct image_location *have_174X;
  long thing_173X;
  struct image_location *have_172X;
  long thing_171X;
  long x_170X;
  char * addr_169X;
  long v_168X;
  long value_167X;
  struct image_location *have_166X;
  long thing_165X;
  char * addr_164X;
  char * start_163X;
  long header_162X;
  long next_161X;
  struct image_location *have_160X;
  long thing_159X;
  struct image_location *have_158X;
  char * addr_157X;
  long v_156X;
  long link_155X;
  struct image_location *v_154X;
  long next_153X;
  char * start_152X;
  long header_151X;
  long next_150X;
  long value_149X;
  long have_148X;
  long available_147X;
  long size_146X;
  char * start_145X;
  long header_144X;
  long shared_143X;
  long link_142X;
  struct image_location *v_141X;
  long next_140X;
  struct image_location *have_139X;
  long thing_138X;
  long i_137X;
  long link_136X;
  long symbol_135X;
  long v_134X;
  long link_133X;
  struct image_location *v_132X;
  long next_131X;
  long *keys_130X;
  struct image_location **values_129X;
  struct table *table_128X;
  long link_127X;
  struct image_location *have_126X;
  long thing_125X;
  long id_124X;
  char * addr_123X;
  long symbol_122X;
  long have_121X;
  char v_120X;
  struct image_location *location_119X;
  long stob_118X;
  char * start_117X;
  long header_116X;
  struct image_location *have_115X;
  long thing_114X;
  long link_113X;
  long v_112X;
  long v_111X;
  long i_110X;
  struct image_location *v_109X;
  long thing_108X;
  long table_107X;
  struct image_location *location_106X;
  long stob_105X;
  long v_104X;
  long v_103X;
  long v_102X;
  long n_101X;
  long v_100X;
  long n_99X;
  struct image_location *have_98X;
  long thing_97X;
  long v_96X;
  long n_95X;
  struct image_location *have_94X;
  long thing_93X;
  long v_92X;
  long n_91X;
  struct image_location *have_90X;
  long thing_89X;
  long v_88X;
  long v_87X;
  long v_86X;
  long v_85X;
  long v_84X;
  long v_83X;
  long i_82X;
  long v_81X;
  long *keys_80X;
  struct image_location **values_79X;
  struct table *table_78X;
  long v_77X;
  char * data_addr_76X;
  long cells_75X;
  long v_74X;
  long v_73X;
  long v_72X;
  struct image_location *last_71X;
  char * addr_70X;
  long next_69X;
  struct image_location *image_location_68X;
  char * start_67X;
  long link_66X;
  long entry_65X;
  long header_64X;
  long stob_63X;
  long link_62X;
  long i_61X;
  long table_60X;
  long resume_proc_59X;
  struct table *v_58X;
  struct table *table_57X;
  long i_56X;
  long *keys_55X;
  char * start_address_54X;
 {  start_address_54X = ((char *) 0);
  keys_55X = (long*)malloc(sizeof(long) * 4097);
  arg0K0 = 0;
  goto L2581;}
 L2581: {
  i_56X = arg0K0;
  if ((i_56X < 4097)) {
    *(keys_55X + i_56X) = 0;
    arg0K0 = (1 + i_56X);
    goto L2581;}
  else {
    table_57X = (struct table*)malloc(sizeof(struct table));
    if ((NULL == table_57X)) {
      arg2K0 = table_57X;
      goto L2550;}
    else {
      table_57X->keys = keys_55X;
      table_57X->values = ((struct image_location**)malloc(sizeof(struct image_location*) * 4096));
      table_57X->count = 0;
      table_57X->size = 4096;
      arg2K0 = table_57X;
      goto L2550;}}}
 L2550: {
  v_58X = arg2K0;
  Sstob_tableS = v_58X;
  Sfirst_stobS = 1;
  Slast_stobS = (NULL);
  Simage_beginS = start_address_54X;
  Simage_hpS = start_address_54X;
  Sundumpable_recordsS = undumpables_48X;
  Sundumpable_countS = 0;
  Sresumer_countS = 0;
  Simage_portS = port_49X;
  Simage_bufferS = ((char *)malloc(4096));
  Simage_buffer_pointerS = (Simage_bufferS);
  SstatusS = NO_ERRORS;
  if (((Simage_bufferS) == NULL)) {
    return ENOMEM;}
  else {
    resume_proc_59X = trace_image_value(resume_proc_47X);
    table_60X = s48_exported_bindings();
    arg0K0 = 0;
    goto L3378;}}
 L3378: {
  i_61X = arg0K0;
  if ((1024 == i_61X)) {
    arg0K0 = (Sfirst_stobS);
    goto L4819;}
  else {
    link_62X = *((long *) ((((char *) (-3 + table_60X))) + (((i_61X)<<2))));
    if ((0 == (3 & link_62X))) {
      arg0K0 = (3 + (-4 & link_62X));
      goto L3352;}
    else {
      arg0K0 = link_62X;
      goto L3352;}}}
 L4819: {
  stob_63X = arg0K0;
  header_64X = *((long *) ((((char *) (-3 + stob_63X))) + -4));
  if ((2 == (3 & header_64X))) {
    if (((31 & (((header_64X)>>2))) < 17)) {
      goto L4691;}
    else {
      goto L4821;}}
  else {
    goto L4691;}}
 L3352: {
  entry_65X = arg0K0;
  if ((1 == entry_65X)) {
    arg0K0 = (1 + i_61X);
    goto L3378;}
  else {trace_image_value(entry_65X);
    link_66X = *((long *) ((((char *) (-3 + entry_65X))) + 12));
    if ((0 == (3 & link_66X))) {
      arg0K0 = (3 + (-4 & link_66X));
      goto L3352;}
    else {
      arg0K0 = link_66X;
      goto L3352;}}}
 L4691: {
  if ((1078 == header_64X)) {
    goto L4821;}
  else {
    start_67X = ((char *) (-3 + stob_63X));
    arg3K0 = start_67X;
    goto L4710;}}
 L4821: {
  if ((0 < ((Sstob_tableS)->size))) {
    image_location_68X = table_ref((Sstob_tableS), stob_63X);
    next_69X = image_location_68X->next;
    if ((3 == (3 & next_69X))) {
      arg0K0 = next_69X;
      goto L4819;}
    else {
      goto L4890;}}
  else {
    goto L4890;}}
 L4710: {
  addr_70X = arg3K0;
  if ((addr_70X == (start_67X + (-4 & (3 + ((long)(((unsigned long)header_64X)>>8))))))) {
    goto L4821;}
  else {trace_image_value((*((long *) addr_70X)));
    arg3K0 = (addr_70X + 4);
    goto L4710;}}
 L4890: {
  last_71X = Slast_stobS;
  v_72X = s48_symbol_table();trace_image_value(v_72X);
  v_73X = s48_imported_bindings();trace_image_value(v_73X);
  v_74X = s48_exported_bindings();trace_image_value(v_74X);
  last_71X->next = 1;
  cells_75X = Sresumer_countS;
  data_addr_76X = (Simage_hpS) + 4;
  Simage_hpS = (data_addr_76X + (((cells_75X)<<2)));
  Sresumer_recordsS = (3 + (((long) data_addr_76X)));
  if ((0 < ((Sstob_tableS)->size))) {
    if (((SstatusS) == NO_ERRORS)) {
      PS_WRITE_CHAR(10, port_49X, v_77X)
      SstatusS = v_77X;
      goto L2254;}
    else {
      goto L2254;}}
  else {
    table_78X = Sstob_tableS;
    values_79X = table_78X->values;
    keys_80X = table_78X->keys;
    arg0K0 = 0;
    goto L5202;}}
 L2254: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, (Simage_portS), v_81X)
    SstatusS = v_81X;
    goto L2256;}
  else {
    goto L2256;}}
 L5202: {
  i_82X = arg0K0;
  if ((i_82X == (table_78X->size))) {
    free(keys_80X);
    free(values_79X);
    free(table_78X);
    free((Simage_bufferS));
    return ENOMEM;}
  else {
    if ((0 == (*(keys_80X + i_82X)))) {
      goto L5204;}
    else {
      free((*(values_79X + i_82X)));
      goto L5204;}}}
 L2256: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_49X, v_83X)
    SstatusS = v_83X;
    goto L2265;}
  else {
    goto L2265;}}
 L5204: {
  arg0K0 = (1 + i_82X);
  goto L5202;}
 L2265: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string("Vanilla 24", port_49X));
    goto L2272;}
  else {
    goto L2272;}}
 L2272: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_49X, v_84X)
    SstatusS = v_84X;
    goto L2281;}
  else {
    goto L2281;}}
 L2281: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, (Simage_portS)));
    goto L2326;}
  else {
    goto L2326;}}
 L2326: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_85X)
    SstatusS = v_85X;
    goto L2283;}
  else {
    goto L2283;}}
 L2283: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(0, (Simage_portS)));
    goto L2343;}
  else {
    goto L2343;}}
 L2343: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_86X)
    SstatusS = v_86X;
    goto L2287;}
  else {
    goto L2287;}}
 L2287: {
  v_87X = (Simage_hpS) - (Simage_beginS);
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((v_87X)>>2)), (Simage_portS)));
    goto L2362;}
  else {
    goto L2362;}}
 L2362: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_88X)
    SstatusS = v_88X;
    goto L2293;}
  else {
    goto L2293;}}
 L2293: {
  thing_89X = s48_symbol_table();
  if ((3 == (3 & thing_89X))) {
    have_90X = table_ref((Sstob_tableS), thing_89X);
    if ((NULL == have_90X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L2380;}
    else {
      goto L2380;}}
  else {
    arg0K0 = thing_89X;
    goto L2297;}}
 L2380: {
  arg0K0 = (have_90X->new_descriptor);
  goto L2297;}
 L2297: {
  n_91X = arg0K0;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(n_91X, (Simage_portS)));
    goto L2390;}
  else {
    goto L2390;}}
 L2390: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_92X)
    SstatusS = v_92X;
    goto L2299;}
  else {
    goto L2299;}}
 L2299: {
  thing_93X = s48_imported_bindings();
  if ((3 == (3 & thing_93X))) {
    have_94X = table_ref((Sstob_tableS), thing_93X);
    if ((NULL == have_94X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L2408;}
    else {
      goto L2408;}}
  else {
    arg0K0 = thing_93X;
    goto L2303;}}
 L2408: {
  arg0K0 = (have_94X->new_descriptor);
  goto L2303;}
 L2303: {
  n_95X = arg0K0;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(n_95X, (Simage_portS)));
    goto L2418;}
  else {
    goto L2418;}}
 L2418: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_96X)
    SstatusS = v_96X;
    goto L2305;}
  else {
    goto L2305;}}
 L2305: {
  thing_97X = s48_exported_bindings();
  if ((3 == (3 & thing_97X))) {
    have_98X = table_ref((Sstob_tableS), thing_97X);
    if ((NULL == have_98X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L2436;}
    else {
      goto L2436;}}
  else {
    arg0K0 = thing_97X;
    goto L2309;}}
 L2436: {
  arg0K0 = (have_98X->new_descriptor);
  goto L2309;}
 L2309: {
  n_99X = arg0K0;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(n_99X, (Simage_portS)));
    goto L2446;}
  else {
    goto L2446;}}
 L2446: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_100X)
    SstatusS = v_100X;
    goto L2311;}
  else {
    goto L2311;}}
 L2311: {
  n_101X = Sresumer_recordsS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(n_101X, (Simage_portS)));
    goto L2460;}
  else {
    goto L2460;}}
 L2460: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_102X)
    SstatusS = v_102X;
    goto L2313;}
  else {
    goto L2313;}}
 L2313: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_59X, (Simage_portS)));
    goto L2474;}
  else {
    goto L2474;}}
 L2474: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_103X)
    SstatusS = v_103X;
    goto L2315;}
  else {
    goto L2315;}}
 L2315: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, (Simage_portS), v_104X)
    SstatusS = v_104X;
    goto L5107;}
  else {
    goto L5107;}}
 L5107: {
  merged_arg0K0 = 1;
  write_descriptor_return_tag = 0;
  goto write_descriptor;
 write_descriptor_return_0:
  arg0K0 = (Sfirst_stobS);
  goto L4556;}
 L4556: {
  stob_105X = arg0K0;
  if ((3 == (3 & stob_105X))) {
    location_106X = table_ref((Sstob_tableS), stob_105X);
    if ((NULL == location_106X)) {
      ps_error("traced stob has no image-table entry", 0);
      goto L4571;}
    else {
      goto L4571;}}
  else {
    table_107X = s48_symbol_table();
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_107X))) + -4)));
    write_descriptor_return_tag = 1;
    goto write_descriptor;
   write_descriptor_return_1:
    arg0K0 = 0;
    goto L3551;}}
 L4571: {
  if ((3 == (3 & stob_105X))) {
    if ((13 == (31 & ((((*((long *) ((((char *) (-3 + stob_105X))) + -4))))>>2))))) {
      merged_arg0K0 = 1078;
      write_descriptor_return_tag = 2;
      goto write_descriptor;
     write_descriptor_return_2:
      thing_108X = *((long *) (((char *) (-3 + stob_105X))));
      if ((3 == (3 & thing_108X))) {
        v_109X = table_ref((Sstob_tableS), thing_108X);
        if ((NULL == v_109X)) {
          merged_arg0K0 = 1;
          write_descriptor_return_tag = 3;
          goto write_descriptor;
         write_descriptor_return_3:
          goto L4573;}
        else {
          goto L4302;}}
      else {
        goto L4302;}}
    else {
      goto L4308;}}
  else {
    goto L4308;}}
 L3551: {
  i_110X = arg0K0;
  if ((i_110X == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_107X))) + -4))))>>8))))>>2)))) {
    v_111X = s48_imported_bindings();
    merged_arg0K0 = v_111X;
    write_shared_table_return_tag = 0;
    goto write_shared_table;
   write_shared_table_return_0:
    v_112X = s48_exported_bindings();
    merged_arg0K0 = v_112X;
    write_shared_table_return_tag = 1;
    goto write_shared_table;
   write_shared_table_return_1:
    merged_arg0K0 = (10 + ((((Sresumer_countS))<<10)));
    write_descriptor_return_tag = 4;
    goto write_descriptor;
   write_descriptor_return_4:
    arg0K0 = (Sfirst_stobS);
    goto L4651;}
  else {
    link_113X = *((long *) ((((char *) (-3 + table_107X))) + (((i_110X)<<2))));
    if ((0 == (3 & link_113X))) {
      arg0K0 = (3 + (-4 & link_113X));
      goto L3561;}
    else {
      arg0K0 = link_113X;
      goto L3561;}}}
 L4573: {
  arg0K0 = (location_106X->next);
  goto L4556;}
 L4302: {
  thing_114X = *((long *) (((char *) (-3 + stob_105X))));
  if ((3 == (3 & thing_114X))) {
    have_115X = table_ref((Sstob_tableS), thing_114X);
    if ((NULL == have_115X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L4366;}
    else {
      goto L4366;}}
  else {
    merged_arg0K0 = thing_114X;
    write_descriptor_return_tag = 5;
    goto write_descriptor;
   write_descriptor_return_5:
    goto L4573;}}
 L4308: {
  if ((3 == (3 & stob_105X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + stob_105X))) + -4))))>>2))))) {
      header_116X = *((long *) ((((char *) (-3 + stob_105X))) + -4));
      merged_arg0K0 = header_116X;
      write_descriptor_return_tag = 6;
      goto write_descriptor;
     write_descriptor_return_6:
      merged_arg0K0 = 0;
      write_descriptor_return_tag = 7;
      goto write_descriptor;
     write_descriptor_return_7:
      start_117X = (((char *) (-3 + stob_105X))) + 4;
      arg3K0 = start_117X;
      goto L3058;}
    else {
      goto L4312;}}
  else {
    goto L4312;}}
 L4651: {
  stob_118X = arg0K0;
  if ((3 == (3 & stob_118X))) {
    location_119X = table_ref((Sstob_tableS), stob_118X);
    v_120X = resumer_recordP(stob_118X);
    if (v_120X) {
      merged_arg0K0 = (location_119X->new_descriptor);
      write_descriptor_return_tag = 8;
      goto write_descriptor;
     write_descriptor_return_8:
      goto L4663;}
    else {
      goto L4663;}}
  else {
    have_121X = (Simage_buffer_pointerS) - (Simage_bufferS);
    if ((0 < have_121X)) {
      if (((SstatusS) == NO_ERRORS)) {
        SstatusS = (ps_write_block((Simage_portS), ((char *) (Simage_bufferS)), have_121X));
        goto L5150;}
      else {
        goto L5150;}}
    else {
      goto L5113;}}}
 L3561: {
  symbol_122X = arg0K0;
  arg0K0 = symbol_122X;
  goto L3595;}
 L4366: {
  merged_arg0K0 = (have_115X->new_descriptor);
  write_descriptor_return_tag = 9;
  goto write_descriptor;
 write_descriptor_return_9:
  goto L4573;}
 L3058: {
  addr_123X = arg3K0;
  if ((addr_123X == (start_117X + (-4 + (-4 & (3 + ((long)(((unsigned long)header_116X)>>8)))))))) {
    ps_write_string("Channel closed in dumped image: ", (stderr));
    id_124X = *((long *) ((((char *) (-3 + stob_105X))) + 4));
    if ((0 == (3 & id_124X))) {
      ps_write_integer((((id_124X)>>2)), (stderr));
      goto L3035;}
    else {
      ps_write_string((((char *)(((char *) (-3 + id_124X))))), (stderr));
      goto L3035;}}
  else {
    thing_125X = *((long *) addr_123X);
    if ((3 == (3 & thing_125X))) {
      have_126X = table_ref((Sstob_tableS), thing_125X);
      if ((NULL == have_126X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L3072;}
      else {
        goto L3072;}}
    else {
      arg0K0 = thing_125X;
      goto L3063;}}}
 L4312: {
  if ((3 == (3 & stob_105X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + stob_105X))) + -4))))>>2))))) {
      link_127X = *((long *) ((((char *) (-3 + stob_105X))) + 4));
      if ((0 == (3 & link_127X))) {
        arg0K0 = (3 + (-4 & link_127X));
        goto L3994;}
      else {
        arg0K0 = link_127X;
        goto L3994;}}
    else {
      goto L4316;}}
  else {
    goto L4316;}}
 L4663: {
  arg0K0 = (location_119X->next);
  goto L4651;}
 L5150: {
  Simage_buffer_pointerS = (Simage_bufferS);
  goto L5113;}
 L5113: {
  table_128X = Sstob_tableS;
  values_129X = table_128X->values;
  keys_130X = table_128X->keys;
  arg0K0 = 0;
  goto L5168;}
 L3595: {
  next_131X = arg0K0;
  if ((3 == (3 & next_131X))) {
    v_132X = table_ref((Sstob_tableS), next_131X);
    if ((NULL == v_132X)) {
      link_133X = *((long *) ((((char *) (-3 + next_131X))) + 4));
      if ((0 == (3 & link_133X))) {
        arg0K0 = (3 + (-4 & link_133X));
        goto L3595;}
      else {
        arg0K0 = link_133X;
        goto L3595;}}
    else {
      arg0K0 = next_131X;
      goto L3563;}}
  else {
    arg0K0 = next_131X;
    goto L3563;}}
 L3035: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  goto L4573;}
 L3072: {
  arg0K0 = (have_126X->new_descriptor);
  goto L3063;}
 L3063: {
  v_134X = arg0K0;
  merged_arg0K0 = v_134X;
  write_descriptor_return_tag = 10;
  goto write_descriptor;
 write_descriptor_return_10:
  arg3K0 = (addr_123X + 4);
  goto L3058;}
 L3994: {
  symbol_135X = arg0K0;
  arg0K0 = symbol_135X;
  goto L4027;}
 L4316: {
  if ((3 == (3 & stob_105X))) {
    if ((14 == (31 & ((((*((long *) ((((char *) (-3 + stob_105X))) + -4))))>>2))))) {
      link_136X = *((long *) ((((char *) (-3 + stob_105X))) + 12));
      if ((0 == (3 & link_136X))) {
        arg0K0 = (3 + (-4 & link_136X));
        goto L3837;}
      else {
        arg0K0 = link_136X;
        goto L3837;}}
    else {
      goto L4320;}}
  else {
    goto L4320;}}
 L5168: {
  i_137X = arg0K0;
  if ((i_137X == (table_128X->size))) {
    free(keys_130X);
    free(values_129X);
    free(table_128X);
    free((Simage_bufferS));
    return (SstatusS);}
  else {
    if ((0 == (*(keys_130X + i_137X)))) {
      goto L5170;}
    else {
      free((*(values_129X + i_137X)));
      goto L5170;}}}
 L3563: {
  thing_138X = arg0K0;
  if ((3 == (3 & thing_138X))) {
    have_139X = table_ref((Sstob_tableS), thing_138X);
    if ((NULL == have_139X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L3614;}
    else {
      goto L3614;}}
  else {
    arg0K0 = thing_138X;
    goto L3565;}}
 L4027: {
  next_140X = arg0K0;
  if ((3 == (3 & next_140X))) {
    v_141X = table_ref((Sstob_tableS), next_140X);
    if ((NULL == v_141X)) {
      link_142X = *((long *) ((((char *) (-3 + next_140X))) + 4));
      if ((0 == (3 & link_142X))) {
        arg0K0 = (3 + (-4 & link_142X));
        goto L4027;}
      else {
        arg0K0 = link_142X;
        goto L4027;}}
    else {
      arg0K0 = next_140X;
      goto L3996;}}
  else {
    arg0K0 = next_140X;
    goto L3996;}}
 L3837: {
  shared_143X = arg0K0;
  arg0K0 = shared_143X;
  goto L3886;}
 L4320: {
  header_144X = *((long *) ((((char *) (-3 + stob_105X))) + -4));
  start_145X = ((char *) (-3 + stob_105X));
  merged_arg0K0 = header_144X;
  write_descriptor_return_tag = 11;
  goto write_descriptor;
 write_descriptor_return_11:
  if ((2 == (3 & header_144X))) {
    if (((31 & (((header_144X)>>2))) < 17)) {
      goto L4336;}
    else {
      size_146X = -4 & (3 + ((long)(((unsigned long)header_144X)>>8)));
      available_147X = 4096 - ((Simage_buffer_pointerS) - (Simage_bufferS));
      if ((available_147X < size_146X)) {
        if ((4096 < size_146X)) {
          have_148X = (Simage_buffer_pointerS) - (Simage_bufferS);
          if ((0 < have_148X)) {
            if (((SstatusS) == NO_ERRORS)) {
              SstatusS = (ps_write_block((Simage_portS), ((char *) (Simage_bufferS)), have_148X));
              goto L2886;}
            else {
              goto L2886;}}
          else {
            goto L2860;}}
        else {
          merged_arg3K0 = start_145X;
          merged_arg0K1 = available_147X;
          copy_image_data_return_tag = 0;
          goto copy_image_data;
         copy_image_data_return_0:
          merged_arg3K0 = (start_145X + available_147X);
          merged_arg0K1 = (size_146X - available_147X);
          copy_image_data_return_tag = 1;
          goto copy_image_data;
         copy_image_data_return_1:
          goto L4573;}}
      else {
        merged_arg3K0 = start_145X;
        merged_arg0K1 = size_146X;
        copy_image_data_return_tag = 2;
        goto copy_image_data;
       copy_image_data_return_2:
        goto L4573;}}}
  else {
    goto L4336;}}
 L5170: {
  arg0K0 = (1 + i_137X);
  goto L5168;}
 L3614: {
  arg0K0 = (have_139X->new_descriptor);
  goto L3565;}
 L3565: {
  value_149X = arg0K0;
  if ((3 == (3 & value_149X))) {
    arg0K0 = (-4 & value_149X);
    goto L3567;}
  else {
    arg0K0 = value_149X;
    goto L3567;}}
 L3996: {
  next_150X = arg0K0;
  header_151X = *((long *) ((((char *) (-3 + stob_105X))) + -4));
  merged_arg0K0 = header_151X;
  write_descriptor_return_tag = 12;
  goto write_descriptor;
 write_descriptor_return_12:
  start_152X = ((char *) (-3 + stob_105X));
  arg3K0 = start_152X;
  goto L4056;}
 L3886: {
  next_153X = arg0K0;
  if ((3 == (3 & next_153X))) {
    v_154X = table_ref((Sstob_tableS), next_153X);
    if ((NULL == v_154X)) {
      link_155X = *((long *) ((((char *) (-3 + next_153X))) + 12));
      if ((0 == (3 & link_155X))) {
        arg0K0 = (3 + (-4 & link_155X));
        goto L3886;}
      else {
        arg0K0 = link_155X;
        goto L3886;}}
    else {
      arg0K0 = next_153X;
      goto L3839;}}
  else {
    arg0K0 = next_153X;
    goto L3839;}}
 L4336: {
  arg3K0 = start_145X;
  goto L4438;}
 L2886: {
  Simage_buffer_pointerS = (Simage_bufferS);
  goto L2860;}
 L2860: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block((Simage_portS), ((char *) start_145X), size_146X));
    goto L4573;}
  else {
    goto L4573;}}
 L3567: {
  v_156X = arg0K0;
  merged_arg0K0 = v_156X;
  write_descriptor_return_tag = 13;
  goto write_descriptor;
 write_descriptor_return_13:
  arg0K0 = (1 + i_110X);
  goto L3551;}
 L4056: {
  addr_157X = arg3K0;
  if ((addr_157X == (start_152X + (-4 + (-4 & (3 + ((long)(((unsigned long)header_151X)>>8)))))))) {
    if ((3 == (3 & next_150X))) {
      have_158X = table_ref((Sstob_tableS), next_150X);
      if ((NULL == have_158X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L4086;}
      else {
        goto L4086;}}
    else {
      arg0K0 = next_150X;
      goto L4009;}}
  else {
    thing_159X = *((long *) addr_157X);
    if ((3 == (3 & thing_159X))) {
      have_160X = table_ref((Sstob_tableS), thing_159X);
      if ((NULL == have_160X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L4070;}
      else {
        goto L4070;}}
    else {
      arg0K0 = thing_159X;
      goto L4061;}}}
 L3839: {
  next_161X = arg0K0;
  header_162X = *((long *) ((((char *) (-3 + stob_105X))) + -4));
  merged_arg0K0 = header_162X;
  write_descriptor_return_tag = 14;
  goto write_descriptor;
 write_descriptor_return_14:
  start_163X = ((char *) (-3 + stob_105X));
  arg3K0 = start_163X;
  goto L3915;}
 L4438: {
  addr_164X = arg3K0;
  if ((addr_164X == (start_145X + (-4 & (3 + ((long)(((unsigned long)header_144X)>>8))))))) {
    goto L4573;}
  else {
    thing_165X = *((long *) addr_164X);
    if ((3 == (3 & thing_165X))) {
      have_166X = table_ref((Sstob_tableS), thing_165X);
      if ((NULL == have_166X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L4452;}
      else {
        goto L4452;}}
    else {
      arg0K0 = thing_165X;
      goto L4443;}}}
 L4086: {
  arg0K0 = (have_158X->new_descriptor);
  goto L4009;}
 L4009: {
  value_167X = arg0K0;
  if ((3 == (3 & value_167X))) {
    merged_arg0K0 = (-4 & value_167X);
    write_descriptor_return_tag = 15;
    goto write_descriptor;
   write_descriptor_return_15:
    goto L4573;}
  else {
    merged_arg0K0 = value_167X;
    write_descriptor_return_tag = 16;
    goto write_descriptor;
   write_descriptor_return_16:
    goto L4573;}}
 L4070: {
  arg0K0 = (have_160X->new_descriptor);
  goto L4061;}
 L4061: {
  v_168X = arg0K0;
  merged_arg0K0 = v_168X;
  write_descriptor_return_tag = 17;
  goto write_descriptor;
 write_descriptor_return_17:
  arg3K0 = (addr_157X + 4);
  goto L4056;}
 L3915: {
  addr_169X = arg3K0;
  if ((addr_169X == (start_163X + (-8 + (-4 & (3 + ((long)(((unsigned long)header_162X)>>8)))))))) {
    x_170X = *((long *) ((((char *) (-3 + stob_105X))) + 4));
    if ((5 == x_170X)) {
      arg0K0 = 529;
      goto L3864;}
    else {
      thing_171X = *((long *) ((((char *) (-3 + stob_105X))) + 8));
      if ((3 == (3 & thing_171X))) {
        have_172X = table_ref((Sstob_tableS), thing_171X);
        if ((NULL == have_172X)) {
          ps_error("traced object has no descriptor in image", 0);
          goto L3976;}
        else {
          goto L3976;}}
      else {
        arg0K0 = thing_171X;
        goto L3864;}}}
  else {
    thing_173X = *((long *) addr_169X);
    if ((3 == (3 & thing_173X))) {
      have_174X = table_ref((Sstob_tableS), thing_173X);
      if ((NULL == have_174X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L3929;}
      else {
        goto L3929;}}
    else {
      arg0K0 = thing_173X;
      goto L3920;}}}
 L4452: {
  arg0K0 = (have_166X->new_descriptor);
  goto L4443;}
 L4443: {
  v_175X = arg0K0;
  merged_arg0K0 = v_175X;
  write_descriptor_return_tag = 18;
  goto write_descriptor;
 write_descriptor_return_18:
  arg3K0 = (addr_164X + 4);
  goto L4438;}
 L3864: {
  v_176X = arg0K0;
  merged_arg0K0 = v_176X;
  write_descriptor_return_tag = 19;
  goto write_descriptor;
 write_descriptor_return_19:
  if ((3 == (3 & next_161X))) {
    have_177X = table_ref((Sstob_tableS), next_161X);
    if ((NULL == have_177X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L3952;}
    else {
      goto L3952;}}
  else {
    arg0K0 = next_161X;
    goto L3868;}}
 L3976: {
  arg0K0 = (have_172X->new_descriptor);
  goto L3864;}
 L3929: {
  arg0K0 = (have_174X->new_descriptor);
  goto L3920;}
 L3920: {
  v_178X = arg0K0;
  merged_arg0K0 = v_178X;
  write_descriptor_return_tag = 20;
  goto write_descriptor;
 write_descriptor_return_20:
  arg3K0 = (addr_169X + 4);
  goto L3915;}
 L3952: {
  arg0K0 = (have_177X->new_descriptor);
  goto L3868;}
 L3868: {
  value_179X = arg0K0;
  if ((3 == (3 & value_179X))) {
    merged_arg0K0 = (-4 & value_179X);
    write_descriptor_return_tag = 21;
    goto write_descriptor;
   write_descriptor_return_21:
    goto L4573;}
  else {
    merged_arg0K0 = value_179X;
    write_descriptor_return_tag = 22;
    goto write_descriptor;
   write_descriptor_return_22:
    goto L4573;}}
 write_shared_table: {
  table_53X = merged_arg0K0;{
  merged_arg0K0 = (*((long *) ((((char *) (-3 + table_53X))) + -4)));
  write_descriptor_return_tag = 23;
  goto write_descriptor;
 write_descriptor_return_23:
  arg0K0 = 0;
  goto L3461;}
 L3461: {
  i_180X = arg0K0;
  temp_181X = i_180X == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_53X))) + -4))))>>8))))>>2));
  if (temp_181X) {
    write_shared_table0_return_value = temp_181X;
    goto write_shared_table_return;}
  else {
    link_182X = *((long *) ((((char *) (-3 + table_53X))) + (((i_180X)<<2))));
    if ((0 == (3 & link_182X))) {
      arg0K0 = (3 + (-4 & link_182X));
      goto L3471;}
    else {
      arg0K0 = link_182X;
      goto L3471;}}}
 L3471: {
  shared_183X = arg0K0;
  arg0K0 = shared_183X;
  goto L3505;}
 L3505: {
  next_184X = arg0K0;
  if ((3 == (3 & next_184X))) {
    v_185X = table_ref((Sstob_tableS), next_184X);
    if ((NULL == v_185X)) {
      link_186X = *((long *) ((((char *) (-3 + next_184X))) + 12));
      if ((0 == (3 & link_186X))) {
        arg0K0 = (3 + (-4 & link_186X));
        goto L3505;}
      else {
        arg0K0 = link_186X;
        goto L3505;}}
    else {
      arg0K0 = next_184X;
      goto L3473;}}
  else {
    arg0K0 = next_184X;
    goto L3473;}}
 L3473: {
  thing_187X = arg0K0;
  if ((3 == (3 & thing_187X))) {
    have_188X = table_ref((Sstob_tableS), thing_187X);
    if ((NULL == have_188X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L3524;}
    else {
      goto L3524;}}
  else {
    arg0K0 = thing_187X;
    goto L3475;}}
 L3524: {
  arg0K0 = (have_188X->new_descriptor);
  goto L3475;}
 L3475: {
  value_189X = arg0K0;
  if ((3 == (3 & value_189X))) {
    arg0K0 = (-4 & value_189X);
    goto L3477;}
  else {
    arg0K0 = value_189X;
    goto L3477;}}
 L3477: {
  v_190X = arg0K0;
  merged_arg0K0 = v_190X;
  write_descriptor_return_tag = 24;
  goto write_descriptor;
 write_descriptor_return_24:
  arg0K0 = (1 + i_180X);
  goto L3461;}
 write_shared_table_return:
  switch (write_shared_table_return_tag) {
  case 0: goto write_shared_table_return_0;
  default: goto write_shared_table_return_1;
  }}

 write_descriptor: {
  descriptor_52X = merged_arg0K0;{
  *((long *) (Simage_buffer_pointerS)) = descriptor_52X;
  Simage_buffer_pointerS = ((Simage_buffer_pointerS) + 4);
  if ((4096 == ((Simage_buffer_pointerS) - (Simage_bufferS)))) {
    have_191X = (Simage_buffer_pointerS) - (Simage_bufferS);
    if ((0 < have_191X)) {
      if (((SstatusS) == NO_ERRORS)) {
        SstatusS = (ps_write_block((Simage_portS), ((char *) (Simage_bufferS)), have_191X));
        goto L2137;}
      else {
        goto L2137;}}
    else {
      goto write_descriptor_return;}}
  else {
    goto write_descriptor_return;}}
 L2137: {
  Simage_buffer_pointerS = (Simage_bufferS);
  goto write_descriptor_return;}
 write_descriptor_return:
  switch (write_descriptor_return_tag) {
  case 0: goto write_descriptor_return_0;
  case 1: goto write_descriptor_return_1;
  case 2: goto write_descriptor_return_2;
  case 3: goto write_descriptor_return_3;
  case 4: goto write_descriptor_return_4;
  case 5: goto write_descriptor_return_5;
  case 6: goto write_descriptor_return_6;
  case 7: goto write_descriptor_return_7;
  case 8: goto write_descriptor_return_8;
  case 9: goto write_descriptor_return_9;
  case 10: goto write_descriptor_return_10;
  case 11: goto write_descriptor_return_11;
  case 12: goto write_descriptor_return_12;
  case 13: goto write_descriptor_return_13;
  case 14: goto write_descriptor_return_14;
  case 15: goto write_descriptor_return_15;
  case 16: goto write_descriptor_return_16;
  case 17: goto write_descriptor_return_17;
  case 18: goto write_descriptor_return_18;
  case 19: goto write_descriptor_return_19;
  case 20: goto write_descriptor_return_20;
  case 21: goto write_descriptor_return_21;
  case 22: goto write_descriptor_return_22;
  case 23: goto write_descriptor_return_23;
  default: goto write_descriptor_return_24;
  }}

 copy_image_data: {
  start_50X = merged_arg3K0;
  size_51X = merged_arg0K1;{
  memcpy((void *)(Simage_buffer_pointerS), (void *)start_50X,size_51X);
  Simage_buffer_pointerS = ((Simage_buffer_pointerS) + size_51X);
  if ((4096 == ((Simage_buffer_pointerS) - (Simage_bufferS)))) {
    have_192X = (Simage_buffer_pointerS) - (Simage_bufferS);
    if ((0 < have_192X)) {
      if (((SstatusS) == NO_ERRORS)) {
        SstatusS = (ps_write_block((Simage_portS), ((char *) (Simage_bufferS)), have_192X));
        goto L2162;}
      else {
        goto L2162;}}
    else {
      goto copy_image_data_return;}}
  else {
    goto copy_image_data_return;}}
 L2162: {
  Simage_buffer_pointerS = (Simage_bufferS);
  goto copy_image_data_return;}
 copy_image_data_return:
  switch (copy_image_data_return_tag) {
  case 0: goto copy_image_data_return_0;
  case 1: goto copy_image_data_return_1;
  default: goto copy_image_data_return_2;
  }}

}void
s48_write_image_init(void)
{
SstatusS = NO_ERRORS;
}
