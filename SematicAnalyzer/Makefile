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
	@make -C ../parser parse.o
../parser/syntaxtree.o:../parser/syntaxtree.c
	@make -C ../parser syntaxtree.o
../scanner/texthandle.o:../scanner/texthandle.c
	@make -C ../scanner texthandle.o
../scanner/scan.o:../scanner/scan.c
	@make -C ../scanner scan.o
../lib/xlib.o:../lib/xlib.c
	@make -C ../lib xlib.o
clean:
	rm -f *.o analyzer
	@make -C ../parser clean
	@make -C ../scanner clean
	@make -C ../lib clean
