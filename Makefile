all:
	gcc mem_copy.c -o mem_copy -O0
clean:
	rm mem_copy
asm:
	gcc mem_copy.c -o mem_copy -O0 -Wa,-adhln -g > asm.S

