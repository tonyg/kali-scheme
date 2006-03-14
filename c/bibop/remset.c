/* Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/* Remembered Sets */

#include <stdlib.h>

#include "scheme48.h"

#include "remset.h"
#include "memory.h"
#include "utils.h"
#include "data.h"
#include "generation_gc.h"
#include "memory_map.h"
#include "gc_config.h"

#if (S48_USE_REMEMBERED_SETS)
 
static int s48_remset_member(s48_address ip, RemSet* remset);

#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
/* Construction of a Rem_ip */
static Rem_ip* allocate_Rem_ip(){
  Rem_ip* rem_ip =  (Rem_ip*)calloc(1,sizeof(Rem_ip));
  if (!rem_ip){
    s48_gc_error("allocate_Rem_ip: out of memory");
  }
  return rem_ip;
}

static void initialize_Rem_ip(Rem_ip* rem_ip){
  rem_ip->ip = 0;
  rem_ip->next_ip = 0;
}

static Rem_ip* make_Rem_ip(){
  Rem_ip* rem_ip = (Rem_ip*)allocate_Rem_ip();
  initialize_Rem_ip(rem_ip);
  return rem_ip;
}
#endif


/* Construction of a Rem_set */
static RemSet* allocate_RemSet(){
  RemSet* remset = (RemSet*)calloc(1, (sizeof(RemSet)));
  if (!remset){
    s48_gc_error("allocate_RemSet: out of memory");
  }
  return remset;
}

static void initialize_RemSet(RemSet* remset){
#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
  Rem_ip* el = (Rem_ip*)make_Rem_ip();
  remset->first_el = el;
  remset->last_el = el;
#elif S48_REMEMBERED_SET_TYPE==S48_STATIC_REMEMBERED_SETS
  remset->free_index = 0;
#elif S48_REMEMBERED_SET_TYPE==S48_EXTENSIBLE_REMEMBERED_SETS
  remset->free_index = 0;
  remset->next_remset = NULL;
#endif
}

RemSet* s48_make_remset(){
  RemSet* remset = allocate_RemSet();
  initialize_RemSet(remset);
  return remset;
}

int s48_remset_size(RemSet* remset){
#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
  Rem_ip* next = remset->first_el;
  Rem_ip* empty = remset->last_el;
  int size = 0;
  
  while(next != empty){
    size++;
    next = next->next_ip;
  }
  return size;
#elif S48_REMEMBERED_SET_TYPE==S48_STATIC_REMEMBERED_SETS
  return remset->free_index;
#elif S48_REMEMBERED_SET_TYPE==S48_EXTENSIBLE_REMEMBERED_SETS
  int size=0;
  while(remset != NULL){
    size += remset->free_index;
    remset = remset->next_remset;
  }
  return size;
#endif
}


/* Adds an IP to a RS and returns a boolean */
int s48_remset_add(s48_address ip, RemSet* remset){
#if S48_UNIQUE_REMEMBERED_SET==TRUE  /* no duplicates */
  if (s48_remset_member(ip, remset))
    return TRUE;
#endif
#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
  {
    Rem_ip* last = remset->last_el;
    Rem_ip* new_el = make_Rem_ip();
    
    last->ip = ip;
    last->next_ip = new_el;
    remset->last_el = new_el;
    return TRUE;
  }
#elif S48_REMEMBERED_SET_TYPE==S48_STATIC_REMEMBERED_SETS
  if (remset->free_index == S48_REMEMBERED_SET_SIZE){
    return FALSE;
  }
  else{
    remset->elements[remset->free_index] = ip;
    remset->free_index++;
    return TRUE;
  }
#elif S48_REMEMBERED_SET_TYPE==S48_EXTENSIBLE_REMEMBERED_SETS
  /* search or create a free remset */
  while (remset->free_index == S48_REMEMBERED_SET_SIZE){
    if (remset->next_remset == NULL)
      remset->next_remset= s48_make_remset();
    remset = remset->next_remset;
  }
    
  remset->elements[remset->free_index] = ip;
  remset->free_index++;
  return TRUE;
#endif /* S48_REMEMBERED_SET_TYPE */
}


