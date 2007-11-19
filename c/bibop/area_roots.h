/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_AREA_ROOTS
#define __S48_AREA_ROOTS

#include "areas.h"
#include "memory.h"
#include "memory_map.h"
#include "scheme48.h"
#include "measure.h"

/* Cards & Dirty Vector */

#define S48_CARD_SIZE (1 << S48_LOG_CARD_SIZE)

void s48_init_dirty_vector(Area* area);
void s48_deinit_dirty_vector(Area* area);

void s48_trace_areas_roots(Area* areas, int generations_count);

/* implementation of the write-barrier */

void s48_set_dirty_vector(Area* area, s48_address addr, long stob,
			  Area* maybe_to_area);

/* Marks the card containing addr as dirty. called from
   s48_write_barrier and s48_internal_write_barrier */ 
inline static void s48_set_dirty_vector_inline(Area* area, s48_address addr,
					       long stob, Area* maybe_to_area)
{
  unsigned long area_offset = addr - area->start;
  unsigned int card_number = area_offset >> S48_LOG_CARD_SIZE ;
  
#if S48_DIRTY_VECTOR_METHOD==S48_OFFSET_DIRTY_VECTORS
  /* the offset of ADDR within the card */
  unsigned char card_offset = \
    S48_BYTES_TO_CELLS(area_offset - (card_number << S48_LOG_CARD_SIZE));
  /* we need to use one bit, to distinguish between a clean card, and
     offset 0 */
  unsigned char new_dirty_byte  = (card_offset << 1) | 1 ;
  /* update the dirty_vector, if card was clean, or the new offset is
     smaller */
  unsigned char old_dirty_byte = area->dirty_vector.items[card_number];
  if ( (old_dirty_byte == 0) || (new_dirty_byte < old_dirty_byte) )
    area->dirty_vector.items[card_number] = new_dirty_byte;
#endif
#if S48_DIRTY_VECTOR_METHOD==S48_ADDRESS_DIRTY_VECTORS
  s48_address current_dirty_address = area->dirty_vector.items[card_number];
  /* Only update if Nullpointer or smaller than current_dirty_address */
  if ((current_dirty_address == NULL) || (addr < current_dirty_address))
    area->dirty_vector.items[card_number] = addr;
#endif
#if S48_DIRTY_VECTOR_METHOD==S48_CROSSINGMAP_DIRTY_VECTORS
  area->dirty_vector.dirty_bits[card_number] = TRUE;
#endif

#if (S48_USE_CARD_GENERATION_INDEXING)
#if S48_WRITE_BARRIER_COMPLEXITY == S48_MUTATED_LOCATION
#error "Generation-indexing cannot be used with write-barrier-complexity: MUTATED_LOCATION."
#endif
  if (maybe_to_area == NULL)
    maybe_to_area = s48_memory_map_ref((s48_address)stob);
  area->dirty_vector.minimum_index[card_number] =
    min(area->dirty_vector.minimum_index[card_number],
	maybe_to_area->generation_index);
#endif

#if (S48_USE_GENERATION_INDEXING)
#if S48_WRITE_BARRIER_COMPLEXITY == S48_MUTATED_LOCATION
#error "Generation-indexing cannot be used with write-barrier-complexity: MUTATED_LOCATION."
#endif
  if (maybe_to_area == NULL)
    maybe_to_area = s48_memory_map_ref((s48_address)stob);
  maybe_to_area->minimum_index = int_min(maybe_to_area->minimum_index,
					 area->generation_index);
  maybe_to_area->maximum_index = int_max(maybe_to_area->maximum_index,
					 area->generation_index);
#endif
}

extern void s48_write_barrier(long stob, s48_address address, long value);

inline static void s48_write_barrier_inline(long stob, s48_address address,
					    long value) {
  /* The area of the stob */
  Area* area = s48_memory_map_ref(address);
#if (MEASURE_GC)
   /* The area of the value pointed to. same result as with:
	 s48_memory_map_ref(S48_ADDRESS_AT_HEADER(value)); */
  if (S48_STOB_P(value)) {
    Area* meas_to_area  = s48_memory_map_ref((s48_address)value); 
    if (area->generation_index > meas_to_area->generation_index) {
      measure_write_barrier(1);
    } else {
      measure_write_barrier(0);
    }
  }
#endif
  /* somehow the write_barrier gets called from some strange places --
     e.g. in read_image, that's why area can get NULL. */
  if ((area == NULL) || (area->generation_index == 0)) return;
  
#if S48_WRITE_BARRIER_COMPLEXITY == S48_MUTATED_LOCATION
  s48_set_dirty_vector(area, address, value, NULL);
#else
    if (S48_STOB_P(value)) {
#if S48_WRITE_BARRIER_COMPLEXITY == S48_STOB_LOCATION
      s48_set_dirty_vector(area, address, value, NULL);
#elif S48_WRITE_BARRIER_COMPLEXITY == S48_INTERGEN_STOB_LOCATION
      /* The area of the value pointed to. same result as with:
	 s48_memory_map_ref(S48_ADDRESS_AT_HEADER(value)); */
      Area* to_area  = s48_memory_map_ref((s48_address)value); 
      
      if (area->generation_index > to_area->generation_index)
	s48_set_dirty_vector(area, address, value, to_area); 
#endif
    }
#endif
}

/* called after a stob in AREA, stored at ADDR pointing into TO_AREA, has
   been updated, either because the object has been copied to a new
   area, a large area has been linked into a new list, or the object
   has been traced but ignored. */

inline static void s48_internal_write_barrier(Area* area, s48_address addr,
					      long value, Area* to_area) {
  /* we're not interested in locations in an other-space area. This
     test should be sufficient. This happens because of root-set
     functions, that trace a stob inside an area that is beeing
     collected (other-space). */
  if (area->action != GC_ACTION_IGNORE)
    return;
#if (S48_HAVE_TRANSPORT_LINK_CELLS)
  /* We could ignore the key field here for optimization, but maybe
     it's not worth the test. */
#endif
  /* We're only interested in pointers from old to young */
  if (area->generation_index > to_area->generation_index) {
#if (MEASURE_GC)
    measure_gc_write_barrier();
#endif
#if (S48_USE_REMEMBERED_SETS)
    /* if the rememberd-set can store this pointer, then we're done,
       otherwise mark the card anyway */
    if (s48_remset_add(addr, to_area->remset))
      return;
#endif
    s48_set_dirty_vector_inline(area, addr, value, to_area);
  }
}

#endif
