#include "select_s6.h"
#include "ui_select_s6.h"

select_s6::select_s6(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::select_s6)
{
    ui->setupUi(this);
}

select_s6::~select_s6()
{
    delete ui;
}
