typedef unsigned long dword;

#include "stdio.h"
#include "SystemCallLib.h"

dword nSemaphoreID;

void main()
{

	while(OpenSemaphore(&nSemaphoreID, "Semaphore1")!=0);

	//printf("proc2 main thread semaphore ID = %d\n", nSemaphoreID);

	while(1)
	{
		WaitSemaphore(nSemaphoreID, true);
		SerialPutchar('m');
		SerialPutchar('a');
		SerialPutchar('i');
		SerialPutchar('n');
		SerialPutchar('2');
		SerialPutchar('\n');
		SignalSemaphore(nSemaphoreID);
		for(int i=0;i<0x300000;i++);
	};
}
