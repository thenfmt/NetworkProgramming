CC = gcc
CFLAGS = -Ilib
DEPS = lib/*.h

OBJ = obj/file_system.o
all: server client

obj/file_system.o: util/file_system.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: src/server.c $(DEPS) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

client: src/client.c $(DEPS) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf obj/*.o server client
