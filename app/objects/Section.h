#ifndef SECTION_H
#define SECTION_H

#include <memory>
#include <vector>

#include <QString>

#include "CourseNode.h"
#include "Task.h"

#include "../stuff/Deletable.h"

class Course;

class Section : public CourseNode, public Deletable
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
    
    
    void addSubsection(std::unique_ptr<Section> subsection);
    void removeSubsection(const Section* subsection);
    Section* getSubsection(size_t n) const;
    int getSubsectionIndex(const Section* subsection);
    size_t getSubsectionCount() const;
    
    void addTask(std::unique_ptr<Task> task);
    void removeTask(const Task* task);
    Task* getTask(size_t n) const;
    int getTaskIndex(const Task* task);
    size_t getTaskCount() const;
    
private:
    unsigned int m_id;
    
    QString m_name;
    
    Course* m_course;
    
    std::vector<std::unique_ptr<Section>> m_subsections;
    std::vector<std::unique_ptr<Task>> m_tasks;
};

#endif // SECTION_H
