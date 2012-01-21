// File: IDT.h
//
// 1998/12/03: (Murphy)
//	Design a class interface for interrupt descriptor table.
//	Use codes from oskit-0.6.
//	inline void* GetBase() const;
//	inline WORD GetLimit() const;
//	void GateInit(const struct gate_init_entry *src);
// 1998/12/07: (Murphy)
//	void Init();

// Copyright (c) 1995-1994 The University of Utah and
// the Computer Systems Laboratory at the University of Utah (CSL).
// All rights reserved.
//
// Permission to use, copy, modify and distribute this software is hereby
// granted provided that (1) source code retains these copyright, permission,
// and disclaimer notices, and (2) redistributions including binaries
// reproduce the notices in supporting documentation, and (3) all advertising
// materials mentioning features or use of this software display the following
// acknowledgement: ``This product includes software developed by the
// Computer Systems Laboratory at the University of Utah.''
//
// THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
// IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
// ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
//
// CSL requests users of this software to return to csl-dist@cs.utah.edu any
// improvements that they make and grant CSL redistribution rights.


#ifndef _EKERNEL_IDT_H_
#define _EKERNEL_IDT_H_

#include "GDT.h"
#include "seg.h"
#include "LibC/Assert/Assert.h"



#define IDT_SIZE 256



typedef struct gate_init_entry
{
	DWORD entrypoint __attribute__ ((packed));
	WORD vector __attribute__ ((packed));
	WORD type __attribute__ ((packed));
};



class IDT {

private:
	x86_gate *m_gateIDT;

public:
	inline void Init() { m_gateIDT = new x86_gate[IDT_SIZE]; assert(m_gateIDT); };

	inline void* GetBase() const { assert(m_gateIDT); return (void*)m_gateIDT; };
	inline WORD GetLimit() const { assert(m_gateIDT); return (IDT_SIZE*8-1); };

	void GateInit(const struct gate_init_entry *src);

	void SetIDT(WORD nVector, DWORD nEntryPoint, WORD nType);
};

#endif
