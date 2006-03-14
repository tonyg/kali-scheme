/* Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/* Implementation of SRFI-22-style script interpreters. */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "sysdep.h"

int
main(int argc, char *argv[])
{
  char **new_argv;
  char *p, *language_tag;
  int i;

  if (argc < 2)
    {
      fprintf(stderr, "usage: %s script arg ...\n", argv[0]);
      return EX_USAGE;
    }

  p = strrchr(argv[0], '/');
  p = p ? (p+1) : argv[0];
  if (strncmp(p, "scheme-", 7))
    {
      fprintf(stderr,
	      "%s: script interpreter name must start with `scheme-'\n",
	      p);
      return EX_USAGE;
    }
  language_tag = p + 7;

  new_argv = (char **)malloc((argc + 4) * sizeof(char *));
  if (!new_argv)
    {
      fprintf(stderr, "%s: out of memory\n", argv[0]);
      return EX_SOFTWARE;
    }

  new_argv[0] = RUNNABLE;
  new_argv[1] = "-a";
  new_argv[2] = "run-script";
  new_argv[3] = language_tag;

  for (i = 4; i < argc + 4; ++i)
    new_argv[i] = argv[i-3];

  new_argv[argc + 3] = NULL;

  execv(RUNNABLE, new_argv);

  fprintf(stderr,
	  "%s: exec'ing %s failed (%s)\n",
	  argv[0], RUNNABLE, strerror(errno));
  return EX_SOFTWARE;
}
