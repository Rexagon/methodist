#ifndef COURSE_H
#define COURSE_H

#include <memory>
#include <vector>

#include "CourseNode.h"
#include "Section.h"

class Course : public CourseNode
{
public:
    Course();
    ~Course();

    void setId(unsigned int id);
    unsigned int getId() const;
    
    void setName(const QString& name);
    QString getName() const;
    
    void setLectureHourCount(size_t n);
    size_t getLectureHourCount() const;
    
    void setPracticeHourCount(size_t n);
    size_t getPracticeHourCount() const;
    
    void setLaboratoryHourCount(size_t n);
    size_t getLaboratoryHourCount() const;
    
    void setCreator(const QString& creator);
    QString getCreator() const;
    
    
    void addSection(std::unique_ptr<Section> section);
    void removeSection(const Section* section);
    Section* getSection(size_t n) const;
    int getSectionIndex(const Section* section);
    size_t getSectionCount() const;
    
private:
    unsigned int m_id;
    
    QString m_name;
    
    size_t m_lectureHourCount;
    size_t m_practiceHourCount;
    size_t m_laboratoryHourCount;
    
    QString m_creator;
    
    std::vector<std::unique_ptr<Section>> m_sections;
};

#endif // COURSE_H
