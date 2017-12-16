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
    struct Data
    {
        Data();
        Data(const QString& name, Course* course = nullptr, Section* parentSection = nullptr);
        Data(size_t id, const QString &name, Course* course = nullptr, Section* parentSection = nullptr);
        
        size_t id;
        
        QString name;
        
        Course* course;
        Section* parentSection;
    };
    
    Section();
    ~Section();
    
    void setId(size_t id);
    size_t getId() const;
    
    void setName(const QString& name);
    QString getName() const;
    
    void setCourse(Course* course);
    Course* getCourse() const;
    
    void setParentSection(Section* parentSection);
    Section* getParentSection();
    
    void setData(const Data& data);
    Data getData() const;
    
    
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
    
    static void dbCreate(const Data& data, std::function<void(std::unique_ptr<Section>)> callback);
    static void dbUpdate(Section* section, const Data& data, std::function<void()> callback);
    static void dbDelete(Section* section, std::function<void()> callback);
    
private:
    Data m_data;
    
    std::vector<std::unique_ptr<Section>> m_subsections;
    std::vector<std::unique_ptr<Task>> m_tasks;
};

#endif // SECTION_H
