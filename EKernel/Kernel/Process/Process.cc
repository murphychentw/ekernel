// File: Process.cc

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
 1998/12/07: Created by Murphy Cheng-Che Chen
 1999/02/04: Murphy
	Move g_list_process and g_nCurrentProcessID from kernel_initialize.cc
	Process::~Process()
 1999/03/18: Murphy
 	Move global objects g_list_process and g_nCurrentProceessID to
 		Initialize\Globals.cc.
 1999/04/08: Murphy
 	From	Array<Process*> g_list_process
 	To	Array<Process*> *g_arrayProcess
*/


#include "Process.h"
#include "Libc\malloc\malloc.h"
#include "Libc\stdio\stdio.h"
#include "Libc\string\string.h"
#include "Hardware\MMU.h"



Process::Process() : m_pProcessName(0) , m_pCommandLineArgument(0)
{
	m_mmAllocator.Init();
}



Process::~Process()
{
	if(m_pProcessName!=0)
	{
		free(m_pProcessName);
	}

	if(m_pCommandLineArgument!=0)
	{
		free(m_pProcessName);
	}
}



void Process::MemoryAllocatorSpace(void *addr, vm_size_t size)
{
	m_mmAllocator.AddRegion(&m_mmRegion, addr, size, 0, 0);
	m_mmAllocator.AddFree(addr, size);
}



void* Process::MemoryAllocatorAlloc(vm_size_t size)
{
	return m_mmAllocator.Alloc(size, 0);
}



void Process::MemoryAllocatorFree(void *ptr)
{
	vm_size_t *chunk = (vm_size_t*)ptr - 1;

	m_mmAllocator.Free((void*)chunk, *chunk);
}



BOOL Process::SetProcessName(const char *pString)
{
	if(pString!=0)
	{
		char *buf=(char*)malloc(strlen(pString)+1);
		if(buf==0)
			return FALSE;
		strcpy(buf, pString);
		m_pProcessName=buf;
	}
	else
		m_pProcessName=0;

	return TRUE;
}



BOOL Process::SetCommandLineArgument(const char *pString)
{
	if(pString!=0)
	{
		char *buf=(char*)malloc(strlen(pString)+1);
		if(buf==0)
			return FALSE;
		strcpy(buf, pString);
		m_pCommandLineArgument=buf;
	}
	else
		m_pCommandLineArgument=0;

	return TRUE;
}



BOOL Process::AddThread(DWORD nThreadID)
{
	// Check whether the thread already existed in the thread list

	m_listThread.AddHead(nThreadID);
	return TRUE;
}



BOOL Process::RemoveThread(DWORD nThreadID)
{
	void *pIter=m_listThread.GetHeadPosition();
	while(pIter!=0)
	{
		if(m_listThread.GetNext(pIter)==nThreadID)
		{
			m_listThread.RemoveAt(pIter);

			// If the process now contains no threads,
			// 	terminate the process.
			if(m_listThread.GetCount()==0)
			{
			}

			return TRUE;
		}
		
	}

	// The thread does not exist in the thread list
	return FALSE;
}
