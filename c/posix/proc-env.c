/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * Scheme 48/POSIX process environment interface
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>

#include "scheme48.h"
#include "posix.h"
#include "unix.h"
#include "sysdep.h"

extern void		s48_init_posix_proc_env(void);
static s48_value	posix_get_pid(s48_value parent_p),
			posix_get_id(s48_value user_p, s48_value real_p),
			posix_set_id(s48_value user_p, s48_value id),
			posix_get_groups(void),
			posix_get_login(void),
			posix_set_sid(void),
			posix_sys_name(s48_value which),
			posix_get_env(s48_value name),
			posix_get_env_alist(void),
  			posix_get_terminal_pathname(void),
			posix_tty_name(s48_value channel),
			posix_is_a_tty(s48_value channel);

/*
 * Install all exported functions in Scheme48.
 */
void
s48_init_posix_proc_env(void)
{
  S48_EXPORT_FUNCTION(posix_get_pid);
  S48_EXPORT_FUNCTION(posix_get_id);
  S48_EXPORT_FUNCTION(posix_set_id);
  S48_EXPORT_FUNCTION(posix_get_groups);
  S48_EXPORT_FUNCTION(posix_get_login);
  S48_EXPORT_FUNCTION(posix_set_sid);
  S48_EXPORT_FUNCTION(posix_sys_name);
  S48_EXPORT_FUNCTION(posix_get_env);
  S48_EXPORT_FUNCTION(posix_get_env_alist);
  S48_EXPORT_FUNCTION(posix_get_terminal_pathname);
  S48_EXPORT_FUNCTION(posix_tty_name);
  S48_EXPORT_FUNCTION(posix_is_a_tty);
}

/*
 * Lots of simple little functions.
 */

static s48_value
posix_get_pid(s48_value parent_p)
{
  extern char going;
  going = 1 == 0;
  return s48_enter_pid(S48_EXTRACT_BOOLEAN(parent_p) ?
		       getppid() :
		       getpid());
}

static s48_value
posix_set_sid()
{
  pid_t pid;

  RETRY_OR_RAISE_NEG(pid, setsid());

  return s48_enter_pid(pid);
}

static s48_value
posix_get_id(s48_value user_p, s48_value real_p)
{
  if (S48_EXTRACT_BOOLEAN(user_p))
    return s48_enter_uid(S48_EXTRACT_BOOLEAN(real_p) ? getuid() : geteuid());
  else
    return s48_enter_gid(S48_EXTRACT_BOOLEAN(real_p) ? getgid() : getegid());
}

static s48_value
posix_set_id(s48_value user_p, s48_value id)
{
  int status;

  if (S48_EXTRACT_BOOLEAN(user_p))
    RETRY_OR_RAISE_NEG(status, setuid(s48_extract_uid(id)));
  else
    RETRY_OR_RAISE_NEG(status, setgid(s48_extract_gid(id)));

  return S48_UNSPECIFIC;
}

static s48_value
posix_get_login(void)
{
  char *login = getlogin();

  return (login == NULL) ? S48_FALSE : s48_enter_byte_string(login);
}

static s48_value
posix_get_env(s48_value name)
{
  char *value;

  value = getenv(s48_extract_byte_vector(name));

  return (value == NULL) ? S48_FALSE : s48_enter_byte_string(value);
}

/*
 * Here we turn an array of strings of the form "name=value" into a list
 * of pairs ("name" . "value").
 */

static s48_value
posix_get_env_alist(void)
{
  extern char **ENVIRON_NAME;
  char **c_env = ENVIRON_NAME;
  s48_value sch_env = S48_NULL;
  s48_value name = S48_FALSE;
  S48_DECLARE_GC_PROTECT(2);
  
  S48_GC_PROTECT_2(sch_env, name);

  for(; *c_env != NULL; c_env++) {
    char *entry = *c_env;
    s48_value value;
    char *name_end = strchr(entry, '=');

    name = s48_enter_byte_substring(entry, name_end - entry);
    value = s48_enter_byte_substring(name_end + 1, strlen(name_end + 1));
    sch_env = s48_cons(s48_cons(name, value), sch_env); }
   
  S48_GC_UNPROTECT();

  return sch_env;
}

/*
 * Again we turn an array into a list.
 */

static s48_value
posix_get_groups(void) 
{
  int status, count, i;
  gid_t *grouplist;
  s48_value groups = S48_NULL;
  s48_value temp = S48_UNSPECIFIC;
  S48_DECLARE_GC_PROTECT(2);
  
  S48_GC_PROTECT_2(groups, temp);
  
  count = getgroups(0, (gid_t *)NULL);

  grouplist = (gid_t *) malloc(count * sizeof(gid_t));

  if (grouplist == NULL)
    s48_raise_out_of_memory_error();

  RETRY_NEG(status, getgroups(count, grouplist));

  if (status == -1) {
    free(grouplist);
    s48_raise_os_error(errno); }

  for(i = count - 1; i > -1; i--) {
    temp = s48_enter_gid(grouplist[i]);
    groups = s48_cons(temp, groups);
  }
    
  S48_GC_UNPROTECT();

  free(grouplist);

  return groups;
}


/*
 * uname() - we could define a record for this, but it seems like overkill.
 */

static s48_value
posix_sys_name(s48_value which)
{
  struct utsname names;
  char *value;
  int status;

  RETRY_OR_RAISE_NEG(status, uname(&names));

  switch (s48_extract_fixnum(which)) {
  case 0: value = names.sysname; break;
  case 1: value = names.nodename; break;
  case 2: value = names.release; break;
  case 3: value = names.version; break;
  default: value = names.machine;
  }

  return s48_enter_string_latin_1(value);
}

/*
 * Terminals
 */

static s48_value
posix_get_terminal_pathname(void)
{
  char termid[L_ctermid];
  char *status =  ctermid(termid);
  
  return (*status == '\0') ? S48_FALSE : s48_enter_byte_string(termid);
}

static s48_value
posix_tty_name(s48_value channel)
{
  char *name;

  name = ttyname(S48_UNSAFE_EXTRACT_FIXNUM(S48_CHANNEL_OS_INDEX(channel)));

  return (name == NULL) ? S48_FALSE : s48_enter_byte_string(name);
}

static s48_value
posix_is_a_tty(s48_value channel)
{
  return S48_ENTER_BOOLEAN(isatty(
			    S48_UNSAFE_EXTRACT_FIXNUM(
				  S48_CHANNEL_OS_INDEX(channel))));
}

