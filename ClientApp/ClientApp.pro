TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ClientApp.cpp

INCLUDEPATH += /home/zaytsev/soft/boost_1_60_0
#    $$PWD/..

LIBS += -L/usr/local/lib/ -lquickfix

QMAKE_CXX = g++-4.9

HEADERS += \
    ClientApp.h

#symlink to access /res in build directory
QMAKE_PRE_LINK  = ln -s $$PWD/res res

