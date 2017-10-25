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
    
    void setRequired(bool required);
    bool isRequired() const;

    void setScore(unsigned int score);
    unsigned int getScore() const;
    
    void setTask(Task* task);
    Task* getTask() const;
    
private:
    unsigned int m_id;
    
    QString m_inputData;
    QString m_outputData;

    bool m_isRequired;
    unsigned int m_score;
    
    Task* m_task;
};

#endif // TEST_H
