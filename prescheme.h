
#include <errno.h>

#define PS_GETC(PORT,RESULT)      /* RESULT = getc(PORT); */ \
/* optional definition for Unix's that are even more losing than usual */ \
{                                  \
  FILE * TTport = PORT;            \
  RESULT = getc(TTport);           \
  if (EOF == RESULT)               \
    clearerr(TTport);              \
}

/* How to fix alarm interrupt retry problem?
  while (EOF == (RESULT = getc(TTport)) \
	 && (clearerr(TTport), errno == EINTR)) \
    ; \
 */

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
