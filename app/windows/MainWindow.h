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
    
    void updateInfoPanel(TreeNode* node);
    
    enum Page {
        DEFAULT,
        COURSE_TREE,
        COURSE_EDIT,
    } m_currentPage;
    
    std::unique_ptr<CoursesModel> m_coursesListModel;
    std::unique_ptr<TreeNodesModel> m_courseTreeModel;
    
    TreeNode* m_selectedNode;
    
    std::unique_ptr<Ui::MainWindow> m_ui;
};

#endif // MAINWINDOW_H
