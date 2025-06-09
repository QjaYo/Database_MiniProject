#include "delete_s3.h"
#include "ui_delete_s3.h"
#include "global.h"
#include "func.h"
#include <QMessageBox>
#include <QDate>

delete_s3::delete_s3(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::delete_s3)
{
    ui->setupUi(this);
}

delete_s3::~delete_s3()
{
    delete ui;
}

void delete_s3::on_btn_cancel_clicked()
{
    QString orderNoStr = ui->textEdit_orderNo->toPlainText();

    if (orderNoStr.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please enter OrderNo.");
        return;
    }

    int orderNo = orderNoStr.toInt();

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        QMessageBox::critical(this, "Error", "Failed to allocate statement handle.");
        return;
    }

    QString query = QString("SELECT OrderDate FROM ORDERS WHERE OrderNo = %1").arg(orderNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to retrieve order.");
        return;
    }

    char dateStr[11] = "";
    if (SQLFetch(hStmt) != SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::warning(this, "Not Found", "No order found with given OrderNo.");
        return;
    }

    SQLGetData(hStmt, 1, SQL_C_CHAR, dateStr, sizeof(dateStr), nullptr);
    SQLFreeStmt(hStmt, SQL_CLOSE);

    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QString orderDate = QString::fromUtf8(dateStr);

    if (orderDate != today)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::warning(this, "Warning", "Only cancellations on the day of order are allowed.");
        return;
    }

    QString deleteQuery = QString("DELETE FROM ORDERS WHERE OrderNo = %1").arg(orderNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)deleteQuery.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::information(this, "Success", "Order has been cancelled.");
        close();
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to cancel order.");
    }
}
