// File: Process.h

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
1998/12/07: Created by Murphy Cheng-Che Chen

1998/12/11: Murphy
	BOOL SetProcessName(const char *pString);
	BOOL SetCommandLineArgument(const char *pString);
1998/12/14: Murphy
	void SetProcessID(DWORD nID);
	void MemoryAllocatorSpace(void *addr, vm_size_t size);
	void* MemoryAllocatorAlloc(vm_size_t size);
	void* PageTableGetBase();
	void PageTableInit();
	void PageTableMapRange(vm_offset_t linear_address,
		vm_offset_t physical_address, vm_size_t size, DWORD mapping_bits);
	DWORD ::CreateProcessFromImage();
1999/01/31: Murphy
	List<DWORD> m_listThread;
	void AddThread(DWORD nThreadID);
	void RemoveThread(DWORD nThreadID);
1999/02/04: Murphy
	Process::~Process()
1999/04/09: Murphy
	Export function : SCMalloc, SCFree.
	void Process::MemoryAllocatorFree(void *ptr);
*/



#ifndef __EKERNEL_PROCESS_H_
#define __EKERNEL_PROCESS_H_



#include "ADT\Array.h"
#include "ADT\List.h"
#include "MemoryManager\MemoryManager.h"
#include "Kernel\Thread\Thread.h"
#include "Hardware\Processor.h"



#define PROCESS_SUCCESS			0x00000000
#define PROCESS_UNRECOGNIZED_IMAGE	0x00000001
#define PROCESS_INVALID_IMAGE		0x00000002
#define PROCESS_MEMORY_SHORTAGE		0x00000004



class Process 
{

private:
	DWORD m_nProcessID;
	char *m_pProcessName;
	char *m_pCommandLineArgument;

	List<DWORD> m_listThread;

	struct lmm_region m_mmRegion;
	MemoryManager m_mmAllocator;

public:
	Process();
	~Process();

	BOOL SetProcessName(const char *pString);
	BOOL SetCommandLineArgument(const char *pString);
	inline void SetProcessID(DWORD nID) { m_nProcessID = nID; }

	void MemoryAllocatorSpace(void *addr, vm_size_t size);
	void* MemoryAllocatorAlloc(vm_size_t size);
	void MemoryAllocatorFree(void *ptr);

	BOOL AddThread(DWORD nThreadID);
	BOOL RemoveThread(DWORD nThreadID);

};



extern DWORD CreateProcessFromImage(
	DWORD *pnProcessID,
	char *pProcessName,
	char *pCommandLineArgument,
	DWORD nPreallocatedSize,
	DWORD nMainThreadStackSize,
	void *pImageStartAddress
);

extern DWORD SCMalloc(void *pArg[]);
extern DWORD SCFree(void *pArg[]);

#endif
