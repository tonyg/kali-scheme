/* Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#define _WIN32_WINNT 0x0400 /* for QueueUserAPC */

#include <windows.h>

#include <stdio.h>
#include <errno.h>              /* for errno, (POSIX?/ANSI) */
#include "scheme48vm.h"
#include "event.h"

enum stream_descriptor_type {
  STREAM_FILE_REGULAR, /* overlapped I/O works */
	   
  STREAM_FILE_SPECIAL, /* overlapped I/O doesn't work */
  STREAM_SOCKET
};

typedef struct
{
  HANDLE thread_handle; /* parameter */
  HANDLE file_handle; /* parameter */
  HANDLE check_semaphore; /* parameter */

  /* for successive calls in the main thread;
     only set and read from there */
  bool checking; 

  /* in */
  bool abort;
  char* buffer;
  size_t requested;

  PAPCFUNC callback;
  HANDLE callback_thread;

  /* out */
  size_t available; /* bytes written, readers only */
  bool eof; /* readers only */
  bool error;
  DWORD error_code;
} file_thread_data_t;

typedef struct {
  /* so we can pass this as LPOVERLAPPED; must be at the beginning */
  OVERLAPPED overlap;
  long fd;
  CHAR* buffer;
  CHAR* current;
  size_t current_size, max_size;
} callback_data_t;

typedef struct {
  enum stream_descriptor_type type;

  bool is_free;

  union {

    struct {
      HANDLE handle;
      DWORD current_offset;
      DWORD current_offset_high;
    } file_regular_data;

    struct {
      file_thread_data_t thread_data;
    } file_special_data;
  
  };

  callback_data_t callback_data;

} stream_descriptor_t;

/* This is the same as DEFAULT-BUFFER-SIZE in rts/port-buffer.scm */
#define FILE_CALLBACK_BUFFER_INITIAL_SIZE 4096

/* If things work smoothly, this will never have to do anything. */

static void
maybe_grow_callback_data_buffer(callback_data_t* callback_data,
				size_t new_max_size)
{
  if (new_max_size <= callback_data->max_size)
    return;

  callback_data->buffer = realloc(callback_data->buffer, new_max_size);
  
  if (callback_data->buffer == NULL)
    {
      fprintf(stderr,
	      "failed to allocate memory for stream buffer\n");
      exit(1);
    }

  callback_data->max_size = new_max_size;
}


#define STREAM_DESCRIPTORS_INITIAL_SIZE 1024

stream_descriptor_t* stream_descriptors;
size_t stream_descriptors_max_size;

static void
initialize_stream_descriptors(void)
{
  stream_descriptors = malloc(sizeof(stream_descriptor_t) *
			      STREAM_DESCRIPTORS_INITIAL_SIZE);
  if (stream_descriptors == NULL)
    {
      fprintf(stderr,
	      "failed to allocate memory for stream descriptors\n");
      exit(1);
    }
  
  {
    int i = 0;

    while (i < STREAM_DESCRIPTORS_INITIAL_SIZE)
      {
	stream_descriptors[i].is_free = TRUE;
	stream_descriptors[i].callback_data.fd = i;
	stream_descriptors[i].callback_data.buffer = NULL;
	++i;
      }
  }

  stream_descriptors_max_size = STREAM_DESCRIPTORS_INITIAL_SIZE;
}


/* We might want to do some sort of freelist thing later. */

static void
grow_stream_descriptors(void)
{
  stream_descriptors_max_size += STREAM_DESCRIPTORS_INITIAL_SIZE;

  stream_descriptors = realloc(stream_descriptors,
			       stream_descriptors_max_size);
  if (stream_descriptors == NULL)
    {
      fprintf(stderr,
	      "Failed to allocate memory for stream descriptor\n");
      exit(1);
    }
}

static void
initialize_stream_descriptor(stream_descriptor_t* stream_descriptor,
			     enum stream_descriptor_type type)
{

  stream_descriptor->type = type;
  stream_descriptor->is_free = FALSE;

  /* #### this should probably move to the caller */

  switch (type)
    {
    case STREAM_FILE_REGULAR:
      stream_descriptor->file_regular_data.current_offset_high = 0;
	  stream_descriptor->file_regular_data.current_offset = 0;
      break;
    }

  stream_descriptor->callback_data.current_size = 0;

