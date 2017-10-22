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
    windows/MainWindow.cpp \
    models/Course.cpp \
    models/Section.cpp \
    models/Task.cpp \
    models/Test.cpp \
    models/CoursesModel.cpp \
    models/TreeNode.cpp \
    models/TreeNodesModel.cpp

HEADERS	+= \
    windows/MainWindow.h \
    models/Course.h \
    models/Section.h \
    models/Task.h \
    models/Test.h \
    models/CoursesModel.h \
    models/TreeNode.h \
    models/TreeNodesModel.h

FORMS	+= \
    windows/MainWindow.ui
