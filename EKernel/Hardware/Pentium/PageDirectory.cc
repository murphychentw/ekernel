// File: PageDirectory.cc

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

#include "PageDirectory.h"
#include "MemoryManager\MemoryManager.h"
#include "Initialize\Globals.h"
#include "LibC\assert\assert.h"
#include "LibC\stdio\stdio.h"
#include "LibC\string\string.h"


void PageDirectory::Allocate()
{
	m_pdeTable=(PageDirectoryEntry*)g_MemoryManager.AllocPage(0);

#ifdef DEBUG
//	printf("Allocate Page Directory Base Address = %x\n",
//		m_pdeTable);
#endif

	memset(m_pdeTable, 0, 1024*4);
	m_ptTables=new PageTable[1024];
}



void PageDirectory::Deallocate()
{
	g_MemoryManager.FreePage(m_pdeTable);
	delete [] m_ptTables;
}



void PageDirectory::MapPage(vm_offset_t linear_address, vm_offset_t physical_address, DWORD mapping_bits)
{
	assert(mapping_bits & PDE_PRESENT);
	assert(!(mapping_bits & PDE_BASEADDRESS));

	WORD pdtIndex=LinearToPageDirectoryTableIndex(linear_address);

	// Find the page table, creating one if necessary.
	if (!(m_pdeTable[pdtIndex].Present))
	{
		m_ptTables[pdtIndex].Allocate();

		// Set the pde to point to it.
		m_pdeTable[pdtIndex] = 
			(DWORD)m_ptTables[pdtIndex].GetBase()
			| PDE_PRESENT | PDE_USER | PDE_WRITE;
	}

	m_ptTables[pdtIndex].MapPage(linear_address, physical_address, mapping_bits);
}



void PageDirectory::MapRange(vm_offset_t linear_address, vm_offset_t physical_address,
	vm_size_t size, DWORD mapping_bits)
{
	assert(mapping_bits & PDE_PRESENT);
	assert(!(mapping_bits & PDE_BASEADDRESS));

	// Make size round to mutiples of 4K.
	size=(size+PAGE_SIZE-1)&~(PAGE_SIZE-1);

//	printf("PageDirectory::MapRange %08x -> %08x, size=%08x\n", linear_address, physical_address, size);

	while (size > 0)
	{
		WORD pdtIndex=LinearToPageDirectoryTableIndex(linear_address);

#if 0
		// Use a 4MB page if we can.
		if (superpage_aligned(linear_address)
			&& superpage_aligned(physical_address)
			&& (size >= SUPERPAGE_SIZE)
			&& ( g_Processor.CPUFeatureVerify(CPUF_4MB_PAGES)) )
		{
//			printf("Size left for mapping : %x\n", size);
			assert(m_pdeTable[pdtIndex].Present==0);
			/* XXX what if an empty page table exists
			   from previous finer-granularity mappings? */
			m_pdeTable[pdtIndex] = physical_address | mapping_bits | PDE_SUPERPAGE;
			linear_address += SUPERPAGE_SIZE;
			physical_address += SUPERPAGE_SIZE;
			size -= SUPERPAGE_SIZE;
		}
		else
#endif
		{
			PageTableEntry *pte;

			// Find the page table, creating one if necessary.
			if (!(m_pdeTable[pdtIndex].Present))
			{
				m_ptTables[pdtIndex].Allocate();

				// Set the pde to point to it.
				m_pdeTable[pdtIndex] = (DWORD)m_ptTables[pdtIndex].GetBase()
					| PDE_PRESENT | PDE_USER | PDE_WRITE;
			}

//			printf("Page Directory Entry %x =%x\n", pdtIndex, m_pdeTable[pdtIndex]);
//			printf("Page Directory Entry Present = %d\n", m_pdeTable[pdtIndex].Present);
//			printf("Page Directory Entry Write = %d\n", m_pdeTable[pdtIndex].Write);
//			printf("Page Directory Entry User = %d\n", m_pdeTable[pdtIndex].User);
//			printf("Page Directory Entry WriteThrough = %d\n", m_pdeTable[pdtIndex].WriteThrough);
//			printf("Page Directory Entry CacheDisable = %d\n", m_pdeTable[pdtIndex].CacheDisable);
//			printf("Page Directory Entry Accessed = %d\n", m_pdeTable[pdtIndex].Accessed);
//			printf("Page Directory Entry SuperPage = %d\n", m_pdeTable[pdtIndex].SuperPage);
//			printf("Page Directory Entry Avail = %d\n", m_pdeTable[pdtIndex].Avail);
//			printf("Page Directory Entry PageTableBaseAddress = %d\n", m_pdeTable[pdtIndex].PageTableBaseAddress);

			assert(m_pdeTable[pdtIndex].SuperPage==0);

			// Use normal 4KB page mappings.
			do
			{
//				printf("Size left for mapping : %x\n", size);

				m_ptTables[pdtIndex].MapPage(linear_address, physical_address, mapping_bits);

				// Advance to the next page.
				linear_address += PAGE_SIZE;
				physical_address += PAGE_SIZE;
				size -= PAGE_SIZE;
			}
			while ((size > 0) && !superpage_aligned(linear_address));
		}
	}
}
