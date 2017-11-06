#ifndef TASKEDITCONTROLLER_H
#define TASKEDITCONTROLLER_H

#include "Controller.h"

#include "../objects/Task.h"

class TaskEditController : public Controller
{
    Q_OBJECT
    
public:
    TaskEditController(Ui::MainWindow* ui, QObject* parent = nullptr);
    ~TaskEditController();
    
    void propose() override;
    
public slots:
    void setTask(Task* task);
    
signals:
    void changesSaved();
    void changesCanceled();
    void testsOpened();
    
private:
    Task* m_currentTask;
};

#endif // TASKEDITCONTROLLER_H
