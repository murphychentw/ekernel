; File: SystemCallStub.asm
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
; 1999/01/04: Created by Murphy Che-Cher Chen
;	void SystemCallStub(dword nSystemCallNo, void *pArguments);
; 1999/01/27: Murphy
;	void SystemCallStub(dword nSystemCallNo, void *pArguments[]);

[GLOBAL _SystemCallStub__FUlPPv]

[SECTION .text]
	align	4

_SystemCallStub__FUlPPv:
		; Can enhance to add codes to make sure the stack will not
		; be corrupted by malicious user programs.
		push	ebp
		mov	ebp, esp

		push	ebx
		push	ecx

		mov	ebx, [ebp+8]
		mov	ecx, [ebp+12]

		call	0x43:0

		pop	ecx
		pop	ebx

		pop	ebp
		ret


