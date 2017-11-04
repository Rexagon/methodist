#ifndef COURSETREEMODEL_H
#define COURSETREEMODEL_H

#include <QAbstractItemModel>

#include "../objects/Course.h"

class CourseTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    CourseTreeModel(QObject *parent = nullptr);
    ~CourseTreeModel();
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    
    QVariant data(const QModelIndex& index, int role) const override;
    
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    void setCourse(Course* course);
    Course* getCourse() const;    
    
private:
    
    Course* m_course;
};

#endif // COURSETREEMODEL_H
