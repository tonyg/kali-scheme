enum event_enum { KEYBOARD_INTERRUPT_EVENT, IO_COMPLETION_EVENT, ALARM_EVENT,
		    ERROR_EVENT, NO_EVENT };
extern int get_next_event(long *ready_fd, long *status);

extern bool add_pending_fd(int fd, bool is_input);
extern void remove_fd(int fd);
extern long schedule_alarm_interrupt(long delta);
extern long run_time(long *mseconds);
extern long real_time(long *mseconds);
extern int wait_for_event(long max_wait, bool is_minutes);


/* these are here only for the CHEAP_TIME() macro */
#define TICKS_PER_SECOND 1000	/* clock resolution */
#define POLLS_PER_SECOND   20   /* how often we poll */
#define TICKS_PER_POLL  (TICKS_PER_SECOND / POLLS_PER_SECOND)

extern long current_time;
#define CHEAP_TIME()  (current_time * TICKS_PER_POLL)
