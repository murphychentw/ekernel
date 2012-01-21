// File: PentiumMain.cc

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

#include "Types.h"

#include "Initialize\Pentium\multi_boot.h"
#include "Kernel\Process\Process.h"
#include "Kernel\Thread\Thread.h"

#include "LibC\String\string.h"
#include "LibC\Stdio\stdio.h"


//#include "Hardware\Pentium\proc_reg.h"



int PentiumMain()
{
	struct multiboot_module *module;
	DWORD procID1, procID2;
	DWORD ret2;

	module=multiboot_find("/proc1.cof");
	if(module!=0)
	{
		ret2=CreateProcessFromImage(&procID1, "proc1", 0, 16384, 1024,
			(void*)module->mod_start);
	}

/*
	module=multiboot_find("/proc2.cof");
	if(module!=0)
	{
		ret2=CreateProcessFromImage(&procID2, "proc2", 0, 8192, 1024,
			(void*)module->mod_start);
	}

	cli();
	
	flush_instr_queue();
	
	// Context Switch
	g_Processor->SwitchContext(0xFFFFFFFF, 0);
*/	
	return 0;

}
