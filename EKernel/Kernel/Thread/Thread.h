// File: Thread.h

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
1998/12/14: Murphy
	Redefine the Thread class.
	BOOL Thread::SetThreadName(const char *pString);
	void Thread::SetThreadID(DWORD nID);
	void Thread::SetStack(void *pStackStart, DWORD nStackSize);
	DWORD ::CreateThread(
		DWORD *pnThreadID,
		char *pThreadName,
		void *pEntryFunction,
		void *pStackSpace,
		DWORD nStackSize,
		BOOL bAutoStart);	
1998/12/25: Murphy
	void Thread::SetTSS();
	Use TSS for context switching.
1998/12/26: Murphy
	void SetTSS(void *pStackStart, DWORD nStackSize, void *pEntryFunction,
		DWORD nPageTableBaseAddress);
	void SetBackLink(DWORD value);
	void* GetTSSBase();
	DWORD GetTSSLimit();
1999/02/01: Murphy
	THREAD_SUCCESS
	THREAD_MEMORY_SHORTAGE
	THREAD_INVALID_TID
1999/02/04: Murphy
	Thread::Thread()
	Thread::~Thread()
	extern DWORD g_nCurrentThreadID;
*/

#ifndef __EKERNEL_KERNEL_THREAD_H_
#define __EKERNEL_KERNEL_THREAD_H_

#include "Types.h"
#include "Adt\Array.h"
#include "Adt\List.h"



#define THREAD_STATE_FREE		0x00		// this slot is empty
#define THREAD_STATE_RUNNING		0x01		// current running
#define THREAD_STATE_READY		0x02		// ready for execution
#define THREAD_STATE_MESSAGE_SUSPENDED	0x03		// suspended for message queue

#define THREAD_SUCCESS			0x00
#define THREAD_MEMORY_SHORTAGE		0x01
#define THREAD_INVALID_TID		0x02



class Thread
{
private:
	DWORD m_nThreadID;
	char *m_pThreadName;

	DWORD m_nProgramCounter;
	DWORD m_nStackStart;	// the context switch code can check 
				// whether the stack pointer is below
				// this, and thus detect stack overflow.
	DWORD m_nStackPointer;

	DWORD m_nPriority;
	DWORD m_nState;
	DWORD m_nProcessID;	// Which process this thread belongs to

public:   
	Thread();
	~Thread();

	BOOL SetThreadName(const char *pString);

	inline void SetThreadID(DWORD nID) { m_nThreadID = nID; }
	inline DWORD GetThreadID() const { return m_nThreadID; }

	inline void SetProcessID(DWORD nID) { m_nProcessID = nID; }
	inline DWORD GetProcessID() const { return m_nProcessID; }

	inline DWORD GetStackPointer() const { return m_nStackPointer; }
	inline void* GetStackPointerAddress() const { return (void*)&m_nStackPointer; }

	void SetState(DWORD nState);
	
};



extern DWORD CreateThread(
	DWORD *pnThreadID,
	DWORD nProcessID,
	char *pThreadName,
	void *pEntryFunction,
	void *pStackSpace,
	DWORD nStackSize,
	void *pPageTableBase,
	BOOL bAutoStart);
extern DWORD TerminateThread(
	DWORD nThreadID,
	DWORD nProcessID);

extern DWORD SCCreateThread(void *pArg[]);
extern DWORD SCTerminateThread(void *pArg[]);

#endif
