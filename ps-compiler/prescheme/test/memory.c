#include <stdio.h>
#include "prescheme.h"


unsigned char test(unsigned char x_0X, long i_1X)
{
  long m_2X;

 {m_2X = (long)malloc(10);
  *((unsigned char *) (m_2X + i_1X)) = x_0X;
  *((long *) (m_2X + i_1X)) = (1 + (*((long *) (m_2X + i_1X))));
  return((*((unsigned char *) (m_2X + i_1X))));}}

