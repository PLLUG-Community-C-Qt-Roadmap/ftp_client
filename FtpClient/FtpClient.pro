#-------------------------------------------------
#
# Project created by QtCreator 2016-01-31T13:18:56
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FtpClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    packet.cpp

HEADERS  += mainwindow.h \
    packet.h

FORMS    += mainwindow.ui

DISTFILES += \
    data.txt

CONFIG += c++11
