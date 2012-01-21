// File: CreateProcess.cc

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
#include "Initialize\Initialize.h"
#include "Initialize\Globals.h"
#include "Libc\string\string.h"
#include "LibExec\coff.h"

/*
pnProcessID :
	Pointer to a location where Kernel can store the process ID.
pProcessName :
	Pointer to the name of the process.
pCommandLineArgument :
	Pointer to the string of the command line argument for the process.
pImageStartAddress :
	Pointer to the start address of the process image.

This function is supposed to be called inside the kernel.

*/
DWORD CreateProcessFromImage(
	DWORD *pnProcessID,
	char *pProcessName,
	char *pCommandLineArgument,
	DWORD nPreallocatedSize,
	DWORD nMainThreadStackSize,
	void *pImageStartAddress
)
{

	Process *proc=new Process();
	BOOL ret;

	// Set process name
	ret=proc->SetProcessName(pProcessName);
	if(ret==FALSE)
	{
		delete proc;
		return PROCESS_MEMORY_SHORTAGE;
	}
	
	// Set command-line argument
	ret=proc->SetCommandLineArgument(pCommandLineArgument);
	if(ret==FALSE)
	{
		delete proc;
		return PROCESS_MEMORY_SHORTAGE;
	}

	// Decide the process ID
	int nProcessID = g_arrayProcess->FindEmptySlot();
	if(nProcessID<0)
	{
		delete proc;
		return PROCESS_MEMORY_SHORTAGE;
	}
	proc->SetProcessID(nProcessID);
	if(pnProcessID!=0)
		*pnProcessID=nProcessID;


	BYTE *pProcessImage;

	// Analysis the image format
	// Assume the code segment is followed immediately by the data segment,
	// which is followed immediately by the bss segment.
	// Assume the start address of the image is 0x80000000.
//	printf("Image Start Address = %08x\n", pImageStartAddress);

	COFFFileHeader *pCoffHeader = (COFFFileHeader*)pImageStartAddress;
	AOUTHeader *pAoutHeader=(AOUTHeader*)((BYTE*)pImageStartAddress+
		sizeof(COFFFileHeader));

//	printf("COFFFileHeader Start Address = %08x\n", pCoffHeader);
//	printf("AOUTHeader Start Address = %08x\n", pAoutHeader);
//	printf("f_magic=%x\n", pCoffHeader->f_magic);
//	printf("magic=%x\n", pAoutHeader->magic);

	// Check the magic number
	if( 	(pCoffHeader->f_magic != 0x014c) ||
		(pAoutHeader->magic != 0x010b) )
	{
		delete proc;
		return PROCESS_UNRECOGNIZED_IMAGE;
	}

	if(	((pCoffHeader->f_flags & F_RELFLG)==0) ||
		((pCoffHeader->f_flags & F_EXEC)==0) ||
		((pCoffHeader->f_flags & F_AR32WR)==0) ||
		(pAoutHeader->text_start != 0x80000000) )
	{
		delete proc;
		return PROCESS_INVALID_IMAGE;
	}


	// Allocate enough pages
	DWORD nProcessOriginalSize = pAoutHeader->tsize + pAoutHeader->dsize + pAoutHeader->bsize;
	// Process can allocate memory from the address 0x80000000+nProcessSize with the size
	// nPreallocatedSize
	DWORD nProcessTotalSize = nProcessOriginalSize + nPreallocatedSize;
	DWORD nProcessOriginalSizeAligned = (nProcessOriginalSize+(PAGE_SIZE-1))&~(PAGE_SIZE-1);
	DWORD nProcessTotalSizeAligned = (nProcessTotalSize+(PAGE_SIZE-1))&~(PAGE_SIZE-1);
	// A mechansim for tracking which pages are allocated by each process should
	// be implemented later.
	pProcessImage = (BYTE*)g_MemoryManager.AllocGen(nProcessTotalSizeAligned, 0, PAGE_SHIFT, 0, 0, 0xffffffff);
	if(pProcessImage == 0)
	{
		delete proc;
		return PROCESS_MEMORY_SHORTAGE;
	}

	g_MMU->PageTableInitialize(nProcessID);
	g_MMU->PageTableMapRangeUser(nProcessID, 0, 0, GetAvailableMemory());
	g_MMU->PageTableMapRangeUser(nProcessID, 0x80000000, (vm_offset_t) pProcessImage, nProcessTotalSizeAligned);

	// For temporary use
	g_MMU->PageTableMapRangeUser(nProcessID, 0xA0000000, 0xA0000000, 0x10000);

	// Switch to the new process's page directory
	g_MMU->SwitchPageTable(nProcessID);
	
	proc->MemoryAllocatorSpace((void*)((BYTE*)0x80000000+nProcessOriginalSize), nPreallocatedSize);

	COFFSectionHeader *pSectionHeader=(COFFSectionHeader*)((BYTE*)pImageStartAddress+
		sizeof(COFFFileHeader)+(pCoffHeader->f_opthdr));
	for(int i=0; i<pCoffHeader->f_nscns; i++)
	{
		if(strcmp(pSectionHeader->s_name, ".text")==0 &&
			pSectionHeader->s_flags == STYP_TEXT )
		{
//			printf("Code segment, linear=%08x, image offset=%08x, size=%08x\n", pSectionHeader->s_paddr, pSectionHeader->s_scnptr, pSectionHeader->s_size);
			// Copy to physical adderss
			// memcpy(pProcessImage+(pSectionHeader->s_paddr-0x80000000), (BYTE*)pImageStartAddress + pSectionHeader->s_scnptr, pSectionHeader->s_size);
			// Copy to linear address
			memcpy((void*)(pSectionHeader->s_paddr), (BYTE*)pImageStartAddress + pSectionHeader->s_scnptr, pSectionHeader->s_size);
		}
		else if(strcmp(pSectionHeader->s_name, ".data")==0 &&
			pSectionHeader->s_flags == STYP_DATA )
		{
//			printf("Data segment, linear=%08x, image offset=%08x, size=%08x\n", pSectionHeader->s_paddr, pSectionHeader->s_scnptr, pSectionHeader->s_size);
			// Copy to physical adderss
			// memcpy(pProcessImage+(pSectionHeader->s_paddr-0x80000000), (BYTE*)pImageStartAddress + pSectionHeader->s_scnptr, pSectionHeader->s_size);
			// Copy to linear address
			memcpy((void*)(pSectionHeader->s_paddr), (BYTE*)pImageStartAddress + pSectionHeader->s_scnptr, pSectionHeader->s_size);
		} else if(strcmp(pSectionHeader->s_name, ".bss")==0 &&
			pSectionHeader->s_flags == STYP_BSS )
		{
//			printf("BSS segment, linear=%08x, image offset=%08x, size=%08x\n", pSectionHeader->s_paddr, pSectionHeader->s_scnptr, pSectionHeader->s_size);
			// Copy to physical adderss
			// memset(pProcessImage+(pSectionHeader->s_paddr-0x80000000), 0, pSectionHeader->s_size);
			// Copy to linear address
			memset((void*)(pSectionHeader->s_paddr), 0, pSectionHeader->s_size);
		}
		pSectionHeader++;
	}

	// Add the process to global process list.
	g_arrayProcess->SetAt(nProcessID, proc);


	// Allocate space for the stack of the main thread
	//   Allocate from kernel address space
	// void *pStackStart=malloc(nMainThreadStackSize);
	//   Allocate from user address space
	void *pStackStart=proc->MemoryAllocatorAlloc(nMainThreadStackSize);
	if(pStackStart==0)
	{
		delete proc;
		return PROCESS_MEMORY_SHORTAGE;
	}

	// printf("Entry=%08x\n", pAoutHeader->entry);
	// Create a main thread for this process
	CreateThread(0, nProcessID, "MainThread", (void*)pAoutHeader->entry, pStackStart, 
		nMainThreadStackSize, g_MMU->GetPageTableBase(nProcessID), TRUE);

	return PROCESS_SUCCESS;
}

