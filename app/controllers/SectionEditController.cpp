#include "SectionEditController.h"

#include <ui_MainWindow.h>

#include "../stuff/ModelManager.h"
#include "../stuff/NetworkManager.h"

SectionEditController::SectionEditController(Ui::MainWindow* ui, QObject* parent) :
    Controller(ui, parent), m_currentSection(nullptr)
{
    connect(m_ui->saveSectionChangesButton, &QPushButton::pressed, this, &SectionEditController::changesSaved);
    connect(m_ui->cancelSectionChangesButton, &QPushButton::pressed, this, &SectionEditController::changesCanceled);
}

SectionEditController::~SectionEditController()
{
}

void SectionEditController::saveChanges()
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

void SectionEditController::propose()
{
    m_ui->mainWorkspace->setCurrentIndex(MAIN_WORKSPACE_COURSE);
    m_ui->workspace->setCurrentIndex(WORKSPACE_TREE);
    m_ui->infoPanelPages->setCurrentIndex(INFO_PANEL_SECTION);
    m_ui->infoPanelButtons->setCurrentIndex(INFO_PANEL_BUTTONS_SECTION);
    
    m_ui->sectionEditName->setEnabled(true);
}

void SectionEditController::setSection(Section* section)
{
    if (section == nullptr) {
        return;
    }
    
    m_currentSection = section;
    
    m_ui->nodeType->setText("Раздел");
    m_ui->sectionEditName->setPlainText(section->getName());
    
    propose();
}
