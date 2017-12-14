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
    DeletionMark deletionMark = course->getDeletionMark();
    
    QString name = m_ui->courseEditName->toPlainText();
    size_t lectureHours = m_ui->courseEditLectureHours->value();
    size_t practiceHours = m_ui->courseEditPracticeHours->value();
    size_t laboratoryHours = m_ui->courseEditLaboratoryHours->value();
    
    QString query = "UPDATE course SET "
                    "course_name='" + name + "', "
                    "lecture_hours=" + QString::number(lectureHours) + ", "
                    "practice_hours=" + QString::number(practiceHours) + ", "
                    "laboratory_hours=" + QString::number(laboratoryHours) + " "
                    "WHERE rowid=" + QString::number(course->getId());
    
    NetworkManager::send(Request(SQL_OPERATOR, "course_edit", {
        {"sql_operator", query}
    }), [this, course, deletionMark, name, lectureHours, practiceHours, laboratoryHours](const Response& response)
    {
        if (*deletionMark == true) {
            return;
        }
        
        course->setName(name);
        course->setLectureHourCount(lectureHours);
        course->setPracticeHourCount(practiceHours);
        course->setLaboratoryHourCount(laboratoryHours);
        
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
