// File: PageDirectory.h

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
	Figure 13-7: Page Directory Entry Format
	void Allocate();
	void Deallocate();
	void* GetBase();
	void MapPage(vm_offset_t linear_address, vm_offset_t physical_address, DWORD mapping_bits);
	void MapRange(vm_offset_t linear_address, vm_offset_t physical_address,
		vm_size_t size, DWORD mapping_bits);
1998/12/10: Murphy
	PageDirectoryEntry& PageDirectoryEntry::operator=(DWORD value);
*/

#ifndef __EKERNEL_HARDWARE_PENTIUM_PAGEDIRECTORY_H_
#define __EKERNEL_HARDWARE_PENTIUM_PAGEDIRECTORY_H_

#include "Types.h"
#include "libc\string\string.h"
#include "PageTable.h"

#define PDE_PRESENT		0x00000001
#define PDE_WRITE		0x00000002
#define PDE_USER		0x00000004
#define PDE_WRITETHRU		0x00000008
#define PDE_CACHEDISABLE 	0x00000010
#define PDE_ACCESSED		0x00000020
#define PDE_MOD			0x00000040	// only for superpages
#define PDE_SUPERPAGE		0x00000080
#define PDE_GLOBAL		0x00000100	// only for superpages
#define PDE_AVAIL		0x00000e00
#define PDE_BASEADDRESS		0xfffff000

#define PDESHIFT		22		// page directory shift
#define PDEMASK			0x3ff		// mask for page directory table index

// Convert linear offset to page directory table index
#define LinearToPageDirectoryTableIndex(a)	(((a) >> PDESHIFT) & PDEMASK)

// Superpage-related macros.
#define SUPERPAGE_SHIFT		PDESHIFT
#define SUPERPAGE_SIZE		(1 << SUPERPAGE_SHIFT)
#define SUPERPAGE_MASK		(SUPERPAGE_SIZE - 1)

#define round_superpage(x)	((vm_offset_t)((((vm_offset_t)(x))	\
				+ SUPERPAGE_MASK) & ~SUPERPAGE_MASK))
#define trunc_superpage(x)	((vm_offset_t)(((vm_offset_t)(x))	\
				& ~SUPERPAGE_MASK))

#define	superpage_aligned(x)	((((vm_offset_t)(x)) & SUPERPAGE_MASK) == 0)



struct PageDirectoryEntry {
	DWORD	Present:1,		// Page Present bit
		Write:1,		// Read/Write bit
		User:1,			// User/Supervisor bit
		WriteThrough:1,		// Page Write-Through
		CacheDisable:1,		// Page Cache Disable
		Accessed:1,		// Access bit
		:1,
		SuperPage:1,		// Page Size (0=4KB, 1=4MB)
		:1,
		Avail:3,
		PageTableBaseAddress:20;

	PageDirectoryEntry& operator=(DWORD value) {
		memcpy(this, &value, sizeof(DWORD));
		return *this;
	}

};


class PageDirectory {

private:
	PageDirectoryEntry *m_pdeTable;
	PageTable *m_ptTables;

public:
	void Allocate();
	void Deallocate();

	inline void* GetBase() const { assert(m_pdeTable); return (void*)m_pdeTable; };

	void MapPage(vm_offset_t linear_address, vm_offset_t physical_address, DWORD mapping_bits);
	void MapRange(vm_offset_t linear_address, vm_offset_t physical_address,
		vm_size_t size, DWORD mapping_bits);
};

#endif
