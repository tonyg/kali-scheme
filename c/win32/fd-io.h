/* Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

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

struct callback_data {
  /* so we can pass this as LPOVERLAPPED; must be at the beginning */
  OVERLAPPED overlap;
  long fd;
  CHAR* buffer;
  CHAR* current;
  size_t current_size, max_size;
};

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
  
  }

  struct callback_data callback_data;

} stream_descriptor_t;

