
/* Dynamic loading.  Copyright unclear. */

#include <stdio.h>
#include "scheme48.h"

/* 
   Hey folks, please help us out with conditions under which the
   ANCIENT_DYNLOAD and/or HAS_DLOPEN code might work.  About all we
   know is that ANCIENT_DYNLOAD worked once upon a time on the DEC
   MIPS (how to conditionalize for that I don't know -- #ifdef ultrix
   perhaps?), is very similar to something that worked under BSD 4.2,
   and doesn't work under HPUX, NeXT, or SGI.
 */
#if defined(ultrix)
#define ANCIENT_DYNLOAD
#endif

#if defined(svr4) || defined(SVR4) || defined (__svr4__) || defined(sun) 
#if !defined(MACH)
#define HAS_DLOPEN
#endif /*not MACH*/
#endif /*svr4*/


#if defined(HAS_DLOPEN)

/*-------------------------------------------------------------------
   Following is the preferred modern method, supposedly.  dlopen() is
   some kind of newfangled SYSV thing.  This code was contributed by
   Basile Starynkevitch <basile@soleil.serma.cea.fr> in October 1993 --
   thanks!
 */

#if defined(svr4) || defined(SVR4) || defined(__svr4__)
#define HAS_LIBGEN
#endif

#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#ifdef HAS_LIBGEN
#include <libgen.h>
/* if we have pathfind, get the file name with $LD_LIBRARY_PATH or $S48_EXTERN_PATH */
static char *shared_object_name(char *name)
{
  char *res=0;
  res = pathfind(getenv("LD_LIBRARY_PATH"), name, "r");
  if (res) 
    return res;
  res = pathfind(getenv("S48_EXTERN_PATH"), name, "r");
  if (res) 
    return res;
  return name;
} /* end of shared_object_name */
#define SHARED_OBJECT_NAME(Name) shared_object_name(Name)
#else /* no HAS_LIBGEN */
#define SHARED_OBJECT_NAME(Name) (Name)
#endif /*HAS_LIBGEN*/


#ifndef MAXNB_DLOPEN
#define MAXNB_DLOPEN 40
#endif MAXNB_DLOPEN

#ifndef S48_DLOPEN_MODE
/* SunoS5 & SVR4 define RTLD_NOW */
#ifdef RTLD_NOW
#define S48_DLOPEN_MODE RTLD_NOW
#else 
/* SunOS4 just says that mode should be 1 ie RTLD_LAZY */
#define S48_DLOPEN_MODE 1
#endif /*RTLD_NOW*/
#endif /*!S48_DLOPEN_MODE*/

static void* dlopened_handle[MAXNB_DLOPEN];

/* with dlopen the only user argument is the full shared object name */
int
dynamic_load(char*sharedobjname)
{
  char pathname[520];
  int  rank= -1;
  int cnt;
  void *newhandle;
  pathname[0]='\0';
  strncpy(pathname, SHARED_OBJECT_NAME(sharedobjname), sizeof(pathname)-1);
  pathname[sizeof(pathname)-1]='\0';
  /* find an unused slot */
  for (cnt=0; cnt<MAXNB_DLOPEN; cnt++)
    if (!dlopened_handle[cnt]) {
      rank=cnt;
      break;
    }
  if (rank<0) {
    fprintf(stderr,
	    " dynamic_load - table of dlopened handles is full (MAXNB_DLOPEN=%d) ",
	    MAXNB_DLOPEN);
    return -1;
  };
  newhandle=dlopen(pathname, S48_DLOPEN_MODE);
  if (!newhandle) {
    fprintf(stderr, " dynamic_load of %s can't dlopen %s: %s ",
	    sharedobjname, pathname, dlerror());
    return -1;
  };
  dlopened_handle[rank] = newhandle;
#ifdef DLDEBUG
  printf(" %s:%d %s sharedobjname='%s' pathname='%s' handle %#x rank=%d \n",
	 __FILE__, __LINE__, __FUNCTION__, 
	 sharedobjname, pathname, newhandle, rank);
#endif /*DLDEBUG*/
  return 0;
} /*------end of dlopening dynamic load -----*/

/* this function implements lookup_external_name with dlsym */
long
lookup_dlsym(char *name, long *location)
{
  void *adr;
  static void *selfhandle;
  int rank;
/* perhaps i should scan the dlopened_handle from last to first,
   to find newest incarnation of symbol? in practice it should be faster
   to go from first (oldest) to last,
   and i hope that every external symbol is defined exactly once most of
   the time!*/
  for (rank=0; rank<MAXNB_DLOPEN; rank++)
    if (dlopened_handle[rank] && (adr=dlsym(dlopened_handle[rank], name))) {
      *location = (long) adr;
#ifdef DLDEBUG
  printf(" %s:%d %s name='%s' in rank=%d at adr=%#x\n",
	 __FILE__, __LINE__, __FUNCTION__, 
	 name, rank, (long) adr);
#endif /*DLDEBUG*/
      return 1;
    };
  /* find the name in the self process image - ie original scheme48
     executable file */
  if (!selfhandle) 
    selfhandle=dlopen((char*)0, S48_DLOPEN_MODE);
  if (adr=dlsym(selfhandle, name)) {
    *location = (long) adr;
#ifdef DLDEBUG
  printf(" %s:%d %s name='%s' in self at adr=%#x\n",
	 __FILE__, __LINE__, __FUNCTION__, 
	 name, (long) adr);
#endif /*DLDEBUG*/
    return 1;
  };
  /* can't find name so return 0 */
#ifdef DLDEBUG
  printf(" %s:%d %s name='%s' not found\n",
	 __FILE__, __LINE__, __FUNCTION__, 
	 name);
#endif /*DLDEBUG*/
  return 0;
} /*----end of lookup_dlsym ---*/

#elif defined(ANCIENT_DYNLOAD)

/*-------------------------------------------------------------------
   This is the ancient Berkeley method for dynamic loading, using the
   disgusting -A flag to "ld".
 */

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

/*

  really_dynamic_load() executes an `ld' command that links
  `user_args' into `output_file', including relocation information
  from `reloc_info_file'.  `output_file' is then loaded into the
  current process.

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


#else /* !ANCIENT_DYNLOAD */

/*-------------------------------------------------------------------
   Dummy definition in case we don't have a clue.
 */

int
dynamic_load( char *user_args )
{
  fprintf(stderr, "Dynamic .o loading not implemented\n");
  return -1;
}

#endif


/* Stub for dynamic_load() that can be called from Scheme using
   external call interface */

scheme_value
s48_dynamic_load( long nargs, scheme_value *argv )
{
  scheme_value arg;

  if (nargs != 1) return(SCHFALSE);

  arg = argv[0];
  
  if (!stringp(arg)) return(SCHFALSE);

  if (0 == dynamic_load(&string_ref(arg, 0)))
    return(SCHTRUE);
  else
    return(SCHFALSE);
}
