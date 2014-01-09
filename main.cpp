#include <QApplication>
#include <QVariant>
#include <QString>
#include <QCommandLineParser>

#include "MainWindow.h"
#include "Logfile.h"
#include "Database.h"

const int VERSION_MAJOR = 1;
<<<<<<< HEAD
const int VERSION_MINOR = 0;
const int VERSION_PATCHLEVEL = 1;
=======
const int VERSION_MINOR = 1;
const int VERSION_PATCHLEVEL = 0;
>>>>>>> ae5e7483368ccbd1595a8f20d7c4d36141af09dc

constexpr int GetVersionInt() {
    return ((VERSION_MAJOR * 100) + (VERSION_MINOR * 10) + (VERSION_PATCHLEVEL));
}

QString GetVersion() {
    QString v = "V";
    v.append(QVariant(VERSION_MAJOR).toString());
    v.append(".");
    v.append(QVariant(VERSION_MINOR).toString());
    v.append(".");
    v.append(QVariant(VERSION_PATCHLEVEL).toString());

    return qMove(v);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Notenarchiv");
    a.setApplicationDisplayName("Notenarchiv");
    a.setApplicationVersion(GetVersion());

    QCommandLineParser cmd;
    cmd.addHelpOption();
    cmd.addVersionOption();
    cmd.process(a);

    MainWindow w;
    w.show();

    int ret = a.exec();

    Log->Del();
    DB->Del();

    return ret;
}
