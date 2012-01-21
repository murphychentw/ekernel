/* 
 * Copyright (c) 1996 The University of Utah and
 * the Computer Systems Laboratory at the University of Utah (CSL).
 * All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software is hereby
 * granted provided that (1) source code retains these copyright, permission,
 * and disclaimer notices, and (2) redistributions including binaries
 * reproduce the notices in supporting documentation, and (3) all advertising
 * materials mentioning features or use of this software display the following
 * acknowledgement: ``This product includes software developed by the
 * Computer Systems Laboratory at the University of Utah.''
 *
 * THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
 * IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
 * ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * CSL requests users of this software to return to csl-dist@cs.utah.edu any
 * improvements that they make and grant CSL redistribution rights.
 */

#include "Initialize/Globals.h"
#include "LibC/stdio/stdio.h"
#include "Trap.h"
#include "proc_reg.h"
#include "Types.h"



int trap_handler(struct trap_state *state)
{
	if(state->trapno>=0x20 && state->trapno<=0x2F)
	{
		g_InterruptDispatcher->Dispatch(state->trapno);
		return 0;
	}

	cli();
	trap_dump(state);
	printf("Halt!");
	while(1);
}

void trap_dump(const struct trap_state *st)
{
	int from_user = (st->cs & 3) || (st->eflags & EFL_VM);
	int i;

	printf("Current Thread ID = %x\n", g_nCurrentThreadID);
	printf("Current Process ID = %x\n", g_nCurrentProcessID);

	printf("Dump of trap_state at %p:\n", st);
	printf("EAX %08x EBX %08x ECX %08x EDX %08x\n",
		st->eax, st->ebx, st->ecx, st->edx);
	printf("ESI %08x EDI %08x EBP %08x ESP %08x\n",
		st->esi, st->edi, st->ebp,
		from_user ? st->esp : (unsigned)&st->esp);
	printf("EIP %08x EFLAGS %08x\n", st->eip, st->eflags);
	printf("CS %04x SS %04x DS %04x ES %04x FS %04x GS %04x\n",
		st->cs & 0xffff, from_user ? st->ss & 0xffff : get_ss(),
		st->ds & 0xffff, st->es & 0xffff,
		st->fs & 0xffff, st->gs & 0xffff);
	if (st->eflags & EFL_VM)
		printf("v86:            DS %04x ES %04x FS %04x GS %04x\n",
			st->v86_ds & 0xffff, st->v86_es & 0xffff,
			st->v86_gs & 0xffff, st->v86_gs & 0xffff);
	printf("trapno %d, error %08x, from %s mode\n",
		st->trapno, st->err, from_user ? "user" : "kernel");
	if (st->trapno == T_PAGE_FAULT)
		printf("page fault linear address %08x\n", st->cr2);

	/* Dump the top of the stack too.  */
	if (!from_user)
	{
		for (i = 0; i < 32; i++)
		{
			printf("%08x%c", (&st->esp)[i],
				((i & 7) == 7) ? '\n' : ' ');
		}
	}
}



void trap_dump_panic(const struct trap_state *st)
{
	cli();
	trap_dump(st);

//	panic("terminated due to trap\n");
	while(1);
}
