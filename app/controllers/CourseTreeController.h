#ifndef COURSETREECONTROLLER_H
#define COURSETREECONTROLLER_H

#include "Controller.h"
#include "../objects/Course.h"

class CourseTreeController : public Controller
{
    Q_OBJECT
    
public:
    CourseTreeController(Ui::MainWindow* ui, QObject* parent = nullptr);
    ~CourseTreeController();
    
    void propose() override;
    
    Course* getCurrentCourse();
    CourseNode* getSelectedCourseNode();
    
public slots:
    void setCourse(Course* course);
    
signals:
    void courseNodeSelected(CourseNode* node);
    
private:    
    Course* m_currentCourse;
    CourseNode* m_selectedCourseNode;
};

#endif // COURSETREECONTROLLER_H
