/*  
  Copyright (c) 2005, Andreas Rottmann

  This code is based on the foreign interface for PLT Scheme (foreign.c),
  
    Copyright (c) 2004 Eli Barzilay

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Library General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public  License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

#include "ffi48.h"

#include <ffi.h>

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#define ALIGN(v, a)  (((((size_t) (v))-1) | ((a)-1))+1)

#ifdef WORDS_BIGENDIAN
#  define RVALUE_PTR(ptr, typ) \
             ((void *)((char *)(ptr) + (sizeof(ffi_arg) - (typ)->size)))
#else
#  define RVALUE_PTR(ptr, typ) ((void *)(ptr))
#endif

typedef struct _s48_ffi_type s48_ffi_type_t;
struct _s48_ffi_type
{
  ffi_type *    type;
  void          (*scheme_to_c) (void * result,
                                s48_value value);
  s48_value     (*c_to_scheme) (void * value);
};

/*
 * Types
 */

#define DEFINE_TYPE(NAME, FFI_TYPE, TO_C, TO_SCHEME) \
static void                                          \
NAME ## _to_c (void * result, s48_value  value)      \
{                                                    \
  TO_C;                                              \
}                                                    \
static s48_value                                     \
NAME ## _to_scheme (void * value)                    \
{                                                    \
  TO_SCHEME;                                         \
}                                                    \
static s48_ffi_type_t type_ ## NAME = {              \
  &ffi_type_ ## FFI_TYPE,                            \
  NAME ## _to_c,                                     \
  NAME ## _to_scheme                                 \
}

#define DEFINE_INT_TYPE(NAME, FFI_NAME, C_TYPE) \
    DEFINE_TYPE (NAME, FFI_NAME, \
                 *(C_TYPE *) result = s48_extract_integer (value), \
                 return s48_enter_integer (*(C_TYPE *) value))

DEFINE_TYPE(void, void, , return S48_UNSPECIFIC);
DEFINE_TYPE(bool, sint, 
            *(int *) result = S48_EXTRACT_BOOLEAN (value),
            return *(int *) value ? S48_TRUE : S48_FALSE);
DEFINE_INT_TYPE(int8,   sint8,  int8_t);
DEFINE_INT_TYPE(uint8,  uint8,  uint8_t);
DEFINE_INT_TYPE(int16,  sint16, int16_t);
DEFINE_INT_TYPE(uint16, uint16, uint16_t);
DEFINE_INT_TYPE(int32,  sint32, int32_t);
DEFINE_INT_TYPE(uint32, uint32, uint32_t);
DEFINE_INT_TYPE(int64,  sint64, int64_t);
DEFINE_INT_TYPE(uint64, uint64, uint64_t);
DEFINE_TYPE(float, float,
            *(float *) result = s48_extract_double (value),
            return s48_enter_double (*(float *) value));
DEFINE_TYPE(double, double,
            *(double *) result = s48_extract_double (value),
            return s48_enter_double (*(double *) value));
DEFINE_TYPE(pointer, pointer,
            *(void **)result = s48_ffi_ptr_mem (value),
            do {
              s48_value result = S48_MAKE_VALUE (void *);
              S48_SET_VALUE (result, void *, *(void **) value);
              return result;
            } while (0));
DEFINE_TYPE(fpointer, pointer,
            pointer_to_c (result, value),
            return pointer_to_scheme (value));

static s48_value string_to_symbol_binding = S48_FALSE;

static s48_value
s48_enter_symbol (const char *name)
{
  if (S48_FALSE_P (string_to_symbol_binding))
    string_to_symbol_binding = s48_get_imported_binding ("string->symbol");
  
  return s48_call_scheme (S48_SHARED_BINDING_REF (string_to_symbol_binding), 1,
                          s48_enter_string ((char *) name));
}


/*
 * Library object. This is just a byte vectoring storing the "void *"
 * handle we get from dlopen.
 */

s48_value
s48_make_ffi_lib (s48_value name,
                  s48_value no_error_p)
{
  return s48_c_make_ffi_lib (s48_extract_string (name),
                             S48_EXTRACT_BOOLEAN (no_error_p));
}

