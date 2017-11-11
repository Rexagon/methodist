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

void Section::addSubsection(std::unique_ptr<Section> subsection)
{
    subsection->setCourse(m_course);
    addChild(subsection.get());
    m_subsections.push_back(std::move(subsection));
}

void Section::removeSubsection(const Section* subsection)
{
    for (auto it = m_subsections.begin(); it != m_subsections.end(); ++it) {
        if (it->get() == subsection) {
            removeChild(subsection);
            m_subsections.erase(it);
            return;
        }
    }
}

Section* Section::getSubsection(size_t n) const
{
    if (n < m_subsections.size()) {
        return m_subsections[n].get();
    }
    
    return nullptr;
}

int Section::getSubsectionIndex(const Section* subsection)
{
    for (size_t i = 0; i < m_subsections.size(); ++i) {
        if (m_subsections[i].get() == subsection) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t Section::getSubsectionCount() const
{
    return m_subsections.size();
}

void Section::addTask(std::unique_ptr<Task> task)
{
    task->setSection(this);
    addChild(task.get());
    m_tasks.push_back(std::move(task));
}

void Section::removeTask(const Task* task)
{
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        if (it->get() == task) {
            removeChild(task);
            m_tasks.erase(it);
            return;
        }
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

size_t Section::getTaskCount() const
{
    return m_tasks.size();
}
