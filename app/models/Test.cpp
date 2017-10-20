#include "Test.h"

#include "Task.h"

Test::Test() :
    m_id(0), m_isRequired(true), m_relativeNumber(0), m_task(nullptr)
{   
}

Test::~Test()
{
}

void Test::setId(unsigned int id)
{
    m_id = id;
}

unsigned int Test::getId() const
{
    return m_id;
}

void Test::setInputData(const QString& data)
{
    m_inputData = data;
}

QString Test::getInputData() const
{
    return m_inputData;
}

void Test::setOutputData(const QString& data)
{
    m_outputData = data;
}

QString Test::getOutputData() const
{
    return m_outputData;
}

void Test::setRequired(bool required)
{
    m_isRequired = required;
}

bool Test::isRequired() const
{
    return m_isRequired;
}

void Test::setRelativeNumber(unsigned int number)
{
    m_relativeNumber = number;
}

unsigned int Test::getRelativeNumber() const
{
    return m_relativeNumber;
}

void Test::setTask(Task* task)
{
    m_task = task;
}

Task* Test::getTask() const
{
    return m_task;
}
