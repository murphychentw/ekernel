// File: Processor.h

/*
 Copyright (c) 1998-1999 Murphy Cheng-Che Chen. All Rights Reserved.

 This software is provided "AS IS," without a warranty of any kind. ALL
 EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MURPHY CHENG-CHE CHEN AND HIS
 LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A
 RESULT OF USING, MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES.
 IN NO EVENT WILL MURPHY CHENG-CHE CHEN OR HIS LICENSORS BE LIABLE FOR ANY
 LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL,
 CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS
 OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE
 SOFTWARE, EVEN IF MURPHY CHENG-CHE CHEN HAS BEEN ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGES.

 murphy@hpc.ee.ntu.edu.tw
 http://hpc.ee.ntu.edu.tw/~murphy/
 
*/

/*
 1999/03/07: Created by Murphy Cheng-Che Chen
 	virtual void Initialize()=0;
	virtual void CPUInfoGather()=0;
	virtual void CPUInfoDump()=0;
	virtual BOOL CPUFeatureVerify(unsigned feature)=0;
 1999/03/08: Murphy
	virtual void SetPageTableBase(DWORD pdir)=0;
	virtual void TriggerTimerInterrupt()=0;
	virtual void SetThreadContext(DWORD nThreadID, void *pStackStart,
			DWORD nStackSize, void *pEntryFunction,
			void *pPageTableBase)=0;	
	virtual void SwitchContext(DWORD nOldThreadID, DWORD nNewThreadID)=0; 
	virtual void InitializePageTable(DWORD nProcessID)=0;
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
 1999/03/08: Murphy
 	Move paging related member functions to class MMU.
 1999/03/17: Murphy
 	void EnableInterrupt();
	void DisableInterrupt();
*/

#ifndef __EKERNEL_HARDWARE_PROCESSOR_H
#define __EKERNEL_HARDWARE_PROCESSOR_H



#include "Types.h"



class Processor {

public:
	virtual void Initialize()=0;

	virtual void CPUInfoGather()=0;
	virtual void CPUInfoDump()=0;

	virtual BOOL CPUFeatureVerify(unsigned feature)=0;

	// move to InterruptController
	virtual void TriggerTimerInterrupt()=0;
	
	virtual void SetThreadContext(DWORD nThreadID, void *pStackStart,
			DWORD nStackSize, void *pEntryFunction,
			void *pPageTableBase)=0;	
	virtual void SwitchContext(DWORD nOldThreadID, DWORD nNewThreadID)=0;
	
	virtual void EnableInterrupt()=0;
	virtual void DisableInterrupt()=0;
};



#endif
