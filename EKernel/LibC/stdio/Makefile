
LibCStdioPath = $(LibCPath)/Stdio
LibCStdioCommonSrc = $(LibCStdioPath)/doprnt.cc $(LibCStdioPath)/printf.cc \
	$(LibCStdioPath)/puts.cc $(LibCStdioPath)/sprintf.cc

LibCStdioSrc = $(LibCStdioCommonSrc)

ifeq ($(MAKECMDGOALS),Pentium)
	LibCStdioSrc = $(LibCStdioCommonSrc) \
		$(LibCStdioPath)/serial_io.cc
endif

ifeq ($(MAKECMDGOALS),StrongArm)
	LibCStdioSrc = $(LibCStdioCommonSrc)
endif

