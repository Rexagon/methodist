#include "Section.h"

#include "Course.h"

Section::Section() :
    CourseNode(CourseNode::Type::SECTION),
    m_course(nullptr)
{
}

Section::~Section()
{
    m_tasks.clear();
}

void Section::setId(unsigned int id)
{
    m_id = id;
}

unsigned int Section::getId() const
{
    return m_id;
}

void Section::setName(const QString& name)
{
    m_name = name;
}

QString Section::getName() const
{
    return m_name;
}

void Section::setCourse(Course* course)
{
    m_course = course;
}

Course* Section::getCourse() const
{
    return m_course;
}

void Section::addTask(std::unique_ptr<Task> task)
{
    task->setSection(this);
    m_tasks.push_back(std::move(task));
}

void Section::removeTask(const Task* task)
{
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        if (it->get() == task) {
            m_tasks.erase(it);
            return;
        }
    }
}

void Section::removeTask(size_t n)
{
    if (n < m_tasks.size()) {
        m_tasks.erase(m_tasks.begin() + n);
    }
}

Task* Section::getTask(size_t n) const
{
    if (n < m_tasks.size()) {
        return m_tasks[n].get();
    }
    
    return nullptr;
}

int Section::getTaskIndex(const Task* task)
{
    for (size_t i = 0; i < m_tasks.size(); ++i) {
        if (m_tasks[i].get() == task) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}
