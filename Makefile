# caculator code line number
all:
	(cd scanner; make)
	(cd parser; make)
	(cd SematicAnalyzer; make)
	(cd calculator; make)
clean:
	(cd scanner; make clean)
	(cd parser; make clean)
	(cd calculator; make clean)
	(cd SematicAnalyzer; make clean)

lines:
	@echo "lines    source code file"
	@wc -l `find | grep "\.[ch]$$"`
	@echo "lines    flex/bison file"
	@wc -l `find | grep "\.[yl]$$"`
