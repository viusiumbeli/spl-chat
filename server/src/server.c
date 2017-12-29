#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

const int port = 3501;
const int limit_listeners = 10;

int main() {
    int listener_d = open_listener_socket();

    bind_to_port(listener_d);

    set_limit_listeners(listener_d);

    int connect_d = connect_client(listener_d);

    char *msg = "YEE\n";
    say(connect_d, msg);

    return 0;
}

int connect_client(int socket) {
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d = accept(socket, (struct sockaddr *) &client_addr, &address_size);
    if (connect_d == -1) {
        error("Can't open second socket");
    } else {
        printf("Opened second socket\n");
    }
    return connect_d;
}

void set_limit_listeners(int socket) {
    if (listen(socket, limit_listeners) == -1) {
        error("Can't listen port");
    } else {
        printf("Listen port: %i\n", port);
    }
}

int open_listener_socket() {
    int s = socket(PF_INET, SOCK_STREAM, 0);

    if (s == -1) {
        error("Can't open socket");
    } else {
        printf("Socket opened\n");
    }

    return s;
}

void bind_to_port(int socket) {
    int reuse = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int))) {
        error("Can't set reuse param");
    } else {
        printf("Set reuse param\n");
    }

    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t) htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int c = bind(socket, (struct sockaddr *) &name, sizeof(name));
    if (c == -1) {
        error("Can't bind to socket");
    } else {
        printf("Bound to socket\n");
    }
}

void say(int socket, char *s) {
    if (send(socket, s, strlen(s), 0) == -1) {
        error("An error occurred while sending the message");
    } else {
        printf("Sent the message\n");
    }
}

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}