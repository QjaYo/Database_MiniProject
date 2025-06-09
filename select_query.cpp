#include "select_query.h"
#include "ui_select_query.h"
#include "select_s1.h"
#include "select_s2.h"
#include "select_s3.h"
#include "select_s4.h"
#include "select_s5.h"
#include "select_s6.h"
#include "global.h"
#include <QMessageBox>

select_query::select_query(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::select_query)
{
    ui->setupUi(this);
}

select_query::~select_query()
{
    delete ui;
}

void select_query::on_btn_s1_clicked()
{
    select_s1 dialog(this);
    dialog.exec();
}

void select_query::on_btn_s2_clicked()
{
    select_s2 dialog(this);
    dialog.exec();
}

void select_query::on_btn_s3_clicked()
{
    select_s3 dialog(this);
    dialog.exec();
}

void select_query::on_btn_s4_clicked()
{
    select_s4 dialog(this);
    dialog.exec();
}

void select_query::on_btn_s6_clicked()
{
    select_s5 dialog(this);
    dialog.exec();
}

void select_query::on_btn_s5_clicked()
{
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    QString query = "SELECT ItemName FROM ITEMS WHERE Stock = 1";

    QStringList lowStockItems;

    if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        char itemName[101];
        while (SQLFetch(stmt) == SQL_SUCCESS)
        {
            SQLGetData(stmt, 1, SQL_C_CHAR, itemName, sizeof(itemName), nullptr);
            lowStockItems << QString::fromUtf8(itemName);
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    if (lowStockItems.isEmpty())
    {
        QMessageBox::information(this, "Low Stock Items", "There are no items with stock = 1.");
    }
    else
    {
        QMessageBox::information(this, "Low Stock Items", "Items with stock = 1:\n\n" + lowStockItems.join("\n"));
    }
}
