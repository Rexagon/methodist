#ifndef TASK_H
#define TASK_H

#include <vector>
#include <memory>

#include <QString>

#include "CourseNode.h"
#include "Test.h"

class Course;
class Section;

class Task : public CourseNode
{
public:
    struct Data
    {
        Data();
        Data(const QString& name, Section* section);
        
        size_t id;
        
        QString name;
        QString text;
        QString inputData;
        QString outputData;
        QString source;
        
        size_t score;
        
        Section* section;
    };
    
    Task();
    ~Task();
    
    void setId(size_t id);
    size_t getId() const;
    
    void setName(const QString& name);
    QString getName() const;
    
    void setText(const QString& text);
    QString getText() const;
    
    void setInputData(const QString& text);
    QString getInputData() const;
    
    void setOutputData(const QString& text);
    QString getOutputData() const;
    
    void setSource(const QString& source);
    QString getSource() const;
    
    void setScore(size_t score);
    size_t getScore() const;
    
    void setSection(Section* section);
    Section* getSection();
    
    void setData(const Data& data);
    Data getData() const;    
    
    
    void addTest(std::unique_ptr<Test> test);
    void removeTest(const Test* test);
    Test* getTest(size_t n) const;
    int getTestIndex(const Test* test) const;
    size_t getTestCount() const;
    
    static void dbCreate(const Data& data, std::function<void(std::unique_ptr<Task>)> callback);
    static void dbUpdate(Task* task,const Data& data, std::function<void()> callback);
    static void dbDelete(Task* task, std::function<void()> callback);
    
private:
    Data m_data;
    
    std::vector<std::unique_ptr<Test>> m_tests;
};

#endif // TASK_H
