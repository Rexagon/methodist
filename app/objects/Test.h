#ifndef TEST_H
#define TEST_H

#include <QString>

class Task;

class Test
{
public:
    Test();
    ~Test();
    
    void setId(unsigned int id);
    unsigned int getId() const;
    
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
    
private:
    unsigned int m_id;
    
    QString m_inputData;
    QString m_outputData;

    size_t m_score;
    bool m_isRequired;
    bool m_isSample;
    
    Task* m_task;
};

#endif // TEST_H
