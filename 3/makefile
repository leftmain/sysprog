M = makefile
F = -c -g
all: a

a: main.o userfs.o
	gcc main.o userfs.o -o a
main.o: main.c userfs.h $M
	gcc $F main.c
userfs.o: userfs.c userfs.h $M
	gcc $F userfs.c

clear:
	rm *.o a
clean:
	rm *.o a
