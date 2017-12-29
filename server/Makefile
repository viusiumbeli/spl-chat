CC=gcc
CFLAGS= -std=c11

all: server.o
	$(CC) $(CFLAGS) -pthread ./build/server.o -o ./build/server

server.o:
	$(CC) $(CFLAGS) -pthread -c ./src/server.c -o ./build/server.o

clean-o-files:
	rm -rf ./build/server.o

clean-project:
	rm -rf ./build/server

clean: clean-project clean-o-files