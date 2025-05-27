#include "select_query.h"
#include "ui_select_query.h"
#include "select_s1.h"
#include "select_s2.h"
#include "select_s3.h"

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

