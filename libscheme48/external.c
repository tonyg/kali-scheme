/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <stdarg.h>

#include "scheme48/libprescheme/c-mods.h"
#include "scheme48/libscheme48/scheme48.h"
#include "scheme48/libscheme48/scheme48vm.h"
#include "scheme48/libscheme48/bignum.h"
#include "scheme48/libscheme48/globals.h"
#include "scheme48/libscheme48/records.h"

/*
 * The Joy of C
 * I don't understand why we need this, but we do.
 */

struct s_jmp_buf {
  jmp_buf buf;
};

/*
 * Longjump target set up by the most recent call into C.
 */
static struct s_jmp_buf	current_return_point;

/*
 * The name of the procedure we are currently executing; used for error messages.
 */
S48_DECL_GLOBAL (new_current_procedure);

static s48_value current_procedure;

/*
 * Stack of Scheme stack-block records which represent portions of the process
 * stack.
 */
S48_DECL_GLOBAL (new_current_stack_block);

static s48_value current_stack_block = S48_FALSE;

/*
 * These need to agree with the record definition in callback.scm.
 */
enum stack_block_offsets {
  stack_block_free,
  stack_block_unwind,
  stack_block_proc,
  stack_block_thread,
  stack_block_next,
};

#define STACK_BLOCK_FREE(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 0)
#define STACK_BLOCK_UNWIND(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 1)
#define STACK_BLOCK_PROC(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 2)
#define STACK_BLOCK_THREAD(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 3)
#define STACK_BLOCK_NEXT(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 4)

/* Miscellaneous new api stuff that needs to eventually be pushed into
 * the VM.
 */
s48_error_t
s48_new_push (s48_arena_t arena,
              s48_word_t * value)
{
  s48_push (*value);
  return 0;
}

s48_error_t
s48_new_resetup_external_exception (s48_word_t * result,
                                    s48_arena_t arena,
                                    long why,
                                    long n_args)
{
  *result = s48_resetup_external_exception (why, n_args);
  return 0;
}

s48_error_t
s48_new_setup_external_exception (s48_arena_t arena,
                                  long why,
                                  long nargs)
{
  s48_setup_external_exception (why, nargs);
  return 0;
}


s48_error_t
s48_new_restart (s48_word_t * result,
                 s48_arena_t arena,
                 s48_word_t * proc,
                 long n_args)
{
  *result = s48_restart (*proc, n_args);
  return 0;
}


void s48_new_raise_scheme_exception (s48_arena_t arena, long why, long nargs, ...);

/*
 * For debugging.
 */
/*

static int
callback_depth()
{
  int depth = 0;
  s48_value stack = current_stack_block;

  for(; stack != S48_FALSE; depth++, stack = STACK_BLOCK_NEXT(stack));

  return depth;
}
*/
/*
 * The value being returned from an external call.  The returns may be preceded
 * by a longjmp(), so we stash the value here.
 */
S48_DECL_GLOBAL (new_external_return_value);

static s48_value external_return_value;

/* Exports to Scheme */
static s48_value	s48_clear_stack_top(void);
static s48_value	s48_trampoline(s48_value proc, s48_value nargs);
static s48_value	s48_system(s48_value string);

/* Imports from Scheme */
S48_DECL_GLOBAL (new_the_record_type_binding);
S48_DECL_GLOBAL (new_stack_block_type_binding);
S48_DECL_GLOBAL (new_callback_binding);
S48_DECL_GLOBAL (new_delay_callback_return_binding);

s48_value the_record_type_binding = S48_FALSE;
static s48_value 	stack_block_type_binding = S48_FALSE;
static s48_value 	callback_binding = S48_FALSE;
static s48_value 	delay_callback_return_binding = S48_FALSE;

void
s48_new_get_imported_binding (s48_word_t * result,
                              s48_arena_t arena,
                              char * binding)
{
  *result = s48_get_imported_binding (binding);
}

