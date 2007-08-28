/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include "generation_gc.h"

#include <stdlib.h>
#include <string.h> /* memcpy */

#include "scheme48heap.h"

#include "areas.h"
#include "data.h"
#include "memory.h"
#include "page_alloc.h"
#include "utils.h"
#include "memory_map.h"
#include "page_constants.h"
#include "area_roots.h"
#include <assert.h>
#include "gc_config.h"
#include "measure.h"
#include "remset.h"

#define FOR_ALL_AREAS(areas, command) \
do { \
  Area* area = areas; \
  while (area != NULL) { \
    Area* next = area->next; \
    command; \
    area = next; \
  } \
} while(0)


typedef struct Generation {
  int index;
  Space* current_space;
  Space* other_space;

  /* size of the generation after its last collection */
  unsigned long last_size; 
  
  /* Number of collections from this generation*/
  unsigned long self_count;

  /* Number of all collections (age_count) */
  unsigned long age_count; 

} Generation;


static Space* make_space(int generation_index) {
  Space* space = (Space*)calloc(1, sizeof(Space));
  if (space == NULL) {
    s48_gc_error("make_space: out of memory");
  }
  
  space->generation_index = generation_index;
  return space;
}

typedef struct {
  /* a list of areas */
  Area* large;
  
  /* always one of the ones below or one of the small
  areas in the first generation in some cases */
  Area* small;
  
  Area* small_below;
  Area* small_above;

  /* only one area */
  Area* weaks;
} CreationSpace;

static CreationSpace creation_space;

static unsigned int current_water_mark; /* pages */

/* from young to old */
static Generation generations[S48_GENERATIONS_COUNT];

#if (S48_USE_RDM)
#define RDM_BEFORE_SWAP 0
#define RDM_AFTER_SWAP 1
#define RDM_AFTER_COLLECTION 2
static int rdm_level = RDM_BEFORE_SWAP;
static unsigned long rdm_threshold = S48_RDM_INITIAL_THRESHOLD;
#endif

static char heap_is_initialized = 0;
static char gc_forbid_count = 0;
static unsigned long gc_count = 0;
static long gc_seconds = 0;
static long gc_mseconds = 0;

static void recreate_creation_space() {
  unsigned int s_below;
  unsigned int s_above;

  /* free current areas */
  if (creation_space.small_below != NULL)
    s48_free_area(creation_space.small_below);
  if (creation_space.small_above != NULL)
    s48_free_area(creation_space.small_above);
  if (creation_space.large != NULL)
    s48_free_areas(creation_space.large);
  creation_space.large = NULL;
  if (creation_space.weaks != NULL)
    s48_free_area(creation_space.weaks);

  /* create some new ones */
  s_below = current_water_mark;
  if (s_below != 0)
    creation_space.small_below = s48_allocate_area(s_below, s_below, 0, AREA_TYPE_SIZE_SMALL);
  else
    creation_space.small_below = NULL;

  s_above = S48_CREATION_SPACE_SIZE - current_water_mark;
  if (s_above != 0)
    creation_space.small_above = s48_allocate_area(s_above, s_above, 0, AREA_TYPE_SIZE_SMALL);
  else
    creation_space.small_above = NULL;

  if (creation_space.small_below == NULL) {
    creation_space.small = creation_space.small_above;
  }
  else {
    creation_space.small = creation_space.small_below;
  }

  creation_space.weaks = s48_allocate_area(S48_MINIMUM_WEAK_AREA_SIZE,
					   S48_MAXIMUM_WEAK_AREA_SIZE, 
					   0,  
					   AREA_TYPE_SIZE_WEAKS);
}

/* FPage 1 */
void s48_initialize_bibop_heap() {

  int i;

  if (heap_is_initialized == 1) return;

  s48_initialize_page_allocation();

  current_water_mark = S48_DEFAULT_WATER_MARK;
  
  creation_space.small_below = NULL;
  creation_space.small_above = NULL;
  creation_space.large = NULL;
  creation_space.weaks = NULL;

  recreate_creation_space();

  for (i = 0; i < S48_GENERATIONS_COUNT; i++) {
    generations[i].index = i;
    generations[i].current_space = make_space(i);
    generations[i].other_space = make_space(i);
    generations[i].last_size = 0;
    generations[i].self_count = 0;
    generations[i].age_count = 0;
  }

  heap_is_initialized++;
}

/* this adds AREA to the "known" heap. Used by find_all for example. */
void s48_integrate_area(Area* area) {
  Space* space = generations[0].current_space;
  Area** a;
  /* put it behind the first area of the large or small list of the
     first generation */
  if ((area->frontier - area->start) < S48_SMALL_OBJECT_LIMIT) {
    if (space->small_area == NULL)
      a = &space->small_area;
    else
      a = &space->small_area->next;
  } else {
    if (space->large_area == NULL)
      a = &space->large_area;
    else
      a = &space->large_area->next;
  }
  area->generation_index = 0;
  area->next = *a;
  *a = area;
}

static float last_aging_space_survival = 0; /* initial value does not
					       matter */

static void adjust_water_mark(float aging_space_survival) {
  /* the easies solution would be to set the water-mark according to
     this survival-percent value, but let's try to get some
     convergence. And we tried to weaken extreme values */
  last_aging_space_survival = ((aging_space_survival 
			       + (gc_count * last_aging_space_survival)) /
			       (gc_count + 1));
  /* maybe take int_max(gc_count, 1000) or something... */

  current_water_mark =
    BYTES_TO_PAGES((unsigned long)(PAGES_TO_BYTES(S48_CREATION_SPACE_SIZE)
				   * last_aging_space_survival));
  /* if the water-mark would be at the top, then nothing would be
     copied into the aging_space, and we could not adjust the
     water-mark in the future. */
  if (current_water_mark == S48_CREATION_SPACE_SIZE)
    current_water_mark--;
}

/********************************************************************
 Starting a Collection
 ********************************************************************/

#define BROKEN_HEART_P S48_STOB_P

static unsigned long calc_generation_area_size(Generation* g);
static unsigned long calc_creation_space_size(CreationSpace* c);
static long heap_size_count;

static void heap_size_step(s48_address start, s48_address end) {
  heap_size_count += end - start;
}

long s48_heap_size() {
  int i;
  unsigned long size = 0;
  for (i=0; i < S48_GENERATIONS_COUNT; i++){
    size += calc_generation_area_size(&generations[i]);
  }
  size += calc_creation_space_size(&creation_space);
  return size;
}

static unsigned long calc_creation_space_size(CreationSpace* c) {
  unsigned long size = 0;
  FOR_ALL_AREAS(c->large, 
		size += (area->end - area->start));
  FOR_ALL_AREAS(c->small_below,
		size += (area->end - area->start));
  FOR_ALL_AREAS(c->small_above,
		size += (area->end - area->start));
  FOR_ALL_AREAS(c->weaks,
		size += (area->end - area->start));
  return size;
}

/* calc the WHOLE allocated heap (Semispace) */
static unsigned long calc_generation_area_size(Generation* g) {
  unsigned long size = 0;
  FOR_ALL_AREAS(g->current_space->small_area,
		size += (area->end - area->start));
  FOR_ALL_AREAS(g->current_space->large_area,
		size += (area->end - area->start));
  FOR_ALL_AREAS(g->current_space->weaks_area,
		size += (area->end - area->start));
  return size;
}

long s48_heap_live_size() {
  heap_size_count = 0;
  s48_walk_heap(&heap_size_step);
  return heap_size_count;
}


/* An extant is either not a stored object, or it has a forwarding
   pointer, or it is in an area that is not currently being collected.
*/

