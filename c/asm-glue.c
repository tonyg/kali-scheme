
#include <stdio.h>
#include "scheme48.h"

static s48_value	s48_provide_asm_values(s48_value asm_vector);
static s48_value	s48_malloc_byte_vector(s48_value length);
static s48_value	s48_stob_start_address(s48_value stob);
static s48_value	s48_free_byte_vector(s48_value byte_vector);

void
s48_init_asm_glue(void)
{
  S48_EXPORT_FUNCTION(s48_provide_asm_values);
  S48_EXPORT_FUNCTION(s48_malloc_byte_vector);
  S48_EXPORT_FUNCTION(s48_stob_start_address);
  S48_EXPORT_FUNCTION(s48_free_byte_vector);
}

static s48_value
s48_provide_asm_values(s48_value asm_vector)
{
  extern void s48_stack_gc();
  extern long s48_unknown_call();
  extern long s48_unknown_return();
  extern long s48_interrupt_handler();
  extern long s48_Sstack_limitS;

  /* 0 *val*        */
  /* 1 *cont*       */
  /* 2 *stack*      */
  S48_VECTOR_SET(asm_vector, 3, s48_enter_fixnum((long) &s48_Sstack_limitS));
  S48_VECTOR_SET(asm_vector, 4, s48_enter_fixnum((long) &s48_stack_gc));
  /* 5 *hp*         */
  /* 6 *heap-limit* */
  S48_VECTOR_SET(asm_vector, 7, s48_enter_fixnum((long) &s48_unknown_call));
  S48_VECTOR_SET(asm_vector, 8, s48_enter_fixnum((long) &s48_unknown_return));
  S48_VECTOR_SET(asm_vector, 9, s48_enter_fixnum((long) &s48_interrupt_handler));

  return S48_UNSPECIFIC;
}

/*
 * Make a byte-vector that is outside the heap (and thus won't be moved (or
 * freed) by the GC).
 */

static s48_value
s48_malloc_byte_vector(s48_value length)
{
  int c_length = s48_extract_fixnum(length);
  int bytes = (c_length + 4 + 3) & -4;  /* space for header + round up */
  char *bv = (char *)malloc(bytes);

  if (bv == NULL)
    s48_raise_out_of_memory_error();

  *((long *) bv) = (c_length << 8)
                 | (S48_STOBTYPE_BYTE_VECTOR << 2)
                 | S48_HEADER_TAG;

  return (s48_value) ((((long) bv) + 4) | S48_STOB_TAG);
}

/*
 * Free up a malloc'ed byte vector.
 */

static s48_value
s48_free_byte_vector(s48_value byte_vector)
{
  if (!S48_BYTE_VECTOR_P(byte_vector))
    s48_raise_argument_type_error(byte_vector);

  free((void *) ((byte_vector & -4)- 4));

  return S48_UNSPECIFIC;
}

/*
 * The assembler needs to be able to get the start address of a stored
 * object.
 */

static s48_value
s48_stob_start_address(s48_value stob)
{
  if (!S48_STOB_P(stob))
    s48_raise_argument_type_error(stob);
  
  return s48_enter_integer((long) S48_ADDRESS_AFTER_HEADER(stob, void));
}

