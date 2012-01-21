
InitializePath = Initialize

ifeq ($(MAKECMDGOALS),Pentium)
	include $(InitializePath)/Pentium/Makefile
	InitializeSrc = $(InitializePath)/Initialize.cc \
		$(InitializePath)/Globals.cc \
		$(InitializePentiumSrc)
	InitializeAsmSrc = $(InitializePentiumAsmSrc)
endif

ifeq ($(MAKECMDGOALS),clean)
	include $(InitializePath)/Pentium/Makefile
	InitializeSrc = $(InitializePath)/Initialize.cc \
		$(InitializePentiumSrc)
	InitializeAsmSrc = $(InitializePentiumAsmSrc)
endif
