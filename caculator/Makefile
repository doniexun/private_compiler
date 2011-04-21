all:caculator

caculator:caculator.o ../scanner/scan.o ../scanner/texthandle.o
	gcc $^ -o $@
caculator.o:caculator.c
	gcc $< -fno-builtin -c -o $@
../scanner/scan.o:../scanner/scan.c
	(cd ../scanner; make scan.o)
../scanner/texthandle.o:../scanner/texthandle.c
	(cd ../scanner; make texthandle.o)
clean:
	(cd ../scanner; make clean)
	rm -f caculator *.o
