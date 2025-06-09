#include "delete_s4.h"
#include "ui_delete_s4.h"
#include "global.h"
#include "func.h"
#include "loginwindow.h"
#include "select_s3.h"
#include <QMessageBox>

delete_s4::delete_s4(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::delete_s4)
{
    ui->setupUi(this);
}

delete_s4::~delete_s4()
{
    delete ui;
}

void delete_s4::on_btn_itemNo_clicked()
{
    select_s3 dialog(this);
    dialog.exec();

    QString itemNo = dialog.getSelectedItemNo();
    ui->textEdit_itemNo->setPlainText(itemNo);
}

void delete_s4::on_btn_delete_clicked()
{
    QString itemNo = ui->textEdit_itemNo->toPlainText();

    if (itemNo.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please enter or select an ItemNo.");
        return;
    }

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        QMessageBox::critical(this, "Error", "Failed to allocate SQL handle.");
        return;
    }

    QString deleteQuery = QString("DELETE FROM ITEMS WHERE ItemNo = %1").arg(itemNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)deleteQuery.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::information(this, "Success", "Item deleted successfully.");
        close();
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to delete the item.");
    }
}
