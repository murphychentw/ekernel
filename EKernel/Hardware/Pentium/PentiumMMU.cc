// File: PentiumMMU.cc

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
 1999/03/10: Created by Murphy Cheng-Che Chen
*/



#include "PentiumMMU.h"
#include "Config.h"
#include "Initialize/Initialize.h"
#include "proc_reg.h"



void PentiumMMU::Initialize()
{
	// Initialize the dummy page table.
	m_PDInit.Allocate();
	m_PDInit.MapRange(0, 0, GetAvailableMemory(),
		PDE_PRESENT | PDE_WRITE | PDE_USER);
		
	// Set the current page table to the dummy page table.
	set_cr3((DWORD)m_PDInit.GetBase());

	m_PDTable = new PageDirectory[MAX_NO_PROCESSES];
}



void* PentiumMMU::GetInitPageTableBase()
{
	return m_PDInit.GetBase();
}



void PentiumMMU::EnablePaging()
{
	// Turn on paging.
	asm volatile("
		movl	%0,%%cr0
		jmp	1f
	1:
	" : : "r" (get_cr0() | CR0_PG));
}



void PentiumMMU::DisablePaging()
{
	// Turn paging off.
	asm volatile("
		movl	%0,%%cr0
		jmp	1f
	1:
	" : : "r" (get_cr0() & ~CR0_PG));

	// Flush the TLB.
	set_cr3(0);
}



void PentiumMMU::SwitchPageTable(DWORD nProcessID)
{
	void *pPageTableBase = GetPageTableBase(nProcessID);
	set_cr3((DWORD)pPageTableBase);
}



void PentiumMMU::PageTableInitialize(DWORD nProcessID)
{
	m_PDTable[nProcessID].Allocate();
}



void PentiumMMU::PageTableMapRangeKernel(
	DWORD nProcessID,
	vm_offset_t linear_address,
	vm_offset_t physical_address,
	vm_size_t size)
{
	m_PDTable[nProcessID].MapRange(linear_address, physical_address, size,
		PDE_PRESENT | PDE_WRITE);
}



void PentiumMMU::PageTableMapRangeUser(
	DWORD nProcessID,
	vm_offset_t linear_address,
	vm_offset_t physical_address,
	vm_size_t size)
{
	m_PDTable[nProcessID].MapRange(linear_address, physical_address, size,
		PDE_PRESENT | PDE_WRITE | PDE_USER);
}



void* PentiumMMU::GetPageTableBase(DWORD nProcessID)
{
	return m_PDTable[nProcessID].GetBase();
}
