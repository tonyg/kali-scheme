/* Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#define _WIN32_WINNT 0x0400/* for SetWaitableTimer */

#include <windows.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>              /* for errno, (ANSI) */
#include "c-mods.h"
#include "scheme48vm.h"
#include "event.h"

/* from Lars Bergstrom's port */

static void
set_signal_catcher(int signum, void (*catcher)(int))
{
  signal(signum, catcher);
}

static long keyboard_interrupt_count = 0;

static void
when_keyboard_interrupt(int ign)
{
  keyboard_interrupt_count += 1;
  NOTE_EVENT;
  return;
}

static void
start_control_c_interrupts()
{
  set_signal_catcher(SIGINT, when_keyboard_interrupt);
}
   
/* ticks since last timer-interrupt request
   (except for some types, identical to Unix code) */
long		s48_current_time = 0;

static long	alarm_time = -1;
static long	poll_time = -1;
static long	poll_interval = 5;

static void
when_alarm_interrupt()
{
  s48_current_time += 1;
  /* fprintf(stderr, "[tick %ld]", s48_current_time); */

  if ((alarm_time >= 0 && alarm_time <= s48_current_time) ||
      (poll_time >= 0 && poll_time <= s48_current_time))
      NOTE_EVENT;
}

#define USEC_PER_POLL   (1000000 / POLLS_PER_SECOND)

/* delta is in ticks, 0 cancels current alarm */

long
s48_schedule_alarm_interrupt(long delta)
{
  long old;

  /* fprintf(stderr, "<scheduling alarm for %ld + %ld>\n", s48_current_time,
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
  if (delta == 0)
    {
      NOTE_EVENT;
      alarm_time = 0;
    }
  else
    alarm_time = delta / TICKS_PER_POLL;

  return old;
}

/* The next two procedures return times in seconds and ticks
   (also from Lars Bergstrom's version */

static DWORD startup_real_time_ticks;

long s48_real_time(long *ticks)
{
  DWORD now;

  now = GetTickCount();

  *ticks = ((now - startup_real_time_ticks) % 1000) * (TICKS_PER_SECOND / 1000);
  return (now - startup_real_time_ticks) / 1000;
}

long s48_run_time(long *ticks)
{
  FILETIME create, exit, user, kernel;
  SYSTEMTIME systime;
  HANDLE this_process;
  BOOL status;

  this_process = GetCurrentProcess();

  status = GetProcessTimes(this_process, &create, &exit, &user, &kernel);

  status = FileTimeToSystemTime(&user, &systime);

  /* go from 100ns to 1ms resolution */
  *ticks = systime.wMilliseconds * (TICKS_PER_SECOND / 1000);

  return systime.wSecond;
}

static HANDLE alarm_thread = NULL;

DWORD
alarm_thread_func(LPDWORD id)
{
  for (;;)
    {
      Sleep(USEC_PER_POLL / 1000);
      when_alarm_interrupt();
    }
  return 0; /* shouldn't get here */
}

/*
 * If it's being called for the first time, create the thread
 * If it's being called again, that means that it's been suspended
 * and should be resumed.
 */

void
s48_start_alarm_interrupts(void)
{
  if (alarm_thread == NULL)
    {
      DWORD alarm_thread_id;
      alarm_thread = CreateThread(NULL, /* no security attributes */
				  0,    /* default stack size */
				  (LPTHREAD_START_ROUTINE) alarm_thread_func,
				  NULL,  /* argument to thread -- ignored */
				  0,    /* default creation flags */
				  &alarm_thread_id);
      if (alarm_thread == NULL) 
	{
	  fprintf(stderr, "failure creating alarm timer thread\n");
	  exit(-1);
	}
    }
  else
    ResumeThread(alarm_thread);
}

void
s48_stop_alarm_interrupts(void)
{
  if (alarm_thread != NULL)
    SuspendThread(alarm_thread);
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
 long   chars_processed;
 bool	is_input;		/* iff input */
 struct fd_struct	*next;	/* next on same queue */
} fd_struct;


/*
 * A queue of fd_structs is empty iff the first field is NULL.  In
 * that case, lastp points to first.
 */
typedef struct fdque
{
  fd_struct	*first, **lastp;
} fdque;


static fd_struct *fds[FD_SETSIZE];
static fdque ready = {
  NULL,
  &ready.first
};

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
  return (res);
}

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
 * Add a fd_struct to a queue.
 */
static void
addque(fd_struct *entry, fdque *que)
{
  *que->lastp = entry;
  entry->next = NULL;
  que->lastp = &entry->next;
}


static bool
there_are_ready_ports(void)
{
  return (ready.first != NULL);
}


static int
next_ready_port(long* chars_processed)
{
  fd_struct	*p;

  p = rmque(&ready.first, &ready);
  p->status = FD_QUIESCENT;
  *chars_processed = p->chars_processed;
  return (p->fd);
}

/*
 * Put fd on to the queue of ports with ready operations.
 * Return TRUE if successful, and FALSE otherwise.
 */

