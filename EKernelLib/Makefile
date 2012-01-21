CXX = g++
CPPFLAGS = -I. -O3 -mpentium -DDEBUG
SHELL = sh

all : libekernel.a

%.o : %.asm
	nasmw -f coff $<

include LibC/Makefile
include SystemCall/Makefile

ccsources = $(SystemCallSrc) $(LibCSrc)

asmsources = $(SystemCallAsmSrc) $(LibAsmSrc)

ccobjects = $(ccsources:.cc=.o)

asmobjects = $(asmsources:.asm=.o)

objects = $(ccobjects) $(asmobjects)

libekernel.a : $(objects)
	ar -r $@ $(objects)
	cp $@ ../EKernelAps/.

#---------------------------- clean ---------------------------------------

.PHONY : clean
clean :
	rm -f $(ccobjects) $(asmobjects) libkernel.a
