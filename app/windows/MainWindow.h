#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "models/CoursesModel.h"
#include "models/TreeNodesModel.h"

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
    void updateData();
    
    void setCurrentCourse(Course* course);
    
    void setInfoType(const QString& type);
    void setInfoTitle(const QString& title);
    
    std::unique_ptr<CoursesModel> m_coursesModel;
    std::unique_ptr<TreeNodesModel> m_treeNodesModel;
    
    Course* m_currentCourse;
    
    std::unique_ptr<Ui::MainWindow> m_ui;
};

#endif // MAINWINDOW_H
