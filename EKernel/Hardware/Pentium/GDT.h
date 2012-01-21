// File: GDT.h

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
 1998/12/02: Murphy
	Design a class interface for global descriptor table.
	Use codes from oskit-0.6.
	inline void* GetBase() const;
	inline WORD GetLimit() const;
	void FillDescriptor(WORD no, DWORD base, DWORD limit,
		BYTE access, BYTE sizebits);
 1998/12/03: Murphy
	void FillDescriptor(const TSS& tss, WORD no_tss);
 1998/12/06: Murphy
	void Init();
 1998/12/25: Murphy
	Remove FillDescriptor(const TSS& tss, WORD no_tss);
 1998/12/26: Murphy
	void ModifyDescriptorAccess(WORD no, BYTE access, DWORD nOption);
 1998/12/27: Murphy
	Define: typedef union gdt_entry
		Let the GDT be able to contain both code/data descriptors
		as well as call gate descriptors.
	void FillGate(WORD no, WORD selector, DWORD offset, BYTE access,
		BYTE WORD_count);
*/


#ifndef __EKERNEL_GDT_H_
#define __EKERNEL_GDT_H_

#include "seg.h"
#include "TSS.h"

#include "LibC/assert/assert.h"

#define KERNEL_CS		0x08	// Kernel's PL0 code segment
#define KERNEL_DS		0x10	// Kernel's PL0 data segment
#define USER_CS			0x1b	// user's PL3 code segment
#define USER_DS			0x23	// user's PL3 data segment
#define GDT_KERNEL_TSS		0x28	// TSS for processor initialization
#define GDT_V86_TSS		0x30	// TSS for switching into Virtual 8086 Mode
#define GDT_USER_LDT		0x3b
#define GDT_SYSTEM_CALL		0x43	// Call Gate for System Call
#define GDT_CONTEXT_SWITCH_TSS	0x48	// TSS for threads begin here

#define GDTSZ			8192

#define GDT_OR			0x01
#define GDT_AND			0x02

typedef union gdt_entry {
	x86_desc entryDesc;
	x86_gate entryGate;
};

class GDT {

private:
	gdt_entry *m_descGdt;

public:
	inline void Init() { m_descGdt = new gdt_entry[GDTSZ]; assert(m_descGdt); };

	inline void* GetBase() const { assert(m_descGdt); return (void*)m_descGdt; };
	inline WORD GetLimit() const { assert(m_descGdt); return (GDTSZ*8-1); };

	void FillDescriptor(WORD no, DWORD base, DWORD limit,
		BYTE access, BYTE sizebits);
	void ModifyDescriptorAccess(WORD no, BYTE access, DWORD nOption);

	void FillGate(WORD no, WORD selector, DWORD offset, BYTE access,
		BYTE WORD_count);
};

#endif
