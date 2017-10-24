#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), 
    m_selectedNode(nullptr), m_ui(nullptr), m_currentPage(DEFAULT)
{
    m_ui = std::make_unique<Ui::MainWindow>();
    m_ui->setupUi(this);
    
    m_ui->infoPanel->hide();
    m_ui->mainWorkspace->setCurrentIndex(m_currentPage);
    
    m_coursesListModel = std::make_unique<CoursesModel>(this);
    m_ui->coursesList->setModel(m_coursesListModel.get());
    
    // Actions
    connect(m_ui->coursesList, &QListView::pressed, this, [this](const QModelIndex& index) {
        m_currentPage = COURSE_TREE;
        m_ui->mainWorkspace->setCurrentIndex(m_currentPage);
        
        Course* course = m_coursesListModel->getCourse(static_cast<size_t>(index.row()));
        
        if (course != nullptr) {
            m_courseTreeModel = std::make_unique<TreeNodesModel>(this);
            m_courseTreeModel->setCourse(course);
            m_ui->courseTree->setModel(m_courseTreeModel.get());
            m_ui->courseTree->setExpanded(m_courseTreeModel->index(0, 0), true);
        }
        
        updateInfoPanel(course);
    });
    
    connect(m_ui->courseTree, &QTreeView::pressed, this, [this](const QModelIndex& index) {
        TreeNode* item = reinterpret_cast<TreeNode*>(index.internalPointer());
        updateInfoPanel(item);        
    });
    
    connect(m_ui->editNodeButton, &QPushButton::pressed, this, [this]() {
        if (m_selectedNode == nullptr) {
            return;
        }
        
        switch (m_selectedNode->getType()) {
        case TreeNode::Type::COURSE:
            m_currentPage = COURSE_EDIT;
            break;
            
        case TreeNode::Type::SECTION:
            break;
            
        case TreeNode::Type::TASK:
            break;
        }
        
        m_ui->mainWorkspace->setCurrentIndex(m_currentPage);
    });
    
    connect(m_ui->rejectCourseChangesButton, &QPushButton::pressed, this, [this]() {
        m_currentPage = COURSE_TREE;
        m_ui->mainWorkspace->setCurrentIndex(m_currentPage);
    });
    
    // Request data
    updateData();
}

MainWindow::~MainWindow()
{
    m_ui.reset(nullptr);
}

void MainWindow::updateData()
{
    // Test data
    
    // Course 1
    std::unique_ptr<Course> course1 = std::make_unique<Course>();
    course1->setName("Разработка и отладка приложений вслепую");
    course1->setLectureHourCount(100);
    course1->setPracticeHourCount(1);
    course1->setCreator("Ivan");
    
    int maxi = rand() % 15;
    for (int i = 0; i < maxi; ++i) {
        std::unique_ptr<Section> section = std::make_unique<Section>();
        
        section->setName(QString("Раздел ") + QString::number(i + 1));
        
        int maxj = rand() % 15;
        for (int j = 0; j < maxj; ++j) {
            std::unique_ptr<Section> subsection = std::make_unique<Section>();
            
            subsection->setName(QString("Подраздел ") + QString::number(i + 1) + QString(".")+ QString::number(j + 1));
            
            int maxk = rand() % 5;
            for (int k = 0; k < maxk; ++k) {
                std::unique_ptr<Task> task = std::make_unique<Task>();
                
                task->setName(QString("Задача ") + QString::number(k + 1));
                
                subsection->addChild(task.get());
                subsection->addTask(std::move(task));
            }
            
            section->addChild(subsection.get());
            course1->addSection(std::move(subsection));
        }
        
        course1->addChild(section.get());
        course1->addSection(std::move(section));
    }
    
    m_coursesListModel->addCourse(std::move(course1));
    
    // Course 2
    std::unique_ptr<Course> course2 = std::make_unique<Course>();
    course2->setName("Итеративные способы разработки для чайников");
    course2->setLectureHourCount(1);
    course2->setPracticeHourCount(50);
    course2->setCreator("Ivan");
    m_coursesListModel->addCourse(std::move(course2));
}

void MainWindow::updateInfoPanel(TreeNode* node)
{
    m_selectedNode = node;
    
    if (node == nullptr) {
        m_ui->infoPanel->hide();
    }
    else {
        m_ui->infoPanel->show();
        
        m_ui->infoPanelPages->setCurrentIndex(static_cast<int>(node->getType()));
        
        switch (node->getType()) {
        case TreeNode::Type::COURSE:
        {
            Course* course = reinterpret_cast<Course*>(node);
            m_ui->nodeType->setText("Курс");
            m_ui->nodeTitle->setText(course->getName());
            
            m_ui->courseInfoLectureHours->setText(QString::number(course->getLectureHourCount()));
            m_ui->courseInfoPracticeHours->setText(QString::number(course->getPracticeHourCount()));
            m_ui->courseInfoCreator->setText(course->getCreator());
            break;
        }
            
            
        case TreeNode::Type::SECTION:
        {
            Section* section = reinterpret_cast<Section*>(node);
            m_ui->nodeType->setText("Раздел");
            m_ui->nodeTitle->setText(section->getName());
            break;
        }
            
        
        case TreeNode::Type::TASK:
        {
            Task* task = reinterpret_cast<Task*>(node);
            m_ui->nodeType->setText("Задача");
            m_ui->nodeTitle->setText(task->getName());
            break;
        }
            
        }
    }
}
