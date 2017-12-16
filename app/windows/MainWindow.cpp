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
    Log::write("Interface initialization finished");
    
    NetworkManager::init("ws://176.9.191.187:55577");
    
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
  
    
    // Actions: select elements
    connect(m_sideMenuController.get(), &SideMenuController::courseSelected, this, [this](Course* course) {
        m_courseTreeController->setCourse(course);
    });
    
    connect(m_courseTreeController.get(), &CourseTreeController::courseNodeSelected, this, [this](CourseNode* node) {
        m_infoPanelController->setCourseNode(node);
    });
    
    connect(m_testsTableController.get(), &TestsTableController::testSelected, this, [this](Test* test) {
       m_infoPanelController->setCourseNode(test); 
    });

    // Actions: add elements
    connect(m_sideMenuController.get(), &SideMenuController::courseAdded, this, &MainWindow::addCourse);
    
    connect(m_infoPanelController.get(), &InfoPanelController::addSectionButtonPressed, this, &MainWindow::addSection);
    
    connect(m_infoPanelController.get(), &InfoPanelController::addTaskButtonPressed, this, &MainWindow::addTask);
    
    connect(m_infoPanelController.get(), &InfoPanelController::addTestButtonPressed, this, &MainWindow::addTest);
    
    // Actions: info panel ui logic
    connect(m_infoPanelController.get(), &InfoPanelController::editNodeButtonPressed, this, &MainWindow::editNode);
    
    connect(m_infoPanelController.get(), &InfoPanelController::deleteNodeButtonPressed, this, &MainWindow::deleteNode);

    connect(m_infoPanelController.get(), &InfoPanelController::exitNodeButtonPressed, this, [this]() {
       m_taskEditController->propose();
    });
    
    // Actions: save edit
    connect(m_courseEditController.get(), &CourseEditController::changesSaved, this, [this]() {
        m_courseEditController->saveChanges();
        m_infoPanelController->propose();
    });
    
    connect(m_sectionEditController.get(), &SectionEditController::changesSaved, this, [this]() {
        m_sectionEditController->saveChanges();
        m_infoPanelController->propose();
    });
    
    connect(m_taskEditController.get(), &TaskEditController::changesSaved, this, [this]() {
        m_taskEditController->saveChanges();
        m_infoPanelController->propose();
    });

    // Actions: cancel edit
    connect(m_courseEditController.get(), &CourseEditController::changesCanceled, this, [this]() {
        m_infoPanelController->propose();
        m_courseTreeController->propose();
    });
    
    connect(m_sectionEditController.get(), &SectionEditController::changesCanceled, this, [this]() {
        m_infoPanelController->propose();
        m_courseTreeController->propose();
    });

    connect(m_taskEditController.get(), &TaskEditController::changesCanceled, this, [this]() {
        m_infoPanelController->propose();
        m_courseTreeController->propose();
    });

    connect(m_testEditController.get(), &TestEditController::changesCanceled, this, [this]() {
        m_infoPanelController->propose();
    });

    // Actions: task edit ui logic
    connect(m_taskEditController.get(), &TaskEditController::testsOpened, this, [this]() {
        m_testsTableController->setTask(m_taskEditController->getCurrentTask());
    });
        
    
    Log::write("Controllers initialization finished");
    
    ModelManager::init(this);
}

MainWindow::~MainWindow()
{
    ModelManager::close();
    NetworkManager::close();
    m_ui.reset(nullptr);
}

void MainWindow::addCourse()
{
    CoursesListModel* model = ModelManager::getCoursesListModel();
    
    Course::Data courseData("Новый курс " + QString::number(model->getCourseCount() + 1));
    
    Course::dbCreate(courseData, [this, model](std::unique_ptr<Course> course) {
        Course* coursePtr = course.get();
        model->addCourse(std::move(course));
        
        m_sideMenuController->selectCourse(coursePtr);
        m_courseEditController->setCourse(coursePtr);  
    });  
}