bool
s48_add_ready_fd(long fd, bool is_input, long chars_processed)
{
  fd_struct* data = fds[fd];  /* we created this before */

  data->is_input = is_input;
  data->chars_processed = chars_processed;

  if (data->status == FD_READY)
    return (TRUE); /* fd is already ready */

  addque(data, &ready);

  return TRUE;
}

/*
 * Add a new fd_struct for fd.
 */
static fd_struct	*
add_fd(long fd, bool is_input)
{
  struct fd_struct	*new;

  new = (struct fd_struct *)malloc(sizeof(*new));
  if (new != NULL) {
    new->fd = fd;
    new->status = FD_QUIESCENT;
    new->is_input = is_input;
    new->next = NULL;
    fds[fd] = new;
  }
  return (new);
}

static fd_struct	*
get_or_create_fd_struct(long fd, bool is_input)
{
  if (fds[fd] == NULL)
    return add_fd(fd, is_input);
  else
    return fds[fd];
}

bool
s48_add_pending_fd(int fd, bool is_input)
{
  fd_struct* data = get_or_create_fd_struct(fd, is_input);
  if (data)
    {
      if (data->status != FD_PENDING)
	{
	  data->status = FD_PENDING;
	  if (poll_time == -1)
	    poll_time = s48_current_time + poll_interval;
	}
      return TRUE;
    }
  else
    return FALSE;
    
}

bool
s48_is_pending(long fd)
{
  return (fds[fd] != NULL) && (fds[fd]->status == FD_PENDING);
}
 

/*
 * Remove fd from any queues it is on.  Returns true if the FD was on a queue
 * and false if it wasn't.
 */
bool
s48_remove_fd(int fd)
{
  struct fd_struct	*data;

  if (! (0 <= fd && fd < FD_SETSIZE)) {
    fprintf(stderr, "ERROR: s48_remove_fd fd %d not in [0, %d)\n",
	    fd,
	    FD_SETSIZE);
    return FALSE;
  }
  data = fds[fd];
  if (data == NULL)
    return FALSE;
  if (data->status == FD_PENDING) {
    /* the callback will see this and no-op */
    data->status = FD_QUIESCENT;
/*####    if (pending.first == NULL)
      poll_time = -1; */
  } else if (data->status == FD_READY)
    findrm(data, &ready);
  free((void *)data);
  fds[fd] = NULL;
  return TRUE;
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

int
s48_get_next_event(long *ready_fd, long *status)
{
  /* extern int s48_os_signal_pending(void); */

  int io_poll_status;
  /*
    fprintf(stderr, "[poll at %d (waiting for %d)]\n", s48_current_time, alarm_time);
    */
  if (keyboard_interrupt_count > 0) {
    --keyboard_interrupt_count;
    /* fprintf(stderr, "[keyboard interrupt]\n"); */
    return (KEYBOARD_INTERRUPT_EVENT);
  }
  if (poll_time != -1 && s48_current_time >= poll_time) {
    SleepEx(0, TRUE);
    io_poll_status = NO_ERRORS; // ####
    if (io_poll_status == NO_ERRORS)
      poll_time = s48_current_time + poll_interval;
    else {
      *status = io_poll_status;
      return (ERROR_EVENT);
    }
  }
  if (there_are_ready_ports()) {
    *ready_fd = next_ready_port(status);
    /* fprintf(stderr, "[i/o completion on port %ld]\n", *ready_fd); */
    return (IO_COMPLETION_EVENT);
  }
  if (alarm_time != -1 && s48_current_time >= alarm_time) {
    alarm_time = -1;
    /* fprintf(stderr, "[alarm %ld]\n", s48_current_time); */
    return (ALARM_EVENT);
  }
  /*
  if (s48_os_signal_pending())
    return (OS_SIGNAL_EVENT);
  */
  if ((keyboard_interrupt_count == 0)
      &&  (alarm_time == -1 || s48_current_time < alarm_time)
      &&  (poll_time == -1 || s48_current_time < poll_time))
    s48_Spending_eventsPS = FALSE;
  return (NO_EVENT);
}

int
s48_wait_for_event(long max_wait, bool is_minutes)
{
  int	status;

  /* fprintf(stderr, "[waiting]\n"); */

  s48_stop_alarm_interrupts();

  if (keyboard_interrupt_count >  0)
    status = NO_ERRORS;
  else {
    DWORD dwMilliseconds;
    if (max_wait == -1)
      dwMilliseconds = INFINITE;
    else if (is_minutes)
      dwMilliseconds = max_wait * 60 * 1000;
    else
      dwMilliseconds = max_wait * (1000 / TICKS_PER_SECOND);

    SleepEx(dwMilliseconds,
	    TRUE);
    if (there_are_ready_ports())
      NOTE_EVENT;
  }
  s48_start_alarm_interrupts();
  return status;
}


void
s48_sysdep_init(void)
{
  startup_real_time_ticks = GetTickCount();

  start_control_c_interrupts();

  s48_start_alarm_interrupts();

  {
    extern void s48_fd_io_init();
    s48_fd_io_init();
  }
}

/* kludge */

void
s48_initialize_external_modules()
{
}
