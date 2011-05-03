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
	(cd ../parser; make bison_parser.o)
../parser/syntaxtree.o:../parser/syntaxtree.c
	(cd ../parser; make syntaxtree.o)
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
	(cd ../lib; make xlib.o)
clean:
	(cd ../lib; make clean)
	(cd ../parser; make clean)
	rm -f scan.c *.o scanner parse.c parser *.output parse.h
