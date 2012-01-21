// File: Globals.cc

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
*/

#include "Globals.h"



// Hardware-related (ENano)
MMU *g_MMU;
Processor *g_Processor;
InterruptController *g_InterruptController;



// Hardware-independent (EMicro)
Scheduler *g_Scheduler;
InterruptDispatcher *g_InterruptDispatcher;
MemoryManager g_MemoryManager;

Array<Thread*> *g_arrayThread;
Array<Process*> *g_arrayProcess;

// Interprocess Communication
Array<Event*> *g_arrayEvent;
Array<MessageQueue*> *g_arrayMessageQueue;
Array<Mutex*> *g_arrayMutex;
Array<Semaphore*> *g_arraySemaphore;


DWORD g_nCurrentProcessID;
DWORD g_nCurrentThreadID;
