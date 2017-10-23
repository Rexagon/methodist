#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), 
    m_currentCourse(nullptr), m_ui(nullptr)
{
    m_ui = std::make_unique<Ui::MainWindow>();
    m_ui->setupUi(this);
    
    m_ui->pnlInfo->hide();
    
    m_coursesModel = std::make_unique<CoursesModel>(this);
    m_ui->lstCourses->setModel(m_coursesModel.get());
    
    // Actions
    connect(m_ui->lstCourses, QListView::pressed, this, [this](const QModelIndex& index) {
        size_t n = static_cast<size_t>(index.row());
        this->setCurrentCourse(m_coursesModel->getCourse(n));
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
    
    m_coursesModel->addCourse(std::move(course1));
    
    // Course 2
    std::unique_ptr<Course> course2 = std::make_unique<Course>();
    course2->setName("Итеративные способы разработки для чайников");
    course2->setLectureHourCount(1);
    course2->setPracticeHourCount(50);
    course2->setCreator("Ivan");
    m_coursesModel->addCourse(std::move(course2));
}

void MainWindow::setCurrentCourse(Course* course)
{
    m_currentCourse = course;
    if (course == nullptr) {
        m_ui->pnlInfo->hide();
    }
    else {
        m_ui->pnlInfo->show();        
        setInfoType("Курс");
        setInfoTitle(course->getName());
        
        m_treeNodesModel.reset(nullptr);
        m_treeNodesModel = std::make_unique<TreeNodesModel>(this);
        m_treeNodesModel->setCourse(course);
        m_ui->trvCourse->setModel(m_treeNodesModel.get());
        m_ui->trvCourse->expandAll();
    }
}

void MainWindow::setInfoType(const QString& type)
{
    m_ui->lblNodeType->setText(type);
}

void MainWindow::setInfoTitle(const QString& title)
{
    m_ui->lblNodeTitle->setText(title);
}
