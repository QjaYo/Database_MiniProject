#ifndef SELECT_S4_H
#define SELECT_S4_H

#include <QDialog>

namespace Ui {
class select_s4;
}

class select_s4 : public QDialog
{
    Q_OBJECT

public:
    explicit select_s4(QWidget *parent = nullptr);
    ~select_s4();

private slots:
    void on_comboBox_category_currentTextChanged();
    void loadCategories();
    void on_pushButton_search_clicked();

private:
    Ui::select_s4 *ui;
};

#endif // SELECT_S4_H
