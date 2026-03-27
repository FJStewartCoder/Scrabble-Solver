CC = gcc
FLAGS = -Wall -g

base.o: src/algorithms/base.c
	${CC} ${FLAGS} $^ -Iinclude/algorithms -o $@ -c

score.o: src/algorithms/score.c
	${CC} ${FLAGS} $^ -Iinclude/algorithms -o $@ -c

all: base.o score.o src/main.c
	${CC} ${FLAGS} $^ -Iinclude

clean:
	del *.o