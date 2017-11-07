#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "controllers/CourseTreeController.h"
#include "controllers/InfoPanelController.h"
#include "controllers/SideMenuController.h"
#include "controllers/CourseEditController.h"
#include "controllers/SectionEditController.h"
#include "controllers/TaskEditController.h"
#include "controllers/TestEditController.h"
#include "controllers/TestsTableController.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    std::unique_ptr<CourseTreeController> m_courseTreeController;
    std::unique_ptr<InfoPanelController> m_infoPanelController;
    std::unique_ptr<SideMenuController> m_sideMenuController;
    std::unique_ptr<CourseEditController> m_courseEditController;
    std::unique_ptr<SectionEditController> m_sectionEditController;
    std::unique_ptr<TaskEditController> m_taskEditController;
    std::unique_ptr<TestEditController> m_testEditController;
    std::unique_ptr<TestsTableController> m_testsTableController;
    
    std::unique_ptr<Ui::MainWindow> m_ui;
};

#endif // MAINWINDOW_H
