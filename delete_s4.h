#ifndef DELETE_S4_H
#define DELETE_S4_H

#include <QDialog>

namespace Ui {
class delete_s4;
}

class delete_s4 : public QDialog
{
    Q_OBJECT

public:
    explicit delete_s4(QWidget *parent = nullptr);
    ~delete_s4();

private slots:
    void on_btn_itemNo_clicked();
    void on_btn_delete_clicked();

private:
    Ui::delete_s4 *ui;
};

#endif // DELETE_S4_H
