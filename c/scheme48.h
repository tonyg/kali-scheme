/* This file was generated automatically.
   It's probably not a good idea to change it. */

#ifndef _H_SCHEME48
#define _H_SCHEME48

/* Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include "write-barrier.h"

typedef long	s48_value;

#define NO_ERRORS 0    /* errno value */

/* Misc stuff */

#define S48_EQ_P(v1, v2) ((v1) == (v2))
/* Superceded name for the above definition, retained for compatibility. */
#define S48_EQ(v1, v2) ((v1) == (v2)) 

#define S48_MAX_FIXNUM_VALUE ((1 << 29) - 1)
#define S48_MIN_FIXNUM_VALUE (-1 << 29)

extern int		s48_stob_has_type(s48_value, int);
extern long		s48_stob_length(s48_value, int);
extern long		s48_stob_byte_length(s48_value, int);
extern s48_value	s48_stob_ref(s48_value, int, long);
extern void		s48_stob_set(s48_value, int, long, s48_value);
extern char		s48_stob_byte_ref(s48_value, int, long);
extern void		s48_stob_byte_set(s48_value, int, long, char);

extern void		s48_register_gc_rootB(char *);
extern void		s48_push_gc_rootsB(char *, long);
extern char		s48_pop_gc_rootsB(void);
extern char		s48_pop_gc_roots_up_to_markerB(char *);

extern s48_value	s48_enter_char(unsigned char);
extern unsigned char	s48_extract_char(s48_value);
extern s48_value	s48_enter_fixnum(long);
extern long		s48_extract_fixnum(s48_value);
extern s48_value	s48_enter_integer(long);
extern long		s48_extract_integer(s48_value);
extern s48_value	s48_enter_double(double);
extern double		s48_extract_double(s48_value);
extern s48_value	s48_cons(s48_value, s48_value);
extern s48_value	s48_enter_string(char *);
extern char *		s48_extract_string(s48_value);
extern s48_value	s48_enter_substring(char *, long);
extern s48_value	s48_enter_byte_vector(char *, long);
extern char *		s48_extract_byte_vector(s48_value);
extern s48_value	s48_make_string(int, char);
extern s48_value	s48_make_vector(int, s48_value);
extern s48_value	s48_make_byte_vector(int);
extern s48_value	s48_make_record(s48_value);
extern s48_value	s48_make_weak_pointer(s48_value);
extern void		s48_check_record_type(s48_value, s48_value);
extern long		s48_length(s48_value);
extern s48_value	s48_enter_pointer(void *);
extern s48_value	s48_get_imported_binding(char *);
extern void		s48_define_exported_binding(char *, s48_value);

extern s48_value	s48_set_channel_os_index(s48_value, long);
extern s48_value	s48_add_channel(s48_value, s48_value, long);
extern void		s48_close_channel(long);

extern s48_value	s48_call_scheme(s48_value proc, long nargs, ...);

#define S48_EXPORT_FUNCTION(p) (s48_define_exported_binding(#p, s48_enter_pointer(p)))

#define S48_MAKE_VALUE(type) (s48_make_byte_vector(sizeof(type)))
extern void *		s48_value_pointer(s48_value);

#define S48_EXTRACT_VALUE_POINTER(x, type) ((type *) s48_value_pointer(x))
#define S48_EXTRACT_VALUE(x, type) (*(S48_EXTRACT_VALUE_POINTER((x), type)))
#define S48_SET_VALUE(x, type, v) (S48_EXTRACT_VALUE((x), type) = (v))

#define S48_UNSAFE_EXTRACT_VALUE_POINTER(x, type)		\
  (S48_ADDRESS_AFTER_HEADER((x), type))
#define S48_UNSAFE_EXTRACT_VALUE(x, type)			\
  (*(S48_UNSAFE_EXTRACT_VALUE_POINTER((x), type)))
#define S48_UNSAFE_SET_VALUE(x, type, v)			\
  (S48_UNSAFE_EXTRACT_VALUE((x), type) = (v))

#define S48_UNSAFE_EXTRACT_DOUBLE(x)				\
  (*(S48_ADDRESS_AFTER_HEADER((x), double)))

