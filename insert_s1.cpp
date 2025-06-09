#include "insert_s1.h"
#include "ui_insert_s1.h"
#include "global.h"
#include "func.h"
#include <QMessageBox>
#include <QDate>

insert_s1::insert_s1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::insert_s1)
{
    ui->setupUi(this);

    for (int i = 1; i <= 5; ++i)
    {
        ui->comboBox->addItem(QString::number(i));
    }
}

insert_s1::~insert_s1()
{
    delete ui;
}

void insert_s1::on_btn_search_clicked()
{
    QString orderNoStr = ui->textEdit_orderNo->toPlainText().trimmed();
    if (orderNoStr.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter OrderNo.");
        return;
    }

    int orderNo = orderNoStr.toInt();

    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    QString query = QString(
                        "SELECT I.ItemName "
                        "FROM ORDERS O "
                        "JOIN ORDER_ITEM_DELIVERY OID ON O.OrderNo = OID.OrderNo "
                        "JOIN ITEMS I ON OID.ItemNo = I.ItemNo "
                        "WHERE O.OrderNo = %1"
                        ).arg(orderNo);

    char itemName[256];
    if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS && SQLFetch(stmt) == SQL_SUCCESS)
    {
        SQLGetData(stmt, 1, SQL_C_CHAR, itemName, sizeof(itemName), nullptr);
        ui->textEdit_itemName->setPlainText(QString::fromUtf8(itemName));
    }
    else
    {
        QMessageBox::warning(this, "Error", "No such order number exists.");
        ui->textEdit_itemName->clear();
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}


void insert_s1::on_btn_confirm_clicked()
{
    QString orderNoStr = ui->textEdit_orderNo->toPlainText().trimmed();
    QString content = ui->textEdit_review->toPlainText().trimmed();
    int rating = ui->comboBox->currentText().toInt();

    if (orderNoStr.isEmpty() || content.isEmpty())
    {
        QMessageBox::warning(this, "Error", "OrderNo and Review content are required.");
        return;
    }

    if (content.length() > 100)
    {
        QMessageBox::warning(this, "Error", "Review content must be 100 characters or less.");
        return;
    }

    QString dateStr = QDate::currentDate().toString("yyyy-MM-dd");
    if (QDate::currentDate() <= QDate(2025, 5, 12))
    {
        QMessageBox::warning(this, "Error", "ReviewDate must be after 2025-05-12.");
        return;
    }

    int orderNo = orderNoStr.toInt();
    int userNo = -1, itemNo = -1;

    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    QString query = QString(
                        "SELECT O.UserNo, OID.ItemNo "
                        "FROM ORDERS O "
                        "JOIN ORDER_ITEM_DELIVERY OID ON O.OrderNo = OID.OrderNo "
                        "WHERE O.OrderNo = %1"
                        ).arg(orderNo);

    if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS && SQLFetch(stmt) == SQL_SUCCESS)
    {
        SQLGetData(stmt, 1, SQL_C_SLONG, &userNo, 0, nullptr);
        SQLGetData(stmt, 2, SQL_C_SLONG, &itemNo, 0, nullptr);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Invalid order number.");
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    content.replace("'", "''");

    query = QString(
                "INSERT INTO REVIEWS (UserNo, ItemNo, ReviewDate, Content, Rating) "
                "VALUES (%1, %2, '%3', '%4', %5)"
                ).arg(userNo).arg(itemNo).arg(dateStr).arg(content).arg(rating);

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        QMessageBox::information(this, "Success", "Review inserted successfully.");
        ui->textEdit_review->clear();
        ui->comboBox->setCurrentIndex(0);
    }
    else {
        SQLCHAR sqlState[6], message[256];
        SQLINTEGER nativeError;
        SQLSMALLINT textLength;
        SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, sqlState, &nativeError, message, sizeof(message), &textLength);
        qDebug() << "ODBC Error:" << sqlState << "-" << (char*)message;
        QMessageBox::warning(this, "Error", "Failed to insert review.");
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
