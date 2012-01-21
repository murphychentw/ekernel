/* 
 * Copyright (c) 1995-1994 The University of Utah and
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


/*
 * Mach Operating System
 * Copyright (c) 1993-1989 Carnegie Mellon University.
 * Copyright (c) 1994 The University of Utah and
 * the Computer Systems Laboratory (CSL).
 * All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * CARNEGIE MELLON, THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF
 * THIS SOFTWARE IN ITS "AS IS" CONDITION, AND DISCLAIM ANY LIABILITY
 * OF ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF
 * THIS SOFTWARE.
 *
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */

#include <stdarg.h>

#include "types.h"
#include "libc/string/string.h"
#include "direct_console_output.h"


// ------------------------ direct_cons_putchar ---------------------------------

/* This is a special "feature" (read: kludge)
   intended for use only for kernel debugging.
   It enables an extremely simple console output mechanism
   that sends text straight to CGA/EGA/VGA video memory.
   It has the nice property of being functional right from the start,
   so it can be used to debug things that happen very early
   before any devices are initialized.  */

#define VIDBASE	((unsigned char *)(0xb8000))

void direct_cons_putchar(unsigned char c)
{
	static int ofs = -1;

	//	base_critical_enter();

	if (ofs < 0)
	{
		/* Called for the first time - initialize.  */
		ofs = 0;
		direct_cons_putchar('\n');
	}

	switch (c)
	{
	case '\n':
		bcopy(VIDBASE+80*2, VIDBASE, 80*2*24);
		bzero(VIDBASE+80*2*24, 80*2);
		/* fall through... */
	case '\r':

		ofs = 0;
		break;

	case '\t':
		ofs = (ofs + 8) & ~7;
		break;

	default:
		/* Wrap if we reach the end of a line.  */
		if (ofs >= 80) {
			direct_cons_putchar('\n');
		}

		/* Stuff the character into the video buffer. */
		{
			volatile unsigned char *p = VIDBASE + 80*2*24 + ofs*2;
			p[0] = c;
			p[1] = 0x0f;
			ofs++;
		}
		break;
	}

//	base_critical_leave();
}



// ---------------------------- putchar -------------------------------------


/*
 * This function defines our kernel "console device";
 * the calls to printf() below will resolve to calls to putchar().
 * Our implementation simply writes characters to the local PC display.
 * If printing a character at a time was too inefficient for some reason
 * (not the case here, but could be the case in other kernels),
 * we could also override the default puts() function,
 * which printf() uses to print entire lines of text.
 */
int putchar(int c)
{
	direct_cons_putchar(c);
	return c;
}

