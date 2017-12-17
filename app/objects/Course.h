#ifndef COURSE_H
#define COURSE_H

#include <functional>
#include <memory>
#include <vector>

#include "../stuff/NetworkManager.h"
#include "CourseNode.h"
#include "Section.h"

class Course : public CourseNode
{
public:
    struct Data
    {
        Data();
        Data(const Response::Row& row);
        Data(const QString &name, size_t lectureHourCount = 0, size_t practiceHourCount = 0, size_t laboratoryHourCount = 0);
        Data(size_t id, const QString &name, size_t lectureHourCount = 0, size_t practiceHourCount = 0, size_t laboratoryHourCount = 0);
        
        size_t id;
        
        QString name;
        
        size_t lectureHourCount;
        size_t practiceHourCount;
        size_t laboratoryHourCount;
    };
    
    Course();
    ~Course();

    void setId(size_t id);
    size_t getId() const;
    
    void setName(const QString& name);
    QString getName() const;
    
    void setLectureHourCount(size_t n);
    size_t getLectureHourCount() const;
    
    void setPracticeHourCount(size_t n);
    size_t getPracticeHourCount() const;
    
    void setLaboratoryHourCount(size_t n);
    size_t getLaboratoryHourCount() const;
    
    void setData(const Data& data);
    Data getData() const;
    
    
    void addSection(std::unique_ptr<Section> section);
    void removeSection(const Section* section);
    Section* getSection(size_t n) const;
    int getSectionIndex(const Section* section);
    size_t getSectionCount() const;
    
    static void dbCreate(const Data& data, std::function<void(std::unique_ptr<Course>)> callback);
    static void dbUpdate(Course* course,const Data& data, std::function<void()> callback);
    static void dbDelete(Course* course, std::function<void()> callback);
    
private:
    Data m_data;
    
    std::vector<std::unique_ptr<Section>> m_sections;
};

#endif // COURSE_H
