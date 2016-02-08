TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Application.cpp \
    Market.cpp \
    ordermatch.cpp \
    Trader.cpp

HEADERS += \
    Application.h \
    IDGenerator.h \
    Market.h \
    Order.h \
    OrderMatcher.h \
    Trader.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lquickfix
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lquickfix
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lquickfix

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include
