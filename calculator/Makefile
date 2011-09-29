CC = gcc
CFLAGS = -fno-builtin -I../include

all:caculator

caculator:caculator.o ../scanner/scan.o ../scanner/texthandle.o ../lib/xlib.o
	$(CC) $(CFLAGS) $^ -o $@
caculator.o:caculator.c
	$(CC) $(CFLAGS) $< -c -o $@
../scanner/scan.o:../scanner/scan.c
	@make -C ../scanner scan.o
../scanner/texthandle.o:../scanner/texthandle.c
	@make -C ../scanner texthandle.o
../lib/xlib.o:../lib/xlib.c
	@make -C ../lib xlib.o
clean:
	rm -f caculator *.o
	@make -C ../scanner clean
	@make -C ../lib clean
