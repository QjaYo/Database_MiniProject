#ifndef UPDATE_S1_H
#define UPDATE_S1_H

#include <QDialog>

namespace Ui {
class update_s1;
}

class update_s1 : public QDialog
{
    Q_OBJECT

public:
    explicit update_s1(QWidget *parent = nullptr);
    ~update_s1();

private slots:
    void on_btn_userNo_clicked();

    void on_btn_recharge_clicked();

    void on_textEdit_userNo_textChanged();

private:
    Ui::update_s1 *ui;
};

#endif // UPDATE_S1_H
