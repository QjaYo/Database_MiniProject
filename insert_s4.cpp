#include "insert_s4.h"
#include "ui_insert_s4.h"
#include "select_s3.h"
#include "global.h"
#include "func.h"
#include "loginwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QDebug>

insert_s4::insert_s4(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::insert_s4)
{
    ui->setupUi(this);
}

insert_s4::~insert_s4()
{
    delete ui;
}

void insert_s4::on_btn_itemNo_clicked()
{
    select_s3 dialog(this);
    dialog.exec();

    QString itemNo = dialog.getSelectedItemNo();
    ui->textEdit_itemNo->setPlainText(itemNo);
}

void insert_s4::on_btn_confirm_clicked()
{
    QString userNo = ui->textEdit_userNo->toPlainText();
    QString itemNo = ui->textEdit_itemNo->toPlainText();
    QString quantityStr = ui->textEdit_quantity->toPlainText();
    QString zip = ui->textEdit_zipdoe->toPlainText();

    if (userNo.isEmpty() || itemNo.isEmpty() || quantityStr.isEmpty() || zip.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "All fields must be filled.");
        return;
    }

    int quantity = quantityStr.toInt();
    int orderNo = 0;
    int deliveryNo = 0;

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString query0 = QString("SELECT ItemStock FROM ITEMS WHERE ItemNo = %1").arg(itemNo);
    if (SQLExecDirect(hStmt, (SQLCHAR*)query0.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    qDebug() << "0";

    int stock = 0;
    SQLBindCol(hStmt, 1, SQL_C_SLONG, &stock, 0, nullptr);
    if (SQLFetch(hStmt) != SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLFreeStmt(hStmt, SQL_CLOSE);

    if (quantity > stock)
    {
        QMessageBox::warning(this, "Unavailable", "Quantity exceeds available stock.");
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    while (true)
    {
        QString query1 = QString("SELECT OrderNo FROM ORDERS WHERE OrderNo = %1").arg(orderNo);
        if (SQLExecDirect(hStmt, (SQLCHAR*)query1.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
        {
            print_error(SQL_HANDLE_STMT, hStmt);
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLFetch(hStmt) != SQL_SUCCESS)
            break;

        SQLFreeStmt(hStmt, SQL_CLOSE);
        orderNo++;
    }

    qDebug() << "1";

    SQLFreeStmt(hStmt, SQL_CLOSE);

    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QString query2 = QString("SELECT ItemPrice FROM ITEMS WHERE ItemNo = %1").arg(itemNo);
    if (SQLExecDirect(hStmt, (SQLCHAR*)query2.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    qDebug() << "2";

    int price = 0;
    SQLBindCol(hStmt, 1, SQL_C_SLONG, &price, 0, nullptr);
    if (SQLFetch(hStmt) != SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLFreeStmt(hStmt, SQL_CLOSE);

    QString query3 = QString("INSERT INTO ORDERS VALUES (%1, '%2', %3, %4, %5)")
                              .arg(orderNo)
                              .arg(date)
                              .arg(price)
                              .arg(quantity)
                              .arg(userNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query3.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    qDebug() << "3";

    SQLFreeStmt(hStmt, SQL_CLOSE);

    while (true)
    {
        QString query4 = QString("SELECT DeliveryNo FROM DELIVERIES WHERE DeliveryNo = %1").arg(deliveryNo);
        if (SQLExecDirect(hStmt, (SQLCHAR*)query4.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
        {
            print_error(SQL_HANDLE_STMT, hStmt);
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLFetch(hStmt) != SQL_SUCCESS)
            break;

        SQLFreeStmt(hStmt, SQL_CLOSE);
        deliveryNo++;
    }

    qDebug() << "4";

    SQLFreeStmt(hStmt, SQL_CLOSE);

    QString query5 = QString("INSERT INTO DELIVERIES VALUES (%1, %2, 0, NULL)")
                                 .arg(deliveryNo)
                                 .arg(zip);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query5.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    qDebug() << "5";

    SQLFreeStmt(hStmt, SQL_CLOSE);

    QString query6 = QString("INSERT INTO ORDER_ITEM_DELIVERY VALUES (%1, %2, %3)")
                            .arg(orderNo)
                            .arg(deliveryNo)
                            .arg(itemNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query6.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    qDebug() << "6";

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    QMessageBox::information(this, "Success", "Order has been placed.");
    close();
}

void insert_s4::on_btn_userNo_clicked()
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
