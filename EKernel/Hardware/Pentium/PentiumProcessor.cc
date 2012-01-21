// File: PentiumProcessor.cc

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
	Create this class implementation file.
*/

#include "PentiumProcessor.h"
#include "Config.h"
#include "cpuid.h"
#include "proc_reg.h"
#include "LibC/stdio/stdio.h"
#include "LibC/malloc/malloc.h"
#include "Initialize/Initialize.h"
#include "Hardware/MMU.h"



extern MMU *g_MMU;
extern void SystemCallStub(void);



static void NullThread(void)
{
	printf("start of null thread!\n");

	while(1)
	{
		printf("null thread!\n");
	}
}



void PentiumProcessor::Initialize()
{
	// Detect the current processor.
	CPUInfoGather();

	// Initialize the processor tables.
	InitializeGDT();
	InitializeTSS();

	SetGDTR(m_GDT);
	SetTR(GDT_KERNEL_TSS);
}


void PentiumProcessor::InitializeGDT()
{
	m_GDT.Init();

	// Initialize the 32-bit kernel code and data segment descriptors
	// to point to the base of the kernel linear space region.
	m_GDT.FillDescriptor(KERNEL_CS/8,
			0x00000000, 0xffffffff,
			ACC_PL_K | ACC_CODE_R, SZ_32);
	m_GDT.FillDescriptor(KERNEL_DS/8,
			0x00000000, 0xffffffff,
			ACC_PL_K | ACC_DATA_W, SZ_32);

	// Initialize the 32-bit user code and data segment descriptors
	// to point to the base of the user linear space region.
	m_GDT.FillDescriptor(USER_CS/8,
			0x00000000, 0xffffffff,
			ACC_PL_U | ACC_CODE_R, SZ_32);
	m_GDT.FillDescriptor(USER_DS/8,
			0x00000000, 0xffffffff,
			ACC_PL_U | ACC_DATA_W, SZ_32);

	// Initialize User LDT
	m_GDT.FillDescriptor(GDT_USER_LDT/8,
			0x00000000, 0x1000,
			ACC_PL_U | ACC_LDT, SZ_32);

	// Initialize Call Gate for System Service
	m_GDT.FillGate(GDT_SYSTEM_CALL/8,
			KERNEL_CS, (DWORD)SystemCallStub,
			ACC_PL_U | ACC_CALL_GATE, 0);

}

void PentiumProcessor::InitializeTSS()
{
	void* stack_m_TSS=malloc(1024);
	void* stack_m_NullTSS=malloc(1024);
	assert(stack_m_TSS);
	assert(stack_m_NullTSS);

	m_TSS.SetCS(KERNEL_CS);
	m_TSS.SetDS(KERNEL_DS);
	m_TSS.SetES(KERNEL_DS);
	m_TSS.SetFS(KERNEL_DS);
	m_TSS.SetGS(KERNEL_DS);
	m_TSS.SetSS(KERNEL_DS);
	m_TSS.SetESP(((DWORD)stack_m_TSS)+1020);
	m_TSS.SetEIP((DWORD)NullThread);
	m_TSS.SetIOBitMapOffset();
	m_TSS.SetEFlags(EFL_IF);
	m_TSS.SetCR3((DWORD)g_MMU->GetInitPageTableBase());
	m_TSS.SetBackLink(GDT_V86_TSS);
	m_TSS.SetLDT(GDT_USER_LDT);
	m_GDT.FillDescriptor(GDT_KERNEL_TSS/8,
			(DWORD)m_TSS.GetBase(), m_TSS.GetLimit(),
			ACC_PL_K | ACC_TSS, 0);

	m_NullTSS.SetCS(KERNEL_CS);
	m_NullTSS.SetDS(KERNEL_DS);
	m_NullTSS.SetES(KERNEL_DS);
	m_NullTSS.SetFS(KERNEL_DS);
	m_NullTSS.SetGS(KERNEL_DS);
	m_NullTSS.SetSS(KERNEL_DS);
	m_NullTSS.SetESP(((DWORD)stack_m_NullTSS)+1020);
	m_NullTSS.SetEIP((DWORD)NullThread);
	m_NullTSS.SetIOBitMapOffset();
	m_NullTSS.SetEFlags(EFL_IF);
	m_NullTSS.SetCR3((DWORD)g_MMU->GetInitPageTableBase());
	m_NullTSS.SetLDT(GDT_USER_LDT);
	m_GDT.FillDescriptor(GDT_V86_TSS/8,
			(DWORD)m_NullTSS.GetBase(), m_NullTSS.GetLimit(),
			ACC_PL_K | ACC_TSS_BUSY | ACC_TSS, 0);

	// Initialize TSS Table
	m_TSSTable = new TSS[MAX_NO_THREADS];
}



