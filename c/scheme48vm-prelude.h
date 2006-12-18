#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* memcpy, strlen */

#include "c-mods.h"
#include "scheme48write-barrier.h"

#define SMALL_MULTIPLY(x,y) ((x) * (y))

#define	NO_ERRORS	0		/* extension to errno.h */

#include "scheme48vm.h"
#include "scheme48heap.h"
#include "scheme48image.h"
#include "bignum.h"
#include "event.h"
#include "fd-io.h"

extern s48_value	s48_extended_vm(long, s48_value),
			s48_lookup_external_name(char *, char *),
			s48_external_call(s48_value proc,
					  s48_value proc_name,
					  long nargs,
					  char *argv);

/*
 * This comes from glue.{s,c}.
 */

extern long		s48_call_native_procedure(long, long),
			s48_invoke_native_continuation(long, long);

#define	GET_PROPOSAL_LOCK()	((void)0)
#define	RELEASE_PROPOSAL_LOCK()	((void)0)
#define SHARED_REF(x)		(x)
#define SHARED_SETB(x, v)	((x) = (v))

extern void s48_raise_argument_type_error(s48_value);
extern void s48_raise_range_error(s48_value, s48_value, s48_value);

char *s48_get_os_string_encoding(void);

/*
 * We rename these to avoid name clashes.
 */
#define TTreturn_value		s48_return_value
#define TTrun_machine(x)	s48_run_machine(x)
     
