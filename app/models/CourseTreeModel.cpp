#include "CourseTreeModel.h"

#include <QIcon>
#include <QFont>

CourseTreeModel::CourseTreeModel(QObject* parent) :
    QAbstractItemModel(parent), m_course(nullptr)
{    
}

CourseTreeModel::~CourseTreeModel()
{
}

void CourseTreeModel::update()
{
    emit layoutChanged();
}

int CourseTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        const CourseNode* parentNode = static_cast<const CourseNode*>(parent.internalPointer());
        return parentNode->getChildCount();
    }
    else {
        return 1;
    }
}

int CourseTreeModel::columnCount(const QModelIndex&) const
{
    return 1;
}

QModelIndex CourseTreeModel::index(int row, int column, const QModelIndex& parent) const
{    
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    
    if (parent.isValid()) {
        CourseNode* parentNode = static_cast<CourseNode*>(parent.internalPointer());
        return createIndex(row, column, parentNode->getChild(row));
    }
    else {        
        return createIndex(row, column, m_course);
    }
}

QModelIndex CourseTreeModel::parent(const QModelIndex& child) const
{    
    if (!child.isValid()) {
        return QModelIndex();
    }

    CourseNode* childNode = static_cast<CourseNode*>(child.internalPointer());
    CourseNode* parentNode = childNode->getParent();
    if (parentNode != nullptr) {
        return createIndex(childNode->getRelativeNumber(), 0, parentNode);
    }
    else {
        return QModelIndex();
    }
}

QModelIndex CourseTreeModel::getIndex(CourseNode* node)
{
    if (node == nullptr) {
        return QModelIndex();
    }
    
    return createIndex(node->getRelativeNumber(), 0, node);
}

QVariant CourseTreeModel::data(const QModelIndex& index, int role) const
{    
    if (!index.isValid()) {
        return QVariant();
    }
    
    const CourseNode* item = static_cast<CourseNode*>(index.internalPointer()); 
    
    if (item == nullptr || !index.isValid()) {
        return QVariant();
    }
    
    if (role == Qt::DisplayRole) {
        switch (item->getType()) {
        case CourseNode::Type::COURSE:
            return reinterpret_cast<const Course*>(item)->getName();
            
        case CourseNode::Type::SECTION:
            return reinterpret_cast<const Section*>(item)->getName();
    
        case CourseNode::Type::TASK:
            return reinterpret_cast<const Task*>(item)->getName();
        }
    }
    else if (role == Qt::DecorationRole) {
        switch (item->getType()) {
        case CourseNode::Type::COURSE:
            return QIcon(":/images/course.png");
            
        case CourseNode::Type::SECTION:
            return QIcon(":/images/section.png");
    
        case CourseNode::Type::TASK:
            return QIcon(":/images/task.png");
        }
    }
    else if (role == Qt::FontRole) {
        if (item->getType() == CourseNode::Type::COURSE) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }
    
    return QVariant();
}

void CourseTreeModel::setCourse(Course* course)
{
    m_course = course;
}

Course* CourseTreeModel::getCourse() const
{
    return m_course;
}

