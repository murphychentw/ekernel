; 
; Mach Operating System
; Copyright (c) 1993,1992,1991,1990 Carnegie Mellon University
; Copyright (c) 1991 IBM Corporation 
; All Rights Reserved.
; 
; Permission to use, copy, modify and distribute this software and its
; documentation is hereby granted, provided that both the copyright
; notice and this permission notice appear in all copies of the
; software, derivative works or modified versions, and any portions
; thereof, and that both notices appear in supporting documentation,
; and that the nema IBM not be used in advertising or publicity 
; pertaining to distribution of the software without specific, written
; prior permission.
; 
; CARNEGIE MELLON AND IBM ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS IS"
; CONDITION.  CARNEGIE MELLON AND IBM DISCLAIM ANY LIABILITY OF ANY KIND FOR
; ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
; 
; Carnegie Mellon requests users of this software to return to
; 
;  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
;  School of Computer Science
;  Carnegie Mellon University
;  Pittsburgh PA 15213-3890
; 
; any improvements or extensions that they make and grant Carnegie Mellon
; the rights to redistribute these changes.
;
;
; This file contains a 'gate_init' initialization table
; to initialize the x86 processor trap vectors to default entrypoints.
; These entrypoints simply push a standard trap_state frame
; and jump to the 'trap_handler' routine.
;
;
; 1998/09/28: Modified by Murphy Che-Cher Chen to access external
;	C++ function instead of C function. ( Handle the name-
;	mangling problem )
;
; 1999/02/02: Murphy
;	From 	"call	_scheduler__Fv"
;	To	"push	dword _g_Scheduler"
;		"call	_Schedule__9Scheduler"
;	for calling Scheduler::Schedule();
;	in timer interrupt handler.
;
; 1999/02/24: Murphy
;	Swith data segment from USER_DS to KERNEL_DS ( defined in GDT.h)
;	when processing timer interrupts.
;
; 1999/04/08: Murphy
;	From	"push	dword _g_Scheduler"
;	To	"push	dword [_g_Scheduler]"
;	because g_Scheduler is now declared as a pointer.

%define	ACC_INTR_GATE	0x0e			; interrupt gate
%define	ACC_TRAP_GATE	0x0f			; trap gate
%define	ACC_PL_K	0x00			; kernel access only
%define	ACC_PL_U	0x60			; user access


;int trap_handler(struct trap_state *state)
[EXTERN _trap_handler__FP10trap_state]

;void trap_dump_panic(const struct trap_state *st)
[EXTERN _trap_dump_panic__FPC10trap_state]

;void Scheduler::Schedule(void)
[EXTERN _Schedule__9Scheduler]
;Scheduler g_Scheduler
[EXTERN _g_Scheduler]

[EXTERN _g_InterruptDispatcher]
[EXTERN _Dispatch__19InterruptDispatcherUl]

[GLOBAL _trap_inittab]

[SECTION .text]
	align	4

t_zero_div:
	push	dword 0
	push	dword 0
	jmp	alltraps

t_debug:
	push	dword 0
	push	dword 1
	jmp	alltraps

t_int3:
	push	dword 0
	push	dword 3
	jmp	alltraps

t_into:
	push	dword 0
	push	dword 4
	jmp	alltraps

t_bounds:
	push	dword 0
	push	dword 5
	jmp	alltraps

t_invop:
	push	dword 0
	push	dword 6
	jmp	alltraps

t_nofpu:
	push	dword 0
	push	dword 7
	jmp	alltraps

a_dbl_fault:
	push	dword 0
	push	dword 8
	jmp	alltraps

a_fpu_over:
	push	dword 0
	push	dword 9
	jmp	alltraps

a_inv_tss:
	push	dword 0x0a
	jmp	alltraps

t_segnp:
	push	dword 0x0b
	jmp	alltraps

t_stack_fault:
	push	dword 0x0c
	jmp	alltraps

t_gen_prot:
	push	dword 0x0d
	jmp	alltraps

t_page_fault:
	push	dword 0x0e
	jmp	alltraps

t_trap_0f:
	push	dword 0
	push	dword 0x0f
	jmp	alltraps

t_fpu_err:
	push	dword 0
	push	dword 0x10
	jmp	alltraps

t_align_check:
	push	dword 0
	push	dword 0x11
	jmp	alltraps

t_machine_check:
	push	dword 0
	push	dword 0x12
	jmp	alltraps

t_int_timer:
	cli

	push	dword 0x10
	pop	ds		; swith to kernel data segment

	push	eax
	
	mov	al, 0x20
	out	0x20, al
	push	dword [_g_Scheduler]
	call	_Schedule__9Scheduler
	add	esp, 4
	
	pop	eax

	push	dword 0x23
	pop	ds		; swith back to user-level data segment

	;sti
	iretd

