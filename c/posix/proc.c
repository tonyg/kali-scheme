/* Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Scheme 48/POSIX process environment interface
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "c-mods.h"
#include "scheme48.h"
#include "scheme48vm.h"
#include "event.h"
#include "posix.h"
#include "unix.h"

extern void		s48_init_posix_proc(void);
static s48_value	posix_fork(void),
			posix_exec(s48_value program, s48_value lookup_p,
				   s48_value env, s48_value args),
  			posix_enter_pid(s48_value pid),
  			posix_waitpid(void),
			posix_integer_to_signal(s48_value sig_int),
			posix_initialize_named_signals(void),
			posix_request_interrupts(s48_value int_number),  
			posix_cancel_interrupt_request(s48_value int_number),  
  			posix_kill(s48_value sch_pid, s48_value sch_signal);

static s48_value	enter_signal(int signal);
static int		extract_signal(s48_value sch_signal);
static void		signal_map_init();

static char		**enter_string_array(s48_value strings),
			*add_dot_slash(char *name);

/*
 * Two lists, one with all the child process ids and the other with all the
 * unnamed signals.  Each CAR is a weak pointer to the actual object.
 *
 * We also have a handy procedure for lookup up values in the lists.
 *
 * These are in C instead of Scheme to prevent them from being written out in
 * images.
 */

static s48_value child_pids = S48_NULL;
static s48_value unnamed_signals = S48_NULL;

static s48_value lookup_record(s48_value *list_loc, int offset, s48_value key);

/*
 * Record types imported from Scheme.
 */

static s48_value 	posix_process_id_type_binding = S48_FALSE;
static s48_value	posix_named_signal_type_binding = S48_FALSE;
static s48_value	posix_unnamed_signal_type_binding = S48_FALSE;

/*
 * Vector of Scheme signal objects imported from Scheme, and a marker that
 * is put in unnamed signals.
 */

static s48_value	posix_signals_vector_binding = S48_FALSE;
static s48_value	posix_unnamed_signal_marker_binding = S48_FALSE;

/*
 * Queue of received interrupts that need to be passed on to Scheme.
 * Kept as list with pointers to the first and last pairs.
 */

static s48_value	interrupt_head = S48_NULL;
static s48_value	interrupt_tail = S48_NULL;

/*
 * Install all exported functions in Scheme48.
 */
void
s48_init_posix_proc(void)
{
  S48_EXPORT_FUNCTION(posix_fork);
  S48_EXPORT_FUNCTION(posix_exec);
  S48_EXPORT_FUNCTION(posix_enter_pid);
  S48_EXPORT_FUNCTION(posix_waitpid);
  S48_EXPORT_FUNCTION(posix_integer_to_signal);
  S48_EXPORT_FUNCTION(posix_initialize_named_signals);
  S48_EXPORT_FUNCTION(posix_request_interrupts);
  S48_EXPORT_FUNCTION(posix_cancel_interrupt_request);
  S48_EXPORT_FUNCTION(posix_kill);

  S48_GC_PROTECT_GLOBAL(posix_process_id_type_binding);
  posix_process_id_type_binding =
    s48_get_imported_binding("posix-process-id-type");

  S48_GC_PROTECT_GLOBAL(posix_named_signal_type_binding);
  posix_named_signal_type_binding =
    s48_get_imported_binding("posix-named-signal-type");

  S48_GC_PROTECT_GLOBAL(posix_unnamed_signal_type_binding);
  posix_unnamed_signal_type_binding =
    s48_get_imported_binding("posix-unnamed-signal-type");

  S48_GC_PROTECT_GLOBAL(posix_signals_vector_binding);
  posix_signals_vector_binding =
    s48_get_imported_binding("posix-signals-vector");

  S48_GC_PROTECT_GLOBAL(posix_unnamed_signal_marker_binding);
  posix_unnamed_signal_marker_binding =
    s48_get_imported_binding("posix-unnamed-signal-marker");

  S48_GC_PROTECT_GLOBAL(child_pids);
  S48_GC_PROTECT_GLOBAL(unnamed_signals);
  S48_GC_PROTECT_GLOBAL(interrupt_head);
  S48_GC_PROTECT_GLOBAL(interrupt_tail);

  signal_map_init();
}

/*
 * Box a process id in a Scheme record.
 */

