all: a.out test

a.out: main.cpp
	g++ main.cpp

test:
	./a.out < input.txt
