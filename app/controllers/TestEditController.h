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
    void setEditable(bool editable);
    bool isEditable() const;
    
    void saveCurrentTest();
    
    void setTest(Test* test);
    Test* getCurrentTest() const;
    
signals:
    void addTestButtonPressed();
    void editButtonPressed();
    void deleteButtonPressed();
    void exitTestsButtonPressed();
    void saveChangesButtonPressed();
    void cancelChangesButtonPressed();
    
private:
    Test* m_currentTest;
    bool m_isEditable;
};

#endif // TESTEDITCONTROLLER_H
