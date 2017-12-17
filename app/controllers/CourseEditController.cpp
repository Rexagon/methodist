#include "CourseEditController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"
#include "../stuff/NetworkManager.h"

CourseEditController::CourseEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentCourse(nullptr), m_isEditable(false)
{
    connect(m_ui->addSectionButton, &QPushButton::pressed, this, &CourseEditController::addSectionButtonPressed);
    connect(m_ui->editCourseButton, &QPushButton::pressed, this, &CourseEditController::editButtonPressed);
    connect(m_ui->deleteCourseButton, &QPushButton::pressed, this, &CourseEditController::deleteButtonPressed);
    connect(m_ui->saveCourseChangesButton, &QPushButton::pressed, this, &CourseEditController::saveChangesButtonPressed);
    connect(m_ui->cancelCourseChangesButton, &QPushButton::pressed, this, &CourseEditController::cancelChangesButtonPressed);
}

CourseEditController::~CourseEditController()
{
}

void CourseEditController::saveCurrentCourse()
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

void CourseEditController::deleteCurrentCourse()
{
    
}

void CourseEditController::propose()
{
    m_ui->infoPanel->setVisible(true);
    
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TREE);
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_COURSE);
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_COURSE);
    
    m_ui->courseEditName->setEnabled(m_isEditable);
    m_ui->courseEditLectureHours->setEnabled(m_isEditable);
    m_ui->courseEditPracticeHours->setEnabled(m_isEditable);
    m_ui->courseEditLaboratoryHours->setEnabled(m_isEditable);
    
    m_ui->addSectionButton->setVisible(!m_isEditable);
    m_ui->editCourseButton->setVisible(!m_isEditable);
    m_ui->deleteCourseButton->setVisible(!m_isEditable);
    m_ui->saveCourseChangesButton->setVisible(m_isEditable);
    m_ui->cancelCourseChangesButton->setVisible(m_isEditable);
}

void CourseEditController::setEditable(bool editable)
{
    m_isEditable = editable;
}

bool CourseEditController::isEditable() const
{
    return m_isEditable;
}

void CourseEditController::setCourse(Course* course)
{
    if (course == nullptr) {
        return;
    }
    
    m_currentCourse = course;
    
    m_ui->courseEditName->setPlainText(course->getName());
    m_ui->courseEditLectureHours->setValue(course->getLectureHourCount());
    m_ui->courseEditPracticeHours->setValue(course->getPracticeHourCount());
    m_ui->courseEditLaboratoryHours->setValue(course->getLaboratoryHourCount());
    
    propose();
}

Course* CourseEditController::getCurrentCourse() const
{
    return m_currentCourse;
}
