// File: libgcc2.cc

/*
 * Copyright (c) 1998-1999 Murphy Cheng-Che Chen <murphychen@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#include "Types.h"
#include "libc/malloc/malloc.h"
#include "libc/stdio/stdio.h"

typedef void (*vfp)(void);
vfp __new_handler = (vfp) 0;
void __default_new_handler (void);

void *__builtin_new (SIZE sz)
{
	void *p;
	vfp handler = (__new_handler) ? __new_handler : __default_new_handler;

//	printf("Need %x BYTEs of memory!\n", sz);

	// malloc (0) is unpredictable; avoid it.
	if (sz == 0)
		sz = 1;
	p = (void *) malloc (sz);
	while (p == 0)
	{
		(*handler) ();
		p = (void *) malloc (sz);
	}

//	printf("Allocated %x BYTEs of memory at %x!\n", sz, p);
  
	return p;
}


void *__builtin_vec_new (SIZE sz)
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
//	printf("Freed memory at %x!\n", ptr);

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
