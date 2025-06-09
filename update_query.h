#ifndef UPDATE_QUERY_H
#define UPDATE_QUERY_H

#include <QDialog>

namespace Ui {
class update_query;
}

class update_query : public QDialog
{
    Q_OBJECT

public:
    explicit update_query(QWidget *parent = nullptr);
    ~update_query();

private slots:
    void on_insert_s1_clicked();

    void on_insert_s2_clicked();

    void on_insert_s3_clicked();

    void on_insert_s4_clicked();

private:
    Ui::update_query *ui;
};

#endif // UPDATE_QUERY_H
