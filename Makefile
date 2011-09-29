MAKEFLAGS += --no-print-directory

all:
	@make -C scanner
	@make -C parser
	@make -C SematicAnalyzer
	@make -C calculator
	@make -C Simulator/PcodeMachine
	@make -C CodeGenerator/pcode
clean:
	@make -C scanner clean
	@make -C parser clean
	@make -C calculator clean
	@make -C SematicAnalyzer clean
	@make -C Simulator/PcodeMachine clean
	@make -C CodeGenerator/pcode clean

# caculator code line number
lines:
	@echo "lines    source code file"
	@wc -l `find | grep "\.[ch]$$"`
	@echo "lines    flex/bison file"
	@wc -l `find | grep "\.[yl]$$"`
