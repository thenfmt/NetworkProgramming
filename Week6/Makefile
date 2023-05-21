CC = gcc
CFLAGS = -Ilib
DEPS = lib/*.h

OBJ = obj/data_structured.o obj/file_system.o

all: server client

obj/data_structured.o: util/data_structured.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

obj/file_system.o: util/file_system.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)



server: thread/tcp_server.c $(DEPS) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

client: thread/tcp_client.c $(DEPS) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf obj/*.o server client
