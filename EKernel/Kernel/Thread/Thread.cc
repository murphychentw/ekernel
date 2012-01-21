// File: Thread.cc

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
 1998/10/14: Created by Murphy Cheng-Che Chen
 1999/02/04: Murphy
	Move g_list_thread and g_nCurrentThreadID from kernel_initialize.cc
	Thread::Thread()
	Thread::~Thread()
 1999/03/18: Murphy
 	Move global objects g_list_thread and g_nCurrentThreadID to
 		Initialize\Globals.h.
*/



#include "Thread.h"

#include "Config.h"
#include "Kernel\Process\Process.h"
#include "LibC\malloc\malloc.h"
#include "LibC\stdio\stdio.h"
#include "LibC\string\string.h"




Thread::Thread() : m_pThreadName(0)
{
}



Thread::~Thread()
{
	if(m_pThreadName!=0)
		free(m_pThreadName);
}



BOOL Thread::SetThreadName(const char *pString)
{
	if(pString!=0)
	{
		char *buf=(char*)malloc(strlen(pString)+1);
		if(buf==0)
			return FALSE;
		strcpy(buf, pString);
		m_pThreadName=buf;
	}
	else
		m_pThreadName=0;

	return TRUE;
}