  if (stream_descriptor->callback_data.buffer == NULL)
    {
      stream_descriptor->callback_data.current
	= stream_descriptor->callback_data.buffer
	= malloc(FILE_CALLBACK_BUFFER_INITIAL_SIZE);

      if (stream_descriptor->callback_data.buffer == NULL)
	{
	  fprintf(stderr,
		  "Failed to allocate memory for stream buffer\n");
	  exit(1);
	}

      stream_descriptor->callback_data.max_size
	= FILE_CALLBACK_BUFFER_INITIAL_SIZE;
      stream_descriptor->callback_data.current_size = 0;
    }
}

static void
deallocate_fd(long fd)
{
  stream_descriptors[fd].is_free = TRUE;
}

static long
allocate_fd(enum stream_descriptor_type type)
{
  size_t i = 0;

  while ((i < stream_descriptors_max_size)
	 && (!(stream_descriptors[i].is_free)))
      ++i;

  if (i == stream_descriptors_max_size)
    grow_stream_descriptors();

  initialize_stream_descriptor(&(stream_descriptors[i]), type);

  return i;
}


/* Windows allows doing fully asynchronous I/O on files and sockets.
   However, Windows still doesn't fit the Scheme 48 model completely.

   Here are the notable mismatches:

   - Scheme 48 uses longs uniformly to represent stream descriptors.
     Windows uses different types for files and sockets (HANDLE and SOCKET)

   - Scheme 48 reads first, sees that it can't complete immediately,
     queues up that it wants to read, then tries to read again.

     In Windows, you request a read, and a callback will get called
     *with the data*.

   - For writes we're better off because the model of Scheme 48
     actually fits Windows quite closely. */

int
ps_open_fd(char *filename, bool is_input, long *status)
{
#define FILE_NAME_SIZE 1024
#define PERMISSION 0666   /* read and write for everyone */

  HANDLE handle;
  char filename_temp[FILE_NAME_SIZE];
  char *expanded;
  extern char *s48_expand_file_name(char *, char *, int);

  expanded = s48_expand_file_name(filename, filename_temp, FILE_NAME_SIZE);
  if (expanded == NULL)
    return -1;

  handle = CreateFile(expanded,
		      is_input ? GENERIC_READ : GENERIC_WRITE,
		      is_input ? FILE_SHARE_READ : FILE_SHARE_WRITE,
		      NULL,
		      is_input ? OPEN_EXISTING : CREATE_ALWAYS,
		      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		      NULL);
  
  if (handle == INVALID_HANDLE_VALUE)
    {
      *status = (long) GetLastError();
      return -1;
    }
  else
    {
      long fd = allocate_fd(STREAM_FILE_REGULAR);
	  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);
	  callback_data_t* callback_data = &(stream_descriptor->callback_data);
      *status = NO_ERRORS;
      /* here, things are always regular */
      stream_descriptor->file_regular_data.handle = handle;
	  callback_data->overlap.Offset = 0;
	  callback_data->overlap.OffsetHigh = 0;
	  
      return fd;
    }
}

/*
 * This is for I/O streams that do not support overlapped I/O or a
 * select-like operations.  (Stdin/out/error don't support overlapped
 * I/O; pipes don't support select.)  The ideas are stolen from
 * MzScheme; the help of Matthew Flatt is gratefully acknowledged.
 *
 * Each such stream has an associated threads which carries out I/O
 * actions on behalf of some other thread.  Upon completion, an
 * asynchronous procedure call to a procedure specified by the caller
 * is registered.
 */

HANDLE s48_main_thread;

DWORD WINAPI
reader_thread_proc(LPVOID lpParameter)
{
  DWORD n_read;

  stream_descriptor_t* stream_descriptor = (stream_descriptor_t*)lpParameter;
  file_thread_data_t* thread_data
    = &(stream_descriptor->file_special_data.thread_data);

  while ((!thread_data->eof) && (!thread_data->error))
    {
      /* this comes from the main thread */
      WaitForSingleObject(thread_data->check_semaphore, INFINITE);

      /* printf("[reader_thread_proc: waited for semaphore]\n"); */

      if (thread_data->abort)
	break;

      /* printf("[reader_thread_proc:want %ld bytes\n", (long) thread_data->requested); */
      
      if (ReadFile(thread_data->file_handle, thread_data->buffer,
		   thread_data->requested, &n_read, NULL))
	{
	  thread_data->error = FALSE;
	  thread_data->available = n_read;
	  /* kludge: pressing Ctrl-C looks like EOF on stdin */
	  if ((n_read == 0) && (stream_descriptor->callback_data.fd != 0))
	    thread_data->eof = TRUE;
	}
      else
	{
	  thread_data->error = TRUE;
	  thread_data->error_code = GetLastError();
	}

      /* printf("reader_thread_proc:got %ld bytes]\n", (long) n_read); */
      

      /* notify */
      /* printf("QueueUserAPC(%p, %p, %ld", thread_data->callback,
	 thread_data->callback_thread,
	 (DWORD) thread_data); */
      if (!QueueUserAPC(thread_data->callback,
			thread_data->callback_thread,
			(DWORD) stream_descriptor))

	{
	  fprintf(stderr, "QueueUserAPC failed\n");
	  exit(-1);
	}
    }

  /* printf("reader_thread_proc]\n"); */

  /* clean up */
  CloseHandle(thread_data->check_semaphore);
  ExitThread(0);
  return 0;
}

