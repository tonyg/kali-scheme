/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <stdarg.h>

#include "c-mods.h"
#include "scheme48.h"
#include "scheme48vm.h"
#include "bignum.h"

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
static s48_value current_procedure;

/*
 * Stack of Scheme stack-block records which represent portions of the process
 * stack.
 */
static s48_value current_stack_block = S48_FALSE;

/*
 * These need to agree with the record definition in callback.scm.
 */
#define STACK_BLOCK_FREE(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 0)
#define STACK_BLOCK_UNWIND(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 1)
#define STACK_BLOCK_PROC(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 2)
#define STACK_BLOCK_THREAD(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 3)
#define STACK_BLOCK_NEXT(stack_block)	S48_UNSAFE_RECORD_REF(stack_block, 4)

/*
 * For debugging.
 */
/*
static int callback_depth()
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
static s48_value external_return_value;

/* Exports to Scheme */
static s48_value	s48_clear_stack_top(void);
static s48_value	s48_trampoline(s48_value proc, s48_value nargs);
static s48_value	s48_system(s48_value string);

/* Imports from Scheme */
static s48_value 	the_record_type_binding = S48_FALSE;
static s48_value 	stack_block_type_binding = S48_FALSE;
static s48_value 	callback_binding = S48_FALSE;
static s48_value 	delay_callback_return_binding = S48_FALSE;

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

/*
 * Call Scheme function `proc' from C.  We push the call-back depth, `proc',
 * and the arguments on the Scheme stack and then restart the VM.  The restarted
 * VM calls the Scheme procedure `callback' which wraps the call to `proc' with
 * a dynamic-wind.  This prevents downward throws back into the call to `proc',
 * which C can't handle, and allows the C stack to be cleaned up if an upward
 * throw occurs.
 *
 * The maximum number of arguments is determined by the amount of space reserved
 * on the Scheme stack for exceptions. See the definition of stack-slack in
 * scheme/vm/stack.scm.
 */
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
      s48_assertion_violation("s48_trampoline", "trampoline bouncing", 0);
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
    s48_assertion_violation("s48_trampoline", "invalid number of arguments", 1, nargs);
    return S48_UNDEFINED; /* not that we ever get here */
  }
}

static s48_value
s48_system(s48_value string)
{
  return s48_enter_integer(system((string == S48_FALSE)
				  ? NULL
				  : s48_extract_byte_vector(string)));
}


/********************************/
/*
 * Raising exceptions.  We push the arguments on the stack end then throw out
 * of the most recent call from Scheme.
 *
 * The maximum number of arguments is determined by the amount of space reserved
 * on the Scheme stack for exceptions. See the definition of stack-slack in
 * scheme/vm/interp/stack.scm.
 */

static long
raise_scheme_exception_prelude(long why, long nargs)
{
  int i;
  va_list irritants;

  s48_setup_external_exception(why, nargs);
  
  if (11 < nargs) {   /* DO NOT INCREASE THIS NUMBER */
    fprintf(stderr, "too many arguments to external exception, discarding surplus\n");
    nargs = 11;
  }
  return nargs;
}

static void
raise_scheme_exception_postlude(void)
{
   external_return_value = S48_UNSPECIFIC;
   longjmp(current_return_point.buf, EXCEPTION_THROW);
}

void
s48_raise_scheme_exception(long why, long nargs, ...)
{
  int i;
  va_list irritants;

  nargs = raise_scheme_exception_prelude(why, nargs + 1) - 1;

  s48_push(current_procedure);

  va_start(irritants, nargs);

  for (i = 0; i < nargs; i++)
    s48_push(va_arg(irritants, s48_value));

  va_end(irritants);

  raise_scheme_exception_postlude();
}

