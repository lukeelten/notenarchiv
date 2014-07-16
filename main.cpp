#include <QApplication>
#include <QVariant>
#include <QString>
#include <QCommandLineParser>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "MainWindow.h"
#include "Logfile.h"
#include "Database.h"
#include "util.h"

const int VERSION_MAJOR = 1;
const int VERSION_MINOR = 1;
const int VERSION_PATCHLEVEL = 1;

constexpr int GetVersionInt() {
    return ((VERSION_MAJOR * 100) + (VERSION_MINOR * 10) + (VERSION_PATCHLEVEL));
}

QString GetVersion() {
    QString v = "V";
    v.append(util::toString(VERSION_MAJOR));
    v.append(".");
    v.append(util::toString(VERSION_MINOR));
    v.append(".");
    v.append(util::toString(VERSION_PATCHLEVEL));

    return v;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Notenarchiv");
    a.setApplicationDisplayName("Notenarchiv");
    a.setApplicationVersion(GetVersion());
    
    QTranslator global_translator;
    global_translator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); // globale Übersetzung laden
    
    a.installTranslator(&global_translator);

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