static s48_value
make_pid(pid_t c_pid)
{
  s48_value weak;
  s48_value sch_pid = s48_make_record(posix_process_id_type_binding);

  S48_UNSAFE_RECORD_SET(sch_pid, 0, s48_enter_fixnum(c_pid));
  S48_UNSAFE_RECORD_SET(sch_pid, 1, S48_FALSE);	/* return status */
  S48_UNSAFE_RECORD_SET(sch_pid, 2, S48_FALSE);	/* terminating signal */
  S48_UNSAFE_RECORD_SET(sch_pid, 3, S48_FALSE);	/* placeholder for waiting threads */

  weak = s48_make_weak_pointer(sch_pid);

  child_pids = s48_cons(weak, child_pids);
  
  return sch_pid;
}

/*
 * Lookup a pid in the list of same.  We clear out any dropped weak pointers
 * on the way.
 */

static s48_value
lookup_pid(pid_t c_pid)
{
  return lookup_record(&child_pids, 0, s48_enter_fixnum(c_pid));
}

/*
 * Lookup a record on a list of weak pointers to same.  We get a value and
 * the record offset at which to look for the value.  Any dropped pointers
 * are cleared out along the way.  If any have been seen we walk the entire
 * list to clear them all out.
 *
 * This is too much C code!  It should all be done in Scheme.
 */

static s48_value
lookup_record(s48_value *the_list_loc, int offset, s48_value key)
{
  bool		cleanup_p = FALSE;
  s48_value	the_list = *the_list_loc;
  s48_value	found = S48_FALSE;

  /* Clear out initial dropped weaks */
  while (the_list != S48_NULL &&
	 S48_UNSAFE_WEAK_POINTER_REF(S48_UNSAFE_CAR(the_list)) == S48_FALSE)
    the_list = S48_UNSAFE_CDR(the_list);

  if (the_list != *the_list_loc) {
    *the_list_loc = the_list;
    cleanup_p = TRUE; }

  if (the_list == S48_NULL)
    return S48_FALSE;			/* Nothing */

  else {

    s48_value first = S48_UNSAFE_WEAK_POINTER_REF(S48_UNSAFE_CAR(the_list));

    if (key == S48_UNSAFE_RECORD_REF(first, offset))
      /* Found it first thing. */
      found = first;
    else {
      /* Loop down. */
      s48_value prev = the_list;
      s48_value next = S48_UNSAFE_CDR(prev);
      for(; next != S48_NULL && found == S48_FALSE;
	  next = S48_UNSAFE_CDR(prev)) {
	s48_value first = S48_UNSAFE_WEAK_POINTER_REF(S48_UNSAFE_CAR(next));
	if (first == S48_FALSE) {
	  S48_UNSAFE_SET_CDR(prev, S48_UNSAFE_CDR(next));
	  cleanup_p = TRUE; }
	else if (key == S48_UNSAFE_RECORD_REF(first, offset))
	  found = first;
	else
	  prev = next; }
    
      /* If we found any empty weaks we check the entire list for them. */
    
      if (cleanup_p) {
	
	for(; next != S48_NULL; next = S48_UNSAFE_CDR(next)) {
	  s48_value first = S48_UNSAFE_WEAK_POINTER_REF(S48_UNSAFE_CAR(next));
	  if (first == S48_FALSE)
	    S48_UNSAFE_SET_CDR(prev, S48_UNSAFE_CDR(next)); } }
      
      return found; } }
}

/*
 * If we already have this process, return it, else make a new one.
 */

s48_value
s48_enter_pid(pid_t c_pid)
{
  s48_value sch_pid = lookup_pid(c_pid);
  return sch_pid == S48_FALSE ? make_pid(c_pid) : sch_pid;
}

/*
 * Version of above for calling from Scheme.
 */

static s48_value
posix_enter_pid(s48_value sch_pid)
{
  return s48_enter_pid(s48_extract_fixnum(sch_pid));
}

/*
 * Waiting for children.  We get finished pid's until we reach one for which
 * there is a Scheme pid record.  The exit status or terminating signal is
 * saved in the record which is then returned.
 *
 * This does not looked for stopped children, only terminated ones.
 */

