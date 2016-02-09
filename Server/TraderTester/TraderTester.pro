TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../Trader.cpp

HEADERS += \
    ../Trader.h

INCLUDEPATH += /home/zaytsev/soft/boost_1_60_0

LIBS += -L/usr/local/lib/ -lquickfix

QMAKE_PRE_LINK  = ln -sfn $$PWD/res res

QMAKE_CXX = g++-4.9

