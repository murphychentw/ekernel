// File: TerminateThread.cc

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
 1999/01/27: Created by Murphy Cheng-Che Chen
 1999/01/31: Murphy
	Add code to remove the thread from the thread list of its own process.
	Add code to remove the thread from global thread list.
*/

#include "Thread.h"
#include "Initialize\Globals.h"
#include "Kernel\Process\Process.h"



DWORD TerminateThread(DWORD nThreadID, DWORD nProcessID)
{
	BOOL bResult;

	// Remove the thread from the thread list of its own process.
	bResult=(g_arrayProcess->GetAt(nProcessID))->RemoveThread(nThreadID);
	if(bResult==FALSE)
		return THREAD_INVALID_TID;

	// Remove the thread from global thread list.
	g_arrayThread->InvalidateAt(nThreadID);

	return THREAD_SUCCESS;
}
