/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <signal.h> /* for sigaction(), pthread_sigmask() / sigprocmask() (POSIX.1) */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>              /* for errno, (POSIX?/ANSI) */
#include <string.h>		/* FD_ZERO sometimes needs this */
#include "sysdep.h"
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#ifdef HAVE_POLL_H
#include <poll.h>
#endif
#include "c-mods.h"
#include "scheme48vm.h"
#include "event.h"

/* turning interrupts and I/O readiness into events */

static sigset_t interrupt_mask;

/*
 * They're basically the same, but the behavior of sigprocmask is
 * undefined in the presence of Pthreads.
 */
#ifdef HAVE_PTHREAD_H
#define SIGMASK pthread_sigmask
#else
/* sigprocmask can be interrupted, while pthread_sigmask cannot */
static int
our_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
  int retval;
  while ((retval = sigprocmask(how, set, oset))
	 && (errno == EINTR))
    ;
  return retval;
}
#define SIGMASK our_sigmask
#endif

static void
block_keyboard_n_alarm_interrupts(void)
{
  if (SIGMASK(SIG_BLOCK, &interrupt_mask, NULL))
    {
      fprintf(stderr,
	      "Failed to block SIGINT/SIGALRM, errno = %d\n",
	      errno);
      exit(1);
    }
}

static void
allow_keyboard_n_alarm_interrupts(void)
{
  if (SIGMASK(SIG_UNBLOCK, &interrupt_mask, NULL))
    {
      fprintf(stderr,
	      "Failed to unblock SIGINT/SIGALRM, errno = %d\n",
	      errno);
      exit(1);
    }
}

/*
 * Unless a signal is being ignored, set up the handler.
 * If we return PSFALSE, something went wrong and errno is set to what.
 */

psbool
s48_setcatcher(int signum, void (*catcher)(int))
{
  struct sigaction	sa;

  if (sigaction(signum, (struct sigaction *)NULL, &sa) != 0)
    return (PSFALSE);
  if (sa.sa_handler == SIG_IGN)
    return (PSTRUE);
  sa.sa_handler = catcher;
  sigemptyset(&sa.sa_mask);

#ifdef HAVE_SIGALTSTACK
  sa.sa_flags = SA_ONSTACK;
#else
  sa.sa_flags = 0;
#endif

  if (sigaction(signum, &sa, (struct sigaction *)NULL) != 0)
    return (PSFALSE);
  return (PSTRUE);
}

static long	keyboard_interrupt_count = 0;

void
s48_when_keyboard_interrupt(int ign)
{
  keyboard_interrupt_count += 1;
  NOTE_EVENT;
  return;
}

/*
   We turn off SIGPIPE interrupts by installing a handler that does nothing.
   Turning them off affects exec()'ed programs, so we don't want to do that.
   Any actual pipe problems are caught when we try to read or write to them.

   We thank Olin Shivers for this hack.
*/

static void
when_sigpipe_interrupt(int ign)
{
  return;
}

/* ticks since last timer-interrupt request */
long		s48_current_time = 0;

static long	alarm_time = -1;
static long	poll_time = -1;
static long	poll_interval = 5;

void
s48_when_alarm_interrupt(int ign)
{
  s48_current_time += 1;
  /* fprintf(stderr, "[tick]"); */

  if ((alarm_time >= 0 && alarm_time <= s48_current_time) ||
      (poll_time >= 0 && poll_time <= s48_current_time)) {
    NOTE_EVENT;
  };
  return;
}

#define USEC_PER_POLL   (1000000 / POLLS_PER_SECOND)

/* delta is in ticks, 0 cancels current alarm */

long
s48_schedule_alarm_interrupt(long delta)
{
  long old;

  /*
  fprintf(stderr, "<scheduling alarm for %ld + %ld>\n", s48_current_time,
	  delta/TICKS_PER_POLL); */

  /* get remaining time */
  if (alarm_time == -1)
    old = -1;
  else
    old = (alarm_time - s48_current_time) * TICKS_PER_POLL;

  /* decrement poll_time and reset current_time */
  if (poll_time != -1)
    poll_time -= s48_current_time;
  s48_current_time = 0;

  /* set alarm_time */
  if (delta == 0) {
    NOTE_EVENT;
    alarm_time = 0; }
  else
    alarm_time = delta / TICKS_PER_POLL;

  return old;
}

