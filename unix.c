/* Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
   If you have concrete suggestions for improvements, they are quite
   welcome.  Please send them to scheme-48-bugs@martigny.ai.mit.edu.

   Non-Posixisms:

   The most annoying thing here is char_ready_p, which is needed for
   R^nRS and is nonportable in two different ways.  There's no
   portable (Posix or anything else) way to figure whether a stdio
   stream has buffered input.  And even if we knew that, there's no
   portable way to find out whether input would block - select() seems
   to be a BSD thing, and AT&T's ioctl() doesn't work for arbitrary
   devices.  If it can't figure out what to do, it just prints a
   warning and returns #t.

   setitimer(), a Berkeleyism, is used if it's available; otherwise
   alarm() is used, which only has 1-second resolution.  Timer
   interrupts are used by the threads apckage but not by the base
   system.

   gettimeofday() is BSD.  ftime() is Version 7 (!).  The POSIX.1/ANSI
   C alternative to these is time(), which returns a number of
   seconds.  There seems to be some disagreement over the number of
   arguments to gettimeofday().

   nlist() derives from ancient Version 6 and 7 unix, so it's pretty
   widespread, but everyone wants to phase it out because it's not
   very abstract.

   Posix/ANSI C things used:
     feof fopen fprintf perror strlen strncpy etc.
     time (if gettimeofday and ftime are unavailable)

   Posix things used: (beware, PC and Mac hackers)
     alarm (if setitimer is unavailable)
     fileno (but only when select is being used)
     getenv getpwnam sigaction sysconf
     times  -- clock() is ANSI but wraps around every 36 minutes

   Other things used (BSD etc.), only when available:
     ftime (if gettimeofday is unavailable)
     gettimeofday
     nlist
     select
     setitimer

   <time.h> is ANSI C, but we apparently don't use anything from it
   that is part of ANSI C.  Under HPUX, the man pages tell one to
   use it in order to get declarations for the things that under SunOS
   are declared in <sys/time.h>.  Oh well, it can't hurt, can it?  Oh
   yeah, this is Unix, of course it can...
*/

#include "sysdep.h"

#include <stdio.h>
#include <stdlib.h>		/* for getenv(), etc. (POSIX?/ANSI) */
#include <string.h>		/* for strncpy(), etc. (POSIX/ANSI) */
#include <pwd.h>		/* for getpwnam() (POSIX.1) */
#include <unistd.h>		/* for sysconf() (POSIX.1/.2)*/
#include <sys/times.h>		/* for times() (POSIX.1) */
#include <signal.h>		/* for sigaction() (POSIX.1) */

#if defined(HAVE_POSIX_TIME_H)
#  include <posix/time.h>	/* RISC/OS + gcc lossage */
#  define _XOPEN_SOURCE 1
#else
#  include <time.h>
#endif

#if defined(HAVE_SETITIMER) || defined(HAVE_GETTIMEOFDAY)
#  include <sys/time.h>		/* for struct itimerval, ITIMER_REAL (Sun) */
#endif

#if defined(HAVE_SELECT)
#  include <sys/types.h>	/* for FD_SET and friends (BSD) */
#endif

#if defined(HAVE_SYS_TIMEB_H)
#  include <sys/timeb.h>	/* for ftime() */
#endif

#if defined(HAVE_NLIST)
#  include <nlist.h>		/* conforms to "SVID2", whatever that is */
#endif


#define INTERRUPT_ALARM     0	/* Cf. rts/arch.scm */
#define INTERRUPT_KEYBOARD  1

extern long Spending_interruptsS;


/* Signal handlers */

static RETSIGTYPE
when_keyboard_interrupt(sig, code, scp)
     int sig, code; 
     struct sigcontext *scp;
{
  Spending_interruptsS |= (1 << INTERRUPT_KEYBOARD);
  /* The following might be necessary with signal(), but shouldn't be
     with sigaction() (I think) */
  /* sigaction(SIGINT, &keyboard_action, NULL); */
  return;
}

static RETSIGTYPE
when_alarm_interrupt(sig, code, scp)
     int sig, code; 
     struct sigcontext *scp;
{
  Spending_interruptsS |= (1 << INTERRUPT_ALARM);
  return;
}


/* OS-dependent initialization */

static struct sigaction keyboard_action;
static struct sigaction alarm_action;

void
sysdep_init()
{
  keyboard_action.sa_handler = when_keyboard_interrupt;
  keyboard_action.sa_flags = 0;
  sigemptyset(&keyboard_action.sa_mask);

  alarm_action.sa_handler = when_alarm_interrupt;
  alarm_action.sa_flags = 0;
  sigemptyset(&alarm_action.sa_mask);

  sigaction(SIGINT, &keyboard_action, NULL);
}

/* ---------------------------------------- */
/* For char-ready? */

