TEMPLATE = app

TARGET	= methodist
QT      += core gui widgets
CONFIG	+= c++14

DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR	= ../../build
OBJECTS_DIR = obj
MOC_DIR = obj

SOURCES += \
    main.cpp \
    windows/MainWindow.cpp

HEADERS	+= \
    windows/MainWindow.h

FORMS	+= \
    windows/MainWindow.ui
