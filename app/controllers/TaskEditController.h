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
    void setEditable(bool editable);
    bool isEditable() const;
    
    void saveCurrentTask(); 
    
    void setTask(Task* task);
    Task* getCurrentTask();
    
signals:
    void editButtonPressed();
    void deleteButtonPressed();
    void openTestsButtonPressed();
    void saveChangesButtonPressed();
    void cancelChangesButtonPressed();
    
private:
    Task* m_currentTask;
    bool m_isEditable;
};

#endif // TASKEDITCONTROLLER_H