S48_DECL_GLOBAL_INIT (s48_external_init_globals);
void
s48_external_init_globals (s48_arena_t arena)
{
  struct s48_external_init_globals_frame
    {
      S48_FRAME;
      s48_word_t tmp;
    } l;

  S48_PROTECT_FRAME (l);

#define IMPORT(var, string) \
  do { \
    s48_new_get_imported_binding (&l.tmp, arena, string); \
    s48_global_set (arena, var, &l.tmp); \
  } while (0)

  IMPORT (new_the_record_type_binding, "s48-the-record-type");
  IMPORT (new_stack_block_type_binding, "s48-stack-block-type");
  IMPORT (new_callback_binding, "s48-callback");
  IMPORT (new_delay_callback_return_binding, "s48-delay-callback-return");

#undef IMPORT

  S48_UNPROTECT_FRAME (l);
}

void
s48_initialize_external()
{
  S48_GC_PROTECT_GLOBAL(the_record_type_binding);
  the_record_type_binding = s48_get_imported_binding("s48-the-record-type");

  S48_GC_PROTECT_GLOBAL(stack_block_type_binding);
  stack_block_type_binding = s48_get_imported_binding("s48-stack-block-type");

  S48_GC_PROTECT_GLOBAL(callback_binding);
  callback_binding = s48_get_imported_binding("s48-callback");

  S48_GC_PROTECT_GLOBAL(delay_callback_return_binding);
  delay_callback_return_binding =
    s48_get_imported_binding("s48-delay-callback-return");

  S48_GC_PROTECT_GLOBAL(current_stack_block);
  S48_GC_PROTECT_GLOBAL(current_procedure);

  S48_EXPORT_FUNCTION(s48_clear_stack_top);
  S48_EXPORT_FUNCTION(s48_trampoline);
  S48_EXPORT_FUNCTION(s48_system);
}

/* The three reasons for an extern-call longjump. */

#define NO_THROW        0
#define EXCEPTION_THROW 1
#define CLEANUP_THROW   2

/*
 * Used to call `proc' from Scheme code. `nargs' the number of arguments in
 * vector `argv'.  If `spread_p' is true the procedure is applied to the
 * arguments, otherwise `proc' is just called on `nargs' and `argv'.
 *
 * We do a setjmp() to get a return point for clearing off this portion of
 * the process stack.  This is used when `proc' calls back to Scheme and
 * then a throw transfers control up past the call to `proc'.
 */

typedef s48_value (*proc_0_t)(void);
typedef s48_value (*proc_1_t)(s48_value);
typedef s48_value (*proc_2_t)(s48_value, s48_value);
typedef s48_value (*proc_3_t)(s48_value, s48_value, s48_value);
typedef s48_value (*proc_4_t)(s48_value, s48_value, s48_value, s48_value);
typedef s48_value (*proc_5_t)(s48_value, s48_value, s48_value, s48_value,
			      s48_value);
typedef s48_value (*proc_6_t)(s48_value, s48_value, s48_value, s48_value,
			      s48_value, s48_value);
typedef s48_value (*proc_7_t)(s48_value, s48_value, s48_value, s48_value,
			      s48_value, s48_value, s48_value);
typedef s48_value (*proc_8_t)(s48_value, s48_value, s48_value, s48_value,
			      s48_value, s48_value, s48_value, s48_value);
typedef s48_value (*proc_9_t)(s48_value, s48_value, s48_value, s48_value,
			      s48_value, s48_value, s48_value, s48_value,
			      s48_value);
typedef s48_value (*proc_10_t)(s48_value, s48_value, s48_value, s48_value,
			       s48_value, s48_value, s48_value, s48_value,
			       s48_value, s48_value);
typedef s48_value (*proc_11_t)(s48_value, s48_value, s48_value, s48_value,
			       s48_value, s48_value, s48_value, s48_value,
			       s48_value, s48_value, s48_value);
typedef s48_value (*proc_12_t)(s48_value, s48_value, s48_value, s48_value,
			       s48_value, s48_value, s48_value, s48_value,
			       s48_value, s48_value, s48_value, s48_value);
typedef s48_value (*proc_n_t)(int, s48_value []);

