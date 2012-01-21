// File: Semaphore.h

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
1999/02/02: Created by Murphy Cheng-Che Chen
	DWORD Wait(BOOL bWait);
	DWORD Signal();
1999/02/04: Murphy
	Semaphore();
	~Semaphore();
	void SetSemaphoreID(DWORD nID);
	DWORD GetSemaphoreID();
	BOOL SetSemaphoreName(const char *pString);
	char* GetSemaphoreName();
	void SetCounter(DWORD nValue);
	DWORD GetCounter();
*/


#ifndef _EKERNEL_SEMAPHORE_H_
#define _EKERNEL_SEMAPHORE_H_



#include "Types.h"
#include "ADT/List.h"



#define	SEMAPHORE_SUCCESS		0x00000000
#define SEMAPHORE_MEMORY_SHORTAGE	0x00000001
#define SEMAPHORE_DUPLICATE_NAME	0x00000002
#define SEMAPHORE_NULL_NAME		0x00000003
#define SEMAPHORE_NOT_EXIST		0x00000004
#define SEMAPHORE_BUSY			0x00000005


class Semaphore
{
private:
	DWORD m_nSemaphoreID;
	char *m_pSemaphoreName;

	DWORD m_nCounter;
	List<DWORD> m_queueThread;

public:
	Semaphore();
	~Semaphore();

	inline void SetSemaphoreID(DWORD nID) { m_nSemaphoreID = nID; }
	inline DWORD GetSemaphoreID() const { return m_nSemaphoreID; }

	BOOL SetSemaphoreName(const char *pString);
	inline char* GetSemaphoreName() const { return m_pSemaphoreName; }

	inline void SetCounter(DWORD nValue) { m_nCounter = nValue; }
	inline DWORD GetCounter() { return m_nCounter; }

	DWORD Wait(BOOL bWait);
	DWORD Signal();
};



// Export functions ( Defined in CreateSemaphore.cc )
extern DWORD CreateSemaphore(
	DWORD *pnSemaphoreID,
	char *pSemaphoreName,
	DWORD nInitialCount);
extern DWORD OpenSemaphore(
	DWORD *pnSemaphoreID,
	char *pSemaphoreName);

extern DWORD SCCreateSemaphore(void *pArg[]);
extern DWORD SCOpenSemaphore(void *pArg[]);
extern DWORD SCWaitSemaphore(void *pArg[]);
extern DWORD SCSignalSemaphore(void *pArg[]);



#endif
