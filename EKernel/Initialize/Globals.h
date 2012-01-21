// File: Globals.h

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
 1999/03/18: Created by Murphy
	Contain global objects and variables.

 1999/04/08: Murphy
 	From	Scheduler g_Scheduler
 	To	Scheduler *g_Scheduler
 	From	Array<Process*> g_list_process
 	To	Array<Process*> *g_arrayProcess
 	...... etc.
 	To improve flexibility via inheritance.
*/

#ifndef __EKERNEL_GLOBALS_H_
#define __EKERNEL_GLOBALS_H_

#include "Types.h"
#include "ADT/Array.h"
#include "Config.h"
#include "Hardware/MMU.h"
#include "Hardware/Processor.h"
#include "Hardware/InterruptController.h"
#include "Kernel/Event/Event.h"
#include "Kernel/Interrupt/InterruptDispatcher.h"
#include "Kernel/Message/MessageQueue.h"
#include "Kernel/Mutex/Mutex.h"
#include "Kernel/Process/Process.h"
#include "Kernel/Scheduler/Scheduler.h"
#include "Kernel/Semaphore/Semaphore.h"
#include "Kernel/Thread/Thread.h"
#include "MemoryManager/MemoryManager.h"


extern MMU *g_MMU;
extern Processor *g_Processor;
extern InterruptController *g_InterruptController;



// Export variables
extern Scheduler *g_Scheduler;
extern InterruptDispatcher *g_InterruptDispatcher;
extern MemoryManager g_MemoryManager;
extern Array<Thread*> *g_arrayThread;
extern Array<Process*> *g_arrayProcess;
extern Array<Event*> *g_arrayEvent;
extern Array<MessageQueue*> *g_arrayMessageQueue;
extern Array<Mutex*> *g_arrayMutex;
extern Array<Semaphore*> *g_arraySemaphore;

extern DWORD g_nCurrentProcessID;
extern DWORD g_nCurrentThreadID;



#endif
