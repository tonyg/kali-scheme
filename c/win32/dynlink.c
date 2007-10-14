/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Load DLLs on Windows.
 */

#include <windows.h>
#include "scheme48.h"
#include "io.h"

extern int s48_utf_8of16_to_utf_16(const unsigned char* utf_8of16,
				   LPWSTR utf_16,
				   int* errorp);

static s48_value
shared_object_dlopen(s48_value name, s48_value complete_name_p)
{
  S48_DECLARE_GC_PROTECT(1);
  HINSTANCE handle;
  s48_value res;
  s48_value full_name;
  WCHAR* name_utf16;
  size_t len = strlen(s48_extract_byte_vector(name));

  S48_GC_PROTECT_1(name);

  if (!S48_EQ(S48_FALSE, complete_name_p))
    {
      full_name = s48_make_byte_vector(len + 5);
      memcpy(s48_extract_byte_vector(full_name),
	     s48_extract_byte_vector(name),
	     len);
      memcpy(s48_extract_byte_vector(full_name) + len,
	     ".dll",
	     5);
      len += 4;
    }
  else
      full_name = name;

  name_utf16 = malloc(sizeof(WCHAR) * (len + 1));
  if (name_utf16 == NULL)
    s48_out_of_memory_error();
  s48_utf_8of16_to_utf_16(s48_extract_byte_vector(full_name), name_utf16, NULL);

  handle = LoadLibraryW(name_utf16);

  free(name_utf16);
  if (handle == NULL)
    s48_os_error("shared_object_dlopen", GetLastError(), 1, name);

  res = S48_MAKE_VALUE(HINSTANCE);
  S48_SET_VALUE(res, HINSTANCE, handle);

  S48_GC_UNPROTECT();

  return res;
}

static s48_value
shared_object_dlsym(s48_value handle, s48_value name)
{
  void *entry;
  HINSTANCE native_handle;
  char *native_name;
  
  native_handle = S48_EXTRACT_VALUE(handle, HINSTANCE);
  native_name = s48_extract_byte_vector(name);

  entry = GetProcAddress(native_handle, native_name);

  if (entry == NULL)
    s48_os_error("shared_object_dlsym", GetLastError(), 2,
		 handle, name);

  return s48_enter_pointer(entry);
}

static s48_value
shared_object_dlclose(s48_value handle)
{
  HINSTANCE native_handle = S48_EXTRACT_VALUE(handle, HINSTANCE);
  
  if (!FreeLibrary(native_handle) < 0)
    s48_os_error("shared_object_dlclose", GetLastError(), 1, handle);
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
