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
; 1998/12/27: Created by Murphy Che-Cher Chen
;	This file contains the entry point of the call gate for system call.
; 1999/01/06: Murphy
;	Call g_SystemCallDispatcher.Dispatch instead of ::SystemCallDispatcher.
;	Before user process calls this routine via a call gate, 
;	ecx and ebx should be setup as follows:
;	ebx : the index number of the intended system call
;	ecx : pointer to the argument list
; 1999/01/27: Murphy
; 	[EXTERN _Dispatch__20SystemCallDispatcherUlPv]
; -->	[EXTERN _Dispatch__20SystemCallDispatcherUlPPv]
; 1999/02/24: Murphy
;	Swith data segment from USER_DS to KERNEL_DS ( defined in GDT.h)
;	when processing system calls.

[GLOBAL _SystemCallStub__Fv]
[EXTERN _Dispatch__20SystemCallDispatcherUlPPv]
[EXTERN _g_SystemCallDispatcher]

[SECTION .text]
	align	4

_SystemCallStub__Fv:

		; Can enhance to add codes to make sure the stack will not
		; be corrupted by malicious user programs.

		push	dword 0x10
		pop	ds		; swith to kernel data segment

		push	ecx
		push	ebx

		push	dword _g_SystemCallDispatcher
		call	_Dispatch__20SystemCallDispatcherUlPPv
		pop	ebx

		pop	ebx
		pop	ecx

		push	dword 0x23
		pop	ds		; swith back to user-level data segment

		retf

