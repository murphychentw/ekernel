// File: SCThread.cc

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
 1999/01/27: Created by Murphy Cheng-Che Chen
	SC stands for System Call
	DWORD SCCreateThread(void *pArg[])
 1999/02/01: Murphy
	DWORD SCTerminateThread(void *pArg[])
*/

#include "Thread.h"
#include "Initialize/Globals.h"
#include "Kernel/Process/Process.h"
#include "Hardware/Processor.h"
#include "Hardware/MMU.h"



DWORD SCCreateThread(void *pArg[])
{
	DWORD *pnThreadID=(DWORD*)pArg[0];
	char *pThreadName=(char*)pArg[1];
	void *pEntryFunction=(void*)pArg[2];
	void *pStackSpace=(void*)pArg[3];
	DWORD nStackSize=(DWORD)pArg[4];
	BOOL bAutoStart=(BOOL)pArg[5];

	void *pPageTableBase = g_MMU->GetPageTableBase(g_nCurrentProcessID);
	
	return CreateThread(pnThreadID, g_nCurrentProcessID, pThreadName,
		pEntryFunction, pStackSpace, nStackSize, pPageTableBase, bAutoStart);

}



DWORD SCTerminateThread(void *pArg[])
{
	DWORD nThreadID=(DWORD)pArg[0];

	return TerminateThread(nThreadID, g_nCurrentProcessID);
}
