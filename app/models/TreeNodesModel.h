#ifndef TREENODESMODEL_H
#define TREENODESMODEL_H

#include <QAbstractItemModel>

#include "Course.h"

class TreeNodesModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    TreeNodesModel(QObject *parent = nullptr);
    ~TreeNodesModel();
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    
    void setCourse(Course* course);
    Course* getCourse() const;    
    
private:
    TreeNode* getItem(const QModelIndex& index) const;
    
    Course* m_course;
};

#endif // TREENODESMODEL_H
