#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QDomDocument>

#include "RequestElement.h"

class Request : public QObject
{
    Q_OBJECT
    
public:
    Request(QObject* parent = nullptr);
    Request(const std::vector<RequestElement>& elements, QObject* parent = nullptr);
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
