#include "CourseTreeController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"

CourseTreeController::CourseTreeController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentCourse(nullptr), m_selectedCourseNode(nullptr)
{
    connect(m_ui->courseTree, &QTreeView::pressed, this, [this](const QModelIndex& index) {
        CourseNode* node = reinterpret_cast<CourseNode*>(index.internalPointer());
        m_selectedCourseNode = node;
        emit courseNodeSelected(node);
    });
}

CourseTreeController::~CourseTreeController()
{
}

void CourseTreeController::propose()
{
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TREE);
}

Course* CourseTreeController::getCurrentCourse()
{
    return m_currentCourse;
}

CourseNode* CourseTreeController::getSelectedCourseNode()
{
    return m_selectedCourseNode;
}

void CourseTreeController::setCourse(Course* course)
{    
    if (course != nullptr) {
        CourseTreeModel* treeModel = ModelManager::getCourseTreeModel(course);
        
        m_ui->courseTree->setModel(treeModel);
        m_ui->courseTree->setExpanded(treeModel->index(0, 0), true);
        
        m_selectedCourseNode = reinterpret_cast<CourseNode*>(course);
        m_ui->courseTree->selectionModel()->select(treeModel->index(0, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        emit courseNodeSelected(m_selectedCourseNode);
    }
    
    m_currentCourse = course;
    
    propose();
}