static s48_value
posix_waitpid(void)
{
  while(1==1) {
    int stat;
    pid_t c_pid = waitpid(-1, &stat, WNOHANG);
    if (c_pid == -1) {
      if (errno == ECHILD)		/* no one left to wait for */
	return S48_FALSE;
      else if (errno != EINTR)
	s48_raise_os_error(errno);
    }
    else {
      s48_value sch_pid = lookup_pid(c_pid);
      S48_DECLARE_GC_PROTECT(1);

      S48_GC_PROTECT_1(sch_pid);

      if (sch_pid != S48_FALSE) {
	if (WIFEXITED(stat))
	  S48_UNSAFE_RECORD_SET(sch_pid, 1, s48_enter_fixnum(WEXITSTATUS(stat)));
	else
	  S48_UNSAFE_RECORD_SET(sch_pid, 2, enter_signal(WTERMSIG(stat)));
	S48_GC_UNPROTECT();
	return sch_pid;
      }
      else
	S48_GC_UNPROTECT();
    }
  }
}

/*
 * Fork and exec.
 */

static s48_value
posix_fork(void)
{
  pid_t child_pid = fork();

  if (child_pid < 0)
    s48_raise_os_error(errno);

  if (child_pid == 0)
    return S48_FALSE;
  else
    return make_pid(child_pid);
}

/*
 * The environment is an array of strings of the form "name=value", where
 * `name' cannot contain `='.
 *
 * It is a nuisance that given three binary choices (arguments explicit or
 * in a vector, path lookup or not, explicit or implicit environment) Posix
 * only gives six functions.  The two calls that have an explict environment
 * both do path lookup.  We work around this by adding `./' to the beginning
 * of the program, if it does not already contain a `/'.
 */

static s48_value
posix_exec(s48_value program, s48_value lookup_p,
	   s48_value env, s48_value args)
{
  char **c_args = enter_string_array(args);
  char *c_program, *real_c_program;
  int status;

  c_program = s48_extract_string(program);

  s48_stop_alarm_interrupts();

  if (env == S48_FALSE)
    if (lookup_p == S48_FALSE)
      status = execv(c_program, c_args);
    else {
      status = execvp(c_program, c_args);
    }
  else {
    char **c_env = enter_string_array(env);
    
    if (NULL == strchr(c_program, '/'))
      real_c_program = add_dot_slash(c_program);
    else
      real_c_program = c_program;

    status = execve(c_program, c_args, c_env);

    free(c_env);
    if (real_c_program != c_program)
      free(real_c_program); }

  /* If we get here, then something has gone wrong. */

  free(c_args);
  s48_start_alarm_interrupts();
  s48_raise_os_error(errno);

  /* appease gcc -Wall */
  return S48_FALSE;
}

/*
 * Convert a list of strings into an array of char pointers.
 */

static char **
enter_string_array(s48_value strings)
{
  int length = S48_UNSAFE_EXTRACT_FIXNUM(s48_length(strings));
  char **result = (char **)malloc((length + 1) * sizeof(char *));
  int i;

  if (result == NULL)
    s48_raise_out_of_memory_error();
  
  for(i = 0; i < length; i++, strings = S48_UNSAFE_CDR(strings)) {
    s48_value string = S48_UNSAFE_CAR(strings);
    if (! S48_STRING_P(string)) {
      free(result);
      s48_raise_argument_type_error(string); }
    result[i] = S48_UNSAFE_EXTRACT_STRING(string); }
  result[length] = NULL;

  return result;
}
  
/*
 * Add `./' to the beginning of `name'.
 */

static char *
add_dot_slash(char *name)
{
  int len = strlen(name);
  char *new_name = (char *)malloc((len + 1) * sizeof(char));
  
  if (new_name == NULL)
    s48_raise_out_of_memory_error();
  
  new_name[0] = '.';
  new_name[1] = '/';
  strcpy(new_name + 2, name);

  return new_name;
}

/*
 * Signals
 */

/*
 * These need to be replaced with something that really blocks interrupts.
 * I don't know what that should be.  This is needed in c/unix/events.c as
 * well.
 */

#define block_interrupts()
#define allow_interrupts()

/*
 * Adds `signum' to the queue of received signals.
 */

static void
queue_interrupt(int signum)
{
  s48_value		sch_signum = S48_UNSAFE_ENTER_FIXNUM(signum);
  s48_value		pair = s48_cons(sch_signum, S48_NULL);

  if (interrupt_head == S48_NULL)
    interrupt_head = pair;
  else
    S48_UNSAFE_SET_CDR(interrupt_tail, pair);
  
  interrupt_tail = pair;
}

