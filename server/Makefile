CC=gcc
CFLAGS= -std=c11

all: server.o list.o
	$(CC) $(CFLAGS) -pthread ./build/server.o ./build/list.o -o ./build/server

server.o:
	$(CC) $(CFLAGS) -pthread -c ./src/server.c -o ./build/server.o

list.o:
	$(CC) $(CFLAGS) -c ./src/list.c -o ./build/list.o

clean-o-files:
	rm -rf ./build/server.o

clean-project:
	rm -rf ./build/server

clean: clean-project clean-o-files
