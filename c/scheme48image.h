/*
 * Externally visible objects defined in scheme48read-image.c and
 * scheme48write-image.c.
 */

extern char	s48_image_writing_okayP(void);
extern long	s48_write_image(long, long, FILE *);
extern long	s48_read_image(unsigned char *, long);

extern long	s48_startup_procedure(void);
extern long	s48_initial_symbols(void);
extern long	s48_initial_imported_bindings(void);
extern long	s48_initial_exported_bindings(void);
extern long	s48_resumer_records(void);

extern void	s48_initialization_completeB(void);
extern void	s48_initializing_gc_root(void);

extern void     s48_set_image_valuesB(long, long, long, long);
