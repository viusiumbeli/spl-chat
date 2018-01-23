#ifndef SPL_CHAT_CLIENT_H
#define SPL_CHAT_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>


#define PORT 3501
#define MAXSIZE 1024

struct client_receive_arguments {
    int socket_fd;
};

struct client_send_arguments {
    int socket_fd;
};

int stop_flag = 0;

void error(char *msg);

void *receive_message(void *args);

void *send_message(void *args);

#endif //SPL_CHAT_CLIENT_H
