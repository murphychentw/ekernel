// File: CreateEvent.cc

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
 1999/03/18: Created by Murphy Cheng-Che Chen
*/



#include "Event.h"
#include "Config.h"
#include "Initialize\Globals.h"
#include "LibC\String\String.h"



DWORD CreateEvent(
	DWORD *pnEventID,
	char *pEventName,
	DWORD nInitialFlags)
{
	// Check whether the provided pointer falls in the region belong to the
	//	calling thread.

	if(pEventName==0)
		return EVENT_NULL_NAME;

	Event *event = new Event();
	BOOL ret;

	// Check if the name already exists
	for(int i=0; i<MAX_NO_EVENTS; i++)
	{
		if(g_arrayEvent->CheckValid(i)==TRUE)
		{
			if(strcmp( (g_arrayEvent->GetAt(i))->GetEventName(),
				pEventName)==0)
				return EVENT_DUPLICATE_NAME;
		}
	}

	// Set event name
	ret=event->SetEventName(pEventName);
	if(ret==FALSE)
	{
		delete event;
		return EVENT_MEMORY_SHORTAGE;
	}

	// Set initial flags
	event->SetFlags(nInitialFlags);

	// Decide the event ID
	int nEventID=g_arrayEvent->FindEmptySlot();
	if(nEventID<0)
	{
		delete event;
		return EVENT_MEMORY_SHORTAGE;
	}
	event->SetEventID(nEventID);

	// Add the event to the global event list.
	g_arrayEvent->SetAt(nEventID, event);

	*pnEventID = nEventID;

	return EVENT_SUCCESS;
}
