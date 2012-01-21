// File: Event.cc

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
#include "ADT\Array.h"
#include "Initialize\Globals.h"
#include "Kernel\Thread\Thread.h"
#include "Kernel\Scheduler\Scheduler.h"
#include "LibC\stdio\stdio.h"
#include "LibC\String\String.h"



Event::Event() : m_pEventName(0)
{
}



Event::~Event()
{
	if(m_pEventName!=0)
	{
		free(m_pEventName);
	}
}



BOOL Event::SetEventName(const char *pString)
{
	if(pString!=0)
	{
		char *buf=(char*)malloc(strlen(pString)+1);
		if(buf==0)
			return FALSE;
		strcpy(buf, pString);
		m_pEventName=buf;
	}
	else
		m_pEventName=0;

	return TRUE;
}



DWORD Event::Get(DWORD nRequestedFlags, DWORD *pnActualFlags, DWORD nGetOption, BOOL bWait)
{
	// Invalid Option
	if( (nGetOption != EVENT_AND) && (nGetOption != EVENT_OR) )
		return EVENT_INVALID_OPTION;
	
	g_Scheduler->DisablePreemption();

	if( (GetFlags() & nRequestedFlags) && nGetOption == EVENT_AND)
	{
		*pnActualFlags = GetFlags();
		g_Scheduler->EnablePreemption();
	}
	else if ( (GetFlags() | nRequestedFlags) && nGetOption == EVENT_OR)
	{
		*pnActualFlags = GetFlags();
		g_Scheduler->EnablePreemption();
	}
	else
	{
		if(bWait==FALSE)
		{
			g_Scheduler->EnablePreemption();
			return EVENT_FAILED;
		}
		else
		{
			m_queueThread.AddTail(g_nCurrentThreadID);
			g_Scheduler->CheckOut(g_nCurrentThreadID);
			g_Scheduler->EnablePreemption();
			g_Scheduler->Yield();
			Get(nRequestedFlags, pnActualFlags, nGetOption, bWait);
		}
	}

	return EVENT_SUCCESS;
}



DWORD Event::Set(DWORD nFlags, DWORD nSetOption)
{
	g_Scheduler->DisablePreemption();

	if(nSetOption==EVENT_AND)
		SetFlags( nFlags & GetFlags());
	else if(nSetOption==EVENT_OR)
		SetFlags( nFlags | GetFlags());
	else
	{
		g_Scheduler->EnablePreemption();
		return EVENT_INVALID_OPTION;
	}

	while(m_queueThread.GetCount()>0)
	{
		DWORD nThreadID = m_queueThread.GetHead();
		m_queueThread.RemoveHead();
		g_Scheduler->CheckIn(nThreadID);
	}

	g_Scheduler->EnablePreemption();

	return EVENT_SUCCESS;
}