char s48_extantP(s48_value thing) {
  if ((!S48_STOB_P(thing)) || BROKEN_HEART_P(S48_STOB_HEADER(thing))) {
    return (0 == 0);
  } else {
    Area* area = s48_memory_map_ref(S48_ADDRESS_AT_HEADER(thing));
    return ( (area == NULL) || (GC_ACTION_IGNORE == area->action) );
  }
}

static void clean_weak_pointers(Area* areas) {
  while (areas != NULL) {
    s48_address end = areas->frontier;
    s48_address addr = S48_ADDRESS_INC(areas->start);
    for (; addr < end; addr += S48_CELLS_TO_A_UNITS(2)) {
      s48_value stob = *((s48_value*)addr);
      if (!s48_extantP(stob))
	*((s48_value*)addr) = S48_FALSE;
      else {
	/* maybe the object location has changed */
	if (S48_STOB_P(stob) && BROKEN_HEART_P(S48_STOB_HEADER(stob)))
	  *((s48_value*)addr) = S48_STOB_HEADER(stob);
      }
    }
    areas = areas->next;
  }
}

static void set_targets(Space* space, Space* target) {
  FOR_ALL_AREAS(space->small_area, area->target_space = target);
  FOR_ALL_AREAS(space->large_area, area->target_space = target);
  FOR_ALL_AREAS(space->weaks_area, area->target_space = target);
}

/* forward declaration */
static unsigned long calc_areas_number(Area* next_area);


#define FOR_YOUNG_AREAS(areas, command) \
do { \
  Area* area = areas; \
  unsigned long count; \
  unsigned long areas_number; \
  unsigned long old_areas; \
  count = 0; \
  areas_number = calc_areas_number(areas); \
  old_areas = areas_number / S48_PART_OF_OLD_AREAS ; \
  while (area != NULL) { \
    Area* next = area->next; \
    if (count > old_areas) command; \
    count++; \
    area = next; \
  } \
} while(0)

/* The youngest areas will be recycled in the same generation*/ 
static void reset_young_targets(Space* space, Space* target) {
  FOR_YOUNG_AREAS(space->small_area, area->target_space = target);
  /* Large Objects should be allways promoted as they are */
  /* FOR_YOUNG_AREAS(space->large_area, area->target_space = target); */
  FOR_YOUNG_AREAS(space->weaks_area, area->target_space = target);
}

static void set_gc_actions(Space* space, unsigned int small,
			   unsigned int large, unsigned int weaks) {
  FOR_ALL_AREAS(space->small_area, area->action = small);
  FOR_ALL_AREAS(space->large_area, area->action = large);
  FOR_ALL_AREAS(space->weaks_area, area->action = weaks);
}

/* How many Area(s) are in the linked listz of areas */ 
static unsigned long calc_areas_number(Area* next_area){
  unsigned long the_areas_number=0;
  
  while (next_area){
    the_areas_number++;
    next_area = next_area->next;
  }

#if (BIBOP_LOG)
  s48_bibop_log("Areas Number: %i", the_areas_number);
#endif

  return the_areas_number;
}

/* calc only the used heap (Semispace) */
static unsigned long calc_generation_size(Generation* g) {
  unsigned long size = 0;
  FOR_ALL_AREAS(g->current_space->small_area,
		size += (area->frontier - area->start));
  FOR_ALL_AREAS(g->current_space->large_area,
		size += (area->frontier - area->start));
  FOR_ALL_AREAS(g->current_space->weaks_area,
		size += (area->frontier - area->start));
  return size;
}

static unsigned long calc_generation_other_space_size(Generation* g) {
  unsigned long size = 0;
  FOR_ALL_AREAS(g->other_space->small_area,
		size += (area->frontier - area->start));
  FOR_ALL_AREAS(g->other_space->large_area,
		size += (area->frontier - area->start));
  FOR_ALL_AREAS(g->other_space->weaks_area,
		size += (area->frontier - area->start));
  return size;
}



/* FPage 6 - 7 - 8 */
inline static void init_areas(int count) {
  int i, j, x, y;
  unsigned long current_size;

  /* Generation indices for the creation_space */
#if (S48_GENERATIONS_COUNT > 1)
  x = 1;
#else
  x = 0;
#endif

  y = 0;

  /* REMARK: At the very first collection, the image is loaded, which
     has source compiled code that rarely changes. At this point there
     was no execution of the main program yet. We can hold the
     surviving objects of the first collection (of the creation_space
     or of the first generation - or both) in a protected generation
     (preferrably the oldest one) which is collected never again.
     (alternatively collecting it after a large number of collections)
     For this purpose we need at least 3 generations!  The option to
     activate this is S48_USE_STATIC_SPACE */

#if (S48_USE_STATIC_SPACE)
  if (s48_gc_count() == 0) {
    x = S48_GENERATIONS_COUNT - 1;
    y = x;
  }
#endif

  /* 
  /* FPage 6 */
  /* initialize the creation_space */
  /* the objects of the small_below area that will survive the
     collection will be moved into an older generation */
  if (creation_space.small_below != NULL) {     
    assert(creation_space.small_below->next == NULL);
    creation_space.small_below->target_space = generations[x].current_space;
    creation_space.small_below->action = GC_ACTION_COPY_SMALL;
  }
  /* the objects of the small_above area, large area and weaks area,
     that will survive the collection will be moved (or marked) into
     the youngest (first) generation, to be soon recollected */
  if (creation_space.small_above != NULL) {
    assert(creation_space.small_above->next == NULL);
    creation_space.small_above->target_space = generations[y].current_space;
    creation_space.small_above->action = GC_ACTION_COPY_SMALL;
  }
  
  assert(creation_space.weaks->next == NULL);
  creation_space.weaks->target_space = generations[y].current_space;
  creation_space.weaks->action = GC_ACTION_COPY_WEAK;
  
  FOR_ALL_AREAS(creation_space.large,
		area->target_space = generations[y].current_space;
		area->action = GC_ACTION_MARK_LARGE );
  
  /* FPage 7 */
  /* Promotion policy - Which generation should the live objects be
     copied to? */
#if (BIBOP_LOG)
  
  /* all current sizes */
#if (S48_PROMOTION_THRESHOLD)
  s48_bibop_log("S48_PROMOTION_THRESHOLD: %i", S48_PROMOTION_THRESHOLD);
#endif
  
#if (S48_PROMOTION_HEAP_LIMIT)
  s48_bibop_log("S48_PROMOTION_HEAP_LIMIT: %i", S48_PROMOTION_HEAP_LIMIT);
#endif
  
#if (S48_PROMOTION_AGE_LIMIT)
  s48_bibop_log("S48_PROMOTION_AGE_LIMIT: %i", S48_PROMOTION_AGE_LIMIT);
#endif
  
  for (i = S48_GENERATIONS_COUNT - 1; i > -1; i--) {
    /* Look  out! Spaces are allready swapped !!! */
    current_size = calc_generation_other_space_size(&generations[i]);
    
#if (S48_PROMOTION_THRESHOLD)
    s48_bibop_log("Generation %i : %i ", i, (current_size - generations[i].last_size));
#endif
    
#if (S48_PROMOTION_HEAP_LIMIT)
    s48_bibop_log("Generation %i : %i ", i, current_size);
    
#endif
    
#if (S48_PROMOTION_AGE_LIMIT)
    s48_bibop_log("Generation %i : Age Count  %i ", i, generations[i].age_count);
    s48_bibop_log("Generation %i : Self Count %i ", i, generations[i].self_count);
#endif
  }
#endif /* #if (BIBOP_LOG) */
  
  
  /* initialize the areas that will be collected. */
  for (i = 0; i < count; i++) {

    /* trace everything */    
    FOR_ALL_AREAS(generations[i].current_space->small_area,
		  area->trace = area->start);
    FOR_ALL_AREAS(generations[i].current_space->large_area,
		  area->trace = area->start);
    
    /* targets of the other_spaces are the current_space of the choosen
       generation according to the promotion option */
    
    /* Look  out! Spaces are allready swapped !!! */
    current_size = calc_generation_other_space_size(&generations[i]);
    
#if (S48_PROMOTION_THRESHOLD)
    
    j = ( (current_size - generations[i].last_size)
	  > S48_PROMOTION_THRESHOLD) 
      ? i + 1  
      : i;
    
#elif (S48_PROMOTION_HEAP_LIMIT)
    /* Look  out! Spaces are allready swapped !!! */
    j = (current_size > S48_PROMOTION_HEAP_LIMIT)
      ? i + 1
      : i;
    
#elif (S48_PROMOTION_AGE_LIMIT)
    j = (generations[i].self_count > 0 &&
	 generations[i].self_count % S48_PROMOTION_AGE_LIMIT == 0)
      ? i + 1
      : i;
    
#else
#error "BIBOP GC configuration error: no promotion policy defined"
#endif

#if (S48_USE_STATIC_SPACE)
    j = (s48_gc_count() == 0) ? x : j ;
#endif
    
    /* Adjust index j (for the last generation) */
#if (S48_USE_STATIC_SPACE)
    j = (j >= S48_GENERATIONS_COUNT - 1) ? S48_GENERATIONS_COUNT - 2 : j ;
#else
    j = (j >= S48_GENERATIONS_COUNT) ? S48_GENERATIONS_COUNT - 1 : j ;
#endif
    
    /* promotion targets */
    set_targets(generations[i].other_space, generations[j].current_space);

    /* Wilson's opportunistic object promotion targets */
    if ( i != j ) {
    reset_young_targets(generations[i].other_space,
			generations[i].current_space);
    }

#if (BIBOP_LOG)
    s48_bibop_log("generations[%i].other_space -> generations[%i].current_space",
		  i, j);
#endif    

    /* actions: the ones that will be evacuated now */
    set_gc_actions(generations[i].other_space, GC_ACTION_COPY_SMALL,
		   GC_ACTION_MARK_LARGE, GC_ACTION_COPY_WEAK);
    /* ignore the ones that will be filled now */
    set_gc_actions(generations[i].current_space, GC_ACTION_IGNORE,
		   GC_ACTION_IGNORE, GC_ACTION_IGNORE);
  }
  
  /* FPage 8 */
  /* initialize the areas that are not collected this time */
  for (i = count; i < S48_GENERATIONS_COUNT; i++) {
    /* trace only what will be added to these */
    /* maybe only the first "old" one will have to be traced ?? */
    FOR_ALL_AREAS(generations[i].current_space->small_area,
		  area->trace = area->frontier);
    FOR_ALL_AREAS(generations[i].current_space->large_area,
		  area->trace = area->frontier);
    
    /* the other spaces should be empty anyway 
     except the last generation with the radioactive-decay-model */
    if (i == S48_GENERATIONS_COUNT - 1) {
#if (S48_USE_RDM)
      ;
#else
      set_gc_actions(generations[i].other_space, GC_ACTION_ERROR,
		     GC_ACTION_ERROR, GC_ACTION_ERROR);
#endif
    }

    set_gc_actions(generations[i].current_space, GC_ACTION_IGNORE,
	 	   GC_ACTION_IGNORE, GC_ACTION_IGNORE);
  }
}

