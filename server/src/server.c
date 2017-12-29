#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

const int port = 3501;

int main() {

    int listener_d = socket(PF_INET, SOCK_STREAM, 0);

    if (listener_d == -1) {
        printf("Can't open socket\n");
    } else {
        printf("socket opened\n");
    }

    int reuse = 1;
    if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int))) {
        printf("Can't set reuse param\n");
    } else {
        printf("Set reuse param\n");
    }

    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t) htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int c = bind(listener_d, (struct sockaddr *) &name, sizeof(name));
    if (c == -1) {
        printf("Can't bind to socket\n");
    } else {
        printf("Bound to socket\n");
    }

    if (listen(listener_d, 10) == -1) {
        printf("Can't listen port\n");
    } else {
        printf("Listen port: %i\n", port);
    }

    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d = accept(listener_d, (struct sockaddr *) &client_addr, &address_size);
    if (connect_d == -1) {
        printf("Can't open second socket\n");
    } else {
        printf("Opened second socket\n");
    }

    char *msg = "YEE\n";
    if (send(connect_d, msg, strlen(msg), 0) == -1) {
        printf("An error occurred while sending the message\n");
    } else {
        printf("Sent the message\n");
    }

    printf("Hello, World!\n");
    return 0;
}