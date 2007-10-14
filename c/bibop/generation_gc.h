/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_GC_GENERATION_GC_H
#define __S48_GC_GENERATION_GC_H

#include "scheme48.h"
#include "scheme48vm.h"
#include "memory.h"
#include "page_constants.h"
#include "areas.h"

extern long s48_heap_size(void);
extern char s48_extantP(s48_value thing);
extern void s48_collect(psbool);
extern long s48_gc_count(void);
extern void s48_trace_locationsB(s48_address start, s48_address end);
extern s48_value s48_trace_value(s48_value stob);
extern void s48_trace_stob_contentsB(s48_value stob);

extern void s48_forbid_gcB();
extern void s48_allow_gcB();

extern long s48_available();

void s48_make_availableAgc(long len_in_bytes);
s48_address s48_allocate_small(long len_in_bytes);

s48_address s48_allocate_tracedAgc(long len_in_bytes);
s48_address s48_allocate_untracedAgc(long len_in_bytes);
s48_address s48_allocate_weakAgc(long len_in_bytes);
s48_address s48_allocate_untraced_unmovableAgc(long len_in_bytes);

extern void s48_initialize_bibop_heap();

extern void s48_integrate_area(Area* area);

/* only used "internally" */

void s48_internal_trace_locationsB(Area* maybe_area, s48_address start,
				   s48_address end, char* called_from);
extern void s48_walk_heap(void (*do_part)(s48_address, s48_address));

#endif
