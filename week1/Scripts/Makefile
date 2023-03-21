CFLAGS = -c -Wall
gpp = g++
LIBS =  -lm 

all: program

program: Main.o IOsystem.o DataStructured.o Functionality.o
	${gpp} Main.o IOsystem.o DataStructured.o Functionality.o -o program

Main.o: Main.cpp
	${gpp} ${CFLAGS} Main.cpp

IOsystem.o: IOsystem.cpp
	${gpp} ${CFLAGS} IOsystem.cpp

DataStructured.o: DataStructured.cpp
	${gpp} ${CFLAGS} DataStructured.cpp

Functionality.o: Functionality.cpp
	${gpp} ${CFLAGS} Functionality.cpp

clean:
	rm -f *.o *~

