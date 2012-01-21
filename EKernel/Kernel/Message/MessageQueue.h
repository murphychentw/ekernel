// File: MessageQueue.h

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
1999/02/28: Created by Murphy Cheng-Che Chen
1999/03/02: Murphy
	Use array instead of list to store messages.
	Use two waiting list: m_queueGetThread , m_queueSendThread.
*/

#ifndef __EKERNEL_MESSAGEQUEUE_H_
#define __EKERNEL_MESSAGEQUEUE_H_

#include "Types.h"
#include "ADT/List.h"
#include "ADT/Queue.h"
#include "Message.h"
#include "Kernel/Thread/Thread.h"


#define	MESSAGEQUEUE_SUCCESS			0x00000000
#define MESSAGEQUEUE_MEMORY_SHORTAGE		0x00000001
#define MESSAGEQUEUE_DUPLICATE_NAME		0x00000002
#define MESSAGEQUEUE_NULL_NAME			0x00000003
#define MESSAGEQUEUE_NOT_EXIST			0x00000004
#define MESSAGEQUEUE_BUSY			0x00000005
#define MESSAGEQUEUE_EMPTY			0x00000006
#define MESSAGEQUEUE_FULL			0x00000007


class MessageQueue
{
private:
	DWORD m_nMessageQueueID;
	char *m_pMessageQueueName;

	Queue<Message> m_queueMessage;

	List<DWORD> m_queueSendThread;	// threads waiting for sending messages
	List<DWORD> m_queueGetThread;	// threads waiting for getting messages
public:

	MessageQueue();
	~MessageQueue();

	inline void SetMessageQueueID(DWORD nID) { m_nMessageQueueID = nID; }
	inline DWORD GetMessageQueueID() const { return m_nMessageQueueID; }

	BOOL SetMessageQueueName(const char *pString);
	inline char* GetMessageQueueName() const { return m_pMessageQueueName; }

	inline void SetMessageQueueSize(DWORD nSize) { m_queueMessage.SetSize(nSize); }

	DWORD Send(DWORD nMessage, DWORD nParam1, DWORD nParam2, BOOL bWait);
	DWORD Get(DWORD *pnMessage, DWORD *pnParam1, DWORD *pnParam2, BOOL bWait);
};



// Export functions
// -------------------------------------------
// Defined in CreateMessageQueue.cc
extern DWORD CreateMessageQueue(
	DWORD *pnMessageQueueID,
	char *pMessageQueueName,
	DWORD nMessageQueueSize
);
// Defined in OpenMessageQueue.cc
extern DWORD OpenMessageQueue(
	DWORD *pnMessageQueueID,
	char *pMessageQueueName
);
// Defined in SCMessageQueue.cc
extern DWORD SCCreateMessageQueue(void *pArg[]);
extern DWORD SCOpenMessageQueue(void *pArg[]);
extern DWORD SCSendMessage(void *pArg[]);
extern DWORD SCGetMessage(void *pArg[]);



#endif
