/* sysdep.h.  Generated automatically by configure.  */
/* sysdep.h.in for Scheme 48.  Originally generated automatically from
   configure.in by autoheader, but then modified manually to add
   NLIST_HAS_N_NAME, etc.  The "configure" script creates sysdep.h
   from this file.  If "configure" doesn't work for you, copy this
   file to sysdep.h and make any appriopriate modifications.  If
   you're not sure what modifications would be appriopriate, it is
   generally safe to leave things as they are. */


/* Define const to empty if the ANSI C "const" keyword doesn't work.  */
/* #undef const */

/* Define this appropriately as the return value of a signal handler on
   your system. */
#define RETSIGTYPE void

/* Define FILE_HAS__CNT if streams, as defined in <stdio.h>, have _cnt
   components.  This is widespread but basically highly unportable.
   It is used in order to implement CHAR-READY?. */
#define FILE_HAS__CNT 1

/* Define HAVE_SELECT if the BSD-derived select() call is available.
   select() is used in order to implement CHAR-READY?. */
#define HAVE_SELECT 1

/* Define HAVE_SETITIMER if the BSD-derived setitimer() call is
   available.  In the absence of setitimer(), alarm() will be used.*/
#define HAVE_SETITIMER 1

/* Define HAVE_GETTIMEOFDAY if you have gettimeofday().  This is a BSD
   thing that returns elapsed time to the nearest microsecond (yeah,
   right).  In the absence of gettimeofday(), ftime() or time() will
   be used. */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have ftime() and its associated header file sys/timeb.h.
   This is an ancient Unix version 7 thing, but if gettimeofday()
   isn't available then it may be the only way to get sub-second
   resolution for elapsed time. */
#define HAVE_FTIME 1
#define HAVE_SYS_TIMEB_H 1

/* Define if you have the nlist() function.  This is a
   not-very-portable way of looking up external symbols. */
#define HAVE_NLIST 1

/* Define if struct nlist, defined in <nlist.h>, has an n_name component.
   If it doesn't then we expect it to have an n_un component. */
#define NLIST_HAS_N_NAME 1

/* Define if you have dlopen() and related routines (dynamic linking
   of shared object files). */
/* #undef HAVE_DLOPEN */

/* Define if you have header file libgen.h.  This has something to do
   with locating shared object files. */
/* #undef HAVE_LIBGEN_H */

/* Define if you have the chroot() system call (quite inessential!). */
#define HAVE_CHROOT 1

/* Define if you have the Posix sigaction() call. */
#define HAVE_SIGACTION 1

/* Define if you have socket() and related functions. */
#define HAVE_SOCKET 1

/* Define if Posix "time.h" include file is <posix/time.h> instead of
   the standard <time.h>.  (MIPS RISC/OS) */
/* #undef POSIX_TIME_H */

/* Define this if /bin/ld exists and accepts a BSD-style -A argument
   (for dynamic loading of .o files). */
/* #undef ANCIENT_DYNLOAD */


#if !defined(HAVE_SIGACTION)
struct sigaction {
  void (*sa_handler)();
  int sa_mask;
  int sa_flags;
};

#define sigaction(sig, act, oact) signal((sig), (act)->sa_handler)
#define sigemptyset(foo) 0
#endif

/* Under HPUX, select() is declared
      extern int select(size_t, int *, int *, int *, const struct timeval *);
   in sys/time.h.  This is probably Posix, but there's no knowing.
   Under SunOS, the int *'s are fd_set *'s. */

#if !defined(fd_set_param)
#if defined(hpux)
#  define fd_set_param int
#else
#  define fd_set_param fd_set
#endif
#endif
