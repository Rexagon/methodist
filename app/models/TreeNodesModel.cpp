#include "TreeNodesModel.h"

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
    return getItem(parent)->getChildCount();
}

int TreeNodesModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QModelIndex TreeNodesModel::index(int row, int column, const QModelIndex& parentIndex) const
{    
    if (parentIndex.isValid() && parentIndex.column() != 0) {
        return QModelIndex();
    }

    TreeNode* item = getItem(parentIndex)->getChild(row);
    if (item == nullptr) {
        return QModelIndex();
    }
    else {
        return createIndex(row, column, item);
    }
}

QModelIndex TreeNodesModel::parent(const QModelIndex& childIndex) const
{    
    if (!childIndex.isValid()) {
        return QModelIndex();
    }

    TreeNode* item = getItem(childIndex)->getParent();

    if (item == m_course) {
        return QModelIndex();
    }

    return createIndex(item->getRelativeNumber(), 0, item);
}

QVariant TreeNodesModel::data(const QModelIndex& index, int role) const
{    
    TreeNode* item = getItem(index);
    
    if (item == nullptr || !index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole) || index.column() != 0) {
        return QVariant();
    }
    
    switch (item->getType()) {
    case TreeNode::Type::COURSE:
        return reinterpret_cast<Course*>(item)->getName();
        
    case TreeNode::Type::SECTION:
        return reinterpret_cast<Section*>(item)->getName();

    case TreeNode::Type::TASK:
        return reinterpret_cast<Task*>(item)->getName();        
        
    default:
        return QVariant();
    }
}

QVariant TreeNodesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_course->getName();
    }
    
    return QVariant();
}

void TreeNodesModel::setCourse(Course* course)
{
    m_course = course;
}

Course* TreeNodesModel::getCourse() const
{
    return m_course;
}

TreeNode* TreeNodesModel::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        TreeNode* item = static_cast<TreeNode*>(index.internalPointer());
        if (item != nullptr) {
            return item;
        }
    }
    
    return m_course;
}

