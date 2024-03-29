#include "CoursesListModel.h"

CoursesListModel::CoursesListModel(QObject* parent) :
    QAbstractListModel(parent)
{
}

CoursesListModel::~CoursesListModel()
{
    m_courses.clear();
}

void CoursesListModel::update()
{
    std::stable_sort(m_courses.begin(), m_courses.end(), [](const std::unique_ptr<Course>& a, const std::unique_ptr<Course>& b) {
        return a->getName() < b->getName(); 
    });
    emit layoutChanged();
}

int CoursesListModel::rowCount(const QModelIndex& parent) const
{
    return m_courses.size();
}

QVariant CoursesListModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole && index.row() < m_courses.size()) {
        Course* course = m_courses[index.row()].get();
        
        return course->getName();
    }
    
    return QVariant();
}

void CoursesListModel::addCourse(std::unique_ptr<Course> course)
{
    m_courses.push_back(std::move(course));
    update();
}

void CoursesListModel::removeCourse(const Course* course)
{
    for (auto it = m_courses.begin(); it != m_courses.end(); ++it) {
        if (it->get() == course) {
            m_courses.erase(it);
            update();
            return;
        }
    }
}

void CoursesListModel::removeCourse(size_t n)
{
    if (n < m_courses.size()) {
        m_courses.erase(m_courses.begin() + n);
        update();
    }
}

Course* CoursesListModel::getCourse(size_t n) const
{
    if (n < m_courses.size()) {
        return m_courses[n].get();
    }
    
    return nullptr;
}

int CoursesListModel::getCourseIndex(const Course* course)
{
    for (size_t i = 0; i < m_courses.size(); ++i) {
        if (m_courses[i].get() == course) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t CoursesListModel::getCourseCount() const
{
    return m_courses.size();
}
