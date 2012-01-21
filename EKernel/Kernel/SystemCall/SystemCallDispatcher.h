// File: SystemCallDispatcher.h

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

/*---------------------------------------------------------------------------
1998/12/27: Created by Murphy Cheng-Che Chen
1999/01/06: Murphy
	typedef SystemCallEntry;
	class SystemCallDispatcher;
1999/01/27: Murphy
	typedef DWORD (*SystemCallEntry)(void*[]);
1999/02/01: Murphy
	SC_CREATE_THREAD

---------------------------------------------------------------------------*/

#ifndef __EKERNEL_SYSTEM_CALL_DISPATCHER_H_
#define __EKERNEL_SYSTEM_CALL_DISPATCHER_H_



#include "config.h"
#include "types.h"

#include "SystemCall.h"


typedef DWORD (*SystemCallEntry)(void*[]);



class SystemCallDispatcher {
private:
	SystemCallEntry *m_pSystemCallTable;
public:
	SystemCallDispatcher();
	~SystemCallDispatcher();

	void HookSystemCall(DWORD nSystemCallNo, SystemCallEntry pEntryFunction);
	void Dispatch(DWORD nSystemCallNo, void* pArgumentList[]);
};

#endif
