
LibCPath = LibC

vpath %.h LibC;LibC/malloc;LibC/string;LibC/stdio;LibC/assert

include LibC/malloc/Makefile
include LibC/stdio/Makefile
include LibC/string/Makefile
include LibC/assert/Makefile

LibCCommonSrc =  $(LibCMallocSrc) $(LibCStdioSrc) $(LibCStringSrc) $(LibCAssertSrc)

LibCAsmSrc = $(LibCStringAsmSrc)

LibCSrc = $(LibCCommonSrc)

ifeq ($(MAKECMDGOALS),Pentium)
	LibCSrc = $(LibCCommonSrc) \
		LibC/djgpp/libgcc2.cc
endif 

ifeq ($(MAKECMDGOALS),StrongArm)
	LibCSrc = $(LibCCommonSrc)
endif

