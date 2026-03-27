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



test.o: src/tests/test.c
	${CC} ${FLAGS} $^ -Iinclude -Iinclude/tests -o $@ -c

speed_test.o: src/tests/speed_test.c
	${CC} ${FLAGS} $^ -Iinclude -Iinclude/tests -o $@ -c

functionality_test.o: src/tests/functionality_test.c
	${CC} ${FLAGS} $^ -Iinclude -Iinclude/tests -o $@ -c


all: test.o speed_test.o functionality_test.o base.o big.o ram_abuse.o shortptr.o simple.o src/main.c
	${CC} ${FLAGS} $^ -Iinclude

clean:
	del *.o