// File: Event.h

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


#ifndef _EKERNEL_EVENT_H_
#define _EKERNEL_EVENT_H_



#include "Types.h"
#include "ADT/List.h"



#define	EVENT_SUCCESS		0x00000000
#define EVENT_MEMORY_SHORTAGE	0x00000001
#define EVENT_DUPLICATE_NAME	0x00000002
#define EVENT_NULL_NAME		0x00000003
#define EVENT_NOT_EXIST		0x00000004
#define EVENT_INVALID_OPTION	0x00000005
#define EVENT_FAILED		0x00000006

// Options for GetEvent and SetEvent
#define EVENT_AND		0x10000000
#define EVENT_OR		0x20000000

class Event
{
private:
	DWORD m_nEventID;
	char *m_pEventName;

	DWORD m_nFlags;
	
	List<DWORD> m_queueThread;

public:
	Event();
	~Event();

	inline void SetEventID(DWORD nID) { m_nEventID = nID; }
	inline DWORD GetEventID() const { return m_nEventID; }

	BOOL SetEventName(const char *pString);
	inline char* GetEventName() const { return m_pEventName; }

	inline void SetFlags(DWORD nFlags) { m_nFlags = nFlags; }
	inline BOOL GetFlags() { return m_nFlags; }

	DWORD Get(DWORD nRequestedFlags, DWORD *pnActualFlags, DWORD nGetOption, BOOL bWait);
	DWORD Set(DWORD nFlags, DWORD nSetOption);
};



// Export functions ( Defined in CreateEvent.cc )
extern DWORD CreateEvent(
	DWORD *pnEventID,
	char *pEventName,
	DWORD nInitialFlags);
extern DWORD OpenEvent(
	DWORD *pnEventID,
	char *pEventName);

extern DWORD SCCreateEvent(void *pArg[]);
extern DWORD SCOpenEvent(void *pArg[]);
extern DWORD SCGetEvent(void *pArg[]);
extern DWORD SCSetEvent(void *pArg[]);



#endif
