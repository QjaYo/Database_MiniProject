#ifndef INSERT_S2_H
#define INSERT_S2_H

#include <QDialog>

namespace Ui {
class insert_s2;
}

class insert_s2 : public QDialog
{
    Q_OBJECT

public:
    explicit insert_s2(QWidget *parent = nullptr);
    ~insert_s2();

private slots:
    void on_btn_confirm_clicked();

private:
    Ui::insert_s2 *ui;
};

#endif // INSERT_S2_H
