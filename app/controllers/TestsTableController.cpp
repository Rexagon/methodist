#include "TestsTableController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"

#include "../stuff/Log.h"

TestsTableController::TestsTableController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentTask(nullptr), m_selectedTest(nullptr)
{
    connect(m_ui->testsTable, &QTableView::pressed, [this](const QModelIndex& index) {
        Test* test = m_currentTask->getTest(static_cast<size_t>(index.row()));
        
        if (test != nullptr) {
            emit testSelected(test);
        }
    });
    
    m_ui->testsTable->resizeColumnsToContents();
    /*for (int c = 0; c < m_ui->testsTable->horizontalHeader()->count(); ++c) {
        m_ui->testsTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    }*/
}

TestsTableController::~TestsTableController()
{
}

void TestsTableController::propose()
{
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TESTS);
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_TESTS_TABLE);
}

Task* TestsTableController::getCurrentTask()
{
    return m_currentTask;
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

void TestsTableController::selectTest(Test* test)
{
    TestsTableModel* model = ModelManager::getTestsTableModel(m_currentTask);
    int index = model->getTestIndex(test);
    if (index != -1) {
        m_ui->testsTable->selectionModel()->select(model->index(index, 0), 
                QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        
        emit testSelected(test);
    }
    else {
        deselectAll();
    }
}

void TestsTableController::deselectAll()
{
    m_ui->testsTable->selectionModel()->clearSelection();
}
