#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_btn_schema_clicked();

    void on_btn_select_clicked();

    void on_btn_insert_clicked();

    void on_btn_delete_clicked();

    void on_btn_update_clicked();

    void on_btn_custom_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
