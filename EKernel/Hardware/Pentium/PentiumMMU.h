// File: PentiumMMU.h

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



#ifndef __EKERNEL_HARDWARE_PENTIUMMMU_H
#define __EKERNEL_HARDWARE_PENTIUMMMU_H



#include "Hardware/MMU.h"
#include "PageDirectory.h"
#include "Types.h"



class PentiumMMU : public MMU {

protected:
	// page directory for initialization
	PageDirectory m_PDInit;

	// each entry in PDTable contains a PageDirectory for one process.
	PageDirectory *m_PDTable;

public:
	void Initialize();

	// Get the base of the page table which is used for initialzation.
	void* GetInitPageTableBase();


	///////////// Low-level Paging related member functions //////////////

	// Enable paging mechanism.
	void EnablePaging();
	
	// Disable paging mechanism.
	void DisablePaging();



	///////// Process oriented Paging related member functions ///////////

	// Switch to a new page table of nProcessID.
	void SwitchPageTable(DWORD nProcessID);

	// Initialize necessary page table related data structures for all
	// processes.
	void PageTableInitialize(DWORD nProcessID);
	
	// Map pages for nProcessID from linear_address to physical_address,
	// and set the attributes of the pages to be priviledged, i.e., only
	// kernel can access these pages.
	void PageTableMapRangeKernel(
			DWORD nProcessID,
			vm_offset_t linear_address,
			vm_offset_t physical_address,
			vm_size_t size);

	// Map pages for nProcessID from linear_address to physical_address,
	// and set the attributes of the pages to be non-priviledged, i.e.,
	// user application can freely access these pages.
	void PageTableMapRangeUser(
			DWORD nProcessID,
			vm_offset_t linear_address,
			vm_offset_t physical_address,
			vm_size_t size);

	// Get the physical address of the page table for nProcessID.
	void* GetPageTableBase(DWORD nProcessID);
};



#endif
