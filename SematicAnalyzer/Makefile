CFLAGS = -fno-builtin -I../include

OBJS = 	analyzer.o\
	symtab.o\
	typecheck.o\
	../parser/parse.o\
	../parser/syntaxtree.o\
	../scanner/texthandle.o\
	../scanner/scan.o\
	../lib/xlib.o

all:analyzer
analyzer:$(OBJS)
	gcc $(CFLAGS) $^ -o $@
symtab.o:symtab.c
	gcc $(CFLAGS) $< -c -o $@
typecheck.o:typecheck.c
	gcc $(CFLAGS) $< -c -o $@
../parser/parse.o:../parser/parse.c
	(cd ../parser; make parse.o)
../parser/syntaxtree.o:../parser/syntaxtree.c
	(cd ../parser; make syntaxtree.o)
../scanner/texthandle.o:../scanner/texthandle.c
	(cd ../scanner; make texthandle.o)
../scanner/scan.o:../scanner/scan.c
	(cd ../scanner; make scan.o)
../lib/xlib.o:../lib/xlib.c
	(cd ../lib; make xlib.o)
clean:
	rm -f *.o analyzer
	(cd ../parser; make clean)
	(cd ../scanner; make clean)
	(cd ../lib; make clean)
