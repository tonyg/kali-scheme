/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * An interface to Unix opendir(), readdir(), closedir(),
 * stat() and lstat().
 * Note, readdir() returns #F on EOF.
 * Note, calling closedir on an already closed directory has no effect.
 * Note, readdir will never return "." or ".." (POSIX leaves this
 * unspecified).
 * Note, a stat object which is written out to a dump and used on a
 * different OS will cause problems because things have moved around.
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "scheme48.h"
#include "scheme48vm.h"		/* ps_close_fd() */
#include "posix.h"
#include "c-mods.h"
#include "unix.h"
#include "fd-io.h"

extern void		s48_init_posix_dir(void);

static s48_value	posix_opendir(s48_value svname),
			posix_closedir(s48_value svdir),
			posix_readdir(s48_value svdir),
			posix_working_directory(s48_value new_wd),
			posix_open(s48_value path, s48_value options,
				   s48_value mode),
			posix_file_stuff(s48_value op, s48_value arg1,
					 s48_value arg2),
			posix_file_info(s48_value svname,
					s48_value follow_link_p,
					s48_value mode_enum),
			posix_ctime(s48_value sch_time),
			posix_time(void);
/*
 * Record types imported from Scheme.
 */
static s48_value	posix_time_type_binding = S48_FALSE,
			posix_file_info_type_binding = S48_FALSE,
			posix_file_mode_type_binding = S48_FALSE,
			posix_user_id_type_binding = S48_FALSE;


/*
 * Forward declarations.
 */

static s48_value enter_mode(mode_t mode);

/*
 * Install all exported functions in Scheme48.
 */
void
s48_init_posix_dir(void)
{
  S48_EXPORT_FUNCTION(posix_opendir);
  S48_EXPORT_FUNCTION(posix_readdir);
  S48_EXPORT_FUNCTION(posix_closedir);

  S48_EXPORT_FUNCTION(posix_working_directory);

  S48_EXPORT_FUNCTION(posix_open);
  S48_EXPORT_FUNCTION(posix_file_stuff);

  S48_EXPORT_FUNCTION(posix_ctime);
  S48_EXPORT_FUNCTION(posix_time);

  S48_EXPORT_FUNCTION(posix_file_info);

  S48_GC_PROTECT_GLOBAL(posix_time_type_binding);
  posix_time_type_binding = s48_get_imported_binding("posix-time-type");
    
  S48_GC_PROTECT_GLOBAL(posix_user_id_type_binding);
  posix_user_id_type_binding = s48_get_imported_binding("posix-user-id-type");
    
  S48_GC_PROTECT_GLOBAL(posix_file_info_type_binding);
  posix_file_info_type_binding = s48_get_imported_binding("posix-file-info-type");
    
  S48_GC_PROTECT_GLOBAL(posix_file_mode_type_binding);
  posix_file_mode_type_binding = s48_get_imported_binding("posix-file-mode-type");
}

/*
 * Interface to opendir.
 */
static s48_value
posix_opendir(s48_value svname)
{
  DIR		*dp;
  s48_value	res;
  char		*c_name;

  c_name = s48_extract_byte_vector(svname);
  RETRY_OR_RAISE_NULL(dp, opendir(c_name));
  res = S48_MAKE_VALUE(DIR *);
  S48_UNSAFE_EXTRACT_VALUE(res, DIR *) = dp;
  return (res);
}

/*
 * Interface to closedir.
 * Note, it is ok to call closedir on an already closed directory.
 */
static s48_value
posix_closedir(s48_value svdir)
{
  DIR	**dpp;

  dpp = S48_EXTRACT_VALUE_POINTER(svdir, DIR *);
  if (*dpp != (DIR *)NULL) {
    int		status;
    RETRY_OR_RAISE_NEG(status, closedir(*dpp));
    *dpp = (DIR *)NULL;
  }
  return (S48_UNSPECIFIC);
}

/*
 * Interface to readdir.
 * If we have already read all the files that are in the directory,
 * #F is returned.  Otherwise, a string with the next file name.
 * Note, "." and ".." are never returned.
 */
