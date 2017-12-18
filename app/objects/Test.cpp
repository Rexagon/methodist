#include "Test.h"

#include "Task.h"

Test::Test() :
    CourseNode(CourseNode::Type::TEST)
{   
}

Test::~Test()
{
}

void Test::setId(size_t id)
{
    m_data.id = id;
}

size_t Test::getId() const
{
    return m_data.id;
}

void Test::setInputData(const QString& data)
{
    m_data.inputData = data;
}

QString Test::getInputData() const
{
    return m_data.inputData;
}

void Test::setOutputData(const QString& data)
{
    m_data.outputData = data;
}

QString Test::getOutputData() const
{
    return m_data.outputData;
}

void Test::setRequired(bool required)
{
    m_data.isRequired = required;
}

bool Test::isRequired() const
{
    return m_data.isRequired;
}

void Test::setSample(bool sample)
{
    m_data.isSample = sample;
}

bool Test::isSample() const
{
    return m_data.isSample;
}

void Test::setScore(size_t score)
{
    m_data.score = score;
}

size_t Test::getScore() const
{
    return m_data.score;
}

void Test::setTask(Task* task)
{
    m_data.task = task;
}

Task* Test::getTask() const
{
    return m_data.task;
}

void Test::setData(Test::Data data)
{
    m_data = data;
}

Test::Data Test::getData() const
{
    return m_data;
}

void Test::dbCreate(const Test::Data& data, std::function<void (std::unique_ptr<Test>)> callback)
{
    if (data.task == nullptr) {
        return;
    }
    
    std::vector<QString> arguments = {
        data.inputData,
        data.outputData,
        data.isRequired ? "TRUE" : "FALSE",
        data.isSample ? "TRUE" : "FALSE",
        QString::number(data.score),
        QString::number(data.task->getId())
    };
        
    QString query = Query::create("INSERT INTO test_c (test_c_input_data, test_c_output_data, "
                                  "is_required, is_sample, test_c_score, task_c_id) "
                                  "VALUES ('@@', '@@', @@, @@, @@, @@) RETURNING rowid", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "task_add", {
        {"sql_operator", query}
    }), [data, callback](const Response& response)
    {
        size_t id = response.getRow(0).get("rowid").asUInt();
        
        std::unique_ptr<Test> test = std::make_unique<Test>();
        test->setData(data);
        test->setId(id);
        
        callback(std::move(test));
    });
}

void Test::dbUpdate(Test* test, const Test::Data& data, std::function<void ()> callback)
{
    if (test == nullptr || data.task == nullptr) {
        return;
    }
    
    DeletionMark deletionMark = test->getDeletionMark();
    
    std::vector<QString> arguments = {
        data.inputData,
        data.outputData,
        data.isRequired ? "TRUE" : "FALSE",
        data.isSample ? "TRUE" : "FALSE",
        QString::number(data.score),
        QString::number(data.task->getId()),
        QString::number(data.id)
    };
    
    QString query = Query::create("UPDATE test_c SET test_c_input_data='@@', test_c_output_data='@@', "
                                  "is_required=@@, is_sample=@@, test_c_score=@@, task_c_id=@@ "
                                  "WHERE rowid=@@", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "test_edit", {
        {"sql_operator", query}
    }), [test, deletionMark, data, callback](const Response& response)
    {        
        if (*deletionMark == true) {
            return;
        }
        
        test->setData(data);
        
        callback();
    });
}

void Test::dbDelete(Test* test, std::function<void ()> callback)
{
    if (test == nullptr) {
        return;
    }
    
    DeletionMark deletionMark = test->getDeletionMark();
    
    std::vector<QString> arguments = {
        QString::number(test->getId())
    };
    QString query = Query::create("DELETE FROM test_c WHERE rowid=@@", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "test_delete", {
        {"sql_operator", query}
    }), [deletionMark, callback](const Response& response)
    {
        if (*deletionMark == true) {
            return;
        }
        
        callback();
    });
}


Test::Data::Data() :
    id(0), score(0), isRequired(false), isSample(false), task(nullptr)
{
    
}

Test::Data::Data(const Response::Row& row) :
    task(nullptr)
{
    id = row.get("rowid").asUInt();
    inputData = row.get("test_c_input_data").asString();
    outputData = row.get("test_c_output_data").asString();
    isRequired = row.get("is_required").asBool();
    isSample = row.get("is_sample").asBool();
    score = row.get("test_c_score").asUInt();
}

Test::Data::Data(const QString& inputData, const QString& outputData, 
                 size_t score, bool isRequired, bool isSample, Task* task) :
    id(0), inputData(inputData), outputData(outputData), score(score), isRequired(isRequired),
    isSample(isSample), task(task)
{
}
