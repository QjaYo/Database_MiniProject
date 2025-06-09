#include "delete_s2.h"
#include "ui_delete_s2.h"
#include "global.h"
#include "func.h"
#include "loginwindow.h"
#include "select_s3.h"
#include <QMessageBox>
#include <QDate>

delete_s2::delete_s2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::delete_s2)
{
    ui->setupUi(this);

    for (int y = 2024; y <= 2026; ++y)
        ui->comboBox_year->addItem(QString::number(y));

    for (int m = 1; m <= 12; ++m)
        ui->comboBox_month->addItem(QString("%1").arg(m, 2, 10, QChar('0')));

    for (int d = 1; d <= 31; ++d)
        ui->comboBox_day->addItem(QString("%1").arg(d, 2, 10, QChar('0')));
}

delete_s2::~delete_s2()
{
    delete ui;
}

void delete_s2::on_btn_userNo_clicked()
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

void delete_s2::on_btn_itemNo_clicked()
{
    select_s3 dialog(this);
    dialog.exec();

    QString itemNo = dialog.getSelectedItemNo();
    ui->textEdit_itemNo->setPlainText(itemNo);
}

void delete_s2::on_btn_search_clicked()
{
    QString userNo = ui->textEdit_userNo->toPlainText();
    QString itemNo = ui->textEdit_itemNo->toPlainText();

    if (userNo.isEmpty() || itemNo.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please check both UserNo and ItemNo.");
        return;
    }

    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    QString query = QString(
                        "SELECT UserNo, ItemNo, ReviewDate, Content, Rating "
                        "FROM REVIEWS "
                        "WHERE UserNo = %1 AND ItemNo = %2")
                        .arg(userNo).arg(itemNo);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels({"UserNo", "ItemNo", "ReviewDate", "Content", "Rating"});

    int row = 0;
    while (SQLFetch(hStmt) == SQL_SUCCESS)
    {
        ui->tableWidget->insertRow(row);

        int userNoVal, itemNoVal, rating;
        char rawDate[20], content[101];

        SQLGetData(hStmt, 1, SQL_C_SLONG, &userNoVal, 0, nullptr);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(userNoVal)));

        SQLGetData(hStmt, 2, SQL_C_SLONG, &itemNoVal, 0, nullptr);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(itemNoVal)));

        SQLGetData(hStmt, 3, SQL_C_CHAR, rawDate, sizeof(rawDate), nullptr);
        QString formattedDate = QDate::fromString(QString::fromUtf8(rawDate), "yyyy-M-d").toString("yyyy-MM-dd");
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(formattedDate));

        SQLGetData(hStmt, 4, SQL_C_CHAR, content, sizeof(content), nullptr);
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromUtf8(content)));

        SQLGetData(hStmt, 5, SQL_C_SLONG, &rating, 0, nullptr);
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(rating)));

        row++;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void delete_s2::on_btn_delete_clicked()
{
    QString userNo = ui->textEdit_userNo->toPlainText();
    QString itemNo = ui->textEdit_itemNo->toPlainText();

    QString year = ui->comboBox_year->currentText();
    QString month = ui->comboBox_month->currentText();
    QString day = ui->comboBox_day->currentText();

    if (userNo.isEmpty() || itemNo.isEmpty() || year.isEmpty() || month.isEmpty() || day.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please fill in UserNo, ItemNo, and full ReviewDate.");
        return;
    }

    QString reviewDate = QString("%1-%2-%3")
                             .arg(year)
                             .arg(month.rightJustified(2, '0'))
                             .arg(day.rightJustified(2, '0'));

    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    QString query = QString(
                        "DELETE FROM REVIEWS "
                        "WHERE UserNo = %1 AND ItemNo = %2 AND ReviewDate = '%3'")
                        .arg(userNo)
                        .arg(itemNo)
                        .arg(reviewDate);

    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::information(this, "Success", "Review deleted successfully.");
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to delete the review.");
    }
}
