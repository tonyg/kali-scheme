/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#define _WIN32_WINNT 0x0400 /* for QueueUserAPC */

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <ws2tcpip.h>

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
  psbool checking; 

  /* in */
  psbool abort;
  char* buffer;
  size_t requested;

  PAPCFUNC callback;
  HANDLE callback_thread;

  /* out */
  size_t available; /* bytes written, readers only */
  psbool eof; /* readers only */
  psbool error;
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

  psbool is_free;

  union {

    struct {
      HANDLE handle;
      DWORD current_offset;
      DWORD current_offset_high;
    } file_regular_data;

    struct {
      file_thread_data_t thread_data;
    } file_special_data;
  
    struct {
      SOCKET socket;
      WSABUF wsabuf;
      SOCKET hatched_socket;
      /* callback to set socket hatched by connect/accept  */
      PAPCFUNC hatch_callback;
    } socket_data;
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
  stream_descriptor->is_free = PSFALSE;

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
ps_open_fd(char *filename, psbool is_input, long *status)
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
  DWORD file_type = GetFileType(thread_data->file_handle);

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
	  thread_data->error = PSFALSE;
	  thread_data->available = n_read;
	  /* kludge: pressing Ctrl-C looks like EOF on stdin */
	  if ((n_read == 0)  && (file_type != FILE_TYPE_CHAR))
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
	  thread_data->error = PSFALSE;
	  thread_data->available = n_written;
	}
      else
	{
	  thread_data->error = PSTRUE;
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

  thread_data->abort = PSFALSE;

  thread_data->available = 0;

  thread_data->error = PSFALSE;
  thread_data->eof = PSFALSE;
  
  thread_data->file_handle = file_handle;
  thread_data->check_semaphore = create_mutex_semaphore();

  thread_data->checking = PSFALSE;
  
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
open_special_fd(HANDLE handle, size_t fd, psbool is_input)
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
    case STREAM_SOCKET:
      {
	SOCKET socket = stream_descriptor->socket_data.socket;
	
	if (closesocket(socket) == 0)
	  {
	    deallocate_fd(fd);
	    return NO_ERRORS;
	  }
	else
	  return (int) WSAGetLastError();
      }
    }
  return -1; /* shouldn't happen */
}

