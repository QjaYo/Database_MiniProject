#ifndef DELETE_S1_H
#define DELETE_S1_H

#include <QDialog>

namespace Ui {
class delete_s1;
}

class delete_s1 : public QDialog
{
    Q_OBJECT

public:
    explicit delete_s1(QWidget *parent = nullptr);
    ~delete_s1();

private:
    Ui::delete_s1 *ui;
};

#endif // DELETE_S1_H
