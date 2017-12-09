#include "TaskEditController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"

TaskEditController::TaskEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentTask(nullptr)
{
    connect(m_ui->openTestsButton, &QPushButton::pressed, this, &TaskEditController::testsOpened);
    connect(m_ui->saveTaskChangesButton, &QPushButton::pressed, this, &TaskEditController::changesSaved);
    connect(m_ui->cancelTaskChangesButton, &QPushButton::pressed, this, &TaskEditController::changesCanceled);

    m_ui->taskEditSamplesTable->setModel(ModelManager::getSamplesTableModel());
    for (int c = 0; c < m_ui->taskEditSamplesTable->horizontalHeader()->count(); ++c) {
        ui->taskEditSamplesTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
}

TaskEditController::~TaskEditController()
{
}

void TaskEditController::propose()
{
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TASK);
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_TASK);
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_TASK);
    
    m_ui->taskEditName->setEnabled(true);
    m_ui->taskEditScore->setEnabled(true);
    
    ModelManager::getSamplesTableModel()->fill(m_currentTask);
    ModelManager::getTestsTableModel(m_currentTask)->setTask(m_currentTask);
}

Task* TaskEditController::getCurrentTask()
{
    return m_currentTask;
}

void TaskEditController::setTask(Task* task)
{
    if (task == nullptr) {
        return;
    }
    
    m_currentTask = task;
    
    m_ui->nodeType->setText("Задача");
    m_ui->taskEditName->setPlainText(task->getName());
    m_ui->taskEditScore->setValue(task->getScore());
    m_ui->taskEditText->setPlainText(task->getText());
    m_ui->taskEditInputData->setPlainText(task->getInputData());
    m_ui->taskEditOutputData->setPlainText(task->getOutputData());
    m_ui->taskEditSource->setPlainText(task->getSource());
    
    propose();
}
