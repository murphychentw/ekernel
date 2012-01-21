// File: Semaphore.cc

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
1999/02/02: Created by Murphy Cheng-Che Chen
1999/02/03: Murphy
	Implement Wait() and Signal().
1999/02/04: Murphy
	Implement Semaphore() ~Semaphore() SetSemaphoreName().
	Define g_arraySemaphore->
1999/02/13: Murphy
	Do a context switch after checking out a thread from scheduler.
	Make sure the atomity of the increasing and decreasing of the counter
	of the semaphore by calling Scheduler::DisablePreemption() and
	Scheduler::EnablePreemption().
1999/03/02: Murphy
	Call Scheduler::Yield() in Wait() to truly yield control.
*/



#include "Semaphore.h"
#include "ADT\Array.h"
#include "Initialize\Globals.h"
#include "Kernel\Thread\Thread.h"
#include "Kernel\Scheduler\Scheduler.h"
#include "LibC\stdio\stdio.h"
#include "LibC\String\String.h"



Semaphore::Semaphore() : m_pSemaphoreName(0)
{
}



Semaphore::~Semaphore()
{
	if(m_pSemaphoreName!=0)
	{
		free(m_pSemaphoreName);
	}
}



BOOL Semaphore::SetSemaphoreName(const char *pString)
{
	if(pString!=0)
	{
		char *buf=(char*)malloc(strlen(pString)+1);
		if(buf==0)
			return FALSE;
		strcpy(buf, pString);
		m_pSemaphoreName=buf;
	}
	else
		m_pSemaphoreName=0;

	return TRUE;
}



DWORD Semaphore::Wait(BOOL bWait)
{
	g_Scheduler->DisablePreemption();

//	printf("Semaphore::Wait Requested by Thread %x\n", g_nCurrentThreadID);
//	printf("Semaphore Count = %d\n", m_nCounter);
//	printf("m_queueThread.GetCount = %d\n", m_queueThread.GetCount());

	if(m_nCounter>0)
	{
		m_nCounter--;
//		printf("Semaphore::Wait End (1) \n");
		g_Scheduler->EnablePreemption();
	}
	else
	{
		if(bWait==FALSE)
		{
			g_Scheduler->EnablePreemption();
			return SEMAPHORE_BUSY;
		}
		else
		{
			m_queueThread.AddTail(g_nCurrentThreadID);
			g_Scheduler->CheckOut(g_nCurrentThreadID);
//			printf("Semaphore::Wait End (2) \n");
			g_Scheduler->EnablePreemption();
			g_Scheduler->Yield();
		}
	}

	return SEMAPHORE_SUCCESS;
}



DWORD Semaphore::Signal()
{
	g_Scheduler->DisablePreemption();

//	printf("Semaphore::Signal Requested by Thread %x\n", g_nCurrentThreadID);
//	printf("Semaphore Count = %d\n", m_nCounter);
//	printf("m_queueThread.GetCount = %d\n", m_queueThread.GetCount());

	if(m_queueThread.GetCount()>0)
	{
		DWORD nThreadID = m_queueThread.GetHead();
		m_queueThread.RemoveHead();
		g_Scheduler->CheckIn(nThreadID);
//		printf("Semaphore::Signal End (1) \n");
		g_Scheduler->EnablePreemption();
	}
	else
	{
		m_nCounter++;
//		printf("Semaphore::Signal End (2) \n");
		g_Scheduler->EnablePreemption();
	}

	return SEMAPHORE_SUCCESS;
}