/* #### most of this is the same as reader_thread_proc #### */

DWORD WINAPI
writer_thread_proc(LPVOID lpParameter)
{
  DWORD n_written;

  stream_descriptor_t* stream_descriptor = (stream_descriptor_t*)lpParameter;
  file_thread_data_t* thread_data
    = &(stream_descriptor->file_special_data.thread_data);

  while (!thread_data->error)
    {
      /* this comes from the main thread */
      WaitForSingleObject(thread_data->check_semaphore, INFINITE);

      /* printf("[writer_thread_proc: waited for semaphore]\n"); */

      if (thread_data->abort)
	break;

      /* printf("[writer_thread_proc:want %ld bytes\n", (long) thread_data->requested); */
      
      if (WriteFile(thread_data->file_handle, thread_data->buffer,
		    thread_data->requested, &n_written, NULL))
	{
	  thread_data->error =FALSE;
	  thread_data->available = n_written;
	}
      else
	{
	  thread_data->error = TRUE;
	  thread_data->error_code = GetLastError();
	}
      
      /* printf("writer_thread_proc:got %ld bytes]\n", (long) n_read); */
      

      /* notify */
      /* printf("QueueUserAPC(%p, %p, %ld", thread_data->callback,
	 thread_data->callback_thread,
	 (DWORD) thread_data); */
      if (!QueueUserAPC(thread_data->callback,
			thread_data->callback_thread,
			(DWORD) stream_descriptor))

	{
	  fprintf(stderr, "QueueUserAPC failed\n");
	  exit(-1);
	}
    }

  /* printf("writer_thread_proc]\n"); */

  /* clean up */
  CloseHandle(thread_data->check_semaphore);
  ExitThread(0);
  return 0;
}

static HANDLE
create_mutex_semaphore()
{
  HANDLE handle = CreateSemaphore(NULL, /* lpSemaphoreAttributes */
				  0, /* lInitialCount */
				  1, /* lMaximumCount */
				  NULL); /* lpName */

  if (handle == NULL)
    {
      fprintf(stderr, "error in CreateSemaphore\n");
      exit(-1);
    }

  return handle;
}

static void
start_reader_slash_writer_thread(HANDLE file_handle,
				 stream_descriptor_t* stream_descriptor,
				 LPTHREAD_START_ROUTINE thread_proc)
{
  file_thread_data_t* thread_data = &(stream_descriptor->file_special_data.thread_data);
  HANDLE thread_handle;
  DWORD id;

  thread_data->abort = FALSE;

  thread_data->available = 0;

  thread_data->error = FALSE;
  thread_data->eof = FALSE;
  
  thread_data->file_handle = file_handle;
  thread_data->check_semaphore = create_mutex_semaphore();

  thread_data->checking = FALSE;
  
  thread_handle = CreateThread(NULL, /* lpThreadAttributes */
			       4096, /* dwStackSize, */
			       (LPTHREAD_START_ROUTINE)thread_proc,
			       stream_descriptor,
			       0, /* dwCreationFlags, */
			       &id);
  if (thread_handle == NULL)
    {
      fprintf(stderr, "CreateThread failed in start_reader_slash_writer_thread\n");
      free(thread_data);
      exit(-1);
    }

  thread_data->thread_handle = thread_handle;
}


