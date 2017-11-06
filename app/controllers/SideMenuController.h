#ifndef SIDEMENUCONTROLLER_H
#define SIDEMENUCONTROLLER_H

#include "Controller.h"
#include "../objects/Course.h"

class SideMenuController : public Controller
{
    Q_OBJECT
    
public:
    SideMenuController(Ui::MainWindow* ui, QObject* parent = nullptr);
    ~SideMenuController();
    
public slots:
    void selectCourse(Course* course);
    
signals:
    void courseAdded();
    void coursesRefreshed();
    void courseSelected(Course* course);
};

#endif // SIDEMENUCONTROLLER_H
