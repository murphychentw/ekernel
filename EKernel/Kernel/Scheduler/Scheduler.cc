// File: Scheduler.cc

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
 1998/12/26: Murphy
	Use hardware-supported task-state-segment and iretd for
		context switching.
 1999/02/02: Murphy
	Modify the funtion scheduler to become a member funtion Schedule
		of class Scheduler.
 1999/02/13: Murphy
	void Scheduler::DisablePreemption();
	void Scheduler::EnablePreemption();
 1999/02/23: Murphy
	Disable interrupts in DisablePreemption() and EnablePreemption().
 1999/03/02: Murphy
	void *m_pCurrentPointer;
	Update m_pCurrentPointer in GetNextThread(), CheckOut().
 1999/03/18: Murphy
 	Move global object g_Scheduler to Initialize\Globals.cc.	
*/



#include "Scheduler.h"

#include "ADT/Array.h"
#include "Initialize/Globals.h"
#include "Kernel/Process/Process.h"
#include "Kernel/Thread/Thread.h"
#include "LibC/stdio/stdio.h"
#include "LibC/String/String.h"



Scheduler::Scheduler() : m_nPreemptionCount(0), m_pCurrentThreadPointer(0)
{
}



DWORD Scheduler::GetNextThread(void)
{
//	printf("pIter=%8x\n", pIter);
//	printf("GetNextThread()::Count=%d\n", m_queueReady.GetCount());

	if(m_queueReady.GetCount()<=0)
		//return g_nCurrentThreadID;
		return 0xffffffff;

	if(m_pCurrentThreadPointer)
	{
		DWORD value=m_queueReady.GetNext(m_pCurrentThreadPointer);
		return m_queueReady.GetAt(m_pCurrentThreadPointer);
	}
	else	// first time
	{
		m_pCurrentThreadPointer=m_queueReady.GetHeadPosition();
		return m_queueReady.GetAt(m_pCurrentThreadPointer);
	}
}



void Scheduler::Schedule(void)
{
	// If preemption has been disabled, return immediately.
	if(m_nPreemptionCount > 0)
	{
//		printf("Pass By Context Switch!\n");
		return;
	}

//	printf("Schedule! Preemption Count=%d\n", m_nPreemptionCount);

	DWORD nOldThread = g_nCurrentThreadID;
	DWORD nNewThread;

//	printf("Old process ID=%x\n", g_nCurrentProcessID);
//	printf("Old thread ID=%x\n", g_nCurrentThreadID);

	// Pick a new thread
	nNewThread = GetNextThread();

//	printf("Old thread ID=%x\n", nOldThread);
//	printf("New thread ID=%x\n", nNewThread);

	// If no context switch need to be done, just return.
	if(nNewThread == nOldThread)
	{
		return;
	}

	////////// Decide current thread ID and current process ID ////////
	g_nCurrentThreadID = nNewThread;
	
	// TODO: Delay the determination of current process until requested.
	if(nNewThread!=0xffffffff)
	{
		Thread* pNewThread;
		pNewThread=g_arrayThread->GetAt(nNewThread);
		g_nCurrentProcessID = pNewThread->GetProcessID();
		
	}
	else
		g_nCurrentProcessID = 0xffffffff;

	// Context Switch
	g_Processor->SwitchContext(nOldThread, nNewThread);

	return;
}



void Scheduler::DisablePreemption()
{
	// In fact, only timer interrupt need to be disabled here.
	g_Processor->DisableInterrupt();
	m_nPreemptionCount++;
	g_Processor->EnableInterrupt();
}



void Scheduler::EnablePreemption()
{
	// In fact, only timer interrupt need to be disabled here.
	g_Processor->DisableInterrupt();
	if(m_nPreemptionCount>0)
		m_nPreemptionCount--;
	g_Processor->EnableInterrupt();
}



BOOL Scheduler::CheckIn(DWORD nThreadID)
{
	// Check whether the thread with nThreadID already existed
	// 	in the ready queue.

	DisablePreemption();

//	printf("Check In : %x\n", nThreadID);

	// Traverse the list.
#if 0
	printf("Traverse the list:\n");
	if(m_queueReady.GetCount()>0)
	{
		void *pIter=m_queueReady.GetHeadPosition();
		while(1)
		{
			printf("pIter=%x value=%x\n", pIter, m_queueReady.GetAt(pIter));
			m_queueReady.GetNext(pIter);
			if(pIter==m_queueReady.GetHeadPosition())
				break;
		}
	}	
#endif

	// Add the thread with nThreadID into the ready queue.
	m_queueReady.AddTail(nThreadID);


#if 0
	// Traverse the list.
	printf("Traverse the list:\n");
	if(m_queueReady.GetCount()>0)
	{
		void *pIter=m_queueReady.GetHeadPosition();
		while(1)
		{
			printf("pIter=%x value=%x\n", pIter, m_queueReady.GetAt(pIter));
			m_queueReady.GetNext(pIter);
			if(pIter==m_queueReady.GetHeadPosition())
				break;
		}
	}	
#endif

	EnablePreemption();

	return TRUE;
}



BOOL Scheduler::CheckOut(DWORD nThreadID)
{
	// Check whether the thread with nThreadID exisis in
	//	the ready queue.

	DisablePreemption();

//	printf("Check Out : %x\n", nThreadID);

	void *pIter;

	// Traverse the list.
#if 0
	printf("Traverse the list:\n");
	if(m_queueReady.GetCount()>0)
	{
		pIter=m_queueReady.GetHeadPosition();
		while(pIter!=0)
		{
			printf("pIter=%x value=%x\n", pIter, m_queueReady.GetAt(pIter));
			m_queueReady.GetNext(pIter);
			if(pIter==m_queueReady.GetHeadPosition())
				break;
		}
	}
#endif

	// Remove the thread with nThreadID from the ready queue.
	if(m_queueReady.GetCount()>0)
	{
		pIter=m_queueReady.GetHeadPosition();
		while(pIter!=0)
		{
//			printf("pIter=%x value=%x\n", pIter, m_queueReady.GetAt(pIter));
			if(m_queueReady.GetAt(pIter)==nThreadID)
			{
				m_queueReady.GetNext(m_pCurrentThreadPointer);
				m_queueReady.RemoveAt(pIter);
//				printf("Check Out : Succeeded!\n");		
				EnablePreemption();
				return TRUE;
			}
			m_queueReady.GetNext(pIter);
			if(pIter==m_queueReady.GetHeadPosition())
				break;
		}	
	}

	// Traverse the list.
#if 0
	printf("Traverse the list:\n");
	if(m_queueReady.GetCount()>0)
	{
		pIter=m_queueReady.GetHeadPosition();
		while(pIter!=0)
		{
			printf("pIter=%x value=%x\n", pIter, m_queueReady.GetAt(pIter));
			m_queueReady.GetNext(pIter);
			if(pIter==m_queueReady.GetHeadPosition())
				break;
		}
	}
#endif


	EnablePreemption();

	return FALSE;
}



void Scheduler::Yield()
{
	g_Processor->TriggerTimerInterrupt();
}
