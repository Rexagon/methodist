#include "ModelManager.h"

#include <stack>
#include <list>

#include "NetworkManager.h"
#include "Log.h"

QObject* ModelManager::m_parent = nullptr;
std::unique_ptr<CoursesListModel> ModelManager::m_coursesListModel = nullptr;
std::unique_ptr<CourseTreeModel> ModelManager::m_courseTreeModel = nullptr;
std::unique_ptr<TestsTableModel> ModelManager::m_testsTableModel = nullptr;
std::unique_ptr<SamplesTableModel> ModelManager::m_samplesTableModel = nullptr;

void ModelManager::init(QObject* parent)
{
    m_parent = parent;
}

void ModelManager::close()
{
    m_coursesListModel.reset();
    m_courseTreeModel.reset();
    m_testsTableModel.reset();
    m_samplesTableModel.reset();
}

void ModelManager::update()
{
    m_coursesListModel = std::make_unique<CoursesListModel>(m_parent);
    
    /*
    // Pollute with temp data
    
    // Course 1
    std::unique_ptr<Course> course1 = std::make_unique<Course>();
    course1->setName("Разработка и отладка приложений вслепую");
    course1->setLectureHourCount(100);
    course1->setPracticeHourCount(1);
    course1->setCreator("Ivan");
    
    int maxi = 4 + rand() % 11;
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
                
                std::unique_ptr<Test> test = std::make_unique<Test>();
                test->setInputData("a b c d");
                test->setOutputData("1");
                test->setScore(5);
                test->setSample(true);
                task->addTest(std::move(test));
                
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
    m_coursesListModel->addCourse(std::move(course2));*/
    
    NetworkManager::send(Request(SQL_QUERY_XML, "select_courses", 
    {
        {"sql_select", "SELECT * FROM course ORDER BY rowid ASC"}
    }), [](const Response& courses) 
    {
        courses.map([](size_t i, const Response::Row& courseData) {           
            NetworkManager::send(Request(SQL_QUERY_XML, "select_course_sections", 
            {
                {"sql_select", "SELECT * FROM section WHERE course_id=" + courseData.get("rowid").asString() }
            }), [courseData](const Response& sectionsData)
            {
                std::unique_ptr<Course> course = std::make_unique<Course>();
                
                course->setId(courseData.get("rowid").asUInt());
                course->setName(courseData.get("course_name").asString());
                course->setLectureHourCount(courseData.get("lecture_hours").asUInt());
                course->setPracticeHourCount(courseData.get("pracrice_hours").asUInt());
                course->setLaboratoryHourCount(courseData.get("laboratory_hours").asInt());
                
                //TODO: optimize this shit
                
                struct SectionData
                {
                    void init()
                    {
                        sectionObject = std::make_unique<Section>();
                        sectionObject->setId(id);
                        sectionObject->setName(name);
                        sectionObject->setCourse(course);
                    }
                    
                    size_t id;
                    QString name;
                    Course* course;
                    
                    bool hasParent;
                    size_t parentIndex;
                    std::list<SectionData*> subsections;
                    
                    std::unique_ptr<Section> sectionObject;
                };
                
                std::map<size_t, SectionData> sectionDataToId;
                                
                for (size_t i = 0; i < sectionsData.getRowCount(); ++i) {
                    Response::Row rowData = sectionsData.getRow(i);
                    Response::Cell parentSection = rowData.get("parent_section_id");
                    
                    SectionData sectionData;
                    sectionData.id = rowData.get("rowid").asUInt();
                    sectionData.name = rowData.get("section_name").asString();
                    sectionData.course = course.get();
                    
                    sectionData.hasParent = !parentSection.isNull();
                    if (sectionData.hasParent) {
                        sectionData.parentIndex = parentSection.asUInt();
                    }
                    else {
                        sectionData.parentIndex = 0;
                    }
                    
                    sectionDataToId[sectionData.id] = std::move(sectionData);
                }
                
                std::vector<SectionData*> sections;
                
                for (auto& it : sectionDataToId) {
                    if (it.second.hasParent) {
                        auto parentIt = sectionDataToId.find(it.second.parentIndex);
                        if (parentIt != sectionDataToId.end()) {
                            parentIt->second.subsections.push_back(&it.second);
                        }
                    }
                    else {
                        it.second.init();
                        sections.push_back(&it.second);
                    }
                }
                
                std::reverse(sections.begin(), sections.end());
                
                while (!sections.empty()) {
                    SectionData* sectionData = sections.back();
                    
                    if (sectionData->subsections.empty()) {
                        if (sectionData->hasParent) {
                            auto parentIt = sectionDataToId.find(sectionData->parentIndex);
                            if (parentIt != sectionDataToId.end()) {
                                auto& subsections = parentIt->second.subsections;
                                
                                for (auto it = subsections.begin(); it != subsections.end(); ++it) {
                                    if ((*it)->id == sectionData->id) {
                                        sectionData->init();
                                        parentIt->second.sectionObject->addSubsection(std::move(sectionData->sectionObject));
                                        subsections.erase(it);
                                        break;
                                    }
                                }
                            }
                        }
                        else {
                            course->addSection(std::move(sectionData->sectionObject));
                        }
                        
                        sections.pop_back();
                        continue;
                    }
                    
                    for (auto it = sectionData->subsections.rbegin(); it != sectionData->subsections.rend(); ++it) {
                        sections.push_back(*it);
                    }
                }
                
                // end of shit
                
                m_coursesListModel->addCourse(std::move(course));
            });
        });
    });
}

CoursesListModel* ModelManager::getCoursesListModel()
{
    if (m_coursesListModel == nullptr) {
        update();
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

SamplesTableModel* ModelManager::getSamplesTableModel()
{
    if (m_samplesTableModel == nullptr) {
        m_samplesTableModel = std::make_unique<SamplesTableModel>(m_parent);
    }
    
    return m_samplesTableModel.get();
}
