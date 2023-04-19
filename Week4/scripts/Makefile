CC = gcc
CFLAGS = -Ilib
DEPS = lib/*.h

OBJ = obj/data_structured.o obj/file_system.o obj/functionality.o

all: server sender_client receiver_client

obj/data_structured.o: util/data_structured.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

obj/file_system.o: util/file_system.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

obj/functionality.o: util/functionality.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: src/server/server.c $(DEPS) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

sender_client: src/client/sender_client.c $(DEPS) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

receiver_client: src/client/receiver_client.c $(DEPS) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf obj/*.o server sender_client receiver_client
