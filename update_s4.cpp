#include "update_s4.h"
#include "ui_update_s4.h"
#include "global.h"
#include "func.h"
#include "loginwindow.h"
#include "select_s3.h"
#include <QMessageBox>
#include <QDate>

update_s4::update_s4(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::update_s4)
{
    ui->setupUi(this);

    for (int y = 2024; y <= 2026; ++y)
        ui->comboBox_year->addItem(QString::number(y));
    for (int m = 1; m <= 12; m++)
        ui->comboBox_month->addItem(QString("%1").arg(m, 2, 10, QChar('0')));
    for (int d = 1; d <= 31; d++)
        ui->comboBox_day->addItem(QString("%1").arg(d, 2, 10, QChar('0')));
}

update_s4::~update_s4()
{
    delete ui;
}

void update_s4::on_btn_userNo_clicked()
{
    LoginWindow login(this);
    if (login.exec() == QDialog::Accepted)
    {
        int userNo = login.getUserNo();
        ui->textEdit_userNo->setPlainText(QString::number(userNo));
    }
}

void update_s4::on_btn_itemNo_clicked()
{
    select_s3 dialog(this);
    dialog.exec();

    QString itemNo = dialog.getSelectedItemNo();
    ui->textEdit_itemNo->setPlainText(itemNo);
}

void update_s4::on_btn_search_clicked()
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
                        "WHERE UserNo = %1 AND ItemNo = %2"
                        ).arg(userNo).arg(itemNo);

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

void update_s4::on_btn_edit_clicked()
{
    QString year = ui->comboBox_year->currentText();
    QString month = ui->comboBox_month->currentText();
    QString day = ui->comboBox_day->currentText();
    QString selectedDate = QString("%1-%2-%3").arg(year).arg(month.rightJustified(2, '0')).arg(day.rightJustified(2, '0'));

    int rowCount = ui->tableWidget->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        QTableWidgetItem* dateItem = ui->tableWidget->item(i, 2);
        if (dateItem && dateItem->text() == selectedDate)
        {
            QTableWidgetItem* contentItem = ui->tableWidget->item(i, 3);
            QTableWidgetItem* ratingItem = ui->tableWidget->item(i, 4);

            ui->tableWidget_new->setRowCount(1);
            ui->tableWidget_new->setColumnCount(2);
            ui->tableWidget_new->setHorizontalHeaderLabels({"Content", "Rating"});
            ui->tableWidget_new->setItem(0, 0, new QTableWidgetItem(contentItem->text()));
            ui->tableWidget_new->setItem(0, 1, new QTableWidgetItem(ratingItem->text()));
            return;
        }
    }

    QMessageBox::warning(this, "Not Found", "No review found for the selected date.");
}

void update_s4::on_btn_update_clicked()
{
    QString userNo = ui->textEdit_userNo->toPlainText();
    QString itemNo = ui->textEdit_itemNo->toPlainText();

    QString year = ui->comboBox_year->currentText();
    QString month = ui->comboBox_month->currentText();
    QString day = ui->comboBox_day->currentText();

    QString reviewDate = QString("%1-%2-%3")
                             .arg(year).arg(month.rightJustified(2, '0')).arg(day.rightJustified(2, '0'));

    if (ui->tableWidget_new->rowCount() == 0)
    {
        QMessageBox::warning(this, "Warning", "No review selected for update.");
        return;
    }

    QString newContent = ui->tableWidget_new->item(0, 0)->text();
    QString newRatingStr = ui->tableWidget_new->item(0, 1)->text();
    bool ok;
    int newRating = newRatingStr.toInt(&ok);
    if (!ok || newRating < 1 || newRating > 5)
    {
        QMessageBox::warning(this, "Input Error", "Rating must be between 1 and 5.");
        return;
    }

    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    QString query = QString(
                        "UPDATE REVIEWS SET Content = '%1', Rating = %2 "
                        "WHERE UserNo = %3 AND ItemNo = %4 AND ReviewDate = '%5'"
                        ).arg(newContent).arg(newRating).arg(userNo).arg(itemNo).arg(reviewDate);


    if (SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS) == SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::information(this, "Success", "Review updated successfully.");
    }
    else
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        QMessageBox::critical(this, "Error", "Failed to update review.");
    }
}
