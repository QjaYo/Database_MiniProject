#ifndef FUNC_H
#define FUNC_H

#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

void print_error(SQLSMALLINT handleType, SQLHANDLE handle);
const char* get_data_type_in_string(SQLSMALLINT dataType);


#endif // FUNC_H
