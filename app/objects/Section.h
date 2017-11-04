#ifndef SECTION_H
#define SECTION_H

#include <memory>
#include <vector>

#include <QString>

#include "CourseNode.h"
#include "Task.h"

class Course;

class Section : public CourseNode
{
public:
    Section();
    ~Section();
    
    void setId(unsigned int id);
    unsigned int getId() const;
    
    void setName(const QString& name);
    QString getName() const;
    
    void setCourse(Course* course);
    Course* getCourse() const;
    
    
    void addTask(std::unique_ptr<Task> task);
    void removeTask(const Task* task);
    void removeTask(size_t n);
    Task* getTask(size_t n) const;
    int getTaskIndex(const Task* task);
    
private:
    unsigned int m_id;
    
    QString m_name;
    
    Course* m_course;
    
    std::vector<std::unique_ptr<Task>> m_tasks;
};

#endif // SECTION_H
