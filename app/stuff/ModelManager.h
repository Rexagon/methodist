#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "../models/CoursesListModel.h"
#include "../models/CourseTreeModel.h"
#include "../models/TestsTableModel.h"
#include "../models/SamplesTableModel.h"

//NOTE: It's a temp class which provides access to models for all
//      controllers. Later it will be transformed into class, which
//      synchronize models with server

class ModelManager
{
public:
    static void init(QObject* parent = nullptr);
    static void close();
    
    static void update();
    
    static CoursesListModel* getCoursesListModel();
    static CourseTreeModel* getCourseTreeModel(Course* course);
    static TestsTableModel* getTestsTableModel(Task* task);
    static SamplesTableModel* getSamplesTableModel();
    
private:
    static QObject* m_parent;
    
    static std::unique_ptr<CoursesListModel> m_coursesListModel;
    static std::unique_ptr<CourseTreeModel> m_courseTreeModel;
    static std::unique_ptr<TestsTableModel> m_testsTableModel;
    static std::unique_ptr<SamplesTableModel> m_samplesTableModel;
};

#endif // MODELMANAGER_H