s48_value
s48_external_call(s48_value sch_proc, s48_value proc_name,
		  long nargs, char *char_argv)
{
  volatile char *gc_roots_marker;	/* volatile to survive longjumps */
  volatile s48_value name = proc_name;	/* volatile to survive longjumps */
  
  /* int depth = callback_depth(); */	/* debugging */

  long *argv = (long *) char_argv;

  proc_0_t proc = S48_EXTRACT_VALUE(sch_proc, proc_0_t);

  int throw_reason;

  current_procedure = name;

  S48_CHECK_VALUE(sch_proc);
  S48_CHECK_STRING(name);

  gc_roots_marker = s48_set_gc_roots_baseB();

  /* fprintf(stderr, "[external_call at depth %d]\n", depth); */

  throw_reason = setjmp(current_return_point.buf);

  if (throw_reason == NO_THROW) {	/* initial entry */
    switch (nargs) {
    case 0:
      external_return_value = proc();
      break;
    case 1:
      external_return_value = ((proc_1_t)proc)(argv[0]);
      break;
    case 2:
      external_return_value = ((proc_2_t)proc)(argv[1], argv[0]);
      break;
    case 3:
      external_return_value = ((proc_3_t)proc)(argv[2], argv[1], argv[0]);
      break;
    case 4:
      external_return_value = ((proc_4_t)proc)(argv[3], argv[2], argv[1], argv[0]);
      break;
    case 5:
      external_return_value = ((proc_5_t)proc)(argv[4],
					       argv[3], argv[2], argv[1], argv[0]);
      break;
    case 6:
      external_return_value = ((proc_6_t)proc)(argv[5], argv[4],
					       argv[3], argv[2], argv[1], argv[0]);
      break;
    case 7:
      external_return_value = ((proc_7_t)proc)(argv[6], argv[5], argv[4],
					       argv[3], argv[2], argv[1], argv[0]);
      break;
    case 8:
      external_return_value = ((proc_8_t)proc)(argv[7], argv[6], argv[5], argv[4],
					       argv[3], argv[2], argv[1], argv[0]);
      break;
    case 9:
      external_return_value = ((proc_9_t)proc)(argv[8],
					       argv[7], argv[6], argv[5], argv[4],
					       argv[3], argv[2], argv[1], argv[0]);
      break;
    case 10:
      external_return_value = ((proc_10_t)proc)(argv[9], argv[8],
						argv[7], argv[6], argv[5], argv[4],
						argv[3], argv[2], argv[1], argv[0]);
      break;
    case 11:
      external_return_value = ((proc_11_t)proc)(argv[10], argv[9], argv[8],
						argv[7], argv[6], argv[5], argv[4],
						argv[3], argv[2], argv[1], argv[0]);
      break;
    case 12:
      external_return_value = ((proc_12_t)proc)(argv[11], argv[10], argv[9], argv[8],
						argv[7], argv[6], argv[5], argv[4],
						argv[3], argv[2], argv[1], argv[0]);
      break;
    default:
      external_return_value = ((proc_n_t)proc)((int)nargs, (s48_value *)argv);
    }

    /* Raise an exception if the user neglected to pop off some gc roots. */
    
    if (! s48_release_gc_roots_baseB((char *)gc_roots_marker)) {
      s48_raise_scheme_exception(S48_EXCEPTION_GC_PROTECTION_MISMATCH, 0);
    }
    
    /* Clear any free stack-blocks off of the top of the stack-block stack and
       then longjmp past the corresponding portions of the process stack. */
    
    if (current_stack_block != S48_FALSE &&
	STACK_BLOCK_FREE(current_stack_block) == S48_TRUE) {

      s48_value bottom_free_block;
      
      do {
	bottom_free_block = current_stack_block;
	current_stack_block = STACK_BLOCK_NEXT(current_stack_block);
      }
      while (current_stack_block != S48_FALSE &&
	     STACK_BLOCK_FREE(current_stack_block) == S48_TRUE);
      
      /* fprintf(stderr, "[Freeing stack blocks from %d to %d]\n",
	                 depth,
	                 callback_depth()); */

      longjmp(S48_EXTRACT_VALUE_POINTER(STACK_BLOCK_UNWIND(bottom_free_block),
					struct s_jmp_buf)->buf,
	      CLEANUP_THROW);
    }
  }
  else {	/* throwing an exception or uwinding the stack */
    /* fprintf(stderr, "[external_call throw; was %d and now %d]\n",
                       depth,
                      callback_depth());
     fprintf(stderr, "[throw unrolling to %ld]\n", gc_roots_marker); */
    s48_release_gc_roots_baseB((char *)gc_roots_marker);
  }

  /* Check to see if a thread is waiting to return to the next block down. */
     
  if (current_stack_block != S48_FALSE &&
      STACK_BLOCK_THREAD(current_stack_block) != S48_FALSE) {
    /* fprintf(stderr, "[releasing return at %d]\n", callback_depth()); */

    if (throw_reason == EXCEPTION_THROW) {
      /* We are in the midst of raising an exception, so we need to piggyback
	 our exception on that one. */
      s48_value old_exception
	= s48_resetup_external_exception(S48_EXCEPTION_CALLBACK_RETURN_UNCOVERED,
					 2);
      s48_push(old_exception);
      s48_push(current_stack_block);
      external_return_value = S48_UNSPECIFIC;
    }
    else {
      s48_setup_external_exception(S48_EXCEPTION_CALLBACK_RETURN_UNCOVERED, 2);
      s48_push(current_stack_block);
      s48_push(external_return_value);
      external_return_value = S48_UNSPECIFIC;
    }
  }

  return external_return_value;
}

