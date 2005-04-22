/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include "utils.h"
#include "areas.h"
#include "page_constants.h"
#include "memory_map.h"
#include "area_roots.h"
#include "page_alloc.h"
#include "gc_config.h"
#include "remset.h"

static Area* make_area(s48_address start, s48_address end,
		       s48_address frontier, 
		       unsigned char generation_index,
		       area_type_size_t area_type_size) {
  Area* area = (Area*)malloc(sizeof(Area));
  if (area == NULL) s48_gc_error("make_area: out of memory");
  area->start = start;
  area->end = end;
  area->frontier = frontier;
  area->trace = start;
  area->next = NULL;
  area->generation_index = generation_index;
  area->area_type_size = area_type_size;
#if S48_USE_GENERATION_INDEXING==TRUE
  area->minimum_index = 255;
  area->maximum_index = 0;
#endif
  s48_init_dirty_vector(area);
#if S48_USE_REMEMBERED_SETS==TRUE
  area->remset = s48_make_remset();
#endif
  return area;
}

inline static void free_area(Area* area) {
  s48_deinit_dirty_vector(area);
#if S48_USE_REMEMBERED_SETS==TRUE
  s48_free_remset(area->remset);
#endif
  free(area);
}

/* delete_area deletes AREA from the linked list starting with START,
   and returns the (eventually different) new start of the list. Does
   NOT free the memory allocated for the area structure (nor the area
   itself)! */

Area* s48_delete_area(Area* start, Area* area) {
  if (start == NULL)
    /* no areas in the list? -> Done */
    return start;
  else if (start == area) {
    /* list starts with AREA? -> next is new start */
    Area* next = area->next;
    area->next = NULL;
    return next;
  } else {
    /* search for the area before AREA */
    Area* prev = start;
    /* if AREA is not in the list, prev->next can be NULL. But of
       corse this should never happen !? */
    while (prev->next != NULL) {
      if (prev->next == area) {
	prev->next = area->next;
	break;
      }
      prev = prev->next;
    }
    area->next = NULL;
    return start;
  }
}

/* Allocate an area of between MINIMUM and MAXIMUM pages, inclusive. */
    
Area* s48_allocate_area(unsigned int minimum, unsigned int maximum,
			unsigned char generation_index, area_type_size_t area_type_size) {
  s48_address start;
  Area* area;
  unsigned int i;
  unsigned int size = s48_allocate_pages(minimum, maximum, &start);
#if (BIBOP_LOG)
  s48_bibop_log("s48_allocate_pages: size = %i",
	    size);
#endif
  area = make_area(start, ADD_PAGES(start, size), start,
			 generation_index, area_type_size);
  /* The area is put into all memory-map cells that are covered by
     it. */
  for (i = 0; i < size; i++)
    s48_memory_map_setB(ADD_PAGES(start, i), area);

  return area;
}

/* Free the pages covered by AREA, and free the struct itself too. */

void s48_free_area(Area* area) {
  unsigned int size = BYTES_TO_PAGES(area->end - area->start);
  s48_address start = area->start;
  unsigned int i;
  
  s48_free_pagesB(start, size);

  /* This is not really needed, I think. It's only a waste of time */
  for (i = 0; i < size; i++) {
    s48_memory_map_setB(ADD_PAGES(start, i), NULL);
  }

  free_area(area);
}

/* Call s48_free_area on all areas in the list starting with START */
void s48_free_areas(Area* start) {
  while (start != NULL) {
    Area* next = start->next;
    s48_free_area(start);
    start = next;
  }
}

/* Get the type size of this stob's area: small, large, weaks. Called
   from the BIBOP dumper */
area_type_size_t s48_area_type_size(s48_value stob) {
  Area* area;
  area = s48_memory_map_ref((s48_address)stob);

  if (area == NULL) {
    return AREA_TYPE_SIZE_ILLEGAL;
  }

  return area->area_type_size;
}


/* Allocate a block for the whole image */
void s48_allocate_image_area(long bytes, s48_address* start, s48_address* end) {
  
  s48_address memory;
  
  memory = (s48_address)malloc(bytes);
  
  if (memory == NULL) s48_gc_error("s48_allocate_image_area: out of memory\n");
  
  *start = PAGE_START_ADDRESS(memory);
  *end = PAGE_START_ADDRESS(*start + bytes);
  
  return;
}

/* Wrap the static make_area */
Area* s48_make_area(s48_address start, s48_address end,
		    s48_address frontier, 
		    unsigned char generation_index,
		    area_type_size_t area_type_size) {
  Area* res;

  res = make_area(start, end, frontier, generation_index, area_type_size);
  return res;
}
