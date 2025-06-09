#include "select_s5.h"
#include "ui_select_s5.h"
#include "global.h"
#include "func.h"
#include <QDebug>

select_s5::select_s5(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::select_s5)
{
    ui->setupUi(this);
}

select_s5::~select_s5()
{
    delete ui;
}

void select_s5::on_pushButton_clicked()
{
    QString userName = ui->textEdit_userName->toPlainText();
    if (userName.isEmpty())
        return;

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString query = QString(
                        "SELECT * "
                        "FROM USERS "
                        "WHERE UserName = '%1';"
                        ).arg(userName);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLSMALLINT colCount;
    SQLNumResultCols(hStmt, &colCount);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(colCount);

    for (int i = 0; i < colCount; i++)
    {
        SQLCHAR colName[100];
        SQLSMALLINT nameLen;
        SQLDescribeCol(hStmt, i + 1, colName, sizeof(colName), &nameLen, NULL,NULL,NULL,NULL);
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromLocal8Bit((char*)colName)));
    }

    int row = 0;
    while (SQLFetch(hStmt) != SQL_NO_DATA)
    {
        ui->tableWidget->insertRow(row);
        for (SQLSMALLINT col = 0; col < colCount; col++)
        {
            SQLCHAR buffer[100];
            SQLGetData(hStmt, col + 1, SQL_C_CHAR, buffer, sizeof(buffer), NULL);
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString::fromLocal8Bit((char*)buffer)));
        }
        row++;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
