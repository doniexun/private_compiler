CC = gcc
CFLAGS = -fno-builtin -I../../include


all:pm
pm:exec.o instruction.o memory.o pmachine.o stack.o ../../lib/xlib.o
	$(CC) $(CFLAGS) -o $@ $^
exec.o:exec.c
	$(CC) $(CFLAGS) -c -o $@ $^
memory.o:memory.c
	$(CC) $(CFLAGS) -c -o $@ $^
instruction.o:instruction.c
	$(CC) $(CFLAGS) -c -o $@ $^
pmachine.o:pmachine.c
	$(CC) $(CFLAGS) -c -o $@ $^
stack.o:stack.c
	$(CC) $(CFLAGS) -c -o $@ $^
../../lib/xlib.o:../../lib/xlib.c
	make -C ../../lib xlib.o

debug:test_stack
test_stack:dstack.o ../../lib/xlib.o
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^
dstack.o:stack.c
	$(CC) $(CFLAGS) -DDEBUG -c -o $@ $^

clean:
	rm -rf pm *.o test_*
	@make -C ../../lib clean
