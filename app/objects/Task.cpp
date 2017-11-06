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

void Task::setSource(const QString& code)
{
    m_code = code;
}

QString Task::getSource() const
{
    return m_code;
}

void Task::setScore(unsigned int score)
{
    m_score = score;
}

unsigned int Task::getScore() const
{
    return m_score;
}

void Task::setInputPattern(const QString& pattern)
{
    m_inputPattern.setPattern(pattern);
}

const QRegExp& Task::getInputRegexp() const
{
    return m_inputPattern;
}

void Task::addTest(std::unique_ptr<Test> test)
{
    m_tests.push_back(std::move(test));
}

void Task::removeTest(Test* test)
{
    for (auto it = m_tests.begin(); it != m_tests.end(); ++it) {
        if (it->get() == test) {
            m_tests.erase(it);
            return;
        }
    }
}

void Task::removeTest(size_t n)
{
    if (n < m_tests.size()) {
        m_tests.erase(m_tests.begin() + n);
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

int Task::getTestIndex(Test* test) const
{
    for (size_t i = 0; i < m_tests.size(); ++i) {
        if (m_tests[i].get() == test) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
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
