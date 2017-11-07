#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "../models/CoursesListModel.h"
#include "../models/CourseTreeModel.h"
#include "../models/TestsTableModel.h"

//NOTE: It's a temp class which provides access to models for all
//      controllers. Later it will be transformed into class, which
//      synchronize models with server

class ModelManager
{
public:
    static void init(QObject* parent = nullptr);
    static void close();
    
    static CoursesListModel* getCoursesListModel();
    static CourseTreeModel* getCourseTreeModel(Course* course);
    static TestsTableModel* getTestsTableModel(Task* task);
    
private:
    static QObject* m_parent;
    
    static std::unique_ptr<CoursesListModel> m_coursesListModel;
    static std::unique_ptr<CourseTreeModel> m_courseTreeModel;
    static std::unique_ptr<TestsTableModel> m_testsTableModel;
    
    static std::vector<std::unique_ptr<Course>> m_courses;
};

#endif // MODELMANAGER_H
