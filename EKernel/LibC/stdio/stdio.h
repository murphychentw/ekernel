// File: stdio.h

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

#ifndef __EKERNEL_STDIO_H_INCLUDED__
#define __EKERNEL_STDIO_H_INCLUDED__

#include <stdarg.h>

int putchar(int c);
int puts(const char *s);
int vprintf(const char *fmt, va_list args);
int printf(const char *fmt, ...);
int vsprintf(char *s, const char *fmt, va_list args);
int vsnprintf(char *s, int size, const char *fmt, va_list args);
int sprintf(char *s, const char *fmt, ...);
int snprintf(char *s, int size, const char *fmt, ...);

#endif
