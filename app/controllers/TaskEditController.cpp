#include "TaskEditController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"
#include "../stuff/NetworkManager.h"

#include "../stuff/Log.h"
#include <QDebug>

TaskEditController::TaskEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentTask(nullptr), m_isEditable(true)
{
    connect(m_ui->editTaskButton, &QPushButton::pressed, this, &TaskEditController::editButtonPressed);
    connect(m_ui->deleteTaskButton, &QPushButton::pressed, this, &TaskEditController::deleteButtonPressed);
    connect(m_ui->openTestsButton, &QPushButton::pressed, this, &TaskEditController::openTestsButtonPressed);
    connect(m_ui->saveTaskChangesButton, &QPushButton::pressed, this, &TaskEditController::saveChangesButtonPressed);
    connect(m_ui->cancelTaskChangesButton, &QPushButton::pressed, this, &TaskEditController::cancelChangesButtonPressed);

    m_ui->taskEditSamplesTable->setModel(ModelManager::getSamplesTableModel());
    for (int c = 0; c < m_ui->taskEditSamplesTable->horizontalHeader()->count(); ++c) {
        ui->taskEditSamplesTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
}

TaskEditController::~TaskEditController()
{
}

void TaskEditController::saveCurrentTask()
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
    m_ui->infoPanel->setVisible(true);
    
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    if (m_isEditable) {
        m_ui->workspace->setCurrentIndex(WORKSPACE_TASK);
    }
    else {
        m_ui->workspace->setCurrentIndex(WORKSPACE_TREE);
    }
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_TASK);
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_TASK);
    
    m_ui->taskEditName->setEnabled(m_isEditable);
    m_ui->taskEditScore->setEnabled(m_isEditable);
    
    m_ui->editTaskButton->setVisible(!m_isEditable);
    m_ui->deleteTaskButton->setVisible(!m_isEditable);
    m_ui->openTestsButton->setVisible(m_isEditable);
    m_ui->saveTaskChangesButton->setVisible(m_isEditable);
    m_ui->cancelTaskChangesButton->setVisible(m_isEditable);
    
    if (m_isEditable) {
        ModelManager::getSamplesTableModel()->fill(m_currentTask);
        ModelManager::getTestsTableModel(m_currentTask)->setTask(m_currentTask);
    }
}

void TaskEditController::setEditable(bool editable)
{
    m_isEditable = editable;
}

bool TaskEditController::isEditable() const
{
    return m_isEditable;
}

void TaskEditController::setTask(Task* task)
{
    if (task == nullptr) {
        return;
    }
    
    m_currentTask = task;
    
    m_ui->taskEditName->setPlainText(task->getName());
    m_ui->taskEditScore->setValue(task->getScore());
    m_ui->taskEditText->setHtml(task->getText());
    m_ui->taskEditInputData->setHtml(task->getInputData());
    m_ui->taskEditOutputData->setHtml(task->getOutputData());
    m_ui->taskEditSource->setHtml(task->getSource());
    
    propose();
}

Task* TaskEditController::getCurrentTask()
{
    return m_currentTask;
}
