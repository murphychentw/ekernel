CXX=g++
CPPFLAGS = -I. -O3 -mpentium -DDEBUG

all : proc1.cof proc2.cof

%.o : %.cc
	$(CXX) $(CPPFLAGS) -c $< -o $@


proc1.cof : proc1.o

	ld --entry=_main -Map $@.map --oformat coff-go32 \
	-Ttext 0x80000000 \
	$< -L . -l ekernel -o $@

	cp $@ a:/

proc2.cof : proc2.o

	ld --entry=_main -Map $@.map --oformat coff-go32 \
	-Ttext 0x80000000 \
	$< -L . -l ekernel -o $@

	cp $@ a:/
