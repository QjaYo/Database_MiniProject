#include "update_s3.h"
#include "ui_update_s3.h"
#include "global.h"
#include "func.h"
#include "select_s3.h"
#include <QMessageBox>

update_s3::update_s3(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::update_s3)
{
    ui->setupUi(this);
}

update_s3::~update_s3()
{
    delete ui;
}

void update_s3::on_btn_itemNo_clicked()
{
    select_s3 dialog(this);
    dialog.exec();

    QString itemNo = dialog.getSelectedItemNo();
    ui->textEdit_itemNo->setPlainText(itemNo);
}

void update_s3::on_btn_update_clicked()
{
    QString itemNo = ui->textEdit_itemNo->toPlainText();
    QString stockStr = ui->textEdit_stock->toPlainText();
    QString status = ui->textEdit_status->toPlainText().trimmed();
    QString priceStr = ui->textEdit_price->toPlainText();

    if (itemNo.isEmpty() || stockStr.isEmpty() || status.isEmpty() || priceStr.isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "All fields must be filled.");
        return;
    }

    bool ok1, ok2;
    int stock = stockStr.toInt(&ok1);
    int price = priceStr.toInt(&ok2);

    if (!ok1 || !ok2 || stock < 0 || price < 0)
    {
        QMessageBox::warning(this, "Input Error", "Stock and Price must be non-negative integers.");
        return;
    }

    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    QString query = QString(
                        "UPDATE ITEMS SET ItemStock = %1, ItemStatus = '%2', ItemPrice = %3 "
                        "WHERE ItemNo = %4"
                        ).arg(stock).arg(status).arg(price).arg(itemNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLLEN affected;
        SQLRowCount(hStmt, &affected);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        if (affected > 0)
        {
            QMessageBox::information(this, "Success", "Item updated successfully.");
        }
        else
        {
            QMessageBox::information(this, "No Changes", "No item was updated. Check ItemNo.");
        }
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to update item.");
    }
}