extern psbool s48_is_pending(long);
extern void s48_add_ready_fd(long, psbool, long);
extern void s48_register_error(DWORD error);

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

  if (dwErr != 0)
    s48_register_error(dwErr);

  if (s48_is_pending(fd))
    {
      switch (stream_descriptor->type) {
      case STREAM_FILE_REGULAR:
	{
	  /* ps_check_fd doesn't go through this */
	  s48_add_ready_fd(fd, PSTRUE, (long)0); /* *not* bytes_read */
	  stream_descriptor->file_regular_data.current_offset += bytes_read;
	  break;
	}
      case STREAM_FILE_SPECIAL:
	{
	  file_thread_data_t* thread_data =
	    &(stream_descriptor->file_special_data.thread_data);
	  if (thread_data->checking) /* ps_check_fd */
	    thread_data->checking = PSFALSE;
	  else
	    /* regular read */
	    s48_add_ready_fd(fd, PSTRUE, (long)0); /* *not* bytes_read */
	  break;
	}
      case STREAM_SOCKET:
	{
	  /* ps_check_fd doesn't go through this */
	  s48_add_ready_fd(fd, PSTRUE, (long)0); /* *not* bytes_read */
	  break;
	}
      }

      // ### need to do offset_high as well
      stream_descriptor->callback_data.current_size = bytes_read;
      stream_descriptor->callback_data.current = 
	stream_descriptor->callback_data.buffer;
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

/* from sockets */
static VOID CALLBACK
recv_completed(DWORD dwErr, DWORD cbBytesRead, LPOVERLAPPED lpOverLap, DWORD dwFlags)
{
  read_completed(dwErr, cbBytesRead, lpOverLap);
}

psbool ps_check_fd(long fd, psbool is_read, long *status)
{
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);

  *status = NO_ERRORS; /* #### overly optimistic #### */

  switch (stream_descriptor->type) {
  case STREAM_FILE_REGULAR:
    {
      HANDLE handle = stream_descriptor->file_regular_data.handle;

      if (stream_descriptor->callback_data.current_size > 0)
	return is_read;

      /* #### this probably doesn't really work */
      return (WaitForSingleObject(handle, 0) == WAIT_OBJECT_0) ? PSTRUE : PSFALSE;
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
      return PSFALSE;
    }
  case STREAM_SOCKET:
    {
      SOCKET socket = stream_descriptor->socket_data.socket;
      DWORD numberOfBytesRecvd;
      DWORD flags;
      callback_data_t* callback_data = &(stream_descriptor->callback_data);

      if (stream_descriptor->callback_data.current_size > 0)
	return is_read;

      stream_descriptor->socket_data.wsabuf.len = 1;
      stream_descriptor->socket_data.wsabuf.buf = (char*)callback_data->buffer;

      /* just making sure */
      callback_data->overlap.Offset = 0;
      callback_data->overlap.OffsetHigh = 0;
      callback_data->overlap.hEvent = NULL; /* this is hopefully invalid */

      flags = MSG_PEEK;

      return (WSARecv(socket,
		      (LPWSABUF)&stream_descriptor->socket_data.wsabuf,
		      1,
		      &numberOfBytesRecvd,
		      &flags,
		      (LPOVERLAPPED)callback_data,
		      NULL)
	      == 0)
	? PSTRUE : PSFALSE;
    }
  }
  return PSFALSE; /* shouldn't happen */
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
ps_read_fd(long fd, char *buffer, long max, psbool waitp,
	   psbool *eofp, psbool *pending, long *status)
{
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);
  callback_data_t* callback_data = &(stream_descriptor->callback_data);

  /* for the normal return */
  *eofp = PSFALSE;
  *pending = PSFALSE;
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

  switch (stream_descriptor->type)
    {
    case STREAM_FILE_REGULAR:
      {
	HANDLE handle = stream_descriptor->file_regular_data.handle;

	if (!s48_add_pending_fd(fd, TRUE))
	  {
	    *status = 4712; //#### out of memory, need symbolic constant
	    return 0;
	  }
	
	/* There's nothing in the buffer---need to do an actual read. */
	maybe_grow_callback_data_buffer(callback_data, (size_t)max);

	callback_data->overlap.Offset = stream_descriptor->file_regular_data.current_offset;
	callback_data->overlap.OffsetHigh = stream_descriptor->file_regular_data.current_offset_high;

	if (waitp ? ReadFileEx(handle,
			       (LPVOID)callback_data->buffer,
			       (DWORD)max,
			       (LPOVERLAPPED)callback_data,
			       read_completed)
	    /* ####this is probably wrong; should just do ReadFileEx always */
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
	file_thread_data_t* thread_data =
	  &(stream_descriptor->file_special_data.thread_data);

	if (thread_data->eof)
	  {
	    *eofp = TRUE;
	    return 0;
	  }

	if (!s48_add_pending_fd(fd, TRUE))
	  {
	    *status = 4712; //#### out of memory, need symbolic constant
	    return 0;
	  }
	
	/* There's nothing in the buffer---need to do an actual read. */
	maybe_grow_callback_data_buffer(callback_data, (size_t)max);

	if (waitp)
	  {
	    thread_data->buffer = callback_data->buffer;
	    thread_data->requested = max;
	    thread_data->callback_thread = s48_main_thread;
	    thread_data->callback = read_callback;
	    ReleaseSemaphore(thread_data->check_semaphore, 1, NULL);
	    *pending = TRUE;
	  }
	return 0;
      }
    case STREAM_SOCKET:
      {
	SOCKET socket = stream_descriptor->socket_data.socket;
	DWORD numberOfBytesRecvd;
	DWORD flags = 0;
	int wsa_status;
	int block;

	if (!s48_add_pending_fd(fd, TRUE))
	  {
	    *status = 4712; //#### out of memory, need symbolic constant
	    return 0;
	  }
	
	/* There's nothing in the buffer---need to do an actual read. */
	maybe_grow_callback_data_buffer(callback_data, (size_t)max);

	/* just making sure */
	callback_data->overlap.Offset = 0;
	callback_data->overlap.OffsetHigh = 0;

	stream_descriptor->socket_data.wsabuf.len = max;
	stream_descriptor->socket_data.wsabuf.buf = (char*)callback_data->buffer;

	block = 1;
	ioctlsocket(socket, FIONBIO, &block);

	wsa_status = WSARecv(socket,
			     (LPWSABUF)&stream_descriptor->socket_data.wsabuf,
			     1,
			     &numberOfBytesRecvd,
			     &flags,
			     (LPOVERLAPPED)callback_data,
			     recv_completed);

	if ((wsa_status == 0)
	    && (numberOfBytesRecvd == 0))
	  *eofp = TRUE;

	if ((wsa_status == 0)
	    || (WSAGetLastError() == WSA_IO_PENDING))
	  {
	    if (waitp)
	      *pending = TRUE;
	    return 0;
	  }
	else
	  {
	    *status = WSAGetLastError();
	    return 0;
	  }
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

  if (dwErr != 0)
    s48_register_error(dwErr);

  if ((bytes_written != 0) && (s48_is_pending(fd)))
    {
      switch (stream_descriptor->type) {
      case STREAM_FILE_REGULAR:
	{
	  stream_descriptor->file_regular_data.current_offset += bytes_written;
	  // ### need to do offset_high as well
	  break;
	}
      }
      s48_add_ready_fd(fd, PSFALSE, (long)bytes_written);
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

/* for sockets */
static VOID CALLBACK
send_completed(DWORD dwErr, DWORD cbTransferred, LPOVERLAPPED lpOverLap, DWORD dwFlags)
{
  write_completed(dwErr, cbTransferred, lpOverLap);
}

long
ps_write_fd(long fd, char *buffer, long max, psbool *pending, long *status)
{
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[fd]);
  callback_data_t* callback_data = &(stream_descriptor->callback_data);

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
	    if (!s48_add_pending_fd(fd, PSFALSE))
	      {
		*status = 4711; //#### out of memory, need symbolic constant
		return 0;
	      }
	    *pending = TRUE;
	    *status = NO_ERRORS;
	    // ####should check GetLastError anyway according to MS docs
	  }
	else
	  {
	    *pending = PSFALSE;
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
	if (!s48_add_pending_fd(fd, PSFALSE))
	  {
	    *status = 4711; //#### out of memory, need symbolic constant
	    return 0;
	  }
	*pending = TRUE;
	*status = NO_ERRORS;
	return 0; /* the thread needs to do the work */
      }
    case STREAM_SOCKET:
      {
	SOCKET socket = stream_descriptor->socket_data.socket;
	DWORD numberOfBytesSent;
	DWORD flags = 0;
	int wsa_status;

	/* just making sure */
	callback_data->overlap.Offset = 0;
	callback_data->overlap.OffsetHigh = 0;

	stream_descriptor->socket_data.wsabuf.len = max;
	stream_descriptor->socket_data.wsabuf.buf = (char*)callback_data->buffer;

	wsa_status = WSASend(socket,
			     (LPWSABUF)&stream_descriptor->socket_data.wsabuf,
			     1,
			     &numberOfBytesSent,
			     flags,
			     (LPOVERLAPPED)callback_data,
			     send_completed);

	if ((wsa_status == 0)
	    | (WSAGetLastError() == WSA_IO_PENDING))
	  {
	    /* success */
	    if (!s48_add_pending_fd(fd, PSFALSE))
	      {
		*status = 4711; //#### out of memory, need symbolic constant
		return 0;
	      }
	    *pending = TRUE;
	    *status = NO_ERRORS;
	  }
	else
	  {
	    *pending = PSFALSE;
	    *status = (int) WSAGetLastError();
	  }
	return 0; /* we always wait for the callback */
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

/*
 * An interface to Windows Sockets 2
 */

/*
 * Windows Sockets 2 is a mumbojumbo of Berkeley sockets, and
 * Microsoft's own stuff thrown in.  Of course, the two halves use
 * different naming conventions, and things are generally confusing.
 * Moreover, socket handles and file handles are distinct (at least
 * partially), so we can't use the file operations we want on sockets.
 */

/* Henry Cejtin says that 5 is the largest safe number for this, at least on Unix. */
#define	LISTEN_QUEUE_SIZE	5

extern void		s48_init_socket(void);
static s48_value	s48_socket(s48_value udp_p, s48_value input_p),
			s48_bind(s48_value socket_channel, s48_value number),
			s48_socket_number(s48_value socket_channel),
			s48_listen(s48_value socket_channel),
			s48_accept(s48_value socket_channel, s48_value retry_p),
			s48_connect(s48_value socket_channel,
				    s48_value machine,
				    s48_value port,
				    s48_value retry_p),
                        s48_dup_socket_channel(s48_value socket_fd),
			s48_close_socket_half(s48_value socket_channel,
					      s48_value input_p),
			s48_get_host_name(void);

/* Forward declaration. */
static s48_value dup_socket_channel(long socket_fd);

/*
 * Start up the sockets stuff and install all exported functions.
 */

void
s48_init_socket(void)
{
  WORD wVersionRequested;
  WSADATA wsaData;
 
  /* This is the *highest* version we can use.  Great. */
  wVersionRequested = MAKEWORD(2, 0);
 
  if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
      fprintf(stderr, "Windows Sockets startup failed.\n");
      exit(-1);
    }

  S48_EXPORT_FUNCTION(s48_socket);
  S48_EXPORT_FUNCTION(s48_bind);
  S48_EXPORT_FUNCTION(s48_socket_number);
  S48_EXPORT_FUNCTION(s48_listen);
  S48_EXPORT_FUNCTION(s48_accept);
  S48_EXPORT_FUNCTION(s48_connect);
  S48_EXPORT_FUNCTION(s48_dup_socket_channel);
  S48_EXPORT_FUNCTION(s48_close_socket_half);
  S48_EXPORT_FUNCTION(s48_get_host_name);
}

static void
raise_windows_error(DWORD id)
{
  char buf[512];

  for (;;)
    {
      if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, /* lpSource */
			id,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, sizeof(buf)-1,
			NULL)) /* arguments ... */
	s48_raise_string_os_error(buf);
      else
	/* risky, but we assume some amount of sanity on the side of
	   the Windows implementors---haha */
	id = GetLastError();
    }
}

/*
 * Create an internet-domain stream (reliable, sequenced) socket.
 * We return an input channel on success and raise an exception on failure.
 * The socket has been made non-blocking.
 */

static s48_value
s48_socket(s48_value udp_p, s48_value input_p)
{
  int mode;
  long fd;
  s48_value channel;
  stream_descriptor_t* stream_descriptor;

  SOCKET socket = WSASocket(AF_INET,
			    (udp_p == S48_FALSE)
			    ? SOCK_STREAM
			    : SOCK_DGRAM,
			    0, /* protocol */
			    NULL,
			    0, /* reserved */
			    WSA_FLAG_OVERLAPPED);

  if (socket == INVALID_SOCKET)
    raise_windows_error(WSAGetLastError());

  mode = (input_p == S48_FALSE)
    ? S48_CHANNEL_STATUS_SPECIAL_OUTPUT
    : S48_CHANNEL_STATUS_SPECIAL_INPUT;

  fd = allocate_fd(STREAM_SOCKET);
  stream_descriptor = &(stream_descriptors[fd]);
  stream_descriptor->socket_data.socket = socket;

  channel = s48_add_channel(mode, s48_enter_string("socket"), fd);

  if (!S48_CHANNEL_P(channel))
    {
      ps_close_fd(fd);
      s48_raise_scheme_exception(s48_extract_fixnum(channel), 0);
    };

  return channel;
}

/*
 * Given an internet-domain stream socket and a port number, bind
 * the socket to the port and prepare to receive connections.
 * If the port number is #f, then we bind the socket to any available
 * port.
 * 
 * Nothing useful is returned.
 */

static s48_value
s48_bind(s48_value channel, s48_value port_number)
{
  int socket_fd;
  unsigned short port;
  int status;
  SOCKET socket;
  struct sockaddr_in address;
  stream_descriptor_t* stream_descriptor;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  stream_descriptor = &(stream_descriptors[socket_fd]);
  socket = stream_descriptor->socket_data.socket;

  if (port_number == S48_FALSE)
    port = 0;
  else
    port = (unsigned short)s48_extract_fixnum(port_number);

  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(port);

  status = bind(socket,
		(struct sockaddr *)&address,
		sizeof(address));
  if (status == SOCKET_ERROR)
    raise_windows_error(WSAGetLastError());
  
  return S48_UNSPECIFIC;
}

/*
 * Return the port number associated with an internet stream socket.
 */

static s48_value
s48_socket_number(s48_value channel)
{
  int socket_fd;
  int status;
  int len;
  struct sockaddr_in address;
  stream_descriptor_t* stream_descriptor;
  SOCKET socket;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  stream_descriptor = &(stream_descriptors[socket_fd]);
  socket = stream_descriptor->socket_data.socket;

  address.sin_addr.s_addr = htonl(INADDR_ANY);

  len = sizeof(address);

  status = getsockname(socket, (struct sockaddr *)&address, &len);

  if (status == SOCKET_ERROR)
    raise_windows_error(WSAGetLastError());

  return s48_enter_fixnum(ntohs(address.sin_port));
}
 
static s48_value
s48_listen(s48_value channel)
{
  int socket_fd;
  int status;
  stream_descriptor_t* stream_descriptor;
  SOCKET socket;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  stream_descriptor = &(stream_descriptors[socket_fd]);
  socket = stream_descriptor->socket_data.socket;

  status = listen(socket, LISTEN_QUEUE_SIZE);

  if (status == SOCKET_ERROR)
    raise_windows_error(WSAGetLastError());

  return S48_UNSPECIFIC;
}
 

/*
 * Given an internet-domain stream socket which has been bound
 * accept a connection and return the resulting socket as a pair of channels
 * (after marking it non-blocking).
 *
 * If the accept fails because the client hasn't connected yet, then we
 * return #f.
 *
 * If it fails for any other reason, then an exception is raised.
 */

/*
 * This guy finally registers the completed accept.
 */

static VOID CALLBACK
accept_callback(DWORD dwParam)
{
  stream_descriptor_t* stream_descriptor = (stream_descriptor_t*)dwParam;
  callback_data_t* callback_data = &(stream_descriptor->callback_data);

  s48_add_ready_fd(callback_data->fd, PSTRUE, (long)0);
}

/*
 *  This guy just waits for the event triggered by an accept,
 *  and notifies the VM.
 */

static DWORD WINAPI
hatch_thread_proc(LPVOID lpParameter)
{
  stream_descriptor_t* stream_descriptor = (stream_descriptor_t*)lpParameter;
  callback_data_t* callback_data = &(stream_descriptor->callback_data);

  WaitForSingleObject(callback_data->overlap.hEvent, INFINITE);

  if (!QueueUserAPC(stream_descriptor->socket_data.hatch_callback,
		    s48_main_thread,
		    (DWORD)stream_descriptor))
    {
      fprintf(stderr, "QueueUserAPC failed\n");
      exit(-1);
    }
  return 0;
}

static s48_value
s48_accept(s48_value channel, s48_value retry_p)
{
  long socket_fd;
  BOOL status;
  s48_value input_channel;
  stream_descriptor_t* stream_descriptor;
  callback_data_t* callback_data;
  SOCKET socket;
  SOCKET accept_socket;
  DWORD bytecount;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  stream_descriptor = &(stream_descriptors[socket_fd]);
  callback_data = &(stream_descriptor->callback_data);
  socket = stream_descriptor->socket_data.socket;

  if (retry_p == S48_FALSE)
    /* first time */
    {
      callback_data->overlap.Offset = 0; /* just making sure */
      callback_data->overlap.OffsetHigh = 0;
      callback_data->overlap.hEvent = CreateEvent(NULL, /* no security attributes */
						  TRUE, /* manual reset */
						  FALSE, /* nonsignaled */
						  NULL); /* name */
      
      accept_socket = WSASocket(AF_INET,
				SOCK_STREAM,
				0, /* protocol */
				NULL,
				0, /* reserved */
				WSA_FLAG_OVERLAPPED);
      if (accept_socket == INVALID_SOCKET)
	raise_windows_error(WSAGetLastError());
      
      stream_descriptor->socket_data.hatched_socket = accept_socket;

      maybe_grow_callback_data_buffer(callback_data,
				      /* it's what the manual says: */
				      sizeof(struct sockaddr_in) + 16
				      + sizeof(struct sockaddr_in) + 16);
      
      status = AcceptEx(socket,
			accept_socket,
			callback_data->buffer,
			0,
			sizeof(struct sockaddr_in) + 16,
			sizeof(struct sockaddr_in) + 16,
			&bytecount,
			&callback_data->overlap);
    }
  else
    /* rebound */
    {
      DWORD numberOfBytesTransferred;
      if (GetOverlappedResult((HANDLE)socket,
			      &callback_data->overlap,
			      &numberOfBytesTransferred,
			      FALSE)) /* don't block */
	{
	  accept_socket = stream_descriptor->socket_data.hatched_socket;
	  status = TRUE;
	}
      else if (GetLastError() == ERROR_IO_INCOMPLETE)
	return S48_FALSE; /* still not done */
      else
	raise_windows_error(GetLastError());
    }
			 
  /*
   * Check for a connection.
   */
  
  if (status)
    /* success */
    {
      long accept_socket_fd = allocate_fd(STREAM_SOCKET);
      stream_descriptor_t* accept_stream_descriptor = &(stream_descriptors[accept_socket_fd]);
      accept_stream_descriptor->socket_data.socket = accept_socket;

      CloseHandle(callback_data->overlap.hEvent);
      
      /* We just need to do it, or things behave ... unexpectedly.
       * Go figure.
       */

      if (setsockopt(accept_socket, 
		     SOL_SOCKET, 
		     SO_UPDATE_ACCEPT_CONTEXT, 
		     (char *)&socket, 
		     sizeof(socket))
	  != 0)
	raise_windows_error(WSAGetLastError());

      input_channel = s48_add_channel(S48_CHANNEL_STATUS_INPUT,
				      s48_enter_string("socket connection"),
				      accept_socket_fd);

      if (!S48_CHANNEL_P(input_channel))
	{
	  ps_close_fd(accept_socket_fd);
	  s48_raise_scheme_exception(s48_extract_fixnum(input_channel), 0);
	};
    
      return input_channel;
    }

  if (WSAGetLastError() == ERROR_IO_PENDING)
    {
      DWORD id;
      HANDLE thread_handle;

      if (!s48_add_pending_fd(socket_fd, TRUE))
	s48_raise_out_of_memory_error();

      stream_descriptor->socket_data.hatch_callback = accept_callback;
      thread_handle = CreateThread(NULL, /* lpThreadAttributes */
				   4096, /* dwStackSize */
				   (LPTHREAD_START_ROUTINE)hatch_thread_proc,
				   stream_descriptor,
				   0, /* dwCreationFlags, */
				   &id);
      return S48_FALSE;
    }
  
  raise_windows_error(WSAGetLastError());
  
  /* not reachable: */
  return S48_FALSE;
}

/*
 * Given an internet-domain stream socket, a machine name and a port number,
 * connect the socket to that machine/port.
 *
 * If this succeeds, it returns an output channel for the connection.
 * If it fails because the connect would block, add the socket to the
 * pending queue (for output) and return #f.
 * If it fails for any other reason, raise an exception.
 */

/*
 * We need to jump through the same hoops as with s48_accept.
 */

static VOID CALLBACK
connect_callback(DWORD dwParam)
{
  stream_descriptor_t* stream_descriptor = (stream_descriptor_t*)dwParam;
  callback_data_t* callback_data = &(stream_descriptor->callback_data);

  s48_add_ready_fd(callback_data->fd, PSFALSE, (long)0);
}

static s48_value
s48_connect(s48_value channel,
	    s48_value machine,
	    s48_value port,
	    s48_value retry_p)
{
  long socket_fd;
  unsigned short port_number;
  char *machine_name;
  struct addrinfo *address_info;
  struct sockaddr_in address;
  stream_descriptor_t* stream_descriptor;
  callback_data_t* callback_data;
  SOCKET socket;
  int status;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  stream_descriptor = &(stream_descriptors[socket_fd]);
  callback_data = &(stream_descriptor->callback_data);
  socket = stream_descriptor->socket_data.socket;

  S48_CHECK_STRING(machine);
  machine_name = S48_UNSAFE_EXTRACT_STRING(machine);
  
  S48_CHECK_FIXNUM(port);
  port_number = (unsigned short)S48_UNSAFE_EXTRACT_FIXNUM(port);
  
  /*
   * Get the host and initialize `address'.
   * ####Note that this blocks.
   */

  {
      struct addrinfo hints;
      int status;
      hints.ai_family = PF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      hints.ai_flags = AI_CANONNAME;
      hints.ai_addrlen = 0;
      hints.ai_canonname = NULL;
      hints.ai_addr = NULL;
      hints.ai_next = NULL;
      status = getaddrinfo(machine_name,
			   NULL, /* servname */
			   &hints,
			   &address_info);
      if (status != 0)
	raise_windows_error(status);
  }

  memset((void *)&address, 0, sizeof(address));

  if (address_info->ai_addrlen > sizeof(address))
    s48_raise_range_error(s48_enter_fixnum(address_info->ai_addrlen),
			  S48_UNSAFE_ENTER_FIXNUM(0),
			  s48_enter_fixnum(sizeof(address)));

  memcpy((void *)&address, (void *)address_info->ai_addr, address_info->ai_addrlen);
  freeaddrinfo(address_info);

  address.sin_port = htons(port_number);

  /*
   * Try the connection.  If it works we make an output channel and return it.
   * The original socket channel will be used as the input channel.
   */

  /*
   * ConnectEx, which works like AcceptEx, only exists on the very
   * latest Windows versions.
   */

  status = WSAConnect(socket,
		      (const struct sockaddr*)&address,
		      sizeof(address),
		      NULL, /* lpCallerData */
		      NULL, /* lpCalleeData */
		      NULL, /* lpSQOS */
		      NULL); /* lpGQOS */
  /*
   * Check for errors.  If we need to wait we set up a callback
   * and return #F to tell the Scheme procedure to try again.
   */
  if (status == 0)
    /* success */
    {
      S48_STOB_SET(channel, S48_CHANNEL_STATUS_OFFSET, S48_CHANNEL_STATUS_INPUT);
      return dup_socket_channel(socket_fd);
    }
  else if (WSAGetLastError() == WSAEWOULDBLOCK)
    {
      DWORD id;
      HANDLE thread_handle;

      if (!s48_add_pending_fd(socket_fd, FALSE))
	s48_raise_out_of_memory_error();

      stream_descriptor->socket_data.hatch_callback = connect_callback;
      thread_handle = CreateThread(NULL, /* lpThreadAttributes */
				   4096, /* dwStackSize */
				   (LPTHREAD_START_ROUTINE)hatch_thread_proc,
				   stream_descriptor,
				   0, /* dwCreationFlags, */
				   &id);
      callback_data->overlap.hEvent = CreateEvent(NULL, /* no security attributes */
						  TRUE, /* manual reset */
						  FALSE, /* nonsignaled */
						  NULL); /* name */
      if (WSAEventSelect(socket, callback_data->overlap.hEvent, FD_ACCEPT)
	  != 0)
	raise_windows_error(WSAGetLastError());
      
      return S48_FALSE;
    }
  else
    raise_windows_error(WSAGetLastError());

  /* not reachable */
  return S48_FALSE;
}

/*
 * dup() `socket_fd' and return an output channel holding the result.
 *
 * We have to versions, one for calling from C and one for calling from Scheme.
 */

static s48_value
s48_dup_socket_channel(s48_value channel)
{
  int			socket_fd;

  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  
  return dup_socket_channel(socket_fd);
}

static s48_value
dup_socket_channel(long socket_fd)
{
  long output_fd;
  SOCKET output_socket;
  s48_value output_channel;
  stream_descriptor_t* stream_descriptor = &(stream_descriptors[socket_fd]);
  callback_data_t* callback_data = &(stream_descriptor->callback_data);
  SOCKET socket = stream_descriptor->socket_data.socket;
  stream_descriptor_t* output_stream_descriptor;
  int buf_size;

  WSAPROTOCOL_INFO protocolInfo;
  if (WSADuplicateSocket(socket,
			 GetCurrentProcessId(),
			 &protocolInfo)
      == SOCKET_ERROR)
    raise_windows_error(WSAGetLastError());

  output_socket = WSASocket(AF_INET,
			    SOCK_STREAM,
			    0, /* protocol */
			    &protocolInfo,
			    0, /* reserved */
			    WSA_FLAG_OVERLAPPED);
  if (output_socket == INVALID_SOCKET)
    raise_windows_error(WSAGetLastError());
  
  buf_size = 0;
  setsockopt(output_socket,
	     SOL_SOCKET, SO_SNDBUF, 
	     (const char*)buf_size,
	     sizeof(buf_size));

  output_fd = allocate_fd(STREAM_SOCKET);
  output_stream_descriptor = &(stream_descriptors[output_fd]);
  output_stream_descriptor->socket_data.socket = output_socket;

  output_channel = s48_add_channel(S48_CHANNEL_STATUS_OUTPUT,
				   s48_enter_string("socket connection"),
				   output_fd);
  
  if (!S48_CHANNEL_P(output_channel))
    {
      ps_close_fd(output_fd);
      s48_raise_scheme_exception(s48_extract_fixnum(output_channel), 0);
    }

  return output_channel;
}

/*
 * Close half of a socket; if `input_p' is true we close the input half,
 * otherwise the output half.  This horribleness is forced upon us by
 * Windows's use of bidirectional file descriptors.
 */

static s48_value
s48_close_socket_half(s48_value channel, s48_value input_p)
{
  long socket_fd;
  stream_descriptor_t* stream_descriptor;
  callback_data_t* callback_data;
  SOCKET socket;
  
  S48_CHECK_CHANNEL(channel);
  socket_fd = S48_UNSAFE_EXTRACT_FIXNUM(S48_UNSAFE_CHANNEL_OS_INDEX(channel));
  stream_descriptor = &(stream_descriptors[socket_fd]);
  callback_data = &(stream_descriptor->callback_data);
  socket = stream_descriptor->socket_data.socket;

  /* We ignore `endpoint is not connected' errors, as we just want to get
     the file descriptor closed. */
  if ((shutdown(socket,
	       S48_EXTRACT_BOOLEAN(input_p) ? SD_RECEIVE : SD_SEND)
       == SOCKET_ERROR)
      && (WSAGetLastError() != WSAENOTCONN))
    raise_windows_error(WSAGetLastError());

  return S48_TRUE;
}

/*
 * Get the name of the local machine.
 */

static s48_value
s48_get_host_name(void)
{
  char	mbuff[4096]; /* we don't have MAXHOSTNAMELEN on Windows */

  if (gethostname(mbuff, sizeof(mbuff)) == SOCKET_ERROR)
    raise_windows_error(WSAGetLastError());

  return s48_enter_string(mbuff);
}
