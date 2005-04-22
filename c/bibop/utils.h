/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#ifndef __S48_BIBOP_UTILS_H
#define __S48_BIBOP_UTILS_H

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#ifndef _WIN32
#include "sysdep.h"
#endif

/* inline isn't in ANSI C */
#ifdef _WIN32
#define inline _inline
#elif !defined(HAVE_INLINE)
#define inline
#endif

extern void s48_gc_error(const char* message, ...);

#if (DISPLAY_MEASURE_GC)
extern void newline();
extern void space();

extern void display_string(char* message);
extern void dis_string(char* message);

extern void display_number(int digits, long n);
extern void dis_number(int digits, long n);

extern void display_double(int digits, double n);
extern void dis_double(int digits, double n);

extern void write_vm_options(FILE* f);
extern void display_vm_options();

extern void display_string_x(char* message, int times);
extern void display_comparison(long new, long old);

extern void write_line(FILE* f, long num);
#endif

#if (BIBOP_LOG)
extern void s48_bibop_log(const char* message, ...);
#endif

inline int int_min(int a, int b);
inline int int_max(int a, int b);

#ifndef FALSE
#define FALSE (0 == 1)
#endif

#ifndef TRUE
#define TRUE (0 == 0)
#endif

#endif
