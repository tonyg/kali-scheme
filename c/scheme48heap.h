/*
 * Externally visible objects defined in scheme48heap.c.
 */

/* initialize top-level variables */
extern void	s48_heap_init(void);

/* heap-init interface */
extern char *	s48_initialize_heap(long, long, char *);
extern void	s48_register_static_areas(unsigned char, long *, long *,
					  unsigned char, long *, long *);

/* heap interface */
extern long	s48_available(void);
extern long	s48_heap_size(void);
extern long	s48_find_allint(long);
extern long	s48_find_all_records(long);

/* for writing images */
extern char *	s48_heap_begin(void);	
extern char *	s48_heap_pointer(void);	
extern char *	s48_oldspace_begin(void);	
extern char *	s48_oldspace_end(void);	

/* used by the space checking and allocation macros in scheme48mv-prelude.h */
extern char *	s48_ShpS;
extern char *	s48_SlimitS;

/* gc interface */
extern void	s48_collect();
extern long	s48_trace_value(long);
extern long	s48_trace_locationsB(char *, char *);
extern long	s48_trace_stob_contentsB(long);
extern char	s48_extantP(long);
extern long	s48_gc_count(void);

/* allocation interface */
extern void	s48_make_availableAgc(long);
extern char *	s48_allocate_small(long);
extern char *	s48_allocate_tracedAgc(long);
extern char *	s48_allocate_untracedAgc(long);

/* debugging */
extern char	s48_check_heap(long);