void MainWindow::addSection()
{
    CourseNode* node = m_infoPanelController->getCurrentCourseNode();
    if (node == nullptr || node->getType() == CourseNode::Type::TASK) {
        return;
    }
    
    CourseNode::Type nodeType = node->getType();
    
    Section::Data sectionData;
    
    if (nodeType == CourseNode::Type::COURSE) {
        Course* course = reinterpret_cast<Course*>(node);
        sectionData.name = "Новый раздел " + QString::number(course->getSectionCount() + 1);
        sectionData.course = course;
    }
    else {
        Section* section = reinterpret_cast<Section*>(node);
        sectionData.name = "Новый подраздел " + QString::number(section->getSubsectionCount() + 1);
        sectionData.course = section->getCourse();
        sectionData.parentSection = section;
    }
    
    Section::dbCreate(sectionData, [this, node, nodeType](std::unique_ptr<Section> section) {
        Section* sectionPtr = section.get();
        
        if (nodeType == CourseNode::Type::COURSE) {
            reinterpret_cast<Course*>(node)->addSection(std::move(section));
        }
        else {
            reinterpret_cast<Section*>(node)->addSubsection(std::move(section));
        }
        
        ModelManager::getCourseTreeModel(m_courseTreeController->getCurrentCourse())->update();
        m_courseTreeController->selectCourseNode(sectionPtr);
        m_sectionEditController->setSection(sectionPtr); 
    });
}

void MainWindow::addTask()
{
    CourseNode* node = m_infoPanelController->getCurrentCourseNode();
    if (node == nullptr || node->getType() != CourseNode::Type::SECTION) {
        return;
    }
    
    Section* section = reinterpret_cast<Section*>(node);
    
    Task::Data taskData;
    taskData.name = "Новая задача " + QString::number(section->getTaskCount() + 1);
    taskData.section = section;
    
    Task::dbCreate(taskData, [this, section](std::unique_ptr<Task> task) {
        Task* taskPtr = task.get();;
        
        section->addTask(std::move(task));
        
        ModelManager::getCourseTreeModel(m_courseTreeController->getCurrentCourse())->update();
        m_courseTreeController->selectCourseNode(taskPtr);
        m_taskEditController->setTask(taskPtr); 
    });
}

void MainWindow::addTest()
{
    Task* task = m_testsTableController->getCurrentTask();
    if (task == nullptr) {
        return;
    }
    
    Test::Data testData;
    testData.task = task;
    
    Test::dbCreate(testData, [this, task](std::unique_ptr<Test> test) {
        Test* testPtr = test.get();
        task->addTest(std::move(test));
        
        ModelManager::getTestsTableModel(task)->update();
        m_testsTableController->selectTest(testPtr);
        m_testEditController->setTest(testPtr); 
    });
}

void MainWindow::editNode()
{
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
        
    case Course::Type::TEST:
        m_testEditController->setTest(reinterpret_cast<Test*>(node));
        break;
    }
}

void MainWindow::deleteNode()
{
    CourseNode* node = m_infoPanelController->getCurrentCourseNode();
    if (node == nullptr) {
        return;
    }

    Course* currentCourse = m_courseTreeController->getCurrentCourse();
    CourseNode::Type type = node->getType();

    switch (type) {
        case CourseNode::Type::COURSE:
        {
            Course::dbDelete(currentCourse, [this, currentCourse]() {
                ModelManager::getCoursesListModel()->removeCourse(currentCourse);
                m_sideMenuController->deselectAll();
                m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_DEFAULT);
            });

            break;
        }

        case CourseNode::Type::SECTION:
        {
            Section* section = reinterpret_cast<Section*>(node);

            Section::dbDelete(section, [this, section, currentCourse]() {
                CourseNode* parent = section->getParent();

                m_courseTreeController->selectCourseNode(parent);

                if (parent->getType() == CourseNode::Type::COURSE) {
                    currentCourse->removeSection(section);
                }
                else {
                    reinterpret_cast<Section*>(parent)->removeSubsection(section);
                }
                ModelManager::getCourseTreeModel(currentCourse)->update();
            });

            break;
        }

        case CourseNode::Type::TASK:
        {
            Task* task = reinterpret_cast<Task*>(node);

            Task::dbDelete(task, [this, task, currentCourse]() {
                Section* parent = task->getSection();

                m_courseTreeController->selectCourseNode(parent);

                parent->removeTask(task);

                ModelManager::getCourseTreeModel(currentCourse)->update();
            });

            break;
        }
        
        case CourseNode::Type::TEST:
        {
            Test* test = reinterpret_cast<Test*>(node);
            
            Test::dbDelete(test, [this, test, currentCourse]() {
               Task* parent = test->getTask();
               
               m_testsTableController->deselectAll();
               
               parent->removeTest(test);
               
               ModelManager::getTestsTableModel(parent)->update();
            });
        
            break;
        }
    }
}
