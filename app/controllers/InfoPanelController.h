#ifndef INFOPANELCONTROLLER_H
#define INFOPANELCONTROLLER_H

#include "Controller.h"
#include "../objects/CourseNode.h"

class InfoPanelController : public Controller
{
    Q_OBJECT
    
public:
    InfoPanelController(Ui::MainWindow* ui, QObject* parent = nullptr);
    ~InfoPanelController();
    
    void propose() override;
    
public slots:
    void setCourseNode(CourseNode* node);
    
signals:
    void addTaskButtonPressed();
    void addSectionButtonPressed();
    void editNodeButtonPressed();
    void deleteNodeButtonPressed();
    
private:
    void setVisible(bool visible);
    
    CourseNode* m_currentCourseNode;
};

#endif // INFOPANELCONTROLLER_H
