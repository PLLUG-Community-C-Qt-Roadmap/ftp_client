#-------------------------------------------------
#
# Project created by QtCreator 2016-01-31T13:18:56
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FtpClient
TEMPLATE = app

SOURCES += main.cpp \
    packet.cpp \
    mainwindow.cpp \
    listmodel.cpp \
    clientcontext.cpp \
    listviewcontroller.cpp

HEADERS  += mainwindow.h \
    packet.h \
    listmodel.h \
    clientcontext.h \
    listviewcontroller.h

FORMS    += mainwindow.ui

DISTFILES += \
    data.txt

CONFIG += c++11
