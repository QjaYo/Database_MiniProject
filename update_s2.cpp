#include "update_s2.h"
#include "ui_update_s2.h"
#include "global.h"
#include "func.h"
#include <QMessageBox>

update_s2::update_s2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::update_s2)
{
    ui->setupUi(this);

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
    {
        QString query = "SELECT CategoryNo, CategoryName FROM CATEGORIES";
        if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
        {
            int catNo;
            char catName[21];
            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &catNo, 0, nullptr);
                SQLGetData(hStmt, 2, SQL_C_CHAR, catName, sizeof(catName), nullptr);
                QString catNameStr = QString::fromUtf8(catName).trimmed();
                ui->comboBox->addItem(catNameStr, catNo);
            }
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
}

update_s2::~update_s2()
{
    delete ui;
}

void update_s2::on_btn_discount_clicked()
{
    QVariant categoryData = ui->comboBox->currentData();
    if (!categoryData.isValid())
    {
        QMessageBox::warning(this, "Input Error", "Please select a valid category.");
        return;
    }

    QString discountStr = ui->textEdit_discount->toPlainText();
    bool ok;
    int discount = discountStr.toInt(&ok);
    if (!ok || discount < 1 || discount > 99)
    {
        QMessageBox::warning(this, "Input Error", "Please enter a discount between 1 and 99.");
        return;
    }

    int catNo = categoryData.toInt();

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        QMessageBox::critical(this, "Error", "Failed to allocate SQL handle.");
        return;
    }

    QString updateQuery = QString(
                              "UPDATE ITEMS SET ItemPrice = CAST(ItemPrice * ((100.0 - %1) / 100.0) AS INT) "
                              "WHERE CategoryNo = %2"
                              ).arg(discount).arg(catNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLLEN affected = 0;
        SQLRowCount(hStmt, &affected);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        if (affected > 0)
        {
            QMessageBox::information(this, "Success", QString("Discount applied to %1 items.").arg(affected));
        }
        else
        {
            QMessageBox::information(this, "No Changes", "No items matched the selected category.");
        }
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to apply discount.");
    }
}
