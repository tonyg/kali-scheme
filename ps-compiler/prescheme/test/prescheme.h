
#define PS_GETC(PORT,RESULT)      RESULT = getc(PORT);
/* optional definition for Unix's that are even more losing than usual
{                                  \
  FILE * TTport = PORT;            \
  RESULT = getc(TTport);           \
  if (EOF == RESULT)               \
    clearerr(TTport);              \
}
*/

/* C shifts may not work if the amount is greater than the machine word size */

#define PS_SHIFT_RIGHT(X,Y,RESULT)   \
{                                    \
  long TTy = Y;                      \
  RESULT = TTy >= 32 ? 0 : X >> TTy; \
}  

#define PS_SHIFT_LEFT(X,Y,RESULT)    \
{                                    \
  long TTy = Y;                      \
  RESULT = TTy >= 32 ? 0 : X << TTy; \
}  

extern long TTreturn_value, TTrun_machine();
