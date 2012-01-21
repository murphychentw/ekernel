; Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details
;
; Modified by Murphy Che-Cher Chen to be compiled by NASM.
;
; 1998/09/28: Modified by Murphy Che-Cher Chen to access external
;	C++ function instead of C function. ( Handle the name-
;	mangling problem )
;

;void *memcpy(void *_dest, const void *_src, size_t _n);
[GLOBAL _memcpy__FPvPCvUl]

[SECTION .text]

	align	4

_memcpy__FPvPCvUl:
	push	ebp
	mov	ebp, esp
	push	esi
	push	edi
	mov	edi, [ebp+8]
	mov	esi, [ebp+12]
	mov	ecx, [ebp+16]
	call	___dj_movedata
	pop	edi
	pop	esi
	mov	eax, [ebp+8]
	pop	ebp
	ret



; This routine moves ecx bytes from ds:esi to es:edi.  It clobbers
; eax, ecx, esi, edi, and eflags.  The memory ranges must not overlap,
; unless esi >= edi.

	align 4

___dj_movedata:
	cld
	cmp	ecx, 15
	jle	small_move
	jmp	mod_4_check
	
	; Transfer bytes until either esi or edi is aligned % 4
align_mod_4:
	movsb
	dec	ecx
mod_4_check:
	test	esi, 3
	jz	big_move
	test	edi, 3
	jnz	align_mod_4

big_move:
	mov	al, cl	 ; We will store leftover count in al
	shr	ecx, 2
	and	al, 3
	rep
	movsd

	; ecx known to be zero here, so insert the leftover count in al
	mov	cl, al
small_move:
	rep
	movsb
	ret