s48_value
s48_c_make_ffi_lib (const char *name, int no_error_p)
{
  void *handle;

  handle = dlopen (name, RTLD_NOW);
  if (!handle)
    {
      if (no_error_p)
        return S48_FALSE;
      else
        s48_raise_string_os_error (dlerror ());
    }
  else
    {
      s48_value result = S48_MAKE_VALUE (void *);
      S48_SET_VALUE (result, void *, handle);
      return result;
    }
  return S48_UNSPECIFIC; /* hush compiler */
}

s48_value
s48_make_ffi_obj (s48_value  lib,
                  s48_value  name)
{
  return s48_c_make_ffi_obj (lib, s48_extract_string (name));
}

s48_value
s48_c_make_ffi_obj (s48_value  lib,
                    const char * name)
{
  void *handle;
  void *dlobj;
  
  handle = S48_EXTRACT_VALUE (lib, void *);
  dlobj = dlsym (handle, name);
  if (!dlobj)
    {
      char *err = dlerror();
      if (err)
        s48_raise_string_os_error (err);
      return S48_FALSE;
    }
  else
    {
      s48_value result = S48_MAKE_VALUE (void *);
      S48_SET_VALUE (result, void *, dlobj);
      return result;
    }
}

typedef struct
{
  ffi_cif cif;
  size_t data_area_size;
  s48_ffi_type_t *rtype;
  s48_ffi_type_t *itypes[0];
} s48_ffi_cif_t;

s48_value
s48_make_ffi_cif (s48_value  itypes,
                  s48_value  otype)
{
  ffi_type *rtype, **atypes;
  s48_ffi_cif_t *s_cif;
  int nargs, i;
  s48_value lst;
  s48_value val;
  s48_value result;
  S48_DECLARE_GC_PROTECT (1);

  S48_GC_PROTECT_1 (itypes);
  
  nargs = S48_UNSAFE_EXTRACT_FIXNUM (s48_length (itypes));
  s_cif = malloc (sizeof (s48_ffi_cif_t) + nargs * sizeof (s48_ffi_type_t *));
  
  s_cif->rtype = S48_EXTRACT_VALUE (otype, s48_ffi_type_t *);
  rtype = s_cif->rtype->type;
  
  /* Data is used by ffi_call, so don't free it */
  atypes = malloc (nargs * sizeof (ffi_type *)); // FIXME: use xmalloc()
  lst = itypes;
  for (i = 0; i < nargs; i++)
    {
      val = S48_CAR (lst);
      s_cif->itypes[i] = S48_EXTRACT_VALUE (val, s48_ffi_type_t *);
      atypes[i] = s_cif->itypes[i]->type;
      
      lst = S48_CDR (lst);
    }

  S48_GC_UNPROTECT ();
  
  s_cif->data_area_size = nargs * sizeof (void *);
  
  if (ffi_prep_cif (&s_cif->cif, FFI_DEFAULT_ABI,
                    nargs, rtype, atypes) != FFI_OK)
    s48_raise_string_os_error ("internal: ffi_prep_cif() failed");

  /* note that we treat the space for the rvalue specially; libffi
   * says it might need up to sizeof(ffi_arg) bytes of
   * word-aligned memory for it, so we put it first (so it should
   * be aligned on word boundary) and make it at least
   * sizeof(ffi_arg) bytes. */
  s_cif->data_area_size += (rtype->size > sizeof(ffi_arg) ?
                            rtype->size : sizeof(ffi_arg));
      
  /* now we know the sizes of the types and calculate the data
   * area size where we store the arguments' values */
  for (i = 0; i < nargs; i++)
    {
      s_cif->data_area_size = ALIGN(s_cif->data_area_size, atypes[i]->alignment);
      s_cif->data_area_size += atypes[i]->size;
    }
      
  result = S48_MAKE_VALUE (s48_ffi_cif_t *);
  S48_SET_VALUE (result, s48_ffi_cif_t *, s_cif);
  return result;
}

