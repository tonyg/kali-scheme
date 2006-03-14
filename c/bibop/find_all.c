/* Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include "generation_gc.h"
#include <string.h>
#include <stdlib.h>
#include "scheme48.h"
#include "areas.h"
#include "data.h"
#include "memory.h"

#define INITIAL_AREA_SIZE 4
#define AREA_SIZE_MULTIPLICATOR 2

static Area* target_area;
static unsigned long target_area_size; /* pages */

static void initialize_find() {
  target_area_size = INITIAL_AREA_SIZE;
  target_area = s48_allocate_area(target_area_size, target_area_size,
				  0, AREA_TYPE_SIZE_SMALL);
  /* reserve space for the header */
  target_area->frontier = target_area->start + S48_STOB_OVERHEAD_IN_BYTES;
}

static void add_stob(s48_value stob) {
  if (AREA_REMAINING(target_area) < S48_CELLS_TO_BYTES(1)) {
    /* if there isn't enough space, create a bigger area, copy
       everything and free the old one. */
    unsigned long size = target_area->frontier - target_area->start;
    Area* new_area;

    target_area_size = target_area_size * AREA_SIZE_MULTIPLICATOR;
    new_area = s48_allocate_area(target_area_size, target_area_size,
				 0, AREA_TYPE_SIZE_SMALL);
    memcpy((void*)new_area->start, (void*)target_area->start, size);

    new_area->frontier = new_area->start + size;
    s48_free_area(target_area);
    target_area = new_area;
  }

  *((s48_value*)target_area->frontier) = stob;
  target_area->frontier += S48_CELLS_TO_BYTES(1);
}

/* S48_GATHER_OBJECTS: The iterator FOR_EACH_OBJECT must be called
   once, it then calls it's argument for every object that should be
   looked at. Store every such object, that also fulfills PREDICATE in
   a vector, return FALSE if the gathering cannot continue - in which
   case FOR_EACH_OBJECT returns FALSE either. S48_GATHER_OBJECTS then
   returns the vector or FALSE */

static char(*gather_predicate)(s48_value);

static char look_at_object(s48_value obj) {
  if (gather_predicate(obj))
    add_stob(obj);
  return TRUE;
}

s48_value s48_gather_objects(char(*predicate)(s48_value),
			     char(*for_each_object)(char(*)(s48_value))) {
  /* sets up target_area */
  initialize_find();
  gather_predicate = predicate;

  if (for_each_object(&look_at_object)) {
    /* everything is fine, vector filled, complete it */
    s48_address addr_after_header = target_area->start + S48_STOB_OVERHEAD_IN_BYTES;
    unsigned long size = target_area->frontier - addr_after_header;
    
    *((long*)target_area->start) = S48_MAKE_HEADER(S48_STOBTYPE_VECTOR, size);

    s48_integrate_area(target_area);
    target_area = NULL;

    return S48_ADDRESS_TO_STOB_DESCRIPTOR(addr_after_header);
  } else {
    /* something went wrong, destroy area and give up */
    s48_free_area(target_area);
    target_area = NULL;

    return S48_FALSE;
  }
}

/* S48_FIND_ALL gathers everything in the heap with the given type.
   S48_FIND_ALL_RECORDS gathers all records in the heap with the given
   record type. */

static long find_type;
static s48_value find_record_type; /* S48_FALSE for s48_find_all */

static char find_predicate(s48_value stob) {
  return ( (S48_STOB_TYPE(stob) == find_type)
	   && ((find_record_type == S48_FALSE)
	       || (S48_RECORD_TYPE(stob) == find_record_type)) );
}

static char find_iterator_result;
static char(*find_iterator_look_at)(s48_value);

static void find_iterator_scan(s48_address start, s48_address end) {
  /* PROBLEM/TODO: we cannot stop s48_walk_heap. So for now just do
     nothing, if we should've already stopped. */
  if (!find_iterator_result)
    return;
  else {
    s48_address addr = start;
    while (addr < end) {
      s48_address content = addr + S48_STOB_OVERHEAD_IN_BYTES;
      s48_value stob = S48_ADDRESS_TO_STOB_DESCRIPTOR(content);
      if (! find_iterator_look_at(stob)) {
	find_iterator_result = FALSE;
	return;
      }
      addr = content + S48_BYTES_TO_A_UNITS(S48_STOB_BYTE_LENGTH(stob));
    }
  }
}

static char find_iterator(char(*look_at)(s48_value)) {
  find_iterator_result = TRUE;
  find_iterator_look_at = look_at;
  s48_walk_heap(&find_iterator_scan);
  return find_iterator_result;
}

static s48_value do_find() {
  return s48_gather_objects(&find_predicate, &find_iterator);
}

s48_value s48_find_all(long type) {
  find_type = type;
  find_record_type = S48_FALSE;
  return do_find();
}

s48_value s48_find_all_records(s48_value record_type) {
  find_type = S48_STOBTYPE_RECORD;
  find_record_type = record_type;
  return do_find();
}
