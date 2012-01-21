; File: entry.asm
;
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
; Compiled using NASM.
;
; Startup code to create MultiBoot-compliant kernel images.
; This file must be linked at the beginning of the kernel image.
;
; 1998/09/28: Modified by Murphy Che-Cher Chen to access external
;	C++ function instead of C function. ( Handle the name-
;	mangling problem )
;
; 1999/01/06: Murphy
;	Setup _djgpp_first_ctor, _djgpp_last_ctor, _djgpp_first_dtor,
;	_djgpp_last_dtor from djgpp_first_ctor, djgpp_last_ctor,
;	djgpp_first_dtor, djgpp_last_dtor

[GLOBAL start]
[GLOBAL _start]

[GLOBAL _djgpp_first_ctor]
[GLOBAL _djgpp_last_ctor]
[GLOBAL _djgpp_first_dtor]
[GLOBAL _djgpp_last_dtor]

;extern void multiboot_main(vm_offset_t boot_info_pa)
[EXTERN _multiboot_main__FUi]

[EXTERN _edata]
[EXTERN _end]

[EXTERN _test_ts]

[EXTERN djgpp_first_ctor]
[EXTERN djgpp_last_ctor]
[EXTERN djgpp_first_dtor]
[EXTERN djgpp_last_dtor]

BASE_STACK_SIZE equ 4096

[BITS 32]

[SECTION .text]

start:
_start:

	jmp boot_entry

	align 4
multiboot_header:
	;multiboot_header
	dd 0x1badb002		; magic
	dd 0x00010001		; flags
	dd 0x00000000-0x1badb002-0x00010001		; checksum
	dd multiboot_header	; header address
	dd start		; load address
	dd _edata
	dd _end
	dd boot_entry

boot_entry:

	; Switch to our own stack. 
	mov	esp, base_stack_end

	; Reset EFLAGS to a known state.
	; This will also clear the DF flag for the string-clear below.
	push	dword 0
	popf

	; Clear uninitialized data. 
	; XXX this shouldn't be necessary, as the multiboot spec now says
	;   that the bootloader does this rather than the OS kernel.
	;   Remove when we're sure all boot loaders have been updated.
	cld
	mov	edi, _edata
	mov	ecx, _end
	sub	ecx, edi
	xor	eax, eax
	rep stosb

	; Setup global constructors and destructors array
	mov	eax, djgpp_first_ctor
	mov	[_djgpp_first_ctor], eax
	mov	eax, djgpp_last_ctor
	mov	[_djgpp_last_ctor], eax
	mov	eax, djgpp_first_dtor
	mov	[_djgpp_first_dtor], eax
	mov	eax, djgpp_last_dtor
	mov	[_djgpp_last_dtor], eax


	; Push the boot_info parameter for multiboot_main()
	; before ebx gets trashed by the initialization code in crti.h.
	push	ebx

	; Clear the base pointer so that stack backtraces will work.
	xor	ebp, ebp

	; Jump into C code.
	call	_multiboot_main__FUi

halt:
	jmp	halt

[SECTION .bss]

_djgpp_first_ctor	resb 4
_djgpp_last_ctor	resb 4
_djgpp_first_dtor	resb 4
_djgpp_last_dtor	resb 4


base_stack_start:
	resb BASE_STACK_SIZE
base_stack_end:
	resb 4
