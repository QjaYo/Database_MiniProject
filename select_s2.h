#ifndef SELECT_S2_H
#define SELECT_S2_H

#include <QDialog>

namespace Ui {
class select_s2;
}

class select_s2 : public QDialog
{
    Q_OBJECT

public:
    explicit select_s2(QWidget *parent = nullptr);
    ~select_s2();

private slots:
    void on_btn_search_clicked();

private:
    Ui::select_s2 *ui;
};

#endif // SELECT_S2_H
