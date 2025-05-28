#include "custom_query.h"
#include "ui_custom_query.h"
#include "global.h"
#include "func.h"

#include <QMessageBox>

custom_query::custom_query(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::custom_query)
{
    ui->setupUi(this);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

custom_query::~custom_query()
{
    delete ui;
}

void custom_query::on_query_btn_clicked()
{
    QString queryStr = ui->textEdit->toPlainText().trimmed();
    if (queryStr.isEmpty()) return;

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hDbc);
        return;
    }

    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)queryStr.toLocal8Bit().constData(), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "SQL Error", "Wrong query.");
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLSMALLINT colCount = 0;
    SQLNumResultCols(hStmt, &colCount);

    if (colCount <= 0) {
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(0);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(colCount);

    for (SQLSMALLINT i = 0; i < colCount; ++i) {
        SQLCHAR colName[128];
        SQLSMALLINT nameLen;
        SQLDescribeCol(
            hStmt,
            i + 1,
            colName,
            sizeof(colName),
            &nameLen,
            nullptr, nullptr, nullptr, nullptr
            );
        ui->tableWidget->setHorizontalHeaderItem(
            i,
            new QTableWidgetItem(QString::fromLocal8Bit((char*)colName))
            );
    }

    int row = 0;
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        ui->tableWidget->insertRow(row);
        for (SQLSMALLINT col = 0; col < colCount; ++col) {
            SQLCHAR buffer[256];
            SQLGetData(
                hStmt,
                col + 1,
                SQL_C_CHAR,
                buffer,
                sizeof(buffer),
                nullptr
                );
            ui->tableWidget->setItem(
                row, col,
                new QTableWidgetItem(QString::fromLocal8Bit((char*)buffer))
                );
        }
        ++row;
    }

    SQLCloseCursor(hStmt);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
