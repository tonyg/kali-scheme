#include <errno.h>
#include "io.h"
#include "scheme48arch.h"

#if SIZEOF_VOID_P == 4
#define BITS_PER_CELL 32
#elif SIZEOF_VOID_P == 8
#define BITS_PER_CELL 64
#else
#error "What size are your pointers, really?"
#endif

#define PS_READ_CHAR(PORT,RESULT,EOFP,STATUS)		\
{							\
  FILE * TTport = PORT;					\
  int TTchar;						\
  if (EOF == (TTchar = getc(TTport)))			\
    RESULT = ps_read_char(TTport, &EOFP, &STATUS, 0==1);\
  else {						\
    RESULT = TTchar; 					\
    EOFP = 0;						\
    STATUS = 0; }					\
}

#define PS_PEEK_CHAR(PORT,RESULT,EOFP,STATUS)		\
{							\
  FILE * TTport = PORT;					\
  int TTchar;						\
  if (EOF == (TTchar = getc(TTport)))			\
    RESULT = ps_read_char(TTport, &EOFP, &STATUS, 0==0);\
  else {						\
    RESULT = TTchar; 					\
    ungetc(RESULT, TTport);				\
    EOFP = 0;						\
    STATUS = 0; }					\
}

#define PS_READ_INTEGER(PORT,RESULT,EOFP,STATUS)	\
RESULT = ps_read_integer(PORT,&EOFP,&STATUS);

#define PS_WRITE_CHAR(CHAR,PORT,STATUS)			\
{							\
  FILE * TTport = PORT;					\
  char TTchar = CHAR;					\
  if (EOF == putc(TTchar,TTport))			\
    STATUS = ps_write_char(TTchar,TTport);		\
  else {						\
    STATUS = 0; }					\
}


/* C shifts may not work if the amount is greater than the machine word size */
/* Patched by JAR 6/6/93 */

#define PS_SHIFT_RIGHT(X,Y,RESULT)   \
{                                    \
  long TTx = X,  TTy = Y;            \
  RESULT = TTy >= BITS_PER_CELL ? (TTx < 0 ? -1 : 0) : TTx >> TTy; \
}  

#define PS_SHIFT_LEFT(X,Y,RESULT)    \
{                                    \
  long TTy = Y;                      \
  RESULT = TTy >= BITS_PER_CELL ? 0 : X << TTy; \
}  

#define PS_SHIFT_RIGHT_LOGICAL(X,Y,RESULT) \
{                                          \
  RESULT = ((unsigned long) X) >> Y;	   \
}

extern long s48_return_value, s48_run_machine();

