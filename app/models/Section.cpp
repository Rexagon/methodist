#include "Section.h"

#include "Course.h"

Section::Section() :
    TreeNode(TreeNode::Type::SECTION),
    m_course(nullptr)
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

void Section::setCourse(Course* course)
{
    m_course = course;
}

Course* Section::getCourse() const
{
    return m_course;
}
