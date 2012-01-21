CC = hcarm
CXX = hcarm
AS = asarm
CPPFLAGS = -Hapcs -Hcode32 -HL -Hnothumb -Hstrongarm -Hnocopyr -I.
SFLAGS =  -code32 -le
SHELL = bash

%.o : %.s
	$(AS) $(SFLAGS) $< -o $@

%.o : %.cc
	rem $(CXX) $(CPPFLAGS) -c $< -o $@
	@echo $(CPPFLAGS) -c $< -o $@ > $*.cmd
	$(CXX) @$*.cmd
	rm -f $*.cmd

%.d: %.cc
	@echo s#.*\.o:#$*.o $@:#g | sed s/\//\\\//g | sed s/#/\//g > $*.sed
	$(CXX) -Hmake -Hrel $(CPPFLAGS) $< | sed --file=$*.sed > $@
	rm -f $*.sed

StrongArm: kernel.bin

kernel.bin : $(Objects) $(Libraries)
	echo $(Objects) $(Libraries) | file2cmd kernel.cmd
	ldarm -B base=0x8000 -B noheader -B static -d n -e _entry -o kernel.elf -A kernel.cmd
	elf2hex -q kernel.elf -o kernel.hex
	hex2bin kernel.hex kernel.bin