/*
 * Returns TRUE if there is a signal to be delivered up to Scheme.
 */

int
s48_os_signal_pending(void)
{
  block_interrupts();

  if (interrupt_head == S48_NULL) {
    allow_interrupts();
    return FALSE; }
  else {
    s48_set_os_signal(S48_UNSAFE_CAR(interrupt_head),
		      S48_UNSAFE_ENTER_FIXNUM(0));
    interrupt_head = S48_UNSAFE_CDR(interrupt_head);
    if (interrupt_head == S48_NULL)
      interrupt_tail = S48_NULL;
    allow_interrupts();
    return TRUE; }
}

/*
 * Simple front for kill().  We have to retry if interrupted.
 */

s48_value
posix_kill(s48_value sch_pid, s48_value sch_signal)
{
  int status;

  s48_check_record_type(sch_pid, posix_process_id_type_binding);

  RETRY_OR_RAISE_NEG(status,
		     kill(s48_extract_fixnum(S48_UNSAFE_RECORD_REF(sch_pid, 0)),
			  extract_signal(sch_signal)));

  return S48_UNSPECIFIC;
}

/*
 * This is an array that maps our `canonical' signal numbers to the local
 * OS's numbers.  The initialization is done via an include file written
 * by a Scheme program.  The include file first calls signal_count_is()
 * with the number of named signals and then adds the named signals supported
 * by the current os to `signal_map'.
 */

static int	*signal_map, signal_map_size;

static void
signal_count_is(int count)
{  
  int i;

  signal_map_size = count;
  signal_map = (int *) malloc(count * sizeof(int));

  if (signal_map == NULL) {
    fprintf(stderr, "ran out of memory during initialization\n");
    exit(1); }

  for (i = 0; i < count; i++)
    signal_map[i] = -1;
}
    
static void
signal_map_init()
{
#include "s48_signals.h"
}

/*
 * Converts from an OS signal to a canonical signal number.
 * We return -1 if there is no matching named signal.
 */

static int
lookup_signal(int c_signal) {
  int i = 0;

  for (i = 0; i < signal_map_size; i++)
    if (signal_map[i] == c_signal)
      return i;

  return -1;
}

/*
 * Use the signal map to set the os-number slot in each named signal to
 * its value in the current OS.
 */

static s48_value
posix_initialize_named_signals(void)
{
  int i, length;
  s48_value named_signals;

  S48_SHARED_BINDING_CHECK(posix_signals_vector_binding);

  named_signals = S48_SHARED_BINDING_REF(posix_signals_vector_binding);

  if(! S48_VECTOR_P(named_signals))
    s48_raise_argument_type_error(named_signals);
    
  length = S48_UNSAFE_VECTOR_LENGTH(named_signals);

  for(i = 0; i < length; i++) {
    s48_value signal = S48_UNSAFE_VECTOR_REF(named_signals, i);
    int canonical = s48_extract_fixnum(S48_UNSAFE_RECORD_REF(signal, 1));
    int c_signal = signal_map[canonical];
    s48_value scm_signal = (c_signal == -1) ?
                           S48_FALSE :
                           s48_enter_fixnum(c_signal);
    
    S48_UNSAFE_RECORD_SET(signal, 2, scm_signal); }

  return S48_UNSPECIFIC;
}

/*
 * Make a new unnamed signal containing `fx_signal' and add it to the weak
 * list of unnamed signals.
 */

static s48_value
make_unnamed_signal(s48_value fx_signal)
{
  s48_value weak;
  s48_value unnamed = s48_make_record(posix_unnamed_signal_type_binding);

  S48_UNSAFE_RECORD_SET(unnamed,
			0,
			S48_UNSAFE_SHARED_BINDING_REF(
			      posix_unnamed_signal_marker_binding));
  S48_UNSAFE_RECORD_SET(unnamed, 1, fx_signal);
  S48_UNSAFE_RECORD_SET(unnamed, 2, S48_NULL);		/* No queues */

  weak = s48_make_weak_pointer(unnamed);

  unnamed_signals = s48_cons(weak, unnamed_signals);
  
  return unnamed;
}

