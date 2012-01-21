// File: multi_boot_main.cc

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

// Copyright (c) 1996 The University of Utah and
// the Computer Systems Laboratory at the University of Utah (CSL).
// All rights reserved.
//
// Permission to use, copy, modify and distribute this software is hereby
// granted provided that (1) source code retains these copyright, permission,
// and disclaimer notices, and (2) redistributions including binaries
// reproduce the notices in supporting documentation, and (3) all advertising
// materials mentioning features or use of this software display the following
// acknowledgement: ``This product includes software developed by the
// Computer Systems Laboratory at the University of Utah.''
//
// THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
// IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
// ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
//
// CSL requests users of this software to return to csl-dist@cs.utah.edu any
// improvements that they make and grant CSL redistribution rights.

/*
 1998/12/06: Murphy
	Modify the initialization sequence.
	Move the initialization of the memory allocator to front. Because
	after the environment for memory allocation has been setup, many
	C/C++ routines can start to work.
 1999/01/06: Murphy
	extern DWORD* djgpp_first_ctor;
	extern DWORD* djgpp_last_ctor;
	Use the above function pointers for calling global object constructors
	in the beginning.
	extern DWORD* djgpp_first_dtor;
	extern DWORD* djgpp_last_dtor;
 1999/02/01: Murphy
	serial_init();
 1999/03/10: Murphy
	Add a global object for MMU.
	Add a global object for InterruptController.
*/

#include "multi_boot.h"
#include "Initialize/Initialize.h"
#include "Initialize/Globals.h"
#include "LibC/stdio/serial_io.h"
#include "LibC/stdio/stdio.h"



extern DWORD* djgpp_first_ctor;
extern DWORD* djgpp_last_ctor;
extern DWORD* djgpp_first_dtor;
extern DWORD* djgpp_last_dtor;


struct multiboot_info boot_info;



static void InitializeMemory(void);
static void phys_lmm_init(void);
static void phys_lmm_add(vm_offset_t min_pa, vm_size_t size);



void multiboot_main(vm_offset_t boot_info_pa)
{
	// Copy the multiboot_info structure into our pre-reserved area.
	// This avoids one loose fragment of memory that has to be avoided.
	boot_info = *(struct multiboot_info*)boot_info_pa;

	g_nAvailableMemory = 0x100000 + boot_info.mem_upper * 1024;

	// Initialize serial port for outputing kernel messages.
	serial_init();

	// Initialize the memory allocator and find all available memory.
	// After this, the C routine 'malloc' and C++ operator 'new' 
	// are able to start to work.
	InitializeMemory();

	// Calling constructors of global objects
//	printf("\nfirst_ctor=%08x, last_ctor=%08x\n", djgpp_first_ctor, djgpp_last_ctor);
	DWORD* ctors=djgpp_first_ctor;
	while( ctors <= djgpp_last_ctor )
	{
//		printf("Calling %08x\n", *ctors);
		((void(*)())(*ctors))();
		ctors++;
	}

	Initialize();
}



// ------------------- multiboot_init_mem ---------------------------


#define skip(hole_min, hole_max)					\
	if ((max > (hole_min)) && (min < (hole_max)))			\
	{								\
		if (min < (hole_min))					\
			max = (hole_min);				\
		else {							\
			min = (hole_max);				\
			continue;					\
		}							\
	}


extern vm_offset_t start, end;

