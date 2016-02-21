#include "Utils.h"

#define MAX_ULONG (unsigned long)0xFFFFFFFF

unsigned long calculateInterval (unsigned long begin, unsigned long end) {
  if (end >= begin) {return end - begin;}
  return (MAX_ULONG - begin) + end + 1;
}

