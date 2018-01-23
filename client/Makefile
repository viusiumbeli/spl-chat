CC=gcc
CFLAGS= -pedantic -std=c11 -Wall -Werror

all: client.o
	$(CC) $(CFLAGS) -pthread ./build/client.o -o ./build/client

client.o:
	mkdir ./build
	$(CC) $(CFLAGS) -c ./src/client.c -o ./build/client.o

clean:
	rm -rf ./build
