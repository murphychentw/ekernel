// File : MemoryManager.h

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
 1998/11/25: Created by Murphy Cheng-Che Chen
			Use the C codes of the link-based memory management module from Utah.
*/

#ifndef __EKERNEL_MEMORYMANAGER_H_
#define __EKERNEL_MEMORYMANAGER_H_

#include "Types.h"

#include "libc\assert\assert.h"

struct lmm_node
{
	struct lmm_node *next;
	vm_size_t size;
};

#define ALIGN_SIZE	sizeof(struct lmm_node)
#define ALIGN_MASK	(ALIGN_SIZE - 1)

typedef natural_t lmm_flags_t;
typedef integer_t lmm_pri_t;

typedef struct lmm_region
{
	struct lmm_region *next;

	// List of free memory blocks in this region.
	struct lmm_node *nodes;

	// Virtual addresses of the start and end of the memory region.
	vm_offset_t min;
	vm_offset_t max;

	// Attributes of this memory.
	lmm_flags_t flags;

	// Allocation priority of this region with respect to other regions.
	lmm_pri_t pri;

	// Current amount of free space in this region in BYTEs.
	vm_size_t free;
} lmm_region_t;

#define PAGE_SIZE	4096
#define PAGE_SHIFT	12

class MemoryManager
{
	protected:
		lmm_region *m_regions;
	public:
		MemoryManager();
		~MemoryManager();

		void AddFree(void *block, vm_size_t size);
		void AddRegion(lmm_region_t *reg, void *addr, vm_size_t size,
		    lmm_flags_t flags, lmm_pri_t pri);
		void *Alloc(vm_size_t size, lmm_flags_t flags);
		void *AllocAligned(vm_size_t size, lmm_flags_t flags,
			int align_bits, vm_offset_t align_ofs);
		void *AllocGen(vm_size_t size, unsigned flags,
		    int align_bits, vm_offset_t align_ofs,
		    vm_offset_t in_min, vm_size_t in_size);
		void *AllocPage(lmm_flags_t flags);
		vm_size_t Avail(lmm_flags_t flags);
		void Dump();
		void FindFree(vm_offset_t *inout_addr,
			vm_size_t *out_size, lmm_flags_t *out_flags);
		void Free(void *block, vm_size_t size);
		void FreePage(void *page);
		void Init();
		void RemoveFree(void *block, vm_size_t block_size);
};


#endif
