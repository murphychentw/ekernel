// File: Initialize.cc

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
1999/03/05: Created by Murphy
	vm_offset_t GetAvailableMemory();
1999/03/12: Murphy
	Put all global variables here in this file.
1999/03/18: Murphy
	Move global objects and variabls to Globals.cc.
*/



#include "Initialize.h"
#include "Globals.h"
#include "LibC/stdio/stdio.h"

#include "Hardware/Pentium/PentiumProcessor.h"
#include "Hardware/Pentium/PentiumMMU.h"
#include "Hardware/Pentium/Intel8259InterruptController.h"




extern int PentiumMain();


vm_offset_t g_nAvailableMemory;


static void KernelInitialize(void);



void Initialize()
{
	// Initialize MMU ( Memory Management Unit ).
	g_MMU = new PentiumMMU();
	g_MMU->Initialize();
	g_MMU->EnablePaging();

	// Initialize Processor.
	g_Processor = new PentiumProcessor();
	g_Processor->Initialize();

	// Initialize InterruptController.
	g_InterruptController = new Intel8259InterruptController();
	g_InterruptController->Initialize();
	g_InterruptController->DisableAll();

	// Initialize kernel realted data structures
	KernelInitialize();

	// Invoke the main program.
	// Let user create threads there.
	PentiumMain();

	// Enable Interrupt Controller Timer IRQ
	// TO DO: Prepare ISR for each IRQ, and enable all IRQs here.
	g_InterruptController->EnableIRQ(0);
	g_InterruptController->EnableIRQ(4);
}



void KernelInitialize(void)
{
	// Initialize the array of pointer to class Process
	g_arrayProcess = new Array<Process*>();
	g_arrayProcess->SetSize(MAX_NO_PROCESSES);

	// Initialize the array of pointer to class Thread
	g_arrayThread = new Array<Thread*>();
	g_arrayThread->SetSize(MAX_NO_THREADS);

	// Initialize the array of pointer to class Event
	g_arrayEvent = new Array<Event*>();
	g_arrayEvent->SetSize(MAX_NO_EVENTS);

	// Initialize the array of pointer to class MessageQueue
	g_arrayMessageQueue = new Array<MessageQueue*>();
	g_arrayMessageQueue->SetSize(MAX_NO_MESSAGEQUEUES);

	// Initialize the array of pointer to class Mutex
	g_arrayMutex = new Array<Mutex*>();
	g_arrayMutex->SetSize(MAX_NO_MUTEXES);

	// Initialize the array of pointer to class Semaphore
	g_arraySemaphore = new Array<Semaphore*>();
	g_arraySemaphore->SetSize(MAX_NO_SEMAPHORES);


	g_InterruptDispatcher = new InterruptDispatcher();
	g_Scheduler = new Scheduler();

	g_nCurrentProcessID=0xffffffff;
	g_nCurrentThreadID=0xffffffff;
}



vm_offset_t GetAvailableMemory()
{
	return g_nAvailableMemory;
}
