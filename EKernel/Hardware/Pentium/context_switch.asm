; File: context_switch.asm

; Copyright (c) 1998-1999 Murphy Cheng-Che Chen <murphychen@gmail.com>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with this program; if not, write to the Free Software
;  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



;
; 1998/09/28: Modified by Murphy Che-Cher Chen to access external
;	C++ function instead of C function. ( Handle the name-
;	mangling problem )
;
; 1998/12/25: (Murphy)
;	Context switch not only ESP and EFLAGS but also CR3
;	, CS, DS, ES, SS.
; 1998/12/26: (Murphy)
;	Use hardware supported task-state-segment and iretd for
;	context switching.

;void ctx_sw(void *old_stack_pointer, void *new_stack_pointer);
[GLOBAL _ctx_sw__Fv]

[SECTION .text]
	align	4

; void context_switch();
_ctx_sw__Fv:

		push	ebp
		mov	ebp, esp

		push	eax

		; set task nested flag
		pushf
		pop	eax
		or	eax, 0x4000
		push	eax
		popf

		; task switch
		iretd

		pop	eax

		pop	ebp
		ret