#define S48_DECLARE_GC_PROTECT(n) long ___gc_buffer[(n)+2]

#define S48_GC_PROTECT_1(v) \
  (___gc_buffer[2]=(long)&(v), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 1))

#define S48_GC_PROTECT_2(v1, v2) \
  (___gc_buffer[2]=(long)&(v1), ___gc_buffer[3]=(long)&(v2), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 2))

#define S48_GC_PROTECT_3(v1, v2, v3) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 3))

#define S48_GC_PROTECT_4(v1, v2, v3, v4) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 4))

#define S48_GC_PROTECT_5(v1, v2, v3, v4, v5) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 5))

#define S48_GC_PROTECT_6(v1, v2, v3, v4, v5, v6) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 6))

#define S48_GC_PROTECT_7(v1, v2, v3, v4, v5, v6, v7) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   ___gc_buffer[8]=(long)&(v7), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 7))

#define S48_GC_PROTECT_8(v1, v2, v3, v4, v5, v6, v7, v8) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   ___gc_buffer[8]=(long)&(v7), \
   ___gc_buffer[9]=(long)&(v8), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 8))

#define S48_GC_PROTECT_9(v1, v2, v3, v4, v5, v6, v7, v8, v9) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   ___gc_buffer[8]=(long)&(v7), \
   ___gc_buffer[9]=(long)&(v8), \
   ___gc_buffer[10]=(long)&(v9), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 9))

#define S48_GC_PROTECT_10(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   ___gc_buffer[8]=(long)&(v7), \
   ___gc_buffer[9]=(long)&(v8), \
   ___gc_buffer[10]=(long)&(v9), \
   ___gc_buffer[11]=(long)&(v10), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 10))

#define S48_GC_UNPROTECT()				\
   do { if (! s48_pop_gc_rootsB())			\
       	  s48_raise_scheme_exception( S48_EXCEPTION_GC_PROTECTION_MISMATCH, 0); \
      } while(0)

#define S48_GC_PROTECT_GLOBAL(v) (s48_register_gc_rootB((char *)&(v)))

/* Exceptions */

extern void s48_raise_scheme_exception(long type, long nargs, ...);
extern void s48_raise_argument_type_error(s48_value value);
extern void s48_raise_argument_number_error(s48_value value,
				            s48_value min,
					    s48_value max);
extern void s48_raise_range_error(s48_value value,
			          s48_value min,
			          s48_value max);
extern void s48_raise_closed_channel_error();
extern void s48_raise_os_error(int the_errno);
extern void s48_raise_string_os_error(char *reason);
extern void s48_raise_out_of_memory_error();

/* Old names retained for compatibility; use the versions with 'ument_'
   after the '_arg'. */

extern void s48_raise_argument_type_error(s48_value value);
extern void s48_raise_argument_number_error(s48_value value,
				            s48_value min,
					    s48_value max);

/* Type checking */

#define S48_CHECK_PAIR(v) do { if (!S48_PAIR_P(v)) s48_raise_argument_type_error(v); } while (0)
#define S48_CHECK_FIXNUM(v) do { if (!S48_FIXNUM_P(v)) s48_raise_argument_type_error(v); } while (0)
#define S48_CHECK_STRING(v) do { if (!S48_STRING_P(v)) s48_raise_argument_type_error(v); } while (0)
#define S48_CHECK_CHANNEL(v) do { if (!S48_CHANNEL_P(v)) s48_raise_argument_type_error(v); } while (0)
#define S48_CHECK_RECORD(v) do { if (!S48_RECORD_P(v)) s48_raise_argument_type_error(v); } while (0)
#define S48_CHECK_VALUE(v) do { if (!S48_BYTE_VECTOR_P(v)) s48_raise_argument_type_error(v); } while (0)
#define S48_CHECK_EXPORT_BINDING(v) do { if (!S48_EXPORT_BINDING_P(v)) s48_raise_argument_type_error(v); } while (0)
#define S48_CHECK_BOOLEAN(v)					\
  do { s48_value s48_temp = (v);				\
       if (s48_temp != S48_TRUE && s48_temp != S48_FALSE)	\
           s48_raise_argument_type_error(v); } while (0)

