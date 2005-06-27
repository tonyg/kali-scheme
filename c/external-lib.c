/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Access to various Scheme-side libraries via the FFI
 */

#include "scheme48.h"

/*
 * Enum sets
 */

static s48_value enum_set_type_binding = S48_FALSE;

/*
 * This needs to be in synch with the layout of :ENUM-SET in enum-set.scm
 */

static void
check_enum_set(s48_value sch_thing)
{
  s48_check_record_type(sch_thing, enum_set_type_binding);
}

void
s48_check_enum_set_type(s48_value sch_thing, s48_value sch_enum_set_type_binding)
{
  check_enum_set(sch_thing);
  {
    s48_value actual_type = S48_UNSAFE_RECORD_REF(sch_thing, 0);
    s48_value binding_val = S48_SHARED_BINDING_REF(sch_enum_set_type_binding);
    s48_value unspecific = S48_UNSPECIFIC;
  
      
  if (!S48_EQ_P(S48_UNSAFE_RECORD_REF(sch_thing, 0),
		S48_SHARED_BINDING_REF(sch_enum_set_type_binding)))
    s48_raise_argument_type_error(sch_thing);
  }
}

long
s48_enum_set2integer(s48_value sch_enum_set)
{
  check_enum_set(sch_enum_set);
  return s48_extract_fixnum(S48_UNSAFE_RECORD_REF(sch_enum_set, 1));
}

s48_value
s48_integer2enum_set(s48_value sch_enum_set_type_binding, long mask)
{
  s48_value sch_enum_set = s48_make_record(enum_set_type_binding);
  S48_UNSAFE_RECORD_SET(sch_enum_set, 0, S48_SHARED_BINDING_REF(sch_enum_set_type_binding));
  S48_UNSAFE_RECORD_SET(sch_enum_set, 1, s48_enter_fixnum(mask));
  return sch_enum_set;
}

void
s48_init_external_libs(void)
{

  S48_GC_PROTECT_GLOBAL(enum_set_type_binding);
  enum_set_type_binding = s48_get_imported_binding("enum-set-type");
}
