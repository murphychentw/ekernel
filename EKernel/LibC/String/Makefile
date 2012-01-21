
LibCStringPath = $(LibCPath)/String

LibCStringSrc = $(LibCStringPath)/bcopy.cc $(LibCStringPath)/bzero.cc \
	$(LibCStringPath)/memcmp.cc $(LibCStringPath)/strcmp.cc \
	$(LibCStringPath)/strcpy.cc $(LibCStringPath)/strlen.cc

ifeq ($(MAKECMDGOALS),Pentium)
	include $(LibCStringPath)/Pentium/Makefile
	LibCStringAsmSrc = $(LibCStringPentiumAsmSrc)
endif

