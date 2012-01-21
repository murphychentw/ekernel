// File: InterruptDispatcher.cc

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
*/



#include "InterruptDispatcher.h"
#include "Initialize\Globals.h"
#include "LibC\stdio\stdio.h"
#include "LibC\assert\assert.h"



InterruptDispatcher::InterruptDispatcher()
{
	m_InterruptTable.SetSize(MAX_NO_INTERRUPTS);
}

InterruptDispatcher::~InterruptDispatcher()
{
}

void InterruptDispatcher::HookInterrupt(
	DWORD nInterruptNo,
	DWORD nEventID)
{
	assert(nInterruptNo<MAX_NO_INTERRUPTS);
	
	// Verify the correctness of nMessageQueueID
	
	m_InterruptTable.SetAt(nInterruptNo, nEventID);
}

void InterruptDispatcher::Dispatch(DWORD nInterruptNo)
{
	assert(nInterruptNo<MAX_NO_INTERRUPTS);

	if(m_InterruptTable.CheckValid(nInterruptNo)==FALSE)
		return;

	DWORD nEventID = m_InterruptTable.GetAt(nInterruptNo);

	(g_arrayEvent->GetAt(nEventID))->Set(0xFFFFFFFF, EVENT_OR);

}
