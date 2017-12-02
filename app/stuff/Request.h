#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <QDomDocument>

#include <vector>

enum CommandType
{
    SQL_TEMPLATE_INSERT = 2,
    SQL_TEMPLATE_UPDATE = 3,
    SQL_TEMPLATE_DELETE = 4,
    SQL_TEMPLATE_OPERATOR = 5,
    SQL_TEMPLATE_QUERY_XML = 6,
    SQL_TEMPLATE_QUERY_BLOB = 7,
    TRANSACTION = 8,
    COMMIT = 9,
    ROLLBACK = 10,
    TASK_TESTING = 11,
    USER_REGISTER = 12,
    USER_LOGIN = 13,
    
    SQL_OPERATOR = 15,
    SQL_QUERY_XML = 16
};

class Request
{
public:
    class Element
    {    
    public:
        Element(const QString& name) : m_name(name) {}
        Element(const QString& name, int value) : m_name(name), m_value(QString::number(value)) {}
        Element(const QString& name, size_t value) : m_name(name), m_value(QString::number(value)) {}
        Element(const QString& name, const QString& value) : m_name(name), m_value(value) {}
        Element(const QString& name, const std::vector<Element>& children) : m_name(name), m_children(children) {}
        
        void setName(const QString& name) { m_name = name; }
        QString getName() const { return m_name; }
    
        void setValue(const QString& value) { m_value = value; }
        QString getValue() const { return m_value; }
        
        void addChild(const Element& element) { m_children.push_back(element); }
        const std::vector<Element>& getChildren() const { return m_children; }
        size_t getChildrenCount() const { return m_children.size(); }
        bool hasChildren() const { return !m_children.empty(); }
        
    private:
        QString m_name;
        QString m_value;
        std::vector<Element> m_children;
    };
    
    Request(CommandType command, const QString& task, const std::vector<Element>& elements);
    ~Request();
    
    QString getTask() const;
    size_t getTaskId() const;
    
    QString getData() const;
    
private:
    void addElement(const QString& name, const QString& value, QDomElement* parent);
    
    static const QString m_dataRootName;
    static size_t m_currentId;
    
    QString m_task;
    size_t m_taskId;
    QDomDocument m_data;
};

#endif // REQUEST_H
