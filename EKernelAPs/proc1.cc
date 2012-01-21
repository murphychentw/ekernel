extern inline void cli() { asm volatile("cli"); }

typedef unsigned long dword;

#include "stdio.h"
#include "SystemCallLib.h"

char stack_thread1[1024];

void Thread1(void);

dword nSemaphoreID;
dword nMessageQueueID;

void main()
{

	dword nThreadID;

	printf("Enter main() of proc1\n");

	CreateSemaphore(&nSemaphoreID, "Semaphore1", 1);

	CreateThread(&nThreadID, "Thread1", Thread1, stack_thread1, 1024, true);

	dword ret=CreateMessageQueue(&nMessageQueueID, "MsgQ1", 10);

	printf("proc1 main thread create message queue ret = %x\n", ret);

	while(1)
	{
		dword nMessage;
		dword nParam1;
		dword nParam2;
		GetMessage(nMessageQueueID, &nMessage, &nParam1, &nParam2, true);
		SerialPutchar(nMessage);
		WaitSemaphore(nSemaphoreID, true);
		SerialPutchar('m');
		SerialPutchar('a');
		SerialPutchar('i');
		SerialPutchar('n');
		SerialPutchar('1');
		SerialPutchar('\n');
		SignalSemaphore(nSemaphoreID);
		for(int i=0;i<0x300000;i++);

	};
}

void Thread1(void)
{
//	printf("proc1 thread1 semaphore ID = %d\n", nSemaphoreID);

	char i='A';
	while(1)
	{
		SendMessage(nMessageQueueID, (dword)i, 0, 0, true);
		i++;
		if(i>'Z')
			i='A';
		WaitSemaphore(nSemaphoreID, true);
		SerialPutchar('t');
		SerialPutchar('h');
		SerialPutchar('r');
		SerialPutchar('d');
		SerialPutchar('1');
		SerialPutchar('\n');
		SignalSemaphore(nSemaphoreID);
		for(int i=0;i<0x300000;i++);
	};
}