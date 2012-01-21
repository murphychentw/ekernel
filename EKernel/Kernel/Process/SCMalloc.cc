// File: SCMalloc.cc

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

/*
 1999/04/08: Created by Murphy Cheng-Che Chen
*/


#include "Process.h"
#include "Initialize\Globals.h"



DWORD SCMalloc(void *pArg[])
{
	DWORD nSize = (DWORD)pArg[0];
	vm_size_t** pnAddr = (vm_size_t**)pArg[1];
	
	nSize += sizeof(vm_size_t);
	*pnAddr = (vm_size_t*)(g_arrayProcess->GetAt(g_nCurrentProcessID))->
		MemoryAllocatorAlloc(nSize);
	if(*pnAddr==0)
		return 0L;
	**pnAddr = nSize;
	*pnAddr += 1;

	return 0L;
}
