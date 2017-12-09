#include "Response.h"

#include <QDomDocument>
#include <QDebug>

#include "Log.h"

Response::Response(const QString& message)
{
    QDomDocument document;
    document.setContent(message);
    
    m_task = document.elementsByTagName("arm_task").at(0).firstChild().nodeValue();
    m_taskId = document.elementsByTagName("arm_task_id").at(0).firstChild().nodeValue().toUInt();
    m_error = document.elementsByTagName("error_text").at(0).firstChild().nodeValue();
        
    Log::write(document.toString(4).toLatin1().toStdString());
    
    QDomNodeList xmlData = document.elementsByTagName("sql_xml");
    
    if (xmlData.size() != 0) {
        QDomDocument data("XML");
        data.setContent(xmlData.at(0).firstChild().nodeValue());
        
        
        QDomNodeList rowsData = data.firstChild().childNodes();
        for (int i = 0; i < rowsData.size(); ++i) {
            QDomNode rowData = rowsData.item(i);
            QDomNodeList columnsData = rowData.childNodes();
            
            std::vector<std::pair<QString, Cell>> cells;
            for (int j = 0; j < columnsData.size(); ++j) {
                QDomNode columnData = columnsData.item(j);
                QDomNode nullAttribute = columnData.attributes().namedItem("xsi:nil");
                
                QString columnName = columnData.nodeName();
                
                if (nullAttribute.nodeValue() == "true") {
                    cells.push_back({ columnName, Cell() });
                }
                else {
                    QString value = columnData.firstChild().nodeValue();
                    cells.push_back({ columnName, Cell(value) });
                }
            }
            
            m_data.emplace_back(cells);
        }
    }
}

QString Response::getTask() const
{
    return m_task;
}

size_t Response::getTaskId() const
{
    return m_taskId;
}

Response::Row Response::getRow(size_t index) const
{
    size_t size = m_data.size();
    if (index < size) {
        return m_data[index];
    }
    else {
        return Row();
    }
}

size_t Response::getRowCount() const
{
    return m_data.size();
}

bool Response::hasError() const
{
    return m_error.size() != 0;
}

QString Response::getError() const
{
    return m_error;
}

void Response::map(std::function<void (size_t, const Row&)> f) const
{
    for (size_t i = 0; i < m_data.size(); ++i) {
        f(i, m_data[i]);
    }
}