void PentiumProcessor::CPUInfoGather()
{
	cpuid(&m_CPUInfo);
}



void PentiumProcessor::CPUInfoDump()
{
	cpu_info_dump(&m_CPUInfo);
}






void PentiumProcessor::SetGDTR(const GDT& gdt)
{
	struct pseudo_descriptor pdesc;

	// Create a pseudo-descriptor describing the GDT.
	pdesc.limit = gdt.GetLimit();
	pdesc.linear_base = (DWORD)gdt.GetBase();

//	printf("Base=%8x, Limit=%8x\n", gdt.GetBase(), gdt.GetLimit());

	// Load it into the CPU.
	set_gdt(&pdesc);

	// Reload all the segment registers from the new GDT.
	asm volatile("
		ljmp	%0,$1f
	1:
	" : : "i" (KERNEL_CS));
	set_ds(KERNEL_DS);
	set_es(KERNEL_DS);
	set_ss(KERNEL_DS);

	// Clear out the FS and GS registers by default,
	// since they're not needed for normal execution of GCC code.
	set_fs(0);
	set_gs(0);

}



void PentiumProcessor::SetTR(WORD no)
{
	set_tr(no);
}



void PentiumProcessor::SetThreadContext(
	DWORD nThreadID,
	void *pStackStart,
	DWORD nStackSize,
	void *pEntryFunction,
	void *pPageTableBase)
{

	assert(nThreadID<MAX_NO_THREADS);
	TSS *tss = &m_TSSTable[nThreadID];
	
	// Allocate stack space in Kernel Space for
	// ring 0 interrupt handlers
	void* pESP0=malloc(512);

	memset(pESP0, 0, 512);

	tss->SetCS(USER_CS);
	tss->SetDS(USER_DS);
	tss->SetES(USER_DS);
	tss->SetFS(USER_DS);
	tss->SetGS(USER_DS);
	tss->SetSS(USER_DS);
	tss->SetSS0(KERNEL_DS);
	tss->SetEIP((DWORD)pEntryFunction);
	tss->SetESP0(((DWORD)pESP0)+512);
	tss->SetESP(((DWORD)pStackStart)+nStackSize-4);

	tss->SetLDT(GDT_USER_LDT);

	tss->SetCR3((DWORD)pPageTableBase);

	tss->SetEFlags(EFL_IF);

	tss->SetIOBitMapOffset();

	assert( nThreadID+(GDT_CONTEXT_SWITCH_TSS/8) < GDTSZ );
	m_GDT.FillDescriptor( nThreadID+(GDT_CONTEXT_SWITCH_TSS/8),
			(DWORD)tss->GetBase(), tss->GetLimit(),
			ACC_PL_K | ACC_TSS, 0);

}



void PentiumProcessor::SwitchContext(DWORD nOldThreadID, DWORD nNewThreadID)
{
	// Set busy bit for the new task. This is required when
	// doing task-switching via IRETD.
	if(nNewThreadID != 0xffffffff)
	{
		m_GDT.ModifyDescriptorAccess(nNewThreadID + GDT_CONTEXT_SWITCH_TSS / 8,
			0x02, GDT_OR);
	}
	else
	{
		m_GDT.ModifyDescriptorAccess(GDT_KERNEL_TSS/8,
			0x02, GDT_OR);
	}

	// Set the Back Link field for the current task.
	if(nOldThreadID==0xffffffff)
	{
		m_TSS.SetBackLink((nNewThreadID*8+GDT_CONTEXT_SWITCH_TSS));
	}
	else
	{
		if(nNewThreadID!=0xffffffff)
			m_TSSTable[nOldThreadID].SetBackLink((nNewThreadID*8+GDT_CONTEXT_SWITCH_TSS));
		else
			m_TSSTable[nOldThreadID].SetBackLink(GDT_KERNEL_TSS);
	}

	
	 ctx_sw();
}
