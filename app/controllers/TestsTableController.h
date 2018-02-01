#ifndef TESTSTABLECONTROLLER_H
#define TESTSTABLECONTROLLER_H

#include "Controller.h"
#include "../objects/Task.h"

class TestsTableController : public Controller
{
    Q_OBJECT
    
public:
    TestsTableController(Ui::MainWindow* ui, QObject* parent = nullptr);
    ~TestsTableController();
    
    void propose() override;
    
    Task* getCurrentTask();
    Test* getSelectedTest();
    
public slots:
    void setTask(Task* task);
    void selectTest(Test* test);
    void deselectAll();
    
signals:
    void testSelected(Test* test);
    
private:
    Task* m_currentTask;
    Test* m_selectedTest;
};

#endif // TESTSTABLECONTROLLER_H
