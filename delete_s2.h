#ifndef DELETE_S2_H
#define DELETE_S2_H

#include <QDialog>

namespace Ui {
class delete_s2;
}

class delete_s2 : public QDialog
{
    Q_OBJECT

public:
    explicit delete_s2(QWidget *parent = nullptr);
    ~delete_s2();

private slots:
    void on_btn_userNo_clicked();

    void on_btn_itemNo_clicked();

    void on_btn_delete_clicked();

    void on_btn_search_clicked();

private:
    Ui::delete_s2 *ui;
};

#endif // DELETE_S2_H
