#include "delete_s1.h"
#include "ui_delete_s1.h"

delete_s1::delete_s1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::delete_s1)
{
    ui->setupUi(this);
}

delete_s1::~delete_s1()
{
    delete ui;
}
