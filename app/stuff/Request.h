#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QDomDocument>

#include "RequestElement.h"

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
    Request(const std::vector<RequestElement>& elements);
    Request(CommandType command, const QString& task, const std::vector<RequestElement>& elements);
    ~Request();
    
    bool hasElement(const QString& name) const;
    QString getElement(const QString& name) const;
    
    QString getData() const;
    
private:
    static const QString m_dataRootName;
    
    QString m_taskId;
    QDomDocument m_data;
};

#endif // REQUEST_H
