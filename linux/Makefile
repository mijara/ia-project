CFLAGS=-I../include --std=c99 -DOPTIMIZATIONS -DKEEP_LINES

all: build

board.o:
	gcc -c ../src/board.c ${CFLAGS}

input.o:
	gcc -c ../src/input.c ${CFLAGS}

piece.o:
	gcc -c ../src/piece.c ${CFLAGS}

tabu_list.o:
	gcc -c ../src/tabu_list.c ${CFLAGS}

tabu.o:
	gcc -c ../src/tabu.c ${CFLAGS}

build: board.o input.o piece.o tabu_list.o tabu.o
	gcc ../src/main.c -I../include board.o input.o piece.o tabu_list.o tabu.o -o Tetris ${CFLAGS}

clean:
	rm -f Tetris
	rm -f board.o
	rm -f input.o
	rm -f piece.o
	rm -f tabu_list.o
	rm -f tabu.o
