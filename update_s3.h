#ifndef UPDATE_S3_H
#define UPDATE_S3_H

#include <QDialog>

namespace Ui {
class update_s3;
}

class update_s3 : public QDialog
{
    Q_OBJECT

public:
    explicit update_s3(QWidget *parent = nullptr);
    ~update_s3();

private slots:
    void on_btn_itemNo_clicked();

    void on_btn_update_clicked();

private:
    Ui::update_s3 *ui;
};

#endif // UPDATE_S3_H
