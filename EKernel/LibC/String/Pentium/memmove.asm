; Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details
;
; Modified by Murphy Che-Cher Chen to be compiled by NASM.
;
; 1998/09/28: Modified by Murphy Che-Cher Chen to access external
;	C++ function instead of C function. ( Handle the name-
;	mangling problem )
;


;void *memmove( void *dst, const void *src, size_t length );
[GLOBAL _memmove__FPvPCvUl]

[SECTION .text]

	align	4

_memmove__FPvPCvUl:
	push	ebp
	mov	ebp, esp
	push	esi
	push	edi
	mov	edi, [ebp+8]
	mov	esi, [ebp+12]
	mov	ecx, [ebp+16]
	jecxz	L2
	cld
	cmp	edi,esi
	jb	L3

	std
	add	esi, ecx
	add	edi, ecx
	dec	esi
	dec	edi
L3:
	rep
	movsb

L2:
	cld
	mov	eax, [ebp+8]
	pop	edi
	pop	esi
	pop	ebp
	ret
