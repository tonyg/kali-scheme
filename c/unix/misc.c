/* Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdio.h>
#include <stdlib.h>		/* for getenv(), etc. (POSIX?/ANSI) */
#include <string.h>		/* for strncpy(), etc. (POSIX/ANSI) */
#include <pwd.h>		/* for getpwnam() (POSIX.1) */
#include <unistd.h>		/* for sysconf(), etc.  (POSIX.1/.2)*/
#include <errno.h>
#include <sys/stat.h>
#include <locale.h>		/* ISO C99 */
#include <langinfo.h>		/* SUSv2 */
#include "sysdep.h"
#include "c-mods.h"


/*
   Expanding Unix filenames
   Unix Sucks
   Richard Kelsey Wed Jan 17 21:40:26 EST 1990
   Later modified by others who wish to remain anonymous

   Expands initial ~ and ~/ in string `name', leaving the result in `buffer'.
   `buffer_len' is the length of `buffer'.

   Note: strncpy(x, y, n) copies from y to x.
*/

char *s48_expand_file_name (char *name, char *buffer, int buffer_len)
{
#define USER_NAME_SIZE 256
  char *dir, *p, user_name[USER_NAME_SIZE];
  struct passwd *user_data;
  int dir_len, i;
  int name_len = strlen(name);

  dir = 0;

  if (name[0] == '~') {
    name++; name_len--;

    if (name[0] == '/' || name[0] == 0) {
      dir = getenv("HOME"); }

    else {
      for (i = 0, p = name; i < name_len && *p != '/'; i++, p++)
	if (i > (USER_NAME_SIZE - 2)) {
	  fprintf(stderr,
		  "\ns48_expand_file_name: user name longer than %d characters\n",
		  USER_NAME_SIZE - 3);
	  return(NULL); };
      strncpy(user_name, name, i);
      user_name[i] = 0;
      user_data = getpwnam(user_name);
      if (!user_data) {
	fprintf(stderr, "\ns48_expand_file_name: unknown user \"%s\"\n",
		user_name);
	return(NULL); };
      name_len -= i;
      name = p;
      dir = user_data->pw_dir; } }

  else if (name[0] == '$') {
    name++; name_len--;

    for (i = 0, p = name; i < name_len && *p != '/'; i++, p++)
      if (i > (USER_NAME_SIZE - 2)) {
	fprintf(stderr,
		"\ns48_expand_file_name: environment variable longer than %d characters\n",
		USER_NAME_SIZE - 3);
	return(NULL); };
    strncpy(user_name, name, i);
    user_name[i] = 0;

    name_len -= i;
    name = p;
    dir = getenv(user_name); }

  if (dir) {
    dir_len = strlen(dir);
    if ((name_len + dir_len + 1) > buffer_len) {
      fprintf(stderr, "\ns48_expand_file_name: supplied buffer is too small\n");
      return(NULL); };
    strncpy(buffer, dir, dir_len);
    strncpy(buffer + dir_len, name, name_len);
    buffer[name_len + dir_len] = 0; }

  else {
    if ((name_len + 1) > buffer_len) {
      fprintf(stderr, "\ns48_expand_file_name: supplied buffer is too small\n");
      return(NULL); };
    strncpy(buffer, name, name_len);
    buffer[name_len] = 0; }

  return(buffer);
}

/* test routine
main(argc, argv)
  int argc;
  char *argv[];
{
  char buffer[32];
  s48_expand_file_name(argv[1], buffer, 32);
  printf("%s\n", buffer);
  return(0);
}
*/


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
  return((unsigned char *)strerror(the_errno));
}

/* Getting the length of a file. */

long
s48_get_file_size(unsigned char *name)
{
  struct stat file_data;
  int status;
  
  if (-1 == stat((char*)name, &file_data) ||
      ! S_ISREG(file_data.st_mode))
    return -1;
  else
    return file_data.st_size;
}

/* encoding of argv */

unsigned char*
s48_get_os_string_encoding(void)
{
  static char setlocale_called = PSFALSE;
  char *codeset;
  static char* encoding = NULL;

  /* Mike has no clue what the rationale for needing this is. */
  if (!setlocale_called)
    {
      setlocale(LC_CTYPE, "");
      setlocale_called = PSTRUE;
    }

  if (encoding == NULL)
    {
      codeset = nl_langinfo(CODESET); /* this ain't reentrant */
      encoding = malloc(strlen(codeset) + 1);
      if (encoding == NULL)
	return NULL;
      strcpy(encoding, codeset);
    }

  return encoding;
}