#define S48_VALUE_P(v) (S48_BYTE_VECTOR_P(v))

#define S48_TRUE_P(v) ((v) == S48_TRUE)
#define S48_FALSE_P(v) ((v) == S48_FALSE)
#define S48_EXTRACT_BOOLEAN(v) ((v) != S48_FALSE)
#define S48_ENTER_BOOLEAN(v) ((v) ? S48_TRUE : S48_FALSE)

extern void s48_check_record_type(s48_value record, s48_value type_binding);

#define S48_SHARED_BINDING_CHECK(binding)					\
  do { if (S48_UNSPECIFIC == S48_SHARED_BINDING_REF(binding))		\
         s48_raise_scheme_exception(S48_EXCEPTION_UNBOUND_EXTERNAL_NAME, 1,	\
				    S48_SHARED_BINDING_NAME(binding));	\
  } while(0)

#define S48_FIXNUM_TAG 0
#define S48_FIXNUM_P(x) (((long)(x) & 3L) == S48_FIXNUM_TAG)
#define S48_IMMEDIATE_TAG 1
#define S48_IMMEDIATE_P(x) (((long)(x) & 3L) == S48_IMMEDIATE_TAG)
#define S48_HEADER_TAG 2
#define S48_HEADER_P(x) (((long)(x) & 3L) == S48_HEADER_TAG)
#define S48_STOB_TAG 3
#define S48_STOB_P(x) (((long)(x) & 3L) == S48_STOB_TAG)

#define S48_UNSAFE_ENTER_FIXNUM(n)   ((s48_value)((n) << 2))
#define S48_UNSAFE_EXTRACT_FIXNUM(x) ((long)(x) >> 2)

#define S48_MISC_IMMEDIATE(n) ((s48_value)(S48_IMMEDIATE_TAG | ((n) << 2)))
#define S48_FALSE    (S48_MISC_IMMEDIATE(0))
#define S48_TRUE    (S48_MISC_IMMEDIATE(1))
#define S48_CHAR    (S48_MISC_IMMEDIATE(2))
#define S48_UNSPECIFIC    (S48_MISC_IMMEDIATE(3))
#define S48_UNDEFINED    (S48_MISC_IMMEDIATE(4))
#define S48_EOF    (S48_MISC_IMMEDIATE(5))
#define S48_NULL    (S48_MISC_IMMEDIATE(6))
#define S48_UNRELEASED    (S48_MISC_IMMEDIATE(7))

#define S48_UNSAFE_ENTER_CHAR(c) (S48_CHAR | ((c) << 8))
#define S48_UNSAFE_EXTRACT_CHAR(x) ((x) >> 8)
#define S48_CHAR_P(x) ((((long) (x)) & 0xff) == S48_CHAR)

#define S48_ADDRESS_AFTER_HEADER(x, type) ((type *)((x) - S48_STOB_TAG))
#define S48_STOB_REF(x, i) (S48_ADDRESS_AFTER_HEADER(x, s48_value)[i])
#define S48_STOB_BYTE_REF(x, i) (((char *)S48_ADDRESS_AFTER_HEADER(x, s48_value))[i])
#define S48_STOB_SET(x, i, v) do { s48_value __stob_set_x = (x); long __stob_set_i = (i); s48_value __stob_set_v = (v); if (S48_STOB_IMMUTABLEP(__stob_set_x)) s48_raise_argument_type_error(__stob_set_x); else { S48_WRITE_BARRIER((__stob_set_x), (char *) (&S48_STOB_REF((__stob_set_x), (__stob_set_i))),(__stob_set_v)); *(&S48_STOB_REF((__stob_set_x), (__stob_set_i))) = (__stob_set_v); } } while (0)
#define S48_STOB_BYTE_SET(x, i, v) do { s48_value __stob_set_x = (x); long __stob_set_i = (i); char __stob_set_v = (v); if (S48_STOB_IMMUTABLEP(__stob_set_x)) s48_raise_argument_type_error(__stob_set_x); else *(&S48_STOB_BYTE_REF((__stob_set_x), (__stob_set_i))) = (__stob_set_v); } while (0)
#define S48_STOB_TYPE(x)   ((S48_STOB_HEADER(x)>>2)&31)
#define S48_STOB_HEADER(x) (S48_STOB_REF((x),-1))
#define S48_STOB_ADDRESS(x) (&(S48_STOB_HEADER(x)))
#define S48_STOB_BYTE_LENGTH(x) (S48_STOB_HEADER(x) >> 8)
#define S48_STOB_DESCRIPTOR_LENGTH(x) (S48_STOB_HEADER(x) >> 10)
#define S48_STOB_IMMUTABLEP(x) ((S48_STOB_HEADER(x)>>7) & 1)
#define S48_STOB_MAKE_IMMUTABLE(x) ((S48_STOB_HEADER(x)) |= (1<<7))

