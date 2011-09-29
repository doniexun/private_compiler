CC = gcc
LEX = flex
BISON = bison
CFLAGS = -I../include

all:scanner parser
parser: ../parser/bison_parser.o\
	../parser/syntaxtree.o	\
	../lib/xlib.o		\
	parse.o scan.o
	$(CC) $^ -o $@
../parser/bison_parser.o:../parser/parser.c
	@make -C ../parser bison_parser.o
../parser/syntaxtree.o:../parser/syntaxtree.c
	@make -C ../parser syntaxtree.o
parse.o:parse.c
	$(CC) $(CFLAGS) -DBUILT_IN $< -c -o $@

parse.c:parse2.y
	$(BISON) -o $@ $<

scanner:scanner.o scan.o ../lib/xlib.o
	$(CC) $^ -o $@ -lfl

scanner.o:scanner.c
	$(CC) $(CFLAGS) -DBUILT_IN $< -c -o $@

scan.o:scan.c
	$(CC) $(CFLAGS) -DBUILT_IN $< -c -o $@

scan.c:scan.l
	$(LEX) -o $@ $<

../lib/xlib.o:../lib/xlib.c
	@make -C ../lib xlib.o
clean:
	@make -C ../lib clean
	@make -C ../parser clean
	rm -f scan.c *.o scanner parse.c parser *.output parse.h
