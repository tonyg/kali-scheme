/*Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.*/

/*
   This module is a collection of quick and dirty hacks written mostly
   by people who don't know much about writing portable Unix code and
   don't particularly want to take the time to learn how.  If you have
   concrete suggestions for improvements, they are quite welcome.
   Please send them to scheme-48-bugs@martigny.ai.mit.edu.

   Expanding Unix filenames
   Unix Sucks
   Richard Kelsey Wed Jan 17 21:40:26 EST 1990
   Later modified by others who wish to remain anonymous
   "ps_" stands for "Pre-Scheme"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

#include <time.h>
#include <unistd.h>		/* for sysconf() */
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#ifndef sgi
#include <sys/timeb.h>
#endif

#include <signal.h>
#include <nlist.h>



#if defined(svr4) || defined(SVR4) || defined (__svr4__) || defined(sun) 
#define HAS_DLOPEN
#endif /*svr4*/

/*
From: Jim.Rees@umich.edu
Date: Sun, 26 Dec 93 16:06:08 EST

In unix.c, the gettimeofday code is wrong.  I didn't think sysV had this
call.  You've only used the Berkeley version for sgi and netbsd machines,
but in fact all bsd systems have this call (it was invented at Berkeley).
I suggest the following fix, which will work on both sysV and bsd machines. 
If you (or your compiler) are squeamish about passing too many parameters in
to a system call, you might want to do it differently, but in any case you
should always use gettimeofday on bsd machines.
 */
#define HAS_GETTIMEOFDAY

#define USER_NAME_SIZE 256

/*
   Expands initial ~ and ~/ in string `name', leaving the result in `buffer'.
   `buffer_len' is the length of `buffer'.

   Note: strncpy(x, y, n) copies from y to x.
*/

