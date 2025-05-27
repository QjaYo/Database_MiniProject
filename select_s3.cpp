#include "select_s3.h"
#include "ui_select_s3.h"
#include "global.h"
#include "func.h"
#include <QMessageBox>
#include <QQueue>
#include <QSet>

select_s3::select_s3(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::select_s3)
{
    ui->setupUi(this);

    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    QString query = "SELECT CategoryName FROM CATEGORIES ORDER BY CategoryNo";
    if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS) {
        SQLCHAR name[100];
        while (SQLFetch(stmt) == SQL_SUCCESS) {
            SQLGetData(stmt, 1, SQL_C_CHAR, name, sizeof(name), nullptr);
            ui->comboBox->addItem(QString::fromUtf8((char*)name));
        }
    } else {
        QMessageBox::warning(this, "Error", "Failed to load categories.");
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

select_s3::~select_s3()
{
    delete ui;
}

void select_s3::on_btn_search_clicked()
{
    QString selectedCategory = ui->comboBox->currentText();

    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

    int rootCategory = -1;
    QString query = QString("SELECT CategoryNo FROM CATEGORIES WHERE CategoryName = '%1'").arg(selectedCategory);

    if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS && SQLFetch(stmt) == SQL_SUCCESS) {
        SQLGetData(stmt, 1, SQL_C_SLONG, &rootCategory, 0, nullptr);
    } else {
        QMessageBox::warning(this, "Error", "Failed to find selected category.");
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    QSet<int> categorySet = getAllCategoryNos(rootCategory);
    if (categorySet.isEmpty()) return;

    QStringList categoryList;
    for (int cat : categorySet)
        categoryList << QString::number(cat);

    QString inClause = categoryList.join(", ");
    query = QString(
                "SELECT ItemNo, ItemName, ItemStock, ItemStatus, CategoryNo, ItemPrice "
                "FROM ITEMS WHERE CategoryNo IN (%1)"
                ).arg(inClause);

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);
    if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) != SQL_SUCCESS) {
        QMessageBox::warning(this, "Error", "Failed to retrieve items.");
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels({"ItemNo", "ItemName", "Stock", "Status", "CategoryNo", "Price"});

    int row = 0;
    while (SQLFetch(stmt) == SQL_SUCCESS) {
        ui->tableWidget->insertRow(row);

        int intVal;
        char strVal[256];

        SQLGetData(stmt, 1, SQL_C_SLONG, &intVal, 0, nullptr);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(intVal)));

        SQLGetData(stmt, 2, SQL_C_CHAR, strVal, sizeof(strVal), nullptr);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(strVal)));

        SQLGetData(stmt, 3, SQL_C_SLONG, &intVal, 0, nullptr);
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(intVal)));

        int itemStatus;
        SQLGetData(stmt, 4, SQL_C_SLONG, &itemStatus, 0, nullptr);
        QString status = (itemStatus == 1) ? "Available" : "Unavailable";
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(status));

        SQLGetData(stmt, 5, SQL_C_SLONG, &intVal, 0, nullptr);
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(intVal)));

        SQLGetData(stmt, 6, SQL_C_SLONG, &intVal, 0, nullptr);
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(intVal)));

        row++;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}


QSet<int> select_s3::getAllCategoryNos(int rootCategoryNo)
{
    QSet<int> result;
    QQueue<int> queue;
    result.insert(rootCategoryNo);
    queue.enqueue(rootCategoryNo);

    while (!queue.isEmpty()) {
        int parent = queue.dequeue();

        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);

        QString query = QString("SELECT CategoryNo FROM CATEGORIES WHERE ParentCategoryNo = %1").arg(parent);
        if (SQLExecDirect(stmt, (SQLCHAR*)query.toUtf8().constData(), SQL_NTS) == SQL_SUCCESS) {
            int childNo;
            while (SQLFetch(stmt) == SQL_SUCCESS) {
                SQLGetData(stmt, 1, SQL_C_SLONG, &childNo, 0, nullptr);
                if (!result.contains(childNo)) {
                    result.insert(childNo);
                    queue.enqueue(childNo);
                }
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    }

    return result;
}

