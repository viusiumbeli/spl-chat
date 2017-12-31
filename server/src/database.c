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
            "message TEXT,"
            "user_id INT,"
            "FOREIGN KEY (user_id)\n"
            "        REFERENCES users(id)\n"
            "        ON DELETE CASCADE);"
    );
}

int create_users_table(MYSQL *conn) {
    return mysql_query(conn, "create table users("
            "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
            "name VARCHAR(100) NOT NULL UNIQUE);"
    );
}

int get_all_messages_with_author(MYSQL *conn) {
    return mysql_query(conn, "select messages.message, users.name "
            "from messages "
            "inner join users on messages.user_id=users.id");
}

int save_message(char *buf, MYSQL *conn, char *id) {
    const char *insert_query = "insert into messages (message, user_id) values(\"";
    char *query = malloc(strlen(insert_query) + strlen(buf) + 5);
    strcat(query, insert_query);
    strcat(query, buf);
    strcat(query, "\", ");
    strcat(query, id);
    strcat(query, ");");
    printf("%s\n", query);
    int result = mysql_query(conn, query);
    free(query);
    return result;
}

int select_user_by_name(char *buf, MYSQL *conn) {
    const char *select_one = "select id from users where name=\"";
    char *query_by_name = malloc(strlen(select_one) + strlen(buf) + 3);

    if (!query_by_name) {
        printf("Allocation in select_user_by_name failure\n");
        return 0;
    } else {
        strcat(query_by_name, select_one);
        strcat(query_by_name, buf);
        strcat(query_by_name, "\";");
        printf("%s\n", query_by_name);
        int res = mysql_query(conn, query_by_name);
        free(query_by_name);
        return res;
    }
}

int create_new_user(char *buf, MYSQL *conn) {
    const char *insert_query = "insert into users (name) values(\"";
    char *query = calloc(strlen(insert_query) + strlen(buf) + 3, 1);
    if (!query) {
        printf("Allocation in create_new_user failure\n");
    } else {
        strcat(query, insert_query);
        strcat(query, buf);
        strcat(query, "\");");
        printf("%s\n", query);
        int res = mysql_query(conn, query);
        free(query);
        return res;
    }
}

void create_tables(MYSQL *conn) {
    MYSQL_RES *res;
    if (!create_messages_table(conn)) {
        printf("Error: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
    } else {
        printf("Messages table created\n");
        res = mysql_store_result(conn);
        mysql_free_result(res);
    }

    if (!create_users_table(conn)) {
        printf("Error: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
    } else {
        printf("Users table created\n");
        res = mysql_store_result(conn);
        mysql_free_result(res);
    }
}

