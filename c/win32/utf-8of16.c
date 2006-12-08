/* Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

/*
 * This file defines functions for dealing with a synthetic text
 * encoding called UTF-8.  It's like UTF-8, but also encodes unpaired
 * surrogates directly, which is what we need for the Windows API.
 */

#include <windows.h>

static char masks[4] = { 0x7f, 0x1f, 0x0f, 0x07 };
 
/*
 * - NUL-terminates
 * - if utf_8of16 is NULL, we just compute the size
 * - returns size (sans NUL) needed for UTF-8of16
 */

int
s48_utf_16_to_utf_8of16(LPWSTR utf_16, 
			unsigned char* utf_8of16)
{
  int p = 0, i = 0;
  while (utf_16[i])
    {
      unsigned int c = utf_16[i];
      ++i;
      if ((c >= 0xD800) && (c <= 0xDBFF) /* high surrogate */
	  && utf_16[i]
	  && (utf_16[i] >= 0xDC00) && (utf_16[i] <= 0xDFFF)) /* low surrogate */
	{
	  c = ((c - 0xd7c0) << 10) + (utf_16[i] & 0x3ff);
	  ++i;
	}
					    
      if (c <= 0x7f)
	{
	  if (utf_8of16)
	    utf_8of16[p] = (unsigned char) c;
	  ++p; 
	}
      else if (c <= 0x7ff)
	{
	  if (utf_8of16)
	    {
	      utf_8of16[p] = (unsigned char) ((c >> 6) + 0xc0);
	      utf_8of16[p+1] = (unsigned char) ((c & 0x3f) + 0x80);
	    }
	  p += 2;
	  }
      else if (c <= 0xffff)
	{
	  if (utf_8of16)
	    {
	      utf_8of16[p] = (unsigned char) ((c >> 12) + 0xe0);
	      utf_8of16[p+1] = (unsigned char) (((c >> 6) & 0x3f) + 0x80);
	      utf_8of16[p+2] = (unsigned char) ((c & 0x3f) + 0x80);
	    }
	  p += 3;
	}
      else
	{
	  if (utf_8of16)
	    {
	      utf_8of16[p] = (unsigned char) ((c >> 18) + 0xf0);
	      utf_8of16[p+1] = (unsigned char) (((c >> 12) + 0xe0) + 0x80);
	      utf_8of16[p+2] = (unsigned char) (((c >> 6) & 0x3f) + 0x80);
	      utf_8of16[p+3] = (unsigned char) ((c & 0x3f) + 0x80);
	    }
	  p += 4;
	}
	
    }
  if (utf_8of16)
    utf_8of16[p] = 0;
  return p;
}

/*
 * The table, and the associated decoding algorithm, is from
 * Richard Gillam: "Unicode Demystified", chapter 14
 */

static char states[3][32] =
  { 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 2, 2, 3, -1},
    {-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 0, 0, 0, 0, 0, 0, 0, 0, -2, -2, -2, -2, -2, -2, -2, -2},
    {-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 1, 1, 1, 1, 1, 1, 1, 1, -2, -2, -2, -2, -2, -2, -2, -2},
  };

/*
 * - NUL-terminates
 * - if utf_6 is NULL, we just compute the size
 * - returns size (sans NUL) needed for UTF-16
 */

int
s48_utf_8of16_to_utf_16(const unsigned char* utf_8of16,
			LPWSTR utf_16,
			int* errorp)
{
  int p = 0, q = 0, state = 0, error = 0;
  unsigned int scalar_value = 0;
  unsigned mask = 0;
  
  while (utf_8of16[q])
    {
      unsigned char c = utf_8of16[q];
      ++q;

      state = states[state][c >> 3];
      
      switch (state) {
      case 0:
	scalar_value += c & 0x7f;

	if (scalar_value > 0xffff)
	  {
	    if (utf_16)
	      {
		utf_16[p] = (scalar_value >> 10) + 0xD7C0;
		utf_16[p+1] = (scalar_value & 0x3FF) + 0xDC00;
	      }
	    p += 2;
	  }
	else
	  {
	    if (utf_16)
	      utf_16[p] = scalar_value;
	    ++p;
	  }
	scalar_value = 0;
	mask = 0;
	break;
	
      case 1:
      case 2:
	if (mask == 0)
	  mask = masks[state];
	scalar_value = (scalar_value + (c & mask)) << 6;
	mask = 0x3f;
	break;

      case -2:
	--q;
	/* fall thru */

      case -1:
	if (utf_16)
	  utf_16[p] = 0xfffd;
	++p;
	scalar_value = 0;
	state = 0;
	mask = 0;
	error = 1;
	break;
      }
    }
  if (errorp)
    *errorp = error;
  if (utf_16)
    utf_16[p] = 0;
  return p;
}

/*

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
  unsigned int t1[] = { 'A', 'B', 0xd800, 0xd900, 0xdfff, 'C', 'D', 0 };

  int size_8 = s48_utf_16_to_utf_8of16(t1, NULL);
  printf("size_8 %d\n", size_8);

  unsigned char c[1000];

  size_8 = s48_utf_16_to_utf_8of16(t1, c);

  printf("size_8 %d\n", size_8);

  {
    int i = 0;
    while (i < size_8)
      {
	printf("%d: %4x\n", i, c[i]);
	++i;
      }
  }

  unsigned int u[1000];
  int error;

  int size_16 = s48_utf_8of16_to_utf_16(c, NULL, &error);
  printf("size_16 %d\n", size_16);

  size_16 = s48_utf_8of16_to_utf_16(c, u, &error);
  printf("size_16 %d\n", size_16);

  {
    int i = 0;
    while (i < size_16)
      {
	printf("%d: %4x\n", i, u[i]);
	++i;
      }
  }
  
}
*/

