// File: SCSemaphore.cc

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
 1999/02/04: Created by Murphy Cheng-Che Chen
	DWORD SCCreateSemaphore(void *pArg[])
 1999/02/13: Murphy
	DWORD SCOpenSemaphore(void *pArg[])
	DWORD SCWaitSemaphore(void *pArg[])
	DWORD SCSignalSemaphore(void *pArg[])
*/


#include "Semaphore.h"
#include "Initialize\Globals.h"



DWORD SCCreateSemaphore(void *pArg[])
{
	DWORD *pnSemaphoreID = (DWORD*)pArg[0];
	char *pSemaphoreName = (char*)pArg[1];
	DWORD nInitialCount = (DWORD)pArg[2];

	return CreateSemaphore(pnSemaphoreID, pSemaphoreName, nInitialCount);
}



DWORD SCOpenSemaphore(void *pArg[])
{
	DWORD *pnSemaphoreID = (DWORD*)pArg[0];
	char *pSemaphoreName = (char*)pArg[1];

	return OpenSemaphore(pnSemaphoreID, pSemaphoreName);
}



DWORD SCWaitSemaphore(void *pArg[])
{
	DWORD nSemaphoreID = (DWORD)pArg[0];
	BOOL bWait = (BOOL)pArg[1];

	// Check the validity of the semaphore ID

	return (g_arraySemaphore->GetAt(nSemaphoreID))->Wait(bWait);
}



DWORD SCSignalSemaphore(void *pArg[])
{
	DWORD nSemaphoreID = (DWORD)pArg[0];

	// Check the validity of the semaphore ID

	return (g_arraySemaphore->GetAt(nSemaphoreID))->Signal();
}