s48_value
s48_ffi_call (s48_value  obj,
              s48_value  cif,
              s48_value  args)
{
  s48_ffi_cif_t *s_cif;
  void *proc;
  void *data;
  void *rvalue, **values;
  unsigned offset;
  long i;
  s48_value lst;
  s48_value val;
  S48_DECLARE_GC_PROTECT (2);

  S48_GC_PROTECT_2 (lst, val);
  
  s_cif = S48_EXTRACT_VALUE (cif, s48_ffi_cif_t *);
  
  data = alloca (s_cif->data_area_size);
  values = (void **) data;

  offset = s_cif->cif.nargs * sizeof (void *);
  rvalue = (void *) ((unsigned char *) data + offset);
  offset += (s_cif->rtype->type->size > sizeof(ffi_arg)
             ? s_cif->rtype->type->size : sizeof(ffi_arg));
  
  lst = args;
  for (i = 0; i < s_cif->cif.nargs; i++)
    {
      offset = ALIGN (offset, s_cif->cif.arg_types[i]->alignment);
      values[i] = (void *) ((uint8_t *) data + offset);
      if (!S48_PAIR_P (lst))
        s48_raise_argument_type_error (args);
      val = S48_CAR (lst);
      s_cif->itypes[i]->scheme_to_c (values[i], val);
      offset += s_cif->cif.arg_types[i]->size;
      lst = S48_CDR (lst);
    }

  proc = S48_EXTRACT_VALUE (obj, void *);

  fprintf (stderr, "proc: %p, cif: %p\n", proc, &s_cif->cif);
  
  ffi_call (&s_cif->cif, proc, rvalue, values);
  rvalue = RVALUE_PTR (rvalue, s_cif->rtype->type); /* adjust on bigendian */

  S48_GC_UNPROTECT ();
  
  return s_cif->rtype->c_to_scheme (rvalue);
}

s48_value
s48_ffi_ptr_ref (s48_value  pointer,
                 s48_value  type)
{
  void *p;
  s48_ffi_type_t *t;
  
  p = s48_ffi_ptr_mem (pointer);
  if (p == NULL)
        s48_raise_argument_type_error (pointer);
  t = S48_EXTRACT_VALUE (type, s48_ffi_type_t *);

  if (t == &type_fpointer)
    return t->c_to_scheme (&p);
  else
    return t->c_to_scheme (p);
}

void
s48_ffi_ptr_set (s48_value ptr, s48_value type, s48_value value)
{
  void *p;
  s48_ffi_type_t *t;

  p = s48_ffi_ptr_mem (ptr);
  if (p == NULL)
    s48_raise_argument_type_error (ptr);
  t = S48_EXTRACT_VALUE (type, s48_ffi_type_t *);
  
  t->scheme_to_c (p, value);
}

void *
s48_ffi_ptr_mem (s48_value ptr)
{
  if (S48_FALSE_P (ptr))
    return NULL;
  else if (S48_STRING_P (ptr))
    {
      return s48_extract_string (ptr);
    }
  else
    {
      size_t len = S48_BYTE_VECTOR_LENGTH (ptr);
      if (len == sizeof (void *))
        return S48_EXTRACT_VALUE (ptr, void *);
      else
        return s48_value_pointer (ptr);
    }
}

int
s48_ffi_is_ptr (s48_value ptr)
{
  return (S48_FALSE_P (ptr) || S48_BYTE_VECTOR_P (ptr));
}

static s48_value int_sym = S48_FALSE,
  char_sym = S48_FALSE,
  void_sym = S48_FALSE,
  short_sym = S48_FALSE,
  long_sym = S48_FALSE,
  float_sym = S48_FALSE,
  double_sym = S48_FALSE,
  star_sym = S48_FALSE;

/* (compiler-sizeof symbols) -> int, where symbols name some C type.
 * The symbols are in 'int 'char 'void 'short 'long '*, order does not matter,
 * when a single symbol is used, a list is not needed.
 * (This is about actual C types, not C type objects.) */
