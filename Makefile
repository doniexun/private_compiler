# caculator code line number
all:
	(cd scanner; make)
	(cd parser; make)
	(cd calculator; make)
clean:
	(cd scanner; make clean)
	(cd parser; make clean)
	(cd calculator; make clean)

lines:
	@echo "lines    source code file"
	@wc -l `find|grep "\.[ch]$$"`
