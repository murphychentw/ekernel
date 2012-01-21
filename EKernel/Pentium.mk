CXX = gxx
CPPFLAGS = -I. -O3 -mpentium -DDEBUG -fno-exceptions -fno-rtti
SHELL = sh

%.o : %.cc
	$(CXX) $(CPPFLAGS) -c $< -o $@

%.o : %.asm
	nasmw -f coff $<

%.d: %.cc
	rm -f $@
	@echo $*.o $@ : \ >> $@
	$(CXX) -MM $(CPPFLAGS) $< | sed 's/.*\.o://g' >> $@

Pentium: kernel.bin

kernel.bin : $(Objects)
	ld --entry=start -Map kernel.map --oformat binary -Ttext 0x00100000 \
	$(Objects) -o $@

	cp $@ o:/
