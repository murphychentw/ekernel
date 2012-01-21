// File: Intel8259InterruptController.h

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
*/



#ifndef __EKERNEL_HARDWARE_PENTIUM_INTEL8259INTERRUPTCONTROLLER_H
#define __EKERNEL_HARDWARE_PENTIUM_INTEL8259INTERRUPTCONTROLLER_H



#include "Hardware/InterruptController.h"
#include "IDT.h"


class Intel8259InterruptController : public InterruptController {
protected:
	IDT m_IDT;
public:
	void Initialize();

	void DisableAll();
	void EnableAll();

	void DisableIRQ(unsigned char nIRQ);
	void EnableIRQ(unsigned char nIRQ);

	void AcknowledgeIRQ(unsigned char nIRQ);

	void HookIRQ(WORD nIRQ, void *pEntryFunction);
};

#endif
