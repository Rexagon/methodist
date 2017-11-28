#ifndef REQUESTELEMENT_H
#define REQUESTELEMENT_H

#include <QString>

#include <vector>

class RequestElement
{    
public:
    RequestElement(const QString& name);
    RequestElement(const QString& name, int value);
    RequestElement(const QString& name, const QString& value);
    RequestElement(const QString& name, const std::vector<RequestElement>& children);
    ~RequestElement();
    
    void setName(const QString& name);
    QString getName() const;

    void setValue(const QString& value);
    QString getValue() const;
    
    void addChild(const RequestElement& element);
    const std::vector<RequestElement>& getChildren() const;
    size_t getChildrenCount() const;
    bool hasChildren() const;
    
private:
    QString m_name;
    QString m_value;
    std::vector<RequestElement> m_children;
};

#endif // REQUESTELEMENT_H
