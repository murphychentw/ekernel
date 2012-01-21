// File: GDT.cc

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

#include "GDT.h"
#include "LibC/assert/assert.h"
#include "LibC/stdio/stdio.h"

void GDT::FillDescriptor(WORD no, DWORD base, DWORD limit,
		BYTE access, BYTE sizebits)
{
	assert(m_descGdt);
	assert(no<GDTSZ);

	if (limit > 0xfffff)
	{
		limit >>= 12;
		sizebits |= SZ_G;
	}
	
	m_descGdt[no].entryDesc.limit_low = limit & 0xffff;
	m_descGdt[no].entryDesc.base_low = base & 0xffff;
	m_descGdt[no].entryDesc.base_med = (base >> 16) & 0xff;
	m_descGdt[no].entryDesc.access = access | ACC_P;
	m_descGdt[no].entryDesc.limit_high = limit >> 16;
	m_descGdt[no].entryDesc.granularity = sizebits;
	m_descGdt[no].entryDesc.base_high = base >> 24;

//	printf("no=%x, base=%x, limit=%x\n", no, base, limit);
}

void GDT::ModifyDescriptorAccess(WORD no, BYTE access, DWORD nOption)
{
	assert(m_descGdt);
	assert(no<GDTSZ);

//	printf("ModifyDescriptorAccess()\n");

	BYTE result;

	switch(nOption) {
		case GDT_OR:
			result = access | m_descGdt[no].entryDesc.access;
			break;
		case GDT_AND:
			result = access & m_descGdt[no].entryDesc.access;
			break;
		default:
			assert(FALSE);
	}

//	printf("index=%08x result=%08x\n", no, result);

	m_descGdt[no].entryDesc.access=result;
}


void GDT::FillGate(WORD no, WORD selector, DWORD offset, BYTE access,
	BYTE WORD_count)
{
	assert(m_descGdt);
	assert(no<GDTSZ);

	m_descGdt[no].entryGate.selector = selector;
	m_descGdt[no].entryGate.offset_low = offset & 0xffff;
	m_descGdt[no].entryGate.offset_high = (offset >> 16) & 0xffff;
	m_descGdt[no].entryGate.access = access | ACC_P;
	m_descGdt[no].entryGate.WORD_count = WORD_count & 0x1f;

}
