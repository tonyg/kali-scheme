#include <stdio.h>
#include "prescheme.h"

struct stream {
  FILE * port;
  long buffer;
  long size;
  long loc;
  long limit;
};

unsigned char stream_read_char(struct stream *stream_0X)
{
  long loc_1X;
  long buffer_2X;
  long count_3X;
  unsigned char ch_4X;

 {loc_1X = stream_0X->loc;
  if ((loc_1X < (stream_0X->limit))) {
    ch_4X = *((unsigned char *) loc_1X);
    stream_0X->loc = (1 + (stream_0X->loc));
    return(ch_4X);}
  else {
    buffer_2X = stream_0X->buffer;
    count_3X = fread((void *)buffer_2X, sizeof(char), (stream_0X->size), (stream_0X->port));
    if ((0 == count_3X)) {
      return(0);}
    else {
      stream_0X->loc = (1 + buffer_2X);
      stream_0X->limit = (buffer_2X + count_3X);
      return((*((unsigned char *) buffer_2X)));}}}}

long port_Gstream(FILE * port_5X)
{
  struct stream *stream_6X;
  long buffer_7X;

 {stream_6X = (struct stream*)malloc(sizeof(struct stream));
  buffer_7X = (long)malloc(1024);
  stream_6X->port = port_5X;
  stream_6X->buffer = buffer_7X;
  stream_6X->size = 1024;
  stream_6X->loc = buffer_7X;
  stream_6X->limit = buffer_7X;
  return(buffer_7X);}}

