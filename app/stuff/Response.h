#ifndef RESPONSE_H
#define RESPONSE_H

#include <QString>

#include <functional>
#include <vector>
#include <map>

class Response
{
public:    
    class Cell
    {
    public:
        Cell() : m_isNull(true) {}
        
        Cell(const QString& data) : m_isNull(false), m_data(data) {}
        
        bool isNull() const { return m_isNull; }
        
        QString getData() const { return m_data; }
        
        int asInt() const { return m_data.toInt(); }
        
        unsigned int asUInt() const { return m_data.toUInt(); }
        
        double asDouble() const { return m_data.toDouble(); }
        
        QString asString() const { return m_data; }
        
    private:
        bool m_isNull;
        QString m_data;
    };
    
    class Row
    {
    public:
        Row() {}
        
        Row(const std::vector<std::pair<QString, Cell>>& data)
        {
            for (const auto& row : data) {
                m_data[row.first] = row.second;
            }
        }
        
        bool isNull() const { return m_data.empty(); }
        
        Cell get(const QString& name) const 
        {
            auto it = m_data.find(name);
            if (it != m_data.end()) {
                return it->second;
            }
            else {
                return Cell();
            }
        }
        
        bool has(const QString& name) const
        {
            return m_data.find(name) != m_data.end();
        }
        
        std::vector<QString> getColumns() const
        {
            std::vector<QString> result(m_data.size());
            size_t i = 0;
            for (auto it = m_data.begin(); it != m_data.end(); ++it, ++i) {
                result[i] = it->first;
            }
            return result;
        }
        
    private:
        std::map<QString, Cell> m_data;
    };
    
    Response(const QString& message);
    
    QString getTask() const;
    size_t getTaskId() const;
    
    Row getRow(size_t index) const;
    size_t getRowCount() const;
    
    bool hasError() const;
    QString getError() const;
    
    void map(std::function<void(size_t, const Row&)> f) const;
    
private:
    QString m_task;
    size_t m_taskId;
    QString m_error;
    
    std::vector<Row> m_data;
};

#endif // RESPONSE_H
