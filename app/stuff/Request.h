#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QDomDocument>

#include <vector>

class Request : public QObject
{
    Q_OBJECT
    
public:
    struct Element
    {
        Element(const QString& name, const QString& value) :
            name(name), value(value)
        {}

        Element(const QString &name, const std::vector<Element>& children) :
            name(name), value(""), children(children)
        {}

        QString name;
        QString value;
        std::vector<Element> children;
    };
    
    Request(QObject* parent = nullptr);
    Request(const std::vector<Element>& elements, QObject* parent = nullptr);
    ~Request();
    
    bool hasElement(const QString& name) const;
    
    void addElement(const QString& name, const QString& value);
    QString getElement(const QString& name) const;
    
    QString getData() const;
    
private:
    QDomDocument m_data;
};

#endif // REQUEST_H
