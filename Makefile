MAKEFLAGS += --no-print-directory

all:
	@(cd scanner; make)
	@(cd parser; make)
	@(cd SematicAnalyzer; make)
	@(cd calculator; make)
	@(cd Simulator/PcodeMachine; make)
	@(cd CodeGenerator/pcode; make)
clean:
	@(cd scanner; make clean)
	@(cd parser; make clean)
	@(cd calculator; make clean)
	@(cd SematicAnalyzer; make clean)
	@(cd Simulator/PcodeMachine; make clean)
	@(cd CodeGenerator/pcode; make clean)

# caculator code line number
lines:
	@echo "lines    source code file"
	@wc -l `find | grep "\.[ch]$$"`
	@echo "lines    flex/bison file"
	@wc -l `find | grep "\.[yl]$$"`
