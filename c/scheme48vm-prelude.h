#include <stdlib.h>
#include <stdio.h>

#include "c-mods.h"
#include "write-barrier.h"

#define SMALL_MULTIPLY(x,y) ((x) * (y))

#define	NO_ERRORS	0		/* extension to errno.h */

#include "scheme48vm.h"
#include "scheme48heap.h"
#include "event.h"
#include "fd-io.h"

extern s48_value	s48_extended_vm(long, s48_value),
			s48_lookup_external_name(char *, char *),
			s48_external_call(s48_value proc,
					  s48_value proc_name,
					  long nargs,
					  char *argv);
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

/*
 * We rename these to avoid name clashes.
 */
#define TTreturn_value		s48_return_value
#define TTrun_machine(x)	s48_run_machine(x)
     
