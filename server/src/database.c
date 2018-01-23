#include <string.h>
#include "database.h"

const char *const host = "localhost";
const char *const user = "root";
const char *const password = "root";
const char *const database = "chat";

const unsigned int database_port = 3306;
const char *const unix_socket = NULL;
const unsigned int flag = 0;


MYSQL *connect_to_database() {
    MYSQL *conn;
    MYSQL_RES *res;

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, host, user, password, database, database_port, unix_socket, flag)) {
        fprintf(stderr,"Error: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
        exit(1);
    } else {
        printf("%s\n","Connection to db successful");
        res = mysql_store_result(conn);
        mysql_free_result(res);
        return conn;
    }
}

void print_database_info() {
    printf("MySQL client version: %s\n", mysql_get_client_info());
}

int create_messages_table(MYSQL *const conn) {
    return mysql_query(conn, "create table messages("
            "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
            "message TEXT,"
            "user_id INT,"
            "FOREIGN KEY (user_id)"
            "        REFERENCES users(id)"
            "        ON DELETE CASCADE);"
    );
}

int create_users_table(MYSQL *const conn) {
    return mysql_query(conn, "create table users("
            "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
            "name VARCHAR(100) NOT NULL UNIQUE);"
    );
}

int get_all_messages_with_author(MYSQL *const conn) {
    return mysql_query(conn, "select messages.message, users.name "
            "from messages "
            "inner join users on messages.user_id=users.id");
}

int save_message(const char *const buf, MYSQL *const conn, const char *const id) {
    const char *const insert_query = "insert into messages (message, user_id) values(\"";
    char *const query = calloc((strlen(insert_query) + strlen(buf) + 5) * sizeof(void *), 1);
    if (!query) {
        fprintf(stderr,"%s\n","Allocation in save_message failure");
        return 0;
    } else {
        strcat(query, insert_query);
        strcat(query, buf);
        strcat(query, "\", ");
        strcat(query, id);
        strcat(query, ");");
        printf("%s\n", query);
        const int result = mysql_query(conn, query);
        free(query);
        return result;
    }
}

int select_user_by_name(const char *const buf, MYSQL *const conn) {
    const char *select_one = "select id from users where name=\"";
    char *query_by_name = malloc(strlen(select_one) + strlen(buf) + 3);

    if (!query_by_name) {
        fprintf(stderr,"%s\n","Allocation in select_user_by_name failure");
        return 0;
    } else {
        strcat(query_by_name, select_one);
        strcat(query_by_name, buf);
        strcat(query_by_name, "\";");
        printf("%s\n", query_by_name);
        const int res = mysql_query(conn, query_by_name);
        free(query_by_name);
        return res;
    }
}

int create_new_user(const char *const buf, MYSQL *const conn) {
    const char *insert_query = "insert into users (name) values(\"";
    char *query = calloc(strlen(insert_query) + strlen(buf) + 3, 1);
    if (!query) {
        fprintf(stderr,"%s\n","Allocation in create_new_user failure");
        return 0;
    } else {
        strcat(query, insert_query);
        strcat(query, buf);
        strcat(query, "\");");
        printf("%s\n", query);
        const int res = mysql_query(conn, query);
        free(query);
        return res;
    }
}

void create_tables(MYSQL *const conn) {
    MYSQL_RES *res;
    if (create_users_table(conn)) {
        fprintf(stderr, "Error: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
    } else {
        printf("Users table created\n");
        res = mysql_store_result(conn);
        mysql_free_result(res);
    }

    if (create_messages_table(conn)) {
        fprintf(stderr,"Error: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
    } else {
        printf("Messages table created\n");
        res = mysql_store_result(conn);
        mysql_free_result(res);
    }
}

