#include "Controller.h"

#include <ui_MainWindow.h>

Controller::Controller(Ui::MainWindow* ui, QObject* parent) :
    QObject(parent), m_ui(ui)
{
}

Controller::~Controller()
{
}

void Controller::propose()
{
}
