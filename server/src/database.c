
#include "database.h"

MYSQL *connect_to_database() {
    MYSQL *conn;

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, host, user, password, database, database_port, unix_socket, flag)) {
        printf("Error: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
        exit(1);
    } else {
        printf("Connection to db successful\n");
        return conn;
    }
}

void print_database_info() {
    printf("MySQL client version: %s\n", mysql_get_client_info());
}

int create_messages_table(MYSQL *conn) {
    return mysql_query(conn, "create table messages("
            "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
            "message TEXT);"
    );
}

void create_tables(MYSQL *conn) {
    if (!create_messages_table(conn)) {
        printf("Error: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
    } else {
        printf("Messages table created\n");
    }
}