/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_REMEMBERED_SET
#define __S48_REMEMBERED_SET

#include "utils.h"
#include "memory.h"
#include "gc_config.h"

#if (S48_USE_REMEMBERED_SETS)

#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
/* Remembered Intergenerational Pointer Structure */
typedef struct Rem_ip{
  s48_address ip;
  struct Rem_ip* next_ip;
} Rem_ip;
#endif

typedef struct RemSet {
#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
  Rem_ip* first_el;
  Rem_ip* last_el;
#elif S48_REMEMBERED_SET_TYPE==S48_STATIC_REMEMBERED_SETS
  s48_address elements[S48_REMEMBERED_SET_SIZE];
  int free_index;
#elif S48_REMEMBERED_SET_TYPE==S48_EXTENSIBLE_REMEMBERED_SETS
  s48_address elements[S48_REMEMBERED_SET_SIZE];
  int free_index;
  struct RemSet* next_remset;
#endif
}RemSet;

/* RemSet Structure Construction */
RemSet* s48_make_remset();
void s48_free_remset(RemSet* remset);

/* Operations on RemSet */
int s48_remset_add(s48_address ip, RemSet* remset);
int s48_remset_size(RemSet* remset);
void s48_trace_remset(RemSet* remset);

/* checks if all locations are inside the heap */
void s48_check_remset(RemSet* remset);
#endif /* #if (S48_USE_REMEMBERED_SETS) */

#endif
