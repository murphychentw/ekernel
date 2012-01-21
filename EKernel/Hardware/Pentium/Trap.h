/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University.
 * Copyright (c) 1994 The University of Utah and
 * the Center for Software Science (CSS).
 * All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * CARNEGIE MELLON, THE UNIVERSITY OF UTAH AND CSS ALLOW FREE USE OF
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
#ifndef _EKERNEL_TRAP_H_
#define _EKERNEL_TRAP_H_

/*
 * Hardware trap vectors for i386.
 */
#define	T_DIVIDE_ERROR		0
#define	T_DEBUG			1
#define	T_NMI			2		/* non-maskable interrupt */
#define	T_INT3			3		/* int 3 instruction */
#define	T_OVERFLOW		4		/* overflow test */
#define	T_OUT_OF_BOUNDS		5		/* bounds check */
#define	T_INVALID_OPCODE	6		/* invalid op code */
#define	T_NO_FPU		7		/* no floating point */
#define	T_DOUBLE_FAULT		8		/* double fault */
#define	T_FPU_FAULT		9
#define T_INVALID_TSS		10
#define	T_SEGMENT_NOT_PRESENT	11
#define	T_STACK_FAULT		12
#define	T_GENERAL_PROTECTION	13
#define	T_PAGE_FAULT		14
/*				15 */
#define	T_FLOATING_POINT_ERROR	16
#define	T_ALIGNMENT_CHECK	17
#define	T_MACHINE_CHECK		18

/*
 * Page-fault trap codes.
 */
#define	T_PF_PROT		0x1		/* protection violation */
#define	T_PF_WRITE		0x2		/* write access */
#define	T_PF_USER		0x4		/* from user state */


/*
 * This structure corresponds to the state of user registers
 * as saved upon kernel trap/interrupt entry.
 * As always, it is only a default implementation;
 * a well-optimized kernel will probably want to override it
 * with something that allows better optimization.
 */
struct trap_state
{
	/* Saved segment registers */
	unsigned int	gs;
	unsigned int	fs;
	unsigned int	es;
	unsigned int	ds;

	/* PUSHA register state frame */
	unsigned int	edi;
	unsigned int	esi;
	unsigned int	ebp;
	unsigned int	cr2;	/* we save cr2 over esp for page faults */
	unsigned int	ebx;
	unsigned int	edx;
	unsigned int	ecx;
	unsigned int	eax;

	/* Processor trap number, 0-31.  */
	unsigned int	trapno;

	/* Error code pushed by the processor, 0 if none.  */
	unsigned int	err;

	/* Processor state frame */
	unsigned int	eip;
	unsigned int	cs;
	unsigned int	eflags;
	unsigned int	esp;
	unsigned int	ss;

	/* Virtual 8086 segment registers */
	unsigned int	v86_es;
	unsigned int	v86_ds;
	unsigned int	v86_fs;
	unsigned int	v86_gs;
};

/*
 * The actual trap_state frame pushed by the processor
 * varies in size depending on where the trap came from.
 */
#define TR_KSIZE	((int)&((struct trap_state*)0)->esp)
#define TR_USIZE	((int)&((struct trap_state*)0)->v86_es)
#define TR_V86SIZE	sizeof(struct trap_state)


/*
 * This library routine initializes the trap vectors in the base IDT
 * to point to default trap handler entrypoints
 * which merely push the standard trap saved-state frame above
 * and call the default trap handler routine, 'trap_handler'.
 */
void trap_init(void);

/*
 * This gate initialization table is used by base_trap_init
 * to initialize the base IDT to the default trap entrypoint code,
 * which pushes the state frame described above
 * and calls the trap handler, below.
 */
extern struct gate_init_entry trap_inittab[];

/*
 * The default trap entrypoint code checks this function pointer
 * (after saving the trap_state frame described by the structure above),
 * and if non-null, calls the C function it points to.
 * The function can examine and modify the provided trap_state as appropriate.
 * If the trap handler function returns zero,
 * execution will be resumed or resterted with the final trap_state;
 * if the trap handler function returns nonzero,
 * the trap handler dumps the trap state to the console and panics.
 */
// extern int (*trap_handler)(struct trap_state *state);
extern int trap_handler(struct trap_state *state);


/*
 * This routine dumps the contents of the specified trap_state structure
 * to the console using printf.
 * It is normally called automatically from trap_dump_panic
 * when an unexpected trap occurs;
 * however, can be called at other times as well
 * (e.g., for debugging custom trap handlers).
 */
void trap_dump(const struct trap_state *st);

/*
 * This routine simply calls trap_dump to dump the trap state,
 * and then calls panic.
 * It is used by the default trap entrypoint code
 * when a trap occurs and there is no trap handler,
 * and by the UNEXPECTED_TRAP assembly language macro below.
 */
void trap_dump_panic(const struct trap_state *st);



#endif /* _EKERNEL_TRAP_H_ */
