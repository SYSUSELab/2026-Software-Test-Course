QT       += core gui widgets

CONFIG += c++17

TARGET = calculator
TEMPLATE = app

SOURCES += \
    Calculator.cpp \
    MainWindow.cpp \
    main.cpp

HEADERS += \
    Calculator.h \
    MainWindow.h

DISTFILES += \
    style.qss
