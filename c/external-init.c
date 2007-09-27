/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/* Initialize external modules. */

extern void s48_initialize_external(void);
extern void s48_init_external_libs(void);
extern void s48_init_dynlink(void);
extern void s48_init_sysexits(void);
extern void s48_init_srfi_27(void);
extern void s48_init_sockets(void);
extern void s48_init_net(void);
extern void s48_init_asm_glue(void);

void
s48_initialize_external_modules(void)
{
  s48_initialize_external();
  s48_init_external_libs();
  s48_init_dynlink();
  s48_init_sysexits();
  s48_init_srfi_27();
  s48_init_os_sockets();
  s48_init_net();
  s48_init_asm_glue();
}