/*
 * Returns a signal record for `signal'.  Unnamed signals are looked up in
 * the weak list of same; if none is found we make one.  Scheme records for
 * named signals are retrieved from a vector sent down by the Scheme code.
 */

static s48_value
enter_signal(int c_signal)
{
  int canonical = lookup_signal(c_signal);

  if (canonical == -1) {
    s48_value fx_signal = s48_enter_fixnum(c_signal);
    s48_value unnamed = lookup_record(&unnamed_signals, 1, fx_signal);
    
    if (unnamed != S48_FALSE)
      return unnamed;
    else
      return make_unnamed_signal(fx_signal); }
  else
    return S48_VECTOR_REF(S48_SHARED_BINDING_REF(posix_signals_vector_binding),
			  canonical);
}

/*
 * Wrapper for enter_signal() for calling from Scheme.
 */

static s48_value
posix_integer_to_signal(s48_value signal_int)
{
  if (S48_FIXNUM_P(signal_int))
    return enter_signal(s48_extract_fixnum(signal_int));
  else
    /* really should do an integer? test here */
    return S48_FALSE;
}

/*
 * Go from a signal back to the local integer.  For named signals we extract
 * the canonical signal to use as an index into the signal map.  Unnamed signals
 * contain the local signal already.
 */

static int
extract_signal(s48_value sch_signal)
{
  s48_value type;

  if (! S48_RECORD_P(sch_signal))
    s48_raise_argument_type_error(sch_signal);

  type = S48_UNSAFE_RECORD_TYPE(sch_signal);

  if (type == S48_UNSAFE_SHARED_BINDING_REF(posix_named_signal_type_binding)) {
    int canonical = s48_extract_fixnum(S48_UNSAFE_RECORD_REF(sch_signal, 1));
    if ((0 <= canonical < signal_map_size)
	&& signal_map[canonical] != -1)
      return signal_map[canonical];
    else
      s48_raise_argument_type_error(sch_signal); }

  else if (type ==
	   S48_UNSAFE_SHARED_BINDING_REF(posix_unnamed_signal_type_binding))
    return s48_extract_fixnum(S48_UNSAFE_RECORD_REF(sch_signal, 1));

  else
    s48_raise_argument_type_error(sch_signal);
}

/*
 * Queue the interrupt.  For SIGINT and SIGALRM we call the event-system's
 * handler as well.
 */

static void
generic_interrupt_catcher(int signum)
{
  queue_interrupt(signum);

  switch (signum) {
  case SIGINT: {
    extern void		s48_when_keyboard_interrupt(int ign);
    s48_when_keyboard_interrupt(0);
    break; }
  case SIGALRM: {
    extern void		s48_when_alarm_interrupt(int ign);
    s48_when_alarm_interrupt(0);
    break; }
  default:
    NOTE_EVENT; }
  
  return;
}

/*
 * Array of actions to be restored when we no longer listen for a signal.
 */

#define MAX_SIGNAL 1023			/* Just a guess. */

struct sigaction *saved_actions[MAX_SIGNAL + 1] = {NULL};

/*
 * If there is a saved action then our handler is already in place and
 * we need do nothing.  Otherwise we save the current action and install
 * our own.
 */

s48_value
posix_request_interrupts(s48_value sch_signum)
{
  int			signum = s48_extract_fixnum(sch_signum);
  struct sigaction	sa;

  if (saved_actions[signum] == NULL) {
    struct sigaction *	old = (struct sigaction *)
                                malloc(sizeof(struct sigaction));
    
    if (old == NULL)
      s48_raise_out_of_memory_error();

    sa.sa_handler = (void(*)) generic_interrupt_catcher;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(signum, &sa, old) != 0) {
      free(old);
      s48_raise_os_error(errno); }

    saved_actions[signum] = old; }
    
  return S48_UNSPECIFIC;
}

/*
 * The reverse of the above.  If there is a saved action then we install it
 * and remove it from the saved_action array.
 */

s48_value
posix_cancel_interrupt_request(s48_value sch_signum)
{
  int			signum = s48_extract_fixnum(sch_signum);
  struct sigaction *	old = saved_actions[signum];

  if (old != NULL) {
    
    if (sigaction(signum, old, (struct sigaction *) NULL) != 0)
      s48_raise_os_error(errno);
    
    free(old);
    saved_actions[signum] = NULL; }
    
  return S48_UNSPECIFIC;
}


