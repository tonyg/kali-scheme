/* Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "scheme48vm.h"
#include "scheme48heap.h"

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

extern void	s48_sysdep_init(void);
extern void	s48_initialize_external_modules(void);

char *s48_object_file;   /* specified via a command line argument */
char *s48_reloc_file;    /* dynamic loading will set this */

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
  long required_heap_size;
  int warn_undefined_imported_bindings_p = 1;

#if defined(STATIC_AREAS)
  extern long static_entry;
  extern long static_symbol_table;
  extern long static_imported_binding_table, static_exported_binding_table;
  extern long p_count, *p_areas[], p_sizes[];
  extern long i_count, *i_areas[], i_sizes[];
#endif

  long vm_argc = 0;
  char *me = *argv;		/* Save program name. */

  s48_object_file = s48_reloc_file = NULL;

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
        s48_object_file = *argv;
	break;
      case 'u':
        argc--; argv++;
	warn_undefined_imported_bindings_p = 0;
	break;
      default:
	fprintf(stderr, "Invalid argument: %s\n", *argv);
	errors++;
      }
    else
      if (argv[0][0] != '\0') {
	fprintf(stderr, "Invalid argument: %s\n", *argv);
	errors++; }
  if (errors != 0) {
    fprintf(stderr,
"Usage: %s [options] [-a arguments]\n\
Options: -h <total heap size in words>\n\
	 -s <stack buffer size in words>\n\
         -i <image file name>\n\
         -o <object file name>\n\
         -u [don't warn on unbound external identifiers]",
	    me);
    return 1;
  }

  s48_sysdep_init();
  s48_heap_init();
  s48_init();

  if (image_name == NULL)
    required_heap_size = 0;
  else {
    /* check_image_header returns number of bytes; required_heap_size
       is number of cells. */
    required_heap_size =
      s48_check_image_header((unsigned char *)image_name) >> 2;
    if (-1 == required_heap_size) {
      fprintf(stderr, "Image file \"%s\" is unusable.\n", image_name);
      return 1; }
  }

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

  s48_initialize_heap((long)heap, heap_size);

#if defined(STATIC_AREAS)
  if (image_name == NULL) {
    s48_register_static_areas(p_count, p_areas, p_sizes,
			      i_count, i_areas, i_sizes);
    s48_set_image_valuesB(static_entry,
			  static_symbol_table,
			  static_imported_binding_table,
			  static_exported_binding_table);
  } else if (s48_read_image() == -1) {
    fprintf(stderr, "Image file \"%s\" is unusable.\n", image_name);
    return 1; }
#else
  if (s48_read_image() == -1) {
    fprintf(stderr, "Image file \"%s\" is unusable.\n", image_name);
    return 1; }
#endif

  s48_initialize_vm(stack, stack_size);

  s48_initialize_external_modules();
  
  if (warn_undefined_imported_bindings_p)
    s48_warn_about_undefined_imported_bindings();
  
  return_value = s48_call_startup_procedure(argv, vm_argc);

  if (s48_reloc_file != NULL)
    if (0 != unlink(s48_reloc_file))
      fprintf(stderr, "unable to delete file %s\n", s48_reloc_file);

  return(return_value);
}
