
/* Dynamic loading.  Copyright unclear. */

#include <stdio.h>

#if !defined(__hpux) && !defined(NeXT)

#include <a.out.h>
#include <sys/types.h>      /* sbrk   */
#include <strings.h>        /* strlen */

#ifdef mips
struct exec {               /* an exec like structure for the MIPS */
  struct filehdr ex_f;
  struct aouthdr ex_o;
};
#endif

extern char *object_file;   /* specified via a command line argument */
extern char *reloc_file;

extern char *get_reloc_file();

/* declarations so that I can put the functions in a reasonable order */
int really_dynamic_load( char *, char *, char * );
void abort_load( char *, FILE *, char *);

int
dynamic_load( char *user_args )
{
  char *old_reloc_file, *new_reloc_file;

  old_reloc_file = get_reloc_file();

  if (old_reloc_file == NULL) {
    fprintf(stderr, "aborting dynamic load\n");
    return(0);
  }
  
  new_reloc_file = (char *) malloc(L_tmpnam);
  
  tmpnam(new_reloc_file);

  if (-1 == really_dynamic_load(old_reloc_file, new_reloc_file,
				user_args)) {
    free(new_reloc_file);
    return(-1);
  }

  reloc_file = new_reloc_file;

  if (old_reloc_file != object_file)
    if (0 != unlink(old_reloc_file))
      fprintf(stderr, "unable to delete file %s\n", old_reloc_file);
    free(old_reloc_file);

  return(0);
}

/* stub for the above that can be called from Scheme48 */

/* this stuff should be in an automatically generated .h file */
#define S48_FALSE 1L
#define S48_TRUE 5L
#define S48_HEADER_TAG 2
#define S48_STOB_TAG 3
#define S48_STRING_TYPE 15

long
s48_dynamic_load( long nargs, long *argv )
{
  long arg, header;
  char *user_args;

  if (nargs != 1)
    return(S48_FALSE);

  arg = argv[0];
  
  if (arg & 3 != S48_STOB_TAG)
    return(S48_FALSE);

  header = (*((long *)(arg - S48_STOB_TAG - 4)));
  user_args = (char *)(arg - S48_STOB_TAG);

  if (((header & 3) != S48_HEADER_TAG) ||
      (((header >> 2) & 31) != S48_STRING_TYPE))
    return(S48_FALSE);

  if (0 == dynamic_load(user_args))
    return(S48_TRUE);
  else
    return(S48_FALSE);
}

/*

  This executes an `ld' command that links `user_args' into `output_file',
  including relocation information from `reloc_info_file'.  `output_file'
  is then loaded into the current process.

  `output_file' can then be used as `reloc_info_file' for subsequent
  dynamic loads.

  0 is returned if all goes well, -1 if something goes wrong.

  This is just N consecutive system calls with error checking.
  The basic sequence is:
    1) allocate storage for the command string
    2) use sbrk() to align the storage pointer on a page boundary
    3) fill in the command string
    4) execute the command
    5) open the resulting file and read its header
    6) allocate storage for reading in the file
    7) read in the text and data segments
  It is important that the storage pointer does not change between
  steps 2 and 6.  The problem is that ld needs to know the eventual
  starting address of the text, but the amount of text cannot be
  determined until after the ld command.

*/

