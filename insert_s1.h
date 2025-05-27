#ifndef INSERT_S1_H
#define INSERT_S1_H

#include <QDialog>

namespace Ui {
class insert_s1;
}

class insert_s1 : public QDialog
{
    Q_OBJECT

public:
    explicit insert_s1(QWidget *parent = nullptr);
    ~insert_s1();

private slots:
    void on_btn_search_clicked();

    void on_btn_confirm_clicked();

private:
    Ui::insert_s1 *ui;
};

#endif // INSERT_S1_H
