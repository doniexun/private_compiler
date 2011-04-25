CC = gcc
CFLAGS = -fno-builtin -I../include

all:scanner
scanner:scanner.o scan.o texthandle.o ../lib/xlib.o
scanner.o:scanner.c
	$(CC) $(CFLAGS) $^ -c -o $@
scan.o:scan.c
	$(CC) $(CFLAGS) $^ -c -o $@
texthandle.o:texthandle.c
	$(CC) $(CFLAGS) $^ -c -o $@
../lib/xlib.o:../lib/xlib.c
	(cd ../lib; make xlib.o)
	
clean:
	rm -f scanner *.o
	(cd ../lib; make clean)