inline static void clear_space(Space* space) {
  FOR_ALL_AREAS(space->small_area, s48_free_area(area)); space->small_area = NULL;
  FOR_ALL_AREAS(space->large_area, s48_free_area(area)); space->large_area = NULL;
  FOR_ALL_AREAS(space->weaks_area, s48_free_area(area)); space->weaks_area = NULL;  
}

static char trace_areas(Area* areas) {
  char hit = 0;

  while (areas != NULL) {
    while (1) {
      s48_address start = areas->trace;
      s48_address end = areas->frontier;
      if (start != end) {
	s48_internal_trace_locationsB(areas, start, end, "trace_areas");
	areas->trace = end;
	hit = 1;
      } else
	break;
    }
    areas = areas->next;
  }
  
  return hit;
}

static void do_gc() {
  char hit;
  do {
    int i;
    char hit0, hit1;
    hit = FALSE;

    /* maybe it's enough to trace up to max_gen+1 */
    for (i = 0; i < S48_GENERATIONS_COUNT; i++) {
      hit0 = trace_areas(generations[i].current_space->small_area);
      hit1 = trace_areas(generations[i].current_space->large_area);
      hit = hit0 || hit1 || hit;
    }

  } while ( hit );
}

inline static void swap(Generation* g) {
  Space* tmp = g->current_space;
  g->current_space = g->other_space;
  g->other_space = tmp;
}

#if (MEASURE_GC)
/* We include this here, because the measurement code uses static
   variables from here a lot */
#include "measure.ci"
#endif

long s48_gc_count() {
  return gc_count;
}

long s48_gc_run_time(long* mseconds) {
  *mseconds = gc_mseconds;
  return gc_seconds;
}

static unsigned long aging_space_survival;

