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
    
    void saveChanges();
    void propose() override;
    
public slots:
    void setSection(Section* section);
    
signals:
    void changesSaved();
    void changesCanceled();
    
private:
    Section* m_currentSection;
};

#endif // SECTIONEDITCONTROLLER_H
