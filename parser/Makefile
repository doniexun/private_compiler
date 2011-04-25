CFLAGS = -fno-builtin

OBJS = 	parser.o\
	parse.o\
	syntaxtree.o\
	../scanner/texthandle.o\
	../scanner/scan.o

all:parser
parser:$(OBJS)
	gcc $(CFLAGS) $^ -o $@
parser.o:parser.c
	gcc $(CFLAGS) $< -c -o $@
parse.o:parse.c
	gcc $(CFLAGS) $< -c -o $@
syntaxtree.o:syntaxtree.c
	gcc $(CFLAGS) $< -c -o $@
../scanner/texthandle.o:../scanner/texthandle.c
	(cd ../scanner; make texthandle.o)
../scanner/scan.o:../scanner/scan.c
	(cd ../scanner; make scan.o)
clean:
	rm -f *.o parser
	(cd ../scanner; make clean)
