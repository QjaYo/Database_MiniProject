#ifndef INSERT_S4_H
#define INSERT_S4_H

#include <QDialog>

namespace Ui {
class insert_s4;
}

class insert_s4 : public QDialog
{
    Q_OBJECT

public:
    explicit insert_s4(QWidget *parent = nullptr);
    ~insert_s4();

private slots:
    void on_btn_itemNo_clicked();

    void on_btn_confirm_clicked();

    void on_btn_userNo_clicked();

private:
    Ui::insert_s4 *ui;
};

#endif // INSERT_S4_H
