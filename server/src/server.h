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

struct client_t{
    char *id;
    char *name;
};

ssize_t say(int socket, const char *s);


void bind_to_port(int socket);

int open_listener_socket();

void error(const char *msg);

void set_limit_listeners(int socket);

int connect_client(int socket);

size_t read_in(int socket, char *buf, size_t len);

void send_all_clients(const char *msg, const struct node *list, int connect_d, const char *name);

void *client_work(void *args);

int send_to_client_all_messages(int connect_d, MYSQL *conn);

struct client_t register_new_client(MYSQL *conn, int connect_d);

struct client_work_arguments{
    int connect_d_arg;
    struct node *node;
    MYSQL *conn;
};

#endif //SPL_CHAT_SERVER_H
