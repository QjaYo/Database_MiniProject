#include "delete_query.h"
#include "ui_delete_query.h"

delete_query::delete_query(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::delete_query)
{
    ui->setupUi(this);
}

delete_query::~delete_query()
{
    delete ui;
}
