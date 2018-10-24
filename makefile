all: sod.o
	gcc sod.o img-converter.c -lm -Ofast -march=native -Wall -std=c99 -o img-converter

sod.o: lib/sod/sod.c
	gcc -c lib/sod/sod.c -lm -Ofast -march=native -Wall -std=c99 -o sod.o