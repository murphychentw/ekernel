// File: SCEvent.cc

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
#include "Initialize\Globals.h"



DWORD SCCreateEvent(void *pArg[])
{
	DWORD *pnEventID = (DWORD*)pArg[0];
	char *pEventName = (char*)pArg[1];
	DWORD nInitialFlags = (DWORD)pArg[2];

	return CreateEvent(pnEventID, pEventName, nInitialFlags);
}



DWORD SCOpenEvent(void *pArg[])
{
	DWORD *pnEventID = (DWORD*)pArg[0];
	char *pEventName = (char*)pArg[1];

	return OpenEvent(pnEventID, pEventName);
}



DWORD SCGetEvent(void *pArg[])
{
	DWORD nEventID = (DWORD)pArg[0];
	DWORD nRequestedFlags = (DWORD)pArg[1];
	DWORD *pnActualFlags = (DWORD*)pArg[2];
	DWORD nGetOption = (DWORD)pArg[3];
	BOOL bWait = (BOOL)pArg[4];

	// Check the validity of the event ID

	return (g_arrayEvent->GetAt(nEventID))->Get(nRequestedFlags, pnActualFlags,
		nGetOption, bWait);
}



DWORD SCSetEvent(void *pArg[])
{
	DWORD nEventID = (DWORD)pArg[0];
	DWORD nFlags = (DWORD)pArg[1];
	DWORD nSetOption = (DWORD)pArg[2];

	// Check the validity of the event ID

	return (g_arrayEvent->GetAt(nEventID))->Set(nFlags, nSetOption);
}
