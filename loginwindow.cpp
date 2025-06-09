#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QMessageBox>
#include "global.h"
#include "func.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_btn_login_clicked()
{
    QString id = ui->textEdit_id->toPlainText();
    QString pw = ui->textEdit_pw->toPlainText();

    if (id.isEmpty() || pw.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please enter ID and password.");
        return;
    }

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString query = QString("SELECT UserNo FROM USERS WHERE ID = '%1' AND PW = '%2'")
                        .arg(id).arg(pw);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS &&
        SQLFetch(hStmt) == SQL_SUCCESS)
    {
        SQLGetData(hStmt, 1, SQL_C_SLONG, &userNo, 0, nullptr);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        accept();
    }
    else
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::warning(this, "Login Failed", "ID or password incorrect.");
    }
}

int LoginWindow::getUserNo() const
{
    return userNo;
}
