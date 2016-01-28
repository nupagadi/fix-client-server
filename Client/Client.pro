#-------------------------------------------------
#
# Project created by QtCreator 2016-01-20T17:46:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    Sender.cpp

HEADERS  += widget.h \
    Sender.h

CONFIG += c++14

LIBS += -L/usr/local/lib/ -lquickfix

QMAKE_CXX = g++-4.9
