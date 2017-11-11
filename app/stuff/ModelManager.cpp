#include "ModelManager.h"

QObject* ModelManager::m_parent = nullptr;
std::unique_ptr<CoursesListModel> ModelManager::m_coursesListModel = nullptr;
std::unique_ptr<CourseTreeModel> ModelManager::m_courseTreeModel = nullptr;
std::unique_ptr<TestsTableModel> ModelManager::m_testsTableModel = nullptr;

void ModelManager::init(QObject* parent)
{
    m_parent = parent;
}

void ModelManager::close()
{
    m_coursesListModel.reset();
    m_courseTreeModel.reset();
    m_testsTableModel.reset();
}

CoursesListModel* ModelManager::getCoursesListModel()
{
    if (m_coursesListModel == nullptr) {
        m_coursesListModel = std::make_unique<CoursesListModel>(m_parent);
        
        // Pollute with temp data
        
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
                    
                    subsection->addTask(std::move(task));
                }
                
                section->addSubsection(std::move(subsection));
            }
            
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
    
    return m_coursesListModel.get();
}

CourseTreeModel* ModelManager::getCourseTreeModel(Course* course)
{
    if (m_courseTreeModel == nullptr || m_courseTreeModel->getCourse() != course) {
        m_courseTreeModel = std::make_unique<CourseTreeModel>(m_parent);
        m_courseTreeModel->setCourse(course);
    }
    
    return m_courseTreeModel.get();
}

TestsTableModel* ModelManager::getTestsTableModel(Task* task)
{
    if (m_testsTableModel == nullptr || m_testsTableModel->getTask() == task) {
        m_testsTableModel = std::make_unique<TestsTableModel>(m_parent);
        m_testsTableModel->setTask(task);
    }
    
    return m_testsTableModel.get();
}
