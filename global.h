#ifndef GLOBAL_H
#define GLOBAL_H

#undef UNICODE
#undef _UNICODE
#pragma comment(lib, "odbc32.lib")

#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

extern const char* dataSourceName;
extern const char* serverId;
extern const char* serverPw;

extern SQLHENV hEnv;
extern SQLHDBC hDbc;

#endif // GLOBAL_H
