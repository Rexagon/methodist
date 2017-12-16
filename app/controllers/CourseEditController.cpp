#include "CourseEditController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"
#include "../stuff/NetworkManager.h"

CourseEditController::CourseEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentCourse(nullptr)
{
    connect(m_ui->saveCourseChangesButton, &QPushButton::pressed, this, &CourseEditController::changesSaved);
    connect(m_ui->cancelCourseChangesButton, &QPushButton::pressed, this, &CourseEditController::changesCanceled);
}

CourseEditController::~CourseEditController()
{
}

void CourseEditController::saveChanges()
{
    if (m_currentCourse == nullptr) {
        return;
    }
    
    Course* course = m_currentCourse;
    
    Course::Data courseData = course->getData();
    
    courseData.name = m_ui->courseEditName->toPlainText();
    courseData.lectureHourCount = m_ui->courseEditLectureHours->value();
    courseData.practiceHourCount = m_ui->courseEditPracticeHours->value();
    courseData.laboratoryHourCount = m_ui->courseEditLaboratoryHours->value();
    
    Course::dbUpdate(course, courseData, [this, course]() {
        ModelManager::getCoursesListModel()->update();
        ModelManager::getCourseTreeModel(course)->update();
    });
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
    m_ui->courseEditLaboratoryHours->setEnabled(true);
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
    m_ui->courseEditLaboratoryHours->setValue(course->getLaboratoryHourCount());
    
    propose();
}
