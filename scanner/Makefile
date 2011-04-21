CC = gcc
CFLAGS = -fno-builtin

all:scanner
scanner:scanner.o scan.o texthandle.o
scanner.o:scanner.c
	$(CC) $(CFLAGS) $^ -c -o $@
scan.o:scan.c
	$(CC) $(CFLAGS) $^ -c -o $@
texthandle.o:texthandle.c
	$(CC) $(CFLAGS) $^ -c -o $@
clean:
	rm -f scanner *.o
