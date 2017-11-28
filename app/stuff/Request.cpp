#include "Request.h"

#include <stack>

const QString Request::m_dataRootName = "cmd";

Request::Request(QObject* parent) :
    QObject(parent), m_data(m_dataRootName)
{
    QDomElement root = m_data.createElement(m_dataRootName);
    m_data.appendChild(root);
}

Request::Request(const std::vector<RequestElement>& elements, QObject* parent) :
    QObject(parent), m_data(m_dataRootName)
{
    QDomElement root = m_data.createElement(m_dataRootName);
    m_data.appendChild(root);

    std::stack<std::pair<QDomElement, size_t>> rootsStack;
    
    std::stack<const RequestElement*> elementsStack;
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        elementsStack.push(&(*it));
    }
    
    while(!elementsStack.empty()) {
        const RequestElement* element = elementsStack.top();
        elementsStack.pop();
        
        QDomElement parent;
        if (rootsStack.empty()) {
            parent = root;
        }
        else {
            parent = rootsStack.top().first;
            if ((--rootsStack.top().second) == 0) {
                rootsStack.pop();
            }
        }
        
        QDomElement node = m_data.createElement(element->getName());
        parent.appendChild(node);
        
        if (element->hasChildren()) {
            auto children = element->getChildren();
            
            for (auto it = children.rbegin(); it != children.rend(); ++it) {
                elementsStack.push(&(*it));
            }
            
            rootsStack.push({ node, children.size()});    
        }
        
        node.appendChild(m_data.createTextNode(element->getValue()));
    }
}

Request::~Request()
{
    
}

bool Request::hasElement(const QString& name) const
{
    return m_data.elementsByTagName(name).size() != 0;
}

QString Request::getElement(const QString& name) const
{
    return "";
}

QString Request::getData() const
{
    return m_data.toString(0);
}
