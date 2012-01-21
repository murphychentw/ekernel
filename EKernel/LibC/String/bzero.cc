#include "libc\string\string.h"

#undef bzero

void *bzero(void *a, SIZE b)
{
  return memset(a,0,b);
}
