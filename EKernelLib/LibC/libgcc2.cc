// File: libgcc2.cc

/*
 Copyright (c) 1998-1999 Murphy Cheng-Che Chen. All Rights Reserved.

 This software is provided "AS IS," without a warranty of any kind. ALL
 EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MURPHY CHENG-CHE CHEN AND HIS
 LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A
 RESULT OF USING, MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES.
 IN NO EVENT WILL MURPHY CHENG-CHE CHEN OR HIS LICENSORS BE LIABLE FOR ANY
 LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL,
 CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS
 OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE
 SOFTWARE, EVEN IF MURPHY CHENG-CHE CHEN HAS BEEN ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGES.

 murphy@hpc.ee.ntu.edu.tw
 http://hpc.ee.ntu.edu.tw/~murphy/
*/


#include "types.h"
#include "libc/malloc/malloc.h"
#include "libc/stdio/stdio.h"

typedef void (*vfp)(void);
vfp __new_handler = (vfp) 0;
void __default_new_handler (void);

void *__builtin_new (size_t sz)
{
	void *p;
	vfp handler = (__new_handler) ? __new_handler : __default_new_handler;

//	printf("Need %x bytes of memory!\n", sz);

	// malloc (0) is unpredictable; avoid it.
	if (sz == 0)
		sz = 1;
	p = (void *) malloc (sz);
	while (p == 0)
	{
		(*handler) ();
		p = (void *) malloc (sz);
	}

//	printf("Allocated %x bytes of memory at %x!\n", sz, p);
  
	return p;
}


void *__builtin_vec_new (size_t sz)
{
  return __builtin_new (sz);
}

vfp set_new_handler (vfp handler)
{
  vfp prev_handler;

  prev_handler = __new_handler;
  if (handler == 0) handler = __default_new_handler;
  __new_handler = handler;
  return prev_handler;
}


void __default_new_handler ()
{
	printf("panic! memory allocation error!");
	while(1);
}


void __builtin_delete (void *ptr)
{
  printf("Freed memory at %x!\n", ptr);

  if (ptr)
    free (ptr);
}

void __builtin_vec_delete (void *ptr)
{
  __builtin_delete (ptr);
}


void terminate()
{
	printf("terminate!");
	while(1);
}

void *__eh_pc;

extern "C" {
void __throw()
{
	printf("panic!\n");
	printf("C++ exception handling mechanism is unsupported!\n");
	while(1);
}
}