/* The next two procedures return times in seconds and ticks */

long
s48_real_time(long *ticks)
{
  struct timeval tv;
  static struct timeval tv_orig;
  static int initp = PSFALSE;
  if (!initp) {
    gettimeofday(&tv_orig, NULL);
    initp = PSTRUE;
  };
  gettimeofday(&tv, NULL);
  *ticks = (tv.tv_usec - tv_orig.tv_usec)/(1000000/TICKS_PER_SECOND);
  return tv.tv_sec - tv_orig.tv_sec;
}

long
s48_run_time(long *ticks)
{
  struct tms time_buffer;
  static long clock_tick = 0;
  long cpu_time;

  if (clock_tick == 0)
    clock_tick = sysconf(_SC_CLK_TCK); /* POSIX.1, POSIX.2 */
  times(&time_buffer);		/* On Sun, getrusage() would be better */

  cpu_time = time_buffer.tms_utime + time_buffer.tms_stime;

  *ticks = (cpu_time % clock_tick) * TICKS_PER_SECOND / clock_tick;
  return cpu_time / clock_tick;
}

void
s48_start_alarm_interrupts(void)
{
  struct itimerval newv, old;

  newv.it_value.tv_sec = 0;
  newv.it_value.tv_usec = USEC_PER_POLL;
  newv.it_interval.tv_sec = 0;
  newv.it_interval.tv_usec = USEC_PER_POLL;
  if (0 != setitimer(ITIMER_REAL, &newv, &old)) {
    perror("setitimer");
    exit(-1); }
}

void
s48_stop_alarm_interrupts(void)
{
  struct itimerval newv, old;

  newv.it_value.tv_sec = 0;
  newv.it_value.tv_usec = 0;
  newv.it_interval.tv_sec = 0;
  newv.it_interval.tv_usec = 0;
  if (0 != setitimer(ITIMER_REAL, &newv, &old)) {
    perror("setitimer");
    exit(-1); }
}

/*
 * We ensure single-threadedness by sending a signal to the main
 * thread, and doing everthing critical there.  This is all probably
 * quite useless without OS threads.
 */

#ifdef HAVE_PTHREAD_H
static pthread_mutex_t external_event_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t main_thread;
#define LOCK_EXTERNAL_EVENTS pthread_mutex_lock(&external_event_mutex)
#define UNLOCK_EXTERNAL_EVENTS pthread_mutex_unlock(&external_event_mutex)
#else
#define LOCK_EXTERNAL_EVENTS
#define UNLOCK_EXTERNAL_EVENTS
#endif

long
s48_dequeue_external_event(char* readyp)
{
  long retval;
  LOCK_EXTERNAL_EVENTS;
  retval = s48_dequeue_external_eventBUunsafe(readyp);
  UNLOCK_EXTERNAL_EVENTS;
  return retval;
}

static char
external_event_pending()
{
  char retval;
  LOCK_EXTERNAL_EVENTS;
  retval = s48_external_event_pendingPUunsafe();
  UNLOCK_EXTERNAL_EVENTS;
  return retval;
}

/* no side effect */
static char
external_event_ready()
{
  char retval;
  LOCK_EXTERNAL_EVENTS;
  retval = s48_external_event_readyPUunsafe();
  UNLOCK_EXTERNAL_EVENTS;
  return retval;
}

void
s48_note_external_event(long uid)
{
  LOCK_EXTERNAL_EVENTS;
  s48_note_external_eventBUunsafe(uid);
  UNLOCK_EXTERNAL_EVENTS;
  NOTE_EVENT;
#ifdef HAVE_PTHREAD_H
  pthread_kill(main_thread, SIG_EXTERNAL_EVENT);
#else
  /* pretty useless probably */
  raise(SIG_EXTERNAL_EVENT);
#endif
}

void
s48_when_external_event_interrupt(int ign)
{
  /* do nothing, except possibly interrupt the running select */
}


