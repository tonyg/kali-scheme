
#include <errno.h>

#define PS_GETC(PORT,RESULT)      /* RESULT = getc(PORT); */ \
/* optional definition for Unixes that are even more losing than usual */ \
{                                          \
  FILE * TTport = PORT;			   \
  int errorp;				   \
  while (EOF == (RESULT = getc(TTport))	   \
	 && (errorp = ferror(TTport),	   \
	     clearerr(TTport),		   \
	     (errorp && errno == EINTR)))  \
    ;					   \
}
/* The trouble with the above is that all read errors look like
   EOF's.  */

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
extern FILE *ps_open(char *, char *);
