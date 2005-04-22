/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/* Implements area_roots.h */

#include <stdlib.h>

#include "scheme48.h"

#include "area_roots.h"
#include "areas.h"
#include "memory.h"
#include "memory_map.h"
#include "utils.h"
#include "data.h"
#include "measure.h"
#include "generation_gc.h"
#include "gc_config.h"

#if (S48_USE_REMEMBERED_SETS)
#include "remset.h"
#endif

/* initializes the dirty vector of AREA */
void s48_init_dirty_vector(Area* area) {
  Dirty_vector* dv = &area->dirty_vector;
  unsigned long area_size = area->end - area->start;

  /* The number of logical cards is the division of area_size through
     the logical card_size (number of 32-bit-addresses). Bitwise Division:
     shift right */
  unsigned long number_of_cards = area_size >> S48_LOG_CARD_SIZE ;
  
  dv->length = number_of_cards;

  /* Space allocation and initialisation */
#if S48_DIRTY_VECTOR_METHOD==S48_OFFSET_DIRTY_VECTORS
  /* A vector of bytes (offset is max 2^9 - 1 = 511 => max logical card size = 9) */
  dv->items = (char*)calloc(sizeof(char), number_of_cards);
#endif
#if S48_DIRTY_VECTOR_METHOD==S48_ADDRESS_DIRTY_VECTORS
  /* A vector of double words */
  dv->items = (s48_address*)calloc(sizeof(s48_address), number_of_cards);
#endif
#if S48_DIRTY_VECTOR_METHOD==S48_CROSSINGMAP_DIRTY_VECTORS
    /* This method uses two bits per card. One bit marks a card as
       beeing dirty, the second marks a card as beeing tracable, which
       means that the card does not begin within a B-vector (A string
       for example). We use a full char to store the bit. */
  dv->dirty_bits = (char*)calloc(sizeof(char), number_of_cards);
  dv->traceable_bits = (char*)calloc(sizeof(char), number_of_cards);
  dv->last_frontier = area->frontier;
#endif

#if (S48_USE_CARD_GENERATION_INDEXING)
  dv->minimum_index = (unsigned char*)malloc(sizeof(char) * number_of_cards);
  for (i = 0; i < number_of_cards; i++)
    dv->minimum_index[i] = 255;
#endif
}

/* deinitializes the dirty vector of AREA. (should free the memory
   allocated in init_dirty_vector */
void s48_deinit_dirty_vector(Area* area) {
  Dirty_vector* dv = &area->dirty_vector;

#if S48_DIRTY_VECTOR_METHOD==S48_OFFSET_DIRTY_VECTORS
  free(dv->items);
#endif
#if S48_DIRTY_VECTOR_METHOD==S48_ADDRESS_DIRTY_VECTORS
  free(dv->items);
#endif
#if S48_DIRTY_VECTOR_METHOD==S48_CROSSINGMAP_DIRTY_VECTORS
  free(dv->dirty_bits);
  free(dv->traceable_bits);
#endif
}

#if (MEASURE_GC)
static unsigned long areas_visited = 0;
static unsigned long areas_passed = 0;
#endif

/* FPage 9 */
/* passes all dirty regions in all areas in the linked list starting
   with AREAS, to trace_locationsB */