static void
raise_scheme_standard_exception(long why, const char* who, const char* message,
				long irritant_count, va_list irritants)
{
  int i;
  long nargs = irritant_count + 2; /* who and message */

  nargs = raise_scheme_exception_prelude(why, nargs);
  irritant_count = nargs - 2;
  
  for (i = 0; i < irritant_count; i++)
    s48_push(va_arg(irritants, s48_value));

  va_end(irritants);

  /* these must be last because of GC protection */
  if (who == NULL)
    s48_push(current_procedure);
  else
    s48_push(s48_enter_string_utf_8((char*)who));
  s48_push(s48_enter_byte_string((char*)message));

  raise_scheme_exception_postlude();
}

/* Specific exceptions */

void
s48_error(const char* who, const char* message,
	  long irritant_count, ...)
{
  va_list irritants;
  va_start(irritants, irritant_count);
  raise_scheme_standard_exception(S48_EXCEPTION_EXTERNAL_ERROR,
				  who, message, irritant_count, irritants);
}

void
s48_assertion_violation(const char* who, const char* message,
			long irritant_count, ...)
{
  va_list irritants;
  va_start(irritants, irritant_count);
  raise_scheme_standard_exception(S48_EXCEPTION_EXTERNAL_ASSERTION_VIOLATION,
				  who, message, irritant_count, irritants);
}

void s48_os_error(const char* who, int the_errno,
		  long irritant_count, ...)
{
  int i;
  long nargs = irritant_count + 2; /* who and errno */
  va_list irritants;

  nargs = raise_scheme_exception_prelude(S48_EXCEPTION_EXTERNAL_OS_ERROR, nargs);
  irritant_count = nargs - 2;
  
  va_start(irritants, irritant_count);

  for (i = 0; i < irritant_count; i++)
    s48_push(va_arg(irritants, s48_value));

  va_end(irritants);

  /* last because of GC protection */
  if (who == NULL)
    s48_push(current_procedure);
  else
    s48_push(s48_enter_string_utf_8((char*)who));
  s48_push(s48_enter_fixnum(the_errno));

  raise_scheme_exception_postlude();
}

void
s48_out_of_memory_error()
{
  s48_raise_scheme_exception(S48_EXCEPTION_OUT_OF_MEMORY, 0);
}

/* For internal use by the VM: */

void
s48_argument_type_violation(s48_value value) {
  s48_assertion_violation(NULL, "argument-type violation", 1, value);
}

void
s48_range_violation(s48_value value, s48_value min, s48_value max) {
  s48_assertion_violation(NULL, "argument out of range", 3, value, min, max);
}

/* The following are deprecated: */

void
s48_raise_argument_type_error(s48_value value) {
  s48_raise_scheme_exception(S48_EXCEPTION_WRONG_TYPE_ARGUMENT, 1, value);
}

void
s48_raise_argument_number_error(s48_value value, s48_value min, s48_value max) {
  s48_raise_scheme_exception(S48_EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS,
			     3, value, min, max);
}

void
s48_raise_range_error(s48_value value, s48_value min, s48_value max) {
  s48_raise_scheme_exception(S48_EXCEPTION_INDEX_OUT_OF_RANGE,
			     3, value, min, max);
}

void
s48_raise_closed_channel_error() {
  s48_raise_scheme_exception(S48_EXCEPTION_CLOSED_CHANNEL, 0);
}

void
s48_raise_os_error(int the_errno) {
  s48_os_error(NULL, the_errno, 0);
}

void
s48_raise_string_os_error(char *reason) {
  s48_error(NULL, (const char*)s48_enter_string_latin_1(reason), 0);
}

void
s48_raise_out_of_memory_error() {
  s48_raise_scheme_exception(S48_EXCEPTION_OUT_OF_MEMORY, 0);
}


/********************************/
/* Support routines for external code */

/*
 * Type-safe procedures for checking types and dereferencing and setting slots.
 */

int
s48_stob_has_type(s48_value thing, int type)
{
  return S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type);
}

long
s48_stob_length(s48_value thing, int type)
{
  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_assertion_violation("s48_stob_length", "not a stob", 1, thing);
  
  return S48_STOB_DESCRIPTOR_LENGTH(thing);
}

