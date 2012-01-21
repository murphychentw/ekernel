// File: serial_output.cc

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
 1999/02/01: Created by Murphy Cheng-Che Chen
*/


#include "Hardware/Pentium/IO.h"
#include "Hardware/Pentium/proc_reg.h"
#include "Kernel/Scheduler/Scheduler.h"

static int serial_io_base=0x2f8;



int serial_getchar(void)
{
	int ch;

	cli();

	// Wait for a character to arrive.
	while (!(inb(serial_io_base + 5) & 0x01));

	// Grab it.
	ch = inb(serial_io_base + 0);

	sti();

	return ch;
}



void serial_putchar(int ch)
{
//	g_Scheduler->DisablePreemption();

	if (ch == '\n')
		serial_putchar('\r');

	// Wait for the transmit buffer to become available.
	while (!(inb(serial_io_base + 5) & 0x20));

	outb(serial_io_base + 0, ch);

//	g_Scheduler->EnablePreemption();
}



void serial_init(void)
{
	unsigned char dfr;

	dfr = 0x18;
	dfr |= 0x03;

	/* Initialize the serial port.  */
	outb(serial_io_base + 3, 0x80 | dfr);
	outb(serial_io_base + 0, 1);		// Baud : 115200
	outb(serial_io_base + 1, 0);
	outb(serial_io_base + 3, 0x03 | dfr);
	outb(serial_io_base + 1, 0x00);
	outb(serial_io_base + 4, 0x0b);

	// Clear all serial interrupts.
	inb(serial_io_base + 6);	// ID 0: read RS-232 status register
	inb(serial_io_base + 2);	// ID 1: read interrupt identification reg
	inb(serial_io_base + 0);	// ID 2: read receive buffer register
	inb(serial_io_base + 5);	// ID 3: read serialization status reg 

//	base_critical_leave();
}



int putchar(int c)
{
	serial_putchar(c);
	return c;
}
