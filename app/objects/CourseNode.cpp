#include "CourseNode.h"

#include <iostream>

CourseNode::CourseNode(CourseNode::Type type) :
    m_type(type), m_parent(nullptr)
{
}

CourseNode::~CourseNode()
{
}

CourseNode::Type CourseNode::getType() const
{
    return m_type;
}

int CourseNode::getRelativeNumber() const
{
    if (m_parent == nullptr) {
        return 0;
    }
    else {
        return m_parent->getChildIndex(this);
    }
}

void CourseNode::setParent(CourseNode* parent)
{
    m_parent = parent;
}

CourseNode* CourseNode::getParent() const
{
    return m_parent;
}

void CourseNode::addChild(CourseNode* child)
{
    m_children.push_back(child);
    child->setParent(this);
}

void CourseNode::removeChild(const CourseNode* child)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        if (*it == child) {
            m_children.erase(it);
            return;
        }
    }
}

CourseNode* CourseNode::getChild(size_t n) const
{
    if (n < m_children.size()) {
        return m_children[n];
    }
    else {
        return nullptr;
    }
}

int CourseNode::getChildIndex(const CourseNode* child) const
{
    for (size_t i = 0; i < m_children.size(); ++i) {
        if (m_children[i] == child) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t CourseNode::getChildCount() const
{
    return m_children.size();
}

std::vector<CourseNode*> CourseNode::getChildren() const
{
    return m_children;
}