#define S48_STOBTYPE_PAIR 0
#define S48_PAIR_P(x) (s48_stob_has_type(x, 0))
#define S48_STOBTYPE_SYMBOL 1
#define S48_SYMBOL_P(x) (s48_stob_has_type(x, 1))
#define S48_STOBTYPE_VECTOR 2
#define S48_VECTOR_P(x) (s48_stob_has_type(x, 2))
#define S48_STOBTYPE_CLOSURE 3
#define S48_CLOSURE_P(x) (s48_stob_has_type(x, 3))
#define S48_STOBTYPE_LOCATION 4
#define S48_LOCATION_P(x) (s48_stob_has_type(x, 4))
#define S48_STOBTYPE_CELL 5
#define S48_CELL_P(x) (s48_stob_has_type(x, 5))
#define S48_STOBTYPE_CHANNEL 6
#define S48_CHANNEL_P(x) (s48_stob_has_type(x, 6))
#define S48_STOBTYPE_PORT 7
#define S48_PORT_P(x) (s48_stob_has_type(x, 7))
#define S48_STOBTYPE_RATNUM 8
#define S48_RATNUM_P(x) (s48_stob_has_type(x, 8))
#define S48_STOBTYPE_RECORD 9
#define S48_RECORD_P(x) (s48_stob_has_type(x, 9))
#define S48_STOBTYPE_CONTINUATION 10
#define S48_CONTINUATION_P(x) (s48_stob_has_type(x, 10))
#define S48_STOBTYPE_EXTENDED_NUMBER 11
#define S48_EXTENDED_NUMBER_P(x) (s48_stob_has_type(x, 11))
#define S48_STOBTYPE_TEMPLATE 12
#define S48_TEMPLATE_P(x) (s48_stob_has_type(x, 12))
#define S48_STOBTYPE_WEAK_POINTER 13
#define S48_WEAK_POINTER_P(x) (s48_stob_has_type(x, 13))
#define S48_STOBTYPE_SHARED_BINDING 14
#define S48_SHARED_BINDING_P(x) (s48_stob_has_type(x, 14))
#define S48_STOBTYPE_UNUSED_D_HEADER1 15
#define S48_UNUSED_D_HEADER1_P(x) (s48_stob_has_type(x, 15))
#define S48_STOBTYPE_UNUSED_D_HEADER2 16
#define S48_UNUSED_D_HEADER2_P(x) (s48_stob_has_type(x, 16))
#define S48_STOBTYPE_STRING 17
#define S48_STRING_P(x) (s48_stob_has_type(x, 17))
#define S48_STOBTYPE_BYTE_VECTOR 18
#define S48_BYTE_VECTOR_P(x) (s48_stob_has_type(x, 18))
#define S48_STOBTYPE_DOUBLE 19
#define S48_DOUBLE_P(x) (s48_stob_has_type(x, 19))
#define S48_STOBTYPE_BIGNUM 20
#define S48_BIGNUM_P(x) (s48_stob_has_type(x, 20))

