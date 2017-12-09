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
       m_testEditController->setTest(test); 
    });

    // Actions: add elements
    connect(m_sideMenuController.get(), &SideMenuController::courseAdded, this, &MainWindow::addCourse);
    
    connect(m_infoPanelController.get(), &InfoPanelController::addSectionButtonPressed, this, &MainWindow::addSection);
    
    connect(m_infoPanelController.get(), &InfoPanelController::addTaskButtonPressed, this, &MainWindow::addTask);
    
    // Actions: info panel ui logic
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
                NetworkManager::send(Request(SQL_OPERATOR, "course_delete", {
                    {"sql_operator", "DELETE FROM course WHERE rowid=" + QString::number(currentCourse->getId())}
                }), [this, currentCourse](const Response& response)
                {
                    ModelManager::getCoursesListModel()->removeCourse(currentCourse);
                    m_sideMenuController->deselectAll();
                    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_DEFAULT);                    
                });
                
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
    
    connect(m_courseEditController.get(), &CourseEditController::changesSaved, this, [this]() {
        m_courseEditController->saveChanges();
        m_infoPanelController->propose();
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
    
    connect(m_testsTableController.get(), &TestsTableController::backButtonPressed, this, [this]() {
       m_taskEditController->propose(); 
    });
    
    Log::write("Controllers initialization finished");
    
    /*
    NetworkManager::send(Request(SQL_OPERATOR, "remove_trash", {
        {"sql_operator", "DELETE FROM section *"}
    }), [this](const Response& response) 
    {
        qDebug() << "asd" << response.getError();
    });
    */
    
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
    
    QString courseName = "Новый курс " + QString::number(model->getCourseCount() + 1);
    
    QString query = "INSERT INTO course (course_name) VALUES ('" + courseName + "')";
    //query += " RETURNING rowid";
    
    NetworkManager::send(Request(SQL_OPERATOR, "course_add", {
        {"sql_operator", query}
    }), [this, model, courseName](const Response& response)
    {
        std::unique_ptr<Course> course = std::make_unique<Course>();       
        Course* coursePtr = course.get();
        course->setName(courseName);
        
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
    
    QString sectionName;
    QString query = "INSERT INTO section ";
    
    if (nodeType == CourseNode::Type::COURSE) {
        Course* course = reinterpret_cast<Course*>(node);
        sectionName = "Новый раздел " + QString::number(course->getSectionCount() + 1);
        query += "(course_id, section_name) VALUES ("+QString::number(course->getId())+",'"+sectionName +"')";
    }
    else {
        Section* section = reinterpret_cast<Section*>(node);
        sectionName = "Новый подраздел " + QString::number(section->getSubsectionCount() + 1);
        query += "(course_id, section_name, parent_section_id) VALUES ("+QString::number(section->getCourse()->getId())+",'"+sectionName +"',"+
                QString::number(section->getId()) + ")";
    }
    
    //query += " RETURNING rowid";
    
    NetworkManager::send(Request(SQL_OPERATOR, "section_add", {
        {"sql_operator", query}
    }), [this, node, nodeType, sectionName](const Response& response)
    {
        std::unique_ptr<Section> section = std::make_unique<Section>();
        Section* sectionPtr = section.get();
        section->setName(sectionName);
        
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
    
    QString taskName = "Новая задача " + QString::number(section->getTaskCount() + 1);
    
    QString query = "INSERT INTO task_c (task_c_name, section_id) VALUES ('" + taskName + "', " + QString::number(section->getId()) + ")";
    //query += " RETURNING rowid";
    
    NetworkManager::send(Request(SQL_OPERATOR, "task_add", {
        {"sql_operator", query}
    }), [this, section, taskName](const Response& response)
    {
        std::unique_ptr<Task> task = std::make_unique<Task>();
        Task* taskPtr = task.get();
        task->setName(taskName);
        
        section->addTask(std::move(task));
        
        ModelManager::getCourseTreeModel(m_courseTreeController->getCurrentCourse())->update();
        m_courseTreeController->selectCourseNode(taskPtr);
        m_taskEditController->setTask(taskPtr); 
    });
}

void MainWindow::addTest()
{
    
}
