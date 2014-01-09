#-------------------------------------------------
#
# Project created by QtCreator 2014-01-04T11:47:23
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Updater
TEMPLATE = app


SOURCES += main.cpp\
        Updater.cpp \
    CommandParser.cpp \
    UpdateInformation.cpp

HEADERS  += Updater.h \
    CommandParser.h \
    UpdateInformation.h

FORMS    += Updater.ui

OTHER_FILES += \
    notenarchiv.json
