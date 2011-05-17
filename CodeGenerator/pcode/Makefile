CFLAGS = -fno-builtin -I../../include

OBJS = 	generator.o\
	pcodegen.o\
	../../SematicAnalyzer/symtab.o\
	../../SematicAnalyzer/typecheck.o\
	../../parser/parse.o\
	../../parser/syntaxtree.o\
	../../scanner/texthandle.o\
	../../scanner/scan.o\
	../../lib/xlib.o

all:generator
generator:$(OBJS)
	gcc $(CFLAGS) $^ -o $@
generator.o:generator.c
	gcc $(CFLAGS) $< -c -o $@
pcodegen.o:pcodegen.c
	gcc $(CFLAGS) $< -c -o $@
../../SematicAnalyzer/symtab.o:../../SematicAnalyzer/symtab.c
	(cd ../../SematicAnalyzer; make symtab.o)
../../SematicAnalyzer/typecheck.o:../../SematicAnalyzer/typecheck.c
	(cd ../../SematicAnalyzer; make typecheck.o)
../../parser/parse.o:../../parser/parse.c
	(cd ../../parser; make parse.o)
../../parser/syntaxtree.o:../../parser/syntaxtree.c
	(cd ../../parser; make syntaxtree.o)
../../scanner/texthandle.o:../../scanner/texthandle.c
	(cd ../../scanner; make texthandle.o)
../../scanner/scan.o:../../scanner/scan.c
	(cd ../../scanner; make scan.o)
../../lib/xlib.o:../../lib/xlib.c
	(cd ../../lib; make xlib.o)
clean:
	rm -f *.o generator
	(cd ../../parser; make clean)
	(cd ../../scanner; make clean)
	(cd ../../lib; make clean)
