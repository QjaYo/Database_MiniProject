#ifndef SCHEMA_CHECK_H
#define SCHEMA_CHECK_H

#include <QDialog>

namespace Ui {
class schema_check;
}

class schema_check : public QDialog
{
    Q_OBJECT

public:
    explicit schema_check(QWidget *parent = nullptr);
    ~schema_check();

private:
    Ui::schema_check *ui;
    void loadTableList();
    void loadTableSchema(const QString &tableName);
};

#endif // SCHEMA_CHECK_H
