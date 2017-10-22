#ifndef COURSE_H
#define COURSE_H

#include <memory>
#include <vector>

#include "TreeNode.h"
#include "Section.h"

class Course : public TreeNode
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
    
    void setCreator(const QString& creator);
    QString getCreator() const;
    
    
    void addSection(std::unique_ptr<Section> section);
    void removeSection(const Section* section);
    void removeSection(size_t n);
    Section* getSection(size_t n) const;
    int getSectionIndex(const Section* section);
    
private:
    unsigned int m_id;
    
    QString m_name;
    
    unsigned int m_lectureHourCount;
    unsigned int m_practiceHourCount;
    
    QString m_creator;
    
    std::vector<std::unique_ptr<Section>> m_sections;
};

#endif // COURSE_H
