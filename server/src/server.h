#ifndef SPL_CHAT_SERVER_H
#define SPL_CHAT_SERVER_H

ssize_t say(int socket, char *s);

void bind_to_port(int socket);

int open_listener_socket();

void error(char *msg);

void set_limit_listeners(int socket);

int connect_client(int socket);

size_t read_in(int socket, char *buf, size_t len);

void send_all_clients(int *connections, char *msg, int size);

void *client_work(void *args);
#endif //SPL_CHAT_SERVER_H