typedef s48_error_t (*new_proc_0_t)(s48_word_t *, s48_arena_t);
typedef s48_error_t (*new_proc_1_t)(s48_word_t *, s48_arena_t, s48_word_t *);
typedef s48_error_t (*new_proc_2_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_3_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_4_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_5_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_6_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_7_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_8_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_9_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_10_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_11_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_12_t)(s48_word_t *, s48_arena_t, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *, s48_word_t *);
typedef s48_error_t (*new_proc_n_t)(s48_word_t *, s48_arena_t, int, s48_word_t *);

static s48_error_t
do_external_call (s48_word_t * result,
                  s48_arena_t arena,
                  new_proc_0_t proc,
                  int nargs,
                  s48_word_t * argv)
{
  switch (nargs)
    {
    case 0: return ((new_proc_0_t)proc) (result, arena);
    case 1: return ((new_proc_1_t)proc) (result, arena, &argv[0]);
    case 2: return ((new_proc_2_t)proc) (result, arena, &argv[1], &argv[0]);
    case 3: return ((new_proc_3_t)proc) (result, arena, &argv[2], &argv[1], &argv[0]);
    case 4: return ((new_proc_4_t)proc) (result, arena, &argv[3], &argv[2], &argv[1], &argv[0]);
    case 5: return ((new_proc_5_t)proc) (result, arena, &argv[4], &argv[3], &argv[2], &argv[1], &argv[0]);
    case 6: return ((new_proc_6_t)proc) (result, arena, &argv[5], &argv[4], &argv[3], &argv[2], &argv[1], &argv[0]);
    case 7: return ((new_proc_7_t)proc) (result, arena, &argv[6], &argv[5], &argv[4], &argv[3], &argv[2], &argv[1], &argv[0]);
    case 8: return ((new_proc_8_t)proc) (result, arena, &argv[7], &argv[6], &argv[5], &argv[4], &argv[3], &argv[2], &argv[1], &argv[0]);
    case 9: return ((new_proc_9_t)proc) (result, arena, &argv[8], &argv[7], &argv[6], &argv[5], &argv[4], &argv[3], &argv[2], &argv[1], &argv[0]);
    case 10: return ((new_proc_10_t)proc) (result, arena, &argv[9], &argv[8], &argv[7], &argv[6], &argv[5], &argv[4], &argv[3], &argv[2], &argv[1], &argv[0]);
    case 11: return ((new_proc_11_t)proc) (result, arena, &argv[10], &argv[9], &argv[8], &argv[7], &argv[6], &argv[5], &argv[4], &argv[3], &argv[2], &argv[1], &argv[0]);
    case 12: return ((new_proc_12_t)proc) (result, arena, &argv[11], &argv[10], &argv[9], &argv[8], &argv[7], &argv[6], &argv[5], &argv[4], &argv[3], &argv[2], &argv[1], &argv[0]);
    default: return ((new_proc_n_t)proc) (result, arena, nargs, argv);
    }
}


