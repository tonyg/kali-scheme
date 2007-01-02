/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * No write barrier is needed for the two-space collector;
 */

#define	S48_WRITE_BARRIER(stob, address, value)	((void)0)
