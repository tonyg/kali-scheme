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
    goto L1796;}
  else {
    return (NULL);}}
 L1796: {
  i_4X = arg0K0;
  next_5X = *(keys_3X + i_4X);
  if ((key_1X == next_5X)) {
    return (*((table_0X->values) + i_4X));}
  else {
    if ((0 == next_5X)) {
      if ((i_4X == (table_0X->size))) {
        arg0K0 = 0;
        goto L1796;}
      else {
        return (NULL);}}
    else {
      arg0K0 = (1 + i_4X);
      goto L1796;}}}
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
  long i_48X;
  struct image_location *value_47X;
  long key_46X;
  long i_45X;
  long i_44X;
  struct image_location **new_values_43X;
  long *new_keys_42X;
  long new_size_41X;
  long old_size_40X;
  long *old_keys_39X;
  struct image_location **old_values_38X;
  long next_37X;
  long i_36X;
  long *keys_35X;
  long size_34X;
  long header_33X;
  char v_32X;
  long x_31X;
  struct image_location *new_30X;
  struct image_location *new_29X;
  struct image_location *image_location_28X;
  long new_descriptor_27X;
  char * data_addr_26X;
  long h_25X;
  long stob_24X;
  struct image_location *image_location_23X;
  long new_alias_22X;
  char * addr_21X;
  long x_20X;
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
              goto L3670;}
            else {
              goto L4913;}}
          else {
            arg0K0 = thing_8X;
            goto L4021;}}
        else {
          arg0K0 = thing_8X;
          goto L4021;}}
      else {
        arg0K0 = thing_8X;
        goto L4021;}}
    else {
      return (have_13X->new_descriptor);}}
  else {
    return thing_8X;}}
 L3670: {
  i_19X = arg0K0;
  if ((i_19X == len_18X)) {
    x_20X = Sundumpable_recordsS;
    addr_21X = (((char *) (-3 + x_20X))) + ((((Sundumpable_countS))<<2));
    S48_WRITE_BARRIER(x_20X, addr_21X, thing_8X);
    *((long *) addr_21X) = thing_8X;
    Sundumpable_countS = (1 + (Sundumpable_countS));
    goto L4913;}
  else {
    if (((*((long *) ((((char *) (-3 + vector_17X))) + (((i_19X)<<2))))) == thing_8X)) {
      goto L4913;}
    else {
      arg0K0 = (1 + i_19X);
      goto L3670;}}}
 L4913: {
  new_alias_22X = trace_image_value((*((long *) ((((char *) (-3 + thing_8X))) + 4))));
  image_location_23X = (struct image_location*)malloc(sizeof(struct image_location));
  if ((NULL == image_location_23X)) {
    arg1K0 = image_location_23X;
    goto L4917;}
  else {
    image_location_23X->new_descriptor = new_alias_22X;
    image_location_23X->next = 0;
    arg1K0 = image_location_23X;
    goto L4917;}}
 L4021: {
  stob_24X = arg0K0;
  h_25X = *((long *) ((((char *) (-3 + stob_24X))) + -4));
  data_addr_26X = (Simage_hpS) + 4;
  Simage_hpS = (data_addr_26X + (-4 & (3 + ((long)(((unsigned long)h_25X)>>8)))));
  new_descriptor_27X = 3 + (((long) data_addr_26X));
  image_location_28X = (struct image_location*)malloc(sizeof(struct image_location));
  if ((NULL == image_location_28X)) {
    arg1K0 = image_location_28X;
    goto L4031;}
  else {
    image_location_28X->new_descriptor = new_descriptor_27X;
    image_location_28X->next = 0;
    arg1K0 = image_location_28X;
    goto L4031;}}
 L4917: {
  new_29X = arg1K0;
  if ((NULL == new_29X)) {
    (Sstob_tableS)->size = 0;
    return new_alias_22X;}
  else {
    merged_arg2K0 = (Sstob_tableS);
    merged_arg0K1 = thing_8X;
    merged_arg1K2 = new_29X;
    table_setB_return_tag = 0;
    goto table_setB;
   table_setB_return_0:
    return new_alias_22X;}}
 L4031: {
  new_30X = arg1K0;
  if ((NULL == new_30X)) {
    (Sstob_tableS)->size = 0;
    return new_descriptor_27X;}
  else {
    x_31X = Sfirst_stobS;
    if ((1 == x_31X)) {
      Sfirst_stobS = stob_24X;
      goto L4050;}
    else {
      (Slast_stobS)->next = stob_24X;
      goto L4050;}}}
 L4050: {
  Slast_stobS = new_30X;
  new_30X->next = 1;
  merged_arg2K0 = (Sstob_tableS);
  merged_arg0K1 = stob_24X;
  merged_arg1K2 = new_30X;
  table_setB_return_tag = 1;
  goto table_setB;
 table_setB_return_1:
  v_32X = resumer_recordP(stob_24X);
  if (v_32X) {
    Sresumer_countS = (1 + (Sresumer_countS));
    return new_descriptor_27X;}
  else {
    return new_descriptor_27X;}}
 gc_recordP: {
  x_12X = merged_arg0K0;{
  if ((3 == (3 & x_12X))) {
    header_33X = *((long *) ((((char *) (-3 + x_12X))) + -4));
    if ((3 == (3 & header_33X))) {
      if ((3 == (3 & header_33X))) {
        gc_recordP0_return_value = (9 == (31 & ((((*((long *) ((((char *) (-3 + header_33X))) + -4))))>>2))));
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
    size_34X = table_9X->size;
    keys_35X = table_9X->keys;
    arg0K0 = ((key_10X ^ ((((key_10X)<<1)) ^ (((key_10X)>>10)))) & (-1 + size_34X));
    goto L2004;}
  else {
    goto table_setB_return;}}
 L2004: {
  i_36X = arg0K0;
  next_37X = *(keys_35X + i_36X);
  if ((key_10X == next_37X)) {
    *((table_9X->values) + i_36X) = value_11X;
    goto table_setB_return;}
  else {
    if ((0 == next_37X)) {
      if ((i_36X == (table_9X->size))) {
        arg0K0 = 0;
        goto L2004;}
      else {
        *((table_9X->keys) + i_36X) = key_10X;
        *((table_9X->values) + i_36X) = value_11X;
        table_9X->count = (1 + (table_9X->count));
        if (((table_9X->count) == ((table_9X->size) / 3))) {
          old_values_38X = table_9X->values;
          old_keys_39X = table_9X->keys;
          old_size_40X = table_9X->size;
          new_size_41X = (((table_9X->size))<<1);
          new_keys_42X = (long*)malloc(sizeof(long) * (1 + new_size_41X));
          new_values_43X = (struct image_location**)malloc(sizeof(struct image_location*) * new_size_41X);
          if ((NULL == new_keys_42X)) {
            goto L1530;}
          else {
            if ((NULL == new_values_43X)) {
              goto L1530;}
            else {
              table_9X->keys = new_keys_42X;
              table_9X->values = new_values_43X;
              table_9X->size = new_size_41X;
              table_9X->count = 0;
              arg0K0 = 0;
              goto L1683;}}}
        else {
          goto table_setB_return;}}}
    else {
      arg0K0 = (1 + i_36X);
      goto L2004;}}}
 L1530: {
  if ((NULL == new_keys_42X)) {
    goto L1538;}
  else {
    free(new_keys_42X);
    goto L1538;}}
 L1683: {
  i_44X = arg0K0;
  if ((i_44X == (1 + new_size_41X))) {
    arg0K0 = 0;
    goto L1561;}
  else {
    *(new_keys_42X + i_44X) = 0;
    arg0K0 = (1 + i_44X);
    goto L1683;}}
 L1538: {
  if ((NULL == new_values_43X)) {
    goto L1546;}
  else {
    free(new_values_43X);
    goto L1546;}}
 L1561: {
  i_45X = arg0K0;
  if ((i_45X == old_size_40X)) {
    free(old_keys_39X);
    free(old_values_38X);
    goto table_setB_return;}
  else {
    key_46X = *(old_keys_39X + i_45X);
    if ((0 == key_46X)) {
      goto L1577;}
    else {
      value_47X = *(old_values_38X + i_45X);
      arg0K0 = ((key_46X ^ ((((key_46X)<<1)) ^ (((key_46X)>>10)))) & (-1 + new_size_41X));
      goto L1705;}}}
 L1546: {
  table_9X->size = 0;
  goto table_setB_return;}
 L1577: {
  arg0K0 = (1 + i_45X);
  goto L1561;}
 L1705: {
  i_48X = arg0K0;
  if ((0 == (*(new_keys_42X + i_48X)))) {
    if ((i_48X == new_size_41X)) {
      arg0K0 = 0;
      goto L1705;}
    else {
      *(new_keys_42X + i_48X) = key_46X;
      *(new_values_43X + i_48X) = value_47X;
      goto L1577;}}
  else {
    arg0K0 = (1 + i_48X);
    goto L1705;}}
 table_setB_return:
  switch (table_setB_return_tag) {
  case 0: goto table_setB_return_0;
  default: goto table_setB_return_1;
  }}

}
long s48_write_image(long resume_proc_49X, long undumpables_50X, FILE * port_51X)
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
  char * start_52X;
  long size_53X;
  long descriptor_54X;
  long table_55X;
  long have_194X;
  long have_193X;
  long v_192X;
  long value_191X;
  struct image_location *have_190X;
  long thing_189X;
  long link_188X;
  struct image_location *v_187X;
  long next_186X;
  long shared_185X;
  long link_184X;
  char temp_183X;
  long i_182X;
  long value_181X;
  long v_180X;
  struct image_location *have_179X;
  long v_178X;
  long v_177X;
  struct image_location *have_176X;
  long thing_175X;
  struct image_location *have_174X;
  long thing_173X;
  long x_172X;
  char * addr_171X;
  long v_170X;
  long value_169X;
  struct image_location *have_168X;
  long thing_167X;
  char * addr_166X;
  char * start_165X;
  long header_164X;
  long next_163X;
  struct image_location *have_162X;
  long thing_161X;
  struct image_location *have_160X;
  char * addr_159X;
  long v_158X;
  long link_157X;
  struct image_location *v_156X;
  long next_155X;
  char * start_154X;
  long header_153X;
  long next_152X;
  long value_151X;
  long have_150X;
  long available_149X;
  long size_148X;
  char * start_147X;
  long header_146X;
  long shared_145X;
  long link_144X;
  struct image_location *v_143X;
  long next_142X;
  struct image_location *have_141X;
  long thing_140X;
  long i_139X;
  long link_138X;
  long symbol_137X;
  long v_136X;
  long link_135X;
  struct image_location *v_134X;
  long next_133X;
  long *keys_132X;
  struct image_location **values_131X;
  struct table *table_130X;
  long link_129X;
  struct image_location *have_128X;
  long thing_127X;
  long id_126X;
  char * addr_125X;
  long symbol_124X;
  long have_123X;
  char v_122X;
  struct image_location *location_121X;
  long stob_120X;
  char * start_119X;
  long header_118X;
  struct image_location *have_117X;
  long thing_116X;
  long link_115X;
  long v_114X;
  long v_113X;
  long i_112X;
  struct image_location *v_111X;
  long thing_110X;
  long table_109X;
  struct image_location *location_108X;
  long stob_107X;
  long v_106X;
  long v_105X;
  long v_104X;
  long n_103X;
  long v_102X;
  long n_101X;
  struct image_location *have_100X;
  long thing_99X;
  long v_98X;
  long n_97X;
  struct image_location *have_96X;
  long thing_95X;
  long v_94X;
  long n_93X;
  struct image_location *have_92X;
  long thing_91X;
  long v_90X;
  long v_89X;
  long v_88X;
  long v_87X;
  long v_86X;
  long v_85X;
  long i_84X;
  long v_83X;
  long *keys_82X;
  struct image_location **values_81X;
  struct table *table_80X;
  long v_79X;
  char * data_addr_78X;
  long cells_77X;
  long v_76X;
  long v_75X;
  long v_74X;
  struct image_location *last_73X;
  char * addr_72X;
  long next_71X;
  struct image_location *image_location_70X;
  char * start_69X;
  long link_68X;
  long entry_67X;
  long header_66X;
  long stob_65X;
  long link_64X;
  long i_63X;
  long table_62X;
  long resume_proc_61X;
  struct table *v_60X;
  struct table *table_59X;
  long i_58X;
  long *keys_57X;
  char * start_address_56X;
 {  start_address_56X = ((char *) 0);
  keys_57X = (long*)malloc(sizeof(long) * 4097);
  arg0K0 = 0;
  goto L1925;}
 L1925: {
  i_58X = arg0K0;
  if ((4097 == i_58X)) {
    table_59X = (struct table*)malloc(sizeof(struct table));
    if ((NULL == table_59X)) {
      arg2K0 = table_59X;
      goto L1894;}
    else {
      table_59X->keys = keys_57X;
      table_59X->values = ((struct image_location**)malloc(sizeof(struct image_location*) * 4096));
      table_59X->count = 0;
      table_59X->size = 4096;
      arg2K0 = table_59X;
      goto L1894;}}
  else {
    *(keys_57X + i_58X) = 0;
    arg0K0 = (1 + i_58X);
    goto L1925;}}
 L1894: {
  v_60X = arg2K0;
  Sstob_tableS = v_60X;
  Sfirst_stobS = 1;
  Slast_stobS = (NULL);
  Simage_beginS = start_address_56X;
  Simage_hpS = start_address_56X;
  Sundumpable_recordsS = undumpables_50X;
  Sundumpable_countS = 0;
  Sresumer_countS = 0;
  Simage_portS = port_51X;
  Simage_bufferS = ((char *)malloc(4096));
  Simage_buffer_pointerS = (Simage_bufferS);
  SstatusS = NO_ERRORS;
  if (((Simage_bufferS) == NULL)) {
    return ENOMEM;}
  else {
    resume_proc_61X = trace_image_value(resume_proc_49X);
    table_62X = s48_exported_bindings();
    arg0K0 = 0;
    goto L3266;}}
 L3266: {
  i_63X = arg0K0;
  if ((1024 == i_63X)) {
    arg0K0 = (Sfirst_stobS);
    goto L4711;}
  else {
    link_64X = *((long *) ((((char *) (-3 + table_62X))) + (((i_63X)<<2))));
    if ((0 == (3 & link_64X))) {
      arg0K0 = (3 + (-4 & link_64X));
      goto L3240;}
    else {
      arg0K0 = link_64X;
      goto L3240;}}}
 L4711: {
  stob_65X = arg0K0;
  header_66X = *((long *) ((((char *) (-3 + stob_65X))) + -4));
  if ((2 == (3 & header_66X))) {
    if (((31 & (((header_66X)>>2))) < 17)) {
      goto L4583;}
    else {
      goto L4713;}}
  else {
    goto L4583;}}
 L3240: {
  entry_67X = arg0K0;
  if ((1 == entry_67X)) {
    arg0K0 = (1 + i_63X);
    goto L3266;}
  else {
    trace_image_value(entry_67X);
    link_68X = *((long *) ((((char *) (-3 + entry_67X))) + 12));
    if ((0 == (3 & link_68X))) {
      arg0K0 = (3 + (-4 & link_68X));
      goto L3240;}
    else {
      arg0K0 = link_68X;
      goto L3240;}}}
 L4583: {
  if ((1078 == header_66X)) {
    goto L4713;}
  else {
    start_69X = ((char *) (-3 + stob_65X));
    arg3K0 = start_69X;
    goto L4602;}}
 L4713: {
  if ((0 < ((Sstob_tableS)->size))) {
    image_location_70X = table_ref((Sstob_tableS), stob_65X);
    next_71X = image_location_70X->next;
    if ((3 == (3 & next_71X))) {
      arg0K0 = next_71X;
      goto L4711;}
    else {
      goto L4782;}}
  else {
    goto L4782;}}
 L4602: {
  addr_72X = arg3K0;
  if ((addr_72X == (start_69X + (-4 & (3 + ((long)(((unsigned long)header_66X)>>8))))))) {
    goto L4713;}
  else {
    trace_image_value((*((long *) addr_72X)));
    arg3K0 = (addr_72X + 4);
    goto L4602;}}
 L4782: {
  last_73X = Slast_stobS;
  v_74X = s48_symbol_table();
  trace_image_value(v_74X);
  v_75X = s48_imported_bindings();
  trace_image_value(v_75X);
  v_76X = s48_exported_bindings();
  trace_image_value(v_76X);
  last_73X->next = 1;
  cells_77X = Sresumer_countS;
  data_addr_78X = (Simage_hpS) + 4;
  Simage_hpS = (data_addr_78X + (((cells_77X)<<2)));
  Sresumer_recordsS = (3 + (((long) data_addr_78X)));
  if ((0 < ((Sstob_tableS)->size))) {
    if (((SstatusS) == NO_ERRORS)) {
      PS_WRITE_CHAR(10, port_51X, v_79X)
      SstatusS = v_79X;
      goto L2297;}
    else {
      goto L2297;}}
  else {
    table_80X = Sstob_tableS;
    values_81X = table_80X->values;
    keys_82X = table_80X->keys;
    arg0K0 = 0;
    goto L5094;}}
 L2297: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, (Simage_portS), v_83X)
    SstatusS = v_83X;
    goto L2299;}
  else {
    goto L2299;}}
 L5094: {
  i_84X = arg0K0;
  if ((i_84X == (table_80X->size))) {
    free(keys_82X);
    free(values_81X);
    free(table_80X);
    free((Simage_bufferS));
    return ENOMEM;}
  else {
    if ((0 == (*(keys_82X + i_84X)))) {
      goto L5096;}
    else {
      free((*(values_81X + i_84X)));
      goto L5096;}}}
 L2299: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_51X, v_85X)
    SstatusS = v_85X;
    goto L2308;}
  else {
    goto L2308;}}
 L5096: {
  arg0K0 = (1 + i_84X);
  goto L5094;}
 L2308: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_string("Vanilla 23", port_51X));
    goto L2315;}
  else {
    goto L2315;}}
 L2315: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, port_51X, v_86X)
    SstatusS = v_86X;
    goto L2324;}
  else {
    goto L2324;}}
 L2324: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(4, (Simage_portS)));
    goto L2369;}
  else {
    goto L2369;}}
 L2369: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_87X)
    SstatusS = v_87X;
    goto L2326;}
  else {
    goto L2326;}}
 L2326: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(0, (Simage_portS)));
    goto L2386;}
  else {
    goto L2386;}}
 L2386: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_88X)
    SstatusS = v_88X;
    goto L2330;}
  else {
    goto L2330;}}
 L2330: {
  v_89X = (Simage_hpS) - (Simage_beginS);
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer((((v_89X)>>2)), (Simage_portS)));
    goto L2405;}
  else {
    goto L2405;}}
 L2405: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_90X)
    SstatusS = v_90X;
    goto L2336;}
  else {
    goto L2336;}}
 L2336: {
  thing_91X = s48_symbol_table();
  if ((3 == (3 & thing_91X))) {
    have_92X = table_ref((Sstob_tableS), thing_91X);
    if ((NULL == have_92X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L2423;}
    else {
      goto L2423;}}
  else {
    arg0K0 = thing_91X;
    goto L2340;}}
 L2423: {
  arg0K0 = (have_92X->new_descriptor);
  goto L2340;}
 L2340: {
  n_93X = arg0K0;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(n_93X, (Simage_portS)));
    goto L2433;}
  else {
    goto L2433;}}
 L2433: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_94X)
    SstatusS = v_94X;
    goto L2342;}
  else {
    goto L2342;}}
 L2342: {
  thing_95X = s48_imported_bindings();
  if ((3 == (3 & thing_95X))) {
    have_96X = table_ref((Sstob_tableS), thing_95X);
    if ((NULL == have_96X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L2451;}
    else {
      goto L2451;}}
  else {
    arg0K0 = thing_95X;
    goto L2346;}}
 L2451: {
  arg0K0 = (have_96X->new_descriptor);
  goto L2346;}
 L2346: {
  n_97X = arg0K0;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(n_97X, (Simage_portS)));
    goto L2461;}
  else {
    goto L2461;}}
 L2461: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_98X)
    SstatusS = v_98X;
    goto L2348;}
  else {
    goto L2348;}}
 L2348: {
  thing_99X = s48_exported_bindings();
  if ((3 == (3 & thing_99X))) {
    have_100X = table_ref((Sstob_tableS), thing_99X);
    if ((NULL == have_100X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L2479;}
    else {
      goto L2479;}}
  else {
    arg0K0 = thing_99X;
    goto L2352;}}
 L2479: {
  arg0K0 = (have_100X->new_descriptor);
  goto L2352;}
 L2352: {
  n_101X = arg0K0;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(n_101X, (Simage_portS)));
    goto L2489;}
  else {
    goto L2489;}}
 L2489: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_102X)
    SstatusS = v_102X;
    goto L2354;}
  else {
    goto L2354;}}
 L2354: {
  n_103X = Sresumer_recordsS;
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(n_103X, (Simage_portS)));
    goto L2503;}
  else {
    goto L2503;}}
 L2503: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_104X)
    SstatusS = v_104X;
    goto L2356;}
  else {
    goto L2356;}}
 L2356: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_integer(resume_proc_61X, (Simage_portS)));
    goto L2517;}
  else {
    goto L2517;}}
 L2517: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(10, (Simage_portS), v_105X)
    SstatusS = v_105X;
    goto L2358;}
  else {
    goto L2358;}}
 L2358: {
  if (((SstatusS) == NO_ERRORS)) {
    PS_WRITE_CHAR(12, (Simage_portS), v_106X)
    SstatusS = v_106X;
    goto L4999;}
  else {
    goto L4999;}}
 L4999: {
  merged_arg0K0 = 1;
  write_descriptor_return_tag = 0;
  goto write_descriptor;
 write_descriptor_return_0:
  arg0K0 = (Sfirst_stobS);
  goto L4448;}
 L4448: {
  stob_107X = arg0K0;
  if ((3 == (3 & stob_107X))) {
    location_108X = table_ref((Sstob_tableS), stob_107X);
    if ((NULL == location_108X)) {
      ps_error("traced stob has no image-table entry", 0);
      goto L4463;}
    else {
      goto L4463;}}
  else {
    table_109X = s48_symbol_table();
    merged_arg0K0 = (*((long *) ((((char *) (-3 + table_109X))) + -4)));
    write_descriptor_return_tag = 1;
    goto write_descriptor;
   write_descriptor_return_1:
    arg0K0 = 0;
    goto L3439;}}
 L4463: {
  if ((3 == (3 & stob_107X))) {
    if ((13 == (31 & ((((*((long *) ((((char *) (-3 + stob_107X))) + -4))))>>2))))) {
      merged_arg0K0 = 1078;
      write_descriptor_return_tag = 2;
      goto write_descriptor;
     write_descriptor_return_2:
      thing_110X = *((long *) (((char *) (-3 + stob_107X))));
      if ((3 == (3 & thing_110X))) {
        v_111X = table_ref((Sstob_tableS), thing_110X);
        if ((NULL == v_111X)) {
          merged_arg0K0 = 1;
          write_descriptor_return_tag = 3;
          goto write_descriptor;
         write_descriptor_return_3:
          goto L4465;}
        else {
          goto L4194;}}
      else {
        goto L4194;}}
    else {
      goto L4200;}}
  else {
    goto L4200;}}
 L3439: {
  i_112X = arg0K0;
  if ((i_112X == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_109X))) + -4))))>>8))))>>2)))) {
    v_113X = s48_imported_bindings();
    merged_arg0K0 = v_113X;
    write_shared_table_return_tag = 0;
    goto write_shared_table;
   write_shared_table_return_0:
    v_114X = s48_exported_bindings();
    merged_arg0K0 = v_114X;
    write_shared_table_return_tag = 1;
    goto write_shared_table;
   write_shared_table_return_1:
    merged_arg0K0 = (10 + ((((Sresumer_countS))<<10)));
    write_descriptor_return_tag = 4;
    goto write_descriptor;
   write_descriptor_return_4:
    arg0K0 = (Sfirst_stobS);
    goto L4543;}
  else {
    link_115X = *((long *) ((((char *) (-3 + table_109X))) + (((i_112X)<<2))));
    if ((0 == (3 & link_115X))) {
      arg0K0 = (3 + (-4 & link_115X));
      goto L3449;}
    else {
      arg0K0 = link_115X;
      goto L3449;}}}
 L4465: {
  arg0K0 = (location_108X->next);
  goto L4448;}
 L4194: {
  thing_116X = *((long *) (((char *) (-3 + stob_107X))));
  if ((3 == (3 & thing_116X))) {
    have_117X = table_ref((Sstob_tableS), thing_116X);
    if ((NULL == have_117X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L4258;}
    else {
      goto L4258;}}
  else {
    merged_arg0K0 = thing_116X;
    write_descriptor_return_tag = 5;
    goto write_descriptor;
   write_descriptor_return_5:
    goto L4465;}}
 L4200: {
  if ((3 == (3 & stob_107X))) {
    if ((6 == (31 & ((((*((long *) ((((char *) (-3 + stob_107X))) + -4))))>>2))))) {
      header_118X = *((long *) ((((char *) (-3 + stob_107X))) + -4));
      merged_arg0K0 = header_118X;
      write_descriptor_return_tag = 6;
      goto write_descriptor;
     write_descriptor_return_6:
      merged_arg0K0 = 0;
      write_descriptor_return_tag = 7;
      goto write_descriptor;
     write_descriptor_return_7:
      start_119X = (((char *) (-3 + stob_107X))) + 4;
      arg3K0 = start_119X;
      goto L2946;}
    else {
      goto L4204;}}
  else {
    goto L4204;}}
 L4543: {
  stob_120X = arg0K0;
  if ((3 == (3 & stob_120X))) {
    location_121X = table_ref((Sstob_tableS), stob_120X);
    v_122X = resumer_recordP(stob_120X);
    if (v_122X) {
      merged_arg0K0 = (location_121X->new_descriptor);
      write_descriptor_return_tag = 8;
      goto write_descriptor;
     write_descriptor_return_8:
      goto L4555;}
    else {
      goto L4555;}}
  else {
    have_123X = (Simage_buffer_pointerS) - (Simage_bufferS);
    if ((0 < have_123X)) {
      if (((SstatusS) == NO_ERRORS)) {
        SstatusS = (ps_write_block((Simage_portS), ((char *) (Simage_bufferS)), have_123X));
        goto L5042;}
      else {
        goto L5042;}}
    else {
      goto L5005;}}}
 L3449: {
  symbol_124X = arg0K0;
  arg0K0 = symbol_124X;
  goto L3483;}
 L4258: {
  merged_arg0K0 = (have_117X->new_descriptor);
  write_descriptor_return_tag = 9;
  goto write_descriptor;
 write_descriptor_return_9:
  goto L4465;}
 L2946: {
  addr_125X = arg3K0;
  if ((addr_125X == (start_119X + (-4 + (-4 & (3 + ((long)(((unsigned long)header_118X)>>8)))))))) {
    ps_write_string("Channel closed in dumped image: ", (stderr));
    id_126X = *((long *) ((((char *) (-3 + stob_107X))) + 4));
    if ((0 == (3 & id_126X))) {
      ps_write_integer((((id_126X)>>2)), (stderr));
      goto L2923;}
    else {
      ps_write_string((((char *)(((char *) (-3 + id_126X))))), (stderr));
      goto L2923;}}
  else {
    thing_127X = *((long *) addr_125X);
    if ((3 == (3 & thing_127X))) {
      have_128X = table_ref((Sstob_tableS), thing_127X);
      if ((NULL == have_128X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L2960;}
      else {
        goto L2960;}}
    else {
      arg0K0 = thing_127X;
      goto L2951;}}}
 L4204: {
  if ((3 == (3 & stob_107X))) {
    if ((1 == (31 & ((((*((long *) ((((char *) (-3 + stob_107X))) + -4))))>>2))))) {
      link_129X = *((long *) ((((char *) (-3 + stob_107X))) + 4));
      if ((0 == (3 & link_129X))) {
        arg0K0 = (3 + (-4 & link_129X));
        goto L3886;}
      else {
        arg0K0 = link_129X;
        goto L3886;}}
    else {
      goto L4208;}}
  else {
    goto L4208;}}
 L4555: {
  arg0K0 = (location_121X->next);
  goto L4543;}
 L5042: {
  Simage_buffer_pointerS = (Simage_bufferS);
  goto L5005;}
 L5005: {
  table_130X = Sstob_tableS;
  values_131X = table_130X->values;
  keys_132X = table_130X->keys;
  arg0K0 = 0;
  goto L5060;}
 L3483: {
  next_133X = arg0K0;
  if ((3 == (3 & next_133X))) {
    v_134X = table_ref((Sstob_tableS), next_133X);
    if ((NULL == v_134X)) {
      link_135X = *((long *) ((((char *) (-3 + next_133X))) + 4));
      if ((0 == (3 & link_135X))) {
        arg0K0 = (3 + (-4 & link_135X));
        goto L3483;}
      else {
        arg0K0 = link_135X;
        goto L3483;}}
    else {
      arg0K0 = next_133X;
      goto L3451;}}
  else {
    arg0K0 = next_133X;
    goto L3451;}}
 L2923: {
  { long ignoreXX;
  PS_WRITE_CHAR(10, (stderr), ignoreXX) }
  goto L4465;}
 L2960: {
  arg0K0 = (have_128X->new_descriptor);
  goto L2951;}
 L2951: {
  v_136X = arg0K0;
  merged_arg0K0 = v_136X;
  write_descriptor_return_tag = 10;
  goto write_descriptor;
 write_descriptor_return_10:
  arg3K0 = (addr_125X + 4);
  goto L2946;}
 L3886: {
  symbol_137X = arg0K0;
  arg0K0 = symbol_137X;
  goto L3919;}
 L4208: {
  if ((3 == (3 & stob_107X))) {
    if ((14 == (31 & ((((*((long *) ((((char *) (-3 + stob_107X))) + -4))))>>2))))) {
      link_138X = *((long *) ((((char *) (-3 + stob_107X))) + 12));
      if ((0 == (3 & link_138X))) {
        arg0K0 = (3 + (-4 & link_138X));
        goto L3729;}
      else {
        arg0K0 = link_138X;
        goto L3729;}}
    else {
      goto L4212;}}
  else {
    goto L4212;}}
 L5060: {
  i_139X = arg0K0;
  if ((i_139X == (table_130X->size))) {
    free(keys_132X);
    free(values_131X);
    free(table_130X);
    free((Simage_bufferS));
    return (SstatusS);}
  else {
    if ((0 == (*(keys_132X + i_139X)))) {
      goto L5062;}
    else {
      free((*(values_131X + i_139X)));
      goto L5062;}}}
 L3451: {
  thing_140X = arg0K0;
  if ((3 == (3 & thing_140X))) {
    have_141X = table_ref((Sstob_tableS), thing_140X);
    if ((NULL == have_141X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L3502;}
    else {
      goto L3502;}}
  else {
    arg0K0 = thing_140X;
    goto L3453;}}
 L3919: {
  next_142X = arg0K0;
  if ((3 == (3 & next_142X))) {
    v_143X = table_ref((Sstob_tableS), next_142X);
    if ((NULL == v_143X)) {
      link_144X = *((long *) ((((char *) (-3 + next_142X))) + 4));
      if ((0 == (3 & link_144X))) {
        arg0K0 = (3 + (-4 & link_144X));
        goto L3919;}
      else {
        arg0K0 = link_144X;
        goto L3919;}}
    else {
      arg0K0 = next_142X;
      goto L3888;}}
  else {
    arg0K0 = next_142X;
    goto L3888;}}
 L3729: {
  shared_145X = arg0K0;
  arg0K0 = shared_145X;
  goto L3778;}
 L4212: {
  header_146X = *((long *) ((((char *) (-3 + stob_107X))) + -4));
  start_147X = ((char *) (-3 + stob_107X));
  merged_arg0K0 = header_146X;
  write_descriptor_return_tag = 11;
  goto write_descriptor;
 write_descriptor_return_11:
  if ((2 == (3 & header_146X))) {
    if (((31 & (((header_146X)>>2))) < 17)) {
      goto L4228;}
    else {
      size_148X = -4 & (3 + ((long)(((unsigned long)header_146X)>>8)));
      available_149X = 4096 - ((Simage_buffer_pointerS) - (Simage_bufferS));
      if ((available_149X < size_148X)) {
        if ((4096 < size_148X)) {
          have_150X = (Simage_buffer_pointerS) - (Simage_bufferS);
          if ((0 < have_150X)) {
            if (((SstatusS) == NO_ERRORS)) {
              SstatusS = (ps_write_block((Simage_portS), ((char *) (Simage_bufferS)), have_150X));
              goto L2774;}
            else {
              goto L2774;}}
          else {
            goto L2748;}}
        else {
          merged_arg3K0 = start_147X;
          merged_arg0K1 = available_149X;
          copy_image_data_return_tag = 0;
          goto copy_image_data;
         copy_image_data_return_0:
          merged_arg3K0 = (start_147X + available_149X);
          merged_arg0K1 = (size_148X - available_149X);
          copy_image_data_return_tag = 1;
          goto copy_image_data;
         copy_image_data_return_1:
          goto L4465;}}
      else {
        merged_arg3K0 = start_147X;
        merged_arg0K1 = size_148X;
        copy_image_data_return_tag = 2;
        goto copy_image_data;
       copy_image_data_return_2:
        goto L4465;}}}
  else {
    goto L4228;}}
 L5062: {
  arg0K0 = (1 + i_139X);
  goto L5060;}
 L3502: {
  arg0K0 = (have_141X->new_descriptor);
  goto L3453;}
 L3453: {
  value_151X = arg0K0;
  if ((3 == (3 & value_151X))) {
    arg0K0 = (-4 & value_151X);
    goto L3455;}
  else {
    arg0K0 = value_151X;
    goto L3455;}}
 L3888: {
  next_152X = arg0K0;
  header_153X = *((long *) ((((char *) (-3 + stob_107X))) + -4));
  merged_arg0K0 = header_153X;
  write_descriptor_return_tag = 12;
  goto write_descriptor;
 write_descriptor_return_12:
  start_154X = ((char *) (-3 + stob_107X));
  arg3K0 = start_154X;
  goto L3948;}
 L3778: {
  next_155X = arg0K0;
  if ((3 == (3 & next_155X))) {
    v_156X = table_ref((Sstob_tableS), next_155X);
    if ((NULL == v_156X)) {
      link_157X = *((long *) ((((char *) (-3 + next_155X))) + 12));
      if ((0 == (3 & link_157X))) {
        arg0K0 = (3 + (-4 & link_157X));
        goto L3778;}
      else {
        arg0K0 = link_157X;
        goto L3778;}}
    else {
      arg0K0 = next_155X;
      goto L3731;}}
  else {
    arg0K0 = next_155X;
    goto L3731;}}
 L4228: {
  arg3K0 = start_147X;
  goto L4330;}
 L2774: {
  Simage_buffer_pointerS = (Simage_bufferS);
  goto L2748;}
 L2748: {
  if (((SstatusS) == NO_ERRORS)) {
    SstatusS = (ps_write_block((Simage_portS), ((char *) start_147X), size_148X));
    goto L4465;}
  else {
    goto L4465;}}
 L3455: {
  v_158X = arg0K0;
  merged_arg0K0 = v_158X;
  write_descriptor_return_tag = 13;
  goto write_descriptor;
 write_descriptor_return_13:
  arg0K0 = (1 + i_112X);
  goto L3439;}
 L3948: {
  addr_159X = arg3K0;
  if ((addr_159X == (start_154X + (-4 + (-4 & (3 + ((long)(((unsigned long)header_153X)>>8)))))))) {
    if ((3 == (3 & next_152X))) {
      have_160X = table_ref((Sstob_tableS), next_152X);
      if ((NULL == have_160X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L3978;}
      else {
        goto L3978;}}
    else {
      arg0K0 = next_152X;
      goto L3901;}}
  else {
    thing_161X = *((long *) addr_159X);
    if ((3 == (3 & thing_161X))) {
      have_162X = table_ref((Sstob_tableS), thing_161X);
      if ((NULL == have_162X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L3962;}
      else {
        goto L3962;}}
    else {
      arg0K0 = thing_161X;
      goto L3953;}}}
 L3731: {
  next_163X = arg0K0;
  header_164X = *((long *) ((((char *) (-3 + stob_107X))) + -4));
  merged_arg0K0 = header_164X;
  write_descriptor_return_tag = 14;
  goto write_descriptor;
 write_descriptor_return_14:
  start_165X = ((char *) (-3 + stob_107X));
  arg3K0 = start_165X;
  goto L3807;}
 L4330: {
  addr_166X = arg3K0;
  if ((addr_166X == (start_147X + (-4 & (3 + ((long)(((unsigned long)header_146X)>>8))))))) {
    goto L4465;}
  else {
    thing_167X = *((long *) addr_166X);
    if ((3 == (3 & thing_167X))) {
      have_168X = table_ref((Sstob_tableS), thing_167X);
      if ((NULL == have_168X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L4344;}
      else {
        goto L4344;}}
    else {
      arg0K0 = thing_167X;
      goto L4335;}}}
 L3978: {
  arg0K0 = (have_160X->new_descriptor);
  goto L3901;}
 L3901: {
  value_169X = arg0K0;
  if ((3 == (3 & value_169X))) {
    merged_arg0K0 = (-4 & value_169X);
    write_descriptor_return_tag = 15;
    goto write_descriptor;
   write_descriptor_return_15:
    goto L4465;}
  else {
    merged_arg0K0 = value_169X;
    write_descriptor_return_tag = 16;
    goto write_descriptor;
   write_descriptor_return_16:
    goto L4465;}}
 L3962: {
  arg0K0 = (have_162X->new_descriptor);
  goto L3953;}
 L3953: {
  v_170X = arg0K0;
  merged_arg0K0 = v_170X;
  write_descriptor_return_tag = 17;
  goto write_descriptor;
 write_descriptor_return_17:
  arg3K0 = (addr_159X + 4);
  goto L3948;}
 L3807: {
  addr_171X = arg3K0;
  if ((addr_171X == (start_165X + (-8 + (-4 & (3 + ((long)(((unsigned long)header_164X)>>8)))))))) {
    x_172X = *((long *) ((((char *) (-3 + stob_107X))) + 4));
    if ((5 == x_172X)) {
      arg0K0 = 529;
      goto L3756;}
    else {
      thing_173X = *((long *) ((((char *) (-3 + stob_107X))) + 8));
      if ((3 == (3 & thing_173X))) {
        have_174X = table_ref((Sstob_tableS), thing_173X);
        if ((NULL == have_174X)) {
          ps_error("traced object has no descriptor in image", 0);
          goto L3868;}
        else {
          goto L3868;}}
      else {
        arg0K0 = thing_173X;
        goto L3756;}}}
  else {
    thing_175X = *((long *) addr_171X);
    if ((3 == (3 & thing_175X))) {
      have_176X = table_ref((Sstob_tableS), thing_175X);
      if ((NULL == have_176X)) {
        ps_error("traced object has no descriptor in image", 0);
        goto L3821;}
      else {
        goto L3821;}}
    else {
      arg0K0 = thing_175X;
      goto L3812;}}}
 L4344: {
  arg0K0 = (have_168X->new_descriptor);
  goto L4335;}
 L4335: {
  v_177X = arg0K0;
  merged_arg0K0 = v_177X;
  write_descriptor_return_tag = 18;
  goto write_descriptor;
 write_descriptor_return_18:
  arg3K0 = (addr_166X + 4);
  goto L4330;}
 L3756: {
  v_178X = arg0K0;
  merged_arg0K0 = v_178X;
  write_descriptor_return_tag = 19;
  goto write_descriptor;
 write_descriptor_return_19:
  if ((3 == (3 & next_163X))) {
    have_179X = table_ref((Sstob_tableS), next_163X);
    if ((NULL == have_179X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L3844;}
    else {
      goto L3844;}}
  else {
    arg0K0 = next_163X;
    goto L3760;}}
 L3868: {
  arg0K0 = (have_174X->new_descriptor);
  goto L3756;}
 L3821: {
  arg0K0 = (have_176X->new_descriptor);
  goto L3812;}
 L3812: {
  v_180X = arg0K0;
  merged_arg0K0 = v_180X;
  write_descriptor_return_tag = 20;
  goto write_descriptor;
 write_descriptor_return_20:
  arg3K0 = (addr_171X + 4);
  goto L3807;}
 L3844: {
  arg0K0 = (have_179X->new_descriptor);
  goto L3760;}
 L3760: {
  value_181X = arg0K0;
  if ((3 == (3 & value_181X))) {
    merged_arg0K0 = (-4 & value_181X);
    write_descriptor_return_tag = 21;
    goto write_descriptor;
   write_descriptor_return_21:
    goto L4465;}
  else {
    merged_arg0K0 = value_181X;
    write_descriptor_return_tag = 22;
    goto write_descriptor;
   write_descriptor_return_22:
    goto L4465;}}
 write_shared_table: {
  table_55X = merged_arg0K0;{
  merged_arg0K0 = (*((long *) ((((char *) (-3 + table_55X))) + -4)));
  write_descriptor_return_tag = 23;
  goto write_descriptor;
 write_descriptor_return_23:
  arg0K0 = 0;
  goto L3349;}
 L3349: {
  i_182X = arg0K0;
  temp_183X = i_182X == ((((3 + ((long)(((unsigned long)(*((long *) ((((char *) (-3 + table_55X))) + -4))))>>8))))>>2));
  if (temp_183X) {
    write_shared_table0_return_value = temp_183X;
    goto write_shared_table_return;}
  else {
    link_184X = *((long *) ((((char *) (-3 + table_55X))) + (((i_182X)<<2))));
    if ((0 == (3 & link_184X))) {
      arg0K0 = (3 + (-4 & link_184X));
      goto L3359;}
    else {
      arg0K0 = link_184X;
      goto L3359;}}}
 L3359: {
  shared_185X = arg0K0;
  arg0K0 = shared_185X;
  goto L3393;}
 L3393: {
  next_186X = arg0K0;
  if ((3 == (3 & next_186X))) {
    v_187X = table_ref((Sstob_tableS), next_186X);
    if ((NULL == v_187X)) {
      link_188X = *((long *) ((((char *) (-3 + next_186X))) + 12));
      if ((0 == (3 & link_188X))) {
        arg0K0 = (3 + (-4 & link_188X));
        goto L3393;}
      else {
        arg0K0 = link_188X;
        goto L3393;}}
    else {
      arg0K0 = next_186X;
      goto L3361;}}
  else {
    arg0K0 = next_186X;
    goto L3361;}}
 L3361: {
  thing_189X = arg0K0;
  if ((3 == (3 & thing_189X))) {
    have_190X = table_ref((Sstob_tableS), thing_189X);
    if ((NULL == have_190X)) {
      ps_error("traced object has no descriptor in image", 0);
      goto L3412;}
    else {
      goto L3412;}}
  else {
    arg0K0 = thing_189X;
    goto L3363;}}
 L3412: {
  arg0K0 = (have_190X->new_descriptor);
  goto L3363;}
 L3363: {
  value_191X = arg0K0;
  if ((3 == (3 & value_191X))) {
    arg0K0 = (-4 & value_191X);
    goto L3365;}
  else {
    arg0K0 = value_191X;
    goto L3365;}}
 L3365: {
  v_192X = arg0K0;
  merged_arg0K0 = v_192X;
  write_descriptor_return_tag = 24;
  goto write_descriptor;
 write_descriptor_return_24:
  arg0K0 = (1 + i_182X);
  goto L3349;}
 write_shared_table_return:
  switch (write_shared_table_return_tag) {
  case 0: goto write_shared_table_return_0;
  default: goto write_shared_table_return_1;
  }}

 write_descriptor: {
  descriptor_54X = merged_arg0K0;{
  *((long *) (Simage_buffer_pointerS)) = descriptor_54X;
  Simage_buffer_pointerS = ((Simage_buffer_pointerS) + 4);
  if ((4096 == ((Simage_buffer_pointerS) - (Simage_bufferS)))) {
    have_193X = (Simage_buffer_pointerS) - (Simage_bufferS);
    if ((0 < have_193X)) {
      if (((SstatusS) == NO_ERRORS)) {
        SstatusS = (ps_write_block((Simage_portS), ((char *) (Simage_bufferS)), have_193X));
        goto L2180;}
      else {
        goto L2180;}}
    else {
      goto write_descriptor_return;}}
  else {
    goto write_descriptor_return;}}
 L2180: {
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
  start_52X = merged_arg3K0;
  size_53X = merged_arg0K1;{
  memcpy((void *)(Simage_buffer_pointerS), (void *)start_52X,size_53X);
  Simage_buffer_pointerS = ((Simage_buffer_pointerS) + size_53X);
  if ((4096 == ((Simage_buffer_pointerS) - (Simage_bufferS)))) {
    have_194X = (Simage_buffer_pointerS) - (Simage_bufferS);
    if ((0 < have_194X)) {
      if (((SstatusS) == NO_ERRORS)) {
        SstatusS = (ps_write_block((Simage_portS), ((char *) (Simage_bufferS)), have_194X));
        goto L2205;}
      else {
        goto L2205;}}
    else {
      goto copy_image_data_return;}}
  else {
    goto copy_image_data_return;}}
 L2205: {
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
