// File: PageTable.h

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
	Reference <<Protected Mode Software Architecture>>, Tom Shanley,
	MindShare, Inc.
	Figure 13-11: Page Table Entry Format
	void Allocate();
	void Deallocate();
	inline void* GetBase();
	void MapPage(vm_offset_t linear_address, vm_offset_t physical_address, DWORD mapping_bits);
1998/12/10: Murphy
	PageTableEntry& PageTableEntry::operator=(DWORD value);
*/

#ifndef __EKERNEL_PAGETABLE_H_
#define __EKERNEL_PAGETABLE_H_

#include "types.h"
#include "LibC\assert\assert.h"
#include "Initialize\Globals.h"

#define PTE_PRESENT		0x00000001
#define PTE_WRITE		0x00000002
#define PTE_USER		0x00000004
#define PTE_WRITETHRU		0x00000008
#define PTE_CACHEDISABLE 	0x00000010
#define PTE_ACCESSED		0x00000020
#define PTE_MOD			0x00000040	// only for superpages
#define PTE_GLOBAL		0x00000100	// only for superpages
#define PTE_AVAIL		0x00000e00
#define PTE_BASEADDRESS		0xfffff000

#define PTESHIFT	12	// page table shift
#define PTEMASK		0x3ff	// mask for page table index

// Convert linear offset to page descriptor/page table index
#define LinearToPageTableIndex(a)	(((a) >> PTESHIFT) & PTEMASK)


typedef struct PageTableEntry {
	DWORD	Present:1,
		Write:1,
		User:1,
		WriteThrough:1,
		CacheDisable:1,
		Accessed:1,
		Dirty:1,
		:1,
		:1,
		Avail:3,
		PageBaseAddress:20;

	PageTableEntry& operator=(DWORD value) {
		memcpy(this, &value, sizeof(DWORD));
		return *this;
	}
};



class PageTable {

private:
	PageTableEntry *m_pteTable;

public:
	void Allocate();
	void Deallocate();

	inline void* GetBase() const { assert(m_pteTable); return (void*)m_pteTable; };

	void MapPage(vm_offset_t linear_address, vm_offset_t physical_address, DWORD mapping_bits);
};

#endif
