#-------------------------------------------------
#
# Project created by QtCreator 2016-03-20T21:13:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qbitviewer
TEMPLATE = app

# make clean target
QMAKE_CLEAN += qbitviewer

# make install target
isEmpty(PREFIX) {
    message(No prefix given. Using /usr/local)
    PREFIX=/usr/local
}

target.path  = $$PREFIX/bin
INSTALLS    += target


target.path = /usr/local/bin/
#target.files = ./qbitviewer

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INSTALLS += target
