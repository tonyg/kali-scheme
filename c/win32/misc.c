/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.

   See file COPYING. */

#include <stdio.h>
#include <stdlib.h>		/* for getenv(), etc. (POSIX?/ANSI) */
#include <string.h>		/* for strncpy(), etc. (POSIX/ANSI) */
#include <errno.h>

#include <windows.h>

#undef TRUE
#define TRUE  (0 == 0)
#undef FALSE
#define FALSE (0 == 1)

/*
   Expanding Windows filenames
   Windows Sucks.  Unix Sucks.  C Sucks.
   Richard Kelsey Wed Jan 17 21:40:26 EST 1990 (for the Unix version)
   Later rewritten by others who wish to remain anonymous.

   Expands ~/ in string `name', leaving the result in `buffer'.
   `buffer_len' is the length of `buffer'.
*/

char *s48_expand_file_name (char *name, char *buffer, int buffer_len)
{
#define ENV_NAME_SIZE 256
  char *drive = NULL, *path = NULL, *dir = NULL;
  int dir_len;
  int name_len = strlen(name);

  if ((name_len >= 2) && (name[0] == '~') && ((name[1] == '\\' || (name[1] == '/'))))
    {
      drive = getenv("HOMEDRIVE");
      /* HOMEPATH is usually \.
       * I have no idea if it will have trailing \ for a subdirectory.
       */
      path  = getenv("HOMEPATH");
      name += 2;
      name_len -= 2;
    }
  else if ((name_len >= 3) && (name[0] == '%'))
    {
      char *pos = strchr(name + 2, '%');
      if (pos) /* #### uh oh #### */
	{
	  if ((pos - name) + 1 > ENV_NAME_SIZE)
	    {
	      fprintf(stderr,
		      "\ns48_expand_file_name: environment variable longer than %d characters\n",
		      ENV_NAME_SIZE - 2);
	      return NULL;
	    }
	}
      *pos = '\0';
      dir = getenv(name+1);
      name = pos + 1;
      name_len -= (pos - name) + 1;
    }

  if ((drive && path) || dir)
    {
      if (drive && path)
	dir_len = strlen(drive) + strlen(path);
      else
	dir_len = strlen(dir);

      if ((name_len + dir_len + 1) > buffer_len)
	{
	  fprintf(stderr, "\ns48_expand_file_name: supplied buffer is too small\n");
	  return(NULL);
	};
      if (drive && path)
	{
	  int drive_length = strlen(drive);

	  strcpy(buffer, drive);
	  strcpy(buffer + drive_length, path);
	  strcpy(buffer + dir_len, name);
	}
      else
	{
	  strcpy(buffer, dir);
	  strcpy(buffer + dir_len, name);
	}
    }
  else
    {
      if ((name_len + 1) > buffer_len)
	{
	  fprintf(stderr, "\ns48_expand_file_name: supplied buffer is too small\n");
	  return NULL;
	};
      strcpy(buffer, name);
    }

  return(buffer);
}

/* test routine */
/*
main(argc, argv)
  int argc;
  char *argv[];
{
  char buffer[512];
  s48_expand_file_name(argv[1], buffer, 512);
  printf("%s\n", buffer);
  return(0);
}
/* */


/* Driver loop for tail-recursive calls */

long s48_return_value;

long
s48_run_machine(long (*proc) (void))
{
  while (proc != 0)
    proc = (long (*) (void)) (*proc)();
  return s48_return_value;
}

unsigned char *
ps_error_string(long the_errno)
{
  DWORD id = the_errno;
  /* the VM is responsible for copying this */
  static char buf[512];

  for (;;)
    {
      if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, /* lpSource */
			(DWORD) id,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, sizeof(buf)-1,
			NULL)) /* arguments ... */
	return buf;
      else
	/* risky, but we assume some amount of sanity on the side of
	   the Windows implementors---haha */
	id = GetLastError();
    }
}

/* Getting the length of a file. */

long
s48_get_file_size(unsigned char *name)
{
  HANDLE handle = CreateFile(name, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (handle == INVALID_HANDLE_VALUE)
    return -1;
  
  {
    DWORD dwSize = GetFileSize(handle, NULL);
    
    CloseHandle(handle);
    
    if (dwSize == 0xFFFFFFFF)
      return -1;
    else
      return (long) dwSize;
  }
}