static void
open_special_fd(HANDLE handle, size_t fd, bool is_input)
{
  stream_descriptor_t* stream_descriptor;

  if (fd >= stream_descriptors_max_size)
    grow_stream_descriptors();

  if (!(stream_descriptors[fd].is_free))
    {
      fprintf(stderr, "fd %d isn't available\n", fd);
      exit(-1);
    }

  stream_descriptor = &(stream_descriptors[fd]);

  initialize_stream_descriptor(stream_descriptor, STREAM_FILE_SPECIAL);
  
  start_reader_slash_writer_thread(handle,
				   stream_descriptor,
				   is_input
				   ? (LPTHREAD_START_ROUTINE)reader_thread_proc
				   : (LPTHREAD_START_ROUTINE)writer_thread_proc);
}

int
ps_close_fd(long fd)
{
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);

  switch (stream_descriptor->type)
    {
    case STREAM_FILE_REGULAR:
      {
	HANDLE handle = stream_descriptor->file_regular_data.handle;
	
	if (CloseHandle(handle))
	  {
	    deallocate_fd(fd); // ####might want to do this in other branch as well
	    return NO_ERRORS;
	  }
	else
	  return (int) GetLastError();
      }
    case STREAM_FILE_SPECIAL:
      {
	file_thread_data_t* thread_data =
	  &(stream_descriptor->file_special_data.thread_data);

	/* at some point, we might wait for a success notification */
	thread_data->abort = TRUE;
	ReleaseSemaphore(thread_data->check_semaphore, 1, NULL);
	return NO_ERRORS;
      }
    }
  return -1; /* shouldn't happen */
}

extern bool s48_is_pending(long);
extern void s48_add_ready_fd(long, bool, long);

/* This is called as the result of a completed read operation; either
   from the overlapped I/O completion routine, or from the callback
   from the reader thread. */

static void
read_done(DWORD dwErr,
	  size_t bytes_read,
	  stream_descriptor_t* stream_descriptor)
{
  callback_data_t* callback_data = &(stream_descriptor->callback_data);
  long fd = callback_data->fd;

  if ((dwErr == 0) && (bytes_read != 0) && s48_is_pending(fd))
    {
      switch (stream_descriptor->type) {
      case STREAM_FILE_REGULAR:
	{
	  /* ps_check_fd doesn't go through this */
	  s48_add_ready_fd(fd, TRUE, (long)0); /* *not* bytes_read */
	  stream_descriptor->file_regular_data.current_offset += bytes_read;
	  break;
	}
      case STREAM_FILE_SPECIAL:
	{
	  file_thread_data_t* thread_data =
	    &(stream_descriptor->file_special_data.thread_data);
	  if (thread_data->checking) /* ps_check_fd */
	    thread_data->checking = FALSE;
	  else
	    /* regular read */
	    s48_add_ready_fd(fd, TRUE, (long)0); /* *not* bytes_read */
	  break;
	}
      }

      // ### need to do offset_high as well
      stream_descriptor->callback_data.current_size = bytes_read;
      stream_descriptor->callback_data.current = 
	stream_descriptor->callback_data.buffer;
    }
  else
    {
      // ####
    }

}

/* for regular files; from overlapped I/O */
static VOID WINAPI
read_completed(DWORD dwErr, DWORD cbBytesRead, LPOVERLAPPED lpOverLap)
{
  callback_data_t* callback_data = (callback_data_t*) lpOverLap;
  long fd = callback_data->fd;
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);

  read_done(dwErr, (size_t)cbBytesRead, stream_descriptor);
}

/* for special files; from QueueUserAPC */
static VOID CALLBACK
read_callback(DWORD dwParam)
{
  stream_descriptor_t* stream_descriptor = (stream_descriptor_t*)dwParam;
  file_thread_data_t* thread_data
    = &(stream_descriptor->file_special_data.thread_data);

  read_done(thread_data->error ? thread_data->error_code : 0,
	    (size_t)thread_data->available,
	    stream_descriptor);
}

bool ps_check_fd(long fd, bool is_read, long *status)
{
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);

  *status = NO_ERRORS; /* #### overly optimistic #### */

  switch (stream_descriptor->type) {
  case STREAM_FILE_REGULAR:
    {
      HANDLE handle = stream_descriptor->file_regular_data.handle;

      if (stream_descriptor->callback_data.current_size > 0)
	return is_read;

      return (WaitForSingleObject(handle, 0) == WAIT_OBJECT_0) ? TRUE : FALSE;
    }
  case STREAM_FILE_SPECIAL:
    {
      /* approximations only */
      file_thread_data_t* thread_data;

      if (!is_read)
	return stream_descriptor->callback_data.current_size == 0;
	
      if (is_read && (stream_descriptor->callback_data.current_size > 0))
	return TRUE;
      /* we only signal that writing is enabled if the buffer's empty */

      thread_data = &(stream_descriptor->file_special_data.thread_data);

      if (!thread_data->checking)
	/* get the reader thread started */
	{
	  thread_data->buffer = stream_descriptor->callback_data.buffer;
	  thread_data->requested = 1;
	  thread_data->callback = read_callback;
	  thread_data->callback_thread = s48_main_thread;
	  thread_data->checking = TRUE;
	  ReleaseSemaphore(thread_data->check_semaphore, 1, NULL);
	}
      return FALSE;
    }
  }
  return FALSE; /* shouldn't happen */
}



