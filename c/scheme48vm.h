#include <stdlib.h>
#include <stdio.h>

#define SMALL_MULTIPLY(x,y) ((x) * (y))

#define	bool		char		/* boolean type */
#define	TRUE		(0 == 0)
#define	FALSE		(! TRUE)
#define	NO_ERRORS	0		/* extension to errno.h */

typedef long	scheme_value;

extern scheme_value	extended_vm(long, scheme_value),
			lookup_external_name(long, long);

#include "scheme48heap.h"
#include "event.h"
#include "fd-io.h"


/*
 * The following are hand-written macro versions of procedures
 * in scheme48heap.c.
 */

extern long	ShpS,
		SlimitS;

#define	AVAILABLEp(cells)		(ShpS + ((cells)<<2) < SlimitS)


static long	HHallocate_temp;

#define	ALLOCATE_SPACE(type, len)		\
		(HHallocate_temp = ShpS,	\
			ShpS += ((len)+3) & ~3,	\
			HHallocate_temp)


#define	WRITE_BARRIER(address, value)	((void)0)
