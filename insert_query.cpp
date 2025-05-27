#include "insert_query.h"
#include "ui_insert_query.h"
#include "insert_s1.h"
#include "insert_s2.h"


insert_query::insert_query(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::insert_query)
{
    ui->setupUi(this);
}

insert_query::~insert_query()
{
    delete ui;
}

void insert_query::on_insert_s1_clicked()
{
    insert_s1 dialog(this);
    dialog.exec();
}


void insert_query::on_insert_s2_clicked()
{
    insert_s2 dialog(this);
    dialog.exec();
}