/*
 *  ; Scheme version of the get-next-event procedure
 *  ;
 *  ; 1. If there has been a keyboard interrupt, return it.
 *  ; 2. Check for ready ports if enough time has passed since the last check.
 *  ; 3. If there is a ready port, return it.
 *  ; 4. If an alarm is due, return it.
 *  ; 5. If no events are pending, clear the event flags.
 *  (define (get-next-event)
 *    (cond ((> *keyboard-interrupt-count* 0)
 *           (without-interrupts
 *             (lambda ()
 *              (set! *keyboard-interrupt-count*
 *                    (- *keyboard-interrupt-count* 1))))
 *           (values (enum event-type keyboard-interrupt) #f #f))
 *          (else
 *           (cond ((>= *current_time* *poll-time*)
 *                  (queue-ready-ports)
 *                  (set! *poll-time* (+ *time* *poll-interval*))))
 *           (cond ((not (queue-empty? ready-ports))
 *                  (values (enum event-type i/o-completion)
 *                          (dequeue! ready-ports)))
 *                 ((>= *current_time* *alarm-time*)
 *                  (set! *alarm-time* max-integer)
 *                  (values (enum event-type alarm-interrupt) #f))
 *                 (else
 *                  (without-interrupts
 *                    (lambda ()
 *                      (if (and (= *keyboard-interrupt-count* 0)
 *                               (> *alarm-time* *current_time*)
 *                               (> *poll-time* *current_time*))
 *                          (set! *pending-event?* #f))))
 *                  (values (enum event-type no-event) #f))))))
 */

static psbool	there_are_ready_ports(void);
static int	next_ready_port(void);
static int	queue_ready_ports(psbool wait, long seconds, long ticks);

int
s48_get_next_event(long *ready_fd, long *status)
{
  int io_poll_status;
  /*
    fprintf(stderr, "[poll at %d (waiting for %d)]\n", s48_current_time, alarm_time);
    */
  if (keyboard_interrupt_count > 0) {
    block_keyboard_n_alarm_interrupts();
    --keyboard_interrupt_count;
    allow_keyboard_n_alarm_interrupts();
    /* fprintf(stderr, "[keyboard interrupt]\n"); */
    return (KEYBOARD_INTERRUPT_EVENT);
  }
  if (poll_time != -1 && s48_current_time >= poll_time) {
    io_poll_status = queue_ready_ports(PSFALSE, 0, 0);
    if (io_poll_status == NO_ERRORS)
      poll_time = s48_current_time + poll_interval;
    else {
      *status = io_poll_status;
      return (ERROR_EVENT);
    }
  }
  if (there_are_ready_ports()) {
    *ready_fd = next_ready_port();
    *status = 0;   /* chars read or written */
    /* fprintf(stderr, "[i/o completion]\n"); */
    return (IO_COMPLETION_EVENT);
  }
  if (alarm_time != -1 && s48_current_time >= alarm_time) {
    alarm_time = -1;
    /* fprintf(stderr, "[alarm %ld]\n", ticks); */
    return (ALARM_EVENT);
  }
  if (s48_os_signal_pending())
    return (OS_SIGNAL_EVENT);
  if (external_event_pending())
    return (EXTERNAL_EVENT);
  block_keyboard_n_alarm_interrupts();
  if ((keyboard_interrupt_count == 0)
      &&  (alarm_time == -1 || s48_current_time < alarm_time)
      &&  (poll_time == -1 || s48_current_time < poll_time))
    s48_Spending_eventsPS = PSFALSE;
  allow_keyboard_n_alarm_interrupts();
  return (NO_EVENT);
}


/*
 * We keep two queues of ports: those that have a pending operation, and
 * those whose operation has completed.  Periodically, we call select() on
 * the pending ports and move any that are ready onto the other queue and
 * signal an event.
 */
#define FD_QUIESCENT 0			/* idle */
#define FD_READY     1			/* I/O ready to be performed */
#define FD_PENDING   2			/* waiting */

