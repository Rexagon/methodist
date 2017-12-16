#include "TaskEditController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"
#include "../stuff/NetworkManager.h"

#include "../stuff/Log.h"
#include <QDebug>

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

void TaskEditController::saveChanges()
{
    if (m_currentTask == nullptr) {
        return;
    }
    
    Task* task = m_currentTask;
    
    Task::Data taskData = task->getData();
    taskData.name = m_ui->taskEditName->toPlainText();
    taskData.text = m_ui->taskEditText->toHtml();
    taskData.inputData = m_ui->taskEditInputData->toHtml();
    taskData.outputData = m_ui->taskEditOutputData->toHtml();
    taskData.source = m_ui->taskEditSource->toHtml();
    taskData.score = m_ui->taskEditScore->value();
    
    Task::dbUpdate(task, taskData, [this, task]() {
        ModelManager::getCourseTreeModel(task->getSection()->getCourse())->update();
    });
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
    m_ui->taskEditText->setHtml(task->getText());
    m_ui->taskEditInputData->setHtml(task->getInputData());
    m_ui->taskEditOutputData->setHtml(task->getOutputData());
    m_ui->taskEditSource->setHtml(task->getSource());
    
    propose();
}
