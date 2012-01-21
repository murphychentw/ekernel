// File: InterruptDispatcher.h

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
1999/03/26: Created by Murphy Cheng-Che Chen
1999/04/08: Murphy
	Use events for notifying interrupt threads instead
	of messaging.
*/

#ifndef __EKERNEL_INTERRUPT_DISPATCHER_H_
#define __EKERNEL_INTERRUPT_DISPATCHER_H_



#include "Config.h"
#include "Types.h"
#include "ADT\Array.h"



class InterruptDispatcher {
protected:
	// Each entry in this table stores an event identifier
	// Whenever an interrupt is triggered, the corresponding
	// event will be set.
	Array<DWORD> m_InterruptTable;

public:
	InterruptDispatcher();
	~InterruptDispatcher();

	void HookInterrupt(DWORD nInterruptNo, DWORD nEventID);
	void Dispatch(DWORD nInterruptNo);
};



// Export functions ( Defined in HookInterrupt.cc )
extern DWORD SCHookInterrupt(void *pArg[]);

#endif