int
char_ready_p( FILE* stream )
{
  fd_set readfds;
  struct timeval timeout;
  static int warnedp = 0;

  if (feof(stream))
    return EOF;

  /* Grossly unportable examination of stdio buffer internals. */
#if defined(FILE_HAS__CNT)
  if (stream->_cnt)
    return 1;
#elif defined(__linux__)
  if (stream->_IO_read_ptr < stream->_IO_read_end)
    return 1;
#else

  /* Add new cases here AND SEND THEM TO scheme-48@martigny.ai.mit.edu
     SO THAT THEY CAN GO INTO THE NEXT RELEASE!  (That means you, Olin.)
     It's generally pretty easy to figure out what to put here by
     examining /usr/include/stdio.h.  If the input stream's buffer is
     nonempty, just return any positive value. */
  if (!warnedp) {
    fprintf(stderr, "Warning: incomplete char-ready? implementation.\n");
    warnedp = 1; }
  return 1;
#endif

  /* Nothing in the buffer.  Find out whether a read would block. */
#if defined(HAVE_SELECT)
  FD_ZERO(&readfds);
  FD_SET(fileno(stream), &readfds);
  timerclear(&timeout);
  return select(FD_SETSIZE,
		(fd_set_param *)
		&readfds,
		NULL, NULL, &timeout);
#else /* No select() - but there will generally be some other way to do this.*/
  if (!warnedp) {
    fprintf(stderr, "Warning: incomplete char-ready? implementation.\n");
    warnedp = 1; }
  return 1;
#endif
}

/* ---------------------------------------- */
/* For open-xxput-file */

FILE *
ps_open(char *filename, char *spec)
{
# define FILE_NAME_SIZE 256
  char filename_temp[FILE_NAME_SIZE];
  char *expanded;
  extern char *expand_file_name(char *, char *, int);

  expanded = expand_file_name(filename, filename_temp, FILE_NAME_SIZE);
  if (expanded == NULL)
    return NULL;
  return fopen(expanded, spec);
}

/*
   Expanding Unix filenames
   Unix Sucks
   Richard Kelsey Wed Jan 17 21:40:26 EST 1990
   Later modified by others who wish to remain anonymous
   "ps_" stands for "Pre-Scheme"

   Expands initial ~ and ~/ in string `name', leaving the result in `buffer'.
   `buffer_len' is the length of `buffer'.

   Note: strncpy(x, y, n) copies from y to x.
*/

char *
expand_file_name (char *name, char *buffer, int buffer_len)
{
# define USER_NAME_SIZE 256
  char *dir, *p, user_name[USER_NAME_SIZE];
  struct passwd *user_data;
  int dir_len, i;
  extern char *getenv();
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
  expand_file_name(argv[1], buffer, 32);
  printf("%s\n", buffer);
  return(0);
}
*/

/* ---------------------------------------- */
/* Timer functions, for the time instruction.
   gettimeofday() version courtesy Basile Starynkevitch.

   From: Jim.Rees@umich.edu
   Date: Sun, 26 Dec 93 16:06:08 EST

   In unix.c, the gettimeofday code is wrong.  ...
   I suggest the following fix, which will work on both sysV and bsd
   machines.  If you (or your compiler) are squeamish about passing
   too many parameters in to a system call, you might want to do it
   differently....
 */

#define TICKS_PER_SECOND 1000	/* should agree with ps_real_time() */

long
ps_real_time()
{
#if defined(HAVE_GETTIMEOFDAY)
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
#elif defined(HAVE_FTIME)
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
#else
  return (long)time(NULL) * TICKS_PER_SECOND;
#endif /*HAVE_GETTIMEOFDAY */
}

long
ps_run_time()
{
  struct tms time_buffer;
  static long clock_tick = 0;

  if (clock_tick == 0)
    clock_tick = sysconf(_SC_CLK_TCK); /* POSIX.1, POSIX.2 */
  times(&time_buffer);		/* On Sun, getrusage() would be better */
  return((long)(time_buffer.tms_utime * TICKS_PER_SECOND) / clock_tick);
}

long
ps_ticks_per_second()
{
  return TICKS_PER_SECOND;
}

long
ps_schedule_interrupt(long delay)
{
  sigaction(SIGALRM, &alarm_action, NULL);

#if defined(HAVE_SETITIMER)
  { struct itimerval new, old;

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
      return -1;
    }
  }
#else
  /* Round up to nearest second.  0 means cancel... */
  return alarm((delay + TICKS_PER_SECOND - 1) / TICKS_PER_SECOND)
	   * TICKS_PER_SECOND;
#endif
}

/* ---------------------------------------- */
/* 
   lookup_external_name(name, loc):
   - On success, stores location in "loc" and returns 1.
   - On failure, returns 0.
 */

#if !defined(NLIST_HAS_N_NAME)
#define n_name	n_un.n_name
#endif

long
lookup_external_name(char *name, long *location)
{
#if defined(HAVE_DLOPEN)
  extern int lookup_dlsym(char*, long*);
  return lookup_dlsym(name, location);
#elif defined(HAVE_NLIST)
  extern char *get_reloc_file();
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
#else
  return 0;
#endif /*! HAVE_DLOPEN */
}

extern char *object_file;   /* specified via a command line argument */
extern char *reloc_file;    /* dynamic loading will set this */

char *
get_reloc_file()
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

/* ---------------------------------------- */
/* Non-unix-specific things. */

/* Driver loop for tail-recursive calls */

long TTreturn_value;

long
TTrun_machine(long (*proc) (void))
{
  while (proc != 0)
    proc = (long (*) (void)) (*proc)();
  return TTreturn_value;
}

/* Temporary hack until this is added as a Pre-Scheme primitive */

long
call_external_value(long proc, long nargs, long *args)
{
  return ((long(*)())proc)(nargs, args);
}
