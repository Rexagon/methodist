#ifndef COURSEEDITCONTROLLER_H
#define COURSEEDITCONTROLLER_H

#include "Controller.h"
#include "../objects/Course.h"

class CourseEditController : public Controller
{
    Q_OBJECT
    
public:
    CourseEditController(Ui::MainWindow* ui, QObject* parent = nullptr);
    ~CourseEditController();
    
    void propose() override;
    
public slots:
    void setCourse(Course* course);
    
signals:
    void changesSaved();
    void changesCanceled();
    
private:
    Course* m_currentCourse;
};

#endif // COURSEEDITCONTROLLER_H
