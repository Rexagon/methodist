#ifndef SECTION_H
#define SECTION_H

#include <vector>

#include <QString>

#include "Task.h"

class Course;

class Section
{
public:
    Section();
    ~Section();
    
    void setId(unsigned int id);
    unsigned int getId() const;
    
    void setName(const QString& name);
    QString getName() const;
    
    void setRelativeNumber(unsigned int number);
    unsigned int getRelativeNumber() const;
    
    void setCourse(Course* course);
    Course* getCourse() const;
    
    void setParent(Section* parent);
    Section* getParent() const;
    
    void addSubSection(Section* section);
    void removeSubsection(Section* section);
    void removeSubsection(size_t n);
    Section* getSubSection(size_t n) const;
    int getSubSectionIndex(Section* section) const;
    
    size_t getSubSectionCount() const;
    std::vector<Section*> getSubSections() const;
    
private:
    unsigned int m_id;
    
    QString m_name;
    
    unsigned int m_relativeNumber;
    
    Course* m_course;
    Section* m_parent;
    std::vector<Section*> m_subSections;
};

#endif // SECTION_H
