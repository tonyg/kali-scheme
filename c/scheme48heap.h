/*
 * Externally visible objects defined in scheme48heap.c.
 */

/* initialize top-level variables */
extern void	s48_heap_init(void);

/* heap-init interface */
extern void	s48_initialize_heap(long, long);
extern void	s48_register_static_areas(unsigned char, long *, long *,
					  unsigned char, long *, long *);

/* heap interface */
extern long	s48_available(void);
extern long	s48_heap_size(void);
extern long	s48_find_all(long);
extern long	s48_find_all_records(long);
extern char	*s48_ShpS;
extern char	*s48_SlimitS;

/* gc interface */
extern void	s48_begin_collection(void);
extern long	s48_trace_value(long);
extern long	s48_trace_locationsB(char *, char *);
extern long	s48_trace_stob_contentsB(long);
extern void	s48_do_gc(void);
extern void	s48_end_collection(void);
extern char	s48_extantP(long);
extern long	s48_gc_count(void);

/* allocation interface */
extern char	s48_availableP(long);
extern long	s48_preallocate_space(long);
extern char	*s48_allocate_space(long, long, long);
extern void	s48_write_barrier(long, char *, long);

/* images interface */
extern char	s48_image_writing_okayP(void);
extern long	s48_write_image(long, FILE *);
extern long	s48_check_image_header(unsigned char *);
extern long	s48_read_image();

extern long	s48_startup_procedure(void);
extern long	s48_initial_symbols(void);
extern long	s48_initial_imported_bindings(void);
extern long	s48_initial_exported_bindings(void);
extern long	s48_resumer_records(void);
extern long	s48_undumpable_records(long *);

extern void	s48_initialization_completeB(void);
extern void	s48_initializing_gc_root(void);

extern void     s48_set_image_valuesB(long, long, long, long);
