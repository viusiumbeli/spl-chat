#include "client.h"

int main(int argc, char *argv[]) {

    struct sockaddr_in server_info;
    const struct hostent *he;
    int socket_fd;


    if (argc != 2) {
        error("Usage: client hostname");
    }

    if ((he = gethostbyname(argv[1])) == NULL) {
        error("Cannot get host name");
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error("Socket Failure!!");
    }

    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr = *((struct in_addr *) he->h_aliases);
    if (connect(socket_fd, (struct sockaddr *) &server_info, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        exit(1);
    }


    struct client_receive_arguments *const args = malloc(sizeof(args));
    args->socket_fd = socket_fd;
    pthread_t *const streams = malloc(sizeof(pthread_t) * 2);
    if (pthread_create(&streams[0], NULL, receive_message, args) == -1) {
        error("Can't create receive stream");
    }

    struct client_send_arguments *const send_arguments = malloc(sizeof(send_arguments));
    send_arguments->socket_fd = socket_fd;
    if (pthread_create(&streams[1], NULL, send_message, send_arguments) == -1) {
        error("Can't create send stream");
    }

    while (1) {
        if (stop_flag == 1) {
            free(args);
            free(streams);
            free(send_arguments);
            exit(0);
        }
    }
}

void *receive_message(void *args) {
    ssize_t num;
    char buffer[MAXSIZE];
    const struct client_receive_arguments *const actual_args = args;
    while (1) {
        num = recv(actual_args->socket_fd, buffer, sizeof(buffer), 0);
        if (num <= 0) {
            fprintf(stderr,"%s\n","Either Connection Closed or Error");
            break;
        }
        printf("%.*s", (int) num, buffer);
    }
    close(actual_args->socket_fd);
    return NULL;
}

void *send_message(void *args) {
    char buffer[MAXSIZE];
    const struct client_send_arguments *const actual_args = args;
    while (1) {
        fgets(buffer, MAXSIZE - 1, stdin);
        if ((send(actual_args->socket_fd, buffer, strlen(buffer), 0)) == -1) {
            close(actual_args->socket_fd);
            error("Failure Sending Message\n");
            exit(1);
        }
        if (strcmp("exit\n", buffer) == 0) {
            close(actual_args->socket_fd);
            stop_flag = 1;
        }
    }
}

void error(char *const msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}