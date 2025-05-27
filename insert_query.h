#ifndef INSERT_QUERY_H
#define INSERT_QUERY_H

#include <QDialog>

namespace Ui {
class insert_query;
}

class insert_query : public QDialog
{
    Q_OBJECT

public:
    explicit insert_query(QWidget *parent = nullptr);
    ~insert_query();

private slots:
    void on_insert_s1_clicked();

    void on_insert_s2_clicked();

private:
    Ui::insert_query *ui;
};

#endif // INSERT_QUERY_H
