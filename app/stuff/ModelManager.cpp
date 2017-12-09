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
QEventLoop ModelManager::m_synchronizationLoop;

// HELPER STRUCTS
struct CourseData;
struct SectionData;
struct TaskData;
struct TestData;

struct CourseData
{
    void init()
    {
        object = std::make_unique<Course>();
    }
    
    std::list<SectionData*> sections;
    
    std::unique_ptr<Course> object;
};

struct SectionData
{
    void init()
    {
        object = std::make_unique<Section>();
    }
    
    size_t courseIndex;
    
    bool hasParent;
    size_t parentIndex;
    std::list<SectionData*> subsections;
    
    std::unique_ptr<Section> object;
};

struct TaskData
{
    void init()
    {
        object = std::make_unique<Task>();
    }
    
    size_t sectionIndex;
    
    std::unique_ptr<Task> object;
};

struct TestData
{
    void init()
    {
        object = std::make_unique<Test>();
    }
    
    size_t taskIndex;
    
    std::unique_ptr<Test> object;
};
// END OF HELPER STRUCTS


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
    
    std::map<size_t, CourseData> coursesData;
    std::map<size_t, SectionData> sectionsData;
    std::map<size_t, TaskData> tasksData;
    std::map<size_t, TestData> testsData;
    
    Async::Functions requests;
    
    // Getting courses
    requests.push_back([&coursesData](Async::Callback callback) {
        NetworkManager::send(Request(SQL_QUERY_XML, "select_courses", {
            {"sql_select", "SELECT * FROM course ORDER BY rowid ASC"}
        }), [&coursesData, callback](const Response& courses)
        {
            for (size_t i = 0; i < courses.getRowCount(); ++i) {
                Response::Row rowData = courses.getRow(i);
                
                CourseData course;
                
                course.init();
                course.object->setId(rowData.get("rowid").asUInt());
                course.object->setName(rowData.get("course_name").asString());
                course.object->setLectureHourCount(rowData.get("lecture_hours").asUInt());
                course.object->setPracticeHourCount(rowData.get("pracrice_hours").asUInt());
                course.object->setLaboratoryHourCount(rowData.get("laboratory_hours").asUInt());
                                
                coursesData[course.object->getId()] = std::move(course);
            }
            
            callback(Async::Error());
        });
    });
    
    // Getting sections
    requests.push_back([&sectionsData](Async::Callback callback) {
        NetworkManager::send(Request(SQL_QUERY_XML, "select_sections", {
            {"sql_select", "SELECT * FROM section ORDER BY rowid ASC"}
        }), [&sectionsData, callback](const Response& sections)
        {
            for (size_t i = 0; i < sections.getRowCount(); ++i) {
                Response::Row rowData = sections.getRow(i);
                
                SectionData section;
                
                section.init();
                section.object->setId(rowData.get("rowid").asUInt());
                section.object->setName(rowData.get("section_name").asString());
                
                section.courseIndex = rowData.get("course_id").asUInt();
                
                Response::Cell parentSection = rowData.get("parent_section_id");
                section.hasParent = !parentSection.isNull();
                if (section.hasParent) {
                    section.parentIndex = parentSection.asUInt();
                }
                else {
                    section.parentIndex = 0;
                }
                
                sectionsData[section.object->getId()] = std::move(section);
            }
            
            callback(Async::Error());
        });
    });
    
    // Getting tasks
    requests.push_back([&tasksData](Async::Callback callback) {
        NetworkManager::send(Request(SQL_QUERY_XML, "select_tasks", {
            {"sql_select", "SELECT * FROM task_c ORDER BY rowid ASC"}
        }), [&tasksData, callback](const Response& tasks)
        {
            for (size_t i = 0; i < tasks.getRowCount(); ++i) {
                Response::Row rowData = tasks.getRow(i);
                
                TaskData task;
                
                task.init();
                task.object->setId(rowData.get("rowid").asUInt());
                task.object->setName(rowData.get("task_c_name").asString());
                task.object->setText(rowData.get("task_c_text").asString());
                //task.object->setInputData(inputData);
                //task.object->setOutputData(outputData);
                task.object->setSource(rowData.get("task_c_source").asString());
                task.object->setScore(rowData.get("task_c_score").asUInt());
                
                task.sectionIndex = rowData.get("section_id").asUInt();
                
                tasksData[task.object->getId()] = std::move(task);
            }
            
            callback(Async::Error());
        });
    });
    
    // Getting tests
    requests.push_back([&testsData](Async::Callback callback) {
        NetworkManager::send(Request(SQL_QUERY_XML, "select_tests", {
            {"sql_select", "SELECT * FROM test_c ORDER BY rowid ASC"}
        }), [&testsData, callback](const Response& tests)
        {
            for (size_t i = 0; i < tests.getRowCount(); ++i) {
                Response::Row rowData = tests.getRow(i);
                
                TestData test;
                
                test.init();
                test.object->setId(rowData.get("rowid").asUInt());
                test.object->setInputData(rowData.get("test_c_input_data").asString());
                test.object->setOutputData(rowData.get("test_c_output_data").asString());
                test.object->setRequired(rowData.get("is_required").asBool());
                test.object->setScore(rowData.get("test_c_score").asUInt());
                
                test.taskIndex = rowData.get("task_c_id").asUInt();
                
                testsData[test.object->getId()] = std::move(test);
            }
            
            callback(Async::Error());
        });
    });
    
    Async::parallel(requests, [&coursesData, &sectionsData, &tasksData, &testsData](const Async::Error& error) {        
        if (!error.isNull()) {
            m_synchronizationLoop.exit();
            throw std::runtime_error(error.getMessage());
        }
        
        for (auto& it : testsData) {
            auto taskIt = tasksData.find(it.second.taskIndex);
            if (taskIt != tasksData.end()) {
                taskIt->second.object->addTest(std::move(it.second.object));
            }
        }
        
        for (auto& it : tasksData) {
            auto sectionIt = sectionsData.find(it.second.sectionIndex);
            if (sectionIt != sectionsData.end()) {
                sectionIt->second.object->addTask(std::move(it.second.object));
            }
        }
        
        std::vector<SectionData*> sections;
        
        for (auto& it : sectionsData) {
            if (it.second.hasParent) {
                auto parentIt = sectionsData.find(it.second.parentIndex);
                if (parentIt != sectionsData.end()) {
                    parentIt->second.subsections.push_back(&it.second);
                }
            }
            else {
                sections.push_back(&it.second);
            }
        }
        
        std::reverse(sections.begin(), sections.end());
        
        while (!sections.empty()) {
            SectionData* section = sections.back();
            
            if (section->subsections.empty()) {
                if (section->hasParent) {
                    auto parentIt = sectionsData.find(section->parentIndex);
                    if (parentIt != sectionsData.end()) {
                        auto& subsections = parentIt->second.subsections;
                        
                        for (auto it = subsections.begin(); it != subsections.end(); ++it) {
                            if ((*it)->object->getId() == section->object->getId()) {
                                parentIt->second.object->addSubsection(std::move(section->object));
                                subsections.erase(it);
                                break;
                            }
                        }
                    }
                }
                else {
                    auto courseIt = coursesData.find(section->courseIndex);
                    if (courseIt != coursesData.end()) {
                        courseIt->second.object->addSection(std::move(section->object));
                    }
                }
                
                sections.pop_back();
                continue;
            }
            
            for (auto it = section->subsections.rbegin(); it != section->subsections.rend(); ++it) {
                sections.push_back(*it);
            }
        }
        
        for (auto& it : coursesData) {
            m_coursesListModel->addCourse(std::move(it.second.object));
        }
        
        m_synchronizationLoop.exit();
    });
    
    m_synchronizationLoop.exec();
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
    if (m_testsTableModel == nullptr || m_testsTableModel->getTask() != task) {
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
