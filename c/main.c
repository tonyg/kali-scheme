/* Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#if !defined(DEFAULT_HEAP_SIZE)
/* 1.5 megacell = 6 megabytes (3 meg per semispace) */
#define DEFAULT_HEAP_SIZE 1500000L
#endif

#if !defined(DEFAULT_STACK_SIZE)
/* 2500 cells = 10000 bytes */
#define DEFAULT_STACK_SIZE 2500L
#endif

#if defined(STATIC_AREAS)
#define DEFAULT_IMAGE_NAME NULL
#else

/* DEFAULT_IMAGE_NAME should be defined using the -D switch to cc. */
#if !defined(DEFAULT_IMAGE_NAME)
#define DEFAULT_IMAGE_NAME "scheme48.image"
#endif

#endif /* STATIC_AREAS */


char *object_file;   /* specified via a command line argument */
char *reloc_file;    /* dynamic loading will set this */


extern void	scheme48_init(void);
extern void	sysdep_init(void);
extern void	s48_heap_init(void);
extern long	required_init_space(unsigned char **, long);
extern void	initialize_heap(long, long);
extern void	initialize_vm(long, long);
extern long	call_startup_procedure(long, char **, long);
extern long	check_image_header(unsigned char *);
extern long	read_image(long);
extern void	register_static_areas(unsigned char, long *, long *, unsigned char, long *, long *);

int
main(argc, argv)
     int argc; char **argv;
{
  char *image_name = DEFAULT_IMAGE_NAME;
  long heap_size = DEFAULT_HEAP_SIZE;    /* in numbers of cells */
  long stack_size = DEFAULT_STACK_SIZE;  /* in numbers of cells */
  int errors = 0;
  long return_value;
  void *heap, *stack;
  long required_heap_size, startup_proc;

#if defined(STATIC_AREAS)
  extern long entry;
  extern long p_count, *p_areas[], p_sizes[];
  extern long i_count, *i_areas[], i_sizes[];
#endif

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
	heap_size = atoi(*argv);
	if (heap_size <= 0) errors++;
	break;
      case 's':
	argc--; argv++;
	if (argc == 0) { errors++; break; }
	stack_size = atoi(*argv);
	if (stack_size <= 0) errors++;
	break;
      case 'i':
	argc--; argv++;
	if (argc == 0) { errors++; break; }
	image_name = *argv;
	break;
      case 'a':
	argc--;
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
"Usage: %s [options] [-a arguments]\n\
Options: -h <total heap size in words>\n\
	 -s <stack buffer size in words>\n\
         -i <image file name>\n\
         -o <object file name>\n",
	    me);
    return 1;
  }

  sysdep_init();
  s48_heap_init();
  scheme48_init();

  if (image_name == NULL)
    required_heap_size = 0;
  else {
    /* check_image_header returns number of bytes; required_heap_size
       is number of cells. */
    required_heap_size = check_image_header((unsigned char *)image_name) >> 2;
    if (-1 == required_heap_size) {
      fprintf(stderr, "Image file \"%s\" is unusable.\n", image_name);
      return 1; }
  }

  required_heap_size += required_init_space((unsigned char **)argv, vm_argc);
  
  /* two semi-spaces, plus we want some room to maneuver */
  if (heap_size < 4 * required_heap_size) {
    fprintf(stderr, "heap size %ld is too small, using %ld\n",
	    heap_size, 4 * required_heap_size);
    heap_size = 4 * required_heap_size; }

  heap = (void *) malloc(heap_size * sizeof(long));
  stack = (void *) malloc(stack_size * sizeof(long));
    
  if (!heap || !stack) {
    fprintf(stderr, "system is out of memory\n");
    return 1; }

  initialize_heap((long)heap, heap_size);
  initialize_vm((long)stack, stack_size);

#if defined(STATIC_AREAS)
  if (image_name == NULL) {
    register_static_areas(p_count, p_areas, p_sizes,
			  i_count, i_areas, i_sizes);
    startup_proc = entry;
  } else
    startup_proc = read_image((long)0);
#else
  startup_proc = read_image((long)0);
#endif

  if (startup_proc == -1) {
    fprintf(stderr, "Image file \"%s\" is unusable.\n", image_name);
    return 1; }

  return_value = call_startup_procedure(startup_proc, argv, vm_argc);

  if (reloc_file != NULL)
    if (0 != unlink(reloc_file))
      fprintf(stderr, "unable to delete file %s\n", reloc_file);

  return(return_value);
}
