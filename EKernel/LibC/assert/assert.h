#ifndef _EKERNEL_ASSERT_H_
#define _EKERNEL_ASSERT_H_

#ifdef NDEBUG

#define assert(ignore) 0

#else

void _Assert(char *strFile, unsigned uLine);

#define assert(expression)	\
	if (expression)		\
		0;		\
	else			\
		_Assert(__FILE__, __LINE__)

#endif

#endif
