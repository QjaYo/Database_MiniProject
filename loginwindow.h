#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    int getUserNo() const;

private slots:
    void on_btn_login_clicked();

private:
    Ui::LoginWindow *ui;
    int userNo = -1;
};

#endif // LOGINWINDOW_H
