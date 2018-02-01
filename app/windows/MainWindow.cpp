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
    
    m_courseEditController = std::make_unique<CourseEditController>(m_ui.get(), this);
    m_sectionEditController = std::make_unique<SectionEditController>(m_ui.get(), this);
    m_taskEditController = std::make_unique<TaskEditController>(m_ui.get(), this);
    m_testEditController = std::make_unique<TestEditController>(m_ui.get(), this);
    
    m_courseTreeController = std::make_unique<CourseTreeController>(m_ui.get(), this);
    m_sideMenuController = std::make_unique<SideMenuController>(m_ui.get(), this);
    m_testsTableController = std::make_unique<TestsTableController>(m_ui.get(), this);
  
    
    // Actions: select elements
    connect(m_sideMenuController.get(), &SideMenuController::courseSelected, this, [this](Course* course) {
        m_courseTreeController->setCourse(course);
    });
    
    connect(m_courseTreeController.get(), &CourseTreeController::courseNodeSelected, this, &MainWindow::selectNode);
    
    connect(m_testsTableController.get(), &TestsTableController::testSelected, this, [this](Test* test) {
       m_testEditController->setTest(test);
    });

    // Actions: add elements
    connect(m_sideMenuController.get(), &SideMenuController::courseAdded, this, &MainWindow::addCourse);
    
    connect(m_courseEditController.get(), &CourseEditController::addSectionButtonPressed, this, &MainWindow::addSection);
    
    connect(m_sectionEditController.get(), &SectionEditController::addSubsectionButtonPressed, this, &MainWindow::addSubsection);
    
    connect(m_sectionEditController.get(), &SectionEditController::addTaskButtonPressed, this, &MainWindow::addTask);
    
    connect(m_testEditController.get(), &TestEditController::addTestButtonPressed, this, &MainWindow::addTest);
    
    // Actions: edit nodes
    connect(m_courseEditController.get(), &CourseEditController::editButtonPressed, this, [this]() {
        m_courseEditController->setEditable(true);
        m_courseEditController->propose();
    });
    
    connect(m_sectionEditController.get(), &SectionEditController::editButtonPressed, this, [this]() {
        m_sectionEditController->setEditable(true);
        m_sectionEditController->propose();
    });
    
    connect(m_taskEditController.get(), &TaskEditController::editButtonPressed, this, [this]() {
        m_taskEditController->setEditable(true);
        m_taskEditController->propose();
    });
    
    connect(m_testEditController.get(), &TestEditController::editButtonPressed, this, [this]() {
        m_testEditController->setEditable(true);
        m_testEditController->propose();
    });
    
    // Actions: delete nodes
    connect(m_courseEditController.get(), &CourseEditController::deleteButtonPressed, this, &MainWindow::deleteCourse);
    
    connect(m_sectionEditController.get(), &SectionEditController::deleteButtonPressed, this, &MainWindow::deleteSection);
    
    connect(m_taskEditController.get(), &TaskEditController::deleteButtonPressed, this, &MainWindow::deleteTask);
    
    connect(m_testEditController.get(), &TestEditController::deleteButtonPressed, this, &MainWindow::deleteTest);
    
    // Actions: save edit
    connect(m_courseEditController.get(), &CourseEditController::saveChangesButtonPressed, this, [this]() {
        m_courseEditController->saveCurrentCourse();
        m_courseEditController->setEditable(false);
        m_courseEditController->propose();
    });
    
    connect(m_sectionEditController.get(), &SectionEditController::saveChangesButtonPressed, this, [this]() {
        m_sectionEditController->saveCurrentSection();
        m_sectionEditController->setEditable(false);
        m_sectionEditController->propose();
    });
    
    connect(m_taskEditController.get(), &TaskEditController::saveChangesButtonPressed, this, [this]() {
        m_taskEditController->saveCurrentTask();
        m_taskEditController->setEditable(false);
        m_taskEditController->propose();
    });
    
    connect(m_testEditController.get(), &TestEditController::saveChangesButtonPressed, this, [this]() {
        m_testEditController->saveCurrentTest();
        m_testEditController->setEditable(false);
        m_testEditController->propose();
    });

    // Actions: cancel edit
    connect(m_courseEditController.get(), &CourseEditController::cancelChangesButtonPressed, this, [this]() {
        m_courseEditController->setEditable(false);
        m_courseEditController->propose();
    });
    
    connect(m_sectionEditController.get(), &SectionEditController::cancelChangesButtonPressed, this, [this]() {
        m_sectionEditController->setEditable(false);
        m_sectionEditController->propose();
    });

    connect(m_taskEditController.get(), &TaskEditController::cancelChangesButtonPressed, this, [this]() {
        m_taskEditController->setEditable(false);
        m_taskEditController->propose();
    });

    connect(m_testEditController.get(), &TestEditController::cancelChangesButtonPressed, this, [this]() {
        m_testEditController->setEditable(false);
        m_testEditController->propose();
    });

    // Actions: tests table
    connect(m_taskEditController.get(), &TaskEditController::openTestsButtonPressed, this, [this]() {
        m_testsTableController->setTask(m_taskEditController->getCurrentTask());
        m_testEditController->setEditable(false);
        m_testEditController->setTest(nullptr);
    });
    
    connect(m_testEditController.get(), &TestEditController::exitTestsButtonPressed, this, [this]() {
        m_taskEditController->propose();
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

void MainWindow::selectNode(CourseNode* node)
{
    if (node == nullptr) {
        return;
    }
    
    switch (node->getType()) {
    case CourseNode::Type::COURSE:
        m_courseEditController->setEditable(false);
        m_courseEditController->setCourse(reinterpret_cast<Course*>(node));
        break;
        
    case CourseNode::Type::SECTION:
        m_sectionEditController->setEditable(false);
        m_sectionEditController->setSection(reinterpret_cast<Section*>(node));
        break;
        
    case CourseNode::Type::TASK:
        m_taskEditController->setEditable(false);
        m_taskEditController->setTask(reinterpret_cast<Task*>(node));
        break;
        
    case CourseNode::Type::TEST:
        m_testEditController->setEditable(false);
        m_testEditController->setTest(reinterpret_cast<Test*>(node));
        break;
    }
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

void MainWindow::deleteCourse()
{
    Course* course = m_courseEditController->getCurrentCourse();
    
    Course::dbDelete(course, [this, course]() {
        ModelManager::getCoursesListModel()->removeCourse(course);
        
        m_sideMenuController->deselectAll();
        m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_DEFAULT);
    });
}

void MainWindow::addSection()
{
    Course* course = m_courseEditController->getCurrentCourse();
    if (course == nullptr) {
        return;
    }
    
    Section::Data sectionData;
    sectionData.name = "Новый раздел " + QString::number(course->getSectionCount() + 1);
    sectionData.course = course;
    
    Section::dbCreate(sectionData, [this, course](std::unique_ptr<Section> section) {
        Section* sectionPtr = section.get();
        
        course->addSection(std::move(section));
        
        ModelManager::getCourseTreeModel(course)->update();
        m_courseTreeController->selectCourseNode(sectionPtr);
        m_sectionEditController->setSection(sectionPtr); 
    });
}

void MainWindow::addSubsection()
{
    Section* section = m_sectionEditController->getCurrentSection();
    if (section == nullptr) {
        return;
    }
    
    Section::Data subsectionData;
    subsectionData.name = "Новый подраздел " + QString::number(section->getSubsectionCount() + 1);
    subsectionData.course = section->getCourse();
    subsectionData.parentSection = section;
    
    Section::dbCreate(subsectionData, [this, section](std::unique_ptr<Section> subsection) {
        Section* subsectionPtr = subsection.get();
        
        section->addSubsection(std::move(subsection));
        
        ModelManager::getCourseTreeModel(section->getCourse())->update();
        m_courseTreeController->selectCourseNode(subsectionPtr);
        m_sectionEditController->setSection(subsectionPtr); 
    });
}

void MainWindow::deleteSection()
{
    Section* section = m_sectionEditController->getCurrentSection();
    
    Section::dbDelete(section, [this, section]() {
        CourseNode* parent = section->getParent();
        Course* course = section->getCourse();

        m_courseTreeController->selectCourseNode(parent);

        if (course == parent) {
            course->removeSection(section);
        }
        else {
            reinterpret_cast<Section*>(parent)->removeSubsection(section);
        }
        
        ModelManager::getCourseTreeModel(course)->update();
    });
}

void MainWindow::addTask()
{
    Section* section = m_sectionEditController->getCurrentSection();
    if (section == nullptr) {
        return;
    }
    
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

void MainWindow::deleteTask()
{
    Task* task = m_taskEditController->getCurrentTask();
    
    Task::dbDelete(task, [this, task]() {
        Section* parent = task->getSection();
        Course* course = parent->getCourse();

        m_courseTreeController->selectCourseNode(parent);

        parent->removeTask(task);

        ModelManager::getCourseTreeModel(course)->update();
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

void MainWindow::deleteTest()
{
    Test* test = m_testEditController->getCurrentTest();
    
    Test::dbDelete(test, [this, test]() {
       Task* parent = test->getTask();
       
       m_testsTableController->deselectAll();
       
       parent->removeTest(test);
       
       ModelManager::getTestsTableModel(parent)->update();
       
       m_testsTableController->propose();
    });
}