static int s48_remset_member(s48_address ip, RemSet* remset){
#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
  Rem_ip* next = remset->first_el;
  Rem_ip* empty = remset->last_el;
  
  /* if both pointers point to the same rem_ip -> not a member */
  while(next != empty){ 
    if ((s48_address)next->ip == ip){
      return TRUE;
    }
    next = next->next_ip;
  }
  return FALSE;
#elif S48_REMEMBERED_SET_TYPE==S48_STATIC_REMEMBERED_SETS
  int i;
  for (i = 0; i < remset->free_index; i++){
    if (remset->elements[i] == ip)
      return TRUE;
  }
  
  return FALSE;
#elif S48_REMEMBERED_SET_TYPE==S48_EXTENSIBLE_REMEMBERED_SETS
  int i;
  while (remset != NULL){
    for (i = 0; i < remset->free_index; i++){
      if (remset->elements[i] == ip)
	return TRUE;
    }
    remset = remset->next_remset;
  } 
  return FALSE;
#endif /* S48_REMEMBERED_SET_TYPE */
}

static inline void call_trace_locationsB(s48_address addr) {
  Area* area = (Area*)s48_memory_map_ref(addr);
  /* we only want to trace locations, that are not currently
     collected. This test should be sufficient */
  if (area->action == GC_ACTION_IGNORE)
    s48_internal_trace_locationsB(area, addr, S48_ADDRESS_INC(addr), "call_trace_locationsB");
}

void s48_trace_remset(RemSet* remset){
#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
  Rem_ip* next = remset->first_el;
  Rem_ip* empty = remset->last_el;

  /* if both pointers point to the same rem_ip -> done*/
  while(next != empty){
    call_trace_locationsB(next->ip);
    next = next->next_ip;
  }
#elif S48_REMEMBERED_SET_TYPE==S48_STATIC_REMEMBERED_SETS
  int i;
  for (i = 0; i < remset->free_index; i++)
    call_trace_locationsB(remset->elements[i]);

#elif S48_REMEMBERED_SET_TYPE==S48_EXTENSIBLE_REMEMBERED_SETS
  int i;
  while (remset != NULL){
    for (i = 0; i < remset->free_index; i++)
      call_trace_locationsB(remset->elements[i]);

    remset = remset->next_remset;
  }
#endif
}

void s48_free_remset(RemSet* remset){
#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
  Rem_ip* next_ip;
  Rem_ip* rem_ip = remset->first_el;
  while(rem_ip != NULL){
    next_ip = rem_ip->next_ip;
    free(rem_ip);
    rem_ip = next_ip;
  }
  free(remset);
#elif S48_REMEMBERED_SET_TYPE==S48_STATIC_REMEMBERED_SETS
  free(remset);
#elif S48_REMEMBERED_SET_TYPE==S48_EXTENSIBLE_REMEMBERED_SETS
  RemSet* next_remset;
  while(remset != NULL){
    next_remset = remset->next_remset;
    free(remset);
    remset = next_remset;
  }
#endif
}

void s48_check_remset(RemSet* remset) {
  /* only implemented for dynamic remsets, for now */
#if S48_REMEMBERED_SET_TYPE==S48_DYNAMIC_REMEMBERED_SETS
  Rem_ip* next = remset->first_el;
  Rem_ip* empty = remset->last_el;
  
  while(next != empty) {
    Area* a = (Area*)s48_memory_map_ref(next->ip);
    if ((a == NULL) || (next->ip >= a->frontier)) {
      s48_gc_error("remembered-set currupted! Location 0x%X outside the heap.",
		   next->ip);
    }
    next = next->next_ip;
  }
#endif
}

#endif
