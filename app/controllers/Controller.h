#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

enum MainWorkspacePages {
    MAIN_WORKSPACE_DEFAULT,
    MAIN_WORKSPACE_COURSE
};

enum WorkspacePages {
    WORKSPACE_TREE,
    WORKSPACE_TASK,
    WORKSPACE_TESTS
};

enum InfoPanelPages {
    INFO_PANEL_COURSE,
    INFO_PANEL_SECTION,
    INFO_PANEL_TASK,
    INFO_PANEL_TEST,
    INFO_PANEL_TESTS_TABLE
};

enum InfoPanelButtons {
    INFO_PANEL_BUTTONS_COURSE,
    INFO_PANEL_BUTTONS_SECTION,
    INFO_PANEL_BUTTONS_TASK,
    INFO_PANEL_BUTTONS_TEST
};

namespace Ui {
class MainWindow;
}

class Controller : public QObject
{
    Q_OBJECT
    
public:
    virtual ~Controller();
    
    virtual void propose();
    
protected:
    Controller(Ui::MainWindow* ui, QObject* parent = nullptr);
    
    Ui::MainWindow* m_ui;
};

#endif // CONTROLLER_H
