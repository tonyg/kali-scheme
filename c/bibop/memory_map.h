/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_MEMORY_MAP_H
#define __S48_MEMORY_MAP_H

#include "memory.h"
#include "areas.h"

/* The middle 10 bits of an address represent the page-index in a
   metapage */
#define LOG_PAGES_PER_METAPAGE 10

/* The upper 10 bits of an address represent the metapage-index in the
   table */
#define LOG_TABLE_SIZE 10

/* With a word-length of 32 bits, and LOG_TABLE_SIZE == 10, there is
   no need to use hashing */
#undef USE_METAPAGE_HASHING


#define PAGES_PER_METAPAGE (1L << LOG_PAGES_PER_METAPAGE)

#define METAPAGE_INDEX_MASK ((1L << LOG_PAGES_PER_METAPAGE) - 1)

#define PAGE_INDEX_IN_METAPAGE(address) ((((unsigned long)address) >> \
					  LOG_BYTES_PER_PAGE) & \
					 METAPAGE_INDEX_MASK)

typedef struct _Metapage {
  s48_address start_address;
  Area* contents[PAGES_PER_METAPAGE];
#ifdef USE_METAPAGE_HASHING
  struct _Metapage* next;
#endif
} Metapage;

/* only extern for use in the inlined s48_memory_map_ref - not
   modified or directly accessed elsewhere */
extern Metapage* s48_memory_table[];

extern void s48_memory_map_setB(s48_address address, Area* value);

/* s48_memory_map_ref */

#define DIRECT_TABLE_INDEX(address) (((unsigned long)address) >> \
				     (LOG_BYTES_PER_PAGE + \
				      LOG_PAGES_PER_METAPAGE))
#ifdef USE_METAPAGE_HASHING

#define HASH_TABLE_INDEX(address) \
  (DIRECT_TABLE_INDEX(address) ^ \
   (DIRECT_TABLE_INDEX(address) >> LOG_TABLE_SIZE))

inline static Metapage* hashed_find_metapage(s48_address start_address) {
  Metapage* bucket = s48_memory_table[HASHED_TABLE_INDEX(start_address)];
  while ((bucket != NULL) && (start_address != bucket->start_address)) {
    bucket = bucket->next;
  }
  return bucket;
}

#define TABLE_INDEX HASH_TABLE_INDEX
#define find_metapage hashed_find_metapage

#endif
#ifndef USE_METAPAGE_HASHING

#define TABLE_INDEX DIRECT_TABLE_INDEX
#define direct_find_metapage(start_address) \
  s48_memory_table[TABLE_INDEX(start_address)]
#define find_metapage direct_find_metapage

#endif

inline static Area* s48_memory_map_ref(s48_address address) {
  /*s48_address start_address = PAGE_START_ADDRESS(address);*/
  Metapage* metapage = find_metapage(address);
  if (metapage == NULL)
    return NULL;
  else
    return metapage->contents[PAGE_INDEX_IN_METAPAGE(address)];
};


#endif
