#include "Section.h"

#include "Course.h"

Section::Section() :
    m_relativeNumber(0), m_course(nullptr), m_parent(nullptr)
{
}

Section::~Section()
{
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

void Section::setRelativeNumber(unsigned int number)
{
    m_relativeNumber = number;
}

unsigned int Section::getRelativeNumber() const
{
    return m_relativeNumber;
}

void Section::setCourse(Course* course)
{
    m_course = course;
}

Course* Section::getCourse() const
{
    return m_course;
}

void Section::setParent(Section* parent)
{
    m_parent = parent;
}

Section* Section::getParent() const
{
    return m_parent;
}

void Section::removeSubsection(Section* section)
{
    for (auto it = m_subSections.begin(); it != m_subSections.end(); ++it) {
        if (*it == section) {
            m_subSections.erase(it);
            return;
        }
    }
}

void Section::removeSubsection(size_t n)
{
    if (n < m_subSections.size()) {
        m_subSections.erase(m_subSections.begin() + n);
    }
}

Section* Section::getSubSection(size_t n) const
{
    if (n < m_subSections.size()) {
        return m_subSections[n];
    }
    else {
        return nullptr;
    }
}

int Section::getSubSectionIndex(Section* section) const
{
    for (size_t i = 0; i < m_subSections.size(); ++i) {
        if (m_subSections[i] == section) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t Section::getSubSectionCount() const
{
    return m_subSections.size();
}

std::vector<Section*> Section::getSubSections() const
{
    return m_subSections;
}

