// File: OpenEvent.cc

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



DWORD OpenEvent(DWORD *pnEventID, char *pEventName)
{
	// Check if the name exists
	for(int i=0; i<MAX_NO_EVENTS; i++)
	{
		if(g_arrayEvent->CheckValid(i)==TRUE)
		{
			if(strcmp( (g_arrayEvent->GetAt(i))->GetEventName(),
				pEventName)==0)
			{
				*pnEventID = (g_arrayEvent->GetAt(i))->GetEventID();
				return EVENT_SUCCESS;
			}
		}
	}

	return EVENT_NOT_EXIST;
}
