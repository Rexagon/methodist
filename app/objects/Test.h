#ifndef TEST_H
#define TEST_H

#include <functional>

#include <QString>

#include "CourseNode.h"

class Task;

class Test : public CourseNode
{
public:
    struct Data
    {
        Data();
        Data(const QString& inputData, const QString& outputData, 
             size_t score, bool isRequired, bool isSample, Task* task);
        
        size_t id;
        
        QString inputData;
        QString outputData;
    
        size_t score;
        bool isRequired;
        bool isSample;
        
        Task* task;
    };
    
    Test();
    ~Test();
    
    void setId(size_t id);
    size_t getId() const;
    
    void setInputData(const QString& data);
    QString getInputData() const;
    
    void setOutputData(const QString& data);
    QString getOutputData() const;
    
    void setScore(size_t score);
    size_t getScore() const;
    
    void setRequired(bool required);
    bool isRequired() const;
    
    void setSample(bool sample);
    bool isSample() const;
    
    void setTask(Task* task);
    Task* getTask() const;
    
    void setData(Data data);
    Data getData() const;
    
    static void dbCreate(const Data& data, std::function<void(std::unique_ptr<Test>)> callback);
    static void dbUpdate(Test* test,const Data& data, std::function<void()> callback);
    static void dbDelete(Test* test, std::function<void()> callback);
    
private:
    Data m_data;
};

#endif // TEST_H
