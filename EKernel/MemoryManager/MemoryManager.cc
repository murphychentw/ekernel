// File : MemoryManager.cc

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

#include "MemoryManager.h"

#include "LibC\stdio\stdio.h"



MemoryManager::MemoryManager()
{
}



MemoryManager::~MemoryManager()
{
}



void MemoryManager::Init()
{
	m_regions = 0;
}



void MemoryManager::AddFree(void *block, vm_size_t size)
{
	struct lmm_region *reg;
	vm_offset_t min = (vm_offset_t)block;
	vm_offset_t max = min + size;

	// Restrict the min and max further to be properly aligned.
	// Note that this is the opposite of what Free() does,
	// because Free() assumes the block was allocated with Alloc()
	// and thus would be a subset of a larger, already-aligned free block.
	// Here we can assume no such thing.
	min = (min + ALIGN_MASK) & ~ALIGN_MASK;
	max &= ~ALIGN_MASK;
	assert(max >= min);

	// If after alignment we have nothing left, we're done.
	if (max == min)
		return;

	// Add the block to the free list(s) of whatever region(s) it overlaps.
	// If some or all of the block doesn't fall into any existing region,
	// then that memory is simply dropped on the floor.
	for (reg = m_regions; reg; reg = reg->next)
	{
		assert(reg->min < reg->max);
		assert((reg->min & ALIGN_MASK) == 0);
		assert((reg->max & ALIGN_MASK) == 0);

		if ((max > reg->min) && (min < reg->max))
		{
			vm_offset_t new_min = min, new_max = max;

			// Only add the part of the block
			// that actually falls within this region.
			if (new_min < reg->min)
				new_min = reg->min;
			if (new_max > reg->max)
				new_max = reg->max;
			assert(new_max > new_min);

			// Add the block.
			Free((void*)new_min, new_max - new_min);
		}
	}
}



void MemoryManager::AddRegion(lmm_region_t *reg, void *addr, vm_size_t size,
		    lmm_flags_t flags, lmm_pri_t pri)
{
	vm_offset_t min = (vm_offset_t)addr;
	vm_offset_t max = min + size;
	struct lmm_region **rp, *r;

	// Align the start and end addresses appropriately.
	min = (min + ALIGN_MASK) & ~ALIGN_MASK;
	max &= ~ALIGN_MASK;

	// If there's not enough memory to do anything with,
	// then just drop the region on the floor.
	// Since we haven't put it on the lmm's list,
	// we'll never see it again.
	if (max <= min)
		return;

	// Initialize the new region header.
	reg->nodes = 0;
	reg->min = min;
	reg->max = max;
	reg->flags = flags;
	reg->pri = pri;
	reg->free = 0;

	// Add the region to the lmm's region list in descending priority order.
	// For regions with the same priority, sort from largest to smallest
	// to reduce the average amount of list traversing we need to do.
	for (rp = &m_regions;
	     ((r = *rp)!=0) && ((r->pri > pri) ||
	     		   ((r->pri == pri) &&
			    (r->max - r->min > reg->max - reg->min)));
	     rp = &r->next)
	{
		assert(r != reg);
		assert((reg->max < r->min) || (reg->min > r->max));
	}
	reg->next = r;
	*rp = reg;
}


