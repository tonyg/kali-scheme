/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include "io.h"
#include "scheme48.h"
#include "unix.h"
#include "c-mods.h"

/* read a character while ignoring interrupts */

#define READ_CHAR(PORT,RESULT)				\
{							\
  FILE * TTport = PORT;					\
  int TTchar;						\
  if (EOF == (TTchar = getc(TTport)))			\
    RESULT = s48_read_char(TTport);			\
  else							\
    RESULT = TTchar; 					\
}

/*
Helper procedure for the READ_CHAR macro.  If the getc was interrupted
we clear the error bit and try again.
*/

int
s48_read_char(FILE *port)
{
  int result;

  while(1) {
    if (ferror(port) && errno == EINTR) {
      clearerr(port);
      result = getc(port);
      if (EOF != result)
	return result; }
    else
      return EOF; }
}

/* called when getc(port) returned EOF */

char
ps_read_char(FILE *port, psbool *eofp, long *status, psbool peekp)
{
  psbool errorp;
  int result;

  result = s48_read_char(port);     /* read past any interruptions */
  if (result != EOF) {
    if (peekp)
      ungetc(result, port);
    *eofp = PSFALSE;
    *status = NO_ERRORS;
    return result; }
  else {
    errorp = ferror(port);
    clearerr(port);
    if (errorp) {
      *eofp = PSFALSE;
      *status = errno;
      return 0; }
    else {
      *eofp = PSTRUE;
      *status = NO_ERRORS;
      return 0; } }
}

long
ps_read_integer(FILE *port, psbool *eofp, long *status)
{
  long result;
  int ch;
  psbool negate;
  psbool errorp;

  /* eat whitespace */
  do { READ_CHAR(port, ch); }
  while (ch == ' ' || ch == '\t' || ch == '\n');

  /* read optional sign */
  if (ch == '-') {
    negate = PSTRUE;
    READ_CHAR(port, ch); }
  else
    negate = PSFALSE;

  if (ch < '0' || '9' < ch) {
    if (ch != EOF) {
      *eofp = PSFALSE;
      *status = EINVAL; }  /* has to be something */
    else {
      errorp = ferror(port);
      clearerr(port);
      if (errorp) {
	*eofp = PSFALSE;
	*status = errno; }
      else {
	*eofp = PSTRUE;
	*status = 0; } }
    result = 0; }
  else {
    result = ch - '0';
    while(1) {
      READ_CHAR(port, ch);
      if (ch < '0' || '9' < ch)
	break;
      result = (10 * result) + (ch - '0'); }
    if (ch != EOF)
      ungetc(ch, port);
    *eofp = PSFALSE;
    *status = 0; }
  return (negate ? -result : result);
}

/* write a character regardless of interrupts */

#define WRITE_CHAR(CH,PORT,RESULT)			\
{							\
  char TTch = CH;					\
  FILE * TTport = PORT;					\
  if (putc(TTch, TTport) != EOF)			\
    RESULT = 0;						\
  else							\
    RESULT = ps_write_char(TTch, TTport);		\
}

/* called when putc(char, port) returned EOF */

long
ps_write_char(char ch, FILE *port)
{

  while(1) {
    clearerr(port);
    if (errno != EINTR)
      return errno;
    else if (putc(ch, port) != EOF)
      return 0; }
}

static long
write_integer(unsigned long n, FILE *port)
{
  char ch;
  long status;

  if (n == 0)
    status = 0;
  else {
    status = write_integer(n / 10, port);
    if (status == 0) {
      ch = (n % 10) + '0';
      WRITE_CHAR(ch, port,status); } }
  return status;
}

long
ps_write_integer(long n, FILE *port)
{
  int status;

  if (n == 0) {
    WRITE_CHAR('0', port, status);
    return status; }
  else if (n > 0)
    return write_integer(n, port);
  else {
    WRITE_CHAR('-', port, status);
    if (status == 0)
      return write_integer(- n, port);
    else
      return status; }
}

long
ps_write_string(char *string, FILE *port)
{
	while (1) {
		if (EOF != fputs(string, port))
			return (0);
		clearerr(port);
		if (errno != EINTR)
			return (errno);
	}
}

long
ps_read_block(FILE *port, char *buffer, long count, psbool *eofp, long *status)
{
  int got = 0;
  psbool errorp;

  while(1) {
    got += fread(buffer, sizeof(char), count - got, port);
    if (got == count) {
      *eofp = PSFALSE;
      *status = NO_ERRORS;
      return got;}
    else if (ferror(port) && errno == EINTR)
      clearerr(port);
    else {
      *eofp = feof(port);
      errorp = ferror(port);
      clearerr(port);
      if (errorp)
	*status = errno;
      else
	*status = NO_ERRORS;
      return got;} };
}

long
ps_write_block(FILE *port, char *buffer, long count)
{
  int sent = 0;

  while(1) {
    sent += fwrite(buffer, sizeof(char), count - sent, port);
    if (sent == count)
      return NO_ERRORS;
    else if (ferror(port) && errno == EINTR)
      clearerr(port);
    else {
      clearerr(port);
      return errno; } }
}

void
ps_error(char *message, long count, ...)
{
  va_list ap;
  extern long opcode_count;

  va_start(ap, count);
  fputs(message, stderr);
  for(; count > 0; --count)
    fprintf(stderr, " %ld", va_arg(ap, long));
  putc('\n', stderr);
  exit(-1);
}

static FILE *
ps_really_open_file(char *filename, long *status, char *mode)
{
#define FILE_NAME_SIZE 1024

  char filename_temp[FILE_NAME_SIZE];
  char *expanded;
  extern char *s48_expand_file_name(char *, char *, int);

  FILE *new_file;

  expanded = s48_expand_file_name(filename, filename_temp, FILE_NAME_SIZE);
  if (expanded == NULL) {
    *status = EDOM;    /* has to be something */
    return NULL; }

  RETRY_NULL(new_file, fopen(expanded, mode));

  if (new_file == NULL) {
    *status = errno;
    return NULL; }

  *status = NO_ERRORS;
  return new_file;
}

FILE *
ps_open_input_file(char *name, long *status)
{
  return ps_really_open_file(name, status, "r");
}

FILE *
ps_open_output_file(char *name, long *status)
{
  return ps_really_open_file(name, status, "w");
}

long
ps_close(FILE *stream)
{
  int status;

  RETRY_NEG(status, fclose(stream));

  if (0 == status)
    return 0;
  else
    return errno;
}
