#ifndef SPL_CHAT_DATABSE_H
#define SPL_CHAT_DATABSE_H

#include <mysql/mysql.h>
#include <stdio.h>

MYSQL *connect_to_database();

int create_messages_table(MYSQL *conn);

void print_database_info();

int get_all_messages_with_author(MYSQL *conn);

int save_message(const char *buf, MYSQL *conn, const char *id);

int create_users_table(MYSQL *conn);

int select_user_by_name(const char *buf, MYSQL *conn);

int create_new_user(const char *buf, MYSQL *conn);

void create_tables(MYSQL *conn);


#endif //SPL_CHAT_DATABSE_H
