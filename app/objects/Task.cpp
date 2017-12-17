#include "Task.h"

#include "../stuff/NetworkManager.h"
#include "Course.h"
#include "Section.h"

Task::Task() :
    CourseNode(CourseNode::Type::TASK)
{
}

Task::~Task()
{
}

void Task::setId(size_t id)
{
    m_data.id = id;
}

size_t Task::getId() const
{
    return m_data.id;
}

void Task::setName(const QString& name)
{
    m_data.name = name;
}

QString Task::getName() const
{
    return m_data.name;
}

void Task::setText(const QString& text)
{
    m_data.text = text;
}

QString Task::getText() const
{
    return m_data.text;
}

void Task::setInputData(const QString& text)
{
    m_data.inputData = text;
}

QString Task::getInputData() const
{
    return m_data.inputData;
}

void Task::setOutputData(const QString& text)
{
    m_data.outputData = text;
}

QString Task::getOutputData() const
{
    return m_data.outputData;
}

void Task::setSource(const QString& source)
{
    m_data.source = source;
}

QString Task::getSource() const
{
    return m_data.source;
}

void Task::setScore(size_t score)
{
    m_data.score = score;
}

size_t Task::getScore() const
{
    return m_data.score;
}

void Task::setSection(Section* section)
{
    m_data.section = section;
}

Section* Task::getSection()
{
    return m_data.section;
}

void Task::setData(const Task::Data& data)
{
    m_data = data;
}

Task::Data Task::getData() const
{
    return m_data;
}

void Task::addTest(std::unique_ptr<Test> test)
{
    test->setTask(this);
    m_tests.push_back(std::move(test));
}

void Task::removeTest(const Test* test)
{
    for (auto it = m_tests.begin(); it != m_tests.end(); ++it) {
        if (it->get() == test) {
            m_tests.erase(it);
            return;
        }
    }
}

Test* Task::getTest(size_t n) const
{
    if (n < m_tests.size()) {
        return m_tests[n].get();
    }
    else {
        return nullptr;
    }
}

int Task::getTestIndex(const Test* test) const
{
    for (size_t i = 0; i < m_tests.size(); ++i) {
        if (m_tests[i].get() == test) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t Task::getTestCount() const
{
    return m_tests.size();
}

void Task::dbCreate(const Task::Data& data, std::function<void (std::unique_ptr<Task>)> callback)
{
    if (data.section == nullptr) {
        return;
    }
    
    std::vector<QString> arguments = {
        data.name,
        data.text,
        data.inputData,
        data.outputData,
        data.source,
        QString::number(data.score),
        QString::number(data.section->getId())
    };
    
    QString query = Query::create("INSERT INTO task_c (task_c_name, task_c_text, task_c_input, "
                                  "task_c_output, task_c_source, task_c_score, section_id) "
                                  "VALUES ('@@','@@','@@','@@','@@',@@,@@) RETURNING rowid", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "task_add", {
        {"sql_operator", query}
    }), [data, callback](const Response& response)
    {        
        std::unique_ptr<Task> task = std::make_unique<Task>();
        task->setData(data);
        
        callback(std::move(task));
    });
}

void Task::dbUpdate(Task* task, const Task::Data& data, std::function<void ()> callback)
{
    if (task == nullptr || data.section == nullptr) {
        return;
    }
    
    DeletionMark deletionMark = task->getDeletionMark();
    
    std::vector<QString> arguments = {
        data.name,
        data.text,
        data.inputData,
        data.outputData,
        data.source,
        QString::number(data.score),
        QString::number(data.section->getId()),
        QString::number(task->getId())
    };
    
    QString query = Query::create("UPDATE task_c SET task_c_name='@@', task_c_text='@@', "
                                  "task_c_input='@@', task_c_output='@@', task_c_source='@@', "
                                  "task_c_score=@@, section_id=@@ WHERE rowid=@@", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "task_edit", {
        {"sql_operator", query}
    }), [task, deletionMark, data, callback](const Response& response)
    {
        if (*deletionMark == true) {
            return;
        }
        
        task->setData(data);
        
        callback();
    });
}

void Task::dbDelete(Task* task, std::function<void ()> callback)
{
    if (task == nullptr) {
        return;
    }
    
    DeletionMark deletionMark = task->getDeletionMark();
    
    std::vector<QString> arguments = {
        QString::number(task->getId())
    };
    QString query = Query::create("DELETE FROM task_c WHERE rowid=@@", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "task_delete", {
        {"sql_operator", query}
    }), [deletionMark, callback](const Response& response)
    {
        if (*deletionMark == true) {
            return;
        }
        
        callback();
    });
}


Task::Data::Data() :
    id(0), score(0), section(nullptr)
{
}

Task::Data::Data(const QString& name, Section* section) :
    id(0), name(name), section(section)
{
}