/* waitp doesn't mean it should wait---it only means that the read
   shouldn't be registered anywhere if this is false.
   
   If EOF has been reached (i.e. no characters can be read),
   *eofp is set.

   If the read operation is queued up for asynchronous completion,
   *pending is set.

   Returns number of characters actually read.  (This may be 0.)
*/

long
ps_read_fd(long fd, char *buffer, long max, bool waitp,
	   bool *eofp, bool *pending, long *status)
{
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);
  callback_data_t* callback_data = &(stream_descriptor->callback_data);
  HANDLE handle = stream_descriptor->file_regular_data.handle;

  /* for the normal return */
  *eofp = FALSE;
  *pending = FALSE;
  *status = NO_ERRORS;

  // ### waitp */

  /* there's still stuff in the buffer */
  /* ### need to do other stuff than just files */
  if (callback_data->current_size > 0)
    {
      /* we want more than what's in the buffer */
      if (max >= (long)callback_data->current_size)
	{
	  size_t size = callback_data->current_size;
	  memcpy(buffer, callback_data->current, size);
	  callback_data->current_size = 0;
	  callback_data->current = stream_descriptor->callback_data.buffer;
	  return size;
	}
      else
	/* less; shouldn't happen */
	{
	  memcpy(buffer, callback_data->current, max);
	  callback_data->current += max;
	  callback_data->current_size -= max;
	  return max;
	}
    }

  if (!s48_add_pending_fd(fd, TRUE))
    {
      *status = 4712; //#### out of memory, need symbolic constant
      return 0;
    }

  /* There's nothing in the buffer---need to do an actual read. */

  maybe_grow_callback_data_buffer(callback_data, (size_t)max);

  switch (stream_descriptor->type)
    {
    case STREAM_FILE_REGULAR:
      {
	callback_data->overlap.Offset = stream_descriptor->file_regular_data.current_offset;
	callback_data->overlap.OffsetHigh = stream_descriptor->file_regular_data.current_offset_high;

	if (waitp ? ReadFileEx(handle,
			       (LPVOID)callback_data->buffer,
			       (DWORD)max,
			       (LPOVERLAPPED)callback_data,
			       read_completed)
	    : ReadFile(handle,
		       (LPVOID)callback_data->buffer,
		       (DWORD)max,
		       NULL, /* wouldn't work under 95/98/ME */
		       (LPOVERLAPPED)callback_data))
	    
	  {
	    // success
	    if (waitp)
	      *pending = TRUE;
	    return 0;
	  }
	else
	  {
	    DWORD last_error = GetLastError();
	    
	    if (last_error == ERROR_HANDLE_EOF)
	      {
		*eofp = TRUE;
		return 0;
		
	      }
	    
	    /* failure */
	    *status = last_error;
	    return 0;
	  }
      }
    case STREAM_FILE_SPECIAL:
      {
	if (waitp)
	  {
	    file_thread_data_t* thread_data =
	      &(stream_descriptor->file_special_data.thread_data);
	    thread_data->buffer = callback_data->buffer;
	    thread_data->requested = max;
	    thread_data->callback_thread = s48_main_thread;
	    thread_data->callback = read_callback;
	    ReleaseSemaphore(thread_data->check_semaphore, 1, NULL);
	  }
	return 0;
      }
    }
  return 0; /* shouldn't happen */
}

static VOID WINAPI
write_done(DWORD dwErr,
	   size_t bytes_written,
	   stream_descriptor_t* stream_descriptor) 
{
  callback_data_t* callback_data = &(stream_descriptor->callback_data);
  long fd = callback_data->fd;
  
  if ((dwErr == 0) && (bytes_written != 0) && (s48_is_pending(fd)))
    {
      switch (stream_descriptor->type) {
      case STREAM_FILE_REGULAR:
	{
	  stream_descriptor->file_regular_data.current_offset += bytes_written;
	  // ### need to do offset_high as well
	  break;
	}
      }
      s48_add_ready_fd(fd, FALSE, (long)bytes_written);
    }
  else
    {
      // ####
    }
}

