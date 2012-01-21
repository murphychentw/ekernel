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
 * CPU identification code for x86 processors.
 */


#include <stdarg.h>
#include "proc_reg.h"
#include "cpuid.h"
#include "LibC/stdio/stdio.h"
#include "LibC/string/string.h"

static void get_cache_config(struct cpu_info *id)
{
	unsigned ci[4];
	unsigned cicount = 0;
	unsigned ccidx = 0;

	do
	{
		unsigned i;

		cicount++;
		asm volatile("
			cpuid
		" : "=a" (ci[0]), 
		    "=b" (ci[1]),
		    "=c" (ci[2]),
		    "=d" (ci[3])
		  : "a" (2));

		for (i = 0; i < 4; i++)
		{
			unsigned reg = ci[i];
			if ((reg & (1 << 31)) == 0)
			{
				/* The low BYTE of EAX isn't a descriptor.  */
				if (i == 0)
					reg >>= 8;
				while (reg != 0)
				{
					if ((reg & 0xff) &&
					    (ccidx < sizeof(id->cache_config)))
					{
						id->cache_config[ccidx++] =
							reg & 0xff;
					}
					reg >>= 8;
				}
			}
		}
	}
	while (cicount < (ci[0] & 0xff));
}

void cpuid(cpu_info *out_id)
{
	int orig_eflags = get_eflags();

	memset(out_id, 0, sizeof(*out_id));

	/* Check for a dumb old 386 by trying to toggle the AC flag.  */
	set_eflags(orig_eflags ^ EFL_AC);
	if ((get_eflags() ^ orig_eflags) & EFL_AC)
	{
		/* It's a 486 or better.  Now try toggling the ID flag.  */
		set_eflags(orig_eflags ^ EFL_ID);
		if ((get_eflags() ^ orig_eflags) & EFL_ID)
		{
			int highest_val;

			/*
			 * CPUID is supported, so use it.
			 * First get the vendor ID string.
			 */
			asm volatile("
				cpuid
			" : "=a" (highest_val),
			    "=b" (*((int*)(out_id->vendor_id+0))),
			    "=d" (*((int*)(out_id->vendor_id+4))),
			    "=c" (*((int*)(out_id->vendor_id+8)))
			  : "a" (0));

			/* Now the feature information.  */
			if (highest_val >= 1)
			{
				asm volatile("
					cpuid
				" : "=a" (*((int*)out_id)),
				    "=d" (out_id->feature_flags)
				  : "a" (1)
				  : "ebx", "ecx");
			}

			/* Cache and TLB information.  */
			if (highest_val >= 2)
				get_cache_config(out_id);
		}
		else
		{
			/* No CPUID support - it's an older 486.  */
			out_id->family = CPU_FAMILY_486;

			/* XXX detect FPU */
		}
	}
	else
	{
		out_id->family = CPU_FAMILY_386;

		/* XXX detect FPU */
	}

	set_eflags(orig_eflags);
}


// -----------------------------------------------------------------------
// cpu_info_format

