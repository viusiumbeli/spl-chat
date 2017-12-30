
#include <string.h>
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

int get_all_rows(MYSQL *conn, char *table_name) {
    const char *select_all = "select * from ";
    size_t query_size = strlen(select_all) + strlen(table_name) + 2;
    char *query = malloc(query_size);
    snprintf(query, query_size, "%s%s;", select_all, table_name);
    printf("%s\n", query);
    return mysql_query(conn, query);
}

int save_message(char *buf, MYSQL *conn) {
    const char *insert_query = "insert into messages (message) values(\"";
    size_t query_size = strlen(insert_query) + strlen(buf);
    char *insert_query_with_msg = malloc(query_size);
    char *query = malloc(query_size+3);
    snprintf(insert_query_with_msg, query_size, "%s%s", insert_query, buf);
    snprintf(query, query_size+3, "%s\");", insert_query_with_msg);
    printf("%s\n", query);
    return mysql_query(conn, query);
}


void create_tables(MYSQL *conn) {
    if (!create_messages_table(conn)) {
        printf("Error: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
    } else {
        printf("Messages table created\n");
    }
}

