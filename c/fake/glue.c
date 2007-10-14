/*
 * Phony versions of native-code helper routines for use on architectures for
 * which we have no native code.
 */

long
s48_call_native_procedure(long procedure, long arg_count)
{
  return 3;	/* indicates failure; see scheme/vm/interp/call.scm */
}

long
s48_invoke_native_continuation(long code_pointer, long protocol_skip)
{
  return 3;	/* indicates failure; see scheme/vm/interp/call.scm */
}

long
s48_jump_to_native_address(long address, long template)
{
  return 3;    /* indicates failure; see scheme/vm/interp/call.scm */
}

void
s48_init_asm_glue(void)
{
}

