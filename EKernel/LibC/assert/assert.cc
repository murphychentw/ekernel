#include "assert.h"
#include "libc\stdio\stdio.h"

void _Assert(char *strFile, unsigned uLine)
{
	printf("\nAssertion failed: %s, line %u\n", strFile, uLine);
	while(1);
}