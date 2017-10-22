#ifndef COURSESMODEL_H
#define COURSESMODEL_H

#include <QAbstractListModel>

#include "Course.h"

class CoursesModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    CoursesModel(QObject* parent = nullptr);
    ~CoursesModel();
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
    void addCourse(std::unique_ptr<Course> course);
    void removeCourse(const Course* course);
    void removeCourse(size_t n);
    Course* getCourse(size_t n) const;
    int getCourseIndex(const Course* course);
    
private:
    std::vector<std::unique_ptr<Course>> m_courses;
};

#endif // COURSESMODEL_H