static s48_value
posix_readdir(s48_value svdir)
{
  DIR		**dpp;
  struct dirent	*dep;
  char		*name;

  dpp = S48_EXTRACT_VALUE_POINTER(svdir, DIR *);
  if (*dpp == (DIR *)NULL)
    s48_raise_argument_type_error(svdir);	/* not really correct error */
  do {
    errno = 0;
    RETRY_NULL(dep, readdir(*dpp));
    if (dep == (struct dirent *)NULL) {
      if (errno != 0)
	s48_raise_os_error(errno);
      return (S48_FALSE);
    }
    name = dep->d_name;
  } while ((name[0] == '.')
	   && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0')));
  return s48_enter_byte_string(name);
}

/* ************************************************************ */
/*
 * Working directory.
 *
 * If the name is a string, we set the working directory to it.  If the name
 * is #f we return the current working directory.  This would be trivial,
 * except that we don't know how big a buffer we need for the path.  So we
 * keep trying until we run out of memory.
 */

int going = 0;
int second = 0;

static s48_value
posix_working_directory(s48_value new_wd)
{
  if (second)
    going = 1;
  else
    second = 1;

  if (new_wd == S48_FALSE) {
    char	*status;
    char	stack_buffer[256];
    char	*buffer = stack_buffer;
    int		buffer_size = 256;
    
    while (0==0) {
      RETRY_NULL(status, getcwd(buffer, buffer_size));

      if (status == buffer) {
	s48_value	result = s48_enter_byte_string(buffer);
	if (buffer != stack_buffer)
	  free(buffer);
	return result;
      }
      else if (errno == ERANGE) {
	if (buffer != stack_buffer)
	  free(buffer);
	buffer_size *= 2;
	buffer = (char *) malloc(buffer_size * sizeof(char));
	if (buffer == NULL)
	  s48_raise_out_of_memory_error();
      }
      else
	s48_raise_os_error(errno);
    }
  }
  else {
    int		status;

    RETRY_OR_RAISE_NEG(status, chdir(s48_extract_byte_vector(new_wd)));
    
    return S48_UNSPECIFIC;
  }
}

/* ************************************************************ */
/*
 * Open() and friends.
 *
 */

static s48_value
posix_open(s48_value path, s48_value options, s48_value mode)
{
  int		fd,
    		c_options;
  char		*c_path;
  s48_value	channel;
  S48_DECLARE_GC_PROTECT(1);

  S48_GC_PROTECT_1(path);

  c_options = s48_extract_file_options(options);
  c_path = s48_extract_byte_vector(path);

  if ((O_WRONLY & c_options) || (O_RDWR & c_options))
    c_options |= O_NONBLOCK;

  if (mode == S48_FALSE)
    RETRY_OR_RAISE_NEG(fd, open(c_path, c_options));
  else {
    mode_t	c_mode = s48_extract_mode(mode);
    RETRY_OR_RAISE_NEG(fd, open(c_path, c_options, c_mode));
  }

  channel = s48_add_channel(O_RDONLY & c_options
			      ? S48_CHANNEL_STATUS_INPUT
			      : S48_CHANNEL_STATUS_OUTPUT,
			    path,
			    fd);

  if (!S48_CHANNEL_P(channel)) {
    ps_close_fd(fd);		/* retries if interrupted */
    s48_raise_scheme_exception(s48_extract_fixnum(channel), 0); };

  S48_GC_UNPROTECT();

  return channel;
}

/*
 * A bunch of simple procedures merged together to save typing.
 */

static s48_value
posix_file_stuff(s48_value op, s48_value arg0, s48_value arg1)
{
  int status;

  switch (s48_extract_fixnum(op)) {

    /* umask(new_mask) */
  case 0:
    return enter_mode(umask(s48_extract_mode(arg0)));

    /* link(existing, new) */
  case 1:
    RETRY_OR_RAISE_NEG(status, link(s48_extract_byte_vector(arg0),
				    s48_extract_byte_vector(arg1)));
    break;

    /* mkdir(path, mode) */
  case 2:
    RETRY_OR_RAISE_NEG(status, mkdir(s48_extract_byte_vector(arg0),
				     s48_extract_mode(arg1)));
    break;

    /* mkfifo(path, mode) */
  case 3:
    RETRY_OR_RAISE_NEG(status, mkfifo(s48_extract_byte_vector(arg0),
				      s48_extract_mode(arg1)));
    break;

    /* unlink(char *path) */
  case 4:
    RETRY_OR_RAISE_NEG(status, unlink(s48_extract_byte_vector(arg0)));
    break;
    
    /* rmdir(char *path) */
  case 5:
    RETRY_OR_RAISE_NEG(status, rmdir(s48_extract_byte_vector(arg0)));
    break;
    
    /* rename(char *old, char *new) */
  case 6:
    RETRY_OR_RAISE_NEG(status, rename(s48_extract_byte_vector(arg0),
				      s48_extract_byte_vector(arg1)));
    break;
    
    /* access(char *path, int modes) */
  case 7: {
    int modes = s48_extract_fixnum(arg1);
    int local_modes = (001 & modes ? R_OK : 0) |
                      (002 & modes ? W_OK : 0) |
                      (004 & modes ? X_OK : 0) |
                      (010 & modes ? F_OK : 0);
    char *path = s48_extract_byte_vector(arg0);

    RETRY_NEG(status, access(path, local_modes));

    if (status == 0)
      return S48_TRUE;
    else
      switch (errno){
      case EACCES:	/* access would be denied or search permission denied */
      case EROFS:	/* want write access to a read-only filesystem */
      case ENOENT:	/* no entry for a directory component */
      case ENOTDIR:	/* using a non-directory as a directory */
      case ELOOP:	/* too many symbolic links */
	return S48_FALSE;
      default:		/* all other errors are (supposed to be) real errors */
	s48_raise_os_error(errno); }
  }
  default:
    /* appease gcc -Wall */
    s48_raise_range_error(op,
			  S48_UNSAFE_ENTER_FIXNUM(0),
			  S48_UNSAFE_ENTER_FIXNUM(6));
  }
  return S48_UNSPECIFIC;
}

/* ************************************************************ */
/*
 * Convert a time_t into a Scheme time record.
 */
static s48_value
enter_time(time_t time)
{
  s48_value	sch_time = S48_UNSPECIFIC;
  s48_value	temp = S48_UNSPECIFIC;
  S48_DECLARE_GC_PROTECT(2);

  S48_GC_PROTECT_2(sch_time, temp);

  sch_time = s48_make_record(posix_time_type_binding);

  /* Stashing the time value into temp before handing tit off to
     S48_UNSAFE_RECORD_SET is necessary because its evaluation may
     cause GC; that GC could destroy the temporary holding the value
     of sch_time. */

  temp = s48_enter_integer(time);
  S48_UNSAFE_RECORD_SET(sch_time, 0, temp);

  S48_GC_UNPROTECT();

  return sch_time;
}

/*
 * Convert a Scheme time record into a time_t.
 */
static time_t
extract_time(s48_value time)
{
  s48_check_record_type(time, posix_time_type_binding);

  return s48_extract_integer(S48_UNSAFE_RECORD_REF(time, 0));
}

/*
 * The posix ctime() procedure, which converts a time_t into a string, using
 * the local time zone.
 *
 * ENTER_STRING does a copy, which gets us out of ctime()'s static buffer.
 */
static s48_value
posix_ctime(s48_value sch_time)
{
  time_t	time;

  s48_check_record_type(sch_time, posix_time_type_binding);
  time = extract_time(sch_time);
  return s48_enter_string_latin_1(ctime(&time));
}

static s48_value
posix_time()
{
  time_t the_time, status;

  RETRY_OR_RAISE_NEG(status, time(&the_time));
    
  return enter_time(the_time);
}

/* ************************************************************ */
/* File modes.
 *
 * We translate the local bits into our own bits and vice versa.
 */

#define S48_ISUID 004000
#define S48_ISGID 002000
#define S48_ISVTX 001000     /* sticky bit, apparently not POSIX */
#define S48_IRUSR  00400
#define S48_IWUSR  00200
#define S48_IXUSR  00100
#define S48_IRGRP  00040
#define S48_IWGRP  00020
#define S48_IXGRP  00010
#define S48_IROTH  00004
#define S48_IWOTH  00002
#define S48_IXOTH  00001

s48_value
enter_mode(mode_t mode)
{
  s48_value	sch_mode;
  mode_t        my_mode;

  my_mode =
    (S_ISUID & mode ? S48_ISUID : 0) |
    (S_ISGID & mode ? S48_ISGID : 0) |
    (S_ISVTX & mode ? S48_ISVTX : 0) |
    (S_IRUSR & mode ? S48_IRUSR : 0) |
    (S_IWUSR & mode ? S48_IWUSR : 0) |
    (S_IXUSR & mode ? S48_IXUSR : 0) |
    (S_IRGRP & mode ? S48_IRGRP : 0) |
    (S_IWGRP & mode ? S48_IWGRP : 0) |
    (S_IXGRP & mode ? S48_IXGRP : 0) |
    (S_IROTH & mode ? S48_IROTH : 0) |
    (S_IWOTH & mode ? S48_IWOTH : 0) |
    (S_IXOTH & mode ? S48_IXOTH : 0);

  sch_mode = s48_make_record(posix_file_mode_type_binding);
  S48_UNSAFE_RECORD_SET(sch_mode, 0, s48_enter_fixnum(my_mode));

  return sch_mode;
}

mode_t
s48_extract_mode(s48_value sch_mode)
{
  mode_t        c_mode;
  long		mode;

  s48_check_record_type(sch_mode, posix_file_mode_type_binding);

  mode = s48_extract_fixnum(S48_UNSAFE_RECORD_REF(sch_mode, 0));

  c_mode =
    (S48_ISUID & mode ? S_ISUID : 0) |
    (S48_ISGID & mode ? S_ISGID : 0) |
    (S48_ISVTX & mode ? S_ISVTX : 0) |
    (S48_IRUSR & mode ? S_IRUSR : 0) |
    (S48_IWUSR & mode ? S_IWUSR : 0) |
    (S48_IXUSR & mode ? S_IXUSR : 0) |
    (S48_IRGRP & mode ? S_IRGRP : 0) |
    (S48_IWGRP & mode ? S_IWGRP : 0) |
    (S48_IXGRP & mode ? S_IXGRP : 0) |
    (S48_IROTH & mode ? S_IROTH : 0) |
    (S48_IWOTH & mode ? S_IWOTH : 0) |
    (S48_IXOTH & mode ? S_IXOTH : 0);

  return c_mode;
}

/* ************************************************************ */
/*
 * Interface to stat(), fstat(), and lstat().
 */

static s48_value
posix_file_info(s48_value svname,
		s48_value follow_link_p,
		s48_value mode_enum)
{
  struct stat	sbuf;
  int		status;
  s48_value	scm_mode = S48_FALSE;
  s48_value	info = S48_FALSE;
  s48_value	temp = S48_UNSPECIFIC;
  S48_DECLARE_GC_PROTECT(5);

  S48_GC_PROTECT_5(svname, mode_enum, scm_mode, info, temp);

  if (S48_CHANNEL_P(svname)) {
    RETRY_OR_RAISE_NEG(status,
		       fstat(S48_UNSAFE_EXTRACT_FIXNUM(
				     S48_UNSAFE_CHANNEL_OS_INDEX(svname)),
			     &sbuf));
    svname = S48_UNSAFE_CHANNEL_ID(svname); }
  else if (follow_link_p == S48_FALSE)
    RETRY_OR_RAISE_NEG(status, stat(s48_extract_byte_vector(svname), &sbuf));
  else
    RETRY_OR_RAISE_NEG(status, lstat(s48_extract_byte_vector(svname), &sbuf));

  info = s48_make_record(posix_file_info_type_binding);

  scm_mode = S48_VECTOR_REF(mode_enum,
			    S_ISREG(sbuf.st_mode)  ? 0 :
			    S_ISDIR(sbuf.st_mode)  ? 1 :
			    S_ISCHR(sbuf.st_mode)  ? 2 :
			    S_ISBLK(sbuf.st_mode)  ? 3 :
			    S_ISFIFO(sbuf.st_mode) ? 4 :
			    /* next two are not POSIX */
			    S_ISLNK(sbuf.st_mode)  ? 5 :
			    S_ISSOCK(sbuf.st_mode) ? 6 :
			    7);

  /* Stashing the various field values into temp before handing them
     off to S48_UNSAFE_RECORD_SET is necessary because their
     evaluation may cause GC; that GC could destroy the temporary
     holding the value of info. */

  S48_UNSAFE_RECORD_SET(info, 0, svname);
  S48_UNSAFE_RECORD_SET(info, 1, scm_mode);
  temp = s48_enter_integer(sbuf.st_dev);
  S48_UNSAFE_RECORD_SET(info, 2, temp);
  temp = s48_enter_integer(sbuf.st_ino);
  S48_UNSAFE_RECORD_SET(info, 3, temp);
  temp = enter_mode(sbuf.st_mode);
  S48_UNSAFE_RECORD_SET(info, 4, temp);
  temp = s48_enter_integer(sbuf.st_nlink);
  S48_UNSAFE_RECORD_SET(info, 5, temp);
  temp = s48_enter_uid(sbuf.st_uid);
  S48_UNSAFE_RECORD_SET(info, 6, temp);
  temp = s48_enter_gid(sbuf.st_gid);
  S48_UNSAFE_RECORD_SET(info, 7, temp);
  temp = s48_enter_integer(sbuf.st_size);
  S48_UNSAFE_RECORD_SET(info, 8, temp);
  temp = enter_time(sbuf.st_atime);
  S48_UNSAFE_RECORD_SET(info, 9, temp);
  temp = enter_time(sbuf.st_mtime);
  S48_UNSAFE_RECORD_SET(info, 10, temp);
  temp = enter_time(sbuf.st_ctime);
  S48_UNSAFE_RECORD_SET(info, 11, temp);

  S48_GC_UNPROTECT();

  return info;
}
