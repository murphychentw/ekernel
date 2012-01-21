/* 
 * Copyright (c) 1996 The University of Utah and
 * the Computer Systems Laboratory at the University of Utah (CSL).
 * All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software is hereby
 * granted provided that (1) source code retains these copyright, permission,
 * and disclaimer notices, and (2) redistributions including binaries
 * reproduce the notices in supporting documentation, and (3) all advertising
 * materials mentioning features or use of this software display the following
 * acknowledgement: ``This product includes software developed by the
 * Computer Systems Laboratory at the University of Utah.''
 *
 * THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
 * IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
 * ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * CSL requests users of this software to return to csl-dist@cs.utah.edu any
 * improvements that they make and grant CSL redistribution rights.
 */
/*
 * Dump out the boot_info struct nicely.
 */

#include "multi_boot.h"
#include "LibC/stdio/stdio.h"

extern struct multiboot_info boot_info;

void multiboot_info_dump()
{
	struct multiboot_info *m = &boot_info;
	struct multiboot_module *mods;
	unsigned i;

	printf("Multiboot Info:\n");
	printf("  flags %b\n", m->flags, MULTIBOOT_FLAGS_FORMAT);

	if (m->flags & MULTIBOOT_MEMORY)
		printf("  memory %08x-%08x\n", m->mem_lower, m->mem_upper);

	if (m->flags & MULTIBOOT_BOOT_DEVICE)
		printf("  boot device {%x,%x,%x,%x}\n",
			m->boot_device[0],
		        m->boot_device[1],
		        m->boot_device[2],
		        m->boot_device[3]);

	if (m->flags & MULTIBOOT_CMDLINE)
		printf("  command line %s\n", (char *)m->cmdline);

	if (m->flags & MULTIBOOT_MODS) {
		mods = (struct multiboot_module *)m->mods_addr;
		printf("  %d boot modules\n", m->mods_count);
		for (i = 0; i < m->mods_count; i++)
			printf("    Boot module %d: \"%s\" %08x-%08x\n",
			       i, (char *)mods[i].string,
			       mods[i].mod_start,
			       mods[i].mod_end);
	}

	if (m->flags & MULTIBOOT_AOUT_SYMS)
		printf("  symtab at %08x, size %d; strtab at %08x, size %d\n",
		       m->syms.a.addr, m->syms.a.tabsize,
		       m->syms.a.addr + m->syms.a.tabsize, m->syms.a.strsize);

	if (m->flags & MULTIBOOT_ELF_SHDR)
		/* XXX */
		printf("  has MULTIBOOT_ELF_SHDR info\n");

	if (m->flags & MULTIBOOT_MEM_MAP)
		/* XXX */
		printf("  has MULTIBOOT_MEM_MAP\n");
}
