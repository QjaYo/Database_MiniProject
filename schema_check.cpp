#include "schema_check.h"
#include "ui_schema_check.h"
#include "global.h"
#include "func.h"

#include <QDebug>

schema_check::schema_check(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::schema_check)
{
    ui->setupUi(this);
    loadTableList();
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &schema_check::loadTableSchema);
    if (ui->comboBox->count() > 0)
    {
        loadTableSchema(ui->comboBox->currentText());
    }
}

schema_check::~schema_check()
{
    delete ui;
}

void schema_check::loadTableList()
{
    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLRETURN ret = SQLTables(hStmt, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR*)"TABLE", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        while (SQLFetch(hStmt) != SQL_NO_DATA)
        {
            SQLCHAR tableName[256];
            SQLCHAR schemaName[256];

            SQLGetData(hStmt, 2, SQL_C_CHAR, schemaName, sizeof(schemaName), NULL);
            SQLGetData(hStmt, 3, SQL_C_CHAR, tableName, sizeof(tableName), NULL);

            QString schemaStr = QString::fromLocal8Bit((char*)schemaName);
            QString tableStr = QString::fromLocal8Bit((char*)tableName);

            if (schemaStr.toLower() == "dbo" && !tableStr.startsWith("trace_xe"))
            {
                ui->comboBox->addItem(tableStr);
            }
        }
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void schema_check::loadTableSchema(const QString &tableName)
{
    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    QString query = "SELECT * FROM " + tableName;

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLSMALLINT numCols = -1;
        SQLCHAR columnName[50][31];
        SQLSMALLINT columnNameLen[50];
        SQLSMALLINT columnDataType[50];
        SQLULEN columnDataSize[50];
        SQLSMALLINT columnDataDecimalDigits[50];
        SQLSMALLINT columnDataNullable[50];

        SQLNumResultCols(hStmt, &numCols);

        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(5);
        ui->tableWidget->setHorizontalHeaderLabels(
            {"Column Name", "Name Len", "SQL Type", "Data Size", "Nullable"});
        ui->tableWidget->setRowCount(numCols);

        for (int i = 0; i < numCols; ++i)
        {
            SQLDescribeCol(hStmt, i + 1, columnName[i], 30, &columnNameLen[i],
                           &columnDataType[i], &columnDataSize[i],
                           &columnDataDecimalDigits[i], &columnDataNullable[i]);

            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromLocal8Bit((char*)columnName[i])));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(columnNameLen[i])));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromUtf8(get_data_type_in_string(columnDataType[i]))));
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(columnDataSize[i])));
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem(columnDataNullable[i] == SQL_NULLABLE ? "NULL" : "NOT NULL"));
        }

        SQLCloseCursor(hStmt);
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
