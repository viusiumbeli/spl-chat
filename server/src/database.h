#ifndef SPL_CHAT_DATABSE_H
#define SPL_CHAT_DATABSE_H

#include <mysql/mysql.h>
#include <stdio.h>

const static char *host = "localhost";
const static char *user = "root";
const static char *password = "root";
const static char *database = "chat";

const static unsigned int database_port = 3306;
const static char *unix_socket = NULL;
const static unsigned int flag = 0;

MYSQL *connect_to_database();

#endif //SPL_CHAT_DATABSE_H
