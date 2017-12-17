#include "Section.h"

#include <stack>

#include "Course.h"

Section::Section() :
    CourseNode(CourseNode::Type::SECTION)
{
}

Section::~Section()
{
    m_tasks.clear();
}

void Section::setId(size_t id)
{
    m_data.id = id;
}

size_t Section::getId() const
{
    return m_data.id;
}

void Section::setName(const QString& name)
{
    m_data.name = name;
}

QString Section::getName() const
{
    return m_data.name;
}

void Section::setCourse(Course* course)
{
    m_data.course = course;
    
    std::stack<Section*> sections;
    sections.push(this);
    while (!sections.empty()) {
        Section* section = sections.top();
        sections.pop();
        
        for (auto& s : section->m_subsections) {
            s->m_data.course = course;
            sections.push(s.get());
        }
    }
}

Course* Section::getCourse() const
{
    return m_data.course;
}

void Section::setParentSection(Section* parentSection)
{
    m_data.parentSection = parentSection;
}

Section*Section::getParentSection()
{
    return m_data.parentSection;
}

void Section::setData(const Section::Data& data)
{
    m_data = data;
}

Section::Data Section::getData() const
{
    return m_data;
}

void Section::addSubsection(std::unique_ptr<Section> subsection)
{
    subsection->setCourse(m_data.course);
    subsection->setParentSection(this);
    
    addChild(subsection.get());
    m_subsections.push_back(std::move(subsection));
}

void Section::removeSubsection(const Section* subsection)
{
    for (auto it = m_subsections.begin(); it != m_subsections.end(); ++it) {
        if (it->get() == subsection) {
            removeChild(subsection);
            m_subsections.erase(it);
            return;
        }
    }
}

Section* Section::getSubsection(size_t n) const
{
    if (n < m_subsections.size()) {
        return m_subsections[n].get();
    }
    
    return nullptr;
}

int Section::getSubsectionIndex(const Section* subsection)
{
    for (size_t i = 0; i < m_subsections.size(); ++i) {
        if (m_subsections[i].get() == subsection) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t Section::getSubsectionCount() const
{
    return m_subsections.size();
}

void Section::addTask(std::unique_ptr<Task> task)
{
    task->setSection(this);
    addChild(task.get());
    m_tasks.push_back(std::move(task));
}

void Section::removeTask(const Task* task)
{
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        if (it->get() == task) {
            removeChild(task);
            m_tasks.erase(it);
            return;
        }
    }
}

Task* Section::getTask(size_t n) const
{
    if (n < m_tasks.size()) {
        return m_tasks[n].get();
    }
    
    return nullptr;
}

int Section::getTaskIndex(const Task* task)
{
    for (size_t i = 0; i < m_tasks.size(); ++i) {
        if (m_tasks[i].get() == task) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t Section::getTaskCount() const
{
    return m_tasks.size();
}

void Section::dbCreate(const Section::Data& data, std::function<void (std::unique_ptr<Section>)> callback)
{
    if (data.course == nullptr) {
        return;
    }
    
    QString parentSectionId;
    if (data.parentSection == nullptr) {
        parentSectionId = "NULL";
    }
    else {
        parentSectionId = QString::number(data.parentSection->getId());
    }
    
    std::vector<QString> arguments = {
        data.name,
        QString::number(data.course->getId()),
        parentSectionId
    };
    QString query = Query::create("INSERT INTO section (section_name, course_id, parent_section_id) "
                    "VALUES ('@@', @@, @@) RETURNING rowid", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "section_add", {
        {"sql_operator", query}
    }), [data, callback](const Response& response)
    {        
        std::unique_ptr<Section> section = std::make_unique<Section>();
        section->setData(data);
        
        callback(std::move(section));
    });
}

void Section::dbUpdate(Section* section, const Section::Data& data, std::function<void ()> callback)
{
    if (section == nullptr || data.course == nullptr) {
        return;
    }
    
    DeletionMark deletionMark = section->getDeletionMark();
    
    QString parentSectionId;
    if (data.parentSection == nullptr) {
        parentSectionId = "NULL";
    }
    else {
        parentSectionId = QString::number(data.parentSection->getId());
    }
    
    std::vector<QString> arguments = {
        data.name,
        QString::number(data.course->getId()),
        parentSectionId,
        QString::number(section->getId())
    };
    
    QString query = Query::create("UPDATE section SET section_name='@@', course_id=@@, "
                                  "parent_section_id=@@ WHERE rowid=@@", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "section_edit", {
        {"sql_operator", query}
    }), [section, deletionMark, data, callback](const Response& response)
    {
        if (*deletionMark == true) {
            return;
        }
        
        section->setData(data);
        
        callback();
    });
}

void Section::dbDelete(Section* section, std::function<void ()> callback)
{
    if (section == nullptr) {
        return;
    }
    
    DeletionMark deletionMark = section->getDeletionMark();
    
    std::vector<QString> arguments = {
        QString::number(section->getId())
    };
    QString query = Query::create("DELETE FROM section WHERE rowid=@@", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "section_delete", {
        {"sql_operator", query}
    }), [deletionMark, callback](const Response& response)
    {
        if (*deletionMark == true) {
            return;
        }
        
        callback();
    });
}


Section::Data::Data() :
    id(0), course(nullptr), parentSection(nullptr)
{
}

Section::Data::Data(const Response::Row& row) :
    course(nullptr), parentSection(nullptr)
{
    id = row.get("rowid").asUInt();
    name = row.get("section_name").asString();
}

Section::Data::Data(const QString &name, Course* course, Section* parentSection) :
    id(0), name(name), course(course), parentSection(parentSection)
{
    
}

Section::Data::Data(size_t id, const QString& name, Course* course, Section* parentSection) :
    id(id), name(name), course(course), parentSection(parentSection)
{
    
}
