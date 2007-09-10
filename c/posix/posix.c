/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

extern void s48_init_posix_dir(void);
extern void s48_init_posix_user(void);
extern void s48_init_posix_regexp(void);
extern void s48_init_posix_proc_env(void);
extern void s48_init_posix_io(void);
extern void s48_init_posix_proc(void);

void
s48_on_load(void)
{
  s48_init_posix_dir();
  s48_init_posix_user();
  s48_init_posix_regexp();
  s48_init_posix_proc_env();
  s48_init_posix_io();
  s48_init_posix_proc();
}

extern void s48_uninit_posix_proc(void);

void
s48_on_unload(void)
{
  s48_uninit_posix_proc();
}
