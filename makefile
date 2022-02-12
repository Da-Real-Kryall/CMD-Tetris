all:
	gcc -lncurses -pthread -Ofast -o Tetris.out main.c
.PHONY: all