void s48_trace_areas_roots(Area* areas, int generations_count) {
  while(areas != NULL) {
    Dirty_vector* dirty_vector = &areas->dirty_vector;
    s48_address frontier = areas->frontier;
    s48_address card_start_address = areas->start;
    s48_address start_address, end_address;
    int i;

#if S48_DIRTY_VECTOR_METHOD==S48_CROSSINGMAP_DIRTY_VECTORS
    /* This method uses two bits per card. One bit marks a card as
       beeing dirty, the second marks a card as beeing tracable, which
       means that the card does not begin within a B-vector (A string
       for example). We use a full char to store the bit. */
    char* dirty_bits = dirty_vector->dirty_bits;
    char* traceable_bits = dirty_vector->traceable_bits;

    /* if new objects were allocated since the last trace, then scan
       the new part of the area to find out which of the new cards are
       traceable */
    s48_address start = areas->start;
    s48_address addr = dirty_vector->last_frontier;

    while (addr < frontier) {
      s48_value v = *((s48_value*)addr);
      unsigned long offset = addr - start;
      if ((offset % S48_CARD_SIZE) == 0) {
	traceable_bits[offset >> S48_LOG_CARD_SIZE] = TRUE;
      }
      else {
	traceable_bits[offset >> S48_LOG_CARD_SIZE] = FALSE;
      }

      if (S48_B_VECTOR_HEADER_P(v))
	addr = addr + S48_STOB_OVERHEAD_IN_A_UNITS +
	  S48_HEADER_LENGTH_IN_A_UNITS(v);
      else
	addr = S48_ADDRESS_INC(addr);
    }
    /* remember that we have looked at this part of the area */
    dirty_vector->last_frontier = frontier;
#endif
    
    
#if (MEASURE_GC)
    areas_visited += areas->frontier - areas->start;
#endif
    
    /* MAIN SCANNING OF THE DIRTY VECTOR */
    /* Search through the whole dirty_vector */
    for (i = 0; i < dirty_vector->length;
	 i++, card_start_address += S48_CARD_SIZE) {
      
#if (S48_USE_CARD_GENERATION_INDEXING)
      /* we can skip this card, if it doesn't contain pointers to a
	 generation that we're collected right now. */
      if (dirty_vector->minimum_index[i] >= generations_count)
	continue;
#endif
      
#if S48_DIRTY_VECTOR_METHOD==S48_OFFSET_DIRTY_VECTORS
      dirty_byte = dirty_vector->items[i];
      if (dirty_byte != 0) {
	/* Get the relative offset in addressable units to the start
	   address of the i-th card */ 
	unsigned char card_offset = dirty_byte >> 1;
	
	/* Get the absolute start_address and end_address of the i-th
	   card */
	start_address = card_start_address + S48_CELLS_TO_BYTES(card_offset);
	end_address = card_start_address + S48_CARD_SIZE;
	
	/* reset to 0. internal_write_barrier will be called to mark
	   the still dirty cards. */
	dirty_vector->items[i] = 0;
#endif
#if S48_DIRTY_VECTOR_METHOD==S48_ADDRESS_DIRTY_VECTORS
	start_address = dirty_vector->items[i];
	if (start_address != NULL) {
	  end_address = card_start_address + S48_CARD_SIZE;
	  dirty_vector->items[i] = NULL;
#endif	  
#if S48_DIRTY_VECTOR_METHOD==S48_CROSSINGMAP_DIRTY_VECTORS
	  if (dirty_bits[i]) {
	    int j = i;
	    
	    /* search backwards for a traceable card (the first one must
	       be a traceable one). */
	    start_address = card_start_address;
	    end_address = card_start_address + S48_CARD_SIZE;
	    while ((j > 0) && (!traceable_bits[j])) {
	      j--;
	      start_address = start_address - S48_CARD_SIZE;
	    }
	    dirty_bits[i] = FALSE;
	    /* a possible optimization would be to avoid tracing a card
	       twice completely, e.g. by tracing backwards from the end to
	       beginning, or to search forward for more consecutive dirty
	       cards. */
#endif
	    
#if (S48_USE_CARD_GENERATION_INDEXING)
	    /* clear the minimum_index too */
	    dirty_vector->minimum_index[i] = 255;
#endif
	    
	    /* adjust/control addresses */
	    if (end_address > frontier) end_address = frontier;
	    if (start_address > frontier)
	      /* i'm not sure if this just has to be ignored... */
	      s48_gc_error("s48_trace_areas_roots: dirty bit set behind frontier.");
	    
#if (MEASURE_GC)
	    areas_passed += (end_address - start_address);
#endif
	    
	    s48_internal_trace_locationsB(areas, start_address, end_address, "s48_trace_areas_roots");
	  }
	}
	
	areas = areas->next;
      }
      
#if (MEASURE_GC)
      measure_areas_roots(areas_visited, areas_passed);
      areas_visited = 0;
      areas_passed = 0;
#endif
      
    }
    
void s48_set_dirty_vector(Area* area, s48_address addr, long stob,
			  Area* maybe_to_area) {
  s48_set_dirty_vector_inline(area, addr, stob, maybe_to_area);
}
 
void s48_write_barrier(long stob, s48_address address, long value) {
  s48_write_barrier_inline(stob, address, value);
}
