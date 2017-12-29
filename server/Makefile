CC=gcc
CFLAGS= -std=c11

all: server.o
	$(CC) $(CFLAGS) ./build/server.o -o ./build/server

server.o:
	$(CC) $(CFLAGS) -c ./src/server.c -o ./build/server.o

clean-o-files:
	rm -rf ./build/server.o

clean-project:
	rm -rf ./build/server
