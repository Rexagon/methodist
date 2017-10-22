#ifndef SECTION_H
#define SECTION_H

#include <QString>

#include "TreeNode.h"
#include "Task.h"

class Course;

class Section : public TreeNode
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
    
private:
    unsigned int m_id;
    
    QString m_name;
    
    Course* m_course;
};

#endif // SECTION_H
