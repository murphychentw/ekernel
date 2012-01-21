#include "libc\string\string.h"

#undef bcopy

void *bcopy(const void *a, void *b, SIZE len)
{
  return memmove(b, a, len);
}
