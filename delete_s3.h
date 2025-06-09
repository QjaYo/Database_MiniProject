#ifndef DELETE_S3_H
#define DELETE_S3_H

#include <QDialog>

namespace Ui {
class delete_s3;
}

class delete_s3 : public QDialog
{
    Q_OBJECT

public:
    explicit delete_s3(QWidget *parent = nullptr);
    ~delete_s3();

private slots:
    void on_btn_cancel_clicked();

private:
    Ui::delete_s3 *ui;
};

#endif // DELETE_S3_H
