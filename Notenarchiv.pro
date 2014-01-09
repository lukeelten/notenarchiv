#-------------------------------------------------
#
# Project created by QtCreator 2013-12-24T18:01:16
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Notenarchiv
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        MainWindow.cpp \
    AboutDialog.cpp \
    Database.cpp \
    Logfile.cpp \
    Eintrag.cpp \
    FilesDialog.cpp \
    File.cpp

HEADERS  += MainWindow.h \
    AboutDialog.h \
    Database.h \
    Singleton.h \
    Logfile.h \
    Eintrag.h \
    FilesDialog.h \
    File.h

FORMS    += MainWindow.ui \
    AboutDialog.ui \
    FilesDialog.ui

RESOURCES += \
    Icons.qrc

OTHER_FILES += \
    Icons/Add.png \
    Icons/Check.png \
    Icons/Delete.png \
    Icons/Download.png \
    Icons/Exit.png \
    Icons/Save.png \
    Icons/Search.png \
    Icons/icon.gif \
    Notenarchiv.sqlite \
    Icons/icon.ico