char *expand_file_name (name, name_len, buffer, buffer_len)
  char *name, *buffer;
  int name_len, buffer_len;
{
  char *dir, *p, user_name[USER_NAME_SIZE];
  struct passwd *user_data;
  int dir_len, i;
  extern char *getenv();

  dir = 0;

  if (name[0] == '~') {
    name++; name_len--;

    if (name[0] == '/' || name[0] == 0) {
      dir = getenv("HOME"); }

    else {
      for (i = 0, p = name; i < name_len && *p != '/'; i++, p++)
	if (i > (USER_NAME_SIZE - 2)) {
	  fprintf(stderr,
		  "\nexpand_file_name: user name longer than %d characters\n",
		  USER_NAME_SIZE - 3);
	  return(NULL); };
      strncpy(user_name, name, i);
      user_name[i] = 0;
      user_data = getpwnam(user_name);
      if (!user_data) {
	fprintf(stderr, "\nexpand_file_name: unknown user \"%s\"\n",
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
		"\nexpand_file_name: environment variable longer than %d characters\n",
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
      fprintf(stderr, "\nexpand_file_name: supplied buffer is too small\n");
      return(NULL); };
    strncpy(buffer, dir, dir_len);
    strncpy(buffer + dir_len, name, name_len);
    buffer[name_len + dir_len] = 0; }

  else {
    if ((name_len + 1) > buffer_len) {
      fprintf(stderr, "\nexpand_file_name: supplied buffer is too small\n");
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

  expand_file_name(argv[1], strlen(argv[1]), buffer, 32);
  printf("%s\n", buffer);
  return(0);
}
*/

#define PS_STRING_LENGTH(s)   (strlen(s))
#define FILENAME_SIZE 256
#define SPEC_SIZE 16

long ps_open(filename, spec)
  char *filename, *spec;

{
  char filename_temp[FILENAME_SIZE], spec_temp[SPEC_SIZE];
  char *expanded;
  int spec_len;

  spec_len = PS_STRING_LENGTH(spec);
  if (spec_len >= SPEC_SIZE) return((long) NULL);

  expanded = expand_file_name(filename,
			      (int) PS_STRING_LENGTH(filename),
			      filename_temp, FILENAME_SIZE);
  if (expanded == NULL) return((long) NULL);
  strncpy(spec_temp, spec, spec_len);
  spec_temp[spec_len] = 0;
  return((long) fopen(expanded, spec_temp));
}

/* For char-ready? */
/* Under HPUX, select() is declared
    extern int select(size_t, int *, int *, int *, const struct timeval *);
   in sys/time.h.  Under SunOS, the int *'s are fd_set *'s. */

int char_ready_p( FILE* stream )
{
  fd_set readfds;
  struct timeval timeout;

  if (feof(stream))
    return EOF;

  /* Grossly unportable examination of stdio buffer internals */
#if defined(sgi) || defined(HPUX) || defined(sun4) || defined(ultrix)
  if (stream->_cnt)
    return stream->_cnt;
#endif

  FD_ZERO(&readfds);
  FD_SET(fileno(stream), &readfds);
  timerclear(&timeout);

  return select(FD_SETSIZE,
#if defined(hpux)
		(int *)
#endif
		&readfds,
		NULL, NULL, &timeout);
}

/* Timer functions, for the time instruction.
   gettimeofday() version courtesy Basile Starynkevitch.

   gettimeofday() is a Berkeleyism, and ftime() is an old System 7
   thing.  The only function that is POSIX.1 / ANSI C compliant is
   time(), but its resolution is only to the nearest second.  Ugh. */


#define TICKS_PER_SECOND 1000	/* should agree with ps_real_time() */

long ps_real_time()
{
#if defined(HAS_GETTIMEOFDAY)
  struct timeval tv;
  static struct timeval tv_orig;
  static int initp = 0;
  if (!initp) {
    gettimeofday(&tv_orig, NULL);
    initp = 1;
  };
  gettimeofday(&tv, NULL);
  return ((long)((tv.tv_sec - tv_orig.tv_sec)*TICKS_PER_SECOND
		 + (tv.tv_usec - tv_orig.tv_usec)/(1000000/TICKS_PER_SECOND)));
#else /*! HAS_GETTIMEOFDAY*/
  struct timeb tb;
  static struct timeb tb_origin;
  static int initp = 0;
  if (!initp) {
    ftime(&tb_origin);
    initp = 1;
  }
  ftime(&tb);
  return((long)((tb.time - tb_origin.time) * TICKS_PER_SECOND
		+ (tb.millitm / (1000 / TICKS_PER_SECOND))));
#endif /*HAS_GETTIMEOFDAY */
}

long ps_ticks_per_second()
{
  return TICKS_PER_SECOND;
}

long ps_run_time()
{
  struct tms time_buffer;
  static long clock_tick = 0;

  if (clock_tick == 0)
    clock_tick = sysconf(_SC_CLK_TCK); /* POSIX.1, POSIX.2 */
  times(&time_buffer);		/* On Sun, getrusage() would be better */
  return((long)(time_buffer.tms_utime) * (TICKS_PER_SECOND / clock_tick));
}

void when_alarm_interrupt(sig, code, scp)
     int sig, code; 
     struct sigcontext *scp;
{
  extern long Spending_interruptsS;
  Spending_interruptsS |= 1;         /* 1 = 2 ** interrupt/alarm */
  return;
}


int alarm_handler_set_p = 0;

long ps_schedule_interrupt( long delay )
{
  struct itimerval new, old;

/* Under HPUX, this needs to be repeated each time.   --JAR 6/6/93
  if (alarm_handler_set_p == 0) {
    signal(SIGALRM, when_alarm_interrupt);
    alarm_handler_set_p = 1;
  }
*/
  signal(SIGALRM, when_alarm_interrupt);
  
  delay = delay * (1000000 / TICKS_PER_SECOND);
  new.it_value.tv_sec = delay / 1000000;
  new.it_value.tv_usec = delay % 1000000;
  new.it_interval.tv_sec = 0;
  new.it_interval.tv_usec = 0;
  if (0 == setitimer(ITIMER_REAL, &new, &old))
    return (old.it_value.tv_usec + 1000000 * old.it_value.tv_sec)
            / (1000000 / TICKS_PER_SECOND);
  else {
    perror("setitimer");
    /* fprintf(stderr, "call to setitimer failed\n"); */
    return -1;
  }
}

/* Driver loop for tail-recursive calls */

long TTreturn_value;

long TTrun_machine(proc)
     long (*proc) (void);
{
  while (proc != 0)
    proc = (long (*) (void)) (*proc)();
  return TTreturn_value;
}


/* 
   lookup_external_name(name, loc):
   - On success, stores location in "loc" and returns 1.
   - On failure, returns 0.
 */

extern char *object_file;   /* specified via a command line argument */
extern char *reloc_file;    /* dynamic loading will set this */

char
*get_reloc_file()
{
  if (reloc_file != NULL)
    return(reloc_file);
  if (object_file != NULL)
    return(object_file);
  else {
    fprintf(stderr, "Object file not specified on command line\n");
    return(NULL);
  }
}

#ifdef NeXT
#define n_name	n_un.n_name
#endif

long
lookup_external_name( char *name, long *location )
{
#ifdef HAS_DLOPEN
  extern int lookup_dlsym(char*, long*);
  return lookup_dlsym(name, location);
#else /* !HAS_DLOPEN */
  char *reloc_info_file;
  struct nlist name_list[2];
  int status;

  reloc_info_file = get_reloc_file();

  if (reloc_info_file == NULL) {
    fprintf(stderr, "Error during external name lookup\n");
    return(0);
  }

  name_list[0].n_name = name;
  name_list[1].n_name = '\0';

  status = nlist(reloc_info_file, name_list);

  if (status != 0 || (name_list[0].n_value  == 0
		      && name_list[0].n_type == 0))
    return 0;
  else {
    *location = name_list[0].n_value;
    return 1;
  }
#endif /*! HAS_DLOPEN */
}

/* temporary hack until this is added as a PreScheme primitive */

call_external_value( long proc, long nargs, long *args )
{
  return ((long(*)())proc)(nargs, args);
}
