CC = gcc
CFLAGS = -fno-builtin

all:scan
scan:scan.c texthandle.c
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f scan