void *MemoryManager::Alloc(vm_size_t size, lmm_flags_t flags)
{
	struct lmm_region *reg;

	assert(size > 0);

	size = (size + ALIGN_MASK) & ~ALIGN_MASK;

	assert((size & ALIGN_MASK) == 0);

	for (reg = m_regions; reg; reg = reg->next)
	{
		struct lmm_node **nodep, *node;

		assert((reg->nodes == 0 && reg->free == 0)
		       || ((vm_offset_t)reg->nodes >= reg->min));
		//assert(reg->free >= 0);
		assert(reg->free <= reg->max - reg->min);

		if (flags & ~reg->flags)
			continue;

		for (nodep = &reg->nodes;
		     (node = *nodep) != 0;
		     nodep = &node->next)
		{
			assert(((vm_offset_t)node & ALIGN_MASK) == 0);
			assert(((vm_offset_t)node->size & ALIGN_MASK) == 0);
			assert((node->next == 0) || (node->next > node));
			assert((vm_offset_t)node < reg->max);

			if (node->size >= size)
			{
				if (node->size > size)
				{
					struct lmm_node *newnode;

					/* Split the node and return its head */
					newnode = (struct lmm_node*)
							((char*)node + size);
					newnode->next = node->next;
					newnode->size = node->size - size;
					*nodep = newnode;
				}
				else
				{
					/* Remove and return the entire node. */
					*nodep = node->next;
				}

				/* Adjust the region's free memory counter.  */
				assert(reg->free >= size);
				reg->free -= size;

				return (void*)node;
			}
		}
	}

	return 0;
}



void *MemoryManager::AllocAligned(vm_size_t size, lmm_flags_t flags,
			int align_bits, vm_offset_t align_ofs)
{
	return AllocGen(size, flags,
			     align_bits, align_ofs,
			     (vm_offset_t)0, (vm_size_t)-1);
}



void *MemoryManager::AllocGen(vm_size_t size, unsigned flags,
		    int align_bits, vm_offset_t align_ofs,
		    vm_offset_t in_min, vm_size_t in_size)
{
	vm_offset_t in_max = in_min + in_size;
	struct lmm_region *reg;

#if DEBUG
//	printf("MemoryManager::AllocGen %08x\n", size);
	Dump();
#endif

	assert(size > 0);

	for (reg = m_regions; reg; reg = reg->next)
	{
		struct lmm_node **nodep, *node;

		assert((reg->nodes == 0 && reg->free == 0)
		       || ((vm_offset_t)reg->nodes >= reg->min));
		//assert(reg->free >= 0);
		assert(reg->free <= reg->max - reg->min);

		// First trivially reject the entire region if possible.
		if ((flags & ~reg->flags) 
		    || (reg->min >= in_max)
		    || (reg->max <= in_min))
			continue;

		for (nodep = &reg->nodes;
		     (node = *nodep) != 0;
		     nodep = &node->next)
		{
			vm_offset_t addr;
			struct lmm_node *anode;
			int i;

			assert(((vm_offset_t)node & ALIGN_MASK) == 0);
			assert(((vm_offset_t)node->size & ALIGN_MASK) == 0);
			assert((node->next == 0) || (node->next > node));
			assert((vm_offset_t)node < reg->max);

			// Now make a first-cut trivial elimination check
			// to skip chunks that are _definitely_ too small.
			if (node->size < size)
				continue;

			// Now compute the address at which
			// the allocated chunk would have to start.
			addr = (vm_offset_t)node;
			if (addr < in_min)
				addr = in_min;
			for (i = 0; i < align_bits; i++)
			{
				vm_offset_t bit = (vm_offset_t)1 << i;
				if ((addr ^ align_ofs) & bit)
					addr += bit;
			}

			// See if the block at the adjusted address
			// is still entirely within the node.
			if ((addr - (vm_offset_t)node + size) > node->size)
				continue;

			// If the block extends past the range constraint,
			// then all of the rest of the nodes in this region
			// will extend past it too, so stop here.
			if (addr + size > in_max)
				break;

			// OK, we can allocate the block from this node.

			// If the allocation leaves at least ALIGN_SIZE
			// space before it, then split the node.
			anode = (struct lmm_node*)(addr & ~ALIGN_MASK);
			assert(anode >= node);
			if (anode > node)
			{
				vm_size_t split_size = (vm_offset_t)anode
							- (vm_offset_t)node;
				assert((split_size & ALIGN_MASK) == 0);
				anode->next = node->next;
				anode->size = node->size - split_size;
				node->size = split_size;
				nodep = &node->next;
			}

			// Now use the first part of the anode
			// to satisfy the allocation,
			// splitting off the tail end if necessary.
			size = ((addr & ALIGN_MASK) + size + ALIGN_MASK)
				& ~ALIGN_MASK;
			if (anode->size > size)
			{
				struct lmm_node *newnode;

				// Split the node and return its head.
				newnode = (struct lmm_node*)
						((char*)anode + size);
				newnode->next = anode->next;
				newnode->size = anode->size - size;
				*nodep = newnode;
			}
			else
			{
				// Remove and return the entire node.
				*nodep = anode->next;
			}

			// Adjust the region's free memory counter.
			assert(reg->free >= size);
			reg->free -= size;

#if DEBUG
//			printf("MemoryManager::AllocGen returning %08x\n", addr);
			Dump();
#endif

			return (void*)addr;
		}
	}

#if DEBUG
	printf("MemoryManager::AllocGen failed\n");
#endif

	return 0;
}