#define S48_CAR_OFFSET 0
#define S48_CAR(x) (s48_stob_ref((x), S48_STOBTYPE_PAIR, 0))
#define S48_UNSAFE_CAR(x) (S48_STOB_REF((x), 0))
#define S48_SET_CAR(x, v) (s48_stob_set((x), S48_STOBTYPE_PAIR, 0, (v)))
#define S48_UNSAFE_SET_CAR(x, v) S48_STOB_SET((x), 0, (v))
#define S48_CDR_OFFSET 1
#define S48_CDR(x) (s48_stob_ref((x), S48_STOBTYPE_PAIR, 1))
#define S48_UNSAFE_CDR(x) (S48_STOB_REF((x), 1))
#define S48_SET_CDR(x, v) (s48_stob_set((x), S48_STOBTYPE_PAIR, 1, (v)))
#define S48_UNSAFE_SET_CDR(x, v) S48_STOB_SET((x), 1, (v))
#define S48_SYMBOL_TO_STRING_OFFSET 0
#define S48_SYMBOL_TO_STRING(x) (s48_stob_ref((x), S48_STOBTYPE_SYMBOL, 0))
#define S48_UNSAFE_SYMBOL_TO_STRING(x) (S48_STOB_REF((x), 0))
#define S48_LOCATION_ID_OFFSET 0
#define S48_LOCATION_ID(x) (s48_stob_ref((x), S48_STOBTYPE_LOCATION, 0))
#define S48_UNSAFE_LOCATION_ID(x) (S48_STOB_REF((x), 0))
#define S48_SET_LOCATION_ID(x, v) (s48_stob_set((x), S48_STOBTYPE_LOCATION, 0, (v)))
#define S48_UNSAFE_SET_LOCATION_ID(x, v) S48_STOB_SET((x), 0, (v))
#define S48_CONTENTS_OFFSET 1
#define S48_CONTENTS(x) (s48_stob_ref((x), S48_STOBTYPE_LOCATION, 1))
#define S48_UNSAFE_CONTENTS(x) (S48_STOB_REF((x), 1))
#define S48_SET_CONTENTS(x, v) (s48_stob_set((x), S48_STOBTYPE_LOCATION, 1, (v)))
#define S48_UNSAFE_SET_CONTENTS(x, v) S48_STOB_SET((x), 1, (v))
#define S48_CELL_REF_OFFSET 0
#define S48_CELL_REF(x) (s48_stob_ref((x), S48_STOBTYPE_CELL, 0))
#define S48_UNSAFE_CELL_REF(x) (S48_STOB_REF((x), 0))
#define S48_CELL_SET(x, v) (s48_stob_set((x), S48_STOBTYPE_CELL, 0, (v)))
#define S48_UNSAFE_CELL_SET(x, v) S48_STOB_SET((x), 0, (v))
#define S48_CLOSURE_TEMPLATE_OFFSET 0
#define S48_CLOSURE_TEMPLATE(x) (s48_stob_ref((x), S48_STOBTYPE_CLOSURE, 0))
#define S48_UNSAFE_CLOSURE_TEMPLATE(x) (S48_STOB_REF((x), 0))
#define S48_CLOSURE_ENV_OFFSET 1
#define S48_CLOSURE_ENV(x) (s48_stob_ref((x), S48_STOBTYPE_CLOSURE, 1))
#define S48_UNSAFE_CLOSURE_ENV(x) (S48_STOB_REF((x), 1))
#define S48_WEAK_POINTER_REF_OFFSET 0
#define S48_WEAK_POINTER_REF(x) (s48_stob_ref((x), S48_STOBTYPE_WEAK_POINTER, 0))
#define S48_UNSAFE_WEAK_POINTER_REF(x) (S48_STOB_REF((x), 0))
#define S48_SHARED_BINDING_NAME_OFFSET 0
#define S48_SHARED_BINDING_NAME(x) (s48_stob_ref((x), S48_STOBTYPE_SHARED_BINDING, 0))
#define S48_UNSAFE_SHARED_BINDING_NAME(x) (S48_STOB_REF((x), 0))
#define S48_SHARED_BINDING_IS_IMPORTP_OFFSET 1
#define S48_SHARED_BINDING_IS_IMPORTP(x) (s48_stob_ref((x), S48_STOBTYPE_SHARED_BINDING, 1))
#define S48_UNSAFE_SHARED_BINDING_IS_IMPORTP(x) (S48_STOB_REF((x), 1))
#define S48_SHARED_BINDING_REF_OFFSET 2
#define S48_SHARED_BINDING_REF(x) (s48_stob_ref((x), S48_STOBTYPE_SHARED_BINDING, 2))
#define S48_UNSAFE_SHARED_BINDING_REF(x) (S48_STOB_REF((x), 2))
#define S48_SHARED_BINDING_SET(x, v) (s48_stob_set((x), S48_STOBTYPE_SHARED_BINDING, 2, (v)))
#define S48_UNSAFE_SHARED_BINDING_SET(x, v) S48_STOB_SET((x), 2, (v))
#define S48_PORT_HANDLER_OFFSET 0
#define S48_PORT_HANDLER(x) (s48_stob_ref((x), S48_STOBTYPE_PORT, 0))
#define S48_UNSAFE_PORT_HANDLER(x) (S48_STOB_REF((x), 0))
#define S48_PORT_STATUS_OFFSET 1
#define S48_PORT_STATUS(x) (s48_stob_ref((x), S48_STOBTYPE_PORT, 1))
#define S48_UNSAFE_PORT_STATUS(x) (S48_STOB_REF((x), 1))
#define S48_SET_PORT_STATUS(x, v) (s48_stob_set((x), S48_STOBTYPE_PORT, 1, (v)))
#define S48_UNSAFE_SET_PORT_STATUS(x, v) S48_STOB_SET((x), 1, (v))
#define S48_PORT_LOCK_OFFSET 2
#define S48_PORT_LOCK(x) (s48_stob_ref((x), S48_STOBTYPE_PORT, 2))
#define S48_UNSAFE_PORT_LOCK(x) (S48_STOB_REF((x), 2))
#define S48_SET_PORT_LOCK(x, v) (s48_stob_set((x), S48_STOBTYPE_PORT, 2, (v)))
#define S48_UNSAFE_SET_PORT_LOCK(x, v) S48_STOB_SET((x), 2, (v))
#define S48_PORT_DATA_OFFSET 3
#define S48_PORT_DATA(x) (s48_stob_ref((x), S48_STOBTYPE_PORT, 3))
#define S48_UNSAFE_PORT_DATA(x) (S48_STOB_REF((x), 3))
#define S48_SET_PORT_DATA(x, v) (s48_stob_set((x), S48_STOBTYPE_PORT, 3, (v)))
#define S48_UNSAFE_SET_PORT_DATA(x, v) S48_STOB_SET((x), 3, (v))
#define S48_PORT_BUFFER_OFFSET 4
#define S48_PORT_BUFFER(x) (s48_stob_ref((x), S48_STOBTYPE_PORT, 4))
#define S48_UNSAFE_PORT_BUFFER(x) (S48_STOB_REF((x), 4))
#define S48_SET_PORT_BUFFER(x, v) (s48_stob_set((x), S48_STOBTYPE_PORT, 4, (v)))
#define S48_UNSAFE_SET_PORT_BUFFER(x, v) S48_STOB_SET((x), 4, (v))
#define S48_PORT_INDEX_OFFSET 5
#define S48_PORT_INDEX(x) (s48_stob_ref((x), S48_STOBTYPE_PORT, 5))
#define S48_UNSAFE_PORT_INDEX(x) (S48_STOB_REF((x), 5))
#define S48_SET_PORT_INDEX(x, v) (s48_stob_set((x), S48_STOBTYPE_PORT, 5, (v)))
#define S48_UNSAFE_SET_PORT_INDEX(x, v) S48_STOB_SET((x), 5, (v))
#define S48_PORT_LIMIT_OFFSET 6
#define S48_PORT_LIMIT(x) (s48_stob_ref((x), S48_STOBTYPE_PORT, 6))
#define S48_UNSAFE_PORT_LIMIT(x) (S48_STOB_REF((x), 6))
#define S48_SET_PORT_LIMIT(x, v) (s48_stob_set((x), S48_STOBTYPE_PORT, 6, (v)))
#define S48_UNSAFE_SET_PORT_LIMIT(x, v) S48_STOB_SET((x), 6, (v))
#define S48_PORT_PENDING_EOFP_OFFSET 7
#define S48_PORT_PENDING_EOFP(x) (s48_stob_ref((x), S48_STOBTYPE_PORT, 7))
#define S48_UNSAFE_PORT_PENDING_EOFP(x) (S48_STOB_REF((x), 7))
#define S48_SET_PORT_PENDING_EOFP(x, v) (s48_stob_set((x), S48_STOBTYPE_PORT, 7, (v)))
#define S48_UNSAFE_SET_PORT_PENDING_EOFP(x, v) S48_STOB_SET((x), 7, (v))
#define S48_CHANNEL_STATUS_OFFSET 0
#define S48_CHANNEL_STATUS(x) (s48_stob_ref((x), S48_STOBTYPE_CHANNEL, 0))
#define S48_UNSAFE_CHANNEL_STATUS(x) (S48_STOB_REF((x), 0))
#define S48_CHANNEL_ID_OFFSET 1
#define S48_CHANNEL_ID(x) (s48_stob_ref((x), S48_STOBTYPE_CHANNEL, 1))
#define S48_UNSAFE_CHANNEL_ID(x) (S48_STOB_REF((x), 1))
#define S48_CHANNEL_OS_INDEX_OFFSET 2
#define S48_CHANNEL_OS_INDEX(x) (s48_stob_ref((x), S48_STOBTYPE_CHANNEL, 2))
#define S48_UNSAFE_CHANNEL_OS_INDEX(x) (S48_STOB_REF((x), 2))

