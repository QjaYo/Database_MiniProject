#ifndef CUSTOM_QUERY_H
#define CUSTOM_QUERY_H

#include <QDialog>

namespace Ui {
class custom_query;
}

class custom_query : public QDialog
{
    Q_OBJECT

public:
    explicit custom_query(QWidget *parent = nullptr);
    ~custom_query();

private slots:
    void on_query_btn_clicked();

private:
    Ui::custom_query *ui;
};

#endif // CUSTOM_QUERY_H
