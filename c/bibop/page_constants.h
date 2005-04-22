/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_PAGE_CONSTANTS_H
#define __S48_PAGE_CONSTANTS_H

#include "memory.h"

#define LOG_BYTES_PER_PAGE 12
#define BYTES_PER_PAGE (1L << LOG_BYTES_PER_PAGE)
#define PAGE_INDEX_MASK (BYTES_PER_PAGE - 1)
#define PAGE_START_ADDRESS(address) ((s48_address)(((long)address) & \
  (~ PAGE_INDEX_MASK)))
#define INDEX_IN_PAGE(address) ((long)((address) & PAGE_INDEX_MASK))
#define BYTES_TO_PAGES(n) (((n) + (BYTES_PER_PAGE - 1)) >> LOG_BYTES_PER_PAGE)
#define PAGES_TO_BYTES(n) ((n) << LOG_BYTES_PER_PAGE)
#define PAGES_TO_CELLS(n) (S48_BYTES_TO_CELLS((n) << LOG_BYTES_PER_PAGE))
#define ADD_PAGES(address, pages) ((s48_address)((address) + \
					       PAGES_TO_BYTES(pages)))

#endif