s48_error_t
s48_new_external_call (s48_word_t * result,
                       s48_arena_t arena,
                       s48_word_t * sch_proc,
                       s48_word_t * proc_name,
                       long nargs,
                       char * char_argv)
{
  struct s48_new_external_call_locals
    {
      S48_FRAME;
      s48_word_t old_exception;
      s48_word_t bottom_free_block;

#undef FIXME
      /* This is set to 10 instead of 12 currently because
         S48_PROTECT_FRAME won't allow us to protect more than 12
         variables in a single frame, and here this includes
         `old_exception' and `bottom_free_block'.  Once local support
         is fixed, this can be changed back. */
      s48_word_t args[10];
    } l;

  /* volatile to survive longjumps */
  /* Actually, I don't think this needs to be volatile, since it's not
     modified after the setjmp call. -mrd */
  volatile char * gc_roots_marker;

  s48_error_t maybe_error = 0;

  int throw_reason;

  new_proc_0_t proc;
  s48_word_t * argv;

  S48_CHECK_VALUE (*sch_proc);
  S48_CHECK_STRING (*proc_name);

  s48_value_extract (arena, sch_proc, &proc, sizeof (proc));
  argv = (s48_word_t *)char_argv;

  s48_global_set (arena, new_current_stack_block, proc_name);

  gc_roots_marker = s48_set_gc_roots_baseB ();

  throw_reason = setjmp (current_return_point.buf);

  S48_PROTECT_FRAME (l);

  /* initial entry */
  if (throw_reason == NO_THROW)
    {
      int i;

#undef FIXME
      /* Just a temporary invariant.  (See explanation above.) */
      if (nargs > 10)
        {
          fprintf (stderr, "Too many arguments to s48_new_external_call()\n");
          exit (1);
        }

      for (i = 0; i < nargs; ++i)
        S48_LSET (&l.args[i], &argv[i]);

      maybe_error = do_external_call (result, arena, proc, nargs, l.args);

      /* Raise an exception if the user neglected to pop off some gc
         roots. */

      if (!s48_release_gc_roots_baseB ((char *)gc_roots_marker))
        {
          s48_new_raise_scheme_exception (arena, S48_EXCEPTION_GC_PROTECTION_MISMATCH,
                                          0);
        }

      /* Clear any free stack-blocks off of the top of the stack-block
         stack and then longjmp past the corresponding portions of the
         process stack. */
      /* Why? -mrd */

      if (   !s48_is_false (arena, &current_stack_block)
          && s48_is_true (arena, &STACK_BLOCK_FREE (current_stack_block)))
        {
          do
            {
              S48_LSET (&l.bottom_free_block, &current_stack_block);
              current_stack_block = STACK_BLOCK_NEXT (current_stack_block);
            }
          while (   !s48_is_false (arena, &current_stack_block)
                 && s48_is_true (arena, &STACK_BLOCK_FREE (current_stack_block)));

          longjmp (S48_EXTRACT_VALUE_POINTER (STACK_BLOCK_UNWIND (l.bottom_free_block), struct s_jmp_buf)->buf,
                   CLEANUP_THROW);
        }
    }
  else
    {
      /* throwing an exception or uwinding the stack */

      s48_release_gc_roots_baseB ((char *)gc_roots_marker);
    }

  /* Check to see if a thread is waiting to return to the next block
     down. */

  if (   !s48_is_false (arena, &current_stack_block)
      && !s48_is_false (arena, &STACK_BLOCK_THREAD (current_stack_block)))
    {
      if (throw_reason == EXCEPTION_THROW)
        {
          /* We are in the midst of raising an exception, so we need
	     to piggyback our exception on that one. */
          s48_new_resetup_external_exception (&l.old_exception, arena,
                                              S48_EXCEPTION_CALLBACK_RETURN_UNCOVERED,
                                              2);
          s48_new_push (arena, &l.old_exception);
          s48_new_push (arena, &current_stack_block);
          s48_make_unspecific (result, arena);
        }
      else
        {
          s48_new_setup_external_exception (arena, S48_EXCEPTION_CALLBACK_RETURN_UNCOVERED, 2);
          s48_new_push (arena, &current_stack_block);
          s48_new_push (arena, &external_return_value);
          s48_make_unspecific (result, arena);
        }
    }

  S48_UNPROTECT_FRAME (l);
  return maybe_error;
}

s48_value
s48_new_external_call_old_api (s48_value sch_proc,
                               s48_value proc_name,
                               long nargs,
                               char * char_argv)
{
  struct s48_new_external_call_old_api_frame
    {
      S48_FRAME;
      s48_word_t sch_proc;
      s48_word_t proc_name;
      s48_word_t result;
    } l;

  s48_arena_t arena = 0;

  S48_PROTECT_FRAME (l);

  l.sch_proc = sch_proc;
  l.proc_name = proc_name;

  s48_new_external_call (&l.result, arena, &l.sch_proc, &l.proc_name, nargs, char_argv);

  S48_UNPROTECT_FRAME (l);
  return l.result;
}


