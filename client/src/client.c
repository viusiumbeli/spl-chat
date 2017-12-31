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

    while(1){

    }
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

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}