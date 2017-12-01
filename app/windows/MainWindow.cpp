#include "MainWindow.h"

#include <ui_MainWindow.h>
#include <iostream>

#include "../stuff/ModelManager.h"
#include "../stuff/NetworkManager.h"

#include "../stuff/Log.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), m_ui(nullptr)
{
    Log::write("Interface initialization started");
    
    m_ui = std::make_unique<Ui::MainWindow>();
    m_ui->setupUi(this);
    ModelManager::init(this);
    
    Log::write("Interface initialization finished");
    
    NetworkManager* network = new NetworkManager("ws://176.9.191.187:55577", this);
    
    Log::write("Controllers initialization started");
    
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
        course->setName("Новый курс " + QString::number(model->getCourseCount() + 1));
        Course* coursePtr = course.get();
        model->addCourse(std::move(course));
        m_sideMenuController->selectCourse(coursePtr);
        m_courseEditController->setCourse(coursePtr);
    });
    
    connect(m_courseTreeController.get(), &CourseTreeController::courseNodeSelected, this, [this](CourseNode* node) {
        m_infoPanelController->setCourseNode(node);
    });
    
    connect(m_infoPanelController.get(), &InfoPanelController::addSectionButtonPressed, this, [this]() {
        CourseNode* node = m_infoPanelController->getCurrentCourseNode();
        if (node == nullptr || node->getType() == CourseNode::Type::TASK) {
            return;
        }
        
        std::unique_ptr<Section> newSection = std::make_unique<Section>();
        Section* sectionPtr = newSection.get();
        
        if (node->getType() == CourseNode::Type::COURSE) {
            Course* course = reinterpret_cast<Course*>(node);
            newSection->setName("Новый раздел " + QString::number(course->getSectionCount() + 1));
            course->addSection(std::move(newSection));
        }
        else {
            Section* section = reinterpret_cast<Section*>(node);
            newSection->setName("Новый подраздел " + QString::number(section->getSubsectionCount() + 1));
            section->addSubsection(std::move(newSection));
        }
        
        ModelManager::getCourseTreeModel(m_courseTreeController->getCurrentCourse())->update();
        m_courseTreeController->selectCourseNode(sectionPtr);
        m_sectionEditController->setSection(sectionPtr);
    });
    
    connect(m_infoPanelController.get(), &InfoPanelController::addTaskButtonPressed, this, [this]() {
        CourseNode* node = m_infoPanelController->getCurrentCourseNode();
        if (node == nullptr || node->getType() != CourseNode::Type::SECTION) {
            return;
        }
        
        std::unique_ptr<Task> newTask = std::make_unique<Task>();
        Task* taskPtr = newTask.get();
        
        Section* section = reinterpret_cast<Section*>(node);
        newTask->setName("Новая задача " + QString::number(section->getTaskCount() + 1));
        section->addTask(std::move(newTask));
        
        ModelManager::getCourseTreeModel(m_courseTreeController->getCurrentCourse())->update();
        m_courseTreeController->selectCourseNode(taskPtr);
        m_taskEditController->setTask(taskPtr);
    });
    
    connect(m_infoPanelController.get(), &InfoPanelController::editNodeButtonPressed, this, [this]() {
        CourseNode* node = m_infoPanelController->getCurrentCourseNode();
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
        CourseNode* node = m_infoPanelController->getCurrentCourseNode();
        if (node == nullptr) {
            return;
        }
        
        Course* currentCourse = m_courseTreeController->getCurrentCourse();
        CourseNode::Type type = node->getType();
        
        switch (type) {
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
                CourseNode* parent = section->getParent();
                
                m_courseTreeController->selectCourseNode(parent);
                
                if (parent->getType() == CourseNode::Type::COURSE) {
                    currentCourse->removeSection(section);
                }
                else {
                    reinterpret_cast<Section*>(parent)->removeSubsection(section);
                }
                
                break;
            }
                
            case CourseNode::Type::TASK:
            {
                Task* task = reinterpret_cast<Task*>(node);
                Section* parent = task->getSection();
                
                m_courseTreeController->selectCourseNode(parent);
                
                parent->removeTask(task);
                
                break;
            }
        }
        
        if (type != CourseNode::Type::COURSE) {
            ModelManager::getCourseTreeModel(currentCourse)->update();
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
    
    Log::write("Controllers initialization finished");
    
    network->sendRequest(Request({
        {"cmd_type", "16"},
                                     {"arm_task", "test"},
        {"arm_task_id", "123"},
        //{"sql_operator", "INSERT INTO course (course_name, lecture_hours, pracrice_hours) VALUES ('wer', 10, 5)"}
        {"sql_select", "SELECT * FROM course"}
    }));
}

MainWindow::~MainWindow()
{
    ModelManager::close();
    m_ui.reset(nullptr);
}
