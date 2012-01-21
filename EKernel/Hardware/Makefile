
HardwarePath = Hardware

ifeq ($(MAKECMDGOALS),Pentium)
	include $(HardwarePath)/Pentium/Makefile
	HardwareSrc = $(HardwarePath)/Processor.cc \
		$(HardwarePentiumSrc)
	HardwareAsmSrc = $(HardwarePentiumAsmSrc)
endif

ifeq ($(MAKECMDGOALS),clean)
	include $(HardwarePath)/Pentium/Makefile
	HardwareSrc = $(HardwarePath)/Processor.cc \
		$(HardwarePentiumSrc)
	HardwareAsmSrc = $(HardwarePentiumAsmSrc)
endif

