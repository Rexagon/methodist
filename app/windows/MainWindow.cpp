#include "MainWindow.h"

#include <ui_MainWindow.h>
#include <iostream>

#include "../stuff/ModelManager.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), m_ui(nullptr)
{
    m_ui = std::make_unique<Ui::MainWindow>();
    m_ui->setupUi(this);
    ModelManager::init(this);
    
    m_ui->infoPanel->hide();
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_DEFAULT);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TREE);
    
    m_courseTreeController = std::make_unique<CourseTreeController>(m_ui.get(), this);
    m_infoPanelController = std::make_unique<InfoPanelController>(m_ui.get(), this);
    m_sideMenuController = std::make_unique<SideMenuController>(m_ui.get(), this);
    m_courseEditController = std::make_unique<CourseEditController>(m_ui.get(), this);
    m_sectionEditController = std::make_unique<SectionEditController>(m_ui.get(), this);
    m_taskEditController = std::make_unique<TaskEditController>(m_ui.get(), this);
    m_testEditController = std::make_unique<TestEditController>(m_ui.get(), this);
    m_testsTableController = std::make_unique<TestsTableController>(m_ui.get(), this);
    
    // Actions
    connect(m_sideMenuController.get(), &SideMenuController::courseSelected, this, [this](Course* course) {
        m_courseTreeController->setCourse(course);
    });
    
    connect(m_courseTreeController.get(), &CourseTreeController::courseNodeSelected, this, [this](CourseNode* node) {
        m_infoPanelController->setCourseNode(node);
    });
    
    connect(m_infoPanelController.get(), &InfoPanelController::editNodeButtonPressed, this, [this]() {
        CourseNode* node = m_courseTreeController->getSelectedCourseNode();
        if (node == nullptr) {
            return;
        }
        
        switch (node->getType()) {
        case CourseNode::Type::COURSE:
            m_courseEditController->setCourse(reinterpret_cast<Course*>(node));
            break;
            
        case CourseNode::Type::SECTION:
            m_sectionEditController->setSection(reinterpret_cast<Section*>(node));
            break;
            
        case CourseNode::Type::TASK:
            m_taskEditController->setTask(reinterpret_cast<Task*>(node));
            break;
        }
    });
    
    connect(m_courseEditController.get(), &CourseEditController::changesCanceled, this, [this]() {
        m_infoPanelController->propose();
    });
    
    connect(m_sectionEditController.get(), &SectionEditController::changesCanceled, this, [this]() {
        m_infoPanelController->propose();
    });
    
    connect(m_taskEditController.get(), &TaskEditController::changesCanceled, this, [this]() {
        m_infoPanelController->propose();
    });
    
    connect(m_taskEditController.get(), &TaskEditController::testsOpened, this, [this]() {
       m_testsTableController->setTask(m_taskEditController->getCurrentTask()); 
    });
    
    connect(m_testsTableController.get(), &TestsTableController::testSelected, this, [this](Test* test) {
       m_testEditController->setTest(test); 
    });
    
    connect(m_testsTableController.get(), &TestsTableController::backButtonPressed, this, [this]() {
       m_taskEditController->propose(); 
    });
}

MainWindow::~MainWindow()
{
    ModelManager::close();
    m_ui.reset(nullptr);
}


/*void MainWindow::setInfoPanelEditable(bool editable)
{
    // Course properties
    m_ui->courseEditName->setEnabled(editable);
    m_ui->courseEditLectureHours->setEnabled(editable);
    m_ui->courseEditPracticeHours->setEnabled(editable);

    // Section properties
    m_ui->sectionEditName->setEnabled(editable);

    // Task properties
    m_ui->taskEditName->setEnabled(editable);
    m_ui->taskEditScore->setEnabled(editable);
    m_ui->taskEditText->setEnabled(editable);
    m_ui->taskEditSource->setEnabled(editable);


    m_ui->infoPanelButtons->setCurrentIndex(static_cast<int>(editable));
    m_ui->courseTree->setEnabled(!editable);
    
    m_ui->coursesList->setEnabled(!editable);
}*/
