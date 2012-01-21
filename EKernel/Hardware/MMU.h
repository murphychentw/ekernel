// File: MMU.h

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
	virtual void Initialize()=0;
	virtual void GetInitPageTableBase()=0;
	virtual void PageTableInitialize(DWORD nProcessID)=0;
	virtual void PageTableMapRangeKernel(
			DWORD nProcessID,
			vm_offset_t linear_address,
			vm_offset_t physical_address,
			vm_size_t size)=0;
	virtual void PageTableMapRangeUser(
			DWORD nProcessID,
			vm_offset_t linear_address,
			vm_offset_t physical_address,
			vm_size_t size)=0;
	virtual void* GetPageTableBase(DWORD nProcessID)=0;
*/



#ifndef __EKERNEL_HARDWARE_MMU_H
#define __EKERNEL_HARDWARE_MMU_H



#include "Types.h"



class MMU {

public:
	virtual void Initialize()=0;
	
	virtual void* GetInitPageTableBase()=0;



	///////////// Low-level Paging related member functions //////////////

	// Enable paging mechanism.
	virtual void EnablePaging()=0;
	
	// Disable paging mechanism.
	virtual void DisablePaging()=0;



	///////// Process oriented Paging related member functions ///////////

	// Switch to a new page table of nProcessID.
	virtual void SwitchPageTable(DWORD nProcessID)=0;

	// Initialize Page Table
	virtual void PageTableInitialize(DWORD nProcessID)=0;
	
	// Map pages for nProcessID from linear_address to physical_address,
	// and set the attributes of the pages to be priviledged, i.e., only
	// kernel can access these pages.
	virtual void PageTableMapRangeKernel(
			DWORD nProcessID,
			vm_offset_t linear_address,
			vm_offset_t physical_address,
			vm_size_t size)=0;

	// Map pages for nProcessID from linear_address to physical_address,
	// and set the attributes of the pages to be non-priviledged, i.e.,
	// user application can freely access these pages.
	virtual void PageTableMapRangeUser(
			DWORD nProcessID,
			vm_offset_t linear_address,
			vm_offset_t physical_address,
			vm_size_t size)=0;

	// Get the physical address of the page table for nProcessID.
	virtual void* GetPageTableBase(DWORD nProcessID)=0;
};



#endif
