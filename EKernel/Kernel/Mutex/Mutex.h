// File: Mutex.h

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
 1999/03/18: Created by Murphy Cheng-Che Chen
*/


#ifndef _EKERNEL_MUTEX_H_
#define _EKERNEL_MUTEX_H_



#include "Types.h"
#include "ADT/List.h"



#define	MUTEX_SUCCESS		0x00000000
#define MUTEX_MEMORY_SHORTAGE	0x00000001
#define MUTEX_DUPLICATE_NAME	0x00000002
#define MUTEX_NULL_NAME		0x00000003
#define MUTEX_NOT_EXIST		0x00000004
#define MUTEX_BUSY		0x00000005


class Mutex
{
private:
	DWORD m_nMutexID;
	char *m_pMutexName;

	BOOL m_bLocked;
	List<DWORD> m_queueThread;

public:
	Mutex();
	~Mutex();

	inline void SetMutexID(DWORD nID) { m_nMutexID = nID; }
	inline DWORD GetMutexID() const { return m_nMutexID; }

	BOOL SetMutexName(const char *pString);
	inline char* GetMutexName() const { return m_pMutexName; }

	inline void SetLock(BOOL bValue) { m_bLocked = bValue; }
	inline BOOL GetLock() { return m_bLocked; }

	DWORD Wait(BOOL bWait);
	DWORD Signal();
};



// Export functions ( Defined in CreateMutex.cc )
extern DWORD CreateMutex(
	DWORD *pnMutexID,
	char *pMutexName,
	BOOL bInitialLock);
extern DWORD OpenMutex(
	DWORD *pnMutexID,
	char *pMutexName);

extern DWORD SCCreateMutex(void *pArg[]);
extern DWORD SCOpenMutex(void *pArg[]);
extern DWORD SCWaitMutex(void *pArg[]);
extern DWORD SCSignalMutex(void *pArg[]);



#endif
