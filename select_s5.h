#ifndef SELECT_S5_H
#define SELECT_S5_H

#include <QDialog>

namespace Ui {
class select_s5;
}

class select_s5 : public QDialog
{
    Q_OBJECT

public:
    explicit select_s5(QWidget *parent = nullptr);
    ~select_s5();

private slots:
    void on_pushButton_clicked();

private:
    Ui::select_s5 *ui;
};

#endif // SELECT_S5_H
