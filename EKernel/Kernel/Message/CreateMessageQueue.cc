// File: CreateMessageQueue.cc

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
	DWORD CreateMessageQueue(DWORD *pnMessageQueueID, char *pMessageQueueName);
1999/03/02: Murphy
	Add a new argument nMessageQueueSize in CreateMessageQueue.
*/


#include "Config.h"
#include "Initialize\Globals.h"
#include "LibC\String\string.h"
#include "MessageQueue.h"



DWORD CreateMessageQueue(
	DWORD *pnMessageQueueID,
	char *pMessageQueueName,
	DWORD nMessageQueueSize)
{
	// Check whether the provided pointer falls in the region belong to the
	//	calling thread.

	if(pMessageQueueName==0)
		return MESSAGEQUEUE_NULL_NAME;

	MessageQueue *msg_queue = new MessageQueue();
	BOOL ret;

	// Check if the name already exists
	for(int i=0; i<MAX_NO_MESSAGEQUEUES; i++)
	{
		if(g_arrayMessageQueue->CheckValid(i)==TRUE)
		{
			if(strcmp( (g_arrayMessageQueue->GetAt(i))->GetMessageQueueName(),
				pMessageQueueName)==0)
				return MESSAGEQUEUE_DUPLICATE_NAME;
		}
	}

	// Set message queue name
	ret=msg_queue->SetMessageQueueName(pMessageQueueName);
	if(ret==FALSE)
	{
		delete msg_queue;
		return MESSAGEQUEUE_MEMORY_SHORTAGE;
	}

	// Set message queue size
	msg_queue->SetMessageQueueSize(nMessageQueueSize);

	// Decide the message queue ID
	int nMessageQueueID=g_arrayMessageQueue->FindEmptySlot();
	if(nMessageQueueID<0)
	{
		delete msg_queue;
		return MESSAGEQUEUE_MEMORY_SHORTAGE;
	}
	msg_queue->SetMessageQueueID(nMessageQueueID);

	// Add msg_queue to the global semaphore list.
	g_arrayMessageQueue->SetAt(nMessageQueueID, msg_queue);

	*pnMessageQueueID = nMessageQueueID;

	return MESSAGEQUEUE_SUCCESS;
}
