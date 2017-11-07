#ifndef TESTEDITCONTROLLER_H
#define TESTEDITCONTROLLER_H

#include "Controller.h"
#include "../objects/Test.h"

class TestEditController : public Controller
{
    Q_OBJECT
    
public:
    TestEditController(Ui::MainWindow* ui, QObject* parent = nullptr);
    ~TestEditController();
    
    void propose() override;
    
public slots:
    void setTest(Test* test);
    
signals:
    void changesSaved();
    void changesCanceled();
    
private:
    Test* m_currentTest;
};

#endif // TESTEDITCONTROLLER_H