#define S48_VECTOR_LENGTH(x) (s48_stob_length((x), S48_STOBTYPE_VECTOR))
#define S48_UNSAFE_VECTOR_LENGTH(x) (S48_STOB_DESCRIPTOR_LENGTH(x))
#define S48_VECTOR_REF(x, i) (s48_stob_ref((x), S48_STOBTYPE_VECTOR, (i)))
#define S48_VECTOR_SET(x, i, v) (s48_stob_set((x), S48_STOBTYPE_VECTOR, (i), (v)))
#define S48_UNSAFE_VECTOR_REF(x, i) (S48_STOB_REF((x), (i)))
#define S48_UNSAFE_VECTOR_SET(x, i, v) S48_STOB_SET((x), (i), (v))
#define S48_RECORD_LENGTH(x) (s48_stob_length((x), S48_STOBTYPE_RECORD))
#define S48_UNSAFE_RECORD_LENGTH(x) (S48_STOB_DESCRIPTOR_LENGTH(x))
#define S48_RECORD_REF(x, i) (s48_stob_ref((x), S48_STOBTYPE_RECORD, (i) + 1))
#define S48_RECORD_SET(x, i, v) (s48_stob_set((x), S48_STOBTYPE_RECORD, (i) + 1, (v)))
#define S48_UNSAFE_RECORD_REF(x, i) (S48_STOB_REF((x), (i) + 1))
#define S48_UNSAFE_RECORD_SET(x, i, v) S48_STOB_SET((x), (i) + 1, (v))
#define S48_RECORD_TYPE(x) (s48_stob_ref((x), S48_STOBTYPE_RECORD, 0))
#define S48_UNSAFE_RECORD_TYPE(x) (S48_STOB_REF((x), 0))
#define S48_BYTE_VECTOR_LENGTH(x) (s48_stob_byte_length((x), S48_STOBTYPE_BYTE_VECTOR))
#define S48_BYTE_VECTOR_REF(x, i) (s48_stob_byte_ref((x), S48_STOBTYPE_BYTE_VECTOR, (i)))
#define S48_BYTE_VECTOR_SET(x, i, v) (s48_stob_byte_set((x), S48_STOBTYPE_BYTE_VECTOR, (i), (v)))
#define S48_UNSAFE_BYTE_VECTOR_REF(x, i) (S48_STOB_BYTE_REF((x), (i)))
#define S48_UNSAFE_BYTE_VECTOR_SET(x, i, v) S48_BYTE_STOB_SET((x), (i), (v))
#define S48_STRING_LENGTH(x) (s48_stob_byte_length((x), S48_STOBTYPE_STRING))
#define S48_STRING_REF(x, i) (s48_stob_byte_ref((x), S48_STOBTYPE_STRING, (i)))
#define S48_STRING_SET(x, i, v) (s48_stob_byte_set((x), S48_STOBTYPE_STRING, (i), (v)))
#define S48_UNSAFE_STRING_REF(x, i) (S48_STOB_BYTE_REF((x), (i)))
#define S48_UNSAFE_STRING_SET(x, i, v) S48_BYTE_STOB_SET((x), (i), (v))
#define S48_UNSAFE_BYTE_VECTOR_LENGTH(x) (S48_STOB_BYTE_LENGTH(x))
#define S48_UNSAFE_STRING_LENGTH(x) (S48_STOB_BYTE_LENGTH(x) - 1)
#define S48_UNSAFE_EXTRACT_STRING(x) (S48_ADDRESS_AFTER_HEADER((x), char))
#define S48_EXTRACT_EXTERNAL_OBJECT(x, type) ((type *)(S48_ADDRESS_AFTER_HEADER(x, long)+1))

