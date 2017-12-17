#include "Course.h"

Course::Course() :
    CourseNode(CourseNode::Type::COURSE)
{
}

Course::~Course()
{
    m_sections.clear();
}

void Course::setId(size_t id)
{
    m_data.id = id;
}

size_t Course::getId() const
{
    return m_data.id;
}

void Course::setName(const QString& name)
{
    m_data.name = name;
}

QString Course::getName() const
{
    return m_data.name;
}

void Course::setLectureHourCount(size_t n)
{
    m_data.lectureHourCount = n;
}

size_t Course::getLectureHourCount() const
{
    return m_data.lectureHourCount;
}

void Course::setPracticeHourCount(size_t n)
{
    m_data.practiceHourCount = n;
}

size_t Course::getPracticeHourCount() const
{
    return m_data.practiceHourCount;
}

void Course::setLaboratoryHourCount(size_t n)
{
    m_data.laboratoryHourCount = n;
}

size_t Course::getLaboratoryHourCount() const
{
    return m_data.laboratoryHourCount;
}

void Course::setData(const Course::Data& data)
{
    m_data = data;
}

Course::Data Course::getData() const
{
    return m_data;
}

void Course::addSection(std::unique_ptr<Section> section)
{
    section->setCourse(this);
    addChild(section.get());
    m_sections.push_back(std::move(section));
}

void Course::removeSection(const Section* section)
{
    for (auto it = m_sections.begin(); it != m_sections.end(); ++it) {
        if (it->get() == section) {
            removeChild(section);
            m_sections.erase(it);
            return;
        }
    }
}

Section* Course::getSection(size_t n) const
{
    if (n < m_sections.size()) {
        return m_sections[n].get();
    }
    
    return nullptr;
}

int Course::getSectionIndex(const Section* section)
{
    for (size_t i = 0; i < m_sections.size(); ++i) {
        if (m_sections[i].get() == section) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

size_t Course::getSectionCount() const
{
    return m_sections.size();
}

void Course::dbCreate(const Course::Data& data, std::function<void (std::unique_ptr<Course>)> callback)
{
    std::vector<QString> arguments = {
        data.name, 
        QString::number(data.lectureHourCount), 
        QString::number(data.practiceHourCount),
        QString::number(data.laboratoryHourCount)
    };
    
    QString query = Query::create("INSERT INTO course (course_name, lecture_hours, practice_hours, laboratory_hours) "
                                  "VALUES ('@@', @@, @@, @@) RETURNING rowid", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "course_add", {
        {"sql_operator", query}
    }), [data, callback](const Response& response)
    {
        qDebug() << response.getRowCount() << "|";
        qDebug() << response.getRow(0).get("rowid").asUInt();
        
        std::unique_ptr<Course> course = std::make_unique<Course>();
        course->setData(data);
        
        callback(std::move(course));
    });
}

void Course::dbUpdate(Course* course, const Course::Data& data, std::function<void()> callback)
{
    if (course == nullptr) {
        return;
    }
    
    DeletionMark deletionMark = course->getDeletionMark();
    
    std::vector<QString> arguments = {
        data.name, 
        QString::number(data.lectureHourCount), 
        QString::number(data.practiceHourCount),
        QString::number(data.laboratoryHourCount),
        QString::number(data.id)
    };
    
    QString query = Query::create("UPDATE course SET course_name='@@', lecture_hours=@@, practice_hours=@@, "
                                  "laboratory_hours=@@ WHERE rowid=@@", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "course_edit", {
        {"sql_operator", query}
    }), [course, deletionMark, data, callback](const Response& response)
    {
        if (*deletionMark == true) {
            return;
        }
        
        course->setData(data);
        
        callback();
    });
}

void Course::dbDelete(Course* course, std::function<void ()> callback)
{
    if (course == nullptr) {
        return;
    }
    
    DeletionMark deletionMark = course->getDeletionMark();
    
    std::vector<QString> arguments = {
        QString::number(course->getId())
    };
    QString query = Query::create("DELETE FROM course WHERE rowid=@@", arguments);
    
    NetworkManager::send(Request(SQL_OPERATOR, "course_delete", {
        {"sql_operator", query}
    }), [deletionMark, callback](const Response& response)
    {
        if (*deletionMark == true) {
            return;
        }
        
        callback();
    });
}


Course::Data::Data() :
    id(0), lectureHourCount(0), practiceHourCount(0), laboratoryHourCount(0)
{
}

Course::Data::Data(const Response::Row& row)
{
    id = row.get("rowid").asUInt();
    name = row.get("course_name").asString();
    lectureHourCount = row.get("lecture_hours").asUInt();
    practiceHourCount = row.get("practice_hours").asUInt();
    laboratoryHourCount = row.get("laboratory_hours").asUInt();
}

Course::Data::Data(const QString& name, size_t lectureHourCount, size_t practiceHourCount, size_t laboratoryHourCount) :
    id(0), name(name), lectureHourCount(lectureHourCount), practiceHourCount(practiceHourCount), 
    laboratoryHourCount(laboratoryHourCount)
{
}

Course::Data::Data(size_t id, const QString& name, size_t lectureHourCount, size_t practiceHourCount, size_t laboratoryHourCount) :
    id(id), name(name), lectureHourCount(lectureHourCount), practiceHourCount(practiceHourCount), 
    laboratoryHourCount(laboratoryHourCount)
{
}