long
s48_stob_byte_length(s48_value thing, int type)
{
  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_assertion_violation("s48_stob_byte_length", "not a stob", 1, thing);

  if (type == S48_STOBTYPE_STRING)
    return S48_STOB_BYTE_LENGTH(thing) - 1;
  else
    return S48_STOB_BYTE_LENGTH(thing);
}

s48_value
s48_stob_ref(s48_value thing, int type, long offset)
{
  long length;

  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_assertion_violation("s48_stob_ref", "not a stob", 1, thing);

  length = S48_STOB_DESCRIPTOR_LENGTH(thing);

  if (offset < 0 || length <= offset)
    s48_assertion_violation("s48_stob_ref", "invalid stob index", 3,
			    s48_enter_integer(offset),
			    S48_UNSAFE_ENTER_FIXNUM(0),
			    S48_UNSAFE_ENTER_FIXNUM(length - 1));
			  
  return S48_STOB_REF(thing, offset);
}

void
s48_stob_set(s48_value thing, int type, long offset, s48_value value)
{
  long length;
  
  if (!(S48_STOB_P(thing) &&
	(S48_STOB_TYPE(thing) == type) &&
	!S48_STOB_IMMUTABLEP(thing)))
    s48_assertion_violation("s48_stob_set", "not a mutable stob", 1, thing);
  
  length = S48_STOB_DESCRIPTOR_LENGTH(thing);

  if (offset < 0 || length <= offset)
    s48_assertion_violation("s48_stob_set", "invalid stob index", 3,
			    s48_enter_integer(offset),
			    S48_UNSAFE_ENTER_FIXNUM(0),
			    S48_UNSAFE_ENTER_FIXNUM(length - 1));
			  
  S48_STOB_SET(thing, offset, value);
}

char
s48_stob_byte_ref(s48_value thing, int type, long offset)
{
  long length;

  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_assertion_violation("s48_stob_byte_ref", "not a stob", 1, thing);
  
  length = (type == S48_STOBTYPE_STRING) ?
           S48_STOB_BYTE_LENGTH(thing) - 1 :
           S48_STOB_BYTE_LENGTH(thing);
  
  if (offset < 0 || length <= offset)
    s48_assertion_violation("s48_stob_byte_ref", "invalid stob index", 3,
			    s48_enter_integer(offset),
			    S48_UNSAFE_ENTER_FIXNUM(0),
			    S48_UNSAFE_ENTER_FIXNUM(length - 1));
			  
  return S48_STOB_BYTE_REF(thing, offset);
}

void
s48_stob_byte_set(s48_value thing, int type, long offset, char value)
{
  long length;

  if (!(S48_STOB_P(thing) && (S48_STOB_TYPE(thing) == type)))
    s48_assertion_violation("s48_stob_byte_set", "not a stob", 1, thing);
  
  length = (type == S48_STOBTYPE_STRING) ?
           S48_STOB_BYTE_LENGTH(thing) - 1 :
           S48_STOB_BYTE_LENGTH(thing);
  
  if (offset < 0 || length <= offset)
    s48_assertion_violation("s48_stob_byte_set", "invalid stob index", 3,
			    s48_enter_integer(offset),
			    S48_UNSAFE_ENTER_FIXNUM(0),
			    S48_UNSAFE_ENTER_FIXNUM(length - 1));
			  
  S48_STOB_BYTE_SET(thing, offset, value);
}

void *
s48_value_pointer(s48_value value)
{
  S48_CHECK_VALUE(value);

  return S48_ADDRESS_AFTER_HEADER(value, void *);
}

/********************************/
/* Numbers, characters, and pointers. */

/*
 * These two functions have the same range as the unsafe macros, but they signal
 * an error if things go wrong, instead of silently producing garbage.  Unlike
 * the integer versions they cannot cause a GC.
 */

