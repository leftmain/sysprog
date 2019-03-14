F = -lrt
all: a
a: main.o arrays.o
	gcc main.c arrays.o -o a
main.o: main.c
	gcc $F -c main.c
arrays.o: arrays.c arrays.h
	gcc $F -c arrays.c
clear:
	rm *.o a

