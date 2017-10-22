#include "TreeNode.h"

#include <iostream>

TreeNode::TreeNode(TreeNode::Type type) :
    m_type(type), m_parent(nullptr)
{
}

TreeNode::~TreeNode()
{
}

TreeNode::Type TreeNode::getType() const
{
    return m_type;
}

int TreeNode::getRelativeNumber() const
{
    if (m_parent == nullptr) {
        return 0;
    }
    else {
        return m_parent->getChildIndex(this);
    }
}

void TreeNode::setParent(TreeNode* parent)
{
    m_parent = parent;
}

TreeNode* TreeNode::getParent() const
{
    return m_parent;
}

void TreeNode::addChild(TreeNode* child)
{
    m_children.push_back(child);
    child->setParent(this);
}

void TreeNode::removeChild(const TreeNode* child)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        if (*it == child) {
            m_children.erase(it);
            return;
        }
    }
}

void TreeNode::removeChild(size_t n)
{
    if (n < m_children.size()) {
        m_children.erase(m_children.begin() + n);
    }
}

TreeNode* TreeNode::getChild(size_t n) const
{
    if (n < m_children.size()) {
        return m_children[n];
    }
    else {
        return nullptr;
    }
}

int TreeNode::getChildIndex(const TreeNode* child) const
{
    for (size_t i = 0; i < m_children.size(); ++i) {
        if (m_children[i] == child) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t TreeNode::getChildCount() const
{
    return m_children.size();
}

std::vector<TreeNode*> TreeNode::getChildren() const
{
    return m_children;
}
