// File: OpenMessageQueue.cc

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
1999/02/28: Created by Murphy
	DWORD OpenMessageQueue(DWORD *pnMessageQueueID, char *pMessageQueueName);
*/

#include "Config.h"
#include "Initialize\Globals.h"
#include "LibC\string\string.h"
#include "MessageQueue.h"



DWORD OpenMessageQueue(DWORD *pnMessageQueueID, char *pMessageQueueName)
{
	// Check if the name exists
	for(int i=0; i<MAX_NO_MESSAGEQUEUES; i++)
	{
		if(g_arrayMessageQueue->CheckValid(i)==TRUE)
		{
			if(strcmp( (g_arrayMessageQueue->GetAt(i))->GetMessageQueueName(),
				pMessageQueueName)==0)
			{
				*pnMessageQueueID = (g_arrayMessageQueue->GetAt(i))->GetMessageQueueID();
				return MESSAGEQUEUE_SUCCESS;
			}
		}
	}

	return MESSAGEQUEUE_NOT_EXIST;
}