static void InitializeMemory(void)
{

	vm_offset_t min;

	g_MemoryManager.Init();

	// Memory regions to skip.
	vm_offset_t cmdline_start_pa = boot_info.flags & MULTIBOOT_CMDLINE
		? boot_info.cmdline : 0;
	vm_offset_t cmdline_end_pa = cmdline_start_pa
		? cmdline_start_pa+strlen((char*)cmdline_start_pa)+1
		: 0;

	// Initialize the base memory allocator
	// according to the PC's physical memory regions.
	phys_lmm_init();

	// Add to the free list all the memory the boot loader told us about,
	// carefully avoiding the areas occupied by boot information.
	// as well as our own executable code, data, and bss.
	// Start at the end of the BIOS data area.
	min = 0x500;
	do
	{
		vm_offset_t max = 0xffffffff;

		// Skip the I/O and ROM area.
		skip(boot_info.mem_lower * 1024, 0x100000);

		// Stop at the end of upper memory.
		skip(0x100000 + boot_info.mem_upper * 1024, 0xffffffff);

		// Skip our own text, data, and bss.
//		printf("skip start=%x, end=%x\n",
//			&start, &end);
		skip((vm_offset_t)&start, (vm_offset_t)&end);

		// Skip the important stuff the bootloader passed to us.
		skip(cmdline_start_pa, cmdline_end_pa);

		if ((boot_info.flags & MULTIBOOT_MODS)
		    && (boot_info.mods_count > 0))
		{
			struct multiboot_module *m = (struct multiboot_module*)
				boot_info.mods_addr;
			unsigned i;

			skip(boot_info.mods_addr,
			     boot_info.mods_addr +
			     boot_info.mods_count * sizeof(*m));
			for (i = 0; i < boot_info.mods_count; i++)
			{
				if (m[i].string != 0)
				{
					char *s = (char*)m[i].string;
					unsigned len = strlen(s);
					skip(m[i].string, m[i].string+len+1);
				}
				skip(m[i].mod_start, m[i].mod_end);
			}
		}

		// We actually found a contiguous memory block
		// that doesn't conflict with anything else!  Whew!
		// Add it to the free list.
//		printf("Add min=%x, max=%x\n",
//			min, max);
		phys_lmm_add(min, max - min);

		// Continue searching just past the end of this region.
		min = max;

		// The skip() macro jumps to this label
		// to restart with a different (higher) min address.
	}
	while (min < 0xffffffff);
}



// -------------------------- phys_lmm_init -------------------------------

static int inited;
static struct lmm_region reg1mb, reg16mb, reghigh;

// This routine sets up the malloc_lmm with three regions,
// one for each of the memory types above.
// You can then call phys_lmm_add() to add memory to those regions.
static void phys_lmm_init(void)
{
	// It woudn't be good to add these regions more than once.
	if (inited) return;
	inited = 1;

	g_MemoryManager.AddRegion(&reg1mb,
			(void*)0x00000000, 0x00100000,
			LMMF_1MB | LMMF_16MB, LMM_PRI_1MB);
	g_MemoryManager.AddRegion(&reg16mb,
			(void*)0x00100000, 0x00f00000,
			LMMF_16MB, LMM_PRI_16MB);
	g_MemoryManager.AddRegion(&reghigh,
			(void*)0x01000000, 0xfeffffff,
			0, LMM_PRI_HIGH);
}



// ----------------------- phys_lmm_add ---------------------------------

// The routines phys_lmm_init and phys_lmm_add keep this variable up-to-date
// so that it always records the highest physical memory address seen so far;
// i.e. the end address of the highest free block added to the free list.
// After all physical memory is found and collected on the free list,
// this variable will contain the highest physical memory address
// that the kernel should ever have to deal with.
vm_offset_t phys_mem_max;


// Call one of these routines to add a chunk of physical memory found
// to the appropriate region(s) on the malloc_lmm.
// The provided memory block may be arbitrarily aligned
// and may cross region boundaries (e.g. the 16MB boundary);
// it will be shrunken and split apart as necessary.
// Note that these routines take _physical_ addresses,
// not virtual addresses as the underlying LMM routines do.
static void phys_lmm_add(vm_offset_t min_pa, vm_size_t size)
{
	vm_offset_t max_pa = min_pa + size;

	// Add the memory region to the malloc_lmm.
	// The LMM package will take care of splitting it up if necessary.
	g_MemoryManager.AddFree((void*)min_pa, size);

	if (max_pa > phys_mem_max)
		phys_mem_max = max_pa;
}

