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
    Task();
    ~Task();
    
    void setId(unsigned int id);
    unsigned int getId() const;
    
    void setName(const QString& name);
    QString getName() const;
    
    void setText(const QString& text);
    QString getText() const;
    
    void setInputData(const QString& text);
    QString getInputData() const;
    
    void setOutputData(const QString& text);
    QString getOutputData() const;
    
    void setSource(const QString& code);
    QString getSource() const;
    
    void setScore(unsigned int score);
    unsigned int getScore() const;
    
    void setCourse(Course* course);
    Course* getCourse() const;
    
    void setSection(Section* section);
    Section* getSection();
    
    
    void addTest(std::unique_ptr<Test> test);
    void removeTest(Test* test);
    Test* getTest(size_t n) const;
    int getTestIndex(Test* test) const;
    size_t getTestCount() const;
    
private:
    unsigned int m_id;
    
    QString m_name;
    QString m_text;
    QString m_inputData;
    QString m_outputData;
    QString m_code;
    
    unsigned int m_score;
    
    Course* m_course;
    Section* m_section;
    
    std::vector<std::unique_ptr<Test>> m_tests;
};

#endif // TASK_H
