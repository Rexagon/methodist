#ifndef COURSE_H
#define COURSE_H

#include <memory>
#include <vector>

#include "Section.h"

class Course
{
public:
    Course();
    ~Course();

    void setId(unsigned int id);
    unsigned int getId() const;
    
    void setName(const QString& name);
    QString getName() const;
    
    void setLectureHourCount(unsigned int n);
    unsigned int getLectureHourCount() const;
    
    void setPracticeHourCount(unsigned int n);
    unsigned int getPracticeHourCount() const;
    
    void addSection(std::unique_ptr<Section> section);
    void removeSection(Section* section);
    void removeSection(size_t n);
    Section* getSection(size_t n) const;
    int getSectionIndex(Section* section) const;
    
    size_t getSectionCount() const;
    std::vector<Section*> getSections() const;
    
private:
    unsigned int m_id;
    
    QString m_name;
    
    unsigned int m_lectureHourCount;
    unsigned int m_practiceHourCount;

    std::vector<std::unique_ptr<Section>> m_sections;
};

#endif // COURSE_H
