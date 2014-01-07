#include <QApplication>
#include <QString>
#include <QVariant>
#include <QDebug>

#include "Updater.h"
#include "CommandParser.h"

const int VERSION_MAJOR = 0;
const int VERSION_MINOR = 1;
const int VERSION_PATCHLEVEL = 0;

QString GetVersion() {
    QString v = "V";
    v.append(QVariant(VERSION_MAJOR).toString());
    v.append(".");
    v.append(QVariant(VERSION_MINOR).toString());
    v.append(".");
    v.append(QVariant(VERSION_PATCHLEVEL).toString());

    return qMove(v);
}

constexpr int GetVersionInt() {
    return ((VERSION_MAJOR * 100) + (VERSION_MINOR * 10) + (VERSION_PATCHLEVEL));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Notenarchiv - Updater");
    app.setApplicationVersion(GetVersion());

    CommandParser cmd;
    cmd.Process(app);

    Updater window(0);
    return window.exec();
}

