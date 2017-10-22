#include "CoursesModel.h"

CoursesModel::CoursesModel(QObject* parent) :
    QAbstractListModel(parent)
{
}

CoursesModel::~CoursesModel()
{
    m_courses.clear();
}

int CoursesModel::rowCount(const QModelIndex& parent) const
{
    return m_courses.size();
}

QVariant CoursesModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole && index.row() < m_courses.size()) {
        Course* course = m_courses[index.row()].get();
        
        return course->getName();
    }
    
    return QVariant();
}

void CoursesModel::addCourse(std::unique_ptr<Course> course)
{
    m_courses.push_back(std::move(course));
}

void CoursesModel::removeCourse(const Course* course)
{
    for (auto it = m_courses.begin(); it != m_courses.end(); ++it) {
        if (it->get() == course) {
            m_courses.erase(it);
            return;
        }
    }
}

void CoursesModel::removeCourse(size_t n)
{
    if (n < m_courses.size()) {
        m_courses.erase(m_courses.begin() + n);
    }
}

Course*CoursesModel::getCourse(size_t n) const
{
    if (n < m_courses.size()) {
        return m_courses[n].get();
    }
    
    return nullptr;
}

int CoursesModel::getCourseIndex(const Course* course)
{
    for (size_t i = 0; i < m_courses.size(); ++i) {
        if (m_courses[i].get() == course) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}
