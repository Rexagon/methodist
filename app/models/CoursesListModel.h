#ifndef COURSESLISTMODEL_H
#define COURSESLISTMODEL_H

#include <QAbstractListModel>

#include "../objects/Course.h"

class CoursesListModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    CoursesListModel(QObject* parent = nullptr);
    ~CoursesListModel();
    
    void update();
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
    void addCourse(std::unique_ptr<Course> course);
    void removeCourse(const Course* course);
    void removeCourse(size_t n);
    Course* getCourse(size_t n) const;
    int getCourseIndex(const Course* course);
    size_t getCourseCount() const;
    
private:
    std::vector<std::unique_ptr<Course>> m_courses;
};

#endif // COURSESLISTMODEL
