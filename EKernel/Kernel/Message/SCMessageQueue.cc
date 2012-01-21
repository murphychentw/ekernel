// File: SCMessageQueue.cc

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
1999/03/01: Murphy
	DWORD SCCreateMessageQueue(void *pArg[]);
	DWORD SCOpenMessageQueue(void *pArg[]);
	DWORD SCSendMessage(void *pArg[]);
	DWORD SCGetMessage(void *pArg[]);
1999/03/02: Murphy
	Add a new argument nMessageQueueSize when calling CreateMessageQueue
		in SCCreateMessageQueue.
*/


#include "Initialize/Globals.h"
#include "MessageQueue.h"



DWORD SCCreateMessageQueue(void *pArg[])
{
	DWORD *pnMessageQueueID = (DWORD*)pArg[0];
	char *pMessageQueueName = (char*)pArg[1];
	DWORD nMessageQueueSize = (DWORD)pArg[2];

	return CreateMessageQueue(pnMessageQueueID, 
		pMessageQueueName, nMessageQueueSize);
}



DWORD SCOpenMessageQueue(void *pArg[])
{
	DWORD *pnMessageQueueID = (DWORD*)pArg[0];
	char *pMessageQueueName = (char*)pArg[1];

	return OpenMessageQueue(pnMessageQueueID, pMessageQueueName);
}



DWORD SCSendMessage(void *pArg[])
{
	DWORD nMessageQueueID = (DWORD)pArg[0];
	DWORD nMessage = (DWORD)pArg[1];
	DWORD nParam1 = (DWORD)pArg[2];
	DWORD nParam2 = (DWORD)pArg[3];
	BOOL bWait = (BOOL)pArg[4];

	// Check the validity of the message queue ID

	return (g_arrayMessageQueue->GetAt(nMessageQueueID))->
		Send(nMessage, nParam1, nParam2, bWait);
}



DWORD SCGetMessage(void *pArg[])
{
	DWORD nMessageQueueID = (DWORD)pArg[0];
	DWORD *pnMessage = (DWORD*)pArg[1];
	DWORD *pnParam1 = (DWORD*)pArg[2];
	DWORD *pnParam2 = (DWORD*)pArg[3];
	BOOL bWait = (BOOL)pArg[4];

	// Check the validity of the message queue ID

	return (g_arrayMessageQueue->GetAt(nMessageQueueID))->
		Get(pnMessage, pnParam1, pnParam2, bWait);
}
