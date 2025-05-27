#include "mainwindow.h"
#include "server_connect.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (db_connect())
    {
        qDebug() << "Connected to" << dataSourceName << "!";
        MainWindow w;
        w.show();
        int result = a.exec();
        db_disconnect();
        return result;
    }
    else
    {
        qDebug() << "Failed to connect to" << dataSourceName;
        return -1;
    }
}
