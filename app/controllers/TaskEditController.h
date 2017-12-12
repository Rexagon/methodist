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
    
    void saveChanges();
    void propose() override;
    
    Task* getCurrentTask();
    
public slots:
    void setTask(Task* task);
    
signals:
    void testsOpened();
    void changesSaved();
    void changesCanceled();
    
private:
    Task* m_currentTask;
};

#endif // TASKEDITCONTROLLER_H
