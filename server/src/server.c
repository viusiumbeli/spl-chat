#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include "server.h"

const int port = 3501;
const int limit_listeners = 10;

int main() {
    int listener_d = open_listener_socket();

    bind_to_port(listener_d);

    set_limit_listeners(listener_d);

    size_t buf_len = 255;
    char buf[buf_len];

    while (1) {
        int connect_d = connect_client(listener_d);

        if (!fork()) {
            close(listener_d);
            char *msg = "YEE\n";
            if (say(connect_d, msg) != -1) {
                size_t len = read_in(connect_d, buf, buf_len);
                printf("%s\n", buf);
                printf("%zu\n", len);
                if (len != 0) {
                    close(connect_d);
                    exit(0);
                }
            }
        }
        close(connect_d);
    }
}

size_t read_in(int socket, char *buf, size_t len) {
    char *s = buf;
    size_t slen = len;
    ssize_t c = recv(socket, s, slen, 0);
    slen -= c;
    while ((c > 0) && (s[c - 1] != '\n')) {
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
        printf("!");
    }
    if (c < 0) {
        printf("An error occurred while receiving the message\n");
    } else {
        if (c == 0) {
            buf[0] = '\0';
        } else {
            s[c - 1] = '\0';
        }
    }
    return len - slen;
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

ssize_t say(int socket, char *s) {
    ssize_t result = send(socket, s, strlen(s), 0);
    if (result == -1) {
        printf("An error occurred while sending the message");
    } else {
        printf("Sent the message\n");
    }
    return result;
}

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}