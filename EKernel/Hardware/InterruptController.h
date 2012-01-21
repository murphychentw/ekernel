// File: InterruptController.h

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
*/



#ifndef __EKERNEL_HARDWARE_INTERRUPTCONTROLLER_H
#define __EKERNEL_HARDWARE_INTERRUPTCONTROLLER_H



#include "Types.h"



class InterruptController {

public:
	virtual void Initialize()=0;

	virtual void DisableAll()=0;
	virtual void EnableAll()=0;

	virtual void DisableIRQ(unsigned char nIRQ)=0;
	virtual void EnableIRQ(unsigned char nIRQ)=0;

	virtual void AcknowledgeIRQ(unsigned char nIRQ)=0;

	virtual void HookIRQ(WORD nIRQ, void *pEntryFunction)=0;
};

#endif
