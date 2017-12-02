#include "Request.h"

#include <stack>
#include <map>

const QString Request::m_dataRootName = "cmd";
size_t Request::m_currentId = 0;

Request::Request(CommandType command, const QString& task, const std::vector<Element>& elements) :
    m_task(task), m_taskId(m_currentId++)
{
    QDomElement root = m_data.createElement(m_dataRootName);
    m_data.appendChild(root);
    
    std::map<QString, QString> reservedElements = {
        {"cmd_type", QString::number(command) },
        {"arm_task", task },
        {"arm_task_id", QString::number(m_taskId) }
    };
    
    for (const auto& e : reservedElements) {
        addElement(e.first, e.second, &root);
    }

    std::stack<std::pair<QDomElement, size_t>> rootsStack;
    
    std::stack<const Element*> elementsStack;
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        QString elementName = it->getName();
        if (reservedElements.find(elementName) == reservedElements.end()) {
            elementsStack.push(&(*it));
        }
    }
    
    while(!elementsStack.empty()) {
        const Element* element = elementsStack.top();
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

QString Request::getData() const
{
    return m_data.toString(0);
}

QString Request::getTask() const
{
    return m_task;
}

size_t Request::getTaskId() const
{
    return m_taskId;
}

void Request::addElement(const QString& name, const QString& value, QDomElement* parent)
{
    if (parent == nullptr) {
        return;
    }
    
    QDomElement node = m_data.createElement(name);
    parent->appendChild(node);
    
    node.appendChild(m_data.createTextNode(value));
}
