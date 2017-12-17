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
    void setEditable(bool editable);
    bool isEditable() const;
    
    void saveCurrentCourse();
    void deleteCurrentCourse();
    
    void setCourse(Course* course);
    Course* getCurrentCourse() const;
    
signals:
    void addSectionButtonPressed();
    void editButtonPressed();
    void deleteButtonPressed();
    void saveChangesButtonPressed();
    void cancelChangesButtonPressed();
    
private:
    Course* m_currentCourse;
    bool m_isEditable;
};

#endif // COURSEEDITCONTROLLER_H
