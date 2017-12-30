
#include "server/src/server.h"

void main() {
    MYSQL *conn;

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, host, user, password, database, database_port, unix_socket, flag)) {
        printf("Can't connect to db\n");
    } else {
        printf("Connection to db successful\n");
    }
}