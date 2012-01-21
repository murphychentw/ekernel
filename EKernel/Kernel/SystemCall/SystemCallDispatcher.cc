// File: SystemCallDispatcher.cc

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
1998/12/27: Created by Murphy Cheng-Che Chen
1999/02/24: Murphy
	Disable preemption when executing any system call.
	(This may be too restricted.)
1999/03/01: Murphy
	Remove 'DisablePreemption()' and 'EnablePreemption()' in Dispatch();
1999/03/24: Murphy
	SCNull: a null system call for performance measurement.
1999/03/26: Murphy
	SCSwitch: a system call used to do a context switch for
		performance measurement.
1999/04/08: Murphy
	Hook system calls for event related functions.
*/



#include "SystemCallDispatcher.h"


#include "LibC\stdio\stdio.h"
#include "LibC\assert\assert.h"
#include "Kernel\Event\Event.h"
#include "Kernel\Message\MessageQueue.h"
#include "Kernel\Thread\Thread.h"	// for thread-related system calls
#include "Kernel\Scheduler\Scheduler.h"
#include "Kernel\Semaphore\Semaphore.h"	// for semaphore-related system calls
#include "Kernel\Interrupt\InterruptDispatcher.h"
#include "Initialize\Globals.h"


SystemCallDispatcher g_SystemCallDispatcher;



static DWORD SCSerialPutchar(void* pArg[])
{
	return putchar((int)pArg[0]);
}



static DWORD SCSerialPuts(void* pArg[])
{
	return puts((char*)pArg[0]);
}


static DWORD SCSwitch(void* pArg[])
{
	g_Processor->SwitchContext(
		(DWORD)pArg[0], (DWORD)pArg[1]);
	return 0L;
}


static DWORD SCNull(void* pArg[])
{
	return 0L;
}


SystemCallDispatcher::SystemCallDispatcher()
{
	m_pSystemCallTable=new SystemCallEntry[MAX_NO_SYSTEM_CALL];

	HookSystemCall(SC_CREATE_THREAD, SCCreateThread);
	HookSystemCall(SC_TERMINATE_THREAD, SCTerminateThread);

	HookSystemCall(SC_CREATE_SEMAPHORE, SCCreateSemaphore);
	HookSystemCall(SC_OPEN_SEMAPHORE, SCOpenSemaphore);
	HookSystemCall(SC_WAIT_SEMAPHORE, SCWaitSemaphore);
	HookSystemCall(SC_SIGNAL_SEMAPHORE, SCSignalSemaphore);

	HookSystemCall(SC_CREATE_MESSAGEQUEUE, SCCreateMessageQueue);
	HookSystemCall(SC_OPEN_MESSAGEQUEUE, SCOpenMessageQueue);
	HookSystemCall(SC_SEND_MESSAGE, SCSendMessage);
	HookSystemCall(SC_GET_MESSAGE, SCGetMessage);

	HookSystemCall(SC_CREATE_EVENT, SCCreateEvent);
	HookSystemCall(SC_OPEN_EVENT, SCOpenEvent);
	HookSystemCall(SC_SET_EVENT, SCSetEvent);
	HookSystemCall(SC_GET_EVENT, SCGetEvent);

	HookSystemCall(SC_HOOK_INTERRUPT, SCHookInterrupt);

	HookSystemCall(SC_MALLOC, SCMalloc);
	HookSystemCall(SC_FREE, SCFree);

	HookSystemCall(SC_SERIAL_PUTCHAR, SCSerialPutchar);
	HookSystemCall(SC_SERIAL_PUTS, SCSerialPuts);

	HookSystemCall(SC_SWITCH, SCSwitch);
	HookSystemCall(SC_NULL, SCNull);

}

SystemCallDispatcher::~SystemCallDispatcher()
{
	delete m_pSystemCallTable;
}

void SystemCallDispatcher::HookSystemCall(DWORD nSystemCallNo, SystemCallEntry pEntryFunction)
{
	assert(nSystemCallNo<MAX_NO_SYSTEM_CALL);
	m_pSystemCallTable[nSystemCallNo] = pEntryFunction;
}

void SystemCallDispatcher::Dispatch(DWORD nSystemCallNo, void* pArgumentList[])
{
	// Could add code to verify whether pArgumentList points to a valid
	// memory location.

//	printf("CS = %x\n", get_cs());
//	printf("DS = %x\n", get_ds());
//	printf("System Call Number = %08x, Argument Pointer = %08x\n",
//		nSystemCallNo, pArgumentList);
	(m_pSystemCallTable[nSystemCallNo])(pArgumentList);

}
