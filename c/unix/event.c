#include <signal.h>		/* for sigaction() (POSIX.1) */
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>              /* for errno, (POSIX?/ANSI) */
#include "sysdep.h"
#include "scheme48vm.h"


/* turning interrupts and I/O readiness into events */

#define block_interrupts()
#define allow_interrupts()

static struct sigaction	keyboard_action;
static struct sigaction	alarm_action;

static void	when_keyboard_interrupt();
static void	when_alarm_interrupt();
static void	start_alarm_interrupts(void);


int
sysdep_init(void)
{
  keyboard_action.sa_handler = when_keyboard_interrupt;
  keyboard_action.sa_flags = 0;
  sigemptyset(&keyboard_action.sa_mask);

  alarm_action.sa_handler = when_alarm_interrupt;
  alarm_action.sa_flags = 0;
  sigemptyset(&alarm_action.sa_mask);

  if (0 != sigaction(SIGINT, &keyboard_action, NULL))
    return errno;

  if (0 != sigaction(SIGALRM, &alarm_action, NULL))
    return errno;

  start_alarm_interrupts();

  return NO_ERRORS;
}

extern unsigned char	Spending_eventsPS;
extern unsigned char	Spending_interruptPS;


/*
 * Fix (HCC) NOTE_EVENT so that it will act like a single
 * statement.
 */
#define	NOTE_EVENT					\
	do {						\
		Spending_eventsPS = 1;			\
		Spending_interruptPS = 1;		\
	} while (0)

static long	keyboard_interrupt_count = 0;

static void
when_keyboard_interrupt()
{
  keyboard_interrupt_count += 1;
  NOTE_EVENT;
  /* The following might be necessary with signal(), but shouldn't be
     with sigaction() (I think) */
  /* sigaction(SIGINT, &keyboard_action, NULL); */
  return;
}

long		current_time = 0;	/* ticks since last timer-interrupt request */
static long	alarm_time = -1;
static long	poll_time = -1;
static long	poll_interval = 5;

static void
when_alarm_interrupt()
{
  current_time += 1;
  if ((alarm_time >= 0 && alarm_time <= current_time) ||
      (poll_time >= 0 && poll_time <= current_time)) {
    NOTE_EVENT;
  };
  return;
}

#define USEC_PER_POLL   (1000000 / POLLS_PER_SECOND)

/* delta is in ticks, 0 cancels current alarm */

long
schedule_alarm_interrupt(long delta)
{
  long old;
/*
  fprintf(stderr, "<scheduling alarm for %ld + %ld>\n", current_time,
	  delta/TICKS_PER_POLL);
*/

  /* get remaining time */
  if (alarm_time == -1)
    old = -1;
  else
    old = (alarm_time - current_time) * TICKS_PER_POLL;

  /* decrement poll_time and reset current_time */
  if (poll_time != -1)
    poll_time -= current_time;
  current_time = 0;

  /* set alarm_time */
  if (delta == 0) {
    NOTE_EVENT;
    alarm_time = 0; }
  else
    alarm_time = delta / TICKS_PER_POLL;

  return old;
}

/* The next two procedures return times in seconds and ticks */

long real_time(long *ticks)
{
  struct timeval tv;
  static struct timeval tv_orig;
  static int initp = 0;
  if (!initp) {
    gettimeofday(&tv_orig, NULL);
    initp = 1;
  };
  gettimeofday(&tv, NULL);
  *ticks = (tv.tv_usec - tv_orig.tv_usec)/(1000000/TICKS_PER_SECOND);
  return tv.tv_sec - tv_orig.tv_sec;
}

long run_time(long *ticks)
{
  struct tms time_buffer;
  static long clock_tick = 0;

  if (clock_tick == 0)
    clock_tick = sysconf(_SC_CLK_TCK); /* POSIX.1, POSIX.2 */
  times(&time_buffer);		/* On Sun, getrusage() would be better */
  *ticks = (time_buffer.tms_utime % clock_tick)
	         * TICKS_PER_SECOND / clock_tick;
  return time_buffer.tms_utime / clock_tick;
}

static void
start_alarm_interrupts()
{
  struct itimerval new, old;

  new.it_value.tv_sec = 0;
  new.it_value.tv_usec = USEC_PER_POLL;
  new.it_interval.tv_sec = 0;
  new.it_interval.tv_usec = USEC_PER_POLL;
  if (0 != setitimer(ITIMER_REAL, &new, &old)) {
    perror("setitimer");
    exit(-1); }
}