typedef struct fd_struct {
 int	fd,			/* file descriptor */
	status;			/* one of the FD_* constants */
 psbool	is_input;		/* iff input */
 struct fd_struct	*next;	/* next on same queue */
} fd_struct;


/*
 * A queue of fd_structs is empty iff the first field is NULL.  In
 * that case, lastp points to first.
 */
typedef struct fdque {
  long          count;
  fd_struct	*first,
		**lastp;
} fdque;


static long		fd_setsize;
static fd_struct	**fds;
static fdque	ready = {
			 0,
			 NULL,
			 &ready.first
			},
		pending = {
			   0,
			   NULL,
			   &pending.first
			  };


static void		findrm(fd_struct *entry, fdque *que);
static fd_struct	*rmque(fd_struct **link, fdque *que);
static void		addque(fd_struct *entry, fdque *que);
static fd_struct	*add_fd(int fd, psbool is_input);

/*
 * Find a fd_struct in a queue, and remove it.
 */
static void
findrm(fd_struct *entry, fdque *que)
{
  fd_struct	**fp,
    *f;

  for (fp = &que->first; (f = *fp) != entry; fp = &f->next)
    if (f == NULL) {
      fprintf(stderr, "ERROR: findrm fd %d, status %d not on queue.\n",
	      entry->fd, entry->status);
      return;
    }
  rmque(fp, que);
}


/*
 * Given a pointer to the link of a fd_struct, and a pointer to
 * the queue it is on, remove the entry from the queue.
 * The entry removed is returned.
 */
static fd_struct	*
rmque(fd_struct **link, fdque *que)
{
  fd_struct	*res;

  res = *link;
  *link = res->next;
  if (res->next == NULL)
    que->lastp = link;
  que->count--;
  return (res);
}


/*
 * Add a fd_struct to a queue.
 */
static void
addque(fd_struct *entry, fdque *que)
{
  *que->lastp = entry;
  entry->next = NULL;
  que->lastp = &entry->next;
  que->count++;
}


static psbool
there_are_ready_ports(void)
{
  return (ready.first != NULL);
}


static int
next_ready_port(void)
{
  fd_struct	*p;

  p = rmque(&ready.first, &ready);
  p->status = FD_QUIESCENT;
  return (p->fd);
}


/*
 * Put fd on to the queue of ports with pending operations.
 * Return PSTRUE if successful, and PSFALSE otherwise.
 */
psbool
s48_add_pending_fd(int fd, psbool is_input)
{
  fd_struct	*data;

  if (! (0 <= fd && fd < fd_setsize)) {
    fd_setsize *= 2;
    fds = (fd_struct **) realloc (fds, sizeof (fd_struct *) * fd_setsize);
    if (fds == NULL) 
      fprintf(stderr, "ERROR: realloc of fds to %d elements failed, errno = %d\n",
	      errno,
	      fd_setsize);
    return (PSFALSE);
  }
  data = fds[fd];
  if (data == NULL) {
    data = add_fd(fd, is_input);
    if (data == NULL)
      return (PSFALSE); }		/* no more memory */

  data->is_input = is_input;

  if (data->status == FD_PENDING)
    return (PSTRUE);			/* fd is already pending */

  if (data->status == FD_READY)
    findrm(data, &ready);
  data->status = FD_PENDING;
  addque(data, &pending);
  if (poll_time == -1)
    poll_time = s48_current_time + poll_interval;
  return PSTRUE;
}


/*
 * Add a new fd_struct for fd.
 */
static fd_struct	*
add_fd(int fd, psbool is_input)
{
  struct fd_struct	*new_fd;

  new_fd = (struct fd_struct *)malloc(sizeof(struct fd_struct));
  if (new_fd != NULL) {
    new_fd->fd = fd;
    new_fd->status = FD_QUIESCENT;
    new_fd->is_input = is_input;
    new_fd->next = NULL;
    fds[fd] = new_fd;
  }
  return (new_fd);
}


/*
 * Remove fd from any queues it is on.  Returns true if the FD was on a queue
 * and false if it wasn't.
 */