/* for regular files; from overlapped I/O */
static VOID WINAPI
write_completed(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap) 
{
  callback_data_t* callback_data = (callback_data_t*) lpOverLap;
  long fd = callback_data->fd;
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);

  write_done(dwErr, (size_t)cbWritten, stream_descriptor);
}

/* for special files; from QueueUserAPC */
static VOID CALLBACK
write_callback(DWORD dwParam)
{
  stream_descriptor_t* stream_descriptor = (stream_descriptor_t*)dwParam;
  file_thread_data_t* thread_data
    = &(stream_descriptor->file_special_data.thread_data);

  write_done(thread_data->error ? thread_data->error_code : 0,
	     (size_t)thread_data->available,
	     stream_descriptor);
}

long
ps_write_fd(long fd, char *buffer, long max, bool *pending, long *status)
{
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);
  callback_data_t* callback_data = &(stream_descriptor->callback_data);


  if (!s48_add_pending_fd(fd, FALSE))
    {
      *status = 4711; //#### out of memory, need symbolic constant
      return 0;
    }

  maybe_grow_callback_data_buffer(callback_data, (size_t)max);

  memcpy(callback_data->buffer, buffer, (size_t)max);

  switch (stream_descriptor->type)
    {
    case STREAM_FILE_REGULAR:
      {
	HANDLE handle = stream_descriptor->file_regular_data.handle;

	callback_data->overlap.Offset = stream_descriptor->file_regular_data.current_offset;
	callback_data->overlap.OffsetHigh = stream_descriptor->file_regular_data.current_offset_high;


	if (WriteFileEx(handle,
			(LPVOID)callback_data->buffer,
			(DWORD)max,
			(LPOVERLAPPED)callback_data,
			write_completed))
	  {
	    // success
	    *pending = TRUE;
	    *status = NO_ERRORS;
	    // ####should check GetLastError anyway according to MS docs
	  }
	else
	  {
	    *pending = FALSE;
	    *status = (int) GetLastError();
	  }
	return 0; /* we always wait for the callback */
      }
    case STREAM_FILE_SPECIAL:
      {
	file_thread_data_t* thread_data =
	  &(stream_descriptor->file_special_data.thread_data);
	thread_data->buffer = callback_data->buffer;
	thread_data->requested = max;
	thread_data->callback_thread = s48_main_thread;
	thread_data->callback = write_callback;
	ReleaseSemaphore(thread_data->check_semaphore, 1, NULL);
	*pending = TRUE;
	*status = NO_ERRORS;
      }
    }
  return 0; /* shouldn't happen */
}

long
ps_abort_fd_op(long fd_as_long)
{
  int fd = (int)fd_as_long;

  if (!s48_remove_fd(fd))
    fprintf(stderr, "Error: ps_abort_fd_op, no pending operation on fd %d\n",
	            fd);
  return 0;      /* because we do not actually do any I/O in parallel the
		    status is always zero: no characters transfered. */
}

/*
 * This checks that fd's destined for output channels are marked
 * as nonblocking.  Stdout and stderr are a special case because
 * we do not want to screw up the shell, if we were invoked from
 * one.
 */

s48_value
s48_add_channel(s48_value mode, s48_value id, long fd)
{
  /* back to the VM */
  return s48_really_add_channel(mode, id, fd);
}

void
s48_fd_io_init()
{
  initialize_stream_descriptors();

  /* Yes, this is the official hoopla to get at an absolute handle for
     the current thread.  GetCurrentThread() returns a *constant*. */
     
  if (!DuplicateHandle(GetCurrentProcess(),
		       GetCurrentThread(), GetCurrentProcess(),
		       &s48_main_thread,
		       THREAD_ALL_ACCESS, FALSE, 0))
    {
      fprintf(stderr, "DuplicateHandle failed\n");
      exit(-1);
    }

  /* these Unix-style indices are hard-wired into the VM */
  open_special_fd(GetStdHandle(STD_INPUT_HANDLE),  0, TRUE);
  open_special_fd(GetStdHandle(STD_OUTPUT_HANDLE), 1, FALSE);
  open_special_fd(GetStdHandle(STD_ERROR_HANDLE),  2, FALSE);
}


