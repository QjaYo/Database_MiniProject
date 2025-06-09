#include "insert_s3.h"
#include "ui_insert_s3.h"
#include "global.h"
#include "func.h"
#include <QMessageBox>

insert_s3::insert_s3(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::insert_s3)
{
    ui->setupUi(this);
    ui->checkBox->setEnabled(false);
}

insert_s3::~insert_s3()
{
    delete ui;
}

void insert_s3::on_btn_duplicateCheck_clicked()
{
    QString id = ui->textEdit_id->toPlainText();
    if (id.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please enter an ID.");
        return;
    }

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString query = QString("SELECT ID FROM USERS WHERE ID = '%1'").arg(id);
    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    if (SQLFetch(hStmt) == SQL_SUCCESS)
    {
        QMessageBox::warning(this, "Warning", "This ID is already taken.");
        ui->checkBox->setChecked(false);
    }
    else
    {
        ui->checkBox->setChecked(true);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}


void insert_s3::on_btn_signUp_clicked()
{
    QString id = ui->textEdit_id->toPlainText();
    QString pw = ui->textEdit_pw->toPlainText();
    QString name = ui->textEdit_name->toPlainText();
    QString phone = ui->textEdit_phoneNo_0->toPlainText();

    if (id.isEmpty() || pw.isEmpty() || name.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Enter ID, PW and Name.");
        return;
    }

    if (!ui->checkBox->isChecked())
    {
        QMessageBox::warning(this, "Warning", "Please check for ID duplication first.");
        return;
    }

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    int userNo = 0;
    bool found = false;

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    while (userNo <= 99999999)
    {
        QString checkQuery = QString("SELECT UserNo FROM USERS WHERE UserNo = %1").arg(userNo);
        if (SQLExecDirect(hStmt, (SQLCHAR*)checkQuery.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
        {
            print_error(SQL_HANDLE_STMT, hStmt);
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLFetch(hStmt) != SQL_SUCCESS)
        {
            found = true;
            break;
        }

        SQLFreeStmt(hStmt, SQL_CLOSE);
        userNo++;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    if (!found)
    {
        QMessageBox::critical(this, "Error", "No available UserNo.");
        return;
    }

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString phone1 = ui->textEdit_phoneNo_0->toPlainText();
    QString phone2 = ui->textEdit_phoneNo_1->toPlainText();
    QString phone3 = ui->textEdit_phoneNo_2->toPlainText();

    QString phone_final = phone1.isEmpty() || phone2.isEmpty() || phone3.isEmpty()
                        ? "NULL"
                        : QString("'%1-%2-%3'").arg(phone1, phone2, phone3);

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString insertQuery = QString(
                              "INSERT INTO USERS (UserNo, UserName, PhoneNo, Balance, ID, PW) "
                              "VALUES (%1, '%2', %3, 0, '%4', '%5')")
                              .arg(userNo)
                              .arg(name)
                              .arg(phone_final)
                              .arg(id)
                              .arg(pw);

    if (SQLExecDirect(hStmt, (SQLCHAR*)insertQuery.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    QMessageBox::information(this, "Success", "Sign-up completed successfully.");
    close();
}


void insert_s3::on_textEdit_id_textChanged()
{
    ui->checkBox->setChecked(false);
}

