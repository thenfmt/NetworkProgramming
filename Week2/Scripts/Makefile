CFLAGS = -c -Wall
gcc = gcc
LIBS =  -lm 

all: resolver

resolver: Main.o Functionality.o
	${gcc} Main.o Functionality.o -o resolver

Main.o: Main.c
	${gcc} ${CFLAGS} Main.c

Functionality.o: Functionality.c
	${gcc} ${CFLAGS} Functionality.c

clean:
	rm -f *.o *~

