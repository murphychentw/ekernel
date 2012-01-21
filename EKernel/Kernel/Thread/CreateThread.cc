// File: CreateThread.cc

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
 1999/01/27: Created by Murphy Cheng-Che Chen
	Move out the function CreateThread from Thread.cc
 1999/01/31: Murphy
	Add the thread to its process's thread list.
 1999/02/04: Murphy
	Release allocated memory when a thread cannot be created successfully.
	Return nThreadID through pointer pnThreadID.
*/

#include "Thread.h"
#include "Initialize\Globals.h"
#include "Kernel\Process\Process.h"
#include "Kernel\Scheduler\Scheduler.h"



DWORD CreateThread(
	DWORD *pnThreadID,
	DWORD nProcessID,
	char *pThreadName,
	void *pEntryFunction,
	void *pStackSpace,
	DWORD nStackSize,
	void *pPageTableBase,
	BOOL bAutoStart)
{
	Thread *thread=new Thread();
	BOOL ret;

	// Set thread name
	ret=thread->SetThreadName(pThreadName);
	if(ret==FALSE)
	{
		delete thread;
		return THREAD_MEMORY_SHORTAGE;
	}

	// Decide the thread ID
	int nThreadID=g_arrayThread->FindEmptySlot();
	if(nThreadID<0)
	{
		delete thread;
		return THREAD_MEMORY_SHORTAGE;
	}
	thread->SetThreadID(nThreadID);

	// Set Process ID
	thread->SetProcessID(nProcessID);

	// Setup Stack
	g_Processor->SetThreadContext(nThreadID, pStackSpace, nStackSize,
		pEntryFunction, pPageTableBase);

	// Add the thread to the global thread list.
	g_arrayThread->SetAt(nThreadID, thread);

	if(bAutoStart==TRUE)
	{
		// Add the thread identifier to the ready queue of the scheduler
		g_Scheduler->CheckIn(nThreadID);
	}

	// Add the thread to its process's thread list.
	(g_arrayProcess->GetAt(nProcessID))->AddThread(nThreadID);

	// Return nThreadID through pointer pnThreadID
	*pnThreadID = nThreadID;

	return THREAD_SUCCESS;
}
