#ifndef SELECT_S1_H
#define SELECT_S1_H

#include <QDialog>

namespace Ui {
class select_s1;
}

class select_s1 : public QDialog
{
    Q_OBJECT

public:
    explicit select_s1(QWidget *parent = nullptr);
    ~select_s1();

private slots:
    void on_btn_search_clicked();

private:
    Ui::select_s1 *ui;
    void initDateCombos();
};

#endif // SELECT_S1_H