/*
 * Call Scheme function `proc' from C.  We push the call-back depth,
 * `proc', and the arguments on the Scheme stack and then restart the
 * VM.  The restarted VM calls the Scheme procedure `callback' which
 * wraps the call to `proc' with a dynamic-wind.  This prevents
 * downward throws back into the call to `proc', which C can't handle,
 * and allows the C stack to be cleaned up if an upward throw occurs.
 *
 * The maximum number of arguments is determined by the amount of
 * space reserved on the Scheme stack for exceptions. See the
 * definition of stack-slack in scheme/vm/stack.scm.
 */

s48_error_t
s48_new_call_scheme (s48_word_t * result,
                     s48_arena_t arena,
                     s48_word_t * proc,
                     ...)
{
  struct s48_new_call_scheme_frame
    {
      S48_FRAME;
      s48_word_t value;
      s48_word_t unwind;
      s48_word_t stack_block;
      s48_word_t proc;
      s48_word_t binding;
      s48_word_t tmp;
    } l;

  int i;
  s48_word_t * arg;
  long n_args = 0;
  va_list ap;

  S48_PROTECT_FRAME (l);

  s48_global_ref (&l.tmp, arena, new_callback_binding);
  s48_shared_binding_check (arena, &l.tmp);

  s48_new_make_value (&l.unwind, arena, &current_return_point, sizeof (current_return_point));

  s48_global_ref (&l.binding, arena, new_stack_block_type_binding);
  s48_global_ref (&l.proc, arena, new_current_procedure);
  s48_global_ref (&l.stack_block, arena, new_current_stack_block);

  s48_make_false (&l.tmp, arena);
  s48_new_make_record (&l.stack_block,
                       arena,
                       &l.binding,
                       &l.unwind,
                       &l.proc,
                       &l.stack_block,
                       &l.tmp,
                       &l.tmp,
                       s48_va_end);

  s48_global_set (arena, new_current_stack_block, &l.stack_block);

  s48_new_push (arena, &l.stack_block);
  s48_new_push (arena, proc);

  va_start (ap, proc);
  while ((arg = va_arg (ap, s48_word_t *)) != s48_va_end)
    {
      s48_new_push (arena, arg);
      ++n_args;
    }
  va_end (ap);

  s48_new_restart (&l.value, arena, &S48_UNSAFE_SHARED_BINDING_REF (callback_binding), n_args + 2);

  while (!s48_values_eq (arena, &s48_Scallback_return_stack_blockS, &current_stack_block))
    {
      if (s48_is_false (arena, &s48_Scallback_return_stack_blockS))
        {
          exit (1);
        }
      else
        {
          /* Someone has returned (because of threads) to the wrong
	     section of the C stack.  We call back to a Scheme
	     procedure that will suspend until our block is at the top
	     of the stack. */

          s48_new_push (arena, &s48_Scallback_return_stack_blockS);
          s48_new_push (arena, &S48_UNSAFE_SHARED_BINDING_REF (delay_callback_return_binding));
          s48_new_push (arena, &s48_Scallback_return_stack_blockS);
          s48_new_push (arena, &l.value);

          s48_disable_interruptsB ();
          s48_new_restart (&l.value, arena, &S48_UNSAFE_SHARED_BINDING_REF (callback_binding), 4);
        }
    }

  /* Restore the state of the current stack block. */

  s48_global_ref (&l.stack_block, arena, new_current_stack_block);

  s48_record_ref (&l.unwind, arena, &l.stack_block, stack_block_unwind);
  s48_value_extract (arena, &l.unwind, &current_return_point, sizeof (current_return_point));

  s48_record_ref (&l.tmp, arena, &l.stack_block, stack_block_proc);
  s48_global_set (arena, new_current_procedure, &l.tmp);

  s48_record_ref (&l.stack_block, arena, &l.stack_block, stack_block_next);
  s48_global_set (arena, new_current_stack_block, &l.stack_block);

  S48_LSET (result, &l.value);

  S48_UNPROTECT_FRAME (l);
  return 0;
}


