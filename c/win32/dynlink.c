/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Load DLLs on Windows.
 */

#include <windows.h>
#include "scheme48.h"
#include "io.h"

static s48_value
shared_object_dlopen(s48_value name)
{
  HINSTANCE handle;
  s48_value res;
  
  handle = LoadLibrary(s48_extract_string(name));
  if (handle == NULL)
    s48_raise_string_os_error(ps_error_string(GetLastError()));

  res = S48_MAKE_VALUE(HINSTANCE);
  S48_SET_VALUE(res, HINSTANCE, handle);

  return res;
}

static s48_value
shared_object_dlsym(s48_value handle, s48_value name)
{
  void *entry;
  HINSTANCE native_handle;
  char *native_name;
  
  native_handle = S48_EXTRACT_VALUE(handle, HINSTANCE);
  native_name = s48_extract_string(name);

  entry = GetProcAddress(native_handle, native_name);

  if (entry == NULL)
    s48_raise_string_os_error(ps_error_string(GetLastError()));

  return s48_enter_pointer(entry);
}

static s48_value
shared_object_dlclose(s48_value handle)
{
  HINSTANCE native_handle = S48_EXTRACT_VALUE(handle, HINSTANCE);
  
  if (!FreeLibrary(native_handle) < 0)
    s48_raise_string_os_error(ps_error_string(GetLastError()));
  return S48_UNSPECIFIC;
}

typedef void (*thunk)();

static s48_value
shared_object_call_thunk(s48_value value)
{
  thunk entry;

  entry = S48_EXTRACT_VALUE(value, thunk);
  entry();
  return S48_UNSPECIFIC;
}

void
s48_init_dynlink(void)
{
  S48_EXPORT_FUNCTION(shared_object_dlopen);
  S48_EXPORT_FUNCTION(shared_object_dlsym);
  S48_EXPORT_FUNCTION(shared_object_dlclose);
  S48_EXPORT_FUNCTION(shared_object_call_thunk);
}
