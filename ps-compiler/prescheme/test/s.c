
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

long
ps_select( fd_set *in, fd_set *out, int block_p )
{
  struct timeval timeout;
  long ret;
  timerclear(&timeout);

  ret = select(FD_SETSIZE, in, out, NULL, &timeout);

/*
  ret = select(FD_SETSIZE, in, out, NULL, NULL);

  printf("select() returned %ld\n", ret);
*/
  return ret;
}


int char_ready_p( FILE* stream )
{
  fd_set readfds;
  struct timeval timeout;
  static int warnedp = 0;

  if (feof(stream))
    return EOF;

  if (stream->_cnt)
    { printf("char is ready\n");
    return 1;
    }
  else
    return 0;
}
