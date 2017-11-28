#include "RequestElement.h"

RequestElement::RequestElement(const QString& name) : 
    m_name(name)
{
}

RequestElement::RequestElement(const QString& name, int value) :
    m_name(name), m_value(QString::number(value))
{   
}

RequestElement::RequestElement(const QString& name, const QString& value) :
    m_name(name), m_value(value)
{
}

RequestElement::RequestElement(const QString& name, const std::vector<RequestElement>& children) :
    m_name(name), m_children(children)
{
}

RequestElement::~RequestElement()
{
}

void RequestElement::setName(const QString& name)
{
    m_name = name;
}

QString RequestElement::getName() const
{
    return m_name;
}

void RequestElement::setValue(const QString& value)
{
    m_value = value;
}

QString RequestElement::getValue() const
{
    return m_value;
}

void RequestElement::addChild(const RequestElement& element)
{
    m_children.push_back(element);
}

const std::vector<RequestElement>& RequestElement::getChildren() const
{
    return m_children;
}

size_t RequestElement::getChildrenCount() const
{
    return m_children.size();
}

bool RequestElement::hasChildren() const
{
    return !m_children.empty();
}
