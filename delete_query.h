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

private:
    Ui::delete_query *ui;
};

#endif // DELETE_QUERY_H
