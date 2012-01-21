// File: Intel8259InterruptController.cc

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
 1999/03/10: Created by Murphy Cheng-Che Chen
 1999/03/12: Murphy
	void Intel8259InterruptController::AcknowledgeIRQ(unsigned char nIRQ);
	Not only put 8259-related stuffs here, but also processor-related
		interrupt mechanism stuffs here.

*/



#include "Intel8259InterruptController.h"
#include "IO.h"
#include "proc_reg.h"
#include "LibC/assert/assert.h"



extern struct gate_init_entry *trap_inittab;



void Intel8259InterruptController::Initialize()
{
	/////////////////////// Initialize 8259-1. //////////////////////////

	// Init command 1: cascade and require 4th init BYTE
	outb_p(0x20, 0x11);

	// Init command 2: switch IRQ's to int 20h
	outb_p(0x21, 0x20);

	// Init command 3: slave controller on IRQ 2
	outb_p(0x21, 0x04);

	// Init command 4: normal EOI, nonbuffered, 80x86
	outb_p(0x21, 0x01);

	/////////////////////// Initialize 8259-2. //////////////////////////

	// Init command 1: cascade and require 4th init BYTE
	outb_p(0xA0, 0x11);

	// Init command 2: switch IRQ's to int 28h
	outb_p(0xA1, 0x28);

	// Init command 3: slave controller on IRQ 2
	outb_p(0xA1, 0x02);

	// Init command 4: normal EOI, nonbuffered, 80x86
	outb_p(0xA1, 0x01);

	// Acknowledge any bogus interrupts by setting the End Of Interrupt bit.
	outb_p(0x20, 0x20);
	outb_p(0xA0, 0x20);

	///////// Initialize the interrupt descriptor table for 386+ ///////
	m_IDT.Init();
	m_IDT.GateInit((struct gate_init_entry*)&trap_inittab);

	// Create a pseudo-descriptor describing the IDT.
	struct pseudo_descriptor pdesc;
	pdesc.limit = m_IDT.GetLimit();
	pdesc.linear_base = (DWORD)m_IDT.GetBase();

	// Load the IDT.
	set_idt(&pdesc);
}



void Intel8259InterruptController::EnableAll()
{
	outb(0x21, 0);
	outb(0xA1, 0);
}



void Intel8259InterruptController::DisableAll()
{
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);
}



// Enable the interrupt request nIRQ by clearing the appropriate bit in 8259.
void Intel8259InterruptController::EnableIRQ(unsigned char nIRQ)
{
	unsigned int flags;

	assert(nIRQ < 16);
	flags = get_eflags();
	cli();

	if (nIRQ < 8)
		outb(0x21, inb(0x21) & ~(1 << nIRQ));
	else {
		// Enable the cascade line on the master.
		outb(0x21, inb(0x21) & ~(1 << 2));

		nIRQ -= 8;
		outb(0xA1, inb(0xA1) & ~(1 << nIRQ));
	}

	set_eflags(flags);
}



// Disable the interrupt request nIRQ by setting the appropriate bit in 8259.
void Intel8259InterruptController::DisableIRQ(unsigned char nIRQ)
{
	unsigned int flags;

	assert(nIRQ < 16);
	flags = get_eflags();
	cli();

	if (nIRQ < 8)
		outb(0x21, inb(0x21) | (1 << nIRQ));
	else {
		nIRQ -= 8;
		outb(0xA1, inb(0xA1) | (1 << nIRQ));
	}

	set_eflags(flags);
}



void Intel8259InterruptController::AcknowledgeIRQ(unsigned char nIRQ)
{
	outb(0x20, 0x20);
	if ((nIRQ) >= 8)
		outb(0xA0, 0x20);
}


void Intel8259InterruptController::HookIRQ(WORD nIRQ, void *pEntryFunction)
{
	m_IDT.SetIDT(nIRQ, (DWORD)pEntryFunction, ACC_PL_K | ACC_INTR_GATE);
}