#define S48_RECORD_TYPE_RESUMER(x) S48_RECORD_REF((x), 0)
#define S48_RECORD_TYPE_UID(x) S48_RECORD_REF((x), 1)
#define S48_RECORD_TYPE_NAME(x) S48_RECORD_REF((x), 2)
#define S48_RECORD_TYPE_FIELD_NAMES(x) S48_RECORD_REF((x), 3)
#define S48_RECORD_TYPE_NUMBER_OF_FIELDS(x) S48_RECORD_REF((x), 4)
#define S48_RECORD_TYPE_DISCLOSER(x) S48_RECORD_REF((x), 5)

#define S48_EXCEPTION_UNASSIGNED_LOCAL 0
#define S48_EXCEPTION_UNDEFINED_GLOBAL 1
#define S48_EXCEPTION_UNBOUND_GLOBAL 2
#define S48_EXCEPTION_BAD_PROCEDURE 3
#define S48_EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS 4
#define S48_EXCEPTION_WRONG_TYPE_ARGUMENT 5
#define S48_EXCEPTION_ARITHMETIC_OVERFLOW 6
#define S48_EXCEPTION_INDEX_OUT_OF_RANGE 7
#define S48_EXCEPTION_HEAP_OVERFLOW 8
#define S48_EXCEPTION_OUT_OF_MEMORY 9
#define S48_EXCEPTION_CANNOT_OPEN_CHANNEL 10
#define S48_EXCEPTION_CHANNEL_OS_INDEX_ALREADY_IN_USE 11
#define S48_EXCEPTION_CLOSED_CHANNEL 12
#define S48_EXCEPTION_BUFFER_FULLEMPTY 13
#define S48_EXCEPTION_UNIMPLEMENTED_INSTRUCTION 14
#define S48_EXCEPTION_TRAP 15
#define S48_EXCEPTION_PROCEEDING_AFTER_EXCEPTION 16
#define S48_EXCEPTION_BAD_OPTION 17
#define S48_EXCEPTION_UNBOUND_EXTERNAL_NAME 18
#define S48_EXCEPTION_TOO_MANY_ARGUMENTS_TO_EXTERNAL_PROCEDURE 19
#define S48_EXCEPTION_TOO_MANY_ARGUMENTS_IN_CALLBACK 20
#define S48_EXCEPTION_CALLBACK_RETURN_UNCOVERED 21
#define S48_EXCEPTION_EXTENSION_EXCEPTION 22
#define S48_EXCEPTION_EXTENSION_RETURN_ERROR 23
#define S48_EXCEPTION_OS_ERROR 24
#define S48_EXCEPTION_GC_PROTECTION_MISMATCH 25
#define S48_EXCEPTION_NO_CURRENT_PROPOSAL 26
#define S48_EXCEPTION_NATIVE_CODE_NOT_SUPPORTED 27

#define S48_CHANNEL_STATUS_CLOSED S48_UNSAFE_ENTER_FIXNUM(0)
#define S48_CHANNEL_STATUS_INPUT S48_UNSAFE_ENTER_FIXNUM(1)
#define S48_CHANNEL_STATUS_OUTPUT S48_UNSAFE_ENTER_FIXNUM(2)
#define S48_CHANNEL_STATUS_SPECIAL_INPUT S48_UNSAFE_ENTER_FIXNUM(3)
#define S48_CHANNEL_STATUS_SPECIAL_OUTPUT S48_UNSAFE_ENTER_FIXNUM(4)

#endif /* _H_SCHEME48 */
