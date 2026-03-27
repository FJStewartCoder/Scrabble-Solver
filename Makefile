CC = gcc
FLAGS = -Wall -g

base.o: src/algorithms/base.c
	${CC} ${FLAGS} $^ -Iinclude/algorithms -o $@ -c

big.o: src/algorithms/big.c
	${CC} ${FLAGS} $^ -Iinclude/algorithms -o $@ -c

ram_abuse.o: src/algorithms/ram_abuse.c
	${CC} ${FLAGS} $^ -Iinclude/algorithms -o $@ -c

shortptr.o: src/algorithms/shortptr.c
	${CC} ${FLAGS} $^ -Iinclude/algorithms -o $@ -c

simple.o: src/algorithms/simple.c
	${CC} ${FLAGS} $^ -Iinclude/algorithms -o $@ -c



all: base.o big.o ram_abuse.o shortptr.o simple.o src/main.c
	${CC} ${FLAGS} $^ -Iinclude

clean:
	del *.o