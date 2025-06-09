#include "update_s1.h"
#include "ui_update_s1.h"
#include "global.h"
#include "func.h"
#include "loginwindow.h"
#include <QMessageBox>

update_s1::update_s1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::update_s1)
{
    ui->setupUi(this);
}

update_s1::~update_s1()
{
    delete ui;
}

void update_s1::on_btn_userNo_clicked()
{
    LoginWindow login(this);
    if (login.exec() == QDialog::Accepted)
    {
        int userNo = login.getUserNo();
        ui->textEdit_userNo->setPlainText(QString::number(userNo));
    }
    else
    {
        QMessageBox::warning(this, "Login Failed", "Invalid ID or password.");
    }
}

void update_s1::on_textEdit_userNo_textChanged()
{
    QString userNo = ui->textEdit_userNo->toPlainText();

    if (userNo.isEmpty())
    {
        ui->textEdit_balance->clear();
        return;
    }

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString query = QString("SELECT Balance FROM USERS WHERE UserNo = %1").arg(userNo);
    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        ui->textEdit_balance->clear();
        return;
    }

    int balance = 0;
    if (SQLFetch(hStmt) == SQL_SUCCESS)
    {
        SQLGetData(hStmt, 1, SQL_C_SLONG, &balance, 0, nullptr);
        ui->textEdit_balance->setPlainText(QString::number(balance));
    }
    else
    {
        ui->textEdit_balance->setPlainText("Not Found");
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void update_s1::on_btn_recharge_clicked()
{
    QString userNo = ui->textEdit_userNo->toPlainText();
    QString amountStr = ui->textEdit_amount->toPlainText();

    if (userNo.isEmpty() || amountStr.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Please enter both UserNo and Amount.");
        return;
    }

    bool ok;
    int amount = amountStr.toInt(&ok);
    if (!ok || amount <= 0)
    {
        QMessageBox::warning(this, "Input Error", "Amount must be a positive integer.");
        return;
    }

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        QMessageBox::critical(this, "Error", "Failed to allocate SQL handle.");
        return;
    }

    QString selectQuery = QString("SELECT Balance FROM USERS WHERE UserNo = %1").arg(userNo);
    int currentBalance = 0;
    if (SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS &&
        SQLFetch(hStmt) == SQL_SUCCESS)
    {
        SQLGetData(hStmt, 1, SQL_C_SLONG, &currentBalance, 0, nullptr);
        SQLFreeStmt(hStmt, SQL_CLOSE);
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to fetch current balance.");
        return;
    }

    int newBalance = currentBalance + amount;
    if (newBalance > 2000000)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::warning(this, "Balance Limit", "Balance cannot exceed 2,000,000.");
        return;
    }

    QString updateQuery = QString("UPDATE USERS SET Balance = %1 WHERE UserNo = %2")
                              .arg(newBalance)
                              .arg(userNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        ui->textEdit_balance->setPlainText(QString::number(newBalance));
        QMessageBox::information(this, "Success", "Balance updated successfully.");
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to update balance.");
    }
}
