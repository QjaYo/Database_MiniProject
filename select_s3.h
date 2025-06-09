#ifndef SELECT_S3_H
#define SELECT_S3_H

#include <QDialog>

namespace Ui {
class select_s3;
}

class select_s3 : public QDialog
{
    Q_OBJECT

public:
    explicit select_s3(QWidget *parent = nullptr);
    ~select_s3();
    QString getSelectedItemNo() const;

private slots:
    void on_btn_search_clicked();
    QSet<int> getAllCategoryNos(int rootCategoryNo);

private:
    Ui::select_s3 *ui;
};

#endif // SELECT_S3_H
