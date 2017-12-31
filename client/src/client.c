#include "client.h"

int main(int argc, char *argv[]) {

    struct sockaddr_in server_info;
    struct hostent *he;
    int socket_fd;
    char buff[MAXSIZE];


    if (argc != 2) {
        fprintf(stderr, "Usage: client hostname\n");
        exit(1);
    }

    if ((he = gethostbyname(argv[1])) == NULL) {
        fprintf(stderr, "Cannot get host name\n");
        exit(1);
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Socket Failure!!\n");
        exit(1);
    }

    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr = *((struct in_addr *) he->h_aliases);
    if (connect(socket_fd, (struct sockaddr *) &server_info, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        exit(1);
    }


    client_receive_arguments *args = malloc(sizeof(args));
    args->socket_fd = socket_fd;
    pthread_t *streams = malloc(sizeof(pthread_t) * 2);
    if (pthread_create(&streams[0], NULL, receive_message, args) == -1) {
        error("Can't create receive stream");
    }

    client_send_arguments *send_arguments = malloc(sizeof(send_arguments));
    send_arguments->socket_fd = socket_fd;
    if (pthread_create(&streams[1], NULL, send_message, send_arguments) == -1) {
        error("Can't create send stream");
    }

    while (1) {

    }
    free(args);
    free(streams);
    free(send_arguments);
}

void *receive_message(void *args) {
    ssize_t num;
    char buffer[MAXSIZE];
    client_receive_arguments *actual_args = args;
    while (1) {
        num = recv(actual_args->socket_fd, buffer, sizeof(buffer), 0);
        if (num <= 0) {
            printf("Either Connection Closed or Error\n");
            break;
        }
        printf("%s\n", buffer);
    }
    close(actual_args->socket_fd);
}

void *send_message(void *args) {
    char buffer[MAXSIZE];
    ssize_t num;
    client_send_arguments *actual_args = args;
    while (1) {
        fgets(buffer, MAXSIZE - 1, stdin);
        if ((send(actual_args->socket_fd, buffer, strlen(buffer), 0)) == -1) {
            close(actual_args->socket_fd);
            error("Failure Sending Message\n");
            exit(1);
        }
    }
}

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}