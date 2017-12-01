TEMPLATE = app

TARGET	= methodist
QT      += core gui widgets websockets xml
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
    objects/Test.cpp \
    controllers/SideMenuController.cpp \
    controllers/CourseTreeController.cpp \
    controllers/Controller.cpp \
    stuff/ModelManager.cpp \
    controllers/InfoPanelController.cpp \
    controllers/CourseEditController.cpp \
    controllers/SectionEditController.cpp \
    controllers/TaskEditController.cpp \
    controllers/TestsTableController.cpp \
    controllers/TestEditController.cpp \
    stuff/NetworkManager.cpp \
    models/SamplesTableModel.cpp \
    stuff/Log.cpp \
    stuff/Request.cpp \
    stuff/RequestElement.cpp \
    stuff/Response.cpp

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
    controllers/SideMenuController.h \
    controllers/CourseTreeController.h \
    controllers/Controller.h \
    stuff/ModelManager.h \
    controllers/InfoPanelController.h \
    controllers/CourseEditController.h \
    controllers/SectionEditController.h \
    controllers/TaskEditController.h \
    controllers/TestsTableController.h \
    controllers/TestEditController.h \
    stuff/NetworkManager.h \
    models/SamplesTableModel.h \
    stuff/Log.h \
    stuff/Request.h \
    stuff/RequestElement.h \
    stuff/Response.h

FORMS	+= \
    windows/MainWindow.ui

RESOURCES += \
    resources.qrc
