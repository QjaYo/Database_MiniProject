#include "delete_query.h"
#include "ui_delete_query.h"
#include "global.h"
#include "func.h"
#include "loginwindow.h"
#include <QMessageBox>
#include "delete_s1.h"
#include "delete_s2.h"
#include "delete_s3.h"
#include "delete_s4.h"

delete_query::delete_query(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::delete_query)
{
    ui->setupUi(this);
}

delete_query::~delete_query()
{
    delete ui;
}

void delete_query::on_delete_s1_clicked()
{
    LoginWindow login(this);
    if (login.exec() == QDialog::Accepted)
    {
        int userNo = login.getUserNo();

        SQLHSTMT hStmt;
        if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        {
            QMessageBox::critical(this, "Error", "Failed to allocate statement handle.");
            return;
        }

        QString query = QString("DELETE FROM USERS WHERE UserNo = %1").arg(userNo);

        if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
        {
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            QMessageBox::information(this, "Success", "Your account has been successfully deleted.");
        }
        else
        {
            print_error(SQL_HANDLE_STMT, hStmt);
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            QMessageBox::critical(this, "Error", "Failed to delete account.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Cancelled", "Login cancelled or failed.");
    }
}


void delete_query::on_delete_s2_clicked()
{

    delete_s2 dialog(this);
    dialog.exec();
}


void delete_query::on_delete_s3_clicked()
{
    delete_s3 dialog(this);
    dialog.exec();
}


void delete_query::on_delete_s4_clicked()
{
    delete_s4 dialog(this);
    dialog.exec();
}

