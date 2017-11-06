#include "CourseTreeController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"

CourseTreeController::CourseTreeController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentCourse(nullptr), m_currentCourseNode(nullptr)
{
    connect(m_ui->courseTree, &QTreeView::pressed, this, [this](const QModelIndex& index) {
        CourseNode* node = reinterpret_cast<CourseNode*>(index.internalPointer());
        m_currentCourseNode = node;
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

CourseNode*CourseTreeController::getSelectedCourseNode()
{
    return m_currentCourseNode;
}

void CourseTreeController::setCourse(Course* course)
{    
    if (course != nullptr && m_currentCourse != course) {
        CourseTreeModel* treeModel = ModelManager::getCourseTreeModel(course);
        
        m_ui->courseTree->setModel(treeModel);
        m_ui->courseTree->setExpanded(treeModel->index(0, 0), true);
        
        m_currentCourseNode = reinterpret_cast<CourseNode*>(course);
        m_ui->courseTree->selectionModel()->select(treeModel->index(0, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        emit courseNodeSelected(m_currentCourseNode);
    }
    
    m_currentCourse = course;
    
    propose();
}
