#include "select_s2.h"
#include "ui_select_s2.h"
#include "global.h"
#include "func.h"

select_s2::select_s2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::select_s2)
{
    ui->setupUi(this);
}

select_s2::~select_s2()
{
    delete ui;
}

void select_s2::on_btn_search_clicked()
{
    QString orderNo = ui->textEdit_orderNo->toPlainText().trimmed();
    if (orderNo.isEmpty()) return;

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) {
        print_error(SQL_HANDLE_STMT, hDbc);
        return;
    }

    QString query = QString(
                        "SELECT * FROM DELIVERIES D, ORDER_ITEM_DELIVERY OID "
                        "WHERE OID.OrderNo = %1 AND OID.DeliveryNo = D.DeliveryNo").arg(orderNo);

    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLSMALLINT colCount = 0;
    SQLNumResultCols(hStmt, &colCount);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(colCount);

    for (int i = 0; i < colCount; ++i) {
        SQLCHAR colName[128];
        SQLSMALLINT nameLen;
        SQLDescribeCol(hStmt, i + 1, colName, sizeof(colName), &nameLen, NULL, NULL, NULL, NULL);
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromLocal8Bit((char*)colName)));
    }

    int row = 0;
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        ui->tableWidget->insertRow(row);
        for (SQLSMALLINT col = 0; col < colCount; ++col) {
            SQLCHAR buffer[256];
            SQLGetData(hStmt, col + 1, SQL_C_CHAR, buffer, sizeof(buffer), NULL);
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString::fromLocal8Bit((char*)buffer)));
        }
        ++row;
    }

    SQLCloseCursor(hStmt);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

