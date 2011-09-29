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
	@make -C ../../SematicAnalyzer symtab.o
../../SematicAnalyzer/typecheck.o:../../SematicAnalyzer/typecheck.c
	@make -C ../../SematicAnalyzer typecheck.o
../../parser/parse.o:../../parser/parse.c
	@make -C ../../parser parse.o
../../parser/syntaxtree.o:../../parser/syntaxtree.c
	@make -C ../../parser syntaxtree.o
../../scanner/texthandle.o:../../scanner/texthandle.c
	@make -C ../../scanner texthandle.o
../../scanner/scan.o:../../scanner/scan.c
	@make -C ../../scanner scan.o
../../lib/xlib.o:../../lib/xlib.c
	@make -C ../../lib xlib.o
clean:
	rm -f *.o generator
	@make -C ../../parser clean
	@make -C ../../scanner clean
	@make -C ../../lib clean
