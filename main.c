/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/

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

static char *c_string_to_prescheme_string(char *str)
{
  typedef struct ps_string {long length; char contents[1];} ps_string;
  struct ps_string *new;
  
  new = (struct ps_string *)malloc(sizeof(ps_string) + strlen(str));
  if (new == NULL) {
    fprintf(stderr, "malloc out of space, aborting execution\n");
    exit(1);
  }
  new->length = strlen(str);
  strcpy(new->contents, str);
  return new->contents;
}

static char *string_append(char *strings[], int count)
{
  int size, i, j;
  char *result;

  if (count == 0)
    result = "";
  else {
    size = count;     /* room for spaces and the final \0 */
    for (i = 0; i < count; ++i)
      size += strlen(strings[i]);
    result = (char *)malloc(size);
    if (result == NULL) {
      fprintf(stderr, "malloc out of space, aborting execution\n");
      exit(1);
    }
    strcpy(result, strings[0]);
    for (i = 1, j = strlen(strings[0]);
	 i < count;
	 j += 1 + strlen(strings[i]), i++)
      {
	result[j] = ' ';
        strcpy(result + j + 1, strings[i]);
      }
  }
  return result;
}

main(argc, argv)
     int argc; char **argv;
{
  char *image_name = DEFAULT_IMAGE_NAME;
  char *argument = "";
  long heap_size = DEFAULT_HEAP_SIZE; /* in numbers of cells. */
  int errors = 0;
  long return_value;
  extern long resume();
  extern long Sinitial_heap_sizeS;
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
	argument = string_append(argv, argc);
	argc = 0;
	break;
      case 'o':
        argc--; argv++;
	if (argc == 0) { errors++; break; }
        object_file = c_string_to_prescheme_string(*argv);
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

  argument = c_string_to_prescheme_string(argument);

  scheme48_init();
  Sinitial_heap_sizeS = heap_size;
  signal(SIGINT, when_interrupt);

  return_value = resume((long)image_name, (long)argument);

  if (reloc_file != NULL)
    if (0 != unlink(reloc_file))
      fprintf(stderr, "unable to delete file %s\n", reloc_file);

  return(return_value);
}

