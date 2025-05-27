#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include "server_connect.h"
#include "schema_check.h"
#include "custom_query.h"
#include "insert_query.h"
#include "delete_query.h"
#include "update_query.h"
#include "select_query.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_schema_clicked()
{
    schema_check dialog(this);
    dialog.exec();
}

void MainWindow::on_btn_select_clicked()
{
    select_query dialog(this);
    dialog.exec();
}

void MainWindow::on_btn_insert_clicked()
{
    insert_query dialog(this);
    dialog.exec();
}

void MainWindow::on_btn_delete_clicked()
{
    delete_query dialog(this);
    dialog.exec();
}

void MainWindow::on_btn_update_clicked()
{
    update_query dialog(this);
    dialog.exec();
}

void MainWindow::on_btn_custom_clicked()
{
    custom_query dialog(this);
    dialog.exec();
}
