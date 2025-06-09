#ifndef SELECT_S6_H
#define SELECT_S6_H

#include <QDialog>

namespace Ui {
class select_s6;
}

class select_s6 : public QDialog
{
    Q_OBJECT

public:
    explicit select_s6(QWidget *parent = nullptr);
    ~select_s6();

private:
    Ui::select_s6 *ui;
};

#endif // SELECT_S6_H
