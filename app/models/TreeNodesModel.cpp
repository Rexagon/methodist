#include "TreeNodesModel.h"

#include <QIcon>
#include <QFont>

#include <iostream>

TreeNodesModel::TreeNodesModel(QObject* parent) :
    QAbstractItemModel(parent), m_course(nullptr)
{    
}

TreeNodesModel::~TreeNodesModel()
{
}

int TreeNodesModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        const TreeNode* parentNode = static_cast<const TreeNode*>(parent.internalPointer());
        return parentNode->getChildCount();
    }
    else {
        return 1;
    }
}

int TreeNodesModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QModelIndex TreeNodesModel::index(int row, int column, const QModelIndex& parent) const
{    
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    
    if (parent.isValid()) {
        TreeNode* parentNode = static_cast<TreeNode*>(parent.internalPointer());
        return createIndex(row, column, parentNode->getChild(row));
    }
    else {        
        return createIndex(row, column, m_course);
    }
}

QModelIndex TreeNodesModel::parent(const QModelIndex& child) const
{    
    if (!child.isValid()) {
        return QModelIndex();
    }

    TreeNode* childNode = static_cast<TreeNode*>(child.internalPointer());
    TreeNode* parentNode = childNode->getParent();
    if (parentNode != nullptr) {
        return createIndex(childNode->getRelativeNumber(), 0, parentNode);
    }
    else {
        return QModelIndex();
    }
}

QVariant TreeNodesModel::data(const QModelIndex& index, int role) const
{    
    if (!index.isValid()) {
        return QVariant();
    }
    
    const TreeNode* item = static_cast<TreeNode*>(index.internalPointer()); 
    
    if (item == nullptr || !index.isValid()) {
        return QVariant();
    }
    
    if (role == Qt::DisplayRole) {
        switch (item->getType()) {
        case TreeNode::Type::COURSE:
            return reinterpret_cast<const Course*>(item)->getName();
            
        case TreeNode::Type::SECTION:
            return reinterpret_cast<const Section*>(item)->getName();
    
        case TreeNode::Type::TASK:
            return reinterpret_cast<const Task*>(item)->getName();
        }
    }
    else if (role == Qt::DecorationRole) {
        switch (item->getType()) {
        case TreeNode::Type::COURSE:
            return QIcon(":/images/course.png");
            
        case TreeNode::Type::SECTION:
            return QIcon(":/images/section.png");
    
        case TreeNode::Type::TASK:
            return QIcon(":/images/task.png");
        }
    }
    else if (role == Qt::FontRole) {
        if (item->getType() == TreeNode::Type::COURSE) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }
    
    return QVariant();
}

Qt::ItemFlags TreeNodesModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }
    
    const TreeNode* item = static_cast<TreeNode*>(index.internalPointer()); 
    if (item->getParent() == nullptr) {
        return Qt::ItemIsEnabled;
    }
    else {
        return QAbstractItemModel::flags(index);
    }
}

void TreeNodesModel::setCourse(Course* course)
{
    m_course = course;
}

Course* TreeNodesModel::getCourse() const
{
    return m_course;
}