static void
stop_alarm_interrupts()
{
  struct itimerval new, old;

  new.it_value.tv_sec = 0;
  new.it_value.tv_usec = 0;
  new.it_interval.tv_sec = 0;
  new.it_interval.tv_usec = 0;
  if (0 != setitimer(ITIMER_REAL, &new, &old)) {
    perror("setitimer");
    exit(-1); }
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

static bool	there_are_ready_ports(void);
static int	next_ready_port(void);
static int	queue_ready_ports(bool wait, long seconds, long ticks);

int
get_next_event(long *ready_fd, long *status)
{
	int io_poll_status;

	if (keyboard_interrupt_count > 0) {
		block_interrupts();
		--keyboard_interrupt_count;
		allow_interrupts();
		return (KEYBOARD_INTERRUPT_EVENT);
	}
	if (poll_time != -1 && current_time >= poll_time) {
		io_poll_status = queue_ready_ports(FALSE, 0, 0);
		if (io_poll_status == NO_ERRORS)
			poll_time = current_time + poll_interval;
		else if (io_poll_status != EINTR) {
			*status = io_poll_status;
			return (ERROR_EVENT);
		}
	}
	if (there_are_ready_ports()) {
		*ready_fd = next_ready_port();
		*status = 0;   /* chars read or written */
		return (IO_COMPLETION_EVENT);
	}
	if (alarm_time != -1 && current_time >= alarm_time) {
		alarm_time = -1;
		return (ALARM_EVENT);
	}
	block_interrupts();
	if ((keyboard_interrupt_count == 0)
	&&  (alarm_time == -1 || current_time < alarm_time)
	&&  (poll_time == -1 || current_time < poll_time))
		Spending_eventsPS = FALSE;
	allow_interrupts();
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

typedef struct	fd_struct {
	int			fd,		/* file descriptor */
				status;		/* one of the FD_* constants */
	bool			is_input;	/* iff input */
	struct fd_struct	*next;		/* next on same queue */
}	fd_struct;


/*
 * A queue of fd_structs is empty iff the first field is NULL.  In
 * that case, lastp points to first.
 */
typedef struct	fdque {
	fd_struct	*first,
			**lastp;
}	fdque;


static fd_struct	*fds[FD_SETSIZE];
static fdque		ready = {
				NULL,
				&ready.first
			},
			pending = {
				NULL,
				&pending.first
			};


static void		findrm(fd_struct *entry, fdque *que);
static fd_struct	*rmque(fd_struct **link, fdque *que);
static void		addque(fd_struct *entry, fdque *que);
static fd_struct	*add_fd(int fd, bool is_input);


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
}


static bool
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
 * Return TRUE if successful, and FALSE otherwise.
 */
bool
add_pending_fd(int fd, bool is_input)
{
	fd_struct	*data;

	if (! (0 <= fd && fd < FD_SETSIZE)) {
		fprintf(stderr, "ERROR: add_pending fd %d not in [0, %d)\n",
			fd,
			FD_SETSIZE);
		return (FALSE);
	}
	data = fds[fd];
	if (data == NULL) {
		data = add_fd(fd, is_input);
		if (data == NULL)
			return (FALSE);		/* no more memory */
	} else if (data->status == FD_PENDING)
		return (TRUE);			/* fd is already pending */
	else if (data->status == FD_READY)
		findrm(data, &ready);
	data->status = FD_PENDING;
	addque(data, &pending);
	if (poll_time == -1)
		poll_time = current_time + poll_interval;
	return TRUE;
}


/*
 * Add a new fd_struct for fd.
 */
static fd_struct	*
add_fd(int fd, bool is_input)
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


/*
 * Remove fd from any queues it is on.
 */
void
remove_fd(int fd)
{
	struct fd_struct	*data;

	if (! (0 <= fd && fd < FD_SETSIZE)) {
		fprintf(stderr, "ERROR: remove_fd fd %d not in [0, %d)\n",
			fd,
			FD_SETSIZE);
		return;
	}
	data = fds[fd];
	if (data == NULL)
		return;
	if (data->status == FD_PENDING) {
		findrm(data, &pending);
		if (pending.first == NULL)
			poll_time = -1;
	} else if (data->status == FD_READY)
		findrm(data, &ready);
	free((void *)data);
	fds[fd] = NULL;
}


int
wait_for_event(long max_wait, bool is_minutes)
{
	int	status;
	long	seconds,
		ticks;

	stop_alarm_interrupts();
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
		status = queue_ready_ports(TRUE, seconds, ticks);
		if (there_are_ready_ports())
			NOTE_EVENT;
	}
	start_alarm_interrupts();
	return (status);
}


/*
 * Call select() on the pending ports and move any ready ones to the ready
 * queue.  If wait is true, seconds is either -1 (wait forever) or the
 * maximum number of seconds to wait (with ticks any additional ticks).
 * The returned value is a status code.
 */
static int
queue_ready_ports(bool wait, long seconds, long ticks)
{
	fd_set		reads,
			writes,
			alls;
	int		limfd;
	fd_struct	*fdp,
			**fdpp;
	struct timeval	tv,
			*tvp;
	int		left;

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
	left = select(limfd, &reads, &writes, &alls, tvp);
	if (left <= 0)
		return (left == 0 ? NO_ERRORS : errno);
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
	return (NO_ERRORS);
}
