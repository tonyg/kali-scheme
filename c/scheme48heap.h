/*
 * Externally visible objects defined in scheme48heap.c.
 */
extern long		available(void);
extern void		begin_collection(void);
extern long		do_gc(long, long*);
extern void		end_collection(void);
extern long		find_all(long);
extern long		find_all_records(long);
extern long		gc_count(void);
extern long		heap_size(void);
extern char		image_writing_okayP(void);
extern long		trace_locationsB(char *, char *);
extern long		trace_stob_contentsB(long);
extern long		trace_value(long);
extern long		write_image(long, FILE *, void(*)(void));
