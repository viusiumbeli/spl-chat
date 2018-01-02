#ifndef SPL_CHAT_SERVER_H
#define SPL_CHAT_SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "list.h"
#include "database.h"

const int port = 3501;
const int limit_listeners = 10;
const size_t buf_len = 1024;

typedef struct {
    char *id;
    char *name;
} client_t;

ssize_t say(int socket, char *s);

void bind_to_port(int socket);

int open_listener_socket();

void error(char *msg);

void set_limit_listeners(int socket);

int connect_client(int socket);

size_t read_in(int socket, char *buf, size_t len);

void send_all_clients(char *msg, node_t *list, int connect_d, char *name);

void *client_work(void *args);

int send_to_client_all_messages(int connect_d, MYSQL *conn);

client_t register_new_client(MYSQL *conn, int connect_d);

typedef struct {
    int connect_d_arg;
    node_t *node;
    MYSQL *conn;
} client_work_arguments;

#endif //SPL_CHAT_SERVER_H
