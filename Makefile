# caculator code line number
lines:
	@echo "lines    source code file"
	@wc -l `find|grep "\.[ch]$$"`
