.PHONY: all

all: elsie-four-x
	./elsie-four-x test.txt a 10240 1 3


elsie-four.exe: elsie-four.c elsie-four/lc4.h
	gcc -o elsie-four-x elsie-four.c
