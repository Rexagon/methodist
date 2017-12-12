#ifndef TASK_H
#define TASK_H

#include <vector>
#include <memory>

#include <QString>

#include "CourseNode.h"
#include "Test.h"

#include "../stuff/Deletable.h"

class Course;
class Section;

class Task : public CourseNode, public Deletable
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
    
    void setSource(const QString& source);
    QString getSource() const;
    
    void setScore(size_t score);
    size_t getScore() const;
    
    void setSection(Section* section);
    Section* getSection();
    
    
    void addTest(std::unique_ptr<Test> test);
    void removeTest(const Test* test);
    Test* getTest(size_t n) const;
    int getTestIndex(const Test* test) const;
    size_t getTestCount() const;
    
private:
    unsigned int m_id;
    
    QString m_name;
    QString m_text;
    QString m_inputData;
    QString m_outputData;
    QString m_source;
    
    size_t m_score;
    
    Section* m_section;
    
    std::vector<std::unique_ptr<Test>> m_tests;
};

#endif // TASK_H
