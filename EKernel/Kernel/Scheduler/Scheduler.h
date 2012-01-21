// File: Scheduler.h

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
	DWORD GetNextThread();
	void Schedule();
	BOOL CheckIn(DWORD nThreadID);
	BOOL CheckOut(DWORD nThreadID);
 1999/02/03: Murphy
	extern Scheduler g_Scheduler;
 1999/02/13: Murphy
	void DisablePreemption();
	void EnablePreemption();
 1999/02/28: Murphy
	void Yield();
 1999/03/02: Murphy
	void *m_pCurrentPointer;
	Update m_pCurrentPointer in GetNextThread(), CheckOut().
 1999/03/18: Murphy
 	Move global object g_Scheduler to Initialize\Globals.cc.
*/

#ifndef _EKERNEL_SCHEDULER_H_
#define _EKERNEL_SCHEDULER_H_



#include "Types.h"
#include "ADT/List.h"


class Scheduler
{
protected:
	List<DWORD> m_queueReady;

	DWORD m_nPreemptionCount;

	// Points to the node of the current thread in ready queue list.
	void *m_pCurrentThreadPointer;

	DWORD GetNextThread();

public:
	Scheduler();

	void Schedule();	// Must be called from timer interrupt routine only

	BOOL CheckIn(DWORD nThreadID);
	BOOL CheckOut(DWORD nThreadID);

	void DisablePreemption();
	void EnablePreemption();

	void Yield();
};



#endif
