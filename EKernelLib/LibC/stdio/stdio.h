// File: stdio.h

/*
 Copyright (c) 1998-1999 Murphy Cheng-Che Chen. All Rights Reserved.

 This software is provided "AS IS," without a warranty of any kind. ALL
 EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MURPHY CHENG-CHE CHEN AND HIS
 LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A
 RESULT OF USING, MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES.
 IN NO EVENT WILL MURPHY CHENG-CHE CHEN OR HIS LICENSORS BE LIABLE FOR ANY
 LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL,
 CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS
 OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE
 SOFTWARE, EVEN IF MURPHY CHENG-CHE CHEN HAS BEEN ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGES.

 murphy@hpc.ee.ntu.edu.tw
 http://hpc.ee.ntu.edu.tw/~murphy/
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