void *MemoryManager::AllocPage(lmm_flags_t flags)
{
	return AllocGen(PAGE_SIZE, flags, PAGE_SHIFT, 0,
			     (vm_offset_t)0, (vm_size_t)-1);
}



vm_size_t MemoryManager::Avail(lmm_flags_t flags)
{
	struct lmm_region *reg;
	vm_size_t count;

	count = 0;
	for (reg = m_regions; reg; reg = reg->next)
	{
//		assert((vm_offset_t)reg->nodes >= reg->min);
//		assert((vm_offset_t)reg->nodes < reg->max);
//		assert(reg->free >= 0);
		assert(reg->free <= reg->max - reg->min);

		// Don't count inapplicable regions.
		if (flags & ~reg->flags)
			continue;

		count += reg->free;
	}
	return count;
}



void MemoryManager::Dump()
{
#if 0 
	struct lmm_region *reg;

	printf("MemoryManager::Dump\n");

	for (reg = m_regions; reg; reg = reg->next)
	{
		struct lmm_node *node;
		vm_size_t free_check;

		printf(" region %08x-%08x size=%08x flags=%08x pri=%d free=%08x\n",
			reg->min, reg->max, reg->max - reg->min,
			reg->flags, reg->pri, reg->free);

		assert((reg->nodes == 0)
		       || (vm_offset_t)reg->nodes >= reg->min);
		assert((reg->nodes ==0) || (vm_offset_t)reg->nodes < reg->max);
		assert(reg->free >= 0);
		assert(reg->free <= reg->max - reg->min);

		free_check = 0;
		for (node = reg->nodes; node; node = node->next)
		{
			printf("  node %p-%08x size=%08x next=%p\n", 
				node, (vm_offset_t)node + node->size, node->size, node->next);

			assert(((vm_offset_t)node & ALIGN_MASK) == 0);
			assert((node->size & ALIGN_MASK) == 0);
			assert(node->size >= sizeof(*node));
			assert((node->next == 0) || (node->next > node));
			assert((vm_offset_t)node < reg->max);

			free_check += node->size;
		}

		printf(" free_check=%08x\n", free_check);
		assert(reg->free == free_check);
	}

	printf("MemoryManager::Dump done\n");
#endif
}



void MemoryManager::FindFree(vm_offset_t *inout_addr,
		   vm_size_t *out_size, lmm_flags_t *out_flags)
{
	struct lmm_region *reg;
	vm_offset_t start_addr = (*inout_addr + ALIGN_MASK) & ~ALIGN_MASK;
	vm_offset_t lowest_addr = (vm_offset_t)-1;
	vm_size_t lowest_size = 0;
	unsigned lowest_flags = 0;

	for (reg = m_regions; reg; reg = reg->next)
	{
		struct lmm_node *node;

		if ((reg->nodes == 0)
		    || (reg->max <= start_addr)
		    || (reg->min > lowest_addr))
			continue;

		for (node = reg->nodes; node; node = node->next)
		{
			assert((vm_offset_t)node >= reg->min);
			assert((vm_offset_t)node < reg->max);

			if ((vm_offset_t)node >= lowest_addr)
				break;
			if ((vm_offset_t)node + node->size > start_addr)
			{
				if ((vm_offset_t)node > start_addr)
				{
					lowest_addr = (vm_offset_t)node;
					lowest_size = node->size;
				}
				else
				{
					lowest_addr = start_addr;
					lowest_size = node->size
						- (lowest_addr - (vm_offset_t)node);
				}
				lowest_flags = reg->flags;
				break;
			}
		}
	}

	*inout_addr = lowest_addr;
	*out_size = lowest_size;
	*out_flags = lowest_flags;
}



