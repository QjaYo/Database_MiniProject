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

private:
    Ui::update_query *ui;
};

#endif // UPDATE_QUERY_H
