/* Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

#include <stdlib.h>
#include <string.h>

extern int s48_dragon(char *buf, double v);
extern void s48_free_init(void);

size_t
s48_double_to_string(char *dest, double v)
{
  char raw[33];
  char *buf = dest;
  char *digits = raw;
  int exponent = s48_dragon(raw, v);
  
  int digit_count = strlen(raw);

  if (*digits == '#') /* infinity or NaN */
    {
      strcpy(dest, digits);
      return digit_count;
    }

  if (*digits == '-')
    {
      *buf++ = '-';
      ++digits;
      --digit_count;
    }

  if ((exponent <= -4) || (exponent > (digit_count + 7)))
    /* print with explicit exponent */
    {
      int decimal_point = exponent;
      *buf++ = *digits++;
      if (*digits)
	{
	  *buf++ = '.';
	  while (*digits)
	    *buf++ = *digits++;
	}

      /* exponent */
      *buf++ = 'e';
      
      if (decimal_point < 0)
	{
	  *buf++ = '-';
	  decimal_point = -decimal_point;
	}
      
      {
	int power_of_10 = 1;
	
	while (decimal_point >= (10*power_of_10))
	  power_of_10 *= 10;
	while (power_of_10 > 0)
	  {
	    int digit = decimal_point / power_of_10;
	    *buf++ = digit + '0';
	    decimal_point -= digit * power_of_10;
	    power_of_10 /= 10;
	  }
      }
      *buf = '\0';
    }
  else if (exponent < 0)
    /* 0.<something> */
    {
      *buf++ = '0';
      *buf++ = '.';
      ++exponent;
      while (exponent < 0)
	{
	  *buf++ = '0';
	  ++exponent;
	}
      while (*digits)
	*buf++ = *digits++;
      *buf = '\0';
    }
  else
    /* <something>.<something> */
    {
      while (*digits)
	{
	  *buf++ = *digits++;
	  if (exponent == 0)
	    *buf++ = '.';
	  --exponent;
	}
      if (exponent >= 0)
	{
	  while (exponent >= 0)
	    {
	      *buf++ = '0';
	      --exponent;
	    }
	  *buf++ = '.';
	  *buf++ = '0';
	}
      else
	{
	  if (exponent == -1)
	    *buf++ = '0';
	}
	  
      *buf = '\0';
    }
  return buf - dest;
}
