#include "TestEditController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"
#include "../stuff/NetworkManager.h"

TestEditController::TestEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentTest(nullptr), m_isEditable(false)
{
    connect(m_ui->addTestButton, &QPushButton::pressed, this, &TestEditController::addTestButtonPressed);
    connect(m_ui->editTestButton, &QPushButton::pressed, this, &TestEditController::editButtonPressed);
    connect(m_ui->deleteTestButton, &QPushButton::pressed, this, &TestEditController::deleteButtonPressed);
    connect(m_ui->exitTestsButton, &QPushButton::pressed, this, &TestEditController::exitTestsButtonPressed);
    connect(m_ui->saveTestChangesButton, &QPushButton::pressed, this, &TestEditController::saveChangesButtonPressed);
    connect(m_ui->cancelTestChangesButton, &QPushButton::pressed, this, &TestEditController::cancelChangesButtonPressed);
}

TestEditController::~TestEditController()
{
}

void TestEditController::saveCurrentTest()
{
    if (m_currentTest == nullptr) {
        return;
    }
    
    Test* test = m_currentTest;
    
    Test::Data testData = test->getData();
    testData.inputData = m_ui->testEditInputData->toPlainText();
    testData.outputData = m_ui->testEditOutputData->toPlainText();
    testData.score = m_ui->testEditScore->value();
    testData.isRequired = m_ui->testEditRequired->isChecked();
    testData.isSample = m_ui->testEditSample->isChecked();
    
    Test::dbUpdate(test, testData, [this, test]() {
        ModelManager::getSamplesTableModel()->fill(test->getTask());
        ModelManager::getTestsTableModel(test->getTask())->update();
    });
}

void TestEditController::deleteCurrentTest()
{
    
}

void TestEditController::propose()
{
    m_ui->infoPanel->setVisible(true);
    
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TESTS);
    if (m_currentTest == nullptr) {
        m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_TESTS_TABLE);
    }
    else {
        m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_TEST);
    }
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_TEST);
    
    m_ui->testEditInputData->setEnabled(m_isEditable);
    m_ui->testEditOutputData->setEnabled(m_isEditable);
    m_ui->testEditScore->setEnabled(m_isEditable);
    m_ui->testEditRequired->setEnabled(m_isEditable);
    m_ui->testEditSample->setEnabled(m_isEditable);
    
    m_ui->addTestButton->setVisible(!m_isEditable);
    m_ui->editTestButton->setVisible(!m_isEditable && m_currentTest != nullptr);
    m_ui->deleteTestButton->setVisible(!m_isEditable && m_currentTest != nullptr);
    m_ui->exitTestsButton->setVisible(!m_isEditable);
    m_ui->saveTestChangesButton->setVisible(m_isEditable);
    m_ui->cancelTestChangesButton->setVisible(m_isEditable);
}

void TestEditController::setEditable(bool editable)
{
    m_isEditable = editable;
}

bool TestEditController::isEditable() const
{
    return m_isEditable;
}

void TestEditController::setTest(Test* test)
{
    m_currentTest = test;
    
    if (test == nullptr) {
        propose();
        return;
    }
    
    m_ui->testEditInputData->setPlainText(test->getInputData());
    m_ui->testEditOutputData->setPlainText(test->getOutputData());
    m_ui->testEditScore->setValue(test->getScore());
    m_ui->testEditRequired->setChecked(test->isRequired());
    m_ui->testEditSample->setChecked(test->isSample());
    
    propose();
}

Test* TestEditController::getCurrentTest() const
{
    return m_currentTest;
}
