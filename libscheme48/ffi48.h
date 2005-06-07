/*  
  Copyright (c) 2005, Andreas Rottmann

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
#if !defined(_INC_FFI48_H)
#define _INC_FFI48_H

#include "scheme48.h"

/* automatically generated __STDC__ prototypes */
extern s48_value s48_make_ffi_lib (s48_value name,
                                   s48_value no_error_p);
extern s48_value s48_c_make_ffi_lib (const char *name, int no_error_p);
extern s48_value s48_make_ffi_obj (s48_value  lib,
                                   s48_value  name);
extern s48_value s48_c_make_ffi_obj (s48_value  lib,
                                     const char * name);
extern s48_value s48_make_ffi_cif (s48_value  itypes,
                                   s48_value  otype);
extern s48_value s48_ffi_call (s48_value  obj,
                               s48_value  cif,
                               s48_value  args);
extern s48_value s48_ffi_ptr_ref (s48_value  pointer,
                                  s48_value  type);
extern void * s48_ffi_ptr_mem (s48_value ptr);
extern s48_value s48_ffi_malloc (s48_value type, s48_value count, s48_value cptr, s48_value raw_p);
extern s48_value s48_ffi_free (s48_value mem);
extern void s48_init_ffi (void);
#endif
