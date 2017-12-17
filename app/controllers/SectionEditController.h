#ifndef SECTIONEDITCONTROLLER_H
#define SECTIONEDITCONTROLLER_H

#include "Controller.h"
#include "../objects/Section.h"

class SectionEditController : public Controller
{
    Q_OBJECT
    
public:
    SectionEditController(Ui::MainWindow* ui, QObject* parent = nullptr);
    ~SectionEditController();
    
    void propose() override;
    void setEditable(bool editable);
    bool isEditable() const;
    
    void saveCurrentSection();
    void deleteCurrentSection();
    
    void setSection(Section* section);
    Section* getCurrentSection() const;
    
signals:
    void addSubsectionButtonPressed();
    void addTaskButtonPressed();
    void editButtonPressed();
    void deleteButtonPressed();
    void saveChangesButtonPressed();
    void cancelChangesButtonPressed();
    
private:
    Section* m_currentSection;
    bool m_isEditable;
};

#endif // SECTIONEDITCONTROLLER_H
