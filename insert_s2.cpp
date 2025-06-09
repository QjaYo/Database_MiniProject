#include "insert_s2.h"
#include "ui_insert_s2.h"
#include "global.h"
#include "func.h"
#include <QMessageBox>
#include <QDebug>

insert_s2::insert_s2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::insert_s2)
{
    ui->setupUi(this);

    ui->comboBox_status->addItem("available", 1);
    ui->comboBox_status->addItem("unavailable", 0);

    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);
    QString query = "SELECT CategoryNo, CategoryName FROM CATEGORIES ORDER BY CategoryNo";

    if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        int catNo;
        char name[100];
        while (SQLFetch(stmt) == SQL_SUCCESS)
        {
            SQLGetData(stmt, 1, SQL_C_SLONG, &catNo, 0, nullptr);
            SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), nullptr);
            ui->comboBox_category->addItem(QString::fromUtf8(name), catNo);
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

insert_s2::~insert_s2()
{
    delete ui;
}

void insert_s2::on_btn_confirm_clicked()
{
    QString name = ui->plainTextEdit->toPlainText().trimmed();
    QString stockStr = ui->plainTextEdit_2->toPlainText().trimmed();
    QString priceStr = ui->plainTextEdit_3->toPlainText().trimmed();
    int status = ui->comboBox_status->currentData().toInt();
    int categoryNo = ui->comboBox_category->currentData().toInt();

    if (name.isEmpty() || stockStr.isEmpty() || priceStr.isEmpty())
    {
        QMessageBox::warning(this, "Error", "All fields must be filled.");
        return;
    }

    if (name.length() > 20)
    {
        QMessageBox::warning(this, "Error", "Item name must be within 20 characters.");
        return;
    }

    bool ok1, ok2;
    int stock = stockStr.toInt(&ok1);
    int price = priceStr.toInt(&ok2);

    if (!ok1 || stock < 0 || stock > 99)
    {
        QMessageBox::warning(this, "Error", "Stock must be an integer between 0 and 99.");
        return;
    }

    if (!ok2 || price < 0 || price > 10000000)
    {
        QMessageBox::warning(this, "Error", "Price must be an integer between 0 and 10,000,000.");
        return;
    }

    int newItemNo = 0;
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    QString idQuery = "SELECT ISNULL(MAX(ItemNo), 0) + 1 FROM ITEMS";
    if (SQLExecDirect(stmt, (SQLCHAR*)idQuery.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS && SQLFetch(stmt) == SQL_SUCCESS)
    {
        SQLGetData(stmt, 1, SQL_C_SLONG, &newItemNo, 0, nullptr);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to generate ItemNo.");
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    name.replace("'", "''");

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);
    QString insertQuery = QString(
                              "INSERT INTO ITEMS (ItemNo, ItemName, ItemStock, ItemStatus, CategoryNo, ItemPrice) "
                              "VALUES (%1, '%2', %3, %4, %5, %6)"
                              ).arg(newItemNo).arg(name).arg(stock).arg(status).arg(categoryNo).arg(price);

    if (SQLExecDirect(stmt, (SQLCHAR*)insertQuery.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        QMessageBox::information(this, "Success", "Item inserted successfully.");
        ui->plainTextEdit->clear();
        ui->plainTextEdit_2->clear();
        ui->plainTextEdit_3->clear();
        ui->comboBox_status->setCurrentIndex(0);
        ui->comboBox_category->setCurrentIndex(0);
    }
    else
    {
        SQLCHAR sqlState[6], message[256];
        SQLINTEGER nativeError;
        SQLSMALLINT textLength;
        SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, sqlState, &nativeError, message, sizeof(message), &textLength);
        qDebug() << "ODBC Error:" << sqlState << "-" << (char*)message;

        QMessageBox::warning(this, "Error", "Failed to insert item.");
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}
