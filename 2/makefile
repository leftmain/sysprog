F = -c -std=c++11
all: a

a: main.o parser.o parser.h makefile
	g++  main.o parser.o -o a
main.o: main.cpp
	g++ $F main.cpp
parser.o: parser.cpp parser.h
	g++ $F parser.cpp

clean:
	rm *.o a
clear:
	rm *.o a

