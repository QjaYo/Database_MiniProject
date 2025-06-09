#ifndef UPDATE_S4_H
#define UPDATE_S4_H

#include <QDialog>

namespace Ui {
class update_s4;
}

class update_s4 : public QDialog
{
    Q_OBJECT

public:
    explicit update_s4(QWidget *parent = nullptr);
    ~update_s4();

private slots:
    void on_btn_userNo_clicked();
    void on_btn_itemNo_clicked();
    void on_btn_search_clicked();
    void on_btn_edit_clicked();
    void on_btn_update_clicked();

private:
    Ui::update_s4 *ui;
};

#endif // UPDATE_S4_H
