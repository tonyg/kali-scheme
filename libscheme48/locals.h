/* Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */
#ifndef INCLUDE__C__LOCALS_H
#define INCLUDE__C__LOCALS_H

/* Old interface for informing the GC of local Scheme values.
 * Consider this interface deprecated.
 *
 * See new interface below.
 */

#define S48_DECLARE_GC_PROTECT(n) long ___gc_buffer[(n)+2]

#define S48_GC_PROTECT_1(v) \
  (___gc_buffer[2]=(long)&(v), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 1))

#define S48_GC_PROTECT_2(v1, v2) \
  (___gc_buffer[2]=(long)&(v1), ___gc_buffer[3]=(long)&(v2), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 2))

#define S48_GC_PROTECT_3(v1, v2, v3) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 3))

#define S48_GC_PROTECT_4(v1, v2, v3, v4) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 4))

#define S48_GC_PROTECT_5(v1, v2, v3, v4, v5) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 5))

#define S48_GC_PROTECT_6(v1, v2, v3, v4, v5, v6) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 6))

#define S48_GC_PROTECT_7(v1, v2, v3, v4, v5, v6, v7) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   ___gc_buffer[8]=(long)&(v7), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 7))

#define S48_GC_PROTECT_8(v1, v2, v3, v4, v5, v6, v7, v8) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   ___gc_buffer[8]=(long)&(v7), \
   ___gc_buffer[9]=(long)&(v8), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 8))

#define S48_GC_PROTECT_9(v1, v2, v3, v4, v5, v6, v7, v8, v9) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   ___gc_buffer[8]=(long)&(v7), \
   ___gc_buffer[9]=(long)&(v8), \
   ___gc_buffer[10]=(long)&(v9), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 9))

#define S48_GC_PROTECT_10(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
  (___gc_buffer[2]=(long)&(v1), \
   ___gc_buffer[3]=(long)&(v2), \
   ___gc_buffer[4]=(long)&(v3), \
   ___gc_buffer[5]=(long)&(v4), \
   ___gc_buffer[6]=(long)&(v5), \
   ___gc_buffer[7]=(long)&(v6), \
   ___gc_buffer[8]=(long)&(v7), \
   ___gc_buffer[9]=(long)&(v8), \
   ___gc_buffer[10]=(long)&(v9), \
   ___gc_buffer[11]=(long)&(v10), \
   s48_push_gc_rootsB((char *) ___gc_buffer, 10))

#define S48_GC_UNPROTECT()				\
   do { if (! s48_pop_gc_rootsB())			\
       	  s48_raise_scheme_exception( S48_EXCEPTION_GC_PROTECTION_MISMATCH, 0); \
      } while(0)

#define S48_GC_PROTECT_GLOBAL(v) (s48_register_gc_rootB((char *)&(v)))



/* New GC interface
 */

#define S48_FRAME S48_DECLARE_GC_PROTECT (10)

#define S48_PROTECT_FRAME(L)                                                   \
  s48_protect_n (arena,                                                        \
                 &(L).___gc_buffer,                                            \
                 (sizeof (L) - sizeof (L.___gc_buffer)) / sizeof (s48_word_t))

#define S48_UNPROTECT_FRAME(L) S48_GC_UNPROTECT()

#define S48_LSET(DEST, SRC) (*(DEST) = *(SRC))

/* automatically generated __STDC__ prototypes */
extern void s48_protect_n (s48_arena_t arena,
                           void * frame,
                           int n_vars);
#endif  /* INCLUDE__C__LOCALS_H */
