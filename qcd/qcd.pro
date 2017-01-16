#-------------------------------------------------
#
# Project created by QtCreator 2017-01-09T00:31:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qcd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientcxn.cpp

HEADERS  += mainwindow.h \
    clientcxn.h

FORMS    += mainwindow.ui
