.PHONY: fx all dub clean

all: elsie-four-k
	./elsie-four-k test1.txt 9 10240 1 4

fx: elsie-four-x
	./elsie-four-x test1.txt a 10240 1 1


elsie-four-k: elsie-four-k.c elsie-four-k.h
	gcc -static -o elsie-four-k elsie-four-k.c

elsie-four-x: elsie-four-x.c elsie-four/lc4.h
	gcc -static -o elsie-four-x elsie-four-x.c

elsie-four.exe: elsie-four-x.c elsie-four/lc4.h
	wine 'c:/program files/gnuwin32/getgnuwin32/bin/gcc' -o elsie-four.exe elsie-four-x.c

dublios: dublios.c
	gcc dublios.c -o dublios

dub: dublios
	./dublios "dublios-data/#_23456789abcdefghijklmnopqrstuvwxyz b.txt"


clean:
	rm -f dublios elsie-four.exe elsie-four-x

