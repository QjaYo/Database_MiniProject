#include "server_connect.h"
#include "global.h"
#include "func.h"

#include <QDebug>
#include <QMessageBox>

bool db_connect()
{
    SQLRETURN Ret;

    // Allocate the Environment Handle (hEnv)
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
    {
        qDebug() << "SQLAllocHandle(SQL_HANDLE_ENV) Error";
        return false;
    }

    // Set attributes of the Environment Handle (hEnv)
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
    {
        qDebug() << "SQLSetEnvAttr Error";
        return false;
    }

    // Allocate the Connection Handle (hDbc)
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
    {
        qDebug() << "SQLAllocHandle(SQL_HANDLE_DBC) Error";
        return false;
    }

    // Connect to the SQL Sever with ODBC name, ID, and PW
    Ret = SQLConnect(hDbc, (SQLCHAR*)dataSourceName, SQL_NTS, (SQLCHAR*)serverId, SQL_NTS, (SQLCHAR*)serverPw, SQL_NTS);

    if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
    {
        print_error(SQL_HANDLE_DBC, hDbc);
        return false;
    }

    return true;
}

void db_disconnect()
{
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}
