/* Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.
   See file COPYING.*/

#include <stdio.h>
#include <sys/signal.h>
#include <string.h>

#if !defined DEFAULT_HEAP_SIZE
/* 1 megacell = 4 megabytes (2 meg per semispace) */
#define DEFAULT_HEAP_SIZE 1000000 
#endif

/* DEFAULT_IMAGE_NAME should be defined using the -D switch to cc. */
#if !defined DEFAULT_IMAGE_NAME
#define DEFAULT_IMAGE_NAME "s48.image"
#endif

char *object_file;   /* specified via a command line argument */
char *reloc_file;    /* dynamic loading will set this */

static void when_interrupt(sig, code, scp)
     int sig, code; 
     struct sigcontext *scp;
{
  extern long Spending_interruptsS;
  Spending_interruptsS |= 2;          /* 2 = 2 ** interrupt/keyboard */
  signal(SIGINT, when_interrupt);
  return;
}

main(argc, argv)
     int argc; char **argv;
{
  char *image_name = DEFAULT_IMAGE_NAME;
  long heap_size = DEFAULT_HEAP_SIZE; /* in numbers of cells. */
  int errors = 0;
  long return_value;
  extern long resume();
  long vm_argc = 0;
  char *me = *argv;		/* Save program name. */

  object_file = reloc_file = NULL;

  argv++; argc--;		/* Skip program name. */

  for (; argc > 0; argc--, argv++)
    if (argv[0][0] == '-')
      switch (argv[0][1]) {
      case 'h':
	argc--; argv++;
	if (argc == 0) { errors++; break; }
	heap_size = atoi(*argv) /*  / sizeof(long)  ***JAR FIX*** */;
	if (heap_size <= 0) errors++;
	break;
      case 'i':
	argc--; argv++;
	if (argc == 0) { errors++; break; }
	image_name = *argv;
	break;
      case 'a':
	argc--; argv++;
	vm_argc = argc;    /* remaining args are passed to the VM */
	argc = 0;
	break;
      case 'o':
        argc--; argv++;
	if (argc == 0) { errors++; break; }
        object_file = *argv;
	break;
      default:
	fprintf(stderr, "Invalid argument: %s\n", *argv);
	errors++;
      }
    else
      if (argv[0][0] != '\0') 
	{ fprintf(stderr, "Invalid argument: %s\n", *argv);
	  errors++; }
  if (errors != 0) {
    fprintf(stderr,
"Usage: %s [options]\n\
Options: -h <total heap size in words>\n\
         -i <image file name>\n\
         -o <object file name>\n\
         -a <startup arguments (must come last)>\n",
	    me);
    exit(1);
  }

  scheme48_init();
  signal(SIGINT, when_interrupt);
  signal(SIGPIPE, SIG_IGN);

  return_value = resume(image_name, argv, vm_argc, heap_size << 2, 10000L);

  if (reloc_file != NULL)
    if (0 != unlink(reloc_file))
      fprintf(stderr, "unable to delete file %s\n", reloc_file);

  return(return_value);
}

