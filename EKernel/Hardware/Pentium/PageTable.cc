// File: PageTable.cc

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
1998/12/09: Created by Murphy Cheng-Che Chen
*/

#include "libc\assert\assert.h"
#include "libc\stdio\stdio.h"
#include "libc\string\string.h"
#include "PageTable.h"




void PageTable::Allocate()
{
	m_pteTable=(PageTableEntry*)g_MemoryManager.AllocPage(0);

#ifdef DEBUG
//	printf("Allocate Page Table Base Address = %x\n",
//		m_pteTable);
#endif

	memset(m_pteTable, 0, 1024*4);
}



void PageTable::Deallocate()
{
	g_MemoryManager.FreePage(m_pteTable);
}



void PageTable::MapPage(vm_offset_t linear_address, vm_offset_t physical_address, DWORD mapping_bits)
{
	assert(!(physical_address & ~PTE_BASEADDRESS));
	assert(mapping_bits & PTE_PRESENT);
	assert(!(mapping_bits & PTE_BASEADDRESS));

	WORD ptIndex=LinearToPageTableIndex(linear_address);

	assert(!(m_pteTable[ptIndex].Present));

	m_pteTable[ptIndex]=physical_address|mapping_bits;
}