t_int_irq4:
	cli
	
	push	dword 0x24
	push	dword [_g_InterruptDispatcher]
	call	_Dispatch__19InterruptDispatcherUl
	add	esp, 8

	iretd

alltraps:

	; Save the rest of the state frame.
	; Note that on the 486 and up,
	; pusha is slower than the equivalent set of manual pushes,
	; so don't do this in your own code if you want speed.
	; For our purposes here we're more worried
	; about size and simplicity...

	pusha
	push	ds
	push	es
	push	fs
	push	gs

	; Load the kernel's segment registers.
	mov	ax, ss
	mov	ds, ax
	mov	es, ax

	; GCC likes the direction flag cleared.
	cld

	; Call the C handler function if one has been installed.
	mov	eax, _trap_handler__FP10trap_state
	or	eax, eax
	jz	unexpected
	push	esp
	call	eax
	pop	edx

	; If the handler function returned zero (success),
	; then resume execution as if the trap never happened.
	; Otherwise, just panic.
	or	eax, eax
	jnz	unexpected

	pop	gs
	pop	fs
	pop	es
	pop	ds
	popa
	add	esp, 4*2	; Pop trap number and error code
	iretd

unexpected:
	; Dump the register state and panic.
	mov	ax, ss
	mov	ds, ax
	mov	es, ax
	mov	eax, esp
	push	eax
	call	_trap_dump_panic__FPC10trap_state


[SECTION .data]

	align	4

_trap_inittab:

	; Divide Error, Fault
	dd t_zero_div
	dw 0x00
	dw ACC_PL_K | ACC_TRAP_GATE

	; Debug, Fault/Trap
	dd t_debug
	dw 0x01
	dw ACC_PL_K | ACC_TRAP_GATE

	; Breakpoint, Trap
	dd t_int3
	dw 0x03
	dw ACC_PL_U | ACC_TRAP_GATE	

	; Overflow, Trap
	dd t_into
	dw 0x04
	dw ACC_PL_U | ACC_TRAP_GATE	

	; BOUND Range Exceeded, Fault
	dd t_bounds
	dw 0x05
	dw ACC_PL_U | ACC_TRAP_GATE	

	; Invalid Opcode ( Undefined Opcode ), Fault
	dd t_invop
	dw 0x06
	dw ACC_PL_K | ACC_TRAP_GATE

	; Device Not Available ( No Math Coprocessor ), Fault
	dd t_nofpu
	dw 0x07
	dw ACC_PL_K | ACC_TRAP_GATE

	; Double Fault, About
	dd a_dbl_fault
	dw 0x08
	dw ACC_PL_K | ACC_TRAP_GATE

	; Coprocessor Segment Overrun ( reserved ), Fault
	dd a_fpu_over
	dw 0x09
	dw ACC_PL_K | ACC_TRAP_GATE

	; Invalid TSS, Fault
	dd a_inv_tss
	dw 0x0a
	dw ACC_PL_K | ACC_TRAP_GATE

	; Segment Not Present, Fault
	dd t_segnp
	dw 0x0b
	dw ACC_PL_K | ACC_TRAP_GATE

	; Stack Segment Fault, Fault
	dd t_stack_fault
	dw 0x0c
	dw ACC_PL_K | ACC_TRAP_GATE

	; General Protection, Fault
	dd t_gen_prot
	dw 0x0d
	dw ACC_PL_K | ACC_TRAP_GATE

	; Page Fault, Fault
	dd t_page_fault
	dw 0x0e
	dw ACC_PL_K | ACC_TRAP_GATE

	; ( Intel reserved. Do not use. )
	dd t_trap_0f
	dw 0x0f
	dw ACC_PL_K | ACC_TRAP_GATE

	; Floating-Point Error ( Math Fault ), Fault
	dd t_fpu_err
	dw 0x10
	dw ACC_PL_K | ACC_TRAP_GATE

	; Alignment Check, Fault
	dd t_align_check
	dw 0x11
	dw ACC_PL_K | ACC_TRAP_GATE

	; Machine Check, Abort
	dd t_machine_check
	dw 0x12
	dw ACC_PL_K | ACC_TRAP_GATE

	dd t_int_timer
	dw 0x20
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x21
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x22
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x23
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x24
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x25
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x26
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x27
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x28
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x29
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x2A
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x2B
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x2C
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x2D
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x2E
	dw ACC_PL_K | ACC_INTR_GATE

	dd t_int_irq4
	dw 0x2F
	dw ACC_PL_K | ACC_INTR_GATE

	dd 0