s48_value
s48_call_scheme(s48_value proc, long nargs, ...)
{
  int i;
  va_list arguments;
  s48_value value;
  s48_value unwind, stack_block;
  S48_DECLARE_GC_PROTECT(2);

  S48_GC_PROTECT_2(unwind, proc);
  
  va_start(arguments, nargs);

  S48_SHARED_BINDING_CHECK(callback_binding);

  /* It would be nice to push a list of the arguments, but we have no way
     of preserving them across a cons. */
  if (nargs < 0 || 12 < nargs) {  /* DO NOT INCREASE THIS NUMBER */
    s48_value sch_nargs = s48_enter_integer(nargs);  /* `proc' is protected */
    s48_raise_scheme_exception(S48_EXCEPTION_TOO_MANY_ARGUMENTS_IN_CALLBACK,
			       2, proc, sch_nargs);
  }

  /* fprintf(stderr, "[s48_call, %ld args, depth %d]\n",
	  nargs, callback_depth()); */

  s48_push(S48_UNSPECIFIC);	/* placeholder */
  s48_push(proc);
  for (i = 0; i < nargs; i++)
    s48_push(va_arg(arguments, s48_value));

  va_end(arguments);

  /* With everything safely on the stack we can do the necessary allocation. */

  unwind = S48_MAKE_VALUE(struct s_jmp_buf);
  S48_EXTRACT_VALUE(unwind, struct s_jmp_buf) = current_return_point;

  stack_block = s48_make_record(stack_block_type_binding);
  STACK_BLOCK_UNWIND(stack_block) = unwind;
  STACK_BLOCK_PROC(stack_block) = current_procedure;
  STACK_BLOCK_NEXT(stack_block) = current_stack_block;
  STACK_BLOCK_FREE(stack_block) = S48_FALSE;
  STACK_BLOCK_THREAD(stack_block) = S48_FALSE;

  S48_GC_UNPROTECT();		/* no more references to `unwind' or `proc'. */

  current_stack_block = stack_block;

  /* if(s48_stack_ref(nargs + 1) != S48_UNSPECIFIC)
     fprintf(stderr, "[stack_block set missed]\n"); */

  s48_stack_setB(nargs + 1, stack_block);

  /* fprintf(stderr, "[s48_call, %ld args, depth %d, off we go]\n",
	  nargs, callback_depth()); */

  value = s48_restart(S48_UNSAFE_SHARED_BINDING_REF(callback_binding),
		      nargs + 2);

  for (;s48_Scallback_return_stack_blockS != current_stack_block;) {
    if (s48_Scallback_return_stack_blockS == S48_FALSE) {

      /* fprintf(stderr, "[s48_call returning from VM %ld]\n", callback_depth()); */

      exit(value);
    }
    else {

      /* Someone has returned (because of threads) to the wrong section of the
	 C stack.  We call back to a Scheme procedure that will suspend until
	 out block is at the top of the stack. */

      s48_push(s48_Scallback_return_stack_blockS);
      s48_push(S48_UNSAFE_SHARED_BINDING_REF(delay_callback_return_binding));
      s48_push(s48_Scallback_return_stack_blockS);
      s48_push(value);

      /* fprintf(stderr, "[Premature return, %ld args, depth %d, back we go]\n",
	      nargs, callback_depth()); */

      s48_disable_interruptsB();
      value = s48_restart(S48_UNSAFE_SHARED_BINDING_REF(callback_binding), 4);
    }
  }

  /* Restore the state of the current stack block. */

  unwind = STACK_BLOCK_UNWIND(current_stack_block);
  current_return_point = S48_EXTRACT_VALUE(unwind, struct s_jmp_buf);
  current_procedure = STACK_BLOCK_PROC(current_stack_block);
  current_stack_block = STACK_BLOCK_NEXT(current_stack_block);

  /* fprintf(stderr, "[s48_call returns from depth %d]\n", callback_depth()); */

  return value;
}

/*
 * Because the top of the stack is cleared on the return from every external
 * call, this doesn't have to do anything but exist.
 */

static s48_error_t
s48_new_clear_stack_top (s48_word_t * result,
                         s48_arena_t arena)
{
  *result = s48_clear_stack_top ();
}

static s48_value
s48_clear_stack_top()
{
  /* fprintf(stderr, "[Clearing stack top]\n"); */
  return S48_UNSPECIFIC;
}

