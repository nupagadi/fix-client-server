TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../Trader.cpp

HEADERS += \
    ../Trader.h

INCLUDEPATH += /home/zaytsev/soft/boost_1_60_0
