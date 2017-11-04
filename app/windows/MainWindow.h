#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "models/CoursesListModel.h"
#include "models/CourseTreeModel.h"
#include "models/TestsTableModel.h"

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
    
    void updateInfoPanel(CourseNode* node);

    void setInfoPanelEditable(bool editable);
    
    enum Page {
        DEFAULT,
        COURSE_TREE,
        TASK_EDIT,

        PAGE_COUNT
    };
    
    std::unique_ptr<CoursesListModel> m_coursesListModel;
    std::unique_ptr<CourseTreeModel> m_courseTreeModel;
    std::unique_ptr<TestsTableModel> m_testsTableModel;

    CourseNode* m_selectedNode;
    
    std::unique_ptr<Ui::MainWindow> m_ui;
};

#endif // MAINWINDOW_H
