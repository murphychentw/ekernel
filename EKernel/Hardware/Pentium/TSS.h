// File: TSS.h

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
	Modified comment style.
	Modified type qualifier. From 'unsigned int' to 'DWORD',
		from 'unsigned short' to 'WORD'.

 1998/12/03: Murphy
	Design a class for tss.
	void Init();
	inline void SetGDTEntry(WORD no_tss, WORD no_gdt);
	inline WORD GetGDTEntry(WORD no_tss);

	inline void* GetBase(WORD no);
	inline DWORD GetLimit(WORD no);

	inline void SetESP0(WORD no, DWORD esp0);
	inline void SetSS0(WORD no, DWORD ss0);
	inline void SetIOBitMapOffset(WORD no);

 1998/12/25: Murphy
	Modify TSS class, let it represent a single Task State Segment,
		instead of a table of Task State Segments.
	Remove Init(), SetGDTEntry(), GetGDTEntry();
 1998/12/26: Murphy
	inline void SetCS(DWORD value);
	inline void SetDS(DWORD value);
	inline void SetES(DWORD value);
	inline void SetFS(DWORD value);
	inline void SetGS(DWORD value);
	inline void SetSS(DWORD value);
	inline void SetLDT(DWORD value);
	inline void SetCR3(DWORD value);
	inline void SetBackLink(DWORD value);
	inline void SetEFlags(DWORD value);
*/

#ifndef	_EKERNEL_TSS_H_
#define	_EKERNEL_TSS_H_

#include "types.h"
#include "libc/assert/assert.h"
#include "libc/string/string.h"


//	Intel x86 32-bit Task State Segment
typedef struct x86_tss {
	DWORD	back_link;	// segment number of previous task, if nested
	DWORD	esp0;		// stack pointer for ring 0
	DWORD	ss0;		// stack segment for ring 0
	DWORD	esp1;		// stack pointer for ring 1
	DWORD	ss1;		// stack segment for ring 1
	DWORD	esp2;		// stack pointer for ring 2
	DWORD	ss2;		// stack segment for ring 2
	DWORD	cr3;		// CR3 - page table directory physical address
	DWORD	eip;
	DWORD	eflags;
	DWORD	eax;
	DWORD	ecx;
	DWORD	edx;
	DWORD	ebx;
	DWORD	esp;		// current stack pointer
	DWORD	ebp;
	DWORD	esi;
	DWORD	edi;
	DWORD	es;
	DWORD	cs;
	DWORD	ss;		// current stack segment
	DWORD	ds;
	DWORD	fs;
	DWORD	gs;
	DWORD	ldt;			// local descriptor table segment
	WORD	trace_trap;		// trap on switch to this task
	WORD	io_bit_map_offset;	// offset to start of IO permission bit map
	BYTE	io_bit_map[8192];
};

class TSS {

private:
	x86_tss m_tss;

public:
	TSS() { memset(&m_tss, 0, sizeof(x86_tss)); memset(m_tss.io_bit_map, 0, 8192); }

	inline void* GetBase() const { return (void*)&m_tss; }
	inline DWORD GetLimit() const { return sizeof(x86_tss)-1; }

	inline void SetSS0(DWORD ss0) { m_tss.ss0=ss0; }
	inline void SetESP0(DWORD esp0) { m_tss.esp0=esp0; }

	inline void SetCS(DWORD value) { m_tss.cs=value; }
	inline void SetDS(DWORD value) { m_tss.ds=value; }
	inline void SetES(DWORD value) { m_tss.es=value; }
	inline void SetFS(DWORD value) { m_tss.fs=value; }
	inline void SetGS(DWORD value) { m_tss.gs=value; }
	inline void SetSS(DWORD value) { m_tss.ss=value; }

	inline void SetEIP(DWORD value) { m_tss.eip=value; }
	inline void SetESP(DWORD esp) { m_tss.esp=esp; }

	inline void SetLDT(DWORD value) { m_tss.ldt=value; }

	inline void SetCR3(DWORD value) { m_tss.cr3=value; }

	inline void SetBackLink(DWORD value) { m_tss.back_link=value; }

	inline void SetEFlags(DWORD value) { m_tss.eflags=value; }

	inline void SetIOBitMapOffset() { m_tss.io_bit_map_offset=sizeof(x86_tss)-8192; }
};

#endif	// _EKERNEL_TSS_H_
