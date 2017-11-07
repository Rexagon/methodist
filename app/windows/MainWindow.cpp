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
    
    connect(m_sideMenuController.get(), &SideMenuController::courseAdded, this, [this]() {
        CoursesListModel* model = ModelManager::getCoursesListModel();
        
       std::unique_ptr<Course> course = std::make_unique<Course>();       
       course->setName("Новый курс " + QString::number(model->getCourseCount()));
       Course* coursePtr = course.get();
       model->addCourse(std::move(course));
       m_sideMenuController->selectCourse(coursePtr);
       m_courseEditController->setCourse(coursePtr);
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
    
    connect(m_infoPanelController.get(), &InfoPanelController::deleteNodeButtonPressed, this, [this]() {
        CourseNode* node = m_courseTreeController->getSelectedCourseNode();
        if (node == nullptr) {
            return;
        }
        
        Course* currentCourse = m_courseTreeController->getCurrentCourse();
        
        switch (node->getType()) {
            case CourseNode::Type::COURSE:
            {
                ModelManager::getCoursesListModel()->removeCourse(currentCourse);
                m_sideMenuController->deselectAll();
                m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_DEFAULT);
                break;
            }
                
            case CourseNode::Type::SECTION:
            {
                Section* section = reinterpret_cast<Section*>(node);
                Section* parent = reinterpret_cast<Section*>(section->getParent());
                
                if (parent == nullptr) {
                    section->getCourse()->removeSection(section);
                }
                else {
                    parent->removeChild(node);
                }
                
                break;
            }
                
            case CourseNode::Type::TASK:
            {
                Task* task = reinterpret_cast<Task*>(node);
                Section* parent = task->getSection();
                parent->removeChild(node);
                parent->removeTask(task);
                
                break;
            }
        }
        
        if (node->getType() != CourseNode::Type::COURSE) {
            ModelManager::getCourseTreeModel(currentCourse)->update();
            m_courseTreeController->setCourse(currentCourse);
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
