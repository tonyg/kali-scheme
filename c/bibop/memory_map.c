/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/* The Boehm collector also has a link to the next entry in ascending
   order. This is used for sweeping etc.. I don't think we need
   such. It would be hard to reconstruct dynamically.  It is easy
   enough to find adjacent blocks, which is all I would think matters
   to us */

#include <stdlib.h>
#include "utils.h"
#include "memory_map.h"
#include "page_constants.h"

Metapage* s48_memory_table[TABLE_SIZE];

void s48_memory_map_setB(s48_address address, Area* value) {
  Metapage** metapagep = find_metapagep(address);
  Metapage* metapage = *metapagep;
  if (metapage == NULL) {

    metapage = (Metapage*)calloc(1, sizeof(Metapage));
    if (metapage == NULL) s48_gc_error("add_metapage: out of memory");

#ifdef NEED_METAPAGE_HASHING
    metapage->start_address = ADDR_REST_MASKED(address);
    assert(IS_CORRECT_METAPAGE(metapage, address));
    metapage->next = NULL;
#endif

    *metapagep = metapage;
  }
  metapage->contents[ADDR_PAGE_INDEX(address)] = value;
  assert(metapage == *(find_metapagep(address)));
}