void cpu_info_format(
	struct cpu_info *id,
	void (*formatter)(void *data, const char *fmt, ...),
	void *data)
{
	const static char *cputype[4] = {"0 (Original OEM processor)",
				   "1 (OverDrive processor)",
				   "2 (Dual processor)",
				   "3"};
	const static char *features[] = {
		"On-chip FPU",
		"Virtual 8086 mode enhancement",
		"I/O breakpoints",
		"4MB page size extensions",
		"Time stamp counter",
		"Model-specific registers",
		"36-bit physical address extension",
		"Machine check exception",
		"CMPXCHG8B instruction",
		"On-chip local APIC",
		0,
		0,
		"Memory type range registers",
		"Global bit in page table entries",
		"Machine check architecture",
		"CMOVcc instruction"};

	const char *company = "";
	const char *modelname = "";
	char fam[4];
	const char *family = fam;
	char vendorname[13];
	unsigned i;

	/*
	 * First try to guess a descriptive processor name
	 * based on the vendor ID, family, and model information.
	 * Much of this information comes from the x86 info file
	 * compiled by Christian Ludloff, ludloff@anet-dfw.com,
	 * http://webusers.anet-dfw.com/~ludloff.
	 */

	/* Default family string: #86 */
	fam[0] = '0' + id->family;
	fam[1] = '8';
	fam[2] = '6';
	fam[3] = 0;

	/* Check for specific vendors and models */
	if (memcmp(id->vendor_id, "GenuineIntel", 12) == 0)
	{
		static const char *m486[] =
			{"DX", "DX50", "SX", "DX/2", "SL",
			 "SX/2", "", "DX/2-WB", "DX/4", "DX/4-WB"};
		static const char *mp5[] =
			{" 60/66",
			 " 60/66",
			 " 75/90/100/120/133",
			 " P24T",
			 " OverDrive for Pentium-3.3v",
			 " OverDrive for iDX4",
			 " OverDrive for Pentium-5v"};
		static const char *mp6[] =
			{"", "", "", "", "OverDrive"};

		company = "Intel ";
		switch (id->family)
		{
		case 4:
			if (id->model < sizeof(m486)/sizeof(m486[0]))
				modelname = m486[id->model];
			break;
		case 5:
			family = "Pentium";
			if (id->model < sizeof(mp5)/sizeof(mp5[0]))
				modelname = mp5[id->model];
			break;
		case 6:
			family = "Pentium Pro";
			if (id->model < sizeof(mp6)/sizeof(mp6[0]))
				modelname = mp6[id->model];
			break;
		}
	}
	else if (memcmp(id->vendor_id, "UMC UMC UMC ", 12) == 0)
	{
		static const char *u486[] = {"", " U5D", " U5S"};

		company = "UMC ";
		switch (id->family)
		{
		case 4:
			if (id->model < sizeof(u486)/sizeof(u486[0]))
				modelname = u486[id->model];
			break;
		}
	}
	else if (memcmp(id->vendor_id, "AuthenticAMD", 12) == 0)
	{
		static const char *a486[] =
			{"", "", "", "DX2", "", "", "", "DX2WB",
			 "DX4", "DX4WB", "", "", "", "", "X5WT", "X5WB"};
		static const char *ak5[] = {" SSA5", " 5k86"};

		company = "AMD ";
		switch (id->family)
		{
		case 4:
			if (id->model < sizeof(a486)/sizeof(a486[0]))
				modelname = a486[id->model];
			break;
		case 5:
			family = "K5";
			if (id->model < sizeof(ak5)/sizeof(ak5[0]))
				modelname = ak5[id->model];
			break;
		}
	}
	else if (memcmp(id->vendor_id, "CyrixInstead", 12) == 0)
	{
		company = "Cyrix ";
		switch (id->family)
		{
		case 4:
			family = "5x86";
			break;
		case 5:
			family = "6x86";
			break;
		}
	}
	else if (memcmp(id->vendor_id, "NexGenDriven", 12) == 0)
	{
		company = "NexGen ";
		switch (id->family)
		{
		case 5:
			family = "Nx586";
			break;
		}
	}
	formatter(data, "Processor: %s%s%s\n", company, family, modelname);

	/*
	 * Now print the specific information
	 * exactly as provided by the processor.
	 */
	memcpy(vendorname, id->vendor_id, 12);
	vendorname[12] = 0;
	formatter(data, "Vendor ID: %s\n", vendorname);
	formatter(data, "Family: %c86\n", '0' + id->family);
	formatter(data, "Model: %d\n", id->model);
	formatter(data, "Stepping: %c\n", 'A' + id->stepping);
	formatter(data, "Type: %s\n", cputype[id->type]);

	/* Now the feature flags.  */
	for (i = 0; i < sizeof(features)/sizeof(features[0]); i++)
	{
		if (features[i])
			formatter(data, "%s: %s\n", features[i],
				  id->feature_flags & (1 << i) ? "yes" : "no");
	}


	/* Now the cache configuration information, if any.  */
	for (i = 0; i < sizeof(id->cache_config)/sizeof(id->cache_config[0]); i++)
	{
		char sbuf[40];
		const char *s = 0;

		switch (id->cache_config[i])
		{
		case 0: 
			break;
		case 0x01: s = "Instruction TLB: 4K-BYTE pages, "
				"4-way set associative, 64 entries"; break;
		case 0x02: s = "Instruction TLB: 4M-BYTE pages, "
				"4-way set associative, 4 entries"; break;
		case 0x03: s = "Data TLB: 4K-BYTE pages, "
				"4-way set associative, 64 entries"; break;
		case 0x04: s = "Data TLB: 4M-BYTE pages, "
				"4-way set associative, 8 entries"; break;
		case 0x06: s = "Instruction cache: 8K BYTEs, "
				"4-way set associative, 32 BYTE line size";
				break;
		case 0x0a: s = "Data cache: 8K BYTEs, "
				"2-way set associative, 32 BYTE line size";
				break;
		case 0x41: s = "Unified cache: 128K BYTEs, "
				"4-way set associative, 32 BYTE line size";
				break;
		case 0x42: s = "Unified cache: 256K BYTEs, "
				"4-way set associative, 32 BYTE line size";
				break;
		case 0x43: s = "Unified cache: 512K BYTEs, "
				"4-way set associative, 32 BYTE line size";
				break;
		default:
			sprintf(sbuf, "(unknown cache/TLB descriptor 0x%02x)",
				id->cache_config[i]);
			s = sbuf;
			break;
		}
		if (s)
			formatter(data, "%s\n", s);
	}
}


// ------------------------------------------------------------------------
// cpu_info_dump

static void formatter(void *data, const char *fmt, ...)
{
	va_list vl;

	va_start(vl, fmt);
	vprintf(fmt, vl);
	va_end(vl);
}

void cpu_info_dump(struct cpu_info *id)
{
	cpu_info_format(id, formatter, 0);
}

