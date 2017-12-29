#ifndef SPL_CHAT_SERVER_H
#define SPL_CHAT_SERVER_H

void say(int socket, char *s);

void bind_to_port(int socket);

int open_listener_socket();

void error(char *msg);

void set_limit_listeners(int socket);

int connect_client(int socket);

#endif //SPL_CHAT_SERVER_H
