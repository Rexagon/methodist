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
    INFO_PANEL_EMPTY
};

enum InfoPanelButtons {
    INFO_PANEL_BUTTONS_DEFAULT,
    INFO_PANEL_BUTTONS_COURSE,
    INFO_PANEL_BUTTONS_SECTION,
    INFO_PANEL_BUTTONS_TASK,
    INFO_PANEL_BUTTONS_TEST,
    INFO_PANEL_BUTTONS_TESTS_TABLE
};

namespace Ui {
class MainWindow;
}

class Controller : public QObject
{
    Q_OBJECT
    
public:
    Controller(Ui::MainWindow* ui, QObject* parent = nullptr);
    virtual ~Controller();
    
    virtual void propose();
    
protected:
    Ui::MainWindow* m_ui;
};

#endif // CONTROLLER_H
