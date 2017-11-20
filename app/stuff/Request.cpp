#include "Request.h"

Request::Request(QObject* parent) :
    QObject(parent)
{
    QDomElement root = m_data.createElement("cmd");
    m_data.appendChild(root);
}

Request::Request(const std::vector<Element>& elements, QObject* parent) :
    QObject(parent), m_data("cmd")
{
    QDomElement root = m_data.createElement("cmd");
    m_data.appendChild(root);
    
    for (const auto& element : elements) {
        addElement(element.name, element.value);
    }
}

Request::~Request()
{
    
}

bool Request::hasElement(const QString& name) const
{
    return m_data.elementsByTagName(name).size() != 0;
}

void Request::addElement(const QString& name, const QString& value)
{    
    QDomElement node = m_data.createElement(name);
    node.appendChild(m_data.createTextNode(value));
    
    m_data.documentElement().toElement().appendChild(node);
}

QString Request::getElement(const QString& name) const
{
    return "";
}

QString Request::getData() const
{
    return m_data.toString(0);
}
