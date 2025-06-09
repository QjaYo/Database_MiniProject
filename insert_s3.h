#ifndef INSERT_S3_H
#define INSERT_S3_H

#include <QDialog>

namespace Ui {
class insert_s3;
}

class insert_s3 : public QDialog
{
    Q_OBJECT

public:
    explicit insert_s3(QWidget *parent = nullptr);
    ~insert_s3();

private slots:
    void on_btn_duplicateCheck_clicked();

    void on_btn_signUp_clicked();

    void on_textEdit_id_textChanged();

private:
    Ui::insert_s3 *ui;
};

#endif // INSERT_S3_H
