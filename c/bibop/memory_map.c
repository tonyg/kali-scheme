/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include "utils.h"
#include "memory_map.h"
#include "page_constants.h"

/* The Boehm collector also has a link to the next entry in ascending
   order. This is used for sweeping etc.. I don't think we need
   such. It would be hard to reconstruct dynamically.  It is easy
   enough to find adjacent blocks, which is all I would think matters
   to us */

#define TABLE_SIZE (1L << LOG_TABLE_SIZE)
#define TABLE_MASK (TABLE_SIZE - 1)

Metapage* s48_memory_table[TABLE_SIZE];

inline static Metapage* add_metapage(unsigned int index,
				     s48_address start_address) {
  Metapage* metapage = (Metapage*)calloc(1, sizeof(Metapage));
  if (metapage == NULL) s48_gc_error("add_metapage: out of memory");

  metapage->start_address = start_address;

  /* Check out the index in the table -  Just to be sure */
  if (s48_memory_table[index] == NULL) {
    s48_memory_table[index] = metapage;
  }
  else {
    s48_gc_error("add_metapage: table[ %i ] is allready ocuppied", index);
  }

  return metapage;
}

void s48_memory_map_setB(s48_address address, Area* value) {
  s48_address start_address = PAGE_START_ADDRESS(address);
  unsigned int index = TABLE_INDEX(start_address);
  Metapage* metapage = find_metapage(start_address);
  if (metapage == NULL)
    metapage = add_metapage(index, start_address);
  metapage->contents[PAGE_INDEX_IN_METAPAGE(address)] = value;
}
