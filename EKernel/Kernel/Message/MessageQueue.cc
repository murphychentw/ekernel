// File: MessageQueue.cc

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
	BOOL MessageQueue::SetMessageQueueName(const char *pString);
	DWORD MessageQueue::Send(DWORD nMessage, DWORD nParam1, DWORD nParam2,
		BOOL bWait);
	DWORD MessageQueue::Get(DWORD *pnMessage, DWORD *pnParam1, DWORD *pnParam2,
		BOOL bWait);
1999/03/02: Murphy
	Use array instead of list to store messages.
	Use two waiting list: m_queueGetThread , m_queueSendThread.
*/



#include "MessageQueue.h"
#include "Initialize\Globals.h"
#include "LibC\String\String.h"







MessageQueue::MessageQueue() : m_pMessageQueueName(0)
{
}



MessageQueue::~MessageQueue()
{
	if(m_pMessageQueueName!=0)
	{
		free(m_pMessageQueueName);
	}
}



BOOL MessageQueue::SetMessageQueueName(const char *pString)
{
	if(pString!=0)
	{
		char *buf=(char*)malloc(strlen(pString)+1);
		if(buf==0)
			return FALSE;
		strcpy(buf, pString);
		m_pMessageQueueName=buf;
	}
	else
		m_pMessageQueueName=0;

	return TRUE;
}



DWORD MessageQueue::Send(
	DWORD nMessage,
	DWORD nParam1,
	DWORD nParam2,
	BOOL bWait
)
{
	g_Scheduler->DisablePreemption();

//	printf("MessageQueue::Send()\n");

	if(m_queueMessage.IsFull()!=TRUE)
	{
		Message msg;
		msg.nMessage = nMessage;
		msg.nParam1 = nParam1;
		msg.nParam2 = nParam2;
		m_queueMessage.Add(msg);

		// If there is any thread waiting to receive message
		// from this message queue.
		if(m_queueGetThread.GetCount() > 0)
		{
			DWORD nThreadID = m_queueGetThread.GetHead();
			m_queueGetThread.RemoveHead();
			g_Scheduler->CheckIn(nThreadID);
		}

		g_Scheduler->EnablePreemption();

		return MESSAGEQUEUE_SUCCESS;

	}
	else
	{
		if(bWait==FALSE)
		{
			g_Scheduler->EnablePreemption();
			return MESSAGEQUEUE_FULL;
		}
		else
		{
			
			m_queueSendThread.AddTail(g_nCurrentThreadID);
			g_Scheduler->CheckOut(g_nCurrentThreadID);
			g_Scheduler->EnablePreemption();
			g_Scheduler->Yield();
			Send(nMessage, nParam1, nParam2, bWait);
		}
	}

	return MESSAGEQUEUE_SUCCESS;
}



DWORD MessageQueue::Get(
	DWORD *pnMessage,
	DWORD *pnParam1,
	DWORD *pnParam2,
	BOOL bWait
)
{
	g_Scheduler->DisablePreemption();

//	printf("MessageQueue::Get()\n");

	if(m_queueMessage.IsEmpty() != TRUE)
	{
		Message msg;
		m_queueMessage.Remove(msg);
		*pnMessage = msg.nMessage;
		*pnParam1 = msg.nParam1;
		*pnParam2 = msg.nParam2;

		// If there is any thread waiting to sending message
		// from this message queue.
		if(m_queueSendThread.GetCount() > 0)
		{
			DWORD nThreadID = m_queueSendThread.GetHead();
			m_queueSendThread.RemoveHead();
			g_Scheduler->CheckIn(nThreadID);
		}

		g_Scheduler->EnablePreemption();

		return MESSAGEQUEUE_SUCCESS;
	}
	else
	{
		if(bWait==FALSE)
		{
			g_Scheduler->EnablePreemption();
			return MESSAGEQUEUE_EMPTY;
		}
		else
		{
			m_queueGetThread.AddTail(g_nCurrentThreadID);
			g_Scheduler->CheckOut(g_nCurrentThreadID);
			g_Scheduler->EnablePreemption();
			g_Scheduler->Yield();

			Get(pnMessage, pnParam1, pnParam2, bWait);
		}
	}


	return MESSAGEQUEUE_SUCCESS;
}