s48_value
s48_ffi_compiler_sizeof (s48_value thing)
{
  enum { NONE, INT, CHAR, VOID, FLOAT, DOUBLE } basetype = NONE;
  int intsize = 0; /* "short" => decrement, "long" => increment */
  int stars = 0;    /* number of "*"s */
  int res;
  s48_value l, p;
  S48_DECLARE_GC_PROTECT (2);

  if (S48_FALSE_P (int_sym))
    {
      int_sym = s48_enter_symbol ("int");
      char_sym = s48_enter_symbol ("char");
      void_sym = s48_enter_symbol ("void");
      short_sym = s48_enter_symbol ("short");
      long_sym = s48_enter_symbol ("long");
      float_sym = s48_enter_symbol ("float");
      double_sym = s48_enter_symbol ("double");
      star_sym = s48_enter_symbol ("*");
    }
  
  S48_GC_PROTECT_2 (l, p);
  l = thing;
  while (!S48_EQ_P(l, S48_NULL))
    {
      if (S48_PAIR_P (l))
        {
          p = S48_CAR (l);
          l = S48_CDR (l);
        }
      else
        {
          p = l;
          l = S48_NULL;
        }
      if (!S48_SYMBOL_P (p))
        s48_raise_argument_type_error (thing);
      else if (S48_EQ_P (p, int_sym))
        if (basetype == NONE)
          basetype = INT;
        else
          s48_raise_argument_type_error (thing);
      else if (S48_EQ_P (p, char_sym))
        if (basetype == NONE)
          basetype = CHAR;
        else
          s48_raise_argument_type_error (thing);
      else if (S48_EQ_P (p, void_sym))
        if (basetype == NONE)
          basetype = VOID;
        else
          s48_raise_argument_type_error (thing);
      else if (S48_EQ_P (p, float_sym))
        if (basetype == NONE)
          basetype = FLOAT;
        else
          s48_raise_argument_type_error (thing);
      else if (S48_EQ_P (p, double_sym))
        if (basetype == NONE)
          basetype = DOUBLE;
        else
          s48_raise_argument_type_error (thing);
      else if (S48_EQ_P (p, short_sym))
        if (intsize <= 0)
          intsize--;
        else
          s48_raise_argument_type_error (thing);
      else if (S48_EQ_P (p, long_sym))
        if (intsize >= 0)
          intsize++;
        else
          s48_raise_argument_type_error (thing);
      else if  (S48_EQ_P (p, star_sym))
        stars++;
      else
        s48_raise_argument_type_error (thing);
    }
  if (stars > 1 || intsize < -1 || intsize > 2) 
    s48_raise_argument_type_error (thing);
  if (basetype == NONE)
    basetype = INT; 
  /* don't assume anything, so it can be used to verify compiler assumptions */
  /* (only forbid stuff that the compiler doesn't allow) */
#define RETSIZE(t) res=((stars==0)?sizeof(t):sizeof(t *))
  switch (basetype)
    {
    case INT:
      switch (intsize)
        {
        case 0:  RETSIZE (int); break;
        case 1:  RETSIZE (long int); break;
        case 2:  RETSIZE (long long int); break;
        case -1: RETSIZE (short int); break;
        }
      break;
    case CHAR:
      if (intsize == 0) RETSIZE (char);
      else              s48_raise_argument_type_error (thing);
      break;
    case VOID:
      if (intsize == 0) RETSIZE(void);
      else              s48_raise_argument_type_error (thing);
      break;
    case FLOAT: 
      if (intsize == 0) RETSIZE(float);
      else              s48_raise_argument_type_error (thing);
      break; 
    case DOUBLE: 
      if (intsize == 0)       RETSIZE(double);
      else if (intsize == -1) RETSIZE(long double);
      else                    s48_raise_argument_type_error (thing);
      break;
    default:
      assert (0);
    }
#undef RETSIZE
  S48_GC_UNPROTECT ();
  
  return s48_enter_integer (res);
}

