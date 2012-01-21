// File: SystemCall.cc

/*
 Copyright (c) 1998-1999 Murphy Cheng-Che Chen. All Rights Reserved.

 This software is provided "AS IS," without a warranty of any kind. ALL
 EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MURPHY CHENG-CHE CHEN AND HIS
 LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A
 RESULT OF USING, MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES.
 IN NO EVENT WILL MURPHY CHENG-CHE CHEN OR HIS LICENSORS BE LIABLE FOR ANY
 LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL,
 CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS
 OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE
 SOFTWARE, EVEN IF MURPHY CHENG-CHE CHEN HAS BEEN ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGES.

 murphy@hpc.ee.ntu.edu.tw
 http://hpc.ee.ntu.edu.tw/~murphy/
*/

/*
1999/02/02: Created by Murphy Che-Cher Chen
	dword SerialPutchar(int c)
	dword SerialPuts(char *s)
1999/02/13: Murphy
	Add CreateSemaphore, OpenSemaphore, WaitSemaphore, SignalSemaphore
1999/03/01: Murphy
	Add CreateMessageQueue, OpenMessageQueue, SendMessage, GetMessage
*/

#include "Types.h"
#include "SystemCall.h"

extern void SystemCallStub(dword nSystemCallNo, void *pArguments[]);



dword CreateThread(
	dword *pnThreadID,
	char *pThreadName,
	void (*pEntryFunction)(),
	void *pStackSpace,
	dword nStackSize,
	bool bAutoStart)
{
	void *ptr[6];

	ptr[0]=(void*)pnThreadID;
	ptr[1]=(void*)pThreadName;
	ptr[2]=(void*)pEntryFunction;
	ptr[3]=(void*)pStackSpace;
	ptr[4]=(void*)nStackSize;
	ptr[5]=(void*)bAutoStart;

	SystemCallStub(SC_CREATE_THREAD, ptr);
}



dword TerminateThread(dword nThreadID)
{
	void *ptr[1];

	SystemCallStub(SC_TERMINATE_THREAD, ptr);
}




dword SerialPutchar(int c)
{
	void *ptr[1];

	ptr[0]=(void*)c;

	SystemCallStub(SC_SERIAL_PUTCHAR, ptr);
}



dword SerialPuts(char *s)
{
	void *ptr[1];

	ptr[0]=(void*)s;

	SystemCallStub(SC_SERIAL_PUTS, ptr);
}



dword CreateSemaphore(
	dword *pnSemaphoreID,
	char *pSemaphoreName,
	dword nInitialCount)
{
	void *ptr[3];

	ptr[0]=(void*)pnSemaphoreID;
	ptr[1]=(void*)pSemaphoreName;
	ptr[2]=(void*)nInitialCount;

	SystemCallStub(SC_CREATE_SEMAPHORE, ptr);
}



dword OpenSemaphore(
	dword *pnSemaphoreID,
	char *pSemaphoreName)
{
	void *ptr[2];

	ptr[0]=(void*)pnSemaphoreID;
	ptr[1]=(void*)pSemaphoreName;

	SystemCallStub(SC_OPEN_SEMAPHORE, ptr);
}



dword WaitSemaphore(
	dword nSemaphoreID,
	bool bWait)
{
	void *ptr[2];

	ptr[0]=(void*)nSemaphoreID;
	ptr[1]=(void*)bWait;

	SystemCallStub(SC_WAIT_SEMAPHORE, ptr);
}



dword SignalSemaphore(dword nSemaphoreID)
{
	void *ptr[1];

	ptr[0]=(void*)nSemaphoreID;
	
	SystemCallStub(SC_SIGNAL_SEMAPHORE, ptr);
}



dword CreateMessageQueue(
	dword *pnMessageQueueID,
	char *pMessageQueueName,
	dword nMessageQueueSize)
{
	void *ptr[3];

	ptr[0]=(void*)pnMessageQueueID;
	ptr[1]=(void*)pMessageQueueName;
	ptr[2]=(void*)nMessageQueueSize;

	SystemCallStub(SC_CREATE_MESSAGEQUEUE, ptr);
}



dword OpenMessageQueue(
	dword *pnMessageQueueID,
	char *pMessageQueueName)
{
	void *ptr[2];

	ptr[0]=(void*)pnMessageQueueID;
	ptr[1]=(void*)pMessageQueueName;

	SystemCallStub(SC_OPEN_MESSAGEQUEUE, ptr);
}



dword SendMessage(
	dword nMessageQueueID,
	dword nMessage,
	dword nParam1,
	dword nParam2,
	bool bWait)
{
	void *ptr[5];

	ptr[0]=(void*)nMessageQueueID;
	ptr[1]=(void*)nMessage;
	ptr[2]=(void*)nParam1;
	ptr[3]=(void*)nParam2;
	ptr[4]=(void*)bWait;

	SystemCallStub(SC_SEND_MESSAGE, ptr);
}



dword GetMessage(
	dword nMessageQueueID,
	dword *pnMessage,
	dword *pnParam1,
	dword *pnParam2,
	bool bWait)
{
	void *ptr[5];

	ptr[0]=(void*)nMessageQueueID;
	ptr[1]=(void*)pnMessage;
	ptr[2]=(void*)pnParam1;
	ptr[3]=(void*)pnParam2;
	ptr[4]=(void*)bWait;

	SystemCallStub(SC_GET_MESSAGE, ptr);
}
