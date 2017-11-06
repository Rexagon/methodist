#include "SideMenuController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"

SideMenuController::SideMenuController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent)
{
    m_ui->coursesList->setModel(ModelManager::getCoursesListModel());
    
    connect(m_ui->addCourseButton, &QPushButton::pressed, this, &SideMenuController::courseAdded);
    
    connect(m_ui->refreshCoursesButton, &QPushButton::pressed, this, &SideMenuController::coursesRefreshed);
    
    connect(m_ui->coursesList, &QListView::pressed, this, [this](const QModelIndex& index) {
        Course* course = ModelManager::getCoursesListModel()->getCourse(static_cast<size_t>(index.row()));
        
        if (course != nullptr) {
            emit courseSelected(course);
        }
    });
}

SideMenuController::~SideMenuController()
{
}

void SideMenuController::selectCourse(Course* course)
{
    CoursesListModel* model = ModelManager::getCoursesListModel();
    int index = model->getCourseIndex(course);
    if (index != -1) {
        m_ui->coursesList->selectionModel()->select(model->index(index), QItemSelectionModel::SelectionFlag::Select);
    }
}
