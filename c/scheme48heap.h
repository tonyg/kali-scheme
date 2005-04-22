/*
 * Externally visible objects defined in scheme48heap.c.
 */

/* initialize top-level variables */
extern void	s48_heap_init(void);

/* called from init.c */
extern void	s48_forbid_gcB(void);
extern void	s48_allow_gcB(void);

/* heap-init interface */
extern void	s48_initialize_heap(long, char *, long);
extern void     s48_initialize_image_areas(long, long, long);
extern void	s48_register_static_areas(long, char **, long *,
					  long, char **, long *);

/* heap interface */
extern long	s48_available(void);
extern char	s48_stob_in_heapP(long);
extern long	s48_heap_size(void);
extern long	s48_find_all(long);
extern long	s48_find_allint(long);
extern long	s48_find_all_records(long);
extern long	s48_gather_objects(char(*)(long), char(*)(char(*)(long)));

/* for reading/writing images (scm-module heap-base ) */
extern void     s48_set_max_heap_sizeB(long);
extern long     s48_max_heap_size(void);
extern char *   s48_get_new_heap_start_addr(void);

extern char *   s48_get_new_small_start_addr(void);
extern void 	s48_set_new_small_start_addrB(char *);
extern char *   s48_get_new_small_end_addr(void);
extern long     s48_get_new_small_size(void);

extern char *   s48_get_new_large_start_addr(void);
extern void 	s48_set_new_large_start_addrB(char *);
extern char *   s48_get_new_large_end_addr(void);
extern long     s48_get_new_large_size(void);

extern char *   s48_get_new_weaks_start_addr(void);
extern void 	s48_set_new_weaks_start_addrB(char *);
extern char *   s48_get_new_weaks_end_addr(void);
extern long     s48_get_new_weaks_size(void);

extern char *	s48_heap_begin(void);	
extern char *	s48_heap_pointer(void);	
extern char *	s48_oldspace_begin(void);	
extern char *	s48_oldspace_end(void);	

extern int	s48_area_type_size(s48_value); /* return value is really an enum */

/* gc interface */
extern void	s48_check_heap_sizeB(void);
extern void	s48_collect(psbool);
extern long	s48_trace_value(long);
extern void	s48_trace_locationsB(char *, char *);
extern void	s48_trace_stob_contentsB(long);
extern void	s48_trace_continuation_contentsB(char *, char *, long);
extern char	s48_extantP(long);
extern long	s48_gc_count(void);

/* allocation interface */
extern void	s48_make_availableAgc(long);
extern char *	s48_allocate_small(long);
extern char *	s48_allocate_tracedAgc(long);
extern char *	s48_allocate_untracedAgc(long);
extern char *   s48_allocate_weakAgc(long);
extern char *	s48_allocate_untraced_unmovableAgc(long);

/* debugging */
extern char	s48_check_heap(long);
