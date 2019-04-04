F = -c -std=c++11
M = makefile
all: a

a: main.o parser.o parser.h $M
	g++  main.o parser.o -o a
main.o: main.cpp $M
	g++ $F main.cpp
parser.o: parser.cpp parser.h $M
	g++ $F parser.cpp

clean:
	rm *.o a
clear:
	rm *.o a

