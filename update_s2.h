#ifndef UPDATE_S2_H
#define UPDATE_S2_H

#include <QDialog>

namespace Ui {
class update_s2;
}

class update_s2 : public QDialog
{
    Q_OBJECT

public:
    explicit update_s2(QWidget *parent = nullptr);
    ~update_s2();

private slots:
    void on_btn_discount_clicked();

private:
    Ui::update_s2 *ui;
};

#endif // UPDATE_S2_H
