/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */

#ifndef _EKERNEL_PIO_H_
#define _EKERNEL_PIO_H_

/* This is a more reliable delay than a few short jmps. */
#define iodelay() \
	asm volatile("pushl %eax; inb $0x80,%al; inb $0x80,%al; popl %eax")

#define inl(port) \
({ unsigned long _tmp__; \
	asm volatile("inl %1, %0" : "=a" (_tmp__) : "d" ((unsigned short)(port))); \
	_tmp__; })
#define inl_p(port) ({		\
	unsigned long r;	\
	r = inl(port);		\
	iodelay();		\
	r;			\
})

#define inw(port) \
({ unsigned short _tmp__; \
	asm volatile(".byte 0x66; inl %1, %0" : "=a" (_tmp__) : "d" ((unsigned short)(port))); \
	_tmp__; })
#define inw_p(port) ({		\
	unsigned short r;	\
	r = inw(port);		\
	iodelay();		\
	r;			\
})


// void inb(unsigned short port, unsigned char val);
#define inb(port) \
({ unsigned char _tmp__; \
	asm volatile("inb %1, %0" : "=a" (_tmp__) : "d" ((unsigned short)(port))); \
	_tmp__; })
#define inb_p(port) ({		\
	unsigned char r;	\
	r = inb(port);		\
	iodelay();		\
	r;			\
})


#define outl(port, val) \
({ asm volatile("outl %0, %1" : : "a" (val) , "d" ((unsigned short)(port))); })
#define outl_p(port, val) ({	\
	outl(port, val);	\
	iodelay();		\
})

#define outw(port, val) \
({asm volatile(".byte 0x66; outl %0, %1" : : "a" ((unsigned short)(val)) , "d" ((unsigned short)(port))); })
#define outw_p(port, val) ({	\
	outw(port, val);	\
	iodelay();		\
})


// void outb(unsigned short port, unsigned char val);
#define outb(port, val) \
({ asm volatile("outb %0, %1" : : "a" ((unsigned char)(val)) , "d" ((unsigned short)(port))); })
#define outb_p(port, val) ({	\
	outb(port, val);	\
	iodelay();		\
})


#endif /* _EKERNEL_PIO_H_ */
