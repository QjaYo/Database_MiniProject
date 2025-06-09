#ifndef SELECT_QUERY_H
#define SELECT_QUERY_H

#include <QDialog>

namespace Ui {
class select_query;
}

class select_query : public QDialog
{
    Q_OBJECT

public:
    explicit select_query(QWidget *parent = nullptr);
    ~select_query();

private slots:
    void on_btn_s1_clicked();

    void on_btn_s2_clicked();

    void on_btn_s3_clicked();

    void on_btn_s4_clicked();

    void on_btn_s6_clicked();

    void on_btn_s5_clicked();

private:
    Ui::select_query *ui;
};

#endif // SELECT_QUERY_H
