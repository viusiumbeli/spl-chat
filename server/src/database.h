#ifndef SPL_CHAT_DATABSE_H
#define SPL_CHAT_DATABSE_H

#include <mysql/mysql.h>
#include <stdio.h>
//#include <mysql/my_global.h>
#include <mysql/mysql.h>

const static char *host = "localhost";
const static char *user = "root";
const static char *password = "root";
const static char *database = "chat";

const static unsigned int database_port = 3306;
const static char *unix_socket = NULL;
const static unsigned int flag = 0;

MYSQL *connect_to_database();

int create_messages_table(MYSQL *conn);

void print_database_info();

int get_all_messages_with_author(MYSQL *conn);

int save_message(char *buf, MYSQL *conn, char *id);

int create_users_table(MYSQL *conn);

int select_user_by_name(char *buf, MYSQL *conn);

int create_new_user (char *buf, MYSQL *conn);

void create_tables(MYSQL *conn);


#endif //SPL_CHAT_DATABSE_H
