/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Dynamically load external modules on machines that support it.
 */

#include "sysdep.h"

#include <stdlib.h>
#include <unistd.h>
#include "scheme48.h"

#if defined(HAVE_DLOPEN)
 #include <dlfcn.h>
#else
#include "fake/dlfcn.h"
#endif

#if	defined(RTLD_NOW)
#define	DLOPEN_MODE	RTLD_NOW
#elif	defined(RTLD_LAZY)
#define	DLOPEN_MODE	(RTLD_LAZY)
#else
#define	DLOPEN_MODE	(1)
#endif

static s48_value
shared_object_dlopen(s48_value name)
{
  void *handle;
  s48_value res;
  
  handle = dlopen(s48_extract_string(name), DLOPEN_MODE);
  if (handle == NULL)
    s48_raise_string_os_error((char *)dlerror());

  res = S48_MAKE_VALUE(void *);
  S48_UNSAFE_EXTRACT_VALUE(res, void *) = handle;

  return res;
}

static s48_value
shared_object_dlsym(s48_value handle, s48_value name)
{
  const char *error;
  void *entry;
  void *native_handle;
  s48_value res;
  char *native_name;
  
  native_handle = S48_EXTRACT_VALUE(handle, void *);

  native_name = s48_extract_string(name);

  entry = dlsym(native_handle, native_name);

  if (entry == NULL)
    s48_raise_string_os_error((char*)dlerror());

  res = S48_MAKE_VALUE(void *);
  S48_UNSAFE_EXTRACT_VALUE(res, void *) = entry;
  return res;
}

static s48_value
shared_object_dlclose(s48_value handle)
{
  void *native_handle = S48_EXTRACT_VALUE(handle, void *);
  
  if (dlclose(native_handle) < 0)
    s48_raise_string_os_error((char*)dlerror());
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
