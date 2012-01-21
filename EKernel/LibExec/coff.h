// File: coff.h

/*
 * Copyright (c) 1998-1999 Murphy Cheng-Che Chen <murphychen@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

/*
 1998/12/08: Created by Murphy Cheng-Che Chen
	Reference: UNIX Internals - A Practical Approach, by Steve D Pate
	Addison-Wesley.
	5.2.2 COFF headers, 5.2.3 COFF sections
	COFFFileHeader, AOUTHeader, COFFSectionHeader
 1998/12/10: Murphy
	Reference: http://www.delorie.com/djgpp/doc/coff/
*/

#ifndef __EKERNEL_COFF_H_
#define __EKERNEL_COFF_H_

#include "Types.h"


#ifdef DJGPP
#define PACKATTR	__attribute__ ((packed))
#else
#define PACKATTR
#endif

// f_magic
#define I386MAGIC	0x014C

// f_flags
/*
F_RELFLG :
	If set, there is no relocation information in this file.
	This is usually clear for objects and set for executables.
F_EXEC :
	If set, all unresolved symbols have been resolved and the file
	may be considered executable.
F_LNNO :
	If set, all line number information has been removed from the
	file (or was never added in the first place).
F_LSYMS :
	If set, all the local symbols have been removed from the file
	(or were never added in the first place).
F_AR32WR :
	Indicates that the file is 32-bit little endian.
*/
#define F_RELFLG	0x0001
#define F_EXEC		0x0002
#define F_LNNO		0x0004
#define F_LSYMS		0x0008
#define F_AR32WR	0x0100



typedef struct {
	WORD	f_magic		PACKATTR;	// magic number
	WORD	f_nscns		PACKATTR;	// number of sections
	DWORD	f_timedat	PACKATTR;	// time & date stamp
	DWORD	f_symptr	PACKATTR;	// file pointer to symtab
	DWORD	f_nsyms		PACKATTR;	// number of symtab entries
	WORD	f_opthdr	PACKATTR;	// sizeof(optional hdr)
	WORD	f_flags		PACKATTR;	// flags
} COFFFileHeader;



typedef struct {
	WORD	magic		PACKATTR;	// type of file
	WORD	vstamp		PACKATTR;	// version stamp
	DWORD	tsize		PACKATTR;	// text size in BYTEs, padded to FW bdry
	DWORD	dsize		PACKATTR;	// initialized data     "    "
	DWORD	bsize		PACKATTR;	// uninitialized data   "    "
	DWORD	entry		PACKATTR;	// entry pointer
	DWORD	text_start	PACKATTR;	// base of text used for this file
	DWORD	data_start	PACKATTR;	// base of data used for this file
} AOUTHeader;



// s_flags
/*
STYP_TEXT :
	If set, indicates that this section contains only executable code.
STYP_DATA :
	If set, indicates that this section contains only initialized data.
STYP_BSS :
	If set, indicates that this section defines uninitialized data, and
	has no data stored in the coff file for it.
*/
#define STYP_TEXT	0x0020
#define STYP_DATA	0x0040
#define STYP_BSS	0x0080

typedef struct {
	char	s_name[8]	PACKATTR;	// section name
	DWORD	s_paddr		PACKATTR;	// physical address, aliased s_nlib
	DWORD	s_vaddr		PACKATTR;	// virtual address
	DWORD	s_size		PACKATTR;	// section size
	DWORD	s_scnptr	PACKATTR;	// file ptr to raw data for section
	DWORD	s_relptr	PACKATTR;	// file ptr to relocation
	DWORD	s_lnnoptr	PACKATTR;	// file ptr to line numbers
	WORD	s_nreloc	PACKATTR;	// number of relocation entries
	WORD	s_nlnno		PACKATTR;	// number of line number entries
	DWORD	s_flags		PACKATTR;	// flags
} COFFSectionHeader;

#endif
