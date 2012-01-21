
extern dword CreateThread(
	dword *pnThreadID,
	char *pThreadName,
	void (*pEntryFunction)(),
	void *pStackSpace,
	dword nStackSize,
	bool bAutoStart);
extern dword TerminateThread(dword nThreadID);
	
extern dword SerialPutchar(int c);
extern dword SerialPuts(char *s);

extern dword CreateSemaphore(dword *pnSemaphoreID, char *pSemaphoreName,
	dword nInitialCount);
extern dword OpenSemaphore(dword *pnSemaphoreID, char *pSemaphoreName);
extern dword WaitSemaphore(dword nSemaphoreID, bool bWait);
extern dword SignalSemaphore(dword nSemaphoreID);

extern dword CreateMessageQueue(dword *pnMessageQueueID,
	char *pMessageQueueName, dword nMessageQueueSize);
extern dword OpenMessageQueue(dword *pnMessageQueueID, char *pMessageQueueName);
extern dword SendMessage(dword nMessageQueueID, dword nMessage, dword nParam1,
	dword nParam2, bool bWait);
extern dword GetMessage(dword nMessageQueueID, dword *pnMessage,
	dword *pnParam1, dword *pnParam2, bool bWait);