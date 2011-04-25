CC = gcc
CFLAGS = -fno-builtin -I../include

all:caculator

caculator:caculator.o ../scanner/scan.o ../scanner/texthandle.o ../lib/xlib.o
	$(CC) $(CFLAGS) $^ -o $@
caculator.o:caculator.c
	$(CC) $(CFLAGS) $< -c -o $@
../scanner/scan.o:../scanner/scan.c
	(cd ../scanner; make scan.o)
../scanner/texthandle.o:../scanner/texthandle.c
	(cd ../scanner; make texthandle.o)
../lib/xlib.o:../lib/xlib.c
	(cd ../lib; make xlib.o)
clean:
	rm -f caculator *.o
	(cd ../scanner; make clean)
	(cd ../lib; make clean)
