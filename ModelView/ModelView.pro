#-------------------------------------------------
#
# Project created by QtCreator 2016-03-12T12:24:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModelView
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    listmodel.cpp \
    listviewcontroller.cpp

HEADERS  += mainwindow.h \
    listmodel.h \
    listviewcontroller.h

FORMS    += mainwindow.ui
