#include "CourseEditController.h"

#include <ui_MainWindow.h>

CourseEditController::CourseEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentCourse(nullptr)
{
    connect(m_ui->saveCourseChangesButton, &QPushButton::pressed, this, &CourseEditController::changesSaved);
    connect(m_ui->cancelCourseChangesButton, &QPushButton::pressed, this, &CourseEditController::changesCanceled);
}

CourseEditController::~CourseEditController()
{
}

void CourseEditController::propose()
{
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TREE);
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_COURSE);
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_COURSE);
    
    m_ui->courseEditName->setEnabled(true);
    m_ui->courseEditLectureHours->setEnabled(true);
    m_ui->courseEditPracticeHours->setEnabled(true);
}

void CourseEditController::setCourse(Course* course)
{
    if (course == nullptr) {
        return;
    }
    
    m_currentCourse = course;
    
    m_ui->nodeType->setText("Курс");
    m_ui->courseEditName->setPlainText(course->getName());
    m_ui->courseEditLectureHours->setValue(course->getLectureHourCount());
    m_ui->courseEditPracticeHours->setValue(course->getPracticeHourCount());
    
    propose();
}
