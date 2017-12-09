#include "Task.h"

#include "Course.h"
#include "Section.h"

Task::Task() :
    CourseNode(CourseNode::Type::TASK),
    m_id(0), m_score(0), m_course(nullptr), m_section(nullptr)
{
}

Task::~Task()
{
}

void Task::setId(unsigned int id)
{
    m_id = id;
}

unsigned int Task::getId() const
{
    return m_id;
}

void Task::setName(const QString& name)
{
    m_name = name;
}

QString Task::getName() const
{
    return m_name;
}

void Task::setText(const QString& text)
{
    m_text = text;
}

QString Task::getText() const
{
    return m_text;
}

void Task::setInputData(const QString& text)
{
    m_inputData = text;
}

QString Task::getInputData() const
{
    return m_inputData;
}

void Task::setOutputData(const QString& text)
{
    m_outputData = text;
}

QString Task::getOutputData() const
{
    return m_outputData;
}

void Task::setSource(const QString& source)
{
    m_source = source;
}

QString Task::getSource() const
{
    return m_source;
}

void Task::setScore(size_t score)
{
    m_score = score;
}

size_t Task::getScore() const
{
    return m_score;
}

void Task::addTest(std::unique_ptr<Test> test)
{
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

void Task::setCourse(Course* course)
{
    m_course = course;
}

Course* Task::getCourse() const
{
    return m_course;
}

void Task::setSection(Section* section)
{
    m_section = section;
}

Section* Task::getSection()
{
    return m_section;
}
