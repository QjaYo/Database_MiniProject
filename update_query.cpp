#include "update_query.h"
#include "ui_update_query.h"
#include "update_s1.h"
#include "update_s2.h"
#include "update_s3.h"
#include "update_s4.h"

update_query::update_query(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::update_query)
{
    ui->setupUi(this);
}

update_query::~update_query()
{
    delete ui;
}

void update_query::on_insert_s1_clicked()
{
    update_s1 dialog(this);
    dialog.exec();
}


void update_query::on_insert_s2_clicked()
{
    update_s2 dialog(this);
    dialog.exec();
}


void update_query::on_insert_s3_clicked()
{
    update_s3 dialog(this);
    dialog.exec();
}


void update_query::on_insert_s4_clicked()
{
    update_s4 dialog(this);
    dialog.exec();
}

