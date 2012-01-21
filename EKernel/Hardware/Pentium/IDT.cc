// File: IDT.cc
//
// 1998/12/03: (Murphy)
//	Implement a class interface for interrupt descriptor table.
//	Use codes from oskit-0.6.

#include "IDT.h"

void IDT::GateInit(const struct gate_init_entry *src)
{
	assert(m_gateIDT);

	// Initialize the trap/interrupt vectors from the trap_inittab.
	while (src->entrypoint)
	{
		fill_gate(&m_gateIDT[src->vector], src->entrypoint,
			      KERNEL_CS, src->type, 0);
		src++;
	}
}



void IDT::SetIDT(WORD nVector, DWORD nEntryPoint, WORD nType)
{
	assert(nVector<IDT_SIZE);

	fill_gate(&m_gateIDT[nVector], nEntryPoint, KERNEL_CS, nType, 0);
}
