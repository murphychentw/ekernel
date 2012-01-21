// File: SCMutex.cc

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


#include "Mutex.h"
#include "Initialize\Globals.h"



DWORD SCCreateMutex(void *pArg[])
{
	DWORD *pnMutexID = (DWORD*)pArg[0];
	char *pMutexName = (char*)pArg[1];
	BOOL bInitialLock = (BOOL)pArg[2];

	return CreateMutex(pnMutexID, pMutexName, bInitialLock);
}



DWORD SCOpenMutex(void *pArg[])
{
	DWORD *pnMutexID = (DWORD*)pArg[0];
	char *pMutexName = (char*)pArg[1];

	return OpenMutex(pnMutexID, pMutexName);
}



DWORD SCWaitMutex(void *pArg[])
{
	DWORD nMutexID = (DWORD)pArg[0];
	BOOL bWait = (BOOL)pArg[1];

	// Check the validity of the mutex ID

	return (g_arrayMutex->GetAt(nMutexID))->Wait(bWait);
}



DWORD SCSignalMutex(void *pArg[])
{
	DWORD nMutexID = (DWORD)pArg[0];

	// Check the validity of the mutex ID

	return (g_arrayMutex->GetAt(nMutexID))->Signal();
}
