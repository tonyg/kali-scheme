/* Copyright (c) 2005 by Andreas Rottmann
   See file COPYING. */

/* Implementation of the scheme48 executable. */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "scheme48/libscheme48-posix/sysdep.h"
#include "scheme48/libscheme48-posix/sysexits.h"
#include "xalloc.h"

static void usage_error(void);
static void show_help(void);
static char *astrcat(char **strp, const char *str1);
static int asprintf_append(char **strp, const char *fmt, ...);

int
main(int argc, char *argv[])
{
  char **new_argv;
  char *p, *language_tag;
  char *exec_commands;
  int i, pos, offset;
  int a_option = 0;
  char *h_option = NULL, *i_option = NULL;
  
  exec_commands = xmalloc(1);
  exec_commands[0] = '\0';
  
  for (pos = 1; pos < argc; pos++)
    {
      if (!strcmp(argv[pos], "-l"))
        {
          if (pos >= argc - 1)
            usage_error();
          else
            asprintf_append(&exec_commands, "(load \"%s\")", argv[++pos]);
        }
      if (!strcmp(argv[pos], "-cl"))
        {
          if (pos >= argc - 1)
            usage_error();
          else
            asprintf_append(&exec_commands, "(config '(load \"%s\"))", argv[++pos]);
        }
      else if (!strcmp(argv[pos], "-o"))
        {
          if (pos >= argc - 1)
            usage_error();
          else
            asprintf_append(&exec_commands, "(user '(open %s))", argv[++pos]);
        }
      else if (!strcmp(argv[pos], "-e"))
        {
          if (pos >= argc - 1)
            usage_error();
          else
            astrcat(&exec_commands, argv[++pos]);
        }
      else if (!strcmp(argv[pos], "-r"))
        {
          if (pos >= argc - 1)
            usage_error();
          else
            asprintf_append(&exec_commands, "(user '(run %s #f))", argv[++pos]);
        }
      else if (!strcmp(argv[pos], "-a"))
        {
          a_option = 1;
          ++pos;
          break;
        }
      else if (!strcmp(argv[pos], "-h"))
        {
          if (pos >= argc - 1)
            usage_error();
          else
            h_option = argv[++pos];
        }
      else if (!strcmp(argv[pos], "-i"))
        {
          if (pos >= argc - 1)
            usage_error();
          else
            i_option = argv[++pos];
        }
      else if (!strcmp(argv[pos], "--"))
        {
          break;
        }
      else if (!strcmp(argv[pos], "--help"))
        {
          show_help();
        }
      else
        break;
    }
  
  new_argv = (char **)xmalloc((argc - pos + 10) * sizeof(char *));

  offset = 0;
  new_argv[offset++] = VM;
  new_argv[offset++] = "-i";
  new_argv[offset++] = IMAGE;
  if (h_option)
    {
      new_argv[offset++] = "-h";
      new_argv[offset++] = h_option;
    }
  new_argv[offset++] = "-a";
  if (!a_option)
    {
      new_argv[offset++] = "run-script";
      new_argv[offset++] = "exec";
      new_argv[offset++] = exec_commands;
    }
  
  for (i = offset; i < argc + offset - pos; ++i)
    new_argv[i] = argv[i - offset + pos];

  new_argv[i] = NULL;

  execv(VM, new_argv);

  fprintf(stderr,
	  "scheme48: exec'ing %s failed (%s)\n",
	  VM, strerror(errno));
  return EX_SOFTWARE;
}

static void usage_error(void)
{
  fprintf(stderr, "usage: scheme48 [OPTIONS]... [SCRIPT] [ARG]...\n");
  exit(EX_USAGE);
}

static void show_help(void)
{
  printf("Usage: scheme48 OPTION... [SCRIPT] ARG...\n");
  printf("Evaluate Scheme code, interactivly or from a script");
  printf("\n");
  printf("  -a ARG...       pass ARG... as resumer arguments\n");
  printf("  --              stop scanning arguments; run interactivly\n");
  
  exit(0);
}

static char *astrcat(char **strp, const char *str1)
{
  size_t old_len = 0, total_len;

  if (*strp)
    old_len = strlen(*strp);
  total_len = old_len + strlen(str1) + 1;

  *strp = realloc(*strp, total_len);
  strcpy(*strp + old_len, str1);

  return *strp;
}
 
static int asprintf_append(char **strp, const char *fmt, ...)
{
  char *buffer = NULL;
  int result;
  va_list args;

  va_start(args, fmt);
  result = vasprintf(&buffer, fmt, args);
  va_end(args);

  astrcat(strp, buffer);
  free(buffer);
  
  return result;
}

/* arch-tag: 7c89c1b2-83e4-400b-961d-d4f649f2ab8e
   (do not change this comment) */
