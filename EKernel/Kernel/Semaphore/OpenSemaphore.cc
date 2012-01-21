// File: OpenSemaphore.cc

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
 1999/02/08: Created by Murphy Cheng-Che Chen
	OpenSemaphore()
*/

#include "Semaphore.h"
#include "Config.h"
#include "Initialize\Globals.h"
#include "LibC\string\string.h"



DWORD OpenSemaphore(DWORD *pnSemaphoreID, char *pSemaphoreName)
{
	// Check if the name exists
	for(int i=0; i<MAX_NO_SEMAPHORES; i++)
	{
		if(g_arraySemaphore->CheckValid(i)==TRUE)
		{
			if(strcmp( (g_arraySemaphore->GetAt(i))->GetSemaphoreName(),
				pSemaphoreName)==0)
			{
				*pnSemaphoreID = (g_arraySemaphore->GetAt(i))->GetSemaphoreID();
				return SEMAPHORE_SUCCESS;
			}
		}
	}

	return SEMAPHORE_NOT_EXIST;
}