s48_value
s48_enter_fixnum(long value)
{
  if (value < S48_MIN_FIXNUM_VALUE || S48_MAX_FIXNUM_VALUE < value)
    s48_assertion_violation("s48_enter_fixnum", "not a fixnum", 1, s48_enter_integer(value));

  return S48_UNSAFE_ENTER_FIXNUM(value);
}

long
s48_extract_fixnum(s48_value value)
{
  if (! S48_FIXNUM_P(value))
    s48_assertion_violation("s48_extract_fixnum", "not a fixnum", 1, value);
  
  return S48_UNSAFE_EXTRACT_FIXNUM(value);
}

/* If we have a fixnum we just extract it. For bignums call the
 * functions in bignum.c.
 */

long
s48_extract_integer(s48_value value)
{
  if (S48_FIXNUM_P(value))
    return S48_UNSAFE_EXTRACT_FIXNUM(value);

  if (S48_BIGNUM_P(value)){
    bignum_type bignum = S48_ADDRESS_AFTER_HEADER(value, long);
    
    if (! s48_bignum_fits_in_word_p(bignum, 32, 1))
      s48_assertion_violation("s48_extract_integer", "does not fit in word", 1, value);
    else return s48_bignum_to_long(bignum);
  }
  else s48_assertion_violation("s48_extract_integer", "not an exact integer", 1, value);
}

/*
 * Doubles and characters are straightforward.
 */

s48_value
s48_enter_double(double value)
{
  s48_value obj;

  obj = s48_allocate_stob(S48_STOBTYPE_DOUBLE, sizeof(double));
  S48_UNSAFE_EXTRACT_DOUBLE(obj) = value;

  return obj;
}

double
s48_extract_double(s48_value s48_double)
{
  if (! S48_DOUBLE_P(s48_double))
    s48_assertion_violation("s48_extract_double", "not a double", 1, s48_double);
  
  return S48_UNSAFE_EXTRACT_DOUBLE(s48_double);
}

s48_value
s48_enter_char(long a_char)
{
  if (! ((a_char >= 0)
	 && ((a_char <= 0xd7ff)
	     || ((a_char >= 0xe000) && (a_char <= 0x10ffff)))))
    s48_assertion_violation("s48_enter_char", "not a scalar value", 1, s48_enter_fixnum(a_char));

  return S48_UNSAFE_ENTER_CHAR(a_char);
}

long
s48_extract_char(s48_value a_char)
{
  if (! S48_CHAR_P(a_char))
    s48_assertion_violation("s48_extract_char", "not a char", 1, a_char);
  
  return S48_UNSAFE_EXTRACT_CHAR(a_char);
}

/********************************/
/* Allocation */

s48_value
s48_enter_pointer(void *pointer)
{
  s48_value obj;

  obj = s48_allocate_stob(S48_STOBTYPE_BYTE_VECTOR, sizeof(void *));
  *(S48_ADDRESS_AFTER_HEADER(obj, void *)) = pointer;

  return obj;
}

s48_value
s48_cons(s48_value v1, s48_value v2)
{
  s48_value obj;
  S48_DECLARE_GC_PROTECT(2);

  S48_GC_PROTECT_2(v1, v2);

  obj = s48_allocate_stob(S48_STOBTYPE_PAIR, 2);
  S48_UNSAFE_SET_CAR(obj, v1);
  S48_UNSAFE_SET_CDR(obj, v2);

  S48_GC_UNPROTECT();
  return obj;
}

s48_value
s48_make_weak_pointer(s48_value value)
{
  s48_value obj;
  S48_DECLARE_GC_PROTECT(1);

  S48_GC_PROTECT_1(value);

  obj = s48_allocate_stob(S48_STOBTYPE_WEAK_POINTER, 1);
  S48_STOB_SET(obj, 0, value);

  S48_GC_UNPROTECT();
  return obj;
}

/*
 * Entering and extracting byte vectors.
 */

s48_value
s48_enter_byte_vector(char *bytes, long length)
{
  s48_value obj = s48_allocate_stob(S48_STOBTYPE_BYTE_VECTOR, length);
  memcpy(S48_UNSAFE_EXTRACT_BYTE_VECTOR(obj), bytes, length);
  return obj;
}

