#include "MainWindow.h"
#include <QApplication>

#include "Logfile.h"
#include "Database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Notenarchiv Musikverein Elten");
    a.setApplicationDisplayName("Notenarchiv MVE");
    a.setApplicationVersion("1.0");

    MainWindow w;
    w.show();

    int ret = a.exec();

    Log->Del();
    DB->Del();

    return ret;
}
