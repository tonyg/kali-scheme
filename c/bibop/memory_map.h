/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_MEMORY_MAP_H
#define __S48_MEMORY_MAP_H

#include "memory.h"
#include "areas.h"

/* When a new page is allocated: */
extern void s48_memory_map_setB(s48_address address, Area* value);

/* When the Area structure of a page is needed: */
/* extern Area* s48_memory_map_ref(s48_address address);
   - defined inline below */

/* The following is only defined here to allow the inlining of
   s48_memory_map_ref, and not used elsewhere: */

#include <assert.h>

/* We need an integer type of the same size as pointers, and the bit
   length of addresses: */
#ifdef WIN32

#define uintptr_t (unsigned long int)
#define ADDRESS_LENGTH 32

#elif WIN64

#define uintptr_t (unsigned long long int)
#define ADDRESS_LENGTH 64

#else

#include <stdint.h>
#include <limits.h>
#define ADDRESS_LENGTH __WORDSIZE

#endif

/*** CONFIGURATION ***/

/* The memory_map provides a way to store and find an Area structure
   for every memory page we have allocated for the heap. It must be
   very fast, without consuming too much memory at the same time.

   For 32bit systems, this is quite easy, but to achieve this for
   64bit systems, a sort of hashing is implemented. It is
   automatically activated if the defined page size and a given size
   for a statically allocated array are too small to cover all
   potential addresses.

   The first relevant size is defined in page_constants.h:

   LOG_BYTES_PER_PAGE

   The next one defines the size of a statically allocated global
   array, which stores pointers to Metapage structures; it's
   logarithmic size is:
*/

#define LOG_TABLE_SIZE 10

/* Metapage structures are allocated by need, and each consists of
   another array of pointers to Area structures. It's logarithmic size
   is:
*/

#define LOG_PAGES_PER_METAPAGE 10

/*** END OF CONFIGURATION ***/

/* Now with the usual sizes on a 32bit system these sizes sum up to 32
   bits and we don't need the hashing algorithm. Let's see if we do
   need it:
 */

#define USED_ADDRESS_BITS \
  (LOG_BYTES_PER_PAGE + LOG_TABLE_SIZE + LOG_PAGES_PER_METAPAGE)

#define REMAINING_ADDRESS_BITS \
  (ADDRESS_LENGTH - USED_ADDRESS_BITS)

#if REMAINING_ADDRESS_BITS > 0
#define NEED_METAPAGE_HASHING
#elif REMAINING_ADDRESS_BITS == 0
#undef NEED_METAPAGE_HASHING
#else
#error "Misconfigured memory map."
#endif

/* For both direct access and hashed access, we split an address into
   the following fields:

   high                                                             low
   |   Rest   |  Metapage in Table  | Page in Metapage | Byte in Page |

   If the Rest has 0-length we don't need hashing.

 */

/* Some sizes: */
#define METAPAGES (((uintptr_t)1) << LOG_TABLE_SIZE)
#define PAGES_PER_METAPAGE (((uintptr_t)1) << LOG_PAGES_PER_METAPAGE)

/* Some accessors for the fields : */

#define ADDR_METAPAGE_INDEX(address) \
  ( ( ((uintptr_t)address) >> (LOG_BYTES_PER_PAGE + LOG_PAGES_PER_METAPAGE) ) \
    & (METAPAGES - 1) )

#define ADDR_PAGE_INDEX(address) \
  ( ( ((uintptr_t)address) >> LOG_BYTES_PER_PAGE ) \
    & (PAGES_PER_METAPAGE - 1) )

#ifdef NEED_METAPAGE_HASHING

#define ADDR_REST(address) \
  ( ((uintptr_t)address) >> USED_ADDRESS_BITS )

/* To identify the correct hash bucket, we need store the start
   address of all pages of a metapage. We use this macro to compare
   them: */
#define ADDR_REST_MASK \
  ( ((uintptr_t)-1) << USED_ADDRESS_BITS )
#define ADDR_REST_MASKED(address) ((void*)(ADDR_REST_MASK & ((uintptr_t)address)))
#define IS_CORRECT_METAPAGE(metapage, address) \
  ( ADDR_REST_MASKED(metapage->start_address) == ADDR_REST_MASKED(address) )

#endif

/* And now the structure we use; for hashing, we use a linked list of
   Metapages for pages which have the same ADDR_METAPAGE_INDEX, but
   different ADDR_REST parts:
*/

typedef struct _Metapage {
#ifdef NEED_METAPAGE_HASHING
  s48_address start_address;
  struct _Metapage* next;
#endif
  Area* contents[PAGES_PER_METAPAGE];
} Metapage;

#define TABLE_SIZE (1L << LOG_TABLE_SIZE)

/* static Metapage* s48_memory_table[TABLE_SIZE]; */
extern Metapage* s48_memory_table[TABLE_SIZE];

#ifdef NEED_METAPAGE_HASHING

/* returns the place of the found pointer to the metapage, resp. the
   place where a newly allocated metepage should be stored: */
inline static Metapage** find_metapagep(s48_address address) {
  Metapage** bucketp = &s48_memory_table[ADDR_METAPAGE_INDEX(address)];
  while ((*bucketp != NULL) && (!IS_CORRECT_METAPAGE((*bucketp), address))) {
    bucketp = &(*bucketp)->next;
  }
  assert(bucketp != NULL);
  return bucketp;
}

#else

#define find_metapagep(address) (&s48_memory_table[ADDR_METAPAGE_INDEX(address)])

#endif

inline static Area* s48_memory_map_ref(s48_address address) {
  Metapage* metapage = *find_metapagep(address);
  if (metapage == NULL)
    return NULL;
  else
    return metapage->contents[ADDR_PAGE_INDEX(address)];
};

#endif
