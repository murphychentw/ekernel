; Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details 
;
; Modified by Murphy Che-Cher Chen to be compiled by NASM.
;
; 1998/09/28: Modified by Murphy Che-Cher Chen to access external
;	C++ function instead of C function. ( Handle the name-
;	mangling problem )
;

;void *memset( void *dst, int c, size_t length );
[GLOBAL _memset__FPviUl]


[SECTION .text]

	align	4

_memset__FPviUl:
	push	ebp
	mov	ebp, esp
	push	edi
	mov	edi, [ebp+8]
	mov	eax, [ebp+12]
	mov	ecx, [ebp+16]
	cld

	; We will handle memsets of <= 15 bytes one byte at a time.
	; This avoids some extra overhead for small memsets, and
	; knowing we are setting > 15 bytes eliminates some annoying
	; checks in the "long move" case.
	cmp	ecx, 15
	jle	L3

	; Otherwise, tile the byte value out into eax.
	; 0x41 -> 0x41414141, etc.
	mov	ah, al
	mov	edx, eax
	sal	eax, 16
	mov	ax, dx
	jmp	L2

	; Handle any cruft necessary to get %edi long-aligned.
L1:	stosb
	dec	ecx
L2:	test	edi, 3
	jnz	L1

	; Now slam out all of the longs.
	mov	edx, ecx
	shr	ecx, 2
	rep
	stosd

	; Finally, handle any trailing cruft.  We know the high three bytes
	; of ecx must be zero, so just put the "slop count" in the low byte.
	mov	cl, dl
	and	cl, 3
L3:	rep
	stosb
	mov	eax, [ebp+8]
	pop	edi
	pop	ebp
	ret
