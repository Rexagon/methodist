#include "InfoPanelController.h"

#include <ui_MainWindow.h>

#include "../objects/Course.h"
#include "../objects/Section.h"
#include "../objects/Task.h"

InfoPanelController::InfoPanelController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentCourseNode(nullptr)
{
    m_ui->infoPanelButtons->setCurrentIndex(0);
    setVisible(false);
    
    connect(m_ui->addTaskButton, &QPushButton::pressed, this, &InfoPanelController::addTaskButtonPressed);
    connect(m_ui->addSectionButton, &QPushButton::pressed, this, &InfoPanelController::addSectionButtonPressed);
    connect(m_ui->editNodeButton, &QPushButton::pressed, this, &InfoPanelController::editNodeButtonPressed);
    connect(m_ui->deleteNodeButton, &QPushButton::pressed, this, &InfoPanelController::deleteNodeButtonPressed);
}

InfoPanelController::~InfoPanelController()
{
}

void InfoPanelController::propose()
{
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TREE);
    m_ui->infoPanelPages->setCurrentIndex(static_cast<int>(m_currentCourseNode->getType()));
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_DEFAULT);
    
    // Course properties
    m_ui->courseEditName->setEnabled(false);
    m_ui->courseEditLectureHours->setEnabled(false);
    m_ui->courseEditPracticeHours->setEnabled(false);

    // Section properties
    m_ui->sectionEditName->setEnabled(false);

    // Task properties
    m_ui->taskEditName->setEnabled(false);
    m_ui->taskEditScore->setEnabled(false);
    
    // Test properties
    m_ui->testEditInputData->setEnabled(false);
    m_ui->testEditScore->setEnabled(false);
    
    setVisible(true);
}

CourseNode*InfoPanelController::getCurrentCourseNode()
{
    return m_currentCourseNode;
}

void InfoPanelController::setCourseNode(CourseNode* node)
{
    if (node == nullptr) {
        return;
    }
    
    m_currentCourseNode = node;
    
    switch (node->getType()) {
        case CourseNode::Type::COURSE:
        {
            Course* course = reinterpret_cast<Course*>(node);
            m_ui->nodeType->setText("Курс");
            m_ui->courseEditName->setPlainText(course->getName());
            m_ui->courseEditLectureHours->setValue(course->getLectureHourCount());
            m_ui->courseEditPracticeHours->setValue(course->getPracticeHourCount());

            m_ui->addSectionButton->setVisible(true);
            m_ui->addTaskButton->setVisible(false);
            break;
        }
            
            
        case CourseNode::Type::SECTION:
        {
            Section* section = reinterpret_cast<Section*>(node);
            m_ui->nodeType->setText("Раздел");
            m_ui->sectionEditName->setPlainText(section->getName());

            m_ui->addSectionButton->setVisible(true);
            m_ui->addTaskButton->setVisible(true);
            break;
        }
            
        
        case CourseNode::Type::TASK:
        {
            Task* task = reinterpret_cast<Task*>(node);
            m_ui->nodeType->setText("Задача");
            m_ui->taskEditName->setPlainText(task->getName());
            m_ui->taskEditScore->setValue(task->getScore());

            m_ui->addSectionButton->setVisible(false);
            m_ui->addTaskButton->setVisible(false);
            break;
        }
    }
    
    propose();
}

void InfoPanelController::setVisible(bool visible)
{
    m_ui->infoPanel->setVisible(visible);
}