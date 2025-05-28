#include "select_s1.h"
#include "ui_select_s1.h"
#include "global.h"
#include "func.h"

select_s1::select_s1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::select_s1)
{
    ui->setupUi(this);
    initDateCombos();
}

select_s1::~select_s1()
{
    delete ui;
}

//user와 OrderDate를 선택하면 해당 날짜의 주문내역(OrderNo, OrderDate, ItemName, PriceAtOrder, Quantity)을 가져온다.

void select_s1::on_btn_search_clicked()
{
    QString userName = ui->textEdit_userName->toPlainText().trimmed();
    QString year = ui->comboBox_year->currentText();
    QString month = ui->comboBox_month->currentText();
    QString day = ui->comboBox_day->currentText();

    if (userName.isEmpty() || year.isEmpty() || month.isEmpty() || day.isEmpty())
    {
        return;
    }

    QString dateStr;

    if (year == "All")
    {
        dateStr = "";
    }
    else if (month == "All")
    {
        dateStr = QString("%1").arg(year);
    }
    else if (day == "All")
    {
        dateStr = QString("%1-%2")
            .arg(year)
            .arg(month.rightJustified(2, '0'));
    }
    else
    {
        dateStr = QString("%1-%2-%3")
            .arg(year)
            .arg(month.rightJustified(2, '0'))
            .arg(day.rightJustified(2, '0'));
    }


    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        print_error(SQL_HANDLE_STMT, hDbc);
        return;
    }

    QString query;

    if (year == "All")
    {
        query = QString(
                    "SELECT O.OrderNo, O.OrderDate, I.ItemName, O.PriceAtOrder, O.Quantity "
                    "FROM USERS U, ORDERS O, ORDER_ITEM_DELIVERY OID, ITEMS I "
                    "WHERE U.UserNo = O.UserNo "
                    "AND O.OrderNo = OID.OrderNo "
                    "AND OID.ItemNo = I.ItemNo "
                    "AND U.UserName = '%1';")
                    .arg(userName);
    }
    else
    {
        query = QString(
                    "SELECT O.OrderNo, O.OrderDate, I.ItemName, O.PriceAtOrder, O.Quantity "
                    "FROM USERS U, ORDERS O, ORDER_ITEM_DELIVERY OID, ITEMS I "
                    "WHERE U.UserNo = O.UserNo "
                    "AND O.OrderNo = OID.OrderNo "
                    "AND OID.ItemNo = I.ItemNo "
                    "AND U.UserName = '%1' "
                    "AND O.OrderDate LIKE '%2%%';")
                    .arg(userName)
                    .arg(dateStr);
    }

    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)query.toLocal8Bit().constData(), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        print_error(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    SQLSMALLINT colCount = 0;
    SQLNumResultCols(hStmt, &colCount);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(colCount);

    QStringList headers = { "OrderNo", "OrderDate", "ItemName", "PriceAtOrder", "Quantity" };
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (SQLFetch(hStmt) != SQL_NO_DATA)
    {
        ui->tableWidget->insertRow(row);
        for (SQLSMALLINT col = 0; col < colCount; ++col)
        {
            SQLCHAR buffer[256];
            SQLGetData(hStmt, col + 1, SQL_C_CHAR, buffer, sizeof(buffer), nullptr);
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString::fromLocal8Bit((char*)buffer)));
        }
        ++row;
    }

    SQLCloseCursor(hStmt);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void select_s1::initDateCombos()
{
    ui->comboBox_year->addItem(QString("All"));
    for (int year = 2019; year <= 2030; year++)
        ui->comboBox_year->addItem(QString::number(year));

    ui->comboBox_month->addItem(QString("All"));
    for (int month = 1; month <= 12; month++)
        ui->comboBox_month->addItem(QString("%1").arg(month, 2, 10, QLatin1Char('0')));

    ui->comboBox_day->addItem(QString("All"));
    for (int day = 1; day <= 31; day++)
        ui->comboBox_day->addItem(QString("%1").arg(day, 2, 10, QLatin1Char('0')));
}
