/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

void
s48_initialize_external_modules()
{
  extern void s48_init_socket(void);
  extern void s48_init_dynlink(void);
  extern void s48_initialize_external(void);
  extern void s48_init_external_libs(void);

  s48_init_socket();
  s48_init_dynlink();
  s48_initialize_external();
  s48_init_external_libs();
}
