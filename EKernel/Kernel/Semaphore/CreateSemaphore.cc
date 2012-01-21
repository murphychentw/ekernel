// File: CreateSemaphore.cc

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
	CreateSemaphore()
*/



#include "Semaphore.h"
#include "Config.h"
#include "Initialize\Globals.h"
#include "LibC\string\string.h"



DWORD CreateSemaphore(
	DWORD *pnSemaphoreID,
	char *pSemaphoreName,
	DWORD nInitialCount)
{
	// Check whether the provided pointer falls in the region belong to the
	//	calling thread.

	if(pSemaphoreName==0)
		return SEMAPHORE_NULL_NAME;

	Semaphore *semaphore = new Semaphore();
	BOOL ret;

	// Check if the name already exists
	for(int i=0; i<MAX_NO_SEMAPHORES; i++)
	{
		if(g_arraySemaphore->CheckValid(i)==TRUE)
		{
			if(strcmp( (g_arraySemaphore->GetAt(i))->GetSemaphoreName(),
				pSemaphoreName)==0)
				return SEMAPHORE_DUPLICATE_NAME;
		}
	}

	// Set semaphore name
	ret=semaphore->SetSemaphoreName(pSemaphoreName);
	if(ret==FALSE)
	{
		delete semaphore;
		return SEMAPHORE_MEMORY_SHORTAGE;
	}

	// Set initial count
	semaphore->SetCounter(nInitialCount);

	// Decide the semaphore ID
	int nSemaphoreID=g_arraySemaphore->FindEmptySlot();
	if(nSemaphoreID<0)
	{
		delete semaphore;
		return SEMAPHORE_MEMORY_SHORTAGE;
	}
	semaphore->SetSemaphoreID(nSemaphoreID);

	// Add the semaphore to the global semaphore list.
	g_arraySemaphore->SetAt(nSemaphoreID, semaphore);

	*pnSemaphoreID = nSemaphoreID;

	return SEMAPHORE_SUCCESS;
}