/* collect the first COUNT generations */
/* FPage 5 ... */
static void collect(int count, psbool emergency) {
  int i; 
#if (S48_USE_STATIC_SPACE)
  psbool major_p = (count == (S48_GENERATIONS_COUNT - 1));
#else
  psbool major_p = (count == S48_GENERATIONS_COUNT);
#endif

  /* this it for the water mark changing at the end of the collection */
#if (S48_ADJUST_WATER_MARK)
  aging_space_survival = 0;
  aging_space_before = 0;
  FOR_ALL_AREAS(generations[0].current_space->small_area,
		aging_space_before += (area->frontier - area->start));
#endif

  /* swap spaces and update age_count first */
  /* FPage 5 */
  for (i = 0; i < count; i++) {
    swap(&generations[i]);
    generations[i].age_count++;
    
#if (BIBOP_LOG)
    s48_bibop_log("swapped current <-> other generation %i", i);
#endif
  }

  /* update self_count for the generation from wich the collection
     starts */
  generations[count-1].self_count++;

  /* Initialize actions, targets and trace pointers */
  /* FPage 6 - 7 - 8 */
  init_areas(count);

  /* trace all roots to the younger generations */
#if (S48_USE_REMEMBERED_SETS)
  for (i = 0; i < count; i++) {
    RemSet* rs; RemSet* nrs;
    FOR_ALL_AREAS(generations[i].other_space->small_area,
		  s48_trace_remset(area->remset));
    FOR_ALL_AREAS(generations[i].other_space->weaks_area,
		  s48_trace_remset(area->remset));
    /* beause large areas are "reused", the remembered set has to be
       freed explicitly */
    FOR_ALL_AREAS(generations[i].other_space->large_area,
		  nrs = s48_make_remset();
		  rs = area->remset;
		  area->remset = nrs;
		  s48_trace_remset(rs);
		  s48_free_remset(rs);
		  );
  }
#endif

#if (S48_USE_GENERATION_INDEXING)
  /* compute the absolute minimum and maximum of all generations 
     that are collected */
  minimum_index = 255; maximum_index = 0;
  for (i = 0; i < count; i++) {
    FOR_ALL_AREAS(generations[i].other_space->small_area,
		  minimum_index = int_min(minimum_index, area->minimum_index);
		  maximum_index = int_max(maximum_index, area->maximum_index));
    FOR_ALL_AREAS(generations[i].other_space->large_area,
		  minimum_index = int_min(minimum_index, area->minimum_index);
		  maximum_index = int_max(maximum_index, area->maximum_index));
  }  

  /* generations that are being collected, can be ignored */
  for (i = int_max(count, minimum_index); i <= maximum_index; i++) {
      s48_bibop_log("GEN_INDEX\nTracing roots from %i generation ...\n", i);

#else
    /* FPage 9 ... */
  for (i = count; i < S48_GENERATIONS_COUNT; i++) {
#endif

#if (BIBOP_LOG)
    s48_bibop_log("Tracing roots from current-space of generation %i\n", i);
#endif

    /* FPage 9 ... area_roots.c */
    s48_trace_areas_roots(generations[i].current_space->small_area, count);
    s48_trace_areas_roots(generations[i].current_space->large_area, count);
  }

#if (S48_USE_RDM)
  if (count != S48_GENERATIONS_COUNT) {
    unsigned char last;
#if (S48_USE_STATIC_SPACE)
    last = S48_GENERATIONS_COUNT-2;
#else
    last = S48_GENERATIONS_COUNT-1;
#endif

    s48_bibop_log("RDM\nTracing roots from other-space of generation %i\n",
		  last);

    s48_trace_areas_roots(generations[last].other_space->small_area, count);
    s48_trace_areas_roots(generations[last].other_space->large_area, count);
  }
#endif

  s48_gc_root();

  /* do the tracing until everything is done */
  do_gc();

  /* clean up*/
  for (i = 0; i < S48_GENERATIONS_COUNT; i++) {
    clean_weak_pointers(generations[i].current_space->weaks_area); 
  }

#if (S48_USE_RDM)
#if (S48_USE_STATIC_SPACE)
  clean_weak_pointers(generations[S48_GENERATIONS_COUNT-2].other_space->weaks_area);
#else
  clean_weak_pointers(generations[S48_GENERATIONS_COUNT-1].other_space->weaks_area);
#endif
#endif

  s48_post_gc_cleanup(major_p, emergency);

  for (i = 0; i < count; i++) {    
    clear_space(generations[i].other_space);
    generations[i].last_size = calc_generation_size(&generations[i]);
  }

  /* reset creation space */
#if (S48_ADJUST_WATER_MARK)
  if (aging_space_before != 0)
    adjust_water_mark((float)aging_space_survival / aging_space_before);
#endif
  recreate_creation_space();

  gc_count++;
}

static psbool do_collect(psbool force_major, psbool emergency);

/* FPage 4 - 5  */
void s48_collect(psbool force_major) {
  /*
    The BIBOP GC's heap gets an absolute maximal size with the -h flag
    of scheme48.

    -h <heap_size> : means <heap_size> cells (0 means no size limit).

    Without the -h flag, the heap size gets a default value
    (init.c).  We have to calculate a minimal heap size, set by the
    special configuration of BIBOP (gc_config.h), to decide during the
    initialization (s48_initialize_bibop_heap()) if the given
    <heap_size> is reasonable or not. This is done after the
    allocation of the image_areas (s48_initialize_image_areas()). If
    the maximal heap size is too small we increase it to a reasonable
    value (the user is informed about that).
    
    The variable 'actual_heap_size' (in cells) is updated before each
    collection and represents the total size of all used areas (but
    without allocated free areas, and without the memory used for the
    various structures like Area, Metapage, Generation etc.). If this
    actual heap size rises above the user defined (or default) maximal
    heap size, we halt the program.

*/
  unsigned long user_defined_hsize, heap_live_size;
  psbool was_major;
  long start_seconds, start_mseconds, end_seconds, end_mseconds;

  start_seconds = s48_run_time(&start_mseconds);

  was_major = do_collect(force_major, FALSE);

  heap_live_size = S48_BYTES_TO_CELLS(s48_heap_live_size());
  user_defined_hsize = s48_max_heap_size();

  if ((user_defined_hsize > 0) &&
      (heap_live_size > (user_defined_hsize *
			 ((100.0 - S48_EMERGENCY_PERCENTAGE)/100.0)))) {
    /* try again with a major collection. If it's still too much
       afterwards, quit. */
    if (! was_major)
      do_collect(TRUE, TRUE);
    heap_live_size = S48_BYTES_TO_CELLS(s48_heap_live_size());
    if (heap_live_size > user_defined_hsize)
      s48_gc_error("Scheme 48 heap overflow (max heap size %i cells)\n",
		   user_defined_hsize);
  }
  end_seconds = s48_run_time(&end_mseconds);
  if (end_mseconds >= start_mseconds) {
    gc_seconds = gc_seconds + (end_seconds - start_seconds);
    gc_mseconds = gc_mseconds + (end_mseconds - start_mseconds);
  }
  else {
    gc_seconds = gc_seconds + ((end_seconds - start_seconds) - 1); 
    gc_mseconds = gc_mseconds + ((1000 + end_mseconds) - start_mseconds);
  }
}

static psbool do_collect(psbool force_major, psbool emergency) {
  /* Which generations should be collected? */

  /* collect up to the oldest generation that has grown enough since
     its last collection. */
  /* The youngest generation is collected always */
  /* FPage 5 */
  int c;  /* generation number */

#if (BIBOP_LOG)

  /* all current sizes */
#if (S48_COLLECTION_THRESHOLD)
  s48_bibop_log("S48_COLLECTION_THRESHOLD: %i", S48_COLLECTION_THRESHOLD);
#endif

#if (S48_COLLECTION_HEAP_LIMIT)
  s48_bibop_log("S48_COLLECTION_HEAP_LIMIT: %i", S48_COLLECTION_HEAP_LIMIT);
#endif

#if (S48_COLLECTION_AGE_LIMIT)
  s48_bibop_log("S48_COLLECTION_AGE_LIMIT: %i", S48_COLLECTION_AGE_LIMIT);
#endif

 { int i;
  for (i = S48_GENERATIONS_COUNT - 1; i > -1; i--) {
    unsigned long current_size;    
    current_size = calc_generation_size(&generations[i]);

#if (S48_COLLECTION_THRESHOLD)
    s48_bibop_log("Generation %i : %i ", i, (current_size - generations[i].last_size));
#endif

#if (S48_COLLECTION_HEAP_LIMIT)
  s48_bibop_log("Generation %i : %i ", i, current_size);

#endif

#if (S48_COLLECTION_AGE_LIMIT)
  s48_bibop_log("Generation %i : %i ", i, generations[i].age_count);
#endif
  }}
#endif /* #if (BIBOP_LOG) */
  
  /* Which generation should be collected? */
#if (S48_USE_STATIC_SPACE)
  c = S48_GENERATIONS_COUNT - 1;
#else
  c = S48_GENERATIONS_COUNT;
#endif

#if (S48_USE_RDM)
  if (rdm_level == RDM_BEFORE_SWAP) {
    /* if the last generation is full, then swap the spaces. */
    if ((current_size - generations[c-1].last_size) > rdm_threshold) {
      s48_bibop_log("RDM_BEFORE_SWAP\nSwapped <-> Generation %i", c-1);
      swap(&generations[c-1]);
      rdm_level = RDM_AFTER_SWAP;
    }
  } else {
    /* otherwise collect all generations */
    if (current_size > rdm_threshold) {
      s48_bibop_log("RDM_AFTER_SWAP\nSwapped <-> Generation %i", c-1);
      swap(&generations[c-1]); /* they're swaped again in collect() */
      rdm_level = RDM_AFTER_COLLECTION;
      force_major = TRUE;
    }
  }
#endif

  if (! force_major) {
   for (; c > 1; c--) {
    unsigned long current_size;
    current_size = calc_generation_size(&generations[c-1]);
    
#if (S48_COLLECTION_THRESHOLD)
    if ((current_size - generations[c-1].last_size) > S48_COLLECTION_THRESHOLD)
      break;
#endif

#if (S48_COLLECTION_HEAP_LIMIT)
    if (current_size > S48_COLLECTION_HEAP_LIMIT)
      break;
#endif

#if (S48_COLLECTION_AGE_LIMIT)
    if (generations[c-1].self_count > 0 &&
	generations[c-1].self_count % S48_COLLECTION_AGE_LIMIT == 0)
      break;
#endif

   }
  }

#if (BIBOP_LOG)
  s48_bibop_log("Generation choosen: %i", c-1);
  s48_bibop_log("Starting Collection...");
#endif

#if (MEASURE_GC)
  measure_before_collection(c);
#endif

  /*************************************/
  /* Now is going to be really collected */
  /* FPage 5 ... */
  collect(c, emergency);
  /*************************************/

#if (S48_USE_RDM)
#if (S48_USE_STATIC_SPACE)
    if ((c == S48_GENERATIONS_COUNT - 1) && (rdm_level == RDM_AFTER_COLLECTION)) {
#else
  if ((c == S48_GENERATIONS_COUNT) && (rdm_level == RDM_AFTER_COLLECTION)) {
#endif
    rdm_threshold = (S48_RDM_MAX_SIZE - generations[c-1].last_size) / 2;
    if (rdm_threshold < S48_RDM_MIN_THRESHOLD)
      rdm_threshold = S48_RDM_MIN_THRESHOLD;

#if (BIBOP_LOG)
    s48_bibop_log("RDM_AFTER_COLLECTION\nrdm_threshold = %i", rdm_threshold);
#endif

    rdm_level = RDM_BEFORE_SWAP;
  }
#endif

#if (MEASURE_GC)
  measure_after_collection(c);
#endif

#if (BIBOP_LOG)
  s48_bibop_log("Collection done!");
#endif

#if (S48_USE_STATIC_SPACE)
  return (c == S48_GENERATIONS_COUNT-1);
#else
  return (c == S48_GENERATIONS_COUNT);
#endif
}

/*********************************************************************
 Tracing
 ********************************************************************/

/* forward declarations */

inline static void mark_large(Area* area, Space* to_space);
inline static Area* allocate_small_area(Space* space,
					unsigned int size_in_bytes);
inline static Area* allocate_weak_area(Space* space);
inline static Area* allocate_large_area(Space* space,
					unsigned int size_in_bytes);

inline static void call_internal_write_barrier(Area* maybe_area, s48_address addr,
					       s48_value stob, Area* to_area) {
  if ((maybe_area != NULL) && (maybe_area->generation_index > 0))
    s48_internal_write_barrier(maybe_area, addr, stob, to_area);
}

inline static void call_internal_write_barrier2(Area* maybe_area, s48_address addr,
						s48_value stob) {
  if ((maybe_area != NULL) && (maybe_area->generation_index > 0)) {
    s48_internal_write_barrier(maybe_area, addr, stob,
			       s48_memory_map_ref((s48_address)stob));
  }
}
  

/* EKG checks for broken hearts - only used internally in
   s48_trace_locationsB */

#define EKG(label) {\
    long header = S48_STOB_HEADER(trace_stob_stob);\
    if (BROKEN_HEART_P((s48_value)header)) {\
      *((s48_value*)addr) = header;\
      call_internal_write_barrier2(maybe_area, addr,\
				  (s48_value)header);\
      addr = next;\
      goto loop;\
    } else {\
      copy_header = header;\
      copy_thing = trace_stob_stob;\
      goto label;\
    }\
  }

/*  Copy everything pointed to from somewhere between START (inclusive)
    and END (exclusive).
*/

void s48_internal_trace_locationsB(Area* maybe_area, s48_address start,
				   s48_address end, char* called_from) {
  s48_address addr = start;
  s48_address next;
  s48_value thing;

  s48_value trace_stob_stob;
  long copy_header;
  s48_value copy_thing;
  Area* copy_area;
  Area* from_area;
  Space* copy_to_space;

 loop: {
    if (addr < end) {
      thing = *((s48_value*) addr);
      next = S48_ADDRESS_INC(addr);
      goto trace;
    }
    return;
  }

 trace: {
   if (S48_HEADER_P(thing)) {
     if (S48_B_VECTOR_HEADER_P(thing)) {
       addr = next + S48_HEADER_LENGTH_IN_A_UNITS(thing);
     }
     else if (S48_HEADER_TYPE(thing) == S48_STOBTYPE_CONTINUATION) {
       unsigned long size = S48_HEADER_LENGTH_IN_A_UNITS(thing);
       extern void s48_trace_continuation(char *, long); /* BIBOP-specific */
       s48_trace_continuation(next, size);
       addr = next + size;
     } else {
       addr = next;
     }
     goto loop;
   } else if (! S48_STOB_P(thing)) {
     addr = next;
     goto loop;
   } else if (S48_STOB_P(thing)){
     trace_stob_stob = thing;
     goto trace_stob;
   } else {
     s48_gc_error("illegal s48_value found while tracing address 0x%X called from %s",
		  addr, called_from);
   }
 }


 trace_stob: { /* parameter: trace_stob_stob */
   from_area = s48_memory_map_ref(S48_ADDRESS_AT_HEADER(trace_stob_stob));
   if (from_area != NULL) {
     switch (from_area->action) {
     case GC_ACTION_COPY_SMALL: {
       copy_to_space = from_area->target_space;
       EKG(copy_small);
     } break;
     case GC_ACTION_COPY_MIXED: {
       copy_to_space = from_area->target_space;
       EKG(copy_mixed);
     } break;
     case GC_ACTION_COPY_WEAK: {
       copy_to_space = from_area->target_space;
       EKG(copy_weak_pointer);
     } break;
     case GC_ACTION_IGNORE: {
       call_internal_write_barrier(maybe_area, addr, trace_stob_stob, from_area);
       addr = next;
       goto loop;
     } break;
     case GC_ACTION_MARK_LARGE: {
       copy_to_space = from_area->target_space;
       mark_large(from_area, copy_to_space);
       /* a large object has been "copied" */
       call_internal_write_barrier(maybe_area, addr, trace_stob_stob, from_area);
       addr = next;
       goto loop;
     } break;
     case GC_ACTION_ERROR: {
       s48_gc_error("got error gc-action in the %i generation", from_area->generation_index + 1);
       return; /* Never reached */
     } break;
     default: {
       s48_gc_error("got unexpected gc-action in the %i generation", from_area->generation_index + 1);
       return; /* Never reached */
     }
     }
   }
   else {
     s48_gc_error("illegal stob descriptor found while tracing address 0x%X called from %s",
		  addr, called_from);
     return; /* Never reached */
   }
 }
 
  assert(FALSE); /* we should never get here */

 /* Find out which is the actual copy_area for small, large, etc. object */
 copy_small: { /* parameter: copy_to_space, copy_header, copy_thing */
   /* get the current Area of the copy_space, means target_space */
   Area* area = copy_to_space->small_area;
   unsigned int size_in_bytes = (S48_HEADER_LENGTH_IN_A_UNITS(copy_header)
				 + S48_STOB_OVERHEAD_IN_A_UNITS);
   if (size_in_bytes <= AREA_REMAINING(area))
     
     /* If the object passes then this is the copy_area ...*/
     copy_area = area;
   else
     /*  otherwise, allocate a small area in this space */    
     copy_area = allocate_small_area(copy_to_space, size_in_bytes);
   goto copy_object;
 }

 copy_large: { /* parameter: copy_to_space, copy_header, copy_thing */
   copy_area = allocate_large_area( copy_to_space,
				    S48_HEADER_LENGTH_IN_BYTES(copy_header) +
				    S48_STOB_OVERHEAD_IN_BYTES );
   goto copy_object;
 }

 copy_mixed: { /* parameter: copy_to_space, copy_header, copy_thing */
   if (S48_STOBTYPE_WEAK_POINTER == S48_HEADER_TYPE(copy_header))
     goto copy_weak_pointer; /* uses copy_to_space, copy_thing! */
   else if (S48_HEADER_LENGTH_IN_BYTES(copy_header) < S48_SMALL_OBJECT_LIMIT)
     goto copy_small; /* uses copy_to_space, copy_thing, copy_header! */
   else
     goto copy_large; /* dito */
 }

 copy_weak_pointer: { /* parameter: copy_to_space, copy_thing */
   Area* area = copy_to_space->weaks_area;
   /*copy_header = WEAK_POINTER_HEADER;*/
   if ((int) (S48_HEADER_LENGTH_IN_A_UNITS(copy_header)
	      + S48_STOB_OVERHEAD_IN_A_UNITS)
       < AREA_REMAINING(area))
     copy_area = area;
   else
     copy_area = allocate_weak_area(copy_to_space);
   goto copy_object;
 }

 copy_object: { /* parameter: from_area, copy_thing, copy_header, copy_area */

   /* we start writing at the frontier location */
   s48_address frontier = copy_area->frontier;

   /* the data, means after the header, will be written at this location */
   s48_address data_addr = frontier + S48_STOB_OVERHEAD_IN_A_UNITS;

   /* Since the s48_address is allways 4 bytes, the lower 2 bits are allways 00 */
   /* We use these 2 bits for the STOB-TAG: 11 to make a scheme-stob */
   s48_value new = S48_ADDRESS_TO_STOB_DESCRIPTOR(data_addr);

#if (S48_ADJUST_WATER_MARK)
   /* count small object-sizes, that survive in the first generation */
   if ((from_area->generation_index == 0) &&
       (from_area != creation_space.small_below) &&
       (from_area != creation_space.small_above) &&
       (from_area->action == GC_ACTION_COPY_SMALL))
     aging_space_survival += S48_HEADER_LENGTH_IN_A_UNITS(copy_header) +
       S48_STOB_OVERHEAD_IN_BYTES;
#endif

   /* count every surviving obj */
#if (MEASURE_GC)
   all_surviving_obj += S48_HEADER_LENGTH_IN_A_UNITS(copy_header) +
     S48_STOB_OVERHEAD_IN_BYTES;
#endif

   /* copy the object to the new location */
   /* first the header at the frontier location */
   *((s48_value*)frontier) = copy_header;

   /* and then the data (thing addresss after header) at the data_addr
      location */
   assert(AREA_REMAINING(copy_area) >= (S48_HEADER_LENGTH_IN_BYTES(copy_header)
					+ S48_STOB_OVERHEAD_IN_BYTES));
	  
   memcpy((void*)data_addr, S48_ADDRESS_AFTER_HEADER(copy_thing, void),
	  S48_HEADER_LENGTH_IN_BYTES(copy_header));

   /* break heart */
   /* alternative: S48_STOB_HEADER(copy_thing) = new;*/
   *((s48_value*)S48_ADDRESS_AT_HEADER(copy_thing)) = new;

   /* advance the allocation pointer */
   copy_area->frontier = data_addr + S48_HEADER_LENGTH_IN_A_UNITS(copy_header);

   /* overwrite the old stob with the new one */
   *((s48_value*)addr) = new;

   /* if we are tracing an area, from an older generation call write_barrier */
     call_internal_write_barrier(maybe_area, addr, new, copy_area);

   /* continue behind that stob */
   addr = next;
   goto loop;
 }
} /* end: trace_locationsB */


void s48_trace_locationsB(s48_address start, s48_address end) {
  Area* maybe_area = s48_memory_map_ref(start);
  s48_internal_trace_locationsB(maybe_area, start, end, "s48_trace_locationsB");
}

/* s48_trace_value passes the location of STOB to
   s48_trace_locationsB. */

s48_value s48_trace_value(s48_value stob) {
  s48_address addr = (s48_address)&stob;
  (void)s48_trace_locationsB(addr, S48_ADDRESS_INC(addr));
  /* stob now holds the new location of the value... */
  return stob;
}

/* s48_trace_stob_contentsB passes the contents of a d-vector stob to
   s48_trace_locations. Never call this with b-vectors! */

void s48_trace_stob_contentsB(s48_value stob) {
  s48_address start = (s48_address)S48_ADDRESS_AFTER_HEADER(stob, void);
  unsigned int size = S48_BYTES_TO_A_UNITS(S48_STOB_BYTE_LENGTH(stob));
  s48_trace_locationsB(start, (start + size));
}

/* creating new areas during gc */

inline static Area* allocate_small_area(Space* space,
					unsigned int size_in_bytes) {
  Area* area = s48_allocate_area(int_max(S48_MINIMUM_SMALL_AREA_SIZE,
					 BYTES_TO_PAGES(size_in_bytes)),
				 int_max(S48_MAXIMUM_SMALL_AREA_SIZE,
					 BYTES_TO_PAGES(size_in_bytes)),
				 (unsigned char)space->generation_index,
				 AREA_TYPE_SIZE_SMALL);
  area->action = GC_ACTION_IGNORE;
  area->next = space->small_area;
  space->small_area = area;

  return area;
}

inline static Area* allocate_large_area(Space* space,
					unsigned int size_in_bytes) {
  unsigned int pages = BYTES_TO_PAGES(size_in_bytes);
  Area* area = s48_allocate_area(pages,
				 pages,
				 (unsigned char)space->generation_index, 
				 AREA_TYPE_SIZE_LARGE);
  area->action = GC_ACTION_IGNORE;
  area->next = space->large_area;
  space->large_area = area;

  return area;
}

inline static Area* allocate_weak_area(Space* space) {
  Area* area = s48_allocate_area(S48_MINIMUM_WEAK_AREA_SIZE,
				 S48_MAXIMUM_WEAK_AREA_SIZE,
				 (unsigned char)space->generation_index,
				 AREA_TYPE_SIZE_WEAKS);
  area->action = GC_ACTION_IGNORE;
  area->next = space->weaks_area;
  space->weaks_area = area;

  return area;
}

/* Remove AREA from from-space's list and put it on to-space's.  Ignore
   AREA from now on.
*/

inline static void delete_large_area(Area* large_area) {
  char hit = FALSE;
  FOR_ALL_AREAS(creation_space.large,
		if (area == large_area) hit = TRUE);
  if (hit)
    creation_space.large = s48_delete_area(creation_space.large, large_area);
  else {
    Space* from_space = generations[large_area->generation_index].other_space;
    from_space->large_area = s48_delete_area(from_space->large_area, large_area);
  }
}

inline static void mark_large(Area* area, Space* to_space) {
  delete_large_area(area);

  area->next = to_space->large_area;
  to_space->large_area = area;
  area->action = GC_ACTION_IGNORE;
  area->generation_index = to_space->generation_index;
  area->trace = area->start;
}

/*********************************************************************
 Allocation
*********************************************************************/

long s48_available() {
  /* it's not 100% sure that all these cells can be allocated, because
     if an object does not fit into area_below, the remaining space is
     discarded. Is this a bad thing ?? */
  /* If the heap can grow, the remaining memory in the creation space
     is available. If it can't, we have to consider, that the actually
     allocated heap (s48_heap_size) cannot grow above
     s48_max_heap_size(). So less space is really available. */

  long max_heap_size = s48_max_heap_size();
  long available_creation_space =
    S48_BYTES_TO_CELLS(AREA_REMAINING(creation_space.small_below)+
		       AREA_REMAINING(creation_space.small_above));
  if (max_heap_size == 0)
    return available_creation_space;
  else {
    long virtually_available =
      max_heap_size - S48_BYTES_TO_CELLS(s48_heap_live_size());
    if (virtually_available < available_creation_space)
      return virtually_available;
    else
      return available_creation_space;
  }
}

void s48_forbid_gcB() {
  gc_forbid_count++;
}

void s48_allow_gcB() {
  /*assert(gc_forbid_count > 0);*/
  gc_forbid_count--;
}

/* Small Objects */

/* FPage 2 - 3 - 4 */
void s48_make_availableAgc(long len_in_bytes) {

#if (BIBOP_LOG)
  int i; /* for the generations-loop */
  int before_size[S48_GENERATIONS_COUNT];
  int after_size[S48_GENERATIONS_COUNT];
#endif

  /* let's see if we run out of space in the current area... */
  if (AREA_REMAINING(creation_space.small) < len_in_bytes) {
    /* if we are under the water-mark, then continue above it... */
    if ((creation_space.small == creation_space.small_below) &&
	(len_in_bytes <= AREA_REMAINING(creation_space.small_above))) {

      /* FPage 2 */
      creation_space.small = creation_space.small_above;
    }
    
    /* While the image is still loading, and the creation_space is
       full, then the creation_space.small points to the small area
       created in current_space of the first generation. In this case
       the allocation is going on in this small area without memory
       limit. When the image has already been read, then a gc is
       allowed and the very next allocation of an object triggers the
       first collection.

       Just for the history: with a
       S48_CREATION_SPACE_SIZE of 512 KB (128 Pages as default), after
       the image is been loaded, there are 89.133 objects in the heap:
       
       - 14.177 objects in small_above area
       - 16.579 objects in the small_below area
       - 58.377 objects(!) in 19 areas in the current_space 
                of the first generation.

       That is, only 1/3 comes into the creation_space and the rest
       2/3 into the first generation, which causes a big delay by the
       first collection. I'll change this, by increasing the
       creation_space_size 3 times.

       From now on, the creation_space alone is responsible for the
       next collections: if we are above it already, and are allowed
       to collect some garbage, then do it. */

    else {

      if (gc_forbid_count == 0) {
      /* FPage 4 */

#if (BIBOP_LOG)
      s48_bibop_log("CREATION SPACE WATER MARK");
      s48_bibop_log("creation_space.small_above: %i pages",
		    S48_CREATION_SPACE_SIZE - current_water_mark);
      s48_bibop_log("creation_space.small_below: %i pages\n",
		    current_water_mark);
      /*s48_bibop_log("NEW OBJECTS");
      s48_bibop_log("Bytes small_above = %i", get_creation_space_small_above());
      s48_bibop_log("Bytes small_below = %i", get_creation_space_small_below());*/
      
      if (s48_gc_count() == 0) {
	s48_bibop_log("Bytes small_gen   = %i",
		      calc_generation_size(&generations[0]));
      }
      s48_bibop_log("");
      
      /* save the current size before the collection */
      for (i = 0; i < S48_GENERATIONS_COUNT; i++) {
	before_size[i] = calc_generation_size(&generations[i]);
      }

#endif

        s48_collect(FALSE);

#if (BIBOP_LOG)
      /* save the current size after the collection */
      for (i = 0; i < S48_GENERATIONS_COUNT; i++) {
	after_size[i] = calc_generation_size(&generations[i]);
      }
      
      s48_bibop_log("AFTER COLLECTION");
      for (i = S48_GENERATIONS_COUNT - 1; i > -1; i--) {
	s48_bibop_log("gen: %i, last size %i, current size %i",
		      i,
		      before_size[i],
		      after_size[i]);
      }

      s48_bibop_log("");
#endif
      }

      /* if a gc is not allowed, or if after the collection, the
	 creation-space is still too small, just use the first
	 generation to allocate space, and allocate a new area if
	 needed. */
      if ((gc_forbid_count != 0) ||
	  (AREA_REMAINING(creation_space.small) < len_in_bytes)) {
	/* FPage 3 */
	creation_space.small = generations[0].current_space->small_area;
	if (AREA_REMAINING(creation_space.small) < len_in_bytes) {
	  Area* new_area =
	    s48_allocate_area(int_max(S48_MINIMUM_SMALL_AREA_SIZE,
				      BYTES_TO_PAGES(len_in_bytes)),
			      int_max(S48_MAXIMUM_SMALL_AREA_SIZE,
				      BYTES_TO_PAGES(len_in_bytes)),
			      0, 
			      AREA_TYPE_SIZE_SMALL);
	  new_area->next = generations[0].current_space->small_area;
	  generations[0].current_space->small_area = new_area;
	  creation_space.small = new_area;
	}
      }
    }
  }

  if (AREA_REMAINING(creation_space.small) < len_in_bytes)
    s48_gc_error("out of memory in s48_make_availableAgc(%d)", len_in_bytes);
}


s48_address s48_allocate_small(long len_in_bytes) {
  s48_address result;
  /* catch misuse of this function */
  /*assert(len_in_bytes <= S48_SMALL_OBJECT_LIMIT);*/

  result = creation_space.small->frontier;
  creation_space.small->frontier += S48_BYTES_TO_A_UNITS(len_in_bytes);
  assert(creation_space.small->frontier <= creation_space.small->end);

  return result;
}

/* Large Objects */

static void s48_make_large_availableAgc(long len_in_bytes) {
  unsigned long current_size = 0;
  /* maybe keep tracking the size while allocating... */
  FOR_ALL_AREAS(creation_space.large,
		current_size += (area->frontier - area->start));
  if ((gc_forbid_count == 0) &&
      (current_size > S48_MAXIMUM_LARGE_CREATION_SPACE_SIZE)) {
    s48_collect(FALSE);
  }
  else {
    ;
  }
}

static s48_address s48_allocate_large(long len_in_bytes) {
  unsigned long len_in_pages = BYTES_TO_PAGES(len_in_bytes);
  Area* area = s48_allocate_area(len_in_pages, len_in_pages, 0,  AREA_TYPE_SIZE_LARGE);
  area->frontier = area->start + len_in_bytes;
  area->next = creation_space.large;
  creation_space.large = area;
  return area->start;
}

/* "Mixed" Objects */

inline static s48_address allocate_mixedAgc(long len_in_bytes) {
  if (len_in_bytes <= S48_SMALL_OBJECT_LIMIT) {
    s48_make_availableAgc(len_in_bytes);
    return s48_allocate_small(len_in_bytes);
  } else {
    s48_make_large_availableAgc(len_in_bytes);
    return s48_allocate_large(len_in_bytes);
  }
}

/* 

The Allocator (s48_allocate_tracedAgc)

- If called from the portable Dumper: the len_in_bytes is of one
object only. All objects are allocated one after the other.

- If called from the TSC-Dumper: the len_in_bytes is of the whole
image. At this phase a gc is forbidden. So if the creation_space is
not big enough, we got a problem. So could increase the size of the
creation_space to allocate the whole image, but we can't copy it
blind, cause the static_space is divided in 3 areas: small, large and
weaks obejcts. We have to find these 3 kinds of obejct separately !!!
*/
   
s48_address s48_allocate_tracedAgc(long len_in_bytes) {
  return allocate_mixedAgc(len_in_bytes);
}

s48_address s48_allocate_untracedAgc(long len_in_bytes) {
  return allocate_mixedAgc(len_in_bytes);
}

/* Unmovable Objects are going into the large area which is collected
   with the non-copy algorithmus */
s48_address s48_allocate_untraced_unmovableAgc(long len_in_bytes) {
    s48_make_large_availableAgc(len_in_bytes);
    return s48_allocate_large(len_in_bytes);
}

/* Weak Pointers */

s48_address s48_allocate_weakAgc(long len_in_bytes) {
  Area* area = creation_space.weaks;
  s48_address result;

  if (AREA_REMAINING(area) < len_in_bytes) {
    if (gc_forbid_count == 0) {
      s48_collect(FALSE);
      area = creation_space.weaks;
    }
    else {
      Area** areap = &generations[0].current_space->weaks_area;
      if (AREA_REMAINING(*areap) < len_in_bytes) {
	Area* new_area = s48_allocate_area(S48_MINIMUM_WEAK_AREA_SIZE,
					   S48_MAXIMUM_WEAK_AREA_SIZE,
					   0, AREA_TYPE_SIZE_WEAKS);
	new_area->next = *areap;
	*areap = new_area;
      }
      area = *areap;
    }
  }
  if (AREA_REMAINING(area) < len_in_bytes)
    /* this should be impossible */
    s48_gc_error("out of memory in s48_allocate_weakAgc(%d).", len_in_bytes);

  result = area->frontier;
  area->frontier += S48_BYTES_TO_A_UNITS(len_in_bytes);
  return (result);
}

/*********************************************************************
 Walking down the heap
*********************************************************************/

inline static void walk_areas(void (*do_part)(s48_address, s48_address),
			      Area* areas) {
  while (areas != NULL) {
    do_part(areas->start, areas->frontier);
    areas = areas->next;
  }
}

void s48_walk_heap(void (*do_part)(s48_address, s48_address)) {
  int i;
  walk_areas(do_part, creation_space.small_below);
  walk_areas(do_part, creation_space.small_above);
  walk_areas(do_part, creation_space.large);
  walk_areas(do_part, creation_space.weaks);

  for (i = 0; i < S48_GENERATIONS_COUNT; i++) {
    walk_areas(do_part, generations[i].current_space->small_area);
    walk_areas(do_part, generations[i].current_space->large_area);
    walk_areas(do_part, generations[i].current_space->weaks_area);
  }
  
#if (S48_USE_RDM)
  walk_areas(do_part, generations[S48_GENERATIONS_COUNT-1].other_space->small_area);
  walk_areas(do_part, generations[S48_GENERATIONS_COUNT-1].other_space->large_area);
  walk_areas(do_part, generations[S48_GENERATIONS_COUNT-1].other_space->weaks_area);
#endif
}
 
 
/* Special area initialization for the BIBOP undumper in the last
   generation */

 
/*****************************************************************************/
void s48_initialize_image_areas(long small_bytes, long small_hp_d,
				long large_bytes, long large_hp_d,
				long weaks_bytes, long weaks_hp_d) {
   
   s48_address start;
   s48_address small_end;
   s48_address large_end;
   s48_address end;
   long img_bytes;
   long i;
   
   Area* small_img;
   Area* large_img;
   Area* weaks_img;   

   /*Wrong image format ? */
   if ((small_bytes < 0) || (large_bytes < 0) || (weaks_bytes < 0))  return;

   /*Get a block */
   img_bytes = small_bytes + large_bytes + weaks_bytes;
   
   s48_allocate_image_area(img_bytes, &start, &end);
   
   /*printf("START: %i  END %i\n", start, end); */

   
   if (img_bytes != (end - start)) {
     s48_gc_error("Image block is not OK!");
   }
   
   small_end = start + small_bytes;
   large_end = small_end + large_bytes;
   
   
   /* Set the start addresses */
   s48_set_new_small_start_addrB(start);
   s48_set_new_large_start_addrB(small_end);
   s48_set_new_weaks_start_addrB(large_end);
   
   
   /* Split this block and assign it to the last generation's areas */
   small_img = s48_make_area(start, small_end, start,
			     S48_GENERATIONS_COUNT - 1, AREA_TYPE_SIZE_SMALL);
   small_img->frontier += S48_BYTES_TO_A_UNITS(small_hp_d);
   small_img->action = GC_ACTION_IGNORE;
   generations[S48_GENERATIONS_COUNT - 1].current_space->small_area =
     small_img;
   
   large_img = s48_make_area(small_end, large_end, small_end,
			     S48_GENERATIONS_COUNT - 1, AREA_TYPE_SIZE_LARGE);
   large_img->frontier += S48_BYTES_TO_A_UNITS(large_hp_d);
   large_img->action = GC_ACTION_IGNORE;
   generations[S48_GENERATIONS_COUNT - 1].current_space->large_area =
     large_img;
   
   weaks_img = s48_make_area(large_end, end, large_end,
			     S48_GENERATIONS_COUNT - 1, AREA_TYPE_SIZE_WEAKS);
   weaks_img->frontier += S48_BYTES_TO_A_UNITS(weaks_hp_d);
   weaks_img->action = GC_ACTION_IGNORE;
   generations[S48_GENERATIONS_COUNT - 1].current_space->weaks_area =
     weaks_img;
   
   /* Put the areas into all memory-map cells that are covered by
      it. */
   for (i = 0; i < BYTES_TO_PAGES(small_bytes); i++) {
     s48_memory_map_setB(ADD_PAGES(start, i), small_img);
   }
   
   for (i = 0; i < BYTES_TO_PAGES(large_bytes); i++) {
     s48_memory_map_setB(ADD_PAGES(small_end, i), large_img);
   }
   
   for (i = 0; i < BYTES_TO_PAGES(weaks_bytes); i++) {
     s48_memory_map_setB(ADD_PAGES(large_end, i), weaks_img);
   }
   
   
   /* Debugging */
   /*printf("small: %i  %i\n", small_img->start, small_img->end); */
   /*printf("large: %i  %i\n", large_img->start, large_img->end); */
   /*printf("weaks: %i  %i\n", weaks_img->start, weaks_img->end); */

   return;

 }

 
/*********************************************************************/
 long s48_get_new_small_size(void) {
/*********************************************************************/
   s48_address start;
   s48_address end;
   
   start = generations[S48_GENERATIONS_COUNT - 1].current_space->small_area->start;
   end = generations[S48_GENERATIONS_COUNT - 1].current_space->small_area->end;
   
   return end - start;
 }
 
/*********************************************************************/
 long s48_get_new_large_size(void) {
/*********************************************************************/
   s48_address start;
   s48_address end;
   
   start = generations[S48_GENERATIONS_COUNT - 1].current_space->large_area->start;
   end = generations[S48_GENERATIONS_COUNT - 1].current_space->large_area->end;
   
   return end - start;
 }
 
/*********************************************************************/
 long s48_get_new_weaks_size(void) {
/*********************************************************************/
   s48_address start;
   s48_address end;
   
   start = generations[S48_GENERATIONS_COUNT - 1].current_space->weaks_area->start;
   end = generations[S48_GENERATIONS_COUNT - 1].current_space->weaks_area->end;
   
   return end - start;
 }
 
/*********************************************************************/
 char *   s48_get_new_small_end_addr(void) {
/*********************************************************************/
   return generations[S48_GENERATIONS_COUNT - 1].current_space->small_area->end;
 }
 
/*********************************************************************/
 char *   s48_get_new_large_end_addr(void) {
/*********************************************************************/
   return generations[S48_GENERATIONS_COUNT - 1].current_space->large_area->end;
 }
 
/*********************************************************************/
 char *   s48_get_new_weaks_end_addr(void) {
/*********************************************************************/
   return generations[S48_GENERATIONS_COUNT - 1].current_space->weaks_area->end;
 }


/*********************************************************************/
 void s48_check_heap_sizeB() {
/*********************************************************************/

   unsigned long max_size = s48_max_heap_size(); /* cells */
   extern long s48_min_heap_size(void);
   unsigned long min_size = s48_min_heap_size(); /* cells */
   
   /*Check the given heap size (flag -h) and the actual one */
   
   if (max_size == 0) {
     printf("Warning: Without a maximum heap size, infinite recursions can cause the system to run out of memory.\n");
   } else if (min_size > max_size) {
     s48_set_max_heap_sizeB( min_size );
     printf("Maximum heap size %i is too small, using %i cells instead.\n", max_size,
	    s48_max_heap_size());
   }
 }

