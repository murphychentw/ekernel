// File: SystemCall.h

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
	SC_SERIAL_PUTCHAR
	SC_SERIAL_PUTS
1999/02/13: Murphy
	Define semaphore related system numbers.
1999/03/01: Murphy
	Define messaging related system numbers.
1999/04/08: Murphy
	Define event related system numbers.
*/

#ifndef __EKERNEL_SYSTEM_CALL_H_
#define __EKERNEL_SYSTEM_CALL_H_


#define MAX_NO_SYSTEM_CALL	0x1000

#define SC_CREATE_THREAD	0x0000
#define SC_TERMINATE_THREAD	0x0001

#define SC_CREATE_SEMAPHORE	0x0110
#define SC_OPEN_SEMAPHORE	0x0111
#define SC_WAIT_SEMAPHORE	0x0112
#define SC_SIGNAL_SEMAPHORE	0x0113

#define	SC_CREATE_MESSAGEQUEUE	0x0120
#define	SC_OPEN_MESSAGEQUEUE	0x0121
#define	SC_SEND_MESSAGE		0x0122
#define	SC_GET_MESSAGE		0x0123

#define SC_CREATE_EVENT		0x0130
#define SC_OPEN_EVENT		0x0131
#define SC_SET_EVENT		0x0132
#define SC_GET_EVENT		0x0133

#define SC_HOOK_INTERRUPT	0x0500

#define SC_MALLOC		0x0600
#define SC_FREE			0x0601

#define SC_SERIAL_PUTCHAR	0x0F9E
#define SC_SERIAL_PUTS		0x0F9F

#define SC_SWITCH		0x0FFE
#define SC_NULL			0x0FFF

#endif