/*
 * For testing callbacks.  This just calls its argument on the specified number
 * of values.
 */

static s48_error_t
s48_new_trampoline (s48_word_t * result,
                    s48_arena_t arena,
                    s48_word_t * proc,
                    s48_word_t * n_args)
{
  *result = s48_trampoline (*proc, *n_args);
  return 0;
}

static s48_value
s48_trampoline(s48_value proc, s48_value nargs)
{

  fprintf(stderr, "[C trampoline, %ld args]\n", S48_UNSAFE_EXTRACT_FIXNUM(nargs));

  switch (s48_extract_fixnum(nargs)) {
  case -2: {
    S48_DECLARE_GC_PROTECT(1);
    
    S48_GC_PROTECT_1(proc);

    return S48_FALSE;
  }
  case -1: {
    long n = - s48_extract_integer(proc);
    fprintf(stderr, "[extract magnitude is %ld (%lx)]\n", n, n);
    return s48_enter_integer(n);
  }
  case 0: {
    s48_value value = s48_call_scheme(proc, 0);
    if (value == S48_FALSE)
      s48_raise_string_os_error("trampoline bouncing");
    return value;
  }
  case 1:
    return s48_call_scheme(proc, 1, s48_enter_fixnum(100));
  case 2:
    return s48_call_scheme(proc, 2, s48_enter_fixnum(100), s48_enter_fixnum(200));
  case 3:
    return s48_call_scheme(proc, 3, s48_enter_fixnum(100), s48_enter_fixnum(200),
		    s48_enter_fixnum(300));
  default:
    s48_raise_range_error(nargs, s48_enter_fixnum(0), s48_enter_fixnum(3));
    return S48_UNDEFINED; /* not that we ever get here */
  }
}

static s48_value
s48_system(s48_value string)
{
  return s48_enter_integer(system((string == S48_FALSE)
				  ? NULL
				  : s48_extract_string(string)));
}

static s48_error_t
s48_new_system (s48_word_t * result,
                s48_arena_t arena,
                s48_word_t * str)
{
  char * command = 0;
  int rv;

  if (!s48_is_false (arena, str))
    {
      s48_string_lock_data (&command, arena, str);
    }

  rv = system (command);

  if (command)
    {
      s48_string_unlock_data (arena, command);
    }

  return s48_fixnum_to_number (result, arena, rv);
}

/********************************/
/*
 * Raising exceptions.  We push the arguments on the stack end then throw out
 * of the most recent call from Scheme.
 *
 * The maximum number of arguments is determined by the amount of space reserved
 * on the Scheme stack for exceptions. See the definition of stack-slack in
 * scheme/vm/stack.scm.
 */
void
s48_new_raise_scheme_exception (s48_arena_t arena,
                                long why, long n_args, ...)
{
  int i;
  va_list irritants;

  s48_setup_external_exception (why, n_args + 1);

  if (n_args > 10) /* DO NOT INCREASE THIS NUMBER */
    {
      fprintf (stderr, "s48_raise_scheme_exception() called with more than 10 arguments, discarding surplus\n");
      n_args = 10;
    }

  s48_new_push (arena, &current_procedure);

  va_start (irritants, n_args);
  for (i = 0; i < n_args; ++i)
    s48_new_push (arena, va_arg (irritants, s48_word_t *));
  va_end (irritants);

  s48_make_unspecific (&external_return_value, arena);
  longjmp (current_return_point.buf, EXCEPTION_THROW);
}

void
s48_raise_scheme_exception(long why, long nargs, ...)
{
  int i;
  va_list irritants;

  va_start(irritants, nargs);

  s48_setup_external_exception(why, nargs + 1);
  
  if (10 < nargs) {   /* DO NOT INCREASE THIS NUMBER */
    fprintf(stderr, "s48_raise_scheme_exception() called with more than 10 arguments, discarding surplus\n");
    nargs = 10;
  }
  
  s48_push(current_procedure);

  for (i = 0; i < nargs; i++)
    s48_push(va_arg(irritants, s48_value));

  va_end(irritants);

   external_return_value = S48_UNSPECIFIC;
   longjmp(current_return_point.buf, EXCEPTION_THROW);
}

void
s48_init_external ()
{
#include "scheme48/libscheme48/external.x"
}
