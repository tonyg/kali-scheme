/*
 * Externally visible objects defined in scheme48vm.c.
 */

#include "scheme48.h"

/* initializing */
extern void		s48_init(void);
extern void		s48_initialize_vm(char *, long);

/* running */
extern long		s48_call_startup_procedure(char **, long);
extern s48_value	s48_restart(s48_value proc, long nargs);
extern s48_value	s48_Scallback_return_stack_blockS;
	  
/* for extension.c */
extern void		s48_set_extension_valueB(s48_value);
extern s48_value	s48_Sextension_valueS;

/* interrupts */
extern void		s48_note_event(void);
extern char		s48_Spending_eventsPS;
extern char		s48_Spending_interruptPS;
extern void		s48_disable_interruptsB(void);
extern void		s48_enable_interruptsB(void);
extern void		s48_set_os_signal(s48_value type, s48_value argument);

/* imported and exported bindings */	  
extern void		s48_define_exported_binding(char *, s48_value);
extern s48_value	s48_get_imported_binding(char *);

/* for raising exceptions in external code */
extern void		s48_setup_external_exception(s48_value exception,
						     long nargs);
extern s48_value	s48_resetup_external_exception(s48_value exception,
						       long additional_nargs);
extern void		s48_push(s48_value value);
extern s48_value	s48_stack_ref(long offset);
extern void		s48_stack_setB(long offset, s48_value value);

/* called when writing an image */
extern s48_value	s48_copy_symbol_table(void);
extern void		s48_mark_traced_channels_closedB(void);
extern s48_value	s48_cleaned_imported_bindings(void);
extern s48_value	s48_exported_bindings(void);

/* called when resuming an image */
extern char	s48_warn_about_undefined_imported_bindings(void);

/* manipulating channels */
extern void		s48_close_channel(long);
extern s48_value	s48_set_channel_os_index(s48_value, long);
extern s48_value	s48_add_channel(s48_value, s48_value, long);

/* external allocation and GC roots */
extern s48_value	s48_allocate_stob(long type, long size);
extern void		s48_push_gc_rootsB(char *, long);
extern char		s48_pop_gc_rootsB(void);
extern char *		s48_set_gc_roots_baseB(void);
extern char		s48_release_gc_roots_baseB(char *);
extern void		s48_register_gc_rootB(char *marker);
