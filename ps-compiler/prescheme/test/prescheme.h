#include <errno.h>

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

extern FILE *ps_open_input_file(unsigned char *, long *);
extern FILE *ps_open_output_file(unsigned char *, long *);
extern long ps_close(FILE *);
extern char ps_read_char(FILE *, unsigned char *, long *, char);
extern long ps_read_integer(FILE *, unsigned char *, long *);
extern long ps_write_char(char, FILE *);
extern long ps_write_integer(long, FILE *);
extern long ps_write_string(unsigned char *, FILE *);
extern long ps_read_block(FILE *, char *, long, unsigned char *, long *);
extern long ps_write_block(FILE *, char *, long);
extern unsigned char *ps_error_string(long);
extern void ps_error(unsigned char *, ...);

/* C shifts may not work if the amount is greater than the machine word size */
/* Patched by JAR 6/6/93 */

#define PS_SHIFT_RIGHT(X,Y,RESULT)   \
{                                    \
  long TTx = X,  TTy = Y;            \
  RESULT = TTy >= 32 ? (TTx < 0 ? -1 : 0) : TTx >> TTy; \
}  

#define PS_SHIFT_LEFT(X,Y,RESULT)    \
{                                    \
  long TTy = Y;                      \
  RESULT = TTy >= 32 ? 0 : X << TTy; \
}  

extern long TTreturn_value, TTrun_machine();

