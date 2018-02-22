#ifndef BITTWIDDLING_H
#define BITTWIDDLING_H

#include <asm/gbz80/types.h>

/*
* Set a range of memory [start, end) to a set value.
*/
inline void wipe_memory(UINT8* start, UINT8* end, UINT8 value)
{
  UINT8* i;
  for (i = start; i < end; ++i) *i = value;
}

#endif