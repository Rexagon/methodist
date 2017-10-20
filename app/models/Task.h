#ifndef TASK_H
#define TASK_H

#include <vector>
#include <memory>

#include <QString>
#include <QRegExp>

#include "Test.h"

class Course;
class Section;

class Task
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
    
    void setCode(const QString& code);
    QString getCode() const;
    
    void setScore(unsigned int score);
    unsigned int getScore() const;
    
    void setInputPattern(const QString& pattern);
    const QRegExp& getInputRegexp() const;
    
    void setOutputPattern(const QString& pattern);
    const QRegExp& getOutputRegexp() const;
    
    void addTest(std::unique_ptr<Test> test);
    void removeTest(Test* test);
    void removeTest(size_t n);
    Test* getTest(size_t n) const;
    int getTestIndex(Test* test) const;
    
    void setRelativeNumber(unsigned int number);
    unsigned int getRelativeNumber() const;
    
    void setCourse(Course* course);
    Course* getCourse() const;
    
    void setParent(Section* section);
    Section* getSection() const;
    
private:
    unsigned int m_id;
    
    QString m_name;
    QString m_text;
    QString m_code;
    
    unsigned int m_score;
    
    QRegExp m_inputPattern;
    QRegExp m_outputPattern;
    
    std::vector<std::unique_ptr<Test>> m_tests;
    
    unsigned int m_relativeNumber;
    
    Course* m_course;
    Section* m_parent;
};

#endif // TASK_H