void MemoryManager::Free(void *block, vm_size_t size)
{
	struct lmm_region *reg;
	struct lmm_node *node = (struct lmm_node*)
				((vm_offset_t)block & ~ALIGN_MASK);
	struct lmm_node *prevnode, *nextnode;

	assert(block != 0);
	assert(size > 0);

	size = (((vm_offset_t)block & ALIGN_MASK) + size + ALIGN_MASK)
		& ~ALIGN_MASK;

	// First find the region to add this block to.
	for (reg = m_regions; ; reg = reg->next)
	{
		assert(reg != 0);
		assert((reg->nodes == 0 && reg->free == 0)
		       || ((vm_offset_t)reg->nodes >= reg->min));

		if (((vm_offset_t)node >= reg->min)
		    && ((vm_offset_t)node < reg->max))
			break;
	}

	// Record the newly freed space in the region's free space counter.
	reg->free += size;
	assert(reg->free <= reg->max - reg->min);

	// Now find the location in that region's free list
	// at which to add the node.
	for (prevnode = 0, nextnode = reg->nodes;
	     (nextnode != 0) && (nextnode < node);
	     prevnode = nextnode, nextnode = nextnode->next);

	// Coalesce the new free chunk into the previous chunk if possible.
	if ((prevnode) &&
	    ((vm_offset_t)prevnode + prevnode->size >= (vm_offset_t)node))
	{
		assert((vm_offset_t)prevnode + prevnode->size
		       == (vm_offset_t)node);

		// Coalesce prevnode with nextnode if possible.
		if (((vm_offset_t)nextnode)
		    && ((vm_offset_t)node + size >= (vm_offset_t)nextnode))
		{
			assert((vm_offset_t)node + size
			       == (vm_offset_t)nextnode);

			prevnode->size += size + nextnode->size;
			prevnode->next = nextnode->next;
		}
		else
		{
			// Not possible -
			// just grow prevnode around newly freed memory.
			prevnode->size += size;
		}
	}
	else
	{
		// Insert the new node into the free list.
		if (prevnode)
			prevnode->next = node;
		else
			reg->nodes = node;

		// Try coalescing the new node with the nextnode.
		if ((nextnode) &&
		    (vm_offset_t)node + size >= (vm_offset_t)nextnode)
		{
			node->size = size + nextnode->size;
			node->next = nextnode->next;
		}
		else
		{
			node->size = size;
			node->next = nextnode;
		}
	}
}



void MemoryManager::FreePage(void *page)
{
	Free( page, PAGE_SIZE);
}



void MemoryManager::RemoveFree(void *block, vm_size_t block_size)
{
	vm_offset_t rstart = (vm_offset_t)block;
	vm_offset_t rend = rstart + block_size;
	assert(rend > rstart);

	while (rstart < rend)
	{
		vm_offset_t size;
		lmm_flags_t flags;
		void *ptr;

		FindFree(&rstart, &size, &flags);
		assert(rstart >= (vm_offset_t)block);
		if ((size == 0) || (rstart >= rend))
			break;
		if (rstart + size > rend)
			size = rend - rstart;
		ptr = AllocGen(size, flags, 0, 0, rstart, size);
		assert((vm_offset_t)ptr == rstart);
	}
}

