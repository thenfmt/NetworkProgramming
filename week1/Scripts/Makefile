CFLAGS = -c -Wall
gcc = gcc
LIBS =  -lm 

all: program

program: Main.o FileSystem.o DataStructured.o Functionality.o
	${gcc} Main.o FileSystem.o DataStructured.o Functionality.o -o program

Main.o: Main.c
	${gcc} ${CFLAGS} Main.c

FileSystem.o: FileSystem.c
	${gcc} ${CFLAGS} FileSystem.c

DataStructured.o: DataStructured.c
	${gcc} ${CFLAGS} DataStructured.c

Functionality.o: Functionality.c
	${gcc} ${CFLAGS} Functionality.c

clean:
	rm -f *.o *~

