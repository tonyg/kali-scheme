/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include <windows.h>

extern int s48_main (int argc, char ** argv);

extern int s48_utf_16_to_utf_8of16(LPWSTR utf_16, 
				   unsigned char* utf_8of16);

int
wmain(int argc, wchar_t *argv_utf16[])
{
  char** argv = malloc(sizeof(char*) * argc);
  int i = 0;

  if (argv == NULL)
    return -1;

  while (i < argc)
    {
      int size = s48_utf_16_to_utf_8of16(argv_utf16[i], NULL);
      argv[i] = malloc(size + 1);
      if (argv[i] == NULL)
	return -1;
      s48_utf_16_to_utf_8of16(argv_utf16[i], argv[i]);
      ++i;
    }
  return s48_main(argc, argv);
}
