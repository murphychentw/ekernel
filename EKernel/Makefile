# Required GNU Make version 3.77
# ( Type 'make -v' to see the version )

default :
	@echo "Usage: make CPU_TYPE"
	@echo
	@echo "Valid CPU_TYPE:"
	@echo "Pentium ( compile using DJGPP C/C++ Compiler)"
	@echo "StrongArm ( compile using Metaware High C/C++ Compier) "


include Initialize/Makefile
include ADT/Makefile
include Hardware/Makefile
include Kernel/Makefile
include LibC/Makefile
include MemoryManager/Makefile


CCSources = $(ADTSrc) $(HardwareSrc) $(InitializeSrc) \
	$(KernelSrc) $(LibCSrc) $(MemoryManagerSrc)

# Note: InitializeAsmSrc should be placed first.
ASMSources = $(InitializeAsmSrc) \
	$(HardwareAsmSrc) $(KernelAsmSrc) $(LibCAsmSrc)

CCObjects = $(CCSources:.cc=.o)
ASMObjects = $(ASMSources:.asm=.o)
Objects = $(ASMObjects) $(CCObjects)

CCDepends = $(CCSources:.cc=.d)


ifeq ($(MAKECMDGOALS),Pentium)
	include $(CCSources:.cc=.d)
	include Pentium.mk
endif


ifeq ($(MAKECMDGOALS),StrongArm)
	include $(CCSources:.cc=.d)
	include StrongArm.mk
endif

# -------- depend --------
.PHONY : depend
depend : $(CCDepends)
	@echo $(CCDepends)

# -------- clean ---------

.PHONY : clean
clean :
	rm -f $(CCObjects) $(ASMObjects) $(CCDepends) kernel.*