psbool
s48_remove_fd(int fd)
{
  struct fd_struct	*data;

  if (! (0 <= fd && fd < fd_setsize)) {
    fprintf(stderr, "ERROR: s48_remove_fd fd %d not in [0, %d)\n",
	    fd,
	    fd_setsize);
    return PSFALSE;
  }
  data = fds[fd];
  if (data == NULL)
    return PSFALSE;
  if (data->status == FD_PENDING) {
    findrm(data, &pending);
    if (pending.first == NULL)
      poll_time = -1;
  } else if (data->status == FD_READY)
    findrm(data, &ready);
  free((void *)data);
  fds[fd] = NULL;
  return PSTRUE;
}


int
s48_wait_for_event(long max_wait, psbool is_minutes)
{
  int	status;
  long	seconds,
	ticks;

  /* fprintf(stderr, "[waiting]\n"); */

  s48_stop_alarm_interrupts();
  ticks = 0;
  if (max_wait == -1)
    seconds = -1;
  else if (is_minutes)
    seconds = max_wait * 60;
  else {
    seconds = max_wait / TICKS_PER_SECOND;
    ticks = max_wait % TICKS_PER_SECOND;
  }
  if (keyboard_interrupt_count > 0)
    status = NO_ERRORS;
  else {
    status = queue_ready_ports(PSTRUE, seconds, ticks);
    if (there_are_ready_ports()
	|| external_event_ready())
      NOTE_EVENT;
  }
  s48_start_alarm_interrupts();
  return (status);
}


#ifdef HAVE_POLL
static struct pollfd    *pollfds;
static long             pollfds_size;

/*
 * Call poll() on the pending ports and move any ready ones to the ready
 * queue.  If wait is true, seconds is either -1 (wait forever) or the
 * maximum number of seconds to wait (with ticks any additional ticks).
 * The returned value is a status code.
 */
static int
queue_ready_ports(psbool wait, long seconds, long ticks)
{
  int           npollfds;
  int           timeout;
  fd_struct	*fdp,
    		**fdpp;
  int		left;

  if ((! wait)
      &&  (pending.first == NULL))
    return (NO_ERRORS);

  if (pending.count > pollfds_size) {
    pollfds_size *= 2;
    pollfds = (struct pollfd *) realloc (pollfds, 
					 sizeof (struct pollfd) * pollfds_size);
    if (pollfds == NULL) {
      fprintf(stderr,
	      "Failed to realloc array of file descriptors to poll, errno = %d\n",
	      errno);
      exit(1);
    }
  }

  for (fdp = pending.first, npollfds = 0; fdp != NULL; fdp = fdp->next, npollfds++) {
    pollfds[npollfds].fd = fdp->fd;
    pollfds[npollfds].events = fdp->is_input? POLLIN : POLLOUT;
  }

  if (wait)
    if (seconds == -1)
      timeout = -1;
    else
      timeout = (int) seconds;
  else
    timeout = 0;

  while(1) {
    left = poll(pollfds, pending.count, timeout);
    if (left > 0) {
      fdpp = &pending.first;
      for (fdp = *fdpp, npollfds = 0;
	   (left > 0) && (fdp != NULL); 
	   fdp = *fdpp, npollfds++) {
	if (pollfds[npollfds].revents & (fdp->is_input? POLLIN : POLLOUT) 
	    | POLLHUP | POLLERR) {
	  rmque(fdpp, &pending);
	  fdp->status = FD_READY;
	  addque(fdp, &ready);
	}
	else
	  fdpp = &fdp->next;
      }
      if (pending.first == NULL)
	poll_time = -1;
      return NO_ERRORS;
    }
    else if (left == 0)
      return NO_ERRORS;
    else if (errno == EINTR) {
      if (external_event_ready())
	return NO_ERRORS;
      timeout = 0;		/* turn off blocking and try again */
    }	      
    else
      return errno;
  }
}
#else /* not HAVE_POLL */
/*
 * Call select() on the pending ports and move any ready ones to the ready
 * queue.  If wait is true, seconds is either -1 (wait forever) or the
 * maximum number of seconds to wait (with ticks any additional ticks).
 * The returned value is a status code.
 */
