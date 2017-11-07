#include "TestsTableController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"

TestsTableController::TestsTableController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentTask(nullptr), m_selectedTest(nullptr)
{
    connect(m_ui->testsTable, &QTableView::pressed, [this](const QModelIndex& index) {
       Test* test = reinterpret_cast<Test*>(index.internalPointer()); 
       m_selectedTest = test;
       emit testSelected(test);
    });
    
    connect(m_ui->closeTestsButton, &QPushButton::pressed, this, &TestsTableController::backButtonPressed);
}

TestsTableController::~TestsTableController()
{
}

void TestsTableController::propose()
{
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TESTS);
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_EMPTY);
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_TESTS_TABLE);
}

Test* TestsTableController::getSelectedTest()
{
    return m_selectedTest;
}

void TestsTableController::setTask(Task* task)
{
    if (task != nullptr && m_currentTask != task) {
        TestsTableModel* testsModel = ModelManager::getTestsTableModel(task);
        
        m_ui->testsTable->setModel(testsModel);
        
        m_selectedTest = nullptr;
        emit testSelected(m_selectedTest);
    }
    
    m_currentTask = task;
    
    propose();
}
