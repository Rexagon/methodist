#include "Course.h"

Course::Course() :
    TreeNode(TreeNode::Type::COURSE),
    m_id(0), m_lectureHourCount(0), m_practiceHourCount(0)
{
}

Course::~Course()
{
    m_sections.clear();
}

void Course::setId(unsigned int id)
{
    m_id = id;
}

unsigned int Course::getId() const
{
    return m_id;
}

void Course::setName(const QString& name)
{
    m_name = name;
}

QString Course::getName() const
{
    return m_name;
}

void Course::setLectureHourCount(unsigned int n)
{
    m_lectureHourCount = n;
}

unsigned int Course::getLectureHourCount() const
{
    return m_lectureHourCount;
}

void Course::setPracticeHourCount(unsigned int n)
{
    m_practiceHourCount = n;
}

unsigned int Course::getPracticeHourCount() const
{
    return m_practiceHourCount;
}

void Course::setCreator(const QString& creator)
{
    m_creator = creator;
}

QString Course::getCreator() const
{
    return m_creator;
}

void Course::addSection(std::unique_ptr<Section> section)
{
    section->setCourse(this);
    m_sections.push_back(std::move(section));
}

void Course::removeSection(const Section* section)
{
    for (auto it = m_sections.begin(); it != m_sections.end(); ++it) {
        if (it->get() == section) {
            m_sections.erase(it);
            return;
        }
    }
}

void Course::removeSection(size_t n)
{
    if (n < m_sections.size()) {
        m_sections.erase(m_sections.begin() + n);
    }
}

Section* Course::getSection(size_t n) const
{
    if (n < m_sections.size()) {
        return m_sections[n].get();
    }
    
    return nullptr;
}

int Course::getSectionIndex(const Section* section)
{
    for (size_t i = 0; i < m_sections.size(); ++i) {
        if (m_sections[i].get() == section) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}
