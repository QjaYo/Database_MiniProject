#include "select_s4.h"
#include "ui_select_s4.h"
#include "global.h"
#include "func.h"
#include <QDebug>

select_s4::select_s4(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::select_s4)
{
    ui->setupUi(this);
    loadCategories();
    on_comboBox_category_currentTextChanged();
}

select_s4::~select_s4()
{
    delete ui;
}

void select_s4::loadCategories()
{
    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString query = "SELECT CategoryName FROM CATEGORIES ORDER BY CategoryName;";
    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLCHAR buffer[64];
        while (SQLFetch(hStmt) != SQL_NO_DATA)
        {
            SQLGetData(hStmt, 1, SQL_C_CHAR, buffer, sizeof(buffer), NULL);
            ui->comboBox_category->addItem(QString::fromLocal8Bit((char*)buffer));
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void select_s4::on_comboBox_category_currentTextChanged()
{
    ui->comboBox_item->clear();

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString categoryName = ui->comboBox_category->currentText();

    QString query = QString(
                        "SELECT ItemName FROM ITEMS "
                        "WHERE CategoryNo IN ("
                        "    SELECT CategoryNo FROM CATEGORIES "
                        "    WHERE CategoryNo = (SELECT CategoryNo FROM CATEGORIES WHERE CategoryName = '%1') "
                        "       OR ParentCategoryNo = (SELECT CategoryNo FROM CATEGORIES WHERE CategoryName = '%1'))"
                        ).arg(categoryName);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLCHAR buffer[64];
        while (SQLFetch(hStmt) != SQL_NO_DATA)
        {
            SQLGetData(hStmt, 1, SQL_C_CHAR, buffer, sizeof(buffer), NULL);
            ui->comboBox_item->addItem(QString::fromLocal8Bit((char*)buffer));
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void select_s4::on_pushButton_search_clicked()
{
    QString itemName = ui->comboBox_item->currentText();
    if (itemName.isEmpty())
        return;

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return;

    QString query = QString(
                        "SELECT U.UserName, I.ItemName, R.ReviewDate, R.Content, R.Rating "
                        "FROM USERS U, ITEMS I, REVIEWS R "
                        "WHERE R.UserNo = U.UserNo AND R.ItemNo = I.ItemNo AND I.ItemName = '%1'"
                        ).arg(itemName);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLSMALLINT colCount;
    SQLNumResultCols(hStmt, &colCount);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(colCount);

    int ratingCol = -1;
    for (int i = 0; i < colCount; i++)
    {
        SQLCHAR colName[128];
        SQLSMALLINT nameLen;
        SQLDescribeCol(hStmt, i + 1, colName, sizeof(colName), &nameLen, NULL, NULL, NULL, NULL);
        QString colStr = QString::fromLocal8Bit((char*)colName);
        if (colStr.toLower() == "rating")
            ratingCol = i;

        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromLocal8Bit((char*)colName)));
    }

    int row = 0;
    double avg = 0.0;

    while (SQLFetch(hStmt) != SQL_NO_DATA)
    {
        ui->tableWidget->insertRow(row);

        for (SQLSMALLINT col = 0; col < colCount; col++)
        {
            SQLCHAR buffer[256];
            SQLGetData(hStmt, col + 1, SQL_C_CHAR, buffer, sizeof(buffer), NULL);

            QString value = QString::fromLocal8Bit((char*)buffer);
            if (col == ratingCol)
            {
                double rating = value.toDouble();
                avg += rating;
            }

            ui->tableWidget->setItem(row, col, new QTableWidgetItem(value));
        }

        row++;
    }

    if (row > 0 && ratingCol != -1)
        avg /= row;
    else
        avg = 0.0;

    ui->textEdit_avgRate->setPlainText(QString::number(avg, 'f', 2));

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
