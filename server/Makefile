CC=gcc
CFLAGS= -pedantic -std=c11 -Wall -Werror

all: server.o list.o database.o
	$(CC) $(CFLAGS) -pthread ./build/server.o ./build/database.o ./build/list.o $(shell mysql_config --libs) -o ./build/server $(shell mysql_config --cflags)

server.o:
	mkdir ./build
	$(CC) $(CFLAGS) -pthread -c ./src/server.c -o ./build/server.o

database.o:
	$(CC) $(CFLAGS) -c ./src/database.c -o ./build/database.o

list.o:
	$(CC) $(CFLAGS) -c ./src/list.c -o ./build/list.o

clean-o-files:
	rm -rf ./build/server.o

clean-project:
	rm -rf ./build/server

clean: clean-project clean-o-files
	rm -rf ./build
