// File: malloc.cc

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

#include "malloc.h"
#include "Initialize\Globals.h"
#include "LibC\Stdio\stdio.h"

void *malloc(vm_size_t size)
{
	vm_size_t *chunk;

	size += sizeof(vm_size_t);

//	printf("Needed %d BYTEs, ", size);
//	printf("Avail memory = %d BYTEs\n", g_MemoryManager.Avail(0));

	if((chunk = (vm_size_t*)g_MemoryManager.Alloc((vm_size_t)size, (lmm_flags_t)0))==0)
	{
		return 0;
	}

	*chunk = size;
	return chunk+1;
}

