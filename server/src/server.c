#include "server.h"

int main() {
    MYSQL *conn = connect_to_database();
    print_database_info();
    create_tables(conn);

    printf("\n");


    const int listener_d = open_listener_socket();

    bind_to_port(listener_d);

    set_limit_listeners(listener_d);

    pthread_t *const streams = malloc(sizeof(pthread_t) * limit_listeners);

    struct node *list = malloc(sizeof(struct node));
    list->next = NULL;
    struct client_work_arguments*const args = malloc(sizeof(args));

    int i = 0;
    while (1) {
        const int connect_d = connect_client(listener_d);
        add_node(list, connect_d);
        args->connect_d_arg = connect_d;
        args->node = list;
        args->conn = conn;
        print_list(list);
        if (pthread_create(&streams[i], NULL, client_work, args) == -1) {
            error("Can't create stream");
        }
        if (i >= 10) {
            break;
        }
    }
    mysql_close(conn);
}

void *client_work(void *args) {
    struct client_work_arguments *actual_args = args;
    const int actual_connect_d = actual_args->connect_d_arg;

    const struct client_t client = register_new_client(actual_args->conn, actual_connect_d);
    char *const buf = malloc(buf_len);

    if (send_to_client_all_messages(actual_connect_d, actual_args->conn) != -1) {
        while (1) {
            read_in(actual_connect_d, buf, buf_len);
            send_all_clients(buf, actual_args->node, actual_connect_d, client.name);
            if (strcmp("exit", buf) == 0) {
                close(actual_connect_d);
                remove_node(actual_args->node, actual_connect_d);
                break;
            } else {
                if (save_message(buf, actual_args->conn, client.id)) {
                    fprintf(stderr, "Error: %s [%d]\n", mysql_error(actual_args->conn), mysql_errno(actual_args->conn));
                } else {
                    MYSQL_RES *res = mysql_store_result(actual_args->conn);
                    mysql_free_result(res);
                }
            }
        }
        return NULL;
    }
    return NULL;
}

struct client_t register_new_client(MYSQL *const conn, const int connect_d) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct client_t client;

    if (say(connect_d, "Enter your name\n") != -1) {
        char *const buf = malloc(buf_len);
        read_in(connect_d, buf, buf_len);
        create_new_user(buf, conn);

        select_user_by_name(buf, conn);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res))) {
            client.id = row[0];
        }

        client.name = buf;

        return client;
    }
    client.name = "admin";
    client.id = "0";
    return client;
}

int send_to_client_all_messages(const int connect_d, MYSQL *const conn) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    get_all_messages_with_author(conn);
    res = mysql_store_result(conn);
    char *full_msg;
    while ((row = mysql_fetch_row(res))) {
        full_msg = malloc(strlen(row[0]) + strlen(row[1]) + 3);
        strcat(full_msg, row[1]);
        strcat(full_msg, ": ");
        strcat(full_msg, row[0]);
        strcat(full_msg, "\n");
        say(connect_d, full_msg);
        free(full_msg);
    }

    return 0;
}

void send_all_clients(const char *const msg, const struct node *list, const int connect_d, const char *const name) {
    char *full_msg;
    while (list->next != NULL) {
        list = list->next;
        if (list->val != connect_d) {
            full_msg = calloc((strlen(msg) + strlen(name) + 4) * sizeof(void *), 1);
            strcat(full_msg, name);
            strcat(full_msg, ": ");
            strcat(full_msg, msg);
            strcat(full_msg, "\n");
            say(list->val, full_msg);
            free(full_msg);
        }
    }
}

size_t read_in(const int socket, char *const buf, size_t len) {
    char *s = buf;
    size_t slen = len;
    ssize_t c = recv(socket, s, slen, 0);
    slen -= c;
    while ((c > 0) && (s[c - 1] != '\n')) {
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
    }
    if (c < 0) {
        fprintf(stderr, "%s\n", "An error occurred while receiving the message");
    } else {
        if (c == 0) {
            buf[0] = '\0';
        } else {
            s[c - 1] = '\0';
            ++slen;
        }
    }
    return len - slen;
}

int connect_client(const int socket) {
    const struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d = accept(socket, (struct sockaddr *) &client_addr, &address_size);
    if (connect_d == -1) {
        error("Can't open second socket");
    } else {
        printf("%s\n", "Opened second socket");
    }
    return connect_d;
}

void set_limit_listeners(const int socket) {
    if (listen(socket, limit_listeners) == -1) {
        error("Can't listen port");
    } else {
        printf("Listen port: %i\n", port);
    }
}

int open_listener_socket() {
    const int s = socket(PF_INET, SOCK_STREAM, 0);

    if (s == -1) {
        error("Can't open socket");
    } else {
        printf("%s\n", "Socket opened");
    }

    return s;
}

void bind_to_port(const int socket) {
    const int reuse = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int))) {
        error("Can't set reuse param");
    } else {
        printf("%s\n", "Set reuse param");
    }

    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t) htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int c = bind(socket, (struct sockaddr *) &name, sizeof(name));
    if (c == -1) {
        error("Can't bind to socket");
    } else {
        printf("%s\n", "Bound to socket");
    }
}

ssize_t say(const int socket, const char *const s) {
    ssize_t result = send(socket, s, strlen(s), 0);
    if (result == -1) {
        fprintf(stderr, "%s\n", "An error occurred while sending the message");
    } else {
        printf("%s\n", "Sent the message");
    }
    return result;
}

void error(const char *const msg) {
    printf("%s\n", msg);
    exit(1);
}