// File: PentiumProcessor.h

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
 1998/12/02: Murphy
	Create this class definition file.
	void SetGDTR(const GDT& gdt);
 1998/12/03: Murphy
	void SetIDTR(const IDT& idt);
	void SetTR(WORD no);
 1998/12/09: Murphy
	void CPUInfoGather();
	void CPUInfoDump();
	BOOL CPUFeatureVerify(unsigned feature);
	void EnablePaging();
	void DisablePaging();
	void SetCR3(DWORD pdir);
 1998/12/27: Murphy
	void InitializeGDT();
	void InitializeIDT();
	void InitializeTSS();
	void InitializePDT();
	void Initialize();
 1999/03/10: Murphy
 	Move paging related stuff to class PentiumMMU.
 1999/03/17: Murphy
 	void EnableInterrupt();
	void DisableInterrupt();
*/

#ifndef __EKERNEL_HARDWARE_PENTIUM_PENTIUMPROCESSOR_H
#define __EKERNEL_HARDWARE_PENTIUM_PENTIUMPROCESSOR_H



#include "Config.h"
#include "Hardware\Processor.h"
#include "cpuid.h"
#include "GDT.h"
#include "IDT.h"
#include "PageDirectory.h"
#include "libc\stdio\stdio.h"
#include "proc_reg.h"



void ctx_sw();				// defined in context_switch.asm
void trigger_timer_interrupt();


class PentiumProcessor : public Processor {

protected:

	cpu_info m_CPUInfo;

	// Each entry in TSSTable contains a TSS for one thread.
	TSS *m_TSSTable;

	// Global Descriptor Table
	GDT m_GDT;

	// Task State Segments for processor initializarion.
	TSS m_TSS;
	TSS m_NullTSS;

	void InitializeGDT();	// Initialize Global Descriptor Table
	void InitializeTSS();	// Initialize Task State Segments

	void SetGDTR(const GDT& gdt);
	void SetTR(WORD no);

public:
	void Initialize();

	void CPUInfoGather();
	void CPUInfoDump();

	inline BOOL CPUFeatureVerify(unsigned feature) { return ((m_CPUInfo.feature_flags & feature)!=0); }

	void SwitchContext(DWORD nOldThreadID, DWORD nNewThreadID);

	void TriggerTimerInterrupt() { trigger_timer_interrupt(); }
	
	void SetThreadContext(DWORD nThreadID, void *pStackStart,
		DWORD nStackSize, void *pEntryFunction, void *pPageTableBase);

	inline void EnableInterrupt() { sti(); }
	inline void DisableInterrupt() { cli(); }
};



#endif
