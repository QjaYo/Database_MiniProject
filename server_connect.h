#ifndef SERVER_CONNECT_H
#define SERVER_CONNECT_H

#include "global.h"

bool db_connect();
void db_disconnect();
void print_error(SQLSMALLINT handleType, SQLHANDLE handle);

#endif // SERVER_CONNECT_H