s48_value
s48_ffi_malloc (s48_value type, s48_value count, s48_value cptr, s48_value raw_p)
{
  s48_ffi_type_t *t;
  void *mem;
  long cnt;
  int alloc_raw;
  size_t total_size;
  s48_value result;
  S48_DECLARE_GC_PROTECT (1);

  S48_GC_PROTECT_1 (result);
  
  t = S48_FALSE_P(type) ? NULL : S48_EXTRACT_VALUE (type, s48_ffi_type_t *);
  if (t && t->type->size <= 0)
    s48_raise_argument_type_error (type);
  cnt = s48_extract_integer (count);
  if (cnt <= 0)
    s48_raise_range_error (count,
                           s48_enter_fixnum(1),
                           s48_enter_fixnum(S48_MAX_FIXNUM_VALUE));
  alloc_raw = S48_EXTRACT_BOOLEAN (raw_p);
  
  total_size = (t ? t->type->size : 1) * cnt;
  if (alloc_raw)
    {
      mem = malloc (total_size);
      result = S48_MAKE_VALUE (void *);
      S48_SET_VALUE (result, void *, mem);
    }
  else
    {
      result = s48_make_byte_vector (total_size);
      mem = (char *) s48_value_pointer (result);
    }

  if (!S48_FALSE_P (cptr))
    {
      void *old_mem = s48_ffi_ptr_mem (cptr);
      memcpy (mem, old_mem, total_size);
    }

  S48_GC_UNPROTECT ();
  
  return result;
}

void *
s48_ffi_extract_raw_ptr (s48_value ptr)
{
  size_t len = S48_BYTE_VECTOR_LENGTH (ptr);
  if (len != sizeof (void *))
    s48_raise_argument_type_error (ptr);
  else
    return S48_EXTRACT_VALUE (ptr, void *);
  return NULL; /* hush compiler */
}

s48_value
s48_ffi_free (s48_value mem)
{
  void *ptr = s48_ffi_extract_raw_ptr (mem);
  if (ptr == NULL)
    s48_raise_argument_type_error (mem);
  free (ptr);
  return S48_UNSPECIFIC;
}

s48_value
s48_ffi_cpointer_to_string (s48_value cptr)
{
  return s48_enter_string ((char *) s48_ffi_ptr_mem (cptr));
}

/*
 * Trampolines
 */
static s48_value
ffi_make_lib (s48_value name, s48_value no_error_p)
{
  s48_value result;
  S48_DECLARE_GC_PROTECT (3);
  
  S48_GC_PROTECT_3 (name, no_error_p, result);
  result = s48_make_ffi_lib (name, no_error_p);
  S48_GC_UNPROTECT ();
  
  return result;
}

static s48_value
ffi_make_obj (s48_value lib, s48_value name)
{
  s48_value result;
  S48_DECLARE_GC_PROTECT (3);
  
  S48_GC_PROTECT_3 (lib, name, result);
  result = s48_make_ffi_obj (lib, name);
  S48_GC_UNPROTECT ();
  
  return result;
}

static s48_value
ffi_malloc (s48_value type, s48_value count, s48_value cptr, s48_value raw_p)
{
  s48_value result;
  S48_DECLARE_GC_PROTECT (5);

  S48_GC_PROTECT_5 (type, count, cptr, raw_p, result);
  result = s48_ffi_malloc (type, count, cptr, raw_p);
  S48_GC_UNPROTECT ();

  return result;
}

static s48_value
ffi_free (s48_value ptr)
{
  s48_value result;
  S48_DECLARE_GC_PROTECT (2);

  S48_GC_PROTECT_2 (ptr, result);
  result = s48_ffi_free (ptr);
  S48_GC_UNPROTECT ();
  
  return result;
}

static s48_value
ffi_is_ptr (s48_value ptr)
{
  return S48_ENTER_BOOLEAN (s48_ffi_is_ptr);
}

static s48_value
ffi_compiler_sizeof (s48_value thing)
{
  s48_value result; 
  S48_DECLARE_GC_PROTECT (2);
 
  S48_GC_PROTECT_2 (thing, result);
  result = s48_ffi_compiler_sizeof (thing);
  S48_GC_UNPROTECT ();
  
  return result;
}

