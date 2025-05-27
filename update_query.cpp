#include "update_query.h"
#include "ui_update_query.h"

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
