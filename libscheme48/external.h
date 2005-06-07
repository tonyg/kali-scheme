/* external.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Matthew Dempsky
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBSCHEME48__EXTERNAL_H
#define INCLUDE__LIBSCHEME48__EXTERNAL_H


extern s48_value the_record_type_binding;


/* automatically generated __STDC__ prototypes */
extern s48_error_t s48_new_push (s48_arena_t arena,
                                 s48_word_t * value);
extern s48_error_t s48_new_resetup_external_exception (s48_word_t * result,
                                                       s48_arena_t arena,
                                                       long why,
                                                       long n_args);
extern s48_error_t s48_new_setup_external_exception (s48_arena_t arena,
                                                     long why,
                                                     long nargs);
extern s48_error_t s48_new_restart (s48_word_t * result,
                                    s48_arena_t arena,
                                    s48_word_t * proc,
                                    long n_args);
extern void s48_new_get_imported_binding (s48_word_t * result,
                                          s48_arena_t arena,
                                          char * binding);
extern void s48_external_init_globals (s48_arena_t arena);
extern void s48_initialize_external();
extern s48_value s48_external_call(s48_value sch_proc, s48_value proc_name,
                                   long nargs, char *char_argv);
extern s48_error_t s48_new_external_call (s48_word_t * result,
                                          s48_arena_t arena,
                                          s48_word_t * sch_proc,
                                          s48_word_t * proc_name,
                                          long nargs,
                                          char * char_argv);
extern s48_value s48_new_external_call_old_api (s48_value sch_proc,
                                                s48_value proc_name,
                                                long nargs,
                                                char * char_argv);
extern s48_error_t s48_new_call_scheme (s48_word_t * result,
                                        s48_arena_t arena,
                                        s48_word_t * proc,
                                        ...);
extern s48_value s48_call_scheme(s48_value proc, long nargs, ...);
extern void s48_new_raise_scheme_exception (s48_arena_t arena,
                                            long why, long n_args, ...);
extern void s48_raise_scheme_exception(long why, long nargs, ...);
extern void s48_init_external ();
#endif  /* INCLUDE__LIBSCHEME48__EXTERNAL_H */


/* arch-tag: Matthew Dempsky Wed Apr 20 23:03:01 2005 (external.h)
 */
