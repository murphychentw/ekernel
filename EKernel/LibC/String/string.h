// File: string.h

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

#ifndef __EKERNEL_STRING_H_INCLUDED__
#define __EKERNEL_STRING_H_INCLUDED__



#include "Types.h"



void *memmove( void *dst, const void *src, SIZE length );
void *memset( void *dst, int c, SIZE length );
void *memcpy(void *_dest, const void *_src, SIZE _n);
int memcmp(const void *_s1, const void *_s2, SIZE _n);

void *bcopy( const void *src, void *dst, SIZE n );
void *bzero( void *dst, SIZE n );

int strcmp(const char *s1, const char *s2);
char *strcpy(char *_s1, const char *_s2);
SIZE strlen( const char *s );

int sprintf(char *s, const char *fmt, ...);


#endif /* __STRING_H_INCLUDED__ */