static s48_value
ffi_ptr_ref (s48_value pointer, s48_value type)
{
  s48_value result; 
  S48_DECLARE_GC_PROTECT (3);
 
  S48_GC_PROTECT_3 (pointer, type, result);
  result = s48_ffi_ptr_ref (pointer, type);
  S48_GC_UNPROTECT ();
  
  return result;
}

static s48_value
ffi_ptr_set (s48_value pointer, s48_value type, s48_value value)
{
  S48_DECLARE_GC_PROTECT (3);
 
  S48_GC_PROTECT_3 (pointer, type, value);
  s48_ffi_ptr_set (pointer, type, value);
  S48_GC_UNPROTECT ();
  
  return S48_UNSPECIFIC;
}

static s48_value
ffi_make_cif (s48_value itypes, s48_value otype)
{
  s48_value result;
  S48_DECLARE_GC_PROTECT (3);
  
  S48_GC_PROTECT_3 (itypes, otype, result);
  result = s48_make_ffi_cif (itypes, otype);
  S48_GC_UNPROTECT ();
  
  return result;
}

static s48_value
ffi_primitive_call (s48_value obj, s48_value cif, s48_value args)
{
  s48_value result;
  S48_DECLARE_GC_PROTECT (4);
  
  S48_GC_PROTECT_4 (obj, cif, args, result);
  result = s48_ffi_call (obj, cif, args);
  S48_GC_UNPROTECT ();

  return result;
}

static s48_value
ffi_cpointer_to_string (s48_value cptr)
{
  s48_value result;
  S48_DECLARE_GC_PROTECT (2);
  
  S48_GC_PROTECT_2 (cptr, result);
  result = s48_ffi_cpointer_to_string (cptr);
  S48_GC_UNPROTECT ();

  return result;
}


#define REGISTER_TYPE(NAME)                  \
{                                            \
  s48_value x = S48_MAKE_VALUE(void *);      \
  S48_SET_VALUE (x, void *, &type_ ## NAME); \
  s48_define_exported_binding ("ffi_type_" # NAME, x); \
}

void
s48_init_ffi (void)
{
  S48_GC_PROTECT_GLOBAL (int_sym);
  S48_GC_PROTECT_GLOBAL (char_sym);
  S48_GC_PROTECT_GLOBAL (void_sym);
  S48_GC_PROTECT_GLOBAL (short_sym);
  S48_GC_PROTECT_GLOBAL (long_sym);
  S48_GC_PROTECT_GLOBAL (float_sym);
  S48_GC_PROTECT_GLOBAL (double_sym);
  S48_GC_PROTECT_GLOBAL (star_sym);
  S48_GC_PROTECT_GLOBAL (string_to_symbol_binding);
  
  S48_EXPORT_FUNCTION (ffi_make_lib);
  S48_EXPORT_FUNCTION (ffi_make_obj);
  S48_EXPORT_FUNCTION (ffi_compiler_sizeof);
  S48_EXPORT_FUNCTION (ffi_is_ptr);
  S48_EXPORT_FUNCTION (ffi_ptr_ref);
  S48_EXPORT_FUNCTION (ffi_ptr_set);
  S48_EXPORT_FUNCTION (ffi_malloc);
  S48_EXPORT_FUNCTION (ffi_free);
  S48_EXPORT_FUNCTION (ffi_make_cif);
  S48_EXPORT_FUNCTION (ffi_primitive_call);
  S48_EXPORT_FUNCTION (ffi_cpointer_to_string);
  
  REGISTER_TYPE (void);
  REGISTER_TYPE (bool);
  REGISTER_TYPE (int8);
  REGISTER_TYPE (uint8);
  REGISTER_TYPE (int16);
  REGISTER_TYPE (uint16);
  REGISTER_TYPE (int32);
  REGISTER_TYPE (uint32);
  REGISTER_TYPE (int64);
  REGISTER_TYPE (uint64);
  REGISTER_TYPE (float);
  REGISTER_TYPE (double);
  REGISTER_TYPE (pointer);
  REGISTER_TYPE (fpointer);
}
