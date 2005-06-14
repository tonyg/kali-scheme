#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* memcpy, strlen */

#include "scheme48/libprescheme/c-mods.h"
#include "scheme48/libscheme48/scheme48write-barrier.h"

#define SMALL_MULTIPLY(x,y) ((x) * (y))

#define	NO_ERRORS	0		/* extension to errno.h */

#include "scheme48/libscheme48/scheme48vm.h"
#include "scheme48/libscheme48/scheme48heap.h"
#include "scheme48/libscheme48/scheme48image.h"
#include "scheme48/libscheme48/bignum.h"
#include "scheme48/libscheme48/event.h"
#include "scheme48/libprescheme/fd-io.h"

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
			s48_invoke_native_continuation(long);

/*
 * The following are hand-written macro versions of procedures
 * in scheme48heap.c.
 */

#define	AVAILABLEp(cells)	(s48_ShpS + ((cells)<<2) < s48_SlimitS)

static char	*_HHallocate_temp;

#define	ALLOCATE_SPACE(type, len)		\
		(_HHallocate_temp = s48_ShpS,	\
			s48_ShpS += ((len)+3) & ~3,	\
			_HHallocate_temp)


#define	GET_PROPOSAL_LOCK()	((void)0)
#define	RELEASE_PROPOSAL_LOCK()	((void)0)
#define SHARED_REF(x)		(x)
#define SHARED_SETB(x, v)	((x) = (v))

/*
 * We rename these to avoid name clashes.
 */
#define TTreturn_value		s48_return_value
#define TTrun_machine(x)	s48_run_machine(x)
     