#ifndef DELETE_QUERY_H
#define DELETE_QUERY_H

#include <QDialog>

namespace Ui {
class delete_query;
}

class delete_query : public QDialog
{
    Q_OBJECT

public:
    explicit delete_query(QWidget *parent = nullptr);
    ~delete_query();

private slots:
    void on_delete_s1_clicked();

    void on_delete_s2_clicked();

    void on_delete_s3_clicked();

    void on_delete_s4_clicked();

private:
    Ui::delete_query *ui;
};

#endif // DELETE_QUERY_H