char *
s48_extract_byte_vector(s48_value byte_vector)
{
  S48_CHECK_VALUE(byte_vector);

  return S48_UNSAFE_EXTRACT_BYTE_VECTOR(byte_vector);
}

/*
 * Making various kinds of stored objects.
 */

s48_value
s48_make_string(int length, long init)
{
  int i;
  s48_value obj = s48_allocate_string(length);
  /* We should probably offer a VM function for this. */
  for (i = 0; i < length; ++i)
    s48_string_set(obj, i, init);
  return obj;
}

s48_value
s48_make_vector(int length, s48_value init)
{
  int i;
  s48_value obj;
  S48_DECLARE_GC_PROTECT(1);

  S48_GC_PROTECT_1(init);

  obj = s48_allocate_stob(S48_STOBTYPE_VECTOR, length);
  for (i = 0; i < length; ++i)
    S48_UNSAFE_VECTOR_SET(obj, i, init);

  S48_GC_UNPROTECT();

  return obj;
}

s48_value
s48_make_byte_vector(int length)
{
    return s48_allocate_stob(S48_STOBTYPE_BYTE_VECTOR, length);
}

s48_value
s48_enter_byte_substring(char *str, long length)
{
  s48_value obj = s48_allocate_stob(S48_STOBTYPE_BYTE_VECTOR, length + 1);
  memcpy(S48_UNSAFE_EXTRACT_BYTE_VECTOR(obj), str, length);
  *(S48_UNSAFE_EXTRACT_BYTE_VECTOR(obj) + length) = '\0';
  return obj;
}

s48_value
s48_enter_byte_string(char *str)
{
  return s48_enter_byte_substring(str, strlen(str));
}

s48_value
s48_make_record(s48_value type_shared_binding)
{
    int i, number_of_fields;
    s48_value record = S48_FALSE;
    s48_value record_type = S48_FALSE;
    S48_DECLARE_GC_PROTECT(1);

    S48_GC_PROTECT_1(record_type);

    S48_SHARED_BINDING_CHECK(type_shared_binding);
    S48_SHARED_BINDING_CHECK(the_record_type_binding);

    record_type = S48_SHARED_BINDING_REF(type_shared_binding);

    s48_check_record_type(record_type, the_record_type_binding);

    number_of_fields =
      S48_UNSAFE_EXTRACT_FIXNUM(S48_RECORD_TYPE_NUMBER_OF_FIELDS(record_type));

    record = s48_allocate_stob(S48_STOBTYPE_RECORD, number_of_fields + 1);

    S48_UNSAFE_RECORD_SET(record, -1, record_type);
    for (i = 0; i < number_of_fields; ++i)
      S48_UNSAFE_RECORD_SET(record, i, S48_UNSPECIFIC);

    S48_GC_UNPROTECT();

    return record;
}

/*
 * Raise an exception if `record' is not a record whose type is the one
 * found in `type_binding'.
 */
void
s48_check_record_type(s48_value record, s48_value type_binding)
{
  if (! S48_RECORD_P(S48_SHARED_BINDING_REF(type_binding)))
    s48_raise_scheme_exception(S48_EXCEPTION_UNBOUND_EXTERNAL_NAME, 1,
			       S48_SHARED_BINDING_NAME(type_binding));

  if ((! S48_RECORD_P(record)) ||
      (S48_UNSAFE_SHARED_BINDING_REF(type_binding) !=
       S48_UNSAFE_RECORD_REF(record, -1)))
    s48_assertion_violation("s48_check_record_type", "not a record of the appropriate type", 2,
			    record, S48_SHARED_BINDING_REF(type_binding));
}

long
s48_length(s48_value list)
{
  long i = 0;

  while (!(S48_EQ(list, S48_NULL)))
    {
      list = S48_CDR(list);
      ++i;
    }
  return S48_UNSAFE_ENTER_FIXNUM(i);
}