int
really_dynamic_load( char *reloc_info_file, char *output_file, char *user_args )
{
  int command_length;
  char *command;                /* the ld command */
  int status;

  long load_point, actual_load_point, end_of_memory;

  FILE *output_file_desc;
  char output_file_buffer[BUFSIZ];
  struct exec output_file_header;
  long loaded_size, required_size;

  extern int getpagesize();
  extern char *sbrk(int);

  int page_size = getpagesize();

  char *template = "/bin/ld -N -x -A %s -T %lx -o %s %s -lc";

  /* calculate how long the ld command will be */
  command_length = strlen(template) + strlen(reloc_info_file) +
    strlen(user_args) + 30;  /* additional space for load_point etc. */

  command = (char *) malloc(command_length);

  if (command == NULL) {
    fprintf(stderr, "malloc failed to allocate %d characters\n",
	    command_length);
    abort_load(NULL, NULL, NULL);
    return(-1);
  }

  end_of_memory = (long) sbrk(0);

  if (end_of_memory < 0) {
    fprintf(stderr, "sbrk(0) failed\n");
    abort_load(command, NULL, NULL);
    return(-1);
  }
  
  /* move the storage pointer to the next page boundary */
  end_of_memory = (long) sbrk(page_size - end_of_memory % page_size);

  if (end_of_memory < 0) {
    fprintf(stderr, "sbrk(...) failed\n");
    abort_load(command, NULL, NULL);
    return(-1);
  }

  load_point = (long) sbrk(0);    /* no malloc or printf after this */
  
  if (load_point < 0 || 0 != load_point % page_size) {
    fprintf(stderr, "couldn't align sbrk on page boundary\n");
    abort_load(command, NULL, NULL);
    return(-1);
  }

  /* finish making the load command */
#ifdef sun  /* Sun's sprintf has a nonstandard return value */
  sprintf(command, template, reloc_info_file, load_point, output_file,
	  user_args);
#else
  status = sprintf(command, template, reloc_info_file, load_point, output_file,
		   user_args);

  if (status < 0) {
    fprintf(stderr, "sprintf error %d\n", status);
    abort_load(command, NULL, NULL);
    return(-1);
  }
#endif

  if (strlen(command) >= command_length) {
    fprintf(stderr, "load command overflowed buffer by %d chars\n",
	    strlen(command) - command_length);
    abort_load(command, NULL, NULL);
    return(-1);
  }

  /* run the ld comand */
  if (system(command) != 0 ) {
    fprintf(stderr, "ld command failed\n");
    abort_load(command, NULL, output_file);
    return(-1);
  }

  free(command);

  /* open the output file */
  output_file_desc = fopen(output_file, "r");
  if (output_file_desc == NULL ) {
    fprintf(stderr, "unable to open output file\n");
    abort_load(NULL, NULL, output_file);
    return(-1);
  }

  /* use our own buffer to prevent calls to malloc etc. */
  setbuf(output_file_desc, output_file_buffer);

  /* read the output file header */
  status = fread((char *)&output_file_header, sizeof(struct exec), 1,
		 output_file_desc);
  if (status != 1) {
    fprintf(stderr, "couldn't read output file header\n");
    abort_load(NULL, output_file_desc, output_file);
    return(-1);
  }

#ifdef mips

  if (N_BADMAG(output_file_header.ex_o)) {
    fprintf(stderr, "output file has bad magic number %d\n",
	    output_file_header.ex_o.magic);
    abort_load(NULL, output_file_desc, output_file);
    return(-1);
  }

  loaded_size = output_file_header.ex_o.tsize + output_file_header.ex_o.dsize;
  required_size = loaded_size + output_file_header.ex_o.bsize;

#else  /* not MIPS */

  if (N_BADMAG(output_file_header)) {
    fprintf(stderr, "output file has bad magic number %d\n",
	    output_file_header.a_magic);
    abort_load(NULL, output_file_desc, output_file);
    return(-1);
  }

  loaded_size = output_file_header.a_text + output_file_header.a_data;
  required_size = loaded_size + output_file_header.a_bss;

#endif

  /* get required memory */
  actual_load_point = (long) sbrk(required_size);
  if (actual_load_point < 0L) {
    fprintf(stderr, "sbrk(%ld) failed\n", required_size);
    abort_load(NULL, output_file_desc, output_file);
    return(-1);
  }

  /* make sure nothing went wrong */
  if (actual_load_point != load_point) {
    fprintf(stderr, "storage pointer changed before file could be loaded\n");
    abort_load(NULL, output_file_desc, output_file);
    return(-1);
  }
  
  /* go to beginning of text */
  if (fseek(output_file_desc,
#ifdef mips
	    (long) N_TXTOFF(output_file_header.ex_f, output_file_header.ex_o),
#else
	    (long) N_TXTOFF(output_file_header),
#endif	    
	    0)
      < 0) {
    fprintf(stderr, "unable to seek to beginning of linked file text\n");
    abort_load(NULL, output_file_desc, output_file);
    return(-1);
  }

  /* read the text and data segments */
  if (fread((char *)load_point, 1, loaded_size, output_file_desc)
      !=
      loaded_size) {
    fprintf(stderr, "unable to load linked file\n");
    abort_load(NULL, output_file_desc, output_file);
    return(-1);
  }

  if (0 != fclose(output_file_desc))
    fprintf(stderr, "unable to close file %s\n", output_file);

  return(0);  /* We made it! */
}

/* Print a message and then clean up. */

void
abort_load( char *command, FILE *file_desc, char *filename )
{
  fprintf(stderr, "aborting dynamic load\n");
  if (command != NULL)
    free(command);
  if (file_desc != NULL)
    if (0 != fclose(file_desc))
      fprintf(stderr, "unable to close file %s\n", filename);
  if (filename != NULL)
    if (0 != unlink(filename))
      fprintf(stderr, "unable to delete file %s\n", filename);
}

#else
int
dynamic_load( char *user_args )
{
  fprintf(stderr, "Dynamic .o loading not implemented under HP-UX\n");
  return -1;
}

#endif /* not hpux and not next */

