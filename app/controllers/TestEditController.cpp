#include "TestEditController.h"

#include <ui_MainWindow.h>

TestEditController::TestEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentTest(nullptr)
{
    connect(m_ui->saveTestChangesButton, &QPushButton::pressed, this, &TestEditController::changesSaved);
    connect(m_ui->cancelTestChangesButton, &QPushButton::pressed, this, &TestEditController::changesCanceled);
}

TestEditController::~TestEditController()
{
}

void TestEditController::propose()
{
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TESTS);
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_TEST);
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_TEST);
    
    m_ui->testEditInputData->setEnabled(true);
    m_ui->testEditScore->setEnabled(true);
}

void TestEditController::setTest(Test* test)
{
    if (test == nullptr) {
        return;
    }
    
    m_currentTest = test;
    
    m_ui->nodeType->setText("Тест");
    m_ui->testEditInputData->setPlainText(test->getInputData());
    m_ui->testEditScore->setValue(test->getScore());
    
    propose();
}