static int
queue_ready_ports(psbool wait, long seconds, long ticks)
{
  fd_set	reads,
    		writes,
    		alls;
  int		limfd;
  fd_struct	*fdp,
    		**fdpp;
  int		left;
  struct timeval	tv,
    			*tvp;

  if ((! wait)
      &&  (pending.first == NULL))
    return (NO_ERRORS);
  FD_ZERO(&reads);
  FD_ZERO(&writes);
  FD_ZERO(&alls);
  limfd = 0;
  for (fdp = pending.first; fdp != NULL; fdp = fdp->next) {
    FD_SET(fdp->fd, fdp->is_input ? &reads : &writes);
    FD_SET(fdp->fd, &alls);
    if (limfd <= fdp->fd)
      limfd = fdp->fd + 1;
  }
  tvp = &tv;
  if (wait)
    if (seconds == -1)
      tvp = NULL;
    else {
      tv.tv_sec = seconds;
      tv.tv_usec = ticks * (1000000 / TICKS_PER_SECOND);
    }
  else
    timerclear(&tv);
  while(1) {
    left = select(limfd, &reads, &writes, &alls, tvp);
    if (left > 0) {
      fdpp = &pending.first;
      while (left > 0 && (fdp = *fdpp) != NULL)
	if ((FD_ISSET(fdp->fd, &alls))
	    ||  (FD_ISSET(fdp->fd, fdp->is_input ? &reads : &writes))) {
	  --left;
	  rmque(fdpp, &pending);
	  fdp->status = FD_READY;
	  addque(fdp, &ready);
	} else
	  fdpp = &fdp->next;
      if (pending.first == NULL)
	poll_time = -1;
      return NO_ERRORS;
    }
    else if (left == 0)
      return NO_ERRORS;
    else if (errno == EINTR) {
      if (external_event_ready())
	return NO_ERRORS;
      tvp = &tv;		/* turn off blocking and try again */
      timerclear(tvp);
    }	      
    else
      return errno;
  }
}
#endif /* not HAVE_POLL */

void
s48_sysdep_init(void)
{
#ifdef HAVE_PTHREAD_H
  main_thread = pthread_self();
#endif

#ifdef HAVE_SIGALTSTACK 
  stack_t ss;
  
  ss.ss_sp = malloc(SIGSTKSZ);
  if (ss.ss_sp == NULL)
    fprintf(stderr,
	    "Failed to malloc alt stack, errno = %d\n",
	    errno);
  ss.ss_size = SIGSTKSZ;
  ss.ss_flags = 0;
  if (sigaltstack(&ss, NULL) == -1)
    fprintf(stderr,
	    "Failed to sigaltstack, errno = %d\n",
	    errno);
#endif

#ifdef HAVE_POLL
  pollfds_size = FD_SETSIZE;
  pollfds = (struct pollfd *) calloc (sizeof (struct pollfd), pollfds_size);

  if (pollfds == NULL) {
    fprintf(stderr,
	    "Failed to alloc array of file descriptors to poll with %d elements, errno = %d\n",
	    pollfds_size,
	    errno);
    exit(1);
  }
#endif /* HAVE_POLL */

  fd_setsize = FD_SETSIZE;
  fds = (fd_struct **) calloc (sizeof (fd_struct *), fd_setsize);

  if (fds == NULL) {
    fprintf(stderr,
	    "Failed to alloc fds with %d elements, errno = %d\n",
	    fd_setsize,
	    errno);
    exit(1);
  }

  if (!s48_setcatcher(SIGINT, s48_when_keyboard_interrupt)
      || !s48_setcatcher(SIGALRM, s48_when_alarm_interrupt)
      || !s48_setcatcher(SIGPIPE, when_sigpipe_interrupt)
      || !s48_setcatcher(SIG_EXTERNAL_EVENT, s48_when_external_event_interrupt)) {
    fprintf(stderr,
	    "Failed to install signal handlers, errno = %d\n",
	    errno);
    exit(1);
  }

  sigemptyset(&interrupt_mask);
  sigaddset(&interrupt_mask, SIGINT);
  sigaddset(&interrupt_mask, SIGALRM);

  s48_start_alarm_interrupts();
}

