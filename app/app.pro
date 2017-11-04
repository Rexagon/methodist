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
    models/CoursesListModel.cpp \
    models/CourseTreeModel.cpp \
    models/TestsTableModel.cpp \
    objects/Course.cpp \
    objects/CourseNode.cpp \
    objects/Section.cpp \
    objects/Task.cpp \
    objects/Test.cpp

HEADERS	+= \
    windows/MainWindow.h \
    models/CoursesListModel.h \
    models/CourseTreeModel.h \
    models/TestsTableModel.h \
    objects/Course.h \
    objects/CourseNode.h \
    objects/Section.h \
    objects/Task.h \
    objects/Test.h \

FORMS	+= \
    windows/MainWindow.ui

RESOURCES += \
    resources.qrc
