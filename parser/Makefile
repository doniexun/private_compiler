CFLAGS = -fno-builtin -I../include

OBJS = 	parser.o\
	parse.o\
	syntaxtree.o\
	../scanner/texthandle.o\
	../scanner/scan.o\
	../lib/xlib.o

GOBJS =	gparser.o\
	parse.o\
	syntaxtree.o\
	../scanner/texthandle.o\
	../scanner/scan.o\
	../lib/xlib.o

all:parser
parser:$(OBJS)
	gcc $(CFLAGS) $^ -o $@
parser.o:parser.c
	gcc $(CFLAGS) $< -c -o $@
gparser:$(GOBJS)
	gcc $(CFLAGS) $^ -o $@
gparser.o:parser.c
	gcc $(CFLAGS) -DGENERAL_TRAVERSE $< -c -o $@
bison_parser.o:parser.c
	gcc $(CFLAGS) -DBISON_PARSER $< -c -o $@
parse.o:parse.c
	gcc $(CFLAGS) $< -c -o $@
syntaxtree.o:syntaxtree.c
	gcc $(CFLAGS) $< -c -o $@
../scanner/texthandle.o:../scanner/texthandle.c
	(cd ../scanner; make texthandle.o)
../scanner/scan.o:../scanner/scan.c
	(cd ../scanner; make scan.o)
../lib/xlib.o:../lib/xlib.c
	(cd ../lib; make xlib.o)
clean:
	rm -f *.o parser gparser
	(cd ../scanner; make clean)
	(cd ../lib; make clean)
