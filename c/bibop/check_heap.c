/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include <stdio.h>
#include "scheme48.h"
#include "generation_gc.h"
#include "utils.h"
#include "data.h"
#include "memory.h"
#include "memory_map.h"

/* how many errors are left before program termination */
static long errors_left;

char s48_stob_in_heapP(s48_value stob) {
  s48_address addr = S48_ADDRESS_AT_HEADER(stob);
  Area* area = s48_memory_map_ref(addr);
  return ((area != NULL) && (area->start <= addr) && (addr < area->frontier));
}

static void error_found(char* message) {
  fprintf(stderr, "check heap: ");
  fprintf(stderr, message);
  fprintf(stderr, "\n");
  if (errors_left == 0) {
    s48_gc_error("check heap: too many errors!");
  } else {
    errors_left--;
  }
}

static void try_describe_area_at(s48_address addr) {
  Area* a = s48_memory_map_ref(addr);
  fprintf(stderr, "Area containing 0x%X", addr);
  if (a != NULL) {
    fprintf(stderr, "\n  0x%X-[0x%X]-0x%X\n", a->start, a->frontier, a->end);
    fprintf(stderr, "  generation: %d\n", a->generation_index);
    fprintf(stderr, "  type: %d\n", a->area_type_size);
    fprintf(stderr, "  %s last in chain\n", a->next ? "not" : "");
  }
  else
    fprintf(stderr, " could not be found\n");
}

static void check_area(s48_address start, s48_address end) {
  s48_address addr = start;
  while (addr < end) {
    long header = *((long*)addr);
    if (!S48_HEADER_P(header)) {
      char s[512];
      try_describe_area_at(addr);
      sprintf(s, "corrupted header 0x%X at 0x%X!", header, addr);
      error_found(s);
      addr = S48_ADDRESS_INC(addr);
    } else {
      s48_address next = addr + S48_STOB_OVERHEAD_IN_A_UNITS
	+ S48_HEADER_LENGTH_IN_A_UNITS(header);
      if (!S48_B_VECTOR_HEADER_P(header)) {
	s48_value v; int i;
	int  len = S48_HEADER_LENGTH_IN_CELLS(header);
	s48_value* this_addr = ((s48_value*)addr); /* increased in the first loop */
	for (i = 0; i < len; i++) {
	  this_addr++;
	  v = *this_addr;
	  if ( S48_HEADER_P(v) ) {
	    char s[512];
	    sprintf(s, "content value 0x%X at 0x%X is a header!",
		    v, this_addr);
	    error_found(s);
	  } else if ( S48_STOB_P(v) && (!s48_stob_in_heapP(v)) ) {
	    char s[512];
	    sprintf(s, "stob value 0x%X in object of type %d at 0x%X pointing outside the heap!", v, S48_HEADER_TYPE(header), this_addr);
	    error_found(s);
	  }
	}
      }
      addr = next;
    }
  }
}

/* error_count : maximal errors allowed */

char s48_check_heap(long error_count) {
  errors_left = error_count;
  s48_walk_heap(&check_area);
  /* If errors_left ist not decremented
     means there was no error => s48_check_heap = TRUE */
  return (errors_left == error_count);
}
