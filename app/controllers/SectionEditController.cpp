#include "SectionEditController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"
#include "../stuff/NetworkManager.h"

SectionEditController::SectionEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentSection(nullptr), m_isEditable(false)
{
    connect(m_ui->addSubsectionButton, &QPushButton::pressed, this, &SectionEditController::addSubsectionButtonPressed);
    connect(m_ui->addTaskButton, &QPushButton::pressed, this, &SectionEditController::addTaskButtonPressed);
    connect(m_ui->editSectionButton, &QPushButton::pressed, this, &SectionEditController::editButtonPressed);
    connect(m_ui->deleteSectionButton, &QPushButton::pressed, this, &SectionEditController::deleteButtonPressed);
    connect(m_ui->saveSectionChangesButton, &QPushButton::pressed, this, &SectionEditController::saveChangesButtonPressed);
    connect(m_ui->cancelSectionChangesButton, &QPushButton::pressed, this, &SectionEditController::cancelChangesButtonPressed);
}

SectionEditController::~SectionEditController()
{
}

void SectionEditController::saveCurrentSection()
{
    if (m_currentSection == nullptr) {
        return;
    }
    
    Section* section = m_currentSection;
    
    Section::Data sectionData = section->getData();
    
    sectionData.name = m_ui->sectionEditName->toPlainText();
    
    Section::dbUpdate(section, sectionData, [this, section]() {
        ModelManager::getCourseTreeModel(section->getCourse())->update();
    });
}

void SectionEditController::deleteCurrentSection()
{
    
}

void SectionEditController::propose()
{
    m_ui->infoPanel->setVisible(true);
    
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TREE);
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_SECTION);
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_SECTION);
    
    m_ui->sectionEditName->setEnabled(m_isEditable);
    
    m_ui->addSubsectionButton->setVisible(!m_isEditable);
    m_ui->addTaskButton->setVisible(!m_isEditable);
    m_ui->editSectionButton->setVisible(!m_isEditable);
    m_ui->deleteSectionButton->setVisible(!m_isEditable);
    m_ui->saveSectionChangesButton->setVisible(m_isEditable);
    m_ui->cancelSectionChangesButton->setVisible(m_isEditable);
}

void SectionEditController::setEditable(bool editable)
{
    m_isEditable = editable;
}

bool SectionEditController::isEditable() const
{
    return m_isEditable;
}

void SectionEditController::setSection(Section* section)
{
    if (section == nullptr) {
        return;
    }
    
    m_currentSection = section;
    
    m_ui->sectionEditName->setPlainText(section->getName());
    
    propose();
}

Section* SectionEditController::getCurrentSection() const
{
    return m_currentSection;
}
